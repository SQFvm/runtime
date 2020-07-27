// Contains parse tests that should not crash
[   ["assert",  { compile """unclosed string" }],
    ["assert", {compile "[1,2,3" }]
    ["assert", {compile "fa" }]
    ["assert", {compile "!" }]
    ["assert", {compile "+" }]
    ["assert", {compile "-" }]
    ["assert", {compile "" }]
    ["assert", {compile "(1" }]
    ["assert", {compile "(1" }]
]