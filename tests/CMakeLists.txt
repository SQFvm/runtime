# Register all .sqf tests

set(TEST_ROOT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})

function(add_sqf_run_test test_full_path)
	get_filename_component(test_name ${test_full_path} NAME_WE)
    file(RELATIVE_PATH test_relative_path ${TEST_ROOT_LOCATION} ${test_full_path})
	set(test_full_name run.${test_name})

	message(VERBOSE "Registering test " ${test_full_path} " as " ${test_full_name})
	add_test(
		NAME "${test_full_name}"
		COMMAND sqfvm -a -V --no-execute-print --max-runtime 1000
                --input-sqf "framework.sqf" --input-config ${TEST_ROOT_LOCATION}/config.cpp 
                --sqf "\"${test_relative_path}\" call test_fnc_run_from_file;"
		WORKING_DIRECTORY ${TEST_ROOT_LOCATION}
	)
	set_tests_properties(${test_full_name} PROPERTIES TIMEOUT 11)
endfunction()

function(add_sqf_preprocess_test test_full_path)
	get_filename_component(test_name ${test_full_path} NAME_WE)
	set(test_full_name preprocess.${test_name})

	message(VERBOSE "Registering test " ${test_full_path} " as " ${test_full_name})
	add_test(
		NAME "${test_full_name}"
		COMMAND sqfvm -a -V --no-execute-print --preprocess-file ${test_full_path} --input-config ${TEST_ROOT_LOCATION}/config.cpp --max-runtime 10000
		WORKING_DIRECTORY ${TEST_ROOT_LOCATION}
	)
endfunction()

add_subdirectory(sqf)
add_subdirectory(preprocess)
add_subdirectory(cba)
