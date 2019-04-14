[
    ["assertTrue",      { ([1, 2, 3] select 1) isEqualTo 2 }],
    ["assertTrue",      { isNil { [1, 2, 3] select 3 } }],
    ["assertTrue",      { isNil { [] select 0 } }],
    ["assertException", { [] select 1; }]
]