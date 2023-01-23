/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#include "src/shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <string>
#include <vector>


const char* Shader::kVertex_shader_code_
  = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 2) in vec2 tex_coord;\n"
    "out vec2 frag_tex_coord;\n"
    "void main() {\n"
    "  gl_Position = vec4(position, 1.0);\n"
    "  frag_tex_coord = vec2(tex_coord.x, tex_coord.y);\n"
    "}\n";

const char* Shader::kFragment_shader_code_
  = "#version 330 core\n"
    "in vec2 frag_tex_coord;\n"
    "layout (location = 0) out vec4 out_color;\n"
    "uniform sampler2D tex;\n"
    "void main() {\n"
    "  out_color = texture(tex, frag_tex_coord);\n"
    "}\n";

Shader::Shader() {
  // Create the shaders on startup;
  // ensures program_id_ is set so shader can be used
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  GLint success = GL_FALSE;

  // Vertex shader
  std::printf("Compiling vertex shader... ");
  glShaderSource(vertex_shader_id, 1, &kVertex_shader_code_ , NULL);
  glCompileShader(vertex_shader_id);
  success = CheckShaderErrors(vertex_shader_id, "shader");
  if (!success) {
    std::fprintf(stderr, "vertex shader compilation failed.\n");
    std::exit(EXIT_FAILURE);
  }
  std::printf("SUCCESS\n");

  // Fragment Shader
  std::printf("Compiling fragment shader... ");
  glShaderSource(fragment_shader_id, 1, &kFragment_shader_code_ , NULL);
  glCompileShader(fragment_shader_id);
  success = CheckShaderErrors(fragment_shader_id, "shader");
  if (!success) {
    std::fprintf(stderr, "fragment shader compilation failed.\n");
    std::exit(EXIT_FAILURE);
  }
  std::printf("SUCCESS\n");

  // Link the program
  std::printf("Linking shader program... ");
  program_id_ = glCreateProgram();
  glAttachShader(program_id_, vertex_shader_id);
  glAttachShader(program_id_, fragment_shader_id);
  glLinkProgram(program_id_);
  success = CheckShaderErrors(program_id_, "program");
  if (!success) {
    std::fprintf(stderr, "shader linking failed.\n");
    std::exit(EXIT_FAILURE);
  }
  std::printf("SUCCESS\n");

  // Compilation and linking successful, shaders may be deleted
  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
}

void Shader::Use() {
  glUseProgram(program_id_);
}

GLint Shader::CheckShaderErrors(GLuint id, const std::string type) {
  GLint result = GL_FALSE;
  int info_log_length;
  std::vector<char> error_message;

  if (type == "shader") {
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
      error_message.resize(info_log_length+1);
      glGetShaderInfoLog(id, info_log_length, NULL, &error_message[0]);
      std::fprintf(stderr, "%s", &error_message[0]);
    }
  } else if (type == "program") {
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
      error_message.resize(info_log_length+1);
      glGetProgramInfoLog(id, info_log_length, NULL, &error_message[0]);
      std::fprintf(stderr, "%s", &error_message[0]);
    }
  }

  return result;
}
