#ifndef FSHADOW_H
#define FSHADOW_H

//#include "mVector.h"
//#include "mTransform.h"

//struct OneMapVertex {
//	short x, y, z;
//	unsigned char Shadow;
//};

class FShadow {
	int* Data;
	int Width, Height;
	Vector3D* TransformedVertices;
	int TransformedVerticesNumber;	
	int ShadowsNumber;
	void PutTriangle(Vector3D* V0, Vector3D* V1, Vector3D* V2);
	void Null();
public:
	float* Shadows;

	bool Initialize(int nSide);
	void Process(OneMapVertex* Vertices, DWORD* Indices, 
					 int VNum, int TNum, float dx, float dy, float dz, int Value);
	void Release();

	FShadow();
	~FShadow();
};

#endif
