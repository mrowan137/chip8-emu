/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#include "src/display.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "src/keyboard.h"


Display::Display(const int display_rows,
                 const int display_cols,
                 uint8_t* display_buffer,
                 Keyboard* keyboard)
  : display_rows_(display_rows),
    display_cols_(display_cols),
    keyboard_(keyboard) {
  // Initialize GLFW
  glewExperimental = true;  // Needed for core profile
  if (!glfwInit()) {
    std::fprintf(stderr, "Failed to initialize GLFW\n");
    std::exit(EXIT_FAILURE);
  }

  // Enable 4x MSAA ("multisample anti-aliasing");
  // each pixel of window's buffer consists of 4 pixels
  glfwWindowHint(GLFW_SAMPLES, 4);

  // Sticking with OpenGL version 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Specify which OpenGL profile to create context for;
  // this selection hides 'old' OpenGL
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  // Set to true because MacOS only supports forward-compatible
  // core profile context for OpenGL 3.2 and above
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Open a window and initialize its OpenGL context
  window_ = glfwCreateWindow(
    display_cols,
    display_rows,
    "Chip8-Emu",
    NULL,
    NULL);
  if (window_ == NULL) {
    std::fprintf(stderr, "Failed to open GLFW window.\n");
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window_);

  // Keyboard input
  glfwSetKeyCallback(window_, keyboard_->QueryInput);

  // Resizing
  glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

  // Allows GLEW to access modern OpenGL extensions, set
  // to true for OpenGL contexts version 3.2 and above
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr, "Failed to initialize GLEW.\n");
    std::exit(EXIT_FAILURE);
  }

  // Compile GLSL shaders program
  shader_ = new Shader();

  // Vertex data for 'quad' with position, color, and texture coordinates;
  // this will be the display region for the pixel buffer
  GLfloat vertices[] = {
    // Positions             Colors              Texture coordinates
     0.95f,  0.9f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // top right
     0.95f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // bottom right
    -0.95f, -0.9f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // bottom left
    -0.95f,  0.9f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f   // top left
    // (margin control)
  };

  GLuint elements[] = {
    0, 1, 3,
    1, 2, 3
  };

  // Load texture image
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(vertices),
    vertices,
    GL_DYNAMIC_DRAW);

  glGenBuffers(1, &ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    sizeof(elements),
    elements,
    GL_DYNAMIC_DRAW);

  // Specify layout of position attribute
  glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    8*sizeof(float),
    reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  // Specify layout of color attribute
  glVertexAttribPointer(
    1,
    3,
    GL_FLOAT,
    GL_FALSE,
    8*sizeof(float),
    reinterpret_cast<void*>(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // Specify layout of texture coordinate attribute
  glVertexAttribPointer(
    2,
    2,
    GL_FLOAT,
    GL_FALSE,
    8*sizeof(float),
    reinterpret_cast<void*>(6*sizeof(float)));
  glEnableVertexAttribArray(2);

  // Configure texture properties
  glGenTextures(1, &tex_);
  glBindTexture(GL_TEXTURE_2D, tex_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Save the display buffer
  display_buffer_ = display_buffer;

  // Allow to capture escape key press event
  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
}

Display::~Display() {
  // Cleanup
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ebo_);
  delete shader_;

  glfwTerminate();
}

void Display::Paint() {
  // Clear the screen, can cause flickering (?)
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Paint
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               display_cols_, display_rows_,
               0, GL_RGB, GL_UNSIGNED_BYTE, display_buffer_);
  glBindTexture(GL_TEXTURE_2D, tex_);

  // Use custom shader
  shader_->Use();

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  // Swap buffers
  glfwSwapBuffers(window_);
  glfwPollEvents();
}

void Display::FramebufferSizeCallback(GLFWwindow* window,
                                      const GLsizei display_rows,
                                      const GLsizei display_cols) {
  // Unused
  static_cast<void>(window);

  // Callback to make viewport match window size; may be changed by user or OS
  glViewport(0, 0, display_rows, display_cols);
}

bool Display::ShouldClose() {
  return glfwWindowShouldClose(window_);
}
