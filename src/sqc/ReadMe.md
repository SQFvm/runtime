This ReadMe is a Work-In-Progress documentation of the SQC language

# Table of Contents
- [Table of Contents](#table-of-contents)
- [Transpiling SQC to SQF](#transpiling-sqc-to-sqf)
  - [Using SQF-VM executable](#using-sqf-vm-executable)
  - [Using the SQF-VM language server with vscode](#using-the-sqf-vm-language-server-with-vscode)
- [Language Reference](#language-reference)
  - [Variable assignment](#variable-assignment)
  - [Calling Mechanisms](#calling-mechanisms)
    - [Calling binary operators](#calling-binary-operators)
    - [Calling unary operators](#calling-unary-operators)
    - [Calling nular operators](#calling-nular-operators)
    - [Calling SQF/SQC functions](#calling-sqfsqc-functions)
    - [Calling Object-functions](#calling-object-functions)
  - [Functions](#functions)
    - [Function Args](#function-args)
  - [File Header](#file-header)
  - [Formatable Strings](#formatable-strings)
  - [Objects](#objects)
    - [Example](#example)
  - [Operators](#operators)
    - [Math](#math)
    - [Logic](#logic)
    - [Other](#other)
  - [Control Structures](#control-structures)
    - [If](#if)
    - [Switch](#switch)
    - [For Step](#for-step)
    - [For Each](#for-each)
    - [While](#while)
    - [Do While](#do-while)
    - [Try Catch](#try-catch)
- [Examples](#examples)
- [SQC features mixed](#sqc-features-mixed)
    - [SQC](#sqc)
    - [Transpiled to SQF](#transpiled-to-sqf)
# Transpiling SQC to SQF
## Using SQF-VM executable
1. Download a `sqfvm_sqc` build (usually packed along with the normal sqfvm versions)
2. Open a command line where your `sqfvm_sqc` binary is located (on windows, you can use `SHIFT + Right Click`)
3. run `sqfvm_sqc` with `--compile-all PATH` (on windows, eg. `sqfvm_sqc.exe --compile-all C:/arma/addon/tag_pboname`)
4. Continue with addon packing like usual
## Using the SQF-VM language server with vscode
1. Download the [SQF-VM language server](https://marketplace.visualstudio.com/items?itemName=SQF-VM.sqf-vm-language-server) extension
2. Open your settings using `CTRL + ,`
3. Search in the new window for `Sqc Support` and tick the box
4. Restart vscode
5. Create or open a `.sqc` file
6. Modify it and save
7. Continue with addon packing like usual
# Language Reference
## Variable assignment
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
## Calling Mechanisms
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

### Calling binary operators
Given the binary operator `ANY in ARRAY` with an array `arr = [1, 2, 3]`

    // All fine here
    2.in(4, 5, 6)                       <-> 2 in [4, 5, 6]
    2.in(arr)                           <-> player in arr

    // Important! Passing single in args will not create arrays
    2.in(5)                             <-> 2 in 5

    // To fix, do the following:
    2.in([5])                           <-> 2 in [5]
### Calling unary operators
Given the unary operator `diag_log ANY`

    // All fine here
    diag_log("foo")                     <-> diag_log "foo"

    // Important! Passing multiple in args will create an array
    diag_log(1, 2, 3)                   <-> diag_log [1, 2, 3]

### Calling nular operators
Given the nular operator `player` with the unary operator `vehicle OBJECT`

    // Nular operators behave like variables
    private playerVehicle = vehicle(player);    <-> private _playerVehicle = vehicle player;

    // Important! Be careful not to locally override nular operators:
    private player = "...".createVehicle(...)
    private playerVehicle = vehicle(player);    <-> private playerVehicle = vehicle _player;

### Calling SQF/SQC functions
Given the function `func1` with a single input parameter and `func3` with three input parameters

    // Calling them utilizes the same syntax as with unary operators
    func1("foo");                               <-> ["foo"] call func1
    func3(2, "foo", "bar")                      <-> [2, "foo", "bar"] call func3

### Calling Object-functions
Given the object `obj = { func: function(){...}, nested: { otherFunc: function() {...} } }`

    // Calling object functions resembles binary calling syntax
    // Notice the fact, that the obj is passed inside of itself
    // which is to supply the `this` keyword of SQC
    obj.func()                                  <-> [obj] call (obj get "func") 
    obj.nestedObj.otherFunc()                   <-> [obj get "nestedObj"] call (obj get "nestedObj" get "otherFunc")
## Functions
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
### Function Args
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
## File Header
SQC Files may start with a so called "params" directive. This is so, that CfgFunctions may be used to initialize theese methods.
It lends itself the comma separated list of `<ARGS>` known from Functions and looks like this:

```js
    params(<ARGS>);
```
## Formatable Strings
SQC features a formatable string which got pretty much stolen from C#.

Translation Example:

```sqf
    $"This is a formatable string. The player position is {{ {position(player)} }}. His Vehicle is {vehicle(player)}"
    <->
    format["This is a formatable string. The player position is { %1 }. His Vehicle is %2", position player, vehicle player]
```
## Objects
In SQC you can create objects, thanks to SQF hashmaps.
Inside of object methods, you can use the `this` keyword to refer to the owning object instance.
### Example
The object syntax is contained of a list of keys (`<name>`) and values of any type received using any meaning (`<value>`)

    private logger = {
        log: function(data) {
            diag_log($"Logger{this.instanceCount}: {data}");
        },
        loggerName: "myLogger",
        instanceCount: getLoggerInstanceCount()
    }

## Operators
### Math
|  Name  |    SQC    |    SQF    |
|--------|-----------|-----------|
|Plus    |`ANY + ANY`|`ANY + ANY`|
|Minus   |`ANY - ANY`|`ANY - ANY`|
|Multiply|`ANY * ANY`|`ANY * ANY`|
|Devide  |`ANY / ANY`|`ANY / ANY`|
|Modulo  |`ANY % ANY`|`ANY % ANY`|
### Logic
|           Name        |      SQC      |         SQF          |
|-----------------------|---------------|----------------------|
|And                    |`ANY && ANY`   |`ANY && ANY`          |
|Or                     |`ANY \|\| ANY` |`ANY \|\| ANY`        |
|Greater than           |`ANY > ANY`    |`ANY > ANY`           |
|Greater than or equal  |`ANY >= ANY`   |`ANY >= ANY`          |
|Less than              |`ANY < ANY`    |`ANY < ANY`           |
|Less than or equal     |`ANY <= ANY`   |`ANY <= ANY`          |
|equals                 |`ANY == ANY`   |`ANY == ANY`          |
|equals exact           |`ANY === ANY`  |`ANY isEqualTo ANY`   |
|not equals             |`ANY != ANY`   |`ANY != ANY`          |
|not equals exact       |`ANY !== ANY`  |`!(ANY isEqualTo ANY)`|
|not                    |`!ANY`         |`!ANY`                |

### Other
|     Name        |        SQC                  |                    SQF                       |
|-----------------|-----------------------------|----------------------------------------------|
|Pre increment    |`func(++VARIABLE)`           |`VARIABLE = VARIABLE + 1; VARIABLE call func;`|
|Post increment   |`func(VARIABLE++)`           |`VARIABLE call func; VARIABLE = VARIABLE + 1;`|
|Pre decrement    |`func(--VARIABLE)`           |`VARIABLE = VARIABLE - 1; VARIABLE call func;`|
|Post decrement   |`func(VARIABLE--)`           |`VARIABLE call func; VARIABLE = VARIABLE - 1;`|
|Array access     |`ARRAY[VALUE]`               |`ARRAY select VALUE`                          |
|Object access    |`OBJECT.IDENTIFIER`          |`OBJECT get "IDENTIFIER"`                     |
|Ternary operator |`CONDITION ? VALUE1 : VALUE2`|`if CONDITION then { VALUE1 } else { VALUE2 }`|

## Control Structures
### If

    if (BOOL1)   func();                                     <-> if BOOL1 then { [] call funcA; }
    if (BOOL1) { funcA(); }                                  <-> if BOOL1 then { [] call funcA; }
    if (BOOL1)   funcA();   else   funcB();                  <-> if BOOL1 then { [] call funcA; } else { [] call funcB; }
    if (BOOL1) { funcA(); } else { funcB(); }                <-> if BOOL1 then { [] call funcA; } else { [] call funcB; }

    // Chaining is also possible thanks to the single-statement acceptance
    if (BOOL1) { funcA(); } else if (BOOL2) { funcB(); }     <-> if BOOL1 then { [] call funcA; } else { if BOOL2 then { [] call funcA; } }
### Switch
**SQC:**

    switch (5)
    {
        case 1: diag_log("single instruction, marking the end of case.");

        case 2: // fallthrough
        case 3: {
            diag_log("codeblock with brackets");
        }

        default: diag_log("default 'fallback'");
    }
**SQF:**

    switch 5 do
    {
        case 1: { diag_log "single instruction, marking the end of case."; };

        case 2; // fallthrough
        case 3: {
            diag_log "codeblock with brackets" ;
        };

        default { diag_log "default 'fallback'"; };
    }

### For Step
**SQC:**

    for i from 0 to 100 {
        diag_log(i);
    }


    for i from 0 to 100
        diag_log(i);


    for i from 0 to 100 step 2 {
        diag_log(i);
    }
**SQF:**

    for "_i" from 0 to 100 do {
        diag_log(i);
    }


    for "_i" from 0 to 100 do {
        diag_log(i);
    }


    for "_i" from 0 to 100 step 2 do {
        diag_log(i);
    }

### For Each
**SQC:**

    for (it : ARRAY) {
        diag_log(it);
    }


    for (it : ARRAY)
        diag_log(it);
**SQF:**

    {
        diag_log(_x);
    } foreach ARRAY;


    {
        diag_log(_x);
    } foreach ARRAY;

### While
**SQC:**

    while (BOOL) {
        diag_log(position(player));
    }


    while (BOOL)
        diag_log(position(player));
**SQF:**

    while { BOOL } do {
        diag_log position player;
    }


    while { BOOL } do {
        diag_log position player;
    }
### Do While
**SQC:**

    do {
        diag_log(position(player));
    } while (BOOL);


    do diag_log(position(player)); while (BOOL);
**SQF:**

    diag_log position player;
    while { BOOL } do {
        diag_log position player;
    }


    diag_log position player;
    while { BOOL } do { diag_log position player; }

### Try Catch
**SQC**

    try {
        throw VALUE;
    }
    catch (exception) {
        diag_log(exception);
    }


    try throw VALUE;
    catch (exception) diag_log(exception);
**SQF**

    try {
        throw VALUE;
    }
    catch {
        diag_log _exception;
    }


    try { throw VALUE; }
    catch { diag_log _exception; }

# Examples
# SQC features mixed
### SQC
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

### Transpiled to SQF
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
