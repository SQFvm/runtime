class nested_tests
{
    node = "0";
    class nested1
    {
        node = "1";
        class nested2
        {
            node = "2";
            class nested3
            {
                node = "3";
                class nested4
                {
                    node = "4";
                };
            };
        };
    };
};
class type_tests
{
    type_array[] = { 1, "test", { 1, 2, 3 } };
    type_string = "test";
    type_anytext = any fancy text should be accepted;
    type_anytext_array[] = { any, fancy, text, should, be, accepted };
    type_scalar = 1;
    class type_class {};
};