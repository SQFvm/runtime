[   
    ["assertEqual",  { +1++1 }, 2],
    ["assertEqual",  { -1--1 }, 0],
    ["assertEqual",  { -1*-1 }, 1],
    ["assertEqual",  { +1*+1 }, 1],
    ["assertEqual",  { 2^-1 }, 0.5],
    ["assertException",  { 1/0 }, 1]
]
