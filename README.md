# CFake
A fast and simple solution to do unit testing

## Purpose

When using FAKE_ON to fake one func with anther mock in your
test case, the mock will be called instead of the faked func.
And the fake action could be cancel at the end of the case. 
In another word, one func could be dynamically replaced as any
other stub during testing program is running.

You may add this library to an existing unit testing framework
to power your testing performance, e.g. [cmockery](https://code.google.com/p/cmockery/), CppUnit.

## API
```c
FAKE_ON(func, mock) //Macro API: Fake one func with anther mock
FAKE_OFF(func)      //Macro API: Release the faked func
FAKE_RESET()        //Macro API: Reset fake database and release all faked func.
```

## Examples:

```c
static SLife * Stub_LifeCopy(SLife * life)
{
    /* check in-param */
    /* return value with UT framework cmockery */
}

static void Stub_AbortLife(char * format, ...)
{
    /* check in-param */
}

static void Stub_LifeUpdate(SLife * newLife, SLife * oldLife)
{
    /* check in-param */
    /* return value with UT framework cmockery */
}

static void inline Stub_LifeFree(SLife * life)
{
    /* check in-param */
}

void Test_LifeNext_NotEnoughMemoryToCopyOldLife(void **state)
{
    FAKE_ON(LifeCopy, Stub_LifeCopy);
    FAKE_ON(AbortLife, Stub_AbortLife);
    
    // test the case with UT framework cmockery

    FAKE_OFF(LifeCopy);
    FAKE_OFF(AbortLife);
}

void Test_LifeNext_Success(void **state)
{
    FAKE_ON(LifeCopy,   Stub_LifeCopy);
    FAKE_ON(LifeUpdate, Stub_LifeUpdate);
    FAKE_ON(LifeFree,   Stub_LifeFree);

    // test the case with UT framework cmockery

    FAKE_RESET();
}
```