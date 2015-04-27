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

#if defined(__i386__)
#   define  FAKE_SYS32
#elif defined(__x86_64__)
#   define  FAKE_SYS64
#else
#   error "Unsupported x86 system(not 32bit or 64bit)"
#endif

/* INFO Format: tag, 'corrupted code', tag, 'original code', tag. tag: 1 byte */
#define FAKE_HW_INFO_SIZE            (FAKE_HW_CORRUPTED_CODE_SIZE * 2 + 3)

/* Tag byte to guard data */
#define FAKE_HW_INFO_GUARDER_BYTE    0xAA

/* Corrupted code(i.e. trampoline asm code) size */
#if defined(FAKE_SYS32)
#   define FAKE_HW_CORRUPTED_CODE_SIZE      5
#elif defined(FAKE_SYS64)
#   define FAKE_HW_CORRUPTED_CODE_SIZE      12
#endif

/* addr info index in corrupted bytes(i.e. trampoline asm code) */
#if defined(FAKE_SYS32)
#   define FAKE_HW_CORRUPTED_CODE_INDEX     1
#elif defined(FAKE_SYS64)
#   define FAKE_HW_CORRUPTED_CODE_INDEX     2
#endif

/* addr info used in corrupted bytes(i.e. trampoline asm code) */
#if defined(FAKE_SYS32)
#   define FAKE_HW_CORRUPTED_CODE_ADDR(configParamPtr)  \
    ((configParamPtr)->mockAddr -                       \
     (configParamPtr)->funcAddr -                       \
     FAKE_HW_CORRUPTED_CODE_SIZE)
#elif defined(FAKE_SYS64)
#   define FAKE_HW_CORRUPTED_CODE_ADDR(configParamPtr)  \
    ((configParamPtr)->mockAddr)
#endif

/* INFO Format: tag, 'corrupted code', tag, 'original code', tag. tag: 1 byte */
#define FAKE_HW_INFO_CORRUPTED_CODE_HEADER(infoPtr)   \
    ((TFakeU8 *)(infoPtr) + 1)

/* INFO Format: tag, 'corrupted code', tag, 'original code', tag. tag: 1 byte */
#define FAKE_HW_INFO_ORIGINAL_CODE_HEADER(infoPtr, codeSize)  \
    ((TFakeU8 *)(infoPtr) + codeSize + 2)

/* INFO Format: tag, 'corrupted code', tag, 'original code', tag. tag: 1 byte */
#define FAKE_HW_INFO_MOCK_CODE_HEADER(infoPtr)  \
    ((TFakeU8 *)(infoPtr) + 1 + FAKE_HW_CORRUPTED_CODE_INDEX)

/******************************************************************************
 * Corrupted code format for fake action, i.e. trampoline asm code format     *
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

/* INFO Format: tag1, 'corrupted bytes', tag2, 'original bytes', tag3 */
static void
FakeHw86_SetTemplate(TFakeU8 * infoPtr)
{
    TFakeUInt const codeSize = FAKE_HW_CORRUPTED_CODE_SIZE;

    /* Set guarder byte in tag1 */
    *infoPtr = FAKE_HW_INFO_GUARDER_BYTE;

    /* Set guarder byte in tag2 */
    *FAKE_HW_INFO_CORRUPTED_CODE_HEADER(infoPtr) = FAKE_HW_INFO_GUARDER_BYTE;

    /* Set guarder byte in tag3 */
    *FAKE_HW_INFO_ORIGINAL_CODE_HEADER(infoPtr,
                                       codeSize) = FAKE_HW_INFO_GUARDER_BYTE;

    /* Fill corrupted code(i.e. trampoline asm code) format */
    (void)memcpy(FAKE_HW_INFO_CORRUPTED_CODE_HEADER(infoPtr),
                 gFakeHwCorruptedCode,
                 codeSize);
}

/************************* Method Definitions Start ***************************/

/* Method: Get corrupted code(i.e. trampoline asm code) size for fake action */
static TFakeUInt FakeHwX86_GetCorruptedCodeSize(void)
{
    return FAKE_HW_CORRUPTED_CODE_SIZE;
}

/* Method: Make corrupted code(i.e. trampoline asm code) for fake action */
static void
FakeHwX86_MakeCorruptedCode(SFakeConfigParam * configParamPtr,
                            TFakeU8          * infoPtr)
{
    /* Relative address for sys32 or Absolute address for sys64 */
    void * addrInfo  = (void *)FAKE_HW_CORRUPTED_CODE_ADDR(configParamPtr);

    /* Set template for corrupted code info which will be saved into database*/
    FakeHw86_SetTemplate(infoPtr);

    /* Fill mock(stub) info into the date info template */
    (void)memcpy(FAKE_HW_INFO_MOCK_CODE_HEADER(infoPtr),
                (&addrInfo),
                sizeof(void*));
}

/* Method: Enable corrupted code(i.e. trampoline asm code) for fake action */
static void
FakeHwX86_EnableCorruptedCode(SFakeConfigParam * configParamPtr,
                              TFakeU8          * infoPtr)
{
    TFakeUInt const codeSize = FAKE_HW_CORRUPTED_CODE_SIZE;

    /* Save original asm code on corrupted place into data info */
    (void)memcpy(FAKE_HW_INFO_ORIGINAL_CODE_HEADER(infoPtr, codeSize),
                 configParamPtr->funcAddr,
                 codeSize);

    /* Corrupt original asm code with trampoline asm code */
    (void)memcpy(configParamPtr->funcAddr,
                 FAKE_HW_INFO_CORRUPTED_CODE_HEADER(infoPtr),
                 codeSize);
}

/* Method: Disable corrupted code(i.e. trampoline asm code) for fake action */
static void
FakeHwX86_DisableCorruptedCode(SFakeConfigParam * configParamPtr,
                               TFakeU8          * infoPtr)
{
    TFakeUInt const codeSize = FAKE_HW_CORRUPTED_CODE_SIZE;

    /* Restore original asm code from corrupted asm code(trampoline code) */
    (void)memcpy(configParamPtr->funcAddr,
                 FAKE_HW_INFO_ORIGINAL_CODE_HEADER(infoPtr, codeSize),
                 codeSize);
}

/************************* Method Definitions End *****************************/

/* Component Object Definition(Singleton Pattern) */
SFakeHw gFakeHw = {
    FakeHwX86_GetCorruptedCodeSize,     /* Method: GetCorruptedCodeSize */
    FakeHwX86_MakeCorruptedCode,        /* Method: MakeCorruptedCode    */
    FakeHwX86_EnableCorruptedCode,      /* Method: EnableCorruptedCode  */
    FakeHwX86_DisableCorruptedCode,     /* Method: DisableCorruptedCode */
    FAKE_HW_INFO_SIZE,                  /* Data:   infoSize             */
};

