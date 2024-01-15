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
void BasicTool::ShowProgress(int Stage,int NStages,const char* Message){	
	Rct R=IRS->GetViewPort();
	rsRestoreShader();
	SetCurrentFont(12);
	int W=200;
	int H=32;
	int dy=0;
	int TW=0;
	if(Message){
		TW=GetTextWidth("%s",Message);
		W=TW+32;
		dy=GetTextHeight()+7;
		H+=dy;
	}
	W=max(W,400);
	Rct R1((R.w-W)/2,(R.h-H)/2,W,H+1);	
	DrawFrame(R1,-1,Rct(0,0,1,1),0xFF808080);
	if(Message){
		DrawText(R1.x+(R1.w-TW)/2-1,R1.y+4+1,0x80000000,"%s",Message);
		DrawText(R1.x+(R1.w-TW)/2,R1.y+4,0xFFFFFFFF,"%s",Message);
	}
	FlushText();

	float w1=W-10;
	float w2=w1*Stage/NStages;
	Rct R2((R.w-w1)/2,R1.y+dy,w2,23);

    R2.w=floorf(R2.w);
	R2.h=floorf(R2.h);

	DrawFrame(R2,-1,Rct(0,0,1,1),0xFFFFFF00);	
	rsFlush();
	SetCurrentFont(8);

	extern bool g_bKangaroo;
	g_bKangaroo = false;

	extern int Diplimit;
	Diplimit=100000;    
	IRS->Flush();	
	IRS->EndFrame();

	g_bKangaroo = true;
}
