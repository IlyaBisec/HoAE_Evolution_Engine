#include "stdheader.h"
#include "pool.h"
#include "SimplifyMesh.h"
#include "Surface\Surface.h"

const int cArrSave[10] = {30000, 10000, 9000, 8000, 6000, 4000, 3000, 2000, 1500, 100};
float SimpLimit = 4e-5;

//extern Progress* gpProgress;

char sDrive	[_MAX_DRIVE];
char sDir	[_MAX_PATH];
char sFile	[_MAX_PATH];
char sExt	[_MAX_PATH];

extern bool DontCheckFlips;

void AddElem(tagElemList*& pEl, tagElemList*& pHead)
{
	if (!pHead){
		pHead = pEl;
		pEl->pNext = NULL;
		pEl->pPrevious = NULL;
	}else{
		pHead->pPrevious = pEl;
		pEl->pNext = pHead;
		pEl->pPrevious = NULL;
		pHead = pEl;
	}
}

void RemoveElem(tagElemList*& pEl, tagElemList*& pHead)
{
	if (pHead == pEl){
		pHead = pEl->pNext;
	}
	else
	{
		if(pEl->pPrevious) pEl->pPrevious->pNext = pEl->pNext;
		if(pEl->pNext) pEl->pNext->pPrevious = pEl->pPrevious;
	}
	delete pEl;
}

//Implementation of class VertexSimple
VertexSimple::VertexSimple()
{
	m_bIsActive = true;
}
VertexSimple::~VertexSimple() {
//	m_NearVerts.clear();
//	m_NearTriangles.clear();
}

void VertexSimple::setCost(float _cost)
{
	m_dCollapseCost = _cost;
}

float VertexSimple::getCost()
{
	return m_dCollapseCost;
}

//Implementation of class TriangleSimple
TriangleSimple::TriangleSimple()
{
	m_bIsActive = true;
//	std::vector<int>        m_arrNeibTri;
}
TriangleSimple::~TriangleSimple()
{
//	m_arrNeibTri.clear();
}

//Implementation of class Simplified_Mesh
void Simplified_Mesh::Clear()
{
	m_pVerts = NULL;
	m_pTriangles = NULL;
	m_nVertSize = 0; m_nMaxVertSize = 0; m_nTrianglesSize = 0; m_nMaxTrianglesSize = 0;
}

void Simplified_Mesh::SetMaxVert(int n)
{
	m_nMaxVertSize = n;
	m_pVerts = new VertexSimple[n];
}

void Simplified_Mesh::SetMaxTriangles(int n)
{
	m_nMaxTrianglesSize = n;
	m_pTriangles = new TriangleSimple[n];
}

void Simplified_Mesh::AddVertex(const Vector3D& vrt,int Code)
{
	VertexSimple Vert;
	Vert.m_ptPos = vrt;
	Vert.m_nIdx = m_nVertSize;
	Vert.m_bBoundary = false;
	Vert.Code=Code;
	m_pVerts[m_nVertSize] = Vert;
	m_nVertSize++;
}

void Simplified_Mesh::AddTriangle(int v1, int v2, int v3)
{
	TriangleSimple Tri;
	Tri.m_nV1 = v1; Tri.m_nV2 = v2; Tri.m_nV3 = v3; 
	Tri.m_nIdx = m_nTrianglesSize;
	m_pTriangles[m_nTrianglesSize] = Tri;
	m_nTrianglesSize++;
}

void Simplified_Mesh::CreateStructOfTriVrt()
{
	int nNum = 0;
	while (nNum != m_nTrianglesSize) {
		TriangleSimple* pTri = &m_pTriangles[nNum++];
		int na = pTri->m_nV1;
		int nb = pTri->m_nV2;
		int nc = pTri->m_nV3;

		VertexSimple* pVrt1 = &m_pVerts[na]; 
		VertexSimple* pVrt2 = &m_pVerts[nb]; 
		VertexSimple* pVrt3 = &m_pVerts[nc]; 

		pVrt1->m_nIdx = na; 
		pVrt1->m_bIsActive = true;
		pVrt1->m_NearTriangles.add(pTri->m_nIdx);
		pVrt1->m_NearVerts.add(nb); 
		pVrt1->m_NearVerts.add(nc); 

		pVrt2->m_nIdx = nb; 
		pVrt2->m_bIsActive = true;
		pVrt2->m_NearTriangles.add(pTri->m_nIdx);
		pVrt2->m_NearVerts.add(na); 
		pVrt2->m_NearVerts.add(nc); 

		pVrt3->m_nIdx = nc; 
		pVrt3->m_bIsActive = true;
		pVrt3->m_NearTriangles.add(pTri->m_nIdx);
		pVrt3->m_NearVerts.add(na); 
		pVrt3->m_NearVerts.add(nb); 
	}
}
inline float FastSqrt(float square)
{
	float retval;

	__asm {
		mov             eax, square
			sub             eax, 0x3F800000
			sar             eax, 1
			add             eax, 0x3F800000
			mov             [retval], eax
	}
	return retval;
}

__forceinline float distvv(Vector3D V1,Vector3D V2){
	V1-=V2;
	return FastSqrt(V1.x*V1.x+V1.y*V1.y+V1.z*V1.z);
}
__forceinline float _norm(Vector3D V1){
	return FastSqrt(V1.x*V1.x+V1.y*V1.y+V1.z*V1.z);
}
float GetTriQuality(Simplified_Mesh* mesh,int v1,int v2,int v3,float& maxL,Vector3D* norm=NULL){
	Vector3D& vec1 = mesh->m_pVerts[v1].m_ptPos;
	Vector3D& vec2 = mesh->m_pVerts[v2].m_ptPos;
	Vector3D& vec3 = mesh->m_pVerts[v3].m_ptPos;

	float L1=distvv(vec1,vec2);
	float L2=distvv(vec2,vec3);
	float L3=distvv(vec3,vec1);

	maxL=max(L1,max(L2,L3));
	
	if(norm){
		Vector3D V2=vec2;
		V2-=vec1;
		Vector3D V3=vec3;
		V3-=vec1;
		norm->cross(V2,V3);
		norm->normalize();
	}
	
	if(maxL<0.00000001)return 0;
	return ((L1+L2+L3)/2-maxL)/maxL;
}
bool ReqGoodTriangles=false;
__forceinline float _dot(Vector3D v1,Vector3D v2){
	return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}
/*
float Simplified_Mesh::GetCollapseCost(int v1, int v2)
{
	VertexSimple* V1 = m_pVerts + v1;
	VertexSimple* V2 = m_pVerts + v2;
	if(V1->m_bBoundary || V2->m_bBoundary)return 10000;
	Vector3D s = V1->m_ptPos;
	s -= V2->m_ptPos;
	float mincost = FLT_MAX; // from float.h
	bool bNeighborFound = false;	

	//calculating start triangles quality

	float minQ=10;

	//calculating vertex normale

	Vector3D vnorm(0,0,0);	

	float maxL_t=0;	

	for(int pos=0;pos<V1->m_NearTriangles.size();pos++){		
		TriangleSimple& t = m_pTriangles[V1->m_NearTriangles[pos]];
		if(t.m_bIsActive){
			float m;
			float Q=GetTriQuality(this,t.m_nV1,t.m_nV2,t.m_nV3,m);
			if(m>maxL_t)maxL_t=m;
			if(Q<minQ)minQ=Q;
			vnorm+=t.m_vecN;
		}
	}
	float d=_norm(vnorm);
	vnorm/=d;
	maxL_t=0;

	int j=v1;	
	int k=v2;
	VertexSimple& n = *V2;	
	float maxd=0;

	//calculating topological penalty

	int NNeibPts=0;
	int NearPts[16];//usually only 2 
	float topw=1;   //topological penalty

	//calculating vertices, that are parts of triangles, that contain k and j

	for(int pos2=0;pos2!=V1->m_NearTriangles.size();pos2++){
		TriangleSimple& t=m_pTriangles[V1->m_NearTriangles[pos2]];
		if(t.m_bIsActive){
			int v1=t.m_nV1;
			int v2=t.m_nV2;
			int v3=t.m_nV3;
			if(NNeibPts<16){
				if( (v1==k && v2==j) || (v2==k && v1==j) ){
					NearPts[NNeibPts++]=v3;
				}
				if( (v2==k && v3==j) || (v3==k && v2==j) ){
					NearPts[NNeibPts++]=v1;
				}
				if( (v1==k && v3==j) || (v3==k && v1==j) ){
					NearPts[NNeibPts++]=v2;
				}
			}

		}
	}

	//checking for topology corruption
	//looking for all neighbours of vertices j and k, that are equal and are not part of triangles, containing j and k.
	//if exists, then collapse leads to the topology corruption

	for(int pos2=0;pos2<V1->m_NearVerts.size();pos2++){
		VertexSimple& vv=m_pVerts[V1->m_NearVerts[pos2]];
		if(vv.m_bIsActive){
			for(int pos3=0;pos3<V2->m_NearVerts.size();pos3++){
				VertexSimple& vv=m_pVerts[V2->m_NearVerts[pos3]];
				if(vv.m_bIsActive){
					int i=V1->m_NearVerts[pos2];
					int p=V2->m_NearVerts[pos3];
					if(i==p){
						//if( V1->m_NearVerts.size()==3 && V2->m_NearVerts.size()==3 ){
						//	topw=10000000;
						//}else{
						for(int q=0;q<NNeibPts;q++){
							if(i==NearPts[q]){
								q=-1;
								break;
							}
						}
						if(q!=-1){
							//ohh.. its corrupted
							topw=10000000;
						}
						//}
					}

				}
			}
		}
	}		

	//triangle quality penalty/bonus

	float curQ=10;//triangle quality penalty

	//float minsc=1;

	float vnormp_min=10;//sharp details penalty				
	float vnormp_max=0;//sharp details penalty	
	float minsp=0.5;
	int nneib=0;

	for(int pos1=0;pos1<V1->m_NearTriangles.size();pos1++){
		TriangleSimple& t=m_pTriangles[V1->m_NearTriangles[pos1]];			
		if(t.m_bIsActive){

			//calculating changed triangles quality

			float maxL=0;
			Vector3D norm;
			int vv1=t.m_nV1==v1?v2:t.m_nV1;
			int vv2=t.m_nV2==v1?v2:t.m_nV2;
			int vv3=t.m_nV3==v1?v2:t.m_nV3;
			if(vv1!=vv2&&vv2!=vv3&&vv1!=vv3){
				float Q=GetTriQuality(this,vv1,vv2,vv3,maxL,&norm);				
				//float sp=t.m_vecN.dot(norm);
				//if(sp<minsp)minsp=sp;

				if(maxL>maxL_t)maxL_t=maxL;
				if(Q>0&&Q<curQ)curQ=Q;

				//calc max distance

				float d=_dot(s,t.m_vecN);
				d*=d;
				if(d>maxd)maxd=d;

				//sharp edges penalty

				float vn=_dot(vnorm,t.m_vecN);
				if(vn>vnormp_max)vnormp_max=vn;			
				if(vn<vnormp_min)vnormp_min=vn;
				nneib++;
			}
		}
	}
	if(minsp<0.00001)minsp=0.00001;
	float D=_norm(s);
	//float coefQ=(minQ+0.2)/(curQ+0.2);	
	float coefQ=(minQ+0.2)/(curQ+0.2);	
	coefQ*=coefQ;
	if(coefQ<1)coefQ=(1+coefQ*4)/5;
	if(minQ<0.0001){
		coefQ*=0.001;
	}

	float vnormp=(vnormp_min+vnormp_max)/2;

	if(vnormp<0.01)vnormp=0.01;
	vnormp=1/vnormp;
	if(vnormp<1.5)vnormp=1.5;//dont be afraid if curvature is small enough

	//float cost=((maxd+maxL_t*maxL_t*0.001)*coefQ*topw/minsp);//its the final cost
	float cost=0;
	if(!ReqGoodTriangles){
		//cost=((maxd+maxL_t*maxL_t*0.0003)*coefQ*topw*vnormp*vnormp/minsp/m_AABB_size2/2.5);
		cost=SquaredDistConst*((maxd+maxL_t*maxL_t*TriQualityConst)*coefQ*topw*vnormp*vnormp/minsp/m_AABB_size2);
	}else{
		cost=((maxd+maxL_t*maxL_t*0.1)*coefQ*topw/minsp/m_AABB_size2/32);
	}
	if(V1->m_ptPos.z<0){
		float co=30/(fabs(30-V1->m_ptPos.z)+0.01);
		if(co<0.25)co=0.25;
		cost*=co;
	}
	//float cost=maxd*topw/minsp/m_AABB_size2;//its the final cost

	return cost;
}
*/
float Simplified_Mesh::GetCollapseCost(int v1, int v2)
{
	VertexSimple* V1 = m_pVerts + v1;
	VertexSimple* V2 = m_pVerts + v2;
	if(V1->m_bBoundary || V2->m_bBoundary)return 10000;
	if(V1->Code!=V2->Code)return 10000;
	Vector3D s = V1->m_ptPos;
	s -= V2->m_ptPos;
	float mincost = FLT_MAX; // from float.h
	bool bNeighborFound = false;	

	//calculating start triangles quality

	float minQ=10;

	//calculating vertex normale

	Vector3D vnorm(0,0,0);	

	float maxL_t=0;	

    float mintdot=1;

    int nv1t=V1->m_NearTriangles.size();
	for(int pos=0;pos<nv1t;pos++){
		TriangleSimple& t = m_pTriangles[V1->m_NearTriangles[pos]];
		if(t.m_bIsActive){
			float m;
			float Q=GetTriQuality(this,t.m_nV1,t.m_nV2,t.m_nV3,m);
			if(m>maxL_t)maxL_t=m;
			if(Q<minQ)minQ=Q;
			vnorm+=t.m_vecN;
            /*
            for(int pos2=0;pos2<V1->m_NearTriangles.size();pos2++){		
                TriangleSimple& t2 = m_pTriangles[V1->m_NearTriangles[pos2]];
                if(t2.m_bIsActive){
                    float dp=t2.m_vecN.dot(t.m_vecN);
                    if(dp<mintdot)mintdot=dp;
                }
            }
            */
		}
	}
	int nv2t=V2->m_NearTriangles.size();
	for(int pos=0;pos<nv2t;pos++){
		TriangleSimple& t = m_pTriangles[V2->m_NearTriangles[pos]];
		if(t.m_bIsActive){
			float m;
			float Q=GetTriQuality(this,t.m_nV1,t.m_nV2,t.m_nV3,m);			
			if(Q<minQ)minQ=Q;			
		}
	}
    int nt1=V1->m_NearTriangles.size();
    int nt2=V2->m_NearTriangles.size();
    for(int i=0;i<nt1;i++){
        Vector3D VN1=m_pTriangles[V1->m_NearTriangles[i]].m_vecN;
        for(int j=0;j<nt2;j++){
            float dp=m_pTriangles[V2->m_NearTriangles[j]].m_vecN.dot(VN1);
            if(dp<mintdot)mintdot=dp;
        }
    }
	//if(mintdot<0.75)return 10000;

	float d=_norm(vnorm);
	vnorm/=d;
	maxL_t=0;

	int j=v1;	
	int k=v2;
	VertexSimple& n = *V2;	
	float maxd=0;

	//calculating topological penalty

	int NNeibPts=0;
	int NearPts[16];//usually only 2 
	float topw=1;   //topological penalty

	//calculating vertices, that are parts of triangles, that contain k and j

	for(int pos2=0;pos2!=nt1;pos2++){
		TriangleSimple& t=m_pTriangles[V1->m_NearTriangles[pos2]];
		if(t.m_bIsActive){
			int v1=t.m_nV1;
			int v2=t.m_nV2;
			int v3=t.m_nV3;
			if(NNeibPts<16){
				if( (v1==k && v2==j) || (v2==k && v1==j) ){
					NearPts[NNeibPts++]=v3;
				}
				if( (v2==k && v3==j) || (v3==k && v2==j) ){
					NearPts[NNeibPts++]=v1;
				}
				if( (v1==k && v3==j) || (v3==k && v1==j) ){
					NearPts[NNeibPts++]=v2;
				}
			}

		}
	}

	//checking for topology corruption
	//looking for all neighbours of vertices j and k, that are equal and are not part of triangles, containing j and k.
	//if exists, then collapse leads to the topology corruption

	for(int pos2=0;pos2<V1->m_NearVerts.size();pos2++){
		VertexSimple& vv=m_pVerts[V1->m_NearVerts[pos2]];
		if(vv.m_bIsActive){
			for(int pos3=0;pos3<V2->m_NearVerts.size();pos3++){
				VertexSimple& vv=m_pVerts[V2->m_NearVerts[pos3]];
				if(vv.m_bIsActive){
					int i=V1->m_NearVerts[pos2];
					int p=V2->m_NearVerts[pos3];
					if(i==p){
						//if( V1->m_NearVerts.size()==3 && V2->m_NearVerts.size()==3 ){
						//	topw=10000000;
						//}else{
						for(int q=0;q<NNeibPts;q++){
							if(i==NearPts[q]){
								q=-1;
								break;
							}
						}
						if(q!=-1){
							//ohh.. its corrupted
							topw=10000000;
						}
						//}
					}

				}
			}
		}
	}		

	//triangle quality penalty/bonus

	float curQ=10;//triangle quality penalty

	//float minsc=1;

	float vnormp_min=10;//sharp details penalty				
	float vnormp_max=0;//sharp details penalty	
	float minsp=1.0;
	if(cMath::IsZero(KeepNormalsCoef)) {
		minsp = 0.5f;
	}
	int nneib=0;

	for(int pos1=0;pos1<V1->m_NearTriangles.size();pos1++){
		TriangleSimple& t=m_pTriangles[V1->m_NearTriangles[pos1]];			
		if(t.m_bIsActive){

			//calculating changed triangles quality

			float maxL=0;
			Vector3D norm;
			int vv1=t.m_nV1==v1?v2:t.m_nV1;
			int vv2=t.m_nV2==v1?v2:t.m_nV2;
			int vv3=t.m_nV3==v1?v2:t.m_nV3;
			if(vv1!=vv2&&vv2!=vv3&&vv1!=vv3){
				float Q=GetTriQuality(this,vv1,vv2,vv3,maxL,&norm);				
				float sp=t.m_vecN.dot(norm);
				if(sp<minsp)minsp=sp;

				if(maxL>maxL_t)maxL_t=maxL;
				if(Q>0&&Q<curQ)curQ=Q;

				//calc max distance

				float d=_dot(s,t.m_vecN);
				d*=d;
				if(d>maxd)maxd=d;

				//sharp edges penalty

				float vn=_dot(vnorm,t.m_vecN);
				if(vn>vnormp_max)vnormp_max=vn;			
				if(vn<vnormp_min)vnormp_min=vn;
				nneib++;
			}
		}
	}
	for(int pos1=0;pos1<V2->m_NearTriangles.size();pos1++){
		TriangleSimple& t=m_pTriangles[V2->m_NearTriangles[pos1]];			
		if(t.m_bIsActive){

			//calculating changed triangles quality

			float maxL=0;
			Vector3D norm;
			int vv1=t.m_nV1==v1?v2:t.m_nV1;
			int vv2=t.m_nV2==v1?v2:t.m_nV2;
			int vv3=t.m_nV3==v1?v2:t.m_nV3;
			if(vv1!=vv2&&vv2!=vv3&&vv1!=vv3){
				float Q=GetTriQuality(this,vv1,vv2,vv3,maxL,&norm);				
				if(Q>0&&Q<curQ)curQ=Q;
			}
		}
	}
	if(minsp<0.00001)minsp=0.00001;
	float D=_norm(s);
	//float coefQ=(minQ+0.2)/(curQ+0.2);	
	float coefQ=1;//(minQ+0.2)/(curQ+0.2);
	//if(coefQ>1.5 || (curQ<0.01 && coefQ>0.5))return 100000;
	//if(curQ<0.1 && coefQ>1)return 100000;
	//coefQ*=coefQ;
	//coefQ*=coefQ;
	//if(coefQ<1)coefQ=(1+coefQ*4)/5;
	//if(minQ<0.0001){
	//	coefQ*=0.001;
	//}

	float cost=0;
	if(!ReqGoodTriangles){
        //static float sp_coef=50;
		cost=SquaredDistConst*((maxd+maxL_t*maxL_t*TriQualityConst)*(1+(1-mintdot)*(1-mintdot)*KeepNormalsCoef/3)*coefQ*topw/minsp/m_AABB_size2);
	}else{
		cost=((maxd+maxL_t*maxL_t*0.1)*coefQ*topw/minsp/m_AABB_size2/3);
	}
	if(V1->m_ptPos.z<0){
		float co=30/(fabs(30-V1->m_ptPos.z)+0.01);
		if(co<0.25)co=0.25;
		cost*=co;
	}
	//float cost=maxd*topw/minsp/m_AABB_size2;//its the final cost

	return cost;
}
void Simplified_Mesh::SetFileRead(const char* fName)
{
	m_fNameRead = fName;
}

void Simplified_Mesh::SetFileWrite(const char* fName)
{
	m_fNameWrite = fName;
}

bool Simplified_Mesh::ReadMeshFromPly(std::vector<Vector3D>& arrPts, std::vector<int>& arrIdx)
{
	std::string str = m_fNameRead;

	FILE* fp = fopen(str.c_str(), "r");
	if (!fp) return false;

	char chr[256];
	int nNumIdx, nNumVrt;
	arrPts.clear(); arrIdx.clear();

	fscanf(fp, "%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s", chr, chr, chr, chr, chr, chr, &nNumVrt, 
		chr, chr, chr, chr, chr, chr, chr, chr, chr, chr, chr, &nNumIdx, chr, chr, chr, chr, chr, chr);
	Vector3D pt;
	for(int i = 0; i < nNumVrt; i++){
		fscanf(fp, "%f%f%f", &pt.x, &pt.y, &pt.z);
		//pt *= 10;
		arrPts.push_back(pt);
	}

	int nNum[4];
	for(int i = 0; i < nNumIdx; i++){
		fscanf(fp, "%d%d%d%d", &nNum[0], &nNum[1], &nNum[2], &nNum[3]);
		arrIdx.push_back(nNum[1]);
		arrIdx.push_back(nNum[2]);
		arrIdx.push_back(nNum[3]);
	}
	fclose(fp);	

	return true;
}

bool Simplified_Mesh::SaveMeshToPly(int nNumVerts)
{
	std::string str = m_fNameWrite;
	char ch[256];
	sprintf(ch, "%d", nNumVerts);
	str.erase( str.size() - 4, 4);
	str += '_';
	str += ch;
	str += ".ply";
	FILE* fp = fopen(str.c_str(), "w");
	if (!fp) return false;

	//Renumbering arrays
	std::vector<int> arrIdxNewForOld;  
	std::vector<int> arrIdx;
	std::vector<Vector3D> arrVrtSimple;
	int nNumVert = 0;

	//Delete vertexes from the array
	for(int i = 0; i < m_nVertSize; i++){
		VertexSimple* pVrt = &m_pVerts[i];
		if(!pVrt->m_bIsActive){
			arrIdxNewForOld.push_back(-1); 
			continue;
		}else{
			arrIdxNewForOld.push_back(nNumVert);
			arrVrtSimple.push_back(pVrt->m_ptPos);
			nNumVert++;
		}
	}

	for(long i = 0; i < m_nTrianglesSize; i++){
		TriangleSimple* pTri = &m_pTriangles[i];
		if(!pTri->m_bIsActive) continue;
		int nIdx1 = arrIdxNewForOld[pTri->m_nV1];
		int nIdx2 = arrIdxNewForOld[pTri->m_nV2];
		int nIdx3 = arrIdxNewForOld[pTri->m_nV3];
		arrIdx.push_back(nIdx1);
		arrIdx.push_back(nIdx2);
		arrIdx.push_back(nIdx3);
	}

	int nIdxCount = arrIdx.size();
	int nPtCount = arrVrtSimple.size();
	int nFaceCount = nIdxCount / 3;

	fprintf(fp, "%s\n", "ply");
	fprintf(fp, "%s\n", "format ascii 1.0");
	fprintf(fp, "%s%d\n", "element vertex ", nPtCount);
	fprintf(fp, "%s\n", "property float32 x");
	fprintf(fp, "%s\n", "property float32 y");
	fprintf(fp, "%s\n", "property float32 z");
	fprintf(fp, "%s%d\n", "element face ", nFaceCount);
	fprintf(fp, "%s\n", "property list uint8 int32 vertex_indices");
	fprintf(fp, "%s\n", "end_header");

	for(int i = 0; i < arrVrtSimple.size(); i++){
		Vector3D pt = arrVrtSimple[i];
		fprintf(fp, "%.3f %.3f %.3f\n", pt.x, pt.y, pt.z);
	}

	int n = 0;
	while (n != nIdxCount) {
		int n1 = arrIdx[n++];
		int n2 = arrIdx[n++];
		int n3 = arrIdx[n++];
		fprintf(fp, "3 %d %d %d\n", n1, n2, n3);
	}

	fclose(fp);
	return true;
}

void Simplified_Mesh::CreateInitMesh()
{
	//Reading triangulated init. mesh
	CreateStructOfTriVrt();	
}

//The main function of Simplified_Mesh to simplify it
void Simplified_Mesh::Simplify()
{
	CreateInitMesh();

	//set normals to all triangles
	SetVecNToTriangles();

	//Calculates first values of costs and create m_VertexSort
	CalcCollapsesCostForAll();

	//Simplification! 
	
	int nCountColpse = m_nVertSize - cArrSave[9] + 1;
	float dPercent = 0.0;
	int nNumSave = 0;
	int nNumAll = m_nVertSize;	

	//FILE* sl=fopen("simp.log","w");
	float maxv=0;
	int lastn=30000;
	bool done30000=false;
	float lastc=0;

	for(int i = 0; i < nCountColpse; i++){

		VertexSimple* quDel;
		float c=0;
		do{
			quDel = (VertexSimple*) m_VertexSort.get_cheapest();
			if(quDel->m_nCollapseDirection==-1)return;
			lastc = c = GetCollapseCost(quDel->m_nIdx,quDel->m_nCollapseDirection);
			if(fabs(c-quDel->m_dCollapseCost)>0.000001){
				m_VertexSort.remove(*quDel);
				float dMinCost = FLT_MAX;
				int nIdxToColps = 0;
				for(int in = 0; in < quDel->m_NearVerts.size(); in++){
					int nCurVrt = quDel->m_NearVerts[in];
					float dCost = GetCollapseCost(quDel->m_nIdx, nCurVrt);
					if(dCost < dMinCost){
						dMinCost = dCost;
						nIdxToColps = nCurVrt;
					}
				}
				quDel->m_nCollapseDirection=nIdxToColps;
				quDel->m_dCollapseCost=dMinCost;
				m_VertexSort.add(*quDel);
			}else break;
		}while(true);

		if(c>maxv)maxv=c;
		int nn=nNumAll - i;
		//if(lastn-nn>=50){
		//	if(sl){
		//		fprintf(sl,"%d %f\n",nn,c);
		//	}
		//	lastn=nn;
		//}

		int nIdxDel = quDel->m_nIdx;
		if(quDel->m_bIsActive){
			Collapse(nIdxDel); //Collapsing the chipest vertex and recalculation costs for neighbors
		}
		m_VertexSort.remove(*quDel);
		VertexSimple* pVrt = & m_pVerts[nIdxDel];
		pVrt->m_bIsActive = false;
		pVrt->m_NearTriangles.clear();
		pVrt->m_NearVerts.clear();
		/*
		if(cArrSave[nNumSave] > (nNumAll - i)){
		//	nNumSave++;
		//}
		//if((nNumAll - i) == cArrSave[nNumSave]){
		if(nNumSave == 9)
		break;
		if(!SaveMeshToPly(cArrSave[nNumSave])) 
		return;
		nNumSave++;			
		}
		if(nNumSave>2)break;
		*/

		//if( nn<=30000 && nn>10000 && !done30000){
			//SaveMeshToPly(30000);
			//done30000=true;
		//	break;
		//}
		if( c>SimpLimit ){
			//SaveMeshToPly(10000);
			//printf("(%d vertices in ply file) ",nn);
			break;
		}
	}//for
	//if(sl)fclose(sl);	

	//	RemoveInactives();
	//SaveMeshToPly(cArrSave[nNumSave]);
}

void Simplified_Mesh::CalculateTriNeighbours()
{
	//checking for potential nicked vertexes
	for(int i = 0; i < m_nVertSize; i++){
		VertexSimple* pVrt = & m_pVerts[i];
		if(!pVrt->m_bIsActive)
			continue;
		int nV = pVrt->m_NearVerts.size();
		int nT = pVrt->m_NearTriangles.size();
		if((nV < 3)||(nT < 3)||(nT != nV)){
			int h = 0;
		}
	}
	HashEdge hEdge;
	int nArr[3];
	for(int i = 0; i < m_nTrianglesSize; i++){
		TriangleSimple* pTri = &m_pTriangles[i];
		pTri->m_arrNeibTri.clear();
	}
	for(int i = 0; i < m_nTrianglesSize; i++){
		TriangleSimple* pTri = &m_pTriangles[i];
		if(!pTri->m_bIsActive){
			continue;
		}
		nArr[0] = pTri->m_nV1;
		nArr[1] = pTri->m_nV2;
		nArr[2] = pTri->m_nV3;
		VertexSimple* pVrt = &m_pVerts[pTri->m_nV1];
		if(!pVrt->m_bIsActive){
			int h = 0;
		}
		pVrt = &m_pVerts[pTri->m_nV2];
		if(!pVrt->m_bIsActive){
			int h = 0;
		}
		pVrt = &m_pVerts[pTri->m_nV3];
		if(!pVrt->m_bIsActive){
			int h = 0;
		}
		for(int ie = 0; ie < 3; ie++){
			int n1 = nArr[ie];
			int n2 = nArr[(ie +1) % 3]; 
			tagEdge Edge(n1, n2, i);
			int idx = hEdge.find(Edge);
			if(idx == -1){
				hEdge.add(Edge);
			}else{
				Edge = hEdge.elem(idx);
				assert(Edge.nTri != -1);
				pTri->m_arrNeibTri.push_back(Edge.nTri);
				TriangleSimple* pTriNeib = &m_pTriangles[Edge.nTri];
				pTriNeib->m_arrNeibTri.push_back(i);
			}
		}//ie - edges
	}//i - triangles

	//checking the number of heighbour triangles
	int n_0 = 0, n_1 = 0, n_2 = 0, n_4 = 0, n_n = 0;
	for(int i = 0; i < m_nTrianglesSize; i++){
		TriangleSimple* pTri = &m_pTriangles[i];
		if(!pTri->m_bIsActive)
			continue;
		int nTri = pTri->m_arrNeibTri.size();
		if(nTri != 3){
			int h = 0;
		}
		switch(nTri) {	
		case 0: n_0++;
			break;
		case 1: n_1++;
			break;
		case 2: n_2++;
			break;
		case 3: 
			break;
		default: n_n++;
			break;
		}
	}
	if((n_0 != 0)||(n_1 != 0)||(n_2 != 0)||(n_4 != 0)||(n_n != 0)){
		int h = 0;
	}
}

void Simplified_Mesh::FindTrianglesAreas()
{
	float a, b, c, p, s, max;
	for(int i = 0; i < m_nTrianglesSize; i++){
		TriangleSimple* pTri = &m_pTriangles[i];
		if(!pTri->m_bIsActive) continue;
		Vector3D pt1, pt2, pt3;
		VertexSimple* pVrt = &m_pVerts[pTri->m_nV1];
		if(!pVrt->m_bIsActive) continue;
		pt1 = pVrt->m_ptPos;
		pVrt = &m_pVerts[pTri->m_nV2];
		if(!pVrt->m_bIsActive) continue;
		pt2 = pVrt->m_ptPos;
		pVrt = &m_pVerts[pTri->m_nV3];
		if(!pVrt->m_bIsActive) continue;
		pt3 = pVrt->m_ptPos;
		a = pt1.distance(pt2);
		b = pt1.distance(pt3);
		c = pt2.distance(pt3);
		p = (a + b + c)/2;
		max = a;
		if(max < b) max = b;
		if(max < c) max = c;
		try{
			s = sqrt(p*(p - a)*(p - b)*(p - c));
			if(s < ((max*max) / 200)){
				int h = 0;
				if (max == a) {
					Collapse(pTri->m_nV1);
					pVrt = &m_pVerts[pTri->m_nV1];
					pVrt->m_bIsActive = false;
				}
				if (max == b) {
					Collapse(pTri->m_nV2);
					pVrt = &m_pVerts[pTri->m_nV2];
					pVrt->m_bIsActive = false;
				}				
				if (max == c) {
					Collapse(pTri->m_nV3);
					pVrt = &m_pVerts[pTri->m_nV3];
					pVrt->m_bIsActive = false;
				}
			}
		}catch (...) {
			continue;
		}
	}
}

void Simplified_Mesh::Collapse(int nIdxDel)
{
	VertexSimple& pVrtDel = m_pVerts[nIdxDel]; 
	if(!pVrtDel.m_bIsActive){
		int h = 0;
	}
	for(int i = 0; i < pVrtDel.m_NearVerts.size(); i++){
		int nIdxVrt = pVrtDel.m_NearVerts[i];
		assert((nIdxVrt >= 0)&&(nIdxVrt < m_nVertSize));
	}

	int nIdxNew = pVrtDel.m_nCollapseDirection;
	VertexSimple* pVrtNew = &m_pVerts[nIdxNew];
	if(!pVrtNew->m_bIsActive){
		int h = 0;
	}

	//Finding triangles to delete
	for(int i = 0; i < pVrtDel.m_NearTriangles.size(); i++){
		int nIdxTri = pVrtDel.m_NearTriangles[i];
		assert((nIdxTri >= 0)&&(nIdxTri < m_nMaxTrianglesSize));
		TriangleSimple* Tri = &m_pTriangles[nIdxTri];
		int nIdx1 = Tri->m_nV1; assert((nIdx1 >= 0)&&(nIdx1 < m_nVertSize));
		int nIdx2 = Tri->m_nV2; assert((nIdx2 >= 0)&&(nIdx2 < m_nVertSize));
		int nIdx3 = Tri->m_nV3; assert((nIdx3 >= 0)&&(nIdx3 < m_nVertSize));
		if((nIdx1 == nIdxNew)||(nIdx2 == nIdxNew)||(nIdx3 == nIdxNew)){
			//Deleting this triangle from it's 3 vertexes
			Tri->m_bIsActive = false;
			VertexSimple* pVrt1 = &m_pVerts[nIdx1]; 
			if(nIdx1 != nIdxDel) 
				pVrt1->m_NearTriangles.del(nIdxTri);
			VertexSimple* pVrt2 = &m_pVerts[nIdx2]; 	           
			if(nIdx2 != nIdxDel) 
				pVrt2->m_NearTriangles.del(nIdxTri);
			VertexSimple* pVrt3 = &m_pVerts[nIdx3];			   
			if(nIdx3 != nIdxDel) 
				pVrt3->m_NearTriangles.del(nIdxTri);
			int h = 0;
			//			continue;
		}else{
			//Adding this triangle to the list of the new vertex
			if(nIdx1 == nIdxDel) Tri->m_nV1 = nIdxNew;
			if(nIdx2 == nIdxDel) Tri->m_nV2 = nIdxNew;
			if(nIdx3 == nIdxDel) Tri->m_nV3 = nIdxNew;
			pVrtNew->m_NearTriangles.add(nIdxTri);
		}
	}

	for(int i = 0; i < pVrtDel.m_NearVerts.size(); i++){
		int nIdxVrt = pVrtDel.m_NearVerts[i];
		assert((nIdxVrt >= 0)&&(nIdxVrt < m_nVertSize));
		VertexSimple* pVrt = &m_pVerts[nIdxVrt];
		pVrt->m_NearVerts.del(nIdxDel);
		if(pVrt->m_nIdx != nIdxNew){
			pVrt->m_NearVerts.add(nIdxNew);
			pVrtNew->m_NearVerts.add(pVrt->m_nIdx);
		}
	}

	pVrtDel.m_bIsActive = false;	
	//Recalculation costs for vertexes near vrtDel 
	//(though vrtDel has been just deleted, but it still has old right list of neighbor vertexes and triangles)
	//Recalculation of normals for neighbor triangles
	for(int i = 0; i < pVrtDel.m_NearTriangles.size(); i++){
		int nIdxTri = pVrtDel.m_NearTriangles[i];
		assert((nIdxTri >= 0)&&(nIdxTri < m_nTrianglesSize));
		TriangleSimple* pTri = &m_pTriangles[nIdxTri];
		if(!pTri->m_bIsActive) continue;
		assert((pTri->m_nV1 >= 0)&&(pTri->m_nV1 < m_nVertSize)); VertexSimple* pVrt1 = &m_pVerts[pTri->m_nV1]; 
		assert((pTri->m_nV2 >= 0)&&(pTri->m_nV2 < m_nVertSize)); VertexSimple* pVrt2 = &m_pVerts[pTri->m_nV2];
		assert((pTri->m_nV3 >= 0)&&(pTri->m_nV3 < m_nVertSize)); VertexSimple* pVrt3 = &m_pVerts[pTri->m_nV3];
		Vector3D vecRes;
		Vector3D vec1 = pVrt2->m_ptPos; vec1 -= pVrt1->m_ptPos;
		Vector3D vec2 = pVrt3->m_ptPos; vec2 -= pVrt1->m_ptPos;
		vecRes.cross(vec1, vec2);  vecRes.normalize();
		pTri->m_vecN = vecRes;
	}

	//Recalculation costs
	for(int i = 0; i < pVrtDel.m_NearVerts.size(); i++){
		int nIdxVrt = pVrtDel.m_NearVerts[i];
		assert((nIdxVrt >= 0)&&(nIdxVrt < m_nVertSize));
		VertexSimple* pVrt = &m_pVerts[nIdxVrt];
		float dMinCost = FLT_MAX;
		int nIdxToColps = -1;
		for(int in = 0; in < pVrt->m_NearVerts.size(); in++){
			int nCurVrt = pVrt->m_NearVerts[in];
			assert((nCurVrt >= 0)&&(nCurVrt < m_nVertSize));
			float dCost = GetCollapseCost(pVrt->m_nIdx, nCurVrt);
			if(dCost < dMinCost){
				dMinCost = dCost;
				nIdxToColps = nCurVrt;
			}
		}
		if(nIdxToColps != pVrt->m_nIdx){
			m_VertexSort.remove(*pVrt);
			pVrt->m_dCollapseCost = dMinCost;
			pVrt->m_nCollapseDirection = nIdxToColps;
			m_VertexSort.add(*pVrt);

		}else{
			pVrt->m_dCollapseCost = FLT_MAX;
			pVrt->m_nCollapseDirection = pVrt->m_nIdx;
		}
	}
	pVrtDel.m_NearTriangles.clear();
	pVrtDel.m_NearVerts.clear();
}

void Simplified_Mesh::RemoveInactives()
{  
	VertexSimple* pArrVrtBuf = new VertexSimple[m_nMaxVertSize];
	std::vector<int> arrIdxNewForOld;  
	int nNumVert = 0;

	//Delete vertexes from the array
	for(int i = 0; i < m_nVertSize; i++){
		VertexSimple* pVrt = &m_pVerts[i];
		if(!pVrt->m_bIsActive){
			arrIdxNewForOld.push_back(-1); 
			continue;
		}else{
			arrIdxNewForOld.push_back(nNumVert);
			pVrt->m_nIdx = nNumVert;
			pArrVrtBuf[nNumVert] = *pVrt;
			nNumVert++;
		}
	}

	m_pVerts = NULL;
	m_nMaxVertSize = m_nVertSize = nNumVert;
	m_pVerts = new VertexSimple[m_nMaxVertSize];
	for(int i = 0; i < m_nVertSize; i++){
		VertexSimple* pVrt = &pArrVrtBuf[i];
		m_pVerts[i] = *pVrt;
	}

	for(long i = 0; i < m_nTrianglesSize; i++){
		TriangleSimple* pTri = &m_pTriangles[i];
		if(!pTri->m_bIsActive) continue;
		pTri->m_nV1 =  arrIdxNewForOld[pTri->m_nV1];
		pTri->m_nV2 =  arrIdxNewForOld[pTri->m_nV2];
		pTri->m_nV3 =  arrIdxNewForOld[pTri->m_nV3];
		if((pTri->m_nV1 > m_nVertSize)||(pTri->m_nV2 > m_nVertSize)||(pTri->m_nV3 > m_nVertSize)){
			int h = 0;
		}
	}
}

//Calculates costs for every vertex and create (fill) m_VertexSort of vertexes 
void Simplified_Mesh::CalcCollapsesCostForAll()
{
	float dGlMinCost = FLT_MAX;
	float dGlMaxCost = FLT_MIN;

	Vector3D vmin(FLT_MAX,FLT_MAX,FLT_MAX);
	Vector3D vmax(FLT_MIN,FLT_MIN,FLT_MIN);

	for(int iv = 0; iv < m_nVertSize; iv++){
		VertexSimple* pVrt = &m_pVerts[iv];

		vmax.x=max(vmax.x,pVrt->m_ptPos.x);
		vmax.y=max(vmax.y,pVrt->m_ptPos.y);
		vmax.z=max(vmax.z,pVrt->m_ptPos.z);

		vmin.x=min(vmin.x,pVrt->m_ptPos.x);
		vmin.y=min(vmin.y,pVrt->m_ptPos.y);
		vmin.z=min(vmin.z,pVrt->m_ptPos.z);
	}

	vmax-=vmin;
	m_AABB_size2=3000*3000;//vmax.x*vmax.x;

	for(int iv = 0; iv < m_nVertSize; iv++){
		VertexSimple* pVrt = &m_pVerts[iv];

		float dMinCost = FLT_MAX;
		int nIdxToColps = -1;
		for(int in = 0; in < pVrt->m_NearVerts.size(); in++){
			int nCurVrt = pVrt->m_NearVerts[in];
			float dCost = GetCollapseCost(pVrt->m_nIdx, nCurVrt);
			if(dCost < dMinCost){
				dMinCost = dCost;
				nIdxToColps = nCurVrt;
			}
		}
		if(nIdxToColps != pVrt->m_nIdx){
			pVrt->m_dCollapseCost = dMinCost;
			pVrt->m_nCollapseDirection = nIdxToColps;
		}else{
			pVrt->m_dCollapseCost = FLT_MAX;
			pVrt->m_nCollapseDirection = pVrt->m_nIdx;
		}
		if(dMinCost < dGlMinCost)
			dGlMinCost = dMinCost;
		if(dMinCost > dGlMaxCost)
			dGlMaxCost = dMinCost;
	}	
	//m_VertexSort.set_range(dGlMinCost, (dGlMaxCost - dGlMinCost)*8, m_nVertSize*8 + 100);
	m_VertexSort.set_range(0,0.001, m_nVertSize*8 + 100);
	for(int i = 0; i < m_nVertSize; i++){
		VertexSimple* pVrt = &m_pVerts[i];
		m_VertexSort.add(*pVrt);
	}	

	m_VertexSort.check();

}
void Simplified_Mesh::SetVecNToTriangles()
{
	for(int i = 0; i < m_nTrianglesSize; i++){
		TriangleSimple* pTri = &m_pTriangles[i];
		VertexSimple* pVrt1 = &m_pVerts[pTri->m_nV1];
		VertexSimple* pVrt2 = &m_pVerts[pTri->m_nV2];
		VertexSimple* pVrt3 = &m_pVerts[pTri->m_nV3];
		Vector3D vecRes;
		Vector3D vec1 = pVrt2->m_ptPos; vec1 -= pVrt1->m_ptPos;
		Vector3D vec2 = pVrt3->m_ptPos; vec2 -= pVrt1->m_ptPos;
		vecRes.cross(vec1, vec2);  vecRes.normalize();
		pTri->m_vecN = vecRes;
	}
}
Simplified_Mesh::Simplified_Mesh(){
	SquaredDistConst=0.1;
	TriQualityConst=0.03;
    KeepNormalsCoef=50;
}
Simplified_Mesh::~Simplified_Mesh(){
	if(m_pVerts){
		delete[]m_pVerts;
		delete[]m_pTriangles;
	}
	m_pVerts=NULL;
	m_pTriangles=NULL;
}
using namespace Surface;

void SimplifySurface(SurfMesh &Mesh, float Degree) {
	cBitMap BoundMap;
	if(!Mesh.GatherVertsBoundaryStats(BoundMap)) return;

	Simplified_Mesh* SM=new Simplified_Mesh;
	SM->SquaredDistConst=9;//9;
	SM->TriQualityConst = cMath::Lerp(0.00075f, 0.0015f, Degree);
//	SM->TriQualityConst=0.0015;//0.00075;
    SM->KeepNormalsCoef=10*Degree;//50
	SM->SetMaxTriangles(Mesh.NTris());
	SM->SetMaxVert(Mesh.NVerts());

	for(int i=0;i < Mesh.NVerts(); i++){
		const SurfVertex &r = Mesh.VERTS[i];
		SurfMapper m=r.GetMapper(r.nFacture);
		DWORD C=m.CalcMappingCode(r);
		SM->AddVertex(Vector3D(r.x, r.y, r.z),r.nFacture+(C<<8)+C);
		if(BoundMap[i]) {
			SM->m_pVerts[i].m_bBoundary = true;
		}
	}

	int i0, i1, i2;
	for(int nTri = 0; nTri < Mesh.NTris(); nTri++) {
		Mesh.GetTri(nTri, i0, i1, i2);
		SM->AddTriangle(i0, i1, i2);
	}

	SM->Simplify();

	DWORD* Encode=new DWORD[Mesh.NVerts()];
	memset(Encode,0xFF,Mesh.NVerts()*4);
	int p=0;
	for(int i=0;i<Mesh.NVerts();i++){
		if(SM->m_pVerts[i].m_bIsActive) {
			Encode[i]=p;
			Mesh.VERTS[p++] = Mesh.VERTS[i];
		}
	}
	Mesh.VERTS.RemoveAt(p,i-p);
	int ni=Mesh.TRIS.Count();
	Mesh.TRIS.Clear();
	p=0;
	int t=0;
	int actualTris=0;
	for(int i=0;i<ni;i+=3,t++){
		TriangleSimple* TS=SM->m_pTriangles+t;
		if(TS->m_bIsActive){			
			int V1=Encode[TS->m_nV1];
			int V2=Encode[TS->m_nV2];
			int V3=Encode[TS->m_nV3];
            if(V1>=0 && V2>=0 && V3>=0)Mesh.AddTri(V1,V2,V3);
			actualTris++;
		}
	}
    delete(SM);
	delete[] Encode;
}