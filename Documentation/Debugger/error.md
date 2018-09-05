## Response Schema
    {
        "mode": "error",
        "data": "string"
    }
* **mode** - `error` - always set, tells the receiving end that this is a error-string in the `data` field
* **data** - `STRING` - always set, contains the actual error comming from the server.

This response will be send whenever some error is generated on the server.