/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#include <assert.h>

#include <cstdio>
#include <vector>
#include <functional>

#include "src/chip8.h"
#include "src/parser.h"
#include "src/chip8_option.h"


int main(int argc, char* argv[]) {
  /*
    Define options
  */
  Parser parser(argc, argv);

  // `play`: attempt to load and run the ROM
  auto play_option_valid_argument_test = [=](const std::string& path_to_rom) {
    // Defer checking that ROM can be read to Chip8::Run
    return !path_to_rom.empty();
  };
  auto play_option = new Chip8Option<decltype(play_option_valid_argument_test)>(
    {"-p", "--play"},
    play_option_valid_argument_test,
    "  -p (--play) [ /PATH/TO/ROM ]: run a Chip-8 program (ROM); will print\n"
    "    info if it's in the known ROMs.\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(play_option));

  // `info`: attempt to print some info about the ROM
  auto info_option_valid_argument_test = [=](const std::string& path_to_rom) {
    // any nonempty path is potentially path to ROM; defer checking that the ROM
    // and be read, and whether it's in the known ROMs, to Parser::PrintRomInfo
    return !path_to_rom.empty();
  };
  auto info_option = new Chip8Option<decltype(info_option_valid_argument_test)>(
    {"-i", "--info"},
    info_option_valid_argument_test,
    "  -i (--info) [ /PATH/TO/ROM ]: print info about a ROM, if it's in the\n"
    "    known ROMs.\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(info_option));

  // `wy`: wrap around in y direction?
  auto wrap_around_y_option_valid_argument_test
  = [=](const std::string& selection) {
    const std::list<std::string> valid_selections = {"on", "off"};
    return (
      std::find(
        valid_selections.begin(),
        valid_selections.end(),
        selection) != valid_selections.end() );
  };
  auto wrap_around_y_option = new Chip8Option<
    decltype(wrap_around_y_option_valid_argument_test)
  >(
    {"-wy", "--wrap-around-y"},
    wrap_around_y_option_valid_argument_test,
    "  -wy (--wrap-around-y) [ on; off; default=off; ]: whether to wrap\n"
    "    around in y-direction; required `off` for some games to run\n"
    "    correctly (e.g. BLITZ) and `on` for others (e.g. VERS).\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(wrap_around_y_option));

  // `br`: background color (red pixel value)?
  auto pixel_value_option_valid_argument_test
  = [=](const std::string& selection) {
    auto StringIsDigit = [=](const std::string& s) {
      for (auto c : s) { if (!std::isdigit(c)) return false; }
      return true;
    };
    if (!StringIsDigit(selection)) return false;
    const int argument_value = std::stoi(selection);
    return (0 <= argument_value) && (argument_value <= 255);
  };

  auto background_red_pixel_value_option = new Chip8Option<
    decltype(pixel_value_option_valid_argument_test)
  >(
    {"-br", "--background-red-pixel-value"},
    pixel_value_option_valid_argument_test,
    "  -br (--background-red-pixel-value) [ integer in the range [0, 255];\n"
    "    default=0; ]: red pixel value for background.\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(background_red_pixel_value_option));

  // `bg`: background color (green pixel value)?
  auto background_green_pixel_value_option = new Chip8Option<
    decltype(pixel_value_option_valid_argument_test)
  >(
    {"-bg", "--background-green-pixel-value"},
    pixel_value_option_valid_argument_test,
    "  -bg (--background-green-pixel-value) [ integer in the range [0, 255];\n"
    "    default=0; ]: green pixel value for background.\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(background_green_pixel_value_option));

  // `bb`: background color (blue pixel value)?
  auto background_blue_pixel_value_option = new Chip8Option<
    decltype(pixel_value_option_valid_argument_test)
  >(
    {"-bb", "--background-blue-pixel-value"},
    pixel_value_option_valid_argument_test,
    "  -bb (--background-blue-pixel-value) [ integer in the range [0, 255];\n"
    "    default=0; ]: blue pixel value for background.\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(background_blue_pixel_value_option));

  // `fr`: foreground color (red pixel value)?
  auto foreground_red_pixel_value_option = new Chip8Option<
    decltype(pixel_value_option_valid_argument_test)
  >(
    {"-fr", "--foreground-red-pixel-value"},
    pixel_value_option_valid_argument_test,
    "  -fr (--foreground-red-pixel-value) [ integer in the range [0, 255];\n"
    "    default=1; ]: red pixel value for foreground.\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(foreground_red_pixel_value_option));

  // `fg`: foreground color (green pixel value)?
  auto foreground_green_pixel_value_option = new Chip8Option<
    decltype(pixel_value_option_valid_argument_test)
  >(
    {"-fg", "--foreground-green-pixel-value"},
    pixel_value_option_valid_argument_test,
    "  -fg (--foreground-green-pixel-value) [ integer in the range [0, 255];\n"
    "    default=1; ]: green pixel value for foreground.\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(foreground_green_pixel_value_option));

  // `fb`: foreground color (blue pixel value)?
  auto foreground_blue_pixel_value_option = new Chip8Option<
    decltype(pixel_value_option_valid_argument_test)
  >(
    {"-fb", "--foreground-blue-pixel-value"},
    pixel_value_option_valid_argument_test,
    "  -fb (--foreground-blue-pixel-value) [ integer in the range [0, 255];\n"
    "    default=1; ]: blue pixel value for foreground.\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(foreground_blue_pixel_value_option));

  // `cs`: color scheme?
  auto color_scheme_option_valid_argument_test
  = [=](const std::string& selection) {
    const std::list<std::string> valid_selections = {
      "bw", "black-white",
      "wb", "white-black",
      "gr", "grays",
      "gb", "gameboy",
      "blw", "blue-white",
    };
    return (
      std::find(
        valid_selections.begin(),
        valid_selections.end(),
        selection) != valid_selections.end() );
  };

  auto color_scheme_option = new Chip8Option<
    decltype(color_scheme_option_valid_argument_test)
  >(
    {"-cs", "--color-scheme"},
    color_scheme_option_valid_argument_test,
    "  -cs (--color-scheme) [ black-white (bw), white-black (wb),\n"
    "    grays (gr), gameboy (gb), blue-white (blw); default=bw ]:\n"
    "    color scheme for background and foreground.\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(color_scheme_option));

  // `help`: print all options
  auto help_option_valid_argument_test = [=](){ return true; };
  auto help_option = new Chip8Option<decltype(help_option_valid_argument_test)>(
    {"-h", "--help"},
    help_option_valid_argument_test,
    "  -h (--help): print this menu :-)\n");
  parser.chip8_options_.push_back(
    std::unique_ptr<Chip8OptionInterface>(help_option));

  /*
    Parse command line input
  */
  if ( parser.IsCommandLineOption(info_option->aliases_)
    || parser.IsCommandLineOption(play_option->aliases_) ) {
    // `info` or `play` mode; print info in either case
    const std::string info_or_play_flag = (
      !parser.WhichCommandLineOption(info_option->aliases_).empty()
      ?parser.WhichCommandLineOption(info_option->aliases_)
      :parser.WhichCommandLineOption(play_option->aliases_) );
    const std::string path_to_rom = parser.GetCommandLineOptionArgument(
      info_or_play_flag);

    if ( !info_option->ArgumentIsValid(path_to_rom)
      || !play_option->ArgumentIsValid(path_to_rom) ) {
      std::printf("Invalid usage of Chip8 options; correct usage:\n");
      if (std::find(
        info_option->aliases_.begin(),
        info_option->aliases_.end(),
        info_or_play_flag) != info_option->aliases_.end() ) {
        info_option->PrintHelp();
      } else {
        play_option->PrintHelp();
      }
      return 0;
    }

    parser.PrintRomInfo(path_to_rom);

    if ( parser.IsCommandLineOption(play_option->aliases_)
      && play_option->ArgumentIsValid(path_to_rom) ) {
      Chip8 chip8;

      /*
        Configure chip8
      */
      if ( parser.IsCommandLineOption(wrap_around_y_option->aliases_) ) {
        const std::string wrap_around_y_flag = parser.WhichCommandLineOption(
          wrap_around_y_option->aliases_);
        const std::string wrap_around_y = parser.GetCommandLineOptionArgument(
          wrap_around_y_flag);

        if (!wrap_around_y_option->ArgumentIsValid(wrap_around_y)) {
          std::printf("Invalid usage of Chip8 options; correct usage:\n");
          wrap_around_y_option->PrintHelp();
          return 0;
        }

        chip8.wrap_around_y_ = (wrap_around_y == "on" ? true : false);
      }

      if ( parser.IsCommandLineOption(
        background_red_pixel_value_option->aliases_) ) {
        const std::string background_red_pixel_value_flag
        = parser.WhichCommandLineOption(
          background_red_pixel_value_option->aliases_);
        const std::string background_red_pixel_value
        = parser.GetCommandLineOptionArgument(
          background_red_pixel_value_flag);

        if ( !background_red_pixel_value_option->ArgumentIsValid(
          background_red_pixel_value) ) {
          std::printf("Invalid usage of Chip8 options; correct usage:\n");
          background_red_pixel_value_option->PrintHelp();
          return 0;
        }

        chip8.background_red_pixel_value_ = std::stoi(
          background_red_pixel_value);
      }

      if ( parser.IsCommandLineOption(
        background_green_pixel_value_option->aliases_) ) {
        const std::string background_green_pixel_value_flag
        = parser.WhichCommandLineOption(
          background_green_pixel_value_option->aliases_);
        const std::string background_green_pixel_value
        = parser.GetCommandLineOptionArgument(
          background_green_pixel_value_flag);

        if ( !background_green_pixel_value_option->ArgumentIsValid(
          background_green_pixel_value) ) {
          std::printf("Invalid usage of Chip8 options; correct usage:\n");
          background_green_pixel_value_option->PrintHelp();
          return 0;
        }

        chip8.background_green_pixel_value_ = std::stoi(
          background_green_pixel_value);
      }

      if ( parser.IsCommandLineOption(
        background_blue_pixel_value_option->aliases_) ) {
        const std::string background_blue_pixel_value_flag
        = parser.WhichCommandLineOption(
          background_blue_pixel_value_option->aliases_);
        const std::string background_blue_pixel_value
        = parser.GetCommandLineOptionArgument(
          background_blue_pixel_value_flag);

        if ( !background_blue_pixel_value_option->ArgumentIsValid(
          background_blue_pixel_value) ) {
          std::printf("Invalid usage of Chip8 options; correct usage:\n");
          background_blue_pixel_value_option->PrintHelp();
          return 0;
        }

        chip8.background_blue_pixel_value_ = std::stoi(
          background_blue_pixel_value);
      }

      if ( parser.IsCommandLineOption(
        foreground_red_pixel_value_option->aliases_) ) {
        const std::string foreground_red_pixel_value_flag
        = parser.WhichCommandLineOption(
          foreground_red_pixel_value_option->aliases_);
        const std::string foreground_red_pixel_value
        = parser.GetCommandLineOptionArgument(
          foreground_red_pixel_value_flag);

        if ( !foreground_red_pixel_value_option->ArgumentIsValid(
          foreground_red_pixel_value) ) {
          std::printf("Invalid usage of Chip8 options; correct usage:\n");
          foreground_red_pixel_value_option->PrintHelp();
          return 0;
        }

        chip8.foreground_red_pixel_value_ = std::stoi(
          foreground_red_pixel_value);
      }

      if ( parser.IsCommandLineOption(
        foreground_green_pixel_value_option->aliases_) ) {
        const std::string foreground_green_pixel_value_flag
        = parser.WhichCommandLineOption(
          foreground_green_pixel_value_option->aliases_);
        const std::string foreground_green_pixel_value
        = parser.GetCommandLineOptionArgument(
          foreground_green_pixel_value_flag);

        if ( !foreground_green_pixel_value_option->ArgumentIsValid(
          foreground_green_pixel_value) ) {
          std::printf("Invalid usage of Chip8 options; correct usage:\n");
          foreground_green_pixel_value_option->PrintHelp();
          return 0;
        }

        chip8.foreground_green_pixel_value_ = std::stoi(
          foreground_green_pixel_value);
      }

      if ( parser.IsCommandLineOption(
        foreground_blue_pixel_value_option->aliases_) ) {
        const std::string foreground_blue_pixel_value_flag
        = parser.WhichCommandLineOption(
          foreground_blue_pixel_value_option->aliases_);
        const std::string foreground_blue_pixel_value
        = parser.GetCommandLineOptionArgument(
          foreground_blue_pixel_value_flag);

        if ( !foreground_blue_pixel_value_option->ArgumentIsValid(
          foreground_blue_pixel_value) ) {
          std::printf("Invalid usage of Chip8 options; correct usage:\n");
          foreground_blue_pixel_value_option->PrintHelp();
          return 0;
        }

        chip8.foreground_blue_pixel_value_ = std::stoi(
          foreground_blue_pixel_value);
      }

      if ( parser.IsCommandLineOption(color_scheme_option->aliases_) ) {
        const std::string color_scheme_flag = parser.WhichCommandLineOption(
          color_scheme_option->aliases_);
        const std::string color_scheme = parser.GetCommandLineOptionArgument(
          color_scheme_flag);

        if (!color_scheme_option->ArgumentIsValid(color_scheme)) {
          std::printf("Invalid usage of Chip8 options; correct usage:\n");
          color_scheme_option->PrintHelp();
          return 0;
        }

        if (color_scheme == "bw" || color_scheme == "black-white") {
          chip8.foreground_red_pixel_value_ = 255;
          chip8.foreground_green_pixel_value_ = 255;
          chip8.foreground_blue_pixel_value_ = 255;
          chip8.background_red_pixel_value_ = 0;
          chip8.background_green_pixel_value_ = 0;
          chip8.background_blue_pixel_value_ = 0;
        } else if (color_scheme == "wb" || color_scheme == "white-black") {
          chip8.foreground_red_pixel_value_ = 0;
          chip8.foreground_green_pixel_value_ = 0;
          chip8.foreground_blue_pixel_value_ = 0;
          chip8.background_red_pixel_value_ = 255;
          chip8.background_green_pixel_value_ = 255;
          chip8.background_blue_pixel_value_ = 255;
        } else if (color_scheme == "gr" || color_scheme == "grays") {
          chip8.foreground_red_pixel_value_ = 84;
          chip8.foreground_green_pixel_value_ = 84;
          chip8.foreground_blue_pixel_value_ = 84;
          chip8.background_red_pixel_value_ = 169;
          chip8.background_green_pixel_value_ = 169;
          chip8.background_blue_pixel_value_ = 169;
        } else if (color_scheme == "gb" || color_scheme == "gameboy") {
          chip8.foreground_red_pixel_value_ = 15;
          chip8.foreground_green_pixel_value_ = 56;
          chip8.foreground_blue_pixel_value_ = 15;
          chip8.background_red_pixel_value_ = 155;
          chip8.background_green_pixel_value_ = 188;
          chip8.background_blue_pixel_value_ = 15;
        } else if (color_scheme == "blw" || color_scheme == "blue-white") {
          chip8.foreground_red_pixel_value_ = 5;
          chip8.foreground_green_pixel_value_ = 65;
          chip8.foreground_blue_pixel_value_ = 255;
          chip8.background_red_pixel_value_ = 242;
          chip8.background_green_pixel_value_ = 242;
          chip8.background_blue_pixel_value_ = 242;
        }
      }

      /*
        Run the ROM
      */
      chip8.Run(path_to_rom);
    }

    return 0;
  }

  if ( parser.IsCommandLineOption(help_option->aliases_)
    && help_option->ArgumentIsValid() ) {
    parser.PrintHelp();
    return 0;
  }

  /*
    Default
  */
  std::printf("Unrecognized options; see the help menu:\n");
  parser.PrintHelp();
  return 0;
}
