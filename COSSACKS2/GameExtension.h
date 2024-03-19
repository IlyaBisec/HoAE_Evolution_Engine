#pragma once
//////////////////////////////////////////////////////////////////////////
#include "LoadSave.h"

class ActiveUnitAbility;

#define MAXVDWORDS 8
#define MAXVFUNC (MAXVDWORDS*32)
class DIALOGS_API GameExtension: public BaseClass{	
	DWORD Usage[MAXVDWORDS];
public:
	_str ExtensionName;
	DWORD GetCode();
	_inline void UnMask (int idx){Usage[idx>>5]&=0xFFFFFFFF-(1<<(idx&31));}
	_inline void Mask   (int idx){Usage[idx>>5]|=1<<(idx&31);}
	_inline bool Check  (int idx){return Usage[idx>>5]&(1<<(idx&31));}

	const char* GetExtName(){return ExtensionName.pchar();}
	GameExtension(){memset(Usage,0xFF,sizeof Usage);};
	//---------actions of the game----------//
	virtual void ProcessingGame()  											{UnMask(0);}//
	virtual void OnGameStart()     											{UnMask(1);}//
	virtual void OnGameEnd()       											{UnMask(2);}//
	virtual void OnVictory(byte Nation)										{UnMask(3);}//
	virtual void OnDefeat(byte Nation)										{UnMask(4);}//
	virtual void OnManualExit(byte Nation)									{UnMask(5);}//
	virtual void OnClassRegistration()										{UnMask(6);}//
	virtual void OnClassUnRegister()										{UnMask(200);}//
	virtual	void BeforeLoadMD()												{UnMask(201);}//
	virtual void BeforeLoadNations()										{UnMask(202);}//
	virtual void OnUnloading()  											{UnMask(7);}//
	virtual void OnReloading()  											{UnMask(8);}//
	virtual void OnInitAfterMapLoading()  									{UnMask(9);}//
	//
	virtual void OnEditorStart()   											{UnMask(10);}//
	virtual void OnEditorEnd()     											{UnMask(11);}//
	virtual bool OnMapSaving(xmlQuote& xml)									{UnMask(12);return false;}
	virtual bool OnMapLoading(xmlQuote& xml)								{UnMask(13);return false;}
	virtual bool OnGameSaving(xmlQuote& xml)								{UnMask(14);return false;}
	virtual bool OnGameLoading(xmlQuote& xml)								{UnMask(15);return false;}
    virtual void OnInitAfterGameLoading()                                   {UnMask(203);}
    virtual void OnInitAfterGameSaving()                                    {UnMask(204);}

	virtual void OnSaveBinaryDataIntoMap(BinStream* bs)						{UnMask(205);}
	virtual void OnLoadBinaryDataFromMap(BinStream* bs)						{UnMask(206);}
	virtual void OnSaveGameBinaryData(BinStream* bs)						{UnMask(207);}
	virtual void OnLoadGameBinaryData(BinStream* bs)						{UnMask(208);}
	virtual bool OnWindowsMessage(bool InEditor,HWND hwnd,int Message,int wParam,int lParam){UnMask(209);return false;}//return true if message was used
	virtual bool OnKeyPressed(bool InEditor,bool InTextEditor,int KeyCode)	{UnMask(210);return false;}//return true if key was used
	//InEditor = true if we are in map editor InTextEditor = true if focus is in some edit box



	virtual bool OnCheatEntering(const char* Cheat)   						{UnMask(16);return false;}//
	virtual bool OnCheatReceived(byte Nation,const char* Cheat)				{UnMask(17);return false;}//
	virtual bool OnMouseHandling
		(int mx,int my,bool& LeftPressed,bool& RightPressed,
		int MapCoordX,int MapCoordY,bool OverMiniMap)						{UnMask(18);return false;}//
	//
	virtual bool OnEndGameMessage(int NI,int VictStatus)					{UnMask(19);return false;}
	//
	virtual bool OnMapUnLoading()											{UnMask(20);return false;}
	//integrated editor management
	virtual bool CheckActivityOfEditor()									{UnMask(21);return false;}
	virtual void ClearActivityOfEditor()									{UnMask(22);}
	virtual void DrawEditorInterface()					                    {UnMask(23);}
	virtual bool GetEditorAttributes(int& gp_file,
		int& ActiveSprite,int& PassiveSprite,_str& Hint)					{UnMask(24);return false;}	
	virtual bool CheckIfMouseOverEditorInterface(int x,int y)               {UnMask(25);return false;}
	virtual bool OnMouseWheel(int Delta)									{UnMask(26);return false;}
	virtual void ActivateEditor()                                           {UnMask(27);}
    virtual void OnShowInGameDialog()                                       {UnMask(28);}

	virtual void OnDrawOnMapAfterLandscape()								{UnMask(30);}//
	virtual void OnDrawOnMapAfterUnits()									{UnMask(31);}//
	virtual void OnDrawOnMapBeforeWater()									{UnMask(32);}//
	virtual void OnDrawOnMapAfterWater()									{UnMask(33);}//
	virtual void OnDrawOnMapAfterTransparentEffects()						{UnMask(34);}//
	virtual void OnDrawOnMapAfterFogOfWar()									{UnMask(35);}//
	virtual void OnDrawOnMapOverAll()										{UnMask(36);}//

	virtual void OnDrawOnMiniMap(int x,int y,int Lx,int Ly)					{UnMask(40);}//
	virtual bool OnSetupCamera(ICamera* Cam)								{UnMask(41);return false;}//

	virtual void OnUnitBirth(OneObject* NewUnit)							{UnMask(50);}//
	virtual bool OnUnitDie(OneObject* Dead,OneObject* Killer)				{UnMask(51);return true;}
	virtual bool OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,
		int& Damage,byte AttType)											{UnMask(52);return true;}
	virtual bool OnUnitCapture(OneObject* CapturedUnit,OneObject* Capturer)	{UnMask(53);return true;}
	virtual bool OnUnitPanic(OneObject* Panicer)							{UnMask(54);return true;}
	virtual bool OnAttemptToAttack(OneObject* Attacker,OneObject* Victim)	{UnMask(55);return true;}
	virtual bool OnAttemptToMove(OneObject* Unit,int x,int y)				{UnMask(56);return true;}
	virtual bool OnAttemptToTakeResource
		(OneObject* Unit,int x,int y,int ResType)							{UnMask(57);return true;}
	virtual bool OnCheckingBuildPossibility(byte NI,int Type,int& x,int& y)	{UnMask(58);return true;}
	virtual void OnUnitWasProduced(OneObject* Producer,OneObject* NewObject){UnMask(59);}

	virtual void OnBrigadeCreated(Brigade* BR)								{UnMask(80);}
	virtual void OnBrigadeFreedManually(Brigade* BR)						{UnMask(81);}
	virtual void OnBrigadeKilled(Brigade* BR,byte KillerNation)				{UnMask(82);}
	//virtual bool OnBrigadeReformed(Brigade* BR,int StartForm,int EndForm)	{UnMask(83);return true;}    

	virtual DWORD OnSavePiece(SaveBuf* sb,int x,int y,int x0,int y0,int x1,int y1)
																			{UnMask(90);return 0;}
	virtual bool  OnLoadPiece(SaveBuf* sb,DWORD Code,int x,int y)			{UnMask(91);return true;}
	virtual bool  OnPreviewPiece(SaveBuf* sb,DWORD Code,int x,int y)		{UnMask(92);return true;}

	virtual bool OnUnknownCommandInMD_File
		(NewMonster* NM,const char* Command,const char* File,int Line)      {UnMask(100);return false;}
	virtual bool OnUnknownCommandInNDS_File
		(byte NI,const char* SectionName,
		const char* Command,const char* File,int Line)						{UnMask(101);return false;}
	virtual bool OnUnknownCommandInRDS_File
		(SprGroup* SG,ObjCharacter* OC,const char* SectionName,
		const char* Command,const char* File,int Line)						{UnMask(102);return false;}	

	virtual bool OnUseItem(int UnitIndex,ActiveUnitAbility* AA)				{UnMask(110);return true;}
	virtual bool OnBuyItem(int BuyIndex,int SellIndex,ActiveUnitAbility* AA){UnMask(111);return true;}
	virtual bool OnSellItem(int SellIndex,int BuyIndex,ActiveUnitAbility* AA){UnMask(112);return true;}
	virtual bool OnDropItem(int Index,ActiveUnitAbility* AA)				{UnMask(113);return true;}
	virtual bool OnPutOnItem(int UnitIndex,ActiveUnitAbility* AA)			{UnMask(114);return true;}
	virtual bool OnPutOffItem(int UnitIndex,ActiveUnitAbility* AA)			{UnMask(115);return true;}

	virtual bool OnEnterShop(int UnitIndex,int ShopIndex)					{UnMask(120);return true;}
	
};
void InstallExtension(GameExtension* Ext,const char* Name);
void UnInstallExtension(const char* Name);

void ext_OnMapSaving(xmlQuote& xml);
void ext_OnMapLoading(xmlQuote& xml);
void ext_OnGameSaving(xmlQuote& xml);
void ext_OnGameLoading(xmlQuote& xml);
void ext_OnSaveBinaryDataFromMap(BinStream* bs);
void ext_OnLoadBinaryDataFromMap(BinStream* bs);
void ext_OnSaveGameBinaryData(BinStream* bs);
void ext_OnLoadGameBinaryData(BinStream* bs);

