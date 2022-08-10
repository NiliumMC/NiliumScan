#[[****************************************************************************
   *
   *    cli/CMakeLists.txt
   *    This file is part of TMCScan distribution released under GPL 3.0
   *    Copyright (C) 2022 _Magenta_
   *    https://github.com/0Magenta0/TMCScan
   *
   ****************************************************************************]]

project (cli)

set (CMAKE_USE_PTHREADS_INIT ON)
include (FindThreads)

set (TUI_SOURCES
     src/cli_parameters.c
     src/cli_help.c
     src/cli.c)

add_library (${PROJECT_NAME} ${TUI_SOURCES})
add_library (tmc::cli ALIAS ${PROJECT_NAME})

configure_file (${PROJECT_SOURCE_DIR}/include/cli/cli_help.h.in ${PROJECT_SOURCE_DIR}/include/cli/cli_help.h NEWLINE_STYLE UNIX)

target_include_directories (${PROJECT_NAME}
                            PUBLIC ${PROJECT_SOURCE_DIR}/include
                            PRIVATE ${CURSES_INCLUDE_DIRS})

target_link_libraries (${PROJECT_NAME}
                       PRIVATE Threads::Threads
                       PRIVATE tmc::scan)

