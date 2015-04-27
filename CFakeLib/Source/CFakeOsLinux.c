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
 * @File    CFakeOsLinux.c
 * @Brief   It provide linux related interface for internal use in CFake.
 ******************************************************************************/

#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "CFakeOs.h"
#include "CFakeLog.h"
#include "CFakePlatform.h"
#include "CFakeHw.h"

static inline TFakeUInt FakeOsLinux_GetPageSize(void)
{
    return sysconf(_SC_PAGE_SIZE);
}

/************************* Method Definitions Start ***************************/

/* Method: Alloc memory protection(Before modify code section of fake action) */
static void
FakeOsLinux_AllocMemoryProtect(SFakeConfigParam * configParamPtr,
                               TFakeU8 *          infoPtr)
{
    TFakeUInt   pageSize  = FakeOsLinux_GetPageSize();
    void      * startAddr = configParamPtr->funcAddr;
    void      * endAddr   = startAddr + gFakeHw.GetCorruptedCodeSize();
    void      * pageAddr  = startAddr - (size_t)startAddr % pageSize;
    TFakeUInt   pageNum   = (endAddr - pageAddr - 1) / pageSize + 1;

    /* TBD:  how to save previous prot flags?? */
    (void)infoPtr;

    /* Set writable memory protection for the necessary code section */
    if (mprotect(pageAddr,
                 pageNum * pageSize,
                 PROT_READ | PROT_WRITE | PROT_EXEC))
    {
        gFakePlatform.RaiseFatal("Fail to alloc for writing code! %s",
                                 strerror(errno));
    }
}

/* Method: Free memory protection(After modify code section of fake action) */
static void
FakeOsLinux_FreeMemoryProtect(SFakeConfigParam * configParamPtr,
                              TFakeU8 *          infoPtr)
{
    (void)configParamPtr;
    (void)infoPtr;

    /* TBD: No solution, as the previous prot flags is not saved.. */
}

/************************* Method Definitions End *****************************/

/* Component Object Definition(Singleton Pattern) */
SFakeOs gFakeOs = {
    FakeOsLinux_AllocMemoryProtect,     /* Method: AllocMemoryProtect */
    FakeOsLinux_FreeMemoryProtect,      /* Method: FreeMemoryProtect  */
    0,                                  /* Data:   infoSize           */
};

