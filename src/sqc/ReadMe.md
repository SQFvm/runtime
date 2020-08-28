# Example
```
    // Assign local values
    let a be 1;
    let b = 2;
    private c be 3;
    private d = 4;

    // Assign local function
    // note that `let f be` is just saying that the
    // value produced should be assigned to the private variable f
    let f be function(arga, argb) {
        diag_log(format("%1 - %2", arga, argb));
    };

    // Assign global function
    function global(arga) {
        diag_log(arga);
    }

    // Call a user method
    test("sqc", "hello world");

    // Call a SQF unary operator
    diag_log("test");

    // Call a SQF binary operator (`getValue`)
    player.getValue("somevalue");
```