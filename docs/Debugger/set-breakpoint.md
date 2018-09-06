## Request Schema
    {
      "mode": "set-breakpoint",
      "data": {
        "line": 0,
        "file": "string"
      }
    }
- **mode** - `set-breakpoint` - always required, tells the debugger that this is a set-breakpoint request.
- **data** - `OBJECT` - always required, object containing the field `line` for the line to break on, and field `file` for file specific stuff.


No response will be send.