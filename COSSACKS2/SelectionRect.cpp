#include "stdheader.h"
#include "Extensions.h"
#include "ITerrain.h"
class OneSelectionType:public BaseClass{
public:
	_str Name;
	int   TextureID;
	int   x;
	int   y;
	int   Lx;
	int   Ly;
	int   CenterX;
	int   CenterY;
	float StandartScale;
	float RotationSpeed;
	bool  DontRotateWithUnit;
	bool  Additive;

	SAVE(OneSelectionType);
		REG_AUTO(Name);
		REG_MEMBER(_TextureID,TextureID);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_int,Lx);
		REG_MEMBER(_int,Ly);
		REG_MEMBER(_int,CenterX);
		REG_MEMBER(_int,CenterY);
		REG_MEMBER(_float,StandartScale);
		REG_MEMBER(_float01,RotationSpeed);
		REG_MEMBER(_bool,DontRotateWithUnit);
		REG_MEMBER(_bool,Additive);
	ENDSAVE;
	const char* GetThisElementView(const char*){
		char* s=GetGlobalBuffer();
		sprintf(s,"{tex %s 0 0 %d %d  %d %d %d %d} %s",IRS->GetTextureName(TextureID),Lx,Ly,x,y,x+Lx,y+Ly,Name.pchar());
		return s;
	}
};
class SelectionTypeList:public BaseClass{
public:
	ClonesArray<OneSelectionType> Selections;
	SAVE(SelectionTypeList);
		REG_AUTO(Selections);
	ENDSAVE;
};
SelectionTypeList SelTypes;
void RegSelEditor(){
	REG_CLASS(OneSelectionType);
	SelTypes.SafeReadFromFile("Dialogs\\SelType.xml");
	AddStdEditor("SelectionTypeEditor",&SelTypes,"Dialogs\\SelType.xml",RCE_DEFAULT);
	Enumerator* E=ENUM.Get("TEXTURE_CURSOR_TYPES");
    E->Clear();
	E->Add("---nothing---",0xFFFFFFFF);
	for(int i=0;i<SelTypes.Selections.GetAmount();i++){
		char* s=SelTypes.Selections[i]->Name.pchar();
		if(!s)s="NULL";
        E->Add(s,i);
	}
}
int GetSelType(char* name){    
	for(int i=0;i<SelTypes.Selections.GetAmount();i++){
		char* s=SelTypes.Selections[i]->Name.pchar();
		if(s&&!strcmp(s,name))return i;
	}
	return -1;
}
bool DrawSelPatch(float x,float y,int Type,float ScaleX,float ScaleY,DWORD Color){
	if(Type<SelTypes.Selections.GetAmount()){
		OneSelectionType* ST=SelTypes.Selections[Type];
		float r=0;
		if(ST->RotationSpeed>0.0001){
			static int T=GetTickCount();
			int DT=float(GetTickCount()-T)/10;
			r=DT*ST->RotationSpeed;
		}
		float w=IRS->GetTextureWidth(ST->TextureID);
		float h=IRS->GetTextureHeight(ST->TextureID);
			ITerra->DrawPatch(x,y,ST->Lx*ScaleX*ST->StandartScale,ST->Ly*ScaleY*ST->StandartScale,r,Rct(float(ST->x)/w,float(ST->y)/h,float(ST->Lx)/w,float(ST->Ly)/h)
				,float( ST->CenterX )/ST->Lx,float( ST->CenterY )/ST->Ly,ST->TextureID,ModDWORD(GPS.GetCurrentDiffuse(),Color),ST->Additive);
			return true;
	}
} // DrawSelPatch

bool DrawSelPatchDir(float x,float y,byte Dir,int Type,float ScaleX,float ScaleY,DWORD Color){
	if(Type<SelTypes.Selections.GetAmount()){
		OneSelectionType* ST=SelTypes.Selections[Type];
		if(ST->DontRotateWithUnit)Dir=0;
		float r=float(Dir)*360/256;
		if(ST->RotationSpeed>0.0001){
			static int T=GetTickCount();
			int DT=float(GetTickCount()-T)/10;
			r+=DT*ST->RotationSpeed;
		}
		float w=IRS->GetTextureWidth(ST->TextureID);
		float h=IRS->GetTextureHeight(ST->TextureID);
			ITerra->DrawPatch(x,y,ST->Lx*ScaleX*ST->StandartScale,ST->Ly*ScaleY*ST->StandartScale,r,Rct(float(ST->x)/w,float(ST->y)/h,float(ST->Lx)/w,float(ST->Ly)/h)
				,float( ST->CenterX )/ST->Lx,float( ST->CenterY )/ST->Ly,ST->TextureID,ModDWORD(GPS.GetCurrentDiffuse(),Color),false);
			return true;
	}
} // DrawSelPatchDir

bool DrawSelPatch(float x,float y,int Type,float Radius,DWORD Color){
	if(Type>=0&&Type<SelTypes.Selections.GetAmount()){
		OneSelectionType* ST=SelTypes.Selections[Type];
		if(ST->Lx&&ST->Ly){
			float ScaleX=2*Radius/ST->Lx;
			float ScaleY=2*Radius/ST->Ly;
			DrawSelPatch( x,y,Type,ScaleX,ScaleY,Color );
			return true;
		}
	}
	return false;
}
struct SelPatchInfo{
	float x;
	float y;
	int Type;
	float Radius;
	DWORD Color;
};
DynArray<SelPatchInfo> SelPatchQueue;
void add_SelPatch(float x,float y,int Type,float Radius,DWORD Color){
	SelPatchInfo sp;
	sp.x=x;
	sp.y=y;
	sp.Color=Color;
	sp.Radius=Radius;
	sp.Type=Type;
	SelPatchQueue.Add(sp);
}
void clear_SelPatches(){
	SelPatchQueue.Clear();
}
void draw_SelPatches(){
	for(int i=0;i<SelPatchQueue.GetAmount();i++){
		SelPatchInfo& sp=SelPatchQueue[i];
		DrawSelPatch(sp.x,sp.y,sp.Type,sp.Radius,sp.Color);
	}
}