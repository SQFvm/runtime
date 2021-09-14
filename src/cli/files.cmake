# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

# Add source to this project's executable.
list(APPEND vm_headers_cli
        "${PROJECT_SOURCE_DIR}/src/cli/cli.hpp"
        "${PROJECT_SOURCE_DIR}/src/cli/colors.h"
        "${PROJECT_SOURCE_DIR}/src/cli/interactive_helper.h"
        "${PROJECT_SOURCE_DIR}/src/cli/main.h")
list(APPEND vm_sources_cli
        "${PROJECT_SOURCE_DIR}/src/cli/cli.cpp"
        "${PROJECT_SOURCE_DIR}/src/cli/interactive_helper.cpp"
        "${PROJECT_SOURCE_DIR}/src/cli/main.cpp")