# CMakeList.txt : CMake project for sqfvm, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.18)

list(APPEND vm_headers_opcodes
        "${PROJECT_SOURCE_DIR}/src/opcodes/assign_to.h"
        "${PROJECT_SOURCE_DIR}/src/opcodes/assign_to_local.h"
        "${PROJECT_SOURCE_DIR}/src/opcodes/call_binary.h"
        "${PROJECT_SOURCE_DIR}/src/opcodes/call_nular.h"
        "${PROJECT_SOURCE_DIR}/src/opcodes/call_unary.h"
        "${PROJECT_SOURCE_DIR}/src/opcodes/common.h"
        "${PROJECT_SOURCE_DIR}/src/opcodes/end_statement.h"
        "${PROJECT_SOURCE_DIR}/src/opcodes/get_variable.h"
        "${PROJECT_SOURCE_DIR}/src/opcodes/make_array.h"
        "${PROJECT_SOURCE_DIR}/src/opcodes/push.h")