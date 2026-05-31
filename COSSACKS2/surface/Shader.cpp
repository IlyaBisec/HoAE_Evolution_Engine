#include "stdheader.h"
#include "Surface.h"

namespace Surface {

//-----------------------------------------------------------------------------------
// SurfShader::Init : void(float, float, float, float, int, float, int, int)
//-----------------------------------------------------------------------------------
void SurfShader::Init(float Phi, float Psi, float AmbientIllumination, float Photon,
					  int nLights, float AngleVariation, int Seed, int ZBufferSide) {
	m_ZBufferSide = ZBufferSide;
	m_toLights.Free();
	srand(Seed);
	dVector to;
	for(int l = 0; l < nLights; l++) {
		float dPhi = AngleVariation * (1.0f - float(rand() % 100) / 50.0f);
		float dPsi = AngleVariation * (1.0f - float(rand() % 100) / 50.0f);
		to = dVector::Cartesian(1, Phi + dPhi, Psi + dPsi);
		m_toLights.Add(cVec3((float)to.x, (float)to.z, (float)to.y));
	}
	m_AmbientIllumination = AmbientIllumination;
	m_Photon = Photon;
	// toSun:
	to = dVector::Cartesian(1, Phi, Psi);
	toSun = cVec3((float)to.x, (float)to.z, (float)to.y);
}

//-----------------------------------------------------------------------------
// SurfShader::ShadeOneVertex : void(SurfVertex &) const
//-----------------------------------------------------------------------------
void SurfShader::ShadeOneVertex(SurfVertex &v) const {
	cVec3 Normal = cVec3::Normalize(cVec3(v.nx, v.ny, v.nz));
	float Luminance = m_AmbientIllumination;
	for(int l = 0; l < m_toLights.Count(); l++) {
		Luminance += m_Photon * cMath::Max(0.0f, cVec3::Dot(m_toLights[l], Normal));
	}
	v.Luminance = (unsigned char)cColor::Pack(Luminance / 2.0f);
}

//-----------------------------------------------------------------------------
// SurfShader::Shade : void(SurfRef &) const
//-----------------------------------------------------------------------------
void SurfShader::Shade(SurfRef &Ref) const {
	assert(Ref.pOrig);
	if(!Ref.idVerts.Count()) return;
	for(int iVert = 0; iVert < Ref.idVerts.Count(); iVert++) {
		int nVert = Ref.idVerts[iVert];
		SurfVertex &r = Ref.pOrig->VERTS[nVert];
		ShadeOneVertex(r);
	}
}

//-----------------------------------------------------------------------------
// SurfShader::Shade : void(SurfMesh &, eIllumination) const
//-----------------------------------------------------------------------------
void SurfShader::Shade(SurfMesh &Mesh, eIllumination Illumination) const {
	if(!Mesh.NVerts()) return;
	switch(Illumination) {
		case I_LOCAL:
			for(int nVert = 0; nVert < Mesh.VERTS.Count(); nVert++) {
				ShadeOneVertex(Mesh.VERTS[nVert]);
			}
			break;
		case I_DIRECT:
			if(Mesh.NTris()) DirectIllumination(Mesh);
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------
// SurfShader::BeginPassedDirectIllumination : void(SurfMesh &, eIllumination)
//-------------------------------------------------------------------------------
void SurfShader::BeginPassedDirectIllumination(SurfMesh &Mesh) {
	assert(-1 == m_CurPass);
	if(!Mesh.NTris()) return;
	
	m_Cache.Init(Mesh, m_ZBufferSide, m_AmbientIllumination);
	m_CurPass = 0;
	m_pMesh = &Mesh;
}

//-----------------------------------------------------------------------------
// SurfShader::OnePass : bool()
//-----------------------------------------------------------------------------
bool SurfShader::OnePass() {
	if(-1 == m_CurPass) return false;

	if(m_CurPass < m_toLights.Count()) {
		OnePassOfDirectIllumination(*m_pMesh, m_Cache, m_toLights[m_CurPass]);
		m_CurPass++;
	}
	if(m_toLights.Count() == m_CurPass) {
		for(int nVert = 0; nVert < m_Cache.nVerts; nVert++) {
			SurfVertex &v = m_pMesh->VERTS[nVert];
			v.Luminance = (unsigned char)cColor::Pack(m_Cache.pShadows[nVert] / 2.0f);
		}
		m_CurPass = -1;
		m_pMesh = NULL;
		return false;
	} else return true;
}

//-----------------------------------------------------------------------------------
// SCache::Init : void(const SurfMesh &, int, float)
//-----------------------------------------------------------------------------------
void SCache::Init(const SurfMesh &Mesh, int ZBufferSide, float AmbientIllumination) {
	Free();

	cZBufferSide = ZBufferSide;
	cZBufferLen = cMath::Square(cZBufferSide);

	nVerts = Mesh.NVerts();
	nTris = Mesh.NTris();
	if(!nVerts || !nTris) return;
	
	// pShadows:
	pShadows = new float[nVerts];
	assert(pShadows);
	for(int i = 0; i < nVerts; i++) pShadows[i] = AmbientIllumination;
	// pZBuffer:
	pZBuffer = new int[cZBufferLen];
	assert(pZBuffer);
	// pTVerts:
	pTVerts = new cVec3[nVerts];
	assert(pTVerts);
	// pNVerts:
	pNVerts = new cVec3[nVerts];
	assert(pNVerts);
	for(int i = 0; i < nVerts; i++) {
		const SurfVertex &v = Mesh.VERTS[i];
		pNVerts[i] = cVec3::Normalize(cVec3(v.nx, v.ny, v.nz));
	}
	// pDots:
	pDots = new float[nVerts];
	assert(pDots);
}

//cDIB dib; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//-----------------------------------------------------------------------------
// SurfShader::DirectIllumination : void(SurfMesh &) const
//-----------------------------------------------------------------------------
void SurfShader::DirectIllumination(SurfMesh &Mesh) const {
	SCache C;
	C.Init(Mesh, m_ZBufferSide, m_AmbientIllumination);

	// Performing pass for each light source:
	for(int l = 0; l < m_toLights.Count(); l++) {
		OnePassOfDirectIllumination(Mesh, C, m_toLights[l]);
	}

	// Shadowing each vert:
	for(int nVert = 0; nVert < C.nVerts; nVert++) {
		SurfVertex &v = Mesh.VERTS[nVert];
		v.Luminance = (unsigned char)cColor::Pack(C.pShadows[nVert] / 2.0f);
	}
}

//------------------------------------------------------------------------------------------------------------
// SurfShader::OnePassOfDirectIllumination : void(const SurfMesh &, SCache &, const cVec3 &) const
//------------------------------------------------------------------------------------------------------------
void SurfShader::OnePassOfDirectIllumination(const SurfMesh &Mesh, SCache &C, const cVec3 &toLight) const {
	// Coord system of light:
	cVec3 zAxis = toLight;
	cVec3 xAxis = cVec3::Normalize(cVec3::Cross(zAxis, cVec3::AxisZ));
	cVec3 yAxis = cVec3::Normalize(cVec3::Cross(zAxis, xAxis));

	// Transforming verts:
	for(int nVert = 0; nVert < C.nVerts; nVert++) {
		const SurfVertex &v = Mesh.VERTS[nVert];
		cVec3 u(v.x, v.y, v.z);
		C.pTVerts[nVert] = cVec3(cVec3::Dot(xAxis, u), cVec3::Dot(yAxis, u), cVec3::Dot(zAxis, u));
	}

	// Calcing extents:
	cVec3 bbMin = C.pTVerts[0], bbMax = C.pTVerts[0];
	for(int nVert = 1; nVert < C.nVerts; nVert++) {
		const cVec3 &t = C.pTVerts[nVert];
		bbMin = cVec3::Min(bbMin, t);
		bbMax = cVec3::Max(bbMax, t);
	}

	// Init pZBuffer:
	for(int i = 0; i < C.cZBufferLen; i++) C.pZBuffer[i] = cMath::IntMinValue;

	// Scaling transformed verts to fit ZBuffer (cZBufferSide x cZBufferSide):
	bbMin -= cVec3(1, 1, 1);
	bbMax += cVec3(1, 1, 1);
	cVec3 d = bbMax - bbMin;
	cVec3 Scale = cVec3(C.cZBufferSide, C.cZBufferSide, 0xffffff) / d;
	for(int nVert = 0; nVert < C.nVerts; nVert++) {
		cVec3 &t = C.pTVerts[nVert];
		t.x = Scale.x * (t.x - bbMin.x - d.x / 2) + C.cZBufferSide / 2;
		t.y = Scale.y * (t.y - bbMin.y - d.y / 2) + C.cZBufferSide / 2;
		t.z = Scale.z * (t.z - bbMin.z);
		// Calcing dots:
		float d = cVec3::Dot(zAxis, C.pNVerts[nVert]);
		if(d >= 0) {
//			int n = (int)t.y * C.cZBufferSide + (int)t.x;
//			C.pZBuffer[n] = - 1;
		} else d = 0;
		C.pDots[nVert] = d;
	}


	// Rasterizing tris to pZBuffer:
	int ii0, i0, i1, i2;
	for(int nTri = 0; nTri < C.nTris; nTri++) {
		ii0 = 3 * nTri;
		i0 = Mesh.TRIS[ii0], i1 = Mesh.TRIS[ii0 + 1], i2 = Mesh.TRIS[ii0 + 2];
		Tri2ZBuffer(C, i0, i1, i2);
	}

	// Calcing shadow for each vert:
	for(int nVert = 0; nVert < C.nVerts; nVert++) {
		const cVec3 &t = C.pTVerts[nVert];
		int x = (int)t.x, y = (int)t.y, z = (int)t.z;
//		assert(comms::RangeCheck(x, 0, C.cZBufferSide - 1));
//		assert(comms::RangeCheck(y, 0, C.cZBufferSide - 1));
		float d = C.pDots[nVert];
		if(0 == d) continue;
		if(z < C.pZBuffer[y * C.cZBufferSide + x]) continue;
		C.pShadows[nVert] += m_Photon * d;
	}
}

//-----------------------------------------------------------------------------
// SurfShader::Tri2ZBuffer : void(SCache &, int, int, int)
//-----------------------------------------------------------------------------
void SurfShader::Tri2ZBuffer(SCache &C, int i0, int i1, int i2) {
	const cVec3 *t[3] = { &C.pTVerts[i0], &C.pTVerts[i1], &C.pTVerts[i2] };
	
	const float Epsilon = 0.1f;
	cVec3 N = cVec3::Cross(*t[1] - *t[0], *t[2] - *t[0]);
	if(N.z > Epsilon) {
		return;
	}

	// Forcing verts to ZBuffer:
	for(int k = 0; k < 3; k++) {
		const int x = t[k]->x;
		const int y = t[k]->y;
		const int z = t[k]->z;
		if(x >= 0 && x < C.cZBufferSide && y >= 0 && y < C.cZBufferSide) {
			const int index = y * C.cZBufferSide + x;
//			if(C.pZBuffer[index] > cMath::IntMinValue) {
				C.pZBuffer[index] = cMath::Max(C.pZBuffer[index], z);
//			}
		}
	}

	// 28.4
	const int x1 = int(16. * t[0]->x), x2 = int(16. * t[1]->x), x3 = int(16. * t[2]->x);
	const int y1 = int(16. * t[0]->y), y2 = int(16. * t[1]->y), y3 = int(16. * t[2]->y);
	const int z1 = int(16. * t[0]->z), z2 = int(16. * t[1]->z), z3 = int(16. * t[2]->z);
	
	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;
	// int fixed - point:
	const int fDx12 = Dx12 << 4, fDx23 = Dx23 << 4, fDx31 = Dx31 << 4;
	const int fDy12 = Dy12 << 4, fDy23 = Dy23 << 4, fDy31 = Dy31 << 4;

	// Bounding rect:
	int xMin = (cMath::Min(x1, x2, x3) + 0xF) >> 4;
	int xMax = (cMath::Max(x1, x2, x3) + 0xF) >> 4;
	int yMin = (cMath::Min(y1, y2, y3) + 0xF) >> 4;
	int yMax = (cMath::Max(y1, y2, y3) + 0xF) >> 4;

	// Constant parts of half - space functions:
	int C1 = Dy12 * x1 - Dx12 * y1;
	int C2 = Dy23 * x2 - Dx23 * y2;
	int C3 = Dy31 * x3 - Dx31 * y3;

	// Fill convention:
	if(Dy12 < 0 || (Dy12 == 0 && Dx12 > 0)) C1++;
	if(Dy23 < 0 || (Dy23 == 0 && Dx23 > 0)) C2++;
	if(Dy31 < 0 || (Dy31 == 0 && Dx31 > 0)) C3++;

	// Starting values of the half - space functions at the top of the bounding rectangle:
	int Cy1 = C1 + Dx12 * (yMin << 4) - Dy12 * (xMin << 4);
	int Cy2 = C2 + Dx23 * (yMin << 4) - Dy23 * (xMin << 4);
	int Cy3 = C3 + Dx31 * (yMin << 4) - Dy31 * (xMin << 4);

	// Scanning:
	for(int y = yMin; y < yMax; y++) {
		// Starting values for horizontal scan:
		int Cx1 = Cy1, Cx2 = Cy2, Cx3 = Cy3;
		for(int x = xMin; x < xMax; x++) {
			if(x >= 0 && x < C.cZBufferSide && y >= 0 && y < C.cZBufferSide) {
				if(Cx1 > 0 && Cx2 > 0 && Cx3 > 0) {
					int n = y * C.cZBufferSide + x;
					//assert(n < C.cZBufferLen);
//					if(C.pZBuffer[n] > cMath::IntMinValue) {
						N = cVec3::Normalize(N);
						float hx = x - t[0]->x, hy = y - t[0]->y;
						int z = - (N.x * hx + N.y * hy) / N.z + int(t[0]->z);
						if(C.pZBuffer[n] < z) {
							C.pZBuffer[y * C.cZBufferSide + x] = z;
						}
//					}
				}
			}
			Cx1 -= fDy12;
			Cx2 -= fDy23;
			Cx3 -= fDy31;
		}
		Cy1 += fDx12;
		Cy2 += fDx23;
		Cy3 += fDx31;
	}
}

} // Surface