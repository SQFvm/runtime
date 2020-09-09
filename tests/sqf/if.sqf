[   
    ["assert",      { for "_i" from 0 to 3 do { assert (_i == 0); if true exitWith {}; }; }],
    ["assert",      { private _i = -1; while { _i = _i + 1; _i < 3 } do { assert (_i == 0); if true exitWith {}; }; }]
]