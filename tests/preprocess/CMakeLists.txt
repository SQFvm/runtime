file(GLOB_RECURSE test_script_files "*.sqf")
foreach(test_full_path ${test_script_files})
	add_sqf_preprocess_test(${test_full_path})
endforeach()
