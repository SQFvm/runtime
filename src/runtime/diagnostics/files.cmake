# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_runtime
        "${PROJECT_SOURCE_DIR}/src/runtime/diagnostics/breakpoint.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/diagnostics/d_stacktrace.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/diagnostics/diag_info.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/diagnostics/stacktrace.h")
list(APPEND vm_sources_runtime
        "${PROJECT_SOURCE_DIR}/src/runtime/diagnostics/stacktrace.cpp")
