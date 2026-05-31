void ReverseLMode();

CEXPORT
void SetObservMode(){
	NOPAUSE=0;
	LockPause=1;
	FogMode=0;
	if(!LMode) ReverseLMode();
}

CEXPORT
void SetNormalMode(){
	NOPAUSE=1;
	LockPause=0;
	FogMode=1;
}

CEXPORT void ShowFogOfWar(){ FogMode=1; }
CEXPORT void HideFogOfWar(){ FogMode=0; }

CEXPORT
int GetNMon(byte NI){
	if(NI<8) return NATIONS[NI].NMon;
		else return 0;
}

CEXPORT
void GetPressentMonsters(byte NI, int* NIndex){
	byte MASK=NATIONS[NI].NMask;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Sdoxlo||OB->NewBuilding)&&OB->NMask==MASK){
			NIndex[OB->NIndex]+=OB->newMons->Force+1;
		}
	}
};

CEXPORT
void GetListOfUpgradesForMon_Old(byte NI,word NIndex,word* Upg,int MaxUpg,int& NUpg){
	NUpg=0;
	if(NI>7) return;

	NewUpgrade** UPG=NATIONS[NI].UPGRADE;
	int NUPG=NATIONS[NI].NUpgrades;

	GeneralObject* GO=NATIONS[NI].Mon[NIndex];
	NewMonster* NM=GO->newMons;

	//ATTACK - search for upgrades		
	for(int i=0;i<NUPG&&NUpg<MaxUpg;i++){
		NewUpgrade* NU=UPG[i];
		if(NU->CtgUpgrade==12&&NU->UnitGroup==NULL&&NU->UnitType==0&&NU->UnitValue==NIndex&&NU->CtgGroup==NULL&&NU->CtgType==1){//Upgrade on attack
			NU->NatID+=32;
			Upg[NUpg]=i;
			NUpg++;
		};
	};

	//SHIELD - search for upgrades
	for(int i=0;i<NUPG&&NUpg<MaxUpg;i++){
		NewUpgrade* NU=UPG[i];
		if(NU->CtgUpgrade==2&&NU->UnitGroup==NULL&&NU->UnitType==0&&NU->UnitValue==NIndex){//Upgrade on protection
			NU->NatID+=32;
			Upg[NUpg]=i;
			NUpg++;
		};
	};

	//Other upgrades
	for(int i=0;i<NUPG;i++){
		NewUpgrade* NU=UPG[i];
		bool add=false;
		if(NU->NatID<32){
			if(GO->NatID==NU->NatID&&strcmp(NU->Name,NU->Message)){
				int UCTG=NU->CtgUpgrade;
				int ICO=NU->IconSpriteID;
				if(UCTG!=5&&UCTG!=10&&(UCTG<18)&&UCTG!=11&&UCTG!=14&&UCTG!=15&&ICO!=91/* || NM->Peasant*//*&&UCTG>=19&&UCTG<=21*/){
					if(NU->UnitGroup){
						for(int p=0;p<NU->NUnits&&!add;p++)if(NU->UnitGroup[p]==NIndex)add=true;
					}else if(NU->UnitValue==NIndex)add=true;
				};
				if(NM->Artilery&&(UCTG==5||UCTG==10))add=false;
				if(NM->Peasant&&UCTG>=19&&UCTG<=21&&NU->NatID==GO->NatID)add=true;
			};
		}else{
			NU->NatID-=32;
		};
		if(add&&NUpg<MaxUpg){
			Upg[NUpg]=i;
			NUpg++;
		};
	}
};

CEXPORT
void GetListOfUpgradesForMon(byte NI,word NIndex,word* Upg,int MaxUpg,int& NUpg){
	NUpg=0;
	if(NI>7) return;

	NewUpgrade** UPG=NATIONS[NI].UPGRADE;
	int NUPG=NATIONS[NI].NUpgrades;

	GeneralObject* GO=NATIONS[NI].Mon[NIndex];
	NewMonster* NM=GO->newMons;

	for(int i=0;i<NUPG;i++){
		NewUpgrade* NU=UPG[i];
		bool add=false;
		if(strcmp(NU->Name,NU->Message)){
			int UCTG=NU->CtgUpgrade;
			int ICO=NU->IconSpriteID;
			if(/*UCTG!=5&&UCTG!=10&&(UCTG<18)&&UCTG!=11&&UCTG!=14&&UCTG!=15&&*/ICO!=91/* || NM->Peasant*//*&&UCTG>=19&&UCTG<=21*/){
				if(NU->UnitGroup){
					for(int p=0;p<NU->NUnits&&!add;p++)if(NU->UnitGroup[p]==NIndex)add=true;
				}else if(NU->UnitValue==NIndex)add=true;
			};
			//if(NM->Artilery&&(UCTG==5||UCTG==10))add=false;
			//if(NM->Peasant&&UCTG>=19&&UCTG<=21&&NU->NatID==GO->NatID)add=true;
		};
		if(add&&NUpg<MaxUpg){
			Upg[NUpg]=i;
			NUpg++;
		};
	}
}

CEXPORT
int GetNUpgrades(byte NI){
	if(NI<8) return NATIONS[NI].NUpgrades;
		else return 0;
}

CEXPORT
char* GetUnitMessage(word NIndex){
	if(NIndex<NATIONS->NMon) return NATIONS->Mon[NIndex]->newMons->Message;
	return NULL;
}

CEXPORT
char* GetUnitGPName(word NIndex, int& x, int& y){
	if(NIndex<NATIONS->NMon){
		NewMonster* NM=NATIONS->Mon[NIndex]->newMons;
		NewAnimation* AN=NM->GetAnimation(anm_Attack);
		if(!AN->Enabled) AN=NM->GetAnimation(anm_Stand);
		if(AN->Enabled){
			NewFrame* NF=AN->Frames[0];
			x=NF->dx;//+NM->RectDx+NM->RectLx;
			y=NF->dy;//+NM->RectDy+NM->RectLy+(NM->RectLy>>1);
			return (char*)GPS.GetGPName(AN->Frames[0]->FileID);
		}		
	}
	return NULL;
}

CEXPORT
int GetAttackNFrame(word NIndex, word AttIndex){
	if(NIndex<NATIONS->NMon&&AttIndex<NAttTypes){
		NewAnimation* AN=NATIONS->Mon[NIndex]->newMons->GetFirstAnimation(anm_Attack+AttIndex);
		if(AN->Enabled){
			return AN->NFrames;
		}
	}
	return 0;
}

CEXPORT
int GetUpgradeIcon(word UIndex){
	if(UIndex<NATIONS->NUpgrades) return NATIONS->UPGRADE[UIndex]->IconSpriteID;
		return -1;
}

CEXPORT
int GetUpgLevel(word UIndex){
	if(UIndex<NATIONS->NUpgrades) return NATIONS->UPGRADE[UIndex]->Level;
		return 0;		
}

CEXPORT
char* GetUpgMessage(word UIndex){
	if(UIndex<NATIONS->NUpgrades) return NATIONS->UPGRADE[UIndex]->Name;
		return NULL;		
}

CEXPORT
void SetUpgEnableStatus(byte NI, word UIndex, bool Status){
	if(UIndex<NATIONS[NI].NUpgrades&&NI<8){
		NATIONS[NI].UPGRADE[UIndex]->Enabled=Status;
	}
}

CEXPORT
void GetUpgCost(byte NI, word UIndex, word* &Cost){
	Cost=NULL;
	if(NI>7||UIndex>=NATIONS[NI].NUpgrades) return;
	NewUpgrade* NU=NATIONS[NI].UPGRADE[UIndex];
	Cost=NU->Cost;
}

void GetCostString(word* Cost,char* str);
void GetCostString(int* Cost,char* str);

CEXPORT
void GetCostStringExp(word* Cost,char* str){
	GetCostString(Cost,str);
};
CEXPORT
void GetCostStringExp(int* Cost,char* str){
	GetCostString(Cost,str);
};

CEXPORT
bool GetUpgHint(byte NI, word UIndex, char* Str){
	if(UIndex>=NATIONS->NUpgrades) return false;
	NewUpgrade* NU=NATIONS[NI].UPGRADE[UIndex];
	char* hnt=Str;
	sprintf(hnt,"%s/",NU->Message);
	hnt+=strlen(hnt);
	strcpy(hnt,GetTextByID("UI_Cost"));
	hnt+=strlen(hnt);
	strcpy(hnt,"  ");
	hnt+=strlen(hnt);
	GetCostString(NU->Cost,hnt);
	return true;
}

extern ResDiscr RDS[8];
CEXPORT char* GetResourceName(byte ResID){
	if(ResID>7) return "";
	return RDS[ResID].Name;
}

extern int Inform;
extern bool DIP_DSS_Vis;

CEXPORT
void CloseDipDialog(){
	Inform=0;
	DIP_DSS_Vis=0;
};

CEXPORT char* GetPlayerName(byte NI){	
	if(NI<7){
		byte Nat=MyNation;
		byte RefNat=GSets.CGame.cgi_NatRefTBL[Nat];
		if(NI==RefNat){
			PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
			if(I){
				return I->name;
			}
		}else{
			for(int i=0;i<7;i++){
				Nat=GSets.CGame.PL_INFO[i].ColorID;
				RefNat=GSets.CGame.cgi_NatRefTBL[Nat];
				if(NI==RefNat){
					return GSets.CGame.PL_INFO[i].name;
				}
			}
		}		
	}
	return NULL;
}

CEXPORT
byte GetPlayerColor(word ID){
	if(ID>7) return 255;
		else return GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[ID].ColorID];
}

CEXPORT
byte GetMyPlayerColor(){
	return GSets.CGame.cgi_NatRefTBL[GetMyNation()];
}

CEXPORT
byte GetMyPlayerID(){
	byte mn=GetMyNation();
	for(int i=0;i<8;i++)if(GSets.CGame.PL_INFO[i].ColorID==mn) return i;
	return 255;
}

CEXPORT
byte* GetNatRefTBL(){
	return GSets.CGame.cgi_NatRefTBL;

}

struct SMainChar{
	short* MaxDamage;
	byte* WeaponKind;
	byte* Protection;
	word Shield;
	word* AttackRadius2;
	word* AttackPause;
	word Life;
};

CEXPORT
int GetMainChar(byte NI, word NIndex, SMainChar* MC){
	if(NIndex>=NATIONS->NMon||!MC) return 0;

	//byte NI=0;
	NewUpgrade** UPG=NATIONS[NI].UPGRADE;
	int NUPG=NATIONS[NI].NUpgrades;

	GeneralObject* GO=NATIONS[NI].Mon[NIndex];
	NewMonster* NM=GO->newMons;
	AdvCharacter* ADV=GO->MoreCharacter;

	MC->MaxDamage=ADV->MaxDamage;
	MC->WeaponKind=ADV->WeaponKind;
	MC->Protection=ADV->Protection;
	MC->Shield=ADV->Shield;
	MC->AttackRadius2=ADV->AttackRadius2;
	MC->AttackPause	=ADV->AttackPause;
	MC->Life=NM->Life;
	
	return NAttTypes;
}

extern short WeaponIcn [32];
extern short ProtectIcn[32];

CEXPORT
word GetWeaponIcon(word WeaponKind){
	if(WeaponKind>=32) return 0xFFFF;
		else return WeaponIcn[WeaponKind];
}

CEXPORT
word GetProtectIcon(word WeaponKind){
	if(WeaponKind>=32) return 0xFFFF;
		else return ProtectIcn[WeaponKind];
}


CEXPORT
NewUpgrade* GetNewUpg(byte NI, word UIndex){
	if(NI>7||UIndex>=NATIONS[NI].NUpgrades) return NULL;
		else return NATIONS[NI].UPGRADE[UIndex];	
}

//extern char GSets.CGame.cgi_CurrentMap[64];

CEXPORT
char* GetCurMissionMapName(){
	if(GSets.CGame.cgi_CurrentMap[0]) return GSets.CGame.cgi_CurrentMap;
	/*
	int CM=MISSLIST.CurrentMission;
	if(CM>=0&&CM<MISSLIST.NMiss){
		return MISSLIST.MISS[CM].MapName;
	}*/
	return NULL;
}

CEXPORT
Brigade* GetBrigade(byte NI, word BrigID){
	if(NI>7||BrigID>=MaxBrig) return NULL;
	return CITY[NI].Brigs+BrigID;
}