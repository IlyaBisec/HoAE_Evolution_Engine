#include "common.h"
#include "comms\comms.h"
#include "comms\csplinebezier.h"
#include "UndoMaster.h"
#include "BasicTool.h"
#include "PickTool.h"

PickTool m_PickTool;

Vector4D PickTool::Summ;
float PickTool::W;

PickTool::PickTool(){    
    m_Active=false;
    InstallTool(&m_PickTool,"PickTool");
}
void PickTool::ModifyPoint(float scale,ModPointInfo& pi,float R,float R1 ){
    float r=sqrt(pi.dx*pi.dx+pi.dy*pi.dy);
    if(r<1){
        float Deg=1;
        if(GeneralPen::CurrentShapePtr){
            Deg=GeneralPen::CurrentShapePtr->GetWeight(pi.dx,pi.dy);
            Deg*=1-pi.Curr->Freeze;
        }
	    Vector4D CL=DW2V4D(pi.Orig->Color);
        CL*=Deg;
        Summ+=CL;
        W+=Deg;
    }
}    
void PickTool::Process(){
    if(BasicTool::Pick.OnObject && Widgets::lPressed && !BasicTool::Locked){
        Summ=Vector4D::null;
        W=0;
        SurfaceModificator::ApplyModFunfcion(&ModifyPoint,false);
        if(W>0){
            Summ/=W*255;
            GeneralPen::Color=Summ;
            GeneralPen::Color.w=255;
            GeneralPen::SpecularDegree=Summ.w;
        }
    }
}
IconInfo PickTool::GetIconInfo(){
    IconInfo I;
    I.IconID=12;
    return I;
}
bool PickTool::OnGlobalKey(char Key){
    if(Key=='P'){
        BasicTool::ActivateTool("PickTool");
        return true;
    }
    return false;
}