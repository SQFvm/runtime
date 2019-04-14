[
    ["assertEqual",     { {_x == 0} count [] }, 0],
    ["assertEqual",     { {_x == 0} count [1,2,3] }, 0],
    ["assertEqual",     { {_x == 0} count [0] }, 1],
    ["assertEqual",     { {_x == 0} count [0,0] }, 1],
    ["assertException", { {_x == ""} count [0] }],
    ["assertException", { {} count [0] }],
    ["assertException", { {""} count [0] }]
]