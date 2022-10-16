#[[
tui/CMakeLists.txt
This file is a part of the TMCScan distribution released under GNU GPLv3
Copyright (C) 2022 _Magenta_
https://github.com/0Magenta0/TMCScan
]]

project (tmcscan-tui)

set (CURSES_NEED_NCURSES ON)
set (CURSES_NEED_WIDE ON)
include (FindCurses)

add_library (${PROJECT_NAME} OBJECT)
add_library (tmc::tui ALIAS ${PROJECT_NAME})

target_sources (
  ${PROJECT_NAME} PRIVATE
    src/tui.c
)

target_include_directories (
  ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CURSES_INCLUDE_DIRS}
)

target_link_libraries (
  ${PROJECT_NAME}
    PRIVATE ${CURSES_LIBRARIES}
)

