#pragma once

//*****************************************************************************
// cMat4
//*****************************************************************************
class cMat4 {
public:
	enum eZeroCtor		{ ZeroCtor };
	enum eIdentityCtor	{ IdentityCtor };
	enum eRowsCtor		{ RowsCtor };
	enum eColsCtor		{ ColsCtor };

	cMat4();
	cMat4(const eZeroCtor);
	cMat4(const eIdentityCtor);
	cMat4(const eRowsCtor, const cVec4 &Row0, const cVec4 &Row1, const cVec4 &Row2, const cVec4 &Row3);
	cMat4(const eColsCtor, const cVec4 &Col0, const cVec4 &Col1, const cVec4 &Col2, const cVec4 &Col3);
	cMat4(const cMat3 &Transf, const cVec3 &Transl);
	cMat4(const float _00, const float _01, const float _02, const float _03,
		const float _10, const float _11, const float _12, const float _13,
		const float _20, const float _21, const float _22, const float _23,
		const float _30, const float _31, const float _32, const float _33);
	
	cMat4 & Copy(const float *pSrc);
	cMat4 & CopyTransposed(const float *pSrc);
	
	void SetZero();
	void SetIdentity();

	const cVec4 & GetRow(const int index) const;
	const cVec4 & GetRow0() const;
	const cVec4 & GetRow1() const;
	const cVec4 & GetRow2() const;
	const cVec4 & GetRow3() const;

	cVec4 & Row(const int index);
	cVec4 & Row0();
	cVec4 & Row1();
	cVec4 & Row2();
	cVec4 & Row3();

	void SetRow(const int index, const cVec4 &);
	void SetRow0(const cVec4 &);
	void SetRow1(const cVec4 &);
	void SetRow2(const cVec4 &);
	void SetRow3(const cVec4 &);

	void SetRow(const int index, const float x, const float y, const float z, const float w);
	void SetRow0(const float x, const float y, const float z, const float w);
	void SetRow1(const float x, const float y, const float z, const float w);
	void SetRow2(const float x, const float y, const float z, const float w);
	void SetRow3(const float x, const float y, const float z, const float w);

	const cVec4 GetCol(const int index) const;
	const cVec4 GetCol0() const;
	const cVec4 GetCol1() const;
	const cVec4 GetCol2() const;
	const cVec4 GetCol3() const;

	void SetCol(const int index, const cVec4 &);
	void SetCol0(const cVec4 &);
	void SetCol1(const cVec4 &);
	void SetCol2(const cVec4 &);
	void SetCol3(const cVec4 &);

	void SetCol(const int index, const float x, const float y, const float z, const float w);
	void SetCol0(const float x, const float y, const float z, const float w);
	void SetCol1(const float x, const float y, const float z, const float w);
	void SetCol2(const float x, const float y, const float z, const float w);
	void SetCol3(const float x, const float y, const float z, const float w);

	float GetElem(const int row, const int col) const;
	float & Elem(const int row, const int col);

	const cVec4 & operator [] (const int row) const;
	cVec4 & operator [] (const int row);

	float operator () (const int row, const int col) const;
	float & operator () (const int row, const int col);
	
	void Mul(const cMat4 &R);
	void operator *= (const cMat4 &R);
	static const cMat4 Mul(const cMat4 &L, const cMat4 &R);
	const cMat4 operator * (const cMat4 &R) const;

	static bool Equals(const cMat4 &, const cMat4 &, const float Eps = cMath::MatrixEpsilon);
	bool IsZero(const float Eps = cMath::MatrixEpsilon) const;
	bool IsIdentity(const float Eps = cMath::MatrixEpsilon) const;
	
	const float * ToFloatPtr() const;
	float * ToFloatPtr();

	static const cMat4 Translation(const cVec3 &t);
	void SetTransl(const cVec3 &t);
	cVec3 GetTransl() const;
	
	void SetAxisX(const cVec3 &X);
	void SetAxisY(const cVec3 &Y);
	void SetAxisZ(const cVec3 &Z);
	cVec3 GetAxisX() const;
	cVec3 GetAxisY() const;
	cVec3 GetAxisZ() const;

	static const cMat4 Zero;
	static const cMat4 Identity;
	
	typedef enum {
		CS_LEFTHANDED,
		CS_RIGHTHANDED
	} eCoordSystem;
	static cMat4 FromAxesAndTransl(const cVec3 &xAxis, const cVec3 &yAxis, const cVec3 &zAxis, const cVec3 &Transl);
	static cMat4 OfLookAt(const cVec3 &At, float Azim, float Elev, float toEye, float Roll = 0.0f, eCoordSystem Type = CS_RIGHTHANDED);
    static cMat4 OfProjection(float Fov, float AspectWtoH, float zNear, float zFar);

	static cMat4 _OfLookAt(const cVec3 &At, float Azim, float Elev, float ToEye, float Roll = 0.0f);

	static const cMat4 PerspectiveYFovRH(float YFov, float AspectWtoH, float zNear, float zFar);
	static const cMat4 PerspectiveYFovRHInf(const float YFov, const float AspectWtoH, const float zNear);
	static const cMat4 PerspectiveYFovLH(float YFov, float AspectWtoH, float zNear, float zFar);
	static const cMat4 PerspectiveYFovLHInf(const float YFov, const float AspectWtoH, const float zNear);
	static const cMat4 PerspectiveXFovRH(float XFov, float AspectWtoH, float zNear, float zFar);
	static const cMat4 PerspectiveXFovLH(float XFov, float AspectWtoH, float zNear, float zFar);
	
	static cMat4 OfLookAtRH(const cVec3 &At, float Azim, float Elev, float ToEye, float Roll = 0.0f);
	static cMat4 OfLookAtLH(const cVec3 &At, float Azim, float Elev, float ToEye, float Roll = 0.0f);

	static const cMat4 Transpose(const cMat4 &);
	void Transpose();

	static bool Invert(const cMat4 &Fm, cMat4 *To);
	bool Invert() { return Invert(*this, this); }
private:
	cVec4 m_Rows[4];
};

// cMat4.ctor : ()
inline cMat4::cMat4() {
}

// cMat4.ctor : (const eRowsCtor, const cVec4 &, const cVec4 &, const cVec4 &, const cVec4 &)
inline cMat4::cMat4(const eRowsCtor, const cVec4 &Row0, const cVec4 &Row1, const cVec4 &Row2, const cVec4 &Row3) {
	m_Rows[0] = Row0;
	m_Rows[1] = Row1;
	m_Rows[2] = Row2;
	m_Rows[3] = Row3;
}

// cMat4.ctor : (const eColsCtor, const cVec4 &, const cVec4 &, const cVec4 &, const cVec4 &)
inline cMat4::cMat4(const eColsCtor, const cVec4 &Col0, const cVec4 &Col1, const cVec4 &Col2, const cVec4 &Col3) {
	m_Rows[0].Set(Col0.x, Col1.x, Col2.x, Col3.x);
	m_Rows[1].Set(Col0.y, Col1.y, Col2.y, Col3.y);
	m_Rows[2].Set(Col0.z, Col1.z, Col2.z, Col3.z);
	m_Rows[3].Set(Col0.w, Col1.w, Col2.w, Col3.w);
}

// cMat4.ctor : (const eZeroCtor)
inline cMat4::cMat4(const eZeroCtor) {
	m_Rows[0].SetZero();
	m_Rows[1].SetZero();
	m_Rows[2].SetZero();
	m_Rows[3].SetZero();
}

// cMat4.ctor : (const eIdentityCtor)
inline cMat4::cMat4(const eIdentityCtor) {
	m_Rows[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
	m_Rows[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
	m_Rows[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
	m_Rows[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// cMat4.ctor
//-----------------------------------------------------------------------------
inline cMat4::cMat4(const cMat3 &Transf, const cVec3 &Transl) {
	m_Rows[0].Set(Transf.GetRow0(), 0.0f);
	m_Rows[1].Set(Transf.GetRow1(), 0.0f);
	m_Rows[2].Set(Transf.GetRow2(), 0.0f);
	m_Rows[3].Set(Transl, 1.0f);
} // cMat4.ctor

// cMat4.ctor
inline cMat4::cMat4(const float _00, const float _01, const float _02, const float _03,
					const float _10, const float _11, const float _12, const float _13,
					const float _20, const float _21, const float _22, const float _23,
					const float _30, const float _31, const float _32, const float _33) {
	m_Rows[0].Set(_00, _01, _02, _03);
	m_Rows[1].Set(_10, _11, _12, _13);
	m_Rows[2].Set(_20, _21, _22, _23);
	m_Rows[3].Set(_30, _31, _32, _33);
}

// cMat4::SetZero
inline void cMat4::SetZero() {
	m_Rows[0].SetZero();
	m_Rows[1].SetZero();
	m_Rows[2].SetZero();
	m_Rows[3].SetZero();
}

// cMat4::SetIdentity
inline void cMat4::SetIdentity() {
	m_Rows[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
	m_Rows[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
	m_Rows[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
	m_Rows[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

// cMat4::GetRow : const cVec4 & (const int) const
inline const cVec4 & cMat4::GetRow(const int index) const {
	cAssert(index >= 0 && index < 4);
	return m_Rows[index];
}

// cMat4::GetRow0 : const cVec4 & () const
inline const cVec4 & cMat4::GetRow0() const {
	return m_Rows[0];
}

// cMat4::GetRow1 : const cVec4 & () const
inline const cVec4 & cMat4::GetRow1() const {
	return m_Rows[1];
}

// cMat4::GetRow2 : const cVec4 & () const
inline const cVec4 & cMat4::GetRow2() const {
	return m_Rows[2];
}

// cMat4::GetRow3 : const cVec4 & () const
inline const cVec4 & cMat4::GetRow3() const {
	return m_Rows[3];
}

// cMat4::Row : cVec4 & (const int)
inline cVec4 & cMat4::Row(const int index) {
	cAssert(index >= 0 && index < 4);
	return m_Rows[index];
}

// cMat4::Row0 : cVec4 & ()
inline cVec4 & cMat4::Row0() {
	return m_Rows[0];
}

// cMat4::Row1 : cVec4 & ()
inline cVec4 & cMat4::Row1() {
	return m_Rows[1];
}

// cMat4::Row2 : cVec4 & ()
inline cVec4 & cMat4::Row2() {
	return m_Rows[2];
}

// cMat4::Row3 : cVec4 & ()
inline cVec4 & cMat4::Row3() {
	return m_Rows[3];
}

// cMat4::SetRow : void (const int, const cVec4 &)
inline void cMat4::SetRow(const int index, const cVec4 &r) {
	cAssert(index >= 0 && index < 4);
	m_Rows[index] = r;
}

// cMat4::SetRow0 : void (const cVec4 &)
inline void cMat4::SetRow0(const cVec4 &r) {
	m_Rows[0] = r;
}

// cMat4::SetRow1 : void (const cVec4 &)
inline void cMat4::SetRow1(const cVec4 &r) {
	m_Rows[1] = r;
}

// cMat4::SetRow2 : void (const cVec4 &)
inline void cMat4::SetRow2(const cVec4 &r) {
	m_Rows[2] = r;
}

// cMat4::SetRow3 : void (const cVec4 &)
inline void cMat4::SetRow3(const cVec4 &r) {
	m_Rows[3] = r;
}

// cMat4::SetRow : void (const int, const float, const float, const float, const float)
inline void cMat4::SetRow(const int index, const float x, const float y, const float z, const float w) {
	cAssert(index >= 0 && index < 4);
	m_Rows[index].Set(x, y, z, w);
}

// cMat4::SetRow0 : void (const float, const float, const float, const float)
inline void cMat4::SetRow0(const float x, const float y, const float z, const float w) {
	m_Rows[0].Set(x, y, z, w);
}

// cMat4::SetRow1 : void (const float, const float, const float, const float)
inline void cMat4::SetRow1(const float x, const float y, const float z, const float w) {
	m_Rows[1].Set(x, y, z, w);
}

// cMat4::SetRow2 : void (const float, const float, const float, const float)
inline void cMat4::SetRow2(const float x, const float y, const float z, const float w) {
	m_Rows[2].Set(x, y, z, w);
}

// cMat4::SetRow3 : void (const float, const float, const float, const float)
inline void cMat4::SetRow3(const float x, const float y, const float z, const float w) {
	m_Rows[3].Set(x, y, z, w);
}

// cMat4::GetCol
inline const cVec4 cMat4::GetCol(const int index) const {
	cAssert(index >= 0 && index < 4);
	return cVec4(m_Rows[0][index], m_Rows[1][index], m_Rows[2][index], m_Rows[3][index]);
}

// cMat4::GetCol0
inline const cVec4 cMat4::GetCol0() const {
	return cVec4(m_Rows[0].x, m_Rows[1].x, m_Rows[2].x, m_Rows[3].x);
}

// cMat4::GetCol1
inline const cVec4 cMat4::GetCol1() const {
	return cVec4(m_Rows[0].y, m_Rows[1].y, m_Rows[2].y, m_Rows[3].y);
}

// cMat4::GetCol2
inline const cVec4 cMat4::GetCol2() const {
	return cVec4(m_Rows[0].z, m_Rows[1].z, m_Rows[2].z, m_Rows[3].z);
}

// cMat4::GetCol3
inline const cVec4 cMat4::GetCol3() const {
	return cVec4(m_Rows[0].w, m_Rows[1].w, m_Rows[2].w, m_Rows[3].w);
}

// cMat4::SetCol : void (const int, const cVec4 &)
inline void cMat4::SetCol(const int index, const cVec4 &u) {
	cAssert(index >= 0 && index < 4);
	m_Rows[0][index] = u.x;
	m_Rows[1][index] = u.y;
	m_Rows[2][index] = u.z;
	m_Rows[3][index] = u.w;
}

// cMat4::SetCol0 : void (const cVec4 &)
inline void cMat4::SetCol0(const cVec4 &u) {
	m_Rows[0].x = u.x;
	m_Rows[1].x = u.y;
	m_Rows[2].x = u.z;
	m_Rows[3].x = u.w;
}

// cMat4::SetCol1 : void (const cVec4 &)
inline void cMat4::SetCol1(const cVec4 &u) {
	m_Rows[0].y = u.x;
	m_Rows[1].y = u.y;
	m_Rows[2].y = u.z;
	m_Rows[3].y = u.w;
}

// cMat4::SetCol2 : void (const cVec4 &)
inline void cMat4::SetCol2(const cVec4 &u) {
	m_Rows[0].z = u.x;
	m_Rows[1].z = u.y;
	m_Rows[2].z = u.z;
	m_Rows[3].z = u.w;
}

// cMat4::SetCol3 : void (const cVec4 &)
inline void cMat4::SetCol3(const cVec4 &u) {
	m_Rows[0].w = u.x;
	m_Rows[1].w = u.y;
	m_Rows[2].w = u.z;
	m_Rows[3].w = u.w;
}

// cMat4::SetCol : void (const int, const float, const float, const float, const float)
inline void cMat4::SetCol(const int index, const float x, const float y, const float z, const float w) {
	cAssert(index >= 0 && index < 4);
	m_Rows[0][index] = x;
	m_Rows[1][index] = y;
	m_Rows[2][index] = z;
	m_Rows[3][index] = w;
}

// cMat4::SetCol0 : void (const float, const float, const float, const float)
inline void cMat4::SetCol0(const float x, const float y, const float z, const float w) {
	m_Rows[0].x = x;
	m_Rows[1].x = y;
	m_Rows[2].x = z;
	m_Rows[3].x = w;
}

// cMat4::SetCol1 : void (const float, const float, const float, const float)
inline void cMat4::SetCol1(const float x, const float y, const float z, const float w) {
	m_Rows[0].y = x;
	m_Rows[1].y = y;
	m_Rows[2].y = z;
	m_Rows[3].y = w;
}

// cMat4::SetCol2 : void (const float, const float, const float, const float)
inline void cMat4::SetCol2(const float x, const float y, const float z, const float w) {
	m_Rows[0].z = x;
	m_Rows[1].z = y;
	m_Rows[2].z = z;
	m_Rows[3].z = w;
}

// cMat4::SetCol3 : void (const float, const float, const float, const float)
inline void cMat4::SetCol3(const float x, const float y, const float z, const float w) {
	m_Rows[0].w = x;
	m_Rows[1].w = y;
	m_Rows[2].w = z;
	m_Rows[3].w = w;
}

// cMat4::GetElem : float (const int, const int) const
inline float cMat4::GetElem(const int row, const int col) const {
	cAssert(row >= 0 && row < 4);
	cAssert(col >= 0 && col < 4);
	return m_Rows[row][col];
}

// cMat4::Elem : float & (const int, const int)
inline float & cMat4::Elem(const int row, const int col) {
	cAssert(row >= 0 && row < 4);
	cAssert(col >= 0 && col < 4);
	return m_Rows[row][col];
}

// cMat3::ToMat4
inline const cMat4 cMat3::ToMat4() const {
	return cMat4(*this, cVec3::Zero);
}


// cMat4::operator [] : cVec4 & (const int)
inline cVec4 & cMat4::operator [] (const int row) {
	cAssert(row >= 0 && row < 4);
	return m_Rows[row];
}

// cMat4::operator [] : const cVec4 & (const int) const
inline const cVec4 & cMat4::operator [] (const int row) const {
	cAssert(row >= 0 && row < 4);
	return m_Rows[row];
}

// cMat4::operator () : float & (const int, const int)
inline float & cMat4::operator () (const int row, const int col) {
	cAssert(row >= 0 && row < 4 && col >= 0 && col < 4);
	return m_Rows[row][col];
}

// cMat4::operator () : float(const int, const int) const
inline float cMat4::operator () (const int row, const int col) const {
	cAssert(row >= 0 && row < 4 && col >= 0 && col < 4);
	return m_Rows[row][col];
}

//-----------------------------------------------------------------------------
// cMat4::Mul : void (const cMat4 &)
//-----------------------------------------------------------------------------
inline void cMat4::Mul(const cMat4 &R) {
	float *l = reinterpret_cast<float *>(this);
	const float *r = reinterpret_cast<const float *>(&R);
	
	float t[4];
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			t[j] = l[0] * r[0 * 4 + j] + l[1] * r[1 * 4 + j] + l[2] * r[2 * 4 + j] + l[3] * r[3 * 4 + j];
		}
		l[0] = t[0];
		l[1] = t[1];
		l[2] = t[2];
		l[3] = t[3];
		l += 4;
	}
} // cMat4::Mul

// cMat4::operator *= : void (const cMat4 &)
inline void cMat4::operator *= (const cMat4 &R) {
	Mul(R);
}

//-----------------------------------------------------------------------------
// cMat4::Mul : const cMat4 (const cMat4 &, const cMat4 &)
//-----------------------------------------------------------------------------
inline const cMat4 cMat4::Mul(const cMat4 &L, const cMat4 &R) {
	const float *l = reinterpret_cast<const float *>(&L);
	const float *r = reinterpret_cast<const float *>(&R);
	cMat4 M;
	float *m = reinterpret_cast<float *>(&M);
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			*m = l[0] * r[0 * 4 + j] + l[1] * r[1 * 4 + j] + l[2] * r[2 * 4 + j] + l[3] * r[3 * 4 + j];
			m++;
		}
		l += 4;
	}
	return M;
} // cMat4::Mul

// cMat4::operator * : const cMat4 (const cMat4 &) const
inline const cMat4 cMat4::operator * (const cMat4 &R) const {
	return Mul(*this, R);
}

// cMat4::Equals
inline bool cMat4::Equals(const cMat4 &P, const cMat4 &Q, const float Eps /*= cMath::MatrixEpsilon*/) {
	if(cVec4::Equals(P[0], Q[0], Eps) && cVec4::Equals(P[1], Q[1], Eps) && cVec4::Equals(P[2], Q[2], Eps) && cVec4::Equals(P[3], Q[3], Eps)) {
		return true;
	}
	return false;
}

// cMat4::IsZero
inline bool cMat4::IsZero(const float Eps /*= cMath::MatrixEpsilon*/) const {
	return Equals(*this, cMat4::Zero, Eps);
}

// cMat4::IsIdentity
inline bool cMat4::IsIdentity(const float Eps /*= cMath::MatrixEpsilon*/) const {
	return Equals(*this, cMat4::Identity, Eps);
}

// cMat4::ToFloatPtr : const float * () const
inline const float * cMat4::ToFloatPtr() const {
	return m_Rows[0].ToFloatPtr();
}

// cMat4::ToFloatPtr : float * ()
inline float * cMat4::ToFloatPtr() {
	return m_Rows[0].ToFloatPtr();
}

// cMat4::SetTransl : void(const cVec3 &)
inline void cMat4::SetTransl(const cVec3 &t) {
	m_Rows[3].Set(t, m_Rows[3].w);
}

// cMat4::GetTransl : cVec3() const
inline cVec3 cMat4::GetTransl() const {
	return m_Rows[3].ToVec3();
}

// cMat4::SetAxisX : void(const cVec3 &)
inline void cMat4::SetAxisX(const cVec3 &X) {
	m_Rows[0].x = X.x;
	m_Rows[1].x = X.y;
	m_Rows[2].x = X.z;
}

// cMat4::SetAxisY : void(const cVec3 &)
inline void cMat4::SetAxisY(const cVec3 &Y) {
	m_Rows[0].y = Y.x;
	m_Rows[1].y = Y.y;
	m_Rows[2].y = Y.z;
}

// cMat4::SetAxisZ : void(const cVec3 &)
inline void cMat4::SetAxisZ(const cVec3 &Z) {
	m_Rows[0].z = Z.x;
	m_Rows[1].z = Z.y;
	m_Rows[2].z = Z.z;
}

// cMat4::GetAxisX : cVec3() const
inline cVec3 cMat4::GetAxisX() const {
	return cVec3(m_Rows[0].x, m_Rows[1].x, m_Rows[2].x);
}

// cMat4::GetAxisY : cVec3() const
inline cVec3 cMat4::GetAxisY() const {
	return cVec3(m_Rows[0].y, m_Rows[1].y, m_Rows[2].y);
}

// cMat4::GetAxisZ : cVec3() const
inline cVec3 cMat4::GetAxisZ() const {
	return cVec3(m_Rows[0].z, m_Rows[1].z, m_Rows[2].z);
}

//----------------------------------------------------------------------------------------------------------------------------------------
// cMat4::FromAxesAndTransl : cMat4(const cVec3 &, const cVec3 &, const cVec3 &, const cVec3 &)
//----------------------------------------------------------------------------------------------------------------------------------------
inline cMat4 cMat4::FromAxesAndTransl(const cVec3 &xAxis, const cVec3 &yAxis, const cVec3 &zAxis, const cVec3 &Transl) {
	cMat4 M(cMat4::IdentityCtor);
	M.SetCol0(cVec4(xAxis, Transl.x));
	M.SetCol1(cVec4(yAxis, Transl.y));
	M.SetCol2(cVec4(zAxis, Transl.z));
	return M;
}

// cMat4::Transpose : const cMat4 (const cMat4 &)
inline const cMat4 cMat4::Transpose(const cMat4 &M) {
	cMat4 T;
	for(int i = 0; i < 4; i++) {
		T.SetRow(i, M.GetCol(i));
	}
	return T;
}

// cMat4::Transpose : void ()
inline void cMat4::Transpose() {
	cMath::Swap(m_Rows[0][1], m_Rows[1][0]);
	cMath::Swap(m_Rows[0][2], m_Rows[2][0]);
	cMath::Swap(m_Rows[1][2], m_Rows[2][1]);
	cMath::Swap(m_Rows[0][3], m_Rows[3][0]);
	cMath::Swap(m_Rows[1][3], m_Rows[3][1]);
	cMath::Swap(m_Rows[2][3], m_Rows[3][2]);
}


//*****************************************************************************
// cVec3 & cMat4
//*****************************************************************************

// cVec3::Transform
inline const cVec4 cVec3::Transform(const cVec3 &u, const cMat4 &M) {
	cVec4 t;
	t.x = u.x * M[0][0] + u.y * M[1][0] + u.z * M[2][0] + M[3][0];
	t.y = u.x * M[0][1] + u.y * M[1][1] + u.z * M[2][1] + M[3][1];
	t.z = u.x * M[0][2] + u.y * M[1][2] + u.z * M[2][2] + M[3][2];
	t.w = u.x * M[0][3] + u.y * M[1][3] + u.z * M[2][3] + M[3][3];
	return t;
}

// cVec3::TransformCoordinate
inline cVec3 & cVec3::TransformCoordinate(const cMat4 &M) {
	float X = x * M[0][0] + y * M[1][0] + z * M[2][0] + M[3][0];
	float Y = x * M[0][1] + y * M[1][1] + z * M[2][1] + M[3][1];
	float Z = x * M[0][2] + y * M[1][2] + z * M[2][2] + M[3][2];
	float W = x * M[0][3] + y * M[1][3] + z * M[2][3] + M[3][3];
	if(!cMath::IsOne(W)) {
		const float iW = 1.0f / W;
		X *= iW;
		Y *= iW;
		Z *= iW;
	}
	x = X;
	y = Y;
	z = Z;
	return *this;
}

// cVec3::TransformCoordinate
inline const cVec3 cVec3::TransformCoordinate(const cVec3 &u, const cMat4 &M) {
	cVec3 r;
	r.x = u.x * M[0][0] + u.y * M[1][0] + u.z * M[2][0] + M[3][0];
	r.y = u.x * M[0][1] + u.y * M[1][1] + u.z * M[2][1] + M[3][1];
	r.z = u.x * M[0][2] + u.y * M[1][2] + u.z * M[2][2] + M[3][2];
	float w = u.x * M[0][3] + u.y * M[1][3] + u.z * M[2][3] + M[3][3];
	if(!cMath::IsOne(w)) {
		float iw = 1.0f / w;
		r.x *= iw;
		r.y *= iw;
		r.z *= iw;
	}
	return r;
}

// cVec3::TransformNormal
inline cVec3 & cVec3::TransformNormal(const cMat4 &M) {
	const float X = x * M[0][0] + y * M[1][0] + z * M[2][0];
	const float Y = x * M[0][1] + y * M[1][1] + z * M[2][1];
	const float Z = x * M[0][2] + y * M[1][2] + z * M[2][2];
	x = X;
	y = Y;
	z = Z;
	return *this;
}

// cVec3::TransformNormal
inline const cVec3 cVec3::TransformNormal(const cVec3 &u, const cMat4 &M) {
	cVec3 r;
	r.x = u.x * M[0][0] + u.y * M[1][0] + u.z * M[2][0];
	r.y = u.x * M[0][1] + u.y * M[1][1] + u.z * M[2][1];
	r.z = u.x * M[0][2] + u.y * M[1][2] + u.z * M[2][2];
	return r;
}

//*****************************************************************************
// cVec4 & cMat4
//*****************************************************************************

// cVec4::Transform : void (const cMat4 &)
inline void cVec4::Transform(const cMat4 &T) {
	const float X = x * T(0, 0) + y * T(1, 0) + z * T(2, 0) + w * T(3, 0);
	const float Y = x * T(0, 1) + y * T(1, 1) + z * T(2, 1) + w * T(3, 1);
	const float Z = x * T(0, 2) + y * T(1, 2) + z * T(2, 2) + w * T(3, 2);
	const float W = x * T(0, 3) + y * T(1, 3) + z * T(2, 3) + w * T(3, 3);
	x = X;
	y = Y;
	z = Z;
	w = W;
}

// cVec4::operator *= : void (const cMat4 &)
inline void cVec4::operator *= (const cMat4 &M) {
	Transform(M);
}

// cVec4::Transform : const cVec4 (const cVec4 &, const cMat4 &)
inline const cVec4 cVec4::Transform(const cVec4 &u, const cMat4 &T) {
	cVec4 r;
	r.x = u.x * T(0, 0) + u.y * T(1, 0) + u.z * T(2, 0) + u.w * T(3, 0);
	r.y = u.x * T(0, 1) + u.y * T(1, 1) + u.z * T(2, 1) + u.w * T(3, 1);
	r.z = u.x * T(0, 2) + u.y * T(1, 2) + u.z * T(2, 2) + u.w * T(3, 2);
	r.w = u.x * T(0, 3) + u.y * T(1, 3) + u.z * T(2, 3) + u.w * T(3, 3);
	return r;
}

// cVec4::operator * : const cVec4 (const cMat4 &) const
inline const cVec4 cVec4::operator * (const cMat4 &T) const {
	return Transform(*this, T);
}