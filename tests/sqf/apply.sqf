[   ["assertEqual",  { private _arr = [1,0,1]; [_arr, _arr apply { _arr set [_x, 0]; 9 }]}, [[0,0,1], [9,9,9]]],
    ["assertException", { private _i = 0; private _arr = [1,2,3]; { _i = _i + 1; if (_i > 3) then { throw "Abort Endless Loop" }; _arr pushBack _x; 0 } apply _arr }],
    ["assertEqual",
    [
        "Frame-Variables Cleared before each repetition.",
        {
            private _out = [];
            [1, 2] apply
            {
                if !isNil "_something" then
                {
                    _out pushBack _something;
                };
                _something = 1;
                false
            };
            _out
        }
    ], []]
]