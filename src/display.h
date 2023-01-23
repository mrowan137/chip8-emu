/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "src/keyboard.h"
#include "src/shader.h"


class Display {
 public:
  Display(
    const int display_rows,
    const int display_cols,
    uint8_t* display_buffer,
    Keyboard* keyboard);
  ~Display();

  const int display_rows_;
  const int display_cols_;
  uint8_t* display_buffer_;

  GLFWwindow* window_;

  GLuint vao_;
  GLuint vbo_;
  GLuint ebo_;
  GLuint tex_;

  Shader* shader_;

  void Paint();
  bool ShouldClose();

  static void FramebufferSizeCallback(
    GLFWwindow* window,
    const int rows,
    const int cols);

  Keyboard* keyboard_;
};

#endif  // SRC_DISPLAY_H_
