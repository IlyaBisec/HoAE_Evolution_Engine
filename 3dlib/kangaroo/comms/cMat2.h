#pragma once

class cMat2 {
public:
	cVec2 m[2];

	cMat2();
	cMat2(const cVec2 &row0, const cVec2 &row1);
	cMat2(const float m00, const float m01,
			 const float m10, const float m11);

	cMat2 & Copy(const float *pSrc);
	cMat2 & CopyTransposed(const float *pSrc);

	cVec2 & operator [] (const int row);
	const cVec2 & operator [] (const int row) const;
	float & operator () (const int row, const int col);
	float operator () (const int row, const int col) const;

	cMat2 operator - () const;

	cMat2 & operator += (const cMat2 &);
	cMat2 & operator -= (const cMat2 &);
	cMat2 & operator *= (const cMat2 &);
	cMat2 & operator *= (const float);
	cMat2 & operator /= (const float);

	cMat2 operator + (const cMat2 &) const;
	cMat2 operator - (const cMat2 &) const;
	cMat2 operator * (const cMat2 &) const;
	cMat2 operator * (const float) const;
	friend cMat2 operator * (const float, const cMat2 &);
	cMat2 operator / (const float) const;

	bool operator == (const cMat2 &) const;
	bool operator != (const cMat2 &) const;
	bool Equals(const cMat2 &, const float Eps = cMath::Epsilon) const;

	float Trace() const;
	float Determinant() const;
	bool Invert();

	static const cMat2 Zero;
	static const cMat2 Identity;

	float * ToPtr();
	const float * ToPtr() const;
};

// cMat2.ctor : ()
inline cMat2::cMat2() {
}

// cMat2.ctor : (const cVec2 &, const cVec2 &)
inline cMat2::cMat2(const cVec2 &row0, const cVec2 &row1) {
	m[0] = row0;
	m[1] = row1;
}

// cMat2.ctor : (const float, const float,
//					const float, const float)
inline cMat2::cMat2(const float m00, const float m01,
						  const float m10, const float m11) {
	m[0].Set(m00, m01);
	m[1].Set(m10, m11);
}