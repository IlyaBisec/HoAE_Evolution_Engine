#ifndef __CVECTORS__
#define __CVECTORS__

#include "cVector.h"
#include "cMatrix.h"

namespace comms {

class cVectors : public cList<cVector>
{
public:
	cVectors & operator *= (double);
	cVectors & operator /= (double);
	cVectors & operator *= (const cVector &);
	cVectors & operator /= (const cVector &);
	cVectors & operator += (const cVector &);
	cVectors & operator -= (const cVector &);
	cVectors & operator ^= (const cVector &);
	cVectors & operator *= (const cMatrix &);
	int Dim() const;
	int Dim(int n);
	void Normalize();
	// Special operations:
	cVector Centre() const;
	void BBox(cVector &bbMin, cVector &bbMax) const;
	void BRadius(cVector &brCentre, double &brRadius) const;
};

// cVectors::operator *= (double)
inline cVectors & cVectors::operator *= (double c) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] *= c;
	return *this;
}

// cVectors::operator /= (double)
inline cVectors & cVectors::operator /= (double c) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] /= c;
	return *this;
}

// cVectors::operator *= (const cVector &)
inline cVectors & cVectors::operator *= (const cVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] *= u;
	return *this;
}

// cVectors::operator /= (const cVector &)
inline cVectors & cVectors::operator /= (const cVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] /= u;
	return *this;
}

// cVectors::operator += (const cVector &)
inline cVectors & cVectors::operator += (const cVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] += u;
	return *this;
}

// cVectors::operator -= (const cVector &)
inline cVectors & cVectors::operator -= (const cVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] -= u;
	return *this;
}

// cVectors::operator ^= (const cVector &)
inline cVectors & cVectors::operator ^= (const cVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] ^= u;
	return *this;
}

// cVectors::operator *= (const cMatrix &)
inline cVectors & cVectors::operator *= (const cMatrix &M) {
	for(int i = 0; i < m_nUsed; i++)
		m_pList[i] *= M;
	return *this;
}

// cVectors::Dim
inline int cVectors::Dim() const {
	assert(m_nUsed > 0);
	if(m_nUsed <= 0) return -1;
	return m_pList[0].Dim();
} // cVectors::Dim

// cVectors::Dim(int)
inline int cVectors::Dim(int n) {
	assert(m_nUsed > 0);
	if(m_nUsed <= 0) return -1;
	for(int i = 0; i < m_nUsed; i++) m_pList[i].Dim(n);
	return m_pList[0].Dim();
}

// cVectors::Normalize
inline void cVectors::Normalize() {
	assert(m_nUsed > 0);
	for(int i = 1; i < m_nUsed; i++) m_pList[i].Normalize();
	m_pList[i].Normalize();
}

} // comms

#endif