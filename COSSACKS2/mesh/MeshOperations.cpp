#include "..\stdheader.h"

#ifdef COSSACKS2

#include "unihash.h"
#include "..\comms\comms.h"
#include "iVector.h"
#include "MeshOperations.h"
#include "iPlaneTriangulator.h"

#undef assert
#define assert(x)

#else //!COSSACKS2

#include "stdafx.h"
#include "unihash.h"
#include "dVector.h"
#include "iVector.h"
#include "MeshOperations.h"
#include "PlaneTriangulator.h"
#include "iPlaneTriangulator.h"

#endif //COSSACKS2 

//#define DEBUG_OUTPUT

DWORD BasicMesh::AddVertex(Vector3D V,DWORD ID,DWORD MtlID,float u,float v){	
	BasicVertex BV;
	BV.Pos=dVector(V.x,V.y,V.z);
	BV.N=dVector(0,0,0);
	BV.MtlID=MtlID;
	BV.u=u;
	BV.v=v;
	return AddVertex(BV,ID);
}
DWORD BasicMesh::AddVertex(dVector V,DWORD ID,DWORD MtlID,float u,float v){
	BasicVertex BV;
	BV.Pos=V;
	BV.N=dVector(0,0,0);
	BV.MtlID=MtlID;
	BV.u=u;
	BV.v=v;
	return AddVertex(BV,ID);    
}
DWORD BasicMesh::AddVertex(BasicVertex& B,DWORD ID){
    if(Verts.get(ID))return ID;
	if(ID==0xFFFFFFFF)ID=Verts.get_free_idx();    
	Verts.add(ID,B);
	return ID;
}
dVector BasicTriangle::CalcN(BasicMesh& m){
    BasicVertex* v1=m.Verts[nIndA];
    BasicVertex* v2=m.Verts[nIndB];
    BasicVertex* v3=m.Verts[nIndC];    
    dVector cv1=v1->Pos;
    dVector cv2=v2->Pos;
    dVector cv3=v3->Pos;
    cv2-=cv1;
    cv3-=cv1;	
    dVector C;
    C.Cross(cv2,cv3);
    C.Normalize();
    return C;
}
dVector BasicTriangle::CalcD(BasicMesh& m){
    BasicVertex* v1=m.Verts[nIndA];
    BasicVertex* v2=m.Verts[nIndB];
    BasicVertex* v3=m.Verts[nIndC];    
    dVector cv1=v1->Pos;
    dVector cv2=v2->Pos;
    dVector cv3=v3->Pos;
    cv2-=cv1;
    cv3-=cv1;	
    dVector C;
    C.Cross(cv2,cv3);    
    return C;
}
double BasicTriangle::getQ(BasicMesh& m,int V1,int V2){
    if(V1!=-1){
        int V3=-1;
        if( (V1==nIndA && V2==nIndB) || (V1==nIndB && V2==nIndA) )V3=nIndC;
        if( (V1==nIndB && V2==nIndC) || (V1==nIndC && V2==nIndB) )V3=nIndA;
        if( (V1==nIndC && V2==nIndA) || (V1==nIndA && V2==nIndC) )V3=nIndB;
        BasicVertex* v1=m.Verts[V1];
        BasicVertex* v2=m.Verts[V2];
        BasicVertex* v3=m.Verts[V3];        
        dVector cv1=v1->Pos;
        dVector cv2=v2->Pos;
        dVector cv3=v3->Pos;
        cv1-=cv3;
        cv2-=cv3;
        cv1.Normalize();
        cv2.Normalize();
        cv3.Cross(cv1,cv2);
        return cv3.Length();//1.00001-cv1.Dot(cv2);
    }else{
        BasicVertex* v1=m.Verts[nIndA];
        BasicVertex* v2=m.Verts[nIndB];
        BasicVertex* v3=m.Verts[nIndC];
        //if(!(v1&&v2&&v3) )return 1;
        dVector cv1=v1->Pos;
        dVector cv2=v2->Pos;
        dVector cv3=v3->Pos;
        dVector cv4=cv3;
        cv2-=cv1;   
        cv3-=cv1;
        cv4-=cv2;
        cv2.Normalize();
        cv3.Normalize();
        cv4.Normalize();
        double d1=1.00001-cv2.Dot(cv3);
        double d2=1.00001-cv3.Dot(cv4);
        double d3=1.00001+cv4.Dot(cv2);
        return tmin(d1,d2,d3);
    }
}
int BasicMesh::AddTriangle(DWORD V1,DWORD V2,DWORD V3,DWORD MTLID){
    BasicTriangle BT;
    BT.MtlID=MTLID;
	BT.nIndA=V1;
	BT.nIndB=V2;
	BT.nIndC=V3;
	BT.N=BT.CalcN(*this);
	DWORD ID=TriangleMaxID++;//Triangles.get_free_idx();
	Triangles.add_i(ID,BT);
	DWORD tid = ID;
	DWORD E1=AddEdge(V1,V2);
	DWORD E2=AddEdge(V2,V3);
	DWORD E3=AddEdge(V3,V1);

    EdgesNearFace.add(tid,E1);
	EdgesNearFace.add(tid,E2);
	EdgesNearFace.add(tid,E3);

	VertsNearVerts.add_uniq(V1,V2);
	VertsNearVerts.add_uniq(V1,V3);
	VertsNearVerts.add_uniq(V2,V1);
	VertsNearVerts.add_uniq(V2,V3);
	VertsNearVerts.add_uniq(V3,V1);
	VertsNearVerts.add_uniq(V3,V2);

	FacesNearVerts.add_uniq(V1,tid);
    FacesNearVerts.add_uniq(V2,tid);
    FacesNearVerts.add_uniq(V3,tid);

	EdgesNearVerts.add_uniq(V1,E1);
	EdgesNearVerts.add_uniq(V2,E1);

    EdgesNearVerts.add_uniq(V2,E2);
    EdgesNearVerts.add_uniq(V3,E2);

    EdgesNearVerts.add_uniq(V3,E3);
    EdgesNearVerts.add_uniq(V1,E3);

	FacesNearEdge.add_uniq(E1,tid);
	FacesNearEdge.add_uniq(E2,tid);
	FacesNearEdge.add_uniq(E3,tid);

	DWORD* F1=FacesNearEdge.get(E1,0);
	DWORD* F2=FacesNearEdge.get(E1,1);
	if(F1 && F2){
		FacesNearFace.add_uniq(*F1,*F2);
		FacesNearFace.add_uniq(*F2,*F1);
	}
	F1=FacesNearEdge.get(E2,0);
	F2=FacesNearEdge.get(E2,1);
	if(F1 && F2){
		FacesNearFace.add_uniq(*F1,*F2);
		FacesNearFace.add_uniq(*F2,*F1);
	}
	F1=FacesNearEdge.get(E3,0);
	F2=FacesNearEdge.get(E3,1);
	if(F1 && F2){
		FacesNearFace.add_uniq(*F1,*F2);
		FacesNearFace.add_uniq(*F2,*F1);
	}
    TriPresence.add(DWORDS3(V1,V2,V3),_DWORD(1));
	return ID;
}
int BasicMesh::AddEdge(int V1,int V2){
	BasicEdge BE;
	BE.Vert1=V1;
	BE.Vert2=V2;
    int id = Edges.add_once_i(DWORDS2(V1,V2),BE);
	return id;
}
void BasicMesh::RemoveVertex(DWORD ID){
    hash_context h;
    VertsNearVerts.start_scan(ID,h);
    int si=0;
    while(DWORD* vv=VertsNearVerts.next(h)){
        VertsNearVerts.del_elm(*vv,ID);
    }
    Verts.del(ID);
	VertsNearVerts.del(ID);
	FacesNearVerts.start_scan(ID,h);
	while(DWORD* t=FacesNearVerts.next(h))RemoveTriangle(*t);
	FacesNearVerts.del(ID);
	EdgesNearVerts.start_scan(ID,h);
	while(DWORD* e=EdgesNearVerts.next(h)){
        BasicEdge* be=Edges.elem(*e);		
        if(be){
    		Edges.del(DWORDS2(be->Vert1,be->Vert2));
		    FacesNearEdge.del(*e);
        }
    }
	EdgesNearVerts.del(ID);
}
void BasicMesh::RemoveTriangle(DWORD ID){
    BasicTriangle* bt=Triangles.get(ID);
    //if(bt){
        FacesNearVerts.del_elm(bt->nIndA,ID);
        FacesNearVerts.del_elm(bt->nIndB,ID);
        FacesNearVerts.del_elm(bt->nIndC,ID);
        hash_context ct;
        FacesNearFace.start_scan(ID,ct);
        while(DWORD* f=FacesNearFace.next(ct)){
            FacesNearFace.del_elm(*f,ID);
        }
        //EdgesNearFace.self_test();
        EdgesNearFace.start_scan(ID,ct);
        while(DWORD* ed=EdgesNearFace.next(ct)){
            FacesNearEdge.del_elm(*ed,ID);
        }
        //EdgesNearFace.self_test();
        TriPresence.del(DWORDS3(bt->nIndA,bt->nIndB,bt->nIndC));
        Triangles.del(ID);
        FacesNearFace.del(ID);
        EdgesNearFace.del(ID);
        //EdgesNearFace.self_test();
    //}
}
void BasicMesh::RemoveDeadVerts(BasicMesh& dest){
	dest.reset();
	hash_context h;
	Verts.start_scan(h);
	int maxv=0;
	int idx;
	while((idx=Verts.next_i(h))!=-1){		
		DWORD* k=Verts.key(idx);
		if((*k)>maxv)maxv=*k;
	}
	int* enc=new int[maxv+1];
	Verts.start_scan(h);
	maxv=0;
	while((idx=Verts.next_i(h))!=-1){		
		BasicVertex* bv=Verts.elem(idx);
		DWORD* k=Verts.key(idx);
		dest.AddVertex(*bv,maxv);
		enc[*k]=maxv;
		maxv++;
	}
	Triangles.start_scan(h);
	while(BasicTriangle* bt=Triangles.next(h)){
        dest.AddTriangle(enc[bt->nIndA],enc[bt->nIndB],enc[bt->nIndC],bt->MtlID);
	}
	delete[]enc;
}
void BasicMesh::CopyTo(BasicMesh& dest,DWORD StartV){
    if(StartV==0)dest.reset();
	hash_context h;
	Verts.start_scan(h);
	int idx;
	while((idx=Verts.next_i(h))!=-1){
        BasicVertex* bv=Verts.elem(idx);
		DWORD* k=Verts.key(idx);
		dest.AddVertex(*bv,(*k)+StartV);
	}
	Triangles.start_scan(h);
	while(BasicTriangle* bt=Triangles.next(h)){
        dest.AddTriangle(bt->nIndA+StartV,bt->nIndB+StartV,bt->nIndC+StartV,bt->MtlID);
	}
}
void BasicMesh::shift(dVector D){
	hash_context ct;
	Verts.start_scan(ct);
	while(BasicVertex* bv=Verts.next(ct)){
		bv->Pos+=D;
	}
}
void BasicMesh::scale(double s){
	hash_context ct;
	Verts.start_scan(ct);
	while(BasicVertex* bv=Verts.next(ct)){
		bv->Pos*=s;
	}
}
bool BasicMesh::CheckMesh(){
    //return true;
    int n_flip_edge=0;
    int n_empty_edge=0;
    int n_double_edge=0;
    uni_hash<DWORD,bi_DWORD> edg;
    hash_context ct;
    Triangles.start_scan(ct);
    while(BasicTriangle* bt=Triangles.next(ct)){
        bi_DWORD D1 (bt->nIndA,bt->nIndB);
        bi_DWORD D2 (bt->nIndB,bt->nIndC);
        bi_DWORD D3 (bt->nIndC,bt->nIndA);

        bi_DWORD D1i(bt->nIndB,bt->nIndA);
        bi_DWORD D2i(bt->nIndC,bt->nIndB);
        bi_DWORD D3i(bt->nIndA,bt->nIndC);

        DWORD* DD=edg.get(D1);
        if(DD)n_flip_edge++;
        edg.add(D1,_DWORD(1));

        DD=edg.get(D2);
        if(DD)n_flip_edge++;
        edg.add(D2,_DWORD(1));

        DD=edg.get(D3);
        if(DD)n_flip_edge++;
        edg.add(D3,_DWORD(1));
    }
    Triangles.start_scan(ct);
    while(BasicTriangle* bt=Triangles.next(ct)){
        bi_DWORD D1 (bt->nIndA,bt->nIndB);
        bi_DWORD D2 (bt->nIndB,bt->nIndC);
        bi_DWORD D3 (bt->nIndC,bt->nIndA);

        bi_DWORD D1i(bt->nIndB,bt->nIndA);
        bi_DWORD D2i(bt->nIndC,bt->nIndB);
        bi_DWORD D3i(bt->nIndA,bt->nIndC);

        DWORD* DD=edg.get(D1i);
        if(!DD)n_empty_edge++;           

        DD=edg.get(D2i);
        if(!DD)n_empty_edge++;        

        DD=edg.get(D3i);
        if(!DD)n_empty_edge++;    

        if(edg.size(D1)>1)n_double_edge++;
        if(edg.size(D2)>1)n_double_edge++;
        if(edg.size(D2)>1)n_double_edge++;        
    }
    Verts.start_scan(ct);

    while(DWORD* v=Verts.next_key(ct)){
        hash_context hc;
        FacesNearVerts.start_scan(*v,hc);
        int summ=0;
        while(DWORD* F=FacesNearVerts.next(hc)){
            BasicTriangle* bt=Triangles[*F];
            DWORD v1=bt->nIndA;
            DWORD v2=bt->nIndB;
            DWORD v3=bt->nIndC;
            if(v2==*v){
                swap(v1,v2);
                swap(v2,v3);
            }else
            if(v3==*v){
                swap(v2,v3);
                swap(v1,v2);                
            }
            summ+=v2*v2*v2-v3*v3*v3;            
        }
        if(summ!=0){
            int fff=0;
        }
    }
    if(n_empty_edge || n_flip_edge){
        int xxx=0;
    }
    Verts.self_test();
    Triangles.self_test();
    Edges.self_test();
    VertsNearVerts.self_test();
    FacesNearVerts.self_test();
    EdgesNearVerts.self_test();
    FacesNearEdge.self_test();
    EdgesNearFace.self_test();
    FacesNearFace.self_test();
    TriPresence.self_test();
    return n_double_edge==0 && n_flip_edge==0;
}

bool BasicMesh::collapse(int from,int to){
    if(Edges.get(DWORDS2(from,to))){        
        hash_context ct;
        FacesNearVerts.start_scan(from,ct);
        std::vector<BasicTriangle> Tris;
        int TD1=-1;
        int TD2=-1;
        while(DWORD* T=FacesNearVerts.next(ct)){
            BasicTriangle* bt=Triangles[*T];            
            if(bt->nIndA==to || bt->nIndB==to || bt->nIndC==to){
                if(TD1==-1)TD1=*T;
                else TD2=*T;                    
            }else{
                BasicTriangle TD=*bt;
                if(TD.nIndA==from)TD.nIndA=to;
                if(TD.nIndB==from)TD.nIndB=to;
                if(TD.nIndC==from)TD.nIndC=to;
                Tris.push_back(TD);
            }
        }
        assert(TD1!=-1 && TD2!=-1);
        if(TD1!=-1)RemoveTriangle(TD1);
        if(TD2!=-1)RemoveTriangle(TD2);
        //WriteToPly("!!before.ply");
        RemoveVertex(from);
        for(int i=0;i<Tris.size();i++){
            BasicTriangle& bt=Tris[i];
            AddTriangle(bt.nIndA,bt.nIndB,bt.nIndC,bt.MtlID);
        }
        //WriteToPly("!after.ply");        
        return true;
    }else return false;
}
double BasicMesh::check_collapse(int from,int to){
    if(Edges.get(DWORDS2(from,to))){
        //topology checking
        hash_context ct1;
        VertsNearVerts.start_scan(from,ct1);
        int nc=0;
        if(VertsNearVerts.size(from)==3 && VertsNearVerts.size(to)==3)return 1e20;
        /*
        while(DWORD* V1=VertsNearVerts.next(ct1)){
            hash_context ct2;
            VertsNearVerts.start_scan(to,ct2);
            while(DWORD* V2=VertsNearVerts.next(ct2)){
                if( (*V1) == (*V2) ){
                    if(!TriPresence.get(DWORDS3(*V1,from,to)))return 1e20;
                }
            }
        }
        */
        //checking for collapse of normals
        DWORD edge=Edges.get_i(DWORDS2(from,to));
        if(edge!=-1){
            BasicVertex* bv1=Verts[from];
            BasicVertex* bv2=Verts[to];
            double d=bv1->Pos.DistTo(bv2->Pos);
            scan_key(FacesNearEdge,edge,DWORD* f){
                BasicTriangle* bt=Triangles[*f];
                assert(bt);
                if(bt){
                    DWORD* T1=FacesNearFace.get(*f,0);
                    DWORD* T2=FacesNearFace.get(*f,1);
                    DWORD* T3=FacesNearFace.get(*f,2);
                    if(T1 && T2){
                        BasicTriangle* BT1=Triangles[*T1];
                        BasicTriangle* BT2=Triangles[*T2];
                        BasicTriangle* BT3=NULL;
                        if(T3)BT3=Triangles[*T3];
                        if(BT1 && BT2 && BT3){
                            if(!BT3->HaveEdge(from,to)){
                                if(BT2->HaveEdge(from,to))swap(BT2,BT3);
                                if(BT1->HaveEdge(from,to))swap(BT1,BT3);
                            }
                        }
                        double dp=BT1->N.Dot(BT2->N);
                        if(dp<0.9){
                            return 1e20;
                        }
                    }                    
                }
            }scan_end;
        }
        //normale flip checking
        double mindot=1;
        double minq_before=1;
        double minq_after=1;
        FacesNearVerts.start_scan(from,ct1);
        while(DWORD* V1=FacesNearVerts.next(ct1)){
            BasicTriangle* bt=Triangles[*V1];
            if(bt){
                BasicTriangle b1=*bt;                
                if(!(b1.nIndA==to || b1.nIndB==to || b1.nIndC==to) ){                    
                    if(b1.nIndA==from)b1.nIndA=to;
                    if(b1.nIndB==from)b1.nIndB=to;
                    if(b1.nIndC==from)b1.nIndC=to;
                    double q1=b1.getQ(*this);
                    if(q1<minq_after)minq_after=q1;
                    dVector C=b1.CalcN(*this);
                    double d=C.Dot(bt->N);                
                    if(d<0)return 1e20;//flip occured
                    if(d<mindot)mindot=d;
                }else{
                    double q0=bt->getQ(*this,from,to);
                    if(q0<minq_before)minq_before=q0;
                }
            }
        }
        double qq=minq_before*minq_before;//*minq_before*100/minq_after;
        return (1.1-mindot)*qq*qq;
    }else{
        return 1e20;
    }
}
void BasicMesh::invertNormals(){
	hash_context ct;
	Triangles.start_scan(ct);
	while(BasicTriangle* bt=Triangles.next(ct)){
		swap(bt->nIndA,bt->nIndB);
		bt->N=-bt->N;
	}
}
void BasicMesh::randshift(double Amplitude){
	hash_context ct;
	Verts.start_scan(ct);
	while(BasicVertex* bv=Verts.next(ct)){
		dVector D((rand()-16384)*Amplitude/16384,(rand()-16384)*Amplitude/16384,(rand()-16384)*Amplitude/16384);
		bv->Pos+=D;
	}    
}
dVector BasicMesh::size(){
	hash_context ct;
	Verts.start_scan(ct);
	dVector _min(1e30,1e30,1e30);
	dVector _max(-1e30,-1e30,-1e30);
	while(BasicVertex* bv=Verts.next(ct)){
        dVector C=bv->Pos;
		_min.x=min(C.x,_min.x);
		_min.y=min(C.x,_min.y);
		_min.z=min(C.x,_min.z);
		_max.x=max(C.x,_max.x);
		_max.y=max(C.x,_max.y);
		_max.z=max(C.x,_max.z);
	}
	_max-=_min;
	return _max;
}
double  BasicMesh::maxsize(){
	dVector C=size();
	return tmax(C.x,C.y,C.z);
}
void BasicMesh::ReadFromPly(const char* name){
	reset();
	int temp,nv,nt,v1,v2,v3,i;
	float x,y,z;
	char tems[256];
	FILE* f=fopen(name,"r");
	if(f){
		fscanf(f,"%s%s%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s%d%s%s%s%s%s%s",tems,tems,tems,tems,tems,tems,&nv,tems,tems,tems,tems,tems,tems,tems,tems,tems,tems,tems,&nt,tems,tems,tems,tems,tems,tems);		
		for(i=0;i<nv;i++){
			fscanf(f,"%f%f%f",&x,&y,&z);
			AddVertex(Vector3D(x,y,z));			
		}
		for(i=0;i<nt;i++){
			fscanf(f,"%d%d%d%d",&temp,&v1,&v2,&v3);
			AddTriangle(v1,v2,v3,0);
		}
		fclose(f);
	}	
}

void BasicMesh::WriteToPly(const char* name){
	FILE* f=fopen(name,"w");	
	if(f){
        BasicMesh* bm=new BasicMesh;
        RemoveDeadVerts(*bm);
		int nv=bm->Verts.size();
		int nf=bm->Triangles.size();
		fprintf(f,"ply\nformat ascii 1.0\nelement vertex %d\nproperty float32 x\nproperty float32 y\nproperty float32 z\nelement face %d\nproperty list uint8 int32 vertex_indices\nend_header\n",nv,nf);		
		for(int i=0;i<nv;i++){
			BasicVertex* bv=bm->Verts[i];
			assert(bv);
			fprintf(f,"%f %f %f\n",bv->Pos.x,bv->Pos.y,bv->Pos.z);
		}
		for(int i=0;i<nf;i++){
			BasicTriangle* bt=bm->Triangles[i];
			assert(bt);
			fprintf(f,"3 %d %d %d\n",bt->nIndA,bt->nIndB,bt->nIndC);
		}
		fclose(f);		
        delete(bm);
	}
}
void BasicMesh::reset(){
	Verts.reset();
	Triangles.reset();
	Edges.reset();
	VertsNearVerts.reset();
	FacesNearVerts.reset();
	EdgesNearVerts.reset();
	FacesNearEdge.reset();
	EdgesNearFace.reset();
	FacesNearFace.reset();
	TriangleMaxID=0;
    VertexMaxID=0;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
const double Epsilon = 0.000001;
double SectionTri(const dVector &RayOrig, const dVector &RayDir, const dVector &t0, const dVector &t1, const dVector &t2) {	
	dVector e1 = t1 - t0;
	dVector e2 = t2 - t0;
	dVector p = RayDir.Cross(e2);
	double d = e1.Dot(p);
	dVector to = RayOrig - t0;
	double id = 1.0 / d;
	dVector q = to.Cross(e1);
	if(d > Epsilon) {
		double u = to.Dot(p);
		if(u < 0.0 || u > d) return -1;
		double v = RayDir.Dot(q);
		if(v < 0.0 || u + v > d) return -1;		
	} else if(d < -Epsilon) {
		double u = to.Dot(p);
		if(u > 0.0 || u < d) return -1;
		double v = RayDir.Dot(q);
		if(v > 0.0 || u + v < d) return -1;
	} else return -1;
	return e2.Dot(q) * id;
}

//complexity - power 3;
double _isLastU;
double _isLastV;
bool _isCriticalSituation=false;
double iSectionTri(const iVector &RayOrig, const iVector &RayDir, const iVector &t0, const iVector &t1, const iVector &t2) {	
	iVector e1 = t1 - t0;
	iVector e2 = t2 - t0;
	iVector64 p;
	p.Cross64(RayDir,e2);
	__int64 d = dot64(e1,p);
	iVector to = RayOrig - t0;
	iVector64 q;
	q.Cross64(to,e1);
	__int64 dd=dot64(e2,q);
	if(d > 0) {
		__int64 u = dot64(to,p);
		if(u < 0 || u > d) return -1;
		__int64 v = dot64(RayDir,q);
		if(v < 0.0 || u + v > d) return -1;				
		_isLastU = double(u)/double(d);
		_isLastV = double(v)/double(d);
        if(dd==0 || dd==d || u==0 || u==d ||v==0 || v==d || u+v==d){
            _isCriticalSituation=true;
        }
		return double(dd)/double(d);		
	} else if(d < 0) {
		__int64 u = dot64(to,p);
		if(u > 0 || u < d) return -1;
		__int64 v = dot64(RayDir,q);
		if(v > 0.0 || u + v < d) return -1;
		_isLastU = double(u)/double(d);
		_isLastV = double(v)/double(d);
        if(dd==0 || dd==d || u==0 || u==d ||v==0 || v==d || u+v==d){
            _isCriticalSituation=true;
        }
		return double(dd)/double(d);		
	} else{		        
		return -1;
	}	
}
double SectionTri2(const dVector &RayOrig, const dVector &RayDir, const dVector &t0, const dVector &t1, const dVector &t2) {	
	dVector e1 = t1 - t0;
	dVector e2 = t2 - t0;
	dVector p = RayDir.Cross(e2);
	double d = e1.Dot(p);
	dVector to = RayOrig - t0;
	double id = 1.0 / d;
	dVector q = to.Cross(e1);
	if(abs(d) > Epsilon) {
		double u = to.Dot(p)/d;
		if(u < 0.0 || u > 1) return -1;
		double v = RayDir.Dot(q)/d;
		if(v < 0.0 || u + v > 1) return -1;	
		assert(u>0.001 && v>0.001 && u+v<0.999);
	}else return -1;
	return e2.Dot(q) * id;
}
class bool_struct;
class inters_point{//it can be intersection point or point in source meshes
public:
	bool   IsIntersectionPoint;
	//if IsIntersectionPoint==true next fields are valid
	DWORD  edge;
	DWORD  triangle;
	double sect;
	double _intersU;
	double _intersV;
	//ifIsIntersectionPoint==false next fields are valid
	int				VertInMesh;
	//common data
    dVector			pos;
	dVector			CalcPos(bool_struct* bs);
    iVector         CalcIUV(bool_struct& bs,int TriangleID);
};
class inters_edge{
public:
	//BasicMesh* TOwner;
	DWORD T1;
	DWORD T2;
	DWORD i_pt1;
	DWORD i_pt2;
	bool  IsBreakline;
};
struct polygon{
	bool			Visible;
	int				BaseTriangle;
	//BasicMesh*		SourceMesh;
};
//struct tri_near_ie{
//	int SurfIdx;
//	int TriIdx;
//	int EdgeIdx;
//};
class bool_struct{
public:
	BasicMesh Summ;
	BasicMesh Res;
	int nedges1;
	int nedges2;
	int nvert1;
	int nvert2;
	int ntri1;
	int ntri2;
	BasicMesh* m1;
	BasicMesh* m2;
	//BasicMesh* dest;
    bool sign1;
	bool sign2;
    bool do_boolean();
	uni_hash<DWORD,bi_DWORD> ip_by_edges;
	uni_hash<DWORD> ip_by_edges_only;
	//uni_hash<DWORD,bi_DWORD> ip_by_edges_m2;	
	uni_hash<inters_point>			ips;//intersection points
	uni_hash<inters_edge>			iedges;//edges between intersection points
	uni_hash<DWORD>					iedges_near_ips;
	uni_hash<DWORD>					iedges_on_tris;	
	uni_hash<polygon>				polygons;
	uni_hash<DWORD>					polygon_edges;
	uni_hash<DWORD>					polygons_near_polygon;
	uni_hash<DWORD>					final_tris_near_breaking_edges;
	uni_hash<DWORD>					final_face_state;// 0/1 - surface index; 0/2 - visible/invisible
	uni_hash<dVector,bi_DWORD>		face_direction;
	uni_hash<DWORD,DWORDS2>			breaking_edges;
	uni_hash<bi_DWORD>				itriang;
	uni_hash<DWORD,tri_DWORD>		volume_edges;
	uni_hash<DWORD,tri_DWORD>		volume_tris;
	uni_hash<DWORD,bi_DWORD>		scanned;
	int								cell_size;
	void							add_volume_object(DWORD id,iVector& _min,iVector& _max,bool tri);
};
void bool_struct::add_volume_object(DWORD id,iVector& _min,iVector& _max,bool tri){
    int nx0=_min.x/cell_size;
	int nx1=_max.x/cell_size;
	int ny0=_min.y/cell_size;
	int ny1=_max.y/cell_size;
	int nz0=_min.z/cell_size;
	int nz1=_max.z/cell_size;
	for(int i=nx0;i<=nx1;i++){
		for(int j=nx0;j<=nx1;j++){
			for(int k=nx0;k<=nx1;k++){
                if(tri)volume_tris.add(tri_DWORD(i,j,k),id);
				else volume_edges.add(tri_DWORD(i,j,k),id);
			}
		}
	}
}
dVector inters_point::CalcPos(bool_struct* bs){
	if(IsIntersectionPoint){
		BasicEdge* be=bs->Summ.Edges.elem(edge);
        dVector V1=bs->Summ.Verts[be->Vert1]->Pos;
		dVector V2=bs->Summ.Verts[be->Vert2]->Pos;
		V1*=(1-sect);
		V2*=sect;
		V1+=V2;
		pos=V1;
		return V1;
	}else{
		pos=bs->Summ.Verts[VertInMesh]->Pos;
        return pos;
	}
}
const int i_scale = 100000;
iVector inters_point::CalcIUV(bool_struct& bs,int TriangleID){
    iVector iv(0,0,0);
    if(IsIntersectionPoint){
        if(TriangleID==triangle){
            iv.y=_intersU*i_scale;
            iv.x=_intersV*i_scale;            
        }else{
            BasicEdge* be=bs.Summ.Edges.elem(edge);
            BasicTriangle* bt=bs.Summ.Triangles.get(TriangleID);
            DWORDS3 D3(bt->nIndA,bt->nIndB,bt->nIndC);
            if(be->Vert1==D3.V1 && be->Vert2==D3.V3){//s=u
                iv.x=sect*i_scale;
            }else
            if(be->Vert1==D3.V3 && be->Vert2==D3.V1){//s=1-u
               iv.x=(1.0-sect)*i_scale;
            }else
            if(be->Vert1==D3.V1 && be->Vert2==D3.V2){//s=v
                iv.y=sect*i_scale;
            }else
            if(be->Vert1==D3.V2 && be->Vert2==D3.V1){//s=1-v
                iv.y=(1-sect)*i_scale;
            }else
            if(be->Vert1==D3.V3 && be->Vert2==D3.V2){//s=v,u+v=1
                iv.y=sect*i_scale;
                iv.x=i_scale-iv.y;
            }else
            if(be->Vert1==D3.V2 && be->Vert2==D3.V3){//s=u,u+v=1
                iv.x=sect*i_scale;
                iv.y=i_scale-iv.x;
            }else{
                assert(0);
            }
        }
        /*
        BasicTriangle* bt=bs.Summ.Triangles.get(TriangleID);
        DWORDS3 D3(bt->nIndA,bt->nIndB,bt->nIndC);
        dVector cv1=bs.Summ.Verts[D3.V1]->Pos;
        dVector cv2=bs.Summ.Verts[D3.V2]->Pos;
        dVector cv3=bs.Summ.Verts[D3.V3]->Pos;
        cv2-=cv1;
        cv3-=cv1;
        dVector cp=cv1+cv3*iv.x/i_scale+cv2*iv.y/i_scale;
        dVector cp2=CalcPos(&bs);
        int xxx=0;
        */
    }else{
        BasicTriangle* bt=bs.Summ.Triangles.get(TriangleID);
        DWORDS3 D3(bt->nIndA,bt->nIndB,bt->nIndC);
        if(VertInMesh==D3.V1){
            //zero
        }else
        if(VertInMesh==D3.V2){
            iv.y=i_scale;
        }else
        if(VertInMesh==D3.V3){
            iv.x=i_scale;
        }else{
            assert(0);
        }
    }
    return iv;
}
_inline bool CubesIntersects(iVector& p1min,iVector& p1max,iVector& p2min,iVector& p2max){
    iVector ic=(p1min+p1max-p2min-p2max);    
    iVector L=(p1max-p1min+p2max-p2min);
    return abs(ic.x)<=L.x && abs(ic.y)<=L.y && abs(ic.z)<=L.z;    
}
bool bool_struct::do_boolean(){
	int nvert1=m1->Verts.size();
	int nvert2=m2->Verts.size();
	int nedges1=m1->Edges.size();
	int nedges2=m2->Edges.size();
	int ntri1=m1->Triangles.size();
	int ntri2=m2->Triangles.size();
	//adiing meshes
	Summ.reset();
	m1->CopyTo(Summ);
	m2->CopyTo(Summ,nvert1);	
    int getNmtl(BasicMesh* m);
    int ss=getNmtl(&Summ);
	//normalizing
    double sz=Summ.maxsize();
    //Summ.scale(4096.0/sz);
    //Summ.scale(100);
    //collecting edges intersections	
	inters_point ip;
	ip.IsIntersectionPoint=false;
	//adding source point to the list of intersection points
	int nv=Summ.Verts.size();		
	for(int q=0;q<nv;q++){
        ip.VertInMesh=q;
		ip.CalcPos(this);
		ips.add(q,ip);
	}
	//intersecting edges with triangles
	ip.IsIntersectionPoint=true;	
	int nedges=Summ.Edges.size();
	int ntri=Summ.Triangles.size();
	iVector* ivs=new iVector[nv];

    iVector* minT=new iVector[ntri];
    iVector* maxT=new iVector[ntri];

    iVector* minE=new iVector[nedges];
    iVector* maxE=new iVector[nedges];   

	for(int i=0;i<nv;i++){
		dVector vv=Summ.Verts[i]->Pos;
		ivs[i]=iVector(floorf(vv.x+0.5),floorf(vv.y+0.5),floorf(vv.z+0.5));
	}
	iVector vmin(10000000,10000000,10000000);
	iVector vmax(-10000000,-10000000,-10000000);
    for(int e=0;e<nedges;e++){
        BasicEdge* be=Summ.Edges.elem(e);        
        iVector E1=ivs[be->Vert1];
        iVector E2=ivs[be->Vert2];

        minE[e].x=min(E1.x,E2.x);
        minE[e].y=min(E1.y,E2.y);
        minE[e].z=min(E1.z,E2.z);

        maxE[e].x=max(E1.x,E2.x);
        maxE[e].y=max(E1.y,E2.y);
        maxE[e].z=max(E1.z,E2.z);

		vmin.x=tmin(vmin.x,E1.x);
		vmin.y=tmin(vmin.y,E1.y);
		vmin.z=tmin(vmin.z,E1.z);

		vmax.x=tmax(vmax.x,E1.x);
		vmax.y=tmax(vmax.y,E1.y);
		vmax.z=tmax(vmax.z,E1.z);
    }
	int lx=vmax.x-vmin.x;
	int ly=vmax.y-vmin.y;
	int lz=vmax.z-vmin.z;
	int maxL=tmax(lx,ly,lz);

	cell_size=maxL/20;

    for(int t=0;t<ntri;t++){
        BasicTriangle* bt=Summ.Triangles[t];
        iVector T1=ivs[bt->nIndA];
        iVector T2=ivs[bt->nIndB];
        iVector T3=ivs[bt->nIndC];

        minT[t].x=tmin(T1.x,T2.x,T3.x);
        minT[t].y=tmin(T1.y,T2.y,T3.y);
        minT[t].z=tmin(T1.z,T2.z,T3.z);

        maxT[t].x=tmax(T1.x,T2.x,T3.x);
        maxT[t].y=tmax(T1.y,T2.y,T3.y);
        maxT[t].z=tmax(T1.z,T2.z,T3.z);

		//add_volume_object(t,minT[t],maxT[t],true);
    }	
	for(int e=0;e<nedges;e++){
		for(int t=0;t<ntri;t++){            
			if( (e<nedges1 && t>=ntri1) || (e>=nedges1 && t<ntri1) ){
                if(CubesIntersects(minE[e],maxE[e],minT[t],maxT[t])){
				    BasicEdge* be=Summ.Edges.elem(e);
				    BasicTriangle* bt=Summ.Triangles[t];				
				    iVector E1=ivs[be->Vert1];
				    iVector E2=ivs[be->Vert2];
				    DWORDS3 DD=DWORDS3(bt->nIndA,bt->nIndB,bt->nIndC);
				    iVector T1=ivs[DD.V1];
				    iVector T2=ivs[DD.V2];
				    iVector T3=ivs[DD.V3];
				    E2-=E1;
				    double s=iSectionTri(E1,E2,T1,T2,T3);
                    assert(!_isCriticalSituation);
                    if(_isCriticalSituation){
                        int xxx=0;
						_isCriticalSituation=false;
						delete[]ivs;
						delete[]minT;
						delete[]maxT;
						delete[]minE;
						delete[]maxE;
						return false;
                    }
				    if(s>0 && s<1){					
					    ip.edge=e;
					    ip.triangle=t;
					    ip.sect=s;
					    ip._intersU=_isLastU;
					    ip._intersV=_isLastV;
					    ip.CalcPos(this);
					    DWORD idx=ips.get_free_idx();
					    ips.add(idx,ip);
					    ip_by_edges.add(bi_DWORD(e,t),idx);
					    ip_by_edges_only.add(e,idx);
						hash_context h;
						Summ.FacesNearEdge.start_scan(e,h);
						while(DWORD* t2=Summ.FacesNearEdge.next(h)){
							itriang.add(itriang.get_free_idx(),bi_DWORD(*t2,t));
						}
				    }
			    }
            }
		}
	}	
	delete[]ivs;
	//creating sections list in triangles using obtained points
	ntri=ntri1+ntri2;
	hash_context h;
	itriang.start_scan(h);
	while(bi_DWORD* b=itriang.next(h)){
		int t1=b->V1;
		int t2=b->V2;	
        //checking triangles for intersecting each other
		DWORD pts[8];
		int npts=0;
		BasicTriangle* T1=Summ.Triangles[t1];
		BasicTriangle* T2=Summ.Triangles[t2];
		DWORD e11=Summ.Edges.get_i(DWORDS2(T1->nIndA,T1->nIndB));
		DWORD e12=Summ.Edges.get_i(DWORDS2(T1->nIndB,T1->nIndC));
		DWORD e13=Summ.Edges.get_i(DWORDS2(T1->nIndC,T1->nIndA));
		DWORD e21=Summ.Edges.get_i(DWORDS2(T2->nIndA,T2->nIndB));
		DWORD e22=Summ.Edges.get_i(DWORDS2(T2->nIndB,T2->nIndC));
		DWORD e23=Summ.Edges.get_i(DWORDS2(T2->nIndC,T2->nIndA));
		DWORD* p1=ip_by_edges.get(bi_DWORD(e11,t2));
		if(p1)pts[npts++]=*p1;
		p1=ip_by_edges.get(bi_DWORD(e12,t2));
		if(p1)pts[npts++]=*p1;
		p1=ip_by_edges.get(bi_DWORD(e13,t2));
		if(p1)pts[npts++]=*p1;
		p1=ip_by_edges.get(bi_DWORD(e21,t1));
		if(p1)pts[npts++]=*p1;
		p1=ip_by_edges.get(bi_DWORD(e22,t1));
		if(p1)pts[npts++]=*p1;
		p1=ip_by_edges.get(bi_DWORD(e23,t1));
		if(p1)pts[npts++]=*p1;
        assert(npts==2 || npts==0);
		//if(npts!=2 && npts!=0)return false;
		if(npts==2){
			inters_edge ie;
			ie.IsBreakline=true;
			ie.i_pt1=pts[0];
			ie.i_pt2=pts[1];
			ie.T1=t1;
			ie.T2=t2;
			DWORD id=iedges.get_free_idx();
			iedges.add(id,ie);
			iedges_near_ips.add_uniq(pts[0],id);
			iedges_near_ips.add_uniq(pts[1],id);
			iedges_on_tris.add_uniq(t1,id);
			iedges_on_tris.add_uniq(t2,id);
		}
	}
	//creating edges list from old triangles edges
    int ne=Summ.Edges.size();
	for(int i=0;i<ne;i++){
		BasicEdge* be=Summ.Edges.elem(i);
		hash_context ct;
		double sc[128];
		int    pt[128];
		int np = 2;
        sc[0]=0;
		pt[0]=be->Vert1;
		sc[1]=1;
		pt[1]=be->Vert2;
        ip_by_edges_only.start_scan(i,ct);		
		while(DWORD* p=ip_by_edges_only.next(ct)){
            inters_point* ip=ips[*p];
			assert(ip->edge==i);
			sc[np]=ip->sect;
			pt[np++]=*p;
		}
		//sorting
        bool change=false;
		do{
            change=false;
			for(int q=1;q<np;q++){
				if(sc[q]<sc[q-1]){
					swap(sc[q],sc[q-1]);
					swap(pt[q],pt[q-1]);
					change=true;
				}
			}
		}while(change);
		//adding edges
		for(int q=1;q<np;q++){
			inters_edge ie;
			ie.IsBreakline=false;
			ie.i_pt1=pt[q-1];
			ie.i_pt2=pt[q];
			DWORD* t1=Summ.FacesNearEdge.get(i,0);
			DWORD* t2=Summ.FacesNearEdge.get(i,1);            
			if(t1)ie.T1=*t1;
			if(t2)ie.T2=*t2;
			int edid=iedges.get_free_idx();
			iedges.add_i(edid,ie);
            if(t1)iedges_on_tris.add_uniq(*t1,_DWORD(edid));
			if(t2)iedges_on_tris.add_uniq(*t2,_DWORD(edid));
		}
	}
	//dividing all geometry on set of triangles	
	ntri=ntri1+ntri2;
	for(int i=0;i<ntri;i++){		
        hash_context ct;
		iedges_on_tris.start_scan(i,ct);
		BasicTriangle* bt=Summ.Triangles[i];
		dVector N=bt->N;
		static iPlaneTriangulator plt;
        plt.reset();        
		while(DWORD* ed=iedges_on_tris.next(ct)){
            inters_edge* ied=iedges.get(*ed);
			inters_point* ip1=ips.get(ied->i_pt1);
            inters_point* ip2=ips.get(ied->i_pt2);

            iVector iv1=ip1->CalcIUV(*this,i);
            iVector iv2=ip2->CalcIUV(*this,i);

            plt.AddVertex(iv1.x,iv1.y,ied->i_pt1);
            plt.AddVertex(iv2.x,iv2.y,ied->i_pt2);

			plt.AddEdge(ied->i_pt1,ied->i_pt2,!ied->IsBreakline);
		}
		
		int nv=plt.points.size();
		assert(nv);
		for(int j=0;j<nv;j++){
			DWORD* ID=plt.points.key(j);
			inters_point* ip=ips[*ID];
			Res.AddVertex(ip->pos,*ID);			
		}

#ifdef DEBUG_OUTPUT
		if(nv>3){
			char cc[32];
			sprintf(cc,"temp_ply\\!%04de.ply",i);
			plt.testwrite_edges(Res,cc);
		}
#endif //DEBUG_OUTPUT

		plt.Triangulate();

        int vv1=bt->nIndA;
        int vv2=bt->nIndB;
        int vv3=bt->nIndC;
        int nswap=0;
        for(int q=0;q<2;q++){
            if(vv2<vv1){
                swap(vv1,vv2);
                nswap++;
            }
            if(vv3<vv2){
                swap(vv2,vv3);
                nswap++;
            }
        }

#ifdef DEBUG_OUTPUT        
		if(nv>3){
			char cc[32];
			sprintf(cc,"temp_ply\\!%04dr.ply",i);			
			plt.testwrite(Res,cc);
		}
#endif //DEBUG_OUTPUT

		int nt=plt.triangles.size();
		assert(nt);
		DWORD tris[128];
		for(int j=0;j<nt;j++){
			BasicTriangle* btn=plt.triangles.elem(j);
            if(nswap&1){
                tris[j]=Res.AddTriangle(btn->nIndA,btn->nIndC,btn->nIndB,bt->MtlID);			
            }else{
                tris[j]=Res.AddTriangle(btn->nIndA,btn->nIndB,btn->nIndC,bt->MtlID);			
            }
            BasicTriangle* bt2=Res.Triangles.get(tris[j]);
            double ddt=bt->N.Dot(bt2->N);
            assert(ddt>0.95);
            bt2->N=bt->N;
        }
		iedges_on_tris.start_scan(i,ct);
		while(DWORD* ed=iedges_on_tris.next(ct)){
			inters_edge* ied=iedges.get(*ed);
			if(ied->IsBreakline){
				int v1=ied->i_pt1;
				int v2=ied->i_pt2;
                int v3=-1;
                int v4=-1;
                int t3=-1;
                int t4=-1;
                for(int q=0;q<nt;q++){
                    BasicTriangle* bt=Res.Triangles[tris[q]];
                    DWORD vv1=-1;
                    DWORD vv2=-1;
                    DWORD vv3=-1;
                    if( (bt->nIndA==v1 && bt->nIndB==v2)
                        ||(bt->nIndB==v1 && bt->nIndA==v2)){
                            vv1=bt->nIndA;vv2=bt->nIndB;vv3=bt->nIndC;
                    }
                    if( (bt->nIndB==v1 && bt->nIndC==v2)
                        ||(bt->nIndC==v1 && bt->nIndB==v2)){
                            vv1=bt->nIndB;vv2=bt->nIndC;vv3=bt->nIndA;
                    }
                    if( (bt->nIndC==v1 && bt->nIndA==v2)
                        ||(bt->nIndA==v1 && bt->nIndC==v2)){
                            vv1=bt->nIndC;vv2=bt->nIndA;vv3=bt->nIndB;
                    }
                    if(vv1!=-1){
                        assert(v3==-1 || v4==-1);
                        if(v3==-1){
                            v3=vv3;
                            t3=tris[q];
                        }else{ 
                            v4=vv3;
                            t4=tris[q];
                        }
                    }
                }
                assert(v3!=-1 && v4!=-1);
				if(v3!=-1 && v4!=-1){
					final_tris_near_breaking_edges.add(*ed,_DWORD(t3));
					final_tris_near_breaking_edges.add(*ed,_DWORD(t4));
					DWORD state=i>=ntri1?1:0;
					final_face_state.add_once(t3,state);
					final_face_state.add_once(t4,state);
					dVector CV1=ips.get(v1)->pos;
					dVector CV2=ips.get(v2)->pos;
					dVector CV3=ips.get(v3)->pos;
					dVector CV4=ips.get(v4)->pos;
					CV2-=CV1;
					CV4-=CV3;
					CV2.Normalize();
					CV4.Normalize();
					dVector CC;
					CC.Cross(bt->N,CV2);
					double dp=CC.Dot(CV4);
					if(dp<0)CC.Invert();

					face_direction.add_once(bi_DWORD(t4,*ed),CC);
					CC.Invert();
					face_direction.add_once(bi_DWORD(t3,*ed),CC);
					breaking_edges.add_uniq(DWORDS2(vv1,vv2),_DWORD(1));
				}
			}
		}
	}
	//and at least boolean breaking!!!
    int nr=getNmtl(&Res);
	hash_context sc;
#ifdef DEBUG_OUTPUT
	//debug
	iedges.start_scan(sc);    
	BasicMesh m3;
	while(inters_edge* ie=iedges.next(sc))if(ie->IsBreakline){
		if(!m3.Verts.get(ie->i_pt1)){
			m3.AddVertex(ips[ie->i_pt1]->pos,ie->i_pt1);
		}
		if(!m3.Verts.get(ie->i_pt2)){
			m3.AddVertex(ips[ie->i_pt2]->pos,ie->i_pt2);
		}
	}
	iedges.start_scan(sc);    
	while(inters_edge* ie=iedges.next(sc)){
		int ed=Res.Edges.get_i(DWORDS2(ie->i_pt1,ie->i_pt2));
        assert(i!=-1);
		int nf=Res.FacesNearEdge.size(ed);
		if(ie->IsBreakline){
			assert(nf==4);
			m3.AddTriangle(ie->i_pt1,ie->i_pt1,ie->i_pt2);
		}else{
			assert(nf<=2);
		}
	}
	m3.WriteToPly("!iedges.ply");	
#endif //DEBUG_OUTPUT

	iedges.start_scan(sc);
	int p;
	//1)boolean breaking edges
	while((p=iedges.next_i(sc))!=-1){
		DWORD tris[8];
		DWORD tstate[8];
		dVector TN[8];
		dVector VDir[8];			
		int ntris=0;
        inters_edge* ie=iedges.elem(p);
		DWORD idx=*iedges.key(p);
		if(ie->IsBreakline){			
			hash_context ct2;
			final_tris_near_breaking_edges.start_scan(idx,ct2);			
			while(DWORD* t=final_tris_near_breaking_edges.next(ct2)){
				DWORD tr=*t;
				tris[ntris]=tr;
                BasicTriangle* bt=Res.Triangles.get(tr);
                TN[ntris]=bt->N;
				tstate[ntris]=*final_face_state[tr];
				VDir[ntris++]=*face_direction[bi_DWORD(tr,idx)];
			}			
		}
		assert(ntris==4 || ntris==0);
		//if(ntris==4){
        int ninv=0;
		for(int i=0;i<ntris;i++){
			for(int j=0;j<ntris;j++)if(i!=j){
				int st1=tstate[i]&1;
				int st2=tstate[j]&1;
				if(st1!=st2){
					double dotp=TN[i].Dot(VDir[j]);
					bool dp=dotp>0;
                    if( (dp==sign1 && st2==1) || (dp==sign2 && st2==0) ){
                        ninv++;                        
                        tstate[j]|=2;//core of boolean checking!!!					
                    }else{
                        tstate[j]|=4;
                    }
				}
			}
		}
        assert(ntris==0 || (ntris==4 && ninv==4));
		for(int i=0;i<ntris;i++){
			final_face_state.add_once(tris[i],tstate[i]);
		}
	}
	//2)iterative breaking connected pieces (marking faces to delete)
    int nc=0;
	bool change=false;	
	do{
		change=false;
		Res.Edges.start_scan(sc);  
		int ed;
		while((ed=Res.Edges.next_i(sc))!=-1){
			BasicEdge* be=Res.Edges.elem(ed);
			inters_edge* ie=iedges.get(DWORDS2(be->Vert1,be->Vert2));
			if(!(ie && ie->IsBreakline)){				
				assert(i!=-1);
				int nf=Res.FacesNearEdge.size(ed);
				//assert(nf<=2);
				if(nf==2){
					DWORD f1=*Res.FacesNearEdge.get(ed,0);
					DWORD f2=*Res.FacesNearEdge.get(ed,1);
					DWORD* st1=final_face_state.get(f1);
					if(!st1)st1=final_face_state.add(f1,_DWORD(0));
					DWORD* st2=final_face_state.get(f2);
					if(!st2)st2=final_face_state.add(f2,_DWORD(0));
					DWORD s1=*st1;
					DWORD s2=*st2;
					if( ((*st1)&4) && !((*st2)&2) )*st2=*st1;
					if( ((*st1)&2) && !((*st2)&4) )*st2=*st1;
					if( ((*st2)&4) && !((*st1)&2) )*st1=*st2;
					if( ((*st2)&2) && !((*st1)&4) )*st1=*st2;
					if(s1!=*st1 || s2!=*st2)change=true;
				}
			}
		}
	}while(change && ++nc<500);	
    
	//3) deleting makred faces
	hash_context ct;
	final_face_state.start_scan(ct);
	int n1=final_face_state.size();
    int n2=Res.Triangles.size();
	int n3=iedges.size();
	int n4=Res.Edges.size();
	assert(n1==n2);
	while((p=final_face_state.next_i(ct))!=-1){
		DWORD* key=final_face_state.key(p);
		DWORD* state=final_face_state.elem(p);
        DWORD S=*state;
        if(S & 2){
            Res.RemoveTriangle(*key);            
        }
	}
    //removing disconnected vertices
    int n=Res.Verts.size();
    for(DWORD p=0;p<n;p++){
        DWORD* v=Res.Verts.key(p);
        if(Res.FacesNearVerts.size(*v)==0){            
            Res.RemoveVertex(*v);                        
        }
    }
	//ohhhhhh. at least......
    assert(Res.CheckMesh());
    int nr2=getNmtl(&Res);
    //stop,stop,stop... you should check for looooooooong triangles and KILL them  }:-[[[
    uni_hash<DWORD,DWORDS2> new_edges;     
    iedges.start_scan(ct);
    while(inters_edge* ie=iedges.next(ct)){
        if(ie->IsBreakline){
            new_edges.add(DWORDS2(ie->i_pt1,ie->i_pt2),_DWORD(1));
        }
    }
	
    bool Change;
    int nkill=0;
    do{
        Change=false;
        double best=0.0003;//angular threshold for killing
        DWORD from = 0xFFFFFFFF;
        DWORD to   = 0xFFFFFFFF;
        int ne=new_edges.size();
        new_edges.start_scan(ct);
        while((p=new_edges.next_i(ct))!=-1){
            DWORDS2 dd=*new_edges.key(p);
            double w=Res.check_collapse(dd.V1,dd.V2);
            if(w<best){
                best=w;
                from=dd.V1;
                to=dd.V2;
            }
            w=Res.check_collapse(dd.V2,dd.V1);
            if(w<best){
                best=w;
                from=dd.V2;
                to=dd.V1;
            }
        }
        if(from!=-1){            
            Res.collapse(from,to);            
            //Res.CheckMesh();            
            int vp=-1;
            DWORDS2 ft(from,to);
            new_edges.del(ft);

            hash_context ct;
            new_edges.start_scan(ct);

            while(DWORDS2* d=new_edges.next_key(ct)){                
                if(*d==ft){
                    assert(0);
                }else
                if(d->V1==from){
                    new_edges.del(*d);
                    DWORDS2 DD=*d;
                    DD.V1=to;
                    if(DD.V1!=DD.V2){
                        new_edges.add(DD,_DWORD(1));
                        new_edges.start_scan(ct);
                    }
                }else
                if(d->V2==from){
                    new_edges.del(*d);
                    DWORDS2 DD=*d;
                    DD.V2=to;
                    if(DD.V1!=DD.V2){
                        new_edges.add(DD,_DWORD(1));
                        new_edges.start_scan(ct);
                    }
                }
            }
            Change=true;
            nkill++;
            //debug: dumping edges
            /*
            BasicMesh* bm=new BasicMesh;
            Res.CopyTo(*bm);
            hash_context hh;
            bm->Triangles.start_scan(hh);            
            uni_hash<DWORD,DWORDS2> new_edges2;
            int nn=2000;
            int lv1=-1;
            int lv2=-1;
            hash_context hct;
            new_edges.start_scan(hct);
            while(DWORDS2* D=new_edges.next_key(hct)){
                new_edges2.add(*D,_DWORD(1));
                lv1=D->V1;
                lv2=D->V2;
                if((nn--)==0)break;
            }            
            while(DWORD* k=bm->Triangles.next_key(hh)){
                BasicTriangle* bt=bm->Triangles[*k];
                DWORDS2 D1(bt->nIndA,bt->nIndB);
                DWORDS2 D2(bt->nIndB,bt->nIndC);
                DWORDS2 D3(bt->nIndC,bt->nIndA);
                if(!(new_edges2.get(D1)||new_edges2.get(D2)||new_edges2.get(D3))){
                    bm->RemoveTriangle(*k);
                }
            }
            bm->WriteToPly("!!edge.ply");
            delete(bm);
            */
        }
    }while(Change);
    assert(Res.CheckMesh());
    int nr3=getNmtl(&Res);
	return true;
}
void BasicMesh::boolean(BasicMesh& m1,BasicMesh& m2,bool Subtract){
    bool_struct* bs=new bool_struct;
	bs->sign1=true;
	bs->sign2=true;
	bs->m1=&m1;
	bs->m2=&m2;		
	int na=0;
	while(++na<5 && !bs->do_boolean()){
		bs->Summ.reset();
		bs->ip_by_edges.reset();
		bs->ip_by_edges.reset();
		bs->ip_by_edges_only.reset();
		bs->ips.reset();
		bs->itriang.reset();
		bs->volume_edges.reset();
		bs->volume_tris.reset();
		bs->scanned.reset();
		m2.randshift(10);
	}
	bs->Res.RemoveDeadVerts(*this);
	delete(bs);
}
BasicMesh m1;
BasicMesh m2;
BasicMesh m3;
BasicMesh m4;
BasicMesh m5;
void TestBool(){	
    //m1.ReadFromPly("!c1.ply");    
	//m2.ReadFromPly("!c2.ply");
   
	//m1.ReadFromPly("111.ply");    
	//m2.ReadFromPly("111.ply");    
    //m2.shift(dVector(20.3156234,30.12346087,20.136583514));

    //m1.scale(256);
    //m2.scale(256);
    //m2.invertNormals();	
	//m1.randshift(1);
	//m2.randshift(1);
	//m3.boolean(m1,m2);
	//m3.invertNormals();
    //m3.CheckMesh();	
    //m3.WriteToPly("!b3.ply");
    
    //m3.RemoveDeadVerts(m5);
	
   
    //m5.ReadFromPly("!b3.ply");
    //m3.select_hard_edges(0.3);
	//m3.CheckMesh();
    //m3.soften_edges(m4,200);    
	//m4.CheckMesh();
    //m4.WriteToPly("!b4.ply");



	m1.ReadFromPly("!arg1.ply");
	m1.scale(128);
	m1.invertNormals();
	m2.ReadFromPly("!arg2.ply");
	m2.scale(128);
	m2.invertNormals();
	assert(m2.CheckMesh());
	m3.boolean(m1,m2);
	m3.invertNormals();
	m3.WriteToPly("!rt.ply");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//mesh edges smoothing

struct bmOneExtraVertex{
    bool Enabled;
    int SourceVertex;
	DWORD MtlID;
    int Edge1;
    int Edge2;
    int NearTris[32];
    int NNearTris;
    int Tri1;
    int Tri2;
    int DestIndex;
    dVector pos;
    void AddTri(int T);
    void AddV(bmOneExtraVertex& V);
};
void bmOneExtraVertex::AddTri(int T){
    for(int i=0;i<NNearTris;i++)if(NearTris[i]==T)return;
    assert(NNearTris<32);
    NearTris[NNearTris++]=T;	
}
void bmOneExtraVertex::AddV(bmOneExtraVertex& V){
    for(int i=0;i<V.NNearTris;i++)AddTri(V.NearTris[i]);
}
struct bmOneEdgeInfo{
    int T1;
    int T2;
    int V1;
    int V2;
    bool IsSharp;
};
/*
struct bmVertExInfo{
    int nevs;
    int evs[8];
};
*/
class bmMeshSplitter{
public:	
    std::vector<bmOneExtraVertex> evs;
    std::vector<bmOneEdgeInfo> edges;
    uni_hash<DWORD> exvinf;//extra verts near old verts	
    uni_hash<DWORD,bi_DWORD> ed_xist;
    BasicMesh*  SRC;
    BasicMesh*  DST;
    void		SoftenEdges(BasicMesh* _SRC,BasicMesh* _DST,float Thickness);
    //helpers:
    dVector 	GetTriTangent(int TriIdx,int EdgeIdx,float& MaxWidth);
    void		CalcSrcTriNormals();    
    void		CalcTrueVertex(int TriIdx,int& Vert);
    bool		CheckEdgeExists(int v1,int v2,BasicMesh* TM);
    void        addt(BasicTriangle& BT){
		if(BT.nIndA!=BT.nIndB && BT.nIndA!=BT.nIndC && BT.nIndB!=BT.nIndC){
            DST->AddTriangle(BT.nIndA,BT.nIndB,BT.nIndC,BT.MtlID);
			ed_xist.add(bi_DWORD(BT.nIndA,BT.nIndB),_DWORD(1));
			ed_xist.add(bi_DWORD(BT.nIndB,BT.nIndC),_DWORD(1));
			ed_xist.add(bi_DWORD(BT.nIndC,BT.nIndA),_DWORD(1));
		}
    }
    void		TestSV();
};
bool bmMeshSplitter::CheckEdgeExists(int v1,int v2,BasicMesh* TM){
    return ed_xist.get(bi_DWORD(v1,v2))!=NULL;    
}
void bmMeshSplitter::TestSV(){
    return;
    /*
    int* vv=new int[SRC->m_arrVerteces.size()];
    memset(vv,0,4*SRC->m_arrVerteces.size());
    for(int i=0;i<evs.size();i++)if(evs[i].Enabled){
        vv[evs[i].SourceVertex]++;
    }
    for(int i=0;i<SRC->m_arrVerteces.size();i++){
        assert(vv[i]>=2);
    }
    delete[]vv;
    */
}
void bmMeshSplitter::CalcTrueVertex(int TriIdx,int& Vert){	
	hash_context ct;
	exvinf.start_scan(Vert,ct);
	while(DWORD* evr=exvinf.next(ct)){
		bmOneExtraVertex& ev=evs[*evr];
		for(int j=0;j<ev.NNearTris;j++){
            if(ev.NearTris[j]==TriIdx){
                Vert=ev.DestIndex;
                return;
            }
        }
	}    
}
dVector bmMeshSplitter::GetTriTangent(int TriIdx,int EdgeIdx,float& MaxWidth){
    BasicTriangle& tri=*SRC->Triangles[TriIdx];
    dVector tn=tri.N;
    bmOneEdgeInfo& oei=edges[EdgeIdx];
    int ev1=oei.V1;
    int ev2=oei.V2;
    int v1,v2,v3;
    if((ev1==tri.nIndA && ev2==tri.nIndB) || (ev1==tri.nIndB && ev2==tri.nIndA)){
        v1=tri.nIndA;
        v2=tri.nIndB;
        v3=tri.nIndC;
    }
    if((ev1==tri.nIndB && ev2==tri.nIndC) || (ev1==tri.nIndC && ev2==tri.nIndB)){
        v1=tri.nIndB;
        v2=tri.nIndC;
        v3=tri.nIndA;
    }
    if((ev1==tri.nIndC && ev2==tri.nIndA) || (ev1==tri.nIndA && ev2==tri.nIndC)){
        v1=tri.nIndC;
        v2=tri.nIndA;
        v3=tri.nIndB;
    }
    dVector VB=SRC->Verts[v1]->Pos;
    dVector VC=SRC->Verts[v3]->Pos;
    VB-=SRC->Verts[v2]->Pos;
    VC-=SRC->Verts[v1]->Pos;
    VB.Normalize();
    dVector vr;
    vr.Cross(VB,tn);
    vr.Normalize();
    MaxWidth=fabs(vr.Dot(VC)/3);
    return vr;
}
//excuse for not using hash tables - bo oblom.
void bmMeshSplitter::SoftenEdges(BasicMesh* _SRC,BasicMesh* _DST,float Thickness){
    SRC=_SRC;
    DST=_DST;
    int nv=SRC->Verts.size();
    int nt=SRC->Triangles.size();
    //creating edges info
    hash_context hc;
	uni_hash<DWORD,DWORDS2> edges_near_biverts;
    SRC->Edges.start_scan(hc);
    int ed;
    while((ed=SRC->Edges.next_i(hc))!=-1){
        DWORDS2* DD=SRC->Edges.key(ed);
        bmOneEdgeInfo ei;
        ei.IsSharp=false;
        ei.V1=DD->V1;
        ei.V2=DD->V2;
        DWORD* t1=SRC->FacesNearEdge.get(ed,0);
        DWORD* t2=SRC->FacesNearEdge.get(ed,1);
		ei.T1=t1?*t1:-1;
        ei.T2=t2?*t2:-1;
        if(t1 && t2 && SRC->CheckIfVertexIsInside(ei.V1) && SRC->CheckIfVertexIsInside(ei.V2) && SRC->SelectedEdges.get(*DD)){
            ei.IsSharp=true;
        }
		edges_near_biverts.add(*DD,_DWORD(edges.size()));
		edges.push_back(ei);
    }
    //creating ExtraVertices
    SRC->Triangles.start_scan(hc);
    int i=0;
    while(DWORD* tr=SRC->Triangles.next_key(hc)){ 
        i=*tr;
        BasicTriangle& T=*SRC->Triangles[*tr];    
        int ed1=*edges_near_biverts[DWORDS2(T.nIndA,T.nIndB)];
        int ed2=*edges_near_biverts[DWORDS2(T.nIndB,T.nIndC)];
        int ed3=*edges_near_biverts[DWORDS2(T.nIndC,T.nIndA)];        
        bmOneExtraVertex EV;
        EV.SourceVertex=T.nIndA;
		EV.MtlID=SRC->Verts[T.nIndA]->MtlID;
        EV.Edge1=ed1;
        EV.Edge2=ed3;
        EV.Enabled=true;
        EV.Tri1=i;
        EV.Tri2=i;
        EV.NNearTris=0;
        EV.AddTri(i);			
        evs.push_back(EV);
        EV.SourceVertex=T.nIndB;
		EV.MtlID=SRC->Verts[T.nIndB]->MtlID;
        EV.Edge1=ed2;
        EV.Edge2=ed1;
        evs.push_back(EV);
        EV.SourceVertex=T.nIndC;
		EV.MtlID=SRC->Verts[T.nIndC]->MtlID;
        EV.Edge1=ed3;
        EV.Edge2=ed2;			
        evs.push_back(EV);      
    }
	uni_hash<DWORD> evs_near_verts;
	//filling evs_near_verts
	for(int i=0;i<evs.size();i++){
		evs_near_verts.add(evs[i].SourceVertex,_DWORD(i));        
	}
    //iterative collapsing extra vertices
    bool change;
    do{
        change=false;
        for(int i=0;i<edges.size();i++){
            bmOneEdgeInfo& ei=edges[i];
            if(!ei.IsSharp){
                //edge is not sharp - we shoud collapse extra vertices
                int vv[2]={ei.V1,ei.V2};
                for(int j=0;j<2;j++){
                    int v=vv[j];
                    bmOneExtraVertex* ev1=NULL;
                    bmOneExtraVertex* ev2=NULL;
					hash_context hc1;
					evs_near_verts.start_scan(v,hc1);
					while(DWORD* vi=evs_near_verts.next(hc1)){
						if(evs[*vi].Enabled){
							bmOneExtraVertex& ev=evs[*vi];
							if(ev.SourceVertex==v){
								if(ev.Edge1==i){
									if(ev1)ev2=&ev;
									else ev1=&ev;
								}
								if(ev.Edge2==i){
									if(ev1)ev2=&ev;
									else ev1=&ev;
								}
							}
						}
                    }
                    //assert(ev1 && ev2);
                    if(ev1 && ev2){
                        bmOneExtraVertex ev10=*ev1;
                        bmOneExtraVertex ev20=*ev2;
                        if(ev1->Edge1==ev2->Edge1){
                            ev1->Edge1=ev2->Edge2;							
                            ev1->Tri1=ev2->Tri2;
                        }else
                        if(ev1->Edge2==ev2->Edge1){
                            ev1->Edge2=ev2->Edge2;							
                            ev1->Tri2=ev2->Tri2;
                        }else
                        if(ev1->Edge1==ev2->Edge2){
                            ev1->Edge1=ev2->Edge1;
                            ev1->Tri1=ev2->Tri1;
                        }else
                        if(ev1->Edge2==ev2->Edge2){
                            ev1->Edge2=ev2->Edge1;
                            ev1->Tri2=ev2->Tri1;
                        }
                        ev1->AddV(*ev2);
                        ev2->Enabled=false;
                        if(ev1->Edge1==ev1->Edge2 && edges[ev1->Edge1].IsSharp==false)
                            ev1->Enabled=false;//total collapse of vertex
                        TestSV();
                        change=true;						
                    }					
                }
            }			
        }        
    }while(change);	
	//removing boundary verts
	for(int i=0;i<evs.size();i++)if(evs[i].Enabled){
		if(!SRC->CheckIfVertexIsInside(evs[i].SourceVertex)){
			evs[i].Enabled=false;
		}
	}
    //removing disabled extra vertices
    int nvr=0;
    for(int i=0;i<evs.size();i++){
        if(evs[i].Enabled)evs[nvr++]=evs[i];				
    }
    evs.resize(nvr);
    //creating extra info for basic verts
    for(int i=0;i<evs.size();i++){
        int v=evs[i].SourceVertex;
		exvinf.add(v,_DWORD(i));
    }        
    //creating destination vertices positions
    for(int i=0;i<evs.size();i++){
        bmOneExtraVertex& ev=evs[i];
        ev.DestIndex=-1;
        float th1,th2;
        dVector n1=GetTriTangent(ev.Tri1,ev.Edge1,th1);
        dVector n2=GetTriTangent(ev.Tri2,ev.Edge2,th2);
        double dotp=n1.Dot(n2);
        th1=min(th1,Thickness);
        th1=min(th1,th2);
        n1+=n2;
        //n1.Normalize();
        //n1*=th1*sqrt(2.0/(1.001+dotp));
        n1*=th1/2;
        ev.pos=SRC->Verts[ev.SourceVertex]->Pos;
        ev.pos+=n1;
    }
    //creating destination vertices table
    //basic vertices
    SRC->Verts.start_scan(hc);
    i=0;    
    while(DWORD* p=SRC->Verts.next_key(hc)){    
        BasicVertex tv;
		int ne=exvinf.size(*p);
        if(ne){
            int v1=*exvinf[*p];
            evs[v1].DestIndex=*p;
            tv.Pos=evs[v1].pos;
			tv.MtlID=evs[v1].MtlID;			
        }else{
            tv.Pos=SRC->Verts[*p]->Pos;
			tv.MtlID=SRC->Verts[*p]->MtlID;
        }
        DST->AddVertex(tv,*p);
        i++;        
    }
    //extra vertices
	hash_context h;
	SRC->Verts.start_scan(h);
    while(DWORD* key=SRC->Verts.next_key(h)){
		int n=exvinf.size(*key);        
        for(int j=1;j<n;j++){
            BasicVertex tv;
            int v1=*exvinf.get(*key,j);
            DWORD id=evs[v1].DestIndex=DST->Verts.get_free_idx();
            tv.Pos=evs[v1].pos;
			tv.MtlID=evs[v1].MtlID;
            DST->AddVertex(tv,id);
        }
    }
    //creating basic faces geometry
    SRC->Triangles.start_scan(hc);
    while(DWORD* k=SRC->Triangles.next_key(hc)){
        BasicTriangle* bt=SRC->Triangles[*k];
        BasicTriangle BT=*bt;
        CalcTrueVertex(*k,BT.nIndA);
        CalcTrueVertex(*k,BT.nIndB);
        CalcTrueVertex(*k,BT.nIndC);
        addt(BT);
    }    
    int getNmtl(BasicMesh* m);
    int nn0=getNmtl(SRC);
    int nn1=getNmtl(_DST);
    //creating rectangular patches on edges
    for(int i=0;i<edges.size();i++){
        bmOneEdgeInfo& ei=edges[i];
        if(ei.IsSharp){
            int V1=ei.V1;
            int V2=ei.V1;
            int V3=ei.V2;
            int V4=ei.V2;
            CalcTrueVertex(ei.T1,V1);
            CalcTrueVertex(ei.T2,V2);
            CalcTrueVertex(ei.T1,V3);
            CalcTrueVertex(ei.T2,V4);
            if(CheckEdgeExists(V1,V3,DST)){
                swap(V1,V3);
                swap(V2,V4);
            }
            BasicTriangle tri;
            tri.MtlID=0;
            tri.nIndA=V1;
            tri.nIndB=V3;
            tri.nIndC=V2;
            addt(tri);
            tri.nIndA=V4;
            tri.nIndB=V2;
            tri.nIndC=V3;
            addt(tri);
        }
    }
    int nn2=getNmtl(_DST);
    //creating triangular patches in branches intersections	

    SRC->Verts.start_scan(h);;
    while(DWORD* key=SRC->Verts.next_key(h)){
		int n=exvinf.size(*key);
        if(n>=3){//polygonal patch
            int verts[16];
            int nv=0;
            for(int j=0;j<n;j++){
                verts[nv++]=evs[*exvinf.get(*key,j)].DestIndex;
            }
            //sorting
            for(int j=0;j<nv;j++){
                for(int q=j+1;q<nv;q++){
                    int v1=verts[j];
                    int v2=verts[q];
                    if(CheckEdgeExists(v2,v1,DST)){
                        if(q>j+1){
                            swap(verts[j+1],verts[q]);
                            break;
                        }
                    }
                }
            }
            //triangulating
            for(int j=2;j<nv;j++){
                BasicTriangle tri;
                tri.nIndA=verts[0];
                tri.nIndB=verts[j-1];
                tri.nIndC=verts[j];
                addt(tri);
            }
        }
    }	    
    int nn3=getNmtl(_DST);
}
bool BasicMesh::CheckIfVertexIsInside(DWORD ID){
    hash_context hc;
    FacesNearVerts.start_scan(ID,hc);
    int summ=0;
    while(DWORD* F=FacesNearVerts.next(hc)){
        BasicTriangle* bt=Triangles[*F];
        DWORD v1=bt->nIndA;
        DWORD v2=bt->nIndB;
        DWORD v3=bt->nIndC;
        if(v2==ID){
            swap(v1,v2);
            swap(v2,v3);
        }else
        if(v3==ID){
            swap(v2,v3);
            swap(v1,v2);                
        }
        summ+=v2*v2*v2-v3*v3*v3;            
    }
    return summ==0;
}
void BasicMesh::soften_edges(BasicMesh& dest,float Width){
    bmMeshSplitter ms;
    ms.SoftenEdges(this,&dest,Width);
}
void BasicMesh::select_hard_edges(float Threshold){
    //walking through edges to find sharp edges
    hash_context hc;
    Edges.start_scan(hc);
    int p;
    while((p=Edges.next_i(hc))!=-1){
        DWORDS2* ed=Edges.key(p);
        DWORD* t1=FacesNearEdge.get(p,0);
        DWORD* t2=FacesNearEdge.get(p,1);
        if(t1&&t2){
            BasicTriangle* bt1=Triangles[*t1];
            BasicTriangle* bt2=Triangles[*t2];
            if(bt1 && bt2){
                if(bt1->N.Dot(bt2->N)<Threshold){
                    if(CheckIfVertexIsInside(ed->V1) && CheckIfVertexIsInside(ed->V2)){
                        SelectedEdges.add(*ed,_DWORD(p));
                    }
                }
            }
        }
    }    
}
struct nvVertex{
    dVector vsumm;
    int     nsumm;
    nvVertex(){
        vsumm=0;
        nsumm=0;
    }
};
void BasicMesh::CalcVertNormals(){
    uni_hash<dVector> N;
    scan_e(Verts,BasicVertex* v){
        v->N=dVector(0,0,0);
    }scan_end;
    scan(Triangles,BasicTriangle* bt,DWORD* key){
        dVector v=bt->CalcD(*this);
        double n=v.Length();
        v*=n;
        Verts[bt->nIndA]->N+=v;
        Verts[bt->nIndB]->N+=v;
        Verts[bt->nIndC]->N+=v;
    }scan_end;
    scan_e(Verts,BasicVertex* v){
        v->N.Normalize();
    }scan_end;
}
void BasicMesh::smart_smooth(int nsteps,double force,double dotp_threshold){
    std::vector<DWORD> sm_verts;    
    scan(Verts,BasicVertex* bv,DWORD* key){
        if(CheckIfVertexIsInside(*key)){
            //threshold checking
            bool doit=true;
            scan_key(FacesNearVerts,*key,DWORD* face1){
                scan_key(FacesNearVerts,*key,DWORD* face2){
                    BasicTriangle* bt1=Triangles[*face1];
                    BasicTriangle* bt2=Triangles[*face2];
                    if(bt1 && bt2){
                        double dp=bt1->N.Dot(bt2->N);
                        if(dp<dotp_threshold){
                            doit=false;
                            break;
                        }
                    }
                }scan_end;                                
            }scan_end;
            if(doit){
                sm_verts.push_back(*key);
            }
        }
    }scan_end;
    int nv=sm_verts.size();
    uni_hash<nvVertex> vSumm;        
    for(int i=0;i<nsteps;i++){
        CalcVertNormals();
        scan_e(vSumm,nvVertex* s){
            s->vsumm=dVector(0,0,0);
            s->nsumm=0;
        }scan_end;
        for(int j=0;j<nv;j++){
            scan_e(Triangles,BasicTriangle* bt){
                BasicVertex* v1=Verts[bt->nIndA];
                BasicVertex* v2=Verts[bt->nIndB];
                BasicVertex* v3=Verts[bt->nIndC];
                if(v1 && v2 && v3){
                    nvVertex* nv1=vSumm.get(bt->nIndA);
                    if(!nv1)nv1=vSumm.add(bt->nIndA,nvVertex());
                    nv1->vsumm+=v2->Pos;
                    nv1->vsumm+=v3->Pos;
                    nv1->nsumm+=2;

                    nvVertex* nv2=vSumm.get(bt->nIndB);
                    if(!nv2)nv2=vSumm.add(bt->nIndB,nvVertex());
                    nv2->vsumm+=v1->Pos;
                    nv2->vsumm+=v3->Pos;
                    nv2->nsumm+=2;

                    nvVertex* nv3=vSumm.get(bt->nIndC);
                    if(!nv3)nv3=vSumm.add(bt->nIndC,nvVertex());
                    nv3->vsumm+=v1->Pos;
                    nv3->vsumm+=v2->Pos;
                    nv3->nsumm+=2;
                }
            }scan_end;
            for(int i=0;i<nv;i++){
                DWORD vert=sm_verts[i];
                nvVertex* vv=vSumm[vert];                
                if(vv && vv->nsumm){
                    dVector v=vv->vsumm/vv->nsumm;
                    BasicVertex* bv=Verts[vert];
                    dVector cv=bv->Pos*(1-force)+v*force;
                    bv->Pos=cv+bv->N.Dot(bv->Pos-cv)*bv->N;
                }
            }
        }
    }
}
void BasicMesh::resolve_dist_threshold(){
    bool change=false;
    do{
        change=false;
        scan(Verts,BasicVertex* v1,DWORD* id){
            scan_key(VertsNearVerts,*id,DWORD* id2){
                BasicVertex* v2=Verts[*id2];
                iVector V1(v1->Pos.x,v1->Pos.y,v1->Pos.z);
                iVector V2(v2->Pos.x,v2->Pos.y,v2->Pos.z);
                if(V1.x==V2.x && V1.y==V2.y && V1.z==V2.z){                    
                    dVector dp=v2->Pos-v1->Pos;
                    dVector pc=(v2->Pos+v1->Pos)/2;
                    double n=dp.Length();
                    double n0=n;
                    if(n0>0.01){
                        if(n<0.99)n=1;
                        else n*=1.1;
                        dp*=n/n0/2;
                        v1->Pos=pc-dp;
                        v2->Pos=pc+dp;
                        change=true;
                    }                    
                }
            }scan_end;
        }scan_end;
    }while(change);
}