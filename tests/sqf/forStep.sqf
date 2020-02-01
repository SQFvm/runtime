[   ["assertEqual",     { for "_i" from 1 to 10 do { _i }; }, 10],
    ["assertEqual",     { private _arr = []; for "_i" from 1 to 3 do { _arr pushBack _i; }; _arr }, [1, 2, 3]],
    ["assertEqual",     { private _arr = []; for "_i" from 0 to 5 step 2 do { _arr pushBack _i; }; _arr }, [0, 2, 4]],
    ["assertEqual",     { for "_i" from -1 to -10 step -1 do { _i }; }, -10],
    ["assertEqual",     { private _arr = []; for "_i" from (-1) to (-3) step -1 do { _arr pushBack _i; }; _arr }, [-1, -2, -3]],
    ["assertEqual",     { private _arr = []; for "_i" from 0 to (-5) step (-2) do { _arr pushBack _i; }; _arr }, [0, -2, -4]]
]