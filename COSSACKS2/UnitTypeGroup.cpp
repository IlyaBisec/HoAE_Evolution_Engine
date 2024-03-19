#include "stdheader.h"
//----------------------------------------------------------------------------------------------------------------//
#include "unitability.h"
#include "UnitTypeGroup.h"
//----------------------------------------------------------------------------------------------------------------//
const char* TypeGroup::GetThisElementView(const char* LocalName){
	if(Name.pchar()){
		static char cc[256];
		sprintf(cc,"%s: {CW}%s{C}",LocalName,Name.pchar());
		return cc;
	}else return LocalName;
}
//----------------------------------------------------------------------------------------------------------------//
int TypeGroupList::GetExpansionRules()
{
	return 1;
}
//----------------------------------------------------------------------------------------------------------------//
bool UnitTypeGroup::GetNewUnitType(int OldUnitType, byte NewNationID, int& NewUnitType){
	int n=Groups.GetAmount();
	for(int i=0;i<n;i++){
		TypeGroup* TG=Groups[i];
		int nt=TG->UnitList.GetAmount();
		for(int j=0;j<nt;j++){
			if(TG->UnitList[j]->UnitType==OldUnitType){
				for(int k=0;k<nt;k++){
					int NT=TG->UnitList[k]->UnitType;

					int nat=NATIONS[0].Mon[NT]->NatID;
					int ConvertNationToSkirmish(int SkirmisID);
					nat=ConvertNationToSkirmish(nat);

					if(nat==NewNationID){
						NewUnitType=NT;
						return true;
					}
				}
			}
		}
	}
	return false;
}
bool UnitTypeGroup::GetHeroUnitType(int RoleID, byte NationID, int& NewUnitType){
	if(RoleID==0){
		// потенциальный глюк
		RoleID=(int(rando())* 3 )>>15;
	}else{
		RoleID--;
	}
	int role=0;
	int n=Groups.GetAmount();
	for(int i=0;i<n;i++){
		TypeGroup* TG=Groups[i];
		if(TG->Hero){
			if(role==RoleID){
				int nt=TG->UnitList.GetAmount();
				for(int j=0;j<nt;j++){
					int NT=TG->UnitList[j]->UnitType;

					int nat=NATIONS[0].Mon[NT]->NatID;
					int ConvertNationToSkirmish(int SkirmisID);
					nat=ConvertNationToSkirmish(nat);

					if(nat==NationID){
						NewUnitType=NT;
						return true;
					}
				}
			}else{
				role++;
			}
		}
	}
	return false;
}
void UnitTypeGroup::CreateAllHeroEnum(EnumeratorOutputStream* out){
	int n=Groups.GetAmount();
	for(int i=0;i<n;i++){
		TypeGroup* TG=Groups[i];
		if(TG->Hero){
			int nt=TG->UnitList.GetAmount();
			for(int j=0;j<nt;j++){
				int NT=TG->UnitList[j]->UnitType;
				//int nat=NATIONS[0].Mon[NT]->NatID;
				//int ConvertNationToSkirmish(int SkirmisID);
				//nat=ConvertNationToSkirmish(nat);
				//if(nat==NationID){
				//	NewUnitType=NT;
				//	return true;
				//}
				out->add(NATIONS[0].Mon[NT]->Message,NT,NULL);
			}
		}
	}
};
word UnitTypeGroup::GetType(word Index, byte NationID, bool Hero){
	int n=Groups.GetAmount();
	if(Index<n) for(int i=0;i<n;i++){
		int nt=Groups[i]->UnitList.GetAmount();
		if(!Hero||Groups[i]->Hero){ //for(int j=0;j<nt;j++)
			 for(int k=0;k<nt;k++){
				int NT=Groups[i]->UnitList[k]->UnitType;
				if(NATIONS[0].Mon[NT]->NatID==NationID){
					if(Index==0){
						return NT;
					}
					Index--;
					break;
				}
			}
		}
	}
	return 0xFFFF;
};
//----------------------------------------------------------------------------------------------------------------//
UnitTypeGroup UnitTypeGroups;
//----------------------------------------------------------------------------------------------------------------//
bool GetHeroUnitType(int RoleID, byte NationID, int& NewUnitType){
	return UnitTypeGroups.GetHeroUnitType( RoleID, NationID, NewUnitType );
}
bool GetNewUnitType(int OldUnitType,byte NewNationID, int& NewUnitType)
{
	return UnitTypeGroups.GetNewUnitType( OldUnitType, NewNationID, NewUnitType );
}
//----------------------------------------------------------------------------------------------------------------//
void LoadUnitTypeGroups(){
	UnitTypeGroups.reset_class(&UnitTypeGroups);
	UnitTypeGroups.SafeReadFromFile("dialogs\\UnitTypeGroups.xml");
}