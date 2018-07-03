## Request Schema
    {
        "mode": "get-callstack",
        "data":  null
    }
- **mode** - `get-callstack` - always required, tells the debugger that this is a get-callstack request.
- **data** - `null` - always required, null as the get-callstack request does not has any further data requirements.

## Response Schema
    {
        "mode": "callstack",
        "data":  [
    		{
    			"lvl": 0,
    			"scopename": "string",
    			"namespace": "string",
				"options": { "line": 0, "column": 0, "file": 0, "available": false  }
    			"variables": [
    				{ "name": "string", "value", "string" }
    			]
    		}
    	]
    }
* **mode** - `callstack` - always set, tells the receiving end that this is a callstack-object in the `data` field
* **data** - `ARRAY` - always set, contains the actual callstack `scope` objects as they are present on the sqf-end.

#### The Scope Object
	{
		"lvl": 0,
		"scopename": "string",
		"namespace": "string",
		"variables": [
			{ "name": "string", "value", "string" }
		]
	}
* **lvl** - `NUMBER` - the level of the current scope object. 0 is the most-outter-scope.
* **scopename** - `STRING` - the name of the current scope object. May be an empty string if the `scopeName` command never was used or scope was created without a scope name.
* **namespace** - `STRING` - the namespace this scope lives in. Most of the time, this will be missionNamespace. May change with eg. `with NAMESPACE do {...}` operators.
* **options** - `OBEJCT` - Simple object containing informations about the current position that gets executed. Information may not be available, which is when `available` will be false.
* **variables** - `ARRAY` - Simple key-value array containing the private variables (those starting with underscore `_`) as object with the fields `name` for the key and `value` for the value.