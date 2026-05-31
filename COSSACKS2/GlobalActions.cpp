#include "stdheader.h"
#include "CurrentMapOptions.h"
#include "SuperEditor.h"
#include ".\vui_GlobalHotKey.h"

//////////////////////////////////////////////////////////////////////////
int SetFillDelay(int KillerID, int VictimID, int Damage);
typedef tpDamageCallback(int KillerIndex,int VictimIndex,int Value);
CEXPORT tpDamageCallback* SetDamageCallback(tpDamageCallback* dc);
bool GetGlobalBrigProp(byte NI);
DLLEXPORT int GetRND(int);
void SetBrigFillDelay(OneObject* Victim);
void OnMakeDamageProcess(OneObject* Sender,OneObject* Take,int& Damage);
//////////////////////////////////////////////////////////////////////////
extern Musorozbornik MusorStorage;
//////////////////////////////////////////////////////////////////////////

bool LoadAbilities(char* FileName);

void SetMonstersInCells();
void ACTION_AfterLoadingMapInEditor(){
	///LoadAbilities("UnitsAbility\\base.ua.xml");
	SetMonstersInCells();
}
void ACTION_AfterLoadingMapInGame(){
	//LoadAbilities("UnitsAbility\\base.ua.xml");
	VictoryConditionRoot* VCond=MOptions.VictoryRule.Get();
	if(VCond){
		VCond->InitOnCurrentMap();
	}
	SetMonstersInCells();
}
void ACTION_NewMapInEditor(){
	//LoadAbilities("UnitsAbility\\base.ua.xml");
}
void ACTION_AfterLoadingSaveFile(){

}
void ACTION_UnLoadingMap(){
	MusorStorage.Clear();
}
void ACTION_UnLoadingAllGame(){
	VictoryConditionRoot* VCond=MOptions.VictoryRule.Get();
	if(VCond){
		VCond->Reset();
	}
}
void ACTION_WhenGameStarts(){
	//SetDamageCallback(SetFillDelay);
	MOptions.OnMissionStart.Process();
}
void ACTION_WhenGameEnds(){

}
extern bool InEdit;
OneScript* ActionScript=NULL;
void TestCGARB();
void UnitAbilityTest();
void ACTION_ProcessingGame(){
	//UnitAbilityTest();
	int i=REALTIME&7;
	if(/*NATIONS[i].AI_Enabled&&*/GetRND(100)>80){
		GetGlobalBrigProp(i);
	}
	if(!InEdit){
		VictoryConditionRoot* VCond=MOptions.VictoryRule.Get();	
		if(VCond&&i==3){
			VCond->CheckVictoryAndDefeat();
		}
	}
	//CREATE_SCRIPT();		//Vitya //  [11/4/2003] // comment by grey [17.03.2004]
	//UnitAbilityTest();
	if(i==0) MusorStorage.ClearMusorProcess();
	//UnitAbilityTest();
	if(ActionScript) ActionScript->Script.Execute();
	//UnitAbilityTest();
	void ProcessWeaponSystem();
	ProcessWeaponSystem();
	//UnitAbilityTest();
	void UnitAbilityProcess();
	UnitAbilityProcess();
	//UnitAbilityTest();
	void BrigadeAlarmEvent();
	BrigadeAlarmEvent();
	//UnitAbilityTest();
	void CheckIfOneObjectHaveLongOrder();
	///CheckIfOneObjectHaveLongOrder();
}
void ACTION_HandlingControls(){

}
void HeroesGatherExperience(OneObject* Victim,word Killer);
void DieSounds(OneObject* OB);
bool ACTION_OnUnitDie(OneObject* OB,word KillerID){
	if(KillerID!=0xFFFF){
		HeroesGatherExperience(OB,KillerID);
		//DieSounds(OB);
		// info for statistic
		OneObject* KOB=Group[KillerID];
		if(KOB){
			Nation* KNT=NATIONS+KOB->NNUM;
			KNT->NErased[OB->NIndex]++;
		}	
	}
	return false;
}
bool ACTION_OnUnitDamage(OneObject* Sender,OneObject* Take,int& Damage){
	OnMakeDamageProcess(Sender,Take,Damage);
	SetBrigFillDelay(Take);
	return true;
}
//void IfHiroBirth(OneObject* OB);
void ACTION_OnUnitBirth(OneObject* OB){
	//IfHiroBirth(OB);
}
//stage:
//0  - Only land drawn
//30 - land,sprites,units,weapon drawn
//40 - land,sprites,units,weapon,smoke drawn
//50 - land,sprites,units,weapon,smoke,fogofwar drawn
//60 - land,sprites,units,weapon,smoke,fogofwar,minimap drawn
//70 - land,sprites,units,weapon,smoke,fogofwar,minimap,units interface drawn
//80 - land,sprites,units,weapon,smoke,fogofwar,minimap,units interface,extra interface drawn
void ACTION_ProcessingDrawing(int stage){
	if(stage==30){
		v_GlobalHotKeys.DrawHelp();
	//}else if(stage==70){
		void vdf_BGI_Draw();
		vdf_BGI_Draw();
	}
}
extern int HeightEditMode;
extern short WheelDelta;
void OnMouseMove(){
	extern SprGroup* CURSG;
	extern SprGroup WALLS;
	if(HeightEditMode==2&&CURSG==&WALLS&&!Lpressed){
		static int csx;
		static int csy;
		static int asx;
		static int asy;
		bool cp=GetKeyState(VK_CONTROL)&0x8000;
		bool ap=GetKeyState(VK_MENU)&0x8000;
		if(cp&&!ap){
			//ObjCharacter::RotX+=float(mouseX-csx)/100.0f;
			//ObjCharacter::RotY+=float(mouseY-csy)/100.0f;
			extern bool realRpressed;
			if(realRpressed){
				//ObjCharacter::RotX=0;
				//ObjCharacter::RotY=0;
			}
			SetCursorPos(csx,csy);
			mouseX=csx;
			mouseY=csy;
		}
		if(ap){
			//ObjCharacter::Scale+=float(mouseX-asx)/1000.0f;
			//ObjCharacter::ModelDZ-=float(mouseY-asy)/2.0f;
			SetCursorPos(asx,asy);
			mouseX=asx;
			mouseY=asy;
		}

		csx=mouseX;
		csy=mouseY;
		asx=mouseX;
		asy=mouseY;		
	}
}