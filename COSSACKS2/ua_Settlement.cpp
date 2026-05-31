#include "stdheader.h"
#include "BE_HEADERS.h"
#include "ua_Settlement.h"
#include ".\cvi_MainMenu.h"
#include "UnitChar.h"

uaa_Settlement* DetectSettlement(OneObject* Settlement){
	itr_AA.Create(Settlement);
	while(ActiveUnitAbility* AA=itr_AA.Next()){
		uaa_Settlement* S=dynamic_cast<uaa_Settlement*>(AA);
		if(S){
			return S;
		}
	}
	return NULL;
}
bool SetHeroToSettlement(OneObject* Hero,OneObject* Settlement){
	if(Hero){
		uaa_Settlement* S=DetectSettlement(Settlement);
		if(S){
			//int dist=Norma(Hero->RealX-Settlement->RealX,Hero->RealY-Settlement->RealY)>>4;
			//if(dist<250){
				S->HeroID=Hero->Index;
				S->HeroSN=Hero->Serial;
				return true;
			//}
		}
	}
	return false;
}

ActiveUnitAbility* ua_Settlement::GetNewActiveAbility(){
	uaa_Settlement* nAA=new uaa_Settlement;
	nAA->SetA(this);
	//
	//nAA->ItemDrop=ItemDrop;
	//
	return nAA;
}
ActiveUnitAbility* ua_BeMsg::GetNewActiveAbility(){
	uaa_BeMsg* nAA=new uaa_BeMsg;
	nAA->SetA(this);
	//
	//nAA->ItemDrop=ItemDrop;
	//
	return nAA;
}

bool uaa_BeMsg::OnClick(){

	addrand(Serial);
	addrand(OB->Index);

	if(DriveMode()->PROCESS){
		addrand(2);
		be_LeftClick.CallFunction((ActiveUnitAbility*)this);
		if(be_Dialog){
			addrand(3);
			be_Dialog->be_LeftClick.CallFunction((ActiveUnitAbility*)this);
		}
	}

	return true;
};
bool uaa_BeMsg::Process(){

	UnitAbility* UA=GetA();
	if(UA){
	}

	if(DriveMode()->PROCESS){
		be_Process.CallFunction((ActiveUnitAbility*)this);
		if(be_Dialog){
			be_Dialog->be_Process.CallFunction((ActiveUnitAbility*)this);
		}
	}

	addrand(OB->NewBuilding);
	addrand(OB->ActiveAbility->LibID);
	addrand(Deleted);

	if(!OB->NewBuilding){
		return OB->ActiveAbility->LibID!=0xFFFF && !Deleted;
	}
	return !Deleted;
};
const char* uaa_BeMsg::GetHint(){
	return Hint.pchar();
}