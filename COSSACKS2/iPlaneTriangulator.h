#include "..\stdheader.h"
#include "UnicHash.h"
#pragma once
#define plt_size 16
class iPlaneTriangulator{
	//intermidiate params:
    uni_hash<DWORD,DWORD,plt_size>	        halfedges;
	uni_hash<DWORD,DWORD,plt_size>          triangles_near_edges;
	uni_hash<DWORD,DWORD,plt_size>	        edges_near_verts;	
	//input params:	
	uni_hash<BasicEdge,DWORDS2,plt_size>	edges;
	uni_hash<DWORD,DWORDS2,plt_size>        breaks;
	uni_hash<DWORD,DWORDS2,plt_size>		remained;
	uni_hash<DWORD,DWORDS3,plt_size>		tri_presence;
	uni_hash<DWORD,DWORDS2,plt_size>		edge_presence;

    DWORD& get_remained(int V1,int V2);
	bool  check_remained(int V1,int V2);
	bool  CheckIntersectionWith(DWORD V1,DWORD V2);	
	bool  ChechIfNoInsidePoints(DWORD V1,DWORD V2,DWORD V3);
	double GetMinimalAngleBetweenEdges(DWORD VC,DWORD V1,DWORD V2);    
public:
	//input:
	Matrix3D			WorldTM;
	uni_hash<iVector>	points;//points are on XY palne
	//output:
	uni_hash<BasicTriangle> triangles;
	void AddVertex		(int U,int V,DWORD ID);
	void AddEdge		(DWORD V1,DWORD V2,bool Edge=false);
	//void RemoveEdge		(DWORD V1,DWORD V2);
	DWORD AddTriangle	(DWORD V1,DWORD V2,DWORD V3);
    void Triangulate	();
	void CopyTo			(BasicMesh& bm);
    void                reset();
	void				testwrite(BasicMesh& m,const char* ply_name){
        BasicMesh* bm=new BasicMesh;
		for(int i=0;i<points.size();i++){
			bm->AddVertex(*m.Verts[*points.key(i)],*points.key(i));
		}
		for(int i=0;i<triangles.size();i++){
			BasicTriangle* bt=triangles.elem(i);
			bm->AddTriangle(bt->nIndA,bt->nIndB,bt->nIndC,0);
		}
		bm->WriteToPly(ply_name);
		delete(bm);
	}
	void				testwrite_edges(BasicMesh& m,const char* ply_name){
        BasicMesh* bm=new BasicMesh;
		for(int i=0;i<points.size();i++){
			bm->AddVertex(*m.Verts[*points.key(i)],*points.key(i));
		}
		for(int i=0;i<edges.size();i++){
			BasicEdge* be=edges.elem(i);
			bm->AddTriangle(be->Vert1,be->Vert1,be->Vert2,0);
		}
		bm->WriteToPly(ply_name);
		delete(bm);
	}
};
