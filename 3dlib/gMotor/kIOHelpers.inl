/*****************************************************************************/
/*	File:	kIOHelpers.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	20.02.2003
/*****************************************************************************/
#include "mMath2D.h"

_inl OutStream&	operator<<( OutStream& os, const Quaternion& val )
{
	os << val.v.x << val.v.y << val.v.z << val.s;
	return os;
}

_inl InStream& operator>>( InStream& is, Quaternion& val )
{
	is >> val.v.x >> val.v.y >> val.v.z >> val.s;
	return is;
}

_inl OutStream&	operator<<( OutStream& os, const AABoundBox& val )
{
	os << val.minv << val.maxv;
	return os;
}

_inl InStream& operator>>( InStream& is, AABoundBox& val )
{
	is >> val.minv >> val.maxv;
	return is;
}

_inl OutStream&	operator<<( OutStream& os, const Sphere& val )
{
    os << val.GetCenter() << val.GetRadius();
    return os;
}

_inl InStream& operator>>( InStream& is, Sphere& val )
{
    Vector3D c;
    float r;
    is >> c >> r;
    val.SetCenter( c );
    val.SetRadius( r );
    return is;
}

_inl OutStream&	operator<<( OutStream& os, const Plane& val )
{
	os << val.a <<val. b << val.c << val.d;
	return os;
}

_inl InStream& operator>>( InStream& is, Plane& val )
{
	is >> val.a >> val.b >> val.c >> val.d;
	return is;
}

_inl OutStream&	operator<<( OutStream& os, const Rct& r )
{
	os << r.x << r.y << r.w << r.h;
	return os;
}

_inl InStream&	operator>>( InStream& is, Rct& r )
{
	is >> r.x >> r.y >> r.w >> r.h;
	return is;
}

_inl OutStream&	operator<<( OutStream& os, const Vector3D& v )
{
	os << v.x << v.y << v.z;
	return os;
}

_inl InStream&	operator>>( InStream& is, Vector3D& v )
{
	is >> v.x >> v.y >> v.z;
	return is;
}

_inl OutStream&	operator<<( OutStream& os, const Vector4D& v )
{
	os << v.x << v.y << v.z << v.w;
	return os;
}

_inl InStream&	operator>>( InStream& is, Vector4D& v )
{
	is >> v.x >> v.y >> v.z >> v.w;
	return is;
}

_inl OutStream& operator<<( OutStream& os, const Matrix4D& m )
{
	os << m.e00 << m.e01 << m.e02;
	os << m.e10 << m.e11 << m.e12;
	os << m.e20 << m.e21 << m.e22;
	os << m.e30 << m.e31 << m.e32;
	return os;
} // operator<<

_inl InStream& operator>>( InStream& is, Matrix4D& m )
{
	is >> m.e00 >> m.e01 >> m.e02;
	is >> m.e10 >> m.e11 >> m.e12;
	is >> m.e20 >> m.e21 >> m.e22;
	is >> m.e30 >> m.e31 >> m.e32;
	m.e03 = 0.0f; m.e13 = 0.0f; m.e23 = 0.0f; m.e33 = 1.0f;
	return is;
} // operator<<


_inl OutStream& operator<<( OutStream& os, const Matrix3D& m )
{
	os << m.e00 << m.e01 << m.e02;
	os << m.e10 << m.e11 << m.e12;
	os << m.e20 << m.e21 << m.e22;
	return os;
} // operator<<

_inl InStream& operator>>( InStream& is, Matrix3D& m )
{
	is >> m.e00 >> m.e01 >> m.e02;
	is >> m.e10 >> m.e11 >> m.e12;
	is >> m.e20 >> m.e21 >> m.e22;
	return is;
} // operator<<

_inl OutStream&	operator<<( OutStream& os, const Matrix2Df& m )
{
	os << m.e00 << m.e01 << m.e10 << m.e11;
	return os;
}

_inl InStream&	operator>>( InStream& is, Matrix2Df& m )
{
	is >> m.e00 >> m.e01 >> m.e10 >> m.e11;
	return is;
}
