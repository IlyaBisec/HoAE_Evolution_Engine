#pragma once

class ActiveAbilityListArray;

class DIALOGS_API units_iterator{
public:
	class DIALOGS_API  ActiveAbilities
	{
		int pos, posMax;
		//OneObject* OB;
		ActiveAbilityListArray* AList;
	public:		
		void Create(OneObject* OBj);
		void Create(OneObject* OBj, int InfluenceBit);
		ActiveUnitAbility* Next();
		ActiveAbilities(){
			AList=NULL;
		}
	};
	class DIALOGS_API  UnitsInRadius
	{
		word ids[2048];
		int nids;
		int pos;
		int _xc,_yc,_R2;
	public:		
		void Create(int _xc,int _yc,int R);
		OneObject* Next();
	};
	class DIALOGS_API UnitsInRadiusFromOtherNations
	{
		word ids[2048];
		int nids;
		int pos;
		int _xc,_yc,_R2;
	public:		
		void Create(byte NI,int _xc,int _yc,int R);
		OneObject* Next();
	};
	class DIALOGS_API UnitsInRadiusFromSomeNation
	{
		word ids[2048];
		int nids;
		int pos;
		int _xc,_yc,_R2;
	public:		
		void Create(byte NI,int _xc,int _yc,int R);
		OneObject* Next();
	};	

	class DIALOGS_API GetUnitsInSquare
	{
		word ids[2048];
		int nids;
		int pos;
		int _x,_y,_x1,_y1;
	public:
		void Create(int x,int y, int x1, int y1);
		OneObject* Next();
	};
	class DIALOGS_API GetBuildingsInRadius
	{
		word ids[2048];
		int nids;
		int pos;
		int _xc,_yc,_R2;
	public:		
		void Create(int _xc,int _yc,int R);
		OneObject* Next();
	};
	class DIALOGS_API GetBuildingsInSquare
	{
		word ids[2048];
		int nids;
		int pos;
		int _x,_y,_x1,_y1;
	public:
		void Create(int x,int y, int x1, int y1);
		OneObject* Next();
	};
	class DIALOGS_API GetUnitsOfBrigade
	{
		int pos;
		Brigade* Br;
	public:
		void Create(Brigade* BR);
		OneObject* Next();
	};
	class DIALOGS_API GetNotCommandUnitsOfBrigade
	{
		int pos;
		Brigade* Br;
	public:
		void Create(Brigade* BR);
		OneObject* Next();
	};
	class DIALOGS_API GetUnitsOfNation
	{
		int pos;
		word* id;
		int   n;
	public:
		void Create(int NI);
		OneObject* Next();
	};
	class DIALOGS_API GetAllUnits
	{
		int pos;
	public:
		void Create();
		OneObject* Next();
	};
	class DIALOGS_API GetSelected
	{
		int n;
		word* id;
		int pos;
	public:
		void Create(int NI);
		OneObject* Next();
	};
	class DIALOGS_API GetImSelected
	{
		int n;
		word* id;
		int pos;
	public:
		void Create(int NI);
		OneObject* Next();
	};
	class DIALOGS_API GetSpriteInRadius
	{
		word ids[2048];
		int nids;
		int pos;
		int _xc,_yc,_R2;
	public:		
		bool Create(int _xc,int _yc,int R);
		OneSprite* Next();
	};
	class DIALOGS_API GetSpriteInSquare
	{
		word ids[2048];
		int nids;
		int pos;
		int _x,_y,_x1,_y1;
	public:
		bool Create(int x,int y, int x1, int y1);
		OneSprite* Next();
	};
};
extern DIALOGS_API units_iterator::ActiveAbilities itr_AA;
extern DIALOGS_API units_iterator::UnitsInRadius itr_UnitsInRadius;
extern DIALOGS_API units_iterator::UnitsInRadiusFromOtherNations itr_UnitsInRadiusFromOtherNations;
extern DIALOGS_API units_iterator::UnitsInRadiusFromSomeNation itr_UnitsInRadiusFromSomeNation;
extern DIALOGS_API units_iterator::GetUnitsInSquare itr_GetUnitsInSquare;
extern DIALOGS_API units_iterator::GetBuildingsInRadius itr_GetBuildingsInRadius;
extern DIALOGS_API units_iterator::GetBuildingsInSquare itr_GetBuildingsInSquare;
extern DIALOGS_API units_iterator::GetUnitsOfBrigade itr_GetUnitsOfBrigade;
extern DIALOGS_API units_iterator::GetNotCommandUnitsOfBrigade itr_GetNotCommandUnitsOfBrigade;
extern DIALOGS_API units_iterator::GetUnitsOfNation itr_GetUnitsOfNation;
extern DIALOGS_API units_iterator::GetAllUnits itr_GetAllUnits;
extern DIALOGS_API units_iterator::GetSelected itr_GetSelected;
extern DIALOGS_API units_iterator::GetImSelected itr_GetImSelected;
extern DIALOGS_API units_iterator::GetSpriteInRadius itr_GetSpritesInRadius;
extern DIALOGS_API units_iterator::GetSpriteInSquare itr_GetSpritesInSquare;

//units_iterator::UnitsInRadius E(...);
//example
//while(OneObject* OB=E.Next()){
//
//}
 
class DIALOGS_API OneSpriteIterator{
public:
	void GetSpritesInRadius(int x,int y,int R);
	void GetSpritesInSquare(int x,int y,int R);
	void GetAllSprites();

	OneSprite* GetNext();
};
