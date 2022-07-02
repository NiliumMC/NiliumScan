#[[****************************************************************************
   *
   *    scan/CMakeLists.txt
   *    This file is part of TMCScan distribution released under GPL 3.0
   *    Copyright (C) 2022 _Magenta_
   *    https://github.com/0Magenta0/TMCScan
   *
   ****************************************************************************]]

project (scan)

find_package(json-c REQUIRED NO_MODULE)

set (SCAN_SOURCES
     src/scan.c
     src/scan_utils.c)

add_library (${PROJECT_NAME} ${SCAN_SOURCES})
add_library (tmc::scan ALIAS ${PROJECT_NAME})

target_include_directories (${PROJECT_NAME}
                            PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries (${PROJECT_NAME}
                       PRIVATE json-c::json-c)

