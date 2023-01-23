find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(
  chip8
  main.cc
  chip8.cc
  display.cc
  keyboard.cc
  parser.cc
  rom_info.cc
  shader.cc
  sound.cc)

# Graphics
target_link_libraries(chip8 glfw)
target_link_libraries(chip8 GLEW::GLEW)

# MacOS audio
target_link_libraries(chip8 "-framework AudioUnit")

# For generating hashes
find_package(OpenSSL REQUIRED)
target_link_libraries(chip8 OpenSSL::SSL)
