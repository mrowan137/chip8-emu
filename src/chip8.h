/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#ifndef SRC_CHIP8_H_
#define SRC_CHIP8_H_

#include <string>

#include "src/keyboard.h"
#include "src/sound.h"
#include "src/display.h"

#ifdef NDEBUG
#  define DEBUG 0
#else
#  define DEBUG 1
#endif


class Chip8 {
 public:
  Chip8();
  ~Chip8();

  // Memory
  static const int kMemorySize_;
  uint8_t* memory_;
  uint16_t index_;

  // Registers
  static const int kRegistersSize_, kStackSize_;
  uint8_t* v_;
  uint16_t* stack_;
  uint8_t sp_;
  uint16_t pc_;

  // Instructions
  inline void UnknownInstruction(const uint16_t opcode);
  void InterpretInstruction(const uint16_t opcode);

  // Keyboard
  Keyboard* keyboard_;

  // Display
  static const int kRows_, kCols_, kPixelSize_, kChannels_;
  uint8_t** pixel_buffer_;
  uint8_t* display_buffer_;
  Display* display_;

  bool TogglePixel(int i, int j);
  void ClearPixelBuffer();
  void ClearDisplayBuffer();
  void DrawPixelToDisplayBuffer(const int& i, const int& j);
  bool DrawSpriteToPixelBuffer(
    const uint8_t& vx,
    const uint8_t& vy,
    const uint8_t& n);
  void DrawPixelsToDisplayBuffer();

  static const int kFontSpritesAddress_;
  static const int kBytesPerFontSprite_;
  static const int kNumFontSprites_;
  static const uint8_t kFontSprites_[];
  void LoadFontSprites();
  static const int kFramesPerSecond_;
  static const double kSecondsPerFrame_;
  static clock_t then_;

  // Timers
  uint8_t delay_timer_, sound_timer_;

  // Sound
  Sound* sound_;

  // Program
  static const int kProgramAddress_;
  static const int kMaxProgramSize_;

  void LoadProgram(const std::string& path_to_rom);
  static void LoadProgram(const std::string& path_to_rom,
                          unsigned char* buffer);
  void Run(const std::string& path_to_rom);
  void Step();
  void EmulateCycle();
  void UpdateTimers();
  void PlaySound();
  void Paint();

  // Configuration
  uint16_t speed_;
  bool wrap_around_y_;
  uint8_t foreground_red_pixel_value_,
    foreground_green_pixel_value_,
    foreground_blue_pixel_value_;
  uint8_t background_red_pixel_value_,
    background_green_pixel_value_,
    background_blue_pixel_value_;

  // Debugging
  template <typename _ = void, typename... Args>
  inline void DebugMessage(const char* message, Args... args) {
    if (DEBUG) std::fprintf(stderr, message, args...);
    return;
  }

  template <typename _ = void>
  inline void DebugMessage(const char* message) {
    if (DEBUG) std::fprintf(stderr, "%s", message);
    return;
  }

  inline void DebugScreen() {
    if (DEBUG) {
      std::fprintf(stderr, "\n    ");  // Indent
      for (int j = 0; j < (kCols_ - 6)/2; ++j) std::fprintf(stderr, " ");
      std::fprintf(stderr, "SCREEN");
      for (int j = 0; j < (kCols_ - 6)/2; ++j) std::fprintf(stderr, " ");
      std::fprintf(stderr, "\n");
      for (int i = 0; i < kRows_; ++i) {
        std::fprintf(stderr, "    ");  // Indent
        for (int j = 0; j < kCols_; ++j) {
          std::fprintf(stderr, "%s",
                       pixel_buffer_[i][j] == 1 ? "\u25A0" : "\u25A1");
        }
        std::fprintf(stderr, "\n");
      }
      std::fprintf(stderr, "\n");
    }
    return;
  }
};

#endif  // SRC_CHIP8_H_
