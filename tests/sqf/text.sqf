[   ["assertEqual",      { str composeText ["hello", " ", "world"] }, "hello world"],
    ["assertEqual",      { typeName composeText ["hello", " ", "world"] }, "TEXT"],
    ["assertEqual",      { str lineBreak }, toString [13, 10]], // \r\n
    ["assertEqual",      { typeName lineBreak }, "TEXT"],
    ["assertEqual",      { str parseText "hello world" }, "hello world"],
    ["assertEqual",      { typeName parseText "hello world" }, "TEXT"],
    ["assertEqual",      { str text "hello world" }, "hello world"],
    ["assertEqual",      { typeName text "hello world" }, "TEXT"]
]
