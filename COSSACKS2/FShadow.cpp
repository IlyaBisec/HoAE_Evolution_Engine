#include "stdheader.h"
#include "surface.h"
#include "FShadow.h"

//----------------------------------------------------------------------------
FShadow::FShadow(void)
{
	Null();
}
//----------------------------------------------------------------------------
FShadow::~FShadow(void)
{
	Release();
}
//----------------------------------------------------------------------------
void FShadow::Null(void)
{	
	Data = NULL;
	Width = Height = 0;
	TransformedVertices = NULL;
	TransformedVerticesNumber = 0;
	Shadows = NULL;
	ShadowsNumber = 0;
}
//----------------------------------------------------------------------------
bool FShadow::Initialize(int nSide)
{	
	Release();
	Width = nSide;
	Height = nSide;
	Data = new int[Width*Height];
	return true;
}
//----------------------------------------------------------------------------
void FShadow::Release(void)
{
	if(Data) delete[] Data;
	if(TransformedVertices) delete[] TransformedVertices;
	if(Shadows) delete[] Shadows;
	Null();
}
//----------------------------------------------------------------------------
void FShadow::PutTriangle(Vector3D* V0, Vector3D* V1, Vector3D* V2)
{
   #define SWAP(a, b) { int t = a; a = b; b = t; }

   int x0 = (int)V0->x, y0 = (int)V0->y, z0 = (int)V0->z;
   int x1 = (int)V1->x, y1 = (int)V1->y, z1 = (int)V1->z;
   int x2 = (int)V2->x, y2 = (int)V2->y, z2 = (int)V2->z;

   // - сортировка по y
   if(y0 > y1)
   {
      SWAP(x0, x1);
      SWAP(y0, y1);
      SWAP(z0, z1);
   }
   if(y0 > y2)
   {
      SWAP(x0, x2);
      SWAP(y0, y2);
      SWAP(z0, z2);
   }
   if(y1 > y2)
   {
      SWAP(x1, x2);
      SWAP(y1, y2);
      SWAP(z1, z2);
   }

   if(y2-y0 == 0) return;

   for(int y = y0; y < y2; y++)
   {
      // - пересечение с длиннейшим ребром
      int _x1 = x0 + (y - y0)*(x2 - x0)/(y2 - y0);
      int _z1 = z0 + (y - y0)*(z2 - z0)/(y2 - y0);

      // - пересечение с коротким ребром
      int _x2, _z2;
      if(y < y1)
      {
         _x2 = x0 + (y - y0)*(x1 - x0)/(y1 - y0);
         _z2 = z0 + (y - y0)*(z1 - z0)/(y1 - y0);
      }
      else
      {
         if(y2 == y1)
         {
            _x2 = x1;
            _z2 = z1;
         }
         else
         {
            _x2 = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
            _z2 = z1 + (y - y1)*(z2 - z1)/(y2 - y1);
         }
      }

      // - обмен если надо
      if(_x1 > _x2)
      {
         SWAP(_x1, _x2);
         SWAP(_z1, _z2);
      }

      // - отрисовка линии
      for(int x = _x1; x < _x2; x++)
      {
         int z = _z1 + (x - _x1)*(_z2 - _z1)/(_x2 - _x1);
			if(x >= 0 && x < Width && y >= 0 && y < Height)
				if(Data[y*Width + x] < z) Data[y*Width + x] = z;
      }
   }
   #undef SWAP
}
//---------------------------------------------------------------------------
void FShadow::Process(OneMapVertex* Vertices, DWORD* Indices, int VNum, int TNum, 
							 float dx, float dy, float dz, int Value)
{
	Vector3D vz(dx, dy, dz);
	vz.normalize();
	vz.reverse();
	Vector3D vz0=vz;
	Vector3D vy(0, 0, 1);
	Vector3D vx;
	vx.cross(vz, vy);
	vx.normalize();
	vy.cross(vz, vx);
	vy.normalize();

	Matrix3D m(vx, vy, vz);
	m.transpose();

	if(TransformedVertices != NULL && TransformedVerticesNumber < VNum)
	{
		delete[] TransformedVertices;
		TransformedVertices = NULL;
	}
	if(TransformedVertices == NULL)
	{
		TransformedVertices = new Vector3D[VNum];
		TransformedVerticesNumber = VNum;
	}

	if(Shadows != NULL && ShadowsNumber < VNum)
	{
		delete[] Shadows;
		Shadows = NULL;
	}
	if(Shadows == NULL)
	{
		Shadows = new float[VNum];
		ShadowsNumber = VNum;

		// - очистка Shadows-буфера
		for(int i = 0; i < VNum; i++)
			Shadows[i] = 0.0f;
	}

	// - очистка z-буфера
	for(int i = 0; i < Width*Height; i++)
		Data[i] = -0x7FFFFFFF;

	int MinX = 0x7FFFFFFF, MaxX = -0x7FFFFFFF;
	int MinY = 0x7FFFFFFF, MaxY = -0x7FFFFFFF;
	int MinZ = 0x7FFFFFFF, MaxZ = -0x7FFFFFFF;

	// - трансформ вершин
	for(i = 0; i < VNum; i++)
	{
		Vector3D V(Vertices[i].x, Vertices[i].y, Vertices[i].z);
		V *= m;

		if((int)V.x < MinX) MinX = (int)V.x;
		if((int)V.x > MaxX) MaxX = (int)V.x;
		if((int)V.y < MinY) MinY = (int)V.y;
		if((int)V.y > MaxY) MaxY = (int)V.y;
		if((int)V.z < MinZ) MinZ = (int)V.z;
		if((int)V.z > MaxZ) MaxZ = (int)V.z;

		TransformedVertices[i] = V;
	}

	// - масштабирование 
	float ScaleX = (float)Width/(float)(MaxX-MinX);
	float ScaleY = (float)Height/(float)(MaxY-MinY);
	float ScaleZ = (float)0xFFFFFF/(float)(MaxZ-MinZ);
	for(i = 0; i < VNum; i++)
	{
		Vector3D& V = TransformedVertices[i];

		V.x = (float)(V.x - MinX - (MaxX-MinX)/2)*ScaleX + Width/2;
		V.y = (float)(V.y - MinY - (MaxY-MinY)/2)*ScaleY + Height/2;
		V.z = (float)(V.z - MinZ - (MaxZ-MinZ)/2)*ScaleZ;

		TransformedVertices[i] = V;
	}

	// - отрисовка z-буфера
	for(i = 0; i < TNum; i++)
	{
		int v0 = Indices[i*3+0];
		int v1 = Indices[i*3+1];
		int v2 = Indices[i*3+2];

		Vector3D V0 = TransformedVertices[v0];
		Vector3D V1 = TransformedVertices[v1];
		Vector3D V2 = TransformedVertices[v2];

		PutTriangle(&V0, &V1, &V2);
	}

	// - просчет тени
	for(i = 0; i < VNum; i++)
	{
		int x = (int)TransformedVertices[i].x;
		int y = (int)TransformedVertices[i].y;
		int z = (int)TransformedVertices[i].z;

		if(x >= 0 && x < Width && y >= 0 && y < Height && z >= Data[y*Width + x]){
			Vector3D N(Vertices[i].nx,Vertices[i].ny,Vertices[i].nz);
			N/=127;
			float dot=0.4+N.dot(vz0)*0.6;
			if(dot<0.4)dot=0.4;
			if(dot>1)dot=1;
			Shadows[i] += float(Value)*dot;
		}
	}

	// - раскраска вершин
	/*for(i = 0; i < VNum; i++)
	{
		int s = (int)Shadows[i];
		if(s > 255) s = 255;
		Vertices[i].Shadow = s;
	}*/

	/*
	FILE* out = fopen("!raw!.ppm", "wb");
	fprintf(out, "P5\n%u %u\n255\n", Width, Height);
	for(i = 0; i < Width*Height; i++)
		fputc(Data[i], out);
	fclose(out);
	*/
}
//----------------------------------------------------------------------------
