cmake_minimum_required(VERSION 2.8.12)
project(sqfvm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MACOSX_RPATH 1)

# Enable multithreaded compilation in MSVC
if (MSVC)
  add_definitions(/MP)
  add_definitions(/wd4100)
endif()

# Add the filesystem library if we are building on Clang or GCC
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(ST_CXXFS_LIBS stdc++fs)
endif()

set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)
set(RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)

include_directories("${PROJECT_SOURCE_DIR}/include/tclap-1.2.2/include")


#####################
##                 ##
##  FILES SECTION  ##
##                 ##
#####################


################################################################## vm_src (SQF-VM Base) ##################################################################
file(GLOB vm_src 
    "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/rvutils/*.hpp" "${PROJECT_SOURCE_DIR}/src/rvutils/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/types/*.h" "${PROJECT_SOURCE_DIR}/src/types/*.cpp" "${PROJECT_SOURCE_DIR}/src/types/*.c"
    "${PROJECT_SOURCE_DIR}/src/cli/*.h" "${PROJECT_SOURCE_DIR}/src/cli/*.hpp" "${PROJECT_SOURCE_DIR}/src/cli/*.cpp" "${PROJECT_SOURCE_DIR}/src/cli/*.c"
    "${PROJECT_SOURCE_DIR}/src/parser/config/*.h" "${PROJECT_SOURCE_DIR}/src/parser/config/*.hh" "${PROJECT_SOURCE_DIR}/src/parser/config/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/parser/config/*.cpp" "${PROJECT_SOURCE_DIR}/src/parser/config/*.c" "${PROJECT_SOURCE_DIR}/src/parser/config/*.cc"
    "${PROJECT_SOURCE_DIR}/src/parser/preprocessor/*.h" "${PROJECT_SOURCE_DIR}/src/parser/preprocessor/*.cpp" "${PROJECT_SOURCE_DIR}/src/parser/preprocessor/*.c"
    "${PROJECT_SOURCE_DIR}/src/parser/sqf/*.h" "${PROJECT_SOURCE_DIR}/src/parser/sqf/*.hh" "${PROJECT_SOURCE_DIR}/src/parser/sqf/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/parser/sqf/*.cpp" "${PROJECT_SOURCE_DIR}/src/parser/sqf/*.c" "${PROJECT_SOURCE_DIR}/src/parser/sqf/*.cc"
    "${PROJECT_SOURCE_DIR}/src/parser/assembly/*.h" "${PROJECT_SOURCE_DIR}/src/parser/assembly/*.hh" "${PROJECT_SOURCE_DIR}/src/parser/assembly/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/parser/assembly/*.cpp" "${PROJECT_SOURCE_DIR}/src/parser/assembly/*.c" "${PROJECT_SOURCE_DIR}/src/parser/assembly/*.cc"
    "${PROJECT_SOURCE_DIR}/src/operators/*.h" "${PROJECT_SOURCE_DIR}/src/operators/*.cpp" "${PROJECT_SOURCE_DIR}/src/operators/*.c"
    "${PROJECT_SOURCE_DIR}/src/opcodes/*.h" "${PROJECT_SOURCE_DIR}/src/opcodes/*.cpp" "${PROJECT_SOURCE_DIR}/src/opcodes/*.c"
    "${PROJECT_SOURCE_DIR}/src/fileio/*.h" "${PROJECT_SOURCE_DIR}/src/fileio/*.cpp" "${PROJECT_SOURCE_DIR}/src/fileio/*.c"
    "${PROJECT_SOURCE_DIR}/src/runtime/*.h" "${PROJECT_SOURCE_DIR}/src/runtime/*.cpp" "${PROJECT_SOURCE_DIR}/src/runtime/*.c"
    "${PROJECT_SOURCE_DIR}/src/runtime/parser/*.h" "${PROJECT_SOURCE_DIR}/src/runtime/parser/*.cpp" "${PROJECT_SOURCE_DIR}/src/runtime/parser/*.c"
    "${PROJECT_SOURCE_DIR}/src/runtime/diagnostics/*.h" "${PROJECT_SOURCE_DIR}/src/runtime/diagnostics/*.cpp" "${PROJECT_SOURCE_DIR}/src/runtime/diagnostics/*.c"
    "${CMAKE_CURRENT_BINARY_DIR}/git_sha1.cpp"
)

# Get the local git revision hash and put it into a header we can use
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

configure_file("${PROJECT_SOURCE_DIR}/cmake/git_sha1.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/git_sha1.cpp" @ONLY)
list(APPEND vm_src "${CMAKE_CURRENT_BINARY_DIR}/git_sha1.cpp")

######################################################## vm_src_sqc (SQF-VM Base with SQC support) #######################################################

file(GLOB vm_src_sqc
    "${PROJECT_SOURCE_DIR}/src/sqc/*.h"
    "${PROJECT_SOURCE_DIR}/src/sqc/*.hh"
    "${PROJECT_SOURCE_DIR}/src/sqc/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/sqc/*.c"
    "${PROJECT_SOURCE_DIR}/src/sqc/*.cc"
)
list(APPEND vm_src_sqc ${vm_src})

############################################################### vm_lib_src (SQF-VM Library) ##############################################################

list(APPEND vm_lib_src ${vm_src})
list(REMOVE_ITEM vm_lib_src "${PROJECT_SOURCE_DIR}/src/cli/*.h" "${PROJECT_SOURCE_DIR}/src/cli/*.cpp" "${PROJECT_SOURCE_DIR}/src/cli/*.c")

#################################################### vm_lib_src_sqc (SQF-VM Library with SQC support) ####################################################

list(APPEND vm_lib_src_sqc ${vm_src_sqc})
list(REMOVE_ITEM vm_lib_src_sqc "${PROJECT_SOURCE_DIR}/src/cli/*.h" "${PROJECT_SOURCE_DIR}/src/cli/*.cpp" "${PROJECT_SOURCE_DIR}/src/cli/*.c")
list(REMOVE_ITEM vm_lib_src_sqc "${PROJECT_SOURCE_DIR}/src/cli/*.h" "${PROJECT_SOURCE_DIR}/src/cli/*.cpp" "${PROJECT_SOURCE_DIR}/src/cli/*.c")

#################################################### vm_lib_src_sqc (SQF-VM Library with SQC support) ####################################################

file(GLOB vm_lib_cexports_src
    "${PROJECT_SOURCE_DIR}/src/export/*.h"
    "${PROJECT_SOURCE_DIR}/src/export/*.hh"
    "${PROJECT_SOURCE_DIR}/src/export/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/export/*.c"
    "${PROJECT_SOURCE_DIR}/src/export/*.cc"
)
list(APPEND vm_lib_cexports_src ${vm_lib_src_sqc})


############################
##                        ##
##  DEPENDENCIES SECTION  ##
##                        ##
############################

find_package(Threads)


option(SQFVM_BUILD_EXECUTABLE "BUILD EXECUTABLE" ON)
option(SQFVM_BUILD_EXECUTABLE_ARMA2_LOCALKEYWORD "BUILD ARMA2 EXECUTABLE" ON)
option(SQFVM_BUILD_LIBRARY "BUILD LIBRARY" OFF)
option(SQFVM_BUILD_LIBRARY_SQC_SUPPORT "BUILD LIBRARY WITH SQC SUPPORT" OFF)
option(SQFVM_BUILD_STATIC_LIBRARY "BUILD STATIC LIBRARY" ON)
option(SQFVM_BUILD_STATIC_LIBRARY_SQC_SUPPORT "BUILD STATIC LIBRARY WITH SQC SUPPORT" ON)
option(SQFVM_BUILD_EXECUTABLE_SQC_SUPPORT "BUILD EXECUTABLE WITH SQC SUPPORT" ON)
option(SQFVM_BUILD_EXECUTABLE_FULL_DIAGNOSE "BUILD DIAGNOSE EXECUTABLE" ON)
option(SQFVM_BUILD_EXECUTABLE_ARMA2_LOCALKEYWORD_FULL_DIAGNOSE "BUILD ARMA2 DIAGNOSE EXECUTABLE" ON)
option(SQFVM_BUILD_CEXPORTS "BUILD C-EXPORTS LIBRARY" ON)


if (SQFVM_BUILD_EXECUTABLE)
    add_executable(sqfvm ${vm_src})
    target_link_libraries(sqfvm ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${ST_CXXFS_LIBS})
    target_include_directories(sqfvm PUBLIC ${PROJECT_SOURCE_DIR}/src)
    
    target_compile_options(sqfvm PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
endif ()

if (SQFVM_BUILD_EXECUTABLE_SQC_SUPPORT)
    add_executable(sqfvm_sqc ${vm_src_sqc})
    
    target_link_libraries(sqfvm_sqc ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${ST_CXXFS_LIBS})
    target_include_directories(sqfvm_sqc PUBLIC ${PROJECT_SOURCE_DIR}/src)
    
    target_compile_definitions(sqfvm_sqc PUBLIC SQF_SQC_SUPPORT)
    
    target_compile_options(sqfvm_sqc PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
endif ()

if (SQFVM_BUILD_EXECUTABLE_ARMA2_LOCALKEYWORD)
    add_executable(sqfvm_a2 ${vm_src})
    target_link_libraries(sqfvm_a2 ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${ST_CXXFS_LIBS})
    target_include_directories(sqfvm_a2 PUBLIC ${PROJECT_SOURCE_DIR}/src)
    
    target_compile_definitions(sqfvm_a2 PUBLIC SQFVM_ARMA2_SYNTAX)
    
    target_compile_options(sqfvm_a2 PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
endif ()

if (SQFVM_BUILD_EXECUTABLE_FULL_DIAGNOSE)
	
    add_executable(sqfvm_full_diagnose ${vm_src_sqc})
    target_link_libraries(sqfvm_full_diagnose ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${ST_CXXFS_LIBS})
    target_include_directories(sqfvm_full_diagnose PUBLIC ${PROJECT_SOURCE_DIR}/src)
    
    target_compile_definitions(sqfvm_full_diagnose PUBLIC SQF_SQC_SUPPORT)
    
    target_compile_definitions(sqfvm_full_diagnose PUBLIC DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE)
    target_compile_definitions(sqfvm_full_diagnose PUBLIC DF__SQF_FILEIO__ALL_FILES)
    target_compile_definitions(sqfvm_full_diagnose PUBLIC DF__SQF_FILEIO__TRACE_REESOLVE)
    target_compile_definitions(sqfvm_full_diagnose PUBLIC DF__SQF_PREPROC__TRACE_MACRO_RESOLVE)
    target_compile_definitions(sqfvm_full_diagnose PUBLIC DF__SQF_PREPROC__TRACE_MACRO_PARSE)
    target_compile_definitions(sqfvm_full_diagnose PUBLIC DF__SQF_CONFIG__REPORT_PROGRESS_BY_LINE)
    target_compile_definitions(sqfvm_full_diagnose PUBLIC DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG)
    target_compile_definitions(sqfvm_full_diagnose PUBLIC DF__CLI_PRINT_INPUT_ARGS)
    target_compile_definitions(sqfvm_full_diagnose PUBLIC DF__CLI_PRINT_TCLAP_ARGUMENTS)
    
    target_compile_options(sqfvm_full_diagnose PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
endif ()

if (SQFVM_BUILD_EXECUTABLE_ARMA2_LOCALKEYWORD_FULL_DIAGNOSE)
	
    add_executable(sqfvm_a2_full_diagnose ${vm_src_sqc})
    target_link_libraries(sqfvm_a2_full_diagnose ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${ST_CXXFS_LIBS})
    target_include_directories(sqfvm_a2_full_diagnose PUBLIC ${PROJECT_SOURCE_DIR}/src)
    
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC SQFVM_ARMA2_SYNTAX)
    
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC SQF_SQC_SUPPORT)
    
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC DF__SQF_RUNTIME__ASSEMBLY_DEBUG_ON_EXECUTE)
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC DF__SQF_FILEIO__ALL_FILES)
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC DF__SQF_FILEIO__TRACE_REESOLVE)
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC DF__SQF_PREPROC__TRACE_MACRO_RESOLVE)
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC DF__SQF_PREPROC__TRACE_MACRO_PARSE)
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC DF__SQF_CONFIG__REPORT_PROGRESS_BY_LINE)
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC DF__SQF_RUNTIME__VALUE_SCOPE_DEBUG)
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC DF__CLI_PRINT_INPUT_ARGS)
    target_compile_definitions(sqfvm_a2_full_diagnose PUBLIC DF__CLI_PRINT_TCLAP_ARGUMENTS)
    
    target_compile_options(sqfvm_a2_full_diagnose PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
endif ()


if (SQFVM_BUILD_LIBRARY)
    add_library(libsqfvm SHARED ${vm_lib_src})
    target_compile_definitions(libsqfvm PUBLIC DISABLE_CLIPBOARD)
    target_include_directories(libsqfvm PUBLIC ${PROJECT_SOURCE_DIR}/src)
    target_compile_options(libsqfvm PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
    SET_TARGET_PROPERTIES(libsqfvm PROPERTIES PREFIX "")
endif ()

if (SQFVM_BUILD_LIBRARY_SQC_SUPPORT)
    add_library(libsqfvm_sqc SHARED ${vm_lib_src_sqc})
    target_compile_definitions(libsqfvm_sqc PUBLIC DISABLE_CLIPBOARD)
    target_include_directories(libsqfvm_sqc PUBLIC ${PROJECT_SOURCE_DIR}/src)
    
    target_compile_definitions(libsqfvm_sqc PUBLIC SQF_SQC_SUPPORT)
    
    target_compile_options(libsqfvm_sqc PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
    SET_TARGET_PROPERTIES(libsqfvm_sqc PROPERTIES PREFIX "")
endif ()

if (SQFVM_BUILD_CEXPORTS)
    add_library(libcsqfvm SHARED ${vm_lib_cexports_src})
    target_compile_definitions(libcsqfvm PUBLIC DISABLE_CLIPBOARD)
    target_include_directories(libcsqfvm PUBLIC ${PROJECT_SOURCE_DIR}/src)
    
    target_compile_definitions(libcsqfvm PUBLIC SQF_SQC_SUPPORT)
    
    target_compile_options(libcsqfvm PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
    SET_TARGET_PROPERTIES(libcsqfvm PROPERTIES PREFIX "")
endif ()

if (SQFVM_BUILD_STATIC_LIBRARY)
    add_library(slibsqfvm STATIC ${vm_lib_src})
    target_include_directories(slibsqfvm PUBLIC ${PROJECT_SOURCE_DIR}/src)
    target_compile_options(slibsqfvm PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
    SET_TARGET_PROPERTIES(slibsqfvm PROPERTIES PREFIX "")
endif ()

if (SQFVM_BUILD_STATIC_LIBRARY_SQC_SUPPORT)
    add_library(slibsqfvm_sqc STATIC ${vm_lib_src_sqc})
    target_compile_definitions(slibsqfvm_sqc PUBLIC SQF_SQC_SUPPORT)
    target_include_directories(slibsqfvm_sqc PUBLIC ${PROJECT_SOURCE_DIR}/src)
    target_compile_options(slibsqfvm_sqc PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
    SET_TARGET_PROPERTIES(slibsqfvm_sqc PROPERTIES PREFIX "")
endif ()
