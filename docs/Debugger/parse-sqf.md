## Request Schema
    {
        "mode": "parse-sqf",
        "data": { "sqf": "string", "file": "string" }
    }
- **mode** - `parse-sqf` - always required, tells the debugger that this is a parse-sqf request.
- **data** - `OBJECT` - always required, object containing the field `sqf` being the code to parse and the field `file` containing file informations.


No response will be send.