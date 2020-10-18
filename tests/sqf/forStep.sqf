[   ["assertEqual",     { for "_i" from 1 to 10 do { _i }; }, 10],
    ["assertEqual",     { private _arr = []; for "_i" from 1 to 3 do { _arr pushBack _i; }; _arr }, [1, 2, 3]],
    ["assertEqual",     { private _arr = []; for "_i" from 0 to 5 step 2 do { _arr pushBack _i; }; _arr }, [0, 2, 4]],
    ["assertEqual",     { for "_i" from -1 to -10 step -1 do { _i }; }, -10],
    ["assertEqual",     { private _arr = []; for "_i" from (-1) to (-3) step -1 do { _arr pushBack _i; }; _arr }, [-1, -2, -3]],
    ["assertEqual",     { private _arr = []; for "_i" from 0 to (-5) step (-2) do { _arr pushBack _i; }; _arr }, [0, -2, -4]],
    ["assert",     		{ for "_i" from 0 to -1 do { assert false }; }],
    ["assert",     		{ for "_i" from -1 to 0 step -1 do { assert false }; }],
    ["assertEqual",
        [
            "Frame-Variables Cleared before each repetition.",
            {
                private _out = [];
                for "_i" from 0 to 2 do
                {
                    _i = _i + 1;
                    if !isNil "_something" then
                    {
                        _out pushBack _i;
                    };
                    _something = 1;
                    false
                };
                _out
            }
        ], []
    ],
    ["assert",
        [
            "Loop with step=0 loops endless.",
            {
                private _i = 0;
				for "_" from 0 to 1 step 0 do
				{
					_i = _i + 1;
					if (_i == 2) exitWith {};
				};
				assert (_i == 2);
            }
        ]
    ]
]