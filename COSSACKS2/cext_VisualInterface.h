#pragma once
//////////////////////////////////////////////////////////////////////////
#include "GameExtension.h"
#include "cvi_MainMenu.h"
#include "CurrentMapOptions.h"
#include "VUI_Actions.h"
//
extern bool GameExit;
extern MapOptions MOptions;
//////////////////////////////////////////////////////////////////////////
class cext_VisualInterface: public GameExtension{
public:
	virtual void OnClassRegistration();
	virtual void ProcessingGame();
	virtual void OnInitAfterMapLoading();
	virtual void OnUnloading();
	virtual bool OnMapUnLoading();
	virtual bool OnEndGameMessage(int NI,int VictStatus);
	//
	virtual void OnUnitBirth(OneObject* NewUnit);
	virtual bool OnUnitCapture(OneObject* CapturedUnit,OneObject* Capturer);
	virtual bool OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType);
	virtual bool OnUnitDie(OneObject* Dead,OneObject* Killer);
	//
	virtual void OnBrigadeCreated(Brigade* BR);
	virtual void OnBrigadeKilled(Brigade* BR,byte KillerNation);
	//
	virtual void OnDrawOnMiniMap(int x,int y,int Lx,int Ly);	
	virtual void OnDrawOnMapAfterTransparentEffects();
	virtual void OnDrawOnMapOverAll();

	virtual bool OnGameSaving(xmlQuote& xml);
	virtual bool OnGameLoading(xmlQuote& xml);
	virtual bool OnMapSaving(xmlQuote& xml);
	virtual bool OnMapLoading(xmlQuote& xml);

	virtual bool OnCheatEntering(const char* Cheat);
	
	virtual bool OnMouseHandling
		(int mx,int my,bool& LeftPressed,bool& RightPressed,
		int MapCoordX,int MapCoordY,bool OverMiniMap);

	bool ShowAIPointInfo;
	int mouseX, mouseY;
	int mapX, mapY;

	static DialogsSystem PulseSquare;
	static DialogsSystem VI_Zone;
	static DialogsSystem Credits;
};

//
regAc(cva_VI_Setl, vfS vfL vfRt);
regAc(cva_VI_EndGame, vfS
	static bool isStarted;
	static int VictStatus;
	static char* UserMessage;
	//SubSection 
	char* VicTitle;
	char* VicMessage;
	char* VicButton;
	char* DefTitle;
	char* DefMessage;
	char* DefButton;
	short gpFile;
	int VicSprite;
	int DefSprite;
	SubSection Reference;
	ClassRef<TextButton> dTitle;
	ClassRef<GPPicture> dPicture;
	ClassRef<TextButton> dMessage;
	ClassRef<VitButton> dButton;
	,	
	REG_MEMBER(_textid,VicTitle);
	REG_MEMBER(_textid,VicMessage);
	REG_MEMBER(_textid,VicButton);
	REG_MEMBER(_textid,DefTitle);
	REG_MEMBER(_textid,DefMessage);
	REG_MEMBER(_textid,DefButton);
	REG_MEMBER(_gpfile,gpFile);
	REG_SPRITE(VicSprite,gpFile);
	REG_SPRITE(DefSprite,gpFile);
	//
	REG_AUTO(Reference);
	REG_AUTO(dTitle);
	REG_AUTO(dPicture);
	REG_AUTO(dMessage);
	REG_AUTO(dButton);
);
/// ai Base ///
class aiBase{
//private:
//	int cn, cx, cy, cr;
public:
	bool Enable;
	byte NI;
	int cX, cY;				// центер масс
	int cUnitX, cUnitY;		// ближайший важный юнит к центру масс
	int cR;					//
	int rDefenders;			//
	//DynArray<int> enemyX;	//
	//DynArray<int> enemyY;	//
	int eX, eY;
	int eR;
	//int GetDistToEnemy(int x, int y);
	//bool 
	//
	//void Prepare();
	//void Add(OneObject* OB);
	//void Calculate();
	void Process();
	void Unload();
	//
	bool CorrectPosition(int& x, int& y);	
};
class aiInfo{
private:
	int CurID;
public:
	int RefreshTime;
	aiBase Base[8];
	void Process();
	void Unload();
	aiInfo(){
		RefreshTime=8000;
	}
};
extern aiInfo aiI;
/// ai Base ///

// Хотел сделать через ClassDesk
//class cDialogHistoryChapter: public BaseClass {
//public:
//	_str Chapter;
//	SAVE(cDialogHistoryChapter){
//		REG_AUTO(Chapter);
//	}ENDSAVE;
//};
//
//class cDialogHistoryDelimiter: public BaseClass {
//public:
//	SAVE(cDialogHistoryDelimiter){
//	}ENDSAVE;
//};
