## Request Schema
    {
        "mode": "get-variables",
        "data": [
            {
                "name":  "varname",
                "scope": "global"
            },
            {
                "name":  "_varname",
                "scope": -1
            }
        ]
    }
- **mode** - `get-variables` - always required, tells the debugger that this is a get-variables request.
- **data** - `ARRAY` - always required, contains all variables you want to receive as VariableRequest objects.

## Response Schema
    {
        "mode": "variables",
        "data":  [
    		{ "name": "string", "value": "string" }
    	]
    }
* **mode** - `variables` - always set, tells the receiving end that this is a variables-response in the `data` field
* **data** - `ARRAY` - always set, simple key-value array containing the private variables (those starting with underscore `_`) as object with the fields `name` for the key and `value` for the value.

#### The VariableRequest Object
    {
        "name":  "varname",
        "scope": "string"
    }
    /* OR */
    {
        "name":  "_varname",
        "scope": -1
    }
* **name** - `STRING` - The name of the variable requested.
* **scope** - `STRING` - The scope this is requesting from. Needs to be one of the following enum values: [missionNamespace, uiNamespace, profileNamespace, parsingNamespace]
* **scope** - `NUMBER` - Stack Location. Telling the debugger where to start the variable search. 0 is the most inner scope, -N is N levels outter.