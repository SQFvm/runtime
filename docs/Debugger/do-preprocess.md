## Request Schema
    {
        "mode": "do-preprocess",
        "data": { "path": "string", "content": "string" }
    }
* **mode** - `do-preprocess` - always set, tells the receiving end that this is a do-preprocess action in the `data` field
* **data** - `STRING` - always set, contains either the `path` to an existing file or a `content` field. If both are provided, path takes priority.
## Response Schema
    {
        "mode": "preprocess",
        "data": "string"
    }
* **mode** - `preprocess` - always set, tells the receiving end that this is a preprocess-string in the `data` field
* **data** - `STRING` - always set, contains the preprocessed data from the server.

Tells the server to preprocess a file or contents.