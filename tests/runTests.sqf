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
test_fnc_assertTrue = {
    private _name = _this select 0;
    private _test = _this select 1;
    testsIndex = testsIndex + 1;
    private _index = testsIndex;
    {
        private _ret = call _test;
        if (_ret isEqualTo true) then
        {
            systemChat format ["Test %1 - '%2' Passed.", _index, _name];
            testsPassed = testsPassed + 1;
        }
        else
        {
            if (_ret isEqualType false) then
            {
                systemChat format ["Test %1 - '%2' Failed.", _index, _name];
                testsFailed = testsFailed + 1;
            }
            else
            {
                systemChat format ["Test %1 - '%2' Failed due to wrong return value. Expected BOOL, got %3 (%4).", _index, _name, typeName _ret, _ret];
                testsFailed = testsFailed + 1;
            }
        }
    }
    except__
    {
        systemChat format ["Test %1 - '%2' Failed: %3", _index, _name, _exception];
        testsFailed = testsFailed + 1;
    }
};
test_fnc_assertException = {
    private _name = _this select 0;
    private _test = _this select 1;
    testsIndex = testsIndex + 1;
    private _index = testsIndex;
    {
        private _ret = call _test;
        systemChat format ["Test %1 - '%2' Failed: Never reached except. Returned: %3", _index, _name, _ret];
        testsFailed = testsFailed + 1;
    }
    except__
    {
        systemChat format ["Test %1 - '%2' Passed with: %3", _index, _name, _exception];
        testsPassed = testsPassed + 1;
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
                    private  _tests = call compile preprocessFileLineNumbers _x;
                    if !(_tests isEqualType []) then
                    {
                        throw format ["Invalid type. Expected ARRAY; Got %1", typeName _tests];
                    };
                    {
                        private _mode = _x select 0;
                        private _test = _x select 1;
                        switch (_mode) do
                        {
                            case "assertTrue": { [_name, _test] call test_fnc_assertTrue };
                            case "assertException": { [_name, _test] call test_fnc_assertException };
                            default { throw format ["Unknown Test-Type %1 in %2", _mode, _name]; }
                        }
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
systemChat format ["%1 out of %2 tests failed.", testsFailed, testsIndex];
if (fatalError) then {
    diag_log "fatalError was set.";
};