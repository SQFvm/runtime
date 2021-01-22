[   ["assertEqual",      { [[2], [2]] matrixMultiply [[3]] }, [[6], [6]]],
    ["assertEqual",      { [[-1,0,0], [0,-1,0]]  matrixMultiply [[1,2], [3,1], [2,3]] }, [[-1,-2], [-3,-1]]],
    ["assertEqual",      { [[-1,0,0], [0,-1,0], [0,0,-1]] matrixMultiply [1, 2, 3] }, []],
    ["assertEqual",      { [[-1,0,0], [0,-1,0], [0,0,-1]] matrixMultiply [[1, 2, 3]] }, []],
    ["assertEqual",      { [[-1,0,0], [0,-1,0], [0,0,-1]] matrixMultiply [[1], [2], [3]] }, [[-1], [-2], [-3]]],
    ["assertEqual",      { matrixTranspose [[1,2,3]] }, [[1], [2], [3]]],
    ["assertEqual",      { matrixTranspose [[1], [2], [3]] }, [[1,2,3]]],
    ["assertEqual",      { matrixTranspose [[1,2,3], [3,1,2], [2,3,1]] }, [[1,3,2], [2,1,3], [3,2,1]]]
]
