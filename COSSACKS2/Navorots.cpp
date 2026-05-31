#include "stdheader.h"

int mul3(int);
void TeleLink(OneObject* OBJ){
	
};
void GetRect(OneObject* ZZ,int* x,int* y,int* Lx,int* Ly);
void OneObject::SetDstPoint(int xd,int yd){
	//if(!Ready)return;
	if(!(Ref.General->CanDest||NewBuilding))return;
	int x0,y0,x1,y1;	
	DstX=xd;
	DstY=yd;	
};
int DestnX[512];
int DestnY[512];
int NDestn;
void InitDestn(){
	NDestn=0;
};
void AddDestn(int x,int y){
	if(NDestn<512){
		DestnX[NDestn]=x;
		DestnY[NDestn]=y;
		NDestn++;
	};
};
extern int tmtmt;
void miniShowDestn(){
	
};
void xLine(int x,int y,int x1,int y1,byte c);
void ShowDestn(){
	///if(LMode&&(tmtmt&7)<3)return;
	int smx=mapx+smaplx;
	int smy=mapy+smaply;
	int x,y,y1;
	int dx=mapx<<5;
	int dy=mul3(mapy)<<3;
	int Lx=smaplx<<5;
	int Ly=mul3(smaply)<<3;
	for(int i=0;i<NDestn;i++){
		x=DestnX[i];
		y=DestnY[i];
		if(x!=-1){
			int cur=GSets.gpRally;
			Vector3D V=SkewPt(x,y,max(GetHeight(x,y),0));
			WorldToScreenSpace(V);
			GPS.ShowGP(V.x-32,V.y-32,cur,(GetTickCount()/40)%17,GSets.CGame.cgi_NatRefTBL[MyNation]);				
		}
	};
};
extern int MiniLx;
extern int MiniLy;
extern int MiniX,MiniY;
void ShowMiniDestn(){
	if((tmtmt&7)<5)return;
	int x,y;
	for(int i=0;i<NDestn;i++){
		x=minix+(DestnX[i]>>(5+ADDSH))-MiniX;
		y=miniy+(DestnY[i]>>(5+ADDSH))-MiniY;
		Hline(x,y,x,clrYello);
		Hline(x-1,y-1,x-1,clrYello);
		Hline(x+1,y+1,x+1,clrYello);
		Hline(x+1,y-1,x+1,clrYello);
		Hline(x-1,y+1,x-1,clrYello);
		//Vline(x,y-1,y+1,clrYello);
	};
};
void PerformAction(OneObject* OB,int x,int y){
	
};
class GreenPulse{
public:
	word GreenObjs[256];
	word GreenTime[256];
	byte ObjColor[256];
	byte ObjOpt[256];
	void AddObj(word ID,byte Color);
	void AddXY(int x,int y);
	GreenPulse();
	void Clear();
	void Show();
};
GreenPulse::GreenPulse(){
	memset(GreenObjs,0xFF,512);
	memset(GreenTime,0,512);
};
void GreenPulse::AddObj(word ID,byte Color){
	for(int i=0;i<256&&GreenObjs[i]!=0xFFFF;i++);
	if(i<256){
		GreenObjs[i]=ID;
		GreenTime[i]=24;
		ObjColor[i]=Color;
		ObjOpt[i]=255;
	};
};
void GreenPulse::AddXY(int x,int y){
	for(int i=0;i<256&&GreenObjs[i]!=0xFFFF;i++);
	if(i<256){
		GreenObjs[i]=x;
		GreenTime[i]=y;
		ObjColor[i]=0;
		ObjOpt[i]=0;
	};
};
void GreenPulse::Clear(){
	memset(GreenObjs,0xFF,512);
	memset(GreenObjs,0xFF,512);
	memset(ObjOpt,0,256);
};
extern int AnmCursorGP;
void DrawColorMarker(OneObject* OB,byte cl);
extern bool Mode3D;
void ShowHealth(OneObject* ZZ);
extern bool HealthMode;
void DrawMarker(OneObject* OB);
void DrawHealth(OneObject* OB);
bool WasL=0;
void GreenPulse::Show(){
	//if(LMode){
	//	if(WasL)Clear();
	//	WasL=0;
	//	return;
	//};
	WasL=1;
	bool show=1;
	if((tmtmt>>2)&1)show=0;
	for(int i=0;i<256;i++){
		if(ObjOpt[i]==255){
			word ID=GreenObjs[i];
			if(ID!=0xFFFF){
				if(show)GreenTime[i]--;
				if(GreenTime[i]){
					OneObject* OB=Group[ID];
					if(OB){
						if(show&&!LMode)DrawColorMarker(OB,ObjColor[i]);
						if(ObjColor[i]==0xD0&&!(HealthMode||LMode)){
							//DrawHealth(OB);
						};
					};
				}else GreenObjs[i]=0xFFFF;
			};
		}else{
			int x=GreenObjs[i];
			int y=GreenTime[i];
			//if(!GPS.IsGPLoaded( AnmCursorGP )) GPS.LoadGP(AnmCursorGP);
			AnmCursorGP=GSets.gpMove;
			if(x&&y){
				if(ObjOpt[i]<GPS.GPNFrames(AnmCursorGP)){
					int spr=ObjOpt[i];
					//if(!LMode)
					//{
						int z=GetTotalHeight(x,y);
						if(z<0)z=0;
						Vector3D pos = SkewPt( x, y, z );
						WorldToScreenSpace( pos );
						GPS.ShowGP(pos.x - 29,pos.y - 26,AnmCursorGP,spr,0);
					//}
					ObjOpt[i]++;
				}else{
					GreenObjs[i]=0xFFFF;
					GreenTime[i]=0;
					ObjOpt[i]=0xFF;
				};
			};
		};
	};
};
GreenPulse PULSAR;
void InitPulse(){
	PULSAR.Clear();
};
void AddPulse(word ObjID,byte c){
	PULSAR.AddObj(ObjID,c);
};
CEXPORT void AddXYPulse(int x,int y){
	PULSAR.AddXY(x,y);
};
void ShowPulse(){
	PULSAR.Show();
};
//-----------------------------------National list---------------------------------
word* NatList[8];
int   NtNUnits[8];
int   NtMaxUnits[8];
byte NUnitsCHK[8192];
bool HealDoubleUnits(){
	for(int i=0;i<8;i++){
		int N=NtNUnits[i];
		word* units=NatList[i];
		for(int j=0;j<N;j++){
			OneObject* OB=Group[units[j]];
			if((!OB)||OB->NNUM!=i){
				if(j<N-1){
					memcpy(units+j,units+j+1,(N-j-1)*2);
				};
				NtNUnits[i]--;
				return true;
			}
		};
	};
	return false;
};
void CheckUnits(){
	return;
	memset(NUnitsCHK,0,8192);
	for(int i=0;i<8;i++){
		int N=NtNUnits[i];
		word* units=NatList[i];
		for(int j=0;j<N;j++){
			NUnitsCHK[units[j]]++;
			if(NUnitsCHK[units[j]]<2){
				//assert(NUnitsCHK[units[j]]<2);
				//healing
				do{}while(HealDoubleUnits());
			};
		};
	};
};
void SetupNatList(){
	for(int i=0;i<8;i++){
		NtNUnits[i]=0;
		NtMaxUnits[i]=256;
		NatList[i]=znew(word,256);
	};
};
void InitNatList(){
	for(int i=0;i<8;i++)free(NatList[i]);
	SetupNatList();
};
void ClearNatList(){
	for(int i=0;i<8;i++){
		free(NatList[i]);
		NatList[i]=0;
	};
};
extern City CITY[8];
void AddObject(OneObject* OB){
	if(OB&&OB->NewBuilding){
		if(OB->Nat->CITY->DestBldGroup!=0xFFFF){
			UnitsGroup* UG=SCENINF.UGRP+OB->Nat->CITY->DestBldGroup;
			UG->AddNewUnit(OB);
		};
		for(int i=0;i<8;i++)if(!(NATIONS[i].NMask&OB->NMask)){
			if(CITY[i].DestEnemyGroup!=0xFFFF){
				UnitsGroup* UG=SCENINF.UGRP+CITY[i].DestEnemyGroup;
				UG->AddNewUnit(OB);
			};
		};
	};

	word ID=OB->Index;
	word NNUM=OB->NNUM;
	if(NtNUnits[NNUM]==NtMaxUnits[NNUM]){
		NtMaxUnits[NNUM]+=1024;
		NatList[NNUM]=(word*)realloc(NatList[NNUM],2*NtMaxUnits[NNUM]);
	};
	NatList[NNUM][NtNUnits[NNUM]]=ID;
	NtNUnits[NNUM]++;
	void OnBirth(int idx);
	OnBirth(ID);
};
void DelObject(OneObject* OB){
	if(OB&&OB->NewBuilding){
		if(OB->Nat->CITY->DestBldGroup!=0xFFFF){
			UnitsGroup* UG=SCENINF.UGRP+OB->Nat->CITY->DestBldGroup;
			UG->RemoveUnit(OB);
		};
		for(int i=0;i<8;i++){
			if(CITY[i].DestEnemyGroup!=0xFFFF){
				UnitsGroup* UG=SCENINF.UGRP+CITY[i].DestEnemyGroup;
				UG->RemoveUnit(OB);
			};
		};
	};
	word ID=OB->Index;
	for(int NNUM=0;NNUM<8;NNUM++){
		int N=NtNUnits[NNUM];
		word* UList=NatList[NNUM];
		for(int j=0;j<N;j++){
			if(UList[j]==ID){
				if(j<N-1){
					memcpy(UList+j,UList+j+1,(N-j-1)*2);
				};
				NtNUnits[NNUM]--;
				return;
			};
		};
	};
	//assert(0);
};
void ReRegisterObjects(){
	for(int i=0;i<8;i++){
		NtNUnits[i]=0;
	}
	for(int ID=0;ID<MAXOBJECT;ID++){
		OneObject* OB=Group[ID];
		if(OB){
			word NNUM=OB->NNUM;
			if(NtNUnits[NNUM]==NtMaxUnits[NNUM]){
				NtMaxUnits[NNUM]+=1024;
				NatList[NNUM]=(word*)realloc(NatList[NNUM],2*NtMaxUnits[NNUM]);
			};
			NatList[NNUM][NtNUnits[NNUM]]=ID;
			NtNUnits[NNUM]++;
		}
	}
}