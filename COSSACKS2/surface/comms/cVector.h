#pragma once

namespace comms {
	
template<class Type, int Dim>
struct cVector {
	Type v[Dim];

	cVector();
	cVector(const cVector<Type, Dim> &Src);
	explicit cVector(const Type S);
	cVector(const Type X, const Type Y);
	cVector(const Type X, const Type Y, const Type Z);
	cVector(const Type X, const Type Y, const Type Z, const Type W);
	cVector(const cVector<Type, 2> &XY, const Type Z);
	cVector(const cVector<Type, 2> &XY, const Type Z, const Type W);
	cVector(const cVector<Type, 3> &XYZ, const Type W);

	cVector<Type, Dim> & operator = (const cVector<Type, Dim> &Src);
	cVector<Type, Dim> & Copy(const Type *pSrc);
	cVector<Type, Dim> & Set(const Type S);
	cVector<Type, 2> & Set(const Type X, const Type Y);
	cVector<Type, 3> & Set(const Type X, const Type Y, const Type Z);
	cVector<Type, 4> & Set(const Type X, const Type Y, const Type Z, const Type W);
	cVector<Type, 3> & Set(const cVector<Type, 2> &XY, const Type Z);
	cVector<Type, 4> & Set(const cVector<Type, 2> &XY, const Type Z, const Type W);
	cVector<Type, 4> & Set(const cVector<Type, 3> &XYZ, const Type W);

	Type & operator [] (const int index);
	const Type & operator [] (const int index) const;

	cVector<Type, Dim> operator - () const;

	cVector<Type, Dim> & operator += (const cVector<Type, Dim> &);
	cVector<Type, Dim> & operator -= (const cVector<Type, Dim> &);
	cVector<Type, Dim> & operator *= (const cVector<Type, Dim> &);
	cVector<Type, Dim> & operator *= (const Type);
	cVector<Type, Dim> & operator /= (const cVector<Type, Dim> &);
	cVector<Type, Dim> & operator /= (const Type);
	
	cVector<Type, Dim> operator + (const cVector<Type, Dim> &) const;
	cVector<Type, Dim> operator - (const cVector<Type, Dim> &) const;
	cVector<Type, Dim> operator * (const cVector<Type, Dim> &) const;
	cVector<Type, Dim> operator * (const Type) const;
	//friend cVector<Type, Dim> operator * (const Type, const cVector<Type, Dim> &);
	cVector<Type, Dim> operator / (const cVector<Type, Dim> &) const;
	cVector<Type, Dim> operator / (const Type) const;

	bool operator == (const cVector<Type, Dim> &) const;
	bool operator != (const cVector<Type, Dim> &) const;
	static bool Equals(const cVector<Type, Dim> &, const cVector<Type, Dim> &, const Type = (Type)cMath::Epsilon);

	Type Length() const;
	Type LengthSq() const;
	Type Normalize();
	bool IsNormalized(const Type = (Type)cMath::Epsilon) const;
	bool IsZero(const Type = (Type)cMath::Epsilon) const;
	
	static cVector<Type, Dim> Abs(const cVector<Type, Dim> &);
	static Type Angle(const cVector<Type, Dim> &, const cVector<Type, Dim> &);
	static cVector<Type, Dim> Clamp(const cVector<Type, Dim> &, const Type, const Type);
	static cVector<Type, Dim> Clamp(const cVector<Type, Dim> &, const cVector<Type, Dim> &, const cVector<Type, Dim> &);
	static cVector<Type, 3> Cross(const cVector<Type, 3> &, const cVector<Type, 3> &);
	static Type Distance(const cVector<Type, Dim> &, const cVector<Type, Dim> &);
	static Type DistanceSq(const cVector<Type, Dim> &, const cVector<Type, Dim> &);
	static Type Dot(const cVector<Type, Dim> &, const cVector<Type, Dim> &);
	static Type Length(const cVector<Type, Dim> &);
	static Type LengthSq(const cVector<Type, Dim> &);
	static cVector<Type, Dim> Lerp(const cVector<Type, Dim> &, const cVector<Type, Dim> &, const Type);
	static cVector<Type, Dim> Lerp(const cVector<Type, Dim> &, const cVector<Type, Dim> &, const cVector<Type, Dim> &);
	static cVector<Type, Dim> Lerp05(const cVector<Type, Dim> &, const cVector<Type, Dim> &);
	static cVector<Type, Dim> Max(const cVector<Type, Dim> &, const cVector<Type, Dim> &);
	static cVector<Type, Dim> Min(const cVector<Type, Dim> &, const cVector<Type, Dim> &);
	static cVector<Type, Dim> Normalize(const cVector<Type, Dim> &);
	static cVector<Type, Dim> Reflect(const cVector<Type, Dim> &RayDir, const cVector<Type, Dim> &Normal);
	static cVector<Type, Dim> Refract(const cVector<Type, Dim> &RayDir, const cVector<Type, Dim> &Normal, const Type Eta);
	static cVector<Type, Dim> Truncate(const cVector<Type, Dim> &, const Type);
	Type DistanceToLineSegSq(const cVector<Type, Dim> &A, const cVector<Type, Dim> &B, Type *pScale = NULL) const;

	Type * ToPtr();
	const Type * ToPtr() const;

	cVector<Type, 2> & To2();
	const cVector<Type, 2> & To2() const;
	cVector<Type, 3> & To3();
	const cVector<Type, 3> & To3() const;

    cStr ToStr(int nPrec = 2) const;
	friend cStr & operator << (cStr &, const cVector<Type, Dim> &);
};

static const int Xelt = 0;
static const int Yelt = 1;
static const int Zelt = 2;
static const int Welt = 3;

// cVector<Type, Dim>.ctor : ()
template<class Type, int Dim>
inline cVector<Type, Dim>::cVector() {
}

// cVector<Type, Dim>.ctor : (const cVector<Type, Dim> &Src)
template<class Type, int Dim>
inline cVector<Type, Dim>::cVector(const cVector<Type, Dim> &Src) {
	for(int i = 0; i < Dim; i++) {
		v[i] = Src[i];
	}
}

// cVector<Type, Dim>.ctor : (const Type S)
template<class Type, int Dim>
inline cVector<Type, Dim>::cVector(const Type S) {
	for(int i = 0; i < Dim; i++) {
		v[i] = S;
	}
}

// cVector<Type, 2>.ctor : (const Type X, const Type Y)
template<class Type, int Dim>
inline cVector<Type, Dim>::cVector(const Type X, const Type Y) {
	assert(Dim == 2);
	v[0] = X;
	v[1] = Y;
}

// cVector<Type, 3>.ctor : (const Type X, const Type Y, const Type Z)
template<class Type, int Dim>
inline cVector<Type, Dim>::cVector(const Type X, const Type Y, const Type Z) {
	assert(Dim == 3);
	v[0] = X;
	v[1] = Y;
	v[2] = Z;
}

// cVector<Type, 4>.ctor : (const Type X, const Type Y, const Type Z, const Type W)
template<class Type, int Dim>
inline cVector<Type, Dim>::cVector(const Type X, const Type Y, const Type Z, const Type W) {
	assert(Dim == 4);
	v[0] = X;
	v[1] = Y;
	v[2] = Z;
	v[3] = W;
}

// cVector<Type, 3>.ctor : (const cVector<Type, 2> &XY, const Type Z)
template<class Type, int Dim>
inline cVector<Type, Dim>::cVector(const cVector<Type, 2> &XY, const Type Z) {
	assert(Dim == 3);
	v[0] = XY[0];
	v[1] = XY[1];
	v[2] = Z;
}

// cVector<Type, 4>.ctor : (const cVector<Type, 2> &XY, const Type Z, const Type W)
template<class Type, int Dim>
inline cVector<Type, Dim>::cVector(const cVector<Type, 2> &XY, const Type Z, const Type W) {
	assert(Dim == 4);
	v[0] = XY[0];
	v[1] = XY[1];
	v[2] = Z;
	v[3] = W;
}

// cVector<Type, 4>.ctor : (const cVector<Type, 3> &XYZ, const Type W)
template<class Type, int Dim>
inline cVector<Type, Dim>::cVector(const cVector<Type, 3> &XYZ, const Type W) {
	assert(Dim == 4);
	v[0] = XYZ[0];
	v[1] = XYZ[1];
	v[2] = XYZ[2];
	v[3] = W;
}

// cVector<Type, Dim>::operator = : cVector<Type, Dim> & (const cVector<Type, Dim> &Src)
template<class Type, int Dim>
inline cVector<Type, Dim> & cVector<Type, Dim>::operator = (const cVector<Type, Dim> &Src) {
	for(int i = 0; i < Dim; i++) {
		v[i] = Src[i];
	}
	return *this;
}

// cVector<Type, Dim>::Set : cVector<Type, Dim> & (const Type S)
template<class Type, int Dim>
inline cVector<Type, Dim> & cVector<Type, Dim>::Set(const Type S) {
	for(int i = 0; i < Dim; i++) {
		v[i] = S;
	}
	return *this;
}

// cVector<Type, 2>::Set : cVector<Type, 2> & (const Type X, const Type Y)
template<class Type, int Dim>
inline cVector<Type, 2> & cVector<Type, Dim>::Set(const Type X, const Type Y) {
	assert(Dim == 2);
	v[0] = X;
	v[1] = Y;
	return *this;
}

// cVector<Type, 3>::Set : cVector<Type, 3> & (const Type X, const Type Y, const Type Z)
template<class Type, int Dim>
inline cVector<Type, 3> & cVector<Type, Dim>::Set(const Type X, const Type Y, const Type Z) {
	assert(Dim == 3);
	v[0] = X;
	v[1] = Y;
	v[2] = Z;
	return *this;
}

// cVector<Type, 4>::Set : cVector<Type, 4> & (const Type X, const Type Y, const Type Z, const Type W)
template<class Type, int Dim>
inline cVector<Type, 4> & cVector<Type, Dim>::Set(const Type X, const Type Y, const Type Z, const Type W) {
	assert(Dim == 4);
	v[0] = X;
	v[1] = Y;
	v[2] = Z;
	v[3] = W;
	return *this;
}

// cVector<Type, 3>::Set : cVector<Type, 3> & (const cVector<Type, 2> &XY, const Type Z)
template<class Type, int Dim>
inline cVector<Type, 3> & cVector<Type, Dim>::Set(const cVector<Type, 2> &XY, const Type Z) {
	assert(Dim == 3);
	v[0] = XY[0];
	v[1] = XY[1];
	v[2] = Z;
	return *this;
}

// cVector<Type, 4>::Set : cVector<Type, 4> & (const cVector<Type, 2> &XY, const Type Z, const Type W)
template<class Type, int Dim>
inline cVector<Type, 4> & cVector<Type, Dim>::Set(const cVector<Type, 2> &XY, const Type Z, const Type W) {
	assert(Dim == 4);
	v[0] = XY[0];
	v[1] = XY[1];
	v[2] = Z;
	v[3] = W;
	return *this;
}

// cVector<Type, 4>::Set : cVector<Type, 4> & (const cVector<Type, 3> &XYZ, const Type W)
template<class Type, int Dim>
inline cVector<Type, 4> & cVector<Type, Dim>::Set(const cVector<Type, 3> &XYZ, const Type W) {
	assert(Dim == 4);
	v[0] = XYZ[0];
	v[1] = XYZ[1];
	v[2] = XYZ[2];
	v[3] = W;
	return *this;
}

// cVector<Type, Dim>::Copy : cVector<Type, Dim> & (const Type *pSrc)
template<class Type, int Dim>
inline cVector<Type, Dim> & cVector<Type, Dim>::Copy(const Type *pSrc) {
	assert(pSrc);
	for(int i = 0; i < Dim; i++) {
		v[i] = pSrc[i];
	}
	return *this;
}

// cVector<Type, Dim>::operator [] : Type & (const int index)
template<class Type, int Dim>
inline Type & cVector<Type, Dim>::operator [] (const int index) {
	assert(index >= 0 && index < Dim);
	return v[index];
}

// cVector<Type, Dim>::operator [] : const Type & (const int index) const
template<class Type, int Dim>
inline const Type & cVector<Type, Dim>::operator [] (const int index) const {
	assert(index >= 0 && index < Dim);
	return v[index];
}

// operator - : cVector<Type, Dim>(const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> operator - (const cVector<Type, Dim> &u) {
	cVector<Type, Dim> r;
	for(int i = 0; i < Dim; i++) {
		r[i] = - u[i];
	}
	return r;
}

// cVector<Type, Dim>::operator += : cVector<Type, Dim> & (const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> & cVector<Type, Dim>::operator += (const cVector<Type, Dim> &u) {
	for(int i = 0; i < Dim; i++) {
		v[i] += u[i];
	}
	return *this;
}

// cVector<Type, Dim>::operator -= : cVector<Type, Dim> & (const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> & cVector<Type, Dim>::operator -= (const cVector<Type, Dim> &u) {
	for(int i = 0; i < Dim; i++) {
		v[i] -= u[i];
	}
	return *this;
}

// cVector<Type, Dim>::operator *= : cVector<Type, Dim> & (const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> & cVector<Type, Dim>::operator *= (const cVector<Type, Dim> &u) {
	for(int i = 0; i < Dim; i++) {
		v[i] *= u[i];
	}
	return *this;
}

// cVector<Type, Dim>::operator *= : cVector<Type, Dim> & (const Type)
template<class Type, int Dim>
inline cVector<Type, Dim> & cVector<Type, Dim>::operator *= (const Type s) {
	for(int i = 0; i < Dim; i++) {
		v[i] *= s;
	}
	return *this;
}

// cVector<Type, Dim>::operator /= : cVector<Type, Dim> & (const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> & cVector<Type, Dim>::operator /= (const cVector<Type, Dim> &u) {
	for(int i = 0; i < Dim; i++) {
		v[i] /= u[i];
	}
	return *this;
}

// cVector<Type, Dim>::operator /= : cVector<Type, Dim> & (const Type)
template<class Type, int Dim>
inline cVector<Type, Dim> & cVector<Type, Dim>::operator /= (const Type s) {
	for(int i = 0; i < Dim; i++) {
		v[i] /= s;
	}
	return *this;
}

// cVector<Type, Dim>::operator + : cVector<Type, Dim>(const cVector<Type, Dim> &) const
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::operator + (const cVector<Type, Dim> &u) const {
	cVector<Type, Dim> r(*this);
	return r += u;
}

// cVector<Type, Dim>::operator - : cVector<Type, Dim>(const cVector<Type, Dim> &) const
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::operator - (const cVector<Type, Dim> &u) const {
	cVector<Type, Dim> r(*this);
	return r -= u;
}

// cVector<Type, Dim>::operator * : cVector<Type, Dim>(const cVector<Type, Dim> &) const
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::operator * (const cVector<Type, Dim> &u) const {
	cVector<Type, Dim> r(*this);
	return r *= u;
}

// cVector<Type, Dim>::operator * : cVector<Type, Dim>(const Type) const
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::operator * (const Type s) const {
	cVector<Type, Dim> r(*this);
	return r *= s;
}

// operator * : cVector<Type, Dim>(const Type, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> operator * (const Type s, const cVector<Type, Dim> &u) {
	cVector<Type, Dim> r(u);
	return r *= s;
}

// cVector<Type, Dim>::operator / : cVector<Type, Dim>(const cVector<Type, Dim> &) const
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::operator / (const cVector<Type, Dim> &u) const {
	cVector<Type, Dim> r(*this);
	return r /= u;
}

// cVector<Type, Dim>::operator / : cVector<Type, Dim>(const Type) const
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::operator / (const Type s) const {
	cVector<Type, Dim> r(*this);
	return r /= s;
}

// cVector<Type, Dim>::operator == : bool(const cVector<Type, Dim> &) const
template<class Type, int Dim>
inline bool cVector<Type, Dim>::operator == (const cVector<Type, Dim> &u) const {
	for(int i = 0; i < Dim; i++) {
		if(v[i] != u[i]) return false;
	}
	return true;
}

// cVector<Type, Dim>::operator != : bool(const cVector<Type, Dim> &) const
template<class Type, int Dim>
inline bool cVector<Type, Dim>::operator != (const cVector<Type, Dim> &u) const {
	for(int i = 0; i < Dim; i++) {
		if(v[i] != u[i]) return true;
	}
	return false;
}

// cVector<Type, Dim>::Equals : bool(const cVector<Type, Dim> &, const cVector<Type, Dim> &v, const Type)
template<class Type, int Dim>
inline bool cVector<Type, Dim>::Equals(const cVector<Type, Dim> &u, const cVector<Type, Dim> &v, const Type Eps) {
	assert(Epsilon >= (Type)0);
	for(int i = 0; i < Dim; i++) {
		if(cMath::Abs(u[i] - v[i]) > Eps) return false;
	}
	return true;
}

// cVector<Type, Dim>::LengthSq : Type() const
template<class Type, int Dim>
inline Type cVector<Type, Dim>::LengthSq() const {
	Type l2(0);
	for(int i = 0; i < Dim; i++) {
		l2 += (v[i] * v[i]);
	}
	return l2;
}

// cVector<Type, Dim>::Length : Type() const
template<class Type, int Dim>
inline Type cVector<Type, Dim>::Length() const {
	return cMath::Sqrt(LengthSq());
}

// cVector<Type, Dim>::Normalize : Type()
template<class Type, int Dim>
inline Type cVector<Type, Dim>::Normalize() {
	Type l = Length();
	if(l > 0) {
		Type il = (Type)1.0 / l;
		for(int i = 0; i < Dim; i++) {
			v[i] *= il;
		}
	}
	return l;
}

// cVector<Type, Dim>::IsNormalized : bool(const Type) const
template<class Type, int Dim>
inline bool cVector<Type, Dim>::IsNormalized(const Type Eps) const {
	return cMath::IsOne(LengthSq(), Eps);
}

// cVector<Type, Dim>::IsZero : bool(const Type) const
template<class Type, int Dim>
inline bool cVector<Type, Dim>::IsZero(const Type Eps) const {
	for(int i = 0; i < Dim; i++) {
		if(!cMath::IsZero(v[i], Eps)) return false;
	}
	return true;
}

// cVector<Type, Dim>::Abs : cVector<Type, Dim>(const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Abs(const cVector<Type, Dim> &u) {
	cVector<Type, Dim> r;
	for(int i = 0; i < Dim; i++) {
		r[i] = cMath::Abs(u[i]);
	}
	return r;
}

// cVector<Type, Dim>::Angle : Type(const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline Type cVector<Type, Dim>::Angle(const cVector<Type, Dim> &u, const cVector<Type, Dim> &v) {
	Type d = cVector<Type, Dim>::Dot(u, v);
	Type ll = u.Length() * v.Length();
	return cMath::ACos(d / ll);
}

// cVector<Type, Dim>::Clamp : cVector<Type, Dim>(const cVector<Type, Dim> &, const Type, const Type)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Clamp(const cVector<Type, Dim> &u, const Type Lo, const Type Hi) {
	cVector<Type, Dim> r;
	for(int i = 0; i < Dim; i++) {
		r[i] = u[i] < Lo ? Lo : (u[i] > Hi ? Hi : u[i]);
	}
	return r;
}

// cVector<Type, Dim>::Clamp : cVector<Type, Dim>(const cVector<Type, Dim> &, const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Clamp(const cVector<Type, Dim> &u, const cVector<Type, Dim> &Lo, const cVector<Type, Dim> &Hi) {
	cVector<Type, Dim> r;
	for(int i = 0; i < Dim; i++) {
		r[i] = u[i] < Lo[i] ? Lo[i] : (u[i] > Hi[i] ? Hi[i] : u[i]);
	}
	return r;
}

// cVector<Type, 3>::Cross : cVector<Type, 3>(const cVector<Type, 3> &, const cVector<Type, 3> &)
template<class Type, int Dim>
inline cVector<Type, 3> cVector<Type, Dim>::Cross(const cVector<Type, 3> &u, const cVector<Type, 3> &v) {
	return cVector<Type, 3>(u[1] * v[2] - u[2] * v[1], v[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
}

// cVector<Type, Dim>::DistanceSq : Type(const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline Type cVector<Type, Dim>::DistanceSq(const cVector<Type, Dim> &p, const cVector<Type, Dim> &q) {
	Type d2(0);
	for(int i = 0; i < Dim; i++) {
		d2 += Square(p[i] - q[i]);
	}
	return d2;
}

// cVector<Type, Dim>::Distance : Type(const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline Type cVector<Type, Dim>::Distance(const cVector<Type, Dim> &p, const cVector<Type, Dim> &q) {
	return cMath::Sqrt(DistanceSq(p, q));
}

// cVector<Type, Dim>::Dot : Type(const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline Type cVector<Type, Dim>::Dot(const cVector<Type, Dim> &u, const cVector<Type, Dim> &v) {
	Type d(0);
	for(int i = 0; i < Dim; i++) {
		d += u[i] * v[i];
	}
	return d;
}

// cVector<Type, Dim>::LengthSq : Type(const cVector<Type, Dim> &)
template<class Type, int Dim>
inline Type cVector<Type, Dim>::LengthSq(const cVector<Type, Dim> &u) {
	Type l2(0);
	for(int i = 0; i < Dim; i++) {
		l2 += u[i] * u[i];
	}
	return l2;
}

// cVector<Type, Dim>::Length : Type(const cVector<Type, Dim> &)
template<class Type, int Dim>
inline Type cVector<Type, Dim>::Length(const cVector<Type, Dim> &u) {
	return cMath::Sqrt(LengthSq(u));
}

// cVector<Type, Dim>::Lerp : cVector<Type, Dim>(const cVector<Type, Dim> &, const cVector<Type, Dim> &, const Type)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Lerp(const cVector<Type, Dim> &u, const cVector<Type, Dim> &v, const Type s) {
	cVector<Type, Dim> r;
	for(int i = 0; i < Dim; i++) {
		r[i] = u[i] + s * (v[i] - u[i]);
	}
	return r;
}

// cVector<Type, Dim>::Lerp : cVector<Type, Dim>(const cVector<Type, Dim> &, const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Lerp(const cVector<Type, Dim> &u, const cVector<Type, Dim> &v, const cVector<Type, Dim> &l) {
	cVector<Type, Dim> r;
	for(int i = 0; i < Dim; i++) {
		r[i] = u[i] + l[i] * (v[i] - u[i]);
	}
	return r;
}

// cVector<Type, Dim>::Lerp05 : cVector<Type, Dim>(const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Lerp05(const cVector<Type, Dim> &u, const cVector<Type, Dim> &v) {
	cVector<Type, Dim> r;
	for(int i = 0; i < Dim; i++) {
		r[i] = (Type)0.5 * (u[i] + v[i]);
	}
	return r;
}

// cVector<Type, Dim>::Max : cVector<Type, Dim>(const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Max(const cVector<Type, Dim> &u, const cVector<Type, Dim> &v) {
	cVector<Type, Dim> r;
	for(int i = 0; i < Dim; i++) {
		r[i] = u[i] > v[i] ? u[i] : v[i];
	}
	return r;
}

// cVector<Type, Dim>::Min : cVector<Type, Dim>(const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Min(const cVector<Type, Dim> &u, const cVector<Type, Dim> &v) {
	cVector<Type, Dim> r;
	for(int i = 0; i < Dim; i++) {
		r[i] = u[i] < v[i] ? u[i] : v[i];
	}
	return r;
}

// cVector<Type, Dim>::Normalize : cVector<Type, Dim>(const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Normalize(const cVector<Type, Dim> &u) {
	Type l = u.Length();
	if(l > 0) {
		Type il = (Type)1.0 / l;
		return u * il;
	}
	return u;
}

// cVector<Type, Dim>::Reflect : cVector<Type, Dim>(const cVector<Type, Dim> &, const cVector<Type, Dim> &)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Reflect(const cVector<Type, Dim> &RayDir, const cVector<Type, Dim> &Normal) {
	return RayDir - (Type)2.0 * cVector<Type, Dim>::Dot(RayDir, Normal) * Normal;
}

// cVector<Type, Dim>::Refract : cVector<Type, Dim>(const cVector<Type, Dim> &, const cVector<Type, Dim> &, const Type)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Refract(const cVector<Type, Dim> &RayDir, const cVector<Type, Dim> &Normal, const Type Eta) {
	Type k = (Type)1.0 - Eta * Eta * ((Type)1.0 - Square(cVector<Type, Dim>::Dot(Normal, RayDir)));
	if(k < (Type)0.0) return cVector<Type, Dim>((Type)0.0);
	return Eta * RayDir - (Eta * cVector<Type, Dim>::Dot(Normal, RayDir) + cMath::Sqrt(k)) * Normal;
}

// cVector<Type, Dim>::Truncate : cVector<Type, Dim>(const cVector<Type, Dim> &, const Type)
template<class Type, int Dim>
inline cVector<Type, Dim> cVector<Type, Dim>::Truncate(const cVector<Type, Dim> &u, const Type MaxLen) {
	if(MaxLen <= 0) {
		return cVector<Type, Dim>(0);
	}
	Type l = u.Length();
	if(l > MaxLen) {
		Type il = MaxLen / l;
		return u * il;
	}
	return u;
}

//---------------------------------------------------------------------------------------------------------------------------------
// cVector<Type, Dim>::DistanceToLineSegSq
//---------------------------------------------------------------------------------------------------------------------------------
template<class Type, int Dim>
inline Type cVector<Type, Dim>::DistanceToLineSegSq(const cVector<Type, Dim> &A, const cVector<Type, Dim> &B, Type *pScale) const {
	const cVector<Type, Dim> Ap = *this - A;
	const cVector<Type, Dim> AB = B - A;
	const Type l2 = AB.LengthSq();
	Type Scale;
	if(l2 < (Type)cMath::Epsilon) {
		Scale = (Type)0.5;
	} else {
		Scale = cVector<Type, Dim>::Dot(Ap, AB) / l2;
	}
	if(pScale) {
		*pScale = Scale;
	}
	if(Scale <= (Type)0.0) {
		return cVector<Type, Dim>::DistanceSq(*this, A);
	}
	if(Scale >= (Type)1.0) {
		return cVector<Type, Dim>::DistanceSq(*this, B);
	}
	return cVector<Type, Dim>::DistanceSq(*this, A + AB * Scale);
}

// cVector<Type, Dim>::ToPtr : Type * ()
template<class Type, int Dim>
inline Type * cVector<Type, Dim>::ToPtr() {
	return v;
}

// cVector<Type, Dim>::ToPtr : const Type * () const
template<class Type, int Dim>
inline const Type * cVector<Type, Dim>::ToPtr() const {
	return v;
}

// cVector<Type, >= 3>::To2 : cVector<Type, 2> & ()
template<class Type, int Dim>
inline cVector<Type, 2> & cVector<Type, Dim>::To2() {
	assert(Dim >= 3);
	return *reinterpret_cast<cVector<Type, 2> *>(this);
}

// cVector<Type, >= 3>::To2 : const cVector<Type, 2> & () const
template<class Type, int Dim>
inline const cVector<Type, 2> & cVector<Type, Dim>::To2() const {
	assert(Dim >= 3);
	return *reinterpret_cast<const cVector<Type, 2> *>(this);
}

// cVector<Type, >= 4>::To3 : cVector<Type, 3> & ()
template<class Type, int Dim>
inline cVector<Type, 3> & cVector<Type, Dim>::To3() {
	assert(Dim >= 4);
	return *reinterpret_cast<cVector<Type, 3> *>(this);
}

// cVector<Type, >= 4>::To3 : const cVector<Type, 3> & () const
template<class Type, int Dim>
inline const cVector<Type, 3> & cVector<Type, Dim>::To3() const {
	assert(Dim >= 4);
	return *reinterpret_cast<const cVector<Type, 3> *>(this);
}

} // comms