

testsIndex = 0;
testsPassed = 0;
testsFailed = 0;
fatalError = false;
test_fnc_assertTrue = {
    private _name = _this select 0;
    private _test = _this select 1;
    private _forEachIndex = _this select 2;
    testsIndex = testsIndex + 1;
    private _index = testsIndex;
    {
        private _ret = call _test;
        if (_ret isEqualTo true) then
        {
            // diag_log format ["Test '%1' - %2 Passed.", _name, _forEachIndex];
            testsPassed = testsPassed + 1;
            true
        }
        else
        {
            if (_ret isEqualType false) then
            {
                diag_log format ["Test '%1' - %2 Failed.", _name, _forEachIndex];
                testsFailed = testsFailed + 1;
                false
            }
            else
            {
                diag_log format ["Test '%1' - %2 Failed due to wrong return value. Expected BOOL, got %3 (%4).", _name, _forEachIndex, typeName _ret, _ret];
                testsFailed = testsFailed + 1;
                false
            }
        }
    }
    except__
    {
        diag_log format ["Test '%1' - %2 Failed: %3", _name, _forEachIndex, trim__ _exception];
        testsFailed = testsFailed + 1;
        false
    }
};
test_fnc_assertException = {
    private _name = _this select 0;
    private _test = _this select 1;
    testsIndex = testsIndex + 1;
    private _index = testsIndex;
    {
        private _ret = call _test;
        diag_log format ["Test '%1' - %2 Failed: Never reached except. Returned: %3", _name, _forEachIndex, _ret];
        testsFailed = testsFailed + 1;
        false
    }
    except__
    {
        // diag_log format ["Test '%1' - %2 Passed with: %3", _name, _forEachIndex, trim__ _exception];
        testsPassed = testsPassed + 1;
        true
    }
};
private _currentDirectory = currentDirectory__;
private _currentDirectoryLength = count _currentDirectory;

diag_log "Loading tests from:";
diag_log format ["    %1", _currentDirectory];

diag_log "Test-Suit requires following commands to work:";
diag_log "   - ARRAY select SCALAR";
diag_log "   - SCALAR + SCALAR";
diag_log "   - call CODE";
diag_log "   - if BOOL";
diag_log "   - IF then CODE";
diag_log "   - IF then ARRAY";
diag_log "   - ANY isEqualTo ANY";
diag_log "   - ANY isEqualType ANY";
diag_log "   - format ARRAY";
diag_log "   - diag_log ANY";
diag_log "   - typeName ANY";
diag_log "   - switch ANY";
diag_log "   - SWITCH do CODE";
diag_log "   - case ANY";
diag_log "   - SWITCH : CODE";
diag_log "   - default CODE";
diag_log "   - compile STRING";
diag_log "   - preprocessFileLineNumbers STRING";
diag_log "   - forEach ARRAY";

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
                        private _res = false;
                        if (_mode isEqualType "") then
                        {
                            switch (_mode) do
                            {
                                case "assertTrue": { _res = [_name, _test, _forEachIndex, _x] call test_fnc_assertTrue };
                                case "assertException": { _res = [_name, _test, _forEachIndex, _x] call test_fnc_assertException };
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