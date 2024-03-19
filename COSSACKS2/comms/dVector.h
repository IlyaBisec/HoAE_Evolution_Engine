#pragma once

class dVector {
protected:
	int m_Dim; // # coords (1, 2 or 3)
public:
	double x, y, z; // z = 0.0 for 2D, y = z = 0.0 for 1D
	// Constructors:
	dVector();
	// Copy - constructor:
	dVector(const dVector &);
	// 1D
	dVector(double);
	// 2D
	dVector(double, double);
	// 3D
	dVector(double, double, double);
	// n - Dim
	dVector(int, int []);
	dVector(int, double []);
	// Destructor:
	~dVector();
	// Attribution operator:
	void Copy(const dVector &);
	dVector & operator = (const dVector &);

	// Set components to the given values with appropriate number of dimensions:
	dVector & Set(double);
	dVector & Set(double, double);
	dVector & Set(double, double, double);

	// Number of dimensions property:
	int Dim() const;
	int Dim(int);

	// Reference indexing operator:
	double & operator[] (int);
	// Indexing operator:
	double operator[] (int) const;

	// Comparisons (epsilon based):
	bool Equals(const dVector &, const double = cMath::dEpsilon) const;
	bool operator == (const dVector &) const;
	bool operator != (const dVector &) const;

	// Products:
	double Dot(const dVector &) const;
	dVector Cross(const dVector &) const;
	dVector & Cross(const dVector &, const dVector &);

	double operator & (const dVector &) const; // dot product
	dVector operator ^ (const dVector &) const; // cross products
	dVector & operator ^= (const dVector &);

	// Unari operators:
	dVector operator + () const;
	dVector operator - () const;

	// Scalar multiplication:
	friend dVector operator * (double, const dVector &);
	dVector operator * (double) const;
	// Scalar division:
	dVector operator / (double) const;
	
	// Vector arithmetic:
	dVector operator + (const dVector &) const; // sum of vectors
	dVector operator - (const dVector &) const; // vectors subtract
	dVector operator * (const dVector &) const; // by component scalar mult
	dVector operator / (const dVector &) const; // by component scalar div

	dVector & operator *= (double);
	dVector & operator /= (double);
	dVector & operator *= (const dVector &);
	dVector & operator /= (const dVector &);
	dVector & operator += (const dVector &);
	dVector & operator -= (const dVector &);

	// Constants:
	static const dVector MinValue;
	static const dVector MaxValue;
	static const dVector Origin;
	static const dVector Origin1D;
	static const dVector Origin2D;
	static const dVector xAxis;
	static const dVector xAxis2D;
	static const dVector yAxis;
	static const dVector yAxis2D;
	static const dVector zAxis;
	static const dVector Unit;

	// Specific operations:
	double Length() const;
	double LengthSq() const;
	dVector Normal() const;
	dVector & Normalize();
	double DistTo(const dVector &) const; // distance to another dVector
	double DistToLine(const dVector &p0, const dVector &V) const;
	dVector Abs() const;
	dVector & Invert();
	dVector & LoadZero(int nDim = -1);
	bool IsZero(const double Epsilon = cMath::dEpsilon) const;
	void UpdateBoundBox(dVector &bbMin, dVector &bbMax) const;
	dVector & Truncate(const double MaxLen);

	static dVector Cartesian(/* Polar: */ double r, double Phi);
	static dVector Cartesian(/* Spherical */ double r, double Phi, double Psi);
	void Spherical(double &r, double &Phi, double &Psi) const;
	const double * Ptr() const;
	double * Ptr();
	friend cStr & operator << (cStr &, const dVector &);

	static double Angle(const dVector &, const dVector &);
};

inline dVector::dVector()
: m_Dim(3), x(0.0), y(0.0), z(0.0) {}

inline dVector::dVector(const dVector &v)
: m_Dim(v.m_Dim), x(v.x), y(v.y), z(v.z) {}

inline dVector::dVector(double a)
: m_Dim(1), x(a), y(0.0), z(0.0) {}

inline dVector::dVector(double a, double b)
: m_Dim(2), x(a), y(b), z(0.0) {}

inline dVector::dVector(double a, double b, double c)
: m_Dim(3), x(a), y(b), z(c) {}

inline dVector::dVector(int n, int a[])
{
	x = y = z = 0.0;
	switch(m_Dim = n) {
	case 3: z = a[2];
	case 2: y = a[1];
	case 1: x = a[0];
		break;
	default:
		cAssert(m_Dim >= 1 && m_Dim <= 3);
	}
}

inline dVector::dVector(int n, double a[])
{
	x = y = z = 0.0;
	switch(m_Dim = n) {
	case 3: z = a[2];
	case 2: y = a[1];
	case 1: x = a[0];
		break;
	default:
		cAssert(m_Dim >= 1 && m_Dim <= 3);
	}
}

inline dVector::~dVector() {}

inline void dVector::Copy(const dVector &Src) {
	m_Dim = Src.m_Dim;
	x = Src.x;
	y = Src.y;
	z = Src.z;
}

inline dVector & dVector::operator = (const dVector &v) {
	Copy(v);
	return *this;
}

inline dVector & dVector::Set(double a)
{ m_Dim = 1; x = a; y = z = 0.0; return *this; }

inline dVector & dVector::Set(double a, double b)
{ m_Dim = 2; x = a; y = b; z = 0.0; return *this; }

inline dVector & dVector::Set(double a, double b, double c)
{ m_Dim = 3; x = a; y = b; z = c; return *this; }

inline int dVector::Dim() const {
	return m_Dim;
}

inline int dVector::Dim(int n) {
	switch(n) {
		case 1: y = 0.0;
		case 2: z = 0.0;
		case 3:
			return m_Dim = n;
		default:
			cAssert(n >= 1 && n <= 3);
			return m_Dim;
	}
}

inline double & dVector::operator [] (int i) {
	cAssert(i >= 0 && i < m_Dim);
	return (&x)[i];
}

inline double dVector::operator [] (int i) const {
	cAssert(i >= 0 && i < m_Dim);
	return (&x)[i];
}

inline bool dVector::Equals(const dVector &Src, const double Epsilon) const {
	if(m_Dim != Src.m_Dim) return false;
	bool f = true;
	switch(m_Dim) {
		case 3: f = f && cMath::Equals(z, Src.z, Epsilon);
		case 2: f = f && cMath::Equals(y, Src.y, Epsilon);
		case 1: f = f && cMath::Equals(x, Src.x, Epsilon);
			break;
		default:
			cAssert(m_Dim >= 1 && m_Dim <= 3);
	}
	return f;
}

inline bool dVector::operator == (const dVector &v) const {
	return Equals(v);
}

inline bool dVector::operator != (const dVector &v) const {
	return !Equals(v);
}

inline double dVector::Dot(const dVector &v) const {
	return (x * v.x + y * v.y + z * v.z);
}

inline dVector dVector::Cross(const dVector &v) const {
	return dVector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

inline dVector & dVector::Cross(const dVector &u, const dVector &v) {
	x = u.y * v.z - u.z * v.y;
	y = u.z * v.x - u.x * v.z;
	z = u.x * v.y - u.y * v.x;
	m_Dim = 3;
	return *this;
}

inline double dVector::operator & (const dVector &u) const {
	return Dot(u);
}

inline dVector dVector::operator ^ (const dVector &u) const {
	return Cross(u);
}

inline dVector & dVector::operator ^= (const dVector &u) {
	return Cross(*this, u);
}

inline dVector dVector::operator + () const {
	return *this;
}

inline dVector dVector::operator - () const {
	dVector v(-x, -y, -z);
	v.Dim(m_Dim);
	return v;
}

inline dVector operator * (double c, const dVector &u) {
	dVector v(c * u.x, c * u.y, c * u.z);
	v.m_Dim = u.Dim();
	return v;
}

inline dVector dVector::operator * (double c) const {
	dVector v(c * x, c * y, c * z);
	v.Dim(m_Dim);
	return v;
}

inline dVector dVector::operator / (double c) const {
	cAssert(!cMath::IsZero(c));
	double i = 1.0 / c;
	dVector v(x * i, y * i, z * i);
	v.Dim(m_Dim);
	return v;
}

inline dVector dVector::operator + (const dVector &u) const {
	dVector v(x + u.x, y + u.y, z + u.z);
	v.Dim(cMath::Max(m_Dim, u.Dim()));
	return v;
}

inline dVector dVector::operator - (const dVector &u) const {
	dVector v(x - u.x, y - u.y, z - u.z);
	v.Dim(cMath::Max(m_Dim, u.Dim()));
	return v;
}

inline dVector dVector::operator * (const dVector &u) const {
	dVector v(x * u.x, y * u.y, z * u.z);
	v.Dim(cMath::Max(m_Dim, u.Dim()));
	return v;
}

inline dVector dVector::operator / (const dVector &u) const {
	dVector v(x, y, z);
	int n = u.Dim();
	switch(n) {
	case 3:
		cAssert(!cMath::IsZero(u.z));
		v.z /= u.z;
	case 2:
		cAssert(!cMath::IsZero(u.y));
		v.y /= u.y;
	case 1:
		cAssert(!cMath::IsZero(u.x));
		v.x /= u.x;
		break;
	default:
		cAssert(n >= 1 && n <= 3);
	}
	v.Dim(cMath::Max(m_Dim, n));
	return v;
}

inline dVector & dVector::operator *= (double c) {
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

inline dVector & dVector::operator /= (double c) {
	cAssert(!cMath::IsZero(c));
	double i = 1.0 / c;
	x *= i;
	y *= i;
	z *= i;
	return *this;
}

inline dVector & dVector::operator *= (const dVector &u) {
	x *= u.x;
	y *= u.y;
	z *= u.z;
	m_Dim = cMath::Max(m_Dim, u.Dim());
	return *this;
}

inline dVector & dVector::operator /= (const dVector &u) {
	int n = u.Dim();
	switch(n) {
	case 3:
		cAssert(!cMath::IsZero(u.z));
		z /= u.z;
	case 2:
		cAssert(!cMath::IsZero(u.y));
		y /= u.y;
	case 1:
		cAssert(!cMath::IsZero(u.x));
		x /= u.x;
		break;
	default:
		cAssert(n >= 1 && n <= 3);
	}
	m_Dim = cMath::Max(m_Dim, n);
	return *this;
}

inline dVector & dVector::operator += (const dVector &u) {
	x += u.x;
	y += u.y;
	z += u.z;
	m_Dim = cMath::Max(m_Dim, u.Dim());
	return *this;
}

inline dVector & dVector::operator -= (const dVector &u) {
	x -= u.x;
	y -= u.y;
	z -= u.z;
	m_Dim = cMath::Max(m_Dim, u.Dim());
	return *this;
}

inline double dVector::Length() const {
	return sqrt(x * x + y * y + z * z);
}

inline double dVector::LengthSq() const {
	return x * x + y * y + z * z;
}

//-----------------------------------------------------------------------------
// dVector::Normalize
//-----------------------------------------------------------------------------
inline dVector & dVector::Normalize() {
	double L = Length();
	if(cMath::IsZero(L)) {
		LoadZero();
	} else {
		double i = 1.0 / L;
		x *= i, y *= i, z *= i;
	}
	return *this;
} // dVector::Normalize

//-----------------------------------------------------------------------------
// dVector::Normal
//-----------------------------------------------------------------------------
inline dVector dVector::Normal() const {
	dVector t(*this);
	return t.Normalize();
} // dVector::Normal

// dVector::DistTo(const dVector &)
inline double dVector::DistTo(const dVector &u) const {
	double dx = u.x - x;
	double dy = u.y - y;
	double dz = u.z - z;
	return sqrt(dx * dx + dy * dy + dz * dz);
} // dVector::DistTo(const dVector &)

// dVector::DistToLine
inline double dVector::DistToLine(const dVector &p0, const dVector &V) const {
	dVector H = (*this - p0) ^ V;
	return H.Length() / V.Length();
} // dVector::DistToLine



inline dVector dVector::Abs() const {
	dVector r(abs(x), abs(y), abs(z));
	r.m_Dim = m_Dim;
	return r;
}

// dVector::Invert
inline dVector & dVector::Invert() {
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

// dVector::LoadZero
inline dVector & dVector::LoadZero(int nDim) {
	if(nDim < 1 || nDim > 3) nDim = m_Dim;
	switch(nDim) {
		case 1: *this = Origin1D;
			break;
		case 2: *this = Origin2D;
			break;
		case 3: *this = Origin;
			break;
		default: cAssert(nDim >= 1 && nDim <= 3);
	}
	return *this;
} // dVector::LoadZero

//-----------------------------------------------------------------------------
// dVector::Cartesian : dVector(double, double)
//-----------------------------------------------------------------------------
inline dVector dVector::Cartesian(double r, double Phi) { /* Polar: */
	double x = r * cos(Phi);
	double y = r * sin(Phi);
	return dVector(x, y);
}

//-----------------------------------------------------------------------------
// dVector::Cartesian : dVector(double, double, double)
//-----------------------------------------------------------------------------
inline dVector dVector::Cartesian(double r, double Phi, double Psi) { /* Spherical */
	double x = r * sin(Phi) * sin(Psi);
	double y = r * cos(Psi);
	double z = r * cos(Phi) * sin(Psi);
	return dVector(x, y, z);
}

//-----------------------------------------------------------------------------
// Spherical
//-----------------------------------------------------------------------------
inline void dVector::Spherical(double &r, double &Phi, double &Psi) const {
	r = Length();
	if(cMath::IsZero(r)) {
		r = 0;
	} else {
		Psi = acos(y / r);
		double l = sqrt(cMath::Square(x) + cMath::Square(z));
		if(cMath::IsZero(l)) {
			Phi = 0;
		} else {
			if(z >= 0) Phi = asin(x / l);
			else Phi = cMath::Pi - asin(x / l);
			Phi = cMath::AngleNormalizeTwoPi((float)Phi);
		}
	}
}

// dVector::Ptr() const
inline const double * dVector::Ptr() const {
	return &x;
} // dVector::Ptr() const

// dVector::Ptr()
inline double * dVector::Ptr() {
	return &x;
} // dVector::Ptr()

//-----------------------------------------------------------------------------
// dVector::IsZero
//-----------------------------------------------------------------------------
inline bool dVector::IsZero(const double Epsilon) const {
	switch(m_Dim) {
		case 1: return Equals(Origin1D, Epsilon);
		case 2: return Equals(Origin2D, Epsilon);
		case 3: return Equals(Origin, Epsilon);
		default: ;//cAssert(RangeCheck(m_Dim, 1, 3));
			return false;
	}
} // dVector::IsZero

//-----------------------------------------------------------------------------
// dVector::Truncate : dVector & (const double)
//-----------------------------------------------------------------------------
inline dVector & dVector::Truncate(const double MaxLen) {
	cAssert(m_Dim == 3);
	if(MaxLen == 0) {
		*this = Origin;
	} else {
		double l = Length();
		if(l > MaxLen) {
			double il = MaxLen / l;
			x *= il;
			y *= il;
			z *= il;
		}
	}
	return *this;
}

//-----------------------------------------------------------------------------
// dVector::Angle : double(const dVector &, const dVector &)
//-----------------------------------------------------------------------------
inline double dVector::Angle(const dVector &u, const dVector &v) {
	double d = u.Dot(v);
	double ll = u.Length() * v.Length();
	double Angle = acos(d / ll);

	if(_isnan(Angle)) {
		return 0;
	}
	return Angle;
}