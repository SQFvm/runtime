# Register all .sqf tests

set(TEST_CONFIG_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp)

function(add_sqf_run_test test_full_path)
	get_filename_component(test_name ${test_full_path} NAME_WE)
	set(test_full_name run.${test_name})

	message("Registering test " ${test_full_path} " as " ${test_full_name})
	add_test(
		NAME "${test_full_name}"
		COMMAND sqfvm -a -V --no-execute-print --input-sqf ${test_full_path} --input-config ${TEST_CONFIG_LOCATION} --max-runtime 10000
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	set_tests_properties(${test_full_name} PROPERTIES TIMEOUT 11)
endfunction()

function(add_sqf_preprocess_test test_full_path)
	get_filename_component(test_name ${test_full_path} NAME_WE)
	set(test_full_name preprocess.${test_name})

	message("Registering test " ${test_full_path} " as " ${test_full_name})
	add_test(
		NAME "${test_full_name}"
		COMMAND sqfvm -a -V --no-execute-print --preprocess-file ${test_full_path} --input-config ${TEST_CONFIG_LOCATION} --max-runtime 10000
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endfunction()

add_subdirectory(sqf)
add_subdirectory(preprocess)
add_subdirectory(cba)