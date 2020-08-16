[   ["assertTrue",      { private _test = nil; isNil "_test"; }],
    ["assertFalse",     { private _test = 1; isNil "_test"; }],
    ["assertTrue",      { isNil { nil } }],
    ["assertFalse",     { isNil { 1 } }]
]