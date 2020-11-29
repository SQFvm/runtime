private _ops_list =
#include "ops.sqf"
;
private _ops_existing = cmds__ apply {
	switch (_x select 0) do
	{
		case "n": { ["n", _x select 1] };
		case "u": { ["u", _x select 1] };
		case "b": { ["b", _x select 2] };
	};
};
private _ops_missing = [];

{
	_x params ["_kind", "_name", "_precedence"];
	private _index = -1;
	switch _kind do
	{
		case "n": {
			private _index = _ops_existing find ["n",_name];
			if (_index == -1) then {
				diag_log format["MISSING NULAR  '%1'", _name];
				_ops_missing pushBack format [
					"runtime.register_sqfop(nular(""%1"", """"," +
					"[](sqf::runtime::runtime& runtime) -> value" + 
					"{ runtime.__logmsg(logmessage::runtime::ErrorMessage(runtime.context_active()" +
					".current_frame().diag_info_from_position(), ""NOT IMPLEMENTED"", ""%1"")); return {}; }));",
					_name
				];
			} else {
				diag_log format["FOUND   NULAR  '%1'", _name];
			}
		};
		case "u": {
			private _index = _ops_existing find ["u",_name];
			if (_index == -1) then {
				diag_log format["MISSING UNARY  '%1'", _name];
				_ops_missing pushBack format [
					"runtime.register_sqfop(unary(""%1"", t_any(), """"," +
					"[](sqf::runtime::runtime& runtime, value::cref r) -> value" + 
					"{ runtime.__logmsg(logmessage::runtime::ErrorMessage(runtime.context_active()" +
					".current_frame().diag_info_from_position(), ""NOT IMPLEMENTED"", ""%1"")); return {}; }));",
					_name
				];
			} else {
				diag_log format["FOUND   UNARY  '%1'", _name];
			}
		};
		case "b": {
			private _index = _ops_existing find ["b",_name];
			if (_index == -1) then {
				diag_log format["MISSING BINARY '%1'", _name];
				_ops_missing pushBack format [
					"runtime.register_sqfop(binary(%1, ""%2"", t_any(), t_any(), """"," +
					"[](sqf::runtime::runtime& runtime, value::cref l, value::cref r) -> value" + 
					"{ runtime.__logmsg(logmessage::runtime::ErrorMessage(runtime.context_active()" +
					".current_frame().diag_info_from_position(), ""NOT IMPLEMENTED"", ""%2"")); return {}; }));",
					_precedence,
					_name
				];
			} else {
				diag_log format["FOUND   BINARY '%1'", _name];
			}
		};
	};
} foreach _ops_list;

diag_log "Copying generated list to clipboard";
copyToClipboard (_ops_missing joinString endl);