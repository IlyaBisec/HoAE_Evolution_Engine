#include "stdafx.h"

//using namespace sg;

typedef struct tagTriProp {
	int				 nNumTri;
	float*			 dArrCamWeights;
	int				 nNumSta;
	std::vector<int> arrNeib;

	tagTriProp(){
		dArrCamWeights = NULL;
		nNumTri = nNumSta = -1;
		arrNeib.clear();
	}
	~tagTriProp(){if(dArrCamWeights) delete[] dArrCamWeights;}

} tagTriProp; //tagTriProp

//Temporary hash for quick search
typedef struct tagEdge{
	int nV1, nV2; //nV1 must be greater than nV2
	//	int nNumTriOwn;
	//	int nNumTriNeib;
	int nTri;

	tagEdge() {}
	tagEdge(int n1, int n2){
		nV1 = n1; nV2 = n2;
		if(n1 < n2)
			swap(nV1, nV2);
		nTri = -1;
		//nNumTriOwn = nNumTriNeib = -1;
	}
	tagEdge(int n1, int n2, int n){ 
		nV1 = n1; nV2 = n2;
		if(n1 < n2)
			swap(nV1, nV2);
		nTri = n;
		//nNumTriOwn = n;  nNumTriNeib = -1;
	}

	unsigned int	hash() const
	{
		DWORD h = nV1 + 541* nV2;
		h = (h << 13) ^ h;
		h += 15731;
		return h;
	}

	bool	equal(const tagEdge& el)
	{
		return ((nV1 == el.nV1)&&(nV2 == el.nV2)); 
	}

	void	copy(const tagEdge& el)
	{
		nV1 = el.nV1;
		nV2 = el.nV2;
		nTri = el.nTri;
		//		nNumTriOwn = el.nNumTriOwn; nNumTriNeib = el.nNumTriNeib;
	}
} tagEdge; // struct tagEdge

typedef Hash <tagEdge, 12997>  HashEdge; 

typedef struct tagElemList{
	tagElemList*   pNext;
	tagElemList*   pPrevious;
	int            nIndex;  

	tagElemList(){ pPrevious = NULL; pNext = NULL; }

	void operator = (const tagElemList& el){
		pNext = el.pNext; pPrevious = el.pPrevious; nIndex = el.nIndex;
	}
} tagElemList; // struct tagElemList

class Simplified_Mesh;

class VertexSimple : public queue_base{
public:
	//VertexSimple& operator = (VertexSimple& bc){
	//	assert(bc.m_NearVerts.size()==0 && bc.m_NearTriangles.size()==0);
	//	return *this;
	//}
	//VertexSimple& operator = (const VertexSimple& bc){		
	//	assert(0);
	//	return *this;
	//}
	int                     m_nIdx;
	Vector3D				m_ptPos;
	//	Vector3D				m_vecN;
	set_of_int				m_NearVerts;
	set_of_int				m_NearTriangles;
	int						m_nCollapseDirection;
	float					m_dCollapseCost;
	//	Simplified_Mesh*        m_pOwner;
	bool					m_bIsActive;
	bool					m_bBoundary;
	int						Code;
	VertexSimple();
	~VertexSimple();
	float					getCost();
	void					setCost(float _cost);
};

class TriangleSimple{
public:
	//TriangleSimple& operator = (TriangleSimple& bc){
	//	assert(bc.m_arrNeibTri.size()==0);
	//	return *this;
	//}
	//TriangleSimple& operator = (const TriangleSimple& bc){
	//	assert(0);
	//	return *this;
	//}
	int						m_nV1;
	int						m_nV2;
	int						m_nV3;
	int                     m_nIdx;
	Vector3D				m_vecN;
	bool					m_bIsActive;
	std::vector<int>        m_arrNeibTri;
	TriangleSimple();
	~TriangleSimple();
};

#define nsort 65536

class Simplified_Mesh{
public:
	void					Clear();																	//+	
	void					SetMaxVert(int n);															//+
	void					SetMaxTriangles(int n);														//+
	void					AddVertex(const Vector3D& vrt,int Code=0);												//+
	void					AddTriangle(int v1, int v2, int v3);										//+
	virtual float			GetCollapseCost(int v1, int v2);											//+
	void					CalcCollapsesCostForAll();													//+
	void					Collapse(int nIdx);															//-		
	void					Collapse();																	//-
	void					RemoveInactives();															//-
	void					CreateStructOfTriVrt();														//+
	void					CopyTo(Simplified_Mesh* dest);    //???								
	bool					SaveMeshToPly(int nNumVerts);															//-
	bool					ReadMeshFromPly(std::vector<Vector3D>& arrPts, std::vector<int>& arrIdx);	//+
	void                    SetFileRead(const char* fName);												//+
	void                    SetFileWrite(const char* fName);											//+
	void                    CreateInitMesh();
protected:
	int						m_nMaxVertSize;
	int						m_nMaxTrianglesSize;
	fast_queue				m_VertexSort;
	const char*				m_fNameRead;
	const char*				m_fNameWrite;
	void					CalculateTriNeighbours();
	void					FindTrianglesAreas();


public:
	//simplification constants
	float SquaredDistConst;//0.1
	float TriQualityConst;//0.0003
    float KeepNormalsCoef;
	//////////////////////////////////////////////////////////////////////////	
	Simplified_Mesh();
	~Simplified_Mesh();
	int						m_nVertSize;
	int						m_nTrianglesSize; 
	VertexSimple*			m_pVerts;
	TriangleSimple*			m_pTriangles;
	float					m_AABB_size2;

	//Functions of mesh's simplification
	void                    Simplify();						//The main function
	void                    CreateStructOfTriVrt(const std::vector<Vector3D>& arrPts, const std::vector<int>& arrIdx);
	void					SetSnapToPoints();
	void					GetNeibPointInRadius(int nIndex, float dRadius, const Vector3D& ptCenter, std::vector<int>& nArrIdxIn);
	void					SetVecNToTriangles();
};