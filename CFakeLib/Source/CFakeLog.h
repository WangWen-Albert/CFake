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
 * @File    CFakePlatform.h
 * @Brief   It provide a fast and simple solution to do unit testing.
 *
 *          When using FAKE_ON to fake one func with anther mock in your
 *          test case, the mock will be called instead of the faked func.
 *          And the fake could be cancel at the end of the case. 
 *
 *          You may add this library to an existing unit testing framework
 *          to power your testing ability, e.g. cmockery, CppUnit.
 ******************************************************************************/

#ifndef __C_FAKE_LOG_H__
#define __C_FAKE_LOG_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include "CFakeTypes.h"

#define FAKE_LOG_ATTR \
    FAKE_ATTR((__format__ (printf, 1, 2)))

#define FAKE_VLOG_ATTR \
    FAKE_ATTR((__format__ (printf, 1, 0)))

typedef enum EFakeLogLevel {
    EFakeLogLevel_Debug    = 0,
    EFakeLogLevel_Info     = 1,
    EFakeLogLevel_Warning  = 2,
    EFakeLogLevel_Error    = 3,
    EFakeLogLevel_Critical = 4,
} EFakeLogLevel;

typedef enum EFakeLogMode {
    EFakeLogMode_Stdout     = 0,
    EFakeLogMode_NewFile    = 1,
    EFakeLogMode_AppendFile = 2,
} EFakeLogMode;

typedef void FAKE_LOG_ATTR  (* TFakeLogApi)  (char * format, ...);
typedef void FAKE_VLOG_ATTR (* TFakeVLogApi) (char * format, va_list args);

typedef struct SFakeLog {
    TFakeLogApi  Debug;
    TFakeLogApi  Info;
    TFakeLogApi  Warning;
    TFakeLogApi  Error;
    TFakeLogApi  Critical;
    TFakeVLogApi VDebug;
    TFakeVLogApi VInfo;
    TFakeVLogApi VWarning;
    TFakeVLogApi VError;
    TFakeVLogApi VCritical;

    /*****************************************************
     @Description:
       Custom log output level, Default is EFakeLogLevel_Info
     @Parameter:
       logLevel: new log output level
     *****************************************************/
    void (* SetLevel) (EFakeLogLevel logLevel);

    /*****************************************************
     @Description:
       Custom log output mode, Default is EFakeLogMode_Stdout
     @Parameter:
       logLevel: new log output level
       modeInfo: pointer to filename if using file for output
     *****************************************************/
    void (* SetMode) (EFakeLogMode logMode, void * modeInfo);
} SFakeLog;

extern SFakeLog gFakeLog;

#ifdef __cplusplus
}
#endif

#endif /* __C_FAKE_LOG_H__ */

