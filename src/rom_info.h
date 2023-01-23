/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#ifndef SRC_ROM_INFO_H_
#define SRC_ROM_INFO_H_

#include <string>
#include <map>


struct RomInfo {
  static std::map<std::string, std::string> info_;
};
#endif  // SRC_ROM_INFO_H_
