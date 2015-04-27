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
 * @File    CFakeOs.h
 * @Brief   It provide os related interface for internal use in CFake.
 ******************************************************************************/

#ifndef __C_FAKE_OS_H__
#define __C_FAKE_OS_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "CFakeTypes.h"

/* Component Object Method Definitions */
typedef struct SFakeOs {
    /* Alloc memory protection(Before modify code section of fake action) */
    void (* AllocMemoryProtect)(SFakeConfigParam * configParamPtr,
                                TFakeU8          * infoPtr);

    /* Free memory protection(After modify code section of fake action) */
    void (* FreeMemoryProtect)(SFakeConfigParam * configParamPtr,
                               TFakeU8          * infoPtr);

    TFakeUInt const infoSize;
} SFakeOs;

/* Component Object Statement(Singleton Pattern) */
extern SFakeOs gFakeOs;

#ifdef __cplusplus
}
#endif

#endif /* __C_FAKE_OS_H__ */

