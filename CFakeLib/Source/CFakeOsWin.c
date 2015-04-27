/*
 * Copyright 2015 Nokia
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/******************************************************************************
 * @File    CFakeOsWin.h
 * @Brief   It provide windows related interface for internal use in CFake.
 ******************************************************************************/
#include <windows.h>
#include <assert.h>
#include "CFakeOs.h"
#include "CFakeLog.h"
#include "CFakePlatform.h"
#include "CFakeHw.h"

/* INFO Format: 0xAA, 'old_protect', '0xAA' */
#define FAKE_OS_INFO_SIZE               (sizeof(DWORD) + 2)
#define FAKE_OS_INFO_GUARDER_BYTE       0xAA
#define FAKE_OS_INFO_HEADER(infoPtr)    ((TFakeU8 *)(infoPtr) + 1)

static void FakeOsWin_SaveDataInfo(DWORD old_protect, TFakeU8 * infoPtr)
{
    infoPtr[0] = FAKE_OS_INFO_GUARDER_BYTE;

    (void)memcpy(FAKE_OS_INFO_HEADER(infoPtr), &old_protect, sizeof(DWORD));

    infoPtr[FAKE_OS_INFO_SIZE - 1] = FAKE_OS_INFO_GUARDER_BYTE;
}

/************************* Method Definitions Start ***************************/

static void 
FakeOsWin_AllocMemoryProtect(SFakeConfigParam * configParamPtr,
                             TFakeU8 *          infoPtr)
{
    DWORD       old_protect;
    TFakeUInt   codeSize = gFakeHw.GetCorruptedCodeSize();

    if (!VirtualProtect(configParamPtr->funcAddr,
                        codeSize,
                        PAGE_WRITECOPY,
                        &old_protect)) {
        gFakePlatform.RaiseFatal("Fail to alloc for writing code!");
    }

    FakeOsWin_SaveDataInfo(old_protect, infoPtr);
}

static void
FakeOsWin_FreeMemoryProtect(SFakeConfigParam * configParamPtr,
                            TFakeU8          * infoPtr)
{
    DWORD old_protect;

    (void)memcpy(&old_protect, FAKE_OS_INFO_HEADER(infoPtr), sizeof(DWORD));

    if (!VirtualProtect(configParamPtr->funcAddr, 
                        gFakeHw.GetCorruptedCodeSize(),
                        old_protect,
                        &old_protect)) {
        gFakePlatform.RaiseFatal("Fail to free from writing code!");
    }
}

/************************* Method Definitions End *****************************/

/* Component Object Definition(Singleton Pattern) */
SFakeOs gFakeOs = {
    FakeOsWin_AllocMemoryProtect,
    FakeOsWin_FreeMemoryProtect,
    FAKE_OS_INFO_SIZE,
};

