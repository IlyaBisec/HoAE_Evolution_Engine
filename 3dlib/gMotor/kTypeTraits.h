/*****************************************************************************/
/*    File:    kTypeTraits.h
/*    Desc:    Generic extandable types descriptors
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-13-2003
/*****************************************************************************/
#ifndef __KTYPETRAITS_H__
#define __KTYPETRAITS_H__

/*****************************************************************************/
/*    Enum:    StdType
/*    Desc:    Enmeration of the "standard" types  
/*****************************************************************************/
enum StdType
{
    stUnknown    = 0,
    stMethod    = 1,
    stSignal    = 2,
    stInt        = 3,
    stFloat        = 4,
    stDWORD        = 5,
    stWORD        = 6,
    stCString    = 7,
    stBool        = 8,
    stBYTE        = 9,
    stDouble    = 10,

    stNumStdTypes    //  MUST be last
}; // enum StdType

extern int g_NumTypes;
template <class T>
int GetTypeID()
{
    static int s_TypeID = g_NumTypes++;
    return s_TypeID;
}

/*****************************************************************************/
/*    Class:    TypeTraits
/*    Desc:    Default type treatment traits  
/*****************************************************************************/
template <class T>
class TypeTraits
{
    static int            s_CurID;
public:
    static int            TypeID        ()    { return GetTypeID<T>(); }
    static const char*    TypeName    ()    { return ""; }
    static bool            ToString    ( const T& val, char* str, int maxLen ) { return false; }
    static bool            FromString    ( T& val, const char* str )                { return false; }
    
    //  iterating through integral types' values
    static bool            NextValue    ( T& val ) { return false; }
    static bool            PrevValue    ( T& val ) { return false; }
}; // class TypeTraits

template <class T> int TypeTraits<T>::s_CurID = stNumStdTypes;

/*****************************************************************************/
/*    int traits
/*****************************************************************************/
template <>
class TypeTraits<int>
{
    typedef int T;
public:
    static int            TypeID()    { return stInt; }
    static const char*    TypeName()    { return "int"; }
    static bool            ToString    ( const T& val, char* str, int maxLen )
    { 
        sprintf( str, "%d", val );
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        if (!str || str[0] == 0)
        {
            val = 0;
            return true;
        }
        return sscanf( str, "%d", &val ) == 1; 
    }

    static bool            NextValue    ( T& val ) { val++; return true; }
    static bool            PrevValue    ( T& val ) { val--; return true; }

}; // class TypeTraits<int>

/*****************************************************************************/
/*    DWORD traits
/*****************************************************************************/
template <>
class TypeTraits<DWORD>
{
    typedef DWORD T;
public:
    static int            TypeID()    { return stDWORD; }
    static const char*    TypeName()    { return "DWORD"; }
    static bool            ToString    ( const T& val, char* str, int maxLen ) 
    { 
        sprintf( str, "%X", val );
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    {  
        if (!str || str[0] == 0)
        {
            val = 0;
            return true;
        }
        return sscanf( str, "%X", &val ) == 1; 
    }

    static bool            NextValue    ( T& val ) { val++; return true; }
    static bool            PrevValue    ( T& val ) { val--; return true; }
}; // class TypeTraits<DWORD>

/*****************************************************************************/
/*    WORD traits
/*****************************************************************************/
template <>
class TypeTraits<WORD>
{
    typedef WORD T;
public:
    static int            TypeID()    { return stWORD; }
    static const char*    TypeName()    { return "WORD"; }
    static bool            ToString    ( const T& val, char* str, int maxLen )
    { 
        sprintf( str, "%hX", val );
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        return sscanf( str, "%hX", &val ) == 1; 
    }

    static bool            NextValue    ( T& val ) { val++; return true; }
    static bool            PrevValue    ( T& val ) { val--; return true; }

}; // class TypeTraits<WORD>

/*****************************************************************************/
/*    BYTE traits
/*****************************************************************************/
template <>
class TypeTraits<BYTE>
{
    typedef BYTE T;
public:
    static int            TypeID()    { return stBYTE; }
    static const char*    TypeName()    { return "BYTE"; }
    static bool            ToString    ( const T& val, char* str, int maxLen )
    { 
        sprintf( str, "%hhX", val );
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        return sscanf( str, "%hhX", &val ) == 1; 
    }

    static bool            NextValue    ( T& val ) { val++; return true; }
    static bool            PrevValue    ( T& val ) { val--; return true; }

}; // class TypeTraits<BYTE>

/*****************************************************************************/
/*    float traits
/*****************************************************************************/
template <>
class TypeTraits<float>
{
    typedef float T;
public:
    static int            TypeID()    { return stFloat; }
    static const char*    TypeName()    { return "float"; }
    static bool            ToString    ( const T& val, char* str, int maxLen ) 
    { 
        sprintf( str, "%f", val );
        //  truncate trailing 0s
        if (strstr( str, "." ))
        {
            int pos = strlen( str ) - 1;
            while (pos > 0)
            {
                if (str[pos] == '0') pos--; else break;
            }
            if (str[pos] == '.') pos--;
            str[pos + 1] = 0;
        }
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        if (!str || str[0] == 0)
        {
            val = 0.0f;
            return true;
        }
        T cval;
        int nS = sscanf( str, "%f", &cval );
        if (nS == 1) 
        {
            val = cval;
            return true;
        }
        return false;
    }

    static bool            NextValue    ( T& val ) { val += 1.0f; return true; }
    static bool            PrevValue    ( T& val ) { val -= 1.0f; return true; }
}; // class TypeTraits<float>

/*****************************************************************************/
/*    double traits
/*****************************************************************************/
template <>
class TypeTraits<double>
{
    typedef double T;
public:
    static int            TypeID()    { return stDouble; }
    static const char*    TypeName()    { return "double"; }
    static bool            ToString    ( const T& val, char* str, int maxLen ) 
    { 
        sprintf( str, "%.10f", val );
        //  truncate trailing 0s
        if (strstr( str, "." ))
        {
            int pos = strlen( str ) - 1;
            while (pos > 0)
            {
                if (str[pos] == '0') pos--; else break;
            }
            if (str[pos] == '.') pos--;
            str[pos + 1] = 0;
        }
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        if (!str || str[0] == 0)
        {
            val = 0.0;
            return true;
        }
        T cval;
        int nS = sscanf( str, "%lf", &cval );
        if (nS == 1) 
        {
            val = cval;
            return true;
        }
        return sscanf( str, "%lf", &val ) == 1; 
    }

    static bool            NextValue    ( T& val ) { val += 1.0; return true; }
    static bool            PrevValue    ( T& val ) { val -= 1.0; return true; }
}; // class TypeTraits<double>

/*****************************************************************************/
/*    const char* traits
/*****************************************************************************/
template <>
class TypeTraits<const char*>
{
    typedef const char* T;
public:
    static int            TypeID()    { return stCString; }
    static const char*    TypeName()    { return "cstring"; }
    static bool            ToString    ( const T& val, char* str, int maxLen ) 
    { 
        if (!val) return false;
        int len = strlen( val );
        if( len >= maxLen ) return false;
        strcpy( str, val );
        return true;
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        val = str;
        return true; 
    }
    
    static bool            NextValue    ( T& val ) { return false; }
    static bool            PrevValue    ( T& val ) { return false; }
}; // class TypeTraits<const char*>

/*****************************************************************************/
/*    std::string traits
/*****************************************************************************/
template <>
class TypeTraits<std::string>
{
    typedef std::string T;
public:
    static int            TypeID()    { return GetTypeID<T>(); }
    static const char*    TypeName()    { return "std::string"; }
    static bool            ToString    ( const T& val, char* str, int maxLen ) 
    { 
        int len = val.size();
        if( len >= maxLen ) return false;
        strcpy( str, val.c_str() );
        return true;
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        val = str;
        return true; 
    }
    
    static bool            NextValue    ( T& val ) { return false; }
    static bool            PrevValue    ( T& val ) { return false; }

}; // class TypeTraits<std::string>

/*****************************************************************************/
/*    bool traits
/*****************************************************************************/
template <>
class TypeTraits<bool>
{
    typedef bool T;
public:
    static int            TypeID()    { return stBool; }
    static const char*    TypeName()    { return "bool"; }
    static bool            ToString    ( const T& val, char* str, int maxLen ) 
    { 
        if (val) strcpy( str, "true" ); else strcpy( str, "false" );
        return true;
    }
    static bool            FromString    ( T& val, const char* str )                
    { 
        if (!stricmp( str, "true" )) { val = true; return true; }
        if (!stricmp( str, "false" )) { val = false; return true; }
        return false; 
    }
    
    static bool            NextValue    ( T& val ) { val = !val; return true; }
    static bool            PrevValue    ( T& val ) { val = !val; return true; }

}; // class TypeTraits<bool>

const int c_TmpBufSize = 1024*1024;
template <class T> const char* ToString( const T& val )
{
    static char buf[c_TmpBufSize];
    TypeTraits<T>::ToString( val, buf, c_TmpBufSize );
    return buf;
} // ToString

#define ENUM(A,B,C)    template <> class TypeTraits<A> {public:\
                    static int            TypeID()    { return ::GetTypeID<A>(); }\
                    static const char*    TypeName()    { return "enum_"##B; }\
                    static bool            ToString    ( const A& val, char* str, int maxLen ) \
                    { static en_val ev = C; return ev.ToString( (int)val, str, maxLen ); }\
                    static bool            FromString    ( A& val, const char* str )\
                    { static en_val ev = C; int nval = 0; \
                    bool res = ev.FromString( nval, str ); if (res) val = A(nval); return res; }\
                    static bool            NextValue    ( A& val ) { static en_val ev = C; \
                        return ev.NextValue( val ); }\
                    static bool            PrevValue    ( A& val ) { static en_val ev = C; \
                        return ev.PrevValue( val ); } };\

const int c_MaxEnumValues = 64;
/*****************************************************************************/
/*    Class:    en_val
/*    Desc:    Enumerator value placeholder
/*****************************************************************************/
class en_val
{
    int                m_Vals    [c_MaxEnumValues];
    const char*        m_Names    [c_MaxEnumValues];
    int                m_NVal;

public:
    template <class T>
        en_val( T eval, const char* ename ) { m_Vals[0] = (int)eval; m_Names[0] = ename; m_NVal = 1; }

        en_val& operator << ( const en_val& ev ) 
        { 
            assert( m_NVal < c_MaxEnumValues );
            m_Vals [m_NVal] = ev.m_Vals [0];
            m_Names[m_NVal] = ev.m_Names[0];
            m_NVal++;
            return *this; 
        }
        
        template <class T> bool NextValue( T& val ) 
        { 
            int idx = GetValIdx( val );
            if (idx < 0 || idx >= m_NVal - 1) return false;
            val = (T)m_Vals[idx + 1];
            return true; 
        }

        template <class T> bool PrevValue( T& val ) 
        { 
            int idx = GetValIdx( val );
            if (idx <= 0 || idx >= m_NVal) return false;
            val = (T)m_Vals[idx - 1];
            return true;                
        }
        
        template <class T>
        int GetValIdx( T val )
        {
            for (int i = 0; i < m_NVal; i++)
            {
                if (m_Vals[i] == (int)val) return i;
            }
            return -1;
        }

        int    GetNVal() const { return m_NVal; }
        const char* GetValName( int idx ) const 
        { 
            if (idx < 0 || idx >= m_NVal) return "";
            return m_Names[idx];
        }

        int    GetValue( int idx )
        {
            //  not sure what to return here
            if (idx < 0 || idx >= m_NVal) return 0;
            return m_Vals[idx];
        }

        bool ToString( int val, char* str, int maxLen )
        {    
            for (int i = 0; i < m_NVal; i++)
            {
                if (m_Vals[i] == val)
                {
                    if (maxLen < strlen( m_Names[i] )) return false;
                    strcpy( str, m_Names[i] );
                    return true;
                }
            }
            return false;
        } // ToString

        bool FromString( int& val, const char* str )
        {
            for (int i = 0; i < m_NVal; i++)
            {
                if (!stricmp( str, m_Names[i] ))
                {
                    val = m_Vals[i];
                    return true;
                }
            }
            //  fallback to integer value
            if (sscanf( str, "%d", &val ) == 1) return true;
            return false;
        } // FromString

}; // class en_val

#endif // __KTYPETRAITS_H__
