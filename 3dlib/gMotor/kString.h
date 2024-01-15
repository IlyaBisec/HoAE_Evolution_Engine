/*****************************************************************************/
/*    File:    kString.h
/*    Desc:    Generic string routines
/*    Author:    Ruslan Shestopalyuk
/*    Date:    29.01.2003
/*****************************************************************************/
#ifndef __KSTRING_H__
#define __KSTRING_H__

BEGIN_NAMESPACE( c2 );
/*****************************************************************************/
/*    Class:    string
/*    Desc:    Generic string class
/*****************************************************************************/
class string
{
    char*                buf;
    int                    len;

public:
    _inl                string        ();
    _inl                string        ( const char* val        ); 

    //  copying
    _inl                string        ( string& orig            );
    _inl const string&    operator=    ( const string& str        );
    _inl const string&    operator=    ( const char* str        );

    _inl                ~string        ();
    _inl void            set            ( const char* val        );
    _inl void            set            ( const char* val, int nsym    );


    _inl void            operator +=    ( const char* catStr    );
    _inl char&            operator[]    ( int idx                );
    _inl void            resize        ( int newSz                );
    _inl operator        const char*    ()                        const { return buf; }
    _inl bool            operator ==    ( const string& str )    const;
    _inl bool            operator ==    ( const char* str    )    const;
    _inl int            size        ()                        const; 
    _inl const char*    c_str        ()                        const; 
    _inl unsigned int    hash        ()                        const;
    _inl void            copy        ( const string& str        );
    _inl bool            equal        ( const string& str        ) const;
    _inl int            read        ( BYTE* sbuf, int maxSize );
    _inl int            write        ( BYTE* sbuf, int maxSize ) const;
    _inl int            read        ( FILE* fp );
    _inl int            write        ( FILE* fp ) const;
    _inl void            toLower        ();
}; // class string

#ifdef _INLINES
#include "kString.inl"
#endif // _INLINES

END_NAMESPACE( c2 );

#endif // __KSTRING_H__