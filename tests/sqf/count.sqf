[   ["assertEqual",     { count [] }, 0],                        //      count ARRAY
    ["assertEqual",     { count [1, 2, 3] }, 3],                 //      count ARRAY
    ["assertEqual",     { count [[1, 1, 1], [2, 2, 2]] }, 2],    //      count ARRAY
    ["assertEqual",     { count "123" }, 3],                     //      count STRING
    ["assertEqual",     { count "" }, 0],                        //      count STRING
    ["assertEqual",     { count "123456789" }, 9],               //      count STRING
    ["assertEqual",     { { _x == 0 } count [1, 2, 3] }, 0],     // CODE count ARRAY
    ["assertEqual",     { { _x == 1 } count [1, 2, 3] }, 1],     // CODE count ARRAY
    ["assertEqual",     { { _x == 1 } count [] }, 0],            // CODE count ARRAY
    ["assertEqual",     { { nil } count [1, 2, 3] }, 0],         // CODE count ARRAY
    ["assertEqual",     { { _x == 0 } count [] }, 0],            // CODE count ARRAY
    ["assertEqual",     { { _x == 0 } count [1, 2, 3] }, 0],     // CODE count ARRAY
    ["assertEqual",     { { _x == 0 } count [0] }, 1],           // CODE count ARRAY
    ["assertEqual",     { { _x == 0 } count [0, 0] }, 2],        // CODE count ARRAY
    ["assertEqual",     { {} count [0] }, 0],                    // CODE count ARRAY
    ["assertException", { { 1 } count [0] }],                    // CODE count ARRAY
    ["assertException", { { {} } count [0] }],                   // CODE count ARRAY
    ["assertException", { { [] } count [0] }],                   // CODE count ARRAY
    ["assertException", { {""} count [0] }],                     // CODE count ARRAY
    ["assertEqual",     { { _x > 1 } count [1, 2, 3] }, 2],      // CODE count ARRAY
    ["assertEqual",     { { _x > 1 } count [] }, 0],             // CODE count ARRAY
    ["assertException", { private _i = 0; private _arr = [1,2,3]; { _i = _i + 1; if (_i > 3) then { throw "Abort Endless Loop" }; _arr pushBack _x; false } count _arr }],
    ["assertEqual",
        [
            "Frame-Variables Cleared before each repetition.",
            {
                private _out = [];
                {
                    if !isNil "_something" then
                    {
                        _out pushBack _i;
                    };
                    _something = 1;
                    false
                }
                count [1, 2];
                _out
            }
        ], []
    ]
]