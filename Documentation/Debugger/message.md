## Response Schema
    {
        "mode": "message",
        "data": "string"
    }
* **mode** - `message` - always set, tells the receiving end that this is a message-string in the `data` field
* **data** - `STRING` - always set, contains the actual message comming from the server.

This response will be send whenever some output is generated on the server.