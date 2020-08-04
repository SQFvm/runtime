[   
    ["assertTrue",      { diag_tickTime > 0 }],
    ["assertTrue",      { assert(true) }],
    ["assertException", { assert(false) }],
	["assertEqual",     { productVersion }, productVersion],
	["assertEqual",     { typename "" }, "STRING"],
	["assertEqual",     { typename 1 }, "SCALAR"],
	["assertEqual",     { typename player }, "OBJECT"],
	["assertEqual",     { typename missionNamespace }, "NAMESPACE"],
	["assertEqual",     { typename grpNull }, "GROUP"], //#TODO add the other types
	["assertNil",       { comment "hah!" }],
	["assertException", { if (true) then [0,0] }, "STRING"],
	["assertException", { if (false) then [0,0] }, "STRING"],
	["assertNil",       { if (false) exitWith {false} }, "STRING"],
	["assertTrue",      { if (true) exitWith {true} }, "STRING"],
	["assertTrue",      { if (true) then {true} else {false} }, "STRING"],
	["assertFalse",     { if (true) then {false} else {true} }, "STRING"]
]