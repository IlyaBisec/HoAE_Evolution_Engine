/*****************************************************************/
/*  File:   rsSettings.h
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __RSSETTINGS_H__
#define __RSSETTINGS_H__

#include <stdio.h>

class IniFile;
/*****************************************************************/
/*  Class:    Settings
/*  Desc:    
/*****************************************************************/
class Settings
{
public:
    Settings();
    void FillFromIniFile( IniFile& inif );
    void initFromIniFile( const char* fname );
    void DumpToIniFile( FILE* fp );

    bool                    fullScreen;
    bool                    hardwareTnL;            
    bool                    pureDevice;                
    bool                    softwareRendering;        
    int                        adapterOrdinal;            
    int                        screenWidth;            
    int                        screenHeight;            
};

#endif // __RSSETTINGS_H__