//==================================================================================================================//
#include "stdheader.h"
//==================================================================================================================//
#define MAX_DEFENCE_TYPES 4
class Combat {
public:
	Combat(){
	}

private:
	class unitDesc:public BaseClass{
	public:
		unitDesc(char *iName, int iId, int iArmyId, int iMaxLife, int iLife,
			int iAspeed, int iAtackType, int iDamage, int iDef[]){
				//��������� ������� ����������
				Name = iName;
				Id = iId;
				ArmyId = iArmyId;
				MaxLife = iMaxLife;
				Life = iLife;
				Aspeed = iAspeed;
				AtackType = iAtackType;
				Damage = iDamage;
				for(int i=0;i<MAX_DEFENCE_TYPES;i++){
					Def[i] = iDef[i];
				}

				//��������� ��������� ����������
				CurLife = Life;
				LastHit = 0;
				isDead = 0; //�� ��������� ���� ��������� �����.
				Target = NULL; //�� ��������� ���� ������ �� �������.
			}
	
			//������� ��������� ������.
			char *Name;
			int Id;
			int ArmyId; //������������� �����.
			int MaxLife;//������������ ����� ������.
			int Life;	//��������� ����� �� ������ ������ �����
			int Aspeed; //���������� ������� ��� ����� ����� �������. ��������, �������� �������� ������.
			int AtackType;
			int Damage;
			int Def[MAX_DEFENCE_TYPES]; 

			//��������� ��������� ������, ������� ��� �� ����� ����� ��� ������������� ��������
			int CurLife; //������� �����.
			int LastHit; //���������� �������, ��������� ����� ���������� �����.
			int isDead;  //����� ������ ��� ���.
			unitDesc *Target;  //������ �� ���� �����.
	};

	DynArray<unitDesc*> A; //������, ��� ���������� ��� ����� � ��������.

	//�������� ������� ��� ������
public:
	//���������� ����� � �����
	int AddUnit(char *iName, int iId, int iArmyId, int iMaxLife, int iLife,
		int iAspeed, int iAtackType, int iDamage, int iDef[]){
			unitDesc* u = new unitDesc(iName, iId, iArmyId, iMaxLife, iLife,
				iAspeed, iAtackType, iDamage, iDef);
			A.Add(u);
			return 0;
		}
		//�������
		int Flush(){
			while(A.GetAmount()>0){
				delete(A[0]);
				A.Del(0,1);
			}
			return 0;
		}

		//������������� ������ ������ �����, ����� ��� ��� � ����������� �������.
		int mixArmy(){
			DynArray<unitDesc*> B;
			srand((unsigned)time(NULL)); //������������� RND

			while(A.GetAmount()>0){
				int i = (int)(A.GetAmount()*rand()/32767);
				B.Add(A[i]);
				A.Del(i,1);
			}
			while(B.GetAmount()>0){
				A.Add(B[0]);
				B.Del(0,1);
			}
			return 0;
		}
		//�������, ������� ���������� ���������� �����.
		int numArmy() {
			DynArray<int> num;
			for(int i=0;i<A.GetAmount();i++){
				bool entry = false;
				for(int j=0;j<num.GetAmount();j++){
					if(A[i]->ArmyId == num[j]) {
						entry = true;
					}
				}
				if(!entry){
					num.Add(A[i]->ArmyId);
				}
			}
			int ret = num.GetAmount();
			num.Clear();
			return ret;
		}

		//�������, ������� ���������� ���������� ����� �����.
		int numArmyAlive() {
			DynArray<int> num;
			for(int i=0;i<A.GetAmount();i++){
				if(A[i]->isDead == 0) {
					bool entry = false;
					for(int j=0;j<num.GetAmount();j++){
						if(A[i]->ArmyId == num[j]) {
							entry = true;
						}
					}
					if(!entry){
						num.Add(A[i]->ArmyId);
					}
				}
			}
			int ret = num.GetAmount();
			num.Clear();
			return ret;
		}

		//��������� �������� 1 ������ ��� ����� ������. ���������� ������ �����
		int cmbUnit(int index){
			//����� ����, ���� �� ��� ���
			if(A[index]->Target == NULL) {
				//����� �������� ���� ����� ������ � ����� B
				DynArray<int> B;
				for(int j=0;j<A.GetAmount();j++){
					if(A[j]->ArmyId != A[index]->ArmyId &&
						A[j]->CurLife > 0){
							B.Add(j);
						}
				}
				//��������� ������� �������� ���� ����� ����.
				if(B.GetAmount()>0) {
					int victim = (int)((rand()*B.GetAmount())/32767);
					if (victim = B.GetAmount()) victim--;
					A[index]->Target = A[B[victim]];
				}
			}
			if(A[index]->Target != NULL) { // ���� ���� ����� ����.
				if(A[index]->LastHit >= A[index]->Aspeed) { //�������� ������ �� ��� ����� �������� ���� ��� ���.
					//����� ������, ��������� ������� �� ����� ����� � ������� ����������.
					A[index]->LastHit -= A[index]->Aspeed;
					A[index]->Target->CurLife -= A[index]->Damage - A[index]->Target->Def[A[index]->AtackType];

					if (A[index]->Target->CurLife <= 0) { //���� ���� ����
						unitDesc *dead = A[index]->Target;
						A[index]->Target->isDead = 1;
						//���� �������� ������� � ����, ��� ���� ��������.
						for(int j=0;j<A.GetAmount();j++) {
							if (A[j]->Target == dead) {
								A[j]->Target = NULL;
							}
						}
					}				
				} else {
					//����� �� ������, ����������� �������.
					A[index]->LastHit++;
				}
			}
			return 0;
		}

		//������������� �����
		int Fight(){
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//������ ������������� ��������.
			while(numArmyAlive() > 1){//��� ���� �������� �������, ������� ��������� ������� ����� ����� ��� ��������.
				for(int k=0;k<A.GetAmount();k++){
					if(A[k]->isDead == 0){
						cmbUnit(k);
					}
				}
			}

			return 0;
		}

		//��������� ���� ����� �� ������� ��� �� ��� �������.
		double cmpArmy() {
			DynArray<int> arm; //������, ���������� �������������� �����
			DynArray<float> power; //������, � ������� ����� ����������� ������������� ���� �����.

			//���������� ��� ����� � ������� �� � ���� ������.
			for(int i=0;i<A.GetAmount();i++){
				bool entry = false;
				for(int j=0;j<arm.GetAmount();j++){
					if(A[i]->ArmyId == arm[j]) {
						entry = true;
					}
				}
				if(!entry){
					arm.Add(A[i]->ArmyId);
					power.Add(-1); //���� ��� ������������� ������������� � -1, ����� ����� ����� ����������� ���� ��� ���
				}
			}

			//���� �� ���� ������ � ��������
			for(int i=0;i<arm.GetAmount();i++){
				//��� ������� ����� ������� �����
				float armyPower = 0;

				for(int j=0;j<A.GetAmount();j++) {	
					if(A[j]->ArmyId == arm[i]){
						float unitPower = 0;
						int unitQuan = 0; //����������� ������
						//�������������� ��� ���� ������������ ������� ����� ���������� �����
						for(int k=0;k<A.GetAmount();k++) {
							if(A[k]->ArmyId != arm[i]){
								//������� ���� ����� A[j] ������������ ����� A[k]
								//������ ��������� ������.
								int damage = 0;
								damage = A[j]->Damage - A[k]->Def[A[j]->AtackType];
								if (damage <= 0) damage = 1;

								//������ ��������� ���������� ������, ������� ��� ����� �������. ����� �����
								double numHit = 0;
								numHit = ceil(double(double(A[k]->Life)/damage));

								//���� ����� ����� �������� �������� ���������� ������, ���������� �� ����� ����� �������.
								unitPower += float(1/(numHit * A[j]->Aspeed));

								unitQuan++; //����������� ������� ������
							}
						}
						//� ����� ��� �������� �� ����� ���������� ������ � ���������� ����� �����
						armyPower += (float)unitPower/unitQuan;
					}
				}
				power[i] = armyPower;
			}
		};

		//����� ���� �����, ID ����� ����������, ���� ����� ������������.
		double cmpArmyN(int ArmyId) {
			float armyPower = 0;

			for(int j=0;j<A.GetAmount();j++) {	
				if(A[j]->ArmyId == ArmyId){
					float unitPower = 0;
					int unitQuan = 0; //����������� ������
					//�������������� ��� ���� ������������ ������� ����� ���������� �����
					for(int k=0;k<A.GetAmount();k++) {
						if(A[k]->ArmyId != ArmyId){
							//������� ���� ����� A[j] ������������ ����� A[k]
							//������ ��������� ������.
							int damage = 0;
							damage = A[j]->Damage - A[k]->Def[A[j]->AtackType];
							if (damage <= 0) damage = 1;

							//������ ��������� ���������� ������, ������� ��� ����� �������. ����� �����
							double numHit = 0;
							numHit = ceil(double(double(A[k]->Life)/damage));

							//���� ����� ����� �������� �������� ���������� ������, ���������� �� ����� ����� �������.
							unitPower += float(1/(numHit * A[j]->Aspeed));
							unitQuan++; //����������� ������� ������
						}
					}
					//� ����� ��� �������� �� ����� ���������� ������ � ���������� ����� �����
					armyPower += (float)unitPower/unitQuan;
				}
			}
			return armyPower;
		}
};
//==================================================================================================================//
Combat C;

float GetAttSpped(OneObject* OB,int AttType)
{
	float rez=-1;
	NewAnimation* NA=OB->newMons->GetFirstAnimation(anm_Attack+AttType);
	if(NA&&NA->NFrames)
	{
		int AF=max(NA->NFrames,int(OB->MoreCharacter->AttackPause[AttType]));
		//rez=GSets.SVOpt.RequiredMsPerFrame*AF*OB->MoreCharacter->AttackSpeed/1000;
		rez=float(GSets.SVOpt.RequiredMsPerFrame*AF*100)/1000.0f/float(OB->GetAttackSpeedBonus());
	}
	return rez;
}
int GetAttSpeed(OneObject* OB,int AttType){ // ���������� ������ �� �������
	int rez=0;
	NewAnimation* NA=OB->newMons->GetFirstAnimation(anm_Attack+AttType);
	if(NA&&NA->NFrames){
		int AF=NA->NFrames+int(OB->MoreCharacter->AttackPause[AttType]);
		//rez=60000/(GSets.SVOpt.RequiredMsPerFrame*AF*100/OB->GetAttackSpeedBonus());
		int ab=OB->GetAttackSpeedBonus();
		if(ab==0)ab=1;
		rez=60000/(30*AF*100/ab);
	}
	return rez;
}
int GetAttSpeedBase(OneObject* OB,int AttType){
	int rez=0;
	NewMonster* NM=OB->newMons;
	NewAnimation* NA=NM->GetFirstAnimation(anm_Attack+AttType);
	if(NA&&NA->NFrames){
		//int AF=max(NA->NFrames,int(OB->MoreCharacter->AttackPause[AttType]));
		int AF=NA->NFrames+int(OB->MoreCharacter->AttackPause[AttType]);
		int asp=OB->MoreCharacter->AttackSpeed;
		if(asp==0)asp=1;
		rez=60000/(30*AF*100/asp);
	}
	return rez;
}
void CombatSimulatorFillSelected()
{
	C.Flush();
	for(int i=0;i<8;i++)
	{
		if(NSL[i]>0)
		{
			for(int k=0;k<NSL[i];k++)
			{
				OneObject* OB=Group[Selm[i][k]];
				if(OB&&!OB->Sdoxlo)
				{
					int AttType=0;
					int Damage=0;
					for(int j=0;j<NAttTypes;j++)
					{
						if(OB->MoreCharacter->MaxDamage[j])
						{
							AttType=j;
							Damage=OB->MoreCharacter->MaxDamage[j];
							break;
						}
					}
					int Deff[MAX_DEFENCE_TYPES];
					for(int e=0;e<MAX_DEFENCE_TYPES;e++)
					{
						Deff[e]=OB->MoreCharacter->Protection[e]+OB->AddShield;
					}
					C.AddUnit(OB->newMons->Name,OB->Index,i,OB->MoreCharacter->Life,
						OB->Life,GetAttSpped(OB,AttType),AttType,Damage,Deff);	
				}
			}
		}
	}
}
void CombatSimulatorDrawResult()
{
	for(int i=0;i<8;i++)
	{
		if(NSL[i]>0)
		{
			char st[64];
			int xx=100;
			int yy=40;
			sprintf(st,"%d: %d - %f",i,NSL[i],C.cmpArmyN(i));
			ShowString(xx,yy+i*20,st,&YellowFont);
		}
	}
}