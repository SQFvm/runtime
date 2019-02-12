cmake_minimum_required(VERSION 2.6)
project(sqfvm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MACOSX_RPATH 1)

set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)
set(RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)

include_directories("${PROJECT_SOURCE_DIR}/include/json/include")
include_directories("${PROJECT_SOURCE_DIR}/include/tclap-1.2.2/include")

file(GLOB vm_src 
    "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/commands/*.h" "${PROJECT_SOURCE_DIR}/src/commands/*.cpp" "${PROJECT_SOURCE_DIR}/src/commands/*.c"
)

find_package(Threads)
add_executable(sqfvm ${vm_src})
target_link_libraries(sqfvm ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(sqfvm PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_library(libsqfvm SHARED ${vm_src})
target_include_directories(libsqfvm PUBLIC ${PROJECT_SOURCE_DIR}/src)
SET_TARGET_PROPERTIES(libsqfvm PROPERTIES PREFIX "")

add_library(slibsqfvm STATIC ${vm_src})
target_include_directories(slibsqfvm PUBLIC ${PROJECT_SOURCE_DIR}/src)
SET_TARGET_PROPERTIES(slibsqfvm PROPERTIES PREFIX "")