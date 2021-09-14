# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_parser
        "${PROJECT_SOURCE_DIR}/src/parser/assembly/assembly_parser.h"
        "${PROJECT_SOURCE_DIR}/src/parser/assembly/astnode.h"
        "${PROJECT_SOURCE_DIR}/src/parser/assembly/location.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/assembly/parser.tab.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/assembly/position.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/assembly/stack.hh"
        "${PROJECT_SOURCE_DIR}/src/parser/assembly/tokenizer.hpp")
list(APPEND vm_sources_parser
        "${PROJECT_SOURCE_DIR}/src/parser/assembly/assembly_parser.cpp"
        "${PROJECT_SOURCE_DIR}/src/parser/assembly/parser.tab.cc")
