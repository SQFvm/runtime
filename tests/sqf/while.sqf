[   ["assertEqual",     { private _i = 0; while { false } do { _i = 1 }; _i }, 0],                // while CODE do CODE: condition false
    ["assertIsNil",     { while { false } do {} } ],                                              // while CODE do CODE: condition false
    ["assertException", { while { } do {} } ],                                                    // while CODE do CODE: condition empty
    ["assertException", { while { "" } do {} } ],                                                 // while CODE do CODE: condition not BOOL
    ["assertIsNil",     { while { nil } do {} } ],                                                // while CODE do CODE: condition nil
    ["assertEqual",     { private _i = 0; while { _i == 0 } do { _i = 1 }; _i }, 1],              // while CODE do CODE: condition true before body executed
    ["assertEqual",     { private _i = 0; while { _i = _i + 1; _i < 5 } do {}; _i }, 5],          // while CODE do CODE: empty body
    ["assertEqual",     { private _i = 0; while { _i = _i + 1; _i < 5 } do { nil }; _i }, 5],     // while CODE do CODE: nil body
    ["assertEqual",     { private _i = 0; while { _i < 5 } do { _i = _i + 1; }; _i }, 5],         // while CODE do CODE: normal body
    ["assertEqual",
        [
            "Frame-Variables Cleared before each repetition.",
            {
                private _arr = [];
                private _i = 0;
                while { _i < 2 } do
                {
                    _i = _i + 1;
                    if !isNil "_something" then
                    {
                        _arr pushBack _i;
                    };
                    _something = 1;
                };
                _arr
            }
        ], []
    ]
]