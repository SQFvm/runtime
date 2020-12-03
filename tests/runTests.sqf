/*********************************************************
 * The following SQF code REQUIRES SQF-VM                *
 * to work. There are special, SQF-VM only commands      *
 * used, to make it as productive as possible.           *
 *                                                       *
 * SQF-VM commands are suffixed with a double underscore *
 * (eg. exitcode__)                                      *
 ********************************************************/


// #define _TEST_FRAMEWORK_DEBUG


#ifdef _TEST_FRAMEWORK_DEBUG
#define DIAGNOSTICS(MESSAGE) diag_log #MESSAGE
#define DIAGNOSTICS_EXEC(MESSAGE) diag_log (MESSAGE)
#else
#define DIAGNOSTICS(MESSAGE)
#define DIAGNOSTICS_EXEC(MESSAGE)
#endif
#define COMMA ,

diag_log str productVersion;
diag_log format(["%1"] + productVersion);
diag_log format(["v %3.%4 (%5)"] + productVersion);
diag_log format(["%7 %8"] + productVersion);

testsIndex = 0;
testsPassed = 0;
testsFailed = 0;
fatalError = false;

test_fnc_testPassed = {
    params["___name___", "___desc___", "___index___"];
    DIAGNOSTICS(TEST PASSED);
    diag_log format["Test  Passed  '%1' - %2", ___name___, ___index___ + 1];
    testsPassed = testsPassed + 1;
};

test_fnc_testFailed = {
    params["___name___", "___desc___", "___index___", "___msg___"];
    DIAGNOSTICS(TEST FAILED);
    private _msg1 = format["Test !FAILED! '%1' - %2  %3", ___name___, ___index___ + 1, trim__ ___desc___];
    diag_log _msg1;
    diag_log ___msg___;
    ___failed___ pushBack _msg1;
    testsFailed = testsFailed + 1;
};

test_fnc_assertEqual = {
    [_this, {
        params["___name___", "___test___", "___desc___", "___index___", "___compare___"];
        private ___ret___ = call ___test___;
        DIAGNOSTICS_EXEC(format ["___ret___: %1" COMMA ___ret___]);
        DIAGNOSTICS_EXEC(format ["___compare___: %1" COMMA ___compare___]);
        if (___ret___ isEqualTo ___compare___) then
        {
            [___name___, ___desc___, ___index___] call test_fnc_testPassed;
        }
        else
        {
            private ___msg___ = format[
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
        params["___name___", "___test___", "___desc___", "___index___", "___compare___"];
        private ___ret___ = call ___test___;
        [___name___, ___desc___, ___index___] call test_fnc_testPassed;
    }] call test_fnc_exceptWrapper;
};

test_fnc_assertIsNil = {
    [_this, {
        params["___name___", "___test___", "___desc___", "___index___", "___compare___"];
        private ___ret___ = call ___test___;
        if (isNil "___ret___") then
        {
            [___name___, ___desc___, ___index___] call test_fnc_testPassed;
        }
        else
        {
            private ___msg___ = format["Wrong return value. Expected nil, got %1 (type %2).",  ___ret___, typeName ___ret___];
            [___name___, ___desc___, ___index___, ___msg___] call test_fnc_testFailed;
        }
    }] call test_fnc_exceptWrapper;
};

test_fnc_assertException = {
    [_this, {
        params["___name___", "___test___", "___desc___", "___index___"];
        {
            private ___ret___ = call ___test___;
            private ___msg___ = format["Never reached except. Returned: %1", ___ret___];
            [___name___, ___desc___, ___index___, ___msg___] call test_fnc_testFailed;
        }
        except__
        {
            [___name___, ___desc___, ___index___] call test_fnc_testPassed;
        }
    }] call test_fnc_exceptWrapper;
};

test_fnc_exceptWrapper = {
    params["___exceptWrapper_args___", "___exceptWrapper_code___"];
    {
        ___exceptWrapper_args___ call ___exceptWrapper_code___
    }
    except__
    {
        private ___msg___ = format["Exception occurred: %1",  _exception];
        [___exceptWrapper_args___ select 0, ___exceptWrapper_args___ select 2, ___exceptWrapper_args___ select 3, ___msg___] call test_fnc_testFailed;
    }
};
test_fnc_setupWrapper = {
    params["___setupWrapper_args___", "___setupWrapper_code___"];
    {
        ___setupWrapper_args___ call ___setupWrapper_code___
    }
    except__
    {
        private ___msg___ = format["Exception occurred during setup: %1",  _exception];
        fatalError = true;
    }
};
test_fnc_cleanup_carraige_return = {
    params["___text___"];
    toString (toArray ___text___ select { /* take all chars but carraige return '\r' */ _x != 13 });
};

private ___currentDirectory___ = currentDirectory__;
private ___currentDirectoryLength___ = count ___currentDirectory___;
private ___exceptions___ = [];
private ___failed___ = [];
diag_log "Loading tests from:";
diag_log format["    %1", ___currentDirectory___];

private ___sqf_test_dir = currentDirectory__ + "/" + "sqf";
private ___preprocessor_test_dir = currentDirectory__ + "/" + "preprocess";
{
    DIAGNOSTICS_EXEC(format["%1 out of %2 tests passed." COMMA testsPassed COMMA testsIndex]);
    DIAGNOSTICS_EXEC(format["Checking if '%1' is '%2'.", _x, ___sqf_test_dir]);
    if (_x select[0, count ___sqf_test_dir] == ___sqf_test_dir) then
    {
        {
            private ___name___ = _x select[___currentDirectoryLength___];
            private ___tests___ = call compile preprocessFileLineNumbers _x;
            private ___setup___ = { [] call _this; };
            if !(___tests___ isEqualType[]) then
            {
                throw format["Invalid type. Expected ARRAY; Got %1", typeName ___tests___];
                fatalError = true;
            };
            {
                DIAGNOSTICS_EXEC(format["%1 out of %2 tests passed." COMMA testsPassed COMMA testsIndex]);
                testsIndex = testsIndex + 1;
                private ___mode___ = _x select 0;
                private ___test___ = _x select 1;
                private ___desc___ = if (___test___ isEqualType[]) then { ___test___ select 0 } else { str(___test___) };
                private ___code___ = if (___test___ isEqualType[]) then { ___test___ select 1 } else { ___test___ };
                
                private ___res___ = false;
                if (___mode___ isEqualType "") then
                {
                    switch (___mode___) do
                    {
                        case "setup": {
                            DIAGNOSTICS_EXEC("___mode___ is setup");
                            ___setup___ = ___code___;
                            testsIndex = testsIndex - 1;
                        };
                        case "assert": {
                            DIAGNOSTICS_EXEC("___mode___ is assert");
                            [{
                                [___name___, ___code___, ___desc___, _forEachIndex, true] call test_fnc_assert
                            }, ___setup___] call test_fnc_setupWrapper;
                        };
                        case "assertTrue": {
                            DIAGNOSTICS_EXEC("___mode___ is assertTrue");
                            [{
                                [___name___, ___code___, ___desc___, _forEachIndex, true] call test_fnc_assertEqual
                            }, ___setup___] call test_fnc_setupWrapper;
                        };
                        case "assertFalse": {
                            DIAGNOSTICS_EXEC("___mode___ is assertFalse");
                            [{
                                [___name___, ___code___, ___desc___, _forEachIndex, false] call test_fnc_assertEqual
                            }, ___setup___] call test_fnc_setupWrapper;
                        };
                        case "assertEqual": {
                            DIAGNOSTICS_EXEC("___mode___ is assertEqual");
                            [{
                                [___name___, ___code___, ___desc___, _forEachIndex, _x select 2] call test_fnc_assertEqual
                            }, ___setup___] call test_fnc_setupWrapper;
                        };
                        case "assertNil";
                        case "assertIsNil": {
                            DIAGNOSTICS_EXEC("___mode___ is assertNil");
                            [{
                                [___name___, ___code___, ___desc___, _forEachIndex] call test_fnc_assertIsNil
                            }, ___setup___] call test_fnc_setupWrapper;
                        };
                        case "assertExcept";
                        case "assertException": {
                            DIAGNOSTICS_EXEC("___mode___ is assertException");
                            [{
                                [___name___, ___code___, ___desc___, _forEachIndex] call test_fnc_assertException
                            }, ___setup___] call test_fnc_setupWrapper;
                        };
                        default {
                            throw format["Unknown Test-Type %1 in %2-%3 (%4)", ___mode___, ___name___, _forEachIndex + 1, ___desc___];
                            fatalError = true;
                        }
                    }
                }
                else
                {
                    if (___mode___ isEqualType {}) then
                    {
                        DIAGNOSTICS_EXEC("___mode___ is CODE");
                        ___res___ = [___name___, ___test___, _forEachIndex, _x] call ___mode___;
                    }
                    else
                    {
                        throw format["Test-Type was expected to be either STRING or CODE but was %1", typeName ___mode___];
                        fatalError = true;
                    };
                };
            } forEach ___tests___;
        }
        except__
        {
            private _msg = format["Exception during test execution of %1: %2%3", _x, endl, _exception];
            diag_log _msg;
            ___exceptions___ pushBack _msg;
            fatalError = true;
        };
    }
    else
    {
        DIAGNOSTICS_EXEC(format["Checking if '%1' is '%2'.", _x, ___preprocessor_test_dir]);
        if (_x select[0, count ___preprocessor_test_dir] == ___preprocessor_test_dir) then
        {
            {
                testsIndex = testsIndex + 1;
                private ___fpath___ = _x;
                private ___actual___ = [preprocess__ loadFile ___fpath___] call test_fnc_cleanup_carraige_return;
                private ___expected___ = [loadFile ((_x select[0, count _x - 3]) + "txt")] call test_fnc_cleanup_carraige_return;
                
                DIAGNOSTICS_EXEC((_x select[0 COMMA count _x - 3]) + "txt");
                DIAGNOSTICS_EXEC(format["___expected___: %1" COMMA toArray (___expected___)]);
                DIAGNOSTICS_EXEC(___expected___);
                DIAGNOSTICS_EXEC(___fpath___);
                DIAGNOSTICS_EXEC(format["___actual___:   %1" COMMA toArray (___actual___)]);
                DIAGNOSTICS_EXEC(___actual___);
                
                private ___name___ = _x select[___currentDirectoryLength___];
                [___name___, { ___actual___ }, "", 0, ___expected___] call test_fnc_assertEqual;
            }
            except__
            {
                private _msg = format["Exception during test execution of %1: %2%3", _x, endl, _exception];
                diag_log _msg;
                ___exceptions___ pushBack _msg;
                fatalError = true;
            };
        };
    };
} forEach allFiles__[".sqf"];
diag_log "############################################################";
diag_log format["%1 out of %2 tests passed.", testsPassed, testsIndex];
diag_log "############################################################";
{
    diag_log _x;
} forEach ___failed___;
if (testsPassed != testsIndex) then
{
    diag_log "############################################################";
    diag_log format["%1 out of %2 tests passed.", testsPassed, testsIndex];
    diag_log "############################################################";
};
if (fatalError) then
{
    diag_log "FATALERROR occured during testing:";
    {
        diag_log _x;
    } forEach ___exceptions___;
    exitCode__ - 1;
}
else
{
    diag_log "";
    diag_log(["FAILED", "SUCCESS"] select(testsPassed == testsIndex));
    exitcode__(testsIndex - testsPassed);
};