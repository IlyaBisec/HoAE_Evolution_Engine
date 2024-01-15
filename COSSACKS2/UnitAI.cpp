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

//������� ���������� ������ ���������� �� ������, ����������� � �������� �������
void findUnitInRadius(OneObject* OB, int R, DynArray<OneObject*>* num){
	PerformActionOverUnitsInRadius(OB->RealX>>4, OB->RealY>>4, R, &isUnitIn, (void*)(num));
}

//���� ��������� ����, ���� �� ���� �� ���� - �� �� ����.
void findClosestTarget(OneObject* OB){
	OB->NoSearchVictim = true;
	bool search = false;
	//����������, ����� ������ ���� ��� ���
	if(OB->EnemyID == 0xFFFF){
		search = true;
	} else {
		OneObject* EN = Group[OB->EnemyID];
		if(!EN || EN->Sdoxlo) search = true;
	}
	if(search){
		//����� ��������� ���� ������ � ����� �� �������
		DynArray<OneObject*> Enemy;
		findUnitInRadius(OB, 1500, &Enemy);

		int enemyIndex = -1; //������ ����
		int enemyValue = -1; //��������������� ��������, �� �������� ������� � ������������ �����
		//������ ����� ��� ����� ����� ������ ��������
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
			//���� �������, ������������� ��
			OB->AttackObj(Enemy[enemyIndex]->Index, 128+16);
		}
	}
}

//������� ���� ������ �����
void clearAllAction(OneObject* OB){
	if(OB->EnemyID != 0xFFFF){
		OB->NoSearchVictim = true;
		OB->ClearOrders();
	}
}