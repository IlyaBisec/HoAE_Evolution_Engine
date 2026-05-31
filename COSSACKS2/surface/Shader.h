#ifndef __Shader__
#define __Shader__

namespace Surface {

typedef enum { I_NONE = 0, I_LOCAL = 1, I_DIRECT = 2 } eIllumination;

class SurfMesh;
class SurfRef;

class SCache {
public:
	SCache() { Null(); }
	~SCache() { Free(); }

	void Init(const SurfMesh &, int ZBufferSide, float AmbientIllumination);

	int nVerts, nTris;
	float *pShadows;
	int *pZBuffer;
	cVec3 *pTVerts, *pNVerts;
	float *pDots;
	int cZBufferSide, cZBufferLen;
private:
	void Null() {
		pShadows = NULL;
		pZBuffer = NULL;
		pTVerts = NULL;
		pNVerts = NULL;
		pDots = NULL;
	}
	void Free() {
		if(pShadows) delete[] pShadows;
		if(pZBuffer) delete[] pZBuffer;
		if(pTVerts) delete[] pTVerts;
		if(pNVerts) delete[] pNVerts;
		if(pDots) delete[] pDots;
		Null();
	}
};

class SurfVertex;
class SurfShader {
public:
	SurfShader() {
		m_CurPass = -1;
	}
	void Shade(SurfMesh &Mesh, eIllumination Illumination) const;
	void Shade(SurfRef &Ref) const; // I_LOCAL, by idVerts
	void Init(float Phi, float Psi, float AmbientIllumination, float Photon,
		int nLights, float AngleVariation, int Seed, int ZBufferSide);
	
	void BeginPassedDirectIllumination(SurfMesh &Mesh);
	bool OnePass();
	cVec3 toSun;
private:
	SCache m_Cache;
	int m_CurPass;
	SurfMesh *m_pMesh;

	void ShadeOneVertex(SurfVertex &v) const;
	void DirectIllumination(SurfMesh &Mesh) const;
	void OnePassOfDirectIllumination(const SurfMesh &Mesh, SCache &C, const cVec3 &toLight) const;
	static void Tri2ZBuffer(SCache &C, int i0, int i1, int i2);

	cList<cVec3> m_toLights;
	float m_AmbientIllumination, m_Photon;
	int m_ZBufferSide;
};

} // Surface

#endif // __Shader__