// Contains parse tests that should not crash
[   ["assert",       { compile """unclosed string" }],
    ["assertExcept", { compile "[1,2,3" }],
    ["assert",       { compile "fa" }],
    ["assertExcept", { compile "!" }],
    ["assertExcept", { compile "+" }],
    ["assertExcept", { compile "-" }],
    ["assert",       { compile "" }],
    ["assertExcept", { compile "(1" }],
    ["assertExcept", { compile "(1" }],
    ["assertExcept", { compile "{" }],
    ["assert",       { compile "1" }],
    ["assert",       { compile ".1" }],
    ["assertExcept", { compile "." }],
    ["assert",       { compile "0x1" }],
    ["assert",       { compile "$1" }],
    ["assert",       { compile "private _test = 1+1, _test == 2" }]
]