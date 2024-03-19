/*****************************************************************************/
/*    File:    kIOHelpers.h
/*    Desc:    Auxiliary input-output routines    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    20.02.2003
/*****************************************************************************/
#ifndef __KIOHELPERS_H__
#define __KIOHELPERS_H__

//  from math library
_inl OutStream&    operator<<( OutStream& os, const Matrix4D& m );
_inl InStream&    operator>>( InStream& is, Matrix4D& m );

_inl OutStream&    operator<<( OutStream& os, const Matrix3D& m );
_inl InStream&    operator>>( InStream& is, Matrix3D& m );

_inl OutStream&    operator<<( OutStream& os, const Matrix2Df& m );
_inl InStream&    operator>>( InStream& is, Matrix2Df& m );

_inl OutStream&    operator<<( OutStream& os, const Rct& r );
_inl InStream&    operator>>( InStream& is, Rct& r );

_inl OutStream&    operator<<( OutStream& os, const Vector3D& v );
_inl InStream&    operator>>( InStream& is, Vector3D& v );

_inl OutStream&    operator<<( OutStream& os, const Vector4D& v );
_inl InStream&    operator>>( InStream& is, Vector4D& v );

_inl OutStream&    operator<<( OutStream& os, const Quaternion& val );
_inl InStream& operator>>( InStream& is, Quaternion& val );

_inl OutStream&    operator<<( OutStream& os, const AABoundBox& val );
_inl InStream& operator>>( InStream& is, AABoundBox& val );

_inl OutStream&    operator<<( OutStream& os, const Sphere& val );
_inl InStream& operator>>( InStream& is, Sphere& val );

_inl OutStream&    operator<<( OutStream& os, const Plane& val );
_inl InStream& operator>>( InStream& is, Plane& val );

inline OutStream& operator<<( OutStream& os, const std::string& s )
{
    os << s.c_str();
    return os;
}

inline InStream& operator>>( InStream& is, std::string& s )
{
    DWORD nCh;
    is >> nCh;
    if (nCh == 0) { s = ""; return is; }
    s.resize( nCh + 1 );
    char* buf = (char*)s.c_str();
    is.Read( (void*)buf, nCh );
    buf[nCh] = 0;
    return is;
}

OutStream&    operator<<( OutStream& os, const BaseMesh& s );
InStream&    operator>>( InStream& is, BaseMesh& s );

//template <class T>
//OutStream&    operator<<( OutStream& os, const T& val )
//{
//    os.Write( &val, sizeof( val ) );
//    return os;
//}
//
//template <class T>
//InStream&    operator>>( InStream& is, T& val )
//{
//    is.Read( &val, sizeof( val ) );
//    return is;
//}

template <class T>
OutStream&    operator<<( OutStream& os, const c2::array<T>& arr )
{
    os << arr.size();
    for (int i = 0; i < arr.size(); i++)
    {
        os << arr[i];
    }
    return os;
}

template <class T>
InStream&    operator>>( InStream& is, c2::array<T>& arr )
{
    int sz;
    is >> sz;
    arr.resize( sz );
    for (int i = 0; i < sz; i++)
    {
        is >> arr[i];
    }
    return is;
}

template <class T>
OutStream&    operator<<( OutStream& os, const std::vector<T>& arr )
{
    int sz = arr.size();
    os << sz;
    for (int i = 0; i < arr.size(); i++)
    {
        os << arr[i];
    }
    return os;
}

template <class T>
InStream&    operator>>( InStream& is, std::vector<T>& arr )
{
    int sz;
    is >> sz;
    arr.resize( sz );
    for (int i = 0; i < sz; i++)
    {
        is >> arr[i];
    }
    return is;
}

template <class T> OutStream& RawWriteArray( OutStream& os, const c2::array<T>& arr )
{
    os << arr.size();
    if (arr.size() > 0) os.Write( (void*)&arr[0], arr.size() * sizeof( T ) );
    return os;
}

template <class T> InStream& RawReadArray( InStream& is, c2::array<T>& arr )
{
    int sz;
    is >> sz;
    arr.resize( sz );
    if (arr.size() > 0) is.Read( (void*)&arr[0], sz * sizeof( T ) );
    return is;
}

template <class T> OutStream& RawWriteArray( OutStream& os, const std::vector<T>& arr )
{
    os << ((int)arr.size());
    if (arr.size() > 0) os.Write( (void*)&arr[0], arr.size() * sizeof( T ) );
    return os;
}

template <class T> InStream& RawReadArray( InStream& is, std::vector<T>& arr )
{
    int sz;
    is >> sz;
    arr.resize( sz );
    if (arr.size() > 0) is.Read( (void*)&arr[0], sz * sizeof( T ) );
    return is;
}

template <typename T>
BYTE& Enum2Byte( T& val )
{
    return *((BYTE*)&val);
}

template <typename T>
DWORD& Enum2Dword( T& val )
{
    return *((DWORD*)&val);
}

#ifdef _INLINES
#include "kIOHelpers.inl"
#endif // _INLINES

#endif // __KIOHELPERS_H__