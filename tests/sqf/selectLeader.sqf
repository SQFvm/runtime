[   ["setup",      { private _group = grpNull; private _unitA = objNull; private _unitB = objNull; [] call _this; todo: delete}],
    ["assert",     { grpNull selectLeader objNull; }, ""],
    ["assert",     { grpNull selectLeader player; }, ""],
    ["assert",     { grpNull selectLeader objNull; }, ""],
    ["assertEqual",     { format ["%%"]; }, ""],
    ["assertEqual",     { format ["%%%"]; }, ""],
    ["assertEqual",     { format ["%1", true]; }, "true"],
    ["assertEqual",     { format ["%1", false]; }, "false"],
    ["assertEqual",     { format ["%1", 1]; }, "1"],
    ["assertEqual",     { format ["%1", []]; }, "[]"],
    ["assertEqual",     { format ["%1", [1,2,3]]; }, "[1,2,3]"],
    ["assertEqual",     { format ["%1", [[]]]; }, "[[]]"],
    ["assertEqual",     { format ["%1", "test"]; }, "test"]
]