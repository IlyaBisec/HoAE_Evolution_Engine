#ifndef __cMatrix__
#define __cMatrix__

#include "cUtils.h"
#include "cVector.h"

namespace comms {

class cMatrix {
protected:
	int m_dim; // 2(D - 2x2), 3(D - 3x3), 4(D - 4x4)
public:
	double m[4][4]; // [row][column]

	// Constructors:
	cMatrix(); // Default constructor.
	cMatrix & operator = (const cMatrix &src); // Assignment operator.
	cMatrix(const cMatrix &src); // Copy - constructor.
	// 2D (2x2):
	cMatrix(double m00, double m01,
		double m10, double m11);
	explicit cMatrix(const double src[2][2]);
	cMatrix(const cVector &xAxis, const cVector &yAxis);
	// 3D (3x3):
	cMatrix(double m00, double m01, double m02,
		double m10, double m11, double m12,
		double m20, double m21, double m22);
	explicit cMatrix(const double src[3][3]);
	cMatrix(const cVector &xAxis, const cVector &yAxis, const cVector &zAxis);
	// 4D (4x4);
	cMatrix(double m00, double m01, double m02, double m03,
		double m10, double m11, double m12, double m13,
		double m20, double m21, double m22, double m23,
		double m30, double m31, double m32, double m33);
	explicit cMatrix(const double src[4][4]);
	// 3D or 4D:
	cMatrix(const cMatrix &Rotation, const cVector &Translation);

	~cMatrix(); // Destructor.

	// Number of dimensions property:
	int Dim() const;
	int Dim(int nDim);

	// Reference indexing operator:
	double & operator () (int row, int col);
	// Indexing operator:
	const double & operator () (int row, int col) const;

	// Comparators (epsilon based):
	bool Equals(const cMatrix &, const double = kEpsilon) const;
	bool operator == (const cMatrix &) const;
	bool operator != (const cMatrix &) const;

	// Unari operators:
	cMatrix operator + () const;
	cMatrix operator - () const;

	// Matrix arithmetic:
	cMatrix operator * (double) const;
	cMatrix & operator *= (double);
	cMatrix operator / (double) const;
	cMatrix & operator /= (double);
	friend cMatrix operator * (double, const cMatrix &);
	cMatrix operator + (const cMatrix &) const;
	cMatrix & operator += (const cMatrix &);
	cMatrix operator - (const cMatrix &) const;
	cMatrix & operator -= (const cMatrix &);
	cMatrix operator * (const cMatrix &) const;
	cMatrix & operator *= (const cMatrix &);
	cVector operator * (const cVector &) const;
	friend cVector operator * (const cVector &, const cMatrix &);
	friend cVector & operator *= (cVector &, const cMatrix &);

	// Constants:
	// (2x2):
	static const cMatrix Zero2D;
	static const cMatrix Identity2D;
	// (3x3):
	static const cMatrix Zero3D;
	static const cMatrix Identity3D;
	// (4x4):
	static const cMatrix Zero4D;
	static const cMatrix Identity4D;

	// Specific operations:
	void LoadZero(int nDim = -1);
	void LoadIdentity(int nDim = -1);
	bool IsZero(const double Epsilon = kEpsilon) const;
	bool IsIdentity(const double Epsilon = kEpsilon) const;
	bool IsSymmetric(const double Epsilon = kEpsilon) const;
	double CalcTrace() const;
	double CalcDeterminant() const;
	bool Invert();
	cMatrix Inverse() const;
	cMatrix &  Transpose();
	cMatrix Transposed() const;
	
	static cMatrix OfTranslation(const cVector &);
	static cMatrix OfRotation(const cVector &Axis, double Angle); // Returns 3D (3x3) matrix of rotation.
	static cMatrix OfRotation(double Yaw, double Pitch, double Roll);
	static cMatrix OfRotation(double Angle); // Returns 2D (2x2) matrix of rotation.
	static cMatrix OfScale(const cVector &);
	static cMatrix OfMirror(const cVector &);
	static cMatrix OfTransform(double SinA, double CosA, double SinB, double CosB, const cVector &Pos);
	static cMatrix OfTransform(double Alpha, double Beta, const cVector &Pos);
	static cMatrix OfTransform(const cVector &Dir, const cVector &Pos);
	static cMatrix OfLookAt(const cVector &Eye, const cVector &Center, const cVector &Up); // Creates a viewing matrix derived from an eye point, a reference point indicating the center of the scene, and an up vector.
	static cMatrix OfPerspective(double c);

	static cMatrix OfProjection(double Fov, double AspectWtoH, double zNear, double zFar);
	static cMatrix OfInfiniteProjection(double Fov, double AspectWtoH, double zNear);

	bool EigenSolve(doubles &EigenValues, cVectors &EigenVectors);
	bool EigenSolve(double &EigenValue, cVector &EigenVector) const; // Returns greatest eigen value of symmetric matrix and proper eigen vector.
};

// cMatrix
inline cMatrix::cMatrix() : m_dim(4) {
	ZeroMemory(m, sizeof(m));
} // cMatrix

// cMatrix::operator =
inline cMatrix & cMatrix::operator = (const cMatrix &src) {
	CopyMemory(m, src.m, sizeof(m));
	m_dim = src.m_dim;
	return *this;
} // cMatrix::operator =

// cMatrix(const cMatrix &)
inline cMatrix::cMatrix(const cMatrix &src) {
	*this = src;
} // cMatrix(const cMatrix &)

// cMatrix(2 x 2 doubles)
inline cMatrix::cMatrix(double m00, double m01,
						double m10, double m11) : m_dim(2) {
	ZeroMemory(m, sizeof(m));
	m[0][0] = m00, m[0][1] = m01;
	m[1][0] = m10, m[1][1] = m11;
} // cMatrix(2 x 2 doubles)

// cMatrix(array 2 x 2 of doubles)
inline cMatrix::cMatrix(const double src[2][2]) : m_dim(2) {
	ZeroMemory(m, sizeof(m));
	m[0][0] = src[0][0], m[0][1] = src[0][1];
	m[1][0] = src[1][0], m[1][1] = src[1][1];
} // cMatrix(array 2 x 2 of doubles)

// cMatrix(const cVector &, const cVector &)
inline cMatrix::cMatrix(const cVector &xAxis, const cVector &yAxis) : m_dim(2) {
	assert(xAxis.Dim() == 2);
	assert(yAxis.Dim() == 2);

	ZeroMemory(m, sizeof(m));
	m[0][0] = xAxis.x, m[0][1] = xAxis.y;
	m[1][0] = yAxis.x, m[1][1] = yAxis.y;
} // cMatrix(const cVector &, const cVector &))

// cMatrix(3 x 3 doubles)
inline cMatrix::cMatrix(double m00, double m01, double m02,
						double m10, double m11, double m12,
						double m20, double m21, double m22) : m_dim(3) {
	ZeroMemory(m, sizeof(m));
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02;
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12;
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22;
} // cMatrix(3 x 3 doubles)

// cMatrix(array 3 x 3 of doubles)
inline cMatrix::cMatrix(const double src[3][3]) : m_dim(3) {
	ZeroMemory(m, sizeof(m));
	m[0][0] = src[0][0], m[0][1] = src[0][1], m[0][2] = src[0][2];
	m[1][0] = src[1][0], m[1][1] = src[1][1], m[1][2] = src[1][2];
	m[2][0] = src[2][0], m[2][1] = src[2][1], m[2][2] = src[2][2];
} // cMatrix(array 3 x 3 of doubles)

// cMatrix(const cVector &, const cVector &, const cVector &)
inline cMatrix::cMatrix(const cVector &xAxis, const cVector &yAxis, const cVector &zAxis) : m_dim(3) {
	assert(xAxis.Dim() == 3);
	assert(yAxis.Dim() == 3);
	assert(zAxis.Dim() == 3);

	ZeroMemory(m, sizeof(m));
	m[0][0] = xAxis.x, m[0][1] = xAxis.y, m[0][2] = xAxis.z;
	m[1][0] = yAxis.x, m[1][1] = yAxis.y, m[1][2] = yAxis.z;
	m[2][0] = zAxis.x, m[2][1] = zAxis.y, m[2][2] = zAxis.z;
} // cMatrix(const cVector &, const cVector &, const cVector &)

// cMatrix(4 x 4 doubles)
inline cMatrix::cMatrix(double m00, double m01, double m02, double m03,
						double m10, double m11, double m12, double m13,
						double m20, double m21, double m22, double m23,
						double m30, double m31, double m32, double m33) : m_dim(4) {
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03;
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13;
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23;
	m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;
} // cMatrix(4 x 4 doubles)

// cMatrix(array 4 x 4 of doubles)
inline cMatrix::cMatrix(const double src[4][4]) : m_dim(4) {
	CopyMemory(m, src, sizeof(m));
} // cMatrix(array 4 x 4 of doubles)

// cMatrix::Dim() const
inline int cMatrix::Dim() const {
	return m_dim;
} // cMatrix::Dim() const

// cMatrix::Dim(int)
inline int cMatrix::Dim(int nDim) {
	assert(RangeCheck(nDim, 2, 4));
	if(!RangeCheck(nDim, 2, 4) || m_dim == nDim) return m_dim;

	switch(nDim) {
		case 2: m[2][0] = m[0][2] = m[2][1] = m[1][2] = m[2][2] = 0.0;
			if(m_dim == 3) break;
		case 4: m[3][3] = 1.0;
			if(m_dim == 3) break;
		case 3:
			if(m_dim == 4) {
				m[3][0] = m[0][3] = m[3][1] = m[1][3] = m[3][2] = m[2][3] = m[3][3] = 0.0;
				break;
			}
			m[2][2] = 1.0;
	}
	return m_dim = nDim;
} // cMatrix::Dim(int)

// cMatrix(const cMatrix &, const cVector &)
inline cMatrix::cMatrix(const cMatrix &Rotation, const cVector &Translation) {
	assert(RangeCheck(Rotation.Dim(), 2, 3));
	assert(RangeCheck(Translation.Dim(), 2, 3));
	assert(Rotation.Dim() == Translation.Dim());

	*this = Rotation;
	Dim(m_dim + 1);
	for(int row = 0; row < Translation.Dim(); row++)
		m[row][m_dim - 1] = Translation[row];
} // cMatrix(const cMatrix &, const cVector &)

// ~cMatrix
inline cMatrix::~cMatrix() {
} // ~cMatrix

// cMatrix::operator ()
inline double & cMatrix::operator () (int row, int col) {
	assert(row >= 0 && row < m_dim && col >= 0 && col < m_dim);
	return m[row][col];
} // cMatrix::operator ()

// cMatrix::operator () const
inline const double & cMatrix::operator () (int row, int col) const {
	assert(row >= 0 && row < m_dim && col >= 0 && col < m_dim);
	return m[row][col];
} // cMatrix::operator () const

// cMatrix::Equals
inline bool cMatrix::Equals(const cMatrix &M, const double Epsilon) const {
	if(m_dim != M.m_dim) return false;
	for(int row = 0; row < m_dim; row++)
		for(int col = 0; col < m_dim; col++) {
			if(!cUtils::AreEquals(m[row][col], M.m[row][col], Epsilon)) return false;
		}
	return true;
} // cMatrix::Equals

// cMatrix::operator ==
inline bool cMatrix::operator == (const cMatrix &M) const {
	return Equals(M);
} // cMatrix::operator ==

// cMatrix::operator !=
inline bool cMatrix::operator != (const cMatrix &M) const {
	return !Equals(M);
} // cMatrix::operator !=

// cMatrix::operator +
inline cMatrix cMatrix::operator + () const {
	return *this;
} // cMatrix::operator +

// cMatrix::operator -
inline cMatrix cMatrix::operator - () const {
	cMatrix M;
	for(int row = 0; row < m_dim; row++)
		for(int col = 0; col < m_dim; col++) {
			M.m[row][col] = - m[row][col];
		}
	M.m_dim = m_dim;
	return M;
} // cMatrix::operator -

// cMatrix::operator * (double) const
inline cMatrix cMatrix::operator * (double c) const {
	cMatrix M;
	for(int row = 0; row < m_dim; row++)
		for(int col = 0; col < m_dim; col++) {
			M.m[row][col] = c * m[row][col];
		}
	M.m_dim = m_dim;
	return M;
} // cMatrix::operator * (double) const

// cMatrix::operator *= (double)
inline cMatrix & cMatrix::operator *= (double c) {
	for(int row = 0; row < m_dim; row++)
		for(int col = 0; col < m_dim; col++) {
			m[row][col] *= c;
		}
	return *this;
} // cMatrix::operator *= (double)

// cMatrix::operator / (double) const
inline cMatrix cMatrix::operator / (double c) const {
	assert(cUtils::NotZero(c));
	double i = 1.0 / c;
	cMatrix M;
	for(int row = 0; row < m_dim; row++)
		for(int col = 0; col < m_dim; col++) {
			M.m[row][col] = i * m[row][col];
		}
	M.m_dim = m_dim;
	return M;
} // cMatrix::operator / (double) const

// cMatrix::operator /= (double)
inline cMatrix & cMatrix::operator /= (double c) {
	assert(cUtils::NotZero(c));
	double i = 1.0 / c;
	for(int row = 0; row < m_dim; row++)
		for(int col = 0; col < m_dim; col++) {
			m[row][col] *= i;
		}
	return *this;
} // cMatrix::operator /= (double)

// operator * (double, const cMatrix &)
inline cMatrix operator * (double c, const cMatrix &M) {
	return M * c;
} // operator * (double, const cMatrix &)

// cMatrix::operator + (const cMatrix &) const
inline cMatrix cMatrix::operator + (const cMatrix &M) const {
	cMatrix R;
	int nDim = Max(m_dim, M.m_dim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			R.m[row][col] = m[row][col] + M.m[row][col];
		}
	R.m_dim = nDim;
	return R;
} // cMatrix::operator + (const cMatrix &) const

// cMatrix::operator += (const cMatrix &)
inline cMatrix & cMatrix::operator += (const cMatrix &M) {
	int nDim = Max(m_dim, M.m_dim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			m[row][col] += M.m[row][col];
		}
	m_dim = nDim;
	return *this;
} // cMatrix::operator += (const cMatrix &)

// cMatrix::operator - (const cMatrix &) const
inline cMatrix cMatrix::operator - (const cMatrix &M) const {
	cMatrix R;
	int nDim = Max(m_dim, M.m_dim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			R.m[row][col] = m[row][col] - M.m[row][col];
		}
	R.m_dim = nDim;
	return R;
} // cMatrix::operator - (const cMatrix &) const

// cMatrix::operator -= (const cMatrix &)
inline cMatrix & cMatrix::operator -= (const cMatrix &M) {
	int nDim = Max(m_dim, M.m_dim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			m[row][col] -= M.m[row][col];
		}
	m_dim = nDim;
	return *this;
} // cMatrix::operator -= (const cMatrix &)

// cMatrix::operator * (const cMatrix &) const
inline cMatrix cMatrix::operator * (const cMatrix &M) const {
	cMatrix R;
	cMatrix A(*this), B(M);
	int nDim = Max(A.m_dim, B.m_dim);
	A.Dim(nDim);
	B.Dim(nDim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			double &s = R.m[row][col];
			for(int i = 0; i < nDim; i++) {
				s += A.m[row][i] * B.m[i][col];
			}
		}
	R.m_dim = nDim;
	return R;

} // cMatrix::operator * (const cMatrix &) const

// cMatrix::operator *= (const cMatrix &)
inline cMatrix & cMatrix::operator *= (const cMatrix &M) {
	return *this = *this * M;
} // cMatrix::operator *= (const cMatrix &)

// cMatrix::operator * (const cVector &) const
inline cVector cMatrix::operator * (const cVector &u) const {
	assert(u.Dim() <= m_dim);

	int nDim = u.Dim();
	if(nDim < m_dim) {
		nDim = m_dim - 1;
	}

	double r[3] = {0.0, 0.0, 0.0};
	for(int row = 0; row < nDim; row++) {
		double &s = r[row];
		for(int i = 0; i < u.Dim(); i++) {
			s += m[row][i] * u[i];
		}
	}
	cVector v(nDim, r);

	if(nDim < m_dim) {
		for(row = 0; row < nDim; row++) {
			v[row] += m[row][m_dim - 1];
		}
		double s = 1.0;
		for(int i = 0; i < u.Dim(); i++) {
			s += m[m_dim - 1][i] * u[i];
		}
		if(!cUtils::AreEquals(s, 1.0)) v /= s;
	}
	return v;
} // cMatrix::operator * (const cVector &) const

// operator * (const cVector &, const cMatrix &)
inline cVector operator * (const cVector &u, const cMatrix &M) {
	return M * u;
} // operator * (const cVector &, const cMatrix &)

// operator *= (cVector &, const cMatrix &)
inline cVector & operator *= (cVector &u, const cMatrix &M) {
	return u = M * u;
} // operator *= (cVector &, const cMatrix &)

// cMatrix::LoadZero
inline void cMatrix::LoadZero(int nDim) {
	if(nDim < 2 || nDim > 4) nDim = m_dim;
	switch(nDim) {
		case 2: *this = Zero2D;
			break;
		case 3: *this = Zero3D;
			break;
		case 4: *this = Zero4D;
			break;
		default: assert(nDim >= 2 && nDim <= 4);
	}
} // cMatrix::LoadZero

// cMatrix::LoadIdentity
inline void cMatrix::LoadIdentity(int nDim) {
	if(nDim < 2 || nDim > 4) nDim = m_dim;
	switch(nDim) {
		case 2: *this = Identity2D;
			break;
		case 3: *this = Identity3D;
			break;
		case 4: *this = Identity4D;
			break;
		default: assert(nDim >= 2 && nDim <= 4);
	}
} // cMatrix::LoadIdentity

// cMatrix::IsZero
inline bool cMatrix::IsZero(const double Epsilon) const {
	switch(m_dim) {
		case 2: return Equals(Zero2D, Epsilon);
		case 3: return Equals(Zero3D, Epsilon);
		case 4: return Equals(Zero4D, Epsilon);
		default: assert(m_dim >= 2 && m_dim <= 4);
			return false;
	}
} // cMatrix::IsZero

// cMatrix::IsIdentity
inline bool cMatrix::IsIdentity(const double Epsilon) const {
	switch(m_dim) {
		case 2: return Equals(Identity2D, Epsilon);
		case 3: return Equals(Identity3D, Epsilon);
		case 4: return Equals(Identity4D, Epsilon);
		default: assert(m_dim >= 2 && m_dim <= 4);
			return false;
	}
} // cMatrix::IsIdentity

// cMatrix::IsSymmetric
inline bool cMatrix::IsSymmetric(const double Epsilon) const {
	switch(m_dim) {
		case 4:
			if(!cUtils::AreEquals(m[3][0], m[0][3], Epsilon)) return false;
			if(!cUtils::AreEquals(m[3][1], m[1][3], Epsilon)) return false;
			if(!cUtils::AreEquals(m[3][2], m[2][3], Epsilon)) return false;
		case 3:
			if(!cUtils::AreEquals(m[2][0], m[0][2], Epsilon)) return false;
			if(!cUtils::AreEquals(m[2][1], m[1][2], Epsilon)) return false;
		case 2:
			if(!cUtils::AreEquals(m[1][0], m[0][1], Epsilon)) return false;
			return true;
		default: assert(m_dim >= 2 && m_dim <= 4);
			return false;
	}
} // cMatrix::IsSymmetric

// cMatrix::CalcTrace
inline double cMatrix::CalcTrace() const {
	double t = 0.0;
	for(int i = 0; i < m_dim; i++)
		t += m[i][i];
	return t;
} // cMatrix::CalcTrace

// cMatrix::Inverse
inline cMatrix cMatrix::Inverse() const {
	cMatrix M(*this);
	bool r = M.Invert();
	assert(r);
	return M;
} // cMatrix::Inverse

} // comms

#endif // __cMatrix__