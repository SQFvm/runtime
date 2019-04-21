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
    ["assertEqual",  { objNull getVariable "don'texist" }, []],
	//["assertEqual",  { objNull setVariable ["nstest", true] }], //#TODO this fails for some reason
    ["assertEqual",  { objNull getVariable "nstest" }, []],
    ["assertEqual",  { objNull getVariable ["nstest", false] }, []],
    ["assertEqual",  { objNull getVariable ["don'texist", false] }, []],
    ["assertEqual",  { objNull getVariable [false] }, []],
    ["assertEqual",  { objNull setVariable [false] }, []],
    ["assertEqual",  { objNull setVariable ["test"] }, []]








]
