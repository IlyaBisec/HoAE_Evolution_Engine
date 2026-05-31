#include "CommonDip.h"
//#include "Mind.h"
//#include "Economic.h"
//#include "CityPopulation.h"
#include "Script.h"

extern CBattle Battle;
DiplomacySystem DIPS;
//Economic Auto_Economic;
//extern Economic Auto_Economic;
extern CityPopulation CP;

int  DIP_SimpleBuilding::CurIndex=0;
CIMPORT
void ActivateDipDialog(char* request);
CIMPORT
void SendDipCommand(char* Data,int size);

void PerformBattleCommads(char* Data,int size);
void SI_PerformCommands(char* Data,int size);
CEXPORT
void PerformDipCommand(char* Data,int size){
	DIPS.PerformCommand(Data,size);
	PerformBattleCommads(Data,size);
	//Auto_Economic.PerformCommand(Data,size);
	SI_PerformCommands(Data,size);
};

void LoadMindData(byte* ptr,int size);
void SaveMindData(byte** ptr,int* size);
void ClearMindData();

byte* tmpptr=NULL;
#define SAVEAR(field,array,type)\
	tmpptr=(byte*)realloc(tmpptr,sz+4);\
	((int*)(tmpptr+sz))[0]=DIPS.field;\
	sz+=4;\
	if(DIPS.field){\
		tmpptr=(byte*)realloc(tmpptr,sz+DIPS.field*sizeof(type));\
		memcpy(tmpptr+sz,DIPS.array,DIPS.field*sizeof(type));\
		sz+=DIPS.field*sizeof(type);\
	};
CEXPORT
void SaveAllDipData(byte** ptr,int* size){
	int sz=4;
	tmpptr=(byte*)malloc(sz);
	*((int*)tmpptr)=DIPS.NDIPS;
	for(int i=0;i<DIPS.NDIPS;i++){
		int size=DIPS.DIPS[i]->GetSaveData(NULL,0);
		tmpptr=(byte*)realloc(tmpptr,sz+size);
		DIPS.DIPS[i]->GetSaveData(tmpptr+sz,1);
		sz+=size;
	};
	SAVEAR(NWalk,WID,word);
	SAVEAR(NWalk,WSN,word);
	SAVEAR(NFirers,FIRERS,FiringGroup);
	SAVEAR(NKillers,KILLERS,KillersGroup);
	SAVEAR(NTomahawks,TOMAHAWKS,TomahawkGroup);
	SAVEAR(DMSize,DangerMap,short);
	SAVEAR(NZOO,ZOO,ZooGroup);
	SAVEAR(NBZOO,BZOO,ZooBirthZone);
	SAVEAR(NCannons,CANNONS,CannonGroup);
	SAVEAR(NStorms,STORMS,StormGroup);
	tmpptr=(byte*)realloc(tmpptr,sz+sizeof DIPS.StartPopulation);
	memcpy(tmpptr+sz,DIPS.StartPopulation,sizeof DIPS.StartPopulation);
	sz+=sizeof DIPS.StartPopulation;
	SaveMindData(&tmpptr,&sz);
	*size=sz;
	*ptr=tmpptr;
};
CEXPORT
void FreeSaveDipData(byte* ptr){
	if(tmpptr)free(tmpptr);
	tmpptr=NULL;
};
CEXPORT
void ClearAllDipData(){
	CP.Clear();
	for(int i=0;i<DIPS.NDIPS;i++){
		DIPS.DIPS[i]->Free();
		delete(DIPS.DIPS[i]);
	};
	if(DIPS.DIPS)free(DIPS.DIPS);
	DIPS.DIPS=NULL;
	DIPS.NDIPS=0;
	DIPS.BusyPageIndex=-1;
	if(DIPS.FIRERS)free(DIPS.FIRERS);
	if(DIPS.WID)free(DIPS.WID);
	if(DIPS.WSN)free(DIPS.WSN);
	if(DIPS.KILLERS)free(DIPS.KILLERS);
	if(DIPS.TOMAHAWKS)free(DIPS.TOMAHAWKS);
	if(DIPS.DangerMap)free(DIPS.DangerMap);
	if(DIPS.ZOO)free(DIPS.ZOO);
	if(DIPS.BZOO)free(DIPS.BZOO);
	if(DIPS.CANNONS)free(DIPS.CANNONS);
	if(DIPS.STORMS)free(DIPS.STORMS);
	memset(&DIPS,0,sizeof DIPS);
	ClearMindData();
	extern bool gInit;
	extern int stDangTime;
	extern int frDangTime;
	extern int psDangTime;
	gInit=false;
	stDangTime=0;
	frDangTime=0;
	psDangTime=0;
};
#define LOADAR(field,array,type)\
	DIPS.field=((int*)(ptr+sz))[0];\
	sz+=4;\
	if(DIPS.field){\
		DIPS.array=(type*)malloc(DIPS.field*sizeof(type));\
		memcpy(DIPS.array,ptr+sz,DIPS.field*sizeof(type));\
		sz+=DIPS.field*sizeof(type);\
	};
CEXPORT
void LoadAllDipData(byte* ptr,int size){
	ClearAllDipData();
	DIPS.SetCallBacks();
	int sz=4;
	DIPS.NDIPS=((int*)ptr)[0];
	DIPS.DIPS=(BasicDiploRelation**)malloc(DIPS.NDIPS<<2);
	for(int i=0;i<DIPS.NDIPS;i++){
		byte tp=ptr[sz];
		sz++;
		int size=((DWORD*)(ptr+sz))[0];
		sz+=4;
		if(tp==1){
			DIPS.DIPS[i]=new DIP_SimpleBuilding;
			DIPS.DIPS[i]->SetSaveData(ptr+sz,size);
			DIPS.DIPS[i]->DIP=&DIPS;
			sz+=size-5;
		};
	};
	LOADAR(NWalk,WID,word);
	LOADAR(NWalk,WSN,word);
	LOADAR(NFirers,FIRERS,FiringGroup);
	LOADAR(NKillers,KILLERS,KillersGroup);
	LOADAR(NTomahawks,TOMAHAWKS,TomahawkGroup);
	LOADAR(DMSize,DangerMap,short);
	LOADAR(NZOO,ZOO,ZooGroup);
	LOADAR(NBZOO,BZOO,ZooBirthZone);
	LOADAR(NCannons,CANNONS,CannonGroup);
	LOADAR(NStorms,STORMS,StormGroup);
	memcpy(DIPS.StartPopulation,ptr+sz,sizeof DIPS.StartPopulation);
	sz+=sizeof DIPS.StartPopulation;
	LoadMindData(tmpptr,sz);

	if(DIPS.NDIPS){
		if(DIPS.CreateContactsList(1))ActivateDipDialog("LW_file&Dip/tmp.cml");
	};
};
void InitSettlementInterface();
DLLIMPORT void ChangeNatMask(byte Nat,byte Flags);
CEXPORT
void InitDipForThisMap(){
	DIPS.ResearchMap();
	//Auto_Economic.ResearchMap();
	CP.ResearchMap();
	InitSettlementInterface();
	//ChangeNatMask(7,0xFF);
};

CEXPORT
void ProcessDipRelations(){
	//DWORD tick=GetTickCount();
	RunPF(256,"ProcessDipRelations");

	RunPF(257,"DIPS.Process");
	DIPS.Process();
	StopPF(257);

	//Auto_Economic.Process();
	CP.Process();
	RunPF(258,"Player[i].Process");
	//for(int i=0;i<7;i++) Player[i].Process();
	StopPF(258);

	RunPF(259,"Battle.Process");
	Battle.Process();
	StopPF(259);

	StopPF(256);

	//char txt[256];
	//sprintf(txt,"Dip m.sec: %d",GetTickCount()-tick);
	//ShowString(100,100,txt,&fon13y2);
};

void HandleDipRequest(char* com,ParsedRQ* Request,ParsedRQ* Result){
};
void HandleBattleCml(char* com,char** params,int npr,ParsedRQ* Result);
void Handle_dipcomm(char* com,char** params,int npr,ParsedRQ* Result){
	if(!strcmp(com,"dipyes")){
		if(npr){
			int idx=atoi(params[0]);
			int clr=atoi(params[1]);
			BasicDiploRelation* BDR=DIPS.DIPS[idx];
			if(BDR->Owner==7&&clr==GetMyNation()){
				word Data[8];
				Data[0]=1;
				Data[1]=idx;//Index of tribe
				Data[2]=word(clr);//color of player
				Data[3]=atoi(params[2]);//Index of shaman
				SendDipCommand((char*)Data,8);
				DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[Data[1]];
				DString DST;
				DST.ReadFromFile("Dip\\WaitMask.cml");
				DST.Replace("$CONTXT$",GetTextByID("$CONTXT$"));
				DST.Replace("$GPVOG$",DSB->gpPix);
				DST.Replace("$FRAME$",DSB->gpidx);
				DST.Replace("$PROPOSE$",GetTextByID("$LETMETHINK$"));
				DST.WriteToFile("Dip\\tmp.cml");
				Result->Clear();
				Result->Parse("LW_file&Dip/tmp.cml");
				DSB->DIP->BusyPageIndex=-1;
			};
		};
	}else
	if(!strcmp(com,"dipno")){
		int idx=atoi(params[0]);
		int clr=atoi(params[1]);
		BasicDiploRelation* BDR=DIPS.DIPS[idx];
		if(BDR->Owner==7&&clr==GetMyNation()){
			DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[idx];
			DString DST;
			DST.ReadFromFile("Dip\\WaitMask.cml");
			DST.Replace("$CONTXT$",GetTextByID("$CONTXT$"));
			DST.Replace("$GPVOG$",DSB->gpPix);
			DST.Replace("$FRAME$",DSB->gpidx);
			DST.Replace("$PROPOSE$",GetTextByID(DSB->TellNo));	
			DST.WriteToFile("Dip\\tmp.cml");
			Result->Clear();
			Result->Parse("LW_file&Dip/tmp.cml");
			DSB->DIP->BusyPageIndex=-1;
		};
	}else
	if(!strcmp(com,"dipresell")){
		if(npr){
			int idx=atoi(params[0]);
			int clr=atoi(params[1]);
			BasicDiploRelation* BDR=DIPS.DIPS[idx];
			if(clr==GetMyNation()){
				word Data[8];
				Data[0]=2;
				Data[1]=idx;//Index of tribe
				Data[2]=word(clr);//color of player
				Data[3]=atoi(params[2]);//Index of shaman
				SendDipCommand((char*)Data,8);
				DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[Data[1]];
				DString DST;
				DST.ReadFromFile("Dip\\WaitMask.cml");
				DST.Replace("$CONTXT$",GetTextByID("$CONTXT$"));
				DST.Replace("$GPVOG$",DSB->gpPix);
				DST.Replace("$FRAME$",DSB->gpidx);
				DST.Replace("$PROPOSE$",GetTextByID("$LETMETHINK$"));
				DST.WriteToFile("Dip\\tmp.cml");
				Result->Clear();
				Result->Parse("LW_file&Dip/tmp.cml");
				DSB->DIP->BusyPageIndex=-1;
			};
		};
	}else
	if(!strcmp(com,"cnlist")){
		DIPS.CreateContactsList(1);
		Result->Clear();
		Result->Parse("LW_file&Dip/tmp.cml");
		DIPS.BusyPageIndex=-1;
	}else
	if(!strcmp(com,"cnlist1")){
		DIPS.CreateContactsList(0);
		Result->Clear();
		Result->Parse("LW_file&Dip/tmp.cml");
		DIPS.BusyPageIndex=-1;
	}else
	if(!strcmp(com,"showtribe")){
		if(npr){
			int idx=atoi(params[0]);
			if(idx<DIPS.NDIPS){
				DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[idx];
				DSB->CreateAbilityPage();
				GAMEOBJ Zone;
				UnitsCenter(&Zone,&DSB->CentralGroup,512);
				SetStartPoint(&Zone);
				Result->Clear();
				Result->Parse("LW_file&Dip/tmp.cml");
			};
		};
	}else if(!strcmp(com,"perform")){
		if(npr>1){
			int idx=atoi(params[0]);
			int pidx=atoi(params[1]);
			int ni=atoi(params[2]);
			if(idx<DIPS.NDIPS){
				DIP_SimpleBuilding* DSB=((DIP_SimpleBuilding*)DIPS.DIPS[idx]);
				if(ni==DSB->Owner){
					if(DSB->Busy&&DSB->Owner==GetMyNation()){
						DSB->CreateAbilityPage();
						ActivateDipDialog("LW_file&Dip/tmp.cml");
					}else DSB->PerformPossibility(pidx);
				}else{
					if(ni==GetMyNation()){
						DSB->CreateAbilityPage();
						Result->Clear();
						Result->Parse("LW_file&Dip/tmp.cml");
					};
				};
			};
		};
	};
	HandleBattleCml(com,params,npr,Result);
	//Auto_Economic.EconomicDipComm(com,params,npr,Result);
	
};