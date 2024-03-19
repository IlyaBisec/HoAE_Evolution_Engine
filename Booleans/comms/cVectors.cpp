#include "cVectors.h"

namespace comms {

// cVectors::Centre
cVector cVectors::Centre() const {
	cVector c = cVector::Origin;
	if(m_nUsed) {
		c.Dim(m_pList[0].Dim());
		for(int i = 0; i < m_nUsed; i++) {
			c += m_pList[i];
		}
		c /= double(m_nUsed);
	}
	return c;
} // cVectors::Centre

// cVectors::BBox
void cVectors::BBox(cVector &bbMin, cVector &bbMax) const {
	bbMin = bbMax = cVector::Origin;
	if(m_nUsed) {
		const cVector &r = m_pList[0];
		int nDim = r.Dim(), i;
		switch(nDim) {
			case 3:
				bbMin.z = bbMax.z = r.z;
				for(i = 1; i < m_nUsed; i++) {
					const cVector &v = m_pList[i];
					bbMin.z = Min(bbMin.z, v.z);
					bbMax.z = Max(bbMax.z, v.z);
				}
			case 2:
				bbMin.y = bbMax.y = r.y;
				for(i = 1; i < m_nUsed; i++) {
					const cVector &v = m_pList[i];
					bbMin.y = Min(bbMin.y, v.y);
					bbMax.y = Max(bbMax.y, v.y);
				}
			case 1:
				bbMin.x = bbMax.x = r.x;
				for(i = 1; i < m_nUsed; i++) {
					const cVector &v = m_pList[i];
					bbMin.x = Min(bbMin.x, v.x);
					bbMax.x = Max(bbMax.x, v.x);
				}
				bbMin.Dim(nDim);
				bbMax.Dim(nDim);
				break;
			default:
				assert(nDim >= 1 && nDim <= 3);
		}
	}
} // cVectors::BBox

// cVectors::BRadius
void cVectors::BRadius(cVector &brCentre, double &brRadius) const {
	brCentre = Centre();
	cVector bbMin, bbMax;
	BBox(bbMin, bbMax);
	cVector b = bbMax - bbMin;
	brRadius = sqrt(Square(b.x / 2.) + Square(b.y / 2.) + Square(b.z / 2.));
} // cVectors::BRadius


} // comms