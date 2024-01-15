#include "stdheader.h"
#include "unitability.h"
#include "VictoryConditions.h"
#include "UnitTypeGroup.h"
#include ".\cvi_HeroButtons.h"
#include ".\cext_VisualInterface.h"
//////////////////////////////////////////////////////////////////////////
extern City CITY[8];
extern int TrueTime;
extern word NPlayers;
void CmdEndGame(byte NI,byte state,byte cause);
//
int CreateNewZone(char* Name,int x,int y,int R);
extern bool EditMapMode;
extern byte PlayGameMode;
//////////////////////////////////////////////////////////////////////////
const char* carr_CapturableZone::GetThisElementView(){
	static char pf_text[64];
	//strcpy(pf_text,GetClassName());
	sprintf(pf_text,"Zones to defend - Amount: %d", GetAmount());
	return pf_text;
}
int carr_CapturableZone::ExpandWith(const char* ElmName,void* base){	
	int id=ClassArray<_AZone>::ExpandWith(ElmName,base);	
	if(id!=-1 && EditMapMode/*&&(PlayGameMode||EditMapMode)*/){
		carr_CapturableZone* arr=dynamic_cast<carr_CapturableZone*>((BaseClass*)base);
		if(arr){			
			VC_CZDB_Player* pl=dynamic_cast<VC_CZDB_Player*>(arr->GetParent());
			int col=-1;
			if(pl) col=pl->ColorID;
			_AZone* z=(*arr)[id];			
			z->print("Player: %d, Zone: %d - capturable",col,id); //z->GetClassName()
			ActiveZone* AZ=NULL;			
			for(int i=0;i<AZones.GetAmount();i++)if(AZones[i]->Name==z->pchar()){
				AZ=AZones[i];				
				break;
			}
			if(!AZ){
				int x=(mapx<<5)+(RealLx>>1);
				int y=(mapy<<5)+RealLy;
				int zid=CreateNewZone(z->pchar(),x,y,500); //1000+id*200,1000+id*200
				AZ=AZones[zid];
				if(AZ){
					c_CapturableExInfo* zInf=new c_CapturableExInfo;
					zInf->Owner=col;
					AZ->AdditionalInfo.Add(zInf);
					//z->AZone=AZ;
				}
			}else{
				//z->AZone=AZ;
			}
		}
	}
	return id;
};
bool carr_CapturableZone::DelElement(int Index){
	int n=GetAmount();
	if(n){
		_AZone* z=(*this)[n-1];
		for(int i=0;i<AZones.GetAmount();i++)if(AZones[i]->Name==z->pchar()){
			AZones.Del(i,1);
			break;
		}		
		Del(n-1,1);
	}
	return true;
};
//
typedef bool tpUnitsCallback(OneObject* OB,void* param);
CEXPORT int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
int vc_Enemy;
int vc_Friend;
int vc_Netral;
bool fvc_OwnetCollector(OneObject* OB,void* param){
	if(!OB->NotSelectable&&OB->BrigadeID!=0xFFFF){
		byte NMask=*(byte*)param;
		if(OB->NNUM==7){
			vc_Netral++;
		}else
		if(OB->NMask&NMask){
			vc_Friend++;
		}else{
			vc_Enemy++;
		}
	}
	return false;
};
bool VC_CZDB_Player::DetectDefeat(byte NMask){
	Nzones=AmountForLoose; // amount zone which player may loose for defeat
	for(int i=0;i<AZones.GetAmount();i++){
		ActiveZone* z=AZones[i]->GetAZone();		
		if(NMask==0) NMask=1<<ColorID;
		//
		/*
		CurrentGameInfo* g=&GSets.CGame;		
		for(int p=0;p<NPlayers;p++){
			PlayerInfo* PI=g->PL_INFO[p];
			if(PI&&PI->GroupID==Team){
				NMask|=1<<p;
			}
		}
		*/
		//
		vc_Enemy=vc_Friend=vc_Netral=0;
		if(z)PerformActionOverUnitsInRadius(z->x,z->y,z->R,fvc_OwnetCollector,&NMask);
		byte owner=0xFF;
		if(vc_Friend>0&&vc_Enemy==0){
			owner=ColorID;
		}else
		if(vc_Friend==0&&vc_Enemy>0){
			//Nzones--;
			owner=ColorID+8;
		}
		if(z){
			c_CapturableExInfo* zi=(c_CapturableExInfo*)z->GetInfo(_ZONE_EX_INFO_CAPTURABLE_,0);
			if(zi){
				if(owner!=0xFF){
					zi->Owner=owner;
				}
				if(zi->Owner>7){
					Nzones--;
				}
			}
		}
	}
	Nbrigs=0;
	for(int c=0;c<MaxNatColors;c++){
		if(NMask&(1<<c)){
			Nation* Nat=NATIONS+c;
			if(Nat->vsInfoInit){
				Nbrigs+=Nat->NBrInfantry.Active+Nat->NBrCavalery.Active+Nat->NCannons.Active;
			}else{
				Nbrigs=1;
				break;
			}
		}
	}
	return Nzones<=0||Nbrigs<=0;
};
//////////////////////////////////////////////////////////////////////////
void VC_TheKingOfTheHill::CheckVictoryAndDefeat(){
	int NBR[MaxNatColors];
	memset(&NBR,0,sizeof NBR);
	for(int i=0;i<MaxNatColors;i++){
		for(int j=0;j<CITY[i].NBrigs;j++){
			Brigade* BR=CITY[i].Brigs+j;
            int xb,yb;
			if(BR->GetCenter(&xb,&yb)){
				int RR=Norma(xc-xb,yc-yb);
				if(RR<r){
					NBR[i]++;
				}
			}
		}
	}
	if(CurrentOwner!=-1&&CurrentPretender==-1){
		for(int i=0;i<MaxNatColors;i++)if(i!=CurrentOwner&&NBR[i]){
			//someone owns the base, but there is more pretenders 
            CurrentPretender=i;
			PretenderStartTime=TrueTime;
		}
		if(CurrentOwner!=-1&&TrueTime-OwnerStartTime>VictoryTime*1000){
			//someone owns the base for a long enough time
			for(int j=0;j<MaxNatColors;j++)
				if(j==CurrentOwner)CmdEndGame(CurrentOwner,2,1);//victory
				else CmdEndGame(CurrentOwner,1,1);//defeat
		}
	}
	if(CurrentOwner!=-1&&CurrentPretender!=-1){
		if(TrueTime-PretenderStartTime>PretendingTime*1000){
			//nobody now owns the base
			CurrentOwner=-1;
			CurrentPretender=-1;
		}
	}
	if(CurrentOwner==-1&&CurrentPretender==-1){
		for(int i=0;i<MaxNatColors;i++)if(NBR[i]){
			//if someone has brigades near point, he can pretend on base
			CurrentPretender=i;
			PretenderStartTime=TrueTime;
		}
	}
	if(CurrentPretender!=-1){
		//somebody pretends on base
		for(int i=0;i<MaxNatColors;i++)
			if(i!=CurrentPretender&&i!=CurrentOwner&&NBR[i])
				CurrentPretender=-1;//there cant be two pretenders
		if(CurrentPretender!=-1&&NBR[CurrentPretender]==0)CurrentPretender=-1;
	}
	if(CurrentOwner==-1&&CurrentPretender!=-1){
		if(TrueTime-PretenderStartTime>PretendingTime*1000){
			CurrentOwner=CurrentPretender;
			OwnerStartTime=TrueTime;
			CurrentPretender=-1;
		}
	}
}
void VC_TheKingOfTheHill::DrawOnMiniMap(int xm,int ym){
	DWORD COLOR=0xFFFFFFFF;
	if(CurrentOwner!=-1){
		DWORD GetNatColor(int i);
        COLOR=GetNatColor(CurrentOwner);
	}
	GPS.DrawLine(xm+(xc>>(ADDSH+5))-4,ym+(yc>>(ADDSH+5))-4,xm+(xc>>(ADDSH+5))+4,ym+(yc>>(ADDSH+5))+4,COLOR);
	GPS.DrawLine(xm+(xc>>(ADDSH+5))+4,ym+(yc>>(ADDSH+5))-4,xm+(xc>>(ADDSH+5))-4,ym+(yc>>(ADDSH+5))+4,COLOR);
}

void VC_TheKingOfTheHill::GetPlayerStatusString(byte NI,char* Dest){
    static const char* PRETENDER=GetTextByID("Pretender");
	static const char* OWNER=GetTextByID("HillOwner");
	if(CurrentOwner==NI){
		int DT=VictoryTime-(TrueTime-OwnerStartTime)/1000;
		if(DT<0)DT=0;
		sprintf(Dest,"%s %d",OWNER,DT);
	}else
	if(CurrentPretender==NI){
		int DT=PretendingTime-(TrueTime-PretenderStartTime)/1000;
		if(DT<0)DT=0;
		sprintf(Dest,"%s %d",PRETENDER,DT);
	}
}
void VC_TheKingOfTheHill::InitOnCurrentMap(){
	VictoryConditionRoot::InitOnCurrentMap();
	Reset();
	for(int i=0;i<AZones.GetAmount();i++)if(!stricmp(AZones[i]->Name.pchar(),"KINGOFTHEHILL")){
		xc=AZones[i]->x;
		yc=AZones[i]->y;
		r=AZones[i]->R;
	}
}
char* VC_TheKingOfTheHill::CheckIfValidForThisMap(){
	if(r==0)return GetTextByID("NoZone_KINGOFTHEHILL");
	else return NULL;
}
void VC_TheKingOfTheHill::SaveData(SaveBuf* SB){
}
void VC_TheKingOfTheHill::LoadData(SaveBuf* SB){

}
void VC_TheKingOfTheHill::Reset(){
	xc=0;yc=0;r=0;
	CurrentOwner=-1;
	CurrentPretender=-1;
	OwnerStartTime=-1;
	PretenderStartTime=-1;
}


//////////////////////////////////////////////////////////////////////////
CIMPORT word GetNTribes(byte Owner);
CIMPORT word GetNTribes();
//
void VC_DestroyTownHalls::CheckVictoryAndDefeat(){
	if(TrueTime>NextRefreshTime){
		NextRefreshTime=TrueTime+MAXOBJECT*2;
		memset(Amount,0,sizeof Amount);
		//
		int Peasant[8];
		memset(Peasant,0,sizeof Peasant);
		//
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&!OB->Sdoxlo&&OB->Ready&&OB->NNUM<MaxNatColors){
				if(OB->Usage==CenterID){
					Amount[OB->NNUM]++;
				}
				if(OB->newMons->Peasant){
					Peasant[OB->NNUM]++;
				}
			}
		}		
		int time=TimeToBuildMinimal*1000;
		//
		byte MyNI=GSets.CGame.cgi_NatRefTBL[MyNation];
		bool victory=true;
		bool defeat=true;
		if(NPlayers<1)NPlayers=1;
		for(int i=0;i<NPlayers||COMPSTART[i];i++){
			int cID=GSets.CGame.PL_INFO[i].ColorID;
			byte c=GSets.CGame.cgi_NatRefTBL[cID];
			bool amount=Amount[c]>=MinimalAmount;
			bool tribe=GetNTribes()==0||GetNTribes(c)>0;
			if(!Init[c]&&amount){
				Init[c]=true;
			}
			bool def=Init[c]&&!amount||!tribe||(!amount&&Peasant[c]==0);
			if(!(NATIONS[c].NMask&NATIONS[MyNI].NMask)){
				if(!def){
					victory=false;
				}
			}			
			if(c==MyNI&&def){
				if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){					
					cva_VI_EndGame::UserMessage=newstr(GetTextByID("#DestroyTownDef")); //DefMessage
				}
				CmdEndGame(MyNation,1,1); //defeat
				victory=false;
				break;
			}
			if(i>=NPlayers&&def){
				CmdEndGame(cID,1,1);
				//NATIONS[c].AI_Enabled=false;
				//NATIONS[c].VictState=1;
			}
		}
		if(victory){ //&&TrueTime>time
			if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){
				cva_VI_EndGame::UserMessage=newstr(GetTextByID("#DefVictory")); //DestroyTownVic
			}
			CmdEndGame(MyNation,2,1); //victory
		}	
	}
}
void VC_DestroyTownHalls::GetPlayerStatusString(byte NI,char* Dest){
	static const char* HALL=GetTextByID("#TownHalls");
	sprintf(Dest,"%s: %d",HALL,Amount[NI]);
}
void VC_DestroyTownHalls::InitOnCurrentMap(){
	Reset();
	/*
	void SetF1Text(char* Help);
	char* h=GetTextByID("#CaptureZonesDestroyBrigadesF1");	
	SetF1Text(h);	
	*/
	VictoryConditionRoot::InitOnCurrentMap();	
}
void VC_DestroyTownHalls::Reset(){
	memset(Init,0,sizeof Init);
	memset(Amount,0,sizeof Amount);
	MinimalAmount=1;
	NextRefreshTime=0;
	TimeToBuildMinimal=90;
}
//////////////////////////////////////////////////////////////////////////
void CreateSumm();
extern int SumAccount[8];
void VC_BaseFantasy::CheckVictoryAndDefeat(){
	if(GSets.RoomParams.GameType>0 && GSets.CGame.PL_INFO[0].GameTime==0){		
		CreateSumm();
		int MaxS=0;				
		for(int i=0;i<7;i++){			
			int S=SumAccount[i];
			if(S>MaxS){
				MaxS=S;
			}
		}
		for(int i=0;i<NPlayers||COMPSTART[i];i++){
			int cID=GSets.CGame.PL_INFO[i].ColorID;
			byte c=GSets.CGame.cgi_NatRefTBL[cID];
            if(SumAccount[c]==MaxS)CmdEndGame(cID,2,111);
			else CmdEndGame(cID,1,112);
		}
	}
	if(TrueTime>NextRefreshTime){
		NextRefreshTime=TrueTime+1200+MAXOBJECT;
		DynArray<int> Heroes;
		DynArray<int> Peasants;
		DynArray<int> TownHalls;
		Heroes.Add(0,MaxNatColors);
		Peasants.Add(0,MaxNatColors);
		TownHalls.Add(0,MaxNatColors);
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if( OB && OB->GetLiveState()==ls_LiveBody ){
				if(DetectHero(OB)){
					Heroes[OB->NNUM]++;
				}else if(OB->newMons->Peasant){
					Peasants[OB->NNUM]++;
				}else if(OB->Usage==CenterID){
					TownHalls[OB->NNUM]++;
				}
			}
		}		
		byte MyNI=GSets.CGame.cgi_NatRefTBL[MyNation];
		bool victory=true;
		//bool defeat=true;
		//if(NPlayers<1)NPlayers=1;
		for(int i=0;i<NPlayers||COMPSTART[i];i++){
			int cID=GSets.CGame.PL_INFO[i].ColorID;
			byte c=GSets.CGame.cgi_NatRefTBL[cID];
			if( !InitPeasants[c] && Peasants[c]>=MinimalPeasants )
				InitPeasants[c]=true;
			if( !InitTownHalls[c] && TownHalls[c]>=MinimalTownHalls )
				InitTownHalls[c]=true;

			char* def_msg=NULL;
			bool def=false;
			if( Heroes[c]==0 && !InitPeasants[c] ){
				def=true;
				static char* msg=GetTextByID("#BaseFantasyDefHeroDeath");
				def_msg=msg;
			}else if( !InitTownHalls[c] && InitPeasants[c] && Peasants[c]<MinimalPeasants ){
				def=true;
				static char* msg=GetTextByID("#BaseFantasyDefPeasants");
				def_msg=msg;
			}else if( InitTownHalls[c] && TownHalls[c]<MinimalTownHalls ){
				def=true;
				static char* msg=GetTextByID("#BaseFantasyDefTownHalls");
				def_msg=msg;
			}

			if(!(NATIONS[c].NMask&NATIONS[MyNI].NMask)){
				if(!def){
					victory=false;
				}
			}			
			if(c==MyNI&&def){
				//if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){					
					cva_VI_EndGame::UserMessage=newstr(def_msg);
				//}
				CmdEndGame(MyNation,1,1); //defeat
				victory=false;
				break;
			}
			if(i>=NPlayers&&def){
				CmdEndGame(cID,1,1);
				//NATIONS[c].AI_Enabled=false;
				//NATIONS[c].VictState=1;
			}
		}
		if(victory){
			static char* msg=GetTextByID("#BaseFantasyVic");
			cva_VI_EndGame::UserMessage=newstr(msg);
			//if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){
			//	cva_VI_EndGame::UserMessage=newstr(VicMessage);
			//}
			CmdEndGame(MyNation,2,1); //victory
		}	
	}
}
void VC_BaseFantasy::GetPlayerStatusString(byte NI,char* Dest){	
	sprintf(Dest,"%d",CITY[NI].Account/100);        
}
void VC_BaseFantasy::InitOnCurrentMap(){
	Reset();
	VictoryConditionRoot::InitOnCurrentMap();
}
void VC_BaseFantasy::Reset(){
	InitPeasants.Clear();
	InitTownHalls.Clear();
	InitPeasants.Add(false,MaxNatColors);
	InitTownHalls.Add(false,MaxNatColors);
	MinimalPeasants=3;
	MinimalTownHalls=1;
	NextRefreshTime=0;
}
//////////////////////////////////////////////////////////////////////////
void VC_CaptureZonesDestroyBrigades::CheckVictoryAndDefeat(){
	if(EditMapMode) return;
	int Np=2;	// amount of player in game
	bool Defeat[2]={true,true};
	DynArray<int> Team[2]; // color of team
	VC_CZDB_Player* winner=NULL;
	byte MyNI=GSets.CGame.cgi_NatRefTBL[MyNation];
	for(int c=0;c<MaxNatColors;c++){
		VC_CZDB_Player* p=Player+c;
		Nation* Nat=NATIONS+c;
		if(Nat->VictState) p->VictState=Nat->VictState;
		if(p->Init||MOptions.Players.Player[c].LockTeam){			
			//
			if(MOptions.Players.Player[c].LockTeam){
				byte NMask=0;
				for(int a=0;a<MaxNatColors;a++){
					if(MOptions.Players.Player[c].Team==MOptions.Players.Player[a].Team){
						NMask|=1<<a;
					}
				}					
				for(int a=0;a<2;a++){
					if(MOptions.Players.Player[c].Team==MOptions.Players.Player[a].Team){
						VC_CZDB_Player* ap=Player+a;
						Team[a].Add(c); //ap->ColorID
						if(p->VictState!=0){
							Defeat[a]=false;
						}else{
							if(ap->DetectDefeat(NMask)){
								//if(MyNI==ap->ColorID && !cva_VI_EndGame::UserMessage){
								if(MyNI==c && !cva_VI_EndGame::UserMessage){
									cva_VI_EndGame::UserMessage=newstr(GetTextByID("#CaptZonesDef"));
								}
								CmdEndGame(c,1,1); //defeat
								ap->VictState=1;
							}else{
								Defeat[a]=false;
								if(ap->VictState==0){
									winner=ap;									
								}
							}							
						}
						break;
					}
				}				
			}else{
				if(p->VictState!=0){
					//CmdEndGame(ap->ColorID,1,1);
				}else{
					PlayerInfo* pInf=GSets.CGame.PL_INFO;
					for(int i=0;i<MaxNatColors;i++,pInf++) if(pInf->ColorID==c){
						if(p->DetectDefeat()){					
							CmdEndGame(p->ColorID,1,1); //defeat
							p->VictState=1;
						}else{
							winner=p;
						}
						break;
					}
				}
			}
		}
	}
	/*
	if(Np==1&&winner){
		CmdEndGame(winner->ColorID,2,1); //victory
		winner->VictState=2;
	}
	*/
	for(int a=0;a<2;a++){
		if(Defeat[a]){
			for(int i=0;i<Team[a].GetAmount();i++){
				byte NI=Team[1-a][i];
				if(MyNI==NI && !cva_VI_EndGame::UserMessage){
					cva_VI_EndGame::UserMessage=newstr(GetTextByID("#CaptZonesVic"));
				}
				CmdEndGame(NI,2,1); //victory
			}
			break;
		}
	}
}
void VC_CaptureZonesDestroyBrigades::GetPlayerStatusString(byte NI,char* Dest){
	static const char* ZonesToLost=GetTextByID("#TL_Zones");
	static const char* BrigadesToLost=GetTextByID("#TL_Brigades");
	static const char* FoodToLost=GetTextByID("#TL_Food");
	if(MOptions.Players.Player[NI].LockTeam){
		for(int a=0;a<2;a++){
			if(MOptions.Players.Player[NI].Team==MOptions.Players.Player[a].Team){
				NI=a;
				break;
			}
		}
	}	
	VC_CZDB_Player* p=Player+NI;
	if(p->Init) sprintf(Dest,"%s: %d, %s: %d, %s: %d",
		ZonesToLost,p->Nzones, BrigadesToLost,p->Nbrigs, FoodToLost,XRESRC(NI,FoodID)/100);
	else sprintf(Dest,"not init");
}
void VC_CaptureZonesDestroyBrigades::InitOnCurrentMap(){
	/*
	if(!(GameF1Text&&GameF1Text[0])){
		GameF1Text=GetTextByID("#CaptureZonesDestroyBrigadesF1");
	}
	*/
	void SetF1Text(char* Help);
	char* h=GetTextByID("#CaptureZonesDestroyBrigadesF1");	
	SetF1Text(h);
	//
	VictoryConditionRoot::InitOnCurrentMap();
	for(int i=0;i<MaxNatColors;i++){
		VC_CZDB_Player* p=Player+i;		
		p->VictState=0;
		p->Nzones=0;
		p->Nbrigs=0;
	}
}
// --- editor ---
class _VC_CZDB_Player_editor:public OneElementEditor{
	VC_CZDB_Player* PL;
	CheckBox* CB;
public:
	_VC_CZDB_Player_editor(){ PL=NULL; CB=NULL;	}
	virtual const char* GetClassID(){ return "VC_CZDB_Player"; }
	virtual OneElementEditor* _new(){ return new _VC_CZDB_Player_editor; }
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
							BaseClass* Class,void* DataPtr,void* ExtraPtr,ControlParams* CParam);	
	virtual bool Process();

};
int _VC_CZDB_Player_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
										  BaseClass* Class,void* DataPtr,void* ExtraPtr,ControlParams* CParam){
	PL=(VC_CZDB_Player*)DataPtr;
	CB=Base->addGP_CheckBox(NULL,x-CParam->LeftColumnWidth,y,"",&YellowFont,&YellowFont,0,PL->Init,0,1,0,0);
	CParam->LeftColumnWidth+=CParam->RightColumnWidth;
	CParam->RightColumnWidth=0;
	return y1;
};
bool _VC_CZDB_Player_editor::Process(){
	if(PL){
		if(CB)PL->Init=CB->State;
		if(PL->AmountForLoose<0) PL->AmountForLoose=0;
		if(PL->AmountForLoose>PL->AZones.GetAmount()) PL->AmountForLoose=PL->AZones.GetAmount();
	}	
	return true; 
}
extern DynArray<OneElementEditor*> type_list;
void RegizterVictoryConditionEditors(){
	type_list.Add(new _VC_CZDB_Player_editor);
}
extern bool NOPAUSE;
//VC_ScoreGame
void VC_ScoreGame::CheckVictoryAndDefeat(){	
	if(NOPAUSE){
		VC_DestroyTownHalls::CheckVictoryAndDefeat();
		LeftTime-=GameSpeed*8;
		if(LeftTime<0){
			int maxacc=0;
			byte myN=GSets.CGame.cgi_NatRefTBL[MyNation];
			byte MMASK=NATIONS[myN].NMask;

			int totscor[16];
			int npl[16];
			memset(totscor,0,sizeof totscor);
			memset(npl,0,sizeof npl);

			for(int i=0;i<7;i++){
				int acc=CITY[GSets.CGame.cgi_NatRefTBL[i]].Account;
				for(int j=0;j<7;j++)if(NATIONS[i].NMask&NATIONS[j].NMask){
					totscor[j]+=acc;
					npl[j]++;
				}
			}
			///for(int i=0;i<7;i++)if(npl[i])totscor[i]/=npl[i];
			for(int i=0;i<7;i++)if(totscor[i]>maxacc)maxacc=totscor[i];

			if(totscor[GSets.CGame.cgi_NatRefTBL[MyNation]]==maxacc){
				if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){
					cva_VI_EndGame::UserMessage=newstr(VicMessage);					
				}
				CmdEndGame(MyNation,2,0);
			}else{
				if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){
					cva_VI_EndGame::UserMessage=newstr(DefMessage);					
				}
				CmdEndGame(MyNation,1,0);
			}
		}
	}
}
void VC_ScoreGame::GetPlayerStatusString(byte NI,char* Dest){
	sprintf(Dest,"%d",CITY[GSets.CGame.cgi_NatRefTBL[NI]].Account);
}
void VC_ScoreGame::InitOnCurrentMap(){
	VictoryConditionRoot::InitOnCurrentMap();
    LeftTime=TimeLimit*60*25*256;
}
void VC_ScoreGame::DrawOnMiniMap(int x,int y){
	char cc[32];
	int dt=LeftTime/256/25;
	if(dt<60*600){
		sprintf(cc,"%02d:%02d",dt/60,dt%60);
		ShowString(x+2,y+2,cc,&SmallWhiteFont);
	}
}
//VC_CaptureGame
void VC_CaptureGame::CheckVictoryAndDefeat(){
	if(NOPAUSE){
		VC_DestroyTownHalls::CheckVictoryAndDefeat();
		LeftTime-=GameSpeed*8;
		if(LeftTime<0){
			int maxacc=0;
			byte myN=GSets.CGame.cgi_NatRefTBL[MyNation];
			byte MMASK=NATIONS[myN].NMask;

			int totscor[16];
			int npl[16];
			memset(totscor,0,sizeof totscor);
			memset(npl,0,sizeof npl);

			for(int i=0;i<7;i++){
				int* R=&NATIONS[GSets.CGame.cgi_NatRefTBL[i]].ResTotal[0];
				int acc=(R[1]+R[4]+R[5])*10;				
				for(int j=0;j<7;j++)if(NATIONS[i].NMask&NATIONS[j].NMask){
					totscor[j]+=acc;
					npl[j]++;
				}
			}
			//for(int i=0;i<7;i++)if(npl[i])totscor[i]/=npl[i];
			for(int i=0;i<7;i++)if(totscor[i]>maxacc)maxacc=totscor[i];

			if(totscor[GSets.CGame.cgi_NatRefTBL[MyNation]]==maxacc){
				if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){
					cva_VI_EndGame::UserMessage=newstr(VicMessage);					
				}
				CmdEndGame(MyNation,2,0);
			}else{
				if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){
					cva_VI_EndGame::UserMessage=newstr(DefMessage);					
				}
				CmdEndGame(MyNation,1,0);
			}
		}
	}
}
void VC_CaptureGame::GetPlayerStatusString(byte NI,char* Dest){
	int* R=&NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].ResTotal[0];
	int acc=R[1]+R[4]+R[5];
	sprintf(Dest,"%d",acc/100);
}
//
extern word COMPSTART[8];
bool CheckBar(int x,int y,int Lx,int Ly);
DLLEXPORT int GetRND(int Max);
// VC_AnnihilateHero
void VC_AnnihilateHero::CheckVictoryAndDefeat(){
	byte MyPID=MyNation; //GSets.CGame.cgi_NatRefTBL[MyNation];
	PlayerInfo* PI=GSets.CGame.GetCurrentPlayerInfo();
	bool victory=true;
	if(NPlayers<1)NPlayers=1;
	for(int i=0;i<NPlayers||COMPSTART[i];i++){
		PlayerInfo* I=GSets.CGame.PL_INFO+i;
		int c=I->ColorID;
		int myc=GSets.CGame.cgi_NatRefTBL[MyNation];
		Amount[c]=vHeroButtons.GetAmount(c);
		if(c==myc){
			if(Amount[c]==0){
				if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){
					cva_VI_EndGame::UserMessage=newstr(DefMessage);					
				}
				CmdEndGame(c,1,1); //defeat
				victory=false;
			}
		}else{			
			if(Amount[c]>0&&(PI->GroupID==0||PI->GroupID!=I->GroupID)&&PI->ColorID!=I->ColorID){
				victory=false;				
			}
			if(!Amount[c])CmdEndGame(c,1,1);
		}
	}
	if(victory){
		if(!(cva_VI_EndGame::UserMessage&&cva_VI_EndGame::UserMessage[0])){
			cva_VI_EndGame::UserMessage=newstr(VicMessage);			
		}
		CmdEndGame(PI->ColorID,2,1); //victory
	}
};
void VC_AnnihilateHero::GetPlayerStatusString(byte NI,char* Dest){
	sprintf(Dest,"Heroes amount: %d",Amount[NI]);
};
int CreateNewTerrMons2(byte NI,int x,int y,word Type);
void VC_AnnihilateHero::InitOnCurrentMap(){
	VictoryConditionRoot::InitOnCurrentMap();
	for(int p=0;p<NPlayers||COMPSTART[p];p++){
		byte NI=GSets.CGame.PL_INFO[p].ColorID; //GSets.CGame.cgi_NatRefTBL[MyNation];
		byte NationID=GSets.CGame.PL_INFO[p].NationID;
		City* CT=CITY+NI;
		//
		word* id=NatList[NI];
		int   n=NtNUnits[NI];
		int cx=0;
		int cy=0;
		int nc=0;
		int cpx=0;
		int cpy=0;
		int ncp=0;
		for(int j=0;j<n;j++)		{
			OneObject* OB=Group[id[j]];
			if(OB){
				NewMonster* NM=OB->newMons;
				if(NM->Peasant){
					cpx+=OB->RealX/16;
					cpy+=OB->RealY/16;
					ncp++;
				}
				cx+=OB->RealX/16;
				cy+=OB->RealY/16;
				nc++;
			}
		}
		if(ncp){			
			CT->CenterX=cpx/ncp;
			CT->CenterY=cpy/ncp;
		}else
		if(nc){
			CT->CenterX=cx/nc;
			CT->CenterY=cy/nc;
		}
		//
		Amount[NI]=0;
		for(int i=0;i<NHero;i++){
			word NIndex=UnitTypeGroups.GetType(i,NationID,true);
			if(NIndex<0xFFFF){
				for(int i=0;i<50;i++){
					int x=CT->CenterX+GetRND(800)-400;
					int y=CT->CenterY+GetRND(800)-400;
					if(!CheckBar((x/16)-10,(y/16)-10,20,20)){
						CreateNewTerrMons2(NI,x*16,y*16,NIndex);
						GeneralObject* GO=NATIONS[NI].Mon[NIndex];
						GO->ManualDisable=true;
						GO->ManualEnable=false;
						Amount[NI]++;
						break;
					}
				}
			}
		}
	}	
};
void VC_AnnihilateHero::Reset(){
	NHero=3;
	for(int i=0;i<MaxNatColors;i++){
		Amount[i]=0;//NHero;
	}	
};
class VC_Saver:public GameExtension{
public:
	virtual bool OnGameSaving(xmlQuote& xml){
        MOptions.VictoryRule.Save(xml,&MOptions.VictoryRule);
		return true;
	}
	virtual bool OnGameLoading(xmlQuote& xml){
		MOptions.VictoryRule.reset_class(&MOptions.VictoryRule);
		ErrorPager ep(0);
		MOptions.VictoryRule.Load(xml,&MOptions.VictoryRule,&ep);
		return true;
	}
};
void RegisterVC_Saver(){
	InstallExtension(new VC_Saver,"saver for victory conditions");
}