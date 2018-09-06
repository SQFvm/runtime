## Request Schema
    {
        "mode": "load-sqf",
        "data": { "path": "string", "name": "string" }
    }
- **mode** - `load-sqf` - always required, tells the debugger that this is a load-sqf request.
- **data** - `OBJECT` - always required, object containing the field `path` being the path to the file to parse local to the VM and the field `name` containing file name.


No response will be send.