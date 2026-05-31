#include "stdheader.h"
int CreateRZone(int x,int y,int lx,int ly,HandlePro* HPro,HandlePro* RHPro,int Index,char* Hint,char* HintLo);
int CreateZone(int x,int y,int lx,int ly,HandlePro* HPro,int Index,char* Hint,char* HintLo);
extern int IconLx;
extern int IconLy;
extern int AddIconLx;
extern int AddIconLy;
extern byte GraySet[256];
void CBar(int x,int y,int Lx,int Ly,byte c);
extern byte KeyCodes[512][2];
#define NKEYS 61
extern char* KeyNames[NKEYS];
extern byte ScanKeys[NKEYS];
void OneIcon::InitIcon(){
	if(!this)return;
    FileID=0xFFFF;
    SpriteID=0xFFFF;
    Type=0;
	Disabled=false;
    Hint=NULL;
	HintLo=NULL;
    RPro=NULL;
    LPro=NULL;
	MoveOver=NULL;
	Level=0;
	Selected=false;
	MoreSprite=0xFFFF;
};
void OneIcon::AssignIcon(word FID,word SID){
	if(!this)return;
    Type=0;
	Disabled=false;
    RPro=NULL;
    LPro=NULL;
	MoveOver=NULL;
	Level=0;

    FileID=FID;
    SpriteID=SID;
	Selected=false;
	MoreSprite=0xFFFF;
};
void OneIcon::AssignHint(char* str){
	return;
	if(!this)return;
    Type|=16;
    Hint=str;
};
void OneIcon::SelectIcon(){
	if(!this)return;
	Selected=true;
};
extern bool EditMapMode;
void OneIcon::CreateHint(char* str){
	if(!this)return;
    Type|=16+128;
	if(Hint){
		//free(Hint);
		//NULL;
	};
	if(!Hint)Hint=znew(char,512);
    strcpy(Hint,str);
	if(HintLo){
		free(HintLo);
		HintLo=NULL;
	};
	if((!EditMapMode)&&SpriteID!=-1&&KeyCodes[SpriteID][0]){
		strcat(Hint," (");
		byte Code=KeyCodes[SpriteID][0];
		byte State=KeyCodes[SpriteID][1];
		if(State&1)strcat(Hint+strlen(Hint),"CTRL+");
		if(State&2)strcat(Hint+strlen(Hint),"ALT+");
		if(State&4)strcat(Hint+strlen(Hint),"SHIFT+");
		strcat(Hint+strlen(Hint),KeyNames[Code]);
		strcat(Hint,")");
	};
};
void OneIcon::CreateHintLo(char* str){
	if(!this)return;
    Type|=16+128;
	if(HintLo){
		//free(HintLo);
		//HintLo=NULL;
	};
	if(!HintLo)HintLo=znew(char,512);
    strcpy(HintLo,str);

};
void OneIcon::AssignColor(byte c){
	if(!this)return;
    Type|=1;
    Color=c;
};
void OneIcon::AssignHealth(int H,int MH){
	if(!this)return;
    Type|=2;
    Health=H;
    MaxHealth=MH;
};
void OneIcon::AssignStage(int S,int MS){
	if(!this)return;
    Type|=4;
    Stage=S;
    MaxStage=MS;
};
void OneIcon::AssignLeft(HandlePro* Lpro,int param){
	if(!this)return;
    Type|=32;
    LPro=Lpro;
    LParam=param;
};
void OneIcon::AssignMoveOver(HandlePro* pro,int param){
	if(!this)return;
    MoveOver=pro;
    MoveParam=param;
};
void OneIcon::AssignRight(HandlePro* Rpro,int param){
	if(!this)return;
    Type|=64;
    RPro=Rpro;
    RParam=param;
};
void OneIcon::AssignIntVal(int ival){
	if(!this)return;
    if(ival<1)return;
    Type|=8;
    IntVal=ival;
};
void OneIcon::AssignIntParam(int i){
	if(!this)return;
	Type|=256;
	IntParam=i;
};
void OneIcon::AssignRedPulse(){
	if(!this)return;
	Type|=512;
};
void OneIcon::Disable(){
	if(!this)return;
	Disabled=true;
	Type&=~(32|64);
	LPro=NULL;
	RPro=NULL;
	MoveOver=NULL;
};
void OneIcon::AssignLevel(byte L){
	if(!this)return;
	Level=L;
};
extern byte graysc[256];
extern bool realLpressed;
bool CheckSpritePressed(int sp);
void xLine(int x,int y,int x1,int y1,byte c);
void DrawInf(int x0,int y0,byte c){
	xLine(x0+2,y0,x0+4,y0,c);
	xLine(x0+4,y0,x0+10,y0+6,c);
	xLine(x0+10,y0+6,x0+12,y0+6,c);
	xLine(x0+12,y0+6,x0+14,y0+4,c);
	xLine(x0+14,y0+4,x0+14,y0+2,c);
	xLine(x0+14,y0+2,x0+12,y0,c);
	xLine(x0+12,y0,x0+10,y0,c);
	xLine(x0+10,y0,x0+4,y0+6,c);
	xLine(x0+4,y0+6,x0+2,y0+6,c);
	xLine(x0+2,y0+6,x0,y0+4,c);
	xLine(x0,y0+4,x0,y0+2,c);
	xLine(x0,y0+2,x0+2,y0,c);
};
byte GetPaletteColor(int r,int g,int b);
byte CYELLOW=0;
void OneIcon::Draw(int x,int y){
	
};
IconSet::IconSet(){
    NIcons=0;
    Space=256;
	Page=0;
    Icons=new OneIcon[Space];
	for(int i=0;i<Space;i++){
		Icons[i].Visible=false;
		Icons[i].Hint=NULL;
		Icons[i].HintLo=NULL;
	};
};
IconSet::~IconSet(){
    ClearIconSet();
    if(Space)delete(Icons);
    NIcons=0;
    Space=0;
};
void IconSet::InitIconSet(){
    NIcons=0;
};
void IconSet::ClearIconSet(){
    for(int i=0;i<NIcons;i++){
        OneIcon* OI=Icons+i;
		if(OI->Hint){
			//free(OI->Hint);
			//OI->Hint=NULL;
		};
		if(OI->HintLo){
			//free(OI->HintLo);
			//OI->HintLo=NULL;
		};
		OI->Visible=false;
    };
    NIcons=0;
	Page=0;
};
OneIcon* IconSet::AddIcon(word FileID,word SpriteID){
	int Index=-1;
	for(int i=0;i<NIcons;i++){
		if(Index==-1&&!Icons[i].Visible){
			Index=i;
		};
	};
	if(Index==-1){
		if(NIcons>=Space){
			OneIcon* Ico=new OneIcon[NIcons+8];
			if(Space>0){
				memcpy(Ico,Icons,NIcons*sizeof OneIcon);
				free(Icons);
			};
			Space=NIcons+8;
			Icons=Ico;
		};
		Index=NIcons;
		NIcons++;
	};
    OneIcon* OI=Icons+Index;
    OI->AssignIcon(FileID,SpriteID);
	OI->Visible=true;
    return OI;
};
OneIcon DEFICON;
extern bool Page3HaveItems;
extern bool Page2HaveItems;
extern bool Page1HaveItems;
OneIcon* IconSet::AddIconFixed(word FileID,word SpriteID,int Index){
	if(Index==-1)return AddIcon(FileID,SpriteID);
	int ix=Index%12;
	int iy=Index/12;
	OneIcon* OI;
	if(ix!=0){
		switch(iy/3){
		case 0:
			Page1HaveItems=1;
			break;
		case 1:
			Page2HaveItems=1;
			break;
		case 2:
			Page3HaveItems=1;
			break;
		};
	};
	if(iy/3==Page){
		OI=Icons+Index-Page*36;
		if(Index>=NIcons)NIcons=Index+1;
		OI->AssignIcon(FileID,SpriteID);
		OI->Visible=true;
	}else{
		OI=NULL;
	};
    return OI;
};
OneIcon* IconSet::AddIcon(word FileID,word SpriteID,char* Hint){
    OneIcon* OI=AddIcon(FileID,SpriteID);
    OI->CreateHint(Hint);
    return OI;
};
OneIcon* IconSet::AddIcon(word FileID,word SpriteID,char* Hint,byte Color){
    OneIcon* OI=AddIcon(FileID,SpriteID,Hint);
    OI->AssignColor(Color);
    return OI;
};
OneIcon* IconSet::AddIcon(word FileID,word SpriteID,byte Color,char* Hint){    
    return NULL;
};
void AssignMovePro(int i,HandlePro* HPro,int id);
void AssignKeys(int i,byte Scan,byte State);
void IconSet::DrawIconSet(int x,int y,int Nx,int Ny,int NyStart){
	
};
int IconSet::GetMaxX(int Lx){
	int Max=-1;
	for(int i=0;i<NIcons;i++)if(Icons[i].Visible){
		int x=i%Lx;
		if(x>Max)Max=x;
	};
	return Max+1;
};