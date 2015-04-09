#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include "CFakeOs.h"
#include "CFakeLog.h"

void
FakeOsLinux_AllocMemoryProtect(SFakeConfigParam * configParamPtr,
                               TFakeU8 *          msgPtr)
{
    gFakeLog.Debug("FakeOsLinux_AllocMemoryProtect");
}

void
FakeOsLinux_FreeMemoryProtect(SFakeConfigParam * configParamPtr,
                              TFakeU8 *          msgPtr)
{
    gFakeLog.Debug("FakeOsLinux_FreeMemoryProtect");
}


SFakeOs gFakeOs = {
    FakeOsLinux_AllocMemoryProtect,
    FakeOsLinux_FreeMemoryProtect,
    100,
};


