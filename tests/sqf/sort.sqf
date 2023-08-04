[   ["assertEqual",      { private _arr = [1, 2, 3, 4, 5]; _arr sort false; _arr }, [5, 4, 3, 2, 1]],
    ["assertEqual",      { private _arr = [5, 4, 3, 2, 1]; _arr sort true; _arr }, [1, 2, 3, 4, 5]],
    ["assertException",  { private _arr = [[3], [3, 1], ["", 2], ["", ""], [1, 2], [1, 1]]; _arr sort false; _arr }],
    ["assertEqual",      { private _arr = [[3, 2], [3, 1], [2, 2], [2, 1], [1, 2], [1, 1]]; _arr sort false; _arr }, [[1, 1], [1, 2], [2, 1], [2, 2], [3, 1], [3, 2]]],
    ["assertEqual",      { private _arr = [[1, 1], [1, 2], [2, 1], [2, 2], [3, 1], [3, 2]]; _arr sort true; _arr }, [[3, 2], [3, 1], [2, 2], [2, 1], [1, 2], [1, 1]]],
    ["assertEqual",      { private _arr = ["tram","tara","pa","pa","ja"]; _arr sort false; _arr }, ["ja","pa","pa","tram","tara"],
    ["assertEqual",      { private _arr = ["ja","pa","pa","tram","tara"]; _arr sort true; _arr }, ["tram","tara","pa","pa","ja"]]],
    ["assertEqual",      { private _arr = [[0,0,3,"Head"],[0,0,4,"Body"],[0,0,1.00125,"LeftArm"],[0,0,1.56359,"RightArm"],[0,0,1.1933,"LeftLeg"],[6.17301,2.05767,1.80874,"RightLeg"],[0,0,1,"#structural"]]; _arr sort false; _arr }, [[6.17301,2.05767,1.80874,"RightLeg"],[0,0,4,"Body"],[0,0,3,"Head"],[0,0,1.56359,"RightArm"],[0,0,1.1933,"LeftLeg"],[0,0,1.00125,"LeftArm"],[0,0,1,"#structural"]]]

]