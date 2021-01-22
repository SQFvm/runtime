[  ["assertEqual",     { 1 call { params ["_test"]; _test } }, 1],
   ["assertEqual",     { true call { params ["_test"]; _test } }, true],
   ["assertEqual",     { "" call { params ["_test"]; _test } }, ""],
   ["assertEqual",     { [1,2,3] params [["_array", [], [[]]], ["_inPlace", false, [false]]]; [_array, _inPlace] }, [[], false]]
]