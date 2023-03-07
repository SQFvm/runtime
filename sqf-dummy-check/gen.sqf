// call compileScript ["gen.sqf"];
// ..\build\Release\sqfvm.exe --input-sqf .\delta.sqf

private _ops_list =
#include "ops.sqf"
;

private _toAdd = [];
(supportInfo "") apply {
	if (_x == "b:SWITCH : CODE") then { continue };
	if (_x == "b:SWITCH do CODE") then { continue };
	_x splitString ":" params ["_t", "_x"];
	if (_t != "t") then {
		_x = _x splitString " ";
		private _cmd = switch count _x do {
			case 1: { ["n", (_x # 0)] };
			case 2: { ["u", (_x # 0)] };
			case 3: { ["b", (_x # 1), 4] };
		};
        if ((_ops_list findIf {(_x select [0,2]) isEqualTo (_cmd select [0,2])}) == -1) then {
            _toAdd pushBackUnique str _cmd;
        };
	};
};
copyToClipboard (_toAdd apply {format ["%1,", _x]} joinString endl);
format ["found %1", count _toAdd];