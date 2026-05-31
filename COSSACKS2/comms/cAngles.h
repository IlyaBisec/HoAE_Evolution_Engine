#pragma once

class cAngles {
public:
	float Pitch; // Up / Down
	float Yaw; // Left / Right
	float Roll; // Rotate clockwise
	
	cAngles();
	cAngles(const float Pitch, const float Yaw, const float Roll);
	explicit cAngles(const cVec3 &u);
	
	cAngles & Set(const float Pitch, const float Yaw, const float Roll);
	cAngles & SetZero();

	float & operator [] (const int index);
	float operator [] (const int index) const;

	cAngles operator - () const;

	cAngles & operator += (const cAngles &);
	cAngles & operator -= (const cAngles &);
	cAngles & operator *= (const float);
	cAngles & operator /= (const float);
	
	cAngles operator + (const cAngles &) const;
	cAngles operator - (const cAngles &) const;
	cAngles operator * (const float) const;
	friend cAngles operator * (const float, const cAngles &);
	cAngles operator / (const float) const;

	bool operator == (const cAngles &) const;
	bool operator != (const cAngles &) const;
	static bool Equals(const cAngles &, const cAngles &, const float Eps = cMath::Epsilon);
	
	float Length() const;
	float LengthSq() const;
	
	cAngles & Normalize360();
	cAngles & Normalize180();
	
	static cAngles Clamp(const cAngles &A, const cAngles &Min, const cAngles &Max);
	static cAngles Rand(const cAngles &Range);
	static float Distance(const cAngles &A, const cAngles &B);
	
	static const cAngles Zero;
	
	int GetDimension() const;
	
	// Yaw around world Y - Axis:
	void ToVectors(cVec3 *Forward, cVec3 *Right = NULL, cVec3 *Up = NULL) const;
	const cVec3 ToForward() const;

	const cMat3 ToMat3() const;
	const cMat4 ToMat4() const;

	// Yaw around world Z - Axis:
	void ToVectorsZ(cVec3 *Forward, cVec3 *Right = NULL, cVec3 *Up = NULL) const;
	const cVec3 ToForwardZ() const;
	
	const cMat3 ToMat3Z() const;
	const cMat4 ToMat4Z() const;
	
	cQuat ToQuat() const;
//	cRotation ToRotation() const;

	
	//	cVec3 ToAngularVelocity() const;

	const float * ToFloatPtr() const;
	float * ToFloatPtr();
	const cStr ToStr(const int nPrec = 2) const;
};

// cAngles.ctor : ()
inline cAngles::cAngles() {
}

// cAngles.ctor : (const float, const float, const float)
inline cAngles::cAngles(const float Pitch, const float Yaw, const float Roll) {
	this->Pitch = Pitch;
	this->Yaw = Yaw;
	this->Roll = Roll;
}

// cAngles.ctor : (const cVec3 &)
inline cAngles::cAngles(const cVec3 &u)
: Pitch(u[0]), Yaw(u[1]), Roll(u[2]) {}

// cAngles::Set : cAngles & (const float, const float, const float)
inline cAngles & cAngles::Set(const float Pitch, const float Yaw, const float Roll) {
	this->Pitch = Pitch;
	this->Yaw = Yaw;
	this->Roll = Roll;
	return *this;
}

// cAngles::SetZero : cAngles & ()
inline cAngles & cAngles::SetZero() {
	Pitch = Yaw = Roll = 0.0f;
	return *this;
}

// cAngles::operator [] : float & (const int)
inline float & cAngles::operator [] (const int index) {
	cAssert(index >= 0 && index < 3);
	return (&Pitch)[index];
}

// cAngles::operator [] : float(const int) const
inline float cAngles::operator [] (const int index) const {
	cAssert(index >= 0 && index < 3);
	return (&Pitch)[index];
}

// cAngles::operator - : cAngles() const
inline cAngles cAngles::operator - () const {
	return cAngles(-Pitch, -Yaw, -Roll);
}

// cAngles::operator += : cAngles & (const cAngles &)
inline cAngles & cAngles::operator += (const cAngles &A) {
	Pitch += A.Pitch;
	Yaw += A.Yaw;
	Roll += A.Roll;
	return *this;
}

// cAngles::operator -= : cAngles & (const cAngles &)
inline cAngles & cAngles::operator -= (const cAngles &A) {
	Pitch -= A.Pitch;
	Yaw -= A.Yaw;
	Roll -= A.Roll;
	return *this;
}

// cAngles::operator *= : cAngles & (const float)
inline cAngles & cAngles::operator *= (const float s) {
	Pitch *= s;
	Yaw *= s;
	Roll *= s;
	return *this;
}

// cAngles::operator /= : cAngles & (const float)
inline cAngles & cAngles::operator /= (const float s) {
	float is = 1.0f / s;
	Pitch *= is;
	Yaw *= is;
	Roll *= is;
	return *this;
}

// cAngles::operator + : cAngles(const cAngles &) const
inline cAngles cAngles::operator + (const cAngles &A) const {
	return cAngles(Pitch + A.Pitch, Yaw + A.Yaw, Roll + A.Roll);
}

// cAngles::operator - : cAngles(const cAngles &) const
inline cAngles cAngles::operator - (const cAngles &A) const {
	return cAngles(Pitch - A.Pitch, Yaw - A.Yaw, Roll - A.Roll);
}

// cAngles::operator * : cAngles(const float) const
inline cAngles cAngles::operator * (const float s) const {
	return cAngles(Pitch * s, Yaw * s, Roll * s);
}

// cAngles::friend operator * : cAngles(const float, const cAngles &)
inline cAngles operator * (const float s, const cAngles &A) {
	return cAngles(s * A.Pitch, s * A.Yaw, s * A.Roll);
}

// cAngles::operator / : cAngles(const float) const
inline cAngles cAngles::operator / (const float s) const {
	float is = 1.0f / s;
	return cAngles(Pitch * is, Yaw * is, Roll * is);
}

// cAngles::operator == : bool(const cAngles &) const
inline bool cAngles::operator == (const cAngles &A) const {
	return Pitch == A.Pitch && Yaw == A.Yaw && Roll == A.Roll;
}

// cAngles::operator != : bool(const cAngles &) const
inline bool cAngles::operator != (const cAngles &A) const {
	return Pitch != A.Pitch || Yaw != A.Yaw || Roll != A.Roll;
}

// cAngles::Equals : bool(const cAngles &, const cAngles &, const float)
inline bool cAngles::Equals(const cAngles &A, const cAngles &B, const float Eps) {
	if(cMath::Abs(A.Pitch - B.Pitch) > Eps) return false;
	if(cMath::Abs(A.Yaw - B.Yaw) > Eps) return false;
	if(cMath::Abs(A.Roll - B.Roll) > Eps) return false;
	return true;
}

// cAngles::Length : float() const
inline float cAngles::Length() const {
	return cMath::Sqrt(Pitch * Pitch + Yaw * Yaw + Roll * Roll);
}

// cAngles::LengthSq : float() const
inline float cAngles::LengthSq() const {
	return Pitch * Pitch + Yaw * Yaw + Roll * Roll;
}

// cAngles::Clamp : cAngles(const cAngles &, const cAngles &, const cAngles &) const
inline cAngles cAngles::Clamp(const cAngles &A, const cAngles &Min, const cAngles &Max) {
	cAngles r;
	r.Pitch = A.Pitch < Min.Pitch ? Min.Pitch : (A.Pitch > Max.Pitch ? Max.Pitch : A.Pitch);
	r.Yaw = A.Yaw < Min.Yaw ? Min.Yaw : (A.Yaw > Max.Yaw ? Max.Yaw : A.Yaw);
	r.Roll = A.Roll < Min.Roll ? Min.Roll : (A.Roll > Max.Roll ? Max.Roll : A.Roll);
	return r;
}

// cAngles::Rand : cAngles(const cAngles &)
inline cAngles cAngles::Rand(const cAngles &Range) {
	float Pitch = Range.Pitch * cMath::RandRange1();
	float Yaw = Range.Yaw * cMath::RandRange1();
	float Roll = Range.Roll * cMath::RandRange1();
	return cAngles(Pitch, Yaw, Roll);
}

// cAngles::Distance : float(const cAngles &, const cAngles &)
inline float cAngles::Distance(const cAngles &A, const cAngles &B) {
	return cVec3::Angle(A.ToForward(), B.ToForward());
}

// cAngles::GetDimension : int() const
inline int cAngles::GetDimension() const {
	return 3;
}

// cAngles::ToFloatPtr : const float * () const
inline const float * cAngles::ToFloatPtr() const {
	return &Pitch;
}

// cAngles::ToFloatPtr : float * ()
inline float * cAngles::ToFloatPtr() {
	return &Pitch;
}