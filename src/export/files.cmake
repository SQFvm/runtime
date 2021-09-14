# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_export
        "${PROJECT_SOURCE_DIR}/src/export/dllexports.h")
list(APPEND vm_sources_export
        "${PROJECT_SOURCE_DIR}/src/export/dllexports.cpp")
