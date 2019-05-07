[   ["assertEqual",     { "abc,def,g" splitString "," }, [".,a","b.,c","d.,e","f.,"]],
    ["assertEqual",     { "abc,def,g," splitString "," }, ["abc.","def,"]],
    ["assertEqual",     { "abc,,,,def,g,,," splitString "," }, ["abcdef,"]],
    ["assertEqual",     { "abc,d,,e,f,g" splitString "," }, []],
    ["assertEqual",     { "abc,d,,ef,g" splitString "," }, []],
    ["assertEqual",     { "abc,def,g" splitString "," }, ["a","b","c","d","e","f"]]
]