

testsIndex = 0;
testsPassed = 0;
testsFailed = 0;
fatalError = false;

test_fnc_testPassed = {
    params ["_name", "_index"];
    systemChat format ["Test  Passed  '%1' - %2.", _name, _index + 1];
    testsPassed = testsPassed + 1;
};

test_fnc_testFailed = {
    params ["_name", "_index", "_reason"];
    systemChat format ["Test !FAILED! '%1' - %2: %3.", _name, _index + 1, trim__ _reason];
    testsFailed = testsFailed + 1;
};

test_fnc_exceptWrapper = {
    params ["_args", "_code"];
    {
        _args call _code
    }
    except__
    {
        private _msg = format ["Exception occurred: %1",  trim__ _exception];
        [_args select 0, _args select 2, _msg] call test_fnc_testFailed;
    }
};

test_fnc_assertEqual = {
    [_this, {
        params ["_name", "_test", "_index", "_compare"];
        private _ret = call _test;
        if (_ret isEqualTo _compare) then
        {
            [_name, _index] call test_fnc_testPassed;
        }
        else
        {
            private _msg = format ["Wrong return value. Expected %1 (type %2), got %3 (type %4).",  _compare, typeName _compare, _ret, typeName _ret];
            [_name, _index, _msg] call test_fnc_testFailed;
        }
    }] call test_fnc_exceptWrapper;
};

test_fnc_assertIsNil = {
    [_this, {
        params ["_name", "_test", "_index", "_compare"];
        private _ret = call _test;
        if (isNil "_ret") then
        {
            [_name, _index] call test_fnc_testPassed;
        }
        else
        {
            private _msg = format ["Wrong return value. Expected nil, got %1 (type %2).",  _ret, typeName _ret];
            [_name, _index, _msg] call test_fnc_testFailed;
        }
    }] call test_fnc_exceptWrapper;
};

test_fnc_assertException = {
    params ["_name", "_test", "_index"];
    {
        private _ret = call _test;
        private _msg = format ["Never reached except. Returned: %1", _ret];
        [_name, _index, _msg] call test_fnc_testFailed;
    }
    except__
    {
        [_name, _index] call test_fnc_testPassed;
    }
};

private _currentDirectory = currentDirectory__;
private _currentDirectoryLength = count _currentDirectory;

diag_log "Loading tests from:";
diag_log format ["    %1", _currentDirectory];

{
    if !(_x isEqualTo pwd__) then {
        if (count _x > _currentDirectoryLength) then {
            if (_x select [0, _currentDirectoryLength] == _currentDirectory) then
            {
                {
                    private _name = _x select [_currentDirectoryLength];
                    private _tests = call compile preprocessFileLineNumbers _x;
                    if !(_tests isEqualType []) then
                    {
                        throw format ["Invalid type. Expected ARRAY; Got %1", typeName _tests];
                    };
                    {
                        private _mode = _x select 0;
                        private _test = _x select 1;
                        private _res = false;
                        if (_mode isEqualType "") then
                        {
                            switch (_mode) do
                            {
                                case "assertTrue": { [_name, _test, _forEachIndex, true] call test_fnc_assertEqual };
                                case "assertFalse": { [_name, _test, _forEachIndex, false] call test_fnc_assertEqual };
                                case "assertEqual": { [_name, _test, _forEachIndex, _x select 2] call test_fnc_assertEqual };
                                case "assertIsNil": { [_name, _test, _forEachIndex] call test_fnc_assertIsNil };
                                case "assertException": { [_name, _test, _forEachIndex] call test_fnc_assertException };
                                default { throw format ["Unknown Test-Type %1 in %2", _mode, _name]; }
                            }
                        }
                        else
                        {
                            if (_mode isEqualType {}) then
                            {
                                _res = [_name, _test, _forEachIndex, _x] call _mode;
                            }
                            else
                            {
                                throw format ["Test-Type was expected to be either STRING or CODE but was %1", typeName _mode];
                            };
                        };
                        testsIndex = testsIndex + 1;
                    } forEach _tests;
                }
                except__
                {
                    diag_log format ["Exception during test execution of %1: %2", _name, _exception];
                    fatalError = true;
                };
            };
        };
    };
} forEach allFiles__ [".sqf"];
diag_log format ["%1 out of %2 tests passed.", testsPassed, testsIndex];
if (fatalError) then
{
    diag_log "fatalError occured during testing.";
};