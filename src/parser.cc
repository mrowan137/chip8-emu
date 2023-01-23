/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#include "src/parser.h"

#include <sstream>
#include <iomanip>

#include "src/chip8.h"


Parser::Parser(int argc, char* argv[]) {
  for (int i = 1 ; i < argc; ++i) tokens_.push_back(std::string(argv[i]));
  rom_info_ = RomInfo();
}

const std::string&
Parser::GetCommandLineOptionArgument(const std::string& option) const {
  // get the argument that corresponds to the option: `--option argument`
  std::vector<std::string>::const_iterator it;
  it = std::find(tokens_.begin(), tokens_.end(), option);
  if (it != tokens_.end() && ++it != tokens_.end()) return *it;
  static const std::string empty("");
  return empty;
}

const std::string
Parser::WhichCommandLineOption(const std::list<std::string>& aliases) {
  for (const std::string& alias : aliases) {
    if (std::find(tokens_.begin(), tokens_.end(), alias) != tokens_.end()) {
      return alias;
    }
  }
  return  "";
}

bool Parser::IsCommandLineOption(const std::list<std::string>& aliases) const {
  for (const std::string& alias : aliases) {
    if (std::find(tokens_.begin(), tokens_.end(), alias) != tokens_.end()) {
      return true;
    }
  }
  return false;
}

void Parser::PrintRomInfo(const std::string& path_to_rom) const {
  if (!path_to_rom.empty()) {
    // if hash is in the keys, provide some info
    uint8_t rom_hash[SHA_DIGEST_LENGTH];
    uint8_t* buffer = new uint8_t[Chip8::kMaxProgramSize_]();
    Chip8::LoadProgram(path_to_rom, buffer);
    SHA1(buffer, Chip8::kMaxProgramSize_, rom_hash);
    std::stringstream rom_hash_key;
    rom_hash_key << std::hex;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
      rom_hash_key << std::setw(2)
      << std::setfill('0')
      << static_cast<unsigned int>(rom_hash[i]);
    }

    std::printf("Checking known ROMs for hash: %s\n",
           rom_hash_key.str().c_str(), SHA_DIGEST_LENGTH);
    if (rom_info_.info_.find(rom_hash_key.str())
        != rom_info_.info_.end()) {
      std::printf("ROM identified:\n"
             "%s",
             rom_info_.info_[rom_hash_key.str()].c_str());
    } else {
      std::printf("No info about this file: %s\n", path_to_rom.c_str());
    }

    delete[] buffer;
  } else {
    std::fprintf(stderr,
      "In Parser::PrintRomInfo: Expected a path to ROM, got empty string;\n"
      "no ROM info to print.\n");
  }
  return;
}

void Parser::PrintHelp() const {
  std::printf(
    "\n"
    "Controls:\n"
    "  ESC: quit Chip8-Emu.\n"
    "  Mapping from user keyboard to Chip8-Emu keyboard:\n"
    "\n"
    "    |1|2|3|4|     |1|2|3|c|\n"
    "    |q|w|e|r| --\\ |4|5|6|d|\n"
    "    |a|s|d|f| --/ |7|8|9|e|\n"
    "    |z|x|c|v|     |a|0|b|f|\n"
    "\n"
    "Supported Chip8-Emu options:\n");

  for (const auto& option : chip8_options_) option->PrintHelp();

  std::printf("\n");
}
