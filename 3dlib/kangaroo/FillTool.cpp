#include "StdAfx.h" #include "common.h" 
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
		Widgets::lPressed=false;
        ExpandSurface es;
		if(es.Start(&PMS,PMS.LastPickInfo.FaceID,0,0)){		
			int ns=ExpandSurface::Result.GetAmount();
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
				if(ns>200000 && (i & 1023) == 1000){
					ShowProgress(i,PMS.FacesInfo.GetAmount(),TextManager::GetText("FILL_UNDO_PROGRESS"));
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
                LayerOnFace* LF=PMS.GetCurrentLayer(ex.Face);
                int ofs=ex.x+ex.y*PMS.SubSurfaceSize;
                mp.Orig=mp.Curr=LF->Points+ofs;				
                mp.Base=pi.pt;
				mp.Face=ex.Face;
				mp.N=pi.pt->N;
				mp.x=ex.x;
				mp.y=ex.y;
				GeneralPen::ModifyPoint(PMS.Scale,mp,R1,R1*ex.W);
				if(ns>200000 && (i & 65535) == 65000){
					ShowProgress(i,ExpandSurface::Result.GetAmount(),TextManager::GetText("FILL_PROGRESS"));
				}
			}
			int n=PMS.FacesInfo.GetAmount();
            for(int i=0;i<n;i++){
				if(ExpandSurface::UsedInsFaces.get(i)){			
                    PMS.CalcFaceAttributesWithLayers(i);					
				}
			}
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
	PenInterface::AddToolSlider(1,256,&ExpandSurface::Diff,RedColor,"FILL_DIFF","FILL_DIFF_HINT");		
};