## Request Schema
    {
        "mode": "get-status",
        "data":  null
    }
- **mode** - `get-status` - always required, tells the debugger that this is a get-status request.
- **data** - `null` - always required, null as the get-status request does not has any further data requirements.

## Response Schema
    {
        "mode": "status",
        "data": "string"
    }
* **mode** - `callstack` - always set, tells the receiving end that this is a callstack-object in the `data` field
* **data** - `STRING` - always set, either `HALT`, `RUNNING`, `DONE` or `NA`.


#### HALT
Halt means that the server currently is in breakmode.

#### RUNNING
Server currently executes code.

#### DONE
Same as `HALT` but with different meaning.
Always set when the server ran out of code it may run.

#### NA
Set when the server could not determine the exact status of the current execution.