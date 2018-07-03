## Response Schema
    {
        "mode": "message",
        "data": "string"
    }
* **mode** - `callstack` - always set, tells the receiving end that this is a callstack-object in the `data` field
* **data** - `STRING` - always set, contains the actual message comming from the server.

This response will be send whenever some error is happening on the server.