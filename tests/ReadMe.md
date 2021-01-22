# Test-Framework Documentation #

SQF-VM features a test-framework to ensure it is valid across multiple releases.
The test-framework depends heavily on SQF-VM own commands and can be used as a reference
implementation (or straight copied) for your own Test-Framework using SQF-VM.

Due to features like `assertException` expecting runtime-errors and Arma not being able to handle
theese, this test-framework can be deemed as incompatible to Arma.

## Executing Tests ##

To run theese tests, start SQF-VM with the following commands:

`-a -i tests/config.cpp -i tests/runTests.sqf`

## Creating Tests ##

Tests are simple `.sqf` files sitting somewhere in the folder, where the `runTests.sqf` file is located at.
`runTests.sqf` will parse ALL of those `.sqf` files and expects them to be test-cases.

A test-case file always has to return an array of test-cases (empty arrays are accepted too).

A single test-case has the following structure: `[METHOD, TEST, ARGUMENT]`

|  String  |             Example                           | Description                                                                                             |
|:--------:|-----------------------------------------------|---------------------------------------------------------------------------------------------------------|
|  METHOD  | `["assert", TEST, ARGUMENT]`                  | The test-framework allows multiple methods to be ran. The STRING given here determines the method used. |
|   TEST   | `[METHOD, { 1 + 1 }, ARGUMENT]`<br/>`[METHOD, ["testname", { 1 + 1 }], ARGUMENT]` | The test to be executed. If provided as array, the first argument will be the test-case name, the second the actual test case. If just code, test-case name is the `str` version of the code.         |
| ARGUMENT | `[METHOD, TEST, "something"]`                 | Some methods require an argument. This is where it gets placed.                                         |


## Test Methods ##

|     Method      |                Example                  | Description                                                                           |
|:---------------:|-----------------------------------------|---------------------------------------------------------------------------------------|
|      setup      | `["setup",  { [] call _this; }]`        | Changes the setup code to the provided.                                               |
|      assert     | `["assert",  { ... }]`                  | Executes the piece of code. Unless Exception is raised, case is considered as passed. |
|   assertTrue    | `["assertTrue",  { true }]`             | Checks if the value returned is equal to `true`                                       |
|   assertFalse   | `["assertFalse",  { false }]`           | Checks if the value returned is equal to `false`                                      |
|   assertEqual   | `["assertEqual",  { 5 }, 5]`            | Compares the value returned with the argument passed via `isEqualTo`                  |
|   assertIsNil   | `["assertIsNil",  { nil }]`             | Checks if the value returned `isNil`                                                  |
|    assertNil    | `["assertNil",  { nil }]`               | Alias of `assertIsNil`                                                                |
| assertException | `["assertException",  { throw "foo" }]` | Executes the piece of code. Will be passed if Exception is raised.                    |
| assertExcept    | `["assertExcept",  { throw "foo" }]`    | Alias of `assertException`.                                                           |
