# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)
include("${PROJECT_SOURCE_DIR}/src/parser/assembly/files.cmake")
include("${PROJECT_SOURCE_DIR}/src/parser/config/files.cmake")
include("${PROJECT_SOURCE_DIR}/src/parser/pbo/files.cmake")
include("${PROJECT_SOURCE_DIR}/src/parser/preprocessor/files.cmake")
include("${PROJECT_SOURCE_DIR}/src/parser/sqf/files.cmake")
