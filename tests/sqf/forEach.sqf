[   ["assertEqual", { private _arr2 = [0,1,2,3,4,5]; { _arr2 deleteAt _x; } forEach _arr2; _arr2 }, [1,2,4,5]],                                                 // CODE forEach ARRAY
    ["assertEqual", { private _arr2 = [5,4,3,2,1,0]; { _arr2 deleteAt _x; } forEach _arr2; _arr2 }, [5,4,3]],                                                    // CODE forEach ARRAY
    ["assertException", { private _i = 0; private _arr = [1,2,3]; { _i = _i + 1; if (_i > 3) then { throw "Abort Endless Loop" }; _arr pushBack _x; } forEach _arr }],
    ["assertEqual",
        [
            "Frame-Variables Cleared before each repetition.",
            {
                private _arr = [];
                {
                    if !isNil "_i" then
                    {
                        _arr pushBack _i;
                    };
                    _i = 1;
                } forEach [1,2,3];
                _arr
            }
        ], []
    ],
    ["assertEqual",
        [
            "Frame iterates all values.",
            {
                private _arr = [];
                {
                    _arr pushBack _x;
                } forEach [1,2,3];
                _arr
            }
        ], [1, 2, 3]
    ]
]