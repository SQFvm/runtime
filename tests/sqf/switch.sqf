[
	[
        "assertEqual",
        [
            "switch cases from call",
            {
                private _arr = [];
                private _cases = {
                    case 1: {_arr pushBack "inside 1"; "a"}; 
                    _arr pushBack "past case 1";
                    case 2: {_arr pushBack "inside 2"; "b"};
                    _arr pushBack "past case 2";
                    case 3: {_arr pushBack "inside 3"; "c"};
                    _arr pushBack "past case 3";
                    default {_arr pushBack "inside default"; "default"};
                    _arr pushBack "past default";
                };
                private _res = switch 2 do {
                    [] call {
                        [] call _cases;
                        _arr pushBack "past cases inner"
                    };
                    _arr pushBack "past cases call outter";
                };
                _arr pushBack _res;
                _arr
            }
        ],
        ["past case 1","past cases inner","past cases call outter","inside 2","b"]
    ],
	[
        "assertEqual",
        {
            private _arr = [];
			switch (1) do
			{
			  private _case = case 1;
			  switch (2) do
			  {
				  case 2: { _arr pushBack "empty"; };
				  _case: { _arr pushBack "arg"; };
			  };
			  default { _arr pushBack "magic"; };
			};
            _arr
        },
        ["empty", "magic"]
    ],
    ["assert", { switch(2) do { case 1: {}; }; }]
]