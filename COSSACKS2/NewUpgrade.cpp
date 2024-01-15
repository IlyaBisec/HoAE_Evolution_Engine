#include "stdheader.h"
#include "StringHash.h"
#include "unitability.h"

void NLine(GFILE* f);
char* GetTextByID(char* ID);
int GetUnitKind(char* Name);
//UPGRADE_NAME Icon_ID Level Message Cost 
//      CATEGORY UNIT VALUE
//-------------------------------
//Level: 1..9
//Message:StringValue||NONE
//Cost: NResources Resoirce1_ID Resource1_Amount ... ResoirceN_ID ResourceN_Amount
//-------------------------------
//CATEGORY:
//CATEGORY_NAME [WEAPONKIND,ATTINDEX,ALL]
//Example:
//DAMAGE ALL
//DAMAGE WEAPONKIND FIRE
//DAMAGE WEAPONKIND GRP 2 FIRE SWORD
//DAMAGE ATTINDEX 2
//DAMAGE ATTINDEX GRP 2 1 2
//-------------------------------
//Categiries:
//SPEED
//SHIELD
//PROTECTION  [WEAPONKIND||ALL]
//ATTPAUSE    [WEAPONKIND||ATTINDEX||ALL]
//WEAPON      [WEAPONKIND||ATTINDEX||ALL]
//BUILD
//LIFE
//PRECISE     [WEAPONKIND||ATTINDEX||ALL]
//ATTRANGE    [WEAPONKIND||ATTINDEX||ALL]
//INSIDE      
//COST
//ENABLE      [UNIT||UPGRADE||UNIT&UPGRADE]
//DAMAGE      [WEAPONKIND||ATTINDEX||ALL]
//CHANGEMD    [UNIT]
//--------------------------------
//UNIT:
//UNIT_NAME||UNITKIND KIND_ID||ALL
//Example:
//Grenader
//UNITKIND SimpleBuilding
//UNITKIND Tower
//UNITKIND Wall
//--------------------------------
//Value:
//ID||IntegerValue
//Example of IntegerValue:
//+10 -10 100 +10% -10% 110%
int GetIconByName(char* Name);
int GetWeaponType(char* Name);
char* Sect;
void ErrM(char* s);
void IncPar(char* name,int line,char* Sect);
void GetWeapons(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){

};
int GetNTUnit(Nation* NT,char* Name){
	int nn=NT->NMon;
	GeneralObject** GO=NT->Mon;
	for(int i=0;i<nn;i++){
		if(GO[i]->MonsterID && !strcmp(Name,GO[i]->MonsterID))return i;
	};
	return -1;
};
int GetNTUpgrade(Nation* NT,char* Name){
	int nn=NT->NUpgrades;
	for(int i=0;i<nn;i++){
		NewUpgrade* NU=NT->UPGRADE[i];
		if(NU&&!strcmp(Name,NU->Name))return i;
	};
	return -1;
};
int GetNTElement(Nation* NT,char* Name,bool UseUnits,bool UseUpgrades){
	int zz=-1;
	if(UseUnits)zz=GetNTUnit(NT,Name);
	if(zz==-1&&UseUpgrades){
		zz=GetNTUpgrade(NT,Name);
		if(zz>=0)zz+=8192;
	};
	return zz;
};
//CtgTypes:
//0-  no category
//1-  CtgValue=i if AttType[i]==CtgValue   //WEAPONKIND
//2-  ATTINDEX
//3-  0<=CtgValue<=NAttTypes
//4-  0..31
void GetWeaponKind(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){
	char gx[128];
	char gy[128];
	int z=Gscanf(f,"%s",gx);
	NU->CtgType=1;
	if(z!=1)IncPar(Name,Line,Sect);
	if(!strcmp(gx,"GRP")){
		int ngrp;
		z=Gscanf(f,"%d",&ngrp);
		if(z!=1){
			sprintf(gx,"%s,Line %d :%s:Invalid GRP directive format for the list of weapon.",Name,Line,Sect);
			ErrM(gx);
		};
		NU->CtgGroup=znew(word,ngrp);
		NU->NCtg=ngrp;
		for(int i=0;i<ngrp;i++){
			z=Gscanf(f,"%s",gx);
			if(z!=1)IncPar(Name,Line,Sect);
			int zz1=GetWeaponType(gx);
			if(zz1==-1){
				sprintf(gy,"%s,Line %d :%s: Unknown weapon type:%s",Name,Line,Sect,gx);
				ErrM(gy);
			};
			NU->CtgGroup[i]=zz1;
		};
	}else{
		int zz1=GetWeaponType(gx);
		if(zz1==-1){
			sprintf(gy,"%s,Line %d :%s: Unknown Monster ID:%s",Name,Line,Sect,gx);
			ErrM(gy);
		};
		NU->CtgValue=zz1;
		NU->CtgGroup=NULL;
	};
};
void GetAttIndex(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){
	char gx[128];
	int z=Gscanf(f,"%s",gx);
	int p1,p2;
	if(z!=1)IncPar(Name,Line,Sect);
	NU->CtgType=2;
	if(!strcmp(gx,"GRP")){
		z=Gscanf(f,"%d",&p1);
		if(z!=1)IncPar(Name,Line,Sect);
		NU->CtgGroup=znew(word,p1);
		NU->NCtg=p1;
		for(int i=0;i<p1;i++){
			z=Gscanf(f,"%d",&p2);
			if(z!=1)IncPar(Name,Line,Sect);
			NU->CtgGroup[i]=p2;
		};
	}else{
		if(!strcmp(gx,"ALL")){
			NU->CtgGroup=znew(word,NAttTypes);
			for(int i=0;i<NAttTypes;i++){
				NU->CtgGroup[i]=i;
			};
			NU->NCtg=NAttTypes;
		}else{
			z=sscanf(gx,"%d",&p1);
			if(z!=1)IncPar(Name,Line,Sect);
			NU->CtgValue=p1;
			NU->CtgGroup=NULL;
		};
	};
};
void GetCtgParam01(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){
	char gx[128];
	char gy[128];
	int z=Gscanf(f,"%s",gx);
	if(!strcmp(gx,"ATTINDEX"))GetAttIndex(f,NU,NT,Name,Line);
	else if(!strcmp(gx,"WEAPONKIND"))GetWeaponKind(f,NU,NT,Name,Line);
	else if(!strcmp(gx,"ALL")){
		NU->CtgType=3;
		NU->CtgGroup=NULL;
		NU->CtgValue=0;
	}else{
		sprintf(gy,"%s, Line %d :%s: ATTINDEX or WEAPONKIND or ALL expected instead of %s",Name,Line,Sect,gx);
		ErrM(gy);
	};
};
void GetUnits(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){
	char gx[128];
	char gy[128];
	int z=Gscanf(f,"%s",gx);
	if(z!=1)IncPar(Name,Line,Sect);
	if(!strcmp(gx,"GRP")){
		int ngrp;
		z=Gscanf(f,"%d",&ngrp);
		if(z!=1){
			sprintf(gx,"%s,Line %d :%s:Invalid GRP directive format for the list of units.",Name,Line,Sect);
			ErrM(gx);
		};
		NU->UnitGroup=znew(word,ngrp);
		NU->NUnits=ngrp;
		NU->UnitType=0;
		for(int i=0;i<ngrp;i++){
			z=Gscanf(f,"%s",gx);
			if(z!=1)IncPar(Name,Line,Sect);
			int zz1=GetNTUnit(NT,gx);
			if(zz1==-1){
				sprintf(gy,"%s,Line %d :%s: Unknown Monster ID:%s",Name,Line,Sect,gx);
				ErrM(gy);
			};
			NU->UnitGroup[i]=zz1;
		};
	}else
	if(!strcmp(gx,"UNITKIND")){
		z=Gscanf(f,"%s",gx);
		if(z!=1)IncPar(Name,Line,Sect);
		if(!strcmp(gx,"GRP")){
			int ngrp;
			z=Gscanf(f,"%d",&ngrp);
			if(z!=1){
				sprintf(gx,"%s,Line %d :%s:Invalid GRP directive format for UNITKIND.",Name,Line,Sect);
				ErrM(gx);
			};
			NU->UnitGroup=znew(word,ngrp);
			NU->NUnits=ngrp;
			NU->UnitType=1;
			for(int i=0;i<ngrp;i++){
				z=Gscanf(f,"%s",gx);
				if(z!=1)IncPar(Name,Line,Sect);
				int zz1=GetUnitKind(gx);
				if(zz1==-1){
					sprintf(gy,"%s,Line %d :%s: Unknown unit kind:%s",Name,Line,Sect,gx);
					ErrM(gy);
				};
				NU->UnitGroup[i]=zz1;
			};
		}else{
			int zz1=GetUnitKind(gx);
			if(zz1==-1){
				sprintf(gy,"%s,Line %d :%s: Unknown unit kind:%s",Name,Line,Sect,gx);
				ErrM(gy);
			};
			NU->UnitValue=zz1;
			NU->UnitGroup=NULL;
			NU->UnitType=1;	
		};
	}else{
		int zz1=GetNTUnit(NT,gx);
		if(zz1==-1)
        {
            Log.Error( "%s,Line %d :%s: Unknown Monster ID:%s",Name,Line,Sect,gx );
        }
        NU->UnitValue=zz1;
		NU->UnitGroup=NULL;
		NU->UnitType=0;
	};
};
void GetUnitsAndUpgrades(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line,bool UseUnits,bool UseUpgrades){
	NU->CtgType=0;
	char gx[128];
	char gy[128];
	int z=Gscanf(f,"%s",gx);
	if(z!=1)IncPar(Name,Line,Sect);
	if(!strcmp(gx,"GRP")){
		int ngrp;
		z=Gscanf(f,"%d",&ngrp);
		if(z!=1){
			sprintf(gx,"%s,Line %d :%s:Invalid GRP directive format for the list of units.",Name,Line,Sect);
			ErrM(gx);
		};
		NU->UnitGroup=znew(word,ngrp);
		NU->NUnits=ngrp;
		for(int i=0;i<ngrp;i++){
			z=Gscanf(f,"%s",gx);
			if(z!=1)IncPar(Name,Line,Sect);
			int zz1=GetNTElement(NT,gx,UseUnits,UseUpgrades);
			if(zz1==-1){
				sprintf(gy,"%s,Line %d :%s: Unknown Monster/Upgrade ID:%s",Name,Line,Sect,gx);
				ErrM(gy);
			};
			NU->UnitGroup[i]=zz1;
		};
	}else{
		int zz1=GetNTElement(NT,gx,UseUnits,UseUpgrades);
		if(zz1==-1){
			sprintf(gy,"%s,Line %d :%s: Unknown Monster/Upgrade ID:%s",Name,Line,Sect,gx);
			ErrM(gy);
		};
		NU->UnitValue=zz1;
		NU->UnitGroup=NULL;
	};
};
bool ReadValue(GFILE* f,int* result,int* ValType){
	char gx[128];
	
	int z=Gscanf(f,"%s",gx);
	if(z!=1)return false;
	if(gx[0]=='+')*ValType=1;else
	if(gx[0]=='-')*ValType=2;else
	*ValType=0;
	if(*ValType)gx[0]=' ';
	if(gx[strlen(gx)-1]=='%'){
		*ValType|=4;
		gx[strlen(gx)-1]=0;
	};
	int value;
	z=sscanf(gx,"%d",&value);
	if(z!=1) return false;
	*result=value;
	return true;
};
void GetValue(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){
	int value,ValType;
	char gy[128];
	if(ReadValue(f,&value,&ValType)){
		NU->Value=value;
		NU->ValGroup=NULL;
		NU->ValueType=ValType;
	}else{
		sprintf(gy,"%s, Line %d :%s: Invalid value",Name,Line,Sect);
		ErrM(gy);
	};
};
int GetResID(char* Name);
void GetCostValue(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){
	char gx[128];
	char gy[128];
	int p1,p2;
	int z=Gscanf(f,"%s",gx);
	if(z!=1)IncPar(Name,Line,Sect);
	if(!strcmp(gx,"GRP")){
		z=Gscanf(f,"%d",&p1);
		if(z!=1)IncPar(Name,Line,Sect);
		NU->ValGroup=znew(int,p1);
		NU->NValues=p1;
		for(int i=0;i<p1;i++){
			z=Gscanf(f,"%s",gx);
			if(z!=1)IncPar(Name,Line,Sect);
			p2=GetResID(gx);
			if(p2==-1){
				sprintf(gy,"%s, Line %d :%s: Invalid resource ID:%s",Name,Line,Sect,gx);
				ErrM(gy);
			};
			int value,ValType;
			if(!ReadValue(f,&value,&ValType)){
				sprintf(gy,"%s, Line &d :%s: Invalid amount of resource %s.",Name,Line,Sect,gx);
				ErrM(gy);
			};
			NU->ValGroup[i]=((ValType+(p2<<3))<<26)+value;
		};
	}else{
		p2=GetResID(gx);
		if(p2==-1){
			sprintf(gy,"%s, Line %d :%s: Invalid resource ID:%s",Name,Line,Sect,gx);
			ErrM(gy);
		};
		int value,ValType;
		if(!ReadValue(f,&value,&ValType)){
			sprintf(gy,"%s, Line &d :%s: Invalid amount of resource %s.",Name,Line,Sect,gx);
			ErrM(gy);
		};
		NU->Value=((ValType+(p2<<3))<<26)+value;
		NU->ValGroup=NULL;
	};
};
void GetUpgradeCost(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){
	char gx[128];
	char gy[128];
	int p1=0;
	int p2=0;
	int p3=0;
	int z=Gscanf(f,"%s",gy);
	if(strcmp(gy,"#COST")){
		sprintf(gx,"%s, Line %d :%s: #COST expected but %s is found.",Name,Line,Sect,gy);
		ErrM(gx);
	};
	memset(NU->Cost,0,sizeof NU->Cost);
	z=Gscanf(f,"%d",&p1);
	if(z!=1)IncPar(Name,Line,Sect);
	for(int i=0;i<p1;i++){
		z=Gscanf(f,"%s",gx);
		if(z!=1)IncPar(Name,Line,Sect);
		p2=GetResID(gx);
		if(p2==-1){
			sprintf(gy,"%s, Line %d :%s: Invalid resource ID:%s",Name,Line,Sect,gx);
			ErrM(gy);
		};
		z=Gscanf(f,"%d",&p3);
		if(z!=1){
			sprintf(gy,"%s, Line %d :%s: Invalid amount of resource %s.",Name,Line,Sect,gx);
			ErrM(gy);
		};
		NU->Cost[p2]=p3;
	};
};
void GetUpgradeTime(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){
	char gx[128];
	char gy[128];
	int p1;
	int z=Gscanf(f,"%s",gy);
	if(strcmp(gy,"#TIME")){
		sprintf(gx,"%s, Line %d :%s: #TIME expected but %s is found.",Name,Line,Sect,gy);
		ErrM(gx);
	};
	z=Gscanf(f,"%d",&p1);
	if(z!=1){
		sprintf(gx,"%s, Line %d :%s: Invalid #TIME value.",Name,Line,Sect);
		ErrM(gx);
	};
	NU->NStages=p1;
};
int GetWeaponIndex(char* str);
void GetWeapon(GFILE* f,NewUpgrade* NU,Nation* NT,char* Name,int Line){
	char gx[128];
	char gy[128];
	char gz[128];
	int z=Gscanf(f,"%s%s",gx,gz);
	int zzk=GetWeaponType(gx);
	if(zzk==-1){
		sprintf(gy,"%s, Line %d :%s: Unknown weapon kind ID: %s",Name,Line,Sect,gx);
		ErrM(gy);
	};
	int zz=GetWeaponIndex(gz);
	if(zz==-1){
		sprintf(gy,"%s, Line %d :%s: Unknown weapon ID: %s",Name,Line,Sect,gz);
		ErrM(gy);
	};
	NU->ValueType=0;
	NU->ValGroup=NULL;
	NU->Value=zz+(zzk<<16);
};

extern StrHash UPGS;
bool GetIconByName(char* Name, int& File, int& Sprite);

bool UnderstandUpgrade(GFILE* f,char* UpgName,Nation* NT,char* name,int* lpLine,byte NID){
	if(NT->NNUM==0&&NT->NUpgrades==0)UPGS.Clear();
	Sect=UpgName;
	int line=*lpLine;
	int z;
	char gx[128];
	char gy[128];
	char gz[128];
	char gg[128];
	char g1[128];
	char g2[128];
	int p1,p4,icx,icy;
	z=Gscanf(f,"%s%s%d%d%s%d%s",gg,g1,&icx,&icy,g2,&p1,gy);
	if(z!=7){
		IncPar(name,line,Sect);
	};
	if(strcmp(g1,"#POSITION")){
		sprintf(gz,"%s, Line %d :%s: #POSITION expected  but %s is found.",name,line,Sect,g1);
		ErrM(gz);
	};
	if(strcmp(g2,"#LEVEL")){
		sprintf(gz,"%s, Line %d :%s: #LEVEL or #STAGELEVEL expected  but %s is found.",name,line,Sect,g2);
		ErrM(gz);
	};
	NewUpgrade* NewUpg=new NewUpgrade;
	memset(NewUpg,0,sizeof NewUpgrade);
	int NUPG=NT->NUpgrades;
	NT->UPGRADE[NUPG]=NewUpg;
	NT->NUpgrades++;
	if(NT->NNUM==0)UPGS.AddString(UpgName);
	GetUpgradeCost(f,NewUpg,NT,name,line+1);
	GetUpgradeTime(f,NewUpg,NT,name,line+1);

	z=Gscanf(f,"%s",gz);

	NewUpg->IconFileID=0;
	NewUpg->IconSpriteID=0xFFFF;

	if( strcmp(gg,"NONE") ){
		p4=GetIconByName(gg);
		if(p4==-1){
			int f,s;
			if(GetIconByName(gg,f,s)){
				NewUpg->IconFileID=f;
				NewUpg->IconSpriteID=s;
			}
			//sprintf(gy,"%s,Line %d :%s: Invalid icon ID:%s",name,line,Sect,gx,gg);
			//ErrM(gy);
		}else{
			NewUpg->IconSpriteID=p4;
		};
	}

	if(!((icx>=0&&icx<12&&icy>=0&&icy<9)||(icx==-1&&icy==-1))){
		sprintf(gy,"%s,Line %d :%s: Invalid icon coordinates:(%d,%d)",name,line,Sect,icx,icy);
		ErrM(gy);
	};

	NewUpg->Options=0;

	NewUpg->Name=znew(char,strlen(UpgName)+1);
	char* UpgMess=GetTextByID(gy);
	NewUpg->Message=znew(char,strlen(UpgMess)+1);
	strcpy(NewUpg->Name,UpgName);
	strcpy(NewUpg->Message,UpgMess);
	NewUpg->Level=p1;
	NewUpg->CtgType=0;
	NewUpg->Enabled=false;
	NewUpg->PermanentEnabled=false;
	NewUpg->Done=false;
	NewUpg->IsDoing=false;
	NewUpg->Gray=false;
	NewUpg->CurStage=0;
	NewUpg->AutoEnable=NULL;
	NewUpg->AutoPerform=NULL;
	NewUpg->NAutoEnable=0;
	NewUpg->NAutoPerform=0;
	NewUpg->IconPosition=icx+icy*12;
	NewUpg->Individual=false;
	NewUpg->StageUp=false;
	NewUpg->Branch=0xFF;
	NewUpg->NatID=NID;
	NewUpg->ManualDisable=0;
	NewUpg->StageMask=0;
	if(!strcmp(gz,"SPEED")){
		NewUpg->CtgUpgrade=0;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"SHIELD")){
		NewUpg->CtgUpgrade=1;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"PROTECTION")){
		NewUpg->CtgUpgrade=2;
		GetWeaponKind(f,NewUpg,NT,name,line+1);
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		NewUpg->CtgType=4;
		line+=2;
	}else
	if(!strcmp(gz,"ATTPAUSE")){
		NewUpg->CtgUpgrade=3;
		GetCtgParam01(f,NewUpg,NT,name,line+1);
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"WEAPON")){
		NewUpg->CtgUpgrade=4;
		GetCtgParam01(f,NewUpg,NT,name,line+1);
		GetUnits(f,NewUpg,NT,name,line+1);
		GetWeapon(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"BUILD")){
		NewUpg->CtgUpgrade=5;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"LIFE")){
		NewUpg->CtgUpgrade=6;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"PRECISE")){
		NewUpg->CtgUpgrade=7;
		GetCtgParam01(f,NewUpg,NT,name,line+1);
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"ATTRANGE")){
		NewUpg->CtgUpgrade=8;
		GetCtgParam01(f,NewUpg,NT,name,line+1);
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"INSIDE")){
		NewUpg->CtgUpgrade=9;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"COST")){
		NewUpg->CtgUpgrade=10;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetCostValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"ENABLE")){
		NewUpg->CtgUpgrade=11;
		z=Gscanf(f,"%s",gg);
		if(!strcmp(gg,"UNITS")){
			GetUnitsAndUpgrades(f,NewUpg,NT,name,line+1,true,false);
		}else if(!strcmp(gg,"UPGRADES")){
			GetUnitsAndUpgrades(f,NewUpg,NT,name,line+1,false,true);
		}else if(!strcmp(gg,"UNITS&UPGRADES")){
			GetUnitsAndUpgrades(f,NewUpg,NT,name,line+1,true,true);
		}else{
			sprintf(gy,"%s, Line %d :%s: UNITS or UPGRADES or UNITS&UPGRADES expected after ENABLED",name,line+1,Sect);
			ErrM(gy);
		};
		NewUpg->ValGroup=NULL;
		line+=2;
	}else
	if(!strcmp(gz,"DAMAGE")){
		NewUpg->CtgUpgrade=12;
		GetCtgParam01(f,NewUpg,NT,name,line+1);
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"GETRES")){
		NewUpg->CtgUpgrade=13;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"SINGLE_INSIDE")){
		NewUpg->CtgUpgrade=14;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"SINGLE_ATTPAUSE")){
		NewUpg->CtgUpgrade=15;
		GetCtgParam01(f,NewUpg,NT,name,line+1);
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"FISHING")){
		NewUpg->CtgUpgrade=16;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"RAZBROS")){
		NewUpg->CtgUpgrade=17;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"SHAR")){
		NewUpg->CtgUpgrade=18;
		GetUnits(f,NewUpg,NT,name,line+1);
		NewUpg->ValGroup=NULL;
		line+=2;
	}else
	if(!strcmp(gz,"EFFECT_FOOD")){
		NewUpg->CtgUpgrade=19;
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"EFFECT_WOOD")){
		NewUpg->CtgUpgrade=20;
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"EFFECT_STONE")){
		NewUpg->CtgUpgrade=21;
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"GEOLOGY")){
		NewUpg->CtgUpgrade=22;
		line+=2;
	}else
	if(!strcmp(gz,"HEALING")){
		NewUpg->CtgUpgrade=23;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"UPSTAGE0")){
		NewUpg->CtgUpgrade=24;
		line+=2;
	}else
	if(!strcmp(gz,"UPSTAGE1")){
		NewUpg->CtgUpgrade=25;
		line+=2;
	}else
	if(!strcmp(gz,"UPSTAGE2")){
		NewUpg->CtgUpgrade=26;
		line+=2;
	}else
	if(!strcmp(gz,"UPSTAGE3")){
		NewUpg->CtgUpgrade=27;
		line+=2;
	}else
	if(!strcmp(gz,"UPSTAGE4")){
		NewUpg->CtgUpgrade=28;
		line+=2;
	}else
	if(!strcmp(gz,"MORELIFE")){
		NewUpg->CtgUpgrade=29;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"MOREFARMS")){
		NewUpg->CtgUpgrade=30;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"MOREFARMS_SINGLE")){
		NewUpg->CtgUpgrade=32;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"FEAR")){
		NewUpg->CtgUpgrade=33;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"FEAR0")){
		NewUpg->CtgUpgrade=34;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"FEAR1")){
		NewUpg->CtgUpgrade=35;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"FEAR2")){
		NewUpg->CtgUpgrade=36;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"FEAR3")){
		NewUpg->CtgUpgrade=37;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"FEAR4")){
		NewUpg->CtgUpgrade=38;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"FEAR5")){
		NewUpg->CtgUpgrade=39;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"PERSONALLIFE")){
		NewUpg->CtgUpgrade=40;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"CHANGEMD")){
		NewUpg->CtgUpgrade=41;
		GetUnits(f,NewUpg,NT,name,line+1);		
		line+=2;
	}else
	if(!strcmp(gz,"CHANGEUTYPE")){
		NewUpg->CtgUpgrade=42;
		GetUnits(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"DISABLE")){
		NewUpg->CtgUpgrade=43;		
		GetUnitsAndUpgrades(f,NewUpg,NT,name,line+1,true,true);		
		line+=2;
	}else
	if(!strcmp(gz,"AGRESSOR")){
		NewUpg->CtgUpgrade=44;
		GetUnits(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
	if(!strcmp(gz,"UNCONSTRUCT")){
		NewUpg->CtgUpgrade=31;
		GetUnits(f,NewUpg,NT,name,line+1);
		line+=2;
	}else{
	if(!strcmp(gz,"ATTSPEEDSCALE")){
		NewUpg->CtgUpgrade=45;
		GetUnits(f,NewUpg,NT,name,line+1);
		GetValue(f,NewUpg,NT,name,line+1);
		line+=2;
	}else
		sprintf(gy,"%s, Line %d :%s: Unknown category of upgrade:%s",name,line+1,Sect,gz);
		ErrM(gy);
	};
	*lpLine=line;
	for(int cp=1;cp<8;cp++){
		NewUpgrade* NewUpg1=new NewUpgrade;
		int NUPG=NT[cp].NUpgrades;
		NT[cp].UPGRADE[NUPG]=NewUpg1;
		NT[cp].NUpgrades++;
		memcpy(NewUpg1,NewUpg,sizeof NewUpgrade);
	};
	return true;
};
//performs upgrade immediately
void UseValue(int* Value,byte Type,int NewValue){
	switch(Type){
	case 0://XX
		*Value=NewValue;
		return;
	case 1://+XX
		*Value+=NewValue;
		return;
	case 2://-XX
		*Value-=NewValue;
		return;
	case 4://XX%
		*Value=div((*Value)*NewValue,100).quot;
		return;
	case 5://+XX%
		*Value+=div((*Value)*NewValue,100).quot;
		return;
	case 6:
		*Value-=div((*Value)*NewValue,100).quot;
		return;
	};
};
typedef void UpgradeFN(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB);
void UpgradeSpeed(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->Speed;
	UseValue(&val,ValueType,Value);
	AC->Speed=val;
	//for(int i=0;i<NAttTypes;i++){
	//	int val=AC->Rate[i];
	//	UseValue(&val,ValueType,Value);
	//	AC->Rate[i]=val;
	//};
	AC->Changed=true;
};
void UpgradeShield(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->Shield;
	UseValue(&val,ValueType,Value);
	AC->Shield=val;
	AC->Changed=true;
};
void UpgradeProtection(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->Protection[CtgValue];
	UseValue(&val,ValueType,Value);
	AC->Protection[CtgValue]=val;
	AC->Changed=true;
};
void UpgradeAttPause(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->AttackPause[CtgValue];
	UseValue(&val,ValueType,Value);
	AC->AttackPause[CtgValue]=val;
	AC->Changed=true;
};
void UpgradeWeapon(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	//AC->DamWeap[CtgValue]=WPLIST[Value&65535];
	//AC->WeaponKind[CtgValue]=Value>>16;
	AC->Changed=true;
};
void UpgradeBuild(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->ProduceStages;
	int oldval=val;
	UseValue(&val,ValueType,Value);
	AC->ProduceStages=val;
	AC->Changed=true;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->Ref.General==GO){
			if(OB->Stage<oldval){
				if(OB->Stage>=val){
					OB->Stage=val-1;
					OB->NextStage();
				}else{
					if(OB->Stage){
						OB->Stage--;
						OB->NextStage();
					};
				};
			}else OB->Stage=val;
		};
	};
};
void UpgradeLife(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->Life;
	int pval=val;
	if(pval==0)pval=1;
	UseValue(&val,ValueType,Value);
	if(val>65535)val=65535;
	AC->Life=val;
	AC->BirthLife=val;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->Ref.General==GO){
			OB->MaxLife=val;
			OB->Life=int(OB->Life)*val/pval;
			if(OB->Life>val)OB->Life=val;
		};
	};
	AC->Changed=true;
};
void MorelifeUnits(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->Life;
	UseValue(&val,ValueType,Value);
	if(val>65535)val=65535;
	AC->Life=val;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->Ref.General==GO){
			OB->MaxLife=val;
		};
	};
	AC->Changed=true;
};
void MarkObjectLikeModified(OneObject* OB);
void PersonalLife(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	if(OB){
		int val=OB->Life;
		UseValue(&val,ValueType,Value);
		if(val>65535)val=65535;
		OB->Life=val;
		OB->MaxLife=val;
        MarkObjectLikeModified(OB);
        OB->MoreCharacter->Life=val;
	};
};
void UpgradeAttRange(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->AttackRadius2[CtgValue];
	UseValue(&val,ValueType,Value);
	AC->AttackRadius2[CtgValue]=val;
	val=AC->DetRadius2[CtgValue];
	UseValue(&val,ValueType,Value);
	AC->DetRadius2[CtgValue]=val;
	if(CtgValue+1<NAttTypes){
		CtgValue++;
		val=AC->AttackRadius1[CtgValue];
		UseValue(&val,ValueType,Value);
		AC->AttackRadius1[CtgValue]=val;
		val=AC->DetRadius1[CtgValue];
		UseValue(&val,ValueType,Value);
		AC->AttackRadius1[CtgValue]=val;
	};
	AC->Changed=true;
	UpdateAttackR(AC);
};
void InviteAI_Peasants(OneObject* Mine);
void UpgradeInside(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->MaxInside;
	UseValue(&val,ValueType,Value);
	AC->MaxInside=val;
	AC->Changed=true;
};
void UpgradeCost(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	byte VT=(Value>>26)&7;
	byte ResID=byte(DWORD(Value)>>29);
	Value&=0xFFFFFF;
	int val=AC->NeedRes[ResID];
	UseValue(&val,VT,Value);
	AC->NeedRes[ResID]=val;
	AC->Changed=true;
};
void UpgradeMFarm(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->NInFarm;
	UseValue(&val,ValueType,Value);
	AC->NInFarm=val;
	AC->Changed=true;
};
void UpgradeMFarm2(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	// by Xeno to fix the MOREFARMS_SINGLE bug
	/*
	int val=AC->NInFarm;
	UseValue(&val,ValueType,Value);
	if(OB){
		OB->AddFarms=val;
	};
	*/
	AC->NInFarm+=Value;
};
extern City CITY[8];
void UpgradeEnable(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	GO->Enabled=true;
	GO->CondEnabled=true;
    City::EnumAllUnits();	
	GO->MoreCharacter->Changed=true;
};
void UpgradeDamage(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->MaxDamage[CtgValue];
	if(!val)return;
	UseValue(&val,ValueType,Value);
	AC->MaxDamage[CtgValue]=val;
	AC->Changed=true;
	UpdateAttackR(AC);
};
void UpgradeGetRes(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->ResEff;
	UseValue(&val,ValueType,Value);
	AC->ResEff=val;
	AC->Changed=true;
};
void ApplyToOneUnitAndOneCategory(GeneralObject* GO,word Category,NewUpgrade* NU,UpgradeFN* UFN,OneObject* OB){
	if(NU->ValGroup){
		for(int i=0;i<NU->NValues;i++)
			UFN(GO,Category,NU->ValueType,NU->ValGroup[i],OB);
	}else{
		UFN(GO,Category,NU->ValueType,NU->Value,OB);
	};
};
void UpgradeSingleAttPause(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	if(!OB)return;
	int val=OB->PersonalDelay;
	UseValue(&val,ValueType,Value);
	OB->PersonalDelay=val;
};
void UpgradeSingleInside(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	if(!OB)return;
	int val=OB->AddInside;
	UseValue(&val,ValueType,Value);
	OB->AddInside=val;
	if(OB&&OB->newMons->Usage==MineID&&OB->Nat->AI_Enabled){
		InviteAI_Peasants(OB);
	};
};
bool ChangeUnitNIndex(OneObject* OB, word NewNIndex, int ThroughState);
void ChangeMD_(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	if(!OB)return;
    //ChangeUnitNIndex(OB,GO->NIndex,0);
	//OB->ActiveAbility->SetBornNIndex();
	if(!OB->ActiveAbility){
		OB->ActiveAbility = new UnitActiveAbilityList(OB);	
	}
	OB->ActiveAbility->BornNIndex=GO->NIndex;
	OB->ActiveAbility->BornNM=GO->newMons;
	OB->ActiveAbility->BornAC=GO->MoreCharacter;
};
GeneralObject* prevGO=NULL;
void ChangeUnitsType(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	if(!OB)return;
	if(!prevGO){
		prevGO=GO;
		return;
	}
	int N=NtNUnits[OB->NNUM];
	for(int i=0;i<N;i++){
		int id=NatList[OB->NNUM][i];
		OneObject* OB=Group[id];
		if(OB){
			if(OB->NIndex==prevGO->NIndex){
				if(!OB->ActiveAbility){
					OB->ActiveAbility = new UnitActiveAbilityList(OB);	
				}
				OB->ActiveAbility->BornNIndex=GO->NIndex;
				OB->ActiveAbility->BornNM=GO->newMons;
				OB->ActiveAbility->BornAC=GO->MoreCharacter;                
			}
		}
	}
	prevGO=NULL;
};
void SetAgression(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	if(!OB)return;
	int N=NtNUnits[OB->NNUM];
	for(int i=0;i<N;i++){
		int id=NatList[OB->NNUM][i];
		OneObject* OB=Group[id];
		if(OB){
			if(OB->NIndex==GO->NIndex){
				OB->NNUM=7;
				OB->NMask=128;				
				if(OB->ActiveAbility)OB->ActiveAbility->BornNNUM=7;
			}
		}
	}	
};

void UpgradeFishing(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->FishAmount;
	UseValue(&val,ValueType,Value);
	AC->FishAmount=val;
	val=AC->FishSpeed;
	UseValue(&val,ValueType,Value);
	AC->FishSpeed=val;
	AC->Changed=true;
};
void UpgradeAttackSpeed(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->AttackSpeed;
	UseValue(&val,ValueType,Value);
	AC->AttackSpeed=val;
	AC->Changed=true;
};
void UpgradeRazbros(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->Razbros;
	UseValue(&val,ValueType,Value);
	AC->Razbros=val;
	AC->Changed=true;
};
void UpgradeShar(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	NATIONS[OB->NNUM].SharAllowed=true;
	NATIONS[OB->NNUM].SharX=OB->RealX;
	NATIONS[OB->NNUM].SharY=OB->RealY;
	NATIONS[OB->NNUM].SearchRadius=180;
};
int CurFear=0;
void UpgradeFear(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
#ifndef NEWMORALE
	AdvCharacter* AC=GO->MoreCharacter;
	int val=AC->FearFactor[CurFear];
	UseValue(&val,ValueType,Value);
	AC->FearFactor[CurFear]=val;
#endif
};
void UpgradeAllFears(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
#ifndef NEWMORALE
	AdvCharacter* AC=GO->MoreCharacter;
	for(int i=0;i<NFEARSUBJ;i++){
		int val=AC->FearFactor[CurFear];
		UseValue(&val,ValueType,Value);
		AC->FearFactor[CurFear]=val;
	};
#endif
};
void HealUnits(GeneralObject* GO,word CtgValue,byte ValueType,int Value,OneObject* OB){
	if(!OB)return;
	int N=NtNUnits[OB->NNUM];
	word* units=NatList[OB->NNUM];
	for(int i=0;i<N;i++){
		word MID=units[i];
		if(MID!=0xFFFF){
			OneObject* OBJ=Group[MID];
			if(OBJ&&OBJ->Ref.General==GO){
				int L=OBJ->Life;
				UseValue(&L,ValueType,Value);
				if(L>OBJ->MaxLife)L=OBJ->MaxLife;
				OBJ->Life=L;
			};
		};
	};
};

void ApplyToOneUnit(GeneralObject* GO,NewUpgrade* NU,UpgradeFN* UFN,OneObject* OB){
	NewMonster* NM=GO->newMons;
	switch(NU->CtgType){
	case 0://no categoties
		ApplyToOneUnitAndOneCategory(GO,0,NU,UFN,OB);
		break;
	case 1://WEAPONKIND
		if(NU->CtgGroup){
			for(int j=0;j<NU->NCtg;j++){
				int Ctgval=NU->CtgGroup[j];
				for(int i=0;i<NAttTypes;i++){
					if(NM->WeaponKind[i]==Ctgval)
						ApplyToOneUnitAndOneCategory(GO,i,NU,UFN,OB);
				};
			};
		}else{
			for(int i=0;i<NAttTypes;i++){
				if(NM->WeaponKind[i]==NU->CtgValue)
					ApplyToOneUnitAndOneCategory(GO,i,NU,UFN,OB);
			};
		};
		break;
	case 2://ATTINDEX
		if(NU->CtgGroup){
			for(int j=0;j<NU->NCtg;j++){
				int Ctgval=NU->CtgGroup[j];
				//assert(Ctgval>=0&&Ctgval<NAttTypes);
				ApplyToOneUnitAndOneCategory(GO,Ctgval,NU,UFN,OB);
			};
		}else{
			//assert(NU->CtgValue>=0&&NU->CtgValue<NAttTypes);
			ApplyToOneUnitAndOneCategory(GO,NU->CtgValue,NU,UFN,OB);
		};
		break;
	case 3://ALL
		{
			for(int i=0;i<NAttTypes;i++)
				ApplyToOneUnitAndOneCategory(GO,i,NU,UFN,OB);
		};
		break;
	case 4://PROTECTION
		if(NU->CtgGroup){
			for(int j=0;j<NU->NCtg;j++){
				int Ctgval=NU->CtgGroup[j];
				//assert(Ctgval>=0&&Ctgval<32);
				ApplyToOneUnitAndOneCategory(GO,Ctgval,NU,UFN,OB);
			};
		}else{
			//assert(NU->CtgValue>=0&&NU->CtgValue<32);
			ApplyToOneUnitAndOneCategory(GO,NU->CtgValue,NU,UFN,OB);
		};
		break;
	};
};
void ApplyToUnits(Nation* NT,NewUpgrade* NU,UpgradeFN* UFN,OneObject* OB){
	if(NU->UnitGroup){
		if(NU->UnitType==1){
			for(int j=0;j<NU->NUnits;j++){
				int UnitValue=NU->UnitGroup[j];
				for(int i=0;i<NT->NMon;i++){
					GeneralObject* GO=NT->Mon[i];
					if(GO->newMons->Kind==UnitValue)
						ApplyToOneUnit(GO,NU,UFN,OB);
				};
			};
		}else{	
			for(int i=0;i<NU->NUnits;i++){
				word val=NU->UnitGroup[i];
				//assert(val<NT->NMon);
				ApplyToOneUnit(NT->Mon[NU->UnitGroup[i]],NU,UFN,OB);
			};
		};
	}else{
		if(NU->UnitType==1){//UNITKIND
			int UnitValue=NU->UnitValue;
			for(int i=0;i<NT->NMon;i++){
				GeneralObject* GO=NT->Mon[i];
				if(GO->newMons->Kind==UnitValue)
					ApplyToOneUnit(GO,NU,UFN,OB);
			};
		}else{
			//assert(NU->UnitValue<NT->NMon);
			ApplyToOneUnit(NT->Mon[NU->UnitValue],NU,UFN,OB);
		};
	};
};
void EraseUnitsInPoint(int x,int y){
	if(x>0&&y>0&&x<VAL_MAPSX&&y<VAL_MAPSX){
		int xc=x>>3;
		int yc=y>>3;
		int cell=((yc>>7)<<VAL_SHFCX)+(xc>>7)+VAL_MAXCX+1;
		int NMon=MCount[cell];
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB->x==x&&OB->y==y)OB->Die();
			};
		};
	};
};
void UnConstructBuilduing(OneObject* OB);
void PerformNewUpgrade(Nation* NT,int UIndex,OneObject* OB){
	//assert(UIndex<=NT->NUpgrades);
	NewUpgrade* NU=NT->UPGRADE[UIndex];
	switch(NU->CtgUpgrade){
	case 0://SPEED
		ApplyToUnits(NT,NU,&UpgradeSpeed,OB);
		break;
	case 1://SHIELD
		ApplyToUnits(NT,NU,&UpgradeShield,OB);
		break;
	case 2://PROTECTION
		ApplyToUnits(NT,NU,&UpgradeProtection,OB);
		break;
	case 3://ATTPAUSE
		ApplyToUnits(NT,NU,&UpgradeAttPause,OB);
		break;
	case 4://WEAPON
		ApplyToUnits(NT,NU,&UpgradeWeapon,OB);
		break;
	case 5://BUILD
		ApplyToUnits(NT,NU,&UpgradeBuild,OB);
		break;
	case 6://LIFE
		ApplyToUnits(NT,NU,&UpgradeLife,OB);
		break;
	case 7://PRECISE
		break;
	case 8://ATTRANGE
		ApplyToUnits(NT,NU,&UpgradeAttRange,OB);
		break;
	case 9://INSIDE
		ApplyToUnits(NT,NU,&UpgradeInside,OB);
		break;
	case 10://COST
		ApplyToUnits(NT,NU,&UpgradeCost,OB);
		break;
	case 11://ENABLE
		ApplyToUnits(NT,NU,&UpgradeEnable,OB);
		break;
	case 12://DAMAGE
		ApplyToUnits(NT,NU,&UpgradeDamage,OB);
		break;
	case 13://GETRES
		ApplyToUnits(NT,NU,&UpgradeGetRes,OB);
		break;
	case 14://SINGLE_INSIDE
		ApplyToUnits(NT,NU,&UpgradeSingleInside,OB);
		break;
	case 15://SINGLE_ATTPAUSE
		ApplyToUnits(NT,NU,&UpgradeSingleAttPause,OB);
		break;
	case 16://FISHING
		ApplyToUnits(NT,NU,&UpgradeFishing,OB);
		break;
	case 17://RAZBROS
		ApplyToUnits(NT,NU,&UpgradeRazbros,OB);
		break;
	case 18://SHAR
		ApplyToUnits(NT,NU,&UpgradeShar,OB);
		break;
	case 19://EFFECT_FOOD
		{
			int val=NT->FoodEff;
			UseValue(&val,NU->ValueType,NU->Value);
			NT->FoodEff=val;
		};
		break;
	case 20://EFFECT_WOOD
		{
			int val=NT->WoodEff;
			UseValue(&val,NU->ValueType,NU->Value);
			NT->WoodEff=val;
		};
		break;
	case 21://EFFECT_STONE
		{
			int val=NT->StoneEff;
			UseValue(&val,NU->ValueType,NU->Value);
			NT->StoneEff=val;
		};
		break;
	case 22://GEOLOGY
		NT->Geology=true;
		break;
	case 23:
		ApplyToUnits(NT,NU,&HealUnits,OB);
		break;
	case 29:
		ApplyToUnits(NT,NU,&MorelifeUnits,OB);
		break;
	case 30:
		ApplyToUnits(NT,NU,&UpgradeMFarm,OB);
		break;
	case 31:
		if(OB)UnConstructBuilduing(OB);
		break;
	case 32:
		ApplyToUnits(NT,NU,&UpgradeMFarm2,OB);
		break;
	case 33:
		ApplyToUnits(NT,NU,&UpgradeAllFears,OB);
		break;
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
		CurFear=NU->CtgUpgrade-34;
		ApplyToUnits(NT,NU,&UpgradeFear,OB);
		break;
	case 40:
		ApplyToUnits(NT,NU,&PersonalLife,OB);
		break;
	case 41:
		ApplyToUnits(NT,NU,&ChangeMD_,OB);
		break;
	case 42:
		ApplyToUnits(NT,NU,&ChangeUnitsType,OB);
		break;
	case 43://DISABLE
		for(int i=0;i<NU->NUnits;i++){
			int ut=NU->UnitGroup[i];
			if(ut<8192){
                GeneralObject* GO=NT->Mon[ut];
				GO->ManualDisable=true;
				GO->Enabled=false;
				GO->MoreCharacter->Changed=true;
			}else{
                NewUpgrade* NU=NT->UPGRADE[ut-8192];
				NU->ManualDisable=true;
				NU->Enabled=false;
			}
		}
		break;
	case 44:
		ApplyToUnits(NT,NU,&SetAgression,OB);        
		break;
	case 45:
		ApplyToUnits(NT,NU,&UpgradeAttackSpeed,OB);        
		break;
#ifdef CONQUEST
	case 24://UPSTAGE0
	case 25:
	case 26:
	case 27:
	case 28://UPSTAGE4
		if(!OB->InFire){
			int sh=NU->CtgUpgrade-24;
			int sh3=sh*3;
			NewMonster* NM=OB->newMons;
			int xc,yc;
			OB->GetCornerXY(&xc,&yc);
			if(OB->newMons->CompxCraft&&NM->CompxCraft->Mask&(1<<sh)){
				OB->StageState&=~(7<<sh3);
				OB->StageState|=(2<<sh3);
				OB->Life=OB->MoreCharacter->Life;
				for(int i=0;i<=5;i++){
					byte ms=(OB->StageState>>(i*3))&7;
					if(ms==2){
						OB->Life+=NM->CompxCraft->Stages[i].AddPoints;
						if(NM->NSLockPt[i]){
							for(int j=0;j<NM->NSLockPt[i];j++){
								BSetPt(xc+NM->SLockX[i][j],yc+NM->SLockY[i][j]);
								BSetPt(xc+NM->SLockX[i][j],yc+NM->SLockY[i][j]);
							};
						};
					};
				};
				OB->MaxLife=OB->Life;
			};
		};
		break;
#endif
	};

	if(NU->AutoEnable){
		for(int i=0;i<NU->NAutoEnable;i++){
			NewUpgrade* NU1=NT->UPGRADE[NU->AutoEnable[i]];
			NU1->Enabled=true;
			NU1->PermanentEnabled=true;
			NU1->Done=false;
		};
	};
	if(NU->AutoPerform){
		for(int i=0;i<NU->NAutoPerform;i++){
			PerformNewUpgrade(NT,NU->AutoPerform[i],OB);
		};
	};
};
void CreateAdvCharacter(AdvCharacter* AC,NewMonster* NM){
	for(int i=0;i<NAttTypes;i++){
		AC->AttackPause[i]=NM->AttackPause[i];
		AC->AttackRadius1[i]=NM->AttackRadius1[i];
		AC->AttackRadius2[i]=NM->AttackRadius2[i];
		//AC->DamWeap[i]=NM->DamWeap[i];
		AC->DetRadius1[i]=NM->DetRadius1[i];
		AC->DetRadius2[i]=NM->DetRadius2[i];
		AC->MaxDamage[i]=NM->MaxDamage[i];
		AC->Rate[i]=NM->Rate[i];
		AC->WeaponKind[i]=NM->WeaponKind[i];
	};
	for(int i=0;i<16;i++)AC->Protection[i]=byte(NM->Protection[i]);
	for(int i=0;i<8;i++)AC->NeedRes[i]=NM->NeedRes[i];
	AC->Life=NM->Life;
	AC->BirthLife=NM->Life;
	AC->MaxMana=NM->MaxMana;

	AC->ProduceStages=NM->ProduceStages;
	AC->Shield=NM->Shield;
	AC->Changed=false;
	UpdateAttackR(AC);
	AC->NInFarm=NM->NInFarm;
#ifdef NEWMORALE
	AC->MoraleDecSpeed=NM->MoraleDecSpeed;
	AC->StartMorale=NM->StartMorale;
#else
	for(i=0;i<NFEARSUBJ;i++)AC->FearFactor[i]=NM->FearFactor[i];
#endif
	AC->Speed=NM->Speed;
	AC->AttackSpeed=NM->AttackSpeed;
	AC->VisionType=NM->VisionType;
	AC->VisRange=NM->VisRange;

	AC->MaxInside=NM->MaxInside;
	AC->ResEff=NM->ResEff;
	AC->FishSpeed=NM->FishSpeed;
	AC->FishAmount=NM->FishAmount;
	AC->Razbros=NM->Razbros;

};
void CreateNewMonster(NewMonster* NM,AdvCharacter* AC){
	for(int i=0;i<NAttTypes;i++){
		NM->AttackPause[i]=AC->AttackPause[i];
		NM->AttackRadius1[i]=AC->AttackRadius1[i];
		NM->AttackRadius2[i]=AC->AttackRadius2[i];
		//NM->DamWeap[i]=AC->DamWeap[i];
		NM->DetRadius1[i]=AC->DetRadius1[i];
		NM->DetRadius2[i]=AC->DetRadius2[i];
		NM->MaxDamage[i]=AC->MaxDamage[i];
		NM->Rate[i]=AC->Rate[i];
		NM->WeaponKind[i]=AC->WeaponKind[i];
	};
	for(int i=0;i<16;i++)NM->Protection[i]=byte(AC->Protection[i]);
	for(int i=0;i<8;i++)NM->NeedRes[i]=AC->NeedRes[i];

	NM->Life=AC->Life;
	NM->Life=AC->BirthLife;
	NM->MaxMana=AC->MaxMana;

	NM->ProduceStages=AC->ProduceStages;
	NM->Shield=AC->Shield;
	//NM->Changed=false;
	UpdateAttackR(AC);
	NM->NInFarm=AC->NInFarm;
#ifdef NEWMORALE
	NM->MoraleDecSpeed=AC->MoraleDecSpeed;
	NM->StartMorale=AC->StartMorale;
#else
	for(i=0;i<NFEARSUBJ;i++)NM->FearFactor[i]=AC->FearFactor[i];
#endif
	NM->Speed=AC->Speed;
	NM->AttackSpeed=AC->AttackSpeed;
	NM->VisionType=AC->VisionType;
	NM->VisRange=AC->VisRange;

	NM->MaxInside=AC->MaxInside;
	NM->ResEff=AC->ResEff;
	NM->FishSpeed=AC->FishSpeed;
	NM->FishAmount=AC->FishAmount;
	NM->Razbros=AC->Razbros;

};
int GetUpgradeID(Nation* NT,char* Name){
	return UPGS.SearchString(Name);
};

