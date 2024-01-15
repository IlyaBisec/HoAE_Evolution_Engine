#include "common.h"
#include "PenShape.h"
#include "Tablet.h"
#include "UndoMaster.h"
#include "FreezeTool.h"

#define FMAX 100000000000 
float SubdivCoef=1.0f;

const int FreeTime = 240*60*1000;
//const int FreeTime = 10*1000;
//////////////
//MeshInCell//
//////////////

DWORD MixDWORD(DWORD C1,DWORD C2,int W1,int W2);

Vector3D Hermit(Vector3D V1,Vector3D N1,Vector3D V2,Vector3D N2,float s,Vector3D& N,float Coef=1){
	//if(N1.dot(N2)<0.7)return (V1+V2)*0.5;
    Vector3D Along=V2-V1;
	float l=Along.norm()*Coef;
	Along.normalize();	
	Vector3D bA;
	bA.cross(N1,Along);
	bA.normalize();
	Vector3D bB;
	bB.cross(N2,Along);
	bB.normalize();
	Vector3D t0;
    t0.cross(bA,N1);
	Vector3D t1;
	t1.cross(bB,N2);
    t0*=l;
	t1*=l;	
	float s2 = s*s;
	float s3 = s2*s;
	Vector3D P = (2 * s3 - 3 * s2 + 1) * V1 + (- 2 * s3 + 3 * s2) * V2 + (s3 - 2 * s2 + s) * t0 + (s3 - s2) * t1;
	Vector3D D = 6*(s2-s)*(V1-V2)+(3*s2-4*s+1)*t0+(3*s2-2*s)*t1;		
	D.normalize();
	N.cross(D,bA+bB);
	N.normalize();
	return P;
}

MeshInCell::MeshInCell(){    
}
MeshInCell::~MeshInCell(){    
}

////////////
//PolyMesh//
////////////

PolyMesh::PolyMesh(){
    CellSize=1;
    AABB.Set(FMAX,FMAX,FMAX,-FMAX,-FMAX,-FMAX);	
}
PolyMesh::~PolyMesh(){
    Clear();
}

DWORD PolyMesh::AddVertexPos(Vector3D V){
    VertexPos v;
    v.Pos=V;
    v.SelectionDegree=0;	
	v.SubdivLevel=0;	
	v.TempSelection=0;
	v.W=1;
    Verts.Add(v);
    AABB.Extend(V);    
    return Verts.GetAmount()-1;
}
DWORD PolyMesh::AddVertexN(Vector3D V){
    V.normalize();
    VertsN.Add(V);
    return VertsN.GetAmount()-1;
}
DWORD PolyMesh::AddVertexUV(VertexUV* V){
    VertsUV.Add(*V);
    return VertsUV.GetAmount()-1;
}
void PolyMesh::AddFaceVertex(DWORD FaceIndex,DWORD VertexUV_index){
    Faces.add(FaceIndex,VertexUV_index);
    VertexUV& uv=VertsUV[VertexUV_index];
    DWORD p=uv.PosIndex;    
	if(FaceIndex==FacesInfo.GetAmount()){
		FaceInfo fi;
		fi.SubdivLevel=0;
		FacesInfo.Add(fi);
	}
}
void PolyMesh::FinalizeFace(DWORD FaceID){    
    int vl=-1;
    scan_key(Faces,FaceID,DWORD* Vert){
        VertexUV* uv=&VertsUV[*Vert];        
        if(vl!=-1){
            FacesOnEdges.add(DWORDS2(vl,uv->PosIndex),FaceID);
        }
        vl=uv->PosIndex;
    }scan_end;
}
void PolyMesh::RemoveVertex(DWORD VertIndex){
}
void PolyMesh::RemoveFace(DWORD Face){    
	int vl=-1;
	scan_key(Faces,Face,DWORD* Vert){
		VertexUV* uv=&VertsUV[*Vert];        
		if(vl!=-1){
			FacesOnEdges.del(DWORDS2(vl,uv->PosIndex),Face);
		}
		vl=uv->PosIndex;
	}scan_end;
    Faces.del(Face);
}
void PolyMesh::Clear(){
    scan(Meshes,MeshInCell** M,tri_DWORD* K){
        try{
            delete(*M);
        }catch(...){
        }
    }scan_end;
	Meshes.reset();
    Verts.Clear();
    VertsUV.Clear();
	VertsN.Clear();
    Faces.reset();    
    SrcFaces.reset();
    FacesTreeL0.reset();
    FacesTreeL2.reset();
	ListL2.reset();
    AABB.Set(FLT_MAX,FLT_MAX,FLT_MAX,-FLT_MAX,-FLT_MAX,-FLT_MAX);
	DirtyFaces.reset();
	FacesOnEdges.reset();
	VertsInfluence.reset();
	FacesBB.reset();
	VertsInCells.reset();
	for(int i=0;i<FacesInfo.GetAmount();i++){
		if(FacesInfo[i].Points)delete[](FacesInfo[i].Points);
	}
	FacesInfo.Clear();
	SurfaceModificator::Backup.reset();
	FreeUndoHistory();	
	UndoMaster::RemoveAllFromDisk();
}

bool read_string(FILE* F,char* STR,int Max){
	STR[0]=0;
	int cc=0;	
	int nn=0;
	while(!(cc==0x0A||cc==EOF)){
		cc=fgetc(F);
		if(Max>2&&!(cc==0x0A||cc==EOF)){
			STR[nn]=cc;
			nn++;
			Max--;
		};
	};
	STR[nn]=0;
	return (cc!=EOF)||STR[0];	
};
bool GetTextureFromMtl(const char* MtlLibName,const char* MtlName,char* TexName){
	FILE* F=fopen(MtlLibName,"rb");
	if(F){
		fseek(F,0,SEEK_END);
		int sz=ftell(F);
        fseek(F,0,SEEK_SET);
		char* s=new char[sz+1];
        s[sz]=0;
        fread(s,sz,1,F);
		fclose(F);
		char mn[256];
		sprintf(mn,"newmtl %s",MtlName);
		char* s2=strstr(s,mn);
		if(s2){
			s2+=strlen(mn);
			s2=strstr(s2,"map_Kd ");
			if(s2){
				s2+=7;
				char* s3=strchr(s2,0x0D);
				if(!s3)s3=strchr(s2,0x0A);
				if(s3){
					int L=s3-s2;
					memcpy(TexName,s2,L);
					TexName[L]=0;
					return true;
				}
			}
		}
	}
	return false;
}
bool PolyMesh::ReadObj(const char* Name){        
	Clear();
    FILE* F=fopen(Name,"r");
    bool success=true;
	char mtl_name[256]="";
	TextureID=-1;
	bool NonTextured=false;
    if(F){
        char c[512];
        int n;
        do{
            n=fscanf(F,"%s",c);
            if(n>0){
                if(c[0]=='v' && c[1]==0){
                    Vector3D v;
                    n=fscanf(F,"%f%f%f",&v.x,&v.z,&v.y);
					//v.y=-v.y;
                    //v.x=-v.x;
                    if(n==3){
                        AddVertexPos(v);                        
                    }else{
                        success=false;
                        break;
                    }
                }else
                if(c[0]=='v' && c[1]=='t'){
                    Vector3D v;
                    n=fscanf(F,"%f%f",&v.x,&v.y);
                    if(n==2){
                        VertexUV uv;
                        uv.u=v.x;
                        uv.v=v.y;
                        uv.NIndex=-1;
                        uv.PosIndex=-1;
                        AddVertexUV(&uv);
                    }else{
                        success=false;
                        break;
                    }
                }else
                if(c[0]=='v' && c[1]=='n'){
                    Vector3D v;
                    n=fscanf(F,"%f%f%f",&v.x,&v.z,&v.y);
					//v.y=-v.y;
                    //v.x=-v.x;
                    if(n==3){
                        AddVertexN(v);
                    }else{
                        success=false;
                        break;
                    }
                }else
                if(c[0]=='f' && c[1]==0){
					if(VertsUV.GetAmount()==0){
						MessageBox(IRS->GetHWND(),TextManager::GetText("OBJ_NOUV_ERROR"),TextManager::GetText("OBJ_READ_ERROR"),MB_ICONEXCLAMATION);
						Clear();
						fclose(F);
						return false;
					}					
                    read_string(F,c,511);
                    int nf=FacesInfo.GetAmount();
                    char* pos=c;
                    char s[512];
					int VertList[512];
					int nvert=0;
                    int v0=-1;
                    do{
                        int p=sscanf(pos,"%s",s);
                        if(p>0){
                            char* m=strchr(s,'/');
                            if(m)m[0]=' ';
                            m=strchr(s,'/');
                            if(m)m[0]=' ';
                            int v1=0;
							int v2=0;
							int v3=0;
                            int p=sscanf(s,"%d%d%d",&v1,&v2,&v3);                            
                            if(p>0){
                                if(p==1){
                                    v2=v3=v1;
                                }
                                if(p==2){
									Vector3D N(0,0,1);
									v3=VertsN.Add(N)+1;                                    
                                }								
								if(v1<0)v1+=Verts.GetAmount();
								else v1--;
								if(v2<0)v2+=VertsUV.GetAmount();
								else v2--;
								if(v3<0)v3+=VertsN.GetAmount();
								else v3--;								

                                if(v2<VertsUV.GetAmount()){
                                    if(v0==-1)v0=v2;
                                    VertexUV* uv=&VertsUV[v2];
									if(uv->NIndex!=-1 || uv->PosIndex!=-1){
                                        if(uv->PosIndex!=v1 || uv->NIndex!=v3){
                                            VertexUV uv2=*uv;
                                            uv2.PosIndex=v1;
                                            uv2.NIndex=v3;
                                            v2=AddVertexUV(&uv2);
                                        }
                                    }else{
                                        uv->PosIndex=v1;
                                        uv->NIndex=v3;
                                    }                                    
                                    AddFaceVertex(nf,v2);
                                    SrcFaces.add(nf,_DWORD(v2));
                                }                                
								VertList[nvert++]=v1;
                            }else{
                                success=false;
                                break;
                            }
                        }else{
                            success=false;
                            break;                        
                        }
                        pos=strchr(pos+1,' ');
                        if(!pos)break;
                    }while(pos[0]!=0);
                    if(v0!=-1){
                        AddFaceVertex(nf,v0);
                        SrcFaces.add(nf,_DWORD(v0));
                    }
                    FinalizeFace(nf);
					for(int i=0;i<nvert;i++){
						DWORD v0=VertList[(i+nvert-1)%nvert];
						DWORD v1=VertList[i];
						DWORD v2=VertList[(i+1)%nvert];												
					}
                }else
				if(!strcmp(c,"usemtl")){
					if(read_string(F,c,511)){							
						char tn[256];
						if(GetTextureFromMtl(mtl_name,c+1,tn)){
							char dir	[_MAX_PATH];
							char drive	[_MAX_PATH];
							char file	[_MAX_PATH];
							char ext	[_MAX_PATH];							
							_splitpath( Name, drive, dir, file, ext );
							sprintf(c,"%s%s%s",drive,dir,tn);
							TextureID=IRS->GetTextureID(c);
                            Texture.LoadTexture(c);
						}
					}
				}else
				if(!strcmp(c,"mtllib")){
					read_string(F,c,511);
					char dir	[_MAX_PATH];
					char drive	[_MAX_PATH];
					char file	[_MAX_PATH];
					char ext	[_MAX_PATH];							
					_splitpath( Name, drive, dir, file, ext );
					sprintf(mtl_name,"%s%s%s",drive,dir,c+1);
				}
            }
        }while(n>0);
        fclose(F);
    }
	if(TextureID==-1){
		TextureID=IRS->GetTextureID("white.dds");
		Texture.LoadTexture("white.dds");
	}
	//subdivision if need
    int nfn4=0;
	for(int i=0;i<FacesInfo.GetAmount();i++){
		int n=Faces.size(i);
		if(n!=0 && n!=5)nfn4++;
	}
	int nsubdiv=0;
	int nf=FacesInfo.GetAmount()+1;
	while(nf<35000){
		nf<<=2;
		nsubdiv++;
	}
	if(nfn4 && nsubdiv==0)nsubdiv++;
	CellSize=AABB.GetDiagonal()/80.0f;
	Scale=1024.0/AABB.GetDiagonal();
    CalcNormals();
	for(int i=0;i<nsubdiv;i++){
		int nf=FacesInfo.GetAmount();
		if( (i==1 && nfn4==0) || i==1 ){
			//SubdivideQuadMesh(4);
			//break;
		}
		for(int q=0;q<nf;q++){
			DivideFace(q);            
		}	
		ApplyCatmullClark(0);			
	}	    
	StartShift=Vector3D::null;
	if(AABB.maxv.x<0 || AABB.minv.x>0)StartShift.x=(AABB.maxv.x+AABB.minv.x)/2;
	if(AABB.maxv.y<0 || AABB.minv.y>0)StartShift.y=(AABB.maxv.y+AABB.minv.y)/2;
	if(AABB.maxv.z<0 || AABB.minv.z>0)StartShift.z=(AABB.maxv.z+AABB.minv.z)/2;
	//StartShift=AABB.GetCenter();
	for(int i=0;i<Verts.GetAmount();i++){
		Verts[i].Pos-=StartShift;
	}
	CreateSubSurfaces(8);
	PutFacesInCells();	
	SetAllDirty();	
	CreateMeshes();
	if(NormalsRT!=-1){
		IRS->PushRenderTarget(NormalsRT);
		IRS->ClearDevice();
		IRS->PopRenderTarget();
		IRS->PushRenderTarget(ColorsRT);
		IRS->ClearDevice();
		IRS->PopRenderTarget();
	}
	UpdateDirtyFaces();
    return success;
}
struct ccVertInfo{
    Vector3D SummPos;
	int nneib;
};
void PolyMesh::CalcNormals(){
    int nf=FacesInfo.GetAmount();	
	DynArray<Vector3D> nrm;	
	nrm.Add(Vector3D::null,VertsN.GetAmount());
	for(int i=0;i<nf;i++){
		static DynArray<DWORD> uvv;
		uvv.FastClear();
		int nv=GetFaceVerts(i,uvv);
		for(int p=0;p<nv;p++){
			int pp=(p+nv-1)%nv;
			int pn=(p+1)%nv;
			VertexUV* vp=&VertsUV[uvv[pp]];
			VertexUV* vc=&VertsUV[uvv[p ]];
			VertexUV* vn=&VertsUV[uvv[pn]];
			Vector3D avg=Verts[vp->PosIndex].Pos+Verts[vn->PosIndex].Pos;
			Vector3D dv1=Verts[vp->PosIndex].Pos-Verts[vc->PosIndex].Pos;
			Vector3D dv2=Verts[vn->PosIndex].Pos-Verts[vc->PosIndex].Pos;
			Vector3D v2;
			v2.cross(dv1,dv2);
			nrm[vc->NIndex]+=v2;            
		}		
	}
	for(int i=0;i<VertsN.GetAmount();i++){		
		Vector3D v1=nrm[i];
		v1.normalize();		
		VertsN[i]=v1;
	}
}
void PolyMesh::ApplyCatmullClark(DWORD){
	int nf=FacesInfo.GetAmount();
	DynArray<Vector4D> npos;
	DynArray<Vector3D> nrm;
	npos.Add(Vector4D::null,Verts.GetAmount());
	nrm.Add(Vector3D::null,VertsN.GetAmount());
	for(int i=0;i<nf;i++){
		static DynArray<DWORD> uvv;
		uvv.FastClear();
		int nv=GetFaceVerts(i,uvv);
		for(int p=0;p<nv;p++){
			int pp=(p+nv-1)%nv;
			int pn=(p+1)%nv;
			VertexUV* vp=&VertsUV[uvv[pp]];
			VertexUV* vc=&VertsUV[uvv[p ]];
			VertexUV* vn=&VertsUV[uvv[pn]];
			Vector3D avg=Verts[vp->PosIndex].Pos+Verts[vn->PosIndex].Pos;
			Vector3D dv1=Verts[vp->PosIndex].Pos-Verts[vc->PosIndex].Pos;
			Vector3D dv2=Verts[vn->PosIndex].Pos-Verts[vc->PosIndex].Pos;
			Vector3D v2;
			v2.cross(dv1,dv2);
			nrm[vc->NIndex]+=v2;
            Vector4D ppos=avg;
			ppos.w=2;
			npos[vc->PosIndex]+=ppos;
		}		
	}
	for(int i=0;i<Verts.GetAmount();i++){
		if(npos[i].w!=0){
			Vector3D v1=npos[i];
			v1/=npos[i].w;
			Verts[i].Pos=(v1+Verts[i].Pos)*0.5f;		
		}
	}
	for(int i=0;i<VertsN.GetAmount();i++){		
		Vector3D v1=nrm[i];
		v1.normalize();		
		VertsN[i]=v1;
	}
}
void PolyMesh::InvalidatePosVertex(DWORD ID){    
	scan_key(VertsInfluence,ID,tri_DWORD* TD){
        tri_DWORD TD2(int(TD->V1)/8,int(TD->V2)/8,int(TD->V3)/8);
		MeshInCell** MC=Meshes.get(TD2);
		if(MC){
			(*MC)->IsValid=false;
		}
		ValidAABB* AB=FacesBB.get(*TD);
		if(AB){
			AB->Valid=false;
		}
	}scan_end;
}
void PolyMesh::PutFacesInCells(){
    FacesTreeL0.reset();
    FacesTreeL2.reset();
	ListL2.reset();
	VertsInfluence.reset();
	FacesBB.reset();
    DWORD maxface=0;
    scan(Faces,DWORD* Vert,DWORD* Face){
        if(*Face>maxface)maxface=*Face;
    }scan_end;
    for(DWORD i=0;i<=maxface;i++){
        Vector3D p(0,0,0);
        int nv=0;
		ValidAABB AB;
		AB.minv=Vector3D(FLT_MAX,FLT_MAX,FLT_MAX);
		AB.maxv=Vector3D(-FLT_MAX,-FLT_MAX,-FLT_MAX);
		AB.Valid=true;
        scan_key(Faces,i,DWORD* Vert){
            VertexUV* uv=&VertsUV[*Vert];
			Vector3D v=Verts[uv->PosIndex].Pos;
			p+=v;
			AB.Extend(v*Scale);            
            nv++;			
        }scan_end;
        if(nv){
            p/=nv;
            int px=ffloorf(p.x/CellSize);
            int py=ffloorf(p.y/CellSize);
            int pz=ffloorf(p.z/CellSize);
			tri_DWORD DW(px,py,pz);
            FacesTreeL0.add(DW,i);
			FacesInfo[i].Pos=DW;
			ValidAABB* AB0=FacesBB.get(DW);
			if(AB0){
				AB0->Extend(AB);
			}else FacesBB.add(DW,AB);
            tri_DWORD TD(px/8,py/8,pz/8);
            FacesTreeL2.add(TD,i);
			ListL2.add_once(TD,_DWORD(1));
			scan_key(Faces,i,DWORD* Vert){
				VertexUV* uv=&VertsUV[*Vert];
				VertsInfluence.add_uniq(uv->PosIndex,DW);
				VertsInCells.add_uniq(DW,uv->PosIndex);
			}scan_end;
        }
    }
}
void PolyMesh::WriteObj(const char* Name){
}
void PolyMesh::Subdivide(PolyMesh* Dest){
}
int PolyMesh::GetFaceVerts(DWORD FaceID,DynArray<DWORD>& Buf){
    int pos=0;
	scan_key(Faces,FaceID,DWORD* Vert){		
		Buf.Add(*Vert);
		pos++;
	}scan_end;
    Buf.pop_back();
	return pos-1;
}
char* PolyMesh::printcoor(int v){
    char* p=new char[128];
    int pi=VertsUV[v].PosIndex;
    Vector3D vp=Verts[pi].Pos;
    sprintf(p,"%d:(%.02f,%.02f,%.02f)",pi,vp.x,vp.y,vp.z);
    return p;
}
void PolyMesh::DivideEdge(DWORD PosVertex1,DWORD PosVertex2){
    DWORDS2 D(PosVertex1,PosVertex2);    
    VertexPos* pv1=&Verts[PosVertex1];
    VertexPos* pv2=&Verts[PosVertex2];
	Vector3D p1=pv1->Pos;
	Vector3D p2=pv2->Pos;
    BYTE L=max(pv1->SubdivLevel,pv2->SubdivLevel)+1;
    DWORD PosVertexC=AddVertexPos((pv1->Pos+pv2->Pos)*0.5f);	
    VertexPos* pvc=&Verts[PosVertexC];
    pvc->SubdivLevel=L;    
    int nf=0;
    
    DWORDS2 PrevUV(0,0);
    DWORD   PrevUVVertex;
    DWORDS2 PrevN(0,0);
    DWORD   PrevNVertex;
    float Coef=0.5;
    if(L>0)Coef=1;

    scan_key(FacesOnEdges,D,DWORD* Face){
        nf++;
        DWORD PrevVertUV=0xFFFFFFFF;
        DWORD ins=0xFFFFFFFF;
        scan_key(Faces,*Face,DWORD* VertUV){
            if(PrevVertUV!=0xFFFFFFFF){
                if(ins==0xFFFFFFFF){
                    VertexUV* uv1=&VertsUV[*VertUV];
                    VertexUV* uv2=&VertsUV[PrevVertUV];
                    DWORDS2 P(uv1->PosIndex,uv2->PosIndex);
                    if(P==D){
                        DWORDS2 cUV(*VertUV,PrevVertUV);
                        DWORDS2 cN(uv1->NIndex,uv2->NIndex);
                        DWORD vnUV;
                        DWORD vnN;
                        if(cN!=PrevN){                        
                            PrevN=cN;
							Vector3D N1=VertsN[uv1->NIndex];
							Vector3D N2=VertsN[uv2->NIndex];
							Vector3D nc=(N1+N2)*0.5f;
							nc.normalize();							
							Vector3D v2=Hermit(Verts[uv1->PosIndex].Pos,N1,Verts[uv2->PosIndex].Pos,N2,0.5f,nc,Coef);
							vnN=PrevNVertex=AddVertexN(nc);
                            Verts[PosVertexC].Pos=v2;
                        }else{
                            vnN=PrevNVertex;
                        }
                        if(cUV!=PrevUV){
                            PrevUV=cUV;
                            VertexUV uv;
                            uv.NIndex=vnN;
                            uv.PosIndex=PosVertexC;
                            uv.u=(uv1->u+uv2->u)/2;
                            uv.v=(uv1->v+uv2->v)/2;
                            vnUV=PrevUVVertex=AddVertexUV(&uv);                        
                        }else{
                            vnUV=PrevUVVertex;
                        }
                        Faces.add(*Face,*VertUV);
                        ins=vnUV;
                        swap(*VertUV,ins);
                    }
                }else{
                    swap(*VertUV,ins);
                }
            }
            PrevVertUV=*VertUV;
        }scan_end;
        FacesOnEdges.add(DWORDS2(PosVertex1,PosVertexC),*Face);
        FacesOnEdges.add(DWORDS2(PosVertex2,PosVertexC),*Face);
    }scan_end;
    FacesOnEdges.del(D);    
    //assert(nf==2);
}
void PolyMesh::DivideFace(DWORD FaceID){
    DWORD PrevPos=0xFFFFFFFF;
    BYTE  PrevLevel=0;
    BYTE  Level=FacesInfo[FaceID].SubdivLevel;    
    //dividing neib edges
    scan_key(Faces,FaceID,DWORD* VertUV){
        VertexUV* uv=&VertsUV[*VertUV];
        DWORD pidx=uv->PosIndex;
        VertexPos* pos=&Verts[uv->PosIndex];
		BYTE L=pos->SubdivLevel;
        if(PrevPos!=0xFFFFFFFF){
            if(uv->PosIndex!=PrevPos){
                if(PrevLevel<=Level && pos->SubdivLevel<=Level){
                    DivideEdge(uv->PosIndex,PrevPos);					
                }
            }
        }
        PrevPos=pidx;
        PrevLevel=L;
    }scan_end;
    //creating new faces
	static DynArray<DWORD> OrigVerts;
    static DynArray<DWORD> BasicVerts;
    static DynArray<DWORD> HalfVerts;
    BasicVerts.FastClear();
	OrigVerts.FastClear();
    HalfVerts.FastClear();
    PrevPos=0xFFFFFFFF;
    Vector3D summ(0,0,0);
    Vector3D summN(0,0,0);
    float summU=0;
    float summV=0;
    float nf=0;	
    scan_key(Faces,FaceID,DWORD* VertUV){
        VertexUV* uv=&VertsUV[*VertUV];
        DWORD pidx=uv->PosIndex;
        VertexPos* pos=&Verts[uv->PosIndex];
        if(PrevPos!=0xFFFFFFFF){            
            BasicVerts.Add(*VertUV);
			if(pos->SubdivLevel<=Level){
                summ+=pos->Pos;
                summN+=VertsN[uv->NIndex];
                summU+=uv->u;
                summV+=uv->v;
				OrigVerts.Add(*VertUV);
                nf++;
            }else
            if(pos->SubdivLevel==Level+1){
                HalfVerts.Add(*VertUV);
			}
            FacesOnEdges.del_elm(DWORDS2(PrevPos,pidx),FaceID);
        }
        PrevPos=pidx;
    }scan_end;
	tri_DWORD dwPos=FacesInfo[FaceID].Pos;
	int px=dwPos.V1;
	int py=dwPos.V2;
	int pz=dwPos.V3;
	tri_DWORD TD(px/8,py/8,pz/8);
    float Coef=0.5f;
    if(Level>=0)Coef=1;
    if(nf>0){
        summN.normalize();
        summ/=nf;
		if(OrigVerts.GetAmount()&1){
			float maxd=0;
			for(int i=0;i<OrigVerts.GetAmount();i++){
				VertexUV* uv=&VertsUV[OrigVerts[i]];
				float _cos=VertsN[uv->NIndex].dot(summN);
                assert(uv->NIndex!=-1);
				float _sin=sqrt(1-_cos*_cos);
				if(_sin==0)_sin=1;
				float d=(1-_cos)/_sin;
				d*=Verts[uv->PosIndex].Pos.distance(summ);
                assert(uv->PosIndex!=-1);
				maxd+=d/8;
			}
			maxd/=OrigVerts.GetAmount();
			Vector3D nv=summ+maxd*summN*SubdivCoef;			
			summ=nv;
		}else{
			int n2=HalfVerts.GetAmount()/2;
			summ=Vector3D::null;
			summN=Vector3D::null;
			for(int i=0;i<n2;i++){
				VertexUV* uv1=&VertsUV[HalfVerts[i]];
				VertexUV* uv2=&VertsUV[HalfVerts[i+n2]];				
				Vector3D N;
				summ+=Hermit(Verts[uv1->PosIndex].Pos,VertsN[uv1->NIndex],Verts[uv2->PosIndex].Pos,VertsN[uv2->NIndex],0.5f,N,Coef);
				summN+=N;
			}
			summ/=n2;
			summN.normalize();
		}
        int n=BasicVerts.GetAmount();        
		int nh=HalfVerts.GetAmount();
        RemoveFace(FaceID);
        DWORD newP=AddVertexPos(summ);
        VertexPos* vp=&Verts[newP];
        vp->SubdivLevel=Level+1;
        DWORD newN=AddVertexN(summN);
        VertexUV uv;
        uv.NIndex=newN;
        uv.PosIndex=newP;
        uv.u=summU/nf;
        uv.v=summV/nf;
        DWORD newUV=AddVertexUV(&uv);
		for(int i=0;i<nh;i++){
            DWORD prev=HalfVerts[i];
            DWORD next=HalfVerts[(i+1)%nh];            
            FaceInfo fi;
            fi.SubdivLevel=Level+1;
            DWORD newFace;
			if(i==0){
				newFace=FaceID;
				FacesInfo[FaceID].SubdivLevel=Level+1;
			}else{
				newFace=FacesInfo.GetAmount();
				FacesInfo.Add(fi);
			}
            AddFaceVertex(newFace,newUV);
            AddFaceVertex(newFace,prev);
			bool addv=false;
			int n2=n<<1;
			for(int j=0;j<n2;j++){				
				int i=j<n?j:j-n;
				DWORD curr=BasicVerts[i];
				if(curr==next && addv)break;				
				if(addv)AddFaceVertex(newFace,curr);
				if(curr==prev)addv=true;
			}            
            AddFaceVertex(newFace,next);
            AddFaceVertex(newFace,newUV);
            FinalizeFace(newFace);			
			FacesInfo[newFace].Pos=dwPos;
			if(FacesTreeL0.size()){
				FacesTreeL0.add(dwPos,newFace);
				FacesTreeL2.add(TD,newFace);		
			}
        }
    }
}
float RayTri(Vector3D &RayOrig, Vector3D &RayDir, Vector3D &t0, Vector3D &t1, Vector3D &t2,float& u,float& v);
Vector3D PolyMesh::PutPointOnSurface(Vector3D pt,Vector3D dir){
	Vector3D p=pt;
	Line3D Ray(pt,dir);
	int n=0;
	int nb=0;
	Vector3D Best;
	float Dist=FMAX;	
	DWORD BestFace;
	scan(FacesBB,ValidAABB* AB,tri_DWORD* Pos){
		if(AB->Overlap(Ray)){
			scan_key(FacesTreeL0,*Pos,DWORD* Face){
				int n=0;
				int v0=-1;
				int vp;
				scan_key(Faces,*Face,DWORD* V){                    
					int p=*V;
					if(v0!=p){
						if(v0==-1)v0=p;
						if(n>=2){
							VertexUV* uv1=&VertsUV[v0];
							VertexUV* uv2=&VertsUV[vp];
							VertexUV* uv3=&VertsUV[p];
							VertexBackup* VB1=SurfaceModificator::Backup.get(uv1->PosIndex);
							VertexBackup* VB2=SurfaceModificator::Backup.get(uv2->PosIndex);
							VertexBackup* VB3=SurfaceModificator::Backup.get(uv3->PosIndex);
							Vector3D pos1=VB1?VB1->Pos*Scale:Verts[uv1->PosIndex].Pos*Scale;
							Vector3D pos2=VB2?VB2->Pos*Scale:Verts[uv2->PosIndex].Pos*Scale;
							Vector3D pos3=VB3?VB3->Pos*Scale:Verts[uv3->PosIndex].Pos*Scale;
							float u,v;
							Vector3D org=Ray.getOrig();
							Vector3D dir=Ray.getDir();
							float d=RayTri(org,dir,pos1,pos2,pos3,u,v);
							if(abs(d)<Dist){
								Dist=abs(d);
								Best=pos1+(pos2-pos1)*u+(pos3-pos1)*v;
								BestFace=*Face;
							}
						}
						vp=p;                
						n++;
					}
				}scan_end;
			}scan_end;
		}
		n++;
	}scan_end;	
	if(Dist<FMAX){
		p=Best;		
	}    
    return p;
}
struct EdgeInfo{
	DWORDS2 Verts;
	DWORD   AdjFace1;
	DWORD   AdjFace2;
	DWORD   OppEdge1;
	DWORD   OppEdge2;
	int		NDiv;
	int		StripIdx;
};
void PolyMesh::SubdivideQuadMesh(float AvgLength){
	//I.create strips along mesh
	DynArray<EdgeInfo> Edges;	
	static uni_hash<DWORD,DWORDS2> EdgeHash;
	EdgeHash.fast_reset();
    //I.1  - create    Edge info
	for(int i=0;i<FacesInfo.GetAmount();i++){
		DWORD Verts[5];		
		int p=0;
		scan_key(Faces,i,DWORD* VertUV){
            VertexUV* uv=&VertsUV[*VertUV];
			Verts[p++]=uv->PosIndex;
		}scan_end;
		assert(p==5);
		for(int i=0;i<4;i++){
			DWORDS2 E(Verts[i],Verts[i+1]);
			DWORDS2 E2(Verts[(i+2)&3],Verts[(i+3)&3]);
			DWORD* EI=EdgeHash.get(E);
			DWORD* EI2=EdgeHash.get(E2);
			EdgeInfo* pEinf;
			DWORD CurrE;
			if(EI){
                pEinf=&Edges[*EI];
				pEinf->AdjFace2=i;				
				CurrE=*EI;
			}else{
				EdgeInfo Einf;
                Einf.AdjFace1=i;
				Einf.AdjFace2=-1;
				Einf.NDiv=-1;
				Einf.OppEdge1=-1;
				Einf.OppEdge2=-1;
				Einf.StripIdx=-1;
				Einf.Verts=E;				
				DWORD pos=Edges.Add(Einf);
				EdgeHash.add(E,pos);
				pEinf=&Edges[pos];
				CurrE=pos;
			}
			if(EI2){
				if(pEinf->OppEdge1==-1)pEinf->OppEdge1=*EI2;
				else if(pEinf->OppEdge2==-1)pEinf->OppEdge2=*EI2;
				pEinf=&Edges[*EI2];
				if(pEinf->OppEdge1==-1)pEinf->OppEdge1=CurrE;
				else if(pEinf->OppEdge2==-1)pEinf->OppEdge2=CurrE;
				assert(*EI2!=CurrE);
			}
		}
	}
	//I.2 - making strips
	int NStrips=0;
	DynArray<int> NDivs;
	DynArray<int> StartEdge;
	int maxlen=0;
	float avg=0;
	for(int i=0;i<Edges.GetAmount();i++){
		EdgeInfo* E=&Edges[i];
		if(E->StripIdx==-1){
			int len=0;
			float avgLen=0;			
			while(E && E->StripIdx!=NStrips){
				avgLen+=Verts[E->Verts.V1].Pos.distance(Verts[E->Verts.V2].Pos);
				E->StripIdx=NStrips;
				EdgeInfo* E0=E;
				E=E->OppEdge1!=-1 ? &Edges[E->OppEdge1] : NULL;
				if(E==NULL || (E->StripIdx==NStrips && E0->OppEdge2!=-1 && Edges[E0->OppEdge2].StripIdx!=NStrips)){
					E=&Edges[E0->OppEdge2];
				}
				len++;
			}
			avgLen/=float(len)/Scale;
			int ndiv=avgLen/AvgLength;
			assert(E->StripIdx==NStrips);
			NDivs.Add(ndiv);
			StartEdge.Add(i);
			NStrips++;
			avg+=len;
			if(len>maxlen)maxlen=len;
		}
	}
    static uni_hash<DWORD,DWORDS2A> MidUVVerts;
	static uni_hash<DWORD,DWORDS2A> MidNVerts;
	static uni_hash<DWORD,DWORDS2A> MidPosVerts;

	MidUVVerts .fast_reset();
	MidNVerts  .fast_reset();
	MidPosVerts.fast_reset();
	int nf=FacesInfo.GetAmount();
	for(int i=0;i<nf;i++){
		DWORD Verts[5];
		DWORD uvVerts[5];
		DWORD NVerts[5];		
		Vector3D P[5]; 
		Vector3D N[5];
		float u[5];
		float v[5];                
		int p=0;
		scan_key(Faces,i,DWORD* VertUV){
			VertexUV* uv=&VertsUV[*VertUV];
			VertexPos* vp=&PolyMesh::Verts[uv->PosIndex];
			if(vp->SubdivLevel<255){
				uvVerts[p]=*VertUV;
				NVerts[p]=uv->NIndex;
				P[p]=vp->Pos;
				N[p]=VertsN[uv->NIndex];
				u[p]=uv->u;
				v[p]=uv->v;
				Verts[p++]=uv->PosIndex;
			}
		}scan_end;
		assert(p==5);
		DWORDS2 E[4]={DWORDS2(Verts[0],Verts[1]),DWORDS2(Verts[1],Verts[2]),DWORDS2(Verts[2],Verts[3]),DWORDS2(Verts[3],Verts[0])};		
		int ND[4];
		int EDG[4];
		for(int q=0;q<4;q++){
			DWORD e1=*EdgeHash.get(E[q]);
			EDG[q]=e1;
			ND[q]=NDivs[Edges[e1].StripIdx];
		}
		assert(ND[0]==ND[2] && ND[1]==ND[3]);
		int Nx=ND[0]+2;
		int Ny=ND[1]+2;
		if(Nx>2 || Ny>2){
			int sq=Nx*Ny;
			static DynArray<DWORD> vUV;
			static DynArray<DWORD> vN;
			static DynArray<DWORD> vP;
			vUV.FastClear();
			vN.FastClear();
			vP.FastClear();
			vUV.Add(0,sq);
			vN.Add(0,sq);
			vP.Add(0,sq);

			vUV[0]=uvVerts[0];
			vUV[Nx-1]=uvVerts[1];
			vUV[(Ny-1)*Nx]=uvVerts[3];
			vUV[Ny*Nx-1]=uvVerts[2];

			vN[0]=NVerts[0];
			vN[Nx-1]=NVerts[1];
			vN[(Ny-1)*Nx]=NVerts[3];
			vN[Ny*Nx-1]=NVerts[2];

			vP[0]=Verts[0];
			vP[Nx-1]=Verts[1];
			vP[(Ny-1)*Nx]=Verts[3];
			vP[Ny*Nx-1]=Verts[2];
			static int edgidx[4]={0,2,3,1};
			static int v1idx[4]={0,2,3,1};
			static int v2idx[4]={1,3,0,2};
			static int lvertidx[4]={0,3,0,1};
			//filling boundary edges - please don't break your mind
			for(int side=0;side<2;side++){
				int NN=side? Ny:Nx;
				for(int p=1;p<NN-1;p++){				
					for(int y=0;y<2;y++){
						int s=side*2+y;
						int ofs= side ? (y*(Nx-1)+p*Nx) : (y*(Ny-1)*Nx+p);
						EdgeInfo* ei=&Edges[EDG[edgidx[s]]];
						int q=ei->Verts.V1 == Verts[lvertidx[s]] ? p : NN-1-p;
						DWORD v1=v1idx[s];
						DWORD v2=v2idx[s];
						DWORDS2A D(Verts[v1],Verts[v2],q);
						DWORDS2A Duv(uvVerts[v1],uvVerts[v2],q);
						DWORDS2A Dn(NVerts[v1],NVerts[v2],q);
						DWORD* pN=MidNVerts.get(Dn);
						DWORD vn;
						if(pN){
							vn=*pN;
						}else{
							vn=VertsN.Add(Vector3D::null);
							MidNVerts.add(Dn,vn);
						}
						DWORD* pP=MidPosVerts.get(D);
						DWORD vp;
						if(pP){
							vp=*pP;
						}else{
							VertexPos V;
							V.SubdivLevel=255;
							V.SelectionDegree=0;
							V.TempSelection=0;
                            V.W=1;
							vp=PolyMesh::Verts.Add(V);
							MidPosVerts.add(D,vp);
						}
						DWORD* puv=MidUVVerts.get(Duv);
						DWORD vuv;
						if(puv){
							vuv=*puv;							
						}else{
							VertexUV u;
							u.NIndex=vn;
							u.PosIndex=vp;				
							vuv=VertsUV.Add(u);
							MidUVVerts.add(Duv,vuv);
						}						
						vP[ofs]=vp;
						vN[ofs]=vn;
						vUV[ofs]=vuv;
					}			
				}
			}
			//filling vertices, that are inside
			for(int x=1;x<Nx-1;x++){
				for(int y=1;y<Ny-1;y++){
					VertexPos V;
					V.SubdivLevel=255;
					V.SelectionDegree=0;
					V.TempSelection=0;
                    V.W=1;
					int vp=PolyMesh::Verts.Add(V);
					int vn=VertsN.Add(Vector3D::null);
					VertexUV u;
					u.NIndex=vn;
					u.PosIndex=vp;				
					int vuv=VertsUV.Add(u);
					int ofs=x+y*Nx;
					vP[ofs]=vp;
					vN[ofs]=vn;
					vUV[ofs]=vuv;
				}
			}
			//setting positions and normals, using Besier splines			
			int ps=0;
			float ddx=1.0f/(Nx-1);
			float ddy=1.0f/(Ny-1);

			Vector3D T[4];
			Vector3D B[4];

			Vector3D t,b;

            const int iv1[4]={1,1,2,2};
            const int iv2[4]={3,2,2,3};
            const int iv3[4]={0,0,3,3};
            const int iv4[4]={0,1,1,0};

            for(int q=0;q<4;q++){
                Vector3D dt=P[iv1[q]]-P[iv3[q]];
                float lt=dt.norm();
                t.cross(dt,N[q]);
                Vector3D db=P[iv2[q]]-P[iv4[q]];
                float lb=db.norm();
			    b.cross(db,N[q]);
			    T[q].cross(N[q],t);
                T[q].normalize();
                T[q]*=lt*1.3;
			    B[q].cross(N[q],b);
                B[q].normalize();
                B[q]*=lb*1.3;
            }
            /*
			t.cross(P[1]-P[0],N[0]);
			b.cross(P[3]-P[0],N[0]);
			T[0].cross(N[0],t);
			B[0].cross(N[0],b);            

			t.cross(P[1]-P[0],N[1]);
			b.cross(P[2]-P[1],N[1]);
			T[1].cross(N[1],t);
			B[1].cross(N[1],b);

			t.cross(P[2]-P[3],N[2]);
			b.cross(P[2]-P[1],N[2]);
			T[2].cross(N[2],t);
			B[2].cross(N[2],b);

			t.cross(P[2]-P[3],N[3]);
			b.cross(P[3]-P[0],N[3]);
			T[3].cross(N[3],t);
			B[3].cross(N[3],b);
            */
           

			Vector3D PT1=P[3]-P[0];
			Vector3D PT2=P[2]-P[1];
			Vector3D PB1=P[1]-P[0];
			Vector3D PB2=P[2]-P[3];
			for(int y=0;y<Ny;y++){
				float dy=y*ddy;
				float y2 = dy*dy;
				float y3 = y2*dy;
				float KY1 = (2 * y3 - 3 * y2 + 1);
				float KY2 = (- 2 * y3 + 3 * y2);
				float KY3 = (y3 - 2 * y2 + dy);
				float KY4 = (y3 - y2);
				Vector3D P1 = KY1 * P[0] + KY2 * P[3] + KY3 * B[0] + KY4 * B[3];
				Vector3D P2 = KY1 * P[1] + KY2 * P[2] + KY3 * B[1] + KY4 * B[2];                
				Vector3D T1 = T[0]*(1-dy) + T[3]*dy;
				Vector3D T2 = T[1]*(1-dy) + T[2]*dy;
				for(int x=0;x<Nx;x++){
					float dx=x*ddx;
					float uu=u[0]+(u[1]-u[0])*dx+(u[3]-u[0])*dy+(u[0]+u[2]-u[1]-u[3])*dx*dy;
					float vv=v[0]+(v[1]-v[0])*dx+(v[3]-v[0])*dy+(v[0]+v[2]-v[1]-v[3])*dx*dy;
					float x2 = dx*dx;
					float x3 = x2*dx;
					PolyMesh::Verts[vP[ps]].Pos=(2*x3 - 3*x2 + 1)*P1 + (-2*x3 + 3*x2)*P2 + (x3 - 2*x2 + dx)*T1 + (x3 - x2)*T2;                    
                    //PolyMesh::VertsN[vN[ps]]=Vector3D::null;//N[0]+(N[1]-N[0])*dx+(N[3]-N[0])*dy+(N[0]+N[2]-N[1]-N[3])*dx*dy;
					VertexUV* uv=&VertsUV[vUV[ps]];
					uv->u=uu;
					uv->v=vv;
					ps++;
				}
			}
			//creating new faces
			int Fc=-1;
			int ofs=0;
			for(int y=0;y<Ny-1;y++){
				for(int x=0;x<Nx-1;x++){					
					if(Fc==-1){
						Fc=i;
						RemoveFace(i);
					}else{
						FaceInfo fi;
						fi.SubdivLevel=255;
						fi.UVEdgeMask=0;
						fi.Points=NULL;						
						Fc=FacesInfo.Add(fi);
					}
					DWORD D1=vUV[ofs];
					DWORD D2=vUV[ofs+1];
					DWORD D3=vUV[ofs+Nx+1];
					DWORD D4=vUV[ofs+Nx];

					AddFaceVertex(Fc,D1);
					AddFaceVertex(Fc,D2);
					AddFaceVertex(Fc,D3);
					AddFaceVertex(Fc,D4);
					AddFaceVertex(Fc,D1);

					Vector3D P0=PolyMesh::Verts[VertsUV[vUV[ofs]].PosIndex].Pos;
					Vector3D P1=PolyMesh::Verts[VertsUV[vUV[ofs+1]].PosIndex].Pos;
					Vector3D P2=PolyMesh::Verts[VertsUV[vUV[ofs+Nx+1]].PosIndex].Pos;
					Vector3D P3=PolyMesh::Verts[VertsUV[vUV[ofs+Nx]].PosIndex].Pos;

					FinalizeFace(Fc);
					ofs++;
				}
				ofs++;
			}			
		}
    }
    //calc normals
    for(int i=0;i<VertsN.GetAmount();i++){
        VertsN[i]=Vector3D::null;
    }    
    for(int i=0;i<FacesInfo.GetAmount();i++){
        Vector3D Pos[5];
        Vector3D* Ns[5];
        int p=0;
        scan_key(Faces,i,DWORD* Vert){
            VertexUV* uv=&VertsUV[*Vert];
            Pos[p]=Verts[uv->PosIndex].Pos;
            Ns[p++]=&VertsN[uv->NIndex];
        }scan_end;
        if(p>0){
            assert(p==5);
            for(int i=0;i<4;i++){
                Vector3D vp=Pos[(i+3)&3]-Pos[i];
                Vector3D vn=Pos[(i+1)&3]-Pos[i];
                Vector3D vc;
                vc.cross(vn,vp);
                vc.normalize();
                *(Ns[i])+=vc;
            }
        }
    }
    for(int i=0;i<VertsN.GetAmount();i++){
        VertsN[i].normalize();
    }
    
}
Vector3D PolyMesh::GetAveragePN(DWORD Face,int x,int y,int Size,Vector3D& N){
    int sz2=Size*(Size+1);
    int n=0;
    Vector3D v(0,0,0);
    N=Vector3D(0,0,0);
    for(int dy=-Size;dy<=Size;dy++){
        for(int dx=-Size;dx<=Size;dx++){
            if(dx*dx+dy*dy<sz2){
                PointInfo p=GetPoint(Face,x+dx,y+dy);
                if(p.pt){
                    v+=p.pt->Pos;
                    N+=p.pt->N;
                    n++;
                }
            }
        }
    }
    if(n){
        v/=n;
        N.normalize();
    }
    return v;
}
Vector3D PolyMesh::GetAveragePNC(DWORD Face,int x,int y,int Size,Vector3D* N,Vector4D* C){
	int sz2=Size*(Size+1);
	int n=0;
    float cw=0;
	Vector3D v(0,0,0);
	if(N)*N=Vector3D::null;
	if(C)*C=Vector4D::null;
	for(int dy=-Size;dy<=Size;dy++){
		for(int dx=-Size;dx<=Size;dx++){
			if(dx*dx+dy*dy<sz2){
				PointInfo p=GetPoint(Face,x+dx,y+dy);
				if(p.pt){
                    LayerOnFace* L=GetCurrentLayer(p.Face);
                    OneLayerPoint* lp=L->Points+p.x+p.y*SubSurfaceSize;
					v+=lp->Dv;
					if(N)*N+=p.pt->N;
					if(C){
                        Vector4D C2=DW2V4D(lp->Color);
                        C2*=lp->Transparency;
						*C+=C2;
                        cw+=lp->Transparency;
					}
					n++;
				}
			}
		}
	}
	if(n){
		v/=n;
		if(N)N->normalize();
		if(C && cw>0)*C/=cw;		
	}
	return v;
}
PointInfo PolyMesh::GetPoint(DWORD Face,int x,int y){
    PointInfo pi;
    pi.x=x;
    pi.y=y;
    pi.Face=Face;
	int ssf=SubSurfaceSize-1;
    FaceInfo* fi=&FacesInfo[Face];
    if(fi->Points){
        bool done=true;
        do{
            done=true;
			DWORD FaceNew=0;
			if(y<0){
				swap(x,y);
				y=ssf-y;
				FaceNew=fi->UpFace;
			}else
			if(x>=SubSurfaceSize){
				x=ssf-x;
				y=ssf-y;
				FaceNew=fi->RFace;
			}else
			if(y>=SubSurfaceSize){
				swap(x,y);
				x=ssf-x;				
				FaceNew=fi->DnFace;
			}else
			if(x<0){
				FaceNew=fi->LFace;
			}
            if(x<0){                
				if(FaceNew==-1){
					clamp(pi.x,0,SubSurfaceSize-1);
					clamp(pi.y,0,SubSurfaceSize-1);
                    pi.pt=&fi->Points[pi.x+pi.y*SubSurfaceSize];
					return pi;
				}else{
					fi=&FacesInfo[FaceNew];
					if(fi->RFace==Face){
						x+=SubSurfaceSize;
					}else 
					if(fi->LFace==Face){
						x=-1-x;
						y=ssf-y;
					}else
					if(fi->UpFace==Face){
						swap(x,y);					
						y=-1-y;
					}else
					if(fi->DnFace==Face){
						swap(x,y);
						x=ssf-x;
						y=ssf+y;
					}
					Face=FaceNew;
					done=false;
				}
            }            
        }while(!done);
        if(fi->Points){
            pi.x=x;
            pi.y=y;
            pi.Face=Face;
            pi.pt=&fi->Points[x+y*SubSurfaceSize];                        
        }
    }
    return pi;
}
void PolyMesh::CalcNormal(PointInfo& p){
    if(p.pt){
        int x=p.x;
        int y=p.y;
        PointInfo L=GetPoint(p.Face,x-1,y);
        PointInfo R=GetPoint(p.Face,x+1,y);
        PointInfo T=GetPoint(p.Face,x,y-1);
        PointInfo B=GetPoint(p.Face,x,y+1);
        if(L.pt && R.pt && T.pt && B.pt){
            Vector3D dvx=R.pt->Pos-L.pt->Pos;
            Vector3D dvy=B.pt->Pos-T.pt->Pos;
			Vector3D n0=p.pt->N;
            p.pt->N.cross(dvy,dvx);
            p.pt->N.normalize();
			//p.pt->N.x=cos(p.pt->Pos.x*300);
			//p.pt->N.y=cos(p.pt->Pos.y*300);
			//p.pt->N.z=cos(p.pt->Pos.z*300);
			/*
			float  dp=n0.dot(p.pt->N);
			float DL=p.pt->Pos.distance(L.pt->Pos);
			float DR=p.pt->Pos.distance(R.pt->Pos);
			float DT=p.pt->Pos.distance(T.pt->Pos);
			float DB=p.pt->Pos.distance(B.pt->Pos);
			dp=dp+1;
			dp=dp-1;
			*/
        }
    }
}
void PolyMesh::CopyFaceToTexture(DWORD Face){
    FaceInfo* fo=&FacesInfo[Face];
    float u[4];
    float v[4];
    for(int i=0;i<4;i++){
        DWORD* puv=Faces.get(Face,i);
        VertexUV* uv=&VertsUV[*puv];
        assert(uv);
        u[i]=uv->u;
        v[i]=uv->v;
    }
    assert(SubSurfaceSize<31);
    const int side=32;
    const int sidex2=side*side;
    static float ui[sidex2];
    static float vi[sidex2];
    static DWORD ci[sidex2];
    static DWORD ni[sidex2];
    int pos=0;
    float d=1.0f/SubSurfaceSize;    
    for(int y=0;y<=SubSurfaceSize;y++){
        int ofs=y*side;
        for(int x=0;x<=SubSurfaceSize;x++){
            float uu=u[0]+(u[1]-u[0])*x*d+(u[3]-u[0])*y*d+(u[0]+u[2]-u[1]-u[3])*x*y*d*d;
            float vv=v[0]+(v[1]-v[0])*x*d+(v[3]-v[0])*y*d+(v[0]+v[2]-v[1]-v[3])*x*y*d*d;            
            ui[ofs]=uu;
            vi[ofs]=vv;
            PointInfo p=GetPoint(Face,x,y);
            ci[ofs]=p.pt->Color;
            Vector3D v=p.pt->N*0.5f+Vector3D(0.5f,0.5f,0.5f);
            ni[ofs]=V4D2DW(Vector4D(v));
            ofs++;
        }
    }
    for(int y=0;y<SubSurfaceSize;y++){
        int ofs=y*side;
        for(int x=0;x<SubSurfaceSize;x++){
            Texture.SetQuad(ui[ofs],vi[ofs],ci[ofs],ui[ofs+1],vi[ofs+1],ci[ofs+1],ui[ofs+side],vi[ofs+side],ci[ofs+side],ui[ofs+side+1],vi[ofs+side+1],ci[ofs+side+1]);
            Normals.SetQuad(ui[ofs],vi[ofs],ni[ofs],ui[ofs+1],vi[ofs+1],ni[ofs+1],ui[ofs+side],vi[ofs+side],ni[ofs+side],ui[ofs+side+1],vi[ofs+side+1],ni[ofs+side+1]);
        }
    }    
}
void PolyMesh::CreateMeshOfOneCell(tri_DWORD& pos){
    bool Centerbased=false;
    if(Centerbased){
        uni_hash<DWORD> UsedVerts;
        static DynArray<WORD> TriList;
        static DynArray<VertexN> VertList;        
        TriList.FastClear();
        VertList.FastClear();        
        MeshInCell* M=new MeshInCell;
        M->IsValid=true;
        scan_key(FacesTreeL2,pos,DWORD* Face){                        
            int vp=-1;
            int n=0;
            Vector3D summ(0,0,0);
            Vector3D nrm(0,0,0);
            float summ_u=0;
            float summ_v=0;
            float ns=0;            
            VertexN VN;
            int vc=VertList.Add(VN);
            scan_key(Faces,*Face,DWORD* V){                
                DWORD* u=UsedVerts.get(*V);
                DWORD p;
                if(!u){
                    VertexUV& uv=VertsUV[*V];                        
                    VN.u=uv.u;
                    VN.v=1-uv.v;
                    Vector3D* vn=&VertsN[uv.NIndex];
                    VN.nx=vn->x;
                    VN.ny=vn->y;
                    VN.nz=vn->z;
                    VertexPos* VP=&Verts[uv.PosIndex];
                    VN.x=VP->Pos.x;
                    VN.y=VP->Pos.y;
                    VN.z=VP->Pos.z;      
                    p=VertList.GetAmount();
                    VertList.Add(VN);                    
                    UsedVerts.add(*V,_DWORD(p));
                }else p=*u;                
                if(vp!=-1){
                    TriList.Add(vc);
                    TriList.Add(vp);
                    TriList.Add(p);
                    if(vc==vp||vc==p||vp==p){
                        vc=p;
                    }
                    VertexN& vv=VertList[p];
                    summ+=Vector3D(vv.x,vv.y,vv.z);
                    nrm+=Vector3D(vv.nx,vv.ny,vv.nz);
                    summ_u+=vv.u;
                    summ_v+=vv.v;
                    ns++;
                }
                vp=p;
            }scan_end;
            summ/=ns;
            nrm/=ns;
            VertexN& vv=VertList[vc];
            vv.x=summ.x;
            vv.y=summ.y;
            vv.z=summ.z;
            vv.nx=nrm.x;
            vv.ny=nrm.y;
            vv.nz=nrm.z;
            vv.u=summ_u/ns;
            vv.v=summ_v/ns;            
        }scan_end;
        if(TriList.GetAmount()){
            M->Mesh.create(VertList.GetAmount(),TriList.GetAmount(),vfVertexN);
            WORD* idxs=M->Mesh.getIndices();
            if(idxs){
                memcpy(idxs,TriList.GetValues(),TriList.GetAmount()*2);
            }
            VertexN* vn=(VertexN*)M->Mesh.getVertexData();
            if(vn){
                memcpy(vn,VertList.GetValues(),VertList.GetAmount()*sizeof(VertexN));
            }
            M->Mesh.setNVert(VertList.GetAmount());
            M->Mesh.setNInd(TriList.GetAmount());
            M->Mesh.setNPri(TriList.GetAmount()/3);
            static int sh=IRS->GetShaderID("obj");
            static int sh_sel=IRS->GetShaderID("obj_sel");
            static int tex=IRS->GetTextureID("WaterSample.dds");
            M->Mesh.setShader(sh);
            M->Mesh.setTexture(ColorsRT);
            M->Mesh.setTexture(NormalsRT,1);
            M->Mesh.unlock();
            Meshes.add(pos,M);
        }else delete(M);
    }else{
        uni_hash<DWORD> UsedVerts;
        int nv=0;
        static DynArray<WORD> TriList;
        TriList.FastClear();  
        MeshInCell* M=new MeshInCell;
        M->IsValid=true;
        scan_key(FacesTreeL2,pos,DWORD* Face){
            int v0=-1;
            int v00=-1;
            int vp=-1;
            int n=0;
            FaceInfo* fi=&FacesInfo[*Face];            
            scan_key(Faces,*Face,DWORD* V){
                if(v00!=*V){
                    if(v00==-1)v00=*V;
                    DWORD* u=UsedVerts.get(*V);
                    int p=nv;
                    if(!u){
                        UsedVerts.add(*V,_DWORD(nv++));                
                    }else p=*u;       
                    if(v0==-1)v0=p;
                    if(n>=2){
                        TriList.Add(v0);
                        TriList.Add(vp);
                        TriList.Add(p);
                    }
                    vp=p;
                    n++;
                }
            }scan_end;            
        }scan_end;
        if(TriList.GetAmount()){            
            M->Mesh.create(UsedVerts.size(),TriList.GetAmount(),vfVertexN);
            WORD* idxs=M->Mesh.getIndices();
            if(idxs){
                memcpy(idxs,TriList.GetValues(),TriList.GetAmount()*2);
            }
            VertexN* vn=(VertexN*)M->Mesh.getVertexData();
            if(vn){
                M->OrgVerts.Add(0,UsedVerts.size());
                scan(UsedVerts,DWORD* Elm,DWORD* Key){
                    VertexUV& uv=VertsUV[*Key];
                    VertexN* VN=vn+(*Elm);
                    VN->u=uv.u;
                    VN->v=1-uv.v;
                    Vector3D* vn=&VertsN[uv.NIndex];
                    VN->nx=vn->x;
                    VN->ny=vn->y;
                    VN->nz=vn->z;
                    VertexPos* VP=&Verts[uv.PosIndex];
                    VN->x=VP->Pos.x;
                    VN->y=VP->Pos.y;
                    VN->z=VP->Pos.z;                
                    M->OrgVerts[*Elm]=*Key;
                }scan_end;
            }
            M->Mesh.setNVert(UsedVerts.size());
            M->Mesh.setNInd(TriList.GetAmount());
            M->Mesh.setNPri(TriList.GetAmount()/3);
            static int sh=IRS->GetShaderID("obj");
            static int tex=IRS->GetTextureID("WaterSample.dds");
            M->Mesh.setShader(sh);		    
            M->Mesh.setTexture(ColorsRT);
            M->Mesh.setTexture(NormalsRT,1);
            M->Mesh.unlock();
            Meshes.add(pos,M);
        }else delete(M);
    }
}
void PolyMesh::RefineMeshOfOneCell(MeshInCell* M){    
    int no=M->OrgVerts.GetAmount();
    if(no){
        VertexN* VN=(VertexN*)M->Mesh.getVertexData();
        for(int i=0;i<no;i++){
            int id=M->OrgVerts[i];
            VertexUV& uv=VertsUV[id];                            
            VN->u=uv.u;
            VN->v=1-uv.v;
            Vector3D* vn=&VertsN[uv.NIndex];
            VN->nx=vn->x;
            VN->ny=vn->y;
            VN->nz=vn->z;
            VertexPos* VP=&Verts[uv.PosIndex];
            VN->x=VP->Pos.x;
            VN->y=VP->Pos.y;
            VN->z=VP->Pos.z; 
            VN++;
        }
    }
    M->Mesh.unlock();
}
void PolyMesh::CreateMeshes(){
    scan(Meshes,MeshInCell** M,tri_DWORD* D){
        if(!(*M)->IsValid){
            delete(*M);
            Meshes.del(*D);
        }
    }scan_end;
    scan(ListL2,DWORD* Elm,tri_DWORD* Key){
        MeshInCell** M=Meshes.get(*Key);
        if(!M){
            CreateMeshOfOneCell(*Key);            
        }
    }scan_end;    
}
void PolyMesh::RefineMeshes(){
    scan(Meshes,MeshInCell** M,tri_DWORD* D){
        if(!(*M)->IsValid){            
            RefineMeshOfOneCell(*M);
            (*M)->IsValid=true;            
        }
    }scan_end;    
}
void PolyMesh::Render(){
    static int sh_sel=IRS->GetShaderID("obj_sel");
    static int sh=IRS->GetShaderID("obj");    
    int shader=FreezeTool::FeezeView ? sh_sel:sh;
    Matrix4D m;
    m.st(Scale,Vector3D::null);
    IRS->SetWorldTM(m);
    static int chess=IRS->GetTextureID("chess.dds");
    IRS->SetTexture(chess,2);
    scan(Meshes,MeshInCell** M,tri_DWORD* D){
        IRS->SetShader(shader);
        IRS->SetTexture((*M)->Mesh.getTexture());
        IRS->SetTexture((*M)->Mesh.getTexture(1),1);        
        (*M)->Mesh.setTexture(chess,2);
        (*M)->Mesh.render();
    }scan_end;
    IRS->Flush();
}
void PolyMesh::CreateRenderTargets(){
    rtSizeX=2048;    
	rtSizeY=2048;
	NormalsRT=IRS->CreateTexture("NormalsRT",rtSizeX,rtSizeY,cfARGB8888,1,tmpDefault,true);
	if(NormalsRT==-1){
		MessageBox(IRS->GetHWND(),"Unable to create render target 2048x2048. It is possible to lack of video-memory.","Critical error.",MB_ICONERROR);
		exit(1);
	}
	ColorsRT=IRS->CreateTexture("ColorsRT",rtSizeX,rtSizeY,cfARGB8888,1,tmpDefault,true);
	if(ColorsRT==-1){
		MessageBox(IRS->GetHWND(),"Unable to create render target 2048x2048. It is possible to lack of video-memory.","Critical error.",MB_ICONERROR);
		exit(1);
	}

	IRS->PushRenderTarget(NormalsRT);
	IRS->ClearDevice();
	IRS->PopRenderTarget();
	IRS->PushRenderTarget(ColorsRT);
	IRS->ClearDevice();
	IRS->PopRenderTarget();
}
void PolyMesh::AddDirtyFace(DWORD FaceID){
    DirtyFaces.add_once(FaceID,FaceID);
}
void PolyMesh::SetAllDirty(){
	for(int i=0;i<FacesInfo.GetAmount();i++){
		AddDirtyFace(i);
	}
}
void PolyMesh::DropFacesHistoryToUndo(){
    SurfaceUndo::DropSurfaceUndo();
}
void PolyMesh::Undo(){
    UndoMaster::Undo();    
}
void OneBackupItem::Undo(){
    for(int i=0;i<Backup.GetAmount();i++){
        OneFaceBackup* FB=&Backup[i];
        FaceInfo* FI=&M->FacesInfo[FB->Face];
        LayerOnFace* LF=M->GetSomeLayer(FB->Face,FB->Layer);
        if(FI->Points){
            memcpy(LF->Points,FB->Points,M->SubSurfaceSize*M->SubSurfaceSize*sizeof OneLayerPoint);
        }
		int p=0;
        scan_key(M->Faces,FB->Face,DWORD* Vert){
            VertexUV* uv=&M->VertsUV[*Vert];
			if(p<4){
				M->Verts[uv->PosIndex].Pos=FB->Verts[p];
				M->VertsN[uv->NIndex]=FB->Normals[p];
			}
            M->InvalidatePosVertex(uv->PosIndex);
			p++;
        }scan_end;		
    }
    for(int i=0;i<Backup.GetAmount();i++){
		OneFaceBackup* FB=&Backup[i];
        M->CalcFaceAttributesWithLayers(FB->Face);
	}
	for(int i=0;i<Backup.GetAmount();i++){
		OneFaceBackup* FB=&Backup[i];
		M->CreateFaceNormals(FB->Face);
		M->AddDirtyFace(FB->Face);
	}
}
void OneBackupItem::Save(BinStream* bs){    
    bs->WriteDWORD(Backup.GetAmount());
    for(int i=0;i<Backup.GetAmount();i++){
        OneFaceBackup* fb=&Backup[i];
        bs->Write(fb,sizeof(OneFaceBackup));
        if(fb->Points){
            bs->Write(fb->Points,M->SubSurfaceSize*M->SubSurfaceSize*sizeof(OneLayerPoint));
        }        
    }
}
void OneBackupItem::Load(BinStream* bs){
    int n=bs->ReadDWORD();
    for(int i=0;i<n;i++){
        static OneFaceBackup* FB=new OneFaceBackup;
        bs->Read(FB,sizeof(OneFaceBackup));
        if(FB->Points){
            FB->Points=new OneLayerPoint[M->SubSurfaceSize*M->SubSurfaceSize];
            bs->Read(FB->Points,M->SubSurfaceSize*M->SubSurfaceSize*sizeof(OneLayerPoint));
        }
        Backup.Add(*FB);
    }
}
void PolyMesh::FreeUndoHistory(){
	for(int i=0;i<UndoHistory.GetAmount();i++){
        delete(UndoHistory[i]);
	}
	UndoHistory.Clear();
	scan(FacesHistory,OneFaceBackup** B,bi_DWORD* Face){
        delete(*B);
	}scan_end;
	FacesHistory.reset();
}
OneLayerPoint* PolyMesh::GetFaceHistory(DWORD Face){
	OneFaceBackup** B=FacesHistory.get(bi_DWORD(Face,CurrentLayerID));
	if(B)return (*B)->Points;
	else return NULL;
}
void PolyMesh::DropFaceToUndo(DWORD Face){
	OneFaceBackup** B=FacesHistory.get(bi_DWORD(Face,CurrentLayerID));
	if(!B){
		if(FacesInfo[Face].Points){
			OneFaceBackup* FB=new OneFaceBackup;
			FB->Face=Face;
            FB->Layer=CurrentLayerID;
			int n=SubSurfaceSize*SubSurfaceSize;
            FB->Points=new OneLayerPoint[n];
            LayerOnFace* LF=GetCurrentLayer(Face);
			memcpy(FB->Points,LF->Points,n*sizeof OneLayerPoint);
			int p=0;
			scan_key(Faces,Face,DWORD* Vert){
				VertexUV* uv=&VertsUV[*Vert];
				if(p<4){
					FB->Verts[p]=Verts[uv->PosIndex].Pos;
					FB->Normals[p]=VertsN[uv->NIndex];
				}				
				p++;
			}scan_end;
			FacesHistory.add(bi_DWORD(Face,CurrentLayerID),FB);
		}
	}
}
OneFaceBackup::OneFaceBackup(){
    Points=NULL;
}
OneBackupItem::OneBackupItem(){
}
OneBackupItem::~OneBackupItem(){
    int n=Backup.GetAmount();
    for(int i=0;i<n;i++){
        delete(Backup[i].Points);
    }
    Backup.Clear();
}
OneFaceBackup::~OneFaceBackup(){
    if(Points)delete[]Points;
    Points=NULL;
}
void PolyMesh::UpdateDirtyFaces(bool NormOnly){
    static BaseMesh* bm;
	int NFacesInPass=300;
    const int D=1;
	int maxvert=NFacesInPass*(SubSurfaceSize+1+D)*(SubSurfaceSize+1+D);
	int maxidx=NFacesInPass*(SubSurfaceSize+D)*(SubSurfaceSize+D)*6;
	static int NrmShader=IRS->GetShaderID("obj_render_normals");
	static int ClrShader=IRS->GetShaderID("obj_render_color");
    if((bm && bm->getMaxVert()!=maxvert) || bm==NULL){
        if(bm)delete(bm);
		bm=new BaseMesh;
		bm->create(maxvert,maxidx,vfVertexTS);
        for(int i=0;i<NFacesInPass;i++){
			//init verts
			VertexTS* vt=(VertexTS*)bm->getVertexData();
			memset(vt,0,sizeof(VertexTS)*bm->getMaxVert());			
			//creating indices
			WORD* idxs=bm->getIndices();
			for(int i=0;i<NFacesInPass;i++){
				int v0=i*(SubSurfaceSize+1+D)*(SubSurfaceSize+1+D);
				for(int y=0;y<SubSurfaceSize+D;y++){
					for(int x=0;x<SubSurfaceSize+D;x++){
                        idxs[0]=v0+x;
						idxs[1]=v0+x+1;
						idxs[2]=v0+x+SubSurfaceSize+1+D;
						idxs[3]=v0+x+1;
						idxs[4]=v0+x+SubSurfaceSize+2+D;
						idxs[5]=v0+x+SubSurfaceSize+1+D;
						idxs+=6;
					}
					v0+=SubSurfaceSize+1+D;
				}
			}
		}
	}
	int NRemains=NFacesInPass;
	VertexTS* vt=(VertexTS*)bm->getVertexData();
    if(NormOnly){
        IRS->PushRenderTarget(NormalsRT);
    }
	scan(DirtyFaces,DWORD* Face,DWORD* F){
		FaceInfo* fo=&FacesInfo[*Face];
		float u[4];
		float v[4];
		int nv=Faces.size(*Face);
		if(nv==5){
			for(int i=0;i<4;i++){
				DWORD* puv=Faces.get(*Face,i);			
				VertexUV* uv=&VertsUV[*puv];
				assert(uv);
				u[i]=uv->u;
				v[i]=uv->v;
			}
			BYTE m=fo->UVEdgeMask;
			if(m){
				m++;
				m--;
			}
			float d=1.0f/SubSurfaceSize;  			
			float dx0=sqrt((u[1]-u[0])*(u[1]-u[0])+(v[1]-v[0])*(v[1]-v[0]))*d*2048.0f;
			float dy0=sqrt((u[3]-u[0])*(u[3]-u[0])+(v[3]-v[0])*(v[3]-v[0]))*d*2048.0f;
			static float Xdiff=5;
			static float Ydiff=5;
			Xdiff=2/dx0;
			Ydiff=2/dy0;
			for(int y=-D;y<=SubSurfaceSize;y++){			
				for(int x=-D;x<=SubSurfaceSize;x++){
					float dx=x+0.5f;
					float dy=y+0.5f;
					if(m&8 && x==-D){						
						dx-=Xdiff;
					}
					if(m&2 && x==SubSurfaceSize){
						dx+=Xdiff;
					}
					if(m&1 && y==-D){
						dy-=Ydiff;
					}
					if(m&4 && y==SubSurfaceSize){
						dy+=Ydiff;
					}
					float uu=u[0]+(u[1]-u[0])*dx*d+(u[3]-u[0])*dy*d+(u[0]+u[2]-u[1]-u[3])*dx*dy*d*d;
					float vv=v[0]+(v[1]-v[0])*dx*d+(v[3]-v[0])*dy*d+(v[0]+v[2]-v[1]-v[3])*dx*dy*d*d;            
					vt->u=uu;
					vt->v=vv;
                    PointInfo p=GetPoint(*Face,x,y);
					if(p.pt){
						vt->diffuse=p.pt->Color;
						Vector3D v=(p.pt->N*0.5f+Vector3D(0.5f,0.5f,0.5f))*255.0f;
                        Vector4D v4=v;
                        v4.w=p.pt->Freeze*255.0f;
						vt->specular=V4D2DW(v4);
						vt++;
					}
				}
			}		
			NRemains--;
			if(NRemains==0){
				bm->setNVert(bm->getMaxVert());
				bm->setNInd(bm->getMaxInd());
				bm->setNPri(bm->getNInd()/6);
				bm->setShader(NrmShader);
				if(!NormOnly)IRS->PushRenderTarget(NormalsRT);
				DrawBM(*bm);
                if(!NormOnly){
				    IRS->PopRenderTarget();
				    IRS->PushRenderTarget(ColorsRT);                
				    bm->setShader(ClrShader);
				    DrawBM(*bm);
				    IRS->PopRenderTarget();
                }
				NRemains=NFacesInPass;
				vt=(VertexTS*)bm->getVertexData();
			}
		}
	}scan_end;
	if(NRemains!=NFacesInPass){
		int n=NFacesInPass-NRemains;
		bm->setNVert((SubSurfaceSize+1+D)*(SubSurfaceSize+1+D)*n);
		bm->setNInd((SubSurfaceSize+D)*(SubSurfaceSize+D)*6*n);
		bm->setNPri(bm->getNInd()/6);
		bm->setShader(NrmShader);
		if(!NormOnly)IRS->PushRenderTarget(NormalsRT);
		DrawBM(*bm);
        if(!NormOnly){
            IRS->PopRenderTarget();
		    IRS->PushRenderTarget(ColorsRT);        
		    bm->setShader(ClrShader);
		    DrawBM(*bm);        
		    IRS->PopRenderTarget();
        }
	}
	DirtyFaces.reset();
    if(NormOnly){
        IRS->PopRenderTarget();
    }
}

float RayTri(Vector3D &RayOrig, Vector3D &RayDir, Vector3D &t0, Vector3D &t1, Vector3D &t2,float& u,float& v) {	
	Vector3D e1 = t1 - t0;
	Vector3D e2 = t2 - t0;
    Vector3D p;
    p.cross(RayDir,e2);
    float d = e1.dot(p);
	Vector3D to = RayOrig - t0;
	float id = 1.0 / d;
	Vector3D q;
    q.cross(to,e1);
	if(fabs(d) > 0){
		u = to.dot(p)/d;
		if(u < -0.001 || u > 1.001) return FLT_MAX;
		v = RayDir.dot(q)/d;
		if(v < -0.001 || u + v > 1.001) return FLT_MAX;
	}else return false;
	return e2.dot(q) * id;
}
PickInfo PolyMesh::Pick(){
    POINT Pt;
    GetCursorPos2(&Pt);
    ICamera* pCam = GetCamera();
    Line3D Ray=pCam->GetPickRay(Pt.x,Pt.y);        
    int n=0;
    int nb=0;
    Vector3D Best;
    float Dist=FMAX;
    //float U,V;
	DWORD BestFace;
    scan(FacesBB,ValidAABB* AB,tri_DWORD* Pos){        
		if(!AB->Valid){
			//need to recalculate AABB
            AB->Set(FMAX,FMAX,FMAX,-FMAX,-FMAX,-FMAX);
			scan_key(VertsInCells,*Pos,DWORD* vp){
				VertexBackup* vb=SurfaceModificator::Backup.get(*vp);
				if(vb){
					AB->Extend(vb->Pos*Scale);
				}else{
					Vector3D v=Verts[*vp].Pos*Scale;
					AB->Extend(v);
				}
			}scan_end;
            AB->minv-=Vector3D(10,10,10);
            AB->maxv+=Vector3D(10,10,10);
			AB->Valid=true;
		}
        if(AB->Overlap(Ray)){
			scan_key(FacesTreeL0,*Pos,DWORD* Face){
				int n=0;
				int v0=-1;
				int vp;
				scan_key(Faces,*Face,DWORD* V){                    
					int p=*V;
                    if(v0!=p){
					    if(v0==-1)v0=p;
					    if(n>=2){
						    VertexUV* uv1=&VertsUV[v0];
						    VertexUV* uv2=&VertsUV[vp];
						    VertexUV* uv3=&VertsUV[p];
						    VertexBackup* VB1=SurfaceModificator::Backup.get(uv1->PosIndex);
						    VertexBackup* VB2=SurfaceModificator::Backup.get(uv2->PosIndex);
						    VertexBackup* VB3=SurfaceModificator::Backup.get(uv3->PosIndex);
						    Vector3D pos1=VB1?VB1->Pos*Scale:Verts[uv1->PosIndex].Pos*Scale;
						    Vector3D pos2=VB2?VB2->Pos*Scale:Verts[uv2->PosIndex].Pos*Scale;
						    Vector3D pos3=VB3?VB3->Pos*Scale:Verts[uv3->PosIndex].Pos*Scale;
						    float u,v;
						    Vector3D org=Ray.getOrig();
						    Vector3D dir=Ray.getDir();
						    float d=RayTri(org,dir,pos1,pos2,pos3,u,v);
						    if(d>0 && d<Dist){
							    Dist=d;
							    Best=pos1+(pos2-pos1)*u+(pos3-pos1)*v;
							    BestFace=*Face;
						    }
					    }
					    vp=p;                
					    n++;
                    }
				}scan_end;
			}scan_end;
        }
        n++;
    }scan_end;
    LastPickInfo.Picked=false;
    if(Dist<FMAX){
        LastPickInfo.Pos=Best;
        LastPickInfo.Picked=true;
		LastPickInfo.FaceID=BestFace;
    }
    return LastPickInfo;
}
float GetPointToLineDist3D(Vector3D P,Vector3D P1,Vector3D P2,float& Weight){
	P-=P1;
	P2-=P1;
	float r=P.norm();
	float r12=P2.norm();
	if(r12>0){		
		float sp=P2.dot(P)/r12/r12;
		if(sp<0){
			Weight=0;
			return r;
		}
		if(sp>1){
			Weight=1;
			return P.distance(P2);
		}
		Weight=sp;
		Vector3D PC=P2*sp;
		return PC.distance(P);
	}
	Weight=0.5;
	return r;
}
Vector3D GetPointToLineDV(Vector3D P,Vector3D P1,Vector3D P2,float& Weight){
	P-=P1;
	P2-=P1;
	float r=P.norm();
	float r12=P2.norm();
	Vector3D dxy;
	if(r12>0){		
		float sp=P2.dot(P)/r12/r12;
		if(sp<0){
			dxy=P;
			Weight=0;
			return dxy;
		}
		if(sp>1){
            dxy=P-P2;
			Weight=1;
			return dxy;
		}
		Weight=sp;
		Vector3D PC=P2*sp;
		dxy=P-PC;
		return dxy;
	}
	dxy=P;
	Weight=0.5;
	return dxy;
}
float SurfaceModificator::GetDistToRode(Vector3D p){
	float W;
    return GetPointToLineDist3D(p,PrevPos,CurrPos,W);
}
Vector3D SurfaceModificator::GetDVToRode(Vector3D p,float& W){	
    return GetPointToLineDV(p,PrevPos,CurrPos,W);
}
Vector3D SurfaceModificator::GetDVToRodeP(Vector3D p,float& W){	
    return GetPointToLineDV(p,PrevPrevPos,PrevPos,W);
}
const int hashsize=121001;
void SurfaceModificator::CreateVolumePick(AABoundBox ABR){
    NearVerts.FastClear();
	NearFaces.FastClear();
	InsideVerts.FastClear();	
	
	static uni_hash<DWORD,DWORD,hashsize>	Verts;		
	static uni_hash<DWORD,DWORDS2,hashsize>	Edges;
	static uni_hash<DWORD,DWORD,hashsize>	InsVerts;
	Verts.fast_reset();
	Edges.fast_reset();
	InsVerts.fast_reset();
	
	scan(M->FacesBB,ValidAABB* AB,tri_DWORD* Key){
		if(AB->Overlap(ABR)){		
            tri_DWORD& D=*Key;
			scan_key(M->FacesTreeL0,D,DWORD* Face){
				bool AtLeastOneVert=false;
                int v0=-1;
				DWORD Prev=-1;
				scan_key(M->Faces,*Face,DWORD* Vert){
					VertexUV*  uv=&M->VertsUV[*Vert];
					DWORD vc=uv->PosIndex;
                    if(*Vert!=v0){
                        if(v0==-1)v0=*Vert;						    
						VertexPos* vp=&M->Verts[uv->PosIndex];
						Vector3D pos=vp->Pos*M->Scale;						    
						PickedVertInfo pi;						    
						AtLeastOneVert=true;
						if(!Verts.get(uv->PosIndex)){
							Verts.add(uv->PosIndex,uv->PosIndex);								
							pi.N=M->VertsN[uv->NIndex];
							pi.Pos=pos;
							pi.PosVertexID=uv->PosIndex;
							pi.NVertexID=uv->NIndex;
							pi.UVVertexID=*Vert;
							NearVerts.Add(pi);
							InsVerts.add_once(uv->PosIndex,uv->PosIndex);
						}
                    }
					if(Prev!=-1){
                        Edges.add_uniq(DWORDS2(Prev,vc),*Face);
					}
					Prev=vc;
				}scan_end;
				if(AtLeastOneVert){
                    NearFaces.Add(*Face);
				}
			}scan_end;				
		}
	}scan_end;
	scan(Edges,DWORD* Face,DWORDS2* Edge){
		if(Edges.size(*Edge)!=2){
			InsVerts.del(Edge->V1);
			InsVerts.del(Edge->V2);
		}
	}scan_end;
	scan(InsVerts,DWORD* Vert,DWORD* _Vert){
        InsideVerts.Add(*Vert);
	}scan_end;	
}
Vector3D SurfaceModificator::CreateVertList(){
    Vector3D N0(0,0,0);
	NearVerts.FastClear();
	NearFaces.FastClear();
	InsideVerts.FastClear();
	if(Pick->Picked){
		float rm=Radius;
		if(GeneralPen::PenPressureType!=0)rm=GeneralPen::PenRadius;
        if(rm<4)rm=4;
		static uni_hash<DWORD,DWORD,hashsize>	Verts;		
		static uni_hash<DWORD,DWORDS2,hashsize>	Edges;
		static uni_hash<DWORD,DWORD,hashsize>	InsVerts;
		Verts.fast_reset();
		Edges.fast_reset();
		InsVerts.fast_reset();

		AABoundBox ABR(CenterPos,InfRadius);
		scan(M->FacesBB,ValidAABB* AB,tri_DWORD* Key){
			if(AB->Overlap(ABR)){		
                tri_DWORD& D=*Key;
				scan_key(M->FacesTreeL0,D,DWORD* Face){
					bool AtLeastOneVert=false;
                    int v0=-1;
					DWORD Prev=-1;
					scan_key(M->Faces,*Face,DWORD* Vert){
						VertexUV*  uv=&M->VertsUV[*Vert];
						DWORD vc=uv->PosIndex;
                        if(*Vert!=v0){
                            if(v0==-1)v0=*Vert;						    
						    VertexPos* vp=&M->Verts[uv->PosIndex];
						    Vector3D pos=vp->Pos*M->Scale;
						    //float D=pos.distance(CenterPos);
						    PickedVertInfo pi;
						    VertexBackup* VB=Backup.get(uv->PosIndex);
						    if(VB){
							    pi.DistToCenter=GetDistToRode(VB->Pos*M->Scale);
						    }else pi.DistToCenter=GetDistToRode(pos);
						    if(pi.DistToCenter<rm){
							    AtLeastOneVert=true;
							    if(!Verts.get(uv->PosIndex)){
								    Verts.add(uv->PosIndex,uv->PosIndex);								
								    pi.N=M->VertsN[uv->NIndex];
								    pi.Pos=pos;
								    pi.PosVertexID=uv->PosIndex;
								    pi.NVertexID=uv->NIndex;
								    pi.UVVertexID=*Vert;
								    NearVerts.Add(pi);
									InsVerts.add_once(uv->PosIndex,uv->PosIndex);
                                    N0+=pi.N;
							    }
						    }						
                        }                 
						if(Prev!=-1){
                            Edges.add_uniq(DWORDS2(Prev,vc),*Face);							
						}
						Prev=vc;
					}scan_end;
					if(AtLeastOneVert){
                        NearFaces.Add(*Face);
					}
				}scan_end;				
			}
		}scan_end;
		scan(Edges,DWORD* Face,DWORDS2* Edge){
			if(Edges.size(*Edge)!=2){
				InsVerts.del(Edge->V1);
				InsVerts.del(Edge->V2);
			}
		}scan_end;
		scan(InsVerts,DWORD* Vert,DWORD* _Vert){
            InsideVerts.Add(*Vert);
		}scan_end;
        if(N0.norm()==0){
            N0=Vector3D(0,1,0);
        }else{
            N0.normalize();
        }       
	}
    return N0;
}
void PolyMesh::CreateFaceNormals(DWORD Face,bool InitN0){
    FaceInfo* fi=&FacesInfo[Face];
    if(fi->Points){
        int ofs=0;
        PointInfo pinf;        
        pinf.Face=Face;
        pinf.pt=fi->Points;

		Vector3D v[4];
		Vector3D n[4];
		memset(&v,0,sizeof v);
		memset(&n,0,sizeof n);

		int half=SubSurfaceSize>>1;
        for(int y=0;y<SubSurfaceSize;y++){
            for(int x=0;x<SubSurfaceSize;x++){               
                pinf.x=x;
                pinf.y=y;
                pinf.Face=Face;                
                CalcNormal(pinf);
				if(InitN0)pinf.pt->N0=pinf.pt->N;
				int p=y<half? (x<half?0:1) : (x<half?3:2);				
				if((x==0 || x==SubSurfaceSize-1) && (y==0 ||y==SubSurfaceSize-1))v[p]+=pinf.pt->Pos;
				n[p]+=pinf.pt->N;                
				ofs++;
                pinf.pt++;
            }
        }		
		int v0=-1;
		int p=0;
		float w=1;//half*half;
		scan_key(Faces,Face,DWORD* Vert){
			if(*Vert!=v0){
				VertexUV* uv=&VertsUV[*Vert];
				VertexPos* vp=&Verts[uv->PosIndex];
				if(vp->TempSelection){				
					vp->Pos+=v[p]*1000;
					vp->W+=w*1000;					
					VertsN[uv->NIndex]+=n[p]*16384.0f;					
				}
			}
			if(v0==-1)v0=*Vert;
			p++;
		}scan_end;        
    }
}
void SurfaceModificator::RecreateNormals(){    
    
}
SurfaceModificator::SurfaceModificator(){	
	Pick=NULL;
	M=NULL;
}
void SurfaceModificator::Set(PolyMesh* Mesh,float R){
	if(Mesh->LastPickInfo.Picked){
		M=Mesh;        
        if(!Pick){            
            PrevPos=M->LastPickInfo.Pos;
            PrevPrevPos=PrevPos;
            PrevR=R;
        }
        else{
            PrevPrevPos=PrevPos;
            PrevPos=CurrPos;
            PrevR=R;
        }
		Pick=&Mesh->LastPickInfo;		
		CurrPos=M->LastPickInfo.Pos;
		float R2=CurrPos.distance(PrevPos);
		InfRadius=GeneralPen::PenRadius*1.3+R2/2;
		Radius=R;		
		CenterPos=(PrevPos+CurrPos)*0.5f;

        if(GeneralPen::SymmetryMask){
            int xm=GeneralPen::SymmetryMask&1?1:0;
				int ym=GeneralPen::SymmetryMask&2?1:0;
				int zm=GeneralPen::SymmetryMask&4?1:0;
				for(int x=0;x<=xm;x++){
					for(int y=0;y<=ym;y++){
						for(int z=0;z<=zm;z++){                            
							if(x||y||z){
                                int ofs=x+y*2+z*4;
								Vector3D mul(x?-1:1,y?-1:1,z?-1:1);
                                Vector3D vc=PMS.LastPickInfo.Pos;
								GeneralPen::TransformSymm(vc,mul);
                                Vector3D dir=SurfaceModificator::LastNormal;
                                dir*=mul;
                                vc=Mesh->PutPointOnSurface(vc,dir);                                
                                PrevPrevPosS[ofs]=PrevPosS[ofs];
                                PrevPosS[ofs]=CurrPosS[ofs];
                                CurrPosS[ofs]=vc;
                                CenterPosS[ofs]=(PrevPosS[ofs]+CurrPosS[ofs])*0.5f;
							}
						}
					}
				}
        }
		//if(InfRadius>Radius*2)InfRadius=Radius*2;
	}else{
		Pick=NULL;
		M=NULL;
	}
}
void SurfaceModificator::KillBackup(){
	if(Backup.size()){		
		scan(Backup,VertexBackup* VB,DWORD* VertID){
			tri_DWORD* TD=M->VertsInfluence.get(*VertID);
			if(TD){
				ValidAABB* AB=M->FacesBB.get(*TD);
				if(AB){
					AB->Valid=false;
				}
			}
		}scan_end;
		Backup.reset();
	}
	M->DropFacesHistoryToUndo();
}
void SurfaceModificator::GetTNB(Vector3D& T,Vector3D& N,Vector3D& B){
    Vector3D N0=GetCamera()->GetDir()*(-1);
    /*
	for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
		VertexBackup* VB=Backup.get(pi.PosVertexID);
		if(VB)N0+=VB->N;
		else{
			N0+=pi.N;			
		}
	}
    */
	N0.normalize();
	Vector3D T0=Vector3D::oZ;
    //if(PrevPos.distance(CurrPos)>0){
    //    T0=CurrPos-PrevPos;
    //    T0.normalize();
    //}
	T0=T0-N0*N0.dot(T0);
	T0.normalize();	
	B.cross(N0,T0);
    T=T0*cos(GeneralPen::PenRotation)-B*sin(GeneralPen::PenRotation);
    B=B*cos(GeneralPen::PenRotation)+T0*sin(GeneralPen::PenRotation);
    N=N0;
}

uni_hash<Vector3D*> SurfaceModificator::SculptHash;
void SurfaceModificator::ClearSculptHash(){
    scan(SculptHash,Vector3D** v,DWORD* ID){
        delete[](*v);
    }scan_end;
    SculptHash.fast_reset();
}
void SurfaceModificator::ApplySculptModification(fnVertModificator* fn,float SmoothDeg){
    static uni_hash<Vector3D> NewPos;
    static uni_hash<Vector4D> AvgPos;
    static uni_hash<Vector3D> NewN;
	static uni_hash<Vector3D> OldN;

    NewPos.fast_reset();
    AvgPos.fast_reset();
    NewN.fast_reset();
	OldN.fast_reset();

    Vector3D N0(0,0,0);
	for(int i=0;i<InsideVerts.GetAmount();i++){
		M->Verts[InsideVerts[i]].TempSelection=1;
	}
	for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
		VertexBackup* VB=Backup.get(pi.PosVertexID);
		if(VB)N0+=VB->N;
		else{
			N0+=pi.N;
			VertexBackup vb;
			vb.N0=vb.N=pi.N;
            vb.Pos=pi.Pos;
			vb.Pos/=M->Scale;
			Backup.add(pi.PosVertexID,vb);			
		}
	}
    N0.normalize();
	//modifying position
    for(int i=0;i<InsideVerts.GetAmount();i++){		
		Vector3D V1=M->Verts[InsideVerts[i]].Pos;
        float smd=1;
        if(fn)fn(V1,smd,N0,M);
        M->Verts[InsideVerts[i]].SelectionDegree=smd*255;
        NewPos.add(InsideVerts[i],V1);        		
	}
	//smoothing
	for(int i=0;i<NearFaces.GetAmount();i++){
		DWORD Face=NearFaces[i];				
		Vector3D   pos[5];
		DWORD	   pts[5];
		int		   p=0;
		scan_key(M->Faces,Face,DWORD* Vert){
			VertexUV* uv=&M->VertsUV[*Vert];
			Vector3D* v=NewPos.get(uv->PosIndex);
			pts[p]=uv->PosIndex;
			Vector3D av=v?*v:M->Verts[uv->PosIndex].Pos;
            pos[p]=av;				
			p++;
		}scan_end;
        for(int i=0;i<4;i++)if(M->Verts[pts[i]].TempSelection){
			Vector3D vp=pos[(i+3)&3];
			Vector3D vc=pos[i];
			Vector3D vn=pos[(i+1)&3];			
			Vector4D* vv=AvgPos.get(pts[i]);
			if(vv){
				Vector4D v1=vp;
				v1.w=1;
				*vv+=v1;
				v1=vn;
				*vv+=v1;
			}else{
				Vector4D v1=vp;
				v1.w=1;
				vv=AvgPos.add(pts[i],v1);
				v1=vn;
				v1.w=1;
				*vv+=vn;
			}			
		}
	}
	scan(AvgPos,Vector4D* apos,DWORD* pos){
		apos->normW();
        float smd=float(SmoothDeg*M->Verts[*pos].SelectionDegree)/255.0f;
		*apos*=smd;
		Vector4D v1=M->Verts[*pos].Pos;
		Vector3D* v2=NewPos.get(*pos);
		if(v2)v1=*v2;
		v1*=(1-smd);
		v1.w=0;
		*apos+=v1;				
		apos->w=1;		
	}scan_end;
	//calc normals
    for(int i=0;i<NearFaces.GetAmount();i++){
        DWORD Face=NearFaces[i];
        DWORD pts[5];
		DWORD npts[5];
        Vector3D pos[5];
		Vector3D posNew[5];
        bool present[5];
		int p=0;
        scan_key(M->Faces,Face,DWORD* Vert){
            VertexUV* uv=&M->VertsUV[*Vert];
            pts[p]=uv->PosIndex;
			npts[p]=uv->NIndex;
			pos[p]=M->Verts[uv->PosIndex].Pos;			
			Vector4D* v=NULL;
			if(M->Verts[uv->PosIndex].TempSelection){
				v=AvgPos.get(uv->PosIndex);
				//assert(v);
			}
			posNew[p]=v?*v:M->Verts[uv->PosIndex].Pos;
            p++;
        }scan_end;
        for(int i=0;i<4;i++)if(M->Verts[pts[i]].TempSelection){
            Vector3D vp=pos[(i+3)&3];
			Vector3D vc=pos[i];
			Vector3D vn=pos[(i+1)&3];
			vp-=vc;
			vn-=vc;
			vc.cross(vp,vn);

			Vector3D vpN=posNew[(i+3)&3];
			Vector3D vcN=posNew[i];
			Vector3D vnN=posNew[(i+1)&3];
			vpN-=vcN;
			vnN-=vcN;
			vcN.cross(vpN,vnN);

            Vector3D* N1=OldN.get(npts[i]);
			Vector3D* N2=NewN.get(npts[i]);

			if(N1)*N1+=vc;
			else OldN.add(npts[i],vc);
			if(N2)*N2+=vcN;
			else NewN.add(npts[i],vcN);
        }
    }
	int nn=0;
	scan(NewN,Vector3D* n,DWORD* pos){
		n->normalize();
		nn++;
	}scan_end;
	int no=0;
	scan(OldN,Vector3D* n,DWORD* pos){
		n->normalize();
		no++;
	}scan_end;	   
	for(int i=0;i<NearFaces.GetAmount();i++){
		DWORD Face=NearFaces[i];
		M->DropFaceToUndo(Face);
		FaceInfo* fi=&M->FacesInfo[Face];
		if(fi->Points){
            LayerOnFace* LF=M->GetCurrentLayer(Face);
            OneLayerPoint* lp=LF->Points;
            int size=M->SubSurfaceSize;
            Vector3D** pvv=SculptHash.get(Face);
            Vector3D* pv=NULL;
            if(pvv)pv=*pvv;
            else{
                pv=new Vector3D[size*size];
                SculptHash.add(Face,pv);
                DWORD pts[5];
                Vector3D P [5];                 
                Vector3D N[5];                
                float u[5];
                float v[5];
                int p=0;
                scan_key(M->Faces,Face,DWORD* Vert){
                    VertexUV* uv=&M->VertsUV[*Vert];
                    pts[p]=uv->PosIndex;
                    P[p]=M->Verts[uv->PosIndex].Pos;                
                    N[p]=M->VertsN[uv->NIndex];
                    u[p]=uv->u;
                    v[p]=uv->v;
					N[p].normalize();
                    p++;
                }scan_end;

                FaceInfo* fi=&M->FacesInfo[Face];
                SurfPoint3D* sp=fi->Points;
                int ps=0;
                float d=1.0f/size;

                Vector3D T[4];
                Vector3D B[4];               

                Vector3D t,b;

			    int idx1[]={1,1,2,2};
			    int idx2[]={0,0,3,3};
			    int idx3[]={3,2,2,3};
			    int idx4[]={0,1,1,0};
			    for(int q=0;q<4;q++){
				    Vector3D DT1=P[idx1[q]]-P[idx2[q]];				
				    float LT1=DT1.norm();
				    Vector3D DB1=P[idx3[q]]-P[idx4[q]];				
				    float LB1=DB1.norm();
				    t.cross(P[idx1[q]]-P[idx2[q]],N[q]);
				    b.cross(P[idx3[q]]-P[idx4[q]],N[q]);
				    T[q].cross(N[q],t);
				    B[q].cross(N[q],b);				
    				
				    //T[q].normalize();
				    //T[q]*=LT1;
				    //B[q].normalize();
				    //B[q]*=LB1;						
			    } 				
				
                for(int y=0;y<size;y++){
                    float dy=(y+0.5f)*d;
                    float y2 = dy*dy;
	                float y3 = y2*dy;

                    float KY1 = (2 * y3 - 3 * y2 + 1);
                    float KY2 = (- 2 * y3 + 3 * y2);
                    float KY3 = (y3 - 2 * y2 + dy);
                    float KY4 = (y3 - y2);                

	                Vector3D P1 = KY1 * P[0] + KY2 * P[3] + KY3 * B[0] + KY4 * B[3];
                    Vector3D P2 = KY1 * P[1] + KY2 * P[2] + KY3 * B[1] + KY4 * B[2];                                    

                    Vector3D T1 = T[0]*(1-dy) + T[3]*dy;
                    Vector3D T2 = T[1]*(1-dy) + T[2]*dy;                    

                    for(int x=0;x<size;x++){
					    float dx=(x+0.5f)*d;
                        float x2 = dx*dx;
	                    float x3 = x2*dx;
                        Vector3D v0=(2*x3 - 3*x2 + 1)*P1 + (-2*x3 + 3*x2)*P2 + (x3 - 2*x2 + dx)*T1 + (x3 - x2)*T2;                        
                        pv[ps] = sp[ps].Pos-v0;
                        ps++;                        
                    }
                }				
            }
            DWORD pts[5];
            Vector3D P [5]; 
            Vector3D Pn[5]; 
            Vector3D N[5];
            Vector3D Nn[5];
            float u[5];
            float v[5];
            int p=0;
            scan_key(M->Faces,Face,DWORD* Vert){
                VertexUV* uv=&M->VertsUV[*Vert];
                pts[p]=uv->PosIndex;
                P[p]=M->Verts[uv->PosIndex].Pos;                
                N[p]=M->VertsN[uv->NIndex];
                u[p]=uv->u;
                v[p]=uv->v;
				Vector4D* vv=AvgPos.get(uv->PosIndex);
				Pn[p]=vv?*vv:P[p];
				Vector3D* vnN=NewN.get(uv->NIndex);
				Nn[p]=vnN?*vnN:N[p];
				Vector3D* vn=OldN.get(uv->NIndex);
				if(vn)N[p]=*vn;
				//N[p].normalize();
				Nn[p].normalize();
                p++;				
            }scan_end;

            FaceInfo* fi=&M->FacesInfo[Face];
            SurfPoint3D* sp=fi->Points;
            int ps=0;
            float d=1.0f/size;            

            Vector3D Tn[4];
            Vector3D Bn[4];

            Vector3D t,b;

			int idx1[]={1,1,2,2};
			int idx2[]={0,0,3,3};
			int idx3[]={3,2,2,3};
			int idx4[]={0,1,1,0};

			for(int q=0;q<4;q++){
				Vector3D DT1=P[idx1[q]]-P[idx2[q]];				
				float LT1=DT1.norm();
				Vector3D DB1=P[idx3[q]]-P[idx4[q]];				
				float LB1=DB1.norm();
				t.cross(Pn[idx1[q]]-Pn[idx2[q]],Nn[q]);
				b.cross(Pn[idx3[q]]-Pn[idx4[q]],Nn[q]);				
				Tn[q].cross(Nn[q],t);
				Bn[q].cross(Nn[q],b);				    			
				/*
				Tn[q].normalize();
				Tn[q]*=LT1;
				Bn[q].normalize();
				Bn[q]*=LB1;
				*/                    							
			}   
			for(int y=0;y<size;y++){
				float dy=(y+0.5f)*d;
				float y2 = dy*dy;
				float y3 = y2*dy;
				float KY1 = (2 * y3 - 3 * y2 + 1);
				float KY2 = (- 2 * y3 + 3 * y2);
				float KY3 = (y3 - 2 * y2 + dy);
				float KY4 = (y3 - y2);
				Vector3D P1 = KY1 * Pn[0] + KY2 * Pn[3] + KY3 * Bn[0] + KY4 * Bn[3];
				Vector3D P2 = KY1 * Pn[1] + KY2 * Pn[2] + KY3 * Bn[1] + KY4 * Bn[2];                
				Vector3D T1 = Tn[0]*(1-dy) + Tn[3]*dy;
				Vector3D T2 = Tn[1]*(1-dy) + Tn[2]*dy;
				for(int x=0;x<size;x++){
					float dx=(x+0.5f)*d;
					float x2 = dx*dx;
					float x3 = x2*dx;
					Vector3D v0= (2*x3 - 3*x2 + 1)*P1 + (-2*x3 + 3*x2)*P2 + (x3 - 2*x2 + dx)*T1 + (x3 - x2)*T2;                        
                    Vector3D V2 = v0+pv[ps];
					//sp[ps].Pos = v0+pv[ps];
                    lp->Dv += V2-sp[ps].Pos;
                    lp++;
					ps++;
				}
			}
			/*
            for(int y=0;y<size;y++){
                float dy=(y+0.5f)*d;
                float y2 = dy*dy;
	            float y3 = y2*dy;

                float KY1 = (2 * y3 - 3 * y2 + 1);
                float KY2 = (- 2 * y3 + 3 * y2);
                float KY3 = (y3 - 2 * y2 + dy);
                float KY4 = (y3 - y2);
                Vector3D P1n = KY1 * Pn[0] + KY2 * Pn[3] + KY3 * Bn[0] + KY4 * Bn[3];
                Vector3D P2n = KY1 * Pn[1] + KY2 * Pn[2] + KY3 * Bn[1] + KY4 * Bn[2];
                Vector3D T1n = Tn[0]*(1-dy) + Tn[3]*dy;
                Vector3D T2n = Tn[1]*(1-dy) + Tn[2]*dy;
                for(int x=0;x<size;x++){
					float dx=(x+0.5f)*d;
                    float x2 = dx*dx;
	                float x3 = x2*dx;
                    Vector3D v1=(2*x3 - 3*x2 + 1)*P1n + (-2*x3 + 3*x2)*P2n + (x3 - 2*x2 + dx)*T1n + (x3 - x2)*T2n;
                    sp[ps].Pos = v1+pv[ps];
                    ps++;
                }
            }
			*/
		}
	}
	for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
		M->InvalidatePosVertex(pi.PosVertexID);
	}
    for(int i=0;i<NearFaces.GetAmount();i++){
		DWORD Face=NearFaces[i];
        M->CalcFaceAttributesWithLayers(Face);		
	}
	for(int i=0;i<NearFaces.GetAmount();i++){
		DWORD Face=NearFaces[i];
		M->CreateFaceNormals(Face);
		M->AddDirtyFace(Face);
	}
	for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];		
		VertexPos* vp=&M->Verts[pi.PosVertexID];		
		float w0=vp->W;
		vp->Pos/=vp->W;
		M->VertsN[pi.NVertexID].normalize();
		vp->W=1;			
	}
	for(int i=0;i<InsideVerts.GetAmount();i++){
		VertexPos* vp=&M->Verts[InsideVerts[i]];
		vp->TempSelection=0;
		vp->Pos/=vp->W;
		vp->W=1;
	}
}
void SurfaceModificator::ApplyModFunfcion(fnModificator* fn,bool Undo){
    Vector3D N0(0,0,0);
	for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
		VertexBackup* VB=Backup.get(pi.PosVertexID);
		if(VB)N0+=VB->N;
		else{
			N0+=pi.N;
			VertexBackup vb;
			vb.N0=vb.N=pi.N;
            vb.Pos=pi.Pos;
			vb.Pos/=M->Scale;
			Backup.add(pi.PosVertexID,vb);			
		}
	}
	for(int i=0;i<InsideVerts.GetAmount();i++){
		M->Verts[InsideVerts[i]].TempSelection=1;
	}

	N0.normalize();
	Vector3D T0=Vector3D::oZ;
    T0=T0-N0*N0.dot(T0);
	T0.normalize();
	Vector3D B;
	B.cross(N0,T0);
    Vector3D T=T0*cos(GeneralPen::PenRotation)-B*sin(GeneralPen::PenRotation);
    B=B*cos(GeneralPen::PenRotation)+T0*sin(GeneralPen::PenRotation);
    
    float sl=CurrPos.distance(PrevPos);
	for(int i=0;i<NearFaces.GetAmount();i++){		
		DWORD Face=NearFaces[i];
		if(Undo)M->DropFaceToUndo(Face);
		FaceInfo* fi=&M->FacesInfo[Face];
		if(fi->Points){
			ModPointInfo mpi;	
            LayerOnFace* LF=M->GetCurrentLayer(Face);
            OneLayerPoint* pi=LF->Points;
			OneLayerPoint* pi0=M->GetFaceHistory(Face);
            if(!pi0)pi0=pi;
            mpi.Base=fi->Points;
			mpi.Curr=pi;
			mpi.Orig=pi0;
			mpi.Face=Face;
			mpi.N=N0;
            mpi.xalong=0;
            mpi.yalong=0;
			if(pi0){
				int sz=M->SubSurfaceSize;
				for(int y=0;y<sz;y++){
					for(int x=0;x<sz;x++){						
						mpi.x=x;
						mpi.y=y;
                        float w=0;
                        Vector3D pos=mpi.Base->Pos;
                        pos-=(pi->Dv-pi0->Dv)*LF->Info->DepthTransparency;
                        Vector3D D=GetDVToRode(pos*M->Scale,w);                        
                        float R1=PrevR*(1-w)+Radius*w;

						float r=R1;
						float r1=GeneralPen::PenPressureType==0 ? R1 : GeneralPen::PenRadius;

						mpi.dx=D.dot(T)/r1*1.2;
						mpi.dy=D.dot(B)/r1*1.2;
						float dz=D.dot(N0)/r1*1.2;
						dz=1-dz*dz;
						if(dz>0){							
							mpi.StrokeLength=0;
							fn(M->Scale,mpi,r1/M->Scale/1.2,dz*r/M->Scale/1.2);
						}
						pi++;
						pi0++;
						mpi.Curr++;
						mpi.Orig++;
                        mpi.Base++;
					}
				}
			}
		}
	}
	for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
		M->InvalidatePosVertex(pi.PosVertexID);
	}
    for(int i=0;i<NearFaces.GetAmount();i++){
		DWORD Face=NearFaces[i];
        M->CalcFaceAttributesWithLayers(Face);		
	}
	for(int i=0;i<NearFaces.GetAmount();i++){
		DWORD Face=NearFaces[i];
		M->CreateFaceNormals(Face);
		M->AddDirtyFace(Face);
	}
	for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
		//M->VertsN[pi.NVertexID].normW();
		VertexPos* vp=&M->Verts[pi.PosVertexID];		
		vp->Pos/=vp->W;
		M->VertsN[pi.NVertexID].normalize();
		vp->W=1;		
	}
	for(int i=0;i<InsideVerts.GetAmount();i++){
		VertexPos* vp=&M->Verts[InsideVerts[i]];
		vp->TempSelection=0;
		vp->Pos/=vp->W;
		vp->W=1;
	}	
}
void SurfaceModificator::ApplyToAll(fnModificator* fn,bool ReCreateNormals){
    for(int i=0;i<M->FacesInfo.GetAmount();i++){		
		DWORD Face=i;
		M->DropFaceToUndo(Face);
		FaceInfo* fi=&M->FacesInfo[Face];
		if(fi->Points){
			ModPointInfo mpi;			
            OneLayerPoint* pi=M->GetCurrentLayer(Face)->Points;
			OneLayerPoint* pi0=M->GetFaceHistory(Face);
            mpi.Base=fi->Points;
			mpi.Curr=pi;
			mpi.Orig=pi0;
			mpi.Face=Face;            
            mpi.xalong=0;
            mpi.yalong=0;
			if(pi0){
				int sz=M->SubSurfaceSize;
				for(int y=0;y<sz;y++){
					for(int x=0;x<sz;x++){						
						mpi.x=x;
						mpi.y=y;
                        mpi.N=mpi.Base->N0;
                        float w=0;
						Vector3D D=GetDVToRode(mpi.Base->Pos*M->Scale,w);                        
                        float R1=PrevR*(1-w)+Radius*w;

						float r=R1;
						float r1=GeneralPen::PenPressureType==0 ? R1 : GeneralPen::PenRadius;

						mpi.dx=0;
						mpi.dy=0;
                        mpi.StrokeLength=0;
                        fn(M->Scale,mpi,r1/M->Scale/1.2,r/M->Scale/1.2);
						pi++;
						pi0++;
						mpi.Curr++;
						mpi.Orig++;
                        mpi.Base++;
					}
				}
			}
		}        
	}    
    for(int Face=0;Face<M->FacesInfo.GetAmount();Face++){
        M->CalcFaceAttributesWithLayers(Face);
    }
    for(int Face=0;Face<M->FacesInfo.GetAmount();Face++){		
		if(ReCreateNormals)M->CreateFaceNormals(Face);
		M->AddDirtyFace(Face);
	}
}
void DrawingModificator::Apply(){
    ApplyModFunfcion(&GeneralPen::ModifyPoint);
}
float SurfaceModificator::GetDegree(Vector3D pos,float DistToCenter,Vector4D& Color,float& ColorDegree,float& SpecularDegree){
	float Deg=(Radius-DistToCenter)/Radius;
	if(Deg>0){		
		Deg=Deg*Deg*(2-Deg)*(2-Deg);
		if(TP){
			Vector3D vp=pos*M->Scale;
			TP->ToUV(vp);
			Color=TP->getPixelUV(vp.x,vp.y);
			ColorDegree=Deg;
			Deg*=Color.y/256.0f;			
		}else ColorDegree=0;
		return Deg;
	}
	return 0;
}
void AdditiveModificator::Apply(){
    Vector3D N0(0,0,0);
	for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
		VertexBackup* VB=Backup.get(pi.PosVertexID);
		if(VB)N0+=VB->N;
		else N0+=pi.N;		
	}
	for(int i=0;i<InsideVerts.GetAmount();i++){
		M->Verts[InsideVerts[i]].TempSelection=1;
	}
	N0.normalize();
    for(int i=0;i<NearFaces.GetAmount();i++){		
        DWORD Face=NearFaces[i];
		M->DropFaceToUndo(Face);
        FaceInfo* fi=&M->FacesInfo[Face];
        if(fi->Points){
            SurfPoint3D* pi=fi->Points;
            int sz=M->SubSurfaceSize;
            for(int y=0;y<sz;y++){
                for(int x=0;x<sz;x++){
                    float D=GetDistToRode(pi->Pos*M->Scale);
					float ColorDegree=0;
					float SpeclDegree=0;
					Vector4D C;
                    float Deg=GetDegree(pi->Pos,D,C,ColorDegree,SpeclDegree)*(1-pi->Freeze);
                    if(Deg>0){
                        pi->Pos+=N0*0.01*Deg*Degree;
						if(ColorDegree>0){
							C.w=C.y;
							int W=ColorDegree*255;
							if(W>255)W=255;
							DWORD D=V4D2DW(C);
							pi->Color=MixDWORD(pi->Color,D,255-W,W);
						}
                    }
                    pi++;
                }
            }
        }
    }
    for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
        M->InvalidatePosVertex(pi.PosVertexID);
	}
    for(int i=0;i<NearFaces.GetAmount();i++){
        DWORD Face=NearFaces[i];
        M->CreateFaceNormals(Face);
		M->AddDirtyFace(Face);
    }
    for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
		//M->VertsN[pi.NVertexID].normW();
		VertexPos* vp=&M->Verts[pi.PosVertexID];
		vp->Pos/=vp->W;
		vp->W=1;		
	}
	for(int i=0;i<InsideVerts.GetAmount();i++){
		VertexPos* vp=&M->Verts[InsideVerts[i]];
		vp->TempSelection=0;
		vp->Pos/=vp->W;
		vp->W=1;
	}    
	RecreateNormals();
}
struct SmoothStruct{
    Vector3D Summ;
    int n;
};
void SmoothModificator::Apply(){	
    for(int i=0;i<InsideVerts.GetAmount();i++){
		M->Verts[InsideVerts[i]].TempSelection=1;
	}	
    bool clr=GeneralPen::EnableChannel[1];
    bool spec=GeneralPen::EnableChannel[3];
    bool ch=clr||spec;
    for(int i=0;i<NearFaces.GetAmount();i++){
        DWORD Face=NearFaces[i];
		M->DropFaceToUndo(Face);
        FaceInfo* fi=&M->FacesInfo[Face];
        LayerOnFace* LF=M->GetCurrentLayer(Face);
        if(fi->Points){
            SurfPoint3D* pi=fi->Points;
            OneLayerPoint* lp=LF->Points;
            int sz=M->SubSurfaceSize;
            for(int y=0;y<sz;y++){
                for(int x=0;x<sz;x++){
					float D=GetDistToRode(pi->Pos*M->Scale);
                    float deg=D/GeneralPen::PenRadius;
					deg=(1-pi->Freeze)*(1-deg*deg)*GeneralPen::SmoothDegree;
					if(D<GeneralPen::PenRadius){
						Vector4D CL;
						Vector3D V2=M->GetAveragePNC(Face,x,y,4,NULL,ch?&CL:NULL);
						if(GeneralPen::EnableChannel[0]){//geometry change is allowed                    
                            lp->Dv=lp->Dv*(1-deg)+V2*deg;
						}
						if(ch){//color change is allowed                 							

							Vector4D V0=DW2V4D(pi->Color);
                            Vector4D VC=V0;
							Vector4D V=CL;

							VC*=1-deg;
                            V*=deg;
                            V+=VC;

							if(!clr){
								V.x=V0.x;
								V.y=V0.y;
								V.z=V0.z;
							}
							if(!spec){
								V.w=V0.w;
							}
							lp->Color=V4D2DW(V);
						}					
					}
                    pi++;
                    lp++;
                }
            }
        }
    }
    for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];
        M->InvalidatePosVertex(pi.PosVertexID);
	}
    for(int i=0;i<NearFaces.GetAmount();i++){
        DWORD Face=NearFaces[i];
        M->CalcFaceAttributesWithLayers(Face);		
    }
    for(int i=0;i<NearFaces.GetAmount();i++){
        DWORD Face=NearFaces[i];
        M->CreateFaceNormals(Face);
		M->AddDirtyFace(Face);
    }
    for(int i=0;i<NearVerts.GetAmount();i++){
		PickedVertInfo& pi=NearVerts[i];		
		VertexPos* vp=&M->Verts[pi.PosVertexID];
		vp->Pos/=vp->W;
		vp->W=1;		
	}
	for(int i=0;i<InsideVerts.GetAmount();i++){
		VertexPos* vp=&M->Verts[InsideVerts[i]];
		vp->TempSelection=0;
		vp->Pos/=vp->W;
		vp->W=1;
	}    
	RecreateNormals();
}
void TexArray::OnPoint(int x,int y,float z,float u,float v){
    TexPixel tp;
    tp.U=x;
    tp.V=y;
    tp.WorldPos=v0+(v1-v0)*u+(v2-v0)*v;
    Texels.Add(tp);
}
void SurfaceModificator::CreateTexPick(){
    if(!TexPick)TexPick=new TexArray;
    TexPick->Texels.FastClear();
    DynArray<DWORD> vlist;
    float MapSizeX=IRS->GetTextureWidth(M->TextureID);
    float MapSizeY=IRS->GetTextureHeight(M->TextureID);	
    for(int i=0;i<NearFaces.GetAmount();i++){
        vlist.FastClear();
        DWORD F=NearFaces[i];        
        scan_key(M->Faces,F,DWORD* Vert){
            vlist.Add(*Vert);
        }scan_end;        
        if(vlist.GetAmount()){
			vlist.pop_back();
            int n=vlist.GetAmount();            
            DWORD v0=vlist[0];
            VertexUV* uv0=&M->VertsUV[v0];
            VertexPos* p0=&M->Verts[uv0->PosIndex];
            for(int i=2;i<n;i++){
                DWORD v1=vlist[i-1];
                DWORD v2=vlist[i];
                VertexUV*  uv1 = &M->VertsUV [v1];
                VertexPos* p1  = &M->Verts   [uv1->PosIndex];
                VertexUV*  uv2 = &M->VertsUV [v2];
                VertexPos* p2  = &M->Verts   [uv2->PosIndex];
                TexPick->v0=p0->Pos;
                TexPick->v1=p1->Pos;
                TexPick->v2=p2->Pos;
                TexPick->Rasterize(Vector3D(uv0->u*MapSizeX,uv0->v*MapSizeY,0),Vector3D(uv1->u*MapSizeX,uv1->v*MapSizeY,0),Vector3D(uv2->u*MapSizeX,uv2->v*MapSizeY,0));
            }
        }
    }
}
DWORD MixDWORD(DWORD C1,DWORD C2,int W1,int W2){
	DWORD R=0;
	if(W1<0)W1=0;
	if(W2<0)W2=0;
	if(W1>255)W1=255;
	if(W2>255)W2=255;
	int DW=(W1+W2)/2;
	for(int i=0;i<4;i++){
		//R<<=8;
		int CH1=C1&255;
		int CH2=C2&255;
		C1>>=8;
		C2>>=8;
		int CC=(CH1*W1+CH2*W2+DW)>>8;
		if(CC>255)CC=255;
		R|=(CC<<(i<<3));
	}
	return R;
}
DWORD MulDWORD4F(DWORD C,Vector4D M,float Coef,float CoefA){
	float B=C&255;
	float G=(C>>8)&255;
	float R=(C>>16)&255;
	float A=(C>>24)&255;

	B*=M.x*Coef;
	G*=M.y*Coef;
	R*=M.z*Coef;
	A*=M.w*CoefA;

	if(B<0  )B=0;
	if(G<0  )G=0;
	if(R<0  )R=0;
	if(A<0  )A=0;
	if(B>255)B=255;
	if(G>255)G=255;
	if(R>255)R=255;
	if(A>255)A=255;
	
	return (DWORD(B))+(DWORD(G)<<8)+(DWORD(R)<<16)+(DWORD(A)<<24);
}
TexPointModificator::TexPointModificator(){
	Color=0xFFFF0000;
}
bool TexPointModificator::ModifyPoint(SurfaceModificator* sm,TexPixel* tp,DWORD* color,TexturePattern* TP){
	Vector3D V=sm->Pick->Pos;
	float dist=V.distance(tp->WorldPos*sm->M->Scale);
	dist/=sm->Radius;
	if(dist<1){	
		dist=1-dist;
		dist*=255*Influence;
		DWORD C=Color;
		if(TP){
			Vector3D v=tp->WorldPos*sm->M->Scale;
			TP->ToUV(v);
			Vector4D vc=TP->getPixelUV(v.x,v.y);
			C=MulDWORD4F(C,vc,2/256.0,1/256.0);
		}
		*color=MixDWORD(*color,C,255-dist,dist);
	}
	return true;
}

void SurfaceModificator::ApplyColor(TexPointModificator* tpm,bool ChangeColor,bool ChangeSpecular,TexturePattern* TP){
	if(M->TextureID){
		int Lx=IRS->GetTextureWidth(M->TextureID);
		int Ly=IRS->GetTextureHeight(M->TextureID);
		int Pitch;
		DWORD* data=(DWORD*)IRS->LockTexBits(M->TextureID,Pitch);
		if(data){
			Pitch>>=2;
			for(int i=0;i<TexPick->Texels.GetAmount();i++){
				TexPixel* tp=&TexPick->Texels[i];
				int x=tp->U;
				int y=tp->V;
				DWORD C=data[x+(Ly-y-1)*Pitch];
				DWORD& CL=data[x+(Ly-y-1)*Pitch];
				tpm->ModifyPoint(this,tp,&CL,TP);				
				if(!ChangeColor)CL=(C&0xFFFFFF)|(CL&0xFF000000);
				if(!ChangeSpecular)CL=(C&0xFF000000)|(CL&0xFFFFFF);
			}
			IRS->UnlockTexBits(M->TextureID);
		}
	}    
}
void PolyMesh::SelfTest(){
	scan(FacesOnEdges,DWORD* Face,DWORDS2* Key){
		int sz=FacesOnEdges.size(*Key);
		assert(sz==2);
	}scan_end;
	for(int i=0;i<FacesInfo.GetAmount();i++){
		int n=Faces.size(i);
		if(n>0){
			assert(*Faces.get(i,0)==*Faces.get(i,n-1));
		}
	}
}
int PolyMesh::GetFEA(){
	int n=0;
	scan(FacesOnEdges,DWORD* Face,DWORDS2* Key){
		int sz=FacesOnEdges.size(*Key);
        if(sz==1)n++;
		assert(sz<3);
	}scan_end;
	return n;
}
void PolyMesh::ApplyTexture(int TextureID){
	TexturePattern TP;
	TP.setTexture(TextureID);
	int n=FacesInfo.GetAmount();
	for(int i=0;i<n;i++){
		int sz=Faces.size(i);
		if(sz==5){
			DWORD pts[5];
			Vector3D P[5]; 
			Vector3D N[5];
			float u[5];
			float v[5];                
			int p=0;
			scan_key(Faces,i,DWORD* Vert){                
				VertexUV* uv=&VertsUV[*Vert];
				pts[p]=uv->PosIndex;
				P[p]=Verts[uv->PosIndex].Pos;
				N[p]=VertsN[uv->NIndex];
				N[p].normalize();
				u[p]=uv->u;
				v[p]=uv->v;
				p++;
			}scan_end;
			FaceInfo* fi=&FacesInfo[i];
			SurfPoint3D* sp=fi->Points;
			int ps=0;
			float d=1.0f/SubSurfaceSize;
			for(int y=0;y<SubSurfaceSize;y++){
				float dy=(y+0.5f)*d;
				for(int x=0;x<SubSurfaceSize;x++){
					float dx=(x+0.5f)*d;
					float uu=u[0]+(u[1]-u[0])*dx+(u[3]-u[0])*dy+(u[0]+u[2]-u[1]-u[3])*dx*dy;
					float vv=v[0]+(v[1]-v[0])*dx+(v[3]-v[0])*dy+(v[0]+v[2]-v[1]-v[3])*dx*dy;
					sp[ps].Color = V4D2DW(TP.getPixelUV(uu,1-vv));					
					ps++;
				}
			}			
		}
	}
    SetAllDirty();
}
void PolyMesh::CreateSubSurfaces(int size){
	for(int i=0;i<Verts.GetAmount();i++){
		Verts[i].TempSelection=0;
	}
	for(int i=0;i<VertsUV.GetAmount();i++){
		VertexUV* uv=&VertsUV[i];
		if(uv->PosIndex!=-1)Verts[uv->PosIndex].TempSelection++;
	}
    SubSurfaceSize=size;
    bool NeedDiv=false;
    int n=FacesInfo.GetAmount();
    AddLayer(true);
    for(int i=0;i<n;i++){
        int sz=Faces.size(i);
        if(sz==5){
            DWORD pts[5];
            Vector3D P[5]; 
            Vector3D N[5];
            float u[5];
            float v[5];                
            int p=0;
            LayerOnFace* LF=GetCurrentLayer(i);
            scan_key(Faces,i,DWORD* Vert){                
                VertexUV* uv=&VertsUV[*Vert];
                pts[p]=uv->PosIndex;
                P[p]=Verts[uv->PosIndex].Pos;
                N[p]=VertsN[uv->NIndex];
				N[p].normalize();
                u[p]=uv->u;
                v[p]=uv->v;
                p++;
            }scan_end;
            FaceInfo* fi=&FacesInfo[i];
            SurfPoint3D* sp=fi->Points=new SurfPoint3D[size*size];
            OneLayerPoint* lp=LF->Points;
            int ps=0;
            float d=1.0f/size;

            Vector3D T[4];
            Vector3D B[4];

            Vector3D t,b;

            t.cross(P[1]-P[0],N[0]);
            b.cross(P[3]-P[0],N[0]);
            T[0].cross(N[0],t);
            B[0].cross(N[0],b);
            t.cross(P[1]-P[0],N[1]);
            b.cross(P[2]-P[1],N[1]);
            T[1].cross(N[1],t);
            B[1].cross(N[1],b);
            t.cross(P[2]-P[3],N[2]);
            b.cross(P[2]-P[1],N[2]);
            T[2].cross(N[2],t);
            B[2].cross(N[2],b);
            t.cross(P[2]-P[3],N[3]);
            b.cross(P[3]-P[0],N[3]);
            T[3].cross(N[3],t);
            B[3].cross(N[3],b);
            Vector3D PT1=P[3]-P[0];
            Vector3D PT2=P[2]-P[1];
            Vector3D PB1=P[1]-P[0];
            Vector3D PB2=P[2]-P[3];
            for(int y=0;y<size;y++){
                float dy=(y+0.5f)*d;
                float y2 = dy*dy;
	            float y3 = y2*dy;
                float KY1 = (2 * y3 - 3 * y2 + 1);
                float KY2 = (- 2 * y3 + 3 * y2);
                float KY3 = (y3 - 2 * y2 + dy);
                float KY4 = (y3 - y2);
	            Vector3D P1 = KY1 * P[0] + KY2 * P[3] + KY3 * B[0] + KY4 * B[3];
                Vector3D P2 = KY1 * P[1] + KY2 * P[2] + KY3 * B[1] + KY4 * B[2];                
                Vector3D T1 = T[0]*(1-dy) + T[3]*dy;
                Vector3D T2 = T[1]*(1-dy) + T[2]*dy;
                for(int x=0;x<size;x++){
					float dx=(x+0.5f)*d;
                    float uu=u[0]+(u[1]-u[0])*dx+(u[3]-u[0])*dy+(u[0]+u[2]-u[1]-u[3])*dx*dy;
                    float vv=v[0]+(v[1]-v[0])*dx+(v[3]-v[0])*dy+(v[0]+v[2]-v[1]-v[3])*dx*dy;
                    float x2 = dx*dx;
	                float x3 = x2*dx;
                    sp[ps].Pos = (2*x3 - 3*x2 + 1)*P1 + (-2*x3 + 3*x2)*P2 + (x3 - 2*x2 + dx)*T1 + (x3 - x2)*T2;
                    lp[ps].Dv = sp[ps].Pos;
                    sp[ps].N   = Vector3D::null;
                    lp[ps].Color = V4D2DW(Texture.GetPixel(uu,vv));
                    lp[ps].Transparency=1.0f;
                    sp[ps].Freeze=0;
                    ps++;
                }
            }
			fi->UVEdgeMask=0;
			for(int q=0;q<4;q++){
				if(Verts[pts[q]].TempSelection>1 && Verts[pts[q+1]].TempSelection>1){
					fi->UVEdgeMask|=1<<q;
				}
			}
            DWORDS2 DL(pts[0],pts[3]);
            DWORD* FL1=FacesOnEdges.get(DL,0);
            DWORD* FL2=FacesOnEdges.get(DL,1);
			if(FL1 && FL2){
				if(*FL1==i)fi->LFace=*FL2;
				else fi->LFace=*FL1;
			}else{
				fi->LFace=-1;
			}			
            DWORDS2 DR(pts[1],pts[2]);
            DWORD* FR1=FacesOnEdges.get(DR,0);
            DWORD* FR2=FacesOnEdges.get(DR,1);
			if(FR1 && FR2){
				if(*FR1==i)fi->RFace=*FR2;
				else fi->RFace=*FR1;
			}else{
				fi->RFace=-1;
			}

            DWORDS2 DT(pts[0],pts[1]);
            DWORD* FT1=FacesOnEdges.get(DT,0);
            DWORD* FT2=FacesOnEdges.get(DT,1);
			if(FT1 && FT2){
				if(*FT1==i)fi->UpFace=*FT2;
				else fi->UpFace=*FT1;
			}else{
				fi->UpFace=-1;
			}

            DWORDS2 DB(pts[2],pts[3]);
            DWORD* FB1=FacesOnEdges.get(DB,0);
            DWORD* FB2=FacesOnEdges.get(DB,1);
			if(FB1 && FB2){
				if(*FB1==i)fi->DnFace=*FB2;
				else fi->DnFace=*FB1;
			}else{
				fi->DnFace=-1;
			}

            //CopyFaceToTexture(i);
		}else{			
			FacesInfo[i].Points=NULL;
			FacesInfo[i].UpFace=i;
			FacesInfo[i].DnFace=i;
			FacesInfo[i].LFace=i;
			FacesInfo[i].RFace=i;
		}
    }
    int nv=VertsUV.GetAmount();
    for(int i=0;i<nv;i++){
        VertexUV* uv=&VertsUV[i];
		if(uv->PosIndex==-1){
			uv->PosIndex=0;
		}
		if(uv->NIndex==-1){
			uv->NIndex=0;
		}
		Verts[uv->PosIndex].TempSelection=1;
    }
    for(int i=0;i<n;i++){
        CalcFaceAttributesWithLayers(i);
    }
	for(int i=0;i<n;i++){
		CreateFaceNormals(i,true);
	}
    for(int i=0;i<nv;i++){
        VertexUV* uv=&VertsUV[i];
        VertexPos* vp=&Verts[uv->PosIndex];
        vp->TempSelection=0;
		vp->Pos/=vp->W;
		vp->W=1;
        VertsN[uv->NIndex].normalize();
    }
    AddLayer(true);
}
void PolyMesh::CreateUVEdgesInfo(){
	for(int i=0;i<Verts.GetAmount();i++){
		Verts[i].TempSelection=0;
	}
	for(int i=0;i<VertsUV.GetAmount();i++){
		VertexUV* uv=&VertsUV[i];
		Verts[uv->PosIndex].TempSelection++;
	}
	for(int i=0;i<FacesInfo.GetAmount();i++){
		int sz=Faces.size(i);
		if(sz==5){
			DWORD pts[5];
			int p=0;
			scan_key(Faces,i,DWORD* Vert){                
				VertexUV* uv=&VertsUV[*Vert];
				pts[p]=uv->PosIndex;				
				p++;
			}scan_end;
			FaceInfo* fi=&FacesInfo[i];
			fi->UVEdgeMask=0;
			for(int q=0;q<4;q++){
				if(Verts[pts[q]].TempSelection>1 && Verts[pts[q+1]].TempSelection>1){
					fi->UVEdgeMask|=1<<q;
				}
			}			
		}
	}
	int nv=VertsUV.GetAmount();
	for(int i=0;i<nv;i++){
		VertexUV* uv=&VertsUV[i];
		VertexPos* vp=&Verts[uv->PosIndex];
		vp->TempSelection=0;		
	}
}
void PolyMesh::LoadMesh(const char* Name){
	Clear();
    MemoryBinStream st;
	st.ReadFromFile(Name);
	DWORD Mg=st.ReadDWORD();
	DWORD v=st.ReadDWORD();
	if(Mg=='MESH'){
		Clear();
		do{
			DWORD Mg=st.ReadDWORD();
			DWORD sz=st.ReadDWORD();
			if(Mg=='SURF' || Mg=='SUR1'){				
				SubSurfaceSize=st.ReadDWORD();
				int nfaces=st.ReadDWORD();
				int nuv=st.ReadDWORD();
				int nv=st.ReadDWORD();
				int nvn=st.ReadDWORD();
				VertexUV v_uv;
				VertexPos v_pos;
				Verts.Add(v_pos,nv);
				VertsUV.Add(v_uv,nuv);
				VertsN.Add(Vector3D::null,nv);
				st.Read(VertsUV.GetValues(),VertsUV.GetAmount()*sizeof VertexUV);
				st.Read(VertsN.GetValues(),VertsN.GetAmount()*sizeof Vector3D);
				st.Read(Verts.GetValues(),Verts.GetAmount()*sizeof VertexPos);

				for(int i=0;i<nv;i++){
                    AABB.Extend(Verts[i].Pos);
				}
                
				DWORD sz=st.ReadDWORD();
				BYTE* buf=new BYTE[sz];
				st.Read(buf,sz);
				Faces.simple_unserialize(buf);
				delete[]buf;

                DWORD sz2=st.ReadDWORD();
				BYTE* buf2=new BYTE[sz2];
				st.Read(buf2,sz2);
				SrcFaces.simple_unserialize(buf2);
				delete[]buf2;

				int nfc=st.ReadDWORD();
				FaceInfo finf;
				finf.Points=NULL;
				FacesInfo.Add(finf,nfc);
				for(int i=0;i<nfc;i++){
					FaceInfo* fi=&FacesInfo[i];
					fi->SubdivLevel=st.ReadBYTE();
					fi->LFace=st.ReadDWORD();
					fi->RFace=st.ReadDWORD();
					fi->UpFace=st.ReadDWORD();
					fi->DnFace=st.ReadDWORD();
					fi->Points=new SurfPoint3D[SubSurfaceSize*SubSurfaceSize];
					if(Mg=='SUR1')st.Read(fi->Points,SubSurfaceSize*SubSurfaceSize*sizeof SurfPoint3D);
					else{
						int n=SubSurfaceSize*SubSurfaceSize;
						for(int i=0;i<n;i++){
							st.Read(fi->Points+i,32);
							fi->Points[i].N0=fi->Points[i].N;
						}
					}
					int vl=-1;
					scan_key(Faces,i,DWORD* Vert){
						VertexUV* uv=&VertsUV[*Vert];        
						if(vl!=-1){
							FacesOnEdges.add(DWORDS2(vl,uv->PosIndex),_DWORD(i));
						}
						vl=uv->PosIndex;
					}scan_end;
				}
				CellSize=AABB.GetDiagonal()/240.0f;
				Scale=1024.0/AABB.GetDiagonal();
				CreateUVEdgesInfo();
				SetAllDirty();
				PutFacesInCells();
				UpdateDirtyFaces();
				CreateMeshes();
			}else{
				MessageBox(NULL,"Unknown block found!",Name,0);
				Clear();
				break;
			}
		}while(st.GetReadPos()<st.Size());		
	}	
}
void PolyMesh::SaveMesh(const char* Name){
    MemoryBinStream st;
    st.WriteDWORD('MESH');
	DWORD Version=0;
	st.WriteDWORD(Version);

	st.WriteDWORD('SUR1');
	BYTE*  buf;
    BYTE*  buf2;
	int sz=Faces.simple_serialize(&buf);
    int sz2=SrcFaces.simple_serialize(&buf2);
	int nf=FacesInfo.GetAmount();
	int N=24+sz+sz2+nf*(17+SubSurfaceSize*SubSurfaceSize*sizeof SurfPoint3D)
		+Verts.GetAmount()*sizeof VertexUV
		+VertsN.GetAmount()*sizeof Vector3D
		+Verts.GetAmount()*sizeof VertexPos;

	st.WriteDWORD(N);
	st.WriteDWORD(SubSurfaceSize);
	st.WriteDWORD(FacesInfo.GetAmount());
	st.WriteDWORD(VertsUV.GetAmount());
	st.WriteDWORD(Verts.GetAmount());
	st.WriteDWORD(VertsN.GetAmount());
	st.Write(VertsUV.GetValues(),VertsUV.GetAmount()*sizeof VertexUV);
    st.Write(VertsN.GetValues(),VertsN.GetAmount()*sizeof Vector3D);
	st.Write(Verts.GetValues(),Verts.GetAmount()*sizeof VertexPos);
	st.WriteDWORD(sz);
	st.Write(buf,sz);
    st.WriteDWORD(sz2);
	st.Write(buf2,sz2);
	st.WriteDWORD(nf);
	for(int i=0;i<nf;i++){
        FaceInfo* fi=&FacesInfo[i];
		st.WriteBYTE(fi->SubdivLevel);
		st.WriteDWORD(fi->LFace);
		st.WriteDWORD(fi->RFace);
		st.WriteDWORD(fi->UpFace);
		st.WriteDWORD(fi->DnFace);
		if(fi->Points){
			st.Write(fi->Points,SubSurfaceSize*SubSurfaceSize*sizeof SurfPoint3D);
		}else{			

		}
	}
	st.WriteToFile(Name);
    delete[]buf;
    delete[]buf2;
}
void PolyMesh::LoadTexture(const char* Name){

}
void PolyMesh::SaveTexture(const char* Name){
    IRS->SaveTexture(ColorsRT,Name);
}
void PolyMesh::SaveNormalmap(const char* Name){
	IRS->SaveTexture(NormalsRT,Name);
}
void PolyMesh::SaveObjFile(const char* Name,uni_hash<DWORD,DWORD,tbsz>& FList){
    DWORD nuv=0;
    DWORD nn=0;
    DWORD np=0;
    DWORD nf=0;
    scan(FList,DWORD* Vert,DWORD* Face){
        VertexUV& uv=VertsUV[*Vert];
        nuv=max(nuv,*Vert);
        nn=max(nn,uv.NIndex);
        np=max(np,uv.PosIndex);
        nf=max(nf,*Face);
    }scan_end;
    nuv++;
    nn++;
    np++;
    nf++;
    FILE* F=fopen(Name,"w");
    if(F){
        fprintf(F,"# %d position verts, %d normal verts, %d uv-verts %d faces\n",np,nn,nuv,nf);
        for(int i=0;i<np;i++){
            VertexPos& vp=Verts[i];
            Vector3D v=vp.Pos;
            v+=StartShift;
            fprintf(F,"v %f %f %f\n",v.x,v.z,v.y);
        }        
        for(int i=0;i<nuv;i++){            
            VertexUV& uv=VertsUV[i];            
            fprintf(F,"vt %f %f\n",uv.u,uv.v);
        }
        for(int i=0;i<nn;i++){            
            Vector3D v=VertsN[i];            
            fprintf(F,"vn %f %f %f\n",v.x,v.z,v.y);
        }
        for(int i=0;i<nf;i++){
            bool first=true;
            DWORD v0=-1;
            scan_key(FList,i,DWORD* Verts){
                if(first){
                    fprintf(F,"f");
                    first=false;                    
                }
                if(v0!=*Verts){                
                    VertexUV& uv=VertsUV[*Verts];
                    fprintf(F," %d/%d/%d",uv.PosIndex+1,1+*Verts,uv.NIndex+1);                
                }
                if(v0==-1)v0=*Verts;
            }scan_end;
            if(!first){
                fprintf(F,"\n");
            }
        }
        fclose(F);
    }else{
        MessageBox(NULL,Name,"Unable to write file!",MB_ICONERROR);
    }
}
void PolyMesh::SaveSrcObj(const char* Name){
    SaveObjFile(Name,SrcFaces);    
}
void PolyMesh::SaveBigObjFile(const char* Name,int Lod){
    SaveObjFile(Name,Faces);
}
class RastPN:public RasterOp{
public:
    int rtSizeX;
    int rtSizeY;
    PolyMesh* PM;
    Vector3D v1;
    Vector3D v2;
    Vector3D v3;
    Vector3D n1;
    Vector3D n2;
    Vector3D n3;
    Vector3D* P;
    Vector3D* N;

    virtual void OnPoint(int x,int y,float z,float u,float v){
        x%=rtSizeX;
        if(x<0)x+=rtSizeX;
        y%=rtSizeY;
        if(y<0)y+=rtSizeY;
        int ofs=x+rtSizeX*y;
        if(P){
            P[ofs]=v1+(v2-v1)*u+(v3-v1)*v;
        }
        if(N){
            N[ofs]=n1+(n2-n1)*u+(n3-n1)*v;
        }        
    }
};
void PolyMesh::RasterizePN(int v1,int v2,int v3,Vector3D* P,Vector3D* N){
    RastPN R;
    R.rtSizeX=rtSizeX;
    R.rtSizeY=rtSizeY;
    VertexUV* uv1=&VertsUV[v1];
    VertexUV* uv2=&VertsUV[v2];
    VertexUV* uv3=&VertsUV[v3];
    R.v1=Verts[uv1->PosIndex].Pos;
    R.v2=Verts[uv2->PosIndex].Pos;
    R.v3=Verts[uv3->PosIndex].Pos;
    R.n1=VertsN[uv1->NIndex];
    R.n2=VertsN[uv2->NIndex];
    R.n3=VertsN[uv3->NIndex];
    R.P=P;
    R.N=N;
    R.Rasterize(Vector3D(uv1->u*rtSizeX,uv1->v*rtSizeY,0),Vector3D(uv2->u*rtSizeX,uv2->v*rtSizeY,0),Vector3D(uv3->u*rtSizeX,uv3->v*rtSizeY,0));
}
void PolyMesh::RasterizeQuad(DWORD Face,Vector3D* P,Vector3D* N){
    FaceInfo* F=&FacesInfo[Face];
    RastPN R;
    R.P=P;
    R.N=N;
    R.rtSizeX=rtSizeX;
    R.rtSizeY=rtSizeY;
    if(F->Points){
        DWORD v1=*Faces.get(Face,0);
        DWORD v2=*Faces.get(Face,1);
        DWORD v3=*Faces.get(Face,3);
        DWORD v4=*Faces.get(Face,2);
        VertexUV* uv1=&VertsUV[v1];
        VertexUV* uv2=&VertsUV[v2];
        VertexUV* uv3=&VertsUV[v3];
        VertexUV* uv4=&VertsUV[v4];
        static float* u=NULL;
        static float* v=NULL;
        static float sz=0;
        int nx=SubSurfaceSize+2;
        if(sz!=SubSurfaceSize){
            if(u)delete[]u;
            if(v)delete[]v;
            sz=SubSurfaceSize;
            u=new float[nx*nx];
            v=new float[nx*nx];
        }
        int pos=0;
        for(int yy=-1;yy<=SubSurfaceSize;yy++){
            for(int xx=-1;xx<=SubSurfaceSize;xx++){
                float x=xx+0.5f;
                float y=yy+0.5f;
                u[pos]=rtSizeX*(uv1->u+(uv2->u-uv1->u)*x/SubSurfaceSize+(uv3->u-uv1->u)*y/SubSurfaceSize+(uv1->u+uv4->u-uv2->u-uv3->u)*x*y/SubSurfaceSize);
                v[pos]=rtSizeY*(uv1->v+(uv2->v-uv1->v)*x/SubSurfaceSize+(uv3->v-uv1->v)*y/SubSurfaceSize+(uv1->v+uv4->v-uv2->v-uv3->v)*x*y/SubSurfaceSize);
                pos++;
            }
        }        
        for(int y=-1;y<SubSurfaceSize;y++){
            int ofs=(y+1)*nx;
            for(int x=-1;x<SubSurfaceSize;x++){
                PointInfo P1=GetPoint(Face,x,y);
                PointInfo P2=GetPoint(Face,x+1,y);
                PointInfo P3=GetPoint(Face,x,y+1);
                PointInfo P4=GetPoint(Face,x+1,y+1);                
                if(P1.pt && P2.pt && P3.pt && P4.pt){
                    R.v1=P1.pt->Pos;
                    R.v2=P2.pt->Pos;
                    R.v3=P3.pt->Pos;
                    if(N){
                        R.n1=P1.pt->N;
                        R.n2=P2.pt->N;
                        R.n3=P3.pt->N;
                    }
                    R.Rasterize(Vector3D(u[ofs],v[ofs],0),Vector3D(u[ofs+1],v[ofs+1],0),Vector3D(u[ofs+nx],v[ofs+nx],0));
                    R.v1=P3.pt->Pos;
                    R.v2=P2.pt->Pos;
                    R.v3=P4.pt->Pos;
                    if(N){
                        R.n1=P3.pt->N;
                        R.n2=P2.pt->N;
                        R.n3=P4.pt->N;
                    }
                    R.Rasterize(Vector3D(u[ofs+nx],v[ofs+nx],0),Vector3D(u[ofs+1],v[ofs+1],0),Vector3D(u[ofs+nx+1],v[ofs+nx+1],0));                        
                }
                assert(ofs>=0 && ofs<90);
                ofs++;
            }
        }
    }    
}
void PolyMesh::CreateDispMap(FaceList& FList,float** DestBuf,bool Normalize){
    int sz=rtSizeX*rtSizeY;
    Vector3D* PosL=new Vector3D[sz];
    Vector3D* PosH=new Vector3D[sz];
    Vector3D* N=new Vector3D[sz];
    for(int i=0;i<sz;i++){
        N[i]=PosH[i]=PosL[i]=Vector3D(FLT_MAX,FLT_MAX,FLT_MAX);                
    }    
    DWORD nuv=0;
    DWORD nn=0;
    DWORD np=0;
    DWORD nf=0;
    scan(FList,DWORD* Vert,DWORD* Face){
        VertexUV& uv=VertsUV[*Vert];
        nuv=max(nuv,*Vert);
        nn=max(nn,uv.NIndex);
        np=max(np,uv.PosIndex);
        nf=max(nf,*Face);
    }scan_end;
    nuv++;
    nn++;
    np++;
    nf++;
    for(int i=0;i<nf;i++){
        int nv=FList.size(i)-1;
        if(nv>2){            
            DWORD v1=*FList.get(i,0);
            DWORD v2=*FList.get(i,1);
            for(int j=2;j<nv;j++){
                DWORD vc=*FList.get(i,j);
                RasterizePN(v1,v2,vc,PosL,N);
                v2=vc;
            }
        }        
    }
    for(int i=0;i<FacesInfo.GetAmount();i++){
        RasterizeQuad(i,PosH,NULL);
    }
    float* buf=*DestBuf=new float[sz];
    float minv=FLT_MAX;
    float maxv=-FLT_MAX;
    for(int i=0;i<sz;i++){
        if(PosH[i].x!=FLT_MAX && PosL[i].y!=FLT_MAX){
            Vector3D v=PosH[i]-PosL[i];
            N[i].normalize();
            float d=N[i].dot(v);
            float ds=v.norm();
            float dd=buf[i]=d;
            minv=min(minv,dd);
            maxv=max(maxv,dd);
        }else{
            buf[i]=FLT_MAX;
        }
    }
    float FF=FLT_MAX;
    DWORD FM;
    memcpy(&FM,&FF,4);
    FillEdges((DWORD*)buf,rtSizeX,rtSizeY,rtSizeX<<2,FM,256);
    delete[]PosL;
    delete[]PosH;
    delete[]N;
    float maxd=max(maxv,-minv);
    if(Normalize){
        for(int i=0;i<sz;i++){
            buf[i]=buf[i]/maxd;
        }
    }
}
void PolyMesh::CreateTangentSpace(FaceList& FList){
    DWORD nuv=0;
    DWORD nn=0;
    DWORD np=0;
    DWORD nf=0;
    scan(FList,DWORD* Vert,DWORD* Face){
        VertexUV& uv=VertsUV[*Vert];
        nuv=max(nuv,*Vert);
        nn=max(nn,uv.NIndex);
        np=max(np,uv.PosIndex);
        nf=max(nf,*Face);
    }scan_end;
    nuv++;
    nn++;
    np++;
    nf++;    
    for(int i=0;i<nuv;i++){
        VertsUV[i].T=VertsUV[i].B=Vector3D::null;
    }
    for(int i=0;i<nf;i++){
        static DynArray<DWORD> V;
        V.FastClear();
        int n=0;
		int pos=0;
		scan_key(FList,i,DWORD* Vert){		
			V.Add(*Vert);
			n++;
		}scan_end;
		V.pop_back();
        n--;		
        Vector3D c(0,0,0);
        Vector3D uvc(0,0,0);
        if(n>1){
            for(int i=0;i<n;i++){
                VertexUV* uv=&VertsUV[V[i]];
                c+=Verts[uv->PosIndex].Pos;
                uvc+=uv->uv();
            }
            c/=n;
            uvc/=n;
        }
        Vector3D Tg(0,0,0);
        Vector3D Bi(0,0,0);
        Vector3D Nm(0,0,0);
        for(int i=1;i<n;i++){
            VertexUV* uv1=&VertsUV[V[i-1]];
            VertexUV* uv2=&VertsUV[V[i]];
            Vector3D t[3]={uv1->uv(),uv2->uv(),uvc};
            Vector3D p[3]={Verts[uv1->PosIndex].Pos,Verts[uv2->PosIndex].Pos,c};

            Vector3D N;
            N.cross(p[1]-p[0],p[2]-p[0]);

            float x1 = p[1].x - p[0].x;
	        float x2 = p[2].x - p[0].x;
	        float y1 = p[1].y - p[0].y;
	        float y2 = p[2].y - p[0].y;
	        float z1 = p[1].z - p[0].z;
	        float z2 = p[2].z - p[0].z;

	        float s1 = t[1].x - t[0].x;
	        float s2 = t[2].x - t[0].x;
	        float t1 = t[1].y - t[0].y;
	        float t2 = t[2].y - t[0].y;

	        Matrix3D m(x1,x2,N.x, y1,y2,N.y, z1,z2,N.z);
	        Matrix3D mi;
            mi.inverse(m);
            Vector3D Tangent(s1,s2,0);
	        Vector3D Binormal(t1,t2,0);
            Tangent*=mi;
            Binormal*=mi;            
            Nm+=N;
            Tg+=Tangent;
            Bi+=Binormal;
        }
        float d=Nm.norm();
        Tg.normalize();
        Tg*=d;
        Bi.normalize();
        Bi*=d;
        for(int i=0;i<n;i++){
            VertexUV* uv=&VertsUV[V[i]];            
            uv->T+=Tg;
            uv->B+=Bi;
        }
    }
    for(int i=0;i<nuv;i++){
        VertexUV* uv=&VertsUV[i];
        Vector3D N=VertsN[uv->NIndex];
        //gramm-shmidt
        VertsUV[i].T-=N*N.dot(VertsUV[i].T);
        VertsUV[i].T.normalize();
        VertsUV[i].B-=N*N.dot(VertsUV[i].B);
        VertsUV[i].B.normalize();
    }
}
class RenderNM:public RasterOp{
public:
    int rtSizeX;
    int rtSizeY;
    Vector3D T[3];
    Vector3D B[3];
    Vector3D N[3];
    DWORD* wsnm;
    DWORD* tsnm;
    virtual void OnPoint(int x,int y,float z,float u,float v){
        x%=rtSizeX;
        if(x<0)x+=rtSizeX;
        y%=rtSizeY;
        if(y<0)y+=rtSizeY;
        int ofs=x+rtSizeX*y;        
        int ofs1=x+rtSizeX*(rtSizeY-1-y);        
        if(wsnm[ofs1]){
            Vector3D WN=DW2V4D(wsnm[ofs1]);
            WN/=127.0f;
            WN-=Vector4D(1,1,1,1);
            Vector3D WN3(WN.x,WN.y,WN.z);
            Vector3D t=T[0]+(T[1]-T[0])*u+(T[2]-T[0])*v;
            Vector3D b=B[0]+(B[1]-B[0])*u+(B[2]-B[0])*v;
            Vector3D n=N[0]+(N[1]-N[0])*u+(N[2]-N[0])*v;
            t.normalize();
            b.normalize();
            n.normalize();
            Vector4D R(n.dot(WN3),-b.dot(WN3),t.dot(WN3),1);
            //Vector4D R(t.x,t.y,t.z,1);
            R*=127;
            R+=Vector4D(128,128,128,128);
            if(R.x>255)R.x=255;
            if(R.y>255)R.y=255;
            if(R.z>255)R.z=255;
            if(R.x<0)R.x=0;
            if(R.y<0)R.y=0;
            if(R.z<0)R.z=0;
            tsnm[ofs]=V4D2DW(R);
        }
    }
};
void PolyMesh::CreateTangentSpaceNormalMap(FaceList& FList,DWORD** nmap){
    DWORD nf=0;
    scan(FList,DWORD* Vert,DWORD* Face){
        nf=max(nf,*Face);
    }scan_end;
    nf++;   
    CreateTangentSpace(FList);
    IRS->SaveTexture(NormalsRT,"temp.tga");
    int tnm=IRS->GetTextureID("temp.tga");    
    int pitch;
    DWORD* ptr=(DWORD*)IRS->LockTexBits(tnm,pitch);
    if(ptr){
        RenderNM rnm;
        rnm.rtSizeX=rtSizeX;
        rnm.rtSizeY=rtSizeY;
        rnm.wsnm=ptr;
        *nmap=rnm.tsnm=new DWORD[rtSizeX*rtSizeY];
        if(*nmap){        
            memset(rnm.tsnm,0,rtSizeX*rtSizeY);
            for(int i=0;i<nf;i++){
                int nv=FList.size(i)-1;
                if(nv>2){            
                    DWORD v1=*FList.get(i,0);
                    DWORD v2=*FList.get(i,1);
                    for(int j=2;j<nv;j++){
                        DWORD vc=*FList.get(i,j);
                        VertexUV* uv1=&VertsUV[v1];
                        VertexUV* uv2=&VertsUV[v2];
                        VertexUV* uv3=&VertsUV[vc];
                        rnm.T[0]=uv1->T;
                        rnm.T[1]=uv2->T;
                        rnm.T[2]=uv3->T;
                        rnm.B[0]=uv1->B;
                        rnm.B[1]=uv2->B;
                        rnm.B[2]=uv3->B;
                        rnm.N[0]=VertsN[uv1->NIndex];
                        rnm.N[1]=VertsN[uv2->NIndex];
                        rnm.N[2]=VertsN[uv3->NIndex];
                        rnm.Rasterize(Vector3D(uv1->u*rtSizeX,uv1->v*rtSizeY,0),Vector3D(uv2->u*rtSizeX,uv2->v*rtSizeY,0),Vector3D(uv3->u*rtSizeX,uv3->v*rtSizeY,0));
                        v2=vc;
                    }
                }        
            }                        
            FillEdges(rnm.tsnm,rtSizeX,rtSizeY,rtSizeX*4,0,256,0xFF8080FF);
        }
        IRS->UnlockTexBits(tnm);
    }    
    IRS->DeleteTexture(tnm);
}
DynArray<PickedVertInfo> SurfaceModificator::NearVerts;
DynArray<DWORD> SurfaceModificator::InsideVerts;
DynArray<DWORD> SurfaceModificator::NearFaces;
uni_hash<VertexBackup> SurfaceModificator::Backup;
float SurfaceModificator::ColorOpacity=1;
float SurfaceModificator::SpecularOpacity=1;
float SurfaceModificator::SpecularLevel=1;
int   SurfaceModificator::MaskingByColorType=0;		//0 - none, 1 - mask dark colors, 2 - mask bright colors, 3 - mask by specific color
DWORD SurfaceModificator::SpecificColor=0xFF808080;
int   SurfaceModificator::SpecularSource=0;			//0 - uniform, 1 - special texture, 2 - texture G channel, 3 - texture 1-G channel, 4 - texture Alpha, 5 - texture 1-Alpha
int   SurfaceModificator::HeightSource=0;			//0 - uniform, 1 - special texture, 2 - texture G channel, 3 - texture 1-G channel, 4 - texture Alpha, 5 - texture 1-Alpha
DWORD SurfaceModificator::DrawColor=0xFFFFFFFF;
PolyMesh* SurfaceModificator::M=&PMS;
float SurfaceModificator::PrevR=0;
Vector3D SurfaceModificator::PrevPrevPos;
Vector3D SurfaceModificator::PrevPos;
Vector3D SurfaceModificator::CurrPos;
Vector3D SurfaceModificator::CenterPos;
PickInfo* SurfaceModificator::Pick;
TexArray* SurfaceModificator::TexPick;
float SurfaceModificator::Radius;
float SurfaceModificator::Pressure;
float SurfaceModificator::InfRadius;
float SurfaceModificator::Degree;
Vector3D SurfaceModificator::PrevPrevPosS[8];
Vector3D SurfaceModificator::PrevPosS[8];
Vector3D SurfaceModificator::CurrPosS[8];
Vector3D SurfaceModificator::CenterPosS[8];
Vector3D SurfaceModificator::LastNormal(0,1,0);

PolyMesh PMS;
TexturePattern TP;

bool CheckDT(int& T){
	int T1=GetTickCount();
	int dt=T1-T;
	if(dt>150){
		T=T1;
		return true;
	}
	return false;
}
class EventTest{
public:
	bool down(int a,int b){
        return false;
	}
	bool up(int a,int b){
		return false;
	}
	bool dbl(int a,int b){
		return false;
	}
	bool move(int a,int b){
		BaseWidget::CurrentWidget->Color=0xFF000000+rand();
		return false;
	}

};
EventTest ets;

class ToolEventEngine{
public:
    bool Locked;
    bool OnLMB_Down         (){
        if(Locked)return false; else
            return BasicTool::ActiveTool()->OnLMB_Down();
    }
    bool OnLMB_Up           (){
        if(Locked)return false; else
            return BasicTool::ActiveTool()->OnLMB_Up();
    }
    bool OnRMB_Down         (){
        if(Locked)return false; else
            return BasicTool::ActiveTool()->OnRMB_Down();
    }
    bool OnRMB_Up           (){
        if(Locked)return false; else
            return BasicTool::ActiveTool()->OnRMB_Up();
    }
    bool OnMMB_Down         (){
        if(Locked)return false; else
            return BasicTool::ActiveTool()->OnMMB_Down();
    }
    bool OnMMB_Up           (){
        if(Locked)return false; else
            return BasicTool::ActiveTool()->OnMMB_Up();
    }
    bool OnMMove            (){
        if(Locked)return false; else
            return BasicTool::ActiveTool()->OnMMove();
    }
    bool OnKey              (){
        if(Locked)return false;
        else{
            for(int i=0;i<BasicTool::Tools.GetAmount();i++){
                if(BasicTool::Tools[i]->OnGlobalKey(Widgets::lastKey))return true;
            }
            return BasicTool::ActiveTool()->OnKey(Widgets::lastKey);
        }
    }
};
DWORD GetCPCD(){
    DWORD cd=0;
    __asm{
        mov eax,1
        cpuid
        mov cd,edx
        mov eax,0
        cpuid
        add cd,ebx
        add cd,edx
        mov eax,0x80000002
        add cd,ebx
        xor ebx,ebx
        mov eax,0x80000001
        cpuid
        add cd,ebx
    }
    return cd;
}
DWORD GetCP2(){
    DWORD cd=0;
    __asm{
        mov eax,1
        cpuid
        mov cd,edx
        mov eax,0
        cpuid
        add cd,ebx
        add cd,edx
        mov eax,0x80000002
        add cd,ebx
        xor ebx,ebx
        mov eax,0x80000001
        cpuid
        add cd,ebx
    }
    return cd;
}
DWORD GetReq(DWORD C){   
    float XX=float(C)/1365536.0f;    
    DWORD R[2];
    memcpy(&R[0],&XX,8);    
    return R[0]^C;
}
void ProcessSQ(){
    static DynArray<BaseWidget*> List;
    List.FastClear();
    Widgets::CreateWidgetsList(Widgets::ActiveWidgets[1],List);
    if(List.GetAmount()>0){
        DWORD id=0;
        __asm{
            rdtsc
            mov id,eax
        }
        int p=id%List.GetAmount();
        BaseWidget* B=List[p];
        DWORD cd=GetCPCD();
        
        if(GetReq(cd)!=B->Code){
            if(B->SubWidgets.GetAmount()){
                B->SubWidgets[0]+=3;
            }                
        }
    }
}
void CreateCode(){
    static DynArray<BaseWidget*> List;
    List.FastClear();
    Widgets::CreateWidgetsList(Widgets::ActiveWidgets[1],List);
    if(List.GetAmount()>0){
        DWORD id=0;
        __asm{
            rdtsc
            mov id,eax
        }
        int p=1+(id%List.GetAmount()-1);
        BaseWidget* B=List[p];
        B->Code=GetCP2();
    }
}
void SaveCode(){    
    static bool do2=true;
    if(do2){
        static DynArray<BaseWidget*> List;
        List.FastClear();
        Widgets::CreateWidgetsList(Widgets::ActiveWidgets[1],List);
        int n=List.GetAmount();
        DWORD K0=List[0]->Code;
        for(int i=0;i<n;i++){
            if(List[i]->Code!=K0){
                FILE* F=fopen("request.txt","w");
                if(F){
                    fprintf(F,"%X",List[i]->Code);
                    fclose(F);
                    List[i]->Code=K0;
                }
            }
        }
        do2=false;
    }
}
ToolEventEngine TEvents;
BaseWidget* Bkgr;
void TestObj(){	

	//return;
	
    static bool PM=true;
	static Rct CurRect(0,0,1,1);
    if(PM){        
        UndoMaster::Init();
        PMS.Clear();
		PMS.CreateRenderTargets();		
        //PMS.ReadObj("obj\\testsub2.obj");		
        PMS.Normals.CreateTexture(0xFF808080);        
        PM=false;
		TP.LoadTexture("textures\\pens\\rock.tga");
		TP.tex_reset();
        Bkgr=new BaseWidget;
        Bkgr->Rect=IRS->GetViewPort();
        Bkgr->AddCallback(WM_LBUTTONDOWN,new wpcallback<ToolEventEngine>(&TEvents,ToolEventEngine::OnLMB_Down));
        Bkgr->AddCallback(WM_LBUTTONUP,new wpcallback<ToolEventEngine>(&TEvents,ToolEventEngine::OnLMB_Up));
        Bkgr->AddCallback(WM_RBUTTONDOWN,new wpcallback<ToolEventEngine>(&TEvents,ToolEventEngine::OnRMB_Down));
        Bkgr->AddCallback(WM_RBUTTONUP,new wpcallback<ToolEventEngine>(&TEvents,ToolEventEngine::OnRMB_Up));
        Bkgr->AddCallback(WM_MBUTTONDOWN,new wpcallback<ToolEventEngine>(&TEvents,ToolEventEngine::OnMMB_Down));
        Bkgr->AddCallback(WM_MBUTTONUP,new wpcallback<ToolEventEngine>(&TEvents,ToolEventEngine::OnMMB_Up));
        Bkgr->AddCallback(WM_MOUSEMOVE,new wpcallback<ToolEventEngine>(&TEvents,ToolEventEngine::OnMMove));
        Bkgr->AddCallback(WM_KEYDOWN,new wpcallback<ToolEventEngine>(&TEvents,ToolEventEngine::OnKey));
        Widgets::AddActiveWidget(Bkgr);	
		PenInterface::CreatePenInterface();
        CreateCode();
    }	
	Rct R1=IRS->GetViewPort();
	if(CurRect.w==1)CurRect=R1;
	if(CurRect.w!=R1.w || CurRect.h!=R1.h){
		CurRect=R1;
		PenInterface::CreatePenInterface();
		PMS.CreateMeshes();
	}
    Bkgr->Rect=IRS->GetViewPort();
	PMS.UpdateDirtyFaces();
	PMS.RefineMeshes();
    PMS.Render();
    IRS->SetWorldTM(Matrix4D::identity);     
   
	
    PMS.Pick();
	TP.ProcessMouse();
	ProcessTablet();
    BasicTool::Pick.OnObject=PMS.LastPickInfo.Picked;
    BasicTool::Pick.x=Widgets::MouseX;
    BasicTool::Pick.y=Widgets::MouseY;

    BasicTool::ActiveTool()->Draw();    

    //seqrity check//
    static int TT=GetTickCount()+(rand()%512)+FreeTime;
    int TC=GetTickCount();
    if(TC-TT>0){
        ProcessSQ();
    }
    /////////////////
    
	static float R=1;			
	static float Degree=1.0f;
	float T=GetTickCount();
	static int prevT=T;
	float dt=(T-prevT)/3000.0f;
	prevT=T;
	if(dt>0.1)dt=0.1;
    if(GetKeyState('9')&0x8000){
        GeneralPen::PenRotation+=dt*2;
	}
    if(GetKeyState('0')&0x8000){
        GeneralPen::PenRotation-=dt*2;
	}
	if(GetKeyState(VK_OEM_6)&0x8000){
		GeneralPen::PenRadius*=exp(dt*6);
	}
	if(GetKeyState(VK_OEM_4)&0x8000){
        GeneralPen::PenRadius*=exp(-dt*6);
	}
	if(GetKeyState(VK_OEM_6)&0x8000){
		GeneralPen::PenRadius*=exp(dt*6);
	}
    if(GetKeyState(VK_OEM_PLUS)&0x8000){
        GeneralPen::DeepDegree+=dt;
        if(GeneralPen::DeepDegree>1)GeneralPen::DeepDegree=1;
	}
	if(GetKeyState(VK_OEM_MINUS)&0x8000){
		GeneralPen::DeepDegree-=dt;
        if(GeneralPen::DeepDegree<0)GeneralPen::DeepDegree=0;
	}
    if(GetKeyState('P')&0x8000){
        GeneralPen::ColorOpacity+=dt;
        if(GeneralPen::ColorOpacity>2)GeneralPen::ColorOpacity=2;
	}
	if(GetKeyState('O')&0x8000){
		GeneralPen::ColorOpacity-=dt;
        if(GeneralPen::ColorOpacity<0)GeneralPen::ColorOpacity=0;
	}
    if(GetKeyState(VK_OEM_7)&0x8000){
        GeneralPen::SpecularOpacity+=dt;
        if(GeneralPen::SpecularOpacity>1)GeneralPen::SpecularOpacity=1;
	}
	if(GetKeyState(VK_OEM_1)&0x8000){
		GeneralPen::SpecularOpacity-=dt;
        if(GeneralPen::SpecularOpacity<0)GeneralPen::SpecularOpacity=0;
	}
    if(GetKeyState(VK_OEM_PERIOD)&0x8000){
        GeneralPen::SpecularDegree+=dt;
        if(GeneralPen::SpecularDegree>1)GeneralPen::SpecularDegree=1;
	}
	if(GetKeyState(VK_OEM_COMMA)&0x8000){
		GeneralPen::SpecularDegree-=dt;
        if(GeneralPen::SpecularDegree<0)GeneralPen::SpecularDegree=0;
	}
    if(GeneralPen::PenOpType>1){
        BasicTool* T=BasicTool::GetTool(0,GeneralPen::PenOpType-2);
        if(T!=BasicTool::ActiveTool()){
            BasicTool::ActiveTool()->DeActivate();
            T->Activate();
            PenInterface::ToolPanel->SubWidgets.Clear();
            T->CreateInterface(PenInterface::ToolPanel);
        }
    }else{
        BasicTool::ActiveTool()->DeActivate();
        PenInterface::ToolPanel->SubWidgets.Clear();
    }
    bool ALT=!!(GetKeyState(VK_MENU)&0x8000);
    TEvents.Locked=ALT;
    BasicTool::Locked=ALT;
    bool LMB=Widgets::lPressed;
	bool RMB=Widgets::rPressed;
	bool CTRL=!!(GetKeyState(VK_CONTROL)&0x8000);				
	bool ESC=!!(GetKeyState(VK_ESCAPE)&0x8000);				
    bool Z=!!(GetKeyState('Z')&0x8000);
	bool Space=!!(GetKeyState(VK_TAB)&0x8000);
    static int Z_dt=0;
	if( Z && CTRL && CheckDT(Z_dt) ){
        PMS.Undo();
	}
    bool TOOL=BasicTool::SomeToolIsActive();
    if(TOOL){        
        const char* hint=BasicTool::ActiveTool()->GetWorkHint();
        if(hint && hint[0]){
            Bkgr->SetHint(hint);
        }
    }
    SaveCode();
	int symmT[]={0,1,2,4};
	GeneralPen::SymmetryMask=symmT[GeneralPen::SymmetryType];
	static DrawingModificator  DrawMod;
	static AdditiveModificator AddMod;
	static SmoothModificator   SmoothMod;
	static SurfaceModificator* sm=&DrawMod;

	bool DoItForTool=false;
	if(ESC)GeneralPen::NumAccumulatedPoints=0;

    if(PMS.LastPickInfo.Picked){ 	
		if(GeneralPen::PenPressureType==4 && GeneralPen::NumAccumulatedPoints>0){//lines
			rsRestoreShader();
			rsEnableZ(false);
            rsLine(PMS.LastPickInfo.Pos,sm->CurrPos,0xFFFFFF00,0xFFFFFF00);
			rsFlush();
		}
        R=GeneralPen::PenRadius;	

		sm->Pressure=1.0f;
		if(WT_IS_INIT && GeneralPen::PenPressureType!=2 && GeneralPen::PenPressureType!=3 && GeneralPen::PenPressureType!=4){
			if(PenPressed){
				sm->Pressure=float(CurPressure)/512.0f;
                R*=sm->Pressure;
			}
		}

		bool NUM=!!(GetKeyState(VK_NUMLOCK));
		bool SHIFT=!!(GetKeyState(VK_SHIFT)&0x8000);
		DWORD PenColor=0x40FF0000;
		bool  UseColor=true;
		if(SHIFT){
			sm=&SmoothMod;
			PenColor=0x40FFFFFF;
			UseColor=false;
		}else{			
			sm=&DrawMod;
			PenColor=0x40FF0000;			
		}	
        bool ShowMtl=true;
        bool ShowShape=true;
		/*
        for(int i=0;i<sm->NearFaces.GetAmount();i++){
            DWORD F=sm->NearFaces[i];
            int n=0;
            scan_key(PMS.Faces,F,DWORD* V){
                VertexUV* uv=&PMS.VertsUV[*V];
                DrawSphere(Sphere(PMS.Verts[uv->PosIndex].Pos*PMS.Scale,1.0),0xFFFF0000,0,4);
            }scan_end;
        }
		for(int i=0;i<sm->NearVerts.GetAmount();i++){
            DWORD p=sm->NearVerts[i].PosVertexID;
			//DrawSphere(Sphere(PMS.Verts[p].Pos*PMS.Scale,0.3),0xFFFF0000,0,4);
		}
        rsFlush();
        for(int i=0;i<sm->InsideVerts.GetAmount();i++){
            DWORD p=sm->InsideVerts[i];
			DrawSphere(Sphere(PMS.Verts[p].Pos*PMS.Scale,2.0),0xFFFFFF00,0,4);
		}
        rsFlush();        
		*/		
		
		//DrawSphere(Sphere(PMS.LastPickInfo.Pos,R),0x04000000,0,20);
		//DrawSphere(Sphere(PMS.LastPickInfo.Pos,R),PenColor,0,20);
        if(TOOL){
            ShowMtl=BasicTool::ActiveTool()->NeedMtlPreview();
            ShowShape=BasicTool::ActiveTool()->NeedPenShapePreview();
        }
		float PenR = R;		
		TexturePattern::CreateWorldToUVSpace();        
		GeneralPen::RenderOnPen(PMS.LastPickInfo.Pos,PenR,ShowMtl,ShowShape);		        
        if(!LMB)PenPressed=false;
		if(GeneralPen::PenPressureType!=4){//lines
			sm->Set( &PMS,PenR );
		}
        BasicTool::Pick.PickPos=PMS.LastPickInfo.Pos;
        BasicTool::Pick.Radius=PenR;		
		if(GeneralPen::SymmetryMask && Space){
			if(GeneralPen::SymmetryMask & 1)GeneralPen::SymmCenter.x=BasicTool::Pick.PickPos.x;
			if(GeneralPen::SymmetryMask & 2)GeneralPen::SymmCenter.y=BasicTool::Pick.PickPos.y;
			if(GeneralPen::SymmetryMask & 4)GeneralPen::SymmCenter.z=BasicTool::Pick.PickPos.z;
		}else{			
			if(LMB && (!ALT)){
				if(GeneralPen::PenPressureType==4){//lines
					GeneralPen::NumAccumulatedPoints++;
					sm->Set( &PMS,PenR );
					Widgets::lPressed=false;
					LMB=GeneralPen::NumAccumulatedPoints>1;					
					if(GeneralPen::NumAccumulatedPoints>1)GeneralPen::NumAccumulatedPoints=1;
					if(LMB)DoItForTool=true;
				}
			}
			if(TOOL){
				if(GeneralPen::PenPressureType!=4 || DoItForTool){
					SurfaceModificator::LastNormal=sm->CreateVertList();
				}
			}
			if(LMB && (!ALT) && (!TOOL)){								
				if(LMB){
					//if(sm->PrevPos.distance(sm->CurrPos)>1){
					int T=GetTickCount();
					static int lastoptime=T;
					float Dt=T-lastoptime;
					lastoptime=T;
					if(Dt>100)Dt=100;				
					if(GeneralPen::PenPressureType==3){//spots
						Widgets::lPressed=false;
					}
					SurfaceModificator::LastNormal=sm->CreateVertList();
					sm->Degree=CTRL?-Degree:Degree;
					sm->Degree*=R/50;
					sm->TP=&TP;
					if(CTRL)GeneralPen::DeepDegree=-GeneralPen::DeepDegree;			
					sm->Apply();
					if(GeneralPen::SymmetryMask){
						int xm=GeneralPen::SymmetryMask&1?1:0;
						int ym=GeneralPen::SymmetryMask&2?1:0;
						int zm=GeneralPen::SymmetryMask&4?1:0;
						for(int x=0;x<=xm;x++){
							for(int y=0;y<=ym;y++){
								for(int z=0;z<=zm;z++){
									if(x||y||z){
										Vector3D mul(x?-1:1,y?-1:1,z?-1:1);
										int ofs=x+y*2+z*4;                                    
										GeneralPen::PushSymmParam(ofs);									
										sm->CreateVertList();
										sm->Apply();
										GeneralPen::PopSymmParam();									
									}
								}
							}
						}
					}
					if(CTRL)GeneralPen::DeepDegree=-GeneralPen::DeepDegree;			
				}
			}
		}		
    }   
	if( !LMB ){
		/*
		if( GeneralPen::PenPressureType==4 ){
			 if( GeneralPen::NumAccumulatedPoints==0 )sm->KillBackup();
			 //else if(DoItForTool)SurfaceUndo::DropSurfaceUndoWithoutReset();
		}else{
			 sm->KillBackup();
		}
		*/
		sm->KillBackup();
	}
	if(DoItForTool){
		Widgets::lPressed=true;
		BasicTool::ActiveTool()->Process();
		Widgets::lPressed=false;
		//if( GeneralPen::PenPressureType==4 ){
		//	SurfaceUndo::DropSurfaceUndoWithoutReset();
		//}
	}else{
		BasicTool::ActiveTool()->Process();
	}
    rsFlush();    
	if(GeneralPen::SymmetryMask){
		rsEnableZ(true);
		Vector3D v0=(PMS.AABB.minv-PMS.StartShift)*PMS.Scale;
		Vector3D v2=(PMS.AABB.maxv-PMS.StartShift)*PMS.Scale;
		if(GeneralPen::SymmetryMask&1){			
			v0.x=GeneralPen::SymmCenter.x;
			v2.x=GeneralPen::SymmCenter.x;
			Vector3D v1(GeneralPen::SymmCenter.x,v2.y,v0.z);
			Vector3D v3(GeneralPen::SymmCenter.x,v0.y,v2.z);
			rsQuad(v0,v1,v3,v2,0x30FF0000);
			rsLine(v0,v1,0xFFFFFF00);
			rsLine(v1,v2,0xFFFFFF00);
			rsLine(v2,v3,0xFFFFFF00);
			rsLine(v3,v0,0xFFFFFF00);
		}
		if(GeneralPen::SymmetryMask&2){			
			v0.y=GeneralPen::SymmCenter.y;
			v2.y=GeneralPen::SymmCenter.y;
			Vector3D v1(v2.x,GeneralPen::SymmCenter.y,v0.z);
			Vector3D v3(v0.x,GeneralPen::SymmCenter.y,v2.z);
			rsQuad(v0,v1,v3,v2,0x3000FF00);			
			rsLine(v0,v1,0xFFFFFF00);
			rsLine(v1,v2,0xFFFFFF00);
			rsLine(v2,v3,0xFFFFFF00);
			rsLine(v3,v0,0xFFFFFF00);
		}
		if(GeneralPen::SymmetryMask&4){			
			v0.z=GeneralPen::SymmCenter.z;
			v2.z=GeneralPen::SymmCenter.z;
			Vector3D v1(v2.x,v0.y,GeneralPen::SymmCenter.z);
			Vector3D v3(v0.x,v2.y,GeneralPen::SymmCenter.z);
			rsQuad(v0,v1,v3,v2,0x300000FF);			
			rsLine(v0,v1,0xFFFFFF00);
			rsLine(v1,v2,0xFFFFFF00);
			rsLine(v2,v3,0xFFFFFF00);
			rsLine(v3,v0,0xFFFFFF00);
		}
		rsFlush();
	}
    //-------------
    static int T2=GetTickCount()+FreeTime-234;
    int T3=GetTickCount();    
	SetCurrentFont(12);
	int DT=(T2-T3)/1000;
	DrawText(400,0,0xA0ffffff,"Trial version - %d:%02d remains",DT/60,DT%60);
	SetCurrentFont(8);
    try{
	    Widgets::DrawWidgets();
    }catch(...){
        if(T3>T2 && T3<T2+2000){
            MessageBox(NULL,"It is possible that you are using unregistered version!\n Please register!","Licensing error!",0);
            exit(1);
        }
    }
	//BasicTool::ShowProgress(1,10,"FILL_PROGRESS");
}
#define IMPLEMENT_CLASS_FACTORY
#include <ClassEngine.hpp>
#include <DString.hpp>