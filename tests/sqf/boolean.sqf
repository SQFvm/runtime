[   ["assertTrue",      { true }],                                                       // BOOL
    ["assertFalse",     { false }],                                                      // BOOL
    ["assertTrue",      { true == true }],                                               // BOOL op BOOL
    ["assertTrue",      { false == false }],                                             // BOOL op BOOL
    ["assertTrue",      { true isEqualTo true }],                                        // BOOL isEqualTo BOOL
    ["assertTrue",      { false isEqualTo false }],                                      // BOOL isEqualTo BOOL
    ["assertFalse",     { true isEqualTo false }],                                       // BOOL isEqualTo BOOL
    ["assertFalse",     { false isEqualTo true }],                                       // BOOL isEqualTo BOOL
    ["assertTrue",      { true && true }],                                               // BOOL && BOOL
    ["assertFalse",     { true && false }],                                              // BOOL && BOOL
    ["assertFalse",     { false && true }],                                              // BOOL && BOOL
    ["assertFalse",     { false && { true } }],                                          // BOOL && CODE
    ["assertTrue",      { false || true }],                                              // BOOL || BOOL
    ["assertTrue",      { false || { true } }],                                          // BOOL || CODE
    ["assertTrue",      { !false }],                                                     // BOOL || CODE
    ["assertFalse",     { !true  }],                                                     // BOOL || CODE
    ["assertTrue",      { not false }],                                                  // BOOL || CODE
    ["assertFalse",     { not true }],                                                   // BOOL || CODE
    ["assertFalse",     { false isEqualTo true }]                                        // BOOL isEqualTo BOOL
]
