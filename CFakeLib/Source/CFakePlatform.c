#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "CFakePlatform.h"
#include "CFakeOs.h"
#include "CFakeHw.h"
#include "CFakeLog.h"

static void FakePlatform_RaiseFatal(char * format, ...)
{
    va_list args;

    va_start(args, format);
    gFakeLog.VCritical(format, args);
    va_end(args);

    exit(1);
}

static void FakePlatform_CheckInConfigType(SFakeConfigParam * configParamPtr)
{
    EFakeConfigType configType = configParamPtr->configType;

    if (configType <= EFakeConfigType_NULL || configType >= EFakeConfigType_Num)
    {
        gFakePlatform.RaiseFatal("Invalid configType(%d), not in [%d, %d]! "
                                 "sourceFile: %s, sourceLine: %d",
                                 configParamPtr->configType,
                                 EFakeConfigType_NULL + 1,
                                 EFakeConfigType_Num - 1,
                                 configParamPtr->sourceFile,
                                 configParamPtr->sourceLine);
        }
}

static void FakePlatform_CheckInFunc(SFakeConfigParam * configParamPtr)
{
    EFakeConfigType configType = configParamPtr->configType;

    if (configType == EFakeConfigType_On || configType == EFakeConfigType_Off)
    {
        if (configParamPtr->funcName == NULL)
        {
            configParamPtr->funcName = "N/A";
        }

        if (configParamPtr->funcAddr == NULL)
        {
            gFakePlatform.RaiseFatal("Invalid func, addr: 0x%X, name: %s! "
                                     "sourceFile: %s, sourceLine: %d",
                                     configParamPtr->funcAddr,
                                     configParamPtr->funcName,
                                     configParamPtr->sourceFile,
                                     configParamPtr->sourceLine);
        }
    }
}

static void FakePlatform_CheckInMock(SFakeConfigParam * configParamPtr)
{    
    EFakeConfigType configType = configParamPtr->configType;

    if (configType == EFakeConfigType_On)
    {
        if (configParamPtr->mockName == NULL)
        {
            configParamPtr->mockName = "N/A";
        }

        if (configParamPtr->mockAddr == NULL)
        {
            gFakePlatform.RaiseFatal("Invalid mock, addr: 0x%X, name: %s! "
                                     "sourceFile: %s, sourceLine: %d",
                                     configParamPtr->mockAddr,
                                     configParamPtr->mockName,
                                     configParamPtr->sourceFile,
                                     configParamPtr->sourceLine);
        }
    }

}

static void FakePlatform_CheckIn(SFakeConfigParam * configParamPtr)
{
    if (configParamPtr == NULL)
    {
        gFakePlatform.RaiseFatal("configParamPtr is NULL! "
                                 "sourceFile: %s, sourceLine: %d",
                                 configParamPtr->sourceFile,
                                 configParamPtr->sourceLine);
    }

    FakePlatform_CheckInConfigType(configParamPtr);
    FakePlatform_CheckInFunc(configParamPtr);
    FakePlatform_CheckInMock(configParamPtr);
}

static SFakeDataInfo * 
FakePlatform_EnableConfig(SFakeConfigParam * configParamPtr)
{
    assert (configParamPtr != NULL);
    {
        SFakeDataInfo * dataInfoPtr;
        TFakeU8       * osMsgPtr;
        TFakeU8       * hwMsgPtr;
        int             msgSize = gFakeOs.msgSize + gFakeHw.msgSize;

        gFakeLog.Debug("FakePlatform_EnableConfig");

        dataInfoPtr = gFakeDb.AllocDataInfo(msgSize);
        osMsgPtr    = dataInfoPtr->msg;
        hwMsgPtr    = dataInfoPtr->msg + gFakeOs.msgSize;

        gFakeOs.AllocMemoryProtect(configParamPtr, osMsgPtr);
        gFakeHw.MakeCode(configParamPtr, hwMsgPtr);
        gFakeHw.EnableCode(configParamPtr, hwMsgPtr);
        gFakeOs.FreeMemoryProtect(configParamPtr, osMsgPtr);

        return dataInfoPtr;
    }
}

static void FakePlatform_DisableConfig(SFakeConfigParam * configParamPtr,
                                      SFakeDataInfo ** dataInfoPtr)
{
    assert (configParamPtr != NULL);
    assert (dataInfoPtr    != NULL);
    assert (*dataInfoPtr   != NULL);
    {
        TFakeU8       * osMsgPtr = (*dataInfoPtr)->msg;
        TFakeU8       * hwMsgPtr = (*dataInfoPtr)->msg + gFakeOs.msgSize;

        gFakeLog.Debug("FakePlatform_DisableConfig");

        gFakeOs.AllocMemoryProtect(configParamPtr, osMsgPtr);
        gFakeHw.DisableCode(configParamPtr, hwMsgPtr);
        gFakeOs.FreeMemoryProtect(configParamPtr, osMsgPtr);
        gFakeDb.FreeDataInfo(dataInfoPtr);
    }
}

SFakePlatform gFakePlatform = {
    FakePlatform_RaiseFatal,
    FakePlatform_CheckIn,
    FakePlatform_EnableConfig,
    FakePlatform_DisableConfig,
};

