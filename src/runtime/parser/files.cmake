# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_runtime
        "${PROJECT_SOURCE_DIR}/src/runtime/parser/config.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/parser/preprocessor.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/parser/sqf.h")
list(APPEND vm_sources_runtime
        "${PROJECT_SOURCE_DIR}/src/runtime/parser/preprocessor.cpp")
