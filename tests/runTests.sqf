diag_log "Theese tests require that the following commands work:";
diag_log "   - ARRAY select SCALAR";
diag_log "   - SCALAR + SCALAR";
diag_log "   - call CODE";
diag_log "   - if BOOL";
diag_log "   - IF then CODE";
diag_log "   - IF then ARRAY";
diag_log "   - ANY isEqualTo ANY";
diag_log "   - ANY isEqualType ANY";
diag_log "   - format ARRAY";
diag_log "   - systemChat STRING";
diag_log "   - typeName ANY";
diag_log "   - switch ANY";
diag_log "   - SWITCH do CODE";
diag_log "   - case ANY";
diag_log "   - SWITCH : CODE";
diag_log "   - default CODE";
diag_log "   - compile STRING";
diag_log "   - preprocessFileLineNumbers STRING";
diag_log "   - forEach ARRAY";

testsIndex = 0;
testsPassed = 0;
testsFailed = 0;
fatalError = false;

test_fnc_testPassed = {
    params ["_index", "_name"];
    systemChat format ["Test %1 - '%2' Passed.", _index, _name];
    testsPassed = testsPassed + 1;
};

test_fnc_testFailed = {
    params ["_index", "_name", "_reason"];
    systemChat format ["Test %1 - '%2' Failed: %3.", _index, _name, _reason];
    testsFailed = testsFailed + 1;
};

test_fnc_exceptWrapper = {
    params ["_args", "_code"];
    testsIndex = testsIndex + 1;
    private _index = testsIndex;
    {
        ([_index] + _args) call _code
    }
    except__
    {
        [_index, _args select 0, _exception] call test_fnc_testFailed;
    }
};

test_fnc_assertEqual = {
    [_this, {
        params ["_index", "_name", "_test", "_compare"];
        private _ret = call _test;
        if (_ret isEqualTo _compare) then
        {
            [_index, _name] call test_fnc_testPassed;
        }
        else
        {
            private _msg = format ["Wrong return value. Expected %1 (type %2), got %3 (type %4).",  _compare, typeName _compare, _ret, typeName _ret];
            [_index, _name, _msg] call test_fnc_testFailed;
        }
    }] call test_fnc_exceptWrapper;
};

test_fnc_assertIsNil = {
        [_this, {
        params ["_index", "_name", "_test", "_compare"];
        private _ret = call _test;
        if (isNil "_ret") then
        {
            [_index, _name] call test_fnc_testPassed;
        }
        else
        {
            private _msg = format ["Wrong return value. Expected nil, got %1 (type %2).",  _ret, typeName _ret];
            [_index, _name, _msg] call test_fnc_testFailed;
        }
    }] call test_fnc_exceptWrapper;
};

test_fnc_assertException = {
    private _name = _this select 0;
    private _test = _this select 1;
    testsIndex = testsIndex + 1;
    private _index = testsIndex;
    {
        private _ret = call _test;
        private _msg = format ["Never reached except. Returned: %1", _ret];
        [_index, _name, _msg] call test_fnc_testFailed;
    }
    except__
    {
        [_index, _name] call test_fnc_testPassed;
    }
};

private _currentDirectory = currentDirectory__;
private _currentDirectoryLength = count _currentDirectory;
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
                        if (_mode isEqualType "") then
                        {
                            switch (_mode) do
                            {
                                case "assertTrue": { [_name, _test, true] call test_fnc_assertEqual };
                                case "assertFalse": { [_name, _test, false] call test_fnc_assertEqual };
                                case "assertEqual": { [_name, _test, _x select 2] call test_fnc_assertEqual };
                                case "assertException": { [_name, _test] call test_fnc_assertException };
                                default { throw format ["Unknown Test-Type %1 in %2", _mode, _name]; }
                            }
                        }
                        else
                        {
                            if (_mode isEqualType {}) then
                            {
                                [_name, _test] call _mode;
                            }
                            else
                            {
                                throw format ["Test-Type was expected to be either STRING or CODE but was %1", typeName _mode];
                            };
                        };
                    } forEach _tests;
                }
                except__
                {
                    systemChat format ["Exception during test execution of %1: %2", _name, _exception];
                    fatalError = true;
                };
            };
        };
    };
} forEach allFiles__ [".sqf"];
diag_log _currentDirectory;
systemChat format ["%1 out of %2 tests passed.", testsPassed, testsIndex];
if (fatalError) then
{
    diag_log "fatalError occured during testing.";
};