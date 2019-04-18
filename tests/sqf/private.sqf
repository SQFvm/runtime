[   ["assertEqual", ["private STRING no override test", { private _arr = []; { private "_x"; _arr pushBack _x; } foreach [1,2,3]; _arr }], [1,2,3] ],   // private STRING
    ["assertEqual", ["private ARRAY no override test", { private _arr = []; { private ["_x"]; _arr pushBack _x; } foreach [1,2,3]; _arr }], [1,2,3] ],  // while CODE do CODE: condition false
    ["assertEqual", { private _private = 0; [] call { private "_private"; _private = 1; }; _private }, 0],
    ["assertIsNil", { [] call { private "_private"; _private = 1; }; _private }],
    ["assertEqual", { private _private = 0; [] call { private ["_private"]; _private = 1; }; _private }, 0],
    ["assertIsNil", { [] call { private ["_private"]; _private = 1; }; _private }]
]