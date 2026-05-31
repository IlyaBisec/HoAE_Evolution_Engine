#include "common.h"
#include "comms\comms.h"
#include "comms\csplinebezier.h"
#include "UndoMaster.h"
#include "BasicTool.h"
#include "FreezeTool.h"

FreezeTool m_FreezeTool;

int FreezeTool::FreezeMode=0;//0-none 1-positive curvature 2-negative 3-freeze plane
int FreezeTool::ColorFreezeMode=0;//0-none 1-by dark
float FreezeTool::HeightScale=800;
float FreezeTool::ColorDiff=32;
bool FreezeTool::FeezeView=true;
bool FreezeTool::InvFreeze=false;

FreezeTool::FreezeTool(){
    InstallTool(&m_FreezeTool,"FreezeTool");
    m_Active=false;
}
IconInfo FreezeTool::GetIconInfo(){
    IconInfo I;
    I.IconID=19;
    return I;
}
float FreezeTool::GetFreezeAmount(ModPointInfo* mp){
    float freeze=0;
    if(FreezeMode){
        if(FreezeMode<3){
            int sz=4;        
            Vector3D N;
            Vector3D avg=PMS.GetAveragePN(mp->Face,mp->x,mp->y,sz,N);        
            float dp=N.dot(avg-mp->Curr->Pos);
            static float dp_degree=1000;
            switch(FreezeMode){
            case 2:               
                freeze=dp>0? dp*dp_degree : 0;                
                break;
            case 1:               
                freeze=dp<0? -dp*dp_degree : 0;
                break;            
            }        
        }else{            
            Vector4D v=DW2V4D(mp->Curr->Color);
            Vector3D V(v.x,v.y,v.z);
            float d=V.distance(Vector3D(GeneralPen::Color.x*255,GeneralPen::Color.y*255,GeneralPen::Color.z*255));            
            if(d<ColorDiff){
                freeze=1-d/(ColorDiff+1);
            }else freeze=0;
            freeze*=4;
            if(freeze>1)freeze=1;
            if(FreezeMode==4){
                freeze=1-freeze;
            }            
        }
    } 
    if(InvFreeze)freeze=1-freeze;
    return freeze;
}
void FreezeTool::ModifyPoint(float scale,ModPointInfo& pi,float R,float R1 ){
    if(InvFreeze){
        float m=(1-sqrt(pi.dx*pi.dx+pi.dy*pi.dy))*6;
        if(m>1)m=1;
        float f=GetFreezeAmount(&pi);        
        if(f<0)f=0;
        if(f>1)f=1;
        f*=m;
        f=1-f;
        pi.Curr->Freeze=min(pi.Curr->Freeze,f);
    }else{
        float m=(1-sqrt(pi.dx*pi.dx+pi.dy*pi.dy))*6;
        if(m>1)m=1;
        float f=GetFreezeAmount(&pi);
        f=1-f;
        if(f<0)f=0;
        if(f>1)f=1;
        f*=m;
        pi.Curr->Freeze=max(pi.Curr->Freeze,f);
    }
}
void FreezeTool::Apply(){
    SurfaceModificator::ApplyModFunfcion(&ModifyPoint);
}
void FreezeTool::Process(){
    InvFreeze=!!(GetKeyState(VK_CONTROL)&0x8000);    
    if(BasicTool::Pick.OnObject && Widgets::lPressed && !BasicTool::Locked){
        Apply();
    }       
}
void FreezeTool::UnFreeze(float scale,ModPointInfo& pi,float R,float R1 ){
    float m=(1-sqrt(pi.dx*pi.dx+pi.dy*pi.dy))*6;
    if(m>1)m=1;
    pi.Curr->Freeze=min(pi.Curr->Freeze,1-m);
}

void FreezeTool::UnFreezeAll(){
    FreezeUndo::DropFreezeUndo();
    bool ch=false;    
    int n=PMS.SubSurfaceSize*PMS.SubSurfaceSize;
    for(int i=0;i<PMS.FacesInfo.GetAmount();i++){
        SurfPoint3D* s=PMS.FacesInfo[i].Points;
        for(int j=0;j<n;j++){            
            if(s->Freeze!=0){
                s->Freeze=0;
                ch=true;
            }
            s++;
        }
    }
    if(ch){
        PMS.SetAllDirty();
        PMS.UpdateDirtyFaces(true);
    }
}
void FreezeTool::FreezeAll(){
    FreezeUndo::DropFreezeUndo();
    bool ch=false;
    ModPointInfo mp;
    mp.dx=0;
    mp.dy=0;
    mp.StrokeLength=0;
    mp.xalong=0;
    mp.yalong=0;    
    int n=PMS.SubSurfaceSize;
    for(int i=0;i<PMS.FacesInfo.GetAmount();i++){
        SurfPoint3D* s=PMS.FacesInfo[i].Points;
        mp.Face=i;
        for(int y=0;y<n;y++){            
            for(int x=0;x<n;x++){
                mp.Orig=mp.Curr=s;
                mp.N=s->N;
                mp.x=x;
                mp.y=y;
                float F=1-GetFreezeAmount(&mp);
                if(F<0)F=0;
                if(F>1)F=1;
                if(s->Freeze!=F){
                    s->Freeze=F;
                    ch=true;
                }
                s++;
            }
        }
    }
    if(ch){
        PMS.SetAllDirty();
        PMS.UpdateDirtyFaces(true);
    }
}
bool FreezeTool::OnGlobalKey(char Key){
    if(Key=='D' && (GetKeyState(VK_CONTROL)&0x8000)){
        UnFreezeAll();
        return true;
    }
    if(Key==-112){//NUMLOCK
        FeezeView=!FeezeView;
        return true;
    }
    return false;
}
bool FreezeTool::OnKey(char Key){
    if(Key==13){//ENTER
        FreezeAll();
        return true;
    }
    return false;    
}
bool FreezeTool::opFreeze(){
    FreezeAll();
    return true;
}
bool FreezeTool::opUnFreeze(){
    UnFreezeAll();
    return true;
}
bool FreezeTool::opToggleFreezeView(){
    FeezeView=!FeezeView;
    return true;
}