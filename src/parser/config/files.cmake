# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_parser
        "${PROJECT_SOURCE_DIR}/src/parser/config/config_parser.hpp"
        "${PROJECT_SOURCE_DIR}/src/parser/config/location.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/config/parser.tab.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/config/position.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/config/stack.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/config/tokenizer.hpp")
list(APPEND vm_sources_parser
        "${PROJECT_SOURCE_DIR}/src/parser/config/config_parser.cpp"
        "${PROJECT_SOURCE_DIR}/src/parser/config/parser.tab.cc")
