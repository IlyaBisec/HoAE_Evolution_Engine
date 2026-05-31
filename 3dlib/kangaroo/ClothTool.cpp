#include "StdAfx.h" #include "common.h" 
#include "comms\comms.h"
#include "comms\csplinebezier.h"
#include "UndoMaster.h"
#include "BasicTool.h"
#include "ClothTool.h"
#include "Expand.h"

ClothTool m_ClothTool;

ClothTool::ClothTool(){    
	m_Active=false;
	//InstallTool(&m_ClothTool,"ClothTool");
}
void ClothTool::Process(){
	if(BasicTool::Pick.OnObject && Widgets::lPressed && !BasicTool::Locked){
		Widgets::lPressed=false;
		ExpandSurface es;
		float D=es.Diff;
		es.Diff=10000;
		es.Start(&PMS,PMS.LastPickInfo.FaceID,0,0);
		es.Diff=D;
		int N=PMS.Verts.GetAmount();
		for(int i=0;i<N;i++){
			PMS.Verts[i].TempSelection=1;            
		}
		for(int i=0;i<PMS.FacesInfo.GetAmount();i++){
			if(ExpandSurface::UsedFaces.get(i)){
				PMS.DropFaceToUndo(i);                
			}
			if(ExpandSurface::UsedInsFaces.get(i)){
				scan_key(PMS.Faces,i,DWORD* Vert){
					VertexUV* uv=&PMS.VertsUV[i];					
					PMS.InvalidatePosVertex(uv->PosIndex);
				}scan_end;
			}
		}
		ModPointInfo mp;
		mp.xalong=0;
		mp.yalong=0;
		mp.dx=0;
		mp.dy=0;
		float R1=GeneralPen::PenRadius/1.2/PMS.Scale;
		float R2=R1*GeneralPen::DeepDegree;
		for(int i=0;i<ExpandSurface::Result.GetAmount();i++){
			ExpResult* ex=&ExpandSurface::Result[i];
			PointInfo pi=PMS.GetPoint(ex->Face,ex->x,ex->y);
			ex->Center=pi.pt;
			for(int i=0;i<4;i++){
				PointInfo pi=PMS.GetPoint(ex->Face,ex->x+(i&1)*2-1,ex->y+(i>>1)*2-1);
				if(pi.pt){
					ex->Neib[i]=pi.pt;
					ex->NeibLen[i]=pi.pt->Pos.distance(ex->Center->Pos);
				}else{
                    ex->Neib[i]=NULL;
				}
			}
		}
		for(int i=0;i<1000;i++){
			float MaxF=0;
			float MinL=100000;
			for(int i=0;i<ExpandSurface::Result.GetAmount();i++){
				ExpResult* ex=&ExpandSurface::Result[i];
				static float force=-0.00001;
				ex->Force=Vector3D(0,0,force);
				for(int i=0;i<4;i++)if(ex->Neib[i]){
					float dist=ex->Center->Pos.distance(ex->Neib[i]->Pos);
					if(dist<MinL && dist>0)MinL=dist;
                    ex->Force+=(dist)*(ex->Neib[i]->Pos-ex->Center->Pos);
				}
				float m=ex->Force.norm();
				if(m>MaxF)MaxF=m;
			}
			float co=MinL*0.5/MaxF;
			for(int i=0;i<ExpandSurface::Result.GetAmount();i++){
				ExpResult* ex=&ExpandSurface::Result[i];
				ex->Center->Pos+=ex->Force*co;
			}
		}
		int n=PMS.FacesInfo.GetAmount();
		for(int i=0;i<n;i++){
			if(ExpandSurface::UsedInsFaces.get(i)){			
				PMS.CreateFaceNormals(i);
				PMS.AddDirtyFace(i);
			}
		}		
		for(int i=0;i<N;i++){
			if(ExpandSurface::UsedFaces.get(i)){
				PMS.AddDirtyFace(i);
			}
			VertexPos* vp=&PMS.Verts[i];
			vp->Pos/=vp->W;
			vp->W=1;
			vp->TempSelection=0;
		}
		N=PMS.VertsN.GetAmount();
		for(int i=0;i<N;i++){
			PMS.VertsN[i].normalize();            
		}		
	}
}
IconInfo ClothTool::GetIconInfo(){
	IconInfo I;
	I.IconID=13;
	return I;
}
bool ClothTool::OnGlobalKey(char Key){
	return false;
}
void ClothTool::CreateInterface    (BaseWidget* Where){
	//PenInterface::AddToolHeader("FILL");
	//PenInterface::AddToolSlider(1,256,&ExpandSurface::Diff,0xFFFF0000,"FILL_DIFF","FILL_DIFF_HINT");		
};