#include "StdAfx.h" #include "common.h" 
#include "PenShape.h"
#include "Tablet.h"
#include "UndoMaster.h"
#include "FreezeTool.h"

int PolyMesh::AddLayer(bool MakeActive){
    int maxLayer=-1;
    for(int i=0;i<Layers.GetAmount();i++){
        if(Layers[i]->LayerID>maxLayer)maxLayer=Layers[i]->LayerID;
    }
    OneLayerInfo* L=new OneLayerInfo;
    L->Visible=true;
    L->ColorTransparency=1.0f;
    L->DepthTransparency=1.0f;
    L->LayerID=maxLayer+1;
    Layers.Add(L);
    if(MakeActive)CurrentLayerID=maxLayer+1;
    return maxLayer+1;
}
void PolyMesh::InvalidateLayer(int LayerID){
    for(int i=0;i<FacesInfo.GetAmount();i++){
        FaceInfo* F=&FacesInfo[i];
        for(int j=0;j<F->Layers.GetAmount();j++){
            if(F->Layers[j].LayerID==LayerID){
                scan_key(Faces,i,DWORD* puv){
                    VertexUV* uv=&VertsUV[*puv];
                    InvalidatePosVertex(uv->PosIndex);
                    Verts[uv->PosIndex].TempSelection=true;
                }scan_end;
                AddDirtyFace(i);
                CalcFaceAttributesWithLayers(i);
            }
        }
    }
    for(int i=0;i<FacesInfo.GetAmount();i++){
       FaceInfo* F=&FacesInfo[i];
       for(int j=0;j<F->Layers.GetAmount();j++){
           if(F->Layers[j].LayerID==LayerID){
               CreateFaceNormals(i);
               break;
           }
       }
    }
    for(int i=0;i<Verts.GetAmount();i++){
        VertexPos* vp=&Verts[i];
		vp->TempSelection=0;
		vp->Pos/=vp->W;
		vp->W=1;
    }
    for(int i=0;i<VertsN.GetAmount();i++){
        VertsN[i].normalize();		
    }
}
void PolyMesh::DeleteLayer(int LayerID){
    SetVisible(LayerID,false);    
    for(int i=0;i<FacesInfo.GetAmount();i++){
        FaceInfo* F=&FacesInfo[i];
        for(int j=0;j<F->Layers.GetAmount();j++){
            if(F->Layers[j].LayerID==LayerID){                
                delete[]F->Layers[j].Points;
                F->Layers.Del(j,1);
                j--;
            }
        }
    }
}
OneLayerInfo* PolyMesh::GetLayer(int LayerID){
    for(int i=0;i<Layers.GetAmount();i++){
        if(Layers[i]->LayerID==LayerID)return Layers[i];
    }
    return NULL;
}
void PolyMesh::SetVisible(int LayerID,bool Visible){
    OneLayerInfo* L=GetLayer(LayerID);
    if(L)L->Visible=Visible;
    InvalidateLayer(LayerID);    
}
bool PolyMesh::GetVisible(int LayerID){    
    OneLayerInfo* L=GetLayer(LayerID);
    if(L)return L->Visible;
    return false;
}
void PolyMesh::SetGeometryTransparency(int LayerID,float T){
    InvalidateLayer(LayerID);
    OneLayerInfo* L=GetLayer(LayerID);
    if(L)L->DepthTransparency=T;
}
void PolyMesh::SetColorTransparency(int LayerID,float T){
    InvalidateLayer(LayerID);
    OneLayerInfo* L=GetLayer(LayerID);
    if(L)L->ColorTransparency=T;
}
void PolyMesh::SetActiveLayer(int LayerID){
    CurrentLayerID=LayerID;
}
void PolyMesh::CheckFaceOnEmptyness(int FaceID,int LayerID){
    int n=SubSurfaceSize*SubSurfaceSize;
    FaceInfo* F=&FacesInfo[FaceID];
    for(int j=0;j<F->Layers.GetAmount();j++){
        LayerOnFace* L=&F->Layers[j];
        if(L->LayerID==LayerID){
            OneLayerPoint* pt=L->Points;
            bool onept=false;
            for(int q=0;q<n;q++){
                if(pt->Dv.norm2()>0.0001 || pt->Transparency>0.001){
                    onept=true;
                    break;
                }
                pt++;
            }
            if(!onept){
                //layer is empty
                delete[]L->Points;
                F->Layers.Del(j,1);
                break;
            }
        }
    }
}
LayerOnFace* PolyMesh::GetCurrentLayer(int FaceID){
    return GetSomeLayer(FaceID,CurrentLayerID);    
}
LayerOnFace* PolyMesh::GetSomeLayer(int FaceID,int LayerID){
    int n=SubSurfaceSize*SubSurfaceSize;
    FaceInfo* F=&FacesInfo[FaceID];
    for(int j=0;j<F->Layers.GetAmount();j++){
        LayerOnFace* L=&F->Layers[j];
        if(L->LayerID==LayerID){
            return L;
        }
    }
    LayerOnFace LF;
    OneLayerPoint* pt=LF.Points=new OneLayerPoint[n];
    for(int i=0;i<n;i++){
        pt->Dv=Vector3D::null;
        pt->Color=0;
        pt->Transparency=0;
        pt++;
    }
    LF.LayerID=LayerID;
    LF.Info=GetLayer(LayerID);
    int p=F->Layers.Add(LF);
    return &F->Layers[p];
}
void PolyMesh::CalcFaceAttributesWithLayers(int Face){
    FaceInfo* fi=&FacesInfo[Face];
    int n=SubSurfaceSize*SubSurfaceSize;
    int nl=fi->Layers.GetAmount();
    SurfPoint3D* pt=fi->Points;        
    for(int i=0;i<n;i++){
        pt->Pos=Vector3D::null;
        Vector4D v=Vector4D::null;
        LayerOnFace* L=fi->Layers.GetValues();        
        for(int q=0;q<nl;q++){
            OneLayerInfo* Li=L->Info;
            if(Li->Visible){
                OneLayerPoint* lp=L->Points+i;
                Vector3D dv=lp->Dv;
                dv*=Li->DepthTransparency;
                pt->Pos+=dv;
                v*=1-lp->Transparency;
                Vector4D c1=DW2V4D(lp->Color);
                c1*=lp->Transparency;
                v+=c1;
            }
            L++;
        }
        pt->Color=V4D2DW(v);
        pt++;        
    }
}