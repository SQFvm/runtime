missionNamespace setVariable ["nstest", true];
private _vars = allVariables missionNamespace;

[   
    ["assertEqual",  { allVariables missionNamespace }, _vars],
    ["assertException",  { allVariables currentNamespace }, _vars],//not yet implemented
    ["assertException",  { with missionNamespace do {allVariables currentNamespace} }, _vars],//not yet implemented
    ["assertIsNil",  { missionNamespace getVariable "don'texist" }],
    ["assertIsNil",  { missionNamespace setVariable ["nstest", true] }],
    ["assertEqual",  { missionNamespace getVariable "nstest" }, true],
    ["assertEqual",  { missionNamespace getVariable ["nstest", false] }, true],
    ["assertEqual",  { missionNamespace getVariable ["don'texist", false] }, false],
    ["assertEqual",  { allVariables player }, []],
    ["assertisNil",  { player getVariable "don'texist" }],
	["assertIsNil",  { player setVariable ["nstest", true] }],
    ["assertEqual",  { player getVariable "nstest" }, true],
    ["assertEqual",  { player getVariable ["nstest", false] }, true],
    ["assertEqual",  { player getVariable ["don'texist", false] }, false],
    ["assertException",  { player getVariable [false] }, []],
    ["assertException",  { player setVariable [false] }, []],
    ["assertException",  { player setVariable ["test"] }, []],
    ["assertException",  { player setVariable [true, "test"] }, []],
    ["assertException",  { player getVariable [true, "test"] }, []],

	["assertEqual",  { allVariables objNull }, []],
    ["assertisNil",  { objNull getVariable "don'texist" }],
	["assertisNil",  { objNull setVariable ["nstest", true] }],
    ["assertisNil",  { objNull getVariable "nstest" }],
    ["assertisNil",  { objNull getVariable ["nstest", false] }],
    ["assertisNil",  { objNull getVariable ["don'texist", false] }],
    ["assertisNil",  { objNull getVariable [false] }],
    ["assertisNil",  { objNull setVariable [false] }],
    ["assertisNil",  { objNull setVariable ["test"] }]








]
