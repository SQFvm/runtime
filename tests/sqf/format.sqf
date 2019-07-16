[   ["assertEqual",     { format ["%"]; }, ""],
    ["assertEqual",     { format ["%1"]; }, ""],
    ["assertEqual",     { format ["%2", 1]; }, ""],
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