# Example in SQC
```js
// Declare file-header; Emits params at file start.
params(a, string b, scalar c = 15);

// Assign local values
let a be 1;
let b = 2;
private c be 3;
private d = 4;

// Assign local function
// note that `let f be` is just saying that the
// value produced should be assigned to the private variable f
let f = function(arga, argb) {
    diag_log(format("%1 - %2", arga, argb));
};

// Assign global function;
// Note that this is the same syntax for the args as with `params` at the start
// Also note that this is not being marked as final and can be overriden any time
function global(scalar val = 0) {
    if (val > 10)
        return true;
    else if (val > 12) {
        return val < 50 ? false : true;
    }
}

// Assign a FINAL (that is: `fnc_myFunc = compileFinal "...") function
final function fnc_myFunc(string msg) {
    diag_log(msg);
}

// Call a user method
f("sqc", "hello world");

// Call a SQF unary operator
diag_log("test");

// Call a SQF binary operator (`getValue`)
player.getVariable("TAG_SomeVar");

// Create array and assign it to `arr`
private arr = [1,2,3];

// Access array elements (can be chained :D)
if (arr[0] == 1) {
    // Set array elements 
    arr[1] = [1,2,3];
}

// Chained array access
if (arr[0][arr[3]] == 3) {
    diag_log("Chained Array Access Operators are cool");
    // and also assignment
    arr[0][arr[3]] = 5;
}

// Switcharoo
switch (5)
{
    case 1: diag_log("single instruction, marking the end of case.");
    case 2:
    case 3: { diag_log("codeblock with brackets"); }
    default: diag_log("also simple to do");
}

// Formatable string expressions
diag_log($"test {"string"} to test {1} or {[1,2,3].select(1)} features of {""""}formatable"" strings! {{ empty btw. gets {} }}");

// select range from array > 2
arr.select(function(it: "_x") { return it > 2; });
```

# Same Example compiled to SQF
```sqf
params ["_a", ["_b", nil, "], ["_c", 15, 0]];
scopename "___sqc_func";
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
    arams [[scopename "___sqc_func", "_val", [0]]];
    if (_val > 10) then {
        true breakout "___sqc_func"
    } else {
        if (_val > 12) then {
            if (_val < 50) then { false }
            else { true } breakout "___sqc_func"
        }
    }
};
fnc_myFunc = compilefinal "{ scopename ""___sqc_func""; params [[""_msg"", nil, [""""]]]; diag_log _msg }";
["sqc", "hello world"] call _f;
diag_log "test";
player getvariable "TAG_SomeVar";
private _arr = [1, 2, 3];
if (_arr select 0 == 1) then {
    _arr set [1, [1, 2, 3]]
};
if (_arr select 0 select (_arr select 3) == 3) then {
    diag_log "Chained Array Access Operators are cool";
    _arr select 0 set [_arr select 3, 5]
};
switch 5 do {
    case 1 : { diag_log "single instruction, marking the end of case." };
    case 2;
    case 3 : { diag_log "codeblock with brackets" };
    default { diag_log "also simple to do" }
};
diag_log format ["test %1 to test %2 or %3 features of %4formatable"" strings! { empty btw. gets %5 }", "string", 1, [1, 2, 3] select 1, """", nil];
_arr select { scopename "___sqc_func"; _x > 2 }
```