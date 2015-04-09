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
 * @File    CFake.c
 * @Brief   It provide a fast and simple solution to do unit testing.
 *
 *          When using FAKE_ON to fake one func with anther mock in your
 *          test case, the mock will be called instead of the faked func.
 *          And the fake could be cancel at the end of the case. 
 *
 *          You may add this library to an existing unit testing framework
 *          to power your testing ability, e.g. cmockery, CppUnit.
 ******************************************************************************/

#include <stdio.h>
#include <assert.h>
#include "CFakePlatform.h"
#include "CFakeDatabase.h"
#include "CFakeLog.h"

typedef void (*TFakeConfigApi) (SFakeConfigParam * configParamPtr);

/* Fake one func with anther mock */
static void FakeOn (SFakeConfigParam * configParamPtr);

/* Release the faked func */
static void FakeOff (SFakeConfigParam * configParamPtr);

/* Release the faked func. */
static void FakeReset (SFakeConfigParam * configParamPtr);

TFakeConfigApi gFakeConfigApi[EFakeConfigType_Num] = {
    FakeOn,
    FakeOff,
    FakeReset,
};

/*****************************************************
 @Description:
   Fake one func with anther mock.
 @Parameter:
   func : the function which want to be faked.
   mock : the function which is used to fack others.
 *****************************************************/
static void FakeOn(SFakeConfigParam * configParamPtr)
{
    SFakeDataHandle    dataHandle;
    SFakeDataInfo    * dataInfoPtr;
    SFakeConfigParam   oldParam;

    gFakeLog.Info("Fake On: FUNC<%s, 0x%X>, MOCK<%s, 0x%X>",
                  configParamPtr->funcName,
                  (TFakeUInt)configParamPtr->funcAddr,
                  configParamPtr->mockName,
                  (TFakeUInt)configParamPtr->mockAddr);

    dataHandle = gFakeDb.GetDataHandle(configParamPtr);
    if (dataHandle != NULL)
    {
        gFakeDb.ReadConfigParam(dataHandle, &oldParam);
        gFakeLog.Warning("FUNC<%s, 0x%X> is already faked by MOCK<%s, 0x%X>",
                         oldParam.funcName,
                         (TFakeUInt)oldParam.funcAddr,
                         oldParam.mockName,
                         (TFakeUInt)oldParam.mockAddr);
        return;
    }

    dataInfoPtr = gFakePlatform.EnableConfig(configParamPtr);
    (void)gFakeDb.PushDataInfo(configParamPtr, dataInfoPtr);
}

/*****************************************************
 @Description:
   Release the faked func.
 @Parameter:
   func : the faked function address
 *****************************************************/
static void FakeOff(SFakeConfigParam * configParamPtr)
{
    SFakeDataHandle   dataHandle;
    SFakeDataInfo   * dataInfoPtr;

    gFakeLog.Info("Fake Off: Func<%s, 0x%X>",
                  configParamPtr->funcName,
                  (TFakeUInt)configParamPtr->funcAddr);

    dataHandle = gFakeDb.GetDataHandle(configParamPtr);
    if (dataHandle == NULL)
    {
        gFakeLog.Warning("FUNC<%s, 0x%X> is not faked by any MOCK",
                         configParamPtr->funcName,
                         (TFakeUInt)configParamPtr->funcAddr);
        return;
    }

    dataInfoPtr = gFakeDb.PopDataInfo(dataHandle);
    gFakePlatform.DisableConfig(configParamPtr, &dataInfoPtr);
}

/*****************************************************
 @Description:
   Reset fake database and release all faked func.
 @Parameter:
   N/A
 *****************************************************/
static void FakeReset(SFakeConfigParam * configParamPtr)
{
    SFakeDataHandle    dataHandle;
    SFakeConfigParam   oldParam;

    gFakeLog.Info("Fake Rest now..");

    while ((dataHandle = gFakeDb.GetFirstDataHandle()) != NULL)
    {
        gFakeDb.ReadConfigParam(dataHandle, &oldParam);
        FakeOff(&oldParam);
    }
}

void FakeConfig(SFakeConfigParam * configParamPtr)
{
    TFakeConfigApi configApi;

    gFakePlatform.CheckIn(configParamPtr);
    configApi = gFakeConfigApi[configParamPtr->configType - 1];
    configApi(configParamPtr);
}

