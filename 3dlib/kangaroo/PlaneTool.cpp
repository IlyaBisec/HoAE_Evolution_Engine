#include "common.h"
#include "comms\comms.h"
#include "comms\csplinebezier.h"
#include "UndoMaster.h"
#include "BasicTool.h"
#include "PlaneTool.h"

PlaneTool m_PlaneTool;


float PlaneTool::W;
Vector3D PlaneTool::avgN;
float PlaneTool::avgS;

PlaneTool::PlaneTool(){    
    m_Active=false;
    InstallTool(&m_PlaneTool,"PlaneTool");
}
void PlaneTool::ModifyPoint(float scale,ModPointInfo& pi,float R,float R1 ){
    float r=sqrt(pi.dx*pi.dx+pi.dy*pi.dy);
    if(r<1){
        float Deg=1;
        if(GeneralPen::CurrentShapePtr){
            Deg=GeneralPen::CurrentShapePtr->GetWeight(pi.dx,pi.dy);
            Deg*=1-pi.Base->Freeze;
        }
        avgN+=pi.Base->N*Deg;	    
    }
}    
void PlaneTool::ModifyPoint1(float scale,ModPointInfo& pi,float R,float R1 ){
    float r=sqrt(pi.dx*pi.dx+pi.dy*pi.dy);
    if(r<1){
        float Deg=1;
        if(GeneralPen::CurrentShapePtr){
            Deg=GeneralPen::CurrentShapePtr->GetWeight(pi.dx,pi.dy);
            Deg*=1-pi.Base->Freeze;
        }
        avgS+=pi.Base->Pos.dot(avgN)*Deg;
        W+=Deg;
    }
}    
void PlaneTool::ModifyPoint2(float scale,ModPointInfo& pi,float R,float R1 ){
    float r=sqrt(pi.dx*pi.dx+pi.dy*pi.dy);
    if(r<1){
        float Deg=1;
        if(GeneralPen::CurrentShapePtr){
            Deg=GeneralPen::CurrentShapePtr->GetWeight(pi.dx,pi.dy);
            Deg*=1-pi.Base->Freeze;
        }
        float D=pi.Base->Pos.dot(avgN);
        Vector3D P=pi.Base->Pos+(avgS-D)*avgN;
        Vector3D vc=pi.Base->Pos*(1-Deg)+P*Deg;
        pi.Curr->Dv+=vc-pi.Base->Pos;        
    }
}    
void PlaneTool::Process(){
    if(BasicTool::Pick.OnObject && Widgets::lPressed && !BasicTool::Locked){
        avgN=Vector3D(0,0,0);
        SurfaceModificator::ApplyModFunfcion(&ModifyPoint,false);
        if(avgN.norm()>0){
            avgN.normalize();
            avgS=0;
            W=0;
            SurfaceModificator::ApplyModFunfcion(&ModifyPoint1,false);
            if(W>0){
                avgS/=W;
                SurfaceModificator::ApplyModFunfcion(&ModifyPoint2,true);
            }
        }
    }
}
IconInfo PlaneTool::GetIconInfo(){
    IconInfo I;
    I.IconID=27;
    return I;
}
bool PlaneTool::OnGlobalKey(char Key){    
    return false;
}