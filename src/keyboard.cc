/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#include "src/keyboard.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>


const int Keyboard::kNumKeys_ = 16;

// Map from Chip8 key to whether it is pressed;
// static var to be used in glfwSetKeyCallback
std::map<const uint8_t, bool>
Keyboard::key_pressed_flag_ = std::map<const uint8_t, bool>({
  {0x1, false}, {0x2, false}, {0x3, false}, {0xC, false},
  {0x4, false}, {0x5, false}, {0x6, false}, {0xD, false},
  {0x7, false}, {0x8, false}, {0x9, false}, {0xE, false},
  {0xA, false}, {0x0, false}, {0xB, false}, {0xF, false},
});

// Map from key: Chip8 keyboard -- same as above,
// make it static var to be used in glfwSetKeyCallback
std::map<const uint8_t, const uint8_t>
Keyboard::keymap_ = std::map<const uint8_t, const uint8_t>({
  {GLFW_KEY_1, 0x1},  // 1: 1
  {GLFW_KEY_2, 0x2},  // 2: 2
  {GLFW_KEY_3, 0x3},  // 3: 3
  {GLFW_KEY_4, 0xC},  // 4: C
  {GLFW_KEY_Q, 0x4},  // Q: 4
  {GLFW_KEY_W, 0x5},  // W: 5
  {GLFW_KEY_E, 0x6},  // E: 6
  {GLFW_KEY_R, 0xD},  // R: D
  {GLFW_KEY_A, 0x7},  // A: 7
  {GLFW_KEY_S, 0x8},  // S: 8
  {GLFW_KEY_D, 0x9},  // D: 9
  {GLFW_KEY_F, 0xE},  // F: E
  {GLFW_KEY_Z, 0xA},  // Z: A
  {GLFW_KEY_X, 0x0},  // X: 0
  {GLFW_KEY_C, 0xB},  // C: B
  {GLFW_KEY_V, 0xF},  // V: F
});

Keyboard::Keyboard() {}

void Keyboard::QueryInput(
  GLFWwindow* window,
  int key,
  int scancode,
  int action,
  int mods) {
  // Unused
  static_cast<void>(scancode);
  static_cast<void>(mods);

  // Window close on escape
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // Accept only a valid key input (catches the ESC case)
  if (keymap_.find(key) == keymap_.end()) return;

  if (action == GLFW_PRESS) {
    key_pressed_flag_[keymap_[key]] = true;
  } else if (action == GLFW_RELEASE) {
    key_pressed_flag_[keymap_[key]] = false;
  }
}

bool Keyboard::KeyIsPressed(uint8_t key) {
  if (key_pressed_flag_.find(key) == key_pressed_flag_.end()) return false;
  return key_pressed_flag_[key];
}
