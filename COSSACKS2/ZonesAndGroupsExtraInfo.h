#ifndef __ZonesAndGroupsExtraInfo_h__
#define __ZonesAndGroupsExtraInfo_h__
#include <ClassEngine.h>
#include <more_types.h>
// vital
#define		_ZONE_EX_INFO_CAPTURABLE_ 20
//VITYA
#define		_CZM_ARS_LV		102	//FOR MISSION AND STRATEGY AI	( VITYA )  ATTACKER CREATE SIDE
#define		_CZM_DRS_LV		103 //FOR MISSION AND STRATEGY AI	( VITYA )  DEFENDER CREATE SIDE
#define		_CAMP_LV		104 //FOR MISSION AND STRATEGY AI	( VITYA )  CREATE CAMP
#define		_STRAT_ZONE_LV	105 //FOR MISSION                	( GREY [03.03.2004]  STRATEGIC DEFEND ZONE
#define		_TOWER_ZONE_LV	106 //FOR MISSION                	( GREY [04.08.2004]  ZONE FOR BUILD TOWERS&BLOKHAUSES
//Alexander
#define AI_IMPORTANT_POINT 500
//-----
class DIALOGS_API ZoneExInfo:public BaseClass{
public:
	DWORD InfID;
	ZoneExInfo(){
		InfID=0;
	}
	SAVE(ZoneExInfo)
	ENDSAVE
};
class DIALOGS_API GroupExInfo:public BaseClass{
public:
	DWORD InfID;
	GroupExInfo(){
		InfID=0;
	}
	SAVE(GroupExInfo)
	ENDSAVE
};

DIALOGS_API GroupExInfo* GetExGroupInfo(char* groupID,DWORD ID,int Index);
DIALOGS_API ZoneExInfo* GetExZoneInfo(char* zoneID,DWORD ID,int Index);
DIALOGS_API void* GetGroupHandle(char* groupID);
DIALOGS_API void* GetZoneHandle(char* zoneID);
DIALOGS_API void* GetGroupHandle(int Index);
DIALOGS_API void* GetZoneHandle(int Index);
DIALOGS_API GroupExInfo* GetExGroupInfo(void* groupID,DWORD ID,int Index);
DIALOGS_API ZoneExInfo*  GetExZoneInfo (void* zoneID,DWORD ID,int Index);

DIALOGS_API bool AddExGroupInfo(char* Name, GroupExInfo* Info);

// sample
class DIALOGS_API StartPoint:public ZoneExInfo{
public:
	StartPoint(){
		InfID=1;
	}
	SAVE(StartPoint)
	REG_PARENT(ZoneExInfo)
	ENDSAVE;
};

// working
DIALOGS_API void VitalError(char*);
class DIALOGS_API BaseResources:public BaseClass{
public:
	int Wood;
	int Gold;
	int Stone;
	int Food;	
	int Iron;
	int Coal;
	int& operator [] (int index){
		switch(index){
			case 0: return Wood;
			case 1: return Gold;
			case 2: return Stone;
			case 3: return Food;
			case 4: return Iron;
			case 5: return Coal;
		};
		VitalError("BaseResources[] not valid");		
		return Wood;
	}

	bool PlayerHasMore(byte Nat);
	bool PlayerHasMorePerc(byte Nat, byte Percentage);
	void GetCostString(DString& txt);

	SAVE(BaseResources)
	REG_MEMBER(_int,Wood);
	REG_MEMBER(_int,Food);
	REG_MEMBER(_int,Stone);
	REG_MEMBER(_int,Gold);
	REG_MEMBER(_int,Iron);
	REG_MEMBER(_int,Coal);
	ENDSAVE
};
//
class DIALOGS_API AiImportantPoint:public ZoneExInfo{
public:
	AiImportantPoint(){
		InfID=AI_IMPORTANT_POINT;
		Extended=false;
	}
	int		Amount;
	bool	Extended;
	SAVE(AiImportantPoint)
		REG_PARENT(ZoneExInfo)
		REG_MEMBER(_int,Amount);
		REG_MEMBER(_bool,Extended);
	ENDSAVE;
};
//
//VITYA
#include "Dialogs\MisMotorAddData.hpp"

#endif //__ZonesAndGroupsExtraInfo_h__