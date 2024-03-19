#pragma once

class cMat3 {
public:
	cVec3 m[3];

	cMat3();
	cMat3(const cVec3 &row0, const cVec3 &row1, const cVec3 &row2);
	cMat3(const float m00, const float m01, const float m02,
			 const float m10, const float m11, const float m12,
			 const float m20, const float m21, const float m22);

	cMat3 & Copy(const float *pSrc);
	cMat3 & CopyTransposed(const float *pSrc);

	cVec3 & operator [] (const int row);
	const cVec3 & operator [] (const int row) const;
	float & operator () (const int row, const int col);
	float operator () (const int row, const int col) const;

	const cVec3 GetColumnX() const;
	const cVec3 GetColumnY() const;
	const cVec3 GetColumnZ() const;

	void SetColumnX(const cVec3 &);
	void SetColumnY(const cVec3 &);
	void SetColumnZ(const cVec3 &);

	cMat3 & operator *= (const cMat3 &);
	cMat3 operator * (const cMat3 &) const;

	bool IsZero(const float Eps = cMath::Epsilon) const;

	static const cMat3 Zero;
	static const cMat3 Identity;

	bool static Invert(const cMat3 &Src, cMat3 *pDst, const float Eps = cMath::Epsilon);
	static cMat3 OfTranslation(const cVec2 &);
	static cMat3 OfScale(const cVec2 &);
	static cMat3 OfRotation(const float Angle);
	static cMat3 OfFitToRect(const cVec2 &bbMin, const cVec2 &bbMax,
		const cVec2 &rcMin, const cVec2 &rcMax, bool xScale = true, bool yScale = true);
	
	float * ToPtr();
	const float * ToPtr() const;
	cMat4 ToMat4() const;

	cAngles ToAngles() const;
	cQuat ToQuat() const;
};

// cMat3.ctor : ()
inline cMat3::cMat3() {
}

// cMat3.ctor : (const cVec3 &, const cVec3 &,const cVec3 &)
inline cMat3::cMat3(const cVec3 &row0, const cVec3 &row1, const cVec3 &row2) {
	m[0] = row0;
	m[1] = row1;
	m[2] = row2;
}

// cMat3.ctor : (const float, const float, const float,
//					const float, const float, const float,
//					const float, const float, const float)
inline cMat3::cMat3(const float m00, const float m01, const float m02,
						  const float m10, const float m11, const float m12,
						  const float m20, const float m21, const float m22) {
	m[0].Set(m00, m01, m02);
	m[1].Set(m10, m11, m12);
	m[2].Set(m20, m21, m22);
}

inline cVec3 & cMat3::operator [] (const int row) {
	assert(row >= 0 && row < 3);
	return m[row];
}

inline const cVec3 & cMat3::operator [] (const int row) const {
	assert(row >= 0 && row < 3);
	return m[row];
}

inline const cVec3 cMat3::GetColumnX() const {
	return cVec3(m[0].x, m[1].x, m[2].x);
}

inline const cVec3 cMat3::GetColumnY() const {
	return cVec3(m[0].y, m[1].y, m[2].y);
}

inline const cVec3 cMat3::GetColumnZ() const {
	return cVec3(m[0].z, m[1].z, m[2].z);
}

inline void cMat3::SetColumnX(const cVec3 &u) {
	m[0].x = u.x;
	m[1].x = u.y;
	m[2].x = u.z;
}

inline void cMat3::SetColumnY(const cVec3 &u) {
	m[0].y = u.x;
	m[1].y = u.y;
	m[2].y = u.z;
}

inline void cMat3::SetColumnZ(const cVec3 &u) {
	m[0].z = u.x;
	m[1].z = u.y;
	m[2].z = u.z;
}

//-----------------------------------------------------------------------------
// cMat3::operator *= : cMat3 & (const cMat3 &)
//-----------------------------------------------------------------------------
inline cMat3 & cMat3::operator *= (const cMat3 &M) {
	float *p1 = reinterpret_cast<float *>(this);
	const float *p2 = reinterpret_cast<const float *>(&M);

	float f[3];
	int i, j;
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			f[j] = p1[0] * p2[0 * 3 + j] + p1[1] * p2[1 * 3 + j] + p1[2] * p2[2 * 3 + j];
		}
		p1[0] = f[0];
		p1[1] = f[1];
		p1[2] = f[2];
		p1 += 3;
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cMat3::operator * : cMat3(const cMat3 &) const
//-----------------------------------------------------------------------------
inline cMat3 cMat3::operator * (const cMat3 &M) const {
	cMat3 R;
	const float *p1 = reinterpret_cast<const float *>(this);
	const float *p2 = reinterpret_cast<const float *>(&M);
	float *r = reinterpret_cast<float *>(&R);

	int i, j;
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			*r = p1[0] * p2[0 * 3 + j] + p1[1] * p2[1 * 3 + j] + p1[2] * p2[2 * 3 + j];
			r++;
		}
		p1 += 3;
	}
	return R;
}

//-----------------------------------------------------------------------------
// cMat3::IsZero : bool(const float) const
//-----------------------------------------------------------------------------
inline bool cMat3::IsZero(const float Eps) const {
	return m[0].IsZero(Eps) && m[1].IsZero(Eps) && m[2].IsZero(Eps);
}

//-----------------------------------------------------------------------------
// cMat3::OfTranslation : cMat3(const cVec2 &)
//-----------------------------------------------------------------------------
inline cMat3 cMat3::OfTranslation(const cVec2 &t) {
	return cMat3(1.0f, 0.0f, 0.0f,
					0.0f, 1.0f,	0.0f,
					t.x,  t.y,	1.0f);
}

//-----------------------------------------------------------------------------
// cMat3::OfScale : cMat3(const cVec2 &)
//-----------------------------------------------------------------------------
inline cMat3 cMat3::OfScale(const cVec2 &s) {
	return cMat3(s.x,  0.0f, 0.0f,
					0.0f, s.y,  0.0f,
					0.0f, 0.0f, 1.0f);
}

inline cMat3 cMat3::OfRotation(const float Angle) {
	float Cos = cMath::Cos(Angle);
	float Sin = cMath::Sin(Angle);
	return cMat3(Cos, Sin, 0.0f,
					- Sin, Cos, 0.0f,
					0.0f, 0.0f, 1.0f);
}

// cVec3::Transform : cVec3 & (const cMat3 &)
inline cVec3 & cVec3::Transform(const cMat3 &T) {
	float X = x * T[0].x + y * T[1].x + z * T[2].x;
	float Y = x * T[0].y + y * T[1].y + z * T[2].y;
	z = x * T[0].z + y * T[1].z + z * T[1].z;
	x = X;
	y = Y;
	return *this;
}

// cVec3::operator *= : cVec3 & (const cMat3 &)
inline cVec3 & cVec3::operator *= (const cMat3 &T) {
	return Transform(T);
}

// cVec3::Transform : const cVec3 (const cVec3 &, const cMat3 &)
inline const cVec3 cVec3::Transform(const cVec3 &u, const cMat3 &T) {
	cVec3 r;
	r.x = u.x * T[0].x + u.y * T[1].x + u.z * T[2].x;
	r.y = u.x * T[0].y + u.y * T[1].y + u.z * T[2].y;
	r.z = u.x * T[0].z + u.y * T[1].z + u.z * T[2].z;
	return r;
}

// cVec3::operator * : const cVec3 (const cMat3 &) const
inline const cVec3 cVec3::operator * (const cMat3 &T) const {
	return Transform(*this, T);
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