# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_parser
        "${PROJECT_SOURCE_DIR}/src/parser/sqf/astnode.hpp"
        "${PROJECT_SOURCE_DIR}/src/parser/sqf/location.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/sqf/parser.tab.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/sqf/position.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/sqf/sqf_parser.hpp"
        "${PROJECT_SOURCE_DIR}/src/parser/sqf/stack.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/sqf/tokenizer.hpp")
list(APPEND vm_sources_parser
        "${PROJECT_SOURCE_DIR}/src/parser/sqf/parser.tab.cc"
        "${PROJECT_SOURCE_DIR}/src/parser/sqf/sqf_parser.cpp")
