# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_parser
        "${PROJECT_SOURCE_DIR}/src/parser/pbo/parsepbo.h")
list(APPEND vm_sources_parser
        "${PROJECT_SOURCE_DIR}/src/parser/pbo/parsepbo.cpp")
