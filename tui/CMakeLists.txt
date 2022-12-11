#[[
tui/CMakeLists.txt
This file is a part of the TMCScan distribution released under GNU GPLv3
Copyright (C) 2022 0Magenta0
https://github.com/TuiMC/TMCScan
]]

PROJECT (tmcscan-tui)

SET (CURSES_NEED_NCURSES ON)
SET (CURSES_NEED_WIDE ON)
INCLUDE (FindCurses)

ADD_LIBRARY (${PROJECT_NAME} OBJECT)
ADD_LIBRARY (tmc::tui ALIAS ${PROJECT_NAME})

TARGET_SOURCES (
  ${PROJECT_NAME} PRIVATE
    src/tui_serv_info_colons.c
    src/tui_main_actions.c
    src/tui_window_scan.c
    src/tui_window_quit.c
    src/tui_textfield.c
    src/tui_min_size.c
    src/tui_button.c
    src/tui_utils.c
    src/tui.c
)

TARGET_INCLUDE_DIRECTORIES (
  ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CURSES_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES (
  ${PROJECT_NAME}
    PRIVATE ${CURSES_LIBRARIES}
)

