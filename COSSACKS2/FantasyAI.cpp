#include "stdheader.h"
#include "GameExtension.h"
#include "ClassEditorsRegistration.h"
#include "FantasyAI.h"
//==================================================================================================================//

void InstallFantasyAI() {
	FantasyAI* FAI = new FantasyAI;
	InstallExtension(FAI, "Fantasy AI");
	AddStdEditor("Fantasy AI Editor",FAI,"",RCE_RIGHT_POSITION|RCE_SHOW_GAME_BACKGROUND|
											RCE_ALLOW_GAME_PROCESSING|RCE_AUTOSAVE|
											RCE_EXITONESCAPE|RCE_HIDEHEADER);

	REG_CLASS(UnitAI);
	REG_CLASS(FantasyAI);
	REG_CLASS(UnitArray);
}

void FantasyAI::SetAIControl(OneObject* OB) {
	if(OB){
		OB->NoSearchVictim = true;
		OB->ClearOrders();
		UnitAI* u=new UnitAI;
		this->Units.Add(u);
		u->OB=OB;
		u->idAction=0;
		u->index=OB->Index;
		u->serial=OB->Serial;
	}
}

void FantasyAI::ClearAIControl(OneObject* OB) {
	if(OB){
		for(int i=0;i<Units.GetAmount();i++){
			OneObject* OO = this->Units[i]->OB;
			if(OO->Index == OB->Index &&
			   OO->Serial == OB->Serial) {
				this->Units[i]->OB->NoSearchVictim = false;
				this->Units.Del(i,1);
			}
		}
	}
}

void FantasyAI::ClearAllAIControl() {
	while(this->Units.GetAmount()>0){
		this->Units[0]->OB->NoSearchVictim = false;
		this->Units.Del(0,1);
	}
}

void FantasyAI::ProcessUnitOrder(){
	for(int i;i<this->Units.GetAmount();i++){
		//void findClosestTarget(OneObject* OB);
		//findClosestTarget(Units[i]->OB);
	}
}