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
 *          And the fake action could be cancel at the end of the case. 
 *
 *          In another word, one func could be dynamically replaced as any
 *          other stub during testing program is running.
 *
 *          You may add this library to an existing unit testing framework
 *          to power your testing performance, e.g. cmockery, CppUnit.
 ******************************************************************************/

#include "CFakePlatform.h"
#include "CFakeDatabase.h"
#include "CFakeLog.h"

#define FAKE_CONFIG_API_NUM     (EFakeConfigType_Num - 1)

typedef void (*TFakeConfigApi)(SFakeConfigParam * configParamPtr);

/* Fake one func with anther mock */
static void Fake_On(SFakeConfigParam * configParamPtr);

/* Release the faked func */
static void Fake_Off(SFakeConfigParam * configParamPtr);

/* Release the faked func. */
static void Fake_Reset(SFakeConfigParam * configParamPtr);

/* Config API Assignment */
TFakeConfigApi gFakeConfigApi[FAKE_CONFIG_API_NUM] = {
    Fake_On,
    Fake_Off,
    Fake_Reset,
};

/* Fake one func with anther mock. */
static void Fake_On(SFakeConfigParam * configParamPtr)
{
    SFakeDataHandle    dataHandle;
    SFakeDataInfo    * dataInfoPtr;
    SFakeConfigParam   oldParam;

    dataHandle = gFakeDb.GetDataHandle(configParamPtr);
    if (dataHandle != NULL)
    {
        gFakeDb.ReadConfigParam(dataHandle, &oldParam);
        gFakeLog.Warning("%s(%p) is faked by %s(%p) yet! "
                         "Recover now..",
                         oldParam.funcName,
                         oldParam.funcAddr,
                         oldParam.mockName,
                         oldParam.mockAddr);
        Fake_Off(&oldParam);
    }

    gFakeLog.Info("Fake_On: %s(%p) => %s(%p)",
                  configParamPtr->funcName,
                  configParamPtr->funcAddr,
                  configParamPtr->mockName,
                  configParamPtr->mockAddr);

    dataInfoPtr = gFakePlatform.EnableMock(configParamPtr);
    (void)gFakeDb.PushDataInfo(configParamPtr, dataInfoPtr);
}

/* Release the faked func. */
static void Fake_Off(SFakeConfigParam * configParamPtr)
{
    SFakeDataHandle   dataHandle;
    SFakeDataInfo   * dataInfoPtr;

    gFakeLog.Info("Fake_Off: Recover %s(%p)",
                  configParamPtr->funcName,
                  configParamPtr->funcAddr);

    dataHandle = gFakeDb.GetDataHandle(configParamPtr);
    if (dataHandle == NULL)
    {
        gFakeLog.Warning("%s(%p) isn't faked by any func!",
                         configParamPtr->funcName,
                         configParamPtr->funcAddr);
        return;
    }

    dataInfoPtr = gFakeDb.PopDataInfo(dataHandle);
    gFakePlatform.DisableMock(configParamPtr, &dataInfoPtr);
}

/* Reset fake database and release all faked func. */
static void Fake_Reset(SFakeConfigParam * configParamPtr)
{
    SFakeDataHandle    dataHandle;
    SFakeConfigParam   oldParam;

    gFakeLog.Info("Fake_Rest is running now..");

    while ((dataHandle = gFakeDb.GetFirstDataHandle()) != NULL)
    {
        gFakeDb.ReadConfigParam(dataHandle, &oldParam);
        Fake_Off(&oldParam);
    }
}

/* Dispatch config param to trusted API according to config type */
void Fake_Config(SFakeConfigParam * configParamPtr)
{
    TFakeConfigApi configApi;

    gFakePlatform.CheckIn(configParamPtr);
    configApi = gFakeConfigApi[configParamPtr->configType - 1];
    configApi(configParamPtr);
}

