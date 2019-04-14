[
    ["assertEqual",     { [1, 2, 3] select 1 }, 2],
    ["assertIsNil",     { [1, 2, 3] select 3 }],
    ["assertIsNil",     { [] select 0 }],
    ["assertException", { [] select 1 }]
]