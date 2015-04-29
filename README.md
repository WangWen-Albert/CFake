# CFake [![Build Status](https://travis-ci.org/WangWen-Albert/CFake.svg)](https://travis-ci.org/WangWen-Albert/CFake)

CFake is a library for providing a fast and simple solution to do unit testing for C/C++ program,
solving how to fake one func with another mock(i.e. stub func) during runtime.

It's not a new unit testing framework. However, you could add this library to any existing 
framework(e.g. [cmockery](https://code.google.com/p/cmockery/)) to enhance the testing capability.

## Purpose

Why do we need to fake one func with another mock(i.e. stub func)?
* Fully make the action of func to be expected, especially the func of external module;
* Fully check param of stub func(i.e. one type of output from the tested func);
* Fully control return value of stub func(i.e. one type of input to the tested func);
* Easily divide the input to equivalent collection;
* Easily make conditions for the exceptional testing case;

What's different of the faking technology from some other library?
* The faking action is done during runtime, instead of building time;
* The faking action could be done for func either in external module or in same module;
* The faking action could be cancel at the end of the case;
* The stub func may has any name and could be built with original func;
# The stub func could be different for a same func in different case, i.e. dynamic stub;

## Solution

Using a trampoline asm code to corrupt original func from head.
The corrupted code will jump to stub func if executed.
* Generate a trampoline asm code which jump to stub func if executed;
* Open writable limits for the code memory protection;
* Corrupt the orignal func with the trampoline asm code from head;
* restore limits for the code memory protection;

Background Knowledage
* C/C++ function call stack
* Assembly language of target chip architecture
* Virtual memory and flat mode

## Description

When using FAKE_ON to fake one func with anther mock in your test case,
the mock will be called instead of the faked func.
And the fake action could be cancel by FAKE_OFF or FAKE_RESET at the end of the case. 

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

* 'x86win32'   &ndash; Build library for windows 32 system on x86 hw architecture;
* 'x86win64'   &ndash; Build library for windows 64 system on x86 hw architecture;
* 'x86linux32' &ndash; Build library for linux 32 system on x86 hw architecture;
* 'x86linux64' &ndash; Build library for linux 64 system on x86 hw architecture;

## Run quick test case

Run quick test case for linux 32 system on x86 hw architecture
```bash
cd Test;
make clean;make x86linux32
```
