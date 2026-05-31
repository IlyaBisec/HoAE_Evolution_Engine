/*****************************************************************
/*  File:   kStrUtil.cpp                                         *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Feb 2002                                             *
/*****************************************************************/
#include "stdafx.h"
#include "kStrUtil.h"

#ifndef _INLINES
#include "kStrUtil.inl"
#endif // !_INLINES

using namespace std;

string trimLR( const string& orig )
{

    int l = 0, r = orig.size() - 1;
    if (r == -1) return orig;
    while (orig[l] == ' ' || orig[l] == '\t') l++; 
    while (orig[r] == ' ' || orig[r] == '\t' || orig[r] == '\n'
            || orig[r] == '\r') r--;
    if (l >= r) return "";
    return string( orig, l, r - l + 1 );
}

string withoutPar( const string& orig )
{
    return string( orig, 1, orig.size() - 2 );
}

bool toFloat( const string& str, float& res )
{
    res = atof( str.c_str() );
    return true;
}

bool toInt( const string& str, int& res )
{
    res = atoi( str.c_str() );
    return true;
}

char tmpBuf[512];
const char* BinToStr( unsigned char num )
{
    for (int i = 0; i < 8; i++)
    {
        if (num & 1) tmpBuf[i] = '1';
            else tmpBuf[i] = '0';
        num >>= 1;
    }
    tmpBuf[8] = 0;
    return tmpBuf;
}

void ParseExtension( const char* fname, char* ext )
{
    int len = strlen( fname );
    const char* pos = &fname[len - 1];
    while (pos > fname && *pos != '.') pos--;
    if (pos == fname && *pos != '.') { *ext = 0; return; }
    strcpy( ext, pos + 1 );
}

/*---------------------------------------------------------------*
/*  Func:    ParseFileName
/*    Desc:    returns file name from full file path
/*    Parm:    origName - full path
/*    Ret:    file name, without extension
/*---------------------------------------------------------------*/
string ParseFileName( const string& origName )
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath( origName.c_str(), drive, dir, fname, ext );
    return string( fname );
    
}//  ParseFileName

/*---------------------------------------------------------------*
/*  Func:    FindFilesInDirectory
/*    Desc:    
/*    Parm:    sTbl - string table to fill with ID/path
/*            path - directory to search in
/*            filter - extension to search for
/*    Ret:    true if OK
/*---------------------------------------------------------------*/
bool FindFilesInDirectory( StringTable& sTbl,
                            const string& path, 
                            const char* filter )
{
    WIN32_FIND_DATA fData;
    char fn[_MAX_PATH];
    strcpy( fn, path.c_str() );
    strcat( fn, filter );

    HANDLE h = FindFirstFile( fn, &fData );
    if (h == INVALID_HANDLE_VALUE) return false;
    
    string fpath = path;
    fpath += string( fData.cFileName );

    sTbl.addRow( ParseFileName( fpath ), fpath );
    while (FindNextFile( h, &fData ))
    {
        fpath = path;
        fpath += string( fData.cFileName );
        sTbl.addRow( ParseFileName( fpath ), fpath );
    }
    FindClose( h );
    return true;    
}//  FindFilesInDirectory

