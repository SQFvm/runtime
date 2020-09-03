#define TEST(ARG1) ARG1 ARG1 ARG1
#define EMPTY(ARG1)
#define OTHER(ARG1) ARG1: TEST(ARG1)
TEST
TEST(something-1)
EMPTY
EMPTY(something-1)
OTHER
OTHER(something-1)