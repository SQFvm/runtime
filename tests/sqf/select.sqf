[   ["assertEqual",     { [1, 2, 3] select 1 }, 2],                             // ARRAY select SCALAR
    ["assertEqual",     { [1, 2, 3] select 1.6 }, 3],                           // ARRAY select SCALAR
    ["assertEqual",     { [1, 2, 3] select 1.5 }, 3],                           // ARRAY select SCALAR
    ["assertEqual",     { [1, 2, 3] select 1.4 }, 2],                           // ARRAY select SCALAR
    ["assertIsNil",     { [1, 2, 3] select 3 }],                                // ARRAY select SCALAR
    ["assertIsNil",     { [] select 0 }],                                       // ARRAY select SCALAR
    ["assertException", { [] select 1 }],                                       // ARRAY select SCALAR
    ["assertException", { [] select -1 }],                                      // ARRAY select SCALAR
    ["assertEqual",     { [1, 2] select true }, 2],                             // ARRAY select BOOL
    ["assertEqual",     { [1, 2] select false }, 1],                            // ARRAY select BOOL
    ["assertIsNil",     { [1] select true }],                                   // ARRAY select BOOL
    ["assertIsNil",     { [] select false }],                                   // ARRAY select BOOL
    ["assertEqual",     { "12345" select [0] }, "12345"],                       // STRING select ARRAY
    ["assertEqual",     { "12345" select [1] }, "2345"],                        // STRING select ARRAY
    ["assertEqual",     { "12345" select [0.6, 3] }, "234"],                    // STRING select ARRAY
    ["assertEqual",     { "12345" select [0, 3] }, "123"],                      // STRING select ARRAY
    ["assertEqual",     { "12345" select [1, 2] }, "23"],                       // STRING select ARRAY
    ["assertEqual",     { "" select [1] }, ""],                                 // STRING select ARRAY
    ["assertEqual",     { "" select [0, 10] }, ""],                             // STRING select ARRAY
    ["assertException", { "" select [] }, ""],                                  // STRING select ARRAY
    ["assertException", { "" select [true] }, ""],                              // STRING select ARRAY
    ["assertEqual",     { "" select [-1] }, ""],                                // STRING select ARRAY
    ["assertEqual",     { "test" select [0, -1] }, ""],                         // STRING select ARRAY
    ["assertException", { "test" select [0, true] }, ""],                       // STRING select ARRAY
    ["assertEqual",     { """" select [0, 1] }, """"],                          // STRING select ARRAY
    ["assertEqual",     { [1, 2, 3, 4, 5] select [0] }, []],                    // ARRAY select ARRAY
    ["assertEqual",     { [1, 2, 3, 4, 5] select [1] }, []],                    // ARRAY select ARRAY
    ["assertEqual",     { [1, 2, 3, 4, 5] select [0.6, 3] }, [2, 3, 4]],        // ARRAY select ARRAY
    ["assertEqual",     { [1, 2, 3, 4, 5] select [0, 3] }, [1, 2, 3]],          // ARRAY select ARRAY
    ["assertEqual",     { [1, 2, 3, 4, 5] select [1, 2] }, [2, 3]],             // ARRAY select ARRAY
    ["assertEqual",     { [] select [0, 10] }, []],                             // ARRAY select ARRAY
    ["assertEqual",     { [] select [10, 10] }, []],                            // ARRAY select ARRAY
    ["assertException", { [] select [] }, []],                                  // ARRAY select ARRAY
    ["assertException", { [] select [true] }, []],                              // ARRAY select ARRAY
    ["assertEqual",     { [1, 2, 3] select [-1] }, []],                         // ARRAY select ARRAY
    ["assertException", { [1, 2, 3] select [0, true] }, []],                    // ARRAY select ARRAY
    ["assertEqual",     { [1, 2, 3] select [0, -1] }, []],                      // ARRAY select ARRAY
    ["assertEqual",     { [1, 2, 3, 4, 5] select { _x > 2 } }, [3, 4, 5]],      // ARRAY select CODE
    ["assertEqual",     { [1, 2, 3, 4, 5] select { true } }, [1, 2, 3, 4, 5]],  // ARRAY select CODE
    ["assertEqual",     { [1, 2, 3, 4, 5] select { false } }, []],              // ARRAY select CODE
    ["assertEqual",     { [1, 2, 3, 4, 5] select { nil } }, []],                // ARRAY select CODE
    ["assertException", { [1, 2, 3, 4, 5] select { 1 } }, []],                  // ARRAY select CODE
    ["assertException", { [1, 2, 3, 4, 5] select { "" } }, []],                 // ARRAY select CODE
    ["assertException", { [1, 2, 3, 4, 5] select { [] } }, []],                 // ARRAY select CODE
    ["assertException", { [1, 2, 3, 4, 5] select { {} } }, []],                 // ARRAY select CODE
    ["assertEqual",     { [] select { true } }, []],                            // ARRAY select CODE


    ["assertEqual",     { selectMax [0,1,2,3,4] }, 4],                            // selectMax ARRAY
    ["assertEqual",     { selectMin [0,1,2,3,4] }, 0],                            // selectMin ARRAY
    ["assertEqual",     { selectRandom [0,0,0,0,0] }, 0]                          // selectRandom ARRAY
]