#ifndef __iVector__
#define __iVector__
#define itype int

class iVector {
protected:
	int m_Dim; // # coords (1, 2 or 3)
public:
	itype x, y, z; // z = 0.0 for 2D, y = z = 0.0 for 1D
	// Constructors:
	iVector();
	// Copy - constructor:
	iVector(const iVector &);
	// 1D
	iVector(itype);
	// 2D
	iVector(itype, itype);
	// 3D
	iVector(itype, itype, itype);	
	// Destructor:
	~iVector();
	// Attribution operator:
	void Copy(const iVector &);
	iVector & operator = (const iVector &);
	// Set components to the given values with appropriate number of dimensions:
	iVector & Set(itype);
	iVector & Set(itype, itype);
	iVector & Set(itype, itype, itype);
	// Number of dimensions property:
	int Dim() const;
	int Dim(itype);
	// Reference indexing operator:
	itype & operator[] (itype);
	// Indexing operator:
	itype operator[] (itype) const;
	// Comparisons (epsilon based):	
	bool operator == (const iVector &) const;
	bool operator != (const iVector &) const;
	// Products:
	itype Dot(const iVector &) const;
	iVector Cross(const iVector &) const;
	iVector & Cross(const iVector &, const iVector &);
	itype operator & (const iVector &) const; // dot product
	iVector operator ^ (const iVector &) const; // cross products
	iVector & operator ^= (const iVector &);
	// Unari operators:
	iVector operator + () const;
	iVector operator - () const;
	// Scalar multiplication:
	friend iVector operator * (itype, const iVector &);
	iVector operator * (itype) const;
	// Scalar division:
	iVector operator / (itype) const;	
	// Vector arithmetic:
	iVector operator + (const iVector &) const; // sum of vectors
	iVector operator - (const iVector &) const; // vectors subtract
	iVector operator * (const iVector &) const; // by component scalar mult
	iVector operator / (const iVector &) const; // by component scalar div
	iVector & operator *= (itype);
	iVector & operator /= (itype);
	iVector & operator *= (const iVector &);
	iVector & operator /= (const iVector &);
	iVector & operator += (const iVector &);
	iVector & operator -= (const iVector &);
	// Constants:
	static const iVector MinValue;
	static const iVector MaxValue;
	static const iVector Origin;
	static const iVector Origin1D;
	static const iVector Origin2D;
	static const iVector xAxis;
	static const iVector xAxis2D;
	static const iVector yAxis;
	static const iVector yAxis2D;
	static const iVector zAxis;
	static const iVector Unit;
	// Specific operations:
	itype Length() const;
	__int64 Norm2() const;
	itype DistTo(const iVector &) const; // distance to another iVector
	itype DistToLine(const iVector &p0, const iVector &V) const;
	iVector Abs() const;
	iVector & Invert();
	iVector & LoadZero(itype nDim = -1);	
	void UpdateBoundBox(iVector &bbMin, iVector &bbMax) const;
	iVector & Truncate(const itype MaxLen);
	static iVector Cartesian(/* Polar: */ itype r, itype Phi);
	static iVector Cartesian(/* Spherical */ itype r, itype Phi, itype Psi);
	void Spherical(itype &r, itype &Phi, itype &Psi) const;
	const itype * Ptr() const;
	itype * Ptr();	
};
inline iVector::iVector()
: m_Dim(3), x(0.0), y(0.0), z(0.0) {}
inline iVector::iVector(const iVector &v)
: m_Dim(v.m_Dim), x(v.x), y(v.y), z(v.z) {}
inline iVector::iVector(itype a)
: m_Dim(1), x(a), y(0.0), z(0.0) {}
inline iVector::iVector(itype a, itype b)
: m_Dim(2), x(a), y(b), z(0.0) {}
inline iVector::iVector(itype a, itype b, itype c)
: m_Dim(3), x(a), y(b), z(c) {}
inline iVector::~iVector() {}
inline void iVector::Copy(const iVector &Src) {
	m_Dim = Src.m_Dim;
	x = Src.x;
	y = Src.y;
	z = Src.z;
}
inline iVector & iVector::operator = (const iVector &v) {
	Copy(v);
	return *this;
}
inline iVector & iVector::Set(itype a)
{ m_Dim = 1; x = a; y = z = 0.0; return *this; }
inline iVector & iVector::Set(itype a, itype b)
{ m_Dim = 2; x = a; y = b; z = 0.0; return *this; }
inline iVector & iVector::Set(itype a, itype b, itype c)
{ m_Dim = 3; x = a; y = b; z = c; return *this; }
inline itype iVector::Dim() const {
	return m_Dim;
}
inline itype iVector::Dim(itype n) {
	switch(n) {
		case 1: y = 0.0;
		case 2: z = 0.0;
		case 3:
			return m_Dim = n;
		default:
			assert(n >= 1 && n <= 3);
			return m_Dim;
	}
}
inline itype & iVector::operator [] (itype i) {
	assert(i >= 0 && i < m_Dim);
	return (&x)[i];
}
inline itype iVector::operator [] (itype i) const {
	assert(i >= 0 && i < m_Dim);
	return (&x)[i];
}
inline bool iVector::operator == (const iVector &v) const {
	return x==v.x && y==v.y && z==v.z;
}
inline bool iVector::operator != (const iVector &v) const {
	return x!=v.x || y!=v.y || z!=v.z;
}
inline itype iVector::Dot(const iVector &v) const {
	return (x * v.x + y * v.y + z * v.z);
}
inline iVector iVector::Cross(const iVector &v) const {
	return iVector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
inline iVector & iVector::Cross(const iVector &u, const iVector &v) {
	x = u.y * v.z - u.z * v.y;
	y = u.z * v.x - u.x * v.z;
	z = u.x * v.y - u.y * v.x;
	m_Dim = 3;
	return *this;
}
inline itype iVector::operator & (const iVector &u) const {
	return Dot(u);
}
inline iVector iVector::operator ^ (const iVector &u) const {
	return Cross(u);
}
inline iVector & iVector::operator ^= (const iVector &u) {
	return Cross(*this, u);
}
inline iVector iVector::operator + () const {
	return *this;
}
inline iVector iVector::operator - () const {
	iVector v(-x, -y, -z);
	//v.Dim(m_Dim);
	return v;
}
inline iVector operator * (itype c, const iVector &u) {
	iVector v(c * u.x, c * u.y, c * u.z);
	//v.m_Dim = u.Dim();
	return v;
}
inline iVector iVector::operator * (itype c) const {
	iVector v(c * x, c * y, c * z);
	//v.Dim(m_Dim);
	return v;
}
inline iVector iVector::operator / (itype c) const {
	itype i = 1.0 / c;
	iVector v(x * i, y * i, z * i);
	//v.Dim(m_Dim);
	return v;
}
inline iVector iVector::operator + (const iVector &u) const {
	iVector v(x + u.x, y + u.y, z + u.z);
	//v.Dim(max(m_Dim, u.Dim()));
	return v;
}
inline iVector iVector::operator - (const iVector &u) const {
	iVector v(x - u.x, y - u.y, z - u.z);
	//v.Dim(max(m_Dim, u.Dim()));
	return v;
}
inline iVector iVector::operator * (const iVector &u) const {
	iVector v(x * u.x, y * u.y, z * u.z);
	//v.Dim(max(m_Dim, u.Dim()));
	return v;
}
inline iVector iVector::operator / (const iVector &u) const {
	iVector v(x, y, z);
	itype n = u.Dim();
	switch(n) {
	case 3:
		
		v.z /= u.z;
	case 2:
		v.y /= u.y;
	case 1:
		v.x /= u.x;
		break;
	default:
		assert(n >= 1 && n <= 3);
	}
	//v.Dim(max(m_Dim, n));
	return v;
}
inline iVector & iVector::operator *= (itype c) {
	x *= c;
	y *= c;
	z *= c;
	return *this;
}
inline iVector & iVector::operator /= (itype c) {
	itype i = 1.0 / c;
	x *= i;
	y *= i;
	z *= i;
	return *this;
}
inline iVector & iVector::operator *= (const iVector &u) {
	x *= u.x;
	y *= u.y;
	z *= u.z;
	//m_Dim = max(m_Dim, u.Dim());
	return *this;
}
inline iVector & iVector::operator /= (const iVector &u) {	
	int n=m_Dim;
	switch(n) {
	case 3:
		z /= u.z;
	case 2:
		y /= u.y;
	case 1:
		x /= u.x;
		break;
	default:
		assert(n >= 1 && n <= 3);
	}
	//m_Dim = max(m_Dim, n);
	return *this;
}
inline iVector & iVector::operator += (const iVector &u) {
	x += u.x;
	y += u.y;
	z += u.z;
	//m_Dim = max(m_Dim, u.Dim());
	return *this;
}
inline iVector & iVector::operator -= (const iVector &u) {
	x -= u.x;
	y -= u.y;
	z -= u.z;
	//m_Dim = max(m_Dim, u.Dim());
	return *this;
}
inline itype iVector::Length() const {
	return sqrt(double(Norm2()));
}
//-----------------------------------------------------------------------------
// iVector::Normal
//-----------------------------------------------------------------------------
inline __int64 iVector::Norm2() const {		
	return __int64(x)*__int64(x)+__int64(y)*__int64(y)+__int64(z)*__int64(z);
} // iVector::Normal
// iVector::DistTo(const iVector &)
inline itype iVector::DistTo(const iVector &u) const {
	itype dx = u.x - x;
	itype dy = u.y - y;
	itype dz = u.z - z;
	return sqrt(double(dx * dx + dy * dy + dz * dz));
} // iVector::DistTo(const iVector &)
// iVector::DistToLine
inline itype iVector::DistToLine(const iVector &p0, const iVector &V) const {
	iVector H = (*this - p0) ^ V;
	return H.Length() / V.Length();
} // iVector::DistToLine
// iVector::Invert
inline iVector & iVector::Invert() {
	x = -x;
	y = -y;
	z = -z;
	return *this;
}
// iVector::LoadZero
inline iVector & iVector::LoadZero(itype nDim) {
	if(nDim < 1 || nDim > 3) nDim = m_Dim;
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
} // iVector::LoadZero
// iVector::Ptr() const
inline const itype * iVector::Ptr() const {
	return &x;
} // iVector::Ptr() const
// iVector::Ptr()
inline itype * iVector::Ptr() {
	return &x;
} // iVector::Ptr()//-----------------------------------------------------------------------------
// iVector::Truncate : iVector & (const itype)
//-----------------------------------------------------------------------------
inline iVector & iVector::Truncate(const itype MaxLen) {
	assert(m_Dim == 3);
	if(MaxLen == 0) {
		*this = Origin;
	} else {
		itype l = Length();
		if(l > MaxLen) {
			itype il = MaxLen / l;
			x *= il;
			y *= il;
			z *= il;
		}
	}
	return *this;
}
//} // comms

//iVector64
class iVector64{
public:
	__int64 x;
	__int64 y;
	__int64 z;

	iVector64(){}
	iVector64(__int64 _x,__int64 _y,__int64 _z){
        x=_x;
		y=_y;
		z=_z;
	}
	void Cross64(iVector u,iVector v){
		x = __int64(u.y) * __int64(v.z) - __int64(u.z) * __int64(v.y);
		y = __int64(u.z) * __int64(v.x) - __int64(u.x) * __int64(v.z);
		z = __int64(u.x) * __int64(v.y) - __int64(u.y) * __int64(v.x);
	}
	void Cross64(iVector64 u,iVector64 v){
		x = u.y * v.z - u.z * v.y;
		y = u.z * v.x - u.x * v.z;
		z = u.x * v.y - u.y * v.x;
	}    
};
_inline __int64 dot64(iVector u,iVector v){
	return __int64(u.x)*__int64(v.x)+__int64(u.y)*__int64(v.y)+__int64(u.z)*__int64(v.z);
}
_inline __int64 dot64(iVector64 u,iVector v){
	return u.x*__int64(v.x)+u.y*__int64(v.y)+u.z*__int64(v.z);
}
_inline __int64 dot64(iVector u,iVector64 v){
	return __int64(u.x)*v.x+__int64(u.y)*v.y+__int64(u.z)*v.z;
}
_inline __int64 dot64(iVector64 u,iVector64 v){
	return u.x*v.x+u.y*v.y+u.z*v.z;
}

#endif // __iVector__
