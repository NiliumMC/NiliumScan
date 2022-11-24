#[[
cli/CMakeLists.txt
This file is a part of the TMCScan distribution released under GNU GPLv3
Copyright (C) 2022 _Magenta_
https://github.com/TuiMC/TMCScan
]]

PROJECT (tmcscan-cli)

ADD_LIBRARY (${PROJECT_NAME} OBJECT)
ADD_LIBRARY (tmc::cli ALIAS ${PROJECT_NAME})

TARGET_SOURCES (
  ${PROJECT_NAME} PRIVATE
    src/cli_parameters.c
    src/cli_help.c
    src/cli.c
)

CONFIGURE_FILE (${PROJECT_SOURCE_DIR}/include/cli/cli_help.h.in ${PROJECT_SOURCE_DIR}/include/cli/cli_help.h NEWLINE_STYLE UNIX)

TARGET_INCLUDE_DIRECTORIES (
  ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

