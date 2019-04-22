

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
    params ["___name___", "___desc___", "___index___", "___reason___"];
    systemChat format ["Test !FAILED! '%1' - %2  %3", ___name___, ___index___ + 1, trim__ ___desc___];
    systemChat format ["    Reason: %1", trim__ ___reason___];
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

diag_log "Loading tests from:";
diag_log format ["    %1", ___currentDirectory___];

{  
    if (_x find "runTests" == -1) then {
        if (count _x > ___currentDirectoryLength___) then {
            if (_x select [0, ___currentDirectoryLength___] == ___currentDirectory___) then
            {
                {
                    private ___name___ = _x select [___currentDirectoryLength___];
                    private ___tests___ = call compile preprocessFileLineNumbers _x;
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
                                case "assertTrue": { [___name___, ___code___, ___desc___, _forEachIndex, true] call test_fnc_assertEqual };
                                case "assertFalse": { [___name___, ___code___, ___desc___, _forEachIndex, false] call test_fnc_assertEqual };
                                case "assertEqual": { [___name___, ___code___, ___desc___, _forEachIndex, _x select 2] call test_fnc_assertEqual };
                                case "assertIsNil": { [___name___, ___code___, ___desc___, _forEachIndex] call test_fnc_assertIsNil };
                                case "assertException": { [___name___, ___code___, ___desc___, _forEachIndex] call test_fnc_assertException };
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
                    diag_log format ["Exception during test execution of %1: %2", ___name___, _exception];
                    fatalError = true;
                };
            };
        };
    };
} forEach allFiles__ [".sqf"];
diag_log format ["%1 out of %2 tests passed.", testsPassed, testsIndex];
if (fatalError) then
{
    diag_log "FATALERROR occured during testing.";
    exit__ -1;
}
else 
{
    diag_log "";
    diag_log (["FAILED", "SUCCESS"] select (testsPassed == testsIndex));
    exit__ (testsIndex - testsPassed);
};
