#include "Test_GameOfLife.h"
#include "CFake.h"

static void DemoX(void)
{

}

static void Stub_DemoX(void)
{

}


void Test_Fake(void **state)
{
    FAKE_ON(DemoX,   Stub_DemoX);
    FAKE_OFF(DemoX);
}

