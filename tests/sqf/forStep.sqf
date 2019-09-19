[   ["assertEqual",     { for "_i" from 1 to 10 do { _i }; }, 10],                                                  // for STRING from SCALAR to SCALAR do CODE
    ["assertEqual",     { private _arr = []; for "_i" from 1 to 3 do { _arr pushBack _i; }; _arr }, [1, 2, 3]],          // for STRING from SCALAR to SCALAR do CODE
    ["assertEqual",     { private _arr = []; for "_i" from 0 to 5 step 2 do { _arr pushBack _i; }; _arr }, [0, 2, 4]]   // for STRING from SCALAR to SCALAR step SCALAR do CODE
]