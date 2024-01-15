#include "common.h"
#include "comms\comms.h"
#include "comms\csplinebezier.h"
#include "UndoMaster.h"
#include "BasicTool.h"
#include "FillTool.h"
#include "Expand.h"

FillTool m_FillTool;

FillTool::FillTool(){    
    m_Active=false;
    InstallTool(&m_FillTool,"FillTool");
}
void FillTool::Process(){
    if(BasicTool::Pick.OnObject && Widgets::lPressed && !BasicTool::Locked){
		Widgets::lPressed=true;
        ExpandSurface es;
        es.Start(&PMS,PMS.LastPickInfo.FaceID,0,0);
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
            ExpResult& ex=ExpandSurface::Result[i];
            PointInfo pi=PMS.GetPoint(ex.Face,ex.x,ex.y);
            mp.Curr=pi.pt;
            mp.Orig=pi.pt;
            mp.Face=ex.Face;
            mp.N=pi.pt->N;
            mp.x=ex.x;
            mp.y=ex.y;
            GeneralPen::ModifyPoint(PMS.Scale,mp,R1,R1*ex.W);
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
		/*
		for(int i=0;i<n;i++){
			if(ExpandSurface::UsedFaces.get(i)){				
				scan_key(PMS.Faces,i,DWORD* Vert){
					VertexUV* uv=&PMS.VertsUV[i];
					VertexPos* vp=&PMS.Verts[i];
					PMS.Verts[uv->PosIndex].TempSelection=0;
					vp->Pos/=vp->W;
					vp->W=1;
					PMS.VertsN[uv->NIndex].normalize();
				}scan_end;               				
				PMS.AddDirtyFace(i);
			}
		}
		*/
    }
}
IconInfo FillTool::GetIconInfo(){
    IconInfo I;
    I.IconID=13;
    return I;
}
bool FillTool::OnGlobalKey(char Key){
    //if(Key=='P'){
    //    BasicTool::ActivateTool("FillTool");
    //    return true;
    //}
    return false;
}
void FillTool::CreateInterface    (BaseWidget* Where){
	PenInterface::AddToolHeader("FILL");
	PenInterface::AddToolSlider(1,256,&ExpandSurface::Diff,0xFFFF0000,"FILL_DIFF","FILL_DIFF_HINT");		
};