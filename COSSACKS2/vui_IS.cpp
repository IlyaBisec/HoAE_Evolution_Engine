#include "stdheader.h"
#include "unitability.h"
#include ".\cvi_brigcreate.h"
#include ".\cvi_MainMenu.h"
#include "GameInterface.h"
//////////////////////////////////////////////////////////////////////////
void vgf_CreateInterfSynchroAction(){
	vui_InterfSynchro::E=ENUM.Get("ve_InterfSynchro");	
	reg_v_InterfSynchro<vui_IS_GBSel>("выделить здания SpecialSel");
	reg_v_InterfSynchro<vui_IS_AttackGround2>("Пушка: залп по точке");
	reg_v_InterfSynchro<vui_IS_EraseObject>("Удалить мгновенно юнита");	
	reg_v_InterfSynchro<vui_IS_AttackGround>("Пушка: залп");
	reg_v_InterfSynchro<vui_IS_FillCannon>("Пушка: пополнение");
	reg_v_InterfSynchro<vui_IS_MakeMaxStage>("Здание: заготовку под ключ");
	reg_v_InterfSynchro<vui_IS_Sel_SetActivity>("sel set ActivityState");
	
	reg_v_InterfSynchro<vui_IS_Abl_Target>("ability target execute");
	reg_v_InterfSynchro<vui_IS_Abl_Click>("active ability execute");
	reg_v_InterfSynchro<vui_IS_Abl_ClothedSlot>("active set clothed slot");
	
	reg_v_InterfSynchro<vui_IS_SelFearDown>("selection go scary movie");
	reg_v_InterfSynchro<vui_IS_SelRTF>("selection redy to fire/not fire");
	reg_v_InterfSynchro<vui_IS_SelChargeAll>("selection charge all");	
	reg_v_InterfSynchro<vui_IS_BrigHoldPosit>("selection brigs hold position");	
	reg_v_InterfSynchro<vui_IS_SelRifle>("selection rifle state");
	// cvi_BrigCreate
	reg_v_InterfSynchro<cvi_IS_BC_Create>("BrigCreate create: from object");
	reg_v_InterfSynchro<vui_IS_AddWallSection>("Add wall section");
	reg_v_InterfSynchro<vui_IS_LeaveMineAll>("Leve mine all units");
	reg_v_InterfSynchro<vui_IS_Sel_MakeReformation>("REFORMA");
	reg_v_InterfSynchro<vui_IS_Sel_ClearOrders>("");
	reg_v_InterfSynchro<vui_IS_CancelAttack>("Cancel orders for cannon");
	reg_v_InterfSynchro<vui_IS_ItemPickUp>("Pick Up");
	reg_v_InterfSynchro<vui_IS_BonusPickUp>("Bonus Up");
	reg_v_InterfSynchro<vui_IS_AbilityTrade>("Ability Trade");
	reg_v_InterfSynchro<vui_IS_ExitMagazine>("Exit Magazine");
	reg_v_InterfSynchro<vui_IS_Abl_TakeOff>("Выбросить предмет");

};
//////////////////////////////////////////////////////////////////////////
void GetUnitCost(byte NI,word NIndex,int* Cost,int x,int y);
DLLEXPORT void ClearSelection(byte Nat);
void AddUnitToSelected(byte NI,OneObject* OB);
void CopyReIm(byte NI);
void MakeStandGround(Brigade* BR);
DLLEXPORT int GetRND(int);
void ClearCannonAutoShot(OneObject* OB);
void StopCannonRotation(OneObject* OB);
//////////////////////////////////////////////////////////////////////////
void vui_IS_EraseObject::SynchroAction(){
	OneObject* OB=Group[Data.ObjID];
	if(OB&&OB->Serial==Data.ObjSN&&!OB->Sdoxlo){
		void EraseObject(OneObject* OB);
		EraseObject(OB);
	}
};
void vui_IS_AttackGround::SynchroAction(){
	OneObject* OB=Group[Data.ObjID];
	if(OB&&OB->Serial==Data.ObjSN&&!OB->Sdoxlo){
		int x=(OB->RealX>>4)+TCos[OB->RealDir];
		int y=(OB->RealY>>4)+TSin[OB->RealDir];
		OB->NewAttackPoint(x,y,16+128,0,1);
		// cossacks2 only
		ClearCannonAutoShot(OB);
		/*
		if(OB->ActiveAbility){
			int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
			for(int i=0;i<n;i++){
				ActiveUnitAbility* UA=OB->ActiveAbility->ActiveAbilities[n];
				if(UA&&UA->Visible){
					CannonAutoShotActive* CAS=dynamic_cast<CannonAutoShotActive*>(UA);
					if(CAS){
						CAS->On=false;
					}
				}
			}				
		}
		*/
	}
};
void vui_IS_AttackGround2::SynchroAction(){
	OneObject* OB=Group[Data.ObjID];
	if(OB&&OB->Serial==Data.ObjSN&&!OB->Sdoxlo){
		OB->NewAttackPoint(Data.x,Data.y,16+128,0,20000);
		// cossacks2 only
		ClearCannonAutoShot(OB);
	}
};
void vui_IS_CancelAttack::SynchroAction(){
	OneObject* OB=Group[Data.ObjID];
	if(OB&&OB->Serial==Data.ObjSN&&!OB->Sdoxlo){
		OB->ClearOrders();
		// cossacks2 only
		ClearCannonAutoShot(OB);
		StopCannonRotation(OB);
	}
};
void vui_IS_Sel_ClearOrders::SynchroAction(){	
	int N=NSL[Data.NI];
	word* IDS=Selm[Data.NI];
	word* SNS=SerN[Data.NI];
	DynArray<word> brList;
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				OB->ClearOrders();
				OB->DestX=-1;
				if(OB->BrigadeID!=0xFFFF)
				{
					int n=brList.GetAmount();
					bool fnd=false;
					for(int j=0;j<n;j++)
					{
						if(brList[j]==OB->BrigadeID)
						{
							fnd=true;
							break;
						}
					}
					if(!fnd)
						brList.Add(OB->BrigadeID);
				}
				// Guard
				OB->Guard=0xFFFF;
				// HoldPosition
				OB->StandGround=false;
				// Motion
				OB->DestX=-1;
			}
		}
	}
	int n=brList.GetAmount();
	for(int i=0;i<n;i++)
	{
		byte NI=GSets.CGame.cgi_NatRefTBL[Data.NI];
		Brigade* br=&NATIONS[NI].CITY->Brigs[brList[i]];
		if(br&&br->Enabled)
		{
			bool stay=true;			
			for(int j=0;j<br->NMemb;j++)
			{
				if(br->Memb[j]!=0xFFFF)
				{
					if(OneObject* ob=Group[br->Memb[j]])
					{
						if(ob->DestX!=-1 || ob->LocalOrder)stay=false;
						ob->ClearOrders();
						ob->DestX=-1;
					}
				}
			}
			int x,y;
			byte D;
			br->lastTimeGetCenter=-1;
			if( stay==false && br->GetCenter(&x,&y,&D) ){
				x+=TCos[D]>>4;
				y+=TSin[D]>>4;
				br->HumanGlobalSendTo(x,y,int(D)+2048,128+16,0);
			}else br->ClearNewBOrders();
		}
	}
}
void vui_IS_FillCannon::SynchroAction(){
	int N=NSL[Data.NI];
	word* IDS=Selm[Data.NI];
	word* SNS=SerN[Data.NI];
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				void FillObjectByUnits(OneObject* OB);
				FillObjectByUnits(OB);
			}
		}
	}
}
void vui_IS_MakeMaxStage::SynchroAction(){
	OneObject* OB=Group[Data.Index];
	if(OB){
		int Cost[8];
		GetUnitCost(OB->NNUM,OB->NIndex,Cost,-1,-1);
		for(int i=0;i<6;i++)if(XRESRC(OB->NNUM,i)<Cost[i])return;
		for(int i=0;i<6;i++)AddXRESRC(OB->NNUM,i,-Cost[i]);
		for(int i=OB->Stage;i<OB->NStages;i++)OB->NextStage();
		OB->Ready=true;
	}
	// Price
}
void vui_IS_GBSel::SynchroAction(){
	return;
	byte NI=Data.NI;
	ClearSelection(NI);
	word* ID=NatList[NI];	// list units in nation
	int N=NtNUnits[NI];
	for(int i=0;i<N;i++,ID++){
		OneObject* OB=Group[*ID];
		if(OB&&OB->NewBuilding&&OB->newMons->SpecialSel&&OB->Stage==OB->NStages){
			AddUnitToSelected(NI,OB);
			//break;
		}
	}
	CopyReIm(NI);
}
void vui_IS_Sel_SetActivity::SynchroAction(){
	if(EngSettings.DontUseAgressiveState)return;
	int N=NSL[Data.NI];
	word* IDS=Selm[Data.NI];
	word* SNS=SerN[Data.NI];
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				OB->ActivityState=Data.ActivityState;
				OB->GroundState=OB->ActivityState==2;
				if(OB->NewState!=OB->GroundState)OB->NewState=OB->GroundState;
			}
		}
	}
}
void vui_IS_Sel_MakeReformation::SynchroAction(){
	void MakeReformation(byte NI,word BrigadeID,byte FormType);
	MakeReformation(Data.NI,Data.BrigadeID,Data.FormType);
}

bool GetHero(OneObject* OB);

void vui_IS_Abl_Target::SynchroAction(){
	addrand(Data.AblID);
	itr_GetSelected.Create(Data.NI);
	if(Data.AblID==-1){
		// персональная абилка - только серийный номер
		while(OneObject* OB=itr_GetSelected.Next()){
			if(OB&&OB->ActiveAbility){
				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					if(AA->Serial==Data.AblSerial){
						
						addrand(Data.AblSerial);
						
						AA->Execute(Data.TargOB,Data.TargX,Data.TargY,-1);
						break;
					}
				}
			}
		}
	}else{
		while(OneObject* OB=itr_GetSelected.Next()){
			if(OB&&OB->ActiveAbility){

				bool Hero=GetHero(OB);
				addrand(Hero);
				
				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					if(AA->TypeAbil!=0){
						UnitAbility* UA=AA->GetA();
						if( UA && UA->Index==Data.AblID && (AA->Serial==Data.AblSerial||!Hero) ){
							
							addrand(Data.AblID);

							AA->Execute(Data.TargOB,Data.TargX,Data.TargY,-1);
							break;
						}
					}
				}
			}
		}
	}
}
vui_IS_Abl_TakeOff::vui_IS_Abl_TakeOff(ActiveUnitAbility* AA, byte NI, int x, int y){
	Data.NI=NI;
	Data.NIndex=AA->OB->NIndex;
	Data.AblSerial=AA->Serial;
	Data.x=x;
	Data.y=y;
};

bool DropDownItem(OneObject* OB, byte OrdType, DWORD AblSerial, int Tx, int Ty, int Dist);

void vui_IS_Abl_TakeOff::SynchroAction(){
	
	addrand(Data.NI);
	addrand(Data.NIndex);
	addrand(Data.AblSerial);

	itr_GetSelected.Create(Data.NI);
	while(OneObject* OB=itr_GetSelected.Next()){
		if( OB && OB->NIndex==Data.NIndex && OB->ActiveAbility ){

			//bool Hero=GetHero(OB);
			//addrand(Hero);
			
			itr_AA.Create(OB);
			while(ActiveUnitAbility* AA=itr_AA.Next()){
				if(AA->TypeAbil!=0){
					UnitAbility* UA=AA->GetA();
					if( UA /*&& UA->Index==Data.AblID*/ && (AA->Serial==Data.AblSerial/*||!Hero*/) ){
						//AA->Droped=true;
						//ext_OnDropItem(OB->Index,AA);
						DropDownItem(OB,0,Data.AblSerial,Data.x,Data.y,100);
						break;
					}
				}
			}
		}
	}

}
void vui_IS_Abl_Click::SynchroAction(){
	
	addrand(Data.NI);
	addrand(Data.NIndex);
	addrand(Data.AblID);
	addrand(Data.AblSerial);
	addrand(Data.Right);

	itr_GetSelected.Create(Data.NI);
	if(Data.AblID==-1){
		// персональная абилка - поиск по серийному номеру
		while(OneObject* OB=itr_GetSelected.Next()){
			if(OB&&OB->ActiveAbility){
				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					if(AA->Serial==Data.AblSerial){
						addrand(Data.AblSerial);
						if(Data.Right){
							AA->OnRightClick();
						}else{
							AA->OnClick();
							ext_OnUseItem(OB->Index,AA);
						}
						break;
					}
				}
			}
		}
	}else{
		while(OneObject* OB=itr_GetSelected.Next()){
			if(OB&&OB->ActiveAbility){

				bool Hero=GetHero(OB);
				addrand(Hero);
				
				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					if(AA->TypeAbil!=0){
						UnitAbility* UA=AA->GetA();
						if( UA && UA->Index==Data.AblID && (AA->Serial==Data.AblSerial||!Hero) ){
							addrand(Data.AblID);
							addrand(Data.Right);
							if(Data.Right){
								AA->OnRightClick();
							}else{
								AA->OnClick();
								ext_OnUseItem(OB->Index,AA);
							}
							break;
						}
					}
				}
			}
		}
	}

}
void vui_IS_Abl_ClothedSlot::SynchroAction(){

	addrand(Data.NI);
	addrand(Data.NIndex);
	addrand(Data.AblID);
	addrand(Data.AblSerial);
	addrand(Data.Clothed);
	addrand(Data.Slot);

	itr_GetSelected.Create(Data.NI);
	if(Data.AblID==-1){
		// персональная абилка - поиск по серийному номеру
		while(OneObject* OB=itr_GetSelected.Next()){
			if(OB&&OB->ActiveAbility){
				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					if(AA->Serial==Data.AblSerial){
						addrand(Data.AblSerial);
						AA->SetClothe(Data.Clothed,Data.Slot);
						break;
					}
				}
			}
		}
	}else{
		while(OneObject* OB=itr_GetSelected.Next()){
			if(OB&&OB->ActiveAbility){
				bool Hero=GetHero(OB);
				addrand(Hero);
				itr_AA.Create(OB);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					if(AA->TypeAbil!=0){
						UnitAbility* UA=AA->GetA();
						if( UA && UA->Index==Data.AblID && (AA->Serial==Data.AblSerial||!Hero) ){
							addrand(Data.AblID);
							addrand(Data.Clothed);
							AA->SetClothe(Data.Clothed,Data.Slot);
							break;
						}
					}
				}
			}
		}
	}
}
void vui_IS_SelFearDown::SynchroAction(){
	int N=NSL[Data.NI];
	word* IDS=Selm[Data.NI];
	word* SNS=SerN[Data.NI];
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				OB->Morale=0;
				if(OB->BrigadeID!=0xFFFF){
					Brigade* BR=&(CITY[Data.NI].Brigs[OB->BrigadeID]);
					if(BR->Enabled&&BR->WarType!=0)
					{
						BR->Morale=0;
					}
				}
			}
		}
	}
}
void vui_IS_SelRTF::SynchroAction(){
	int N=NSL[Data.NI];
	word* IDS=Selm[Data.NI];
	word* SNS=SerN[Data.NI];
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				if(OB->BrigadeID!=0xFFFF){
					Brigade* BR=&(CITY[Data.NI].Brigs[OB->BrigadeID]);
					if(BR->Enabled&&BR->WarType!=0){
						BR->AttEnm=Data.State;
					}
				}
				OB->NewState=Data.State;
			}
		}			
	}
}
void vui_IS_SelChargeAll::SynchroAction(){
	int N=NSL[Data.NI];
	word* IDS=Selm[Data.NI];
	word* SNS=SerN[Data.NI];
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				OB->delay=0;
			}
		}
	}
}
void vui_IS_BrigHoldPosit::SynchroAction(){
	City* C=CITY+Data.NI;
	for(int i=0;i<C->NBrigs;i++){
		Brigade* BR=C->Brigs+i;
		if(BR->Enabled&&BR->WarType){
			bool sel=false;
			for(int bp=0;bp<BR->NMemb;bp++){
				word ID=BR->Memb[bp];
				if(ID!=0xFFFF){
					OneObject* OB=Group[ID];
					if(OB&&OB->Serial==BR->MembSN[bp]&&!OB->Sdoxlo&&OB->Selected){
						sel=true;
					}
				}
			}
			if(sel){
				BR->ClearBOrders();
				BR->BrigDelay=0;
				if(!BR->BrigDelay) MakeStandGround(BR);
			}
		}
	}
}
void vui_IS_SelRifle::SynchroAction(){
	int N=NSL[Data.NI];
	word* IDS=Selm[Data.NI];
	word* SNS=SerN[Data.NI];
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&OB->RifleAttack!=Data.State){
				if(GetRND(100)<20) OB->RifleAttack=Data.State;
			}
		}
	}
}
void vui_IS_LeaveMineAll::SynchroAction(){
	OneObject* OB=Group[Data.ObjID];
	if(OB&&OB->Serial==Data.ObjSN){
		OB->LeaveMine(0xFFFF);
	}
}
bool ComItemPickUp(OneObject* Hero, OneObject* Item);
void vui_IS_ItemPickUp::SynchroAction(){
	OneObject* Hero=Group[Data.HeroID];
	OneObject* Item=Group[Data.ItemID];
	if (Hero && Item && Data.HeroSN==Hero->Serial && Data.ItemSN==Item->Serial){
		ComItemPickUp(Hero,Item);
	}
}
bool ComBonusPickUp(byte NI, OneObject* Hero, OneObject* Bonus);
void vui_IS_BonusPickUp::SynchroAction(){
	OneObject* Hero=Group[Data.HeroID];
	OneObject* Bonus=Group[Data.BonusID];
	if (Hero && Bonus && Data.HeroSN==Hero->Serial && Data.BonusSN==Bonus->Serial){
		ComBonusPickUp(Data.NI,Hero,Bonus);
	}
}

vui_IS_AbilityTrade::vui_IS_AbilityTrade(){
};
vui_IS_AbilityTrade::vui_IS_AbilityTrade(OneObject* Seller, OneObject* Client, ActiveUnitAbility* AA, byte NI, int CashAmount){
	Data.SellerID=Seller->Index;
	Data.SellerSN=Seller->Serial;
	Data.ClientID=Client->Index;
	Data.ClientSN=Client->Serial;
	Data.AblSerial=AA->Serial;
	Data.CashNI=NI;
	Data.Cash=CashAmount;
}

ActiveUnitAbility* AddAbility(UnitAbility* Source, OneObject* Dest, int TypeAbil=2);
bool DetectMagazine(OneObject* OB);
void EventReceiveItem(OneObject* Acceptor, OneObject* ItemOB, ActiveUnitAbility* AA, char* Msg);

void vui_IS_AbilityTrade::SynchroAction(){

	OneObject* Seller=Group[Data.SellerID];
	OneObject* Client=Group[Data.ClientID];

	if (Seller && Client && Data.SellerSN==Seller->Serial && Data.ClientSN==Client->Serial) {
		ActiveUnitAbility* SellAA=NULL;
		itr_AA.Create(Seller);
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			if(AA->Serial==Data.AblSerial){
				int res=XRESRC(Data.CashNI,MoneyID);
				if( res+Data.Cash>=0 && !AA->Deleted ){
					SellAA=AA;
					break;
				}
			}
		}
		if(SellAA){
			if(DetectMagazine(Client)){
				SellAA->Deleted=true;
				ext_OnSellItem(Data.ClientID,Data.SellerID,SellAA);
			}else{
				UnitAbility* BuyUA=SellAA->GetA();
				if( BuyUA->Amount && !(SellAA->TypeAbil==4||SellAA->TypeAbil==5) ){
					itr_AA.Create(Client);
					while(ActiveUnitAbility* AA=itr_AA.Next()){
						if( BuyUA==AA->GetA() ){
							AA->Amount+=1;//SellAA->Amount;
							BuyUA=NULL;
							break;
						}
					}
				}
				if(BuyUA){
					ActiveUnitAbility* BuyAA=AddAbility(BuyUA,Client,SellAA->TypeAbil);
					if(BuyAA){
						BuyAA->Amount=1;
						BuyAA->StartCoolDown();
						static char* msg=GetTextByID("#GainMessageBuy");
						EventReceiveItem(Client,Seller,BuyAA,msg);
						ext_OnBuyItem(Data.ClientID,Data.SellerID,BuyAA);
					}
				}
				//UnitAbility* UA=AA->GetA();
				//if(UA){
				//	// информационное сообщение
				//	static _str h;
				//	h.Clear();
				//	static char* msg=GetTextByID("#GainMessage");
				//	//h.print("%s %s",msg,AA->GetHint());
				//	h.print("%s %s",msg,UA->HintName);
				//	AssignHint1(h.pchar(),100);

				//	// визуальная информация
				//	WeaponModificator* wG=v_ISys.eGain.Get();
				//	if(wG){
				//		void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);

				//		//word UnitIndex=OB->Index;
				//		int sx=Seller->RealX>>4;
				//		int sy=Seller->RealY>>4;
				//		int sz=Seller->RZ+Seller->OverEarth;

				//		int dx=Client->RealX>>4;
				//		int dy=Client->RealY>>4;
				//		int dz=Client->RZ+Client->OverEarth;

				//		CreateNewActiveWeapon(wG->Name.pchar(),Seller->Index,sx,sy,sz,Client->Index,dx,dy,dz,NULL);
				//	}
				//}
			}
			AddXRESRC(Data.CashNI,MoneyID,Data.Cash);
			// hack begin
			// в режиме RTS обнулить герою ресурсы
			UnitAbility* UA=SellAA->GetA();
			if( UA && UA->SellAfterUse && Data.Cash>0 ){
				for(int i=0;i<6;i++){
					SetXRESRC(SellAA->OB->NNUM,i,0);
				}
			}
			// hack end
		}
	}
}

void vui_IS_ExitMagazine::SynchroAction(){
	OneObject* Hero=Group[Data.HeroID];
	if( Hero && Hero->Serial==Data.HeroSN && Hero->ActiveAbility ){
		Hero->ActiveAbility->LibID=0xFFFF;
	}
};

void ExitMagazine(byte NI){
	pSelected Sel(/*NI*/MyNation);
	int n=Sel._getamount();
	for(int i=0;i<n;i++){
		OneObject* sOB=Sel.object(i);
		if( sOB && sOB->ActiveAbility ){
			sOB->ActiveAbility->LibID=0xFFFF;
		}
	}
	IGI.Inventory=false;
	IGI.Library=false;
}