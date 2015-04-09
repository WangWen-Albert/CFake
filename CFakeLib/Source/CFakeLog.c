#include <stdarg.h>
#include <assert.h>
#include "CFakeLog.h"
#include "CFakePlatform.h"

#define FAKE_LOG_BUFFER 512

static EFakeLogLevel gFakeLogLevel  = EFakeLogLevel_Debug;
static EFakeLogMode  gFakeLogMode   = EFakeLogMode_Stdout;
static FILE *        gFakeLogStream = NULL;

static void FakeLog_OutputLogs(char * format, char * header, va_list args)
{
    /* print logs */
    if (gFakeLogMode == EFakeLogMode_Stdout)
    {
        printf("%s", header);
        vprintf(format, args);
        printf("\n");
    }
    else
    {
        vfprintf(gFakeLogStream, "%s", header);
        vfprintf(gFakeLogStream, format, args);
        fprintf(gFakeLogStream, "\n");
    }
}

static void FakeLog_PrintDebug(char * format, ...)
{
    assert (format != NULL);
    {
        va_list args;

        va_start(args, format);
        gFakeLog.VDebug(format, args);
        va_end(args);
    }
}

static void FakeLog_PrintInfo(char * format, ...)
{
    assert (format != NULL);
    {
        va_list args;

        va_start(args, format);
        gFakeLog.VInfo(format, args);
        va_end(args);
    }
}

static void FakeLog_PrintWarning(char * format, ...)
{
    assert (format != NULL);
    {
        va_list args;

        va_start(args, format);
        gFakeLog.VWarning(format, args);
        va_end(args);
    }
}

static void FakeLog_PrintError(char * format, ...)
{
    assert (format != NULL);
    {
        va_list args;

        va_start(args, format);
        gFakeLog.VError(format, args);
        va_end(args);
    }
}

static void FakeLog_PrintCritical(char * format, ...)
{
    assert (format != NULL);
    {
        va_list args;

        va_start(args, format);
        gFakeLog.VCritical(format, args);
        va_end(args);
    }
}

static void FakeLog_VPrintDebug(char * format, va_list args)
{
    assert (format != NULL);
    assert (args   != NULL);
    {
        if (gFakeLogLevel <= EFakeLogLevel_Debug)
        {
            FakeLog_OutputLogs(format, "CFake-DBG# ", args);
        }
    }
}

static void FakeLog_VPrintInfo(char * format, va_list args)
{
    assert (format != NULL);
    assert (args   != NULL);
    {
        if (gFakeLogLevel <= EFakeLogLevel_Info)
        {
            FakeLog_OutputLogs(format, "CFake-INF# ", args);
        }
    }
}

static void FakeLog_VPrintWarning(char * format, va_list args)
{
    assert (format != NULL);
    assert (args   != NULL);
    {
        if (gFakeLogLevel <= EFakeLogLevel_Warning)
        {
            FakeLog_OutputLogs(format, "CFake-WRN# ", args);
        }
    }
}

static void FakeLog_VPrintError(char * format, va_list args)
{
    assert (format != NULL);
    assert (args   != NULL);
    {
        if (gFakeLogLevel <= EFakeLogLevel_Error)
        {
            FakeLog_OutputLogs(format, "CFake-ERR# ", args);
        }
    }
}

static void FakeLog_VPrintCritical(char * format, va_list args)
{
    assert (format != NULL);
    assert (args   != NULL);
    {
        if (gFakeLogLevel <= EFakeLogLevel_Critical)
        {
            FakeLog_OutputLogs(format, "CFake-CRITICAL# ", args);
        }
    }
}

/*****************************************************
 @Description:
   Custom log output level, Default is EFakeLogLevel_Info
 @Parameter:
   logLevel: new log output level
 *****************************************************/
void FakeLog_SetLevel(EFakeLogLevel logLevel)
{
    if (logLevel < EFakeLogLevel_Debug || logLevel > EFakeLogLevel_Critical)
    {
        gFakePlatform.RaiseFatal("Invalid logLevel to set CFake, logLevel: %d",
                                  logLevel);
    }

    gFakeLogLevel = logLevel;
}

/*****************************************************
 @Description:
   Custom log output mode, Default is EFakeLogMode_Stdout
 @Parameter:
   logLevel: new log output level
   modeInfo: pointer to filename if using file for output
 *****************************************************/
void FakeLog_SetMode(EFakeLogMode logMode, void * modeInfo)
{
    char * fileMode;
    FILE * fileHandler;

    if (logMode < EFakeLogMode_Stdout || logMode > EFakeLogMode_AppendFile)
    {
        gFakePlatform.RaiseFatal("Invalid logMode to set CFake, logMode: %d",
                             logMode);
    }

    if (modeInfo == NULL)
    {
        gFakePlatform.RaiseFatal("Invalid modeInfo(NULL) to set CFake");
    }

    if (logMode == EFakeLogMode_Stdout)
    {
        gFakeLogMode = logMode;
    }
    else
    {
        if (logMode == EFakeLogMode_NewFile)
        {
            fileMode = "w";
        }
        else
        {
            fileMode = "w+";
        }

        fileHandler = fopen((char *)modeInfo, fileMode);
        if (fileHandler != NULL)
        {
            gFakePlatform.RaiseFatal("Cannot open log file for CFake,"
                                     " filename: %s",
                                     (char *)modeInfo);
        }

        if (gFakeLogStream != NULL)
        {
            fclose(gFakeLogStream);
        }

        gFakeLogStream = fileHandler;
    }
}

SFakeLog gFakeLog = {
    FakeLog_PrintDebug,
    FakeLog_PrintInfo,
    FakeLog_PrintWarning,
    FakeLog_PrintError,
    FakeLog_PrintCritical,
    FakeLog_VPrintDebug,
    FakeLog_VPrintInfo,
    FakeLog_VPrintWarning,
    FakeLog_VPrintError,
    FakeLog_VPrintCritical,
    FakeLog_SetLevel,
    FakeLog_SetMode,
};

