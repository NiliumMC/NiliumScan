#[[
cli/CMakeLists.txt
This file is a part of the TMCScan distribution released under GNU GPLv3
Copyright (C) 2022 _Magenta_
https://github.com/0Magenta0/TMCScan
]]

project (tmcscan-cli)

add_library (${PROJECT_NAME} OBJECT)
add_library (tmc::cli ALIAS ${PROJECT_NAME})

target_sources (
  ${PROJECT_NAME} PRIVATE
    src/cli_parameters.c
    src/cli_help.c
    src/cli.c
)

configure_file (${PROJECT_SOURCE_DIR}/include/cli/cli_help.h.in ${PROJECT_SOURCE_DIR}/include/cli/cli_help.h NEWLINE_STYLE UNIX)

target_include_directories (
  ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

