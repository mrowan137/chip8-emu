/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#ifndef SRC_CHIP8_OPTION_H_
#define SRC_CHIP8_OPTION_H_

#include <string>
#include <list>


class Chip8OptionInterface {
 public:
  virtual void PrintHelp() const = 0;
};

template<typename T>
class Chip8Option : public Chip8OptionInterface {
 public:
  Chip8Option(
    const std::list<std::string> aliases,
    const T& ArgumentIsValidLambda,
    const std::string help_message)
    : aliases_(aliases),
      ArgumentIsValid(ArgumentIsValidLambda),
      help_message_(help_message) {
  }

  const std::list<std::string> aliases_;

  const T& ArgumentIsValid;

  const std::string help_message_;

  void PrintHelp() const {
    std::printf("%s", help_message_.c_str());
  }
};
#endif  // SRC_CHIP8_OPTION_H_
