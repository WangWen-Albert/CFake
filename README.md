# CFake
A fast and simple solution to do unit testing for C/C++ program.

## Purpose

When using FAKE_ON to fake one func with anther mock in your
test case, the mock will be called instead of the faked func.
And the fake action could be cancel by FAKE_OFF or FAKE_RESET
at the end of the case. 

In another word, one func could be dynamically replaced as any
other stub during testing program is running.

You may add this library to an existing unit testing framework
to power your testing performance, e.g. [cmockery](https://code.google.com/p/cmockery/), CppUnit.

## API
```c
#define FAKE_ON(func, mock) ...  //Macro API: Fake one func with anther mock
#define FAKE_OFF(func)      ...  //Macro API: Release the faked func
#define FAKE_RESET()        ...  //Macro API: Reset fake database and release all faked func.
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

## Build the library

The following option is used for different system.
Use it like "Make x86win32" under CFakeLib/Source

* 'x86win32' &ndash; Make static library for windows 32 system on x86 hw architecture;
* 'x86win64' &ndash; Make static library for windows 64 system on x86 hw architecture;
* 'x86linux32' &ndash; Make static library for linux 32 system on x86 hw architecture;
* 'x86linux64' &ndash; Make static library for linux 32 system on x86 hw architecture;

## Run quick test case

Run quick test case for linux 32 system on x86 hw architecture
```bash
cd Test;
make clean;make x86linux32
```
