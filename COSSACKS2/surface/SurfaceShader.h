#ifndef __SurfaceShader__
#define __SurfaceShader__

#include "comms\comms.h"

typedef enum { SSLocalIllumination, SSDirectIllumination } eSSIllumination;

class VertexBasedMesh;

class SSCache {
public:
	SSCache();
	~SSCache();
	void Init(const VertexBasedMesh &);

	int nVerts, nTris;
	float *pShadows;
	int *pZBuffer;
	cVector *pTVerts, *pNVerts;

	static const int cZBufferSide = 4096;
	static const int cZBufferLen = cZBufferSide * cZBufferSide;
private:
	void Null();
	void Free();
};

// SSCache::Null
inline void SSCache::Null() {
	pShadows = NULL;
	pZBuffer = NULL;
	pTVerts = NULL;
	pNVerts = NULL;
} // SSCache::Null

// SSCache
inline SSCache::SSCache() {
	Null();
} // SSCache

// SSCache::Free
inline void SSCache::Free() {
	if(pShadows) delete[] pShadows;
	if(pZBuffer) delete[] pZBuffer;
	if(pTVerts) delete[] pTVerts;
	if(pNVerts) delete[] pNVerts;
	Null();
} // SSCache::Free

// ~SSCache
inline SSCache::~SSCache() {
	Free();
} // ~SSCache

class OneMapVertex;
class SurfaceShader {
public:
	SurfaceShader();
	void Shade(VertexBasedMesh *pMesh, eSSIllumination Illumination) const;
	void ShadeOneVertex(OneMapVertex *pVertex) const;
	void Init();
private:
	void DirectIllumination(VertexBasedMesh *pMesh) const;
	void OnePassOfDirectIllumination(const VertexBasedMesh &Mesh, SSCache &C, const cVector &toLight) const;
	void Tri2ZBuffer(SSCache &C, int i0, int i1, int i2) const;
//	void intTri2ZBuffer(SSCache &C, int nTri, const cVector &t0, const cVector &t1, const cVector &t2) const;
//	void Tri2ZBuffer(SSCache &C, int nTri, int i0, int i1, int i2) const;

	cVectors m_toLights;
	int m_ShadowMinLevel, m_ShadowMaxLevel;
	float m_k, m_kShadow;
};

#endif // __SurfaceShader__