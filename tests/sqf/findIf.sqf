[   ["assertEqual",     { [1, 2, 3] findIf { _x == 1 } }, 0],
    ["assertEqual",     { [1, 2, 3] findIf { _x == 2 } }, 1],
    ["assertEqual",     { [1, 2, 3] findIf { _x == 3 } }, 2],
    ["assertEqual",     { [1, 2, 3] findIf { _x > 5 } }, -1],
    ["assertEqual",     { [] findIf {_x > 1} }, -1],
    ["assertException", { private _i = 0; private _arr = [1, 2, 3]; _arr findIf { _i = _i + 1; if (_i > 3) then { throw "Abort Endless Loop" }; _arr pushBack _x; false } }],
    ["assertEqual",
        [
            "Frame-Variables Cleared before each repetition.",
            {
                private _out = [];
                [1, 2] findIf
                {
                    if !isNil "_something" then {
                        _out pushBack _i;
                    };
                    _something = 1;
                    false
                };
                _out
            }
        ], []
    ]
]