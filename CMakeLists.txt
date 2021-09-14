cmake_minimum_required(VERSION 3.18)
project(sqfvm)

########################
##                    ##
## BASE CONFIGURATION ##
##                    ##
########################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MACOSX_RPATH 1)

# Enable multi-threaded compilation in MSVC
if (MSVC)
  set(MSVC_COMPILE_FLAGS "/MP")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_COMPILE_FLAGS} /wd4100")
  string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
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

include(src/cli/files.cmake)
include(src/export/files.cmake)
include(src/fileio/files.cmake)
include(src/opcodes/files.cmake)
include(src/operators/files.cmake)
include(src/parser/files.cmake)
include(src/runtime/files.cmake)
include(src/rvutils/files.cmake)
include(src/sqc/files.cmake)


######################################################## vm_sources_git #######################################################
# Get the local git revision hash and put it into a header we can use
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

configure_file("${PROJECT_SOURCE_DIR}/cmake/git_sha1.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/git_sha1.cpp" @ONLY)
list(APPEND vm_headers_git
        "${PROJECT_SOURCE_DIR}/src/runtime/git_sha1.h")
list(APPEND vm_sources_git
        "${CMAKE_CURRENT_BINARY_DIR}/git_sha1.cpp")


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
    add_executable(sqfvm "")
    set_target_properties(sqfvm PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(sqfvm PUBLIC
            ${vm_headers_cli}
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils}
            ${vm_headers_git})
    target_sources(sqfvm PRIVATE
            ${vm_sources_cli}
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime}
            ${vm_sources_git})
    target_link_libraries(sqfvm ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${ST_CXXFS_LIBS})

    target_compile_options(sqfvm PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
endif ()

if (SQFVM_BUILD_EXECUTABLE_SQC_SUPPORT)
    add_executable(sqfvm_sqc "")
    set_target_properties(sqfvm_sqc PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(sqfvm_sqc PUBLIC
            ${vm_headers_cli}
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils}
            ${vm_headers_sqc}
            ${vm_headers_git})
    target_sources(sqfvm_sqc PRIVATE
            ${vm_sources_cli}
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime}
            ${vm_sources_sqc}
            ${vm_sources_git})

    target_link_libraries(sqfvm_sqc ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${ST_CXXFS_LIBS})
    target_compile_definitions(sqfvm_sqc PUBLIC SQF_SQC_SUPPORT)
    target_compile_options(sqfvm_sqc PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
endif ()

if (SQFVM_BUILD_EXECUTABLE_ARMA2_LOCALKEYWORD)
    add_executable(sqfvm_a2 "")
    set_target_properties(sqfvm_a2 PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(sqfvm_a2 PUBLIC
            ${vm_headers_cli}
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils}
            ${vm_headers_git})
    target_sources(sqfvm_a2 PRIVATE
            ${vm_sources_cli}
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime}
            ${vm_sources_git})
    target_link_libraries(sqfvm_a2 ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${ST_CXXFS_LIBS})
    target_compile_definitions(sqfvm_a2 PUBLIC SQFVM_ARMA2_SYNTAX)
    target_compile_options(sqfvm_a2 PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
endif ()

if (SQFVM_BUILD_EXECUTABLE_FULL_DIAGNOSE)

    add_executable(sqfvm_full_diagnose "")
    set_target_properties(sqfvm_full_diagnose PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(sqfvm_full_diagnose PUBLIC
            ${vm_headers_cli}
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils}
            ${vm_headers_git})
    target_sources(sqfvm_full_diagnose PRIVATE
            ${vm_sources_cli}
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime}
            ${vm_sources_git})
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

    add_executable(sqfvm_a2_full_diagnose "")
    set_target_properties(sqfvm_a2_full_diagnose PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(sqfvm_a2_full_diagnose PUBLIC
            ${vm_headers_cli}
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils}
            ${vm_headers_git})
    target_sources(sqfvm_a2_full_diagnose PRIVATE
            ${vm_sources_cli}
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime}
            ${vm_sources_git})
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
    add_library(libsqfvm SHARED "")
    set_target_properties(libsqfvm PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(libsqfvm PUBLIC
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils}
            ${vm_headers_sqc})
    target_sources(libsqfvm PRIVATE
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime}
            ${vm_sources_sqc})
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
    add_library(libsqfvm_sqc SHARED "")
    set_target_properties(libsqfvm_sqc PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(libsqfvm_sqc PUBLIC
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils}
            ${vm_headers_sqc})
    target_sources(libsqfvm_sqc PRIVATE
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime}
            ${vm_sources_sqc})
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
    add_library(libcsqfvm SHARED "")
    set_target_properties(libcsqfvm PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(libcsqfvm PUBLIC
            ${vm_headers_export})
    target_sources(libcsqfvm PRIVATE
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils}
            ${vm_headers_sqc}

            ${vm_sources_export}
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime}
            ${vm_sources_sqc})
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
    add_library(slibsqfvm STATIC "")
    set_target_properties(slibsqfvm PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(slibsqfvm PUBLIC
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils})
    target_sources(slibsqfvm PRIVATE
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime})
    target_include_directories(slibsqfvm PUBLIC ${PROJECT_SOURCE_DIR}/src)
    target_compile_options(slibsqfvm PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
    SET_TARGET_PROPERTIES(slibsqfvm PROPERTIES PREFIX "")
endif ()

if (SQFVM_BUILD_STATIC_LIBRARY_SQC_SUPPORT)
    add_library(slibsqfvm_sqc STATIC "")
    set_target_properties(slibsqfvm_sqc PROPERTIES LINKER_LANGUAGE CXX)
    target_sources(slibsqfvm_sqc PUBLIC
            ${vm_headers_fileio}
            ${vm_headers_opcodes}
            ${vm_headers_operators}
            ${vm_headers_parser}
            ${vm_headers_runtime}
            ${vm_headers_rvutils}
            ${vm_headers_sqc})
    target_sources(slibsqfvm_sqc PRIVATE
            ${vm_sources_fileio}
            ${vm_sources_operators}
            ${vm_sources_parser}
            ${vm_sources_runtime}
            ${vm_sources_sqc})
    target_compile_definitions(slibsqfvm_sqc PUBLIC SQF_SQC_SUPPORT)
    target_include_directories(slibsqfvm_sqc PUBLIC ${PROJECT_SOURCE_DIR}/src)
    target_compile_options(slibsqfvm_sqc PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wno-unknown-pragmas>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
    SET_TARGET_PROPERTIES(slibsqfvm_sqc PROPERTIES PREFIX "")
endif ()
