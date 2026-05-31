/*****************************************************************************/
/*    File:    kMathTypeTraits.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11.07.2003
/*****************************************************************************/
#ifndef __KMATHTYPETRAITS_H__
#define __KMATHTYPETRAITS_H__

/*****************************************************************************/
/*    Vector3D traits
/*****************************************************************************/
template <>
class TypeTraits<Vector3D>
{
    typedef Vector3D T;
public:
    static int            TypeID()    { return GetTypeID<Vector3D>(); }
    static const char*    TypeName()    { return "Vector3D"; }
    static bool            ToString    ( const T& val, char* str, int maxLen )
    { 
        sprintf( str, "%f %f %f", val.x, val.y, val.z );
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        return (sscanf( str, "%f%f%f", &val.x, &val.y, &val.z ) == 3);
    }

    static bool            NextValue    ( T& val ) { return false; }
    static bool            PrevValue    ( T& val ) { return false; }

}; // class TypeTraits<Vector3D>

/*****************************************************************************/
/*    Rct traits
/*****************************************************************************/
template <>
class TypeTraits<Rct>
{
    typedef Rct T;
public:
    static int            TypeID()    { return GetTypeID<Rct>(); }
    static const char*    TypeName()    { return "Rct"; }
    static bool            ToString    ( const T& val, char* str, int maxLen )
    { 
        sprintf( str, "%f %f %f %f", val.x, val.y, val.w, val.h );
        return true; 
    }
    static bool            FromString    ( T& val, const char* str )
    { 
        return (sscanf( str, "%f%f%f%f", &val.x, &val.y, &val.w, &val.h ) == 4);
    }

    static bool            NextValue    ( T& val ) { return false; }
    static bool            PrevValue    ( T& val ) { return false; }

}; // class TypeTraits<Rct>
#endif // __KMATHTYPETRAITS_H__