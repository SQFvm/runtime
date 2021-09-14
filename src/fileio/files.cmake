# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_fileio
        "${PROJECT_SOURCE_DIR}/src/fileio/default.h")
list(APPEND vm_sources_fileio
        "${PROJECT_SOURCE_DIR}/src/fileio/default.cpp")
