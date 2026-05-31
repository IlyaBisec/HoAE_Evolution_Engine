#pragma once

//*****************************************************************************
// cMat3
//*****************************************************************************
class cMat3 {
public:
	enum eZeroCtor		{ ZeroCtor };
	enum eIdentityCtor	{ IdentityCtor };
	enum eRowsCtor		{ RowsCtor };
	enum eColsCtor		{ ColsCtor };
	enum eDiagonalCtor	{ DiagonalCtor };
	
	cMat3();
	cMat3(const eZeroCtor);
	cMat3(const eIdentityCtor);
	cMat3(const eRowsCtor, const cVec3 &Row0, const cVec3 &Row1, const cVec3 &Row2);
	cMat3(const eColsCtor, const cVec3 &Col0, const cVec3 &Col1, const cVec3 &Col2);
	cMat3(const eDiagonalCtor, const cVec3 &S);
	cMat3(const eDiagonalCtor, const float Sx, const float Sy, const float Sz);
	cMat3(const float _00, const float _01, const float _02,
		const float _10, const float _11, const float _12,
		const float _20, const float _21, const float _22);
	
	cMat3 & Copy(const float *pSrc);
	cMat3 & CopyTransposed(const float *pSrc);

	void SetZero();
	void SetIdentity();

	const cVec3 & GetRow(const int index) const;
	const cVec3 & GetRow0() const;
	const cVec3 & GetRow1() const;
	const cVec3 & GetRow2() const;
	
	cVec3 & Row(const int index);
	cVec3 & Row0();
	cVec3 & Row1();
	cVec3 & Row2();

	void SetRow(const int index, const cVec3 &);
	void SetRow0(const cVec3 &);
	void SetRow1(const cVec3 &);
	void SetRow2(const cVec3 &);

	void SetRow(const int index, const float x, const float y, const float z);
	void SetRow0(const float x, const float y, const float z);
	void SetRow1(const float x, const float y, const float z);
	void SetRow2(const float x, const float y, const float z);

	const cVec3 GetCol(const int index) const;
	const cVec3 GetCol0() const;
	const cVec3 GetCol1() const;
	const cVec3 GetCol2() const;

	void SetCol(const int index, const cVec3 &);
	void SetCol0(const cVec3 &);
	void SetCol1(const cVec3 &);
	void SetCol2(const cVec3 &);

	void SetCol(const int index, const float x, const float y, const float z);
	void SetCol0(const float x, const float y, const float z);
	void SetCol1(const float x, const float y, const float z);
	void SetCol2(const float x, const float y, const float z);
	
	float GetElem(const int row, const int col) const;
	float & Elem(const int row, const int col);

	const cVec3 & operator [] (const int row) const;
	cVec3 & operator [] (const int row);

	float operator () (const int row, const int col) const;
	float & operator () (const int row, const int col);

	const cVec3 GetDiagonal() const;
	void SetDiagonal(const cVec3 &);
	void SetToDiagonal(const cVec3 &);
	
	float GetTrace() const;
	float GetDeterminant() const;
	
	static bool Equals(const cMat3 &, const cMat3 &, const float Eps = cMath::MatrixEpsilon);
	
	bool IsZero(const float Eps = cMath::MatrixEpsilon) const;
	bool IsIdentity(const float Eps = cMath::MatrixEpsilon) const;
	bool IsSymmetric(const float Eps = cMath::MatrixEpsilon) const;
	bool IsDiagonal(const float Eps = cMath::MatrixEpsilon) const;
	bool IsRotated() const; // ?
	bool IsOrthonormal(const float Eps = cMath::MatrixEpsilon) const; // IsRotated
	bool IsOrthogonal(const float Eps = cMath::MatrixEpsilon) const; // IsRotated + IsScaled (No Shear)
	bool IsUniformScale(const float Eps = cMath::MatrixEpsilon) const;
	float GetUniformScale() const;
	float GetUniformScaleSq() const;

	const cVec3 GetScale() const;

	void Add(const cMat3 &R);
	void operator += (const cMat3 &R);
	static const cMat3 Add(const cMat3 &L, const cMat3 &R);
	const cMat3 operator + (const cMat3 &R) const;

	void Sub(const cMat3 &R);
	void operator -= (const cMat3 &R);
	static const cMat3 Sub(const cMat3 &L, const cMat3 &R);
	const cMat3 operator - (const cMat3 &R) const;
	
	void Mul(const cMat3 &R);
	void operator *= (const cMat3 &R);
	static const cMat3 Mul(const cMat3 &L, const cMat3 &R);
	const cMat3 operator * (const cMat3 &R) const;
	
	static const cMat3 Lerp(const cMat3 &, const cMat3 &, const float);
	static const cMat3 Lerp05(const cMat3 &, const cMat3 &, const float);
	
	static const cMat3 Zero;
	static const cMat3 Identity;

	static const cMat3 Transpose(const cMat3 &);
	void Transpose();
	
	static bool Invert(const cMat3 &Fm, cMat3 *To);
	bool Invert() { return Invert(*this, this); }
	
	static cMat3 OfTranslation(const cVec2 &);
	static cMat3 OfScale(const cVec2 &);
	static cMat3 OfRotation(const float Angle);

	static const cMat3 RotationX(const float Angle);
	static const cMat3 RotationY(const float Angle);
	static const cMat3 RotationZ(const float Angle);
	static const cMat3 RotationAxis(const cVec3 &Axis, const float Angle);
	static const cMat3 RotationZXY(const float Roll, const float Pitch, const float Yaw);
	static const cMat3 RotationZXZ(const float Roll, const float Pitch, const float Yaw);
	
	static const cMat3 OfRotation(const cVec3 &xToAxis, const cVec3 &yToAxis, const cVec3 &zToAxis,
		const cVec3 &xFmAxis = cVec3::AxisX, const cVec3 &yFmAxis = cVec3::AxisY, const cVec3 &zFmAxis = cVec3::AxisZ);


	static cMat3 OfFitToRect(const cVec2 &bbMin, const cVec2 &bbMax,
		const cVec2 &rcMin, const cVec2 &rcMax, bool xScale = true, bool yScale = true);


	const float * ToFloatPtr() const;
	float * ToFloatPtr();
	const cMat4 ToMat4() const;
	const cQuat ToQuat() const;

	void ToVectors(cVec3 *Forward, cVec3 *Right = NULL, cVec3 *Up = NULL) const;
	const cVec3 ToForward() const;

	cAngles ToAngles() const;

private:
	cVec3 m_Rows[3];
};

// cMat3.ctor : ()
inline cMat3::cMat3() {
}

// cMat3.ctor : (const eZeroCtor)
inline cMat3::cMat3(const eZeroCtor) {
	m_Rows[0].SetZero();
	m_Rows[1].SetZero();
	m_Rows[2].SetZero();
}

// cMat3.ctor : (const eIdentityCtor)
inline cMat3::cMat3(const eIdentityCtor) {
	m_Rows[0].Set(1.0f, 0.0f, 0.0f);
	m_Rows[1].Set(0.0f, 1.0f, 0.0f);
	m_Rows[2].Set(0.0f, 0.0f, 1.0f);
}

// cMat3.ctor : (const eRowsCtor, const cVec3 &, const cVec3 &, const cVec3 &)
inline cMat3::cMat3(const eRowsCtor, const cVec3 &Row0, const cVec3 &Row1, const cVec3 &Row2) {
	m_Rows[0] = Row0;
	m_Rows[1] = Row1;
	m_Rows[2] = Row2;
}

// cMat3.ctor : (const eColsCtor, const cVec3 &, const cVec3 &, const cVec3 &)
inline cMat3::cMat3(const eColsCtor, const cVec3 &Col0, const cVec3 &Col1, const cVec3 &Col2) {
	m_Rows[0].Set(Col0.x, Col1.x, Col2.x);
	m_Rows[1].Set(Col0.y, Col1.y, Col2.y);
	m_Rows[2].Set(Col0.z, Col1.z, Col2.z);
}

// cMat3.ctor : (const eDiagonalCtor, const cVec3 &)
inline cMat3::cMat3(const eDiagonalCtor, const cVec3 &S) {
	m_Rows[0].Set(S.x, 0.0f, 0.0f);
	m_Rows[1].Set(0.0f, S.y, 0.0f);
	m_Rows[2].Set(0.0f, 0.0f, S.z);
}

// cMat3.ctor : (const eDiagonalCtor, const float, const float, const float)
inline cMat3::cMat3(const eDiagonalCtor, const float Sx, const float Sy, const float Sz) {
	m_Rows[0].Set(Sx, 0.0f, 0.0f);
	m_Rows[1].Set(0.0f, Sy, 0.0f);
	m_Rows[2].Set(0.0f, 0.0f, Sz);
}

// cMat3.ctor
inline cMat3::cMat3(const float _00, const float _01, const float _02,
					const float _10, const float _11, const float _12,
					const float _20, const float _21, const float _22) {
	m_Rows[0].Set(_00, _01, _02);
	m_Rows[1].Set(_10, _11, _12);
	m_Rows[2].Set(_20, _21, _22);
}

// cMat3::GetRow : const cVec3 & (const int) const
inline const cVec3 & cMat3::GetRow(const int index) const {
	cAssert(index >= 0 && index < 3);
	return m_Rows[index];
}

// cMat3::GetRow0 : const cVec3 & () const
inline const cVec3 & cMat3::GetRow0() const {
	return m_Rows[0];
}

// cMat3::GetRow1 : const cVec3 & () const
inline const cVec3 & cMat3::GetRow1() const {
	return m_Rows[1];
}

// cMat3::GetRow2 : const cVec3 & () const
inline const cVec3 & cMat3::GetRow2() const {
	return m_Rows[2];
}

// cMat3::Row : cVec3 & (const int)
inline cVec3 & cMat3::Row(const int index) {
	cAssert(index >= 0 && index < 3);
	return m_Rows[index];
}

// cMat3::Row0 : cVec3 & ()
inline cVec3 & cMat3::Row0() {
	return m_Rows[0];
}

// cMat3::Row1 : cVec3 & ()
inline cVec3 & cMat3::Row1() {
	return m_Rows[1];
}

// cMat3::Row2 : cVec3 & ()
inline cVec3 & cMat3::Row2() {
	return m_Rows[2];
}

// cMat3::SetRow : void (const int, const cVec3 &)
inline void cMat3::SetRow(const int index, const cVec3 &r) {
	cAssert(index >= 0 && index < 3);
	m_Rows[index] = r;
}

// cMat3::SetRow0 : void (const cVec3 &)
inline void cMat3::SetRow0(const cVec3 &r) {
	m_Rows[0] = r;
}

// cMat3::SetRow1 : void (const cVec3 &)
inline void cMat3::SetRow1(const cVec3 &r) {
	m_Rows[1] = r;
}

// cMat3::SetRow2 : void (const cVec3 &)
inline void cMat3::SetRow2(const cVec3 &r) {
	m_Rows[2] = r;
}

// cMat3::SetRow : void (const int, const float, const float, const float)
inline void cMat3::SetRow(const int index, const float x, const float y, const float z) {
	cAssert(index >= 0 && index < 3);
	m_Rows[index].Set(x, y, z);
}

// cMat3::SetRow0 : void (const float, const float, const float)
inline void cMat3::SetRow0(const float x, const float y, const float z) {
	m_Rows[0].Set(x, y, z);
}

// cMat3::SetRow1 : void (const float, const float, const float)
inline void cMat3::SetRow1(const float x, const float y, const float z) {
	m_Rows[1].Set(x, y, z);
}

// cMat3::SetRow2 : void(const float, const float, const float)
inline void cMat3::SetRow2(const float x, const float y, const float z) {
	m_Rows[2].Set(x, y, z);
}

// cMat3::GetCol : const cVec3 (const int) const
inline const cVec3 cMat3::GetCol(const int index) const {
	cAssert(index >= 0 && index < 3);
	return cVec3(m_Rows[0][index], m_Rows[1][index], m_Rows[2][index]);
}

// cMat3::GetCol0 : const cVec3 () const
inline const cVec3 cMat3::GetCol0() const {
	return cVec3(m_Rows[0].x, m_Rows[1].x, m_Rows[2].x);
}

// cMat3::GetCol1 : const cVec3 () const
inline const cVec3 cMat3::GetCol1() const {
	return cVec3(m_Rows[0].y, m_Rows[1].y, m_Rows[2].y);
}

// cMat3::GetCol2 : const cVec3 () const
inline const cVec3 cMat3::GetCol2() const {
	return cVec3(m_Rows[0].z, m_Rows[1].z, m_Rows[2].z);
}

// cMat3::SetCol : void (const int, const cVec3 &)
inline void cMat3::SetCol(const int index, const cVec3 &u) {
	cAssert(index >= 0 && index < 3);
	m_Rows[0][index] = u.x;
	m_Rows[1][index] = u.y;
	m_Rows[2][index] = u.z;
}

// cMat3::SetCol0 : void (const cVec3 &)
inline void cMat3::SetCol0(const cVec3 &u) {
	m_Rows[0].x = u.x;
	m_Rows[1].x = u.y;
	m_Rows[2].x = u.z;
}

// cMat3::SetCol1 : void (const cVec3 &)
inline void cMat3::SetCol1(const cVec3 &u) {
	m_Rows[0].y = u.x;
	m_Rows[1].y = u.y;
	m_Rows[2].y = u.z;
}

// cMat3::SetCol2 : void (const cVec3 &)
inline void cMat3::SetCol2(const cVec3 &u) {
	m_Rows[0].z = u.x;
	m_Rows[1].z = u.y;
	m_Rows[2].z = u.z;
}

// cMat3::SetCol : void (const int, const float, const float, const float)
inline void cMat3::SetCol(const int index, const float x, const float y, const float z) {
	cAssert(index >= 0 && index < 3);
	m_Rows[0][index] = x;
	m_Rows[1][index] = y;
	m_Rows[2][index] = z;
}

// cMat3::SetCol0 : void (const float, const float, const float)
inline void cMat3::SetCol0(const float x, const float y, const float z) {
	m_Rows[0].x = x;
	m_Rows[1].x = y;
	m_Rows[2].x = z;
}

// cMat3::SetCol1 : void (const float, const float, const float)
inline void cMat3::SetCol1(const float x, const float y, const float z) {
	m_Rows[0].y = x;
	m_Rows[1].y = y;
	m_Rows[2].y = z;
}

// cMat3::SetCol2 : void (const float, const float, const float)
inline void cMat3::SetCol2(const float x, const float y, const float z) {
	m_Rows[0].z = x;
	m_Rows[1].z = y;
	m_Rows[2].z = z;
}

// cMat3::GetElem : float (const int, const int) const
inline float cMat3::GetElem(const int row, const int col) const {
	cAssert(row >= 0 && row < 3);
	cAssert(col >= 0 && col < 3);
	return m_Rows[row][col];
}

// cMat3::Elem : float & (const int, const int)
inline float & cMat3::Elem(const int row, const int col) {
	cAssert(row >= 0 && row < 3);
	cAssert(col >= 0 && col < 3);
	return m_Rows[row][col];
}

// cMat3::operator [] : const cVec3 & (const int) const
inline const cVec3 & cMat3::operator [] (const int row) const {
	cAssert(row >= 0 && row < 3);
	return m_Rows[row];
}

// cMat3::operator [] : cVec3 & (const int)
inline cVec3 & cMat3::operator [] (const int row) {
	cAssert(row >= 0 && row < 3);
	return m_Rows[row];
}

// cMat3::operator () : float (const int, const int) const
inline float cMat3::operator () (const int row, const int col) const {
	cAssert(row >= 0 && row < 3);
	cAssert(col >= 0 && col < 3);
	return m_Rows[row][col];
}

// cMat3::operator () : float & (const int, const int)
inline float & cMat3::operator () (const int row, const int col) {
	cAssert(row >= 0 && row < 3);
	cAssert(col >= 0 && col < 3);
	return m_Rows[row][col];
}

// cMat3::GetDiagonal : const cVec3 () const
inline const cVec3 cMat3::GetDiagonal() const {
	return cVec3(m_Rows[0].x, m_Rows[1].y, m_Rows[2].z);
}

// cMat3::Equals
inline bool cMat3::Equals(const cMat3 &P, const cMat3 &Q, const float Eps /*= cMath::MatrixEpsilon*/) {
	if(cVec3::Equals(P[0], Q[0]) && cVec3::Equals(P[1], Q[1]) && cVec3::Equals(P[2], Q[2])) {
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// cMat3::Add : void (const cMat3 &)
//-----------------------------------------------------------------------------
inline void cMat3::Add(const cMat3 &R) {
	float *l = reinterpret_cast<float *>(this);
	const float *r = reinterpret_cast<const float *>(&R);

	for(int i = 0; i < 9; i++) {
		*l++ += *r++;
	}
} // cMat3::Add

// cMat3::operator += : void (const cMat3 &)
inline void cMat3::operator += (const cMat3 &R) {
	Add(R);
}

//-----------------------------------------------------------------------------
// cMat3::Add : const cMat3 (const cMat3 &, const cMat3 &)
//-----------------------------------------------------------------------------
inline const cMat3 cMat3::Add(const cMat3 &L, const cMat3 &R) {
	const float *l = reinterpret_cast<const float *>(&L);
	const float *r = reinterpret_cast<const float *>(&R);
	cMat3 S;
	float *s = reinterpret_cast<float *>(&S);

	for(int i = 0; i < 9; i++) {
		*s++ = *l++ + *r++;
	}
	return S;
}

// cMat3::operator + : const cMat3 (const cMat3 &) const
inline const cMat3 cMat3::operator + (const cMat3 &R) const {
	return Mul(*this, R);
}

//-----------------------------------------------------------------------------
// cMat3::Sub : void (const cMat3 &)
//-----------------------------------------------------------------------------
inline void cMat3::Sub(const cMat3 &R) {
	float *l = reinterpret_cast<float *>(this);
	const float *r = reinterpret_cast<const float *>(&R);

	for(int i = 0; i < 9; i++) {
		*l++ -= *r++;
	}
} // cMat3::Sub

// cMat3::operator -= : void (const cMat3 &)
inline void cMat3::operator -= (const cMat3 &R) {
	Sub(R);
}

//-----------------------------------------------------------------------------
// cMat3::Sub : const cMat3 (const cMat3 &, const cMat3 &)
//-----------------------------------------------------------------------------
inline const cMat3 cMat3::Sub(const cMat3 &L, const cMat3 &R) {
	const float *l = reinterpret_cast<const float *>(&L);
	const float *r = reinterpret_cast<const float *>(&R);
	cMat3 D;
	float *d = reinterpret_cast<float *>(&D);

	for(int i = 0; i < 9; i++) {
		*d++ = *l++ - *r++;
	}
	return D;
} // cMat3::Sub

inline const cMat3 cMat3::operator - (const cMat3 &R) const {
	return Sub(*this, R);
}

//-----------------------------------------------------------------------------
// cMat3::Mul : void (const cMat3 &)
//-----------------------------------------------------------------------------
inline void cMat3::Mul(const cMat3 &R) {
	float *l = reinterpret_cast<float *>(this);
	const float *r = reinterpret_cast<const float *>(&R);
	
	float t[3];
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			t[j] = l[0] * r[0 * 3 + j] + l[1] * r[1 * 3 + j] + l[2] * r[2 * 3 + j];
		}
		l[0] = t[0];
		l[1] = t[1];
		l[2] = t[2];
		l += 3;
	}
} // cMat3::Mul

// cMat3::operator *= : void (const cMat3 &)
inline void cMat3::operator *= (const cMat3 &R) {
	Mul(R);
}

//-----------------------------------------------------------------------------
// cMat3::Mul : const cMat3 (const cMat3 &, const cMat3 &)
//-----------------------------------------------------------------------------
inline const cMat3 cMat3::Mul(const cMat3 &L, const cMat3 &R) {
	cMat3 M;
	const float *l = reinterpret_cast<const float *>(&L);
	const float *r = reinterpret_cast<const float *>(&R);
	float *m = reinterpret_cast<float *>(&M);
	
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			*m = l[0] * r[0 * 3 + j] + l[1] * r[1 * 3 + j] + l[2] * r[2 * 3 + j];
			m++;
		}
		l += 3;
	}
	return M;
} // cMat3::Mul

// cMat3::operator * : const cMat3 (const cMat3 &) const
inline const cMat3 cMat3::operator * (const cMat3 &R) const {
	return Mul(*this, R);
}

//-----------------------------------------------------------------------------
// cMat3::IsZero : bool(const float) const
//-----------------------------------------------------------------------------
inline bool cMat3::IsZero(const float Eps) const {
	return m_Rows[0].IsZero(Eps) && m_Rows[1].IsZero(Eps) && m_Rows[2].IsZero(Eps);
}

// cMat3::IsOrthonormal
inline bool cMat3::IsOrthonormal(const float Eps /*= cMath::MatrixEpsilon*/) const {
	const float Det = GetDeterminant();
	return cMath::IsOne(Det, Eps);
}

//-----------------------------------------------------------------------------
// cMat3::OfTranslation : cMat3(const cVec2 &)
//-----------------------------------------------------------------------------
inline cMat3 cMat3::OfTranslation(const cVec2 &t) {
	cMat3 T;
	T.SetRow0(1.0f, 0.0f, 0.0f);
	T.SetRow1(0.0f, 1.0f, 0.0f);
	T.SetRow2(t.x, t.y, 1.0f);
	return T;
}

//-----------------------------------------------------------------------------
// cMat3::OfScale : cMat3(const cVec2 &)
//-----------------------------------------------------------------------------
inline cMat3 cMat3::OfScale(const cVec2 &s) {
	cMat3 S;
	S.SetRow0(s.x, 0.0f, 0.0f);
	S.SetRow1(0.0f, s.y, 0.0f);
	S.SetRow2(0.0f, 0.0f, 1.0f);
	return S;
}

inline cMat3 cMat3::OfRotation(const float Angle) {
	float Cos = cMath::Cos(Angle);
	float Sin = cMath::Sin(Angle);
	cMat3 R;
	R.SetRow0(Cos, Sin, 0.0f);
	R.SetRow1(-Sin, Cos, 0.0f);
	R.SetRow2(0.0f, 0.0f, 1.0f);
	return R;
}

// cMat3::Transpose : const cMat3 (const cMat3 &)
inline const cMat3 cMat3::Transpose(const cMat3 &M) {
	cMat3 T;
	for(int i = 0; i < 3; i++) {
		T.SetRow(i, M.GetCol(i));
	}
	return T;
}

// cMat3::Transpose : void ()
inline void cMat3::Transpose() {
	cMath::Swap(m_Rows[0][1], m_Rows[1][0]);
	cMath::Swap(m_Rows[0][2], m_Rows[2][0]);
	cMath::Swap(m_Rows[1][2], m_Rows[2][1]);
}

// cVec3::Transform : cVec3 & (const cMat3 &)
inline cVec3 & cVec3::Transform(const cMat3 &R) {
	float rx = x * R(0, 0) + y * R(1, 0) + z * R(2, 0);
	float ry = x * R(0, 1) + y * R(1, 1) + z * R(2, 1);
	z = x * R(0, 2) + y * R(1, 2) + z * R(2, 2);
	x = rx;
	y = ry;
	return *this;
}

// cVec3::operator *= : cVec3 & (const cMat3 &)
inline cVec3 & cVec3::operator *= (const cMat3 &R) {
	return Transform(R);
}

// cVec3::Transform : const cVec3 (const cVec3 &, const cMat3 &)
inline const cVec3 cVec3::Transform(const cVec3 &u, const cMat3 &R) {
	cVec3 r;
	r.x = u.x * R(0, 0) + u.y * R(1, 0) + u.z * R(2, 0);
	r.y = u.x * R(0, 1) + u.y * R(1, 1) + u.z * R(2, 1);
	r.z = u.x * R(0, 2) + u.y * R(1, 2) + u.z * R(2, 2);
	return r;
}

// cVec3::operator * : const cVec3 (const cMat3 &) const
inline const cVec3 cVec3::operator * (const cMat3 &R) const {
	return Transform(*this, R);
}


// FIX IT!
//-----------------------------------------------------------------------------
// cVec2::TransformCoordiante : cVec2(const cVec2 &, const cMat3 &)
//-----------------------------------------------------------------------------
inline cVec2 cVec2::TransformCoordinate(const cVec2 &u, const cMat3 &M) {
	return cVec2(u.x * M[0][0] + u.y * M[1][0] + M[2][0], u.x * M[0][1] + u.y * M[1][1] + M[2][1]);
}

// cVec2::TransformNormal : cVec2(const cVec2 &, const cMat3 &)
inline cVec2 cVec2::TransformNormal(const cVec2 &u, const cMat3 &M) {
	return cVec2(u.x * M[0][0] + u.y * M[1][0], u.x * M[0][1] + u.y * M[1][1]);
}

//-----------------------------------------------------------------------------
// cVec2::TransformCoordinate : cVec2 & (const cMat3 &)
//-----------------------------------------------------------------------------
inline cVec2 & cVec2::TransformCoordinate(const cMat3 &M) {
	return *this = cVec2::TransformCoordinate(*this, M);
}