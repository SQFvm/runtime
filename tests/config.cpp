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
class flat_tests {
    class A { key = 1; };
    class B { key = 2; };
    class C: A {};
    class D: C { key = 4; };
    class E: B { key = 5; };
};
class test_select_selects_addon
{
    class addon {};
};

class test_config_classes_only_returns_config_entries
{
    property = 0;
    class TestSub
    {
        subProperty = 0;
    }; 
};