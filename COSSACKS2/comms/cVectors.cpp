#include "comms.h"

//-----------------------------------------------------------------------------
// cVectors2::CalcBoundBox : void(cVec2 &, cVec2 &) const
//-----------------------------------------------------------------------------
void cVectors2::CalcBoundBox(cVec2 &bbMin, cVec2 &bbMax) const {
	bbMin = bbMax = cVec2::Zero;
	if(Count()) {
		bbMin = bbMax = GetAt(0);
		for(int i = 1; i < Count(); i++) {
			const cVec2 &u = GetAt(i);
			bbMin = cVec2::Min(bbMin, u);
			bbMax = cVec2::Max(bbMax, u);
		}
	}
}

//-----------------------------------------------------------------------------
// cVectors2::Centre : cVec2() const
//-----------------------------------------------------------------------------
cVec2 cVectors2::Centre() const {
	cVec2 c(cVec2::Zero);
	if(Count()) {
		for(int i = 0; i < Count(); i++) {
			c += GetAt(i);
		}
		c /= float(Count());
	}
	return c;
}

//-----------------------------------------------------------------------------
// cVectors2::TransformCoordinate : cVectors2 & (const cMat3 &)
//-----------------------------------------------------------------------------
cVectors2 & cVectors2::TransformCoordinate(const cMat3 &T) {
	for(int i = 0; i < Count(); i++) {
		GetAt(i).TransformCoordinate(T);
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cVectors3::operator *= : cVectors3 & (const float)
//-----------------------------------------------------------------------------
cVectors3 & cVectors3::operator *= (const float s) {
	for(int i = 0; i < Count(); i++) {
		GetAt(i) *= s;
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cVectors3::operator += : cVectors3 & (const cVec3 &)
//-----------------------------------------------------------------------------
cVectors3 & cVectors3::operator += (const cVec3 &u) {
	for(int i = 0; i < Count(); i++) {
		GetAt(i) += u;
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cVectors3::operator -= : cVectors3 & (const cVec3 &)
//-----------------------------------------------------------------------------
cVectors3 & cVectors3::operator -= (const cVec3 &u) {
	for(int i = 0; i < Count(); i++) {
		GetAt(i) -= u;
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cVectors3::TransformCoordinate : cVectors3 & (const cMat4 &)
//-----------------------------------------------------------------------------
cVectors3 & cVectors3::TransformCoordinate(const cMat4 &M) {
	for(int i = 0; i < Count(); i++) {
		GetAt(i).TransformCoordinate(M);
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cVectors3::CalcBoundBox : void(cVec3 &, cVec3 &) const
//-----------------------------------------------------------------------------
void cVectors3::CalcBoundBox(cVec3 &bbMin, cVec3 &bbMax) const {
	bbMin = bbMax = cVec3::Zero;
	if(Count()) {
		bbMin = bbMax = GetAt(0);
		for(int i = 1; i < Count(); i++) {
			const cVec3 &u = GetAt(i);
			bbMin = cVec3::Min(bbMin, u);
			bbMax = cVec3::Max(bbMax, u);
		}
	}
}

//-----------------------------------------------------------------------------
// cVectors3::CalcBoundSphere : void(cVec3 &, float &) const
//-----------------------------------------------------------------------------
void cVectors3::CalcBoundSphere(cVec3 &Centre, float &Radius) const {
	cVec3 bbMin, bbMax;
	CalcBoundBox(bbMin, bbMax);
	Centre = (bbMin + bbMax) / 2;
	Radius = cVec3::Length(bbMax - Centre);
}

//-----------------------------------------------------------------------------
// cVectors3::ScaleTo : void(const float)
//-----------------------------------------------------------------------------
void cVectors3::ScaleTo(const float bbMaxDim) {
	cVec3 bbMin, bbMax;
	CalcBoundBox(bbMin, bbMax);
	cVec3 bbDim = bbMax - bbMin;
	float d = cMath::Max(bbDim.x, bbDim.y, bbDim.z);
	if(d > 0) {
		float Scale = bbMaxDim / d;
		(*this) *= Scale;
	}
}

//-----------------------------------------------------------------------------
// cVectors3::RemoveDups : void(DWORDS *)
//-----------------------------------------------------------------------------
void cVectors3::RemoveDups(DWORDS *pRemap) {
	const cVec3 Shifts[] = { cVec3::Zero, cVec3(0.5f, 0, 0), cVec3(0, 0.5f, 0), cVec3(0, 0, 0.5f),
		cVec3(0.5f, 0.5f, 0), cVec3(0.5f, 0, 0.5f), cVec3(0, 0.5f, 0.5f), cVec3(0.5f, 0.5f, 0.5f) };

	if(!Count()) return;
	if(pRemap) {
		pRemap->SetCount(Count());
		for(int i = 0; i < Count(); i++) {
			pRemap->SetAt(i, i);
		}
	}
	if(Count() < 2) return;

	DWORDS Map;
	for(int i = 0; i < 8; i++) {
		*this += Shifts[i];
		((cList<dVector>*)this)->RemoveDups(pRemap ? &Map : NULL);
		*this -= Shifts[i];
		if(pRemap) {
			for(i = 0; i < pRemap->Count(); i++) {
				pRemap->SetAt(i, Map[pRemap->GetAt(i)]);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// cVectors3::Centre : cVec3 () const
//-----------------------------------------------------------------------------
cVec3 cVectors3::Centre() const {
	cVec3 c = cVec3::Zero;
	if(Count()) {
		for(int i = 0; i < Count(); i++) {
			c += GetAt(i);
		}
		c /= float(Count());
	}
	return c;
}

//-----------------------------------------------------------------------------
// cVectors3::SwapYZ : void()
//-----------------------------------------------------------------------------
void cVectors3::SwapYZ() {
	for(int i = 0; i < Count(); i++) {
		cVec3 &u = GetAt(i);
		cMath::Swap(u.y, u.z);
	}
}

//-----------------------------------------------------------------------------
// dVectors::Centre : dVector () const
//-----------------------------------------------------------------------------
dVector dVectors::Centre() const {
	dVector c = dVector::Origin;
	if(Count()) {
		for(int i = 0; i < Count(); i++) {
			c += GetAt(i);
		}
		c /= double(Count());
	}
	return c;
}

//-----------------------------------------------------------------------------
// dVectors::CalcBoundBox : void(dVector &, dVector &) const
//-----------------------------------------------------------------------------
void dVectors::CalcBoundBox(dVector &bbMin, dVector &bbMax) const {
	bbMin = bbMax = dVector::Origin;
	if(Count()) {
		bbMin = bbMax = GetAt(0);
		for(int i = 1; i < Count(); i++) {
			const dVector &u = GetAt(i);
			u.UpdateBoundBox(bbMin, bbMax);
		}
	}
}

//-----------------------------------------------------------------------------
// dVectors::CalcBoundSphere : void(dVector &, double &) const
//-----------------------------------------------------------------------------
void dVectors::CalcBoundSphere(dVector &Centre, double &Radius) const {
	dVector bbMin, bbMax;
	CalcBoundBox(bbMin, bbMax);
	Centre = (bbMin + bbMax) / 2;
	Radius = (bbMax - Centre).Length();
}

//-----------------------------------------------------------------------------
// dVectors::CalcMinDist : double() const
//-----------------------------------------------------------------------------
double dVectors::CalcMinDist() const {
	double d = cMath::DoubleMaxValue;
	for(int i = 0; i < Count(); i++) {
		for(int j = 0; j < Count(); j++) {
			if(i == j) continue;
			d = cMath::Min(d, GetAt(i).DistTo(GetAt(j)));
		}
	}
	return d;
}

//-----------------------------------------------------------------------------
// dVectors::ScaleTo : void(double)
//-----------------------------------------------------------------------------
void dVectors::ScaleTo(double bbMaxDim) {
	dVector bbMin, bbMax;
	CalcBoundBox(bbMin, bbMax);
	dVector bbDim = bbMax - bbMin;
	double Scale = bbMaxDim / cMath::Max(bbDim.x, bbDim.y, bbDim.z);
	(*this) *= Scale;
}

//-----------------------------------------------------------------------------
// dVectors::RemoveDups : void(DWORDS *)
//-----------------------------------------------------------------------------
void dVectors::RemoveDups(DWORDS *pRemap) {
	const dVector Shifts[] = { dVector::Origin, dVector(0.5, 0, 0), dVector(0, 0.5, 0), dVector(0, 0, 0.5),
		dVector(0.5, 0.5, 0), dVector(0.5, 0, 0.5), dVector(0, 0.5, 0.5), dVector(0.5, 0.5, 0.5) };

	if(!Count()) return;
	if(pRemap) {
		pRemap->SetCount(Count());
		for(int i = 0; i < Count(); i++) {
			pRemap->SetAt(i, i);
		}
	}
	if(Count() < 2) return;

	DWORDS Map;
	for(int i = 0; i < 8; i++) {
		*this += Shifts[i];
		((cList<dVector>*)this)->RemoveDups(pRemap ? &Map : NULL);
		*this -= Shifts[i];
		if(pRemap) {
			for(i = 0; i < pRemap->Count(); i++) {
				pRemap->SetAt(i, Map[pRemap->GetAt(i)]);
			}
		}
	}
}