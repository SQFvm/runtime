[   
    ["assertEqual",  { + 1 }, 1],
    ["assertEqual",  { - 1 }, -1],
    ["assertEqual",  { +1 + +1 }, 2],
    ["assertEqual",  { -1 - -1 }, 0],
    ["assertEqual",  { -1 * -1 }, 1],
    ["assertEqual",  { +1 * +1 }, 1],
    ["assertEqual",  { 2 ^ -1 }, 0.5],
    ["assertEqual",  { 1 / 0 }, 0],
    ["assertEqual",  { 1 / 1 }, 1],
    ["assertEqual",  { 0.001 / 0 }, 0],
    ["assertEqual",  { abs 1337.7 }, 1337.7],
    ["assertEqual",  { 0.9 atan2 0.7 }, 0.909753157944],
    ["assertEqual",  { round deg 0.7 }, 40],
    ["assertEqual",  { log 1337 }, 3.1261312961578369],
    ["assertEqual",  { sin 120 }, 0.5806111693382263],
    ["assertEqual",  { pi }, 3.1415927410125732],
    ["assertEqual",  { acos 0.6 },  0.9272952079772949],
    ["assertEqual",  { cos 0.6 },  0.8253356218338013],
    ["assertEqual",  { exp 2 }, 7.3890562057495117],
    ["assertEqual",  { rad 220 }, 3.8397243022918701],
    ["assertEqual",  { sqrt 220 }, 14.8323965072631836],
    ["assertEqual",  { tan 220 }, 0.0887461379170418],
    ["assertEqual",  { 5 min 20 }, 5],
    ["assertEqual",  { 5 max 20 }, 20],
    ["assertEqual",  { floor 133.7 }, 133],
    ["assertEqual",  { ceil 133.7 }, 134],
    ["assertEqual",  { asin 0.2 }, 0.2013579308986664],
    ["assertEqual",  { atan 220 }, 1.5662509202957153],
    ["assertEqual",  { ln 220 }, 5.3936276435852051],
    ["assertEqual",  { 1337 mod 220 }, 17],
    ["assertEqual",  { round 133.7 }, 134],
    ["assertEqual",  { [1,2,3] vectorDotProduct [3,2,1] }, 10],
    ["assertEqual",  { vectorMagnitude [1,2,3] }, 3.7416574954986572],
    ["assertEqual",  { vectorMagnitudeSqr [1,2,3] }, 14],
    ["assertEqual",  { [1,2,3] vectorDistanceSqr [3,2,1] }, 8],
    ["assertEqual",  { [1,2,3] vectorDistance [3,2,1] }, 2.8284270763397217],
    ["assertEqual",  { [1,2,3] vectorAdd [3,2,1] }, [4,4,4]],
    ["assertEqual",  { [1,2,3] vectorCos [3,2,1] }, 0.7142857313156128],
    ["assertEqual",  { [1,2,3] vectorDiff [3,2,1] },[-2,0,2]],
    ["assertEqual",  { [1,2,3] vectorCrossProduct [3,2,1] }, [-4,8,-4]],
    ["assertEqual",  { [1,2,3] vectorMultiply 3 }, [3,6,9]],
    ["assertEqual",  { vectorNormalized [3,2,1] }, [0.5345224738121033,0.2672612369060516,0.8017837405204773]],
    ["assertEqual",  { vectorNormalized [0,0,0] }, [0,0,0]],
    ["assertEqual",  { 5 toFixed 2 }, "5.00"],
    ["assertEqual",  { 5 toFixed 25 }, "5.00000000000000000000"],
    ["assertEqual",  { 5 toFixed -2 }, "5"],
    ["assertIsNil",  { toFixed 2; toFixed -1 }, "5.00"],
    ["assertIsNil",  { random 5; nil }, "5.00"],



    ["assertException",  { [1,2,3] vectorAdd [true,2,1] }],
    ["assertException",  { [1,2,3] vectorCos [true,2,1] }],
    ["assertException",  { [1,2,3] vectorDotProduct [true,2,1] }],
    ["assertException",  { [1,2,3] vectorCrossProduct [true,2,1] }],
    ["assertException",  { [1,2,3] vectorDistance [true,2,1] }],
    ["assertException",  { [1,2,3] vectorDistanceSqr [true,2,1] }],
    ["assertException",  { [true,2,3] vectorMultiply 0 }],
    ["assertException",  { [1,2,3] vectorDiff [true,2,1] }],
    ["assertException",  { vectorMagnitude [true,2,1] }],
    ["assertException",  { vectorMagnitudeSqr [true,2,1] }],
    ["assertException",  { vectorNormalized [true,2,1] }]











]
