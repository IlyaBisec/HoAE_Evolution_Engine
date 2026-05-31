#ifndef GEN_EXT_CODE
#define ITERATE_REF(idx,expr,argslist,args)void ext_##expr##argslist##;
#define ITERATE_BOOL_REF(idx,expr,argslist,args)bool ext_##expr##argslist##;
#define ITERATE_TBOOL_REF(idx,expr,argslist,args)bool ext_##expr##argslist##;
#define ITERATE_TYPE_REF(idx,type,expr,argslist,args)type ext_##expr##argslist##;
#define ITERATE_NOTHING(idx,type,expr,argslist,args)
#endif //GEN_EXT_CODE
ITERATE_REF(0,ProcessingGame,(),());
ITERATE_REF(1,OnGameStart,(),());
ITERATE_REF(2,OnGameEnd,(),());
ITERATE_REF(3,OnVictory,(byte Nation),(Nation));
ITERATE_REF(4,OnDefeat,(byte Nation),(Nation));
ITERATE_REF(5,OnManualExit,(byte Nation),(Nation));
ITERATE_REF(6,OnClassRegistration,(),());
ITERATE_REF(200,OnClassUnRegister,(),());
ITERATE_REF(201,BeforeLoadMD,(),());
ITERATE_REF(202,BeforeLoadNations,(),());
ITERATE_REF(7,OnUnloading,(),());
ITERATE_REF(8,OnReloading,(),());
ITERATE_REF(9,OnInitAfterMapLoading,(),());
ITERATE_REF(10,OnEditorStart,(),());
ITERATE_REF(11,OnEditorEnd,(),());
ITERATE_NOTHING (12,DWORD,OnMapSaving,(xmlQuote& xml),(xml));
ITERATE_NOTHING (13,OnMapLoading,(xmlQuote& xml,DWORD Code),(xml,Code));
ITERATE_NOTHING (14,DWORD,OnGameSaving,(xmlQuote& xml),(xml));
ITERATE_NOTHING (15,OnGameLoading,(xmlQuote& xml,DWORD Code),(xml,Code));
ITERATE_REF(203,OnInitAfterGameLoading,(),());
ITERATE_REF(204,OnInitAfterGameSaving,(),());
ITERATE_BOOL_REF(209,OnWindowsMessage,(bool InEditor,HWND hwnd,int Message,int wParam,int lParam),(InEditor,hwnd,Message,wParam,lParam));
ITERATE_BOOL_REF(210,OnKeyPressed,(bool InEditor,bool InTextEditor,int KeyCode),(InEditor,InTextEditor,KeyCode));
ITERATE_BOOL_REF(16,OnCheatEntering,(const char* Cheat),(Cheat));
ITERATE_BOOL_REF(17,OnCheatReceived,(byte Nation,const char* Cheat),(Nation,Cheat));
ITERATE_BOOL_REF(18,OnMouseHandling,(int mx,int my,bool& LeftPressed,bool& RightPressed,int MapCoordX,int MapCoordY,bool OverMiniMap),(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap));
ITERATE_BOOL_REF(19,OnEndGameMessage,(int NI,int VictStatus),(NI,VictStatus));
ITERATE_REF     (20,OnMapUnLoading,(),());
ITERATE_BOOL_REF(21,CheckActivityOfEditor,(),());
ITERATE_REF     (22,ClearActivityOfEditor,(),());
ITERATE_REF		(23,DrawEditorInterface,(),());
ITERATE_BOOL_REF(24,GetEditorAttributes,(int& gp_file,int& ActiveSprite,int& PassiveSprite,_str& Hint),(gp_file,ActiveSprite,PassiveSprite,Hint));
ITERATE_BOOL_REF(25,CheckIfMouseOverEditorInterface,(int x,int y),(x,y));
ITERATE_BOOL_REF(26,OnMouseWheel,(int Delta),(Delta));
ITERATE_REF     (27,ActivateEditor,(),());
ITERATE_REF     (28,OnShowInGameDialog,(),());

ITERATE_REF(30,OnDrawOnMapAfterLandscape,(),());
ITERATE_REF(31,OnDrawOnMapAfterUnits,(),());
ITERATE_REF(32,OnDrawOnMapBeforeWater,(),());
ITERATE_REF(33,OnDrawOnMapAfterWater,(),());
ITERATE_REF(34,OnDrawOnMapAfterTransparentEffects,(),());
ITERATE_REF(35,OnDrawOnMapAfterFogOfWar,(),());
ITERATE_REF(36,OnDrawOnMapOverAll,(),());

ITERATE_REF(40,OnDrawOnMiniMap,(int x,int y,int Lx,int Ly),(x,y,Lx,Ly));
ITERATE_BOOL_REF(41,OnSetupCamera,(ICamera* ICam),(ICam));
//
ITERATE_REF(50,OnUnitBirth,(OneObject* NewUnit),(NewUnit));
ITERATE_TBOOL_REF(51,OnUnitDie,(OneObject* Dead,OneObject* Killer),(Dead,Killer));
ITERATE_TBOOL_REF(52,OnUnitDamage,(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType),(DamagedUnit,Damager,Damage,AttType));
ITERATE_TBOOL_REF(53,OnUnitCapture,(OneObject* CapturedUnit,OneObject* Capturer),(CapturedUnit,Capturer));
ITERATE_TBOOL_REF(54,OnUnitPanic,(OneObject* Panicer),(Panicer));
ITERATE_TBOOL_REF(55,OnAttemptToAttack,(OneObject* Attacker,OneObject* Victim),(Attacker,Victim));
ITERATE_TBOOL_REF(56,OnAttemptToMove,(OneObject* Unit,int x,int y),(Unit,x,y));
ITERATE_TBOOL_REF(57,OnAttemptToTakeResource,(OneObject* Unit,int x,int y,int ResType),(Unit,x,y,ResType));
ITERATE_TBOOL_REF(58,OnCheckingBuildPossibility,(byte NI,int Type,int& x,int& y),(NI,Type,x,y));
ITERATE_REF(59,OnUnitWasProduced,(OneObject* Producer,OneObject* NewObject),(Producer,NewObject));

ITERATE_REF(80,OnBrigadeCreated,(Brigade* BR),(BR));
ITERATE_REF(81,OnBrigadeFreedManually,(Brigade* BR),(BR));
ITERATE_REF(82,OnBrigadeKilled,(Brigade* BR,byte KillerNation),(BR,KillerNation));

ITERATE_TBOOL_REF(110,OnUseItem,(int UnitIndex,ActiveUnitAbility* AA),(UnitIndex,AA));
ITERATE_TBOOL_REF(111,OnBuyItem,(int BuyIndex,int SellIndex,ActiveUnitAbility* AA),(BuyIndex,SellIndex,AA));
ITERATE_TBOOL_REF(112,OnSellItem,(int SellIndex,int BuyIndex,ActiveUnitAbility* AA),(SellIndex,BuyIndex,AA));
ITERATE_TBOOL_REF(113,OnDropItem,(int Index,ActiveUnitAbility* AA),(Index,AA));
ITERATE_TBOOL_REF(114,OnPutOnItem,(int UnitIndex,ActiveUnitAbility* AA),(UnitIndex,AA));
ITERATE_TBOOL_REF(115,OnPutOffItem,(int UnitIndex,ActiveUnitAbility* AA),(UnitIndex,AA));

ITERATE_TBOOL_REF(120,OnEnterShop,(int UnitIndex,int ShopIndex),(UnitIndex,ShopIndex));

//ITERATE_TBOOL_REF(83,OnBrigadeReformed,(Brigade* BR,int StartForm,int EndForm),(BR,StartForm,EndForm));