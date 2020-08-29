# Example in SQC
```js
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
function global(val) {
    if (val > 10)
        return true;
    else if (val > 12)
    {
        return val < 50 ? false : true;
    }
}

// Call a user method
f("sqc", "hello world");

// Call a SQF unary operator
diag_log("test");

// Call a SQF binary operator (`getValue`)
player.getVariable("TAG_SomeVar");

// Create array and assign it to `arr`
private arr = [1,2,3];
```

# Same Example compiled to SQF
```sqf
private _a = 1;
private _b = 2;
private _c = 3;
private _d = 4;
private _f = {
    scopename "___sqc_func";
    params ["_arga", "_argb"];
    diag_log format ["%1 - %2", _arga, _argb]
};
global = {
    scopename "___sqc_func";
    params ["_val"];
    if (_val > 10) then
    {
        true breakout "___sqc_func"
    }
    else
    {
        if (_val > 12) then {
            if (_val < 50) then { false }
            else { true } breakout "___sqc_func"
        }
    }
};
["sqc", "hello world"] call _f;
diag_log "test";
player getvariable "TAG_SomeVar";
private _arr = [1, 2, 3]
```