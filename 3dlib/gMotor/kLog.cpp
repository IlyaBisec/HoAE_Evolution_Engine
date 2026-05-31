/*****************************************************************
/*  File:   kLog.cpp                                             *
/*  Desc:   Logger implementation                                *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#include "stdafx.h"
#include "kLog.h"
#include "time.h" 
#include "direct.h"

Logger Log; // logger instance

void strcpy_safe( char* dest, const char* src, int bufSz )
{
    int len = strlen( src );
    if (len > bufSz - 1)
    {
        strncpy( dest, src, len );
        dest[bufSz - 1] = 0;
    }
    else
    {
        strcpy( dest, src ); 
    }
} // strcpy_safe

/*****************************************************************************/
/*    LogMsgDescr implementation
/*****************************************************************************/
LogMsgDescr::LogMsgDescr()
{
    Default();
}

void LogMsgDescr::Default()
{
    setCurrentTime    ();
    setMsg            ( "" );
    setLocation        ( "" );
    setFile            ( "" );
    setLine            ( 0 );
    setStatus        ( lmsMessage );

    hasFileLine = false;
    hasLocation = false;
    hasTime        = false;
} // LogMsgDescr::Default

void LogMsgDescr::setCurrentTime()
{
    time_t long_time;
    time( &long_time );
    tm*    newtime = localtime( &long_time );
    hasTime = true;

    hour    = newtime->tm_hour; 
    minute    = newtime->tm_min;
    sec        = newtime->tm_sec;
} // LogMsgDescr::setCurrentTime

void LogMsgDescr::setMsg( const char* _msg )
{
    strcpy_safe( msg, _msg, c_LogBufSz );
} // LogMsgDescr::setMsg

void LogMsgDescr::setLocation( const char* _location )
{
    strcpy_safe( location, _location, c_LocationMaxLen );
    hasLocation = true;
} // LogMsgDescr::setLocation

void LogMsgDescr::setStatus( LogMsgStatus _status  )
{
    status = _status;
} // LogMsgDescr::setStatus

void LogMsgDescr::setFile( const char* _file, bool parseFileName )
{
    if (parseFileName)
    {
        // parse source file name
        char p_drive    [_MAX_DRIVE    ];
        char p_dir        [_MAX_DIR    ];
        char p_file        [_MAX_FNAME    ];
        char p_ext        [_MAX_EXT    ];
        
        _splitpath( _file, p_drive, p_dir, p_file, p_ext );
        sprintf( file, "%s%s", p_file, p_ext );
    }
    else
    {
        strcpy_safe( file, _file, _MAX_PATH );
    }
    hasFileLine = true;
} // LogMsgDescr::setFile

void LogMsgDescr::setLine( int _line )
{
    line = _line;
    hasFileLine = true;
} // LogMsgDescr::setLine

void LogMsgDescr::setTimeHMS( int _hour, int _minute, int _sec )
{
    hour    = _hour;
    minute    = _minute;
    sec        = _sec;
    hasTime = true;
} // LogMsgDescr::setTimeHMS

void LogMsgDescr::FormatPrint( char* buf, int bufLen )
{
    buf[0] = 0;
    char* cbuf = buf;
    if (hasTime) sprintf( cbuf, "%02d:%02d:%02d | ", hour, minute, sec );
    cbuf = buf + strlen( buf );
    if (status != lmsMessage)
    {
        PrintStatus( cbuf );
        cbuf = buf + strlen( buf );
        sprintf( cbuf, " | " );
        cbuf = buf + strlen( buf );
    }
    if (hasLocation)
    {
        sprintf( cbuf, "%s | ", location );
        cbuf = buf + strlen( buf );
    }
    if (hasFileLine)
    {
        sprintf( cbuf, "File: %s Line:%5d | ", file, line );
        cbuf = buf + strlen( buf );
    }
    char tmp[256];
    memset(tmp,0,sizeof tmp);
    strncpy(tmp,msg,255);
    sprintf( cbuf, "%s", tmp );
} // LogMsgDescr::FormatPrint

void LogMsgDescr::PrintStatus( char* buf )
{
    switch (status)
    {
        case lmsUnknown:    sprintf( buf, "%-8s", "Unknown"    ); break;
        case lmsError:        sprintf( buf, "%-8s", "Error"    ); break;
        case lmsFatalError: sprintf( buf, "%-8s", "Fatal"    ); break;
        case lmsWarning:    sprintf( buf, "%-8s", "Warning"    ); break;
        case lmsMessage:    sprintf( buf, "%-8s", "Message"    ); break;
        case lmsInfo:        sprintf( buf, "%-8s", "Info"    ); break;
    }
} // LogMsgDescr::PrintStatus

void LogMsgDescr::FormatPrint( FILE* fp )
{
    if (!fp) return;
    char buf[1024];
    FormatPrint( buf, 1024 );
    fprintf( fp, "%s\n", buf );
} // LogMsgDescr::FormatPrint

char g_LogBuffer[c_LogBufSz];
const char* GlueString( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    vsprintf( g_LogBuffer, format, argList );
    va_end( argList );
    return g_LogBuffer;
}

/*****************************************************************
/*  Logger implementation                                        *
/*****************************************************************/
char Logger::timeBuf[c_MaxTimeStrLen];

Logger::Logger()
{    
    outCB        = 0;
    numWarnings = 0;
    
    getcwd( logFName, _MAX_PATH );
    strcat( logFName, "\\log.txt" );

    fLog        = fopen( logFName, "wt" );
    if (fLog) fclose( fLog );

    Message( "**********************************************************************************" );
    Message( "*    Logger session start: %s", getCurDateTimeString() );
    Message( "**********************************************************************************" );
    Message( "" );

} // Logger::Logger

Logger::~Logger()
{    
    Message( "" );
    Message( "**********************************************************************************" );
    Message( "*    Logger session end: %s", getCurDateTimeString() );
    Message( "**********************************************************************************" );
}

void Logger::SetPrintCallback( OutCallback callb )
{
    outCB = callb;
}

void Logger::logMessage()
{
    fLog = fopen( logFName, "at+" );
    if (!fLog) return;
    curMessage.FormatPrint( fLog );
    fclose( fLog );
    fLog = 0;
}

const char* Logger::Message( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    vsprintf( g_LogBuffer, format, argList );
    va_end( argList );
    
    curMessage.Default();
    curMessage.setMsg( g_LogBuffer );
    
    logMessage();

    return g_LogBuffer;
}//  Logger::Message

const char* Logger::Error( const char* format, ... )
{

    va_list argList;
    va_start( argList, format );
    vsprintf( g_LogBuffer, format, argList );
	//
	//Warning(format,argList);
	//
    va_end( argList );
	//
	//return g_LogBuffer;
	//

    curMessage.Default();
    curMessage.setStatus( lmsError );
    curMessage.setMsg( g_LogBuffer );
    curMessage.setCurrentTime();

    logMessage();

    //  show the little sweet window to unhappy programmer
    AssertionFailure assf( g_LogBuffer, g_LogBuffer, __FILE__, __LINE__ );
    static bool __ignore_always = false;
    FDResult result = AssertMgr::Failure( assf, __ignore_always );
    if (result == fdrAbort)
    {
        exit( 1 );
    }
    else if (result == fdrDebug)
    {
        __asm int 3;
    }

    return g_LogBuffer;
}//  Logger::Error

const char* Logger::ErrorFL( const char* file, int line, const char* g_LogBuffer )
{
    curMessage.Default();
    curMessage.setStatus    ( lmsError );
    curMessage.setMsg        ( g_LogBuffer );
    curMessage.setFile        ( file, true );
    curMessage.setLine        ( line );
    curMessage.setCurrentTime();

    logMessage();    
    return g_LogBuffer;
}//  Logger::Error

const char* Logger::Warning( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    vsprintf( g_LogBuffer, format, argList );
    va_end( argList );
    
    curMessage.Default();
    curMessage.setStatus( lmsWarning );
    curMessage.setMsg( g_LogBuffer );
    curMessage.setCurrentTime();

    logMessage();

    numWarnings++;
    return g_LogBuffer;
}//  Logger::Warning

const char* Logger::Info( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    vsprintf( g_LogBuffer, format, argList );
    va_end( argList );
    
    curMessage.Default();
    curMessage.setStatus( lmsInfo );
    curMessage.setMsg( g_LogBuffer );
    curMessage.setCurrentTime();

    logMessage();
    return g_LogBuffer;
}//  Logger::Info

const char*    Logger::getCurTimeString()
{
    time_t long_time;
    time( &long_time );
    tm*    newtime = localtime( &long_time );
    sprintf( timeBuf, "%2d:%2d:%2d", 
                newtime->tm_hour, 
                newtime->tm_min, 
                newtime->tm_sec );
    return timeBuf;
} // Logger::getCurTimeString

const char*    Logger::getCurDateTimeString()
{
    time_t long_time;
    time( &long_time );
    tm*    newtime = localtime( &long_time );

    sprintf( timeBuf, "%.19s", asctime( newtime ) );
    return timeBuf;
} // Logger::getCurDateTimeString