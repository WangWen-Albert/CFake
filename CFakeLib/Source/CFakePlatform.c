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
 * @File    CFakePlatform.c
 * @Brief   It isolate the difference of os api and hardware from the bussiness.
 ******************************************************************************/

#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "CFakePlatform.h"
#include "CFakeOs.h"
#include "CFakeHw.h"
#include "CFakeLog.h"

/* Check if config type is valid for config param */
static void FakePlatform_CheckInConfigType(SFakeConfigParam * configParamPtr)
{
    EFakeConfigType configType = configParamPtr->configType;

    if (configType <= EFakeConfigType_NULL || configType >= EFakeConfigType_Num)
    {
        gFakePlatform.RaiseFatal("Invalid configType(%d), not in [%d, %d]! "
                                 "FILE: %s, LINE: %d",
                                 configParamPtr->configType,
                                 EFakeConfigType_NULL + 1,
                                 EFakeConfigType_Num - 1,
                                 configParamPtr->sourceFile,
                                 configParamPtr->sourceLine);
        }
}

/* Check if the given func is valid for config param */
static void FakePlatform_CheckInFunc(SFakeConfigParam * configParamPtr)
{
    EFakeConfigType configType = configParamPtr->configType;

    if (configType == EFakeConfigType_On || configType == EFakeConfigType_Off)
    {
        if (configParamPtr->funcName == NULL)
        {
            configParamPtr->funcName = "N/A";
        }

        if (configParamPtr->funcAddr == NULL)
        {
            gFakePlatform.RaiseFatal("Invalid func, addr: %p, name: %s! "
                                     "FILE: %s, LINE: %d",
                                     configParamPtr->funcAddr,
                                     configParamPtr->funcName,
                                     configParamPtr->sourceFile,
                                     configParamPtr->sourceLine);
        }
    }
}

/* Check if the given mock is valid for config param */
static void FakePlatform_CheckInMock(SFakeConfigParam * configParamPtr)
{    
    EFakeConfigType configType = configParamPtr->configType;

    if (configType == EFakeConfigType_On)
    {
        if (configParamPtr->mockName == NULL)
        {
            configParamPtr->mockName = "N/A";
        }

        if (configParamPtr->mockAddr == NULL)
        {
            gFakePlatform.RaiseFatal("Invalid mock, addr: %p, name: %s! "
                                     "FILE: %s, LINE: %d",
                                     configParamPtr->mockAddr,
                                     configParamPtr->mockName,
                                     configParamPtr->sourceFile,
                                     configParamPtr->sourceLine);
        }

        if (configParamPtr->mockAddr == configParamPtr->funcAddr)
        {
            gFakePlatform.RaiseFatal("Invalid mock, addr: %p, name: %s! "
                                     "Cannot fake itself! "
                                     "FILE: %s, LINE: %d",
                                     configParamPtr->mockAddr,
                                     configParamPtr->mockName,
                                     configParamPtr->sourceFile,
                                     configParamPtr->sourceLine);
        }
    }

}

/************************* Method Definitions Start ***************************/

/* Method: Quit current progress after print errors */
static void FakePlatform_RaiseFatal(char * format, ...)
{
    va_list args;

    va_start(args, format);
    gFakeLog.VCritical(format, args);
    va_end(args);

    exit(1);
}

/* Method: Check if config param is valid */
static void FakePlatform_CheckIn(SFakeConfigParam * configParamPtr)
{
    if (configParamPtr == NULL)
    {
        gFakePlatform.RaiseFatal("configParamPtr is NULL! "
                                 "FILE: %s, LINE: %d",
                                 configParamPtr->sourceFile,
                                 configParamPtr->sourceLine);
    }

    FakePlatform_CheckInConfigType(configParamPtr);
    FakePlatform_CheckInFunc(configParamPtr);
    FakePlatform_CheckInMock(configParamPtr);
}

/* Method: Replace func as mock */
static SFakeDataInfo * 
FakePlatform_EnableMock(SFakeConfigParam * configParamPtr)
{
    assert (configParamPtr != NULL);
    {
        SFakeDataInfo * infoPtr;
        TFakeU8       * osInfoPtr;
        TFakeU8       * hwInfoPtr;
        TFakeUInt       infoSize = gFakeOs.infoSize + gFakeHw.infoSize;

        infoPtr   = gFakeDb.AllocDataInfo(infoSize);
        osInfoPtr = infoPtr->info;
        hwInfoPtr = infoPtr->info + gFakeOs.infoSize;

        gFakeOs.AllocMemoryProtect(configParamPtr, osInfoPtr);
        gFakeHw.MakeCorruptedCode(configParamPtr, hwInfoPtr);
        gFakeHw.EnableCorruptedCode(configParamPtr, hwInfoPtr);
        gFakeOs.FreeMemoryProtect(configParamPtr, osInfoPtr);

        return infoPtr;
    }
}

/* Method: Recover func from mock */
static void FakePlatform_DisableMock(SFakeConfigParam *  configParamPtr,
                                     SFakeDataInfo    ** dataInfoPtr)
{
    assert (configParamPtr != NULL);
    assert (dataInfoPtr    != NULL);
    assert (*dataInfoPtr   != NULL);
    {
        TFakeU8 * osInfoPtr = (*dataInfoPtr)->info;
        TFakeU8 * hwInfoPtr = (*dataInfoPtr)->info + gFakeOs.infoSize;

        gFakeOs.AllocMemoryProtect(configParamPtr, osInfoPtr);
        gFakeHw.DisableCorruptedCode(configParamPtr, hwInfoPtr);
        gFakeOs.FreeMemoryProtect(configParamPtr, osInfoPtr);
        gFakeDb.FreeDataInfo(dataInfoPtr);
    }
}

/************************* Method Definitions End *****************************/

/* Component Object Definition(Singleton Pattern) */
SFakePlatform gFakePlatform = {
    FakePlatform_RaiseFatal,            /* Method: RaiseFatal  */
    FakePlatform_CheckIn,               /* Method: CheckIn     */
    FakePlatform_EnableMock,            /* Method: EnableMock  */
    FakePlatform_DisableMock,           /* Method: DisableMock */
};

