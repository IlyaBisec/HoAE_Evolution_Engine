#pragma once

typedef struct tagVector{
	Vector3D vec; //, vecPos;

	tagVector() {}
	tagVector(const Vector3D& v) : vec(v){}

	unsigned int	hash() const
	{
		DWORD h = int(vec.x) + 541* int(vec.y) + int(vec.z)*81799;
		h = (h << 13) ^ h;
		h += 15731;
		return h;
	}

	bool	equal(const tagVector& el)
	{
		if(!el.vec.isEqual(vec))   return false;
		return true;
	}

	void	copy(const tagVector& el)
	{
		vec = el.vec; // vecPos = el.vecPos;
	}
} tagVector; // struct tagVector

struct CubeTriangulation{
	CubeTriangulation(){
		NPoints=0;
		NIndices=0;
	}
	Vector3D Points[12];
	int NPoints;
	int Indices[15];
	int NIndices;
};

typedef Hash <tagVector,1299709>    HashPoints;

class WholeTriangulation{
public:
	//	std::vector<Vector3D> Vert;
	std::vector<int>      Idxs;
	HashPoints            HVert;
	int AddVertex(Vector3D V);//it should return index of new vertex
	void AddIndex(int v);
	void SaveToPly(const char* name, float dScale);
	void SmartSmooth();
	void NormalSmooth(int ntimes);
};
typedef struct tagDWORD{
	DWORD v;
	unsigned int	hash() const{
		return v;
	}
	bool	equal(const tagDWORD& el){
		return el.v==v;
	}
	void	copy(const tagDWORD& el){
		v = el.v;
	}
};
class DWORD_hash{
	Hash <tagDWORD,1299709> D;
public:
	bool check(DWORD x){
		tagDWORD Dv;
		Dv.v=x;
		return D.find(Dv)!=NO_ELEMENT;
	}
	void add(DWORD x){
		tagDWORD Dv;
		Dv.v=x;
		D.add(Dv);
	}
};
#define  HashSize 179351
class CubeTriangulator{

	struct HashItem{
		float w;
		int x,y,z;
	};
	HashItem wHash[HashSize];	
	float GetFHashed(int x,int y,int z);
	int x0,y0,z0,x1,y1,z1;
	DWORD getcellidx(int x,int y,int z);
	void  getcellcoor(int& x,int& y,int& z,DWORD V);
	bool UseTransform;
	Matrix4D transform;
	Matrix4D transformInv;	
	bool AllowDistortion;
public:
	Vector3D Distort(int x,int y,int z);
	void SetTransform(Matrix4D& m,bool doDistort);
	CubeTriangulator();
	virtual bool GetValue(int x,int y,int z);
	virtual float GetValueF(float x,float y,float z){
		return GetValue(x,y,z)?-1:1;
	}
	DWORD GetTriangulation(int x,int y,int z,CubeTriangulation& T);
	DWORD GetDistortedTriangulation(int x,int y,int z,CubeTriangulation& T);
	void GetFullTriangilation(int x0,int y0,int z0,int x1,int y1,int z1,WholeTriangulation& WT);
	void GetFullTriangilationRecursive(int x,int y,int z,int x0,int y0,int z0,int x1,int y1,int z1,WholeTriangulation& WT);
};