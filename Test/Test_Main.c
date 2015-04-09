#include "Test_CFakeLib.h"
#include "Test_GameOfLife.h"

static const UnitTest test_cases[] = {
    UT_CASES_GROUP_TEST_C_FAKE_LIB
    UT_CASES_GROUP_TEST_GAME_OF_LIFE
};

int main(int argc, char * argv[])
{
    return run_tests(test_cases);
}

