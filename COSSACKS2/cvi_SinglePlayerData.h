#pragma once
#include "MapTemplates.h"
#include ".\cvi_campaign.h"
#include "unitability.h"
//////////////////////////////////////////////////////////////////////////

class SinglePlayerData_HeroAbilityInfo : public BaseClass {
public:
	
	int Level;
	bool VirtualUp;
	int AbIndex;
	
	SAVE(SinglePlayerData_HeroAbilityInfo)
		REG_MEMBER(_int,Level);
		REG_MEMBER(_bool,VirtualUp);
		REG_MEMBER(_int,AbIndex);
	ENDSAVE;

	SinglePlayerData_HeroAbilityInfo();

};

class SinglePlayerData_HeroInfo : public BaseClass {
public:
	
	//int HeroType;
	_str HeroType;
	int Level;
	int Expa;
	bool WaitForAbilUp;
	ClassArray<SinglePlayerData_HeroAbilityInfo> Abilki;

	int Money;
	AdvCharacter MoreCharacter;
	ActiveAbilityListArray Abilities;
	ClonesArray<_str> AbilitiesName;

	SAVE(SinglePlayerData_HeroInfo){

		//REG_MEMBER(_int,HeroType);
		REG_AUTO(HeroType);
		REG_MEMBER(_int,Level);
		REG_MEMBER(_int,Expa);
		REG_MEMBER(_bool,WaitForAbilUp);
		REG_AUTO(Abilki);

		REG_MEMBER(_int,Money);
		REG_AUTO(MoreCharacter);
		REG_AUTO(Abilities);
		REG_AUTO(AbilitiesName);

	}ENDSAVE;

	SinglePlayerData_HeroInfo();

};
class SinglePlayerData_HeroesInfoList : public BaseClass
{
public:
	SinglePlayerData_HeroesInfoList();
	ClassArray<SinglePlayerData_HeroInfo> MissionHeroes;
	int FreeExp;

	SAVE(SinglePlayerData_HeroesInfoList){
		REG_AUTO(MissionHeroes);
		REG_MEMBER(_int,FreeExp);
	}ENDSAVE;
};
class SinglePlayerData_CampaignHeroInfo : public BaseClass
{
public:
	SinglePlayerData_CampaignHeroInfo();
	ClassArray<SinglePlayerData_HeroesInfoList> CampaignMissions;		

	SAVE(SinglePlayerData_CampaignHeroInfo){
		REG_AUTO(CampaignMissions);		
	}ENDSAVE;
};

class cvi_SinglePlayerData: public BaseClass
{
public:
	cvi_SinglePlayerData(void);
	~cvi_SinglePlayerData(void);

	LinearArray<int,_int> NDoneMission;
	_str PlayerName;
	int Difficulty;
	ClassArray<SinglePlayerData_CampaignHeroInfo> Heri;	
	ClonesArray< ClassRef<cvi_mfMovie> > Movies;
	bool CampMessageCheck;
	
	SAVE(cvi_SinglePlayerData){
		REG_AUTO(NDoneMission);
		REG_AUTO(PlayerName);
		REG_MEMBER(_int,Difficulty);
		REG_AUTO(Heri);		
		REG_AUTO(Movies);
		REG_MEMBER(_bool,CampMessageCheck);		
	}ENDSAVE;

};

class cvi_PlayerData: public BaseClass
{
public:

	ClonesArray<cvi_SinglePlayerData> Aka;
	int CurAkaID;
	_str PlayerNick;

	SAVE(cvi_PlayerData){
		REG_AUTO(Aka);
		REG_MEMBER(_int,CurAkaID);
		REG_AUTO(PlayerNick);
	}ENDSAVE;

	void Write();
	bool Read();

	bool AddNewAka(const char* Name);
	bool DelAka(int ID);
	int SetCurAka(int ID);
	void WriteCurAka();
};

extern cvi_SinglePlayerData vmSinglePlayerData;
extern const char* vmSinglePlayerDataXML;
extern cvi_PlayerData vmPlayerData;

regAc(cva_SPD_AkaList, vfS vfI vfA , );
regAc(cva_SPD_MovieList, vfS vfI vfA , );
regAc(cva_SPD_DelAka, vfL , );
regAc(cva_SPD_AddAka, vfS
	_str BackModalDesk;
	,
	REG_AUTO(BackModalDesk);
);
regAc(cva_SPD_ChangeAka, vfL , );
regAc(cva_SPD_CancelAka, vfL , );
regAc(cva_SPD_ToMainIfNotAka, vfL
	_str MainModalDesk;
	,
	REG_AUTO(MainModalDesk);
);


