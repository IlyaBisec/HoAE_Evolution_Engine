#include "..\stdheader.h"
#include "Surface.h"

//-----------------------------------------------------------------------------
// .ctor : void()
//-----------------------------------------------------------------------------
SurfaceShader::SurfaceShader() {
	m_toLights.Add(cVector(10, 3, -63)); // 0
	m_toLights.Add(cVector(14, 5, -20)); // 1
	m_toLights.Add(cVector(2, 19, -22)); // 2
	m_toLights.Add(cVector(8, 16, -64)); // 3
	m_toLights.Add(cVector(15, 2, -50)); // 4
	m_toLights.Add(cVector(3, -12, -16)); // 5
	m_toLights.Add(cVector(9, 0, -60)); // 6
	m_toLights.Add(cVector(21, 18, -57)); // 7
	m_toLights.Add(cVector(0, 19, -56)); // 8
	m_toLights.Add(cVector(23, -8, -31)); // 9
	m_toLights.Add(cVector(135, 51, 59)); // 10
	m_toLights.Add(cVector(110, 38, 64)); // 11
	m_toLights.Add(cVector(127, 61, 64)); // 12
	m_toLights.Add(cVector(138, 77, 64)); // 13
	m_toLights.Add(cVector(109, 48, 68)); // 14
	m_toLights.Add(cVector(141, 61, 62)); // 15
	m_toLights.Add(cVector(132, 69, 59)); // 16
	m_toLights.Add(cVector(111, 85, 65)); // 17
	m_toLights.Add(cVector(102, 66, 59)); // 18
	m_toLights.Add(cVector(134, 45, 59)); // 19

//	cVector L(2, 1, 1);
//	L.Normalize();
//	m_toLights.Add(L);

	m_ShadowMinLevel = 40;
	m_ShadowMaxLevel = 160;

	m_k = 0.4f;

	m_kShadow = 0;
	for(int l = 0; l < m_toLights.Count(); l++) {
		m_toLights[l].Normalize();
		m_kShadow += Clamp(m_k + (1.f - m_k) * float(m_toLights[l].z), m_k, 1.f);
	}
} // SurfaceShader

//-----------------------------------------------------------------------------
// SurfaceShader::ShadeOneVertex : void(OneMapVertex *) const
//-----------------------------------------------------------------------------
void SurfaceShader::ShadeOneVertex(OneMapVertex *pVertex) const {
	cVector Normal(pVertex->nx, pVertex->ny, pVertex->nz);
	if(cUtils::IsZero(Normal.Normalize())) return;
	float Shadow = 0;
	for(int l = 0; l < m_toLights.Count(); l++) {
		Shadow += Clamp(m_k + (1.f - m_k) * float(Normal.Dot(m_toLights[l])), m_k, 1.f);
	}
	Shadow = 128.f * Shadow / m_kShadow;
	pVertex->Shadow = Clamp(int(Shadow), m_ShadowMinLevel, m_ShadowMaxLevel);
}

//-------------------------------------------------------------------------------------
// SurfaceShader::Shade : void(VertexBasedMesh *, eSSIllumination) const
//-------------------------------------------------------------------------------------
void SurfaceShader::Shade(VertexBasedMesh *pMesh, eSSIllumination Illumination) const {
	if(!pMesh->Vert.NValues) return;
	switch(Illumination) {
		case SSLocalIllumination:
			for(int nVert = 0; nVert < pMesh->Vert.NValues; nVert++) {
				ShadeOneVertex(&pMesh->Vert[nVert]);
			}
			break;
		case SSDirectIllumination:
			if(pMesh->Idxs.NValues) DirectIllumination(pMesh);
			break;
		default:
			break;
	}
}

cDIB dib(SSCache::cZBufferSide, SSCache::cZBufferSide); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// SSCache::Init
void SSCache::Init(const VertexBasedMesh &Mesh) {
	Free();

	assert(nVerts = Mesh.Vert.NValues);
	assert(nTris = Mesh.Idxs.NValues / 3);
	// pShadows:
	pShadows = new float[nVerts];
	assert(pShadows);
	ZeroMemory(pShadows, nVerts * sizeof(float));
	// pZBuffer:
	pZBuffer = new int[cZBufferLen];
	assert(pZBuffer);
	// pTVerts:
	pTVerts = new cVector[nVerts];
	assert(pTVerts);
	// pNVerts:
	pNVerts = new cVector[nVerts];
	assert(pNVerts);
	for(int i = 0; i < nVerts; i++) {
		const OneMapVertex &v = Mesh.Vert[i];
		pNVerts[i].Set(v.nx, v.ny, v.nz);
		if(cUtils::NotZero(pNVerts[i].Length())) pNVerts[i].Normalize();
	}
} // SSCache::Init

//-----------------------------------------------------------------------------
// SurfaceShader::DirectIllumination : void(VertexBasedMesh *) const
//-----------------------------------------------------------------------------
void SurfaceShader::DirectIllumination(VertexBasedMesh *pMesh) const {
	SSCache C;
	C.Init(*pMesh);

	// Performing pass for each light source:
	for(int l = 0; l < m_toLights.Count(); l++) {
		OnePassOfDirectIllumination(*pMesh, C, m_toLights[l]);
//		dib.toFile("c:\\1.bmp"); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}

	// Shadowing each vert:
	for(int nVert = 0; nVert < C.nVerts; nVert++) {
		OneMapVertex &v = pMesh->Vert[nVert];
		float s = 128.f * C.pShadows[nVert] / m_kShadow;
		v.Shadow = Clamp(int(s), m_ShadowMinLevel, m_ShadowMaxLevel);
	}
}

//-------------------------------------------------------------------------------------------------------------
// SurfaceShader::OnePassOfDirectIllumination : void(const VertexBasedMesh &, SSCache &, const cVector &) const
//-------------------------------------------------------------------------------------------------------------
void SurfaceShader::OnePassOfDirectIllumination(const VertexBasedMesh &Mesh, SSCache &C, const cVector &toLight) const {
	// Calculating coordinate system of light:
	cVector XAxis, YAxis, ZAxis = toLight;
	XAxis.Cross(ZAxis, cVector::zAxis);
	YAxis.Cross(ZAxis, XAxis);
	XAxis.Normalize();
	YAxis.Normalize();
	//cMatrix T(XAxis.x, YAxis.x, ZAxis.x, XAxis.y, YAxis.y, ZAxis.y, XAxis.z, YAxis.z, ZAxis.z);
	cMatrix T(XAxis.x, XAxis.y, XAxis.z,
		YAxis.x, YAxis.y, YAxis.z,
		ZAxis.x, ZAxis.y, ZAxis.z);

	// Transforming verts:
	for(int nVert = 0; nVert < C.nVerts; nVert++) {
		const OneMapVertex &v = Mesh.Vert[nVert];
		cVector &t = C.pTVerts[nVert];
		t.Set(v.x, v.y, v.z);
		t *= T;
	}
	// Calcing normals in tris:
/*	int ii0, i0, i1, i2;
	for(int nTri = 0; nTri < C.nTris; nTri++) {
		ii0 = 3 * nTri;
		i0 = Mesh.Idxs[ii0], i1 = Mesh.Idxs[ii0 + 1], i2 = Mesh.Idxs[ii0 + 2];
		cVector &N = C.pNTris[nTri];
		N = (C.pTVerts[i1] - C.pTVerts[i0]) ^ (C.pTVerts[i2] - C.pTVerts[i0]);
		if(cUtils::NotZero(N.Length())) {
			N.Normalize();
		} else N = cVector::Null;
	}*/
	// Calcing extents:
	const cVector &t = C.pTVerts[0];
	double xMin = t.x, xMax = t.x, yMin = t.y, yMax = t.y, zMin = t.z, zMax = t.z;
	for(nVert = 1; nVert < C.nVerts; nVert++) {
		const cVector &t = C.pTVerts[nVert];
		xMin = Min(xMin, t.x);
		xMax = Max(xMax, t.x);
		yMin = Min(yMin, t.y);
		yMax = Max(yMax, t.y);
		zMin = Min(zMin, t.z);
		zMax = Max(zMax, t.z);
	}
	// Scaling transformed verts to fit ZBuffer (cZBufferSide x cZBufferSide):
	double dx = xMax - xMin, dy = yMax - yMin, dz = zMax - zMin;
	double xScale = double(SSCache::cZBufferSide) / dx,
		yScale = double(SSCache::cZBufferSide) / dy,
		zScale = double(0xFFFFFF) / dz;
	for(nVert = 0; nVert < C.nVerts; nVert++) {
		cVector &t = C.pTVerts[nVert];
		t.x = xScale * (t.x - xMin - dx / 2) + SSCache::cZBufferSide / 2;
		t.y = yScale * (t.y - yMin - dy / 2) + SSCache::cZBufferSide / 2;
		t.z = zScale * (t.z - zMin - dz / 2);
	}

	// Init pZBuffer && pZMask:
	for(int i = 0; i < SSCache::cZBufferLen; i++) C.pZBuffer[i] = comms::IntMin;
//	for(i = 0; i < C.nVerts; i++) {
//		const cVector &t = C.pTVerts[i];
//		int x = (int)t.x, y = (int)t.y, z = (int)t.z;
//		if(x < 0 || x >= SSCache::cZBufferSide || y < 0 || y >= SSCache::cZBufferSide) continue;
//		int n = y * SSCache::cZBufferSide + x;
//		C.pZBuffer[n] = z;
//	}
//	ZeroMemory(C.pZMask, SSCache::cZBufferLen * sizeof(bool));
//	C.MaskOfZBuffer.ClearAll();
//	for(i = 0; i < C.nVerts; i++) {
//		const cVector &t = C.pTVerts[i];
//		int x = (int)t.x, y = (int)t.y, z = (int)t.z;
//		if(x < 0 || x >= SSCache::cZBufferSide || y < 0 || y >= SSCache::cZBufferSide) continue;
//		int n = y * SSCache::cZBufferSide + x;
//		C.MaskOfZBuffer.Set(n);
//		C.pZMask[n] = true;
//	}
//	for(i = 0; i < SSCache::cZBufferLen; i++) C.pZMask[i] = true; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	C.MaskOfZBuffer.SetAll(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// Rasterizing tris to pZBuffer:
	int ii0, i0, i1, i2;
	for(int nTri = 0; nTri < C.nTris; nTri++) {
		ii0 = 3 * nTri;
		i0 = Mesh.Idxs[ii0], i1 = Mesh.Idxs[ii0 + 1], i2 = Mesh.Idxs[ii0 + 2];
		Tri2ZBuffer(C, i0, i1, i2);
//		const cVector &t0 = C.pTVerts[i0], &t1 = C.pTVerts[i1], &t2 = C.pTVerts[i2];
//		cVector &N = C.pNTris[nTri];
//		if(cUtils::NotZero(N.z)) {
//			oldTri2ZBuffer(C, t0, t1, t2);
//			if(N.z < 0.) Tri2ZBuffer(C, nTri, t0, t1, t2);
//			else {
//				N.Reverse();
//				Tri2ZBuffer(C, nTri, t2, t1, t0);
//			}
//		}
//		C.NCurTri = (t1 - t0) ^ (t2 - t0);
//		if(cUtils::IsZero(C.NCurTri.Length())) continue;
//		C.NCurTri.Normalize();
//		if(cUtils::IsZero(C.NCurTri.z)) continue;
//		cVector N = (t1 - t0) ^ (t2 - t0);
//		if(N.z < 0.) Tri2ZBuffer(C, t0, t1, t2);
//		else Tri2ZBuffer(C, t2, t1, t0);
//		Tri2ZBuffer(C, C.pTVerts[i2], C.pTVerts[i1], C.pTVerts[i0]);
//		oldTri2ZBuffer(C, C.pTVerts[i0], C.pTVerts[i1], C.pTVerts[i2]);
//		const cVector &t0 = C.pTVerts[i0], &t1 = C.pTVerts[i1], &t2 = C.pTVerts[i2];
//		cVector N = (t1 - t0) ^ (t2 - t0);
//		if(N.z < 0.) Tri2ZBuffer(C, t0, t1, t2);
//		else Tri2ZBuffer(C, t2, t1, t0);
	//	oldTri2ZBuffer(C, C.pTVerts[i2], C.pTVerts[i1], C.pTVerts[i0]);
	}

	// Calcing shadow for each vert:
	for(nVert = 0; nVert < C.nVerts; nVert++) {
		const cVector &t = C.pTVerts[nVert];
		int x = (int)t.x, y = (int)t.y, z = (int)t.z;
		if(x < 0 || x >= SSCache::cZBufferSide || y < 0 || y >= SSCache::cZBufferSide) continue;
		bool f = (z < C.pZBuffer[y * SSCache::cZBufferSide + x]);
		if(f) continue; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		C.pShadows[nVert] += Clamp(m_k + (1.f - m_k) * float(C.pNVerts[nVert].Dot(ZAxis)), m_k, 1.f);
	}
} // SurfaceShader::OnePassOfDirectIllumination

// SurfaceShader::Tri2ZBuffer
/*void SurfaceShader::Tri2ZBuffer(SSCache &C, int nTri, int i0, int i1, int i2) const {
	cVector &TriNormal = C.pNTris[nTri];
	if(cUtils::IsZero(TriNormal.z)) return;

	// Anticlockwise assurance:
	if(TriNormal.z > 0) {
		Swap(i0, i2);
		TriNormal.Reverse();
	}

	const cVector &t0 = C.pTVerts[i0], &t1 = C.pTVerts[i1], &t2 = C.pTVerts[i2];

	// 28.4
	const int x1 = int(16. * t0.x), x2 = int(16. * t1.x), x3 = int(16. * t2.x);
	const int y1 = int(16. * t0.y), y2 = int(16. * t1.y), y3 = int(16. * t2.y);
	const int z1 = int(16. * t0.z), z2 = int(16. * t1.z), z3 = int(16. * t2.z);
	
	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;
	// int fixed - point:
	const int fDx12 = Dx12 << 4, fDx23 = Dx23 << 4, fDx31 = Dx31 << 4;
	const int fDy12 = Dy12 << 4, fDy23 = Dy23 << 4, fDy31 = Dy31 << 4;

	// Bounding rect:
	int xMin = (comms::Min(x1, x2, x3) + 0xF) >> 4;
	int xMax = (comms::Max(x1, x2, x3) + 0xF) >> 4;
	int yMin = (comms::Min(y1, y2, y3) + 0xF) >> 4;
	int yMax = (comms::Max(y1, y2, y3) + 0xF) >> 4;

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
			if(x >= 0 && x < SSCache::cZBufferSide && y >= 0 && y < SSCache::cZBufferSide) {
				if(Cx1 > 0 && Cx2 > 0 && Cx3 > 0) {
					int n = y * SSCache::cZBufferSide + x;
					//if(n < SSCache::cZBufferLen && C.MaskOfZBuffer[n]) { // here we calc z
					if(n < SSCache::cZBufferLen && C.pZBuffer[n] != comms::IntMin) { // here we calc z
//						dib.SetPixel(x, y, comms::cWhite); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
					}
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
}*/ // SurfaceShader::Tri2ZBuffer

// SurfaceShader::Tri2ZBuffer
void SurfaceShader::Tri2ZBuffer(SSCache &C, int i0, int i1, int i2) const {
	const cVector &t0 = C.pTVerts[i0], &t1 = C.pTVerts[i1], &t2 = C.pTVerts[i2];

	int x0 = (int)t0.x, y0 = (int)t0.y, z0 = (int)t0.z;
	int x1 = (int)t1.x, y1 = (int)t1.y, z1 = (int)t1.z;
	int x2 = (int)t2.x, y2 = (int)t2.y, z2 = (int)t2.z;

	// Sorting ascending along y axis:
	if(y0 > y1) { Swap(x0, x1); Swap(y0, y1); Swap(z0, z1); }
	if(y0 > y2) { Swap(x0, x2); Swap(y0, y2); Swap(z0, z2); }
	if(y1 > y2) { Swap(x1, x2); Swap(y1, y2); Swap(z1, z2); }

	if(y2 - y1 == 0) return;

	for(int y = y0; y <= y2; y++) {
		// Crossing with longest section ((x0, y0) --- (x2, y2)):
		int _x1 = x0 + (y - y0) * (x2 - x0) / (y2 - y0);
		int _z1 = z0 + (y - y0) * (z2 - z0) / (y2 - y0);

		// Crossing with short section ((x0, y0) --- (x1, y1) or (x1, y1) --- (x2, y2)):
		int _x2, _z2;
		if(y < y1) {
			_x2 = x0 + (y - y0) * (x1 - x0) / (y1 - y0);
			_z2 = z0 + (y - y0) * (z1 - z0) / (y1 - y0);
		} else {
			if(y2 == y1) {
				_x2 = x1;
				_z2 = z1;
			}
			else {
				_x2 = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
				_z2 = z1 + (y - y1) * (z2 - z1) / (y2 - y1);
			}
		}

		if(_x1 > _x2) {
			Swap(_x1, _x2);
			Swap(_z1, _z2);
		}

		// Rasterizing horizontal line (_x1 - _x2):
		for(int x = _x1; x < _x2; x++) {
			int z = _z1 + (x - _x1) * (_z2 - _z1) / (_x2 - _x1);
			if(x >= 0 && x < SSCache::cZBufferSide && y >= 0 && y < SSCache::cZBufferSide) {
				if(C.pZBuffer[y * SSCache::cZBufferSide + x] < z) {
					C.pZBuffer[y * SSCache::cZBufferSide + x] = z;
//					dib.SetPixel(x, y, comms::cWhite); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				}
			}
		}
	}
} // SurfaceShader::oldTri2ZBuffer

// SurfaceShader::intTri2ZBuffer
/*void SurfaceShader::intTri2ZBuffer(SSCache &C, int nTri, const cVector &t0, const cVector &t1, const cVector &t2) const {
	const int x1 = t0.x, x2 = t1.x, x3 = t2.x;
	const int y1 = t0.y, y2 = t1.y, y3 = t2.y;

	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;

	// Bounding rect:
	int xMin = comms::Min(x1, x2, x3);
	int xMax = comms::Max(x1, x2, x3);
	int yMin = comms::Min(y1, y2, y3);
	int yMax = comms::Max(y1, y2, y3);

	// Constant parts of half - space functions:
	int C1 = Dy12 * x1 - Dx12 * y1;
	int C2 = Dy23 * x2 - Dx23 * y2;
	int C3 = Dy31 * x3 - Dx31 * y3;

	// Fill convention:
	if(Dy12 < 0 || (Dy12 == 0 && Dx12 > 0)) C1++;
	if(Dy23 < 0 || (Dy23 == 0 && Dx23 > 0)) C2++;
	if(Dy31 < 0 || (Dy31 == 0 && Dx31 > 0)) C3++;

	// Starting values of the half - space functions at the top of the bounding rectangle:
	int Cy1 = C1 + Dx12 * yMin - Dy12 * xMin;
	int Cy2 = C2 + Dx23 * yMin - Dy23 * xMin;
	int Cy3 = C3 + Dx31 * yMin - Dy31 * xMin;

	// Scanning:
	for(int y = yMin; y < yMax; y++) {
		// Starting values for horizontal scan:
		int Cx1 = Cy1, Cx2 = Cy2, Cx3 = Cy3;
		for(int x = xMin; x < xMax; x++) {
			if(x >= 0 && x < SSCache::cZBufferSide && y >= 0 && y < SSCache::cZBufferSide) {
				if(Cx1 > 0 && Cx2 > 0 && Cx3 > 0) {
					int n = y * SSCache::cZBufferSide + x;
					if(n < SSCache::cZBufferLen && C.MaskOfZBuffer[n]/*C.pZMask[n]*///) { // here we calc z
//						dib.SetPixel(x, y, comms::cWhite); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//						const cVector &N = C.pNTris[nTri];
//						double hx = x - x1, hy = y - y1;
//						int z;
//						if(x == x1 && y == y1) z = t0.z;
//						else if(x == x2 && y == y2) z = t1.z;
//						else if(x == x3 && y == y3) z = t2.z;
//						else z = - (N.x * hx + N.y * hy) / N.z + int(t0.z);
//						if(C.pZBuffer[n] < z) C.pZBuffer[n] = z;
/*					}
				}
			}
			Cx1 -= Dy12;
			Cx2 -= Dy23;
			Cx3 -= Dy31;
		}
		Cy1 += Dx12;
		Cy2 += Dx23;
		Cy3 += Dx31;
	}
} */// SurfaceShader::intTri2ZBuffer