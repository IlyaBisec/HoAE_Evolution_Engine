//==================================================================================================================//
#include "stdheader.h"
//==================================================================================================================//
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
//
bool isUnitIn(OneObject* OB,void* param) {
	if(OB && !OB->Sdoxlo && !OB->NewBuilding){
		((DynArray<OneObject*>*)param)->Add(OB);
	}
	return true;
}

//функция возвращает массив указателей на юнитов, находящихся в заданном радиусе
void findUnitInRadius(OneObject* OB, int R, DynArray<OneObject*>* num){
	PerformActionOverUnitsInRadius(OB->RealX>>4, OB->RealY>>4, R, &isUnitIn, (void*)(num));
}

//ищет ближайшую цель, если он кого то бьет - то не ищет.
void findClosestTarget(OneObject* OB){
	OB->NoSearchVictim = true;
	bool search = false;
	//определяем, нужно искать цель или нет
	if(OB->EnemyID == 0xFFFF){
		search = true;
	} else {
		OneObject* EN = Group[OB->EnemyID];
		if(!EN || EN->Sdoxlo) search = true;
	}
	if(search){
		//нужно загрузить всех врагов в каком то радиусе
		DynArray<OneObject*> Enemy;
		findUnitInRadius(OB, 1500, &Enemy);

		int enemyIndex = -1; //индекс цели
		int enemyValue = -1; //характеризующая величина, по значению которой и производится отбор
		//теперь среди них нужно найти самого близкого
		for(int i=0;i<Enemy.GetAmount();i++) {
			OneObject* O = Enemy[i];
			if(OB->NNUM != O->NNUM){
				int distance = int(sqrt((OB->RealX - O->RealX)^2 + (OB->RealY - O->RealY)^2));
				if(enemyValue == -1 ||
				   enemyValue > distance){
					enemyValue = distance;
					enemyIndex = i;
				}
			}
		}
		if(enemyIndex != -1){
			//цель найдена, устанавливаем ее
			OB->AttackObj(Enemy[enemyIndex]->Index, 128+16);
		}
	}
}

//очищает стэк команд юнита
void clearAllAction(OneObject* OB){
	if(OB->EnemyID != 0xFFFF){
		OB->NoSearchVictim = true;
		OB->ClearOrders();
	}
}