#include "stdheader.h"
#include "UnitsInterface.h"
#include "ai_scripts.h"
#include "CurrentMapOptions.h"
#include "mp3\\oggvor.h"
#include ".\cext_VisualInterface.h"
#include ".\cvi_HeroButtons.h"
#include "unitability.h"
#include "GameInterface.h"
#include "PluginsInterface.h"
//
void TryAlarm(int x,int y,int reason);

void GetCostString(int* Cost,char* str);

extern bool NOPAUSE;
extern bool LockPause;
extern int CurrentAnswer;
byte AssignTBL[10]={0,1,2,3,4,5,6,7,8,9};
void AIER(char* Mess);
bool AiIsRunNow=false;
ScenaryInterface SCENINF;
ScenaryInterface::ScenaryInterface(){
	memset(this,0,sizeof ScenaryInterface);
};
ScenaryInterface::~ScenaryInterface(){
	UnLoading();
};
extern int tmtmt;
extern int AnimTime;
extern int TrueTime;
void HandleMission(){
	void PrecessScriptForExposedInterfaces();
	PrecessScriptForExposedInterfaces();
	RunPF(256*5+25,"Diplomacy,Missions");
	addrand(tmtmt);
	if(ProcessDipRelations)ProcessDipRelations();
	addrand(tmtmt);
	if(SCENINF.ScenaryHandler)SCENINF.ScenaryHandler();
	addrand(tmtmt);
	if(SCENINF.CTextTime>0){
		SCENINF.CTextTime-=8<<SpeedSh;
		if(SCENINF.CTextTime<0)SCENINF.CTextTime=0;
	};
	GTimer* GTM=SCENINF.TIME;
	addrand(tmtmt);
	for(int i=0;i<32;i++){
		if(GTM->Time){
			if(GTM->trueTime){
				GTM->Time-=TrueTime-GTM->LastCheckAnimTime;
				GTM->LastCheckAnimTime=TrueTime;
			}else{
				//GTM->Time-=8<<SpeedSh;
				GTM->Time-=(AnimTime-GTM->LastCheckAnimTime)/256;
				GTM->LastCheckAnimTime=AnimTime;
			}
			if(GTM->Time<=0){
				GTM->Time=0;
				GTM->First=1;
				GTM->Used=1;
			};
		};
		GTM++;
	};
	int NU=SCENINF.UGRP.GetAmount();
	addrand(tmtmt);
	for(int v=0;v<NU;v++){
		if(SCENINF.UGRP[v].GOrder&&SCENINF.UGRP[v].GOrder->Link)SCENINF.UGRP[v].GOrder->Link(&SCENINF.UGRP[v]);
	};
	addrand(tmtmt);
	StopPF(256*5+25);
};
extern int maxFX;
extern int maxFY;
#define FMSHI 3
extern int maxFX;
extern int maxFY;
extern int FMSX;
__forceinline void FogSpot(int x,int y){
	int fx=((x+32)>>6)+FMSHI;
	int fy=((y+32)>>6)+FMSHI;
	if(fx<1)fx=1;
	if(fy<1)fy=1;
	if(fx>maxFX)fx=maxFX;
	if(fy>maxFY)fy=maxFY;
    fmap[fy*FMSX+fx]=3000;
};
void SetFogSpot(int xx, int yy, int r){
	if(xx){
		int rr=200;
		int rr1=rr-(rr>>2);
		//yy-=(GetHeight(xx,yy)<<1);
		switch(r){
			case 0:
				FogSpot(xx,yy);
				break;
			case 1:
				FogSpot(xx+128,yy);
				FogSpot(xx-128,yy);
				FogSpot(xx,yy+128);
				FogSpot(xx,yy-128);
				break;
			case 2:
				FogSpot(xx+128,yy+128);
				FogSpot(xx-128,yy+128);
				FogSpot(xx+128,yy-128);
				FogSpot(xx-128,yy-128);
				break;
			case 3:
				FogSpot(xx+2*128,yy);
				FogSpot(xx-2*128,yy);
				FogSpot(xx,yy-2*128);
				FogSpot(xx,yy+2*128);
				FogSpot(xx+128,yy+128);
				FogSpot(xx-128,yy+128);
				FogSpot(xx+128,yy-128);
				FogSpot(xx-128,yy-128);
				break;
			case 4:
				FogSpot(xx+3*128,yy);
				FogSpot(xx-3*128,yy);
				FogSpot(xx,yy+3*128);
				FogSpot(xx,yy-3*128);
				FogSpot(xx+2*128,yy+2*128);
				FogSpot(xx-2*128,yy+2*128);
				FogSpot(xx+2*128,yy-2*128);
				FogSpot(xx-2*128,yy-2*128);
				break;
			case 5:
				FogSpot(xx+4*128,yy);
				FogSpot(xx-4*128,yy);
				FogSpot(xx,yy+4*128);
				FogSpot(xx,yy-4*128);
				FogSpot(xx+3*128,yy+3*128);
				FogSpot(xx-3*128,yy+3*128);
				FogSpot(xx+3*128,yy-3*128);
				FogSpot(xx-3*128,yy-3*128);
				break;
			case 6:
				FogSpot(xx+5*128,yy);
				FogSpot(xx-5*128,yy);
				FogSpot(xx,yy+5*128);
				FogSpot(xx,yy-5*128);
				FogSpot(xx+4*128,yy+4*128);
				FogSpot(xx-4*128,yy+4*128);
				FogSpot(xx+4*128,yy-4*128);
				FogSpot(xx-4*128,yy-4*128);
				break;
			case 7:
				FogSpot(xx+6*128,yy);
				FogSpot(xx-6*128,yy);
				FogSpot(xx,yy+6*128);
				FogSpot(xx,yy-6*128);
				FogSpot(xx+4*128,yy+4);
				FogSpot(xx-4*128,yy+4*128);
				FogSpot(xx+4*128,yy-4*128);
				FogSpot(xx-4*128,yy-4*128);
				break;
			default:
				FogSpot(xx+7*128,yy);
				FogSpot(xx-7*128,yy);
				FogSpot(xx,yy+7*128);
				FogSpot(xx,yy-7*128);
				FogSpot(xx+5*128,yy+3);
				FogSpot(xx-5*128,yy+5*128);
				FogSpot(xx+5*128,yy-5*128);
				FogSpot(xx-5*128,yy-5*128);
		};
	};
}
void ScenaryLights(){
	for(int i=0;i<64;i++){
		int xx=SCENINF.LSpot[i].x;
		int yy=SCENINF.LSpot[i].y;
		int Type=SCENINF.LSpot[i].Type;
		SetFogSpot(xx,yy,Type);
	};
};
void ScenaryInterface::UnLoading(){
	for(int i=0;i<8;i++)AssignTBL[i]=i;
	if(MaxSaves){
		free(SaveZone);
		free(SaveSize);
	};
	SaveZone=NULL;
	SaveSize=NULL;
	if(DLLName)free(DLLName);
	for(int i=0;i<UGRP.GetAmount();i++){
		UGRP[i].ClearOrders();
		UGRP[i].Free();
	}
	UGRP.Clear();
	ZGRP.Clear();
	if(MaxMess){
		free(Messages);
	};
	if(MaxSnds){
		for(int i=0;i<NSnd;i++)free(Sounds[i]);
		free(Sounds);
	};
	NSnd=0;
	//if(MissText)delete(MissText);
	if(MaxPages){
		for(int i=0;i<NPages;i++){
			free(Page[i]);
			free(PageID[i]);
			free(PageBMP[i]);
		};
		free(Page);
		free(PageID);
		free(PageBMP);
		free(PageSize);
	};
	NPages=0;
	if(hLib)FreeLibrary(hLib);
	memset(this,0,sizeof ScenaryInterface);
};

void ScenErr(char* Mess){
	ResFile FFF=RReset("ERR.LOG");
	if(FFF!=INVALID_HANDLE_VALUE){
		RClose(FFF);
		MessageBox(NULL,Mess,"Scenary error...",MB_TOPMOST);
		assert(false);
	};
};
void ScenErr(char* Mess,char* Par1){
	char ccc[160];
	sprintf(ccc,Mess,Par1);
	ScenErr(ccc);
};
void ScenErr(char* Mess,char* Par1,char* Par2){
	char ccc[160];
	sprintf(ccc,Mess,Par1,Par2);
	ScenErr(ccc);
};
int GetCPos(char C){
	if(C>='A'&&C<='Z'){
		return C-'A';
	};
	if(C>='a'&&C<='z'){
		return 26+C-'a';
	};
	return 0;
};
extern _str vm_F1_VictoryCond;
void CreateMissText(DString& Miss){
	Miss.Clear();
	int TextSize=0;//SCENINF.TextSize;
	char* MissText=NULL;//SCENINF.MissText;
	for(int i=0;i<SCENINF.NPages;i++){
		if(!strcmp(SCENINF.PageID[i],"#BRIEFING")){
			MissText=SCENINF.Page[i];
			TextSize=SCENINF.PageSize[i];
			break;
		};
	};
	if(TextSize&&MissText){
		vm_F1_VictoryCond.Clear();
		char* TmpText=znew(char,TextSize);
		bool Visible=true;
		char ch[2]={0,0};
		for(int i=0;i<TextSize;i++){
			char c=MissText[i];			
			if(c!='['){				
				if(Visible){
					if(c==13){
						c='\\';
						i++;
					}
					ch[0]=c;
					Miss.Add(ch);					
				};
			}else{
				//Miss.Add("{C}");
				char com=MissText[i+1];
				int pos=GetCPos(com);				
				Visible=pos>=0&&pos<52&&!SCENINF.TextDisable[pos];
				if(Visible&&com!='A'&&com!='Q'){
					//Miss.Add("{R FF840100 ");
					//ch[0]=com;
					//Miss.Add(ch);
					//Miss.Add("}");
				}
				i+=2;
			};
		};
		//C0665F57
		//Miss.Add("{C}");
		free(TmpText);
	};
};
void CreateMissText(){
	int TextSize=0;//SCENINF.TextSize;
	char* MissText=NULL;//SCENINF.MissText;
	for(int i=0;i<SCENINF.NPages;i++){
		if(!strcmp(SCENINF.PageID[i],"#BRIEFING")){
			MissText=SCENINF.Page[i];
			TextSize=SCENINF.PageSize[i];
		};
	};
	if(TextSize&&MissText){
		char* TmpText=znew(char,TextSize);
		int sz=0;
		bool Visible=true;
		for(int i=0;i<TextSize;i++){
			char c=MissText[i];
			if(c!='{'){
				if(Visible){
					TmpText[sz]=c;
					sz++;
				};
			}else{
				int pos=GetCPos(MissText[i+1]);
				if(pos>=0&&pos<52){
					Visible=!SCENINF.TextDisable[pos];
				};
				i+=2;
			};
		};
		ResFile RF=RRewrite("Miss.txt");
		if(RF!=INVALID_HANDLE_VALUE){
			RBlockWrite(RF,TmpText,sz);
			RClose(RF);
		};
		free(TmpText);
	}else{
		ResFile RF=RRewrite("Miss.txt");
		if(RF!=INVALID_HANDLE_VALUE){
			RClose(RF);
		};
	};
};
int ReadStr(char* Dst,char* Src,char c){
	int L=0;
	while(Src[L]!=c&&Src[L]!=0){
		Dst[L]=Src[L];
		L++;
	};
	Dst[L]=0;
	return L;
};
extern bool ProtectionMode;
#ifdef STARFORCE
extern HGLOBAL PTR_AI;
extern HGLOBAL PTR_MISS;
#endif
extern word COMPSTART[8];
void ClearCompstart(){
	memset(COMPSTART,0,sizeof COMPSTART);
}
void ScenaryInterface::Load(char* Name,char* Text){
	for(int i=0;i<8;i++)AssignTBL[i]=i;
	//ProtectionMode=1;
	//memset(COMPSTART,0,sizeof COMPSTART);
	ResFile RF=RReset(Text);
	//ProtectionMode=0;
	if(RF!=INVALID_HANDLE_VALUE){
		int sz=RFileSize(RF);
		if(sz){
			char* TMP=znew(char,sz+1);
			char* TMP1=znew(char,sz+1);
			RBlockRead(RF,TMP1,sz);
			char* STR0=TMP1;
			TMP1[sz]=0;
			bool exit=false;
			do{
				int L=ReadStr(TMP,STR0,' ');
				if(STR0[0]=='#'){
					if(NPages>=MaxPages){
						MaxPages+=32;
						Page=(char**)realloc(Page,4*MaxPages);
						PageID=(char**)realloc(PageID,4*MaxPages);
						PageBMP=(char**)realloc(PageBMP,4*MaxPages);
						PageSize=(int*)realloc(PageSize,4*MaxPages);
					};
					STR0+=L+1;
					PageID[NPages]=znew(char,strlen(TMP)+1);
					strcpy(PageID[NPages],TMP);
					L=ReadStr(TMP,STR0,0x0D);
					STR0+=L;
					PageBMP[NPages]=znew(char,strlen(TMP)+1);
					strcpy(PageBMP[NPages],TMP);
					if(STR0[0]==0x0D)STR0+=2;
					L=ReadStr(TMP,STR0,'#');
					Page[NPages]=znew(char,strlen(TMP)+1);
					PageSize[NPages]=L;
					strcpy(Page[NPages],TMP);
					NPages++;
					STR0+=L;
				}else exit=true;
			}while(!exit);
			free(TMP);
			free(TMP1);
		};
		CreateMissText();
		RClose(RF);
	};
	char cc3[200];
	strcpy(cc3,Name);
	strupr(cc3);
	if(strstr(cc3,".CMS")){
		FILE* F=fopen("UserMissions\\start.dat","w");
		if(F){
			fprintf(F,"%s",cc3);
			fclose(F);
			hLib=LoadLibrary("UserMissions\\CMS_start.dll");
		};
	}else{
#ifdef STARFORCE
		if(PTR_MISS&&((DWORD*)PTR_MISS)[0]==0x4351A234)hLib=LoadLibrary(Name);
		else hLib=NULL;
#else
		hLib=LoadLibrary(Name);
#endif
	};
	ScenaryHandler=NULL;
	if(hLib==NULL){
#ifndef STARFORCE
		ScenErr("Can't load DLL:%s",Name);
#endif
	}else{
		NErrors=0;
		ScenaryHandler=(StdVoid*)GetProcAddress(hLib,"ProcessScenary");
		if(!ScenaryHandler){
			ScenErr("%s: can't find function:void ProcessScenary()",Name);
		};
		if(NErrors){
			ScenErr("%s:Errors was found during initialisation.",Name);
			ScenaryHandler=NULL;
		};
	};
};

//-------------Registration commands--------------
void IntErr(char* Mess){
	ResFile FFF=RReset("ERR.LOG");
	if(FFF!=INVALID_HANDLE_VALUE){
		RClose(FFF);
		if(AiIsRunNow)MessageBox(NULL,Mess,"AI registration error...",MB_TOPMOST);
		else MessageBox(NULL,Mess,SCENINF.DLLName,MB_TOPMOST);
		assert(false);
	};
};
void IntErr(char* Mess,char* s1){
	char ccc[256];
	sprintf(ccc,Mess,s1);
	IntErr(ccc);
};
void TestUnitsGroup(int id){
	UnitsGroup* AG=&SCENINF.UGRP[id];
	int N=AG->N;
	word* idx=AG->IDS;
	word* sns=AG->SNS;
	for(int i=0;i<N;i++){
		word MID=idx[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&(OB->Serial&0xFFFE)==(sns[i]&0xFFFE)){
				sns[i]=OB->Serial;
			};
		};
	};
};
DLLEXPORT
void AssignNation(byte Src,byte Dst){
	if(Src<8&&Dst<8)AssignTBL[Src]=Dst;
};
CEXPORT
bool AddExGroupInfo(char* Name, GroupExInfo* Info){
	int N=AGroups.GetAmount();
	for(int i=0;i<N;i++)
		if(!strcmp(AGroups[i]->Name.pchar(),Name)){
			AGroups[i]->AdditionalInfo.Add(Info);
			return true;
		}
	return false;
}
DLLEXPORT 
bool RegisterUnits(GAMEOBJ* GOBJ,char* Name){
	for(int i=0;i<AGroups.GetAmount();i++)if(!strcmp(AGroups[i]->Name.pchar(),Name)){
		GOBJ->Type='UNIT';	
		UnitsGroup* UG=SCENINF.GetNewGroup();
		GOBJ->Index=UG->Index;		
		UG->IDS=znew(word,AGroups[i]->Units.GetAmount());
		UG->SNS=znew(word,AGroups[i]->Units.GetAmount());
		UG->N=AGroups[i]->Units.GetAmount();
		UG->Max=AGroups[i]->Units.GetAmount();
		for(int j=0;j<UG->N;j++){
			UG->IDS[j]=AGroups[i]->Units[j].ID;
			UG->SNS[j]=AGroups[i]->Units[j].SN;
		}
		UG->Index=UG->Index;
		GOBJ->Serial=0;
		return true;
	};
	GOBJ->Type=0;
	//IntErr("RegisterUnits : Unknown ID of group : %s",Name);
	//SCENINF.NErrors++;
	return false;
};
char* GetTextByID(char* ID);
DLLEXPORT 
bool RegisterString(GAMEOBJ* GOBJ,char* ID){
	char* id=GetTextByID(ID);
	if(!strcmp(id,ID)){
		IntErr("RegisterString : Unknown string ID : %s ,(see COMMENT.TXT)",ID);
		SCENINF.NErrors++;
	};
	GOBJ->Type='STRI';
	GOBJ->Index=SCENINF.NMess;
	GOBJ->Serial=0;
	if(SCENINF.NMess>=SCENINF.MaxMess){
		SCENINF.MaxMess+=64;
		SCENINF.Messages=(char**)realloc(SCENINF.Messages,SCENINF.MaxMess*4);
	};
	SCENINF.Messages[SCENINF.NMess]=id;
	SCENINF.NMess++;
	return true;
};
DLLEXPORT
bool RegisterSound(GAMEOBJ* GOBJ,char* Name){
	GOBJ->Type='SOUN';
	GOBJ->Index=SCENINF.NSnd;
	GOBJ->Serial=0;
	if(SCENINF.NSnd>=SCENINF.MaxSnds){
		SCENINF.MaxSnds+=32;
		SCENINF.Sounds=(char**)realloc(SCENINF.Sounds,4*SCENINF.MaxSnds);
	};
	SCENINF.Sounds[SCENINF.NSnd]=znew(char,strlen(Name)+1);
	strcpy(SCENINF.Sounds[SCENINF.NSnd],Name);
	SCENINF.NSnd++;
	return true;
};
DLLEXPORT
void RegisterVar(void* Var,int size){
	if(SCENINF.NSaves>=SCENINF.MaxSaves){
		SCENINF.MaxSaves+=32;
		SCENINF.SaveZone=(void**)realloc(SCENINF.SaveZone,4*SCENINF.MaxSaves);
		SCENINF.SaveSize=(int*)realloc(SCENINF.SaveSize,4*SCENINF.MaxSaves);
	};
	SCENINF.SaveSize[SCENINF.NSaves]=size;
	SCENINF.SaveZone[SCENINF.NSaves]=Var;
	SCENINF.NSaves++;
};
DLLEXPORT
void RegisterZoneEx(GAMEOBJ* GOBJ,char* Name, bool ErrRaport){
	int NZON=0;
	word ZIDS[64];
	for(int i=0;i<AZones.GetAmount();i++){
		if(!strcmp(Name,AZones[i]->Name.pchar())){
			if(NZON<64){
				ZIDS[NZON]=i;
				NZON++;
				AZones[i]->Dir=0;
			};
		};
	};
	if(NZON){
		ZonesGroup* ZG=SCENINF.GetNewZone();
		ZG->N=NZON;
		ZG->ZoneID=znew(word,NZON*2);
		memcpy(ZG->ZoneID,ZIDS,NZON*2);
		GOBJ->Index=ZG->Index;
		GOBJ->Serial=0;
		GOBJ->Type='ZONE';
	}else{
		if(ErrRaport){
			IntErr("RegisterZone : Unknown zone name : %s",Name);
			SCENINF.NErrors++;
		}
	};
};
DLLEXPORT
void RegisterZone(GAMEOBJ* GOBJ,char* Name){
	RegisterZoneEx(GOBJ,Name,true);
}
DLLEXPORT 
void RegisterVisibleZone(GAMEOBJ* GOBJ,char* Name){
	int NZON=0;
	word ZIDS[64];
	for(int i=0;i<AZones.GetAmount();i++){
		if(!strcmp(Name,AZones[i]->Name.pchar())){
			if(NZON<64){
				ZIDS[NZON]=i;
				NZON++;
				AZones[i]->Dir=117;
			};
		};
	};
	if(NZON){
		ZonesGroup* ZG=SCENINF.GetNewZone();
		ZG->N=NZON;
		ZG->ZoneID=znew(word,NZON*2);
		memcpy(ZG->ZoneID,ZIDS,NZON*2);
		GOBJ->Index=ZG->Index;
		GOBJ->Serial=0;
		GOBJ->Type='ZONE';
	}else{	
		IntErr("RegisterZone : Unknown zone name : %s",Name);
		SCENINF.NErrors++;
	};
};
DLLEXPORT
bool RegisterUnitType(GAMEOBJ* GOBJ,char* Name){
	if(!Name)return false;
	GeneralObject** GOS=NATIONS[0].Mon;
	int N=NATIONS[0].NMon;
	for(int i=0;i<N;i++)if(!strcmp(GOS[i]->MonsterID,Name)){
		GOBJ->Index=i;
		GOBJ->Serial=0;
		GOBJ->Type='UTYP';
		return true;
	};
	IntErr("RegisterUnitType : Unknown unit type : %s",Name);
	SCENINF.NErrors++;
	return false;
};
DLLEXPORT
bool RegisterUpgrade(GAMEOBJ* GOBJ,char* Name){
	NewUpgrade** NUP=NATIONS[0].UPGRADE;
	int N=NATIONS[0].NUpgrades;
	for(int i=0;i<N;i++){
		if(!strcmp(NUP[i]->Name,Name)){
			GOBJ->Index=i;
			GOBJ->Serial=0;
			GOBJ->Type='UPGR';
			return true;
		};
	};
	IntErr("RegisterUpgrade : Unknown upgrade : %s",Name);
	SCENINF.NErrors++;
	return false;
};
DLLEXPORT
bool SafeRegisterUpgrade(GAMEOBJ* GOBJ,char* Name){
	NewUpgrade** NUP=NATIONS[0].UPGRADE;
	int N=NATIONS[0].NUpgrades;
	for(int i=0;i<N;i++){
		if(!strcmp(NUP[i]->Name,Name)){
			GOBJ->Index=i;
			GOBJ->Serial=0;
			GOBJ->Type='UPGR';
			return true;
		};
	};
	GOBJ->Index=0;
	GOBJ->Serial=0;
	GOBJ->Type='SAFE';
	return true;
};
void PerformNewUpgrade(Nation* NT,int UIndex,OneObject* OB);
DLLEXPORT
void InitialUpgrade(char* Grp,char* Upgrade){
	for(int i=0;i<AGroups.GetAmount();i++)if(!strcmp(AGroups[i]->Name.pchar(),Grp)){
		GAMEOBJ UP;
		if(RegisterUpgrade(&UP,Upgrade)){
			int Nu=AGroups[i]->Units.GetAmount();
			for(int j=0;j<Nu;j++){
				word MID=AGroups[i]->Units[j].ID;
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==AGroups[i]->Units[j].SN&&!OB->Sdoxlo){
						PerformNewUpgrade(OB->Nat,UP.Index,OB);
						NewUpgrade* NU=OB->Nat->UPGRADE[UP.Index];
						if(!(NU->Individual||NU->StageUp)){
							NU->Done=true;
							NU->PermanentEnabled=0;
							NU->Enabled=0;
						};
						return;
					};
				};
			};
		};
		return;
	};
	IntErr("InitialUpgrade : Unknown <Group>");
};
DLLEXPORT
void InitialUpgradeNew(byte NI,GAMEOBJ* Upg,word Index){
	if(Upg->Type=='UPGR'){
		NewUpgrade* NU=NATIONS[NI].UPGRADE[Upg->Index];
		if(Index==0xFFFF){
			PerformNewUpgrade(NATIONS+NI,Upg->Index,NULL);
		}else{
			OneObject* OB=Group[Index];
			if(OB&&!OB->Sdoxlo){
				NI=OB->NNUM;
				PerformNewUpgrade(NATIONS+NI,Upg->Index,Group[Index]);
			};
		};
		if(!(NU->Individual||NU->StageUp)){
			NU->Done=true;
			NU->PermanentEnabled=0;
			NU->Enabled=0;
		};
	};
};
DLLEXPORT
void DisableUpgrade(byte Nat,GAMEOBJ* Upg){
	Nat=AssignTBL[Nat];
	if(Nat<8){
		if(Upg->Type!='UPGR'){
			IntErr("DisableUpgrade : Incorrect parameter <Upgrade>");
			SCENINF.NErrors++;
			return;
		};
		NewUpgrade* NU=NATIONS[Nat].UPGRADE[Upg->Index];
		NU->ManualDisable=1;
		/*
		NU->Enabled=false;
		NU->PermanentEnabled=false;
		NU->Done=true;
		*/
	};
};
DLLEXPORT
void EnableUpgrade(byte Nat,GAMEOBJ* Upg){
	Nat=AssignTBL[Nat];
	if(Nat<8){
		if(Upg->Type!='UPGR'){
			IntErr("DisableUpgrade : Incorrect parameter <Upgrade>");
			SCENINF.NErrors++;
			return;
		};
		NewUpgrade* NU=NATIONS[Nat].UPGRADE[Upg->Index];
		NU->ManualDisable=0;
		/*
		NU->Enabled=false;
		NU->PermanentEnabled=false;
		NU->Done=true;
		*/
	};
};
DLLEXPORT
void SetUpgradeEnableStatus(byte Nat,GAMEOBJ* Upg,int state){
	Nat=AssignTBL[Nat];
	if(Nat<8){
		if(Upg->Type!='UPGR'){
			IntErr("DisableUpgrade : Incorrect parameter <Upgrade>");
			SCENINF.NErrors++;
			return;
		};
		NewUpgrade* NU=NATIONS[Nat].UPGRADE[Upg->Index];
		NU->ManualEnable=0;
		NU->ManualDisable=0;
		if(state>0)NU->ManualEnable=1;
		else if(state<0)NU->ManualDisable=1;
	};
};
DLLEXPORT
void SetUnitEnableState(byte Nat,GAMEOBJ* Type,int state){
	Nat=AssignTBL[Nat];
	if(Nat<8){
		if(Type->Type!='UTYP'){
			IntErr("EnableUnit : Incorrect parameter <Type>");
			SCENINF.NErrors++;
			return;
		};
		GeneralObject* GO=NATIONS[Nat].Mon[Type->Index];
		GO->ManualEnable=0;
		GO->ManualDisable=0;
		if(state>0)GO->ManualEnable=1;
		else if(state<0)GO->ManualDisable=1;
	};
};
DLLEXPORT
void EnableUnit(byte Nat,GAMEOBJ* Type,bool State){
	Nat=AssignTBL[Nat];
	if(Nat<8){
		if(Type->Type!='UTYP'){
			IntErr("EnableUnit : Incorrect parameter <Type>");
			SCENINF.NErrors++;
			return;
		};
		GeneralObject* GO=NATIONS[Nat].Mon[Type->Index];
		GO->ManualDisable=!State;
		GO->MoreCharacter->Changed=1;
		if(State)GO->Enabled=1;
	};
};

DLLEXPORT
bool RegisterFormation(GAMEOBJ* GOBJ,char* Name){
	for(int i=0;i<NEOrders;i++){
		if(!strcmp(ElementaryOrders[i].ID,Name)){
			GOBJ->Index=i;
			GOBJ->Serial=0;
			GOBJ->Type='FORM';
			return true;
		};
	};
	IntErr("RegisterFormation : Unknown formation type :%s  (see ORDERS.LST)",Name);
	SCENINF.NErrors++;
	return false;
};
DLLEXPORT
bool RegisterUnitsForm(GAMEOBJ* GOBJ,char* Name){
	return false;
};
//---------------Checking commands----------------
//1.In zones
DLLEXPORT
int GetUnitsAmount0(GAMEOBJ* Zone,byte Nation){
	Nation=AssignTBL[Nation];
	if(Nation>=8){
		IntErr("GetUnitsAmount(Zone,Nation) : Incorrect parameter <Nation>");
		SCENINF.NErrors++;
		return 0;
	};
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		int R0=Zone->Type&0x00FFFFFF;
		int R=(R0>>7)+2;
		int cx=Zone->Index>>7;
		int cy=Zone->Serial>>7;

		int zx=int(Zone->Index)<<4;
		int zy=int(Zone->Serial)<<4;
		int Rz=R0<<4;

		int mxx=msx>>2;
		int myy=msy>>2;
		int NU=0;

		for(int r=0;r<R;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int Np=Rarr[r].N;
			for(int p=0;p<Np;p++){
				int xp=cx+xi[p];
				int yp=cy+yi[p];
				if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
					int cell=xp+1+((yp+1)<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->NNUM==Nation&&(!OB->Sdoxlo)&&
									Norma(OB->RealX-zx,OB->RealY-zy)<Rz)NU++;
							};
						};
					};
				};
			};
		};
		return NU;
	};
	if((!Zone)||Zone->Type!='ZONE'){
		IntErr("GetUnitsAmount(Zone,Nation) : Incorrect parameter <Zone>");
		SCENINF.NErrors++;
		return 0;
	};
	
	ZonesGroup* ZGRP=&SCENINF.ZGRP[Zone->Index];
	assert(SCENINF.ZGRP.GetAmount()>=Zone->Index);
	int mxx=msx>>2;
	int myy=msy>>2;
	int NU=0;
	for(int j=0;j<ZGRP->N;j++){
		ActiveZone* AZ=AZones[ZGRP->ZoneID[j]];
		int R=(AZ->R>>7)+2;
		int cx=AZ->x>>7;
		int cy=AZ->y>>7;

		int zx=AZ->x<<4;
		int zy=AZ->y<<4;
		int Rz=AZ->R<<4;

		for(int r=0;r<R;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int Np=Rarr[r].N;
			for(int p=0;p<Np;p++){
				int xp=cx+xi[p];
				int yp=cy+yi[p];
				if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
					int cell=xp+1+((yp+1)<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->NNUM==Nation&&(!OB->Sdoxlo)&&
									Norma(OB->RealX-zx,OB->RealY-zy)<Rz)NU++;
							};
						};
					};
				};
			};
		};
	};
	return NU;
};
void CopyReIm(byte NI);
DLLEXPORT
void ClearSelection(byte Nat);
extern short AlarmSoundID;
extern int AlarmDelay;
DLLEXPORT
void ShowAlarm(GAMEOBJ* Zone){
	int x=0;
	int y=0;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		x=int(Zone->Index);
		y=int(Zone->Serial);
	}else
	if(Zone->Type=='ZONE'){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		x=AZ->x;
		y=AZ->y;
	}else{
		IntErr("ShowAlarm : Invalid <Zone>");
		SCENINF.NErrors++;
		return;
	};	
	TryAlarm(x,y,0);
};
void AddUnitToSelected(byte NI,OneObject* OB);
DLLEXPORT void SelectUnitsInZone(GAMEOBJ* Zone,byte Nation,bool add){
	Nation=AssignTBL[Nation];
	if(Nation>=8){
		IntErr("GetUnitsAmount(Zone,Nation) : Incorrect parameter <Nation>");
		SCENINF.NErrors++;
		return;
	};
	if(!add)ClearSelection(Nation);
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		int R0=Zone->Type&0x00FFFFFF;
		int R=(R0>>7)+2;
		int cx=Zone->Index>>7;
		int cy=Zone->Serial>>7;

		int zx=int(Zone->Index)<<4;
		int zy=int(Zone->Serial)<<4;
		int Rz=R0<<4;

		int mxx=msx>>2;
		int myy=msy>>2;
		int NU=0;

		for(int r=0;r<R;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int Np=Rarr[r].N;
			for(int p=0;p<Np;p++){
				int xp=cx+xi[p];
				int yp=cy+yi[p];
				if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
					int cell=xp+1+((yp+1)<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->NNUM==Nation&&(!OB->Sdoxlo)&&
									Norma(OB->RealX-zx,OB->RealY-zy)<Rz){
									AddUnitToSelected(Nation,OB);
								};
							};
						};
					};
				};
			};
		};
		CopyReIm(Nation);
		return;
	};
	if((!Zone)||Zone->Type!='ZONE'){
		IntErr("GetUnitsAmount(Zone,Nation) : Incorrect parameter <Zone>");
		SCENINF.NErrors++;
		return;
	};
	
	ZonesGroup* ZGRP=&SCENINF.ZGRP[Zone->Index];
	assert(SCENINF.ZGRP.GetAmount()>=Zone->Index);
	int mxx=msx>>2;
	int myy=msy>>2;
	int NU=0;
	for(int j=0;j<ZGRP->N;j++){
		ActiveZone* AZ=AZones[ZGRP->ZoneID[j]];
		int R=(AZ->R>>7)+2;
		int cx=AZ->x>>7;
		int cy=AZ->y>>7;

		int zx=AZ->x<<4;
		int zy=AZ->y<<4;
		int Rz=AZ->R<<4;

		for(int r=0;r<R;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int Np=Rarr[r].N;
			for(int p=0;p<Np;p++){
				int xp=cx+xi[p];
				int yp=cy+yi[p];
				if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
					int cell=xp+1+((yp+1)<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->NNUM==Nation&&(!OB->Sdoxlo)&&
									Norma(OB->RealX-zx,OB->RealY-zy)<Rz)
									AddUnitToSelected(Nation,OB);
							};
						};
					};
				};
			};
		};
	};
	CopyReIm(Nation);
	return;
};
DLLEXPORT
void ChangeUnitParam(GAMEOBJ* Type,byte NI,int AddDamage,int DamType,int AddShield){
	if(NI>=8||DamType>4)return;
	if(Type->Type!='UTYP'){
		IntErr("ChangeUnitParam(Type,Nation,...) : Incorrect Type");
		SCENINF.NErrors++;
		return;
	};
	NATIONS[NI].Mon[Type->Index]->MoreCharacter->Shield+=AddShield;
	NATIONS[NI].Mon[Type->Index]->MoreCharacter->MaxDamage[DamType]+=AddDamage;
	NATIONS[NI].Mon[Type->Index]->MoreCharacter->Changed=1;
};
DLLEXPORT
void SelectTypeOfUnitsInZone(GAMEOBJ* Zone,GAMEOBJ* Type,byte Nation,bool add){
	Nation=AssignTBL[Nation];
	if(Nation>=8){
		IntErr("GetTypeOfUnitsAmount(Zone,Type,Nation) : Incorrect parameter <Nation>");
		SCENINF.NErrors++;
		return;
	};
	if(Type->Type!='UTYP'){
		IntErr("GetTypeOfUnitsAmount(Zone,Type,Nation) : Incorrect parameter <Type>");
		SCENINF.NErrors++;
		return;
	};
	if(!add)ClearSelection(Nation);
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		int R0=Zone->Type&0x00FFFFFF;
		int R=(R0>>7)+2;
		int cx=Zone->Index>>7;
		int cy=Zone->Serial>>7;

		int zx=int(Zone->Index)<<4;
		int zy=int(Zone->Serial)<<4;
		int Rz=R0<<4;

		int mxx=msx>>2;
		int myy=msy>>2;
		int NU=0;

		for(int r=0;r<R;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int Np=Rarr[r].N;
			for(int p=0;p<Np;p++){
				int xp=cx+xi[p];
				int yp=cy+yi[p];
				if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
					int cell=xp+1+((yp+1)<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->NNUM==Nation&&OB->NIndex==Type->Index&&(!OB->Sdoxlo)&&
									Norma(OB->RealX-zx,OB->RealY-zy)<Rz){
									AddUnitToSelected(Nation,OB);
								};
							};
						};
					};
				};
			};
		};
		CopyReIm(Nation);
		return;
	};
	if((!Zone)||Zone->Type!='ZONE'){
		IntErr("GetTypeOfUnitsAmount(Zone,Type,Nation) : Incorrect parameter <Zone>");
		SCENINF.NErrors++;
		return;
	};
	
	ZonesGroup* ZGRP=&SCENINF.ZGRP[Zone->Index];
	assert(SCENINF.ZGRP.GetAmount()>=Zone->Index);
	int mxx=msx>>2;
	int myy=msy>>2;
	int NU=0;
	for(int j=0;j<ZGRP->N;j++){
		ActiveZone* AZ=AZones[ZGRP->ZoneID[j]];
		int R=(AZ->R>>7)+2;
		int cx=AZ->x>>7;
		int cy=AZ->y>>7;

		int zx=AZ->x<<4;
		int zy=AZ->y<<4;
		int Rz=AZ->R<<4;

		for(int r=0;r<R;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int Np=Rarr[r].N;
			for(int p=0;p<Np;p++){
				int xp=cx+xi[p];
				int yp=cy+yi[p];
				if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
					int cell=xp+1+((yp+1)<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->NNUM==Nation&&OB->NIndex==Type->Index&&(!OB->Sdoxlo)&&
									Norma(OB->RealX-zx,OB->RealY-zy)<Rz)
									AddUnitToSelected(Nation,OB);
							};
						};
					};
				};
			};
		};
	};
	CopyReIm(Nation);
	return;
};
DLLEXPORT
int GetUnitsAmount1(GAMEOBJ* Zone,GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("GetUnitsAmount(Zone,Units) : Incorrect parameter <Units>");
		return 0;
	};
	TestUnitsGroup(Units->Index);
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		int R0=Zone->Type&0x00FFFFFF;
		int cx=int(Zone->Index)<<4;
		int cy=int(Zone->Serial)<<4;
		int R=R0<<4;
		word* UIDS=SCENINF.UGRP[Units->Index].IDS;
		word* SIDS=SCENINF.UGRP[Units->Index].SNS;
		int Nu=SCENINF.UGRP[Units->Index].N;
		int NU=0;
		for(int p=0;p<Nu;p++){
			word MID=UIDS[p];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(OB->GetLiveState()==ls_LiveBody)&&OB->Serial==SIDS[p]&&Norma(OB->RealX-cx,OB->RealY-cy)<R)NU++;
			};
		};
		return NU;
	};
	if(Zone->Type!='ZONE'){
		IntErr("GetUnitsAmount(Zone,Units) : Incorrect parameter <Zone>");
		return 0;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	ZonesGroup* ZGRP=&SCENINF.ZGRP[Zone->Index];
	assert(SCENINF.ZGRP.GetAmount()>=Zone->Index);
	int NU=0;
	for(int j=0;j<ZGRP->N;j++){
		ActiveZone* AZ=AZones[ZGRP->ZoneID[j]];
		int R=(AZ->R)<<4;
		int cx=AZ->x<<4;
		int cy=AZ->y<<4;
		for(int p=0;p<Nu;p++){
			word MID=UIDS[p];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==SIDS[p]&&Norma(OB->RealX-cx,OB->RealY-cy)<R)NU++;
			};
		};
	};
	return NU;
};
DLLEXPORT
int GetUnitsAmount2(GAMEOBJ* Zone,GAMEOBJ* UnitType,byte Nation){
	Nation=AssignTBL[Nation];
	if(Nation>=8){
		IntErr("GetUnitsAmount(Zone,UnitType,Nation) : Incorrect parameter <Nation>");
		SCENINF.NErrors++;
		return 0;
	};
	if(UnitType->Type!='UTYP'){
		IntErr("GetUnitsAmount(Zone,UnitType,Nation) : Incorrect parameter <UnitType>");
		SCENINF.NErrors++;
		return 0;
	};
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		GeneralObject* GO=NATIONS[Nation].Mon[UnitType->Index];
		int R0=Zone->Type&0x00FFFFFF;
		int R=(R0>>7)+2;
		int cx=Zone->Index>>7;
		int cy=Zone->Serial>>7;

		int zx=int(Zone->Index)<<4;
		int zy=int(Zone->Serial)<<4;
		int Rz=R0<<4;

		int mxx=msx>>2;
		int myy=msy>>2;
		int NU=0;
		for(int r=0;r<R;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int Np=Rarr[r].N;
			for(int p=0;p<Np;p++){
				int xp=cx+xi[p];
				int yp=cy+yi[p];
				if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
					int cell=xp+1+((yp+1)<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->Ref.General==GO&&(!OB->Sdoxlo)&&
									Norma(OB->RealX-zx,OB->RealY-zy)<Rz)NU++;
							};
						};
					};
				};
			};
		};
		return NU;
	};
	if(Zone->Type!='ZONE'){
		IntErr("GetUnitsAmount(Zone,UnitType,Nation) : Incorrect parameter <Zone>");
		SCENINF.NErrors++;
		return 0;
	};
	GeneralObject* GO=NATIONS[Nation].Mon[UnitType->Index];
	ZonesGroup* ZGRP=&SCENINF.ZGRP[Zone->Index];
	assert(SCENINF.ZGRP.GetAmount()>=Zone->Index);
	int mxx=msx>>2;
	int myy=msy>>2;
	int NU=0;
	for(int j=0;j<ZGRP->N;j++){
		ActiveZone* AZ=AZones[ZGRP->ZoneID[j]];
		int R=(AZ->R>>7)+2;
		int cx=AZ->x>>7;
		int cy=AZ->y>>7;

		int zx=AZ->x<<4;
		int zy=AZ->y<<4;
		int Rz=AZ->R<<4;

		for(int r=0;r<R;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int Np=Rarr[r].N;
			for(int p=0;p<Np;p++){
				int xp=cx+xi[p];
				int yp=cy+yi[p];
				if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
					int cell=xp+1+((yp+1)<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->Ref.General==GO&&(!OB->Sdoxlo)&&
									Norma(OB->RealX-zx,OB->RealY-zy)<Rz)NU++;
							};
						};
					};
				};
			};
		};
	};
	return NU;
};
//2.total
DLLEXPORT
int GetTotalAmount0(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("GetTotalAmount(Units) : Invalid parameter <Units>");
		SCENINF.NErrors++;
		return 0;
	};
	TestUnitsGroup(Units->Index);
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	int NU=0;
	for(int i=0;i<Nu;i++){
		word MID=UIDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&OB->Serial==SIDS[i])NU++;
		};
	};
	return NU;
};
DLLEXPORT
int	GetNUnitsInBrigade(byte NI,int BrigID){
	if(NI>7||BrigID>=MaxBrig) return 0;
	return CITY[NI].Brigs[BrigID].NMemb;
}
class Brigade_LV
{
public:
	word	ID;
	word	SN;
	word	NMemeb;
	word*	Memb;
	word*	MembSN;
	bool	Enabled;
};
DLLEXPORT
void	GetBrigadeLV(byte NI,word BrigID,Brigade_LV* pBR){
	if (pBR&&NI<8&&BrigID<MaxBrig) {
		pBR->ID = CITY[NI].Brigs[BrigID].ID;
		pBR->SN	= CITY[NI].Brigs[BrigID].SN;
		pBR->Enabled= CITY[NI].Brigs[BrigID].Enabled;
		pBR->NMemeb = CITY[NI].Brigs[BrigID].NMemb;
		for (int i=0; i<pBR->NMemeb; i++){
			pBR->Memb[i]	= CITY[NI].Brigs[BrigID].Memb[i];
			pBR->MembSN[i]	= CITY[NI].Brigs[BrigID].MembSN[i];
		}
	}
}
DLLEXPORT
word	GetBrigadeSN(byte NI,word BrigID){
	if (NI<8&&BrigID<MaxBrig){
		return CITY[NI].Brigs[BrigID].SN;
	}
	return 0xFFFF;
}
DLLEXPORT
bool	GetBrigadeEnable(byte NI,word BrigID){
	if (NI<8&&BrigID<MaxBrig){
		return CITY[NI].Brigs[BrigID].Enabled;
	}
	return false;
}
DLLEXPORT
bool	SetBrigadeAutoFormID(byte NI,word BrigID,word FormID) { //grey  [12/01/2003]
	if (NI<8&&BrigID<MaxBrig){
		CITY[NI].Brigs[BrigID].AutoFormID = FormID;
		return true;
	}
	return false;
}
DLLEXPORT
word	GetCITYNBrigs(byte NI) { //grey  [16/12/2004]
	if( NI>=0 && NI<8 )  return CITY[NI].NBrigs;
	return 0;
}
DLLEXPORT
bool	SetBrigadeNewMorale(byte NI,word BrigID,word NewMorale,int kills=-1) { //grey  [03/02/2004]
	if( NI<8 && BrigID<MaxBrig && CITY[NI].Brigs[BrigID].Enabled ) 
	{
		if( NewMorale == 0xEEEE ) {							// make super-morale squadron
			CITY[NI].Brigs[BrigID].Morale		 = CITY[NI].Brigs[BrigID].MaxMorale ; 
			if( CITY[NI].Brigs[BrigID].Morale < CITY[NI].Brigs[BrigID].AddMaxMorale ) 
				CITY[NI].Brigs[BrigID].Morale += CITY[NI].Brigs[BrigID].AddMaxMorale ;
			CITY[NI].Brigs[BrigID].AddMaxMorale  = 10000 * 10000 ; // 10000 * 10000 ; 
		}
		if( NewMorale == 0xAAAA && kills > 0 ) {			// add some Kills for squadron (rase Exps)
			CITY[NI].Brigs[BrigID].NKills += kills * 100 ;
			CITY[NI].Brigs[BrigID].Morale  = CITY[NI].Brigs[BrigID].MaxMorale = 10000 * 10000 ;
		} 
		if( NewMorale <= 0x1111 && kills == -1 ) {			// dec morale for squadron
			CITY[NI].Brigs[BrigID].Morale = CITY[NI].Brigs[BrigID].PrevMorale = 10000 * NewMorale ; //-=33
		} 
		return true;
	}
	return false;
}
DLLEXPORT
bool	SetBrigadeNoMorale(byte NI,word BrigID,bool state) { //grey  [14/07/2004]
	return true; // grey [03.12.2004]
	//if( NI<8 && BrigID<MaxBrig ) {
	//	Brigade* BR=CITY[NI].Brigs + BrigID;
	//	if(BR) {
	//		for(int i=0; i < BR->NMemb ; i++) {
	//			if(BR->Memb[i]!=0xFFFF) {
	//				OneObject* OB=Group[ BR->Memb[i] ];
	//				if(OB&&OB->newMons) 
	//					///OB->newMons->NoMorale = state;	// COMMENT BY GEC&GREY
	//			}
	//		}
	//		return true;	
	//	}	
	//}
	//return false;
}
DLLEXPORT
int	GetBrigadesID(byte NI,GAMEOBJ* pGrp){
	TestUnitsGroup(pGrp->Index);
	word* UIDS=SCENINF.UGRP[pGrp->Index].IDS;
	word* SIDS=SCENINF.UGRP[pGrp->Index].SNS;
	int	  NU  =SCENINF.UGRP[pGrp->Index].N;
	for (int i=0; i<NU; i++){
		word MID=UIDS[i];
		if (MID!=0xFFFF) {
			OneObject* OB=Group[MID];
			if (OB&&(OB->GetLiveState()==ls_LiveBody)&&OB->Serial==SIDS[i]){
				if (OB->BrigadeID!=0xFFFF) {
					return OB->BrigadeID;
				};
			};
		};
	};
	return 0xFFFF;
};
DLLEXPORT
int GetBrigadsAmount0(byte NI){
	if (NI<0||7<NI) {
		IntErr("GetBrigadsAmount(byte NI) : Invalid parametr <NI>");
		SCENINF.NErrors++;
		return 0;
	}
	int N_BR = 0;
	for (int i=0; i<CITY[NI].NBrigs; i++){
		if (CITY[NI].Brigs[i].Enabled)	N_BR++;
	}
	return N_BR;
}
DLLEXPORT
int GetBrigadsAmount(GAMEOBJ* pGrp){
	if (pGrp->Type!='UNIT') {
		IntErr("GetBrigadsAmount(GAMEOBJ* pGrp) : Invalid parametr <pGrp>");
		SCENINF.NErrors++;
		return	0;
	}
	TestUnitsGroup(pGrp->Index);
	word* UIDS=SCENINF.UGRP[pGrp->Index].IDS;
	word* SIDS=SCENINF.UGRP[pGrp->Index].SNS;
	int	  NU  =SCENINF.UGRP[pGrp->Index].N;
	int	  SQNUM=0;
	int	  SQID[256];
    for (int i=0;i<256;i++)	SQID[i]=-1;
	for (i=0; i<NU; i++){
		word MID=UIDS[i];
		if (MID!=0xFFFF) {
			OneObject* OB=Group[MID];
			if (OB&&(OB->GetLiveState()==ls_LiveBody)&&OB->Serial==SIDS[i]) {
				if (OB->BrigadeID!=0xFFFF) {
					for (int j=0;j<256;j++){
						if (SQID[j]==OB->BrigadeID) {
							j=256;
						}else
							if (SQID[j]==-1) {
								SQID[j]=OB->BrigadeID;
								SQNUM++;
								j=256;
							}
					}
				}
			}
		}
	}
	return SQNUM;
}
DLLEXPORT int GetTotalAmount1(GAMEOBJ* UnitType,byte Nation){
	Nation=AssignTBL[Nation];
	if(UnitType->Type!='UTYP'){
		IntErr("GetTotalAmount(UnitType,Nation) : Invalid parameter <UnitType>");
		SCENINF.NErrors++;
		return 0;
	};
	if(Nation>=8){
		IntErr("GetTotalAmount(UnitType,Nation) : Invalid parameter <Nation>");
		SCENINF.NErrors++;
		return 0;
	};
	if(UnitType->Index<NATIONS->NMon)return NATIONS[Nation].CITY->UnitAmount[UnitType->Index];
	else return 0;
};
DLLEXPORT int GetTotalAmount2(GAMEOBJ* Units,GAMEOBJ* UnitType,byte Nation){
	Nation=AssignTBL[Nation];
	if(UnitType->Type!='UTYP'){
		IntErr("GetTotalAmount(UnitType,Nation) : Invalid parameter <UnitType>");
		SCENINF.NErrors++;
		return 0;
	};
	if(Units->Type!='UNIT'){
		IntErr("GetTotalAmount(Units) : Invalid parameter <Units>");
		SCENINF.NErrors++;
		return 0;
	};
	TestUnitsGroup(Units->Index);
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	int NU=0;
	int uid=UnitType->Index;
	for(int i=0;i<Nu;i++){
		word MID=UIDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==SIDS[i]&&OB->NNUM==Nation&&OB->NIndex==uid)NU++;
		};
	};
	return NU;
};
DLLEXPORT int GetReadyAmount(GAMEOBJ* UnitType,byte Nation){
	Nation=AssignTBL[Nation];
	if(UnitType->Type!='UTYP'){
		IntErr("GetReadyAmount(UnitType,Nation) : Invalid parameter <UnitType>");
		SCENINF.NErrors++;
		return 0;
	};
	if(Nation>=8){
		IntErr("GetReadyAmount(UnitType,Nation) : Invalid parameter <Nation>");
		SCENINF.NErrors++;
		return 0;
	};
	return NATIONS[Nation].CITY->ReadyAmount[UnitType->Index];
};
DLLEXPORT
int GetAmountOfWarriors(byte Nat){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("GetAmountOfWarriors : Invalid <Nat>");
		return 0;
	};
	word* Mons=NatList[Nat];
	int N=NtNUnits[Nat];
	int NU=0;
	for(int i=0;i<N;i++){
		OneObject* OB=Group[i];
		if(OB&&!OB->Sdoxlo){
			if(!(OB->NewBuilding||OB->newMons->Capture||OB->LockType)){
				NU++;
			};
		};
	};
	return NU;
};
//3.Upgrades
DLLEXPORT
bool IsUpgradeDoing(GAMEOBJ* Upgrade,byte Nation){
	Nation=AssignTBL[Nation];
	if(Upgrade->Type!='UPGR'){
		IntErr("IsUpgradeDoung(Upgrade,Nation) : Incorrect parameter <Upgrade>");
		SCENINF.NErrors++;
		return 0;
	};
	if(Nation>=8){
		IntErr("IsUpgradeDoing(Upgrade,Nation) : Invalid parameter <Nation>");
		SCENINF.NErrors++;
		return 0;
	};
	return NATIONS[Nation].UPGRADE[Upgrade->Index]->IsDoing;
};
DLLEXPORT
bool IsUpgradeDone(GAMEOBJ* Upgrade,byte Nation){
	Nation=AssignTBL[Nation];
	if(Upgrade->Type!='UPGR'){
		IntErr("IsUpgradeDone(Upgrade,Nation) : Incorrect parameter <Upgrade>");
		SCENINF.NErrors++;
		return 0;
	};
	if(Nation>=8){
		IntErr("IsUpgradeDone(Upgrade,Nation) : Invalid parameter <Nation>");
		SCENINF.NErrors++;
		return 0;
	};
	return NATIONS[Nation].UPGRADE[Upgrade->Index]->Done;
};
DLLEXPORT
bool IsUpgradeEnabled(GAMEOBJ* Upgrade,byte Nation){
	Nation=AssignTBL[Nation];
	if(Upgrade->Type!='UPGR'){
		IntErr("IsUpgradeEnabled(Upgrade,Nation) : Incorrect parameter <Upgrade>");
		SCENINF.NErrors++;
		return 0;
	};
	if(Nation>=8){
		IntErr("IsUpgradeEnabled(Upgrade,Nation) : Invalid parameter <Nation>");
		SCENINF.NErrors++;
		return 0;
	};
	return NATIONS[Nation].UPGRADE[Upgrade->Index]->Enabled;
};
//4.Died units
DLLEXPORT
int GetDied(GAMEOBJ* UnitType,byte Nation){
	Nation=AssignTBL[Nation];
	if(UnitType->Type!='UNIT'){
		IntErr("GetDied(UnitType,Nation) : Invalid parameter <UnitType>");
		SCENINF.NErrors++;
		return 0;
	};
	if(Nation>=8){
		IntErr("GetDied(UnitType,Nation) : Invalid parameter <Nation>");
		SCENINF.NErrors++;
		return 0;
	};
	return NATIONS[Nation].NKilled[UnitType->Index];
};
//---------------Creating/Erasure commands--------------
int CreateNewTerrMons2(byte NI,int x,int y,word Type);
DLLEXPORT
bool CreateObject0(GAMEOBJ* DstObj,GAMEOBJ* Form,GAMEOBJ* UnitType,byte NatID,GAMEOBJ* Zone,byte Direction){
	NatID=AssignTBL[NatID];
	if(NatID>=8){
		IntErr("CreateObject(DstObj,Formation,UnitType,Nation,Zone,Direction) : Invalid parameter <Nation>");
		SCENINF.NErrors++;
		return false;
	};
	if(Form->Type!='FORM'){
		IntErr("CreateObject(DstObj,Formation,UnitType,Nation,Zone,Direction) : Invalid parameter <Formation>");
		SCENINF.NErrors++;
		return false;
	};
	if(UnitType->Type!='UTYP'){
		IntErr("CreateObject(DstObj,Formation,UnitType,Nation,Zone,Direction) : Invalid parameter <UnitType>");
		SCENINF.NErrors++;
		return false;
	};
	int xc,yc;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
	}else
	if(Zone->Type!='ZONE'){
		IntErr("CreateObject(DstObj,Formation,UnitType,Nation,Zone,Direction) : Invalid parameter <Zone>");
		SCENINF.NErrors++;
		return false;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
	};
	OrderDescription* ODS=ElementaryOrders+Form->Index;
	int N=ODS->NUnits;
	PORD.CreateSimpleOrdPos(xc,yc,Direction,ODS->NUnits,NULL,ODS);
	Nation* Nat=NATIONS+NatID;
	word NewIds[1024];
	int NU=0;
	for(int j=0;j<N;j++){
		int ID=CreateNewTerrMons2(NatID,PORD.px[j],PORD.py[j],UnitType->Index);
		if(ID!=-1&&NU<1024){
			NewIds[NU]=ID;
			NU++;

		};
	};
	UnitsGroup* UG=SCENINF.GetNewGroup();
	if(NU){
		UG->Allocate(NU);
		UG->N=NU;
		for(int j=0;j<NU;j++){
			UG->IDS[j]=NewIds[j];
			UG->SNS[j]=Group[NewIds[j]]->Serial;
		};
	}else{
		UG->Allocate(1);
		UG->N=0;
	};
	DstObj->Index=UG->Index;
	DstObj->Serial=0;
	DstObj->Type='UNIT';
	return true;
};
//---------------------Action commands----------------------
void ImClearSelection(byte Nat){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("ClearSelection(Nat) : Invalid parameter <Nat>");
		SCENINF.NErrors++;
		return;
	};
	word* SMon=ImSelm[Nat];
	word* SN=ImSerN[Nat];
	int N=ImNSL[Nat];
	if(N){
		for(int i=0;i<N;i++){
			word MID=SMon[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SN[i])OB->ImSelected&=~GM(Nat);
			};
		};
		free(SMon);
		free(SN);
		ImSelm[Nat]=NULL;
		ImSerN[Nat]=NULL;
		ImNSL[Nat]=NULL;
	};
	ImSelm[Nat]=NULL;
	ImSerN[Nat]=NULL;
	ImNSL[Nat]=NULL;
};
void ReClearSelection(byte Nat){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("ClearSelection(Nat) : Invalid parameter <Nat>");
		SCENINF.NErrors++;
		return;
	};
	word* SMon=Selm[Nat];
	word* SN=SerN[Nat];
	int N=NSL[Nat];
	if(N){
		for(int i=0;i<N;i++){
			word MID=SMon[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SN[i])OB->Selected&=~GM(Nat);
			};
		};
		free(SMon);
		free(SN);
		Selm[Nat]=NULL;
		SerN[Nat]=NULL;
		NSL[Nat]=NULL;
	};
	Selm[Nat]=NULL;
	SerN[Nat]=NULL;
	NSL[Nat]=NULL;
	addrand(NSL[Nat]+1);
};
void AddUnitToImSelected(byte NI,OneObject* OB){
	if(OB->ImSelected&GM(NI))return;
	OB->ImSelected|=GM(NI);
	int N=ImNSL[NI];
	ImSelm[NI]=(word*)realloc(ImSelm[NI],(N+1)<<1);
	ImSerN[NI]=(word*)realloc(ImSerN[NI],(N+1)<<1);
	ImNSL[NI]++;
	ImSelm[NI][N]=OB->Index;
	ImSerN[NI][N]=OB->Serial;
};
void AddUnitToSelected(byte NI,OneObject* OB){
	if(OB->Selected&GM(NI))return;
	OB->Selected|=GM(NI);
	int N=NSL[NI];
	Selm[NI]=(word*)realloc(Selm[NI],(N+1)<<1);
	SerN[NI]=(word*)realloc(SerN[NI],(N+1)<<1);
	NSL[NI]++;
	Selm[NI][N]=OB->Index;
	SerN[NI][N]=OB->Serial;
};
void ClearBMASK();

DLLEXPORT void ClearSelection(byte Nat){
	addrand(Nat);
	ReClearSelection(Nat);
	ImClearSelection(Nat);	
	//ClearBMASK();
};

void CopyReIm(byte NI){
	int Ni=ImNSL[NI]; 
	if(Ni){
		word* mid=ImSelm[NI];
		word* msn=ImSerN[NI];
		for(int i=0;i<Ni;i++){
			word MID=mid[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==msn[i]){
					OB->ImSelected&=~GM(NI);
				};
			};
		};
	};
	int N=NSL[NI];
	if(N){
		ImNSL[NI]=NSL[NI];
		N*=2;
		ImSelm[NI]=(word*)realloc(ImSelm[NI],N);
		ImSerN[NI]=(word*)realloc(ImSerN[NI],N);
		memcpy(ImSelm[NI],Selm[NI],N);
		memcpy(ImSerN[NI],SerN[NI],N);
	}else{
		if(ImSelm){
			free(ImSelm[NI]);
			free(ImSerN[NI]);
			ImSelm[NI]=NULL;
			ImSerN[NI]=NULL;
			ImNSL[NI]=0;
		};
	};
	Ni=ImNSL[NI];
	if(Ni){
		word* mid=ImSelm[NI];
		word* msn=ImSerN[NI];
		for(int i=0;i<Ni;i++){
			word MID=mid[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==msn[i]){
					OB->ImSelected|=1<<NI;
				};
			};
		};
	};
};
DLLEXPORT
void SelectUnits(GAMEOBJ* Units,bool Add){
	if(Units->Type!='UNIT'){
		IntErr("SelectUnits(Units,Add) : Invalid parameter <Units>");
		SCENINF.NErrors++;
		return;
	};
	TestUnitsGroup(Units->Index);
	int Nat=-1;
	UnitsGroup* UG=&SCENINF.UGRP[Units->Index];
	int NU=0;
	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==UG->SNS[i]){
				if(Nat==-1)Nat=OB->NNUM;
				if(OB->NNUM==Nat)NU++;
			};
		};
	};
	if(Nat!=-1){
		if(!Add)ClearSelection(Nat);
		if(NU){
			int N=NSL[Nat];
			Selm[Nat]=(word*)realloc(Selm[Nat],(N+NU)*2);
			SerN[Nat]=(word*)realloc(SerN[Nat],(N+NU)*2);

			word* SMon=Selm[Nat];
			word* SN=SerN[Nat];
			NSL[Nat]+=NU;

			NU=0;
			for(int i=0;i<UG->N;i++){
				word MID=UG->IDS[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==UG->SNS[i]){
						if(Nat==-1)Nat=OB->NNUM;
						if(OB->NNUM==Nat){
							Selm[Nat][N+NU]=OB->Index;
							SerN[Nat][N+NU]=OB->Serial;
							OB->Selected|=GM(Nat);
							NU++;
						};
					};
				};
			};
		};
		CopyReIm(Nat);
	};
};
DLLEXPORT
void SelectUnits1(byte Nat,GAMEOBJ* Units,bool Add){
	Nat=AssignTBL[Nat];
	if(Nat>7)return;
	if(Units->Type!='UNIT'){
		IntErr("SelectUnits1(Nat,Units,Add) : Invalid parameter <Units>");
		SCENINF.NErrors++;
		return;
	};
	TestUnitsGroup(Units->Index);
	UnitsGroup* UG=&SCENINF.UGRP[Units->Index];
	int NU=0;
	if(!Add)ClearSelection(Nat);
	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==UG->SNS[i]&&/*OB->NNUM==Nat&&*/!(OB->Sdoxlo||OB->Selected&GM(Nat)))NU++;
		};
	};
	if(NU){
		int N=NSL[Nat];
		Selm[Nat]=(word*)realloc(Selm[Nat],(N+NU)*2);
		SerN[Nat]=(word*)realloc(SerN[Nat],(N+NU)*2);
		word* SMon=Selm[Nat];
		word* SN=SerN[Nat];
		NSL[Nat]+=NU;

		NU=0;
		for(int i=0;i<UG->N;i++){
			word MID=UG->IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==UG->SNS[i]&&/*OB->NNUM==Nat&&*/!(OB->Sdoxlo||OB->Selected&GM(Nat))){
					Selm[Nat][N+NU]=OB->Index;
					SerN[Nat][N+NU]=OB->Serial;
					OB->Selected|=GM(Nat);
					NU++;
				};
			};
		};
	};
	CopyReIm(Nat);
};
DLLEXPORT
void SelectUnitsType(GAMEOBJ* UnitsType,byte Nat,bool Add){
	Nat=AssignTBL[Nat];
	if(UnitsType->Type!='UTYP'){
		IntErr("SelectUnitsType(UnitsType,Nation,Add) : Invalid parameter <UnitsType>");
		SCENINF.NErrors++;
		return;
	};
	if(Nat>=8){
		IntErr("SelectUnitsType(UnitsType,Nation,Add) : Invalid parameter <Nation>");
		SCENINF.NErrors++;
		return;
	};
	int NU=0;
	GeneralObject* GO=NATIONS[Nat].Mon[UnitsType->Index];
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)&&OB->Ref.General==GO&&OB->NNUM==Nat)NU++;
	};
	if(!Add)ClearSelection(Nat);
	if(NU){
		int N=NSL[Nat];
		Selm[Nat]=(word*)realloc(Selm[Nat],(N+NU)*2);
		SerN[Nat]=(word*)realloc(SerN[Nat],(N+NU)*2);

		word* SMon=Selm[Nat];
		word* SN=SerN[Nat];
		NSL[Nat]+=NU;

		NU=0;
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&(!OB->Sdoxlo)&&OB->Ref.General==GO&&OB->NNUM==Nat){
				Selm[Nat][N+NU]=OB->Index;
				SerN[Nat][N+NU]=OB->Serial;
				OB->Selected|=GM(Nat);
				NU++;
			};
		};
	};
	CopyReIm(Nat);
};

void DieSelected(byte NI);
void EraseSelected(byte NI);
DLLEXPORT
bool SelDie(byte Nat){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("SelDie(Nat) : Invalid parameter <Nat>");
		SCENINF.NErrors++;
		return false;
	};
	DieSelected(Nat);
	return true;
};
extern bool BreefUInfo;
DLLEXPORT
void DoMessagesBrief(){
	BreefUInfo=1;
};
DLLEXPORT
void SelErase(byte NI){
	if(NI>=8){
		IntErr("SelDie(Nat) : Invalid parameter <Nat>");
		SCENINF.NErrors++;
		return;
	};
	EraseSelected(NI);
};
DLLEXPORT
void SelAttackGroup(byte Nat,GAMEOBJ* Enemy){
	Nat=AssignTBL[Nat];
	if(Enemy->Type!='UNIT'){
		IntErr("SelAttackGroup : Invalid <Enemy>");
		SCENINF.NErrors++;
		return;
	};
	if(Nat>=8){
		IntErr("SelAttackGroup : Invalid <Nat>");
		SCENINF.NErrors++;
		return;
	};
	TestUnitsGroup(Enemy->Index);
	UnitsGroup* AG=&SCENINF.UGRP[Enemy->Index];
	int N=AG->N;
	word* Uids=AG->IDS;
	word* Sns=AG->SNS;
	word* MyUnits=Selm[Nat];
	word* MySN=SerN[Nat];
	int NU=NSL[Nat];
	if(!NSL)return;
	for(int i=0;i<N;i++){
		word MID=Uids[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==Sns[i]){
				for(int j=0;j<NU;j++){
					word MyMid=MyUnits[j];
					if(MyMid!=0xFFFF){
						OneObject* MyObj=Group[MyMid];
						if(MyObj&&MyObj->Serial==MySN[j]&&!(MyObj->Sdoxlo||MyObj->EnemyID!=0xFFFF)/*MyObj->LocalOrder*/){
							MyObj->AttackObj(MID,128+16,0);
						};
					};
				};
			};
		};
	};
};
DLLEXPORT 
bool SelOpenGates(byte Nat){
	return true;
};
DLLEXPORT
int GetNInside(byte Nat){
	Nat=AssignTBL[Nat];
	int Ni=0;
	int N=NSL[Nat];
	word* SM=Selm[Nat];
	word* SR=SerN[Nat];
	for(int i=0;i<N;i++){
		word MID=SM[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SR[i]&&!OB->Sdoxlo){
				Ni+=OB->NInside;
			};
		};
	};
	return Ni;
};
DLLEXPORT
int GetMaxInside(byte Nat){
	Nat=AssignTBL[Nat];
	int Ni=0;
	int N=NSL[Nat];
	word* SM=Selm[Nat];
	word* SR=SerN[Nat];
	for(int i=0;i<N;i++){
		word MID=SM[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SR[i]&&!OB->Sdoxlo){
				Ni+=OB->MoreCharacter->MaxInside+OB->AddInside;
			};
		};
	};
	return Ni;
};
DLLEXPORT
void PushUnitAway(byte Nat){
	Nat=AssignTBL[Nat];
	int N=NSL[Nat];
	word* SM=Selm[Nat];
	word* SR=SerN[Nat];
	for(int i=0;i<N;i++){
		word MID=SM[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SR[i]&&OB->NInside&&!(OB->Sdoxlo||OB->LocalOrder)){
				if(OB->Transport)OB->LeaveTransport(Group[OB->Inside[0]]->NIndex);
				else OB->LeaveMine(Group[OB->Inside[0]]->NIndex);
			};
		};
	};
};
void LeaveAll(OneObject* OB);
DLLEXPORT
void PushAllUnitsAway(byte Nat){
	Nat=AssignTBL[Nat];
	if(NSL[Nat]){
		int N=NSL[Nat];
		for(int i=0;i<N;i++){
			word MID=Selm[Nat][i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SerN[Nat][i]){
					if(OB->Transport)LeaveAll(OB);
				};
			};
		};
	};
};
DLLEXPORT
void SendUnitsToTransport(byte Nat){
	Nat=AssignTBL[Nat];
	word TransID=0xFFFF;
	if(NSL[Nat]){
		int N=NSL[Nat];
		for(int i=0;i<N;i++){
			word MID=Selm[Nat][i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SerN[Nat][i]&&!OB->Sdoxlo){
					if(OB->Transport)TransID=MID;
				};
			};
		};
		if(TransID!=0xFFFF){
			for(i=0;i<N;i++){
				word MID=Selm[Nat][i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SerN[Nat][i]&&!OB->Sdoxlo){
						if(!OB->Transport)OB->GoToTransport(TransID,128+16);
					};
				};
			};
		};
	};
};
DLLEXPORT
bool CheckLeaveAbility(byte Nat){
	Nat=AssignTBL[Nat];
	int N=NSL[Nat];
	word* SM=Selm[Nat];
	word* SR=SerN[Nat];
	for(int i=0;i<N;i++){
		word MID=SM[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SR[i]&&OB->Transport&&OB->DstX>0)return true;
		};
	};
	return false;
};
word SearchUnitInZone(int zx,int zy,int Rz,byte Nat){
	Nat=AssignTBL[Nat];
	int R=(Rz>>11)+2;
	int cx=zx>>11;
	int cy=zy>>11;

	int mxx=msx>>2;
	int myy=msy>>2;
	int NU=0;

	for(int r=0;r<R;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
			int Np=Rarr[r].N;
			for(int p=0;p<Np;p++){
				int xp=cx+xi[p];
				int yp=cy+yi[p];
				if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
					int cell=xp+1+((yp+1)<<VAL_SHFCX);
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->NNUM==Nat&&(!OB->Sdoxlo)&&
									Norma(OB->RealX-zx,OB->RealY-zy)<Rz)return MID;
							};
						};
					};
				};
			};
		};
		return 0xFFFF;
};
DLLEXPORT
void AttackZoneByArtillery(GAMEOBJ* ArtGroup,GAMEOBJ* Zone,byte Nat){
	Nat=AssignTBL[Nat];
	int xc,yc,R;
	if(ArtGroup->Type!='UNIT'){
		IntErr("AttackZoneByArtillery : Invalid <ArtGroup>");
		return;
	};
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=int(Zone->Index)<<4;
		yc=int(Zone->Serial)<<4;
		R=int(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type=='ZONE'){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	}else{
		IntErr("AttackZoneByArtillery : Invalid <Zone>");
		SCENINF.NErrors++;
		return;
	};
	word MID=SearchUnitInZone(xc,yc,R,Nat);
	if(MID!=0xFFFF){
		OneObject* Enm=Group[MID];
		int EnX=Enm->RealX>>4;
		int EnY=Enm->RealY>>4;
		int EnZ=GetHeight(EnX,EnY)+32;
		word* AMID=SCENINF.UGRP[ArtGroup->Index].IDS;
		word* AUSN=SCENINF.UGRP[ArtGroup->Index].SNS;
		int AUN=SCENINF.UGRP[ArtGroup->Index].N;
		for(int j=0;j<AUN;j++){
			word MMID=AMID[j];
			if(MMID!=0xFFFF){
				OneObject* OB=Group[MMID];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==AUSN[j]&&OB->newMons->Artilery){
					if(!OB->delay)OB->AttackPoint(EnX,EnY,EnZ,1,4,0,128+16);
				};
			};
		};
	};
};

word FindBuildingInZone(int x0,int y0,int R,int xc,int yc,byte Nat){
	Nat=AssignTBL[Nat];
	word* Uni=NatList[Nat];
	int N=NtNUnits[Nat];
	word UNIT=0xFFFF;
	int RMIN=1000000;
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Uni[i]];
		if(OB&&OB->NewBuilding&&!OB->Sdoxlo){
			byte Usage=OB->newMons->Usage;
			if(Usage!=FarmID&&Usage!=SkladID){
				int R0=Norma(OB->RealX-xc,OB->RealY-yc);
				if(R0<RMIN){
					RMIN=R0;
					UNIT=OB->Index;
				};
			};
		};
	};
	return UNIT;
};
word FindShipInZone(int x0,int y0,int R,byte Nat){
	Nat=AssignTBL[Nat];
	word* Uni=NatList[Nat];
	int N=NtNUnits[Nat];
	word UNIT=0xFFFF;
	int RMIN=1000000;
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Uni[i]];
		if(OB&&OB->LockType&&!OB->Sdoxlo){
			byte Usage=OB->newMons->Usage;
			if(Usage!=FisherID){
				int R0=Norma(OB->RealX-x0,OB->RealY-y0);
				if(R0<RMIN){
					RMIN=R0;
					UNIT=OB->Index;
				};
			};
		};
	};
	return UNIT;
};
OneObject* SearchUnitInCell(int cell,byte nmask){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(NMon<3)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NMask&nmask&&!OB->LockType)return OB;
		};
	};
    return NULL;
};
OneObject* SearchShipInCell(int cell,byte nmask){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(NMon<3)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NMask&nmask&&OB->LockType&&OB->newMons->Usage!=FisherID)return OB;
		};
	};
    return NULL;
};
word GetOneBld(int cell,int pos=0);
OneObject* SearchBuildingInCell(int cell,byte nmask){
    cell+=VAL_MAXCX+1;
	if(cell<0||cell>=VAL_MAXCIOFS)return NULL;
	word MID;
	for(int pos=0;(MID=GetOneBld(cell,pos))!=0xFFFF;pos++){
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NMask&nmask&&!OB->Sdoxlo)return OB;
		};
	}
	return NULL;
};
bool CheckVisibility(int x1,int y1,int x2,int y2,word MyID);
void SearchEnemyForAIArtillery(OneObject* OB);
void AIArtilleryAgainstTowers(OneObject* OB);
void SearchEnemyForMortira(OneObject* OB);
void EraseBrigade(Brigade* BR);
OneObject* TryToFindEnemy(int x,int y,int r0,int r1,byte mask){
    int cell=((y>>11)<<VAL_SHFCX)+(x>>11);
	int mindx=-(x>>11);
	int maxdx=(msx>>2)+mindx;
	int mindy=-(y>>11);
	int maxdy=(msx>>2)+mindy;
    int rx1=(r1>>7)+1;
    byte nmask=~mask;
    OneObject* DestObj=NULL;
	OneObject* DestShip=NULL;
	OneObject* DestBld=NULL;
    int mindist=10000000;
	int minSdist=10000000;
	int minBdist=10000000;
    int dist;
    //long range search
	int rx2=rx1+rx1+1;
	int stcell;
	r0<<=4;
	r1<<=4;
	for(int i=0;i<50;i++){
		int dx=((rando()*rx2)>>15)-rx1;
		int dy=((rando()*rx2)>>15)-rx1;
		int rr=Norma(dx,dy);
		if(dx<=maxdx&&dx>=mindx&&dy>=mindx&&dy<=maxdx){
			stcell=cell+dx+(dy<<VAL_SHFCX);
			if(stcell>=0&&stcell<VAL_MAXCIOFS){
				if(NPresence[stcell]&nmask){
					OneObject* OB=SearchShipInCell(stcell,nmask);
					if(OB){
						dist=Norma(OB->RealX-x,OB->RealY-y);
						if(dist>r0&&dist<r1&&dist<mindist){
							minSdist=dist;
							DestShip=OB;
						};
					}else{
						OB=SearchBuildingInCell(stcell,nmask);
						if(OB){
							dist=Norma(OB->RealX-x,OB->RealY-y);
							if(dist>r0&&dist<r1&&dist<mindist){
								minBdist=dist;
								DestBld=OB;
							};	
						}else if(rr<20){
							OB=SearchUnitInCell(stcell,nmask);
							if(OB){
								dist=Norma(OB->RealX-x,OB->RealY-y);
								if(dist>r0&&dist<r1&&dist<mindist){
									mindist=dist;
									DestObj=OB;
								};	
							};
						};
					};
				};
			};
		};
    };
	if(DestShip)return DestShip;
	if(DestBld)return DestBld;
	if(DestObj)return DestObj;
	return NULL;
};
DLLEXPORT
void AttackBuildingsInZone(GAMEOBJ* ArtGroup,GAMEOBJ* Zone,byte Nat){
	Nat=AssignTBL[Nat];
	int xc,yc,R;
	if(ArtGroup->Type!='UNIT'){
		IntErr("AttackZoneByArtillery : Invalid <ArtGroup>");
		return;
	};
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=int(Zone->Index)<<4;
		yc=int(Zone->Serial)<<4;
		R=int(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type=='ZONE'){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	}else{
		IntErr("AttackZoneByArtillery : Invalid <Zone>");
		SCENINF.NErrors++;
		return;
	};
	int XC=0;
	int YC=0;
	int NU0=0;
	word* AMID=SCENINF.UGRP[ArtGroup->Index].IDS;
	word* AUSN=SCENINF.UGRP[ArtGroup->Index].SNS;
	int AUN=SCENINF.UGRP[ArtGroup->Index].N;
	for(int j=0;j<AUN;j++){
		word MID=AMID[j];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==AUSN[j]){
				if(OB->EnemyID==0xFFFF){
					AdvCharacter* ADC=OB->MoreCharacter;
					OneObject* EOB=TryToFindEnemy(OB->RealX,OB->RealY,ADC->MinR_Attack,ADC->MaxR_Attack,OB->NMask);
					if(EOB)OB->AttackObj(EOB->Index,16+128,1,10);
				};
			};
		};
	};
};
void ProduceObject(byte NI,word Type);
DLLEXPORT
void ProduceOneUnit(byte Nat,GAMEOBJ* UnitType){
	Nat=AssignTBL[Nat];
	if(UnitType->Type!='UTYP'){
		IntErr("ProduceOneUnit : Invalid <UnitType>");
		return;
	};
	ProduceObject(Nat,UnitType->Index);
};
DLLEXPORT 
bool SelCloseGates(byte Nat){
	return true;
};
void SendSelectedToXY(byte NI,int xx,int yy,short Dir,byte Prio,byte Type);
DLLEXPORT 
bool SelSendTo(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("SelSendTo(Nat,Zone,Dir,Type) : Invalid parameter <Nat>");
		SCENINF.NErrors++;
		return false;
	};
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		SendSelectedToXY(Nat,int(Zone->Index)<<4,int(Zone->Serial)<<4,Dir,16,Type);
		return true;
	};
	if(Zone->Type!='ZONE'){
		IntErr("SelSendTo(Nat,Zone,Dir,Type) : Invalid parameter <Zone>");
		SCENINF.NErrors++;
		return false;
	};
	ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
	SendSelectedToXY(Nat,AZ->x<<4,AZ->y<<4,Dir,16,Type);
	return true;
};
DLLEXPORT 
bool SelSendAndKill(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("SelSendAndKill(Nat,Zone,Dir,Type) : Invalid parameter <Nat>");
		SCENINF.NErrors++;
		return false;
	};
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		SendSelectedToXY(Nat,int(Zone->Index)<<4,int(Zone->Serial)<<4,Dir,128,Type);
		return true;
	};
	if(Zone->Type!='ZONE'){
		IntErr("SelSendAndKill(Nat,Zone,Dir,Type) : Invalid parameter <Zone>");
		SCENINF.NErrors++;
		return false;
	};
	ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
	SendSelectedToXY(Nat,AZ->x<<4,AZ->y<<4,Dir,128,Type);
	return true;
};
void PatrolGroup(byte NI,int x1,int y1,byte Dir);
DLLEXPORT 
bool Patrol(byte Nat,GAMEOBJ* Zone,byte Dir){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("Patrol(Nat,Zone,Dir) : Invalid parameter <Nat>");
		SCENINF.NErrors++;
		return false;
	};
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		PatrolGroup(Nat,int(Zone->Index)<<4,int(Zone->Serial)<<4,Dir);
		return true;
	};
	if(Zone->Type!='ZONE'){
		IntErr("Patrol(Nat,Zone,Dir) : Invalid parameter <Zone>");
		SCENINF.NErrors++;
		return false;
	};
	ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
	PatrolGroup(Nat,AZ->x<<4,AZ->y<<4,Dir);
	return true;
};
DLLEXPORT
void ChangeNatMask(byte Nat,byte Flags){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("ChangeFriends(Nat,Flags) : Invalid parameter <Nat>");
		SCENINF.NErrors++;
		return;
	};
	NATIONS[Nat].NMask=Flags;
}
DLLEXPORT
void ChangeFriends(byte Nat,byte Flags){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("ChangeFriends(Nat,Flags) : Invalid parameter <Nat>");
		SCENINF.NErrors++;
		return;
	};
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NNUM==Nat){
			OB->NMask=Flags;
			if(OB->ActiveAbility)
				OB->ActiveAbility->BornNMask=OB->NMask;
		};
	};
	NATIONS[Nat].NMask=Flags;
};
extern char PlName[64];
DLLEXPORT void SetPlayerNation(int NI){
	if (0<=NI&&NI<8) {        
		if(NI!=MyNation){
			CurrentGameInfo* g=&GSets.CGame;
			strcpy(g->PL_INFO[NI].name,PlName);
			static char* cn=GetTextByID("#CampComputerName");
			strcpy(g->PL_INFO[MyNation].name,cn);
		}
		SetMyNation(NI);
		return;
	}
	SetMyNation(0);
}
DLLEXPORT
int		GetPlayerNation(){
	return	MyNation;
}
DLLEXPORT
void	RunTactikalAI(byte NI){
	if (0<=NI&&NI<8){
		NATIONS[NI].AI_Enabled = true;
	}
}
CEXPORT bool GetAIEnable(byte NI){
	if(0<=NI&&NI<8){
		return NATIONS[NI].AI_Enabled;
	}
	return false;
}
extern City CITY[8];
DLLEXPORT void ChangeUnitColor(int Index,byte DstNI);

void ChangeBrigNation(word SBID, byte SrcNat, byte DstNat){
	if(SrcNat==DstNat||SrcNat>7||DstNat>7) return;

	City* SC=CITY+SrcNat;
	City* DC=CITY+DstNat;

	if(SBID>MaxBrig) return;
	Brigade* SBR=SC->Brigs+SBID;
	if(!SBR->Enabled) return;

	int BID=DC->GetFreeBrigade();
	if(BID!=-1){
		Brigade* DBR=DC->Brigs+BID;
		//DBR->Enabled=1;
		word BSN=DBR->SN;
		if(SBR->Ability)
		{
			SBR->Ability->Clear();
			delete SBR->Ability;
			SBR->Ability=NULL;
		}
		memcpy(DBR,SBR,sizeof(Brigade));
		DBR->ID=BID;
		DBR->SN=BSN;
		DBR->CT=DC;

		int NM=DBR->NMemb;
		word* ID=DBR->Memb;
		word* SN=DBR->MembSN;
		for(int i=0;i<NM;i++){
			word mid=ID[i];
			if(mid!=0xFFFF){
				OneObject* OB=Group[mid];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==SN[i]){
					//ChangeUnitColor(mid,DstNat);
					DelObject(OB);
					//OB->Nat->CITY->UnRegisterNewUnit(OB);
					OB->Ref.General=NATIONS[DstNat].Mon[OB->NIndex];
					OB->Nat=&NATIONS[DstNat];
					OB->NMask=OB->Nat->NMask;
					if(OB->ActiveAbility)
						OB->ActiveAbility->BornNMask=OB->NMask;
					OB->NNUM=DstNat;
					OB->Nat->CITY->RegisterNewUnit(OB);					
					AddObject(OB);

					
					SN[i]=OB->Serial;
					OB->BrigadeID=BID;
				};
			};
		};

		SBR->Init(SC,SBID);

#ifdef COSSACKS2
		void NewBrigadeIsBorn(Brigade* B);
		NewBrigadeIsBorn(DBR);
#endif COSSACKS2
		
	};

}

DLLEXPORT void OBJ_ChangeNation(OneObject* OB, byte DstNat);

DLLEXPORT void SelChangeNation(byte SrcNat,byte DstNat){
	if(SrcNat>=8||DstNat>=8||DstNat>7){
		IntErr("SelChangeNation(SrcNat,DstNat) : Invalid parameters");
		SCENINF.NErrors++;
		return;
	};
	if(SrcNat==DstNat){
		IntErr("SelChangeNation(SrcNat,DstNat) : SrcNat and DstNat must be different");
		SCENINF.NErrors++;
		return;
	};

	int NBr=0;
	word BrID[256];

	int N=NSL[SrcNat];
	word* SMon=Selm[SrcNat];
	word* SN=SerN[SrcNat];
	Nation* DNat=NATIONS+DstNat;
	for(int i=0;i<N;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int bid=OB->BrigadeID;
				if(!OB->Sdoxlo&&OB->Serial==SN[i]&&bid!=0xFFFF&&CITY[OB->NNUM].Brigs[bid].Enabled&&NBr<256){
					bool find=false;
					for(int b=0;b<NBr;b++) if(BrID[b]==bid){
						find=true;
					}
					if(!find){
						BrID[NBr]=bid;
						NBr++;
					}
				}else{
					ChangeUnitColor(MID,DstNat);
				}
			};
		};
	};
	ClearSelection(SrcNat);
	for(i=0;i<NBr;i++){
		ChangeBrigNation(BrID[i],SrcNat,DstNat);
	}
};
DLLEXPORT
void SelAutoKill(byte Nat){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("SelAutoKill(Nat) : Invalid parameters");
		SCENINF.NErrors++;
		return;
	};
	int N=NSL[Nat];
	word* SMon=Selm[Nat];
	word* SN=SerN[Nat];
	for(int i=0;i<N;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				OB->AutoKill=true;
			};
		};
	};
};
void MakeStandGround(byte NI);
void CancelStandGround(byte NI);
void SetSearchVictim(byte NI,byte Val);
DLLEXPORT
void SetStandGround(byte Nat,byte val){
	Nat=AssignTBL[Nat];
	if(Nat>7){
		IntErr("SetStandGround(Nat,val) : Invalid <Nat>");
		SCENINF.NErrors++;
		return;
	};
	if(val)MakeStandGround(Nat);
	else CancelStandGround(Nat);
};
DLLEXPORT
void AllowAttack(byte Nat,byte val){
	Nat=AssignTBL[Nat];
	if(Nat>7){
		IntErr("AllowAttack(Nat,val) : Invalid <Nat>");
		SCENINF.NErrors++;
		return;
	};
	if(val)SetSearchVictim(Nat,2);
	else SetSearchVictim(Nat,3);
};
//-----------------------INFORMATION COMMANDS----------------------------//
DLLEXPORT void HINT(GAMEOBJ* Hint,int time){
	if(Hint->Type!='STRI'){
		IntErr("HINT(Hint,Time) : invalid parameter <Hint>");
		SCENINF.NErrors++;
		return;
	};
	char* cc=SCENINF.Messages[Hint->Index];
	if(cc)AssignHint1(cc,time);
};
DLLEXPORT void DisableMission(char MISSID){
	//if(MISSID<'A'||MISSID>'Z'){
	//	IntErr("DisableMission(ID) : Invalid <ID>");
	//	SCENINF.NErrors++;
	//	return;
	//};
	SCENINF.TextDisable[GetCPos(MISSID)]=1;
	CreateMissText();
};
DLLEXPORT void EnableMission(char MISSID){
	//if(MISSID<'A'||MISSID>'Z'){
	//	IntErr("EnableMission(ID) : Invalid <ID>");
	//	SCENINF.NErrors++;
	//	return;
	//};
	SCENINF.TextDisable[GetCPos(MISSID)]=0;
	CreateMissText();
};

DLLEXPORT void SetVictoryText(char* ID){
	char* txt=GetTextByID(ID);
	if(strcmp(txt,ID)){
		SCENINF.VictoryText=ID;
	}else{
		IntErr("SetVictoryText(ID) : Unknown ID : %s  (see COMMENT.TXT)",ID);
		SCENINF.NErrors++;
	};
};

DLLEXPORT void SetLooseText(char* ID){
	char* txt=GetTextByID(ID);
	if(strcmp(txt,ID)){
		SCENINF.LooseText=ID;
	}else{
		IntErr("SetLoosText(ID) : Unknown ID : %s  (see COMMENT.TXT)",ID);
		SCENINF.NErrors++;
	};
};
extern bool ShowStat;
void CmdEndGame(byte NI,byte state,byte cause);
extern int HISPEED;
extern bool ShowPostScreen;
extern int CurrentSinBattle;
extern void SetBattleResult();
extern bool vmVictory;
void rcm_InitStatistic();
void vmPlayerDataWrite();

DLLEXPORT void ShowVictory(char* Message, byte NI=0xFF){
	if(NI==0xFF||NI==MyNation){
		vHeroButtons.SetVisible(false);
		cva_VI_EndGame::UserMessage=Message;
		vmVictory=true;
		SCENINF.Victory=true;
		if(CurrentSinBattle!=-1) SetBattleResult();
		HISPEED=0;
		NOPAUSE=0;
		LockPause=1;
		ShowStat=0;
		//if(SCENINF.StandartVictory)CmdEndGame(MyNation,2,100);		
		if(TPEN.Connected){
			//peerMessagePlayer(TPEN.Peer,"spybot","\\launched\\0\\gamename\\cossacks\\");
			TPEN.Disconnect();
		};
		if(CurrentMission!=-1)ShowPostScreen=1;	
		rcm_InitStatistic();
		//
		vHeroButtons.SetVisible(false);
		//
		CmdEndGame(MyNation,2,100);

		vmPlayerDataWrite();

	}else{
		CmdEndGame(NI,2,100);
	}
};
/*
DLLEXPORT void ShowVictory(char* Message){
	ShowVictory(Message,0xFF);
}
DLLEXPORT void ShowVictory(){
	ShowVictory(NULL);
}
*/
DLLEXPORT void LooseGame(char* Message, byte NI=0xFF){
	if(NI==0xFF||NI==MyNation){
		vHeroButtons.SetVisible(false);
		cva_VI_EndGame::UserMessage=Message;
		SCENINF.LooseGame=true;
		//BattleResult=2;
		HISPEED=0;
		NOPAUSE=0;
		LockPause=1;
		ShowStat=0;
		if(SCENINF.hLib)SCENINF.StandartVictory=0;
		if(TPEN.Connected){
			//peerMessagePlayer(TPEN.Peer,"spybot","\\launched\\0\\gamename\\cossacks\\");
			TPEN.Disconnect();
		};		
		//
		vHeroButtons.SetVisible(false);
		//
		CmdEndGame(MyNation,1,100);	
	}else{
		CmdEndGame(NI,1,100);	
	}
};
/*
DLLEXPORT void LooseGame(char* Message){
	LooseGame(Message,0xFF);
}
DLLEXPORT void LooseGame(){
	LooseGame(NULL);
}
*/
DLLEXPORT void ShowCentralText(char* ID,int time){
	SCENINF.CentralText=GetTextByID(ID);
	SCENINF.CTextTime=time;
};
void ProcessMissionText(char* Bmp,char* Text);
void AddHistory(Nation* NAT,char* Name){
	NAT->History=(char**)realloc(NAT->History,4*NAT->NHistory+4);
	NAT->History[NAT->NHistory]=znew(char,strlen(Name)+1);
	strcpy(NAT->History[NAT->NHistory],Name);
	NAT->NHistory++;
};
DLLEXPORT void ShowPage(char* Name){
	int NP=0;
	AddHistory(NATIONS+MyNation,Name);
	for(int i=0;i<SCENINF.NPages;i++){
		if(!strcmp(Name,SCENINF.PageID[i])){
			ProcessMissionText(SCENINF.PageBMP[i],SCENINF.Page[i]);
			NP++;
		};
	};
	if(!NP){
		IntErr("ShowPage : Unknown page: %s",Name);
		SCENINF.NErrors++;
	};
};
CEXPORT void ShowMessageII(int FileID,int SpriteID,char* MessageID);
DLLEXPORT void ShowPageParam(char* Name,...){
	va_list va;
    va_start( va, Name );
	char cc2[4096];
	char cc3[4096];
	int NP=0;
	for(int i=0;i<SCENINF.NPages;i++){
		if(!strcmp(Name,SCENINF.PageID[i])){
			vsprintf(cc2,SCENINF.Page[i],va);
			//sprintf(cc3,"%s @ %s",SCENINF.PageBMP[i],cc2);
			//AddHistory(NATIONS+MyNation,cc3);
			//ProcessMissionText(SCENINF.PageBMP[i],cc2);
			char file[64];
			int sprite=0;
			if(sscanf(SCENINF.PageBMP[i],"%s%d",file,&sprite)==2){
				ShowMessageII(GPS.PreLoadGPImage(file),sprite,cc2);
			}
			NP++;
			break;
		};
	};
	va_end( va );
	if(!NP){
		IntErr("ShowPage : Unknown page: %s",Name);
		SCENINF.NErrors++;
	};
};
//
extern char* v_MessBmp;
extern _str v_mess;
extern char* v_MessText;
//
CEXPORT void ShowMessageII(int FileID,int SpriteID,char* MessageID){
	static _str bmp;
	bmp.Clear();
	bmp.print("%s %d",GPS.GetGPName(FileID),SpriteID);	
	v_MessBmp=bmp.pchar();	
	v_mess=MessageID;
	v_MessText=v_mess.pchar();
	v_MainMenu.GameMode=false;
	PlayEffect(GetSound("INTERF_MESSAGE"),0,0);
	if(v_MainMenu.StartDS("Message",true)){
		//v_MainMenu.GameMode=true;
		return;
	}
};
extern byte PlayGameMode;
bool AskMissionQuestion(char* Bmp,char* Text);
DLLEXPORT bool AskQuestion(char* Name){
	if(PlayGameMode)RGAME.Extract();
	int NP=0;
	AddHistory(NATIONS+MyNation,Name);
	for(int i=0;i<SCENINF.NPages;i++){
		if(!strcmp(Name,SCENINF.PageID[i])){
			int t=AskMissionQuestion(SCENINF.PageBMP[i],SCENINF.Page[i]);
			if(RecordMode){
				RGAME.AddByte(3);
				RGAME.AddByte(t);
			};
			return !!t;
		};
	};
	IntErr("ShowPage : Unknown page: %s",Name);
	SCENINF.NErrors++;
	return 0;
};
DLLEXPORT bool AskQuestionParam(char* Name,...){
	va_list va;
    va_start( va, Name );
	char cc2[4096];
	char cc3[4096];
	int NP=0;
	if(PlayGameMode)RGAME.Extract();

	for(int i=0;i<SCENINF.NPages;i++){
		if(!strcmp(Name,SCENINF.PageID[i])){
			vsprintf(cc2,SCENINF.Page[i],va);
			sprintf(cc3,"%s @ %s",SCENINF.PageBMP[i],cc2);
			AddHistory(NATIONS+MyNation,cc3);
			int t=AskMissionQuestion(SCENINF.PageBMP[i],cc2);
			if(RecordMode){
				RGAME.AddByte(3);
				RGAME.AddByte(t);
			};
			return !!t;
		};
	};
	IntErr("ShowPage : Unknown page: %s",Name);
	SCENINF.NErrors++;
	return 0;
};
int ProcessComplexQuestion(int Nx,char* Bmp1,byte or1,char* Text1,char* Bmp2,byte or2,char* Text2,char* Quest);
int _pr_q1=-1;
int _pr_q2=-1;
int _pr_Nx=-1;
byte _pr_or1;
byte _pr_or2;
DLLEXPORT int AskComplexQuestion(int Nx,char* Name1,byte or1,char* Name2,byte or2,char* Quest){
	if(PlayGameMode)RGAME.Extract();
	int q1=-1;
	int q2=-1;
	for(int i=0;i<SCENINF.NPages;i++){
		if(!strcmp(Name1,SCENINF.PageID[i])){
			q1=i;
			goto qq1;
		};
	};
	IntErr("AskComplexQuestion : Unknown page : %s",Name1);
qq1:
	for(i=0;i<SCENINF.NPages;i++){
		if(!strcmp(Name2,SCENINF.PageID[i])){
			q2=i;
			goto qq2;
		};
	};
	IntErr("AskComplexQuestion : Unknown page : %s",Name2);
qq2:
	_pr_q1=q1;
	_pr_q2=q2;
	_pr_Nx=Nx;
	_pr_or1=or1;
	_pr_or2=or2;
	int t=ProcessComplexQuestion(Nx,SCENINF.PageBMP[q1],or1,SCENINF.Page[q1],SCENINF.PageBMP[q2],or2,SCENINF.Page[q2],Quest);
	if(RecordMode){
		RGAME.AddByte(3);
		RGAME.AddByte(t);
	};
	return t;
};
void RetryCQuest(){
	if(_pr_Nx!=-1){
		ProcessComplexQuestion(_pr_Nx,SCENINF.PageBMP[_pr_q1],_pr_or1,SCENINF.Page[_pr_q1],SCENINF.PageBMP[_pr_q2],_pr_or2,SCENINF.Page[_pr_q2],GetTextByID("TUTCLOS"));
	};
};
int ShowHistryItem(char* Bmp,char* Text,bool Shadow);
void ShowHistory(){
	int res=0;
	Nation* Nat=NATIONS+MyNation;
	int Cur=Nat->NHistory-1;
	if(Cur<0)return;
	bool sh=1;
	do{
		char* Name=Nat->History[Cur];
		if(Name[0]=='#'){
			int cp=-1;
			for(int i=0;i<SCENINF.NPages;i++){
				if(!strcmp(Name,SCENINF.PageID[i])){
					cp=i;
				};
			};
			if(cp==-1)return;
			res=ShowHistryItem(SCENINF.PageBMP[cp],SCENINF.Page[cp],sh);
		}else{
			char CC3[4096];
			strcpy(CC3,Name);
			char* cc=strstr(CC3,"@");
			if(cc){
				cc[-1]=0;
				res=ShowHistryItem(CC3,cc+1,sh);
			}else return;
		};
		if(res==1&&Cur>0)Cur--;
		if(res==2&&Cur<Nat->NHistory-1)Cur++;
		sh=0;
	}while(res!=3);
};

//-----------------------------Resource functions------------------------//
DLLEXPORT
int GetResource(byte Nat,byte ID){
	Nat=AssignTBL[Nat];
	if(Nat<8&&ID<7){
		return XRESRC(Nat,ID);
	}else{
		IntErr("GetResource : Invalid parameters");
		SCENINF.NErrors++;
	};
	return 0;
};
DLLEXPORT void AddResource(byte Nat,byte ID,int Amount){
	Nat=AssignTBL[Nat];

	addrand(Nat);
	addrand(ID);
	addrand(Amount);

	if(Nat<8&&ID<7){
		AddXRESRC(Nat,ID,Amount);
		if(XRESRC(Nat,ID)<0)SetXRESRC(Nat,ID,0);
	}else{
		IntErr("AddResource : Invalid parameters");
		SCENINF.NErrors++;
	};

};
DLLEXPORT
void SetResource(byte Nat,byte ID,int Amount){
	Nat=AssignTBL[Nat];
	if(Nat<8&&ID<7){
		SetXRESRC(Nat,ID,Amount);
		if(XRESRC(Nat,ID)<0)SetXRESRC(Nat,ID,0);
	}else{
		IntErr("SetResource : Invalid parameters");
		SCENINF.NErrors++;
	};

};
void GetUnitCost(byte NI,word NIndex,int* Cost,int x,int y);
DLLEXPORT
int GetUnitCost(byte Nat,GAMEOBJ* UnitType,byte ResID){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("GetUnitCost(Nat,UnitType,ResID) : Invalid <Nat>");
		SCENINF.NErrors++;
		return 0;
	};
	if(UnitType->Type!='UTYP'){
		IntErr("GetUnitCost(Nat,UnitType,ResID) : Invalid <UnitType>");
		SCENINF.NErrors++;
		return 0;
	};
	if(ResID>=6){
		IntErr("GetUnitCost(Nat,UnitType,ResID) : Invalid <ResID>");
		SCENINF.NErrors++;
		return 0;
	};
	int Cost[8];
	GetUnitCost(Nat,UnitType->Index,Cost,-1,-1);
	return Cost[ResID];
};
DLLEXPORT
int GetUpgradeCost(byte Nat,GAMEOBJ* Upgrade,byte ResID){
	Nat=AssignTBL[Nat];
	if(Nat>=8){
		IntErr("GetUpgradeCost(Nat,Upgrade,ResID) : Invalid <Nat>");
		SCENINF.NErrors++;
		return 0;
	};
	if(Upgrade->Type!='UPGR'){
		IntErr("GetUpgradeCost(Nat,Upgrade,ResID) : Invalid <Upgrade>");
		SCENINF.NErrors++;
		return 0;
	};
	if(ResID>=6){
		IntErr("GetUpgradeCost(Nat,Upgrade,ResID) : Invalid <ResID>");
		SCENINF.NErrors++;
		return 0;
	};
	return NATIONS[Nat].UPGRADE[Upgrade->Index]->Cost[ResID];
};
//----------------------------TRIGGER FUNCTION---------------------------//
DLLEXPORT
byte Trigg(byte ID){
	if(ID>511){
		IntErr("Trigg(ID) : Invalid ID");
		SCENINF.NErrors++;
		return 0;
	};
	return ~SCENINF.TRIGGER[ID];
};
DLLEXPORT
void SetTrigg(byte ID,byte Val){
	if(ID>511){
		IntErr("SetTrigg(ID) : Invalid ID");
		SCENINF.NErrors++;
		return;
	};
	SCENINF.TRIGGER[ID]=~Val;
};
DLLEXPORT
word WTrigg(byte ID){
	if(ID>511){
		IntErr("Trigg(ID) : Invalid ID");
		SCENINF.NErrors++;
		return 0;
	};
	return ~SCENINF.TRIGGER[ID];
};
DLLEXPORT
void SetWTrigg(byte ID,word Val){
	if(ID>511){
		IntErr("SetTrigg(ID) : Invalid ID");
		SCENINF.NErrors++;
		return;
	};
	SCENINF.TRIGGER[ID]=~Val;
};
//------------------------------AI Functions-----------------------------//
void LoadAIFromDLL(byte Nat,char* Name);
DLLEXPORT
void RunAI(byte Nat){
	Nat=AssignTBL[Nat];
	if(Nat<1||Nat>7){
		IntErr("RunAI(Nat) :  Invalid <Nat>");
		SCENINF.NErrors++;
		return;
	};
	Nation* NT=&NATIONS[Nat];
	if(!NT->AI_Enabled){
		NT->AI_Enabled=true;
		//NT->NMask=0x7E;
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->NNUM==Nat&&OB->newMons->Usage==PeasantID){
				for(int j=0;j<GlobalAI.NAi;j++){
					if(!strcmp(GlobalAI.Ai[j].Peasant_ID,OB->Ref.General->MonsterID)){
						LoadAIFromDLL(Nat,GlobalAI.Ai[j].LandAI[0]);
						goto RegUni;
					};
				};
			};
		};
RegUni:;
		for(i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->NNUM==Nat&&OB->BrigadeID==0xFFFF){
				NT->CITY->RegisterNewUnit(OB);
				//OB->NMask=0x7E;
			};
		};
	};
};
DLLEXPORT 
void RunAIWithPeasants(byte Nat,char* P_Name){
	Nat=AssignTBL[Nat];
	if(Nat<1||Nat>7){
		IntErr("RunAI(Nat) :  Invalid <Nat>");
		SCENINF.NErrors++;
		return;
	};
	Nation* NT=&NATIONS[Nat];
	if(!NT->AI_Enabled){
		NT->AI_Enabled=true;
		//NT->NMask=0x7E;
		for(int j=0;j<GlobalAI.NAi;j++){
			if(!strcmp(GlobalAI.Ai[j].Peasant_ID,P_Name)){
				LoadAIFromDLL(Nat,GlobalAI.Ai[j].LandAI[0]);
			};
		};
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->NNUM==Nat&&OB->BrigadeID==0xFFFF){
				NT->CITY->RegisterNewUnit(OB);
				//OB->NMask=0x7E;
			};
		};
	};
};
//-----------------------------Timer commands----------------------------//
void RunTimer(byte ID, int Long, bool trueTime){
	if(ID>=32){
		IntErr("RunTimer : Invalid timer index");
		SCENINF.NErrors++;
		return;
	};
	GTimer* TM=SCENINF.TIME+ID;
	TM->Time=Long;
	TM->Used=true;
	TM->First=false;
	TM->trueTime=trueTime;
	if(trueTime){
		TM->LastCheckAnimTime=TrueTime;
	}else{
		TM->LastCheckAnimTime=AnimTime;
	}
}
DLLEXPORT void RunTimer(byte ID,int Long){
	RunTimer(ID,Long,true);
};
DLLEXPORT
bool TimerDone(byte ID){
	if(ID>=32){
		IntErr("TimerDone : Invalid timer index");
		SCENINF.NErrors++;
		return false;
	};
	GTimer* TM=SCENINF.TIME+ID;
	return TM->Time==0&&TM->Used; 
};
DLLEXPORT
bool TimerDoneFirst(byte ID){
	if(ID>=32){
		IntErr("TimerDoneFirst : Invalid timer index");
		SCENINF.NErrors++;
		return false;
	};
	GTimer* TM=SCENINF.TIME+ID;
	if(TM->Time==0&&TM->Used&&TM->First){
		TM->First=0;
		return true;
	}else return false;

};
DLLEXPORT
bool TimerIsEmpty(byte ID){
	if(ID>=32){
		IntErr("TimerIsEmpty : Invalid timer index");
		SCENINF.NErrors++;
		return false;
	};
	GTimer* TM=SCENINF.TIME+ID;
	return !TM->Used;
};
DLLEXPORT
void FreeTimer(byte ID){
	if(ID>=32){
		IntErr("FreeTimer : Invalid timer index");
		SCENINF.NErrors++;
		return;
	};
	GTimer* TM=SCENINF.TIME+ID;
	TM->First=0;
	TM->Time=0;
	TM->Used=0;
};
DLLEXPORT
int GetTime(byte ID){
	if(ID>=32){
		IntErr("GeRunTimer : Invalid timer index");
		SCENINF.NErrors++;
		return 0;
	};
	GTimer* TM=SCENINF.TIME+ID;
	return TM->Time;
};

//extern int tmtmt;
extern int TrueTime;
extern int AnimTime;

DLLEXPORT int GetGlobalTime(){
	//int time=AnimTime*5/32;
	//return time;
	return TrueTime;
};
DLLEXPORT int GetAnimTime(){
	return AnimTime;
}
//---------------------------Dynamical zones-----------------------------//
DLLEXPORT
bool UnitsCenter(GAMEOBJ* DstZone,GAMEOBJ* Units,word R){
	if(Units->Type!='UNIT'){
		IntErr("UnitsCenter(DstZone,Units,R) : invalid <Units>");
		SCENINF.NErrors++;
		return false;
	};
	TestUnitsGroup(Units->Index);
	int xs=0;
	int ys=0;
	int N=0;
	UnitsGroup* UG=&SCENINF.UGRP[Units->Index];
	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
				xs+=OB->RealX;
				ys+=OB->RealY;
				N++;
			};
		};
	};
	if(N){
		xs=(xs>>4)/N;
		ys=(ys>>4)/N;
		DstZone->Index=xs;
		DstZone->Serial=ys;
		DstZone->Type='@   '-0x202020+R;
	}else{
		DstZone->Type=-1;
		return false;
	};
	return true;
};
void ProcessScreen();
void GSYSDRAW();
DLLEXPORT
void RefreshScreen(){
	ProcessScreen();
	GSYSDRAW();
};

DLLEXPORT
bool SelCenter(GAMEOBJ* DstZone,byte Nat,int R){
	Nat=AssignTBL[Nat];
	if(Nat>7){
		IntErr("SelCenter(DstZone,byte Nat,R) : invalid <Nat>");
		SCENINF.NErrors++;
		return false;
	};
	int xs=0;
	int ys=0;
	int N=0;
	word* SNS=SerN[Nat];
	word* IDS=Selm[Nat];
	int NU=NSL[Nat];
	for(int i=0;i<NU;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==SNS[i]){
				xs+=OB->RealX;
				ys+=OB->RealY;
				N++;
			};
		};
	};
	if(N){
		xs=(xs>>4)/N;
		ys=(ys>>4)/N;
		DstZone->Index=xs;
		DstZone->Serial=ys;
		DstZone->Type='@   '-0x202020+R;
	}else{
		DstZone->Type=-1;
		return false;
	};
	return true;
};
DLLEXPORT
bool CreateZoneNearUnit(GAMEOBJ* DstZone,GAMEOBJ* Zone,GAMEOBJ* UnitType,byte Nat,int R){
	Nat=AssignTBL[Nat];
	if(UnitType->Type!='UTYP'){
		IntErr("CreateZoneNearUnit : invalid <UnitType>");
		SCENINF.NErrors++;
		DstZone->Type=0xFFFFFFFF;
		return false;
	};
	GeneralObject* GO=NATIONS[Nat].Mon[UnitType->Index];
	word* Mons=NatList[Nat];
	int N=NtNUnits[Nat];
	if(Zone){
		int x0,y0;
		if((Zone->Type&0xFF000000)==('@   '-0x202020)){
			x0=int(Zone->Index)<<4;
			y0=int(Zone->Serial)<<4;
		}else
		if(Zone->Type=='ZONE'){
			ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
			x0=AZ->x<<4;
			y0=AZ->y<<4;
		}else{
			IntErr("CreateZoneNearUnits : Invalid <Zone>");
			SCENINF.NErrors++;
			return false;
		};
		int Rmin=10000000;
		int xc=-1;
		int yc=-1;
		for(int i=0;i<N;i++){
			OneObject* OB=Group[Mons[i]];
			if(OB&&OB->Ref.General==GO&&!(OB->GetLiveState()!=ls_LiveBody)){
				int R=Norma(OB->RealX-x0,OB->RealY-y0);
				if(R<Rmin){
					xc=OB->RealX;
					yc=OB->RealY;
					Rmin=R;
				};
			};
		};	
		if(xc>-1){
			DstZone->Index=xc>>4;
			DstZone->Serial=yc>>4;
			DstZone->Type='@   '-0x202020+R;
			return true;
		}else{
			DstZone->Type=0xFFFFFFFF;
			return false;
		};
	}
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Mons[i]];
		if(OB&&OB->Ref.General==GO){
			DstZone->Index=OB->RealX>>4;
			DstZone->Serial=OB->RealY>>4;
			DstZone->Type='@   '-0x202020+R;
			return true;
		};
	};
	DstZone->Type=0xFFFFFFFF;
	return false;
};
DLLEXPORT
bool CreateZoneNearGroup(GAMEOBJ* DstZone,GAMEOBJ* Zone,GAMEOBJ* Grp,int R){
	if(Grp->Type!='UNIT'){
		IntErr("CreateZoneNearGroup : invalid <Group>");
		SCENINF.NErrors++;
		DstZone->Type=0xFFFFFFFF;
		return false;
	};
	if(Zone){
		int x0,y0;
		if((Zone->Type&0xFF000000)==('@   '-0x202020)){
			x0=int(Zone->Index)<<4;
			y0=int(Zone->Serial)<<4;
		}else
		if(Zone->Type=='ZONE'){
			ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
			x0=AZ->x<<4;
			y0=AZ->y<<4;
		}else{
			IntErr("CreateZoneNearUnits : Invalid <Zone>");
			SCENINF.NErrors++;
			return false;
		};
		int Rmin=10000000;
		int xc=-1;
		int yc=-1;
		UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];
		for(int i=0;i<UG->N;i++){
			word MID=UG->IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
					int R=Norma(OB->RealX-x0,OB->RealY-y0);
					if(R<Rmin){
						xc=OB->RealX;
						yc=OB->RealY;
						Rmin=R;
					};
				};
			};
		};
		if(xc>-1){
			DstZone->Index=xc>>4;
			DstZone->Serial=yc>>4;
			DstZone->Type='@   '-0x202020+R;
			return true;
		}else{
			DstZone->Type=0xFFFFFFFF;
			return false;
		};
	};
	return false;
};
DLLEXPORT void SetLightSpot(GAMEOBJ* Zone,int R,byte index){
	int x0,y0;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		x0=int(Zone->Index);
		y0=int(Zone->Serial);
	}else
	if(Zone->Type=='ZONE'){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		x0=AZ->x;
		y0=AZ->y;
	}else
	if(Zone->Type!='ZONE'){
		IntErr("SetLightSpot : Invalid  <Zone>");
		SCENINF.NErrors++;
		return;
	};
	if(index>63){
		IntErr("SetLightSpot : Invalid  <Index>");
		SCENINF.NErrors++;
		return;
	};
	SCENINF.LSpot[index].x=x0;
	SCENINF.LSpot[index].y=y0;
	SCENINF.LSpot[index].Type=R;
};
DLLEXPORT void ClearLightSpot(byte index){
	if(index>63){
		IntErr("ClearLightSpot : Invalid  <Index>");
		SCENINF.NErrors++;
		return;
	};
	SCENINF.LSpot[index].x=0;
	SCENINF.LSpot[index].y=0;
	SCENINF.LSpot[index].Type=0;
};
extern int RealLx;
extern int RealLy;
extern float fMapX;
extern float fMapY;
void SetCameraPos(float x,float y);
void SetCameraPosEx(float x,float y);
void SetScreenCenterToXY(int x, int y){
	//SetCameraPosEx(x / 32.0f, (y - GetTotalHeight(x, y)) / 32.0f);
	GameInterface::ScrollToPositionOnMap(x,y,400);
}
DLLEXPORT void SetStartPoint(GAMEOBJ* Zone){
	int x;
	int y;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		x=int(Zone->Index);
		y=int(Zone->Serial);
	}else
	if(Zone->Type=='ZONE'){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		x=AZ->x;
		y=AZ->y;
	}else{
		IntErr("SetStartPoint : Invalid <Zone>");
		return;
	};	
	SetScreenCenterToXY(x,y);
};
void AttackObjLink(OneObject* OBJ);
int GetTopDistance(int xa,int ya,int xb,int yb);
bool CheckVisibility(int x1,int y1,int x2,int y2,word MyID);
DLLEXPORT 
void AttackEnemyInZone(GAMEOBJ* Grp,GAMEOBJ* Zone,byte EnmNation){
	int xc,yc,R;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
		R=(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type!='ZONE'){
		IntErr("AttackEnemyInZone : Invalid parameter <Zone>");
		SCENINF.NErrors++;
		return;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	};
	word ENLIST[800];
	int NEN=0;
	word* enm=NatList[EnmNation];
	int Nu=NtNUnits[EnmNation];
	int MinR=100000000;
	word EMID=0xFFFF;
	for(int i=0;i<Nu;i++){
		OneObject* OB=Group[enm[i]];
		if(OB&&!(OB->Sdoxlo)){
			int r0=Norma(OB->RealX-xc,OB->RealY-yc);
			if(r0<R&&NEN<800){
				ENLIST[NEN]=OB->Index;
				NEN++;
			};
		};
	};
	if(NEN){
		UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];
		for(int i=0;i<UG->N;i++){
			word MID=UG->IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
					if(!OB->LocalOrder){
						//need to find appropriate unit;
						//1.Search for unit in the attack area
						AdvCharacter* ADC=OB->MoreCharacter;
						int R0=ADC->MinR_Attack;
						int R1=ADC->MaxR_Attack;
						byte kmask=OB->newMons->KillMask;
						bool KBUI=OB->newMons->AttBuild;
						word EMID=0xFFFF;
						int Dist=10000;
						int Type=-1;
						int tx0=OB->RealX>>10;
						int ty0=OB->RealY>>10;
						for(int j=0;j<NEN;j++){
							OneObject* EOB=Group[ENLIST[j]];
							if(kmask&EOB->MathMask&&!EOB->NewBuilding){
								int R=Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)>>4;
								R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,tx0,ty0);
								int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY);
								if(Rv>R0&&Rv<R1){
									if(R<Dist){
										EMID=EOB->Index;
										Dist=R;
										Type=0;
									};
								};
							};
						};
						Dist=10000;
						if(EMID==0xFFFF&&KBUI){
							for(int j=0;j<NEN;j++){
								OneObject* EOB=Group[ENLIST[j]];
								if(EOB->NewBuilding){
									int R=Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)>>4;
									int Rt=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,tx0,ty0);
									if(R>R0&&R<R1){
										if(Rt+R<Dist){
											EMID=EOB->Index;
											Dist=Rt+R;
											Type=1;
										};
									};
								};
							};
						};
						bool Vis=true;
						word OldEMID=EMID;
						if(EMID!=0xFFFF&&(KBUI||R1>150)){
							OneObject* EOB=Group[EMID];
							//if(!CheckVisibility(EOB->RealX,EOB->RealY,OB->RealX,OB->RealY,OB->Index)){
							//	Vis=false;
							//	EMID=0xFFFF;
							//};
						};
						Dist=100000;
						if(EMID==0xFFFF&&KBUI){
							//search for walls
							for(int j=0;j<NEN;j++){
								OneObject* EOB=Group[ENLIST[j]];
								if(EOB->Wall){
									int R=Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)>>4;
									if(R>R0){
										if(R<Dist){
											EMID=EOB->Index;
											Dist=R;
											Type=2;
										};
									};
								};
							};
						};
						if(EMID==0xFFFF&&OldEMID!=0xFFFF)EMID=OldEMID;
						if(EMID==0xFFFF){
							for(int j=0;j<NEN;j++){
								OneObject* EOB=Group[ENLIST[j]];
								if(kmask&EOB->MathMask||(KBUI&&EOB->NewBuilding)){
									int R=Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)>>4;
									int Rt=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,tx0,ty0);
									if(R>R0&&Rt<560){
										if(Rt<Dist){
											EMID=EOB->Index;
											Dist=Rt;
										};
									};
								};
							};
						};
						if(EMID!=0xFFFF){
							OB->AttackObj(EMID,1,0,0);
						};
					}else{
						if(rando()<600&&OB->LocalOrder->DoLink==&AttackObjLink){
							//word en=OB->EnemyID;
							//if(en!=0xFFFF){
							//	OneObject* EO=Group[en];
							//	if(EO->Serial==OB->EnemySN){
							//		if((Norma(EO->RealX-OB->RealX,EO->RealY-OB->RealY)>>4)>OB->MoreCharacter->MaxR_Attack){
							//			addrand(177);OB->DeleteLastOrder();
							//		};
							//	};
							//};
							addrand(177);OB->DeleteLastOrder();
						};
					};
				};
			};
		};
	};
};
DLLEXPORT 
bool AttackEnemyPeasantsInZone(GAMEOBJ* Grp,GAMEOBJ* Zone,byte EnmNation){
	int xc,yc,R;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
		R=(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type!='ZONE'){
		IntErr("AttackEnemyPeasantsInZone : Invalid parameter <Zone>");
		SCENINF.NErrors++;
		return 0;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	};
	word ENLIST[800];
	int NEN=0;
	word* enm=NatList[EnmNation];
	int Nu=NtNUnits[EnmNation];
	int MinR=100000000;
	word EMID=0xFFFF;
	for(int i=0;i<Nu;i++){
		word MID=enm[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->newMons->Peasant){
				int r0=Norma(OB->RealX-xc,OB->RealY-yc);
				if(r0<R&&NEN<800){
					ENLIST[NEN]=OB->Index;
					NEN++;
				};
			};
		}
	};
	bool somdon=0;
	if(NEN){
		UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];
		for(int i=0;i<UG->N;i++){
			word MID=UG->IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
					if(!OB->LocalOrder){
						//need to find appropriate unit;
						//1.Search for unit in the attack area
						AdvCharacter* ADC=OB->MoreCharacter;
						int R0=ADC->MinR_Attack;
						int R1=ADC->MaxR_Attack;
						byte kmask=OB->newMons->KillMask;
						bool KBUI=OB->newMons->AttBuild;
						word EMID=0xFFFF;
						int Dist=10000;
						int Type=-1;
						int tx0=OB->RealX>>10;
						int ty0=OB->RealY>>10;
						for(int j=0;j<NEN;j++){
							OneObject* EOB=Group[ENLIST[j]];
							if(kmask&EOB->MathMask&&!EOB->NewBuilding&&OB->LockType==EOB->LockType){
								int R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,tx0,ty0);
								int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
								if(Rv>R0&&Rv<R1){
									if(R<Dist){
										EMID=EOB->Index;
										Dist=R;
										Type=0;
									};
								};
							};
						};
						Dist=10000;
						if(EMID==0xFFFF&&KBUI){
							for(int j=0;j<NEN;j++){
								OneObject* EOB=Group[ENLIST[j]];
								if(EOB->NewBuilding){
									int R=Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)>>4;
									int Rt=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,tx0,ty0);
									if(R>R0&&R<R1){
										if(Rt+R<Dist){
											EMID=EOB->Index;
											Dist=Rt+R;
											Type=1;
										};
									};
								};
							};
						};
						bool Vis=true;
						word OldEMID=EMID;
						if(EMID!=0xFFFF&&(KBUI||R1>150)){
							OneObject* EOB=Group[EMID];
							//if(!CheckVisibility(EOB->RealX,EOB->RealY,OB->RealX,OB->RealY,OB->Index)){
							//	Vis=false;
							//	EMID=0xFFFF;
							//};
						};
						Dist=100000;
						if(EMID==0xFFFF&&KBUI){
							//search for walls
							for(int j=0;j<NEN;j++){
								OneObject* EOB=Group[ENLIST[j]];
								if(EOB->Wall){
									int R=Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)>>4;
									if(R>R0){
										if(R<Dist){
											EMID=EOB->Index;
											Dist=R;
											Type=2;
										};
									};
								};
							};
						};
						if(EMID==0xFFFF&&OldEMID!=0xFFFF)EMID=OldEMID;
						if(EMID==0xFFFF){
							for(int j=0;j<NEN;j++){
								OneObject* EOB=Group[ENLIST[j]];
								if(kmask&EOB->MathMask||(KBUI&&EOB->NewBuilding)){
									int R=Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)>>4;
									int Rt=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,tx0,ty0);
									if(R>R0&&Rt<560){
										if(Rt<Dist){
											EMID=EOB->Index;
											Dist=Rt;
										};
									};
								};
							};
						};
						if(EMID!=0xFFFF){
							if(OB->BrigadeID!=0xFFFF){
								OB->Nat->CITY->Brigs[OB->BrigadeID].ClearBOrders();
							};
							OB->AttackObj(EMID,128+15,0,0);
							somdon=1;
						};
					}else{
						somdon=1;
						if(rando()<600&&OB->LocalOrder->DoLink==&AttackObjLink){
							//word en=OB->EnemyID;
							//if(en!=0xFFFF){
							//	OneObject* EO=Group[en];
							//	if(EO->Serial==OB->EnemySN){
							//		if((Norma(EO->RealX-OB->RealX,EO->RealY-OB->RealY)>>4)>OB->MoreCharacter->MaxR_Attack){
							//			addrand(177);OB->DeleteLastOrder();
							//		};
							//	};
							//};
							addrand(177);OB->DeleteLastOrder();
						};
					};
				};
			};
		};
	};
	return somdon;
};
DLLEXPORT
int GetMyRefNation(){
	return GSets.CGame.cgi_NatRefTBL[MyNation];
};
DLLEXPORT int GetMyNation(){
	return MyNation;
};
CEXPORT int GetRefNation(byte NI){
	if(NI<8){
		return GSets.CGame.cgi_NatRefTBL[NI];
	}
	return 0;	
}
int GetTopDistance(int xa,int ya,int xb,int yb);
DLLEXPORT
int GetTopDst(GAMEOBJ* Z1,GAMEOBJ* Z2){
	int xc1,yc1,xc2,yc2;
	if((Z1->Type&0xFF000000)==('@   '-0x202020)){
		xc1=Z1->Index;
		yc1=Z1->Serial;
	}else
	if(Z1->Type!='ZONE'){
		IntErr("GetTopDst : Invalid parameter <Z1>");
		SCENINF.NErrors++;
		return 0xFFFF;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Z1->Index].ZoneID[0]];
		xc1=AZ->x;
		yc1=AZ->y;
	};
	if((Z2->Type&0xFF000000)==('@   '-0x202020)){
		xc2=Z2->Index;
		yc2=Z2->Serial;
	}else
	if(Z2->Type!='ZONE'){
		IntErr("GetTopDst : Invalid parameter <Z2>");
		SCENINF.NErrors++;
		return 0xFFFF;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Z2->Index].ZoneID[0]];
		xc2=AZ->x;
		yc2=AZ->y;
	};
	return GetTopDistance(xc1>>6,yc1>>6,xc2>>6,yc2>>6);
};
extern City CITY[8];
extern GlobalEnemyInfo GNFO;
void ResearchCurrentIsland(byte Nat);
CEXPORT
void StartAIEx(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty);
DLLEXPORT
void StartAI(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty){
	byte MSKS[8];
	for(int i=0;i<8;i++){
		MSKS[i]=NATIONS[i].NMask;
		if(i){
			NATIONS[i].NMask=0x7E;
		}else{
			NATIONS[i].NMask=1;
		};
	};
	GNFO.Clear();
	GNFO.Setup();
	for(i=0;i<8;i++){
		NATIONS[i].NMask=MSKS[i];
	};
	StartAIEx(Nat,Name,Land,Money,ResOnMap,Difficulty);
};
CEXPORT	
void StartAIEx(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty){
	if(!(strstr(Name,".xml")||strstr(Name,".XML"))){
		Name=GetTextByID(Name);
	}	
	if(MOptions.Players.Player[Nat].CustomAI_Script.pchar()&&MOptions.Players.Player[Nat].CustomAI_Script.pchar()[0]){
		Name=MOptions.Players.Player[Nat].CustomAI_Script.pchar();
	}
	if(Name[0]==0) return;// grey [25.03.2004]
	Nat=AssignTBL[Nat];	
	//LoadAIFromDLL(Nat,GetTextByID(Name));
	void LoadScriptedAI(int Nat,char* file);	
	LoadScriptedAI(Nat,Name);
	if(Difficulty!=-1){
		CITY[Nat].Difficulty=Difficulty;
		//AddXRESRC(Nat,FoodID,1000*Difficulty);
		//AddXRESRC(Nat,TreeID,500*Difficulty);
		//AddXRESRC(Nat,StoneID,500*Difficulty);
	}
	CITY[Nat].StartRes=Money;
	CITY[Nat].LandType=Land;
	CITY[Nat].ResOnMap=ResOnMap;	

	City* CT=CITY+Nat;
	CT->AllWariorsGroup=SCENINF.GetNewGroupIndex();
	
	strcpy(CITY[Nat].AIID,Name);
	NATIONS[Nat].AI_Enabled=1;
	for(int i=0;i<8;i++)ResearchCurrentIsland(i);
	word* Units=NatList[Nat];
	int N=NtNUnits[Nat];
	int NML=0;
	int cxi[4];
	int cyi[4];
	for(i=0;i<N;i++){
		OneObject* OB=Group[Units[i]];
		if(OB){
			byte Usage=OB->newMons->Usage;
			if(Usage==MelnicaID&&NML<4){
				if(CITY[Nat].NMeln<4){
					CITY[Nat].MelnID[NML]=OB->Index;
					CITY[Nat].MelnSN[NML]=OB->Serial;
					cxi[NML]=OB->RealX>>4;
					cyi[NML]=OB->RealY>>4;
					NML++;
				};
			};
			NewMonster* NM=OB->newMons;
			if(NM->ResConcentrator&1){//wood
				UnitsGroup* UG=&SCENINF.UGRP[CITY[Nat].WoodSkladGroup];
				UG->RefreshUnits();
				UG->AddNewUnit(OB);
			}
			if(NM->ResConcentrator&4){//stone
				UnitsGroup* UG=&SCENINF.UGRP[CITY[Nat].StoneSkladGroup];
				UG->RefreshUnits();
				UG->AddNewUnit(OB);
			}
			if(NM->ResConcentrator&8){//food
				UnitsGroup* UG=&SCENINF.UGRP[CITY[Nat].MelnGroup];
				UG->RefreshUnits();
				UG->AddNewUnit(OB);
			}			
		}
	}
	CITY[Nat].NMeln=NML;
	CITY[Nat].NFields=0;
	if(NML){
		int NF=0;
		OneSprite* OS=Sprites;
		for(i=0;i<MaxSprt&&NF<512;i++){
			if(OS->Enabled&&OS->SG==&COMPLEX){
				int cx=OS->x;
				int cy=OS->y;
				for(int j=0;j<NML;j++){
					if(Norma(cx-cxi[j],cy-cyi[j])<800){
						CITY[Nat].FieldsID[NF]=i;
						CITY[Nat].FieldsSN[NF]=(OS->x>>5)+((OS->y>>5)<<8);
						NF++;
					};
				};
			};
			OS++;
		};
		CITY[Nat].NFields=NF;
	};
	CITY[Nat].FieldReady=0;
	//registering storages&centers
	Log.Warning("");
	Log.Warning("Started AI: %s",Name);
	Log.Warning("");

	if( VAI() )
		VAI()->OnAIStarted(Nat);

};
DLLEXPORT
void DoNotUseSelInAI(byte Nat){
	Nat=AssignTBL[Nat];
	word* SL=Selm[Nat];
	word* SN=SerN[Nat];
	int N=NSL[Nat];
	for(int i=0;i<N;i++){
		word MID=SL[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||OB->NewBuilding)){
				OB->Borg=true;
				OB->DoNotCall=true;
				OB->NoBuilder=1;
			};
		};
	};
};
extern bool Tutorial;
extern bool NoPress;
extern bool TutOver;
extern bool MiniActive;
void ClearMINIMENU();
DLLEXPORT
void SetTutorial(bool State){
	Tutorial=State;
	//MiniActive=0;
	ClearMINIMENU();
};
DLLEXPORT
bool GetQuestPressed(){
	return (!NoPress)&&TutOver&&Tutorial;
};
#define DEVELOPMENT	   0x1001
#define WATER_BATTLE   0x1002
#define LAND_BATTLE    0x1003
#define MINES_CPTURE   0x1004
#define TOWN_DEFENCE   0x1005
#define MINES_UPGRADE  0x1006
#define FAST_DIVERSION 0x1007
DLLEXPORT
void SetAIProperty(byte NAT,int Prop,int Val){
	City* CT=CITY+NAT;
	switch(Prop){
	case DEVELOPMENT:
		CT->AI_DEVELOPMENT=!!Val;
		break;
	case WATER_BATTLE:
		CT->AI_WATER_BATTLE=!!Val;
		break;
	case LAND_BATTLE:
		CT->AI_LAND_BATTLE=!!Val;
		break;
	case MINES_CPTURE:
		CT->AI_MINES_CPTURE=!!Val;
		break;
	case TOWN_DEFENCE: 
		CT->AI_TOWN_DEFENCE=!!Val;
		break;
	case MINES_UPGRADE:
		CT->AI_MINES_UPGRADE=!!Val;
		break;
	case FAST_DIVERSION: 
		CT->AI_FAST_DIVERSION=!!Val;
		break;
	default:
		IntErr("SetAIProperty : Unknown property");
	};
};
int ProcessMultilineQuestion(int Nx,char* Bmp1,byte or1,char* Text1,char* Quest);
DLLEXPORT
int AskMultilineQuestion(int Nx,char* Name1,byte or1,char* Quest){
	if(PlayGameMode)RGAME.Extract();
	int q1=-1;
	int q2=-1;
	for(int i=0;i<SCENINF.NPages;i++){
		if(!strcmp(Name1,SCENINF.PageID[i])){
			q1=i;
			goto qq2;
		};
	};
	IntErr("AskMultilineQuestion : Unknown page : %s",Name1);
	return 0;
qq2:
	int t=ProcessMultilineQuestion(Nx,SCENINF.PageBMP[q1],or1,SCENINF.Page[q1],Quest);
	if(RecordMode){
		RGAME.AddByte(3);
		RGAME.AddByte(t);
	};
	return t;
};
DLLEXPORT
void SetReadyState(GAMEOBJ* Units,bool State){
	if(Units->Type!='UNIT'){
		IntErr("TakeFood(Units) : Incorrect parameter");
		return;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(!OB->LocalOrder)&&OB->Serial==SIDS[p]){
				OB->Ready=State;
			};
		};
	};
};
DLLEXPORT
void TakeFood(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("TakeFood(Units) : Incorrect parameter");
		return;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(!OB->LocalOrder)&&OB->Serial==SIDS[p]&&OB->newMons->Peasant){
				OB->TakeResource(OB->RealX>>4,OB->RealY>>4,FoodID,128,0);
			};
		};
	};
};
DLLEXPORT
void TakeWood(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("TakeWood(Units) : Incorrect parameter");
		return;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(!OB->LocalOrder)&&OB->Serial==SIDS[p]&&OB->newMons->Peasant){
				OB->TakeResource(OB->RealX>>4,OB->RealY>>4,TreeID,128,0);
			};
		};
	};
};
DLLEXPORT
void TakeStone(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("TakeStone(Units) : Incorrect parameter");
		return;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(!OB->LocalOrder)&&OB->Serial==SIDS[p]&&OB->newMons->Peasant){
				OB->TakeResource(OB->RealX>>4,OB->RealY>>4,StoneID,128,0);
			};
		};
	};
};
void BuildWithSelected(byte NI,word ObjID,byte OrdType);
DLLEXPORT
void RepairBuildingsBySel(byte Nat,GAMEOBJ* Buildings){
	if(Nat>7)return;
	if(Buildings->Type!='UNIT'){
		IntErr("RepairBuildings(Nat,Buildings) : Incorrect parameter <Buildings>");
		return;
	};
	word* UIDS=SCENINF.UGRP[Buildings->Index].IDS;
	word* SIDS=SCENINF.UGRP[Buildings->Index].SNS;
	int Nu=SCENINF.UGRP[Buildings->Index].N;
	byte otp=0;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&OB->Serial==SIDS[p]&&OB->NewBuilding){
				BuildWithSelected(Nat,OB->Index,otp);
				otp=1;
			};
		};
	};
};
DLLEXPORT
bool CheckBuildingsComplete(GAMEOBJ* Buildings){
	if(Buildings->Type!='UNIT'){
		IntErr("CheckBuildingsComplete(Buildings) : Incorrect parameter <Buildings>");
		return false;
	};
	word* UIDS=SCENINF.UGRP[Buildings->Index].IDS;
	word* SIDS=SCENINF.UGRP[Buildings->Index].SNS;
	int Nu=SCENINF.UGRP[Buildings->Index].N;
	byte otp=0;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(!OB->LocalOrder)&&OB->Serial==SIDS[p]
				&&OB->NewBuilding&&OB->Stage<OB->MoreCharacter->ProduceStages)return false;
		};
	};
	return true;
};
DLLEXPORT
int GetKilled(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("GetKilled(Units) : Incorrect parameter <Units>");
		return 0;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	int NK=0;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(!OB->LocalOrder)&&OB->Serial==SIDS[p]){
				NK+=OB->Kills;
			};
		};
	};
	return NK;
};
DLLEXPORT
int GetUnitsByNation(GAMEOBJ* Units,byte Nat){
	if(Units->Type!='UNIT'){
		IntErr("GetUnitsByNation(Units,Nat) : Incorrect parameter <Units>");
		return 0;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	int NK=0;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(OB->Serial>>1)==(SIDS[p]>>1)&&OB->NNUM==Nat)NK++;
		};
	};
	return NK;
};
DLLEXPORT
void ProduceUnit(GAMEOBJ* Units,GAMEOBJ* UnitType,GAMEOBJ* DestGroup){
	if(Units->Type!='UNIT'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <Units>");
		return;
	};
	if(DestGroup->Type!='UNIT'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <DestGroup>");
		return;
	};
	if(UnitType->Type!='UTYP'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <UnitType>");
		return;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	int NK=0;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(!OB->LocalOrder)&&(OB->Serial^SIDS[p])<2){
				OB->Produce(UnitType->Index,DestGroup->Index);
			};
		};
	};
};
DLLEXPORT
void ProduceUnitFast(GAMEOBJ* Units,GAMEOBJ* UnitType,GAMEOBJ* DestGroup,int Speed){
	if(Speed>6)Speed=6;
	if(Units->Type!='UNIT'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <Units>");
		return;
	};
	if(DestGroup->Type!='UNIT'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <DestGroup>");
		return;
	};
	if(UnitType->Type!='UTYP'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <UnitType>");
		return;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	int NK=0;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(OB->Serial^SIDS[p])<2){
				OB->Produce(UnitType->Index+8192*(Speed+1),DestGroup->Index);
			};
		};
	};
};
void ProduceObjLink(OneObject* OBJ);
CEXPORT
bool ProduceUnitAtLeast(GAMEOBJ* Units,GAMEOBJ* UnitType,GAMEOBJ* DestGroup,int CollectAmount,int Ticks){
	if(CollectAmount<=0||Ticks<0) return false;
	if(Units->Type!='UNIT'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <Units>");
		return false;
	};
	if(DestGroup->Type!='UNIT'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <DestGroup>");
		return false;
	};
	if(UnitType->Type!='UTYP'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <UnitType>");
		return false;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	int NK=0;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(OB->Serial^SIDS[p])<2){				
				int N=0;
				Order1* Ord=OB->LocalOrder;
				while(Ord){
					if(Ord->DoLink==&ProduceObjLink&&Ord->info.Produce.ObjIndex==UnitType->Index){
						if(Ord->info.Produce.ID==0xFFFE) return false;
						N++;
					}
					Ord=Ord->NextOrder;
				}				
				if(N==0&&N<CollectAmount&&OB->Produce(UnitType->Index,DestGroup->Index)){
					Ord=OB->LocalOrder;
					while(Ord->NextOrder){
						Ord=Ord->NextOrder;
					}
					int p=Ord->info.Produce.NStages-(Ticks<<8);
					//if(p<0) Ord->info.Produce.Progress=0;
					//	else 
					Ord->info.Produce.Progress=p;
					return true;
				}
			};
		};
	};
	return false;
};
CEXPORT
bool ProduceUnitTicks(GAMEOBJ* Units,GAMEOBJ* UnitType,GAMEOBJ* DestGroup,int Ticks){
	if(Ticks<0) return false;
	if(Units->Type!='UNIT'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <Units>");
		return false;
	};
	if(DestGroup->Type!='UNIT'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <DestGroup>");
		return false;
	};
	if(UnitType->Type!='UTYP'){
		IntErr("ProduceUnit(Units,UnitType,Dest) : Invalid parameter <UnitType>");
		return false;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	int NK=0;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(OB->Serial^SIDS[p])<2&&OB->Produce(UnitType->Index,DestGroup->Index)){
				Order1* Ord=OB->LocalOrder;
				while(Ord->NextOrder){
					Ord=Ord->NextOrder;
				}
				int p=Ord->info.Produce.NStages-(Ticks<<8);
				//if(p<0) Ord->info.Produce.Progress=0;
				//	else 
				Ord->info.Produce.Progress=p;
				return true;
			};
		};
	};
	return false;
};
DLLEXPORT
void AddOneUnitToGroup(GAMEOBJ* Units,OneObject* OB){
	int idx=Units->Index;
	if(idx>=SCENINF.UGRP.GetAmount())return;
	UnitsGroup* UG=&SCENINF.UGRP[idx];
	
	
	int Nu=UG->N;
	UG->Allocate(Nu+1);
	word* UIDS=UG->IDS;
	word* SIDS=UG->SNS;
	int uidx=OB->Index;
	for(int i=0;i<Nu;i++)if(uidx==UIDS[i])return;
	UIDS[Nu]=OB->Index;
	SIDS[Nu]=OB->Serial;
	UG->N++;
	
	
};
CEXPORT
void AddBrigadeToGroup(byte NI, GAMEOBJ* pGroup, int Brig_ID){
	if (pGroup->Type!='UNIT') {
		IntErr( "AddBrigadeToGroup : Invalid <pGroup>" );
		return;
	}
	if (NI<0||NI>=8) {
		IntErr( "AddBrigadeToGroup : Invalid <NI>" );
		return;
	}
	if (Brig_ID<0||Brig_ID>=CITY[NI].NBrigs) {
		IntErr( "AddBrigadeToGroup : Invalid <Brig_ID>" );
		return;
	}
	int			NU	= CITY[NI].Brigs[Brig_ID].NMemb;
	word*		pUI = CITY[NI].Brigs[Brig_ID].Memb;
	word*		pUS = CITY[NI].Brigs[Brig_ID].MembSN;
	OneObject*	pOB = NULL;
	for (int i=0; i<NU; i++){
		pOB = Group[ pUI[i] ];
		if (pOB&&pOB->Serial==pUS[i]) {
			AddOneUnitToGroup(pGroup,pOB);
		}
	}
}
DLLEXPORT
void SaveSelectedUnits(byte NI,GAMEOBJ* Units,bool add){
	if(Units->Type!='UNIT'){
		IntErr("SaveSelectedUnits : Invalid <Units>");
		return;
	};
	if(!add){	
		if(SCENINF.UGRP[Units->Index].IDS){
			SCENINF.UGRP[Units->Index].N=0;
		};
	};
	int NS=NSL[NI];
	word* uni=Selm[NI];
	word* sns=SerN[NI];
	for(int i=0;i<NS;i++){
		word MID=uni[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==sns[i]){
				AddOneUnitToGroup(Units,OB);
			};
		};
	};
};
DLLEXPORT
bool CheckProduction(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("CheckProduction : Invalid <Units>");
		return false;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	for(int i=0;i<Nu;i++){
		word MID=UIDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SIDS[i]&&OB->NewBuilding&&OB->Ready&&!(OB->Sdoxlo||OB->LocalOrder))
				return true;
		};
	};
	return false;
};
DLLEXPORT
bool CreateBuilding(byte Nat,GAMEOBJ* Zone,GAMEOBJ* UnitType,GAMEOBJ* DestGroup){
	if(Nat>7)return false;
	if(UnitType->Type!='UTYP'){
		IntErr("CreateBuilding(Zone,UnitType,DestGroup) : Invalid <UnitType>");
		return false;
	};
	if(DestGroup->Type!='UNIT'){
		IntErr("CreateBuilding(Zone,UnitType,DestGroup) : Invalid <DestGroup>");
		return false;
	};
	int zx=0;
	int zy=0;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		int R0=Zone->Type&0x00FFFFFF;
		zx=int(Zone->Index)<<4;
		zy=int(Zone->Serial)<<4;
	}else{
		if(Zone->Type=='ZONE'){
			zx=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]]->x<<4;
			zy=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]]->y<<4;
		}else{
			IntErr("CreateBuilding(Zone,UnitType,DestGroup) : Invalid <Zone>");
			return false;
		};
	};
	int id=NATIONS[Nat].CreateNewMonsterAt(zx,zy,UnitType->Index,0);
	if(id!=-1){
		OneObject* OB=Group[id];
		int N=SCENINF.UGRP[DestGroup->Index].N;
		SCENINF.UGRP[DestGroup->Index].Allocate(N+1);
		SCENINF.UGRP[DestGroup->Index].IDS[N]=id;
		SCENINF.UGRP[DestGroup->Index].SNS[N]=OB->Serial;
		SCENINF.UGRP[DestGroup->Index].N++;
		return true;
	};
	return false;
};
DLLEXPORT
void SetDestPoint(GAMEOBJ* Units,GAMEOBJ* Zone){
	if(Units->Type!='UNIT'){
		IntErr("SetDestPoint(Units,Zone) : Incorrect parameter <Units>");
		return;
	};
	int zx=0;
	int zy=0;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		int R0=Zone->Type&0x00FFFFFF;
		zx=int(Zone->Index)<<4;
		zy=int(Zone->Serial)<<4;
	}else{
		if(Zone->Type=='ZONE'){
			zx=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]]->x<<4;
			zy=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]]->y<<4;
		}else{
			IntErr("SetDestPoint(Units,Zone) : Incorrect parameter <Zone>");
			return;
		};
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	int NK=0;
	for(int p=0;p<Nu;p++){
		word MID=UIDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&(!OB->LocalOrder)&&OB->Serial==SIDS[p]&&OB->Ref.General->CanDest){
				OB->DstX=zx;
				OB->DstY=zy;
			};
		};
	};
};
DLLEXPORT
void RegisterDynGroup(GAMEOBJ* Units){
	Units->Type='UNIT';
	UnitsGroup* UG=SCENINF.GetNewGroup();
	Units->Index=UG->Index;
	Units->Serial=0;
};
DLLEXPORT
void DeleteDynGroup(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("DeleteDynGroup: Incorrect parameter <Units>");
		return;
	}
	SCENINF.DeleteGroup(Units->Index);
}
DLLEXPORT
void RegisterDynGroupEx(GAMEOBJ* Units,char* file,int Line){
	Units->Type='UNIT';
	UnitsGroup* UG=SCENINF.GetNewGroup();
	Units->Index=UG->Index;
	UG->Index=UG->Index;
	Units->Serial=0;
	UG->file=file;
	UG->Line=Line;
};
DLLEXPORT int GetNUnits(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("GetNUnits(Units) : Incorrect parameter <Units>");
		return 0;
	};
	return SCENINF.UGRP[Units->Index].N;
};
struct OneUnit{
	word Index;
	word Serial;
	word Life;
	word MaxLife;
	byte AddDamage;
	byte AddShield;
	word Stage;
	word MaxStage;
	word Kills;
	word NIndex;
	byte Usage;
	byte Building;
	int  x,y;
	byte NI;
	byte Direction;
	byte IsBuilt;
	byte ChargeProgress;
	byte GroundState;
	byte NMask;
	byte Busy;
	byte Attack;
	byte Reserved[8];
};
DLLEXPORT bool GetUnitInfo(GAMEOBJ* Units,int Index,OneUnit* Uni){
	Uni->Index=0xFFFF;
	if(Units->Type!='UNIT'||Units->Index>=SCENINF.UGRP.GetAmount()){
		IntErr("GetUnitInfo(Units,Index,Uni) : Incorrect parameter <Units>");
		return false;
	};
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	if(Index>=Nu)return false;
	if(UIDS[Index]!=0xFFFF){
		OneObject* OB=Group[UIDS[Index]];
		if(OB&&OB->Serial==SIDS[Index]&&((!OB->Sdoxlo)||OB->Hidden)){
			if(EngSettings.MoraleType!=1){
				Uni->AddDamage=OB->AddDamage;
				Uni->AddShield=OB->AddShield;
			}
			Uni->Building=OB->NewBuilding;
			Uni->Index=OB->Index;
			Uni->Kills=OB->Kills;
			Uni->Life=OB->Life;
			Uni->MaxLife=OB->MaxLife;
			Uni->MaxStage=OB->MoreCharacter->ProduceStages;
			Uni->NIndex=OB->NIndex;
			Uni->Serial=OB->Serial;
			Uni->Stage=OB->Stage;
			Uni->Usage=OB->newMons->Usage;
			Uni->x=OB->RealX>>4;
			Uni->y=OB->RealY>>4;

			Uni->NI=OB->NNUM;
			Uni->IsBuilt=OB->Stage==OB->MoreCharacter->ProduceStages;
			Uni->Direction=OB->RealDir;
			if(OB->MaxDelay)Uni->ChargeProgress=OB->delay*100/OB->MaxDelay;
			else Uni->ChargeProgress=0;
			Uni->GroundState=OB->GroundState;
			Uni->NMask=OB->NMask;
			Uni->Busy=OB->LocalOrder!=NULL;
			Uni->Attack=OB->Attack;
			return true;
		};
	};
	return false;
};
DLLEXPORT bool GetUnitGlobalInfo(int Index,OneUnit* Uni){
	Uni->Index=0xFFFF;
	OneObject* OB=Group[Index];
	if(OB&&Index<0xFFFF&&((!OB->Sdoxlo)||OB->Hidden)){
		if(EngSettings.MoraleType!=1){
			Uni->AddDamage=OB->AddDamage;
			Uni->AddShield=OB->AddShield;
		}
		Uni->Building=OB->NewBuilding;
		Uni->Index=OB->Index;
		Uni->Kills=OB->Kills;
		Uni->Life=OB->Life;
		Uni->MaxLife=OB->MaxLife;
		Uni->MaxStage=OB->MoreCharacter->ProduceStages;
		Uni->NIndex=OB->NIndex;
		Uni->Serial=OB->Serial;
		Uni->Stage=OB->Stage;
		Uni->Usage=OB->newMons->Usage;
		Uni->x=OB->RealX>>4;
		Uni->y=OB->RealY>>4;

		Uni->NI=OB->NNUM;
		Uni->IsBuilt=OB->Stage==OB->MoreCharacter->ProduceStages;
		Uni->Direction=OB->RealDir;
		if(OB->MaxDelay)Uni->ChargeProgress=OB->delay*100/OB->MaxDelay;
		else Uni->ChargeProgress=0;
		Uni->Busy=OB->LocalOrder!=NULL;
		return true;
	};
	return false;
};
DLLEXPORT
void SetUnitInfo(OneUnit* Uni){
	if(Uni->Index!=0xFFFF){
		OneObject* OB=Group[Uni->Index];
		if(OB&&OB->Serial==Uni->Serial&&(!OB->Sdoxlo)){
			OB->Life=Uni->Life;
			OB->Kills=Uni->Kills;//grey 
			if(EngSettings.MoraleType!=1){
				OB->AddDamage=Uni->AddDamage;
				OB->AddShield=Uni->AddShield;
			}
			if(!OB->NewBuilding){
				OB->RealX=Uni->x<<4;
				OB->RealY=Uni->y<<4;
			};
		};
	};
};
DLLEXPORT
void SetUnitInfoNew(OneUnit* Uni){
	if(Uni->Index!=0xFFFF){
		OneObject* OB=Group[Uni->Index];
		if(OB&&OB->Serial==Uni->Serial&&(!OB->Sdoxlo)){
			OB->Life=Uni->Life;
			OB->AddDamage=Uni->AddDamage;
			OB->AddShield=Uni->AddShield;
			if(!OB->NewBuilding){
				OB->RealX=Uni->x<<4;
				OB->RealY=Uni->y<<4;
			};
			OB->RealDir=Uni->Direction;
			OB->GroundState=Uni->GroundState;
			OB->NMask=Uni->NMask;
		};
	};
};

DLLEXPORT
word GetUnitBrigadeID(int Index) { //grey[20.01.2005]	(Uni->Index)
	OneObject* OB=Group[Index];
	if(OB&&Index<0xFFFF&&((!OB->Sdoxlo)||OB->Hidden)) {
		return  OB->BrigadeID;
	};
	return  0xFFFF;
};
void PanicUnit(OneObject* OB);
DLLEXPORT
void PanicUnitByIndex(int Index) { //grey[15.02.2005]	(Uni->Index) //very new
	OneObject* OB=Group[Index];
	if(OB&&Index<0xFFFF&&((!OB->Sdoxlo)||OB->Hidden)) {
		PanicUnit(OB);
	};
};
DLLEXPORT void RemoveGroup(GAMEOBJ* Source,GAMEOBJ* Dest){
	if(Source->Type!='UNIT'){
		IntErr("RemoveGroup(Source,Dest) : Invalid <Source>");
		return;
	};
	if(Dest->Type!='UNIT'){
		IntErr("RemoveGroup(Source,Dest) : Invalid <Dest>");
		return;
	};

	word* UIDS=SCENINF.UGRP[Source->Index].IDS;
	word* SIDS=SCENINF.UGRP[Source->Index].SNS;
	int Ns=SCENINF.UGRP[Source->Index].N;
	int Nd=SCENINF.UGRP[Dest->Index].N;
	if(Ns){
		
		
		SCENINF.UGRP[Dest->Index].Allocate(Ns+Nd);
		memcpy(SCENINF.UGRP[Dest->Index].IDS+Nd,SCENINF.UGRP[Source->Index].IDS,Ns*2);
		memcpy(SCENINF.UGRP[Dest->Index].SNS+Nd,SCENINF.UGRP[Source->Index].SNS,Ns*2);
		SCENINF.UGRP[Source->Index].N=0;
		SCENINF.UGRP[Dest->Index].N+=Ns;
		
		
	};
};
//-------------------------AI Low level functions------------------------//
const int AI_PROB[4]={32768/100,32768/50,32768/10,32768};
byte CurAINation=1;
City* CCIT=NULL;
Nation* CNAT;
#define PRC(x) ((32768*##x##)/100)

int PERCONV[101]={PRC( 0),PRC( 1),PRC( 2),PRC( 3),PRC( 4),PRC( 5),PRC( 6),PRC( 7),PRC( 8),PRC( 9),
                  PRC(10),PRC(11),PRC(12),PRC(13),PRC(14),PRC(15),PRC(16),PRC(17),PRC(18),PRC(19),
				  PRC(20),PRC(21),PRC(22),PRC(23),PRC(24),PRC(25),PRC(26),PRC(27),PRC(28),PRC(29),
				  PRC(30),PRC(31),PRC(32),PRC(33),PRC(34),PRC(35),PRC(36),PRC(37),PRC(38),PRC(39),
				  PRC(40),PRC(41),PRC(42),PRC(43),PRC(44),PRC(45),PRC(46),PRC(47),PRC(48),PRC(49),
				  PRC(50),PRC(51),PRC(52),PRC(53),PRC(54),PRC(55),PRC(56),PRC(57),PRC(58),PRC(59),
				  PRC(60),PRC(61),PRC(62),PRC(63),PRC(64),PRC(65),PRC(66),PRC(67),PRC(68),PRC(69),
				  PRC(70),PRC(71),PRC(72),PRC(73),PRC(74),PRC(75),PRC(76),PRC(77),PRC(78),PRC(79),
				  PRC(80),PRC(81),PRC(82),PRC(83),PRC(84),PRC(85),PRC(86),PRC(87),PRC(88),PRC(89),
				  PRC(90),PRC(91),PRC(92),PRC(93),PRC(94),PRC(95),PRC(96),PRC(97),PRC(98),PRC(99),
				  PRC(100)};
char* CurAIDLL=NULL;
void AIER(char* Mess){
	MessageBox(NULL,Mess,CurAIDLL,MB_TOPMOST);
	assert(0);
};
void AI_Error(){
	AIER("ERROR: AI function must be called only from AI section.");
};
DLLEXPORT
int GetAINation(){
	if(AiIsRunNow){
		return CurAINation;
	}else{
		AI_Error();
		return 0;
	};
};
CEXPORT word GetPortBranch(OneObject* OB){
	if(OB){
		return OB->newMons->PortBranch;
	}
	return 0xFFFF;
}
extern byte CannonState;
int TryToStandBuildingAnywhere(int x,int y,byte NI,int Type,BuildRequirements* Req);

#include "mesh\unihash.h"
uni_hash<DWORD,bi_DWORD> TryUnitLastTime;

void ResetTryUnitLastTime(){
	TryUnitLastTime.reset();
}
void SetTryUnitTime(byte NI,int Type){
	TryUnitLastTime.add_once(bi_DWORD(NI,Type),_DWORD(TrueTime));	
}
int  GetTryUnitLastTimePeriod(byte NI,int Type){
	DWORD* u=TryUnitLastTime.get(bi_DWORD(NI,Type));
	if(u)return TrueTime-*u;
	else return 1000000;
}	

bool TryUnit(byte NI,int UID,int Max,byte CostPercent,byte Probability,BuildRequirements* req){
	if(Probability>100)Probability=100;		
	City* CCIT=CITY+NI;
	if(CCIT->NBestProjects>=127)return false;	
	GeneralObject* GO=NATIONS[NI].Mon[UID];	
	if(CannonState==1){
		if(GO->newMons->Artilery||GO->newMons->Usage==TowerID)return false;
	};	
	GO->Branch=0xFF;	
	word prid=CCIT->PRPIndex[UID];
	if(prid!=0xFFFF){
		ProposedProject* PRP=&CCIT->Prop[prid];
		OneObject* POB=Group[PRP->ProducerIndex];
		bool NEWBTYPE=GO->newMons->Building&&POB&&POB->NewBuilding;
		if((NEWBTYPE&&Max>CCIT->ReadyAmount[UID])||((!NEWBTYPE)&&Max>CCIT->UnitAmount[UID])){
			if(rando()<PERCONV[Probability]&&CostPercent>=PRP->Percent){
				if(GO->newMons->Building){					
					if(POB&&POB->NewBuilding){
						bool BuyBlankBuilding(byte NI, word NIndex);
						BuyBlankBuilding(NI,UID);
					}else{
						if(GO->newMons->Building){
							if(GetTryUnitLastTimePeriod(NI,UID)<3000)return false;
							SetTryUnitTime(NI,UID);
						}
						int id=TryToStandBuildingAnywhere(CITY[NI].CenterX<<7,CITY[NI].CenterY<<7,NI,UID,req);
						return id!=-1;
					}
				}
				if(CCIT->PresentProject){
					if(!GO->newMons->Building){
						CCIT->BestProj[CCIT->NBestProjects]=prid;
						CCIT->NBestProjects++;
						return true;
					}
				}else{
					if(CCIT->AI_DEVELOPMENT||!GO->newMons->Building){
						CCIT->BestProj[CCIT->NBestProjects]=prid;
						CCIT->NBestProjects++;
					}
					return true;
				}
			}
		}
	}
	return false;	
};
DLLEXPORT
bool TryUnit(GAMEOBJ* UnitType,int Max,byte CostPercent,byte Probability){
	if(AiIsRunNow){
		//if(AI_PROB[CCIT->Difficulty]<rando())return false;
		if(UnitType->Type!='UTYP'){
			AIER("TryUnit : invalid <UnitType>");
			return 0;
		};
		if(Probability>100){
			AIER("TryUnit : invalid <Probability>");
			return 0;
		};
		if(CCIT->NBestProjects>=127)return false;
		int UID=UnitType->Index;
		GeneralObject* GO=CNAT->Mon[UID];
		if(CannonState==1){
			if(GO->newMons->Artilery||GO->newMons->Usage==TowerID)return false;
		};
		GO->Branch=0xFF;
		if(Max>CCIT->UnitAmount[UID]){
			word prid=CCIT->PRPIndex[UID];
			if(prid!=0xFFFF){
				ProposedProject* PRP=&CCIT->Prop[prid];
				if(rando()<PERCONV[Probability]&&CostPercent>=PRP->Percent){
					if(CCIT->PresentProject){
						if(!GO->newMons->Building){
							CCIT->BestProj[CCIT->NBestProjects]=prid;
							CCIT->NBestProjects++;
							return true;
						};
					}else{
						if(CCIT->AI_DEVELOPMENT||!GO->newMons->Building){
							CCIT->BestProj[CCIT->NBestProjects]=prid;
							CCIT->NBestProjects++;
						}
						return true;
					};
				};
			};
		};
		return false;
	}else{
		AI_Error();
		return 0;
	};
};
DLLEXPORT
bool TryUpgrade(GAMEOBJ* Upgrade,int CostPercent,int Probability){
	if(Upgrade->Type=='SAFE')return false;
	if(AiIsRunNow){
		//if(AI_PROB[CCIT->Difficulty]<rando())return false;
		if(Upgrade->Type!='UPGR'){
			//AIER("TryUpgrade : invalid <Upgrade>");
			return 0;
		};
		if(Probability>100){
			AIER("TryUpgrade : invalid <Probability>");
			return 0;
		};
		if(CCIT->NBestProjects>=127)return false;
		int UID=Upgrade->Index;
		NewUpgrade* NU=CNAT->UPGRADE[UID];
		NU->Branch=0xFF;
		word prid=CCIT->UPGIndex[UID];
		if(prid!=0xFFFF){
			ProposedProject* PRP=&CCIT->Prop[prid];
			if(rando()<PERCONV[Probability]&&CostPercent>=PRP->Percent){
				CCIT->BestProj[CCIT->NBestProjects]=prid;
				CCIT->NBestProjects++;
				return true;
			};
		};
		return false;
	}else{
		AI_Error();
		return 0;
	};
};
DLLEXPORT
bool TryUpgradeEx(GAMEOBJ* Upgrade,int CostPercent,int Probability,int MaxCenterR,int MaxBuildingR){
	if(Upgrade->Type=='SAFE')return false;
	if(AiIsRunNow){
		//if(AI_PROB[CCIT->Difficulty]<rando())return false;
		if(Upgrade->Type!='UPGR'){
			//AIER("TryUpgrade : invalid <Upgrade>");
			return 0;
		};
		if(Probability>100){
			AIER("TryUpgrade : invalid <Probability>");
			return 0;
		};
		if(CCIT->NBestProjects>=127)return false;
		int UID=Upgrade->Index;
		NewUpgrade* NU=CNAT->UPGRADE[UID];
		NU->Branch=0xFF;
		word prid=CCIT->UPGIndex[UID];
		if(prid!=0xFFFF){
			ProposedProject* PRP=&CCIT->Prop[prid];
			if(rando()<PERCONV[Probability]&&CostPercent>=PRP->Percent){
				//checking radiuses
				ProposedProject* PRP=&CCIT->Prop[prid];
				word MID=PRP->ProducerIndex;
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					int R=Norma((OB->RealX>>4)-(CCIT->CenterX<<7),(OB->RealY>>4)-(CCIT->CenterY<<7));
					if(R<MaxCenterR){
						int MR=100000;
						int N=CCIT->NBuild;
						for(int i=0;i<N;i++){
							word BMID=CCIT->BUILDLISTID[i];
							if(BMID!=0xFFFF){
								OneObject* BOB=Group[BMID];
								if(BOB&&BOB->Serial==CCIT->BUILDLISTSN[i]&&BOB->newMons->Usage!=MineID){
									int RR=Norma(OB->RealX-BOB->RealX,OB->RealY-BOB->RealY)>>4;
									if(RR<MR)MR=RR;
								};
							};
						};
						if(MR<MaxBuildingR){
							CCIT->BestProj[CCIT->NBestProjects]=prid;
							CCIT->NBestProjects++;
							return true;
						};
					};
				};
			};
		};
		return false;
	}else{
		AI_Error();
		return 0;
	};
};
bool TryUpgradeEx(byte NI,int UpgID,int CostPercent,int Probability,int MaxCenterR,int MaxBuildingR){	
	City* CCIT=CITY+NI;
	Nation* CNAT=NATIONS+NI;
	if(CCIT->NBestProjects>=127)return false;
	int UID=UpgID;
	NewUpgrade* NU=CNAT->UPGRADE[UID];
	NU->Branch=0xFF;
	word prid=CCIT->UPGIndex[UID];
	if(prid!=0xFFFF){
		ProposedProject* PRP=&CCIT->Prop[prid];
		if(rando()<PERCONV[Probability]&&CostPercent>=PRP->Percent){
			//checking radiuses
			ProposedProject* PRP=&CCIT->Prop[prid];
			word MID=PRP->ProducerIndex;
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				int R=Norma((OB->RealX>>4)-(CCIT->CenterX<<7),(OB->RealY>>4)-(CCIT->CenterY<<7));
				if(R<MaxCenterR){
					int MR=100000;
					int N=CCIT->NBuild;
					for(int i=0;i<N;i++){
						word BMID=CCIT->BUILDLISTID[i];
						if(BMID!=0xFFFF){
							OneObject* BOB=Group[BMID];
							if(BOB&&BOB->Serial==CCIT->BUILDLISTSN[i]&&BOB->newMons->Usage!=MineID){
								int RR=Norma(OB->RealX-BOB->RealX,OB->RealY-BOB->RealY)>>4;
								if(RR<MR)MR=RR;
							};
						};
					};
					if(MR<MaxBuildingR){
						CCIT->BestProj[CCIT->NBestProjects]=prid;
						CCIT->NBestProjects++;
						return true;
					};
				};
			};
		};
	};
	return false;
}
DLLEXPORT
void SetMineBalanse(int N,word* Bal){
	if(AiIsRunNow){
		CNAT->PBL=Bal;
		CNAT->NPBal=N;
	}else{
		AI_Error();
	};
};
DLLEXPORT
void SetPDistribution(int OnFood,int OnWood,int OnStone){
	if(AiIsRunNow){
		CNAT->POnFood=OnFood;
		CNAT->POnWood=OnWood;
		CNAT->POnStone=OnStone;
	}else{
		AI_Error();
	};
};
DLLEXPORT
void AssignAmountOfMineUpgrades(int MU){
	if(AiIsRunNow){
		CNAT->UGRP_MINEUP.N=MU;
		CNAT->UGRP_MINEUP.UIDS=znew(word,MU);
		CNAT->UGRP_MINEUP.UVAL=znew(word,MU);
		memset(CNAT->UGRP_MINEUP.UIDS,0xFF,MU*2);
		memset(CNAT->UGRP_MINEUP.UVAL,0xFF,MU*2);
	}else{
		AI_Error();
	};
};
DLLEXPORT
bool AssignMineUpgrade(word U,char* Str,word val){
	if(AiIsRunNow){
		if(U<CNAT->UGRP_MINEUP.N){
			NewUpgrade** NUG=CNAT->UPGRADE;
			int N=CNAT->NUpgrades;
			for(int k=0;k<N&&strcmp(NUG[k]->Name,Str);k++);
			if(k>=N){
				char cc[128];
				sprintf(cc,"AssignMineUpgrade : Invalid upgrade name:%s",Str);
				AIER(cc);
				return false;
			}else{
				CNAT->UGRP_MINEUP.UIDS[U]=k;
				CNAT->UGRP_MINEUP.UVAL[U]=val;
			};
		}else{
			AIER("AssignMineUpgrade : invalid index of upgrade");
			return 0;
		};
	}else{
		AI_Error();
		return 0;
	};
	return 1;
};
int GETUID(char* Name){
	GeneralObject** GOG=NATIONS[CurAINation].Mon;
	int N=NATIONS[CurAINation].NMon;
	for(int k=0;k<N&&strcmp(GOG[k]->MonsterID,Name);k++);
	if(k>=N)return -1;
	return k;
};
DLLEXPORT
void AssignMine(char* Name){
	if(AiIsRunNow){
		int id=GETUID(Name);
		if(id==-1){
			AIER("AssignMine : Unknown ID");
		}else CNAT->UID_MINE=id;
	}else{
		AI_Error();
	};
};
DLLEXPORT
void AssignPeasant(char* Name){
	if(AiIsRunNow){
		int id=GETUID(Name);
		if(id==-1){
			AIER("AssignPeasant : Unknown ID");
		}else CNAT->UID_PEASANT=id;
	}else{
		AI_Error();
	};
};
DLLEXPORT
void AssignHouse(char* Name){
	if(AiIsRunNow){
		int id=GETUID(Name);
		if(id==-1){
			AIER("AssignHouse : Unknown ID");
		}else{
			CNAT->UID_HOUSE=id;
			NewMonster* NM=CNAT->Mon[CNAT->UID_HOUSE]->newMons;
			int x0=NM->BuildX0-NM->BuildY0;
			int x1=NM->BuildX1-NM->BuildY1;
			int y0=NM->BuildX0+NM->BuildY0;
			int y1=NM->BuildX1+NM->BuildY1;
			CCIT->FarmLX=(x1-x0)+20;
			CCIT->FarmLY=(y1-y0)+20;
			CCIT->LFarmIndex=0;
			CCIT->LFarmRadius=0;
		};
	}else{
		AI_Error();
	};
};
DLLEXPORT
void AssignWall(char* Name){
	if(AiIsRunNow){
		int id=GETUID(Name);
		if(id==-1){
			AIER("AssignWall : Unknown ID");
		}else CNAT->UID_WALL=id;
	}else{
		AI_Error();
	};
};
DLLEXPORT
void SET_MINE_CAPTURE_RADIUS(int x){
	if(AiIsRunNow){
		CNAT->MINE_CAPTURE_RADIUS=x;
	}else{
		AI_Error();
	};
};
DLLEXPORT
void SET_MINE_UPGRADE1_RADIUS(int x){
	if(AiIsRunNow){
		CNAT->MINE_UPGRADE1_RADIUS=x;
	}else{
		AI_Error();
	};
};
DLLEXPORT
void SET_MINE_UPGRADE2_RADIUS(int x){
	if(AiIsRunNow){
		CNAT->MINE_UPGRADE2_RADIUS=x;
	}else{
		AI_Error();
	};
};
DLLEXPORT
void SET_DEFAULT_MAX_WORKERS(int x){
	if(AiIsRunNow){
		CNAT->DEFAULT_MAX_WORKERS=x;
	}else{
		AI_Error();
	};
};
DLLEXPORT
void SET_MIN_PEASANT_BRIGADE(int x){
	if(AiIsRunNow){
		CNAT->MIN_PBRIG=x;
	}else{
		AI_Error();
	};
};
DLLEXPORT
int GetMoney(byte id){
	if(AiIsRunNow){
		if(id<6){
			return XRESRC(CurAINation,id);
		}else return 0;
	}else{
		AI_Error();
		return 0;
	};
};
DLLEXPORT
int GetUnits(GAMEOBJ* UnitType){
	if(AiIsRunNow){
		if(UnitType->Type!='UTYP'){
			AIER("GetUnits : Invaid unit ID");
			return 0;
		};
		return CCIT->UnitAmount[UnitType->Index];
	}else{
		AI_Error();
		return 0;
	};
};
DLLEXPORT
int GetUnitsByUsage(byte Nat,byte Usage){
	Nat=AssignTBL[Nat];
	if(AiIsRunNow){
		int N=NtNUnits[Nat];
		word* Units=NatList[Nat];
		int NUN=0;
		for(int i=0;i<N;i++){
			word MID=Units[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&OB->newMons->Usage==Usage)NUN++;
			};
		};
		return NUN;
	}else{
		AI_Error();
		return 0;
	};
};
void CmdTorg(byte NI,byte SellRes,byte BuyRes,int SellAmount);
byte INVECO[6]={2,5,4,0,1,3};
void PerformTorg(byte Nation,byte SellRes,byte BuyRes,int SellAmount);
DLLEXPORT
void AI_Torg(byte SellRes,byte BuyRes,int SellAmount){
	PerformTorg(CNAT->NNUM,INVECO[SellRes],INVECO[BuyRes],SellAmount);
};
int GetTorgResultEx(byte SellRes,byte BuyRes,int SellAmount);
DLLEXPORT
int GetTorgResult(byte SellRes,byte BuyRes,int SellAmount){
	return GetTorgResultEx(INVECO[SellRes],INVECO[BuyRes],SellAmount);
};
DLLEXPORT
int GetReadyUnits(GAMEOBJ* UnitType){
	if(AiIsRunNow){
		if(UnitType->Type!='UTYP'){
			AIER("GetReadyUnits : Invaid unit ID");
			return 0;
		};
		return CCIT->ReadyAmount[UnitType->Index];
	}else{
		AI_Error();
		return 0;
	};
};
DLLEXPORT
void SetUpgradeLock(byte Res,byte Val){
	if(Res<8)CCIT->LockUpgrade[Res]=Val;
};

DLLEXPORT
void SetDefSettings(int p1,int p2){
	if(AiIsRunNow){
		CCIT->MaxGuards=p1;
		CCIT->AbsMaxGuards=p2;
	}else{
		AI_Error();
	};
};
DLLEXPORT
int GetMaxPeasantsInMines(){
	int N=NtNUnits[CNAT->NNUM];
	word* Uni=NatList[CNAT->NNUM];
	int Max=0;
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Uni[i]];
		if(OB&&OB->Inside){
			int Nm=OB->MoreCharacter->MaxInside+OB->AddInside;
			if(Nm>Max)Max=Nm;
		};
	};
	return Max;
};
DLLEXPORT
bool UpgIsDone(GAMEOBJ* Upgrade){
	if(AiIsRunNow){
		if(Upgrade->Type!='UPGR'){
			AIER("UpgIsDone : Invalid Upgrade ID");
			return 0;
		};
		return CNAT->UPGRADE[Upgrade->Index]->Done;
	}else{
		AI_Error();
		return 0;
	};
};
DLLEXPORT
bool UpgIsRun(GAMEOBJ* Upgrade){
	if(AiIsRunNow){
		if(Upgrade->Type!='UPGR'){
			AIER("UpgIsDone : Invalid Upgrade ID");
			return 0;
		};
		int id=Upgrade->Index;
		return CNAT->UPGRADE[id]->Done||CNAT->UPGRADE[id]->IsDoing;
	}else{
		AI_Error();
		return 0;
	};
};
extern int* ResTBL;
extern int  NInResTBL;
DLLEXPORT
void SetMinesBuildingRules(int* Table,int NElm){
	ResTBL=Table;
	NInResTBL=NElm;
};
DLLEXPORT
void SetMinesUpgradeRules(int* Table){
	if(!AiIsRunNow)return;
	Nation* Nat=CNAT;
	Nat->MU1G_PERCENT[0]=Table[0];
	Nat->MU1G_PERCENT[1]=Table[1];
	Nat->MU1G_PERCENT[2]=Table[2];

	Nat->MU1I_PERCENT[0]=Table[3];
	Nat->MU1I_PERCENT[1]=Table[4];
	Nat->MU1I_PERCENT[2]=Table[5];

	Nat->MU1C_PERCENT[0]=Table[6];
	Nat->MU1C_PERCENT[1]=Table[7];
	Nat->MU1C_PERCENT[2]=Table[8];
//=============II==============//

	Nat->MU2G_PERCENT[0]=Table[9];
	Nat->MU2G_PERCENT[1]=Table[10];
	Nat->MU2G_PERCENT[2]=Table[11];

	Nat->MU2I_PERCENT[0]=Table[12];
	Nat->MU2I_PERCENT[1]=Table[13];
	Nat->MU2I_PERCENT[2]=Table[14];

	Nat->MU2C_PERCENT[0]=Table[15];
	Nat->MU2C_PERCENT[1]=Table[16];
	Nat->MU2C_PERCENT[2]=Table[17];
//=============III==============//

	Nat->MU3G_PERCENT[0]=Table[18];
	Nat->MU3G_PERCENT[1]=Table[19];
	Nat->MU3G_PERCENT[2]=Table[20];

	Nat->MU3I_PERCENT[0]=Table[21];
	Nat->MU3I_PERCENT[1]=Table[22];
	Nat->MU3I_PERCENT[2]=Table[23];

	Nat->MU3C_PERCENT[0]=Table[24];
	Nat->MU3C_PERCENT[1]=Table[25];
	Nat->MU3C_PERCENT[2]=Table[26];
};
DLLEXPORT
bool FieldExist(){
	return CCIT->FieldReady;
};
DLLEXPORT
int GetDifficulty(){
	return CCIT->Difficulty;
};
DLLEXPORT
int GetStartRes(){
	return CCIT->StartRes;

};
DLLEXPORT
int GetDiff(byte NI){
	return CITY[NI].Difficulty;
};
DLLEXPORT
int GetResOnMap(){
	return CCIT->ResOnMap;
};
DLLEXPORT
int GetLandType(){
	return CCIT->LandType;
};
DLLEXPORT
void SetStandartVictory(){
	SCENINF.StandartVictory=1;
};
DLLEXPORT
bool NationIsErased(byte Nat){
	if(Nat<8){
		Nat=GSets.CGame.cgi_NatRefTBL[Nat];
		int NMyPeasants=0;
		int NMyCenters=0;
		//int NThemPeasants=0;
		//int NThemCenters=0;
		int MyMask=1<<Nat;
		//byte LockN[8];
		//memset(LockN,1,8);
		//for(int i=0;i<8;i++)if(NATIONS[i].VictState==1)LockN[i]=0;
		int NMyUnits=0;
		//NThemUnits=0;
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&!(OB->GetLiveState()!=ls_LiveBody)){
				byte USE=OB->newMons->Usage;
				if(USE==PeasantID){
				if(OB->NNUM==Nat)NMyPeasants++;
					//else if(!(OB->NMask&MyMask))NThemPeasants++;
				}else
				if(USE==CenterID){
					if(OB->NNUM==Nat)NMyCenters++;
					//else if(!(OB->NMask&MyMask))NThemCenters++;
				};
				if(!(OB->LockType||OB->NewBuilding||OB->Wall)){
					if(OB->NNUM==Nat)NMyUnits++;
					//else if(!(OB->NMask&MyMask))NThemUnits++;
				};
			};
		};
		if(NMyPeasants==0&&NMyCenters==0&&NMyUnits<10)NMyUnits=0;
		//if(NThemPeasants==0&&NThemCenters==0&&NThemUnits<10)NThemUnits=0;
		return NMyUnits==0;
	}else return 1;
};
DLLEXPORT
void AssignFormUnit(char* Name){
	GeneralObject** GOS=NATIONS[0].Mon;
	int N=NATIONS[0].NMon;
	for(int i=0;i<N;i++)if(!strcmp(GOS[i]->MonsterID,Name)){
		CNAT->FormUnitID=i;		
		return;
	};
	IntErr("AssignFormUnit : Unknown unit type : %s",Name);
	SCENINF.NErrors++;
};
DLLEXPORT void SetPlayerName(byte Nat,char* ID){
	for(int i=0;i<8;i++)if(GSets.CGame.PL_INFO[i].ColorID==Nat){
		char cc[128];
		strcpy(cc,GetTextByID(ID));
		cc[31]=0;
		strcpy(GSets.CGame.PL_INFO[i].name,cc);
		return;
	};
};

extern int MaxPeaceTime;
extern int PeaceTimeStage;
DLLEXPORT
int GetMaxPeaceTime(){
	return MaxPeaceTime/60;
};
DLLEXPORT
int GetPeaceTimeLeft(){
	return PeaceTimeStage;
};
extern bool RecordMode;
extern byte PlayGameMode;
int PrevRand=-1;
extern word NPlayers;
DLLEXPORT
int GetRandomIndex(){
	if(PlayGameMode||RecordMode)return 0;
	if(NPlayers){
		int RR=0;
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB){
				RR+=(OB->RealX+OB->RealY)*int(OB->NNUM);
			};
		};
		addrand((RR+rando())&4095);
		RR+=rando();
		RR>>=3;
		return RR;
	}else{
		if(PrevRand==-1)PrevRand=(GetTickCount()/100)&32767;
		else PrevRand*=177;
	};
	return PrevRand;
};
int AI_Registers[8][32];
DLLEXPORT
void SetAIRegister(int Reg,int Val){
	if(Reg<32&&CurAINation<8)AI_Registers[CurAINation][Reg]=Val;
};
DLLEXPORT
int GetAIRegister(int Reg){
	if(Reg<32&&CurAINation<8)return AI_Registers[CurAINation][Reg];
	else return 0;
};
extern int WasInGold[8];
extern int WasInIron[8];
extern int WasInCoal[8];
extern int WasInStone[8];
DLLEXPORT
int GetExtraction(byte ResID){
	if(!AiIsRunNow)return 0;
	if(ResID==FoodID){
		return CCIT->FoodSpeed;
	}else
	if(ResID==TreeID){
		return CCIT->WoodSpeed;
	}else
	if(ResID==StoneID){
		return WasInStone[CNAT->NNUM];
	}else
	if(ResID==IronID){
		return WasInIron[CNAT->NNUM];
	}else
	if(ResID==CoalID){
		return WasInCoal[CNAT->NNUM];
	}else
	if(ResID==GoldID){
		return WasInGold[CNAT->NNUM];
	}else return 0;
};
DLLEXPORT
void SetDefenseState(byte State){
	if(AiIsRunNow){
		CCIT->DefenceStage=!!State;
	};
};
//-----------------------------------------------------------------------//
void DosToWin(char* Str);

bool ReadWinString(GFILE* F,char* STR,int Max){
	STR[0]=0;
	int cc=0;
	//int z1=0;
	int nn=0;
	while(!(cc==0x0A||cc==EOF)){
		cc=Ggetch(F);
		if(Max>2&&!(cc==0x0A||cc==EOF)){
			STR[nn]=cc;
			nn++;
			Max--;
		};
	};
	STR[nn]=0;
	return (cc!=EOF)||STR[0];
	//DosToWin(STR);
};
extern int CurPalette;
void ErrM(char* s,char* s1)
{
	char pal[128];
	sprintf(pal,"%d\\agew_1.pal",CurPalette);
	LoadPalette(pal);
	char cc2[200];
	sprintf(cc2,s,s1);
	MessageBox(hwnd,cc2,"LOADING FAILED...",MB_ICONWARNING|MB_OK);
	assert(false);
};
extern bool ProtectionMode;
void MissPack::LoadMissions(){
	ProtectionMode=1;
	GFILE* F=Gopen("Missions\\Missions.txt","r");
	ProtectionMode=0;
	if(F){
		char MissID[256];
		char MissName[256];
		char DLLPath[256];
		char MapName[256];
		char MissDesc[256];
		bool MissLoad=false;
		do{
			MissLoad=false;
			ReadWinString(F,MissID,256);
			ReadWinString(F,MissName,256);
			ReadWinString(F,DLLPath,256);
			ReadWinString(F,MapName,256);
			ReadWinString(F,MissDesc,256);
			if(MissID[0]&&MissName[0]&&DLLPath[0]&&MapName[0]&&MissDesc[0]){
				if(NMiss>=MaxMiss){
					MaxMiss+=32;
					MISS=(SingleMission*)realloc(MISS,MaxMiss*sizeof SingleMission);
				};
				SingleMission* SM=MISS+NMiss;
				NMiss++;
				SM->ID=znew(char,strlen(MissID)+1);
				strcpy(SM->ID,MissID);
				SM->Name=znew(char,strlen(MissName)+1);
				strcpy(SM->Name,MissName);
				SM->DLLPath=znew(char,strlen(DLLPath)+1);
				strcpy(SM->DLLPath,DLLPath);
				SM->MapName=znew(char,strlen(MapName)+1);
				strcpy(SM->MapName,MapName);
				SM->Description=znew(char,strlen(MissDesc)+1);
				strcpy(SM->Description,MissDesc);
				MissLoad=true;
				ReadWinString(F,MissDesc,256);
				int z=sscanf(MissDesc,"%d",&SM->NIntro);
				if(z!=1){
					ErrM("Invalid file Missiions\\Missions.txt");
				};
				SM->Intro=(char**)malloc(4*SM->NIntro);
				for(int j=0;j<SM->NIntro;j++){
					ReadWinString(F,MissDesc,256);
					SM->Intro[j]=znew(char,strlen(MissDesc)+1);
					strcpy(SM->Intro[j],MissDesc);
				};
			};
		}while(MissLoad);
		Gclose(F);
	}else{
		MessageBox(NULL,"Could not open Missions\\Missions.txt","Missions loading failed...",MB_TOPMOST);
		assert(0);
	};
	ProtectionMode=1;
	F=Gopen("Missions\\SingleMiss.txt","r");
	ProtectionMode=0;
	if(F){
		int z=Gscanf(F,"%d",&MSMiss);
		if(z!=1){
			ErrM("Invalid file Missions\\SingleMiss.txt");
			return;
		};
		char cc1[64];
		SingleMS=znew(int,MSMiss);
		for(int i=0;i<MSMiss;i++){
			Gscanf(F,"%s",cc1);
			SingleMS[i]=0;
			for(int j=0;j<NMiss;j++)if(!strcmp(MISS[j].ID,cc1)){
				SingleMS[i]=j;
				goto GG1;
			};
			ErrM("SingleMiss.txt: Unknown mission %s",cc1);
GG1:;
		};
	}else{
		MessageBox(NULL,"Could not open Missions\\SingleMiss.txt","Missions loading failed...",MB_TOPMOST);
		assert(0);
	};
	// --------
	ProtectionMode=1;
	F=Gopen("Missions\\MultiMiss.txt", "r");
	ProtectionMode=0;
	if(F)
	{
		int z = Gscanf(F, "%d", &RoSMiss);
		if(z != 1)
		{
			ErrM("Invalid file Missiions\\MultiMiss.txt");
			return;
		};
		char cc1[64];
		RoSMS = znew(int, RoSMiss);
		for(int i=0; i<RoSMiss; i++)
		{
			Gscanf(F, "%s", cc1);
			RoSMS[i] = 0;
			for(int j = 0; j < NMiss; j++) // ?? NMiss
			{
				if(!strcmp(MISS[j].ID, cc1)) // ?? MISS
				{
					RoSMS[i] = j;
					goto GG2;
				};
			}
			ErrM("MultiMiss.txt: Unknown mission %s", cc1);
			GG2:;
		};
	}
	else
	{
		MessageBox(NULL,"Could not open Missions\\MultiMiss.txt", "Missions loading failed...", MB_TOPMOST);
		assert(0);
	};/*
	if(F)
	{
		char MissID[256];
		char MissName[256];
		char DLLPath[256];
		char MapName[256];
		char MissDesc[256];
		bool MissLoad = false;
		do{
			MissLoad = false;
			ReadWinString(F,MissID,256);
			ReadWinString(F,MissName,256);
			ReadWinString(F,DLLPath,256);
			ReadWinString(F,MapName,256);
			ReadWinString(F,MissDesc,256);
			if(MissID[0] && MissName[0] && DLLPath[0] && MapName[0] && MissDesc[0])
			{
				if(NMiss >= MaxMiss)
				{
					MaxMiss+=32;
					RoSMULTIMISS = (SingleMission*)realloc(RoSMULTIMISS, MaxMiss * sizeof SingleMission);
				};
				SingleMission* SM = RoSMULTIMISS+NMiss;
				NMiss++;
				SM->ID=znew(char,strlen(MissID)+1);
				strcpy(SM->ID,MissID);
				SM->Name=znew(char,strlen(MissName)+1);
				strcpy(SM->Name,MissName);
				SM->DLLPath=znew(char,strlen(DLLPath)+1);
				strcpy(SM->DLLPath,DLLPath);
				SM->MapName=znew(char,strlen(MapName)+1);
				strcpy(SM->MapName,MapName);
				SM->Description=znew(char,strlen(MissDesc)+1);
				strcpy(SM->Description,MissDesc);
				MissLoad=true;
				ReadWinString(F,MissDesc,256);
				int z=sscanf(MissDesc,"%d",&SM->NIntro);
				if(z!=1) ErrM("Invalid file Missions\\MultiMissions.txt");
				SM->Intro=(char**)malloc(4*SM->NIntro);
				for(int j=0;j<SM->NIntro;j++)
				{
					ReadWinString(F,MissDesc,256);
					SM->Intro[j]=znew(char,strlen(MissDesc)+1);
					strcpy(SM->Intro[j],MissDesc);
				};
			};
		}
		while(MissLoad);
		Gclose(F);
	}
	else
	{
		MessageBox(NULL,"Could not open Missions\\Missions.txt","Missions loading failed...",MB_TOPMOST);
		assert(0);
	};*/
	// -------
};
MissPack::MissPack(){
	memset(this,0,sizeof MissPack);
	CurrentMission=-1;	
};
MissPack::~MissPack(){
	for(int i=0;i<NMiss;i++){
		free(MISS[i].Description);
		free(MISS[i].DLLPath);
		free(MISS[i].ID);
		free(MISS[i].MapName);
		free(MISS[i].Name);
		for(int j=0;j<MISS[i].NIntro;j++)free(MISS[i].Intro[j]);
		if(MISS[i].Intro)free(MISS[i].Intro);
	};
	free(MISS);
	free(SingleMS);
};
extern MissPack MISSLIST;
extern SinBattles SBattles;
extern CampaginPack CAMPAGINS;
void InvCamp(){
	ErrM("Invalid file CAMPAIGNS.TXT");
};
void SavePlayerData(char* Name);
CampaginPack::CampaginPack(){
}
void CampaginPack::Init(){
	GFILE* F=Gopen("Missions\\Campaigns.txt","r");
	if(!F)F=Gopen("Missions\\Campagins.txt","r");
	if(F){
		char cc[256];
		ReadWinString(F,cc,255);
		int z=sscanf(cc,"%d",&NCamp);
		if(z!=1)InvCamp();
		SCamp=new SingleCampagin[NCamp];
		char CampMessage[256];
		char CampText[256];
		char CampBmp[256];
		for(int i=0;i<NCamp;i++){ 
			ReadWinString(F,CampMessage,255);
			ReadWinString(F,CampText,255);
			//adWinString(F,CampPostText,255);			
			ReadWinString(F,CampBmp,255);
			ReadWinString(F,cc,255);
			if(cc[0]&&CampMessage[0]&&CampText[0]&&CampBmp[0]){
				SCamp[i].CampMessage=znew(char,strlen(CampMessage)+1);
				strcpy(SCamp[i].CampMessage,CampMessage);
				SCamp[i].CampText=znew(char,strlen(CampText)+1);
				strcpy(SCamp[i].CampText,CampText);
				SCamp[i].CampBmp=znew(char,strlen(CampBmp)+1);
				strcpy(SCamp[i].CampBmp,CampBmp);
				z=sscanf(cc,"%d",&SCamp[i].NMiss);
				if(z!=1)InvCamp();
				SCamp[i].Miss=znew(int,SCamp[i].NMiss);
				SCamp[i].OpenIndex=znew(DWORD,SCamp[i].NMiss);
				DWORD op0,op1,op2,op3;
				char ONOFF[32];
				char MISSID[64];
				for(int q=0;q<SCamp[i].NMiss;q++){
					int miss=-1;
					ReadWinString(F,cc,255);
					z=sscanf(cc,"%s%s%d%d%d",MISSID,ONOFF,&op1,&op2,&op3);
					if(z==5){
						if(!(strcmp(ONOFF,"ON")&&strcmp(ONOFF,"on")))op0=1;
						else op0=0;
						for(int p=0;p<MISSLIST.NMiss;p++)if(!strcmp(MISSID,MISSLIST.MISS[p].ID))miss=p;
						if(miss==-1){
							sprintf(cc,"CAMPAGINS.TXT: Uncnown mission %s",MISSID);
							ErrM(cc);
						};
						SCamp[i].OpenIndex[q]=op0+(op1<<8)+(op2<<16)+(op3<<24);
						SCamp[i].Miss[q]=miss;
					}else InvCamp();
				};
			}else InvCamp();
		};
		Gclose(F);
		SavePlayerData("standart player info");
	}else InvCamp();
};
CampaginPack::~CampaginPack(){
	for(int i=0;i<NCamp;i++){
		free(SCamp[i].CampBmp);
		free(SCamp[i].CampMessage);
		free(SCamp[i].CampText);
		free(SCamp[i].Miss);
		free(SCamp[i].OpenIndex);
	};
	if(SCamp)delete[](SCamp);
};
void LoadAIFromDLL(byte Nat,char* Name){
	Nat=AssignTBL[Nat];
	Nation* NT=NATIONS+Nat;
	AiIsRunNow=true;
	CNAT=NATIONS+Nat;
	CCIT=CNAT->CITY;
	CurAINation=Nat;
#ifdef STARFORCE
	if(PTR_AI&&((DWORD*)PTR_AI)[0]==0x4351A234)NT->hLibAI=LoadLibrary(Name);
#else
	NT->hLibAI=LoadLibrary(Name);
#endif
	AiIsRunNow=false;
	if(NT->hLibAI){
		NT->ProcessAIinDLL=(VoidProc*)GetProcAddress(NT->hLibAI,"ProcessAI");
		if(!NT->ProcessAIinDLL){
			char cc[128];
			sprintf(cc,"%s : Could not load <void ProcessAI()>",Name);
			MessageBox(NULL,cc,"AI loadind from DLL",MB_TOPMOST);
			assert(0);
		}else{
			VoidProc* INITAI=(VoidProc*)GetProcAddress(NT->hLibAI,"InitAI");
			if(!INITAI){
				char cc[128];
				sprintf(cc,"%s : Could not load <void InitAI()>",Name);
				MessageBox(NULL,cc,"AI loadind from DLL",MB_TOPMOST);
				assert(0);
			};
			AiIsRunNow=true;
			INITAI();
			AiIsRunNow=false;
			NT->DLLName=Name;
		};
	}else{
#ifndef STARFORCE
		char cc[128];
		sprintf(cc,"Could not load %s",Name);
		MessageBox(NULL,cc,"AI loadind from DLL",MB_TOPMOST);
		assert(0);
#endif
	};
};

int SBDScore[128];	// single battles difficulty score
int SBTScore[128];	// single battles tactical score

//extern int NSinBattle;
extern int GetNSBattle();

extern char PlName[64];
void SavePlayerData(char* Name){
	char cc[200];
	sprintf(cc,"Players\\%s.txt",Name);
	GFILE* F=Gopen(cc,"w");
	if(F){
		Gprintf(F,"%d ",CAMPAGINS.NCamp);
		for(int i=0;i<CAMPAGINS.NCamp;i++){
			Gprintf(F,"%d ",CAMPAGINS.SCamp[i].NMiss);
			for(int j=0;j<CAMPAGINS.SCamp[i].NMiss;j++){
				Gprintf(F,"%d ",CAMPAGINS.SCamp[i].OpenIndex[j]);
			};
		};
		
		// Save single battle score
		if(!strstr(Name,"standart player info")){
//			Gprintf(F,"%d ",NSinBattle);
			for(i=0;i<GetNSBattle();i++){
				Gprintf(F,"%d ",SBDScore[i]);
				Gprintf(F,"%d ",SBTScore[i]);
			}
		}

		Gclose(F);
	};
};
CEXPORT
void SavePlayerData(){
	SavePlayerData(PlName);
};
#ifdef MAKE_PTC
bool PLOAD=0;

void LoadPDATA(char* Name){
	PLOAD=0;
	char cc[200];
	sprintf(cc,"Players\\%s.txt",Name);
	GFILE* F=Gopen(cc,"r");
	if(F){
		int x1,x2;
		Gscanf(F,"%d ",&x1);
		if(x1!=CAMPAGINS.NCamp){
			Gclose(F);
			return;
		};
		for(int i=0;i<x1;i++){
			Gscanf(F,"%d ",&x2);
			if(x2!=CAMPAGINS.SCamp[i].NMiss){
				Gclose(F);
				return;
			};
			for(int j=0;j<x2;j++){
				Gscanf(F,"%d ",&CAMPAGINS.SCamp[i].OpenIndex[j]);
			};
		};
		
		// Load single battle score
//		if(!Gscanf(F,"%d ",&NSinBattle)||NSinBattle<1||NSinBattle>GetNSBattle())
//			NSinBattle=1;
		int pts;
		for(i=0;i<GetNSBattle();i++){
			if(Gscanf(F,"%d ",&pts)) SBDScore[i]=pts;
				else SBDScore[i]=0;
			if(Gscanf(F,"%d ",&pts)) SBTScore[i]=pts;
				else SBTScore[i]=0;
		}
		
		Gclose(F);
		PLOAD=1;
	};
	
};
CEXPORT
void LoadPlayerData(){
	LoadPDATA(PlName);
	if(!PLOAD)LoadPDATA("standart player info");
};
#endif
WarPack WARS;
void InvBatt(){
	ErrM("Invalid file: History_battl\\Battles.txt");
};
void InvWar(){
	ErrM("Invalid file: History_battl\\Wars.txt");
};
void NLine(GFILE* f);
WarPack::WarPack(){
	
};
WarPack::~WarPack(){
	
};
DLLEXPORT
void MissErrorMessage(char* Header,char* Message){
	MessageBox(hwnd,Message,Header,0);
};
bool RUNMAPEDITOR=0;
bool RUNUSERMISSION=0;
char USERMISSPATH[128];
DLLEXPORT
void RunMapeditor(char* path){
	RUNMAPEDITOR=1;
	strcpy(USERMISSPATH,path);
};
DLLEXPORT
void RunUserMission(char* path){
	RUNUSERMISSION=1;
	strcpy(USERMISSPATH,path);
};
//------------------------NEW! FUNCTIONS----------------------//
/*
ReportFn* ORDLIST[OrdCount]={
	&BuildObjLink,
	&ProduceObjLink,
	&AttackObjLink,
	&TakeResLink,
	&PerformUpgradeLink,
	&GoToMineLink,
	&LeaveMineLink,
	&TakeResourceFromSpriteLink,
	&NewMonsterSendToLink,
	&DeleteBlockLink,
	&SetUnlimitedLink,
	&ClearUnlimitedLink,
	&WaterNewMonsterSendToLink,
	&AbsorbObjectLink,
	&LeaveShipLink,
	&BuildWallLink,
	&WaterAttackLink,
	&LeaveTransportLink,
	&GoToTransportLink,
	&FishingLink,
	&CreateGatesLink,
	&NewMonsterSmartSendToLink,
	&AI_AttackPointLink,
	&GrenaderSupermanLink,
	&GotoFinePositionLink,
	&NewMonsterPreciseSendToLink,
	&RotUnitLink,
	&PatrolLink,
	&RotateShipAndDieLink,
	&SearchArmyLink,
	&NewAttackPointLink,
	&MakeOneShotLink
};
*/
int GetOrderKind(ReportFn* RF);
struct UnitExCaps{
	int Direction;
	int DestX;
	int DestY;
	int Type;
	char* UnitID;
	int NI;
	int StandTime;
	int OrderType;
};
DLLEXPORT
byte GetUnitNMASK(int Index){
	if(Index<0||Index>=0xFFFF)return 0;
	OneObject* OB=Group[Index];
	if(OB)return OB->NMask;
	else return 0;
};
DLLEXPORT
void SetNMASK(int Index,byte NMASK){
	if(Index<0||Index>=0xFFFF)return;
	OneObject* OB=Group[Index];
	if(OB)OB->NMask=NMASK;
};
DLLEXPORT void SetGroupNMASK(GAMEOBJ* Grp,byte NMASK){
	if(Grp->Type=='UNIT'){
		UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];
		word* IDS=UG->IDS;
		word* SNS=UG->SNS;
		int N=UG->N;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i])OB->NMask=NMASK;
			};
		};
	};
};
DLLEXPORT
int GetUnitIcon(int Type){
	if(Type<NATIONS->NMon){
		return NATIONS->Mon[Type]->newMons->IconID;
	}else return -1;
};
DLLEXPORT
int GetAmountOfGroups(){
	return AGroups.GetAmount();
};
DLLEXPORT
int GetAmountOfZones(){
	return AZones.GetAmount();
};
DLLEXPORT
char* GetZoneName(int Index){
	if(Index<AZones.GetAmount())return AZones[Index]->Name.pchar();
	else return NULL;
};
DLLEXPORT
int	  GetZoneXbyName(char* name){
	if( name ) {//grey
		for (int i=0; i<AZones.GetAmountOfElements(); i++){
			if (strcmp(AZones[i]->Name.pchar(),name)==0) {
				return AZones[i]->x;
			}
		}
	}
	return 0;
};
DLLEXPORT
int	  GetZoneYbyName(char* name){
	if( name ) {//grey
		for (int i=0; i<AZones.GetAmountOfElements(); i++){
			if (strcmp(AZones[i]->Name.pchar(),name)==0) {
				return AZones[i]->y;
			}
		}
	}
	return 0;
};
DLLEXPORT
int	  GetZoneRbyName(char* name){
	if( name ) {//grey
		for (int i=0; i<AZones.GetAmountOfElements(); i++){
			if (strcmp(AZones[i]->Name.pchar(),name)==0) {
				return AZones[i]->R;
			}
		}
	}
	return 0;
};
DLLEXPORT
int	  GetZoneX(int Index){
	if (Index<AZones.GetAmount())	return	AZones[Index]->x;
	else return	0;
};
DLLEXPORT
int	  GetZoneY(int Index){
	if (Index<AZones.GetAmount())	return	AZones[Index]->y;
	else				return	0;
};
DLLEXPORT
char* GetGroupName(int Index){
	if(Index<AGroups.GetAmount())return AGroups[Index]->Name.pchar();
	else return NULL;
};
DLLEXPORT
char* GetUnitName(int Type){
	if(Type<NATIONS->NMon){
		return NATIONS->Mon[Type]->Message;
	}else return NULL;
};
DLLEXPORT
char* GetUnitID(int Type){
	if(Type<NATIONS->NMon){
		return NATIONS->Mon[Type]->MonsterID;
	}else return NULL;
};
DLLEXPORT 
bool GetUnitExCaps(int Index,UnitExCaps* CAPS,bool NeedOrderType){
	if(Index>=0&&Index<MAXOBJECT){
		OneObject* OB=Group[Index];
		if(OB&&((!OB->Sdoxlo)||OB->Hidden)){
			CAPS->DestX=OB->DestX>>4;
			CAPS->DestY=OB->DestY>>4;
			CAPS->NI=OB->NNUM;
			CAPS->Direction=OB->RealDir;
			CAPS->Type=OB->NIndex;
			CAPS->UnitID=OB->Ref.General->MonsterID;
			CAPS->StandTime=OB->StandTime;
			if(NeedOrderType){
				if(OB->LocalOrder){
					CAPS->OrderType=GetOrderKind(OB->LocalOrder->DoLink);
				}else CAPS->OrderType=-1;
			}else CAPS->OrderType=-1;
			return 1;
		}else return 0;
	}else return false;
};
DLLEXPORT
int InsertUnitToGroup(GAMEOBJ* Src,GAMEOBJ* Dst,int Index){
	if((!Dst)||Dst->Type!='UNIT')return -1;
	if(Src){
		if(Src->Type!='UNIT')return -1;
		if(Src->Index<SCENINF.UGRP.GetAmount()){
			UnitsGroup* UG=&SCENINF.UGRP[Src->Index];
			if(Index<UG->N){
				int Id=UG->IDS[Index];
				int Sn=UG->SNS[Index];
				UnitsGroup* DG=&SCENINF.UGRP[Dst->Index];
				
				DG->Allocate(DG->N+1);
				DG->IDS[DG->N]=Id;
				DG->SNS[DG->N]=Sn;
				DG->N++;
				
				return DG->N-1;
			};
		};
	}else{
		if(Index>=0&&Index<=MAXOBJECT){
			OneObject* OB=Group[Index];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)){
				UnitsGroup* DG=&SCENINF.UGRP[Dst->Index];
				DG->Allocate(DG->N+1);
				DG->IDS[DG->N]=OB->Index;
				DG->SNS[DG->N]=OB->Serial;
				DG->N++;
				return DG->N-1;
			};
		};
	};
	return -1;
};
DLLEXPORT
void RemoveUnitFromGroup(GAMEOBJ* Src,int Index){
	if(Src->Type!='UNIT')return;
	if(Src->Index>=SCENINF.UGRP.GetAmount())return;
	UnitsGroup* SG=&SCENINF.UGRP[Src->Index];
	
	if(Index<SG->N){
		if(Index<SG->N-1){
			memcpy(SG->IDS+Index,SG->IDS+Index+1,(SG->N-Index-1)<<1);
			memcpy(SG->SNS+Index,SG->SNS+Index+1,(SG->N-Index-1)<<1);
		};
		SG->N--;
	};
	
};
DLLEXPORT
bool GetZoneCoor(GAMEOBJ* Zone,int* x,int* y){
	if(!Zone){
		*x=0;
		*y=0;
		return false;
	};
	if(Zone->Type=='ZONE'&&Zone->Index<SCENINF.ZGRP.GetAmount()){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		*x=AZ->x;
		*y=AZ->y;
		return true;
	}else
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		*x=Zone->Index;
		*y=Zone->Serial;
		return true;
	}else return false;
};
DLLEXPORT
bool GetZoneR(GAMEOBJ* Zone,int* R){
	if(Zone->Type=='ZONE'&&Zone->Index<SCENINF.ZGRP.GetAmount()){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		*R=AZ->R;
		return true;
	}else
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		*R=Zone->Type&0xFFFFFF;
		return true;
	}else return false;
};
void RM_Load(char* Name,int x,int y);
extern bool ImmVis;
DLLEXPORT
void PastePiece(GAMEOBJ* Zone,char* Name){
	int x,y;
	if(GetZoneCoor(Zone,&x,&y)){
		ImmVis=true;
		RM_Load(Name,x,y);
		ImmVis=false;
	};
};
void AddUnitToSelected(byte NI,OneObject* OB);
DLLEXPORT void SelectBuildingsInZone(byte NI,GAMEOBJ* Zone,GAMEOBJ* UTP,bool Add){
	if(!Add)ClearSelection(NI);
	int x,y,R;
	if(Zone->Type=='ZONE'&&Zone->Index<SCENINF.ZGRP.GetAmount()){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		x=AZ->x;
		y=AZ->y;
		R=AZ->R;
	}else
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		x=Zone->Index;
		y=Zone->Serial;
		R=Zone->Type&0x00FFFFFF;
	}else return;
	if(UTP){
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->NewBuilding&&OB->NIndex==UTP->Index&&OB->NNUM==NI&&!OB->Sdoxlo){
				int r=Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
				if(r<R)AddUnitToSelected(NI,OB);
			};
		};
	}else{
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->NewBuilding&&OB->NNUM==NI&&!OB->Sdoxlo){
				int r=Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
				if(r<R)AddUnitToSelected(NI,OB);
			};
		};
	};
};
DLLEXPORT
void InviteUnitIntoBuilding(int BIndex,int UIndex,byte Type){
	if(BIndex<0||BIndex>=0xFFFF||UIndex<0||UIndex>=0xFFFF)return;
	OneObject* BOB=Group[BIndex];
	OneObject* UOB=Group[UIndex];
	if(BOB&&UOB&&BOB->NewBuilding&&!UOB->NewBuilding){
		UOB->GoToMine(BIndex,255,Type);
	};
};
DLLEXPORT
void LeaveBuilding(int BIndex,int Type){
	if(BIndex<0||BIndex>=0xFFFF)return;
	OneObject* BOB=Group[BIndex];
	if(BOB&&BOB->NInside){
		BOB->LeaveMine(Type);
	};
};
DLLEXPORT
int GetInsideList(int BIndex,word** List){
	if(BIndex<0||BIndex>=0xFFFF)return 0;
	OneObject* BOB=Group[BIndex];
	if(BOB&&BOB->NInside){
		*List=BOB->Inside;
		return BOB->NInside;
	}else{ 
		*List=NULL;
		return 0;
	};
};
DLLEXPORT
void AddUnitToSel(int Index){
	if(Index>=0&&Index<0xFFFF){
		OneObject* OB=Group[Index];
		if(OB&&!OB->Sdoxlo)AddUnitToSelected(OB->NNUM,OB);
	};
};
CEXPORT int GetAmountBrigsInZone(byte NI,GAMEOBJ* Zone){
	int Amount=0;
	int x=0;
	int y=0;
	int RR=0;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		x=int(Zone->Index);
		y=int(Zone->Serial);
		RR=Zone->Type&0xFFFFFF;
	}else
	if(Zone->Type=='ZONE'){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		x=AZ->x;
		y=AZ->y;
		RR=AZ->R;
	}else{
		IntErr("GetAmountBrigsInZone : Invalid <Zone>");
		SCENINF.NErrors++;
		return Amount;
	};
	if(NI<8){
		int nb=CITY[NI].NBrigs;
		for(int j=0;j<nb;j++){
			Brigade* BR=CITY[NI].Brigs+j;
			if(BR->Enabled&&BR->WarType){ //&&BR->NMemb>40
				int xc,yc;
				if(BR->GetCenter(&xc,&yc)){
					int d=Norma(x-xc,y-yc);
					if(d<RR){
						Amount++;
					}
				}
			}
		}
	}
	return Amount;
}
DLLEXPORT
void AddUnitsInZoneToGroup(GAMEOBJ* Zone,GAMEOBJ* GROUP,byte NI,int Usage){
	int x=0;
	int y=0;
	int RR=0;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		x=int(Zone->Index);
		y=int(Zone->Serial);
		RR=Zone->Type&0xFFFFFF;
	}else
	if(Zone->Type=='ZONE'){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		x=AZ->x;
		y=AZ->y;
		RR=AZ->R;
	}else{
		IntErr("AddUnitsInZoneToGroup : Invalid <Zone>");
		SCENINF.NErrors++;
		return;
	};	
	int R=(RR>>7)+2;
	int cx=x>>7;
	int cy=y>>7;

	int zx=x<<4;
	int zy=y<<4;
	int Rz=RR<<4;
	if(GROUP->Type!='UNIT'){
		IntErr("AddUnitsInZoneToGroup : Invalid <Group>");
		SCENINF.NErrors++;
		return;
	};
	word* UIDS=NULL;
	word* SNS=NULL;
	int NUIDS=0;
	int MaxUIDS=0;
	int mxx=msx>>2;
	int myy=msy>>2;
	for(int r=0;r<R;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int Np=Rarr[r].N;
		for(int p=0;p<Np;p++){
			int xp=cx+xi[p];
			int yp=cy+yi[p];
			if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
				int cell=xp+1+((yp+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(NI==0xFF){
								if(OB&&(!OB->Sdoxlo)&&Norma(OB->RealX-zx,OB->RealY-zy)<Rz){
									if((Usage==0||Usage>0&&OB->newMons->Usage==Usage||Usage==-1&&OB->BrigadeID!=0xFFFF)
										&&OB->NNUM!=7){
										if(NUIDS>=MaxUIDS){
											MaxUIDS+=128;
											UIDS=(word*)realloc(UIDS,MaxUIDS<<1);
											SNS=(word*)realloc(SNS,MaxUIDS<<1);
										};
										UIDS[NUIDS]=MID;
										SNS[NUIDS]=OB->Serial;
										NUIDS++;
									};
								};
							}else{
								if(OB&&OB->NNUM==NI&&(!OB->Sdoxlo)&&Norma(OB->RealX-zx,OB->RealY-zy)<Rz){
									if(Usage==0 || Usage>0&&OB->newMons->Usage==Usage ||
									   Usage==-1&&OB->BrigadeID!=0xFFFF){
										if(NUIDS>=MaxUIDS){
											MaxUIDS+=128;
											UIDS=(word*)realloc(UIDS,MaxUIDS<<1);
											SNS=(word*)realloc(SNS,MaxUIDS<<1);
										};
										UIDS[NUIDS]=MID;
										SNS[NUIDS]=OB->Serial;
										NUIDS++;
									};
								};
							};
						};
					};
				};
			};
		};
	};
	
	if(SCENINF.UGRP[GROUP->Index].IDS){
		free(SCENINF.UGRP[GROUP->Index].IDS);
		free(SCENINF.UGRP[GROUP->Index].SNS);
	};
	SCENINF.UGRP[GROUP->Index].IDS=UIDS;
	SCENINF.UGRP[GROUP->Index].SNS=SNS;
	SCENINF.UGRP[GROUP->Index].N=NUIDS;
	SCENINF.UGRP[GROUP->Index].Max=NUIDS;
	
};
DLLEXPORT
void DeleteUnitFromGroup(GAMEOBJ* Grp, int Index){
	if(Grp->Type!='UNIT')return;
	if(Grp->Index>=SCENINF.UGRP.GetAmount())return;
	UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
	
	if(Index<GRP->N){
		GRP->IDS[Index] = 0xFFFF;
	};
	
}

DLLEXPORT int CleanGroup(GAMEOBJ* Grp){
	try{
	if(!Grp)return 0;
	if(Grp->Type!='UNIT')return 0;
	if(Grp->Index>=SCENINF.UGRP.GetAmount())return 0;
	UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
	int n=0;
	int N=GRP->N;
	
	
	word* IDS=GRP->IDS;
	word* SNS=GRP->SNS;
	for(int i=0; i<N; i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&((!OB->Sdoxlo)||OB->Hidden)){
				IDS[n] = IDS[i];
				SNS[n] = SNS[i];
				n++;
			};
		}
	}
	
	
	return GRP->N=n;
	}catch(...){
		return 0;
	};
}
int CleanGroup(UnitsGroup* GRP){
	int n=0;
	int N=GRP->N;
	word* IDS=GRP->IDS;
	word* SNS=GRP->SNS;
	
	
	for(int i=0; i<N; i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&((!OB->Sdoxlo)||OB->Hidden)){
				IDS[n] = IDS[i];
				SNS[n] = SNS[i];
				n++;
			};
		}
	}
	
	
	return GRP->N=n;
}
int RemoveNUnitsFast(UnitsGroup* DST,UnitsGroup* SRC,int N,int NIndex){
	if(N<=0) return 0;
	DST->Allocate(DST->N+N);
	int N0=N;

	word* USRC=SRC->IDS;
	word* SSRC=SRC->SNS;
	int NSRC=SRC->N;

	word* UDST=DST->IDS;
	word* SDST=DST->SNS;
	int NDST=DST->N;

	if(NIndex==0xFFFF){
		for(int i=NSRC-1;i>=0&&N;i--){
			word MID=USRC[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&((!OB->Sdoxlo)||OB->Hidden)&&OB->Serial==SSRC[i]){
					UDST[NDST]=MID;
					SDST[NDST]=OB->Serial;
					NDST++;
					//NSRC--;
					USRC[i]=0xFFFF;
					N--;
				};
			};
		};
	}else
		if(NIndex>=30000){
			NIndex-=30000;
			for(int i=NSRC-1;i>=0&&N;i--){
				word MID=USRC[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&((!OB->Sdoxlo)||OB->Hidden)&&OB->Serial==SSRC[i]&&OB->newMons->Usage!=NIndex){
						UDST[NDST]=MID;
						SDST[NDST]=OB->Serial;
						NDST++;
						//NSRC--;
						USRC[i]=0xFFFF;
						N--;
					};
				};
			};
		}else
			if(NIndex>=20000){
				NIndex-=20000;
				for(int i=NSRC-1;i>=0&&N;i--){
					word MID=USRC[i];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB&&((!OB->Sdoxlo)||OB->Hidden)&&OB->Serial==SSRC[i]&&OB->newMons->Usage==NIndex){
							UDST[NDST]=MID;
							SDST[NDST]=OB->Serial;
							NDST++;
							//NSRC--;
							USRC[i]=0xFFFF;
							N--;
						};
					};
				};
			}else
				if(NIndex>=10000){
					NIndex-=10000;
					for(int i=NSRC-1;i>=0&&N;i--){
						word MID=USRC[i];
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&((!OB->Sdoxlo)||OB->Hidden)&&OB->Serial==SSRC[i]&&OB->NIndex!=NIndex){
								UDST[NDST]=MID;
								SDST[NDST]=OB->Serial;
								NDST++;
								//NSRC--;
								USRC[i]=0xFFFF;
								N--;
							};
						};
					};
				}else{
					for(int i=NSRC-1;i>=0&&N;i--){
						word MID=USRC[i];
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&((!OB->Sdoxlo)||OB->Hidden)&&OB->Serial==SSRC[i]&&OB->NIndex==NIndex){
								UDST[NDST]=MID;
								SDST[NDST]=OB->Serial;
								NDST++;
								//NSRC--;
								USRC[i]=0xFFFF;
								N--;
							};
						};
					};
				};
				//SRC->N=NSRC;
				DST->N=NDST;
				return N0-N;
};
DLLEXPORT int RemoveNUnits(GAMEOBJ* Dst,GAMEOBJ* Src,int N,int NIndex){
	int GMAX=SCENINF.UGRP.GetAmount();
	if(Src->Type=='UNIT'&&Dst->Type=='UNIT'&&Src->Index<GMAX&&Dst->Index<GMAX){
		UnitsGroup* DST=&SCENINF.UGRP[Dst->Index];
		UnitsGroup* SRC=&SCENINF.UGRP[Src->Index];
		return RemoveNUnitsFast(DST,SRC,N,NIndex);
	};
	return 0;
};
DLLEXPORT
void EraseGroup(GAMEOBJ* Grp){
	if(Grp->Type=='UNIT'){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		GRP->N=0;
	};
};
DLLEXPORT int CollectUnits(GAMEOBJ* Dst,GAMEOBJ* Src,int N,int NIndex){
	int GMAX=SCENINF.UGRP.GetAmount();
	if(Src->Type=='UNIT'&&Dst->Type=='UNIT'&&Src->Index<GMAX&&Dst->Index<GMAX){
		UnitsGroup* DST=&SCENINF.UGRP[Dst->Index];
		UnitsGroup* SRC=&SCENINF.UGRP[Src->Index];
		if(DST->N<N){
			RemoveNUnitsFast(DST,SRC,N-DST->N,NIndex);
		}/*else
		if(DST->N>N){
			RemoveNUnitsFast(SRC,DST,DST->N-N,NIndex);
		};*/
		return DST->N;
	};
	return 0;
};
DLLEXPORT void DeleteHiddenUnits(GAMEOBJ* Grp){
	if(Grp->Type!='UNIT' || Grp->Index>=SCENINF.UGRP.GetAmount()) return;
	UnitsGroup* UGRP=&SCENINF.UGRP[Grp->Index];
	
	int N = UGRP->N;
	word* IDS = UGRP->IDS;
	word* SNS = UGRP->SNS;
	for(int i=0; i<N; i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&OB->Hidden){
				IDS[i] = 0xFFFF;
			};
		};
	};
	
};
void PanicUnitLink(OneObject* OBJ);
DLLEXPORT
bool DetectPanic(int Index){
	if(Index>=0&&Index<0xFFFF){
		OneObject* OB=Group[Index];
		if(OB&&OB->UnlimitedMotion){
			Order1* OR1=OB->LocalOrder;
			while(OR1){
				if(OR1->DoLink==&PanicUnitLink)return true;
				OR1=OR1->NextOrder;
			};
		};
	};
	return false;
};
DLLEXPORT
int RemovePanicUnits(GAMEOBJ* Src,GAMEOBJ* Dst){
	int GMAX=SCENINF.UGRP.GetAmount();
	int NR=0;
	if(Src->Type=='UNIT'&&Dst->Type=='UNIT'&&Src->Index<GMAX&&Dst->Index<GMAX){
		UnitsGroup* DST=&SCENINF.UGRP[Dst->Index];
		UnitsGroup* SRC=&SCENINF.UGRP[Src->Index];
		
		
		for(int i=0;i<SRC->N;i++){
			int idx=SRC->IDS[i];
			if(DetectPanic(idx)){
				SRC->IDS[i]=0xFFFF;
				DST->Allocate(DST->N+1);
				DST->IDS[DST->N]=idx;
				DST->SNS[DST->N]=SRC->SNS[i];
				SRC->SNS[i]=0xFFFF;
				DST->N++;
				NR++;
			};
		};
		
		
	};
	return NR;
};
DLLEXPORT
bool DetectHiddenUnit(int Index){
	if(Index>=0&&Index<0xFFFF){
		OneObject* OB=Group[Index];
		return (OB&&OB->Hidden);
	};
	return false;
};
DLLEXPORT
byte GetUnitCategory(int Index){
	if(Index>=0&&Index<0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)return OB->newMons->Category;
	};
	return 0;
};
DLLEXPORT
byte GetUnitTypeCategory(int NIndex){
	if(NIndex>=0&&NIndex<NATIONS->NMon){
		return NATIONS->Mon[NIndex]->newMons->Category;
	};
	return 0;
};
void UnitsGroup::AddNewUnit(OneObject* OB){
	
	
	word ID=OB->Index;
	for(int i=0;i<N;i++)if(IDS[i]==ID)return;
	Allocate(N+1);
	IDS[N]=ID;
	SNS[N]=OB->Serial;
	N++;
	
	
};
void UnitsGroup::RemoveUnit(OneObject* OB){
	
	
	word ID=OB->Index;
	word SN=OB->Serial;
	for(int i=0;i<N;i++)if(IDS[i]==ID&&SNS[i]==SN){
		if(i<N-1){
			memcpy(IDS+i,IDS+i+1,(N-i-1)<<1);
			memcpy(SNS+i,SNS+i+1,(N-i-1)<<1);
		};
		i--;
		N--;
	};
	
	
};
//------------------------ADDITIONAL GROUPS INTERFACE------------------//
bool DetectArcher(GeneralObject* GO){
	NewMonster* NM=GO->newMons;
	return NM->WeaponKind[1]==8 || NM->WeaponKind[2]==8;
};
bool DetectShortRangeUnit(GeneralObject* GO){
	NewMonster* NM=GO->newMons;
	return NM->DamWeap[1]==NULL&&NM->MaxDamage[0];
};
bool DetectShootingUnit(GeneralObject* GO){
	NewMonster* NM=GO->newMons;
	return NM->WeaponKind[1]==4||NM->WeaponKind[1]==9;
};

bool DetectTamagavkUnit(GeneralObject* GO){
	NewMonster* NM=GO->newMons;
	return NM->WeaponKind[1]==11||NM->WeaponKind[1]==12;
};

CEXPORT
bool DetArcher(word NIndex){
	if(NIndex<NATIONS->NMon) return DetectArcher(NATIONS->Mon[NIndex]);
	return false;
}

CEXPORT
bool DetShoot(word NIndex){
	if(NIndex<NATIONS->NMon) return DetectShootingUnit(NATIONS->Mon[NIndex]);
	return false;
}

CEXPORT
bool DetPeasant(word NIndex){
	if(NIndex<NATIONS->NMon) return NATIONS->Mon[NIndex]->newMons->Peasant;
	return false;
}

//0-archer
//1-short range unit
//2-shooter
//3-peasant
//4-hidden
//5-tamagavshik & kopiemet
DLLEXPORT
void FilterUnitsByCategory(GAMEOBJ* G_Src,GAMEOBJ* G_Dst,int Ctg){
	if(G_Src->Type=='UNIT'&&G_Dst->Type=='UNIT'){
		UnitsGroup* SRC=&SCENINF.UGRP[G_Src->Index];
		UnitsGroup* DST=&SCENINF.UGRP[G_Dst->Index];
		word* IDS=SRC->IDS;
		word* SNS=SRC->SNS;
		
		
		int N=SRC->N;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]){
					bool ok=false;
					switch(Ctg){
					case 0://archer
						ok=DetectArcher(OB->Ref.General);
						break;
					case 1://short range unit
						ok=DetectShortRangeUnit(OB->Ref.General);
						break;
					case 2://shooter
						ok=DetectShootingUnit(OB->Ref.General);
						break;
					case 3:
						ok=OB->newMons->Peasant;
						break;
					case 4:
						ok=!OB->Hidden;
						break;
					case 5:
						ok=DetectTamagavkUnit(OB->Ref.General);
						break;
					};
					if(ok){
						DST->Allocate(DST->N+1);
						DST->IDS[DST->N]=SRC->IDS[i];
						DST->SNS[DST->N]=SRC->SNS[i];
						DST->N++;
						if(i<SRC->N-1){
							memcpy(SRC->IDS+i,SRC->IDS+i+1,(SRC->N-i-1)<<1);
							memcpy(SRC->SNS+i,SRC->SNS+i+1,(SRC->N-i-1)<<1);
						};
						SRC->N--;
						N--;
						i--;
					};
				};
			};
		};
		
		
	};
};
DLLEXPORT
bool RemoveNearestNotBusyUnit(GAMEOBJ* G_Src,GAMEOBJ* G_Dst,int x,int y,int MaxR){
	if(G_Src->Type=='UNIT'&&G_Dst->Type=='UNIT'){
		UnitsGroup* SRC=&SCENINF.UGRP[G_Src->Index];
		UnitsGroup* DST=&SCENINF.UGRP[G_Dst->Index];
		word* IDS=SRC->IDS;
		word* SNS=SRC->SNS;
		
		
		int N=SRC->N;
		int minR=MaxR;
		int ID=-1;
		int idx;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&!(OB->Sdoxlo||OB->LocalOrder)){
					int r=Norma(x-(OB->RealX>>4),y-(OB->RealY>>4));
					if(r<minR){
						minR=r;
						ID=MID;
						idx=i;
					};
				};
			};
		};
		if(ID!=-1){
			if(idx<N-1){
				memcpy(IDS+idx,IDS+idx+1,(N-idx-1)<<1);
				memcpy(SNS+idx,SNS+idx+1,(N-idx-1)<<1);
			};
			SRC->N--;
			DST->AddNewUnit(Group[ID]);
			return true;
		};
		
		
	};
	return false;
};

DLLEXPORT
bool RemoveNearestUnit(GAMEOBJ* G_Src,GAMEOBJ* G_Dst,int x,int y,int MaxR){
	if(G_Src->Type=='UNIT'&&G_Dst->Type=='UNIT'){
		UnitsGroup* SRC=&SCENINF.UGRP[G_Src->Index];
		UnitsGroup* DST=&SCENINF.UGRP[G_Dst->Index];
		word* IDS=SRC->IDS;
		word* SNS=SRC->SNS;
		
		
		int N=SRC->N;
		int minR=MaxR;
		int ID=-1;
		int idx;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&(!OB->Sdoxlo)){
					int r=Norma(x-(OB->RealX>>4),y-(OB->RealY>>4));
					if(r<minR){
						minR=r;
						ID=MID;
						idx=i;
					};
				};
			};
		};
		if(ID!=-1){
			if(idx<N-1){
				memcpy(IDS+idx,IDS+idx+1,(N-idx-1)<<1);
				memcpy(SNS+idx,SNS+idx+1,(N-idx-1)<<1);
			};
			SRC->N--;
			DST->AddNewUnit(Group[ID]);
			return true;
		};
		
		
	};
	return false;
};
extern int FogMode;
word GetFog(int x,int y);
DLLEXPORT
bool GetFogSpotVisibility(int x,int y){
	if(FogMode&&(!NATIONS[MyNation].Vision)&&GetFog(x,y<<1)<900)return false;
	return true;
};
CEXPORT
bool GetUnitVisibility(word Index){
	OneObject* OB=Group[Index];
	if(OB){
		return GetFogSpotVisibility(OB->RealX>>4,(OB->RealY>>5)-OB->RZ);
	}
	return false;
}
CEXPORT
bool GetSpotOnMapVisibility(int x,int y){
	return GetFogSpotVisibility(x,(y>>1)-GetHeight(x,y));
}
DLLEXPORT
int StormSimulator(GAMEOBJ* StormGroup,GAMEOBJ* AddDefenders,word BldIndex){
	if(StormGroup->Type!='UNIT')return 0;
	OneObject* OB=Group[BldIndex];
	UnitsGroup* SGRP=&SCENINF.UGRP[StormGroup->Index];
	if(OB&&OB->NewBuilding){
		UnitsGroup INSGRP;
		int NINS=OB->NInside;
		for(int i=0;i<NINS;i++){
			word MID=OB->Inside[i];	
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB){
					INSGRP.AddNewUnit(OB);
				};
			};
		};
		if(AddDefenders&&AddDefenders->Type=='UNIT'){
			UnitsGroup* UG=&SCENINF.UGRP[AddDefenders->Index];
			UG->CopyUnits(&INSGRP,0,UG->N,1,0);
		};
		NINS=INSGRP.N;
		word* IDS=INSGRP.IDS;
		word* SNS=INSGRP.SNS;
		word* S_IDS=SGRP->IDS;
		word* S_SNS=SGRP->SNS;
		int NSTORM=SGRP->N;
		for(i=0;i<NSTORM;i++){
			if(!NINS)return NSTORM-i;
			word SMID=S_IDS[i];
			if(SMID!=0xFFFF){
				OneObject* SOB=Group[SMID];
				if(OB&&OB->Serial==S_SNS[i]){
					word MID=IDS[NINS-1];
					if(MID!=0xFFFF){
						OneObject* IOB=Group[MID];
						if(IOB){
							int sf=IOB->newMons->StormForce;
							int min=OB->newMons->MinOposit;
							int max=OB->newMons->MaxOposit;
							int ins=OB->NInside;
							int mins=OB->MoreCharacter->MaxInside;
							int VV=((sf*(min+((max-min)*ins)/mins))*int(rando()))>>15;
							if(VV<SOB->newMons->StormForce){
								NINS--;
								if(!NINS)return NSTORM-i-1;
							};
						};
					};
				};
			};
		};
		return -NINS;
	};
	return  SGRP->N;
};
DLLEXPORT void CopyUnits(GAMEOBJ* Src,GAMEOBJ* Dest,int start,int n,bool add,bool remove){
	if(Src->Type=='UNIT'&&Dest->Type=='UNIT'){
		UnitsGroup* SRC=&SCENINF.UGRP[Src->Index];
		UnitsGroup* DST=&SCENINF.UGRP[Dest->Index];
		SRC->CopyUnits(DST,start,n,add,remove);
	};
};
bool MovePointToNotLockedArea(int& x,int& y,int Lx,int LockType){
	MotionField* MFI=&MFIELDS[LockType];
	int xx=(x-(Lx<<3))>>4;
	int yy=(y-(Lx<<3))>>4;
	if(!MFI->CheckBar(xx,yy,Lx,Lx))return true;
	for(int i=0;i<8;i++){
        int N=Rarr[i].N;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		for(int j=0;j<N;j++){
			if(!MFI->CheckBar(xx+xi[j],yy+yi[j],Lx,Lx)){
                x=(xx+xi[j])*16+Lx*8;
				y=(yy+yi[j])*16+Lx*8;
				return true;
			}
		}
	}
	return false;
}
DLLEXPORT bool GetBuildingEntryPoint(int* x,int* y,word Index,int AddDistance){
	OneObject* OB=Group[Index];
	*x=0;
	*y=0;
	if(OB&&OB->NewBuilding){
		*x=OB->RealX>>4;
		*y=OB->RealY>>4;
		NewMonster* NM=OB->newMons;
		if(NM->ConcPtX.GetAmount()){
			int xx,yy,xm,ym;
			OB->GetCornerXY(&xm,&ym);
			xx=(xm<<4)+NM->ConcPtX[0];
			yy=(ym<<4)+NM->ConcPtY[0];
			int XC=OB->RealX>>4;
			int YC=OB->RealY>>4;
			int NR=Norma(XC-xx,YC-yy);
			if(NR>20){
				xx+=(AddDistance*(xx-XC))/NR;
				yy+=(AddDistance*(yy-YC))/NR;
			};
			*x=xx;
			*y=yy;
			MovePointToNotLockedArea(*x,*y,5,0);
			return true;
		};
	};
	return false;
};
bool GetBuildingInternalPoint(int* x,int* y,word Index){
	OneObject* OB=Group[Index];
	*x=0;
	*y=0;
	if(OB&&OB->NewBuilding){
		*x=OB->RealX>>4;
		*y=OB->RealY>>4;
		NewMonster* NM=OB->newMons;
		int N=NM->ConcPtX.GetAmount();
		if(N){
			int xx,yy,xm,ym;
			OB->GetCornerXY(&xm,&ym);
			xx=(xm<<4)+NM->ConcPtX[N-1];
			yy=(ym<<4)+NM->ConcPtY[N-1];			
			*x=xx;
			*y=yy;			
			return true;
		};
	};
	return false;
};
bool GetBuildingEntryPointShift(int* x,int* y,word TypeIndex){
	GeneralObject* GO=NATIONS->Mon[TypeIndex];
	NewMonster* NM=GO->newMons;
	if(NM->Building){
		if(NM->ConcPtX.GetAmount()){
			int xx,yy,xm,ym;			
			xm=(NM->PicDx)>>4;
			ym=(NM->PicDy)>>3;
			xx=(xm<<4)+NM->ConcPtX[0];
			yy=(ym<<4)+NM->ConcPtY[0];
			*x=xx;
			*y=yy;
			return true;
		};
	};
	return false;
};

void GoToMineLink(OneObject* OBJ);
DLLEXPORT
void SetToInternalResourcesZero(GAMEOBJ* Grp){
	if(Grp->Type=='UNIT'){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		word* IDS=GRP->IDS;
		word* SNS=GRP->SNS;
		int N=GRP->N;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&OB->RType!=255){
					OB->RType=0xFF;
					OB->RAmount=0;
					OB->GroundState=0;
					OB->NewState=0;
					if(OB->UnlimitedMotion){
						if(!CheckBar(OB->x-1,OB->y-1,OB->Lx+2,OB->Lx+2)&&(!OB->LocalOrder || OB->LocalOrder&&OB->LocalOrder->DoLink!=&GoToMineLink)){
							OB->UnlimitedMotion=0;
						}
					}					
				};
			};
		};
	};
};
DLLEXPORT void LimitUnitsMotion(GAMEOBJ* Grp){
	if(Grp->Type=='UNIT'){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		word* IDS=GRP->IDS;
		word* SNS=GRP->SNS;
		int N=GRP->N;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]){
					OB->UnlimitedMotion=0;
				};
			};
		};
	};
}
DLLEXPORT
void DeleteUpperGroundUnits(GAMEOBJ* Grp){
	if(Grp->Type=='UNIT'){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		word* IDS=GRP->IDS;
		word* SNS=GRP->SNS;
		
		
		int N=GRP->N;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&!OB->Hidden){
					SNS[i]=0xFFFF;
				};
			};
		};
		
		
	};
}
DLLEXPORT
void RemoveUpperGroundUnits(GAMEOBJ* Src,GAMEOBJ* Dst){
	if(Src->Type=='UNIT'){
		if(Dst!=NULL&&Dst->Type=='UNIT'){
			UnitsGroup* GRP=&SCENINF.UGRP[Src->Index];
			UnitsGroup* DST=&SCENINF.UGRP[Dst->Index];
			word* IDS=GRP->IDS;
			word* SNS=GRP->SNS;
			
			
			
			
			int N=GRP->N;
			for(int i=0;i<N;i++){
				word MID=IDS[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SNS[i]&&!OB->Hidden){
						DST->AddNewUnit(OB);
						IDS[i]=0xFFFF;
					};
				};
			};
			
			
			
			
		}else{
			DeleteUpperGroundUnits(Src);
		}
	};
}
DLLEXPORT OneObject* GetOBJ(int Index){
	// MAXOBJECT
	if(Index<0xFFFF&&Index>=0)return Group[Index];
	else return NULL;
};
void RotUnit(OneObject* OB,char Dir,byte OrdType);
DLLEXPORT
void OBJ_Rotate(word Index,byte Dir,byte OrdType){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		addrand(Index);
		if(OB)RotUnit(OB,Dir,OrdType);
	};
};

DLLEXPORT void OBJ_SendTo(word Index,int x,int y,byte Prio,byte OrdType){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)OB->NewMonsterSendTo(x<<4,y<<4,Prio,OrdType);
	};
};
DLLEXPORT
void OBJ_PreciseSendTo(word Index,int x,int y,byte Prio,byte OrdType){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		addrand(Prio);
		if(OB)OB->NewMonsterPreciseSendTo(x<<4,y<<4,Prio,OrdType);
	};
};
DLLEXPORT
void OBJ_SmartSendTo(word Index,int x,int y,int dx,int dy,byte Prio,byte OrdType){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)OB->NewMonsterSmartSendTo(x,y,dx,dy,Prio,OrdType);
	};
};
DLLEXPORT
void OBJ_AttackObj(word Index,word Enemy,byte Prio,int NTimes){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)OB->AttackObj(Enemy,Prio,0,NTimes);
	};
};
DLLEXPORT
bool OBJ_GoToMine(word Index,word DestMine,byte prio,byte OrdType){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)return OB->GoToMine(DestMine,prio,OrdType);
	};
	return false;
};
DLLEXPORT
bool OBJ_GoToMineAlways(word Index,word DestMine,byte prio,byte OrdType){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB){
			bool peas=OB->newMons->Peasant;
			OB->newMons->Peasant=true;
			bool res=OB->GoToMine(DestMine,prio,OrdType);
			OB->newMons->Peasant=peas;
			return res;
		}
	};
	return false;
};
void LeaveMineLink(OneObject* OBJ);
DLLEXPORT
bool OBJ_LeaveMine(word Index,word NIndex){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB&&(!OB->LocalOrder||OB->LocalOrder->DoLink!=&LeaveMineLink)){
			OB->LeaveMine(NIndex);
			return true;
		}
	};
	return false;
};
DLLEXPORT
bool OBJ_GoToTransport(word Index,word TrID,byte Prio){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)return OB->GoToTransport(TrID,Prio);
	};
	return false;
};
DLLEXPORT
void OBJ_LeaveTransport(word Index,word NIndex){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)OB->LeaveTransport(NIndex);
	};
};
DLLEXPORT
bool OBJ_BuildObj(word Index,word BldIndex,byte Prio,byte OrdType){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)return OB->BuildObj(BldIndex,Prio,0,OrdType);
	};
	return false;
};
DLLEXPORT
int OBJ_TakeResource(word Index,int x,int y,byte ResID,byte Prio,byte OrdType){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)return OB->TakeResource(x,y,ResID,Prio,OrdType);
	};
	return 0;
};
DLLEXPORT
void OBJ_SetNotSelectable(word Index, bool Value){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB){
			OB->NotSelectable=Value;
		}
	};
};
DLLEXPORT
void OBJ_Produce(word Index,word NIndex,word GroupID){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB&&(GroupID!=0xFFFF||NATIONS[OB->NNUM].Mon[NIndex]->Enabled))OB->Produce(NIndex,GroupID);
	};
};
DLLEXPORT
int OBJ_GetProducingUnits(word Index,word NIndex){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB) return OB->CheckAbility(NIndex&8191);
	};
	return -1;
};
	
DLLEXPORT
void OBJ_AttackPoint(word Index,int x,int y,byte Prio,byte OrdType,int Times){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB)OB->NewAttackPoint(x,y,Prio,OrdType,Times);
	};
};
DLLEXPORT
int GetOrderType(OneObject* OB){
	if(OB&&OB->LocalOrder) return OB->LocalOrder->OrderType;
		else return 0;
};
DLLEXPORT
int GetMaxAttRadius(int Index){
	if(Index>=MAXOBJECT||Index<0) return -1;
	OneObject* OB=Group[Index];
	if(!OB)return -1;
	return OB->MoreCharacter->MaxR_Attack;
}
DLLEXPORT
int GetSearchRadius(int Index){
	if(Index>=MAXOBJECT||Index<0) return -1;
	OneObject* OB=Group[Index];
	if(!OB)return -1;
	//return OB->newMons->VisRange>>4;
	return OB->MoreCharacter->VisRange>>4;
}
DLLEXPORT
void GetTargetObject(word AttackID, word* Index, word* Serial){
	if(AttackID>=MAXOBJECT||AttackID==0xFFFF){
		*Index=0xFFFF;
		return;
	}
	OneObject* OB=Group[AttackID];
	if(OB&&OB->Attack){
		*Index = OB->EnemyID;
		*Serial = OB->EnemySN;
		OneObject* Enemy=Group[*Index];
		if(Enemy->Sdoxlo) *Index=0xFFFF;
	}else{
		*Index=0xFFFF;
	}
};
DLLEXPORT
int GetGroupForce(GAMEOBJ* Grp){
	int F=0;
	if(Grp->Type=='UNIT'){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		word* IDS=GRP->IDS;
		word* SNS=GRP->SNS;
		int N=GRP->N;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&!OB->Sdoxlo)F+=OB->newMons->Force;
			};
		};
	};
	return F;
};
DLLEXPORT
bool CheckLandLocking(int x,int y,int Lx,int Ly){
	return CheckBar(x>>4,y>>4,(Lx>>4)+1,(Ly>>4)+1);
};
DLLEXPORT byte GetNatNMASK(byte NI){
	return NATIONS[NI].NMask;
};
DLLEXPORT
void SetGroupStartNMASK(GAMEOBJ* Grp,byte Mask){
	if(Grp->Type=='UNIT'){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		GRP->NMASK=Mask;
	};
};
DLLEXPORT
int GetTSin(byte Idx){
	return TSin[Idx];
};
DLLEXPORT
int GetTCos(byte Idx){
	return TCos[Idx];
};
DLLEXPORT
char* GetMoneyString(int* res,char* tmp){
	tmp[0]=0;
	for(int i=0;i<6;i++)if(res[i]){
		if(tmp[0])strcat(tmp,", ");
		sprintf(tmp+strlen(tmp),"%s: %d",GetTextByID(RDS[i].Name),res[i]);
	};
	return tmp;
};
DLLEXPORT
bool CheckIfNotBusy(GAMEOBJ* Grp){
	if(Grp->Type=='UNIT'){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		word* IDS=GRP->IDS;
		word* SNS=GRP->SNS;
		int N=GRP->N;
		int TIME=GetGlobalTime();
		int NTotal=0;
		int NFree=0;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==SNS[i]){
					NTotal++;
					if(OB->BrigadeID!=0xFFFF){
						Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
						if(TIME-BR->LastOrderTime<20){
							if(!BR->BOrder)NFree++;;
						}else if(!OB->LocalOrder)NFree++;
					}else if(!OB->LocalOrder)NFree++;
				};
			};
		};
		if(!NTotal)return true;
		int Percent=NFree*100/NTotal;
		if(Percent>70)return true;
		if(Percent>60&&rando()<3000)return true;
		if(Percent>50&&rando()<1500)return true;
		if(Percent>40&&rando()<700)return true;
		if(Percent>30&&rando()<250)return true;
		if(Percent>20&&rando()<100)return true;
		if(Percent>10&&rando()<50)return true;
	};
	return false;
};
void AddGrpInfo(int idx,UnitsGroup* UG,char* s){
	if(UG->file){
		int L=strlen(UG->file);
		while(L&&UG->file[L]!='\\'&&UG->file[L]!='/')L--;
		if(L)L++;
		sprintf(s," %d[%s,%d]",idx,UG->file+L,UG->Line);
		if(UG->GOrder){
			typedef void GrpOrderLink(UnitsGroup* UGR);
			char* GetGrpName(GrpOrderLink* GR);
			sprintf(s+strlen(s),"<order: %s>",GetGrpName(UG->GOrder->Link));
		}
	}else{
		sprintf(s," %d",idx);
	};
};
void DetermineWhereUnitIs(word Index,char* dest){
	dest[0]=0;
	OneObject* OB=Group[Index];
	if(OB && OB->GRP_N){
		strcat(dest,"be_CGROUP(");
		for(int i=0;i<OB->GRP_N;i++){
            ClassRef<be_CGROUP> g;
			g.CPG_Index=OB->pGRP_IDS[i];
			if( g.CPG_Index!=0xFFFF ){
				if(g.Get()){
					strcat(dest," ");
					strcat(dest,g.Get()->Name.pchar());				
					sprintf(dest+strlen(dest),"(%X)",g.CPG_Index);
				}else{
					strcat(dest," ??? ");
				}
			}
			g.CPG_Index=0xFFFFFFFF;
		}
		strcat(dest," ) ");
	}
	//dest[0]=0;
	int NG=0;
	int N1=SCENINF.UGRP.GetAmount();
	for(int i=0;i<N1;i++){
		UnitsGroup* UG=&SCENINF.UGRP[i];
		word* IDS=UG->IDS;
		if(IDS){
			int N=UG->N;
			for(int j=0;j<N;j++)if(IDS[j]==Index&&NG<8){
				AddGrpInfo(i,UG,dest+strlen(dest));
				NG++;
				if(strlen(dest)>600)return;
			};
		};
	};
	
};
int* DMap=znew(int,2048);
int  DMapSize=2048;
DLLEXPORT void SetDangerMap(int* Map){
	int NA=GetNAreas();
	if(NA!=DMapSize){
		DMapSize=NA;
		DMap=(int*)realloc(DMap,DMapSize<<2);
	};
	memcpy(DMap,Map,NA<<2);
};
DLLEXPORT
int CreateListOfResourceSprites(byte ResID,int* Xi,int* Yi,int NMax){
	OneSprite* OS=Sprites;
	int n=0;
	for(int i=0;i<MaxSprt;i++){
		if(OS->Enabled&&OS->OC->ResType==ResID){
			if(n<NMax){
				Xi[n]=OS->x;
				Yi[n]=OS->y;
				n++;
			};
		};
		OS++;
	};
	return n;
};
DLLEXPORT
int EraseFarSpritesFromList(byte NI,byte ResID,int* Xi,int* Yi,int N,int MaxR){
	//creating list of storages
	int StoreX[256];
	int StoreY[256];
	int NStor=0;
	int No=NtNUnits[NI];
	word rcmask=1<<ResID;
	word* IDS=NatList[NI];
	for(int i=0;i<No;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(NStor<256&&OB&&OB->NewBuilding&&(!OB->Sdoxlo)&&OB->newMons->ResConcentrator&rcmask){
				StoreX[NStor]=OB->RealX>>4;
				StoreY[NStor]=OB->RealY>>4;
				NStor++;
			};
		};
	};
	if(NStor){
		int ns=0;
		for(int i=0;i<N;i++){
			int x0=Xi[i];
			int y0=Yi[i];
			for(int j=0;j<NStor;j++){
				int R=Norma(x0-StoreX[j],y0-StoreY[j]);
				if(R<MaxR){
					Xi[ns]=Xi[i];
					Yi[ns]=Yi[i];
					ns++;
				};
			};
		};
		return ns;
	}else{
		return 0;
	};
};
void DestructBuilding(OneObject* OB);
DLLEXPORT
void DieUnit(word MID){
	if(MID!=0xFFFF){
		OneObject* OB=Group[MID];
		if(OB)DestructBuilding(OB);
	};
};
DLLEXPORT
void AssignDesantStartGroup(byte NI,GAMEOBJ* Grp){
	if(NI<8&&Grp->Type=='UNIT'){
		CITY[NI].DesantStartGroup=Grp->Index;
	};
};
DLLEXPORT
void AssignDesantDestGroup(byte NI,GAMEOBJ* Grp){
	if(Grp->Type=='UNIT'){
		CITY[NI].DesantDestGroup=Grp->Index;
	};
};
DLLEXPORT void SetAI_Guard(GAMEOBJ* Grp,bool val){
	if(Grp->Type=='UNIT'){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		word* IDS=GRP->IDS;
		word* SNS=GRP->SNS;
		int N=GRP->N;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&!OB->Sdoxlo)OB->AI_Guard=val;
			};
		};
	};
};
DLLEXPORT
void SetMaxInTransport(byte NI,int Max){
	if(NI<8)CITY[NI].MaxInTransport=Max;
};
extern bool ViewFriendsInScenario;
DLLEXPORT
void SetScenarioVisionMode(bool Mode){
	ViewFriendsInScenario=Mode;
};
extern bool AllowArt;

typedef bool SubstInterfFunc(word IDX);
extern SubstInterfFunc* CheckInterface;

DLLEXPORT
void SetInterfaceCallback(SubstInterfFunc* F){
	CheckInterface=F;
};

typedef tpDeathCallback(int KilledIndex,int VictimIndex);
DynArray<tpDeathCallback*> daDeathCallback;
CEXPORT tpDeathCallback* SetDeathCallback(tpDeathCallback* dc){
	if(daDeathCallback.find(dc)==-1)daDeathCallback.Add(dc);
	return NULL;
}

typedef tpBirthCallback(int NewbornIndex);
DynArray<tpBirthCallback*> daBirthCallback;
CEXPORT tpBirthCallback* SetBirthCallback(tpBirthCallback* dc){
	if(daBirthCallback.find(dc)==-1)daBirthCallback.Add(dc);
	return NULL;
}

typedef tpDamageCallback(int KillerIndex,int VictimIndex,int Value);
DynArray<tpDamageCallback*> daDamageCallback;
CEXPORT tpDamageCallback* SetDamageCallback(tpDamageCallback* dc){
	if(daDamageCallback.find(dc)==-1)daDamageCallback.Add(dc);
	return NULL;
};

SetFillDelay(int KillerID, int VictimID, int Damage){
	if(VictimID>=0&&VictimID<MAXOBJECT){
		OneObject* VOB=Group[VictimID];
		if(VOB&&!VOB->Sdoxlo&&VOB->BrigadeID!=0xFFFF){
			Brigade* VBr=CITY[VOB->NNUM].Brigs+VOB->BrigadeID;
			if(VBr&&VBr->Enabled){
				VBr->FillDelay=REALTIME+((20*GameSpeed)>>8);
			}
		}
	}
	return 0;
};

void SetBrigFillDelay(OneObject* Victim){
	if(Victim->BrigadeID!=0xFFFF){
		Brigade* VBr=CITY[Victim->NNUM].Brigs+Victim->BrigadeID;
		if(VBr&&VBr->Enabled){
			VBr->FillDelay=REALTIME+((20*GameSpeed)>>8);
		}
	}
}

typedef tpCaptureCallback(int VictimIndex,byte SrcNation,byte DstNation,int CapturerIndex);
DynArray<tpCaptureCallback*> daCaptureCallback;
CEXPORT tpCaptureCallback* SetCaptureCallback(tpCaptureCallback* dc){
	if(daCaptureCallback.find(dc)==-1)daCaptureCallback.Add(dc);
	return NULL;
};

typedef tpDrawOnMiniMapCallback(int x,int y,int Lx,int Ly,int mx0,int my0);
DynArray<tpDrawOnMiniMapCallback*> daDrawOnMiniMapCallback;
CEXPORT tpDrawOnMiniMapCallback* SetDrawOnMiniMapCallback(tpDrawOnMiniMapCallback* dc){
	if(daDrawOnMiniMapCallback.find(dc)==-1)daDrawOnMiniMapCallback.Add(dc);
	return NULL;
};

DynArray<tpDrawOnMapCallback*> daDrawOnMapCallback;
CEXPORT tpDrawOnMapCallback* SetDrawOnMapCallback(tpDrawOnMapCallback* dc){
	if(daDrawOnMapCallback.find(dc)==-1)daDrawOnMapCallback.Add(dc);
	return NULL;
};

void ClearCallbacks(){
	daDrawOnMapCallback.Clear();
	daDrawOnMiniMapCallback.Clear();
	daCaptureCallback.Clear();
	daDamageCallback.Clear();
	daBirthCallback.Clear();
	daDeathCallback.Clear();
}
int LastDeadIndex=0xFFFF;
void OnDie(int Killer,int Victim){
	bool ACTION_OnUnitDie(OneObject* OB,word KillerID);
	ACTION_OnUnitDie(Group[Victim],Killer);
	if(Killer==0xFFFF){
		if(LastDeadIndex==Victim)return;
	}else{
		LastDeadIndex=Victim;
	}
	for(int i=0;i<daDeathCallback.GetAmount();i++)daDeathCallback[i](Killer,Victim);
};
void OnDamage(int Killer,int Victim,int Damage){
	for(int i=0;i<daDeathCallback.GetAmount();i++)daDamageCallback[i](Killer,Victim,Damage);
};
//extern int MinMapX;
//extern int MinMapY;

void OnMiniDraw(int x,int y,int Lx,int Ly,int mx0,int my0){
	RunPF((5<<8)+4,"OnMiniDraw");
	//return;
	for(int i=0;i<daDrawOnMiniMapCallback.GetAmount();i++){
		TempWindow TW;
		PushWindow(&TW);
		IntersectWindows(x,y,x+Lx-1,y+Ly-1);
		daDrawOnMiniMapCallback[i](x/*-MinMapX*/,y/*-MinMapY*/,x+Lx-1,y+Ly-1,mx0,my0);		
		//MinMapX=x0>>5;
		//MinMapY=y0>>5;		
		PopWindow(&TW);
	}
	StopPF((5<<8)+4);	
};
void OnMapDraw(){
	RunPF((5<<8)+5,"OnMapDraw");
	//return;
	for(int i=0;i<daDrawOnMapCallback.GetAmount();i++)daDrawOnMapCallback[i]();
	StopPF((5<<8)+5);
}
void OnBirth(int idx){
	for(int i=0;i<daBirthCallback.GetAmount();i++)daBirthCallback[i](idx);
}
void OnCapture(int Victim,byte Src,byte Dst,int Capturer){
	for(int i=0;i<daCaptureCallback.GetAmount();i++)daCaptureCallback[i](Victim,Src,Dst,Capturer);
};
CEXPORT
void SetDrawScale(float scale){
#ifdef _USE3D
	GPS.SetScale(scale);
#endif
}
CEXPORT
void DrawPicture(int x,int y,int File,int Sprite,byte Nat){
	GPS.ShowGP(x,y,File,Sprite,Nat);
}
CEXPORT
int GetPictureIndex(char* name){
	return GPS.PreLoadGPImage(name);	
}
CEXPORT
void FlushPictures(){
#ifdef _USE3D
	GPS.FlushBatches();
#endif
}
#ifdef _USE3D
void M4_srtpp(Matrix4D& M,
			  float dx,float dy,float dz,
			  float pivX,float pivY,float Angle,
			  float SX,float SY);
#endif
CEXPORT
void SetCurrentDiffuse(DWORD Color){
#ifdef _USE3D
	GPS.SetCurrentDiffuse(Color);
#endif
};
CEXPORT
void DrawRotatedPicture(int x,int y,int File,int Sprite,byte Nat,
						int dx,int dy,float Angle,float sx,float sy){
#ifdef _USE3D
	Matrix4D M;
	M4_srtpp(M,x+0.5,y+0.5,0,dx,dy,Angle,sx,sy);
	int sh=GPS.GetCurrentShader();
	if(ISM->GetPackageMagic(File)=='D2PG'){
		static int s=IRS->GetShaderID("sprite_natcolor_interface");
		static int s1=IRS->GetShaderID("sprite_natcolor_interface_1v1");
		if(sx==1.0f && sy==1.0f && GPS.GetScreenSpaceTM()==NULL)GPS.SetShader(s1);
		else GPS.SetShader(s);
	}
	GPS.ShowGPTransformed(File,Sprite,M,Nat);
	if(sh!=GPS.GetCurrentShader()){
		//GPS.FlushBatches();
        ISM->Flush();
		GPS.SetShader(sh);
	}
#endif
}
CEXPORT
void TransformToScreen(int x,int y,int z,float& scrx,float& scry,float& Scale){
	Scale=1.0/float(1<<(5-Shifter));
	scrx=(x-(mapx<<5))*Scale;
	scry=(((y-(mapy<<5))>>1)-z)*Scale;
}
//interface substituting
typedef int tpUnitInterfaceSubst(int x0,int y0,int Lx0,int Ly0,int x1,int y1,int Index);
//return value:
//0-interface not changed
//1-miss default left panel
//2-miss default right panel
//3-miss default left&right panel
tpUnitInterfaceSubst* lpUnitInterfaceSubst=NULL;
CEXPORT
tpUnitInterfaceSubst* SetUnitInterfaceCallback(tpUnitInterfaceSubst* cb){
	tpUnitInterfaceSubst* cur=lpUnitInterfaceSubst;
	lpUnitInterfaceSubst=cb;
	return lpUnitInterfaceSubst;
};
void xLine(int x,int y,int x1,int y1,byte c);

void GetUnitsSelGroups(byte NI);
void CreateSelInterface();
void CreateUnitInterface();
int CreateStandartUnitsInterface();
extern int NSelWay;
void PrepareDrawObjWay(OneObject* OB);
void PrepareDrawBrigWay(Brigade* Br);
void ShowBuildComPanel(OneObject* OB);

CIMPORT void SetTopSelBrigID(word BID);
int DrawUnitsInterface(){
	if(v_MainMenu.vDS){
		return 0;
	}
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//
	NSelWay=0;
	GetUnitsSelGroups(MyNation);
	SetTopSelBrigID(0xFFFF);
	//
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->NI==NI){
		if(SP->Type==ve_UT_brigade){
			word BrID=SP->Inf.Brigade.BrigID;
			SetTopSelBrigID(BrID);
			Brigade* br=NATIONS[NI].CITY->Brigs+BrID;
			PrepareDrawBrigWay(br);		
		}else
		if(SP->Type!=ve_UT_building){
			OneObject* OB=NULL;
			int n=ImNSL[MyNation];
			word* id=ImSelm[MyNation];
			word* sn=ImSerN[MyNation];
			for(int i=0;i<n;i++,id++,sn++){
				if(*id!=0xFFFF){
					OB=Group[*id];
					if(OB&&OB->Serial==*sn&&!OB->Sdoxlo&&OB->NIndex==SP->NIndex) break;
				}
			}
			if(OB){
				PrepareDrawObjWay(OB);
			}
		}
	}

	return vgf_UI_Process();
		
	if(!OIS.NSelBr&&((OIS.NSelObj==1&&NATIONS->Mon[OIS.SelObj[0]]->newMons->Peasant)||OIS.NSelObj==0&&(OIS.Bld.GetAmount()>0||OIS.Settlement!=0xFFFF||OIS.Oboz!=0xFFFF))){
		RunPF((5<<8)+1,"DrawUnitsInterface");
		if(OIS.NSelObj==1){
			NewMonster* NM=NATIONS[NI].Mon[OIS.SelObj[0]]->newMons;
			if(NM->VUI!=0xFFFF){
				CreateUnitInterface();
				StopPF((5<<8)+1);
				return 4;
			}
		}
		int result=CreateStandartUnitsInterface();
		StopPF((5<<8)+1);
		return result;
	}else	
	if(OIS.NSelBr==1&&!OIS.NSelObj){
		return 4;
	}else{
		if(OIS.NSelBr==0&&OIS.NSelObj==0) return 4;
		if(OIS.NSelObj>0){
			NewMonster* NM=NATIONS[NI].Mon[OIS.SelObj[0]]->newMons;
			if(NM->VUI!=0xFFFF){
				CreateUnitInterface();			
				StopPF((5<<8)+1);
				return 4;
			}
		}
		RunPF((5<<8)+1,"CreateSelInterface");
		CreateSelInterface();
		if(OIS.NSelObj>0){
			OneObject* OB=NULL;
			int n=ImNSL[MyNation];
			word* id=ImSelm[MyNation];
			word* sn=ImSerN[MyNation];
			for(int i=0;i<n;i++,id++,sn++){
				OB=Group[*id];
				if(OB&&OB->Serial==*sn&&!OB->Sdoxlo) break;
			}
			if(OB){
				PrepareDrawObjWay(OB);
				ShowBuildComPanel(OB);
			}
		}
		StopPF((5<<8)+1);
		return 4;
	}
}
/*
Summary of interface substituting functions:

typedef int tpUnitInterfaceSubst(int x0,int y0,int Lx0,int Ly0,int x1,int y1,int Index);
//return value:
//0-interface not changed
//1-miss default left panel
//2-miss default right panel
//3-miss default left&right panel
//7-dont draw anything
tpUnitInterfaceSubst* SetUnitInterfaceCallback(tpUnitInterfaceSubst* cb);

typedef void HandlePro(int);

void* AddIconToPanel(int x,int y,int FileID,int SpriteID);
void AddHintToIcon(void* OI,char* Hint);
void AddProcToIcon(void* OI,HandlePro* L,int Lparam,HandlePro* R,int Rparam);
void AddStageToIcon(void* OI,int Value,int MaxValue);
void AddNumberToIcon(void* OI,int Value);
void AddLevelToIcon(void* OI,int Value);

Summary of the graphical functions:

void SetDrawScale(float scale);
void DrawPicture(int x,int y,int File,int Sprite,byte Nat);
int GetPictureIndex(char* name);
void FlushPictures();
void SetCurrentDiffuse(DWORD Color);
void DrawRotatedPicture(int x,int y,int File,int Sprite,byte Nat,
						int dx,int dy,float Angle,float sx,float sy);
void TransformToScreen(int x,int y,int z,float& scrx,float& scry,float& Scale);
*/

CEXPORT
int GetMapMaxCoor(){
	return 8192<<ADDSH;
}

CEXPORT
int GetSelectedList(byte NI, word*& List){
	if(NI<8){
		List=ImSelm[NI];
		return ImNSL[NI];
	}else{
		List=NULL;
		return 0;
	}
}

#include "Vital\vital.h"
#include "Vital\ArmyMind.h"
#include "Vital\BattleDialogs.h"
bool GetOfficersType(byte NI, word UnitType, word &OffID, word &BarabanID, word &FlagID)
{
	OffID=0xFFFF;
	BarabanID=0xFFFF;
	FlagID=0xFFFF;
	for(int k=0;k<NATIONS[NI].NMon;k++)
	{
		GeneralObject* GO=NATIONS[NI].Mon[k];
		if(GO->OFCR)
			for(int i=0;i<GO->OFCR->NStroi;i++)
			{
				for(int j=0;j<GO->OFCR->SDES[i].NUnits;j++)
					if(GO->OFCR->SDES[i].Units[j]==UnitType)
					{
						OffID=k;
						BarabanID=GO->OFCR->BarabanID;
						FlagID=GO->OFCR->FlagID;
						return true;
					}
			}
	}
	return false;
}
CEXPORT int GetFormationIndexBySizeIndex(byte,word,byte);
CEXPORT
word CreateFormationFromGroup(byte BrigNI, GAMEOBJ* Source, int x, int y, byte dir){ //, word bNIndex=0xFFFF
	// detect NIndex of brig
	word bNIndex=0xFFFF;
	UnitsGroup* RG=GetUnitsGroup(Source);
	int nrg=RG->N;
	if(bNIndex==0xFFFF){
		for(int i=0;i<nrg;i++){
			OneObject* OB=GetOBJ(RG->IDS[i]);
			if(OB&&OB->Serial==RG->SNS[i]&&!OB->Sdoxlo){
				bNIndex=OB->NIndex;
				break;
			}
		}
	}
	//
	if(bNIndex!=0xFFFF){
		int FormID=GetFormationIndexBySizeIndex(BrigNI,bNIndex,0);
		int BID=CITY[BrigNI].GetFreeBrigade();
		if(BID!=-1&&FormID!=-1)
		{
			Brigade* BR=CITY[BrigNI].Brigs+BID;
			BR->Enabled=1;
			BR->Direction=dir;
			if(BR->CreateFromGroup(RG,FormID,dir))
			{				
				for(int i=0;i<nrg;i++){
					OneObject* OB=Group[RG->IDS[i]];
					if(OB&&OB->Serial==RG->SNS[i]&&!OB->Sdoxlo&&OB->InArmy){
						RG->IDS[i]=0xFFFF;
					}
				}
				ext_OnBrigadeCreated(BR);
				return BID;
			}			
		}		
	}
	return 0xFFFF;
}
CEXPORT
int PutNewFormation(byte NI, word UnitType, word OffID, word BarabanID, word FlagID, word FormationIndex, int x, int y, byte Dir=0)
{
	int retBID = -1;
	OrderDescription* OD=&(ElementaryOrders[FormationIndex]);
	if(OD)
	{
		UnitsGroup UG;
		if(OffID!=0xFFFF)
		{
			if(OffID!=UnitType)
			{
				OneObject* OB=Group[NATIONS[NI].CreateNewMonsterAt(x,y,OffID,true)];
				if(OB)UG.AddNewUnit(OB);
			}
			else
			{
				UG.Allocate(1);
				UG.N=1;
				UG.IDS[0]=0xFFFF;
			}
		}
		if(BarabanID!=0xFFFF)
		{
			OneObject* OB=Group[NATIONS[NI].CreateNewMonsterAt(x,y,BarabanID,true)];
			if(OB)UG.AddNewUnit(OB);
		}
		for(int i=0;i<OD->NUnits;i++)
		{
			OneObject* OB=Group[NATIONS[NI].CreateNewMonsterAt(x,y,UnitType,true)];
			if(OB)UG.AddNewUnit(OB);
		}
		if(FlagID!=0xFFFF)
		{
			OneObject* OB=Group[NATIONS[NI].CreateNewMonsterAt(x,y,FlagID,true)];
			if(OB)UG.AddNewUnit(OB);
		}
		int BID=CITY[NI].GetFreeBrigade();
		if(BID!=-1&&UG.IDS!=NULL)
		{
			Brigade* BR=CITY[NI].Brigs+BID;
			BR->Enabled=1;
			BR->Direction=Dir;
			if(BR->CreateFromGroup(&UG,FormationIndex,Dir))
			{
				BR->CreateSimpleOrderedPositions(x,y,Dir);
				for(int e=0;e<BR->NMemb;e++)
				{
					word MID=BR->Memb[e];
					if(MID!=0xFFFF)
					{
						OneObject* OB=Group[MID];
						OB->RealX=BR->posX[e]<<4;
						OB->RealY=BR->posY[e]<<4;
						OB->RZ=GetTotalHeight(OB->RealX>>4,OB->RealY>>4);
						addrand(OB->RZ);
						OB->RealDir=Dir;
						OB->GraphDir=Dir;
					}
				}
			}
			retBID = BR->ID;
			BR->ClearBOrders();
			BR->KeepPositions(0,128);
			ext_OnBrigadeCreated(BR);
		}
	}
	return retBID;
}
CEXPORT
int PutNewFormation(byte NI, word UnitType, word FormationIndex, int x, int y, byte Dir=0)
{
	int	ret = -1;
	OrderDescription* OD=&(ElementaryOrders[FormationIndex]);
	if(OD)
	{
		word OffID, BarabanID, FlagID;
		if(GetOfficersType(NI, UnitType, OffID, BarabanID, FlagID))
		{
			ret = PutNewFormation( NI, UnitType, OffID, BarabanID, FlagID, FormationIndex, x, y, Dir);
		}
	}
	return ret;
}
//
CEXPORT int GetFormationIndexBySizeIndex(byte NI, word UnitType, byte SizeIndex)
{
	word OffID, BarabanID, FlagID;//, FormationIndex;
	if(GetOfficersType(NI, UnitType, OffID, BarabanID, FlagID))
	{
		GeneralObject* GO=NATIONS[NI].Mon[OffID];
		int CurShape=0;
		int max=GO->OFCR->SDES[CurShape].NAmount;
		if(max==3)
		{
			CurShape=1;
			max=GO->OFCR->SDES[CurShape].NAmount;
		}
		if(max)
		{
			SizeIndex%=max;
			return GO->OFCR->SDES[CurShape].LocalID[SizeIndex];
		}
	}
	return -1;
}
CEXPORT int GetFormationIndexBySizeIndex(byte NI, word UnitType, byte SizeIndex, byte CurShape)
{
	word OffID, BarabanID, FlagID;//, FormationIndex;
	if(GetOfficersType(NI, UnitType, OffID, BarabanID, FlagID))
	{
		GeneralObject* GO=NATIONS[NI].Mon[OffID];
		int maxCurShape=GO->OFCR->NStroi;
		CurShape%=maxCurShape;
		int max=GO->OFCR->SDES[CurShape].NAmount;
		if(max)
		{
			SizeIndex%=max;
			return GO->OFCR->SDES[CurShape].LocalID[SizeIndex];
		}
	}
	return -1;
}
OfficerRecord* GetOfficerRecord(byte NI, word UnitType){
	word OffID, BarabanID, FlagID;
	if(GetOfficersType(NI, UnitType, OffID, BarabanID, FlagID))
	{
		GeneralObject* GO=NATIONS[NI].Mon[OffID];
		int maxCurShape=GO->OFCR->NStroi;
		return GO->OFCR;
	}
	return NULL;
}
//
CEXPORT
int PutNewFormationBySizeIndex(byte NI, word UnitType, byte SizeIndex, int x, int y, byte Dir=0)
{
	int ret = -1;
	word OffID, BarabanID, FlagID, FormationIndex;
	if(GetOfficersType(NI, UnitType, OffID, BarabanID, FlagID))
	{
		GeneralObject* GO=NATIONS[NI].Mon[OffID];
		int CurShape=0;
		int max=GO->OFCR->SDES[CurShape].NAmount;
		if(max==3)
		{
			CurShape=1;
			max=GO->OFCR->SDES[CurShape].NAmount;
		}
		if(max)
		{
			SizeIndex%=max;
			FormationIndex=GO->OFCR->SDES[CurShape].LocalID[SizeIndex];
			ret = PutNewFormation(NI, UnitType, OffID, BarabanID, FlagID, FormationIndex, x, y, Dir);
		}
	}
	return ret;
}
CEXPORT
int PutNewFormationBySizeIndex(byte NI, word UnitType, byte SizeIndex, int formT, int x, int y, byte Dir=0)
{
	int ret = -1;
	word OffID, BarabanID, FlagID, FormationIndex;
	if(GetOfficersType(NI, UnitType, OffID, BarabanID, FlagID))
		{
		GeneralObject* GO=NATIONS[NI].Mon[OffID];
		int CurShape=formT;
		int max=GO->OFCR->SDES[CurShape].NAmount;
		if(max==3)
			{
			CurShape=1;
			max=GO->OFCR->SDES[CurShape].NAmount;
			}
		if(max)
			{
			SizeIndex%=max;
			FormationIndex=GO->OFCR->SDES[CurShape].LocalID[SizeIndex];
			ret = PutNewFormation(NI, UnitType, OffID, BarabanID, FlagID, FormationIndex, x, y, Dir);
			}
		}
	return ret;
}
CEXPORT	//Vitya //  [11/4/2003]
int PutNewFormationBySizeIndex(byte NI, char* UnitType, byte SizeIndex, int x, int y, byte Dir=0) {
	int UTID=0xFFFF;
	GeneralObject** GOS=NATIONS[0].Mon;
	int N=NATIONS[0].NMon;
	for(INT i=0;i<N;i++)if(!strcmp(GOS[i]->MonsterID,UnitType)){
		UTID=i;
	};
	if (UTID==0xFFFF) {
		IntErr("RegisterUnitType : Unknown unit type : %s",UnitType);
		SCENINF.NErrors++;
		return 0xFFFF;
	}
	return	PutNewFormationBySizeIndex(NI,UTID,SizeIndex,x,y,Dir);
}

CEXPORT int PutNewFormationBySizeIndex2(byte NI, char* UnitType, byte SizeIndex, int x, int y, byte Dir=0){	//grey //  [03/02/2004]
	int UTID=0xFFFF;
	int TMID=0xFFFF;
	GeneralObject** GOS=NATIONS[0].Mon;
	int N=NATIONS[0].NMon;
	for(INT i=0;i<N;i++) {
		if(!strcmp(GOS[i]->MonsterID,UnitType))			{ UTID=i; }
		if(!strcmp(GOS[i]->MonsterID, "TempMent(FR)" )) { TMID=i; }
	};
	if (UTID==0xFFFF) {
		IntErr("RegisterUnitType : Unknown unit type : %s",UnitType);
		SCENINF.NErrors++;
		return 0xFFFF;
	}
	if ( TMID==0xFFFF )   { return 0xFFFF; }
	word  OffID = UTID, BarabanID = 0xFFFF, FlagID = 0xFFFF;
	word  FormationIndex = GetFormationIndexBySizeIndex(NI, TMID, SizeIndex);
	return  PutNewFormation( NI, UTID, OffID, BarabanID, FlagID, FormationIndex, x, y, Dir) ;
}

CEXPORT	int CreateUnitByTypeAt(byte NI, word UnitType, int x, int y)//grey //  [11/08/2004]
{
	int id = -1;
	if(NI>=0 && NI<8)
		id = NATIONS[NI].CreateNewMonsterAt(x,y,UnitType,true);
	return  id;
}

CEXPORT
bool	SetCOMPSTART( int nation ) {	//grey [06.02.2004]
	if( nation < 0 || nation > 7 )	{ return false; }
	COMPSTART[nation] = 1 ;
	return true ;
}

CEXPORT
int GetGameSpeed(){
	return GameSpeed;
}
typedef bool tpUnitsCallback(OneObject* OB,void* param);
CEXPORT 
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param){
	int NR=(R>>7)+1;
	if(NR>RRad) NR=RRad;
	int cx0=(xc>>7)+1;
	int cy0=(yc>>7)+1;
	int NN=0;
    int R2=R*R;
	for(int i=0;i<NR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
        int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=cx0+xi[j];
			int yy=cy0+yi[j];
			if(xx>=0&&yy>=0&&xx<VAL_MAXCX&&yy<VAL_MAXCX){
				int cell=xx+(yy<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
                            if (!OB) continue;
                            int dx=OB->RealX/16-xc;
                            int dy=OB->RealY/16-yc;
							if(dx*dx+dy*dy<R2&&CB(OB,Param))NN++;
						}
					}
				}
			}
		}
	}
	return NN;
}
CEXPORT 
int PerformActionOverBuildingsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param){
	int NR=(R>>7)+1;
	int cx0=(xc>>7);
	int cy0=(yc>>7);
	int NN=0;
	for(int i=0;i<NR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=cx0+xi[j];
			int yy=cy0+yi[j];
			if(xx>=0&&yy>=0&&xx<VAL_MAXCX&&yy<VAL_MAXCX){
				int cell=xx+(yy<<VAL_SHFCX);
				word MID;
				word GetOneBld(int cell,int pos);
				for(int pos=0;(MID=GetOneBld(cell,pos))!=0xFFFF;pos++){				
					OneObject* OB=Group[MID];
					if(OB&&Norma((OB->RealX>>4)-xc,(OB->RealY>>4)-yc)<R){
						if(CB(OB,Param))NN++;
					}
				}
			}
		}
	}
	return NN;
}
void SetArmAttackState(byte NI,byte i);
CEXPORT
void DLLSetAttState(byte NI,byte i){
	SetArmAttackState(NI,i);
}

extern bool PeaceMode;
CEXPORT
bool GetPeaceMode(){
	return PeaceMode;
}
CEXPORT
bool CheckNoCityLife(){
	return MOptions.DontAllowCityLife;
}

extern int NNations;
extern char** NatNames;
extern char* WeaponIDS[32];

void SaveUnitsStatsToFile(char* file){
	DString FN(file);
	FN.Add(".csv");

	DString US;
	//Nation* Nat=NATIONS->
	DString UnitName;
	US.Add("Nation");US.Add(";");
	US.Add("Type");US.Add(";");
	US.Add("Name");US.Add(";");
	US.Add("MDName");US.Add(";");
	US.Add("Speed");US.Add(";");
	US.Add("Life");US.Add(";");
	US.Add("Damage");US.Add(";");
	US.Add("Attack1 Type");US.Add(";");
	US.Add("Attack1 Damege");US.Add(";");
	US.Add("Attack2 Type");US.Add(";");
	US.Add("Attack2 Damege");US.Add(";");
	US.Add("AttackRadius");US.Add(";");
	US.Add("AttackPause");US.Add(";");
	// Protection types : 0,1,2,3
	for ( int i=0; i<4; i++ )	{ US.Add( WeaponIDS[i] );US.Add( " protection" );US.Add(";"); };
	//US.Add("Price");US.Add(";");
	US.Add("WOOD");US.Add(";");
	US.Add("GOLD");US.Add(";");
	US.Add("STONE");US.Add(";");
	US.Add("FOOD");US.Add(";");
	US.Add("IRON");US.Add(";");
	US.Add("COAL");US.Add(";");
	
	US.Add("BuildStage");US.Add(";");
	US.Add("ShotRes");US.Add(";");
	US.Add("Vision");US.Add(";");
	US.Add("Razbros");US.Add(";");
	US.Add("UnitRadius");US.Add(";");
	US.Add("FearType");US.Add(";");
	US.Add("StartMorale");US.Add(";");
	US.Add("Experience");US.Add(";");

	US.Add("\n");

	for(int i=0;i<NNations;i++){
		//US.Add(NatNames[i]);
		//US.Add("\n");		
		int NUNITS=NATIONS->NUnits[i];
		word* UNITS=NATIONS->UnitsIDS[i];
		for(int j=0;j<NUNITS;j++){
			word UIDS=UNITS[j];
			if(UIDS==0xFFFF){
				UnitName.Clear();
				UnitName.Add(GetTextByID(NATIONS->UnitNames[i][j]));
			}else{
				US.Add(NatNames[i]);                US.Add(";");
				US.Add(UnitName);					US.Add(";");
				GeneralObject* GO=NATIONS->Mon[UIDS];
				US.Add(GO->Message);                US.Add(";");
				US.Add(GO->newMons->MD_File);       US.Add(";");
				US.Add(GO->newMons->MotionDist);    US.Add(";");				
				US.Add(GO->newMons->Life);			US.Add(";");
				int k;
				for(k=0;k<NAttTypes;k++)
				{
					if(GO->newMons->MaxDamage[k])
					{
						US.Add(k);US.Add("-");US.Add(GO->newMons->MaxDamage[k]);US.Add(",");
					}
				}
				US.Add(";");
				// Attack1 - Type
				if(GO->newMons->MaxDamage[0])
					US.Add( WeaponIDS[ GO->newMons->WeaponKind[0] ] );
				US.Add(";");
				// Attack1 - Damage
				if(GO->newMons->MaxDamage[0])
					US.Add(GO->newMons->MaxDamage[0]);
				US.Add(";");
				// Attack2 - Type
				if(GO->newMons->MaxDamage[1])
					US.Add( WeaponIDS[ GO->newMons->WeaponKind[1] ] );
				US.Add(";");
				// Attack2 - Damage
				if(GO->newMons->MaxDamage[1])
					US.Add(GO->newMons->MaxDamage[1]);
				US.Add(";");

				for(k=0;k<NAttTypes;k++)
				{
					if(GO->newMons->MaxDamage[k])
					{
						US.Add(k);US.Add("-");
						US.Add(GO->newMons->AttackRadius1[k]);
						US.Add(" ");
						US.Add(GO->newMons->AttackRadius2[k]);
						US.Add(",");
					}
				}
				US.Add(";");
				for(k=0;k<NAttTypes;k++)
				{
					if(GO->MoreCharacter->MaxDamage[k])
					{
						US.Add(k);US.Add("-");US.Add(GO->MoreCharacter->AttackPause[k]);US.Add(",");
					}
				}
				US.Add(";");

				// Protection 0,1,2,3
				for ( int i=0; i<4; i++ ){
					US.Add( GO->newMons->Protection[i] );
					US.Add(";");
				};

				for(k=0;k<6;k++)
				{
					if(GO->newMons->NeedRes[k])
					{
						//US.Add(k);US.Add("-");
						US.Add(GO->newMons->NeedRes[k]);
						//US.Add(",");
					}
					US.Add(";");
				}

				US.Add(GO->MoreCharacter->ProduceStages);	US.Add(";");

				for(k=0;k<GO->newMons->NShotRes;k++)
				{
					if(k)US.Add(",");
					if(GO->newMons->ShotRes[k*2]==5)
						US.Add("C");
					else
					if(GO->newMons->ShotRes[k*2]==4)
						US.Add("I");
					else
						US.Add(GO->newMons->ShotRes[k*2]);
					US.Add(" ");US.Add(GO->newMons->ShotRes[k*2+1]);
				}
				US.Add(";");
				
				US.Add(GO->newMons->VisionType);			US.Add(";");
				US.Add(GO->newMons->Razbros);	    		US.Add(";");
				US.Add(GO->newMons->UnitRadius);			US.Add(";");

				for(k=0;k<NAttTypes;k++)
				{
					if(GO->newMons->FearType[k]!=255)
					{
						US.Add(k);US.Add("-");US.Add(GO->newMons->FearType[k]);US.Add(",");
					}
					
				}
				US.Add(";");

				US.Add(GO->newMons->StartMorale);			US.Add(";");
					
				// Experience
				US.Add( GO->newMons->Expa );				US.Add(";");

				US.Add("\n");
			}
		}
	}

	US.WriteToFile(FN.pchar());
};


bool GAMEOBJ::CreateZone(int x, int y, int r){	
	Index=x;
	Serial=y;
	Type='@   '-0x202020+r;
	return true;
}

bool GAMEOBJ::isGroup(){
	return Type=='UNIT';
};

word GAMEOBJ::FindUnits(word ID, word SN){
	if(isGroup()){
		UnitsGroup* UG=SCENINF.UGRP+Index;
		int n=UG->N;
		word* id=UG->IDS;
		word* sn=UG->SNS;
		for(int i=0;i<n;i++,id++,sn++){
			if(*id==ID&&*sn==SN) return i;
		}
	}
	return 0xFFFF;
};
//
int FindUnits(int x,int y,int r,int Type,byte Nation,int Need,word* Dest);
//
void FillFormation(Brigade* BR){
	word ULOCAL[512];
	int N1=0;
	int N2=0;
	int xx=0;
	int yy=0;
	bool SELECTED=0;
	bool ImSELECTED=0;
	byte NI=BR->CT->NI;
	bool ArmAttack=1;
	bool RifleAttack=1;
	int NN=ElementaryOrders[BR->WarType-1].NUnits+NBPERSONAL;
	addrand(BR->Memb[0]);
	addrand(BR->Memb[1]);
	addrand(BR->Memb[2]);
	for(int i=NBPERSONAL;i<NN;i++){
		addrand(BR->Memb[i]);
		if(BR->Memb[i]==0xFFFF)N1++;
		else{
			OneObject* OB=Group[BR->Memb[i]];
			if(OB&&!OB->Sdoxlo){
				xx+=OB->RealX;
				yy+=OB->RealY;
				N2++;
				if(OB->Selected&GM(NI))SELECTED=1;
				if(OB->ImSelected&GM(NI))ImSELECTED=1;
				ArmAttack=OB->ArmAttack;
				RifleAttack=OB->RifleAttack;
			}else N1++;
		};
	};
	if(!N2)return;
	xx/=N2;
	yy/=N2;
	bool LINEENB[64];
	memset(LINEENB,0,sizeof LINEENB);
	if(BR->WarType){
		OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
		int NA=ODE->NActualLines;
		int NL=0;
		int FL=ODE->FirstActualLine;
		int bp=NBPERSONAL;
		for(int p=0;p<NA;p++){
			int NU=ODE->LineNU[p+FL];
			if(NU){
				for(int q=0;q<NU;q++){
					if(bp<BR->NMemb){
						word ID=BR->Memb[bp];
						if(ID!=0xFFFF){
							OneObject* OB=Group[ID];
							if(OB&&OB->Serial==BR->MembSN[bp]){
								LINEENB[p]=OB->RifleAttack;
							};
						};
						bp++;
					};
				};
				NL++;
			};
		};
	};
	bool CHANGE=0;	
	if(N1){
		int N=FindUnits(xx,yy,MobilR*16,BR->MembID,BR->CT->NI,N1,ULOCAL);
		N2=0;
		for(i=NBPERSONAL;i<NN&&N;i++){
			bool INS=0;
			if(BR->Memb[i]==0xFFFF)INS=1;
			else{
				OneObject* OB=Group[BR->Memb[i]];
				INS=!(OB&&!OB->Sdoxlo);
			};
			if(INS){
				OneObject* OB=Group[ULOCAL[N2]];
				BR->Memb[i]=ULOCAL[N2];
				BR->MembSN[i]=OB->Serial;
				OB->BrigadeID=BR->ID;
				OB->BrIndex=i;
				OB->InArmy=true;
				OB->AddDamage=BR->AddDamage;
				OB->AddShield=BR->AddShield;
				OB->StandGround=0;
				(&BR->BM.Peons)[GetBMIndex(OB)]++;
				N2++;
				N--;
				CHANGE=1;
				if(EngSettings.MoraleType==1){
					BR->Morale+=10000;
					BR->IncBrigExperience(int(OB->Kills)*100);
				}
				OB->ArmAttack=ArmAttack;
				OB->RifleAttack=RifleAttack;
			};
		};
	};
	if(BR->Memb[0]==0xFFFF&&BR->OfficerID!=0xFFFF&&BR->OfficerID!=BR->BarID){
		int N=FindUnits(xx,yy,MobilR*16,BR->OfficerID,BR->CT->NI,1,ULOCAL);
		if(N){
			OneObject* OB=Group[ULOCAL[0]];
			BR->Memb[0]=ULOCAL[0];
			BR->MembSN[0]=OB->Serial;
			OB->BrigadeID=BR->ID;
			OB->BrIndex=0;
			OB->InArmy=true;
			if(EngSettings.MoraleType!=1){
				OB->AddDamage=BR->AddDamage;
				OB->AddShield=BR->AddShield;
			}
			OB->StandGround=0;
			(&BR->BM.Peons)[GetBMIndex(OB)]++;
			CHANGE=1;
			if(EngSettings.MoraleType==1){
				BR->Morale+=10000;
				BR->IncBrigExperience(int(OB->Kills)*100);
			}
			OB->ArmAttack=ArmAttack;
			OB->RifleAttack=RifleAttack;
			BR->ErasureTime=0;
		};
	};
	if(BR->Memb[1]==0xFFFF&&BR->BarID!=0xFFFF&&BR->OfficerID!=BR->BarID){
		int N=FindUnits(xx,yy,MobilR*16,BR->BarID,BR->CT->NI,1,ULOCAL);
		if(N){
			OneObject* OB=Group[ULOCAL[0]];
			BR->Memb[1]=ULOCAL[0];
			BR->MembSN[1]=OB->Serial;
			OB->BrigadeID=BR->ID;
			OB->BrIndex=1;
			OB->InArmy=true;
			if(EngSettings.MoraleType!=1){
				OB->AddDamage=BR->AddDamage;
				OB->AddShield=BR->AddShield;
			}
			OB->StandGround=0;
			(&BR->BM.Peons)[GetBMIndex(OB)]++;
			CHANGE=1;
			if(EngSettings.MoraleType==1){
				BR->Morale+=10000;
				BR->IncBrigExperience(int(OB->Kills)*100);
			}
			OB->ArmAttack=ArmAttack;
			OB->RifleAttack=RifleAttack;
		};
	};
	if(BR->Memb[2]==0xFFFF&&BR->FlagID!=0xFFFF){
		int N=FindUnits(xx,yy,MobilR*16,BR->FlagID,BR->CT->NI,1,ULOCAL);
		if(N){
			OneObject* OB=Group[ULOCAL[0]];
			BR->Memb[2]=ULOCAL[0];
			BR->MembSN[2]=OB->Serial;
			OB->BrigadeID=BR->ID;
			OB->BrIndex=2;
			OB->InArmy=true;
			if(EngSettings.MoraleType!=1){
				OB->AddDamage=BR->AddDamage;
				OB->AddShield=BR->AddShield;
			}
			OB->StandGround=0;
			(&BR->BM.Peons)[GetBMIndex(OB)]++;
			CHANGE=1;
			if(EngSettings.MoraleType==1){
				BR->Morale+=10000;
				BR->IncBrigExperience(int(OB->Kills)*100);
			}
			OB->ArmAttack=ArmAttack;
			OB->RifleAttack=RifleAttack;
		};
	};
	if(CHANGE){
		BR->CreateOrderedPositions(xx,yy,BR->Direction);
		addrand(128+16);
		BR->KeepPositions(0,128+16);
		BR->NMemb=NN;
	};
	if(BR->WarType){
		OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
		int NA=ODE->NActualLines;
		int NL=0;
		int FL=ODE->FirstActualLine;
		int bp=NBPERSONAL;
		for(int p=0;p<NA;p++){
			int NU=ODE->LineNU[p+FL];
			if(NU){
				for(int q=0;q<NU;q++){
					if(bp<BR->NMemb){
						word ID=BR->Memb[bp];
						if(ID!=0xFFFF){
							OneObject* OB=Group[ID];
							if(OB&&OB->Serial==BR->MembSN[bp]){
								OB->RifleAttack=LINEENB[p];
								addrand(OB->Index);
							};
						};
						bp++;
					};
				};
				NL++;
			};
		};
	};
	if(CHANGE){
		byte Mask=BR->GetNMask();
		BR->SetNMask(Mask);
	}
};
void DoFillFormation(byte NI,word BrigadeID){
	Brigade* BR=CITY[GSets.CGame.cgi_NatRefTBL[NI]].Brigs+BrigadeID;
	if(BR->WarType)FillFormation(BR);
};
DLLEXPORT
bool FillFormationFromGroup(byte BrigNI, word BrigID, GAMEOBJ* Source, GAMEOBJ* Dest=NULL){
	
	const RB=120;
	word bNIndex=0xFFFF;
	UnitsGroup* RG=GetUnitsGroup(Source);
	int nrg=RG->N;
	if(nrg==0) return false;

	int N;
	word *SN,*ID;
	GetBrGroup(BrigNI,BrigID,&N,&ID,&SN);

	/*
	// detect NIndex of brig
	for(int i=3;i<N;i++){
		OneObject* OB=GetOBJ(ID[i]);
		if(OB&&OB->Serial==SN[i]&&!OB->Sdoxlo){
			bNIndex=OB->NIndex;
			break;
		}
	}
	UnitsGroup* RG=GetUnitsGroup(Source);
	int nrg=RG->N;
	if(bNIndex==0xFFFF){
		for(int i=0;i<nrg;i++){
			OneObject* OB=GetOBJ(RG->IDS[i]);
			if(OB&&OB->Serial==RG->SNS[i]&&!OB->Sdoxlo){
				bNIndex=OB->NIndex;
				break;
			}
		}
	}
	*/

	//if(bNIndex!=0xFFFF)
	Brigade* BR=CITY[BrigNI].Brigs+BrigID;
	if(BR->Enabled){
		int dfposx[RB];		// positions x
		int dfposy[RB];
		int bpos[RB];
		int bnpos=0;
		int cx,cy;
		int bfree=GetFreePlacesInBrig(BrigNI,BrigID,RB,dfposx,dfposy,bpos,cx,cy);
		if(bfree){
			for(int i=0;i<nrg;i++){
				word uid=RG->IDS[i];
				addrand(uid);
				OneObject* OB=GetOBJ(uid);
				if(OB&&OB->Serial==RG->SNS[i]&&!(OB->Sdoxlo||OB->UnlimitedMotion||OB->InArmy)&&OB->NIndex==BR->MembID){
					for(int j=0;j<bfree;j++){
						//if(bfree>0&&bnpos<RB){
							int bridx=bpos[j];
							if(ID[bridx]==0xFFFF){
								OBJ_ChangeNation(OB,BrigNI);
								ID[bridx]=uid;
								SN[bridx]=OB->Serial;
								OB->BrigadeID=BrigID;
								OB->BrIndex=bridx;
								OB->InArmy=1;
								addrand(OB->BrIndex);
								addrand(OB->BrigadeID);
								OBJ_PreciseSendTo(uid,dfposx[j],dfposy[j],128+16,0);
								byte dir;
								if(GetBrDir(BrigNI,BrigID,dir)){	
									OBJ_Rotate(uid,dir,2);
								}										
								GetBrAddDS(BrigNI,BrigID,OB->AddDamage,OB->AddShield);
								SetOBRifleAttack(OB,0);
								//bnpos++;
								UnitsGroup* DG=GetUnitsGroup(Dest);
								if(DG){									
									DG->AddNewUnit(OB);
									addrand(OB->Index);
									addrand(OB->BrIndex);
								}
								RG->IDS[i]=0xFFFF;
								//bfree--;							
								break;
							}
						//}
					}
				}
			}
			return true;
		}		
	}
	return false;
}

CEXPORT
char* GetMonsterMD(int NIndex){
	return NATIONS->Mon[NIndex]->newMons->MD_File;
}
CEXPORT
char* GetMonsterID(int NIndex){
	return NATIONS->Mon[NIndex]->MonsterID;
}

CEXPORT void PlayerStatistic_AddTotalRes(byte NI, byte ResType, int Amount){
	if(NI<8&&ResType<6&&Amount>0){
		NATIONS[NI].ResTotal[ResType]+=Amount;
	}else{
		//VitalError("PlayerStatistic_AddTotalRes");
	}
}
CEXPORT void PlayerStatistic_AddOnUpgradeRes(byte NI, byte ResType, int Amount){
	if(NI<8&&ResType<6&&Amount>0){
		NATIONS[NI].ResOnUpgrade[ResType]+=Amount;
	}else{
		VitalError("PlayerStatistic_AddOnUpgradeRes");
	}
}

// род войск юнита: 0 - пехота, 1 - конница, 2 - пушки ...
CEXPORT
word GetObjectBranch(char* NDS_Name){
	GAMEOBJ Type;
	if(RegisterUnitType(&Type,NDS_Name)){
		return NATIONS->Mon[Type.Index]->newMons->PortBranch;
	}
	return 0xFFFF;	
}

CEXPORT
word GetObjectBranch(word NIndex){
	if(NIndex<0xFFFE){
		return NATIONS->Mon[NIndex]->newMons->PortBranch;
	}
	return 0xFFFF;
}
void B_GoOnRoadLink(Brigade*);
CEXPORT
void DrawBrigSign(byte NI, word BrID, int x, int y, int GPFile){
	Brigade* Br=CITY[NI].Brigs+BrID;
	if(Br&&Br->Enabled){
		int bx,by;		
		byte dir;
		int s=(GetTickCount()>>7)&31;
		if(s<4&&Br->GetCenter(&bx,&by,&dir)){//8,12,4,16
			dir=(dir+16)&0xE0;
			int i=Br->GetFormIndex();
			int Spr=8;
			switch(i){
				case 0: Spr=8; break;
				case 1: Spr=12; break;
				case 2: Spr=4; break;
			};
			if(Br->BOrder&&Br->BOrder->BLink==B_GoOnRoadLink){
				Spr=16;
			}
			
			bx=x+(bx>>6);//-3;
			by=y+(by>>6);//-6;
			int dx=-13;//6
			int dy=-14;//10
			//DrawRotatedPicture(bx,by,GPFile,Spr/*+1*/,0,dx,dy,(dir*2*c_PI)/255,1.0,1.0);			
			
			//int s=(REALTIME>>0)7;
			//if(s>3) s=7-s;
			Spr=28+3-s;
			DrawPicture(bx+dx,by+dy,GPFile,Spr/*+1*/,0);
		}
	}
}

bool BaseResources::PlayerHasMore(byte Nat){
	for(int i=0;i<6;i++){
		if(XRESRC(Nat,i)<(*this)[i])return false;
	}
	return true;		
};

bool BaseResources::PlayerHasMorePerc(byte Nat, byte Percentage){
	for(int i=0;i<6;i++){
		if(XRESRC(Nat,i)*Percentage/100<(*this)[i])return false;
	}
	return true;		
};

void BaseResources::GetCostString(DString& txt){
	char t[128];
	int m[6];
	for(int i=0;i<6;i++) m[i]=(*this)[i];
	::GetCostString(m,t);
	txt+="\\";
	txt+=t;
};

DLLEXPORT int GetTopDist(int,int,int,int);
CEXPORT int GetDistToCenterOfCity(byte Nat, int x, int y){ // in pixels
	if(Nat<8&&x>0&&y>0){
		int l=GetTopDist(CITY[Nat].CenterX<<7,CITY[Nat].CenterY<<7,x,y);
		return l<<6;		
	}
	return 0xFFFF;	
}
CIMPORT void SendDipCommand(char*,int);
CEXPORT void CmdSetOrderedStateForComplexObject(OneObject* OB, int State){
	word Data[4];
	Data[0]=3006;
	Data[1]=OB->Index;
	Data[2]=OB->Serial;
	Data[3]=State;
	SendDipCommand((char*)Data,sizeof(Data));
}
byte g_MissionPulseZone;				    //grey  [12/03/2003]
CEXPORT	void SetMissionPulseZone(byte ID) { //grey  [12/03/2003]
	g_MissionPulseZone = ID;
}
CEXPORT	byte GetMissionPulseZone() {		//grey  [12/03/2003]
	return g_MissionPulseZone;
}

//  [1/6/2004]
//////////////////////////////////////////////////////////////////////////
//-----------------------For creating scripts-----------------------------
CEXPORT	void	missGrpHoldPosition(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("missGrpHoldPosition(Units) : Invalid parameter <Units>");
		SCENINF.NErrors++;
		return;
	};
	TestUnitsGroup(Units->Index);
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	for(int i=0;i<Nu;i++){
		word MID=UIDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->GetLiveState()==ls_LiveBody)&&OB->Serial==SIDS[i]){
				OB->ActivityState = 1;
			};
		};
	};
	return;	
};

CEXPORT	void	SportRuins(GAMEOBJ* Units){
	if(Units->Type!='UNIT'){
		IntErr("SportRuins(Units) : Invalid parameter <Units>");
		SCENINF.NErrors++;
		return;
	};
	TestUnitsGroup(Units->Index);
	word* UIDS=SCENINF.UGRP[Units->Index].IDS;
	word* SIDS=SCENINF.UGRP[Units->Index].SNS;
	int Nu=SCENINF.UGRP[Units->Index].N;
	for(int i=0;i<Nu;i++){
		word MID=UIDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(OB->Sdoxlo>300)&&OB->Serial==SIDS[i]){
				OB->Sdoxlo = 300;
			}
		};
	};
};

CEXPORT	int		missGetTotalAmount(byte Nat){
	int	Amount = 0;
	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit&&!pUnit->Sdoxlo) {
			if (pUnit->NNUM==Nat) {
				Amount++;
			}
		}
	}
	return	Amount;
};

CEXPORT int		missFogMode(int value){
	int oldValue = FogMode;
	FogMode = value;
	return oldValue;
};

CEXPORT	void	SetPlayGameMode(int p){
	GSets.CGame.ViewMask = p;
}
void SetIllusion(OneObject* OBJ,bool State);
void SetInvisiblen(OneObject* OBJ,bool State);
CEXPORT	void	FreezeGame(){
	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit) {
			//pUnit->Illusion = true;
			SetIllusion(pUnit,true);
		}
	}
}

CEXPORT	void	UnFreezeGame(){
	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit) {
			//pUnit->Illusion = false;
			SetIllusion(pUnit,false);
		}
	}
}

CEXPORT	void	FreezeAndHiddenGame(bool doma=false){
	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit) {
			if (pUnit->NewBuilding==false) {
				//pUnit->Invisible = true;
				SetInvisiblen(pUnit,true);
			}else if (doma) {
				//pUnit->Invisible = true;
				SetInvisiblen(pUnit,true);
			}

			//pUnit->Illusion = true;
			SetIllusion(pUnit,true);
		}
	}
}

CEXPORT	void	UnFreezeAndUnHiddenGame(bool doma=false){
	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit) {
			//pUnit->Invisible = false;
			SetInvisiblen(pUnit,false);
			//pUnit->Illusion = false;
			SetIllusion(pUnit,false);
		}
	}
}

CEXPORT	void	FreezeAndHiddenGame(GAMEOBJ* pZone, bool doma=false){
	if (pZone==NULL) {
		return;
	};
	int	zR = 0;
	GetZoneR(pZone,&zR);
	int x,y;
	if (pZone->Type=='ZONE') {
		GetZoneCoor(pZone,&x,&y);
	}else{
		x = pZone->Index;
		y = pZone->Serial;
	}

	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit&&(Norma((pUnit->RealX>>4)-x,(pUnit->RealY>>4)-y)<=zR-100)) {
			if (pUnit->NewBuilding==false) {
				//pUnit->Invisible = true;
				SetInvisiblen(pUnit,true);
			}else if (doma) {
				//pUnit->Invisible = true;
				SetInvisiblen(pUnit,true);
			}

			//pUnit->Illusion = true;
			SetIllusion(pUnit,true);
		}
	}
}

CEXPORT	void	UnFreezeAndUnHiddenGame(GAMEOBJ* pZone, bool doma=false){
	if (pZone==NULL) {
		return;
	};

	int	zR = 0;
	GetZoneR(pZone,&zR);
	int x,y;
	if (pZone->Type=='ZONE') {
		GetZoneCoor(pZone,&x,&y);
	}else{
		x = pZone->Index;
		y = pZone->Serial;
	}

	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit&&(Norma((pUnit->RealX>>4)-x,(pUnit->RealY)>>4-y)<=zR+100)) {
			//pUnit->Invisible = false;
			SetInvisiblen(pUnit,true);
			//pUnit->Illusion = false;
			SetIllusion(pUnit,false);
		}
	}
}

CEXPORT	void	GroupVisibility(GAMEOBJ* Units,bool state){
	TestUnitsGroup(Units->Index);
	int Nat=-1;
	UnitsGroup* UG=&SCENINF.UGRP[Units->Index];
	int NU=0;
	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==UG->SNS[i]){
				//OB->Illusion = state;
				SetIllusion(OB,state);
				//OB->Invisible = state;
				SetInvisiblen(OB,state);
			};
		};
	};
}

CEXPORT	void	DrawFillRect(DWORD color, int x0=0,int y0=0, int w=0, int h=0){
	if (x0==0&&y0==0&&w==0&&h==0){
		w = RealLx;
		h = RealLy;
	}
	if ((color&0xFF000000)!=0) {
		GPS.DrawFillRect((float)x0,(float)y0,(float)w,(float)h,color);
	}
};

CEXPORT	void	missDrawLine(DWORD color, float x1, float y1, float x2, float y2){
	Vector3D	begin(x1,y1,(float)GetHeight((int)x1,(int)y1));
	Vector3D	end(x2,y2,(float)GetHeight((int)x2,(int)y2));
	GPS.DrawLine(begin,end,color);
};

CEXPORT	void	ShowStringMiss(int x,int y,char* lps,int maxLx){
//	ShowString(x, y, lps, &BlackFont);
	int mLx = maxLx;
	DrawMultilineText(x,y,lps,&BlackFont,mLx,mLx,0,true);
}

CEXPORT	void	GetDefinition(int& x, int& y){
	x = RealLx;
	y = RealLy;
}

CEXPORT	void	GetMapXY(int& x,int& y){
	x = mapx;
	y = mapy;
}

CEXPORT	void	SetMapXY(int x, int y){
	mapx = x; 
	mapy = y;
}

CEXPORT	int		GetSpriteWidth(int h,int s){
	return GPS.GetGPWidth(h,s);
}

CEXPORT	int		GetSpriteHeight(int h,int s){
	return GPS.GetGPHeight(h,s);
}

CEXPORT	void	PlayOGMiss(char* fileName,DWORD dwStream=0){
	ov_Play(fileName,dwStream);
}

CEXPORT	int		GetSelNation(){
	return	MyNation;
};

// ShowClassEditor() /////////////////////////////////////////////////////
extern	void	PreDrawGameProcess();
extern	void	GFieldShow();
extern	void	PostDrawGameProcess();
extern	void	GlobalHandleMouse();
CEXPORT	void	ShowClassEditor(BaseClass* pBC,int x=10,int y=10,int x1=700,int y1=700){
	DialogsSystem	dialog;
	ClassEditor		editor;
	bool			show=true;
	
	//Create window.
	{
	editor.ClearAll();
	editor.CreateFromClass( &dialog, x, y, x1, y1, pBC, CED_MEMBERS|CED_TREE );
	}

	while (show) {
		// Non stop game process /////////////////////////////////////////////////
		PRFUNC(PreDrawGameProcess());
		GlobalHandleMouse();
		GFieldShow();
		PostDrawGameProcess();
		//////////////////////////////////////////////////////////////////////////
		ProcessMessages();
		editor.Process();
		dialog.ProcessDialogs();
		dialog.RefreshView();
		
		if (GetKeyState(VK_ESCAPE)&0x8000) {
			show=false;
		};
	};
};

CEXPORT	void	StopOGMiss(DWORD dwStream=0){
	ov_Stop(dwStream);
}

CEXPORT	void	OGSetVolume(DWORD dwVolume,DWORD dwStream=0){
	ov_SetVolume(dwVolume,dwStream);
}

CEXPORT	bool	OGFinishMiss(DWORD dwStream=0){
	return (ov_StreamFinished(dwStream)!=0);	
}
//////////////////////////////////////////////////////////////////////////
CEXPORT void DeleteOrders(GAMEOBJ* Grp){
	if(Grp->Type=='UNIT'){
		UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];
		word* IDS=UG->IDS;
		word* SNS=UG->SNS;
		int N=UG->N;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i])OB->ClearOrders();
			};
		};
	};
};
OneObject* TransformUnitType(OneObject* OB,int DestType);
void SetBrigadeFlagbearers(Brigade* BR){
	if(BR->WarType){
		OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
        GeneralObject* GO=NATIONS->Mon[BR->MembID];
		if(GO->FlagBearerID!=0xFFFF){
			for(int i=NBPERSONAL;i<BR->NMemb;i++){
				if(ODE->Opt[i-NBPERSONAL]==2){//place for flagbearer
                    word MID=BR->Memb[i];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB&&OB->Serial==BR->MembSN[i]){
							TransformUnitType(OB,rando()<16384?GO->FlagBearerID:GO->FlagBearerID2);
						}
					}
				}
			}
		}
	}
}
void ClearBrigadeFlagbearers(Brigade* BR){
	if(BR->WarType){
		OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
		GeneralObject* GO=NATIONS->Mon[BR->MembID];
		if(GO->FlagBearerID!=0xFFFF){
			for(int i=NBPERSONAL;i<BR->NMemb;i++){				
				word MID=BR->Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==BR->MembSN[i]&&OB->NIndex!=BR->MembID){
						TransformUnitType(OB,BR->MembID);
					}
				}
			}
		}
	}
}
// create formation function
DLLEXPORT bool CreateFormation(byte NI,GAMEOBJ* Officers,GAMEOBJ* Drummers,GAMEOBJ* FlagBearers,GAMEOBJ* Soldiers,GAMEOBJ* DestGroup,char* FormID){
	if(Officers->Type=='UNIT'&&Drummers->Type=='UNIT'&&FlagBearers->Type=='UNIT'&&DestGroup->Type=='UNIT'){
		CleanGroup(Officers);
		CleanGroup(Drummers);
		CleanGroup(FlagBearers);
		CleanGroup(Soldiers);
		for(int i=0;i<NEOrders;i++){
			OrderDescription* ODS=ElementaryOrders+i;
			if(!strcmp(ODS->ID,FormID)){
				EraseGroup(DestGroup);
				if(GetNUnits(Soldiers)>=ODS->NUnits){
					CopyUnits(Officers,DestGroup,0,1,1,1);
					if(GetNUnits(DestGroup)==1){
						CopyUnits(Drummers,DestGroup,0,1,1,1);
						if(GetNUnits(DestGroup)==2){
							CopyUnits(Soldiers,DestGroup,0,ODS->NUnits,1,1);
							if(GetNUnits(DestGroup)==ODS->NUnits+2){
								CopyUnits(FlagBearers,DestGroup,0,1,1,1);
								bool HaveFlagBearer=GetNUnits(DestGroup)==ODS->NUnits+3;
								UnitsGroup* UG=&SCENINF.UGRP[DestGroup->Index];
								int BID=CITY[NI].GetFreeBrigade();
								if(BID!=-1){
									Brigade* BR=CITY[NI].Brigs+BID;
									BR->Enabled=1;
									bool R=BR->CreateFromGroup(UG,i,0);
									if(R){
										ext_OnBrigadeCreated(BR);
									}									
									return R;
								};
							}else{
								CopyUnits(DestGroup,Soldiers,2,ODS->NUnits,1,1);
							};
						}else{
							CopyUnits(DestGroup,Officers,0,1,1,1);
						};
					};
				};				
				return false;
			};
		};
	};	
	return false;
};

CEXPORT bool CreateBrigade(GAMEOBJ* DstGrp,GAMEOBJ* OficerType,GAMEOBJ* SoldierType,byte FormID,byte AmountID,byte NI,GAMEOBJ* Zone,byte Direction){
	if(/*DstGrp->Type!='UNIT'||*/OficerType->Type!='UTYP'||SoldierType->Type!='UTYP'||NI>7) return false;
	Nation* NAT=NATIONS+NI;
	GeneralObject* OGO=NAT->Mon[OficerType->Index];
	OfficerRecord* OR=OGO->OFCR;
	if(!OR||FormID>=OR->NStroi) return false;
	StroiDescription* SD=OR->SDES+FormID;

	bool find=0;
	for(int i=0;i<SD->NUnits;i++){
		if(SD->Units[i]==SoldierType->Index){
			find=1;
			break;
		}
	}
	if(!find) return false;

	int xc,yc;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
	}else
		if(Zone->Type!='ZONE'){
			IntErr("CreateObject(DstObj,Formation,UnitType,Nation,Zone,Direction) : Invalid parameter <Zone>");
			SCENINF.NErrors++;
			return false;
		}else{
			ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
			xc=AZ->x<<4;
			yc=AZ->y<<4;
		};

		//squad
		if(AmountID>=SD->NAmount) return false;
		GAMEOBJ Form={SD->LocalID[AmountID],0,'FORM'};	
		if(!CreateObject0(DstGrp,&Form,SoldierType,NI,Zone,Direction)) return false;

		UnitsGroup* UG=&SCENINF.UGRP[DstGrp->Index];

		int ofc=0;

		// oficer
		int OID=CreateNewTerrMons2(NI,xc,yc,OficerType->Index);
		if(OID!=-1){
			ofc++;
		}

		int BID=-1;
		if(OR->BarabanID!=0xFFFF){
			BID=CreateNewTerrMons2(NI,xc,yc,OR->BarabanID);
			if(BID!=-1){
				ofc++;
			}
		}

		if(OR->FlagID!=0xFFFF){		
			int FID=CreateNewTerrMons2(NI,xc,yc,OR->FlagID);
			if(FID!=-1) AddOneUnitToGroup(DstGrp,Group[FID]);
		}

		if(ofc){		
			UG->Allocate(UG->N+ofc);
			memmove(UG->IDS+ofc,UG->IDS,UG->N<<1);
			memmove(UG->SNS+ofc,UG->SNS,UG->N<<1);
			UG->N+=ofc;
			if(BID!=-1){
				ofc--;
				UG->IDS[ofc]=BID;
				UG->SNS[ofc]=Group[BID]->Serial;			
			}
			if(OID!=-1){
				ofc--;
				UG->IDS[ofc]=OID;
				UG->SNS[ofc]=Group[OID]->Serial;
			}		
		}

		int BrID=CITY[NI].GetFreeBrigade();
		if(BrID!=-1){
			Brigade* BR=CITY[NI].Brigs+BrID;
			BR->Enabled=1;
			if(BR->CreateFromGroup(UG,Form.Index,Direction)){
				BR->CreateOrderedPositions(xc,yc,Direction);
				addrand(128+16);
				BR->KeepPositions(0,128+16);
				ext_OnBrigadeCreated(BR);
				return true;
			}else{
				BR->Enabled=0;
			}
		};

		return false;
}

int Norma(int,int);
bool GetOfficersType(byte NI, word UnitType, word &OffID, word &BarabanID, word &FlagID);
CEXPORT int CreateBrigInZone(byte NI, int x, int y, word OfType, word SlType, byte Amount){
	if(NI>7||x<0||y<0||SlType==0xFFFF) return -2;

	if(OfType==0xFFFF){
		word b,f;
		if(!GetOfficersType(NI,SlType,OfType,b,f)) return -2;		
	}
    
	addrand(NI);
	addrand(x);
	addrand(y);
	addrand(OfType);
	addrand(SlType);
	addrand(Amount);

	GeneralObject* OGO=NATIONS[NI].Mon[OfType];
	GeneralObject* SGO=NATIONS[NI].Mon[SlType];
	if(!(OGO&&SGO))return -3;

	OfficerRecord* SD=OGO->OFCR;	

	if(Amount>=SD->SDES->NAmount) return -2;

	int NU=SD->SDES[0].Amount[Amount]+3;	
	LogBattle(NI,"^ST_CREATEFRM^ %d %s",SD->SDES[0].Amount[Amount],GetSt_UName(SlType));
	if(OfType==SlType){
		OfType=0xFFFF;
	}
	UnitsGroup UG;	
	UG.N=2;
	UG.Allocate(NU);
	UG.IDS[0]=0xFFFF;
	UG.IDS[1]=0xFFFF;
	UG.IDS[NU-1]=0xFFFF;

	int N=NtNUnits[NI];
	word* List=NatList[NI];	

	//  собираем 
	for(int i=0;i<N;i++){
		OneObject* OB=Group[List[i]];
		if(OB&&OB->Mobilised&&!(OB->Sdoxlo||OB->InArmy||OB->UnlimitedMotion||OB->LockedInBuilding!=0xFFFF)/*&&Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y)<1500*/){
			addname(OB->Index);
			if(OB->NIndex==OfType&&UG.IDS[0]==0xFFFF){
				UG.IDS[0]=OB->Index;
				UG.SNS[0]=OB->Serial;
			}else
			if(OB->NIndex==SD->BarabanID&&UG.IDS[1]==0xFFFF){
				UG.IDS[1]=OB->Index;
				UG.SNS[1]=OB->Serial;
			}else
			if(OB->NIndex==SD->FlagID&&UG.IDS[NU-1]==0xFFFF){
				UG.IDS[NU-1]=OB->Index;
				UG.SNS[NU-1]=OB->Serial;
			}else
			if(OB->NIndex==SlType&&UG.N<NU-1){
				UG.IDS[UG.N]=OB->Index;
				UG.SNS[UG.N]=OB->Serial;
				UG.N++;
			}
		}
	}
	if(UG.N<NU-1) return -4;
	UG.N=NU;

	// Alexander correction
	N=0;
	for(i=0;i<2;i++){
		if(UG.IDS[i]==0xFFFF){
			if(i==0&&OfType==0xFFFF) continue;
			N++;
			int sz=(1-i)<<1;
			if(sz){
				memcpy(UG.IDS+i,UG.IDS+i+1,sz);
				memcpy(UG.SNS+i,UG.SNS+i+1,sz);
			}
		}
	}
	if(N){
		for(i=2-N;i<2;i++){
			UG.IDS[i]=UG.IDS[UG.N+i-3];
			UG.SNS[i]=UG.SNS[UG.N+i-3];
		}
		if(UG.IDS[NU-1]!=0xFFFF){
			UG.IDS[UG.N-N-1]=UG.IDS[NU-1];
			UG.SNS[UG.N-N-1]=UG.SNS[NU-1];
		}
		UG.N-=N;
	}
	if(UG.IDS[NU-1]==0xFFFF) UG.N--;

	// Create
	addrand(87);
	int BID=CITY[NI].GetFreeBrigade();
	addrand(93);
	if(BID!=-1){
		Brigade* BR=CITY[NI].Brigs+BID;
		BR->CreateFromGroup(&UG,SD->SDES[0].LocalID[Amount],0);
		BR->Enabled=1;
		ext_OnBrigadeCreated(BR);		
	}
	return BID;

};
CEXPORT int CreateBrigInZone(byte NI, int x, int y, word OfType, word SlType){
	return CreateBrigInZone(NI,x,y,OfType,SlType,0);
}

CEXPORT int CreateBrig(byte NI, UnitsGroup* Source, UnitsGroup* Dest, word UnitNIndex){
	if(NI>7||UnitNIndex==0xFFFF){
		return -2;
	}
	//word OfType, word SlType, byte Amount
	word OfType=0xFFFF;
	if(OfType==0xFFFF){
		word b,f;
		if(!GetOfficersType(NI,UnitNIndex,OfType,b,f)){
			return -3;
		}
	}

	GeneralObject* OGO=NATIONS[NI].Mon[OfType];
	GeneralObject* SGO=NATIONS[NI].Mon[UnitNIndex];
	if(!(OGO&&SGO)){
		return -4;
	}
	OfficerRecord* SD=OGO->OFCR;

	// find amount
	UnitsGroup UG;
	//UG.N=2;
	//UG.Allocate(2);
	//UG.IDS[0]=0xFFFF;
	//UG.IDS[1]=0xFFFF;	
	Source->RemoveUnits(&UG,1000,UnitNIndex);	
	//UG.IDS[NU-1]=0xFFFF;
	int Amount=-1;
	int N=0;
	for(int i=0;i<SD->SDES[0].NAmount;i++){		
		int A=SD->SDES[0].Amount[i];
		if(UG.N>=A&&A>N){ //UG.N-2
			N=A;
			Amount=i;
		}
	}
	if(Amount==-1){
		UG.CopyUnits(Source,0,UG.N,true,false);
		return -5;
	}
	N=SD->SDES->Amount[Amount];
	Source->RefreshUnits();
	UG.CopyUnits(Source,N,UG.N,true,true); //+2
	//UG.IDS[UG.N]=0xFFFF;
	//UG.N++;
	/*
	int NU=N+3;
	if(OfType==UnitNIndex){
		OfType=0xFFFF;
	}
	*/
	/*
	int N=NtNUnits[NI];
	word* List=NatList[NI];	
	//  собираем 	
	for(int i=0;i<N;i++){
		OneObject* OB=Group[List[i]];
		if(OB&&!(OB->Sdoxlo||OB->InArmy||OB->UnlimitedMotion)&&Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y)<1500){
			if(OB->NIndex==OfType&&UG.IDS[0]==0xFFFF){
				UG.IDS[0]=OB->Index;
				UG.SNS[0]=OB->Serial;
			}else
				if(OB->NIndex==SD->BarabanID&&UG.IDS[1]==0xFFFF){
					UG.IDS[1]=OB->Index;
					UG.SNS[1]=OB->Serial;
				}else
					if(OB->NIndex==SD->FlagID&&UG.IDS[NU-1]==0xFFFF){
						UG.IDS[NU-1]=OB->Index;
						UG.SNS[NU-1]=OB->Serial;
					}else
						if(OB->NIndex==SlType&&UG.N<NU-1){
							UG.IDS[UG.N]=OB->Index;
							UG.SNS[UG.N]=OB->Serial;
							UG.N++;
						}
						OB->RifleAttack=0;
		}
	}
	if(UG.N<NU-1) return -4;
	UG.N=NU;
	*/
	// Alexander correction
	/*
	N=0;
	for(i=0;i<2;i++){
		if(UG.IDS[i]==0xFFFF){
			if(i==0&&OfType==0xFFFF) continue;
			N++;
			int sz=(1-i)<<1;
			if(sz){
				memcpy(UG.IDS+i,UG.IDS+i+1,sz);
				memcpy(UG.SNS+i,UG.SNS+i+1,sz);
			}
		}
	}
	if(N){
		for(i=2-N;i<2;i++){
			UG.IDS[i]=UG.IDS[UG.N+i-3];
			UG.SNS[i]=UG.SNS[UG.N+i-3];
		}
		if(UG.IDS[NU-1]!=0xFFFF){
			UG.IDS[UG.N-N-1]=UG.IDS[NU-1];
			UG.SNS[UG.N-N-1]=UG.SNS[NU-1];
		}
		UG.N-=N;
	}
	if(UG.IDS[NU-1]==0xFFFF) UG.N--;
	*/
	UG.IDS[UG.N]=UG.IDS[0];		UG.SNS[UG.N]=UG.SNS[0];
	//UG.IDS[UG.N+1]=UG.IDS[1];	UG.SNS[U	G.N+1]=UG.SNS[1];
	UG.IDS[0]=0xFFFF;
	//UG.IDS[1]=0xFFFF;
	//UG.IDS[UG.N+2]=0xFFFF;
	UG.N+=1;

	// Create
	int BID=CITY[NI].GetFreeBrigade();
	if(BID!=-1){
		Brigade* BR=CITY[NI].Brigs+BID;
		UG.CopyUnits(Dest,0,UG.N,true,false);
		if(!BR->CreateFromGroup(&UG,SD->SDES[0].LocalID[Amount],0)){
			UG.CopyUnits(Source,0,UG.N,true,false);
			return -6;
		}
		BR->Enabled=1;
	}
	return BID;

};
extern MotionField UnitsField;
void ChangeUnitCoor(OneObject* OB,int newX,int newY){	
	bool bl=false;
	if(OB->BlockInfo){
        OB->UnBlockUnit();		
		bl=true;
	}
	UnitsField.BClrBar(OB->x,OB->y,OB->Lx);
	OB->RealX=newX;
	OB->RealY=newY;
	OB->RZ=max(GetTotalHeight(OB->RealX/16,OB->RealY/16),2);
	addrand(OB->RZ);
	addrand(OB->RealX);addrand(OB->RealY);
	OB->x=(OB->RealX-(OB->Lx<<7))>>8;
	OB->y=(OB->RealY-(OB->Lx<<7))>>8;
	if(bl){
		OB->BlockUnit();
	}
	if(!OB->LockedInBuilding)UnitsField.BSetBar(OB->x,OB->y,OB->Lx);
	UnitsField.BClrBar(OB->x,OB->y,OB->Lx);
	OB->DestX=-1;
	if(OB->CObjIndex!=0xFFFF){
		void SetPositionForCOB_ONLY(OneObject* OB,int x,int y,int Dir);
        SetPositionForCOB_ONLY(OB,newX/16,newY/16,OB->RealDir);
	}
}
extern int MiniLx;
extern int MiniLy;
//extern int MinMapX;
//extern int MaxMapX;
//extern int MinMapY;
//extern int MaxMapY;
extern int ExtraDir;
/*
int GetXOnMiniMap(int x,int y){
	float COS=TCos[byte(ExtraDir)]/256.0f;
	int	posX = x>>5;
	if (posX<1)		posX=1;
	if (posX>msx)	posX=msx;
	double	newX = ((double)(posX-MinMapX))/((double)(MaxMapX-MinMapX));	
	return	minix+L+(int)((double)MiniLx*newX);
}
int GetYOnMiniMap(int x,int y){
	int	posY = y>>5;
	if (posY<1)		posY=1;
	if (posY>msy)	posY=msy;
	double	newY = ((double)(posY-MinMapY))/((double)(MaxMapY-MinMapY));
	return	miniy+(int)((double)MiniLy*newY);
}
*/
float resX,resY;
CEXPORT int GetXOnMiniMap(int x,int y){
	//int z=GetTotalHeight(x,y);
	//if(z<0)z=0;
	//y-=z*2;
	float COS=TCos[byte(ExtraDir)]/256.0f;
	float SIN=-TSin[byte(ExtraDir)]/256.0f;
	float ss=1.0f/(fabs(COS)+fabs(SIN));
	COS*=ss;
	SIN*=ss;
	float LX=MiniLx/2.0f;
	float LY=MiniLy/2.0f;


	int	posX = x>>5;
//	if(posX < 0) {
//		posX = 0; // or 1?
//	}
//	if (posX>msx)	posX=msx;

	const RECT rc = MOptions.LimitCamArgs.GetRect();
//	double	newX = ((double)(posX-MinMapX))/((double)(MaxMapX-MinMapX));
	double	newX = ((double)(posX-rc.left))/((double)(rc.right-rc.left));
	int	posY = y>>5;
//	if(posY < 0) {
//		posY = 0; // or 1?
//	}
//	if (posY>msy)	posY=msy;
//	double	newY = ((double)(posY-MinMapY))/((double)(MaxMapY-MinMapY));
	double	newY = ((double)(posY-rc.top))/((double)(rc.bottom-rc.top));

	resX=minix+LX+((newX-0.5)*COS-(newY-0.5)*SIN)*LX*2;
	resY=miniy+LY+((newX-0.5)*SIN+(newY-0.5)*COS)*LY*2;

	return resX;
}
int GetYOnMiniMap(int x,int y){
	return resY;
}
void Mini2World(int& x,int& y){
	float COS=TCos[byte(ExtraDir)]/256.0f;
	float SIN=TSin[byte(ExtraDir)]/256.0f;
	float ss=1.0f/(fabs(COS)+fabs(SIN));
	COS/=ss;
	SIN/=ss;
	float LX=MiniLx/2.0f;
	float LY=MiniLy/2.0f;
	
	float dx=x-minix-LX;
	float dy=y-miniy-LY;
    float x1=(dx*COS-dy*SIN)/MiniLx+0.5;
	float y1=(dx*SIN+dy*COS)/MiniLy+0.5;
//	x=MinMapX+(MaxMapX-MinMapX)*x1;
//	y=MinMapY+(MaxMapY-MinMapY)*y1;
	const RECT rc = MOptions.LimitCamArgs.GetRect();
	x=rc.left+(rc.right-rc.left)*x1;
	y=rc.top+(rc.bottom-rc.top)*y1;
}
CEXPORT int GetXOnMiniMapEx(int x,int y){
	return GetXOnMiniMap(x,y-GetTotalHeight(x,y));
};
CEXPORT int GetYOnMiniMapEx(int x,int y){
	return GetYOnMiniMap(x,y-GetTotalHeight(x,y));
};
void DrawFillRectOnMiniMap(int x,int y,int x1,int y1,DWORD Color){
	int xm0=GetXOnMiniMap(x,y);
	int ym0=GetYOnMiniMap(x,y);
	int xm1=GetXOnMiniMap(x1,y1);
	int ym1=GetYOnMiniMap(x1,y1);
	if(xm1<xm0)swap(xm0,xm1);
	if(ym1<ym0)swap(ym0,ym1);
    GPS.DrawFillRect(xm0,ym0,xm1-xm0+1,ym1-ym0+1,Color);
}
void DrawFillRectOnMiniMap(int x,int y,int x1,int y1,DWORD Color1,DWORD Color2,DWORD Color3,DWORD Color4){
	int xm0=GetXOnMiniMap(x,y);
	int ym0=GetYOnMiniMap(x,y);
	int xm1=GetXOnMiniMap(x1,y1);
	int ym1=GetYOnMiniMap(x1,y1);
	if(xm1<xm0)swap(xm0,xm1);
	if(ym1<ym0)swap(ym0,ym1);
	GPS.DrawFillRect(xm0,ym0,xm1-xm0+1,ym1-ym0+1,Color1,Color2,Color3,Color4);
}
void DrawRectOnMiniMap(int x,int y,int x1,int y1,DWORD Color){
	int xm0=GetXOnMiniMap(x,y);
	int ym0=GetYOnMiniMap(x,y);
	int xm1=GetXOnMiniMap(x1,y1);
	int ym1=GetYOnMiniMap(x1,y1);
	if(xm1<xm0)swap(xm0,xm1);
	if(ym1<ym0)swap(ym0,ym1);
	GPS.DrawRect(xm0,ym0,xm1-xm0+1,ym1-ym0+1,Color);
}
void DrawLineOnMiniMap(int x,int y,int x1,int y1,DWORD Color){
	int xm0=GetXOnMiniMap(x,y);
	int ym0=GetYOnMiniMap(x,y);
	int xm1=GetXOnMiniMap(x1,y1);
	int ym1=GetYOnMiniMap(x1,y1);
	GPS.DrawLine(xm0,ym0,xm1-xm0+1,ym1-ym0+1,Color);
}
void DrawCircleOnMiniMap(int x,int y,int R,DWORD Color){
	    
}
CEXPORT
void CopyBrigadeUnitsToGroup(byte BrigNI, word BrigID, UnitsGroup* UG){
	if(!UG) return;
	Brigade* BR=CITY[BrigNI].Brigs+BrigID;
	if(BR->Enabled){
		word* id=BR->Memb;
		word* sn=BR->MembSN;
		for(int i=0;i<BR->NMemb;i++){
			if(id[i]!=0xFFFF){
				OneObject* OB=Group[id[i]];
				if(OB&&OB->Serial==sn[i]){
					UG->AddNewUnit(OB);
				}
			}			
		}
	}
}
CEXPORT
void RemoveNotBrigadesUnit(UnitsGroup* Source, UnitsGroup* Dest){
	if(!Source) return;
	word* id=Source->IDS;
	word* sn=Source->SNS;
	for(int i=0;i<Source->N;i++){
		OneObject* OB=Group[id[i]];
		if(OB&&OB->Serial==sn[i]&&!OB->InArmy){
			id[i]=0xFFFF;
			if(Dest)Dest->AddNewUnit(OB);
		}
	}
}
void EraseObject(OneObject* OB);
CEXPORT void EraseObjIfOutOfScreen(int Index){
	if(Index>0&&Index<MAXOBJECT){
		OneObject* OB=Group[Index];
		if(OB){
			int x=OB->RealX>>4;
			int y=OB->RealY>>4;
			Vector3D v = SkewPt(x,y,GetTotalHeight(x,y));
			WorldToScreenSpace(v);
			//!IMM->IsPointVisible(v)
			if(!(v.x>-200&&v.y>-200&&v.x<RealLx+200&&v.y<RealLy+200)){
				EraseObject(OB);
			}
		}
	}
}
CEXPORT void CmdEraseObjIfOutOfScreen(int Index){
	if(Index>0&&Index<MAXOBJECT){
		OneObject* OB=Group[Index];
		if(OB){
			int x=OB->RealX>>4;
			int y=OB->RealY>>4;
			Vector3D v = SkewPt(x,y,GetTotalHeight(x,y));
			WorldToScreenSpace(v);			
			if(!(v.x>-200&&v.y>-200&&v.x<RealLx+200&&v.y<RealLy+200)){
				vui_IS_EraseObject EO;
				EO.Data.ObjID=OB->Index;
				EO.Data.ObjSN=OB->Serial;
				EO.InterfAction();
			}
		}
	}
}
extern int RealAnimTime;
extern bool RealAnimTimeIsRun;
int GetScaledGameTime(){
	return RealAnimTime*40/256;	
}
void RunGameTime(bool State){
	RealAnimTimeIsRun=State;
}