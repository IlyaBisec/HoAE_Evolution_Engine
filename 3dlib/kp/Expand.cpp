#include "common.h"
#include "Expand.h"

DynArray<SurfPix>   ExpandSurface::Stack1;
DynArray<SurfPix>   ExpandSurface::Stack2;
DynArray<ExpResult> ExpandSurface::Result;
UnlimitedBitset     ExpandSurface::Used;
UnlimitedBitset     ExpandSurface::UsedFaces;
float               ExpandSurface::Diff=32;
Vector4D            ExpandSurface::SrcColor;
UnlimitedBitset		ExpandSurface::UsedInsFaces;

float ExpandSurface::GetDiff(SurfPoint3D* Src,SurfPoint3D* Curr){    
    Vector4D v2=DW2V4D(Curr->Color);
    float dv=v2.distance(SrcColor);
    if(dv<Diff){
        dv=2-2*dv/Diff;
        if(dv>1)dv=1;
        return dv;
    }
    return 0;
}
void ExpandSurface::Start(PolyMesh* M,DWORD Face,int x,int y){
    Stack1.FastClear();
    Stack2.FastClear();
    Result.FastClear();	
	UsedInsFaces.clear();
    Used.clear();    
    UsedFaces.clear();
    SurfPix sp;
    sp.Face=Face;
    sp.x=x;
    sp.y=y;
    ExpResult er;
    er.sp(sp);
    er.W=1;
    Result.Add(er);
    Stack1.Add(sp);
    PointInfo PC=M->GetPoint(Face,x,y);       
    SrcColor=DW2V4D(PC.pt->Color);
    DynArray<SurfPix>* CST=&Stack1;
    DynArray<SurfPix>* CSN=&Stack2;    
    DWORD sz=M->SubSurfaceSize;
    bool change=false;
    do{
        change=false;
        int n=CST->GetAmount();
        for(int p=0;p<n;p++){
            SurfPix& sp=(*CST)[p];            
            DWORD F=sp.Face;
            int x=sp.x;
            int y=sp.y;

            PointInfo PT[4]=
            {   M->GetPoint(F,x-1,y),
                M->GetPoint(F,x+1,y),
                M->GetPoint(F,x,y-1),
                M->GetPoint(F,x,y+1)
            };
            for(int j=0;j<4;j++){
                PointInfo& P=PT[j];
                DWORD DW=(P.Face*sz+P.y)*sz+P.x;
                if(!Used.get(DW)){
                    Used.set(DW,true);
                    float w=GetDiff(PC.pt,P.pt);
                    if(w>0){
                        ExpResult er;
                        SurfPix sp;
                        sp.Face=P.Face;
                        sp.x=P.x;
                        sp.y=P.y;
                        CSN->Add(sp);
                        er.sp(sp);
                        er.W=w;
                        Result.Add(er);                                                
                        change=true;						
                    }
					UsedInsFaces.set(P.Face,true);
					UsedFaces.set(P.Face,true);
                }
            }
        }
        CST->FastClear();
        swap(CST,CSN);
    }while(change);
	int n=M->FacesInfo.GetAmount();	
	for(int i=0;i<n;i++){
		if(UsedInsFaces.get(i)){
			FaceInfo* F=&M->FacesInfo[i];
			DWORD FS[4]={F->UpFace,F->DnFace,F->LFace,F->RFace};
			for(int k=0;k<4;k++){
				if(FS[k]!=-1){
					UsedFaces.set(FS[k],true);
					FaceInfo* F2=&M->FacesInfo[FS[k]];
					if(F2->UpFace!=-1)UsedFaces.set(F->UpFace,true);
					if(F2->DnFace!=-1)UsedFaces.set(F->DnFace,true);
					if(F2->LFace!=-1)UsedFaces.set(F->LFace,true);
					if(F2->RFace!=-1)UsedFaces.set(F->RFace,true);
				}				
			}
		}
	}
    Stack1.FastClear();
    Stack2.FastClear();    
    Used.clear();
}