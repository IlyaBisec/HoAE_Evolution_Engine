#include "..\stdheader.h"

#ifdef COSSACKS2

#include "unichash.h"
#include "iVector.h"
#include "..\comms\comms.h"
#include "MeshOper.h"
#include "iPlaneTriangulator.h"

#else //!COSSACKS2

#include "stdafx.h"
#include "unihash.h"
#include "iVector.h"
#include "dVector.h"
#include "MeshOperations.h"
#include "iPlaneTriangulator.h"

#endif //COSSACKS2 
void iPlaneTriangulator::AddVertex(int U,int V,DWORD ID){
    points.add_once(ID,iVector(U,V,0));
}
void iPlaneTriangulator::AddEdge(DWORD V1,DWORD V2,bool Edge){	
	if(edges.get(DWORDS2(V1,V2)))return;
	BasicEdge BE;
	BE.Vert1=V1;
	BE.Vert2=V2;
    int id=edges.add_i(DWORDS2(V1,V2),BE);
	DWORD N=Edge?1:2;
	remained.add_once(DWORDS2(V1,V2),N);
    edges_near_verts.add(V1,_DWORD(id));
	edges_near_verts.add(V2,_DWORD(id));
	if(Edge)breaks.add(DWORDS2(V1,V2),_DWORD(1));
}
DWORD& iPlaneTriangulator::get_remained(int V1,int V2){
	DWORD* R=remained.get(DWORDS2(V1,V2));
	if(R)return *R;
	R=remained.add(DWORDS2(V1,V2),_DWORD(2));
	return *R;
}
bool iPlaneTriangulator::check_remained(int V1,int V2){
	DWORD* R=remained.get(DWORDS2(V1,V2));
	if(R)return *R!=0;
	else return true;
}
DWORD iPlaneTriangulator::AddTriangle(DWORD V1,DWORD V2,DWORD V3){
	BasicTriangle bt;
    bt.nIndA=V1;
	bt.nIndB=V2;
	bt.nIndC=V3;
	int id=triangles.add_i(triangles.get_free_idx(),bt);
	AddEdge(V1,V2,false);
	AddEdge(V2,V3,false);
	AddEdge(V3,V1,false);
	int ed1=edges.get_i(DWORDS2(V1,V2));
	int ed2=edges.get_i(DWORDS2(V2,V3));
	int ed3=edges.get_i(DWORDS2(V3,V1));
	if(ed1!=-1){
		triangles_near_edges.add_uniq(ed1,_DWORD(id));
		halfedges.add_uniq(ed1,_DWORD(ed1));
	}
	if(ed2!=-1){
		triangles_near_edges.add_uniq(ed2,_DWORD(id));
		halfedges.add_uniq(ed2,_DWORD(ed2));
	}
	if(ed3!=-1){
		triangles_near_edges.add_uniq(ed3,_DWORD(id));
		halfedges.add_uniq(ed3,_DWORD(ed3));
	}
	tri_presence.add(DWORDS3(V1,V2,V3),_DWORD(id));
	edge_presence.add_once(DWORDS2(V1,V2),_DWORD(ed1));
	edge_presence.add_once(DWORDS2(V2,V3),_DWORD(ed2));
	edge_presence.add_once(DWORDS2(V3,V1),_DWORD(ed3));
	DWORD& D1=get_remained(V1,V2);
	D1--;
	assert(D1>=0);
	DWORD& D2=get_remained(V2,V3);
	D2--;
	assert(D2>=0);
	DWORD& D3=get_remained(V3,V1);
	D3--;
	assert(D3>=0);
	return id;
}
//complexity - power2
bool precise_check_lines_intersection(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4){
	if( (x1==x3 && y1==y3) || (x1==x4 && y1==y4) || (x2==x3 && y2==y3) || (x2==x4 && y2==y4) )return false;
	int a=x2-x1;
	int b=x3-x4;
	int c=y2-y1;
	int d=y3-y4;
	int e=x3-x1;
	int f=y3-y1;
	__int64 det=__int64(a)*__int64(d)-__int64(b)*__int64(c);//pow 2
	if(det==0)return false;
	__int64 td=__int64(d)*__int64(e)-__int64(b)*__int64(f);
	if(td==0)return false;
	if(td==det)return false;
	if(det>0){
		if(td>det || td<0)return false;
	}else{
		if(td<det || td>0)return false;
	}	
	td=__int64(a)*__int64(f)-__int64(c)*__int64(e);
	if(det>0){
		if(td>det || td<0)return false;
	}else{
		if(td<det || td>0)return false;
	}	
	return true;
}
int get_isign(int dx,int dy,int x,int y){	
    __int64 v=__int64(dx)*__int64(y)-__int64(dy)*__int64(x);
	if(v<0)return -1;
	if(v>0)return 1;
	return 0;
}
bool precise_check_point_in_triangle(int x1,int y1,int x2,int y2,int x3,int y3,int x,int y){
    x2-=x1;
	y2-=y1;    
	x3-=x1;
	y3-=y1;    
	x-=x1;
	y-=y1;
	int s1=get_isign(x2,y2,x,y);
	int s2=get_isign(x3-x2,y3-y2,x-x2,y-y2);
	int s3=get_isign(-x3,-y3,x-x3,y-y3);
	return (s1>=0 && s2>=0 && s3>=0) || (s1<=0 && s2<=0 && s3<=0);
}
bool iPlaneTriangulator::CheckIntersectionWith(DWORD V1,DWORD V2){
    iVector cv1=*(points[V1]);
	iVector cv2=*(points[V2]);
    int ne=edges.size();
	for(int i=0;i<ne;i++){
        BasicEdge* be=edges.elem(i);
		if(be){
			iVector cv3=*(points[be->Vert1]);
			iVector cv4=*(points[be->Vert2]);
			if(precise_check_lines_intersection(cv1.x,cv1.y,cv2.x,cv2.y,cv3.x,cv3.y,cv4.x,cv4.y))return true;
		}
	}
	return false;
}
bool  iPlaneTriangulator::ChechIfNoInsidePoints(DWORD v1,DWORD v2,DWORD v3){
    iVector V1=*points[v1];
	iVector V2=*points[v2];
	iVector V3=*points[v3];
	int nv=points.size();
	for(int i=0;i<nv;i++){
		DWORD* ppt=points.key(i);
		if(ppt){
			DWORD p=*ppt;
			if(p!=v1 && p!=v2 && p!=v3){
				iVector V=*points[p];
				if(precise_check_point_in_triangle(V1.x,V1.y,V2.x,V2.y,V3.x,V3.y,V.x,V.y))return false;
			}
		}
	}
	return true;
}
dVector _i2c(iVector i){
	dVector cv(i.x,i.y,i.z);
	return cv;	
}
double iPlaneTriangulator::GetMinimalAngleBetweenEdges(DWORD VC,DWORD V1,DWORD V2){
    static iVector dv[64];
	static double  dvN[64];
	static DWORD   vi[64];
	int n=0;
    hash_context ct;
	edges_near_verts.start_scan(VC,ct);
	iVector vc=*points[VC];
	while(DWORD* pe=edges_near_verts.next(ct)){
		BasicEdge* be=edges.elem(*pe);
		if(be->Vert1==VC){
			vi[n]=be->Vert2;
			dv[n++]=((*points[be->Vert2])-vc);
		}else
		if(be->Vert2==VC){
			vi[n]=be->Vert1;
			dv[n++]=((*points[be->Vert1])-vc);
		}
	}
	//if(!edges.get(DWORDS2(V1,VC)))dv[n++]=(*points[V1])-VC;
	//if(!edges.get(DWORDS2(V2,VC)))dv[n++]=(*points[V2])-VC;
	iVector dv1=((*points[V1])-vc);
	iVector dv2=((*points[V2])-vc);

	double N1=sqrt(double(dv1.Norm2()));
	double N2=sqrt(double(dv2.Norm2()));

	for(int i=0;i<n;i++){
		dvN[i]=sqrt(double(dv[i].Norm2()));
	}
	assert(n<63);
	double minA=1;
	for(int i=0;i<n;i++){
		iVector VI=dv[i];
		if(vi[i]!=V1){			
			__int64 dot=__int64(VI.x)*__int64(dv1.x)+__int64(VI.y)*__int64(dv1.y);
			if(dot>0){
				__int64 ort=__int64(VI.x)*__int64(dv1.y)-__int64(VI.y)*__int64(dv1.x);
				if(ort==0)return ort;
				if(ort<0)ort=-ort;
				double dd=double(ort)/N1/dvN[i];
				if(dd<minA)minA=dd;
			}
		}
		if(vi[i]!=V2){
			__int64 dot=__int64(VI.x)*__int64(dv2.x)+__int64(VI.y)*__int64(dv2.y);
			if(dot>0){
				__int64 ort=__int64(VI.x)*__int64(dv2.y)-__int64(VI.y)*__int64(dv2.x);
				if(ort==0)return ort;
				if(ort<0)ort=-ort;
				double dd=double(ort)/N2/dvN[i];
				if(dd<minA)minA=dd;
			}
		}
	}
	return minA;
}
void iPlaneTriangulator::Triangulate(){
	if(points.size()==3){
		DWORD bV1=*points.key(0);
		DWORD bV2=*points.key(1);
		DWORD bV3=*points.key(2);
		iVector cv1=*points[bV1];
		iVector cv2=*points[bV2];
		iVector cv3=*points[bV3];
		cv2-=cv1;
		cv3-=cv1;	
		iVector64 iv;
		iv.Cross64(cv2,cv3);		
		if(iv.z<0)AddTriangle(bV1,bV2,bV3);
		else AddTriangle(bV2,bV1,bV3);
		return;
	}
    //moving all edges to halfedges
	int n=edges.size();
	for(int i=0;i<n;i++){		
        halfedges.add(i,_DWORD(i));
		BasicEdge* be=edges.elem(i);
		edge_presence.add_once(DWORDS2(be->Vert1,be->Vert2),_DWORD(i));
	}
	bool change=false;	
	do{
		change=false;
        float maxA=0;
		int bestEdge=-1;
		int bestPoint=-1;
		int bV1,bV2,bV3;		
		int np=points.size();        
		int nh=edges.size();
		for(int i=0;i<nh;i++){
			DWORD* he=halfedges.elem(i);
			if(he){
				bool havetri=false;
				DWORD edge=*he;
				BasicEdge* be=edges.elem(edge);
				DWORD V1=be->Vert1;
				DWORD V2=be->Vert2;
				bool isbreak=breaks.get(DWORDS2(V1,V2))!=NULL;
				for(int j=0;j<np;j++){
                    DWORD V3=*points.key(j);
					if(V1!=V2 && V2!=V3 && V1!=V3){
						if(check_remained(V1,V2) && check_remained(V2,V3) && check_remained(V3,V1)){
							if(!tri_presence.get(DWORDS3(V1,V2,V3))){
								if(false && edge_presence.get(DWORDS2(V1,V2)) && edge_presence.get(DWORDS2(V2,V3)) && edge_presence.get(DWORDS2(V3,V1))){
									bV1=V1;
									bV2=V2;
									bV3=V3;
									change=true;	
									maxA=100;
									havetri=true;
									break;
								}else{
									if(ChechIfNoInsidePoints(V1,V2,V3)){
										if(!(CheckIntersectionWith(V1,V3) || CheckIntersectionWith(V2,V3))){								
											double a1=GetMinimalAngleBetweenEdges(V1,V2,V3);
											double a2=GetMinimalAngleBetweenEdges(V2,V3,V1);
											double a3=GetMinimalAngleBetweenEdges(V3,V1,V2);
											if(a2<a1)a1=a2;
											if(a3<a1)a1=a3;
											if(a1>1e-6){
												if(!isbreak)a1+=5;
												havetri=true;
												if(a1>maxA){//we seek for triangle with maximal minimal angle
													bestEdge=edge;
													bestPoint=V3;
													bV1=V1;
													bV2=V2;
													bV3=V3;
													change=true;	
													maxA=a1;
												}
											}
										}
									}
								}
							}
						}
					}
				}
				//if(!havetri){
				//	halfedges.del(*he);
				//}
			}
		}
		if(change){
			//adding new triangle
			iVector cv1=*points[bV1];
			iVector cv2=*points[bV2];
			iVector cv3=*points[bV3];
			cv2-=cv1;
			cv3-=cv1;
			iVector64 iv;
			iv.Cross64(cv2,cv3);		
			if(iv.z<0)AddTriangle(bV1,bV2,bV3);
			else AddTriangle(bV2,bV1,bV3);
		}
	}while(change);
	//checking for remained - must be zero
	hash_context ct;
	remained.start_scan(ct);
    
	while(DWORD* D=remained.next(ct)){
		if(*D){
			static BasicMesh ibm;
			CopyTo(ibm);
			ibm.WriteToPly("!tr1.ply");
			//*D=0;
		}
	}
    
}
void iPlaneTriangulator::CopyTo(BasicMesh& bm){
	BasicMesh bm2;
	hash_context ct;
	points.start_scan(ct);
	int p;
	while((p=points.next_i(ct))!=-1){
        iVector* V=points.elem(p);
		DWORD ID=*points.key(p);
		bm2.AddVertex(_i2c(*V),ID);
	}
	triangles.start_scan(ct);
	while(BasicTriangle* bt=triangles.next(ct)){
        bm2.AddTriangle(bt->nIndA,bt->nIndB,bt->nIndC,bt->MtlID);
	}
	bm2.RemoveDeadVerts(bm);
}
void iPlaneTriangulator::reset(){
    halfedges.fast_reset();
    triangles_near_edges.fast_reset();
    edges_near_verts.fast_reset();
    edges.fast_reset();
    remained.fast_reset();
    tri_presence.fast_reset();
    edge_presence.fast_reset();
    points.fast_reset();
    triangles.fast_reset();
	breaks.fast_reset();
}
iPlaneTriangulator iPT;
BasicMesh ibm;
void iTestTriang(){	
	
	iPT.AddVertex(0 ,0, 0);
	iPT.AddVertex(-3,6, 11);
	iPT.AddVertex(9 ,0, 22);
	iPT.AddVertex(-1,2, 33);
	iPT.AddVertex(5 ,1, 44);
	iPT.AddVertex(1 ,4, 55);
	iPT.AddVertex(-1,4, 66);
	iPT.AddVertex(0 ,4, 77);
	iPT.AddVertex(1 ,2, 88);
	iPT.AddVertex(-1,3, 99);
	iPT.AddEdge(0,22);
	iPT.AddEdge(22,55);
	iPT.AddEdge(55,11);
	iPT.AddEdge(11,33);
	iPT.AddEdge(33,0);
	iPT.AddEdge(33,44);
	iPT.AddEdge(44,55);
	iPT.AddEdge(66,77);
	iPT.AddEdge(77,88);
	iPT.AddEdge(88,99);
	iPT.AddEdge(99,66);
	iPT.Triangulate();	
	iPT.CopyTo(ibm);
	ibm.WriteToPly("!tr.ply");
}