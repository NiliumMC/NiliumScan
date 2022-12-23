#[[
CMakeLists.txt
This file is a part of the TMCScan distribution released under GNU GPLv3
Copyright (C) 2022 0Magenta0
https://github.com/TuiMC/TMCScan
]]

CMAKE_MINIMUM_REQUIRED (VERSION 3.10)

IF (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  MESSAGE (FATAL_ERROR "Please create the build directory and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
ENDIF ()


#/////////////////////////////////////////////////////////#
#              General Project Configuration              #
#/////////////////////////////////////////////////////////#

PROJECT (
  tmcscan
    VERSION 3.0
    DESCRIPTION "Advanced TUI Minecraft Port Scanner"
    LANGUAGES C
)

SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET (CMAKE_C_STANDARD 99)
SET (CMAKE_C_EXTENSIONS ON)
SET (CMAKE_C_STANDARD_REQUIRED ON)
SET (CMAKE_BUILD_TYPE RELEASE)


#/////////////////////////////////////////////////////////#
#                   Debug Configuration                   #
#/////////////////////////////////////////////////////////#

IF (ISDEBUG)
  SET (CMAKE_BUILD_TYPE DEBUG)
  SET (
    CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} \
        -Wall -Wextra -Werror -Wpedantic \
        -Wcast-qual -Wconversion -Wcast-align \
        -Wredundant-decls -Wsign-conversion \
        -Waggregate-return -Wswitch-enum"
  )
ENDIF ()

IF (USE_SANITIZE)
  SET (
    CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} \
        -fsanitize=address -fsanitize=leak \
        -fsanitize=undefined"
  )
ENDIF ()


#/////////////////////////////////////////////////////////#
#                      Dependencies                       #
#/////////////////////////////////////////////////////////#

ADD_SUBDIRECTORY (cli)
ADD_SUBDIRECTORY (tui)
ADD_SUBDIRECTORY (scan)
ADD_EXECUTABLE (${CMAKE_PROJECT_NAME} main.c)

TARGET_LINK_LIBRARIES (
  ${CMAKE_PROJECT_NAME}
    PRIVATE tmc::cli
    PRIVATE tmc::tui
    PRIVATE tmc::scan
)

