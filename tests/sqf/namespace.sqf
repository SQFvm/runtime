[   
    ["assertEqual",      { private _ns = createNamespace__ "custom"; _ns setVariable ["TEST", 0]; allVariables _ns }, ["TEST"]],
    ["assertEqual",      { private _ns = createNamespace__ "custom"; _ns setVariable ["TEST", 0]; with _ns do { allVariables currentNamespace } }, ["TEST"]],
    ["assertNil",        { missionNamespace getVariable "don'texist" }],
    ["assertNil",        { missionNamespace setVariable ["nstest", true] }],
    ["assertEqual",      { missionNamespace getVariable "nstest" }, true],
    ["assertEqual",      { missionNamespace getVariable ["nstest", false] }, true],
    ["assertEqual",      { missionNamespace getVariable ["don'texist", false] }, false],
    ["assertEqual",      { allVariables player }, []],
    ["assertNil",        { player getVariable "don'texist" }],
	["assertNil",        { player setVariable ["nstest", true] }],
    ["assertEqual",      { player getVariable "nstest" }, true],
    ["assertEqual",      { player getVariable ["nstest", false] }, true],
    ["assertEqual",      { player getVariable ["don'texist", false] }, false],
    ["assertException",  { player getVariable [false] }, []],
    ["assertException",  { player setVariable [false] }, []],
    ["assertException",  { player setVariable ["test"] }, []],
    ["assertException",  { player setVariable [true, "test"] }, []],
    ["assertException",  { player getVariable [true, "test"] }, []],
	["assertEqual",      { allVariables objNull }, []],
    ["assertNil",        { objNull getVariable "don'texist" }],
	["assertNil",        { objNull setVariable ["nstest", true] }],
    ["assertNil",        { objNull getVariable "nstest" }],
    ["assertNil",        { objNull getVariable ["nstest", false] }],
    ["assertNil",        { objNull getVariable ["don'texist", false] }],
    ["assertNil",        { objNull getVariable [false] }],
    ["assertNil",        { objNull setVariable [false] }],
    ["assertNil",        { objNull setVariable ["test"] }]








]
