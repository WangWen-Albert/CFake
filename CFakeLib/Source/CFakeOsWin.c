#include <windows.h>
#include <assert.h>
#include "CFakeOs.h"
#include "CFakeLog.h"
#include "CFakePlatform.h"
#include "CFakeHw.h"

/* Saving info: 0xAA, 'old_protect', '0xAA' */
#define FAKE_OS_MSG_SIZE                (sizeof(DWORD) + 2)
#define FAKE_OS_MSG_GUARDER_BYTE        0xAA
#define FAKE_OS_MSG_HEADER(msgPtr)      ((TFakeU8 *)(msgPtr) + 1)

static void FakeOsWin_SaveDataInfo(DWORD old_protect, TFakeU8 * msgPtr)
{
    msgPtr[0] = FAKE_OS_MSG_GUARDER_BYTE;

    (void)memcpy(FAKE_OS_MSG_HEADER(msgPtr), &old_protect, sizeof(DWORD));

    msgPtr[FAKE_OS_MSG_SIZE - 1] = FAKE_OS_MSG_GUARDER_BYTE;
}

static void 
FakeOsWin_AllocMemoryProtect(SFakeConfigParam * configParamPtr,
                             TFakeU8 *          msgPtr)
{
    DWORD       old_protect;
    TFakeUInt   codeSize = gFakeHw.GetCodeSize();

    gFakeLog.Debug("Alloc VirtualProtect for changing %d bytes code", codeSize);

    if (!VirtualProtect(configParamPtr->funcAddr,
                        codeSize,
                        PAGE_WRITECOPY,
                        &old_protect)) {
        gFakePlatform.RaiseFatal("Fail to alloc for writing code!");
    }

    gFakeLog.Debug("0x%X", old_protect);

    FakeOsWin_SaveDataInfo(old_protect, msgPtr);
}

static void
FakeOsWin_FreeMemoryProtect(SFakeConfigParam * configParamPtr,
                            TFakeU8          * msgPtr)
{
    DWORD old_protect;

    gFakeLog.Debug("Free VirtualProtect for changing code");

    (void)memcpy(&old_protect, FAKE_OS_MSG_HEADER(msgPtr), sizeof(DWORD));

    gFakeLog.Debug("0x%X", old_protect);

    if (!VirtualProtect(configParamPtr->funcAddr, 
                        gFakeHw.GetCodeSize(),
                        old_protect,
                        &old_protect)) {
        gFakePlatform.RaiseFatal("Fail to free from writing code!");
    }
}

SFakeOs gFakeOs = {
    FakeOsWin_AllocMemoryProtect,
    FakeOsWin_FreeMemoryProtect,
    FAKE_OS_MSG_SIZE,
};

