#pragma once

typedef enum { CSLeftHanded, CSRightHanded } eCoordSystemType;

class dMatrix {
protected:
	int m_Dim; // 2(D - 2x2), 3(D - 3x3), 4(D - 4x4)
public:
	double m[4][4]; // [row][column]

	// Constructors:
	dMatrix(); // Default constructor.
	void Copy(const dMatrix &Src);
	dMatrix & operator = (const dMatrix &src); // Assignment operator.
	dMatrix(const dMatrix &src); // Copy - constructor.
	// 2D (2x2):
	dMatrix(double m00, double m01,
		double m10, double m11);
	explicit dMatrix(const double src[2][2]);
	dMatrix(const dVector &xAxis, const dVector &yAxis);
	// 3D (3x3):
	dMatrix(double m00, double m01, double m02,
		double m10, double m11, double m12,
		double m20, double m21, double m22);
	explicit dMatrix(const double src[3][3]);
	dMatrix(const dVector &xAxis, const dVector &yAxis, const dVector &zAxis);
	// 4D (4x4);
	dMatrix(double m00, double m01, double m02, double m03,
		double m10, double m11, double m12, double m13,
		double m20, double m21, double m22, double m23,
		double m30, double m31, double m32, double m33);
	explicit dMatrix(const double src[4][4]);
	// 3D or 4D:
	dMatrix(const dMatrix &R, const dVector &T);

	~dMatrix(); // Destructor.

	// Number of dimensions property:
	int Dim() const;
	int Dim(int nDim);

	// Reference indexing operator:
	double & operator () (int row, int col);
	// Indexing operator:
	const double & operator () (int row, int col) const;

	// Comparators (epsilon based):
	bool Equals(const dMatrix &, const double = cMath::dEpsilon) const;
	bool operator == (const dMatrix &) const;
	bool operator != (const dMatrix &) const;

	// Unari operators:
	dMatrix operator + () const;
	dMatrix operator - () const;

	// Matrix arithmetic:
	dMatrix operator * (double) const;
	dMatrix & operator *= (double);
	dMatrix operator / (double) const;
	dMatrix & operator /= (double);
	friend dMatrix operator * (double, const dMatrix &);
	dMatrix operator + (const dMatrix &) const;
	dMatrix & operator += (const dMatrix &);
	dMatrix operator - (const dMatrix &) const;
	dMatrix & operator -= (const dMatrix &);
	dMatrix operator * (const dMatrix &) const;
	dMatrix & operator *= (const dMatrix &);
	dVector operator * (const dVector &) const;
	friend dVector operator * (const dVector &, const dMatrix &);
	friend dVector & operator *= (dVector &, const dMatrix &);

	// Constants:
	// (2x2):
	static const dMatrix Zero2D;
	static const dMatrix Identity2D;
	// (3x3):
	static const dMatrix Zero3D;
	static const dMatrix Identity3D;
	// (4x4):
	static const dMatrix Zero4D;
	static const dMatrix Identity4D;

	// Specific operations:
	void LoadZero(int nDim = -1);
	void LoadIdentity(int nDim = -1);
	bool IsZero(const double Epsilon = cMath::dEpsilon) const;
	bool IsIdentity(const double Epsilon = cMath::dEpsilon) const;
	bool IsSymmetric(const double Epsilon = cMath::dEpsilon) const;
	double CalcTrace() const;
	double CalcDeterminant() const;
	bool Invert();
	dMatrix Inverse() const;
	dMatrix &  Transpose();
	dMatrix Transposed() const;
	friend cStr & operator << (cStr &, const dMatrix &);
	
	static dMatrix OfTranslation(const dVector &);
	static dMatrix OfRotation(const dVector &Axis, double Angle); // Returns 3D (3x3) matrix of rotation.
	static dMatrix OfRotation(double Yaw, double Pitch, double Roll);
	static dMatrix OfRotation(double Angle); // Returns 2D (2x2) matrix of rotation.
	static dMatrix OfScale(const dVector &);
	static dMatrix OfMirror(const dVector &);
	static dMatrix OfTransform(double SinA, double CosA, double SinB, double CosB, const dVector &Pos);
	static dMatrix OfTransform(double Alpha, double Beta, const dVector &Pos);
	static dMatrix OfTransform(const dVector &Dir, const dVector &Pos);
	static dMatrix OfLookAt(const dVector &Eye, const dVector &At, const dVector &Up, eCoordSystemType Type = CSLeftHanded);
	static dMatrix OfLookAt(const dVector &At, double Azim, double Elev, double toEye, eCoordSystemType Type = CSLeftHanded);
	static dMatrix OfPerspective(double c);

	static dMatrix OfProjection(double Fov, double AspectWtoH, double zNear, double zFar);
	static dMatrix OfInfiniteProjection(double Fov, double AspectWtoH, double zNear);

	bool EigenSolve(doubles &EigenValues, dVectors &EigenVectors);
	bool EigenSolve(double &EigenValue, dVector &EigenVector) const; // Returns greatest eigen value of symmetric matrix and proper eigen vector.
};

// dMatrix
inline dMatrix::dMatrix() : m_Dim(4) {
	ZeroMemory(m, sizeof(m));
} // dMatrix

//-----------------------------------------------------------------------------
// dMatrix::Copy
//-----------------------------------------------------------------------------
inline void dMatrix::Copy(const dMatrix &Src) {
	CopyMemory(m, Src.m, sizeof(m));
	m_Dim = Src.m_Dim;
} // dMatrix::Copy

//-----------------------------------------------------------------------------
// dMatrix::operator =
//-----------------------------------------------------------------------------
inline dMatrix & dMatrix::operator = (const dMatrix &src) {
	Copy(src);
	return *this;
} // dMatrix::operator =

//-----------------------------------------------------------------------------
// dMatrix.ctor : void(const dMatrix &)
//-----------------------------------------------------------------------------
inline dMatrix::dMatrix(const dMatrix &src) {
	Copy(src);
} // dMatrix.ctor : void(const dMatrix &)

// dMatrix(2 x 2 doubles)
inline dMatrix::dMatrix(double m00, double m01,
						double m10, double m11) : m_Dim(2) {
	ZeroMemory(m, sizeof(m));
	m[0][0] = m00, m[0][1] = m01;
	m[1][0] = m10, m[1][1] = m11;
} // dMatrix(2 x 2 doubles)

// dMatrix(array 2 x 2 of doubles)
inline dMatrix::dMatrix(const double src[2][2]) : m_Dim(2) {
	ZeroMemory(m, sizeof(m));
	m[0][0] = src[0][0], m[0][1] = src[0][1];
	m[1][0] = src[1][0], m[1][1] = src[1][1];
} // dMatrix(array 2 x 2 of doubles)

// dMatrix(const dVector &, const dVector &)
inline dMatrix::dMatrix(const dVector &xAxis, const dVector &yAxis) : m_Dim(2) {
	assert(xAxis.Dim() == 2);
	assert(yAxis.Dim() == 2);

	ZeroMemory(m, sizeof(m));
	m[0][0] = xAxis.x, m[0][1] = xAxis.y;
	m[1][0] = yAxis.x, m[1][1] = yAxis.y;
} // dMatrix(const dVector &, const dVector &))

// dMatrix(3 x 3 doubles)
inline dMatrix::dMatrix(double m00, double m01, double m02,
						double m10, double m11, double m12,
						double m20, double m21, double m22) : m_Dim(3) {
	ZeroMemory(m, sizeof(m));
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02;
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12;
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22;
} // dMatrix(3 x 3 doubles)

// dMatrix(array 3 x 3 of doubles)
inline dMatrix::dMatrix(const double src[3][3]) : m_Dim(3) {
	ZeroMemory(m, sizeof(m));
	m[0][0] = src[0][0], m[0][1] = src[0][1], m[0][2] = src[0][2];
	m[1][0] = src[1][0], m[1][1] = src[1][1], m[1][2] = src[1][2];
	m[2][0] = src[2][0], m[2][1] = src[2][1], m[2][2] = src[2][2];
} // dMatrix(array 3 x 3 of doubles)

//----------------------------------------------------------------------------------------------------
// dMatrix.ctor : void(const dVector &, const dVector &, const dVector &)
//----------------------------------------------------------------------------------------------------
inline dMatrix::dMatrix(const dVector &xAxis, const dVector &yAxis, const dVector &zAxis) : m_Dim(3) {
	assert(xAxis.Dim() == 3);
	assert(yAxis.Dim() == 3);
	assert(zAxis.Dim() == 3);

	ZeroMemory(m, sizeof(m));
	m[0][0] = xAxis.x, m[0][1] = yAxis.x, m[0][2] = zAxis.x;
	m[1][0] = xAxis.y, m[1][1] = yAxis.y, m[1][2] = zAxis.y;
	m[2][0] = xAxis.z, m[2][1] = yAxis.z, m[2][2] = zAxis.z;
} // dMatrix.ctor : void(const dVector &, const dVector &, const dVector &)

// dMatrix(4 x 4 doubles)
inline dMatrix::dMatrix(double m00, double m01, double m02, double m03,
						double m10, double m11, double m12, double m13,
						double m20, double m21, double m22, double m23,
						double m30, double m31, double m32, double m33) : m_Dim(4) {
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03;
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13;
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23;
	m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;
} // dMatrix(4 x 4 doubles)

// dMatrix(array 4 x 4 of doubles)
inline dMatrix::dMatrix(const double src[4][4]) : m_Dim(4) {
	CopyMemory(m, src, sizeof(m));
} // dMatrix(array 4 x 4 of doubles)

// dMatrix::Dim() const
inline int dMatrix::Dim() const {
	return m_Dim;
} // dMatrix::Dim() const

// dMatrix::Dim(int)
inline int dMatrix::Dim(int nDim) {
	assert(cMath::IsInRange(nDim, 2, 4));
	if(!cMath::IsInRange(nDim, 2, 4) || m_Dim == nDim) return m_Dim;

	switch(nDim) {
		case 2: m[2][0] = m[0][2] = m[2][1] = m[1][2] = m[2][2] = 0.0;
			if(m_Dim == 3) break;
		case 4: m[3][3] = 1.0;
			if(m_Dim == 3) break;
		case 3:
			if(m_Dim == 4) {
				m[3][0] = m[0][3] = m[3][1] = m[1][3] = m[3][2] = m[2][3] = m[3][3] = 0.0;
				break;
			}
			m[2][2] = 1.0;
	}
	return m_Dim = nDim;
} // dMatrix::Dim(int)

//-----------------------------------------------------------------------------
// dMatrix.ctor : void(const dMatrix &, const dVector &)
//-----------------------------------------------------------------------------
inline dMatrix::dMatrix(const dMatrix &R, const dVector &T) {
	assert(R.Dim() == T.Dim() && (R.Dim() == 2 || R.Dim() == 3));
	Copy(R);
	Dim(m_Dim + 1);
	for(int col = 0; col < T.Dim(); col++) {
		m[m_Dim - 1][col] = T[col];
	}
} // dMatrix.ctor : void(const dMatrix &, const dVector &)

// ~dMatrix
inline dMatrix::~dMatrix() {
} // ~dMatrix

// dMatrix::operator ()
inline double & dMatrix::operator () (int row, int col) {
	assert(row >= 0 && row < m_Dim && col >= 0 && col < m_Dim);
	return m[row][col];
} // dMatrix::operator ()

// dMatrix::operator () const
inline const double & dMatrix::operator () (int row, int col) const {
	assert(row >= 0 && row < m_Dim && col >= 0 && col < m_Dim);
	return m[row][col];
} // dMatrix::operator () const

// dMatrix::Equals
inline bool dMatrix::Equals(const dMatrix &M, const double Epsilon) const {
	if(m_Dim != M.m_Dim) return false;
	for(int row = 0; row < m_Dim; row++)
		for(int col = 0; col < m_Dim; col++) {
			if(!cMath::Equals(m[row][col], M.m[row][col], Epsilon)) return false;
		}
	return true;
} // dMatrix::Equals

// dMatrix::operator ==
inline bool dMatrix::operator == (const dMatrix &M) const {
	return Equals(M);
} // dMatrix::operator ==

// dMatrix::operator !=
inline bool dMatrix::operator != (const dMatrix &M) const {
	return !Equals(M);
} // dMatrix::operator !=

// dMatrix::operator +
inline dMatrix dMatrix::operator + () const {
	return *this;
} // dMatrix::operator +

// dMatrix::operator -
inline dMatrix dMatrix::operator - () const {
	dMatrix M;
	for(int row = 0; row < m_Dim; row++)
		for(int col = 0; col < m_Dim; col++) {
			M.m[row][col] = - m[row][col];
		}
	M.m_Dim = m_Dim;
	return M;
} // dMatrix::operator -

// dMatrix::operator * (double) const
inline dMatrix dMatrix::operator * (double c) const {
	dMatrix M;
	for(int row = 0; row < m_Dim; row++)
		for(int col = 0; col < m_Dim; col++) {
			M.m[row][col] = c * m[row][col];
		}
	M.m_Dim = m_Dim;
	return M;
} // dMatrix::operator * (double) const

// dMatrix::operator *= (double)
inline dMatrix & dMatrix::operator *= (double c) {
	for(int row = 0; row < m_Dim; row++)
		for(int col = 0; col < m_Dim; col++) {
			m[row][col] *= c;
		}
	return *this;
} // dMatrix::operator *= (double)

// dMatrix::operator / (double) const
inline dMatrix dMatrix::operator / (double c) const {
	assert(!cMath::IsZero(c));
	double i = 1.0 / c;
	dMatrix M;
	for(int row = 0; row < m_Dim; row++)
		for(int col = 0; col < m_Dim; col++) {
			M.m[row][col] = i * m[row][col];
		}
	M.m_Dim = m_Dim;
	return M;
} // dMatrix::operator / (double) const

// dMatrix::operator /= (double)
inline dMatrix & dMatrix::operator /= (double c) {
	assert(!cMath::IsZero(c));
	double i = 1.0 / c;
	for(int row = 0; row < m_Dim; row++)
		for(int col = 0; col < m_Dim; col++) {
			m[row][col] *= i;
		}
	return *this;
} // dMatrix::operator /= (double)

// operator * (double, const dMatrix &)
inline dMatrix operator * (double c, const dMatrix &M) {
	return M * c;
} // operator * (double, const dMatrix &)

// dMatrix::operator + (const dMatrix &) const
inline dMatrix dMatrix::operator + (const dMatrix &M) const {
	dMatrix R;
	int nDim = cMath::Max(m_Dim, M.m_Dim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			R.m[row][col] = m[row][col] + M.m[row][col];
		}
	R.m_Dim = nDim;
	return R;
} // dMatrix::operator + (const dMatrix &) const

// dMatrix::operator += (const dMatrix &)
inline dMatrix & dMatrix::operator += (const dMatrix &M) {
	int nDim = cMath::Max(m_Dim, M.m_Dim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			m[row][col] += M.m[row][col];
		}
	m_Dim = nDim;
	return *this;
} // dMatrix::operator += (const dMatrix &)

// dMatrix::operator - (const dMatrix &) const
inline dMatrix dMatrix::operator - (const dMatrix &M) const {
	dMatrix R;
	int nDim = cMath::Max(m_Dim, M.m_Dim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			R.m[row][col] = m[row][col] - M.m[row][col];
		}
	R.m_Dim = nDim;
	return R;
} // dMatrix::operator - (const dMatrix &) const

// dMatrix::operator -= (const dMatrix &)
inline dMatrix & dMatrix::operator -= (const dMatrix &M) {
	int nDim = cMath::Max(m_Dim, M.m_Dim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			m[row][col] -= M.m[row][col];
		}
	m_Dim = nDim;
	return *this;
} // dMatrix::operator -= (const dMatrix &)

// dMatrix::operator * (const dMatrix &) const
inline dMatrix dMatrix::operator * (const dMatrix &M) const {
	dMatrix R;
	dMatrix A(*this), B(M);
	int nDim = cMath::Max(A.m_Dim, B.m_Dim);
	A.Dim(nDim);
	B.Dim(nDim);
	for(int row = 0; row < nDim; row++)
		for(int col = 0; col < nDim; col++) {
			double &s = R.m[row][col];
			for(int i = 0; i < nDim; i++) {
				s += A.m[row][i] * B.m[i][col];
			}
		}
	R.m_Dim = nDim;
	return R;

} // dMatrix::operator * (const dMatrix &) const

// dMatrix::operator *= (const dMatrix &)
inline dMatrix & dMatrix::operator *= (const dMatrix &M) {
	return *this = *this * M;
} // dMatrix::operator *= (const dMatrix &)

// dMatrix::operator * (const dVector &) const
inline dVector dMatrix::operator * (const dVector &u) const {
	assert(u.Dim() <= m_Dim);

	int nDim = u.Dim();
	if(nDim < m_Dim) {
		nDim = m_Dim - 1;
	}

	double r[3] = {0.0, 0.0, 0.0};
	for(int row = 0; row < nDim; row++) {
		double &s = r[row];
		for(int i = 0; i < u.Dim(); i++) {
			s += m[row][i] * u[i];
		}
	}
	dVector v(nDim, r);

	if(nDim < m_Dim) {
		for(row = 0; row < nDim; row++) {
			v[row] += m[row][m_Dim - 1];
		}
		double s = 1.0;
		for(int i = 0; i < u.Dim(); i++) {
			s += m[m_Dim - 1][i] * u[i];
		}
		if(!cMath::Equals(s, 1.0)) v /= s;
	}
	return v;
} // dMatrix::operator * (const dVector &) const

// operator * (const dVector &, const dMatrix &)
inline dVector operator * (const dVector &u, const dMatrix &M) {
	return M * u;
} // operator * (const dVector &, const dMatrix &)

// operator *= (dVector &, const dMatrix &)
inline dVector & operator *= (dVector &u, const dMatrix &M) {
	return u = M * u;
} // operator *= (dVector &, const dMatrix &)

// dMatrix::LoadZero
inline void dMatrix::LoadZero(int nDim) {
	if(nDim < 2 || nDim > 4) nDim = m_Dim;
	switch(nDim) {
		case 2: *this = Zero2D;
			break;
		case 3: *this = Zero3D;
			break;
		case 4: *this = Zero4D;
			break;
		default: assert(nDim >= 2 && nDim <= 4);
	}
} // dMatrix::LoadZero

// dMatrix::LoadIdentity
inline void dMatrix::LoadIdentity(int nDim) {
	if(nDim < 2 || nDim > 4) nDim = m_Dim;
	switch(nDim) {
		case 2: *this = Identity2D;
			break;
		case 3: *this = Identity3D;
			break;
		case 4: *this = Identity4D;
			break;
		default: assert(nDim >= 2 && nDim <= 4);
	}
} // dMatrix::LoadIdentity

// dMatrix::IsZero
inline bool dMatrix::IsZero(const double Epsilon) const {
	switch(m_Dim) {
		case 2: return Equals(Zero2D, Epsilon);
		case 3: return Equals(Zero3D, Epsilon);
		case 4: return Equals(Zero4D, Epsilon);
		default: assert(m_Dim >= 2 && m_Dim <= 4);
			return false;
	}
} // dMatrix::IsZero

// dMatrix::IsIdentity
inline bool dMatrix::IsIdentity(const double Epsilon) const {
	switch(m_Dim) {
		case 2: return Equals(Identity2D, Epsilon);
		case 3: return Equals(Identity3D, Epsilon);
		case 4: return Equals(Identity4D, Epsilon);
		default: assert(m_Dim >= 2 && m_Dim <= 4);
			return false;
	}
} // dMatrix::IsIdentity

// dMatrix::IsSymmetric
inline bool dMatrix::IsSymmetric(const double Epsilon) const {
	switch(m_Dim) {
		case 4:
			if(!cMath::Equals(m[3][0], m[0][3], Epsilon)) return false;
			if(!cMath::Equals(m[3][1], m[1][3], Epsilon)) return false;
			if(!cMath::Equals(m[3][2], m[2][3], Epsilon)) return false;
		case 3:
			if(!cMath::Equals(m[2][0], m[0][2], Epsilon)) return false;
			if(!cMath::Equals(m[2][1], m[1][2], Epsilon)) return false;
		case 2:
			if(!cMath::Equals(m[1][0], m[0][1], Epsilon)) return false;
			return true;
		default: assert(m_Dim >= 2 && m_Dim <= 4);
			return false;
	}
} // dMatrix::IsSymmetric

// dMatrix::CalcTrace
inline double dMatrix::CalcTrace() const {
	double t = 0.0;
	for(int i = 0; i < m_Dim; i++)
		t += m[i][i];
	return t;
} // dMatrix::CalcTrace

// dMatrix::Inverse
inline dMatrix dMatrix::Inverse() const {
	dMatrix M(*this);
	bool r = M.Invert();
	assert(r);
	return M;
} // dMatrix::Inverse