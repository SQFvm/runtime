[   ["assertException", { throw "test"; }],
    ["assertEqual",     { private "_val"; try { throw true; } catch { _val = _exception; }; _val }, true],
    ["assertEqual",     { private "_val"; try { throw "string"; } catch { _val = _exception; }; _val }, "string"],
    ["assertEqual",     { private "_val"; try { throw 1; } catch { _val = _exception; }; _val }, 1],
    ["assertEqual",     { private "_val"; try { throw [1,2,3]; } catch { _val = _exception; }; _val }, [1,2,3]],
    ["assertEqual",     { private "_val"; try { throw {1+1}; } catch { _val = _exception; }; _val }, {1+1}],
    ["assertEqual",     { private _exception = 1; try { throw "string"; } catch { private _val = _exception; }; _exception }, 1]
]