/*********************************************************
 * The following SQF code REQUIRES SQF-VM                *
 * to work. There are special, SQF-VM only commands      *
 * used, to make it as productive as possible.           *
 *                                                       *
 * SQF-VM commands are suffixed with a double underscore *
 * (eg. exitcode__)                                      *
 * ----------------------------------------------------- *
 * Test-Case format:                                     *
 * - ["OP", { CODE }, PARA]                              *
 * - ["OP", ["DESCRIPTION", {CODE}], PARA]               *
 * ----------------------------------------------------- *
 * Test-Case Operations:                                 *
 * - assert:                                             *
 *      Allows to just execute a piece of code. Will     *
 *      PASS if there is no exception raised during      *
 *      execution.                                       *
 * - assertTrue:                                         *
 *      Will PASS if the codes return value is true.     *
 * - assertFalse:                                        *
 *      Will PASS if the codes return value is false.    *
 * - assertNil|assertIsNil:                              *
 *      Will PASS if the codes return value is nil.      *
 * - assertEqual:                                        *
 *      Will PASS if the codes return value is equal     *
 *      to whatever is passed with PARA.                 *
 * - assertException:                                    *
 *      Will PASS if the code threw any Exception.       *
 *      Note that this is including, but not limited     *
 *      to the `throw` operator.                         *
 ********************************************************/
 
 
diag_log format (["%1"] + productVersion);
diag_log format (["v %3.%4 (%5)"] + productVersion);
diag_log format (["%7 %8"] + productVersion);

testsIndex = 0;
testsPassed = 0;
testsFailed = 0;
fatalError = false;

test_fnc_testPassed = {
    params ["___name___", "___desc___", "___index___"];
    systemChat format ["Test  Passed  '%1' - %2", ___name___, ___index___ + 1];
    testsPassed = testsPassed + 1;
};

test_fnc_testFailed = {
    params ["___name___", "___desc___", "___index___"];
    private _msg1 = format ["Test !FAILED! '%1' - %2  %3", ___name___, ___index___ + 1, trim__ ___desc___];
    systemChat _msg1;
    ___failed___ pushBack _msg1;
    testsFailed = testsFailed + 1;
};

test_fnc_exceptWrapper = {
    params ["___args___", "___code___"];
    {
        ___args___ call ___code___
    }
    except__
    {
        private ___msg___ = format ["Exception occurred: %1",  trim__ _exception];
        [___args___ select 0, ___args___ select 2, ___args___ select 3, ___msg___] call test_fnc_testFailed;
    }
};

test_fnc_assertEqual = {
    [_this, {
        params ["___name___", "___test___", "___desc___", "___index___", "___compare___"];
        private ___ret___ = call ___test___;
        if (___ret___ isEqualTo ___compare___) then
        {
            [___name___, ___desc___, ___index___] call test_fnc_testPassed;
        }
        else
        {
            private ___msg___ = format [
                "Wrong return value. Expected %1 (type %2), got %3 (type %4).",
                ___compare___,
                typeName ___compare___,
                ___ret___,
                typeName ___ret___
            ];
            [___name___, ___desc___, ___index___, ___msg___] call test_fnc_testFailed;
        }
    }] call test_fnc_exceptWrapper;
};

test_fnc_assert = {
    [_this, {
        params ["___name___", "___test___", "___desc___", "___index___", "___compare___"];
        private ___ret___ = call ___test___;
        [___name___, ___desc___, ___index___] call test_fnc_testPassed;
    }] call test_fnc_exceptWrapper;
};

test_fnc_assertIsNil = {
    [_this, {
        params ["___name___", "___test___", "___desc___", "___index___", "___compare___"];
        private ___ret___ = call ___test___;
        if (isNil "___ret___") then
        {
            [___name___, ___desc___, ___index___] call test_fnc_testPassed;
        }
        else
        {
            private ___msg___ = format ["Wrong return value. Expected nil, got %1 (type %2).",  ___ret___, typeName ___ret___];
            [___name___, ___desc___, ___index___, ___msg___] call test_fnc_testFailed;
        }
    }] call test_fnc_exceptWrapper;
};

test_fnc_assertException = {
    params ["___name___", "___test___", "___desc___", "___index___"];
    {
        private ___ret___ = call ___test___;
        private ___msg___ = format ["Never reached except. Returned: %1", ___ret___];
        [___name___, ___desc___, ___index___, ___msg___] call test_fnc_testFailed;
    }
    except__
    {
        [___name___, ___desc___, ___index___] call test_fnc_testPassed;
    }
};

private ___currentDirectory___ = currentDirectory__;
private ___currentDirectoryLength___ = count ___currentDirectory___;
private ___exceptions___ = [];
private ___failed___ = [];
diag_log "Loading tests from:";
diag_log format ["    %1", ___currentDirectory___];

private ___sqf_test_dir = currentDirectory__ + "\" + "sqf";
private ___preprocessor_test_dir = currentDirectory__ + "\" + "preprocess";
{
    if (_x select [0, count ___sqf_test_dir] == ___sqf_test_dir) then {
        if (count _x > ___currentDirectoryLength___) then {
            if (_x select [0, ___currentDirectoryLength___] == ___currentDirectory___) then
            {
                {
                    private ___name___ = _x select [___currentDirectoryLength___];
                    private ___tests___ = call compile preprocessFileLineNumbers _x;
					private ___setup___ = { [] call _this; };
                    if !(___tests___ isEqualType []) then
                    {
                        throw format ["Invalid type. Expected ARRAY; Got %1", typeName ___tests___];
                        fatalError = true;
                    };
                    {
                        private ___mode___ = _x select 0;
                        private ___test___ = _x select 1;
                        private ___desc___ = if(___test___ isEqualType []) then { ___test___ select 0 } else { str(___test___) };
                        private ___code___ = if(___test___ isEqualType []) then { ___test___ select 1 } else { ___test___ };

                        private ___res___ = false;
                        if (___mode___ isEqualType "") then
                        {
                            
                            switch (___mode___) do
                            {
								case "setup": { ___setup___ = ___code___; };
                                case "assert": {
									[{
										[___name___, ___code___, ___desc___, _forEachIndex, true] call test_fnc_assert
									}, ___setup___] call test_fnc_exceptWrapper;
								};
                                case "assertTrue": {
									[{
										[___name___, ___code___, ___desc___, _forEachIndex, true] call test_fnc_assertEqual
									}, ___setup___] call test_fnc_exceptWrapper;
								};
								case "assertFalse": {
									[{
										[___name___, ___code___, ___desc___, _forEachIndex, false] call test_fnc_assertEqual
									}, ___setup___] call test_fnc_exceptWrapper;
								};
								case "assertEqual": {
									[{
										[___name___, ___code___, ___desc___, _forEachIndex, _x select 2] call test_fnc_assertEqual
									}, ___setup___] call test_fnc_exceptWrapper;
								};
                                case "assertNil";
                                case "assertIsNil": {
									[{
										[___name___, ___code___, ___desc___, _forEachIndex] call test_fnc_assertIsNil
									}, ___setup___] call test_fnc_exceptWrapper;
								};
                                case "assertException": {
									[{
										[___name___, ___code___, ___desc___, _forEachIndex] call test_fnc_assertException
									}, ___setup___] call test_fnc_exceptWrapper;
								};
                                default {
                                    throw format ["Unknown Test-Type %1 in %2-%3 (%4)", ___mode___, ___name___, _forEachIndex + 1, ___desc___];
                                    fatalError = true;
                                }
                            }
                        }
                        else
                        {
                            if (___mode___ isEqualType {}) then
                            {
                                ___res___ = [___name___, ___test___, _forEachIndex, _x] call ___mode___;
                            }
                            else
                            {
                                throw format ["Test-Type was expected to be either STRING or CODE but was %1", typeName ___mode___];
                                fatalError = true;
                            };
                        };
                        testsIndex = testsIndex + 1;
                    } forEach ___tests___;
                }
                except__
                {
                    private _msg = format ["Exception during test execution of %1: %2", ___name___, _exception];
                    diag_log _msg;
                    ___exceptions___ pushBack _msg;
                    fatalError = true;
                };
            };
        };
    } else {
        if (_x select [0, count ___preprocessor_test_dir] == ___preprocessor_test_dir) then {
            {
                private ___fpath___ = _x;
                private ___code___ = { preprocess__ loadFile ___fpath___ };
                private ___expected___ = loadFile ((_x select [0, count _x - 3]) + "txt");
                systemChat ___fpath___;
                systemChat ((_x select [0, count _x - 3]) + "txt");
                systemChat preprocess__ loadFile ___fpath___;
                systemChat loadFile ((_x select [0, count _x - 3]) + "txt");
                
                private ___name___ = _x select [___currentDirectoryLength___];
                private ___tests___ = call compile preprocessFileLineNumbers _x;
                [___name___, ___code___, "", 0, ___expected___] call test_fnc_assertEqual;
                testsIndex = testsIndex + 1;
            }
            except__
            {
                private _msg = format ["Exception during test execution of %1: %2", ___name___, _exception];
                diag_log _msg;
                ___exceptions___ pushBack _msg;
                fatalError = true;
            };
        };
    };
} forEach allFiles__ [".sqf"];
diag_log format ["%1 out of %2 tests passed.", testsPassed, testsIndex];
{
    diag_log _x;
} foreach ___failed___;
if (fatalError) then
{
    diag_log "FATALERROR occured during testing:";
    {
        diag_log _x;
    } foreach ___exceptions___;
    exitcode__ -1;
}
else 
{
    diag_log "";
    diag_log (["FAILED", "SUCCESS"] select (testsPassed == testsIndex));
    exitcode__ (testsIndex - testsPassed);
};
