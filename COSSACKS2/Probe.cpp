#include "stdheader.h"
#include "surface.h"

Vector3D bezierTemplate[4][4];

void probeShift(Vertex2t *pv2tData, float dx, float dy, float dz, int nVertices)
{
	Vertex2t *pv2t = NULL;
	for(int i = 0; i < nVertices; i++)
	{
		pv2t = pv2tData + i;
		pv2t->x += dx;
		pv2t->y += dy;
		pv2t->z += dz;
	}
};
void probeRotate(Vertex2t *pv2tData, float ax, float ay, float az,
				 float Angle, int nVertices)
{
	Matrix4D m4d;
	Vector3D v3d, axis;
	Vertex2t *pv2t = NULL;
	axis.x = ax, axis.y = ay, axis.z = az;
	for(int i = 0; i < nVertices; i++)
	{
		pv2t = pv2tData + i;
		m4d.setIdentity();
		m4d.rotation(axis, Angle);
		v3d.x = pv2t->x, v3d.y = pv2t->y, v3d.z = pv2t->z;
		pv2t->x = v3d.x * m4d.e00 + v3d.y * m4d.e01;
		pv2t->y = v3d.x * m4d.e10 + v3d.y * m4d.e11 + v3d.z * m4d.e12;
		pv2t->z = v3d.x * m4d.e20 + v3d.y * m4d.e21 + v3d.z * m4d.e22;
	}
};
void beziersurfaceCalcVertex(float dS, float dT, float *pX, float *pY, float *pZ)
{
	int C[4] = {1, 3, 3, 1};
	float x = 0, y = 0, z = 0, k = 0;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			k = C[i] * pow(dS, i) * pow(1 - dS, 3 - i) *
				C[j] * pow(dT, j) * pow(1 - dT, 3 - j);
			x += k * bezierTemplate[i][j].x;
			y += k * bezierTemplate[i][j].y;		
			z += k * bezierTemplate[i][j].z;		
		}
	}
	*pX = x, *pY = y, *pZ = z;
};

void beziersurfaceGenerate(Vertex2t *pv2tData, word *pIndices,
							 float x00, float y00, float z00,
							 float x01, float y01, float z01,
							 float x02, float y02, float z02,
							 float x03, float y03, float z03,
							 float x10, float y10, float z10,
							 float x11, float y11, float z11,
							 float x12, float y12, float z12,
							 float x13, float y13, float z13,
							 float x20, float y20, float z20,
							 float x21, float y21, float z21,
							 float x22, float y22, float z22,
							 float x23, float y23, float z23,
							 float x30, float y30, float z30,
							 float x31, float y31, float z31,
							 float x32, float y32, float z32,
							 float x33, float y33, float z33,
							 int nPolygonsAlongCurve,
							 int *nVertices, int *nIndices)
{
	// 00 - 03
	bezierTemplate[0][0].x = x00, bezierTemplate[0][0].y = y00, bezierTemplate[0][0].z = z00;
	bezierTemplate[0][1].x = x01, bezierTemplate[0][1].y = y01, bezierTemplate[0][1].z = z01;
	bezierTemplate[0][2].x = x02, bezierTemplate[0][2].y = y02, bezierTemplate[0][2].z = z02;
	bezierTemplate[0][3].x = x03, bezierTemplate[0][3].y = y03, bezierTemplate[0][3].z = z03;
	// 10 - 13
	bezierTemplate[1][0].x = x10, bezierTemplate[1][0].y = y10, bezierTemplate[1][0].z = z10;
	bezierTemplate[1][1].x = x11, bezierTemplate[1][1].y = y11, bezierTemplate[1][1].z = z11;
	bezierTemplate[1][2].x = x12, bezierTemplate[1][2].y = y12, bezierTemplate[1][2].z = z12;
	bezierTemplate[1][3].x = x13, bezierTemplate[1][3].y = y13, bezierTemplate[1][3].z = z13;
	// 20 - 23
	bezierTemplate[2][0].x = x20, bezierTemplate[2][0].y = y20, bezierTemplate[2][0].z = z20;
	bezierTemplate[2][1].x = x21, bezierTemplate[2][1].y = y21, bezierTemplate[2][1].z = z21;
	bezierTemplate[2][2].x = x22, bezierTemplate[2][2].y = y22, bezierTemplate[2][2].z = z22;
	bezierTemplate[2][3].x = x23, bezierTemplate[2][3].y = y23, bezierTemplate[2][3].z = z23;
	// 30 - 33
	bezierTemplate[3][0].x = x30, bezierTemplate[3][0].y = y30, bezierTemplate[3][0].z = z30;
	bezierTemplate[3][1].x = x31, bezierTemplate[3][1].y = y31, bezierTemplate[3][1].z = z31;
	bezierTemplate[3][2].x = x32, bezierTemplate[3][2].y = y32, bezierTemplate[3][2].z = z32;
	bezierTemplate[3][3].x = x33, bezierTemplate[3][3].y = y33, bezierTemplate[3][3].z = z33;

	// wave
	static float C = 0, d = 3.14 / 6;
	C += 0.01f;
    float A[7];
	A[0] = sin(C), A[1] = sin(C + d), A[2] = sin(C + 2 * d), A[3] = sin(C + 3 * d),
		A[4] = sin(C + 4 * d), A[5] = sin(C + 5 * d), A[6] = sin(C + 6 * d);
	for(int i = 0; i < 7; i++) A[i] *= 30;
	bezierTemplate[0][3].z += A[0];	// 0
	// 1
	bezierTemplate[0][2].z += A[1], bezierTemplate[1][3].z += A[1];
	// 2
	bezierTemplate[0][1].z += A[2], bezierTemplate[1][2].z += A[2],
		bezierTemplate[2][3].z += A[2];
	// 3
	bezierTemplate[0][0].z += A[3], bezierTemplate[1][1].z += A[3],
		bezierTemplate[2][2].z += A[3], bezierTemplate[3][3].z += A[3];
	// 4
	bezierTemplate[1][0].z += A[4], bezierTemplate[2][1].z += A[4],
		bezierTemplate[3][2].z += A[4];
	// 5
	bezierTemplate[2][0].z += A[5], bezierTemplate[3][1].z += A[5];
	// 6
	bezierTemplate[3][0].z += A[6];

	// calculating vertices
	*nVertices = (nPolygonsAlongCurve + 1) * (nPolygonsAlongCurve + 1);
	float dS = 0, dT = 0, X = 0, Y = 0, Z = 0;
	Vertex2t *pv2t = NULL;

	for(int i = 0; i <= nPolygonsAlongCurve; i++)
	{
		dS = (float)i / (float)nPolygonsAlongCurve;
		for(int j = 0; j <= nPolygonsAlongCurve; j++)
		{
			dT = (float)j / (float)nPolygonsAlongCurve;

			beziersurfaceCalcVertex(dS, dT, &X, &Y, &Z);
			pv2t = pv2tData + i * (nPolygonsAlongCurve + 1) + j;
			pv2t->x = X, pv2t->y = Y, pv2t->z = Z;
			// uv
			pv2t->u = pv2t->u2 = (float)j / (float)nPolygonsAlongCurve;
			pv2t->v = pv2t->v2 = (float)i / (float)nPolygonsAlongCurve;
		}
	}
	// triangulation
	*nIndices = 6 * nPolygonsAlongCurve * nPolygonsAlongCurve;
	word *i0 = NULL, *i1 = NULL, *i2 = NULL;
	int N = 0;
	for(int i = 0; i < nPolygonsAlongCurve; i++)
		for(int j = 0; j < nPolygonsAlongCurve; j++)
		{
			i0 = pIndices + N + 0, i1 = pIndices + N + 1, i2 = pIndices + N + 2;
			*i0 = j + i * (nPolygonsAlongCurve + 1),
				*i1 = j + i * (nPolygonsAlongCurve + 1) + 1,
				*i2 = j + i * (nPolygonsAlongCurve + 1) + 1 + (nPolygonsAlongCurve + 1);
			i0 = pIndices + N + 3, i1 = pIndices + N + 4, i2 = pIndices + N + 5;
			*i0 = j + i * (nPolygonsAlongCurve + 1),
				*i1 = j + i * (nPolygonsAlongCurve + 1) + 1 + (nPolygonsAlongCurve + 1),
				*i2 = j + i * (nPolygonsAlongCurve + 1) + (nPolygonsAlongCurve + 1);
			N += 6;
		}
};

void parallelogramGenerate(Vertex2t *pv2tData, word *pIndices,
						   float Sx, float Sy, float Dz, int *vtxN, int *indN)
{
	Vertex2t *pv2t = NULL;
	// 0
	pv2t = pv2tData + 0;
	pv2t->x = 0, pv2t->y = 0, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0;
	pv2t->v = pv2t->v2 = 0;
	// 1
	pv2t = pv2tData + 1;
	pv2t->x = Sx, pv2t->y = 0, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0.125;
	pv2t->v = pv2t->v2 = 0;
	// 2
	pv2t = pv2tData + 2;
	pv2t->x = Sx, pv2t->y = Sy, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0.125;
	pv2t->v = pv2t->v2 = 0.125;
	// 3
	pv2t = pv2tData + 3;
	pv2t->x = 0, pv2t->y = Sy, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0;
	pv2t->v = pv2t->v2 = 0.125;
	// 4
	pv2t = pv2tData + 4;
	pv2t->x = 0, pv2t->y = 0, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0.25;
	pv2t->v = pv2t->v2 = 0.125;
	// 5
	pv2t = pv2tData + 5;
	pv2t->x = Sx, pv2t->y = 0, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0.625;
	pv2t->v = pv2t->v2 = 0.125;
	// 6
	pv2t = pv2tData + 6;
	pv2t->x = Sx, pv2t->y = Sy, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0.5;
	pv2t->v = pv2t->v2 = 0.125;
	// 7
	pv2t = pv2tData + 7;
	pv2t->x = 0, pv2t->y = Sy, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0.375;
	pv2t->v = pv2t->v2 = 0.125;
	// 8
	pv2t = pv2tData + 8;
	pv2t->x = 0, pv2t->y = 0, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0.25;
	pv2t->v = pv2t->v2 = 0;
	// 9
	pv2t = pv2tData + 9;
	pv2t->x = Sx, pv2t->y = 0, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0.625;
	pv2t->v = pv2t->v2 = 0;
	// 10
	pv2t = pv2tData + 10;
	pv2t->x = Sx, pv2t->y = Sy, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0.5;
	pv2t->v = pv2t->v2 = 0;
	// 11
	pv2t = pv2tData + 11;
	pv2t->x = 0, pv2t->y = Sy, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0.375;
	pv2t->v = pv2t->v2 = 0;
	// 12
	pv2t = pv2tData + 12;
	pv2t->x = 0, pv2t->y = 0, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0;
	pv2t->v = pv2t->v2 = 0.125;
	// 13
	pv2t = pv2tData + 13;
	pv2t->x = Sx, pv2t->y = 0, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0.125;
	pv2t->v = pv2t->v2 = 0.125;
	// 14
	pv2t = pv2tData + 14;
	pv2t->x = Sx, pv2t->y = Sy, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0.125;
	pv2t->v = pv2t->v2 = 0.25;
	// 15
	pv2t = pv2tData + 15;
	pv2t->x = 0, pv2t->y = Sy, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0;
	pv2t->v = pv2t->v2 = 0.25;
	// 16
	pv2t = pv2tData + 16;
	pv2t->x = 0, pv2t->y = 0, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0.125;
	pv2t->v = pv2t->v2 = 0;
	// 17
	pv2t = pv2tData + 17;
	pv2t->x = Sx, pv2t->y = 0, pv2t->z = Dz;
	pv2t->u = pv2t->u2 = 0.25;
	pv2t->v = pv2t->v2 = 0;
	// 18
	pv2t = pv2tData + 18;
	pv2t->x = Sx, pv2t->y = 0, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0.25;
	pv2t->v = pv2t->v2 = 0.125;
	// 19
	pv2t = pv2tData + 19;
	pv2t->x = 0, pv2t->y = 0, pv2t->z = 0;
	pv2t->u = pv2t->u2 = 0.125;
	pv2t->v = pv2t->v2 = 0.125;


	*vtxN = 20;

	word *i0 = NULL, *i1 = NULL, *i2 = NULL;
	// 012
	i0 = pIndices + 0, i1 = pIndices + 1, i2 = pIndices + 2;
	*i0 = 0, *i1 = 1, *i2 = 2;
	// 123
	i0 = pIndices + 3, i1 = pIndices + 4, i2 = pIndices + 5;
	*i0 = 0, *i1 = 2, *i2 = 3;
	// 12_13_14
	i0 = pIndices + 6, i1 = pIndices + 7, i2 = pIndices + 8;
	*i0 = 12, *i1 = 13, *i2 = 14;
	// 12_14_15
	i0 = pIndices + 9, i1 = pIndices + 10, i2 = pIndices + 11;
	*i0 = 12, *i1 = 14, *i2 = 15;
	// 695
	i0 = pIndices + 12, i1 = pIndices + 13, i2 = pIndices + 14;
	*i0 = 6, *i1 = 9, *i2 = 5;
	// 6_10_9
	i0 = pIndices + 15, i1 = pIndices + 16, i2 = pIndices + 17;
	*i0 = 6, *i1 = 10, *i2 = 9;
	// 7_11_10
	i0 = pIndices + 18, i1 = pIndices + 19, i2 = pIndices + 20;
	*i0 = 7, *i1 = 11, *i2 = 10;
	// 7_10_6
	i0 = pIndices + 21, i1 = pIndices + 22, i2 = pIndices + 23;
	*i0 = 7, *i1 = 10, *i2 = 6;
	// 4_11_7
	i0 = pIndices + 24, i1 = pIndices + 25, i2 = pIndices + 26;
	*i0 = 4, *i1 = 11, *i2 = 7;
	// 4_8_11
	i0 = pIndices + 27, i1 = pIndices + 28, i2 = pIndices + 29;
	*i0 = 4, *i1 = 8, *i2 = 11;
	// 18_17_16
	i0 = pIndices + 30, i1 = pIndices + 31, i2 = pIndices + 32;
	*i0 = 18, *i1 = 17, *i2 = 16;
	// 18_16_19
	i0 = pIndices + 33, i1 = pIndices + 34, i2 = pIndices + 35;
	*i0 = 18, *i1 = 16, *i2 = 19;

	*indN = 36;
};


#define PROBE_MAXN	4096
bool old_probeDraw()
{
	static float az = 0;
	do{
		BaseMesh *pMesh = NULL;
		pMesh = new BaseMesh();
		if(pMesh == NULL) return false;

		Vertex2t *pv2tData = NULL;
		word *pIndices = NULL;

		pMesh->create(PROBE_MAXN, PROBE_MAXN);
		pv2tData = (Vertex2t*)pMesh->getVertexData();
		if(pv2tData == NULL) return false;

		pIndices = pMesh->getIndices();
		if(pIndices == NULL) return false;
		
		int nVertices = 0, nIndices = 0;
/*
		parallelogramGenerate(pv2tData, pIndices, 300, 300, 300, &nVertices, &nIndices);
		az += 0.01f;
		probeShift(pv2tData, -150, -150, -150, nVertices);
		probeRotate(pv2tData, 0, 0, 1, az, nVertices);
		probeRotate(pv2tData, 1, 0, 0, az / 3, nVertices);
		pMesh->setTexture(IRS->GetTextureID("groundtex.bmp"));*/
		int Z = 2.8;
		beziersurfaceGenerate(pv2tData, pIndices,
			0, 0, Z*50,		Z*100, 0, Z*-20,		Z*200, 0, Z*-20,		Z*300, 0, Z*130,
			0, Z*100, 0,	Z*100, Z*100, Z*-100,	Z*200, Z*100, Z*-100,	Z*300, Z*100, 0,
			0, Z*200, 0,	Z*100, Z*200, Z*100,	Z*200, Z*200, Z*100,	Z*300, Z*200, 0,
			0, Z*300, Z*-130,	Z*100, Z*300, Z*20,	Z*200, Z*300, Z*20,		Z*300, Z*300, Z*50,
			16, &nVertices, &nIndices);
		for(int i = 0; i < nVertices; i++) (pv2tData + i)->diffuse = 0xFFFF0000;
		probeShift(pv2tData, -450, -450, 0, nVertices);
		probeRotate(pv2tData, 0, 0, 1, 3.14 / 6, nVertices);
		probeShift(pv2tData, 700, 700, 0, nVertices);
		

		pv2tData = NULL, pIndices = NULL;

		pMesh->setNInd(nIndices);
		pMesh->setNVert(nVertices);
		pMesh->setNPri(nIndices / 3);
		pMesh->setShader(IRS->GetShaderID("Wire"));
		DrawBM(*pMesh);
		delete pMesh;
		pMesh = NULL;
			
		FlipPages();
		ProcessMessages();
		IRS->ClearDevice(0,true,true);
		extern float fMapX;
		extern float fMapY;
		float fstep=0.05;
		if(GetKeyState(VK_LEFT)&0x8000){			
            fMapX-=fstep;
		}
		if(GetKeyState(VK_RIGHT)&0x8000){			
			fMapX+=fstep;;
		}
		if(GetKeyState(VK_UP)&0x8000){			
			fMapY-=fstep;
		}
		if(GetKeyState(VK_DOWN)&0x8000){			
			fMapY+=fstep;
		}
		void SetupCamera();
		SetupCamera();
	}while(true);
	return true;
}

bool probeDraw(void)
{
	float A = 0, Z = 2.8;
	int tStart = 0, tCreateSurface = 0, tModifySurface = 0,
		tCalcNormals = 0, tColoring = 0, tCalcShadow = 0,
		tGetMeshRef = 0, tSplit = 0;
	Vector3D v3dOrigin, v3dSun, v3dBase;
	v3dSun.x = 0, v3dSun.y = 1, v3dSun.z = 1;
	VertexBasedMesh VBMesh;
	// Cool Surface
/*	VBMesh.CreateBezierHexagonalPlane(v3dOrigin, 900, 900, 150, 150,
		200 + (200 << 8) + (200 << 16) + 0xFF000000, 0, 128,
		Z*50,		Z*-20,		Z*-20,	Z*130,
		Z*0,		Z*-100,		Z*-100,	Z*0,
		Z*0,		Z*100,		Z*100,	Z*0,
		Z*-130,		Z*20,		Z*20,	Z*50);
	v3dOrigin.x = 100, v3dOrigin.y = 100, v3dOrigin.z = 0;
	VBMesh.SetFactureInRadius(v3dOrigin, 150, 1);
	v3dOrigin.x = 800, v3dOrigin.y = 800, v3dOrigin.z = 0;
	VBMesh.SetFactureInRadius(v3dOrigin, 200, 3);
	v3dOrigin.x = 600, v3dOrigin.y = 800, v3dOrigin.z = 0;
	VBMesh.SetFactureInRadius(v3dOrigin, 200, 3);
	v3dOrigin.x = 500, v3dOrigin.y = 500, v3dOrigin.z = 0;
	VBMesh.SetFactureInRadius(v3dOrigin, 300, 5);
	v3dOrigin.x = 100, v3dOrigin.y = 800, v3dOrigin.z = 0;
	VBMesh.SetFactureInRadius(v3dOrigin, 350, 7);
	VBMesh.CalcNormals();
	VBMesh.CalcShadow(v3dSun, 0.2, 60, 255);*/

	BaseSurfaceFunction BSFunction;
	tStart = GetTickCount(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	VBMesh.CreateSurface(150, 150, 10, &BSFunction);
	tCreateSurface = GetTickCount(); // <<<<<<<<<<<<<<<<<<<<<<<
	VBMesh.ModifySurface(2, &BSFunction);
	tModifySurface = GetTickCount(); // <<<<<<<<<<<<<<<<<<<<<<<
	VBMesh.CalcNormals();
	tCalcNormals = GetTickCount(); // <<<<<<<<<<<<<<<<<<<<<<<<<
	for(int i = 0; i < VBMesh.Vert.GetAmount(); i++)
	{
		OneMapVertex& vtx = VBMesh.Vert[i];
		vtx.nFacture = vtx.nz > 120 ? 1:8;
		vtx.Color = 0xFF808080;
	}
	tColoring = GetTickCount(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	VBMesh.CalcShadow(v3dSun, 0.2, 60, 255);
//	VBMesh.CalcShadow();
	tCalcShadow = GetTickCount(); // <<<<<<<<<<<<<<<<<<<<<<<<<<
	VertexBasedMeshRef VBMeshRef;
	v3dOrigin.x = v3dOrigin.y = v3dOrigin.z = 0;
	VBMesh.GetMeshRefSphere(v3dOrigin, 9000, &VBMeshRef);
	tGetMeshRef = GetTickCount(); // <<<<<<<<<<<<<<<<<<<<<<<<<<
	OneSplitMesh OSMesh;
	OSMesh.AdvSplitMesh(&VBMeshRef);
	tSplit = GetTickCount(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// calculating and displaying time
	tSplit -= tGetMeshRef;
	tGetMeshRef -= tCalcShadow;
	tCalcShadow -= tColoring;
	tColoring -= tCalcNormals;
	tCalcNormals -= tModifySurface;
	tModifySurface -= tCreateSurface;
	tCreateSurface -= tStart;
	_str Str;
	Str = "CreateSurface = ";
	Str += tCreateSurface;
	Str += "\r\nModifySurface = ";
	Str += tModifySurface;
	Str += "\r\nCalcNormals = ";
	Str += tCalcNormals;
	Str += "\r\nColoring = ";
	Str += tColoring;
	Str += "\r\nCalcShadow = ";
	Str += tCalcShadow;
	Str += "\r\nGetMeshRef = ";
	Str += tGetMeshRef;
	Str += "\r\nSplit = ";
	Str += tSplit;
	MessageBox(NULL, Str.str, "Timing:", 0);
	do{
		A += 0.08;
		v3dSun.x = cos(A),
			v3dSun.y = 1,
			v3dSun.z = sin(A);
		for(int i = 0; i < VBMesh.Vert.GetAmount(); i++)
		{
			OneMapVertex *pvtx = 0;
			pvtx = &VBMesh.Vert[i];
			pvtx->Color = 0xFF808080;
		}
//		VBMesh.CalcShadow(v3dSun, 0.2, 60, 255);
//		VBMesh.CalcShadow();
		//OSMesh.AdvSplitMesh(&VBMeshRef);
//		OSMesh.SpeedySplit(VBMeshRef);
//		OSMesh.Draw();

		FlipPages();
		ProcessMessages();
		IRS->ClearDevice(1,true,true);
		extern float fMapX;
		extern float fMapY;
		float fstep = 0.5;
		if(GetKeyState(VK_LEFT) & 0x8000) fMapX -= fstep;
		if(GetKeyState(VK_RIGHT) & 0x8000) fMapX += fstep;
		if(GetKeyState(VK_UP) & 0x8000) fMapY -= fstep;
		if(GetKeyState(VK_DOWN)&0x8000) fMapY += fstep;
		void SetupCamera();
		SetupCamera();
	}while(true);
	return true;
}