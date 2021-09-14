# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

include("${PROJECT_SOURCE_DIR}/src/runtime/diagnostics/files.cmake")
include("${PROJECT_SOURCE_DIR}/src/runtime/parser/files.cmake")
list(APPEND vm_headers_runtime
        "${PROJECT_SOURCE_DIR}/src/runtime/confighost.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/context.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/d_array.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/d_boolean.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/d_code.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/d_scalar.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/d_string.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/data.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/fileio.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/frame.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/instruction.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/instruction_set.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/logging.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/runtime.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/sqfop.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/type.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/util.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/value.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/value_scope.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/vec.h"
        "${PROJECT_SOURCE_DIR}/src/runtime/version.h")
list(APPEND vm_sources_runtime
        "${PROJECT_SOURCE_DIR}/src/runtime/d_array.cpp"
        "${PROJECT_SOURCE_DIR}/src/runtime/d_scalar.cpp"
        "${PROJECT_SOURCE_DIR}/src/runtime/fileio.cpp"
        "${PROJECT_SOURCE_DIR}/src/runtime/frame.cpp"
        "${PROJECT_SOURCE_DIR}/src/runtime/logging.cpp"
        "${PROJECT_SOURCE_DIR}/src/runtime/runtime.cpp")
