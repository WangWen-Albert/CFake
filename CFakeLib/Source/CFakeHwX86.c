#include <assert.h>
#include <string.h>
#include "CFakeHw.h"
#include "CFakeLog.h"

/* 0xAA, 'corrupted data', '0xAA', 'original data', '0xAA'  */
#define FAKE_HW_MSG_SIZE            (FAKE_CODE_SIZE_MAX * 2 + 3)
#define FAKE_HW_MSG_GUARDER_BYTE    0xAA

#define FAKE_WORD_SIZE_SYS32        4
#define FAKE_WORD_SIZE_SYS64        8

#define FAKE_CODE_SIZE_SYS32        5
#define FAKE_CODE_SIZE_SYS64        8
#define FAKE_CODE_SIZE_MAX          8

#define FAKE_CODE_INDEX_SYS32       1
#define FAKE_CODE_INDEX_SYS64       2

#define FAKE_HW_MSG_CORRUPTED_DATA_HEADER(msgPtr)   \
    ((TFakeU8 *)(msgPtr) + 1)

#define FAKE_HW_MSG_ORIGINAL_DATA_HEADER(msgPtr, codeSize)  \
        ((TFakeU8 *)(msgPtr) + codeSize + 2)

#define FAKE_HW_MSG_MOCK_DATA_HEADER(msgPtr, index)  \
    ((TFakeU8 *)(msgPtr) + 1 + index)

#define FAKE_IS_SYS32()  (sizeof(int) == FAKE_WORD_SIZE_SYS32)
#define FAKE_IS_SYS64()  (sizeof(int) == FAKE_WORD_SIZE_SYS64)

/******************************************************************************
 * SYS32 : __asm("jmp  $mock")                  : 0xE9 {$mock - $func - 5}    *
 * SYS32 : __asm("movl $mock, %eax; jmpl %eax") : 0xB8 {$mock} 0xFF 0xE0      *
 * SYS64 : __asm("movq $mock, %rax; jmpq %rax") : 0x48 0xB8 {$mock} 0xFF 0xE0 * 
 ******************************************************************************/
TFakeU8 gFakeCodeSys32[FAKE_CODE_SIZE_SYS32] = {
    0xE9, 0x0, 0x0, 0x0, 0x0,
};
TFakeU8 gFakeCodeSys64[FAKE_CODE_SIZE_SYS64] = {
    0x48, 0xB8, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xE0,
};

static void
FakeHw86_SetTemplate(TFakeUInt codeSize, TFakeU8 * codePtr, TFakeU8 * msgPtr)
{
    *msgPtr = FAKE_HW_MSG_GUARDER_BYTE;

    *FAKE_HW_MSG_CORRUPTED_DATA_HEADER(msgPtr) = FAKE_HW_MSG_GUARDER_BYTE;

    *FAKE_HW_MSG_ORIGINAL_DATA_HEADER(msgPtr,
                                      codeSize) = FAKE_HW_MSG_GUARDER_BYTE;

    (void)memcpy(FAKE_HW_MSG_CORRUPTED_DATA_HEADER(msgPtr), codePtr, codeSize);
}

static TFakeUInt FakeHwX86_GetCodeSize(void)
{
    TFakeUInt codeSize;

    if (FAKE_IS_SYS32())
    {
        codeSize = FAKE_CODE_SIZE_SYS32;
    }
    else if (FAKE_IS_SYS64())
    {
        codeSize =  FAKE_CODE_SIZE_SYS64;
    }

    return codeSize;
}

static void FakeHwX86_MakeCode(SFakeConfigParam * config, TFakeU8 * msgPtr)
{
    TFakeUInt codeSize;
    TFakeU8 * codePtr;
    TFakeUInt addrIndex;
    TFakeUInt addrInfo;

    if (FAKE_IS_SYS32())
    {
        codeSize  = FAKE_CODE_SIZE_SYS32;
        codePtr   = gFakeCodeSys32;
        addrIndex = FAKE_CODE_INDEX_SYS32;
        addrInfo  = (TFakeUInt)(config->mockAddr - config->funcAddr - codeSize);
    }
    else if (FAKE_IS_SYS64())
    {
        codeSize  = FAKE_CODE_SIZE_SYS64;
        codePtr   = gFakeCodeSys64;
        addrIndex = FAKE_CODE_INDEX_SYS64;
        addrInfo  = (TFakeUInt)config->mockAddr;
    }

    gFakeLog.Debug("FakeHwX86_MakeCode, codeSize: %d", codeSize);

    FakeHw86_SetTemplate(codeSize, codePtr, msgPtr);
    
    (void)memcpy(FAKE_HW_MSG_MOCK_DATA_HEADER(msgPtr, addrIndex),
                (&addrInfo),
                sizeof(void*));
}

static void
FakeHwX86_EnableCode(SFakeConfigParam * configParamPtr, TFakeU8 * msgPtr)
{
    TFakeUInt codeSize;

    gFakeLog.Debug("FakeHwX86_EnableCode");

    if (FAKE_IS_SYS32())
    {
        codeSize = FAKE_CODE_SIZE_SYS32;
    }
    else if (FAKE_IS_SYS64())
    {
        codeSize = FAKE_CODE_SIZE_SYS64;
    }

    (void)memcpy(FAKE_HW_MSG_ORIGINAL_DATA_HEADER(msgPtr, codeSize),
                 configParamPtr->funcAddr,
                 codeSize);
    (void)memcpy(configParamPtr->funcAddr,
                FAKE_HW_MSG_CORRUPTED_DATA_HEADER(msgPtr),
                codeSize);
}

static void FakeHwX86_DisableCode(SFakeConfigParam * configParamPtr, TFakeU8 * msgPtr)
{
    TFakeUInt codeSize;

    gFakeLog.Debug("FakeHwX86_DisableCode");

    if (FAKE_IS_SYS32())
    {
        codeSize = FAKE_CODE_SIZE_SYS32;
    }
    else if (FAKE_IS_SYS64())
    {
        codeSize = FAKE_CODE_SIZE_SYS64;
    }

    (void)memcpy(configParamPtr->funcAddr,
                FAKE_HW_MSG_ORIGINAL_DATA_HEADER(msgPtr, codeSize),
                codeSize);
}

SFakeHw gFakeHw = {
    FakeHwX86_GetCodeSize,
    FakeHwX86_MakeCode,
    FakeHwX86_EnableCode,
    FakeHwX86_DisableCode,
    FAKE_HW_MSG_SIZE,
};

