/*****************************************************************/
/*  File:   rsSettings.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "rsSettings.h"
/*****************************************************************
/*    Settings implementation
/*****************************************************************/
const char c_TrueStr[]    = "True";
const char c_FalseStr[] = "False";
const char* BoolStr( bool val )
{
    return val ? c_TrueStr : c_FalseStr;
}

Settings::Settings()
{
    fullScreen                = false;
    hardwareTnL                = true;
    pureDevice                = false;
    softwareRendering        = false;
    adapterOrdinal            = 0;
    screenWidth                = 1024;
    screenHeight            = 768;
}

void Settings::FillFromIniFile( IniFile& inif )
{
    /*
    fullScreen                = inif.GetValBool    (    "FullScreen", "Video"            );
    hardwareTnL                = inif.GetValBool    (    "HardwareTnL", "Video"            );
    pureDevice                = inif.GetValBool    (    "PureDevice", "Video"            );
    softwareRendering        = inif.GetValBool    (    "SoftwareRendering", "Video"    );
    adapterOrdinal            = inif.GetValInt    (    "m_Ordinal", "Video"        );
    screenWidth                = inif.GetValInt    (    "ScreenWidth", "Video"            );
    screenHeight            = inif.GetValInt    (    "ScreenHeigth", "Video"            );
    */
}

void Settings::initFromIniFile( const char* fname )
{
    /*
    IniFile inif( fname );
    if (inif.Error()) return;
    FillFromIniFile( inif );
    */
}

void Settings::DumpToIniFile( FILE* fp )
{
    fprintf( fp, ";autocreated cll configuration file\n\n"    );
    fprintf( fp, "[Video]\n\n"                            );
    fprintf( fp, "FullScreen\t\t\t\t\t\t= %s\n",        BoolStr( fullScreen )            );
    fprintf( fp, "HardwareTnL\t\t\t\t\t\t= %s\n",        BoolStr( hardwareTnL )            );
    fprintf( fp, "PureDevice\t\t\t\t\t\t= %s\n",        BoolStr( pureDevice )            );
    fprintf( fp, "SoftwareRendering\t\t\t\t= %s\n",        BoolStr( softwareRendering )    );
    fprintf( fp, "m_Ordinal\t\t\t\t\t= %d\n",        adapterOrdinal                    );
    fprintf( fp, "ScreenWidth\t\t\t\t\t\t= %d\n",        screenWidth                        );
    fprintf( fp, "ScreenHeigth\t\t\t\t\t= %d\n",        screenHeight                    );
}

