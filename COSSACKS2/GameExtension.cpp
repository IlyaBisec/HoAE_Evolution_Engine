#include "stdheader.h"
#include "WeaponSystem.h"
#include "GameExtension.h"
#include ".\cext_Cheats.h"
#include "BoidsExtension.h"
#include ".\cext_VisualInterface.h"
#include "ai_scripts.h"
#include "WeaponSystemExtension.h"
#include "UnitAbilityExtension.h"
#include "BrigadeAbilityExtension.h"
#include "BattleShipAI.h"
#include "PlaceInAJob.h"
#include "Surface\Surface.h"
#include "Grass.h"
#include "Obj3D.h"
#include "PropertyPanel.h"
#include "BE_HEADERS.h"
#include "UndoMaster.h"


//////////////////////////////////////////////////////////////////////////
typedef DynArray<GameExtension*> ExtScope;
typedef DynArray<GameExtension*> ExtRef;
//
ExtRef ExtReferences[MAXVFUNC];
ExtScope* ExtList=NULL;
static bool RefInit[MAXVFUNC];
//
#define ITERATE_REF(idx,expr,argslist,args)\
void ext_##expr##argslist##{\
	ExtRef* ER=&ExtReferences[idx];\
	int N=ER->GetAmount();\
	for(int i=0;i<N;i++){\
		(*ER)[i]->expr##args;\
	}\
	if(!RefInit[idx]){\
		for(int i=N-1;i>=0;i--){\
			if(!(*ER)[i]->Check(idx)){\
				ER->Del(i,1);\
			}\
		}\
		RefInit[idx]=1;\
	}\
}
#define ITERATE_BOOL_REF(idx,expr,argslist,args)\
bool ext_##expr##argslist##{\
	ExtRef* ER=&ExtReferences[idx];\
	int N=ER->GetAmount(); bool R=0; bool r=RefInit[idx]; if(!r)RefInit[idx]=1;\
	for(int i=0;i<N;i++)if(i<ER->GetAmount()){\
		R|=(*ER)[i]->expr##args;\
	}\
	if(!RefInit[idx]){\
		for(int i=N-1;i>=0;i--){\
			if(!(*ER)[i]->Check(idx)){\
				ER->Del(i,1);\
			}}}\
	return R;\
}
#define ITERATE_TBOOL_REF(idx,expr,argslist,args)\
bool ext_##expr##argslist##{\
	ExtRef* ER=&ExtReferences[idx];\
	bool r=RefInit[idx];if(!r)RefInit[idx]=1;int N=ER->GetAmount();\
	bool R=1;\
	for(int i=0;i<N;i++){\
		R&=(*ER)[i]->expr##args;\
	}\
	if(!r){\
		for(int i=N-1;i>=0;i--){\
			if(!(*ER)[i]->Check(idx)){\
				ER->Del(i,1);\
			}\
		}\
	}\
	return R;\
	}
#define ITERATE_TYPE_REF(idx,type,expr,argslist,args)\
type ext_##expr##argslist##{\
	ExtRef* ER=&ExtReferences[idx];\
	bool r=RefInit[idx];\
	if(!r)RefInit[idx]=1;\
	int N=ER->GetAmount();\
	for(int i=0;i<N;i++){\
		(*ER)[i]->expr##args;\
	}\
	if(!RefInit[idx]){\
		for(int i=N-1;i>=0;i--){\
			if(!(*ER)[i]->Check(idx)){\
				ER->Del(i,1);\
			}\
		}\
	}\
	return 0;\
}

#define ITERATE_NOTHING(idx,type,expr,argslist,args)

#define GEN_EXT_CODE
#include "GameExtensionInternal.h"
DWORD GameExtension::GetCode(){
	DWORD p=0;
	int L=strlen(ExtensionName.pchar());
	for(int i=0;i<L;i++)p+=(i+20)*DWORD(ExtensionName.pchar()[i]);
	return p;
}
void ext_OnGameSaving(xmlQuote& xml){
    xml.ClearAll();
	ExtRef* ER=&ExtReferences[14];
	int N=ER->GetAmount();
	xmlQuote* xTemp=new_xmlQuota();	
	for(int i=0;i<N;i++){		
		if((*ER)[i]->OnGameSaving(*xTemp)){
			DWORD V=(*ER)[i]->GetCode();
			char c[32];
			sprintf(c,"%X",V);
            xTemp->SetQuoteName(c);
			xml.AddSubQuote(xTemp);
			xTemp=new_xmlQuota();
		}
	}
	delete_xmlQuote(xTemp);
	if(!RefInit[14]){
		for(int i=N-1;i>=0;i--){
			if(!(*ER)[i]->Check(14)){
				ER->Del(i,1);
			}
		}
		RefInit[14]=1;
	}	
}
void ext_OnGameLoading(xmlQuote& xml){
	///xml.ClearAll();
	ExtRef* ER=&ExtReferences[15];
	int N=ER->GetAmount();
	int NS=xml.GetNSubQuotes();	
	for(int i=0;i<N;i++){	
		DWORD Code=(*ER)[i]->GetCode();
		char cc[16];
		sprintf(cc,"%X",Code);
		for(int j=0;j<NS;j++){
			xmlQuote* xq=xml.GetSubQuote(j);
			const char* s=xq->GetQuoteName();
			if(s&&!strcmp(s,cc)){
                bool r=(*ER)[i]->OnGameLoading(*xq);
			}
		}
	}
	if(!RefInit[15]){
		for(int i=N-1;i>=0;i--){
			if(!(*ER)[i]->Check(15)){
				ER->Del(i,1);
			}
		}
		RefInit[15]=1;
	}
}
void ext_OnMapSaving(xmlQuote& xml){
	xml.ClearAll();
	ExtRef* ER=&ExtReferences[12];
	int N=ER->GetAmount();
	xmlQuote* xTemp=new_xmlQuota();	
	for(int i=0;i<N;i++){		
		if((*ER)[i]->OnMapSaving(*xTemp)){
			DWORD V=(*ER)[i]->GetCode();
			char c[32];
			sprintf(c,"%X",V);
			xTemp->SetQuoteName(c);
			xml.AddSubQuote(xTemp);
			xTemp=new_xmlQuota();
		}
	}
	delete_xmlQuote(xTemp);
	if(!RefInit[12]){
		for(int i=N-1;i>=0;i--){
			if(!(*ER)[i]->Check(14)){
				ER->Del(i,1);
			}
		}
		RefInit[12]=1;
	}	
}
void ext_OnMapLoading(xmlQuote& xml){	
	ExtRef* ER=&ExtReferences[13];
	int N=ER->GetAmount();
	int NS=xml.GetNSubQuotes();	
	for(int i=0;i<N;i++){	
		DWORD Code=(*ER)[i]->GetCode();
		char cc[16];
		sprintf(cc,"%X",Code);
		for(int j=0;j<NS;j++){
			xmlQuote* xq=xml.GetSubQuote(j);
			const char* s=xq->GetQuoteName();
			if(s&&!strcmp(s,cc)){
				bool r=(*ER)[i]->OnMapLoading(*xq);
			}
		}
	}
	if(!RefInit[13]){
		for(int i=N-1;i>=0;i--){
			if(!(*ER)[i]->Check(13)){
				ER->Del(i,1);
			}
		}
		RefInit[13]=1;
	}
}
void ext_OnSaveBinaryDataFromMap(BinStream* bs){	
	ExtRef* ER=&ExtReferences[205];
	int N=ER->GetAmount();	
	for(int i=0;i<N;i++){	
		MemoryBinStream M;
        (*ER)[i]->OnSaveBinaryDataIntoMap(&M);
		if(M.Size()){
			DWORD Code=(*ER)[i]->GetCode();
			bs->WriteDWORD(Code);
			bs->WriteDWORD(M.Size());
			bs->Write(M.GetData(),M.Size());
		}
	}
	if(!RefInit[205]){
		for(int i=N-1;i>=0;i--){
			if(!(*ER)[i]->Check(205)){
				ER->Del(i,1);
			}
		}
		RefInit[205]=1;
	}
}
void ext_OnLoadBinaryDataFromMap(BinStream* bs){	
	if(bs->Size()){
		ExtRef* ER=&ExtReferences[206];
		int N=ER->GetAmount();
		do{
            DWORD C=bs->ReadDWORD();
			for(int i=0;i<N;i++)if( C==(*ER)[i]->GetCode() ){
                DWORD sz=bs->ReadDWORD();
				MemoryBinStream M;
				M.Write(bs->GetData()+bs->GetReadPos(),sz);
				(*ER)[i]->OnSaveBinaryDataIntoMap(&M);
			}
		}while(bs->GetReadPos()<bs->Size());
		if(!RefInit[206]){
			for(int i=N-1;i>=0;i--){
				if(!(*ER)[i]->Check(206)){
					ER->Del(i,1);
				}
			}
			RefInit[206]=1;
		}
	}
}
void ext_OnSaveGameBinaryData(BinStream* bs){	
	ExtRef* ER=&ExtReferences[207];
	int N=ER->GetAmount();	
	for(int i=0;i<N;i++){	
		MemoryBinStream M;
		(*ER)[i]->OnSaveGameBinaryData(&M);
		if(M.Size()){
			DWORD Code=(*ER)[i]->GetCode();
			bs->WriteDWORD(Code);
			bs->WriteDWORD(M.Size());
			bs->Write(M.GetData(),M.Size());
		}
	}
	if(!RefInit[207]){
		for(int i=N-1;i>=0;i--){
			if(!(*ER)[i]->Check(207)){
				ER->Del(i,1);
			}
		}
		RefInit[207]=1;
	}
}
void ext_OnLoadGameBinaryData(BinStream* bs){	
	if(bs->Size()){
		ExtRef* ER=&ExtReferences[208];
		int N=ER->GetAmount();
		do{
			DWORD C=bs->ReadDWORD();
			for(int i=0;i<N;i++)if( C==(*ER)[i]->GetCode() ){
				DWORD sz=bs->ReadDWORD();
				MemoryBinStream M;
				M.Write(bs->GetData()+bs->GetReadPos(),sz);
				(*ER)[i]->OnLoadGameBinaryData(&M);
			}
		}while(bs->GetReadPos()<bs->Size());
		if(!RefInit[208]){
			for(int i=N-1;i>=0;i--){
				if(!(*ER)[i]->Check(208)){
					ER->Del(i,1);
				}
			}
			RefInit[208]=1;
		}
	}
}

void InitExtensions(){
    if(!ExtList)ExtList=new ExtScope;
    memset(RefInit,0,sizeof RefInit);
}
void InstallExtension(GameExtension* Ext,const char* Name){
	InitExtensions();
	Ext->ExtensionName=Name;
	DWORD Code=Ext->GetCode();
	for(int i=0;i<ExtList->GetAmount();i++)if((*ExtList)[i]->GetCode()==Code){
		char cc[128];
		sprintf(cc,"WARNING! Extension <%s> installed twice! You should specify another name!",Name);
		MessageBox(NULL,cc,"Extension install error",0);
		assert(1);
	}
    ExtList->Add(Ext);
	for(int i=0;i<MAXVFUNC;i++){
        ExtReferences[i].Add(Ext);
	}
}
void UnInstallExtension(GameExtension* GI){
    GI->OnClassUnRegister();
	for(int i=0;i<ExtList->GetAmount();i++){
		if((*ExtList)[i]==GI){
			ExtList->Del(i,1);
			break;
		}
	}
	for(int i=0;i<MAXVFUNC;i++){
		for(int j=0;j<ExtReferences[i].GetAmount();j++){
			if(ExtReferences[i][j]==GI){
				ExtReferences[i].Del(j,1);
				break;
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////
//////////////INSERT THERE YOUR EXTENSION INSTALLATION/////////////////
///////////////////////////////////////////////////////////////////////
//TEST SAMPLE
bool GecOptimCollision;
bool GecOptimCityEnum;
bool GecOptimCDirSound;
bool GecOptimRasterizator;
bool GecOptimDialogs;
bool GecOptimBattleEditor;
bool GecOptimImmortal;
bool GecOptimMultycast;
    
class TestExtension:public GameExtension{
public:
	bool Tormoz;
	TestExtension(){
		Tormoz=false;
		GecOptimCollision=false;
		GecOptimCityEnum=false;
		GecOptimCDirSound=false;
		GecOptimRasterizator=false;
		GecOptimDialogs=false;
		GecOptimBattleEditor=false;
		GecOptimImmortal=false;
		GecOptimMultycast=false;
	}
	virtual void ProcessingGame(){
		if(Tormoz){
			Sleep(200);
		}                
	}
	bool OnUnitDie(OneObject* Dead,OneObject* Killer){
        //UMS units
		if(Killer&&Dead&&Killer->NNUM!=Dead->NNUM){
			for(int i=0;i<6;i++){
				AddXRESRC(Killer->NNUM,i,Dead->newMons->UMS_Res[i]);
			}
		}
		return true;
	}
	virtual bool OnCheatEntering(const char* Cheat){
		if(!strcmp(Cheat,"fdl")){
			//void testFileDialog();
			//testFileDialog();
			//AssignHint1("mi tut vezde!!",200,0);
			return true;
		}
		if(!strncmp(Cheat,"genm ",5)){
			bool GenerateRandomMap(char* MapName);
			GenerateRandomMap((char*)(Cheat)+5);
		}
		if(!strcmp(Cheat,"normf")){
			void NormalizeFormations();
			NormalizeFormations();
		}
        if(strstr(Cheat,"limpri")){
            extern int LimitedDipsCount;
            LimitedDipsCount=atoi(strstr(Cheat,"limpri")+6);
        }        
        if(!strcmp(Cheat,"rottrees")){
            for(int i=0;i<MaxSprt;i++){
                OneSprite* OS=Sprites+i;
                if(OS->Enabled && OS->M4){
                    Matrix4D mm;
                    mm.rotation(Vector3D::oZ,float(rand())/1024.0f);
                    OS->M4->mulLeft(mm);
                }
            }
        }
		if(!strcmp(Cheat,"noflag")){
			void NormalizeFlagbearers();
			NormalizeFlagbearers();
		}
		if(!strcmp(Cheat,"gecoptimcollision")){
			GecOptimCollision=!GecOptimCollision;
		}
		if(!strcmp(Cheat,"gecoptimcityenum")){
			GecOptimCityEnum=!GecOptimCityEnum;
		}
		if(!strcmp(Cheat,"gecoptimcdirsound")){
			GecOptimCDirSound=!GecOptimCDirSound;
		}
		if(!strcmp(Cheat,"gecoptimrasterizator")){
			GecOptimRasterizator=!GecOptimRasterizator;
		}
		if(!strcmp(Cheat,"gecoptimdialogs")){
			GecOptimDialogs=!GecOptimDialogs;
		}
		if(!strcmp(Cheat,"gecoptimbattleeditor")){
			GecOptimBattleEditor=!GecOptimBattleEditor;
		}
		if(!strcmp(Cheat,"gecoptimimmortal")){
			GecOptimImmortal=!GecOptimImmortal;
		}
		if(!strcmp(Cheat,"gecoptimmultycast")){
			GecOptimMultycast=!GecOptimMultycast;
		}
		if(!strcmp(Cheat,"tormozz")){
			Tormoz=!Tormoz;
		}
		if(!strcmp(Cheat,"lzons")){
			void LoadZonesOnlyDialog();
			LoadZonesOnlyDialog();
		}
		if(!strcmp(Cheat,"delg17")){
			void DeleteRedundantG17();
			DeleteRedundantG17();
		}
		if(!strcmp(Cheat,"qwerty")){
			void TestConn();
			TestConn();
		}		
		if(!strcmp(Cheat,"printtex")){
			void PrintTex();
			PrintTex();
		}
		if(!strcmp(Cheat,"bldplane")){
			for(int i=0;i<MAXOBJECT;i++){
				OneObject* OB=Group[i];
				if(OB&&OB->NewBuilding){
					void CreatePlaneUnderBuilding(int xc,int yc,NewMonster* NM);
					CreatePlaneUnderBuilding(OB->RealX>>4,OB->RealY>>4,OB->newMons);
				}
			}			
		}
		if(!strcmp(Cheat,"nosave")){
			extern int ME_prevtime;
			ME_prevtime=GetTickCount()+1000*60*60;
		}
		if(!strcmp(Cheat,"abones")){
			extern bool ShowActiveBones;
			ShowActiveBones=!ShowActiveBones;
		}
		if(!strcmp(Cheat,"dosave")){
			extern int ME_prevtime;
			ME_prevtime=GetTickCount();
		}
		if(!strcmp(Cheat,"testg17")){
			void TestAllG17();
			TestAllG17();
		}
        if(!strcmp(Cheat,"runscript")){
            DriveMode()->PROCESS = true;
            AssignHint1("Script is run now!",255);
        }       
        if(!strcmp(Cheat,"stopscript")){
            DriveMode()->PROCESS = false;
            AssignHint1("Script is paused now!",255);
        }		
		if(!strcmp(Cheat,"fastbool")){
			extern bool FastBool;
			FastBool=true;
		}
		if(!strcmp(Cheat,"CYLINDER")){
			extern bool CYLINDER_test_mode;
			CYLINDER_test_mode=!CYLINDER_test_mode;
		}
        if(!strcmp(Cheat,"leaks")){
            ProcessEditor("PrintLeaks");
        }
        if(!strcmp(Cheat,"bbones")){
			extern bool _dbgDrawBonesMode;
			_dbgDrawBonesMode = !_dbgDrawBonesMode;
		}
		if(!strcmp(Cheat,"undo")){
			UndoMaster::Undo();
		}
        if(!strcmp(Cheat,"shotpts")){
            extern bool _dbgDrawShotPts;
            _dbgDrawShotPts = !_dbgDrawShotPts;
        }
		if(!strcmp(Cheat,"drawcol")){
			extern bool DrawCollisionMode;
			DrawCollisionMode=!DrawCollisionMode;			
		}	
		if(!strcmp(Cheat,"qwer")){
			void ReloadSelectedUnits();
			ReloadSelectedUnits();
		}	
		if(!strcmp(Cheat,"terrlim")){
			extern int MaxTerrTris;
			if(MaxTerrTris<10000){
				MaxTerrTris=10000*3;
			}else{            
				MaxTerrTris=3;
			}
		}
		if(strstr(Cheat,"killholes")){
			int n=atoi(Cheat+9);
			if(n==0)n=1024;
			void CloseHoles(MotionField* MF,int MinimalSquare,bool PrintReport);
			CloseHoles(&MFIELDS[0],n,true);
			CloseHoles(&MFIELDS[0],n,true);
		}
		if(!strcmp(Cheat,"convmdls")){
			void ConvertModels( const char* root );
			ConvertModels("Models\\");
		}
		if(!strcmp(Cheat,"drawexit")){
			extern bool DrawExitPathMode;
			DrawExitPathMode=!DrawExitPathMode;			
		}		
		if(!strcmp(Cheat,"debugweap")){
			extern bool DebugWeapMode;
			DebugWeapMode=true;			
		}
		if(!strcmp(Cheat, "hideinf")) {
			if(DriveMode()->BE_vGRP_VISIBLE) {
				OnCheatEntering("noinf");
				return true;
			}
		}
        if(!strcmp(Cheat,"recltrees")){
            void RecalcTrees();
            RecalcTrees();
        }        
		if(!strcmp(Cheat, "showinf")) {
			if(!DriveMode()->BE_vGRP_VISIBLE) {
				return OnCheatEntering("noinf");
			}
		}
		if(!strcmp(Cheat,"noinf")){
            DriveMode()->NodeView=0; 
			DriveMode()->NodeAct=4;
			DriveMode()->EdgeView=0;
			DriveMode()->fSHOW_ALL.EvaluateFunction();
		}
        if(!strcmp(Cheat,"KillSpeedVariation")){
            extern int KillSpeedVariation;
            KillSpeedVariation=256;
        }
		if(!strcmp(Cheat,"remroad")){
			void RemoveRoadsFromLockPoints();
			RemoveRoadsFromLockPoints();
		}
		if(strstr(Cheat,"frwater")){
			char cc1[32];
			int h1=-30;
			int h2=-80;
			int sc=32;
			sscanf(Cheat,"%s%d%d%d",cc1,&h1,&h2,&sc);
			for(int ix=0;ix<VertInLine;ix++){
				for(int iy=0;iy<MaxTH;iy++){
                    int xx=ix<<5;
					int yy=(iy<<5)-((ix&1)*16);
					int H=THMap[ix+iy*VertInLine];
					int GetFractalVal(int x,int y);
					int Hc=h1+((h2-h1)*GetFractalVal(xx*sc/32,yy*sc/32)/512);
					if(H<Hc)THMap[ix+iy*VertInLine]=Hc;
				}
			}
			void MakeAllDirtyGBUF();
			MakeAllDirtyGBUF();
			void ResetWaterHash();
			ResetWaterHash();
			void ResetGroundCache();
			ResetGroundCache();		
			void CreateCostPlaces();
			CreateCostPlaces();
			void CreateMiniMap();
		}
		if(!strcmp(Cheat,"bbridge")){
			bool UseGroupLikeBridgeBuilders(UnitsGroup* ugrp,int BridgeType,byte NI);
			UnitsGroup* UG=SCENINF.GetNewGroup();
			for(int i=0;i<ImNSL[MyNation];i++){
				word id=ImSelm[MyNation][i];
				word sn=ImSerN[MyNation][i];
				UG->AddNewUnit(Group[id]);
			}
			UseGroupLikeBridgeBuilders(UG,3,MyNation);
		}
		return false;
	}
};
#define MaxBInPt 8
word GetOneBld(int cell,int pos){
	if(cell>=0&&cell<VAL_MAXCIOFS&&pos<MaxBInPt){
        if(BLDList[cell])return BLDList[cell][pos];
	}
	return 0xFFFF;
}
void RefreshBiuldings(){
	void SetMonstersInCells();
	SetMonstersInCells();
	OneObject* OB;
	int ofst,ofst1,k;
	for(int i=0;i<MAXOBJECT;i++){
		OB=Group[i];
		if(OB){
			if(OB->NewBuilding){
				word MID;
				ofst=(OB->RealX>>11)+((OB->RealY>>11)<<VAL_SHFCX)+VAL_MAXCX+1;
				for(int pos=0;(MID=GetOneBld(ofst,pos))!=0xFFFF;pos++){
					if(MID!=OB->Index){
						OneObject* OBJ=Group[MID];
						if(OBJ&&Norma(OBJ->RealX-OB->RealX,OBJ->RealY-OB->RealY)<16*16){
							void EliminateBuilding(OneObject* OB);
							EliminateBuilding(OBJ);
							delete(Group[MID]);
                            Group[MID]=NULL;
						}
					}
				}				
			}
		}
	}
}
void NormalizeFlagbearers(){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->BrigadeID==0xFFFF){
			if(OB->Ref.General->UsualFormID!=0xFFFF){
                OneObject* TransformUnitType(OneObject* OB,int DestType);
				TransformUnitType(OB,OB->Ref.General->UsualFormID);
			}
		}
	}
}
void NormalizeFormations(){
	for(int ni=0;ni<8;ni++){
		City* CT=CITY+ni;
		for(int j=0;j<CT->NBrigs;j++){
			Brigade* BR=CT->Brigs+j;
			if(BR->Enabled&&BR->WarType){
                int fidx=BR->GetFormIndex();                
				bool GetOfficersType(byte NI, word UnitType, word &OffID, word &BarabanID, word &FlagID);
				word ofid,barid,flagid;
				if(GetOfficersType(ni,BR->MembID,ofid,barid,flagid)){
					GeneralObject* GO=NATIONS[ni].Mon[ofid];
					if(GO->OFCR){
						OfficerRecord* OFR=GO->OFCR;
						for(int j=0;j<OFR->NStroi;j++){
                            StroiDescription* SDS=OFR->SDES+j;
							for(int q=0;q<SDS->NAmount;q++)if(SDS->Amount[q]==BR->NMemb-NBPERSONAL){
								OrderDescription* ODS0=ElementaryOrders+BR->WarType-1;
								OrderDescription* ODS=ElementaryOrders+SDS->LocalID[q];
								if(ODS->GroupID!=0xFF&&ODS0->GroupID!=0xFF&&ODS->GroupID%3==ODS0->GroupID%3){									
									BR->WarType=SDS->LocalID[q]+1;
									int xx,yy;
									BR->GetCenter(&xx,&yy);
									BR->CreateOrderedPositions(xx*16,yy*16,BR->Direction);
									BR->ResortMembByPos();								
									BR->KeepPositions(0,128+16);
								}
							}
						}
					}
				}
			}
		}
	}
}
void SavePiece(SaveBuf* sb,int x,int y,int x0,int y0,int x1,int y1){
	DWORD C='TRAP';
	xBlockWrite(sb,&C,4);
	ExtRef* ER=&ExtReferences[90];
	int N=ER->GetAmount();	
	for(int i=0;i<N;i++){
		DWORD Code,sz;
		xBlockWrite(sb,&Code,4);
		xBlockWrite(sb,&Code,4);
		int p=sb->Pos;
		Code=(*ER)[i]->OnSavePiece(sb,x,y,x0,y0,x1,y1);
		sz=sb->Pos-p;
		if(sz){
            ((DWORD*)p)[-2]=Code;
			((DWORD*)p)[-1]=sz;
		}else{
			sb->Pos-=8;
			sb->Size-=8;
		}
	}
	if(!RefInit[90]){
		for(int i=N-1;i>=0;i--){
			if(!(*ER)[i]->Check(90)){
				ER->Del(i,1);
			}
		}
		RefInit[90]=1;
	}    
}
void LoadPiece(SaveBuf* sb,int x,int y){
	DWORD C;
	xBlockRead(sb,&C,4);
	if(C=='TRAP'){
		ExtRef* ER=&ExtReferences[91];
		int N=ER->GetAmount();	
		do{
			int sz;
			xBlockRead(sb,&C,4);
			xBlockRead(sb,&sz,4);
			bool done=false;
			int N=ER->GetAmount();	
			for(int i=0;i<N;i++){
				if((*ER)[i]->OnLoadPiece(sb,C,x,y)){
					break;
					done=true;
				}
			}
			if(!done){
				sb->Pos+=sz;
			}
		}while(sb->Pos<sb->Size);
	}
}
void PreviewPiece(SaveBuf* sb,int x,int y){
	DWORD C;
	xBlockRead(sb,&C,4);
	if(C=='TRAP'){
		ExtRef* ER=&ExtReferences[92];
		int N=ER->GetAmount();	
		do{
			int sz;
			xBlockRead(sb,&C,4);
			xBlockRead(sb,&sz,4);
			bool done=false;
			int N=ER->GetAmount();	
			for(int i=0;i<N;i++){
				if((*ER)[i]->OnPreviewPiece(sb,C,x,y)){
					break;
					done=true;
				}
			}
			if(!done){
				sb->Pos+=sz;
			}
		}while(sb->Pos<sb->Size);
	}
}
#include "Surface\CCombineExtension.h"
#include "Camera\Navigator.h"
#include "Surface\Colorize.h"
#include "Surface\Facturize.h"
#include "Surface\Deform.h"
#include "Surface\Smooth.h"
#include "Surface\Archway.h"
#include "Surface\Racist.h"
#include "Surface\SurfExporter.h"
#include "Surface\Typify.h"
#include "Surface\CmdExt.h"
#include "Surface\CmdCallbacks.h"
#include "Surface\Layer.h"
#include "Surface\Scissors.h"
#include "Surface\CollidingCamera.h"
#include "Surface\TankController.h"
#include "Surface\LocalGenerator.h"
#include "Surface\LocalExporter.h"
#include "Surface\Messager.h"
#include "Surface\HiResScreenShot.h"
#include "Surface\Chamfer.h"
#include "Surface\Manipulators.h"

void UnitAbilityTest();
void on_ProcessGame(){
	//UnitAbilityTest();
	ExtRef* ER=&ExtReferences[0];
	int N=ER->GetAmount();
	for(int i=0;i<N;i++){
		//UnitAbilityTest();
		GameExtension* GE=(*ER)[i];
		if(GE)
			GE->ProcessingGame();
		//UnitAbilityTest();
	}		
	//UnitAbilityTest();
}
// Insert there
void IstalAllExtensions(){
	InitExtensions();

	InstallExtension(&g_CollidingCamera, "CollidingCamera");
	InstallExtension(&g_Navigator, "Navigator");
	InstallExtension(&g_TankController, "TankController");
	InstallExtension(new Surface::Colorize, "Colorize");
	InstallExtension(new Surface::Facturize, "Facturize");
	InstallExtension(new Surface::Deform, "Deform");
	InstallExtension(new Surface::Smooth, "Smooth");
	InstallExtension(new Surface::CCombineExtension(&g_UniversalMap), "Combine");
	InstallExtension(new Surface::Archway, "Archway");
	InstallExtension(new Surface::Racist, "Racist");
	InstallExtension(new Surface::SurfExporter, "Exporter");
	InstallExtension(new Surface::Typify, "Typify");
	InstallExtension(new Surface::CmdExt(Surface::SimplifyCallback), "Simplify");
	InstallExtension(new Surface::CmdExt(Surface::GenerateCallback), "Generate");
	InstallExtension(new Surface::CmdExt(Surface::RefreshCallback), "Refresh");
	InstallExtension(new Surface::Layer, "Layer");
	InstallExtension(new Surface::Scissors, "Scissors");
	InstallExtension(new Surface::LocalGenerator, "LocalGenerator");
	InstallExtension(new Surface::LocalExporter, "LocalExporter");
	InstallExtension(&g_Messager, "Messager");
	InstallExtension(&g_HiResScreenShot, "HiResScreenShot");
	InstallExtension(new Surface::Chamfer, "Chamfer");
	InstallExtension(new Surface::Grass, "Grass");
	InstallExtension(new Surface::Manipulators, "Manipulators");

	InstallExtension(new MapOptionsGameSaver, "MapOptionsGameSaver");
	InstallExtension(new SceneObjExtension, "ModelsInScene");
	InstallExtension(new PropertyPanel,"PropertyPanel");

//	InstallExtension(new SelectSurfaceType(0),"---------Select current surface type---------");
//	Enumerator* E=ENUM.Get("SURFACETYPE");
//	if(E){
//		for(int i=0;i<E->GetAmount();i++){				
//			InstallExtension(new SelectSurfaceType(E->GetVal(i)),E->GetStr(i));
//		}

	InstallExtension(new TestExtension,"TestExtension");	
	// cheats	
	InstallExtension(new cext_Cheat_StartAI,"Start AI in editor for current nations");
	InstallExtension(new BoidsExtension,"Boids Process");
	InstallExtension(new WeaponSystemExtension,"WeaponSystemExtension");
	InstallExtension(new UnitAbilityExtension,"UnitAbilityExtension");
	InstallExtension(new BrigadeAbilityExtension,"BrigadeAbilityExtension");
	InstallExtension(new BattleShipAI,"BattleShipAI");
	InstallExtension(new PlaceInAJob,"PlaceInAJob");

	
	//Fantasy AI
	//void InstallFantasyAI();
	//InstallFantasyAI();

	// Vitya //  [3/16/2004] // Battle Editor
	void BE_InstallExtension();
	BE_InstallExtension();
	// Vitya //  [6/16/2004] // Alert Editor
	void Alert_InstallExtension();
	Alert_InstallExtension();

	void RegisterBrigAbl();
	RegisterBrigAbl();
	void Install_cext_VeryStupid();
	Install_cext_VeryStupid();
	void InstallWallsSaver();
	InstallWallsSaver();
	void mai_mInit();
	mai_mInit();
	void RegisterVC_Saver();
	RegisterVC_Saver();
	void RegModifiedUnitsSaver();
	RegModifiedUnitsSaver();

	// vital
	InstallExtension(new cext_VisualInterface,	"Message when: defeat, victory or disconnect");
	InstallExtension(new cv_AI_ScriptExt,	"ai script");

    void InstallMassiveRoom();
    InstallMassiveRoom();

	//loading plugins
	void LoadPermanentPlugins();
	LoadPermanentPlugins();
}
void PrintTex(){
	FILE* F=fopen("tex.log","w");
	if(F){
		int N=IRS->GetNTextures();
		int* ti=new int[N*2];
		int nt=0;
		for(int i=0;i<N;i++){
			TextureMemoryPool t=IRS->GetTexturePool(i);
			if(t==mpManaged){						
				ti[nt*2]=IRS->GetTextureSize(i);
				ti[nt*2+1]=i;
				nt++;
			}
		}
		bool c=false;
		do{
            c=false;
			for(int i=1;i<nt;i++){
				if(ti[i*2]>ti[i*2-2]){
					swap(ti[i*2],ti[i*2-2]);
					swap(ti[i*2+1],ti[i*2-1]);
					c=true;
				}
			}
		}while(c);
		char* fmt[]={"Unknown","ARGB4444","XRGB1555","ARGB8888","RGB565","A8","RGB888","XRGB8888","V8U8","R16F","GR16F","ABGR16F","R32F","GR32F","ABGR32F","DXT3"};     
		for(int i=0;i<nt;i++){
			int t=ti[i*2+1];
			int tf=IRS->GetTextureFormat(t);
			fprintf(F,"%3d:[%4d] %6dk %dx%d [%s] %s\n",i,t,ti[i*2]/1024,IRS->GetTextureWidth(t),IRS->GetTextureHeight(t),tf<=15?fmt[tf]:"???",IRS->GetTextureName(t));
		}
		fclose(F);
	}
}