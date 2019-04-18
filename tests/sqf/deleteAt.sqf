[   ["assertEqual",     ["Returns deleted value", { private _arr = [1,2,3]; _arr deleteAt 1 } ], 2],                // ARRAY deleteAt SCALAR: Returns deleted value
    ["assertIsNil",     ["Positive Out Of Bounds returns nil", { private _arr = [1,2,3]; _arr deleteAt 10 } ] ],    // ARRAY deleteAt SCALAR: Positive Out Of Bounds returns nil
    ["assertIsNil",     ["Negative Out Of Bounds returns nil", { private _arr = [1,2,3]; _arr deleteAt -1 } ] ]     // ARRAY deleteAt SCALAR: Negative Out Of Bounds returns nil
]