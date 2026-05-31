#include "comms.h"

//-----------------------------------------------------------------------------
// cBounds::FromPoints : cBounds(const cVec2 *, const int)
//-----------------------------------------------------------------------------
cBounds cBounds::FromPoints(const cVec2 *pPoints, const int nCount) {
	assert(pPoints);
	assert(nCount > 0);

	cVec2 bMin(pPoints[0]), bMax(pPoints[0]);
	for(int i = 1; i < nCount; i++) {
		bMin = cVec2::Min(bMin, pPoints[i]);
		bMax = cVec2::Max(bMax, pPoints[i]);
	}

	return cBounds(cVec3(bMin, 0.0f), cVec3(bMax, 0.0f));
}

//-----------------------------------------------------------------------------
// cBounds::FromPoints : cBounds(const cVec3 *, const int)
//-----------------------------------------------------------------------------
cBounds cBounds::FromPoints(const cVec3 *pPoints, const int nCount) {
	assert(pPoints);
	assert(nCount > 0);

	cVec3 bMin(pPoints[0]), bMax(pPoints[0]);
	for(int i = 1; i < nCount; i++) {
		bMin = cVec3::Min(bMin, pPoints[i]);
		bMax = cVec3::Max(bMax, pPoints[i]);
	}

	return cBounds(bMin, bMax);
}

//-----------------------------------------------------------------------------
// cBounds::DistanceToPointSq : float(const cVec2 &) const
//-----------------------------------------------------------------------------
float cBounds::DistanceToPointSq(const cVec2 &p) const {
	if(ContainsPoint(p)) return 0.0f;

	float d2 = 0.0f;
	for(int i = 0; i < 2; i++) {
		if(p[i] < m_Min[i]) {
			d2 += cMath::Square(m_Min[i] - p[i]);
		} else if(p[i] > m_Max[i]) {
			d2 += cMath::Square(p[i] - m_Max[i]);
		}
	}
	return d2;
}

//-----------------------------------------------------------------------------
// cBounds::DistanceToPointSq : float(const cVec3 &) const
//-----------------------------------------------------------------------------
float cBounds::DistanceToPointSq(const cVec3 &p) const {
	if(ContainsPoint(p)) return 0.0f;

	float d2 = 0.0f;
	for(int i = 0; i < 3; i++) {
		if(p[i] < m_Min[i]) {
			d2 += cMath::Square(m_Min[i] - p[i]);
		} else if(p[i] > m_Max[i]) {
			d2 += cMath::Square(p[i] - m_Max[i]);
		}
	}
	return d2;
}