[
    ["assertTrue",      { (count []) == 0 }],
    ["assertTrue",      { (count [1, 2, 3]) == 3 }],
    ["assertTrue",      { ({ _x == 0 } count [1, 2, 3]) == 0 }],
    ["assertTrue",      { ({ _x == 1 } count [1, 2, 3]) == 1 }],
    ["assertTrue",      { ({ _x == 1 } count []) == 0 }],
    ["assertTrue",      { (count "123") == 3 }],
    ["assertTrue",      { ({ nil } count [1, 2, 3]) == 0 }]
]