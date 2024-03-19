/*****************************************************************
/*  File:   kLog.h                                               *
/*  Desc:   Logger                                               *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#ifndef __KLOG_H__
#define __KLOG_H__

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

typedef void (*OutCallback)(char*);

const int c_LogBufSz        = 2048;
const int c_LocationMaxLen    = 512;
const int c_MaxTimeStrLen    = 64;

/*****************************************************************************/
/*    Enum: LogMsgStatus
/*****************************************************************************/
enum LogMsgStatus
{
    lmsUnknown        = 0,
    lmsError        = 1,
    lmsFatalError    = 2,
    lmsWarning        = 3,
    lmsMessage        = 4,
    lmsInfo            = 5
}; // enum LogMsgStatus

/*****************************************************************************/
/*    Class:    LogMsgDescr
/*****************************************************************************/
class LogMsgDescr
{
public:
    LogMsgDescr();

    void        setCurrentTime        ();
    void        setMsg                ( const char* _msg        );
    void        setLocation            ( const char* _location );
    void        setStatus            ( LogMsgStatus _status  );

    void        setFile                ( const char* _file, 
                                        bool parseFileName = false );
    void        setLine                ( int _line                );
    void        setTimeHMS            ( int _hour, int _minute, int _sec );

    void        FormatPrint            ( char* buf, int bufLen );
    void        FormatPrint            ( FILE* fp );
    void        Default                ();

    void        PrintStatus            ( char* buf );

protected:
    LogMsgStatus        status;

    char                msg[c_LogBufSz];

    bool                hasLocation;
    char                location[c_LocationMaxLen];
    
    //  location
    bool                hasFileLine;
    char                file[_MAX_PATH];
    int                    line;

    //  time
    bool                hasTime;
    int                    hour;
    int                    minute;
    int                    sec;
}; // class ErrorDescr

/*****************************************************************
/*  Class:  Logger                                                 *
/*  Desc:   Manages output to the log stream                     *
/*****************************************************************/
class Logger{
public:
    Logger();    
    ~Logger();

    void                SetPrintCallback( OutCallback callb );
    void                SetCurrentLocation( const char* location );

    const char*            Error    ( const char* format, ... );
    const char*            ErrorFL    ( const char* file, int line, const char* buffer );
    
    const char*            Warning    ( const char* format, ... );
    const char*            Info    ( const char* format, ... );
    const char*            Message    ( const char* format, ... );

    const char*            GetLogFilePath() const { return logFName; } 
    
private:
    FILE*                fLog;
    char                logFName[_MAX_PATH];

    int                    numWarnings;
    OutCallback            outCB;

    LogMsgDescr            curMessage;

    const char*            getCurTimeString();
    const char*            getCurDateTimeString();

    void                logMessage();

    static char            timeBuf[c_MaxTimeStrLen];    
}; // class Logger

const char* GlueString( const char* format, ... );
extern Logger Log;

#define LogError(A) {const char* message = Log.ErrorFL( __FILE__, __LINE__, A ); massert( false, message );}

#endif // __KLOG_H__