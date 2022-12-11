#[[
scan/CMakeLists.txt
This file is a part of the TMCScan distribution released under GNU GPLv3
Copyright (C) 2022 0Magenta0
https://github.com/TuiMC/TMCScan
]]

PROJECT (tmcscan-scan)

SET (CMAKE_USE_PTHREADS_INIT ON)
INCLUDE (FindThreads)

FIND_PACKAGE (json-c REQUIRED NO_MODULE)

ADD_LIBRARY (${PROJECT_NAME} OBJECT)
ADD_LIBRARY (tmc::scan ALIAS ${PROJECT_NAME})

TARGET_SOURCES (
  ${PROJECT_NAME} PRIVATE
    src/scan.c
)

TARGET_INCLUDE_DIRECTORIES (
  ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

TARGET_LINK_LIBRARIES (
  ${PROJECT_NAME}
    PRIVATE Threads::Threads
    PRIVATE json-c::json-c
)

