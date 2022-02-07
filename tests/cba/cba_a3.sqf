private _addons = ["accessorys", "ai", "arrays", "common", "diagnostic", "disposable", "ee", "events", "hashes", "help", "jam", "jr", "keybinding", "main", "main_a3", "modules", "music", "network", "optics", "settings", "statemachine", "strings", "ui", "vectors", "versioning", "xeh"];
private _tests = ["arrays", "common", "diagnostic", "events", "hashes", "network", "strings", "vectors"]; // "jam" disabled, "main" is all tests
private _functions = [];

{
  private _addons = _x;
  private _addonPath = format ["\x\cba\addons\%1", _x];
  private _addonSqfFiles = allFiles__ [".sqf"];
  {
    private _addonFunctionPrefix = format ["%1/fnc_", _addons];
    if (_x find _addonFunctionPrefix > 0) then {
      private _splitStr = _x splitString "/";
      private _filename = _splitStr select (count _splitStr - 1);
      private _filePath = format ["%1\%2", _addonPath, _filename];

      if (not (_filePath in _functions)) then {
        private _functionName = "CBA_fnc_" + (_filename select [4, count _filename - 8]);
        missionNamespace setVariable [_functionName, compile preprocessFileLineNumbers _filePath];
        _functions pushBackUnique _filePath;
      };
    };
  } forEach _addonSqfFiles;
} forEach _addons;

{
  call compile preprocessFileLineNumbers  format ["\x\cba\addons\%1\test.sqf", _x];
} forEach _tests;

nil;
