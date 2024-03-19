#ifndef __cVector__
#define __cVector__

#include "cUtils.h"
#include "cStr.h"

namespace comms {

class cVector {
protected:
	int m_dim; // # coords (1, 2 or 3)
public:
	double x, y, z; // z = 0.0 for 2D, y = z = 0.0 for 1D
	// Constructors:
	cVector();
	// Copy - constructor:
	cVector(const cVector &);
	// 1D
	cVector(double);
	// 2D
	cVector(double, double);
	// 3D
	cVector(double, double, double);
	// n - Dim
	cVector(int, int []);
	cVector(int, double []);
	// Destructor:
	~cVector();
	// Attribution operator:
	cVector & operator = (const cVector &);

	// Set components to the given values with appropriate number of dimensions:
	cVector & Set(double);
	cVector & Set(double, double);
	cVector & Set(double, double, double);

	// Number of dimensions property:
	int Dim() const;
	int Dim(int);

	// Reference indexing operator:
	double & operator[] (int);
	// Indexing operator:
	double operator[] (int) const;

	// Comparisons (epsilon based):
	bool Equals(const cVector &, const double = kEpsilon) const;
	bool operator == (const cVector &) const;
	bool operator != (const cVector &) const;

	// Products:
	double Dot(const cVector &) const;
	cVector Cross(const cVector &) const;
	cVector & Cross(const cVector &, const cVector &);

	double operator & (const cVector &) const; // dot product
	cVector operator ^ (const cVector &) const; // cross products
	cVector & operator ^= (const cVector &);

	// Unari operators:
	cVector operator + () const;
	cVector operator - () const;

	// Scalar multiplication:
	friend cVector operator * (double, const cVector &);
	cVector operator * (double) const;
	// Scalar division:
	cVector operator / (double) const;
	
	// Vector arithmetic:
	cVector operator + (const cVector &) const; // sum of vectors
	cVector operator - (const cVector &) const; // vectors subtract
	cVector operator * (const cVector &) const; // by component scalar mult
	cVector operator / (const cVector &) const; // by component scalar div

	cVector & operator *= (double);
	cVector & operator /= (double);
	cVector & operator *= (const cVector &);
	cVector & operator /= (const cVector &);
	cVector & operator += (const cVector &);
	cVector & operator -= (const cVector &);

	// Constants:
	static const cVector MinValue;
	static const cVector MaxValue;
	static const cVector Origin;
	static const cVector Origin1D;
	static const cVector Origin2D;
	static const cVector xAxis;
	static const cVector xAxis2D;
	static const cVector yAxis;
	static const cVector yAxis2D;
	static const cVector zAxis;
	static const cVector Unit;

	// Specific operations:
	double Length() const;
	double Normalize();
	double DistTo(const cVector &) const; // distance to another cVector
	double DistToLine(const cVector &p0, const cVector &V) const;
	cVector & Abs();
	cVector & Reverse();
	cVector & LoadZero(int nDim = -1);

	static cVector Cartesian(/* Polar: */ double r, double Phi);
	static cVector Cartesian(/* Spherical */ double r, double Phi, double Psi);
	const double * Ptr() const;
	double * Ptr();
	cStr ToStr(int nPrec = 2, const char *pSep = ", ") const;
};

inline cVector::cVector()
: m_dim(3), x(0.0), y(0.0), z(0.0) {}

inline cVector::cVector(const cVector &v)
: m_dim(v.m_dim), x(v.x), y(v.y), z(v.z) {}

inline cVector::cVector(double a)
: m_dim(1), x(a), y(0.0), z(0.0) {}

inline cVector::cVector(double a, double b)
: m_dim(2), x(a), y(b), z(0.0) {}

inline cVector::cVector(double a, double b, double c)
: m_dim(3), x(a), y(b), z(c) {}

inline cVector::cVector(int n, int a[])
{
	x = y = z = 0.0;
	switch(m_dim = n) {
	case 3: z = a[2];
	case 2: y = a[1];
	case 1: x = a[0];
		break;
	default:
		assert(m_dim >= 1 && m_dim <= 3);
	}
}

inline cVector::cVector(int n, double a[])
{
	x = y = z = 0.0;
	switch(m_dim = n) {
	case 3: z = a[2];
	case 2: y = a[1];
	case 1: x = a[0];
		break;
	default:
		assert(m_dim >= 1 && m_dim <= 3);
	}
}

inline cVector::~cVector() {}

inline cVector & cVector::operator = (const cVector &v)
{ m_dim = v.Dim(); x = v.x, y = v.y, z = v.z; return *this; }

inline cVector & cVector::Set(double a)
{ m_dim = 1; x = a; y = z = 0.0; return *this; }

inline cVector & cVector::Set(double a, double b)
{ m_dim = 2; x = a; y = b; z = 0.0; return *this; }

inline cVector & cVector::Set(double a, double b, double c)
{ m_dim = 3; x = a; y = b; z = c; return *this; }

inline int cVector::Dim() const {
	return m_dim;
}

inline int cVector::Dim(int n) {
	switch(n) {
		case 1: y = 0.0;
		case 2: z = 0.0;
		case 3:
			return m_dim = n;
		default:
			assert(n >= 1 && n <= 3);
			return m_dim;
	}
}

inline double & cVector::operator [] (int i) {
	assert(i >= 0 && i < m_dim);
	return (&x)[i];
}

inline double cVector::operator [] (int i) const {
	assert(i >= 0 && i < m_dim);
	return (&x)[i];
}

inline bool cVector::Equals(const cVector &v, const double Epsilon) const {
	if(m_dim != v.Dim()) return false;
	switch(m_dim) {
	case 1:
		return cUtils::AreEquals(x, v.x, Epsilon);
	case 2:
		return cUtils::AreEquals(x, v.x, Epsilon) &&
			cUtils::AreEquals(y, v.y, Epsilon);
	case 3:
	default:
		return cUtils::AreEquals(x, v.x, Epsilon) &&
			cUtils::AreEquals(y, v.y, Epsilon) &&
			cUtils::AreEquals(z, v.z, Epsilon);
	}
}

inline bool cVector::operator == (const cVector &v) const {
	return Equals(v);
}

inline bool cVector::operator != (const cVector &v) const {
	return !Equals(v);
}

inline double cVector::Dot(const cVector &v) const {
	return (x * v.x + y * v.y + z * v.z);
}

inline cVector cVector::Cross(const cVector &v) const {
	return cVector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

inline cVector & cVector::Cross(const cVector &u, const cVector &v) {
	x = u.y * v.z - u.z * v.y;
	y = u.z * v.x - u.x * v.z;
	z = u.x * v.y - u.y * v.x;
	m_dim = 3;
	return *this;
}

inline double cVector::operator & (const cVector &u) const {
	return Dot(u);
}

inline cVector cVector::operator ^ (const cVector &u) const {
	return Cross(u);
}

inline cVector & cVector::operator ^= (const cVector &u) {
	return Cross(*this, u);
}

inline cVector cVector::operator + () const {
	return *this;
}

inline cVector cVector::operator - () const {
	cVector v(-x, -y, -z);
	v.Dim(m_dim);
	return v;
}

inline cVector operator * (double c, const cVector &u) {
	cVector v(c * u.x, c * u.y, c * u.z);
	v.m_dim = u.Dim();
	return v;
}

inline cVector cVector::operator * (double c) const {
	cVector v(c * x, c * y, c * z);
	v.Dim(m_dim);
	return v;
}

inline cVector cVector::operator / (double c) const {
	assert(cUtils::NotZero(c));
	double i = 1.0 / c;
	cVector v(x * i, y * i, z * i);
	v.Dim(m_dim);
	return v;
}

inline cVector cVector::operator + (const cVector &u) const {
	cVector v(x + u.x, y + u.y, z + u.z);
	v.Dim(Max(m_dim, u.Dim()));
	return v;
}

inline cVector cVector::operator - (const cVector &u) const {
	cVector v(x - u.x, y - u.y, z - u.z);
	v.Dim(Max(m_dim, u.Dim()));
	return v;
}

inline cVector cVector::operator * (const cVector &u) const {
	cVector v(x * u.x, y * u.y, z * u.z);
	v.Dim(Max(m_dim, u.Dim()));
	return v;
}

inline cVector cVector::operator / (const cVector &u) const {
	cVector v(x, y, z);
	int n = u.Dim();
	switch(n) {
	case 3:
		assert(cUtils::NotZero(u.z));
		v.z /= u.z;
	case 2:
		assert(cUtils::NotZero(u.y));
		v.y /= u.y;
	case 1:
		assert(cUtils::NotZero(u.x));
		v.x /= u.x;
		break;
	default:
		assert(n >= 1 && n <= 3);
	}
	v.Dim(Max(m_dim, n));
	return v;
}

inline cVector & cVector::operator *= (double c) {
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

inline cVector & cVector::operator /= (double c) {
	assert(cUtils::NotZero(c));
	double i = 1.0 / c;
	x *= i;
	y *= i;
	z *= i;
	return *this;
}

inline cVector & cVector::operator *= (const cVector &u) {
	x *= u.x;
	y *= u.y;
	z *= u.z;
	m_dim = Max(m_dim, u.Dim());
	return *this;
}

inline cVector & cVector::operator /= (const cVector &u) {
	int n = u.Dim();
	switch(n) {
	case 3:
		assert(cUtils::NotZero(u.z));
		z /= u.z;
	case 2:
		assert(cUtils::NotZero(u.y));
		y /= u.y;
	case 1:
		assert(cUtils::NotZero(u.x));
		x /= u.x;
		break;
	default:
		assert(n >= 1 && n <= 3);
	}
	m_dim = Max(m_dim, n);
	return *this;
}

inline cVector & cVector::operator += (const cVector &u) {
	x += u.x;
	y += u.y;
	z += u.z;
	m_dim = Max(m_dim, u.Dim());
	return *this;
}

inline cVector & cVector::operator -= (const cVector &u) {
	x -= u.x;
	y -= u.y;
	z -= u.z;
	m_dim = Max(m_dim, u.Dim());
	return *this;
}

inline double cVector::Length() const {
	return sqrt(x * x + y * y + z * z);
}

// cVector::Normalize
inline double cVector::Normalize() {
	double L = Length();
	if(cUtils::IsZero(L)) {
		LoadZero();
		return 0.0;
	}
	double i = 1.0 / L;
	x *= i, y *= i, z *= i;
	return i * L;
} // cVector::Normalize

// cVector::DistTo(const cVector &)
inline double cVector::DistTo(const cVector &u) const {
	double dx = u.x - x;
	double dy = u.y - y;
	double dz = u.z - z;
	return sqrt(dx * dx + dy * dy + dz * dz);
} // cVector::DistTo(const cVector &)

// cVector::DistToLine
inline double cVector::DistToLine(const cVector &p0, const cVector &V) const {
	cVector H = (*this - p0) ^ V;
	return H.Length() / V.Length();
} // cVector::DistToLine



inline cVector & cVector::Abs() {
	x = fabs(x);
	y = fabs(y);
	z = fabs(z);
	return *this;
}

// cVector::Reverse
inline cVector & cVector::Reverse() {
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

// cVector::LoadZero
inline cVector & cVector::LoadZero(int nDim) {
	if(nDim < 1 || nDim > 3) nDim = m_dim;
	switch(nDim) {
		case 1: *this = Origin1D;
			break;
		case 2: *this = Origin2D;
			break;
		case 3: *this = Origin;
			break;
		default: assert(nDim >= 1 && nDim <= 3);
	}
	return *this;
} // cVector::LoadZero

//-----------------------------------------------------------------------------
// cVector::Cartesian : cVector(double, double)
//-----------------------------------------------------------------------------
inline cVector cVector::Cartesian(double r, double Phi) { /* Polar: */
	double x = r * cos(Phi);
	double y = r * sin(Phi);
	return cVector(x, y);
}

//-----------------------------------------------------------------------------
// cVector::Cartesian : cVector(double, double, double)
//-----------------------------------------------------------------------------
inline cVector cVector::Cartesian(double r, double Phi, double Psi) { /* Spherical */
	double x = r * sin(Phi) * sin(Psi);
	double y = r * cos(Psi);
	double z = r * cos(Phi) * sin(Psi);
	return cVector(x, y, z);
}

// cVector::Ptr() const
inline const double * cVector::Ptr() const {
	return &x;
} // cVector::Ptr() const

// cVector::Ptr()
inline double * cVector::Ptr() {
	return &x;
} // cVector::Ptr()

} // comms

#endif // __cVector__