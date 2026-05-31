/*****************************************************************
/*  File:   kColorTraits.h                                            
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:                                      
/*****************************************************************/
#ifndef __KCOLORTRAITS_H__
#define __KCOLORTRAITS_H__

/*****************************************************************************/
/*    ColorRamp traits
/*****************************************************************************/
template <>
class TypeTraits<ColorRamp>
{
    typedef ColorRamp T;
public:
    static int            TypeID()    { return GetTypeID<ColorRamp>(); }
    static const char*    TypeName()    { return "ColorRamp"; }
    static bool            ToString    ( const T& val, char* str, int maxLen ) 
    { 
        char* cstr = str;
        int nKeys = val.GetNKeys();
        for (int i = 0; i < nKeys; i++)
        {
            sprintf( cstr, "{%f,%X}", val.GetKeyTime( i ), val.GetKey( i ) );
            cstr = str + strlen( str );
            if (i < nKeys - 1)
            {
                sprintf( cstr, "," );
                cstr = str + strlen( str );
            }
            if (strlen( cstr ) >= maxLen - 32) return false;
        }
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        val.ClearKeys();
        if (!str || str[0] == 0) return true;
        const char* cstr = str;
        while (*cstr != 0) 
        {
            float t;
            DWORD col;
            int nCh = strspn( cstr, "\t\n,{ " );
            if (nCh == 0) return false;
            cstr += nCh;
            if (sscanf( cstr, "%f,%X", &t, &col ) != 2) return false;
            val.AddKey( t, col );
            nCh = strcspn( cstr, "} " );
            if (nCh == 0) return false;
            cstr += nCh;
            nCh = strspn( cstr, "} " );
            if (nCh == 0) return false;
            cstr += nCh;
        }
        return true; 
    }

    static bool            NextValue    ( T& val ) { return false; }
    static bool            PrevValue    ( T& val ) { return false; }
}; // class TypeTraits<ColorRamp>

/*****************************************************************************/
/*    AlphaRamp traits
/*****************************************************************************/
template <>
class TypeTraits<AlphaRamp>
{
    typedef AlphaRamp T;
public:
    static int            TypeID()    { return GetTypeID<AlphaRamp>(); }
    static const char*    TypeName()    { return "AlphaRamp"; }
    static bool            ToString    ( const T& val, char* str, int maxLen ) 
    { 
        char* cstr = str;
        int nKeys = val.GetNKeys();
        for (int i = 0; i < nKeys; i++)
        {
            sprintf( cstr, "{%f,%f}", val.GetKeyTime( i ), val.GetKey( i ) );
            cstr = str + strlen( str );
            if (i < nKeys - 1)
            {
                sprintf( cstr, "," );
                cstr = str + strlen( str );
            }
            if (strlen( cstr ) >= maxLen - 32) return false;
        }
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        val.ClearKeys();
        if (!str || str[0] == 0) return true;
        const char* cstr = str;
        while (*cstr != 0) 
        {
            float t;
            float a;
            int nCh = strspn( cstr, "\t\n,{ " );
            if (nCh == 0) return false;
            cstr += nCh;
            if (sscanf( cstr, "%f,%f", &t, &a ) != 2) return false;
            val.AddKey( t, a );
            nCh = strcspn( cstr, "} " );
            if (nCh == 0) return false;
            cstr += nCh;
            nCh = strspn( cstr, "} " );
            if (nCh == 0) return false;
            cstr += nCh;
        }
        return true; 
    }

    static bool            NextValue    ( T& val ) { return false; }
    static bool            PrevValue    ( T& val ) { return false; }
}; // class TypeTraits<AlphaRamp>

#endif // __KCOLORTRAITS_H__