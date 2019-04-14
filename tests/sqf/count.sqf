[   ["assertEqual",     { count [] }, 0],                       // count ARRAY
    ["assertEqual",     { count [1, 2, 3] }, 3],
    ["assertEqual",     { count "123" }, 3],                    // count STRING
    ["assertEqual",     { { _x == 0 } count [1, 2, 3] }, 0],    // CODE count ARRAY
    ["assertEqual",     { { _x == 1 } count [1, 2, 3] }, 1],
    ["assertEqual",     { { _x == 1 } count [] }, 0],
    ["assertEqual",     { { nil } count [1, 2, 3] }, 0],
    ["assertEqual",     { { _x == 0 } count [] }, 0],
    ["assertEqual",     { { _x == 0 } count [1, 2, 3] }, 0],
    ["assertEqual",     { { _x == 0 } count [0] }, 1],
    ["assertEqual",     { { _x == 0 } count [0, 0] }, 2],
    ["assertException", { { _x == "" } count [0] }],
    ["assertException", { {} count [0] }],
    ["assertException", { {""} count [0] }]
]