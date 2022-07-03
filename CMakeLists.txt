#[[****************************************************************************
   *
   *    CMakeLists.txt
   *    This file is part of TMCScan distribution released under GPL 3.0
   *    Copyright (C) 2022 _Magenta_
   *    https://github.com/0Magenta0/TMCScan
   *
   ****************************************************************************]]

cmake_minimum_required (VERSION 3.10)

project (tmcscan
         VERSION 0.6
         DESCRIPTION "TUI Minecraft Port Scanner"
         LANGUAGES C)

set (CMAKE_C_STANDARD 99)
set (CMAKE_C_EXTENSIONS ON)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_BUILD_TYPE DEBUG)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -Wall -Wextra -Wpedantic")

add_subdirectory (scan)
add_subdirectory (tui)
add_executable (${CMAKE_PROJECT_NAME} main.c)

target_link_libraries (${CMAKE_PROJECT_NAME}
                       PRIVATE tmc::tui)

