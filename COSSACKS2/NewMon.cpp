#include "stdheader.h"
#include "unitability.h"
#include "GameInterface.h"
#include "CurrentMapOptions.h"
#include ".\cvi_InGamePanel.h"
#include "unichash.h"
#include "QuadsRender.h"
#include "MapSprites.h"
//
int GetUnitModifiedBySprite(int _MineID,OneSprite* OS);
void UnLockComplexObject(OneObject* OB);
void LockComplexObject(OneObject* OB);
bool FindBestPositionOLD(OneObject* OB,int* xd,int* yd,int R0);

void KeyTestMem();
void ErrMS(char* s);
bool CheckShooterAbilityToRecharge(OneObject* OBJ);
bool CheckIfPossibleToBreakOrder(OneObject* OB);
extern byte CaptState;
extern bool NOPAUSE;
extern ReportFn* AttackLink;
void CheckArmies(City*);
void DestructBuilding(OneObject* OB);
int GetTopology(int x,int y, byte LockType=0);
int GetTopology(int* x,int* y, byte LockType=0);
bool CheckTopDirectWay(int x0,int y0,int x1,int y1,byte Type);
word CheckMotionThroughEnemyAbility(OneObject* OB,int px,int py);
bool CheckBuildPossibility(byte NI,int x,int y);
extern bool FastMode;
void GotoFinePosition(OneObject* OB);
extern bool AllowPathDelay;
extern byte   WeaponFlags[32];
NewAnimation* Shar;
NewAnimation* SharBuild;
void CheckUnits();
int SearchStr(char** Res,char* s,int count);
int SearchStrBack(char** Res,char* s,int count);
NewAnimation* GetNewAnimationByName(char* Name);
int GetResID(char* gy);
int GetResByName(char* gy);
int GetWeaponType(char* Name);
extern bool PeaceMode;
extern byte LockMode;
extern bool TransMode;
extern char* SoundID[MaxSnd];
extern word NSounds;
int CheckPt(int x,int y);
void LoadIcons();
int Prop43(int y);
extern byte* NPresence;
extern bool Mode3D;
extern short TSin[257];
extern short TCos[257];
extern short TAtg[257];
int PortBuiX,PortBuiY;
void NLine(GFILE* f);
int GetUnitKind(char* Name);
void ShowRLCItemPal(int x,int y,lpRLCTable lprt,int n,byte* Pal);
void ShowRLCItemGrad(int x,int y,lpRLCTable lprt,int n,byte* Pal);
void normstr(char* str);
void InitFlags(GeneralObject* GO);
int mul3(int);
void DrawMarker(OneObject* OB);
void DrawMiniMarker(OneObject* OB);
void ClearMaps();
int GetWallType(char* Name);
short LastDirection=512;
void CheckCapture(OneObject* OBJ);
//01-Order
//02-Attack
//04-select
void AddUnlimitedEffect(int x,int y,int id);

int NTimesClick=0;
OneObject* LastObj=NULL;
int mosLastClickTime=0;

void MakeOrderSound(OneObject* OB,byte SMask){
	if(SMask==4&&OB->NNUM!=MyNation)return;
	Nation* NT=OB->Nat;
	if(NT->SoundMask[OB->NIndex]&SMask)return;
	NT->SoundMask[OB->NIndex]|=SMask;
	NewMonster* NM=OB->newMons;
	int SID=-1;
	bool Longrange=false;
	switch(SMask){
	case 1:
		SID=NM->AttackSoundID;
		break;
	case 2:
		{
			int T=GetTickCount();
			if(T-mosLastClickTime>300 || LastObj!=OB){
				LastObj=NULL;
				NTimesClick=0;
			}
			NTimesClick++;
			mosLastClickTime=T;
			LastObj=OB;
			if(NTimesClick>4){
				SID=NM->FastClickSoundID;
			}else{
				SID=NM->OrderSoundID;
			}
		}
		break;
	case 4:
		SID=NM->SelectSoundID;
		break;
	case 8:
		SID=NM->ReadySoundID;
		break;
	case 16:
		Longrange=true;
		SID=NM->UpgradeSoundID;
		break;
	};
	if(SID==-1)return;
	extern int LastMx;
	extern int LastMy;
	if(Longrange)AddSingleEffect(LastMx,LastMy,SID);
	else AddSingleEffect(OB->RealX>>4,OB->RealY>>4,SID);
};
void ConvertToUp(char* str){
	while(str[0]){
		if(str[0]>='a'&&str[0]<='z'){
			str[0]-='a'-'A';
		};
		str++;
	};
};
int GetRAngle(int dx,int dy,int Angle){
	int ang;
	int sx=abs(dx);
	int sy=abs(dy);
	if(!(dx||dy))return 0;
	if(sx>sy){
		ang=div(dy<<8,sx).quot;
		if(dx<0)ang=1024-ang;
	}else{
		ang=512-div(dx<<8,sy).quot;
		if(dy<0)ang=2048-ang;
	};
	ang=Angle-ang;
	while(ang<0)ang+=2048;
	if(ang>1792)ang-=2048;
	return ang;
};
int GetLAngle(int dx,int dy,int Angle){
	int ang;
	int sx=abs(dx);
	int sy=abs(dy);
	if(!(dx||dy))return 0;
	if(sx>sy){
		ang=div(dy<<8,sx).quot;
		if(dx<0)ang=1024-ang;
	}else{
		ang=512-div(dx<<8,sy).quot;
		if(dy<0)ang=2048-ang;
	};
	ang-=Angle;
	while(ang<0)ang+=2048;
	if(ang>1792)ang-=2048;
	return ang;
};
bool PInside(int x,int y,int x1,int y1,int xp,int yp){
	if(xp>=x&&xp<=x1&&yp>=y&&yp<=y1)return true;
	else return false;
};
bool InClOrders=0;
void OneObject::ClearOrders(){
	InClOrders=1;
	//if(int(InLineCom))FreeAsmLink();
	while(LocalOrder)DeleteLastOrder();		
	InClOrders=0;
	Attack=0;
    PrioryLevel=0;
	EnemyID=0xFFFF;
}
void InitAllGame(){
	ClearMaps();
	memset(Group,0,sizeof Group);
	memset(Sprites,0,sizeof Sprites);
	memset(NSL,0,sizeof NSL);
};
void DosToWin(char* Str){
	int len=strlen(Str);
	for(int i=0;i<len;i++){
		byte c=Str[i];
		if(c>=0x80&&c<0xB0)c+=0x40;
		else if(c>=0xE0&&c<0xF0)c+=0x10;
		Str[i]=c;
	};
};
extern int CurPalette;
extern bool PalDone;
void ErrM(char* s)
{
	//if(PalDone){
	//	char pal[128];
	//	sprintf(pal,"%d\\agew_1.pal",CurPalette);
	//	LoadPalette(pal);
	//};
	//MessageBox(hwnd,s,"LOADING FAILED...",MB_ICONWARNING|MB_OK);
	//assert(false);
	Log.Error(s);
};
void pErr(char* sz,...)
{
	char ach[256];
	va_list va;

	va_start( va, sz );
	vsprintf ( ach, sz, va );   
	va_end( va );	
	MessageBox(hwnd,ach,"LOADING FAILED...",MB_ICONWARNING|MB_OK);
	assert(false);
};
void NEPar(char* name,int line,char* Sect,int Need){
	char gx[128];
	sprintf(gx,"%s,%d : %s : Not enough parameters. Need: %d",name,line,Sect,Need);
	ErrM(gx);
};
void InvPar(char* name,int line,char* Sect,char* parm){
	char gx[128];
	sprintf(gx,"%s,%d : %s : Invalid parameter: %s",name,line,Sect,parm);
	ErrM(gx);
};
void IncPar(char* name,int line,char* Sect){
	char gx[128];
	sprintf(gx,"%s,%d : %s : Incorrect parameters",name,line,Sect);
	ErrM(gx);
};
void UpConv(char* str){
	int i=0;
	char c;
	do{
		c=str[i];
		if(c>='a'&&c<='z')c-=0x20;
		str[i]=c;
		i++;
	}while(c!=0);
};
//------------------------------------------New RLC information
//int NRLFiles;//number of files with RLC pictures
//char* RLNames[512];
//RLCTable RMImages[512];
//RLCTable miniRMImages[512];
//RLCTable SRMImages[512];
//RLCTable miniSRMImages[512];
word RLCNSpr[MaxGPIdx];
word NNewMon;
extern NewMonster* NewMon[2048];
char* MonNames[2048];
//-----------------New weapon animation discription------------
extern NewAnimation WNewAnm[768];
char* WAnmNames[768];
word NNewAnm;
//-------------------------------------------------------------
int CurrentCursorGP=0;
int AnmCursorGP=0;
extern int CBB_GPFILE;
void LoadMapInterface();
void InitNewMonstersSystem(){
	char* Name="icons";
	GPS.PreLoadGPImage(Name);
	GPS.PreLoadGPImage("smicons");
	GPS.PreLoadGPImage("flag");
	GPS.PreLoadGPImage("Interf3\\elements\\scroll");
	GPS.PreLoadGPImage("smicons1");
#ifdef _USE3D
	CurrentCursorGP = 0;
#endif //_USE3D
	GPS.PreLoadGPImage("Cursor_00");
	GPS.PreLoadGPImage("Cursor_01");
	GPS.PreLoadGPImage("Cursor_02");
	CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\combo2");
	LoadMapInterface();
	AnmCursorGP=0; //GPS.PreLoadGPImage("Interface\\CursGo");
	NNewMon=0;
	NNewAnm=0;
	LoadIcons();
	UFONTS.LoadFonts();
};
NewAnimation::NewAnimation(){
	Enabled=true;
	ActiveFrame=0xFF;
	LineInfo=NULL;
	ModelID=-1;
	AnimationID=-1;
	Scale=1.0f;
	AnimationType=1;
	Code=0;//anm_MotionL;
	CanBeBrokenAfterFrame=1024;
};
NewMonster::NewMonster(){
	//memset(&MotionL,0,sizeof NewMonster);
};
//--------------------Icons------------------
void ReadKeyCodes();
char* IconNames[4096];
byte KeyCodes[4096][2];
word NIcons;
void LoadMapInterface();
void LoadIcons(){
	NIcons=0;
	GFILE* f=Gopen("IconList.txt","r");
	AFile("IconList.txt");
	char gx[128];
	int z;
	if(f){
		do{
			z=Gscanf(f,"%s",gx);
			if(!strcmp(gx,"[END]"))return;
			assert(NIcons<4096);
			IconNames[NIcons]=znew(char,strlen(gx)+1);
			ConvertToUp(gx);
			strcpy(IconNames[NIcons],gx);
			NIcons++;
		}while(z==1);
	}else{
		ErrM("Unable to open IconList.txt");
	};
	ReadKeyCodes();
};
void FreeIcons(){
	for(int i=0;i<NIcons;i++)free(IconNames[i]);
};
int GetIconByName(char* Name){
	ConvertToUp(Name);
	for(int i=0;i<NIcons;i++){
		if(!strcmp(IconNames[i],Name))return i;
	};
	return -1;
};
CEXPORT
int GetIconByNameExt(char* Name){
	return GetIconByName(Name);
}
char* ReadStatement(char* Src,char* Dst){
	Dst[0]=0;
	if(Src){
		while(Src[0]==' '||Src[0]==9)Src++;
		int z=sscanf(Src,"%s",Dst);
		if(z==1){
			Src+=strlen(Dst);
			return Src;
		}else return 0;
	}else return NULL;
};
#define NKEYS 77
char* KeyNames[NKEYS]={"0","1","2","3","4","5","6","7","8","9",
	"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
	"ENTER","ESC",
	"NUM0","NUM1","NUM2","NUM3","NUM4","NUM5","NUM6","NUM7","NUM8","NUM9",
	"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","PGUP","PGDN","HOME","END","INS","NUM+","NUM-","NUM*","NUM/","<",">","?",
	"F11","F12",
	"`~","SPACE","BACK","PAUSE","DELETE"};
byte ScanKeys[NKEYS]={'0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	13,27,
	VK_NUMPAD0,VK_NUMPAD1,VK_NUMPAD2,VK_NUMPAD3,VK_NUMPAD4,VK_NUMPAD5,VK_NUMPAD6,VK_NUMPAD7,VK_NUMPAD8,VK_NUMPAD9,
	VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8,VK_F9,VK_F10,VK_NEXT,VK_PRIOR,VK_HOME,VK_END,VK_INSERT,107,109,106,111,188,190,191,
	VK_F11,VK_F12,
	VK_OEM_3,VK_SPACE,VK_BACK,VK_PAUSE,VK_DELETE};
//
byte GetKeyByName(char* Name){
	for(int i=0;i<NKEYS;i++){
		if(strcmp(KeyNames[i],Name)==0){
			return ScanKeys[i];
		}
	}
	return 0;
}
char* GetKeyName(word Key){
	for(int i=0;i<NKEYS;i++){
		if(ScanKeys[i]==Key){
			return KeyNames[i];
		}
	}
	return "";
}
void CreateKeysEnumerator(Enumerator* E){
	if(E){
		E->Clear();
		E->Add("NONE",DWORD(0));
		for(int i=0;i<NKEYS;i++) E->Add(KeyNames[i],ScanKeys[i]);
	}
}

bool ReadWinString(GFILE* F,char* STR,int Max);
void ReadKeyCodes(){
	memset(&KeyCodes[0][0],0,sizeof KeyCodes);
	GFILE* F=Gopen("Data\\Keyboard.dat","r");
	if(F){
		char ccc[257];
		bool Done;
		do{
			Done=ReadWinString(F,ccc,256);
			if(Done&&ccc[0]!='/'){
				_strupr(ccc);
				char DST[256];
				char* Next;
				byte ScanCode=0;
				byte KeyState=0;
				int NCODES=0;
				bool IconMode=0;
				Next=ccc;
				do{
					Next=ReadStatement(Next,DST);
					if(Next){
						if(IconMode){
							int id=GetIconByName(DST);
							if(id==-1){
								char err[128];
								sprintf(err,"Unknown icon ID: %s",DST);
								ErrM(err);
							}else{
								KeyCodes[id][0]=ScanCode;
								KeyCodes[id][1]=KeyState;
							};
							}else{
							if(DST[0]==':'){
								IconMode=1;
							}else
							if(!strcmp(DST,"CTRL"))KeyState|=1;
							else if(!strcmp(DST,"ALT"))KeyState|=2;
							else if(!strcmp(DST,"SHIFT"))KeyState|=4;
							else{
								ScanCode=255;
								for(int j=0;j<NKEYS;j++)if(!strcmp(DST,KeyNames[j]))ScanCode=j;
								if(ScanCode==255){
									char err[128];
									sprintf(err,"Unknown key code: '%s'",DST);
									ErrM(err);
									ScanCode=0;
								};	
							};
						};
					};	
				}while(Next);
			};
		}while(Done);
	}else{
		//ErrM("Unable to open Data\\Keyboard.dat");
	};
};
void CreateRoundLockpointsAndBuildpoints(NewMonster* NM,int R){
	if(NM->LockX)delete[]NM->LockX;
	if(NM->LockY)delete[]NM->LockY;
	if(NM->CheckX)delete[]NM->CheckX;
	if(NM->CheckY)delete[]NM->CheckY;
	if(NM->BLockX)delete[]NM->BLockX;
	if(NM->BLockY)delete[]NM->BLockY;
	int np=0;
	for(int i=0;i<R;i++){
        np+=Rarr[i].N;
	}
	NM->LockX=znew(char,np);
	NM->LockY=znew(char,np);
	NM->CheckX=znew(char,np);
	NM->CheckY=znew(char,np);	
	NM->BLockX=znew(char,np);
	NM->BLockY=znew(char,np);
	NM->NLockPt=NM->NBLockPt=NM->NCheckPt=np;
	np=0;
	for(int i=0;i<R;i++){
		int N=Rarr[i].N;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		for(int j=0;j<N;j++){
            NM->LockX[np]=NM->CheckX[np]=NM->BLockX[np]=xi[j];
			NM->LockY[np]=NM->CheckY[np]=NM->BLockY[np]=yi[j];
			np++;
		}
	}
	R+=3;
	int Np=2*R;
	for(int i=0;i<Np;i++){
        int x=R*TCos[i*255/Np]/256;
		int y=R*TSin[i*255/Np]/256;
		NM->BuildPtX.Add(x);
		NM->BuildPtY.Add(y);
	}
}
//-----------Loading additional New animations----------
extern NewAnimation* Shar;
void FreeWNames(){
	for(int i=0;i<NNewAnm;i++)free(WAnmNames[i]);
};
void LoadNewAimations(){
	GFILE* f1=Gopen("weapon.ads","r");
	AFile("weapon.ads");
	char Fn[128];
	char gx[128];
	char gy[128];
	char gz[128];
	int MaxRLC=-1;
	word RLCRef[128];
	memset(&RLCRef,0,sizeof RLCRef);
	short RLCdx[128];
	short RLCdy[128];
	int z1,z2,z3,p1,p2,p3;
	bool finish=false;
	int Line=1;
	int z=0;
	if(f1){
		do{
			z=Gscanf(f1,"%s",gx);
			if(z!=1)IncPar("weapon.ads",Line,"?");
			if(gx[0]!='/'&&gx[0]!='['){
                if(gx[0]=='@'){
					//Animation;
                    gx[0]='#';
                    WAnmNames[NNewAnm]=znew(char,strlen(gx)+1);
					strcpy(WAnmNames[NNewAnm],gx);
					NewAnimation* NANM=&WNewAnm[NNewAnm];
                    int z4;
					if(NANM){
						z=Gscanf(f1,"%d%d%d%d",&z1,&z2,&z3,&z4);
						if(z!=4)NEPar("weapon.ads",Line,gx,4);
						//NANM->StartDx=NAStartDx;
						//NANM->StartDy=NAStartDy;
						//NANM->Parts=NAParts;
						//NANM->PartSize=NAPartSize;
						NANM->Name=znew(char,strlen(gx)+1);
						strcpy(NANM->Name,gx);
						NANM->Enabled=true;
						NANM->Rotations=z1;
						if(z1==255)z1=1;
						NANM->SoundID=-1;
						NANM->ActiveFrame=0;
						//NANM->ActivePtX=znew(short,z1+z1+z1);
						//NANM->ActivePtY=znew(short,z1+z1+z1);
						OneActivePt AP;
						NANM->ActivePt.Add(AP,z1+z1+z1);
						NANM->LineInfo=NULL;
						for(int i=0;i<z1+z1+z1;i++){
							NANM->ActivePt[i]->x=0;
							NANM->ActivePt[i]->y=0;
						};
						int dz,nz;
						p1=RLCRef[z2];
						if(p1==0){
							Log.Error( "Weapon.ads,line %d : %s : Undefined index used: %d", Line, gx, z2 );
						}
						if(z3>=z4){
							if(z3*z1>=GPS.GPNFrames(p1)){
								Log.Error( "Weapon.ads,line %d : Reference to RLC sprite is beyond the end of RLC file.", Line );
							};
							dz=-1;
							nz=z3-z4+1;
						}else{
							if(z4*z1>=GPS.GPNFrames(p1)){
								Log.Error( "Weapon.ads,line %d : Reference to RLC sprite is beyond the end of RLC file.", Line );
							};
							dz=1;
							nz=z4-z3+1; 


						};
						NANM->NFrames=nz;
						//NANM->Frames=new NewFrame[nz];
						NANM->Frames.Clear();
						//for(i=0;i<nz;i++)NANM->Frames.Add(new NewFrame());
						NANM->AllocFrames(nz);
						for(int i=0;i<nz;i++){
							NewFrame* NF=NANM->Frames[i];
							NF->FileID=p1;
							NF->SpriteID=z3;
							z3+=dz;
							NF->dx=RLCdx[z2];
							NF->dy=RLCdy[z2];
						};
						assert(NNewAnm<768);
                        NNewAnm++;
						NLine(f1);
						Line++;
					}else{
						gx[0]='@';
						Log.Error( "Weapon.ads,line %d : Unknown animation type: %s", Line, gx );
					};
                }else
				if(gx[0]=='!'){
					gx[0]='#';
					WAnmNames[NNewAnm]=znew(char,strlen(gx)+1);
					strcpy(WAnmNames[NNewAnm],gx);
					NewAnimation* NANM=&WNewAnm[NNewAnm];
					NNewAnm++;
					if(NANM){
						NANM->Enabled=true;
						int nf=0;
						float scal=1.0f;
						char mesh[256];
						char anim[256];
						z=Gscanf(f1,"%d%f%s%s",&nf,&scal,mesh,anim);
						if(z==4){
							NANM->ModelID=IMM->GetModelID(mesh);
							NANM->AnimationType=true;
							NANM->Scale=scal;
							NANM->NFrames=abs(nf);
							NANM->Inverse=nf<0;
							if(NANM->ModelID==-1){
								sprintf(gy,"weapon.ads ,line %d : Unable to load model [%s] for animation: %s",Line,mesh,gx);
								ErrM(gy);
							}
							if(strcmp(anim,"none")){
								NANM->AnimationID=IMM->GetModelID(anim);							
								if(NANM->AnimationID==-1){
									sprintf(gy,"weapon.ads, line %d : Unable to load animation [%s] for animation: %s",Line,anim,gx);
									ErrM(gy);
								}
							}else NANM->AnimationID=-1;
						}else{
							sprintf(gy,"weapon.ads, line %d : Bad parameters for animation: %s",Line,gx);
							ErrM(gy);
						}
						NLine(f1);
						Line++;
					}else{
						sprintf(gy,"weapon.ads, line %d : Unknown animation type: %s",Line,gx);
						ErrM(gy);
					}
				}else
				if(gx[0]=='$'){
					//Animation;
                    gx[0]='#';
                    WAnmNames[NNewAnm]=znew(char,strlen(gx)+1);
					strcpy(WAnmNames[NNewAnm],gx);
					NewAnimation* NANM=&WNewAnm[NNewAnm];
                    int z4,dx,dy;
					if(NANM){
						z=Gscanf(f1,"%d%d%d%d%d%d",&dx,&dy,&z1,&z2,&z3,&z4);
						if(z!=6)NEPar("weapon.ads",Line,gx,6);
						//NANM->StartDx=NAStartDx;
						//NANM->StartDy=NAStartDy;
						//NANM->Parts=NAParts;
						//NANM->PartSize=NAPartSize;
						NANM->Enabled=true;
						NANM->Rotations=z1;
						NANM->ActiveFrame=0;
						NANM->SoundID=-1;
						//NANM->ActivePtX=znew(short,z1+z1);
						//NANM->ActivePtY=znew(short,z1+z1);
						OneActivePt AP;
						NANM->ActivePt.Add(AP,z1+z1);
						NANM->LineInfo=NULL;
						for(int i=0;i<z1+z1+z1;i++){
							NANM->ActivePt[i]->x=0;
							NANM->ActivePt[i]->y=0;
						};
						int dz,nz;
						p1=RLCRef[z2];
						if(z3>=z4){
							if(z3*z1>=GPS.GPNFrames(p1)){
								Log.Error( "Weapon.ads,line %d : Reference to RLC sprite is beyond the end of RLC file.", Line );
							};
							dz=-1;
							nz=z3-z4+1;
						}else{
							if(z4*z1>=GPS.GPNFrames(p1)){
								Log.Error( "Weapon.ads,line %d : Reference to RLC sprite is beyond the end of RLC file.", Line );
							};
							dz=1;
							nz=z4-z3+1;


						};
						NANM->NFrames=nz;
						//NANM->Frames=new NewFrame[nz];
						NANM->Frames.Clear();
						//for(i=0;i<nz;i++)NANM->Frames.Add(new NewFrame());
						NANM->AllocFrames(nz);
						for(int i=0;i<nz;i++){
							NewFrame* NF=NANM->Frames[i];
							NF->FileID=p1;
							NF->SpriteID=z3;
							z3+=dz;
							NF->dx=dx;
							NF->dy=dy;
						};
						assert(NNewAnm<768);
                        NNewAnm++;
						NLine(f1);
						Line++;
					}else{
						gx[0]='@';
						Log.Error( "Weapon.ads,line %d : Unknown animation type: %s", Line, gx );
					};
                }else
				if(gx[0]=='#'){
					//Animation;
					WAnmNames[NNewAnm]=znew(char,strlen(gx)+1);
					strcpy(WAnmNames[NNewAnm],gx);
					NewAnimation* NANM=&WNewAnm[NNewAnm];
					z=Gscanf(f1,"%d%d",&z1,&z2);
					if(z!=2)IncPar("weapon.ads",Line,gx);
					NANM->Enabled=true;
					NANM->NFrames=z2;
					NANM->Rotations=z1;
					NANM->ActiveFrame=0xFF;
					//NANM->ActivePtX=znew(short,z1+z1+z1);
					//NANM->ActivePtY=znew(short,z1+z1+z1);
					OneActivePt AP;
					NANM->ActivePt.Add(AP,z1+z1+z1);
					NANM->SoundID=-1;
					NANM->LineInfo=NULL;
					for(int i=0;i<z1+z1+z1;i++){
						NANM->ActivePt[i]->x=0;
						NANM->ActivePt[i]->y=0;
					};
					//NANM->Frames=new NewFrame[z2];
					NANM->Frames.Clear();
					//for(i=0;i<z2;i++)NANM->Frames.Add(new NewFrame());
					NANM->AllocFrames(z2);
					for(int i=0;i<z2;i++){
						z=Gscanf(f1,"%d%d",&z1,&z3);
						if(z!=2)IncPar("weapon.ads",Line,gx);
						if(z1>MaxRLC){
							sprintf(gy,"Weapon.ads,line %d : Incorrect RLC reference: %d",Line,z1);
							ErrM(gy);
						};
						NewFrame* NF=NANM->Frames[i];
						NF->FileID=RLCRef[z1];
						if(z3>=GPS.GPNFrames(RLCRef[z1]))
						{
							Log.Error( "Weapon.ads,line %d : Reference to sprite is beyond the end of file: %d", Line, z3 );
						};
						NF->SpriteID=z3;
						NF->dx=RLCdx[z1];
						NF->dy=RLCdy[z1];
					};
					assert(NNewAnm<768);
					NNewAnm++;
					NLine(f1);
					Line++;
				}else
				if(gx[0]=='%'){
					//Animation;
					int ddx,ddy;
					WAnmNames[NNewAnm]=znew(char,strlen(gx)+1);
					strcpy(WAnmNames[NNewAnm],gx);
					NewAnimation* NANM=&WNewAnm[NNewAnm];
					z=Gscanf(f1,"%d%d%d%d",&ddx,&ddy,&z1,&z2);
					if(z!=4)IncPar("weapon.ads",Line,gx);
					NANM->Enabled=true;
					NANM->NFrames=z2;
					NANM->Rotations=z1;
					NANM->ActiveFrame=0xFF;
					NANM->HotFrame=0;
					NANM->SoundID=-1;
					//NANM->ActivePtX=znew(short,z1+z1+z1);
					//NANM->ActivePtY=znew(short,z1+z1+z1);
					OneActivePt AP;
						NANM->ActivePt.Add(AP,z1+z1+z1);
					NANM->LineInfo=NULL;
					for(int i=0;i<z1+z1+z1;i++){
						NANM->ActivePt[i]->x=0;
						NANM->ActivePt[i]->y=0;
					};
					//NANM->Frames=new NewFrame[z2];
					NANM->Frames.Clear();
					//for(i=0;i<z2;i++)NANM->Frames.Add(new NewFrame());
					NANM->AllocFrames(z2);
					for(int i=0;i<z2;i++){
						z=Gscanf(f1,"%d%d",&z1,&z3);
						if(z!=2)IncPar("weapon.ads",Line,gx);
						if(z1>MaxRLC){
							sprintf(gy,"Weapon.ads,line %d : Incorrect RLC reference: %d",Line,z1);
							ErrM(gy);
						};
						NewFrame* NF=NANM->Frames[i];
						NF->FileID=RLCRef[z1];
						if(z3>=GPS.GPNFrames(RLCRef[z1])){
							sprintf(gy,"Weapon.ads,line %d : Reference to RLC sprite is beyond the end of file: %d",Line,z3);
							ErrM(gy);
						};
						NF->SpriteID=z3;
						NF->dx=ddx;
						NF->dy=ddy;
					};
					NNewAnm++;
					NLine(f1);
					Line++;
				}else{
					//Simple command
					if(!strcmp(gx,"USERLC")){
						z=Gscanf(f1,"%d%s%s%d%d",&p1,gy,gz,&p2,&p3);
						if(z!=5)IncPar("weapon.ads",Line,"USERLC");
						if(p1>MaxRLC)MaxRLC=p1;
						UpConv(gy);
						int nr=GPS.PreLoadGPImage(gy);
						RLCRef[p1]=nr;
						RLCdx[p1]=p2;
						RLCdy[p1]=p3;
						NLine(f1);
						Line++;
					}else
					if(!strcmp(gx,"SOUND")){
						z=Gscanf(f1,"%d%s%s",&p1,gy,Fn);
						if(z!=3)IncPar("Weapon.ads",Line,gx);
						NewAnimation* NA=GetNewAnimationByName(gy);
						if(NA){
							NA->SoundID=SearchStrBack(SoundID,Fn,NSounds);
							if(NA->SoundID==-1){
								sprintf(gz,"%s,line %d :%s: Unknown sound:%s","Weapon.ads",Line,gx,Fn);
								ErrMS(gz);
							};
							NA->HotFrame=p1;
							NA->SoundProbability=32767;
						}else{
							sprintf(gz,"%s,line %d :%s: Unknown animation:%s","Weapon.ads",Line,gx,gy);
							ErrM(gz);
						};
					}else{
						sprintf(gy,"Weapon.ads,line %d : Unknown command : %s",Line,gx);
						ErrM(gy);
					};
				};
			}else{
				if(!strcmp(gx,"[END]"))finish=true;
				NLine(f1);
				Line++;
			};
		}while((!finish)&&z!=-1);
		Gclose(f1);
		Shar=GetNewAnimationByName("#SHAR");
		if(!Shar){
			ErrMS("Could not find special animation:#SHAR");
		};
		SharBuild=GetNewAnimationByName("#SHARBUILD");
		if(!SharBuild){
			ErrMS("Could not find special animation:#SHARBUILD");
		};
		return;
	}else{
		if(!f1)ErrM("Could not open Weapon.ads");
		return;
	};

};
NewAnimation* GetNewAnimationByName(char* Name){
	for(int i=0;i<NNewAnm;i++)
		if(!strcmp(WAnmNames[i],Name))return &WNewAnm[i];
	return NULL;
};
//-----------Loading new monsters discriptions----------
NewAnimation* NewMonster::CreateAnimation(char* Name){
	static Enumerator* E=ENUM.Get("MON_ANIMATIONS");
	DWORD V=E->Get(Name+1);
	if(V==0xFFFFFFFF)return NULL;
	NewAnimation* NA=new NewAnimation;
	NA->Code=V;
	Animations->Add(NA,V);
	NA->Enabled=1;
	NA->AnimationType=0;
	return NA;
}
NewAnimation* NewMonster::LoadNewAnimationByName(char* gx,int Index){
	static Enumerator* E=ENUM.Get("MON_ANIMATIONS");
	DWORD V=E->Get(gx+1);
	if(V==0xFFFFFFFF)return NULL;
	NewAnimation* NA=NULL;
	if(Index==-1){
        NewAnimation* NA=Animations->Get(V,0);        
        return NA;
	}else{
		NA=Animations->Get(V,Index);	
	}
	return NA;
	/*
	NewAnimation* NANM=NULL;
	if(!strcmp(gx,"#MOTION_L"))NANM=&MotionL;else
	if(!strcmp(gx,"#MOTION_R"))NANM=&MotionR;else
	if(!strcmp(gx,"#MOTION_LB"))NANM=&MotionLB;else
	if(!strcmp(gx,"#MOTION_RB"))NANM=&MotionRB;else
	if(!strcmp(gx,"#MINISTEP_L"))NANM=&MiniStepL;else
	if(!strcmp(gx,"#MINISTEP_R"))NANM=&MiniStepL;else
	if(!strcmp(gx,"#MINISTEP_LB"))NANM=&MiniStepL;else
	if(!strcmp(gx,"#MINISTEP_RB"))NANM=&MiniStepL;else
	if(!strcmp(gx,"#ATTACK"))NANM=&Attack[0];else
	if(!strcmp(gx,"#PATTACK"))NANM=&PAttack[0];else
	if(!strcmp(gx,"#UATTACK"))NANM=&UAttack[0];else
	if(!strcmp(gx,"#DEATH"))NANM=&Death;else
	if(!strcmp(gx,"#DEATHLIE1"))NANM=&DeathLie1;else
	if(!strcmp(gx,"#DEATHLIE2"))NANM=&DeathLie2;else
	if(!strcmp(gx,"#DEATHLIE3"))NANM=&DeathLie3;else
	if(!strcmp(gx,"#STAND"))NANM=&Stand;else
	if(!strcmp(gx,"#PSTAND"))NANM=&PStand[0];else
	if(!strcmp(gx,"#WORK"))NANM=&Work;else
	if(!strcmp(gx,"#FIST"))NANM=&Fist;else
	if(!strcmp(gx,"#STANDHI"))NANM=&StandHi;else
	if(!strcmp(gx,"#STANDLO"))NANM=&StandLo;else
	if(!strcmp(gx,"#BUILDHI"))NANM=&BuildHi;else
	if(!strcmp(gx,"#BUILD"))NANM=&Build;else
	if(!strcmp(gx,"#DAMAGE"))NANM=&Damage;else
	if(!strcmp(gx,"#TRANS01"))NANM=&Trans01;else
	if(!strcmp(gx,"#TRANS10"))NANM=&Trans10;else
	if(!strcmp(gx,"#ATTACK1"))NANM=&Attack[1];else
	if(!strcmp(gx,"#PSTAND1"))NANM=&PStand[1];else
	if(!strcmp(gx,"#PATTACK0"))NANM=&PAttack[0];else
	if(!strcmp(gx,"#UATTACK0"))NANM=&UAttack[0];else
	if(!strcmp(gx,"#ATTACK0"))NANM=&Attack[0];else
	if(!strcmp(gx,"#PSTAND0"))NANM=&PStand[0];else
	if(!strcmp(gx,"#MOTION_L0"))NANM=&PMotionL[0];else
	if(!strcmp(gx,"#MOTION_R0"))NANM=&PMotionR[0];else
	if(!strcmp(gx,"#MOTION_LB0"))NANM=&PMotionLB[0];else
	if(!strcmp(gx,"#MOTION_RB0"))NANM=&PMotionRB[0];else
	if(!strcmp(gx,"#MINISTEP_L0"))NANM=&PMiniStepL[0];else
	if(!strcmp(gx,"#MINISTEP_R0"))NANM=&PMiniStepL[0];else
	if(!strcmp(gx,"#MINISTEP_LB0"))NANM=&PMiniStepL[0];else
	if(!strcmp(gx,"#MINISTEP_RB0"))NANM=&PMiniStepL[0];else
	if(!strcmp(gx,"#PATTACK1"))NANM=&PAttack[1];else
	if(!strcmp(gx,"#UATTACK1"))NANM=&UAttack[1];else
	if(!strcmp(gx,"#ATTACK1"))NANM=&Attack[1];else
	if(!strcmp(gx,"#PSTAND1"))NANM=&PStand[1];else
	if(!strcmp(gx,"#MOTION_L1"))NANM=&PMotionL[1];else
	if(!strcmp(gx,"#MOTION_R1"))NANM=&PMotionR[1];else
	if(!strcmp(gx,"#MOTION_LB1"))NANM=&PMotionLB[1];else
	if(!strcmp(gx,"#MOTION_RB1"))NANM=&PMotionRB[1];else
	if(!strcmp(gx,"#MINISTEP_L1"))NANM=&PMiniStepL[1];else
	if(!strcmp(gx,"#MINISTEP_R1"))NANM=&PMiniStepL[1];else
	if(!strcmp(gx,"#MINISTEP_LB1"))NANM=&PMiniStepL[1];else
	if(!strcmp(gx,"#MINISTEP_RB1"))NANM=&PMiniStepL[1];else
	if(!strcmp(gx,"#PATTACK2"))NANM=&PAttack[2];else
	if(!strcmp(gx,"#UATTACK2"))NANM=&UAttack[2];else
	if(!strcmp(gx,"#ATTACK2"))NANM=&Attack[2];else
	if(!strcmp(gx,"#PSTAND2"))NANM=&PStand[2];else
	if(!strcmp(gx,"#MOTION_L2"))NANM=&PMotionL[2];else
	if(!strcmp(gx,"#MOTION_R2"))NANM=&PMotionR[2];else
	if(!strcmp(gx,"#MOTION_LB2"))NANM=&PMotionLB[2];else
	if(!strcmp(gx,"#MOTION_RB2"))NANM=&PMotionRB[2];else
	if(!strcmp(gx,"#MINISTEP_L2"))NANM=&PMiniStepL[2];else
	if(!strcmp(gx,"#MINISTEP_R2"))NANM=&PMiniStepL[2];else
	if(!strcmp(gx,"#MINISTEP_LB2"))NANM=&PMiniStepL[2];else
	if(!strcmp(gx,"#MINISTEP_RB2"))NANM=&PMiniStepL[2];else
	if(!strcmp(gx,"#PATTACK3"))NANM=&PAttack[3];else
	if(!strcmp(gx,"#UATTACK3"))NANM=&UAttack[3];else
	if(!strcmp(gx,"#ATTACK3"))NANM=&Attack[3];else
	if(!strcmp(gx,"#PSTAND3"))NANM=&PStand[3];else
	if(!strcmp(gx,"#MOTION_L3"))NANM=&PMotionL[3];else
	if(!strcmp(gx,"#MOTION_R3"))NANM=&PMotionR[3];else
	if(!strcmp(gx,"#MOTION_LB3"))NANM=&PMotionLB[3];else
	if(!strcmp(gx,"#MOTION_RB3"))NANM=&PMotionRB[3];else
	if(!strcmp(gx,"#MINISTEP_L3"))NANM=&PMiniStepL[1];else
	if(!strcmp(gx,"#MINISTEP_R3"))NANM=&PMiniStepL[1];else
	if(!strcmp(gx,"#MINISTEP_LB3"))NANM=&PMiniStepL[1];else
	if(!strcmp(gx,"#MINISTEP_RB3"))NANM=&PMiniStepL[1];else
#ifdef CONQUEST
	if(!strcmp(gx,"#PATTACK4"))NANM=&PAttack[4];else
	if(!strcmp(gx,"#UATTACK4"))NANM=&UAttack[4];else
	if(!strcmp(gx,"#ATTACK4"))NANM=&Attack[4];else
	if(!strcmp(gx,"#PSTAND4"))NANM=&PStand[4];else
	if(!strcmp(gx,"#MOTION_L4"))NANM=&PMotionL[4];else
	if(!strcmp(gx,"#MOTION_R4"))NANM=&PMotionR[4];else
	if(!strcmp(gx,"#MOTION_LB4"))NANM=&PMotionLB[4];else
	if(!strcmp(gx,"#MOTION_RB4"))NANM=&PMotionRB[4];else
	if(!strcmp(gx,"#MINISTEP_L4"))NANM=&PMiniStepL[4];else
	if(!strcmp(gx,"#MINISTEP_R4"))NANM=&PMiniStepL[4];else
	if(!strcmp(gx,"#MINISTEP_LB4"))NANM=&PMiniStepL[4];else
	if(!strcmp(gx,"#MINISTEP_RB4"))NANM=&PMiniStepL[4];else

	if(!strcmp(gx,"#PATTACK5"))NANM=&PAttack[5];else
	if(!strcmp(gx,"#UATTACK5"))NANM=&UAttack[5];else
	if(!strcmp(gx,"#ATTACK5"))NANM=&Attack[5];else
	if(!strcmp(gx,"#PSTAND5"))NANM=&PStand[5];else
	if(!strcmp(gx,"#MOTION_L5"))NANM=&PMotionL[5];else
	if(!strcmp(gx,"#MOTION_R5"))NANM=&PMotionR[5];else
	if(!strcmp(gx,"#MOTION_LB5"))NANM=&PMotionLB[5];else
	if(!strcmp(gx,"#MOTION_RB5"))NANM=&PMotionRB[5];else
	if(!strcmp(gx,"#MINISTEP_L5"))NANM=&PMiniStepL[5];else
	if(!strcmp(gx,"#MINISTEP_R5"))NANM=&PMiniStepL[5];else
	if(!strcmp(gx,"#MINISTEP_LB5"))NANM=&PMiniStepL[5];else
	if(!strcmp(gx,"#MINISTEP_RB5"))NANM=&PMiniStepL[5];

	if(!strcmp(gx,"#PATTACK6"))NANM=&PAttack[6];else
	if(!strcmp(gx,"#UATTACK6"))NANM=&UAttack[6];else
	if(!strcmp(gx,"#ATTACK6"))NANM=&Attack[6];else
	if(!strcmp(gx,"#PSTAND6"))NANM=&PStand[6];else
	if(!strcmp(gx,"#MOTION_L6"))NANM=&PMotionL[6];else
	if(!strcmp(gx,"#MOTION_R6"))NANM=&PMotionR[6];else
	if(!strcmp(gx,"#MOTION_LB6"))NANM=&PMotionLB[6];else
	if(!strcmp(gx,"#MOTION_RB6"))NANM=&PMotionRB[6];else
	if(!strcmp(gx,"#MINISTEP_L6"))NANM=&PMiniStepL[6];else
	if(!strcmp(gx,"#MINISTEP_R6"))NANM=&PMiniStepL[6];else
	if(!strcmp(gx,"#MINISTEP_LB6"))NANM=&PMiniStepL[6];else
	if(!strcmp(gx,"#MINISTEP_RB6"))NANM=&PMiniStepL[6];else

	if(!strcmp(gx,"#PATTACK7"))NANM=&PAttack[7];else
	if(!strcmp(gx,"#UATTACK7"))NANM=&UAttack[7];else
	if(!strcmp(gx,"#ATTACK7"))NANM=&Attack[7];else
	if(!strcmp(gx,"#PSTAND7"))NANM=&PStand[7];else
	if(!strcmp(gx,"#MOTION_L7"))NANM=&PMotionL[7];else
	if(!strcmp(gx,"#MOTION_R7"))NANM=&PMotionR[7];else
	if(!strcmp(gx,"#MOTION_LB7"))NANM=&PMotionLB[7];else
	if(!strcmp(gx,"#MOTION_RB7"))NANM=&PMotionRB[7];else
	if(!strcmp(gx,"#MINISTEP_L7"))NANM=&PMiniStepL[7];else
	if(!strcmp(gx,"#MINISTEP_R7"))NANM=&PMiniStepL[7];else
	if(!strcmp(gx,"#MINISTEP_LB7"))NANM=&PMiniStepL[7];else
	if(!strcmp(gx,"#MINISTEP_RB7"))NANM=&PMiniStepL[7];else

	if(!strcmp(gx,"#PATTACK8"))NANM=&PAttack[8];else
	if(!strcmp(gx,"#UATTACK8"))NANM=&UAttack[8];else
	if(!strcmp(gx,"#ATTACK8"))NANM=&Attack[8];else
	if(!strcmp(gx,"#PSTAND8"))NANM=&PStand[8];else
	if(!strcmp(gx,"#MOTION_L8"))NANM=&PMotionL[8];else
	if(!strcmp(gx,"#MOTION_R8"))NANM=&PMotionR[8];else
	if(!strcmp(gx,"#MOTION_LB8"))NANM=&PMotionLB[8];else
	if(!strcmp(gx,"#MOTION_RB8"))NANM=&PMotionRB[8];else
	if(!strcmp(gx,"#MINISTEP_L8"))NANM=&PMiniStepL[8];else
	if(!strcmp(gx,"#MINISTEP_R8"))NANM=&PMiniStepL[8];else
	if(!strcmp(gx,"#MINISTEP_LB8"))NANM=&PMiniStepL[8];else
	if(!strcmp(gx,"#MINISTEP_RB8"))NANM=&PMiniStepL[8];else

	if(!strcmp(gx,"#PATTACK9"))NANM=&PAttack[9];else
	if(!strcmp(gx,"#UATTACK9"))NANM=&UAttack[9];else
	if(!strcmp(gx,"#ATTACK9"))NANM=&Attack[9];else
	if(!strcmp(gx,"#PSTAND9"))NANM=&PStand[9];else
	if(!strcmp(gx,"#MOTION_L9"))NANM=&PMotionL[9];else
	if(!strcmp(gx,"#MOTION_R9"))NANM=&PMotionR[9];else
	if(!strcmp(gx,"#MOTION_LB9"))NANM=&PMotionLB[9];else
	if(!strcmp(gx,"#MOTION_RB9"))NANM=&PMotionRB[9];else
	if(!strcmp(gx,"#MINISTEP_L9"))NANM=&PMiniStepL[9];else
	if(!strcmp(gx,"#MINISTEP_R9"))NANM=&PMiniStepL[9];else
	if(!strcmp(gx,"#MINISTEP_LB9"))NANM=&PMiniStepL[9];else
	if(!strcmp(gx,"#MINISTEP_RB9"))NANM=&PMiniStepL[9];else

	if(!strcmp(gx,"#PATTACK10"))NANM=&PAttack[10];else
	if(!strcmp(gx,"#UATTACK10"))NANM=&UAttack[10];else
	if(!strcmp(gx,"#ATTACK10"))NANM=&Attack[10];else
	if(!strcmp(gx,"#PSTAND10"))NANM=&PStand[10];else
	if(!strcmp(gx,"#MOTION_L10"))NANM=&PMotionL[10];else
	if(!strcmp(gx,"#MOTION_R10"))NANM=&PMotionR[10];else
	if(!strcmp(gx,"#MOTION_LB10"))NANM=&PMotionLB[10];else
	if(!strcmp(gx,"#MOTION_RB10"))NANM=&PMotionRB[10];else
	if(!strcmp(gx,"#MINISTEP_L10"))NANM=&PMiniStepL[10];else
	if(!strcmp(gx,"#MINISTEP_R10"))NANM=&PMiniStepL[10];else
	if(!strcmp(gx,"#MINISTEP_LB10"))NANM=&PMiniStepL[10];else
	if(!strcmp(gx,"#MINISTEP_RB10"))NANM=&PMiniStepL[10];else

#endif
	if(!strcmp(gx,"#REST"))NANM=&Rest;else
	if(!strcmp(gx,"#REST1"))NANM=&Rest1;else
	if(!strcmp(gx,"#REST2"))NANM=&Rest2;else
	if(!strcmp(gx,"#REST3"))NANM=&Rest3;else
	if(!strcmp(gx,"#ROTATE_R"))NANM=&RotateR;else
	if(!strcmp(gx,"#ROTATE_L"))NANM=&RotateL;else
	if(!strcmp(gx,"#WORKTREE"))NANM=&WorkTree;else
	if(!strcmp(gx,"#WORKSTONE"))NANM=&WorkStone;else
	if(!strcmp(gx,"#WORKFIELD"))NANM=&WorkField;else
	if(!strcmp(gx,"#BUILDLO_0"))NANM=&PMotionL[0];else
	if(!strcmp(gx,"#BUILDLO_1"))NANM=&PMotionL[1];else
	if(!strcmp(gx,"#BUILDLO_2"))NANM=&PMotionL[2];else
	if(!strcmp(gx,"#BUILDLO_3"))NANM=&PMotionL[3];else
	if(!strcmp(gx,"#BUILDHI_0"))NANM=&PMotionR[0];else
	if(!strcmp(gx,"#BUILDHI_1"))NANM=&PMotionR[1];else
	if(!strcmp(gx,"#BUILDHI_2"))NANM=&PMotionR[2];else
	if(!strcmp(gx,"#BUILDHI_3"))NANM=&PMotionR[3];else
    if(!strcmp(gx,"#HIANIMATION"))NANM=&Work;else
	if(!strcmp(gx,"#TRANSX3"))NANM=&TransX3;else
	if(!strcmp(gx,"#TRANS3X"))NANM=&Trans3X;
	return NANM;
	*/	
};
int GetMatherialType(char* str){
 //   if(!strcmp(str,"BODY" ))return 1; else
 //   if(!strcmp(str,"STONE"))return 2; else
 //   if(!strcmp(str,"WOOD" ))return 4; else
 //   if(!strcmp(str,"IRON" ))return 8; else
 //   if(!strcmp(str,"FLY"  ))return 16;else
	//if(!strcmp(str,"BUILDING"  ))return 32;else
	//if(!strcmp(str,"WOOD_BUILDING"  ))return 64;else
	//if(!strcmp(str,"STENA"))return 128;
 //   return -1;
	Enumerator* E=ENUM.Get("Matherials");
	int m=E->FindStr(str);
	return 1<<m;
};
int GetWeaponIndex(char* str);
char* GetTextByID(char* ID);
extern bool ProtectionMode;
int FindCTG(char* Ctg);
extern int IDLE_PX;
extern int IDLE_PY;
extern int IDLE_MX;
extern int IDLE_MY;
#ifdef GETTIRED
extern int Default_StandTiring;
extern int Default_RestTiring;
extern int Default_MotionTiring;
extern int Default_AttackTiring;
extern int Default_PAttackTiring;
extern int Default_UAttackTiring;
extern int Default_AttMotionTiring; //GEC
extern int Default_PStandTiring; //GEC
void SetDefaultAnmTiring(NewAnimation* NA,char* Name){
	NA->TiringChange=Default_StandTiring;
	if(strstr(Name,"#ATTACK"))NA->TiringChange=Default_AttackTiring;
	if(strstr(Name,"#MOTION"))NA->TiringChange=Default_MotionTiring;
	if(strstr(Name,"#REST"))NA->TiringChange=Default_RestTiring;
	if(strstr(Name,"#ATTACK"))NA->TiringChange=Default_AttackTiring;
	if(strstr(Name,"#UATTACK"))NA->TiringChange=Default_UAttackTiring;
	if(strstr(Name,"#PATTACK"))NA->TiringChange=Default_PAttackTiring;
	if(strstr(Name,"#MOTION_L0"))NA->TiringChange=Default_AttMotionTiring;  //GEC
	if(strstr(Name,"#PSTAND"))NA->TiringChange=Default_PStandTiring;  //GEC
};

extern int Morale_MinDueToTired;
extern int Morale_DecWhenTired;
extern City CITY[8];
extern int tmtmt;
int GetTiringBonus(int x,int y,int LockType);
void ApplyTiring(OneObject* OB){
	if(!EngSettings.AllowTiring)return;
	int T=OB->GetTired;
	int DT=OB->NewAnm->TiringChange;
	Brigade* BR=NULL;
	int brigid=OB->BrigadeID;
	if(brigid!=0xFFFF){
		BR=CITY[OB->NNUM].Brigs+brigid;
	}
	if(DT<0){
		int ts=256;				
		if(BR){
			void B_GoOnRoadLink(Brigade* BR);			
			//if(BR->BOrder&&BR->BOrder->BLink==&B_GoOnRoadLink)
			if(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD)
			{
				ts=GetTiringBonus(OB->RealX>>4,OB->RealY>>4,OB->LockType);
			}
		}		
		DT=(DT*ts)>>8;		
		/*
		if(OB->BrigadeID!=0xFFFF){
			Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
			if(BR->WarType){
				if(ElementaryOrders[BR->WarType-1].OrdUsage!=3)ts=100;
				int btb=BR->GetAbilityValue(17);//tiring speed bonus
				if(btb){
					if(btb>95)btb=95;
					DT=(DT*(100-btb))/100;
				}
			}
		}
		*/
	}
	if(GSets.CGame.ArcadeMode){
		extern word NPlayers;
		if(NPlayers==1){
			if(OB->NNUM==MyNation){
				if(DT<0){
                    DT>>=1;
				}else{
					DT<<=1;
				}
			}
		}else{
			if(DT<0){
				DT>>=1;
			}else{
				DT<<=1;
			}
		}
	}
	T+=(DT*GameSpeed)>>8;
	if(T<0)T=0;
	if(T>100*1000)T=100*1000;
	OB->GetTired=T;
	if(BR){	
		if(tmtmt-BR->LastTireCheckTime>16){
			if(BR->NMemb){
				int AVM=0;
				int NA=0;
				int NM=0;
				int N=BR->NMemb;
				for(int i=NBPERSONAL;i<N;i++){
					word MID=BR->Memb[i];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB&&OB->Serial==BR->MembSN[i]){
							NA++;
							if(OB->InMotion)NM++;
							AVM+=OB->GetTired/1000;
						};
					};
				};
				if(NA){
					if(AVM<=NA*2&&NM>NA/2)BR->IsTired=1;
					else BR->IsTired=0;
				};
				BR->LastTireCheckTime=tmtmt;

			};
		};
		if(BR->IsTired&&BR->NMemb){
			int N=32768/BR->NMemb;
			if(rando()<N){
				if(BR->Morale>Morale_MinDueToTired){
					BR->Morale-=Morale_DecWhenTired*GameSpeed/256;
				};
			};
		};
	}else{
		if(T==0&&OB->Morale>Morale_MinDueToTired){
			OB->Morale-=Morale_DecWhenTired*GameSpeed/256;
		};
	};
};
#define APPLY_TIRING(OB) ApplyTiring(OB)
#else 
#define SetDefaultAnmTiring(x,y)
#define APPLY_TIRING(OB)
#endif //GETTIRED
void NewMonster::InitNM(char* name){
	//memset(this,0,sizeof *this);
	//NewMonster();
	char Fn[128];
	char gx[128];
	char gy[128];
	char gz[128];
	UnitCylinderRadius=10;
	UnitCylinderHeight=45;
	FormationDistanceScale=100;
	StrikeFlySpeed=0;
	StrikeFlyMaxSpeed=0;
	StrikeForce=100;
    AttackSpeed=100;
    Speed=100;
	NotSelectable=0;
	RectDx=0;
	RectDy=3;
	RectLx=40;
	RectLy=64;
	ResEff=100;
	IconFileID=0xFFFF;
	CostPercent=100;
	ProduceStages=64;
	BattleForce=32;
	AnmUpShift=16;
	Archer=0;
	SpeedScale=256;
	selIndex=-1;
	selIndexBR=-1;
	AttackSoundID=-1;
	OrderSoundID=-1;
	SelectSoundID=-1;
	BornSoundID=-1;
	DeathSoundID=-1;
	ReadySoundID=-1;
	UpgradeSoundID=-1;
	StrikeSoundID=-1;
	MinRotator=16;
	UnitRadius=16;
	ExitPause=8;
	KillDelay=0;
	CO_MathMask=1;
	Ves=10;
	TempDelay=30;
	FireLimit=0;
	StopDistance=256;
	ArmRadius=100;	
	StartFlyHeight=60;								
	FlyHeight=100;
	MaxAttackersAmount=EngSettings.Default_MaxMeleeAttackers;
	BoidsMovingMinDist=-1;
	BoidsMovingWeight=-1;
	PlaneFactor=1.0;
	MissProbabilityForInsideUnits=EngSettings.DefaultMissInsideUnitsDamage;
	MissProbabilityOnHeightDiff100=EngSettings.DefaultMissOnHeight;
	MaxMissProbabilityOnHeightDiff=EngSettings.DefaultMissOnHeightMax;
	MotionOnRoadSound=-1;
	for(int i=0;i<NAttTypes;i++){
		Rate[i]=16;
		WeaponKind[i]=0;
		DamageDecr[i]=65535;
		FearType[i]=0xFF;
	};
	int Line=1;
	int z;
	Sprite=0xFFFF; 
	SpriteVisual=0xFFFF;
	MD_File=znew(char,strlen(name)+1);
	strcpy(MD_File,name);
	ConvertToUp(name);
	sprintf(gx,"%s.MD",name);
	Message=GetTextByID(gx);
	sprintf(gx,"%s.MD.HINT",name);
	AutoBornSprite=-1;
	if(!strcmp(Message,gx)){
		Message=znew(char,strlen(gx)+1);
		strcpy(Message,gx);
	}
	LongMessage=GetTextByID(gx);
	if(!strcmp(LongMessage,gx)){
		LongMessage=znew(char,strlen(gx)+1);
		strcpy(LongMessage,gx);
	}
	DstZPoint=EngSettings.Default_DstZPoint;
	MeatTransformIndex=0xFF; 
	WaterCheckDist1=0;
	WaterCheckDist2=60;
	Page1Icon=0xFFFF;
	Page2Icon=0xFFFF;
	Page3Icon=0xFFFF;
	Page1IconON=0xFFFF;
	Page2IconON=0xFFFF;
	Page3IconON=0xFFFF;
	IdlePPos=IDLE_PX+IDLE_PY*12;
	IdleMPos=IDLE_MX+IDLE_MY*12;
	EuropeanMask=0;
	Psixoz=32;
	IconID=0xFFFF;
	FreeShotDist=EngSettings.Default_FreeShotDist;
	BigWeapFile=GPS.PreLoadGPImage("Interf3\\FormInterface");
	BigFireWeapSprite=4;
	BigColdWeapSprite=7;
	extern int BoidsR;
	BoidsMoving=BoidsR!=0;
	//LowCollision=EngSettings.Default_LowCollision;
#ifdef COSSACKS2
	ComplexObjIndex=0xFFFF;
#endif
#ifdef NEWMORALE
	StartMorale=50;
	MoraleDecSpeed=100;
#endif //NEWMORALE
	if(Message==gx){
		Message=new char[strlen(gx)+1];
		strcpy(Message,gx);
	//	sprintf(gy,"Could not find name for monster %s",gx);
	//	ErrM(gy);
	};
#ifndef NEWMORALE
	for(i=0;i<NFEARSUBJ;i++)FearRadius[i]=255;
#endif

	BigIconFile=0xFFFF;
	MinIconFile=0xFFFF;

	VUI=0xFFFF;
	PortBackColor=0;
	PortBranch=0xFFFF;
	PortBackSprite=0xFFFF;
	StandGroundTime=50;
	FlagIndex=-1;
	SpecialSel=false;
	ShowInsideUnits=0;

	InviteMask=0;
	VisitorMask=0;
	NPlacesInTransport=1;
	for(int i=0;i<8;i++){
		UMS_Res[i]=0;
	}
	StartScale=256;
	FormFactor=256;
	CollisionRadius=EngSettings.Default_Collision;
	BornOnSprite=-1;
	UnitsColumnIndex=0xFF;
    NewParams.Init();

	VisibleRadius1=150;
};

inline char* NewMonster::GetLongMessage(){
	return (NewParams.MessageLong&&NewParams.MessageLong[0]!=0)?NewParams.MessageLong:(LongMessage?LongMessage:"");
};
bool read_next(const char* &str,char* buf,int maxsz){
	int pos=0;	
	buf[0]=0;
	int bpos=0;
	while(str[0]==' ')str++;
	if(str[0]=='"'){
        str++;
		while(str[pos]!='"' && str[pos]!=0){
			if(bpos<maxsz){
				buf[bpos++]=str[pos++];
			}else{
				pos++;
			}
		}
		buf[bpos]=0;
		if(str[pos]!=0)pos++;		
	}else{
		while(str[pos]!=' ' && str[pos]!=0){
			if(bpos<maxsz){
				buf[bpos++]=str[pos++];
			}else{
				pos++;
			}
		}
		buf[bpos]=0;
		if(str[pos]!=0)pos++;		
	}
	str+=pos;
	return buf[0]!=0;
}
//reading md-file
bool NewMonster::CreateFromFile(char* name){	
	if(!name)return false;
	extern int CurrentAnmQuality;
	CurrentAnmQuality=GSets.AnimQuality;
	char Fn[128];
	char gx[128];
	char gy[128];
	char gz[128];
	int MaxRLC=-1;
	word RLCRef[128];
	short RLCdx[128];
	short RLCdy[128];
	byte  RLCParts[128];
	byte  RLCPartSize[128];
	int z1,z2,z3,z4,p1,p2,p3,p4,p5,p6,xx,yy;
	sprintf(Fn,"%s.md",name);
	ProtectionMode=1;
	GFILE* f1=Gopen(Fn,"r");
	ProtectionMode=0;
	AFile(Fn);
	bool finish=false;
	short NAStartDx=0;
	short NAStartDy=0;
	byte NAParts=1;
	byte NAPartSize=96;
	int z;
	int Line=1;

	InitNM(name);
	Animations=new AnimationsScope;
    
    static OneClassStorage* CPS=CGARB.GetClass("CommonParameters");
    assert(CPS);

	if(f1){
		do{
			z=Gscanf(f1,"%s",gx);
			if(z!=1)IncPar(name,Line,"COMMAND");
			if(gx[0]!='/'&&gx[0]!='['){
				if(gx[0]=='*'){//*ANIMID Nparts    NFrames1 Scale1 Model1 Animation1 AddDir1 StartTime1 EndTime1  ...
					gx[0]='#';
					NewAnimation* NANM=CreateAnimation(gx);
					if(NANM){
						NANM->Enabled=true;
						NANM->AnimationType=1;
						int np=0;
						z=Gscanf(f1,"%d",&np);
                        NANM->NFrames=0;
						for(int j=0;j<np;j++){
							int nf=0;
							float scal=1.0f;
							float addr=0;
							float stt=0;
							float ent=0;
							char mesh[256];
							char anim[256];
							z=Gscanf(f1,"%d%f%s%s%f%f%f",&nf,&scal,mesh,anim,&addr,&stt,&ent);
							if(z==7){
								AnimFrame3D* AF3=new AnimFrame3D;
								IMM->GetModelID(mesh);
								AF3->Model=IMM->GetModelID(mesh);
								AF3->Scale=scal;
								AF3->AddDir=addr;
								AF3->StartAnmTime=stt;
								AF3->FinalAnmTime=ent;
								AF3->NFrames=nf;
								NANM->NFrames+=nf;
								if(AF3->Model==-1){
									sprintf(gy,"%s,line %d : Unable to load model [%s] for animation: %s",name,Line,mesh,gx);
									ErrM(gy);
								}
								if(strcmp(anim,"none")){
									AF3->Animation=IMM->GetModelID(anim);							
									if(AF3->Animation==-1){
										sprintf(gy,"%s,line %d : Unable to load animation [%s] for animation: %s",name,Line,anim,gx);
										ErrM(gy);
									}
								}else AF3->Animation=-1;
                                NANM->AnimSet3D.Add(AF3);
							}else{
								sprintf(gy,"%s,line %d : Bad parameters for animation: %s",name,Line,gx);
								ErrM(gy);
							}
						}
						NLine(f1);
						Line++;
					}else{
						sprintf(gy,"%s,line %d : Unknown animation type: %s",name,Line,gx);
						ErrM(gy);
					}
				}else
				if(gx[0]=='!'){
					gx[0]='#';
					NewAnimation* NANM=CreateAnimation(gx);
					if(NANM){
                        NANM->Enabled=true;
						int nf=0;
						float scal=1.0f;
						char mesh[256];
						char anim[256];
                        z=Gscanf(f1,"%d%f%s%s",&nf,&scal,mesh,anim);
						if(z==4){
							NANM->ModelID=IMM->GetModelID(mesh);
							NANM->AnimationType=true;
							NANM->Scale=scal;
							NANM->NFrames=abs(nf); 
							NANM->Inverse=nf<0; 
							if(NANM->ModelID==-1){   
								Log.Warning("%s,line %d : Unable to load model [%s] for animation: %s",name,Line,mesh,gx);
							}
							if(strcmp(anim,"none")){
								NANM->AnimationID=IMM->GetModelID(anim);							
								if(NANM->AnimationID==-1){
									Log.Warning("%s,line %d : Unable to load animation [%s] for animation: %s",name,Line,anim,gx);
								}
							}else NANM->AnimationID=-1;
						}else{
							sprintf(gy,"%s,line %d : Bad parameters for animation: %s",name,Line,gx);
							ErrM(gy);
						}
						NLine(f1);
						Line++;
					}else{
						sprintf(gy,"%s,line %d : Unknown animation type: %s",name,Line,gx);
						ErrM(gy);
					}
				}else
				if(gx[0]=='#'){
					//Animation;
					NewAnimation* NANM=CreateAnimation(gx);
					if(NANM){
						SetDefaultAnmTiring(NANM,gx);
						z=Gscanf(f1,"%d%d",&z1,&z2);
						if(z!=2)IncPar(name,Line,gx);
						//NANM->StartDx=NAStartDx;
						//NANM->StartDy=NAStartDy;
						NANM->Parts=NAParts;
						NANM->PartSize=NAPartSize;
						NANM->Enabled=true;
						NANM->NFrames=z2;
						NANM->Rotations=z1;
						int nrot=z1;
						NANM->ActiveFrame=0;
						//if(NANM->ActivePtX)free(NANM->ActivePtX);
						//if(NANM->ActivePtY)free(NANM->ActivePtY);
						NANM->ActivePt.Clear();
						//NANM->ActivePtX=znew(short,z1+z1+z1);
						//NANM->ActivePtY=znew(short,z1+z1+z1);
						OneActivePt AP;
						NANM->ActivePt.Add(AP,z1+z1+z1);
						NANM->HotFrame=0;
						NANM->SoundID=-1;
						NANM->LineInfo=NULL;
						for(int i=0;i<z1+z1+z1;i++){
							NANM->ActivePt[i]->x=0;
							NANM->ActivePt[i]->y=0;
						};
						//if(NANM->Frames)delete[](NANM->Frames);
						NANM->Frames.Clear();
						//NANM->Frames=new NewFrame[z2];
						//for(i=0;i<z2;i++)NANM->Frames.Add(new NewFrame());
						NANM->AllocFrames(z2);
						for(int i=0;i<z2;i++){
							z=Gscanf(f1,"%d%d",&z1,&z3);
							if(z!=2)IncPar(name,Line,gx);
							if(z1>MaxRLC){
								Log.Error( "%s,line %d : Incorrect RLC reference: %d",name,Line,z1);
							};
							NewFrame* NF=NANM->Frames[i];
							NF->FileID=RLCRef[z1];
							//if((z3+1)*nrot>GPS.GPNFrames(RLCRef[z1])){
							//	Log.Error( "%s,line %d : Reference to RLC sprite is beyond the end of file: %d",name,Line,z3);
							//};
							NF->SpriteID=z3;							
							if(Building){
								NF->dx=PicDx;
								NF->dy=PicDy;
							}else{
								NF->dx=RLCdx[z1];
								NF->dy=RLCdy[z1];
							}
						};
						NLine(f1);
						Line++;
					}else{
						Log.Error( "%s,line %d : Unknown animation type: %s",name,Line,gx);
					};
				}else
				if(gx[0]=='@'){
					//Animation;
					gx[0]='#';
					NewAnimation* NANM=CreateAnimation(gx);
					if(NANM){
						SetDefaultAnmTiring(NANM,gx);

						z=Gscanf(f1,"%d%d%d%d",&z1,&z2,&z3,&z4);
						if(z!=4)NEPar(name,Line,gx,4);
						//NANM->StartDx=NAStartDx;
						//NANM->StartDy=NAStartDy;
						NANM->Parts=NAParts;
						NANM->PartSize=NAPartSize;
						NANM->Enabled=true;
						NANM->Rotations=z1;
						NANM->ActiveFrame=0;
						NANM->HotFrame=0;
						NANM->SoundID=-1;
						//if(NANM->ActivePtX)free(NANM->ActivePtX);
						//if(NANM->ActivePtY)free(NANM->ActivePtY);
						NANM->ActivePt.Clear();
						//NANM->ActivePtX=znew(short,z1+z1+z1);
						//NANM->ActivePtY=znew(short,z1+z1+z1);
						OneActivePt AP;
						NANM->ActivePt.Add(AP,z1+z1+z1);
						NANM->LineInfo=NULL;
						for(int i=0;i<z1+z1+z1;i++){
							NANM->ActivePt[i]->x=0;
							NANM->ActivePt[i]->y=0;
						};
						int dz,nz;
						p1=RLCRef[z2];
						if(z3>=z4){
							//if((z3+1)*z1>GPS.GPNFrames(p1)){
							//	Log.Error( "%s,line %d : Reference to RLC sprite is beyond the end of RLC file.",name,Line);
							//};
							dz=-1;
							nz=z3-z4+1;
						}else{
							//if((z4+1)*z1>GPS.GPNFrames(p1)){
							//	Log.Error( "%s,line %d : Reference to RLC sprite is beyond the end of RLC file.",name,Line);
							//};
							dz=1;
							nz=z4-z3+1;


						};
						NANM->NFrames=nz;
						//if(NANM->Frames)delete[](NANM->Frames);
						//NANM->Frames=new NewFrame[nz];
						NANM->Frames.Clear();
						//for(i=0;i<nz;i++)NANM->Frames.Add(new NewFrame());
						NANM->AllocFrames(nz);
						for(int i=0;i<nz;i++){
							NewFrame* NF=NANM->Frames[i];
							NF->FileID=p1;
							NF->SpriteID=z3;
							z3+=dz;
							if(Building&&(NANM->Code<1500||NANM->Code>=1600)){
								NF->dx=PicDx;
								NF->dy=PicDy;
							}else{
								NF->dx=RLCdx[z2];
								NF->dy=RLCdy[z2];
							}
						}
						NLine(f1);
						Line++;
					}else{
						gx[0]='@';
						Log.Error( "%s,line %d : Unknown animation type: %s",name,Line,gx);
					};
				}else
				if(gx[0]=='$'){
					//Animation;
					gx[0]='#';
					NewAnimation* NANM=CreateAnimation(gx);
					if(NANM){
						SetDefaultAnmTiring(NANM,gx);
						int NPARTS=0;
						int P_File[128];
						int P_Start[128];
						int P_End[128];
						int NP=0;
						int NF=0;
						
						z=Gscanf(f1,"%d%d",&z1,&NP);
						if(z!=2)NEPar(name,Line,gx,2);
						for(int i=0;i<NP;i++){
							z=Gscanf(f1,"%d%d%d",&P_File[i],&P_Start[i],&P_End[i]);
							if(z!=3)IncPar(name,Line,gx);
							NF+=abs(P_Start[i]-P_End[i])+1;
						};
						//NANM->StartDx=NAStartDx;
						//NANM->StartDy=NAStartDy;
						NANM->Parts=NAParts;
						NANM->PartSize=NAPartSize;
						NANM->Enabled=true;
						NANM->Rotations=z1;
						NANM->ActiveFrame=0;
						NANM->HotFrame=0;
						NANM->SoundID=-1;
						//if(NANM->ActivePtX)free(NANM->ActivePtX);
						//if(NANM->ActivePtY)free(NANM->ActivePtY);
						NANM->ActivePt.Clear();
						OneActivePt AP;
						NANM->ActivePt.Add(AP,z1+z1+z1);
						//NANM->ActivePtX=znew(short,z1+z1+z1);
						//NANM->ActivePtY=znew(short,z1+z1+z1);
						NANM->LineInfo=NULL;
						for(int i=0;i<z1+z1+z1;i++){
							NANM->ActivePt[i]->x=0;
							NANM->ActivePt[i]->y=0;
						};
						NANM->NFrames=NF;
						//if(NANM->Frames)delete[](NANM->Frames);
						//NANM->Frames=new NewFrame[NF];
						NANM->Frames.Clear();
						//for(i=0;i<NF;i++)NANM->Frames.Add(new NewFrame());
						NANM->AllocFrames(NF);
						int p=0;
						for(int i=0;i<NP;i++){
							int s=P_Start[i]>P_End[i]?-1:1;
							int fn=P_End[i]+s;
							for(int sp=P_Start[i];sp!=fn;sp+=s){
								NewFrame* NF=NANM->Frames[p];
								NF->FileID=RLCRef[P_File[i]];
								NF->SpriteID=sp;
								if(Building){
									NF->dx=PicDx;
									NF->dy=PicDy;
								}else{
									NF->dx=RLCdx[P_File[i]];
									NF->dy=RLCdy[P_File[i]];
								}
								p++;
							};
						};
						NLine(f1);
						Line++;
					}else{
						gx[0]='$';
						Log.Error( "%s,line %d : Unknown animation type: %s",name,Line,gx);
					};
				}else{
					int Line0=Line;
                    //attempt to read information from md-file to NewMonster::NewParams in xml-like style
                    int Ne=CPS->Members.GetAmount();
                    for(int i=0;i<Ne;i++){
                        if(!stricmp(CPS->Members[i]->xmlID,gx)){
							OneClassMemberStorage* OCMS=CPS->Members[i];
							const char* ec=OCMS->Member->GetExpansionBaseClass();
							char  dest[1024];
							if(ec){								
								ReadWinString(f1,dest,1023);
								const char* curr=dest;
								xmlQuote root;
                                OneClassStorage* OCS=CGARB.GetClass(ec);
								if(ec){
                                    int n=OCS->Members.GetAmount();
									char m[128];
									bool good=true;
									do{
										xmlQuote* xml=new_xmlQuote((char*)ec);										
										for(int i=0;i<n;i++){
											read_next(curr,m,128);
											if(m[0]==0 ){
												if(i!=0){
													IncPar(name,Line,gx);													
												}
												good=false;												
												break;
											}else{
												xmlQuote* xm2=new_xmlQuote(OCS->Members[i]->xmlID);
												xm2->Assign_string(m);
												xml->AddSubQuote(xm2);
											}
										}
										if(good){
											root.AddSubQuote(xml);
										}else{
											delete_xmlQuote(xml);
										}
									}while(good);
									ErrorPager ep(1);
									BaseClass* BC=(BaseClass*)(((byte*)&NewParams)+OCMS->OffsetFromClassRoot);
									memcpy(BC,OCMS->Member,4);
									BC->Load(root,BC,&ep);
								}
							}else
							if(OCMS->Member->GetAmountOfElements()>1){
								ReadWinString(f1,dest,1023);
								const char* curr=dest;
								xmlQuote root;
								OneClassStorage* OCS=CGARB.GetClass(ec);
								if(ec){
									int n=OCS->Members.GetAmount();
									char m[128];
									bool good=true;
									for(int i=0;i<n;i++){
										read_next(curr,m,128);
										if(m[0]==0 ){
											if(i!=0){
												IncPar(name,Line,gx);													
											}
											good=false;												
											break;
										}else{
											xmlQuote* xm2=new_xmlQuote(OCS->Members[i]->xmlID);
											xm2->Assign_string(m);
											root.AddSubQuote(xm2);
										}
									}
									ErrorPager ep(1);
									BaseClass* BC=(BaseClass*)(((byte*)&NewParams)+OCMS->OffsetFromClassRoot);
									memcpy(BC,OCMS->Member,4);
									BC->Load(root,BC,&ep);
								}
							}else{
								OneClassMemberStorage* OCMS=CPS->Members[i];
								strcpy(dest,"<>");
								ReadWinString(f1,dest+2,511);
								strcat(dest,"</>");
								xmlQuote xml;
								xml.Load(dest);
								ErrorPager ep(1);
								static CommonParameters cp;
								*((DWORD*)&NewParams)=*((DWORD*)&cp);
								OCMS->Member->Load(xml,((byte*)&NewParams)+OCMS->OffsetFromClassRoot,&ep);								
							}
							Line++;
							Ne=-1;
                            break;
                        }
                    }
                    if(Ne==-1)continue;
					//Simple command
					switch(gx[0]){
					case 'A':			
						if(!strcmp(gx,"ANMHARDNESS")){
							float f;
							char anm[64];
							z=Gscanf(f1,"%s%f",gx,&f);
							if(z!=2)IncPar(name,Line,gx);
							{
								NewAnimation* NANM=LoadNewAnimationByName(gx,-1);
								if(NANM&&NANM->Enabled){
									NANM->Hardness=f;
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTSPEEDSCALE")){
							z=Gscanf(f1,"%d",&p2);
							if(z!=1)IncPar(name,Line,"ATTSPEEDSCALE");
							AttackSpeed=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ACTIVEBONE")){
                            int z=Gscanf(f1,"%s%s",gy,gz);
							if(z==2){
								OneBoneInfo* OBI=new OneBoneInfo;
								OBI->ModelName=gy;
								OBI->NodeName=gz;
								OBI->MainDir=Vector3D::oZ;
								ActiveBones.Add(OBI);
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ACTIVEBONE3D")){
							Vector3D v;
							int z=Gscanf(f1,"%s%s%f%f%f",gy,gz,&v.x,&v.y,&v.z);
							if(z==5){
								OneBoneInfo* OBI=new OneBoneInfo;
								OBI->ModelName=gy;
								OBI->NodeName=gz;
								OBI->MainDir=v;
								ActiveBones.Add(OBI);
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ALIGN_WITH_3POINTS")){
                            int z=Gscanf(f1,"%d%d%d%d%d%d%d%d%d"
								,&AlignPt1x,&AlignPt1y,&AlignPt1z
								,&AlignPt2x,&AlignPt2y,&AlignPt2z
								,&AlignPt3x,&AlignPt3y,&AlignPt3z);
							if(z!=9)IncPar(name,Line,gx);
							Use3pAlign=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTPREVIEW1")){
							int R2;
							int z=Gscanf(f1,"%d",&R2);
							if(z==1){
								VisibleRadius1=R2;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTPREVIEW2")){
							int R2;
							int z=Gscanf(f1,"%d",&R2);
							if(z==1){
								VisibleRadius2=R2;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ANMEXT")){//ANMEXT #SRCANM #SUBANM dx dy dz scale Period
							int x,y,z,p;
							float sc;
							int zz=Gscanf(f1,"%s%s%d%d%d%f%d",gy,gz,&x,&y,&z,&sc,&p);
							if(zz==7){
								{
									NewAnimation* NANM=LoadNewAnimationByName(gy,0);
									if(NANM){
                                        NewAnimation* SUB=LoadNewAnimationByName(gz,0);													
										if(SUB){
                                            AnimationExtension* AEX=new AnimationExtension;
											AEX->NA=SUB;
											AEX->dx=x;
											AEX->dy=y;
											AEX->dz=z;
											AEX->Scale=sc;
											AEX->dFi=0;
											AEX->dDir=0;
											AEX->Period=p;
											NANM->AnmExt.Add(AEX);
										}
									}
								}
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"AUTOROTATE")){
							int na=0;
							char fL[32];
							char fR[32];
							int dx,dy;
							int z=Gscanf(f1,"%d%d%s%s",&dx,&dy,fL,fR);
							if(z==4){
								NewAnimation* L=CreateAnimation("#ROTATEL");
								NewAnimation* R=CreateAnimation("#ROTATER");
								int LF=GPS.PreLoadGPImage(fL);
								int RF=GPS.PreLoadGPImage(fR);
								for(int i=0;i<128;i++){
									NewFrame* NFL=new NewFrame;
									NFL->FileID=LF;
									NFL->dx=dx;
									NFL->dy=dy;
									NFL->SpriteID=((16-(i/16)*2)%16)+(i%16)*16;
									L->Frames.Add(NFL);
									NewFrame* NFR=new NewFrame;
									NFR->FileID=RF;
									NFR->dx=dx;
									NFR->dy=dy;
									NFR->SpriteID=((i/16)*2)+(i%16)*16;
									R->Frames.Add(NFR);
								}
								L->NFrames=128;
								R->NFrames=128;
								L->Rotations=1;
								R->Rotations=1;
								L->Enabled=1;
								R->Enabled=1;
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ANMSUMM")){
							int na=0;
                            int z=Gscanf(f1,"%s%d",gy,&na);
							if(z==2){
								NewAnimation* NAS[32];
								int NNAS=0;
								for(int j=0;j<na;j++){
									char gg[128];
									z=Gscanf(f1,"%s",gg);
									if(z==1){
                                        NewAnimation* NA=LoadNewAnimationByName(gg,0);
										if(NA&&NNAS<32){
											NAS[NNAS]=NA;
											NNAS++;
										}
									}else IncPar(name,Line,gx);
								}
								{
									NewAnimation* NA=LoadNewAnimationByName(gy,0);
									if(NA){
										for(int p=0;p<NNAS;p++){
											NewAnimation* NA1=NAS[p];
											int nf=NA1->NFrames;
											for(int j=0;j<nf;j++){
												NewFrame* NF=new NewFrame;
												NA1->Frames[j]->Copy(NF);
												NA->Frames.Add(NF);
												NA->NFrames=NA->Frames.GetAmount();
											}
										}
									}
								}
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;							
						}else
						//if(!strcmp(gx,"ABILITY")){
						if(strstr(gx,"ABILITY")){
							int CommonRoomStartParam=0;
							if(!strcmp(gx,"ABILITYOPT1")){
								CommonRoomStartParam=1;
							}else
							if(!strcmp(gx,"ABILITYOPT2")){
								CommonRoomStartParam=2;
							}
                            char com1[128];
							int val1;
							int z=Gscanf(f1,"%s",gy);
							if(z>0){
								bool AddMonsterAbility(MonsterAbility** MA,char* Name);
								if(AddMonsterAbility(&Ability,gy)){
									Ability->ItemDrop.Add(-1);
									Ability->CommonRoomStartParam.Add(CommonRoomStartParam);
								};
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;							
						}else
						if(!strcmp(gx,"ABLDROP")){
							char com1[128];
							int val1;
							int z=Gscanf(f1,"%s%d",gy,&val1);
							if(z>1){
								for(int i=0;i<Ability->AbilityNames.GetAmount();i++){
									if((*Ability->AbilityNames[i])==gy){
										Ability->ItemDrop[i]=val1;
										break;
									}
								}
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;							
						}else
						if(!strcmp(gx,"ANMSHIFT")){
							int x,y;
							int z=Gscanf(f1,"%s%d%d",gy,&x,&y);
							if(z==3){
								{
									NewAnimation* NANM=LoadNewAnimationByName(gy,0);
									if(NANM){
										for(int i=0;i<NANM->NFrames;i++){
											NANM->Frames[i]->dx=x;
											NANM->Frames[i]->dy=y;
										}									
									}
								}
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ADDHDIR")){
							int A,H;
							int z=Gscanf(f1,"%s%d%d",gy,&H,&A);
							if(z==3){
								{
									NewAnimation* NANM=LoadNewAnimationByName(gy,0);
									if(NANM){
										NANM->AddDirection=A;
										NANM->AddHeight=H;
									}
								}
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ARMRADIUS")){
							int R2;
							int z=Gscanf(f1,"%d",&R2);
							if(z==1){
								ArmRadius=R2;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"AISHIP")){
							char cc[32];
							int R1,R2;
							int z=Gscanf(f1,"%s%d%d",cc,&R1,&R2);
							if(z==3){
								if(!strcmp(cc,"B"))AI_use_against_buildings=1;
								AI_PreferredAttR_Min=R1;
								AI_PreferredAttR_Max=R2;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ANIMAL")){
							Animal=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ARMATTACK")){
							ArmAttack=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ARTDEPO")){
							p1=5;p2=5;p3=5;p4=5;p5=5;							
							z=Gscanf(f1,"%d%d%d%d",&p1,&p2,&p3,&p4,&p5);
							ArtDepo=true;
							ArtCap[0]=p1;
							ArtCap[1]=p2;
							ArtCap[2]=p3;
							ArtCap[3]=p4;
							ArtCap[4]=p5;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ARTPLACE")){
							p1=0;p2=0;
							z=Gscanf(f1,"%d%d",&p1,&p2);
							ArtDepo=true;
							ArtCap[p1]=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ADDSECTION")){
							//ADDSECTION Index Gp AddPts Empty x y  Stage1 x y Ready x y Dead x y
							int Idx,gp,apt,st1,x1,y1,st2,x2,y2,st3,x3,y3,st4,x4,y4;
							int z=Gscanf(f1,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",&Idx,&gp,&apt,
								&st1,&x1,&y1,
								&st2,&x2,&y2,
								&st3,&x3,&y3,
								&st4,&x4,&y4);
							if(z==15){
								if(!CompxCraft){
									CompxCraft=new ComplexBuilding;
									memset(CompxCraft,0,sizeof ComplexBuilding);
								};
								if(Idx>=MaxAStages){
									Log.Error( "%s,line %d : ADDSECTION : Too large stage index",name,Line);
								}else{
									CompxCraft->Mask|=1<<Idx;
									CompxCraft->Stages[Idx].GPID=RLCRef[gp];
									CompxCraft->Stages[Idx].AddPoints=apt;
	
									CompxCraft->Stages[Idx].Empty.SpriteID=st1;
									CompxCraft->Stages[Idx].Empty.SortX=x1;
									CompxCraft->Stages[Idx].Empty.SortY=y1;
	
									CompxCraft->Stages[Idx].Stage1.SpriteID=st2;
									CompxCraft->Stages[Idx].Stage1.SortX=x2;
									CompxCraft->Stages[Idx].Stage1.SortY=y2;
		
									CompxCraft->Stages[Idx].Ready.SpriteID=st3;
									CompxCraft->Stages[Idx].Ready.SortX=x3;
									CompxCraft->Stages[Idx].Ready.SortY=y3;
	
									CompxCraft->Stages[Idx].Dead.SpriteID=st4;
										CompxCraft->Stages[Idx].Dead.SortX=x4;
									CompxCraft->Stages[Idx].Dead.SortY=y4;
	
								};
							}else{
								Log.Error( "%s,line %d : ADDSECTION : Not enough or wrong parameters",name,Line);
							};
						}else
						if(!strcmp(gx,"ATTACK_PAUSE")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,"ATTACK_PAUSE");
							if(p1>=NAttTypes){
								Log.Error( "%s,line %d : Attack type index in ATTACK_PAUSE is too big.",name,Line);
							};
							AttackPause[p1]=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTACK_ANGLES")){
							z=Gscanf(f1,"%d%d%d",&p1,&p2,&p3);
							if(z!=3)IncPar(name,Line,"ATTACK_ANGLES");
							if(p1>=NAttTypes){
								Log.Error( "%s,line %d : Attack type index in SHOT_ANGLE is too big.",name,Line);
							};
							AngleUp[p1]=p3;
							AngleDn[p1]=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ADD_ATTACK_RADIUS")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,"ADD_ATTACK_RADIUS");
							if(p1>=NAttTypes){
								Log.Error( "%s,line %d : Attack type index in ADD_ATTACK_RADIUS is too big.",name,Line);
							};
							AttackRadiusAdd[p1]=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTACK_RADIUS")){
							z=Gscanf(f1,"%d%d%d",&p1,&p2,&p3);
							if(z!=3)IncPar(name,Line,gx);
							if(p1>=NAttTypes){
								Log.Error( "%s,line %d : Attack type index in ATTACK_RADIUS is too big.",name,Line);
							};
							AttackRadius1[p1]=p2;
							AttackRadius2[p1]=p3;
							DetRadius1[p1]=p2;
							DetRadius2[p1]=p3;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTMASK")){
							z=Gscanf(f1,"%d%d",&p1,&p4);
							if(z!=2)IncPar(name,Line,gx);
							AttackMask[p1]=0;
							for(int k=0;k<p4;k++){
								z=Gscanf(f1,"%s",gy);
								p2=GetMatherialType(gy);
								if(p2!=-1){
									AttackMask[p1]|=p2;
									if(!strcmp(gy,"BUILDING")){
										AttBuild=true;
									};
								}else{
									Log.Error( "%s, Line: %d :ATTMASK: Unknown matherial:%s",name,Line,gy);
								};
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ARCHER")){
							Archer=1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"ANMSH")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							AnmUpShift=p1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"ATTACK_SOUND")){
							z=Gscanf(f1,"%s",gy);
							AttackSoundID=SearchStr(SoundID,gy,NSounds);
							if(AttackSoundID==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"AUTO_NO_ATTACK")){
							AutoNoAttack=1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"AUTO_STAND_GROUND")){
							AutoStandGround=1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"ARTPODGOTOVKA")){
							Artpodgotovka=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ADDSHOTRADIUS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							AddShotRadius=p1;
							NLine(f1);
							Line++;
						}else{
							Log.Error( "%s,line %d : Unknown command : %s",name,Line,gx);
						};
						break;
					case 'B':						
						if(!strcmp(gx,"BORNONSPRITE")){
							z=Gscanf(f1,"%s",gy);
							p1=TREES.GetIndexByName(gy);
							if(p1==-1){
								p1=STONES.GetIndexByName(gy);
								if(p1==-1){
									Log.Error( "%s, line %d : %s : Unknown Sprite Object : %s",name,Line,gx,gy);
								}else p1+=4096;
							}							
							BornOnSprite=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BORNSPRITE")){
							z=Gscanf(f1,"%s",gy);
							p1=TREES.GetIndexByName(gy);
							if(p1==-1){
								Log.Error( "%s, line %d : %s : Unknown Sprite Object : %s",name,Line,gx,gy);
							};							
							AutoBornSprite=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BODYSTYLE")){
							char bs[256];
							z=Gscanf(f1,"%s",&bs);
							//
							Enumerator* E=ENUM.Get("BODYSTYLE");							
							BodyStyle=E->FindStr(bs);
							if(BodyStyle<0)BodyStyle=0;
							//							
							if(z!=1)IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BORN_BEHIND_BUILDING")){
							BornBehindBuilding=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BRIGADEWAITINGCYCLES")){
							z=Gscanf(f1,"%d",&p1);
							BrigadeWaitingCycles=p1;
							if(z!=1)IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BOIDSMOVING")){
							BoidsMoving=true;
							int r,d;
							BoidsMovingMinDist=-1;
							BoidsMovingWeight=-1;
							z=Gscanf(f1,"%d%d",&r,&d);
							if(z==2){
								BoidsMovingMinDist=r;
								BoidsMovingWeight=d;
							}
							Line++;
						}else
						if(!strcmp(gx,"BOMB")){
							int r,d;
							z=Gscanf(f1,"%d%d",&r,&d);
							if(z==2){
                                MineRadius=r;
								MineDamage=d;
							}
							Line++;
						}else
						if(!strcmp(gx,"BUILDNEARBUILDING")){
							int r;
							z=Gscanf(f1,"%d",&r);
							if(z==1){
								BuildNearBuildingRadius=r;
							}
							Line++;
						}else							
						if(!strcmp(gx,"BUILDHOTKEY")){
							char HK[32];
							z=Gscanf(f1,"%s",HK);
							if(z==1){
								//BuildHotKey=HK[0];
								BuildHotKey=GetKeyByName(HK);
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BIGFIREWEAP")){
							char gp[128];
							z=Gscanf(f1,"%s%d",gp,&p1);
							if(z==2){
								BigWeapFile=GPS.PreLoadGPImage(gp);
								BigFireWeapSprite=p1;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BIGCOLDWEAP")){
							char gp[128];
							z=Gscanf(f1,"%s%d",gp,&p1);
							if(z==2){
								BigWeapFile=GPS.PreLoadGPImage(gp);
								BigColdWeapSprite=p1;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;						
						}else
						if(!strcmp(gx,"BIGICON")){
							z=Gscanf(f1,"%s%d",gy,&p1);
							if(z==2){
								BigIconFile=0xFFFF;
								BigIconFile=GPS.PreLoadGPImage(gy);
								if(BigIconFile==0xFFFF){
									Log.Error( "%s,line %d : Icon file not exists [%s]: %s",name,Line,gx,gy);
								}
								BigIconIndex=p1;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BREFLECT")){
							BReflection=true;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BUILDING")){
							Building=true;
							Ves=500;
							WaterCheckDist1=180;
							WaterCheckDist2=250;
			                NLine(f1);
							FireLimit=15;
							Line++;
							EuropeanMask=100;
							MaxAttackersAmount=200;
							if(TFactor==0)TFactor=0xFF949494; //CHANGED BY GEC FROM FFA0A0A0
						}else
						if(!strcmp(gx,"BREAKANIMATION")){
							z=Gscanf(f1,"%s",gx);
							if(z!=1)IncPar(name,Line,gx);
							{
								for(int i=0;i<NAttTypes;i++){
									NewAnimation* NANM=LoadNewAnimationByName(gx,i);
									if(NANM){
										NANM->CanBeBrokenAfterFrame=0;
									}
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BREAKAFTERFRAME")){
							int ff=0;
							z=Gscanf(f1,"%s%d",gx,&ff);
							if(z!=2)IncPar(name,Line,gx);
							{
								for(int i=0;i<NAttTypes;i++){
									NewAnimation* NANM=LoadNewAnimationByName(gx,i);
									if(NANM){
										NANM->CanBeBrokenAfterFrame=ff;
									}
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BARABAN")){
							Baraban=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BORN_SOUND")){
							z=Gscanf(f1,"%s",gy);
							BornSoundID=SearchStr(SoundID,gy,NSounds);
							if(BornSoundID==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"BATTLEFORCE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							BattleForce=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BUILDBAR")){
							z=Gscanf(f1,"%d%d%d%d",&p1,&p2,&p3,&p4);
							if(z!=4)IncPar(name,Line,gx);
							BuildX0=PicDx+(p1<<4);
							BuildY0=(PicDy+(p2<<3))<<1;
							BuildX1=PicDx+(p3<<4);
							BuildY1=(PicDy+(p4<<3))<<1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BUILDLOCKPOINTS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							NBLockPt=p1;
							BLockX=znew(char,p1);
							BLockY=znew(char,p1);
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,gx);
								BLockX[i]=p2;
								BLockY[i]=p3;
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BORNPOINTS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"BORNPOINTS");
							BornPtX.Clear();
							BornPtY.Clear();
							BornPtZ.Clear();
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,"BORNPOINTS");
								BornPtX.Add(p2*16+8);
								BornPtY.Add(p3*16+8);
								BornPtZ.Add(0);
							};
							NLine(f1);
							Line++;
						}else						
						if(!strcmp(gx,"BORNPOINTS2")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"BORNPOINTS");
							BornPtX.Clear();
							BornPtY.Clear();
							BornPtZ.Clear();
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,gx);
								BornPtX.Add(p2);
								BornPtY.Add(p3<<1);
								BornPtZ.Add(0);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BORNPOINTS3")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"BORNPOINTS");
							BornPtX.Clear();
							BornPtY.Clear();
							BornPtZ.Clear();
							for(int i=0;i<p1;i++){
								int p4;
								z=Gscanf(f1,"%d%d%d",&p2,&p3,&p4);
								if(z!=3)IncPar(name,Line,"BORNPOINTS");
								BornPtX.Add(p2);
								BornPtY.Add(p3);
								BornPtZ.Add(p4);
							};
							if(!ConcPtX.GetAmount()){
                                for(int i=0;i<p1;i++){
									ConcPtX.Add(BornPtX[p1-i-1]);
									ConcPtY.Add(BornPtY[p1-i-1]);
									ConcPtZ.Add(BornPtZ[p1-i-1]);
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BUILDPOINTS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"BUILDPOINTS");
							BuildPtX.Clear();
							BuildPtY.Clear();
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,"BUILDPOINTS");
								BuildPtX.Add(p2);
								BuildPtY.Add(p3);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BUILDSTAGES")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"BUILDSTAGES");
							ProduceStages=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BRANDOMPOS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							BRandomPos=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"BRANDOMSPEED")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							BRandomSpeed=p1;
							NLine(f1);
							Line++;
						}else{
							Log.Error( "%s,line %d : Unknown command : %s",name,Line,gx);
						};
						break;
					case 'C':
						if(!strcmp(gx,"CYLINDER")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)NEPar(name,Line,gx,1);
							UnitCylinderRadius=p1;
							UnitCylinderHeight=p2;							
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"COLUMN")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							UnitsColumnIndex=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"COLLISION")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							CollisionRadius=p1;
							//Radius2=max(Radius2,CollisionRadius*16);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CANBUILD")){
							CanBuild=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CANBEINFOCUSOFFORMATION")){
							CanBeInFocusOfFormation=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"COLORVARIATION")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							ColorVariation=p1;
							NLine(f1);
							Line++;
						}else
#ifdef COSSACKS2
						if(!strcmp(gx,"COMPLEXOBJECT")){
							z=Gscanf(f1,"%s",gy);
							word GetComplexObjectIndex(char* Name);
							ComplexObjIndex=GetComplexObjectIndex(gy);
							if(ComplexObjIndex==0xFFFF){
								Log.Error( "%s,line %d : Unknown complex object : %s",name,Line,gy);
							};
						}else
#endif //COSSACKS2
						if(!strcmp(gx,"CANSTOPBUILD")){
							CanStopBuild=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CATEGORY")){
							z=Gscanf(f1,"%s",gy);
							int z=FindCTG(gy);
							if(z==-1){
								Log.Error( "%s,line %d : Unknown category: %s",name,Line,gy);
							}else{
								Category=z;
							};
						}else
						if(!strcmp(gx,"COMMANDCENTER")){
							CommandCenter=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CHECKWATER")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,gx);
							WaterCheckDist1=p1;
							WaterCheckDist2=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"COST_FOOD")){
						    z=Gscanf(f1,"%d",&p1);
					        if(z!=1)NEPar(name,Line,"COST_FOOD",1);
				            NeedRes[3]=p1;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CANKILL")){
					        z=Gscanf(f1,"%d",&p1);
				            if(z!=1)IncPar(name,Line,gx);
			                for(int i=0;i<p1;i++){
		                        z=Gscanf(f1,"%s",gx);
	                            if(z!=1)IncPar(name,Line,gx);
								p2=GetMatherialType(gx);
							    if(p2!=-1){
						            KillMask|=p2;
									if(!strcmp(gx,"BUILDING")){
										AttBuild=true;
									};
					            }else{
				                    Log.Error( "%s, Line: %d :CANKILL: Unknown matherial:%s",name,Line,gx);
		                        };
	                        };
							//for(i=0;i<NAttTypes;i++){
							//	AttackMask[i]=KillMask;
							//};
							NLine(f1);
							Line++;
	                    }else
						if(!strcmp(gx,"COST")){
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"COSTPERCENT")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							CostPercent=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CAPTURE")){
							Capture=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CANTCAPTURE")){
							CantCapture=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CHECKPOINTS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"CHECKPOINTS");
							NCheckPt=p1;
							CheckX=znew(char,p1);
							CheckY=znew(char,p1);
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,"CHECKPOINTS");
								CheckX[i]=p2;
								CheckY[i]=p3;
							};
							NLine(f1);
							Line++;
							//creating checking round
						    //1. center mass
					        int xc=0;
				            int yc=0;
			                for(int i=0;i<p1;i++){
		                        xc+=CheckX[i];
	                            yc+=CheckY[i];
						    };
					        xc=div(xc,p1).quot;
				            yc=div(yc,p1).quot;
			                int maxd=0;
		                    int r;
	                        for(int i=0;i<p1;i++){
							    r=Norma(int(CheckX[i])-xc,int(CheckY[i])-yc);
						        if(r>maxd)maxd=r;
					        };
				            CenterMX=xc;
			                CenterMY=yc;
		                    BRadius=maxd;
						}else
						if(!strcmp(gx,"CONCENTRATOR")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"CONCENTRATOR");
							ConcPtX.Clear();
							ConcPtY.Clear();
							ConcPtZ.Clear();
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,"CONCENTRATOR");
								ConcPtX.Add(p2*16+8);
								ConcPtY.Add(p3*16+8);
								ConcPtZ.Add(0);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CONCENTRATOR2")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"CONCENTRATOR");
							ConcPtX.Clear();
							ConcPtY.Clear();
							ConcPtZ.Clear();
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,"CONCENTRATOR");
								ConcPtX.Add(p2);
								ConcPtY.Add(p3<<1);
								ConcPtZ.Add(0);
							};
							if(!BornPtX.GetAmount()){
								for(int i=p1-1;i>=0;i--){									
									BornPtX.Add(ConcPtX[i]);
									BornPtY.Add(ConcPtY[i]);
									BornPtZ.Add(0);
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CONCENTRATOR3")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							ConcPtX.Clear();
							ConcPtY.Clear();
							ConcPtZ.Clear();
							for(int i=0;i<p1;i++){
								int p4;
								z=Gscanf(f1,"%d%d%d",&p2,&p3,&p4);
								if(z!=3)IncPar(name,Line,gx);
								ConcPtX.Add(p2);
								ConcPtY.Add(p3);
								ConcPtZ.Add(p4);
							};
							if(!BornPtX.GetAmount()){
								for(int i=p1-1;i>=0;i--){									
									BornPtX.Add(ConcPtX[i]);
									BornPtY.Add(ConcPtY[i]);
									BornPtZ.Add(ConcPtZ[i]);
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CRAFTPOINTS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							CraftPtX.Clear();
							CraftPtY.Clear();
							CraftPtZ.Clear();
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,gx);
								CraftPtX.Add(p2*16+8);
								CraftPtY.Add(p3*16+8);
								CraftPtZ.Add(0);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CRAFTPOINTS2")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							CraftPtX.Clear();
							CraftPtY.Clear();
							CraftPtZ.Clear();
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,gx);
								CraftPtX.Add(p2);
								CraftPtY.Add(p3<<1);
								CraftPtZ.Add(0);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CRAFTPOINTS3")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							CraftPtX.Clear();
							CraftPtY.Clear();
							CraftPtZ.Clear();
							for(int i=0;i<p1;i++){
								int p4;
								z=Gscanf(f1,"%d%d%d",&p2,&p3,&p4);
								if(z!=3)IncPar(name,Line,gx);
								CraftPtX.Add(p2);
								CraftPtY.Add(p3);
								CraftPtZ.Add(p4);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CANBEKILLEDINSIDE")){
							CanBeKilledInside=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CANBECAPTUREDWHENFREE")){
							CanBeCapturedWhenFree=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CANSHOOT")){
							CanShoot=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CANSTORM")){
							CanStorm=1;
							NLine(f1);
							Line++;
						}else{
							Log.Error( "%s,line %d : Unknown command : %s",name,Line,gx);
						};
						break;
					case 'D':
						
						if(!strcmp(gx,"DAMAGEONBUILDINGMULTIPLICATOR")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							DamageOnBuildingMultiplicator=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DONTROTATEONDEATH")){
							DontRotateOnDeath=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DONTANSWERONATTACK")){
							DontAnswerOnAttack=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DONTAFFECTFOGOFWAR")){
							DontAffectFogOfWar=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DONTFILLCANNON")){
							DontFillCannon=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DONTSTUCKINENEMY")){
							DontStuckInEnemy=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DONTTRANSFORMTOCHARGESTATE")){
							DontTransformToChargeState=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DIRECTFIGHT")){
							z=Gscanf(f1,"%d",&p1);
							if(z==1){
								DirectFight|=1<<p1;                                
							}							
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DETONATE")){
							z=Gscanf(f1,"%d",&p1);
							if(z==1){
								DetonationForce=p1;                                
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DIR3DBAR")){
							z=Gscanf(f1,"%d",&p1);
							if(z==1){
                                Directional3DBarsEffect=p1;                                
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DOUBLE")){
							z=Gscanf(f1,"%s",gz);
							if(z!=1)IncPar(name,Line,gx);
							{
								NewAnimation* NA=LoadNewAnimationByName(gz,0);
								if(NA){
									if((!NA->NFrames)||(NA->NFrames&1)){
										Log.Error( "%s,line %d : DOUBLE %s - Invalid amount of frames in animation (%d)!",name,Line,gz,NA->NFrames);
									};
									NA->NFrames>>=1;
									NA->DoubleAnm=1;
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DOOR")){
							z=Gscanf(f1,"%d",&p1);
							if(z==1){
								Doors=(ExRect*)malloc(sizeof ExRect+(p1-1)*16);
								Doors->NRects=0;
								for(int i=0;i<p1;i++){
									int x,y,x1,y1;
									z=Gscanf(f1,"%d%d%d%d",&x,&y,&x1,&y1);
									if(z==4){
										int i4=i<<2;
										Doors->Coor[i4  ]=x;
										Doors->Coor[i4+1]=y;
										Doors->Coor[i4+2]=x1;
										Doors->Coor[i4+3]=y1;
										Doors->NRects++;
									};
								};
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DET_RADIUS")){
							z=Gscanf(f1,"%d%d%d",&p1,&p2,&p3);
							if(z!=3)IncPar(name,Line,gx);
							if(p1>=NAttTypes){
								Log.Error( "%s,line %d : Attack type index in ATTACK_RADIUS is too big.",name,Line);
							};
							DetRadius1[p1]=p2;
							DetRadius2[p1]=p3;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DEATH_SOUND")){
							z=Gscanf(f1,"%s",gy);
							DeathSoundID=SearchStr(SoundID,gy,NSounds);
							if(DeathSoundID==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"DESTRUCT")){
			                z=Gscanf(f1,"%d%d",&p1,&p2);
		                    if(z!=2)NEPar(name,Line,"DESTRUCT",2);
	                        Destruct.WProb=p1;
							Destruct.NWeap=p2;
						    Destruct.Weap=znew(word,p2);
					        for(int i=0;i<p2;i++){
				                z=Gscanf(f1,"%s",gz);
			                    if(z!=1)NEPar(name,Line,"DESTRUCT",2+p2);
		                        p3=GetWeaponIndex(gz);
							    if(p3==-1){
									Log.Error( "%s,line %d :DESTRUCT: Unknown weapon ID: %s",name,Line,gz);
						        };
					            Destruct.Weap[i]=p3;
				            };
							NLine(f1);
							Line++;
	                    }else
						if(!strcmp(gx,"DAMPOINTS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"DAMPOINTS");
							DamPtX.Clear();
							DamPtY.Clear();
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,"DAMPOINTS");
								DamPtX.Add(p2);
								DamPtY.Add(p3<<1);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DAMAGE")){
							z=Gscanf(f1,"%d%d",&p2,&p3);
							if(z!=2)IncPar(name,Line,"DAMAGE");
							MinDamage[p2]=p3;
							MaxDamage[p2]=p3;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DAMAGEDEC")){
							z=Gscanf(f1,"%d%d",&p2,&p3);
							if(z!=2)IncPar(name,Line,"DAMAGE");
							DamageDecr[p2]=p3;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"DIRECTTRANS")){
							z=Gscanf(f1,"%d%d",&p2,&p3);
							if(z!=2)IncPar(name,Line,gx);
							TransMask[p2]|=1<<p3;
							TransMask[p3]|=1<<p2;
							NLine(f1);
							Line++;
						}else{
							Log.Error( "%s,line %d : Unknown command : %s",name,Line,gx);
						};
						break;
					case 'E':
						if(!strcmp(gx,"EMOTICON")){
							z=Gscanf(f1,"%s%s%d",gy,gz,&p1);
							if(z==3){
								OneEmotion* OE=new OneEmotion;
								OE->File=GPS.PreLoadGPImage(gz);
								OE->EmotionName=gy;
								OE->Sprite=p1;
								EmotIcons.Add(OE);
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EXTRALOCK")){
                            //adding extra lockpoints on the edge of the building
							bool ExtraLock(word& NLockPt,char*& LockX,char*& LockY);
							ExtraLock(NLockPt,LockX,LockY);
							//buildpoints
							if(BuildPtX.GetAmount()){
								int xs=0;
								int ys=0;
								for(int i=0;i<BuildPtX.GetAmount();i++){
                                    xs+=BuildPtX[i];
									ys+=BuildPtY[i];
								}
								xs/=BuildPtX.GetAmount();
								ys/=BuildPtY.GetAmount();
								for(int i=0;i<BuildPtX.GetAmount();i++){
									if(BuildPtX[i]<xs)BuildPtX[i]--;
									if(BuildPtX[i]>xs)BuildPtX[i]++;
									if(BuildPtY[i]<xs)BuildPtY[i]--;
									if(BuildPtY[i]>xs)BuildPtY[i]++;
								}
							}
                            NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EFFECT")){
							OneClassStorage* OCS=CGARB.GetClass("OneBoundEffect");
							if(OCS){
								int z=Gscanf(f1,"%s",gy);
								if(z==1){
									char  dest[512]="";
									ReadWinString(f1,dest,511);
									bool parsed=false;
									_str Log;
									OneBoundEffect* OBF=NULL;
									for(int i=0;i<OCS->Children.GetAmount()&&!parsed;i++){
										char* s=OCS->Children[i];
										if(s){
											OneClassStorage* OCS2=CGARB.GetClass(s);
											if(OCS2){
												OBF=(OneBoundEffect*)OCS2->OneMemb->new_element();
												if(OBF->Parse(dest+1,Log)){
													NewAnimation* NA=LoadNewAnimationByName(gy,0);
													if(NA&&NA->ModelID>0){
														IEffMgr->BindEffectSet(OBF->EffectFileID,NA->ModelID);
                                                        NA->Effects.Add(OBF);
													}else{
														delete(OBF);
														OBF=NULL;
													}
                                                    parsed=true;
													break;
												}
												delete(OBF);
												OBF=NULL;
												if(Log.pchar()){
													IncPar(name,Line,Log.pchar());
													parsed=true;
													break;
												}
											}
										}
									}
									if(!parsed){
										IncPar(name,Line,gx);
									}									
								}
							}
							//NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EXPERT")){
							int k,d,s;
							z=Gscanf(f1,"%d%d%d",&k,&d,&s);
							if(z!=3)IncPar(name,Line,gx);
							ExpertKills=k;
							ExpertExtraDamage=d;
							ExpertExtraShield=s;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EXPA")){
							int r;
							z=Gscanf(f1,"%d",&r);
							if(z==1){
								Expa=r;
							}
							Line++;
						}else
						if(!strcmp(gx,"EXANIMATION")){

							////EXANIMATION #anm #anm1 dx dy dz Scale dDir dOrt
							//int dx,dy,dz;
							//float Scale,dDir,dOrt;
							//char anm[64];
							//z=Gscanf(f1,"%s%s%d%d%d%f%f%f",gy,anm,&fr,&vr);
							//if(z!=4)IncPar(name,Line,gx);
							//int mod=IMM->GetModelID(anm);
							//if(mod==-1){
							//	sprintf(gz,"%s,line %d :%s: Unknown animation:%s",name,Line,gx,gy);
							//	ErrMS(gz);
							//}
							//for(int q=0;q<10;q++){
							//	NewAnimation* NAN=LoadNewAnimationByName(gy,q);
							//	if(NAN){
							//		NAN->TimeAnimationID=mod;
							//		NAN->TimeAnimationFrames=fr;
							//		NAN->TimeAnimationVariation=vr;									
							//	}
							//}
						}else
						if(!strcmp(gx,"EVROUNIT")){
							EuropeanMask=1;							
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EVROSHOT")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							EuropeanMask=p1;							
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EXITPAUSE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							ExitPause=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EXSPRITES")){
	                        z=Gscanf(f1,"%s%d%d%d%d",gy,&BRadius,&Radius1,&MotionDist,&Radius2);
		                    p1=COMPLEX.GetIndexByName(gy);
				            if(p1==-1){
					            Log.Error( "%s, line %d : %s : Unknown Sprite Object : %s",name,Line,gx,gy);
							};
							ExField=1;
					        Sprite=p1;
						    SpriteVisual=p1;
							NLine(f1);
	                        Line++;
		                }else
						if(!strcmp(gx,"EXPLRADIUS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							ExplRadius=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EXPLMEDIA")){
							z=Gscanf(f1,"%s%d",gy,&p1);
							if(z!=2)NEPar(name,Line,gx,2);
							p2=GetExMedia(gy);
							if(p2==-1){
								Log.Error( "%s , line %d : %s :Unknown media :%s",name,Line,gy);
							};
							ExplosionMedia=p2;
							EMediaRadius=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EX_SHOTS")){
					        z=Gscanf(f1,"%d",&p1);
				            if(z!=1)IncPar(name,Line,gx);
			                NShotPt=p1;
		                    ShotPtX=znew(short,p1);
	                        ShotPtY=znew(short,p1);
							ShotPtYG=znew(short,p1);
							ShotPtZ=znew(short,p1);
							ShotDelay=znew(short,p1);
							ShotAttType=znew(byte,p1);
							ShotDir=znew(byte,p1);
							ShotDiff=znew(byte,p1);
							ShotMinR=znew(word,p1);
							ShotMaxR=znew(word,p1);

		                    for(int i=0;i<p1;i++){
								//x y z minr maxr dir diff
								int x,y,z,minr,maxr,dir,diff,del,att;
							    int zz=Gscanf(f1,"%d%d%d%d%d%d%d%d%d",&x,&y,&z,&minr,&maxr,&dir,&diff,&del,&att);
						        if(zz!=9)IncPar(name,Line,gx);
					            ShotPtX[i]=x;
				                ShotPtY[i]=y;
								ShotPtYG[i]=y+z;
								ShotPtZ[i]=z;
		                        ShotMinR[i]=minr;
								ShotMaxR[i]=maxr;
								ShotDir[i]=dir;
								ShotDiff[i]=diff;
								ShotDelay[i]=del;
								ShotAttType[i]=att;
								NLine(f1);
								Line++;
		                    };
	                    }else
						if(!strcmp(gx,"EX_SHOTS2")){
					        z=Gscanf(f1,"%d",&p1);
				            if(z!=1)IncPar(name,Line,gx);
			                NShotPt=p1;
		                    ShotPtX=znew(short,p1);
	                        ShotPtY=znew(short,p1);
							ShotPtYG=znew(short,p1);
							ShotPtZ=znew(short,p1);
							ShotDelay=znew(short,p1);
							ShotAttType=znew(byte,p1);
							ShotDir=znew(byte,p1);
							ShotDiff=znew(byte,p1);
							ShotMinR=znew(word,p1);
							ShotMaxR=znew(word,p1);

		                    for(int i=0;i<p1;i++){
								//x y z minr maxr dir diff
								int x,y,z,minr,maxr,dir,diff,del,att,ygr;
							    int zz=Gscanf(f1,"%d%d%d%d%d%d%d%d%d%d",&x,&y,&z,&minr,&maxr,&dir,&diff,&del,&att,&ygr);
						        if(zz!=10)IncPar(name,Line,gx);
					            ShotPtX[i]=x;
				                ShotPtY[i]=y;
								ShotPtYG[i]=ygr;
								ShotPtZ[i]=z-y;
		                        ShotMinR[i]=minr;
								ShotMaxR[i]=maxr;
								ShotDir[i]=dir;
								ShotDiff[i]=diff;
								ShotDelay[i]=del;
								ShotAttType[i]=att;
								NLine(f1);
								Line++;
		                    };
	                    }else
						if(!strcmp(gx,"EXPLPOINTS")){
							int st;
							z=Gscanf(f1,"%d%d",&st,&p1);
							if(z!=2)IncPar(name,Line,gx);
							if(st<MaxAStages&&CompxCraft){
								CompxCraft->Stages[st].NExplPoints=p1;
								CompxCraft->Stages[st].ExplCoor=znew(short,p1*2);
								for(int i=0;i<p1;i++){
									z=Gscanf(f1,"%d%d",&p2,&p3);
									if(z!=2)IncPar(name,Line,gx);
									CompxCraft->Stages[st].ExplCoor[i+i]=p2;
									CompxCraft->Stages[st].ExplCoor[i+i+1]=p3;
								};
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"EXDAMAGE")){
							z=Gscanf(f1,"%d%d%d",&p1,&p2,&p3);
							if(z!=3)IncPar(name,Line,"EXDAMAGE");
							MinDamage[p1]=p2;
							MaxDamage[p1]=p3;
							NLine(f1);
							Line++;
						}else{
							Log.Error( "%s,line %d : Unknown command : %s",name,Line,gx);
						};
						break;
					case 'F':
						if(!strcmp(gx,"FASTCLICK_SOUND")){
							z=Gscanf(f1,"%s",gy);
							FastClickSoundID=SearchStr(SoundID,gy,NSounds);
							if(FastClickSoundID==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"FORMFACTOR")){
							int h0;
							z=Gscanf(f1,"%d",&h0);
							if(z==1){
								FormFactor=h0;
							}
							NLine(f1);
							Line++;							
						}else
						if(!strcmp(gx,"FORMDISTSCALE")){
							int h0;
							z=Gscanf(f1,"%d",&h0);
							if(z==1){
								FormationDistanceScale=h0;
							}else{
								Log.Error( "%s: Invalid FORMDISTSCALE parameters",name);
							};
							NLine(f1);
							Line++;							
						}else
						if(!strcmp(gx,"FLYPARAM")){
							int h0,h;
							z=Gscanf(f1,"%d%d",&h0,&h);
							if(z==2){
								StartFlyHeight=h0;								
								FlyHeight=h;
							}else{
								Log.Error( "%s: Invalid FLYPARAM parameters",name);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FLAG")){
							char cc[64];
							int dx,dy;
							z=Gscanf(f1,"%s%d%d",cc,&dx,&dy);
							if(z==3){
								int GetFlagIndex(char* Name);
								FlagIndex=GetFlagIndex(cc);
								FlagX=dx;
								FlagY=dy;
							}else{
								Log.Error( "%s: Invalid FLAG parameters",name);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FORMLIKESHOOTERS")){
							FormLikeShooters=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FEARRADIUS")){
							int idx,v;
							z=Gscanf(f1,"%d%d",&idx,&v);
							if(z==2&&idx<NFEARSUBJ){
								if(v>255)v=255;
								FearRadius[idx]=v;
							}else{
								Log.Error( "%s: Invalid FEARRADIUS parameters",name);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FIRELIMIT")){
							int v;
							z=Gscanf(f1,"%d",&v);
							if(z==1){
								FireLimit=v;
							};
							NLine(f1);
							Line++;
						}else
#ifdef NEWMORALE
						if(!strcmp(gx,"FEARSTART")){
							int v;
							z=Gscanf(f1,"%d",&v);
							if(z==1){
								StartMorale=v;								
							}else{
								Log.Error( "%s: Invalid FEARSTART parameters",name);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FEARSPEED")){
							int v;
							z=Gscanf(f1,"%d",&v);
							if(z==1){
								MoraleDecSpeed=v;								
							}else{
								Log.Error( "%s: Invalid FEARSTART parameters",name);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FEARFACTOR")){
							int idx,v;
							z=Gscanf(f1,"%d%d",&idx,&v);
							NLine(f1);
							Line++;
						}else
#else //NEWMORALE
						if(!strcmp(gx,"FEARFACTOR")){
							int idx,v;
							z=Gscanf(f1,"%d%d",&idx,&v);
							if(z==2&&idx<NFEARSUBJ){
								FearFactor[idx]=v;
							}else{
								Log.Error( "%s: Invalid FEARFACTOR parameters",name);
							};
							NLine(f1);
							Line++;
						}else
#endif //NEWMORALE
						if(!strcmp(gx,"FEARTYPE")){
							int idx,v;
							z=Gscanf(f1,"%d%d",&idx,&v);
							if(z==2&&idx<NAttTypes&&v<NFEARSUBJ){
								FearType[idx]=v;
							}else{
								sprintf(gz,"%s: Invalid FEARTYPE parameters",name);
						        ErrM(gz);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FRIENDLYFIRE")){
							FriendlyFire=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FULLROTATION")){
							FullRotation=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FLAGS")){
							int xc,N,x1,x2,y2,dy;
							z=Gscanf(f1,"%d%d",&xc,&N);
							if(z==2){
								FLAGS=(Flags3D*)malloc(sizeof(Flags3D)-48*2+N*8);
								FLAGS->N=N;
								FLAGS->Xr=xc;
								for(int i=0;i<N;i++){
									z=Gscanf(f1,"%d%d%d%d",&x1,&x2,&y2,&dy);
									int i3=i<<2;
									FLAGS->Points[i3]=x1;
									FLAGS->Points[i3+1]=x2;
									FLAGS->Points[i3+2]=y2;
									FLAGS->Points[i3+3]=dy;
								};
							}else{
								sprintf(gz,"%s: Invalid FLAGS parameters",name);
						           ErrM(gz);
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FARM")){
							NInFarm=UnitsPerFarm;
							Farm=true;
							NLine(f1);
							Ves=50;
							Line++;
						}else
						if(!strcmp(gx,"FORCE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							Force=p1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"FISHING")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,gx);
							FishSpeed=p1;
							FishAmount=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FOGGING")){
			                z=Gscanf(f1,"%d%d",&p1,&p2);
		                    if(z!=2)NEPar(name,Line,"FOGGING",2);
	                        Fogging.WProb=div(p1<<15,101).quot;
							Fogging.NWeap=p2;
						    Fogging.Weap=znew(word,p2);
					        for(int i=0;i<p2;i++){
				                z=Gscanf(f1,"%s",gz);
			                    if(z!=1)NEPar(name,Line,"FOGGING",2+p2);
		                        p3=GetWeaponIndex(gz);
							    if(p3==-1){
									sprintf(gy,"%s,line %d :FOGGING: Unknown weapon ID: %s",name,Line,gz);
							    	ErrM(gy);
						        };
					            Fogging.Weap[i]=p3;
				            };
							NLine(f1);
							Line++;
	                    }else
						if(!strcmp(gx,"FIRE")){
			                z=Gscanf(f1,"%d%d",&p1,&p2);
		                    if(z!=2)NEPar(name,Line,"FIRE",2);
	                        Fire.WProb=div(p1<<15,101).quot;
							Fire.NWeap=p2;
						    Fire.Weap=znew(word,p2);
					        for(int i=0;i<p2;i++){
				                z=Gscanf(f1,"%s",gz);
			                    if(z!=1)NEPar(name,Line,"FOGGING",2+p2);
		                        p3=GetWeaponIndex(gz);
							    if(p3==-1){
									sprintf(gy,"%s,line %d :FOGGING: Unknown weapon ID: %s",name,Line,gz);
							    	ErrM(gy);
						        };
					            Fire.Weap[i]=p3;
				            };
							NLine(f1);
							Line++;
	                    }else
						if(!strcmp(gx,"FIRES")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							FireX[0]=znew(short,p1);
							FireY[0]=znew(short,p1);
							NFires[0]=p1;
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,gx);
								FireX[0][i]=p2;
								FireY[0][i]=p3;
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FOUNDATION")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							NHideTri=p1;
							HideTriX=znew(short,p1);
							HideTriY=znew(short,p1);
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,gx);
								HideTriX[i]=p2+PicDx;
								HideTriY[i]=p3+PicDy;
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"FORWARD")){
							ShotForward=true;
							NLine(f1);
							Line++;
						}
						else
						if(!strcmp(gx,"FREESHOTDIST"))
						{
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							FreeShotDist=p1;
							NLine(f1);
							Line++;
						}
						else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'G':
						if(!strcmp(gx,"GLOBALCOMMANDCENTER")){
							GlobalCommandCenter=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"GATE")){
							UseLikeGate=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"GRENADE")){
#ifdef COSSACKS2
							int n1,n2;
							if(Gscanf(f1,"%d%d",&n1,&n2)==2){
								GrenadeRechargeTime=n2>>3;
								MaxGrenadesInFormation=n1;
							}else IncPar(name,Line,"GRENADE");
#endif
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"GEOMETRY")){
							z=Gscanf(f1,"%d%d%d",&p1,&p2,&p3);
							if(z!=3)IncPar(name,Line,"GEOMETRY");
							Radius1=p1<<4;
							Radius2=p2<<4;
							MotionDist=p3;
							//for(int i=0;i<256;i++){
							//	POneStepDX[i]=(TCos[i]*MotionDist)>>4;
							//	POneStepDY[i]=(TSin[i]*MotionDist)>>4;
							//	OneStepDX[i]=(TCos[i]*MotionDist)>>8;
							//	OneStepDY[i]=(TSin[i]*MotionDist)>>8;
							//};
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'H':						
						if(!strcmp(gx,"HIGHUNIT")){
							HighUnit=1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"HIGHUNITCANTENTER")){
							HighUnitCantEnter=1;
							Line++;
							NLine(f1);
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'I':
						if(!strcmp(gx,"ISOMODEL")){//ISOMODEL #anm iso_model.c2m
							z=Gscanf(f1,"%s%s",gz,gy);
							NewAnimation* NANM=LoadNewAnimationByName(gz,0);
							if(NANM){
                                NANM->ModelID_iso=IMM->GetModelID(gy);
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"INCORPOREAL")){
							Incorporeal=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"IMMORTAL")){
							Immortal=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"INVISIBLEONMINIMAP")){
							InvisibleOnMinimap=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"INVITEMASK")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							InviteMask=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"INMENUICON")){
							z=Gscanf(f1,"%s%d",gy,&p1);
							if(z==2){
								InMenuIconFile=0xFFFF;
								InMenuIconFile=GPS.PreLoadGPImage(gy);
								if(InMenuIconFile==0xFFFF){
										sprintf(gz,"%s,line %d : Icon file not exists [%s]: %s",name,Line,gx,gy);
										ErrM(gz);
									}
									InMenuIconIndex=p1;
								}
							Line++;
						}else
						if(!strcmp(gx,"IDLE")){
							z=Gscanf(f1,"%d%d%d%d",&p1,&p2,&p3,&p4);
							if(z==4){
								IdlePPos=p1+p2*12;
								IdleMPos=p3+p4*12;
							};
						}else
						if(!strcmp(gx,"ICON")){
							z=Gscanf(f1,"%s",&gy);
							if(z!=1)IncPar(name,Line,"ICON");
							p2=GetIconByName(gy);
							if(p2==-1){
								sprintf(gz,"%s,%d : Unknown Icon ID: %s",name,Line,gy);
								ErrM(gz);
							};
							IconFileID=0;
							IconID=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ICONEX")){//ICONEX dx dy FileID SpriteID
							int dx,dy,sp;
							z=Gscanf(f1,"%d%d%s%d",&dx,&dy,gy,&sp);
							if(z!=4)IncPar(name,Line,gx);
							OneSpriteExIcon* ic=new OneSpriteExIcon;
							ic->dx=dx;
							ic->dy=dy;
							ic->Step=0;
							ic->StartSpriteID=sp;
							ic->EndSpriteID=sp;
							ic->FileID=GPS.PreLoadGPImage(gy);							
							if(ic->FileID==-1){
								sprintf(gz,"%s,%d : icon not found : %s",name,Line,gy);
								ErrM(gz);
							};
							ExIcon.Icons.Add(ic);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ICONANM")){//ICONANM dx dy FileID SpriteID EndSprite Step
							int dx,dy,sp,es,st;
							z=Gscanf(f1,"%d%d%s%d%d%d",&dx,&dy,gy,&sp,&es,&st);
							if(z!=6)IncPar(name,Line,gx);
							OneSpriteExIcon* ic=new OneSpriteExIcon;
							ic->dx=dx;
							ic->dy=dy;
							ic->Step=st;
							ic->StartSpriteID=sp;
							ic->EndSpriteID=es;
							ic->FileID=GPS.PreLoadGPImage(gy);							
							if(ic->FileID==-1){
								sprintf(gz,"%s,%d : icon not found : %s",name,Line,gy);
								ErrM(gz);
							};
							ExIcon.Icons.Add(ic);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ICONSET")){//ICONSET ProbabilityValue dx dy FileID SpriteID EndSprite Step
							int ProbVal,dx,dy,sp,es,st;
							z=Gscanf(f1,"%d%d%d%s%d%d%d",&ProbVal,&dx,&dy,gy,&sp,&es,&st);
							if(z!=7)IncPar(name,Line,gx);

							OneSpriteExIconSet* Set=NULL;
							for(int i=0;i<ExIcon.Icons.GetAmount();i++){
								Set=dynamic_cast<OneSpriteExIconSet*>(ExIcon.Icons[i]);
								if(Set)
									break;
							}
							if(!Set){
								Set=new OneSpriteExIconSet;
								ExIcon.Icons.Add(Set);
							}

							OneSpriteExIcon* ic=new OneSpriteExIcon;
							ic->dx=dx;
							ic->dy=dy;
							ic->Step=st;
							ic->StartSpriteID=sp;
							ic->EndSpriteID=es;
							ic->FileID=GPS.PreLoadGPImage(gy);							
							if(ic->FileID==-1){
								sprintf(gz,"%s,%d : icon not found : %s",name,Line,gy);
								ErrM(gz);
							};

							Set->Icons.Add(ic);
							Set->Val+=ProbVal;
							Set->ProbValue.Add(ProbVal);

							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ICON3D")){//ICON3D ModelName AnimName AnimTime(ms) ExtraScale zShift StartAngle(0..1) RotationSpeed							
							OneSpriteExIcon3D* ic=new OneSpriteExIcon3D;
							z=Gscanf(f1,"%s%s%d%f%f%f%f",gy,Fn,&ic->Period,&ic->Scale,&ic->zShift,&ic->StartAngle,&ic->RotateSpeed);
							if(z!=7){
								IncPar(name,Line,gx);
								delete(ic);
							}else{
								ic->ModelID=IMM->GetModelID(gy);
								ic->AnimID=IMM->GetModelID(Fn);
								ic->RotateSpeed*=c_PI*2;
								ExIcon.Icons.Add(ic);
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"INFO")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,gx);
							InfType=p1;
							PictureID=p2;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'J':
						{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'K':
						if(!strcmp(gx,"KINETIC")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							KineticLimit=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"KILLDELAY")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"KILLDELAY");
							KillDelay=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"KIND")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"KINO");
							Kind=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"KILLERAWARD")){
							z=Gscanf(f1,"%s%d",gy,&p1);
							if(z<2)IncPar(name,Line,"KILLERAWARD");
							z2=GetResByName(gy);
							if(z2==-1){
								//char str[MAX_PATH+128];
								//sprintf(str,"%s : KILLERAWARD : Unknown resource type :%s",fn,gy);
								//ErrM(str);
							}else{
								KillerAward[z2]=p1;
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"KILLERAWARDRANDOM")){
							z=Gscanf(f1,"%s%d",gy,&p1);
							if(z<2)IncPar(name,Line,"KILLERAWARDRANDOM");
							z2=GetResByName(gy);
							if(z2==-1){
								//char str[MAX_PATH+128];
								//sprintf(str,"%s : KILLERAWARDRANDOM : Unknown resource type :%s",fn,gy);
								//ErrM(str);
							}else{
								KillerAwardRandom[z2]=p1;
							}
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'L':						
						if(!strcmp(gx,"LIKE2DONMINIMAP")){
							Like2DOnMinimap=1;
							NLine(f1);
							Line++;
						}else						
						if(!strcmp(gx,"LOWCOLLISION")){
							LowCollision=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"LONGDEATH")){
							LongDeath=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"CANNONLIKEMOTION")){
							LikeCannon=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"LOWCOSTRADIUS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							LowCostRadius=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"LINEARSORT")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							LinearLength=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"LINESORT")){
							z=Gscanf(f1,"%s",gz);
							{
								NewAnimation* NANM=LoadNewAnimationByName(gz,0);
								if(NANM){
									short* LINF=znew(short,NANM->NFrames<<2);
									NANM->LineInfo=LINF;
									int LoIdx[20];
									int NLo=0;
									int MinY=10000;
									int MinX=10000;
									int MaxX=-10000;
									for(int i=0;i<NANM->NFrames;i++){
										int x1,y1,x2,y2;
										z=Gscanf(f1,"%s",gy);
										int p=i<<2;
										if(!strcmp(gy,"LINE")){
											z=Gscanf(f1,"%d%d%d%d",&x1,&y1,&x2,&y2);
											if(z!=4){
												IncPar(name,Line,gx);
											};
											LINF[ p ]=x1;
											LINF[p+1]=y1;
											LINF[p+2]=x2;
											LINF[p+3]=y2;
											if(y1<MinY)MinY=y1;
											if(x1<MinX)MinX=x1;
											if(x1>MaxX)MaxX=x1;
											if(y2<MinY)MinY=y2;
											if(x2<MinX)MinX=x2;
											if(x2>MaxX)MaxX=x2;
										}else
										if(!strcmp(gy,"POINT")){
											z=Gscanf(f1,"%d%d",&x1,&y1);
											if(z!=2){
												IncPar(name,Line,gx);
											};
											int p=i<<2;
											LINF[ p ]=x1;
											LINF[p+1]=y1;
											LINF[p+2]=x1;
											LINF[p+3]=y1;
											if(y1<MinY)MinY=y1;
											if(x1<MinX)MinX=x1;
											if(x1>MaxX)MaxX=x1;
										}else
										if(!strcmp(gy,"GROUND")){
											LINF[p  ]=-10000;
											LINF[p+1]=-10000;
											LINF[p+2]=-10000;
											LINF[p+3]=-10000;
											if(NLo<20){
												LoIdx[NLo]=i;
												NLo++;
											};
										}else
										if(!strcmp(gy,"TOP")){
											LINF[p  ]=10000;
											LINF[p+1]=10000;
											LINF[p+2]=10000;
											LINF[p+3]=10000;
										}else{
											sprintf(gy,"%s,line %d : Unknown sort type(LINE,POINT,GROUND or TOP expected): %s",name,Line,gy);
											ErrM(gy);
										};
									};
									if(NLo){
										MinY=-10;
										int avx=(MinX+MaxX)>>1;
										for(int i=0;i<NLo;i++){
											int idx=LoIdx[i]<<2;
	#ifdef _USE3D
											if(LINF[idx  ]!=-10000)LINF[idx  ]=avx;
	#else
											LINF[idx  ]=avx;
	#endif
											LINF[idx+1]=MinY;
											LINF[idx+2]=avx;
											LINF[idx+3]=MinY;
										};
									};
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"LOCATION")){
							z=Gscanf(f1,"%d%d%d%d",&p1,&p2,&p3,&p4);
							if(z!=4)IncPar(name,Line,"LOCATION");
							PicDx=p1;
							PicDy=p2;

							PicLx=p3;
							PicLy=p4;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"LOCKPOINTS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"LOCKPOINTS");
							NLockPt=p1;
							LockX=znew(char,p1);
							LockY=znew(char,p1);
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,"LOCKPOINTS");
								LockX[i]=p2;
								LockY[i]=p3;
								};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"LIFE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"LIFE");
							Life=p1;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'M':
						if(!strcmp(gx,"MANA")){
							int mana;
							int z=Gscanf(f1,"%d",&mana);
							if(z==1){
								MaxMana=mana;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MOVEXY")){
							int A,H;
							int z=Gscanf(f1,"%s%d%d",gy,&H,&A);
							if(z==3){
								{
									NewAnimation* NANM=LoadNewAnimationByName(gy,0);
									if(NANM){
										NANM->StartDx=H;
										NANM->StartDy=A;
									}
								}
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MOTION_ON_ROAD_SOUND")){
							z=Gscanf(f1,"%s",gy);
							MotionOnRoadSound=SearchStr(SoundID,gy,NSounds);
							if(MotionOnRoadSound==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else						
						if(!strcmp(gx,"MinDistanceToEnterRoad")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							MinDistanceToEnterRoad=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MinDistForLineFormations")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							MinDistForLineFormations=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MinTopDistanceToEnterRoad")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							MinTopDistanceToEnterRoad=p1;
							NLine(f1);
							Line++;
						}else							
						if(!strcmp(gx,"MISSINSIDEUNITSDAMAGE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							MissProbabilityForInsideUnits=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MISSONHEIGHT")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)NEPar(name,Line,gx,1);
							MissProbabilityOnHeightDiff100=p1;
							MaxMissProbabilityOnHeightDiff=p2;
							NLine(f1);
							Line++;
						}else												
						if(!strcmp(gx,"MREGENERATION")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							MoraleRegenerationSpeed=p1;
							NLine(f1);
							Line++;
						}else												
						if(!strcmp(gx,"MAXATTACKERS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							MaxAttackersAmount=p1;
							NLine(f1);
							Line++;
						}else						
						if(!strcmp(gx,"MOREANIMATION")){
							float f;
							z=Gscanf(f1,"%s%s%f",gy,gz,&f);
							if(z==3){
								{
									NewAnimation* NA=LoadNewAnimationByName(gy,0);
									if(NA){
										int aid=IMM->GetModelID(gz);
										if(aid!=-1){
											NA->SecondAnimationID=aid;
											NA->DirFactor=f;
											NA->AnimationType=true;
										}else IncPar(name,Line,"unknown model id in MOREANIMATION");
									}
								}
							}else IncPar(name,Line,gx);
							Line++;
						}else
						if(!strcmp(gx,"MINICON")){
							z=Gscanf(f1,"%s%d",gy,&p1);
							if(z==2){
								MinIconFile=0xFFFF;
								MinIconFile=GPS.PreLoadGPImage(gy);
								if(MinIconFile==0xFFFF){
									sprintf(gz,"%s,line %d : Icon file not exists [%s]: %s",name,Line,gx,gy);
									ErrM(gz);
								}
								MinIconIndex=p1;
							}
							Line++;
						}else
						if(!strcmp(gx,"MOVEBREAK")){	
							z=Gscanf(f1,"%s",gx);
							if(z!=1)IncPar(name,Line,gx);
							{
								for(int i=0;i<NAttTypes;i++){
									NewAnimation* NANM=LoadNewAnimationByName(gx,i);
									if(NANM){
										NANM->MoveBreak=1;
									}
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MAXINDEPO")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)NEPar(name,Line,gx,1);
							if(p1>=MaxArtDep)InvPar(name,Line,gx,"first parameter must be<16");
							NInArtDepot=p2;
							ArtSet=p1+1;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MATHERIAL")){
		                    z=Gscanf(f1,"%d",&p1);
	                        if(z!=1)IncPar(name,Line,gx);
							for(int i=0;i<p1;i++){
						        z=Gscanf(f1,"%s",gx);
					            if(z!=1)IncPar(name,Line,gx);
				                p2=GetMatherialType(gx);
			                    if(p2!=-1){
		                            MathMask|=p2;
									//if(!strcmp(gx,"BUILDING")){
									//	AttBuild=true;
									//};
						        }else{
					                sprintf(gz,"%s, Line: %d :MATHERIAL: Unknown matherial:%s",name,Line,gy);
			                        ErrM(gz);
		                        };
				            };
	                    }else
						if(!strcmp(gx,"MATHERIAL0")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							CO_MathMask=0;
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%s",gx);
								if(z!=1)IncPar(name,Line,gx);
								p2=GetMatherialType(gx);
								if(p2!=-1){
									CO_MathMask|=p2;									
								}else{
									sprintf(gz,"%s, Line: %d :MATHERIAL0: Unknown matherial:%s",name,Line,gy);
									ErrM(gz);
								};
							};
						}else
						if(!strcmp(gx,"MESSAGE")){
							z=Gscanf(f1,"%s",gz);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MFARM")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							NInFarm=p1;
							Farm=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MULTIWEAPON2")){//MULTIWEAPON2 Scale ModelID LocatorID AngleDiff AttTypeMask
							char modid[32];
							char locid[32];
							int Mask,AngDif;
							float sc;
                            z=Gscanf(f1,"%f%s%s%d%d",&sc,modid,locid,&AngDif,&Mask);
							if(z==5){
								int mod=IMM->GetModelID(modid);
								if(mod==-1){
									sprintf(gz,"%s, Line %d :%s: Model not found: %s",name,Line,gx,modid);
									ErrM(gz);
								}
								for(int i=0;i<256;i++){
									char loc[64];
									sprintf(loc,"%s%d",locid,i);
                                    int nod=IMM->GetNodeID(mod,loc);
									if(nod!=-1){
										Matrix4D M4=IMM->GetNodeTransform(nod);
                                        ComplexFireSource* CFS=new ComplexFireSource;
										CFS->x=M4.e30*sc;
										CFS->y=M4.e31*sc;
										CFS->z=M4.e32*sc;
										word GetDir(int dx,int dy);
										CFS->Dir=GetDir(M4.e00*100,M4.e01*100);
										CFS->DirDiff=AngDif;
										CFS->AttTypeMask=Mask;
										MultiWp.Add(CFS);
									}
								}
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MULTIWEAPON")){
							ComplexFireSource* CFS=new ComplexFireSource;
							CFS->x=0;
							CFS->y=0;
							CFS->z=20;
							word GetDir(int dx,int dy);
							CFS->Dir=64;
							CFS->DirDiff=2;
							CFS->AttTypeMask=0;
							MultiWp.Add(CFS);

							CFS=new ComplexFireSource;
							CFS->x=0;
							CFS->y=0;
							CFS->z=20;
							word GetDir(int dx,int dy);
							CFS->Dir=192;
							CFS->DirDiff=2;
							CFS->AttTypeMask=0;
							MultiWp.Add(CFS);

							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MEDIA")){
							z=Gscanf(f1,"%s",gy);
							if(!strcmp(gy,"LAND"))LockType=0;
							else if(!strcmp(gy,"WATER"))LockType=1;
							else if(!strcmp(gy,"LOWAIR"))LockType=2;
							else if(!strcmp(gy,"HIAIR"))LockType=3;
							//else if(!strcmp(gy,"4"))LockType=4;
							else{
								sprintf(gz,"%s, Line %d :%s: Unknown media: %s",name,Line,gx,gy);
								ErrM(gz);
							};
							if(LockType==1)CanBeInFocusOfFormation=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"MOTIONSTYLE")){
							z=Gscanf(f1,"%s",gy);
							if(!strcmp(gy,"FASTROTATE&MOVE"))MotionStyle=0;
							else if(!strcmp(gy,"SLOWROTATE"))MotionStyle=1;
							else if(!strcmp(gy,"SHEEPS"))MotionStyle=5;
							else if(!strcmp(gy,"COMPLEXROTATE"))MotionStyle=3;
							else if(!strcmp(gy,"ROTATE&MOVE"))MotionStyle=4;
							else if(!strcmp(gy,"NEWSHEEPS"))MotionStyle=5;
							else if(!strcmp(gy,"COMPLEXOBJECT"))MotionStyle=6;
							else if(!strcmp(gy,"SINGLESTEP"))MotionStyle=7;
							else if(!strcmp(gy,"FLY"))MotionStyle=8;
							else{
								sprintf(gz,"%s, Line %d :%s: Unknown media: %s",name,Line,gx,gy);
								ErrM(gz);
							};
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'N':						
                        if(!strcmp(gx,"NOBLENDING")){
                            NoBlending=1;
                            NLine(f1);
                            Line++;
                        }else						
						if(!strcmp(gx,"NOCOLLISION")){
							NoCollision=1;
							NLine(f1);
							Line++;
						}else						
						if(!strcmp(gx,"NOALTINFO")){
							DontShowALT_Info=true;
							NLine(f1);
							Line++;
						}else						
						if(!strcmp(gx,"NOINFINITY")){
							NoInfinity=true;
							NLine(f1);
							Line++;
						}else						
						if(!strcmp(gx,"NEVERCAPTURE")){
							NeverCapture=true;
							NLine(f1);
							Line++;
						}else						
						if(!strcmp(gx,"NOTSELECTABLE")){
							NotSelectable=1;
							NLine(f1);
							Line++;
						}else						
						if(!strcmp(gx,"NOMORALE")){
							NoMorale=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"NOFARM")){
							NoFarm=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"NO25")){
							No25=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"NIKAKIXMAM")){
							NikakixMam=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"NEDORAZVITOE")){
							Nedorazvitoe=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"NAME")){
							z=Gscanf(f1,"%s",gy);
							if(z!=1)IncPar(name,Line,"NAME");
								Name=znew(char,strlen(gy)+1);
							strcpy(Name,gy);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"NO_HUNGRY")){
							NotHungry=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"NAEMNIK")){
							Behavior=2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"NOFULLDESTRUCT")){
							NoDestruct=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"NOPAUSEDATTACK")){
							z=Gscanf(f1,"%d",&p2);
							if(z!=1)IncPar(name,Line,gx);
							NoWaitMask|=1<<p2;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'O':
						if(!strcmp(gx,"OFFICER")){
							Officer=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ORDER_SOUND")){
							z=Gscanf(f1,"%s",gy);
							OrderSoundID=SearchStr(SoundID,gy,NSounds);
							if(OrderSoundID==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"OPPOSIT")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,gx);
							MinOposit=p1;
							MaxOposit=p2;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'P':
						if(!strcmp(gx,"PLACEINTRANSPORT")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							NPlacesInTransport=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"POSPOINTS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							PosPtX.Clear();
							PosPtY.Clear();
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,gx);
								PosPtX.Add(p2);
								PosPtY.Add(p3<<1);
							};
							ShowInsideUnits=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PLANEFACTOR")){
							z=Gscanf(f1,"%f",&PlaneFactor);
							if(z!=1)IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else						
						if(!strcmp(gx,"PARTICLES")){//PARTICLES #animation nodeName LinkedTo(0,1,2) Phase AverageDensity
							char anm[64];
							char node[64];
							int LinkTo;
							float Phase,Avr;
							int z=Gscanf(f1,"%s%s%d%f%f",anm,node,&LinkTo,&Phase,&Avr);
							if(z==5){
								{
									NewAnimation* NA=LoadNewAnimationByName(anm,0);
									
								}
							}else NEPar(name,Line,gx,5);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PORTBRANCH")){
							z=Gscanf(f1,"%d",&p1);
							if(z==1){
								PortBranch=p1;
							}else NEPar(name,Line,gx,1);
						}else
						if(!strcmp(gx,"PORTBACKSPRITE")){
							z=Gscanf(f1,"%d",&p1);
							if(z==1){
								PortBackSprite=p1;
							}else NEPar(name,Line,gx,1);
						}else
						if(!strcmp(gx,"PORTBACKCOLOR")){
							z=Gscanf(f1,"%d%d%d%d",&p1,&p2,&p3,&p4);
							if(z==4){
								p1&=0xFF;
								p2&=0xFF;
								p3&=0xFF;
								p4&=0xFF;
								PortBackColor=((p1&0xFF)<<24)+((p2&0xFF)<<16)+((p3&0xFF)<<8)+(p4&0xFF);
							}else NEPar(name,Line,gx,1);
						}else
						if(!strcmp(gx,"PORTIONLIMIT")){
							z=Gscanf(f1,"%d",&p1);
							if(z==1){
								PortionLimit=p1;
							}else NEPar(name,Line,gx,1);
						}else
						if(!strcmp(gx,"PIECE")){
							char pname[128];
							z=Gscanf(f1,"%s",pname);
							if(z==1){
								PieceName=(char*)malloc(strlen(pname)+1);
								strcpy(PieceName,pname);
							};
						}else
						if(!strcmp(gx,"PSIXOZ")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,"PSIXOZ",1);
							Psixoz=p1;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PAGE1")){
							z=Gscanf(f1,"%s",gy);
							if(z==1){
								int ic=GetIconByName(gy);
								Page1Icon=ic;
								strcat(gy,"_ON");
								Page1IconON=GetIconByName(gy);
								if(Page1IconON==0xFFFF)Page1IconON=Page1Icon;
							}else NEPar(name,Line,gx,1);
						}else
						if(!strcmp(gx,"PAGE2")){
							z=Gscanf(f1,"%s",gy);
							if(z==1){
								int ic=GetIconByName(gy);
								Page2Icon=ic;
								strcat(gy,"_ON");
								Page2IconON=GetIconByName(gy);
								if(Page2IconON==0xFFFF)Page2IconON=Page2Icon;
							}else NEPar(name,Line,gx,1);
						}else
						if(!strcmp(gx,"PAGE3")){
							z=Gscanf(f1,"%s",gy);
							if(z==1){
								int ic=GetIconByName(gy);
								Page3Icon=ic;
								strcat(gy,"_ON");
								Page3IconON=GetIconByName(gy);
								if(Page3IconON==0xFFFF)Page3IconON=Page3Icon;
							}else NEPar(name,Line,gx,1);
						}else
						if(!strcmp(gx,"PAGE1HINT")){
							z=Gscanf(f1,"%s",gy);
							Page1Hint=GetTextByID(gy);
							if(Page1Hint==gy)Page1Hint=NULL;
						}else
						if(!strcmp(gx,"PAGE2HINT")){
							z=Gscanf(f1,"%s",gy);
							Page2Hint=GetTextByID(gy);
							if(Page2Hint==gy)Page2Hint=NULL;
						}else
						if(!strcmp(gx,"PAGE3HINT")){
							z=Gscanf(f1,"%s",gy);
							Page3Hint=GetTextByID(gy);
							if(Page3Hint==gy)Page3Hint=NULL;
						}else
						if(!strcmp(gx,"PEASANTABSORBER")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,"PEASANTABSORBER",1);
							PeasantAbsorber=true;
							MaxInside=p1;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PEASANT")){
							Peasant=true;
		                    NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PROTECTION")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%s%d",gy,&p2);
								if(z!=2)IncPar(name,Line,gx);
								int zz2=GetWeaponType(gy);
								if(zz2==-1){
									sprintf(gz,"%s,Line %d :%s: Unknown weapon kind(see NRES.DAT):%s",name,Line,gx,gy);
									ErrM(gz);
								};
								Protection[zz2]=p2;//div(p2*255,100).quot;
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PRICE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"COST");
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%s%d",gy,&p2);
								int r=GetResByName(gy);
								if(r>=0&&r<100){
									NeedRes[r]=p2;
								}else{
									sprintf(gz,"%s,line %d:%s: Unknown resource:%s",name,Line,gx,gy);
									ErrM(gz);
								};
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PRODUCER")){
							Producer=true;
	                        z=Gscanf(f1,"%d",&p1);
				            if(z!=1)IncPar(name,Line,gx);
						    ProdType=0;
					        for(int i=0;i<p1;i++){
				                z=Gscanf(f1,"%s",gy);
			                    if(z!=1)IncPar(name,Line,gx);
		                        if(!strcmp(gy,"WOOD" ))ProdType|=1;else
							    if(!strcmp(gy,"GOLD" ))ProdType|=2;else
								if(!strcmp(gy,"STONE"))ProdType|=4;else
							    if(!strcmp(gy,"FOOD" ))ProdType|=8;else
							    if(!strcmp(gy,"IRON" ))ProdType|=16;else
							    if(!strcmp(gy,"COAL" ))ProdType|=32;else{
								    sprintf(gz,"%s,Line %d : %s : Unknown resource type : %s.\n Must be one of WOOD,STONE,GOLD,FOOD,IRON,COAL.",name,Line,gx,gy);
								    ErrM(gz);
							    };
	                        };
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,gx);
							FreeAdd=p1;
							PeasantAdd=p2;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PRIEST")){
							Priest=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PEASANTCONVERTOR")){
							PeasantConvertor=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PORT")){
							z=Gscanf(f1,"%s%d%d",gy,&p1,&p2);
							if(z!=3)IncPar(name,Line,gx);
							BuiAnm=GetNewAnimationByName(gy);
							if(!BuiAnm){
								sprintf(gz,"%s,line %d : Unknown animation: %s",name,Line,gy);
								ErrM(gz);
							};
							BuiDist=p1;
							MaxPortDist=p2;
							Port=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PORTION")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"PORTION");
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%s%d",gy,&p2);
								if(z!=2)NEPar(name,Line,"PORTION",p1);
								byte ms=0;
#ifdef COSSACKS2
								ms=GetResID(gy);
#else
								if(!strcmp(gy,"WOOD")){
									ms=0;
								}else
								if(!strcmp(gy,"GOLD")){
									ms=1;
								}else
								if(!strcmp(gy,"STONE")){
									ms=2;
								}else
								if(!strcmp(gy,"FOOD")){
									ms=3;
								}else{
									InvPar(name,Line,"PORTION",gy);
								};
#endif
								MaxResPortion[ms]=p2;
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PROMAX")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							for(int i=0;i<8;i++)PromaxPercent[i]=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PROMAX2")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,gx);
							if(p1<8&&p1>=0)PromaxPercent[p1]=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"PROMAXCTG")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							PromaxCategory=p1;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'Q':
						{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'R':
						if(!strcmp(gx,"READY_SOUND")){
							z=Gscanf(f1,"%s",gy);
							ReadySoundID=SearchStr(SoundID,gy,NSounds);
							if(ReadySoundID==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"ROUNDLOCK")){
							z=Gscanf(f1,"%d",&p1);
							if(p1>60)p1=60;
							CreateRoundLockpointsAndBuildpoints(this,p1);
							NLine(f1);
							Line++;                            
						}else
						if(!strcmp(gx,"RETREATRADIUS")){
							z=Gscanf(f1,"%d",&p1);
							RetreatRadius=p1;
							if(z!=1)IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"RPLACESPEED")){
							int rs=0;
							z=Gscanf(f1,"%d",&rs);
							if(z==1){
								NewAnimation* NA=GetAnimation(anm_RotateAtPlace);
								if(!NA){
									sprintf(gz,"%s,line %d :To use RPLACESPEED you should define #ROTATEATPLACE animation",name,Line);
									ErrM(gz);
								}else{
									NA->CanBeBrokenAfterFrame=0;									
								}
								RotationAtPlaceSpeed=rs;
								DiscreteRotationDirections=true;
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"REFLECTMODEL")){
							char s[64];
							char s1[64];
							z=Gscanf(f1,"%s%s",&s,&s1);
							if(z==2){
								int MID=IMM->GetModelID(s1);
								{
									NewAnimation* NA=LoadNewAnimationByName(s,0);
									if(NA){
										NA->ReflectionID=MID;
									}
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"RASTRATA_NA_VISTREL2")){
							z=Gscanf(f1,"%d%d%d",&p4,&p5,&p1);
							if(z!=3)IncPar(name,Line,gx);
							if(ShotRes)free(ShotRes);
							ShotRes=znew(word,p1*2);
							NShotRes=p1;
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%s%d",gy,&p2);
									if(z!=2)IncPar(name,Line,gx);
								p3=GetResByName(gy);
								if(p3>=0&&p3<100){
									ShotRes[i*2]=p3;
									ShotRes[i*2+1]=p2;
								}else{
									sprintf(gz,"%s,line %d :%s: Unknown resource:%s",name,Line,gx,gy);
									ErrM(gz);
								};
							};
							ResAttType=p4;
							ResAttType1=p5;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"RATE")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,"RATE");
							if(p1>NAttTypes){
								sprintf(gy,"%s,line %d : Attack type index in RATE must be less 4",name,Line);
								ErrM(gy);
							};
							Rate[p1]=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"RANGE")){
							z=Gscanf(f1,"%d%d%d%d%d%d",&p1,&p2,&p3,&p4,&p5,&p6);
							if(z!=6)IncPar(name,Line,gx);
							//AttRange=p1<<8;
							//MinAttRange=p2<<8;
							//MaxAttRange=p3<<8;
							//MaxNearRange=p4<<8;
							//AttType=p5;
							//VisRange=p6;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"RESSUBST")){
							ResSubst=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ROTATE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1){
								sprintf(gy,"%s, Line %d : Invalid %s value",name,Line,gx);
								ErrM(gy);
							};
							MinRotator=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"RAZBROS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							Razbros=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"RESCONSUMER")){
							z=Gscanf(f1,"%s%d",gy,&p2);
							if(z!=2)IncPar(name,Line,gx);
							p1=GetResID(gy);
							if(p1==-1){
								sprintf(gz,"%s , Line %d :%s: Unknown resource ID:%s",name,Line,gx,gy);
								ErrM(gz);
							};
							ResConsID=p1;
							ResConsumer=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"RASTRATA_NA_VISTREL")){
							z=Gscanf(f1,"%d%d",&p4,&p1);
							if(z!=2)IncPar(name,Line,gx);
							if(ShotRes)free(ShotRes);
							ShotRes=znew(word,p1*2);
							NShotRes=p1;
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%s%d",gy,&p2);
								if(z!=2)IncPar(name,Line,gx);
								p3=GetResByName(gy);
								if(p3>=0&&p3<100){
									ShotRes[i*2]=p3;
									ShotRes[i*2+1]=p2;
								}else{
									sprintf(gz,"%s,line %d :%s: Unknown resource:%s",name,Line,gx,gy);
									ErrM(gz);
								};
							};
							ResAttType=p4;
							ResAttType1=p4;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"REFLECT")){
							z=Gscanf(f1,"%s",gy);
							if(z!=1)IncPar(name,Line,gx);							
						}else
						if(!strcmp(gx,"RESOURCEBASE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"RESOURCEBASE");
							word ms=0;
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%s",gy);
								if(z!=1)NEPar(name,Line,"RESOURCEBASE",p1);
								ms|=1<<GetResByName(gy);
								ResConcentrator=ms;
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"RECTANGLE")){
							z=Gscanf(f1,"%d%d%d%d",&p1,&p2,&p3,&p4);
							if(z!=4)IncPar(name,Line,"RECTANGLE");
							RectDx=p1;
							RectDy=p2;
							RectLx=p3;
							RectLy=p4;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'S':
						if(!strcmp(gx,"SHADOWLESS")){
							for(int i=0;i<Animations->ANM.GetAmount();i++){
								if(Animations->ANM[i]->AnimationType==0){
									Animations->ANM[i]->AnimationType=3;
									Animations->ANM[i]->Scale=EngSettings.SpriteUnitsScale;
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SPLITCLUSTERS")){//SPLITCLUSTERS #anim NFrames NClusters RotationCoef SpeedCoef PlaneZpos
							int nf=0,nc=0;
							float Scale=1,RotCoef=1,SpeedCoef=1,PlaneZ=0;
							z=Gscanf(f1,"%s%d%d%f%f%f",gx,&nf,&nc,&RotCoef,&SpeedCoef,&PlaneZ);
							if(z!=6)IncPar(name,Line,gx);
							{
								for(int i=0;i<NAttTypes;i++){
									NewAnimation* NANM=LoadNewAnimationByName(gx,i);
									void SplitModel(int id,int& dst_model,int& dst_anim,int NClusters,float RotCoef,float SpeedCoef,float PlaneZPos);
									if(NANM && NANM->ModelID>0){
										int model,anim;
										SplitModel(NANM->ModelID,model,anim,nc,RotCoef,SpeedCoef,PlaneZ);
										NewAnimation* NANM2=CreateAnimation("#DEATHLIE1");
										if(NANM){
											NANM2->Enabled=true;
											NANM2->ModelID=model;
											NANM2->AnimationType=1;
											NANM2->Scale=Scale;
											NANM2->NFrames=nf; 
											NANM2->AnimationID=anim;
											NANM2->AddHeight=NANM->AddHeight;
											NANM2->AddDirection=NANM->AddDirection;
											NANM2->Scale=NANM->Scale;
											NANM2->StartDx=NANM->StartDx;
											NANM2->StartDy=NANM->StartDy;
										}
									}
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SELTYPE")){
							char st[32];
							z=Gscanf(f1,"%s%f%f",st,&selScaleX,&selScaleY);
							if(z!=3)IncPar(name,Line,gx);
							int GetSelType(char* name);
							selIndex=GetSelType(st);
							if(selIndex==-1){
								sprintf(gy,"%s,line %d : Unknown selection type: %s",name,Line,st);
								ErrM(gy);
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SELSHIFT")){
							int shift=0;
							z=Gscanf(f1,"%d",&shift);
							selShift=shift;
							if(z!=1)IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SELTYPE_BRIG")){
							char st[32];
							z=Gscanf(f1,"%s%f%f",st,&selScaleX,&selScaleY);
							if(z!=3)IncPar(name,Line,gx);
							int GetSelType(char* name);
							selIndexBR=GetSelType(st);
							if(selIndex==-1){
								sprintf(gy,"%s,line %d : Unknown selection type: %s",name,Line,st);
								ErrM(gy);
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SPEEDSCALEONTREES")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							SpeedScaleOnTrees=p1*256/100;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SHOTALWAYSON")){
							ShotAlwaysOn=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SPEEDSCALE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							SpeedScale=p1*256/100;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SELFTRANSFORM")){
							SelfTransform=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STRIKEFLYSPEED")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,gx);
							StrikeFlySpeed=p1;
							StrikeFlyMaxSpeed=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STRIKEROTATE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							StrikeRotate=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STRIKEFORCE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							StrikeForce=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SITINFORMATIONS")){
							SitInFormations=1;
							DamWeap[4]=DamWeap[1];
							AttackRadius1[4]=AttackRadius1[1];
							AttackRadius2[4]=AttackRadius2[1];
							DamageDecr[4]=DamageDecr[1];
							AttackMask[4]=0;//AttackMask[1];
							AttackPause[4]=AttackPause[1];
							MaxDamage[4]=MaxDamage[1];
							DetRadius1[4]=DetRadius1[1];
							DetRadius2[4]=DetRadius2[1];
							FearType[4]=FearType[1];
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STRIKEPROBABILITY")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							StrikeProbability=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SLOWFRAME")){//SLOWFRAME #anm start end speed
							char anm[32];
							int s,e,sp;
							z=Gscanf(f1,"%s%d%d%d",anm,&s,&e,&sp);
							if(z==4){
								{
									NewAnimation* NA=LoadNewAnimationByName(anm,0);
									if(NA){
										NA->SlowFrameStart=s;
										NA->SlowFrameEnd=e;
										NA->SlowFrameSpeed=sp;
									}
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SPECIALSEL")){
							SpecialSel=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STANDGROUNDTIME")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							StandGroundTime=p1*25/80;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SINGLESHOT")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							SingleShot|=1<<p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SHOWIDLE")){
							ShowIdlePeasants=1;
							ShowIdleMines=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SHOWIDLEPEASANTS")){
							ShowIdlePeasants=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SHOWIDLEMINES")){
							ShowIdleMines=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SHAMAN")){
							Shaman=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STANDANYWHERE")){
							StandAnywhere=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STOPDISTANCE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							StopDistance=p1;
							if(StopDistance>300)StopDistance=300;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SKILLDAMAGEMASK")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							SkillDamageMask=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SKILLDAMAGEBONUS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							SkillDamageBonus=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SKILLDAMAGEFORMBONUS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							SkillDamageFormationBonus=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SKILLDAMAGEFORMBONUSSTEP")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							SkillDamageFormationBonusStep=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SPRITEOBJECT")){
							SpriteObject=true;
				            Building=true;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SETHOTFRAME")){
							int f;
							char anm[64];
							z=Gscanf(f1,"%s%d%s",gx,&f,anm);
							if(z!=3)IncPar(name,Line,gx);
							{
								NewAnimation* NANM=LoadNewAnimationByName(gx,-1);
								if(NANM&&NANM->AnimationID>0){
									float AT=IMM->GetAnimTime(NANM->AnimationID);
                                    IMM->StartModel(NANM->ModelID);
									if(NANM->NFrames)IMM->AnimateModel(NANM->AnimationID,float(f)*AT/NANM->NFrames);
									else IMM->AnimateModel(NANM->AnimationID,0);
									int node=IMM->GetNodeID(NANM->ModelID,anm);
									if(node!=-1){
										Matrix4D M4=IMM->GetNodeTransform(node);
										if(NANM->AddDirection){
											Matrix4D M1;
											M1.rotation(Vector3D::oZ,float(NANM->AddDirection)*3.1415/128.0);
											M4*=M1;
										}
										NANM->ActiveFrame=f;
										NANM->HotRadius=int(M4.e30);
										NANM->HotHeight=int(M4.e32);;
										NANM->HotShift=int(M4.e31);
									}
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SETHOTFRAME2")){
							int f,r,h,s;
							char anm[64];
 							z=Gscanf(f1,"%s%d%d%d%d",gx,&f,&r,&h,&s);
							if(z!=5)IncPar(name,Line,gx);
							{
								NewAnimation* NANM=LoadNewAnimationByName(gx,-1);
								NANM->ActiveFrame=f;
								NANM->HotRadius=r;
								NANM->HotHeight=h;
								NANM->HotShift=s;
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SOUNDFRAME2")){
							int f;
							char anm[64];
							z=Gscanf(f1,"%s%d",gx,&f);
							if(z!=2)IncPar(name,Line,gx);
							{
								NewAnimation* NANM=LoadNewAnimationByName(gx,-1);
								NANM->HotFrame2=f;
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SETACTIVEPOINT")){
							z=Gscanf(f1,"%s%d",gx,&p1);
							if(z!=2)IncPar(name,Line,"SETACTIVEPOINT");
							{
								NewAnimation* NANM=LoadNewAnimationByName(gx,-1);
								if(NANM){
									for(int j=0;j<NANM->Rotations;j++){
										z=Gscanf(f1,"%d%d",&xx,&yy);
										if(z!=2){
											sprintf(gy,"%s,line %d : Not enough active points. Must be : %d",name,Line,NANM->Rotations);
											ErrM(gy);
										};
										NANM->ActivePt[j]->x=xx;
										NANM->ActivePt[j]->y=yy;
										NANM->ActiveFrame=p1;
									};
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SETACTIVEPOINT2")){
							z=Gscanf(f1,"%s%d",gx,&p1);
							if(z!=2)IncPar(name,Line,gx);
							{
								NewAnimation* NANM=LoadNewAnimationByName(gx,-1);
								if(NANM){
									int N=NANM->Rotations<<1;
									for(int j=0;j<N;j++){
										z=Gscanf(f1,"%d%d",&xx,&yy);
										if(z!=2){
											sprintf(gy,"%s,line %d : Not enough active points. Must be : %d",name,Line,N);
											ErrM(gy);
										};
										NANM->ActivePt[j]->x=xx;
										NANM->ActivePt[j]->y=yy;
										NANM->ActiveFrame=p1;
									};
									NANM->DoubleShot=1;
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SETACTIVEPOINT3")){
							z=Gscanf(f1,"%s%d",gx,&p1);
							if(z!=2)IncPar(name,Line,gx);
							{
								NewAnimation* NANM=LoadNewAnimationByName(gx,-1);
								if(NANM){
									int N=NANM->Rotations*3;
									for(int j=0;j<N;j++){
										z=Gscanf(f1,"%d%d",&xx,&yy);
										if(z!=2){
											sprintf(gy,"%s,line %d : Not enough active points. Must be : %d",name,Line,N);
											ErrM(gy);
										};
										NANM->ActivePt[j]->x=xx;
										NANM->ActivePt[j]->y=yy;
										NANM->ActiveFrame=p1;
									};
									NANM->DoubleShot=2;
								}
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SETACTIVEPOINT0")){
							z=Gscanf(f1,"%s%d",gx,&p1);
							if(z!=2)IncPar(name,Line,gx);
							{
								NewAnimation* NANM=LoadNewAnimationByName(gx,-1);
								if(NANM){									
									NANM->ActiveFrame=p1;
								};	
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SEARCH_ENEMY_RADIUS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							VisRange=p1<<4;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SEARCH_ENEMY_RADIUS_SHIFT")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							VisRange=(VisRange*100)/(100+p1);
							VisRangeShift=(VisRange*p1)/100;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SHIELD")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"SHIELD");
							Shield=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SHOTDELAY")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							TempDelay=p1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"SELECT_SOUND")){
							z=Gscanf(f1,"%s",gy);
							SelectSoundID=SearchStr(SoundID,gy,NSounds);
							if(SelectSoundID==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"STANDGROUND")){
							CanStandGr=1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"STRIKE_SOUND")){
							z=Gscanf(f1,"%s",gy);
							StrikeSoundID=SearchStr(SoundID,gy,NSounds);
							if(StrikeSoundID==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"SLOWDEATH")){
							SlowDeath=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SOUND")){
							z=Gscanf(f1,"%d%s%s",&p1,gy,Fn);
							if(z!=3)IncPar(name,Line,gx);
							{
								NewAnimation* NA=LoadNewAnimationByName(gy,-1);
								if(NA){
									NA->SoundID=SearchStrBack(SoundID,Fn,NSounds);
									if(NA->SoundID==-1){
										sprintf(gz,"%s,line %d :%s: Unknown sound:%s",name,Line,gx,Fn);
										ErrMS(gz);
									};
									NA->HotFrame=p1;
									NA->SoundProbability=32767;
								}
							}
						}else
						if(!strcmp(gx,"SOUND2")){
							float pr;
							z=Gscanf(f1,"%d%s%s%f",&p1,gy,Fn,&pr);
							if(z!=4)IncPar(name,Line,gx);
							{
								NewAnimation* NA=LoadNewAnimationByName(gy,-1);
								if(NA){
									NA->SoundID=SearchStr(SoundID,Fn,NSounds);
									NA->SoundProbability=pr*32767/100;
									if(NA->SoundID==-1){
										sprintf(gz,"%s,line %d :%s: Unknown sound:%s",name,Line,gx,Fn);
										ErrMS(gz);
									};
									NA->HotFrame=p1;
								}
							}
						}else
						if(!strcmp(gx,"SHOWDELAY")){
							ShowDelay=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SPRITES")){
	                        z=Gscanf(f1,"%s%s",gy,gz);
		                    p1=COMPLEX.GetIndexByName(gy);
			                p2=COMPLEX.GetIndexByName(gz);
				            if(p1==-1){
					            sprintf(Fn,"%s, line %d : %s : Unknown Sprite Object : %s",name,Line,gx,gy);
						        ErrM(Fn);
							};
	                        if(p2==-1){
		                        sprintf(Fn,"%s, line %d : %s : Unknown Sprite Object : %s",name,Line,gx,gz);
			                    ErrM(Fn);
				            };
					        Sprite=p1;
						    SpriteVisual=p2;
							NLine(f1);
	                        Line++;
		                }else
						
						if(!strcmp(gx,"SHOTS")){
	                        z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
						    NShotPt=p1;
					        ShotPtX=znew(short,p1);
				            ShotPtY=znew(short,p1);
                            ShotPtZ=znew(short,p1);
							ShotDir=0;
							ShotDir=0;
	                        for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
							    if(z!=2)IncPar(name,Line,gx);
						        ShotPtX[i]=p2;
					            ShotPtY[i]=p3;
                                ShotPtZ[i]=50;
				            };
							NLine(f1);
							Line++;
	                    }else
                        if(!strcmp(gx,"SHOTS3D")){
                            z=Gscanf(f1,"%d",&p1);
                            if(z!=1)IncPar(name,Line,gx);
                            NShotPt=p1;
                            ShotPtX=znew(short,p1);
                            ShotPtY=znew(short,p1);
                            ShotPtZ=znew(short,p1);
                            ShotDir=0;
                            ShotDir=0;
                            for(int i=0;i<p1;i++){
                                int p4;
                                z=Gscanf(f1,"%d%d%d",&p2,&p3,&p4);
                                if(z!=3)IncPar(name,Line,gx);
                                ShotPtX[i]=p2;
                                ShotPtY[i]=p3;
                                ShotPtZ[i]=p4;
                            };
                            NLine(f1);
                            Line++;
                        }else
						if(!strcmp(gx,"STAGELOCKPOINTS")){
							z=Gscanf(f1,"%d%d",&p2,&p1);
							if(z!=2)IncPar(name,Line,gx);
	
							NSLockPt[p2]=p1;
							SLockX[p2]=znew(char,p1);
							SLockY[p2]=znew(char,p1);
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p4,&p3);
								if(z!=2)IncPar(name,Line,"STAGELOCKPOINTS");
								SLockX[p2][i]=p4;
								SLockY[p2][i]=p3;
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STAGE_FIRES")){
							int st=0;
							z=Gscanf(f1,"%d%d",&st,&p1);
							if(z!=2)IncPar(name,Line,gx);
							if(CompxCraft&&st<MaxAStages){
								if(CompxCraft->Stages[st].FireX[0])free(CompxCraft->Stages[st].FireX[0]);
								if(CompxCraft->Stages[st].FireY[0])free(CompxCraft->Stages[st].FireY[0]);
								CompxCraft->Stages[st].FireX[0]=znew(short,p1);
								CompxCraft->Stages[st].FireY[0]=znew(short,p1);
								CompxCraft->Stages[st].NFires[0]=p1;
								for(int i=0;i<p1;i++){
									z=Gscanf(f1,"%d%d",&p2,&p3);
									if(z!=2)IncPar(name,Line,gx);
									CompxCraft->Stages[st].FireX[0][i]=p2;
									CompxCraft->Stages[st].FireY[0][i]=p3;
								};
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SMOKE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							FireX[1]=znew(short,p1);
							FireY[1]=znew(short,p1);
							NFires[1]=p1;
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%d%d",&p2,&p3);
								if(z!=2)IncPar(name,Line,gx);
								FireX[1][i]=p2;
								FireY[1][i]=p3;
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STAGE_SMOKE")){
							int st=0;
							z=Gscanf(f1,"%d%d",&st,&p1);
							if(z!=2)IncPar(name,Line,gx);
							if(CompxCraft&&st<MaxAStages){
								if(CompxCraft->Stages[st].FireX[1])free(CompxCraft->Stages[st].FireX[1]);
								if(CompxCraft->Stages[st].FireY[1])free(CompxCraft->Stages[st].FireY[1]);
								CompxCraft->Stages[st].FireX[1]=znew(short,p1);
								CompxCraft->Stages[st].FireY[1]=znew(short,p1);
								CompxCraft->Stages[st].NFires[1]=p1;
								for(int i=0;i<p1;i++){
									z=Gscanf(f1,"%d%d",&p2,&p3);
									if(z!=2)IncPar(name,Line,gx);
									CompxCraft->Stages[st].FireX[1][i]=p2;
									CompxCraft->Stages[st].FireY[1][i]=p3;
								};
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SETANMPARAM")){
							z=Gscanf(f1,"%d%d%d%d",&p1,&p2,&p3,&p4);
							if(z!=4)IncPar(name,Line,"SETANMPARAM");
							NAStartDx=p1;
							NAStartDy=p2;
							NAParts=p3;
							NAPartSize=p4;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"SLOWRECHARGE")){
							SlowRecharge=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"STORMFORCE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							StormForce=p1;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'T':
						if(!strcmp(gx,"TFACTOR")){
							z=Gscanf(f1,"%X",&p1);
							if(z!=1)IncPar(name,Line,gx);
							TFactor=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"TIMEANIMATION")){
							//TIMEANIMATION #anm AnimationName Frames FrameVariation
							int fr,vr;
							char anm[64];
							z=Gscanf(f1,"%s%s%d%d",gy,anm,&fr,&vr);
							if(z!=4)IncPar(name,Line,gx);
							int mod=IMM->GetModelID(anm);
							if(mod==-1){
								sprintf(gz,"%s,line %d :%s: Unknown animation:%s",name,Line,gx,gy);
								ErrMS(gz);
							}
							{
								NewAnimation* NA=LoadNewAnimationByName(gy,-1);
								if(NA){
									NA->TimeAnimationID=mod;
									NA->TimeAnimationFrames=fr;
									NA->TimeAnimationVariation=vr;									
								}
							}
						}else
						if(!strcmp(gx,"TIREDCHANGE")){
							z=Gscanf(f1,"%s%d",gy,&p1);
#ifdef GETTIRED
							if(z!=2)NEPar(name,Line,gx,1);
							{
								NewAnimation* NA=LoadNewAnimationByName(gy,-1);
								if(NA)NA->TiringChange=p1;								
							}
#endif //GETTIRED
						}else
						if(!strcmp(gx,"TRANSFORMTOMEAT")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							MeatTransformIndex=p1;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"TRANSPORT")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							Transport=true;
							MaxInside=p1;
			                NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"TORG")){
							Rinok=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"TURNOFF")){
							Behavior=1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"TWOPARTS")){
							z=Gscanf(f1,"%s%d",gy,&p1);
							if(z!=2)IncPar(name,Line,gx);							
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"TAKERESSTAGES")){
							z=Gscanf(f1,"%s%d%d%d",gy,&p1,&p2,&p3);
							if(z==4){
								int r=GetResByName(gy);
								if(r>=0&&r<8&&p1<16&&p1>=0){
									if(!CompxUnit){
										CompxUnit=new ComplexUnitRecord;
										memset(((BYTE*)CompxUnit)+4,0,sizeof ComplexUnitRecord-4);
									};
									CompxUnit->GoWithStage[p1]=p2;
									CompxUnit->TakeResStage[p1]=p3;
									CompxUnit->TransformTo[p1]=r;
									CompxUnit->CanTakeExRes=1;
								}else IncPar(name,Line,gx);
							}else IncPar(name,Line,gx);
						}else
						if(!strcmp(gx,"TIME")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							ProduceStages=p1;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'U':
						if(!strcmp(gx,"UNITSCALE")){
							z=Gscanf(f1,"%d",&p1);
							if(z>0){
								StartScale=p1;
							}else{
								Log.Error("%s,line %d:%s: Unknown StartScale",name,Line,gx);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"UPG_SOUND")){
							z=Gscanf(f1,"%s",gy);
							UpgradeSoundID=SearchStr(SoundID,gy,NSounds);
							if(UpgradeSoundID==-1){
								sprintf(gz,"%s,line %d:%s: Unknown sound:%s",name,Line,gx,gy);
							};
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"UMS_PRICE")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,"UMS_PRICE");
							for(int i=0;i<p1;i++){
								z=Gscanf(f1,"%s%d",gy,&p2);
								int r=GetResByName(gy);
								if(r>=0&&r<100){
									UMS_Res[r]=p2;
								}else{
									sprintf(gz,"%s,line %d:%s: Unknown resource:%s",name,Line,gx,gy);
									ErrM(gz);
								};
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"UNBEATABLEWHENFREE")){
							UnbeatableWhenFree=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"UNITABSORBER")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,"UNITABSORBER",1);
							UnitAbsorber=true;
							MaxInside=p1;
					        NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"USERLC")){
							z=Gscanf(f1,"%d%s%s%d%d",&p1,gy,gz,&p2,&p3);
							if(z!=5)IncPar(name,Line,"USERLC");
							if(p1>MaxRLC)MaxRLC=p1;
							UpConv(gy);
							//search for RLC file in global table
							int nr=GPS.PreLoadGPImage(gy);
							RLCRef[p1]=nr;
							RLCdx[p1]=p2;
							RLCdy[p1]=p3;
							RLCParts[p1]=1;
							RLCPartSize[p1]=0;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"USERLCEXT")){
							z=Gscanf(f1,"%d%d%d%s%s%d%d",&p1,&p4,&p5,gy,gz,&p2,&p3);
							if(z!=7)IncPar(name,Line,"USERLCEXT");
							if(p1>MaxRLC)MaxRLC=p1;
							UpConv(gy);
							//search for RLC file in global table
							int nr=GPS.PreLoadGPImage(gy);
							RLCRef[p1]=nr;
							RLCdx[p1]=p2;
							RLCdy[p1]=p3;
								RLCParts[p1]=p4;
							RLCPartSize[p2]=p5;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"UNITKIND")){
							z=Gscanf(f1,"%s",gz);
							if(z!=1)IncPar(name,Line,gx);
							p1=GetUnitKind(gz);
							if(p1==-1){
								sprintf("%s, Line %d :%s: Unknown kind of unit(see [UNITKIND],NRES.DAT):%s",name,Line,gx,gz);
								ErrM(gy);
							};
							Kind=p1;
						}else
						if(!strcmp(gx,"UNITRADIUS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							UnitRadius=p1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"USAGE")){
							z=Gscanf(f1,"%s",&gy);
							if(z!=1)IncPar(name,Line,gx);
							if(!strcmp(gy,FarmIDS))Usage=FarmID;else
							if(!strcmp(gy,MelnicaIDS)){
								Usage=MelnicaID;
								Ves=100;
							}else
							if(!strcmp(gy,SkladIDS)){
								Usage=SkladID;
								Ves=30;
							}else
							if(!strcmp(gy,TowerIDS))Usage=TowerID;else
							if(!strcmp(gy,CenterIDS))Usage=CenterID;else
							if(!strcmp(gy,MineIDS))Usage=MineID;else
							if(!strcmp(gy,FieldIDS))Usage=FieldID;else
							if(!strcmp(gy,PeasantIDS))Usage=PeasantID;else
							if(!strcmp(gy,FastHorseIDS))Usage=FastHorseID;else
							if(!strcmp(gy,MortiraIDS)){
								Usage=MortiraID;
								Artilery=true;
								//AttBuild=true;
							}else
							if(!strcmp(gy,PushkaIDS)){
								Usage=PushkaID;
								Artilery=true;
								//AttBuild=true;
							}else
							if(!strcmp(gy,MultiCannonIDS)){
								Usage=MultiCannonID;
								Artilery=true;
								//AttBuild=true;
							}else
							if(!strcmp(gy,GrenaderIDS))Usage=GrenaderID;else
							if(!strcmp(gy,HardWallIDS))Usage=HardWallID;else
							if(!strcmp(gy,WeakWallIDS))Usage=WeakWallID;else
							if(!strcmp(gy,LinkorIDS))Usage=LinkorID;else
							if(!strcmp(gy,WeakIDS))Usage=WeakID;else
							if(!strcmp(gy,FisherIDS))Usage=FisherID;else
							if(!strcmp(gy,ArtDepoIDS))Usage=ArtDepoID;else
							if(!strcmp(gy,SupMortIDS)){
								Usage=SupMortID;
								Artilery=true;
								//AttBuild=true;
							}else
							if(!strcmp(gy,PortIDS))Usage=PortID;else
							if(!strcmp(gy,LightInfIDS))Usage=LightInfID;else
							if(!strcmp(gy,StrelokIDS))Usage=StrelokID;else
							if(!strcmp(gy,HardHorceIDS))Usage=HardHorceID;else
							if(!strcmp(gy,HorseStrelokIDS))Usage=HorseStrelokID;else
							if(!strcmp(gy,FregatIDS))Usage=FregatID;else
							if(!strcmp(gy,GaleraIDS))Usage=GaleraID;else
							if(!strcmp(gy,IaxtaIDS))Usage=IaxtaID;else
							if(!strcmp(gy,ShebekaIDS))Usage=ShebekaID;else
							if(!strcmp(gy,ParomIDS))Usage=ParomID;else 
							if(!strcmp(gy,ArcherIDS))Usage=ArcherID;else
							if(!strcmp(gy,DiplomatIDS))Usage=DiplomatID;else
							if(!strcmp(gy,MentIDS))Usage=MentID;else 
							if(!strcmp(gy,EgerIDS))Usage=EgerID;else
							if(!strcmp(gy,CostlyIDS))Usage=CostlyID;
							else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"USETRANSX")){
							z=Gscanf(f1,"%d",&p2);
							if(z!=1)IncPar(name,Line,gx);
							TransXMask|=1<<p2;
							NLine(f1);
							Line++;
						}else{
							Log.Error( "%s,line %d : Unknown command : %s",name,Line,gx );
						};
						break;
					case 'V':
						if(!strcmp(gx,"VISITORMASK")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)NEPar(name,Line,gx,1);
							VisitorMask=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"VETERAN")){
							int k,d,s;
							z=Gscanf(f1,"%d%d%d",&k,&d,&s);
							if(z!=3)IncPar(name,Line,gx);
                            VeteranKills=k;
							VeteranExtraDamage=d;
							VeteranExtraShield=s;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"VIT_UNIT_INTERFACE")){
							z=Gscanf(f1,"%s",gy);
							if(z!=1)IncPar(name,Line,gx);
							VUI=vui_Action::VUI->Get(gy);
							//word GetVUIid(char* _Name);
							//VUI=GetVUIid(gy);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"VES")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							Ves=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"VESACCEL")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,gx);
							VesStart=p1;
							VesFin=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"VISION")){
							z=Gscanf(f1,"%d",&p1);
							if(p1<0){
								sprintf(gy,"%s, Line %d :%s: Argument must be 0..8",name,Line,gx);
								ErrM(gy);
							};
							if(z!=1)IncPar(name,Line,gx);
							VisionType=p1;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"VESLA")){
							z=Gscanf(f1,"%d%d%s%d",&p1,&p2,gy,&p3);
							if(z!=4)IncPar(name,Line,gx);							
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'W':
						if(!strcmp(gx,"WEAPONINSECTOR")){
							WSECT=(WeaponInSector*)malloc(sizeof WeaponInSector);
							z=Gscanf(f1,"%d%d%d%d%d%d%d",&WSECT->AttIndex,&WSECT->RMin,&WSECT->RMax,&WSECT->MaxDamage,&WSECT->MinDamage,&WSECT->Angle,&WSECT->AnglFactor);
							if(z!=7)IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"WATERCHECKDIST")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,gx);
							WaterCheckDist1=p1;
							WaterCheckDist2=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"WALLOBJECT")){
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"WEAPON")){
							z=Gscanf(f1,"%d%s",&p1,gz);
							if(z!=2)IncPar(name,Line,"WEAPON");
							if(p1>=NAttTypes){
								sprintf(gy,"%s,line %d :WEAPON: Index too big : %d",name,Line,p1);
								ErrM(gy);
							};
							/*
							Weapon* GetWeaponWithModificator(char* Name);
							Weapon* W = GetWeaponWithModificator(gz);
							if(W)
							{
								DamWeap[p1]=W;
							}
							else
							{
								p2=GetWeaponIndex(gz);
								if(p2==-1){
									sprintf(gy,"%s,line %d :WEAPON: Unknown weapon ID: %s",name,Line,gz);
									ErrM(gy);
								};
								DamWeap[p1]=WPLIST[p2];
							}
							*/
							p2=GetWeaponIndex(gz);
							if(p2!=-1){
								DamWeap[p1]=WPLIST[p2];
								Log.Error( "Weapon Weapon.nds style detected: %s. In unit %s", gz, name );
								//sprintf(gy,"%s,line %d :WEAPON: Unknown weapon ID: %s",name,Line,gz);
								//ErrM(gy);
							}
							else
							{
								Weapon* GetWeaponWithModificator(char* Name);
								Weapon* W = GetWeaponWithModificator(gz);
								DamWeap[p1]=W;
							}
							
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"WAVES")){
							z=Gscanf(f1,"%d%d%d%d",&p5,&p6,&p1,&p2);
							if(z!=4)IncPar(name,Line,gx);
							WaveDZ=p1;
							NWaves=p2;
							WavePoints=znew(short,p2*2);
							for(int i=0;i<p2;i++){
								z=Gscanf(f1,"%d%d",&p3,&p4);
								if(z!=2)IncPar(name,Line,gx);
								WavePoints[i+i]=p3-p5;
								WavePoints[i+i+1]=(p4-p6-p1)<<1;
							};
							NLine(f1);
							Line++;
						}else
							if(!strcmp(gx,"WATERROUND")){
							WaterActive=true;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"WEAPONKIND")){
							z=Gscanf(f1,"%d%s",&p1,gy);
							if(z!=2)IncPar(name,Line,gx);
							if(p1>=NAttTypes){
								sprintf(gz,"%s,Line %d :%s: Too big attack index",name,Line,gx);
								ErrM(gz);
							};
							int zz2=GetWeaponType(gy);
							if(zz2==-1){
								sprintf(gz,"%s,Line %d :%s: Unknown weapon kind(see NRES.DAT):%s",name,Line,gx,gy);
								ErrM(gz);
							};
							WeaponKind[p1]=zz2;
							if(WeaponFlags[zz2]&(8+16))CanFire=1;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'X':
						{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'Y':
						{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					case 'Z':
						if(!strcmp(gx,"ZPOINTS")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,"ZPOINTS");
							SrcZPoint=p1;
							DstZPoint=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ZALP")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							if(p1>255)p1=255;
							MaxZalp=p1;
							NLine(f1);
							Line++;
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
						break;
					default:
						if(!strcmp(gx,"3DBARS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							NBars=p1;
							Bars3D=znew(short,p1*5);
							int q=0;
							for(int i=0;i<p1;i++){
								int XB,YB,L1,L2,Hi;
								z=Gscanf(f1,"%d %d %d %d %d",&XB,&YB,&L1,&L2,&Hi);
								if(z!=5)IncPar(name,Line,gx);
								Bars3D[q  ]=XB;
								Bars3D[q+1]=YB;
								Bars3D[q+2]=L1;
								Bars3D[q+3]=L2;
								Bars3D[q+4]=Hi;
								q+=5;
							};
							Line++;
							NLine(f1);
						}else{
							sprintf(gy,"%s,line %d : Unknown command : %s",name,Line,gx);
							ErrM(gy);
						};
					};
				};
			}else{
				if(!strcmp(gx,"[END]"))finish=true;
				NLine(f1);
				Line++;
			};
		}while((!finish)&&z!=-1);
		Gclose(f1);
		for(int p=0;p<10;p++){
			NewAnimation* NA=Animations->Get(anm_MotionL,p);
			if(NA)NA->CanBeBrokenAfterFrame=0;//1024
			NA=Animations->Get(anm_MotionR,p);
			if(NA)NA->CanBeBrokenAfterFrame=0;//1024
			for(int i=0;i<NAttTypes;i++){
				NA=Animations->Get(anm_PMotionL+i,p);
				if(NA)NA->CanBeBrokenAfterFrame=0;			
			}
		}		
	}else{
		sprintf(gx,"Could not open new monster discription file %s.nm",name);
		Log.Warning(gx);
		return false;
	};	
	if(Expa==0)
	{
		void SetUnitExperience(NewMonster* NM);
		SetUnitExperience(this);
	}
	if(Ves==0)
	{
		Ves=Expa;
	}
	HaveRotateAnimations=GetAnimation(anm_RotateL)->Enabled;
	for(int i=0;i<10;i++){
		NewAnimation* NA=Animations->Get(anm_Rest,i);
		if(NA){
			NA->MoveBreak=true;
			NA->CanBeBrokenAfterFrame=0;
		}
		NA=Animations->Get(anm_RestA1,i);
		if(NA){
			NA->MoveBreak=true;
			NA->CanBeBrokenAfterFrame=0;
		}
	}
	if(UnitsColumnIndex==0xFF){
		int R=max(AttackRadius2[0],AttackRadius2[1]);
		if(CollisionRadius<20){
			if(R>150){
                UnitsColumnIndex=1;
			}else{
				UnitsColumnIndex=0;
			}
		}else{
			UnitsColumnIndex=2;
		}
	}
	return true;
};
extern bool ProtectionMode;
void ClearMonNames(){
	for(int i=0;i<NNewMon;i++)free(MonNames[i]);
};

int fl_compare( const void *arg1, const void *arg2 ){
	_str* s1=(_str*)(*(DWORD*)arg1);
	_str* s2=(_str*)(*(DWORD*)arg2);
	return stricmp(s1->pchar(),s2->pchar());
}

void CreateSortedFileList(const char* Directory,const char* Mask,FileList& result,bool subdir){
    char x[512];
    sprintf(x,"%s%s",Directory,Mask);
    TGSCFindInfo* FD=GSFILES.gFindFirst(x);
    bool havedir=false;
    while(FD){
        if( FD->m_FileName[0]!='%' && FD->m_FileName[0]!='.' ){
            bool dir=strchr(FD->m_FileName,'\\');
            if(dir==false || dir==subdir){
                _str* s=new _str;
                *s=FD->m_FileName;
                result.Add(s);
                if(dir)havedir=dir;
            }
        }        
        if(!GSFILES.gFindNext(FD))break;
    }
    if(subdir && !havedir){
        WIN32_FIND_DATA FD;
        char x[512];
        sprintf(x,"%s*.*",Directory);
        HANDLE H=FindFirstFile(x,&FD);
        if(H!=INVALID_HANDLE_VALUE){
            do{		
                if(FD.cFileName[0]!='%' && FD.cFileName[0]!='.' && (FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
                    sprintf(x,"%s%s\\",Directory,FD.cFileName);
                    FileList tmp;
                    CreateSortedFileList(x,Mask,tmp,subdir);
                    for(int j=0;j<tmp.GetAmount();j++){
                        sprintf(x,"%s\\%s",FD.cFileName,tmp[j]->pchar());
                        _str* s=new _str;
                        *s=x;
                        result.Add(s);
                    }						
                }
                if(!FindNextFile(H,&FD))break;
            }while(true);
            FindClose(H);
        }
    }
    //sorting
    qsort(result.GetValues(),result.GetAmount(),4,fl_compare);
	//deleting repeated files
	for(int i=1;i<result.GetAmount();i++){
		if(!stricmp(result[i]->pchar(),result[i-1]->pchar())){
			result.DelElement(i);
			i--;
		}
	}
    /*
    WIN32_FIND_DATA FD;
	char x[512];
	sprintf(x,"%s%s",Directory,Mask);
	HANDLE H=FindFirstFile(x,&FD);
	if(H!=INVALID_HANDLE_VALUE){
		do{	
			if( FD.cFileName[0]!='%' && FD.cFileName[0]!='.' ){
				_str* s=new _str;
				*s=FD.cFileName;
				result.Add(s);
			}
			if(!FindNextFile(H,&FD))break;
		}while(true);
		FindClose(H);
		if(subdir){
			sprintf(x,"%s*.*",Directory);
			HANDLE H=FindFirstFile(x,&FD);
			if(H!=INVALID_HANDLE_VALUE){
				do{		
					if(FD.cFileName[0]!='%' && FD.cFileName[0]!='.' && (FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
						sprintf(x,"%s%s\\",Directory,FD.cFileName);
						FileList tmp;
						CreateSortedFileList(x,Mask,tmp,subdir);
						for(int j=0;j<tmp.GetAmount();j++){
							sprintf(x,"%s\\%s",FD.cFileName,tmp[j]->pchar());
							_str* s=new _str;
							*s=x;
							result.Add(s);
						}						
					}
					if(!FindNextFile(H,&FD))break;
				}while(true);
				FindClose(H);
			}
		}
		//sorting
		qsort(result.GetValues(),result.GetAmount(),4,fl_compare);
	}
    */
}
void LoadAllNewMonsters(){
	NNewMon=0;
	ProtectionMode=1;
	GFILE* f1=Gopen("nmlist.txt","r");
	ProtectionMode=0;	
	int NM=0;
	if(f1){
		char mname[128];
		int z;
		do{
			mname[0]=0;
			z=Gscanf(f1,"%s",mname);
			if(mname[0])NM++;
		}while(z!=-1&&mname[0]);
		Gclose(f1);
	}
	ProtectionMode=1;
	f1=Gopen("nmlist.txt","r");
	ProtectionMode=0;	
	int pp=0;
	if(f1){
		char mname[128];
		int z;
		do{
			mname[0]=0;
			z=Gscanf(f1,"%s",mname);
			ShowLoadProgress("Monsters",pp,NM);
			if(mname[0]){
				pp++;
				//memset(&NewMon[NNewMon],0,sizeof NewMonster);
				NewMon[NNewMon]=new NewMonster;
				NewMon[NNewMon]->MyIndex=NNewMon;
				if(NewMon[NNewMon]->CreateFromFile(mname)){
					if(!NewMon[NNewMon]->Name){
						char cc[256];
						sprintf(cc,"Name for %s not specified!",mname);
						ErrM(cc);
					}
					NewMon[NNewMon]->MyIndex=NNewMon;
					NewMon[NNewMon]->CharacterWasModified=false;
					MonNames[NNewMon]=znew(char,strlen(NewMon[NNewMon]->Name)+1);
					strcpy(MonNames[NNewMon],NewMon[NNewMon]->Name);
					NNewMon++;
				};
			};
		}while(z!=-1&&mname[0]);
		Gclose(f1);
	}else{
		//ErrM("Could not open NMLIST.TXT");
	};
	FileList res;
	FileList Cres; //Trying to Load Camp Monsters from 1.00
	CreateSortedFileList("CAMP\\(units)\\","*.md",Cres,true);
	CreateSortedFileList("(units)\\","*.md",res,true);

	char mname[128];
	int z;
	for(int i=0;i<res.GetAmount();i++){
		sprintf(mname,"(units)\\%s",res[i]->pchar());		
		mname[strlen(mname)-3]=0;
		ShowLoadProgress("Monsters",i,res.GetAmount());
		//memset(&NewMon[NNewMon],0,sizeof NewMonster);
		NewMon[NNewMon]=new NewMonster;
		pp++;
		NewMon[NNewMon]->MyIndex=NNewMon;
		if(NewMon[NNewMon]->CreateFromFile(mname)){
			//if(!NewMon[NNewMon].Name){
			//	char cc[256];
			//	sprintf(cc,"Name for %s not specified!",mname);
			//	ErrM(cc);
			//}
			NewMon[NNewMon]->MyIndex=NNewMon;
			NewMon[NNewMon]->CharacterWasModified=false;
			MonNames[NNewMon]=znew(char,strlen(NewMon[NNewMon]->Name)+1);
			strcpy(MonNames[NNewMon],NewMon[NNewMon]->Name);
			NNewMon++;
		};
	};
};
bool CreateGOByName(GeneralObject* GO,char* name,char* newName){
	Visuals* VS=(Visuals*)GO;
	memset(GO,0,sizeof Visuals);
	for(int i=0;i<NNewMon;i++){
		if(i==NNewMon-1 || !strcmp(MonNames[i],name)){
			if(strcmp(MonNames[i],name)){
				char ddd[128];
				Log.Warning("Could not create GeneralObject by name: %s",name);
			}
			NewMonster* NM=NewMon[i];
			InitFlags(GO);
			VS->newMons=NM;
			VS->newMons=NM;
			for(int j=1;j<4;j++){
				GO->ResourceID[j]=255;
				GO->ResAmount[j]=0;
			};
			GO->Message=NM->Message;
			GO->MonsterID=znew(char,strlen(newName)+1);
			strcpy(GO->MonsterID,newName);
			GO->NStages=(NM->Time);
			GO->cost=byte(NM->Res1cost);
			GO->Kind=byte(NM->Kind);
			GO->ResourceID[0]=1;
			GO->ResAmount[0]=NM->Res1cost;
			GO->ManualDisable=0;
			if(NM->Res2cost){
				GO->ResourceID[1]=2;
				GO->ResAmount[1]=NM->Res2cost;
			};
			if(NM->Res3cost){
				GO->ResourceID[2]=3;
				GO->ResAmount[2]=NM->Res3cost;
			};
			GO->NStages=(NM->ProduceStages);
			GO->MoreCharacter=new AdvCharacter;
			GO->CondEnabled=false;
			GO->Enabled=false;
			GO->Branch=0xFF;

			memcpy(GO->NeedRes,NM->NeedRes,sizeof GO->NeedRes);
			GO->CostPercent=NM->CostPercent;
			GO->CostGrowsOnlyForReadyBuildings=0;

			CreateAdvCharacter(GO->MoreCharacter,GO->newMons);
			return true;
		};
	};	
	return false;
};
//----------------Elements of the terrain: trees,stones,...-----------
int  SprGroup::GetIndexByName(char* Name){
    if(!strcmp(Name,"NONE"))return 0xFFFF;
	for(int i=0;i<GetNSprites();i++){
		if(Objects[i]->Name.pchar()&&!strcmp(Objects[i]->Name.pchar(),Name))return i;
	};
	return -1;
};
int GetIndexByName2(char* Name,int NSpr,ObjCharacter** ObjChar){	
	if(!strcmp(Name,"NONE"))return 0xFFFF;
	for(int i=0;i<NSpr;i++){
		if(ObjChar[i]->Name.pchar()&&!strcmp(ObjChar[i]->Name.pchar(),Name))return i;
	};
	return -1;
};
#define GetIndexByNameOLD(Name)GetIndexByName2(Name,NSpr,ObjChar)
	
int GetResByName(char* gy){
	if(gy[0]=='#'&&gy[1]=='R'){
		gy+=2;
		int v;
		int z=sscanf(gy,"%d",&v);
		if(z==1){
			return v;
		};
	};
    if(!strcmp(gy,"WOOD"))return 0;
	else if(!strcmp(gy,"GOLD"))return 1;
	else if(!strcmp(gy,"STONE"))return 2;
	else if(!strcmp(gy,"FOOD"))return 3;
    else if(!strcmp(gy,"IRON"))return 4;
    else if(!strcmp(gy,"COAL"))return 5;
    else if(!strcmp(gy,"NONE"))return 0xFE;
    else if(!strcmp(gy,"REMOVE"))return 0xFF;
    else return -1;
};
int AddOneGroundType(SprGroup* GRP,int id);
#define xnew(s,t) (t*)malloc((s)*sizeof(t))
void SetSprDefaults(ObjCharacter* OC){
	OC->ModelManagerID=-1;
	OC->FixHeight=-1000;
	OC->AutoDeep=-1000;
	OC->AutoDeepR=0;
	OC->AutoDeepR1=100;
	OC->Amplitude=0;
	OC->WNextObj=0xFFFF;
	OC->TNextObj=0xFFFF;
}
void SprGroup::LoadSprites(char* fname){
	char fn[128];
	sprintf(fn,"%s.lst",fname);
	GFILE* f1=Gopen(fn,"r");
	AFile(fn);
	char str[128];
	char gy[128];
	char gx[128];
	int z,nl,dx,dy,rr,z1,z2,parm1;
    int Line=1;
    int p4,p5;

	reset_class(this);

	int NSpr=0;
	word FileID=0;
	word* Items=NULL;
	short* Dx=NULL;
	short* Dy=NULL;
	short* Radius=NULL;
	ObjCharacter** ObjChar=NULL;
	int NSections=0;
	char** SectNames=NULL;
	int** SectContence=NULL;
	int* NInSect=NULL;

	if(f1){
		z=Gscanf(f1,"%s%d",gy,&nl);
		if(z!=2)IncPar(fname,0,"?");
		NSpr=nl;
		Items=xnew(nl,word);
		for(int i=0;i<nl;i++)Items[i]=i;
		Dx=xnew(nl,short);
		Dy=xnew(nl,short);				
		Radius=xnew(nl,short);				
		ObjChar=xnew(nl,ObjCharacter*);		
		for(int q=0;q<nl;q++){
			ObjChar[q]=new ObjCharacter;
		}		
		NLine(f1);
        Line++;
		UpConv(gy);
        if(!strcmp(gy,"COMPLEX")){
            //Loading complex
            FileID=0xFFFF;
            for(int i=0;i<nl;i++){
                z=Gscanf(f1,"%s",gx);
                if(gx[0]!=0&&gx[0]!='/'){
                    z=Gscanf(f1,"%d%d%d%s%d%d%d%d",&dx,&dy,&rr,str,&p4,&p5,&z1,&z2);
                    if(z!=8)IncPar(fname,Line,gy);
                    Dx[i]=dx;
	        		Dy[i]=dy;
	    	    	Radius[i]=rr;
		    	    ObjCharacter* OBC=ObjChar[i];
    		    	OBC->Name=gx;
		        	OBC->ResType=0xFF;
					OBC->WorkRadius=32;
		        	OBC->WorkAmount=0;
					OBC->ResPerWork=0;
		    	    OBC->WNextObj=i;
    		    	OBC->DNextObj=i;
	    	    	OBC->DamageAmount=0;
                    OBC->IntResType=0xFF;
                    OBC->IntResPerWork=0;
                    OBC->IntWorkRadius=32;
                    OBC->TimeAmount=0;
                    OBC->TNextObj=i;
                    OBC->Parts=p4;
                    if(!p5)p5=1;
                    OBC->Delay=p5;
                    OBC->Z0=z1;
					OBC->OnGround=0xFFFF;
                    OBC->DZ=z2;
                    OBC->Stand=GetNewAnimationByName(str);
					SetSprDefaults(OBC);
                    char ggg[128];
                    sprintf(ggg,"%sS",str);
                    OBC->Shadow=GetNewAnimationByName(ggg);
                    if(OBC->Stand){
                        OBC->Frames=div(OBC->Stand->NFrames,p4).quot;
                        if(!OBC->Frames){
                            sprintf(gx,"%s, Line %d : %s : Too many parts (more then sprites in animation) : %s",fname,Line,gy,str);
                            ErrM(gx);
                        };
                    }else{
                        sprintf(gx,"%s, Line %d : %s : Unknown Animation : %s",fname,Line,gy,str);
                        ErrM(gx);
                    };
                    NLine(f1);
                    Line++;
                }else{
                    NLine(f1);
                    Line++;
                    i--;
                };
            };  
        }else{
    		//search for RLC file in global table
    		int nr=GPS.PreLoadGPImage(gy);
			//if(!strcmp(fname,"animations"))GPS.SetWhiteFont(nr);
    		FileID=nr;
			int spidx=0;
			int gpidx=0;
	    	for(int i=0;i<nl;i++){
                z=Gscanf(f1,"%s",gx);
                if(gx[0]!=0&&gx[0]!='/'){
					if(gx[0]=='#'){
						int N1=0;
						int N2=0;
						int z=Gscanf(f1,"%d%d",&N1,&N2);
						if(z==2){
							char cc[64];
							z=Gscanf(f1,"%s",cc);
							if(z==1){
								z=Gscanf(f1,"%d%d%d",&dx,&dy,&rr);
								if(z==3){
									if(cc[0]=='@'){
										int nnew=NSpr+2*(N2-N1+1)-1;
										Items=(word*)realloc(Items,nnew*2);
										Items[NSpr]=i;
										Dx=(short*)realloc(Dx,nnew*2);
										Dy=(short*)realloc(Dy,nnew*2);
										Radius=(short*)realloc(Radius,nnew*2);				
										ObjChar=(ObjCharacter**)realloc(ObjChar,nnew*4);
										memset(ObjChar+NSpr,0,(nnew-NSpr)*4);
										for(int q=0;q<(nnew-NSpr);q++){
											ObjChar[q+NSpr]=new ObjCharacter;
										}
										NSpr=nnew;
										for(int j=N1;j<=N2;j++){
											Dx[spidx]=dx;
		        							Dy[spidx]=dy;
											Radius[spidx]=rr;
											ObjCharacter* OBCF=ObjChar[spidx];
											OBCF->SpriteIndex=gpidx;
											char cc3[64];
											sprintf(cc3,"%s%dF",cc,j);
		 		    						OBCF->Name=cc3;
			        						OBCF->ResType=0xFF;
			        						OBCF->WorkAmount=0;
											OBCF->WorkRadius=32;
											OBCF->ResPerWork=0;
			    	    					OBCF->WNextObj=i;
				    						OBCF->DNextObj=i;
		 		    						OBCF->DamageAmount=0;
			            					OBCF->IntResType=0xFF;
			            					OBCF->IntResPerWork=0;
				        					OBCF->IntWorkRadius=32;
				        					OBCF->TimeAmount=0;
				        					OBCF->TNextObj=i;
											OBCF->Parts=1;
											OBCF->Z0=0;
											OBCF->DZ=0;
											OBCF->Frames=1;
											OBCF->Stand=NULL;
											OBCF->Discret=1;
											OBCF->OnGround=0xFFFF;
											SetSprDefaults(OBCF);
											spidx++;
											gpidx++;
										};
										gpidx-=N2-N1+1;
										for(int j=N1;j<=N2;j++){
											Dx[spidx]=dx;
		        							Dy[spidx]=dy;
											Radius[spidx]=rr;
											ObjCharacter* OBCF=ObjChar[spidx];
											OBCF->SpriteIndex=gpidx+4096;
											char cc3[64];
											sprintf(cc3,"%s%dB",cc,j);
		 		    						OBCF->Name=cc3;
			        						OBCF->ResType=0xFF;
			        						OBCF->WorkAmount=0;
											OBCF->WorkRadius=32;
											OBCF->ResPerWork=0;
			    	    					OBCF->WNextObj=i;
				    						OBCF->DNextObj=i;
		 		    						OBCF->DamageAmount=0;
			            					OBCF->IntResType=0xFF;
			            					OBCF->IntResPerWork=0;
				        					OBCF->IntWorkRadius=32;
				        					OBCF->TimeAmount=0;
				        					OBCF->TNextObj=i;
											OBCF->Parts=1;
											OBCF->Z0=0;
											OBCF->DZ=0;
											OBCF->Frames=1;
											OBCF->Stand=NULL;
											OBCF->Discret=1;
											OBCF->OnGround=0xFFFF;
											SetSprDefaults(OBCF);
											spidx++;
											gpidx++;
										};
									}else{
										int nnew=NSpr+N2-N1;
										Items=(word*)realloc(Items,nnew*2);
										Items[NSpr]=i;
										Dx=(short*)realloc(Dx,nnew*2);
										Dy=(short*)realloc(Dy,nnew*2);
										Radius=(short*)realloc(Radius,nnew*2);				
										ObjChar=(ObjCharacter**)realloc(ObjChar,nnew*4);
										memset(ObjChar+NSpr,0,(nnew-NSpr)*4);
										for(int q=0;q<(nnew-NSpr);q++){
											ObjChar[q+NSpr]=new ObjCharacter;
										}
										NSpr=nnew;
										for(int i=N1;i<=N2;i++){
											Dx[spidx]=dx;
		        							Dy[spidx]=dy;
		    	    						Radius[spidx]=rr;
			    	    					ObjCharacter* OBC=ObjChar[spidx];
											OBC->SpriteIndex=gpidx;
		 		    						OBC->Name=gx;
			        						OBC->ResType=0xFF;
			        						OBC->WorkAmount=0;
											OBC->WorkRadius=32;
											OBC->ResPerWork=0;
			    	    					OBC->WNextObj=i;
				    						OBC->DNextObj=i;
		 		    						OBC->DamageAmount=0;
			            					OBC->IntResType=0xFF;
			            					OBC->IntResPerWork=0;
				        					OBC->IntWorkRadius=32;
				        					OBC->TimeAmount=0;
				        					OBC->TNextObj=i;
                        					OBC->Parts=1;
											OBC->Z0=0;
											OBC->DZ=0;
											OBC->Frames=1;
											OBC->Stand=NULL;
											OBC->Discret=1;
											OBC->OnGround=0xFFFF;
											spidx++;
											gpidx++;
										};
									};
								};
							};	
						};  
						NLine(f1);
						Line++;
					}else
					if(gx[0]=='@'){

						Items=(word*)realloc(Items,(NSpr+1)*2);
						Items[NSpr]=i;
						Dx=(short*)realloc(Dx,(NSpr+1)*2);
						Dy=(short*)realloc(Dy,(NSpr+1)*2);
						Radius=(short*)realloc(Radius,(NSpr+1)*2);				
						ObjChar=(ObjCharacter**)realloc(ObjChar,(NSpr+1)*sizeof ObjCharacter);
						memset(ObjChar+NSpr,0,sizeof ObjCharacter);
						ObjChar[NSpr]=new ObjCharacter;
						NSpr++;

						z=Gscanf(f1,"%d%d%d",&dx,&dy,&rr);
		 	    		if(z!=3)IncPar(fname,Line,"");
		        		Dx[spidx]=dx;
		        		Dy[spidx]=dy;
						Dx[spidx+1]=-dx;
		        		Dy[spidx+1]=dy;
		    	    	Radius[spidx]=rr;
						Radius[spidx+1]=rr;
			    	    ObjCharacter* OBCF=ObjChar[spidx];
						ObjCharacter* OBCB=ObjChar[spidx+1];
						OBCF->SpriteIndex=gpidx;
						OBCB->SpriteIndex=gpidx+4096;
		 		    	OBCF->Name=gx;
						OBCB->Name=gx;
						OBCF->Name+="F";
						OBCB->Name+="B";

			        	OBCF->ResType=0xFF;
						OBCB->ResType=0xFF;

			        	OBCF->WorkAmount=0;
						OBCB->WorkAmount=0;

						OBCF->WorkRadius=32;
						OBCB->WorkRadius=32;

						OBCF->ResPerWork=0;
						OBCB->ResPerWork=0;

			    	    OBCF->WNextObj=i;
						OBCB->WNextObj=i;

				    	OBCF->DNextObj=i;
						OBCB->DNextObj=i;

		 		    	OBCF->DamageAmount=0;
						OBCB->DamageAmount=0;

			            OBCF->IntResType=0xFF;
						OBCB->IntResType=0xFF;

			            OBCF->IntResPerWork=0;
						OBCB->IntResPerWork=0;

				        OBCF->IntWorkRadius=32;
						OBCB->IntWorkRadius=32;

				        OBCF->TimeAmount=0;
						OBCB->TimeAmount=0;

				        OBCF->TNextObj=i;
						OBCB->TNextObj=i;

						OBCF->Parts=1;
                        OBCB->Parts=1;

						OBCF->Z0=0;
						OBCB->Z0=0;

						OBCF->DZ=0;
						OBCB->DZ=0;

						OBCF->Frames=1;
						OBCB->Frames=1;

						OBCF->Stand=NULL;
						OBCB->Stand=NULL;

						OBCF->Discret=1;
						OBCB->Discret=1;

						OBCF->OnGround=0xFFFF;
						OBCB->OnGround=0xFFFF;

						SetSprDefaults(OBCF);
						SetSprDefaults(OBCB);

						NLine(f1);
						Line++;
						spidx+=2;
						gpidx++;
					}else{
		    		    z=Gscanf(f1,"%d%d%d",&dx,&dy,&rr);
		 	    		if(z!=3)IncPar(fname,Line,"");
		        		Dx[spidx]=dx;
		        		Dy[spidx]=dy;
		    	    	Radius[spidx]=rr;
			    	    ObjCharacter* OBC=ObjChar[spidx];
						OBC->SpriteIndex=gpidx;
		 		    	OBC->Name=gx;
			        	OBC->ResType=0xFF;
			        	OBC->WorkAmount=0;
						OBC->WorkRadius=32;
						OBC->ResPerWork=0;
			    	    OBC->WNextObj=i;
				    	OBC->DNextObj=i;
		 		    	OBC->DamageAmount=0;
			            OBC->IntResType=0xFF;
			            OBC->IntResPerWork=0;
				        OBC->IntWorkRadius=32;
				        OBC->TimeAmount=0;
				        OBC->TNextObj=i;
                        OBC->Parts=1;
						OBC->Z0=0;
						OBC->DZ=0;
						OBC->Frames=1;
						OBC->Stand=NULL;
						OBC->Discret=1;
						OBC->OnGround=0xFFFF;
						SetSprDefaults(OBC);
						NLine(f1);
						Line++;
						spidx++;
						gpidx++;
					};
                }else{
                    NLine(f1);
                    Line++;
                };
		    };
        };
    }else{
        sprintf(str,"Could not open sprites discription : %s",fn);
    	ErrM(str);
    };
    Gclose(f1);
	//loading RSR file
	sprintf(fn,"%s.rsr",fname);
	f1=Gopen(fn,"r");
	AFile(fn);
	if(f1){
		int mode=0;
		do{
			z=Gscanf(f1,"%s",gx);
			if(z!=1)IncPar(fn,0,"?");
			if(gx[0]=='['){
				if(!strcmp(gx,"[WORKTRANSFORM]"))mode=1;
				else
				if(!strcmp(gx,"[FIRETRANSFORM]"))mode=2;
				else
				if(!strcmp(gx,"[SOURCES]"))mode=3;
				else
                if(!strcmp(gx,"[TIMETRANSFORM]"))mode=4;
                else
                if(!strcmp(gx,"[INTERNAL_SOURCE]"))mode=5;
                else
				if(!strcmp(gx,"[DISCRET]"))mode=6;
                else
				if(!strcmp(gx,"[LOCK]"))mode=7;
                else
				if(!strcmp(gx,"[GROUND]"))mode=8;
                else
				//new!!!
				if(!strcmp(gx,"[HIDDEN]"))mode=9;
                else
				if(!strcmp(gx,"[AUTOBORN]"))mode=10;
                else
				if(!strcmp(gx,"[WATERDEPENDENT]"))mode=11;
                else
				if(!strcmp(gx,"[ANIMATE]"))mode=12;
                else
				if(!strcmp(gx,"[SECTIONS]"))mode=13;
                else
				if(!strcmp(gx,"[AUTOANIMATE]"))mode=14;
                else
				if(!strcmp(gx,"[RANDOM]"))mode=15;
                else
				if(!strcmp(gx,"[FIXEDPLACE]"))mode=16;
                else
				if(!strcmp(gx,"[SHOTSHIELD]"))mode=17;
				else
				if(!strcmp(gx,"[CONNECTOR]"))mode=18;
                else
				if(!strcmp(gx,"[ALIGNING]"))mode=19;
                else
				if(!strcmp(gx,"[FASTSPRITE]"))mode=20;
				else
				if(!strcmp(gx,"[MODEL]"))mode=21;
				else
				if(!strcmp(gx,"[FIXH]"))mode=22;
				else
				if(!strcmp(gx,"[AUTODEEP]"))mode=23;
				else
				if(!strcmp(gx,"[AMPLITUDE]"))mode=24;
				else
				if(!strcmp(gx,"[SOUND]"))mode=25;
				else
				if(!strcmp(gx,"[MODS]"))mode=26;
				else
				if(!strcmp(gx,"[EDITZ]"))mode=27;
				else
                if(!strcmp(gx,"[LQSHADOW]"))mode=28;
                else
				if(!strcmp(gx,"[NATSOUND]"))mode=29;
				else
				if(!strcmp(gx,"[GRASS]"))mode=30;
				else
				//---end---
				if(!strcmp(gx,"[END]"))mode=255;
				else{
					sprintf(gy,"Unknown section in <%s.rsr> : %s",fname,gx);
					ErrM(gy);
				};
			}else{
				if(gx[0]=='/')NLine(f1);
				else{
					switch(mode){
					case 1://[WORKTRANSFORM]
					case 2://[FIRETRANSFORM]
                    case 4://[TIMETRANSFORM]
						z=Gscanf(f1,"%s%d",gy,&parm1);
						if(z!=2)IncPar(fn,0,"[WORK/FIRE/TIMETRANSFORM]");
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
						z2=GetIndexByNameOLD(gy);
						if(z2==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gy);
							ErrM(str);
						};	
						if(mode==1){
							ObjChar[z1]->WNextObj=z2;
							ObjChar[z1]->WorkAmount=parm1;
						};
						if(mode==2){
							ObjChar[z1]->DNextObj=z2;
							ObjChar[z1]->DamageAmount=parm1;
						};
                        if(mode==4){
                            ObjChar[z1]->TNextObj=z2;
							ObjChar[z1]->TimeAmount=parm1;
                        };
						break;
					case 3://[SOURCES]
						z=Gscanf(f1,"%s%d%d",gy,&parm1,&rr);
						if(z!=3)IncPar(fn,0,"[SOURCES]");
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
                        z2=GetResByName(gy);
                        if(z2==-1){
							sprintf(str,"%s : Unknown resource type :%s",fn,gy);
							ErrM(str);
						};
						ObjChar[z1]->ResType=z2;
						ObjChar[z1]->ResPerWork=parm1;
						ObjChar[z1]->WorkRadius=rr;
						break;
                    case 5://[INTERNAL_SOURCE]
                        z=Gscanf(f1,"%s%d%d",gy,&parm1,&rr);
						if(z!=3)IncPar(fn,0,"[INTERNAL_SOURCE]");
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
                        z2=GetResByName(gy);
                        if(z2==-1){
							sprintf(str,"%s : Unknown resource type :%s",fn,gy);
							ErrM(str);
						};
						ObjChar[z1]->IntResType=z2;
						ObjChar[z1]->IntResPerWork=parm1;
						ObjChar[z1]->IntWorkRadius=rr;
						break;
					case 6:
						z=Gscanf(f1,"%d",&parm1);
						if(z!=1)IncPar(fn,0,"[DISCRET]");
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
						ObjChar[z1]->Discret=parm1;
						break;
					case 8://[GROUND]
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						}else ObjChar[z1]->OnGround=AddOneGroundType(this,z1);
						break;
					case 15://[RANDOM]
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
						z=Gscanf(f1,"%d",&parm1);
						if(z!=1)IncPar(fn,0,"[RANDOM]");
						ObjChar[z1]->NRandom=parm1+1;
						break;
					case 10://[AUTOBORN]
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
						z=Gscanf(f1,"%d",&parm1);
						if(z!=1)IncPar(fn,0,"[AUTOBORN]");
						//ObjChar[z1]->NBorn=parm1;
						//ObjChar[z1]->BREF=xnew(parm1,BornRef);
						for(int i=0;i<parm1;i++){
							int dx,dy;
							z=Gscanf(f1,"%s%d%d",gy,&dx,&dy);
							if(z==3){
								int z2=GetIndexByNameOLD(gy);
								if(z2==-1){
									sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
									ErrM(str);
								}else{
									BornRef* BR=new BornRef;
									BR->ObjectToBorn.Set(ObjChar[z2]);
									BR->BDx=dx;
									BR->BDy=dy;
									ObjChar[z1]->BREF.Add(BR);
								};
							}else IncPar(fn,0,"[AUTOBORN]");
						};
						break;
					case 12://[ANIMATE]
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
						z=Gscanf(f1,"%d",&parm1);
						if(z!=1)IncPar(fn,0,"[ANIMATE]");
						nl=z1;
						for(int i=0;i<parm1;i++){
							z=Gscanf(f1,"%s",gy);
							if(z==1){
								int z2=GetIndexByNameOLD(gy);
								if(z2==-1){
									sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
									ErrM(str);
								}else{
									ObjChar[z1]->TimeAmount=1;
									ObjChar[z1]->TNextObj=z2;
									z1=z2;
									if(i==parm1-1){
										ObjChar[z1]->TimeAmount=1;
										ObjChar[z1]->TNextObj=nl;
									};
								};
							};
						};
						break;
					case 14://[AUTOANIMATE]
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
						z=Gscanf(f1,"%d",&parm1);
						if(z!=1)IncPar(fn,0,"[AUTOANIMATE]");
						nl=z1;
						for(int i=0;i<parm1;i++){
							ObjChar[nl+i]->TimeAmount=1;
							ObjChar[nl+i]->TNextObj=nl+((i+1)%parm1);
						};
						break;
					case 16://[FIXEDPLACE]
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
						{
							int dx,dy,ang,r;
							z=Gscanf(f1,"%d%d%d%d",&dx,&dy,&ang,&r);
							if(z!=4)IncPar(fn,0,"[FIXEDPLACE]");
							ObjChar[z1]->FixDx=dx;
							ObjChar[z1]->FixDy=dy;
							ObjChar[z1]->FixDir=ang;
							ObjChar[z1]->FixR=r;
						};
						break;
					case 17://[SHOTSHIELD]
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
						{
							int a,b;
							z=Gscanf(f1,"%d%d",&a,&b);
							if(z!=2)IncPar(fn,0,"[SHOTSHIELD]");
							ObjChar[z1]->ShieldProbability=b;
							ObjChar[z1]->ShieldRadius=a;
						};
						break;
					case 18://[CONNECTOR]
                        z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						};
						{
							//SPRID Nconn x0 y0 Type0  x1 y1 Type1 ... 
							int NC,NO;
							z=Gscanf(f1,"%d%d",&NO,&NC);
							if(z==2){
								NC+=NO;
								for(int i=0;i<NC;i++){
									int x0,y0,CT0;
									z=Gscanf(f1,"%d%d%d",&x0,&y0,&CT0);
									if(z==3){
										ObjCharacter* OC=ObjChar[z1];
										OneConnector* OCN=new OneConnector;
										OCN->x=x0;
										OCN->y=y0;
										OCN->ConnType=CT0;
										OC->CONN.Add(OCN);
										OC->NOutConn=NO;
									}else IncPar(gx,0,"[CONNECTOR]");
								}
							}else IncPar(gx,0,"[CONNECTOR]");
						}
                        break;
					case 26://[MODS]
						//treeID LandID Model Scale Rotation Color RiseTime DeathTime Shader ShaderRiseVar ShaderDeathVar
						{
							z1=GetIndexByNameOLD(gx);							
							if(z1!=-1){
								gy[0]=0;
								TreeModificationParam* TM=new TreeModificationParam;
								char shName[64];
								char Land[32];
								char TreeID[32];
								char TreeID2[32];
								z=Gscanf(f1,"%s%s%f%f%X%d%d%s%s%s%f",Land,gy,&TM->Scale,&TM->RotAngle,&TM->Color,&TM->RiseTime,&TM->DeathTime,shName,TreeID,TreeID2,&TM->ShadowScale);
								if(z!=11){
									IncPar(gx,0,"[MODS]");
								}
								int idx=IMM->GetModelID(gy);
								if(idx!=-1){
									TM->ModelID=idx;
									TM->LeafsShaderID=-1;
									TM->LeafsDeathVariable=-1;
									TM->LeafsRiseVariable=-1;
									if(strcmp(shName,"none")){
										TM->LeafsShaderID=IRS->GetShaderID(shName);
										TM->LeafsRiseVariable=IRS->GetShaderVarID(TM->LeafsShaderID,"stage");
									}
									static Enumerator* E=ENUM.Get("LANDLAYERS");
									TM->LandType=E->Get(Land);
									TM->RotAngle*=c_PI/180.0f;
									TM->SpriteForShadowID=GetIndexByNameOLD(TreeID);
									TM->SpriteForIsomerticMode=GetIndexByNameOLD(TreeID2);
									ObjCharacter* OC=ObjChar[z1];
									OC->TreeMods.Add(TM);
								}else{
									Log.Warning("%s : Unknown model ID :%s",fn,gy);
								}
							}else{
								Log.Warning("%s : Unknown sprite ID :%s",fn,gx);
							}
						}
						break;
					case 21://[MODEL]
						{
							z1=GetIndexByNameOLD(gx);							
							if(z1!=-1){
								gy[0]=0;
								float sx=0;
								float sy=0;
								float Scale=1.0f;
								float Rotate=0.0f;
								DWORD TF=0xFF808080;
                                z=Gscanf(f1,"%s%f%f%f%f%X",gy,&sx,&sy,&Scale,&Rotate,&TF);
								int idx=IMM->GetModelID(gy);
								if(idx!=-1){
									ObjCharacter* OC=ObjChar[z1];
									OC->ModelManagerID=idx;
									OC->MShiftX=sx;
									OC->MShiftY=sy;
									OC->Scale=Scale;
									OC->RotFI=Rotate*c_PI*2/256.0;
									OC->TFactor=TF;
									///OC->EditableModel=OC->MShiftX!=0;
									OC->ViewType=1;
									OC->RenderType=0;
								}else{
									Log.Warning("%s : Unknown model ID :%s",fn,gy);
								}
							}else{
								Log.Warning("%s : Unknown sprite ID :%s",fn,gx);
							}
						}						
						break;
					case 27://[EDITZ]
						{
							z1=GetIndexByNameOLD(gx);							
							if(z1!=-1)ObjChar[z1]->EditableModel=true;
							else{
								Log.Warning("%s : Unknown sprite ID :%s",fn,gx);
							}
						}						
						break;
                    case 28://[LQSHADOW]
                        {
                            z1=GetIndexByNameOLD(gx);
                            int idx;
                            float sx,sy;
                            z=Gscanf(f1,"%d%f%f",&idx,&sx,&sy);
                            if(z1!=-1){
                                ObjChar[z1]->LoQShadowID=idx;
                                ObjChar[z1]->LoQShadowSX=sx;
                                ObjChar[z1]->LoQShadowSY=sy;
                            }else{
                                Log.Warning("%s : Unknown sprite ID :%s",fn,gx);
                            }
                        }						
                        break;
					case 22://[FIXH]
						{
							z1=GetIndexByNameOLD(gx);							
							if(z1!=-1){
								int hh=0;
								z=Gscanf(f1,"%d",&hh);
								ObjCharacter* OC=ObjChar[z1];
								OC->FixHeight=hh;									
							}else{
								sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
								ErrM(str);								
							}
						}						
						break;
					case 23://[AUTODEEP]
						{
							z1=GetIndexByNameOLD(gx);							
							if(z1!=-1){
								int r1=100;
								int r2=200;
								int hh=-60;
								z=Gscanf(f1,"%d%d%d",&hh,&r1,&r2);
								ObjCharacter* OC=ObjChar[z1];
								OC->AutoDeep=hh;
								OC->AutoDeepR=r1;									
								OC->AutoDeepR1=r2;									
							}else{
								sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
								ErrM(str);								
							}
						}						
						break;
					case 24://[AMPLITUDE]
						{
							z1=GetIndexByNameOLD(gx);							
							if(z1!=-1){
								int r1=0;
								z=Gscanf(f1,"%d",&r1);
								ObjCharacter* OC=ObjChar[z1];
								OC->Amplitude=r1;
							}else{
								sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
								ErrM(str);								
							}
						}						
						break;
					case 25://[SOUND]
						{
							z1=GetIndexByNameOLD(gx);							
							if(z1!=-1){
								float r1=0;
								z=Gscanf(f1,"%s%f",gy,&r1);
								if(z==2){
									ObjCharacter* OC=ObjChar[z1];
									OC->SoundID=SearchStr(SoundID,gy,NSounds);
									OC->SoundProb=int(r1*32768/100);
								}
							}else{
								sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
								ErrM(str);								
							}
						}						
						break;
					case 29://[NATSOUND]
						{
							z1=GetIndexByNameOLD(gx);
							if(z1!=-1){
								float r1=0;
								char Land[32];
								z=Gscanf(f1,"%s%f%s",gy,&r1,Land);
								if(z==3){
									static Enumerator* E=ENUM.Get("LANDLAYERS");
									int LT=E->Get(Land);
									if(LT!=-1){										
										ObjCharacter* OC=ObjChar[z1];
										for(int i=0;i<OC->TreeMods.GetAmount();i++){
											TreeModificationParam* tm=OC->TreeMods[i];
											if(tm->LandType==LT){
                                                tm->SoundID=SearchStr(SoundID,gy,NSounds);
												tm->SoundProbability=int(r1*32768/100);
											}
										}										
									}else{
										sprintf(str,"%s : Unknown land type :%s",fn,Land);
										ErrM(str);								
									}
								}
							}else{
								sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
								ErrM(str);								
							}
						}						
						break;
					case 30://[GRASS]
						{
							z1=GetIndexByNameOLD(gx);							
							if(z1!=-1){
								int gh,gr,gt;
								z=Gscanf(f1,"%d%d%d",&gt,&gr,&gh);
								if(z==3){
									ObjCharacter* OC=ObjChar[z1];
									OC->GrassHeight=gh;
									OC->GrassRadius=gr;
									OC->GrassType=gt;
								}else{
									sprintf(str,"%s : wrong [GRASS] parameters for sprite :%s",fn,gx);
									ErrM(str);								
								}
							}else{
								sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
								ErrM(str);								
							}
						}						
						break;
						/*
					case 24://[ROADCENTER]
						z1=GetIndexByName(gx);
						if(z1!=-1){							
							ObjCharacter* OC=ObjChar+z1;
							OC->RoadCenter=true;
						}else{
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);								
						}
						break;
						*/
					case 19://[ALIGNING]
						//SPRID V x1 y1 x2 y2
						//SPRID H
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						}
						{
							char cc[32];
							z=Gscanf(f1,"%s",cc);
							if(z==1){
								ObjCharacter* OC=ObjChar[z1];
								SprGroup* SG=this;
								float TBias=-16.0f;
								float TBiasY=TBias*0.866025f;
								float TBiasZ=TBias*0.5f;
								if(cc[0]=='V'){
									//vertical aligning																		
									z=Gscanf(f1,"%d%d%d%d",&OC->va_x1,&OC->va_y1,&OC->va_x2,&OC->va_y2);
									OC->Aligning=2;									
								}else
								if(cc[0]=='S'){
									//vertical aligning																		
									z=Gscanf(f1,"%d%d%d%d",&OC->va_x1,&OC->va_y1,&OC->va_x2,&OC->va_y2);
									OC->Aligning=2;																			
								}else
								if(cc[0]=='H'){
									//horizontal aligning
									OC->Aligning=1;											
								}else if(cc[0]=='U'){//universal alignung
									float _x0,_y0,_z0,_x1,_y1,_z1,_x2,_y2,_z2;
									z=Gscanf(f1,"%f%f%f%f%f%f%f%f%f",&_x0,&_y0,&_z0,&_x1,&_y1,&_z1,&_x2,&_y2,&_z2);
								}else IncPar(gx,0,"[ALIGNING]");
								//ObjCharacter* OC=ObjChar[z1];
								//SprGroup* SG=this;
								//float TBias=-16.0f;
								//float TBiasY=TBias*0.866025f;
								//float TBiasZ=TBias*0.5f;
								//if(cc[0]=='V'){
								//	//vertical aligning
								//	float Bias=0.6f;
								//	float BiasY=Bias*0.866025f;
								//	float BiasZ=Bias*0.5f;

								//	int x1,y1,x2,y2;
								//	z=Gscanf(f1,"%d%d%d%d",&x1,&y1,&x2,&y2);
								//	OC->HaveAligning=1;
								//	OC->P1.xp=x1;
								//	OC->P1.yp=y1;
								//	OC->P1.wx=x1-Dx[z1];
								//	OC->P1.wy=(y1-Dy[z1])*2;
								//	OC->P1.wz=0;

								//	OC->P2.xp=x2;
								//	OC->P2.yp=y2;
								//	OC->P2.wx=x2-Dx[z1];
								//	OC->P2.wy=(y2-Dy[z1])*2;
								//	OC->P2.wz=0;

								//	if(y2<y1){
        //                                OC->P1.wy-=BiasY;
								//		OC->P1.wz-=BiasZ;
								//		OC->P2.wy+=BiasY;
								//		OC->P2.wz+=BiasZ;
								//	}else{
								//		OC->P1.wy+=BiasY;
								//		OC->P1.wz+=BiasZ;
								//		OC->P2.wy-=BiasY;
								//		OC->P2.wz-=BiasZ;
								//	}
								//	int dz=128;
								//	OC->P3.xp=(x1+x2)/2;
								//	OC->P3.yp=(y1+y2)/2-dz;
								//	OC->P3.wx=(OC->P1.wx+OC->P2.wx)/2;
								//	OC->P3.wy=(OC->P1.wy+OC->P2.wy-dz)/2;
								//	OC->P3.wz=dz*866/1000;
								//	for(int i=0;i<3;i++){
								//		(&OC->P1)[i].wy+=BiasY*10;
								//		(&OC->P1)[i].wz+=BiasZ*10;
								//	}
								//	OC->P1.wy-=TBiasY;
								//	OC->P1.wz-=TBiasZ;
								//	OC->P2.wy-=TBiasY;
								//	OC->P2.wz-=TBiasZ;
								//	OC->P3.wy-=TBiasY;
								//	OC->P3.wz-=TBiasZ;
								//}else
								//if(cc[0]=='S'){
								//	//shadow for vertical aligning
								//	float Bias=0.0f;
								//	float BiasY=Bias*0.866025f;
								//	float BiasZ=Bias*0.5f;

								//	int x1,y1,x2,y2;
								//	z=Gscanf(f1,"%d%d%d%d",&x1,&y1,&x2,&y2);
								//	OC->HaveAligning=1;
								//	OC->P1.xp=x1;
								//	OC->P1.yp=y1;
								//	OC->P1.wx=x1-Dx[z1];
								//	OC->P1.wy=(y1-Dy[z1])*2;
								//	OC->P1.wz=0;

								//	OC->P2.xp=x2;
								//	OC->P2.yp=y2;
								//	OC->P2.wx=x2-Dx[z1];
								//	OC->P2.wy=(y2-Dy[z1])*2;
								//	OC->P2.wz=0;

								//	if(y2<y1){
        //                                OC->P1.wy-=BiasY;
								//		OC->P1.wz-=BiasZ;
								//		OC->P2.wy+=BiasY;
								//		OC->P2.wz+=BiasZ;
								//	}else{
								//		OC->P1.wy+=BiasY;
								//		OC->P1.wz+=BiasZ;
								//		OC->P2.wy-=BiasY;
								//		OC->P2.wz-=BiasZ;
								//	}
								//	int dy=128;
								//	OC->P3.xp=(x1+x2)/2;
								//	OC->P3.yp=(y1+y2)/2+dy;
								//	OC->P3.wx=(OC->P1.wx+OC->P2.wx)/2;
								//	OC->P3.wy=(OC->P1.wy+OC->P2.wy+dy*4)/2;
								//	OC->P3.wz=0;
								//	for(int i=0;i<3;i++){
								//		(&OC->P1)[i].wy+=BiasY*10;
								//		(&OC->P1)[i].wz+=BiasZ*10;
								//	}
								//	OC->P1.wy-=TBiasY;
								//	OC->P1.wz-=TBiasZ;
								//	OC->P2.wy-=TBiasY;
								//	OC->P2.wz-=TBiasZ;
								//	OC->P3.wy-=TBiasY;
								//	OC->P3.wz-=TBiasZ;
								//}else
								//if(cc[0]=='H'){
								//	//horizontal aligning
								//	float Bias=-6.0f;
								//	OC->HaveAligning=1;
								//	int xc=Dx[z1];
								//	int yc=Dy[z1];
								//	OC->P1.xp=xc-20;
								//	OC->P1.yp=yc-5;
								//	OC->P1.wx=-20;
								//	OC->P1.wy=-10;
								//	OC->P1.wz=Bias;

								//	OC->P2.xp=xc+20;
								//	OC->P2.yp=yc-5;
								//	OC->P2.wx=20;
								//	OC->P2.wy=-10;
								//	OC->P2.wz=Bias;

								//	OC->P3.xp=xc;
								//	OC->P3.yp=yc+10;
								//	OC->P3.wx=0;
								//	OC->P3.wy=20;
								//	OC->P3.wz=Bias;
								//	OC->P1.wy-=TBiasY;
								//	OC->P1.wz-=TBiasZ;
								//	OC->P2.wy-=TBiasY;
								//	OC->P2.wz-=TBiasZ;
								//	OC->P3.wy-=TBiasY;
								//	OC->P3.wz-=TBiasZ;
								//}else if(cc[0]=='U'){//universal alignung
								//	float _x0,_y0,_z0,_x1,_y1,_z1,_x2,_y2,_z2;
								//	z=Gscanf(f1,"%f%f%f%f%f%f%f%f%f",&_x0,&_y0,&_z0,&_x1,&_y1,&_z1,&_x2,&_y2,&_z2);
								//	if(z==9){
								//		int xc=Dx[z1];
								//		int yc=Dy[z1];
								//		OC->HaveAligning=1;
        //                                OC->P1.xp=_x0;
								//		OC->P1.yp=_y0;
								//		OC->P1.wx=_x0-xc;
								//		OC->P1.wy=(_y0+_z0-yc)*2-_z0/2;
        //                                OC->P1.wz=_z0*0.866025f;

								//		OC->P2.xp=_x1;
								//		OC->P2.yp=_y1;
								//		OC->P2.wx=_x1-xc;
								//		OC->P2.wy=(_y1+_z1-yc)*2-_z1/2;
        //                                OC->P2.wz=_z1*0.866025f;

								//		OC->P3.xp=_x2;
								//		OC->P3.yp=_y2;
								//		OC->P3.wx=_x2-xc;
								//		OC->P3.wy=(_y2+_z2-yc)*2-_z2/2;
								//      OC->P3.wz=_z2*0.866025f;

								//	}else IncPar(gx,0,"[ALIGNING] ('U' option)");
								//}else IncPar(gx,0,"[ALIGNING]");
							}else IncPar(gx,0,"[ALIGNING]");
						}
						break;
					case 20://[FASTSPRITE]
						{
							//char cc[32];
							float x,y,lx,ly,xc,yc;
							int z=Gscanf(f1,"%f%f%f%f%f%f",&x,&y,&lx,&ly,&xc,&yc);
							if(z==6){
								int z2=GetIndexByNameOLD(gx);
								if(z2!=-1){
                                    ObjCharacter* OC=ObjChar[z2];
									OC->UseTexture=1;
									OC->uL=x;
									OC->vL=y;
									OC->uR=lx;
									OC->vR=ly;
									OC->uC=xc;
									OC->vC=yc;
								}else{
									sprintf(str,"%s : Unknown sprite ID :%s",fn,gy);
									ErrM(str);
								} 
							}else IncPar(gx,0,"[FASTSPRITE]");
						}
						break;
					case 13://[SECTIONS]
						{
							void AddTreesToGroup(char* Section,SprGroup* SG,int Index);
							char* sname=gx;
							SectNames=(char**)realloc(SectNames,(NSections+1)*4);
							SectContence=(int**)realloc(SectContence,(NSections+1)*4);
							NInSect=(int*)realloc(NInSect,(NSections+1)*4);
							int z=Gscanf(f1,"%d",&nl);
							if(z==1){
								SectNames[NSections]=xnew(strlen(sname)+1,char);
								SectContence[NSections]=xnew(nl,int);
								strcpy(SectNames[NSections],sname);
								NInSect[NSections]=nl;
								for(int i=0;i<nl;i++){
									z=Gscanf(f1,"%s",gy);
									if(z==1){
										int z2=GetIndexByNameOLD(gy);
										if(z2==-1){
											sprintf(str,"%s : Unknown sprite ID :%s",fn,gy);
											ErrM(str);
										}else{ 
											SectContence[NSections][i]=z2;
											AddTreesToGroup(sname,this,z2);
											if(ObjChar[z2]->GroupIndex){
												ObjChar[z2]->GroupIndex=NSections+1;
												ObjChar[z2]->IndexInGroup=i;
											}
										}
									}else IncPar(sname,0,"[SECTIONS]");
								};
							}else IncPar(sname,0,"[SECTIONS]");
							NSections++;
						};
						break;
					case 7://[LOCK]
						z=Gscanf(f1,"%d",&parm1);
						if(z!=1)IncPar(fn,0,"[LOCK]");
						z1=GetIndexByNameOLD(gx);
						if(z1==-1){
							sprintf(str,"%s : Unknown sprite ID :%s",fn,gx);
							ErrM(str);
						}else{
							//ObjChar[z1]->NLockPt=parm1;
							//ObjChar[z1]->LockX=xnew(parm1,short);
							//ObjChar[z1]->LockY=xnew(parm1,short);
							for(int i=0;i<parm1;i++){
								LockInfoPoint* LIP=new LockInfoPoint;
								int x,y;
								z=Gscanf(f1,"%d%d",&x,&y);
								if(z==2){
									LIP->x=x;
									LIP->y=y;
									ObjChar[z1]->LockInfo.Add(LIP);
								}else{
									IncPar(fn,0,"[LOCK]");
								};
							};
						};
						break;
                    };
				};
			};
		}while(mode!=255);
		Gclose(f1);	
	};
	int tr1=GPS.PreLoadGPImage("trees");
	int tr2=GPS.PreLoadGPImage("treesall");
	for(int i=0;i<NSpr;i++){
		ObjChar[i]->CenterX=Dx[i];
		ObjChar[i]->CenterY=Dy[i];
		ObjChar[i]->Radius=Radius[i];
		ObjChar[i]->FileID=FileID;
		ObjChar[i]->SpriteID=i;
		ObjChar[i]->FileID_forBackground=FileID;
		ObjChar[i]->SpriteID_forBackground=i;
		ObjChar[i]->FileID_forPreview=FileID;
		ObjChar[i]->SpriteID_forPreview=i;
		if(this==&TREES){
			ObjChar[i]->FileID_forBackground=tr2;
			ObjChar[i]->FileID_forPreview=tr1;
			if(!ObjChar[i]->UseTexture)ObjChar[i]->RenderType=1;
			else ObjChar[i]->RenderType=0;

		}
		if(this==&STONES){
			ObjChar[i]->RenderType=2;
		}
		if(this==&COMPLEX){
			ObjChar[i]->ViewType=2;
		}
		ObjChar[i]->ParentIndex=i;
		if(ObjChar[i]->WNextObj!=0xFFFF){
			ObjCharacter* OC=ObjChar[ObjChar[i]->WNextObj];
			if(OC)ObjChar[i]->NextWorkObj.Set(OC);
		}
		if(ObjChar[i]->TNextObj!=0xFFFF){
			ObjCharacter* OC=ObjChar[ObjChar[i]->TNextObj];
			if(OC)ObjChar[i]->NextTimeObj.Set(OC);
		}
		Objects.Add(ObjChar[i]);
	}
	for(int i=0;i<NSections;i++){
		OneSprSection* OSS=new OneSprSection;
		OSS->SectionName=SectNames[i];
		int N1=NInSect[i];
		for(int j=0;j<N1;j++){
			ObjCharRef* CR=new ObjCharRef;
			ObjCharacter* OC=ObjChar[SectContence[i][j]];
			CR->Set(OC);
            OSS->ObjectsList.Add(CR);
		}
		Sections.Add(OSS);
	}
	if(ObjChar)free(ObjChar);
	if(Dx)free(Dx);
	if(Dy)free(Dy);
	if(Radius)free(Radius);	
	if(SectNames)free(SectNames);
	if(SectContence)free(SectContence);
	if(NInSect)free(NInSect);
};
//-----------------------------------------------------------------------//
//                                                                       //
//                       PHYSICS OF THE NEW MONSTER                      //
//           New monsters choose freedom from cells through cells!       //
//-----------------------------------------------------------------------//
byte* MCount;//amount of monsters in 4x4 cell
byte* TmpMC; //amount of monsters in 4x4 cell,
                          //this is used only for force calculating
//word* NMsList;//array of ID of new monsters
word** BLDList;
void ZMem(byte* pntr,int siz){
	__asm{
		push	edi
		cld
		xor		eax,eax
		mov		edi,pntr
		mov		ecx,siz
		shr		ecx,2
		or		ecx,ecx
		jz		www
//		cli
		rep		stosd
		//sti
www:	pop		edi
	};
};



//Creating list of monsters according to cells
word GetDir(int dx,int dy){
	int PhDir;
	if(dx!=0||dy!=0){
		int adx=abs(dx);
		int ady=abs(dy);
		if(adx>ady)PhDir=byte(TAtg[div(ady<<8,adx).quot]);else
				   PhDir=64-byte(TAtg[div(adx<<8,ady).quot]);
		if(dx<0)PhDir=128-PhDir;
		if(dy<0)PhDir=256-PhDir;
	}else PhDir=64+128;
	return (PhDir+1024)&255;
};
CEXPORT word GetDirEx(int dx,int dy){
	return GetDir(dx,dy);
}
word GetDirDbg(int dx,int dy){
	int PhDir;
	if(dx!=0||dy!=0){
		int adx=abs(dx);
		int ady=abs(dy);
		addrand(adx);
		addrand(ady);
		if(adx>ady){
			addrand(div(ady<<8,adx).quot);
			addrand(TAtg[div(ady<<8,adx).quot]);
			PhDir=byte(TAtg[div(ady<<8,adx).quot]);
		}else{
			addrand(div(adx<<8,ady).quot);
			addrand(TAtg[div(adx<<8,ady).quot]);
            PhDir=64-byte(TAtg[div(adx<<8,ady).quot]);
		}
		if(dx<0)PhDir=128-PhDir;
		if(dy<0)PhDir=256-PhDir;
		addrand(PhDir);
	}else PhDir=64+128;
	return (PhDir+1024)&255;
};

short UnitLogY[4096];
short UnitLogX[4096];
short UnitX[4096];
short UnitY[4096];
word  UFileID[4096];
word  USpriteID[4096];
word  NURef[4096];
int   UParam1[4096];
int   UParam2[4096];
OneObject* OBJS[4096];
word NVUnits;
word FirstUN;
// - - - - -//-------\\- - - - - 
//----------\\Hashing//----------
word Hash16   [4096];
word Hash64   [1024];
word Hash256  [256];
void InitHash(){
	memset(Hash16,0,sizeof Hash16);
	memset(Hash64,0,sizeof Hash64);
	memset(Hash256,0,sizeof Hash256);
};

//----------//-------\\----------
void InitVisual(){
	NVUnits=0;
	FirstUN=0;
	InitHash();
};
/*
void AddToVisual(short uy,short x,short y,OneObject* OB,word FileID,word Sprite,int Param1,int Param2){
	int ux=x;
	if(!NVUnits){
		NURef[0]=0xFFFF;
		UnitLogY[0]=uy;
		UnitX[0]=x;
		UnitY[0]=y;
        UParam1[0]=Param1;
        UParam2[0]=Param2;
		UFileID[0]=FileID;
		USpriteID[0]=Sprite;
		OBJS[0]=OB;
		NVUnits=1;
		int uuy=(int(uy)+32768)>>4;
		Hash16[uuy]=0;
		Hash64[uuy>>2]=0;
		Hash256[uuy]>>4=0;
	}else{
		word un;
		int uuy=(int(uy)+32768)>>4;
		un=Hash16[uuy];
		if(un==0xFFFF){
			un=Hash64[uuy>>2];
			if(un==0xFFFF){
				un=Hash256[uuy>>4];
			};
		};
		if(un==0xFFFF)un=FirstUN;
		word un0=0xFFFF;
		word un1;
		do{
			un1=NURef[un];
			if(uy<UnitLogY[un]){
				if(un0!=0xFFFF)
					NURef[un0]=NVUnits;
				else FirstUN=NVUnits;
				NURef[NVUnits]=un;
				UnitLogY[NVUnits]=uy;
				UnitLogX[NVUnits]=ux;
				UnitX[NVUnits]=x;
				UnitY[NVUnits]=y;
				OBJS[NVUnits]=OB;
				UFileID[NVUnits]=FileID;
				USpriteID[NVUnits]=Sprite;
                UParam1[NVUnits]=Param1;
                UParam2[NVUnits]=Param2;
				if(un==Hash16[uuy]){
					Hash16[uuy]=NVUnits;
					uuy>>=2;
					if(Hash64[uuy]==un){
						Hash64[uuy]=NVUnits;
						uuy>>=2;
						if(Hash256[uuy]==un){
							hash256[uuy]=NVUnits;
						};
					};
				};
				NVUnits++;
				return;
			};
			if(un1==0xFFFF){
				NURef[un]=NVUnits;
				NURef[NVUnits]=0xFFFF;
				UnitLogY[NVUnits]=uy;
				UnitX[NVUnits]=x;
				UnitY[NVUnits]=y;
				UFileID[NVUnits]=FileID;
				USpriteID[NVUnits]=Sprite;
                UParam1[NVUnits]=Param1;
                UParam2[NVUnits]=Param2;
				OBJS[NVUnits]=OB;
				NVUnits++;
				return;
			};
			un0=un;
			un=un1;
		}while(true);
	};
};
*/
void AddToVisual(short uy,short x,short y,OneObject* OB,word FileID,word Sprite){
	//assert(RLCNSpr[FileID]>(Sprite&4095));
	if(GPS.GPNFrames(FileID)<=(Sprite&4095))return;
	if(!NVUnits){
		NURef[0]=0xFFFF;
		UnitLogY[0]=uy;
		UnitX[0]=x;
		UnitY[0]=y;
        UParam1[0]=0;
        UParam2[0]=0;
		UFileID[0]=FileID;
		USpriteID[0]=Sprite;
		OBJS[0]=OB;
		NVUnits=1;
	}else{
		word un=FirstUN;
		word un0=0xFFFF;
		word un1;
		do{
			un1=NURef[un];
			if(uy<UnitLogY[un]){
				if(un0!=0xFFFF)
					NURef[un0]=NVUnits;
				else FirstUN=NVUnits;
				NURef[NVUnits]=un;
				UnitLogY[NVUnits]=uy;
				UnitX[NVUnits]=x;
				UnitY[NVUnits]=y;
				OBJS[NVUnits]=OB;
				UFileID[NVUnits]=FileID;
				USpriteID[NVUnits]=Sprite;
                UParam1[NVUnits]=0;
                UParam2[NVUnits]=0;
				NVUnits++;
				return;
			};
			if(un1==0xFFFF){
				NURef[un]=NVUnits;
				NURef[NVUnits]=0xFFFF;
				UnitLogY[NVUnits]=uy;
				UnitX[NVUnits]=x;
				UnitY[NVUnits]=y;
				UFileID[NVUnits]=FileID;
				USpriteID[NVUnits]=Sprite;
                UParam1[NVUnits]=0;
                UParam2[NVUnits]=0;
				OBJS[NVUnits]=OB;
				NVUnits++;
				return;
			};
			un0=un;
			un=un1;
		}while(true);
	};
};
void AddToVisual(short uy,short x,short y,OneObject* OB,word FileID,word Sprite,int Param1,int Param2){
	if(!NVUnits){
		NURef[0]=0xFFFF;
		UnitLogY[0]=uy;
		UnitX[0]=x;
		UnitY[0]=y;
        UParam1[0]=Param1;
        UParam2[0]=Param2;
		UFileID[0]=FileID;
		USpriteID[0]=Sprite;
		OBJS[0]=OB;
		NVUnits=1;
	}else{
		word un=FirstUN;
		word un0=0xFFFF;
		word un1;
		do{
			un1=NURef[un];
			if(uy<UnitLogY[un]){
				if(un0!=0xFFFF)
					NURef[un0]=NVUnits;
				else FirstUN=NVUnits;
				NURef[NVUnits]=un;
				UnitLogY[NVUnits]=uy;
				UnitX[NVUnits]=x;
				UnitY[NVUnits]=y;
				OBJS[NVUnits]=OB;
				UFileID[NVUnits]=FileID;
				USpriteID[NVUnits]=Sprite;
                UParam1[NVUnits]=Param1;
                UParam2[NVUnits]=Param2;
				NVUnits++;
				return;
			};
			if(un1==0xFFFF){
				NURef[un]=NVUnits;
				NURef[NVUnits]=0xFFFF;
				UnitLogY[NVUnits]=uy;
				UnitX[NVUnits]=x;
				UnitY[NVUnits]=y;
				UFileID[NVUnits]=FileID;
				USpriteID[NVUnits]=Sprite;
                UParam1[NVUnits]=Param1;
                UParam2[NVUnits]=Param2;
				OBJS[NVUnits]=OB;
				NVUnits++;
				return;
			};
			un0=un;
			un=un1;
		}while(true);
	};
};
void ShowRLCItemTrans8(int x,int y,lpRLCTable lprt,int n);
void ShowVisual(){
	if(!NVUnits)return;
	int Lx1=smaplx<<Shifter;
	int Ly1=mul3(smaply)<<(Shifter-2);
	SetRLCWindow(smapx,smapy,Lx1,Ly1,SCRSizeX);
	word fu=FirstUN;
	word spr,FID;
	do{
		spr=USpriteID[fu];
		FID=UFileID[fu];
		//Shadows
		OneObject* OB=OBJS[fu];
		if(OB&&OB->Selected&GM(MyNation)){
			if(MiniMode)DrawMiniMarker(OB);
			else DrawMarker(OB);
		};
		GPS.ShowGP(smapx+UnitX[fu],smapy+UnitY[fu],FID,spr,0);
		fu=NURef[fu];
	}while(fu!=0xFFFF);
};
//Param1 values:
//bits 0..2-visibility options
//0-simple
//1-pulsing(Param2-pointer to pulsing data)
//2-transparent(Param2-pointer to transparency data)
//3-encoded with palette(Param2-pointer to palette)
//4-shading with mask(Param2-pointer to gradient data)

void AddToVisual(short uy,short x,short y,OneObject* OB,word FileID,word Sprite,int Options){    
};
void ShowVisualLess(int yend){	
};
bool PInside(int x,int y,int x1,int y1,int xp,int yp);
extern int time1;
extern int time2;
extern int tmtmt;
#ifdef CONQUEST
#define mzoom(x) (LMode?x>>2:x)
#define SCDY 400
#endif
void FireObjectInfo::Erase(){
	free(FireSprite);
	free(FireStage);
};
void FireInfo::Erase(){
	for(int k=0;k<2;k++)
		if(Objs[k]){
			Objs[k]->Erase();
			free(Objs[k]);
		};
};
void ShowProducedShip(OneObject* Port,int CX,int CY);
bool AddSprRefl(int xg,int yg,int xc,int yc,int GP_File,int Sprite,int,int);
void AddPointEx(short XL,short YL,short x,short y,OneObject* OB,word FileID,word SpriteID,word FileIDex,word SpriteIDex,int Param1,int Param2);

#define MinRot 16

//---------------------NEW MOTION ALGORITM----------------------//
// Flags & Variables in OneObject
//  
//   bool CanMove:1
//   bool ImpMotion:1;
//   bool NeedForceCalc:1;
//   byte RotCntr;
//   int ForceX;
//   int ForceY;
void RotateMon(OneObject* OB,char angle){
	//OB->NothingTime=0;
    if(OB->UsesDestDidection){
        OB->DestDirection=OB->RealDir+angle;
        OB->DestDirectionIsActive=true;
    }else{
	    char a0=angle;
	    if(GameSpeed<256){
		    int gs=GameSpeed;
		    if(gs<64)gs=64;
		    angle=angle*gs/256;
		    if(angle==0){
                if(a0<0)angle=-1;
			    else angle=1;
		    }
	    }
	    OB->GraphDir=OB->RealDir;
	    OB->RealDir+=angle;
	    OB->GraphDir+=angle;
	    OB->UnBlockUnit();
        OB->RealDirPrecise=int(OB->RealDir)<<8;	    
    }
};
extern word* MRef;
extern word* MCash;
extern int MaxMCash;
extern int CurMCash;
__forceinline void SetNMSL_FAST(int i,word W){
	int v=i>>SHFCELL;
	int idx=MRef[v];
	if(idx==0xFFFF){
		if(CurMCash>=MaxMCash){
			MaxMCash+=512;
			MCash=(word*)realloc(MCash,MaxMCash<<(SHFCELL+1));
		};
		idx=CurMCash;
		CurMCash++;
		MRef[v]=idx;
	};
	MCash[(idx<<SHFCELL)+(i&((1<<SHFCELL)-1))]=W;
};
DynArray<word> BLD_BUF;
void SetMonstersInCells(){
	BLD_BUF.Check(20000);
	ZMem(MCount,VAL_MAXCX*VAL_MAXCX);
	ZMem(TmpMC,VAL_MAXCX*VAL_MAXCX);
	memset(BLDList,0,VAL_MAXCX*VAL_MAXCX*4);
	CleanNMSL();
	OneObject* OB;
	int ofst,ofst1,k;
	int BLD_BUF_pos=0;
	memset(NPresence,0,VAL_MAXCX*VAL_MAXCX);
	for(int i=0;i<MAXOBJECT;i++){
		OB=Group[i];
		if(OB){
			int ofs=((OB->RealY>>11)<<VAL_SHFCX)+(OB->RealX>>11);
            if(ofs>=0&&ofs<VAL_MAXCIOFS)NPresence[ofs]|=1<<OB->NNUM;
			if(OB->NewBuilding){
				ofst=(OB->RealX>>11)+((OB->RealY>>11)<<VAL_SHFCX)+VAL_MAXCX+1;
				if(ofst>=0&&ofst<VAL_MAXCIOFS){
					if(BLDList[ofst]){
						word* P=BLDList[ofst]+1;
						int q;
						for(q=1;q<7&&P[0]!=0xFFFF;q++,P++);
						if(q<7){
							BLDList[ofst][q]=OB->Index;
							BLDList[ofst][q+1]=0xFFFF;
						}
					}else{  
						if(BLD_BUF_pos>=BLD_BUF.GetAmount())BLD_BUF.Add(0xFFFF,8);
						BLDList[ofst]=&BLD_BUF[BLD_BUF_pos];
						BLDList[ofst][0]=OB->Index;
						BLDList[ofst][1]=0xFFFF;
						BLD_BUF_pos+=8;
					}
				}
			}else{
				if(!OB->Sdoxlo){
					ofst=(OB->GetAttX()>>11)+((OB->GetAttY()>>11)<<VAL_SHFCX)+VAL_MAXCX+1;
					ofst1=ofst<<SHFCELL;
					if(ofst>=0&&ofst<VAL_MAXCIOFS){
						k=MCount[ofst];
						if(k<MAXINCELL-1 && !OB->Sdoxlo){
							MCount[ofst]++;
							TmpMC[ofst]++;
							SetNMSL_FAST(ofst1+k,OB->Index);
						}
					}
				}
			}
		}
	}
	
	for(int i=0;i<MAXOBJECT;i++){
		OB=Group[i];
		if(OB&&OB->Sdoxlo){
			if(!OB->NewBuilding){
				ofst=(OB->RealX>>11)+((OB->RealY>>11)<<VAL_SHFCX)+VAL_MAXCX+1;
				ofst1=ofst<<SHFCELL;
				if(ofst>=0&&ofst<VAL_MAXCX*VAL_MAXCX){
					k=MCount[ofst];
					if(OB->Sdoxlo && k<MAXINCELL-1){
						int acc=MAXINCELL-(OB->Sdoxlo>>2)-1;
						if(acc<64)acc=64;
						if(OB->NewAnm && OB->NewAnm->AnimationType==1)acc=MAXINCELL;
						if(k<acc){
							MCount[ofst]++;
							TmpMC[ofst]++;
							SetNMSL_FAST(ofst1+k,OB->Index);
						}
					}
				}
			}
		}
	}
	void ReRegisterObjects();
	ReRegisterObjects();
}
//checking position for monster MID in  (x,y)
bool CheckPosition(int x,int y,int R,word MID){
	int cx=(x>>11);
	int cy=(y>>11);
	int ofs0=(cx)+((cy)<<VAL_SHFCX);
	int ofs1=ofs0<<SHFCELL;
	for(int dy=0;dy<3;dy++){
		for(int dx=0;dx<3;dx++){
			int NMon=MCount[ofs0];
            if(ofs0<MAXCIOFS){
			    for(int z=0;z<NMon;z++){
			    	word MD=GetNMSL(z+ofs1);
			    	if(MD!=MID){
			    		OneObject* OB=Group[MD];
			    		if(OB&&(!(OB->Sdoxlo||OB->CantRestrictMotion))&&Norma(OB->RealX-x,OB->RealY-y)<R+OB->Radius2)return false;
			    	};
			    };
            };
			ofs0++;
			ofs1+=MAXINCELL;
		};
		ofs0+=VAL_MAXCX-3;
		ofs1+=(VAL_MAXCX-3)<<SHFCELL;
	};
	return true;
};
bool CheckBar(int x,int y,int Lx,int Ly);
bool CheckTerra(int x,int y,int Lx,int Media){
	int xs=(x-((Lx)<<7))>>8;
	int ys=(y-((Lx)<<7))>>8;
	if(MFIELDS[Media].CheckBar(xs,ys,Lx,Lx))return false;
	else return true;
};
int FORCEX;
int FORCEY;

void ChkAnm(OneObject* OB){
	//assert(OB->NewAnm->Enabled);
	//assert(OB->NewState<4);
};
void TryToStand(OneObject* OB,bool rest){
	if(OB->CObjIndex!=0xFFFF)return;
#ifdef COSSACKS2
	//if(rest)rest=(tmtmt&15)==0;
	//no rest for busy formation!!!
	if(rest&&OB->BrigadeID!=0xFFFF){
		//if(CITY[OB->NNUM].Brigs[OB->BrigadeID].BOrder)rest=0;
		if(CITY[OB->NNUM].Brigs[OB->BrigadeID].NewBOrder)
			rest=0;
	};
#endif //COSSACKS2
	rando();
	//OB->StandTime++;
	OB->InMotion=false;
	OB->NothingTime++;
	//OB->RealDir=OB->GraphDir;
	OB->CheckState();
	NewMonster* NMN=OB->newMons;
	if(OB->NewState>NAttTypes)OB->NewState=0;
	if(OB->LocalNewState>NAttTypes)OB->LocalNewState=0;
	OB->UnBlockUnit();	
	if(OB->NewState!=OB->LocalNewState){
		if(OB->LocalNewState!=0){
			int n1=OB->LocalNewState;
			int n2=OB->NewState;
			if(n1&n2){
				NewAnimation* NAT=NMN->GetAnimation(anm_Trans+n1-1+(n2-1)*10);
				if(NAT){
					OB->NewAnm=NAT;
					OB->SetZeroFrame();
					OB->LocalNewState=n2;
					OB->InMotion=false;
					return;                    
				}
			}
			if(n2&&NMN->TransMask[n1-1]&(1<<(n2-1))){
				OB->LocalNewState=n2;
				return;
			}else{
				if(n1==1&&n2==2){
					NewAnimation* T01=NMN->GetAnimation(anm_Trans01);
					if(T01->Enabled){
						OB->NewAnm=T01;
						OB->SetZeroFrame();
						OB->LocalNewState=2;
						OB->InMotion=false;
						//OB->BackMotion=false;
						return;
					};
				}else
				if(n1==2&&n2==1){
					NewAnimation* T10=NMN->GetAnimation(anm_Trans10);
					if(T10->Enabled){
						OB->NewAnm=T10;
						OB->SetZeroFrame();
						OB->LocalNewState=1;
						OB->InMotion=false;
						//OB->BackMotion=false;
						return;
					};
				}else
				if(n2==4){
					if(n1&&NMN->TransXMask&(1<<(n1-1))){
						OB->NewAnm=NMN->GetAnimation(anm_TransX3);
						OB->SetZeroFrame();
						OB->LocalNewState=n2;
						OB->InMotion=false;
						//OB->BackMotion=false;
						return;
					};
				}else
				if(n1==4){
					if(n2&&NMN->TransXMask&(1<<(n2-1))){
						OB->NewAnm=NMN->GetAnimation(anm_Trans3X);
						OB->SetZeroFrame();
						OB->LocalNewState=n2;
						OB->InMotion=false;
						//OB->BackMotion=false;
						return;
					};
				};
			};
			//try to transform in state 0
			NewAnimation* UA=NMN->GetAnimation(anm_UAttack+OB->LocalNewState-1);
			if(UA->Enabled){
				OB->NewAnm=UA;
				OB->SetZeroFrame();
				OB->LocalNewState=0;
				OB->InMotion=false;
				//OB->BackMotion=false;
				return;
			}else{
				OB->NewAnm=NMN->GetAnimation(anm_Stand);
				OB->SetZeroFrame();
				OB->LocalNewState=0;
				OB->InMotion=false;
				//OB->BackMotion=false;
				return;
			};
		}else{
			//now we can transform unit to state NewState
			NewAnimation* PA=NMN->GetAnimation(anm_PAttack+OB->NewState-1);
			if(PA->Enabled){
				OB->NewAnm=PA;
				OB->SetZeroFrame();
				OB->LocalNewState=OB->NewState;
				OB->InMotion=false;
				//OB->BackMotion=false;
				return;
			}else{
				NewAnimation* AT=NMN->GetAnimation(anm_PStand+OB->NewState-1);
				if(AT->Enabled){
					OB->NewAnm=AT;
					OB->SetZeroFrame();
					OB->LocalNewState=OB->NewState;
					OB->InMotion=false;
					//OB->BackMotion=false;
					return;
				}else{
					OB->NewAnm=NMN->GetAnimation(anm_Stand);
					OB->SetZeroFrame();
					OB->LocalNewState=OB->NewState;
					OB->InMotion=false;
					//OB->BackMotion=false;
					return;
				};
			};
		};
	};
	if(OB->NewState){
		NewAnimation* AT=NULL;
		if(OB->NewState==1&&OB->FrameFinished&&rest&&(rando()<128)){
            AT=NMN->GetAnimation(anm_RestA1);
			if(!AT->Enabled)AT=NULL;
		}
		if(!AT)AT=NMN->GetAnimation(anm_PStand+OB->NewState-1);
		if(AT&&AT->Enabled){
			if(AT->Code!=OB->NewAnm->Code || OB->FrameFinished){
				OB->NewAnm=AT;
				OB->SetZeroFrame();
				OB->InMotion=false;
			}
			//OB->BackMotion=false;
			return;
		}else{
			if(OB->MotionStyle!=8){
				OB->NewAnm=NMN->GetAnimation(anm_Stand);
				OB->SetZeroFrame();
				OB->InMotion=false;
				//OB->BackMotion=false;
			}
			return;
		};
	}else{
		//if(rest)
		if(OB->MotionStyle==7||OB->NewAnm&&OB->FrameFinished){
			if(rest&&rando()<128*8){      //Chastota otigrivania rest //GEC
				NewAnimation* RANM[4];				
				int nridx=0;
				RANM[nridx]=NMN->GetAnimation(anm_Rest);
				if(RANM[nridx]->Enabled)nridx++;
				/*
				RANM[nridx]=NMN->GetAnimation(anm_Rest1);
				if(RANM[nridx]->Enabled)nridx++;
				RANM[nridx]=NMN->GetAnimation(anm_Rest2);
				if(RANM[nridx]->Enabled)nridx++;
				RANM[nridx]=NMN->GetAnimation(anm_Rest3);
				if(RANM[nridx]->Enabled)nridx++;
				*/
				if(nridx){
					OB->NewAnm=RANM[rando()%nridx];
				}else{
					OB->NewAnm=NMN->GetAnimation(anm_Stand);
				}
			}else OB->NewAnm=NMN->GetAnimation(anm_Stand);
			OB->SetZeroFrame();
			OB->InMotion=false;
			//OB->BackMotion=false;
		};
		return;
	};
};
void OneObject::CheckState(){
	if(RType<16){
		if(newMons->CompxUnit){
			//NewState=newMons->CompxUnit->GoWithStage[RType];
			GroundState=newMons->CompxUnit->GoWithStage[RType];
			addrand(GroundState+1);
		}else{
			switch(RType){
			case 0://wood
				NewState=2;
				GroundState=2;
				addrand(GroundState+1);
				break;
			case 1:
			case 2:
				NewState=3;
				GroundState=3;
				addrand(GroundState+1);
				break;
			case 3:
				NewState=4;
				GroundState=4;
				addrand(GroundState+1);
			};
		};
	};
};
int GetPrecHeight(int x,int y);
//attempt to move back or forward NewDir
//if(Dirc)attempt to move back
//returns:
//-1 : land is locked there
//1  : motion succesfull
//0  : imposible to move because of other monsters

int TryToMove(OneObject* OB,byte NewDir,bool Dirc){
	return 0;
};
void DamageUnitsInAngle(int x,int y,WeaponInSector* WS,byte Angle,OneObject* Sender,int AttType);
bool Create3DAnmObject(Weapon* Weap,int xs,int ys,int zs,
					                int xd,int yd,int zd,
									OneObject* OB);
void CreateFullPath(int x1,int y1,OneObject* OB);
void AddDestn(int x,int y);
extern int tmtmt;
//extern int RESRC[8][8];

void UnitLight(OneObject* OB);
void CorrectLockPosition(OneObject* OB);

void SWAP(short* x,short* y){
	short t=*x;
	(*x)=(*y);
	*y=t;
};
void SWAP(word* x,word* y){
	word t=*x;
	(*x)=(*y);
	*y=t;
};
void SWAP(int* x,int* y){
	int t=*x;
	(*x)=(*y);
	*y=t;
};
void SWAP(byte*x,byte* y){
	byte t=*x;
	(*x)=(*y);
	*y=t;
};
void SWAP(char* x,char* y){
	char t=*x;
	(*x)=(*y);
	*y=t;
};
void SWAP(NewAnimation** x,NewAnimation** y){
	NewAnimation* t=*x;
	(*x)=(*y);
	*y=t;
};
extern MotionField UnitsField;
void AttackObjLink(OneObject* OBJ);
bool RemoveFoolsInCell(int cell,int x,int y,int Lx,OneObject* MyObj){
	cell+=VAL_MAXCX+1;
	word MyMid=MyObj->Index;
	int mx1=x+Lx-1;
	int my1=y+Lx-1;
    int NMon=MCount[cell];
	if(!NMon)return false;
	int ofs1=cell<<SHFCELL;
	word MID;
	int cx=(x<<1)+Lx;
	int cy=(y<<1)+Lx;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Index!=MyMid&&!(OB->InMotion||OB->BlockInfo)){
				int olx=OB->Lx;
				int ox=((OB->x)<<1)+olx;
				int oy=((OB->y)<<1)+olx;
				int dx=abs(ox-cx);
				int dy=abs(oy-cy);
				if(!(dx>=(olx+Lx)||dy>=(olx+Lx))){
					if(OB->NMask&MyObj->NMask){
						//intersection
						if(OB->StandTime>20&&OB->Lx==MyObj->Lx){
							if(OB->FrameFinished){
								SWAP(&OB->x,&MyObj->x);
								SWAP(&OB->y,&MyObj->y);
								SWAP(&OB->RealX,&MyObj->RealX);
								SWAP(&OB->RealY,&MyObj->RealY);
								SWAP(&OB->RealVx,&MyObj->RealVx);
								SWAP(&OB->RealVy,&MyObj->RealVy);
								SWAP(&OB->RealDir,&MyObj->RealDir);
								SWAP(&OB->GraphDir,&MyObj->GraphDir);
							};
							return true;
						};
						int rx=OB->RealX>>5;
						int ry=OB->RealY>>5;
						UnitsField.BClrBar(OB->x,OB->y,OB->Lx);
						bool NeedRemove=true;
						int lx2=olx<<2;
						for(int r=0;r<8&&NeedRemove;r++){
							char* xi=Rarr[r].xi;
							char* yi=Rarr[r].yi;
							int N=Rarr[r].N;
							for(int j=0;j<N&&NeedRemove;j++){
								int rx1=rx+xi[j];
								int ry1=ry+yi[j];
								int sx=(rx1-lx2)>>3;
								int sy=(ry1-lx2)>>3;
								if(!(UnitsField.CheckBar(sx,sy,olx,olx)||CheckBar(sx,sy,olx,olx))){
									ox=sx+sx+olx;
									oy=sy+sy+olx;
									dx=abs(ox-cx);
									dy=abs(oy-cy);
									if(dx>=(olx+Lx)||dy>=(olx+Lx)){
										OB->x=sx;
										OB->y=sy;
										OB->RealX=rx1<<5;
										OB->RealY=ry1<<5;
										addrand(OB->RealX);addrand(OB->RealY);
										if(!OB->UnlimitedMotion)UnitsField.BSetBar(OB->x,OB->y,OB->Lx);
										NeedRemove=false;
									};
								};
							};
						};
						if(NeedRemove){
							if(!OB->UnlimitedMotion)UnitsField.BSetBar(OB->x,OB->y,OB->Lx);
						};
					}else{
						if(MyObj->LocalOrder&&MyObj->LocalOrder->DoLink==&AttackObjLink&&
							MyObj->LocalOrder->info.MoveToObj.ObjIndex==OB->Index)return true;
						else if(MyObj->EnemyID==0xFFFF)MyObj->AttackObj(OB->Index,128+4,1);
					};
				};
			};
		};
	};
	return false;
};
void RemoveFools(int x,int y,int Lx,OneObject* MyMid){
	if(MyMid&&MyMid->EnemyID==0xFFFF&&MyMid->BrigadeID!=0xFFFF)return;
	int cell=(x>>3)+((y>>3)<<VAL_SHFCX);

	if(RemoveFoolsInCell(cell,x,y,Lx,MyMid))return;

	if(RemoveFoolsInCell(cell-1,x,y,Lx,MyMid))return;
	if(RemoveFoolsInCell(cell+1,x,y,Lx,MyMid))return;
	if(RemoveFoolsInCell(cell-VAL_MAXCX,x,y,Lx,MyMid))return;
	if(RemoveFoolsInCell(cell+VAL_MAXCX,x,y,Lx,MyMid))return;

	if(RemoveFoolsInCell(cell-VAL_MAXCX+1,x,y,Lx,MyMid))return;
	if(RemoveFoolsInCell(cell+VAL_MAXCX+1,x,y,Lx,MyMid))return;;
	if(RemoveFoolsInCell(cell-VAL_MAXCX-1,x,y,Lx,MyMid))return;
	if(RemoveFoolsInCell(cell+VAL_MAXCX-1,x,y,Lx,MyMid))return;
};
int GetPState(int dir){
	int d=((dir-64+8+1024)>>4);
	return d&15;
};

//for slow rotated objects




void CreateOrdersList(OneObject* OB,char* Str){
	char ccc[32];
	Str[0]=0;
	if(OB->LocalOrder){
		Order1* OR1=OB->LocalOrder;
		while(OR1){
			sprintf(ccc," %d",OR1->OrderType);
			strcat(Str,ccc);
			rando();
			OR1=OR1->NextOrder;
		};
	}else{
		strcpy(Str,"No orders.");
	};

};
char scs[16];
char* its1(int i){
	sprintf(scs," %d",i);
	return scs;
};
void AssignHint1(char* s,int time);
int DoLink_Time,SearchVictim_Time,CheckCapture_Time;
//void addrand(int i);
void MotionHandler4(OneObject* OB);
void MotionHandlerOfNewSheeps(OneObject* OB);
void addSpriteAnyway(int x,int y,SprGroup* SG,word id);
extern SprGroup SPECIAL;
extern int LastAddSpr;
extern byte BaloonState;
extern int FogMode;
void EliminateBuilding(OneObject* OB);
extern bool SpriteSuccess;
bool ViewFriendsInScenario=0;
int HUNGPROB=50;
int CheckNtIntegrity(byte NI);
bool CheckObjectForCapture(OneObject* OB);
void KeyTestMem();
void TestCGARB();
extern bool EditMapMode;
void LongProcesses(){
	//TestCGARB();
	void clear_SelPatches();
	clear_SelPatches();
	void InternalProcess();
	InternalProcess();
	void ProcessPluginsSystem();
	ProcessPluginsSystem();
	void ProcessActiveGroupsInGameInterface();
	ProcessActiveGroupsInGameInterface();
	void ProcessNationOfRoads();
	if(rando()<512)ProcessNationOfRoads();
	if(GSets.AllowFormationsStatesProcessing&&!EngSettings.DontUseAgressiveState){
		for(int i=0;i<8;i++){
			City* CT=CITY+i;
			for(int j=0;j<CT->NBrigs;j++){
				Brigade* BR=CT->Brigs+j;
				if(BR->Enabled&&BR->WarType){
					word* IDS=BR->Memb;
					word* SNS=BR->MembSN;
					int bstate=0;					
					bool stg=0;
					for(int j=NBPERSONAL;j<BR->NMemb;j++){
						word MID=IDS[j];
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&OB->Serial==SNS[j]){
                                bstate=OB->ActivityState;
								stg=OB->StandGround;
								break;
							}
						}
					}
					void CancelStandGroundAnyway(Brigade* BR);
					if(stg&&bstate!=1)CancelStandGroundAnyway(BR);
					if(bstate==2){
						if(!BR->AttEnm){
							BrigadeOrder* BO=BR->BOrder;
							while(BO){
                                BO->Prio=128;
								BO=BO->Next;
							}							
						}
						BR->AttEnm=1;
					}
					if(bstate==1){
						if(!BR->AttEnm){
							BrigadeOrder* BO=BR->BOrder;
							while(BO){
								BO->Prio=142;
								BO=BO->Next;
							}							
						}
						BR->AttEnm=0;
					}
					for(int j=0;j<BR->NMemb;j++){
						word MID=IDS[j];
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&OB->Serial==SNS[j]){
								OB->ActivityState=bstate;								
							}
						}
					}
				}
			}
		}
	}
	void ProcessTerramorph();
	ProcessTerramorph();
	int T0=GetTickCount();
	RunPF(1024+2,"LongProcess");
	int d=tmtmt&3;
	int d1=tmtmt&15;
	InternalProcess();
	
//	StopPF(10);
	SearchVictim_Time=GetTickCount()-T0;
	T0=GetTickCount();
	//RunPF(11);
	bool LIGHT=FogMode&&BaloonState!=1&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision);
	byte Mask=NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NMask;
	byte MyNT=GSets.CGame.cgi_NatRefTBL[MyNation];
	bool sce=!((SCENINF.hLib==NULL)||ViewFriendsInScenario/*||SCENINF.StandartVictory*/);
	RunPF(256*5+23,"DoLink");
	for(int i=0;i<8;i++){
		CITY[i].TheyMovesInto_prev.FastClear();
		CITY[i].MoversIndices_prev.FastClear();
		CITY[i].TheyMovesInto_prev.AddValues(CITY[i].TheyMovesInto.GetValues(),CITY[i].TheyMovesInto.GetAmount());
		CITY[i].MoversIndices_prev.AddValues(CITY[i].MoversIndices.GetValues(),CITY[i].TheyMovesInto.GetAmount());
		CITY[i].TheyMovesInto.FastClear();
		CITY[i].MoversIndices.FastClear();
	}
	int ia=0;
    int ic=tmtmt&7;
    CITY[ic].StartEnumUnits();
	for(int i=0;i<8;i++){
		int N=NtNUnits[i];
		if(N){
            void OpenAllGates(byte NI,bool State);
			OpenAllGates(i,1);
			for(int j=0;j<NtNUnits[i];j++){
				//assert(units[j]<8192);
				//addrand(CheckNtIntegrity(i));
				word UMID=NatList[i][j];
                if(j<NtNUnits[i]-3){
                    DWORD adr=DWORD(Group[NatList[i][j+2]]);
                    if(adr){
                        __asm{
                            mov edx,adr
                            prefetchnta [edx]
                            prefetchnta [edx+64]
                            prefetchnta [edx+128]
                            prefetchnta [edx+192]
                            prefetchnta [edx+256]
                        }
                    }
                }
				//NL++;
				if(UMID!=0xFFFF){
					OneObject* OB=Group[UMID];
					addrand(UMID);
					addrand(j);
					if(OB && !OB->Disabled){
						if((ia&7)==ic){
							//if(OB->newMons->Ability){
							//	OB->newMons->Ability->Process(i);
							//}
							if(OB->ActiveAbility){
								OB->ActiveAbility->Process();
							}
						}
						ia++;
                        if(i==ic)CITY[i].EnumOneUnit(OB);
						addname(OB->Index);
						addrand(OB->NIndex);
                        OB->WasDrawn=false;
                        OB->IsInFog=false;
						if(!OB->Sdoxlo){
							addrand(OB->RealDir);
							addrand(OB->RealX);
							addrand(OB->RealY);
							if(OB->InFire&&OB->Life){
								if(OB->NewBuilding){
									int DL=OB->MaxLife>>11;
									if(DL<1)DL=1;
									if(OB->Life)OB->MakeDamage(DL,DL,NULL,255);
								}else{
									if(!(tmtmt&31)){
										if(!(OB->LockType||OB->newMons->Artilery)){
											int MIN=OB->MaxLife/10;
											if(OB->Life>MIN)OB->MakeDamage(1,1,NULL,255);
										};
									};
								};
							};
							/*if(OB->ActivityState==1&&OB->StandTime>10){//peaceful //GEC- GecOptim zakomentirova 29.05.06 ibo ne znal zachem eto nado voobshe!!!
								if(OB->BrigadeID!=0xFFFF){
									if(!OB->StandGround){
										Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;	
										void MakeStandGroundTemp(Brigade* BR);
										MakeStandGroundTemp(BR);
									}
								}else OB->StandGround=1;
							}*/
							/*if(OB->NewState!=5&&OB->FrameFinished&&OB->StandGround&&OB->BrigadeID!=0xFFFF&&OB->newMons->SitInFormations&&OB->BrIndex>=NBPERSONAL){ //GEC- GecOptim zakomentirova 29.05.06 ibo ne znal zachem eto nado voobshe!!!
                                Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
								if(BR->InStandGround&&BR->WarType){
									OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
									if(ODS->Opt[OB->BrIndex-NBPERSONAL]==1){
										//OB->GroundState=5;
										if(!(OB->LocalOrder||OB->delay)){
											OB->NewState=5;
											TryToStand(OB,false);
										}
									}
								}
							}*/
							/*if(OB->ActivityState==2){//aggressive //GEC- GecOptim ubral ibo eto iz alexandra
								if(OB->BrigadeID==0xFFFF){
									OB->StandGround=0;
								}else{
									if(OB->StandGround){
										Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
										void CancelStandGroundAnyway(Brigade* BR);
										CancelStandGroundAnyway(BR);
									}
								}
							}*/
							/*if(OB->Nat->AI_Enabled){
								int use=OB->newMons->Usage;
								if(use!=TowerID){
									if((i&63)==d)OB->SearchVictim();
								}else{
									OB->SearchVictim();
								};
							}else{*/
								if(!OB->Attack && OB->PrioryLevel<15){//GEC- GecOptim pomenial mestami proverku (OB->PrioryLevel<15 && !OB->Attack )
									//int use=OB->newMons->Usage;
									//if(use==PeasantID){
									//	if((i&63)==d1)OB->SearchVictim();
									//}else{
										if(OB->BrigadeID==0xFFFF){											
											if(OB->Excited){
                                                if(rando()<10000){
                                                    OB->SearchVictim();
												    if(rando()<512 || !OB->Attack){
													    OB->Excited=false;
												    }
                                                }
											}else if(/*OB->InMotion || */(rando()<3000))OB->SearchVictim();
										}else if(((i&15)==d))OB->SearchVictim();
									//}
								}
							//};
							int mm=i&31;
							if((tmtmt&31)==mm&&(CheckObjectForCapture(OB)||!(OB->Ready||OB->LocalOrder)))CheckCapture(OB);
							addrand(OB->RealX);
							addrand(OB->RealDir);
							addrand(OB->RealY);
							addname(OB->Index);
							addname(OB->BrIndex);
							addname(OB->BrigadeID);
							Order1* OR1=OB->LocalOrder;
							//int orr=-1;
							//addrand(CheckNtIntegrity(i));
							void CheckAllLinks();
							if(OR1){
								//Order1 ORR=*OR1;
								addname(OB->Index);
								//assert(OR1->DoLink);
#ifdef  CHECK_SYNC_FAILURE
								int GetOrderKind(ReportFn* RF);
								addrand(GetOrderKind(OR1->DoLink));
#endif //CHECK_SYNC_FAILURE
								OR1->DoLink(OB);
								addrand(OB->LocalOrder!=NULL);
								if(OB->Attack&&OB->StandTime>80&&OB->FrameFinished){//==64+(OB->Index&31)){
									OB->StandTime=0;
									void GotoFinePosition1(OneObject*);
									GotoFinePosition1(OB);
								}
							}else{
								//addrand(OB->Index+40000);
								OB->PrioryLevel=0;
								if(OB->StandTime==48){
									addrand(OB->Index+50000);
									GotoFinePosition(OB);
								};
							};
							//addrand(CheckNtIntegrity(i));
							addname(OB->Index);
							addrand(OB->RealDir);
							//addrand(CheckNtIntegrity(i));
							if(OB->NewBuilding){
								int DT=(GameSpeed+((tmtmt&15)<<4))>>8;
								if(OB->delay>=DT)OB->delay-=DT;
								else OB->delay=0;
								if(OB->TempDelay){
									int N=OB->newMons->NShotPt;
									for(int i=0;i<N;i++){
										if(OB->TempDelay[i]>GameSpeed){
											OB->TempDelay[i]-=GameSpeed;
										}else OB->TempDelay[i]=0;
									};
								};
							}else{
								switch(OB->MotionStyle){
								case 0:									
									break;
								case 1:									
									break;
								case 2://ships									
									//PerformMotion2(OB);
									//addrand(OB->Index+10000);
									break;
								case 3:									
									break;
								case 4:									
									break;
								case 5:
									MotionHandlerOfNewSheeps(OB);
									break;
#ifdef COSSACKS2
								case 6:
									void MotionHandlerForComplexObjects(OneObject* OB);
									MotionHandlerForComplexObjects(OB);
									break;
								case 7:
									void MotionHandlerForSingleStepObjects(OneObject* OB);
									MotionHandlerForSingleStepObjects(OB);
									break;
								case 8:
									void MotionHandlerForFlyingObjects(OneObject* OB);
									MotionHandlerForFlyingObjects(OB);
                                    break;
#endif //COSSACKS2
								};
							};
							//addrand(CheckNtIntegrity(i));
							if(LIGHT){
								//if(sce){
								//	if(OB->NNUM==MyNT)UnitLight(OB);
								//}else 
								if(OB->NMask&Mask)UnitLight(OB);
							};
							NewMonster* NM=OB->newMons;
								
								int DT=(GameSpeed+((tmtmt&15)<<4))>>8;
								if(OB->delay>=DT)OB->delay-=DT;
								else OB->delay=0;
							//if(!NM->SlowRecharge){//GEC- GecOptim Ubrano za nenadobnostiu sviazano s nignei strokoi
							/*if(!NM->SlowRecharge){//GEC- GecOptim Ubrano za nenadobnostiu sviazano s nignei strokoi
								int DT=(GameSpeed+((tmtmt&15)<<4))>>8;
								if(OB->delay>=DT)OB->delay-=DT;
							else OB->delay=0;//GEC- GecOptim Ubrano za nenadobnostiu sviazano s verhnei strokoi
							}else{
								if(OB->delay){
									if(OB->StandTime>20&&!OB->LocalOrder){
										int cf=OB->NewCurSprite>>SpeedSh;
										int nf=(OB->NewAnm->NFrames-1)>>SpeedSh;
										if(nf==cf){
											if(OB->NewState!=4){
												bool CheckShooterAbilityToRecharge(OneObject* OBJ);
												if(CheckShooterAbilityToRecharge(OB)){
													OB->NewState=4;
													TryToStand(OB,0);
												}
											}else{
												NewAnimation* ANM=NM->GetAnimation(anm_Attack+3);
												if(OB->NewAnm!=ANM){
													if(OB->delay>ANM->NFrames)OB->delay-=ANM->NFrames;
													else OB->delay=0;
													OB->NewAnm=ANM;
													OB->SetZeroFrame();
													OB->InMotion=0;
													OB->BackMotion=0;
												};
											};
										};
									};
								}else{
									if(OB->NewState==4){
										OB->NewState=OB->GroundState;
										addrand(OB->NewState+1);
									};
								};
							};*/
							//addrand(CheckNtIntegrity(i));
							word HARCH=OB->Nat->Harch;
							if(!(HARCH||OB->NewBuilding||rando()>HUNGPROB||EngSettings.DontDieIfHungry||DetectHero(OB)||EditMapMode)){
								if(!OB->newMons->NotHungry){
									if(OB->NNUM==MyNation)AssignHint1(VERYHUN,50,32);//"�� �� ���. ��..."
									OB->Die();
									OB->Nat->CITY->LastGolodTime=tmtmt;
								};
							};
							if(OB->NInside&&NM->ShowInsideUnits){
                                int SIN=TSin[byte(OB->RealDir+64)];
								int COS=TCos[byte(OB->RealDir+64)];
								for(int i=0;i<OB->NInside;i++){
                                    word ID=OB->Inside[i];
									if(ID!=0xFFFF){
										OneObject* OBJ=Group[ID];										
										void NewMonsterPreciseSendToLink(OneObject* OB);
										if(OBJ&&OBJ->LockedInBuilding!=0xFFFF){
											if(OBJ->CObjIndex!=0xFFFF){
												void SetPositionForCOB(OneObject* OB,int x,int y,int Dir);
												OBJ->UnlimitedMotion=true;
												SetPositionForCOB(OBJ,OB->RealX/16,OB->RealY/16,OB->RealDir+128);
											}else{
												int px=i;
												if(px>=NM->PosPtX.GetAmount())px=NM->PosPtX.GetAmount()-1;
												int x1=OB->RealX+(NM->PosPtX[px]*COS-NM->PosPtY[px]*SIN)/16;
												int y1=OB->RealY+(NM->PosPtX[px]*SIN+NM->PosPtY[px]*COS)/16;
												if(OBJ->newMons->NPlacesInTransport>80){
													x1=OB->RealX;
													y1=OB->RealY;
												}
												int r=Norma(OBJ->RealX-x1,OBJ->RealY-y1);
												if(r>4*16&&OBJ->UnlimitedMotion){
													//OBJ->ClearOrders();
													OBJ->UnlimitedMotion=true;
													OBJ->DestX=x1;
													OBJ->DestY=y1;
												}else{
													OBJ->UnlimitedMotion=false;
													OBJ->DestX=-1;
													OBJ->DestY=-1;
													if(OBJ->RealX!=x1||OBJ->RealY!=y1){
														void ChangeUnitCoor(OneObject* OB,int newX,int newY);
														addrand(x1);
														addrand(y1);
														ChangeUnitCoor(OBJ,x1,y1);
													}
													if(!OBJ->Attack){
														if(NM->BornPtX.GetAmount()){
															x1-=OB->RealX+(NM->BornPtX[0]*COS-NM->BornPtY[0]*SIN)/16;
															y1-=OB->RealY+(NM->BornPtX[0]*SIN+NM->BornPtY[0]*COS)/16;
															OBJ->RealDir=GetDir(x1,y1);
															OBJ->RealDirPrecise=int(OBJ->RealDir)<<8;
															OBJ->GraphDir=OBJ->RealDir;
														}else{
															OBJ->RealDir=OB->RealDir;
															OBJ->RealDirPrecise=int(OBJ->RealDir)<<8;
															OBJ->GraphDir=OBJ->RealDir;
														}
													}
												}												
											}											
										}
									}
								}
							}
							if(OB->NewBuilding){
								int HLife=OB->MaxLife>>1;
								if(OB->Life<HLife){									
									bool DoFog=false;
									HLife>>=1;
									int Prob=NM->Fogging.WProb;
									if(OB->Life<HLife){
										if(rando()<Prob)DoFog=true;
									}else{
										Prob>>=1;
										if(rando()<Prob)DoFog=true;
									};
									if(DoFog&&NM->Fogging.NWeap&&OB->Stage==OB->NStages){
										Prob=(rando()*NM->Fogging.NWeap)>>15;
										Weapon* WP=WPLIST[NM->Fogging.Weap[Prob]];
										Prob=(rando()*NM->DamPtX.GetAmount())>>15;
										int xx,yy;
										OB->GetCornerXY(&xx,&yy);
										if(NM->DamPtX.GetAmount()){
											xx+=NM->DamPtX[Prob];
											yy+=NM->DamPtY[Prob];
										};
										xx<<=4;
										yy<<=4;
										Create3DAnmObject(WP,xx,yy,GetHeight(xx,yy)+4,xx,yy,-1000,NULL);
									};
								};
							};
							if(OB->NewBuilding&&OB->DstX&&OB->ImSelected&GM(MyNation))AddDestn(OB->DstX>>4,OB->DstY>>4);
							//addrand(CheckNtIntegrity(i));
						}else if(!OB->Hidden){			
                            if(OB->LocalOrder && OB->LocalOrder->OrderType==0x37){//hack to process specific orders after death
                                OB->LocalOrder->DoLink(OB);
                            }
							//death processing
							NewMonster* NMN=OB->newMons;
							if(OB->MotionStyle==8){
								void MotionHandlerForFlyingObjects(OneObject* OB);
								MotionHandlerForFlyingObjects(OB);								
							}
							if(OB->NewBuilding){
								OB->SdoxloLow+=GameSpeed;
								OB->Sdoxlo+=OB->SdoxloLow>>8;
								OB->SdoxloLow&=255;
								//if(!OB->newMons->LongDeath){
								//	if(OB->Sdoxlo>1000){
								//		OB->LoLayer=NMN->GetAnimation(anm_DeathLie2);
								//	};
								//}
								if(OB->Sdoxlo>800){
									EliminateBuilding(OB);
									DelObject(OB);
									Group[OB->Index]=NULL;
								};
							}else{
								NewAnimation* DEATH=NMN->GetAnimation(anm_Death);
								if(DEATH->Enabled){
									if(OB->Sdoxlo==1){
										OB->NewAnm=DEATH;
										OB->SetZeroFrame();
									};
									if(OB->NewAnm->Code==anm_Death){
										OB->SetNextFrame();
										if(OB->NewCurSprite>=DEATH->NFrames-FrmDec){
											if(OB->newMons->MeatTransformIndex!=0xFF){
												addSpriteAnyway(OB->RealX>>4,OB->RealY>>4,&SPECIAL,OB->newMons->MeatTransformIndex);
												if(SpriteSuccess){
													OneSprite* OS=Sprites+LastAddSpr;
													if(OS->Enabled){
														OS->NIndex=OB->NIndex;
														OS->Direction=OB->RealDir;
													};
												};
												OB->DeletePath();
												OB->ClearOrders();
												DelObject(OB);
												Group[OB->Index]=NULL;
												OB=NULL;//died.	
											}											
										}
										if(OB){
											//OB->SdoxloLow+=GameSpeed;
											//OB->Sdoxlo+=OB->SdoxloLow>>8;
											//OB->SdoxloLow&=255;
											if(OB->LockType==1){
												if(OB->Sdoxlo>4000)OB->Sdoxlo=4000;
												if(OB->Sdoxlo>100){
													if((!OB->NewAnm)||OB->FrameFinished){
														OB->DeletePath();
														OB->ClearOrders();
														DelObject(OB);
														Group[OB->Index]=NULL;
														OB=NULL;//died.
													}
												}
											}else{
												if(OB->Sdoxlo>4000){
													NewAnimation* DEATHL2=NMN->GetAnimation(anm_DeathLie2);
													if(DEATHL2->Enabled){
														OB->NewAnm=DEATHL2;
														OB->SetZeroFrame();
													}else{
														OB->DeletePath();
														OB->ClearOrders();
														DelObject(OB);
														Group[OB->Index]=NULL;
														OB=NULL;//died.
													}
												}
											}
										}
									}
									if(OB){
										OB->SdoxloLow+=GameSpeed;
										OB->Sdoxlo+=OB->SdoxloLow>>8;
										OB->SdoxloLow&=255;
										if(OB->Sdoxlo>4000){											
											OB->DeletePath();
											OB->ClearOrders();
											DelObject(OB);
											Group[OB->Index]=NULL;
											OB=NULL;//died.
										}
									}
								}else{
									OB->DeletePath();
									OB->ClearOrders();
									DelObject(OB);
									Group[OB->Index]=NULL;
									OB=NULL;//died.                                    
								}
							}
						};
					};
				};				
			};
			void OpenAllGates(byte NI,bool State);
			OpenAllGates(i,0);
		};
	};
    CITY[ic].PostEnumUnits();
	void AutoSendFreeUnitsToHome();
	AutoSendFreeUnitsToHome();
	void PerformPathFiding();
	if(NOPAUSE)PerformPathFiding();
	StopPF(256*5+23);
//	StopPF(11);
	//RunPF(12);
	DoLink_Time=GetTickCount()-T0;
	T0=GetTickCount();
	StopPF(1024+2);
	CheckCapture_Time=GetTickCount()-T0;
	addrand(3321);
	GPS.EnableZBuffer(0);
	InternalProcess();
};
void CheckArmies(City*);

void addSpriteAnyway(int x,int y,SprGroup* SG,word id);
extern SprGroup SPECIAL;
extern bool SpriteSuccess;
extern int LastAddSpr;
extern byte BaloonState;
extern int FogMode;
void UnitAbilityTest();
void CalculateMotion(){
	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
    //UnitAbilityTest();
	LongProcesses();
    //UnitAbilityTest();
	//CheckUnits();
	_clearfp();
	_controlfp(_PC_64|_RC_NEAR|_DN_SAVE,_MCW_PC|_MCW_RC|_MCW_DN);
	void CalculateCollision();
    //UnitAbilityTest();
	CalculateCollision();
    //UnitAbilityTest();
};


//--------------------------------------------------------------//
int RoundX(int x){
	return x;//(x>>8)<<8;
};
int RoundY(int y){
	return (div(y,8*16).quot)*(8*16);
};
void BSetPt(int x,int y);
void CreateAveragePlane(int x,int y,int r);
extern word LastObject;
bool CheckSpritesInArea(int x,int y,int r);
bool CheckBuildingsInArea(int x0,int y0,int x1,int y1,word* BLD,int Nb,bool Mine){
	if(x0==x1&&y0==y1)return true;	
	int nx0=x0+y0;
	int ny0=y0-x0;
	int nx1=x1+y1;
	int ny1=y1-x1;
	int nxc=(nx0+nx1)>>1;
	int nyc=(ny0+ny1)>>1;
	int Rx=abs(nx1-nx0)>>1;
	int Ry=abs(ny1-ny0)>>1;
	int xc=(x0+x1)>>1;
	int yc=(y0+y1)>>1;
	if(Mine){
		for(int i=0;i<Nb;i++){
			OneObject* OB=Group[BLD[i]];
			if(OB){
				NewMonster* NM=OB->newMons;
				if(/*NM->Building&&*/NM->ProdType){
					if(Norma(OB->RealX-xc,OB->RealY-yc)<16*120)return false;
				};
			};
		};
	}else{
		for(int i=0;i<Nb;i++){
			OneObject* OB=Group[BLD[i]];
			if(OB){
				NewMonster* NM=OB->newMons;
				//if(NM->Building){
					int bx0=OB->RealX+(NM->BuildX0<<4);
					int by0=OB->RealY+(NM->BuildY0<<4);
					int bx1=OB->RealX+(NM->BuildX1<<4);
					int by1=OB->RealY+(NM->BuildY1<<4);
					if(bx0!=bx1||by0!=by1){
						int bnx0=bx0+by0;
						int bny0=by0-bx0;
						int bnx1=bx1+by1;
						int bny1=by1-bx1;
						int bnxc=(bnx0+bnx1)>>1;
						int bnyc=(bny0+bny1)>>1;
						int bRx=abs(bnx1-bnx0)>>1;
						int bRy=abs(bny1-bny0)>>1;
						if(abs(bnxc-nxc)<bRx+Rx&&abs(bnyc-nyc)<bRy+Ry)return false;
					};
				//};
			};
		};
	};
	return true;
};
#ifdef _USE3D
int GetNRoads();
void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
int GetPointToLineDist(int x,int y,int x1,int y1,int x2,int y2);
int GetKnotNeighbours(int Zone,word* Buf);
bool CheckPointInCurve(int x,int y,int* xi,int* yi,int N){
	int NN=0;
	for(int i=0;i<N;i++){
		int x0=xi[i];
		int y0=yi[i];
		int x1=i<N-1?xi[i+1]:xi[0];
		int y1=i<N-1?yi[i+1]:yi[0];
		if(x1<x0){
			swap(x0,x1);
			swap(y0,y1);
		}
		if(x>=x0&&x<x1){
            int yy=y0+(y1-y0)*(x-x0)/(x1-x0);
			if(yy<y)NN++;
		}
	}
	return NN&1;
}
CEXPORT void AddXYPulse(int x,int y);
bool CheckRoadsInArea(int x0,int y0,int x1,int y1){
	if(EngSettings.DontUseRoads)return true;
	if(x0==x1&&y0==y1)return true;	
	x0>>=4;
	y0>>=4;
	x1>>=4;
	y1>>=4;
	int nx0=x0+y0;
	int ny0=y0-x0;
	int nx1=x1+y1;
	int ny1=y1-x1;
	int nxc=(nx0+nx1)>>1;
	int nyc=(ny0+ny1)>>1;
	int Rx=abs(nx1-nx0)>>1;
	int Ry=abs(ny1-ny0)>>1;
	int xc=(x0+x1)>>1;
	int yc=(y0+y1)>>1;
	int NR=GetNRoads();
	int dst=30;
	int XED[4];
	int YED[4];

	x0-=32;
	x1+=32;
	
	XED[0]=x0;
	YED[0]=y0;
	XED[1]=(x0+x1+y0-y1)/2;
	YED[1]=(y0+y1+x0-x1)/2;
	XED[2]=x1;
	YED[2]=y1;
	XED[3]=(x0+x1+y1-y0)/2;
	YED[3]=(y0+y1+x1-x0)/2;

	int XED2[8];
	int YED2[8];
	int ddd=64;

	XED2[0]=XED[0];
	YED2[0]=YED[0]+ddd;
	XED2[1]=XED[0];
	YED2[1]=YED[0]-ddd;
	XED2[2]=XED[1]-ddd;
	YED2[2]=YED[1];
	XED2[3]=XED[1]+ddd;
	YED2[3]=YED[1];
	XED2[4]=XED[2];
	YED2[4]=YED[2]-ddd;
	XED2[5]=XED[2];
	YED2[5]=YED[2]+ddd;
	XED2[6]=XED[3]+ddd;
	YED2[6]=YED[3];
	XED2[7]=XED[3]-ddd;
	YED2[7]=YED[3];
	
	/*
	void DrawWorldLine(float x0,float y0,float x1,float y1,DWORD Diffuse);
	for(int i=0;i<8;i++){
		int ii=(i+1)&7;
		DrawWorldLine(XED2[i],YED2[i],XED2[ii],YED2[ii],0xFFFFFF00);
	}	
	*/
	for(int i=0;i<NR;i++){
		word  buf[16];
		int rx,ry;
		GetPreciseTopCenter(i,0,rx,ry);
		if(Norma(rx-(x0+x1)/2,ry-(y0+y1)/2)<1000){
			if(CheckPointInCurve(rx,ry,XED2,YED2,8)){
				return false;
				/*
				int nz=GetKnotNeighbours(i,buf);
				for(int j=0;j<nz;j++){
					int Z=buf[j];
					if(Z>i){
						bool good=0;
						int nx,ny;
						GetPreciseTopCenter(Z,0,nx,ny);
						if(GetPointToLineDist((x0+x1)/2,(y0+y1)/2,rx,ry,nx,ny)>(dst+dst)){
							if(GetPointToLineDist(x0,y0,rx,ry,nx,ny)>dst){
								if(GetPointToLineDist(x1,y1,rx,ry,nx,ny)>dst){
									int xx=(x0+x1+y0-y1)/2;
									int yy=(y0+y1+x0-x1)/2;
									if(GetPointToLineDist(xx,yy,rx,ry,nx,ny)>dst){
										xx=(x0+x1+y1-y0)/2;
										yy=(y0+y1+x1-x0)/2;
										if(GetPointToLineDist(xx,yy,rx,ry,nx,ny)>dst){										
											good=1;										
										}                            
									}
								}
							}
						}

						
						if(!good){
							return false;
						}
					}
				}
				*/
			}
		}
	}		
	return true;
};
#endif
word GetOneBld(int cell,int pos=0);
bool CheckBuildingsForWalls(int x,int y){
	int nr=4;
    int nr1=nr+nr+1;
    int cx=(x>>11);
	int cy=(y>>11);

	int nxc=x+y;
	int nyc=y-x;

	int ofs0=(cx-nr)+((cy-nr)<<VAL_SHFCX)+VAL_MAXCX+1;
	int ofs1=ofs0<<SHFCELL;
	for(int dy=0;dy<nr1;dy++){
		for(int dx=0;dx<nr1;dx++){
            if(ofs0>=0&&ofs0<MAXCIOFS){
			    word Mon1=GetOneBld(ofs0);
				if(Mon1!=0xFFFF){
					OneObject* OB=Group[Mon1];
					if(OB){
						NewMonster* NM=OB->newMons;
						if(NM->Building){
							int bx0=OB->RealX+(NM->BuildX0<<4);
							int by0=OB->RealY+(NM->BuildY0<<4);
							int bx1=OB->RealX+(NM->BuildX1<<4);
							int by1=OB->RealY+(NM->BuildY1<<4);
							if(bx0!=bx1||by0!=by1){
								int bnx0=bx0+by0;
								int bny0=by0-bx0;
								int bnx1=bx1+by1;
								int bny1=by1-bx1;
								int bnxc=(bnx0+bnx1)>>1;
								int bnyc=(bny0+bny1)>>1;
								int bRx=abs(bnx1-bnx0)>>1;
								int bRy=abs(bny1-bny0)>>1;
								if(abs(bnxc-nxc)<bRx+2048&&abs(bnyc-nyc)<bRy+2048)return false;
							};
						};
					};

				};
            };
			ofs0++;
			ofs1+=MAXINCELL;
		};
		ofs0+=VAL_MAXCX-nr1;
		ofs1+=(VAL_MAXCX-nr1)<<SHFCELL;
	};
    return true;
};
bool CheckMonstersInArea(byte Mask,int x,int y,int r){
    int nr=(r>>11)+1;
    int nr1=nr+nr+1;
    int cx=(x>>11);
	int cy=(y>>11);
	int ofs0=(cx-nr)+((cy-nr)<<VAL_SHFCX);
	int ofs1=ofs0<<SHFCELL;
	for(int dy=0;dy<nr1;dy++){
		for(int dx=0;dx<nr1;dx++){
            if(ofs0>=0&&ofs0<MAXCIOFS){
			    int NMon=MCount[ofs0];
		     	for(int z=0;z<NMon;z++){
		    		word MD=GetNMSL(z+ofs1);
	    			OneObject* OB=Group[MD];
	    			if(OB&&(!OB->Sdoxlo)&&OB->NewMonst&&(!(OB->NMask&Mask||OB->NNUM==7))&&Norma(OB->RealX-x,OB->RealY-y)<r)return false;
		    	};
            };
			ofs0++;
			ofs1+=MAXINCELL;
		};
		ofs0+=VAL_MAXCX-nr1;
		ofs1+=(VAL_MAXCX-nr1)<<SHFCELL;
	};
    return true;
};
void ChangeUnitCoor(OneObject* OB,int newX,int newY);
void FindPositionForUnit(OneObject* OB){
	if(OB){
		MotionField* MF=MFIELDS+OB->LockType;
		if(MF->CheckBar(OB->x-1,OB->y-1,3,3)){
			int x0=OB->x;
			int y0=OB->y;
			for(int rr=0;rr<60;rr++){
				char* xi=Rarr[rr].xi;
				char* yi=Rarr[rr].yi;
				int N=Rarr[rr].N;
				for(int j=0;j<N;j++){
					int xx=x0+xi[j];
					int yy=y0+yi[j];
					if(!MF->CheckBar(xx-1,yy-1,3,3)){
						ChangeUnitCoor(OB,(xx<<8)+128,(yy<<8)+128);
						addrand(OB->RealX);addrand(OB->RealY);
						OB->DestX=-1;
						OB->DestY=-1;
						if(OB->PathX)FastFreeMem(OB->PathX);
						if(OB->PathY)FastFreeMem(OB->PathY);
						OB->PathX=NULL;
						OB->PathY=NULL;
						OB->NIPoints=0;
						rr=60;
						j=N;
						OB->CPdestX=-1;
						OB->CPdestY=-1;
					};
				};
			};
		}
	}
};
void GetAwayMonstersInArea(int x,int y,int r){
	SetMonstersInCells();
    int nr=(r>>11)+1;
    int nr1=nr+nr+1;
    int cx=(x>>11);
	int cy=(y>>11);
	int ofs0=(cx-nr)+((cy-nr)<<VAL_SHFCX);
	int ofs1=ofs0<<SHFCELL;
	for(int dy=0;dy<nr1;dy++){
		for(int dx=0;dx<nr1;dx++){
            if(ofs0>=0&&ofs0<MAXCIOFS){
			    int NMon=MCount[ofs0];
		     	for(int z=0;z<NMon;z++){
		    		word MD=GetNMSL(z+ofs1);
	    			OneObject* OB=Group[MD];
					if(OB&&(OB->LockedInBuilding==0xFFFF)&&OB->NewMonst&&Norma(OB->RealX-x,OB->RealY-y)<r){
						if(CheckBar(OB->x-2,OB->y-2,5,5)){
							int x0=OB->x;
							int y0=OB->y;
							for(int rr=0;rr<60;rr++){
								char* xi=Rarr[rr].xi;
								char* yi=Rarr[rr].yi;
								int N=Rarr[rr].N;
								for(int j=0;j<N;j++){
									int xx=x0+xi[j];
									int yy=y0+yi[j];
									if(!CheckBar(xx-1,yy-1,3,3)){
										OB->RealX=(xx<<8)+128;
										OB->RealY=(yy<<8)+128;
										addrand(OB->RealX);addrand(OB->RealY);
										OB->x=xx;
										OB->y=yy;
										OB->DestX=-1;
										OB->DestY=-1;
										if(OB->PathX)FastFreeMem(OB->PathX);
										if(OB->PathY)FastFreeMem(OB->PathY);
										OB->PathX=NULL;
										OB->PathY=NULL;
										OB->NIPoints=0;
										rr=60;
										j=N;
										OB->CPdestX=-1;
										OB->CPdestY=-1;
									};
								};
							};
						};
					};
		    	};
            };
			ofs0++;
			ofs1+=MAXINCELL;
		};
		ofs0+=VAL_MAXCX-nr1;
		ofs1+=(VAL_MAXCX-nr1)<<SHFCELL;
	};
};
void GetAwayAllUnitsFromLockpoints(){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Sdoxlo||OB->LockedInBuilding!=0xFFFF||OB->NewBuilding
					||OB->GLock||OB->UnlimitedMotion||OB->CObjIndex!=0xFFFF||OB->LockType==1||OB->BlockInfo)){			
			FindPositionForUnit(OB);
		}
	}
}
int CheckMinePosition(int id,int* xi,int* yi,int r,bool Manual=true);
bool FindPortPlace(NewMonster* NM,int x,int y,int* BuiX,int* BuiY);
int CheckVLine(int x,int y,int Lx);
int CheckHLine(int x,int y,int Lx);
bool CheckSpritesInAreaNew(int x,int y,int r,bool Erase);
void EraseTreesInPoint(int x,int y);
int CreateBLDList(byte NI,word* BLD,int MaxBLD,int x0,int y0){
	int N=0;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding){
			if(N<MaxBLD){
				BLD[N]=i;
				N++;
			};
		};
	};
	return N;
};
int CheckCreationAbility(byte NI,int UType,int* x2i,int* y2i,word* BLD,int NBLD,bool Manual){	
	NewMonster* NM=NATIONS->Mon[UType]->newMons;
	word TotBLD[512];
	if(NM->Building&&NI!=0xFF&&!BLD){
		NBLD=CreateBLDList(NI,TotBLD,512,*x2i,*y2i);
		BLD=TotBLD;
	};	
	int NBLD1=NBLD;
	byte Use=NM->Usage;
	if(NI!=0xFF&&NATIONS[NI].AI_Enabled&&Use!=SkladID)NBLD1=0;
	int x2=*x2i;
	int y2=*y2i;
	int xs=x2>>9;
	int ys=y2>>9;
	if(xs<=0||ys<=0||xs>=msx||ys>=msy)return -1;
	if(NI!=0xFF&&!CheckBuildPossibility(NI,x2>>4,y2>>4))return -1;
    if(NM->Building){
        int x,y,xm,ym;
        int ID=0;
	    int PicSX=NM->PicDx<<4;
	    int PicSY=(NM->PicDy)<<5;
	    xm=(x2+PicSX)>>8;
	    ym=(y2+PicSY)>>8;
        x=((xm+NM->CenterMX)<<8)+128;
        y=((ym+NM->CenterMY)<<8)+128;
        int r=(NM->BRadius+15)<<8;
		if(NM->BornOnSprite>=0){
            itr_GetSpritesInRadius.Create(x>>4,y>>4,512);
			int MinR=10000;
			while(OneSprite* OS=itr_GetSpritesInRadius.Next()){
				if((OS->SG==&TREES && OS->SGIndex==NM->BornOnSprite) 
				   ||(OS->SG==&STONES && OS->SGIndex==NM->BornOnSprite-4096)){
					int R=Norma(OS->x-(x>>4),OS->y-(y>>4));
					if(R<MinR){
						itr_GetBuildingsInRadius.Create(OS->x,OS->y,64);
						if(!itr_GetBuildingsInRadius.Next()){
							MinR=R;
							*x2i=OS->x<<4;
							*y2i=OS->y<<4;
						}
					}
				}
			}
			return MinR<512?ID:-1;
		}else{
			if(NM->ProdType){
				int xx=x2;
				int yy=y2;
				ID=CheckMinePosition(UType,&xx,&yy,r,Manual);
				*x2i=xx;
				*y2i=yy;
				x2=xx;
				y2=yy;
				xm=(x2+PicSX)>>8;
				ym=(y2+PicSY)>>8;
				x=((xm+NM->CenterMX)<<8)+128;
				y=((ym+NM->CenterMY)<<8)+128;
				if(ID==-1)return -1;
			}else{
				bool ETree=1;//(!NBLD1)||NM->StandAnywhere;
				for(int p=0;p<NBLD1;p++){
					OneObject* OB=Group[BLD[p]];
					if(OB&&OB->Ready&&Norma(OB->RealX-x,OB->RealY-y)<3000*16)ETree=true;
				};
				if(!CheckSpritesInAreaNew(x,y,r,ETree))return -1;
			};
		}
        if(!CheckMonstersInArea(NATIONS[NI].NMask,x,y,r))return -1;
		if(NM->BuildNearBuildingRadius){
			int GetDistanceToBuilding(OneObject* BLD,int x,int y);
			bool Well=0;
			for(int i=0;i<NBLD;i++){
				OneObject* OB=Group[BLD[i]];
				if(OB&&OB->NewBuilding){
					int r=Norma(x-OB->RealX,y-OB->RealY)>>4;
					if(r<1000){
						r=GetDistanceToBuilding(OB,x>>4,y>>4);
						if(r<NM->BuildNearBuildingRadius){
							Well=1;
							break;
						}
					}
				}
			}
			if(!Well)return  -1;
		}else{
			if(!CheckBuildingsInArea(x+(NM->BuildX0<<4),y+(NM->BuildY0<<4),x+(NM->BuildX1<<4),y+(NM->BuildY1<<4),BLD,NBLD,NM->ProdType!=0))return -1;
		}
#ifdef _USE3D
		if(!CheckRoadsInArea(x2+(NM->BuildX0<<4),y2+(NM->BuildY0<<4),x2+(NM->BuildX1<<4),y2+(NM->BuildY1<<4)))return -1;
#endif
        
        //checking locking information
        int np=NM->NCheckPt;
        char* Cpx=NM->CheckX;
        char* Cpy=NM->CheckY;
		int maxZ=-100000;
		int minZ=100000;
		if(NM->ProdType){
			int nn=0;
			//for(int i=0;i<np;i++){
			//	int xx=xm+Cpx[i];
			//	int yy=ym+Cpy[i];
			//	int z=GetHeight(xx<<4,yy<<4);
			//	if(z>maxZ)maxZ=z;
			//	if(z<minZ)minZ=z;
			//	if(CheckPt(xx,yy))nn++;
			//	if(nn>2)return -1;
			//};
		}else{
			int st=(np/90)+1;
	        for(int i=0;i<np;i+=st){
				int xx=xm+Cpx[i];
				int yy=ym+Cpy[i];
				int z=GetHeight(xx<<4,yy<<4);
				if(z>maxZ)maxZ=z;
				if(z<minZ)minZ=z;
				if(CheckVLine(xx,yy-4,8))return -1;
				if(CheckHLine(xx-3,yy,6))return -1;
			};
        };
		if(NM->ProdType)maxZ=minZ;
		if(abs(maxZ-minZ)>50)return -1;
		if(NM->Port){
			if(!FindPortPlace(NM,x,y,&PortBuiX,&PortBuiY))return -1;
		};
        return ID;
    }else{
        int mLx=(NM->Radius2+3)>>7;
	    if(!mLx)mLx=1;
	    if(!CheckTerra(x2,y2,mLx,NM->LockType))return -1;
        if(CheckPosition(x2,y2,NM->Radius2+8,0xFFFF))return 0;
        else return -1;
    };
};
int CheckSmartCreationAbility(byte NI,int Type,int* x2i,int* y2i){
	NewMonster* NM=NATIONS->Mon[Type]->newMons;
	word TotBLD[512];
	int Nb=0;
	if(NM->Building)Nb=CreateBLDList(NI,TotBLD,512,*x2i,*y2i);
	int v=CheckCreationAbility(NI,Type,x2i,y2i,TotBLD,Nb,1);
	if(NM->Port)return v;
	if(v==-1){
		int x0=((*x2i)>>8)<<8;
		int y0=((*y2i)>>8)<<8;
		for(int r=1;r<10;r++){//10
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int N=Rarr[r].N;
			for(int j=0;j<N;j++){
				int xc=x0+(int(xi[j])<<8);
				int yc=y0+(int(yi[j])<<8);
				int xc0=xc;
				int yc0=yc;
				v=CheckCreationAbility(NI,Type,&xc,&yc,TotBLD,Nb,0);
				if(v!=-1){
					*x2i=xc;
					*y2i=yc;
					return v;
				};
			};
		};
		return -1;
	}else return v;
};
int CheckAISmartCreationAbility(byte NI,int Type,int* x2i,int* y2i){
	NewMonster* NM=NATIONS->Mon[Type]->newMons;
	word TotBLD[512];
	int Nb=0;
	if(NM->Building)Nb=CreateBLDList(NI,TotBLD,512,*x2i,*y2i);
	int v=CheckCreationAbility(NI,Type,x2i,y2i,TotBLD,Nb,0);
	if(NM->Port)return v;
	if(v==-1){
		int x0=((*x2i)>>8)<<8;
		int y0=((*y2i)>>8)<<8;
		for(int r=1;r<10;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int N=Rarr[r].N;
			for(int j=0;j<N;j++){
				int xc=x0+(int(xi[j])<<9);
				int yc=y0+(int(yi[j])<<9);
				v=CheckCreationAbility(NI,Type,&xc,&yc,TotBLD,Nb,0);
				if(v!=-1){
					*x2i=xc;
					*y2i=yc;
					return v;
				};
			};
		};
		return -1;
	}else return v;
};
int SmartCreationUnit(byte NI,int NIndex,int x,int y){
	int v=CheckAISmartCreationAbility(NI,NIndex,&x,&y);
	if(v!=-1){
		return NATIONS[NI].CreateNewMonsterAt(x,y,NIndex,false);
	}else return -1;
};

extern City CITY[8];
extern byte BaloonState;
extern byte CannonState;
extern byte XVIIIState;
int CV_time=-1;
int CV_Cost[8];
int CV_x=-1;
int CV_y=-1;
int CV_NIndex=-1;
int CV_NI=-1;
void GetUnitCost(byte NI,word NIndex,int* Cost,int x,int y){
	if(NI==CV_NI&&NIndex==CV_NIndex&&tmtmt==CV_time&&x==CV_x&&y==CV_y){
		memcpy(Cost,CV_Cost,8*4);
		return;
	};
	CV_NI=NI;
	CV_NIndex=NIndex;
	CV_x=x;
	CV_y=y;
	CV_time=tmtmt;

	Nation* NT=&NATIONS[NI];
	GeneralObject* GO=NT->Mon[NIndex];
	AdvCharacter* ADC=GO->MoreCharacter;
	NewMonster* NM=GO->newMons;
	for(int i=0;i<8;i++)Cost[i]=ADC->NeedRes[i];
	if(GO->CostPercent==100){
		memcpy(CV_Cost,Cost,8*4);
		return;
	};
	int NUNITS=GO->CostGrowsOnlyForReadyBuildings?CITY[NI].ReadyAmount[NIndex]:CITY[NI].UnitAmount[NIndex];
	int Percent=GO->CostPercent;
	bool EXPENS=0;
	if(NM->Usage==PushkaID&&CannonState==2)EXPENS=1;
	int TIMES=1000;
	int LR=NM->LowCostRadius;
	if(Percent!=100){
		if(LR&&x>0&&y>0){
			//checking distance
			bool USUALCOST=0;
			int N=NtNUnits[NI];
			word* IDS=NatList[NI];
			for(int i=0;i<N;i++){
				word ID=IDS[i];
				OneObject* OB=Group[ID];
				if(OB&&OB->NIndex==NIndex&&!OB->Sdoxlo){					
					int R=Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
					if(R<LR){
						USUALCOST=1;
						break;
					};
				}
			}
			if(!USUALCOST)NUNITS=0;
		};
		for(int j=0;j<NUNITS;j++){
			if(TIMES<1000000){
				TIMES=(TIMES*Percent)/100;
				for(int i=0;i<8;i++){
					Cost[i]=div(Cost[i]*Percent,100).quot;
					if(Cost[i]<0)Cost[i]=10000000;
				};
			};
		};
	};
	if(EXPENS){
		for(int i=0;i<8;i++){
			Cost[i]*=10;
			if(Cost[i]<0)Cost[i]=10000000;
		};
	};
	memcpy(CV_Cost,Cost,8*4);
};
void GetUnitCost(byte NI,word NIndex,int* Cost,word Power){
	Nation* NT=&NATIONS[NI];
	GeneralObject* GO=NT->Mon[NIndex];
	AdvCharacter* ADC=GO->MoreCharacter;
	NewMonster* NM=GO->newMons;
	for(int i=0;i<8;i++)Cost[i]=ADC->NeedRes[i];
	if(GO->CostPercent==100)return;
	int NUNITS=GO->CostGrowsOnlyForReadyBuildings?CITY[NI].ReadyAmount[NIndex]:CITY[NI].UnitAmount[NIndex];
	int Percent=GO->CostPercent;
	bool EXPENS=0;
	if(NM->Usage==PushkaID&&CannonState==2)EXPENS=1;
	int TIMES=1000;
	for(int j=0;j<Power;j++){
		if(TIMES<1000000){
			TIMES=(TIMES*Percent)/100;
			for(int i=0;i<8;i++){
				Cost[i]=(Cost[i]*Percent)/100;
				///if(EXPENS)Cost[i]*=10;
				if(Cost[i]<0)Cost[i]=10000000;
			};
		};
	};
	if(EXPENS){
		for(int i=0;i<8;i++){
			Cost[i]*=10;
			if(Cost[i]<0)Cost[i]=10000000;
		};
	};
};
extern int LastMx;
extern int LastMy;
bool CheckCostHint(byte NI,word NIndex,bool Message){
	Nation* NT=&NATIONS[NI];
	GeneralObject* GO=NT->Mon[NIndex];
    AdvCharacter* ADC=GO->MoreCharacter;
	int Cost[8];
	GetUnitCost(NI,NIndex,Cost,LastMx,LastMy);
	char NRES=0;
	char CCC[128];
	CCC[0]=0;
    for(int i=0;i<8;i++){
        if(XRESRC(NI,i)<Cost[i]){
			if(CCC[0])strcat(CCC,", ");
			strcat(CCC,RDS[i].Name);
			NRES++;
		};
	};
	if(CCC[0]){ //&&Message
		/*
		char CC1[200];
		if(NRES>1)sprintf(CC1,NETRESS,CCC);//"Not enough resources: %s"
			else sprintf(CC1,NETRES1,CCC);//"Not enough resource: %s"
		AssignHint1(CC1,70);
		*/
		return false;
	};
    return true;
};
bool CheckCostHint(byte NI,word NIndex){
	return CheckCostHint(NI,NIndex,false);
}
bool CheckCost(byte NI,word NIndex,int x,int y){
	Nation* NT=&NATIONS[NI];
	GeneralObject* GO=NT->Mon[NIndex];
    AdvCharacter* ADC=GO->MoreCharacter;
	int Cost[8];
	GetUnitCost(NI,NIndex,Cost,x,y);
    for(int i=0;i<8;i++)
        if(XRESRC(NI,i)<Cost[i])return false;
    return true;
};
bool ApplyCost(byte NI,word NIndex,int x,int y){
	Nation* NT=&NATIONS[NI];
	GeneralObject* GO=NT->Mon[NIndex];
	AdvCharacter* ADC=GO->MoreCharacter;
	int Cost[8];
	GetUnitCost(NI,NIndex,Cost,x,y);
    if(!CheckCost(NI,NIndex,x,y))return false;
    for(int i=0;i<8;i++){
		AddXRESRC(NI,i,-Cost[i]);
		NT->ControlProduce(GO->Branch,i,-Cost[i]);
		if(GO->newMons->Building)NT->ResOnBuildings[i]+=Cost[i];
		else NT->ResOnUnits[i]+=Cost[i];
	};
    return true;
};

void GetDiscreta(int* x,int* y,int val){
	if(!val)return;
	int xx1=(*x)-val;
	int yy1=(*y);
	int p=(xx1+yy1)>>1;
	int q=(xx1-yy1)>>1;
	p=div(p+(val>>1),val).quot*val;
	q=div(q+(val>>1),val).quot*val;
	*x=p+q;
	*y=p-q;
};
extern MotionField UnitsField;
extern bool SubCost;
extern bool SpriteSuccess;
void FindUnitPosition(int* x,int *y,int Type){
	NewMonster* NM=NATIONS->Mon[Type]->newMons;
	int x0=*x;
	int y0=*y;
	int SH=9;
	if(NM->Usage==FastHorseID)SH++;
	for(int r=0;r<50;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int N=Rarr[r].N;
		if(rando()<16384){
			for(int j=0;j<N;j++){
				int x1=x0+(int(xi[j])<<SH);
				int y1=y0+(int(yi[j])<<SH);
				if(!CheckCreationAbility(0xFF,Type,&x1,&y1,NULL,0,1)){
					*x=x1;
					*y=y1;
					return;
				};
			};
		}else{
			for(int j=N-1;j>=0;j--){
				int x1=x0+(int(xi[j])<<SH);
				int y1=y0+(int(yi[j])<<SH);
				if(!CheckCreationAbility(0xFF,Type,&x1,&y1,NULL,0,1)){
					*x=x1;
					*y=y1;
					return;
				};
			};
		};
	};
};
void CHKS();
bool CheckSpritesInAreaFast(int x,int y,int r);
extern bool SpriteSuccess;
extern int LastSpriteIndex;
void CreateFields(byte NI,int x,int y,int n){
	extern bool TexPieceMode;
	if(TexPieceMode)return;
	Nation* NT=NATIONS+NI;
	City* CT=NT->AI_Enabled?CITY+NI:NULL;
	NewMonster* NM=NT->Mon[n]->newMons;
	if(NM->ExField){
		int r0=NM->Radius1;
		int L=NM->MotionDist;
		int dr=NM->Radius2;
		for(int ix=-r0;ix<=r0;ix++)
			for(int iy=-r0;iy<=r0;iy++){
				int xx=x+((ix+iy)*L*16)+((int(rando())*dr)>>11);
				int yy=y+((ix-iy)*L*16)+((int(rando())*dr)>>11);
				//if(CheckSpritesInAreaFast(xx,yy,35*16)){
				SpriteSuccess=false;
				NT->CreateNewMonsterAt(xx,yy,n,false);
				if(CT&&CT->NFields<512){
					CT->FieldsID[CT->NFields]=LastSpriteIndex;
					OneSprite* OS=&Sprites[LastSpriteIndex];
					if(OS->Enabled){
						CT->FieldsSN[CT->NFields]=(OS->x>>5)+((OS->y>>5)<<8);
						CT->NFields++;
					};
				};
				//};
			};
	}else{
		
		for(int ix=-3;ix<=3;ix++)
			for(int iy=-3;iy<=3;iy++){
				int xx=x+((ix+iy)*64*16);
				int yy=y+((ix-iy)*64*16);
				NT->CreateNewMonsterAt(xx,yy,n,false);
			};
	};
};
void SlowCreateFields(byte NI,int x,int y,int n){
	Nation* NT=NATIONS+NI;
	NewMonster* NM=NT->Mon[n]->newMons;
	if(NM->ExField){
		int r0=NM->Radius1;
		int L=NM->MotionDist;
		int dr=NM->Radius2;
		int RR=r0*L*16;
		for(int i=0;i<20;i++){
			int ix=((int(rando())*(RR+RR+1))>>15)-RR;
			int iy=((int(rando())*(RR+RR+1))>>15)-RR;
			int xx=x+(ix+iy);
			int yy=y+(ix-iy);
			NT->CreateNewMonsterAt(xx,yy,n,false);
		};

		/*
		for(int ix=-r0;ix<=r0;ix++)
			for(int iy=-r0;iy<=r0;iy++){
				int xx=x+((ix+iy)*L*16)+((int(rando())*dr)>>11);
				int yy=y+((ix-iy)*L*16)+((int(rando())*dr)>>11);
				if(rando()<80){
					NT->CreateNewMonsterAt(xx,yy,n,false);
				};
			};
		*/
	}else{
		for(int ix=-3;ix<=3;ix++)
			for(int iy=-3;iy<=3;iy++){
				int xx=x+((ix+iy)*64*16);
				int yy=y+((ix-iy)*64*16);
				if(rando()<80){
					NT->CreateNewMonsterAt(xx,yy,n,false);
				};
			};
	};
};
bool GroundBox=1;
extern byte NTex1[32];
extern byte NTex2[32];
void CreateGround(OneObject* G);
int GetTotalUnits(){
	int N=0;
	for(int i=0;i<MAXOBJECT;i++){
		if(Group[i])N++;
	};
	return N;
};
extern bool Loadingmap;
void CreateGrassGround(int x,int y);
void GenerateMiniMapSquare(int x0,int y0,int nx,int ny);
void GetAwayMonstersInArea(int x,int y,int r);
void RM_LoadNotObj(char* Name,int x,int y);
void RM_GetObjVector(char* Name,int* x,int* y);
extern bool ImmVis;
bool NoGround=0;
void WriteUnitAction(byte NI,word NIndex,int* R0,int* R1);
int LastAddUnit=0;
void CreatePlaneUnderBuilding(int xc,int yc,NewMonster* NM){
	int X0=xc+NM->BuildX0;
	int Y0=yc+NM->BuildY0;
	int X1=xc+NM->BuildX1;
	int Y1=yc+NM->BuildY1;
	int D=(Y1-Y0+X1-X0)>>1;
	int D2=D;
	int DD1=40;
	int DD2=0;
	if(!D){
		DD1=80;
		DD2=40;
	};
	NCurves=0;
	AddPointToCurve(X0-40,Y0,0,255);
	AddPointToCurve(X0+D,Y0+D2+40,0,255);
	AddPointToCurve(X1+40,Y1,0,255);
	AddPointToCurve(X1-D,Y1-D2-40,1,255);
}
void InitFromNM(OneObject* G,NewMonster* NM,GeneralObject* GO,AdvCharacter* ADC){
	G->CleanDamage=0;
	G->CollisionRadius=NM->CollisionRadius;
	G->NoCollision=NM->NoCollision;	
	G->SdoxloLow=0;
	G->MathMask=NM->MathMask;
	G->Usage=NM->Usage;
	G->AttachedEffectMask=1;	
	G->NotSelectable=NM->NotSelectable;
	G->Phase=0;
	G->NMisses=0;
	G->Guard=0xFFFF;
	G->GroundState=0;
	G->InPatrol=false;
	G->AddDamage=0;
	G->AddShield=0;
	G->StandGround=false;
	G->InArmy=false;
	G->Zombi=0;
	G->Ustage=0;
	G->NUstages=0;
	G->SingleUpgLevel=2;
	G->InternalLock=true;
	G->IFire=NULL;
	G->DefaultSettings(GO);	
	G->LockType=NM->LockType;
	G->OverEarth=0;
	G->NewMonst=false;
	G->NewBuilding=false;
	G->Wall=false;
	G->NothingTime=0;
	G->Hidden=false;
	G->UnlimitedMotion=false;
	G->NoBuilder=false;
	G->DoNotCall=false;
	G->AddInside=0;
	G->PersonalDelay=100;
	G->BrigadeID=0xFFFF;
	G->BrIndex=0;
	G->AutoKill=false;
	G->StandGround=NM->AutoStandGround;
	G->NoSearchVictim=NM->AutoNoAttack;
	G->ActivityState=0;
	G->ObjectScale=NM->StartScale;


	if(NM->CompxCraft){
		G->StageState=32768;
	};
	if(GO->newMons->NShotPt&&GO->newMons->ShotPtZ){
		G->TempDelay=znew(int,GO->newMons->NShotPt);
		memset(G->TempDelay,0,GO->newMons->NShotPt<<2);
	};
	if(GO->newMons->Building&&!NM->Wall)G->NewBuilding=true;
	else{
		if(NM->Wall)G->Wall=true;
		else G->NewMonst=true;
	};
	int mLx=(NM->Radius2+3)>>7;
	if(!(NM->LockType||NM->ComplexObjIndex!=0xFFFF))mLx=1;
	G->ImSelected=0;
	G->NewState=0;
	G->LocalNewState=0;
	G->newMons=GO->newMons;	
	G->Ref.General=GO;
	G->DestX=-1;
	G->DestY=-1;
	G->RealVx=0;
	G->RealVy=0;	
	G->OctantInfo=0xFF;		
	//G->Media=0;	
	G->Ready=true;
	G->delay=0;
	G->MaxDelay=0;
	G->NearBase=0xFFFF;
	G->RStage=0;
	G->RType=0xFF;
	G->RAmount=0;			
	G->BackMotion=0;
	G->Selected=0;
	G->Borg=0;
	G->Life=ADC->BirthLife;//ADC->Life;
	G->MaxLife=ADC->Life;
	if(NM->Nedorazvitoe)G->Life=NM->Life;	
	G->MoreCharacter=G->Ref.General->MoreCharacter;	
	G->LocalOrder=NULL;
	G->PrioryLevel=0;
	G->Sdoxlo=false;		
	G->Attack=false;
	G->EnemyID=0xFFFF;
	G->SafeWall=false;
	G->InMotion=false;
	G->MotionDir=1;
	G->Radius1=GO->newMons->Radius1;
	G->Radius2=GO->newMons->Radius2;
	G->Lx=mLx;
	G->x=(G->RealX-(G->Lx<<7))>>8;
	G->y=(G->RealY-(G->Lx<<7))>>8;
	G->FireOwner=0xFF;
	G->BlockInfo=0;
	G->AbRes=NM->ResConcentrator;
	G->Kills=0;
	G->NZalp=NM->MaxZalp;
	G->LockedInBuilding=0xFFFF;
	G->TotalPath=0;
	G->UnitSpeed=64;
	G->CurUnitSpeed=64;
	G->GroupSpeed=G->newMons->MotionDist;
	G->SearchOnlyThisBrigadeToKill=0xFFFF;
	G->MotionStyle=NM->MotionStyle;	
	G->Disabled=false;
}
int Nation::CreateNewMonsterAt(int rx,int ry,int n,bool Anyway,short Dir){
	addrand(rx);
	addrand(ry);
	addrand(NMon);
	addrand(NNUM);
	addrand(n);
	addrand(Anyway);	
	int maxx=(msx+2)<<(5+4);
	if((!Anyway)&&(rx<1024||ry<1024||rx>maxx||ry>maxx))return -1;
	rando();
	int x=rx;
	int y=ry;
	int R0[6];
	int R1[6];
	for(int j=0;j<6;j++)R0[j]=XRESRC(NNUM,j);
	GeneralObject* GO=Mon[n];
	NewMonster* NM=GO->newMons;
	//if(!FindPortPlace(NM,x,y,&PortBuiX,&PortBuiY))return -1;
	AdvCharacter* ADC=GO->MoreCharacter;
	int ID=-1;
	if((!Anyway)){
		if((!CheckBuildPossibility(NNUM,rx>>4,ry>>4))||
        (!CheckCost(NNUM,n,rx>>4,ry>>4)))return -1;
		rando();
    };
	rando();
    if(NM->SpriteObject&&!NM->Wall){
		rando();
		ObjCharacter* OC=COMPLEX.Objects[NM->Sprite];
		int sx=rx>>4;
		int sy=ry>>4;
		GetDiscreta(&sx,&sy,OC->Discret);
		//accurate check
		int cxx=sx>>4;
		int cyy=sy>>4;
		if(!CheckBar(cxx-4,cyy-3,8,15)){
			int H1=GetHeight(sx-64,sy);
			int H2=GetHeight(sx+64,sy);
			int H3=GetHeight(sx,sy-64);
			int H4=GetHeight(sx,sy+64);
			int Ha=(H1+H2+H3+H4)>>2;
			if(abs(H1-Ha)<40&&abs(H2-Ha)<32&&abs(H3-Ha)<32&&abs(H4-Ha)<32){
				int sid=NM->Sprite;
				if(NM->BRadius)sid+=rando()%NM->BRadius;
				addSprite(sx,sy,&COMPLEX,sid);
				if(SpriteSuccess){
					//RenewInfoMap(sx,sy);
					if(!Anyway)ApplyCost(NNUM,n,rx>>4,ry>>4);
					//CreateGrassGround(sx,sy);
				};
			};
		};
        return -1;
    };
	if(!(Anyway||NM->Wall)){
		addrand(345);
		if(!ext_OnCheckingBuildPossibility(NNUM,n,rx,ry))return -1;
		ID=CheckCreationAbility(NNUM,n,&rx,&ry,NULL,0,1);
		addrand(ID);
		x=rx;
		y=ry;
        if(ID==-1)return -1;
		rando();
    };
	if(NM->Building&&!Anyway){
		ApplyCost(NNUM,n,rx>>4,ry>>4);
	};
	for(int j=0;j<6;j++)R1[j]=XRESRC(NNUM,j);
	if(NM->PieceName&&!NoGround){
		int xx,yy;
		RM_GetObjVector(NM->PieceName,&xx,&yy);
		ImmVis=1;
		RM_LoadNotObj(NM->PieceName,(rx>>4),(ry>>4));
		x=((x>>10)<<10)+(xx<<4);
		y=((y>>10)<<10)+(yy<<4);
		rx=x;
		ry=y;
		ImmVis=0;
	};
	if(NM->ResSubst){
		int xxx=x>>4;
		int yyy=y>>4;
		for(int i=0;i<MaxSprt;i++)if(Sprites[i].Enabled){
			OneSprite* OS=Sprites+i;
			if(OS->OC->IntResType<8&&Norma(OS->x-xxx,OS->y-yyy)<64){
				int nn=GetUnitModifiedBySprite(n,OS);
				if(nn!=-1){
					i=MaxSprt;
                    n=nn;
					GO=Mon[n];
					NM=GO->newMons;
					ADC=GO->MoreCharacter;
				}				
			}
		}
	}
	int PicSX=NM->PicDx<<4;
	int PicSY=(NM->PicDy)<<5;
	if(NM->Building&&!NM->Wall){
		x=RoundX(rx+PicSX)-PicSX;
		y=RoundX(ry+PicSY)-PicSY;
	};	
	extern bool EditMapMode;
	int GetFreeIndexForUnit();
	int i=GetFreeIndexForUnit();
	if(i==-1)return -1;
	if(i>=MAXOBJECT)MAXOBJECT=i+1;
	extern bool EditMapMode;
	/*	
	if(!EditMapMode)LastAddUnit=0;
	else if(rando()<128)LastAddUnit=0;
	for(int i=LastAddUnit;i<MaxObj&&int(Group[i]);i++);
	if(i>=MaxObj)return -1;
	rando();
	if(n>=NMon) return -1;
	rando();
	LastAddUnit=i;
	if(i>=MAXOBJECT)MAXOBJECT=i+1;
	*/
	Group[i]=new OneObject;//OBJECTS+i;
	memset(Group[i],0,sizeof OneObject);
	LastObject=i;
	//Cell8x8* CELL=&TCInf[NNUM][y>>11][x>>11];
	OneObject* G=Group[i];
	//AddOrderEffect(x>>9,y>>9,GO->BornSound);
	//CELL->UnitsAmount[GO->Kind]++;	
	if(NM->Port){
		G->WallX=PortBuiX;
		G->WallY=PortBuiY;
		int LL=NM->BuiDist;
//		MFIELDS[1].BSetBar(PortBuiX-LL-1,PortBuiY-1,3);
//		MFIELDS[1].BSetBar(PortBuiX+LL-1,PortBuiY-1,3);
//		MFIELDS[1].BSetBar(PortBuiX-1,PortBuiY-LL-1,3);
//		MFIELDS[1].BSetBar(PortBuiX-1,PortBuiY+LL-1,3);
	};
	G->RealX=x;
	G->RealY=y;
	G->Nat=this;
	G->NMask=NMask;
	G->NNUM=NNUM;
	G->NIndex=n;
	G->Index=i;
	G->BirthNIndex=n;	
	InitFromNM(G,NM,GO,GO->MoreCharacter);
    G->UniqID=ActiveUnitAbility::CurSerial++;
	if(G->Usage==ArcherID&&rando()<2600)G->Usage=GrenaderID;	
	if(NM->CompxCraft){
		G->StageState=32768;
	};
	if(GO->newMons->NShotPt&&GO->newMons->ShotPtZ){
		G->TempDelay=znew(int,GO->newMons->NShotPt);
		memset(G->TempDelay,0,GO->newMons->NShotPt<<2);
	};
	if(GO->newMons->Building&&!NM->Wall)G->NewBuilding=true;
	else{
		if(NM->Wall)G->Wall=true;
		else G->NewMonst=true;
	};
	G->newMons=GO->newMons;
	G->RealDir=Dir==-1?(GO->newMons->Building?-64:rando()&255):Dir;
	G->RealDirPrecise=int(G->RealDir)<<8;
	G->GraphDir=G->RealDir;
	Visuals* m;
	m=(Visuals*)Mon[n];
	G->NewAnm=G->newMons->GetAnimation(anm_Stand);
	G->SetZeroFrame();
	G->x=(G->RealX-(G->Lx<<7))>>8;
	G->y=(G->RealY-(G->Lx<<7))>>8;
	AddObject(G);
	if(!G->NewBuilding)CITY->Account+=EngSettings.BornModulator*G->newMons->Expa;
	if(NM->ArtSet){
		NATIONS[G->NNUM].NArtUnits[NM->ArtSet-1]++;
	}
	if(G->LockType==1&&!G->NewBuilding){
		int NewX=G->RealX;
		int NewY=G->RealY;
		int LX=G->Lx;
		int ULX=(LX)<<7;
		int NewLockX=(NewX-ULX)>>8;
		int NewLockY=(NewY-ULX)>>8;
		UnitsField.SetRound(NewLockX,NewLockY,LX);
		G->LLock=true;
	}else{
		if(!(G->NewBuilding||G->Wall))UnitsField.BSetBar(G->x,G->y,G->Lx);
	};
	NProduced[G->NIndex]++;
	CITY->ReadyAmount[G->NIndex]++;

	G->RZ=GetTotalHeight(x>>4,y>>4);
	addrand(G->RZ);
	if(G->NewBuilding&&!NM->Wall){
		G->LoLayer=GO->newMons->GetAnimation(anm_BuildLo);
		//G->HiLayer=GO->newMons->GetAnimation(anm_BuildLo);
		extern bool AllowUndo;
		//if(!(AllowUndo||Anyway))CreatePlaneUnderBuilding(x>>4,y>>4,GO->newMons);
		G->RZ=GetHeight(x>>4,y>>4);
		addrand(G->RZ);
		G->Stage=0;
		if(NM->NBLockPt){
			int x0=(x+PicSX)>>8;
			int y0=(y+PicSY)>>8;
			for(i=0;i<NM->NBLockPt;i++){
				int xx=x0+NM->BLockX[i];
				int yy=y0+NM->BLockY[i];
				BSetPt(xx,yy);
				EraseTreesInPoint(xx,yy);
			};
			int xx1,yy1,xm,ym;
			int ID=0;
			int x2=rx;
			int y2=ry;
			xm=(x2+PicSX)>>8;
			ym=(y2+PicSY)>>8;
			xx1=((xm+NM->CenterMX)<<8)+128;
			yy1=((ym+NM->CenterMY)<<8)+128;
			int r=(NM->BRadius+4)<<8;
			GetAwayMonstersInArea(xx1,yy1,r);
		}else
		if(NM->NLockPt){
			int x0=(x+PicSX)>>8;
			int y0=(y+PicSY)>>8;
			for(i=0;i<NM->NLockPt;i++){
				int xx=x0+NM->LockX[i];
				int yy=y0+NM->LockY[i];
				BSetPt(xx,yy);
				EraseTreesInPoint(xx,yy);
			};
		};
		G->Life=2;
        G->Ready=false;
        if(NM->ProdType){
            G->TakeResourceFromSprite(ID);
        };
	};
	if(NM->NBars){
		int bx0=(G->RealX>>4)+NM->PicDx;
		int by0=(G->RealY>>4)+(NM->PicDy<<1);
		int N=NM->NBars;
		for(int i=0;i<N;i++){
			int pp=i*5;
			int XB0=NM->Bars3D[pp]+bx0;
			int YB0=(NM->Bars3D[pp+1]<<1)+by0;
			int L1=NM->Bars3D[pp+2];
			int L2=NM->Bars3D[pp+3];
			Add3DBar(XB0,YB0,XB0+L1+L2,YB0+L1-L2,NM->Bars3D[pp+4],G->Index,G->Index);
		};
	};
	if(!(NM->Building||NM->NoFarm))NGidot++;
	if(G->NewBuilding){
		
	}else{ 
		CITY->RegisterNewUnit(G);
		int ofst=(G->RealX>>11)+((G->RealY>>11)<<VAL_SHFCX)+VAL_MAXCX+1;
		int ofst1=ofst<<SHFCELL;
		if(ofst>=0&&ofst<VAL_MAXCX*VAL_MAXCX){
			int k=MCount[ofst];
			if(k<MAXINCELL-1){
				MCount[ofst]++;
				TmpMC[ofst]++;
				SetNMSL_FAST(ofst1+k,G->Index);
			}
		}
	}
	//if(GroundBox)CreateGround(G);
	addname(G->Index);
	addrand(G->NoSearchVictim);
	WriteUnitAction(G->NNUM,G->NIndex,R0,R1);

#ifdef NEWMORALE
	G->MaxMorale=G->MoreCharacter->StartMorale*10000;
	G->Morale=G->MaxMorale/2;
	if(G->Morale<33*10000)G->Morale=33*10000;
#endif //NEWMORALE
#ifdef GETTIRED
	G->GetTired=100*1000;
#endif //GETTIRED
#ifdef SIMPLEMANAGE
	if(NM->ArmAttack){
		G->ArmAttack=false;
		G->RifleAttack=true;
	}else{
		G->RifleAttack=false;
	};
#endif //SIMPLEMANAGE
#ifdef COSSACKS2
	G->CObjIndex=0xFFFF;
	if(NM->ComplexObjIndex!=0xFFFF){
		int CreateComplexObjectAt(int x,int y,word Type,byte NI,word Owner,byte StartDir);
		G->CObjIndex=CreateComplexObjectAt(x>>4,y>>4,NM->ComplexObjIndex,G->NNUM,G->Index,G->RealDir);
	};	
#endif //COSSACKS2
	addrand(G->RealDir);

	void ACTION_OnUnitBirth(OneObject* OB);
	ACTION_OnUnitBirth(G);
	ext_OnUnitBirth(G);
	if(NM->AutoBornSprite!=-1 && !G->NewBuilding){
		addSprite(x>>4,y>>4,&TREES,NM->AutoBornSprite);
	}

	G->MotionStyle=NM->MotionStyle;
	
	//--------- for be_CGROUP ------------//
	G->pGRP_IDS = NULL;
	G->GRP_N=0;
	//---------------end------------------//
	
	G->KillMask=NM->KillMask;	

	return G->Index;
};
void GenerateMiniMapSquare(int x0,int y0,int nx,int ny);
void CreateGround(OneObject* G){
	NewMonster* NM=G->newMons;
	if(NM->Building){
		byte Use=NM->Usage;
		if(Use==MelnicaID||Use==SkladID||Use==MineID)return;
		G->InFire=0;
		G->FireOwner=0xFF;
		GeneralObject* GO=G->Ref.General;
		byte tex1=NTex1[GO->NatID];
		byte tex2=NTex2[GO->NatID];
		int CX=G->RealX>>4;
		int CY=G->RealY>>4;
		int X0=CX+NM->BuildX0;
		int Y0=CY+NM->BuildY0;
		int X1=CX+NM->BuildX1;
		int Y1=CY+NM->BuildY1;
		int D=(Y1-Y0+X1-X0)>>1;
		int D2=D;
		int DD1=40;
		int DD2=0;
		if(!D){
			DD1=80;
			DD2=40;
		};
		if(tex2!=0xFF){
			NCurves=0;
			NTextures=1;
			TexList[0]=tex2;
			AddPointToCurve(X0-40,Y0,0,255);
			AddPointToCurve(X0+D,Y0+D2+40,0,255);
			AddPointToCurve(X1+40,Y1,0,255);
			AddPointToCurve(X1-D,Y1-D2-40,1,255);
		};
		if(tex1!=0xFF){
			NCurves=0;
			NTextures=1;
			TexList[0]=tex1;
			AddPointToCurve(X0,Y0,0,1);
			AddPointToCurve(X0+D,Y0+D2,0,1);
			AddPointToCurve(X1,Y1,0,1);
			AddPointToCurve(X1-D,Y1-D2,1,1);
		};
	};
};
void CreateGrassGround(int x,int y){
	y+=32;
	NCurves=0;
	NTextures=2;
	TexList[0]=7;
	TexList[1]=11;
	AddPointToCurve(x-100,y,0,1);
	AddPointToCurve(x,y-100,0,1);
	AddPointToCurve(x+100,y,0,1);
	AddPointToCurve(x,y+100,1,1);
};
void OneObject::GetCornerXY(int* x,int* y){
	NewMonster* NM=newMons;
	int PicSX=NM->PicDx<<4;
	int PicSY=(NM->PicDy)<<5;
	*x=(RealX+PicSX)>>8;
	*y=(RealY+PicSY)>>8;
};
void OneObject::GetPreciseCornerXY(int* x,int* y){
	NewMonster* NM=newMons;
	int PicSX=NM->PicDx;
	int PicSY=NM->PicDy<<1;
	*x=(RealX>>4)+PicSX;
	*y=(RealY>>4)+PicSY;
};
void NewMonsterSendToLink(OneObject* OB);
bool ParkWaterNewMonster(OneObject* OB,int x,int y,byte Prio,byte OrdType);
#undef NewMonsterSendTo
void OneObject::NewMonsterSendTo(int x,int y,byte Prio,byte OrdType){
	NewMonsterSendTo(x,y,Prio,OrdType,NULL,0);
}
bool CheckIfSomeoneMovesIntoThisUnit(OneObject* OB);
void OneObject::NewMonsterSendTo(int x,int y,byte Prio,byte OrdType,OneObject* Enemy,int NSteps){
	if(LockedInBuilding!=0xFFFF)return;

	if(!ext_OnAttemptToMove(this,x>>4,y>>4))return;

	if(ActivityState==2){//aggressive
		Prio=128+1;
	}
	if(CObjIndex!=0xFFFF){ 
		if(Norma(RealX-x,RealY-y)<16*80)return;
	}
	if(CheckIfSomeoneMovesIntoThisUnit(this))return;
	addrand(x);
	addrand(y);
	bool OPT=OrdType>=128;
	//OrdType&=127;
	NewMonster* NM=newMons;
	//if(Ref.General->OFCR&&InArmy){
	//	return;
	//};
	if(LockType==1&& ( NM->ShowInsideUnits || NM->MaxInside) ){
		bool CTR=CheckTransportOnParking(this,x>>8,y>>8);
		if(ParkWaterNewMonster(this,x,y,Prio,OrdType))return;
	};
	if(!(NM->Transport||NM->ShowInsideUnits||NM->MaxInside)){
		if(CheckOrderAbility())return;
	};
	if(NM->Building)return;
	if(PrioryLevel>Prio)return;
	addrand(1014);
	Order1* Or1=CreateOrder(OrdType==0?3:OrdType);
	if(!int(Or1))return;

	if(!(LockType==1||Sdoxlo||UnlimitedMotion||OPT||BlockInfo)){
#ifdef COSSACKS2
		if(CObjIndex==0xFFFF){
#endif
		MotionField* MF=MFIELDS+LockType;
		if(MF->CheckPt(this->x,this->y)){
			//FindPositionForUnit(this);			
		};
#ifdef COSSACKS2
		};
#endif
	};
	Or1->PrioryLevel=Prio&127;
	Or1->OrderType=12;
	Or1->OrderTime=0;
	Or1->DoLink=&NewMonsterSendToLink;
	Or1->info.MoveToXY.x=(x-((Lx)<<7))>>8;
	Or1->info.MoveToXY.y=(y-((Lx)<<7))>>8;
	Or1->info.MoveToXY.EnemyID=0xFFFF;
	Or1->info.MoveToXY.EnemySN=0xFFFF;
	Or1->info.MoveToXY.Steps=NSteps;
	addrand(Or1->info.MoveToXY.x);
	addrand(Or1->info.MoveToXY.y);
	if(Enemy){
		addrand(Enemy->Index);
		addrand(Enemy->Serial);
		Or1->info.MoveToXY.EnemyID=Enemy->Index;
		Or1->info.MoveToXY.EnemySN=Enemy->Serial;
	}

	Order1* LOR=LocalOrder;
	//LocalOrder=Or1;
	//DestX=x;
	//DestY=y;
	//PrioryLevel=Prio&127;
	UnBlockUnit();
	PrioryLevel=Or1->PrioryLevel;
	MakeOrderSound(this,2);
	StandTime=0;
	PathDelay=0;
	addrand(x);
	addrand(y);
	if(Or1->PrioryLevel==0)InPatrol=true;
};
//#define NewMonsterSendTo(x,y,Prio,OrdType) NewMonsterSendTo(x,y,Prio,OrdType);addrand(33)
bool FindBestPosition(OneObject* OB,int* xd,int* yd,int R0);
int CheckBreaksBetweenPoints(int xs,int ys,int zs,int xd,int yd,int zd,int Index){
	if(Index==0xFFFF)return-1;
	if(!Group[Index])return-1;
	int CheckWpLine(int xs,int ys,int zs,int xd,int yd,int zd,word Index);
	int CheckFriendlyUnitsInLine(int xs,int ys,int zs,int* xD,int* yD,int* zD,byte MASK,OneObject*);
	int r1=CheckWpLine(xs,ys,zs,xd,yd,zd,Index);
	if(r1==-1){
		OneObject* EN=Group[Index];
		if(EN){
			r1=CheckFriendlyUnitsInLine(xs,ys,zs,&xd,&yd,&zd,EN->NMask,Group[Index]);
			return r1;
		}else return -1;
	}else return r1;
}
bool CanIStandHere(OneObject* OB)
{
	bool rez=true;
	if(OB)
	{
		UnitsField.BClrBar(OB->x,OB->y,OB->Lx);
		if(UnitsField.CheckBar(OB->x,OB->y,OB->Lx,OB->Lx))
			rez=false;
		UnitsField.BSetBar(OB->x,OB->y,OB->Lx);
	}
	return rez;
}
void RestLink(OneObject* OB){
	Order1* OR=OB->LocalOrder;
	//OB->PrioryLevel=OR->PrioryLevel;
	NewAnimation* NA=OB->newMons->GetAnimation(anm_Rest);
	if( OB->NewAnm!=NA || OB->FrameFinished ){
		byte& init=OR->info.MoveToXY.Times;
		word& motion=OR->info.MoveToXY.PrevDist;
		int& destx=OR->info.MoveToXY.x;
		if( init==0 ){
			init=1;
			motion=OB->InMotion;
			OB->InMotion=false;
			destx=OB->DestX;
			OB->DestX=-1;
			OB->NewAnm=NA;
			OB->SetZeroFrame();
		}else{
			OB->InMotion=motion;
			OB->DestX=destx;
			OB->DeleteLastOrder();
		}
	}
}
bool Rest(OneObject* OB, byte OrdType){
	if( OB /*&& !OB->UnlimitedMotion*/ ){
		Order1* OR=OB->CreateOrder(OrdType);
		if(OR){
			OR->DoLink=&RestLink;
			OR->PrioryLevel=128+16;
			OR->OrderType=12;

			OR->info.MoveToXY.Times=0;

			OB->PrioryLevel=OR->PrioryLevel;

			//addrand(AblSerial);
			//addrand(Dist);
			return true;
		}
	}
	return false;
}
void NewMonsterSendToLink(OneObject* OB){
	if(OB->PathDelay&&OB->StandTime>64){
		if(int(OB->LocalOrder)){
			OB->DeleteLastOrder();
			return;
		};
	};
	OB->PrioryLevel=OB->LocalOrder->PrioryLevel;
	OB->NewState=OB->GroundState;
	addrand(OB->NewState+1);
	int xx1=OB->LocalOrder->info.MoveToXY.x;
	int yy1=OB->LocalOrder->info.MoveToXY.y;
	addrand(xx1);
	addrand(yy1);
	int Lx=OB->Lx;
	MotionField* MF=MFIELDS+OB->LockType;
	if(rando()<2048){
		if(OB->GLock)MF->ClearRound(OB->x,OB->y,OB->Lx);
		if((!OB->UnlimitedMotion)&&MF->CheckRound(xx1-1,yy1-1,Lx+2)){
			if(FindBestPositionOLD(OB,&xx1,&yy1,30)){
				OB->LocalOrder->info.MoveToXY.x=xx1;
				OB->LocalOrder->info.MoveToXY.y=yy1;
			};
		};
		if(OB->GLock)MF->SetRound(OB->x,OB->y,OB->Lx);
	}
	Order1* ORD=OB->LocalOrder;
	if(ORD->info.MoveToXY.Steps){
		if(!(ORD->info.MoveToXY.Steps--)){
			OB->DeleteLastOrder();
			return;
		}		
	}
	addrand(ORD->info.MoveToXY.EnemyID);
	addrand(ORD->info.MoveToXY.EnemySN);
	if(rando()<4096&&ORD->info.MoveToXY.EnemyID!=0xFFFF){
		OneObject* EOB=Group[ORD->info.MoveToXY.EnemyID];
		if(EOB&&(!EOB->Sdoxlo)&&EOB->Serial==ORD->info.MoveToXY.EnemySN){
			addrand(34);
			if(CheckBreaksBetweenPoints(OB->RealX>>4,OB->RealY>>4,OB->RZ+32,EOB->RealX>>4,EOB->RealY>>4,EOB->RZ+32,OB->Index)==-1){
				addrand(35);
                if(CanIStandHere(OB))
				{
					addrand(36);
					OB->DeleteLastOrder();
					addrand(76);
					return;
				}
			}
		}else{
			addrand(73);
			OB->DeleteLastOrder();
			return;
		}
	}
	Order1* OR1;
	OR1=OB->LocalOrder->NextOrder;
	while(OR1){
		void GoToMineLink(OneObject* OBJ);
		if(OR1->DoLink==&GoToMineLink){
			word OID=OR1->info.BuildObj.ObjIndex;
			CITY[OB->NNUM].TheyMovesInto.Add(OID);
			CITY[OB->NNUM].MoversIndices.Add(OB->Index);            
		}
		OR1=OR1->NextOrder;
	}
	addrand(79);
	addrand(xx1);
	addrand(yy1);
	int r=(OB->newMons->StopDistance>>8);
	int ms=OB->MotionStyle;
    if(ms==6)r+=2;
	if(OB->DistTo(xx1,yy1)<=r){
		if(int(OB->LocalOrder)){
			//OB->DeleteLastOrder();
			//if(OB->MotionStyle==3){
#ifdef COSSACKS2
				if(ms==8||ms==7||ms==2||OB->FrameFinished){
#else
				if(OB->FrameFinished){
#endif
					if(ms==3){
						OB->GroundState=0;
						addrand(OB->GroundState+1);
						OB->NewState=0;
						addrand(OB->NewState+1);
					};
#ifdef COSSACKS2
					if(ms!=7)TryToStand(OB,0);
#else
					TryToStand(OB,0);
#endif
					addrand(xx1);
					addrand(yy1);
					OB->DeleteLastOrder();
				};
			//}else{
			//	OB->DeleteLastOrder();
			//};
		};
		addrand(xx1);
		addrand(yy1);
	}else{
		addrand(xx1);
		addrand(yy1);
		OB->CreatePath(xx1,yy1);
	}
};
void CorrectUnitPositionBeforeSend(OneObject* OB,int& x,int& y){
	int Lx=OB->Lx;
	int xx1=(x-(Lx<<7))>>8;
	int yy1=(y-(Lx<<7))>>8;
	int xx2=xx1;
	int yy2=yy1;	
	UnLockComplexObject(OB);
	if(FindBestPositionOLD(OB,&xx1,&yy1,40)){
		if(xx1!=xx2||yy1!=yy2){			
			x=(xx1<<8)+(Lx<<7);
			y=(yy1<<8)+(Lx<<7);
		}
	}
	LockComplexObject(OB);
}
void NewMonsterPreciseSendToLink(OneObject* OB);
#undef NewMonsterPreciseSendTo
void OneObject::NewMonsterPreciseSendTo3D(int x,int y,int z0,int z1,byte Prio,byte OrdType){
	if(LockedInBuilding!=0xFFFF)return;

	if(!ext_OnAttemptToMove(this,x>>4,y>>4))return;

	if(ActivityState==2){//aggressive
		Prio=128+1;
	}
	if(CheckIfSomeoneMovesIntoThisUnit(this))return;
	byte OPT=OrdType&128;
	NewMonster* NM=newMons;
	if(LockType==1&& ( NM->ShowInsideUnits || NM->MaxInside) ){
		bool CTR=CheckTransportOnParking(this,x>>8,y>>8);
		if(ParkWaterNewMonster(this,x,y,Prio,OrdType))return;
	};
	if(!(NM->Transport||NM->ShowInsideUnits||NM->MaxInside)){
		if(CheckOrderAbility())return;
	};
	if(NM->Building)return;
	if(PrioryLevel>Prio)return;
#ifdef COSSACKS2
	void NewAttackPointLink(OneObject* OBJ);
	if(OrdType<2&& CObjIndex==0xFFFF &&(!CheckIfPossibleToBreakOrder(this))){
		OrdType=2;
	}
#endif
	addrand(1015);
	Order1* Or1=CreateOrder(OrdType==0?3:OrdType);
	if(!int(Or1))return;
	StandTime=0;
	Or1->PrioryLevel=Prio&127;
	Or1->OrderType=12;
	Or1->OrderTime=0;
	Or1->DoLink=&NewMonsterPreciseSendToLink;
	Or1->info.MoveToXY.x=x;
	Or1->info.MoveToXY.y=y;
	Or1->info.MoveToXY.z0=z0;
	Or1->info.MoveToXY.z1=z1;
	Or1->info.MoveToXY.PrevDist=0xFFFF;
	addrand(x);
	addrand(y);
	Order1* LOR=LocalOrder;
	UnBlockUnit();
	MakeOrderSound(this,2);
	StandTime=0;
	PathDelay=0;
	if(ActivityState==2&&!(Prio&127)){
		if(newMons->GetAnimation(anm_PMotionL)->Enabled)GroundState=1;
		addrand(332);
	}else{
		bool GS0=1;
		addrand(333);
		addrand(InArmy);
		addrand(BrigadeID);
		if(InArmy&&BrigadeID!=0xFFFF){
			addrand(334);
			Brigade* BR=Nat->CITY->Brigs+BrigadeID;
			if(BR->AttEnm)GS0=0;
			addrand(GS0+1);
			addrand(BR->AttEnm);
		};
		if(GS0){
			addrand(335);
			GroundState=ActivityState==2;
			addrand(GroundState+1);
			CheckState();
		};
	};
}
void OneObject::NewMonsterPreciseSendTo(int x,int y,byte Prio,byte OrdType){
	if(LockedInBuilding!=0xFFFF)return;

	if(!ext_OnAttemptToMove(this,x>>4,y>>4))return;

	if(ActivityState==2){//aggressive
		Prio=128+1;
	}
	if(CheckIfSomeoneMovesIntoThisUnit(this))return;
	byte OPT=OrdType&128;
	//OrdType&=127;

	/*
	int xx1=(x-(Lx<<7))>>8;
	int yy1=(y-(Lx<<7))>>8;
	int xx2=xx1;
	int yy2=yy1;
	
	bool FindBestPositionOLD(OneObject* OB,int* xd,int* yd,int R0);
	if(FindBestPositionOLD(this,&xx1,&yy1,40)){
		if(xx1!=xx2||yy1!=yy2){			
			x=(xx1<<8)+(Lx<<7);
			y=(yy1<<8)+(Lx<<7);
		}
	}
	*/

	NewMonster* NM=newMons;
	if(LockType==1&&NM->ShowInsideUnits||NM->MaxInside){
		bool CTR=CheckTransportOnParking(this,x>>8,y>>8);
		if(ParkWaterNewMonster(this,x,y,Prio,OrdType))return;
	};
	if(!(NM->Transport||NM->ShowInsideUnits||NM->MaxInside)){
		if(CheckOrderAbility())return;
	};
	if(NM->Building)return;
	if(PrioryLevel>Prio)return;
#ifdef COSSACKS2
	void NewAttackPointLink(OneObject* OBJ);
	if(OrdType<2&& CObjIndex==0xFFFF &&(!CheckIfPossibleToBreakOrder(this))){
		OrdType=2;
	}
#endif
	addrand(1015);
	int x1=DestX;
	int y1=DestY;
	Order1* Or1=CreateOrder(OrdType==0?3:OrdType);
	if(!int(Or1))return;
	//if(!(LockType==1||Sdoxlo||UnlimitedMotion||OPT)){
		//if(MFIELDS[LockType].CheckPt(this->x,this->y)){
			//FindPositionForUnit(this);			
		//};
	//};
	DestX=x1;
	DestY=y1;
	StandTime=0;
	Or1->PrioryLevel=Prio&127;
	Or1->OrderType=12;
	Or1->OrderTime=0;
	Or1->DoLink=&NewMonsterPreciseSendToLink;
	Or1->info.MoveToXY.x=x;
	Or1->info.MoveToXY.y=y;
	Or1->info.MoveToXY.z0=-1000;
	Or1->info.MoveToXY.z1=-1000;
	addrand(x);
	addrand(y);
	//assert(Or1->info.MoveToXY.x>1256);
	Order1* LOR=LocalOrder;
	//LocalOrder=Or1;
	//DestX=x;
	//DestY=y;
	//PrioryLevel=Prio&127;
	UnBlockUnit();
	//PrioryLevel=Or1->PrioryLevel;
	MakeOrderSound(this,2);
	StandTime=0;
	PathDelay=0;
	if(OrdType==0||OrdType==1)PrioryLevel=Prio&127;
	//assert(Or1->info.MoveToXY.x>1256);
	if(ActivityState==2&&!(Prio&127)){
		if(newMons->GetAnimation(anm_PMotionL)->Enabled)GroundState=1;
		addrand(332);
	}else{
		bool GS0=1;
		addrand(333);
		addrand(InArmy);
		addrand(BrigadeID);
		if(InArmy&&BrigadeID!=0xFFFF){
			addrand(334);
			Brigade* BR=Nat->CITY->Brigs+BrigadeID;
			if(BR->AttEnm)GS0=0;
			addrand(GS0+1);
			addrand(BR->AttEnm);
		};
		if(GS0){
			addrand(335);
			GroundState=ActivityState==2;
			addrand(GroundState+1);
			CheckState();
		};
	};
};
#define NewMonsterPreciseSendTo(x,y,Prio,OrdType) NewMonsterPreciseSendTo(x,y,Prio,OrdType);addrand(44)
void NewMonsterPreciseSendToLink(OneObject* OB){
	if(OB->PathDelay&&OB->StandTime>64){
		if(int(OB->LocalOrder)){
			OB->DeleteLastOrder();
			return;
		};
	};
	OB->PrioryLevel=OB->LocalOrder->PrioryLevel;
	OB->NewState=OB->GroundState;
	addrand(OB->NewState+1);
	int xx2=OB->LocalOrder->info.MoveToXY.x;
	int yy2=OB->LocalOrder->info.MoveToXY.y;
	if(xx2<-16*500||xx2>16*16000||yy2<-16*500||yy2>16*16000){
		OB->DeleteLastOrder();
		return;
	}
	//assert(xx2>1256);
	int Lx=OB->Lx;
	int xx1=(xx2-(Lx<<7))>>8;
	int yy1=(yy2-(Lx<<7))>>8;
	addrand(xx1);
	addrand(xx2);
	Order1* OR1;
	OR1=OB->LocalOrder->NextOrder;
	while(OR1){
		void GoToMineLink(OneObject* OBJ);
		if(OR1->DoLink==&GoToMineLink){
			word OID=OR1->info.BuildObj.ObjIndex;
			CITY[OB->NNUM].TheyMovesInto.Add(OID);
			CITY[OB->NNUM].MoversIndices.Add(OB->Index);            
		}
		OR1=OR1->NextOrder;
	}
	int dr=Norma(xx2-OB->RealX,yy2-OB->RealY);
	if(dr<1024){
		OB->DestX=xx2;
		OB->DestY=yy2;
		addrand(OB->DestX);
		addrand(OB->DestY);
	}else{
		bool pdz=OB->PathDelay==0;
		if(OB->PathDelay){
			UnLockComplexObject(OB);
			if(FindBestPositionOLD(OB,&xx1,&yy1,40)){
				OB->LocalOrder->info.MoveToXY.x=(xx1<<8)+(Lx<<7);
				OB->LocalOrder->info.MoveToXY.y=(yy1<<8)+(Lx<<7);
			}
			LockComplexObject(OB);
		}
		OB->CreatePath(xx1,yy1);	
		/*
		if(OB->PathX){
			if(OB->NIPoints==1){
				int cx=OB->PathX[OB->NIPoints-1];
				int cy=OB->PathY[OB->NIPoints-1];
				int dis=OB->DistTo(cx,cy);
				int D=1;
				if(OB->LockType==1)D=3;
				if(dis<=D){
					OB->DeleteLastOrder();
					OB->DestX=-1;
					OB->DestY=-1;
					OB->DeletePath();
					return;
				}
			}
		}
		*/
		if(pdz&&OB->PathDelay){
			xx1+=(rando()&7)-3;
			yy1+=(rando()&7)-3;
			UnLockComplexObject(OB);
			if(FindBestPositionOLD(OB,&xx1,&yy1,40)){
				OB->LocalOrder->info.MoveToXY.x=(xx1<<8)+(Lx<<7);
				OB->LocalOrder->info.MoveToXY.y=(yy1<<8)+(Lx<<7);
			}
			LockComplexObject(OB);
		}
#ifdef COSSACKS2
		int dr2=Norma(xx2-OB->DestX,yy2-OB->DestY);
		if(dr2<512){
			OB->DestX=xx2;
			OB->DestY=yy2;
			addrand(OB->DestX);
			addrand(OB->DestY);
		};
#endif
	};
    NewMonster* NM=OB->newMons;
	int drm=NM->MotionDist;
    if(OB->MotionStyle==5 || OB->MotionStyle==8)drm=512;
	if(OB->CObjIndex!=0xFFFF){
		char dr=OB->RealDir-GetDir(OB->DestX-OB->RealX,OB->DestY-OB->RealY);
		if(abs(dr)<32)drm=drm*3;
		else drm=drm<<3;
		if(drm<65)drm=65;
	}
	if(OB->LocalOrder->info.MoveToXY.z0>-1000){
		word& d=OB->LocalOrder->info.MoveToXY.PrevDist;
		if(d==0xFFFF){
			d=Norma(xx2-OB->RealX,yy2-OB->RealY)/16;
		}
		int pd=int(d);
		if(pd==0)pd=1;
		int v=dr*1024/16/pd;
		if(v>1024)v=1024;
		OB->OverEarth=OB->LocalOrder->info.MoveToXY.z1+(OB->LocalOrder->info.MoveToXY.z0-OB->LocalOrder->info.MoveToXY.z1)*v/1024;
	}
	if(dr<drm){
		if(OB->CObjIndex==0xFFFF && drm<256){
			OB->RealX=xx2;
			OB->RealY=yy2;
			addrand(OB->RealX);addrand(OB->RealY);
			if(xx1!=OB->x||yy1!=OB->y){
				UnitsField.BClrBar(OB->x,OB->y,Lx);
				//if(!UnitsField.CheckBar(xx1,yy1,Lx,Lx)){
				OB->x=xx1;
				OB->y=yy1;
				UnitsField.BSetBar(xx1,yy1,Lx);
			};
			TryToStand(OB,false);
		}
		OB->DeleteLastOrder();
		OB->DestX=-1;
		OB->DestY=-1;
		return;
	};
};
int SetInCellTime=0;
void ProcessNewMonsters(){
	void CheckMemoryIntegrity();
	//RunPF(5);
	SetupSpot();
//	StopPF(5);
	if(SetInCellTime<=0 || tmtmt<5){
		SetMonstersInCells();
		SetInCellTime=256*10;
	}
	SetInCellTime-=GameSpeed;
	//RunPF(14);
	CalculateMotion();
//	StopPF(14);
	void CheckMemoryIntegrity();
	return;
};
typedef  bool CHOBJ(OneObject* OB,int N);
void GetRect(OneObject* ZZ,int* x,int* y,int* Lx,int* Ly);
bool PInside(int x,int y,int x1,int y1,int xp,int yp);
word CheckCoorInGP(int x,int y);
Rct GetRect( OneObject* OB );
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);
bool GetObjectVisibilityInFogWithInvisibility(int x,int y,int z,OneObject* OB);
bool GetObjectVisibilityInFog(OneObject* OB);
bool GetObjectVisibilityInFog(OneObject* OB,int MinFogValue);
extern bool EditMapMode;
extern bool InfoMode;
//--------------------------------------------------------------------------
//	Func:	GoodSelectNewMonsters
//	Desc:	Processes all units inside selection rectangle
//	Parm:	NI - nation index of selection
//			xr, yr, xr1, yr1 - selection rectangle (world space)
//			Collect - here selected objects' indices are returned
//			Ser - here selected objects' serials are returned
//			WRITE - when false, arrays Collect and Ser are not filled
//			FN	- additional selection criteria predicate
//			NN - ?
//			MAX - maximal size of arrays Collect and Ser
//			InForm - select only units that are in formation
//			NotInForm - select only units that are not in formation
//	Ret:	Number of selected units
//--------------------------------------------------------------------------
WORD GoodSelectNewMonsters( byte	NI, 
							int xr, int yr, int xr1, int yr1, 
							WORD*	Collect, 
							WORD*	Ser,
							bool	WRITE,
							CHOBJ*	FN,
							int		NN,
							int		MAX,
							bool	InForm,
							bool	NotInForm )
{
	if(!(GSets.CGame.ViewMask&4))return 0;
	byte NIX=GSets.CGame.cgi_NatRefTBL[NI];
	int NSLC=0;
	
	int wr = xr1 - xr;
	int hr = yr1 - yr;	

	//  selection area is small, so do point selection
	if (abs( wr ) < 16 && abs( hr ) < 16)
	{
		WORD IDX = CheckCoorInGP( xr, yr );
		if (IDX == 0xFFFF) return 0;
		OneObject* OB=Group[IDX];
		if(OB&&OB->newMons&&(!((OB->ImSelected&GM(NI))||OB->Hidden||OB->Sdoxlo||OB->UnlimitedMotion))
								&&((!FN)||FN(OB,NN)))
		{
			if( GetObjectVisibilityInFog(OB,100) ){
				if( EditMapMode&&InfoMode || !(OB->notSelectable()||OB->newMons->NewParams.PortretDisable) ){
					if((OB->BrigadeID!=0xFFFF&&InForm)||(OB->BrigadeID==0xFFFF&&NotInForm)){
						if(WRITE){
							Collect[NSLC]=OB->Index;
							Ser[NSLC]=OB->Serial;
							OB->ImSelected|=GM(NI);
						};
						NSLC++;
					}
				}
			}
		}
		return NSLC;
	};
	bool UPresent=false;
	
	Rct rctSel( xr, yr, wr, hr );
	//  search monsters list for which of them are inside selection area
	bool EnemyPresent=false;
	bool MyPresent=false;
	for(int i=0;i<MAXOBJECT;i++)
	{
		OneObject* OB=Group[i];
		if (!OB || !OB->newMons) continue;
		if ((OB->ImSelected&GM( NI )) || OB->Hidden || OB->Sdoxlo || OB->UnlimitedMotion) continue;
		if (FN && !FN( OB, NN )) continue;

		if( !(EditMapMode&&InfoMode) ){
			if( OB->notSelectable() || OB->newMons->NewParams.PortretDisable ) 
				continue;
			if( OB->NNUM!=NIX ) 
				continue;
		}

		if((OB->BrigadeID!=0xFFFF&&InForm)||(OB->BrigadeID==0xFFFF&&NotInForm))
		{
			
			if (rctSel.Overlap( GetRect( OB ) ) && GetObjectVisibilityInFogWithInvisibility(OB->RealX/16,OB->RealY/16,OB->RZ,OB) )
			{
				if( OB->NNUM != NIX ){
				//	if( EnemyPresent )
				//		continue;
				//	EnemyPresent=true;
				}else 
					MyPresent=true;

				if(NSLC<MAX)
				{
					if(WRITE){
						Collect[NSLC]=i;
						Ser[NSLC]=OB->Serial;
						OB->ImSelected|=GM(NI);
					};
					NSLC++;
					if(!(OB->NewBuilding||OB->Wall))UPresent=true;
				}else assert(0);
			}
		}
	}

	if(UPresent&&WRITE){
		for(int i=0;i<NSLC;i++){
			OneObject* OB=Group[Collect[i]];
			if(OB&&(OB->NewBuilding||OB->Wall||(MyPresent&&OB->NNUM!=NIX))){
				Collect[i]=0xFFFF;
				OB->ImSelected&=~GM(NI);
			}
		}
	}else if(WRITE){
		for(int i=0;i<NSLC;i++){
			OneObject* OB=Group[Collect[i]];
			if(OB&&(MyPresent&&OB->NNUM!=NIX)){
				Collect[i]=0xFFFF;
				OB->ImSelected&=~GM(NI);
			}
		}
	}	
	return NSLC;
};
void DrawMarker(OneObject* OB);
void DrawMiniMarker(OneObject* OB);
void ShowNMHealth(OneObject* OB){
	if(MiniMode)DrawMiniMarker(OB);
	else DrawMarker(OB);
};
//-------------------------------------------------------//
//                                                       //
//                    ���������                 //
//                                                       //
//-------------------------------------------------------//
//byte MapH[MAPSY*BMSX];//(x,y)-bit matrix(1point=16x12 pix)
//byte MapV[MAPSY*BMSX];//(y,x)-T bit matrix
const int Power2[24]={
		0x00000001,0x00000003,0x00000007,0x0000000F,
		0x0000001F,0x0000003F,0x0000007F,0x000000FF,
		0x000001FF,0x000003FF,0x000007FF,0x00000FFF,
		0x00001FFF,0x00003FFF,0x00007FFF,0x0000FFFF,
		0x0001FFFF,0x0003FFFF,0x0007FFFF,0x000FFFFF,
		0x001FFFFF,0x003FFFFF,0x007FFFFF,0x00FFFFFF};
void ClearMaps(){
	MFIELDS->ClearMaps();
	UnitsField.ClearMaps();
};
void BSetPt(int x,int y){
	MFIELDS->BSetPt(x,y);
	MFIELDS[2].BSetPt(x,y);	
}
void BSetTotalPt(int x,int y){
	MFIELDS->BSetPt(x,y);
	MFIELDS[2].BSetPt(x,y);
	MFIELDS[3].BSetPt(x,y);
}
void BClrPt(int x,int y){
	MFIELDS->BClrPt(x,y);
	MFIELDS[2].BClrPt(x,y);	
}
void BClrTotalPt(int x,int y){
	MFIELDS->BClrPt(x,y);
	MFIELDS[2].BClrPt(x,y);	
	MFIELDS[3].BClrPt(x,y);	
}

void BSetBar(int x,int y,int Lx){
	MFIELDS[0].BSetBar(x,y,Lx);
	MFIELDS[2].BSetBar(x,y,Lx);
};
void BSetTotalBar(int x,int y,int Lx){
	MFIELDS[0].BSetBar(x,y,Lx);
	MFIELDS[2].BSetBar(x,y,Lx);
	MFIELDS[3].BSetBar(x,y,Lx);
};
void BClrBar(int x,int y,int Lx){
	MFIELDS[0].BClrBar(x,y,Lx);
	MFIELDS[2].BClrBar(x,y,Lx);
};
void BClrTotalBar(int x,int y,int Lx){
	MFIELDS[0].BClrBar(x,y,Lx);
	MFIELDS[2].BClrBar(x,y,Lx);
	MFIELDS[3].BClrBar(x,y,Lx);
};
void BXorBar(int x,int y,int Lx){
	MFIELDS->BXorBar(x,y,Lx);
};
void BSetSQ(int x,int y,int Lx,int Ly){
	MFIELDS->BSetSQ(x,y,Lx,Ly);
};
void BClrSQ(int x,int y,int Lx,int Ly){
	MFIELDS->BClrSQ(x,y,Lx,Ly);
};
int CheckPt(int x,int y){
	return MFIELDS->CheckPt(x,y);
};
int CheckHLine(int x,int y,int Lx){
	return MFIELDS->CheckHLine(x,y,Lx);
};
int CheckVLine(int x,int y,int Lx){
	return MFIELDS->CheckVLine(x,y,Lx);
};
bool CheckBar(int x,int y,int Lx,int Ly){
	return MFIELDS->CheckBar(x,y,Lx,Ly);
};
byte NewCirc[16];

void SetLock(int x,int y,char val){
	if(val){
		MFIELDS->BSetBar(x<<1,y<<1,2);
	}else{
		MFIELDS->BClrBar(x<<1,y<<1,2);
	};
};
//------------------------------------------------//
//          KERNEL OF THE MOTION ENGINE           //
//            Search for the best way             //
//------------------------------------------------//
#define MaxP (4096)
word pxx[MaxP+160];
word pyy[MaxP+160];
word RVIS[MaxP+160];
word LVIS[MaxP+160];
int GetLAngle(int dx,int dy,int Angle);
int GetRAngle(int dx,int dy,int Angle);
//const int ndrr[9]={7,6,5,0,0,4,1,2,3};
//const nidrx[8]={0,1,1,1,0,-1,-1,-1};
//const nidry[8]={-1,-1,0,1,1,1,0,-1};



int div24(int y){
	return y>>4;
};
int Prop43(int y){
	//return div(y<<2,3).quot;
	return y;//<<1;
};
void CBar(int x,int y,int Lx,int Ly,byte c);
extern byte LockGrid;
extern bool VHMode;
void TopShow();
void xLine(int,int,int,int,byte);
void ShowAString(int,int,RLCFont*,byte,char*,...);

void Skos3D(Vector3D& V){
	V.y-=V.z/2;
	V.z*=0.866025f;
}
DWORD CC1[]={0x8000FF00,0x80FF0000,0x80FFFF00,0x80FFFFFF,0x80FFFFFF,0x80FFFFFF,0x80FFFFFF,0x80FFFFFF,0x80FFFFFF,0x80FFFFFF};

DWORD GetColorByZ(int z){
	return 0x00FF0000;
	int cidx=z<-128?0:(z+128)/128;
	DWORD C1=CC1[cidx];
	DWORD C2=CC1[cidx+1];
	return MixDWORD(C1,C2,(z&127)*2,255-(z&127)*2);
}
void LittleBias(Vector3D& V){
	V.y+=12;
	V.z+=6;
}

//#include "CurrentMapOptions.h"

void LShow(){
	TopShow();	
	if(LMode)return;	
	if(!LockGrid)return;
    rsEnableZ(false);

	///ShowAString(RealLx-150,50,&YellowFont,0,"MotionField: %d",LockGrid-1);

	extern float fMapX;
	extern float fMapY;

	const RECT rc = MOptions.LimitCamArgs.GetRect();
	const cBounds rcFixed = MOptions.LimitCamArgs.GetRectFixed();

	mapx = int(fMapX);
	mapy = int(fMapY);

	mapx -= rc.right - rcFixed.GetMax().x;
	mapy -= rc.bottom - rcFixed.GetMax().y;

	int ox=mapx*32;
	int oy=mapy*32;

	MotionField* MFI=MFIELDS+LockGrid-1;
	//if(GetKeyState(VK_CONTROL)&0x8000)MFI=&UnitsField;
	static int s=IRS->GetShaderID("locking");	

	GPS.EnableZBuffer(1);
	rsSetShader(s);

	int x1=smaplx*2+30;
	for(int ix=-20;ix<x1;ix++){
		int iy=-40;
		bool nydone=true;
		do{			
			int xx0=mapx+mapx+ix;
			int yy0=mapy+mapy+iy;
			bool res0=MFI->CheckPt(xx0,yy0);
			bool res1=MFI->CheckPt(xx0+1,yy0);
			bool res2=MFI->CheckPt(xx0,yy0+1);
			bool res3=MFI->CheckPt(xx0+1,yy0+1);
			int yy=smapy+(mul3(iy)<<2);
			int yy1=yy;
			if(Mode3D){
				yy1-=GetUnitHeight(ox+(ix<<4),oy+(iy<<4));
			};			
			if(res0||res1||res2||res3){
				int dz1=0;
				int dz2=0;
				int dz3=0;
				int dz4=0;
				int Dz1=0;
				int Dz2=0;
				int Dz3=0;
				int Dz4=0;
				if(Mode3D){
					dz1=GetUnitHeight(ox+(ix<<4)+8,oy+(iy<<4)+8);
					dz2=GetUnitHeight(ox+(ix<<4)+16+8,oy+(iy<<4)+8);
					dz3=GetUnitHeight(ox+(ix<<4)+8,oy+(iy<<4)+16+8);
					dz4=GetUnitHeight(ox+(ix<<4)+16+8,oy+(iy<<4)+16+8);
					Dz1=dz1;
					Dz2=dz2;
					Dz3=dz3;
					Dz4=dz4;
				}else{
					Dz1=GetUnitHeight(ox+(ix<<4)+8,oy+(iy<<4)+8);
					Dz2=GetUnitHeight(ox+(ix<<4)+16+8,oy+(iy<<4)+8);
					Dz3=GetUnitHeight(ox+(ix<<4)+8,oy+(iy<<4)+16+8);
					Dz4=GetUnitHeight(ox+(ix<<4)+16+8,oy+(iy<<4)+16+8);
				};
				if(dz1<0)dz1=0;
				if(dz2<0)dz2=0;
				if(dz3<0)dz3=0;
				if(dz4<0)dz4=0;
				dz1+=2;
				dz2+=2;
				dz3+=2;
				dz4+=2;
				int xx=smapx+(ix<<4);				
				Vector3D V1(mapx*32+ix*16+8,mapy*32+iy*16+8,dz1);
				Vector3D V2(mapx*32+ix*16+16+8,mapy*32+iy*16+8,dz2);
				Vector3D V3(mapx*32+ix*16+8,mapy*32+iy*16+16+8,dz3);
				Vector3D V4(mapx*32+ix*16+16+8,mapy*32+iy*16+16+8,dz4);				
				//Skos3D(V1);Skos3D(V2);Skos3D(V3);Skos3D(V4);			
				//LittleBias(V1);LittleBias(V2);LittleBias(V3);LittleBias(V4);
				byte V=(res0?1:0)+(res1?2:0)+(res2?4:0)+(res3?8:0);
				DWORD LColor=0;
				if(V==11||V==2||V==13||V==4||V==9){
					rsQuad(V2,V1,V4,V3,GetColorByZ(Dz2)+(res1?0x8F000000:LColor),GetColorByZ(Dz1)+(res0?0x8F000000:LColor)
						,GetColorByZ(Dz4)+(res3?0x8F000000:LColor),GetColorByZ(Dz3)+(res2?0x8F000000:LColor));
				}else{
					rsQuad(V1,V2,V3,V4,GetColorByZ(Dz1)+(res0?0x8F000000:LColor),GetColorByZ(Dz2)+(res1?0x8F000000:LColor)
						,GetColorByZ(Dz3)+(res2?0x8F000000:LColor),GetColorByZ(Dz4)+(res3?0x8F000000:LColor));
				};
			}
#ifndef _USE3D
			if(yy1>WindY1)nydone=false;
#else
			if (yy1-256 > RealLy) nydone = false;
#endif // _USE3D
			iy++;
		}while(nydone);
	};

	rsFlushPoly3D();	
	rsRestoreShader();
	GPS.EnableZBuffer(0);
};//10251
extern int* DMap;
extern int  DMapSize;
extern byte TopTypeMode;
extern DWORD TXCOLOR[64];
void Draw3DLine(int x0,int y0,int x1,int y1,DWORD Color){	
	int dx=mapx<<5;
	int dy=mapy<<5;
	GPS.DrawLine(x0-dx,(y0-dy)/2-(Mode3D?GetHeight(x0,y0):0),x1-dx,(y1-dy)/2-(Mode3D?GetHeight(x1,y1):0),Color);
}
void DrawWorldLine(float x0,float y0,float x1,float y1,DWORD Diffuse);
void Draw3DLineEx(int x0,int y0,int x1,int y1,DWORD Color){
	int R=Norma(x0-x1,y0-y1);
	int N=(R/20)+1;
	int xp=x0;
	int yp=y0;	
	for(int i=1;i<=N;i++){
		int xx=x0+(x1-x0)*i/N;
		int yy=y0+(y1-y0)*i/N;
		DrawWorldLine(xp,yp,xx,yy,Color);
		xp=xx;
		yp=yy;
	}
}
void TopShow(){
	if(LMode)return;
	if(!TopTypeMode)return;

	ShowAString(RealLx-150,70,&YellowFont,0,"Top Type: %d",TopTypeMode-1);
	
	byte TopType=TopTypeMode-1;

	int ox=(mapx&0xFFFE)<<5;
	int oy=(mapy&0xFFFE)<<5;
	int nx=(smaplx>>1)+1;
	int stx=mapx>>1;
	int sty=mapy>>1;
	int N=0;
	for(int ix=0;ix<nx;ix++){
		int iy=0;
		bool nydone=true;
		do{
			int xx0=stx+ix;
			int yy0=sty+iy;
			int yy=smapy+(mul3(iy)<<4);
			int yy1=yy;
			int rx=ox+(ix<<6);
			int ry=oy+(iy<<6);
			int top=GetTopRef(xx0+yy0*TopLx,TopType);
			if(top<0xFFFE){
				DWORD tc=TXCOLOR[(top&63)]|0xFF000000;				
				//Draw3DLine(rx,ry,rx+64,ry+64,tc);				
				//Draw3DLine(rx+64,ry,rx,ry+64,tc);				
				N++;				
				if(GetTopRef(xx0+1+yy0*TopLx,TopType)!=top)Draw3DLine(rx+63,ry,rx+63,ry+63,0xFFFFFFFF);
				if(xx0>0&&GetTopRef(xx0-1+yy0*TopLx,TopType)!=top)Draw3DLine(rx,ry,rx,ry+63,0xFFFFFFFF);
				if(GetTopRef(xx0+yy0*TopLx+TopLx,TopType)!=top)Draw3DLine(rx,ry+63,rx+63,ry+63,0xFFFFFFFF);
				if(yy0>0&&GetTopRef(xx0+yy0*TopLx-TopLx,TopType)!=top)Draw3DLine(rx,ry,rx+63,ry,0xFFFFFFFF);				
				//yy+=32;
			}else{
				DWORD tc=0xFFD00000;				
				Draw3DLine(rx,ry,rx+64,ry+64,tc);
				Draw3DLine(rx+64,ry,rx,ry+64,tc);				
			}
			if (yy1 > RealLy) nydone = false;
			iy++;
		}while(nydone);
	};
	if(DMap){
		oy>>=1;
		int NA=GetNAreas(TopType);
		int dx=mapx<<5;
		int dy=mapy<<5;
		bool Links=(GetKeyState(VK_MENU)&0x8000)!=0;
		for(int i=0;i<NA;i++){
			char cc[64];
			sprintf(cc,"Z%d:%d",i,DMap[i]);
			Area* AR=GetTopMap(i,TopType);
			int x=(AR->x<<6)+32;
			int y=(AR->y<<6)+32;
			for(int j=0;j<AR->NLinks;j++){
				int tz=AR->Link[j].NextAreaID;
                Area* AR1=GetTopMap(tz,TopType);
				int x1=(AR1->x<<6)+32;
				int y1=(AR1->y<<6)+32;			
				if(Links){
					int GetNextWayPoints(int From, int Dest, int *PointsXYList);
					bool CheckIfRoadZone(int Zone);
					if(CheckIfRoadZone(i)&&CheckIfRoadZone(tz)&&GetNextWayPoints(i,tz,NULL))Draw3DLine(x,y,x1,y1,0xFF00FFFF);
					else Draw3DLine(x,y,x1,y1,0xFFFFFF00);
				}
			}
			Draw3DLine(x-32,y-32,x+32,y+32,0x80FF00FF);
			Draw3DLine(x+32,y-32,x-32,y+32,0x80FF00FF);
			ShowString(x-dx,(y-dy)/2-(Mode3D?GetHeight(x,y):0),cc,&BigYellowFont);
			
		}
	}
}
//------------------ATTACK POINT---------------------//
//it is used for the mortira,pushka(AI)
#define ATTP_ALLOWMOTION	1
#define ATTP_FINDPLACE		2
#define ATTP_IMMEDIATECHECK 4
#define ATTP_SINGLESHOT     8
#define ATTP_IMMEDIATEFINDPLACE 16
//return values:
//>0-ready
//-1-Too far or too near
//-2-prepiatsvia
int PredictShot(Weapon* Weap,int xs,int ys,int zs,int xd,int yd,int zd,word Index);
int PredictShotEx(Weapon* Weap,int xs,int ys,int zs,int xd,int yd,int zd,word Index);
int CheckDamageAbility(OneObject* OB,int x,int y,int z,byte Nation,int Soft){
	//1.Let us determine weapon
	int ox=OB->RealX>>4;
	int oy=OB->RealY>>4;
	int oz=OB->RZ+OB->newMons->SrcZPoint;
	int dst=Norma(x-ox,y-oy);
	if(Soft)dst-=48;
	int dstx=dst+((z-oz)<<1);
	if(dstx<0)dstx=0;
	int NeedState=-1;
	NewMonster* NMN=OB->newMons;
	AdvCharacter* ADC=OB->MoreCharacter;
	bool NWATER=!OB->LockType;
	int DRMAX=0;
	if(Soft==3)DRMAX=280;
	for(int i=0;i<NAttTypes;i++){
		int wpk=NMN->WeaponKind[i];
		if(NWATER&&WeaponFlags[wpk]&1){
			if(dst>ADC->AttackRadius1[i]&&dstx<ADC->AttackRadius2[i]){
				NeedState=i;
			};
		}else{
			int R2=ADC->AttackRadius2[i];
			if(R2)R2+=DRMAX;
			if(dst>=ADC->AttackRadius1[i]&&dst<=R2){
				NeedState=i;
			};
		};
	};
	if(NeedState==-1)return -1;
	Weapon* WP=NMN->DamWeap[NeedState];
	if(!WP)return -1;
	int res=PredictShot(WP,ox,oy,oz,x,y,z,OB->Index);
	if(res==-1)return NeedState;
	if(res>=8192)return -2;
	OneObject* OBJ=Group[res];
	if(OBJ&&OBJ->NNUM!=Nation)return NeedState;
	return -2;
	
};
int FastCheckDamageAbility(OneObject* OB,int x,int y,int z,byte Nation,int Soft){
	//1.Let us determine weapon
	int ox=OB->RealX>>4;
	int oy=OB->RealY>>4;
	int oz=OB->RZ+OB->newMons->SrcZPoint;
	int dst=Norma(x-ox,y-oy);
	if(Soft)dst-=48;
	int dstx=dst+((z-oz)<<1);
	if(dstx<0)dstx=0;
	int NeedState=-1;
	NewMonster* NMN=OB->newMons;
	AdvCharacter* ADC=OB->MoreCharacter;
	bool NWATER=!OB->LockType;
	int DRMAX=0;
	if(Soft==3)DRMAX=280;
	for(int i=0;i<NAttTypes;i++){
		int wpk=NMN->WeaponKind[i];
		if(NWATER&&WeaponFlags[wpk]&1){
			if(dst>ADC->AttackRadius1[i]&&dstx<ADC->AttackRadius2[i]){
				NeedState=i;
			};
		}else{
			int R2=ADC->AttackRadius2[i];
			if(R2)R2+=DRMAX;
			if(dst>=ADC->AttackRadius1[i]&&dst<=R2){
				NeedState=i;
			};
		};
	};
	if(NeedState==-1)return -1;
	Weapon* WP=NMN->DamWeap[NeedState];
	if(!WP)return -1;
	int res=PredictShot(WP,ox,oy,oz,x,y,z,OB->Index);
	if(res==-1)return NeedState;
	if(res>=8192)return -2;
	OneObject* OBJ=Group[res];
	if(OBJ&&OBJ->NNUM!=Nation)return NeedState;
	return -2;
	
};
int FindPlaceForAttack(OneObject* OB,int x,int y,int z,byte Nation,
					   int MaxSteps,int SearchMethod,int* Newx,int* Newy){
	int OldRX=OB->RealX;
	int OldRY=OB->RealY;
	int myx=OldRX>>4;
	int myy=OldRY>>4;
	int DX=x-myx;
	int DY=y-myy;
	int N=Norma(DX,DY);
	if(!(DX||DY))return 1;
	DX=(DX<<5)/N;
	DY=(DY<<5)/N;
	DX+=(rando()&15)-8;
	DY+=(rando()&15)-8;
	if(SearchMethod==-1){
		//move forward
		myx+=DX;
		myy+=DY;
		MaxSteps<<=1;
		for(int i=0;i<MaxSteps;i++){
			if(CheckPt(myx>>4,myy>>4)){
				OB->RealX=OldRX;
				OB->RealY=OldRY;
				addrand(OB->RealX);addrand(OB->RealY);
				return SearchMethod;
			}else{
				OB->RealX=(myx<<4);
				OB->RealY=(myy<<4);
				addrand(OB->RealX);addrand(OB->RealY);
				int r=CheckDamageAbility(OB,x,y,z,Nation,false);
				if(!r){
					OB->RealX=OldRX;
					OB->RealY=OldRY;
					addrand(OB->RealX);addrand(OB->RealY);
					*Newx=myx;
					*Newy=myy;
					return 0;
				};
			};
			myx+=DX;
			myy+=DY;
		};
	}else{
		//strafe
		int lfx=myx+DY;
		int lfy=myy-DX;
		int rfx=myx-DY;
		int rfy=myy+DX;
		bool LMove=true;
		bool RMove=true;
		for(int i=0;i<MaxSteps&&(LMove||RMove);i++){
			if(LMove){
				if(!CheckPt(lfx>>4,lfy>>4)){
					OB->RealX=lfx<<4;
					OB->RealY=lfy<<4;
					addrand(OB->RealX);addrand(OB->RealY);
					int r=CheckDamageAbility(OB,x,y,z,Nation,false);
					if(!r){
						OB->RealX=OldRX;
						OB->RealY=OldRY;
						addrand(OB->RealX);addrand(OB->RealY);
						*Newx=lfx;
						*Newy=lfy;
						return 0;
					};	
				}else LMove=false;
				lfx+=DY;
				lfy-=DX;
			};
			if(RMove){
				if(!CheckPt(rfx>>4,rfy>>4)){
					OB->RealX=rfx<<4;
					OB->RealY=rfy<<4;
					addrand(OB->RealX);addrand(OB->RealY);
					int r=CheckDamageAbility(OB,x,y,z,Nation,false);
					if(!r){
						OB->RealX=OldRX;
						OB->RealY=OldRY;
						addrand(OB->RealX);addrand(OB->RealY);
						*Newx=rfx;
						*Newy=rfy;
						return 0;
					};	
				}else RMove=false;
				rfx+=DY;
				rfy-=DX;
			};
		};
	};
	OB->RealX=OldRX;
	OB->RealY=OldRY;
	addrand(OB->RealX);addrand(OB->RealY);
	return SearchMethod;
};
//only for pushka and mortira
void CreateRazbros(OneObject* OBJ,Coor3D* C3D);
bool MoveAwayInSector(int x,int y,int r0,int r1,char Dir,int ddir);
void AI_AttackPointLink(OneObject* OBJ){
	if(OBJ->delay)return;
	int dax=OBJ->LocalOrder->info.AttackXY.ox;
	int day=OBJ->LocalOrder->info.AttackXY.oy;
	int x=OBJ->LocalOrder->info.AttackXY.x;
	int y=OBJ->LocalOrder->info.AttackXY.y;
	int z=OBJ->LocalOrder->info.AttackXY.z;
	int myx=OBJ->RealX>>4;
	int myy=OBJ->RealY>>4;
	int RRot=OBJ->newMons->MinRotator;
	int rot2=RRot<<1;
	if(rot2>16)rot2=16;
	if(Norma(dax-myx,day-myy)<32){
		//rotation
		char Ndir=char(GetDir(x-myx,y-myy));
		char ddir=Ndir-OBJ->RealDir;
		if(abs(ddir)<rot2){
			//can make shot now
			//1.let us determine method of attack
			int r=CheckDamageAbility(OBJ,x,y,z,OBJ->NNUM,true);
			if(r<0){
				OBJ->DeleteLastOrder();
				return;
			};
			NewMonster* NMN=OBJ->newMons;
			if(NMN->Usage!=SupMortID&&NMN->Usage!=MortiraID){
				if(MoveAwayInSector(OBJ->RealX,OBJ->RealY,16*16,200*16,OBJ->RealDir,33))return;
			};
			//2.checking of resources
			bool AllowShot=true;
			AdvCharacter* ADC=OBJ->MoreCharacter;
			if(NMN->NShotRes){
				for(int k=0;k<NMN->NShotRes;k++){
					if(XRESRC(OBJ->NNUM,NMN->ShotRes[k+k])<NMN->ShotRes[k+k+1])AllowShot=false;
				};
				if(AllowShot){
					for(int k=0;k<NMN->NShotRes;k++){
						AddXRESRC(OBJ->NNUM,NMN->ShotRes[k+k],-NMN->ShotRes[k+k+1]);
						OBJ->Nat->AddResource(byte(NMN->ShotRes[k+k]),-NMN->ShotRes[k+k+1]);
						OBJ->Nat->ResOnLife[NMN->ShotRes[k+k]]+=NMN->ShotRes[k+k+1];
					};
				};
			};
			//3.source point
			OBJ->delay=ADC->AttackPause[r];
			OBJ->MaxDelay=div(int(OBJ->delay)*OBJ->PersonalDelay,100).quot;
			OBJ->MaxDelay=int(OBJ->MaxDelay)*100/OBJ->GetAttackSpeedBonus();
			OBJ->delay=OBJ->MaxDelay;
			addrand(OBJ->delay);
			Weapon* WP=NMN->DamWeap[r];
			//pubuxxxxxxxxx...
			if(WP){
				//----------
				NewAnimation* UNAM=NMN->GetAnimation(anm_Attack+r);
				int uocts=(UNAM->Rotations-1)*2;
				int uoc2=UNAM->Rotations-1;
				if(!uocts)uocts=1;
				int usesize=div(255,uocts<<1).quot;
				byte udir=(((OBJ->RealDir+64+usesize)&255)*uocts)>>8;
				NewFrame* NF=UNAM->Frames[0];
				//----------
				NewAnimation* NAM=WP->NewAnm;
				int octs=(NAM->Rotations-1)*2;
				int oc2=NAM->Rotations-1;
				if(!octs)octs=1;
				int sesize=div(255,octs<<1).quot;
				byte dir=(((OBJ->RealDir+64+sesize)&255)*octs)>>8;
				int x0,y0,z0,x1,y1,z1;
				if(udir<uoc2){
					int udir1=uoc2-udir;
					x0=(OBJ->RealX>>4)-(UNAM->ActivePt[udir1]->x+NF->dx);
					y0=(OBJ->RealY>>4)+(UNAM->ActivePt[udir1]->y+NF->dy+NMN->SrcZPoint)*2;
				}else{
					int udir1=uoc2-uocts+udir;
					x0=(OBJ->RealX>>4)+(UNAM->ActivePt[udir1]->x+NF->dx);
					y0=(OBJ->RealY>>4)+(UNAM->ActivePt[udir1]->y+NF->dy+NMN->SrcZPoint)*2;
				};
				Coor3D C3D;
				C3D.x=x;
				C3D.y=y;
				C3D.z=z;
				CreateRazbros(OBJ,&C3D);
				z0=OBJ->RZ+NMN->SrcZPoint;
				if(udir<uoc2){
					udir=uoc2-udir;
					x1=C3D.x;
					y1=C3D.y;
					z1=C3D.z+NMN->DstZPoint;
					Create3DAnmObject(WP,x0,y0,z0,x1,y1,z1,OBJ,OBJ->NewState-1,0xFFFF);
				}else{
					udir=uoc2-uocts+udir;
					x1=C3D.x;
					y1=C3D.y;
					z1=C3D.z+NMN->DstZPoint;
					Create3DAnmObject(WP,x0,y0,z0,x1,y1,z1,OBJ,OBJ->NewState-1,0xFFFF);
				};
			};
			if(OBJ->LocalOrder->info.AttackXY.wep!=255)OBJ->DeleteLastOrder();
			return;
		}else{
			//Need to rotate
			if(ddir>0)OBJ->RealDir+=RRot;
			else OBJ->RealDir-=RRot;
			OBJ->GraphDir=OBJ->RealDir;
			OBJ->DeletePath();
			OBJ->DestX=-1;
			OBJ->DestY=-1;
		};
	}else{
		OBJ->CreatePath(dax>>4,day>>4);
	};
};

bool OneObject::AttackPoint(int x,int y,int z,byte Times,byte Flags,byte OrdType,byte Prio){
	int ImmCanDam=CheckDamageAbility(this,x,y,z,NNUM,false);
	if((delay&&Times!=255)||LocalOrder&&LocalOrder->DoLink==&AI_AttackPointLink)return false;
	if((Flags&ATTP_IMMEDIATECHECK)&&ImmCanDam<0)return false;
	int r=0;
	int Newx=-1;
	int Newy=-1;
	if(ImmCanDam<0)r=FindPlaceForAttack(this,x,y,z,NNUM,30,ImmCanDam,&Newx,&Newy);
	else{
		Newx=RealX>>4;
		Newy=RealY>>4;
	};
	if((ImmCanDam<0)&&(Flags&ATTP_IMMEDIATEFINDPLACE)&&r)return false;
	addrand(1016);
	Order1* OR1=CreateOrder(OrdType);
	if(!OR1)return false;
	OR1->info.AttackXY.ox=Newx;
	OR1->info.AttackXY.oy=Newy;
	OR1->DoLink=&AI_AttackPointLink;
	OR1->OrderType=177;
	OR1->OrderTime=0;
	OR1->info.AttackXY.x=x;
	OR1->info.AttackXY.y=y;
	OR1->info.AttackXY.z=z;
	OR1->info.AttackXY.wep=Times;
	return true;
};
//Special for pushek
int GetEnemyDifference(int cell,byte Mask){
	int NHim=0;
	int NMy=0;
	cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(OB->NMask&Mask)NMy++;
				else NHim++;
			};
		};
	};
	if(NMy>5||NHim<3||NHim<=NMy)return -1;
    return NHim-NMy;
};

//bool PushUnitsInCell(int cell,int x0,int y0
//bool PushUnitsAway(OnObject* OB){
//
//};
void SearchEnemyForAIArtillery(OneObject* OB){
	if(OB->delay)return;
	//if(rando()<20000)return;
    int cell=((OB->RealY>>11)<<VAL_SHFCX)+(OB->RealX>>11);
    NewMonster* NM=OB->newMons;
	AdvCharacter* ADC=OB->MoreCharacter;
    int rr=ADC->MaxR_Attack;
	int rx0=(OB->RealX>>4)+64;
	int ry0=(OB->RealY>>4)+64;
	int x0=OB->RealX>>11;
	int y0=OB->RealY>>11;
	EnemyInfo* EIN=GNFO.EINF[OB->NNUM];
	//search for towers
	DangerInfo* DF=EIN->DINF;
	int xxx=OB->RealX>>4;
	int yyy=OB->RealY>>4;
	for(int p=0;p<EIN->NDINF;p++){
		if(Norma(DF->x-xxx,DF->y-yyy)<rr){
			OB->AttackPoint(DF->x,DF->y,DF->z+32,1,ATTP_IMMEDIATEFINDPLACE,1,128+64);
			if(OB->LocalOrder&&OB->LocalOrder->DoLink==&AI_AttackPointLink)return;
		};
		DF++;
	};
	//-----------------
    int rx1=(rr>>7)+1;
	int EnX=-1;
	int EnY=-1;
    int mindist=10000000;
    int rx2=rx1+rx1+1;
    int stcell;
	int MaxDif=-1;
    for(int i=0;i<64;i++){
        int dx=((rando()*rx2)>>15)-rx1;
        int dy=((rando()*rx2)>>15)-rx1;
		int x1=rx0+(dx<<7);
		int y1=ry0+(dy<<7);
		if(Norma(x1-rx0,y1-ry0)<rr){
			stcell=cell+dx+(dy<<VAL_SHFCX);
	        if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
		        if(NPresence[stcell]){
			        int N=GetEnemyDifference(stcell,OB->NMask);
				    if(N>MaxDif){
						MaxDif=N;
						EnX=x1;
						EnY=y1;
					};
				 };
			 };
        };
    };
    if(MaxDif>5){
		OB->AttackPoint(EnX,EnY,GetHeight(EnX,EnY)+32,1,ATTP_IMMEDIATEFINDPLACE,1,128+64);
	}else{
		//search of wall to erase
		if(rando()<4096){
			rr-=510;
			for(int p=0;p<EIN->NEnWalls;p++){
				int xx1=(int(EIN->WallsX[p])<<6)+32;
				int yy1=(int(EIN->WallsY[p])<<6)+32;

				if(Norma(xx1-xxx,yy1-yyy)<rr){
					OB->AttackPoint(xx1,yy1,GetHeight(xx1,yy1)+5,1,ATTP_IMMEDIATEFINDPLACE,1,128+64);
					if(OB->LocalOrder&&OB->LocalOrder->DoLink==&AI_AttackPointLink)return;
				};
				DF++;
			};
		};
	};
};
void SearchEnemyForMortira(OneObject* OB){
	if(OB->delay)return;
	//if(rando()<20000)return;
    int cell=((OB->RealY>>11)<<VAL_SHFCX)+(OB->RealX>>11);
    NewMonster* NM=OB->newMons;
	AdvCharacter* ADC=OB->MoreCharacter;
    int rr=ADC->MaxR_Attack+400;
	int rx0=(OB->RealX>>4)+64;
	int ry0=(OB->RealY>>4)+64;
	int x0=OB->RealX>>11;
	int y0=OB->RealY>>11;
	int xxx=OB->RealX>>4;
	int yyy=OB->RealY>>4;
	EnemyInfo* EIN=GNFO.EINF[OB->NNUM];
	if(rando()<8192){
		rr-=510;
		for(int p=0;p<EIN->NEnWalls;p++){
			int xx1=(int(EIN->WallsX[p])<<6)+32;
			int yy1=(int(EIN->WallsY[p])<<6)+32;
			if(Norma(xx1-xxx,yy1-yyy)<rr){
				OB->AttackPoint(xx1,yy1,GetHeight(xx1,yy1)+5,1,ATTP_IMMEDIATEFINDPLACE,1,128+64);
				if(OB->LocalOrder&&OB->LocalOrder->DoLink==&AI_AttackPointLink)return;
			};
		};
	};
	rr-=390;
	//search for towers
	DangerInfo* DF=EIN->DINF;
	for(int p=0;p<EIN->NDINF;p++){
		if(Norma(DF->x-xxx,DF->y-yyy)<rr){
			OB->AttackPoint(DF->x,DF->y,DF->z+32,1,ATTP_IMMEDIATECHECK,1,128+64);
			if(OB->LocalOrder&&OB->LocalOrder->DoLink==&AI_AttackPointLink)return;
		};
		DF++;
	};
	//-----------------
    int rx1=(rr>>7)+1;
	int EnX=-1;
	int EnY=-1;
    int mindist=10000000;
    int rx2=rx1+rx1+1;
    int stcell;
	int MaxDif=-1;
    for(int i=0;i<64;i++){
        int dx=((rando()*rx2)>>15)-rx1;
        int dy=((rando()*rx2)>>15)-rx1;
		int x1=rx0+(dx<<7);
		int y1=ry0+(dy<<7);
		if(Norma(x1-rx0,y1-ry0)<rr){
			stcell=cell+dx+(dy<<VAL_SHFCX);
	        if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
		        if(NPresence[stcell]){
			        int N=GetEnemyDifference(stcell,OB->NMask);
				    if(N>MaxDif){
						MaxDif=N;
						EnX=x1;
						EnY=y1;
					};
				 };
			 };
        };
    };
    if(MaxDif>5){
		OB->AttackPoint(EnX,EnY,GetHeight(EnX,EnY)+32,1,ATTP_IMMEDIATECHECK,1,128+64);
	};
};
void AIArtilleryAgainstTowers(OneObject* OB){
	if(OB->delay)return;
	//if(rando()<20000)return;
    int cell=((OB->RealY>>11)<<VAL_SHFCX)+(OB->RealX>>11);
    NewMonster* NM=OB->newMons;
	AdvCharacter* ADC=OB->MoreCharacter;
    int rr=ADC->MaxR_Attack+900;
	int rx0=(OB->RealX>>4)+64;
	int ry0=(OB->RealY>>4)+64;
	int x0=OB->RealX>>11;
	int y0=OB->RealY>>11;
	//search for towers
	EnemyInfo* EIN=GNFO.EINF[OB->NNUM];
	DangerInfo* DF=EIN->DINF;
	int xxx=OB->RealX>>4;
	int yyy=OB->RealY>>4;
	for(int p=0;p<EIN->NDINF;p++){
		if(Norma(DF->x-xxx,DF->y-yyy)<rr){
			OB->AttackPoint(DF->x,DF->y,DF->z+32,1,ATTP_IMMEDIATEFINDPLACE,1,128+64);
			if(OB->LocalOrder&&OB->LocalOrder->DoLink==&AI_AttackPointLink)return;
		};
		DF++;
	};
};
//---------------------ATTACK------------------------//
//���� ������������
void AttackObjLink(OneObject* OBJ);
void BuildObjLink(OneObject* OBJ);
void ProduceObjLink(OneObject* OBJ);
void WaterAttackLink(OneObject* OBJ);
bool OneObject::AttackObj(word OID,int Prio){
	return AttackObj(OID,Prio,0);
};
bool OneObject::AttackObj(word OID,int Prio,byte OrdType){
	return AttackObj(OID,Prio,OrdType,0);
};
void B_BitvaLink(Brigade* BR);
void GrenaderSupermanLink(OneObject* OBJ);
void EraseBrigade(Brigade* BR);
void SetAttState(Brigade* BR,bool Val);
void PanicUnitLink(OneObject* OBJ);
void LeaveMineLink(OneObject* OB);
void PerformUpgradeLink(OneObject* OBJ);
bool OneObject::AttackObj(word OID,int Prio1,byte OrdType,word NTimes){

	if(NewBuilding&&LocalOrder){
		if(LocalOrder->DoLink==&LeaveMineLink||LocalOrder->DoLink==&PerformUpgradeLink)return false;
		if(!Attack)Prio1=128+1;
	};
	if(ActiveAbility){
		//if( (ActiveAbility->ActiveAbilities.InfluenceMask & ABL_Compassion) ){
			bool curr=false;
			ActiveAbility->ActiveAbilities.modifyCompassion(false,curr); // жалостливый не может атаковать
			if(curr)return false;
		//}
		if((Prio1&128)<16){
			bool inv=false;
			byte ms=0;
			bool EnableSearchVictim=false;
			ActiveAbility->ActiveAbilities.modifyInvisibility(inv,inv,ms,EnableSearchVictim); // приказ невидимке атакует, только если игрок ему это приказал
			if(inv&&!EnableSearchVictim)return false;
		}
	}
	bool CheckIfPossibleToBreakOrder(OneObject* OB);
	//if(!CheckIfPossibleToBreakOrder(this))return false;
	if(Sdoxlo||Hidden||UnlimitedMotion||!Ready)return false;
	if(CheckOrderAbility())return false;
	//if((!(Nat->AI_Enabled||StopBuildMode))&&LocalOrder&&LocalOrder->DoLink==&ProduceObjLink)return false;
	//assert(OID!=0xFFFF);
	byte Prio;
	if(Prio1==254){
		Prio=16+128;
		StandGround=0;
	}else Prio=Prio1;

	// ---
	if(Prio<PrioryLevel/*&&!Attack*/)return false; // --- можно проверять и раньше (vital)
	// ---

	//if((Prio&127)>=16&&!InArmy)StandGround=0;
	if(newMons->MaxInside&&! ( newMons->DamWeap[0] || newMons->MaxDamage[0] || newMons->MaxDamage[1] ) )return false;
	OneObject* EOB=Group[OID];
	if(!EOB){
		//DeleteLastOrder();
		return 0;
	};
	if( EOB->Sdoxlo || EOB->newMons->Immortal ){
		//DeleteLastOrder();
		return 0;
	};
	int RE=Norma(RealX-EOB->RealX,RealY-EOB->RealY)>>4;
	//if(RE>=MoreCharacter->MaxR_Attack){
	//	if(NATIONS[NNUM].AI_Enabled&&EOB->InMotion&&(EOB->EnemyID==0xFFFF)){			
	//		int dde=GetDir(EOB->GetAttX()-RealX,EOB->GetAttY()-RealY);
	//		char ddr=EOB->RealDir-dde;
	//		if(abs(ddr)<64){				
	//			return 0;
	//		}
	//	}
	//}
	int ARMR=newMons->ArmRadius;
	if(ActivityState==2||(ActivityState==0&&GSets.AllowFormationsStatesProcessing))ARMR=2000;
	if(InArmy&&(!RifleAttack)&&((RE>ARMR&&(Prio1&127)<14))){
		if((SearchOnlyThisBrigadeToKill==0xFFFF||SearchOnlyThisBrigadeToKill!=EOB->BrigadeID))return false;
	}
#ifdef COSSACKS2
	//if(newMons->Usage==PushkaID){
	//	NewAttackPoint(EOB->RealX>>4,EOB->RealY>>4,Prio1,0,0);
	//	return true;
	//};
#endif //COSSACKS2
#ifdef SIMPLEMANAGE
	if(newMons->ArmAttack){
		if(Prio<128&&!(ArmAttack||RifleAttack))return false;
	};
#endif //SIMPLEMANAGE
	if(NMask&EOB->NMask&&!(newMons->Priest||newMons->Shaman))return false;
	if(!CheckAttAbility(this,OID)){
		if(OID&&(!(LockType==1||NewBuilding||newMons->Capture))&&EOB->newMons->Capture){
			NewMonsterSmartSendTo(EOB->RealX>>4,EOB->RealY>>4,0,0,128,0);
		};
		return false;
	};
	if( !(newMons->KillMask&EOB->MathMask) ){
		return false;
	}
	if( !(newMons->AttackMask[0]&EOB->MathMask)&&ArmAttack){//1.10.2022 TBH ArmAttack Fix
		return false;
	}
	if(EOB->ActiveAbility){
		//if( (EOB->ActiveAbility->ActiveAbilities.InfluenceMask & ABL_Invulnerability) ){
			bool curr=false;
			EOB->ActiveAbility->ActiveAbilities.modifyInvulnerability(false,curr);
			if(curr)return false;
		//}		
	}

	if(!ext_OnAttemptToAttack(this,EOB))return false;
	//checking if it possible to addack due to amount of attackers
	/*
	if(MoreCharacter->MaxR_Attack<160){
		int NA=ATTLIST.GetNAttackers(EOB);
		if(NA>=newMons->MaxAttackersAmount){
			if(!StandGround){
				int r=Norma(EOB->RealX-RealX,EOB->RealY-RealY)>>4;
				if(r>150&&BrigadeID==0xFFFF){
					if(!(LocalOrder)||(LocalOrder&&LocalOrder->DoLink!=&NewMonsterSendToLink)){
						NewMonsterSendTo(EOB->RealX+(rando()&2047)-1024,EOB->RealY+(rando()&2047)-1024,128+1,1);
					}
				}
			}
			return false;
		}
	}
	*/
	//------------------------------------------------------------
	bool STRELOK=false;
	Brigade* BR=NULL;
	byte Use=newMons->Usage==GrenaderID;
	if(InArmy&&(Use==GrenaderID||Use==ArcherID)&&(EOB->NewBuilding||EOB->Wall)){
		BR=CITY[NNUM].Brigs+BrigadeID;
		SetAttState(BR,0);
		EraseBrigade(BR);
	};
	if(!EOB->newMons->CanBeInFocusOfFormation){
		if((!Zombi)&&InArmy&&Prio1!=15+128&&!(newMons->Artilery||EOB->NewBuilding)){
			BR=CITY[NNUM].Brigs+BrigadeID;
			if(!RifleAttack){
				if(Prio1==254){
					return true;
				}
				else
				{
					if(BR->NewBOrder){
						if(BR->NewBOrder->GetBrigadeOrderID()!=BRIGADEORDER_BITVA)//B_BitvaLink
						{
							if(BR->AttEnm){
								if(Norma(RealX-EOB->RealX,RealY-EOB->RealY)<ARMR*16||(Prio1&128)>=14)
								{
									BR->Bitva();
									return true;
								}else return false;
							}else return true;
						};
					}else{
						BR->Bitva();
						return true;
					};
				};
			}else{
				//if(BR->BOrder&&(BR->BOrder->Prio&127))return true;
				if(BR->NewBOrder&&(BR->NewBOrder->GetBrigadeOrderPrio()&127))return true;
				STRELOK=1;
			};
		};
	}
	if(NoSearchVictim&&Prio<128)return true;
	NoSearchVictim=false;
	OneObject* OB=Group[OID];
	//allow shooting of panikers
	if((!OB)/*||(OB->UnlimitedMotion&&!(OB->LocalOrder&&OB->LocalOrder->DoLink==&PanicUnitLink))*/)return false;

	
	
	if(Prio<5&&EnemyID!=0xFFFF){
		OneObject* EOB=Group[EnemyID];
		if(EOB){
			int dist1=Norma(OB->RealX-RealX,OB->RealY-RealY);
			int dist2=Norma(EOB->RealX-RealX,EOB->RealY-RealY);
			int maxr=MoreCharacter->MaxR_Attack*4/5;
			if(ArmAttack&&!RifleAttack){
				maxr=200;
			}			
			if(dist1<=MoreCharacter->MinR_Attack || dist2<maxr || dist2<=dist1)return false;
		};
	};    
	if(!Ready)return false;
	if(RAmount){
		RAmount=0;
		RType=0xFF;
		GroundState=0;
		addrand(OB->GroundState+1);
	};
	if(!LocalOrder)Attack=false;
	if(!int(OB)||OB->Sdoxlo)return false;
	//Important=true;
	if(Attack){
		if(!(LocalOrder&&LocalOrder->DoLink==&GrenaderSupermanLink)){
			EnemyID=OID;
			EnemySN=OB->Serial;
			PrioryLevel=Prio&127;
			LocalOrder->PrioryLevel=PrioryLevel;
			//SearchSupport(OID);
#ifdef COSSACKS2
			if(RifleAttack)
#endif
			StandTime=0;
			return true;
		};
	};
	if(InPatrol&&OrdType==0)
		OrdType=1;
	if(newMons->ShotDir){
		if(OrdType==0)OrdType=1;
	};
	addrand(1017);
	Order1* Or1=CreateOrder(OrdType);
	if(!int(Or1))return false;
	Or1->PrioryLevel=Prio&127;
	Or1->OrderType=3;//���
	Or1->OrderTime=0;
	Or1->DoLink=AttackLink;
	if(newMons->MultiWp.GetAmount())Or1->DoLink=&WaterAttackLink;
	Or1->info.BuildObj.ObjX=OB->x;
	Or1->info.BuildObj.ObjY=OB->y;
	Or1->info.BuildObj.SN=NTimes;
	Or1->info.BuildObj.AttMethod=0xFF;
	EnemyID=OID;
	EnemySN=OB->Serial;
	Attack=true;
	//if(int(InLineCom))FreeAsmLink();
	if(Prio>=16)MakeOrderSound(this,1);
	RType=0xFF;
	PathDelay=0;
	DestX=-1;
	DeletePath();
#ifdef COSSACKS2
	if(StandGround && (OrdType==0||OrdType==1)){
		AttackObjLink(this);
		if(EnemyID==0xFFFF)return false;
	};
#endif //COSSACKS2	
#ifdef COSSACKS2
	UnitSpeed=64;
	if(RifleAttack)
#endif
	StandTime=0;
	//if(MoreCharacter->MaxR_Attack<180)
    ATTLIST.Add(this,EOB);
	return true;
};	
void NewAttackPointLink(OneObject* OBJ);
bool OneObject::NewAttackPoint(int x,int y,int Prio1,byte OrdType,word NTimes){
	if(Sdoxlo||Hidden||UnlimitedMotion)return false;
	if(CheckOrderAbility())return false;
	if(LocalOrder){
		if(LocalOrder->DoLink==&ProduceObjLink)return false;
		//if(LocalOrder->DoLink==NewAttackPointLink){
		//	if(NewAnm&&NewAnm->ActiveFrame)return false;
		//};

	}
	//assert(OID!=0xFFFF);
	byte Prio;
	if(Prio1==254){
		Prio=16+128;
		StandGround=0;
	}else Prio=Prio1;
	//if((Prio&127)>=16&&!InArmy)StandGround=0;
	if(newMons->MaxInside)return false;
	
	if(Prio<PrioryLevel/*&&!Attack*/)return false;
	
	if(!Ready)return false;
	//if(Weap)return;
	//ClearOrders();
	if(RAmount){
		RAmount=0;
		RType=0xFF;
		GroundState=0;
		addrand(GroundState+1);
	};
	addrand(1018);
	Order1* Or1=CreateOrder(OrdType);
	if(!int(Or1))return false;
	Or1->PrioryLevel=Prio&127;
	Or1->OrderType=76;//���
	Or1->OrderTime=0;
	Or1->DoLink=&NewAttackPointLink;
	
	Or1->info.BuildObj.ObjX=x>>4;
	Or1->info.BuildObj.ObjY=y>>4;
	Or1->info.BuildObj.SN=NTimes;
	Or1->info.BuildObj.AttMethod=0xFF;
	DstX=x<<4;
	DstY=y<<4;
	EnemyID=0xFFFF;
	EnemySN=0xFFFF;
	Attack=0;
	//if(int(InLineCom))FreeAsmLink();
	if(Prio>=16)MakeOrderSound(this,1);
	RType=0xFF;
	if(OrdType==0||OrdType==1)PrioryLevel=Prio&127;
	//if(CrowdRef)CrowdRef->AddToAttackQueue(OID);
	PathDelay=0;
	StandTime=0;
	Attack=1;	
	return true;
};	
//int GetTopDistance(int xa,int ya,int xb,int yb,int LT);
int GetTopDistance(int xa,int ya,int xb,int yb,int LT,byte NI);
void CorrectWarriorPosition(short& x,short& y,int R,char BestDir){
	int cx=x>>3;
	int cy=y>>3;
	int NInSect[8];
	memset(NInSect,0,sizeof NInSect);
	bool WasUnit=0;
	if(cx>=0&&cy>=0&&cx<VAL_MAXCX-1&&cy<VAL_MAXCX-1){
		int cell=cx+(cy<<VAL_SHFCX)+VAL_MAXCX+1;
		int NMon=MCount[cell];
		if(!NMon)return;
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!OB->Sdoxlo){
					int dx=(OB->RealX>>4)-(x<<4)-8;
					int dy=(OB->RealY>>4)-(y<<4)-8;
                    int r=Norma(dx,dy);
					if(r<R&&r>10){
                        int Ang=(GetDir(dx,dy)>>5)&7;
						WasUnit=true;
						NInSect[Ang]++;
					}
				}
			}
		}
	}
	if(!WasUnit)return;
	int BDIR=0;
	int BS=1000;
	for(int i=0;i<8;i++){
		char dir1=(i<<5)+8;
		char dd=abs(int(char(dir1-BestDir-128)));
		if(dd<70){
			if(!NInSect[i]){
                BDIR=dir1;
				BS=0;
				break;
			}
			if(NInSect[i]<BS){
				BS=NInSect[i];
				BDIR=dir1;
			}
		}
	}
	if(BS<1000){
        int x0=(x<<4)+8;
		int y0=(y<<4)+8;
		x0+=(int(TCos[BDIR])*R)/300;
		y0+=(int(TSin[BDIR])*R)/300;
		x=x0>>4;
		y=y0>>4;
	}
}
bool FastFindBestPositionForAttack(OneObject* OB,int* xd,int* yd,int R0);
bool SetDestUnit(OneObject* OBJ,OneObject* OB,short OLDX,short OLDY,short NEWX,short NEWY,int ReqR){
#ifdef COSSACKS2
	//if(GetTopDistance((OBJ->RealX+512)>>10,(OBJ->RealY+512)>>10,(OB->RealX+512)>>10,(OB->RealY+512)>>10,OBJ->LockType,OBJ->NNUM)>4096)return false;
	if(ReqR<180){
		//CorrectWarriorPosition(NEWX,NEWY,ReqR,GetDir(OB->RealX-OBJ->RealX,OB->RealY-OBJ->RealY));
	}
	int xd=NEWX;
	int yd=NEWY;	
	UnLockComplexObject(OB);
	//FindBestPosition(OBJ,&xd,&yd,40);
    FastFindBestPositionForAttack(OBJ,&xd,&yd,40);
	LockComplexObject(OB);
	NEWX=xd;
	NEWY=yd;
#endif //COSSACKS2
	if(!OB->newMons->Artilery){
		int dr=Norma(NEWX-OBJ->x,NEWY-OBJ->y);
		int dr1=Norma(NEWX-OBJ->CPdestX,NEWY-OBJ->CPdestY);
		bool renew=0; 
		if(dr>500){
			if(dr1>30)renew=1;
		}else if(dr>100){
			if(dr1>30)renew=1;
		}else if(dr>50){
			if(dr1>20)renew=1;
		}else if(dr>30&&dr1>10)renew=1;
		if(dr>30){
			if(renew){
				OBJ->PathDelay=0;
				OBJ->CreatePath(NEWX,NEWY);
			}else OBJ->CreatePath(OBJ->CPdestX,OBJ->CPdestY);
			return true;
		};
	};
	if(!OBJ->PathX){
		OBJ->CreatePath(NEWX,NEWY);
		OBJ->LocalOrder->info.BuildObj.ObjX=NEWX;
		OBJ->LocalOrder->info.BuildObj.ObjY=NEWY;
	}else{
		int dis=Norma(OBJ->x-NEWX,OBJ->y-NEWY);
		int dis1=Norma(NEWX-OLDX,NEWY-OLDY);
		int dds=0;
		if(dis>100)dds=20;
		else if(dis>50)dds=10;
		else if(dis>30)dds=5;
		else if(dis>10)dds=3;
		else dds=2;
		if(dis1>=dds){
			OBJ->DeletePath();
			//OBJ->DestX=OB->RealX;
			//OBJ->DestY=OB->RealY;
			OBJ->CreatePath(NEWX,NEWY);
			OBJ->LocalOrder->info.BuildObj.ObjX=NEWX;
			OBJ->LocalOrder->info.BuildObj.ObjY=NEWY;
		}else OBJ->CreatePath(OLDX,OLDY);
	};
	return true;
};

bool Create3DAnmObject(Weapon* Weap,int xs,int ys,int zs,
					                int xd,int yd,int zd,
									OneObject* OB);
extern bool ShotRecommended;
void CreateRazbros(OneObject* OBJ,Coor3D* C3D){
	AdvCharacter* ADC=OBJ->MoreCharacter;
	if(!ADC->Razbros)return;
	int R=Norma(((OBJ->RealX)>>4)-C3D->x,(OBJ->RealY>>4)-C3D->y)>>5;
	C3D->x+=div((int(rando()>>5)-512)*R*ADC->Razbros,32000).quot;
	C3D->y+=div((int(rando()>>5)-512)*R*ADC->Razbros,32000).quot;
};
int CheckPointForDamageAbility(OneObject* OBJ,int x,int y,int z){
	int dst=Norma((OBJ->RealX>>4)-x,(OBJ->RealY>>4)-y);
#ifdef COSSACKS2
	int D=(int(OBJ->RZ+OBJ->newMons->SrcZPoint-z)<<2);
	if(D>0)D=0;
	int dst1=dst-D;
#else //COSSACKS2
	int dst1=dst-(int(OBJ->RZ+OBJ->newMons->SrcZPoint-z)<<1);
#endif //COSSACKS2
	NewMonster* NMN=OBJ->newMons;
	AdvCharacter* ADC=OBJ->MoreCharacter;
    if(dst<ADC->AttackRadius1[0]||dst1>ADC->AttackRadius2[0])return -1;
    int xx,yy,x0,y0;
    OBJ->GetCornerXY(&xx,&yy);
    xx<<=8;
    yy<<=8;
    int mindis=1000000;
    int bsp=0;
    int np=NMN->NShotPt;
    int xx1=x<<4;
    int yy1=y<<4;
	int zzz=NMN->SrcZPoint;
	int DZZ=OBJ->RZ;
    for(int i=0;i<np;i++){
        x0=xx+(NMN->ShotPtX[i]<<4);
        y0=yy+(Prop43(NMN->ShotPtY[i])<<4);
        dst=Norma(x0-xx1,y0-yy1);
        if(dst<mindis){
            mindis=dst;
            bsp=i;
		};
    };
    x0=xx+(NMN->ShotPtX[bsp]<<4);
    y0=yy+(Prop43(NMN->ShotPtY[bsp])<<4)+(zzz<<5);
	Weapon* Weap=NMN->DamWeap[0];
    if(Weap){
		int wx0=x0>>4;
		int wy0=(y0>>4)+6;
		int wz0=3+DZZ+zzz;
		int WRes=PredictShot(Weap,wx0,wy0,wz0,x,y,z,OBJ->Index);
		if(WRes!=-1) return -1;
		return 0;
	};
	return -1;
};
bool TestUnitsInSectorForGrenade(OneObject* GRE);
bool AttGroundMod=0;
void NewAttackPointLink(OneObject* OBJ){
	AttGroundMod=1;
	int DX=OBJ->DestX;
	int DY=OBJ->DestY;
	AttackObjLink(OBJ);
	/*if(DX>0){
		OBJ->DestX=DX;
		OBJ->DestY=DY;
	}*/
#ifdef COSSACKS2
	if(OBJ->newMons->Usage!=PushkaID){
		//OBJ->DestX=-1;
		//OBJ->DestY=-1;
	};
#endif
	AttGroundMod=0;
};
extern int ADDRANGE;
extern int ADDPAUSE;
bool Create3DAnmObjectEX(Weapon* Weap,int xs,int ys,int zs1,
					                int xd,int yd,int zd,
									OneObject* OB,byte AttType,word DestObj,
									int vx,int vy,int vz,int GRDZ);
/*
int CheckBuildingDamagePoint(word NIndex,int dx,int dy){
	if(NMN->ShotDir){
		int xx,yy;
		OBJ->GetCornerXY(&xx,&yy);
		xx<<=4;
		yy<<=4;
		int N=NMN->NShotPt;
		int dst=Norma(OBJ->RealX-OB->RealX,OBJ->RealY-OB->RealY)>>4;
		if(dst<=OBJ->MoreCharacter->MinR_Attack||
			dst>OBJ->MoreCharacter->MaxR_Attack){
			OBJ->DeleteLastOrder();
			return;
		};
			for(int i=0;i<N;i++){
				if(!OBJ->TempDelay[i]){
					int x=xx+NMN->ShotPtX[i];
					int y=NMN->ShotPtY[i];
					int z=NMN->ShotPtZ[i];
					int dz=NMN->ShotPtYG[i]-y-z;
					y=yy+((y+z)<<1);
					z+=OBJ->RZ;
					char dir=NMN->ShotDir[i];
					byte diff=NMN->ShotDiff[i];
					int minr=NMN->ShotMinR[i];
					int maxr=NMN->ShotMaxR[i];

					int rx=OB->RealX>>4;
					int ry=OB->RealY>>4;
						
					int dst=Norma(x-rx,y-ry);
					char rdir=char(GetDir(rx-x,ry-y));
					int ddir=abs(char(rdir-dir));
					if(dst>=minr&&dst<=maxr&&ddir<diff){
						//can shot
						Weapon* Weap=NMN->DamWeap[NMN->ShotAttType[i]];
						if(Weap){
};
*/
int TestCapture(OneObject* OBJ);
void AttackByBuildingLink(OneObject* OBJ){
	NewMonster* NMN=OBJ->newMons;
	AdvCharacter* ADC=OBJ->MoreCharacter;
	word OID=OBJ->EnemyID;
	word OSN=OBJ->EnemySN;
	if(OID>=ULIMIT&&!AttGroundMod){
		OBJ->DeleteLastOrder();
		return;
	};
	OneObject TMPOBJ;
    TMPOBJ.pGRP_IDS=NULL;
	OneObject* OB;
#ifdef SIMPLEMANAGE
	bool ARMONLY=OBJ->ArmAttack&&!(OBJ->RifleAttack||AttGroundMod);
	bool SHOTONLY=OBJ->RifleAttack&&!(OBJ->ArmAttack||AttGroundMod);
#else //SIMPLEMANAGE
	bool ARMONLY=OBJ->ArmAttack;
#endif //SIMPLEMANAGE
	
	if(AttGroundMod){
		memset(&TMPOBJ,0,sizeof OneObject);
		OB=&TMPOBJ;
		TMPOBJ.RealX=OBJ->DstX;
		TMPOBJ.RealY=OBJ->DstY;
		TMPOBJ.x=OBJ->DstX>>8;
		TMPOBJ.y=OBJ->DstY>>8;
		TMPOBJ.RZ=GetHeight(OBJ->RealX>>4,OBJ->RealY>>4);
		addrand(TMPOBJ.RZ);
		TMPOBJ.Index=0xFFFF;
		TMPOBJ.Serial=0xFFFF;
		TMPOBJ.Ref.General=NATIONS[7].Mon[0];
		TMPOBJ.newMons=NATIONS[7].Mon[0]->newMons;
		TMPOBJ.NIndex=7;
		TMPOBJ.LockType=OBJ->LockType;
		OID=0xFFFF;
		OSN=0xFFFF;
	}else OB=Group[OID];
	if(NMN->CompxUnit&&NMN->CompxUnit->CanTakeExRes){
		if(OB&&OB->Sdoxlo&&OB->Serial==OSN&&OB->newMons->MeatTransformIndex!=0xFF){
			ObjCharacter* OCR=SPECIAL.Objects[OB->newMons->MeatTransformIndex];
			int rt=OCR->ResType;
			if(rt>=0&&rt<16){
				if(NMN->CompxUnit->TakeResStage[rt]){
					OBJ->DeleteLastOrder();
					for(int i=0;i<60;i++)OBJ->TakeResource(OB->RealX>>4,OB->RealY>>4,rt,16+128,1);
					OBJ->NewMonsterSendTo(OB->RealX,OB->RealY,16+128,1);
					return;
				};
			};
		};
	};
	if(((!OB)||(OB->Sdoxlo>8)||OSN!=OB->Serial)||OBJ->TurnOff){
		if(OBJ->PrioryLevel>=16&&OBJ->NNUM==MyNation&&OBJ->TurnOff){
			char ccc[128];
			sprintf(ccc,GetTextByID("CSNOGL"),OBJ->Ref.General->Message);
			AssignHint1(ccc,50);
		};
		OBJ->DeleteLastOrder();
		return;
	};
	if(ARMONLY&&OB->NewBuilding)ARMONLY=0;
#ifdef SIMPLEMANAGE
	if(SHOTONLY&&OB->NewBuilding)SHOTONLY=0;
	if(ARMONLY){
		if(OBJ->LockType!=OB->LockType&&MFIELDS[OB->LockType].CheckBar(OB->x-1,OB->y,3,3)){
			OBJ->DeleteLastOrder();
			return;
		}
	}
#endif //SIMPLEMANAGE
	if(OBJ->newMons->Shaman&&!(OBJ->NMask&OB->NMask))ARMONLY=1;
	if(NMN->Priest){
		if(!OB->NMask&OBJ->NMask){
			OBJ->DeleteLastOrder();
			return;
		};
	}else{
		if(!NMN->Shaman){
			if(OB->NMask&OBJ->NMask){
				OBJ->DeleteLastOrder();
				return;
			};
		};
	};
	
	
	byte maska=OB->MathMask;
	if(AttGroundMod)maska=0xFF;
	bool killins=0;
	if(OB->newMons->CanBeKilledInside){
		if(NMN->CanFire){
			if(OB->InFire&&OB->NInside)killins=1;
		}else if(OB->NInside)killins=1;
	};
	short OLDX=OBJ->LocalOrder->info.BuildObj.ObjX;
	short OLDY=OBJ->LocalOrder->info.BuildObj.ObjY;
	word NTIMES=OBJ->LocalOrder->info.BuildObj.SN;
	if(OBJ->LocalOrder->NextOrder && OBJ->LocalOrder->NextOrder->DoLink==&ProduceObjLink){
		Order1* OR1=OBJ->LocalOrder;
		OBJ->LocalOrder=OR1->NextOrder;
		OBJ->LocalOrder->DoLink(OBJ);
		OR1->NextOrder=OBJ->LocalOrder;
		OBJ->LocalOrder=OR1;
	}
	short NEWX=OB->RealX>>8;
	short NEWY=OB->RealY>>8;
	if(OBJ->NewBuilding){
		if(OBJ->delay){ 
			//OBJ->delay--;
			return;
		};
		if(NMN->ShotDir){
			if(OBJ->NInside==0&&OBJ->MoreCharacter->MaxInside){
				OBJ->DeleteLastOrder();
				return;
			};
			bool UseEvro=0;
			if(OBJ->Inside){
				word MID=OBJ->Inside[0];
				if(MID!=0xFFFF){
					OneObject* IOB=Group[MID];
					if(IOB)
						UseEvro=!!(IOB->newMons->EuropeanMask);
				};
			};
			int xx,yy;
			OBJ->GetPreciseCornerXY(&xx,&yy);
			int N=NMN->NShotPt;
			int dst=Norma(OBJ->RealX-OB->RealX,OBJ->RealY-OB->RealY)>>4;
			if(dst<=OBJ->MoreCharacter->MinR_Attack||
				dst>OBJ->MoreCharacter->MaxR_Attack){
				OBJ->DeleteLastOrder();
				return;
			};
			int ATTTP=OBJ->newMons->EuropeanMask;
			if(ATTTP==100)UseEvro=0;

			bool COMPSEARCH=OB->NewBuilding;
			bool DELAYPROBLEM=0;
			for(int i=0;i<N;i++){
				if(!OBJ->TempDelay[i]){
					int x=xx+NMN->ShotPtX[i];
					int y=yy+NMN->ShotPtY[i];
					int z=OBJ->RZ+NMN->ShotPtZ[i];
					int dz=0;//NMN->ShotPtYG[i]-y-z;
					//y=yy+((y+z)<<1);
					//y=yy+y;
					//z+=OBJ->RZ;
					char dir=NMN->ShotDir[i];
					byte diff=NMN->ShotDiff[i];
					int minr=NMN->ShotMinR[i];
					int maxr=NMN->ShotMaxR[i];

					int rx=OB->RealX>>4;
					int ry=OB->RealY>>4;
						
					int dst=Norma(x-rx,y-ry);
					char rdir=char(GetDir(rx-x,ry-y));
					int ddir=abs(char(rdir-dir));
					if(dst>=minr&&dst<=maxr&&ddir<diff){
						//can shot
						int sat=NMN->ShotAttType[i];
						if(((!UseEvro)&&sat!=ATTTP)||(UseEvro&&sat==ATTTP)){
							Weapon* Weap=NMN->DamWeap[sat];
							if(Weap){
								Coor3D C3D;
								word DestObj=0xFFFF;
								if(OB->GetDamagePoint(OBJ->x,OBJ->y,&C3D,0))DestObj=OID;
								int wx0=x;
								int wy0=y;
								int wz0=z;
								int wx1=C3D.x;
								int wy1=C3D.y;
								int wz1=C3D.z;
								int WRes=-1;
//	DoShotA:;
								bool AllowShot=true;
								if(NMN->NShotRes){
									for(int k=0;k<NMN->NShotRes;k++){
										if(XRESRC(OBJ->NNUM,NMN->ShotRes[k+k])<NMN->ShotRes[k+k+1])AllowShot=false;
									};
									if(AllowShot){
										for(int k=0;k<NMN->NShotRes;k++){
											AddXRESRC(OBJ->NNUM,NMN->ShotRes[k+k],-NMN->ShotRes[k+k+1]);
											OBJ->Nat->AddResource(byte(NMN->ShotRes[k+k]),-NMN->ShotRes[k+k+1]);
											OBJ->Nat->ResOnLife[NMN->ShotRes[k+k]]+=NMN->ShotRes[k+k+1];
										};
									}else{
										if(OBJ->NNUM==MyNation&&OBJ->PrioryLevel>=16){
											char ccc[150];
											sprintf(ccc,GetTextByID("CSNORS"),OBJ->Ref.General->Message);
											AssignHint1(ccc,64);
										};
										OBJ->DeleteLastOrder();
										return;
									};
								};	
		
								if(AllowShot){
									CreateRazbros(OBJ,&C3D);
									Create3DAnmObjectEX(Weap,wx0,wy0,wz0,wx1,wy1,wz1,OBJ,NMN->ShotAttType[i],DestObj,0,0,0,dz);
									//determining the pause
									if(OBJ->NInside||ADC->MaxInside==0){
										OBJ->delay=ADC->AttackPause[sat];
										OBJ->delay=0;//int(OBJ->delay)*100/OBJ->GetAttackSpeedBonus();
										OBJ->delay=0;//(int(OBJ->delay)*OBJ->PersonalDelay)/(100*OBJ->NInside);
										OBJ->MaxDelay=OBJ->delay;
										OBJ->TempDelay[i]=int(OBJ->newMons->ShotDelay[i])<<8;
										//if(OBJ->newMons->AttackPause[sat]){
										//	OBJ->TempDelay[i]=(int(OBJ->TempDelay[i])*OBJ->delay)/OBJ->newMons->AttackPause[sat];
										//}
									}else{
										OBJ->delay=ADC->AttackPause[sat];
										OBJ->delay=int(OBJ->delay)*100/OBJ->GetAttackSpeedBonus();
										OBJ->delay=(int(OBJ->delay)*OBJ->PersonalDelay)/100;
										OBJ->MaxDelay=OBJ->delay;
										OBJ->DeleteLastOrder();
										return;
									};
									if(NTIMES&&OBJ->LocalOrder){
										OBJ->LocalOrder->info.BuildObj.SN--;
										if(NTIMES==1){
											OBJ->DeleteLastOrder();
											if(OBJ->LocalOrder)OBJ->LocalOrder->DoLink(OBJ);
											return;
										};
									};
									return;
								};
							};
							OBJ->DeleteLastOrder();
							if(OBJ->LocalOrder)OBJ->LocalOrder->DoLink(OBJ);
							return;
						};
					};
				}else{
					if(COMPSEARCH){
						int x=xx+NMN->ShotPtX[i];
						int y=NMN->ShotPtY[i];
						int z=NMN->ShotPtZ[i];
						int dz=NMN->ShotPtYG[i]-y-z;
						y=yy+((y+z)<<1);
						z+=OBJ->RZ;
						char dir=NMN->ShotDir[i];
						byte diff=NMN->ShotDiff[i];
						int minr=NMN->ShotMinR[i];
						int maxr=NMN->ShotMaxR[i];

						int rx=OB->RealX>>4;
						int ry=OB->RealY>>4;
							
						int dst=Norma(x-rx,y-ry);
						char rdir=char(GetDir(rx-x,ry-y));
						int ddir=abs(char(rdir-dir));
						if(dst>=minr&&dst<=maxr&&ddir<diff){
							DELAYPROBLEM=1;
						};
					};
				};
			};
			if(!DELAYPROBLEM){
				OBJ->DeleteLastOrder();
				//if(OBJ->LocalOrder)OBJ->LocalOrder->DoLink(OBJ);
			};
			return;
		};
        
        if(!NMN->NShotPt){
            OBJ->DeleteLastOrder();
            return;
        };
		//Unit's attack
        int dst=(Norma(OBJ->RealX-OB->RealX,OBJ->RealY-OB->RealY)>>4);
#ifdef COSSACKS2
		int D=OBJ->RZ-OB->RZ+int(OBJ->newMons->SrcZPoint)-int(OB->newMons->SrcZPoint);
		if(D>0)D=0;
		int dst1=dst-(D<<2);
#else //COSSACKS2
		int dst1=dst-(int(OBJ->RZ+OBJ->newMons->SrcZPoint-(OB->RZ+OB->newMons->SrcZPoint))<<1);
#endif //COSSACKS2
        if(dst<ADC->AttackRadius1[0]||dst1>ADC->AttackRadius2[0]){
            OBJ->DeleteLastOrder();
            return;
        };
        int xx,yy,x0,y0;
        OBJ->GetPreciseCornerXY(&xx,&yy);
        xx<<=4;
        yy<<=4;
        int mindis=1000000;
        int bsp=0;
        int np=NMN->NShotPt;
        int xx1=OB->RealX;
        int yy1=OB->RealY;
		int zzz=NMN->SrcZPoint;
		int DZZ=OBJ->RZ;
        for(int i=0;i<np;i++){
            x0=xx+(NMN->ShotPtX[i]<<4);
            y0=yy+(NMN->ShotPtY[i]<<4);
            dst=Norma(x0-xx1,y0-yy1);
            if(dst<mindis){
                mindis=dst;
                bsp=i;
            };
        };
        x0=xx+(NMN->ShotPtX[bsp]<<4);
        y0=yy+(NMN->ShotPtY[bsp]<<4);
		Weapon* Weap=NMN->DamWeap[0];
        if(Weap){
			Coor3D C3D;
			word DestObj=0xFFFF;
			if(OB->GetDamagePoint(OBJ->x,OBJ->y,&C3D,0))DestObj=OID;
			int wx0=x0>>4;
			int wy0=y0>>4;
			int wz0=( NMN->ShotPtZ ? NMN->ShotPtZ[bsp]+DZZ : 50+DZZ );
			//int yc=OBJ->RealY;
			//wz0-=(y0-yc)>>5;
			//wy0=yc>>4;
			int wx1=C3D.x;
			int wy1=C3D.y;
			int wz1=C3D.z;
            /*
			int WRes=PredictShot(Weap,wx0,wy0,wz0,wx1,wy1,wz1,OBJ->Index);
			if(WRes!=-1){
				char atts[128];
				//something on the way...
				if(WRes>8192){
					//ground
					if(OBJ->NNUM==MyNation){
						sprintf(atts,MESHPOV,OBJ->newMons->Message);//"%s : � �� ����. �������� ���"
						AssignHint1(atts,48);
					};
				}else{
					OneObject* OBX=Group[WRes];
					if(OBX){
						if(ShotRecommended&&!(OBX->NMask&OBJ->NMask))goto DoShot0;
						if(OBJ->NNUM==MyNation){
							sprintf(atts,MESHOBJ,OBJ->newMons->Message,OBX->newMons->Message);//"%s : � �� ����. ���%s"
							AssignHint1(atts,48);
						};
					};
				};
				OBJ->delay=100;
				addname(OBJ->Index);
				if(OBJ->delay>OBJ->MaxDelay)OBJ->delay=OBJ->MaxDelay;
				addrand(OBJ->delay);
				OBJ->DeleteLastOrder();
				return;
			};
            */
DoShot0:;
			bool AllowShot=true;
			if(NMN->NShotRes){
				for(int k=0;k<NMN->NShotRes;k++){
					if(XRESRC(OBJ->NNUM,NMN->ShotRes[k+k])<NMN->ShotRes[k+k+1])AllowShot=false;
				};
				if(AllowShot){
					for(int k=0;k<NMN->NShotRes;k++){
						AddXRESRC(OBJ->NNUM,NMN->ShotRes[k+k],-NMN->ShotRes[k+k+1]);
						OBJ->Nat->AddResource(byte(NMN->ShotRes[k+k]),-NMN->ShotRes[k+k+1]);
						OBJ->Nat->ResOnLife[NMN->ShotRes[k+k]]+=NMN->ShotRes[k+k+1];
					};
				}else{
					if(OBJ->NNUM==MyNation&&OBJ->PrioryLevel>=16){
						char ccc[150];
						sprintf(ccc,GetTextByID("CSNORS"),OBJ->Ref.General->Message);
						AssignHint1(ccc,64);
					};
					OBJ->DeleteLastOrder();
					return;
				};
			};

			if(AllowShot){
				CreateRazbros(OBJ,&C3D);
				Create3DAnmObject(Weap,wx0,wy0,wz0,wx1,wy1,wz1,OBJ,0,DestObj);
				OBJ->delay=ADC->AttackPause[0];
				addrand(OBJ->delay);
				OBJ->MaxDelay=div(int(OBJ->delay)*OBJ->PersonalDelay,100).quot;
				OBJ->MaxDelay=int(OBJ->MaxDelay)*100/OBJ->GetAttackSpeedBonus();
				OBJ->delay=OBJ->MaxDelay;
				addrand(OBJ->delay);
				if(NTIMES&&OBJ->LocalOrder){
					OBJ->LocalOrder->info.BuildObj.SN--;
					if(NTIMES==1){
						OBJ->DeleteLastOrder();
						return;
					};
				};
			};
		};
        return;
    };
}
//
extern bool fes_bAmunition[8192];
void fes_neAmmunition(word NIndex, char* Message=NULL);
//
void ActionAfterAttackFinished(OneObject* OBJ,OneObject* EOB,int prio){
	OBJ->SearchVictim();
	if(EOB){		
		int R=Norma(EOB->RealX-OBJ->RealX,EOB->RealY-OBJ->RealY)>>4;
		if(prio>10 && R>1000 && !OBJ->Attack)OBJ->NewMonsterSmartSendTo(EOB->RealX>>4,EOB->RealY>>4,(rando()&63)-32,(rando()&63)-32,128+1,1);  
	}
	//if(OBJ->CObjIndex==0xFFFF && !OBJ->Attack){
	//	OBJ->NewState=0;
	//	TryToStand(OBJ,true);
	//}
}
void AttackObjLink(OneObject* OBJ){
	if(OBJ->BrigadeID!=0xFFFF&&OBJ->RifleAttack&&OBJ->delay==0){
        Brigade* BR=CITY[OBJ->NNUM].Brigs+OBJ->BrigadeID;
		if(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()!=BRIGADEORDER_RIFLEATTACK){
			void BrigadeRifleAttack(OneObject *OB);
			BrigadeRifleAttack(OBJ);
			return;
		}
	}
	addrand(OBJ->Index);

	word OID=OBJ->EnemyID;
	word OSN=OBJ->EnemySN;

	static OneObject TMPOBJ;
	OneObject* OB; // target
#ifdef SIMPLEMANAGE
	bool ARMONLY=OBJ->ArmAttack&&!(OBJ->RifleAttack||AttGroundMod);
	bool SHOTONLY=OBJ->RifleAttack&&!(OBJ->ArmAttack||AttGroundMod);
#else //SIMPLEMANAGE
	bool ARMONLY=OBJ->ArmAttack;
#endif //SIMPLEMANAGE
	OBJ->UnitSpeed=64;
	OBJ->GroupSpeed=OBJ->newMons->MotionDist;
	if(AttGroundMod){
		memset(&TMPOBJ,0,sizeof OneObject);
		OB=&TMPOBJ;
		OneObject* GetBldInPoint(int x,int y);
		OneObject* EOB=GetBldInPoint(OBJ->DstX>>4,OBJ->DstY>>4);
		TMPOBJ.RealX=OBJ->DstX;
		TMPOBJ.RealY=OBJ->DstY;
		TMPOBJ.CObjIndex=0xFFFF;
		TMPOBJ.x=OBJ->DstX>>8;
		TMPOBJ.y=OBJ->DstY>>8;
		TMPOBJ.RZ=GetHeight(OBJ->RealX>>4,OBJ->RealY>>4);
		addrand(TMPOBJ.RZ);
		TMPOBJ.Index=0xFFFF;
		TMPOBJ.Serial=0xFFFF;
		TMPOBJ.Ref.General=EOB?EOB->Ref.General:NATIONS[7].Mon[0];
		TMPOBJ.newMons=EOB?EOB->newMons:NATIONS[7].Mon[0]->newMons;
		TMPOBJ.NIndex=7;
		TMPOBJ.LockType=OBJ->LockType;
		TMPOBJ.MathMask=0xFF;
		TMPOBJ.Sdoxlo=0;
		TMPOBJ.Hidden=0;
		OID=0xFFFF;
		OSN=0xFFFF;
	}else OB=Group[OID];

	if(OB&&OB->ActiveAbility){
		bool invul=false;
		OB->ActiveAbility->ActiveAbilities.modifyInvulnerability(invul,invul);
		addrand(invul);
		byte mask=0;
		bool inv=false;
		bool EnableSearchVictim;
		addrand(72);
		OB->ActiveAbility->ActiveAbilities.modifyInvisibility(false,inv,mask,EnableSearchVictim); // перестать атаковать невидимку
		addrand(inv);
		addrand(mask);
		if( inv && !(mask&OBJ->NMask) || invul ) {
			addrand(OB->Index);
			int pl=OBJ->PrioryLevel;
			OBJ->DeleteLastOrder();
			addrand(56);
			ActionAfterAttackFinished(OBJ,OB,pl);
			return;
		}
	}

	bool NoMove=false;
	if(OBJ->ActiveAbility){
		int s1=100;
		OBJ->ActiveAbility->ActiveAbilities.modifyMotionSpeed(s1,s1,NoMove);
		if(NoMove)return;
		if(s1==0)NoMove=true;
		if(OBJ->LocalOrder->PrioryLevel<16){		
			byte mask=0;
			bool inv=false;
			bool EnableSearchVictim=false;
			addrand(91);
			OBJ->ActiveAbility->ActiveAbilities.modifyInvisibility(false,inv,mask,EnableSearchVictim); // сбросить приказ атаки если атакующий невидим
			addrand(inv);
			addrand(EnableSearchVictim);
			bool comp=false;
			OBJ->ActiveAbility->ActiveAbilities.modifyCompassion(comp,comp); // отменять приказ атаки для жалостливых
			addrand(comp);
			if( inv&&!((mask&OBJ->NMask)||EnableSearchVictim) || comp ){
				addrand(OB->Index);
				int pl=OBJ->PrioryLevel;
				OBJ->DeleteLastOrder();
				addrand(61);
				ActionAfterAttackFinished(OBJ,OB,pl);
				return;
			}
		}
	}

	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;
	byte prl=OBJ->PrioryLevel;
	if(OBJ->NewBuilding){
		AttackByBuildingLink(OBJ);
		addrand(11);
		return;
	}
	NewMonster* NMN=OBJ->newMons;
	int RMAX=100000000;
	//if(OBJ->LockType==1&&OBJ->Nat->AI_Enabled&&NMN->AI_PreferredAttR_Max){
	//	RMAX=NMN->AI_PreferredAttR_Max;
	//};
	AdvCharacter* ADC=OBJ->MoreCharacter;
#ifndef COSSACKS2
	if(OBJ->LockType!=1)OBJ->DestX=-1;
#endif
	//OBJ->UnBlockUnit();
	if(OID>=ULIMIT&&!AttGroundMod){
		addrand(OBJ->Index);
		OBJ->DeleteLastOrder();
		addrand(33);
		return;
	};

	if(OBJ->CObjIndex!=0xFFFF){
		void AttackPointByComplexObjectLink(OneObject* OBJ,int x,int y);
		if(OB&&!OB->Sdoxlo&&OB->Serial==OSN){
			AttackPointByComplexObjectLink(OBJ,OB->GetAttX(),OB->GetAttY());
		}else{
			OBJ->DeleteLastOrder();
		}
		addrand(123);
		return;
	}
	if(NMN->CompxUnit&&NMN->CompxUnit->CanTakeExRes){
		if(OB&&OB->Sdoxlo&&OB->Serial==OSN&&OB->newMons->MeatTransformIndex!=0xFF){
			ObjCharacter* OCR=SPECIAL.Objects[OB->newMons->MeatTransformIndex];
			int rt=OCR->ResType;
			if(rt>=0&&rt<16){
				if(NMN->CompxUnit->TakeResStage[rt]){
					addrand(OBJ->Index);
					OBJ->DeleteLastOrder();
					for(int i=0;i<60;i++)OBJ->TakeResource(OB->GetAttX()>>4,OB->GetAttY()>>4,rt,16+128,1);
					OBJ->NewMonsterSendTo(OB->GetAttX(),OB->GetAttY(),16+128,1);
					addrand(256);
					return;
				};
			};
		};
	};
	if(((!OB)||(OB->Hidden)||(OB->Sdoxlo>8)||OSN!=OB->Serial)||OBJ->TurnOff){
		/*if(OBJ->PrioryLevel>=16&&OBJ->NNUM==MyNation&&OBJ->TurnOff){
			char ccc[128];
			sprintf(ccc,GetTextByID("CSNOGL"),OBJ->Ref.General->Message);
			AssignHint1(ccc,50);
		};*/
		addrand(OBJ->Index);
		int pl=OBJ->PrioryLevel;
		OBJ->DeleteLastOrder();
		ActionAfterAttackFinished(OBJ,OB,pl);
		return;
	};
	if(ARMONLY&&OB->NewBuilding)ARMONLY=0;
#ifdef SIMPLEMANAGE
	if(SHOTONLY&&OB->NewBuilding)SHOTONLY=0;
	addrand(ARMONLY);
	addrand(OB->UnlimitedMotion);
	if(ARMONLY){
		//for gecalka
		if(OB->UnlimitedMotion&&rando()<512){
			addrand(OBJ->Index);
			OBJ->DeleteLastOrder();
			addrand(561);
			return;
		}
		addrand(65);
		addrand(OB->LockType);
		addrand(OBJ->LockType);
		if(OBJ->LockType!=3&&OBJ->LockType!=OB->LockType&&MFIELDS[OB->LockType].CheckBar(OB->x,OB->y,1,1)){
			addrand(OBJ->Index);
			OBJ->DeleteLastOrder();
			addrand(562);
			return;
		}
	}
#endif //SIMPLEMANAGE
	if(OBJ->newMons->Shaman&&!(OBJ->NMask&OB->NMask))ARMONLY=1;
	if(NMN->Priest){
		if(!OB->NMask&OBJ->NMask){
			addrand(OBJ->Index);
			OBJ->DeleteLastOrder();
			addrand(563);
			return;
		};
	}else{
		if(!NMN->Shaman){
			byte AMASK=OBJ->newMons->KillMask|OBJ->newMons->AttackMask[0]|OBJ->newMons->AttackMask[1]|OBJ->newMons->AttackMask[2];
			if(OB->NMask&OBJ->NMask||!(OB->MathMask&AMASK)){
				addrand(OBJ->Index);
				OBJ->DeleteLastOrder();
				addrand(564);
				return;
			};
		};
	};
	
	
	byte maska=OB->MathMask;
	if(AttGroundMod)maska=0xFF;
	bool killins=0;
	if(OB->newMons->CanBeKilledInside){
		if(NMN->CanFire){
			if(OB->InFire&&OB->NInside)killins=1;
		}else if(OB->NInside)killins=1;
	};
	short OLDX=OBJ->LocalOrder->info.BuildObj.ObjX;
	short OLDY=OBJ->LocalOrder->info.BuildObj.ObjY;
	word NTIMES=OBJ->LocalOrder->info.BuildObj.SN;
	short NEWX=OB->GetAttX()>>8;
	short NEWY=OB->GetAttY()>>8;

	addname(OBJ->Index);
	addrand(OBJ->NewState);
	addrand(OBJ->delay);
	addrand(OBJ->NewCurSprite);

	int nst=OBJ->NewState;
	int Lnst=OBJ->LocalNewState;
	bool DELAY;
	bool CANSETDELAY=1;
	bool NEEDSLOWRECHARGE=0;
#ifdef CONQUEST
	DELAY=OBJ->delay!=0;
	if(DELAY){
		if(NMN->NoWaitMask&(1<<(Lnst-1)))DELAY=0;
		CANSETDELAY=0;
	};
#else
	DELAY=OBJ->delay;
#endif
	if(DELAY&&NMN->SlowRecharge){
		NEEDSLOWRECHARGE=1;
	};
	bool Attack5=NMN->SitInFormations&&OBJ->NewState==5;

	int x0=OB->GetAttX();
	int y0=OB->GetAttY();
	int dst=Norma(x0-OBJ->RealX,y0-OBJ->RealY)>>4;
	addrand(dst); 
    bool can=false;
    for(int i=0;i<NAttTypes;i++){
        if(ADC->MaxDamage[i]){
            if(dst>=ADC->DetRadius1[i]){
                can=true;
                break;
            }
        }else break;
    }
    if(!can){
        OBJ->DeleteLastOrder();
        return;
    }
    
	int MyZ=OBJ->RZ+NMN->SrcZPoint;
	int EnZ=OB->RZ+OB->newMons->DstZPoint;
	int dst1=dst;
	int D=MyZ-EnZ;
	if(D>0)D=0;
	int dstx=dst-((OBJ->RZ-OB->RZ)<<1);
	addrand(OBJ->RZ);
	addrand(OB->RZ);
	if(OB->MoreCharacter && OB->MoreCharacter->MaxR_Attack<180)dstx=dst;
	if(dstx>dst)dstx=dst;
	if(dst-dstx>300)dstx=dst-300;		
	if(dstx<120&&dst>120)dstx=120;
	int exR=OB->newMons->AddShotRadius;
	if(!nst){//for catapult
		int cod=OBJ->NewAnm->Code;
		if(cod>=anm_Attack&&cod<anm_Attack+100){
			nst=cod-anm_Attack+1;
			OBJ->NewState=nst;
		}
	}
	if(nst&&!DELAY){
		//attack animation to shoot
		NewAnimation* Anm=NMN->GetAnimation(anm_Attack+OBJ->NewState-1);
		addrand(Anm->Enabled);
		addrand(dstx);
		addrand(OBJ->GetMaxAttackRadius(OBJ->NewState-1));
		addrand(OBJ->NewState);
		addrand(ADC->MinR_Attack);
		if(Anm->Enabled&&dstx<OBJ->GetMaxAttackRadius(OBJ->NewState-1)+150+exR&&dst>ADC->MinR_Attack-80){
			addname(OBJ->Index);
			int af=OBJ->NewAnm->ActiveFrame;
			if(!af)af=OBJ->NewAnm->NFrames/2;
			if(af<4)af=4;

			int cf0=(OBJ->CurrentFrameLong)>>8;
			int cf1=(OBJ->CurrentFrameLong+GameSpeed)>>8;
            
			if((OBJ->NewAnm->Code/100)==(Anm->Code/100)&&OBJ->NewCurSprite>OBJ->NewCurSpritePrev&&af>=OBJ->NewCurSpritePrev&&af<OBJ->NewCurSprite){
				//time to shot immediately
				if(NMN->Usage==PushkaID&&OBJ->NewState==2&&prl<10){
					if(!TestUnitsInSectorForGrenade(OBJ)){
						addrand(OBJ->Index);
						OBJ->DeleteLastOrder();
						return;
					};
				};
				int DPAUS=100;
				if(OBJ->NewState&&AttGroundMod){
					DPAUS=ADDPAUSE;
				};
				//Weapon* WP=NMN->DamWeap[OBJ->NewState-1];

				Weapon* WP=NULL;

				if(OBJ->LockType==1)WP=NMN->DamWeap[OBJ->NewState-1];
				else WP=NMN->DamWeap[OBJ->LocalNewState-1];

				if(CANSETDELAY){
					int ST=0;

					if(OBJ->LockType==1)ST=OBJ->NewState-1;
					else ST=OBJ->LocalNewState-1;
					if(ST<0)ST=0;

					if(NMN->MaxZalp&&WP){
						if(OBJ->NZalp>0)OBJ->NZalp--;
						else{
							OBJ->delay=ADC->AttackPause[OBJ->NewState-1];
							OBJ->NZalp=NMN->MaxZalp;
						};
					}else OBJ->delay=ADC->AttackPause[ST];
					addrand(OBJ->delay);
					addrand(OBJ->NewState);
					if(OBJ->delay){
						OBJ->MaxDelay=div(int(OBJ->delay)*OBJ->PersonalDelay,100).quot;
						OBJ->MaxDelay=int(OBJ->MaxDelay)*100/OBJ->GetAttackSpeedBonus();
						OBJ->MaxDelay=(int(OBJ->MaxDelay)*DPAUS)/100;
						OBJ->delay=OBJ->MaxDelay;
						addrand(OBJ->delay);
					};
				};

				//pubuxxxxxxxxx...
				if(OBJ->ActiveAbility){
					
					// Invisible
					OBJ->ActiveAbility->ActiveAbilities.InvisibleEnableTime=TrueTime+1000;

					OBJ->ActiveAbility->ActiveAbilities.modifyWeapon(WP,&WP,true);
				}
				if(WP){
					//----------
					NewAnimation* UNAM=OBJ->NewAnm;
					if(UNAM->AnimationType==1 && (UNAM->ModelID>0 || UNAM->AnimSet3D.GetAmount()) ){ 
						Coor3D C3D;
						word DestObj=0xFFFF;
						int x0,y0,z0,x1,y1,z1;
						//NewAnimation* NAM=WP->NewAnm;
						if(OB->GetDamagePoint(OBJ->x,OBJ->y,&C3D,0))DestObj=OID;
						DestObj=0xFFFF;//temp
						if(OB->Wall)DestObj=OID;
						z0=OBJ->RZ+NMN->SrcZPoint;
						x1=C3D.x;
						y1=C3D.y;
						z1=C3D.z;
						CreateRazbros(OBJ,&C3D);

						int SC=int(UNAM->Scale*100);
						if(UNAM->ModelID==-1){
							AnimFrame3D* AF3D=UNAM->AnimSet3D[0];
							if(AF3D){
								SC=int(AF3D->Scale*100);
							}
						}

						x0=(OBJ->RealX>>4)+((SC*UNAM->HotRadius)/100*TCos[OBJ->RealDir]>>8);
						y0=(OBJ->RealY>>4)+((SC*UNAM->HotRadius)/100*TSin[OBJ->RealDir]>>8);
						z0=OBJ->RZ+OBJ->OverEarth+UNAM->HotHeight*SC/100;
						Create3DAnmObject(WP,x0,y0,z0,x1,y1,NMN->DirectFight&(1<<(OBJ->NewState-1))?z0:z1,OBJ,OBJ->LocalNewState>0?OBJ->LocalNewState-1:OBJ->NewState-1,DestObj);
					}else if(UNAM->Frames.GetAmount()){
						int NR=UNAM->Rotations;
						int NWP=1;
						int SWS=-NR;
						if(UNAM->DoubleShot){
							NWP=1+UNAM->DoubleShot;
						};
						for(int q=0;q<NWP;q++){
							SWS+=NR;
							Coor3D C3D;
							word DestObj=0xFFFF;
							int x0,y0,z0,x1,y1,z1,uocts,uoc2;
							byte udir;	
							z0=OBJ->RZ+OBJ->OverEarth;
							//Searching for the active point coordinates
							if(NR&1||!NR){
								int csp=OBJ->NewCurSprite;
								uocts=(UNAM->Rotations-1)*2;
								uoc2=UNAM->Rotations-1;
								if(!uocts)uocts=1;
								int usesize=div(255,uocts<<1).quot;
								udir=(((OBJ->RealDir+64+usesize)&255)*uocts)>>8;
								NewFrame* NF=UNAM->Frames[csp];
								if(udir<uoc2){
									int udir1=uoc2-udir;
									x0=(OBJ->RealX>>4)-(UNAM->ActivePt[udir1+SWS]->x+NF->dx);
									y0=(OBJ->RealY>>4);
									z0-=UNAM->ActivePt[udir1+SWS]->y+NF->dy;
								}else{
									int udir1=uoc2-uocts+udir;
									x0=(OBJ->RealX>>4)+(UNAM->ActivePt[udir1+SWS]->x+NF->dx);
									y0=(OBJ->RealY>>4);
									z0-=UNAM->ActivePt[udir1+SWS]->y+NF->dy;
								};
							}else{
								int csp=OBJ->NewCurSprite;
								uocts=NR;
								uoc2=NR;
								if(!uocts)uocts=1;
								int usesize=128/uocts;
								udir=((OBJ->RealDir-64+usesize)&255)/NR;
								NewFrame* NF=UNAM->Frames[csp];
								x0=(OBJ->RealX>>4)+(UNAM->ActivePt[udir+SWS]->x+NF->dx);
								y0=(OBJ->RealY>>4);//+NMN->SrcZPoint;
								z0-=UNAM->ActivePt[udir+SWS]->y+NF->dy;
							};
							//----------
							NewAnimation* NAM=OBJ->NewAnm;//WP->NewAnm
							int octs=(NAM->Rotations-1)*2;
							int oc2=NAM->Rotations-1;
							if(!octs)octs=1;
							int sesize=div(255,octs<<1).quot;
							byte dir=(((OBJ->RealDir+64+sesize)&255)*octs)>>8;
							if(OB->GetDamagePoint(OBJ->x,OBJ->y,&C3D,0))DestObj=OID;
							DestObj=0xFFFF;//temp
							if(OB->Wall)DestObj=OID;
							
							
							x1=C3D.x;
							y1=C3D.y;
							z1=C3D.z;
							
	//DoShot:;					
							bool AllowShot=true;
							if(!AllowShot)return;
							CreateRazbros(OBJ,&C3D);
							//shot occurs there
							{
								if(udir<uoc2){
									udir=uoc2-udir;
									x1=C3D.x;
									y1=C3D.y;
									z1=C3D.z;//+NMN->DstZPoint;
									Create3DAnmObject(WP,x0,y0,z0,x1,y1,NMN->DirectFight&(1<<(OBJ->NewState-1))?z0:z1,OBJ,OBJ->LocalNewState>0?OBJ->LocalNewState-1:OBJ->NewState-1,DestObj);
									if(NMN->WSECT&&OBJ->NewState-1==NMN->WSECT->AttIndex){
										DamageUnitsInAngle(OBJ->RealX>>4,OBJ->RealY>>4,NMN->WSECT,OBJ->RealDir,OBJ,OBJ->NewState-1);
									};
									//CreateExObjD(OBJ->Weap,(OBJ->RealX>>2)-((UNAM->ActivePtX[udir]+NF->dx)<<2),(OBJ->RealY>>2)+((UNAM->ActivePtY[udir]+NF->dy)<<2),OBJ->RealDir,0,0,NULL);
								}else{
									udir=uoc2-uocts+udir;
									x1=C3D.x;
									y1=C3D.y;
									z1=C3D.z;//+NMN->DstZPoint;
									addrand(78);
									Create3DAnmObject(WP,x0,y0,z0,x1,y1,NMN->DirectFight&(1<<(OBJ->NewState-1))?z0:z1,OBJ,OBJ->LocalNewState>0?OBJ->LocalNewState-1:OBJ->NewState-1,DestObj);
									if(NMN->WSECT&&OBJ->NewState-1==NMN->WSECT->AttIndex){
										DamageUnitsInAngle(OBJ->RealX>>4,OBJ->RealY>>4,NMN->WSECT,OBJ->RealDir,OBJ,OBJ->NewState-1);
									};
									//CreateExObjD(OBJ->Weap,(OBJ->RealX>>2)+((UNAM->ActivePtX[udir]+NF->dx)<<2),(OBJ->RealY>>2)+((UNAM->ActivePtY[udir]+NF->dy)<<2),OBJ->RealDir,0,0,NULL);
								};
							}
							if(NMN->SingleShot&(1<<(OBJ->NewState-1)))NTIMES=1;							
						}
					}
					if(NTIMES&&OBJ->LocalOrder){
						OBJ->LocalOrder->info.BuildObj.SN--;
						if(NTIMES==1){
							addrand(OBJ->Index);
							OBJ->DeleteLastOrder();
							return;
						}
					}
                    if(NMN->Priest||(NMN->Shaman&&OB->NMask&OBJ->NMask))goto ph;
				}else{
					//melee attack
					if(NMN->Priest||(NMN->Shaman&&OB->NMask&OBJ->NMask)){
ph:;
						bool SomethingDone=0;						
						if(OB->BrigadeID!=0xFFFF&&EngSettings.MoraleType==0){
							Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
							if(BR->Morale<BR->MaxMorale){
								extern int Morale_DamageDec;
								int FTP=OB->newMons->FearType[0];
								if(FTP!=0xFF){
									BR->Morale+=(&Morale_DamageDec)[FTP];
									SomethingDone=1;
								}
							}
						}
						if(OB->BrigadeID!=0xFFFF){
                            Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
							int MAXDL=0;
							OneObject* HOB=NULL;
							for(int i=0;i<BR->NMemb;i++){
                                word MID=BR->Memb[i];
								if(MID!=0xFFFF){
                                    OneObject* OB=Group[MID];
									if(OB&&OB->Serial==BR->MembSN[i]){
										int DL=OB->MaxLife-OB->Life;
										if(DL>MAXDL){
											MAXDL=DL;
                                            HOB=OB;
										}
									}
								}
							}
							if(HOB){
								if(HOB->Life<HOB->MaxLife){
									HOB->Life+=OBJ->MoreCharacter->MaxDamage[0];
									if(HOB->Life>=HOB->MaxLife){
										HOB->Life=HOB->MaxLife;
										HOB->InFire=0;
										HOB->FireOwner=0xFF;
										HOB->FiringStage=0;										
									}
									SomethingDone=1;
								}
							}
						}else{
							if(OB->Life<OB->MaxLife){
								OB->Life+=OBJ->MoreCharacter->MaxDamage[0];
								if(OB->Life>=OB->MaxLife){
									OB->Life=OB->MaxLife;
									OB->InFire=0;
									OB->FireOwner=0xFF;
									OB->FiringStage=0;
									SomethingDone=0;
								};
							}else SomethingDone=0;
						}
						if(!SomethingDone){
							addrand(OBJ->Index);
							OBJ->DeleteLastOrder();
							return;
						};
					}else{
						if(OB->Nat){
							if(OBJ->NMisses)OBJ->NMisses--;
							else{
								if(ADC->MaxDamage[OBJ->NewState-1]){
									OB->MakeDamage(0,ADC->MaxDamage[OBJ->NewState-1],OBJ,OBJ->NewState-1);
									//AddEffect(OBJ->RealX>>4,(OBJ->RealY>>5)-OBJ->RZ,NMN->StrikeSoundID);
									AddEffect(OBJ->RealX/16,OBJ->RealY/16,NMN->StrikeSoundID);
								}
							}
							//if(OB->newMons->Usage!=PushkaID&&OB->newMons->Priest==false&&OB->newMons->DontAnswerOnAttack==false)OB->AttackObj(OBJ->Index,2);
						};
						//OBJ->BlockUnit();
					};
					if(NMN->SingleShot&(1<<(OBJ->NewState-1)))NTIMES=1;
					if(NTIMES&&OBJ->LocalOrder){
						OBJ->LocalOrder->info.BuildObj.SN--;
						if(NTIMES==1){
							addrand(OBJ->Index);
							OBJ->DeleteLastOrder();
							return;
						};
					};
                };
				return;
			};
		};
	};
	addrand(OBJ->FrameFinished);
	addrand(OBJ->NewCurSprite);
	addrand(OBJ->NewState);
	addrand(OBJ->NewAnm->Code);
	
	if(OBJ->FrameFinished||(OBJ->NewAnm->CanBeBrokenAfterFrame<=OBJ->NewCurSprite&&OBJ->NewAnm->Code!=anm_Attack+std::max(int(OBJ->NewState)-1,0))){//to optimise
		addrand(3);
		//frame is finished, but it is not attack frame
		//checking if thore is enough resources to shot
		if(!NMN->SlowRecharge){
			//if SlowRecharge, then resources will be substracted in other place
			if(NMN->ArmAttack&&NMN->NShotRes){
				int n=NMN->NShotRes;
				for(int i=0;i<n;i++){
					int rid=NMN->ShotRes[i+i];
					int ram=NMN->ShotRes[i+i+1];
					if(XRESRC(OBJ->NNUM,rid)<ram)ARMONLY=1;
				}
			}
		}	
		addname(OBJ->Index);
		if(DELAY){
			//NewAnimation* Anm=&NMN->Attack[OBJ->NewState-1];
			//if(Anm->Enabled){
				addname(OBJ->Index);
				//if(OBJ->NewAnm==Anm){
					if(NMN->SlowRecharge){
						if(OBJ->delay==OBJ->MaxDelay){
							if(NMN->ArmAttack&&NMN->NShotRes){
								int n=NMN->NShotRes;
								for(int i=0;i<n;i++){
									int rid=NMN->ShotRes[i+i];
									int ram=NMN->ShotRes[i+i+1];
									if(XRESRC(OBJ->NNUM,rid)<ram)ARMONLY=1;
								}
								//if(!ARMONLY){
								//	for(int i=0;i<n;i++){
								//		int rid=NMN->ShotRes[i+i];
								//		int ram=NMN->ShotRes[i+i+1];
								//		AddXRESRC(OBJ->NNUM,rid,-ram);
								//	}
								//}
							}
						}
						int ns=OBJ->NewState;
						if(ns){
							if(ns==4||WeaponFlags[NMN->WeaponKind[ns-1]]&1){
								bool CheckShooterAbilityToRecharge(OneObject* OBJ);
								if(CheckShooterAbilityToRecharge(OBJ)){
									OBJ->NewState=4;
									if(!OBJ->newMons->DontTransformToChargeState){										
										OBJ->LocalNewState=4;
									}else{
										TryToStand(OBJ,0);
										OBJ->StandTime=20;
									}
								}
							}else TryToStand(OBJ,false);
						}else TryToStand(OBJ,false);
					}else TryToStand(OBJ,false);
				//};
			//};
		}else{
			//need to check if this code is need(???)
			if(NMN->SlowRecharge){
				NewAnimation* Anm=NMN->GetAnimation(anm_Attack+OBJ->NewState-1);
				if(Anm->Enabled&&OBJ->NewAnm->Code==Anm->Code){
					int ns=OBJ->NewState;
					if(ns){
						if(ns==4||WeaponFlags[NMN->WeaponKind[ns-1]]&1){
							if(CheckShooterAbilityToRecharge(OBJ)){
								OBJ->NewState=4;
								OBJ->LocalNewState=4;
							}
						}else{
							if(OBJ->DestX<0)TryToStand(OBJ,false);
						}
					}else{
						if(OBJ->DestX<0)TryToStand(OBJ,false);
					}
				}else{
					if(OBJ->DestX<0)TryToStand(OBJ,false);
				}
			};
		};
		//this inserted to avoid animation breaking 
		if(!(OBJ->FrameFinished||(OBJ->NewAnm->CanBeBrokenAfterFrame<=OBJ->NewCurSprite&&OBJ->NewAnm->Code!=anm_Attack+OBJ->NewState-1)))return;//to optimise
		
		//animation has finished and now we can analyse all
		//determining the need state		

		//???
		//dst=dstx;
		//???
		int NeedState=-1;
		addrand(1);
#ifdef SIMPLEMANAGE
		if(SHOTONLY&&!killins){
			if(Attack5)NeedState=4;
			else NeedState=1;
		}else
#endif //SIMPLEMANAGE
		if(ARMONLY&&!killins){
			NeedState=0;
			if(!NMN->AttackMask[0]&maska){
				addrand(OBJ->Index);
				OBJ->DeleteLastOrder();
				return;
			};
		}else{
			//determining required attack state
			for(int i=0;i<NAttTypes;i++){
				int wpk=NMN->WeaponKind[i];
				//if(WeaponFlags[wpk]&1){
					if(dst1>=ADC->DetRadius1[i]&&dstx<=ADC->DetRadius2[i]&&((NMN->AttackMask[i]&maska)||(killins&&NMN->DamWeap[i]))){
						NeedState=i;
					};
				//}else{
				//	if(dst>=ADC->DetRadius1[i]&&dst<=ADC->DetRadius2[i]&&((NMN->AttackMask[i]&maska)||(killins&&NMN->DamWeap[i]))){
				//		NeedState=i;
				//	};
				//};
			};
			if(NeedState==1&&Attack5)NeedState=4;
			if(OBJ->delay&&NMN->NoWaitMask){
				//sometimes delay is not required
				byte wpmask=NMN->NoWaitMask;
				for(int i=0;i<NAttTypes;i++)if(wpmask&(1<<i)){
					int wpk=NMN->WeaponKind[i];
					//if(WeaponFlags[wpk]&1){
						if(dst1>=ADC->DetRadius1[i]&&dstx<=ADC->DetRadius2[i]&&((NMN->AttackMask[i]&maska)||(killins&&NMN->DamWeap[i]))){
							NeedState=i;
						};
					//}else{
					//	if(dst>=ADC->DetRadius1[i]&&dst<=ADC->DetRadius2[i]&&((NMN->AttackMask[i]&maska)||(killins&&NMN->DamWeap[i]))){
					//		NeedState=i;
					//	};
					//};
				};
				if(NeedState==1&&Attack5)NeedState=4;
			};
		};
		if(NeedState==-1){
			//we cant attack because we don't know how (state not determined)
			addrand(1);
			OBJ->DeleteLastOrder();
			if(!TestCapture(OB))OBJ->NewMonsterSmartSendTo(OB->GetAttX()>>4,OB->GetAttY()>>4,0,0,OBJ->PrioryLevel,1);
			return;
		};
		int wpk=NMN->WeaponKind[NeedState];
		if(WeaponFlags[wpk]&1||NeedState==4){//attack through rifle			
			dst=dstx;
			if(NEEDSLOWRECHARGE){				
				if(OBJ->LocalNewState!=4){
					if(CheckShooterAbilityToRecharge(OBJ)){
						if(OBJ->NewState==5){
							OBJ->NewState=0;
                            TryToStand(OBJ,0);
							return;
						}
						if(OBJ->LocalNewState==5)return;
						OBJ->NewState=4;
					}else{
						addrand(OBJ->Index);
                        OBJ->DeleteLastOrder();
					}
					return;
				}else{
					NewAnimation* NRE=NMN->GetAnimation(anm_Attack+3);
					if(OBJ->NewAnm!=NRE){
						if(OBJ->delay>NRE->NFrames)OBJ->delay-=NRE->NFrames;
						else OBJ->delay=0;
						OBJ->NewAnm=NMN->GetAnimation(anm_Attack+3);
						OBJ->SetZeroFrame();
						OBJ->InMotion=0;
						OBJ->BackMotion=0;
#ifdef COSSACKS2
						OBJ->DestX=-1;
#endif
						return;
					}else{
						if(OBJ->delay>NRE->NFrames){
							OBJ->delay-=NRE->NFrames;
							OBJ->NewAnm=NMN->GetAnimation(anm_Attack+3);
							OBJ->SetZeroFrame();
							OBJ->InMotion=0;
							OBJ->BackMotion=0;
#ifdef COSSACKS2
							OBJ->DestX=-1;
#endif

							//OB->delay-=NRE->NFrames;
							return;
						};
						OBJ->delay=0;
					};
				};
			};
		};
		//checking distance
		NewAnimation* NAMO=NMN->GetAnimation(anm_PMotionL+NeedState);
		bool AMove=NAMO->Enabled;
		int MRot=NMN->MinRotator;
		if(OBJ->/*NMN->*/MotionStyle==2){
			MRot=MRot*GameSpeed/256/8;
			if(MRot<1)MRot=1;
			if(MRot>8)MRot=8;
		}
		int MRot1=MRot+(MRot>>1);
		if(NMN->RotationAtPlaceSpeed){
			MRot=1;
			MRot1=1;
		}
		if(MRot1>16)MRot1=16;
		int DR2=0;
		if(/*NeedState==0&&*/AttGroundMod){
			DR2=((ADDRANGE-100)*OBJ->GetMaxAttackRadius(NeedState))/100;
		};
		DR2+=OB->newMons->AddShotRadius;
		int rm=OBJ->GetMaxAttackRadius(NeedState);
		addrand(rm);
		addrand(dst1);
		addrand(DR2);
		addrand(dst);
		addrand(RMAX);
		addrand(NeedState);
		addrand(ADC->AttackRadius1[NeedState]);
		if(dst1>=ADC->AttackRadius1[NeedState]&&dstx<rm+DR2&&dst<RMAX){
			addrand(1);
			//can attack at once
			//checking angle of rotation
			if(OBJ->LockType==1)OBJ->DestX=-1;
			char EnDir=char(GetDir(x0-OBJ->RealX,y0-OBJ->RealY));
			char ddir=EnDir-OBJ->RealDir;
			bool NewArt=OBJ->MotionStyle==3;
			if(NewArt){
				int enmd=((EnDir+8+512)>>4)&15;
				int pusd=((OBJ->RealDir+8+512)>>4)&15;
				if(enmd==pusd){
					ddir=0;
					//OBJ->RealDir=EnDir;
					if(OBJ->NewState==2){
						OBJ->NewState=0;
						addrand(OB->NewState+1);
						OBJ->GroundState=0;
						addrand(OB->GroundState+1);
						TryToStand(OBJ,0);
						return;
					};
				}else{
					if(OBJ->LocalNewState==2){
						if(OBJ->NewAnm->Code==anm_PMotionL+1){//to optimise
							OBJ->RealDir+=16;
							OBJ->GraphDir=OBJ->RealDir;
							TryToStand(OB,0);
							return;//
						}else 
						if(OBJ->NewAnm->Code==anm_PMotionR+1){//to optimise
							TryToStand(OB,0);
							return;//
						}else{
							if(ddir>0){
								OBJ->NewAnm=NMN->GetAnimation(anm_PMotionL+1);
								OBJ->SetZeroFrame();
#ifdef COSSACKS2
								OBJ->DestX=-1;
#endif
								return;
							}else{
								OBJ->NewAnm=NMN->GetAnimation(anm_PMotionR+1);
								OBJ->SetZeroFrame();
								OBJ->RealDir-=16;
								OBJ->GraphDir=OBJ->RealDir;
#ifdef COSSACKS2
								OBJ->DestX=-1;
#endif
								return;
							};
						};
					}else{
						OBJ->NewState=2;
						OBJ->GroundState=2;
						addrand(OB->GroundState+1);
						TryToStand(OB,0);
						return;
					};
				};
			};
			if(abs(ddir)<=MRot1){
				addrand(4);
				OBJ->RealDir=EnDir;
				OBJ->GraphDir=EnDir;
				if(OBJ->LocalNewState==NeedState+1||(OBJ->LockType==1&&OBJ->NewState==NeedState+1)){
					addrand(3);
					if(!DELAY){
						addrand(2);
						//checking attack ability
						Weapon* WP;
						if(OBJ->LockType==1)WP=NMN->DamWeap[OBJ->NewState-1];
						else WP=NMN->DamWeap[OBJ->LocalNewState-1];
						if(WP){
							Coor3D C3D;
							word DestObj=0xFFFF;
							if(OB->GetDamagePoint(OBJ->x,OBJ->y,&C3D,0))DestObj=OID;
							if(OB->Wall)DestObj=OID;
							//OBJ->BlockUnit();
							int x0,y0,z0,x1,y1,z1;
							x0=(OBJ->RealX>>4);
							y0=(OBJ->RealY>>4);
							z0=OBJ->RZ+32;//NMN->SrcZPoint;
							x1=C3D.x;
							y1=C3D.y;
							z1=C3D.z;
							int CheckBreaksBetweenPoints(int x,int y,int z,int xd,int yd,int zd,int Index);
							int WRes=-1;//OBJ->LocalNewState==3||OBJ->MotionStyle==2?-1:CheckBreaksBetweenPoints(x0,y0,z0,x1,y1,z1,OBJ->Index);
							//PredictShotEx(WP,x0,y0,z0,x1,y1,z1,OBJ->Index);
							if(WRes!=-1){
								if(OBJ->NewCurSprite==0){
									
#ifdef COSSACKS2	
									//OBJ->DestX=-1;
									//OBJ->CreatePath(OB->x,OB->y);
#else
									if(WRes<0xFFFF){
										OneObject* OB=Group[WRes];
										if(OB&&OB->NewBuilding&&OB->NInside&&!(OBJ->NMask&OB->NMask))goto DoShot1;
									};
									OBJ->NewAnm=&NMN->PStand[OBJ->LocalNewState-1];
									addrand(OBJ->Index);
									OBJ->DeleteLastOrder();
									return;
#endif
								};
								char atts[128];
								//something on the way...
								if(WRes>=ULIMIT){
									//ground
									if(OBJ->NNUM==MyNation){
										sprintf(atts,MESHPOV,OBJ->newMons->Message);
										//AssignHint1(atts,48);
									};
								}else{
									/*if(WRes==OB->Index)*/goto DoShot1;
									OneObject* OBX=Group[WRes];
									if(OBX){
										if(ShotRecommended&&!(OBX->NMask&OBJ->NMask))goto DoShot1;
										if(OBJ->NNUM==MyNation){
											sprintf(atts,MESHOBJ,OBJ->newMons->Message,OBX->newMons->Message);
											//AssignHint1(atts,48);
										};
									};
								};
								//attempt to move
								addrand(2);
								if( !(OBJ->StandGround || NoMove) ){
									OBJ->NewMonsterSendTo(OB->GetAttX(),OB->GetAttY(),128+16,1,OB,30);
									return;
									/*
									addrand(1);
									int dxx=y1-y0;
									int dyy=x0-x1;
									int nn=Norma(dxx,dyy);
									dxx=(dxx*20)/nn;
									dyy=(dyy*20)/nn;
									bool LAllow=true;
									bool RAllow=true;
									int XL=x0;
									int YL=y0;
									int XR=x0;
									int YR=y0;
									for(int j=0;j<10&&(LAllow||RAllow);j++){
										XL+=dxx;
										YL+=dyy;
										XR-=dxx;
										YR-=dyy;
										if(CheckPt(XL>>4,YL>>4))LAllow=false;
										if(CheckPt(XR>>4,YR>>4))RAllow=false;
										if(LAllow&&PredictShot(WP,XL,YL,z0,x1,y1,z1,OBJ->Index)==-1){
											OBJ->NewMonsterSendTo((XL+dxx)<<4,(YL+dyy)<<4,128+16,1);
											return;
										};
										if(RAllow&&PredictShot(WP,XR,YR,z0,x1,y1,z1,OBJ->Index)==-1){
											OBJ->NewMonsterSendTo((XR-dxx)<<4,(YR-dyy)<<4,128+16,1);
											return;
										};
										if(CheckPt(XL>>4,YL>>4))LAllow=false;
										if(CheckPt(XR>>4,YR>>4))RAllow=false;
#ifdef COSSACKS2
										
#endif
									};
									*/
#ifdef COSSACKS2
									//OBJ->CreatePath(OB->x,OB->y);
									//return;
#endif

									if(CANSETDELAY){
										OBJ->delay=20;
										OBJ->MaxDelay=20;
										if(OBJ->delay>OBJ->MaxDelay)OBJ->delay=OBJ->MaxDelay;
										addrand(OBJ->delay);
									};
									addrand(OBJ->Index);
									OBJ->DeleteLastOrder();
									return;
								}else{
									addrand(OBJ->Index);
									OBJ->DeleteLastOrder();
									return;
								};
								if(CANSETDELAY){
									OBJ->delay=20;
									OBJ->MaxDelay=20;
									addname(OBJ->Index);
								};
							};
DoShot1:;
							if((NMN->ResAttType==OBJ->LocalNewState-1||NMN->ResAttType1==OBJ->LocalNewState-1)&&NMN->NShotRes){
								bool AllowShot=true;
								for(int k=0;k<NMN->NShotRes;k++){
									if(XRESRC(OBJ->NNUM,NMN->ShotRes[k+k])<NMN->ShotRes[k+k+1])AllowShot=false;
								};
								if(AllowShot){
									if(!NMN->SlowRecharge){
										for(int k=0;k<NMN->NShotRes;k++){
											AddXRESRC(OBJ->NNUM,NMN->ShotRes[k+k],-NMN->ShotRes[k+k+1]);
											OBJ->Nat->AddResource(byte(NMN->ShotRes[k+k]),-NMN->ShotRes[k+k+1]);
											OBJ->Nat->ResOnLife[NMN->ShotRes[k+k]]+=NMN->ShotRes[k+k+1];
										}
									}else{
										if(OBJ->NNUM==GSets.CGame.cgi_NatRefTBL[MyNation]){
											fes_bAmunition[OBJ->NIndex]=false;
										}
									}
								}else{
									if(!NMN->SlowRecharge){
										if(OBJ->NNUM==MyNation&&OBJ->PrioryLevel>=16){
											char ccc[150];
											sprintf(ccc,GetTextByID("CSNORS"),OBJ->Ref.General->Message);
											AssignHint1(ccc,64,32);
										};
										addrand(OBJ->Index);
										OBJ->DeleteLastOrder();
										return;
									}else{
										if(OBJ->NNUM==GSets.CGame.cgi_NatRefTBL[MyNation]){											
											fes_neAmmunition(OBJ->NIndex);
										}
									}
								}
							}
						}
						if(!DELAY){
							//if((tmtmt&15)==3){
								//attacks animation begins there
								if(OBJ->LockType==1 && OBJ->NewState)OBJ->NewAnm=NMN->GetAnimation(anm_Attack+OBJ->NewState-1);
								else OBJ->NewAnm=NMN->GetAnimation(anm_Attack+OBJ->LocalNewState-1);
								OBJ->InMotion=false;
								OBJ->BackMotion=false;
								OBJ->SetZeroFrame();
	#ifdef COSSACKS2
								OBJ->DestX=-1;
	#endif
								return;
							//}else return;
							//}else{
							//	addname(OBJ->Index);
							//	TryToStand(OBJ,0);
							//};
						}
					}else{
						OBJ->DestX=-1;
						OBJ->DeletePath();
					}
				}else{
					OBJ->NewState=NeedState+1;
					OBJ->DestX=-1;
					TryToStand(OBJ,0);
					addrand(OB->NewState+1);
					return;
				};
			}else if(!NewArt){
				if(NMN->RotationAtPlaceSpeed){
					void RotUnit(OneObject* OB,char Dir,byte OrdType, byte PrioryLevel);
					RotUnit(OBJ,EnDir,1,128+16);
					OBJ->DestX=-1;
					return;
				}
				addrand(5);
				if(!AMove){
					addrand(6);
					//if(abs(ddir)>MRot1){
					OBJ->DestX=-1;
					OBJ->DestY=-1;
					/*if(abs(ddir)>36){
						OBJ->NewState=OBJ->GroundState;
						addrand(OB->NewState+1);
						addname(OBJ->Index);
						TryToStand(OBJ,false);
					};*/
                    if(OBJ->UsesDestDidection){
                        RotateMon(OBJ,ddir);
                    }else{
					    if(ddir>0)RotateMon(OBJ,MRot);//OBJ->RealDir+=16;
					    else RotateMon(OBJ,-MRot);//OBJ->RealDir-=16;
                    }
				}else{
					OBJ->DestX=-1;
					OBJ->DestY=-1;
					addrand(31);
                    if(OBJ->UsesDestDidection){
                        RotateMon(OBJ,ddir);
                    }else{
					    if(ddir>0)RotateMon(OBJ,min(int(ddir),MRot));//OBJ->RealDir+=16;
					    else RotateMon(OBJ,max(int(ddir),(-MRot)));//OBJ->RealDir-=16;
                    }
				};
			};
		}else{
			addrand(334);
			if(OBJ->StandGround&&!AttGroundMod){
				addrand(112);
				OBJ->DeleteLastOrder();
				return;
			};
			if(dst>=OBJ->GetMaxAttackRadius(NeedState)+DR2||dst>=RMAX){
				addrand(223);
				Order1* OR1=OB->LocalOrder;
				//if(NATIONS[OBJ->NNUM].AI_Enabled&&OB->InMotion&&(OB->EnemyID==0xFFFF)){
				//	addrand(122);
				//	int dde=GetDir(OB->GetAttX()-OBJ->RealX,OB->GetAttY()-OBJ->RealY);
				//	char ddr=OB->RealDir-dde;
				//	if(abs(ddr)<64){
				//		addrand(132);
				//		OBJ->DeleteLastOrder();
				//		return;
				//	};
				//};
				if(AMove){
					addrand(26);
					/*if(OBJ->NewState!=NeedState+1){
						addrand(289);
						OBJ->NewState=NeedState+1;
						addrand(OB->NewState+1);
					}else{*/
						addrand(244);
						if(!SetDestUnit(OBJ,OB,OLDX,OLDY,NEWX,NEWY,OBJ->GetMaxAttackRadius(NeedState))){
							OBJ->DeleteLastOrder();
							return;
						};
					//};
				}else{
					addrand(44);
					/*if(OBJ->NewState!=OBJ->GroundState){
						addrand(234);
						OBJ->NewState=OBJ->GroundState;
						addrand(OB->NewState+1);
						addname(OBJ->Index);
						TryToStand(OBJ,false);
					}else{*/
						addrand(1123);
						if(!SetDestUnit(OBJ,OB,OLDX,OLDY,NEWX,NEWY,OBJ->GetMaxAttackRadius(NeedState))){
							OBJ->DeleteLastOrder();
							return;
						};
					//};
				};
				return;
			}else{
				//need to make several steps back
				if(!OBJ->StandGround){
					int R0=(OBJ->GetMaxAttackRadius(NeedState)+ADC->AttackRadius1[NeedState]*3)/4;
                    int dx=OB->GetAttX()-OBJ->RealX;
					int dy=OB->GetAttY()-OBJ->RealY;
                    int N=Norma(dx,dy);
					OBJ->GroupSpeed=OBJ->newMons->MotionDist*3/5;
					OBJ->UnitSpeed=64;
					if(N){
						dx=dx*R0/N;
						dy=dy*R0/N;
						OBJ->NewMonsterSendTo(OB->GetAttX()-(dx<<4),OB->GetAttY()-(dy<<4),128+16,1);
						return;
					}
				}else{
					OBJ->DeleteLastOrder();
					return;
				}				
			};
		};
	};
};
void MakeOneShotLink(OneObject* OBJ){
	int cf0=OBJ->NewCurSpritePrev;
	int cf1=OBJ->NewCurSprite;
	int ns=OBJ->NewState;		
	if(!ns){
		addrand(OBJ->Index);
		OBJ->DeleteLastOrder();
		if(CheckShooterAbilityToRecharge(OBJ)){
			OBJ->NewState=4;
			if(!OBJ->newMons->DontTransformToChargeState){				
				OBJ->LocalNewState=4;
			}else TryToStand(OBJ,0);
			OBJ->StandTime=20;
		}
		return;
	};
	NewMonster* NM=OBJ->newMons;
	AdvCharacter* AC=OBJ->MoreCharacter;
	NewAnimation* NMA=NM->GetAnimation(anm_Attack+ns-1);
	if(OBJ->NewAnm!=NMA){
		addrand(OBJ->Index);
		OBJ->DeleteLastOrder();
		if(CheckShooterAbilityToRecharge(OBJ)){
			OBJ->NewState=4;
			if(!OBJ->newMons->DontTransformToChargeState){				
				OBJ->LocalNewState=4;
			}else TryToStand(OBJ,0);
			OBJ->StandTime=20;
		}
		return;
	};
	int hf=NMA->ActiveFrame;
	
	/*if(cf0>=hf){
		OBJ->DeleteLastOrder();
		if(CheckShooterAbilityToRecharge(OBJ)){
			OBJ->NewState=4;
			OBJ->LocalNewState=4;
			OBJ->StandTime=20;
		}
		return;
	};*/
	
	if(cf1>=hf){
		Weapon* WP=NM->DamWeap[OBJ->NewState-1];
		if(WP){
			NewAnimation* UNAM=OBJ->NewAnm;
			int csp=OBJ->NewCurSprite;
			int uocts=(UNAM->Rotations-1)*2;
			int uoc2=UNAM->Rotations-1;
			if(!uocts)uocts=1;
			int usesize=div(255,uocts<<1).quot;
			byte udir=(((OBJ->RealDir+64+usesize)&255)*uocts)>>8;
			NewFrame* NF=UNAM->Frames[csp];
			//----------
			NewAnimation* NAM=WP->NewAnm;
			int octs=(NAM->Rotations-1)*2;
			int oc2=NAM->Rotations-1;
			if(!octs)octs=1;
			int sesize=div(255,octs<<1).quot;
			byte dir=(((OBJ->RealDir+64+sesize)&255)*octs)>>8;
			int x0,y0,z0,x1,y1,z1;
			if(udir<uoc2){
				int udir1=uoc2-udir;
				x0=(OBJ->RealX>>4)-(UNAM->ActivePt[udir1]->x+NF->dx);
				y0=(OBJ->RealY>>4)+Prop43(UNAM->ActivePt[udir1]->y+NF->dy+NM->SrcZPoint);
			}else{
				int udir1=uoc2-uocts+udir;
				x0=(OBJ->RealX>>4)+(UNAM->ActivePt[udir1]->x+NF->dx);
				y0=(OBJ->RealY>>4)+Prop43(UNAM->ActivePt[udir1]->y+NF->dy+NM->SrcZPoint);
			};						
			z0=OBJ->RZ+NM->SrcZPoint;
			x1=int(OBJ->LocalOrder->info.AttackXY.x)<<2;
			y1=int(OBJ->LocalOrder->info.AttackXY.y)<<2;
			z1=int(OBJ->LocalOrder->info.AttackXY.z);
			if(!OBJ->delay){
				if(udir<uoc2){
					udir=uoc2-udir;
					Create3DAnmObject(WP,x0,y0,z0,x1,y1,z1,OBJ,OBJ->NewState-1,0xFFFF);
				}else{
					udir=uoc2-uocts+udir;
					Create3DAnmObject(WP,x0,y0,z0,x1,y1,z1,OBJ,OBJ->NewState-1,0xFFFF);
				};
			}
			int ST=OBJ->NewState-1;
			if(OBJ->LockType!=1)ST=OBJ->LocalNewState-1;
			if(ST<0)ST=0;
			OBJ->delay=OBJ->MoreCharacter->AttackPause[ST];
			OBJ->MaxDelay=div(int(OBJ->delay)*OBJ->PersonalDelay,100).quot;
			OBJ->MaxDelay=int(OBJ->MaxDelay)*100/OBJ->GetAttackSpeedBonus();
			OBJ->delay=OBJ->MaxDelay;

			OBJ->DeleteLastOrder();
			if(CheckShooterAbilityToRecharge(OBJ)){
				OBJ->NewState=4;
				if(!OBJ->newMons->DontTransformToChargeState){					
					OBJ->LocalNewState=4;
				}else TryToStand(OBJ,0);
				OBJ->StandTime=20;
			}
		};
	};
};
void MakeOneShot(OneObject* OB,int xd,int yd,int zd){	
	addrand(1019);
	Order1* OR1=OB->CreateOrder(1);
	OR1->info.AttackXY.ox=OB->Index;
	OR1->info.AttackXY.x=xd>>2;
	OR1->info.AttackXY.y=yd>>2;
	OR1->info.AttackXY.z=zd;
	OR1->DoLink=&MakeOneShotLink;
	OR1->PrioryLevel=255;
	OB->PrioryLevel=255;
	MakeOneShotLink(OB);
};
void NewMonsterSmartSendToLink(OneObject* OBJ);
int SmartLink=0x153829;
void __stdcall CDGINIT_INIT1(){
	SmartLink=int(&NewMonsterSmartSendToLink)+0x7654391;
};
void WaterAttackLink(OneObject* OBJ){
	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;
	int NTimes=OBJ->LocalOrder->info.BuildObj.SN;
	bool ai=OBJ->Nat->AI_Enabled;
	NewMonster* NMN=OBJ->newMons;
	if(!NMN->MultiWp.GetAmount()){
		OBJ->DeleteLastOrder();
		return;
	};
	AdvCharacter* ADC=OBJ->MoreCharacter;	
	//OBJ->UnBlockUnit();
	word OID=OBJ->EnemyID;
	word OSN=OBJ->EnemySN;
	if(OID>8192){
		OBJ->DeleteLastOrder();
		return;
	};
	OneObject* OB=Group[OID];
	if(!OB||OB->Sdoxlo||OBJ->TurnOff||OSN!=OB->Serial){
		/*if(OBJ->PrioryLevel>=16&&OBJ->NNUM==MyNation&&OBJ->TurnOff){
			char ccc[128];
			sprintf(ccc,GetTextByID("CSNOGL"),OBJ->Ref.General->Message);
			AssignHint1(ccc,50,32);
		};*/
		OBJ->DestX=-1;
		OBJ->DeleteLastOrder();
		return;
	};
	int dst=Norma(OBJ->RealX-OB->GetAttX(),OBJ->RealY-OB->GetAttY())>>4;
	//detecting attack method
	int AttMethod=-1;
	for(int i=0;i<NAttTypes;i++){
		if(ADC->DetRadius1[i]<=dst&&ADC->DetRadius2[i]>=dst)AttMethod=i;
	};
	if(AttMethod!=-1){
		byte ATM=OBJ->LocalOrder->info.BuildObj.AttMethod;
		if(ATM!=0xFF){
			if(!OB->NewBuilding)AttMethod=ATM;
		};
		//if(OB->StandTime>5)dst+=90;
		//checking actual distance
		if(dst>=ADC->AttackRadius1[AttMethod]&&dst<=ADC->AttackRadius2[AttMethod]){
			{
				OBJ->DeletePath();
				//checking rotation
				char endir=char(GetDir(OB->GetAttX()-OBJ->RealX,OB->GetAttY()-OBJ->RealY));
				int minddir=1000;
				char mindir=0;
				int wpindex[64];
				int NGoodWp=0;
				for(int i=0;i<NMN->MultiWp.GetAmount();i++){
					char wdir=NMN->MultiWp[i]->Dir+OBJ->RealDir;
					char dd=endir-wdir;
					int absd=abs(int(dd));
					if(absd<minddir){
						minddir=absd;
						mindir=dd;
					}
					if(absd<NMN->MultiWp[i]->DirDiff){
						if(NGoodWp<64){
                            wpindex[NGoodWp]=i;
							NGoodWp++;
						}
					}
				}
				if(NGoodWp==0||NGoodWp<NMN->MaxZalp){
                    //if(OBJ->UsesDestDidection){
                    //    OBJ->DestDirection=endir;
                    //    OBJ->DestDirectionIsActive=true;
                    //}else{
                        if(mindir>0)OBJ->RealDirPrecise+=(GameSpeed*OB->newMons->MinRotator)>>4;
					    else OBJ->RealDirPrecise-=(GameSpeed*OB->newMons->MinRotator)>>4;
					    OBJ->RealDir=OBJ->RealDirPrecise>>8;
                    //}
				}else{
                    //we can attack
					int caindex=wpindex[rando()%NGoodWp];
					if(!OBJ->delay){
						bool AllowShot=true;
						if(NMN->NShotRes){
							for(int k=0;k<NMN->NShotRes;k++){
								if(XRESRC(OBJ->NNUM,NMN->ShotRes[k+k])<NMN->ShotRes[k+k+1])AllowShot=false;
							};
							if(AllowShot){
								for(int k=0;k<NMN->NShotRes;k++){
									AddXRESRC(OBJ->NNUM,NMN->ShotRes[k+k],-NMN->ShotRes[k+k+1]);
									OBJ->Nat->AddResource(byte(NMN->ShotRes[k+k]),-NMN->ShotRes[k+k+1]);
									OBJ->Nat->ResOnLife[NMN->ShotRes[k+k]]+=NMN->ShotRes[k+k+1];
								};								
								fes_bAmunition[OBJ->NIndex]=false;
							}else{
								if(OBJ->NNUM==GSets.CGame.cgi_NatRefTBL[MyNation]&&OBJ->PrioryLevel>=16){
									char ccc[150];
									sprintf(ccc,GetTextByID("CSNORS"),OBJ->Ref.General->Message);
									//AssignHint1(ccc,64,32);									
									fes_neAmmunition(OBJ->NIndex,ccc);
								};
								OBJ->DeleteLastOrder();
								return;
							};
						};
						if(AllowShot){
							ComplexFireSource* CFS=NMN->MultiWp[caindex];
							int COS=TCos[byte(OBJ->RealDir-64)];
							int SIN=TSin[byte(OBJ->RealDir-64)];		
                            int xs=(OBJ->RealX>>4)+((CFS->x*COS+CFS->y*SIN)>>8);
							int ys=(OBJ->RealY>>4)+((-CFS->y*COS+CFS->x*SIN)>>8);
							int zs=CFS->z;
							if(NMN->MaxZalp&&OBJ->NZalp){
								OBJ->NZalp--;
							}else{
								OBJ->NZalp=NMN->MaxZalp;
								OBJ->delay=NMN->AttackPause[AttMethod];
								OBJ->MaxDelay=div(int(OBJ->delay)*OBJ->PersonalDelay,100).quot;
								OBJ->MaxDelay=int(OBJ->MaxDelay)*100/OBJ->GetAttackSpeedBonus();
								OBJ->delay=OBJ->MaxDelay;
							};
							Coor3D C3D;
							word DestObj=0xFFFF;
							if(OB->GetDamagePoint(OBJ->x,OBJ->y,&C3D,0))DestObj=OID;
							if(OB->Wall)DestObj=OID;
							CreateRazbros(OBJ,&C3D);
							addrand(76);
							Weapon* WP=NMN->DamWeap[AttMethod];
							if(WP){
								Create3DAnmObject(WP,xs,ys,zs,
									C3D.x,C3D.y,C3D.z+OBJ->NewAnm->AddHeight,OBJ,AttMethod,0xFFFF);
							}
							if(NTimes==1){
								OBJ->DeleteLastOrder();
							}else{
								if(NTimes>1&&OBJ->LocalOrder)OBJ->LocalOrder->info.BuildObj.SN--;
							}                            
						}
					}
				}				
			}
			OBJ->DestX=-1;
		}else{
			if(dst>NMN->AttackRadius2[AttMethod]){
				if(NTimes>0){
					OBJ->DestX=-1;
					OBJ->DeleteLastOrder();
					return;
				};
				if(!OBJ->StandGround){
					OBJ->CreatePath(OB->x,OB->y);
				}else{
					OBJ->DestX=-1;
					OBJ->DeleteLastOrder();
				}
				//OBJ->DeleteLastOrder();
			}else{
				OBJ->DestX=-1;
				OBJ->DeleteLastOrder();
			}
		}
	}else{ 
		OBJ->DestX=-1;
		OBJ->DeleteLastOrder();
	}
};
//-----------------------Search for the enemy------------------------
word GetNewEnemy(int xr,int yr,byte NI){
	NI=GSets.CGame.cgi_NatRefTBL[NI];
	byte nms=1<<NI;
	OneObject* OBX=NULL;
	for(int ii=0;ii<MAXOBJECT;ii++){
		OneObject* OB=Group[ii];
		if(OB&&(!( (nms&OB->NMask) || OB->Sdoxlo))){
			int xs,ys,xs1,ys1,LY;
			GetRect(OB,&xs,&ys,&xs1,&LY);
			ys1=ys+LY-1;
			xs1+=xs-1;
			if(PInside(xs,ys,xs1,ys1,xr,yr)){
				if((!OBX)||OBX->NewBuilding){
					OBX=Group[ii];
				};
			};
		};
	};
	if(OBX)return OBX->Index;
	else return 0xFFFF;
};
word AdvancedGetNewEnemyInCell(int cx,int cy,int xr,int yr,byte EMask){
	if(cx<0||cy<0||cx>=VAL_MAXCX||cy>=VAL_MAXCX)return 0xfFFF;
	int cell=VAL_MAXCX+1+cx+(cy<<VAL_SHFCX);
	cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	int ofs1=cell<<SHFCELL;
	word MID;
	Rct rctSel( xr, yr, 1 , 1 );
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!(OB->NMask&EMask))&&!OB->Sdoxlo){
				if(rctSel.Overlap(GetRect(OB)))return MID;								
			};
		};
	};
	/*MID=BLDList[cell];
	if(MID!=0xFFFF){
		OneObject* OB=Group[MID];
		if(OB&&(!(OB->NMask&EMask))&&!OB->Sdoxlo){			
			if(rctSel.Overlap(GetRect(OB)))return MID;			
		};
	};	*/
	return 0xFFFF;
};
word CheckCoorInGP(int x,int y);
WORD CheckCoorInGP_withoutMask( int x, int y ,byte Mask);
bool GetObjectVisibilityInFog(OneObject* OB,int MinFogValue);
word AdvancedGetNewEnemy(int rex,int rey,int xr,int yr,byte NI){
	byte nms=NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].NMask;
	word ID=CheckCoorInGP_withoutMask(xr,yr,nms);
	if(ID!=0xFFFF){
		OneObject* OB=Group[ID];
		if(OB && (!(OB->Sdoxlo || OB->NMask&nms /*|| OB->newMons->Immortal*/)) && GetObjectVisibilityInFog(OB,100))return ID;
	};
	/*int x0=rex>>7;
	int y0=rey>>7;
	for(int ix=-2;ix<3;ix++){
		for(int iy=-2;iy<3;iy++){
			word MID=AdvancedGetNewEnemyInCell(x0+ix,y0+iy,xr,yr,nms);
			if(MID!=0xFFFF)return MID;
		};
	};*/
	return 0xFFFF;
};
word GetNewFriend(int xr,int yr,byte NI){
	byte nms=1<<NI;
	for(int ii=0;ii<MAXOBJECT;ii++){
		OneObject* OB=Group[ii];
		if(OB&&OB->NNUM==NI&&!OB->Sdoxlo){
			int xs,ys,xs1,ys1,LY;
			GetRect(OB,&xs,&ys,&xs1,&LY);
			ys1=ys+LY-1;
			xs1+=xs-1;
			if(PInside(xs,ys,xs1,ys1,xr,yr))return ii;
		};
	};
	return 0xFFFF;
};
word AdvancedGetNewFriendInCell(int cx,int cy,int xr,int yr,byte nms){
	if(cx<0||cy<0||cx>=VAL_MAXCX||cy>=VAL_MAXCX)return 0xfFFF;
	int cell=VAL_MAXCX+1+cx+(cy<<VAL_SHFCX);
	cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	int ofs1=cell<<SHFCELL;
	word MID;
	Rct rctSel( xr, yr, 1 , 1 );
	/*
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NMask&nms&&!OB->Sdoxlo){
				if(rctSel.Overlap(GetRect(OB)))return MID;
			};
		};
	};
	*/
	MID=GetOneBld(cell,0);
	if(MID!=0xFFFF){
		OneObject* OB=Group[MID];
		if(OB&&OB->NMask&nms&&!OB->Sdoxlo){
			if(rctSel.Overlap(GetRect(OB)))return MID;			
		};
	};
	return 0xFFFF;
};
word AdvancedGetNewFriend(int rex,int rey,int xr,int yr,byte NI){
	byte nms=NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].NMask;
	word ID=CheckCoorInGP(xr,yr);
	if(ID!=0xFFFF){
		OneObject* OB=Group[ID];
		if(OB&&(!OB->Sdoxlo)&&(OB->NMask&nms)&&GetObjectVisibilityInFog(OB,100))return ID;
	};

	/*int x0=rex>>7;
	int y0=rey>>7;
	for(int ix=-2;ix<3;ix++){
		for(int iy=-2;iy<3;iy++){
			word MID=AdvancedGetNewFriendInCell(x0+ix,y0+iy,xr,yr,nms);
			if(MID!=0xFFFF)return MID;
		};
	};*/
	return 0xFFFF;	
};
word GetDoorsInCell(int cx,int cy,int xr,int yr,byte NI){
	/*if(cx<0||cy<0||cx>=VAL_MAXCX||cy>=VAL_MAXCX)return 0xfFFF;
	int cell=VAL_MAXCX+1+cx+(cy<<VAL_SHFCX);
	cell+=VAL_MAXCX+1;
    byte mask=NATIONS[NI].NMask;
	word MID=BLDList[cell];
	if(MID!=0xFFFF){
		OneObject* OB=Group[MID];
		if(OB&&(!(OB->NMask&mask))&&!OB->Sdoxlo){
			NewMonster* NM=OB->newMons;
			if(NM->Doors){
				int x0=(OB->RealX>>4)+NM->PicDx;
				int y0=(OB->RealY>>5)+NM->PicDy-OB->RZ;
				ExRect* DR=NM->Doors;
				int j4=0;
				for(int j=0;j<DR->NRects;j++){
					int xL=x0+DR->Coor[j4  ];
					int yL=y0+DR->Coor[j4+1];
					int xR=x0+DR->Coor[j4+2];
					int yR=y0+DR->Coor[j4+3];
					if(xr>=xL&&xr<=xR&&yr>=yL&&yr<=yR)return MID;
					j4+=4;
				};
			};
		};
	};*/
	return 0xFFFF;
};
word GetDoorsOfEnemy(int rex,int rey,int xr,int yr,byte NI){
	NI=GSets.CGame.cgi_NatRefTBL[NI];
	int x0=rex>>7;
	int y0=rey>>7;
	for(int ix=-5;ix<5;ix++){
		for(int iy=-5;iy<5;iy++){
			word MID=GetDoorsInCell(x0+ix,y0+iy,xr,yr,NI);
			if(MID!=0xFFFF)return MID;
		};
	};
	return 0xFFFF;
};
//------------------Block&UnBlock---------------------//
void OneObject::WeakBlockUnit(){
	if(BlockInfo)return;
	/*
	if(!CheckPt(x,y)){
		BlockInfo|=1;
		BSetPt(x,y);
	};
	if(!CheckPt(x,y-1)){
		BlockInfo|=2;
		BSetPt(x-1,y-1);
	};
	if(!CheckPt(x,y+1)){
		BlockInfo|=4;
		BSetPt(x,y+1);
	};
	if(!CheckPt(x-1,y)){
		BlockInfo|=8;
		BSetPt(x+1,y);
	};
	if(!CheckPt(x-1,y)){
		BlockInfo|=16;
		BSetPt(x-1,y);
	};
	return;
	//--------------------
	*/
	/*if(!CheckPt(x-1,y-1)){
		BlockInfo|=1;
		BSetPt(x-1,y-1);
	};
	if(!CheckPt(x,y-1)){
		BlockInfo|=2;
		BSetPt(x,y-1);
	};
	if(!CheckPt(x+1,y-1)){
		BlockInfo|=4;
		BSetPt(x+1,y-1);
	};
	if(!CheckPt(x-1,y)){
		BlockInfo|=8;
		BSetPt(x-1,y);
	};*/
	if(!CheckPt(x,y)){
		BlockInfo|=16;
		BSetPt(x,y);
	};
	/*
	if(!CheckPt(x+1,y)){
		BlockInfo|=32;
		BSetPt(x+1,y);
	};
	if(!CheckPt(x-1,y+1)){
		BlockInfo|=64;
		BSetPt(x-1,y+1);
	};
	if(!CheckPt(x,y+1)){
		BlockInfo|=128;
		BSetPt(x,y+1);
	};
	if(!CheckPt(x+1,y+1)){
		BlockInfo|=256;
		BSetPt(x+1,y+1);
	};*/
};
void OneObject::BlockUnit(){
	//return;
	if(BlockInfo)return;
	if(!CheckPt(x-1,y-1)){
		BlockInfo|=1;
		BSetPt(x-1,y-1);
	};
	if(!CheckPt(x,y-1)){
		BlockInfo|=2;
		BSetPt(x,y-1);
	};
	if(!CheckPt(x+1,y-1)){
		BlockInfo|=4;
		BSetPt(x+1,y-1);
	};
	if(!CheckPt(x-1,y)){
		BlockInfo|=8;
		BSetPt(x-1,y);
	};
	if(!CheckPt(x,y)){
		BlockInfo|=16;
		BSetPt(x,y);
	};
	if(!CheckPt(x+1,y)){
		BlockInfo|=32;
		BSetPt(x+1,y);
	};
	if(!CheckPt(x-1,y+1)){
		BlockInfo|=64;
		BSetPt(x-1,y+1);
	};
	if(!CheckPt(x,y+1)){
		BlockInfo|=128;
		BSetPt(x,y+1);
	};
	if(!CheckPt(x+1,y+1)){
		BlockInfo|=256;
		BSetPt(x+1,y+1);
	};
};
void OneObject::UnBlockUnit(){	
	if(!BlockInfo)return;
	/*if(BlockInfo&1)BClrPt(x,y);
	if(BlockInfo&2)BClrPt(x,y-1);
	if(BlockInfo&4)BClrPt(x,y+1);
	if(BlockInfo&8)BClrPt(x+1,y);
	if(BlockInfo&16)BClrPt(x-1,y);
	return;*/
	
	if(BlockInfo&1)BClrPt(x-1,y-1);
	if(BlockInfo&2)BClrPt(x  ,y-1);
	if(BlockInfo&4)BClrPt(x+1,y-1);
	if(BlockInfo&8)BClrPt(x-1,y);
	if(BlockInfo&16)BClrPt(x  ,y);
	if(BlockInfo&32)BClrPt(x+1,y);
	if(BlockInfo&64)BClrPt(x-1,y+1);
	if(BlockInfo&128)BClrPt(x  ,y+1);
	if(BlockInfo&256)BClrPt(x+1,y+1);

	BClrBar(x,y,1);
	BlockInfo=0;
};
void OneObject::DeletePath(){
	if(PathX){
		FastFreeMem(PathX);
		FastFreeMem(PathY);
		PathX=NULL;
		PathY=NULL;
        CPdestX=-1;
		NIPoints=0;
	};
};
//Creation the building
bool OneObject::FindPoint(int* x1,int* y1,byte Flags){
	NewMonster* NM=newMons;
	short* px;
	short* py;
	int Shift=0;
	int Npt=0;
	if(Flags&FP_FIND_WORKPOINT){
		px=NM->BuildPtX.GetValues();
		py=NM->BuildPtY.GetValues();
		Npt=NM->BuildPtX.GetAmount();
	};
	if(Flags&FP_FIND_DAMPOINT){
		px=NM->DamPtX.GetValues();
		py=NM->DamPtY.GetValues();
		Npt=NM->DamPtX.GetAmount();
	};
	if(Flags&FP_CONCENTRATION){
		px=NM->ConcPtX.GetValues();
		py=NM->ConcPtY.GetValues();
		Npt=NM->ConcPtX.GetAmount();
		Shift=4;
	};
	if(!Npt)return false;
	if(Flags&FP_NEAREST_POINT){
		int xx=*x1;
		int yy=*y1;
		int dist=10000;
		int dis;
		int PicSX=NM->PicDx<<4;
		int PicSY=(NM->PicDy)<<5;
		int x0=(RealX+PicSX)>>8;
		int y0=(RealY+PicSY)>>8;
		int x2,y2,x3,y3;
		if(Flags&FP_UNLOCKED_POINT){
			for(int i=0;i<Npt;i++){
				x2=x0+(px[i]>>Shift);
				y2=y0+(py[i]>>Shift);
				dis=Norma(x2-xx,y2-yy);
				if(dis<dist&&!CheckBar(x2-1,y2-1,3,3)){
					dist=dis;
					x3=x2;
					y3=y2;
				};
			};
		}else{
			for(int i=0;i<Npt;i++){
				x2=x0+(px[i]>>Shift);
				y2=y0+(py[i]>>Shift);
				dis=Norma(x2-xx,y2-yy);
				if(dis<dist){
					dist=dis;
					x3=x2;
					y3=y2;
				};
			};
		};
		if(dist<10000){
			*x1=x3;
			*y1=y3;
			return true;
		};
	};
	return false;
};
void CmdFieldBar(byte NI,word n);
void CreateFields(byte NI,int x,int y,int n);
void InvitePeasant(OneObject* Mine);
extern bool EditMapMode;
void OneObject::NextStage(){
	if(!NewBuilding||Life==MaxLife)return;
	NewMonster* NM=newMons;
	AdvCharacter* ACR=MoreCharacter;
	if(Stage<ACR->ProduceStages){
		Stage++;
		//Life=div(Stage*MaxLife,ACR->ProduceStages).quot;
		int L1=Stage*MaxLife/ACR->ProduceStages;
		int L2=(Stage-1)*MaxLife/ACR->ProduceStages;
		Life+=L1-L2;
		int nn=div(Stage<<2,ACR->ProduceStages).quot;
		LoLayer=NM->GetAnimation(anm_BuildLo+nn);
		//HiLayer=NM->GetAnimation(anm_PMotionR+nn);
		NewCurSpritePrev=NewCurSprite;
		NewCurSprite=0;
		if(Stage==ACR->ProduceStages){			
			LogBattle(NNUM,"^ST_BLD^ %s (%d)",GetSt_UName(NIndex),CITY[NNUM].ReadyAmount[NIndex]+1);
			if(NM->NBLockPt){
				int xx,yy;
				this->GetCornerXY(&xx,&yy);
				for(int i=0;i<NM->NBLockPt;i++){
					BClrPt(xx+NM->BLockX[i],yy+NM->BLockY[i]);
				};
				for(int i=0;i<NM->NLockPt;i++){
					BSetPt(xx+NM->LockX[i],yy+NM->LockY[i]);
				};
			};
			if(NM->AutoBornSprite!=-1 && NewBuilding){
				void addSpriteAnyway(int x,int y,SprGroup* SG,word id);
				addSpriteAnyway(RealX>>4,RealY>>4,&TREES,NM->AutoBornSprite);
			}
			Ready=true;
			LoLayer=NM->GetAnimation(anm_StandLo);
			HiLayer=NM->GetAnimation(anm_StandHi);
            NewAnm=NM->GetAnimation(anm_Work);
			NewCurSpritePrev=NewCurSprite;
            NewCurSprite=0;
			byte Usage=newMons->Usage;
			if(Usage==TowerID)GNFO.AddTow(this);
#ifdef CONQUEST
			if(Usage==MelnicaID){
				if(Nat->PACount[NIndex])CreateFields(NNUM,RealX,RealY,Nat->PAble[NIndex][0]);
			};
#else
			if(Usage==MelnicaID&&(!Nat->AI_Enabled)&&Nat->CITY->ReadyAmount[NIndex]==0){
				if(Nat->PACount[NIndex])CreateFields(NNUM,RealX,RealY,Nat->PAble[NIndex][0]);
			};
#endif
			if(Usage==MineID&&!EditMapMode){
				addrand(6);
				InvitePeasant(this);
			};
			byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
			if(NNUM==NI && !(NotSelectable||NM->NewParams.PortretDisable) ){
				char ccc[200];
				//void PlaySound(char* Name);
				//PlaySound("POSTROYKA_UP");
				void MakeOrderSound(OneObject* OB,byte SMask);
				MakeOrderSound(this,8);
#ifdef _USE3D
				ccc[0] = 0;
				int msgLen = strlen( Ref.General->Message );
				if (msgLen < 100) 
#endif // _USE3D
				sprintf(ccc,BLDBLD,Ref.General->Message);
				AssignHint1(ccc,100);
				LastActionX=RealX>>4;
				LastActionY=RealY>>4;
			};
			Nat->CITY->Account+=EngSettings.BornModulator*newMons->Expa;;
            if(Life>=MaxLife)Life=MaxLife;
			for(int i=0;i<NM->NewParams.MassSprites.GetAmount();i++){				
				MassSprite* msp=NM->NewParams.MassSprites[i];
				addSprite(RealX/16+msp->dx,RealY/16+msp->dy,&TREES,msp->Sprite);
			}
		};
	}else{
		Life+=20;
		LoLayer=NM->GetAnimation(anm_StandLo);
		HiLayer=NM->GetAnimation(anm_StandHi);
        NewAnm=NM->GetAnimation(anm_Work);
		//NewCurSprite=0;
		if(Life>=MaxLife){
			Life=MaxLife;
			InFire=0;
			FireOwner=0xFF;
			FiringStage=0;
		};
		//Ready=true;
		if(newMons->Usage==TowerID&&NNUM==0)GNFO.AddTow(this);
	};
};
void OneObject::ClearBuildPt(){
    if(NewBuilding&&Stage>=NStages&&Life>=MaxLife)return;
	NewMonster* NM=newMons;
	short* px=NM->BuildPtX.GetValues();
	short* py=NM->BuildPtY.GetValues();
	int Npt=NM->BuildPtX.GetAmount();
	if(!Npt)return;
	int PicSX=NM->PicDx<<4;
	int PicSY=(NM->PicDy)<<5;
	int x0=(RealX+PicSX)>>8;
	int y0=(RealY+PicSY)>>8;
	for(int i=0;i<Npt;i++){
		if(UnitsField.CheckPt(x0+px[i],y0+py[i]))MFIELDS[0].BClrPt(x0+px[i],y0+py[i]);
	}
};
void BuildObjLink(OneObject* OBJ);
void TakeResLink(OneObject* OB);
void UnConstructBuilduingLink(OneObject* OBJ);
bool FindNearestBildPlace(OneObject* Worker);
bool OneObject::BuildObj(word OID,int Prio,bool LockPoint,byte OrdType){
	addrand(OID);
	if(CheckOrderAbility())return false;
	if(Prio<PrioryLevel&&Prio<100)return false;
    if(LocalOrder&&LocalOrder->DoLink==&BuildObjLink&&LocalOrder->info.BuildObj.ObjIndex==OID)return true;
	if(LocalOrder&&LocalOrder->DoLink==&UnConstructBuilduingLink)return false;
	NewMonster* NM=newMons;
	if(!NM->GetAnimation(anm_Work)->Enabled)return false;
	OneObject* OB=Group[OID];
	if(!int(OB)||OB->Sdoxlo)return false;
	if(!OB->NewBuilding)return false;
	if(!(OB->NMask&NMask))return false;
	if(OB->Life>=OB->MaxLife)return false;
	int xx=x;
	int yy=y;
	if(OB->FindPoint(&xx,&yy,FP_NEAREST_POINT|FP_FIND_WORKPOINT|FP_UNLOCKED_POINT)){
		//if(OrdType==0||OrdType==2)NewMonsterSmartSendTo(xx<<4,yy<<4,0,0,Prio,OrdType);
		if(OrdType==2&&LocalOrder&&LocalOrder->DoLink==&TakeResLink){
			DeleteLastOrder();
			OrdType=1;
		};
		//Important=true;
		Order1* Or1;
		addrand(1020);
		if(!OrdType){
			if(LocalOrder&&LocalOrder->DoLink==&BuildObjLink)Or1=CreateOrder(2);
			else Or1=CreateOrder(0);
		}
		else Or1=CreateOrder(OrdType);
		if(!int(Or1))return false;
		Or1->OrderType=89;//Build
		Or1->OrderTime=0;
		Or1->DoLink=&BuildObjLink;
		Or1->info.BuildObj.ObjIndex=OID;
		Or1->info.BuildObj.ObjX=xx;
		Or1->info.BuildObj.ObjY=yy;
		Or1->info.BuildObj.SN=OB->Serial;
		Or1->PrioryLevel=Prio&127;
		if(LockPoint && !MFIELDS[0].CheckPt(xx,yy)){
			MFIELDS[0].BSetPt(xx,yy);
			UnitsField.BSetPt(xx,yy);
		}
		//PrioryLevel=Prio&127;	
		//PrioryLevel=LocalOrder->PrioryLevel;
		if(OrdType==1||OrdType==0)NewMonsterSmartSendTo(xx<<4,yy<<4,0,0,Prio,1);
		addrand(OID);
		return true;
	};
	return false;
};
void InvitePeasant(OneObject* Mine){
	if(!SCENINF.hLib){
		if(Mine->Nat->AI_Enabled)return;
	};
	word PLIST[64];
	int Ri[64];
	int NP=Mine->Ref.General->newMons->MaxInside;
	if(NP>64)NP=64;
	byte NI=Mine->NNUM;
	int NN=0;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)&&OB->NNUM==NI&&OB->Ref.General->newMons->Peasant&&
			((OB->LocalOrder&&OB->LocalOrder->DoLink==&BuildObjLink&&OB->LocalOrder->info.BuildObj.ObjIndex==Mine->Index)||
			(!OB->LocalOrder))){
			int R=Norma(OB->RealX-Mine->RealX,OB->RealY-Mine->RealY)>>4;
			addrand(i);
			addrand(R);
			if(R<512){
				addrand(i);
				if(NN<64){
					PLIST[NN]=i;
					Ri[NN]=R;
					if(!OB->LocalOrder)R+=500;
					NN++;
				};
			};
		};
	};
	addrand(NN);
	UNISORT.CheckSize(NN);
	for(int i=0;i<NN;i++){
		UNISORT.Parms[i]=Ri[i];
		UNISORT.Uids[i]=PLIST[i];
		addrand(Ri[i]);
		addrand(PLIST[i]);
	};
	UNISORT.NUids=NN;
	UNISORT.Sort();
	for(int i=0;i<NN&&i<NP;i++)Group[UNISORT.Uids[i]]->GoToMine(Mine->Index,128+16);
};
void TakeResLink(OneObject* OBJ);
void InviteAI_Peasants(OneObject* Mine){
	if(Mine->Nat->CITY->FreePS>Mine->Nat->CITY->Nat->MIN_PBRIG)return;
	word PLIST[512];
	int Ri[512];
	int NP=Mine->Ref.General->newMons->MaxInside+Mine->AddInside-Mine->NInside;
	if(NP<0)NP=0;
	if(NP>64)NP=64;
	byte NI=Mine->NNUM;
	int NN=0;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)&&OB->NNUM==NI&&OB->Ref.General->newMons->Peasant&&
			((OB->LocalOrder&&OB->LocalOrder->DoLink==&TakeResLink)||(!OB->LocalOrder))){
			int R=Norma(OB->RealX-Mine->RealX,OB->RealY-Mine->RealY)>>4;
			addrand(i);
			addrand(R);
			if(R<2048){
				addrand(i);
				if(NN<512){
					PLIST[NN]=i;
					Ri[NN]=R;
					NN++;
				};
			};
		};
	};
	addrand(NN);
	UNISORT.CheckSize(NN);
	for(int i=0;i<NN;i++){
		UNISORT.Parms[i]=Ri[i];
		UNISORT.Uids[i]=PLIST[i];
		addrand(Ri[i]);
		addrand(PLIST[i]);
	};
	UNISORT.NUids=NN;
	UNISORT.Sort();
	for(int i=0;i<NN&&i<NP;i++){
		//Group[UNISORT.Uids[i]]->Serial+=35;
		OneObject* PS=Group[UNISORT.Uids[i]];
		if(PS){
			if(PS->BrigadeID!=0xFFFF){
				Brigade* BR=PS->Nat->CITY->Brigs+PS->BrigadeID;
				Brigade* BRD=&PS->Nat->CITY->InMines;
				if(BR!=BRD){
					BR->RemoveOne(PS->BrIndex,BRD);
				};
			};
			PS->DoNotCall=1;
			PS->GoToMine(Mine->Index,128+127);
		};
	};
};
//==================================================================================================================//
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverBuildingsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
bool CBFindBild(OneObject* OB,void* param)
{
	if(OB&&OB->NewBuilding&&OB->Stage<OB->NStages)
	{
		int* P=(int*)param;
		OneObject* W=(OneObject*)(P[0]);
		if(W->NNUM==OB->NNUM)
		{
			int xx=P[2]/16;
			int yy=P[3]/16;
			if(OB->FindPoint(&xx,&yy,FP_NEAREST_POINT|FP_FIND_WORKPOINT|FP_UNLOCKED_POINT))
			{
				int ds=Norma(P[2]-xx*16,P[3]-yy*16);
				if(ds<P[1])
				{
					P[1]=ds;
					P[4]=(int)(OB);
					P[5]=xx;
					P[6]=yy;
					return true;
				}
			}
		}
	}
	return false;
}
bool FindNearestBildPlace(OneObject* Worker)
{
	if(Worker&&Worker->LocalOrder&&Worker->LocalOrder->info.BuildObj.ObjIndex!=0xFFFF)
	{
		OneObject* bld = Group[Worker->LocalOrder->info.BuildObj.ObjIndex];
		if(bld)
		{
			OneObject* GetNewObjectToBuild(OneObject* PrevObject);
			if(!bld->newMons->SelfTransform)
				return false;
			
			if(CheckPt(Worker->LocalOrder->info.BuildObj.ObjX,Worker->LocalOrder->info.BuildObj.ObjY))
			{
				int dst=Norma(Worker->x-Worker->LocalOrder->info.BuildObj.ObjX,Worker->y-Worker->LocalOrder->info.BuildObj.ObjY);
				if(dst<3)
					return true;
			}
			if(GetNewObjectToBuild(bld))
			{
				// tipa stena
				int x=Worker->RealX/16;
				int y=Worker->RealY/16;
				int P[8];
				
				P[0]=(int)Worker;
				P[1]=999999;
				P[2]=x;
				P[3]=y;
				if(PerformActionOverBuildingsInRadius(x,y,700,&CBFindBild,P))
				{
					OneObject* Building=(OneObject*)P[4];
					Worker->LocalOrder->info.BuildObj.ObjIndex=Building->Index;
					Worker->LocalOrder->info.BuildObj.ObjX=P[5];
					Worker->LocalOrder->info.BuildObj.ObjY=P[6];
					Worker->LocalOrder->info.BuildObj.SN=Building->Serial;
					return true;
				}
			}
			else
			{
				int ObjX=Worker->LocalOrder->info.BuildObj.ObjX;
				int ObjY=Worker->LocalOrder->info.BuildObj.ObjY;
				if(bld->FindPoint(&ObjX,&ObjY,FP_NEAREST_POINT|FP_FIND_WORKPOINT|FP_UNLOCKED_POINT)){
					Worker->LocalOrder->info.BuildObj.ObjX=ObjX;
					Worker->LocalOrder->info.BuildObj.ObjY=ObjY;
					return true;
				}
			}
		}
	}
	return false;
}
//==================================================================================================================//
void GoToMineLink(OneObject* OB);
void BuildObjLink(OneObject* OBJ){
	addrand(OBJ->Index);
	if(OBJ->NewBuilding){
		OBJ->DeleteLastOrder();
		return;
	};
	OBJ->UnitSpeed=64;
	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;	
	NewMonster* NM=OBJ->newMons;
	word OID=OBJ->LocalOrder->info.BuildObj.ObjIndex;
	int ObjX=OBJ->LocalOrder->info.BuildObj.ObjX;
	int ObjY=OBJ->LocalOrder->info.BuildObj.ObjY;
	word OSN=OBJ->LocalOrder->info.BuildObj.SN;
	OBJ->UnBlockUnit();
	OneObject* OB=Group[OID];
	//OBJ->Important=true;
	Order1* Or1;
	if((!OB)||OB->Sdoxlo||OB->Life==OB->MaxLife||OSN!=OB->Serial||(OB->LocalOrder&&OB->LocalOrder->DoLink==&UnConstructBuilduingLink)){
		if(OB&&OB->NNUM==MyNation&&OB->Life==OB->MaxLife)AddUnlimitedEffect(OB->RealX>>4,(OB->RealY>>5)-OB->RZ,OB->newMons->BornSoundID);
		if(OBJ->LocalOrder){
			OBJ->DeleteLastOrder();
			OBJ->DeletePath();
			if(OB&&OB->Life==OB->MaxLife){
				if(OB->newMons->Usage==MineID){
					int N=OB->NInside;
					int MID=OB->Index;
					for(int i=0;i<MAXOBJECT;i++){
						OneObject* MOB=Group[i];
						if(MOB&&MOB->LocalOrder&&MOB->LocalOrder->DoLink==&GoToMineLink&&MOB->LocalOrder->info.BuildObj.ObjIndex==MID)N++;
					};
					if(N<OB->AddInside+OB->MoreCharacter->MaxInside)OBJ->GoToMine(OB->Index,128+16,1);
				}else
				if(OB->newMons->MineDamage){
                    OBJ->NewMonsterSendTo(OBJ->RealX+(rando()&8191)-4096,OBJ->RealY+(rando()&8191)-4096,128+16,0);
				}
			}

			if( OB && !OB->Sdoxlo && OSN==OB->Serial && OB->Stage>=OB->MoreCharacter->ProduceStages && OB->newMons->AutoBornSprite!=-1 ){
                int rt=TREES.Objects[OB->newMons->AutoBornSprite]->ResType;
				OBJ->TakeResource(OBJ->RealX>>4,OBJ->RealY>>4,rt,16+128,2);
			}

			return;
		};
	};

	int dst=Norma(OBJ->x-ObjX,OBJ->y-ObjY);
	if(dst<7)
	{
		if(CheckPt(ObjX,ObjY))
		{
			if(FindNearestBildPlace(OBJ))
			{
				if(ObjX!=OBJ->LocalOrder->info.BuildObj.ObjX&&
					ObjY!=OBJ->LocalOrder->info.BuildObj.ObjY)
				{
					ObjX=OBJ->LocalOrder->info.BuildObj.ObjX;
					ObjY=OBJ->LocalOrder->info.BuildObj.ObjY;
					OBJ->DeletePath();
					OBJ->CreatePath(ObjX,ObjY)
				}
				return;
			}
			else
			{
				OBJ->DeleteLastOrder();
				OBJ->DeletePath();
				return;
			}
			/*
			if(OB->FindPoint(&ObjX,&ObjY,FP_NEAREST_POINT|FP_FIND_WORKPOINT|FP_UNLOCKED_POINT)){
				OBJ->LocalOrder->info.BuildObj.ObjX=ObjX;
				OBJ->LocalOrder->info.BuildObj.ObjY=ObjY;
				return;
			}else{
				OBJ->DeleteLastOrder();
				OBJ->DeletePath();
				return;
			};
			*/
		}
	};
	if(dst<=2){
		OBJ->DestX=-1;
		OBJ->DeletePath();
		/*if(dst==1&&OBJ->NothingTime<5){
			OBJ->DestX=(ObjX<<8);
			OBJ->DestY=(ObjY<<8);
		}else{;*/
			//we can build now
			char dir=char(GetDir(OB->RealX-OBJ->RealX,OB->RealY-OBJ->RealY));
			char ddir=dir-OBJ->RealDir;
			if(abs(ddir)<32){
				if(OBJ->FrameFinished || OBJ->NewAnm->CanBeBrokenAfterFrame<OBJ->NewCurSprite){
					OBJ->RealDir=dir;
					OBJ->GraphDir=OBJ->RealDir;
					OBJ->NewAnm=NM->GetAnimation(anm_Work);
					OBJ->SetZeroFrame();
					OBJ->InMotion=false;
					OBJ->BackMotion=false;
					OBJ->WeakBlockUnit();
					OB->NextStage();
					if(NATIONS[OB->NNUM].AI_Enabled)OB->NextStage();
					//
					// смотри немного выше
					//
					//if(OB->Stage>=OB->MoreCharacter->ProduceStages && OB->newMons->AutoBornSprite!=-1){
                    //    int rt=TREES.Objects[OB->newMons->AutoBornSprite]->ResType;
					//	OBJ->TakeResource(OBJ->RealX>>4,OBJ->RealY>>4,rt,16+128,2);
					//}
				}
			}else{
				if(ddir>0)RotateMon(OBJ,16);
				else RotateMon(OBJ,-16);
				TryToStand(OBJ,false);
			};
		//};
	}else{
		//if(CheckPt(ObjX,ObjY)){
		//	ObjX=OBJ->x;
		//	ObjY=OBJ->y;
			/*
			if(OB->FindPoint(&ObjX,&ObjY,FP_NEAREST_POINT|FP_FIND_WORKPOINT|FP_UNLOCKED_POINT)){
				OBJ->LocalOrder->info.BuildObj.ObjX=ObjX;
				OBJ->LocalOrder->info.BuildObj.ObjY=ObjY;
			}*/
		if(OBJ->DestX!=-1)
		{
			if(dst>1&&FindNearestBildPlace(OBJ))
			{
				if(ObjX!=OBJ->LocalOrder->info.BuildObj.ObjX&&
					ObjY!=OBJ->LocalOrder->info.BuildObj.ObjY)
				{	
					ObjX=OBJ->LocalOrder->info.BuildObj.ObjX;
					ObjY=OBJ->LocalOrder->info.BuildObj.ObjY;
					
					//OBJ->DeletePath();
					OBJ->CreatePath(ObjX,ObjY);
				}
			}
			else
			{
				//OBJ->DeleteLastOrder();
				//OBJ->DeletePath();
				if(OBJ->NothingTime>4)
					OBJ->NewMonsterSmartSendTo(OBJ->LocalOrder->info.BuildObj.ObjX<<4,
						OBJ->LocalOrder->info.BuildObj.ObjY<<4,0,0,128+16,1);
				return;
			}
			/*
			else
			{
				Or1=OBJ->LocalOrder->NextOrder;
				OBJ->FreeOrdBlock(OBJ->LocalOrder);
				OBJ->LocalOrder=Or1;
				//OBJ->Important=false;
				TryToStand(OBJ,true);
				return;
			};
			*/
		}
		//};
		if(OBJ->NothingTime>4){
			if(OBJ->CheckBlocking())
				OBJ->DeleteBlocking();
			OBJ->CreatePath(ObjX,ObjY);
		}else OBJ->CreatePath(ObjX,ObjY);
	};
	/*
	int nnd=Norma(OB->RealX/16-ObjX*16,OB->RealY/16-ObjY*16);
	if(dst>1&&OBJ->DestX==-1&&OBJ->NothingTime>4)
	{
		if(!CheckPt(ObjX,ObjY))
		{
			OBJ->NewMonsterSmartSendTo(ObjX<<4,ObjY<<4,0,0,128+16,1);
			return;
		}
	}
	*/
};
//x,y-coordinates of point on the 2D plane (unit:pix)
//returnfs index of building,otherwise 0xFFFF
word DetermineBuilding(int x,int y,byte NMask){
	int xx=x>>4;
	int yy=y>>4;
	int xr=x<<4;
	int yr=y<<4;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding&&(OB->NMask&NMask)){
			NewMonster* NM=OB->newMons;
			int rr=NM->PicLx<<4;
			if(Norma(xr-OB->RealX,yr-OB->RealY)<rr){
				//precise checking
				int PicSX=NM->PicDx<<4;
				int PicSY=(NM->PicDy)<<5;
				int x0=(OB->RealX+PicSX)>>8;
				int y0=(OB->RealY+PicSY)>>8;
				char* px=NM->CheckX;
				char* py=NM->CheckY;
				int Npt=NM->NCheckPt;
				for(int j=0;j<Npt;j++)
					if(x0+px[j]==xx&&y0+py[j]==yy) return i;
			};
		};
	};
	return 0xFFFF;
};
bool OneObject::CheckBlocking(){
	return (CheckHLine(x-1,y-1,3)||CheckHLine(x-1,y+1,3)||
		CheckPt(x-1,y)||CheckPt(x+1,y));
};
void DeleteBlockLink(OneObject* OBJ);
void OneObject::DeleteBlocking(){
    return;
	Order1* OR1=GetOrdBlock();
	if(!OR1)return;//torba
	//PrioryLevel=0;
	DeletePath();
	OR1->OrderType=100;
	OR1->DoLink=&DeleteBlockLink;
	if(!CheckPt(x,y+2)){
		OR1->info.DelBlock.EndX=x;
		OR1->info.DelBlock.EndY=y+2;
		if(CheckPt(x,y+1)){
			OR1->info.DelBlock.LockX=x;
			OR1->info.DelBlock.LockY=y+1;
		}else{
			OR1->info.DelBlock.LockX=-1;
			OR1->info.DelBlock.LockY=-1;
		};
	}else
	if(!CheckPt(x,y-2)){
		OR1->info.DelBlock.EndX=x;
		OR1->info.DelBlock.EndY=y-2;
		if(CheckPt(x,y-1)){
			OR1->info.DelBlock.LockX=x;
			OR1->info.DelBlock.LockY=y-1;
		}else{
			OR1->info.DelBlock.LockX=-1;
			OR1->info.DelBlock.LockY=-1;
		};
	}else
	if(!CheckPt(x-2,y)){
		OR1->info.DelBlock.EndX=x-2;
		OR1->info.DelBlock.EndY=y;
		if(CheckPt(x-1,y)){
			OR1->info.DelBlock.LockX=x-1;
			OR1->info.DelBlock.LockY=y;
		}else{
			OR1->info.DelBlock.LockX=-1;
			OR1->info.DelBlock.LockY=-1;
		};
	}else
	if(!CheckPt(x+2,y)){
		OR1->info.DelBlock.EndX=x+2;
		OR1->info.DelBlock.EndY=y;
		if(CheckPt(x+1,y)){
			OR1->info.DelBlock.LockX=x+1;
			OR1->info.DelBlock.LockY=y;
		}else{
			OR1->info.DelBlock.LockX=-1;
			OR1->info.DelBlock.LockY=-1;
		};
	};
	NothingTime=0;
	OR1->NextOrder=LocalOrder;
	LocalOrder=OR1;
};
void DeleteBlockLink(OneObject* OBJ){
	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;
	if(OBJ->NewCurSprite<OBJ->NewAnm->NFrames-FrmDec)return;
	int lox=OBJ->LocalOrder->info.DelBlock.LockX;
	int loy=OBJ->LocalOrder->info.DelBlock.LockY;
	int enx=OBJ->LocalOrder->info.DelBlock.EndX;
	int eny=OBJ->LocalOrder->info.DelBlock.EndY;
	if(lox>0)BClrPt(lox,loy);
	BClrPt(enx,eny);
	OBJ->DestX=enx<<8;
	OBJ->DestY=eny<<8;
	addrand(OBJ->DestX);
	addrand(OBJ->DestY);
	int RATE=16;
	if(OBJ->NewState){
		RATE=OBJ->newMons->Rate[OBJ->NewState-1];
	};
	int RR=0;//((OBJ->newMons->OneStepDX[0]*OBJ->NewAnm->NFrames*RATE)>>5)+8;
	if(OBJ->NothingTime>9||Norma(OBJ->RealX-OBJ->DestX,OBJ->RealY-OBJ->DestY)<RR){
		Order1* OR1=OBJ->LocalOrder;
		OBJ->LocalOrder=OR1->NextOrder;
		OBJ->FreeOrdBlock(OR1);
		OBJ->NothingTime=0;
		OBJ->DeletePath();
	};
};
//-----------Inverse references from (x,y)->Index----------//
/*word MonInd[MAPSX*MAPSX];//512k
#define MAXMI (MAPSX*MAPSY)
inline word GetMI(int x,int y){
	__asm{
		mov		ebx,y
		shl		ebx,9
		add		ebx,x
		cmp		ebx,0
		jl		nona
		cmp		ebx,MAXMI
		jge		nona
		shl		ebx,1
		mov		ax,[eax]
		jmp		done
nona:	mov		ax,0xFFFF
done:
	};
	return;
};
inline void SetMI(int x,int y,word MI){
	__asm{
		mov		eax,y
		shl		eax,9
		add		eax,x
		cmp		eax,0
		jl		nona
		cmp		eax,MAXMI
		jge		nona
		shl		eax,1
		mov		bx,MI
		mov		[eax],bx
nona:
	};
	return;
};*/
int ExplMedia;
bool DamageInCell(int cell,int x,int y,int r,word Damage,OneObject* Sender,word Attr){
	if(cell<0||cell>=VAL_MAXCX*VAL_MAXCX)return 0;
	int NMon=MCount[cell];
	if(!NMon)return 0;
	int ofs1=cell<<SHFCELL;
	word MID;
	bool DAM=0;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				NewMonster* NM=OB->newMons;
				int rr=int(NM->EMediaRadius+NM->ExplRadius)<<4;
				int dr=Norma(OB->GetAttX()-x,OB->GetAttY()-y);
				if(OB&&dr<r+rr){
					//Unit is in danger !
					if(dr<rr)ExplMedia=NM->ExplosionMedia;
#ifdef COSSACKS2
					if(!(Sender&&Sender->NMask&OB->NMask&&Norma(Sender->RealX-OB->RealX,Sender->RealY-OB->RealY)<int(Sender->newMons->FreeShotDist)*16&&Sender->newMons->FriendlyFire))
#endif //COSSACKS2
					{
						OB->MakeDamage(0,Damage,Sender,byte(Attr));
						DAM=1;
                        return 1;
					};
				};
			};
		};
	};
	return DAM;
};
bool DamageInCell2(int cell,int x,int y,int r,word Damage,OneObject* Sender,word Attr){
	if(cell<0||cell>=VAL_MAXCX*VAL_MAXCX)return 0;
	int NMon=MCount[cell];
	if(!NMon)return 0;
	int ofs1=cell<<SHFCELL;
	word MID;
	bool DAM=0;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				NewMonster* NM=OB->newMons;
				int rr=int(NM->EMediaRadius+NM->ExplRadius)<<4;
				int dr=Norma(OB->RealX-x,OB->RealY-y);
				if(OB&&dr<r+rr){
					//Unit is in danger !
					if(dr<rr)ExplMedia=NM->ExplosionMedia;
					if(!(Sender&&Sender->NMask&OB->NMask&&Norma(Sender->RealX-OB->GetAttX(),Sender->RealY-OB->GetAttY())<int(Sender->newMons->FreeShotDist)*16&&Sender->newMons->FriendlyFire))
					{
						OB->MakeDamage(0,Damage,Sender,byte(Attr));
						DAM=1;						
					};
				};
			};
		};
	};
	return DAM;
};
bool MakeRoundDamage(int x,int y,int r,word Damage,OneObject* Sender,word Attr,bool Splash){
	int cx=(x>>11);
	int cy=(y>>11);
	int cell=cx+(cy<<VAL_SHFCX)+VAL_MAXCX+1;
	ExplMedia=-1;
	if(Splash){
		bool R=DamageInCell2(cell-VAL_MAXCX-1,x,y,r,Damage,Sender,Attr)|
			DamageInCell2(cell-VAL_MAXCX  ,x,y,r,Damage,Sender,Attr)|
			DamageInCell2(cell-VAL_MAXCX+1,x,y,r,Damage,Sender,Attr)|
			DamageInCell2(cell-1      ,x,y,r,Damage,Sender,Attr)|
			DamageInCell2(cell        ,x,y,r,Damage,Sender,Attr)|
			DamageInCell2(cell+1      ,x,y,r,Damage,Sender,Attr)|
			DamageInCell2(cell+VAL_MAXCX-1,x,y,r,Damage,Sender,Attr)|
			DamageInCell2(cell+VAL_MAXCX  ,x,y,r,Damage,Sender,Attr)|
			DamageInCell2(cell+VAL_MAXCX+1,x,y,r,Damage,Sender,Attr);        
		return R;
	}else{
		return 
		DamageInCell(cell-VAL_MAXCX-1,x,y,r,Damage,Sender,Attr)||
		DamageInCell(cell-VAL_MAXCX  ,x,y,r,Damage,Sender,Attr)||
		DamageInCell(cell-VAL_MAXCX+1,x,y,r,Damage,Sender,Attr)||
		DamageInCell(cell-1      ,x,y,r,Damage,Sender,Attr)||
		DamageInCell(cell        ,x,y,r,Damage,Sender,Attr)||
		DamageInCell(cell+1      ,x,y,r,Damage,Sender,Attr)||
		DamageInCell(cell+VAL_MAXCX-1,x,y,r,Damage,Sender,Attr)||
		DamageInCell(cell+VAL_MAXCX  ,x,y,r,Damage,Sender,Attr)||
		DamageInCell(cell+VAL_MAXCX+1,x,y,r,Damage,Sender,Attr);
	}
};
bool OneObject::GetDamagePoint(int xo,int yo,Coor3D* dp,int Precise){
	NewMonster* NM=newMons;
	if(NM->NBars){
		int bx0=(RealX>>4)+NM->PicDx;
		int by0=(RealY>>4)+(NM->PicDy<<1);
		int N=(int(rando())*int(NM->NBars))>>15;
		int pp=N*5;
		int XB0=NM->Bars3D[pp]+bx0;
		int YB0=(NM->Bars3D[pp+1]<<1)+by0;
		int L1=NM->Bars3D[pp+2];
		int L2=NM->Bars3D[pp+3];
		dp->x=XB0+((L1+L2)>>1);
		dp->y=YB0+((L1-L2)>>1);
		dp->z=GetHeight(dp->x,dp->y)+(NM->Bars3D[pp+4]>>1);
	}else{
		dp->x=GetAttX()>>4;
		dp->y=GetAttY()>>4;
		//dp->z=RZ+32;
		dp->z=RZ+OverEarth+NM->DstZPoint;
	};
	x+=(rando()*Precise)>>15;
	y+=(rando()*Precise)>>15;
	return NewBuilding;
};

//Type:
//0 - single order (previous orders will be erased)
//1 - add order to the head of link
//2 - add order to the tile of link
void CheckAllLinks();
void CheckAllLinks(OneObject*);
bool CheckIfNowAttack(OneObject* OB){	
	if(OB->delay)return false;
	if(OB&&OB->LocalOrder&&OB->LocalOrder->DoLink==&AttackObjLink&&OB->NewAnm->Code>=anm_Attack&&OB->NewAnm->Code<anm_Attack+100){
		if(OB->NewCurSprite>OB->NewAnm->ActiveFrame)return false;
        word& w=OB->LocalOrder->info.BuildObj.SN;
        if(w!=1)w=1;
		return true;
	}
	return false;
}
Order1* OneObject::CreateOrder(byte Type){
	addrand(Type+1);
	
	//if(ActiveAbility){
	//	bool Insub=false;
	//	ActiveAbility->ActiveAbilities.modifyInsubordination(false,Insub);
	//	if(Insub){
	//		addrand(Type+1);
	//		return NULL;
	//	}
	//}

	//checking deepness of orders

	Order1* Or1=LocalOrder;
	int n=0;
	while(Or1 && n<500){
		Or1=Or1->NextOrder;
		n++;
	}
	if(n==500){
		Or1=LocalOrder;
		n=0;
        int no[128];
		memset(no,0,sizeof no);
		while(Or1 && n<500){
			int GetOrderKind(ReportFn* RF);
			int t=GetOrderKind(Or1->DoLink);
			if(t>0)no[t]++;
			Or1=Or1->NextOrder;
			n++;
		}
		int nn=0;
		for(int i=0;i<128;i++){
			if(no[i]>nn){
				nn=no[i];
				n=i;
			}
		}
		char* GetOrdName(int idx);
		Log.Error("Too many orders: %s (%d)",GetOrdName(n),nn);
		return NULL;
	}

	void PushUnitBackLink(OneObject* OBJ);
	if(LocalOrder&&LocalOrder->DoLink==&PushUnitBackLink&&PrioryLevel==255){
		addrand(Type+1);
		return NULL;
	}
	BackMotion=0;
	if((UnlimitedMotion||Disabled)&&Type<128){
		addrand(Type+1);
		return NULL;
	}
	addrand(Type+71);
	Illusion=0;
	Type&=127;
	Order1* OR1=GetOrdBlock();
	Order1* OR2;
	if(!OR1){
		addrand(Type+1);
		return NULL;
	}
	switch(Type){	
	case 1:
		OR1->NextOrder=LocalOrder;
		LocalOrder=OR1;
		break;
	case 2:
		OR1->NextOrder=NULL;
		if(LocalOrder){
			OR2=LocalOrder;
			while(OR2->NextOrder)OR2=OR2->NextOrder;
			OR2->NextOrder=OR1;
		}else LocalOrder=OR1;
		break;
	case 7:
		{
			bool A=CheckIfNowAttack(this);
			word ID=EnemyID;
			word SN=EnemySN;
			bool AT=Attack;
			Order1* O1=LocalOrder;			
			if(A&&O1)LocalOrder=O1->NextOrder;
			ClearOrders();
			LocalOrder=OR1;
			OR1->NextOrder=NULL;
			if(A){
				O1->NextOrder=OR1;
				O1->info.BuildObj.SN=1;
				LocalOrder=O1;
				EnemyID=ID;
				EnemySN=SN;
				Attack=AT;
			}
		}
		break;
	case 3:
	default:
		{			
			ClearOrders();
			LocalOrder=OR1;
			OR1->NextOrder=NULL;			
		};
	}
	addrand(Type+73);	
	return OR1;
};
void NewMonsterSmartSendToLink(OneObject* OBJ);
int GetOrderKind(ReportFn* RF);
#undef DeleteLastOrder
void NewAttackPointLink(OneObject* OBJ);
void MakeOneShotLink(OneObject* OBJ);
void PanicUnitLink(OneObject* OBJ);
extern bool InClOrders;
void CheckOrders(OneObject* OB){
	Order1* ORR=OB->LocalOrder;
	while(ORR){
		if(!ORR->DoLink){
			int x=0;
		}
		assert(ORR->DoLink);
		ORR=ORR->NextOrder;
	}
}
void CheckAllOrders(){
	for(int i=0;i<MAXOBJECT;i++)if(Group[i]){
        CheckOrders(Group[i]);
	}
}
bool CheckShooterAbilityToRecharge(OneObject* OBJ){
	if(!OBJ->MaxDelay)return true;
	if(OBJ->delay==OBJ->MaxDelay){
		NewMonster* NMN=OBJ->newMons;
		bool AllowShot=1;
		for(int k=0;k<NMN->NShotRes;k++){
			if(XRESRC(OBJ->NNUM,NMN->ShotRes[k+k])<NMN->ShotRes[k+k+1])AllowShot=false;
		};
		if(AllowShot){
			for(int k=0;k<NMN->NShotRes;k++){
				AddXRESRC(OBJ->NNUM,NMN->ShotRes[k+k],-NMN->ShotRes[k+k+1]);
				OBJ->Nat->AddResource(byte(NMN->ShotRes[k+k]),-NMN->ShotRes[k+k+1]);
				OBJ->Nat->ResOnLife[NMN->ShotRes[k+k]]+=NMN->ShotRes[k+k+1];					
			}
			OBJ->delay=OBJ->MaxDelay-1;
			return true;
		}		
		return false;
	}else return true;
}
bool AllowShot=true;
void CheckAllLinks(OneObject* OB){
	Order1* OR1=OB->LocalOrder;
	while(OR1){
		assert(int(OR1)==0||DWORD(OR1)>100000);
		OR1=OR1->NextOrder;
	};
}
void CheckAllLinks(){	
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB){
			CheckAllLinks(OB);
		}
	}
}
void OneObject::DeleteLastOrder(){
	addrand(345);
	Order1* OR1=LocalOrder;
	if(!OR1)return;
	addrand(346);
	OneObject* bld=NULL;
	if(LocalOrder->DoLink==&BuildObjLink){
		word ID=LocalOrder->info.BuildObj.ObjIndex;
		bld=Group[ID];
	};
	void RotUnitLink(OneObject* OB);
	if(LocalOrder->DoLink==&RotUnitLink){
        BackMotion=0;
	}
	BackMotion=0;
	addrand(56);
	//check integrity 
	BackMotion=0;
	///DeletePath();
	DestX=-1;
	if(OR1->DoLink==&PanicUnitLink){
		UnlimitedMotion=0;
	};	
	bool NewEnemy=0;
	if(OR1->DoLink==&AttackObjLink||OR1->DoLink==&WaterAttackLink){
		NewEnemy=1;
		if(EnemyID!=0xFFFF){
			OneObject* EOB=Group[EnemyID];
			if(EOB){
                ATTLIST.Del(this,EOB);
			}			
		};
		Attack=false;
		if(newMons->SlowRecharge){
			int ns=NewState;
			if(ns&&WeaponFlags[newMons->WeaponKind[ns-1]]&1&&NewAnm->Code==anm_Attack+ns-1){//to optimise
				if(EnemyID!=0xFFFF){
					OneObject* EOB=Group[EnemyID];
					if(EOB&&EOB->Sdoxlo>8){
						MakeOneShot(this,EOB->RealX>>4,EOB->RealY>>4,EOB->RZ+40);
					};
				};
				if(LocalOrder&&LocalOrder->DoLink!=&MakeOneShotLink){
					if(CheckShooterAbilityToRecharge(this)){						
						if(!newMons->DontTransformToChargeState){							
							NewState=4;
							LocalNewState=4;
						}else{
							if(delay){
								NewState=4;
								TryToStand(this,0);
							}
						}
						StandTime=20;
					}
				}				
			}
		}
		EnemyID=0xFFFF;
		EnemySN=0xFFFF;
	};
	assert(int(LocalOrder)==0||DWORD(LocalOrder)>100000);
	if(OR1->DoLink==NewAttackPointLink){
		DstX=-1;
		DstY=-1;
	};
	if(LocalOrder){
		//addrand(GetOrderKind(LocalOrder->DoLink));
	};
	assert(int(LocalOrder)==0||DWORD(LocalOrder)>100000);
	LocalOrder=OR1->NextOrder;
	assert(int(LocalOrder)==0||DWORD(LocalOrder)>100000);
	FreeOrdBlock(OR1);

	//if(LocalOrder){
	//	PrioryLevel=LocalOrder->PrioryLevel;
	//}else{
	//	PrioryLevel=0;
	//}

	//search for patrol commands
	bool Patr=false;
	bool Atta=false;
	OR1=LocalOrder;
	assert(int(LocalOrder)==0||DWORD(LocalOrder)>100000);
	while(OR1){
		if(OR1->DoLink==&AttackObjLink)Atta=true;
		else
		if(OR1->DoLink==&NewMonsterSmartSendToLink||OR1->DoLink==NewMonsterSendToLink&&
		   OR1->PrioryLevel<5)Patr=true;
		OR1=OR1->NextOrder;
	};
	assert(int(LocalOrder)==0||DWORD(LocalOrder)>100000);
	Attack=Atta;
	InPatrol=Patr;
	UnBlockUnit();
#ifdef COSSACKS2
	//if((!InClOrders)&&LocalOrder&&LocalOrder->DoLink&&LocalOrder->OrderType==12)LocalOrder->DoLink(this);
	int GetOrderKind(ReportFn* RF);	
#endif //COSSACKS2
	extern bool InCloseObjects;	
	extern bool InClOrders;
	if(bld&&!(InCloseObjects||InClOrders||LocalOrder)&&bld->newMons->SelfTransform){
        OneObject* GetNewObjectToBuild(OneObject* PrevObject);
		OneObject* OB=GetNewObjectToBuild(bld);
		if(OB)BuildObj(OB->Index,128+16,0,0);
	}
};
//#define DeleteLastOrder() DeleteLastOrder();addrand(0)
OneObject* SearchEnemyInCell(int x0,int y0,int cell,byte nmask,byte mmask,byte Priest,word Brig){
	addrand(x0);
	addrand(y0);
	addrand(cell);
	addrand(Brig);
	addrand(Priest);
	addrand(nmask);
	addrand(mmask);
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	OneObject* BOBJ=NULL;	
	int MINS=10000;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		addrand(MID);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||(OB->AI_Guard&&!OB->Attack))){
				int NA=ATTLIST.GetNAttackers(OB);
				//if(NA<3){
					if(Priest==3){
						if(OB->newMons->Capture&&TestCapture(OB)==0){
							if(Brig==0xFFFF)return OB;
							else{
								if(OB->BrigadeID==Brig||Norma(OB->RealX-x0,OB->RealY-y0)<100*16)return OB;
							}
						}
					}else
					if(Priest==2&&(!(OB->NMask&nmask))&&OB->newMons->Capture&&TestCapture(OB)==0){
						if(Brig==0xFFFF)return OB;
						else{
							if(OB->BrigadeID==Brig||Norma(OB->RealX-x0,OB->RealY-y0)<100*16)return OB;
						}
					}else{
						if((!(OB->NMask&nmask))&&OB->MathMask&mmask){
 							if(Priest==1){
								if(OB->BrigadeID!=0xFFFF){
									Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
									if(BR->Morale<BR->MaxMorale)return OB;
								}
								if(OB->Life<OB->MaxLife)return OB;
							}else{
								if(Brig==0xFFFF){
									if(NA<MINS){
                                        BOBJ=OB;
										MINS=NA;
									}
								}else{
									if(OB->BrigadeID==Brig||Norma(OB->RealX-x0,OB->RealY-y0)<100*16){
										if(NA<MINS){
											BOBJ=OB;
											MINS=NA;
										}
									}
								}
							}
						}
					}
				//}
			}
		}
	}
    return BOBJ;
};
OneObject* SearchEnemyInCell3(int cell,byte nmask,byte mmask,byte Priest){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return NULL;
	int ofs1=cell<<SHFCELL;
	word mons[64];
	int NMONS=0;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				if((!(OB->NMask&nmask))&&OB->MathMask&mmask&&NMONS<64){
					mons[NMONS]=MID;
					NMONS++;
				};
			};
		};
	};
	if(NMONS){
		return Group[mons[(NMONS*int(rando()))>>15]];
	};
    return NULL;
};
OneObject* SearchEnemyGroupInCell(int cell,byte nmask,byte mmask,word Brig){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(NMon<3)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NMask&nmask&&OB->MathMask&mmask&&!OB->LockType==1){
				if(Brig==0xFFFF)return OB;
				else if(Brig==OB->BrigadeID)return OB;
			}
		};
	};
    return NULL;
};
bool CheckVisibility(int x1,int y1,int x2,int y2,word MyID);
void SearchEnemyForAIArtillery(OneObject* OB);
void AIArtilleryAgainstTowers(OneObject* OB);
void SearchEnemyForMortira(OneObject* OB);
void EraseBrigade(Brigade* BR);
int TestCapture(OneObject* OBJ);
void SearchVictimForAI_Archer(OneObject* OB){
	//int x0=OB->RealX>>11;
	//int y0=OB->RealY>>11;
	//NewMonster* NM=OB->newMons;
	////int rr=NM->VisRange;
	//int rr=OB->MoreCharacter->VisRange;
	//int rx1=(rr>>11)+1;
	//int rx2=rx1+rx1+1;
	//byte nmask=~OB->NMask;
	//OneObject* DestObj=NULL;
	//int stcell;
	//int NSR=6;
	//int RMIN=rr+100;
	//for(int i=0;i<6;i++){
	//	int dx=x0+((rando()*rx2)>>15)-rx1;
	//	int dy=y0+((rando()*rx2)>>15)-rx1;
	//	if(dx>=0&&dy>0&&dx<VAL_MAXCX-2&&dy<VAL_MAXCX-2){
	//		stcell=dx+(dy<<VAL_SHFCX);
	//		word BMID=BLDList[stcell+VAL_MAXCX+1];
	//		if(BMID!=0xFFFF){
	//			OneObject* BOB=Group[BMID];
	//			if(BOB&&(!BOB->Sdoxlo)&&BOB->NMask&nmask){
	//				int r=Norma(BOB->RealX-OB->RealX,BOB->RealY-OB->RealY)>>4;
	//				if(r<RMIN){
	//					RMIN=r;
	//					DestObj=BOB;
	//				};
	//			};
	//		}
	//	};
 //   };
	//if(DestObj)OB->AttackObj(DestObj->Index,16+128,0,0);
};
int GetUnitActivity(OneObject* OB);
bool CheckVictimInAngle(int x,int y,WeaponInSector* WS,byte Angle,byte Mask);
void B_BrigadeBattleLink(Brigade* BR);

bool CheckPossibilityToAttackEnemy(OneObject* OB,OneObject* EOB){
	if(EOB->Sdoxlo)return false;	
	if(OB->NMask & EOB->NMask)return false;
	if(!(EOB->MathMask & OB->newMons->KillMask))return false;
	if(!(EOB->MathMask & OB->newMons->AttackMask[0])&&OB->ArmAttack)return false;
	if(EOB->newMons->Immortal)return false;
	if(EOB->ActiveAbility){
		bool inv=false;
		EOB->ActiveAbility->ActiveAbilities.modifyInvulnerability(inv,inv);
		if(inv)return false;
		bool sv=true;
		inv=false;
		byte Det=0;
		EOB->ActiveAbility->ActiveAbilities.modifyInvisibility(inv,inv,Det,sv); // CheckPossibilityToAttackEnemy
		if(inv && !(OB->NMask&Det))return false;
	}
	return true;
}

bool CheckPossibilityToHealUnit(OneObject* OB,OneObject* EOB){
	if(EOB->Sdoxlo)return false;	
	if(!(OB->NMask & EOB->NMask))return false;
	if(!(EOB->MathMask & OB->newMons->AttackMask[0]))return false;
	if(EOB->newMons->Immortal)return false;
	if(EOB->ActiveAbility){
		bool inv=false;
		EOB->ActiveAbility->ActiveAbilities.modifyInvulnerability(inv,inv);
		if(inv)return false;
		bool sv=true;
		inv=false;
		byte Det=0;
		EOB->ActiveAbility->ActiveAbilities.modifyInvisibility(inv,inv,Det,sv); // CheckPossibilityToHealUnit
		if(inv && !(OB->NMask&Det))return false;
	}
	return true;
}
void OneObject::SearchVictim(){
	if(PeaceMode||AI_Guard||NoSearchVictim)return;

	int rr=MoreCharacter->VisRange;
	int RM=GetMaxAttackRadius()<<4;
	if(RM>rr)rr=RM;
	RM>>=4;

	OneObject* Victim=NULL;

	int minR=MoreCharacter->MinR_Attack;

	int NShots=0;

	if(ActiveAbility){
		ActiveAbility->ActiveAbilities.modifySearchVictim(Victim,rr>>4,NShots);
	}

	if(!Victim){
		if(newMons->Priest || (newMons->Shaman && rando()>8192)){
			itr_UnitsInRadius.Create(RealX>>4,RealY>>4,rr>>4);
			int R=10000;
			while(OneObject* OB=itr_UnitsInRadius.Next()){
				if(CheckPossibilityToHealUnit(this,OB)){				
					int r=Norma(OB->RealX-RealX,OB->RealY-RealY)>>4;
					if(r<R && r>minR && OB->Life<OB->MaxLife){
						Victim=OB;
						R=r;
					}                
				}            
			}
		}else{
			itr_UnitsInRadiusFromOtherNations.Create(NNUM,RealX>>4,RealY>>4,rr>>4);
			int R=10000;
			while(OneObject* OB=itr_UnitsInRadiusFromOtherNations.Next()){
				if(CheckPossibilityToAttackEnemy(this,OB)){
					if(OB->AI_Guard==false || OB->Attack){
						int r=Norma(OB->RealX-RealX,OB->RealY-RealY)>>4;
						if(r<minR)r+=4000;
						if(r<R){
                            extern word NPlayers;
                            if(NPlayers<=1 && NNUM==MyNation){
                                if( !OB->IsInFog ){
                                    Victim=OB;
                                    R=r;
                                }
                            }else{
							    Victim=OB;
							    R=r;
                            }
						}
					}
				}
			}
		}
	}

	if(Victim){
		AttackObj(Victim->Index,1,1,NShots);
		itr_UnitsInRadius.Create(RealX>>4,RealY>>4,256);
		while(OneObject* OB=itr_UnitsInRadius.Next()){
			if(OB->NMask & NMask)OB->Excited=true;
		}
	}

}
/*
void OneObject::SearchVictim(){
#ifdef SIMPLEMANAGE
	if(PeaceMode||AI_Guard||NoSearchVictim)return;
#else //SIMPLEMANAGE
	if(PeaceMode||NoSearchVictim||AI_Guard)return;
#endif //SIMPLEMANAGE
	if(ActivityState==1&&StandTime<2)return;
	int ac1=GetUnitActivity(this);
	if(ac1){
		if(newMons->Peasant||newMons->Usage==TowerID||newMons->Artilery||Media)return;
	};
	NewMonster* NM=newMons;
	if(NM->VisRange==0)
		return;
	//if(NM->Archer&&Nat->AI_Enabled){
	//	return;
	//};
	addrand(Index);
	int addr=0;
	if(BrigadeID!=0xFFFF){
		Brigade* BR=CITY[NNUM].Brigs+BrigadeID;
		addr=BR->AddAttackRange;
		if(newMons->ArmAttack){
			if(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_BITVA)//>BLink==&B_BrigadeBattleLink)return;
				return;
			if(!BR->AttEnm){
				//if(BR->BOrder)return;
				if(!RifleAttack)return;
				if(RifleAttack&&delay!=0)return;
			}
		}		
	}
	word OnlyBrig=0xFFFF;
	if(SearchOnlyThisBrigadeToKill!=0xFFFF&&BrigadeID!=0xFFFF){
		OnlyBrig=SearchOnlyThisBrigadeToKill;
	}
	if(NM->WSECT){
#ifdef COSSACKS2
		if(!RifleAttack)return;
#endif //COSSACKS2
		int R=NM->WSECT->RMax;
		//M->WSECT->RMax=NM->VisRange>>4;
		NM->WSECT->RMax=MoreCharacter->VisRange>>4;
		if((!LocalOrder)&&PrioryLevel<15&&LocalNewState!=0&&(!delay)&&NewAnm->NFrames-FrmDec<=NewCurSprite&&CheckVictimInAngle(RealX>>4,RealY>>4,NM->WSECT,RealDir,NMask)){
			NewAttackPoint((RealX>>4)+TCos[RealDir],(RealY>>4)+TSin[RealDir],128+16,0,1);
		};
		NM->WSECT->RMax=R;
		return;
	};
	int minR=int(MoreCharacter->MinR_Attack)<<4;
	int REALX=RealX;
	int REALY=RealY;
	//int rr=NM->VisRange;
	int rr=MoreCharacter->VisRange+(addr<<4);
	int RM=GetMaxAttackRadius()<<4;
	if(RM>rr)rr=RM;
	int VRS=NM->VisRangeShift;
#ifdef COSSACKS2
	if(BrigadeID!=0xFFFF&&newMons->ArmAttack){
		//rr+=RZ<<(1+4);
		if(StandGround&&!RifleAttack){
			rr=120*16;
			VRS=0;
		};
	};
#endif //COSSACKS2
	if(NM->VisRangeShift){
		REALX+=(int(TCos[RealDir])*VRS)>>8;
		REALY+=(int(TSin[RealDir])*VRS)>>8;

	};
	if(LockType==1&&Nat->AI_Enabled&&EnemyID==0xFFFF){
		if(delay)return;
		
		bool Kill=false;
		//search victim for AI Battleship
		int x0=REALX>>11;
		int y0=REALY>>11;
		int maxx=msx>>2;
		int DY=TopLx>>1;
		//int cell=((y0)<<VAL_SHFCX)+(x0>11);

		
		if(NM->Usage==GaleraID)rr=NM->AttackRadius2[1];
		int rx1=(rr>>11)+1;
		byte MMASK=NMask;
		byte nmask=~NMask;
		byte mmask=NM->KillMask;
		OneObject* DestObj=NULL;
		int mindist=10000000;
		int dist;
		int rx2=rx1+rx1+1;
		int stcell;
		for(int i=0;i<6;i++){
			int dx=x0+((rando()*rx2)>>15)-rx1;
			int dy=y0+((rando()*rx2)>>15)-rx1;
			if(dx>0&&dx<maxx&&dy>=0&&dy<maxx){
				stcell=dx+(dy<<VAL_SHFCX);
				byte BPT=NPresence[stcell];
				if((BPT&nmask)&&!((BPT&MMASK)||
					(NPresence[stcell-1]&MMASK)||(NPresence[stcell+1]&MMASK)||
					(NPresence[stcell-DY]&MMASK)||(NPresence[stcell+DY]&MMASK))){
					OneObject*OB=SearchEnemyGroupInCell(stcell,nmask,mmask,OnlyBrig);
					if(OB){
						dist=Norma(OB->RealX-REALX,OB->RealY-REALY);
						if(dist>minR&&dist<mindist){
							mindist=dist;
							DestObj=OB;
						};
					};
				};
			};
		};
		if(DestObj){
			if(LocalOrder)AttackObj(DestObj->Index,128+16,1,1);
			else AttackObj(DestObj->Index,128+16,1,4);
		};
		return;
	};
	addname(Index);
	//if(Zombi){
	//	if(newMons->Usage==PushkaID){
	//		SearchEnemyForAIArtillery(this);
	//		return;
	//	};
	//	if(newMons->Usage==MortiraID){
	//		SearchEnemyForMortira(this);
	//		return;
	//	};
	//	if(newMons->Usage==SupMortID&&GNFO.EINF[NNUM]&&!delay){
	//		int BX,BY;
	//		word MID=GNFO.EINF[NNUM]->SearchBestEnemyAndPlaceForSupermortira(this,&BX,&BY);
	//		if(MID!=0xFFFF){
	//			if(BrigadeID!=0xFFFF){
	//				Nat->CITY->UnRegisterNewUnit(this);
	//				Zombi=true;
	//			};
	//			if(BX!=REALX){
	//				NewMonsterSmartSendTo(BX>>4,BY>>4,0,0,128+64,0);
	//				delay=16;
	//				MaxDelay=16;
	//			}else{
	//				OneObject* OB=Group[MID];
	//				if(OB)AttackPoint(OB->RealX>>4,OB->RealY>>4,OB->RZ+32,1,ATTP_IMMEDIATECHECK,0,128+64);
	//			};
	//		}else{
	//			//need to connect to nearest brigade
	//			if(BrigadeID==0xFFFF){
	//				//search for the nearest army
	//				AI_Army* BestArmy=NULL;
	//				int MinTopDist=1000000;
	//				int BestFinalTop=0xFFFF;
	//				int MyTop=GetTopology(REALX>>4,REALY>>4);
	//				if(MyTop<0xFFFE){
	//					int TOPSTART=MyTop*GetNAreas();
	//					AI_Army* ARM=Nat->CITY->ARMS;
	//					for(int i=0;i<MaxArm;i++){
	//						if(ARM->Enabled&&(ARM->Spec==0||ARM->Spec==1)){		
	//							int top=ARM->TopPos;
	//							if(top<0xFFFE){
	//								int topdst=GetLinksDist(top+TOPSTART);
	//								if(top==MyTop)topdst=0;
	//								if(topdst<MinTopDist){
	//									MinTopDist=topdst;
	//									BestFinalTop=top;
	//									BestArmy=ARM;
	//								};
	//							};
	//						};
	//						ARM++;
	//					};
	//					if(BestFinalTop!=0xFFFF){
	//						int NextTop=GetMotionLinks(TOPSTART+BestFinalTop,LockType,NNUM);
	//						if(NextTop==BestFinalTop||MinTopDist==0){
	//							//we can connect now!
	//							City* CT=Nat->CITY;
	//							int BID=CT->GetFreeBrigade();
	//							if(BID!=-1){
	//								Brigade* BR=CT->Brigs+BID;
	//								BR->Enabled=true;
	//								BR->AddObject(this);
	//								BestArmy->AddBrigade(BR);
	//								BR->DeleteAll();
	//								EraseBrigade(BR);
	//								BestArmy->Parad();
	//							};
	//						}else{
	//							//move to next topology zone
	//							if(NextTop<0xFFFE){
	//								Area* AR=GetTopMap(NextTop);
	//								NewMonsterSendTo(int(AR->x)<<10,int(AR->y)<<10,128+16,0);
	//							};
	//						};
	//					};
	//				};
	//				delay=50;
	//				MaxDelay=50;
	//			};
	//		};
	//		//AIArtilleryAgainstTowers(this);
	//		return;
	//	};
	//	int cell=((REALY>>11)<<VAL_SHFCX)+(REALX>>11);
	//    NewMonster* NM=newMons;
	//    //int rr=NM->VisRange;
	//	int rr=MoreCharacter->VisRange;
	//	int rx1=6;
	//	byte nmask=~NMask;
	//    byte mmask=NM->KillMask;
	//	OneObject* DestObj=NULL;
	//	int mindist=10000000;
	//	int dist;
	//	int rx2=rx1+rx1+1;
	//	int stcell;

	//	for(int i=0;i<6;i++){
	//		int dx=((rando()*rx2)>>15)-rx1;
	//		int dy=((rando()*rx2)>>15)-rx1;
	//		stcell=cell+dx+(dy<<VAL_SHFCX);
	//		if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
	//			if(NPresence[stcell]&nmask){
	//				OneObject*OB=SearchEnemyInCell(RealX,RealY,stcell,~nmask,mmask,3,OnlyBrig);
	//				if(OB){
	//					int NA=ATTLIST.GetNAttackers(OB);
	//					dist=Norma(OB->RealX-REALX,OB->RealY-REALY);
	//					int dist1=dist+NA*140*16;
	//					if(dist1<mindist&&dist>minR){
	//						mindist=dist1;
	//						DestObj=OB;
	//					};
	//				};
	//			};
	//		};
 //       };
	//	if(DestObj){
	//		int t0=GetTopology(REALX>>4,REALY>>4);
	//		if(t0>=0xFFFE)return;
	//		int t1=GetTopology(DestObj->RealX>>4,DestObj->RealY>>4);
	//		if(t0==t1||GetLinksDist(t1*GetNAreas()+t0)<40)AttackObj(DestObj->Index,1);
	//	};
	//	return;
	//};
	
    int cell=((REALY>>11)<<VAL_SHFCX)+(REALX>>11);
    
    int rx1=(rr>>11)+1;
    byte nmask=~NMask;
	byte Priest=NM->Priest;
	if(NM->Shaman){
		if(rando()<4096){
			Priest=1;
		};
	};
	if(Priest){
		nmask=NMask;
		Priest=1;
	}else{
		if(!(NM->Capture||LockType==1||NewBuilding))Priest=2;
	};
    byte mmask=NM->KillMask;
    OneObject* DestObj=NULL;
    int mindist=10000000;
    int dist;
#ifdef CONQUEST
	if(TempDelay&&(NInside||MoreCharacter->MaxInside==0)){
		word VICTIMS[1024];
		int NVICT=0;
		int rx2=rx1+rx1+1;
        int stcell=cell-rx1-(rx1<<VAL_SHFCX);
        byte* bpt=NPresence+stcell;
        for(int nx=0;nx<rx2;nx++){
            for(int ny=0;ny<rx2;ny++){
                if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
                    if(bpt[0]&nmask){
                        OneObject*OB=SearchEnemyInCell3(stcell,~nmask,mmask,Priest);
                        if(OB){
							dist=Norma(OB->RealX-REALX,OB->RealY-REALY);
							if(dist>minR&&NVICT<1024){
								VICTIMS[NVICT]=OB->Index;
								NVICT++;
							};
                        };
                    };
                };
                stcell++;
                bpt++;
            };
            stcell+=VAL_MAXCX-rx2;
            bpt+=VAL_MAXCX-rx2;
        };
		if(NVICT){
			DestObj=Group[VICTIMS[(NVICT*int(rando()))>>15]];
			mindist=100;
		};
		if(DestObj){
			AttackObj(DestObj->Index,1,1,0);
			return;
		}
	}else
	if(rx1<=10){
		//if(rando()>8192)return;
#else
    if(rx1<=5){
#endif
		addname(Index);
        //short range search
        int rx2=rx1+rx1+1;
        int stcell=cell-rx1-(rx1<<VAL_SHFCX);
        byte* bpt=NPresence+stcell;
        for(int nx=0;nx<rx2;nx++){
            for(int ny=0;ny<rx2;ny++){
                if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
                    if(bpt[0]&nmask){
                        OneObject*OB=SearchEnemyInCell(RealX,RealY,stcell,~nmask,mmask,Priest,OnlyBrig);
                        if(OB){
                            dist=Norma(OB->RealX-REALX,OB->RealY-REALY);
                            if(dist<mindist&&dist>minR){
                                mindist=dist;
                                DestObj=OB;
								addname(OB->Index);
								addrand(OB->RealX);
								addrand(OB->RealY);
                            };
                        };
                    };
                };
                stcell++;
                bpt++;
            };
               stcell+=VAL_MAXCX-rx2;
               bpt+=VAL_MAXCX-rx2;
        };
    }else{
		//near nose search
		if(MoreCharacter->MinR_Attack<140&&!(StandTime>16&&rando()>6000)){
			addname(Index);
			int rxx=1;
			int rx2=rxx+rxx+1;
			int stcell=cell-rxx-(rxx<<VAL_SHFCX);
			byte* bpt=NPresence+stcell;
			for(int nx=0;nx<rx2;nx++){
				for(int ny=0;ny<rx2;ny++){
					if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
						if(bpt[0]&nmask){
							OneObject*OB=SearchEnemyInCell(RealX,RealY,stcell,~nmask,mmask,Priest,OnlyBrig);
							if(OB){
								dist=Norma(OB->RealX-REALX,OB->RealY-REALY);
								if(dist<mindist&&dist>minR){
									mindist=dist;
									DestObj=OB;
									addname(OB->Index);
									addrand(OB->RealX);
									addrand(OB->RealY);
								};
							};
						};
					};
				};
				stcell++;
				bpt++;
           };
           stcell+=VAL_MAXCX-rx2;
           bpt+=VAL_MAXCX-rx2;
		};
		if(!DestObj){
			addname(Index);
	        //long range search
			int rx2=rx1+rx1+1;
			int stcell;
			int NSR=6;
			if(NM->Usage==TowerID)NSR=20;
			if(TempDelay)NSR=50;
			for(int i=0;i<6;i++){
				int dx=((rando()*rx2)>>15)-rx1;
				int dy=((rando()*rx2)>>15)-rx1;
				stcell=cell+dx+(dy<<VAL_SHFCX);
				if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
					rando();
					if(NPresence[stcell]&nmask){
						rando();
						OneObject*OB=SearchEnemyInCell(RealX,RealY,stcell,~nmask,mmask,Priest,OnlyBrig);
						if(OB){
							rando();
							dist=Norma(OB->RealX-REALX,OB->RealY-REALY);
							if(dist<mindist&&dist>minR){
								mindist=dist;
								DestObj=OB;
								addname(OB->Index);
								addrand(OB->RealX);
								addrand(OB->RealY);
							};
						};
					};
				};
            };
        };
    };
	//if(!DestObj&&!(newMons->Capture||NewBuilding||LockType)){
	//};
	//addname(Index);
	//if(DestObj)addname(DestObj->Index);
	if(DestObj){
		if(DestObj->ActiveAbility){
			bool vis=false;
			byte det=0;
			bool EnableSearchVictim;
            DestObj->ActiveAbility->ActiveAbilities.modifyInvisibility(vis,vis,det,EnableSearchVictim); // если DestObj невидим - не атаковать его
			if( vis && !(NMask & det) )return;
		}
		int dr=(RZ-DestObj->RZ)<<5;
		if(dr<0)dr=0;
		if(dr>300*16)dr=300*16;
		if(mindist<(rr+dr)){
			int ac2=GetUnitActivity(DestObj);
			if(ac2!=1||DestObj->NMask&NMask){
				addrand(DestObj->Index);
				//if(!CheckVisibility(DestObj->RealX,DestObj->RealY,REALX,REALY,Index))return;
				addrand(DestObj->Index);
				//
				void BrigadeRifleAttack(OneObject *OB);
				//
				if(BrigadeID!=0xFFFF&&RifleAttack&&delay==0){
					if(PrioryLevel<=1){					
						int CheckImmediateAttackAbility(OneObject* Killer,OneObject* Victim,byte& Reason,bool Fast);
						byte reason;
						int r=CheckImmediateAttackAbility(this,DestObj,reason,0);
						if(WeaponFlags[newMons->WeaponKind[1]]&3)
						{
							if(r==1)
							{
								addrand(Index);
								BrigadeRifleAttack(this);
							}
						}
						else 
							AttackObj(DestObj->Index,1);
						//
					}
				}else{
					if(BrigadeID==0xFFFF || (!newMons->ArmAttack) || RifleAttack || Norma(RealX-DestObj->RealX,RealY-DestObj->RealY)<100*16){
						AttackObj(DestObj->Index,NewBuilding?128+1:1,1);
					}
				}
			}
		}
	}else addrand(Index+10000);
	
	if(OnlyBrig!=0xFFFF&&SearchOnlyThisBrigadeToKill!=0xFFFF&&BrigadeID!=0xFFFF&&LocalOrder!=NULL)
	{
		SearchOnlyThisBrigadeToKill=0xFFFF;
	}
};
*/
int GetTopDistance(int xa,int ya,int xb,int yb);
word SearchVictim(OneObject* OBJ,int R0,int R1){
    int cell=((OBJ->RealY>>11)<<VAL_SHFCX)+(OBJ->RealX>>11);
    NewMonster* NM=OBJ->newMons;
    int rr=R1<<4;
	int rmin=R0<<4;
    int rx1=(R1>>7)+1;
    byte nmask=~OBJ->NMask;
    byte mmask=NM->KillMask;
    OneObject* DestObj=NULL;
    int mindist=10000000;
    int dist;
    //short range search
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
    byte* bpt=NPresence+stcell;
	int RealX=OBJ->RealX;
	int RealY=OBJ->RealY;
    for(int nx=0;nx<rx2;nx++){
        for(int ny=0;ny<rx2;ny++){
            if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
                if(bpt[0]&nmask){
                    OneObject*OB=SearchEnemyInCell(RealX,RealY,stcell,~nmask,mmask,0,0xFFFF);
                    if(OB){
                        dist=Norma(OB->RealX-RealX,OB->RealY-RealY);
                        if(dist>rmin&&dist<mindist){
                            mindist=dist;
                            DestObj=OB;
                        };
                    };
                };
            };
            stcell++;
            bpt++;
        };
        stcell+=VAL_MAXCX-rx2;
        bpt+=VAL_MAXCX-rx2;
    };
    if(DestObj&&mindist<rr){
		if(GetTopDistance(OBJ->RealX>>10,OBJ->RealY>>10,DestObj->RealX>>10,DestObj->RealY>>10)<20)
			return DestObj->Index;
	};
	return 0xFFFF;
};
void SetUnlimitedLink(OneObject* OB){
	OB->UnlimitedMotion=true;
	OB->DeleteLastOrder();
};
void OneObject::SetOrderedUnlimitedMotion(byte OrdType){
	if(CheckOrderAbility())return;
	addrand(1013);
	Order1* Or1=CreateOrder(OrdType);
	if(!int(Or1))return;
	Or1->OrderType=57;
	Or1->OrderTime=0;
	Or1->PrioryLevel=0;
	Or1->DoLink=&SetUnlimitedLink;	
};
bool FINDNEAREMPTY(int* x,int* y){
	int xx=*x;
	int yy=*y;
	for(int r=1;r<30;r++){
		int N=Rarr[r].N;
		char* dx=Rarr[r].xi;
		char* dy=Rarr[r].yi;
		for(int p=0;p<N;p++){
			if(!CheckBar(xx+dx[p]-7,yy+dy[p]-7,15,15)){
				*x=xx+dx[p];
				*y=yy+dy[p];
				return 1;
			};
		};
	};
	return 0;
};
void XorBlockComplexUnit(OneObject* OB);
void ClearUnlimitedLink(OneObject* OB){
	Order1* OR1=OB->LocalOrder->NextOrder;
	int ms=OB->MotionStyle;
	if(ms!=2&&ms!=5&&ms!=8){
		if(OB->StandTime<5&&!OR1)return;
	}
	if(OB->CObjIndex!=0xFFFF){
		XorBlockComplexUnit(OB);
	}
	if(CheckBar(OB->x-1,OB->y-1,3,3)&&OB->LockedInBuilding==0xFFFF){
		int xx=OB->x;
		int yy=OB->y;
		if(FINDNEAREMPTY(&xx,&yy)){
			OB->UnlimitedMotion=0;
			OB->NewMonsterPreciseSendTo(xx<<8,yy<<8,128+16,1+128);
			OB->UnlimitedMotion=1;
			if(OB->CObjIndex!=0xFFFF){
				XorBlockComplexUnit(OB);
			}
			return;
		};
	};
	if(OB->CObjIndex!=0xFFFF){
		XorBlockComplexUnit(OB);
	}
	OB->UnlimitedMotion=false;
	word GID=OB->LocalOrder->info.BuildObj.ObjIndex;
	OB->DeleteLastOrder();
	OB->PrioryLevel=0;
	if(GID!=0xFFFF&&GID<SCENINF.UGRP.GetAmount()){
		UnitsGroup* UG=SCENINF.UGRP+GID;
		UG->AddNewUnit(OB);
		//UG->Allocate(UG->N+1);
		//UG->IDS[UG->N]=OB->Index;
		//UG->SNS[UG->N]=OB->Serial;
		//UG->N++;		
		if(UG->NMASK)OB->NMask=UG->NMASK;
	};
};
void OneObject::ClearOrderedUnlimitedMotion(byte OrdType,word GroupID){
	if(CheckOrderAbility())return;
	addrand(1015);
	Order1* Or1=CreateOrder(OrdType);
	if(!int(Or1))return;
	Or1->OrderType=57;
	Or1->OrderTime=0;
	Or1->PrioryLevel=0;
	Or1->info.BuildObj.ObjIndex=GroupID;
	Or1->DoLink=&ClearUnlimitedLink;	
};
void OneObject::ClearOrderedUnlimitedMotion(byte OrdType){
	ClearOrderedUnlimitedMotion(OrdType,0xFFFF);
};
OneObject* SearchCapturers(int cell,byte mmask){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				NewMonster* NM=OB->newMons;
				if(!(OB->NMask&mmask||NM->Capture||NM->CantCapture||OB->NNUM==7))return OB;
			};
		};
	};
    return NULL;
};
OneObject* SearchProtectors(int cell,byte mmask){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->NMask&mmask)&&!(OB->newMons->Capture||OB->Sdoxlo)){
				if(OB->BrigadeID!=0xFFFF||(!EngSettings.DefendOnlyWithFormations)){
					return OB;
				}
			}
		};
	};
    return NULL;
};
bool CheckProtectors_Walls(int x,int y,byte mask){
	return false;
};
int GetProtectors(int cell,byte mmask){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	int N=0;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->NMask&mmask)&&!(OB->newMons->Capture||OB->Sdoxlo)){
				if(OB->BrigadeID!=0xFFFF||(!EngSettings.DefendOnlyWithFormations))N++;
			}
		};
	};
    return N;
};
word GetNearestDefender(OneObject* OBJ){
	int cell=((OBJ->RealY>>11)<<VAL_SHFCX)+(OBJ->RealX>>11);
	int CELL0=cell;
	int rx1=3;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
	byte NMASK=OBJ->NMask;
	byte* bpt=NPresence+stcell;
	for(int nx=0;nx<rx2;nx++){
		for(int ny=0;ny<rx2;ny++){
			if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
				if(bpt[0]&NMASK){
					OneObject* EOB=SearchProtectors(stcell,NMASK);
					if(EOB)return EOB->Index;
				};
			};
			stcell++;
			bpt++;
		};
		stcell+=VAL_MAXCX-rx2;
		bpt+=VAL_MAXCX-rx2;
	};
	return 0xFFFF;
};
bool CheckObjectForCapture(OneObject* OB);
int TestCapture(OneObject* OBJ){
	if(!EngSettings.EnableCapturing){
		if(OBJ->Stage<OBJ->NStages&&OBJ->MoreCharacter->MaxR_Attack){
            return true;
		}
		return false;
	}	
	if(!(CheckObjectForCapture(OBJ)||!(OBJ->Ready||OBJ->LocalOrder)))return -1;
	switch(CaptState){
	case 1:
		if(OBJ->newMons->Peasant)return -1;
		break;
	case 2:
		if(OBJ->newMons->Peasant||OBJ->newMons->Usage==CenterID||OBJ->newMons->Usage==MineID)return -1;
		break;
	case 3:
		if(!OBJ->newMons->Artilery)return -1;
		break;
	};
	int cell=((OBJ->RealY>>11)<<VAL_SHFCX)+(OBJ->RealX>>11);

	int X0=OBJ->RealX>>10;
	int Y0=OBJ->RealY>>10;

	int CELL0=cell;
    NewMonster* NM=OBJ->newMons;
    byte nmask=~OBJ->NMask;
	byte NMASK=OBJ->NMask;
    OneObject* DestObj=NULL;
    int mindist=10000000;
	int rx1=3+((NM->AddShotRadius+64)/128);
	int rcap=(120*3+NM->AddShotRadius)*16;
	//X0-=rx1+rx1;
	//Y0-=rx1+rx1;
	bool Capture=true;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
    byte* bpt=NPresence+stcell;
	//byte MyMask=OBJ->NMask;
	if(!(OBJ->Wall&&OBJ->Life<OBJ->MaxLife/3)){
		if(OBJ->NewBuilding){
			rx1+=4;
			rcap+=128*16*4;
		}else{
			rx1++;
			rcap+=128*16;
		}
		rx2=rx1+rx1+1;
		stcell=CELL0-rx1-(rx1<<VAL_SHFCX);
		//checking help
		bpt=NPresence+stcell;
		for(int nx=0;nx<rx2&&Capture;nx++){
			for(int ny=0;ny<rx2&&Capture;ny++){
				if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
					if(bpt[0]&NMASK){
						OneObject*OB=SearchProtectors(stcell,NMASK);
						if(OB&&!OB->newMons->Capture){
							int dist=Norma(OB->RealX-OBJ->RealX,OB->RealY-OBJ->RealY);
							if(dist<rcap){
								Capture=false;
							}
						}
						//if(CheckProtectors_Walls(X0+nx+nx,Y0+ny+ny,MyMask))Capture=false;
					};
				};
				stcell++;
				bpt++;
			};
			stcell+=VAL_MAXCX-rx2;
			bpt+=VAL_MAXCX-rx2;
		};
    };
	if(Capture)return 0;
	else return -1;
};

extern int LastAttackDelay;
extern short AlarmSoundID;
extern int LastAttackX;
extern int LastAttackY;
extern int AlarmDelay;

DLLEXPORT void ChangeUnitColor(int Index,byte DstNI);

void CheckCapture(OneObject* OBJ){	
	
	return;

	NewMonster* NM=OBJ->newMons;
	if(NM->NeverCapture)return;
	switch(CaptState){
	case 1:
		if(OBJ->newMons->Peasant)return;
		break;
	case 2:
		if(OBJ->newMons->Peasant||OBJ->newMons->Usage==CenterID||OBJ->newMons->Usage==MineID)return;
		break;
	case 3:
		if(!(OBJ->newMons->Artilery||OBJ->Wall))return;
		break;
	};
	int cell=((OBJ->RealY>>11)<<VAL_SHFCX)+(OBJ->RealX>>11);

	//int X0=OBJ->RealX>>10;
	//int Y0=OBJ->RealY>>10;

	int CELL0=cell;    
    byte nmask=~OBJ->NMask;
	byte NMASK=OBJ->NMask;
    OneObject* DestObj=NULL;
    int mindist=10000000;
    int dist;	
	int rx1=3+((NM->AddShotRadius+64)/128);
	int rcap=(120*3+NM->AddShotRadius)*16;
	
	//X0-=rx1+rx1;
	//Y0-=rx1+rx1;

	bool Capture=false;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
	byte CapNation;
    byte* bpt=NPresence+stcell;
	int NCapt=0;
	OneObject* CAPUNIT=NULL;
	//byte MyMask=OBJ->NMask;
    for(int nx=0;nx<rx2;nx++){
        for(int ny=0;ny<rx2;ny++){
            if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
                if(bpt[0]&nmask){
                    OneObject*OB=SearchCapturers(stcell,NMASK);
                    if( OB && !OB->newMons->Capture && (OB->BrigadeID!=0xFFFF||!EngSettings.CaptureOnlyWithFormations) ){
                        dist=Norma(OB->RealX-OBJ->RealX,OB->RealY-OBJ->RealY);
                        if(dist<rcap){
							Capture=true;
							CapNation=OB->NNUM;
							NCapt++;
							CAPUNIT=OB;
						}
                    }
                }
            }
            stcell++;
            bpt++;
        };
        stcell+=VAL_MAXCX-rx2;
        bpt+=VAL_MAXCX-rx2;
    };
	int npro=0;
	if(Capture){
		int ac1=GetUnitActivity(OBJ);
		if(ac1==1&&!CAPUNIT)return;
		if(CAPUNIT){
			int ac2=GetUnitActivity(CAPUNIT);
			if(ac1==1){
				CAPUNIT->Die();
				return;
			};
		};
	};
	if(Capture&&!(OBJ->Wall&&OBJ->Life<OBJ->MaxLife/3)){
		if(!OBJ->newMons->Peasant){
			OBJ->delay=100;
			OBJ->MaxDelay=100;
			addname(OBJ->Index);
		};
		//if(OBJ->NewBuilding){
		//	rx1+=4;
		//	rcap+=128*16*4;
		//}else{
			rx1++;
			rcap+=128*16;
		//}
		rx2=rx1+rx1+1;
		stcell=CELL0-rx1-(rx1<<VAL_SHFCX);
		//checking help
		bpt=NPresence+stcell;
		for(int nx=0;nx<rx2;nx++){
			for(int ny=0;ny<rx2;ny++){
				if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
					if(bpt[0]&NMASK){
						OneObject*OB=SearchProtectors(stcell,NMASK);
						if( OB && !(OB->newMons->Capture||OB->newMons->CantCapture) ){
							dist=Norma(OB->RealX-OBJ->RealX,OB->RealY-OBJ->RealY);
							if(dist<rcap){
								Capture=false;
								npro++;
							}
						}
						//if(CheckProtectors_Walls(X0+nx+nx,Y0+ny+ny,MyMask))Capture=false;
					}
				}
				stcell++;
				bpt++;
			};
			stcell+=VAL_MAXCX-rx2;
			bpt+=VAL_MAXCX-rx2;
		};
		if(OBJ->Nat->AI_Enabled&&OBJ->newMons->Artilery){
			if(npro&&(NCapt>=npro&&npro==1)||(NCapt>3&&npro==2)||(NCapt>7&&npro==3)||(NCapt>10&&npro==4)){
				OBJ->Die();
				return;
			};
		};
    };
	if(Capture){
		if(!EngSettings.EnableCapturing){
			if(OBJ->Stage<OBJ->NStages&&OBJ->MoreCharacter->MaxR_Attack){
				return;
			}
			return;
		}
		byte OldNat=OBJ->NNUM;
		bool Easy=CITY[OldNat].Difficulty<3;
		if(OBJ->UnlimitedMotion&&!OBJ->Hidden){
			OBJ->Die();
			return;
		};
		if(OBJ->AutoKill||OBJ->Wall){
			int idx=OBJ->Index;
			DestructBuilding(OBJ);
			if(Group[idx]&&OBJ->Sdoxlo)return;
		}else{
			/*
			if(NATIONS[OldNat].AI_Enabled){
				int ttt=tmtmt&63;
				if(OBJ->NewBuilding&&ttt>16)DestructBuilding(OBJ);
				else{
					byte use=OBJ->newMons->Usage;
					if(use==PeasantID&&!Easy){
						DestructBuilding(OBJ);
					}else{
						if((use==SupMortID&&ttt>30)||(use==PushkaID&&ttt>20)||
							(use==MortiraID&&ttt>8)||(use==PeasantID&&ttt>35)){
								DestructBuilding(OBJ);
							return;
						};
					};
				};
			}else{
			*/
				
			//};
		};
		if(!ext_OnUnitCapture(OBJ,CAPUNIT))return;

		LogBattle(OldNat,"^ST_BCAPT^ %s(%d)",GetSt_UName(CAPUNIT->NIndex),CITY[OldNat].ReadyAmount[CAPUNIT->NIndex]-1);

		NATIONS[OldNat].NKilled[OBJ->NIndex]++;
		OBJ->Nat->CITY->Account-=EngSettings.LooseModulator*OBJ->newMons->Expa;
		CITY[CapNation].Account+=EngSettings.KillModulator*OBJ->newMons->Expa;;
		if(OBJ->NNUM==MyNation&&OBJ->NewBuilding){
			char ccc[200];
			sprintf(ccc,CAPBLD,OBJ->Ref.General->Message);
			//AssignHint1(ccc,100,32);
		};
		if(OBJ->NNUM==MyNation){
			void TryAlarm(int x,int y,int);
			TryAlarm(OBJ->RealX>>4,OBJ->RealY>>4,1);//capture alarm
		};
		if(OBJ->newMons->Usage==PeasantID)OBJ->ClearOrders();

		//if(OBJ->NewBuilding&&(!OBJ->Ready)&&OBJ->Stage>=OBJ->MoreCharacter->ProduceStages)
		//	OBJ->Ready=true;

		//DelObject(OBJ);
		//OBJ->Nat->CITY->UnRegisterNewUnit(OBJ);

		int oldst=OBJ->Stage;
		int oldmax=OBJ->MoreCharacter->ProduceStages;
		if(!oldmax) oldmax=1;

		//OBJ->Ref.General=NATIONS[CapNation].Mon[OBJ->NIndex];
		//OBJ->Nat=&NATIONS[CapNation];
		//OBJ->NMask=OBJ->Nat->NMask;

		//OBJ->Serial^=1;

		OBJ->Selected=0;
		OBJ->ImSelected=0;

		ChangeUnitColor(OBJ->Index,CapNation);
		OBJ->NMask|=128;

		OBJ->Zombi=false;

		if(NATIONS[OldNat].AI_Enabled){
			if(OBJ->NewBuilding  && OBJ->CObjIndex==0xFFFF){
				//DestructBuilding(OBJ);
				//return;
			};
		};
		
		//if(OBJ->NewBuilding&&OldNat==MyNation){
		//	void TryToAddAlarm(int x,int y);
		//	TryToAddAlarm(OBJ->RealX>>4,OBJ->RealY>>4);			
		//};

		//OBJ->NNUM=CapNation;
		//OBJ->Nat->CITY->RegisterNewUnit(OBJ);
		//AddObject(OBJ);

		if( OBJ->NewBuilding && oldmax ){
			if(oldst>oldmax)oldst=oldmax;
			OBJ->Stage=(oldst*int(OBJ->MoreCharacter->ProduceStages))/oldmax;
		}

		if(OBJ->NInside){
			if(!CaptState){
				word* Uni=OBJ->Inside;
				for(int j=0;j<OBJ->NInside;j++){
					word MID=Uni[j];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB){
							OB->Nat->CITY->Account-=EngSettings.LooseModulator*OB->newMons->Expa;
							CITY[CapNation].Account+=EngSettings.KillModulator*OB->newMons->Expa;;
							if(OB->newMons->Usage==PeasantID)OB->ClearOrders();
							DelObject(OB);
							OB->Nat->CITY->UnRegisterNewUnit(OB);
							OB->Ref.General=NATIONS[CapNation].Mon[OB->NIndex];
							OB->Nat=&NATIONS[CapNation];
							OB->NMask=OB->Nat->NMask;
							OB->Serial^=1;
							OB->Selected=false;
							OB->ImSelected=false;
							OB->Zombi=false;
							byte OldNat=OB->NNUM;
							OB->NNUM=CapNation;
							OB->Nat->CITY->RegisterNewUnit(OB);
							AddObject(OB);
							if(OB->LocalOrder)OB->Ready=1;
							OB->ClearOrders();							
						};
					};
				};
			}else DestructBuilding(OBJ);
		};
		if(NATIONS[CapNation].AI_Enabled)DestructBuilding(OBJ);
	};
};
int GetAmountOfProtectors(OneObject* OBJ){
	int cell=((OBJ->RealY>>11)<<VAL_SHFCX)+(OBJ->RealX>>11);
	int CELL0=cell;
	int rx1=5;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
	int N=0;
	byte NMASK=OBJ->NMask;
	byte* bpt=NPresence+stcell;
	for(int nx=0;nx<rx2;nx++){
		for(int ny=0;ny<rx2;ny++){
			if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
				if(bpt[0]&NMASK){
					N+=GetProtectors(stcell,NMASK);
				};
			};
			stcell++;
			bpt++;
		};
		stcell+=VAL_MAXCX-rx2;
		bpt+=VAL_MAXCX-rx2;
	};
	return N;
};
extern int HISPEED;
byte TempSoundBuf[1024];
void ClearTempSoundBuf(){
	memset(TempSoundBuf,0,sizeof TempSoundBuf);
}
int probT[6]={32768,5000,1000,200,0,0};
void PlayAnimation(NewAnimation* NA,int Frame,int x,int y){
	extern bool vCreditsMode;
	if(vCreditsMode||!NOPAUSE)return;
	if(GSets.SVOpt.SoundVolume<3)return;	
	if(NA->SoundID<1||(NA->NFrames==0&&NA->HotFrame!=0xFF))return;	
	
    int HF=int(NA->HotFrame+y%8)<<8;
	extern CDirSound* CDS;
	CDS->HitSound(NA->SoundID);
	if((NA->HotFrame==0xFF||(Frame<=HF&&Frame+GameSpeed>HF))&&rand()<NA->SoundProbability){
		int n=NA->SoundID;
		if(n>=0&&n<4096){
			static int LastReqTime[4096];
			static bool init=true;
			if(init){
				int T=GetTickCount();
				for(int i=0;i<4096;i++){
					LastReqTime[i]=T-1000;
				}
				init=false;
			}
			extern int LastFlipTime;
			if(LastFlipTime-LastReqTime[n]<rand()%150){			
				return;
			}
			LastReqTime[n]=LastFlipTime;
			AddEffect(x,y,NA->SoundID);
		}
	}
}
void PlayAnimationEx(OneObject* OB,NewAnimation* NA,int Frame,int x,int y){
	extern bool vCreditsMode;
	if(vCreditsMode||!NOPAUSE)return;
	if(GSets.SVOpt.SoundVolume<3)return;
	if(NA->SoundID<1||(NA->NFrames==0&&NA->HotFrame!=0xFF))return;

	int SoundID=NA->SoundID;

    /*
	if(OB&&NA->Code==anm_MotionL&&OB->BrigadeID!=0xFFFF){
        Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
		if(BR&&BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD){
			NewMonster* NM=OB->newMons;
			if(NM->MotionOnRoadSound!=-1)SoundID=NM->MotionOnRoadSound;
		}
	}
    */

	int HF=int(NA->HotFrame+y%8)<<8;

	extern CDirSound* CDS;
	CDS->HitSound(SoundID);
	bool DoPlay=false;
	if(NA->HotFrame==0xFF)DoPlay=true;
	else if(OB){
        if(rand()<NA->SoundProbability){
    		if(OB->NewCurSpritePrev<NA->HotFrame && OB->NewCurSprite>=NA->HotFrame)DoPlay=true;		
	    	if(NA->HotFrame2>0 && OB->NewCurSpritePrev<NA->HotFrame2 && OB->NewCurSprite>=NA->HotFrame2)DoPlay=true;
        }
	}else{
		DoPlay=(Frame<=HF&&Frame+GameSpeed>HF)&&rand()<NA->SoundProbability;		
	}

	if(DoPlay){
		int n=SoundID;
		if(n>=0&&n<4096){
			static int LastReqTime[4096];
			static bool init=true;
			if(init){
				int T=GetTickCount();
				for(int i=0;i<4096;i++){
					LastReqTime[i]=T-1000;
				}
				init=false;
			}
			extern int LastFlipTime;
			if(LastFlipTime-LastReqTime[n]<rand()%150){			
				return;
			}
			LastReqTime[n]=LastFlipTime;
			AddEffect(x,y,SoundID);
		}
	}
}
bool CheckPlaceForShar(int x,int y){
	if(CheckBar((x>>4)-8,(y>>4)-8,17,17))return false;
	int h=GetHeight(x,y);
	if(abs(GetHeight(x-60,y)-h)>10||abs(GetHeight(x+60,y)-h)>10||abs(GetHeight(x,y-60)-h)>10||abs(GetHeight(x,y+60)-h)>10)return false;
	//check monsters
	return CheckMonstersInArea(0xFF,x<<4,y<<4,150<<4);//&&CheckSpritesInArea(x<<4,y<<4,150<<4);
};
void HandleShar(Nation* NT){
	if(NT->SharAllowed){
		addrand(NT->NNUM);
		if(NT->SharPlaceFound){
			addrand(NT->NNUM);
			if(NT->SharStage==0xFF){
				addrand(NT->NNUM);
				//processing and drawing <shar> in motion.
				//1.processing
				int x=NT->SharX>>4;
				int y=NT->SharY>>4;
				if(y>(msy<<5))y=msy<<5;
				int z=GetHeight(x,y);
				addrand(x);
				addrand(y);
				int dz=(NT->SharZ>>4)-z;
				addrand(dz);
				addrand(z);
				bool AllowClip=false;
				if(dz<100){
					NT->SharAx=0;
					NT->SharAy=0;
					NT->SharAz=64;
				}else{
					NT->Vision=true;
					NT->SharAz=(250-dz)>>5;
					if(NT->SharAz>64)NT->SharAz=64;
					if(NT->SharAz<-64)NT->SharAz=-64;
					//free motion;
					if(rando()<60){
						do{
							NT->SharAx=(rando()&64)-32;
						}while(abs(NT->SharAx)<16);
						do{
							NT->SharAy=(rando()&64)-32;
						}while(abs(NT->SharAy)<16);
					};
					AllowClip=true;
				};
				//Friction
				int fx=-NT->SharVx>>7;
				int fy=-NT->SharVy>>7;
				int fz=-NT->SharVz>>7;
				if(AllowClip){
					if(x<1200)fx+=64;
					if(y<1200)fy+=64;
					if(x>(msx<<5)-1200)fx-=64;
					if(y>(msy<<5)-1200)fy-=64;
				};
				NT->SharVx+=NT->SharAx+fx;
				NT->SharVy+=NT->SharAy+fy;
				NT->SharVz+=NT->SharAz+fz;
				NT->SharX+=NT->SharVx>>7;
				NT->SharY+=NT->SharVy>>7;
				NT->SharZ+=NT->SharVz>>7;
			}else{
				int frm=0;
				if(NT->SharStage>180)frm=(NT->SharStage-180)>>1;
				NT->SharStage++;
				int dx=mapx<<5;
				int dy=mapy<<4;
				int x=NT->SharX>>4;
				int y=NT->SharY>>4;
				int z=GetHeight(x,y);
				if(frm>=SharBuild->NFrames-FrmDec){
					BClrBar((NT->SharX>>8)-3,(NT->SharY>>8)-3,7);
					NT->SharStage=0xFF;
					NT->SharZ=z<<4;
					NT->SharVx=0;
					NT->SharVy=0;
					NT->SharVz=0;
					NT->SharAx=0;
					NT->SharAy=0;
					NT->SharAz=0;
				};
			};
		}else{
			int xx=(NT->SharX>>4)+((int(rando())*NT->SearchRadius)>>15);
			int yy=(NT->SharY>>4)+((int(rando())*NT->SearchRadius)>>15);
			if(CheckPlaceForShar(xx,yy)){
				NT->SharPlaceFound=true;
				NT->SharStage=0;
				NT->SharX=xx<<4;
				NT->SharY=yy<<4;
				BSetBar((NT->SharX>>8)-3,(NT->SharY>>8)-3,7);
				addrand(xx+yy);
			}else NT->SearchRadius+=10;
		};
	};
};
void DrawShar(Nation* NT){
	if(NT->SharAllowed){
		if(NT->SharPlaceFound){
			if(NT->SharStage==0xFF){
				//processing and drawing <shar> in motion.
				//1.processing
				int x=NT->SharX>>4;
				int y=NT->SharY>>4;
				if(y>(msy<<5))y=msy<<5;
				int z=GetHeight(x,y);
				int dz=(NT->SharZ>>4)-z;
				bool AllowClip=false;
				
				//2.drawing
				if(Shar){
					int dx=mapx<<5;
					int dy=mapy<<4;
					int x=NT->SharX>>4;
					int y=NT->SharY>>4;
					int z=NT->SharZ>>4;
					int vx=NT->SharVx>>5;
					int vy=NT->SharVy>>5;
					z+=int(sqrt(vx*vx+vy*vy));
					int z0=GetHeight(x,y);
					int dz=(z-z0)>>1;
					x-=dx;
					y=(y>>1)-dy-z;
					int maxxx=smaplx<<5;
					int maxyy=smaply<<4;
					CurDrawNation=NT->NNUM;
					if(x>-128&&y>-160&&x<maxxx+128&&y<maxyy+160){
						NewFrame* NF=Shar->Frames[0];
						AddPoint(x,y+z,x+NF->dx,y+NF->dy,NULL,NF->FileID,NF->SpriteID,0,0);
					};
					//shadow drawing
					x=(NT->SharX>>4)-dz;
					y=(NT->SharY>>4)-(dz>>1);
					z=GetHeight(x,y);
					x-=dx;
					y=(y>>1)-dy-z;
					if(x>-128&&y>-160&&x<maxxx+128&&y<maxyy+160){
						NewFrame* NF=Shar->Frames[1];
						CurDrawNation=NT->NNUM;
						AddPoint(x,y+z,x+NF->dx,y+NF->dy,NULL,NF->FileID,NF->SpriteID,0,0);
					};
				};
			}else{
				int frm=0;
				if(NT->SharStage>180)frm=(NT->SharStage-180)>>1;
				int dx=mapx<<5;
				int dy=mapy<<4;
				int x=NT->SharX>>4;
				int y=NT->SharY>>4;
				int z=GetHeight(x,y);
				x-=dx;
				y=(y>>1)-dy-z;
				int maxxx=smaplx<<5;
				int maxyy=smaply<<4;
				if(x>-128&&y>-160&&x<maxxx+128&&y<maxyy+160){
					NewFrame* NF=SharBuild->Frames[frm];
					CurDrawNation=NT->NNUM;
					AddPoint(x,y,x+NF->dx,y+NF->dy,NULL,NF->FileID,NF->SpriteID,0,0);
				};
			};
		};
	};
};
BlockBars::BlockBars(){
	BC=NULL;
	NBars=0;
	MaxBars=0;
};
void BlockBars::Clear(){
	if(BC)free(BC);
	NBars=0;
	MaxBars=0;
	BC=NULL;
};
BlockBars::~BlockBars(){
	Clear();
};
bool BlockBars::Add(word x,word y){
	if(NBars){
		DWORD DT=x+(y<<16);
		DWORD* SDAT=(DWORD*)BC;
		for(int i=0;i<NBars;i++)if(SDAT[i]==DT)return false;
	};
	if(NBars>=MaxBars){
		MaxBars+=128;
		BC=(BlockCell*)realloc(BC,MaxBars*sizeof(BlockCell));
	};
	BC[NBars].x=x;
	BC[NBars].y=y;
	NBars++;
	return true;
};
bool BlockBars::FastAdd(word x,word y){
	if(NBars>=MaxBars){
		MaxBars+=128;
		BC=(BlockCell*)realloc(BC,MaxBars*sizeof(BlockCell));
	};
	BC[NBars].x=x;
	BC[NBars].y=y;
	NBars++;
	return true;
};
bool BlockBars::Delete(word x,word y){
	if(NBars){
		DWORD DT=x+(y<<16);
		DWORD* SDAT=(DWORD*)BC;
		int i;
		for(i=0;i<NBars&&SDAT[i]!=DT;i++);
		if(i>=NBars)return false;
		if(i<NBars-1){
			memcpy(SDAT+i,SDAT+i+1,(NBars-i-1)<<2);
		};
		NBars--;
		return false;
	};
	return false;
};
void MoveAwayInCell(int cell,int x,int y){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int dist=Norma(OB->RealX-x,OB->RealY-y);
                if(dist<364*16){
					int dx=OB->RealX-x;
					int dy=OB->RealY-y;
					if(dx||dy){
						dx=(350*16*dx)/dist;
						dy=(350*16*dy)/dist;
						if(!CheckBar(((x+dx)>>8)-2,((y+dy)>>8)-2,4,4)){
							OB->NewMonsterSendTo(x+dx,y+dy,128,1);
						};
					};
				};
			};
		};
	};
};
void MoveAway(int x,int y){
	int cell=((y>>11)<<VAL_SHFCX)+(x>>11);
	int CELL0=cell;
	int rx1=3;
	bool Capture=false;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
    for(int nx=0;nx<rx2;nx++){
        for(int ny=0;ny<rx2;ny++){
            if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
				MoveAwayInCell(stcell,x,y);
            };
            stcell++;
        };
        stcell+=VAL_MAXCX-rx2;
    };
};
bool MoveAwaySectorInCell(int cell,int x,int y,int r0,int r1,char Dir,int ddir){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return false;
	int ofs1=cell<<SHFCELL;
	word MID;
	bool Pushed=false;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int dist=Norma(OB->RealX-x,OB->RealY-y);
                if(dist<r1&&dist>r0){
					int dx=OB->RealX-x;
					int dy=OB->RealY-y;
					if(dx||dy){
						char ndir=char(GetDir(dx,dy));
						if(abs(ndir-Dir)<ddir){
							if(OB->FrameFinished){
								int r=TryToMove(OB,OB->RealDir,false);
								if(r==-1)TryToMove(OB,OB->RealDir,true);
								/*
								dx=(int(TCos[OB->RealDir])<<3);
								dy=(int(TSin[OB->RealDir])<<3);
								
								if(!CheckBar(((x+dx)>>8)-2,((y+dy)>>8)-2,4,4)){
									OB->NewMonsterSendTo(OB->RealX+dx,OB->RealY+dy,128+64,1);
								}else{
									OB->RealDir+=32;
									OB->GraphDir=OB->RealDir;
								};
								*/
							};
							Pushed=true;
						};
					};
				};
			};
		};
	};
	return Pushed;
};
bool MoveAwayInSector(int x,int y,int r0,int r1,char Dir,int ddir){
	int cell=((y>>11)<<VAL_SHFCX)+(x>>11);
	int CELL0=cell;
	int rx1=2;
	bool Capture=false;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
	bool Pushed=false;
    for(int nx=0;nx<rx2;nx++){
        for(int ny=0;ny<rx2;ny++){
            if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
				if(MoveAwaySectorInCell(stcell,x,y,r0,r1,Dir,ddir))Pushed=true;
            };
            stcell++;
        };
        stcell+=VAL_MAXCX-rx2;
    };
	return Pushed;
};
#define SectRAD (450*16)
#define SectMIN (32*16)
void GetUnitsInSector(int cell,int x,int y,char IniDir,int* My,int* Enemy,byte Mask){
	cell+=VAL_MAXCX+1;
	if(cell>=0&&cell<VAL_MAXCX*VAL_MAXCX){
	    int NMon=MCount[cell];
		if(!NMon)return;
		int ofs1=cell<<SHFCELL;
		word MID;
		bool Pushed=false;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB){
					int dx=OB->RealX-x;
					int dy=OB->RealY-y;
					int dist=Norma(dx,dy);
		            if(dist<SectRAD&&dist>SectMIN){
						if(dx||dy){
							char ndir=char(GetDir(dx,dy));
							if(abs(ndir-IniDir)<28){
								if(OB->NMask&Mask)(*My)++;
								else (*Enemy)++;
							};
						};
					 };
				};
			};
		};
	};
};
bool TestUnitsInSectorForGrenade(OneObject* GRE){
	int x=GRE->RealX;
	int y=GRE->RealY;
	int cell=((y>>11)<<VAL_SHFCX)+(x>>11);
	int CELL0=cell;
	int rx1=4;
	bool Capture=false;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
	bool Pushed=false;
	int NMY=0;
	int NHIM=0;
    for(int nx=0;nx<rx2;nx++){
        for(int ny=0;ny<rx2;ny++){
            if(stcell>=0&&stcell<VAL_MAXCX*VAL_MAXCX){
				GetUnitsInSector(stcell,x,y,GRE->RealDir,&NMY,&NHIM,GRE->NMask);
            };
            stcell++;
        };
        stcell+=VAL_MAXCX-rx2;
    };
	return NMY<9&&(NMY+NMY+NMY<=NHIM);
};
int GetTopology1(int x,int y){
	int xc=x>>2;
	int yc=y>>2;
	word tr=SafeTopRef(xc,yc);
	if(tr<0xFFFE)return tr;
	else return 0xFFFF;
};
int GetWTopology1(int x,int y,byte LTP){
	int xc=x>>2;
	int yc=y>>2;
	if(xc<0||yc<0||xc>=TopLx||yc>=TopLy)return 0xFFFF;
	word tr=GetTopRef(xc+(yc<<TopSH),LTP);
	if(tr<0xFFFE)return tr;
	else return 0xFFFF;
};
bool TopFindBestPosition(OneObject* OB,int* xd,int* yd,int R0,int Top,byte LTP){
	MotionField* MFI=MFIELDS+OB->LockType;
	if(!OB)return false;
	if(!MFI->CheckBar((*xd)-1,(*yd)-1,OB->Lx+1,OB->Lx+1))return true;
	int Lx=OB->Lx;
	int R1=R0;
	int bx=*xd;
	int by=*yd;
	int bdist=100000;
	int xxx=bx-1;
	int yyy=by-1;
	int LL=2;
	int bx1,by1,dstn;
	int x=OB->x;
	int y=OB->y;
	while(R1){
		int i;
		for(i=0;i<=LL;i++)if(!MFI->CheckBar(xxx+i-1,yyy-1,Lx,Lx)){
			bx1=xxx+i;
			by1=yyy;
			dstn=Norma(bx1-x,by1-y);
			if(dstn<bdist&&GetWTopology1(bx1,by1,LTP)==Top){
				bx=bx1;
				by=by1;
				bdist=dstn;
			};
		};
		for(i=0;i<=LL;i++)if(!MFI->CheckBar(xxx+i-1,yyy+LL-1,Lx,Lx)){
			bx1=xxx+i;
			by1=yyy+LL;
			dstn=Norma(bx1-x,by1-y);
			if(dstn<bdist&&GetWTopology1(bx1,by1,LTP)==Top){
				bx=bx1;
				by=by1;
				bdist=dstn;
			};
		};
		for(i=0;i<LL-1;i++)if(!MFI->CheckBar(xxx-1,yyy+i,Lx,Lx)){
			bx1=xxx;
			by1=yyy+i+1;
			dstn=Norma(bx1-x,by1-y);
			if(dstn<bdist&&GetWTopology1(bx1,by1,LTP)==Top){
				bx=bx1;
				by=by1;
				bdist=dstn;
			};
		};
		for(i=0;i<LL-1;i++)if(!MFI->CheckBar(xxx+LL-1,yyy+i,Lx,Lx)){
			bx1=xxx+LL;
			by1=yyy+i+1;
			dstn=Norma(bx1-x,by1-y);
			if(dstn<bdist&&GetWTopology1(bx1,by1,LTP)==Top){
				bx=bx1;
				by=by1;
				bdist=dstn;
			};
		};
		if(bdist<100000){
			*xd=bx;
			*yd=by;
			return true;
		};
		R1--;
		LL+=2;
		xxx--;
		yyy--;
	};
	return false;
};
bool CheckDirectWay(int x0,int y0,int x1,int y1){
	int bx=x0<<10;
	int by=y0<<10;
	int Lx=x1-x0;
	int Ly=y1-y0;
	int r=Norma(Lx,Ly);
	int N=(r>>6)+1;
	Lx=(Lx<<10)/N;
	Ly=(Ly<<10)/N;
	for(int i=0;i<N;i++){
		bx+=Lx;
		by+=Ly;
		if(CheckPt(bx>>14,by>>14))return false;
	};
	return true;
};
int FindSuperSmartBestPosition(OneObject* OB,int* cx,int* cy,int dx,int dy,word Top,byte LTP,int uidx){
	MotionField* MF=MFIELDS;;
	//if(OB) MF=MFIELDS+OB->LockType;
	MF=MFIELDS+LTP;

	int LDX=-2;
	int LLX=4;
	if(OB&&OB->LockType){
		LDX=-7;
		LLX=14;
	};
	int x0=(*cx)<<8;
	int y0=(*cy)<<8;
	int N=(Norma(dx,dy)>>5)+1;
	int DX=(dx<<8)/N;
	int DY=(dy<<8)/N;
	int mx=msx>>1;
	if(LTP!=1){
		int i;
		for(i=0;i<N;i++){
			x0+=DX;
			y0+=DY;
			int tx=x0>>(8+6);
			int ty=y0>>(8+6);
			if(tx<=0||ty<=0||tx>=mx||ty>=mx)goto NextSm1;
			int ofs=tx+TopLx*ty;
			if(GetTopRef(ofs,LTP)>=0xFFFE)goto NextSm1;
		};
NextSm1:;
		bool Prolong=true;
		if(i!=N){
			x0-=DX;
			y0-=DY;
			Prolong=false;
		};
		N=i;
		if(!MF->CheckBar((x0>>(8+4))+LDX,(y0>>(8+4))+LDX,LLX,LLX)){
			*cx=x0>>8;
			*cy=y0>>8;
			//assert(!CheckBar((*cx>>4)-1,(*cy>>4)-1,3,3));
			return 1;
		};
		int xx=x0;
		int yy=y0;
		for(i=0;i<N;i++){
			x0-=DX;
			y0-=DY;
			if(!MF->CheckBar((x0>>(8+4))+LDX,(y0>>(8+4))+LDX,LLX,LLX)){
				*cx=x0>>8;
				*cy=y0>>8;
				//assert(!CheckBar((*cx>>4)-1,(*cy>>4)-1,3,3));
				return 2;
			};
		};
		if(Prolong){
			x0=xx;
			y0=yy;
			for(int i=0;i<5;i++){
				x0+=DX;
				y0+=DY;
				int tx=x0>>(8+6);
				int ty=y0>>(8+6);
				if(tx<=0||ty<=0||tx>=mx||ty>=mx)goto NextSm2;
				int ofs=tx+TopLx*ty;
				if(GetTopRef(ofs,LTP)>=0xFFFE)goto NextSm2;
				if(!MF->CheckBar((x0>>(8+4))+LDX,(y0>>(8+4))+LDX,LLX,LLX)){
					*cx=x0>>8;
					*cy=y0>>8;
					//assert(!CheckBar((*cx>>4)-1,(*cy>>4)-1,3,3));
					return 3;
				};
			};
		};
	};
NextSm2:;
	if(!OB) return false;
	int ccx=(*cx)>>4;
	int ccy=(*cy)>>4;
	if(TopFindBestPosition(OB,&ccx,&ccy,60,Top,LTP)){
		*cx=ccx<<4;
		*cy=ccy<<4;
		//assert(!CheckBar((*cx>>4)-1,(*cy>>4)-1,3,3));
		return 4;
	};	
	return false;
};
bool CheckDirectConnectionOfPoints(int LTP,int xs,int ys,int& x1,int& y1){
	int dx=x1-xs;
	int dy=y1-ys;
	int x0=(xs)<<8;
	int y0=(ys)<<8;
	int N=(Norma(dx,dy)>>5)+1;
	int DX=(dx<<8)/N;
	int DY=(dy<<8)/N;
	int mx=msx>>1;
	if(LTP!=1){
		for(int i=0;i<N;i++){
			x0+=DX;
			y0+=DY;
			int tx=x0>>(8+6);
			int ty=y0>>(8+6);
			if(tx<=0||ty<=0||tx>=mx||ty>=mx){
                x1=x0>>8;
				y1=y0>>8;
				return false;
			}
			int ofs=tx+TopLx*ty;
			if((MFIELDS[LTP].CheckBar((x0>>12)-1,(y0>>12)-1,3,3))||GetTopRef(ofs,LTP)>=0xFFFE){
				x1=x0>>8;
				y1=y0>>8;
				return false;
			}
		}
	}
	return true;
}
int FindSuperSmartBestPositionForBrigade(OneObject* OB,int* cx,int* cy,int dx,int dy,word Top,byte LTP,int uidx){
	MotionField* MF=MFIELDS;;
	//if(OB) MF=MFIELDS+OB->LockType;
	MF=MFIELDS+LTP;
	int mx=msx>>1;
	int x1=(*cx)+dx;
	int y1=(*cy)+dy;
	if(CheckDirectConnectionOfPoints(LTP,*cx,*cy,x1,y1)){
		*cx+=dx;
		*cy+=dy;
		return 1;
	}
	for(int i=0;i<60;i++){
		int N=Rarr[i].N;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		for(int j=0;j<N;j++){
			int xn=x1+int(xi[j])*20;
			int yn=y1+int(yi[j])*20;
			if((!CheckBar((xn>>4)-1,(yn>>4)-1,3,3))&&CheckDirectConnectionOfPoints(LTP,*cx,*cy,xn,yn)){
				if(uidx)uidx--;
				else{
					*cx=xn;
					*cy=yn;
					return 2;
				}
			}
		}
	}
	return false;
};
int GetWTopology(int x,int y,byte LTP){
	if(!GetNAreas(LTP))return 0xFFFF;
	int xc=x>>6;
	int yc=y>>6;
	if(xc<0||yc<0||xc>=TopLx||yc>=TopLy)return 0xFFFF;
	word tr=GetTopRef(xc+(yc<<TopSH),LTP);
	if(tr<0xFFFE)return tr;
	for(int i=0;i<10;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=xc+xi[j];
			int yy=yc+yi[j];
			if(xx<0||yy<0||xx>=TopLx||yy>=TopLy)tr=0xFFFF;
			else tr=GetTopRef(xx+(yy<<TopSH),LTP);
			if(tr<0xFFFE)return tr;
		};
	};
	return 0xFFFF;
};
int GetWTopology(int* x,int* y,byte LTP){
	int xc=(*x)>>6;
	int yc=(*y)>>6;
	if(xc<0||yc<0||xc>=TopLx||yc>=TopLy)return 0xFFFF;
	word tr=GetTopRef(xc+(yc<<TopSH),LTP);
	if(tr<0xFFFE)return tr;
	for(int i=0;i<10;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=xc+xi[j];
			int yy=yc+yi[j];
			if(xx<0||yy<0||xx>=TopLx||yy>=TopLy)tr=0xFFFF;
			else tr=GetTopRef(xx+(yy<<TopSH),LTP);
			if(tr<0xFFFE){
				*x=(xx<<6)+32;
				*y=(yy<<6)+32;
				return tr;
			};
		};
	};
	return 0xFFFF;
};
void NewMonsterSmartSendToLink(OneObject* OBJ){
#ifdef _USE3D
	//---definitions----
	void CreateWayNet(int x0,int y0,int Z1,int Z2,int *x,int *y,int& N,word UType,int DR);
	bool CheckIfRoadZone(int Zone);
	//------------------
	Order1* OR1=OBJ->LocalOrder;
	OBJ->PrioryLevel=OR1->PrioryLevel;
	int x=OR1->info.SmartSend.x;
	int y=OR1->info.SmartSend.y;
	int dx=OR1->info.SmartSend.dx;
	int dy=OR1->info.SmartSend.dy;
	int NextX=OR1->info.SmartSend.NextX;
	int NextY=OR1->info.SmartSend.NextY;
	int NextTop=OR1->info.SmartSend.NextTop;
	byte LTP=OBJ->LockType;
#ifdef COSSACKS2
	OBJ->UnitSpeed=64;
#endif
	int FinalTop=GetWTopology(&x,&y,LTP);

	if(FinalTop<0xFFFE&&OBJ->GroundState!=1){
		//searching for the next point
		int CTOP=GetWTopology(OBJ->RealX>>4,OBJ->RealY>>4,LTP);
		if(CTOP<0xFFFE){
			int NA=GetNAreas(LTP);
			NextTop=CTOP;
			/*
			int NTP=CTOP;
			if(!CheckIfRoadZone(CTOP)){
				
				int NTP0;
				int NATT=0;
				do{
					NTP0=NTP;
					NTP=GetMotionLinks(FinalTop+NA*NTP,LTP);
					if(NTP<0xFFFE&&NTP!=FinalTop&&CheckIfRoadZone(NTP)){
						Area* AR=GetTopMap(NTP,LTP);
						OBJ->NewMonsterSmartSendTo(AR->x<<6,AR->y<<6,0,0,OR1->PrioryLevel,1);
						return;
					};
					NATT++;
				}while(NTP<0xFFFE&&NTP!=FinalTop&&NTP!=NTP0&&NATT<10);
				
				
			};
			*/
			int NTOP=GetMotionLinks(FinalTop+NA*CTOP,LTP,OBJ->NNUM);
			if(OBJ->Lx>1&&OBJ->BrigadeID==0xFFFF){//thick objects like Cannon...
				if(NTOP<0xFFFE&&NTOP!=FinalTop){
					NTOP=GetMotionLinks(FinalTop+NA*NTOP,LTP,OBJ->NNUM);
					if(NTOP<0xFFFE&&NTOP!=FinalTop){
						NTOP=GetMotionLinks(FinalTop+NA*NTOP,LTP,OBJ->NNUM);
					}
				}                
			}
			if(NTOP<0xFFFE){
				if(CheckIfRoadZone(NTOP)&&CheckIfRoadZone(CTOP)){
					int xx[8];
					int yy[8];
					int NP=8;
						CreateWayNet(OBJ->RealX>>4,OBJ->RealY>>4,CTOP,NTOP,xx,yy,NP,OBJ->NIndex,0);
					for(int p=NP-1;p>=0;p--){
#ifdef COSSACKS2
						int GetZoneSpeedBunus(int Zone);
						OBJ->UnitSpeed=(64*GetZoneSpeedBunus(CTOP))>>8;
						//if(OBJ->CObjIndex!=0xFFFF){
						//	OBJ->UnitSpeed=64+32;
						//}
						addrand(OBJ->UnitSpeed);
#endif //COSSACKS2
						/*
						int R=Norma(dx,dy);
						if(R>32)R=32;
						Area* AR=GetTopMap(NTOP,LTP);
						int DX=(OBJ->RealX>>4)-AR->x;
						int DY=(OBJ->RealY>>4)-AR->y;
						int NN=Norma(DX,DY);
						if(NN){
							DX=0;//DX*R/NN;
							DY=0;//DY*R/NN;
						};
						*/
						int ntp=-1;
						//if(dx>64)dx=64;
						//if(dx<-64)dx=-64;
						//if(dy>64)dy=64;
						//if(dy<-64)dy=-64;
						if(dx>64)dx&=63;
						if(dx<-64)dx=-((-dx)&63);
						if(dy>64)dy&=63;
						if(dy<-64)dy=-((-dy)&63);
						do{
							dx=dx*3/4;
							dy=dy*3/4;
							int cx=xx[p]-dx;
							int cy=yy[p]+dy;
							int t1=GetWTopology(cx-40,cy-40,LTP);
							int t2=GetWTopology(cx+40,cy-40,LTP);
							int t3=GetWTopology(cx-40,cy+40,LTP);
							int t4=GetWTopology(cx+40,cy+40,LTP);
							if(t1==t2&&t2==t3&&t3==t4)ntp=t1;else ntp=CTOP;
						}while(ntp==CTOP&&(dx!=0||dy!=0));
						int xd=(xx[p]-dx)<<4;
						int yd=(yy[p]+dy)<<4;
						void CorrectUnitPositionBeforeSend(OneObject* OB,int& x,int& y);
                        CorrectUnitPositionBeforeSend(OBJ,xd,yd);
						OBJ->NewMonsterPreciseSendTo(xd,yd,OR1->PrioryLevel,1);
					};
					return;
				};
			};
		};
	};
	if(NextTop==0xFFFF){
		NextTop=GetWTopology(OBJ->RealX>>4,OBJ->RealY>>4,LTP);
		if(NextTop==0xFFFF){
			OBJ->DeleteLastOrder();
			return;
		};
	}

	if(CheckTopDirectWay(OBJ->RealX>>4,OBJ->RealY>>4,x+dx,y+dy,OBJ->LockType)){
		int prior=OR1->PrioryLevel;
		OBJ->DeleteLastOrder();
		OBJ->NewMonsterPreciseSendTo((x+dx)<<4,(y+dy)<<4,prior,1);
		return;
	};
	if(NextTop==0xFFFF){
		NextTop=GetWTopology(OBJ->RealX>>4,OBJ->RealY>>4,LTP);
		if(NextTop==0xFFFF){
			OBJ->DeleteLastOrder();
			return;
		};
	}else{
		//if(Norma(x-(OBJ->RealX>>4),y-(OBJ->RealY>4))>64){
		//	OBJ->NewMonsterSendTo(NextX<<4,NextY<<4,OR1->PrioryLevel,1);
		//	return;
		//};
	};
	if(FinalTop>=0xFFFE){
		OBJ->DeleteLastOrder();
		return;
	};
	OR1->info.SmartSend.x=x;
	OR1->info.SmartSend.y=y;
	int NA=GetNAreas(LTP);
	word NextNextTop=GetMotionLinks(FinalTop+NA*NextTop,LTP,OBJ->NNUM);
	if(NextNextTop==FinalTop||FinalTop==NextTop||NextNextTop==0xFFFF){
		int prio=OR1->PrioryLevel;
		OBJ->DeleteLastOrder();
		if(!FindSuperSmartBestPosition(OBJ,&x,&y,dx,dy,FinalTop,LTP,0)){
			OBJ->DeleteLastOrder();
			return;
		};
		OBJ->NewMonsterPreciseSendTo((x+dx)<<4,(y+dy)<<4,prio,1);
		return;
	};
	if(NextNextTop!=0xFFFF){
		//atttempt to optomise way
		int MaxPre=3;
		int cox=OBJ->RealX>>4;
		int coy=OBJ->RealY>>4;
		do{
			int Next2=GetMotionLinks(FinalTop+NA*NextNextTop,LTP,OBJ->NNUM);
			if(Next2!=0xFFFF){
				Area* AR2=GetTopMap(Next2,LTP);
				int nx=(int(AR2->x)<<6)+32;
				int ny=(int(AR2->y)<<6)+32;
				int rx=nx+dx;
				int ry=ny+dy;
				if(CheckTopDirectWay(cox,coy,rx,ry,OBJ->LockType)){
					NextNextTop=Next2;
					MaxPre--;
				}else MaxPre=0;
			}else MaxPre=0;
		}while(MaxPre);
		Area* NextAr=GetTopMap(NextNextTop,LTP);
		//------------------------
		int nx=(int(NextAr->x)<<6)+32;
		int ny=(int(NextAr->y)<<6)+32;
		int rx=nx+dx;
		int ry=ny+dy;
		//if(CheckBar((rx>>4)-2,(ry>>4)-2,4,4)){
			//search for better place
		int xx0=nx;
		int yy0=ny;
		if(!FindSuperSmartBestPosition(OBJ,&xx0,&yy0,dx,dy,NextNextTop,LTP,0)){
			OBJ->DeleteLastOrder();
			return;
		};
		rx=xx0;
		ry=yy0;
		//};
		OR1->info.SmartSend.NextX=rx;
		OR1->info.SmartSend.NextY=ry;
		OR1->info.SmartSend.NextTop=NextNextTop;
		if(Norma(OBJ->RealX/16-rx,OBJ->RealY/16-ry)<32){
			int prio=OBJ->LocalOrder->PrioryLevel;
			OBJ->DeleteLastOrder();
			OBJ->NewMonsterPreciseSendTo((x+dx)<<4,(y+dy)<<4,prio,1);
			return;
		}else{
			OBJ->NewMonsterSendTo(rx<<4,ry<<4,OR1->PrioryLevel,1);
			return;
		}
	}else{
		OBJ->DeleteLastOrder();
		return;
	};
#endif //COSSACKS2
};
void TestObj(){
}
#undef NewMonsterSmartSendTo
void FindPositionForUnit(OneObject* OB);
bool CheckIfSomeoneMovesIntoThisUnit(OneObject* OB);
void OneObject::NewMonsterSmartSendTo(int x,int y,int dx,int dy,byte Prio,byte OrdType){
	if(LockedInBuilding!=0xFFFF)return;
	if(ActivityState==2){//aggressive
		Prio=128+1;
	}
	if(Lx>1){
		x+=dx;
		y+=dy;
		dx=0;
		dy=0;
	}
	if(!GetNAreas())return;
	if(NewBuilding||!Ready)return;
	if(!ext_OnAttemptToMove(this,x+dx,y+dy))return;
	if(CheckIfSomeoneMovesIntoThisUnit(this))return; 
	StandTime=0;
	int RR=Norma(dx,dy);
	int start=GetTopRef((RealX>>10)+(RealY>>10)*TopLx,LockType);
	int fin=GetTopRef((x>>6)+(y>>6)*TopLx,LockType);
	int TD;
	if(start==fin) TD=0;
		else TD=GetTopDistance(x>>6,y>>6,RealX>>10,RealY>>10,LockType,NNUM);//GetMotionLinks(start*GetNAreas(LockType)+fin,LockType);

	if(LockType!=1 && TD<80&&Norma(x-(RealX>>4),(y-(RealY>>4))>>1)<1100){
			if(ActivityState==2||!(Prio&127)){
				if(newMons->GetAnimation(anm_PMotionL)->Enabled)GroundState=1;
		}else{
			GroundState=0;
			CheckState();
		};
		x=(x+dx)<<4;
		y=(y+dy)<<4;
		
		int xx1=(x-(Lx<<7))>>8;
		int yy1=(y-(Lx<<7))>>8;
		int xx2=xx1;
		int yy2=yy1;

		bool FindBestPositionOLD(OneObject* OB,int* xd,int* yd,int R0);
		UnLockComplexObject(this);
		if(FindBestPositionOLD(this,&xx1,&yy1,40)){
			if(xx1!=xx2||yy1!=yy2){			
				x=(xx1<<8)+(Lx<<7);
				y=(yy1<<8)+(Lx<<7);
			}
		}
		LockComplexObject(this);
		NewMonsterPreciseSendTo(x,y,Prio,OrdType);
		return;
	}else{
		int xx=0;
	}

	if(RR>128){
		int dx1=(dx*120)/RR;
		int dy1=(dy*90)/RR;
		NewMonsterSmartSendTo(x+dx-dx1,y+dy-dy1,dx1,dy1,Prio,OrdType);
		return;
	};
	/*
	if(BrigadeID!=0xFFFF){
		Brigade* BR=Nat->CITY->Brigs+BrigadeID;
		if(BR->WarType){
			BR->HumanGlobalSendTo(x,y,LastDirection,Prio,OrdType);
			return;
		};
	};
	*/
	if(x<64)x=64;
	if(y<64)y=64;
	if(Prio<PrioryLevel)return;
	word Top=GetWTopology(&x,&y,LockType);
	if(Top==0xFFFF)return;
	addrand(1021);
	Order1* OR1=CreateOrder((OrdType&127)==0?3:(OrdType&127));
	if(!OR1)return;

	if(!(LockType||Sdoxlo||UnlimitedMotion)){
		if(CheckPt(this->x,this->y)&&!(OrdType&128)){
			//FindPositionForUnit(this);			
		};
	};
	//OrdType&=127;


	OR1->DoLink=(ReportFn*)(SmartLink-0x7654391);//&NewMonsterSmartSendToLink;
	OR1->OrderType=12;
	OR1->PrioryLevel=Prio&127;
	OR1->info.SmartSend.x=x;
	OR1->info.SmartSend.y=y;
	OR1->info.SmartSend.dx=dx;
	OR1->info.SmartSend.dy=dy;
	OR1->info.SmartSend.NextX=0xFFFF;
	OR1->info.SmartSend.NextY=0xFFFF;
	OR1->info.SmartSend.NextTop=0xFFFF;
	if(OrdType==0||OrdType==1)PrioryLevel=Prio&127;
	if(OR1->PrioryLevel==0)InPatrol=true;
	if(!(Prio&127)){
		if(newMons->GetAnimation(anm_PMotionL)->Enabled)GroundState=1;
	}else{
		GroundState=0;
		CheckState();
	};
};
#define NewMonsterSmartSendTo(x,y,dx,dy,Prio,OrdType) NewMonsterSmartSendTo(x,y,dx,dy,Prio,OrdType);addrand(55)
int CurTR=2;
int PreMID=0xFFFF;
#define DLR 3
int CheckPointForDamageAbility(OneObject* OBJ,int x,int y,int z);
void ProcessSelectedTower(){
	return;
	if(NSL[MyNation]==1){
		word MID=Selm[MyNation][0];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->Usage==TowerID){
				if(PreMID!=MID)CurTR=2;
				PreMID=MID;
				int N=Rarr[CurTR].N;
				char* xi=Rarr[CurTR].xi;
				char* yi=Rarr[CurTR].yi;
				int nn=0;
				int xc=OB->RealX>>10;
				int yc=OB->RealY>>10;
				int mpdx=mapx<<5;
				int mpdy=mapy<<4;
				int MLX=smaplx<<5;
				int MLY=smaply<<4;
				for(int p=0;p<N;p++){
					int xx=((xc+int(xi[p]))<<6)+32;
					int yy=((yc+int(yi[p]))<<6)+32;
					int zz=GetHeight(xx,yy);
					if(!CheckPointForDamageAbility(OB,xx,yy,zz+32)){
						nn++;
						int xs=xx-mpdx;
						int ys=(yy>>1)-mpdy-zz;
						if(xs>0&&ys>0&&xs<MLX&&ys<MLY){
							int ys1=((yy-DLR)>>1)-mpdy-GetHeight(xx-DLR,yy-DLR);
							int ys2=((yy-DLR)>>1)-mpdy-GetHeight(xx+DLR,yy-DLR);
							int ys3=((yy+DLR)>>1)-mpdy-GetHeight(xx-DLR,yy+DLR);
							int ys4=((yy+DLR)>>1)-mpdy-GetHeight(xx+DLR,yy+DLR);
							DrawLine(xs-DLR,ys1,xs+DLR,ys4,255);
							DrawLine(xs-DLR,ys3,xs+DLR,ys2,255);
						};
					};
				};
				if(nn||CurTR<12)CurTR++;else CurTR=2;
			};
		};
	};
};
//------------------------smart motion-----------------------
void CorrectTopPoint(int* cx,int* cy,int dx,int dy){
	if(!(dx||dy))return;
	int NR=Norma(dx,dy);
	int x0=(*cx)<<4;
	int y0=(*cy)<<4;
	int N=(NR>>6)+1;
	int sx=(dx<<4)/N;
	int sy=(dy<<4)/N;
	for(int i=0;i<N;i++){
		x0+=sx;
		y0+=sy;
		int tx=x0>>10;
		int ty=y0>>10;
		if(tx>0&&ty>0&&tx<TopLx&&ty<TopLy){
			int ofs=tx+(ty<<TopSH);
			if(GetTopRef(ofs)>=0xFFFE){
				x0-=sx;
				y0-=sy;
				*cx=x0>>4;
				*cy=y0>>4;
				return;
			};
		};
	};
};
void CorrectBlocking(int* cx,int* cy){
	if(!CheckBar(((*cx)>>4)-2,((*cy)>>4)-2,4,4))return;
	int top=GetTopology(*cx,*cy);
	if(top>=0xFFFE){
		//need to find free point with the same topology
		int tcx=(*cx)>>6;
		int tcy=(*cy)>>6;
		for(int r=1;r<10;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int N=Rarr[r].N;
			for(int j=0;j<N;j++){
				int tx=tcx+xi[j];
				int ty=tcy+yi[j];
				if(tx>0&&ty>0&&tx<TopLx&&ty<TopLy){
					int ofs=tx+(ty<<TopSH);
					if(GetTopRef(ofs)==top){
						if(!CheckBar((tx<<2),(ty<<2),4,4)){
							*cx=(tx<<6)+32;
							*cy=(ty<<6)+32;
						};
					};

				};
			};
		};
	};
};
bool CheckTopDirectWay(int x0,int y0,int x1,int y1,byte TopType){
	x0<<=8;
	y0<<=8;
	x1<<=8;
	y1<<=8;
	int dx=x1-x0;
	int dy=y1-y0;
	int N=(Norma(dx,dy)>>(8+6))+1;
	dx/=N;
	dy/=N;
	for(int i=0;i<N;i++){
		x0+=dx;
		y0+=dy;
		int tx=x0>>(8+6);
		int ty=y0>>(8+6);
		if(tx>0&&ty>0&&tx<TopLx&&ty<TopLy){
			if(GetTopRef(tx+(ty<<TopSH),TopType)>=0xFFFE)return false;
		}else return false;

	};
	return true;
};

int EnumUnitsInCell(int cell,int x,int y,int r,word Type,byte Nation){
	int N=0;
	if(cell>0){
		cell+=VAL_MAXCX+1;
		if(cell>=VAL_MAXCX*VAL_MAXCX)return 0;
		int NMon=MCount[cell];
		if(!NMon)return NULL;
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!(OB->Guard!=0xFFFF||OB->Sdoxlo))&&OB->BrigadeID==0xFFFF&&OB->NNUM==Nation&&OB->NIndex==Type&&Norma(OB->RealX-x,OB->RealY-y)<r)N++;
			};
		};
	};
    return N;
};
int EnumUnitsInRound(int x,int y,int r,word Type,byte Nation){
    int rx1=(r>>11)+1;
	int N=0;
	int x0=x>>11;
	int y0=y>>11;
	int maxx=msx>>2;
	for(int R=0;R<=rx1;R++){
		char* xi=Rarr[R].xi;
		char* yi=Rarr[R].yi;
		int NN=Rarr[R].N;
		for(int p=0;p<NN;p++){
			int xx=x0+xi[p];
			int yy=y0+yi[p];
			if(xx>=0&&yy>=0&&xx<maxx&&yy<maxx){
				int cell=xx+(yy<<VAL_SHFCX);
				N+=EnumUnitsInCell(cell,x,y,r,Type,Nation);
			};
		};
	};
	return N;
};
int CheckShipDirection(char Dir){
	word MinD=127;
	char BestD=0;
	if(abs(Dir-32)<MinD){
		BestD=32;
		MinD=abs(Dir-32);
	};
	if(abs(Dir+32)<MinD){
		BestD=-32;
		MinD=abs(Dir+32);
	};
	if(abs(Dir-96)<MinD){
		BestD=96;
		MinD=abs(Dir-86);
	};
	if(abs(Dir+96)<MinD){
		BestD=-96;
		MinD=abs(Dir+96);
	};
	char D=Dir-BestD;
	if(abs(D)<4)return 0;
	if(D>0)return -2;
	return 2;
};
void RotateShipAndDieLink(OneObject* OBJ){
	char DD=CheckShipDirection(OBJ->RealDir);
	if(DD==0){
		OBJ->ClearOrders();
		OBJ->MaxDelay=2000;
		OBJ->delay=2000;
		OBJ->Die();
		return;
	};
	OBJ->RealDir+=DD;
	OBJ->GraphDir=OBJ->RealDir;
};
void RotateShipAndDie(OneObject* OBJ){
	addrand(1024);
	Order1* OR1=OBJ->CreateOrder(0);
	OR1->DoLink=&RotateShipAndDieLink;
	OR1->PrioryLevel=255;
	OBJ->PrioryLevel=255;
	OBJ->RealVx=0;
	OBJ->RealVy=0;
	OBJ->Speed=0;
	OBJ->DestX=-1;
	OBJ->DestY=-1;
	NewMonster* NM=OBJ->newMons;
	if(NM->Destruct.NWeap){
		FogRec* FR=&NM->Destruct;
		if(FR->NWeap){
			for(int j=0;j<FR->NWeap;j++){
				int xp=(OBJ->RealX>>4)+(rando()%180)-90;
				int yp=(OBJ->RealY>>4)+(rando()%180)-90;
				int nw=j;//(rando()*FR->NWeap)>>15;
				addrand(71);
				Create3DAnmObject(WPLIST[FR->Weap[nw]],xp,yp,GetHeight(xp,yp)+60,xp,yp,1000,NULL,0,0xFFFF);
			};
		};
	};
};
///DEBUG
char LASTFILE[128];
int LastLine;
//byte GSets.CGame.cgi_NatRefTBL[8]={0,1,2,3,4,5,6,7};
void DoNormalTBL(){
	for(int i=0;i<8;i++)GSets.CGame.cgi_NatRefTBL[i]=i;
};
void ProcessGuard(){
	if((tmtmt&127)==5){
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&(!OB->Sdoxlo)){
				word GUID=OB->Guard;
				if(GUID!=0xFFFF){
					OneObject* GUOB=Group[GUID];
					if(GUOB&&!GUOB->Sdoxlo){
						int R=Norma(OB->RealX-GUOB->RealX,OB->RealY-GUOB->RealY)>>4;
						if(GUOB->NewBuilding&&R>250){
							int vx=(GUOB->RealX-OB->RealX)>>4;
							int vy=(GUOB->RealY-OB->RealY)>>4;
							int NR=Norma(vx,vy);
							vx=(150*vx)/NR;
							vy=(150*vy)/NR;
							OB->NewMonsterSmartSendTo((GUOB->RealX>>4)-vx,(GUOB->RealY>>4)-vy,0,0,16+128,0);
							//OB->StandGround=1;
						};
						if((!GUOB->NewBuilding)&&R>150){
							int vx=(GUOB->RealX-OB->RealX)>>4;
							int vy=(GUOB->RealY-OB->RealY)>>4;
							int NR=Norma(vx,vy);
							vx=(64*vx)/NR;
							vy=(64*vy)/NR;
							OB->NewMonsterSmartSendTo((GUOB->RealX>>4)-vx,(GUOB->RealY>>4)-vy,0,0,16+128,0);
							//OB->StandGround=1;

						};
					}else{
						OB->Guard=0xFFFF;
					};
				};
			};
		};
	};
};
typedef void AnglActor(OneObject* OB,WeaponInSector* WS,byte Angle,int param);
bool CheckPtInSector(WeaponInSector* WS,byte Angle,int dx,int dy){
	int R=Norma(dx,dy);
	if(R<WS->RMin||R>WS->RMax)return false;
	char DD=GetDir(dx,dy)-Angle;
	int dang=abs(int(DD));
	if(dang>WS->Angle)return false;
	return true;
};
int GetPtDamageInSector(WeaponInSector* WS,byte Angle,int dx,int dy){
	int R=Norma(dx,dy);
	if(R<WS->RMin||R>WS->RMax)return false;
	char DD=GetDir(dx,dy)-Angle;
	int dang=abs(int(DD));
	if(dang>WS->Angle)return 0;
	int DAM=WS->MaxDamage-(R-WS->RMin)*(WS->MaxDamage-WS->MinDamage)/(WS->RMax-WS->RMin);
	return DAM*(100-dang*(100-WS->AnglFactor)/WS->Angle)/100;
};
void ActInSector(int x,int y,WeaponInSector* WS,byte Angle,AnglActor* ANGA,int param){
	int MAX=WS->RMax;
	if(MAX>1600)MAX=1600;
	int RM=(MAX+256)>>7;
	int RM2=RM+RM;
	int LRM=RM2+1;
	byte DamBuff[4096];
	//memset(DamBuff,0,(LRM*LRM));
	int X0=(x>>7)-RM;
	int Y0=(y>>7)-RM;
	int ofs=0;
	for(int iy=0;iy<LRM;iy++){
		for(int ix=0;ix<LRM;ix++){
			DamBuff[ofs]=CheckPtInSector(WS,Angle,((ix+X0)<<7)-x,((iy+Y0)<<7)-y);
			ofs++;
		};
	};
	ofs=0;
	for(int iy=0;iy<RM2;iy++){
		for(int ix=0;ix<RM2;ix++){
			if(DamBuff[ofs]||DamBuff[ofs+1]||DamBuff[ofs+LRM]||DamBuff[ofs+LRM+1]){
				int cx=X0+ix+1;
				int cy=Y0+iy+1;
				if(cx>=0&&cy>=0&&cx<VAL_MAXCX&&cy<VAL_MAXCX){
					int cell=cx+(cy<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&(!OB->Sdoxlo)){
									ANGA(OB,WS,Angle,param);
								};
							};
						};
					};
				};
			};
			ofs++;
		};
		ofs++;
	};
};
struct AngParam1{
	int NEnemy;
	int NFriends;
	byte NMask;
};
void CheckUnitsInAngle(OneObject* OB,WeaponInSector* WS,byte Angle,int param){
	AngParam1* v=(AngParam1*)param;
	if(OB->NMask&v->NMask)v->NFriends++;
	else v->NEnemy++;
};
bool CheckVictimInAngle(int x,int y,WeaponInSector* WS,byte Angle,byte Mask){
	AngParam1 APR;
	APR.NMask=Mask;
	APR.NEnemy=0;
	APR.NFriends=0;
	ActInSector(x,y,WS,Angle,&CheckUnitsInAngle,int(&APR));
	return APR.NEnemy>3&&APR.NEnemy>APR.NFriends*2;

};
struct AngParam2{
	OneObject* Sender;
	int AttType;
	int x,y;
};
void DamageInAngle(OneObject* OB,WeaponInSector* WS,byte Angle,int param){
	AngParam2* v=(AngParam2*)param;
	int Dam=GetPtDamageInSector(WS,Angle,(OB->RealX>>4)-v->x,(OB->RealY>>4)-v->y);
	if(Dam){
		OB->MakeDamage(Dam,Dam,v->Sender,v->AttType);
	};
};
void DamageUnitsInAngle(int x,int y,WeaponInSector* WS,byte Angle,OneObject* Sender,int AttType){
	AngParam2 APR;
	APR.Sender=Sender;
	APR.AttType=AttType;
	APR.x=x;
	APR.y=y;
	ActInSector(x,y,WS,Angle,&DamageInAngle,int(&APR));
};
NewMonster::~NewMonster(){
	//for(int i=0;i<NAttTypes;i++){
	//	if(DamWeap[i])delete(DamWeap[i]);
	//};
	if(CompxCraft){
		for(int i=0;i<MaxAStages;i++){
			if(CompxCraft->Stages[i].ExplCoor)free(CompxCraft->Stages[i].ExplCoor);
			for(int j=0;j<2;j++){
				if(CompxCraft->Stages[i].FireX[j])free(CompxCraft->Stages[i].FireX[j]);
				if(CompxCraft->Stages[i].FireY[j])free(CompxCraft->Stages[i].FireY[j]);
			};
		};
		delete(CompxCraft);
	};
	if(CompxUnit)delete(CompxUnit);
	if(ShotRes)free(ShotRes);
	if(LockX)free(LockX);
	if(LockY)free(LockY);
	if(CheckX)free(CheckX);
	if(CheckY)free(CheckY);
	if(BLockX)free(BLockX);
	if(BLockY)free(BLockY);
	if(Doors)free(Doors);
	if(PieceName)free(PieceName);

	BuildPtX.Clear();
	BuildPtY.Clear();
	ConcPtX.Clear();
	ConcPtY.Clear();
	CraftPtX.Clear();
	CraftPtY.Clear();
	DamPtX.Clear();
	DamPtY.Clear();
	BornPtX.Clear();
	BornPtY.Clear();

	//Drop.Clear();
	//ItemDrop.Clear();

	if(ShotPtX)free(ShotPtX);
	if(ShotPtY)free(ShotPtY);
	if(ShotPtYG)free(ShotPtYG);
	if(ShotPtZ)free(ShotPtZ);
	if(ShotDelay)free(ShotDelay);
	if(ShotAttType)free(ShotAttType);
	if(ShotDir)free(ShotDir);
	if(ShotDiff)free(ShotDiff);
	if(ShotMinR)free(ShotMinR);
	if(ShotMaxR)free(ShotMaxR);
	for(int i=0;i<2;i++){
		if(FireX[i])free(FireX[i]);
		if(FireY[i])free(FireY[i]);
	};
	if(Bars3D)free(Bars3D);
	if(VesloX)free(VesloX);
	if(VesloY)free(VesloY);
	/*if(MultiWpX)free(MultiWpX);
	if(MultiWpY)free(MultiWpY);*/
	if(HideTriX)free(HideTriX);
	if(HideTriY)free(HideTriY);
	if(WavePoints)free(WavePoints);
	if(FLAGS)free(FLAGS);
	if(WSECT)free(WSECT);
	if(Fogging.Weap)free(Fogging.Weap);
	if(Destruct.Weap)free(Destruct.Weap);
	if(Fire.Weap)free(Fire.Weap);
	if(MD_File)free(MD_File);
	if(Name)free(Name);
	ActiveBones.Clear();
	EmotIcons.Clear();
	ExIcon.Icons.Clear();
	if(Message)delete[](Message);
	if(LongMessage)delete[](LongMessage);
};
NewAnimation::~NewAnimation(){
	//if(ActivePtX)free(ActivePtX);
	//if(ActivePtY)free(ActivePtY);
	ActivePt.Clear();
	if(LineInfo)free(LineInfo);
	//if(Frames)delete[](Frames);
	Frames.Clear();
	for(int i=0;i<Frames.GetAmount();i++){
		if(i==0)delete[](Frames[0]);
		Frames[i]=NULL;
	}
	memset(this,0,sizeof*this);
};
//-----------------IMPLEMENTATION OF SINGLE STEP-BY-STEP MOTION-----------------//
int BSpeed=12;
int GameSpeed=256;
#define CHKXY addrand(OB->RealX);addrand(OB->RealY);
void MotionHandlerForSingleStepObjects(OneObject* OB){
#ifdef COSSACKS2
	APPLY_TIRING(OB);
	if(OB->DestX<=0)OB->StandTime++;
	NewMonster* NMN=OB->newMons;
	/*
#ifndef NOLITTLESHIFT
	if(OB->Attack&&!OB->FrameFinished&&!NMN->RotationAtPlaceSpeed)
	{
		void UnitLitleShift(OneObject* OB);
		if(OB->LockedInBuilding==0xFFFF&&OB->EnemyID!=0xFFFF){
            OneObject* EOB=Group[OB->EnemyID];
			if(EOB){
				int RM=(EOB->newMons->AddShotRadius+140)*16;
				if(EOB&&(Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)<RM||EOB->NewBuilding)){
					UnitLitleShift(OB);
				}
			}
		}
	}
#endif //NOLITTLESHIFT
	*/
	void RotUnit(OneObject* OB,char Dir,byte OrdType,byte);
	void RotUnitLink(OneObject* OB);	

    int d=(GameSpeed>>6)+1;
    if(OB->OverEarth>d)OB->OverEarth-=d;
    else{
        if(OB->OverEarth>=0)OB->OverEarth=0;
        else OB->OverEarth++;
    }
	
	if(OB->FrameFinished){
		if(OB->DestX<0&&!(OB->InMotion||OB->LocalOrder||OB->NewState)){
			if((tmtmt&15)!=9){
				return;
			}
		}
	}
	
	if(OB->RealDir!=(OB->RealDirPrecise>>8))OB->RealDirPrecise=word(OB->RealDir)<<8;

    OB->UsesDestDidection=false;

	//if(OB->BrigadeID==0xFFFF&&!(OB->LocalOrder || OB->LockedInBuilding!=0xFFFF || OB->PathDelay || OB->FrameFinished==false))OB->DestX=-1;
	if(OB->PathDelay>=FrmDec)OB->PathDelay-=FrmDec;
	else OB->PathDelay=0;	
	AdvCharacter* ADC=OB->MoreCharacter;
	if(NMN->WaterActive)SpotByUnit(OB->RealX,OB->RealY,35,OB->RealDir);
	bool CANNOWMOVE=OB->InMotion||OB->FrameFinished;
	bool FF=false;
	if(!CANNOWMOVE){
		if(OB->NewAnm->MoveBreak){
			if(OB->DestX>0)CANNOWMOVE=1;
		}
		if(OB->NewAnm->CanBeBrokenAfterFrame<=OB->NewCurSprite){
			if(OB->DestX>0||OB->NewState!=OB->LocalNewState)CANNOWMOVE=1;
		};
	};
	addrand(OB->RealDir);
	int DirAnm=0;
	if(CANNOWMOVE){		
		if(OB->DestX>0){
			addrand(OB->DestX);
			addrand(OB->DestY);
			addrand(OB->RealX);
			addrand(OB->RealY);
			addrand(OB->RealDir);
			OB->StandTime=0;
			NewMonster* NM=OB->newMons;
			int R=NM->MotionDist;
			addrand(R);
			int RInFrame=R;
#ifdef GETTIRED
			R=OB->GroupSpeed;
			R=R*MOptions.GlobalMapScale/256;
			addrand(R);
			if(OB->BackMotion)R=R*2/3;
			if(!R){
				OB->GroupSpeed=OB->newMons->MotionDist;
				OB->UnitSpeed=64;
				R=OB->GroupSpeed;
			}
			if(OB->BrigadeID!=0xFFFF){
				Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
				int sb=BR->UnitsSpeedBonus;//speed bonus
				if(sb<10)sb=10;
				if(sb!=100){
					R=(R*sb)/100;
				};
				if(BR->IsTired){
					int T=OB->GetTired;
					if(T<5000)R-=(R>>2);
				}
			}
#endif
			if(OB->CurUnitSpeed!=64){
				R=(R*int(OB->CurUnitSpeed))>>6;
				addrand(OB->CurUnitSpeed);
			};
			OB->CurUnitSpeed=OB->UnitSpeed;
			R=(R*NM->SpeedScale)>>8;
			int BaseR=R;
			R=(R*OB->MoreCharacter->Speed)/100;
			bool Stun=false;

			if(OB->ActiveAbility){
				R<<=5;
				OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(BaseR<<5,R,Stun);
				R>>=5;
			}	

			addrand(R);
			addrand(OB->RealDir);
			/*
			//checking TEXTURE
			int TX=(OB->RealX+256)>>9;
			int TY=(OB->RealY+256)>>9;
			int ofs=TX+TY*VertInLine;
			if(ofs>=0&&ofs<MaxPointIndex){
				int T=TexMap[ofs];
				if(TexFlags[T]&(256*64))R=(R*BSpeed)>>4;
			};
			addrand(R);
			*/
			if(NM->SpeedScaleOnTrees){
				if( MFIELDS[2].CheckPt(OB->x,OB->y) && !MFIELDS[0].CheckPt(OB->x,OB->y) ){
					R=(R*NM->SpeedScaleOnTrees)>>8;
				}
			}
			int st=OB->NewState;
			int AutoSpeedAnm=16;
			NewAnimation* GOANM=NULL;
			AutoSpeedAnm=NMN->Rate[0];
			if(st){
				int RAT=NMN->Rate[st-1];
				R=(R*RAT)>>4;
				int SPD=(int(OB->CurUnitSpeed)*RAT)>>4;
				if(AutoSpeedAnm>16){
					if(SPD>80){
						//R=(R*AutoSpeedAnm)>>4;
						RInFrame=(RInFrame*AutoSpeedAnm)>>4;
						GOANM=NMN->GetAnimation(anm_PMotionL);
					}else{
						GOANM=NMN->GetAnimation(anm_MotionL);
					};
				}else{
					RInFrame=(RInFrame*RAT)>>4;
				};
			}else{
				if(AutoSpeedAnm>16){
					if(OB->CurUnitSpeed>80){
						//R=(R*AutoSpeedAnm)>>4;
						RInFrame=(RInFrame*AutoSpeedAnm)>>4;
						GOANM=NMN->GetAnimation(anm_PMotionL);
					}else{
						GOANM=NMN->GetAnimation(anm_MotionL);
					};
				}
			};

			if(R>80){
				NewAnimation* RunAN=NMN->GetAnimation(anm_MotionR);
				if(RunAN&&RunAN->Enabled){
					GOANM=RunAN;
				}
			}

			addrand(R);
			addrand(OB->RealDir);
			
			int dx=OB->DestX-OB->RealX;
			int dy=OB->DestY-OB->RealY;
			int Nrm=Norma(dx,dy);
			//
#ifndef NOBOIDS
			if(MAXOBJECT<EngSettings.BoidsOffLimit&&OB->newMons->BoidsMoving&&(dx||dy)&&Nrm>16*16)
			{
				void BoidsSingleStep2(OneObject* OB,int &dx, int &dy, int &ChangeSpeed);
				int AddSpeed=0;
				BoidsSingleStep2(OB,dx,dy,AddSpeed);
				//int MS=R/3;
				//if(MS>16)MS=16;
				//if(AddSpeed>MS)AddSpeed=MS;
				//if(AddSpeed<-MS)AddSpeed=-MS;
				R+=AddSpeed;
				addrand(AddSpeed);
				addrand(dx);
				addrand(dy);
				if(R<1)R=1;
			}
			*/
#endif //NOBOIDS
			addrand(OB->RealDir);
			int RS=R;//<<SpeedSh;
			addrand(RS);
			RS=(RS*GameSpeed)>>8;
			addrand(RS);
			addrand(GameSpeed);
			OB->InMotion=1;			
			//			
			bool FixEnd=0;
			addrand(OB->RealDir);
			if(Nrm<RS){
				RS=Nrm;
				FixEnd=1;
			};
			addrand(OB->RealDir);
			if(Nrm){
				addrand(dx);
				addrand(dy);
				char BestDir=char(GetDir(dx,dy));
				if(OB->BackMotion)BestDir+=128;
				DWORD MoveDir=0xFFFFFFFF;
				// BestDir=(BestDir+8)&0xF0;//new!!!!				
				addrand(BestDir);
				char ddir=BestDir-OB->RealDir;
				addrand(ddir);
				addrand(OB->RealDir);
				if(abs(ddir)>32){
					//OB->CurUnitSpeed>>=1;
				};
				int mr=Stun?0:NMN->MinRotator;
				int mr1=(mr*GameSpeed)>>8;
				if(mr1<1)mr1=1;
				//if(Nrm<RS*4){
					//mr<<=1;
				//}
				if(abs(ddir)>16 && MFIELDS[OB->LockType].CheckBar(OB->x,OB->y,OB->Lx,OB->Lx)){
					RS=0;
				}
				addrand(OB->RealDir);
				addrand(OB->RealDirPrecise);
				//if(NMN->HaveRotateAnimations&&Nrm>120*16)mr=2;
				bool HaveRotAnm=NMN->HaveRotateAnimations && MFIELDS[OB->LockType].CheckBar(OB->x-1,OB->y-1,3,3)==false;
				int RotSpeed=OB->newMons->RotationAtPlaceSpeed;
				if(OB->BrigadeID!=0xFFFF&&(RotSpeed||HaveRotAnm)){					
					Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
					if(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD){
						RotSpeed=0;
						HaveRotAnm=false;
					}
				}
                bool isLock=false;//HaveRotAnm&&MFIELDS[OB->LockType].CheckBar(OB->x-1,OB->y-1,3,3);
                int adr=abs(int(ddir));
				if(adr<mr1+1){
					int rs=0;
					if(HaveRotAnm&&Nrm>120*16&&(!isLock)){
						int ccod=OB->NewAnm->Code;
						if(!(ccod==anm_RotateL||ccod==anm_RotateR)){
							OB->RealDir=(OB->RealDir+8)&0xF0;
							OB->RealDirPrecise=int(OB->RealDir)<<8;							
						}else goto DoRot;
						rs=1;
					}
					if(!rs){
						addrand(OB->RealDir);
						if(adr>1)OB->RealDir=BestDir;
						addrand(OB->RealDir);
						if(HaveRotAnm&&Nrm>120*16){
							OB->RealDir=(OB->RealDir+8)&0xF0;
							OB->RealDirPrecise=int(OB->RealDir)<<8;							
						}
					}
				}else{
DoRot:;
					if(HaveRotAnm&&Nrm>120*16&&(!isLock)){
						int ccod=OB->NewAnm->Code;
						bool nowr=ccod==anm_RotateL||ccod==anm_RotateR;
						if(!nowr){
							OB->RealDir=(OB->RealDir+8)&0xF0;
							OB->RealDirPrecise=int(OB->RealDir)<<8;	
							NewAnimation* NA1=NMN->GetAnimation(anm_RotateL);
							int cnf=OB->NewAnm->NFrames;
							if(!cnf)cnf=1;
							int cf;
							if(ddir<0)cf=((int(WORD(64*256-OB->RealDirPrecise))*NA1->NFrames)>>16)%cnf;
							else cf=((int(WORD(OB->RealDirPrecise-64*256))*NA1->NFrames)>>16)%cnf;
							if(cnf>1 && (cf-OB->NewCurSprite+(cnf<<4))%cnf>=(GameSpeed>>8)+1){
								ddir=0;
							}
						}
						if(ddir<0){
							if(ccod!=anm_RotateR){
								GOANM=NMN->GetAnimation(anm_RotateL);
								if(GOANM&&GOANM->NFrames){
									int dd=65536/GOANM->NFrames;
									dd=dd*RS/RInFrame;
									OB->RealDirPrecise-=dd;
									OB->RealDir=OB->RealDirPrecise>>8;
								}
							}else{
								GOANM=NMN->GetAnimation(anm_MotionL);
								if(GOANM){
									int fr=((abs(OB->TotalPath+100000)<<8)/RInFrame)%(int(GOANM->NFrames)<<8)>>8;
									int df=fr-(OB->NewCurSprite%GOANM->NFrames);
									OB->TotalPath-=df*RInFrame;									
								}
							}
						}else if(ddir>0){
							if(ccod!=anm_RotateL){
								GOANM=NMN->GetAnimation(anm_RotateR);
								if(GOANM&&GOANM->NFrames){
									int dd=65536/GOANM->NFrames;
									dd=dd*RS/RInFrame;
									OB->RealDirPrecise+=dd;
									OB->RealDir=OB->RealDirPrecise>>8;
								}
							}else{
								GOANM=NMN->GetAnimation(anm_MotionL);
								if(GOANM){
									int fr=((abs(OB->TotalPath+100000)<<8)/RInFrame)%(int(GOANM->NFrames)<<8)>>8;
									int df=fr-(OB->NewCurSprite%GOANM->NFrames);
									OB->TotalPath-=df*RInFrame;									
								}
							}
						}else{
							if(nowr){
								GOANM=NMN->GetAnimation(anm_MotionL);
								if(GOANM){
									int fr=((abs(OB->TotalPath+100000)<<8)/RInFrame)%(int(GOANM->NFrames)<<8)>>8;
									int df=fr-(OB->NewCurSprite%GOANM->NFrames);
									OB->TotalPath-=df*RInFrame;									
								}
							}
						}
						DirAnm=ddir;
					}else{
						if(RotSpeed){							
                            RotUnit(OB,BestDir,1,OB->PrioryLevel);
							return;
						}else{
							//if(mr>8){
							//	RS=0;
							//	if(ddir<0)OB->RealDir-=mr;
							//	else OB->RealDir+=mr;
							//	addrand(OB->RealDir);
							//	OB->RealDirPrecise=word(OB->RealDir)<<8;
							//}else{
								int sp=mr*GameSpeed;
								if(Nrm<RS*64)MoveDir=BYTE(BestDir);
								//int minsp=120*256*RS/(Nrm+1);
								//if(minsp>(GameSpeed*8))minsp=(GameSpeed*8);
								//if(sp<minsp)sp=minsp;
								if(adr<=(sp>>8))OB->RealDirPrecise=int(BYTE(BestDir))<<8;
								else if(ddir<0)OB->RealDirPrecise-=sp;
								else OB->RealDirPrecise+=sp;
								addrand(OB->RealDir);
								OB->RealDir=OB->RealDirPrecise>>8;
								addrand(OB->RealDir);
							//}
						}
					}
				}
				addrand(OB->RealDirPrecise);
				addrand(OB->RealDir);
				OB->GraphDir=OB->RealDir;
				addrand(OB->RealDir);
				word ENMID=0xFFFF;
				addrand(OB->LocalNewState);
				addrand(OB->NewState+1);
				if(OB->LocalNewState==OB->NewState){
					if(GOANM)
						OB->NewAnm=GOANM;
					else{
						if(OB->NewState){
							//OB->NewAnm=&NMN->PMotionL[0];
							OB->NewAnm=NMN->GetAnimation(anm_PMotionL+OB->NewState-1);
							if(!OB->NewAnm->Enabled){
								OB->NewAnm=NMN->GetAnimation(anm_PMotionL);
								if(!OB->NewAnm->Enabled){
									OB->NewAnm=NMN->GetAnimation(anm_MotionL);
								}
							}
						}else OB->NewAnm=NMN->GetAnimation(anm_MotionL);
					};
					int rx1,ry1;
					CHKXY;
					if(!FixEnd){
						CHKXY;
						addrand(OB->RealDir);
						addrand(RS);
						addrand(TCos[OB->RealDir]);
						if(MoveDir==0xFFFFFFFF)MoveDir=OB->RealDir;
						if(!OB->BackMotion){
							rx1=OB->RealX+((RS*TCos[MoveDir])>>8);
							ry1=OB->RealY+((RS*TSin[MoveDir])>>8);
						}else{
							rx1=OB->RealX+((RS*TCos[byte(MoveDir+128)])>>8);
							ry1=OB->RealY+((RS*TSin[byte(MoveDir+128)])>>8);
						}
						CHKXY;
						addrand(OB->RealDir);
						addrand(RS);
						addrand(TCos[OB->RealDir]);
					}else{
						CHKXY;
						rx1=OB->DestX;
						ry1=OB->DestY;
						CHKXY;
					};
					/*if((!NM->DontStuckInEnemy)&&UnitsField.CheckBar(OB->x-1,OB->y-1,OB->Lx+2,OB->Lx+2)){
						ENMID=CheckMotionThroughEnemyAbility(OB,rx1,ry1);
					};*/
					OB->RealX=rx1;
					OB->RealY=ry1;
					CHKXY;
					OB->TotalPath+=OB->BackMotion?-RS:RS;
					UnitsField.BClrBar(OB->x,OB->y,OB->Lx);
					OB->x=(OB->RealX-((OB->Lx)<<7))>>8;
					OB->y=(OB->RealY-((OB->Lx)<<7))>>8;					
					UnitsField.BSetBar(OB->x,OB->y,OB->Lx);
					/*if(ENMID!=0xFFFF&&!OB->NoSearchVictim){
						if(OB->newMons->LowCollision&&OB->ActivityState!=2){
                            OB->CurUnitSpeed=20;                            
						}else{
							OB->AttackObj(ENMID,128+15,1,1);
						}
					}else{
						if(OB->newMons->LowCollision){
							OB->CurUnitSpeed=64;
						}
					}*/
				}else{
					addrand(77);
					TryToStand(OB,0);
					CANNOWMOVE=0;
				};
			}else{
				TryToStand(OB,0);
				CANNOWMOVE=0;
			}
			//BlockIncFrames=1;
			if(CANNOWMOVE){
				if(OB->NewAnm->NFrames){
					int code=OB->NewAnm->Code;
					if(code==anm_RotateL){
						if(MFIELDS[OB->LockType].CheckBar(OB->x-1,OB->y-1,3,3)==false){
							OB->CurrentFrameLong=(int(WORD(64*256-OB->RealDirPrecise))*OB->NewAnm->NFrames)>>8;
							OB->NewCurSpritePrev=OB->NewCurSprite;
							OB->NewCurSprite=OB->CurrentFrameLong>>8;                        
						}else{
							TryToStand(OB,0);
							CANNOWMOVE=0;
						}
					}else
					if(code==anm_RotateR){
						if(MFIELDS[OB->LockType].CheckBar(OB->x-1,OB->y-1,3,3)==false){
							OB->CurrentFrameLong=(int(WORD(OB->RealDirPrecise-64*256))*OB->NewAnm->NFrames)>>8;
							OB->NewCurSpritePrev=OB->NewCurSprite;
							OB->NewCurSprite=OB->CurrentFrameLong>>8;                        
						}else{
							TryToStand(OB,0);
							CANNOWMOVE=0;
						}
					}else{
						int nf=OB->NewAnm->NFrames;
						if(nf==0)nf=1;
						int rf=RInFrame;
						if(!rf)rf=1;

						word Scale=OB->ObjectScale;
						if(OB->ActiveAbility) OB->ActiveAbility->ActiveAbilities.modifyScale(OB->newMons->StartScale,Scale);

						OB->CurrentFrameLong=((abs(OB->TotalPath+100000)*(65536/(Scale+1)))/rf)%(int(nf)<<8);
						if(OB->TotalPath>100000)OB->TotalPath=1000;
						OB->NewCurSpritePrev=OB->NewCurSprite;
						OB->NewCurSprite=OB->CurrentFrameLong>>8;
						if(OB->NewCurSpritePrev>OB->NewCurSprite)FF=true;
					}
				}else OB->SetZeroFrame();
				//void AddSmokeOnHorseStep(OneObject* OB);
				//if(R>70)AddSmokeOnHorseStep(OB);
			}else{
				OB->NewCurSpritePrev=OB->NewCurSprite;
				if(!Stun)OB->SetNextFrame();
				if(OB->FrameFinished)FF=true;
				if(OB->NewAnm->NFrames)OB->NewCurSprite=OB->NewCurSprite%OB->NewAnm->NFrames;
				else OB->SetZeroFrame();
			}
		}else{
			if(!(OB->LocalOrder&&OB->LocalOrder->OrderType==12)){
				OB->InMotion=0;
				TryToStand(OB,1);
				OB->CurUnitSpeed=0;
			};
		};
	}else{
		OB->NewCurSpritePrev=OB->NewCurSprite;
		OB->SetNextFrame();
		if(OB->NewAnm->NFrames)OB->NewCurSprite=OB->NewCurSprite%OB->NewAnm->NFrames;
		else OB->SetZeroFrame();
		OB->CurUnitSpeed=0;
	};
	if(OB->DestX>0 && OB->BrigadeID==0xFFFF && (OB->FrameFinished || FF) && !(OB->LocalOrder || OB->LockedInBuilding!=0xFFFF || OB->PathDelay ))OB->DestX=-1;
	if(OB->RealDir!=(OB->RealDirPrecise>>8))OB->RealDirPrecise=word(OB->RealDir)<<8;
	if(OB->StandTime<16){
        int H=GetUnitHeight((OB->RealX)>>4,(OB->RealY)>>4);
        int DH=H-OB->RZ;
        int adh=DH<0?-DH:DH;
        int D=(adh>>3)+1;        
        if(OB->RZ<H)OB->RZ+=D;
        if(OB->RZ>H)OB->RZ-=D;		
		if(OB->RZ<2)OB->RZ=2;
	}
	//OB->RealDirPrecise=word(OB->RealDir)<<8;
#endif //COSSACKS2
};
int GetDamFall(int x,int x0,int Dam);
int GetDamage(word ShooterID,word VictimID,int AttType){
	if(ShooterID==0xFFFF||VictimID==0xFFFF)return 0;
	OneObject* Victim=Group[VictimID];
	if(!Victim)return 0;
	OneObject* Shooter=Group[ShooterID];
	if(!Shooter)return 0;
	int R=Norma(Shooter->RealX-Victim->RealX,Shooter->RealY-Victim->RealY)>>4;
	if((AttType)&&(R>Shooter->MoreCharacter->AttackRadius2[AttType]))return 0;
	int DAM0=Shooter->MoreCharacter->MaxDamage[AttType];
	if(AttType)
		DAM0=GetDamFall(R,Shooter->newMons->DamageDecr[AttType],DAM0);
	return Victim->MakeDamage(DAM0,DAM0,Shooter,AttType,0);
};
int GetSpritesInCell(int cell,int x,int y,int r,int* buf,int maxs,int GrpIndex){
	if(cell<0||cell>=VAL_SPRSIZE)return 0;
	int* CEL=SpRefs[cell];
	int   NCEL=NSpri[cell];
	if(!(CEL&&NCEL))return 0;
	int ns=0;
	for(int i=0;i<NCEL&&maxs;i++){
		OneSprite* OS=&Sprites[CEL[i]];
		//assert(OS->Enabled);
		ObjCharacter* OC=OS->OC;
		if((OC->GroupIndex==GrpIndex||GrpIndex==-1)&&Norma(OS->x-x,OS->y-y)<r){
			buf[0]=CEL[i];
			maxs--;
			buf++;
			ns++;
		}
	}
	return ns;
};
int GetSpritesInArea(int x,int y,int r,int* buf,int maxs,int GrpIndex=-1){
	int nr=(r>>7)+1;
	int nr1=nr+nr+1;
	int cell=(x>>7)-nr+(((y>>7)-nr)<<SprShf);
	int nt=0;
	for(int ix=0;ix<nr1;ix++)
		for(int iy=0;iy<nr1;iy++){
			int N=GetSpritesInCell(cell+ix+(iy<<SprShf),x,y,r,buf,maxs,GrpIndex);
			buf+=N;
			maxs-=N;
			nt+=N;
		}
		return nt;
};
bool CheckPointVisibility(int x,int y){
	int Y0=(y/2)-GetHeight(x,y);
	for(int i=0;i<30;i++){
        y+=64;
		int Y1=(y/2)-GetHeight(x,y);
		if(Y0>Y1-16)return false;
	}
	return true;
}
void SetCurrentProgress(char* Message,int Stage,int maxStage);
void RearrangeSprites(){
	/*
	int SprIdxs[512];
	for(int i=0;i<MaxSprt;i++){
		if(!(i&1023))SetCurrentProgress("Sprites",i,MaxSprt);
		OneSprite* OS=&Sprites[i];
		if(OS->Enabled){
			if(CheckPointVisibility(OS->x,OS->y)){
				int x0=OS->x;
				int y0=OS->y;
				int n=GetSpritesInArea(x0,y0,700,SprIdxs,512,OS->OC->GroupIndex);
				if(n>1){
					int gidx=OS->OC->GroupIndex-1;
					if(gidx>=0){
						SprGroup* SG=OS->SG;
						int GrpWeight[256];
						int NInGrp=SG->NInSect[gidx];
						if(NInGrp>256)NInGrp=256;
						memset(GrpWeight,0,NInGrp*4);
						for(int j=0;j<n;j++){
							OneSprite* OS1=&Sprites[SprIdxs[j]];
							ObjCharacter* OC1=OS1->OC;
							if(OC1->IndexInGroup<NInGrp){
								int R=Norma(OS1->x-x0,OS1->y-y0);
								if(R>0)GrpWeight[OC1->IndexInGroup]+=20000000/(R*R*R+30);
							}
						}
						int Candidates[256];
						int NCand=0;
						int MinW=100000;
						int bidx=-1;
						for(int j=0;j<NInGrp;j++){
							if(GrpWeight[j]==0){
								Candidates[NCand]=j;
								NCand++;
							}
							if(GrpWeight[j]<MinW){
								MinW=GrpWeight[j];
								bidx=j;
							}
						}
						if(NCand){
							bidx=Candidates[(NCand*rand())>>15];
						}
						int ocidx=SG->SectContence[gidx][bidx];
						OS->SGIndex=ocidx;
						OS->OC=OS->SG->Objects[ocidx];
					}
				}
			}else{
				void EraseSprite(int Index);
				EraseSprite(i);
			}
		}
	}	
	*/
}
//special ability for flying objects
class FlyBodyParams;
class BodyPositon{
public:
    Vector3D Pos;    
    word     Rotation;
    Vector3D Speed;
    //bottom params
    Vector3D BottomPos;
    Vector3D BottomSpeed;
    int      Time;
    void UpdateBottom(float dt,float WireLength,FlyBodyParams* FB);
};


#define nBodyPos 3
class uaFlyBodyParams:public UnitAbility{
public:
    float Acceleration;
    float ZAcceleration;
    float BreakModulator;
    float WireLength;
    float AirFriction;
    float Gravity;
    float AngleX_modulator;
    float AngleY_modulator;
	bool  DontAllowStrafe;
	bool  AllowBackMotion;
	bool  TankStyleAnimation;
	float MotionNoiseStrength;


    SAVE(uaFlyBodyParams);
        REG_PARENT(UnitAbility);
        REG_MEMBER(_float,Acceleration);
        REG_MEMBER(_float,ZAcceleration);
        REG_MEMBER(_float,BreakModulator);
        REG_MEMBER(_float,WireLength);
        REG_MEMBER(_float,AirFriction);
        REG_MEMBER(_float,Gravity);
        REG_MEMBER(_float,AngleX_modulator);
        REG_MEMBER(_float,AngleY_modulator);
		REG_MEMBER(_bool,DontAllowStrafe);
		REG_MEMBER(_bool,AllowBackMotion);
		REG_MEMBER(_bool,TankStyleAnimation);
		REG_MEMBER(_float,MotionNoiseStrength);
    ENDSAVE;

    __constructor(uaFlyBodyParams){
        Acceleration=1;
        ZAcceleration=1;
        BreakModulator=2;
        WireLength=60;
        AirFriction=1.0;
        Gravity=40.0f;
        AngleX_modulator=1.0;
        AngleY_modulator=1.0;
		DontAllowStrafe=false;
		AllowBackMotion=false;
		TankStyleAnimation=false;
		MotionNoiseStrength=0.0f;
    }
    virtual ActiveUnitAbility* GetNewActiveAbility();
};
class FlyBodyParams:public ActiveUnitAbility{
public:
    BodyPositon Pos[nBodyPos];
    bool        PosInit;
    float       Mass;
    float       MaxSpeed;
    float       Acceleration;
    float       ZAcceleration;
    float       BreakModulator;
    float       WireLength;
    float       AirFriction;
    float       Gravity;
    float       AngleX_modulator;
    float       AngleY_modulator;
	bool		DontAllowStrafe;
	bool		AllowBackMotion;
	bool		TankStyleAnimation;
	float		MotionNoiseStrength;
    void        Init();
    void        ProcessTo(int DestTime);
    bool        Process(OneObject* OB);
    SAVE(FlyBodyParams);
        REG_MEMBER(_float,Mass);
        REG_MEMBER(_float,Acceleration);
        REG_MEMBER(_float,ZAcceleration);
        REG_MEMBER(_float,BreakModulator);
        REG_MEMBER(_float,WireLength);
        REG_MEMBER(_float,AirFriction);
        REG_MEMBER(_float,Gravity);
        REG_MEMBER(_float,AngleX_modulator);
        REG_MEMBER(_float,AngleY_modulator);
		REG_MEMBER(_bool,DontAllowStrafe);
		REG_MEMBER(_bool,AllowBackMotion);
		REG_MEMBER(_bool,TankStyleAnimation);
		REG_MEMBER(_float,MotionNoiseStrength);
    ENDSAVE;
};
void reg_uaFlyBodyParams(){
	REG_CLASS(uaFlyBodyParams);
	REG_CLASS(FlyBodyParams);
}
ActiveUnitAbility* uaFlyBodyParams::GetNewActiveAbility(){
    FlyBodyParams* nAA=new FlyBodyParams;
    nAA->Mass=1;
    nAA->Acceleration=Acceleration;
    nAA->ZAcceleration=ZAcceleration;
    nAA->BreakModulator=BreakModulator;
    nAA->WireLength=WireLength;
    nAA->AirFriction=AirFriction;
    nAA->Gravity=Gravity;
    nAA->AngleX_modulator=AngleX_modulator;
    nAA->AngleY_modulator=AngleY_modulator;
    nAA->PosInit=false;
	nAA->DontAllowStrafe=DontAllowStrafe;
	nAA->AllowBackMotion=AllowBackMotion;
	nAA->TankStyleAnimation=TankStyleAnimation;
	nAA->MotionNoiseStrength=MotionNoiseStrength;	
    nAA->SetA(this);
    return nAA;
}    
void BodyPositon::UpdateBottom(float dt,float WireLength,FlyBodyParams* FB){
    Vector3D N=BottomPos;
    N-=Pos;
    N.normalize();
    BottomPos=Pos+N*WireLength;

    float G=FB->Gravity;
    float Frict=FB->AirFriction;
    BottomSpeed=Speed+(BottomSpeed-Speed)*exp(-dt*Frict);
    BottomSpeed.z+=G*dt;
    Vector3D vr=BottomSpeed-Speed;
    vr-=N*N.dot(vr);
    vr+=Speed;
    BottomSpeed=vr;
	Vector3D V((rando()&32)-16,(rando()&32)-16,(rando()&32)-16);
	V.normalize();	
	BottomSpeed+=V*FB->MotionNoiseStrength*Speed.norm()*0.05;

    BottomPos+=BottomSpeed*dt;
}
void FlyBodyParams::Init(){
    OneObject* OB=Group[GetUnitIndex()];
    if(OB){
        BodyPositon P;
        P.Pos=Vector3D(OB->RealX/16.0f,OB->RealY/16.0f,OB->RZ+OB->OverEarth);
        P.Rotation=OB->RealDirPrecise;
        P.Speed=Vector3D::null;
        P.BottomSpeed=Vector3D::null;
        P.BottomPos=P.Pos;
        P.BottomPos.z-=WireLength;        
        P.Time=TrueTime;
        for(int i=0;i<nBodyPos;i++){
            Pos[i]=P;
            P.Time-=100;
        }
        PosInit=true;
    }
}
extern bool NOPAUSE;
bool FlyBodyParams::Process(OneObject* OB){    
    return true;
}
void FlyBodyParams::ProcessTo(int DestTime){
    OneObject* OB=Group[GetUnitIndex()];
    if(OB){
        for(int i=nBodyPos-1;i>0;i--){
            Pos[i]=Pos[i-1];
        }        
        //updating body position
        int tp=Pos[1].Time;
        int dt=DestTime-tp;
        if(dt>0){
            int _maxspeed=OB->GroupSpeed;
			NewMonster* NM=OB->newMons;
			/*if(NM->MotionStyle!=8)*/_maxspeed=(_maxspeed*NM->SpeedScale)>>8;
			int BaseR=_maxspeed;
			_maxspeed=(_maxspeed*OB->MoreCharacter->Speed)/100;
			bool Stun=false;

			if(OB->ActiveAbility){
				_maxspeed<<=5;
				OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(BaseR<<5,_maxspeed,Stun);
				_maxspeed>>=5;
			}	
            
            if(OB->ActiveAbility){                
                OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(_maxspeed,_maxspeed,Stun);                
            }	
            float maxspeed=_maxspeed;
            float rdt=dt/40.0f/16.0f;
            BodyPositon& bp=Pos[0];
            bp.Pos+=bp.Speed*rdt;            
            float sp=bp.Speed.norm();
            bool SlowFrame=false;
            //speed processing
            int DestX=OB->DestX;
            int DestY=OB->DestY;
            bool UseDestDir=false;
            if(OB->DestDirectionIsActive){
                UseDestDir=true;
                if(DestX=-1){
                    DestX=OB->RealX;
                    DestY=OB->RealY;
                }
                //OB->DestDirectionIsActive=false;
            }
            if(DestX>0 && !(OB->Sdoxlo||Stun)){
				float spz=bp.Speed.z;
				bp.Speed.z=0;
                Vector3D dest(DestX/16.0f,DestY/16.0f,bp.Pos.z);
                dest-=bp.Pos;
                float r=dest.norm();
				Vector3D Vdst=dest;
				Vdst.z=0;
				Vdst.normalize();
				float nr=bp.Speed.norm();
				float accel=Acceleration;
				if(r<maxspeed*2){
					float mp=r/maxspeed/2;
                    maxspeed*=mp;
				}
				//float r1=nr/accel;
				//if(r<r1){
				//	if(r<r1/BreakModulator)accel=-accel;
				//	else accel=0;					
				//}
				bp.Speed+=Vdst*accel*dt;
				nr=bp.Speed.norm();
				if(nr>maxspeed){
					bp.Speed*=maxspeed/nr;
				}
				float ang=OB->RealDirPrecise*c_PI*2/65536.0f;
				Vector3D Vdir(cos(ang),sin(ang),0);				
				Vector3D VCurr=bp.Speed;				
				VCurr.normalize();
				float rotator=OB->newMons->MinRotator;  
				int rs=rotator*dt*256/40/4;
				word wang=atan2(bp.Speed.y,bp.Speed.x)*32768/c_PI;
				if(UseDestDir){
					wang=int(OB->DestDirection)<<8;
				}
				short da=OB->RealDirPrecise-wang;
				int dam=abs(da)+1024;
				if(dam>4096)dam=4096;
				rs=1+(rs*dam)/4096;
				if(abs(da)<rs){
					OB->RealDirPrecise=wang;
				}else{
					if(da<0){
						OB->RealDirPrecise+=rs;
					}else{
						OB->RealDirPrecise-=rs;
					}
				}
				if(DontAllowStrafe){
					ang=OB->RealDirPrecise*c_PI*2/65536.0f;
					Vector3D Vdir(cos(ang),sin(ang),0);
					bp.Speed=Vdir*bp.Speed.norm();					
				}
				bp.Speed.z=spz;
			}else{
				if(OB->Sdoxlo)bp.Speed*=exp(-rdt);
                else bp.Speed*=exp(-rdt*Acceleration*BreakModulator);
			}
			/*
                float RA=maxspeed*6;
                float RB=2;//maxspeed*2;
                float RC=1;//maxspeed;
                float ang=OB->RealDirPrecise*c_PI*2/65536.0f;
                Vector3D Vdir(cos(ang),sin(ang),0);
                Vector3D Vdst=dest;
                Vdst.normalize();
                Vector3D V;
				//direction processing
				word destdir;				
				if(UseDestDir && r<=RB){
					destdir=word(OB->DestDirection)<<8;
				}else{
					destdir=int(GetDir(dest.x*256,dest.y*256))<<8;
				}                
                float rotator=OB->newMons->MinRotator;  
				rotator*=RA/(r+RA/4);
				if(rotator>32)rotator=32;

				int rs=rotator*dt*256/40/4;
				short ddir=destdir-OB->RealDirPrecise;
				int adir=abs(ddir);
				bool InvDir=false;
				if(AllowBackMotion && r<420){
					if(abs(adir)>22000){
						destdir+=32768;
						ddir=destdir-OB->RealDirPrecise;
						adir=abs(ddir);
						sp=-sp;
						InvDir=true;
					}
				}
				if(DontAllowStrafe){
					V=Vdir;					
					float maxsp=(r-8);
					if(maxsp<maxspeed/3)maxsp=maxspeed/3;
					if(sp>maxsp)sp=maxsp;
					if(sp<-maxsp)sp=-maxsp;
					if(r>0){
						float mod=float(adir)/r/10;
						if(mod<1)mod=1;
						if(mod>5)mod=5;
						rs*=mod;
					}
				}else{
					V=Vdir;					
				}
                if(r<RC){
                    sp*=exp(-Acceleration*rdt);
					if(sp<0){
						if(sp>-maxspeed/4 && r>1)sp=-maxspeed/4;
					}else{
						if(sp<maxspeed/4 && r>1)sp=maxspeed/4;
					}
                }else{
					if(InvDir)sp-=Acceleration*maxspeed*dt/256;
                    else sp+=Acceleration*maxspeed*dt/256;                    
                }                
                int ang0=512;
                if(UseDestDir)ang0=rs;
                if(adir>ang0){                    
                    if(UseDestDir)rs=rs*3;
                    else rs=rs+rs*(32768-adir)/16384/2;
                    if(adir>rs){
                        if(ddir>0){
                            OB->RealDirPrecise+=rs;
                            if(adir>8192 && r>RB)SlowFrame=true;
                        }else{
                            OB->RealDirPrecise-=rs;
                            if(adir>8192 && r>RB)SlowFrame=true;
                        }
					}else{
						if(UseDestDir)OB->RealDirPrecise=destdir;
					}
                }else{
                    if(UseDestDir)OB->RealDirPrecise=destdir;
                }
                if(sp>maxspeed)sp=maxspeed;
				if(sp<-maxspeed)sp=-maxspeed;
                bp.Speed=V*sp;
            }else{
                if(sp<maxspeed/6)sp=0;
                if(OB->Sdoxlo)sp*=exp(-rdt);
                else sp*=exp(-Acceleration*BreakModulator*rdt);
                if(sp>0){
                    bp.Speed.normalize();
                    bp.Speed*=sp;
                }else{
                    bp.Speed=Vector3D::null;
                }
            }
			*/
            int zc=GetTotalHeight(OB->RealX>>4,OB->RealY>>4);
            if(zc<0 && !OB->Sdoxlo)zc=0;
            float dz=bp.Pos.z-zc;
			int zdiff=8;
			if(zdiff>OB->newMons->StartFlyHeight)zdiff=OB->newMons->StartFlyHeight;
            if(dz<zdiff && !OB->Sdoxlo)bp.Pos.z=zc+zdiff;
            pUnit uu(OB);
            iVector2D vd=uu.GetGlobalDesination();
            int d;
            if(vd.x==-1)d=0;
            else d=uu.DistTo(vd);
            int HD=OB->newMons->StartFlyHeight+d/2;
            if(HD>OB->newMons->FlyHeight)HD=OB->newMons->FlyHeight;
            if(Stun && HD>32)HD=32;
            if(OB->Sdoxlo)HD=0;
            HD+=zc;
            float dzc=fabs(bp.Pos.z-HD);
            if(OB->Sdoxlo){
                HD=zc;
                //bp.Speed*=exp(-rdt);
                if(dz>10)bp.Speed.z-=dt/3;
                else bp.Speed.z*=exp(-dt*3.0f);
                bp.BottomSpeed=bp.Speed;
                bp.BottomPos=bp.Pos;
                bp.BottomPos.z-=WireLength;
            }            
            float dzcc=dzc/64;
            if(dzcc>1)dzcc=1;
            if(OB->Sdoxlo)dzcc=3;
            float dzz=dzcc*float(dt)/16;            
            if(dzc>dzz){
                if(bp.Pos.z<HD)bp.Pos.z+=dzz;
                else bp.Pos.z-=dzz;
            }else{
                bp.Pos.z=HD;
                if(OB->Sdoxlo)bp.Speed.z=0;
            }			
            OB->RealX=bp.Pos.x*16;
            OB->RealY=bp.Pos.y*16;
            OB->RZ=zc;
			if(AllowBackMotion){
				OB->RZ=GetTotalHeight(OB->RealX>>4,OB->RealY>>4);//bp.Pos.z;
				OB->OverEarth=0;
			}else{
				addrand(OB->RZ);
				OB->OverEarth=bp.Pos.z-OB->RZ;
			}
            bp.Time=DestTime;
            
            NewMonster* NMN=OB->newMons;
            NewAnimation* NAM=OB->NewAnm;
            if( NAM && NAM->NFrames>0 ){ //crash if( NAM->NFrames ){
                OB->NewCurSprite=OB->NewCurSprite%NAM->NFrames;
            }
            if(OB->FrameFinished){
                if(Stun){
                    NewAnimation* NA=NMN->GetAnimation(anm_PStand+9);
                    if(NA->Enabled){
                        OB->NewAnm=NA;
                    }else{
                        OB->NewAnm=NMN->GetAnimation(anm_Stand);
                    }
                }
                if(!OB->Sdoxlo){
					if(TankStyleAnimation){
                        OB->NewAnm=NMN->GetAnimation(anm_MotionL);
						if(OB->NewAnm->NFrames>0){							
							int fr=((abs(OB->TotalPath+100000)/(_maxspeed+1))%(int(OB->NewAnm->NFrames)<<8));
							OB->TotalPath+=sp*dt;
							OB->CurrentFrameLong=fr;
							OB->NewCurSprite=fr>>8;
						}
					}else{
						OB->SetZeroFrame();                
						int d=OB->DestX>0?anm_MotionL:anm_Stand;
						int d0=d;
						if(OB->LocalNewState){
							d=(OB->DestX>0?anm_PMotionL:anm_PStand)+OB->LocalNewState-1;
						}
						OB->NewAnm=NMN->GetAnimation(d);
						if(!OB->NewAnm->Enabled){
							OB->NewAnm=NMN->GetAnimation(d0);
						}
					}
                }
            }	
			if(!(OB->Sdoxlo||TankStyleAnimation)){
				if(SlowFrame&&OB->NewCurSprite>NAM->SlowFrameStart&&OB->NewCurSprite<NAM->SlowFrameEnd){
					int gs=GameSpeed;
					GameSpeed=256*dt/40*NAM->SlowFrameSpeed/100;
					OB->SetNextFrame();
					GameSpeed=gs;
				}else{
					int gs=GameSpeed;
					GameSpeed=256*dt/40;
					OB->SetNextFrame();
					GameSpeed=gs;
				}
			}
            bp.UpdateBottom(rdt,WireLength,this);
            //updating fiOrt,fiDir
            float fi=OB->RealDirPrecise*c_PI/32768;
            float _cos=cos(fi);
            float _sin=sin(fi);
            Vector3D vdir(_cos,_sin,0);
            Vector3D vort(_sin,-_cos,0);
            Vector3D N=bp.Pos-bp.BottomPos;
			if(N.normalize()>0){
				Vector3D Loc(vdir.dot(N),vort.dot(N),1);
				OB->ForceX=-atan2(Loc.x,Loc.z)*32768*AngleX_modulator/c_PI;
				OB->ForceY=-atan2(Loc.y,Loc.z)*32768*AngleY_modulator/c_PI;
				OB->GraphDir=OB->RealDir=OB->RealDirPrecise>>8;
			}
        }
    }        
}
FlyBodyParams* GetBodyParams(OneObject* OB){
    if(!OB->ActiveAbility){
        OB->ActiveAbility=new UnitActiveAbilityList(OB);        
    }
    int N=OB->ActiveAbility->ActiveAbilities.GetAmount();
    for(int i=0;i<N;i++){
        ActiveUnitAbility* UA=OB->ActiveAbility->ActiveAbilities[i];
        if(!strcmp(UA->GetClassName(),"FlyBodyParams")){
            FlyBodyParams* FB=(FlyBodyParams*)UA;
            if(!FB->PosInit){
                FB->Init();
            }
            return FB;
        }
    }
    FlyBodyParams* FB=new FlyBodyParams;
    FB->Visible=false;
    //FB->UnitIndex=OB->Index;
    FB->OB=OB;
    FB->Init();

    FB->Mass=1;
    FB->Acceleration=1;
    FB->BreakModulator=2;
    FB->WireLength=60;
    FB->AirFriction=1.0f;
    FB->AngleX_modulator=0.0f;
    FB->AngleY_modulator=0.0f;
    FB->Gravity=40.0f;
    
    OB->ActiveAbility->ActiveAbilities.Add(FB);
    return FB;
}
void ApplyImpulse(OneObject* OB,Vector3D dir,float Force){
    FlyBodyParams* FB=GetBodyParams(OB);
	if(FB){
		FB->Pos[0].BottomSpeed+=dir*Force*OB->GroupSpeed*0.5;
	}
}
void MotionHandlerForFlyingObjects(OneObject* OB){
	APPLY_TIRING(OB);
    OB->UsesDestDidection=true;
    FlyBodyParams* FBP=GetBodyParams(OB);
    BodyPositon& bp=FBP->Pos[0];
    if(Norma(OB->RealX/16-bp.Pos.x,OB->RealY/16-bp.Pos.y)>8){
        float dz=OB->RZ+OB->OverEarth-bp.Pos.z;
        bp.Pos=Vector3D(OB->RealX/16,OB->RealY/16,bp.Pos.z+dz);
        bp.BottomPos=Vector3D(OB->RealX/16,OB->RealY/16,bp.BottomPos.z+dz);
        bp.Speed=bp.BottomSpeed=Vector3D::null;                
        FBP->Pos[0].Time=TrueTime-5;
    }
    int t0=FBP->Pos[0].Time;
    int dt=TrueTime-FBP->Pos[0].Time;
    int n=1+dt/8;
    dt/=n;
    int sp=OB->NewCurSprite;
    for(int i=1;i<=n;i++){
        FBP->ProcessTo(t0+i*dt);
    }
	OB->DestDirectionIsActive=false;
    OB->NewCurSpritePrev=sp;
    OB->x=(OB->RealX-((OB->Lx)<<7))>>8;
    OB->y=(OB->RealY-((OB->Lx)<<7))>>8;
    return;


	if(OB->Sdoxlo){		
        if(OB->OverEarth>0)OB->OverEarth-=4;
        if(OB->OverEarth<0)OB->OverEarth=0;
		return;
	}
	if(OB->DestX<=0)OB->StandTime++;
	if(OB->BrigadeID==0xFFFF&&!OB->LocalOrder)OB->DestX=-1;
	if(OB->PathDelay>=FrmDec)OB->PathDelay-=FrmDec;
	else OB->PathDelay=0;
	NewMonster* NMN=OB->newMons;
	AdvCharacter* ADC=OB->MoreCharacter;	
	int vx0=OB->RealVx;
	int vy0=OB->RealVy;	
	int N=Norma(OB->RealVx,OB->RealVy);
	bool SlowFrame=0;
	NewAnimation* NAM=OB->NewAnm;

	int R=OB->newMons->MotionDist;
	int BaseR=R;
	R=(R*OB->MoreCharacter->Speed)/100;
	bool Stun=false;

	if(OB->ActiveAbility){
		R<<=5;
		OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(BaseR<<5,R,Stun);
		R>>=5;
	}	

	int SP0=Stun?0:R;
	bool Pike=false;
	bool nogrow=false;

	byte MoveDir=OB->RealDir;

	if(OB->DestX>0){		
		int DIS=Norma(OB->RealX-OB->DestX,OB->RealY-OB->DestY);				
		char D0=GetDir(OB->DestX-OB->RealX,OB->DestY-OB->RealY);
		if(DIS<400*16)MoveDir=D0;
		char dd=D0-OB->RealDir;
		N=(SP0*GameSpeed);
		if(!Stun){
			int W=NMN->MinRotator*GameSpeed;
			if(abs(int(dd)<<8)<=W){
				OB->RealDir=D0;
				OB->RealDirPrecise=int(D0)<<8;
			}else{
				if(dd>0){
					OB->RealDirPrecise+=W;
					SlowFrame=1;
				}else{
					OB->RealDirPrecise-=W;
					SlowFrame=1;
				}            
				OB->RealDir=OB->RealDirPrecise>>8;
			}
		}		
	}else{
		N=0;		
	}

	OB->GraphDir=OB->RealDir;
	OB->RealVx=(N*TCos[MoveDir])>>8;
	OB->RealVy=(N*TSin[MoveDir])>>8;
	OB->ForceX=(OB->ForceX*63+(OB->RealVx-vx0)/4)/68;
	OB->ForceY=(OB->ForceY*63+(OB->RealVy-vy0)/4)/68;
	if(OB->DestX>0 && Norma(OB->RealX-OB->DestX,OB->RealY-OB->DestY)<=(N>>8)){
		OB->RealX=OB->DestX;
		OB->RealY=OB->DestY;
	}else{
		OB->RealX+=OB->RealVx>>8;
		OB->RealY+=OB->RealVy>>8;
	}
	addrand(OB->RealX);
	addrand(OB->RealY);
	OB->x=(OB->RealX-((OB->Lx)<<7))>>8;
	OB->y=(OB->RealY-((OB->Lx)<<7))>>8;						
	if(N>16000)N=16000;
	int H=(N>>5)*NMN->FlyHeight/3200+NMN->StartFlyHeight;	
	int dzr=H*350/(OB->Sdoxlo+350);
	if(!nogrow){
		int dd=GameSpeed*2;
		int dd0=dd>>8;
		dd-=dd0<<8;
		if((rando()&255)<dd)dd0++;
		if(OB->OverEarth>dzr)OB->OverEarth-=dd0;
		if(OB->OverEarth<dzr)OB->OverEarth+=dd0;	
	}
	if(OB->NewAnm->NFrames){
		OB->NewCurSprite=OB->NewCurSprite%OB->NewAnm->NFrames;
	}
	if(OB->FrameFinished){
		OB->SetZeroFrame();
		if(!OB->Sdoxlo){
			int d=OB->DestX>0?anm_MotionL:anm_Stand;
			int d0=d;
			if(OB->LocalNewState){
				d=(OB->DestX>0?anm_PMotionL:anm_PStand)+OB->LocalNewState-1;
			}
			OB->NewAnm=NMN->GetAnimation(d);
			if(!OB->NewAnm->Enabled){
				OB->NewAnm=NMN->GetAnimation(d0);
			}
		}
	}	
	if(SlowFrame&&OB->NewCurSprite>NAM->SlowFrameStart&&OB->NewCurSprite<NAM->SlowFrameEnd){
		int gs=GameSpeed;
		GameSpeed=GameSpeed*NAM->SlowFrameSpeed/100;
		OB->SetNextFrame();
		GameSpeed=gs;
	}else{
		OB->SetNextFrame();
	}
	int z=GetHeight(OB->RealX>>4,OB->RealY>>4);
	if(z<0 && !OB->Sdoxlo)z=0;
	int z0=OB->RZ;
	OB->RZ=z;
	addrand(OB->RZ);
	int ov0=OB->OverEarth;
	OB->OverEarth+=z0-z;
	if(ov0>=20 && OB->OverEarth<20){
        OB->OverEarth=20;
	}
	if(OB->RZ<0 && !OB->Sdoxlo)OB->RZ=0;
	//OB->RealDirPrecise=word(OB->RealDir)<<8;
}
void OneObject::SetZeroFrame(){
	SoundWasEmit=false;
	NewCurSpritePrev=NewCurSprite;
	if(false){
		NewCurSprite=NewAnm->NFrames-1;
		CurrentFrameLong=int(NewAnm->NFrames-1)<<8;
		FrameFinished=NewAnm->NFrames<=1;
	}else{
		NewCurSprite=0;
		CurrentFrameLong=0;
		FrameFinished=NewAnm->NFrames<=1;
	}
}
void OneObject::SetNextFrame(){
	NewCurSpritePrev=NewCurSprite;
	if(!FrameFinished){
		if(false){//BackMotion&&DestX>0){
			CurrentFrameLong-=GameSpeed;
			NewCurSprite=CurrentFrameLong>>8;
			if(NewCurSprite<0){
				NewCurSprite=NewAnm->NFrames-1;
				CurrentFrameLong=int(NewAnm->NFrames-1)<<8;
				FrameFinished=1;
				if(NewCurSprite<0){
					NewCurSprite=0;
					CurrentFrameLong=0;						
				}										
			}
		}else{
			if(NewAnm->Code>=100&&NewAnm->Code<200){
				int sp=GetAttackSpeedBonus();
				CurrentFrameLong+=(GameSpeed*sp)/100;
			}else{
				CurrentFrameLong+=GameSpeed;
			}
			NewCurSprite=CurrentFrameLong>>8;
			if(NewCurSprite>=NewAnm->NFrames){
				NewCurSprite=NewAnm->NFrames-1;
				CurrentFrameLong=int(NewCurSprite)<<8;
				FrameFinished=1;
			}
		}
	}
}
void OneObject::SetNextFrameCycled(){
	NewCurSpritePrev=NewCurSprite;
	CurrentFrameLong+=GameSpeed;
	NewCurSprite=CurrentFrameLong>>8;
	if(NewCurSprite>=NewAnm->NFrames){
		CurrentFrameLong%=(int(NewAnm->NFrames)<<8);
		NewCurSprite=CurrentFrameLong>>8;
	}
}
void StayForSomeTimeLink(OneObject* OBJ){
	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;
	OBJ->LocalOrder->info.Produce.Progress-=GameSpeed;
	if(OBJ->LocalOrder->info.Produce.Progress<0){
		OBJ->DeleteLastOrder();
	}	
}
void StayForSomeTime(OneObject* OB,byte OrdType,int Time){
	Order1* OR=OB->CreateOrder(OrdType);
	if(!OR)return;
	OR->info.Produce.Progress=Time;
	OR->DoLink=&StayForSomeTimeLink;
	OR->PrioryLevel=16;
}
void CheckVitality(OneObject* OB){
	if(OB->LockType!=1&&!OB->NewBuilding){
		if(MFIELDS[OB->LockType].CheckBar(OB->x,OB->y,OB->Lx,OB->Lx)){
			OB->Die();
		}
	}
}
void PushUnitBackLink(OneObject* OBJ){
	int cod=OBJ->NewAnm->Code;
	if(cod!=18&&cod!=19){
		OBJ->DeleteLastOrder();
		return;
	}
	OBJ->PrioryLevel=255;
	int dx=short(OBJ->LocalOrder->info.AttackXY.x);
	int dy=short(OBJ->LocalOrder->info.AttackXY.y);
	OBJ->RealX+=dx;
	OBJ->RealY+=dy;
	addrand(OBJ->RealX);
	addrand(OBJ->RealY);
	OBJ->RZ=GetHeight(OBJ->RealX>>4,OBJ->RealY>>4);
	addrand(OBJ->RZ);
	OBJ->x=(OBJ->RealX-(OBJ->Lx<<7))>>8;
	OBJ->y=(OBJ->RealY-(OBJ->Lx<<7))>>8;
	if(OBJ->FrameFinished){        
		if(dx||dy){
			OBJ->LocalOrder->info.AttackXY.x=0;
			OBJ->LocalOrder->info.AttackXY.y=0;
			NewAnimation* NA=OBJ->newMons->GetAnimation(anm_StandUp);
			if(NA){
				OBJ->NewAnm=NA;
				OBJ->SetZeroFrame();                
			}else{
				OBJ->PrioryLevel=0;
				OBJ->DeleteLastOrder();
				CheckVitality(OBJ);
			}
		}else{
			OBJ->PrioryLevel=0;
			OBJ->DeleteLastOrder();
			CheckVitality(OBJ);
			OBJ->RealDir+=OBJ->newMons->StrikeRotate;
			OBJ->GraphDir=OBJ->RealDir;
		}
	}
}
void PushUnitBack(OneObject* OB,byte OrdType,int Force){
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&PushUnitBackLink)return;
	NewAnimation* NA=OB->newMons->GetAnimation(anm_FallDown);
	if(NA&&NA->Enabled){
		Order1* OR=OB->CreateOrder(OrdType);
		if(OR){
			OB->NewAnm=NA;
			OB->SetZeroFrame();
			OR->DoLink=&PushUnitBackLink;
			OR->info.AttackXY.x=-(((int(TCos[OB->RealDir])*GameSpeed)>>8)*Force)>>8;
			OR->info.AttackXY.y=-(((int(TSin[OB->RealDir])*GameSpeed)>>8)*Force)>>8;
		}	
	}
}
void PushUnitBack(OneObject* OB,byte OrdType,int Force, int EpicenterX, int EpicenterY)
{
	if(OB)
	{
		if( !(OB->newMons->RotationAtPlaceSpeed||OB->newMons->MinRotator<16) )
		{
			int x=OB->RealX/16;
			int y=OB->RealY/16;
			byte Drr=GetDir(x-EpicenterX,y-EpicenterY);
			OB->RealDir=Drr+128;
			PushUnitBack(OB,OrdType,Force);
		}
	}
}
int OneObject::GetAttackSpeedBonus(){
	int sp;
    sp=MoreCharacter->AttackSpeed;
	if(BrigadeID!=0xFFFF){
		sp=sp*CITY[NNUM].Brigs[BrigadeID].AttackRateBonus/100;
	}
	if(sp<=0)sp=1;
	if(ActiveAbility){
		OneObject* V=NULL;
		if(EnemyID!=0xFFFF){
			V=Group[EnemyID];
			if(!(V&&V->Serial==EnemySN))V=NULL;
		}
		ActiveAbility->ActiveAbilities.modifyAttackRate(LocalNewState?LocalNewState-1:0,100,sp,V);
	}
	return sp;
}
int OneObject::GetMaxAttackRadius(int AttType){
	if(AttType<0)AttType=0;
	//static bool recursion=false;
	int R=MoreCharacter->AttackRadius2[AttType];
	/*if(R>180)*/{
		if(BrigadeID!=0xFFFF){
			R+=CITY[NNUM].Brigs[BrigadeID].AddAttackRange;
		}
		if(ActiveAbility){
			//if(!recursion){
			//	recursion=true;
				ActiveAbility->ActiveAbilities.modifyAttackRadiusMax(R,R);
			//}
		}
	}
	//recursion=false;
	return R;
};
int OneObject::GetMaxAttackRadius(){
	//static bool recursion=false;
	int R=MoreCharacter->MaxR_Attack;
	/*if(R>180)*/{
		if(BrigadeID!=0xFFFF){
			R+=CITY[NNUM].Brigs[BrigadeID].AddAttackRange;
		}
		if(ActiveAbility){
			//if(!recursion){
			//	recursion=true;
				ActiveAbility->ActiveAbilities.modifyAttackRadiusMax(R,R);
			//}
		}
	}
	//recursion=false;
	return R;
}
extern bool InfoMode;
bool OneObject::notSelectable(){
	return NotSelectable && !(EditMapMode&&InfoMode);
}

bool ExtraLock(word& NLockPt,char*& LockX,char*& LockY)
{
	byte MaxX=0;
	byte MaxY=0;
	if((!LockX)||(!LockY))
		return true;
	for(word i=0;i<NLockPt;i++)
	{
		if(LockX[i]>MaxX)
		{
			MaxX=LockX[i];
		}
		if(LockY[i]>MaxY)
		{
			MaxY=LockY[i];
		}
	}
	int sz=(MaxX+3)*(MaxY+3);
	if(sz)
	{
		char* Ar = new char[sz];
		memset(Ar,0,sz);
		byte xz=MaxX+2;
		for(word i=0;i<NLockPt;i++)
		{
			byte x=LockX[i];
			byte y=LockY[i];
			Ar[x+y*xz]=1;
			Ar[x+1+y*xz]=1;
			Ar[x+(y+1)*xz]=1;
			if(x>0)
				Ar[x-1+y*xz]=1;
			if(y>0)
				Ar[x+(y-1)*xz]=1;
		}
		int NA=0;
		for(int j=0;j<sz;j++)
		{
			if(Ar[j])
				NA++;
		}
		char* newLX = (char*)malloc(NA);
		char* newLY = (char*)malloc(NA);
		
		NA=0;
		
		for(byte y=0;y<(MaxY+2);y++)
		{
			for(byte x=0;x<(MaxX+2);x++)
			{
				if(Ar[x+y*xz])
				{
					newLX[NA]=x;
					newLY[NA]=y;
					NA++;
				}
			}
		}
	
		/*
		for(j=0;j<sz;j++)
		{
			if(Ar[j])
			{
				newLX[NA]=(byte)j%xz;
				newLY[NA]=(byte)j/xz;
				NA++;
			}
		}
		*/
		free(LockX);
		free(LockY);
		delete[] Ar;

		NLockPt=(word)NA;
		LockX=newLX;
		LockY=newLY;
		return true;
	}
	return false;
}
int CurrentAnmQuality=0;
void ResetAnimationQuality(bool LowQ){
	//return;
    if(CurrentAnmQuality==LowQ)return;
	CurrentAnmQuality=LowQ;
	for(int i=0;i<NNewMon;i++){
		NewMonster* NM=NewMon[i];
		for(int j=0;j<NM->Animations->ANM.GetAmount();j++){
			NewAnimation* NA=NM->Animations->ANM[j];
			if(NA&&NA->AnimationType==0){//2d
				int pg=-1;
				int pg1=-1;
				for(int q=0;q<NA->Frames.GetAmount();q++){
                    NewFrame* NF=NA->Frames[q];
					if(NF){
						if(NF->FileID==pg)NF->FileID=pg1;
						else{
							pg=NF->FileID;
							if(LowQ)pg1=GetShortGPID(pg);
							else pg1=GetLongGPID(pg);
							NF->FileID=pg1;
						}
					}
				}
			}
		}
	}
}
void ExtendedIcon::Draw(int x,int y,int Lx,int Ly,byte NI){
	TempWindow TW;
	PushWindow(&TW);
	IntersectWindows(x,y,x+Lx-1,y+Ly-1);
	for(int i=0;i<Icons.GetAmount();i++){
		Icons[i]->Draw(x,y,Lx,Ly,NI);
	}
	PopWindow(&TW);
}
extern int LastFlipTime;
CEXPORT void DrawRotatedPicture(int x,int y,int File,int Sprite,byte Nat,
								int dx,int dy,float Angle,float sx,float sy);

void OneSpriteExIconSet::Draw(int x,int y,int Lx,int Ly,byte NI){
	if(Val){
		if(LastFlipTime>=CurrentEndTime){

			int CurVal=rand()%Val; //GetTickCount()
			int v=0;
			for(int i=0;i<ProbValue.GetAmount();i++){
				v+=ProbValue[i];
				if(CurVal<v){
					Current=i;
					break;
				}
			}
			OneSpriteExIcon* EI=Icons[Current];
			int NFrames=1;
			if(EI->Step&&EI->EndSpriteID>EI->StartSpriteID){
				NFrames=(EI->EndSpriteID-EI->StartSpriteID+1)/EI->Step;
			}

			int Scene=NFrames*40;
			if(CurrentEndTime==0){
				CurrentEndTime=LastFlipTime;
			}else{
				CurrentStartTime=LastFlipTime-(LastFlipTime-CurrentEndTime)%Scene;
			}
			CurrentEndTime=CurrentStartTime+Scene;
		}
		int t=LastFlipTime;
		LastFlipTime-=CurrentStartTime;
		Icons[Current]->Draw(x,y,Lx,Ly,NI);
		LastFlipTime=t;
	}
}

void OneSpriteExIcon::Draw(int x,int y,int Lx,int Ly,byte NI){	
    int spr=StartSpriteID;
	if( Step && EndSpriteID>StartSpriteID ){
		if( vmIGP.FirstTimeShowExIcon==0 ){
			vmIGP.FirstTimeShowExIcon=TrueTime;
		}else if(TrueTime>vmIGP.FirstTimeShowExIcon+EndSpriteID*100){
			//if(	vmIGP.FirstTimeShowExIcon==0 )
			//	vmIGP.FirstTimeShowExIcon=LastFlipTime;
			//int dt=LastFlipTime-vmIGP.FirstTimeShowExIcon;
			int ds=(EndSpriteID-StartSpriteID+1)/Step;
			//spr=StartSpriteID+((dt/40)%ds)*Step;
			spr=StartSpriteID+((LastFlipTime/40)%ds)*Step;
			int NFrames=GPS.GPNFrames(FileID);
			if(spr>=NFrames) spr=StartSpriteID;
		}else{
			//StartTime=0;
		}
	}
	float sx=1.0;
	float sy=1.0;
	if(Lx!=0&&Ly!=0){
		int W=GPS.GetGPWidth(FileID,-2);
		int H=GPS.GetGPHeight(FileID,-2);
		if(W!=Lx||H!=Ly){
			sx=(float)Lx/(float)W;
			sy=(float)Ly/(float)H;
		}
	}
	DrawRotatedPicture(x,y,FileID,spr,NI,-dx,-dy,0,sx,sy);
	//GPS.ShowGP(x-dx,y-dy,FileID,spr,NI);
}
void OneSpriteExIcon3D::Draw(int x,int y,int Lx,int Ly,byte NI){
	if(ModelID>0){
		int RS=RotateSpeed>0.001?int(1000/RotateSpeed):0;
		int T1=RS?LastFlipTime%RS:0;
		int T2=LastFlipTime%Period;
		float T=IMM->GetAnimTime(AnimID);
		DrawThumbnail(ModelID,Rct(x,y,Lx,Ly),AnimID,(T2?float(T2)/Period:0)*T,StartAngle*c_PI*2+(RS?c_PI*2*T1/RS:0),0,Scale,zShift);
	}	
}
void OneObject::GetCylinder(int& CylinderRarius,int& CylinderHeight,int& UnitHeight){
	CylinderRarius=newMons->UnitCylinderRadius*newMons->StartScale/256;
	CylinderHeight=newMons->UnitCylinderHeight*newMons->StartScale/256;
	if(ActiveAbility){
		word s=256;
        ActiveAbility->ActiveAbilities.modifyScale(s,s);
        CylinderRarius=CylinderRarius*s/256;
		CylinderHeight=CylinderHeight*s/256;
	}
	UnitHeight=RZ+OverEarth;
}
void OneObject::GetPureCylinder(int& CylinderRarius,int& CylinderHeight,int& UnitHeight){
    CylinderRarius=newMons->UnitCylinderRadius*newMons->StartScale/256;
    CylinderHeight=newMons->UnitCylinderHeight*newMons->StartScale/256;
    UnitHeight=RZ+OverEarth;
}
AdvCharacter::AdvCharacter(){
}
AdvCharacter::~AdvCharacter(){
}