#include "StdAfx.h" #include "common.h" 
#include "comms\comms.h"
#include "comms\csplinebezier.h"
#include "BasicTool.h"
#include "CurveTool.h"

CurveTool m_CurveTool;
int	CurveTool::CurveProfile=0;
float CurveTool::CurveProfileLength=1.0f;

CurveTool::CurveTool(){
    InstallTool(&m_CurveTool,"CurveTool");
    m_Active=false;
}
Vector3D CurveTool::GetProfileShape(float T){
	Vector3D pr(1,1,1);
	if(T>0.5)T=1-T;
	T/=CurveProfileLength;
	if(T>0.5)T=0.5;
	T*=2;
	switch(CurveProfile){	
	case 1:
        pr.y=pr.x=(1-(1-T)*(1-T));
		pr.y*=pr.y;
		break;
	case 2:
		pr.y=(1-(1-T)*(1-T));
		pr.y*=pr.y;
		break;
	}
	if(pr.x<0.01)pr.x=0.01;
	if(pr.y<0.01)pr.y=0.01;
	return pr;
}
void CurveTool::CreateSpline(){
	if(PointsArray.Count()>1){
		cList<cSplineBezier3::Knot> P;
		cList<cSplineBezier3::Knot> N;
		NRH.Free();
		float L=0;
		cVec3 Prev=PointsArray[0];
        float Rp=RadiusArray[0];
		for(int i=0;i<PointsArray.Count();i++){
            cVec3 p=PointsArray[i];
			if(i>0){
				L+=(p-Prev).Length()*2/(Rp+RadiusArray[i]);
			}
			cSplineBezier3::Knot K;
			K.Point=p;
			K.Time=L;
			P.Add(K);
			K.Point=NormalsArray[i];
			N.Add(K);
            Prev=p;
            Rp=RadiusArray[i];
		}
		for(int i=0;i<PointsArray.Count();i++){
			P[i].Time/=L;
			N[i].Time/=L;
			if(i==PointsArray.Count()-1){
				P[i].Time=1;
				N[i].Time=1;
			}
            cVec5 D;
            cVec3& v3=N[i].Point;
            D[0]=v3.x;
            D[1]=v3.y;
            D[2]=v3.z;
            D[3]=RadiusArray[i];
            D[4]=HeightArray[i];            
			NRH.AddValue(N[i].Time,D);
		}
		Points.SetFromKnots(P.ToPtr(),PointsArray.Count());
		SplineLength=L;		
	}
}

void CurveTool::DrawSpline(){
    int n=PointsArray.Count();
    if(n>1){
        int nsegm=16;
        int np=(n-1)*30;
        int nv=np*nsegm;        
        if(nv>40000){
            np=40000/nsegm;
            nv=np*nsegm;
        }
        int ni=(np-1)*nsegm*6;
        static int prevN=0;
        static BaseMesh* M=NULL;
        if(M==NULL || prevN!=nv){
            if(M)delete(M);
            M=new BaseMesh;
            M->create(nv,ni,vfVertexN);
            WORD* idx=M->getIndices();
            int ofs=0;
            int v0=0;
            for(int i=0;i<np-1;i++){
                for(int q=0;q<nsegm;q++){
                    int q1=(q+1)%nsegm;
                    idx[ofs++]=v0+q1;
                    idx[ofs++]=v0+q;
                    idx[ofs++]=v0+q+nsegm;
                    idx[ofs++]=v0+q1+nsegm;
                    idx[ofs++]=v0+q1;
                    idx[ofs++]=v0+q+nsegm;

                }
                v0+=nsegm;
            }
            VertexN* vn=(VertexN*)M->getVertexData();
            float dang=c_PI*2/nsegm;
            float dang2=dang*2;            
            for(int i=0;i<np;i++){
                float T=float(i)/(np-1);
				Vector3D PR=GetProfileShape(T);
                Vector3D p=_Vector3D(Points.GetValue(T));
                Vector3D dir=_Vector3D(Points.GetDerivativeGlobal(T));
                dir.normalize();                
                cVec5 nrh=NRH.GetCurrentValue(T);                                                
                Vector3D n(nrh[0],nrh[1],nrh[2]);				
                Vector3D t;
                t.cross(dir,n);
				t*=PR.x;
				n*=PR.y;
                float ang=0;
                for(int q=0;q<nsegm;q++){
                    Vector3D vv=p+n*nrh[4]*cos(ang)+t*nrh[3]*sin(ang);                    
                    vn->x=vv.x;
                    vn->y=vv.y;
                    vn->z=vv.z;
                    vn->u=T*SplineLength;
                    vn->v=float(q)/nsegm;
                    ang+=dang;
                    vn++;
                }
            }
            M->setNVert(nv);
            M->setNInd(ni);
            M->setNPri(ni/3);
            M->calcNormals();            
            M->setShader(IRS->GetShaderID("reflect"));
        }        
        rsEnableZ(false);		
		cVec3 Prev;
		for(int i=0;i<=np;i++){
			float t=float(i)/np;
            cVec3 p=Points.GetValue(t);
			if(i>1){
				rsLine(*((Vector3D*)(&p)),*((Vector3D*)(&Prev)),0xFFFFFF00,0xFFFFFF00);
			}
			Prev=p;
        }					
		for(int i=0;i<n;i++){
			cVec3 p=PointsArray[i];
			DrawSphere(Sphere(*((Vector3D*)(&p)),RadiusArray[i]/2),0xFF0000FF,0,12);
		}
		// Drawing Bezier spline segs:
		/*
		for(int nSeg = 0; nSeg < Points.GetNumSegments(); nSeg++) {
			const cSegBezier3 &Seg = Points.GetCurve(nSeg);
			for(int k = 0; k < 4; k++) {
				DrawSphere(Sphere(*((Vector3D *)(&Seg.GetPoint(k))), 8), 0xff0000ff, 0, 4);
			}
		}
		*/
		rsFlush();
        rsEnableZ(true);
        DrawBM(*M);	
    }else{
        for(int i=0;i<n;i++){
			cVec3 p=PointsArray[i];
			DrawSphere(Sphere(*((Vector3D*)(&p)),RadiusArray[i]/2),0xFF0000FF,0,32);
		}
        rsFlush();
    }
}
bool CurveTool::IsSplined(){
	return PointsArray.Count()>1;
}
cVec2 CurveTool::TransformPointBySpline(cVec3 p,cVec2& ofsAlong,cVec2& ofsRadial,cVec3& Norm,float& dz){
	ofsAlong=cVec2(p.x,p.y);
	dz=1;
	if(IsSplined()){
		static float Prec=0.0001;
		cSplineBezier3::Place place=Points.CalcClosestPointPlaceAccurate(p,Prec);
		float time=Points.GetTimeFromPlace(place);
		time=cMath::Clamp01(time);
		Vector3D PR=GetProfileShape(time);
        cVec5 nc4=NRH.GetCurrentValue(time);
        cVec3 nc(nc4.v[0],nc4.v[1],nc4.v[2]);
        float R=nc4.v[3]*PR.x;
        float H=nc4.v[4]*PR.y;
		cVec3 pos=Points.GetValue(time);
		cVec3 dir=Points.GetDerivativeGlobal(time);
		dir.Normalize();
		p-=pos;        
		if(time<=0){
			ofsAlong.x=cVec3::Dot(dir,p)/R;
		}else
		if(time>=1){
			ofsAlong.x=SplineLength+cVec3::Dot(dir,p)/R;
		}else ofsAlong.x=time*SplineLength;
		
		cVec3 bin=cVec3::Cross(nc,dir);
		ofsAlong.y=cVec3::Dot(p,bin)/R;
        ofsRadial.y=ofsAlong.y;
        ofsRadial.x=cVec3::Dot(p,dir)/R;
        Norm=nc;
		dz=cVec3::Dot(p,Norm)/R;
        return cVec2(R,H);
	}	
    return cVec2(1,1);
}
void CurveTool::AddPoint(cVec3 n,cVec3 p,float R,float H){
	for(int i=0;i<PointsArray.Count();i++){
		float D=cVec3::Distance(PointsArray[i],p);
		if(D<0.01){
			PointsArray[i]=p;
			NormalsArray[i]=n;
			CreateSpline();
			return;
		}        
	}
	if(PointsArray.Count()==0 || cVec3::Distance(PointsArray[PointsArray.Count()-1],p)>5){
		PointsArray.Add(p);
		NormalsArray.Add(n);
        RadiusArray.Add(R);
        HeightArray.Add(H);
		CreateSpline();
	}
}
void CurveTool::DelLast(){
	if(PointsArray.Count()){
		PointsArray.RemoveBack();
		NormalsArray.RemoveBack();
        RadiusArray.RemoveBack();
        HeightArray.RemoveBack();
		CreateSpline();
	}
}
void CurveTool::ClearAllPoints(){
    PointsArray.Clear();
	NormalsArray.Clear();
    RadiusArray.Clear();
    HeightArray.Clear();
}
bool CurveTool::OnLMB_Up(){
    if(BasicTool::Pick.OnObject){
        AddPoint(_cVec3(BasicTool::Pick.AverageNormal()),_cVec3(BasicTool::Pick.PickPos),BasicTool::Pick.Radius,GeneralPen::DeepDegree*BasicTool::Pick.Radius);
    }
    return false;
}

void CurveTool::Draw(){	
	int xm=GeneralPen::SymmetryMask&1?1:0;
	int ym=GeneralPen::SymmetryMask&2?1:0;
	int zm=GeneralPen::SymmetryMask&4?1:0;
	for(int x=0;x<=xm;x++){
		for(int y=0;y<=ym;y++){
			for(int z=0;z<=zm;z++){					
				Vector3D mul(x?-1:1,y?-1:1,z?-1:1);
				for(int i=0;i<PointsArray.Count();i++){
					Vector3D v=_Vector3D(PointsArray[i]);
					GeneralPen::TransformSymm(v,mul);
					PointsArray[i]=_cVec3(v);
					v=_Vector3D(NormalsArray[i]);
					v*=mul;
					NormalsArray[i]=_cVec3(v);
				}
				CreateSpline();
				DrawSpline();
				for(int i=0;i<PointsArray.Count();i++){
					Vector3D v=_Vector3D(PointsArray[i]);
					GeneralPen::TransformSymm(v,mul);
					PointsArray[i]=_cVec3(v);
					v=_Vector3D(NormalsArray[i]);
					v*=mul;
					NormalsArray[i]=_cVec3(v);
				}					
			}
		}
	}	
	CreateSpline();	
}
void CurveTool::ApplySplineToSurface(){
    AABoundBox AB(FLT_MAX,FLT_MAX,FLT_MAX,-FLT_MAX,-FLT_MAX,-FLT_MAX);
    for(int i=0;i<PointsArray.Count();i++){
        cVec3 cC=PointsArray[i];
        Vector3D C(cC.x,cC.y,cC.z);
        AABoundBox ABC(Vector3D(cC.x,cC.y,cC.z),RadiusArray[i]*2);
        AB.Extend(ABC);
    }
    SurfaceModificator::CreateVolumePick(AB);
    PolyMesh* M=&PMS;

    for(int i=0;i<SurfaceModificator::InsideVerts.GetAmount();i++){
		M->Verts[SurfaceModificator::InsideVerts[i]].TempSelection=1;
	}
    for(int i=0;i<SurfaceModificator::NearFaces.GetAmount();i++){		
		DWORD Face=SurfaceModificator::NearFaces[i];
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
			if(pi0){
				int sz=M->SubSurfaceSize;
				for(int y=0;y<sz;y++){
					for(int x=0;x<sz;x++){						
						mpi.x=x;
						mpi.y=y;
                        float w=0;
						float dz=0;
                        cVec2 along;
                        cVec2 radial;
                        cVec3 nrm;
                        cVec3 pos3=_cVec3(mpi.Base->Pos*M->Scale);
                        cVec2 rh=TransformPointBySpline(pos3,along,radial,nrm,dz);
						if(abs(dz)<1){
							mpi.N=Vector3D(nrm.x,nrm.y,nrm.z);						
							mpi.dx=radial.x;
							mpi.dy=radial.y;
							mpi.xalong=along.x;
							mpi.yalong=along.y;
							mpi.StrokeLength=0;
							rh.y*=1-dz*dz/4;
							GeneralPen::ModifyPoint(M->Scale,mpi,rh.x/M->Scale,rh.y/M->Scale);
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
		if( (i& 63)==60 )BasicTool::ShowProgress(i,SurfaceModificator::NearFaces.GetAmount(),TextManager::GetText("CURVE_PROGRESS"));
	}
	for(int i=0;i<SurfaceModificator::NearVerts.GetAmount();i++){
		PickedVertInfo& pi=SurfaceModificator::NearVerts[i];
		M->InvalidatePosVertex(pi.PosVertexID);
	}
    for(int i=0;i<SurfaceModificator::NearFaces.GetAmount();i++){
		DWORD Face=SurfaceModificator::NearFaces[i];
        M->CalcFaceAttributesWithLayers(Face);		
	}
	for(int i=0;i<SurfaceModificator::NearFaces.GetAmount();i++){
		DWORD Face=SurfaceModificator::NearFaces[i];
		M->CreateFaceNormals(Face);
		M->AddDirtyFace(Face);
	}
	for(int i=0;i<SurfaceModificator::NearVerts.GetAmount();i++){
		PickedVertInfo& pi=SurfaceModificator::NearVerts[i];
		//M->VertsN[pi.NVertexID].normW();
		VertexPos* vp=&M->Verts[pi.PosVertexID];		
		vp->Pos/=vp->W;
		M->VertsN[pi.NVertexID].normalize();
		vp->W=1;		
	}
	for(int i=0;i<SurfaceModificator::InsideVerts.GetAmount();i++){
		VertexPos* vp=&M->Verts[SurfaceModificator::InsideVerts[i]];
		vp->TempSelection=0;
		vp->Pos/=vp->W;
		vp->W=1;
	}    	
}
bool CurveTool::OnKey(char KeyCode){
    if(KeyCode==13){
        bool CTRL=!!(GetKeyState(VK_CONTROL)&0x8000);
        float D=GeneralPen::DeepDegree;
        if(CTRL){
            GeneralPen::DeepDegree=-1;
        }else GeneralPen::DeepDegree=1;		
		int xm=GeneralPen::SymmetryMask&1?1:0;
		int ym=GeneralPen::SymmetryMask&2?1:0;
		int zm=GeneralPen::SymmetryMask&4?1:0;
		for(int x=0;x<=xm;x++){
			for(int y=0;y<=ym;y++){
				for(int z=0;z<=zm;z++){						
					Vector3D mul(x?-1:1,y?-1:1,z?-1:1);
                    DynArray<cVec3> pt0;
					for(int i=0;i<PointsArray.Count();i++){
						Vector3D n=_Vector3D(NormalsArray[i]);
						n*=mul;
						NormalsArray[i]=_cVec3(n);

						Vector3D v=_Vector3D(PointsArray[i]);
						pt0.Add(PointsArray[i]);
						GeneralPen::TransformSymm(v,mul);
						PointsArray[i]=_cVec3(PMS.PutPointOnSurface(v,n));						
					}
					CreateSpline();
					ApplySplineToSurface();
					for(int i=0;i<PointsArray.Count();i++){
						PointsArray[i]=pt0[i];
						Vector3D n=_Vector3D(NormalsArray[i]);
						n*=mul;
						NormalsArray[i]=_cVec3(n);
					}
				}
			}
		}		
		CreateSpline();
        GeneralPen::DeepDegree=D;
        return true;
    }
    if(KeyCode==27){
        ClearAllPoints();
        return true;
    }
    if(KeyCode==8){
        DelLast();
        return true;
    }
    return false;
}
IconInfo CurveTool::GetIconInfo(){
    IconInfo I;
    I.IconID=18;
    return I;
}