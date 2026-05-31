#pragma once

class cMat4 {
public:
	cVec4 m[4];

	cMat4();
	cMat4(const cVec4 &row0, const cVec4 &row1, const cVec4 &row2, const cVec4 &row3);
	cMat4(const float m00, const float m01, const float m02, const float m03,
			 const float m10, const float m11, const float m12, const float m13,
			 const float m20, const float m21, const float m22, const float m23,
			 const float m30, const float m31, const float m32, const float m33);

	cMat4 & Copy(const float *pSrc);
	cMat4 & CopyTransposed(const float *pSrc);

	cVec4 & operator [] (const int row);
	const cVec4 & operator [] (const int row) const;
	float & operator () (const int row, const int col);
	float operator () (const int row, const int col) const;

	cMat4 operator * (const cMat4 &) const;

	float * ToPtr();
	const float * ToPtr() const;

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

	static cMat4 OfLookAtRH(const cVec3 &At, float Azim, float Elev, float ToEye, float Roll = 0.0f);
	static cMat4 OfLookAtLH(const cVec3 &At, float Azim, float Elev, float ToEye, float Roll = 0.0f);
	static cMat4 OfPerspectiveYFovRH(float yFov, float AspectWtoH, float zNear, float zFar);
	static cMat4 OfPerspectiveXFovRH(float xFov, float AspectWtoH, float zNear, float zFar);
	static cMat4 OfPerspectiveYFovLH(float yFov, float AspectWtoH, float zNear, float zFar);
	static cMat4 OfPerspectiveXFovLH(float xFov, float AspectWtoH, float zNear, float zFar);

	static cMat4 OfShadow(const cVec4 &Light, const cPlane &Plane);
};

inline cMat4 cMat3::ToMat4() const {
	return cMat4(m[0][0], m[0][1], m[0][1], 0.0f,
				 m[1][0], m[1][1], m[1][2], 0.0f,
				 m[2][0], m[2][1], m[2][2], 0.0f,
				 0.0f,	 0.0f,	  0.0f,    1.0f);
}

//-----------------------------------------------------------------------------
// cVec3::TransformCoordinate : cVec3 & (const cMat4 &)
//-----------------------------------------------------------------------------
inline cVec3 & cVec3::TransformCoordinate(const cMat4 &M) {
	float X = x * M[0][0] + y * M[1][0] + z * M[2][0] + M[3][0];
	float Y = x * M[0][1] + y * M[1][1] + z * M[2][1] + M[3][1];
	float Z = x * M[0][2] + y * M[1][2] + z * M[2][2] + M[3][2];
	float W = x * M[0][3] + y * M[1][3] + z * M[2][3] + M[3][3];
	if(!cMath::IsOne(W)) {
		float iW = 1.0f / W;
		X *= iW;
		Y *= iW;
		Z *= iW;
	}
	x = X;
	y = Y;
	z = Z;
	return *this;
}

//-----------------------------------------------------------------------------------
// cVec3::TransformCoordinate : const cVec3(const cVec3 &, const cMat4 &)
//-----------------------------------------------------------------------------------
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

// cMat4.ctor : ()
inline cMat4::cMat4() {
}

// cMat4.ctor : (const cVec4 &, const cVec4 &, const cVec4 &, const cVec4 &)
inline cMat4::cMat4(const cVec4 &row0, const cVec4 &row1, const cVec4 &row2, const cVec4 &row3) {
	m[0] = row0;
	m[1] = row1;
	m[2] = row2;
	m[3] = row3;
}

// cMat4.ctor : (const float, const float, const float, const float,
//					const float, const float, const float, const float,
//					const float, const float, const float, const float,
//					const float, const float, const float, const float)
inline cMat4::cMat4(const float m00, const float m01, const float m02, const float m03,
						  const float m10, const float m11, const float m12, const float m13,
						  const float m20, const float m21, const float m22, const float m23,
						  const float m30, const float m31, const float m32, const float m33) {
	m[0].Set(m00, m01, m02, m03);
	m[1].Set(m10, m11, m12, m13);
	m[2].Set(m20, m21, m22, m23);
	m[3].Set(m30, m31, m32, m33);
}

// cMat4::operator [] : cVec4 & (const int)
inline cVec4 & cMat4::operator [] (const int row) {
	assert(row >= 0 && row < 4);
	return m[row];
}

// cMat4::operator [] : const cVec4 & (const int) const
inline const cVec4 & cMat4::operator [] (const int row) const {
	assert(row >= 0 && row < 4);
	return m[row];
}

// cMat4::operator () : float & (const int, const int)
inline float & cMat4::operator () (const int row, const int col) {
	assert(row >= 0 && row < 4 && col >= 0 && col < 4);
	return m[row][col];
}

// cMat4::operator () : float(const int, const int) const
inline float cMat4::operator () (const int row, const int col) const {
	assert(row >= 0 && row < 4 && col >= 0 && col < 4);
	return m[row][col];
}

//-----------------------------------------------------------------------------
// cMat4::operator * : cMat4(const cMat4 &) const
//-----------------------------------------------------------------------------
inline cMat4 cMat4::operator * (const cMat4 &M) const {
	const float *p1 = reinterpret_cast<const float *>(this);
	const float *p2 = reinterpret_cast<const float *>(&M);
	cMat4 R;
	float *r = reinterpret_cast<float *>(&R);

	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			*r = p1[0] * p2[0 * 4 + j] + p1[1] * p2[1 * 4 + j] + p1[2] * p2[2 * 4 + j] + p1[3] * p2[3 * 4 + j];
			r++;
		}
		p1 += 4;
	}
	return R;
}

// cMat4::ToPtr : float * ()
inline float * cMat4::ToPtr() {
	return m[0].ToPtr();
}

// cMat4::ToPtr : const float * () const
inline const float * cMat4::ToPtr() const {
	return m[0].ToPtr();
}

// cMat4::SetTransl : void(const cVec3 &)
inline void cMat4::SetTransl(const cVec3 &t) {
	m[3].Set(t, m[3].w);
}

// cMat4::GetTransl : cVec3() const
inline cVec3 cMat4::GetTransl() const {
	return m[3].To3();
}

// cMat4::SetAxisX : void(const cVec3 &)
inline void cMat4::SetAxisX(const cVec3 &X) {
	m[0].x = X.x;
	m[1].x = X.y;
	m[2].x = X.z;
}

// cMat4::SetAxisY : void(const cVec3 &)
inline void cMat4::SetAxisY(const cVec3 &Y) {
	m[0].y = Y.x;
	m[1].y = Y.y;
	m[2].y = Y.z;
}

// cMat4::SetAxisZ : void(const cVec3 &)
inline void cMat4::SetAxisZ(const cVec3 &Z) {
	m[0].z = Z.x;
	m[1].z = Z.y;
	m[2].z = Z.z;
}

// cMat4::GetAxisX : cVec3() const
inline cVec3 cMat4::GetAxisX() const {
	return cVec3(m[0].x, m[1].x, m[2].x);
}

// cMat4::GetAxisY : cVec3() const
inline cVec3 cMat4::GetAxisY() const {
	return cVec3(m[0].y, m[1].y, m[2].y);
}

// cMat4::GetAxisZ : cVec3() const
inline cVec3 cMat4::GetAxisZ() const {
	return cVec3(m[0].z, m[1].z, m[2].z);
}

//----------------------------------------------------------------------------------------------------------------------------------------
// cMat4::FromAxesAndTransl : cMat4(const cVec3 &, const cVec3 &, const cVec3 &, const cVec3 &)
//----------------------------------------------------------------------------------------------------------------------------------------
inline cMat4 cMat4::FromAxesAndTransl(const cVec3 &xAxis, const cVec3 &yAxis, const cVec3 &zAxis, const cVec3 &Transl) {
	return cMat4(xAxis.x,  yAxis.x, 	zAxis.x,  0.0f,
					xAxis.y,  yAxis.y,  zAxis.y,  0.0f,
					xAxis.z,  yAxis.z,  zAxis.z,  0.0f,
					Transl.x, Transl.y, Transl.z, 1.0f);
}