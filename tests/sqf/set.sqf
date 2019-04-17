[   ["assertEqual",     { private _arr = [];    _arr set [0,1]; _arr }, [1]],                       // ARRAY set ARRAY
    ["assertEqual",     { private _arr = [0];   _arr set [0,1]; _arr }, [1]],                       // ARRAY set ARRAY
    ["assertEqual",     { private _arr = [0];   _arr set [0,""]; _arr }, [""]],                     // ARRAY set ARRAY
    ["assertEqual",     { private _arr = [0];   _arr set [1,1]; _arr }, [0,1]],                     // ARRAY set ARRAY
    ["assertEqual",     { private _arr = [0];   _arr set [1,""]; _arr }, [0,""]],                   // ARRAY set ARRAY
    ["assertIsNil",     { private _arr = [];    _arr set [1,1]; _arr#0 }],                          // ARRAY set ARRAY
    ["assertEqual",     { private _arr = [0];   _arr set [count _arr,1]; _arr }, [0,1]],            // ARRAY set ARRAY
    ["assertEqual",     { private _arr = [0];   _arr set [count _arr - 1, 1]; _arr }, [1]],         // ARRAY set ARRAY
    ["assertException", { [] set [-1,1] }]                                                          // ARRAY set ARRAY
]