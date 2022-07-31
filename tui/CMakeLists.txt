#[[****************************************************************************
   *
   *    tui/CMakeLists.txt
   *    This file is part of TMCScan distribution released under GPL 3.0
   *    Copyright (C) 2022 _Magenta_
   *    https://github.com/0Magenta0/TMCScan
   *
   ****************************************************************************]]

project (tui)

set (CURSES_NEED_NCURSES ON)
set (CURSES_NEED_WIDE ON)
include (FindCurses)

set (CMAKE_USE_PTHREADS_INIT ON)
include (FindThreads)

set (TUI_SOURCES
     src/tui_filter.c
     src/tui_params.c
     src/tui_utils.c
     src/tui_about.c
     src/tui_acts.c
     src/tui_scan.c
     src/tui_quit.c
     src/tui.c)

add_library (${PROJECT_NAME} ${TUI_SOURCES})
add_library (tmc::tui ALIAS ${PROJECT_NAME})

configure_file (${PROJECT_SOURCE_DIR}/include/tui/tui_about.h.in ${PROJECT_SOURCE_DIR}/include/tui/tui_about.h NEWLINE_STYLE UNIX)

target_include_directories (${PROJECT_NAME}
                            PUBLIC ${PROJECT_SOURCE_DIR}/include
                            PRIVATE ${CURSES_INCLUDE_DIRS})

target_link_libraries (${PROJECT_NAME}
                       PRIVATE ${CURSES_LIBRARIES}
                       PRIVATE Threads::Threads
                       PRIVATE tmc::scan)

