#pragma once
#include "MapTemplates.h"
#include "CurrentMapOptions.h"
//
CEXPORT	void StartAIEx(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty);
//
class mai_ACGroup: public BaseClass
{
public:	
	mai_ACGroup(){
		dangIteration=4;
		dangFear=1;
	}
	void DrawOnMap();
	void DrawOnMiniMap(int x,int y,int Lx,int Ly);
	//
	//bool ChechGroup();
	word GroupID;
	word Targets[4096];
	int DangerMap[4096];
	DynArray<word> Way;
	//
	bool Visualization;
	byte Type;
	int dangIteration;
	int dangFear;
	SAVE(mai_ACGroup){		
		REG_MEMBER(_bool,Visualization);
		REG_ENUM(_byte_index,Type,MAI_ACType);		
		REG_MEMBER(_int,dangIteration);
		REG_MEMBER(_int,dangFear);
	}ENDSAVE;
};
////////////////////////////////////////////////////////////////////////////////////////
class mai_Monitor: public BaseClass
{
public:
	mai_Monitor(void);
	~mai_Monitor(void);
	//
	void DrawOnMap(){
		AC_Group.DrawOnMap();
	};
	void DrawOnMiniMap(int x,int y,int Lx,int Ly){
		AC_Group.DrawOnMiniMap(x,y,Lx,Ly);
	};
	//
	bool Started;
	class mai_StartAI: public BaseFunction{
	public:
		_str File;
		int Difficulty;
		StartResources Resource;
		//
		mai_StartAI(){
			Difficulty=3;
			Resource.Wood=2000;
			Resource.Gold=1000;
			Resource.Stone=2000;
			Resource.Food=4000;
			Resource.Iron=1000;
			Resource.Coal=1000;
		}
		void EvaluateFunction(){
			mai_Monitor* M=get_parent<mai_Monitor>();
			if(M&&!M->Started){
				M->Started=true;
				byte NI=MyNation;
				StartAIEx(NI,File.pchar(),0,0,0,Difficulty);
				SetXRESRC(NI,TreeID,Resource.Wood);
				SetXRESRC(NI,GoldID,Resource.Gold);
				SetXRESRC(NI,StoneID,Resource.Stone);
				SetXRESRC(NI,FoodID,Resource.Food);
				SetXRESRC(NI,IronID,Resource.Iron);
				SetXRESRC(NI,CoalID,Resource.Coal);
			}
		}
		SAVE(mai_StartAI){
			REG_PARENT(BaseFunction);
			REG_FILEPATH(File,".ai.xml");
			REG_MEMBER(_int,Difficulty);
			REG_AUTO(Resource);
		}ENDSAVE;
	};
	mai_StartAI Start;	
	//
	mai_ACGroup AC_Group;
	//
	//SubSection AC_Groups;
	//SubSection Sprite;
	//
	SAVE(mai_Monitor){	
		REG_MEMBER(_bool, Started);		
		REG_AUTO(Start);
		REG_AUTO(AC_Group);
	}ENDSAVE;
};
extern mai_Monitor vaiMonitor;
////////////////////////////////////////////////////////////////////////////////////////