find_package(Python3 COMPONENTS Interpreter QUIET)
if(Python3_Interpreter_FOUND)
    add_test(NAME fileio.pbo
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pbo.py $<TARGET_FILE:sqfvm>)
    set_tests_properties(fileio.pbo PROPERTIES TIMEOUT 60)
else()
    message(STATUS "Python3 not found: PBO CLI regression tests are unavailable")
endif()
