# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_sqc
        "${PROJECT_SOURCE_DIR}/src/sqc/location.hh"
        "${PROJECT_SOURCE_DIR}/src/sqc/parser.tab.hh"
        "${PROJECT_SOURCE_DIR}/src/sqc/position.hh"
        "${PROJECT_SOURCE_DIR}/src/sqc/sqc_parser.h"
        "${PROJECT_SOURCE_DIR}/src/sqc/stack.hh"
        "${PROJECT_SOURCE_DIR}/src/sqc/tokenizer.h")
list(APPEND vm_sources_sqc
        "${PROJECT_SOURCE_DIR}/src/sqc/parser.tab.cc"
        "${PROJECT_SOURCE_DIR}/src/sqc/sqc_parser.cpp"
        )
