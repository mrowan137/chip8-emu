/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include <openssl/sha.h>

#include <string>
#include <list>
#include <vector>
#include <memory>

#include "src/rom_info.h"
#include "src/chip8_option.h"


class Parser {
 public:
  Parser(int argc, char* argv[]);

  const std::string&
  GetCommandLineOptionArgument(const std::string& option) const;

  bool IsCommandLineOption(const std::list<std::string>& aliases) const;

  const std::string
  WhichCommandLineOption(const std::list<std::string>& aliases);

  template<typename T>
  bool IsCommandLineOption(T option) const {
    return (
      std::find(tokens_.begin(), tokens_.end(), option)
      != tokens_.end());
  }

  template<typename T, typename... Args>
  bool IsCommandLineOption(T option, Args... other_options) const {
    return (
      std::find(tokens_.begin(), tokens_.end(), option)
      != tokens_.end()) || IsCommandLineOption(other_options...);
  }

  template<typename T>
  std::string WhichCommandLineOption(T option) const {
    return (
      std::find(tokens_.begin(), tokens_.end(), option) != tokens_.end()
      ? option
      : "");
  }

  template<typename T, typename... Args>
  std::string WhichCommandLineOption(T option, Args... other_options) const {
    return (
      std::find(tokens_.begin(), tokens_.end(), option) != tokens_.end()
      ? option
      : WhichCommandLineOption(other_options...));
  }

  RomInfo rom_info_;
  void PrintRomInfo(const std::string& path_to_rom) const;

  std::vector<std::unique_ptr<const Chip8OptionInterface> > chip8_options_;
  void PrintHelp() const;

 private:
  std::vector<std::string> tokens_;
};
#endif  // SRC_PARSER_H_
