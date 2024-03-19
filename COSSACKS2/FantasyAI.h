//==================================================================================================================//
#include "stdheader.h"
#include "GameExtension.h"
#include "UnitAI.h"
//==================================================================================================================//
class UnitArray: public ClonesArray<UnitAI>{
public:
	virtual bool DelElement(int Index){
		OneObject* OB = (*this)[Index]->OB;
		OB->NoSearchVictim = false;	
		return ClonesArray<UnitAI>::DelElement(Index);
	}
	SAVE(UnitArray)
	ENDSAVE;
};
//базовый класс для работы с АИ. Содержит экстеншены для отдачи объекта под контроль АИ и для снятия с объекта контроля АИ.
class FantasyAI: public GameExtension {
public:
	bool isAI;

	UnitArray Units; //массив в котором хранятся все юниты
	void SetAIControl(OneObject* OB); //АИ устанавливает контроль над объектом
	void ClearAIControl(OneObject* OB); //снимает контроль АИ с текущего объекта
	void ClearAllAIControl(); //Снимает контроль АИ со всех объектов
	void ProcessUnitOrder(); //Раздает команды юнитам и бригадам.

public:
	//Собственно сама работа АИ
	virtual void ProcessingGame(){
		char st[64];
		sprintf(st,"Total units under AI control: %d",Units.GetAmount());
		ShowString(0,100,st,&YellowFont);

		if(isAI) {
		/*
			word* SMon=ImSelm[0];
			word* SN=ImSerN[0];
			int N=ImNSL[0];
			if(N){
				for(int i=0;i<N;i++){
					word MID=SMon[i];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
					}
				}
			}
		*/
			ProcessUnitOrder();
		}
	}

	//При рождении все юниты передаются под управление АИ
	virtual void OnUnitBirth(OneObject* NewUnit){
		if(isAI){
			SetAIControl(NewUnit);
		}
	}

	//умершие юниты убираются из под контроля АИ
	virtual bool OnUnitDie(OneObject* Dead,OneObject* Killer){
		ClearAIControl(Dead);
		return false;
	}

	virtual void OnGameEnd(){
		ClearAllAIControl();
	}

	//Коды для управления АИ
	virtual bool OnCheatEntering(const char* Cheat){
		if(!strcmp(Cheat,"FanAI")){
			isAI = true;
			AssignHint1("Fantasy AI started!",100,0);
		}
		if(!strcmp(Cheat,"noFanAI")){
			isAI = false;
			AssignHint1("Fantasy AI stoped!",100,0);
			ClearAllAIControl();
		}
		return false;
	}
	SAVE(FantasyAI)
		REG_MEMBER(_bool,isAI);
		REG_AUTO(Units);
	ENDSAVE
};