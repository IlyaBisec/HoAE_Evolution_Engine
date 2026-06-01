#pragma once

class cVectors2 : public cList<cVec2> {
public:
	void CalcBoundBox(cVec2 &bbMin, cVec2 &bbMax) const;
	cVec2 Centre() const;
	cVectors2 & TransformCoordinate(const cMat3 &T);
};

class cVectors3 : public cList<cVec3> {
public:
	cVectors3 & operator *= (const float);
	cVectors3 & operator += (const cVec3 &);
	cVectors3 & operator -= (const cVec3 &);
	cVectors3 & TransformCoordinate(const cMat4 &);
	cVec3 Centre() const;
	void CalcBoundBox(cVec3 &bbMin, cVec3 &bbMax) const;
	void CalcBoundSphere(cVec3 &Centre, float &Radius) const;
	void ScaleTo(const float bbMaxDim);
	void RemoveDups(DWORDS *pRemap = NULL);
	void SwapYZ();
};

class dMatrix;

class dVectors : public cList<dVector>
{
public:
	dVectors & operator *= (double);
	dVectors & operator /= (double);
	dVectors & operator *= (const dVector &);
	dVectors & operator /= (const dVector &);
	dVectors & operator += (const dVector &);
	dVectors & operator -= (const dVector &);
	dVectors & operator ^= (const dVector &);
	dVectors & operator *= (const dMatrix &);
	int Dim() const;
	int Dim(int n);
	void Normalize();
	// Special operations:
	dVector Centre() const;
	void CalcBoundBox(dVector &bbMin, dVector &bbMax) const;
	void CalcBoundSphere(dVector &Centre, double &Radius) const;
	double CalcMinDist() const;
	void ScaleTo(double bbMaxDim);
	void RemoveDups(DWORDS *pRemap = NULL);
};

// dVectors::operator *= (double)
inline dVectors & dVectors::operator *= (double c) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] *= c;
	return *this;
}

// dVectors::operator /= (double)
inline dVectors & dVectors::operator /= (double c) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] /= c;
	return *this;
}

// dVectors::operator *= (const dVector &)
inline dVectors & dVectors::operator *= (const dVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] *= u;
	return *this;
}

// dVectors::operator /= (const dVector &)
inline dVectors & dVectors::operator /= (const dVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] /= u;
	return *this;
}

// dVectors::operator += (const dVector &)
inline dVectors & dVectors::operator += (const dVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] += u;
	return *this;
}

// dVectors::operator -= (const dVector &)
inline dVectors & dVectors::operator -= (const dVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] -= u;
	return *this;
}

// dVectors::operator ^= (const dVector &)
inline dVectors & dVectors::operator ^= (const dVector &u) {
	for(int i = 0; i < m_nUsed; i++) m_pList[i] ^= u;
	return *this;
}

// dVectors::operator *= (const dMatrix &)
inline dVectors & dVectors::operator *= (const dMatrix &M) {
	for(int i = 0; i < m_nUsed; i++)
		m_pList[i] *= M;
	return *this;
}

// dVectors::Dim
inline int dVectors::Dim() const {
	cAssert(m_nUsed > 0);
	if(m_nUsed <= 0) return -1;
	return m_pList[0].Dim();
} // dVectors::Dim

// dVectors::Dim(int)
inline int dVectors::Dim(int n) {
	cAssert(m_nUsed > 0);
	if(m_nUsed <= 0) return -1;
	for(int i = 0; i < m_nUsed; i++) m_pList[i].Dim(n);
	return m_pList[0].Dim();
}

// dVectors::Normalize
inline void dVectors::Normalize() {
	cAssert(m_nUsed > 0);
	int i;
	for(i = 1; i < m_nUsed; i++) m_pList[i].Normalize();
	m_pList[i].Normalize();
}