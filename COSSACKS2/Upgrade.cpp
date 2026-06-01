#include "stdheader.h"
Upgrade::Upgrade(){
	NUpgrades=0;
};
void PerformUpgradeLink(OneObject* OBJ);
void WriteUpgradeAction(byte NI,NewUpgrade* NU);
void OneObject::PerformUpgrade(word NewU,word MakerID){
	if(CheckOrderAbility())return;
	if(UnlimitedMotion)return;
	if(Nat->NUpgrades<=NewU||!Ready)return;
	word NUP=Ref.General->NUpgrades;
	word* UPL=Ref.General->Upg;
	bool canU=false;
	for(int pp=0;pp<NUP;pp++)if(UPL[pp]==NewU)canU=true;
	if(!canU)return;
	NewUpgrade* NU=Nat->UPGRADE[NewU];
	if(!NU->Enabled)return;
	if(NU->StageMask){
		byte m=NU->StageMask;
		word st=StageState;
		for(int i=0;i<5;i++){
			if(m&1){
				byte cs=st&7;
				if(cs!=2)return;
			};
			m >>=1;
			st>>=3;
		};
	};
	for(int i=0;i<8;i++){
		if(XRESRC(NNUM,i)<NU->Cost[i])return;
	};
	if(NU->Individual){
		if(SingleUpgLevel!=NU->Level)return;
	};
#ifdef CONQUEST
	if(NU->StageUp){
		byte st=(StageState>>((NU->CtgUpgrade-24)*3))&7;
		if(st==2)return;
	};
#endif
	int Use=newMons->Usage;
	for(int i=0;i<8;i++){
		AddXRESRC(NNUM,i,-NU->Cost[i]);
		Nat->ControlProduce(NU->Branch,i,-NU->Cost[i]);
		if(Use==MineID)Nat->ResOnMines[i]+=NU->Cost[i];
		Nat->ResOnUpgrade[i]+=NU->Cost[i];
	};
	Order1* Or1=GetOrdBlock();
	Or1->PrioryLevel=96;
	Or1->NextOrder=LocalOrder;
	Or1->OrderType=75;//Upgrade
	Or1->OrderTime=0;
	Or1->DoLink=&PerformUpgradeLink;
	Or1->info.PUpgrade.NewUpgrade=NewU;
	Or1->info.PUpgrade.Stage=1;
	Or1->info.PUpgrade.NStages=int(NU->NStages)<<8;
	Or1->info.PUpgrade.OldUpgrade=MakerID;
	//Nat->UPG.utp[NewU]->Stage=1;
	Order1* LOR=LocalOrder;
	//if(int(InLineCom))FreeAsmLink();
	LocalOrder=Or1;
	//OrderReport=NULL;
	//MessageKind=0;
	//Sender=0xFFFF;
	if( !(NU->Individual||NU->StageUp) || NATIONS[NNUM].AI_Enabled/*&&!newMons->Producer*/ ){
		NU->Enabled=false;
		NU->PermanentEnabled=false;
		NU->IsDoing=true;
	};
	Ustage=0;
	NUstages=NU->NStages;
	Ready=false;
	WriteUpgradeAction(NNUM,NU);
};	
void AssignHint1(char* s,int time);
void CorrectUpgradeHint(_str& str, NewUpgrade* Upg);
extern int tmtmt;
extern int StartTmtmt;
extern bool vFastUpg;
void PerformUpgradeLink(OneObject* OBJ){
	word OI=OBJ->LocalOrder->info.PUpgrade.NewUpgrade;
	Nation* NT=OBJ->Nat;
	byte NI=NT->NNUM;
	NewUpgrade* NUP=NT->UPGRADE[OI];
	OBJ->LocalOrder->info.PUpgrade.Stage+=GameSpeed;
	OBJ->Ustage=OBJ->LocalOrder->info.PUpgrade.Stage>>8;
	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;
#ifdef CONQUEST
	byte ctg=NUP->CtgUpgrade;
	if(OBJ->newMons->CompxCraft){
		if(ctg>=24&&ctg<=28){
			int sh=ctg-24;
			if(OBJ->newMons->CompxCraft->Mask&(1<<sh)){
				sh*=3;
				OBJ->StageState&=~(7<<sh);
				OBJ->StageState|=(1<<sh);
			};
		};
	};
#endif
	if (OBJ->LocalOrder->info.PUpgrade.Stage>=OBJ->LocalOrder->info.PUpgrade.NStages || vFastUpg) {
		OBJ->Ready=true;
		OneObject* OBU=NULL;
		word MID=OBJ->LocalOrder->info.PUpgrade.OldUpgrade;
		if(MID!=0xFFFF){
			OBU=Group[MID];
		};
		if(!(NUP->Individual||NUP->StageUp))NUP->Done=true;
		PerformNewUpgrade(&NATIONS[OBJ->NNUM],OI,OBU);
		OBJ->Nat->AddUpgrade(OI,tmtmt+StartTmtmt);
		if(NI==MyNation){
			_str str;
			CorrectUpgradeHint(str,NUP);
			char ccc[1024];
			sprintf(ccc,UPGMADE,str.pchar());
			AssignHint1(ccc,100);
			LastActionX=OBJ->RealX>>4;
			LastActionY=OBJ->RealY>>4;
			void MakeOrderSound(OneObject* OB,byte SMask);
			MakeOrderSound(OBJ,16);
		};
		if(OBJ->LocalOrder&&OBJ->LocalOrder->DoLink==&PerformUpgradeLink)OBJ->DeleteLastOrder();
		if(NUP->Individual)OBJ->SingleUpgLevel++;

	};
};
void SimpleUTP::DoUpgrade(byte NI,OneObject *OB){
	
};