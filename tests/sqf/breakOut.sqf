[
    ["assertException",
        [
            "Unknown scope errors",
            {
                private _arr = [];
                [] call {
                    _arr pushBack 1;
                    [] call {
                        _arr pushBack 2;
                        [] call {
                          _arr pushBack 3;
                          breakOut "someunknowngiberishstuffyscope";
                          _arr pushBack 3;
                        };
                      _arr pushBack 2;
                    };
                    _arr pushBack 1;
                };
                _arr
            }
        ],
        [1, 2, 3]
    ],
    ["assertEqual",
        [
            "Empty scope leaves current scope",
            {
                private _arr = [];
                [] call {
                    _arr pushBack 1;
                    [] call {
                        _arr pushBack 2;
                        [] call {
                          _arr pushBack 3;
                          breakOut "";
                          _arr pushBack 3;
                        };
                      _arr pushBack 2;
                    };
                    _arr pushBack 1;
                };
                _arr
            }
        ],
        [1, 2, 3, 2, 1]
    ],
    ["assertEqual",
        [
            "breakOut actually works",
            {
                private _arr = [];
                [] call {
                    _arr pushBack 1;
                    [] call {
                        _arr pushBack 2;
                        scopeName "test-scope";
                        [] call {
                          _arr pushBack 3;
                          breakOut "test-scope";
                          _arr pushBack 3;
                        };
                      _arr pushBack 2;
                    };
                    _arr pushBack 1;
                };
                _arr
            }
        ],
        [1, 2, 3, 1]
    ],
    ["assertEqual",
        [
            "breakOut works with value",
            {
                private _arr = [];
                [] call {
                    _arr pushBack 1;
                    _arr pushBack ([] call {
                        _arr pushBack 2;
                        scopeName "test-scope";
                        [] call {
                          _arr pushBack 3;
                          true breakOut "test-scope";
                          _arr pushBack 3;
                          false
                        };
                      _arr pushBack 2;
                      false
                    });
                    _arr pushBack 1;
                    false
                };
                _arr
            }
        ],
        [1, 2, 3, true, 1]
    ]
]