[   ["assertEqual",     { private _arr = [0,1,2,3,4,5,6]; _arr deleteRange [0.6, 2.4]; _arr; }, [0,3,4,5,6]],
    ["assertEqual",     { private _arr = [0,1,2,3,4,5,6]; _arr deleteRange [2, 1];     _arr; }, [0,1,3,4,5,6]],
    ["assertEqual",     { private _arr = [0,1,2,3,4,5,6]; _arr deleteRange [1, 1];     _arr; }, [0,2,3,4,5,6]],
    ["assertEqual",     { private _arr = [0,1,2,3,4,5,6]; _arr deleteRange [-1, 1];    _arr; }, [0,1,2,3,4,5,6]],
    ["assertEqual",     { private _arr = [0,1,2,3,4,5,6]; _arr deleteRange [1, 10];    _arr; }, [0]]
]