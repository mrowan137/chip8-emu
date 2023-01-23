/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#ifndef SRC_KEYBOARD_H_
#define SRC_KEYBOARD_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>


class Keyboard {
 public:
  Keyboard();

  static const int kNumKeys_;
  static std::map<const uint8_t, const uint8_t> keymap_;
  static std::map<const uint8_t, bool> key_pressed_flag_;

  static void QueryInput(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods);

  bool KeyIsPressed(uint8_t keycode);
};
#endif  // SRC_KEYBOARD_H_
