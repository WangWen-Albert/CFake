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
 * @File    CFakeLog.h
 * @Brief   It provide log related interface for internal use in CFake.
 ******************************************************************************/

#ifndef __C_FAKE_LOG_H__
#define __C_FAKE_LOG_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include "CFakeTypes.h"

/* Compile-Time Checking for format string with args */
#define FAKE_LOG_ATTR \
    FAKE_ATTR((__format__ (printf, 1, 2)))

/* Compile-Time Checking for format string with args */
#define FAKE_VLOG_ATTR \
    FAKE_ATTR((__format__ (printf, 1, 0)))

/* Print log level */
typedef enum EFakeLogLevel {
    EFakeLogLevel_Debug    = 0,
    EFakeLogLevel_Info     = 1,
    EFakeLogLevel_Warning  = 2,
    EFakeLogLevel_Error    = 3,
    EFakeLogLevel_Critical = 4,
} EFakeLogLevel;

/* Print log mode */
typedef enum EFakeLogMode {
    EFakeLogMode_Stdout     = 0,
    EFakeLogMode_NewFile    = 1,
    EFakeLogMode_AppendFile = 2,
} EFakeLogMode;

/* Print API */
typedef void (* TFakeLogApi)  (char * format, ...) FAKE_LOG_ATTR;

/* Print API with va_list */
typedef void (* TFakeVLogApi) (char * format, va_list args) FAKE_VLOG_ATTR;

/* Component Object Method Definitions */
typedef struct SFakeLog {
    TFakeLogApi  Debug;         /* Print in debug level */
    TFakeLogApi  Info;          /* Print in info level */
    TFakeLogApi  Warning;       /* Print in warning level */
    TFakeLogApi  Error;         /* Print in error level */
    TFakeLogApi  Critical;      /* Print in critical level */
    TFakeVLogApi VDebug;        /* Print in debug level with va_list */
    TFakeVLogApi VInfo;         /* Print in info level with va_list */
    TFakeVLogApi VWarning;      /* Print in warning level with va_list */
    TFakeVLogApi VError;        /* Print in error level with va_list */
    TFakeVLogApi VCritical;     /* Print in critical level with va_list */

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

/* Component Object Statement(Singleton Pattern) */
extern SFakeLog gFakeLog;

#ifdef __cplusplus
}
#endif

#endif /* __C_FAKE_LOG_H__ */

