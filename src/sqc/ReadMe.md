This ReadMe is a Work-In-Progress documentation of the SQC language


# Variable assignment
A variable in SQC is assigned like this:

```js
    variable = <VALUE>;
    variable be <VALUE>;
```

To make a variable private, you just add the `let` or `private` keyword in front of it:

```js
    let variable be <VALUE>;
    private variable = <VALUE>;
```


# Calling a function or operator
In SQC, you have no direct syntax to "call" sqf like one would in SQF.
Instead, SQC attempts to automatically mangle the different types and functions for you.

For example, a call to `diag_log` in SQC, treats `diag_log` as method which makes the call look like this: `diag_log("fooo bar")`.
special attention should be given to operators, expecting arrays as their parameter though.
For theese, SQC automatically transforms parameter lists into arrays (`nearestObjects(player, ["Tank"],500)`) however, given that only one argument
gets passed to an operator expecting an array, the correct way to call that then is to pass the array then as a proper array (`private(["_someVar"])`)

With Binary operators (eg. `in`), SQC gives you a somewhat OOP way to access them, making `player setPosition [1,2,3]` to `player.setPosition(1,2,3)`.
Same rules apply here too. If you are expected to have an array on the right side with only one in-argument, you need to pass in an array.
Note that the left side for the binary operators always will be evaluated "as is".

User Functions are always treated as "normal" functions (`myFunc(arg0, arg1, arg2)`) and no special care has to be given to them.

Translation Examples:

```sqf
    diag_log("foo bar")                <-> diag_log "foo bar"
    diag_log(1, 2, 3)                  <-> diag_log [1, 2, 3]
    player.in(player, vehicle(player)) <-> player in [player, vehicle player]
    player.in(vehicle(player))         <-> player in vehicle player
    myFunc(arg0, arg1, arg2)           <-> [arg0, arg1, arg2] call myFunc
```

# Functions
In SQC, a function is made using the following syntax:
(to make a function final, a `final` prefix may be added)

```js
    // Returns CODE type
    function(<ARGS>) { }
    // Assigns the function to a variable named fncName
    function fncName(<ARGS>) { }
    // Stringifies the function, calls compileFinal and assigns the value to fncName
    final function fncName(<ARGS>) { }
```

`<ARGS>` are a comma separated list of "arguments" that the method shall receive.
Theese can be typed. They follow the following syntax:

```js
    variableName
    variableName = <VALUE>
    <TYPE> variableName
    <TYPE> variableName = <VALUE>
    variableName: "actualVariableName"
```

The mentioned `<TYPE>` here is one of the many, valid arma types (eg. string, array, scalar).

The last syntax example, `variableName: "actualVariableName"` is crafted for a special case.
Given that one may want to use operators that provide existing arguments (eg. `ARRAY select CODE`), SQC needs a way to allow the programmer to denote
existing variables in the function code. This is what that syntax does. It rewrites the `actualVariableName` onto the "virtual" `variableName`.

Full example: `arr.select(function(it: "_x") { return it > 2; });` gets `arr select { _x > 2 }`

# File Header
SQC Files may start with a so called "params" directive. This is so, that CfgFunctions may be used to initialize theese methods.
It lends itself the comma separated list of `<ARGS>` known from Functions and looks like this:

```js
    params(<ARGS>);
```


# Formatable Strings
SQC features a formatable string which got pretty much stolen from C#.

Translation Example:

```sqf
    $"This is a formatable string. The player position is {{ {position(player)} }}. His Vehicle is {vehicle(player)}"
    <->
    format["This is a formatable string. The player position is { %1 }. His Vehicle is %2", position player, vehicle player]
```
# Operators

## Math

## Logic

## Increment/Decrement

SQC has incremental and decremental pre- and postfix operators `++` and `--`.
Using theese, will increase/decrease a given variable by one (1).

Translation Example:
```sqf
    diag_log(x++); <-> diag_log x; x = x + 1;
    diag_log(++x); <-> x = x + 1; diag_log x;
    diag_log(x--); <-> diag_log x; x = x - 1;
    diag_log(--x); <-> x = x - 1; diag_log x;
```

# Array Index operators - assignment & receiving

# Control Structures

## If

## Switch

## For Step

## For Each

## While

## Do While

## Try Catch



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
