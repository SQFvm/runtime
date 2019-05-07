[   ["assertEqual",     { ".,a,.b.,c,.d.,e,.f.," splitString ",." }, ["a","b","c","d", "e","f"]],
    ["assertEqual",     { "abc.,.,.,.,.,.def," splitString ",." }, ["abc","def"]],
    ["assertEqual",     { "abcdef," splitString ",." }, ["abcdef"]],
    ["assertEqual",     { ",." splitString ",." }, []],
    ["assertEqual",     { "" splitString ",." }, []],
    ["assertEqual",     { "abcdef" splitString "" }, ["a","b","c","d","e","f"]]
]