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
 * @File    CFakeDatabase.c
 * @Brief   It provide database for internal use in CFake.
 ******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "CFakePlatform.h"
#include "CFakeDatabase.h"
#include "CFakeLog.h"

#define FAKE_DATABASE_INIT_BYTE     0xAA

typedef struct SFakeDataItem {
    struct SFakeDataItem * prev;
    struct SFakeDataItem * next;
    SFakeDataInfo        * dataInfoPtr;
    SFakeConfigParam       configParam;
} SFakeDataItem;

static SFakeDataItem gFakeDbDataItemList = {
    &gFakeDbDataItemList,       /* prev */
    &gFakeDbDataItemList,       /* next */
    NULL,                       /* dataInfoPtr */
};

static inline EFakeBool FakeDb_IsEmpty(void)
{
    return gFakeDbDataItemList.next == &gFakeDbDataItemList;
}

static SFakeDataHandle FakeDb_GetFirstDataHandle(void)
{
    SFakeDataHandle dataHandle = NULL;

    if (!FakeDb_IsEmpty())
    {
        dataHandle = (SFakeDataHandle)gFakeDbDataItemList.next;
    }

    return dataHandle;
}

/************************* Method Definitions Start ***************************/

static SFakeDataHandle 
FakeDb_GetNextDataHandle(SFakeDataHandle currentDataHandle)
{
    SFakeDataHandle   dataHandle = NULL;
    SFakeDataItem   * dataItem;

    if (currentDataHandle != NULL)
    {
        dataItem = (SFakeDataItem *)currentDataHandle;
        if (dataItem->next != &gFakeDbDataItemList)
        {
            dataHandle = (SFakeDataHandle)dataItem->next;
        }
    }

    return dataHandle;
}

static SFakeDataHandle FakeDb_GetDataHandle(SFakeConfigParam * configParamPtr)
{
    assert (configParamPtr != NULL);
    {
        SFakeDataHandle  dataHandle;
        SFakeConfigParam oldParam;

        dataHandle = gFakeDb.GetFirstDataHandle();
        while (dataHandle != NULL)
        {
            gFakeDb.ReadConfigParam(dataHandle, &oldParam);
            if (oldParam.funcAddr == configParamPtr->funcAddr)
            {
                break;
            }

            dataHandle = gFakeDb.GetNextDataHandle(dataHandle);
        }

        return dataHandle;
    }
}

static SFakeDataHandle FakeDb_PushDataInfo(SFakeConfigParam * configParamPtr,
                                           SFakeDataInfo    * dataInfoPtr)
{
    assert (configParamPtr != NULL);
    assert (dataInfoPtr    != NULL);
    {
        SFakeDataItem * dataItem = NULL;

        dataItem = (SFakeDataItem *)malloc(sizeof(SFakeDataItem));
        if (dataItem != NULL)
        {
            dataItem->configParam = *configParamPtr;
            dataItem->dataInfoPtr = dataInfoPtr;

            dataItem->next = gFakeDbDataItemList.next;
            dataItem->prev = &gFakeDbDataItemList;
            gFakeDbDataItemList.next->prev = dataItem;
            gFakeDbDataItemList.next       = dataItem;   
        }
        else
        {
             gFakeLog.Error("Memory not enough for SFakeDataItem "
                            "size: %d!",
                            sizeof(SFakeDataItem));
        }

        return (SFakeDataItem *)dataItem;
    }
}

static SFakeDataInfo * FakeDb_PopDataInfo(SFakeDataHandle dataHandle)
{
    SFakeDataInfo * dataInfoPtr = NULL;
    SFakeDataItem * dataItem;

    if (dataHandle != NULL)
    {
        dataItem = (SFakeDataItem *)dataHandle;
        if (dataItem != &gFakeDbDataItemList)
        {
            dataInfoPtr = dataItem->dataInfoPtr;

            dataItem->next->prev = dataItem->prev;
            dataItem->prev->next = dataItem->next;

            free(dataItem);
        }
    }

    return dataInfoPtr;
}

static SFakeDataInfo * FakeDb_GetDataInfo(SFakeDataHandle dataHandle)
{
    SFakeDataInfo * dataInfoPtr = NULL;
    SFakeDataItem * dataItem;

    if (dataHandle != NULL)
    {
        dataItem    = (SFakeDataItem *)dataHandle;
        dataInfoPtr = dataItem->dataInfoPtr;
    }

    return dataInfoPtr;
}

static void FakeDb_ReadConfigParam(SFakeDataHandle    dataHandle,
                                   SFakeConfigParam * configParamPtr)
{
    assert (configParamPtr != NULL);
    {
        SFakeDataItem * dataItem;

        if (dataHandle != NULL)
        {
            dataItem        = (SFakeDataItem *)dataHandle;
            *configParamPtr = dataItem->configParam;
        }
    }
}

static SFakeDataInfo * FakeDb_AllocDataInfo(TFakeUInt size)
{
    assert (size > 0);
    {
        SFakeDataInfo * dataInfoPtr = NULL;

        dataInfoPtr = (SFakeDataInfo *)malloc(sizeof(SFakeDataInfo) + size);
        if (dataInfoPtr == NULL)
        {
            gFakeLog.Error("Memory not enough for SFakeDataInfo, size: %u!",
                           size);
        }
        dataInfoPtr->size = size;
        memset(dataInfoPtr->info, FAKE_DATABASE_INIT_BYTE, size);

        return dataInfoPtr;
    }
}

static void FakeDb_FreeDataInfo(SFakeDataInfo ** dataInfoPtr)
{
    if (dataInfoPtr != NULL)
    {
        free(*dataInfoPtr);
        *dataInfoPtr = NULL;
    }
}

/************************* Method Definitions End *****************************/

/* Component Object Definition(Singleton Pattern) */
SFakeDatabase gFakeDb = {
    FakeDb_GetFirstDataHandle,
    FakeDb_GetNextDataHandle,
    FakeDb_GetDataHandle,
    FakeDb_PushDataInfo,
    FakeDb_PopDataInfo,
    FakeDb_GetDataInfo,
    FakeDb_ReadConfigParam,
    FakeDb_AllocDataInfo,
    FakeDb_FreeDataInfo,
};

