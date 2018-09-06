## Response Schema
    {
        "mode": "position",
        "data": { "line": 0, "col": 0, "file": "string" }
    }
* **mode** - `position` - always set, tells the receiving end that this is a position-object in the `data` field
* **data** - `STRING` - always set, contains the current line, column and file where the vm currently is at.

This response will be send whenever a breakpoint is hit or an error is raised.