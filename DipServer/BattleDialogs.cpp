#include "CommonDip.h"
#include "Script.h"
#include "Mind.h"
//#include "..\COSSACKS2\Plugins.h"

class NewUpgrade {
public:
	//information
	char* Name;
	char* Message;
	byte Level;
	byte Branch;
	word IconFileID;
	word IconSpriteID;
	char IconPosition;
	byte NatID;
	word  Cost[8];
	byte CtgUpgrade;
	byte CtgType;
	union{
		int  NCtg;
		int  CtgValue;
	};
	word*    CtgGroup;
	byte UnitType;
	union{
		int NUnits;
		int UnitValue;
	};
	word* UnitGroup;
	byte ValueType;
	union{
		int  Value;
		int  NValues;
	};
	int* ValGroup;
	//mechanics
	bool Done:1;
	bool Enabled:1;
	bool PermanentEnabled:1;
	bool IsDoing:1;
	bool Gray:1;
	bool Individual:1;
	bool ManualDisable:1;
	bool ManualEnable:1;
	bool StageUp:1;
	word NStages;
	word CurStage;
	byte NAutoPerform;
	byte NAutoEnable;
	word* AutoPerform;
	word* AutoEnable;
	byte Options;
	byte StageMask;
};

CBattle Battle;

const STMR=30000;

CEXPORT void StartBattle(){
	Battle.Init();
};

void HandleBattleCml(char* com,char** params,int npr,ParsedRQ* Result){
	if(strcmp(com,"btl")||Battle.Start) return;
	byte ID=GetMyPlayerID();
	CPlayerInfo* PInfo=Battle.Info+ID;
	if(!strcmp(params[0],"ready")){
		word Data[2];
		Data[0]=2048;	// command
		Data[1]=ID;	// player color
		SendDipCommand((char*)Data,sizeof Data);
	}else
	if(!strcmp(params[0],"typsel")){
		CPlayerInfo* PInfo=Battle.Info+GetMyPlayerID();
		word ns=atoi(params[1]);
		if(PInfo->SelUT!=ns){
			PInfo->SelUT=ns;
			Battle.MakeDialog(true);
		}
	}else
	if(!strcmp(params[0],"upgchk")){
		if(PInfo->Ready) return;
		word Data[3];
		Data[0]=2049;
		Data[1]=ID;	// player color
		Data[2]=atoi(params[1]);	// upgrade id
		
		SendDipCommand((char*)Data,sizeof Data);

		/*
		CNation* PNat=Battle.Nat;
		byte nat=GetMyPlayerColor();
		bool* check=PNat[nat].UpgCheck+Data[2];
		*check=!*check;		

		ParsedRQ COM;
		COM.AddComm("LW_show");
		char buf[60];
		if(*check) sprintf(buf,":LW_vis&1&%s",params[2]);
			else sprintf(buf,":LW_vis&0&%s",params[2]);
		COM.AddParam(buf,strlen(buf));
		int size=COM.UnParse(NULL,0);
		COM.UnParse(buf,size);
		ActivateDipDialog(buf);
		*/

		/*
		char buf[30];
		DString COM;
		COM.ReadFromFile("Battle\\acheck.cml");
		for(int t=0;t<PInfo->NUT;t++){
			UnitType* UT=PInfo->UT+t;
			DString STR(COM.pchar());			
			bool find=false;
			for(int u=0;u<UT->NUpg;u++)if(UT->Upg[u]==Data[2]){
				find=true;
				itoa(u,buf,10);
				STR.Replace0("<UID>",buf);
				if(*check) STR.Replace0("<STATE>","1");
					else STR.Replace0("<STATE>","0");
			}
			if(find){
				itoa(t,buf,10);
				STR.Replace0("<TYPE>",buf);
				STR.WriteToFile("Battle\\tm2.cml");
				ActivateDipDialog("LW_file&Battle/timer.cml");
			}
		}
		*/
	}else
	if(!strcmp(params[0],"timer")){
		bool con=false;
		for(int i=0;i<8;i++){
			if(Battle.Info[i].Connect) con=true;
		}
		if(con&&!Battle.Start){
			int time=Battle.ShowTimer();
			if(time>0){
				if(time<5000){
					//PopupDipDialog();
					ShowFogOfWar();
					CPlayerInfo* PInfo=Battle.Info+ID;
					if(!PInfo->Ready){
						PInfo->Ready=1;
						Battle.MakeDialog(false);
						return;
					}
				}
				ActivateDipDialog("LW_file&Battle/timer.cml");
			}else{
				//PopupDipDialog();
				if(!PInfo->WaitStart){
					word Data[3];
					Data[0]=2051;
					Data[1]=ID;	// player color
					SendDipCommand((char*)Data,sizeof Data);
				}
			}
		}
	}
	
}

void RunStartTimer(){
	word Data[3];
	Data[0]=2050;
	Data[1]=GetMyPlayerID();	// player color
	SendDipCommand((char*)Data,sizeof Data);
}

int CBattle::ShowTimer(){
	char* var=TimerVarName;
	char* oldval=SXP_GetVar(1,var);
	long val=(long)StartTimer-(long)GetTickCount();//atoi(oldval);
	if(val<0){
		return false;
	}else{
		char newval[100];
		itoa(val/1000,newval,10);
		SXP_SetVar(1,var,newval);
	}
	return val;
}
void CBattle::SetStartTimer(int Time){
	StartTimer=GetTickCount()+Time;
};

CIMPORT void SetInfoStyle(byte Style,int LY);

void PerformBattleCommads(char* Data,int size){
	byte NI=GetMyPlayerColor();
	byte ID=GetMyPlayerID();
	word* wData=(word*)Data;
	word com=wData[0];
	word co1=wData[1];	// player id
	word co2=wData[2];	
	CPlayerInfo* BInfo=Battle.Info;
	if(2047<com&&com<3001){
		switch(com){
		case 2048:	//	ready			
			BInfo[co1].Ready=!BInfo[co1].Ready;
			Battle.MakeDialog(true);
			{				
				bool start=true;
				for(int i=0;i<8;i++)if(BInfo[i].Connect&&!BInfo[i].Ready){
					start=false;
					break;
				}
				if(start){
					int CurTime=GetTickCount();
					if(Battle.StartTimer-5000>CurTime) Battle.StartTimer=CurTime+5200;				
				}
			}
			break;
		case 2049:	// upgrade chek
			{
				byte nat=GetPlayerColor(co1);
				word* Cost=NULL;
				GetUpgCost(nat,co2,Cost);
				if(Cost){
					bool* check=Battle.Nat[nat].UpgCheck+co2;
					bool change=false;
					if(*check){
						// return money
						for(int i=0;i<6;i++) AddResource(nat,i,Cost[i]);
						change=true;
					}else{
						// give money
						bool deal=true;
						for(int i=0;i<6;i++) if(GetResource(nat,i)<Cost[i]){
							deal=false;
							break;
						}
						if(deal){
							for(i=0;i<6;i++) AddResource(nat,i,-Cost[i]);
							change=true;
						}
					}
					if(change){
						BInfo[co1].CheckUpg(co2,*check);
						*check=!(*check);						
						if(NI==nat){
							/*
							ParsedRQ COM;
							COM.AddComm("LW_show");
							char buf[512];
							buf[0]=':';
							buf[1]=0;
							int l=1;
							for(int t=0;t<BInfo->NUT;t++){
								UnitType* PInf=BInfo->UT+t;
								for(int u=0;u<PInf->NUpg;u++){
									if(PInf->Upg[u]==co2){
										buf[l++]='|';
										if(*check) sprintf(buf+l,"LW_vis&1&%%CH%d_%d",t,u);
											else sprintf(buf+l,"LW_vis&0&%%CH%d_%d",t,u);
										l=strlen(buf);
									}
								}
							}
							
							COM.AddParam(buf,strlen(buf));
							int size=COM.UnParse(NULL,0);
							COM.UnParse(buf,size);
							ActivateDipDialog(buf);
							*/
							Battle.MakeDialog(true);
						}
					}
				}
			}
			break;
		case 2050:	// start timer
			Battle.SetStartTimer(STMR);
			ActivateDipDialog("LW_file&Battle/timer.cml");
			break;
		case 2051:	// wait for other players
			{				
				if(!BInfo[co1].WaitStart&&co1==ID){					
					//Battle.MakeDialog(true);					
					ActivateDipDialog("LW_new&|LW_file&Battle/empty.cml");
					CloseDipDialog();
					SetDipDlgDX(0);
					SetInfoStyle(0,0);
				}
				BInfo[co1].WaitStart=1;				
				bool start=true;
				for(int i=0;i<8;i++) if(BInfo[i].Connect&&!BInfo[i].WaitStart){
					start=false;
					break;
				}
				if(start){
					SetNormalMode();
					Battle.Start=1;
					Battle.RunUpgrades();					
				}
			}
			break;
		}		
	}
}

CEXPORT
void CloseBattle(){
	Battle.Free();
}

void CBattle::Free(){
	//if(!Start) return;
	for(int i=0;i<8;i++){
		CPlayerInfo* PInfo=Info+i;
		CNation* PNat=Nat+i;
		for(int j=0;j<PInfo->NUT;j++){
			UnitType* UT=PInfo->UT+j;
			if(UT->NUpg){
				if(UT->Upg)free(UT->Upg);						
			}
		}		
		if(PInfo->UT)free(PInfo->UT);
		if(PNat->UpgCheck)free(PNat->UpgCheck);
	}
	memset(this,0,sizeof *this);
}


CBattle::CBattle(){
	memset(this,0,sizeof *this);	
};

CBattle::~CBattle(){	
	Free();
};

void CBattle::Init(){
	Free();

	SetDipDlgDX(82);
	SetInfoStyle(1,0);
	
	SetObservMode();	
	
	for(byte id=0;id<8;id++){

		byte NI=GetPlayerColor(id);
		char* name=GetPlayerName(id);

		if(name&&name[0]&&name[0]!='?'){
			CPlayerInfo* PInfo=Info+id;
			CNation* PNat=Nat+NI;

			memcpy(PInfo->Name,name,10);
			
			PInfo->Connect=1;
			if(Player[NI].NI==NI){
				PInfo->Ready=1;
				PInfo->WaitStart=1;
				continue;
			}else{
				PInfo->Ready=0;
			}

			// создать список апгрейдов для каждого монстра	
			int NMon=GetNMon(NI);
			int* NIndex=(int*)malloc(NMon*sizeof(int));
			memset(NIndex,0,NMon*sizeof(int));			
						
			GetPressentMonsters(NI,NIndex);
			
			do{
				word ID=0xFFFF;
				word MaxForce=0;
				for(int i=0;i<NMon;i++)if(NIndex[i]>MaxForce){
					ID=i;
					MaxForce=NIndex[i];
				}
				if(ID!=0xFFFF){
					PInfo->AddNewMon(NI,ID);					
					NIndex[ID]=0;
				}else{
					break;
				}
			}while(true);

			free(NIndex);
			
			if(!PNat->UpgCheck){
				// выделить память под заказ апгрейдов			
				int nupg=GetNUpgrades(NI);
				PNat->NUpgCheck=nupg;
				int size=nupg*sizeof(bool);
				PNat->UpgCheck=(bool*)malloc(size);
				memset(PNat->UpgCheck,0,size);
			}


		}
	}
	
	// запрещаем апгрейды в зданиях	
	byte NI=GetMyPlayerColor();
	int nupg=Nat[NI].NUpgCheck;
	for(int u=0;u<nupg;u++){
		//SetUpgEnableStatus(NI,u,0);
		NewUpgrade* NU=GetNewUpg(NI,u);
		NU->ManualDisable=1;
	}

	RunStartTimer();
	sprintf(TimerVarName,"%s%d","%GV_BT_TIME",GetMyPlayerID());
	
	MakeDialog(false);

	Battle.SetStartTimer(STMR);
	Battle.ShowTimer();
	
}

void CBattle::RunUpgrades(){
	byte NI=GetMyPlayerColor();
	int nupg=Nat[NI].NUpgCheck;
	for(int u=0;u<nupg;u++){
		//SetUpgEnableStatus(NI,u,1);
		NewUpgrade* NU=GetNewUpg(NI,u);
		NU->ManualDisable=0;
	}	

	for(int i=0;i<8;i++){
		CNation* PInfo=Nat+i;		
		for(int u=0;u<PInfo->NUpgCheck;u++)if(PInfo->UpgCheck[u]){
			GAMEOBJ UO;
			UO.Type='UPGR';
			UO.Index=u;
			InitialUpgradeNew(i,&UO,0xFFFF);
		}
	}
}

void CBattle::Process(){
	if(Start){
		// The End	
		byte prim[8];
		memset(prim,0,8);

		int MO=GetMaxObject();
		for(int i=0;i<MO;i++){
			OneObject* OB=GetOBJ(i);
			if(OB&&!OB->Sdoxlo){
				byte ONI=OB->NNUM;
				if(OB&&!OB->Sdoxlo&&!prim[ONI]){
					CPlayerScript* PS=PScript+ONI;
					if(PS->Enable){					
						for(int j=0;j<PS->NProtectGrp;j++){
							if(OB->NIndex==PS->ProtectGrp[j]) prim[ONI]=1;
						}
					}
				}
			}
		}
		
		for(i=0;i<8;i++) if(PScript[i].Enable&&!prim[i]){
			PScript[i].Defeat=1;
			//if(i==NI) NI=0xFF;
		}

		byte MNI=GetMyNation();
		if(PScript[MNI].Defeat){		
			LooseGame();
			Free();
		}else{
			bool vict=true;
			for(int i=0;i<8;i++){
				if(i!=MNI&&PScript[i].Enable&&!PScript[i].Defeat) vict=false;
			}
			if(vict){
				ShowVictory();
				Free();
			}
		}
	}

	//int NI=GetMyPlayerColor();
	//if(!Start){
	//}	
}

void CBattle::MakeDialog(bool frame){
	byte NI=GetMyPlayerColor();
	int PID=GetMyPlayerID();
	CPlayerInfo* PInfo=Info+PID;
	CNation* PNat=Nat+NI;

	DString CML;
	if(!frame){
		CML.ReadFromFile("Battle\\Main.cml");
		CML.Replace0("<MAKEUPG>",GetTextByID("INTF_MAKEUPG"));
	}
	DString FRM;
	FRM.ReadFromFile("Battle\\Frame.cml");
	CML+=FRM;

	char buf[250];

	DString PLR;	PLR.ReadFromFile("Battle\\Player.cml");

	// player info
	int ply=3;
	int ph=20;
	for(int i=0;i<8;i++)if(Info[i].Connect){
		byte CurColor=GetPlayerColor(i);
		DString TMP=PLR.pchar();
		 
		itoa(i,buf,10);
		TMP.Replace0("<ID>",buf);

		itoa(CurColor,buf,10);
		TMP.Replace0("<COLOR>",buf);

		itoa(ply+i*ph,buf,10);
		TMP.Replace0("<YCOOR>",buf);
		
		CNation* PNat=Nat+CurColor;
		int nupg=0;
		for(int u=0;u<PNat->NUpgCheck;u++)if(PNat->UpgCheck[u])nupg++;
		sprintf(buf,"%s (%d)",Info[i].Name,nupg);
		TMP.Replace0("<PNAME>",buf);

		if(CurColor==NI){
			TMP.Replace0("<PSTATUS>","{<PSTATUS>}");
			TMP.Replace0("<TIMERVAR>",TimerVarName);			
		}else{
			TMP.Replace0("<TIMERVAR>","");
		}
		TMP.Replace0("<READYHINT>",GetTextByID("BTL_Ready_Hint"));

		if(Info[i].Ready) sprintf(buf,"|Battle\\PlayerList2.gp,2,1,9,9|");
			else sprintf(buf,"|Battle\\PlayerList2.gp,0,3,8,8|");
		TMP.Replace0("<PSTATUS>",buf);
		
		CML+=TMP;
	}

	DString	TXT;	TXT.ReadFromFile("Battle\\TextLine.cml"); if(!TXT.pchar())return;
	DString	CTXT;	CTXT.ReadFromFile("Battle\\CTextLine.cml"); if(!CTXT.pchar())return;

	// character info
	
	{
		UnitType* UT=PInfo->UT+PInfo->SelUT;
		bool can=DetCannon(UT->NIndex);

		int y0=can?10:0;
		int x0=80+20;
		int x=can?70:0;
		int y=can?110:85;
		int xx=60;	// смещение цифры от иконки
		int nx=3;
		int dy=20;	// высота строчки

		DString VAL;	VAL.ReadFromFile("Battle\\Weapon.cml");

		int n=0;
		char buf[128];
		for(int i=0;i<UT->NAttTypes;i++){
			short md=UT->CurDamage[i];			
			if(UT->MC.MaxDamage[i]&&UT->MC.AttackRadius2[i]){
				byte wk=UT->MC.WeaponKind[i];				
				
				// damage
				DString TMP(VAL.pchar());

				sprintf(buf,"_WD_%d",wk);
				TMP.Replace0("<ID>",buf);
				itoa(GetWeaponIcon(wk),buf,10);
				TMP.Replace0("<Icon>",buf);
				itoa(md,buf,10);
				TMP.Replace0("<Dam>",buf);
				
				itoa(x0+x+n*xx,buf,10);
				TMP.Replace0("<XCOOR>",buf);
				itoa(y0,buf,10);
				TMP.Replace0("<YCOOR>",buf);

				CML+=TMP;

				// range
				TMP=VAL.pchar();

				sprintf(buf,"_WR_%d",wk);
				TMP.Replace0("<ID>",buf);
				itoa(24,buf,10);
				TMP.Replace0("<Icon>",buf);
				itoa(UT->CurR2[i],buf,10);
				TMP.Replace0("<Dam>",buf);
				
				itoa(x0+x+n*xx,buf,10);
				TMP.Replace0("<XCOOR>",buf);
				itoa(y0+dy,buf,10);
				TMP.Replace0("<YCOOR>",buf);
				
				CML+=TMP;

				// delay
				TMP=VAL.pchar();

				sprintf(buf,"_WD_%d",wk);
				TMP.Replace0("<ID>",buf);
				itoa(23,buf,10);
				TMP.Replace0("<Icon>",buf);
				itoa(UT->CurAPause[i],buf,10);
				TMP.Replace0("<Dam>",buf);
				
				itoa(x0+x+n*xx,buf,10);
				TMP.Replace0("<XCOOR>",buf);
				itoa(y0+2*dy,buf,10);
				TMP.Replace0("<YCOOR>",buf);
				
				CML+=TMP;

				n++;
				if(n>=3){
					n=0;
					y0+=3*dy;
				}				
				
			}
		}
			
		if(n){
			n=0;
			y0+=dy;
		}
		
		y0=dy*3+2;
		

		word sh=UT->CurShield;
		//if(sh)
		{
			DString TMP(VAL.pchar());

			TMP.Replace0("<ID>","_AL_");
			itoa(13,buf,10);
			TMP.Replace0("<Icon>",buf);
			itoa(sh,buf,10);
			TMP.Replace0("<Dam>",buf);
			
			itoa(x0+x+n*xx,buf,10);
			TMP.Replace0("<XCOOR>",buf);
			itoa(y0,buf,10);
			TMP.Replace0("<YCOOR>",buf);

			n++;

			CML+=TMP;
		}	
		
		// Life
		DString TMP=CTXT.pchar();				
		TMP.Replace0("<BOX>","%IBX");
		TMP.Replace0("<XCOOR>","45");
		itoa(y-5,buf,10);
		TMP.Replace0("<YCOOR>",buf);
		itoa(UT->CurLife,buf,10);
		TMP.Replace0("<Text>",buf);
		CML+=TMP;

		y+=dy;//+10;

		x=3;

		// Protection
		int nn=0;
		int yp=y;
		n=0;
		for(i=0;i<UT->NAttTypes;i++){
			byte pr=UT->CurProtect[i];
			if(pr){
				byte wk=i;//UT->WeaponKind[i];				
				
				DString TMP(VAL.pchar());

				sprintf(buf,"_PR_%d",wk);
				TMP.Replace0("<ID>",buf);
				itoa(GetProtectIcon(wk),buf,10);
				TMP.Replace0("<Icon>",buf);
				itoa(pr+sh,buf,10);
				TMP.Replace0("<Dam>",buf);
				
				itoa(x+(nn>>1)*xx,buf,10);
				TMP.Replace0("<XCOOR>",buf);
				itoa(yp+(nn&1)*dy,buf,10);
				TMP.Replace0("<YCOOR>",buf);
				nn++;

				n++;
				if(n>=3){
					n=0;
					y+=dy;
				}
				
				CML+=TMP;
			}
		}
		
	}

	// upgrades
	int w=42;
	int h=42;
	int x=0;
	int y=0;
	int yt=0;	// coor of unit type icon
	int nx=5;
	
	DString UTP;	UTP.ReadFromFile("Battle\\UnitType.cml"); if(!UTP.pchar())return;
	DString UTA;	UTA.ReadFromFile("Battle\\untypean.cml"); if(!UTA.pchar())return;
	DString UTS;	UTS.ReadFromFile("Battle\\SelPan.cml"); if(!UTS.pchar())return;	
	DString UPS;	UPS.ReadFromFile("Battle\\Upgrade.cml"); if(!UPS.pchar())return;
	DString CHK;	CHK.ReadFromFile("Battle\\Check.cml"); if(!CHK.pchar())return;	

	for(int t=0;t<PInfo->NUT;t++){
		UnitType* UT=PInfo->UT+t;

		word NIndex=UT->NIndex;
		DString TMP;
		DString TM2;
		
		{
			TMP=UTP.pchar();
			itoa(t,buf,10); TMP.Replace0("<ID>",buf);
			itoa(yt,buf,10); TMP.Replace0("<YCOOR>",buf);
			char* Mess=GetUnitMessage(NIndex);
			if(Mess){
				TMP.Replace0("<UTInfo>",Mess);
				TMP.Replace0("<HINT>",Mess);
			}
			itoa(GetUnitIcon(NIndex),buf,10); TMP.Replace0("<UTIcon>",buf);
			CML+=TMP;		
		}
		if(t==PInfo->SelUT){
			TMP=UTS.pchar();
			itoa(yt,buf,10); TMP.Replace0("<YCOOR>",buf);
			CML+=TMP;

			TMP=UTA.pchar();
			itoa(t,buf,10); TMP.Replace0("<ID>",buf);
			char* Mess=GetUnitMessage(NIndex);
			if(Mess) TMP.Replace0("<HINT>",Mess);

			int xc,yc;
			char* file=GetUnitGPName(NIndex,xc,yc);
			if(file){
				bool can=DetCannon(UT->NIndex);
				sprintf(buf,"%s",file); TMP.Replace0("<FILE>",buf);
				itoa(xc+35,buf,10); TMP.Replace0("<X>",buf);
				itoa(can?yc+93:yc+70,buf,10); TMP.Replace0("<Y>",buf);						
				itoa(can?120:80,buf,10); TMP.Replace0("<W>",buf);
			}

			itoa(2,buf,10); TMP.Replace0("<STR>",buf);
			itoa(2,buf,10); TMP.Replace0("<END>",buf);
			itoa(2,buf,10); TMP.Replace0("<ASTR>",buf);
			itoa(2,buf,10); TMP.Replace0("<AEND>",buf);
			CML+=TMP;		
			//yt+=h;
		}
		
		yt+=h;

		y=0;

		/*
		TMP=TXT.pchar();
		TMP.Replace0("<XCOOR>","0");
		TMP.Replace0("<YCOOR>","0");
		TMP.Replace0("<Text>","Upgrades");
		CML+=TMP;
		y+=20;
		*/
		
		/*
		int NUpg=0;
		while(NUpg<UT->NUpg&&t==PInfo->SelUT){
			for(int n=0;n<nx&&NUpg<UT->NUpg;n++){
				word UIndex=UT->Upg[NUpg];
				
				bool check=PNat->UpgCheck[UIndex];

				TMP=UPS.pchar();
				if(check){					
					TMP+=CHK;
					TMP.Replace0("<CHECK>","1");					
				}else{
					TMP.Replace0("<CHECK>","0");
				}
				
				itoa(t,buf,10);
				TMP.Replace0("<TYPE>",buf);
				itoa(NUpg,buf,10);
				TMP.Replace0("<UID>",buf);
				itoa(UIndex,buf,10);
				TMP.Replace0("<GUID>",buf);
				
				GetUpgHint(NI,UIndex,buf);				
				TMP.Replace0("<HINT>",buf);
				
				itoa(x+n*w,buf,10);
				TMP.Replace0("<XCOOR>",buf);
				itoa(y,buf,10);
				TMP.Replace0("<YCOOR>",buf);

				itoa(GetUpgradeIcon(UIndex),buf,10);
				TMP.Replace0("<UIcon>",buf);
								
				buf[0]=GetUpgLevel(UIndex);
				buf[1]=0;
				TMP.Replace0("<ULEVEL>",buf);

				CML+=TMP;
				NUpg++;
			}
			y+=h;
		}		
		y+=5;
		*/
		
		int n=0;
		if(t==PInfo->SelUT){
			vector<CUpgCategory>::iterator ctg=UT->Category.begin();
			vector<CUpgCategory>::iterator ctgEnd=UT->Category.end();
			for(;ctg!=ctgEnd;ctg++) if(ctg->Linked.size()+ctg->UnLinked.size()){				
				
				if(n!=0){
					y+=h;
					n=0;
				}
								
				TMP=TXT.pchar();				
				TMP.Replace0("<BOX>","%EBX");
				TMP.Replace0("<XCOOR>","0");
				itoa(y,buf,10);
				TMP.Replace0("<YCOOR>",buf);
				TMP.Replace0("<Text>",ctg->Message);
				CML+=TMP;
				y+=20;

				vector<VWord>::iterator lin=ctg->Linked.begin();
				vector<VWord>::iterator linEnd=ctg->Linked.end();
				while(lin!=linEnd){
					//VWord curlin=*lin;

					VWord::iterator u=lin->begin();
					VWord::iterator uEnd=lin->end();
					word u0=0xFFFF, u1=0xFFFF;
					while(u!=uEnd/*&&u0==0xFFFF&&u1==0xFFFF*/){
						VWord::iterator next=u+1;
						if(!PNat->UpgCheck[*u]||next==uEnd){
							u0=*u;
							break;
						}else
						if(!PNat->UpgCheck[*next]){
							u0=*next;
							u1=*u;
							break;
						}/*else
						if(next==uEnd){
							u0=*u;
							break;
						}*/
						u++;
					}

					if(u0!=0xFFFF){
						AddUpgToCml(CML, UPS.pchar(), u0, 0, x+n*w, y);
						if(++n>=nx){
							y+=h;
							n=0;
						}
					}
					if(u1!=0xFFFF){
						AddUpgToCml(CML, UPS.pchar(), u1, 0, x+n*w, y);
						if(++n>=nx){
							y+=h;
							n=0;
						}
					}

					lin++;
				}				
				
				VWord::iterator unl=ctg->UnLinked.begin();
				VWord::iterator unlEnd=ctg->UnLinked.end();
				while(unl!=unlEnd){
					word UIndex=*unl;

					AddUpgToCml(CML, UPS.pchar(), UIndex, 0, x+n*w, y);

					if(++n>=nx){
						y+=h;
						n=0;
					}
					unl++;
				}
				//ctg++;
			}
		}

	}
	CML+=FRM;
	CML.WriteToFile("Battle\\tmp.cml");
	ActivateDipDialog("LW_new&|LW_file&Battle/tmp.cml");
};

void CBattle::AddUpgToCml(DString& CML, char* ups, word UIndex, word NUpg, int x, int y){
	byte NI=GetMyPlayerColor();
	CNation* PNat=Nat+NI;
	CPlayerInfo* PInfo=Info+GetMyPlayerID();

	bool check=PNat->UpgCheck[UIndex];

	DString TMP=ups;
	if(check){					
		//TMP+=CHK;
		TMP.Replace0("<CHECK>","1");					
	}else{
		TMP.Replace0("<CHECK>","0");
	}
	
	char buf[256];
	itoa(PInfo->SelUT,buf,10);
	TMP.Replace0("<TYPE>",buf);
	itoa(UIndex/*NUpg*/,buf,10);
	TMP.Replace0("<UID>",buf);
	itoa(UIndex,buf,10);
	TMP.Replace0("<GUID>",buf);
	
	GetUpgHint(NI,UIndex,buf);				
	TMP.Replace0("<HINT>",buf);
	
	itoa(x,buf,10);
	TMP.Replace0("<XCOOR>",buf);
	itoa(y,buf,10);
	TMP.Replace0("<YCOOR>",buf);

	itoa(GetUpgradeIcon(UIndex),buf,10);
	TMP.Replace0("<UIcon>",buf);
					
	buf[0]=GetUpgLevel(UIndex);
	buf[1]=0;
	TMP.Replace0("<ULEVEL>",buf);

	CML+=TMP;
};

//
// CPlayerInfo
//
void CPlayerInfo::AddNewMon(byte NI, word NIndex){
	if(DetCannon(NIndex) || DetSheep(NIndex)) return;

	UT=(UnitType*)realloc(UT,sizeof(UnitType)*(NUT+1));
	UnitType* NewUT=UT+NUT;
	NewUT->NIndex=NIndex;

	// создать список апгрейдов
	const MaxUpg=50;
	word Upg[MaxUpg];
	//memset(Upg,0xFF,sizeof(Upg));
	int NUpg=0;

	GetListOfUpgradesForMon_Old(NI,NIndex,Upg,MaxUpg,NUpg);
	
	bool* UpgEn=PScript[NI].UpgEnabled;
	for(int i=0;i<NUpg;i++) if(!UpgEn[Upg[i]]){
		Upg[i]=0xFFFF;
	}
	
	NewUT->NUpg=NUpg;
	int size=NUpg<<1;
	NewUT->Upg=(word*)malloc(size);
	memcpy(NewUT->Upg,Upg,size);

	// создаем категории
	//VWord UList;
	//UList.reserve(NewUT->NUpg);
	//for(int i=0;i<NewUT->NUpg;i++) UList[i]=Upg[i];	
	
	NewUT->AddUpgCategory(12,GetTextByID("BTL_Attack"),Upg,NUpg);
	NewUT->AddUpgCategory(2,GetTextByID("BTL_Defence"),Upg,NUpg);
	//NewUT->AddUpgCategory(1,"Shield",Upg,NUpg);
	NewUT->AddUpgCategory(0xFFFF,GetTextByID("BTL_Others"),Upg,NUpg);

	// заполняем характеристики
	NewUT->GetCharacter(NI);

	NUT++;	
};

void CPlayerInfo::CheckUpg(word UIndex, bool OldState){
	byte NI=GetMyPlayerColor();
	NewUpgrade* NU=GetNewUpg(NI,UIndex);
	if(NU){
		// определяем оператор апгрейда
		TDoUpgOperator Operator=NULL;
		switch(NU->CtgUpgrade){
		case 1:
			Operator=DoShield;
			break;
		case 2:
			Operator=DoProtect;
			break;
		case 3:
			Operator=DoAttPause;
			break;
		case 12:
			Operator=DoDamage;
			break;
		};
		// вызов оператора
		if(Operator){
			// определяем знак операции
			byte Sign=NU->ValueType&3;
			byte Perc=NU->ValueType&4;
			if(OldState==true){
				if(Sign){
					if(Sign==1) Sign=2;
						else Sign=1;
				}
			}
			// обход типов юнитов			
			for(int t=0;t<NUT;t++){
				UnitType* ut=UT+t;
				// действует ли апгрейд на этот тип
				bool doit=false;
				if(NU->UnitType==0){
					if(NU->UnitGroup){
						for(int i=0;i<NU->NUnits;i++){
							if(NU->UnitGroup[i]==ut->NIndex){
								doit=true;
								break;
							}
						}
					}else{
						if(NU->UnitValue==ut->NIndex){
							doit=true;
						}
					}
				}
				// поиск категории (тип вооружения)
				if(doit){
					int w;
					switch(NU->CtgType){
					case 0:
						Operator(ut,-1,Sign,Perc,NU->Value);
						break;
					case 1:
						if(NU->CtgGroup){
							for(w=0;w<NU->NCtg;w++){
								for(int i=0;i<ut->NAttTypes;i++){									
									if(ut->MC.WeaponKind[i]==NU->CtgGroup[w]) Operator(ut,i,Sign,Perc,NU->Value);
								}
							}
						}else{
							for(int i=0;i<ut->NAttTypes;i++){									
								if(ut->MC.WeaponKind[i]==NU->CtgValue&&ut->MC.MaxDamage[i]) Operator(ut,i,Sign,Perc,NU->Value);
							}							
						}
						break;
					case 2:
					case 4:
						if(NU->CtgGroup){
							for(w=0;w<NU->NCtg;w++){
								Operator(ut,NU->CtgGroup[w],Sign,Perc,NU->Value);
							}
						}else{
							Operator(ut,NU->CtgValue,Sign,Perc,NU->Value);
						}
						break;
					case 3:
						for(w=0;w<ut->NAttTypes;w++){									
							if(ut->MC.MaxDamage[w]) Operator(ut,w,Sign,Perc,NU->Value);
						}
						break;
					};
				}
			}
		}		
	}
};

void DoDamage(UnitType* UT, short AttIndex, byte Sign, byte Perc, int Value){
	if(Sign){
		if(Perc){
			if(Sign==1) Value=UT->CurDamage[AttIndex]*Value/100;
				else Value=UT->CurDamage[AttIndex]-UT->CurDamage[AttIndex]*100/(Value+100);
		}
		if(Sign==1) UT->CurDamage[AttIndex]+=Value;
			else UT->CurDamage[AttIndex]-=Value;
	}
};
void DoProtect(UnitType* UT, short AttIndex, byte Sign, byte Perc, int Value){
	if(Sign&&!Perc){
		if(Sign==1) UT->CurProtect[AttIndex]+=Value;
			else UT->CurProtect[AttIndex]-=Value;
	}
};
void DoShield(UnitType* UT, short AttIndex, byte Sign, byte Perc, int Value){
	if(Sign&&!Perc){
		if(Sign==1) UT->CurShield+=Value;
			else UT->CurShield-=Value;
	}
};
void DoAttPause(UnitType* UT, short AttIndex, byte Sign, byte Perc, int Value){
	if(Sign){
		if(Perc){
			if(Sign==1) Value=UT->CurAPause[AttIndex]*100/(100-Value)-UT->CurAPause[AttIndex];
				else Value=UT->CurAPause[AttIndex]*Value/100;
		}
		if(Sign==1) UT->CurAPause[AttIndex]+=Value;
			else UT->CurAPause[AttIndex]-=Value;
	}
};


//
// UnitType
//
void UnitType::GetCharacter(byte NI){	
	NAttTypes=GetMainChar(NI, NIndex, &MC);

	if(NAttTypes){
		int i;
		CurDamage.reserve(NAttTypes); for(i=0;i<NAttTypes;i++) CurDamage[i]=MC.MaxDamage[i];
		CurProtect.reserve(NAttTypes); for(i=0;i<NAttTypes;i++) CurProtect[i]=MC.Protection[i];
		CurShield=MC.Shield;
		CurR2.reserve(NAttTypes); for(i=0;i<NAttTypes;i++) CurR2[i]=MC.AttackRadius2[i];
		CurAPause.reserve(NAttTypes); for(i=0;i<NAttTypes;i++){
			int p=GetAttackNFrame(NIndex,i);
			if(/*i!=0&&*/(p<MC.AttackPause[i]||p<0)) p=MC.AttackPause[i];
			CurAPause[i]=p;
		}
		CurLife=MC.Life;
	}
};

void UnitType::AddUpgCategory(word UpgCtg, char* Mess, word* UList, int NUList){
	Category.push_back(	CUpgCategory( UpgCtg, Mess, UList, NUList ) );
};

void UnitType::RunProtectUpg(byte Nat, int Level){
	vector<CUpgCategory>::iterator UC=Category.begin();
	vector<CUpgCategory>::iterator UCE=Category.end();
	for(;UC!=UCE;UC++) if(UC->Index==2){
		VWord::iterator lnk=UC->Linked[0].begin();
		VWord::iterator lnkEnd=UC->Linked[0].end();
		for(;lnk!=lnkEnd;lnk++){
			if(Level<=0) return;
			GAMEOBJ UO;
			UO.Type='UPGR';
			UO.Index=*lnk;
			InitialUpgradeNew(Nat,&UO,0xFFFF);
			Level--;
		}
	}
};

CEXPORT void RunProtectUpg(byte NI, int Level){
	// создать список апгрейдов для каждого монстра	
	int NMon=GetNMon(NI);
	int* NIndex=(int*)malloc(NMon*sizeof(int));
	memset(NIndex,0,NMon*sizeof(int));			
				
	GetPressentMonsters(NI,NIndex);
	
	for(int i=0;i<NMon;i++)if(NIndex[i]){
		// создать список апгрейдов
		const MaxUpg=50;
		word Upg[MaxUpg];
		int NUpg=0;

		int l=Level;

		GetListOfUpgradesForMon_Old(NI,i,Upg,MaxUpg,NUpg);

		for(int j=0;j<NUpg;j++){
			NewUpgrade* UP=GetNewUpg(NI,Upg[j]);
			if(UP&&(UP->CtgUpgrade==1||UP->CtgUpgrade==2)){
				if(l<=0) break;
				GAMEOBJ UO;
				UO.Type='UPGR';
				UO.Index=Upg[j];
				InitialUpgradeNew(NI,&UO,0xFFFF);
				l--;
			}
		}
		
	}

	free(NIndex);

}

//
// CUpgCategory
//
CUpgCategory::CUpgCategory( word _Index, char* _Message, word* UList, int NUList){
	Index=_Index;
	Message=_Message;

	VWord Link;
	for(int i=0;i<NUList;i++){
		word NUID=UList[i];
		if(NUID!=0xFFFF){
			Link.clear();		
			NewUpgrade* NU=GetNewUpg(0,NUID);
			if(NU&&(NU->CtgUpgrade==Index||Index==0xFFFF)){				
				Link.push_back(NUID);
				UList[i]=0xFFFF;
				// добавляем хобот
				for(int j=i+1;j<NUList;j++)if(UList[j]!=0xFFFF){
					NU=GetNewUpg(0,UList[j]);
					if((NU->CtgUpgrade==Index||Index==0xFFFF)&&NU->NAutoEnable&&NU->AutoEnable[0]==NUID){
						NUID=UList[j];
						Link.push_back(NUID);
						UList[j]=0xFFFF;
					}
				}
				// разворачиваем хобот
				Linked.push_back(VWord());
				vector<VWord>::iterator NewLink=Linked.end()-1;				
				
				while(!Link.empty()){
					NewLink->push_back(Link.back());
					Link.pop_back();
				}

				// добавляем хвост
				NUID=NewLink->back();
				for(j=i+1;j<NUList;j++)if(UList[j]!=0xFFFF){
					NU=GetNewUpg(0,NUID);
					if((NU->CtgUpgrade==Index||Index==0xFFFF)&&NU->NAutoEnable&&NU->AutoEnable[0]==UList[j]){
						NUID=UList[j];
						NewLink->push_back(NUID);
						UList[j]=0xFFFF;
					}
				}

				if(NewLink->size()==1){
					UnLinked.push_back(NewLink->front());
					Linked.erase(NewLink);
				}
				
			}
		}
	}
};

CUpgCategory::~CUpgCategory(){
	//if(Message) free(Message);
}

/*
void CUpgCategory::Init(){
	memset(this,0,sizeof(*this));
	Index=0xFFFF;	
}
void CUpgCategory::Free(){
	if(Message) free(Message);
}
*/

//
// CWeapInfo
//
CWeapInfo::CWeapInfo(){
	memset(this,0,sizeof(*this));
	Kind=0xFF;
}
