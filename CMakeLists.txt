cmake_minimum_required(VERSION 3.9)
project(chip8)

set(CMAKE_CXX_STANDARD 11)
if (UNIX)
  set (CMAKE_CXX_FLAGS "-Wall -Wextra")
endif ()

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(src)
