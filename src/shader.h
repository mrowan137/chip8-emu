/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#ifndef SRC_SHADER_H_
#define SRC_SHADER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>


class Shader {
 public:
  Shader();
  void Use();
  GLint CheckShaderErrors(GLuint shader_id, const std::string);

 private:
  static const char* kVertex_shader_code_;
  static const char* kFragment_shader_code_;
  GLuint program_id_;
};

#endif  // SRC_SHADER_H_
