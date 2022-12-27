#--------------==================================================--------------#
#  CMakeLists.txt                                                              #
#  This file is a part of the TMCScan distribution released under GNU GPLv3    #
#  Copyright (C) 2022  0Magenta0                                               #
#  https://github.com/TuiMC/TMCScan                                            #
#--------------==================================================--------------#

CMAKE_MINIMUM_REQUIRED (VERSION 3.10 FATAL_ERROR)

IF (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  MESSAGE (FATAL_ERROR "Please create the build directory and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
ENDIF ()


#--------=========================================--------#
#              General Project Configuration              #
#--------=========================================--------#
PROJECT (
  tmcscan
    VERSION 3.0
    DESCRIPTION "Advanced TUI Minecraft Port Scanner"
    LANGUAGES C
)

SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET (CMAKE_C_STANDARD 11)
SET (CMAKE_C_EXTENSIONS OFF)
SET (CMAKE_C_STANDARD_REQUIRED ON)

IF (NOT CMAKE_BUILD_TYPE)
  MESSAGE (STATUS "Build type is not specified! Setting up to Release.")
  SET (CMAKE_BUILD_TYPE Release)
ENDIF (NOT CMAKE_BUILD_TYPE)


#--------=========================================--------#
#                   Debug Configuration                   #
#--------=========================================--------#
IF (CMAKE_BUILD_TYPE MATCHES DEBUG)
  MESSAGE (STATUS "CMAKE_BUILD_TYPE is Debug! Setting up the Warning flags...")
  SET (
    CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} \
        -Wall -Wextra -Werror -Wpedantic \
        -Wcast-qual -Wconversion -Wcast-align \
        -Wredundant-decls -Wsign-conversion \
        -Waggregate-return -Wswitch-enum"
  )
ENDIF (CMAKE_BUILD_TYPE MATCHES DEBUG)

# TODO: Make it with option
IF (USE_SANITIZE)
  MESSAGE (STATUS "USE_SANITIZE is ON! Setting up the Sanitize flags...")
  SET (
    CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} \
        -fsanitize=address -fsanitize=leak \
        -fsanitize=undefined"
  )
ENDIF (USE_SANITIZE)


#--------=========================================--------#
#                      Dependencies                       #
#--------=========================================--------#
ADD_EXECUTABLE (${CMAKE_PROJECT_NAME} main.c)

