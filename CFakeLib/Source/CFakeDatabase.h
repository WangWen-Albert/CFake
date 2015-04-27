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
 * @File    CFakeDatabase.h
 * @Brief   It provide database for internal use in CFake.
 ******************************************************************************/

#ifndef __C_FAKE_DATABASE_H__
#define __C_FAKE_DATABASE_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "CFakeTypes.h"

/* Fake internal data info, which is used to restore fake action */
typedef struct SFakeDataInfo {
    TFakeUInt   size;
    TFakeU8     info[FAKE_FLEXIBLE_ARRAY_SIZE];
} SFakeDataInfo;

typedef void * SFakeDataHandle;

/* Component Object Method Definitions */
typedef struct SFakeDatabase {
    /* Get first handle in database, for visiting hanlde one by one */
    SFakeDataHandle (* GetFirstDataHandle) (void);

    /* Get next handle by current one, for visiting hanlde one by one */
    SFakeDataHandle (* GetNextDataHandle) (SFakeDataHandle currentDataHandle);

    /* Get handle by config param, Note: key is funcAddr! */
    SFakeDataHandle (* GetDataHandle) (SFakeConfigParam * configParamPtr);

    /* Push data info with config param, Note: it's saved into database! */
    SFakeDataHandle (* PushDataInfo) (SFakeConfigParam * configParamPtr,
                                      SFakeDataInfo    * dateInfo);

    /* Pop data info by handle, Note: it's removed from database! */
    SFakeDataInfo * (* PopDataInfo) (SFakeDataHandle dataHandle);

    /* Get data info by handle, Note: it's still saved in database! */
    SFakeDataInfo * (* GetDataInfo) (SFakeDataHandle dataHandle);

    /* Read config param saved in database by handle */
    void (* ReadConfigParam) (SFakeDataHandle    dataHandle,
                              SFakeConfigParam * configParamPtr);

    /* Alloc memory for data info, which is used to restore fake action */
    SFakeDataInfo * (* AllocDataInfo) (TFakeUInt size);

    /* free memory for data info, which is used to restore fake action */
    void (* FreeDataInfo) (SFakeDataInfo ** dataInfoPtr);
} SFakeDatabase;

/* Component Object Statement(Singleton Pattern) */
extern SFakeDatabase gFakeDb;

#ifdef __cplusplus
}
#endif

#endif /* __C_FAKE_DATABASE_H__ */

