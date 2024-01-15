/*****************************************************************
/*  File:   StrUtil.h                                            *
/*  Desc:   String utilities                                     *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Feb 2002                                             *
/*****************************************************************/
#ifndef __KSTRUTIL_H__
#define __KSTRUTIL_H__
#pragma once

#include <string>
#include <vector>
#include "gmDefines.h"

using namespace std;

string      trimLR( const string& orig );
string      withoutPar( const string& orig );
bool        toFloat( const string& str, float& res );
bool        toInt( const string& str, int& res );

void        ParseExtension( const char* fname, char* ext );


struct STRecord{
    string            id;
    string            str;
};

class StringTable
{
public:
    StringTable(){}

    _inl void           clean       ();

    _inl int            getNRows    () const;
    _inl const string&  getRowID    ( int rowIdx ) const;
    _inl const string&  getRowStr   ( int rowIdx ) const;

    _inl void           addRow      ( const string& id, const string& str );
    _inl void           setRowID    ( int rowIdx, const string& id );
    _inl void           setRowStr   ( int rowIdx, const string& str );

private:
    vector<STRecord>    rows;
};


#ifdef _INLINES
#include "kStrUtil.inl"
#endif // _INLINES

#endif // __KSTRUTIL_H__