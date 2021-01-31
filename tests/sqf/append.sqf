[   ["assertEqual",     { private _arr = []; _arr append [1]; _arr; }, [1]],
    ["assertEqual",     { private _arr = [1]; _arr append []; _arr; }, [1]],
    ["assertEqual",     { private _arr = [1]; _arr append [2]; _arr; }, [1, 2]],
    ["assertEqual",     { private _arr = [1]; _arr append [2]; _arr append [3]; _arr; }, [1, 2, 3]],
    ["assertEqual",     { private _arr = [1, 2]; _arr append [3, 4]; _arr; }, [1, 2, 3, 4]]
]
