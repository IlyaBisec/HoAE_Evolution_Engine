#ifndef __cPlane__
#define __cPlane__

#include "cVector.h"

namespace comms {

class cPlane {
public:
	cVector Normal;
	double toOrig;

	cPlane();
	cPlane(const cVector &Normal, double toOrig);
	cPlane(const cVector &Normal, const cVector &Point);

	void Set(const cVector &Normal, double toOrig);
	void Set(const cVector &Normal, const cVector &Point);
	bool RayIntersect(const cVector &Orig, const cVector &Dir, double *pScale, cVector *pCross) const;
};

// .ctor : void()
inline cPlane::cPlane() {
}

// .ctor : void(const cVector &, double)
inline cPlane::cPlane(const cVector &Normal, double toOrig) {
	Set(Normal, toOrig);
}

// cPlane::Set : void(const cVector &, double)
inline void cPlane::Set(const cVector &Normal, double toOrig) {
	this->Normal = Normal;
	this->toOrig = toOrig;
}

// .ctor
inline cPlane::cPlane(const cVector &Normal, const cVector &Point) {
	Set(Normal, Point);
}

// cPlane::Set : void(const cVector &Normal, const cVector &Point)
inline void cPlane::Set(const cVector &Normal, const cVector &Point) {
	Set(Normal, Normal.Dot(Point));
}

// RayIntersect : bool(const cVector &, const cVector &, double *, cVector *) const
inline bool cPlane::RayIntersect(const cVector &Orig, const cVector &Dir, double *pScale, cVector *pCross) const {
	double d1 = Normal.Dot(Orig) - toOrig;
	double d2 = Normal.Dot(Dir);
	if(cUtils::IsZero(d2)) {
		return false;
	}
	double Scale = - d1 / d2;
	if(pScale) {
		*pScale = Scale;
	}
	if(pCross) {
		*pCross = Orig + Scale * Dir;
	}
	return true;
}

} // comms

#endif // __cPlane__