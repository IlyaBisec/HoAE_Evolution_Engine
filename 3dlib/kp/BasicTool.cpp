#include "common.h"
#include "BasicTool.h"

BasicPickInfo BasicTool::Pick;
DynArray<BasicTool*> BasicTool::Tools;
bool BasicTool::Locked=false;
void BasicTool::InstallTool(BasicTool* Tool,const char* Name){
    Tool->Name=Name;
    Tools.Add(Tool);
}
bool BasicTool::SomeToolIsActive(){
    for(int i=0;i<Tools.GetAmount();i++){
        if(Tools[i]->IsActive()) return true;
    }
    return false;
}
void BasicTool::DeactivateAll(){
    for(int i=0;i<Tools.GetAmount();i++){
        if(Tools[i]->IsActive()){
            Tools[i]->DeActivate();
        }
    }
}
void BasicTool::ActivateTool(const char* Name){
    DeactivateAll();
    for(int i=0;i<Tools.GetAmount();i++){
        if(Tools[i]->Name.equal(Name)){
            Tools[i]->Activate();            
        }
    }
}
BasicTool* BasicTool::ActiveTool(){
    for(int i=0;i<Tools.GetAmount();i++){
        if(Tools[i]->IsActive())return Tools[i];
    }
    static BasicTool T;
    return &T;
}
BasicTool* BasicTool::GetTool(const char* Name){
    for(int i=0;i<Tools.GetAmount();i++){
        if(Tools[i]->Name.equal(Name)){
            return Tools[i];
        }
    }
    return NULL;

}
IconInfo::IconInfo(){
    IconName="stdbutton.dds";
    int T=IRS->GetTextureID(IconName);
    IconNx=IRS->GetTextureWidth(T)/64;
    IconNy=IRS->GetTextureHeight(T)/64;
    IconID=0;
}
IconInfo BasicTool::GetIconInfo(){
    IconInfo I;    
    return I;
}
int BasicTool::GetToolsAmount      (int Category){
    int N=0;
    for(int i=0;i<Tools.GetAmount();i++){
        if(Tools[i]->GetMenuCategory()==Category)N++;
    }
    return N;
}
BasicTool* BasicTool::GetTool      (int Category,int Index){
    for(int i=0;i<Tools.GetAmount();i++){
        if(Tools[i]->GetMenuCategory()==Category){
            if(!Index)return Tools[i];
            else Index--;
        }
    }
    return NULL;    
}
Vector3D BasicPickInfo::AverageNormal(){
    Vector3D V(0,0,0);
    for(int i=0;i<SurfaceModificator::NearVerts.GetAmount();i++){
        SurfaceModificator::NearVerts[i].N;
        V+=SurfaceModificator::NearVerts[i].N;
    }
    V.normalize();
    return V;
}