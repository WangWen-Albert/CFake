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
 * @File    CFakeHwX86.h
 * @Brief   It provide X86 related interface for internal use in CFake.
 ******************************************************************************/
#include <assert.h>
#include <string.h>
#include "CFakeHw.h"
#include "CFakeLog.h"

#if defined(__x86_64__)
#   define  FAKE_SYS32
#elif defined(__i386__)
#   define  FAKE_SYS64
#else
#   error "Unsupported x86 system(not 32bit or 64bit)"
#endif

#if defined(FAKE_SYS32)
#   define FAKE_HW_CORRUPTED_CODE_SIZE      5
#   define FAKE_HW_CORRUPTED_CODE_INDEX     1
#elif defined(FAKE_SYS64)
#   define FAKE_HW_CORRUPTED_CODE_SIZE      12
#   define FAKE_HW_CORRUPTED_CODE_INDEX     2
#endif

/* INFO Format: 0xAA, 'corrupted code', '0xAA', 'original code', '0xAA' */
#define FAKE_HW_INFO_SIZE            (FAKE_HW_CORRUPTED_CODE_SIZE * 2 + 3)
#define FAKE_HW_INFO_GUARDER_BYTE    0xAA

#if defined(FAKE_SYS32)
#   define FAKE_HW_CORRUPTED_CODE_ADDR(configParamPtr)  \
    ((configParamPtr)->mockAddr -                       \
     (configParamPtr)->funcAddr -                       \
     FAKE_HW_CORRUPTED_CODE_SIZE)
#elif defined(FAKE_SYS64)
#   define FAKE_HW_CORRUPTED_CODE_ADDR(configParamPtr)  \
    ((configParamPtr)->mockAddr)
#endif

/* INFO Format: 0xAA, 'corrupted code', '0xAA', 'original code', '0xAA' */
#define FAKE_HW_INFO_CORRUPTED_CODE_HEADER(infoPtr)   \
    ((TFakeU8 *)(infoPtr) + 1)

/* INFO Format: 0xAA, 'corrupted code', '0xAA', 'original code', '0xAA' */
#define FAKE_HW_INFO_ORIGINAL_CODE_HEADER(infoPtr, codeSize)  \
    ((TFakeU8 *)(infoPtr) + codeSize + 2)

/* INFO Format: 0xAA, 'corrupted code', '0xAA', 'original code', '0xAA' */
#define FAKE_HW_INFO_MOCK_CODE_HEADER(infoPtr, index)  \
    ((TFakeU8 *)(infoPtr) + 1 + index)

/******************************************************************************
 * SYS32 : __asm("jmp  $mock")                  : 0xE9 {$mock - $func - 5}    *
 * SYS32 : __asm("movl $mock, %eax; jmpl %eax") : 0xB8 {$mock} 0xFF 0xE0      *
 * SYS64 : __asm("movq $mock, %rax; jmpq %rax") : 0x48 0xB8 {$mock} 0xFF 0xE0 * 
 ******************************************************************************/
#if defined(FAKE_SYS32)
TFakeU8 gFakeHwCorruptedCode[FAKE_HW_CORRUPTED_CODE_SIZE] = {
    0xE9, 0x0, 0x0, 0x0, 0x0,
};
#elif defined(FAKE_SYS64)
TFakeU8 gFakeHwCorruptedCode[FAKE_HW_CORRUPTED_CODE_SIZE] = {
    0x48, 0xB8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xE0,
};
#endif

static void
FakeHw86_SetTemplate(TFakeUInt codeSize, TFakeU8 * codePtr, TFakeU8 * infoPtr)
{
    *infoPtr = FAKE_HW_INFO_GUARDER_BYTE;

    *FAKE_HW_INFO_CORRUPTED_CODE_HEADER(infoPtr) = FAKE_HW_INFO_GUARDER_BYTE;

    *FAKE_HW_INFO_ORIGINAL_CODE_HEADER(infoPtr,
                                       codeSize) = FAKE_HW_INFO_GUARDER_BYTE;

    (void)memcpy(FAKE_HW_INFO_CORRUPTED_CODE_HEADER(infoPtr),
                 codePtr,
                 codeSize);
}

/************************* Method Definitions Start ***************************/

static TFakeUInt FakeHwX86_GetCorruptedCodeSize(void)
{
    return FAKE_HW_CORRUPTED_CODE_SIZE;
}

static void
FakeHwX86_MakeCorruptedCode(SFakeConfigParam * configParamPtr,
                            TFakeU8          * infoPtr)
{
    TFakeUInt   codeSize  = FAKE_HW_CORRUPTED_CODE_SIZE;
    TFakeU8   * codePtr   = gFakeHwCorruptedCode;
    TFakeUInt   addrIndex = FAKE_HW_CORRUPTED_CODE_INDEX;
    void      * addrInfo  = FAKE_HW_CORRUPTED_CODE_ADDR(configParamPtr);

    FakeHw86_SetTemplate(codeSize, codePtr, infoPtr);

    (void)memcpy(FAKE_HW_INFO_MOCK_CODE_HEADER(infoPtr, addrIndex),
                (&addrInfo),
                sizeof(void*));
}

static void
FakeHwX86_EnableCorruptedCode(SFakeConfigParam * configParamPtr,
                              TFakeU8          * infoPtr)
{
    TFakeUInt const codeSize = FAKE_HW_CORRUPTED_CODE_SIZE;

    (void)memcpy(FAKE_HW_INFO_ORIGINAL_CODE_HEADER(infoPtr, codeSize),
                 configParamPtr->funcAddr,
                 codeSize);
    (void)memcpy(configParamPtr->funcAddr,
                 FAKE_HW_INFO_CORRUPTED_CODE_HEADER(infoPtr),
                 codeSize);
}

static void
FakeHwX86_DisableCorruptedCode(SFakeConfigParam * configParamPtr,
                               TFakeU8          * infoPtr)
{
    TFakeUInt const codeSize = FAKE_HW_CORRUPTED_CODE_SIZE;

    (void)memcpy(configParamPtr->funcAddr,
                 FAKE_HW_INFO_ORIGINAL_CODE_HEADER(infoPtr, codeSize),
                 codeSize);
}

/************************* Method Definitions End *****************************/

/* Component Object Definition(Singleton Pattern) */
SFakeHw gFakeHw = {
    FakeHwX86_GetCorruptedCodeSize,
    FakeHwX86_MakeCorruptedCode,
    FakeHwX86_EnableCorruptedCode,
    FakeHwX86_DisableCorruptedCode,
    FAKE_HW_INFO_SIZE,
};

