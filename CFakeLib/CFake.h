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
 * @File    CFake.h
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

#ifndef __C_FAKE_H__
#define __C_FAKE_H__

#ifdef __cplusplus
    extern "C" {
#endif

/************************* Interface Definitions Start ************************/

/* Fake one func with anther mock. */
#define FAKE_ON(func, mock)     do {                \
    SFakeConfigParam configParam;                   \
    configParam.configType = EFakeConfigType_On;    \
    configParam.funcAddr   = func;                  \
    configParam.funcName   = #func;                 \
    configParam.mockAddr   = mock;                  \
    configParam.mockName   = #mock;                 \
    configParam.sourceFile = __FILE__;              \
    configParam.sourceLine = __LINE__;              \
    Fake_Config(&configParam);                      \
} while (0)

/* Release the faked func. */
#define FAKE_OFF(func)          do {                \
    SFakeConfigParam configParam;                   \
    configParam.configType = EFakeConfigType_Off;   \
    configParam.funcAddr   = func;                  \
    configParam.funcName   = #func;                 \
    configParam.sourceFile = __FILE__;              \
    configParam.sourceLine = __LINE__;              \
    Fake_Config(&configParam);                      \
} while (0)

/* Reset fake database and release all faked func. */
#define FAKE_RESET()            do {                \
    SFakeConfigParam configParam;                   \
    configParam.configType = EFakeConfigType_Reset; \
    configParam.sourceFile = __FILE__;              \
    configParam.sourceLine = __LINE__;              \
    Fake_Config(&configParam);                      \
} while (0)

/************************* Interface Definitions End **************************/

/************ Types Definitions Start (Use interfaces instead of them) ********/

typedef enum EFakeConfigType {
    EFakeConfigType_NULL  = 0,      /* RESERVED, MUST NOT REMOVE */
    EFakeConfigType_On    = 1,      /* FAKE_ON */
    EFakeConfigType_Off   = 2,      /* FAKE_OFF */
    EFakeConfigType_Reset = 3,      /* FAKE_RESET */
    EFakeConfigType_Num   = 4,
} EFakeConfigType;

typedef struct SFakeConfigParam {
    EFakeConfigType   configType;
    void            * funcAddr;     /* Addr of the function to be faked. */
    char            * funcName;     /* Name of the function to be faked. */
    void            * mockAddr;     /* Addr of the function to fack others. */
    char            * mockName;     /* Name of the function to fack others. */
    char            * sourceFile;   /* File name of config position in source */
    unsigned int      sourceLine;   /* Line no. of config position in source */
} SFakeConfigParam;

/************ Types Definitions End (Use interfaces instead of them) **********/

/* The marco FAKE_XXX is recommend instead of using FakeConfig directly */
void Fake_Config(SFakeConfigParam * configParamPtr);

#ifdef __cplusplus
}
#endif

#endif /* __C_FAKE_H__ */

