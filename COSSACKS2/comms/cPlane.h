#pragma once

class cPlane {
public:
	float a, b, c, d;

	cPlane();
	cPlane(float A, float B, float C, float D);
	cPlane(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2); // from points
	cPlane(const cVec3 &Pt, const cVec3 &Normal); // from normal and point
	cPlane(const cVec3 &Normal, float ToOrig); // from normal and distance to origin

	const cVec3 & GetNormal() const;
	void SetNormal(const cVec3 &);
	cVec3 & MutableNormal();

	void SetDistanceToOrig(float);

	float SetFromPoints(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2);
	void SetFromPointAndNormal(const cVec3 &Pt, const cVec3 &Normal);
	
	void MoveToPoint(const cVec3 &p);
	
	float SignedDistanceToOrig() const;
	float SignedDistanceToPoint(const cVec3 &p) const;

	enum eSide {
		S_FRONT, S_BACK, S_CROSS, S_ON
	};
	
	const cVec3 ProjectPoint(const cVec3 &p) const;
	const cVec3 ProjectVector(const cVec3 &u) const;
	
	eSide ClassifyPoint(const cVec3 &p, const float Eps = cMath::SpaceEpsilon) const;
	eSide ClassifySphere(const cSphere &S, const float Eps = cMath::SpaceEpsilon) const;

	bool IsFrontFacingTo(const cVec3 &Dir) const;
	bool RayIntersection(const cVec3 &RayOrig, const cVec3 &RayDir, float *pScale = NULL, cVec3 *pCross = NULL) const;
	bool LineIntersection(const cVec3 &A, const cVec3 &B, cVec3 *pCross = NULL) const;
	bool PlaneIntersection(const cPlane &P, cVec3 *pCross = NULL, cVec3 *pDir = NULL) const;

	float * ToPtr();
	const float * ToPtr() const;

	cVec4 & ToVec4();
	const cVec4 & ToVec4() const;

	const cStr ToStr(const int nPrec = 2) const;
};

// .ctor : ()
inline cPlane::cPlane() {
}

// .ctor : (float, float, float, float)
inline cPlane::cPlane(float A, float B, float C, float D)
: a(A), b(B), c(C), d(D) {}

// .ctor : (const cVec3 &, const cVec3 &, const cVec3 &)
inline cPlane::cPlane(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2) {
	SetFromPoints(t0, t1, t2);
}

// .ctor : (const cVec3 &, const cVec3 &)
inline cPlane::cPlane(const cVec3 &Pt, const cVec3 &Normal) {
	SetFromPointAndNormal(Pt, Normal);
}

// .ctor : (const cVec3 &, float)
inline cPlane::cPlane(const cVec3 &Normal, float ToOrig)
: a(Normal.x), b(Normal.y), c(Normal.z), d(-ToOrig) {}

// cPlane::GetNormal : const cVec3 & () const
inline const cVec3 & cPlane::GetNormal() const {
	return *reinterpret_cast<const cVec3 *>(&a);
}

// cPlane::SetNormal : void(const cVec3 &)
inline void cPlane::SetNormal(const cVec3 &Normal) {
	a = Normal.x;
	b = Normal.y;
	c = Normal.z;
}

// cPlane::MutableNormal : cVec3 & ()
inline cVec3 & cPlane::MutableNormal() {
	return *reinterpret_cast<cVec3 *>(&a);
}

// cPlane::SetDistanceToOrig : void(float)
inline void cPlane::SetDistanceToOrig(float ToOrig) {
	d = - ToOrig;
}

// cPlane::SetFromPoints : float(const cVec3 &, const cVec3 &, const cVec3 &)
inline float cPlane::SetFromPoints(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2) {
	SetNormal(cVec3::Cross(t1 - t0, t2 - t0));
	const float AreaSq = GetNormal().LengthSq();
	if(AreaSq < cMath::EpsilonSq) {
		a = b = c = d = 0.0f;
		return 0.0f;
	} else {
		const float Area = cMath::Sqrt(AreaSq);
		MutableNormal() *= 1.0f / Area;
		d = - cVec3::Dot(GetNormal(), t0);
		return Area;
	}
}

// cPlane::SetFromPointAndNormal : void(const cVec3 &, const cVec3 &)
inline void cPlane::SetFromPointAndNormal(const cVec3 &Pt, const cVec3 &Normal) {
	SetNormal(Normal);
	d = - cVec3::Dot(Normal, Pt);
}

// cPlane::MoveToPoint
inline void cPlane::MoveToPoint(const cVec3 &p) {
	d = - cVec3::Dot(GetNormal(), p);
}

// cPlane::SignedDistanceToOrig : float () const
inline float cPlane::SignedDistanceToOrig() const {
	return - d;
}

// cPlane::SignedDistanceToPoint : float(const cVec3 &) const
inline float cPlane::SignedDistanceToPoint(const cVec3 &p) const {
	return cVec3::Dot(GetNormal(), p) + d;
}

// cPlane::ProjectPoint
inline const cVec3 cPlane::ProjectPoint(const cVec3 &p) const {
	return p - SignedDistanceToPoint(p) * GetNormal();
}

// cPlane::ProjectVector
inline const cVec3 cPlane::ProjectVector(const cVec3 &u) const {
	return u - cVec3::Dot(u, GetNormal()) * GetNormal();
}


// cPlane::IsFrontFacingTo : bool(const cVec3 &) const
inline bool cPlane::IsFrontFacingTo(const cVec3 &Dir) const {
	return cVec3::Dot(GetNormal(), Dir) <= 0.0f;
}

// cPlane::RayIntersection : bool(const cVec3 &, const cVec3 &, float *, cVec3 *) const
inline bool cPlane::RayIntersection(const cVec3 &RayOrig, const cVec3 &RayDir, float *pScale, cVec3 *pCross) const {
	float d1 = cVec3::Dot(GetNormal(), RayOrig) + d;
	float d2 = cVec3::Dot(GetNormal(), RayDir);

	if(d2 == 0.0) {
		return false;
	}
	float Scale = - d1 / d2;
	if(pScale) {
		*pScale = Scale;
	}
	if(pCross) {
		*pCross = RayOrig + Scale * RayDir;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------
// cPlane::LineIntersection : bool(const cVec3 &, const cVec3 &, cVec3 *) const
//--------------------------------------------------------------------------------------------------
inline bool cPlane::LineIntersection(const cVec3 &A, const cVec3 &B, cVec3 *pCross) const {
	float d1 = cVec3::Dot(GetNormal(), A) + d;
	float d2 = cVec3::Dot(GetNormal(), B) + d;
	
	if(d1 > 0.0f && d2 > 0.0f) {
		return false;
	}
	if(d1 < 0.0f && d2 < 0.0f) {
		return false;
	}
	float fr = d1 / (d1 - d2);
	if(fr >= 0.0f && fr <= 1.0f) {
		if(pCross) {
			cVec3 u = cVec3::Normalize(B - A);
			*pCross = A + fr * u;
			// consider this: *pCross = d1 * B - d2 * A / (d1 - d2);
		}
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------
// cPlane::PlaneIntersection
//--------------------------------------------------------------------------------------
inline bool cPlane::PlaneIntersection(const cPlane &P, cVec3 *pCross, cVec3 *pDir) const {
	double n00 = GetNormal().LengthSq();
	double n01 = cVec3::Dot(GetNormal(), P.GetNormal());
	double n11 = P.GetNormal().LengthSq();
	double Det = n00 * n11 - n01 * n01;
	
	if(cMath::IsZero(Det)) {
		return false;
	}
	
	if(pDir) {
		*pDir = cVec3::Cross(GetNormal(), P.GetNormal());
	}
	if(pCross) {
		double iDet = 1.0 / Det;
		float f0 = float((n01 * P.d - n11 * d) * iDet);
		float f1 = float((n01 * d - n00 * P.d) * iDet);
		
		*pCross = f0 * GetNormal() + f1 * P.GetNormal();
	}
	return true;
}

//------------------------------------------------------------------------------------
// cPlane::ClassifyPoint : cPlane::eSide(const cVec3 &, const float) const
//------------------------------------------------------------------------------------
inline cPlane::eSide cPlane::ClassifyPoint(const cVec3 &p, const float Eps) const {
	float d = SignedDistanceToPoint(p);
	if(d > Eps) {
		return S_FRONT;
	}
	if(d < - Eps) {
		return S_BACK;
	}
	return S_ON;
}

//------------------------------------------------------------------------------------
// cPlane::ClassifySphere : cPlane::eSide(const cSphere &, const float) const
//------------------------------------------------------------------------------------
inline cPlane::eSide cPlane::ClassifySphere(const cSphere &S, const float Eps) const {
	float d = SignedDistanceToPoint(S.GetCenter());
	if(d > S.GetRadius() + Eps) {
		return S_FRONT;
	}
	if(d < - S.GetRadius() - Eps) {
		return S_BACK;
	}
	return S_CROSS;
}

// cPlane::ToVec4 : const cVec4 & () const
inline const cVec4 & cPlane::ToVec4() const {
	return *reinterpret_cast<const cVec4 *>(&a);
}

// cPlane::ToVec4 : cVec4 & ()
inline cVec4 & cPlane::ToVec4() {
	return *reinterpret_cast<cVec4 *>(&a);
}

// cPlane::ToPtr : const float * () const
inline const float * cPlane::ToPtr() const {
	return reinterpret_cast<const float *>(&a);
}

// cPlane::ToPtr : float * ()
inline float * cPlane::ToPtr() {
	return reinterpret_cast<float *>(&a);
}




class dPlane {
public:
	double a, b, c, d;

	dPlane();
	dPlane(double A, double B, double C, double D);
	dPlane(const dVector &t0, const dVector &t1, const dVector &t2); // from points
	dPlane(const dVector &Normal, const dVector &Pt); // from normal and point
	dPlane(const dVector &Normal, double Offset); // from normal and offset

	void SetNormal(const dVector &);
	void SetOffset(double);
	dVector Normal() const;
	// should be: const dVector & Normal() const; dVector & Normal();
	double Offset() const;

	double SetFromPoints(const dVector &t0, const dVector &t1, const dVector &t2);
	void SetFromNormalAndPoint(const dVector &Normal, const dVector &Pt);
	
	double SignedDistanceTo(const dVector &Pt) const;
	bool RayIntersection(const dVector &RayOrig, const dVector &RayDir, double *pScale, dVector *pCross) const;
	bool LineIntersection(const dVector &A, const dVector &B) const;
};

// .ctor : ()
inline dPlane::dPlane() {
}

// .ctor : (double, double, double, double)
inline dPlane::dPlane(double A, double B, double C, double D)
: a(A), b(B), c(C), d(D) {}

// .ctor : (const dVector &, const dVector &, const dVector &)
inline dPlane::dPlane(const dVector &t0, const dVector &t1, const dVector &t2) {
	SetFromPoints(t0, t1, t2);
}

// .ctor : (const dVector &, const dVector &)
inline dPlane::dPlane(const dVector &Normal, const dVector &Pt) {
	SetFromNormalAndPoint(Normal, Pt);
}

// .ctor : (const dVector &, double)
inline dPlane::dPlane(const dVector &Normal, double Offset)
: a(Normal.x), b(Normal.y), c(Normal.z), d(- Offset) {}

// dPlane::SetNormal : void(const dVector &)
inline void dPlane::SetNormal(const dVector &Normal) {
	a = Normal.x;
	b = Normal.y;
	c = Normal.z;
}

// dPlane::SetOffset : void(double)
inline void dPlane::SetOffset(double Offset) {
	d = - Offset;
}

// dPlane::Normal : const dVector & () const
inline dVector dPlane::Normal() const {
	return dVector(a, b, c);
//	should be: return *reinterpret_cast<const dVector *>(&a);
}

// dPlane::Offset : double () const
inline double dPlane::Offset() const {
	return - d;
}

// dPlane::SetFromPoints : double(const dVector &, const dVector &, const dVector &)
inline double dPlane::SetFromPoints(const dVector &t0, const dVector &t1, const dVector &t2) {
	SetNormal((t1 - t0).Cross(t2 - t0));
	const double AreaSq = Normal().LengthSq();
	if(AreaSq < cMath::Square(cMath::dEpsilon)) {
		a = b = c = d = 0.0;
		return 0.0;
	} else {
		const double Area = sqrt(AreaSq);
		SetNormal(Normal() * (1.0 / Area));
		d = - Normal().Dot(t0);
		return Area;
	}
}

// dPlane::SetFromNormalAndPoint : void (const dVector &, const dVector &)
inline void dPlane::SetFromNormalAndPoint(const dVector &Normal, const dVector &Pt) {
	SetNormal(Normal);
	d = - Normal.Dot(Pt);
}

// Consider this!
// dPlane::Normal : dVector & ()
//inline dVector & dPlane::Normal() {
//	return *reinterpret_cast<dVector *>(&a);
//}

// dPlane::SignedDistanceTo : double(const dVector &) const
inline double dPlane::SignedDistanceTo(const dVector &Pt) const {
	return a * Pt.x + b * Pt.y + c * Pt.z + d;
}

// dPlane::RayIntersection : bool(const dVector &, const dVector &, double *, dVector *) const
inline bool dPlane::RayIntersection(const dVector &RayOrig, const dVector &RayDir, double *pScale, dVector *pCross) const {
	double d1 = Normal().Dot(RayOrig) + d;
	double d2 = Normal().Dot(RayDir);

	if(d2 == 0.0) {
		return false;
	}
	double Scale = - d1 / d2;
	if(pScale) {
		*pScale = Scale;
	}
	if(pCross) {
		pCross->Copy(RayOrig + Scale * RayDir);
	}
	return true;
}

// dPlane::LineIntersection : bool(const dVector &, const dVector &) const
inline bool dPlane::LineIntersection(const dVector &A, const dVector &B) const {
	double d1 = Normal().Dot(A) + d;
	double d2 = Normal().Dot(B) + d;

	if(d1 > 0.0 && d2 > 0.0) {
		return false;
	}
	if(d1 < 0.0 && d2 < 0.0) {
		return false;
	}
	double fr = d1 / (d1 - d2);
	return fr >= 0.0 && fr <= 1.0;
}