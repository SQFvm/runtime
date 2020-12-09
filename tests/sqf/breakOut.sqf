[
    ["assertException",
        [
            "Unknown scope errors",
            {
                private _arr = [];
                [] call {
                    _arr push_back 1;
                    [] call {
                        _arr push_back 2;
                        [] call {
                          _arr push_back 3;
                          breakOut "someunknowngiberishstuffyscope";
                          _arr push_back 3;
                        };
                      _arr push_back 2;
                    };
                    _arr push_back 1;
                };
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
                    _arr push_back 1;
                    [] call {
                        _arr push_back 2;
                        [] call {
                          _arr push_back 3;
                          breakOut "";
                          _arr push_back 3;
                        };
                      _arr push_back 2;
                    };
                    _arr push_back 1;
                };
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
                    _arr push_back 1;
                    [] call {
                        _arr push_back 2;
                        scopeName "test-scope"
                        [] call {
                          _arr push_back 3;
                          breakOut "test-scope";
                          _arr push_back 3;
                        };
                      _arr push_back 2;
                    };
                    _arr push_back 1;
                };
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
                    _arr push_back 1;
                    _arr pushBack ([] call {
                        _arr push_back 2;
                        scopeName "test-scope"
                        [] call {
                          _arr push_back 3;
                          true breakOut "test-scope";
                          _arr push_back 3;
                          false
                        };
                      _arr push_back 2;
                      false
                    });
                    _arr push_back 1;
                    false
                };
            }
        ],
        [1, 2, 3, true, 1]
    ],
]