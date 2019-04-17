[   ["assertTrue", { true }],                                                       // BOOL
    ["assertFalse", { false }],                                                     // BOOL
    ["assertException", { true == true }],                                          // BOOL op BOOL
    ["assertTrue", { true isEqualTo true }],                                        // BOOL isEqualTo BOOL
    ["assertTrue", { false isEqualTo false }],                                      // BOOL isEqualTo BOOL
    ["assertFalse", { true isEqualTo false }],                                      // BOOL isEqualTo BOOL
    ["assertFalse", { false isEqualTo true }]                                       // BOOL isEqualTo BOOL
]
