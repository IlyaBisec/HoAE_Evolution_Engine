#pragma	once
//----------------------------------------------------------------------------------------------------------------//
class TypeGroup : public BaseClass
{
public:
	_str Name;
	bool Hero;
	UnitTypeList UnitList;
	SAVE(TypeGroup){
		REG_MEMBER(_str,Name);
		REG_MEMBER(_bool,Hero);		
		REG_AUTO(UnitList);
	}ENDSAVE;

	virtual const char* GetThisElementView(const char* LocalName);
};
//----------------------------------------------------------------------------------------------------------------//
class TypeGroupList:public ClassArray<TypeGroup>
{
public:
	int GetExpansionRules();
};
//----------------------------------------------------------------------------------------------------------------//
class UnitTypeGroup : public BaseClass
{
public:
	TypeGroupList Groups;
	SAVE(UnitTypeGroup)
		REG_AUTO(Groups);
	ENDSAVE;

	bool GetHeroUnitType(int RoleID, byte NationID, int& NewUnitType);
	bool GetNewUnitType(int OldUnitType,byte NewNationID, int& NewUnitType);
	
	word GetType(word Index, byte NationID, bool Hero=false);
	void CreateAllHeroEnum(EnumeratorOutputStream* out);
};
extern UnitTypeGroup UnitTypeGroups;
//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//