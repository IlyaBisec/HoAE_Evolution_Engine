#include "stdheader.h"
#include "unitability.h"
#include "UnitsInterface.h"
#include "BE_HEADERS.h"
#include "mp3\\oggvor.h"
#include "HashTop.h"
#include ".\cvi_InGamePanel.h"
#include "MassivePlayer.h"


// GLOBAL - use for define param list for user
DWORD	g_CO_USER_FLAGS = 0x00000010; // for user set 0x00000010

extern float fMapX;
extern float fMapY;

#define BE_MinR	500 
int GetMinR_ByNUnits(int N){
	if (N==1)			return	50;
	if (1<N&&N<=10)		return	150;
	if (10<N&&N<=36)	return	360;
	if (36<N&&N<=72)	return	550;
	if (72<N&&N<=130)	return	600;
						return	800;
};


bool isNotInvisible(OneObject* OB, byte SeekerNMask);
extern	bool		NOPAUSE;
extern	_str		g_vvElementView;
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int				PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
void			SendSelectedToXY(byte NI,int xx,int yy,short Dir,byte Prio,byte Type);
void			MakeReformation(byte NI,word BrigadeID,byte FormType);
void			CopyReIm(byte NI);
void			ImClearSelection(byte Nat);
void			BSetSQ(int x,int y,int Lx,int Ly);		// Set   locks in area (in all layers) (pixel coord / 16)
void			BClrSQ(int x,int y,int Lx,int Ly);		// Clear locks in area (in all layers) (pixel coord / 16)
CEXPORT int vdf_GetAmountOfSettlements(byte Owner,	int ResType, bool CheckUpgrades, int Level); // COSS 2
CEXPORT	  void	StartAIEx(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty);
DLLEXPORT void	ShowVictory(char* Message=NULL, byte NI=0xFF);
DLLEXPORT void	LooseGame(char* Message=NULL, byte NI=0xFF);
CEXPORT	  void	FreezeGame();
CEXPORT	  void	UnFreezeGame();
DLLEXPORT void	ChangeFriends(byte Nat,byte Flags);
void  AddUnitToSelected(byte NI,OneObject* OB);
DLLEXPORT void	ClearSelection(byte Nat);
DLLEXPORT void	AddUnitToSel(int Index);
DLLEXPORT int	GetDiff(byte NI);
DLLEXPORT int	GetResource(byte Nat,byte ID);
DLLEXPORT void	SetResource(byte Nat,byte ID,int Amount);
DLLEXPORT int	GetOrderType(OneObject* OB);
DLLEXPORT void	OBJ_Rotate(word Index,byte Dir,byte OrdType);
DLLEXPORT bool	NationIsErased(byte Nat);
DLLEXPORT void	AddResource(byte Nat,byte ID,int Amount);
DLLEXPORT int	GetBrigadsAmount0(byte NI);
CEXPORT	  int	PutNewFormationBySizeIndex(byte NI, word UnitType, byte SizeIndex, int formT, int x, int y, byte Dir=0);
CEXPORT	  void	SetOrderedStateForComplexObject(OneObject* OB, int State);
bool			DetectArcher(GeneralObject* GO);
bool			DetectShortRangeUnit(GeneralObject* GO);
bool			DetectShootingUnit(GeneralObject* GO);
bool			DetectTamagavkUnit(GeneralObject* GO);
CEXPORT	void SetFreezeDipSysProcess(bool State);
CEXPORT	int GetFormationIndexBySizeIndex(byte NI, word UnitType, byte SizeIndex);
CEXPORT byte GetTribeOwner(char* MainGroup);
CEXPORT void ActivateTacticalAI(byte NI);
double	lvBE_GetVecFi(double x, double y)	//Угол между вектором (x,y) и (1,0); Fi>=0&&Fi<2*Pi
{
	if (x==0&&y==0)		return 0.0;
	
	const double Pi = 3.141592;

	double	dx = x;	if (x<0)	{ dx = -x; };
	double	dy = y;	if (y<0)	{ dy = -y; };


	//x==0||y==0
	if (dx<=0.01&&dy<=0.01)	{ return 0.0;			};
	if (dx<=0.01&&y>0.0)	{ return Pi/2.0;		};
	if (dx<=0.01&&y<0.0)	{ return (3.0*Pi)/2.0;	};
	if (dy<=0.01&&x>0.0)	{ return 0.0;			};
	if (dy<=0.01&&x<0.0)	{ return Pi;			};

	//x<>0&&y<>0
	double	Fi = acos(abs(x)/sqrt(x*x+y*y));

	if (x<0.0&&y>0.0)		{ Fi = Pi-Fi;		};
	if (x<0.0&&y<0.0)		{ Fi = Pi+Fi;		};
	if (x>0.0&&y<0.0)		{ Fi = 2.0*Pi-Fi;	};

	return	Fi;
};
int		lvBE_GetDirec(int x0,int y0){
	double dFi = lvBE_GetVecFi(double(x0),double(y0));
	int Fi = (int)((dFi/(2.0*3.1415))*255.0);
	return Fi;
};	
int		lvBE_GetGrpDir(int grpID,int &x,int &y) //Вычисление направления группы
{
	int Nu=AGroups[grpID]->Units.GetAmount();
	byte bd_;
	if (Nu==0)	return -1;
	while (Nu--) {
		word UID=AGroups[grpID]->Units[Nu].ID;
		if(UID!=0xFFFF){
			OneObject* OB=Group[UID];
			if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==AGroups[grpID]->Units[Nu].SN){
				if (OB->BrigadeID!=0xFFFF){
					Brigade* BR=&CITY[OB->NNUM].Brigs[OB->BrigadeID];
					BR->GetCenter(&x,&y,&bd_);
				};
			};
		};
	};
	return bd_;
};
bool	lvBE_TruePrior(OneObject* OB,int Dir){
	if (GetOrderType(OB)==Dir){
		return true;
	}else{
		return false;
	}
};
bool	lvBE_TrueGrpPrior(int grpID,int Dir,int percent){
	int ret=0;
	int Nu=AGroups[grpID]->Units.GetAmount();
	if (Nu==0)	return false;
	int N=0;
	while (Nu--) {
		word UID=AGroups[grpID]->Units[Nu].ID;
		if(UID!=0xFFFF){
			OneObject* OB=Group[UID];
			if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==AGroups[grpID]->Units[Nu].SN){
				if (!lvBE_TruePrior(OB,Dir)) {
					ret++;
				};
				N++;
			};
		};
	};
	int res=100*ret/N;
	return res<percent;
};
extern	ClassArray<ActiveGroup> AGroups;
bool	lvBE_GrpInZone(int grpID,int x,int y,int R){
	if (AGroups.GetAmount()<grpID||grpID<0)	return false;
	int Nu=AGroups[grpID]->Units.GetAmount();
	if (Nu==0)	return false;
	word MID=0;
	bool	ret = true;
	while (ret&&Nu--) {
		MID=AGroups[grpID]->Units[Nu].ID;
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==AGroups[grpID]->Units[Nu].SN){
				if (Norma(OB->RealX/16-x,OB->RealY/16-y)>R) {
					ret = false;
				};
			};
		};
	};
	return ret;
};

bool	lvBE_GrpFree(int grpID,int percent=0){
	if (AGroups.GetAmount()<grpID||grpID<0)	return false;
	int Nu=AGroups[grpID]->Units.GetAmount();
	if (Nu==0)	return false;
	int NNN = 0;
	word MID=0;
	int	ret = 0;
	while (Nu--) {
		MID=AGroups[grpID]->Units[Nu].ID;
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==AGroups[grpID]->Units[Nu].SN){
				NNN++;
				if (((OB->StandTime>0)&&(!OB->Attack))&&(OB->DestX==-1)) {
					ret++;
				};		
			};
		};
	};
	if (NNN==0)	return false;
	int res=100*ret/NNN;
	return res>(100-percent);
};

void	lvBE_ClearSelection(int grpID){
	if (AGroups.GetAmount()<grpID||grpID<0)	return;
	int Nat=-1;
	int Nu=AGroups[grpID]->Units.GetAmount();
	if (Nu==0)	return;
	while(Nu--){
		word MID=AGroups[grpID]->Units[Nu].ID;
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==AGroups[grpID]->Units[Nu].SN){
				ClearSelection(OB->NNUM);
				addrand(OB->Index);
				return;
			};
		};
	};
};

void	lvBE_SelectGroup(int grpID){	
	if (AGroups.GetAmount()<grpID||grpID<0)	return;
	int Nu=AGroups[grpID]->Units.GetAmount();
	if (Nu==0)	return;
	byte NI=0xFF;
	for(int i=0;i<Nu;i++){
		word MID=AGroups[grpID]->Units[i].ID;
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==AGroups[grpID]->Units[i].SN){
				AddUnitToSel(OB->Index);
				NI=OB->NNUM;				
			};			
		};
	};
	if(NI!=0xFF) CopyReIm(NI);
};

void	lvBE_SelSendGroupToXY(int grpID, int x, int y, int dir, int prior, int ndX=0, int ndY=0){
	if (AGroups.GetAmount()<grpID||grpID<0)	return;
	int Nu=AGroups[grpID]->Units.GetAmount();
	if (Nu==0)	return;
	while(Nu--){
		word MID=AGroups[grpID]->Units[Nu].ID;
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==AGroups[grpID]->Units[Nu].SN){
				if (dir<0) {
					SendSelectedToXY(OB->NNUM,x<<4,y<<4,lvBE_GetDirec(x-ndX,y-ndY),16,prior);
					return;
				}else{
					SendSelectedToXY(OB->NNUM,x<<4,y<<4,dir,16,prior);
					return;
				};
			};
			};
	};
};

void	lvBE_SendGroupToXY(int grpID, int x, int y, int dir, int prior, int ndX=0, int ndY=0){
	lvBE_SelectGroup(grpID);
	lvBE_SelSendGroupToXY(grpID,x,y,dir,prior,ndX,ndY);
};
void	lvBE_TakeResource(word UID,int ResID,int SN){
	if(UID!=0xFFFF){
		OneObject* OB=Group[UID];
		if (OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==SN&&(!OB->LocalOrder)&&OB->newMons->Peasant) {
			OB->TakeResource(OB->RealX>>4,OB->RealY>>4,ResID,128,0);
		};
	};
};
bool	lvBE_GroupInNodeFree(int grpID, int x, int y, bool vGrp){
	if (vGrp) {
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(grpID);
		if (pvGRP) {
			int Nu=pvGRP->GetTotalAmount();
			if (Nu==0) return false;
			int NU;
			NU=pvGRP->GetAmountInZone(x, y, GetMinR_ByNUnits(Nu));
			if ( (Nu==NU) && (pvGRP->GetORDER()==vgNO_ORDERS) ) return true;
			return false;
		}else{
			return false;
		}
	}else{
		if ( AGroups[grpID]!=NULL ) {
			if ( lvBE_GrpInZone(grpID,x,y,BE_MinR) && lvBE_GrpFree(grpID,0) ) return true;
			return false;
		};
		return false;
	};
	return false;
};
bool	lvBE_GroupInNode(int grpID, int x, int y, bool vGrp,int* N=NULL){
	if (vGrp) {
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(grpID);
		if (pvGRP) {
			int Nu=pvGRP->GetTotalAmount();
			if (Nu==0) {
				return false;
			};
			int NU;
			NU=pvGRP->GetAmountInZone(x, y, GetMinR_ByNUnits(Nu));
			if (N!=NULL)	*N=NU;
			if ((Nu==NU)) {
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	}else
		if ((AGroups[grpID]!=NULL)) {
			if (lvBE_GrpInZone(grpID,x,y,BE_MinR)) {
				return true;
			}else{
				return false;
			};
		}else return false;
};
int		GetAmountOfMoversTo(OneObject* Dest,OneObject* Mover=NULL);
// GLOBAL ////////////////////////////////////////////////////////////////
void SetIllusion(OneObject* OBJ,bool State);
void SetInvisiblen(OneObject* OBJ,bool State);
bool	CheckFilePath(char* _FilePath){
	if (_FilePath==NULL)	return false;
	FILE* file = fopen(_FilePath,"r");
	bool ret=(file!=NULL);
	if (file)	fclose(file);
	return ret;
};
bool	CheckFilePath(_str& _FilePath){
	return CheckFilePath(_FilePath.pchar());
};
struct lvSSelParam {
	int NI;
	int SN;
};
struct lvSSum {
	int Sum;
};
struct lvSSumb {
	int Sum;
	bool bHome;
};
struct lvSSumNat {
	int Sum;
	int Nat;
};
struct lvSSumNatType {
	int Sum;
	int Nat;
	int TypeID;
};
struct lvSSumGr {
	int GrpID;
	int Sum;
};
class  lvSSumSquad {
public:
	lvSSumSquad(){};
	~lvSSumSquad(){ arrBrID.Clear(); };
	bool	checkBrID(int BrID){
		if (BrID==0xFFFF)	return false;
		if (Nat<0||Nat>7)	return false;
		Brigade* pBR=&CITY[Nat].Brigs[ BrID ];
		return ( (pBR!=NULL) && (pBR->Enabled==true) );
	};
	bool	addID(OneObject* pOB){ 
		if (pOB==NULL||pOB->Sdoxlo||pOB->NNUM!=Nat)	return false;
		if (checkBrID(pOB->BrigadeID)==false)	return false;
		bool newID=true; 
		int N=getAmount(); 
		while (N--){ 
			if(arrBrID[N]==pOB->BrigadeID) { newID=false; N=0; };
		}; 
		if (newID)	arrBrID.Add(pOB->BrigadeID);
		return newID;
	};
	int		getAmount()		{ return arrBrID.GetAmount(); };
	LinearArray<int,_int>	arrBrID;	// list of brig id in zone 
	int						Nat;
};
// Add Brigade id in list if unit in Brigade
bool	AddUnitsToSelection(OneObject* OB,void* param){
	lvSSelParam* pPar = (lvSSelParam*)(param);
	if ( OB->NNUM==pPar->NI ) {
		AddUnitToSelected(pPar->NI,OB);
		pPar->SN++;
	};
	return true;
};
bool	AddBrIDifPresent(OneObject* OB,void* param){
	lvSSumSquad* pPar = (lvSSumSquad*)(param);
	pPar->addID(OB);
	return true;
};
bool	AddUnitToSumHide(OneObject* OB,void* param){
	lvSSumb* pPar = reinterpret_cast<lvSSumb*>(param);
	if(OB&&(!OB->Sdoxlo)){
		if (OB->NewBuilding==false) {
		//	OB->Invisible = true;
			SetInvisiblen(OB,true);
		}else if (pPar->bHome) {
		//	OB->Invisible = true;
			SetInvisiblen(OB,true);
		};
		//OB->Illusion = true;
		SetIllusion(OB,true);
		(pPar->Sum)++;
	};
	return true;
};
bool	AddUnitToSumUnHide(OneObject* OB,void* param){
	lvSSum* pPar = reinterpret_cast<lvSSum*>(param);
	if(OB&&(!OB->Sdoxlo)){
		//OB->Invisible = false;
		SetInvisiblen(OB,false);
		//OB->Illusion = false;
		SetIllusion(OB,false);
		(pPar->Sum)++;
	};
	return true;
};
bool	AddUnitToSumN(OneObject* OB,void* param){
	lvSSumNat* pPar = reinterpret_cast<lvSSumNat*>(param);
	if(OB&&OB->NNUM==pPar->Nat&&(!OB->Sdoxlo)&&OB->NewBuilding==false){
		(pPar->Sum)++;
	};
	return true;
};
bool	AddUnitToSumType(OneObject* OB,void* param){
	lvSSumNatType* pPar = reinterpret_cast<lvSSumNatType*>(param);
	if(OB&&(!OB->Sdoxlo)&&OB->NNUM==pPar->Nat&&OB->NIndex==pPar->TypeID&&OB->NewBuilding==false){
		(pPar->Sum)++;
	};
	return true;
};
bool	AddUnitToSumGr(OneObject* OB,void* param){
	lvSSumGr* pPar = reinterpret_cast<lvSSumGr*>(param);

	if (pPar->GrpID<AGroups.GetAmount()) {
		int Nu=AGroups[pPar->GrpID]->Units.GetAmount();
		for(int i=0;i<Nu;i++){
			if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Index==AGroups[pPar->GrpID]->Units[i].ID&&OB->Serial==AGroups[pPar->GrpID]->Units[i].SN&&OB->NewBuilding==false){
				(pPar->Sum)++;
			};
		};
	};
	return true;
};

void	FilterUnitsByCategory(be_CGROUP* pvg_Src,be_CGROUP* pvg_Dst,int Ctg){
	if (pvg_Src==NULL||pvg_Dst==NULL)	return;
	
	int N=pvg_Src->GetTotalAmount();
	OneObject*	pOB = NULL;
	bool		ok = false;
	for (int i=0;i<N;i++){
		pOB = pvg_Src->GetOneObj_lID(i);
		if (pOB!=NULL){
			ok=false;
			switch(Ctg){
				case 0://archer
					ok=DetectArcher(pOB->Ref.General);
					break;
				case 1://short range unit
					ok=DetectShortRangeUnit(pOB->Ref.General);
					break;
				case 2://shooter
					ok=DetectShootingUnit(pOB->Ref.General);
					break;
				case 3://pesants
					ok=pOB->newMons->Peasant;
					break;
				case 4://not hiden
					ok=!pOB->Hidden;
					break;
				case 5://tomagavk
					ok=DetectTamagavkUnit(pOB->Ref.General);
					break;
			};
			if (ok){
				pvg_Src->RemUnitGPT(pOB);
				pvg_Dst->AddUnitGPT(pOB);
				addname(pOB->Index);
			};
			pOB=NULL;
		};
	};
};

void	BClrBar_InR(int x0,int y0,int R){
	int x16 = (int)((float)x0/16.f+.5f);
	int y16 = (int)((float)y0/16.f+.5f);
	
	int Lx = R/16;

	BClrBar(x16,y16,Lx);
};

bool	KillUnitsinZone(OneObject* OB,void* param){
	lvSSumNat* pPar = reinterpret_cast<lvSSumNat*>(param);
	if(OB&&OB->NNUM==pPar->Nat&&(!OB->Sdoxlo)&&(OB->NewBuilding==false)){
		OneObject* pOB = NULL;
			pOB=OB;
			if(pOB){
				pOB->delay=6000;
				if(pOB->LockType==1)pOB->RealDir=32;
				pOB->Die(true);
				pOB=OB;
				if(pOB){
					pOB->Sdoxlo=2500;
				};
			};
			pOB = NULL;
		};
	(pPar->Sum)++;
	return true;
};


// SOUND
void			ov_Play_lua(const char* pcszFileName,int ucStream){
	ov_Play(pcszFileName,ucStream);
};
void			ov_Stop_lua(int ucStream){
	ov_Stop(ucStream);
};
void			ov_SetVolume_lua(int ucVolume,int ucStream){
	ov_SetVolume(ucVolume,ucStream);
};
void			ov_Cyclic_lua(bool bCyclic,int ucStream){
	// ManOwaR :: ov_Cyclic() is not implemented
};
bool			ov_StreamFinished_lua(int ucStream){
	return	ov_StreamFinished(ucStream);
};

//////////////////////////////////////////////////////////////////////////
//						PRO MISSION										//
//////////////////////////////////////////////////////////////////////////
CPRO_MISS_FILER	g_PMF;
const	char*	COPERCOND_CALLDESCR::GetThisElementView(const char* LocalName){
	_str myview; myview=NCALL; return myview.pchar();
};
void	COPERCOND_CALLDESCR::saveToFile(FILE* file){
	if (file!=NULL){
		fprintf(file,"---------------------------------------\n");
		fprintf(file,"DESCR : %s\n",DESCR.pchar());
		fprintf(file,"VIEW  : %s\n",VIEW.pchar() );
		fprintf(file,"NCALL : %d\n",NCALL    );
	};
};
void	CPRO_MISS_FILER::addCALL(int ID){
	if (0<=ID&&ID<_SIZE)	DATA[ID]->NCALL += 1;
};
void	CPRO_MISS_FILER::clearCALL_DATA(){
	DATA.Clear();
	_SIZE=0;
};
void	CPRO_MISS_FILER::CCLEAR::EvaluateFunction(){
	g_PMF.clearCALL_DATA();
};
void	CPRO_MISS_FILER::clearSD(){
	int N=SORT_DATA.GetAmount();
	while (N--) {
		SORT_DATA[N]=NULL;
	};
	SORT_DATA.Clear();
};
void	CPRO_MISS_FILER::setSD_MoreThenNCall(int NCall){
	clearSD();
	for (int i=0; i<_SIZE; i++){
		if (DATA[i]->NCALL>=NCall)	SORT_DATA.Add( DATA[i] );
	};
};
void	CPRO_MISS_FILER::CPARM_DEPEND_ONLY::EvaluateFunction(){
	g_PMF.setSD_MoreThenNCall(MoreThenNCall);	
};
void	CPRO_MISS_FILER::Save_log(char* FName){
	if (FName!=NULL){
		FILE* file = fopen(FName,"w");
		assert(file!=NULL);
		if (file) {
			int N=SORT_DATA.GetAmount();
			for (int i=0; i<N; i++){
				SORT_DATA[i]->saveToFile(file);
			};
			fprintf(file,"---------------------------------------\n");
			fclose(file);
		};
	};
};
void	CPRO_MISS_FILER::CSAVE_LOG::EvaluateFunction(){
	g_PMF.Save_log(FName.pchar());
};
//////////////////////////////////////////////////////////////////////////
//======================================================================//
//=================    OPERATION FOR SCRIPT		========================//
//======================================================================//
lvCBaseOperCond::lvCBaseOperCond(){
	InfID = _lvCBaseOperCond_;
//	use_vGroup=DriveMode()->USE_vGRP;
	use_vGroup=true;
	first=true;
	UseNode = false;
	if (EngSettings.GameName==2/*Fantasy*/)	UseNode = true;
};
lvCBaseOperCond::lvCBaseOperCond(lvCBaseOperCond* pBaseOperCond){
	InfID = _lvCBaseOperCond_;
	use_vGroup=true;
	first=true;
	if (pBaseOperCond!=NULL) {
		InfID			= pBaseOperCond->InfID;
		Descr			= pBaseOperCond->Descr.pchar();
		x0				= pBaseOperCond->x0;
		y0				= pBaseOperCond->y0;
		x1				= pBaseOperCond->x1;
		y1				= pBaseOperCond->y1;
		squardID		= pBaseOperCond->squardID;
		timeInProc		= pBaseOperCond->timeInProc;
		use_vGroup		= pBaseOperCond->use_vGroup;
		first			= pBaseOperCond->first;
		UseNode			= pBaseOperCond->UseNode;
		parNode			= pBaseOperCond->parNode;
	};
};

void			lvCBaseOperCond::GetCopy(lvCBaseOperCond** pCopy){
	*pCopy = new lvCBaseOperCond(this);
}

const	char*	lvCBaseOperCond::GetThisElementView(const char* LocalName){

	char* Presentation = GetGlobalBuffer();
	CE_WCP.GetPresentation(Presentation,this);
	return Presentation;

};
// lvCOperation //////////////////////////////////////////////////////////
lvCOperation::lvCOperation()/* : lvCBaseOperCond() */{ 
	InfID = _lvCOperation_; 
	RepeatInTime=false; 
	myID=-1; 
};
int		lvCOperation::Process(int time){ 
	if (g_PMF.USE_CALL_OPERATION){
		if (myID==-1)	myID=g_PMF.getNEW_SELL<lvCOperation>(dynamic_cast<BaseClass*>(this));
		g_PMF.addCALL(myID);
	};
	AddIteration(); 
	CheckIteration(); 
	return 0; 
};
void			lvCOperation::GetCopy(lvCOperation** pCopy){
	*pCopy = new lvCOperation(this);
};

void			lvCOperation::CheckIteration()	{ 
	if (RepeatInTime&&CurIter>50) { 
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(squardID);
		if (pvGRP!=NULL&&pvGRP->GetORDER()!=vgATTACK) {
			CurIter=0; first=true; 
		};
	}; 
};
char*			lvCOperation::GetSourceCode(int shift/* = 0*/){
	GetThisElementView(NULL);
	return Descr.pchar();
};
// lvCKillNatinZone //////////////////////////////////////////////////////////
int				lvCKillNatinZone::CallFunction(){
	int xxx=-1;
	int yyy=-1;
	int RRR=-1;

	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL){
		xxx = pNode->vGetX();
		yyy = pNode->vGetY();
		RRR = pNode->vGetR();
	};

	if (xxx!=-1&&yyy!=-1&&RRR!=-1)	
		return KillNatinInPOS_lua(parNat,xxx,yyy,RRR);

	return 0;
};
int KillNatinInPOS_lua(int nat,int x,int y,int R){
	lvSSumNat	Ret;	Ret.Sum = 0; Ret.Nat=nat;
	if ( 0<=nat&&nat<8 ) {
		Ret.Nat = nat;
		PerformActionOverUnitsInRadius(x,y,R,KillUnitsinZone,&Ret);
	};
	return 1;
}
// lvCKillNatNear //////////////////////////////////////////////////////////
int				lvCKillNatNear::CallFunction(){
	if ((0<=parNat&&parNat<8)&&(0<parRad)) {
		int vgX;
		int vgY;
		be_CGROUP* pvGRP = (be_CGROUP*)(vGrp.Get());
		if (pvGRP) {
			pvGRP->GetGroupCenter(vgX,vgY);
			itr_UnitsInRadius.Create(vgX,vgY,parRad);
			OneObject* pOB = NULL;
			while(pOB=itr_UnitsInRadius.Next()){
				if ((!pOB->Sdoxlo||pOB->Hidden)&&pOB->NNUM==parNat){
					pOB->delay=6000;
					if(pOB->LockType==1)pOB->RealDir=32;
					pOB->Die();
					if(pOB)	pOB->Sdoxlo=2500;
				};
			};		
//			PerformActionOverUnitsInRadius(	vgX,vgY,parRad,KillUnitsinZone,&Ret );
			return 1;
		};
	};
	return 0;
};
// lvCEraseNation ////////////////////////////////////////////////////////
int				lvCEraseNation::CallFunction(){
	itr_GetUnitsOfNation.Create(parNat);
	OneObject* pOB = NULL;
	while(pOB=itr_GetUnitsOfNation.Next()){
		if( !pOB->Sdoxlo || pOB->Hidden ){
			if( !DetectHero(pOB) ){
				pOB->Hidden=true;
				pOB->Die(true);
				pOB->Sdoxlo=30000;
				void DeleteAbilities(OneObject* OB);
				DeleteAbilities(pOB);
			}
		};
	};
	return 0;
};
// lvCSelectAll //////////////////////////////////////////////////////////
int				lvCSelectAll::CallFunction(){
	return SelectAll_lua(parNat);
};
int SelectAll_lua(int nat){
	bool ff;
	OneObject*	pUnit = NULL;
	ClearSelection(nat);
	addrand(3108);
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit&&!pUnit->Sdoxlo) {
			if (pUnit->NNUM==nat) {
				AddUnitToSelected(nat,pUnit);
				if(!ff) ff=true;
			};
		};
	};
	if (ff) {
		return 1;
	}else{
		return 0;
	};
}
int SelectUnits_lua(int nat,int x,int y,int R,bool add){
	lvSSelParam	Ret;	
	Ret.NI = nat;
	if ( add==false )	ClearSelection(Ret.NI);
	addrand(67);
	PerformActionOverUnitsInRadius(	x,y,R,AddUnitsToSelection,&Ret );
	CopyReIm(Ret.NI);
	return Ret.SN;
};
int SelectUnitsType_lua		(int nat,int UnitType,int x,int y,int R,bool add){
	if ( add==false )	ClearSelection(nat);
	addrand(318);
	OneObject* pOB = NULL;
	itr_UnitsInRadius.Create(x,y,R);
	while(pOB=itr_UnitsInRadius.Next()){
		if ( (!pOB->Sdoxlo||pOB->Hidden) && pOB->NNUM==nat && pOB->NIndex==UnitType ) {
			AddUnitToSelected(nat,pOB);
		};
	};
	CopyReIm(nat);
	return 1;
};
// lvCSelectNation //////////////////////////////////////////////////////
int		lvCSelectNation::CallFunction(){
	if ( parNat<0 || parNat>7 ) return 0;
	if ( parAdd==false )	ClearSelection(parNat);
	addrand(39);
	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit&&(!pUnit->Sdoxlo||pUnit->Hidden)&&pUnit->NNUM==parNat) {
			if ( (parDom && pUnit->NewBuilding) || !parDom ) {
				AddUnitToSelected(parNat,pUnit);
			};
		};
	};
	CopyReIm(parNat);
	return 1;
};
// lvCChangeAS //////////////////////////////////////////////////////////
int				lvCChangeAS::CallFunction(){
	return ChangeAS_lua(parNat,parState);
};
int ChangeAS_lua(int nat,int state){
	bool ff;
	OneObject*	pUnit = NULL;
	ClearSelection(nat);
	addrand(113);
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit&&!pUnit->Sdoxlo) {
			if (pUnit->NNUM==nat) {
				pUnit->ActivityState=state;
				if(!ff) ff=true;
			};
		};
	};
	if (ff) {
		return 1;
	}else{
		return 0;
	};
}
// lvCSelSendTo //////////////////////////////////////////////////////////
int				lvCSelSendTo::CallFunction(){
	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL) {
        SendSelectedToXY(parNat,pNode->vGetX()<<4,pNode->vGetY()<<4,parDir,16,parType);
		return 1;
	};
		
	return 0;
};
int SelSendTo_lua(int nat,int x,int y,int dir,int type){
	SendSelectedToXY(nat,x<<4,y<<4,dir,16,type);
	return 1;
}
// lvCGroupSendTo ////////////////////////////////////////////////////////
int				lvCGroupSendTo::CallFunction(){
	
	int xxx=-1;
	int	yyy=-1;
	int RRR=-1;
	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL) {
		xxx = pNode->vGetX();
		yyy = pNode->vGetY();
		RRR = pNode->vGetR();
	};
	if (xxx!=-1&&yyy!=-1&&RRR!=-1){
		be_CGROUP* pvGRP = (be_CGROUP*)parGrp.Get();
		if (pvGRP) {
			pvGRP->SendTo(xxx,yyy,parDir,parType);
			return 1;
		};
	};

	return 0;
};
// lvCSelSendToNode //////////////////////////////////////////////////////
lvCSelSendToNode::lvCSelSendToNode(lvCSelSendToNode* pSelSendToNode) : lvCOperation(dynamic_cast<lvCOperation*>(pSelSendToNode)) {
	if (pSelSendToNode!=NULL) {
		parNat		= pSelSendToNode->parNat;
		parDir		= pSelSendToNode->parDir;
		parType		= pSelSendToNode->parType;
	};
};

void			lvCSelSendToNode::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCSelSendToNode(this));
};

const char*		lvCSelSendToNode::GetThisElementView(const char* LocalName){
	Descr="";
	if(0<=parNat&&parNat<8){
		Descr += "SelSendToNode(";
		Descr += parNat;
		Descr += ", ";
		Descr += parDir;
		Descr += ", ";
		Descr += parType;
		Descr += ")";
	}else{
		Descr="lvCSelSendToNode(NULL)";
	};
	return Descr.pchar();
};
int				lvCSelSendToNode::Process(int time){
	lvCOperation::Process(time);
	if (parDir<0) {
		SendSelectedToXY(parNat,x1<<4,y1<<4,lvBE_GetDirec(x1-x0,y1-y0),16,parType);
		return 1;
	}else{
		SendSelectedToXY(parNat,x1<<4,y1<<4,parDir,16,parType);
		return 1;
	};
	return 0;
};
// lvCGroupSendToNode ////////////////////////////////////////////////////
lvCGroupSendToNode::lvCGroupSendToNode(lvCGroupSendToNode* pGroupSendToNode) : lvCOperation(dynamic_cast<lvCOperation*>(pGroupSendToNode)) {
	if (pGroupSendToNode!=NULL) {
		parGrp		= pGroupSendToNode->parGrp;
		parDir		= pGroupSendToNode->parDir;	
		parType		= pGroupSendToNode->parType;
	};
};

void			lvCGroupSendToNode::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCGroupSendToNode(this));
};

const char*		lvCGroupSendToNode::GetThisElementView(const char* LocalName){
	Descr="";
	if(0<=parGrp){
		Descr += "GroupToNode(";
		if (use_vGroup) {
			be_CGROUP* pvGRP = GroupsMap()->GetGroupID(parGrp);
			if (pvGRP) {
				Descr += pvGRP->GetName();
			};
		}else{
			if ((AGroups[parGrp]!=NULL)) {
				Descr += AGroups[parGrp]->Name.pchar();
			};
		};
		Descr += ", ";
		Descr += parDir;
		Descr += ", ";
		Descr += parType;
		Descr += ")";
	}else{
		Descr="lvCGroupSendToNode(NULL)";
	};
	return Descr.pchar();
};
int				lvCGroupSendToNode::Process(int time){
	lvCOperation::Process(time);
	if (use_vGroup) {
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(parGrp);
		if (pvGRP) {
			if (!pvGRP->GetORDER()){
				if (first || (!pvGRP->GetAmountInZone(x1,y1,GetMinR_ByNUnits(pvGRP->GetTotalAmount()))) || (pvGRP->GetDirection()!=parDir)){
					pvGRP->SendTo(x1,y1,parDir);
					if (first) first=false;
					return 1;
				};
			};
		};
	}else{
		if ((AGroups[parGrp]!=NULL)) {
			lvBE_ClearSelection(parGrp);
			lvBE_SelectGroup(parGrp);
			if (first || (!lvBE_GrpInZone(parGrp,x1,y1,BE_MinR)&&lvBE_GrpFree(parGrp,10)) ) {
				lvBE_SendGroupToXY(parGrp,x1,y1,parDir,parType,x0,y0);
				if (first) first=false;
			};
		};
	};
	return 0;
};
// lvCRotateGroup ////////////////////////////////////////////////////////
int				lvCRotateGroup::CallFunction(){

	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) {
		pvGRP->ChangeDirection(parDir,prior);
		return 1;
	}
	
	return 0;
};
// lvCRotate /////////////////////////////////////////////////////////////
lvCRotate::lvCRotate(lvCRotate* pRotate) : lvCOperation(dynamic_cast<lvCOperation*>(pRotate)) {
	if (pRotate!=NULL) {
		parDir	= pRotate->parDir;
		prior	= pRotate->prior;
	};
};

void			lvCRotate::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCRotate(this));
};

const char*		lvCRotate::GetThisElementView(const char* LocalName){
	Descr="";
	if (parDir>=0){
		Descr += "Rotate(";
		Descr += parDir;
		Descr += ", ";
		Descr += prior;
		Descr += ")";
	}else{
		Descr="lvCRotate(NULL)";
	};
	return Descr.pchar();
};
int				lvCRotate::Process(int time){
	lvCOperation::Process(time);
	int	bx_,by_;
	bx_=x1;
	by_=y1;
	if (use_vGroup) {
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(squardID);
		if (pvGRP) {
			if (!pvGRP->GetORDER()){
				if (first || abs(pvGRP->GetDirection()-parDir)>20 ) {
				//	pvGRP->GetGroupCenter(bx_,by_);
				//	pvGRP->SendTo(bx_,by_,parDir);
					pvGRP->ChangeDirection(parDir);
					if (first){ first=false;};
					return 1;
				};
			};
		};
	}else{
		if ((AGroups[squardID]!=NULL)) {
			word MID=0;
			if (first && (lvBE_GrpFree(squardID,10)&&!lvBE_TrueGrpPrior(squardID,parDir,10))) {
				int Nu=AGroups[squardID]->Units.GetAmount();
				for(int i=0;i<Nu;i++){
					MID=AGroups[squardID]->Units[i].ID;
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB&&OB->GetLiveState()==ls_LiveBody&&OB->Serial==AGroups[squardID]->Units[i].SN){
							if(lvBE_GetGrpDir(squardID,bx_,by_)!=parDir){
								if (first && (OB->BrigadeID!=0xFFFF)){
									Brigade* BR=&CITY[OB->NNUM].Brigs[OB->BrigadeID];
									first=false;
								};
								addrand(MID);
								addrand(parDir);
								addrand(prior);
								OBJ_Rotate(MID,parDir,prior);
							};
						};
					};
				};
				lvBE_SendGroupToXY(squardID,bx_,by_,parDir,prior,x0,y0);
				if (first) {first=false;};
			};
		};
	};
	return 0;
};

int				lvCRotate::Complite(){
	if (use_vGroup)	{
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(squardID);
		if (pvGRP!=NULL && abs(pvGRP->GetDirection()-parDir)<=20 && !pvGRP->GetORDER())	return 1;
	};
	return 0;
};
// lvCSendToNode /////////////////////////////////////////////////////////
lvCSendToNode::lvCSendToNode(lvCSendToNode* pSendToNode) : lvCOperation(dynamic_cast<lvCOperation*>(pSendToNode)) {
	if (pSendToNode!=NULL) {
		parDir	 = pSendToNode->parDir;
		parType  = pSendToNode->parType;
	};
};

void			lvCSendToNode::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCSendToNode(this));
};

const char*		lvCSendToNode::GetThisElementView(const char* LocalName){
	Descr="";
	if(0<=squardID){
		Descr += "SendToNode(";
		Descr += parDir;
		Descr += ", ";
		Descr += parType;
		Descr += ")";
	}else{
		Descr="lvCSendToNode(NULL)";
	};
	return Descr.pchar();
};
int				lvCSendToNode::Process(int time){
	lvCOperation::Process(time);
	if (use_vGroup) {
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(squardID);
		if (pvGRP) {
			if (pvGRP->GetORDER()==vgNO_ORDERS || (parType==0 && first)) {
				if (	first 
						|| 
						pvGRP->GetAmountInZone(x1,y1,GetMinR_ByNUnits(pvGRP->GetTotalAmount()))!=pvGRP->GetTotalAmount() 
						||
						( parDir!=512 && GetCircleDif(pvGRP->GetDirection(),parDir)>32 ) 
				   )
				{
					pvGRP->SendTo(x1,y1,parDir);
					if (first) first=false;
					return 1;
				};
			};
		};
	}else{
		if ((AGroups[squardID]!=NULL)) {
			lvBE_ClearSelection(squardID);
			lvBE_SelectGroup(squardID);
			/*!lvBE_TrueGrpPrior(squardID,parDir,10) || */
			if (first || (lvBE_GrpFree(squardID,10)&&(!lvBE_GrpInZone(squardID,x1,y1,BE_MinR)))) {
				lvBE_SendGroupToXY(squardID,x1,y1,parDir,parType,x0,y0);
				if (first) first=false;
			};
		};
	};
	return 0;
};

int				lvCSendToNode::Complite(){
	if (use_vGroup) {
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(squardID);
		if (pvGRP
			&&
			!pvGRP->GetORDER()
			&&
			pvGRP->GetAmountInZone(x1,y1,GetMinR_ByNUnits(pvGRP->GetTotalAmount()))==pvGRP->GetTotalAmount()
			)
		{
			if ( (parDir!=512 && GetCircleDif(pvGRP->GetDirection(),parDir)<=32) || parDir==512) return 1;
		};
	};
	return 0;
};
// lvCSelectUnits ////////////////////////////////////////////////////////
int				lvCSelectUnits::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) {
		pvGRP->SelectUnits(parAdd);
		/*
		if (!parAdd) {
			pvGRP->SelectUnits(0);
		}else{
			pvGRP->SelectUnits(1);
		};
		*/
		return 1;
	}
	
	return 0;
};
// lvCSetUnitMoveType ///////////////////////////////////////////////////////
int				lvCSetUnitMoveType::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) 
	{
		pvGRP->SetLockType(movetype);
		return 1;
	};
	return 0;
};
// lvCGetUnitMoveType ///////////////////////////////////////////////////////
int				lvCGetUnitMoveType::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) 
	{
		return pvGRP->GetLockType();
	};
	return -1;
};
// lvCSetUnitState ///////////////////////////////////////////////////////
int				lvCSetUnitState::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) {
		if (pvGRP->SetAgresiveST(parmode)==1)	return 1;
	};
	
	return 0;
};
// lvCIncreaseLevel ///////////////////////////////////////////////////////
int				lvCIncreaseLevel::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) {
		pvGRP->IncreaseLevel(amount);
		return 1;
	};
	return 0;
};
// lvCSetState ///////////////////////////////////////////////////////////
lvCSetState::lvCSetState(lvCSetState* pSetState) : lvCOperation(dynamic_cast<lvCOperation*>(pSetState)) {
	if (pSetState!=NULL) {
		parmode = pSetState->parmode;
	};
}

void			lvCSetState::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCSetState(this));
};

const char*		lvCSetState::GetThisElementView(const char* LocalName){
	Descr="";
	Enumerator* E=ENUM.Get("BE_UNIT_MOVE_MODE");
	if(0<=parmode){
		Descr += "SetState(";
		Descr += E->GetStr(parmode);
		Descr += ")";
	}else{
		Descr="lvCSetState(NULL)";
	};
	return Descr.pchar();
};
int				lvCSetState::Process(int time){
	lvCOperation::Process(time);
	if (use_vGroup) {
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(squardID);
		if (pvGRP) {
			if (pvGRP->SetAgresiveST(parmode)) return 1;		
		};
	}else{
		if ((AGroups[squardID]!=NULL)) {
			int Nu=AGroups[squardID]->Units.GetAmount();
			int NU=0;
			for(int i=0;i<Nu;i++){
				word MID=AGroups[squardID]->Units[i].ID;
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if (OB&&OB->GetLiveState()==ls_LiveBody&&OB->Index==AGroups[squardID]->Units[i].ID&&OB->Serial==AGroups[squardID]->Units[i].SN) {
						OB->ActivityState=parmode;
					};
				};
			};
		};
	};
	return 0;
};
// lvCReformation ////////////////////////////////////////////////////////
lvCReformation::lvCReformation(lvCReformation* pReformation) : lvCOperation(dynamic_cast<lvCOperation*>(pReformation)) {
	if (pReformation!=NULL) {
		TypeForm = pReformation->TypeForm;
	};
};

void			lvCReformation::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCReformation(this));
};

const char*		lvCReformation::GetThisElementView(const char* LocalName){
	Descr="";
	if(0<=TypeForm){
		Descr += "Reformation(";
		Descr += TypeForm;
		if (UseInNode) {
			Descr += ")";
		}else{
			Descr += ",";
			be_CGROUP* pvGRP_A = GroupsMap()->GetGroupID(vGrp);
			if (pvGRP_A!=NULL&&pvGRP_A->Name.pchar()!=NULL)	Descr += pvGRP_A->Name.pchar();
			else										Descr += "NoGroup";
			Descr += ")";
		};
		
	}else{
		Descr="lvCReformation(NULL)";
	};
	return Descr.pchar();
};
int				lvCReformation::Process(int time){
	lvCOperation::Process(time);
	if (use_vGroup) {
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(squardID);
		if (pvGRP) {
			if (first || pvGRP->GetORDER()==vgNO_ORDERS || UseInNode==true ){
				pvGRP->ChengeFormation(TypeForm);
				if (first)	first=false;
			}else if (UseInNode==false) {
				be_CGROUP* pvGRP_A = GroupsMap()->GetGroupID(vGrp);
				if (pvGRP_A!=NULL)	pvGRP_A->ChengeFormation(TypeForm);
			};
		};
	}else{
		if ((AGroups[squardID]!=NULL)) {
			int Nu=AGroups[squardID]->Units.GetAmount();
			int NU=0;
			for(int i=0;i<Nu;i++){
				word MID=AGroups[squardID]->Units[i].ID;
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if (OB&&OB->GetLiveState()==ls_LiveBody&&OB->Index==AGroups[squardID]->Units[i].ID&&OB->Serial==AGroups[squardID]->Units[i].SN) {
						MakeReformation(OB->NNUM,OB->BrigadeID,TypeForm);
					};
				};
			};
		};
	};
	return TypeForm;
};
// lvCBrigReformation ////////////////////////////////////////////////////
int				lvCBrigReformation::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP) {
		pvGRP->ChengeFormation(TypeForm);
		return 1;
	};

	return 0;
};
// lvCChangeFriends //////////////////////////////////////////////////////
int				lvCChangeFriends::CallFunction(){
	return ChangeFriends_lua( Nation, Value );
};
int ChangeFriends_lua(int nat, int state){
	if( 0<=nat&&nat<8 ){
		ChangeFriends( nat, state );
		return 1;
	};	
	return 0;
};
// lvCSetFriends /////////////////////////////////////////////////////////
int				lvCSetFriends::CallFunction(){
	return SetFriends_lua(fstNat,secNat);
};
int SetFriends_lua(int nat0,int nat1){
	int val=1;
	int Flags;
	if( (0<=nat0&&nat0<8)&&(0<=nat1&&nat1<8) ){
		for(int i=0;i<nat1;i++) val*=2;
		int i=MAXOBJECT;
		while (i--){
			OneObject* OB=Group[i];
			if(OB&&OB->NNUM==nat0){
				Flags=OB->NMask;
				break;
			};
		};
		if (i==-1) {
			return 0;
		}
		val|=Flags;
		ChangeFriends( nat0, val );
		return 1;
	};	
	return 0;
};
// lvCChangeNation ///////////////////////////////////////////////////////
lvCChangeNation::lvCChangeNation(lvCChangeNation* pChangeNation) : lvCOperation(dynamic_cast<lvCOperation*>(pChangeNation)) {
	if (pChangeNation!=NULL) {
		nwNat = pChangeNation->nwNat;
	};
};

void			lvCChangeNation::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCChangeNation(this));
};

const char*		lvCChangeNation::GetThisElementView(const char* LocalName){
	Descr="";
	Descr += "Change Nation(";
	if(0<=nwNat && nwNat<8){
		Descr += nwNat;
	}else{
		Descr+="NULL";
	};
	Descr += ")";
	return Descr.pchar();
};
int				lvCChangeNation::Process(int time){
	lvCOperation::Process(time);
	if(use_vGroup){
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(squardID);
		if (pvGRP) {
			pvGRP->SetNation(nwNat);
		};
		return nwNat;
	}else{
		return 0;
	};
};
// lvCChangeNationG //////////////////////////////////////////////////////
int				lvCChangeNationG::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP) {
		pvGRP->SetNation(nwNat);
		return 1;
	};
	
	return 0;
};
// lvCShowVictory ////////////////////////////////////////////////////////
int				lvCShowVictory::CallFunction(){
//	vvTEXT* pText = (vvTEXT*)(TextID.Get());
//	if (pText!=NULL) {
	if ( TextID.pchar()!=0 && (TextID.pchar())[0]!=0 ){
//		ShowVictory( (char*)(pText->Get()), Nat );
		ShowVictory( GetTextByID(TextID.pchar()), Nat );
	}else{
		ShowVictory(NULL,Nat);
	};
	DriveMode()->PROCESS=false;
	return 1;
};
int ShowVictory_lua(int nat,const char* TextID){
	if (TextID!=NULL){
        char* pText=(char*)malloc(strlen(TextID)+1);
		strcpy(pText,TextID);
		pText[strlen(TextID)]=0;
		ShowVictory( GetTextByID(pText), nat );
		free(pText);
	}else{
		ShowVictory();
	};
	DriveMode()->PROCESS=false;
	return 1;
};
// lvCLooseGame //////////////////////////////////////////////////////////
int				lvCLooseGame::CallFunction(){
//	vvTEXT* pText = (vvTEXT*)(TextID.Get());
//	if (pText!=NULL) {
	if ( TextID.pchar()!=NULL && (TextID.pchar())[0]!=0 ){
//		LooseGame( (char*)(pText->Get()), Nat );	
		LooseGame( GetTextByID(TextID.pchar()), Nat );	
	}else{
		LooseGame(NULL,Nat);
	};
	DriveMode()->PROCESS=false;
	return 1;
};
int LooseGame_lua(int nat,const char* TextID){
	if (TextID!=NULL){
		char* pText=(char*)malloc(strlen(TextID)+1);
		strcpy(pText,TextID);
		pText[strlen(TextID)]=0;
		LooseGame( GetTextByID(pText), nat );
		free(pText);
	}else{
		LooseGame();
	};
	DriveMode()->PROCESS=false;
	return 1;
};
// lvCSetTrigg ///////////////////////////////////////////////////////////
int				lvCSetTrigg::CallFunction(){
 	if(TID>511)	return 0;
	SCENINF.TRIGGER[TID]=~NewVal;
	return 1;
};
// lvCSavePlayersProfile /////////////////////////////////////////////////
int				lvCSavePlayersProfile::CallFunction(){
	void vmPlayerDataWrite();
	vmPlayerDataWrite();
	return 1;
};
// lvCSetLightSpot ///////////////////////////////////////////////////////
int				lvCSetLightSpot::CallFunction(){
	int x0=-1;
	int	y0=-1;
	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL) {
		x0=pNode->vGetX();
		y0=pNode->vGetY();
	};
	if (x0!=-1&&y0!=-1){
		return SetLightSpot_lua(x0,y0,Radius,Index);
	};

	return 0;
};
int SetLightSpot_lua(int x,int y,int R,int index){
	if(index>63)	return 0;  
	SCENINF.LSpot[index].x=x;
	SCENINF.LSpot[index].y=y;
	SCENINF.LSpot[index].Type=R;
	return 1;
};
// lvCClearLightSpot /////////////////////////////////////////////////////
int				lvCClearLightSpot::CallFunction(){
	return ClearLightSpot_lua(Index);
};
int ClearLightSpot_lua(int index){
	if(index>63) return 0;
	SCENINF.LSpot[index].x=0;
	SCENINF.LSpot[index].y=0;
	SCENINF.LSpot[index].Type=0;
	return 1;
};
// lvCSetStartPoint //////////////////////////////////////////////////////
int				lvCSetStartPoint::CallFunction(){
	int x=-1;
	int y=-1;
	if (Use_VVal) {
		be_CINT*	psX = (be_CINT*)(sX.Get());
		be_CINT*	psY = (be_CINT*)(sY.Get());
		if (psX!=NULL&&psY!=NULL) {
			x = psX->Value;
			y = psY->Value;
		};
	}else{
		be_CNODE* pNode = (be_CNODE*)(parNode.Get());
		if (pNode!=NULL) {
			x = pNode->vGetX();
			y = pNode->vGetY();
		};
	};
	if (x!=-1&&y!=-1) {
		return	SetStartPoint_lua(x,y);
	};
		
	return 0;
};
DWORD			lvCSetStartPoint::GetClassMask(){
	if (Use_VVal)	return	0x02000000;
	else			return	0x01000000;
};
int SetStartPoint_lua(int x,int y){
	x=(x-(RealLx>>1))>>5;
	y=(y-RealLy)>>5;
	if(x<1)x=1;
	if(y<1)y=1;
	if(x+smaplx>msx-1)x=msx-smaplx-1;
	if(y+smaply>msy-1)y=msy-smaply-1;
	if (x!=0||y!=0) {
		void SetCameraPos(float x,float y);
		SetCameraPos((float)x,(float)y);
		return 1;
	};
	return 0;
};
// lvCTakeFood ///////////////////////////////////////////////////////////
int				lvCTakeFood::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP!=NULL) {
		pvGRP->TakeFood();
		return 1;
	};
	return 0;
};
// lvCTakeWood ///////////////////////////////////////////////////////////
int				lvCTakeWood::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP!=NULL) {
		pvGRP->TakeWood();
		return 1;
	};
	return 0;
};
// lvCTakeStone //////////////////////////////////////////////////////////
int				lvCTakeStone::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP!=NULL) {
		pvGRP->TakeStone();
		return 1;
	};
	return 0;
};
// lvCSetValue ///////////////////////////////////////////////////////////
int				lvCSetValue::CallFunction(){
    switch(TypeID) {
	case 0:			// Triger
		{
			be_CBOOL* pVariable = (be_CBOOL*)(Variable.Get());
			if (pVariable!=NULL) {
				if (UseVV) {
					be_CBOOL* pValue = (be_CBOOL*)(Value.Get());
					pVariable->Value = pValue->Value;
					return 1;
				}else{
					pVariable->Value = SetTG;
					return 1;
				};
			};
		};
		break;
	case 1:			// Word
		{
			be_CDWORD* pVariable = (be_CDWORD*)(Variable.Get());
			if (pVariable!=NULL) {
				if (UseVV) {
					be_CDWORD* pValue = (be_CDWORD*)(Value.Get());
					pVariable->Value = pValue->Value;
					return 1;
				}else{
					pVariable->Value = SetWD;
					return 1;
				};
			};
		};
		break;
	case 2:			// Integer
		{
			be_CINT* pVariable = (be_CINT*)(Variable.Get());
			if (pVariable!=NULL) {
				if (UseVV) {
					be_CINT* pValue = (be_CINT*)(Value.Get());
					pVariable->Value = pValue->Value;
					return 1;
				}else{
					pVariable->Value = SetIN;
					return 1;
				};
			};
		};
		break;
	};

	return	0;
};

bool			lvCSetValue::AskParentForUsingExpansionClass(char* MemberName,char* ClassName){
	if (strcmp(MemberName,"Value")!=0)	return true;
	
	if (TypeID==0 && strcmp(ClassName,"be_CBOOL"	)	==0)	return true;		// Triger
	if (TypeID==1 && strcmp(ClassName,"be_CDWORD"	)	==0)	return true;		// Word
	if (TypeID==2 && strcmp(ClassName,"be_CINT"		)	==0)	return true;		// Integer
	
	return false;
};

DWORD			lvCSetValue::GetClassMask(){
	DWORD MASK = 0x80000000;

	switch(TypeID) {
	case 0:			// Triger
		MASK = 0x00010000;
		if (UseVV) 	MASK += 0x00001000;
		else		MASK += 0x00000100;
		break;
	case 1:			// Word
		MASK = 0x00020000;
		if (UseVV)	MASK += 0x00002000;
		else		MASK += 0x00000200;
		break;
	case 2:			// Integer
		MASK = 0x00040000;
		if (UseVV) 	MASK += 0x00004000;
		else		MASK += 0x00000400;
		break;
	};

	return MASK;
};
// lvCSetValueEX /////////////////////////////////////////////////////////
int				lvCSetValueEX::CallFunction(){
    switch(TypeID) {
	case 0:			// Triger
		{
			be_CBOOL* pVariable = (be_CBOOL*)(Variable.Get());
			if (pVariable!=NULL) {
				pVariable->Value = (bool)Function.CallFunction();
				return 1;
			};
		};
		break;
	case 1:			// Word
		{
			be_CDWORD* pVariable = (be_CDWORD*)(Variable.Get());
			if (pVariable!=NULL) {
				pVariable->Value = (int)Function.CallFunction();
				return 1;
			};
		};
		break;
	case 2:			// Integer
		{
			be_CINT* pVariable = (be_CINT*)(Variable.Get());
			if (pVariable!=NULL) {
				pVariable->Value = (int)Function.CallFunction();
				return 1;
			};
		};
		break;
	};

	return	0;
};
DWORD			lvCSetValueEX::GetClassMask(){
	DWORD MASK = 0x80000000;

	switch(TypeID) {
	case 0:			// Triger
		MASK = 0x00010000;
		break;
	case 1:			// Word
		MASK = 0x00020000;
		break;
	case 2:			// Integer
		MASK = 0x00040000;
		break;
	};

	return MASK;	
};
// lvCAddToInt ///////////////////////////////////////////////////////////
int				lvCAddToInt::CallFunction(){
	be_CINT* pInt = (be_CINT*)(IntValue.Get());
	if (pInt!=NULL){
		pInt->Value += IntAdd;
		return 1;
	};
    return 0;
};
// lvCAddToIntEx ///////////////////////////////////////////////////////////
int				lvCAddToIntEx::CallFunction(){
	be_CINT* pInt = (be_CINT*)(IntValue.Get());
	if (pInt!=NULL){
		pInt->Value += IntAdd;
		return 1;
	};
    return 0;
};
// lvCKillNUnits /////////////////////////////////////////////////////////
int				lvCKillNUnits::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP) {
		pvGRP->KillUnits(UCount);
		return 1;
	};
	return 0;
};
// lvCEraseNUnits ////////////////////////////////////////////////////////
int				lvCEraseNUnits::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP) {
		pvGRP->EraseUnits(UCount);
		return 1;
	};
	return 0;
};
// lvCSavePosition ///////////////////////////////////////////////////////
int				lvCSavePosition::CallFunction(){
	vvPOINT_SET* vPS=(vvPOINT_SET*)(VVpPos.Get());
	vvPOINT2D*	 vvP=(vvPOINT2D*)(vvPoint.Get());
	if (vPS==NULL&&vvP==NULL) {
		return 0;
	};
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP==NULL)	return 0;
	if (UseVV=true&&vPS!=NULL){
		int c=pvGRP->GetTotalAmount();
		vPS->CleanARR();
		while (c--) {
			OneObject* OB=pvGRP->GetOneObj_lID(c);
			if(OB){
				vPS->AddPoint(OB->x,OB->y);
			};
		};
		return 1;
	}else if (vvP!=NULL) {
		int xcc,ycc;
		pvGRP->GetGroupCenter(xcc,ycc);
		if (xcc!=0||ycc!=0) {
			vvP->SetTR(xcc,ycc);
		};
		return 1;
	};
	return 0;
};
DWORD			lvCSavePosition::GetClassMask(){
	if (UseVV==true)	return 0x00010000;
	else				return 0x00020000;
};
int SavePosition_lua(be_CGROUP* pGrp,vvPOINT2D* pPos){
	if (pGrp==NULL||pPos==NULL) return 0;
	int xcc,ycc;
	pGrp->GetGroupCenter(xcc,ycc);
	pPos->SetTR(xcc,ycc);
	return 1;
}
int SavePositionArr_lua(be_CGROUP* pGrp,vvPOINT_SET* pPosArr){
	if (pGrp==NULL||pPosArr==NULL) return 0;
	int c=pGrp->GetTotalAmount();
	pPosArr->CleanARR();
	while (c--) {
		OneObject* OB=pGrp->GetOneObj_lID(c);
		if(OB){
			pPosArr->AddPoint(OB->x,OB->y);
		};
	};
	return 1;
};
// lvCSendToPosition /////////////////////////////////////////////////////
int				lvCSendToPosition::CallFunction(){
	vvPOINT_SET* vPS=(vvPOINT_SET*)(VVpPos.Get());
	vvPOINT2D*	 vvP=(vvPOINT2D*)(vvPoint.Get());
	if (vPS==NULL&&vvP==NULL) {
		return 0;
	};
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) {
		if (UsePoint==true){
			pvGRP->SendTo(vvP->GetX(),vvP->GetY(),512,0);
		}else{
			pvGRP->SendToPosition(vPS);
		};
		return 1;
	};
	return 0;
};
DWORD	lvCSendToPosition::GetClassMask(){
	if (UsePoint==true)	return 0x00020000;
	else				return 0x00010000;
};
// lvCSetRessource ///////////////////////////////////////////////////////
int				lvCSetRessource::CallFunction(){
	if ( GetClassMask()&0x00000002 ){
		be_CINT* pNation = (be_CINT*)refNation.Get();
		if ( pNation )	Nation = pNation->Value;
		else			return 0;
	};
	if (UseVV==true){
		be_CINT* pInt = (be_CINT*)(WoodVV.Get());
		if (pInt!=NULL) {
			SetResource(Nation,0,pInt->Value);		// WOOD
		};
		pInt = (be_CINT*)(GoldVV.Get());
		if (pInt!=NULL) {
			SetResource(Nation,1,pInt->Value);		// GOLD
		};
		pInt = (be_CINT*)(StownVV.Get());
		if (pInt!=NULL) {
			SetResource(Nation,2,pInt->Value);		// STOWN
		};
		pInt = (be_CINT*)(FoodVV.Get());
		if (pInt!=NULL) {
			SetResource(Nation,3,pInt->Value);		// FOOD
		};
		pInt = (be_CINT*)(IronVV.Get());
		if (pInt!=NULL) {
			SetResource(Nation,4,pInt->Value);		// IRON 
		};
		pInt = (be_CINT*)(CoalVV.Get());
		if (pInt!=NULL) {
			SetResource(Nation,5,pInt->Value);		// COAL
		};
		pInt = (be_CINT*)(MoneyVV.Get());
		if (pInt!=NULL) {
			SetResource(Nation,6,pInt->Value);		// MONEY
		};
		return 1;
	}else{
		SetResource(Nation,0,Wood);		// WOOD
		SetResource(Nation,1,Gold);		// GOLD
		SetResource(Nation,2,Stown);	// STOWN
		SetResource(Nation,3,Food);		// FOOD
		SetResource(Nation,4,Iron);		// IRON 
		SetResource(Nation,5,Coal);		// COAL
		SetResource(Nation,6,Money);	// MONEY
		return 1;
	};
	return 0;
};

DWORD			lvCSetRessource::GetClassMask(){
	DWORD mask = 0x00000001;
	if (isElementInParentClass("InArgList"))	mask = 0x00000002;
	if (UseVV==true)	mask += 0x00010000;
	else				mask += 0x00020000;
	return mask;
};
int SetResource_lua(int nat,int resID,int Amount){
	if ( 0<=nat&&nat<8 && 0<=resID&&resID<6 ){
		SetResource(nat,resID,Amount);
		return 1;
	};
	return 0;
};
// lvCAddRessource ///////////////////////////////////////////////////////
int				lvCAddRessource::CallFunction(){
	
	addrand(RessType);
	addrand(Nat);
	addrand(Use_VV);
	addrand(Value);

	if ( GetClassMask()&0x00000002 ){
		be_CINT* pNat = (be_CINT*)refNat.Get();
		if ( pNat )	Nat = pNat->Value;
		else		return 0;
	};

	addrand(12345);

	if (Nat<0||Nat>7) return 0;
	if (Use_VV) {
		be_CINT* pvValue = (be_CINT*)(vValue.Get());
		if (pvValue!=NULL) {
			addrand(pvValue->Value);
			AddResource(Nat,RessType,pvValue->Value);
			return 1;
		};
	}else{
		addrand(Value);
		AddResource(Nat,RessType,Value);
		return 1;
	};
	return 0;
};
DWORD			lvCAddRessource::GetClassMask(){
	DWORD mask = 0x00000001;
	if (isElementInParentClass("InArgList"))	mask = 0x00000002;
	if (Use_VV==true)	mask += 0x00020000;
	else				mask += 0x00010000;
	return mask;
};
int AddRessource_lua(int nat,int resID,int Amount){
	if ( 0<=nat&&nat<8 && 0<=resID&&resID<8 ){
		addrand(Amount);
		AddResource(nat,resID,Amount);
		return 1;
	};
	return 0;
};
// lvCStartAIEx //////////////////////////////////////////////////////////
int				lvCStartAIEx::CallFunction(){
	if (EngSettings.GameName==1){
		ActivateTacticalAI(Nation);
		return 1;
	};
	if (Use_VV){
		be_CINT* pInt = (be_CINT*)(vDifficulty.Get());
		if (pInt!=NULL) {
			StartAIEx(Nation,NameXML.pchar(),Land,Money,ResOnMap,pInt->Value);
			return 1;
		};
	};
	if (!Use_VV){
		StartAIEx(Nation,NameXML.pchar(),Land,Money,ResOnMap,Difficulty);
		return 1;
	};
	return 0;
};

DWORD			lvCStartAIEx::GetClassMask(){
	if (Use_VV==true)	return 0x00010000;
	else				return 0x00020000;
};
int ActivateTacticalAI_lua(int nat){
	if (EngSettings.GameName==1) ActivateTacticalAI(nat);
	return 1;
};
int StartAIEx_lua(int nat,const char* file,int lend,int mony,int res,int diff){
	_str FName; FName=file;
	StartAIEx(nat,FName.pchar(),lend,mony,res,diff);
	return 1;
};
// lvCSetAIEnableState ///////////////////////////////////////////////////
int				lvCSetAIEnableState::CallFunction(){
	return SetAIEnableState_lua(Nat,State);
};
int SetAIEnableState_lua(int nat,bool state){
	if(0<=nat&&nat<8){
		NATIONS[nat].AI_Enabled=state;
		return 1;
	};
	return 0;
}
// lvCSetCamera //////////////////////////////////////////////////////////
int				lvCSetCamera::CallFunction(){
	be_CCAMERA* pCamera = (be_CCAMERA*)(camera.Get());
	if ( pCamera!=NULL ){
//		ICam->SetPosition(pCamera->pos);
//		ICam->SetDirection(pCamera->dir);
//		ICam->SetLookAt(pCamera->lookAt);
//		BE_NewSetCamera();
		return 1;
	};
	return 0;
};
// lvCScrollCamera ///////////////////////////////////////////////////////
int		lvCScrollCamera::CallFunction(){
	be_CNODE* pStartNode = (be_CNODE*)(StartPos.Get());
	be_CNODE* pDestNode  = (be_CNODE*)(DestPos.Get());
	if ( pDestNode!=NULL && ( ScrollFromCurPos || pStartNode!=NULL ) ) {
		if ( ScrollFromCurPos ){
			CameraDriver()->Set(pDestNode->x,pDestNode->y,ScrollTime);	
			CameraDriver()->Set(true);
		}else{
			Vector3D	_startPos(	
									(float)pStartNode->x,
									(float)pStartNode->y
								 );
			Vector3D	_destPos(	
									(float)pDestNode->x,
									(float)pDestNode->y
								);
			CameraDriver()->Set(&_startPos,&_destPos,ScrollTime,0);	
			CameraDriver()->Set(true);
		}
		return 1;
	};
	return 0;
};
// lvCMoveCamera /////////////////////////////////////////////////////////
int				lvCMoveCamera::MoveType(){
	bool	bPOS0 = (POS0.Get()!=NULL);
	bool	bPOS1 = (POS1.Get()!=NULL);
	bool	bDIR0 = (DIR0.Get()!=NULL);
	bool	bDIR1 = (DIR1.Get()!=NULL);
	if (bPOS0 && !bPOS1 && !bDIR0 && !bDIR1)	return	1;
	if (bPOS0 && !bPOS1 &&  bDIR0 && !bDIR1)	return	2;
	if (bPOS0 &&  bPOS1 && !bDIR0 && !bDIR1)	return	3;
	if (bPOS0 &&  bPOS1 &&  bDIR0 &&  bDIR1)	return	4;
};

int				lvCMoveCamera::CallFunction(){
	if (Time==0){
//		CameraDriver()->MoveType=lvCRotateCamera::enMoveType::beTO_FREE;
		CameraDriver()->Stop();
		return 1;
	};

	vvVector3D* pPOS0 = (vvVector3D*)(POS0.Get());
	vvVector3D* pPOS1 = (vvVector3D*)(POS1.Get());
	vvVector3D* pDIR0 = (vvVector3D*)(DIR0.Get());
	vvVector3D* pDIR1 = (vvVector3D*)(DIR1.Get());

	Vector3D	Pos0(0.f,0.f,0.f);
	Vector3D	Pos1(0.f,0.f,0.f);
	Vector3D	Dir0(0.f,0.f,0.f);
	Vector3D	Dir1(0.f,0.f,0.f);
	int	ttt = MoveType();
	switch(ttt) {
	case 1:
		{
			Pos0.set(pPOS0->x,pPOS0->y,pPOS0->z);
			CameraDriver()->Set(&Pos0,Time);
		}
		break;
	case 2:
		{
			Pos0.set(pPOS0->x,pPOS0->y,pPOS0->z);
			Dir0.set(pDIR0->x,pDIR0->y,pDIR0->z);
			CameraDriver()->Set(&Pos0,&Dir0,Time);
		}
		break;
	case 3:
		{
			if (!useMapXY){
				Pos0.set(pPOS0->x,pPOS0->y,pPOS0->z);
				Pos1.set(pPOS1->x,pPOS1->y,pPOS1->z);
				CameraDriver()->Set(&Pos0,&Pos1,Time,0);
			}else{
                Pos0.set(pPOS0->mX,pPOS0->mY,0.f);
				Pos1.set(pPOS1->mX,pPOS1->mY,0.f);
				CameraDriver()->Set(&Pos0,&Pos1,Time,0);
			};
		}
		break;
	case 4:
		{
			Pos0.set(pPOS0->x,pPOS0->y,pPOS0->z);
			Dir0.set(pDIR0->x,pDIR0->y,pDIR0->z);
			Pos1.set(pPOS1->x,pPOS1->y,pPOS1->z);
			Dir1.set(pDIR1->x,pDIR1->y,pDIR1->z);
			CameraDriver()->Set(&Pos0,&Dir0,&Pos1,&Dir1,Time);
		}
		break;
	};
	CameraDriver()->Set(useMapXY);
	return 1;
};
// lvCAttachCameraToGroup ////////////////////////////////////////////////
int				lvCAttachCameraToGroup::CallFunction(){
	be_CGROUP*	pvGrp = (be_CGROUP*)(vGrpID.Get());
	if (pvGrp!=NULL) {
		CameraDriver()->Set(vGrpID);
		return 1;
	};
	return 0;
};

// lvCFreeCamera /////////////////////////////////////////////////////////
int				lvCFreeCamera::CallFunction(){
	CameraDriver()->Stop();
	return 1;
};

// lvCShowDialog /////////////////////////////////////////////////////////
void ProcessScreen();
int				lvCShowDialog::CallFunction(){
	vvDIALOG* pDlg = (vvDIALOG*)(Dialog.Get());
	if ( pDlg!=NULL )
		return ShowDialog_lua(pDlg);
	return 0;
};
int ShowDialog_lua(vvDIALOG* pDLG){
	if (pDLG!=NULL){
		DialogsSystem* pDialog  = reinterpret_cast<DialogsSystem*>(pDLG->Get());
		if (pDialog==NULL)	return 0;
		int OldItemChoose = ItemChoose;
		while (ItemChoose!=0) {
			ProcessScreen();
			ProcessMessages();
			pDialog->ProcessDialogs();
			pDialog->RefreshView();
		};
		ItemChoose = OldItemChoose;
		return 1;
	};
	return 0;
}
// lvCAddTextToDlg ///////////////////////////////////////////////////////
int				lvCAddTextToDlg::CallFunction(){
	vvDIALOG* pDlg = (vvDIALOG*)(Dialog.Get());
	vvTEXT*   pTxt = (vvTEXT*)(Text.Get());
	if (pDlg!=NULL&&pTxt!=NULL) {
		return AddTextToDlg_lua(pDlg,pTxt);
	};
	return 0;
};
int AddTextToDlg_lua(vvDIALOG* pDLG,vvTEXT* pTXT){
	if (pDLG!=NULL&&pTXT!=NULL){
		DialogsSystem* pDialog  = reinterpret_cast<DialogsSystem*>(pDLG->Get());
		ListDesk* pLD = dynamic_cast<ListDesk*>(pDialog->Find("MainDesck"));
		if (pLD!=NULL){
			vvTEXT* pText = pTXT;
			pLD->AddElement(GetTextByID(pText->TextID.pchar()));
			return 1;
		};
	};
	return 0;
};
// lvCClearDialog ////////////////////////////////////////////////////////
int				lvCClearDialog::CallFunction(){
	vvDIALOG* pDlg = (vvDIALOG*)(Dialog.Get());
	if (pDlg!=NULL) {
		return ClearDialog_lua((vvDIALOG*)(Dialog.Get()));
	};
	return 0;
};
int ClearDialog_lua(vvDIALOG* pDLG){
	if (pDLG==NULL) return 0;
	DialogsSystem* pDialog  = reinterpret_cast<DialogsSystem*>(pDLG->Get());
	if (pDialog!=NULL){
		ListDesk* pLD = dynamic_cast<ListDesk*>(pDialog->Find("MainDesck"));
		if (pLD!=NULL){
			// Delete all objects
			pLD->DSS.Clear();
			return 1;
		};
	};
	return 0;
};
// lvCSetScrollLimit /////////////////////////////////////////////////////
int				lvCSetScrollLimit::CallFunction(){
	be_CNODE* pLT = (be_CNODE*)(PosLT.Get());
	be_CNODE* pRB = (be_CNODE*)(PosRB.Get());
	if (pLT!=NULL&&pRB!=NULL) {
		return	SetScrollLimit00_lua(pLT->x,pLT->y,pRB->x,pRB->y,LockAroud);
	};
	return 0;
};
int SetScrollLimit_lua(vvPOINT2D* pPosLT,vvPOINT2D* pPosRB,bool LockAroud){
	if (pPosLT!=NULL&&pPosRB!=NULL){
		int x0 = pPosLT->Value.x;
		int y0 = pPosLT->Value.y;
		int x1 = pPosRB->Value.x;
		int y1 = pPosRB->Value.y;
		SetScrollLimit00_lua(x0,y0,x1,y1,LockAroud);
		return 1;
	};
	return	0;
};
int SetScrollLimit00_lua(int x0,int y0,int x1, int y1,bool LockAroud){
	void SetScrollLimitations(int x0,int y0,int x1,int y1);
	SetScrollLimitations(x0,y0,x1,y1);
	// set lock areas around scrool limitation
	if (LockAroud==true){
		int xl0 = x0/16;
		int yl0 = y0/16;
		int xl1 = x1/16;
		int yl1 = y1/16;
		BSetSQ(xl0-1,yl0-1,xl1-xl0+5,2);
		BSetSQ(xl0-1,yl1+1,xl1-xl0+5,2);
		BSetSQ(xl0-1,yl0-1,2,yl1-yl0+5);
		BSetSQ(xl1+1,yl0-1,2,yl1-yl0+5);
	};
	return	1;
}
// lvCSetScrollLimitNation /////////////////////////////////////////////////////
int				lvCSetScrollLimitNation::CallFunction(){
	be_CNODE* pLT = (be_CNODE*)(PosLT.Get());
	be_CNODE* pRB = (be_CNODE*)(PosRB.Get());
	if (pLT != NULL && pRB != NULL) {
		return	SetScrollLimitNation00_lua(pLT->x, pLT->y, pRB->x, pRB->y, Nation, LockAroud);
	};
	return 0;
};
int SetScrollLimitNation_lua(vvPOINT2D* pPosLT, vvPOINT2D* pPosRB, int Nation, bool LockAroud){
	if (pPosLT != NULL && pPosRB != NULL)
	{
		int x0 = pPosLT->Value.x;
		int y0 = pPosLT->Value.y;
		int x1 = pPosRB->Value.x;
		int y1 = pPosRB->Value.y;
		SetScrollLimitNation00_lua(x0,y0,x1,y1,Nation,LockAroud);
		return 1;
	};
	return	0;
};
int SetScrollLimitNation00_lua(int x0,int y0,int x1, int y1, int Nation, bool LockAroud){
	void SetScrollLimitationsForNation(int x0,int y0,int x1,int y1, int Nation);
	SetScrollLimitationsForNation(x0,y0,x1,y1, Nation);
	// set lock areas around scrool limitation
	if (LockAroud==true)
	{
		int xl0 = x0/16;
		int yl0 = y0/16;
		int xl1 = x1/16;
		int yl1 = y1/16;
		BSetSQ(xl0-1,yl0-1,xl1-xl0+5,2);
		BSetSQ(xl0-1,yl1+1,xl1-xl0+5,2);
		BSetSQ(xl0-1,yl0-1,2,yl1-yl0+5);
		BSetSQ(xl1+1,yl0-1,2,yl1-yl0+5);
	};
	return	1;
}
// lvCSpotNUnits /////////////////////////////////////////////////////////
lvTypeNum::lvTypeNum(lvTypeNum* pTypeNum){
	if (pTypeNum!=NULL) {
		InfID		= pTypeNum->InfID;
		vgUnitsID	= pTypeNum->vgUnitsID;
		UnitType	= pTypeNum->UnitType;
		Use_VV		= pTypeNum->Use_VV;
		Num			= pTypeNum->Num;
		vNum		= pTypeNum->vNum;
	};
};

void			lvTypeNum::GetCopy(lvTypeNum** pCopy){
	*pCopy = new lvTypeNum(this);
};

bool			lvTypeNum::Prepare(){
	if (Use_VV){
		if (vNum.Get()!=NULL){
			Num = ((be_CINT*)(vNum.Get()))->Value;
		}else{
			return false;
		};
	};
	return true;
};

DWORD			lvTypeNum::GetClassMask(){
	if (Use_VV==true)	return 0x00010000;
	else				return 0x00020000;
};

const	char*	lvTypeNum::GetThisElementView(const char* LocalName){

	char* Presentation = GetGlobalBuffer();
	CE_WCP.GetPresentation(Presentation,this);
	return Presentation;

};
int				lvCSpotNUnits::CallFunction(){
	Create();
	if (Use_VV){
		if (vProduceTime.Get()!=NULL){
			ProduceTime = ((be_CINT*)(vProduceTime.Get()))->Value;
		}else{
			return 0;
		};		
	};
	for (int i=0; i<TypeNum.GetAmount(); i++){
		if (TypeNum[i]->Prepare()==false)	return 0;
	};
	pUnitsGRP->RefreshUnits();
	switch(Status) {
	case 0:
		if (GetTickCount()-LastProdTime>=ProduceTime&&pUnitsGRP->N==0){
			be_CGROUP* pvBLD = (be_CGROUP*)(vgBildsID.Get());
			if (pvBLD!=NULL) {
				OneObject* pOB = pvBLD->GetOneObj_lID(0);
				if (pOB!=NULL) {
                    // Выбираем че нам строить.
					be_CGROUP* pvg = NULL;
					for (int i=0; i<TypeNum.GetAmount(); i++){
						pvg = (be_CGROUP*)(TypeNum[i]->vgUnitsID.Get());
						if (pvg!=NULL) {
							if (pvg->GetTotalAmount2(TypeNum[i]->UnitType)<TypeNum[i]->Num){
								pOB->Produce(TypeNum[i]->UnitType,pUnitsGRP->Index);
								TypePID = i;
								i = TypeNum.GetAmount();

								Order1* Ord=pOB->LocalOrder;
								if (Ord!=NULL){
									while(Ord->NextOrder){
										Ord=Ord->NextOrder;
									};
									Ord->info.Produce.Progress = Ord->info.Produce.NStages;
								};
								LastProdTime = GetTickCount();
								Status = 1;
							};
						};
					};
				};
			};
		}else if (pUnitsGRP->N>0&&GetTickCount()-LastProdTime>=ProduceTime){
			Status=1;
		};
		return 1;
		break;
	case 1:
		if (pUnitsGRP->N==0&&GetTickCount()-LastProdTime>=ProduceTime){
			Status=0;
			LastProdTime = GetTickCount();
		};
		if (pUnitsGRP->N>0) {
			int N=pUnitsGRP->N;
			while (N--) {
				OneObject* pOB = Group[pUnitsGRP->IDS[N]];
				if (pOB!=NULL&&(!pOB->Sdoxlo||pOB->Hidden)){
					be_CGROUP* pvUNIT = (be_CGROUP*)(TypeNum[TypePID]->vgUnitsID.Get());
					if (pvUNIT!=NULL){
						pvUNIT->AddUnitGPT(pOB);
						addname(pOB->Index);
						pvUNIT->GetTotalAmount();
						pUnitsGRP->RemoveUnit(pOB);
					};
				};
			};
		};
		return 1;
		break;
	};
	return 0;
};

DWORD			lvCSpotNUnits::GetClassMask(){
	if (Use_VV==true)	return 0x00010000;
	else				return 0x00020000;
};
void			lvCSpotNUnits::Create(){
	if (UGRP_ID==0xFFFF){
		pUnitsGRP = SCENINF.GetNewGroup();
		UGRP_ID = pUnitsGRP->Index;
		LastProdTime = GetTickCount();
		be_CGROUP* pvBLD = (be_CGROUP*)(vgBildsID.Get());
		OneObject* OB = pvBLD->GetOneObj_lID(0);
		if(OB&&OB->GetLiveState()==ls_LiveBody&&(!OB->LocalOrder)&&OB->Ref.General->CanDest){
			be_CNODE* pNode = (be_CNODE*)(parNode.Get());
			if (pNode!=NULL) {
				OB->DestX=pNode->vGetX();
				OB->DestY=pNode->vGetY();
			};
		};
	};
	pUnitsGRP = &(SCENINF.UGRP[UGRP_ID]);
};
// lvCGoInBattle /////////////////////////////////////////////////////////
lvGrpNumBld::lvGrpNumBld(lvGrpNumBld* pGrpNumBld){
	if (pGrpNumBld!=NULL) {
/*
        be_CBaseUnaryOperator* pCond=NULL;
		for (int i=0; i<pGrpNumBld->Cond.GetAmount(); i++){
			pGrpNumBld->Cond[i]->GetCopy(&pCond);
			if (pCond!=NULL) {
				Cond.Add(pCond);
			};
			pCond=NULL;
		};
*/
		vgUnits			= pGrpNumBld->vgUnits;
		vgBildg			= pGrpNumBld->vgBildg;
		Use_VV			= pGrpNumBld->Use_VV;
		Num				= pGrpNumBld->Num;
		vNum			= pGrpNumBld->vNum;
		AttackerType	= pGrpNumBld->AttackerType;
		Diff			= pGrpNumBld->Diff;
	};
};

DWORD			lvGrpNumBld::GetClassMask(){
	if (Use_VV==true)	return 0x00010000;
	else				return 0x00020000;
};

void			lvGrpNumBld::GetCopy(lvGrpNumBld** pCopy){
	*pCopy = new lvGrpNumBld(this);
};

const	char*	lvGrpNumBld::GetThisElementView(const char* LocalName){

	char* Presentation = GetGlobalBuffer();
	CE_WCP.GetPresentation(Presentation,this);
	return Presentation;

};
bool			lvGrpNumBld::Prepare(){
	if (Use_VV){
		if (vNum.Get()!=NULL){
			Num = ((be_CINT*)(vNum.Get()))->Value;
		}else{
			return false;
		};
	};
	return true;
};

void			lvGrpNumBld::CallFunction(){
	if (Cond.CallFunction()==true) {
		be_CGROUP* pvgUnits = (be_CGROUP*)(vgUnits.Get());
		be_CGROUP* pvgBuild = (be_CGROUP*)(vgBildg.Get());
		if (pvgUnits==NULL)	return;
		if ( pvgUnits->GetTotalAmount()>=Num || ( pvgBuild!=NULL && pvgBuild->GetTotalAmount()==0 && pvgUnits->GetTotalAmount()>0 ) ){
			switch(AttackerType) {
			case 0:
				AddFirers(pvgUnits,pvgUnits->GetNation(),RemoveAfterSend);
				break;
			case 1:
				AddTomahawks(pvgUnits,pvgUnits->GetNation(),RemoveAfterSend,0,0,0);
				break;
			case 2:
				AddPsKillers(pvgUnits,pvgUnits->GetNation(),RemoveAfterSend,false);
				break;
			case 3:
				AddStorm(pvgUnits,pvgUnits->GetNation(),Diff,RemoveAfterSend);
				break;
			};
		};
	};
};

int				lvCGoInBattle::CallFunction(){
	int N = GrpNumBld.GetAmount();
	while (N--) {
		if (GrpNumBld[N]->Prepare()){
			GrpNumBld[N]->CallFunction();
		};
	};
	return 1;
};
// lvCArtAttack //////////////////////////////////////////////////////////
int				lvCArtAttack::CallFunction(){
	be_CBOOL* pTrg = (be_CBOOL*)(AttackActive.Get());
	if (pTrg!=NULL&&pTrg->Value==true){
		be_CGROUP* pvgArt  = (be_CGROUP*)(vgArtID.Get());		
		be_CGROUP* pvgTarg = (be_CGROUP*)(vgTargID.Get());
		if (pvgTarg!=NULL&&pvgTarg->GetTotalAmount()==0){
			return 1;
		};
		if (pvgArt!=NULL&&pvgTarg!=NULL){
			if (!pvgArt->GetORDER()) {
				// Select target
				int N = pvgTarg->GetTotalAmount();
				OneObject* pOTrg = NULL;
				while (pOTrg==NULL&&N--) {
					pOTrg = pvgTarg->GetOneObj_lID(N);
				};
				assert(pOTrg);
				if (pOTrg==NULL)	return 1;
				// Attack
				N = pvgArt->GetTotalAmount();
				OneObject* pOArt = NULL;
				while (N--) {
					pOArt = pvgArt->GetOneObj_lID(N);
					if (pOArt!=NULL) {
						int minAD = (int)pOArt->MoreCharacter->MinR_Attack;
						int maxAD = (int)pOArt->MoreCharacter->MaxR_Attack;
						int Dist = Norma((pOArt->RealX-pOTrg->RealX)>>4,(pOArt->RealY-pOTrg->RealY)>>4);
						if (minAD<Dist&&Dist<maxAD){
							pOArt->AttackObj(pOTrg->Index,128+15,0,0);
						};
					//	pOArt->AttackObj(pOTrg->Index,128+15,0,0);
					};
				};
			};
		};
	};
	return 0;
};
// lvCPutNewSquad ////////////////////////////////////////////////////////
int				lvCPutNewSquad::CallFunction(){
	if (Nat<0||Nat>=7)	return 0;
	be_CGROUP*	pGrp = (be_CGROUP*)(GrpID.Get());
	if (pGrp!=NULL){
		if (pGrp->GetTotalAmount()!=0&&pGrp->GetNation()!=Nat)	return 0;
		be_CNODE* pNode = (be_CNODE*)(parNode.Get());
		int x,y;
		if (pNode!=NULL) {
			x = pNode->vGetX();
			y = pNode->vGetY();
		}else return 0;
		
		int FormT = SizeType/100;
		int newBrigID = PutNewFormationBySizeIndex((byte)Nat,(word)UnitType,SizeType%100,FormT,x<<4,y<<4,(byte)dir);
		if (newBrigID!=0xFFFF){
			Brigade* pBR=CITY[Nat].Brigs+newBrigID;
			if (pBR!=NULL){
				for (int i=0; i<pBR->NMemb&&pBR->Memb!=NULL; i++){
					pGrp->AddUnitGID(pBR->Memb[i]);
					addrand(pBR->Memb[i]);
				};
				return 1;
			};
		};
	};
	return 0;
};

int GetUTypeByName_lua(const char* UTName){
	int TID=-1;
	for(int i=0;(TID==-1)&&(i<NATIONS->NMon);i++){
		if(NATIONS->Mon[i]->MonsterID && !strcmp(UTName,NATIONS->Mon[i]->MonsterID)){
			TID=i;
		}
	}
	return TID;
};
int PutNewSquad_lua(be_CGROUP* pGRP,int nat,int uType,int size,int x,int y,int dir){
	int newBrigID = PutNewFormationBySizeIndex((byte)nat,(word)uType,size%100,size/100,x<<4,y<<4,(byte)dir);
	if (newBrigID!=0xFFFF){
		Brigade* pBR=CITY[nat].Brigs+newBrigID;
		if (pBR!=NULL){
			for (int i=0; i<pBR->NMemb&&pBR->Memb!=NULL; i++){
				pGRP->AddUnitGID(pBR->Memb[i]);
				addrand(pBR->Memb[i]);
			};
			return 1;
		};
	};
	return 0;
};
// lvCPutNewFormation ////////////////////////////////////////////////////
int				lvCPutNewFormation::CallFunction(){
	be_CGROUP*	pGrp = (be_CGROUP*)(vGrpID.Get());
	int xc=-1;
	int yc=-1;
	if (Use_Zone) {
		be_CNODE* pNode = (be_CNODE*)(parNode.Get());
		if (pNode!=NULL) {
			xc = pNode->vGetX()<<4;
			yc = pNode->vGetY()<<4;
		};
	}else{
		vvPOINT2D* pPOS = (vvPOINT2D*)(Point.Get());
		if (pPOS!=NULL) {
			xc = (pPOS->Value.x)<<4;
			yc = (pPOS->Value.y)<<4;
		};
	};

	if (pGrp==NULL||Nat<0||Nat>7||Form<0||Form>255||xc==-1||yc==-1)		return 0;

	OrderDescription* ODS=ElementaryOrders+Form;
	int N=ODS->NUnits;
	PORD.CreateSimpleOrdPos(xc,yc,dir,ODS->NUnits,NULL,ODS);
	word NewIds[1024];
	int NU=0;
	for(int j=0;j<N;j++){
		int CreateNewTerrMons3(byte NI,int x,int y,word Type,int RP);
		int ID=CreateNewTerrMons3(Nat,PORD.px[j],PORD.py[j],UType,ReadyPercent);
		if(ID!=-1&&NU<1024){
			NewIds[NU]=ID;
			NU++;
		};
	};
	for(int j=0;j<NU;j++){
		pGrp->AddUnitGID(NewIds[j]);
		addrand(NewIds[j]);
	};
	return 1;
};

DWORD			lvCPutNewFormation::GetClassMask(){
	if (Use_Zone==true)	return 0x00010000;
	else				return 0x00020000;
};

int				lvCRepairBuildings::CallFunction(){
	be_CGROUP*	pGrp = (be_CGROUP*)(vGrpID.Get());
	be_CGROUP*	pBld = (be_CGROUP*)(vBuildings.Get());
	int NUP=pGrp->GetTotalAmount();
	int NB=pBld->GetTotalAmount();
	for(int j=0;j<NB;j++){
		OneObject* OBB=pBld->GetOneObj_lID(j);
		if(OBB && OBB->NewBuilding && OBB->Life<OBB->MaxLife){				
			for(int i=0;i<NUP;i++){
				OneObject* OBP=pGrp->GetOneObj_lID(i);		
				if(OBP){				
					OBP->BuildObj(OBB->Index,128+16,true,0);
				}		
			}
			OBB->ClearBuildPt();
		}
	}
	return 1;
}

// lvCPutNewFormationParametric ////////////////////////////////////////////////////
int				lvCPutNewFormationParametric::CallFunction(){
	
	be_CARRAY* pArr = g_beVAR_STORE.ARRAY_LIST.GetByName(arrayName.pchar());
	if (pArr == NULL) return 0;
	int Index;
	if (UseVar) Index = ((be_CINT*)(parIndex.Get()))->Value;
	else Index = ArrIndex;
	if (Index > pArr->GetAmount() || Index < 1) return 0;
	be_CGROUP*	pGrp = (be_CGROUP*)(pArr->GetValue(Index));
	
	int xc=-1;
	int yc=-1;
	
	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL) 
	{
		xc = pNode->vGetX()<<4;
		yc = pNode->vGetY()<<4;
	};

	if (pGrp==NULL||Nat<0||Nat>7||UNum<0||UNum>255||xc==-1||yc==-1)		return 0;

	OrderDescription* ODS=ElementaryOrders;
	PORD.CreateSimpleOrdPos(xc,yc,dir,UNum,NULL,ODS);
	word NewIds[1024];
	int NU=0;
	for(int j=0;j<UNum;j++){
		int CreateNewTerrMons3(byte NI,int x,int y,word Type,int RP);
		int ID=CreateNewTerrMons3(Nat,PORD.px[j],PORD.py[j],UType,ReadyPercent);
		if(ID!=-1&&NU<1024){
			NewIds[NU]=ID;
			NU++;
		};
	};
	for(int j=0;j<NU;j++){
		pGrp->AddUnitGID(NewIds[j]);
		addrand(NewIds[j]);
	};
	return 1;
};

DWORD			lvCPutNewFormationParametric::GetClassMask(){
	if (UseVar==true)	return 0x00010000;
	else				return 0x00020000;
};

//extern int NEOrders;
//extern OrderDescription ElementaryOrders [256];
int GetFormationID_lua(const char* formID){
	for(int i=0;i<NEOrders;i++)if(!strcmp(formID,ElementaryOrders[i].ID))return i;
	return 0xFFFFFFFF;
};
int PutNewFormation_lua(be_CGROUP* pGRP,int nat,int uType,int form,int x,int y,int dir){
	if (pGRP==NULL||nat<0||nat>7||form<0||form>255)		return 0;
	OrderDescription* ODS=ElementaryOrders+form;
	int N=ODS->NUnits;
	PORD.CreateSimpleOrdPos(x<<4,y<<4,dir,ODS->NUnits,NULL,ODS);
	word NewIds[1024];
	int NU=0;
	for(int j=0;j<N;j++){
		int CreateNewTerrMons2(byte NI,int x,int y,word Type);
		int ID=CreateNewTerrMons2(nat,PORD.px[j],PORD.py[j],uType);
		if(ID!=-1&&NU<1024){
			NewIds[NU]=ID;
			NU++;
		};
	};
	for(int j=0;j<NU;j++){
		pGRP->AddUnitGID(NewIds[j]);
		addrand(NewIds[j]);
	};
	return 1;
};
int PutRandFormation_lua	(be_CGROUP* pGRP,int nat,int uType,int UN,int x,int y,int r){
	if (pGRP==NULL||nat<0||nat>7||r<=0)		return 0;
	int it = 3,rx,ry,id,run=0;
	for ( int i=0; i<UN; i++ ){
		while (it--) {
			rx = x + rando()%r - r/2;
			ry = y + rando()%r - r/2;
			SetMyActiveSubset();			
			id = NATIONS[nat].CreateNewMonsterAt(rx*16,ry*16,uType,false,rando()%256);
			SetDefaultActiveSubset();
			if (id!=-1) {
				pGRP->AddUnitGID(id);
				addrand(id);
				run++;
				break;
			};
		};
		it = 3;
	};
	return run;
};
// lvCSetUnitStateCII ///////////////////////////////////////////////////////
DWORD			lvCSetUnitStateCII::GetClassMask(){
	if (Fire==true)	return 0x00010000;
	else			return 0x00020000;
};

int				lvCSetUnitStateCII::CallFunction(){
	be_CGROUP*	pGrp = (be_CGROUP*)(GrpID.Get());
	if (pGrp==NULL)	return 0;

	if (SetSG_Immediately){
		pGrp->SetInStandGround();
		return 1;
	};

	int Nat = pGrp->GetNation();
	if (Nat<0||Nat>7)	return 0;
	// Set RifleAttack, Brigade not need.
	int N = pGrp->GetTotalAmount();
	OneObject* pOB = NULL;
	bool	FireState = Fire;
	if (Fire&&(LineI||LineII||LineIII))	FireState=false;
	while (N--) {
		pOB = pGrp->GetOneObj_lID(N);
		if (pOB!=NULL) {
			pOB->RifleAttack=FireState;
		};
	};
	// Finde Brigade by line.
	N = pGrp->GetTotalAmount();
	Brigade* pBR = NULL;
	pOB = NULL;
	while (pBR==NULL&&N--) {
		pOB = pGrp->GetOneObj_lID(N);
		if (pOB!=NULL&&pOB->BrigadeID!=0xFFFF) {
			pBR = CITY[Nat].Brigs+pOB->BrigadeID;
		};
	};
	if (pBR==NULL)	return 0;
	if (Fire&&(LineI||LineII||LineIII)){
		void ComShotLine(byte NI,byte State,int Line,int BID);
		if (LineI)		ComShotLine(Nat,0,4,pBR->ID);
		if (LineII)		ComShotLine(Nat,0,2,pBR->ID);
		if (LineIII)	ComShotLine(Nat,0,0,pBR->ID);
	};
	// Opustit shtiki
	if (Fire==false){
		pBR->AttEnm=Stiki;
		N = pGrp->GetTotalAmount();
		while (N--) {
			pOB = pGrp->GetOneObj_lID(N);
			if (pOB!=NULL) {
				if (Stiki)	pOB->NewState=1;
				else		pOB->NewState=0;
			};
		};
	};
	return 1;
};
int SetUnitStateCII_lua(be_CGROUP* pGrp,bool shtiki,bool Fire,bool LI,bool LII,bool LIII){
	if (pGrp==NULL) return 0;
	int Nat = pGrp->GetNation();
	if (Nat<0||Nat>7)	return 0;
	// Set RifleAttack, Brigade not need.
	int N = pGrp->GetTotalAmount();
	OneObject* pOB = NULL;
	bool	FireState = Fire;
	if (Fire&&(LI||LII||LIII))	FireState=false;
	while (N--) {
		pOB = pGrp->GetOneObj_lID(N);
		if (pOB!=NULL) {
			pOB->RifleAttack=FireState;
		};
	};
	// Finde Brigade by line.
	N = pGrp->GetTotalAmount();
	Brigade* pBR = NULL;
	pOB = NULL;
	while (pBR==NULL&&N--) {
		pOB = pGrp->GetOneObj_lID(N);
		if (pOB!=NULL&&pOB->BrigadeID!=0xFFFF) {
			pBR = CITY[Nat].Brigs+pOB->BrigadeID;
		};
	};
	if (pBR==NULL)	return 0;
	if (Fire&&(LI||LII||LIII)){
		void ComShotLine(byte NI,byte State,int Line,int BID);
		if (LI)		ComShotLine(Nat,0,4,pBR->ID);
		if (LII)	ComShotLine(Nat,0,2,pBR->ID);
		if (LIII)	ComShotLine(Nat,0,0,pBR->ID);
	};
	// Opustit shtiki
	if (Fire==false){
		pBR->AttEnm=shtiki;
		N = pGrp->GetTotalAmount();
		while (N--) {
			pOB = pGrp->GetOneObj_lID(N);
			if (pOB!=NULL) {
				if (shtiki)	pOB->NewState=1;
				else		pOB->NewState=0;
			};
		};
	};
	return 1;
};
// lvCSendStikiToZone ////////////////////////////////////////////////////
int				lvCSendStikiToZone::CallFunction(){
	be_CGROUP*	pGrp = (be_CGROUP*)(GrpID.Get());
	if (pGrp==NULL)	return 0;

	int Nat = pGrp->GetNation();
	if (Nat<0||Nat>7)	return 0;

	int rX = -1;
	int rY = -1;
	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL) {
		rX = pNode->vGetX() << 4;
		rY = pNode->vGetY() << 4;
	};
    
	if ( rX==-1 || rY==-1 )	return 0;

	int	N = pGrp->GetTotalAmount();
	Brigade* pBR = NULL;
	OneObject* pOB = NULL;
	while (pBR==NULL&&N--) {
		pOB = pGrp->GetOneObj_lID(N);
		if (pOB!=NULL&&pOB->BrigadeID!=0xFFFF) {
			pBR = CITY[Nat].Brigs+pOB->BrigadeID;
		};
	};
	if (pBR==NULL)	return 0;

	pBR->AttEnm=true;
	N = pGrp->GetTotalAmount();
	while (N--) {
		pOB = pGrp->GetOneObj_lID(N);
		if (pOB!=NULL) {
			pOB->NewState=1;
		};
	};

	pBR->HumanLocalSendTo(rX,rY,dir,128,OrdType);

	return 1;
};

// lvCSetUnitEnableState /////////////////////////////////////////////////
int				lvCSetUnitEnableState::CallFunction(){
	if (Nat<0||Nat>7)	return 0;

	GeneralObject* GO=NATIONS[Nat].Mon[TypeID];
	GO->ManualEnable=0;
	GO->ManualDisable=0;
	if(State>0)GO->ManualEnable=1;
	else if(State<0)GO->ManualDisable=1;

	return 1;
};

// lvCSetUpgradeEnableStatus /////////////////////////////////////////////
int				lvCSetUpgradeEnableStatus::CallFunction(){
	if (Nat<0||Nat>7)	return 0;

	NewUpgrade* NU=NATIONS[Nat].UPGRADE[UpgradeID];
	NU->ManualEnable=0;
	NU->ManualDisable=0;
	if(State>0)NU->ManualEnable=1;
	else if(State<0)NU->ManualDisable=1;

	return 1;
};

void PerformNewUpgrade(Nation* NT,int UIndex,OneObject* OB);
// lvCSetUpgradeDone /////////////////////////////////////////////////////
int				lvCSetUpgradeDone::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
    if(pvGRP!=NULL){
		if ( pvGRP->GetTotalAmount()==0 )	return 0;
		int Nat = pvGRP->GetNation();
		if (Nat<0&&Nat>7)	return 0;
		int lInd=0;
		OneObject* pOB = pvGRP->GetOneObj_lID(lInd);
		if(pOB)
		{
			Nation* NNT=&NATIONS[Nat];
			PerformNewUpgrade(NNT,UpgradeID,pOB);
			NewUpgrade* NU=NNT->UPGRADE[UpgradeID];
			NU->Done=true;
			NU->PermanentEnabled=0;
			NU->Enabled=0;
			return 1;
		};
	};
	return 0;
};
// lvCTeleport ///////////////////////////////////////////////////////////
DWORD			lvCTeleport::GetClassMask(){
	if (Use_VVal==true)	return 0x00020000;
	else				return 0x00010000;
};

int				lvCTeleport::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrpID.Get());
	if (pvGrp!=NULL) {
		int destRX=0;
		int destRY=0;
		if (Use_VVal) {
			vvPOINT2D* pPOS = (vvPOINT2D*)(Point.Get());
			if (pPOS!=NULL) {
				destRX = (pPOS->Value.x)<<4;
				destRY = (pPOS->Value.y)<<4;
			};
		}else{
			be_CNODE* pNode = (be_CNODE*)(parNode.Get());
			if (pNode!=NULL){
				destRX = pNode->vGetX()<<4;
				destRY = pNode->vGetY()<<4;
			};
		};
		int deltaX = 0;
		int deltaY = 0;
		if (destRX!=0||destRY!=0) {
			int cRX,cRY;
			pvGrp->GetGroupCenter(cRX,cRY);
			cRX = cRX<<4;
			cRY = cRY<<4;
			int dlRX = destRX-cRX;
			int dlRY = destRY-cRY;
			if (Direction!=512)	{
				float fi = ((float)Direction/256.f) * (2.f*3.1414f);
				deltaX = (int)(-(float)dX*cosf(fi))<<4;
				deltaY = (int)(-(float)dY*sinf(fi))<<4;
			};
			OneObject* pOB = NULL;
			int N = pvGrp->GetTotalAmount();
			void ChangeUnitCoor(OneObject* OB,int newX,int newY);
			int newRX=0;
			int newRY=0;
			while (N--) {
				pOB = pvGrp->GetOneObj_lID(N);
				if (pOB!=NULL) {
					pOB->ClearOrders();
					newRX = pOB->RealX + dlRX + deltaX;
					newRY =	pOB->RealY + dlRY + deltaY;
					addrand(newRX);
					addrand(newRY);
					ChangeUnitCoor(pOB,newRX,newRY);
					pOB->DeletePath();
					pOB->DestX=-1;
				};
			};
			// change position for Briade (if brigade present)
			LinearArray<int,_int> BrigIDList;
			pvGrp->GetBrigadeList(false,&BrigIDList);
			int NBR=BrigIDList.GetAmount();
			if (NBR>0){
				int Nat=pvGrp->GetNation();
				Brigade* pBR = NULL;
				while (NBR--) {
					pBR = CITY[Nat].Brigs+BrigIDList[NBR];
					// change position for cur Brigade
					pBR->ClearNewBOrders();
					for (int ii=3; ii<pBR->NMemb; ii++){
						if (pBR->Memb[ii]!=0xFFFF){
							pOB = Group[pBR->Memb[ii]];
							if (pOB!=NULL&&(!pOB->Sdoxlo||pOB->Hidden)){
								pBR->posX[ii] = Group[pBR->Memb[ii]]->RealX>>4/*dlRX>>4*/;
								pBR->posY[ii] = Group[pBR->Memb[ii]]->RealY>>4/*dlRY>>4*/;
							};
						};						
					};
				};
			};
			if (Direction!=512){
				pvGrp->SendTo((cRX+dlRX)>>4,(cRY+dlRY)>>4,Direction);
			};
			return 1;
		};
	};
	return 0;
};



// lvCDisband ////////////////////////////////////////////////////////////
int				lvCDisband::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrpID.Get());
	if (pvGrp!=NULL) {
		LinearArray<int,_int> ListBR;
		int NI = pvGrp->GetNation();
		pvGrp->GetBrigadeList(false,&ListBR);
		if (0<=NI && NI<8){
			if (ListBR.GetAmount()>0) {
				Brigade* pBR=&CITY[NI].Brigs[ListBR[0]];
				if (pBR!=NULL) {
					void EraseBrigade(Brigade* BR);
					EraseBrigade(pBR);
					return 1;
				};	
			};
		};
	};
	return 0;
};

// lvCScare //////////////////////////////////////////////////////////////
int				lvCScare::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrpID.Get());
	if (pvGrp!=NULL) {
		LinearArray<int,_int> ListBR;
		int NI = pvGrp->GetNation();
		pvGrp->GetBrigadeList(false,&ListBR);
		if (0<=NI && NI<8){
			if (ListBR.GetAmount()>0) {
				Brigade* pBR=&CITY[NI].Brigs[ListBR[0]];
				if (pBR!=NULL) {
					pBR->Morale=0;
					return 1;
				};	
			};
		};
	};
	return 0;
};

// lvCClearSG ////////////////////////////////////////////////////////////
int				lvCClearSG::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrpID.Get());
	if (pvGrp!=NULL) {
		LinearArray<int,_int> ListBR;
		int N=pvGrp->GetTotalAmount();
		OneObject* pOB=NULL;
		while (N--) {
			pOB=pvGrp->GetOneObj_lID(N);
			if (pOB) pOB->StandTime=0;
		};
		int NI = pvGrp->GetNation();
		pvGrp->GetBrigadeList(false,&ListBR);
		if (0<=NI && NI<8){
			for (int i=0; i<ListBR.GetAmount(); i++){
				Brigade* pBR=&CITY[NI].Brigs[ListBR[i]];
				if (pBR!=NULL) {
					void CancelStandGroundAnyway(Brigade* BR);
					CancelStandGroundAnyway(pBR);
				};	
			};
			return 1;
		};
	};
	return 0;
};
// lvCUnloadSquad ////////////////////////////////////////////////////////
int				lvCUnloadSquad::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vGRP.Get());
	if (pvGrp!=NULL) {
		OneObject* pOB=NULL;
		int N=pvGrp->GetTotalAmount();
		while (N--) {
			pOB=pvGrp->GetOneObj_lID(N);
			if (pOB!=NULL) {
				pOB->MaxDelay=200;
				pOB->delay=200;
			};
			pOB=NULL;
		};
		return 1;
	};
	return 0;
};
// lvCSpotGrpByUType /////////////////////////////////////////////////////
int				lvCSpotGrpByUType::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrpID.Get());
	if (pvGrp==NULL||Nat<0||Nat>=8||UnitType==NULL)	return 0;

    OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit&&!pUnit->Sdoxlo) {
			if (pUnit->NNUM==Nat&&pUnit->NIndex==UnitType) {
				pvGrp->AddUnitGPT(pUnit);
				addname(pUnit->Index);
			};
		};
	};
	return 1;
};

// lvCRemoveNUnitsTo /////////////////////////////////////////////////////
int				lvCRemoveNUnitsTo::CallFunction(){
	be_CGROUP* pvGrpSource	= (be_CGROUP*)(vGrpSource.Get());
	be_CGROUP* pvGrpDest		= (be_CGROUP*)(vGrpDest.Get());
	if (pvGrpSource==NULL)	return 0;
	if (N==0) {
		if (pvGrpDest==NULL) {
			pvGrpSource->RemAllUnits();
		}else{
			pvGrpSource->RemoveNUnitsToCGroup(pvGrpDest);
		};
	}else{
		if (pvGrpDest==NULL) {
			pvGrpSource->RemoveNUnitsToCGroup(NULL,N);
		}else{
			pvGrpSource->RemoveNUnitsToCGroup(pvGrpDest,N);
		};
	}
	return 1;
};
// lvCApplyTerafoming ////////////////////////////////////////////////////
lvCApplyTerafoming::lvCApplyTerafoming(lvCApplyTerafoming* pApplyTerafoming) : lvCOperation(dynamic_cast<lvCOperation*>(pApplyTerafoming)) {
	if (pApplyTerafoming!=NULL) {
		// ... 
	};
};
const	char*	lvCApplyTerafoming::GetThisElementView(const char* LocalName){
	Descr = "ApplyTerafoming(";
	Descr += ")";
	return Descr.pchar();
};
int				lvCApplyTerafoming::Process(int time){
	lvCOperation::Process(time);
	if (LastUpdateTime==0){
		LastUpdateTime=GetTickCount();
		return 1;
	};
	int PosN = TerraPointsArr.GetAmount();
    if (PosN<=0)	return 0;
	lvCTeraforming* pCurPos = GetPossition(0);
	if (PosN==1){
		if (pCurPos!=NULL) {
			pCurPos->Apply();
			TerraPointsArr.Clear();
			return 1;
		};
	}else if(PosN>1) {
		SetSpeed(Get_Speed());
		float	dt = (float)(GetTickCount()-LastUpdateTime);
		float MaxShift = Get_MaxShift(dt,0,1);
		if (Get_MaxDist()<(float)MinFromDest) {
            TerraPointsArr.DelElement(0);
			if (vNOfComplitePoints.Get()!=NULL) {
				vNOfComplitePoints.Get()->Value += 1;
			};
		}else if (MaxShift>=(float)MinStep) {
			lvCTeraforming* pDestPos = GetPossition(1);
			if (pDestPos!=NULL) {
				float maxR = tmax(pCurPos->r,pDestPos->r);
				x0 = (int)(tmin(pCurPos->PosXYZ.x,pDestPos->PosXYZ.x)-maxR-.5f)/64;
				y0 = (int)(tmin(pCurPos->PosXYZ.y,pDestPos->PosXYZ.y)-maxR-.5f)/64;
				x1 = (int)(tmax(pCurPos->PosXYZ.x,pDestPos->PosXYZ.x)+maxR+.5f)/64;
				y1 = (int)(tmax(pCurPos->PosXYZ.y,pDestPos->PosXYZ.y)+maxR+.5f)/64;
			};

			LastUpdateTime=GetTickCount();
			pCurPos->Add_X( xV*dt, MaxShift );
			pCurPos->Add_Y( yV*dt, MaxShift );
			pCurPos->Add_h( hV*dt, MaxShift );
			pCurPos->Add_H( HV*dt, MaxShift );
			pCurPos->Add_r( rV*dt, MaxShift );
			pCurPos->Add_R( RV*dt, MaxShift );
			pCurPos->Apply();

			BClrBar_InR(pCurPos->PosXYZ.x,pCurPos->PosXYZ.y,pCurPos->r);
			for(int i=0;i<NMFIELDS;i++){
				HashTable[i].ReCreateAreas(x0,y0,x1,y1);
			};
			x0=x1=y1=y0=0;
		};
		return 1;
	};

	return 0;
};
void			lvCApplyTerafoming::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCApplyTerafoming(this));
};
void			lvCApplyTerafoming::Draw(){
	if (visible==false)	return;
	int PosN = TerraPointsArr.GetAmount();
	Vector3D	p0(0.f,0.f,0.f),p1(0.f,0.f,0.f);
	lvCTeraforming*	pPos;
	while (PosN--) {
		pPos = TerraPointsArr[PosN]->Get();
		if (pPos!=NULL) {
//			pPos->visible=true;
			if (p0.x==0.f&&p0.y==0.f) {
				p0.set(pPos->PosXYZ.x,pPos->PosXYZ.y,pPos->PosXYZ.z+(float)pPos->R);
			}else{
				p1.set(pPos->PosXYZ.x,pPos->PosXYZ.y,pPos->PosXYZ.z+(float)pPos->R);
				GPS.DrawLine(p0,p1,color);
				p0.set(pPos->PosXYZ.x,pPos->PosXYZ.y,pPos->PosXYZ.z+(float)pPos->R);
			};
			void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
			void WorldToScreenSpace ( Vector4D& vec );

			Vector4D	p(p0.x,p0.y,p0.z,1);
			WorldToScreenSpace(p);
			char s[128];
			sprintf(s,"%s,%s,%d",pPos->GetName()," ID=",PosN);
			ShowStringEx(p.x,p.y,s,&SmallWhiteFont);
		};
	};
};
void			lvCApplyTerafoming::SetSpeed(int _speed){
	bool	recul=false;
	recul = (_speed!=Get_Speed())||(xV==0.f&&yV==0.f&&rV==0.f&&RV==0.f&&hV==0.f&&HV==0.f);
	if (recul) {
		Set_xV();
		Set_yV();
		Set_rV();
		Set_RV();
		Set_hV();
		Set_HV();
	};
};
lvCTeraforming*	lvCApplyTerafoming::GetPossition(int index){
	lvCTeraforming* pRet=NULL;
	if (TestIndexPos(index)){
		pRet = TerraPointsArr[index]->Get();
	};
	return pRet;
};
bool			lvCApplyTerafoming::TestIndexPos(int index){
	return (0<=index&&index<TerraPointsArr.GetAmount()&&TerraPointsArr[index]->Get()!=NULL);
};
float			lvCApplyTerafoming::Get_Speed(){
	float _speed=0;
	if (Use_VV&&vSpeed.Get()!=NULL) {
		_speed=(float)(vSpeed.Get()->Value)/10000.f;
	}else{
		_speed=(float)Speed/10000.f;
	};
	return _speed;
};
float			lvCApplyTerafoming::Get_DT(int p0,int p1){
	float DS = Get_DS(p0,p1);
	if (DS==0.f||Get_Speed()==0.f) return 0.f;
	return DS/Get_Speed();
};
float			lvCApplyTerafoming::Get_DS(int p0,int p1){
	float dist=0.f;
	lvCTeraforming* pPos0 = GetPossition(p0);
	lvCTeraforming* pPos1 = GetPossition(p1);
	if (pPos0!=NULL&&pPos1!=NULL) {
		dist = abs( sqrt( (pPos0->PosXYZ.x-pPos1->PosXYZ.x)*(pPos0->PosXYZ.x-pPos1->PosXYZ.x)+
						  (pPos0->PosXYZ.y-pPos1->PosXYZ.y)*(pPos0->PosXYZ.y-pPos1->PosXYZ.y)	
					    )
				  );
	};
	return dist;
};
float			lvCApplyTerafoming::Get_Dx(int p0,int p1){
	float dist=0.f;
	lvCTeraforming* pPos0 = GetPossition(p0);
	lvCTeraforming* pPos1 = GetPossition(p1);
	if (pPos0!=NULL&&pPos1!=NULL) {
		dist = ( pPos1->PosXYZ.x - pPos0->PosXYZ.x );
	};
	return dist;
};
float			lvCApplyTerafoming::Get_Dy(int p0,int p1){
	float dist=0.f;
	lvCTeraforming* pPos0 = GetPossition(p0);
	lvCTeraforming* pPos1 = GetPossition(p1);
	if (pPos0!=NULL&&pPos1!=NULL) {
		dist = ( pPos1->PosXYZ.y - pPos0->PosXYZ.y );
	};
	return dist;
};
float			lvCApplyTerafoming::Get_Dr(int p0,int p1){
	float dist=0.f;
	lvCTeraforming* pPos0 = GetPossition(p0);
	lvCTeraforming* pPos1 = GetPossition(p1);
	if (pPos0!=NULL&&pPos1!=NULL) {
		dist = ( pPos1->r - pPos0->r );
	};
	return dist;
};
float			lvCApplyTerafoming::Get_DR(int p0,int p1){
	float dist=0.f;
	lvCTeraforming* pPos0 = GetPossition(p0);
	lvCTeraforming* pPos1 = GetPossition(p1);
	if (pPos0!=NULL&&pPos1!=NULL) {
		dist = ( pPos1->R - pPos0->R );
	};
	return dist;
};
float			lvCApplyTerafoming::Get_Dh(int p0,int p1){
	float dist=0.f;
	lvCTeraforming* pPos0 = GetPossition(p0);
	lvCTeraforming* pPos1 = GetPossition(p1);
	if (pPos0!=NULL&&pPos1!=NULL) {
		dist = ( pPos1->h - pPos0->h );
	};
	return dist;
};
float			lvCApplyTerafoming::Get_DH(int p0,int p1){
	float dist=0.f;
	lvCTeraforming* pPos0 = GetPossition(p0);
	lvCTeraforming* pPos1 = GetPossition(p1);
	if (pPos0!=NULL&&pPos1!=NULL) {
		dist = ( pPos1->H - pPos0->H );
	};
	return dist;
};
float			lvCApplyTerafoming::Get_MaxShift(float dt,int p0,int p1){
	float _MaxShift=0.f;
	if (abs(xV*dt)>abs(_MaxShift))	_MaxShift = abs(xV*dt);
	if (abs(yV*dt)>abs(_MaxShift))	_MaxShift = abs(yV*dt);
	if (abs(rV*dt)>abs(_MaxShift))	_MaxShift = abs(rV*dt);
	if (abs(RV*dt)>abs(_MaxShift))	_MaxShift = abs(RV*dt);
	if (abs(hV*dt)>abs(_MaxShift))	_MaxShift = abs(hV*dt);
	if (abs(HV*dt)>abs(_MaxShift))	_MaxShift = abs(HV*dt);
	return _MaxShift;
};
float			lvCApplyTerafoming::Get_MaxDist(int p0,int p1){
	float _Max_Dist=0.f;
	if (abs(Get_Dx(p0,p1))>_Max_Dist)	_Max_Dist=abs(Get_Dx(p0,p1));
	if (abs(Get_Dy(p0,p1))>_Max_Dist)	_Max_Dist=abs(Get_Dy(p0,p1));
	if (abs(Get_Dr(p0,p1))>_Max_Dist)	_Max_Dist=abs(Get_Dr(p0,p1));
	if (abs(Get_DR(p0,p1))>_Max_Dist)	_Max_Dist=abs(Get_DR(p0,p1));
	if (abs(Get_Dh(p0,p1))>_Max_Dist)	_Max_Dist=abs(Get_Dh(p0,p1));
	if (abs(Get_DH(p0,p1))>_Max_Dist)	_Max_Dist=abs(Get_DH(p0,p1));
	return _Max_Dist;
};
void			lvCApplyTerafoming::Set_xV(int p0,int p1){
	float DX = Get_Dx(p0,p1);
	if (DX==0.f||Get_DT(p0,p1)==0.f) {
		xV=0.f;
		return;
	};
	xV = DX/Get_DT(p0,p1);
};
void			lvCApplyTerafoming::Set_yV(int p0,int p1){
	float DY = Get_Dy(p0,p1);
	if (DY==0.f||Get_DT(p0,p1)==0.f) {
		yV=0.f;
		return;
	};
	yV = DY/Get_DT(p0,p1);
};
void			lvCApplyTerafoming::Set_rV(int p0,int p1){
	float Dr = Get_Dr(p0,p1);
	if (Dr==0.f||Get_DT(p0,p1)==0.f) {
		rV=0.f;
		return;
	};
	rV = Dr/Get_DT(p0,p1);
};
void			lvCApplyTerafoming::Set_RV(int p0,int p1){
	float DR = Get_DR(p0,p1);
	if (DR==0.f||Get_DT(p0,p1)==0.f) {
		RV=0.f;
		return;
	};
	RV = DR/Get_DT(p0,p1);
};
void			lvCApplyTerafoming::Set_hV(int p0,int p1){
	float Dh = Get_Dh(p0,p1);
	if (Dh==0.f||Get_DT(p0,p1)==0.f) {
		hV=0.f;
		return;
	};
	hV = Dh/Get_DT(p0,p1);
};
void			lvCApplyTerafoming::Set_HV(int p0,int p1){
	float DH = Get_DH(p0,p1);
	if (DH==0.f||Get_DT(p0,p1)==0.f) {
		HV=0.f;
		return;
	};
	HV = DH/Get_DT(p0,p1);
};
// lvCSetMyNation ////////////////////////////////////////////////////////
DLLEXPORT	void	SetPlayerNation(int NI);
int				lvCSetMyNation::CallFunction(){
	if (0<=Nat&&Nat<8){
		SetPlayerNation(Nat);
		return	1;
	};
	return 0;
};
// lvCEqualizeSpeed //////////////////////////////////////////////////////
int				lvCEqualizeSpeed::CallFunction(){
	be_CGROUP* pvGrp0		= (be_CGROUP*)(vGrp0.Get());
	be_CGROUP* pvGrp1		= (be_CGROUP*)(vGrp1.Get());
	if (pvGrp0==NULL&&pvGrp1==NULL)	return 0;
	if (pvGrp0!=NULL||pvGrp1!=NULL) {
		be_CGROUP* pGrp=( (pvGrp0!=NULL) ? (pvGrp0) : (pvGrp1) );
		int speed=0;
		switch(EqType) {
		case 0:		// Min
			speed=pGrp->GetMinSpeed();
			break;
		case 1:		// Max
			speed=pGrp->GetMaxSpeed();
			break;
		case 2:		// Average
			speed=pGrp->GetAverageSpeed();
			break;
		};
		pGrp->SetSpeed(speed);
		if (pvGrp1)	pvGrp1->SetSpeed(speed);
	};
	return 1;
};
// lvCCreateBrigade //////////////////////////////////////////////////////
int		lvCCreateBrigade::GetSize(){
	int Size = -1;
    if (Use_VV&&vSize.Get()!=NULL) {
		Size=((be_CINT*)(vSize.Get()))->Value;
	}else{
		Size=iSize;
	}
	return Size;
};
DWORD			lvCCreateBrigade::GetClassMask(){
	if (Use_VV==true)	return 0x00020000;
	else				return 0x00010000;
};
int				lvCCreateBrigade::CallFunction(){
	if (GetSize()==-1)	return 0;

	be_CGROUP* pvGrp		= (be_CGROUP*)(vGrp.Get());
	be_CGROUP* pvBrigGrp	= (be_CGROUP*)(vBrigGrp.Get());

	if (pvGrp->GetTotalAmount()<GetSize())	return 0;

	LinearArray<int,_int>	RealCandidate;

	OneObject* pOB=NULL;
	int N=pvGrp->GetTotalAmount();
	word NIIII = 0xFFFF;
	for (int i=0; i<N; i++){
		pOB=pvGrp->GetOneObj_lID(i);
		if ( pOB && pOB->BrigadeID==0xFFFF ){
			if (NIIII==0xFFFF) NIIII=pOB->NIndex;
			if (NIIII==pOB->NIndex) RealCandidate.Add(pOB->Index);
		};
	};
	
	if (RealCandidate.GetAmount()<GetSize())	return 0;

	LinearArray<int,_int>	NewBrigad;

	int NI = pvGrp->GetNation();
	if (0<=NI&&NI<8) {
		UnitsGroup* pUG = new UnitsGroup;
		OneObject* pOB=Group[ RealCandidate[0] ];
		int n=GetSize();
		int N=n;
		while (pOB==NULL&&n--) {
			pOB = Group[ RealCandidate[n] ];
		};
		if (pOB!=NULL) {
			word bNIndex=pOB->NIndex;
			pUG->AddNewUnit(pOB);
			pUG->IDS[0]=0xFFFF;
			for (int i=0; i<GetSize(); i++){
				pOB = Group[ RealCandidate[i] ];
				pUG->AddNewUnit(pOB);
				NewBrigad.Add(pOB->Index);
			};
			//////////////////////////////////////////////////////////////////////////
			int FormID=GetFormationIndexBySizeIndex(NI,bNIndex,0);
			int BID=CITY[NI].GetFreeBrigade();
			if(BID!=-1)
			{
				Brigade* BR=CITY[NI].Brigs+BID;
				BR->Enabled=1;
				BR->Direction=0;
				if(BR->CreateFromGroup(pUG,FormID,0))
				{				
					for(int i=0;i<N;i++){
						OneObject* OB=Group[pUG->IDS[i]];
						if(OB&&OB->Serial==pUG->SNS[i]&&!OB->Sdoxlo&&OB->InArmy){
							pUG->IDS[i]=0xFFFF;
						};
					};
					ext_OnBrigadeCreated(BR);
				};			
			};		
			//////////////////////////////////////////////////////////////////////////
		};
		if (pUG) delete pUG;
		if ( pvBrigGrp==NULL || vGrp==vBrigGrp ){
			// Do nothing
		}else{
			// Copy units to newBrig
			int NNN=NewBrigad.GetAmount();
			while (N--) {
				pvGrp->RemUnitGID(NewBrigad[N]);
				pvBrigGrp->AddUnitGID(NewBrigad[N]);
				addrand(NewBrigad[N]);
			};
		};
		return 1;
	};
	return 0;
};
// lvCAddWallSegment /////////////////////////////////////////////////////
int				lvCAddWallSegment::CallFunction(){
	void AddWallSegment(byte NI,int Type,int x0,int y0,int x1,int y1);
	void AddCheapWallSegment(byte NI,int Type,int x0,int y0,int x1,int y1);
	vvPOINT2D* pBegPos = (vvPOINT2D*)(BegPos.Get());
	vvPOINT2D* pEndPos = (vvPOINT2D*)(EndPos.Get());
	if (Nat<0||Nat>=8||pBegPos==NULL||pEndPos==NULL)	return 0;
//	AddWallSegment(Nat,Type,pBegPos->Value.x,pBegPos->Value.y,pEndPos->Value.x,pEndPos->Value.y);
	AddCheapWallSegment(Nat,Type,pBegPos->Value.x,pBegPos->Value.y,pEndPos->Value.x,pEndPos->Value.y);
	return 1;
};
// lvCSetFarms ///////////////////////////////////////////////////////////
int				lvCUnitLimit::CallFunction(){
	GSets.CGame.UnitLimit =  Farms;
	return 1;
};
// lvCAddFarms ///////////////////////////////////////////////////////////
int				lvCAddFarms::CallFunction(){
	return SetAddFarms_lua(Nat,Farms,SET);
};
// lvCSetSerchWFlag //////////////////////////////////////////////////////
int				lvCSetSerchWFlag::CallFunction(){
	be_CGROUP* pvGrp	= (be_CGROUP*)(vGrp.Get());
	if (pvGrp!=NULL) {
		int N = pvGrp->GetTotalAmount();
		OneObject* pOB=NULL;
		while (N--) {
			pOB=pvGrp->GetOneObj_lID(N);
			if (pOB!=NULL) {
				pOB->NoSearchVictim=!SearchWict;
			};
			pOB=NULL;
		};
		return 1;
	};
	return 0;
};
// lvCClearOrders ////////////////////////////////////////////////////////
int				lvCClearOrders::CallFunction(){
	be_CGROUP* pvGrp	= (be_CGROUP*)(vGrp.Get());
	if (pvGrp!=NULL) {
		pvGrp->ClearOrders();
		return 1;
	};
	return 0;
};
// lvCClearDead ////////////////////////////////////////////////////////
int				lvCClearDead::CallFunction(){
	int N=MAXOBJECT;
	while (N--) {
		OneObject* OB = Group[N];
		if(OB&&OB->Sdoxlo>0&&!OB->NewBuilding){
			OB->Sdoxlo=1000;
		}
	};
	return 1;
};
// lvCGroupMovement //////////////////////////////////////////////////////
int				lvCGroupMovement::CallFunction(){
	be_CGROUP* pvGrp	= (be_CGROUP*)(vGrp.Get());
	if (pvGrp!=NULL) {
		if (MovementState==0) {	// ADD
			GroupsMap()->__AddDonMoveGrp(pvGrp);
		}else if (MovementState==0) {	// DELETE
			GroupsMap()->__RemDonMoveGrp(pvGrp);
		}else{
			return 0;
		};
	};
	return 1;
};
// lvCSetTired ///////////////////////////////////////////////////////////
int				lvCSetTired::CallFunction(){
	if (SetedTiredValue<0)	return 0;
	be_CGROUP* pvGrp	= (be_CGROUP*)(vGrp.Get());
	if (pvGrp==NULL) return 0;
	int N=pvGrp->GetTotalAmount();
	OneObject* pOB=NULL;
	while (N--) {
		pOB = pvGrp->GetOneObj_lID(N);
		if (pOB!=NULL)	pOB->GetTired=SetedTiredValue;
		pOB=NULL;
	};
	return 1;
};

int	SetTired_lua(be_CGROUP* pGRP,int VAL){
	if (VAL<0)	return 0;
	if (pGRP==NULL) return 0;
	int N=pGRP->GetTotalAmount();
	OneObject* pOB=NULL;
	while (N--) {
		pOB = pGRP->GetOneObj_lID(N);
		if (pOB!=NULL)	pOB->GetTired=VAL;
		pOB=NULL;
	};
	return 1;
};
//======================================================================//
//=================	 OPERATION FOR TRANSPORT	 =======================//
//======================================================================//
// lvCPushUnitAway ///////////////////////////////////////////////////////
int				lvCPushNUnitAway::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrpID.Get());
	if (pvGrp!=NULL) {
		pvGrp->PushNUnitAway(N);
		return 1;
	};
	return 0;
};
// lvCSendUnitsToTransport ///////////////////////////////////////////////
int				lvCSendUnitsToTransport::CallFunction(){
	be_CGROUP* pvGrpT = (be_CGROUP*)(vGrpTransID.Get());
	be_CGROUP* pvGrpU = (be_CGROUP*)(vGrpUnitsID.Get());

	if (pvGrpT==NULL||pvGrpU==NULL)	return 0;

	int NT = pvGrpT->GetTotalAmount();
	int NU = pvGrpU->GetTotalAmount();

	if (NT==0||NU==0)	return 0;

	OneObject* pOBT=NULL;
	OneObject* pOBU=NULL;

	OneObject** pOBArrU = (OneObject**)new DWORD[NU];
	for (int i=0; i<NU; i++){
		pOBArrU[i]=pvGrpU->GetOneObj_lID(i);
	};
	int key=0;
	for (int i=1; i<NU; i++){
		if (pOBArrU[i]){
			key=i;
			do{
                if(key<=0)break;
				OneObject* OB1=pOBArrU[key];
				OneObject* OB2=pOBArrU[key-1];
				int N1=OB1?OB1->newMons->NPlacesInTransport:0;
				int N2=OB2?OB2->newMons->NPlacesInTransport:0;
				if(N2<=N1)break;
				OneObject* pTemp = pOBArrU[key];
				pOBArrU[key]=pOBArrU[key-1];
				pOBArrU[key-1]=pTemp;
				key--;
			}while(true);
		};
	};

	int CurTransID=0;
	for (int i=0; i<NU; i++){
		if (pOBArrU[i]!=0) {
			bool stop=false;
			int  iter=0;
			do {
				pOBT=pvGrpT->GetOneObj_lID(CurTransID);
				if (pOBT!=NULL && pOBArrU[i]->GoToMine(pOBT->Index,128+16,0)) {
					stop=true;
				}else{
					CurTransID = (CurTransID+1)%NT;
				};
				iter++;
            } while(!stop&&iter<NT);			
		};
	};

	delete[]pOBArrU;
	pOBArrU=NULL;

	return 1;
};
//////////////////////////////////////////////////////////////////////////

//======================================================================//
//=================		OPERATION FOR FILM		========================//
//======================================================================//
// lvCSetFGV /////////////////////////////////////////////////////////////
int				lvCSetFGV::CallFunction(){
	lvCGraphObject* pObj  = (lvCGraphObject*)(GraphObj.Get());
	vvBASE*			pnObj = (vvBASE*)(newGraphObj.Get());
	if (pObj!=NULL){
		if (Immediately){
			pObj->SetVisible(State);
		}else{
			if (State)	pObj->StartShow();
			else		pObj->StopShow();
		};
	};
	if (pnObj!=NULL) pnObj->visible=State;
	return 1;
};
// lvCPlayOGMiss /////////////////////////////////////////////////////////
int				lvCPlayOGMiss::CallFunction(){
	ov_Play(pFileName.pchar(),pStream);
	return 1;
};
// lvCStopOGMiss /////////////////////////////////////////////////////////
int				lvCStopOGMiss::CallFunction(){
	ov_Stop(pStream);
	return 1;
};
// lvCOGSetVolume ////////////////////////////////////////////////////////
int				lvCOGSetVolume::CallFunction(){
	ov_SetVolume(Volume,pStream);
	return 1;
};
// lvCOGFinishMiss ///////////////////////////////////////////////////////
int				lvCOGFinishMiss::CallFunction(){
	return (ov_StreamFinished(pStream)!=0);
};
// lvCFreezeGame /////////////////////////////////////////////////////////
int				lvCFreezeGame::CallFunction(){
	FreezeGame();
	SetFreezeDipSysProcess(true);
	return 1;
};
// lvCUnFreezeGame ///////////////////////////////////////////////////////
int				lvCUnFreezeGame::CallFunction(){
	UnFreezeGame();
	SetFreezeDipSysProcess(false);
	return 1;
};
// lvCUnFreezeGroup //////////////////////////////////////////////////////
int				lvCUnFreezeGroup::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP) {
		int NU=pvGRP->GetTotalAmount();
		OneObject*	pUnit = NULL;
		for (int i=0; i<NU; i++){
			pUnit = pvGRP->GetOneObj_lID(i);
			if (pUnit) {
				//pUnit->Invisible = false;
				SetInvisiblen(pUnit,false);
				//pUnit->Illusion = false;
				SetIllusion(pUnit,false);
			};
		};
		return 1;
	};
	return 0;
};
// lvCFreezeAndHidden ////////////////////////////////////////////////////
int				lvCFreezeAndHidden::CallFunction(){
	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit) {
			if (pUnit->NewBuilding==false) {
			//	pUnit->Invisible = true;
				SetInvisiblen(pUnit,true);
			}else if (bHome) {
			//	pUnit->Invisible = true;
				SetInvisiblen(pUnit,true);
			}

			//pUnit->Illusion = true;
			SetIllusion(pUnit,true);
		}
	}
	SetFreezeDipSysProcess(true);
	return 1;
};
// lvCUnFreezeAndUnHidden ////////////////////////////////////////////////
int				lvCUnFreezeAndUnHidden::CallFunction(){
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
	SetFreezeDipSysProcess(false);
	return 1;
};
// lvCFreezeAndHiddenGame ////////////////////////////////////////////////
int				lvCFreezeAndHiddenGame::CallFunction(){
	int	zR = -1;
	int x  = -1;
	int y  = -1;
	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL){
		x  = pNode->vGetX();
		y  = pNode->vGetY();
		zR = pNode->vGetR();
	};
	
	if (x!=-1&&y!=-1&&zR!=-1){
		lvSSumb	Ret;	Ret.Sum = 0;	Ret.bHome = bHome;
		PerformActionOverUnitsInRadius(	x,y,zR,AddUnitToSumHide,&Ret);
		return 1;
	};
	
	return 0;
};
// lvCUnFreezeAndUnHiddenGame ////////////////////////////////////////////
int				lvCUnFreezeAndUnHiddenGame::CallFunction(){
	int	zR =  0;
	int x  = -1;
	int	y  = -1;
	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL){
		x  = pNode->vGetX();
		y  = pNode->vGetY();
		zR = pNode->vGetR();
	};
	if ( x!=-1 && y!=-1 && zR!=0 ){
		lvSSum	Ret;	Ret.Sum = 0;
		PerformActionOverUnitsInRadius(	x,y,zR,AddUnitToSumUnHide,&Ret);
		return 1;
	};

	return 0;
};
// lvCUnFreezeAndUnHiddenGroup ///////////////////////////////////////////
int				lvCUnFreezeAndUnHiddenGroup::CallFunction(){
	be_CGROUP* pvGRP = (be_CGROUP*)(GrpID.Get());
	if (pvGRP) {
		int NU=pvGRP->GetTotalAmount();
		OneObject*	pUnit = NULL;
		for (int i=0; i<NU; i++){
			pUnit = pvGRP->GetOneObj_lID(i);
			if (pUnit) {
				//pUnit->Invisible = false;
				SetInvisiblen(pUnit,false);
				//pUnit->Illusion = false;
				SetIllusion(pUnit,false);
			};
		};
		return 1;
	};
	return 0;
};
// lvCSetLeftPort ////////////////////////////////////////////////////////
int				lvCSetLeftPort::CallFunction(){
	lvCDeffFilmMenu* pMenu = (lvCDeffFilmMenu*)(MENU.Get());
	if (pMenu!=NULL) {
		if (FileID!=0xFFFF) {
			pMenu->LEFT_MORDA.FileID = FileID;
			pMenu->SetLeftMorda(SpriteID);
			// Set hero name for menu
			if (pMenu->CII_HeroName_style){
				vvTEXT* pName = (vvTEXT*)(HName.Get());
				if (pName!=NULL&&pName->SpeakerID.pchar()!=NULL) {
					pMenu->LeftName.Set(pName);
				};
			};	
			return 1;
		};
	};
	return 0;
};
// lvCSetRightPort ///////////////////////////////////////////////////////
int				lvCSetRightPort::CallFunction(){
	lvCDeffFilmMenu* pMenu = (lvCDeffFilmMenu*)(MENU.Get());
	if (pMenu!=NULL) {
		if (FileID!=0xFFFF) {
			pMenu->RIGHT_MORDA.FileID = FileID;
			pMenu->SetRightMorda(SpriteID);
			// Set hero name for menu
			if (pMenu->CII_HeroName_style){
				vvTEXT* pName = (vvTEXT*)(HName.Get());
				if (pName!=NULL&&pName->SpeakerID.pchar()!=NULL) {
					pMenu->RightName.Set(pName);
				};
			};	
			return 1;
		};
	};
	return 0;
};
// lvCPlayText ///////////////////////////////////////////////////////////
int				lvCPlayText::CallFunction(){
	if (MENU.Get()!=NULL){
		((lvCDeffFilmMenu*)(MENU.Get()))->ShowText(true);
		((lvCDeffFilmMenu*)(MENU.Get()))->PlayText(canal);
		return 1;
	};
	return 0;
};
// lvCSetText ////////////////////////////////////////////////////////////
int				lvCSetText::CallFunction(){
	lvCDeffFilmMenu*	pMENU = (lvCDeffFilmMenu*)(MENU.Get());
	vvTEXT*				pTEXT = (vvTEXT*)(TEXT.Get());
	if (pMENU!=NULL&&pTEXT!=NULL){
		pMENU->SetText(pTEXT);
		return 1;
	};
	return 0;
};
// lvCSetActivFrame //////////////////////////////////////////////////////
int				lvCSetActivFrame::CallFunction(){
	lvCDeffFilmMenu*	pMENU = (lvCDeffFilmMenu*)(MENU.Get());
	if (pMENU!=NULL){
		pMENU->SetFarmeState(STATE);
		return 1;
	};
	return 0;
};
// lvCRunTimer ///////////////////////////////////////////////////////////
void RunTimer(byte ID, int Long, bool trueTime);
int				lvCRunTimer::CallFunction(){
	be_CINT* pTimer = (be_CINT*)(TimerID.Get());
	if (pTimer!=NULL){
		if (UseVV) {
			be_CINT* vInt=(be_CINT*)(VVpInt.Get());
			if (vInt!=NULL) {
				RunTimer(pTimer->Value,vInt->Value,TrTime);
				return 1;
			};
		}else{
			RunTimer(pTimer->Value,Time,TrTime);
			return 1;
		};
	};
	return 0;
};
DWORD			lvCRunTimer::GetClassMask(){
	if (UseVV)	return	0x00020000;
	else		return	0x00010000;
};
// lvCSetGameMode ////////////////////////////////////////////////////////
int				lvCSetGameMode::CallFunction(){
	// Unselect player units
	ClearSelection(MyNation);
	addrand(370);
	ImClearSelection(MyNation);
	GSets.CGame.ViewMask = ModeST;
	if (ModeST==0) {
		SetCurPtr(15);
	};
	return 1;
};
// lvCSetLMode ///////////////////////////////////////////////////////////
int				lvCSetLMode::CallFunction(){
	void ReverseLMode();
	bool	lModeLoc = 0;
	if (Use_VV){
		be_CINT* pMode = (be_CINT*)(vMode.Get());
		if (pMode!=NULL){
			if (pMode->Value==0||pMode->Value==1){
				lModeLoc = pMode->Value;
			};
		};
	}else{
		if (iMode==1||iMode==0){
			lModeLoc=iMode;
		};
	};
	LMode=lModeLoc;
	ReverseLMode();
	return 1;
};

DWORD			lvCSetLMode::GetClassMask(){
	if (Use_VV)	return	0x00010000;
	else		return	0x00020000;
};
// lvCSetFogMode /////////////////////////////////////////////////////////
int				lvCSetFogMode::CallFunction(){
	extern int FogMode;
	FogMode = Mode;
	return 1;
};
// lvCSetMessageState ////////////////////////////////////////////////////
int				lvCSetMessageState::CallFunction(){
	switch(MessType) {
	case 0:
		if (MESSAGE.Get()!=NULL){
			MESSAGE.Get()->Visible = Visible;
			MESSAGE.Get()->Deleted = Deleted;

			if (EngSettings.MIS_SET.DonotShowComleteQuest&&MESSAGE.Get()->Deleted==true)	
				MESSAGE.Get()->Visible=false;

			//	MESSAGE.Get()->Color   = Color;
			//	MESSAGE.Get()->x	   = x;
			//	MESSAGE.Get()->y	   = y;
			
			// Play some sound
			if (EngSettings.MIS_SET.Play_Task_Sound){
				if ( Visible && !Deleted ) {
					if (EngSettings.MIS_SET.PTS_FileName.pchar()!=NULL&&
						EngSettings.MIS_SET.PTS_FileName.pchar()[0]!=0){
							ov_Play(EngSettings.MIS_SET.PTS_FileName.pchar(),0);
					};
				};
				if ( Visible && Deleted ) {
					if (EngSettings.MIS_SET.PTS_FileName_Give.pchar()!=NULL&&
						EngSettings.MIS_SET.PTS_FileName_Give.pchar()[0]!=0){
							ov_Play(EngSettings.MIS_SET.PTS_FileName_Give.pchar(),0);
					};
				};
			};

			return 1;
		};
		break;
	case 1:
		if (TALK_LST.Get()!=NULL){
			TALK_LST.Get()->Visible=Visible;
			return 1;
		};
		break;
	case 2:
		if (ONE_HINT.Get()!=NULL){
			ONE_HINT.Get()->Visible=Visible;
			ONE_HINT.Get()->HasRead=false;
		};
		return 1;
		break;
	case 3:
		{
			vvMESSGES* pMess = dynamic_cast<vvMESSGES*>(vValuesMap()->GetVValueTypeID(_vvMESSGES_));
			if (pMess!=NULL) {
				pMess->SetMessDS_Visbility(TaskListVisible);
			};
		};
	};

	return 0;
};
DWORD			lvCSetMessageState::GetClassMask(){
	DWORD MessTypeDW = 0x80000000;
	switch(MessType) {
	case 0:
		MessTypeDW = 0x00010000;
		break;
	case 1:
		MessTypeDW = 0x00020000;
		break;
	case 2:
		MessTypeDW = 0x00040000;
		break;
	case 3:
		MessTypeDW = 0x00080000;
		break;
	};

	return MessTypeDW;
};
// lvCSaveScreenPos //////////////////////////////////////////////////////
int				lvCSaveScreenPos::CallFunction(){
	vvVector3D* pPos = (vvVector3D*)(vCameraPos.Get());
	vvVector3D* pDir = (vvVector3D*)(vCameraDir.Get());
	if (pPos!=NULL) pPos->fSetPos.EvaluateFunction();
	if (pDir!=NULL) pDir->fSetDir.EvaluateFunction();
	return 1;
};
// lvCFilmCopliteState ///////////////////////////////////////////////////
int				lvCFilmCopliteState::CallFunction(){
	lvCFilm* pFilm = (lvCFilm*)(Film.Get());
	if (pFilm!=NULL) {
		pFilm->ScriptComplit = Complite;
		return 1;
	};
	return 0;
};
//======================================================================//
//=================  GLOBAL APPLYNING FUNCTION  ========================//
//======================================================================//
// lvCSetGameSpeed ///////////////////////////////////////////////////////
int				lvCSetGameSpeed::CallFunction(){
	if (Use_VV){
		be_CINT* pInt = (be_CINT*)(vSpeed.Get());
		if (pInt!=NULL){
			GSets.SVOpt.RequiredMsPerFrame=pInt->Value;
			return 1;
		};
	}else{
		GSets.SVOpt.RequiredMsPerFrame=iSpeed;
		return 1;
	};	
	return 0;
};
DWORD			lvCSetGameSpeed::GetClassMask(){
	if (Use_VV)	return	0x00010000;
	else		return	0x00020000;
};
// lvCSetGameType ///////////////////////////////////////////////////////
int				lvCSetGameType::CallFunction(){
	if (Use_VV)
	{
		be_CINT* pType = (be_CINT*)(vType.Get());
		if (pType!=NULL)
		{
		//		GSets.RoomParams.RTS_mode = pType->Value;
		};
	}
	else
	{
		//GSets.RoomParams.RTS_mode = iType;
	};
	return 1;
};

DWORD			lvCSetGameType::GetClassMask(){
	if (Use_VV)	return	0x00010000;
	else		return	0x00020000;
};
// lvCGetGameSpeed ///////////////////////////////////////////////////////
int				lvCGetGameSpeed::CallFunction(){
	be_CINT* pInt = (be_CINT*)(vSpeed.Get());
	if (vSpeed.Get()){
		pInt->Value = GSets.SVOpt.RequiredMsPerFrame;
		return 1;
	};
	return 0;
};
// lvCShowPanel //////////////////////////////////////////////////////////
int				lvCShowPanel::CallFunction(){
	vvTEXT* pText = (vvTEXT*)(TextID.Get());
	if (pText!=NULL){
		vmIGP.StartFrame( (char*)(pText->Get()),OpenTime,ShowTime,CloseTime);
		return 1;
	};
	return 0;
};
// lvCShowPanel //////////////////////////////////////////////////////////
int				lvCQuestData::CallFunction(){
	vvMissionLOG* pMLOG = (vvMissionLOG*)(Data.Get());
	if (pMLOG==NULL) return 0;
	switch(OperType) {
		case 0:	// "SetQuestData"
			pMLOG->SetQuestData(QuestN,QuestTotalExperience);
			pMLOG->SetKilsData(KillsTotalExperience);
			pMLOG->SetTimeData(TimeEverage,TimeEverageExperience);
			break;
		case 1: // "AddComplQuest"
			pMLOG->AddCopmleteQuest(QuestComleteExperience);
			break;
		case 2: // "SaveData"
			pMLOG->WriteToLogClass();
			break;
	};
	return 1;
};

DWORD			lvCQuestData::GetClassMask(){
		DWORD	ret = 0x80000000;
		switch(OperType) {
			case 0:	// "SetQuestData"
				ret += 0x00010000;
				break;
			case 1: // "AddComplQuest"
				ret += 0x00020000;
				break;
			case 2: // "SaveData"
				ret += 0x00030000;
				break;
		};

		return ret;	
};
// lvCReStartSquadShema //////////////////////////////////////////////////
lvCReStartSquadShema::lvCReStartSquadShema(lvCReStartSquadShema* pReStartSquadShema) : lvCOperation(dynamic_cast<lvCOperation*>(pReStartSquadShema)) {
	if (pReStartSquadShema!=NULL) {
		vGroup = pReStartSquadShema->vGroup;
		NodeID = pReStartSquadShema->NodeID;
	};
};
const	char*	lvCReStartSquadShema::GetThisElementView(const char* LocalName){
	Descr = "RestartNodeShema(Group[";
	be_CGROUP* pvGroup = GroupsMap()->GetGroupID(vGroup);
	if (pvGroup!=NULL) {
		Descr += pvGroup->Name.pchar();
	}else{
		Descr += "NoGroup";
	}
	Descr += "]";
	if (NodeID!=-1) {
		Descr += ",NewStartNodeID[";
		Descr += NodeID;
		Descr += "]";
	};
	Descr += ")";
	return Descr.pchar();
};
int				lvCReStartSquadShema::Process(int time){
	lvCOperation::Process(time);
	lvCSquardShema* pSS = BattleShema()->vGetSqShemaID(vGroup);
	lvCProcSquad*	pPS = BattleHandler()->vGetSquadScriptID(vGroup);

	if (pSS==NULL||pPS==NULL) return 0;

	if (NodeID!=-1) {
		pPS->NodeID		= NodeID;
	}else{
		pPS->NodeID		= pSS->vGetProbablyFirstNodeID();
	};
	pPS->EdgeID		= 0xFFFF;
	pPS->TimeInProc	= 0;

	// Set first -> true in all edges in shema
	pSS->vRestartEdges();

	return 1;
};
void			lvCReStartSquadShema::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCReStartSquadShema(this));
};
//////////////////////////////////////////////////////////////////////////
int				lvCPAUSE::CallFunction(){
	NOPAUSE = state;
	return 1;
};
//////////////////////////////////////////////////////////////////////////
int				lvCSetSilence::CallFunction(){
	GSets.CGame.SilenceMessageEvents=State;
	return 1;
};
//======================================================================//
//========================  COSSAKS II  ================================//
//======================================================================//
// lvCShowMessageII //////////////////////////////////////////////////////
int				lvCShowMessageII::CallFunction(){
	if (CheckState()==false)	return 0;
	_str	MessgeText;
	PrepareString(MessgeText);
	if (autoHideElse){
//		vValuesMap()->VIEW_OBJS(_vvMISSMGR_,false);
		vValuesMap()->VIEW_OBJS(_vvTASKS_CII_,false);
	};
	ShowMessageII(FileID,SpriteID,MessgeText.pchar());
	if (autoHideElse){
//		vValuesMap()->VIEW_OBJS(_vvMISSMGR_,true);
		vValuesMap()->VIEW_OBJS(_vvTASKS_CII_,true);
	};
	return 1;
};
bool			lvCShowMessageII::CheckState(){
	bool state = true;
	state = state && ( FileID!=0xFFFF );
	state = state && ( SpriteID!=-1 );
	state = state && ( TextID.pchar()!=NULL );
	return state;
};
void			lvCShowMessageII::PrepareString(_str& FullString){
	if (FullString.pchar()!=NULL) FullString.Clear();
	char	data[4096];
	char*	sPart = NULL;
	char	first[4096];
	char	second[4096];
	sprintf(data,"%s",GetTextByID(TextID.pchar()));
	int NP = paramList.GetAmount();
	int cP=0;
	if (NP==0){
		FullString = data;
		return;
	};

	// Have some params for insert
	bool stop=false;
	int iteration=0;
	while (!stop) {
		iteration++;
		sprintf(first,"%s","");
		sprintf(second,"%s","");
		sPart = strstr(data,"%");
		if (sPart!=NULL) {
			int nc = (int)(sPart-data+1);
			strncpy(first,data,nc+2);
			first[nc+2]=0;
			if (data[nc+2]!=0)	sprintf(second,"%s",&(data[nc+2]));
			// set params
			if (cP<NP){
				if		 (strcmp("be_CINT",paramList[cP]->GetParamType())==0){
				//	sprintf(data,first,((be_CBasePrametr*)(paramList[cP]))->Value);
				//	strcpy(first,data);
				}else if (strcmp("be_CSTR",paramList[cP]->GetParamType())==0){
				//	sprintf(data,first,GetTextByID(((vvTEXT*)(paramList[cP]))->TextID.pchar()));
				//	strcpy(first,data);
				}/*else if (strcmp("",paramList[cP]->ParamType.pchar())==0) {
					_str picture;
					if ( ((vvPICTURE*)(paramList[cP]))->GetAsStringForMessage(picture) ){
						sprintf(data,first,picture.pchar());
						strcpy(first,data);
					}else{
						sprintf(data,first,"NO PICTURE");
						strcpy(first,data);
					};
				}*/
				cP++;
			};
			// create new data string
			sprintf(data,"%s%s",first,second);
		};
		if (sPart==NULL||cP>=NP||iteration>=100) {
			stop=true;
		};
	};

	FullString = data;
};
// lvCBrigPanelSet ///////////////////////////////////////////////////////
int			lvCBrigPanelSet::CallFunction(){
	//cvs_BrigPanel BP;
	//ApplyParams(BP);
	//void SetBrigPanel(cvs_BrigPanel& BP);
	//SetBrigPanel(BP);
	return 1;
};
void		lvCBrigPanelSet::ApplyParams(cvs_BrigPanel& BP){
	//BP.Weapon[0]=Bayonet;
	//BP.Weapon[1]=Rifle;
	//BP.Weapon[2]=Grenade;

	//BP.Formation		= Formation;
	//BP.Disband			= Disband;
	//BP.Fill				= Fill;
	//BP.Stop				= Stop;
};
// lvCGroupHoldNode //////////////////////////////////////////////////////
bool g_BrigadeAttackPoint(int NI, word index, int x, int y, const char *rulesFileName);
int				lvCGroupHoldNode::CallFunction(){
	be_CGROUP* pvGroup	= (be_CGROUP*)(vGrp.Get());
	be_CNODE*  pNode		= (be_CNODE*)(parNode.Get());
	if (pvGroup!=NULL&&pNode!=NULL) {
		int xxx,yyy;
		xxx=pNode->vGetX();
		yyy=pNode->vGetY();
		int NI = pvGroup->GetNation();
		LinearArray<int,_int> BR_LIST;
		pvGroup->GetBrigadeList(false,&BR_LIST);
		int N = BR_LIST.GetAmount();
		while (N--) {
			g_BrigadeAttackPoint(NI,BR_LIST[N],xxx,yyy,RulesFile.pchar());
		};
		return 1;
	};
	return 0;
};

int GroupHoldPOS_AI_lua(be_CGROUP* pGRP,int x,int y,const char* FNane){
	if (pGRP!=NULL && FNane!=NULL){
		int NI = pGRP->GetNation();
		LinearArray<int,_int> BR_LIST;
		pGRP->GetBrigadeList(false,&BR_LIST);
		int N = BR_LIST.GetAmount();
		while (N--) {
			g_BrigadeAttackPoint(NI,BR_LIST[N],x,y,FNane);
		};
		return 1;
	};
	return 0;
};
// GetNation_lua /////////////////////////////////////////////////////////
int GetNation_lua(const char* strID){
	DWORD uid = 0xFFFFFFFF;
	sscanf(strID,"%u",&uid);
	if (0<=uid&&uid<MAXOBJECT) {
		OneObject* pOB = Group[uid];
		if ( pOB && (!pOB->Sdoxlo||pOB->Hidden) ) {
			return pOB->NNUM;
		};
	};
	return -1;
};
// ChengeNMaskInNode_lua /////////////////////////////////////////////////
int ChengeNMaskInNode_lua	(int Nat,int NMask,be_CNODE* pND){
	itr_UnitsInRadius.Create(pND->x,pND->y,pND->R);
	OneObject* pOB = NULL;
	while(pOB=itr_UnitsInRadius.Next()){
		if ( pOB->NNUM==Nat && (!pOB->Sdoxlo || pOB->Hidden) ) {
			pOB->NMask = NMask;	
		};
	};
	return 1;	
};
// SetAddFarms ///////////////////////////////////////////////////////////
int SetAddFarms_lua			(int Nat,int Farms,bool set){
	if (0<=Nat&&Nat<8) {
		if (set)	NATIONS[Nat].AddFarms =  Farms;
		else		NATIONS[Nat].AddFarms += Farms;
		return 1;
	};
	return 0;
};
// TeleportGroup_lua /////////////////////////////////////////////////////
int	TeleportGroup_lua(be_CGROUP* pvGrp,be_CNODE* pNode,int Direction,int dX,int dY){
	if (pvGrp!=NULL&&pNode!=NULL) {
		int destRX=pNode->vGetX()<<4;
		int destRY=pNode->vGetY()<<4;
		int deltaX = 0;
		int deltaY = 0;
		if (destRX!=0||destRY!=0) {
			int cRX,cRY;
			pvGrp->GetGroupCenter(cRX,cRY);
			cRX = cRX<<4;
			cRY = cRY<<4;
			int dlRX = destRX-cRX;
			int dlRY = destRY-cRY;
			if (Direction!=512)	{
				float fi = ((float)Direction/256.f) * (2.f*3.1414f);
				deltaX = (int)(-(float)dX*cosf(fi))<<4;
				deltaY = (int)(-(float)dY*sinf(fi))<<4;
			};
			OneObject* pOB = NULL;
			int N = pvGrp->GetTotalAmount();
			void ChangeUnitCoor(OneObject* OB,int newX,int newY);
			int newRX=0;
			int newRY=0;
			while (N--) {
				pOB = pvGrp->GetOneObj_lID(N);
				if (pOB!=NULL) {
					pOB->ClearOrders();
					newRX = pOB->RealX + dlRX + deltaX;
					newRY =	pOB->RealY + dlRY + deltaY;
					addrand(newRX);
					addrand(newRY);
					ChangeUnitCoor(pOB,newRX,newRY);
					pOB->DeletePath();
					pOB->DestX=-1;
				};
			};
			if (Direction!=512&&dX!=0&&dY!=0){
				pvGrp->SendTo((cRX+dlRX)>>4,(cRY+dlRY)>>4,Direction);
			};
			return 1;
		};
	};
	return 0;	
};
// lvCAddElemTHE_CII /////////////////////////////////////////////////////
lvCAddElemTHE_CII::lvCAddElemTHE_CII(){
	InfID=_lvCAddElemTHE_CII_; 
	add_TASK=add_HINT=add_ELSE=false; 
	TASK_Dublicate=HINT_Dublicate=ELSE_Dublicate=false;
};
lvCAddElemTHE_CII::lvCAddElemTHE_CII(lvCAddElemTHE_CII* pAddElemTHE_CII) : lvCOperation(dynamic_cast<lvCOperation*>(pAddElemTHE_CII)) {
	// ...
};
const	char*	lvCAddElemTHE_CII::GetThisElementView(const char* LocalName){
	Descr = "lvCAddElemTHE_CII()";
	return Descr.pchar();
};
int				lvCAddElemTHE_CII::Process(int time){
	lvCOperation::Process(time);

	vvTASKS_CII* pTHE = OBJECT.Get();
	if (pTHE==NULL)	return 0;
	
	pTHE->SETIN_OBJECT();

	if (add_TASK&&TASK.pchar()!=NULL) pTHE->addLT_TASK(TASK.pchar(),TASK_POS,TASK_Dublicate);
	if (add_HINT&&HINT.pchar()!=NULL) pTHE->addLT_HINT(HINT.pchar(),HINT_POS,HINT_Dublicate);
	if (add_ELSE&&ELSE.pchar()!=NULL) pTHE->addLT_ELSE(ELSE.pchar(),ELSE_POS,ELSE_Dublicate);
	
	pTHE->ApplyMM_state();

	if      (add_TASK)		pTHE->vv_TASK_LCLIC();
//	else if (add_HINT)		pTHE->vv_HINT_LCLIC();
//	else if (add_ELSE)		pTHE->vv_ELSE_LCLIC();

	return 0;
};
void			lvCAddElemTHE_CII::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCAddElemTHE_CII(this));
};
// lvCDelElemTHE_CII /////////////////////////////////////////////////////
lvCDelElemTHE_CII::lvCDelElemTHE_CII(lvCDelElemTHE_CII* pDelElemTHE_CII) : lvCOperation(dynamic_cast<lvCOperation*>(pDelElemTHE_CII)) {
	// ...
	// ...
	// ...
};
const	char*	lvCDelElemTHE_CII::GetThisElementView(const char* LocalName){
	Descr = "lvCDelElemTHE_CII()";
	return Descr.pchar();
};
int				lvCDelElemTHE_CII::Process(int time){
	lvCOperation::Process(time);

	vvTASKS_CII* pTHE = OBJECT.Get();
	if (pTHE==NULL)	return 0;

	if (TASK_USE){
		if (FULL_DELETE)	pTHE->delLT_TASK(TASK.pchar());
		else				pTHE->setLT_TASK_COMPLITE(TASK.pchar());
	};
	if (HINT_USE)			pTHE->delLT_HINT(HINT.pchar());
	if (ELSE_USE)			pTHE->delLT_ELSE(ELSE.pchar());
		
	return 0;
};
void			lvCDelElemTHE_CII::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCDelElemTHE_CII(this));
};
// lvCSET_MISS_MANAGER ///////////////////////////////////////////////////
lvCSET_MISS_MANAGER::lvCSET_MISS_MANAGER(lvCSET_MISS_MANAGER* pSET_MISS_MANAGER) : lvCOperation(dynamic_cast<lvCOperation*>(pSET_MISS_MANAGER)){
	if (pSET_MISS_MANAGER!=NULL) {
		MISS_MANAGER	= pSET_MISS_MANAGER->MISS_MANAGER;
		PAUSE			= pSET_MISS_MANAGER->PAUSE;
		PAUSE_Animate	= pSET_MISS_MANAGER->PAUSE_Animate;
		RESTART			= pSET_MISS_MANAGER->RESTART;
		RESTART_Animate	= pSET_MISS_MANAGER->RESTART_Animate;
		NEXT			= pSET_MISS_MANAGER->NEXT;
		NEXT_Animate	= pSET_MISS_MANAGER->NEXT_Animate;
	};
};

/*
const	char*	lvCSET_MISS_MANAGER::GetThisElementView(const char* LocalName){
	Descr = "SET_MISS_MANAGER(MANAGER[";
	vvMISSMGR* pMM = MISS_MANAGER.Get();
	if (pMM)	Descr += pMM->Name.pchar();
	else		Descr += "NOOBJECT";
	Descr += "],PAUSE[";
	Descr += PAUSE;
	if (PAUSE_Animate) Descr += ",Animate";
	Descr += "],RESTART[";
	Descr += RESTART;
	if (RESTART_Animate) Descr += ",Animate";
	Descr += "],NEXT[";
	Descr += NEXT;
	if (NEXT_Animate) Descr += ",Animate";
	Descr += "])";
	return Descr.pchar();
};
*/

int				lvCSET_MISS_MANAGER::Process(int time){
	lvCOperation::Process(time);

	vvMISSMGR* pMM = MISS_MANAGER.Get();
	if (pMM!=NULL) {
		pMM->STPS_ENABLED	( PAUSE				);
		pMM->STPS_ANIM		( PAUSE_Animate		);
		pMM->REST_ENABLED	( RESTART			);
		pMM->REST_ANIM		( RESTART_Animate	);
		pMM->NEXT_ENABLED	( NEXT				);
		pMM->NEXT_ANIM		( NEXT_Animate		);
		return 1;
	};
	return 0;
};
void			lvCSET_MISS_MANAGER::GetCopy(lvCOperation** pCopy){
	*pCopy = dynamic_cast<lvCOperation*>(new lvCSET_MISS_MANAGER(this));
};
// lvCArtChangeCharge ////////////////////////////////////////////////////
int				lvCArtChangeCharge::CallFunction(){
	be_CGROUP* pvGroup	= (be_CGROUP*)(vGrp.Get());
	if (pvGroup!=NULL) {
		OneObject* pOB=NULL;
		int N=pvGroup->GetTotalAmount();
		while (N--) {
			pOB=pvGroup->GetOneObj_lID(N);
			if (pOB!=NULL) SetOrderedStateForComplexObject(pOB,Charge);
			pOB=NULL;
		};
		return 1;
	};
	return 0;
};
// lvCArtAttackPoint /////////////////////////////////////////////////////
int				lvCArtAttackPoint::CallFunction(){
	be_CGROUP* pvGroup	= (be_CGROUP*)(vGrp.Get());
	be_CNODE*  pNode		= (be_CNODE*)(parNode.Get());
	if (pvGroup!=NULL&&pNode!=NULL) {
		OneObject* pOB=NULL;
		int xxx=pNode->vGetX();
		int yyy=pNode->vGetY();
		int N=pvGroup->GetTotalAmount();
		while (N--) {
			pOB=pvGroup->GetOneObj_lID(N);
			if (pOB!=NULL) pOB->NewAttackPoint(xxx,yyy,128+16,0,NTimes);
			pOB=NULL;
		};
		return 1;
	};

	return 0;
};
//========================  STATE FUNC  ================================//
// lvCSUBF_AddAbility ////////////////////////////////////////////////////
int				lvCSUBF_AddAbility::CallFunction(){
	if ( g_beCheckChar(nameSUBF.pchar())==false || g_beCheckChar(nameABIL.pchar())==false ) return 0;
	be_CGROUP* pGROUP = (be_CGROUP*)(refGROUP.Get());
	if ( pGROUP==NULL ) return 0;
	be_CSUBFUNCTION* pSUBF = g_beVAR_STORE.SUBF_LIST.GetByName(nameSUBF.pchar());
	if ( pSUBF==NULL )	return 0;
	AblName* pABIL = NULL;
	int N = pSUBF->AbilityList.GetAmount();
	while (pABIL==NULL&&N--) {
		if ( pSUBF->AbilityList[N]->Name == nameABIL ){
			pABIL = pSUBF->AbilityList[N];
		};
	};
	int addN = 0;
	N = pGROUP->GetTotalAmount();
	ActiveUnitAbility* AddAblNameAbility(OneObject* OB, AblName* AN);
	while (N--) {
        if ( AddAblNameAbility( (*pGROUP)[N], pABIL ) != NULL ) addN++;
	}
	return addN;
};
//**********************************************************************//
//*****************    FUNCTION FOR CONDITION	************************//
//**********************************************************************//
// lvCBaseFunction ///////////////////////////////////////////////////////
lvCBaseFunction::lvCBaseFunction(){ 
	InfID = _lvCBaseFunctions_; 
};
lvCBaseFunction::lvCBaseFunction(lvCBaseFunction* pBaseFunction) : lvCBaseOperCond( dynamic_cast<lvCBaseOperCond*>(pBaseFunction) ) {
	if (pBaseFunction!=NULL) {
		
	};
};

void			lvCBaseFunction::GetCopy(lvCBaseFunction** pCopy){
	*pCopy = new lvCBaseFunction(this);
};

// lvCGetValue ///////////////////////////////////////////////////////////
DWORD			lvCGetValue::GetClassMask(){
	if (isElementInParentClass("InArgList")){
		DWORD	mask = 0x00000002;
		if			(TypeID==0) {	// BOOL
			mask += 0x00000010;
		}else if	(TypeID==1) {	// WORD
			mask += 0x00000020;
		}else if	(TypeID==2) {	// INT
			mask += 0x00000040;
		};
		return mask;
	};
	return 0x00000001;
};
int				lvCGetValue::CallFunction(){
	int RetVal = 0;
	be_CVariableBase* pValue = (be_CVariableBase*)Value.Get();
	if (pValue!=NULL) {
		if (pValue!=NULL) {
			if (strcmp(pValue->GetClassName(),"be_CBOOL")==0){
				be_CBOOL* pTrigg = (be_CBOOL*)(pValue);
				(pTrigg->Value) ? (RetVal=1) : (RetVal=0);
			}else if (strcmp(pValue->GetClassName(),"be_CDWORD")==0){
				be_CDWORD* pWord = (be_CDWORD*)(pValue);
				RetVal = (int)(pWord->Value);
			}else if (strcmp(pValue->GetClassName(),"be_CINT")==0) {
				be_CINT* pInteger = (be_CINT*)(pValue);
				RetVal = pInteger->Value;
			};
		};
	};
	return	RetVal;
};
// lvCBool ///////////////////////////////////////////////////////////////
int				lvCBool::CallFunction(){
	return lvB;
};
// lvCInt ////////////////////////////////////////////////////////////////
int				lvCInt::CallFunction(){
	return lvI;
};
// lvCGetScreenXY ////////////////////////////////////////////////////////
int				lvCGetScreenXY::CallFunction(){
	be_CINT* psX = (be_CINT*)(sX.Get());
	be_CINT* psY = (be_CINT*)(sY.Get());
	if (psX!=NULL&&psY!=NULL) {
		(*psX)	= mapx;
		(*psY)  = mapy;
		return 1;
	};
	return	0;
};
// lvCChkTime ////////////////////////////////////////////////////////////
int				lvCChkTime::CallFunction(){
		return 1;
};
// lvCGetAmount //////////////////////////////////////////////////////////
int				lvCGetAmount::CallFunction(){
	return GetAmount_lua(parNat,Buildings);
};
int	GetAmount_lua(int nat,bool buildings){
	int NU=0;
	OneObject*	pUnit = NULL;
	for (int i=0; i<MAXOBJECT; i++){
		pUnit = Group[i];
		if (pUnit&&!pUnit->Sdoxlo) {
			if (buildings==false&&pUnit->NNUM==nat&&pUnit->NewBuilding==false) {
				NU++;
			}else if (buildings==true&&pUnit->NNUM==nat&&pUnit->NewBuilding==true) {
				NU++;
			};
		}
	}
	return NU;
};
// lvCGetUnitsAmount0 ////////////////////////////////////////////////////
int				lvCGetUnitsAmount0::CallFunction(){
	int xxx=-1;
	int yyy=-1;
	int RRR=-1;

	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL) {
		xxx=pNode->vGetX();
		yyy=pNode->vGetY();
		RRR=pNode->vGetR();
	}else return 0;
	
	return GetUnitsAmount0_lua(parNat,xxx,yyy,RRR);
};
int GetUnitsAmount0_lua(int nat,int x,int y,int R){
	int UN = 0;
	itr_UnitsInRadius.Create(x,y,R);
	while(OneObject* pOB=itr_UnitsInRadius.Next()){
		if (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&pOB->NNUM==nat) UN++;	
	};		
	return UN;	
};
// lvCGetUnitsAmount1 ////////////////////////////////////////////////////
int				lvCGetUnitsAmount1::CallFunction(){
	lvSSumGr	Ret;	Ret.Sum = 0; 
	
	int xxx=-1;
	int yyy=-1;
	int RRR=-1;

	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL) {
		xxx=pNode->vGetX();
		yyy=pNode->vGetY();
		RRR=pNode->vGetR();
	}else return 0;
	
	if (xxx!=-1&&yyy!=-1&&RRR!=-1){
		be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
		if (pvGRP!=NULL) {
			Ret.Sum=pvGRP->GetAmountInZone(xxx,yyy,RRR);
		}else return 0;
	}else return 0;

	return Ret.Sum;
};
// lvCGetUnitsAmountPar ////////////////////////////////////////////////////
int				lvCGetUnitsAmountPar::CallFunction(){
	lvSSumGr	Ret;	Ret.Sum = 0; 
	
	int xxx=-1;
	int yyy=-1;
	int RRR=-1;

	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL) {
		xxx=pNode->vGetX();
		yyy=pNode->vGetY();
		RRR=pNode->vGetR();
	}else return 0;

	if (xxx!=-1&&yyy!=-1&&RRR!=-1)
	{
		be_CARRAY* pArr = g_beVAR_STORE.ARRAY_LIST.GetByName(arrayName.pchar());
		if (pArr == NULL) return 0;
		int Index = ((be_CINT*)(parInd.Get()))->Value;
		if (Index > pArr->GetAmount() || Index < 1) return 0;
		be_CGROUP* pvGRP = (be_CGROUP*)(pArr->GetValue(Index));
		if (pvGRP!=NULL) {
			Ret.Sum=pvGRP->GetAmountInZone(xxx,yyy,RRR);
		}else return 0;
	}else return 0;

	return Ret.Sum;
};
// lvCGetUnitsAmount2 ////////////////////////////////////////////////////
int				lvCGetUnitsAmount2::CallFunction(){
	lvSSumNatType	Ret;	Ret.Sum = 0; 
	if ((0<=parNat&&parNat<8)&&(0<UnitType)){
		Ret.Nat		= parNat;
		Ret.TypeID  = UnitType;
		be_CNODE* pNode = (be_CNODE*)(parNode.Get());
		if (pNode!=NULL) {
			PerformActionOverUnitsInRadius(	pNode->vGetX(),pNode->vGetY(),pNode->vGetR(),AddUnitToSumType,&Ret);
		};
	};
	return Ret.Sum;
};
int GetUnitsAmount2_lua(int nat,int UT,int x,int y,int R){
	lvSSumNatType	Ret;	Ret.Sum = 0; 
	if (0<=nat&&nat<8&&R>0){
		Ret.Nat = nat;
		Ret.TypeID  = UT;
		PerformActionOverUnitsInRadius(	x,y,R,AddUnitToSumType,&Ret );
	};
	return Ret.Sum;
};
// GetEnemyUnitsAmount_lua ///////////////////////////////////////////////
int GetEnemyUnitsAmount_lua	(int mynat,int x,int y,int R){
	int EnN = 0;
	itr_UnitsInRadius.Create(x,y,R);
	while(OneObject* pOB=itr_UnitsInRadius.Next()){
		if (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&(pOB->NMask&NATIONS[mynat].NMask)==0){
			if ( isNotInvisible(pOB,NATIONS[mynat].NMask) )	EnN++;	
		};
	};		
	return EnN;	
};
// GetEnemyBuildindsAmount_lua ///////////////////////////////////////////
int GetEnemyBuildindsAmount_lua	(int mynat,int x,int y,int R){
	int EnN = 0;
	itr_GetBuildingsInRadius.Create(x,y,R);
	while(OneObject* pOB=itr_GetBuildingsInRadius.Next()){
		if (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&(pOB->NMask&NATIONS[mynat].NMask)==0){
			if ( isNotInvisible(pOB,NATIONS[mynat].NMask) )	EnN++;	
		};
	};		
	return EnN;	
};
// lvCGetUnitsAmount3 ////////////////////////////////////////////////////
int				lvCGetUnitsAmount3::CallFunction(){
	lvSSumNat	Ret;	Ret.Sum = 0; 
	if ((0<=parNat&&parNat<8)) {
		Ret.Nat = parNat;
		int vgX;
		int vgY;
		be_CGROUP* pvGRP = (be_CGROUP*)(vGrp.Get());
		if (pvGRP!=NULL) {
			pvGRP->GetGroupCenter(vgX,vgY);
			PerformActionOverUnitsInRadius(	vgX,vgY,parRad,AddUnitToSumN,&Ret );
		};
	};
	return Ret.Sum;
};
// lvCGetTotalAmount0 ////////////////////////////////////////////////////
int				lvCGetTotalAmount0::CallFunction(){
	int NU=0;
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) {
		NU = pvGRP->GetTotalAmount();
	};
	return NU;
};
// lvCGetTotalAmount1 ////////////////////////////////////////////////////
int				lvCGetTotalAmount1::CallFunction(){
	return GetTotalAmount1_lua(parNat,UnitType);
};
int GetTotalAmount1_lua(int nat,int UT){
	int NU=0;
	if (0<=nat&&nat<8) {
		if(UT<NATIONS->NMon){
			NU = NATIONS[nat].CITY->UnitAmount[UT];
		};
	};
	return NU;
};
// lvCGetTotalAmount2 ////////////////////////////////////////////////////
int				lvCGetTotalAmount2::CallFunction(){
	int NU=0;
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) {
		NU=pvGRP->GetTotalAmount2(UnitType);
	};
	return NU;
};

// lvCGetReadyAmount /////////////////////////////////////////////////////
int				lvCGetReadyAmount::CallFunction(){
	return GetReadyAmount_lua(parNat,UnitType);
};	
int GetReadyAmount_lua(int nat,int UT){
	int NU=0;
	if((0<UT)&&(0<=nat&&nat<8)){
		return NATIONS[nat].CITY->ReadyAmount[UT];
	};
	return NU;
};
// lvCGetResource ////////////////////////////////////////////////////////
DWORD			lvCGetResource::GetClassMask(){
	if (isElementInParentClass("InArgList"))	return 0x00000002;
	return 0x00000001;		
};
int				lvCGetResource::CallFunction(){
	if ( GetClassMask()==0x00000002 ){
		be_CINT* pNat = (be_CINT*)refNat.Get();
		if ( pNat ) parNat = pNat->Value;
		else		return 0;
	};
	return GetResource_lua(parNat,parID);
};
int GetResource_lua(int nat,int resid){
	int NU=0;
	NU=GetResource(nat,resid);
	return NU;
};
// GetMoney_lua //////////////////////////////////////////////////////////
int	GetMoney_lua			(int nat){
	return 0;			
};
// lvCGetDiff ////////////////////////////////////////////////////////////
int				lvCGetDiff::CallFunction(){
	return GetDiff_lua(parNI);	
};
int GetDiff_lua(int nat){
	int NU=0;
	NU=GetDiff(nat);	
	return NU;
};
// lvCProbably ///////////////////////////////////////////////////////////
int				lvCProbably::CallFunction(){
	int NU=rando();
	if(parVer>(100*NU/32768)){
		return 1;
	}else{
		return 0;
	};
};
// lvCGetUnitState ///////////////////////////////////////////////////////
int				lvCGetUnitState::CallFunction(){
	int NU=1;
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP) {
		NU=pvGRP->GetAgresiveState();
	};
	return NU;
};

// lvCTrigg //////////////////////////////////////////////////////////////
int				lvCTrigg::CallFunction(){
	return Trigg_lua(TID);
};
int Trigg_lua(int trigid){
	return ( (trigid>511) ? (0) : (~SCENINF.TRIGGER[trigid]) );
};
// lvCGrpInNode //////////////////////////////////////////////////////////
lvCGrpInNode::lvCGrpInNode(lvCGrpInNode* pGrpInNode) : lvCBaseFunction(dynamic_cast<lvCBaseFunction*>(pGrpInNode)) {
	if (pGrpInNode!=NULL) {
		
	};
};

void			lvCGrpInNode::GetCopy(lvCBaseFunction** pCopy){
	*pCopy = dynamic_cast<lvCBaseFunction*>(new lvCGrpInNode(this));
};

const char*		lvCGrpInNode::GetThisElementView(const char* LocalName){
	Descr="";
	Descr += "GroupInNode ()";
	return Descr.pchar();
};
int				lvCGrpInNode::GetValue(int time){
	bool ret;
	ret=lvBE_GroupInNode(squardID,x1,y1,use_vGroup);
	return ret;
};
// lvCGrpInNodeFree //////////////////////////////////////////////////////
lvCGrpInNodeFree::lvCGrpInNodeFree(lvCGrpInNodeFree* pGrpInNodeFree) : lvCBaseFunction(dynamic_cast<lvCBaseFunction*>(pGrpInNodeFree)) {
	if (pGrpInNodeFree!=NULL) {
		
	};
};

void			lvCGrpInNodeFree::GetCopy(lvCBaseFunction** pCopy){
	*pCopy = dynamic_cast<lvCBaseFunction*>(new lvCGrpInNodeFree(this));
};

const char*		lvCGrpInNodeFree::GetThisElementView(const char* LocalName){
	Descr="";
	Descr += "FreeGroupInNode ()";
	return Descr.pchar();
};
int				lvCGrpInNodeFree::GetValue(int time){
	bool ret;
	ret=lvBE_GroupInNodeFree(squardID,x1,y1,use_vGroup);
	return ret;
};
// lvCAllGrpInNode ///////////////////////////////////////////////////////
lvCAllGrpInNode::lvCAllGrpInNode(lvCAllGrpInNode* pAllGrpInNode) : lvCBaseFunction(dynamic_cast<lvCBaseFunction*>(pAllGrpInNode)) {
	if (pAllGrpInNode!=NULL) {
		grpID = pAllGrpInNode->grpID;
	};
};

void			lvCAllGrpInNode::GetCopy(lvCBaseFunction** pCopy){
	*pCopy = dynamic_cast<lvCBaseFunction*>(new lvCAllGrpInNode(this));
};

const char*		lvCAllGrpInNode::GetThisElementView(const char* LocalName){
	Descr="";
	if(0<=grpID){
		Descr += "GroupInNode(";
		if (use_vGroup) {
			be_CGROUP* pvGRP = GroupsMap()->GetGroupID(grpID);
			if (pvGRP) {
				Descr += pvGRP->GetName();
			};
		}else{
			if ((AGroups[grpID]!=NULL)) {
				Descr += AGroups[grpID]->Name.pchar();
			};
		};
		Descr += ")";
	}else{
		Descr = "lvCAllGrpInNode(NULL)";
	};
	return Descr.pchar();
};
int				lvCAllGrpInNode::GetValue(int time){
	bool ret;
	ret=lvBE_GroupInNode(grpID,x1,y1,use_vGroup);
	return ret;
};
// lvCAllGrpInNodeFree ///////////////////////////////////////////////////
lvCAllGrpInNodeFree::lvCAllGrpInNodeFree(lvCAllGrpInNodeFree* pAllGrpInNodeFree) : lvCBaseFunction(dynamic_cast<lvCBaseFunction*>(pAllGrpInNodeFree)) {
	if (pAllGrpInNodeFree!=NULL) {
		grpID = pAllGrpInNodeFree->grpID;
	};
};

void			lvCAllGrpInNodeFree::GetCopy(lvCBaseFunction** pCopy){
	*pCopy = dynamic_cast<lvCBaseFunction*>(new lvCAllGrpInNodeFree(this));
};

const char*		lvCAllGrpInNodeFree::GetThisElementView(const char* LocalName){
	Descr="";
	if(0<=grpID){
		Descr += "FreeGroupInNode(";
		if (use_vGroup) {
			be_CGROUP* pvGRP = GroupsMap()->GetGroupID(grpID);
			if (pvGRP) {
				Descr += pvGRP->GetName();
			};
		}else{
			if ((AGroups[grpID]!=NULL)) {
				Descr += AGroups[grpID]->Name.pchar();
			};
		};
		Descr += ")";
	}else{
		Descr = "lvCAllGrpInNodeFree(NULL)";
	};
	return Descr.pchar();
};
int				lvCAllGrpInNodeFree::GetValue(int time){
	bool ret;
	ret=lvBE_GroupInNodeFree(grpID,x1,y1,use_vGroup);
	return ret;
};
// FG_Visible ////////////////////////////////////////////////////////////
int				FG_Visible::CallFunction(){
	lvCGraphObject* pObj = (lvCGraphObject*)(GraphObj.Get());
	if (pObj){
		if (pObj->isVissible())	return 1;
	};
	return 0;
};

// FG_InVisible //////////////////////////////////////////////////////////
int				FG_InVisible::CallFunction(){
	lvCGraphObject* pObj = (lvCGraphObject*)(GraphObj.Get());
	if (pObj){
		if (pObj->isInVisible())	return	1;
	};
	return 0;
};
// ogSTOP ////////////////////////////////////////////////////////////////
int				ogSTOP::CallFunction(){
	return ogSTOP_lua(canal);
};
int ogSTOP_lua(int canal){
	if (ov_StreamFinished((DWORD)canal)!=0)	return 1;
	return 0;
};
// lvCTimerDone //////////////////////////////////////////////////////////
DLLEXPORT bool TimerDone(byte ID);
int				lvCTimerDone::CallFunction(){
	be_CINT* pTimerID = (be_CINT*)(TimerID.Get());
	if (pTimerID!=NULL){
		if ( TimerDone(pTimerID->Value) ) return 1;
	};
	return 0;
};
// lvCGetElapsedTime //////////////////////////////////////////////////////////
DLLEXPORT int GetTime(byte ID);
int				lvCGetTime::CallFunction(){
	be_CINT* pTimerID = (be_CINT*)(TimerID.Get());
	if (pTimerID!=NULL){
		return (int)(GetTime(pTimerID->Value)/1000);
	};
	return -1;
};
// lvCChekPosition ///////////////////////////////////////////////////////
int				lvCChekPosition::CallFunction(){
	vvPOINT_SET* vPS = (vvPOINT_SET*)(VVpPos.Get());
	if (vPS==NULL) return 0;
	be_CGROUP* pvGRP = (be_CGROUP*)(parGrp.Get());
	if (pvGRP==NULL) return 0;
	return pvGRP->ChekPosition(vPS);
};
// lvCCameraSTOP /////////////////////////////////////////////////////////
int				lvCCameraSTOP::CallFunction(){
	return CameraSTOP_lua();
};
int CameraSTOP_lua(){
	if ((int)(CameraDriver()->MoveType)==0)	return 1;
	return 0;
};
// lvCNationIsErased /////////////////////////////////////////////////////
int				lvCNationIsErased::CallFunction(){
	return NationIsErased(Nat);
};
// lvCGetLMode ///////////////////////////////////////////////////////////
int				lvCGetLMode::CallFunction(){
	return	GetLMode_lua();
};
int GetLMode_lua(){
	if (LMode)	return 1;
	return 0;
};
// lvCCheckButton ////////////////////////////////////////////////////////
int				lvCCheckButton::CallFunction(){
	if (vkID==0) return CheckButton_lua(VK_ESCAPE);
	else		 return CheckButton_lua(vkID);
};
int CheckButton_lua(int vkid){
	if(GetKeyState(vkid)&0x8000){
		return 1;
	};
	return 0;				
};
// lvCIsBrigade //////////////////////////////////////////////////////////
int				lvCIsBrigade::CallFunction(){
	be_CGROUP* pGrp = (be_CGROUP*)(vgGrpID.Get());
	if (pGrp!=NULL) {
		if (pGrp->GetBrigadeList(checkKOM)) {
			return 1;
		};
	};
	return 0;
};

// lvCGetFormationType ///////////////////////////////////////////////////
int				lvCGetFormationType::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vgGrpID.Get());
	return GetFormationType_lua(pvGrp);
};
int GetFormationType_lua(be_CGROUP* pGrp){
	if (pGrp!=NULL) {
		LinearArray<int,_int> ListBR;
		int NI = pGrp->GetNation();
		pGrp->GetBrigadeList(false,&ListBR);
		if (0<=NI && NI<8){
			if (ListBR.GetAmount()>0) {
				Brigade* pBR=&CITY[NI].Brigs[ListBR[0]];
				if (pBR!=NULL) {
					return pBR->GetFormIndex();
				};	
			};
		};
	};
	return -1;
};
//======================================================================//
//=================	 OPERATION FOR TRANSPORT	 =======================//
//======================================================================//
// lvCGetNInsode /////////////////////////////////////////////////////////
int				lvCGetNInside::CallFunction(){
	int retN=0;
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrpID.Get());
	if (pvGrp!=NULL)	retN += pvGrp->GetNInside(Max);
	return retN;
};
// lvCCheckLeaveAbility //////////////////////////////////////////////////
int				lvCCheckLeaveAbility::CallFunction(){
	bool retB=false;
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrpID.Get());
	if (pvGrp!=NULL)	retB = pvGrp->GetLeaveAbility();
	return retB;
};
// lvCLoadingCoplite /////////////////////////////////////////////////////
int				lvCLoadingCoplite::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrpTransport.Get());
	return LoadingCoplite_lua(pvGrp);
};
int LoadingCoplite_lua(be_CGROUP* pvGrp){
	int NMovers=0;
	if (pvGrp!=NULL) {
		OneObject* pOB=NULL;
		int N=pvGrp->GetTotalAmount();
		while (N--) {
			pOB = pvGrp->GetOneObj_lID(N);
			if (pOB!=NULL) {
				NMovers += GetAmountOfMoversTo(pOB);
			};
		};
	};
	return (NMovers==0);
};
// GetTrueTime	//////////////////////////////////////////////////////////
int GetTrueTime(){
	return TrueTime;
};
// lvCGetNofNewUnitInGrp /////////////////////////////////////////////////
int				lvCGetNofNewUnitInGrp::CallFunction(){
	be_CGROUP* pvGrp = (be_CGROUP*)(vGrp.Get());
	if (pvGrp!=NULL) {
		return pvGrp->GetAmountOfNewUnits(ClearNew);
	};
	return 0;
};
// lvCGetNofMyVillage ////////////////////////////////////////////////////
int				lvCGetNofMyVillage::CallFunction(){
	return vdf_GetAmountOfSettlements(Owner,ResType,false,0);
};
// lvCCheckRBBP //////////////////////////////////////////////////////////
int				lvCCheckRBBP::CallFunction(){
	int NU=0;
	vvPOINT2D* pP0 = (vvPOINT2D*)(P0.Get());
	vvPOINT2D* pP1 = (vvPOINT2D*)(P1.Get());
	if (pP0!=NULL&&pP1!=NULL) {
		int x = (pP0->Value.x+pP1->Value.x)/2;
		int y = (pP0->Value.y+pP1->Value.y)/2;
		OneObject*	pUnit = NULL;
		for (int i=0; i<MAXOBJECT&&NU==0; i++){
			pUnit = Group[i];
			if (pUnit&&!pUnit->Sdoxlo&&pUnit->NewBuilding==true) {
				if (Norma(x-(pUnit->RealX>>4),y-(pUnit->RealY>>4))<=MaxR&&pUnit->NStages==pUnit->Stage) {
					NU++;
				};
			};
		};
	};
	return NU;
};
// lvCIsTired ////////////////////////////////////////////////////////////
int				lvCIsTired::CallFunction(){
	be_CGROUP* pGrp = (be_CGROUP*)(vGrp.Get());
	if (pGrp!=NULL&&pGrp->GetIsTired()) return 1;
	return 0;
};
// lvCBrigadesAmount /////////////////////////////////////////////////////
int				lvCBrigadesAmount::CallFunction(){
	return GetBrigadsAmount0(Nat);
};
// lvCTestFillingAbility /////////////////////////////////////////////////
int				lvCTestFillingAbility::CallFunction(){
	be_CGROUP* pGrp = (be_CGROUP*)(vGrp.Get());
	return TestFillingAbility_lua(pGrp);	
};	

int TestFillingAbility_lua(be_CGROUP* pGrp){
	bool	CanFill=false;
	bool TestFillingAbility(OneObject* OB);
	if (pGrp!=NULL) {
		int N=pGrp->GetTotalAmount();
		while (N--&&!CanFill) {
			CanFill = TestFillingAbility( pGrp->GetOneObj_lID(N) );
		};
	};
	return CanFill;
};
// lvCInStandGround //////////////////////////////////////////////////////
int				lvCInStandGround::CallFunction(){
	be_CGROUP* pGrp = (be_CGROUP*)(vGrp.Get());
	return CInStandGround_lua(pGrp,AllBrigades);
};

int CInStandGround_lua(be_CGROUP* pGrp,int AllBrigades){
	if (pGrp!=NULL) {
		int inSG_N	=0;
		int NB		=0;
		LinearArray<int,_int> BrigList;
		pGrp->GetBrigadeList(false,&BrigList);
		NB=BrigList.GetAmount();
		if (NB>0){
			Brigade* pBR=NULL;
			int Nat=pGrp->GetNation();
			for (int i=0; i<NB; i++){
				pBR=&CITY[Nat].Brigs[ BrigList[i] ];
				if (pBR!=NULL&&pBR->InStandGround==true) inSG_N++;
			};
		};
		if (AllBrigades==true&&(inSG_N>0)&&inSG_N==NB) return 1;
		if (AllBrigades==false&&inSG_N>0)			   return 1;
	};
	return 0;
};
// lvCVillageOwner ///////////////////////////////////////////////////////
int				lvCVillageOwner::CallFunction(){
	return VillageOwner_lua(VillageName.pchar());
};

int VillageOwner_lua(const char* vilname){
	_str	ttt; ttt = vilname;
	int owner = -1;
	if (ttt.pchar()!=NULL){
		owner = (int)( GetTribeOwner(ttt.pchar()) );	
	};
	return owner;
};
// lvCGetNofBrigInNode ///////////////////////////////////////////////////
int				lvCGetNofBrigInNode::CallFunction(){
	be_CNODE* pNode = (be_CNODE*)(parNode.Get());
	if (pNode!=NULL) {
		return GetNofBrigInNode_lua(Nat,pNode->vGetX(),pNode->vGetY(),pNode->vGetR());
	};
	return 0;
};

int GetNofBrigInNode_lua(int nat,int x,int y,int R){
	lvSSumSquad	Ret;	
	Ret.Nat = nat;
	PerformActionOverUnitsInRadius(	x,y,R,AddBrIDifPresent,&Ret );
	return Ret.getAmount();
};
// lvCGetCurGrpORDER /////////////////////////////////////////////////////
int				lvCGetCurGrpORDER::CallFunction(){
	be_CGROUP* pGrp = (be_CGROUP*)(vGrp.Get());
	if (pGrp!=NULL) {
		return (int)(pGrp->GetORDER());
	};
	return 0;
};
// lvCGetNofBRLoadedGun //////////////////////////////////////////////////
int				lvCGetNofBRLoadedGun::CallFunction(){
	be_CGROUP* pGrp = (be_CGROUP*)(vGrp.Get());
	if (pGrp!=NULL) {
		return (int)(pGrp->GetNofBRLoadedGun());
	};
	return 0;
};
// lvCSendMSG ////////////////////////////////////////////////////////////
int				lvCSendMSG::CallFunction(){
	g_beRouteMessage( &FSM_MSG );
	return 1;
};
// lvCSetInfo ////////////////////////////////////////////////////////////
int		lvCSetInfo::CallFunction(){
	_str l_title;
	be_gPrepareString(l_title,TitleID.pchar(),titlePL);
	_str l_descr;
	be_gPrepareString(l_descr,DescrID.pchar(),descrPL);
	_str l_task;
	be_gPrepareString(l_task,TaskID.pchar(),taskPL);

	g_beInfoDialog.AddTask( Name, l_title, l_descr, l_task );
	return 1;
};
bool	lvCSetInfo::PrepareString(_str& FullString){
//	return be_gPrepareString(FullString,TextID.pchar(),paramList);
	return true;
};
// lvCDelInfo ////////////////////////////////////////////////////////////
int		lvCDelInfo::CallFunction()
{
	g_beInfoDialog.DelTask( Name );
	return 1;
} // lvCDelInfo::CallFunction
// lvCInfoVisibility /////////////////////////////////////////////////////
int		lvCInfoVisibility::CallFunction(){
	be_CBOOL* pTST = (be_CBOOL*)(state.Get());
	if ( pTST==NULL ) return 0;
	g_beInfoDialog.Visible = (pTST->GetValue());	
};
// lvCShowHint	//////////////////////////////////////////////////////////
int		lvCShowHint::CallFunction(){
	_str Hint;
	if ( !PrepareString(Hint) ) return 0;
	be_CINT* pTime = (be_CINT*)(Time.Get());
	void AssignHint1(char* s,int time);
	if ( pTime!=NULL ){
		AssignHint1(Hint.pchar(),pTime->GetValue());
	}else{
		AssignHint1(Hint.pchar(),150);
	};
	return 1;
};
bool	lvCShowHint::PrepareString(_str& FullString){
	return be_gPrepareString(FullString,TextID.pchar(),paramList);
};
// lvCABIL_SetVisibility /////////////////////////////////////////////////
int				lvCABIL_SetVisibility::CallFunction(){
	if (pAbility){
		pAbility->SetVisible(Visible);
		return 1;
	};
	return 0;
};
// lcCABIL_SetHint ///////////////////////////////////////////////////////
int				lcCABIL_SetHint::CallFunction(){
	be_CSTR* pHint = (be_CSTR*)(HINT.Get());
	if (pAbility&&pHint){
		pAbility->Hint = pHint->GetValue();
		return 1;
	};
	return 0;
};
//////////////////////////////////////////////////////////////////////////
void REG_BE_FUNCTIONS_class(){
	REG_CLASS(COPERCOND_CALLDESCR);
	REG_CLASS(CPRO_MISS_FILER);

//	REG_CLASS(lvTypeNum);
//	REG_CLASS(lvGrpNumBld);
	
	REG_CLASS(lvCBaseOperCond);

	REG_CLASS(lvCBaseFunction);
	REG_CLASS(lvCOperation);

	REG_CLASS(ClassRef<lvCTeraforming>);

	//==========// OPERATION //==========//

	REG_CLASS_EX(lvCGroupSendTo,				"GROUP");			// +++ Node ( --- Node in fantasy )
	REG_CLASS_EX(lvCSendToPosition,				"GROUP");
	REG_CLASS_EX(lvCDisband,					"GROUP");
	REG_CLASS_EX(lvCTeleport,					"GROUP");			// +++ Node
	REG_CLASS_EX(lvCRotateGroup,				"GROUP");			// +++ Node ( --- Node in fantasy )
	REG_CLASS_EX(lvCSetUnitState,				"GROUP");
	REG_CLASS_EX(lvCBrigReformation,			"GROUP");
	REG_CLASS_EX(lvCChangeNationG,				"GROUP");
	REG_CLASS_EX(lvCPutNewSquad,				"GROUP");			// +++ Node
	REG_CLASS_EX(lvCPutNewFormation,			"GROUP");			// +++ Node
	REG_CLASS_EX(lvCPutNewFormationParametric,	"GROUP");
	REG_CLASS_EX(lvCTakeFood,					"GROUP");
	REG_CLASS_EX(lvCTakeWood,					"GROUP");
	REG_CLASS_EX(lvCTakeStone,					"GROUP");
	REG_CLASS_EX(lvCKillNUnits,					"GROUP");
	REG_CLASS_EX(lvCEraseNUnits,				"GROUP");
	REG_CLASS_EX(lvCSpotGrpByUType,				"GROUP");
	REG_CLASS_EX(lvCRemoveNUnitsTo,				"GROUP");
	REG_CLASS_EX(lvCEqualizeSpeed,				"GROUP");
	REG_CLASS_EX(lvCCreateBrigade,				"GROUP");
	REG_CLASS_EX(lvCSavePosition,				"GROUP");
	REG_CLASS_EX(lvCSelectUnits,				"GROUP");
	REG_CLASS_EX(lvCSelectAll,					"GROUP");
	REG_CLASS_EX(lvCSelSendTo,					"GROUP");
	REG_CLASS_EX(lvCSetSerchWFlag,				"GROUP");
	REG_CLASS_EX(lvCClearOrders,				"GROUP");
	REG_CLASS_EX(lvCSetUnitMoveType,			"GROUP");
	REG_CLASS_EX(lvCRepairBuildings,			"GROUP");
	REG_CLASS_EX(lvCIncreaseLevel,				"GROUP");

//	REG_CLASS_EX(lvCGroupMovement,				"GROUP");

	REG_CLASS_EX(lvCSetValue,					"GLOBAL_VALUE");
	REG_CLASS_EX(lvCAddToInt,					"GLOBAL_VALUE");
	REG_CLASS_EX(lvCSetValueEX,					"GLOBAL_VALUE");
//	REG_CLASS_EX(lvCAddToIntEx,					"GLOBAL_VALUE");	Need remove allways

	REG_CLASS_EX(lvCSetFriends,					"NATION");
	REG_CLASS_EX(lvCChangeFriends,				"NATION");
	REG_CLASS_EX(lvCSetRessource,				"NATION");
	REG_CLASS_EX(lvCAddRessource,				"NATION");
	REG_CLASS_EX(lvCStartAIEx,					"NATION");
	REG_CLASS_EX(lvCSetAIEnableState,			"NATION");
	REG_CLASS_EX(lvCSetUnitEnableState,			"NATION");
	REG_CLASS_EX(lvCSetUpgradeEnableStatus,		"NATION");
	REG_CLASS_EX(lvCSetMyNation,				"NATION");
	REG_CLASS_EX(lvCChangeAS,					"NATION");
	REG_CLASS_EX(lvCKillNatinZone,				"NATION");
	REG_CLASS_EX(lvCKillNatNear,				"NATION");
	REG_CLASS_EX(lvCEraseNation,				"NATION");
	REG_CLASS_EX(lvCSetUpgradeDone,				"NATION");
	REG_CLASS_EX(lvCSelectNation,				"NATION");
		
	REG_CLASS_EX(lvCRunTimer,					"SYSTEM");
	REG_CLASS_EX(lvCSetScrollLimit,				"SYSTEM");
	REG_CLASS_EX(lvCSetScrollLimitNation,		"SYSTEM");
	REG_CLASS_EX(lvCShowVictory,				"SYSTEM");
	REG_CLASS_EX(lvCLooseGame,					"SYSTEM");
	REG_CLASS_EX(lvCSetGameSpeed,				"SYSTEM");
	REG_CLASS_EX(lvCGetGameSpeed,				"SYSTEM");
	REG_CLASS_EX(lvCSetLMode,					"SYSTEM");
	REG_CLASS_EX(lvCSetFogMode,					"SYSTEM");
	REG_CLASS_EX(lvCSetGameMode,				"SYSTEM");
	REG_CLASS_EX(lvCPAUSE,						"SYSTEM");
	REG_CLASS_EX(lvCSetSilence,					"SYSTEM");
	REG_CLASS_EX(lvCSetTrigg,					"SYSTEM");
	REG_CLASS_EX(lvCSavePlayersProfile,			"SYSTEM");
	REG_CLASS_EX(lvCSetGameType,				"SYSTEM");
	
//	REG_CLASS_EX(lvCSetFGV,						"GRAPHIC");
//	REG_CLASS_EX(lvCShowPanel,					"GRAPHIC");

	REG_CLASS_EX(lvCPlayOGMiss,					"SOUND");
	REG_CLASS_EX(lvCStopOGMiss,					"SOUND");
	REG_CLASS_EX(lvCOGSetVolume,				"SOUND");
	REG_CLASS_EX(lvCOGFinishMiss,				"SOUND");		// -

//	REG_CLASS_EX(lvCSendToNode,					"NODE_GROUP");	// -
//	REG_CLASS_EX(lvCRotate,						"NODE_GROUP");	// -
//	REG_CLASS_EX(lvCReformation,				"NODE_GROUP");	// -
//	REG_CLASS_EX(lvCSetState,					"NODE_GROUP");	// -
//	REG_CLASS_EX(lvCChangeNation,				"NODE_GROUP");	// -
//	REG_CLASS_EX(lvCSelSendToNode,				"NODE_GROUP");	// -
//	REG_CLASS_EX(lvCGroupSendToNode,			"NODE_GROUP");	// -
    
	REG_CLASS_EX(lvCPushNUnitAway,				"TRANSPORT");
	REG_CLASS_EX(lvCSendUnitsToTransport,		"TRANSPORT");

	REG_CLASS_EX(lvCSetCamera,					"CAMERA");
	REG_CLASS_EX(lvCMoveCamera,					"CAMERA");
	REG_CLASS_EX(lvCScrollCamera,				"CAMERA");
	REG_CLASS_EX(lvCAttachCameraToGroup,		"CAMERA");
	REG_CLASS_EX(lvCFreeCamera,					"CAMERA");
	REG_CLASS_EX(lvCSetStartPoint,				"CAMERA");
	REG_CLASS_EX(lvCSaveScreenPos,				"CAMERA");

//	REG_CLASS_EX(lvCSetLeftPort,				"MOVIE");
//	REG_CLASS_EX(lvCSetRightPort,				"MOVIE");
//	REG_CLASS_EX(lvCPlayText,					"MOVIE");
//	REG_CLASS_EX(lvCSetText,					"MOVIE");
//	REG_CLASS_EX(lvCSetActivFrame,				"MOVIE");
	REG_CLASS_EX(lvCShowDialog,					"MOVIE");
	REG_CLASS_EX(lvCAddTextToDlg,				"MOVIE");
	REG_CLASS_EX(lvCClearDialog,				"MOVIE");
	REG_CLASS_EX(lvCSetMessageState,			"MOVIE");
//	REG_CLASS_EX(lvCFilmCopliteState,			"MOVIE");

//	REG_CLASS_EX(lvCFreezeGame,					"FREEZE");
//	REG_CLASS_EX(lvCUnFreezeGame,				"FREEZE");
//	REG_CLASS_EX(lvCFreezeAndHidden,			"FREEZE");
//	REG_CLASS_EX(lvCUnFreezeAndUnHidden,		"FREEZE");
//	REG_CLASS_EX(lvCFreezeAndHiddenGame,		"FREEZE");
//	REG_CLASS_EX(lvCUnFreezeAndUnHiddenGame,	"FREEZE");
//	REG_CLASS_EX(lvCUnFreezeGroup,				"FREEZE");
//	REG_CLASS_EX(lvCUnFreezeAndUnHiddenGroup,	"FREEZE");
	
	REG_CLASS_EX(lvCSetUnitStateCII,			"COSSAKSII");
	REG_CLASS_EX(lvCSendStikiToZone,			"COSSAKSII");
	REG_CLASS_EX(lvCScare,						"COSSAKSII");
	REG_CLASS_EX(lvCClearSG,					"COSSAKSII");
	REG_CLASS_EX(lvCShowMessageII,				"COSSAKSII");
	REG_CLASS_EX(lvCBrigPanelSet,				"COSSAKSII");
	REG_CLASS_EX(lvCGroupHoldNode,				"COSSAKSII");
	REG_CLASS_EX(lvCUnloadSquad,				"COSSAKSII");
	REG_CLASS_EX(lvCAddElemTHE_CII,				"COSSAKSII");
	REG_CLASS_EX(lvCDelElemTHE_CII,				"COSSAKSII");
	REG_CLASS_EX(lvCSET_MISS_MANAGER,			"COSSAKSII");
	REG_CLASS_EX(lvCSetTired,					"COSSAKSII");
    
//	REG_CLASS_EX(lvCAddWallSegment,				"SPECIAL");
	REG_CLASS_EX(lvCSetLightSpot,				"SPECIAL");
	REG_CLASS_EX(lvCClearLightSpot,				"SPECIAL");
//	REG_CLASS_EX(lvCSpotNUnits,					"SPECIAL");		// +++ Node
//	REG_CLASS_EX(lvCGoInBattle,					"SPECIAL");
	REG_CLASS_EX(lvCArtAttack,					"SPECIAL");
	REG_CLASS_EX(lvCApplyTerafoming,			"SPECIAL");
	REG_CLASS_EX(lvCUnitLimit,					"SPECIAL");
	REG_CLASS_EX(lvCAddFarms,					"SPECIAL");
	REG_CLASS_EX(lvCQuestData,					"SPECIAL");
	REG_CLASS_EX(lvCReStartSquadShema,			"SPECIAL");		// -
	REG_CLASS_EX(lvCArtChangeCharge,			"SPECIAL");
	REG_CLASS_EX(lvCArtAttackPoint,				"SPECIAL");
	REG_CLASS_EX(lvCClearDead,					"SPECIAL");
	REG_CLASS_EX(lvCSendMSG,					"SPECIAL");
	REG_CLASS_EX(lvCSetInfo,					"SPECIAL");
	REG_CLASS_EX(lvCDelInfo,					"SPECIAL");
	REG_CLASS_EX(lvCInfoVisibility,				"SPECIAL");
	REG_CLASS_EX(lvCShowHint,					"SPECIAL");

	REG_CLASS_EX(lvCABIL_SetVisibility,			"ABILITY");
	REG_CLASS_EX(lcCABIL_SetHint,				"ABILITY");
	REG_CLASS_EX(lvCSUBF_AddAbility,			"ABILITY");
		
	//==========// CONDITION //==========//

	REG_CLASS_EX(lvCGetUnitsAmount1,	"F_GROUP");				// +++ Node			// +++ __LAU__
	REG_CLASS_EX(lvCGetUnitsAmount3,	"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCGetTotalAmount0,	"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCGetTotalAmount2,	"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCGetUnitsAmountPar,	"F_GROUP");				// +++ Node			// +++ __LAU__
	REG_CLASS_EX(lvCChekPosition,		"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCAllGrpInNode,		"F_GROUP");									// --- __LAU__	// -
	REG_CLASS_EX(lvCAllGrpInNodeFree,	"F_GROUP");									// --- __LAU__	// -
	REG_CLASS_EX(lvCGetFormationType,	"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCGetUnitState,		"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCIsBrigade,			"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCGetNofNewUnitInGrp,	"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCIsTired,			"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCTestFillingAbility,	"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCInStandGround,		"F_GROUP");									// +++ __LAU__
	REG_CLASS_EX(lvCGetUnitMoveType,	"F_GROUP");

	REG_CLASS_EX(lvCGetValue,			"F_GLOBAL_VALUE");							// +++ __LAU__
	REG_CLASS_EX(lvCBool,				"F_GLOBAL_VALUE");							// +++ __LAU__						
	REG_CLASS_EX(lvCInt,				"F_GLOBAL_VALUE");							// +++ __LAU__
	REG_CLASS_EX(lvCTrigg,				"F_GLOBAL_VALUE");							// +++ __LAU__
		
	REG_CLASS_EX(lvCGetAmount,			"F_NATION");								// +++ __LAU__
	REG_CLASS_EX(lvCGetUnitsAmount0,	"F_NATION");			// +++ Node			// +++ __LAU__
	REG_CLASS_EX(lvCGetUnitsAmount2,	"F_NATION");								// +++ __LAU__
	REG_CLASS_EX(lvCGetTotalAmount1,	"F_NATION");								// +++ __LAU__
	REG_CLASS_EX(lvCGetReadyAmount,		"F_NATION");								// +++ __LAU__
	REG_CLASS_EX(lvCGetResource,		"F_NATION");								// +++ __LAU__
	REG_CLASS_EX(lvCNationIsErased,		"F_NATION");								// +++ __LAU__
	REG_CLASS_EX(lvCGetNofMyVillage,	"F_NATION");								// +++ __LAU__				
	REG_CLASS_EX(lvCBrigadesAmount,		"F_NATION");								// +++ __LAU__
	REG_CLASS_EX(lvCGetNofBrigInNode,	"F_NATION");								// +++ __LAU__

	REG_CLASS_EX(lvCTimerDone,			"F_SYSTEM");								// +++ __LAU__
	REG_CLASS_EX(lvCGetTime,			"F_SYSTEM");
	REG_CLASS_EX(lvCCheckButton,		"F_SYSTEM");								// +++ __LAU__
	REG_CLASS_EX(lvCCameraSTOP,			"F_SYSTEM");								// +++ __LAU__
	REG_CLASS_EX(lvCGetDiff,			"F_SYSTEM");								// +++ __LAU__
	REG_CLASS_EX(lvCGetScreenXY,		"F_SYSTEM");								// --- __LAU__
	REG_CLASS_EX(lvCGetLMode,			"F_SYSTEM");								// +++ __LAU__
	REG_CLASS_EX(lvCProbably,			"F_SYSTEM");								// --- __LAU__

	REG_CLASS_EX(FG_Visible,			"F_GRAPHIC");								// +++ __LAU__
	REG_CLASS_EX(FG_InVisible,			"F_GRAPHIC");								// +++ __LAU__

	REG_CLASS_EX(ogSTOP,				"F_SOUND");									// +++ __LAU__

//	REG_CLASS_EX(lvCGrpInNode,			"F_NODE_GROUP");							// --- __LAU__	// -
//	REG_CLASS_EX(lvCGrpInNodeFree,		"F_NODE_GROUP");							// --- __LAU__	// -
//	REG_CLASS_EX(lvCPrOfSquadInNode,	"F_NODE_GROUP");							// --- __LAU__	// -
//	REG_CLASS_EX(lvCChkTime,			"F_NODE_GROUP");							// --- __LAU__

	REG_CLASS_EX(lvCGetNInside,			"F_TRANSPORT");								// +++ __LAU__
	REG_CLASS_EX(lvCCheckLeaveAbility,	"F_TRANSPORT");								// +++ __LAU__
	REG_CLASS_EX(lvCLoadingCoplite,		"F_TRANSPORT");								// +++ __LAU__

	REG_CLASS_EX(lvCCheckRBBP,			"F_SPECIAL");								// --- __LAU__
	REG_CLASS_EX(lvCVillageOwner,		"F_SPECIAL");								// +++ __LAU__
	REG_CLASS_EX(lvCGetCurGrpORDER,		"F_SPECIAL");								// +++ __LAU__
	REG_CLASS_EX(lvCGetNofBRLoadedGun,	"F_SPECIAL");								// +++ __LAU__
	
};
























