#include "stdheader.h"
#include <more_types.h>
#include "MapTemplates.h"
#include "vui_Action.h"
#include "vui_Actions.h"
#include "Language.h"
//////////////////////////////////////////////////////////////////////////
#pragma warning ( pop )
#include "BE_HEADERS.h"
#include "vui_Actions.h"
#include "UnitsInterface.h"
#include "cvi_MainMenu.h"
#include "AI_Scripts.h"								//Vitya //  [11/11/2003]
#include "Dialogs\MISSIONSCRIPT\MissionScript.h"	//Vitya //  [11/11/2003]
#include "Dialogs\MISSIONSCRIPT\EndMissionData.h"	//Vitya //  [11/12/2003]
#include "Dialogs\ClasDeclaration.h"				//Vitya.
#include "Dialogs\CPlayerSave.hpp"					//Grey.
#include "unitability.h"
#include "CurrentMapOptions.h"
#include "HelpSystem.h"
#include ".\cvi_brigcreate.h"
#include ".\cvi_market.h"
#include ".\cvi_academy.h"
#include ".\cvi_oboz.h"
#include ".\cvi_HeroButtons.h"
#include ".\cext_VisualInterface.h"
//#include ".\cva_DeskArray.h"
#include ".\cvi_postgamestatistic.h"
#include ".\cvi_campaign.h"
#include ".\cvi_singleplayerdata.h"
#include ".\cvi_RomeHelp.h"
#include ".\cvi_InGamePanel.h"
#include ".\vui_GlobalHotKey.h"
#include ".\cvi_Missions.h"
#include "DrawFeatures.h"
#include "ClassPresentation.h"
#include "ua_Item.h"
#include "ua_Orders.h"
#include "GameInterface.h"

void ACT(int x);

// cva_BR_RoSList
char* mMP_DirRos="RiseofSols\\Maps\\";
// cva_BR_MapList
char* mMP_Dir="Maps\\";

bool vMapListInit = false,
     vMapRoSInit = false;
_str vMapMask, vMapMaskRoS;

bool vmBattles = false,
     vmRoSMission = false;


//////////////////////////////////////////////////////////////////////////
void vui_CreateActionEnum(){
	vui_Action::E=ENUM.Get("ve_Action");
	reg_v_Action<va_Null>("Ничего");
	//
	REG_CLASS_EX(cva_SP_Class,				"InGame");
	REG_CLASS_EX(cva_SP_Amount, 			"InGame");

	REG_CLASS_EX(cva_Inventory_HeroPortret,	"InGame");
	REG_CLASS_EX(cva_Inventory_ShopPortret,	"InGame");
	REG_CLASS_EX(cva_Inventory_TabButton, 	"InGame");

	REG_CLASS_EX(cva_FocusedUnitHint,		"InGame");
	REG_CLASS_EX(cva_DialogsHistory,		"InGame");

	REG_CLASS_EX(cva_FanUnitInfoToggle,		"InGame");
	REG_CLASS_EX(cva_DemoMessage,			"InGame");

	REG_CLASS_EX(cva_MM_GoldenTextXY,		"MM");
	//
	REG_CLASS_EX(cva_CommonRoomParamDD,		"CommonRoomParam");
	REG_CLASS_EX(cva_CommonRoomParam,		"CommonRoomParam");

	REG_CLASS_EX(cva_BrigBinkVideoTry,		"BinkVideo");
	REG_CLASS_EX(cva_BrigBinkVideoShow,		"BinkVideo");
	REG_CLASS_EX(cva_BrigBinkVisibility,	"BinkVideo");
	REG_CLASS_EX(cva_BrigBinkSetVision,		"BinkVideo");
	//
	REG_CLASS_EX(va_EW2_Mis_List,			"EW2_Mis");
	REG_CLASS_EX(va_EW2_Mis_Tap,			"EW2_Mis");
	REG_CLASS_EX(va_EW2_Mis_Text,			"EW2_Mis");
	REG_CLASS_EX(va_EW2_Mis_Picture,		"EW2_Mis");
	REG_CLASS_EX(va_EW2_Mis_CheckTutorial,	"EW2_Mis");	
	REG_CLASS_EX(va_EW2_Mis_Start,			"EW2_Mis");
	//
	REG_CLASS_EX(cva_VI_Setl,			"VI_Extension");
	REG_CLASS_EX(cva_VI_EndGame,		"VI_Extension");		
	//	
	REG_CLASS_EX(cva_CampStat_Show,		"EW2 Camp Stats");
	REG_CLASS_EX(cva_CampStat_Mode,		"EW2 Camp Stats");
	REG_CLASS_EX(cva_CampStat_PlFlag,	"EW2 Camp Stats");
	REG_CLASS_EX(cva_CampStat_PlName,	"EW2 Camp Stats");
	REG_CLASS_EX(cva_CampStat_PlScore,	"EW2 Camp Stats");
	REG_CLASS_EX(cva_CampStat_Graphs,	"EW2 Camp Stats");
	REG_CLASS_EX(cva_CampStat_Oy,		"EW2 Camp Stats");
	REG_CLASS_EX(cva_CampStat_Ox,		"EW2 Camp Stats");	
	REG_CLASS_EX(cva_CampStat_Player,	"EW2 Camp Stats");	
	REG_CLASS_EX(cva_CampStat_Color,	"EW2 Camp Stats");	
	//
	REG_CLASS_EX(cva_HideIfUInfoActive,		"InGame");
	REG_CLASS_EX(cva_C2_HintMessage,		"InGame");
	REG_CLASS_EX(cva_TutButtonVisibleAdd,	"InGame");
	//	
	REG_CLASS_EX(cva_NullHint,			"Tools");
	REG_CLASS_EX(cva_ShowAboutText,		"Credits");
	REG_CLASS_EX(cva_CreditsDesk,		"Credits");
	REG_CLASS_EX(cva_HotKeyButton,		"Tools");
	REG_CLASS_EX(cva_MixWithWhite,		"Tools");
	REG_CLASS_EX(cva_ClearClick,		"Tools");
	REG_CLASS_EX(cva_OpenWebLink,		"Tools");
	REG_CLASS_EX(cva_ExitToWin,			"Tools");
	REG_CLASS_EX(cva_Exit2MainMenu,		"Tools");
	REG_CLASS_EX(cva_CircleNormaTest,	"Tools");
	//
	REG_CLASS(_strArray);
	REG_CLASS_EX(cva_DemoScreen,		"Demo");
	REG_CLASS_EX(cva_DemoNext,			"Demo");
	REG_CLASS_EX(cva_DemoExit,			"Demo");
	REG_CLASS_EX(cva_DemoDisable,		"Demo");
	//

	REG_CLASS_EX(cva_HotKeyAction,		"Engine");
	//	
	REG_CLASS_EX(cva_Peasant_AutoWork,	"InGamePanel");
	REG_CLASS_EX(cva_Peasant_Idle,		"InGamePanel");	
	REG_CLASS_EX(cva_IGP_Credits,		"InGamePanel");
	REG_CLASS_EX(cva_IGP_Frame,			"InGamePanel");
	//	
	REG_CLASS_EX(cva_LoadingTips,				"MM");
	REG_CLASS_EX(cva_Loading_Back,				"MM");
	REG_CLASS_EX(cva_MM_LobbyVersion,			"MM");
	REG_CLASS_EX(cva_PlayFullscreenVideo,		"MM");	
	REG_CLASS_EX(cva_NatPics,					"MM");	
	REG_CLASS_EX(cva_MM_UnloadBeforeMission,	"MM");
	//
	REG_CLASS_EX(cva_M_CrossActivator,	"Cross");
	REG_CLASS_EX(cva_M_CrossFade,		"Cross");	
	//
	REG_CLASS_EX(cva_GM_CloseButtons,	"GM");
	//	
	REG_CLASS_EX(cva_SPD_AkaList,		"MM");
	REG_CLASS_EX(cva_SPD_AddAka,		"MM");
	REG_CLASS_EX(cva_SPD_AddAka,		"MM");	
	REG_CLASS_EX(cva_SPD_ChangeAka,		"MM");
	REG_CLASS_EX(cva_SPD_CancelAka,		"MM");
	REG_CLASS_EX(cva_SPD_DelAka,		"MM");
	REG_CLASS_EX(cva_SPD_ToMainIfNotAka,"MM");	
	//
	REG_CLASS_EX(cva_SPD_MovieList,			"MM");
	REG_CLASS_EX(cva_SPD_FrescoList,		"MM");
	REG_CLASS_EX(cva_SPD_CampMessageCheck,	"MM");	
	//
	REG_CLASS_EX(cva_BrigDestination,	"Markers");	
	//
	REG_CLASS_EX(cva_MR_Desk,			"MultiRoom");
	REG_CLASS_EX(cva_MR_isServer,		"MultiRoom");

	REG_CLASS_EX(cva_MR_GameSpeed,		"MultiRoom");
	//
	REG_CLASS_EX(cva_vGameMode_Set,			"GameMode");
	REG_CLASS_EX(cva_vGameMode_SetVisible,	"GameMode");

	REG_CLASS_EX(cva_MM_LoadingProgress,"MM");
	//
	REG_CLASS_EX(cva_GameMiniMap,		"InGameInterface");
	REG_CLASS_EX(cva_Inventory,			"InGameInterface");
	REG_CLASS_EX(cva_Library,			"InGameInterface");
	//
	REG_CLASS_EX(cva_U_Info,			"ResPanel");
	REG_CLASS_EX(cva_U_Info_Switch,		"ResPanel");
	REG_CLASS_EX(cva_U_InfoDesk,		"ResPanel");
	REG_CLASS_EX(cva_U_HistoryMess,		"ResPanel");
	REG_CLASS_EX(cva_U_HistoryTitle,	"ResPanel");
	REG_CLASS_EX(cva_U_HistoryPicture,	"ResPanel");	

	REG_CLASS_EX(cva_SP_SetRally,		"SelPoint");
	REG_CLASS_EX(cva_SP_Destroy,		"SelPoint");

	REG_CLASS_EX(cva_SP_Stop,			"SelPoint");
	REG_CLASS_EX(cva_SP_Stop_Cannon,	"SelPoint");
	REG_CLASS_EX(cva_Guardian,			"SelPoint");
	REG_CLASS_EX(cva_SP_AllowShoot,		"SelPoint");
	REG_CLASS_EX(cva_OIS_Scroll,		"SelPoint");
	REG_CLASS_EX(cva_OIS_ScrollLeft,	"SelPoint");
	REG_CLASS_EX(cva_OIS_ScrollRight,	"SelPoint");
	REG_CLASS_EX(cva_OIS_ArtDepo,		"SelPoint");

	REG_CLASS_EX(cva_OIS_Rome,			"SelPoint");
	REG_CLASS_EX(cva_OIS_Fantasy,		"SelPoint");
	REG_CLASS_EX(cva_OIS_Produce,		"SelPoint");
	REG_CLASS_EX(cva_OIS_Upgrade,		"SelPoint");
	REG_CLASS_EX(cva_OIS_ControlPanel,	"SelPoint");
	REG_CLASS_EX(cva_OIS_Abilities,		"SelPoint");
	REG_CLASS_EX(cva_OIS_Inside,		"SelPoint");

	REG_CLASS_EX(cva_SP_Kinetic,		"SelPoint");
	//
	REG_CLASS_EX(cva_RND_MapParam,		"RND Map");
	reg_cva(cva_MM_SinPrepare, "");
	// Geometry
	reg_cva(cva_Geometry_CompactHorisontList, "");
	//	
	REG_CLASS_EX(cva_HeroesUpgrades,	"Campaign");
	REG_CLASS_EX(cva_HeroeXUpgradeX,	"Campaign");
	REG_CLASS_EX(cva_RC_Statistic,		"Campaign");
	REG_CLASS_EX(cva_RC_Hero,			"Campaign");
	REG_CLASS_EX(cva_RC_Ability,		"Campaign");
	//
	REG_CLASS_EX(cva_Campaign,			"Campaign");
	REG_CLASS_EX(cva_Mission,			"Campaign");
	REG_CLASS_EX(cva_MissList,			"Campaign");
	REG_CLASS_EX(cva_Fresco_Next,		"Campaign");
	REG_CLASS_EX(cva_Fresco_Desk,		"Campaign");	
	REG_CLASS_EX(cva_Fresco_Close,		"Campaign");
	REG_CLASS_EX(cva_Camp_Select,		"Campaign");
	REG_CLASS_EX(cva_CCamp_Name,		"Campaign");
	REG_CLASS_EX(cva_CCamp_Sectors,		"Campaign");	
	REG_CLASS_EX(cva_CampDiff,			"Campaign");	
	REG_CLASS_EX(cva_CMiss_Name,		"Campaign");
	REG_CLASS_EX(cva_Camp_StartMission,	"Campaign");
	REG_CLASS_EX(cva_Missions_XY,		"Campaign");

	//REG_CLASS_EX(cva_DeskArray,					"Desk");
	REG_CLASS_EX(cva_DA_StatUnits,					"Desk");	
	//
	REG_CLASS_EX(cva_DrawBrig_LAbility,		"DrawBrig");
	REG_CLASS_EX(cva_BB_Morale,				"DrawBrig");
	//	
	REG_CLASS_EX(cva_ItemChoose_Set,				"ItemChoose");
	REG_CLASS_EX(cva_Hero_Button,					"Hero buttons");
	//
	REG_CLASS_EX(cva_Map_Preview,					"Show preview");
	REG_CLASS_EX(cva_Map_PreviewJpg,				"Show preview");	
	// cvi_Oboz
	REG_CLASS(vui_Action);	
	//REG_CLASS_EX(cva_InGameMenu_MainDesk,			"In game menu");
	//REG_CLASS_EX(cva_InGameMenu_MainDesk_Set,		"In game menu");
	void RegisterAutoActions();
	RegisterAutoActions();
	reg_cva(cva_InGameMenu_MainDesk, "");
	reg_cva(cva_InGameMenu_MainDesk_Set, "");
	reg_cva(cva_InGameMenu_ExitConfirm, "");

	REG_CLASS_EX(cva_CreateRandomMapName,		"Game Room");
	REG_CLASS_EX(cvi_Act_Oboz,					"SelPoint");	
	//
	REG_CLASS_EX(cva_MM_AcceptInDesk,	"MM");
	REG_CLASS_EX(cva_MM_CancelInDesk,	"MM");	
	reg_cva(cva_MM_Accept,					"Accept changes");
	reg_cva(cva_MM_Cancel,					"Return changes back");
	reg_cva(cva_MM_FilesList,				"List of files");
	reg_cva(cva_MM_Run_MissionM3D,			"Run mission from input box");
	//
	REG_CLASS_EX(cva_LoadInGame_Accept,	"LoadSave");
	REG_CLASS_EX(cva_DeleteSaveFile,	"LoadSave");
	REG_CLASS_EX(cva_DeleteButton,		"LoadSave");
	//
	REG_CLASS_EX(cva_ConfirmSaveFileName, "LaodSave");
	reg_cva(cva_SaveInGame_Accept,	"Save: in game accept");
	reg_cva(cva_LS_LoadName,		"Save: sav load name");
	reg_cva(cva_LS_SaveName,		"Save: sav save name");
	reg_cva(cva_LS_FileList,		"Save: sav list");	
	// cvi_Academy
	reg_v_Action<cvi_AcademyDesk>(		"Academy Desk");
	reg_v_Action<cvi_Acd_Inf>(			"Academy infantry");
	reg_v_Action<cvi_Acd_Cav>(			"Academy cavalery");
	reg_v_Action<cvi_Acd_Can>(			"Academy cannons");
	reg_v_Action<cvi_Acd_Peas>(			"Academy peasant");
	reg_v_Action<cvi_Acd_Spend>(		"Academy spended resources");
	// cvi_Market	
	reg_v_Action<cvi_MarketDesk>(		"Market Desk");
	reg_v_Action<cvi_M_SellAmount>(		"Market sell amount");
	reg_v_Action<cvi_M_SellSelect>(		"Market sell select");
	reg_v_Action<cvi_M_BuyAmount>(		"Market buy amount");
	reg_v_Action<cvi_M_BuySelect>(		"Market buy select");
	reg_v_Action<cvi_M_Buy>(			"Market BUY");
	reg_v_Action<cvi_M_Reset>(			"Market reset");
	reg_v_Action<cvi_M_ChangeAmount>(	"Market change amount");
	//		
	reg_v_Action<va_MissDescription>(	"Miss descript Text button");
	REG_CLASS_EX(cva_MissName,			"MM");
	reg_v_Action<va_MissMapBMP>(		"SinMiss map bmp");
	//	
	reg_v_Action<cva_MM_StartInternet>(	"MM start internet");
	reg_v_Action<cva_GameTime>(			"Game Day Time");	
	// cvi_BrigCreate
	reg_v_Action<cvi_A_BC_Active>(		"BrCr: Active");
	reg_v_Action<cvi_A_BC_Crew>(		"BrCr: Crew");
	reg_v_Action<cvi_A_BC_CrewAmount>(	"BrCr: Crew Amount");
	//	
	reg_cva(cva_M_Coss2_Relax, "");	// M: Coss2_Relax in main menu
	reg_v_Action<cva_M_ModDesSetRetChang>("M: ModDeskSet RetChange");
	reg_v_Action<cva_M_Accept>(			"M: accept not close");
	reg_v_Action<cva_M_AcceptSettings>(	"M: accept NotCls settings");
	REG_CLASS_EX(cva_M_ModalDesk,		"ModalDesk");
	REG_CLASS_EX(cva_M_ModalDeskSet,	"ModalDesk");
	REG_CLASS_EX(cva_M_ModalDeskBack,	"ModalDesk");
	//
	reg_v_Action<cva_Mess_CenterQuest>(	"Mess: center quest");
	reg_v_Action<cva_Mess_Quests>(		"Mess: quests list");
	reg_v_Action<cva_Mess_QuestsButton>("Mess: quests show\\hide");
	reg_v_Action<cva_Mess_Dialogs>(		"Mess: dialogs");
	//
	reg_v_Action<cva_MM_SinStart>(		"MM: singl enter");
	//
	REG_CLASS_EX(cva_ProfSave,			"Profile EW2");
	REG_CLASS_EX(cva_ProfLoad,			"Profile EW2");
	reg_v_Action<cva_ProfSel_Accept>(	"Prof sel accept");
	reg_v_Action<cva_ProfSel_Add>(		"Prof sel add");
	reg_v_Action<cva_ProfSel_Cancel>(	"Prof sel cancel");		
	reg_v_Action<cva_ProfDel_Desk>(		"Prof del desk");
	reg_v_Action<cva_ProfDel_Cancel>(	"Prof del cancel");
	reg_v_Action<cva_ProfDel_Accept>(	"Prof del accept");
	reg_v_Action<cva_ProfDelete>(		"Prof delete");	
	REG_CLASS_EX(cva_ProfStats,			"Profile EW2");
	reg_cva(cva_ProfCur_Desc, "");
	reg_v_Action<cva_ProfCur_Name>(		"Prof cur name");	
	reg_v_Action<cva_ProfCur_Race>(		"Prof cur race");
	reg_v_Action<cva_ProfCur_Rank>(		"Prof cur rank");	
	reg_v_Action<cva_ProfCur_Diff>(		"Prof cur diff");
	reg_v_Action<cva_ProfCur_Port>(		"Prof cur port");
	reg_v_Action<cva_ProfCur_Ocup>(		"Prof cur ocup");
	//
	reg_v_Action<cva_ProfAdd_Accept>(	"Profile add accept");
	reg_v_Action<cva_ProfAdd_Cancel>(	"Prof add cancel");		
	reg_v_Action<cva_ProfAdd_RaceFlg>(	"Profile add race flag");
	reg_v_Action<cva_ProfAdd_Name>(		"Profile add name");
	reg_v_Action<cva_ProfAdd_Port>(		"Profile add portrait");
	reg_v_Action<cva_ProfAdd_Desc>(		"Profile add description");
	reg_v_Action<cva_ProfAdd_PortScr>(	"Profile add port scroll");	
	REG_CLASS_EX(cva_ProfAdd_PortScrBut,"Profile");
	reg_v_Action<cva_ProfAdd_Race>(		"Profile add race");
	reg_v_Action<cva_ProfAdd_Diff>(		"Profile add diff");
	reg_v_Action<cva_ProfList>(			"Singl Profile list");
	reg_v_Action<cva_Sin_ProfName>(		"Cur profile name");
	//
	reg_v_Action<cva_Inf_ShowExperience>("UnitExperience");
	reg_v_Action<cva_Inf_ShowMana>		("UnitMana");
	reg_v_Action<cva_Inf_ShowLevel>		("UnitLevel");
	reg_v_Action<cva_Inf_ShowSpeed>		("UnitSpeed");
	reg_v_Action<cva_Inf_ShowStamina>	("UnitStamina");	
	reg_v_Action<cva_Inf_ShowRomeMorale>("RomeMorale");
	REG_CLASS_EX(cva_Inf_BrigExp_Rome,	"SP Brigade");

	reg_v_Action<cva_Inf_ShowAttSpeed>  ("UnitAttSpeed");

	reg_v_Action<cva_SP_in_Box>(		"SP: in box");
	REG_CLASS_EX(cva_SP_LeaveAll_Box,	"SelPoint")

		reg_v_Action<cva_SP_in_Pic>(		"SP: in picture");	
	reg_v_Action<cva_SP_in_Amount>(		"SP: in amount");
	reg_v_Action<cva_SP_in_AmountMax>(	"SP: in amount max");
	//
	REG_CLASS_EX(cva_BR_Title,			"BR");	
	//
	reg_v_Action<cva_H_TaskList>(		"Help: task list");
	reg_v_Action<cva_H_TalkList>(		"Help: talk list");
	reg_v_Action<cva_H_HintList>(		"Help: hint list");
	reg_v_Action<cva_MU_NameInput>(		"Mult: name input");
	REG_CLASS_EX(cva_MU_NickInput,		"MM");
	reg_v_Action<cva_BR_ChatEnter>(		"Room: chat enter");
	reg_v_Action<cva_BR_ChatDesk>(		"Room: chat desk");
	reg_v_Action<cva_BR_ChatInput>(		"Room: chat input");	
	reg_v_Action<cva_BR_ChatDesk>(		"Room: chat desk");	

	REG_CLASS_EX(cva_BR_PlPing,				"BR");
	REG_CLASS_EX(cva_BR_InetGo2Chat,		"BR");
	REG_CLASS_EX(cva_BR_SetVisibleIfInet,	"BR");
	REG_CLASS_EX(cva_BR_SetVisibleIfLAN,	"BR");

	REG_CLASS_EX(cva_BR_Player,	        "BR");
	REG_CLASS_EX(cva_BR_PlName,	        "BR");
	REG_CLASS_EX(cva_BR_PlRace,	        "BR");
	REG_CLASS_EX(cva_BR_PlHero,	        "BR");
	REG_CLASS_EX(cva_BR_PlColor,		"BR");
	REG_CLASS_EX(cva_BR_PlTeam,			"BR");
	REG_CLASS_EX(cva_BR_Ready,	        "BR");
	REG_CLASS_EX(cva_BR_PlMapLoading,	"BR");

	REG_CLASS_EX(cva_BR_WhyNotStart,    "BR");
	REG_CLASS_EX(cva_BR_VirginPlayers,	"BR");

	reg_v_Action<cva_BR_SetSkirBatl>(	"Room: map mask select");
	reg_v_Action<cva_BR_TabMap>(		"Room: map tab buttons");	
	reg_v_Action<cva_BR_MapList>(		"Room: map list");
	reg_v_Action<cva_BR_RoSList>(		"Room: Internet list");
	reg_v_Action<cva_BR_MapPict>(		"Room: map picture");
	reg_v_Action<cva_BR_SavList>(		"Room: save list");
	//
	REG_CLASS_EX(cva_Opt_Options,			"GameOptions");	
	REG_CLASS_EX(cva_Opt_AnimQuality,		"GameOptions");
	REG_CLASS_EX(cva_Opt_ArcadeMode,		"GameOptions");
	REG_CLASS_EX(cva_Opt_Subtitres,			"GameOptions");
	REG_CLASS_EX(cva_Opt_SelectionType,		"GameOptions");
	REG_CLASS_EX(cva_Opt_ReflectedTerrain,	"GameOptions"); //DrawReflectedTerrain
	REG_CLASS_EX(cva_Opt_PostEffects,	"GameOptions"); // 
	reg_v_Action<cva_Opt_SoundVolume>(		"Opt: sound vol");
	reg_v_Action<cva_Opt_MusicVolume>(		"Opt: music vol");
	reg_v_Action<cva_Opt_RequiredMsPerFrame_Scr>("Opt: maxFPS scr");
	reg_v_Action<cva_Opt_RequiredMsPerFrame_Dig>("Opt: maxFPS dig");
	reg_v_Action<cva_Opt_ScrollSpeed>(		"Opt: scroll speed");
	reg_v_Action<cva_Opt_MusicPlayMode>(	"Opt: music on/off");
	reg_v_Action<cva_Opt_WaterQuality>(		"Opt: water qual");
	reg_v_Action<cva_Opt_VMode>(			"Opt: video mode");	
	reg_v_Action<cva_Opt_PEBrightDegree>(		"Opt: bright degree");
	reg_v_Action<cva_Opt_PEBrightSpeed>(		"Opt: bright speed");
	reg_v_Action<cva_Opt_PEMotionBlur>(		"Opt: motion blur");
	REG_CLASS_EX(cva_Opt_PreSet,			"GameOptions");
	REG_CLASS_EX(cva_Opt_ColorBit,			"GameOptions");
	REG_CLASS_EX(cva_Opt_Antialiasing,		"GameOptions");
	REG_CLASS_EX(cva_Opt_ShaderQuality,		"GameOptions");
	REG_CLASS_EX(cva_Opt_BumpQuality,		"GameOptions");
	REG_CLASS_EX(cva_LANG_PARAM, "GameOptions");

	reg_v_Action<cva_F1_Object>(		"F1: object");
	REG_CLASS_EX(cva_F1_VictoryCond,	"GameHelp");
	reg_v_Action<cva_F1_History>(		"F1: history");
	reg_v_Action<cva_F1_Start>(			"F1: запуск");
	REG_CLASS_EX(cva_RomeHelp,			"GameHelp");	
	//
	reg_v_Action<cva_GM_MessPic>(		"Message: картинка");
	reg_v_Action<cva_GM_MessTxt>(		"Message: текст");

	REG_CLASS_EX(cva_ActAblDD,			"SelPoint");
	REG_CLASS_EX(cva_ActAbl,			"SelPoint");

	REG_CLASS_EX(cva_SwitchAttType,		"SP Commands");

	reg_v_Action<cva_MM_Profile>(		"MМеню: профиль");
	reg_v_Action<cva_MM_Campaign>(		"MМеню: компании");
	reg_v_Action<cva_SP_Activity0>(		"SP: activity 0");
	reg_v_Action<cva_SP_Activity12>(	"SP: activity 12");
	reg_v_Action<cva_SP_Patrol>(		"SP: patrol");
	reg_v_Action<cva_SP_Guard>(			"SP: guard");
	reg_v_Action<cva_SP_id_Life>(		"SP=ID: life");
	reg_v_Action<cva_SP_id_LifeLineHor>("SP=ID: life line hor");
	REG_CLASS_EX(cva_SP_id_Mana,		"SelPoint");
	reg_v_Action<cva_SP_id_Amount>(		"SP=ID: amount");	
	reg_v_Action<cva_WeapMiniDefPort>(	"SP: weap mini def type");
	reg_v_Action<cva_WeapMiniDef>(		"SP: weap mini defence");
	reg_v_Action<cva_WeapMiniDamPort>(	"SP: weap mini dam type");
	reg_v_Action<cva_WeapMiniDam>(		"SP: weap mini damage");
	reg_v_Action<cva_SP_EnableAttack>(	"SP: разр/запр атаку");
	reg_v_Action<cva_ResPanel_Money>(	"Панель ресур: деньги");
	reg_v_Action<cva_MM_ReplayCurMiss>(	"MainМ: replay cur sin mission");

	REG_CLASS_EX(cva_MM_Defeat_ReplayCurMiss,	"F12");
	REG_CLASS_EX(cva_MM_Defeat_LoadAutoSave,	"F12");

	reg_v_Action<cva_MM_BackToMain>(	"MainМ: back to main");

	REG_CLASS_EX(cva_MM_RunSingleMiss,	"MainМenu: Single");
	REG_CLASS_EX(cva_MM_PlayOgg,		"MainМenu: Single");
	REG_CLASS_EX(cva_MM_StopOgg,		"MainМenu: Single");

	reg_cva(cva_MM_RunRecordGame,"");	// MainМеню: запуск запис. демки	
	REG_CLASS_EX(cva_MM_RunCredits,		"MM");
	REG_CLASS_EX(cva_MM_RunShopingDemo,	"MM");	
	reg_v_Action<cva_MM_RunMapEditor>(	"MainМеню: запуск редактора карты");
	reg_v_Action<cva_MM_MultiEnter>(	"MainМеню: мульт вход");
	reg_v_Action<cva_MM_MultiBack>(		"MainМеню: мульт выход");
	//
	REG_CLASS_EX(cva_LD_MultiGame,			"Multi");
	REG_CLASS_EX(cva_Multi_ManualServer,	"Multi");
	REG_CLASS_EX(cva_Multi_ManualIPServer,	"Multi");
	//
	reg_v_Action<cva_MM_DialogsEditor>(	"MainМеню: редактор диалогов");
	reg_v_Action<cva_MM_MultiStart>(	"MainМеню: мульт старт");
	reg_v_Action<cva_MM_SinglStart>(	"MainМеню: сингл старт");
	REG_CLASS_EX(cva_PlayerInfoTab,		"MM");
	reg_v_Action<cva_MM_MultiCreate>(	"MainМеню: мульт. создать");
	reg_v_Action<cva_MM_MultiJoin>(		"MainМеню: мульт. присоед");
	reg_v_Action<cva_M_MultRoom_Back>(	"M: mult room back");
	REG_CLASS_EX(cva_MM_Start,			"MM");
	REG_CLASS_EX(cva_MM_StartInGame,	"MM");
	REG_CLASS_EX(cva_MM_StartInDemo,	"MM");
	reg_v_Action<cva_MM_StartInGameModal>("MМ: старт InGame modal");
	REG_CLASS_EX(cva_MM_Close,			"MM");	
	reg_v_Action<va_AR_MenuScroll>(		"ГлМеню: скролинг");
	reg_v_Action<va_SP_UpgPicture>(		"Прт: апгрейд картинка");
	reg_v_Action<va_SP_UpgBut>(			"Прт: апгрейд кнопка");
	reg_v_Action<va_SP_UpgStageLine>(	"Прт: апгрейд линия");	
	reg_v_Action<va_LD_Skirmish>(		"List of skirmish maps");
	reg_v_Action<va_LD_RoS>(		"List of RoS maps"); // <---------
	reg_v_Action<va_LD_Battle>(			"List of battle maps");
	reg_v_Action<va_vi_ReturnID>(		"set Name to vi_ReturnID");
	reg_v_Action<va_ListItem>(			"List Item");	// 
	reg_v_Action<va_ListDesk>(			"List Desk");	// dialog buttons
	reg_v_Action<va_DB_SetVarReturn>(	"Диалог: уст. Var_Return");	// dialog buttons
	reg_v_Action<va_GB_SelBuildings>(	"Глобал: выделить здания","Портрет");
	reg_v_Action<va_SP_MoraleSideBlink>("Прт: мораль бока цвет","Портрет");
	reg_v_Action<va_SP_MoraleSide>(		"Прт: мораль бока","Портрет");
	reg_v_Action<va_SP_MoraleSideText>(	"Прт: мораль бока текст","Портрет");
	reg_v_Action<va_SP_NameCircle>(		"Прт: имя кружки","Портрет");
	reg_v_Action<va_SP_NameCircleSide>(	"Прт: имя кружки бока","Портрет");
	reg_v_Action<va_SP_NameColor>(		"Прт: имя цвет","Портрет");
	REG_CLASS_EX(va_Unit_P_BuildMode2,	"Produce");
	REG_CLASS_EX(va_Unit_P_Unlimit2,	"Produce");
	REG_CLASS_EX(va_Unit_P_BuildMode,	"Produce");
	REG_CLASS_EX(va_Unit_P_Amount,		"Produce");
	REG_CLASS_EX(va_Unit_P_Unlimit,		"Produce");
	reg_v_Action<va_SP_UnitSprSide>(	"Прт: юнит карт бока","Портрет");
	reg_v_Action<va_SP_UnitNameSide>(	"Прт: юнит юмя","Портрет");
	REG_CLASS_EX(va_SP_SideBox,			"SelPoint"); //		"Портрет: бока подложка","Портрет");
	reg_v_Action<va_SP_BldOnly_Side>(	"Прт: здание только бока","Портрет");
	reg_v_Action<va_SP_ConstructMode>(	"Портрет: здание строится","Портрет");
	reg_v_Action<va_SP_ConstructedMode>("Портрет: здание построилось","Портрет");
	reg_v_Action<va_SP_B_Stage>(		"Портрет: здание стр цифры","Портрет");
	reg_v_Action<va_SP_B_StageLine>(	"Портрет: здание стр линия","Портрет");
	reg_v_Action<va_SP_B_Life>(			"Портрет: здание жизнь","Портрет");
	reg_v_Action<va_SP_B_Places>(		"Портрет: здание места","Портрет");
	reg_v_Action<va_SP_B_Population>(	"Портрет: здание население","Портрет");
	reg_v_Action<va_SP_BuildingOnly>(	"Портрет: только здания","Портрет");
	reg_v_Action<va_SP_CenUp_One>(		"Портрет: верх 1","Портрет");
	reg_v_Action<va_SP_CenUp_Mul>(		"Портрет: верх много","Портрет");
	reg_v_Action<va_SP_CenDown>(		"Портрет: низ","Портрет");
	reg_v_Action<va_SP_PortretBox>(		"Портрет: подложка","Портрет");
	REG_CLASS_EX(va_SP_PortretBox2,		"SelPoint"); //	"Портрет: подложка 3row","Портрет"
	REG_CLASS_EX(va_SP_PortretBox3,		"SelPoint"); //	"Портрет: подложка 3row","Портрет"
	reg_v_Action<va_SP_TiredLine>(		"Портрет: усталость линия","Портрет");
	reg_v_Action<va_SP_LifeLine>(		"Портрет: жизн линия","Портрет");
	reg_v_Action<va_SP_MoraleLine>(		"Портрет: мораль линия","Портрет");
	reg_v_Action<va_BR_StandGroundLine>("Бригада: стенд гроунд линия");
	reg_v_Action<va_MC_Discard>(		"МинКом: роспуск","МинКом");
	reg_v_Action<va_MC_Fill>(			"МинКом: пополнение","МинКом");
	reg_v_Action<va_MC_LineShot>(		"МинКом: стрел лин","МинКом");
	reg_v_Action<va_MC_Reform>(			"МинКом: перестройка","МинКом");
	reg_v_Action<va_W_CannonSet>(		"Оруж: пушка набор","Оружие");
	reg_v_Action<va_CannonFillUnits>(   "Пушка: Пополнение","Пушка");
	reg_v_Action<va_CannonUnLink>(      "Пушка: Отцепка","Пушка");
	REG_CLASS_EX(va_CannonReload,		"Пушка"); //"Пушка: перезарядка"
	reg_v_Action<va_CannonReloadStage>(	"Пушка: полоска перезарядки","Пушка");
	reg_v_Action<va_CannonFire>(		"Пушка: залп","Пушка");
	reg_v_Action<va_SP_Bld_BigPortret>(	"Прт: здание картинка");
	reg_v_Action<va_UnitPortret_Small>(	"Маленький портрет юнита","Юнит");
	reg_v_Action<va_UnitBigPortret>(	"Большая картинка юнита","Юнит");
	reg_v_Action<va_Unit_P_Box>(		"Юнит: постройка подложка","Юнит");
	reg_v_Action<va_UnitProdPort>(		"Юнит: постройка портрет","Юнит");
	reg_v_Action<va_UnitProdStage>(		"Юнит: постройка стадия","Юнит");
	reg_v_Action<va_WeapPort>(			"Оруж: портрет","Оружие");
	reg_v_Action<va_WeapPortBack>(		"Оруж: портрет рамка","Оружие");
	reg_v_Action<va_MiniComDesk>(		"Бригада: desk","Бригада");
	reg_v_Action<va_SP_BranchColor>(	"Портрет: цвет войск","Портрет");
	reg_v_Action<va_SP_BColorSide>(		"Портрет: цвет бока","Портрет");
	reg_v_Action<va_SP_BranchSprite>(	"Портрет: симвл войск","Портрет");
	reg_v_Action<va_SP_Amount>(			"Портрет: количество","Портрет");
	reg_v_Action<va_SP_AmountMini>(		"Портрет: количество мини","Портрет");
	reg_v_Action<va_SP_Kills>(			"Портрет: скальпы","Портрет");
	reg_v_Action<va_SP_KillsAward>(		"Портрет: скальп-награды","Портрет");
	REG_CLASS_EX(cva_SP_KillsGuardian,	"SelPoint");	
	reg_v_Action<va_SP_Morale>(			"Портрет: мораль","Портрет");
	reg_v_Action<va_SP_NatFlag>(		"Портрет: флаг","Портрет");
	reg_v_Action<va_SP_Protect>(		"Портрет: броня","Портрет");
	reg_v_Action<va_W_Damage>(			"Оруж: атака","Оружие");
	reg_v_Action<va_W_Charge>(			"Оруж: заряженные","Оружие");
	reg_v_Action<va_W_ChargeLine>(		"Оруж: заряд линия","Оружие");	
	// vit unit interface
	Enumerator* E=vui_Action::VUI=vui_Action::VUI=ENUM.Get("ve_VUI");
	E->Add("PUSHKA",DWORD(0));
	E->Add("POVOZKA",DWORD(1));
}
//////////////////////////////////////////////////////////////////////////
RLCFont* GetFont(const char* FontName);
extern WholeClassPresentation CE_WCP;
NewItem* DetectItem(word NIndex);
extern bool vDropReady;
void InitGame();
extern int RunMethod;
void PrepareGameMedia(byte myid,bool SaveNR,bool DisableDLL);
bool OnDrawMinimap(SimpleDialog* SD);
extern MapOptions MOptions;
bool CreateMultiplaterInterface();
extern CPlayerSAVE_STRUCT vNewProf;
CIMPORT void AddPulseBox(int x, int y);
extern int AnimTime;
void CmdLeaveMine(byte NI,word Type);
int GetRealTime();
void CmdSaveNetworkGame(byte NI,int ID,char* Name);
extern bool ContinueGame;
CIMPORT char PlName[64];
extern char CHATSTRING[256];
extern DWORD CHATDPID;
char* GetPlNameByID(DWORD ID);
void SendChat(char* str,bool Ally);
extern bool GameInProgress;
extern word NPlayers;
void SetGameDisplayModeAnyway(int,int);
extern int ModeLX[32];
extern int ModeLY[32];
void ShowBrigMiniBox(int x, int y);
extern int NModes;
extern bool NoText;
CEXPORT void SetMissionPulseZone(byte ID);
void CreateMissText(DString& Miss);
extern int VitCursor;
extern int VitCursorLowProirity;
bool ProcessBigMap(int campainID);
//bool PlayerInterface();
extern bool GUARDMODE;
extern bool PATROLMODE;
extern byte PlayGameMode;
DLLEXPORT int GetCurrentUnits(byte NI);
DLLEXPORT int GetMaxUnits(byte NI);
extern short WeaponIcn [32];
void CmdSetSrVictim(byte NI,byte val);
void GetCorrectMoney(byte NI,int* MONEY);
void REPLAY();
extern bool GameExit;
extern bool InGame;
void RunMission(int missID,int diff);
CEXPORT void PlayRecfile(char* RecFile);
void PrepareEditMedia(byte myid);
void EditGame();
void PlayGame();
extern CCommCore IPCORE;
extern GameSettings GSets;
void ProcessDialogsEditor();
void CmdStopUpgrade(byte NI, int UIndex);
void GetCostString(byte NI,word* Cost,char* str);
//void GetUpgCost(byte NI, word UIndex, word* &Cost);
bool GetPushkaChargeState(OneObject*,int&,int&);
void CmdSetOrderedStateForComplexObject(OneObject* OB,int State);
void CmdAttackGround(byte NI,int x,int y);
DLLEXPORT void SetStartPoint(GAMEOBJ* Zone);
extern bool AttGrMode;
bool CheckCostHint(byte,word,bool Message=false);
extern bool BuildMode;
extern word BuildingID;
extern Nation* BNat;
void UnPress();
void CmdUnProduceObj(byte,word);
void GetMonCost(byte,word,char*);
void FILLFORM(int i);
void DelAbil(int i);
void UseAbil(int i);
void ThrowGrenade(int i);
void ENBLINE(int i);
void REFORMA(int i);
void SETATTSTATE_Pro(int i);
void FREEORD(int i);
void CmdSelUnitsSet(byte NI,word* BUF,int NU);
void CmdUnSelUnitsSet(byte NI,word* BUF,int NU);
extern MissPack MISSLIST;
extern SinBattles SBattles;
extern bool InternetGame;
extern bool UseGSC_Login;
extern int CurProtocol;
void LimitString(char* str,lpRLCFont FONT,int L);
ActiveUnitAbility* AddAbility(UnitAbility* Source, OneObject* Dest, int TypeAbil=2);
CEXPORT void DrawRotatedPicture(int x,int y,int File,int Sprite,byte Nat,int dx,int dy,float Angle,float sx,float sy);

HeroVariableStorage* DetectHero(OneObject* OB);
bool DetectMagazine(OneObject* OB);

//
DIALOGS_API veGameMode vGameMode;
CEXPORT bool isInCampaign(){
	return cva_InGameMenu_MainDesk::CurrentID==0;
}
//
tpHelpMessegesMiss* vGetHelp=NULL;
CEXPORT	void SetHelpMessagesArrays(tpHelpMessegesMiss* dc){
	vGetHelp=dc;
};
//  [2/5/2004]
void CenterSP(vui_SelPoint* SP){
	if(SP){
		GAMEOBJ Z;
		Z.CreateZone(SP->rX>>4,SP->rY>>4,100);
		SetStartPoint(&Z);
	}
}
void CmdSelObject(OneObject* OB){
	if(OB){
		byte NI=MyNation;//OB->NNUM;
		PrepareToImSelection(NI);
		word* N=ImNSL+NI;
		if(*N==0){
			ImSelm[NI]=new word;
			ImSerN[NI]=new word;
		}
		word* id=ImSelm[NI];
		word* sn=ImSerN[NI];
		byte Mask=~(1<<NI);
		for(int i=0;i<*N;i++){
			OneObject* ob=Group[id[i]];
			if(ob&&ob->Serial==sn[i]){
				ob->ImSelected&=Mask;
			}			
		}
		*N=1;
		id[0]=OB->Index;
		sn[0]=OB->Serial;
		OB->ImSelected|=~Mask;
		FinalizeImSelection(NI);
	}
}
void CmdUnSelObject(OneObject* OB){
	if(OB){
		byte NI=MyNation;//OB->NNUM;		
		word* N=ImNSL+NI;
		word* id=ImSelm[NI];
		word* sn=ImSerN[NI];
		byte Mask=~(1<<NI);
		for(int i=0;i<*N;i++){
			if(id[i]==OB->Index&&sn[i]==OB->Serial){				
				PrepareToImSelection(NI);
				id[i]=0xFFFF;
				OB->ImSelected&=Mask;
				FinalizeImSelection(NI);
				return;
			}
		}				
	}
}
void SelSP(vui_SelPoint* SP){
	if(SP){
		if(SP->Type==ve_UT_brigade){
			word BID=SP->Inf.Brigade.BrigID;
			if(GetKeyState(VK_SHIFT)&0x8000) CmdChooseUnSelBrig(MyNation,BID);//SP->NI
			else CmdChooseSelBrig(MyNation,BID);
		}else if(SP->Type==ve_UT_cannon){
			word ID[1];
			vui_CannInfo* I=&SP->Inf.Cannon;
			if(GetKeyState(VK_SHIFT)&0x8000){
				CmdUnSelObject(I->OB);
			}else{
				CmdSelObject(I->OB);
			}
		}else{
			if(GetKeyState(VK_SHIFT)&0x8000) CmdChooseUnSelType(MyNation,SP->NIndex);
			else CmdChooseSelType(MyNation,SP->NIndex);
		}		
	}	
}
//////////////////////////////////////////////////////////////////////////
_str vLoadFile;
bool LoadInGame(){
	if(vLoadFile.pchar()){
		if(strstr(vLoadFile.pchar(),".rec")){
			char drive		[_MAX_DRIVE];
			char directory	[_MAX_DIR  ];
			char filename	[_MAX_PATH ];
			char ext		[_MAX_EXT  ];
			char rec		[_MAX_PATH ];

			_splitpath( vLoadFile.pchar(), drive, directory, filename, ext );
			sprintf(rec,"records\\%s.rec",filename);
			void PlayRecfile(char*);
			PlayRecfile(rec);
			return true;

		}else
			if(vLoadFile.pchar()[0]){
				SaveBuf SB;
				char ccc[128];
				if(_strnicoll(vLoadFile.pchar(),"Save\\",5)!=0){
					sprintf(ccc,"Save\\%s",vLoadFile.pchar());
				}else{
					strcpy(ccc,vLoadFile.pchar());
				}
				char* GetTrueMapName(char* Name);
				ResFile f1=RReset(GetTrueMapName(ccc));
				if(f1!=INVALID_HANDLE_VALUE){
					SB.LoadFromFile(f1);
					DLL2(SFLB_PreLoadGame,&SB,true);
				}
				RClose(f1);
				ContinueGame=true;
				vLoadFile.pchar()[0]=0;
			}
	}else{
		vLoadFile="";
	}
	return false;
}
//
struct SavePreload{

	//
	int Header; 
	int Version;
	int i;
	int NPlayers;
	DWORD NatMask;
	char GameName[128];
	float fMapX;
	float fMapY;
	char Names[8][32];
	byte Colors[8];
	byte NatRefTBL[8];
	//
	char FN[64];
	//
	void Preload(char* FileName);
} vSaveInfo;
//
void SavePreload::Preload(char* FileName){
	if(!FileName) return;

	SaveBuf SB;

	char ccc[128];
	if(_strnicoll(FileName,"Save\\",5)!=0){
		sprintf(ccc,"Save\\%s",FileName);
	}else{
		strcpy(ccc,FileName);
	}
	if(strcmp(FN,ccc)==0&&NPlayers>0) return;
	strcpy(FN,ccc);
	//
	char* GetTrueMapName(char* Name);
	ResFile f1=RReset(GetTrueMapName(ccc));
	if(f1!=INVALID_HANDLE_VALUE){
		SB.LoadFromFile(f1);
		//
		//void xBlockRead(SaveBuf* SB,void* Data,int Size);
		xBlockRead(&SB,&Header,4);		
		//if(Header!=sfHeader)return;
		xBlockRead(&SB,&Version,4);
		extern int sfVersion;
		if(Version!=sfVersion)return;
		xBlockRead(&SB,&i,4);
		NPlayers=0;
		xBlockRead(&SB,&NPlayers,2);
		xBlockRead(&SB,&NatMask,4);
		int sl=0;
		xBlockRead(&SB,&sl,1);
		xBlockRead(&SB,GameName,sl);
		xBlockRead(&SB,&fMapX,4);
		xBlockRead(&SB,&fMapY,4);
		//-------------
		for(int i=0;i<NPlayers;i++){
			xBlockRead(&SB,Names[i],32);
			xBlockRead(&SB,Colors+i,1);
		};
		xBlockRead(&SB,NatRefTBL,8);
	}
	RClose(f1);	
}
//////////////////////////////////////////////////////////////////////////
Enumerator* vui_Action::E=NULL;
Enumerator* vui_Action::VUI=NULL;
//
bool vui_UserLeftClick(SimpleDialog* SD){	
	return ((vui_Action*)SD->v_ActionID)->LeftClick(SD);
}
bool vui_UserRightClick(SimpleDialog* SD){	
	return ((vui_Action*)SD->v_ActionID)->RightClick(SD);
}
////////////////////////////////////////////////////////////////
// va_SP_MoraleSideBlink
void va_SP_MoraleSideBlink::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	ParentFrame* PF=SD->ParentDS;
	if(PF) PF=PF->ParentDS; else return;
	if(PF) PF=PF->ParentDS; else return;
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP&&SP->Type==ve_UT_brigade){
		vui_BrigInfo* I=&SP->Inf.Brigade;
		if(I->Tiring<30&&(GetTickCount()&1023)<512){
			SD->Visible=true;
		}
	}
}
// va_SP_MoraleSide
void va_SP_MoraleSide::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	ParentFrame* PF=SD->ParentDS;
	if(PF) PF=PF->ParentDS; else return;
	//if(PF) PF=PF->ParentDS; else return;
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP&&SP->Type==ve_UT_brigade){
		SD->Visible=true;
	}
}
// va_SP_MoraleSideText
void va_SP_MoraleSideText::SetFrameState(SimpleDialog* SD){
	//SD->Visible=false;
	ParentFrame* PF=SD->ParentDS;
	if(PF) PF=PF->ParentDS; else return;
	if(PF) PF=PF->ParentDS; else return;
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP&&SP->Type==ve_UT_brigade){
		vui_BrigInfo* I=&SP->Inf.Brigade;
		TextButton* TB=(TextButton*)SD;
		_str txt;
		txt.Add(I->Morale);
		TB->SetMessage(txt.pchar());
	}
}
// va_SP_NameCircle
void va_SP_NameCircle::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	ParentFrame* PF=SD->ParentDS;
	if(PF) PF=PF->ParentDS; else return;
	//if(PF) PF=PF->ParentDS; else return;
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP){
		GPPicture* P=(GPPicture*)SD;
		static int spr=P->SpriteID;
		P->SetSpriteID(spr);
		if(SP->Type==ve_UT_brigade){
			P->SetSpriteID(spr+3);
		}else
			if(SP->Type==ve_UT_units){
				if(SP->Inf.Units.Peasant) P->SetSpriteID(spr+1);
				else P->SetSpriteID(spr+2);
			}
			SD->Visible=true;
	}
}
// va_SP_NameCircleSide
void va_SP_NameCircleSide::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	ParentFrame* PF=SD->ParentDS;
	if(PF) PF=PF->ParentDS; else return;
	//if(PF) PF=PF->ParentDS; else return;
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP){
		GPPicture* P=(GPPicture*)SD;
		static int spr=P->SpriteID;
		P->SetSpriteID(spr);
		if(SP->Type==ve_UT_units){
			if(SP->Inf.Units.Peasant) P->SetSpriteID(spr+2);
			else P->SetSpriteID(spr+1);
		}
		SD->Visible=true;
	}
}
// va_SP_NameColor
void va_SP_NameColor::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	ParentFrame* PF=SD->ParentDS;
	if(PF) PF=PF->ParentDS; else return;
	//if(PF) PF=PF->ParentDS; else return;
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP){
		GPPicture* P=(GPPicture*)SD;
		static int spr=P->SpriteID;
		P->SetSpriteID(spr);
		if(SP->Type==ve_UT_brigade){
			P->SetSpriteID(spr+1);
		}else if(SP->Type==ve_UT_units){
			if(SP->Inf.Units.Peasant) P->SetSpriteID(spr+2);
		}
		SD->Visible=true;
	}
}
// va_SP_UnitSprSide
void va_SP_UnitSprSide::SetFrameState(SimpleDialog* SD){
	//SD->Visible=false;
	ParentFrame* PF=SD->ParentDS;
	if(PF) PF=PF->ParentDS; else return;
	if(PF) PF=PF->ParentDS; else return;
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP){
		NewMonster* NM=NATIONS->Mon[SP->NIndex]->newMons;
		if(NM&&NM->BigIconFile!=0xFFFF){
			GPPicture* BT=(GPPicture*)SD;
			BT->FileID=NM->BigIconFile;
			BT->SetSpriteID(NM->BigIconIndex);
		}
	}
}

inline char* NewMonster_GetMessage(NewMonster* NM){
	return (NM->Message&&NM->Message[0]!=0)?NM->Message:(NM->NewParams.Message?NM->NewParams.Message:"");//(Message?Message:"");
};

// va_SP_UnitNameSide
void va_SP_UnitNameSide::SetFrameState(SimpleDialog* SD){
	//SD->Visible=false;
	ParentFrame* PF=SD->ParentDS;
	if(PF) PF=PF->ParentDS; else return;
	if(PF) PF=PF->ParentDS; else return;
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP){
		TextButton* TB=(TextButton*)SD;
		NewMonster* NM=NATIONS->Mon[SP->NIndex]->newMons;
		if( SP->OB->ActiveAbility )
			NM=SP->OB->ActiveAbility->BornNM;
		if(strcmp(NewMonster_GetMessage(NM),TB->Message)){
			TB->SetMessage(NewMonster_GetMessage(NM));
			if(NM->GetLongMessage()){
				TB->SetHint(NM->GetLongMessage());
			}
		}
	}
}
// va_SP_ConstructMode
void va_SP_ConstructMode::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_building){
		vui_BuildInfo* I=&SP->Inf.Buildings;
		SD->Visible=(I->Stage<I->StageMax);
	}	
}
// va_SP_ConstructedMode
void va_SP_ConstructedMode::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	va_SP_ConstructMode::SetFrameState(SD);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_building){
		SD->Visible=!SD->Visible;
	}
}
// va_SP_B_Stage
void va_SP_B_Stage::SetFrameState(SimpleDialog* SD){
	va_SP_ConstructMode::SetFrameState(SD);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SD->Visible&&SP){
		TextButton* T=(TextButton*)SD;
		_str Txt;
		vui_BuildInfo* I=&SP->Inf.Buildings;
		Txt.Add(I->Stage);
		Txt+="/";
		Txt.Add(I->StageMax);
		if(Txt.pchar())	T->SetMessage(Txt.pchar());
	}	
}
// va_SP_B_StageLine
void va_SP_B_StageLine::SetFrameState(SimpleDialog* SD){
	va_SP_ConstructMode::SetFrameState(SD);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SD->Visible&&SD->DSS.GetAmount()>0){
		GPPicture* P=(GPPicture*)SD;
		DialogsDesk* D=(DialogsDesk*)SD->DSS[0];
		vui_BuildInfo* I=&SP->Inf.Buildings;
		int L=P->GetWidth()*I->Stage/I->StageMax;
		D->SetWidth(L);
	}	
}
// va_SP_B_Life
void va_SP_B_Life::SetFrameState(SimpleDialog* SD){
	va_SP_ConstructedMode::SetFrameState(SD);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SD->Visible&&SP){
		TextButton* T=(TextButton*)SD;
		_str Txt;
		vui_BuildInfo* I=&SP->Inf.Buildings;
		int a=I->Amount;
		if(a==0)a=1;
		Txt.Add(I->Life/a);
		Txt+="/";
		Txt.Add(I->LifeMax);
		if(Txt.pchar())	T->SetMessage(Txt.pchar());
	}	
}
// va_SP_B_Places
void va_SP_B_Places::SetFrameState(SimpleDialog* SD){
	va_SP_ConstructedMode::SetFrameState(SD);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SD->Visible&&SP){
		TextButton* T=(TextButton*)SD;
		_str Txt;
		vui_BuildInfo* I=&SP->Inf.Buildings;
		Txt.Add(I->Places);
		if(Txt.pchar())	T->SetMessage(Txt.pchar());
	}	
}
// va_SP_B_Population
void va_SP_B_Population::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->Type==ve_UT_building&&SP->NI==NI){
		TextButton* T=(TextButton*)SD;
		_str Txt;
		vui_BuildInfo* I=&SP->Inf.Buildings;
		Txt.Add(I->Population);
		Txt+="/";
		Txt.Add(I->PopulationMax);
		if(Txt.pchar())	T->SetMessage(Txt.pchar());
		SD->Visible=true;
	}	
}
// va_SP_BuildingOnly
void va_SP_BuildingOnly::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_building){
		SD->Visible=true;
		vui_BuildInfo* I=&SP->Inf.Buildings;
		if(SP->OB->newMons->ResConcentrator && SP->NI==GSets.CGame.cgi_NatRefTBL[MyNation] && I->Ready){
			extern bool vPeasant_Idle;
			extern bool vPeasant_AutoWork;
			vPeasant_Idle=true;
			vPeasant_AutoWork=true;
		}
	}else{
		SD->Visible=false;
	};	
}
// va_SP_CenUp_One
void va_SP_CenUp_One::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	SD->Visible=SP&&(SP->Type==ve_UT_units&&SP->Inf.Units.Amount==1||
		SP->Type==ve_UT_building&&SP->Inf.Buildings.Amount==1||
		SP->Type==ve_UT_cannon);
}
// va_SP_CenUp_Mul
void va_SP_CenUp_Mul::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	SD->Visible=SP&&(SP->Type==ve_UT_units&&SP->Inf.Units.Amount>1||
		SP->Type==ve_UT_brigade||
		SP->Type==ve_UT_building&&SP->Inf.Buildings.Amount>1);
}
// va_SP_CenDown
void va_SP_CenDown::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		GPPicture* P=(GPPicture*)SD;
		static int spr=P->SpriteID;
		if(	SP->Type==ve_UT_building){
			P->SetSpriteID(spr+1);
		}else{
			P->SetSpriteID(spr);
		}
	}
}
// cva_OIS_ArtDepo
void cva_OIS_ArtDepo::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(SP&&TB){
		TB->Visible=false;
		OneObject* OBJ=SP->OB;
		NewMonster* NM=OBJ->newMons;
		if(NM->ArtDepo){
			extern char* ARTCAPS[8];
			if(!ARTCAPS[0]){
				ARTCAPS[0]=GetTextByID("__ARTCAP0");
				ARTCAPS[1]=GetTextByID("__ARTCAP2");
				ARTCAPS[2]=GetTextByID("__ARTCAP1");
				ARTCAPS[3]=GetTextByID("__ARTCAP3");
			};
			static _str txt;
			txt.Clear();
			Nation* NT=OBJ->Nat;
			for(int i=0;i<4;i++){
				if(ARTCAPS[i][0]!='_'&&NM->ArtCap[i]){
					txt.print(ARTCAPS[i],NT->NArtUnits[i],NM->ArtCap[i]*NT->NArtdep[i]);
					txt+="\\";
				}
			};
			if(!txt.isClear()){
				TB->Visible=true;
				TB->SetMessage(txt.pchar());
			}			
		};
	}
}
// va_SP_TiredLine
void va_SP_TiredLine::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	Canvas* C=(Canvas*)SD;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	int Ly=0;
	DWORD c=0xFFEF1212;
	if(SP&&SP->Type==ve_UT_brigade){
		vui_BrigInfo* I=&SP->Inf.Brigade;
		Ly=C->GetHeight()*I->Tiring/100;
		if(I->Tiring<45){
			//DWORD cc=((GetTickCount()/4)&0xFF)*0x40/0xFF;
			//c-=cc*0x10000;
			//MixDWORD(0xFFFFFFFF,C,30,255-30);
			static int t=GetTickCount();
			float a=sin(float(GetTickCount()-t)/200)*60-20;//+60;
			//if(a<0) a=0;
			//c=MixDWORD(0xFFFFFFFF,c,int(a),255-int(a));
			c=MulDWORD(c,255+int(a));

		}
	}
	if(Ly>0){
		C->L=0;		
		C->AddBar(0,C->GetHeight()-Ly,C->GetWidth(),Ly,c);
		SD->Visible=true;
	}	
};
// va_SP_LifeLine
void va_SP_LifeLine::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	Canvas* C=(Canvas*)SD;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	int Ly=0;
	if(SP&&SP->Type==ve_UT_brigade){
		vui_BrigInfo* I=&SP->Inf.Brigade;
		Ly=C->GetHeight()*I->Life/I->MaxLife;
	}
	if(Ly>0){
		C->L=0;
		C->AddBar(0,C->GetHeight()-Ly,C->GetWidth(),Ly,0xFF00FF00);
		SD->Visible=true;
	}	
};
// va_SP_MoraleLine
bool SetMorale(Canvas* C, int Morale, int MoraleMax){
	if(C){
		int n=Morale/100;
		int m=Morale%100;
		int M=MoraleMax-n*100;
		if(M>100)M=100;
		float a=0;
		if(Morale<45){
			static int t=GetTickCount();
			a=sin(float(GetTickCount()-t)/200)*60-20;
		}
		//
		if(n+M>0&&n<10){
			int Lx=m*C->GetWidth()/100;
			int LX=M*C->GetWidth()/100;
			int Lr=0;
			if(n==0){
				if(m<32) Lr=m*C->GetWidth()/100;
				else Lr=32*C->GetWidth()/100;
			}
			int L=C->GetHeight();
			C->L=0;
			for(int i=0;i<L;i++){
				if(Lr>0) C->AddLine(0,i,Lr,i,MulDWORD(0xFFFD2828,255+int(a)));
				C->AddLine(Lr,i,Lx,i,MulDWORD(0xFFFDC407,255+int(a)));
				if(LX>Lx) C->AddLine(Lx,i,LX,i,0x8FFDC407);
			}
			if(n){
				DWORD c=0xFFAF0000;
				int W=L-1;
				int x=(C->GetWidth()-(n+n-1)*W)/2;
				for(int i=0;i<n;i++){
					int xx=x+i*2*W;
					for(int j=0;j<W;j++)
						C->AddLine(xx+j+L,0,xx+j,L,c);
				}
			}			
			return true;
		}
	}
	return false;
}
void va_SP_MoraleLine::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	Canvas* C=(Canvas*)SD;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	if(SP){
		if(SP->Type==ve_UT_brigade){
			vui_BrigInfo* I=&SP->Inf.Brigade;
			SD->Visible=SetMorale(C,I->Morale,I->MaxMorale);
		}else if(SP->Type==ve_UT_units){
			vui_UnitInfo* I=&SP->Inf.Units;
			SD->Visible=SetMorale(C,I->Morale,I->MoraleMax);
		}
	}	
};
// va_BR_StandGroundLine
void va_BR_StandGroundLine::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	Canvas* C=(Canvas*)SD;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	int Lx=0;
	if(SP&&SP->Type==ve_UT_brigade){
		vui_BrigInfo* I=&SP->Inf.Brigade;
		if(I->BrigDelayMax) Lx=C->GetWidth()*(I->BrigDelayMax-I->BrigDelay)/I->BrigDelayMax;
	}
	if(Lx>0){
		C->L=0;
		C->AddBar(0,0,Lx,C->GetHeight(),0xDFFF0000);
		SD->Visible=true;
	}	
};
// va_MC_Discard
void va_MC_Discard::SetFrameState(SimpleDialog* SD){
	SD->Visible=vBrigPanel.Disband;
}
bool va_MC_Discard::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_brigade){
		FREEORD(SP->Inf.Brigade.BrigID);
		ACT(16);
	}
	return true;
};
// va_MC_Fill
void va_MC_Fill::SetFrameState(SimpleDialog* SD){
	SD->Visible=vBrigPanel.Fill;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_brigade){
		if(EngSettings.MoraleType==0){
			vui_BrigInfo* I=&SP->Inf.Brigade;
			SD->Diffuse=I->isFillable?0xFFFFFFFF:0x80808080;
			SD->UserParam=I->BrigID;
		}else{
			vui_BrigInfo* I=&SP->Inf.Brigade;
			SD->Diffuse=0xFFFFFFFF;
			SD->UserParam=I->BrigID;
		}
	}
}
bool va_MC_Fill::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_brigade&&(SP->Inf.Brigade.isFillable||EngSettings.MoraleType!=0)){
		FILLFORM(SD->UserParam);
		ACT(17);
	}
	return true;
};
// va_MC_LineShot
void va_MC_LineShot::OnDraw(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SD->MouseOver){
		if(SP&&SP->Type==ve_UT_brigade){ //&&SP->OB->newMons->PortBranch==0
			vui_BrigInfo* I=&SP->Inf.Brigade;
			int SL=I->ShotLine[SD->ID];
			if(SL>=1*8*1024){
				SL-=1*8*1024;
			}			
			FList.AddAttackRangesOfBrigade(I->BR,SL); //SD->ID
		}
	}
}
void va_MC_LineShot::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	SD->Diffuse=0x80808080;
	if(SP&&SP->Type==ve_UT_brigade&&SP->OB->newMons->PortBranch==0){
		vui_BrigInfo* I=&SP->Inf.Brigade;
		if(I->FormID[0]!=255&&I->Damage[1]>0){
			SD->Visible=true;
			static char* Enable[3]={ GetTextByID("PRESS_TO_ENABLE_SHOT_LINE0"), GetTextByID("PRESS_TO_ENABLE_SHOT_LINE1"), GetTextByID("PRESS_TO_ENABLE_SHOT_LINE2") };
			static char* Disable[3]={ GetTextByID("PRESS_TO_DISABLE_SHOT_LINE0"), GetTextByID("PRESS_TO_DISABLE_SHOT_LINE1"), GetTextByID("PRESS_TO_DISABLE_SHOT_LINE2") };
			static char* NAllowed[3]={ GetTextByID("SHOT_LINE_NOT_ALLOWED0"), GetTextByID("SHOT_LINE_NOT_ALLOWED1"), GetTextByID("SHOT_LINE_NOT_ALLOWED2") };
			//static char* Enable1;
			//static char* Enable2;		
			int SL=I->ShotLine[SD->ID];		
			if(SL!=0xFFFF&&SP->OB->newMons->PortBranch!=1){
				SD->Diffuse=0xFFFFFFFF;
				if(SD->DSS.GetAmount()){
					SimpleDialog* sd=SD->DSS[0];
					if(sd){
						if(SL<=1024){
							sd->Visible=true;
							SD->SetHint(Enable[SD->ID],true);
						}else{
							sd->Visible=false;
							SD->SetHint(Disable[SD->ID],true);
						}
					}

				}
			}else{
				SD->SetHint(NAllowed[SD->ID],true);
			}
		}
	}
	SD->Visible&=vBrigPanel.Formation;
}
bool va_MC_LineShot::LeftClick(SimpleDialog* SD){	
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_brigade){
		vui_BrigInfo* I=&SP->Inf.Brigade;
		if(SD->UserParam!=0xFFFF){
			int l=I->ShotLine[SD->ID];
			ENBLINE(l+int(I->BrigID)*8);
			if(OIS.SelPoint.GetAmount()==1){
				if(l>=8192){
					ACT(10);
				}else{
					ACT(9-(l&7)/2);
				}
			}
		}
	}
	return true;
};
// cva_Guardian
void cva_Guardian::SetFrameState(SimpleDialog* SD){
	//SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	//TextButton* TB=dynamic_cast<TextButton*>(SD);
	//if(SP&&SP->Type==ve_UT_brigade&&SP->Inf.Brigade.NKills>=250){
	SD->Visible = SP&&SP->Type==ve_UT_brigade&&SP->Inf.Brigade.NKills>=250;
	//}
}
// cva_SP_Stop
void cva_SP_Stop::SetFrameState(SimpleDialog* SD){
	SD->Visible=true;
	SD->Visible&=vBrigPanel.Stop;
}
bool cva_SP_Stop::LeftClick(SimpleDialog* SD){
	//byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	vui_IS_Sel_ClearOrders Com;
	Com.Data.NI=MyNation;
	Com.InterfAction();
	ACT(18);
	return true;
}
// cva_SP_Stop_Cannon
void cva_SP_Stop_Cannon::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI&&SP->Type==ve_UT_cannon&&!TestFillingAbility(SP->OB)){
		SD->Visible=true;
	}
}
bool cva_SP_Stop_Cannon::LeftClick(SimpleDialog* SD){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	vui_IS_CancelAttack com;
	for(int i=0;i<OIS.SelPoint.GetAmount();i++){
		vui_SelPoint* SP=OIS.SelPoint+i;		
		if(SP&&SP->Type==ve_UT_cannon&&SP->NI==NI){
			com.Data.ObjID=SP->OB->Index;
			com.Data.ObjSN=SP->OB->Serial;
			com.InterfAction();
		}
	}
	return true;
}
// va_MC_Reform
void va_MC_Reform::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	//vui_SelPoint* SP=NULL;
	//if(OIS.SelPoint.GetAmount()) SP=OIS.SelPoint+0;
	if(SP&&SP->Type==ve_UT_brigade){
		SD->Visible=true;
		vui_BrigInfo* I=&SP->Inf.Brigade;
		int id=SD->ID-100;
		if(id>=0&&id<3){			
			VitButton* VB=dynamic_cast<VitButton*>(SD);
			int Scale[3]={EngSettings.LowerFormationDistanceScale,100,EngSettings.GreaterFormationDistanceScale};
			if(VB){
				if(Scale[id]==I->ScaleFactor) VB->State=1;
				else VB->State=0;
			}else{
				if(SD->DSS.GetAmount()){
					SimpleDialog* sd=SD->DSS[0];
					if(sd) sd->Visible=(id!=I->ScaleFactor);
				}
			}		
		}
		id=SD->ID;
		if(id>=0&&id<3){
			if(I->FormID[id]!=255){
				SD->Visible=true;
				SD->SetHint(OrderDesc[I->FormID[id]].Message,true);			
				if(SD->DSS.GetAmount()){
					GPPicture* GP=dynamic_cast<GPPicture*>(SD->DSS[0]);
					if(GP){
						//GP->Visible=(I->CurForm!=I->FormID[SD->ID]);
						GP->Visible=(I->CurForm!=SD->ID);
					}
				}
				GPPicture* gpI=dynamic_cast<GPPicture*>(SD);
				if(gpI){
					gpI->SetSpriteID(OrderDesc[I->FormID[id]].IconID);
				}
			}else{
				SD->Visible=false;
			}			
		}
	}
	SD->Visible&=vBrigPanel.Formation;
}
bool va_MC_Reform::LeftClick(SimpleDialog* SD){
	if(SD->ID<3||SD->ID==100||SD->ID==101||SD->ID==102){
		for(int i=0;i<OIS.SelPoint.GetAmount();i++){
			vui_SelPoint* SP=OIS.SelPoint+i; //OIS.GetLastSelPoint();
			if(SP&&SP->Type==ve_UT_brigade){
				vui_BrigInfo* I=&SP->Inf.Brigade;
				DWORD id;
				if(SD->ID==100)id=I->BrigID+(0xFD<<16);
				else if(SD->ID==101)id=id=I->BrigID+(0xFE<<16);
				else if(SD->ID==102)id=id=I->BrigID+(0xFF<<16);
				else id=I->BrigID+(I->FormID[SD->ID]<<16);
				REFORMA(id);
				ACT(20+I->FormID[SD->ID]);
			}
		}
	}
	return true;
};
// va_CannonReload
bool vCannAuto=false;
void va_CannonReload::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	GPPicture* _VUB=(GPPicture*)SD;
	_VUB->Visible=false;
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->Type==ve_UT_cannon){
		/*
		SD->HotKey=HotKey;
		if(!SD->Hint){
		_str txt;
		//char key[2]={HotKey,0};
		//txt.print(Hint,key);
		SD->SetHint(&txt,true);
		}
		*/
		vui_CannInfo* I=&SP->Inf.Cannon;
		OneObject* _OB=SP->Inf.Cannon.OB;
		int n=0;
		for(int i=0;i<3;i++){
			if(I->Damage[i]) n++;
		}
		if(n>1&&_OB&&I->Damage[SD->ID]>0&&!TestFillingAbility(_OB)){	
			int CType,CStage;
			if(GetPushkaChargeState(_OB,CType,CStage)){
				_VUB->Visible=true;
				GPPicture* sp=NULL;
				if(SD->DSS.GetAmount()>0) sp=dynamic_cast<GPPicture*>(SD->DSS[0]);
				if(sp){
					sp->Visible=CType!=SD->ID;
				}else{
					if(CType!=SD->ID){
						_VUB->Diffuse=0x60FFFFFF;
					}else{
						_VUB->Diffuse=0xFFFFFFFF;
					}
				}				
				_VUB->UserParam=1;//(CStage==100)||vCannAuto;
				_VUB->DeepColor=true;
				if(_VUB->UserParam||CType==SD->ID){					
					if(SP->NI==NI) SD->Diffuse=0xFFFFFFFF;
					else SD->Diffuse=0x80FFFFFF;
				}else{					
					if(SP->NI==NI) SD->Diffuse=0x80FFFFFF;
					else SD->Diffuse=0x40FFFFFF;
				}
			}
		}		
	}	
};
bool va_CannonReload::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_cannon&&SD->UserParam){
		for(int i=0;i<OIS.SelPoint.GetAmount();i++){
			SP=OIS.SelPoint+i;
			byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
			if(SP&&SP->Type==ve_UT_cannon&&SP->NI==NI){			
				vui_CannInfo* I=&SP->Inf.Cannon;
				CmdSetOrderedStateForComplexObject(I->OB,SD->ID);
				if(SD->ID==0){
					ACT(102);
				}else
					if(SD->ID==1){
						ACT(103);
					}				
			}
		}
	}
	return true;
};
// va_CannonReloadStage
void va_CannonReloadStage::SetFrameState(SimpleDialog* SD){
	Canvas* C=(Canvas*)SD;
	C->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	if(SP){
		vui_CannInfo* vC=&SP->Inf.Cannon;
		C->L=0;
		int Ly=C->GetHeight()*vC->ChargeStage/100;
		C->AddBar(0,C->GetHeight()-Ly,C->GetWidth(),Ly,0xFF00FF00);
		C->Visible=true;
	}	
};
// va_CannonFillUnits
OneObject* vCannonOB=NULL;
bool ShowCannonFillRadius(){
	if(vCannonOB){
		void DrawCannonFillRadius(OneObject* OB);
		DrawCannonFillRadius(vCannonOB);
		vCannonOB=NULL;
		return true;
	}
	return false;
}
void va_CannonFillUnits::OnDraw(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SD->MouseOver){
		vCannonOB=SP->OB;
	}
}
void va_CannonFillUnits::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI){
		SD->Visible=TestFillingAbility(SP->OB);
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		if(SP->NI==NI) SD->Diffuse=0xFFFFFFFF;
		else SD->Diffuse=0x80FFFFFF;
		SD->DeepColor=true;
	}
}
bool va_CannonFillUnits::LeftClick(SimpleDialog* SD){	
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI){
		vui_IS_FillCannon temp;
		temp.Data.NI=MyNation;
		temp.InterfAction();
		ACT(106);
	}
	return true;
}
// va_CannonUnLink
void va_CannonUnLink::SetFrameState(SimpleDialog* SD){
	SD->Visible=0;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		bool TestDisconnectAbility(word HeadID);
		SD->Visible=TestDisconnectAbility(SP->OB->Index);
		return;
		//
		int N=ImNSL[SP->NI];
		word* IDS=ImSelm[SP->NI];
		word* SNS=ImSerN[SP->NI];
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]){
					bool TestDisconnectAbility(word HeadID);
					if(TestDisconnectAbility(MID)){
						SD->Visible=true;
						return;
					}
				}
			}
		}
	}
}
bool va_CannonUnLink::LeftClick(SimpleDialog* SD){
	void CmdDisconnectObjs(byte NI);	
	void DisconnectFromObject(OneObject* Head);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI){
		CmdDisconnectObjs(SP->NI);
	}
	return true;
}
// va_W_CannonSet
void va_W_CannonSet::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	SD->Visible=SP&&SP->Type==ve_UT_cannon&&!TestFillingAbility(SP->OB);
	//
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP->NI==NI) SD->Diffuse=0xFFFFFFFF;
	else SD->Diffuse=0x80FFFFFF;
	SD->DeepColor=true;
}
// va_CannonFire
void va_CannonFire::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	GPPicture* P=(GPPicture*)SD;
	P->Visible=false;
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->Type==ve_UT_cannon&&!TestFillingAbility(SP->OB)){
		static int spr=P->GetSpriteID();
		vui_CannInfo* I=&SP->Inf.Cannon;
		P->Visible=true;
		if(I->ChargeType!=0) AttGrMode=false;		
		if(I->NShots==0) P->SetSpriteID(spr);
		else P->SetSpriteID(spr+1);
		// weapon
		//gpWeap->FileID=0;
		if(SD->DSS.GetAmount()){
			GPPicture* gpWeap=dynamic_cast<GPPicture*>(SD->DSS[0]);
			NewMonster* NM=SP->OB->newMons;
			if(gpWeap){
				gpWeap->FileID=NM->BigWeapFile;
				gpWeap->SetSpriteID(NM->BigFireWeapSprite);
			}

		}
		return;	
	}	
	AttGrMode=false;	
};
bool va_CannonFire::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->Type==ve_UT_cannon&&SP->NI==NI){
		vui_CannInfo* C=&SP->Inf.Cannon;
		if(C->ChargeType==0){
			AttGrMode=true;
			LastKey=0;
		}else{
			vui_IS_AttackGround com;
			for(int i=0;i<OIS.SelPoint.GetAmount();i++){
				SP=OIS.SelPoint+i;
				if(SP&&SP->Type==ve_UT_cannon){
					vui_CannInfo* I=&SP->Inf.Cannon;
					com.Data.ObjID=I->OB->Index;
					com.Data.ObjSN=I->OB->Serial;
					com.InterfAction();
					ACT(101);
				}
			}
		}
		GoAndAttackMode=false;
	}
	return true;
};
bool va_CannonFire::RightClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->Type==ve_UT_cannon&&SP->NI==NI){
		vui_CannInfo* C=&SP->Inf.Cannon;
		//if(C->ChargeType!=0){
		vui_IS_CancelAttack com;
		for(int i=0;i<OIS.SelPoint.GetAmount();i++){
			SP=OIS.SelPoint+i;
			if(SP&&SP->Type==ve_UT_cannon){
				vui_CannInfo* I=&SP->Inf.Cannon;
				com.Data.ObjID=I->OB->Index;
				com.Data.ObjSN=I->OB->Serial;					
				com.InterfAction();
			}
		}
		//}
		GoAndAttackMode=false;
		AttGrMode=false;
	}
	return true;
};
// va_SP_Bld_BigPortret
void va_SP_Bld_BigPortret::SetFrameState(SimpleDialog* SD){
	if(SD->ParentDS){
		ParentFrame* P=SD->ParentDS;
		if(P) P=P->ParentDS;
		if(P) P=P->ParentDS;
		if(P){
			DialogsDesk* D=(DialogsDesk*)P;
			int NIndex=OIS.GetNIndex(D->ID);
			if(NIndex<NATIONS->NMon){
				NewMonster* NM=NATIONS->Mon[NIndex]->newMons;
				if(NM&&NM->BigIconFile!=0xFFFF){
					GPPicture* BT=(GPPicture*)SD;
					BT->FileID=NM->BigIconFile;
					BT->SetSpriteID(NM->BigIconIndex);
				}
			}
		}
	}
};
// va_UnitBigPortret
void va_UnitBigPortret::SetFrameState(SimpleDialog* SD){
	if(SD->ParentDS){
		DialogsDesk* DD=(DialogsDesk*)SD->ParentDS->ParentDS;
		if(DD){
			int NIndex=OIS.GetNIndex(DD->ID);
			if(NIndex<NATIONS->NMon){
				NewMonster* NM=NATIONS->Mon[NIndex]->newMons;
				if(NM&&NM->BigIconFile!=0xFFFF){
					GPPicture* BT=(GPPicture*)SD;
					BT->FileID=NM->BigIconFile;
					BT->SetSpriteID(NM->BigIconIndex);
				}
			}
		}
	}
};

void PrintHintParam(_str& Hint, const char* FormStr, int Base, int Current){
	if(Current<Base){
		Hint.print("%s: {CR}%d{C}\n",FormStr,Current);
	}else
		if(Current>Base){
			Hint.print("%s: {CG}%d{C}\n",FormStr,Current);
		}else{
			Hint.print("%s: %d\n",FormStr,Current);
		}
}
// va_UnitPortret_Small
va_UnitPortret_Small::va_UnitPortret_Small(){
	Lx=80;
	Ly=120;
};
void va_UnitPortret_Small::OnDraw(SimpleDialog* SD){
	if(SD->ParentDS){
		SimpleDialog* PSD=(SimpleDialog*)SD->ParentDS;
		vui_SelPoint* SP=OIS.GetSelPoint(PSD);
		if(SP){
			int NIndex=SP->OB->NIndex;
			if( NIndex<NATIONS->NMon ){
				NewMonster* NM=NATIONS->Mon[NIndex]->newMons;
				bool summon=false;
				if(SP->OB->ActiveAbility)
					SP->OB->ActiveAbility->ActiveAbilities.modifySummon(summon);
				if( NM && (NM->NewParams.PortretHeroFile==-1 || summon)/*!( Inventory || MultiSel )*/ ){
					if(NM->ExIcon.Icons.GetAmount()){
						NM->ExIcon.Draw(SD->x,SD->y,Lx,Ly,MyNation);	//40,40 - must change
					}
				}
			}
		}
	}
}
void va_UnitPortret_Small::SetFrameState(SimpleDialog* SD){
	if(SD->ParentDS){
		SimpleDialog* PSD=(SimpleDialog*)SD->ParentDS;
		vui_SelPoint* SP=OIS.GetSelPoint(PSD);
		if(SP){
			//int NIndex=OIS.GetNIndex(PSD->ID);
			int NIndex=SP->OB->NIndex;
			if(NIndex<NATIONS->NMon){
				NewMonster* NM=NATIONS->Mon[NIndex]->newMons;
				if(NM){ //&&NM->BigIconFile!=0xFFFF
					SD->Visible=true;
					bool summon=false;
					if(SP->OB->ActiveAbility)
						SP->OB->ActiveAbility->ActiveAbilities.modifySummon(summon);
					if( NM->NewParams.PortretHeroFile!=-1 && !summon ){
						GPPicture* BT=(GPPicture*)SD;
						BT->FileID=NM->NewParams.PortretHeroFile;
						if( MultiSel ){
							BT->SetSpriteID(2);
						}else{
							BT->SetSpriteID(3);
						}
					}else if(NM->ExIcon.Icons.GetAmount()){
						GPPicture* BT=(GPPicture*)SD;
						BT->FileID=0xFFFF;
						BT->SetSpriteID(-1);
					}else{
						GPPicture* BT=(GPPicture*)SD;
						BT->FileID=NM->IconFileID;
						BT->SetSpriteID(NM->IconID);
					}
				}
			}
		}
	}
};

void SetUnitHint(SimpleDialog* SD, vui_SelPoint* SP){

	OneObject* OB=SP->OB;
	//NewMonster* NM=OB->Ref.General->newMons;
	NewMonster* NM=OB->newMons;

	_str Hint="";
	if(NM->GetLongMessage()){
		Hint.print("%s\n",NM->GetLongMessage());
	}else{
		NewMonster* NM=OB->newMons;
		if( SP->OB->ActiveAbility )
			NM=SP->OB->ActiveAbility->BornNM;
		Hint.print("%s\n",NewMonster_GetMessage(NM));
	}				

	const char* cLives=GetTextByID("#Lives");
	const char* cLifeRegen=GetTextByID("#LifeRegen");
	const char* cManaRegen=GetTextByID("#ManaRegen");
	const char* cSpellPower=GetTextByID("#SpellPower");
	const char* cAttackRate=GetTextByID("#AttackRate");
	const char* cAttackFirstRate=GetTextByID("#AttackFirstRate");
	const char* cAttackSecondRate=GetTextByID("#AttackSecondRate");
	const char* cAttackRadius=GetTextByID("#AttackRadius");
	const char* cMotionSpeed=GetTextByID("#MotionSpeed");
	const char* cKillCount=GetTextByID("#KillCount");

	HeroVariableStorage* HV=DetectHero(OB);
	if( HV && HV->Hero ){
		Hint.print("%s: %d\n",cLives,HV->Lives);

		int LifeRegen=HV->Hero->LifeRegeneration+HV->UpLifeRegeneration;
		int LR=LifeRegen;
		int ManaRegen=HV->Hero->ManaRegeneration+HV->UpManaRegeneration+OB->ActiveAbility->CardManaRegeneration/100;
		int MR=ManaRegen;
		if(OB->ActiveAbility){
			OB->ActiveAbility->ActiveAbilities.modifyLifeRegeneration(LifeRegen,LR);
			OB->ActiveAbility->ActiveAbilities.modifyManaRegeneration(ManaRegen,MR);
		}
		PrintHintParam(Hint,cLifeRegen,LifeRegen,LR);
		PrintHintParam(Hint,cManaRegen,ManaRegen,MR);
		int LevelPower=HV->Level;
		int SpellPower=HV->Level;
		if(OB->ActiveAbility)
			OB->ActiveAbility->ActiveAbilities.modifyLevelForEffects(SpellPower,SpellPower);
		if (LevelPower) PrintHintParam(Hint,cSpellPower,LevelPower,SpellPower);
	}

	int GetAttSpeedBase(OneObject* OB,int AttType);
	int GetAttSpeed(OneObject* OB,int AttType);
	int AttackRate=GetAttSpeedBase(OB,0);
	int AttackFirstRate=GetAttSpeedBase(OB,0);
	int AttackSecondRate=GetAttSpeedBase(OB,1);
	int FR=GetAttSpeed(OB,0);
	int FR2=GetAttSpeed(OB,1);

	int AttackRadius=OB->MoreCharacter->MaxR_Attack;
	int AR=OB->GetMaxAttackRadius();

	int BaseR=OB->GroupSpeed;
	int MotionSpeed=0;
	if(OB->MotionStyle==6){ // COMPLEXOBJECT
		MotionSpeed=OB->GroupSpeed;
	}else
		if(OB->newMons->MotionStyle==7){ // SINGLESTEP
			MotionSpeed=OB->GroupSpeed;
			MotionSpeed=(MotionSpeed*NM->SpeedScale)>>8;
			BaseR=MotionSpeed;
			MotionSpeed=(MotionSpeed*OB->MoreCharacter->Speed)/100;
		}else
			if(OB->newMons->MotionStyle==8){ // FLY
				MotionSpeed=((NM->MotionDist*NM->SpeedScale)>>8)*OB->MoreCharacter->Speed/100;
				BaseR=MotionSpeed;
			}
			int MS=MotionSpeed;

			int Vision;
			int V=Vision;

			if(OB->ActiveAbility){
				bool Stun=false;
				MS<<=5;
				OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(BaseR<<5,MS,Stun);
				MS>>=5;
			}
			if (AttackSecondRate<=0){
				PrintHintParam(Hint,cAttackRate,AttackRate,FR);
			}else{
				PrintHintParam(Hint,cAttackFirstRate,AttackFirstRate,FR);
				PrintHintParam(Hint,cAttackSecondRate,AttackSecondRate,FR2);
			}
			PrintHintParam(Hint,cAttackRadius,AttackRadius,AR);
			if(MotionSpeed) PrintHintParam(Hint,cMotionSpeed,MotionSpeed,MS);

			if(SP->Type==ve_UT_units){
				vui_UnitInfo* I=&SP->Inf.Units;
				Hint.print("%s: %d\n",cKillCount,I->NKills);
			}

			SD->SetHint(Hint.pchar());
}

// va_SP_PortretBox
void va_SP_PortretBox::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	SD->Visible=SPClass.Check(SP); 
}
void va_SP_PortretBox::OnDraw(SimpleDialog* SD){
	if(SD->MouseOver){
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		if(SP){
			ShowBrigMiniBox(SP->rX>>4,SP->rY>>4);
			SetUnitHint(SD,SP);
		}
	}
}
bool va_SP_PortretBox::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(OIS.SelPoint.GetAmount()==1) CenterSP(SP);
	else SelSP(SP);
	return true;
}
bool va_SP_PortretBox::RightClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	CenterSP(SP);
	return true;
}
// va_SP_PortretBox2
void va_SP_PortretBox2::SetFrameState(SimpleDialog* SD){
	ParentFrame* PF=SD->ParentDS;	
	if(PF) PF=PF->ParentDS; else return;
	if(PF){
		SD->ID=((SimpleDialog*)PF)->ID;
	}
}
void va_SP_PortretBox2::OnDraw(SimpleDialog* SD){
	if(SD->MouseOver){
		vui_SelPoint* SP=OIS.GetSelPoint(SD);
		if(SP){
			ShowBrigMiniBox(SP->rX>>4,SP->rY>>4);
			SetUnitHint(SD,SP);
		}		
	}
}
bool va_SP_PortretBox2::LeftClick(SimpleDialog* SD){
	ParentFrame* PF=SD->ParentDS;	
	if(PF) PF=PF->ParentDS; else return true;
	if(PF){		
		vui_SelPoint* SP=OIS.GetSelPoint(PF);
		SelSP(SP);
	}
	return true;
}
bool va_SP_PortretBox2::RightClick(SimpleDialog* SD){
	ParentFrame* PF=SD->ParentDS;	
	if(PF) PF=PF->ParentDS; else return true;
	if(PF){		
		vui_SelPoint* SP=OIS.GetSelPoint(PF);
		CenterSP(SP);
	}
	return true;
}
int GlobalSelPointID;
// va_SP_PortretBox3
void va_SP_PortretBox3::SetFrameState(SimpleDialog* SD){
	GlobalSelPointID=SD->ID;
}
void va_SP_PortretBox3::OnDraw(SimpleDialog* SD){
	if(SD->MouseOver){
		vui_SelPoint* SP=OIS.GetSelPoint(SD);
		if(SP){
			ShowBrigMiniBox(SP->rX>>4,SP->rY>>4);
			SetUnitHint(SD,SP);
		}
	}
}
bool va_SP_PortretBox3::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetSelPoint(SD);
	SelSP(SP);
	return true;
}
bool va_SP_PortretBox3::RightClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetSelPoint(SD);
	CenterSP(SP);
	return true;
}
// va_SP_BldOnly_Side
void va_SP_BldOnly_Side::SetFrameState(SimpleDialog* SD){	
	va_SP_SideBox::SetFrameState(SD);	
	ParentFrame* PF=SD->ParentDS;
	//if(PF) PF=PF->ParentDS; else return;
	if(PF) PF=PF->ParentDS; else return;
	if(PF){
		vui_SelPoint* SP=OIS.GetSelPoint(PF);
		SD->Visible=SP&&SP->Type==ve_UT_building;
	}
};
// va_SP_SideBox
void va_SP_SideBox::OnDraw(SimpleDialog* SD){	
	if(SD->MouseOver&&OIS.SelPoint.GetAmount()){
		ParentFrame* PF=SD->ParentDS;
		if(PF) PF=PF->ParentDS; else return;
		int id=((SimpleDialog*)PF)->ID;
		vui_SelPoint* SP=OIS.SelPoint+id;
		if(SP){
			ShowBrigMiniBox(SP->rX>>4,SP->rY>>4);
		}		
	}
}
void va_SP_SideBox::SetFrameState(SimpleDialog* SD){
	if(SD->ID==1) return;
	/*
	if(LastKey==VK_TAB&&
	((GetKeyState(VK_CONTROL)&0x8000)&&OIS.LastSP==0|| //VK_SHIFT
	(GetKeyState(VK_SHIFT)&0x8000))){ //VK_CONTROL
	LastKey=0;
	}
	*/
	ParentFrame* PF=SD->ParentDS;
	//if(PF) PF=PF->ParentDS; else return;
	if(PF) PF=PF->ParentDS; else return;
	//vui_SelPoint* SP=OIS.GetSelPoint(PF);
	int id=((SimpleDialog*)PF)->ID;
	//if(SP){
	SD->HotKey=0;
	if(PF&&!(GetKeyState(VK_SHIFT)&0x8000)){		
		if(GetKeyState(VK_CONTROL)&0x8000){
			if(OIS.LastSP==0){
				if(id==OIS.SelPoint.GetAmount()-1){
					SD->HotKey=VK_TAB;
				}				
			}else{
				if(id==OIS.LastSP-1){
					SD->HotKey=VK_TAB;
				}
			}			
		}else
			if(!(GetKeyState(VK_CONTROL)&0x8000)){
				if(OIS.LastSP==OIS.SelPoint.GetAmount()-1){
					if(id==0){
						SD->HotKey=VK_TAB;
					}				
				}else{
					if(id==OIS.LastSP+1){
						SD->HotKey=VK_TAB;
					}
				}
			}
	}	
};
bool va_SP_SideBox::LeftClick(SimpleDialog* SD){
	if(SD->ID==1){
		SD=(SimpleDialog*)SD->ParentDS;
	}
	if(!SD) return true;
	DialogsDesk* D=(DialogsDesk*)SD->ParentDS;
	if(D){
		D=(DialogsDesk*)D->ParentDS;
		vui_SelPoint* SP=OIS.GetSelPoint(D);
		if(SP){
			if(GetKeyState(VK_SHIFT)&0x8000){
				SelSP(SP);
			}else{
				OIS.SetLastSP(D->ID);
			}			
		}
	}
	return true;
}
bool va_SP_SideBox::RightClick(SimpleDialog* SD){
	if(SD->ID==1){
		SD=(SimpleDialog*)SD->ParentDS;
	}
	if(!SD) return true;
	DialogsDesk* D=(DialogsDesk*)SD->ParentDS;
	if(D){
		D=(DialogsDesk*)D->ParentDS;
		vui_SelPoint* SP=OIS.GetSelPoint(D);
		CenterSP(SP);
	}
	return true;
}
// va_Unit_P_Box
char* GetKeyName(word Key);
int CurProdID=-1;
void va_Unit_P_Box::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	GPPicture* P=(GPPicture*)SD;


	P->ID=cva_OIS_Produce::curID++;
	CurProdID=P->ID;
	//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
	vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);

	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&I&&I->NIndex<NATIONS->NMon){
		vui_BuildInfo* BI=&SP->Inf.Buildings;
		if(SP->Type==ve_UT_building&&BI->Ready){
			if(NATIONS->Mon[I->NIndex]->newMons->Peasant && SP->NI==GSets.CGame.cgi_NatRefTBL[MyNation]){
				extern bool vPeasant_Idle;
				extern bool vPeasant_AutoWork;
				vPeasant_Idle=true;
				vPeasant_AutoWork=true;
			}
		}
		CurProdID=P->ID;
		SD->Visible=true;
		P->HotKey=I->HotKey;

		// Hint
		GeneralObject* GO=NATIONS[SP->NI].Mon[I->NIndex];
		NewMonster* NM=GO->newMons;
		SD->CheckMouseOver(mouseX,mouseY);
		if( SD->MouseOver && !I->Disabled ){
			_str str;
			if(EngSettings.HintTemplate.Produce.isClear()){

				char txt[512];

				// Title
				char* msgTitle=GO->Message;
				if(NM->NewParams.MessageProduce){
					msgTitle=NM->NewParams.MessageProduce;
				}
				str.print("{Fgr14}{C FFffcb2b}%s\n",msgTitle);

				// Hot key
				if(P->HotKey!=0){
					char* key=GetTextByID(GetKeyName(P->HotKey));
					str.print(" ({C FFb5b09c}%s{C FFffcb2b})\n",key);
				}

				// Message
				if(GO->newMons->GetLongMessage()){
					str.print("{dy4}{Ftm12}{C FFb5b09c}%s\n",GO->newMons->GetLongMessage());
				}

				// ARTDEPO или UNITLOCK
				bool limit=false;
				Nation* NT=SP->OB->Nat;
				City* CT=CITY+SP->OB->NNUM;
				static char* cLimit=GetTextByID("#Limit");
				if(NM->ArtSet){
					int n=NT->NArtUnits[NM->ArtSet-1];
					int m=NT->NArtdep[NM->ArtSet-1]*NM->NInArtDepot;
					str.print("{dy4}{Fsm01}{C AAbcbcbc}%s: ",cLimit);
					if(n>=m){
						str.print("{C FFff0000}%d/%d\n",n,m);
						limit=true;
					}else{
						str.print("{C FFb39a58}%d/%d\n",n,m);
					}
				}				
				if(GO->LockID!=0xFFFF){
					int n=CT->UnitAmount[GO->LockID];
					int m=GO->NLockUnits;
					str.print("{dy4}{Fsm01}{C 9Fbcbcbc}%s: {Ftm12}",cLimit);
					if(n>=m){
						str.print("{C FFff0000}%d/%d\n",n,m);
						limit=true;
					}else{
						str.print("{C FFb39a58}%d/%d\n",n,m);
					}
				};

				// access control
				if(!I->Enabled){
					char* acMess[32];
					int acN=GetMonsterAccessControl(SP->NI, I->NIndex, acMess);
					if(acN){
						static char* cAccessControl=GetTextByID("#AccessControl");
						str.print("{dy4}{Fsm01}{C AAbcbcbc}%s:\n{dy2}{Ftm12}{C FFff0000}",cAccessControl);
						for(int i=0;i<acN;i++){
							str.print("%s\n",acMess[i]);
						}
					}
				}

				// cost	
				if(!limit){
					txt[0]=0;
					GetMonCost(SP->NI,I->NIndex,txt);
					str+=txt;
				}

				if(SP->Type==ve_UT_building){
					if(!I->Building){
						static char* txt=GetTextByID("#BuildUnlimit");
						str.print("{dy2}{Fsm01}{C 9FBCBCBC}%s",txt);
					}
				}

			}else{
				str=EngSettings.HintTemplate.Produce;
				char txt[512];
				_str tx2;
				// Message
				txt[0]=0;
				GetChar(GO,txt);
				str.Replace(EngSettings.HintTemplate.MessageID.pchar(),txt);
				// Hot key
				txt[0]=P->HotKey; txt[1]=0;
				str.Replace(EngSettings.HintTemplate.HotKeyID.pchar(),txt);
				// ARTDEPO или UNITLOCK				
				Nation* NT=SP->OB->Nat;
				if(NM->ArtSet){
					int n=NT->NArtUnits[NM->ArtSet-1];
					int m=NT->NArtdep[NM->ArtSet-1]*NM->NInArtDepot;
					if(n>=m){
						str.print("{CR}Limit: %d/%d\\{C}",n,m);
					}else{
						str.print("Limit: %d/%d\\",n,m);
					}						
				}
				// Access control
				//txt[0]=0;
				tx2="";
				if(!I->Enabled){					
					char* acMess[32];
					int acN=GetMonsterAccessControl(SP->NI, I->NIndex, acMess);
					for(int i=0;i<acN;i++){
						tx2.print(EngSettings.HintTemplate.AccessControl.pchar(),acMess[i]);
					}
				}
				str.Replace(EngSettings.HintTemplate.AccessControlID.pchar(),tx2.pchar());
				// Price
				txt[0]=0;
				GetMonCost(SP->NI,I->NIndex,txt);
				str.Replace(EngSettings.HintTemplate.PriceID.pchar(),txt);
			}

			P->SetHint(&str); //SD
			SD->HintWithHotKey=true;
		}else{
			P->SetHint("");
		}

	}
}
bool va_Unit_P_Box::LeftClick(SimpleDialog* SD){
	//if(GetKeyState(VK_CONTROL)&0x8000){
	//	RightClick(SD);
	//	return true;
	//}
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	GPPicture* P=(GPPicture*)SD;//->ParentDS;
	if(SP&&P){
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		if(I&&I->Enabled&&I->NIndex!=0xFFFF&&CheckCostHint(NI,I->NIndex,true)){
			if(SP->Type==ve_UT_building){
				if(I->Building){
					vui_IS_MakeMaxStage Com;
					Com.Data.Index=I->NUnlimit;
					Com.InterfAction();
				}else{
					byte NI=MyNation;
					//bool DetectMagazine(OneObject* OB);
					//if(DetectMagazine(SP->OB)){
					//	NI=SP->OB->NNUM;
					//}
					CmdProduceObj(NI,I->NIndex);
				}
			}else{
				int CheckIfObjectIsWall(int NIndex);
				int wt=CheckIfObjectIsWall(I->NIndex);
				if(wt){
					void SetWallBuildMode(int Mode);
					SetWallBuildMode(wt);
				}else{
					BuildMode=true;
					BuildingID=I->NIndex;
					BNat=NATIONS+NI;				
				}
			}
		}
	}
	return true;
};
bool va_Unit_P_Box::RightClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	GPPicture* P=(GPPicture*)SD;//->ParentDS;
	if(SP&&P){
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		if(I&&I->Enabled&&SP->Type==ve_UT_building){
			if(I->Building){
				OneObject* OB=Group[I->NUnlimit];
				if(OB){
					GAMEOBJ Z;
					Z.CreateZone(OB->RealX>>4,OB->RealY>>4,100);
					SetStartPoint(&Z);
				}
			}else{
				CmdUnProduceObj(MyNation,I->NIndex);
			}			
		}
	}
	return true;
};
// va_Unit_P_BuildMode
void va_Unit_P_BuildMode::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	SimpleDialog* P=(SimpleDialog*)SD;
	if(CurProdID!=-1){
		P->ID=CurProdID;
	}else{
		P=(SimpleDialog*)SD->ParentDS;
	}
	if(P){
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		if(SP&&I&&I->NIndex<NATIONS->NMon&&
			(I->NProduce>0||I->NUnlimit==1||SP->Type==ve_UT_building&&I->Building)){
				SD->Visible=true;
			}
	}
}
// va_Unit_P_BuildMode2
void va_Unit_P_BuildMode2::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	SimpleDialog* P=(SimpleDialog*)SD;
	if(CurProdID!=-1){
		P->ID=CurProdID;
	}else{
		P=(SimpleDialog*)SD->ParentDS;
	}
	if(P){
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		if(SP&&I&&I->NIndex<NATIONS->NMon&&I->NUnlimit>1&&
			(SP->Type!=ve_UT_building||!I->Building)){
				SD->Visible=true;
			}
	}
}
// va_Unit_P_Amount
void va_Unit_P_Amount::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	SimpleDialog* P=(SimpleDialog*)SD;
	if(CurProdID!=-1){
		P->ID=CurProdID;
	}else{
		P=(SimpleDialog*)SD->ParentDS;
		if(P) P=(SimpleDialog*)P->ParentDS; else return;
	}
	if(P){		
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		_str txt;
		if(I&&I->NIndex<NATIONS->NMon){
			if(SP->Type==ve_UT_building&&I->Building||
				I->NProduce>0&&I->NUnlimit==0){
					txt.Add(I->NProduce);
				}
		}
		if(txt.pchar()){
			TextButton* T=(TextButton*)SD;
			T->SetMessage(txt.pchar());
			T->Visible=true;
		}
	}
}
// va_Unit_P_Unlimit
void va_Unit_P_Unlimit::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	SimpleDialog* P=(SimpleDialog*)SD;
	if(CurProdID!=-1){
		P->ID=CurProdID;
	}else{
		P=(SimpleDialog*)SD->ParentDS;
		if(P) P=(SimpleDialog*)P->ParentDS; else return;
	}
	if(P){
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		if(I&&I->NIndex<NATIONS->NMon&&I->NUnlimit>0&&
			(SP->Type!=ve_UT_building||!I->Building)){
				SD->Visible=true;
			}
	}
}
// va_Unit_P_Unlimit2
void va_Unit_P_Unlimit2::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	SimpleDialog* P=(SimpleDialog*)SD;
	if(CurProdID!=-1){
		P->ID=CurProdID;
	}else{
		P=(SimpleDialog*)SD->ParentDS;
		if(P) P=(SimpleDialog*)P->ParentDS; else return;
	}
	if(P){
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		_str txt;
		if(I&&I->NIndex<NATIONS->NMon&&I->NUnlimit>1){
			txt.Add(I->NUnlimit);			
		}
		if(txt.pchar()){
			TextButton* T=dynamic_cast<TextButton*>(SD);
			if(T){
				T->SetMessage(txt.pchar());
				T->Visible=true;
			}
			VitButton* VB=dynamic_cast<VitButton*>(SD);
			if(VB){
				VB->SetMessage(txt.pchar());
				VB->Visible=true;
			}
		}
	}
}
// va_UnitProdPort
va_UnitProdPort::va_UnitProdPort(){
	Lx=46;
	Ly=69;
}
void va_UnitProdPort::OnDraw(SimpleDialog* SD){
	GPPicture* P=(GPPicture*)SD->ParentDS;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&P){
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		if(I&&I->NIndex<NATIONS->NMon){
			NewMonster* NM=NATIONS->Mon[I->NIndex]->newMons;
			GPPicture* p=(GPPicture*)SD;

			// Color
			if(I->Enabled){
				if(!CheckCostHint(SP->NI,I->NIndex)){
					SD->Diffuse=0xFFDD2020;
				}else{
					SD->Diffuse=0xFFFFFFFF;
				}				
			}else{
				SD->Diffuse=0xFFDD2020;
			}
			if(I->Disabled){
				SD->Diffuse=0xFF354045;
			}

			// можно сделать и перезарядку
			if(NM->ExIcon.Icons.GetAmount()){
				DWORD CurDiffuse=GPS.GetCurrentDiffuse();
				GPS.SetCurrentDiffuse(SD->Diffuse);
				NM->ExIcon.Draw(p->x,p->y,Lx,Ly,MyNation);    
				GPS.SetCurrentDiffuse(CurDiffuse);
			}
		}
	}
}
void va_UnitProdPort::SetFrameState(SimpleDialog* SD){
	GPPicture* P=(GPPicture*)SD->ParentDS;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&P){
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		if(I&&I->NIndex<NATIONS->NMon){
			NewMonster* NM=NATIONS->Mon[I->NIndex]->newMons;
			GPPicture* p=(GPPicture*)SD;
			if(NM->ExIcon.Icons.GetAmount()){
				p->FileID=0xFFFF;
				p->SetSpriteID(-1);
			}else{
				//p->Visible=true;
				if(NM->MinIconFile!=0xFFFF){
					p->FileID=NM->MinIconFile;
					p->SetSpriteID(NM->MinIconIndex);
				}else{
					p->FileID=NM->IconFileID;
					p->SetSpriteID(NM->IconID);
				}
			}
			//			
			if(SP->Upg.GetAmount()) I->Enabled=false;

			//SD->Visible=!I->Disabled;
			SD->Visible=true;
		}
	}
};
// va_UnitProdStage
void va_UnitProdStage::SetFrameState(SimpleDialog* SD){	
	GPPicture* P=(GPPicture*)SD->ParentDS;	
	if(P){
		//vui_ProduceInfo* I=OIS.GetProduceInfo(P->ID);
		vui_ProduceInfo* I=OIS.GetProduceInfoByAIndex(P->ID);
		if(I&&(I->NProduce||I->NUnlimit)){
			//Canvas* C=dynamic_cast<Canvas*>(SD);
			//if(C){
			//	C->L=0;
			//	int M=I->MaxStage;
			//	if(M>1)M--;
			//	int Ly=C->GetHeight()*I->Stage/M;
			//	C->AddBar(0,C->GetHeight()-Ly,C->GetWidth(),Ly,0xFF00FF00);
			//	C->Visible=true;
			//	return;
			//}
			ProgressBar* PB=dynamic_cast<ProgressBar*>(SD);
			if(PB){
				PB->Visible=true;
				PB->Type=1;//0;
				PB->GaugeType=0;//1;
				PB->ProgressType=0;//1;
				PB->Diffuse=0xB22222FF;//0xB2000000;
				PB->MaxValue=I->MaxStage;
				PB->Value=I->Stage;
				return;
			}
		}
	}
	SD->Visible=false;
};
// va_WeapPort
void va_WeapPort::SetFrameState(SimpleDialog* SD){
	GPPicture* P=(GPPicture*)SD->ParentDS;
	if(P){
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		GPPicture* p=(GPPicture*)SD;
		if(SP){
			if(SP->Type==ve_UT_brigade){
				vui_BrigInfo* I=&SP->Inf.Brigade;
				p->FileID=I->WeapFileID;
				switch(P->ID){
				case 0:					
					p->SetSpriteID(I->WeapColdSprite);
					break;
				case 1:
					//p->Diffuse=I->RifleAttack?0xFFFFFFFF:0xFFFF0000;
					p->SetSpriteID(I->WeapFireSprite);
					break;
				case 2:
					if(I->isGrenaders){
						p->SetSpriteID(2);
					}				
					break;
				}			
			}else
				if(SP->Type==ve_UT_units){
					vui_UnitInfo* I=&SP->Inf.Units;
					p->FileID=I->WeapFileID;
					switch(P->ID){
					case 0:					
						p->SetSpriteID(I->WeapColdSprite);
						break;
					case 1:
						//p->Diffuse=I->RifleAttack?0xFFFFFFFF:0xFFFF0000;
						p->SetSpriteID(I->WeapFireSprite);
						break;
					}
				}
		}
	}
}
// cva_WeapMiniDamPort
void cva_WeapMiniDamPort::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	GPPicture* P=dynamic_cast<GPPicture*>(SD);
	if(SP&&OIS.SelPoint.GetAmount()==1){		
		switch(SP->Type){
		case ve_UT_brigade:	{
			vui_BrigInfo* I=&SP->Inf.Brigade;
			int dam=I->Damage[SD->ID];
			if(dam>0){
				if(P)P->SetSpriteID(WeaponIcn[I->WeapType[SD->ID]]);
				SD->Visible=true;
			}
			break; }
		case ve_UT_units: {
			vui_UnitInfo* I=&SP->Inf.Units;
			int dam=I->Damage[SD->ID];
			if(dam>0){
				SD->Visible=true;
				if(P)P->SetSpriteID(WeaponIcn[I->WeapType[SD->ID]]);
			}
			break; }			
		case ve_UT_building: {
			vui_UnitInfo* I=&SP->Inf.Buildings.UI;
			if(I->Damage[SD->ID]>0){
				SD->Visible=true;
				if(P)P->SetSpriteID(WeaponIcn[I->WeapType[SD->ID]]);
			}
			break; }			
		};
	}	
}
// cva_WeapMiniDam
void cva_WeapMiniDam::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	TextButton* T=dynamic_cast<TextButton*>(SD);
	if(SP&&T&&OIS.SelPoint.GetAmount()==1){
		_str dam; dam=0;		
		switch(SP->Type){
		case ve_UT_brigade:	
			if(EngSettings.MoraleType==0){
				vui_BrigInfo* I=&SP->Inf.Brigade;
				if(I->Damage[SD->ID]>0) dam=I->Damage[SD->ID];			
			}else{			
				vui_BrigInfo* I=&SP->Inf.Brigade;
				Brigade* BR=CITY[SP->NI].Brigs+I->BrigID;
				if(BR->WarType){
					int dam=I->Damage[SD->ID];
					int adam=I->DamageAdd[SD->ID];
					if(dam){
						char cc[128];
						if(adam){
							//sprintf(cc,"%d+{CG}%d{C}",dam,adam);
							sprintf(cc,"{CG}%d{C}",dam+adam);
						}else{
							sprintf(cc,"%d",dam);
						}						
						T->SetMessage(cc);
						T->Visible=true;
					}
					return;
				}
			}
			break; 
		case ve_UT_units: { 
			vui_UnitInfo* I=&SP->Inf.Units;
			int d=I->Damage[SD->ID];
			if(d>0){
				dam=d;
			}
			break; }			
		case ve_UT_building: { 
			vui_UnitInfo* I=&SP->Inf.Buildings.UI;
			if(I->Damage[SD->ID]>0) dam=I->Damage[SD->ID];
			break; }			
		};
		if(dam!="0"){
			T->SetMessage(dam.pchar());
			SD->Visible=true;
		}
	}	
}
// cva_WeapMiniDefPort
//
bool vAllwaysWeapPort=true;
//
void cva_WeapMiniDefPort::SetFrameState(SimpleDialog* SD){		
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	GPPicture* P=dynamic_cast<GPPicture*>(SD);
	SD->Visible=false;
	if(SP&&P&&OIS.SelPoint.GetAmount()==1){
		bool Enb=false;
		int WPK=-1;
		if(SP->Type==ve_UT_brigade){
			vui_BrigInfo* I=&SP->Inf.Brigade;
			int adam=I->DefenceAdd[SD->ID];
			int dam=I->Defence[SD->ID]+I->Shield+adam;
			if(dam==0&&adam<=0){
				Enb=false;
			}else{
				Enb=true;
			}
		}else
			if(SP->Type==ve_UT_units){
				vui_UnitInfo* I=&SP->Inf.Units;
				Enb=I->Defence[SD->ID]>0;
				WPK=I->WeapType[SD->ID];
			}else
				if(SP->Type==ve_UT_building){
					vui_UnitInfo* I=&SP->Inf.Buildings.UI;
					Enb=I->Defence[SD->ID]>0;
				}
				if(Enb||vAllwaysWeapPort){
					SD->Visible=true;
					//int wk=-1;
					switch(SD->ID){
						case 0: WPK=3; break;
						case 1: WPK=0; break;
						case 2: WPK=2; break;
						case 3: WPK=1; break;
					};
					if(WPK!=-1){
						P->SetSpriteID(WeaponIcn[WPK]);
						if(P->MouseOver){
							static DString txt;
							txt.Clear();
							txt.print("%s %s",GetTextByID("#mod_Protection"),GetTextByID(WeaponIDS[WPK]));
							P->SetHint( txt.pchar() );
						}
					}
				}
				if( SD->Visible && WPK!=-1 ){
					int T=SP->TimeLastActivationDefence[WPK]; //wk
					if(T){
						int dT=TrueTime-T;
						if(dT<3000){
							SD->Visible=(GetTickCount()>>9)&1;
						}
					}
				}
	}	
}
// cva_WeapMiniDef
void cva_WeapMiniDef::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	TextButton* T=dynamic_cast<TextButton*>(SD);
	if(SP&&T&&OIS.SelPoint.GetAmount()==1){
		_str def;
		switch(SP->Type){
			case ve_UT_brigade:	{
				if(EngSettings.MoraleType==0){
					vui_BrigInfo* I=&SP->Inf.Brigade;
					def=I->Defence[SD->ID];			
				}			
				vui_BrigInfo* I=&SP->Inf.Brigade;
				Brigade* BR=CITY[SP->NI].Brigs+I->BrigID;
				if(BR->WarType){
					int dam=I->Defence[SD->ID]+I->Shield;
					int adam=I->DefenceAdd[SD->ID];
					if(dam+adam){
						char cc[128];
						if(adam){
							if(adam>0){
								sprintf(cc,"{CG}%d{C}",dam+adam);
							}else{
								if(dam){
									int d=dam+adam;
									sprintf(cc,"{CR}%d{C}",d>0?d:0);
								}
							}						
						}else{
							sprintf(cc,"%d",dam);
						}
						T->SetMessage(cc);
						T->Visible=true;
					}
					return;
				}                				
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&SP->Inf.Units;
				int b=I->Defence[SD->ID];
				int d=b;
				if(SP->OB->ActiveAbility){
					SP->OB->ActiveAbility->ActiveAbilities.modifyShield(b,d,NULL);
					extern byte ProtWeap[4];
					SP->OB->ActiveAbility->ActiveAbilities.modifyProtection(ProtWeap[SD->ID],b,d,NULL);
				}
				if(d>0){
					def=d;
				}
				break; }
			case ve_UT_building: {
				vui_UnitInfo* I=&SP->Inf.Buildings.UI;
				int b=I->Defence[SD->ID];
				int d=b;
				if(SP->OB->ActiveAbility){
					SP->OB->ActiveAbility->ActiveAbilities.modifyShield(b,d,NULL);
					extern byte ProtWeap[4];
					SP->OB->ActiveAbility->ActiveAbilities.modifyProtection(ProtWeap[SD->ID],b,d,NULL);
				}
				if(d>0){
					def=d;
				}
				break; }
		};	
		if(!def.isClear()){
			T->SetMessage(def.pchar());
			SD->Visible=true;
		}
	}	
}

void cva_SwitchAttType::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if( SP && SP->NI==NI && SP->OB->newMons->ArmAttack ){

		int n=SP->Passive.GetAmount();
		if( SD->ID==0&&n>0 || SD->ID==1&&n==0 )
			return;

		VitButton* VB=dynamic_cast<VitButton*>(SD);
		if( !VB && SD->DSS.GetAmount() ){
			VB=dynamic_cast<VitButton*>(SD->DSS[0]);
		}
		if(VB){
			switch(SP->Type){
			case ve_UT_units:
				{
					vui_UnitInfo* I=&SP->Inf.Units;
					if(I->Damage[1]>0){
						SD->Visible=true;
						if(I->RifleAttack){
							VB->State=1;
						}else{
							VB->State=0;
						}
					}
					break;
				}
			}
		}
	}
}
bool cva_SwitchAttType::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	if(SP){
		VitButton* VB=(VitButton*)SD;
		switch(SP->Type){
			case ve_UT_units:
				{
					vui_UnitInfo* I=&SP->Inf.Units;
					if(I->Damage[1]>0){
						if(I->RifleAttack){
							SETATTSTATE_Pro(128);
							SETATTSTATE_Pro(1);
						}else{
							SETATTSTATE_Pro(129);
							SETATTSTATE_Pro(2);
						}
					}
					break;
				}
		}
	}
	return true;
}

// va_WeapPortBack
extern bool ForceShowAttRanges;
void va_WeapPortBack::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if(!vBrigPanel.Weapon[SD->ID]){
		return;
	}
	SD->UserParam=0xFFFF;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	if(SP){
		bool Ours = SP->NI==GSets.CGame.cgi_NatRefTBL[MyNation];
		GPPicture* P=(GPPicture*)SD;
		//
		int spr_0=0;
		//
		if(SD->ID==0){
			static int spr0=P->SpriteID;
			spr_0=spr0;
			P->SetSpriteID(spr0);
		}		
		switch(SP->Type){
		case ve_UT_brigade:
			{
				vui_BrigInfo* I=&SP->Inf.Brigade;
				switch(SD->ID){
		case 0: {						
			if(I->AttEnm) P->SetSpriteID(spr_0+1);
			SD->Visible=true;
			if(GetKeyState(VK_SHIFT)&0x8000){
				SD->UserParam=2;
			}else{
				SD->UserParam=1;
			}						
			static char* hint=GetTextByID("USE_MELEE_ATTACK");
			SD->SetHint(hint,true);
			if(SD->MouseOver&&Ours){
				FList.AddMeleeOpponentForBrigade(I->BR);
			}else
				if(I->AttEnm){
					/*
					// GEC only fpr you
					extern int LastMx;
					extern int LastMy;
					int x=SP->rX>>4;
					int y=SP->rY>>4;
					if(Norma(x-LastMx,y-LastMy)<500){
					FList.AddArrowWhenAttEnm(I->BR);
					}
					*/
				}
				break; }
		case 1:
			if(I->Damage[1]>0){
				if(SD->MouseOver||(GetKeyState(VK_MENU)&0x8000)){
					if(Ours)ForceShowAttRanges=true;
				}
				SD->Visible=true;
				if(I->RifleAttack){
					if(Hint_Enabled){
						SD->SetHint(Hint_Enabled,true);
					}
					SD->UserParam=128;
					P->SetSpriteID(3);
				}else{
					if(Hint_Disabled){
						SD->SetHint(Hint_Disabled,true);
					}
					SD->UserParam=129;
					P->SetSpriteID(2);
				}
			}
			break;
		case 2:
			if(I->isGrenaders){
				SD->Visible=true;
				SD->UserParam=I->BrigID;
				if(SD->MouseOver&&Ours){
					extern bool ForceShowAttRanges2;
					ForceShowAttRanges2=true;								
				}
			}
			break;
				}				
			}
			break;
		case ve_UT_units:
			{
				vui_UnitInfo* I=&SP->Inf.Units;
				if(SD->ID==0&&I->Damage[0]>0){
					SD->Visible=true;
					if(I->GroundState) P->SetSpriteID(spr_0+1);
					SD->UserParam=1;
					if(!SP->OB->newMons->Priest){
						static char* hint=GetTextByID("USE_MELEE_ATTACK");
						SD->SetHint(hint,true);
					}else{
						static char* hint=GetTextByID("#PriestAttackHint");
						SD->SetHint(hint,true);
					}
				}else
					if(SD->ID==1&&I->Damage[0]>0&&I->Damage[1]>0){
						if(SD->MouseOver||(GetKeyState(VK_MENU)&0x8000)){
							ForceShowAttRanges=true;
						}
						SD->Visible=true;
						if(I->RifleAttack){
							if(Hint_Enabled){
								SD->SetHint(Hint_Enabled,true);
							}
							SD->UserParam=128;
							P->SetSpriteID(3);
						}else{
							if(Hint_Disabled){
								SD->SetHint(Hint_Disabled,true);
							}
							SD->UserParam=129;
							P->SetSpriteID(2);
						}
					}
			}
			break;
		};
		if(SP->NI==GSets.CGame.cgi_NatRefTBL[MyNation]){
			SD->Diffuse=0xFFFFFFFF;
		}else{
			SD->Diffuse=0x80FFFFFF;
		}
		SD->DeepColor=true;
	}	
}
bool va_WeapPortBack::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SD->UserParam!=0xFFFF&&SP&&SP->NI==NI){
		switch(SD->ID){
			case 0:
			case 1:
				SETATTSTATE_Pro(SD->UserParam);
				if(OIS.SelPoint.GetAmount()==1){
					switch(SD->UserParam){
			case 1:
				ACT(5);//MELEE
				break;
			case 129:
				ACT(3);//RIFLE
				break;
			case 128:
				ACT(4);//NORIFLE
				break;
			case 4:
				//ACT(6);//GRENADE
				break;
					};
				}
				break;
			case 2:
				for(int i=0;i<OIS.SelPoint.GetAmount();i++){
					vui_SelPoint* SP=OIS.SelPoint+i;
					if(SP&&SP->Type==ve_UT_brigade&&SP->Inf.Brigade.isGrenaders){
						ThrowGrenade(SP->Inf.Brigade.BrigID);
					}
				}
				//ThrowGrenade(SD->UserParam);
				ACT(6);//GRENADE
				break;
		}
	}
	return true;
};
// va_MiniComDesk
void va_MiniComDesk::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	static int StartX=SD->Getx();
	if(SP&&SP->NI==GSets.CGame.cgi_NatRefTBL[MyNation]){
		if(SP->Type==ve_UT_brigade){
			SD->Visible=true;
			if(SD->Name!="3row"){
				vui_BrigInfo* BI=&SP->Inf.Brigade;
				int ShiftX=0;
				if(BI->Damage[1]>0) ShiftX+=70;
				if(BI->isGrenaders) ShiftX+=70;
				if(OIS.SelPoint.GetAmount()>1) ShiftX+=(OIS.SelPoint.GetAmount()-1)*OIS.SPSideLx;
				SD->Setx(StartX+ShiftX);
			}
		}
	}
}
//
int vDrawSelPointID=0;
//va_SP_BColorSide
void va_SP_BColorSide::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	DialogsDesk* D=(DialogsDesk*)SD->ParentDS;
	if(D&&D->ParentDS){
		D=(DialogsDesk*)D->ParentDS;
		GeneralObject* GO=OIS.GetGeneralObject(D->ID);
		vDrawSelPointID=D->ID;
		if(GO){
			GPPicture* P=(GPPicture*)SD;
			SD->Visible=true;
			if(GO->newMons->PortBackSprite!=0xFFFF){
				P->SetSpriteID(1+GO->newMons->PortBackSprite);
			}else{
				P->SetSpriteID(0);
			}
		}
	}
}
// va_SP_BranchColor
void va_SP_BranchColor::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	GeneralObject* GO=OIS.GetGeneralObject(OIS.LastSP);
	vDrawSelPointID=OIS.LastSP;
	if(GO){
		GPPicture* P=(GPPicture*)SD;
		SD->Visible=true;
		if(GO->newMons->PortBackSprite!=0xFFFF){
			P->SetSpriteID(1+GO->newMons->PortBackSprite);
		}else{
			P->SetSpriteID(0);
		}
		//P->SetHint(GO->newMons->GetLongMessage());
	}
}
// va_SP_BranchSprite
void va_SP_BranchSprite::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	GeneralObject* GO=OIS.GetGeneralObject(OIS.LastSP);
	if(GO){
		if(GO->newMons->PortBranch!=0xFFFF){
			SD->Visible=true;
			GPPicture* P=(GPPicture*)SD;
			P->SetSpriteID(GO->newMons->PortBranch);
		}
	}
}
// va_SP_AmountMini
void va_SP_AmountMini::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	ParentFrame* PF=SD->ParentDS;
	if(PF) PF=PF->ParentDS; else return;
	if(PF) PF=PF->ParentDS; else return;
	cva_SP_id_Amount::SetFrameState((SimpleDialog*)PF);
}
// va_SP_Amount
void va_SP_Amount::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	TextButton* T=(TextButton*)SD;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		static _str Txt;
		Txt="";
		switch(SP->Type){
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				Txt=I->NLiveMembers;
				Txt+="/";
				Txt+=I->NMembers;
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&SP->Inf.Units;
				Txt=I->Amount;
				break; } 
			case ve_UT_building: {
				vui_BuildInfo* I=&SP->Inf.Buildings;
				Txt=I->Amount;
				break; } 
		}
		if(Txt.pchar()[0]){
			T->SetMessage(Txt.pchar());
			T->Visible=true;
		}
	}
}
// va_SP_Kills
void va_SP_Kills::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	TextButton* T=(TextButton*)SD;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		static _str Txt;
		Txt="";
		switch(SP->Type){
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				Txt+=I->NKills;
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&SP->Inf.Units;
				Txt+=I->NKills;
				break; } 
			case ve_UT_cannon: {
				vui_CannInfo* I=&SP->Inf.Cannon;
				Txt+=I->NKills;
				break; } 
		}
		if(Txt.pchar()[0]){
			T->SetMessage(Txt.pchar());
			T->Visible=true;
		}
	}
}
// va_SP_KillsAward
void va_SP_KillsAward::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	GPPicture* P=(GPPicture*)SD;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		int NKills=0;
		switch(SP->Type){
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				NKills=I->NKills;
				break; }
			case ve_UT_cannon: {
				vui_CannInfo* I=&SP->Inf.Cannon;
				NKills=I->NKills;
				break; } 
		}
		int Rank=0;
		if(NKills){
			if(NKills>20)Rank++;
			if(NKills>60)Rank++;
			if(NKills>120)Rank++;
			if(NKills>300)Rank++;
			if(NKills>600)Rank++;
		}
		if(Rank>=P->ID) SD->Visible=true;
	}
}
// cva_SP_KillsGuardian
void cva_SP_KillsGuardian::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	GPPicture* P=dynamic_cast<GPPicture*>(SD);	
	if(P&&vDrawSelPointID<OIS.SelPoint.GetAmount()){
		vui_SelPoint* SP=OIS.SelPoint+vDrawSelPointID;
		int NKills=0;
		switch(SP->Type){
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				NKills=I->NKills;
				break; }
			case ve_UT_cannon: {
				vui_CannInfo* I=&SP->Inf.Cannon;
				NKills=I->NKills;
				break; } 
		}
		if(NKills&&NKills>=v_ISys.NKillsForGuardian){
			SD->Visible=true;
			static char* hint=GetTextByID("#Guardian_hint");
			SD->SetHint(hint);
			static int time=GetTickCount();
			int NFrames=GPS.GPNFrames(P->FileID);
			if(NFrames){
				int f=((GetTickCount()-time)/40)%NFrames;
				P->SetSpriteID(f);
			}

		}
	}
}
// va_SP_Morale
void va_SP_Morale::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	GP_TextButton* T=(GP_TextButton*)SD;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		static _str Txt;
		Txt="";
		switch(SP->Type){
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				if(I->MaxMorale<5000){
					Txt+=I->Morale;
					Txt+="/";
					Txt+=I->MaxMorale;
				}
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&SP->Inf.Units;
				if(I->MoraleMax<5000){
					Txt+=I->Morale;
					Txt+="/";
					Txt+=I->MoraleMax;
				}
				break; } 
		}
		if(Txt.pchar()[0]){
			T->SetMessage(Txt.pchar());
			T->Visible=true;
		}
	}
}
// va_SP_NatFlag
void va_SP_NatFlag::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP&&SP&&SP->NatID<GlobalAI.NAi&&SP->NI!=7){
		SD->Visible=true;
		//GP->SetSpriteID(SP->NatID);
		GP->SetSpriteID(GlobalAI.Ai[SP->NatID].NWaterAI);
		GP->SetHint(GlobalAI.Ai[SP->NatID].Message);
	}
}
// va_SP_Protect
void va_SP_Protect::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	TextButton* T=(TextButton*)SD;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		static _str Txt;
		Txt="";
		switch(SP->Type){
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				Txt=I->Shield;
				if(I->ShieldAdd){
					Txt+="+";
					Txt+=I->ShieldAdd;
				}
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&SP->Inf.Units;
				Txt+=I->Shield;
				break; } 
			case ve_UT_cannon: {
				vui_CannInfo* I=&SP->Inf.Cannon;
				Txt+=I->Shield;
				break; } 
		}
		if(Txt.pchar()[0]){
			T->SetMessage(Txt.pchar());
			T->Visible=true;
		}
	}
}
// va_W_Damage
void va_W_Damage::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	TextButton* T=(TextButton*)SD;
	GPPicture* P=(GPPicture*)SD->ParentDS;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	if(SP&&P&&T){
		//static _str hint=SD->Hint;
		//SD->SetHint(hint.pchar());
		SD->SetHint("DAMAGE_OF_SABRE");		
		_str Txt;
		if(SP->Type==ve_UT_brigade){
			vui_BrigInfo* I=&SP->Inf.Brigade;
			if(P->ID<2&&I->Damage[P->ID]){
				Txt.Add(I->Damage[P->ID]);
				if(I->DamageAdd[P->ID]){
					Txt+="+";
					Txt.Add(I->DamageAdd[P->ID]);
				}
			}			
		}else
			if(SP->Type==ve_UT_units){
				vui_UnitInfo* I=&SP->Inf.Units;
				if(P->ID<2&&I->Damage[P->ID]){
					Txt.Add(I->Damage[P->ID]);
				}
				if(SP->OB->newMons->Priest){
					SD->SetHint("");
				}			
			}
			if(Txt.pchar()){
				T->SetMessage(Txt.pchar());
				T->Visible=true;
			}
	}	
}
//
void cva_SP_Kinetic::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	TextButton* T=(TextButton*)SD;	
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		_str Txt;		
		if(SP->Type==ve_UT_units){
			vui_UnitInfo* I=&SP->Inf.Units;
			if(I->KineticMax>0){
				Txt.print("%d/%d",I->Kinetic,I->KineticMax);
			}
		}		
		if(Txt.pchar()){
			T->SetMessage(Txt.pchar());
			T->Visible=true;
		}
		TextButton* TB=Title.Get();
		if(TB){
			TB->Visible=Txt.pchar();
		}
	}
}
// va_W_Charge
void va_W_Charge::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	TextButton* T=(TextButton*)SD;
	GPPicture* P=(GPPicture*)SD->ParentDS;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&P){
		_str Txt;
		if(SP->Type==ve_UT_brigade){
			vui_BrigInfo* I=&SP->Inf.Brigade;
			if(P->ID==1){
				if(I->Damage[1]>0){
					Txt.Add(I->NShots);
					//Txt+="/";
					//Txt.Add(I->NLiveMembers);
				}
			}else
				if(P->ID==2){
					if(I->isGrenaders){
						Txt.Add(I->Grenades);
						Txt+="/";
						Txt.Add(I->GrenadesMax);
					}
				}		
		}else
			if(SP->Type==ve_UT_units){
				vui_UnitInfo* I=&SP->Inf.Units;
				if(P->ID==1){
					if(I->Damage[1]>0){
						Txt.Add(I->NShots);
						//Txt+="/";
						//Txt.Add(I->NLiveMembers);
					}
				}
			}
			if(Txt.pchar()){
				T->SetMessage(Txt.pchar());
				T->Visible=true;
			}
	}
}
// va_W_ChargeLine
void va_W_ChargeLine::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	Canvas* C=(Canvas*)SD;
	GPPicture* P=(GPPicture*)SD->ParentDS;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	int Ly=0;
	if(SP&&P){
		if(SP->Type==ve_UT_brigade){
			vui_BrigInfo* I=&SP->Inf.Brigade;
			if(P->ID==1){
				if(I->Damage[1]>0){
					Ly=C->GetHeight()*I->ReadyPercent/100;
				}
			}else
				if(P->ID==2){
					if(I->isGrenaders){
						Ly=C->GetHeight()*I->Grenades/I->GrenadesMax;
					}
				}		
		}else
			if(SP->Type==ve_UT_units){
				vui_UnitInfo* I=&SP->Inf.Units;
				if(P->ID==1){
					if(I->Damage[1]>0){
						if(I->DelayMax==0) I->DelayMax=1;
						Ly=C->GetHeight()*(I->DelayMax-I->Delay/I->Amount)/I->DelayMax;
					}
				}
			}
	}
	if(Ly>0){
		C->L=0;
		C->AddBar(0,C->GetHeight()-Ly,C->GetWidth(),Ly,0xFF00FF00);
		SD->Visible=true;
	}	
};
// va_GB_SelBuildings
void va_GB_SelBuildings::SetFrameState(SimpleDialog* SD){
	SD->Visible=OIS.SelPoint.GetAmount()==0 && MOptions.Players.Player[MyNation].AutoSelectBuildings && !NoText;

}
bool va_GB_SelBuildings::LeftClick(SimpleDialog* SD){
	vui_IS_GBSel Com;
	Com.Data.NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	Com.InterfAction();
	return true;
}
// va_DB_SetVarReturn
bool va_DB_SetVarReturn::LeftClick(SimpleDialog* SD){
	extern int MMVar_Return;
	MMVar_Return=SD->ID;
	return true;
}
//
extern bool vHidePreview;
//
// va_ListDesk
void va_ListDesk::SetFrameState(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	/*
	if(LD&&LD->DSS.GetAmount()==0){
	// add temp
	_str Msg;
	for(int i=0;i<10;i++){
	Msg.Clear();
	Msg="Element ";
	Msg+=i;
	LD->AddElement(Msg);
	}
	}
	*/
	// SD->vm_Init active item
	if(LD&&LD->CurrentElement>=0){
		int N=LD->DSS.GetAmount();
		if(N>0){
			VitButton* vb=dynamic_cast<VitButton*>(LD->Element.Get());
			if(vb) for(int i=0;i<N;i++){
				SimpleDialog* SD=LD->DSS[i];
				if(SD){
					SD->Enabled=LD->Enabled;
				}
				vb=(VitButton*)SD;
				if(vb){ 
					if(vHidePreview){
						vb->State=0;
					}else{
						vb->State=LD->CurrentElement==i?1:0;
					}
				}
			}			
		}else{
			LD->CurrentElement=-1;
		}
	}
	vHidePreview=false;
}
// va_ListItem
bool va_ListItem::LeftClick(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD->ParentDS);
	if(LD&&LD->Enabled&&!vHidePreview){
		int id=LD->DSS.find(SD);
		if(id!=LD->CurrentElement){
			if(LD->CurrentElement!=-1){
				VitButton* vb=dynamic_cast<VitButton*>(LD->DSS[LD->CurrentElement]);
				if(vb)vb->State=0;
			}			
			if(id!=-1){
				VitButton* vb=dynamic_cast<VitButton*>(LD->DSS[id]);
				if(vb)vb->State=1;
			}
			LD->CurrentElement=id;
		}
	}
	return true;
}
void va_ListItem::SetFrameState(SimpleDialog* SD){
	//if(SD->Visible)
	//	SD->CheckMouseOver(mouseX,mouseY);
}

extern int vCurMiss;
extern bool vls_Active;
// cva_CreateRandomMapName
//
bool mInitRNDMapName=false;
int mRND=0xF0000;
//
extern bool GameInProgress;
void cva_CreateRandomMapName::SetFrameState(SimpleDialog* SD){
	/*
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD&&LD->Visible){
	}
	*/
	if(GameInProgress)return;

	mMapPreview=false;
	PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
	if(I){
		if(v_MM_Host||vGameMode==gmSin){
			//CurMapName=MISSLIST.MISS[vCurMiss].MapName;
			vCurMiss=-1;
#ifdef _DEMO_
			mRND=0xF0000;
#else
			//mRND=GetTickCount();
			if(!mInitRNDMapName){
				mInitRNDMapName=true;
				mRND=GetTickCount();
			}
#endif _DEMO_
			_str name;
			DWORD GetRND_MapParam();
			name.print("RN0 %x %x ",mRND,GetRND_MapParam());
			bool color[8];
			memset(color,0,sizeof(color));
			for(int i=0;i<NPlayers||COMPSTART[i];i++){
				color[GSets.CGame.PL_INFO[i].ColorID]=true;
			}
			for(int i=0;i<8;i++){
				name+=color[i]?1:0;
			}
			sprintf(I->MapName,name.pchar());
			sprintf(GSets.CGame.cgi_CurrentMap,name.pchar());
			MOptions.reset_class(&MOptions);
			extern char pname[256];
			pname[0]=0;
			//strcpy(I->MapName,MISSLIST.MISS[vCurMiss].MapName);
		}
		vls_Active=false;
	}
}
// cva_Map_PreviewJpg
_str mpjFileName;
int mpjCx=0;
int mpjCy=0;
ClonesArray<cvi_Point>* ScreenSaver=NULL;
void SetMapPreviewJpg(SingleMission* SM){
	if(SM&&SM->NIntro){
		char name[128];
		if(sscanf(SM->Intro[0],"%s%d%d",name,&mpjCx,&mpjCy)==3){
			mpjFileName=name;
		};
		static ClonesArray<cvi_Point> SS;
		ScreenSaver=&SS;
		SS.Clear();
		if(SM->NIntro>1){
			DString str;
			str=SM->Intro[1];
			for(int i=0;i<2;i++){
				DString s;
				cvi_Point* P=new cvi_Point;
				P->x=mpjCx;
				P->y=mpjCy;
				str.ExtractWord(s);
				if(!s.isClear()) P->x=atoi(s.pchar());
				str.ExtractWord(s);
				if(!s.isClear()) P->y=atoi(s.pchar());
				SS.Add(P);
			}			
		}
	}
}
void SetMapPreviewJpg(char* FileName, int CenterX, int CenterY){
	if(FileName){
		mpjFileName=FileName;
		mpjCx=CenterX;
		mpjCy=CenterY;
	}
}
void SetMapPreviewJpg(cvi_Missions* CM){
	if(CM){
		mpjFileName=CM->MapPreview.pchar();
		mpjCx=CM->MapCenterX;
		mpjCy=CM->MapCenterY;
		ScreenSaver=&CM->ScreenSaver;
	}
}
void cva_Map_PreviewJpg::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if(SD->DSS.GetAmount()>0&&!(vls_Active||vHidePreview)){
		BitPicture* BP=dynamic_cast<BitPicture*>(SD->DSS[0]);
		if(BP){			
			if(BP->SetFileName(mpjFileName.pchar())){
				// new file name
				FirstDraw=true;				
			}
			if(!BP->FileName.isClear()){
				SD->Visible=true;
			}
		}
	}	
}
void CorrectPosition(int& x, int scrLx, int picLx){
	// correct position
	if(x+picLx<scrLx){
		x=scrLx-picLx;
		if(x>0){
			x/=2;
		}
	}else
		if(x>0){
			x=0;
		}
}
void CorrectFirstPosition(int& x, int scrLx, int picLx){
	if(x<scrLx/2){
		x=scrLx/2;
	}
	if(x>picLx-scrLx/2){
		x=picLx-scrLx/2;
	}
}
void cva_Map_PreviewJpg::OnDraw(SimpleDialog* SD){	
	if(SD->DSS.GetAmount()>0){
		BitPicture* BP=dynamic_cast<BitPicture*>(SD->DSS[0]);
		if(BP){
			static bool Rpress=false;
			//
			int x=BP->x;
			int y=BP->y;
			if(ScreenSaver&&ScreenSaver->GetAmount()>1){
				static bool ssActive=false;
				DWORD T=GetTickCount();
				static DWORD time=T;
				//
				static int state,state2;
				static int sx,sy,sLx,sLy;
				static int sx2,sy2,sLx2,sLy2;
				static float speed_x,speed_y;				
				int t=10000;
				int frame=40;
				if(FirstDraw){
					FirstDraw=false;					
					//state2=1;
					//sLx=BP->Pic.GetLx();
					//sLy=BP->Pic.GetLy();
					//for(int i=0;i<ScreenSaver->GetAmount();i++){
					//	CorrectFirstPosition((*ScreenSaver)[i]->x,SD->GetWidth(),BP->GetWidth());
					//	CorrectFirstPosition((*ScreenSaver)[i]->y,SD->GetHeight(),BP->GetHeight());						
					//}					
					ssActive=false;
					time=T;
					state=0;
					sx=(*ScreenSaver)[state]->x;
					sy=(*ScreenSaver)[state]->y;
					//
					BP->SetWidth(BP->Pic.GetLx());
					BP->SetHeight(BP->Pic.GetLy());
					//CorrectFirstPosition(sx,SD->GetWidth(),BP->Pic.GetLx());
					//CorrectFirstPosition(sy,SD->GetHeight(),BP->Pic.GetLy());
					//
					CorrectFirstPosition(sx,SD->GetWidth(),BP->GetWidth());
					CorrectFirstPosition(sy,SD->GetHeight(),BP->GetHeight());
					x=SD->GetWidth()/2-sx;
					y=SD->GetHeight()/2-sy;
				}
				if(Rpress){
					ssActive=false;
					time=T;					
				}
				//static speed;
				if(!ssActive&&time+2000<T){
					ssActive=true;
					time=T;
					state=-1;
					sx=SD->GetWidth()/2-x;
					sy=SD->GetHeight()/2-y;
				}
				if(ssActive){
					if(time+15000<T){
						time=T;
						state=state2;
						sx=(*ScreenSaver)[state]->x;
						sy=(*ScreenSaver)[state]->y;
						CorrectFirstPosition(sx,SD->GetWidth(),BP->GetWidth());
						CorrectFirstPosition(sy,SD->GetHeight(),BP->GetHeight());
					}
					if(time==T){
						if(state==-1){
							int norm=0;
							for(int i=0;i<2;i++){ //ScreenSaver->GetAmount()
								int n=Norma(sx-(*ScreenSaver)[i]->x,sy-(*ScreenSaver)[i]->y);
								if(norm<n){
									norm=n;
									state2=i;
								}
							}
							//state2=GetTickCount()%2;
						}else{
							state2=!state;
						}						
						sx2=(*ScreenSaver)[state2]->x;
						sy2=(*ScreenSaver)[state2]->y;
						CorrectFirstPosition(sx2,SD->GetWidth(),BP->GetWidth());
						CorrectFirstPosition(sy2,SD->GetHeight(),BP->GetHeight());
						int dx=sx2-sx;
						int dy=sy2-sy;
						speed_x=float(dx*frame)/t;
						speed_y=float(dy*frame)/t;
					}
					int tt=T-time;
					if(tt>t)tt=t;
					x=sx+speed_x*tt/frame;
					y=sy+speed_y*tt/frame;					
					x=SD->GetWidth()/2-x;
					y=SD->GetHeight()/2-y;
				}
			}else
				if(FirstDraw){
					FirstDraw=false;
					CorrectFirstPosition(mpjCx,SD->GetWidth(),BP->GetWidth());
					CorrectFirstPosition(mpjCy,SD->GetHeight(),BP->GetHeight());
					x=SD->GetWidth()/2-mpjCx;
					y=SD->GetHeight()/2-mpjCy;
				}
				if(true){				
					static int Sx;
					static int Sy;
					static int Bx;
					static int By;
					if(Rpressed||Lpressed){
						extern int mouseX;
						extern int mouseY;
						if(SD->MouseOver&&!Rpress){
							Rpress=true;
							Sx=mouseX;
							Sy=mouseY;
							Bx=x;
							By=y;
						}
						if(Rpress){
							x=Bx+mouseX-Sx;
							y=By+mouseY-Sy;
						}
					}else{
						Rpress=false;
					}
				}
				// correct position
				CorrectPosition(x,SD->GetWidth(),BP->GetWidth());
				CorrectPosition(y,SD->GetHeight(),BP->GetHeight());
				//CorrectFirstPosition(x,SD->GetWidth(),BP->Pic.GetLx());
				//CorrectFirstPosition(y,SD->GetHeight(),BP->Pic.GetLy());
				// setup
				BP->Setx(x);
				BP->Sety(y);
		}
	}
}
// va_vi_ReturnID
bool va_vi_ReturnID::LeftClick(SimpleDialog* SD){
	extern _str vi_ReturnID;
	if(SD) vi_ReturnID=SD->Name;
	return true;
}
extern bool vmBattles;
// va_LD_Skirmish
void va_LD_Skirmish::Init(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		LD->DSS.Clear();
	}
	vCurMiss=-1;
}
//
bool vHidePreview=false;
//
void va_LD_Skirmish::SetFrameState(SimpleDialog* SD){
	if(MISSLIST.MSMiss==0) return;
	//#ifndef _DEMO_
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD&&LD->Visible){
		vmBattles=false;
		vmRoSMission=false;
#ifdef _DEMO_
		MISSLIST.MSMiss=1;
#endif _DEMO_
		if(LD->DSS.GetAmount()!=MISSLIST.MSMiss){
			LD->DSS.Clear();
			_str mname;			
			for(int i=0;i<MISSLIST.MSMiss;i++){
				char* name=MISSLIST.MISS[MISSLIST.SingleMS[i]].Name;
				mname=GetTextByID(name);
				LD->AddElement(mname);
				LD->CurrentElement=0;
			}			
		}
		LD->Enabled=true;
		mMapPreview=false;
		vHidePreview=false;
		//
		char* GetSpecMap(int& rnd);
		int rnd=0;
		char* NAME=GetSpecMap(rnd);
		//
		PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
		if(I&&vGameMode!=gmSin){
			if(NAME&&strstr(NAME,"RANDOM")){
				LD->CurrentElement=rnd%MISSLIST.MSMiss;				//
				vHidePreview=true;
				LD->Enabled=false;
			}else{
				NAME=NULL;
				if(vCurMiss>=0){
					if(v_MM_Host){
						strcpy(I->MapName,MISSLIST.MISS[vCurMiss].MapName);
						mInitRNDMapName=false;
					}
				}
			}
			if(!v_MM_Host){
				LD->Enabled=false;
				I=GSets.CGame.GetHostPlayerInfo();
				if(I){
					/*
					if(strstr(I->MapName,"RANDOM")){
					static char name[64];
					sscanf(I->MapName,"%s%d",name,&LD->CurrentElement);
					NAME=name;
					vHidePreview=true;
					}else
					*/
					for(int i=0;i<MISSLIST.MSMiss;i++){
						if(!strcmp(MISSLIST.MISS[MISSLIST.SingleMS[i]].MapName,I->MapName)){
							vCurMiss=MISSLIST.SingleMS[i];
							SetMapPreviewJpg(MISSLIST.MISS+vCurMiss);
							CreateNationalMaskForMap(I->MapName);
							mMapPreview=true;
							LD->CurrentElement=i;
						};
					}
				}
			}
		}
		if(LD->Enabled){
			if(LastKey==VK_UP){
				LD->CurrentElement--;
				if(LD->CurrentElement<0)LD->CurrentElement=0;
				LastKey=0;
			}else
				if(LastKey==VK_DOWN){
					LD->CurrentElement++;
					if(LD->CurrentElement>=MISSLIST.MSMiss)LD->CurrentElement=MISSLIST.MSMiss-1;
					LastKey=0;
				}
		}
		if(LD->CurrentElement>=0&&LD->CurrentElement<MISSLIST.MSMiss){
			int id=MISSLIST.SingleMS[LD->CurrentElement];
			if(vCurMiss!=id){
				vCurMiss=id;				
				SetMapPreviewJpg(MISSLIST.MISS+vCurMiss);
				CreateNationalMaskForMap(MISSLIST.MISS[id].MapName);				
				BitPicture* P=Preview.Get();
				if(P){
					P->Visible=false;
				}				
				PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
				if(I){
					strcpy(I->MapName,MISSLIST.MISS[id].MapName);
				}				
			}
			mMapPreview=true;				
			vls_Active=false;
		}
	}
	//#endif _DEMO_
}
void vf_SetRandomNationID(int i){
	PlayerInfo* I=GSets.CGame.PL_INFO+i;	
	int n=I->NationID;
	if(I->NationID==0||I->NationID>GlobalAI.NAi){
		n=rand()*GlobalAI.NAi/RAND_MAX;		
	}else{
		n--;
	}
	if(i<NPlayers)I->NationID=n;
	else{
		COMPSTART[i]&=0x07FF;
		COMPSTART[i]|=(n&7)<<11;
	}
}
void va_LD_Skirmish::Accept(SimpleDialog* SD){
#ifndef _DEMO_
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD&&!vls_Active){
		if(LD->CurrentElement!=-1&&LD->CurrentElement<LD->DSS.GetAmount()){
			VitButton* VB=dynamic_cast<VitButton*>(LD->DSS[LD->CurrentElement]);
			if(VB){
				//int id=MISSLIST.GetIDbyName(VB->Message);
				int id=MISSLIST.SingleMS[LD->CurrentElement];				
				if(id!=-1){
					CurrentGameInfo* g=&GSets.CGame;
					for(int i=0;i<8;i++){
						if(i<NPlayers){
							vf_SetRandomNationID(i);
						}else if(COMPSTART[i]){

						}
					}
					GSets.CGame.UnitTransform=false;
					RunMission(id,vNewProf.m_iDifficulty);
					DriveMode()->StartMission();
					PlayGame();
				}
			}
		}
	}
#endif _DEMO_
}

// ----------- XENO
// LD_RoS
void va_LD_RoS::Init(SimpleDialog* SD)
{
	ListDesk* LD = dynamic_cast<ListDesk*>(SD);
	if(LD) LD->DSS.Clear();
	vCurMiss = -1;
}

void va_LD_RoS::Accept(SimpleDialog* SD)
{
	ListDesk* LD = dynamic_cast<ListDesk*>(SD);
	if(LD&&!vls_Active)
	{
		if(LD->CurrentElement!=-1&&LD->CurrentElement<LD->DSS.GetAmount()){
			VitButton* VB = dynamic_cast<VitButton*>(LD->DSS[LD->CurrentElement]);
			if(VB)
			{
				int id=MISSLIST.RoSMS[LD->CurrentElement];				
				if(id!=-1)
				{
					CurrentGameInfo* g=&GSets.CGame;
					for(int i=0;i<8;i++)
					{
						if(i<NPlayers) vf_SetRandomNationID(i);
						else if(COMPSTART[i]){
						}
					}
					GSets.CGame.UnitTransform=false;
					RunMission(id, vNewProf.m_iDifficulty);
					DriveMode()->StartMission();
					PlayGame();
				}
			}
		}
	}
}

void va_LD_RoS::SetFrameState(SimpleDialog* SD)
{
	if(MISSLIST.RoSMiss == 0) return;
	ListDesk* LD = dynamic_cast<ListDesk*>(SD);
	if(LD && LD->Visible)
	{
		vmRoSMission=true;
		vmBattles=false;
		if(LD->DSS.GetAmount()!=MISSLIST.RoSMiss)
		{
			LD->DSS.Clear();
			_str mname;			
			for(int i=0;i<MISSLIST.RoSMiss;i++)
			{
				char* name=MISSLIST.MISS[MISSLIST.RoSMS[i]].Name;
				mname=GetTextByID(name);
				LD->AddElement(mname);
				LD->CurrentElement=0;
			}			
		}
		LD->Enabled=true;
		mMapPreview=false;
		vHidePreview=false;
		char* GetSpecMap(int& rnd);
		int rnd=0;
		char* NAME=GetSpecMap(rnd);
		PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
		if(I&&vGameMode!=gmSin)
		{
			if(NAME&&strstr(NAME,"RANDOM"))
			{
				LD->CurrentElement=rnd%MISSLIST.MSMiss;	
				vHidePreview=true;
				LD->Enabled=false;
			}
			else
			{
				NAME=NULL;
				if(vCurMiss>=0)
				{
					if(v_MM_Host)
					{
						strcpy(I->MapName,MISSLIST.MISS[vCurMiss].MapName);
						mInitRNDMapName=false;
					}
				}
			}
			if(!v_MM_Host)
			{
				LD->Enabled=false;
				I=GSets.CGame.GetHostPlayerInfo();
				if(I)
				{
					for(int i=0;i<MISSLIST.RoSMiss;i++)
					{
						if(!strcmp(MISSLIST.MISS[MISSLIST.RoSMS[i]].MapName, I->MapName))
						{
							vCurMiss = MISSLIST.RoSMS[i];
							SetMapPreviewJpg(MISSLIST.MISS+vCurMiss);
							CreateNationalMaskForMap(I->MapName);
							mMapPreview=true;
							LD->CurrentElement=i;
						};
					}
				}
			}
		}
		if(LD->Enabled)
		{
			if(LastKey==VK_UP){
				LD->CurrentElement--;
				if(LD->CurrentElement<0)LD->CurrentElement=0;
				LastKey=0;
			}
			else
			{
				if(LastKey==VK_DOWN)
				{
					LD->CurrentElement++;
					if(LD->CurrentElement>=MISSLIST.RoSMiss)LD->CurrentElement=MISSLIST.RoSMiss-1;
					LastKey=0;
				}
			}
		}
		if(LD->CurrentElement>=0&&LD->CurrentElement<MISSLIST.RoSMiss)
		{
			int id=MISSLIST.RoSMS[LD->CurrentElement];
			if(vCurMiss!=id)
			{
				vCurMiss=id;				
				SetMapPreviewJpg(MISSLIST.MISS+vCurMiss);
				CreateNationalMaskForMap(MISSLIST.MISS[id].MapName);				
				BitPicture* P=Preview.Get();
				if(P) P->Visible=false;
				PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
				if(I) strcpy(I->MapName,MISSLIST.MISS[id].MapName);
			}
			mMapPreview=true;				
			vls_Active=false;
		}
	}
}
// ------------

// va_LD_Battle
void va_LD_Battle::Init(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		LD->DSS.Clear();
	}
	vCurMiss=-1;
}
void va_LD_Battle::SetFrameState(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD&&LD->Visible){
		vmBattles=true;
		vmRoSMission=false;
#ifdef _DEMO_
		SBattles.N=2;
#endif _DEMO_
		if(LD->DSS.GetAmount()!=SBattles.N){
			LD->DSS.Clear();
			_str mname;			
			for(int i=0;i<SBattles.N;i++){
				mname=SBattles.Battle[i].SM->Name;
				LD->AddElement(GetTextByID(mname.pchar()));
				LD->CurrentElement=0;
			}
		}		
		//static char* CurMapName=NULL;
		LD->Enabled=true;
		mMapPreview=false;
		if(vGameMode!=gmSin){			
			PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
			if(I&&vCurMiss>=0){
				if(v_MM_Host){
					//CurMapName=SBattles.Battle[i].SM->Name;
					strcpy(I->MapName,MISSLIST.MISS[vCurMiss].MapName);
					mInitRNDMapName=false;
				}else{
					LD->Enabled=false;
					I=GSets.CGame.GetHostPlayerInfo();
					if(I){
						for(int i=0;i<SBattles.N;i++){
							if(!strcmp(SBattles.Battle[i].SM->MapName,I->MapName)){
								vCurMiss=SBattles.Battle[i].GID;
								SetMapPreviewJpg(MISSLIST.MISS+vCurMiss);
								CreateNationalMaskForMap(I->MapName);
								mMapPreview=true;
								LD->CurrentElement=i;
							};
						}
					}
				}
			}
		}
		if(LD->Enabled){
			if(LastKey==VK_UP){
				LD->CurrentElement--;
				if(LD->CurrentElement<0)LD->CurrentElement=0;
				LastKey=0;
			}else
				if(LastKey==VK_DOWN){
					LD->CurrentElement++;
					if(LD->CurrentElement>=SBattles.N)LD->CurrentElement=SBattles.N-1;
					LastKey=0;
				}
		}
		//va_ListDesk::SetFrameState(SD);
		if(LD->CurrentElement>=0&&LD->CurrentElement<SBattles.N){
			int id=SBattles.Battle[LD->CurrentElement].GID;
			if(vCurMiss!=id){
				vCurMiss=id;
				SetMapPreviewJpg(MISSLIST.MISS+vCurMiss);
				CreateNationalMaskForMap(MISSLIST.MISS[id].MapName);
				BitPicture* P=Preview.Get();
				if(P){
					//P->FileName.Assign(MOptions.Preview.Temp.pchar());
					P->Visible=false;
				}
				PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
				if(I){
					strcpy(I->MapName,MISSLIST.MISS[id].MapName);
				}
			}
			//vls_Active=false;
			mMapPreview=true;
		}
		vls_Active=false;
	}
}
// cva_MissName
void cva_MissName::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&vCurMiss>-1&&vCurMiss<MISSLIST.NMiss){
		SingleMission* Miss=MISSLIST.MISS+vCurMiss;
		if(Miss){
			char* name=GetTextByID(Miss->Name);
			TB->SetMessage(name);
		}
	}
}
// va_MissDescription
extern _str vm_F1_VictoryCond;
void SetF1Text(char* Help);
void va_MissDescription::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		if(!vls_Active&&vCurMiss>-1&&vCurMiss<MISSLIST.NMiss){
			SingleMission* Miss=MISSLIST.MISS+vCurMiss;
			if(Miss)
			{
				bool desc_not_found = true;
				if(Miss->NIntro)
				{
					//char* fn=Miss->Intro[0];
					char* fn=Miss->Description;
					TB->Name=fn;
					ResFile f=RReset(fn);
					if(f!=INVALID_HANDLE_VALUE)
					{
						desc_not_found = false;
						int sz=RFileSize(f)+1;
						byte* buf=znew(byte,sz);
						RBlockRead(f,buf,sz);
						RClose(f);
						buf[sz-1]=0;
						TB->SetMessage(&buf);
						//SetF1Text(&buf);
						vm_F1_VictoryCond=(char*)buf;
						free(buf);
					}
				}
				if (desc_not_found)
				{
					TB->Name="";
					TB->SetMessage(TB->Name.pchar());
					//SetF1Text(&TB->Name.pchar());
					vm_F1_VictoryCond=TB->Name.pchar();					
				}
				GPPicture* gpPic=gpPicture.Get();
				if(gpPic&&Miss->NIntro>1){
					int spr=atoi(Miss->Intro[1]);
					gpPic->SetSpriteID(spr);
				}
			}
		}else{
			static char* t=GetTextByID("#EW2_SkirmishLoad");
			TB->SetMessage(t);
			TB->Name="";
			vCurMiss=-1;			
		}
	}
}
// va_MissMapBMP
void va_MissMapBMP::SetFrameState(SimpleDialog* SD){
	BitPicture* BP=dynamic_cast<BitPicture*>(SD);
	if(BP&&vCurMiss>-1&&vCurMiss<MISSLIST.NMiss){
		SingleMission* Miss=MISSLIST.MISS+vCurMiss;
		//DLLPath		
		if(Miss){			
			int l=strlen(Miss->DLLPath);
			if(l>3){
				BP->FileName.Assign(Miss->DLLPath);
				BP->FileName.Replace(".dll",".bmp");
			}
		}
	}
}
// cva_SP_AllowShoot
void cva_SP_AllowShoot::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI){
		if(SP->Type==ve_UT_building){
			vui_BuildInfo* I=&SP->Inf.Buildings;
			if(I->UI.Damage[0]){				
				SD->Visible=true;
				VitButton* VB=dynamic_cast<VitButton*>(SD);
				if(VB){
					VB->State=I->AllowShoot;
				}
			}
		}
	}
}
bool cva_SP_AllowShoot::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI){
		if(SP->Type==ve_UT_building){
			vui_BuildInfo* I=&SP->Inf.Buildings;
			void CmdSetSrVictim(byte NI,byte val);
			CmdSetSrVictim(SP->NI,I->AllowShoot);
		}
	}
	return true;
}
// va_SP_UpgBut
void CorrectUpgradeHint(_str& str, NewUpgrade* Upg){
	if( Upg->CtgUpgrade==0 || Upg->CtgUpgrade==2 || Upg->CtgUpgrade==5
		|| Upg->CtgUpgrade==6 || Upg->CtgUpgrade==8 || Upg->CtgUpgrade==10 
		|| Upg->CtgUpgrade==12 || Upg->CtgUpgrade==14
		|| Upg->CtgUpgrade==19 || Upg->CtgUpgrade==20 || Upg->CtgUpgrade==21
		|| Upg->CtgUpgrade==23 || Upg->CtgUpgrade==40 ){
			int Value=Upg->Value;
			if( Upg->ValGroup ){
				Value=Upg->ValGroup[0]&0xFFFFFF;
			}
			bool comment=false;
			bool reading=false;
			int n=strlen(Upg->Message);
			for(int i=0;i<n;i++){
				char ch=Upg->Message[i];
				if(ch==127){
					str.Add(ch);
					str.Add(Upg->Message[i+1]);
					str.Add(Upg->Message[i+2]);
					str.Add(Upg->Message[i+3]);
					str.Add(Upg->Message[i+4]);
					i+=4;
				}else{
					char ch2=Upg->Message[i+1];
					if( !comment && ch=='/' && ch2=='*' || comment && ch=='*' && ch2=='/' ){
						reading=false;
						comment=!comment;
						i++;
					}else /*if( !comment )*/{
						if( !comment && ch>='0' && ch<='9' ){
							if( !reading ){
								reading=true;
								//str+="%s";
								str.Add(Value);
							}
						}else{
							reading=false;
							str.Add(ch);
						}
					}
				}	
			}
		}else{
			str+=Upg->Message;
		}
};
void va_SP_UpgBut::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;

	//VitButton* VB=dynamic_cast<VitButton*>(SD);
	GPPicture* VB=dynamic_cast<GPPicture*>(SD);

	SD->ID=cva_OIS_Upgrade::curID++;
	vui_UpgradeInfo* UI=OIS.GetUpgradeInfoByIconPosition(SD->ID);

	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(VB&&SP&&UI){
		SD->Visible=true;
		//vu_UpgInfo* Upg=NULL;
		//for(int i=0;i<SP->Upg.GetAmount();i++){
		//	if(SP->Upg[i].ID==UI->Index){
		//		Upg=SP->Upg+i;
		//		break;
		//	}
		//}
		//bool enough=true;
		//for(int i=0;i<8;i++) if(XRESRC(SP->NI,i)<UI->Upg->Cost[i]){
		//	enough=false;
		//	break;
		//};
		// Visible
		//VB->State=0;
		//VB->Diffuse=0xFFFFFFFF;
		//VB->DeepColor=true;
		//GPPicture* GP=NULL;
		//if(VB->DSS.GetAmount()){
		//	GP=dynamic_cast<GPPicture*>(VB->DSS[0]);
		//}		
		//if(GP){				
		//	if( enough || Upg ) GP->Diffuse=0xFFFFFFFF;
		//	else GP->Diffuse=0x5FFFFFFF;
		//}
		//// Enable
		//if (!UI->Enable){
		//	VB->Diffuse=0x5FFFFFFF;
		//	if(GP)GP->Diffuse=0x5FFFFFFF;
		//}
		//if(SP->Upg.GetAmount()){
		//	if(Upg){
		//		//VB->State=1;
		//	}else{
		//		if(!SP->CanUpg){
		//			VB->Diffuse=0x5FFFFFFF;
		//			if(GP){
		//				GP->Diffuse=0x5FFFFFFF;
		//			}
		//		}
		//	}
		//}
		if( SD->MouseOver ){
			// Hint
			_str str;
			NewUpgrade* Upg=UI->Upg;
			CorrectUpgradeHint(str,Upg);
			EngSettings.HintTemplate.AddHotKey(&str,UI->HotKey);
			str+="\\ ";
			char txt[256];
			GetCostString(SP->NI,UI->Upg->Cost,txt);
			str+=txt;			
			SD->SetHint(&str);
			SD->HintWithHotKey=true;
		}
	}
}
bool va_SP_UpgBut::LeftClick(SimpleDialog* SD){
	if(GetKeyState(VK_SHIFT)&0x8000){
		RightClick(SD);
		return true;
	}
	//VitButton* VB=dynamic_cast<VitButton*>(SD);
	vui_UpgradeInfo* UI=OIS.GetUpgradeInfoByIconPosition(SD->ID);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(/*VB&&*/SP&&UI&&UI->Enable){
		if(SP->CanUpg){
			CmdPerformUpgrade(MyNation,UI->Index);
		}		
	}
	return true;
}
bool va_SP_UpgBut::RightClick(SimpleDialog* SD){
	//VitButton* VB=dynamic_cast<VitButton*>(SD);
	vui_UpgradeInfo* UI=OIS.GetUpgradeInfoByIconPosition(SD->ID);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(/*VB&&*/SP&&UI&&UI->Enable){
		//if(SP->Upg.ID==UI->Index) CmdStopUpgrade(SP->NI);
		//if(VB->State==1) 
		CmdStopUpgrade(MyNation,UI->Index);		
	}
	return true;
}
// va_SP_UpgStageLine
void va_SP_UpgStageLine::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;	
	SimpleDialog* PF=(SimpleDialog*)SD->ParentDS;
	vui_UpgradeInfo* UI=OIS.GetUpgradeInfoByIconPosition(PF->ID);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(UI&&SP){
		vu_UpgInfo* Upg=NULL; // апгрейд в исполнении
		for(int i=0;i<SP->Upg.GetAmount();i++){
			if(SP->Upg[i].ID==UI->Index){
				Upg=SP->Upg+i;
				break;
			}
		}
		bool enough=true;
		for(int i=0;i<8;i++) if(XRESRC(SP->NI,i)<UI->Upg->Cost[i]){
			enough=false;
			break;
		};
		bool Disable=!( Upg || SP->CanUpg&&UI->Enable&&enough );		
		if( Upg || Disable ){
			//Canvas* C=dynamic_cast<Canvas*>(SD);
			//if(C){
			//	int Ly=C->GetHeight()*Upg->Stage/Upg->StageMax;
			//	if(Ly>0){
			//		C->L=0;
			//		C->AddBar(0,C->GetHeight()-Ly,C->GetWidth(),Ly,0xFF00FF00);
			//		SD->Visible=true;
			//	}
			//}
			ProgressBar* PB=dynamic_cast<ProgressBar*>(SD);
			if(PB){
				SD->Visible=true;
				PB->Type=0;
				//if(PB->Type==0){
				PB->BackGP_File=UI->FileID;
				PB->BackSprite=UI->SpriteID;
				PB->BackColor=0;
				PB->ProgressGP_File=UI->FileID;
				PB->ProgressSprite=UI->SpriteID;
				PB->Diffuse=0xFFFFFFFF;
				if(Upg){
					// исполняется
					PB->ProgressColor=0xEE2020DD;
					PB->MaxValue=Upg->StageMax;
					PB->Value=Upg->Stage;//SP->Upg.StageMax-SP->Upg.Stage;
				}else{
					// запрещен
					PB->ProgressColor=0xFFDD0000;
					PB->MaxValue=100;
					PB->Value=100;
				}
				//}
			}		
		}	
	}
}
// va_SP_UpgPicture
void va_SP_UpgPicture::SetFrameState(SimpleDialog* SD){
	GPPicture* P=dynamic_cast<GPPicture*>(SD);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	SimpleDialog* PF=(SimpleDialog*)SD->ParentDS;
	vui_UpgradeInfo* UI=OIS.GetUpgradeInfoByIconPosition(PF->ID);
	if(P&&SP&&UI){
		P->FileID=UI->FileID;
		P->SetSpriteID(UI->SpriteID);
	}
}
// va_AR_MenuScroll
void va_AR_MenuScroll::SetFrameState(SimpleDialog* SD){
	static DWORD begin=GetTickCount();
	static long d; if(SD->ID==0) d=GetTickCount()-begin;
	int dTick=40;
	int dDiff=120;
	int n=SD->ParentDS->DSS.GetAmount();
	BitPicture* BP=dynamic_cast<BitPicture*>(SD);
	if(BP){		
		int Lx=BP->Pic.GetLx();
		if(Lx){
			int x=-Lx+(d/dTick+int(SD->ID)*Lx+Lx)%(Lx*n);
			BP->Setx(x);
			BP->Visible=true;
		}else{
			BP->Visible=false;
		}
		return;
	}
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP){
		int Lx=GP->GetWidth()-1;
		if(Lx){
			int x;
			if(SD->ID>9){
				x=-Lx+(d/dTick+SD->ID*Lx+Lx)%(Lx*n);
			}else{
				x=1024-(d/dTick-SD->ID*Lx+2048)%(Lx*n);
				//
				int dd=TCos[(d/dDiff)&0xFF]+125;
				if(dd<0) dd=0;
				if(dd>0xFF) dd=0xFF;				
				GP->Diffuse=(dd<<24)+0xFFFFFF;
			}			
			GP->Setx(x);
			GP->Visible=true;
		}
		return;
	}
}
// cva_MM_StartInDemo
bool cva_MM_StartInDemo::LeftClick(SimpleDialog* SD){
#ifdef _DEMO_
	if(v_MainMenu.Enable){
		Lpressed=false;
		if(!(SD->Name.pchar()&&v_MainMenu.StartDS(SD->Name.pchar()))) v_MainMenu.StartDS(Name.pchar());
	}
#endif _DEMO_
	return true;
}
// cva_MM_Start
bool cva_MM_Start::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		Lpressed=false;
		if(!(SD->Name.pchar()&&v_MainMenu.StartDS(SD->Name.pchar()))) v_MainMenu.StartDS(Name.pchar());
	}
	return true;
}
// cva_MM_StartInGame
bool cva_MM_StartInGame::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		Lpressed=false;
		v_MainMenu.StartDS(SD->Name.pchar(),true);
	}
	return true;
}
// cva_MM_StartInGameModal
bool cva_MM_StartInGameModal::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		Lpressed=false;
		int gM=v_MainMenu.GameMode;
		v_MainMenu.GameMode=false;
		v_MainMenu.StartDS(Name.pchar(),true);
		v_MainMenu.GameMode=gM;
	}
	return true;
}
// cva_MM_Close
bool cva_MM_Close::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		v_MainMenu.CloseDS();
	}
	return true;
}
// cva_MM_BackToMain
bool cva_MM_BackToMain::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){		
		if(NPlayers>1){
			if(PlayGameMode){
				GameExit=true;
			}else{
				void CmdEndGame(byte NI,byte state,byte cause);
				CmdEndGame(MyNation,1,123);
			}
		}else{
			GameExit=true;
			GSets.LeibzigDemo.RecBroken=true;
			ext_OnManualExit(MyNation);
		}
	}
	return cva_MM_Close::LeftClick(SD);
}
extern _str PlayerName;
bool v_MM_Host=false;
// cva_MM_MultiCreate
void cva_MM_MultiCreate::SetFrameState(SimpleDialog* SD){
	SD->Enabled=PlayerName.pchar()&&PlayerName.pchar()[0];
}
bool cva_MM_MultiCreate::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable&&!PlayerName.isClear()){
		//IPCORE.CloseNetwork();
		CurrentGameInfo* g=&GSets.CGame;
		strcpy(PlName,PlayerName.pchar());
		if(g->CreateRoom(LobbyVersion,7)){
			memset(COMPSTART,0,sizeof COMPSTART);

			vmPlayerData.Write();

			v_MM_Host=true;
			cva_MM_Start::LeftClick(SD);

			//g->LeaveRoom();
			//v_MainMenu.CloseDS();
		}
		//IPCORE.InitNetwork();
	}
	return true;
}
DynArray<SERVER_ENTRY> v_pServers;
SERVER_ENTRY v_curServer;
// cva_MM_MultiJoin
void cva_MM_MultiJoin::SetFrameState(SimpleDialog* SD){
	SD->Enabled=PlayerName.pchar()&&PlayerName.pchar()[0]&&v_curServer.m_szServerIP[0];
}
bool cva_MM_MultiJoin::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		//IPCORE.CloseNetwork();
		CurrentGameInfo* g=&GSets.CGame;
		strcpy(PlName,PlayerName.pchar());
		if(v_curServer.m_szServerIP&&g->JoinRoom(v_curServer.m_szServerIP,v_curServer.m_szSessionName)){

			vmPlayerData.Write();

			vWaitServerInfo=GetTickCount()+10000;
			v_MM_Host=false;
			GameInProgress=false;
			cva_MM_Start::LeftClick(SD);			
			//g->LeaveRoom();
			//v_MainMenu.CloseDS();
		};
		//IPCORE.InitNetwork();
	}
	return true;
}
MultStartErr v_MSErr;
_str StartDisableReason;
bool ShowStReason=false;
void cva_BR_WhyNotStart::OnDraw(SimpleDialog* SD){
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	if(VB&&VB->MouseOver){
		ShowStReason=true;
		return;
	}
}
void cva_BR_WhyNotStart::SetFrameState(SimpleDialog* SD){
	DialogsDesk* DD=dynamic_cast<DialogsDesk*>(SD);
	if(DD){
		DD->Visible=ShowStReason&&StartDisableReason.pchar()&&StartDisableReason.pchar()[0];
		return;
	}
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		if(StartDisableReason.pchar())TB->SetMessage(StartDisableReason.pchar());
		TB->Visible=ShowStReason&&StartDisableReason.pchar()&&StartDisableReason.pchar()[0];
		ShowStReason=false;
		return;
	}
}
void SetStartEnabled(SimpleDialog* SD){
	if(GameInProgress)return;

	StartDisableReason=GetTextByID("You_cant_start_game");
	StartDisableReason+="\\";

	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* CI=g->GetCurrentPlayerInfo();
	if(NPlayers>8)NPlayers=8;
	memset(v_MSErr.Color,0,sizeof(v_MSErr.Color));
	memset(v_MSErr.Team,0,sizeof(v_MSErr.Team));
	SD->Enabled=true;
	//if(NPlayers>1){
	bool col[8]={false,false,false,false,false,false,false,false};
	int nColor=0;			
	for(int i=0;i<8;i++){
		if(i<NPlayers){
			PlayerInfo* I=g->PL_INFO+i;
			col[I->ColorID]=true;
		}else
			if(COMPSTART[i]){
				col[COMPSTART[i]&15]=true;
			}
	}	
	for(i=0;i<8;i++){
		if(col[i]){
			nColor++;
		}
	}
	int NPlMax=7;
	if(vGameMode==gmSin){
		NPlMax=MOptions.Players.GetN_Single();
	}else
		if(vGameMode==gmMul){
			NPlMax=MOptions.Players.GetN_Multi();
		}
		if(MOptions.MaxPlayers&&MOptions.MaxPlayers<NPlMax)NPlMax=MOptions.MaxPlayers;
		if(nColor>NPlMax){
			SD->Enabled=false;
			StartDisableReason+=GetTextByID("#too_many_players_on_map");
			StartDisableReason+="\\";
		}
		int nTeam=0;		
		for(int i=0;i<NPlayers||COMPSTART[i];i++){
			PlayerInfo* I=g->PL_INFO+i;
			if(I->GroupID==0||v_MSErr.Team[I->GroupID]==0){
				nTeam++;
			}
			v_MSErr.Team[I->GroupID]++;
		}
		if(nTeam==1||nColor==1&&!vls_Active){ //&&NPlayers>1
			//memset(v_MSErr.Team,1,sizeof(v_MSErr.Team));
			SD->Enabled=false;
			if(NPlayers==1){
				StartDisableReason+=GetTextByID("#you_are_alone_in_room");
			}else{
				StartDisableReason+=GetTextByID("#all_players_in_one_team");
			}
			StartDisableReason+="\\";
		}else{
			memset(v_MSErr.Team,0,sizeof(v_MSErr.Team));
			if(vGameMode==gmMul){
				if(NPlayers<2){
					SD->Enabled=false;
					StartDisableReason+=GetTextByID("#you_are_alone_in_room");
				}
			}
		}
		bool enf=false;
		/*
		// each player have diffirent color	
		for(int i=0;i<NPlayers||COMPSTART[i];i++){
		PlayerInfo* I=g->PL_INFO+i;		
		//if(!I->ColorID){
		for(int j=0;j<NPlayers||COMPSTART[j];j++)if(j!=i){
		PlayerInfo* J=g->PL_INFO+j;
		if(J->ColorID==I->ColorID){
		v_MSErr.Color[I->ColorID]=true;
		SD->Enabled=false;
		enf=true;
		}
		}
		//}
		}	
		if(enf){
		StartDisableReason+=GetTextByID("#players_shoul_have_diff_colors");
		StartDisableReason+="\\";
		}
		*/
		if(NPlayers>1){
			enf=false;
			PlayerInfo* PI=GSets.CGame.GetHostPlayerInfo();
			if(PI&&PI->MapHashValue&&strncmp(PI->MapName,"RN0 ",4)){
				for(int i=0;i<NPlayers;i++){
					if(PI->MapHashValue!=GSets.CGame.PL_INFO[i].MapHashValue){
						//SD->Enabled=false;
						//enf=true;
					}
				}
			}
			if(enf){
				StartDisableReason+=GetTextByID("#players_have_diff_map_checksumm");
				StartDisableReason+="\\";
			}
		}
		if(NPlayers>1){
			enf=false;
			for(int i=0;i<NPlayers;i++)if(!GSets.CGame.PL_INFO[i].Ready){
				SD->Enabled=false;
				enf=true;
			}
			if(enf){
				StartDisableReason+=GetTextByID("#not_all_players_ready");
				StartDisableReason+="\\";
			}
		}
		int rp=g->GetReadyPercent();
		if(v_MM_Host&&NPlayers>1&&rp<99){
			char cc[256];
			sprintf(cc,GetTextByID("#should_wait_for_ping"),rp);
			StartDisableReason+=cc;
			StartDisableReason+="\\";
			SD->Enabled=false;
		}
		if(!(CI&&CI->MapName[0])){
			SD->Enabled=false;
			StartDisableReason+=GetTextByID("#map_is_absent");
			StartDisableReason+="\\";
		}
		SD->Enabled=SD->Enabled&&CI&&CI->MapName[0];
		//
		extern bool LoadDisable;
		if(vls_Active){
			if(LoadDisable){
				LoadDisable=false;
				SD->Enabled=false;
			}
			if(NPlayers>vSaveInfo.NPlayers){
				SD->Enabled=false;
			}
		}
		//
		if(SD->Enabled)StartDisableReason="";
}
// cva_MM_MultiStart
void cva_MM_MultiStart::SetFrameState(SimpleDialog* SD){
	//if(GameInProgress)return;
	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* CI=g->GetCurrentPlayerInfo();
	SetStartEnabled(SD);
	//SD->Enabled=SD->Enabled&&v_MM_Host&&NPlayers>1&&g->GetReadyPercent()>99;
	if(v_MM_Host){
		for(int i=0;i<NPlayers;i++){
			DWORD id=g->PL_INFO[i].PlayerID;				
			if(g->GetPing(id)>15000){
				g->Kick(id);
			}
		}
		SD->Visible=true;
	}else{
		SD->Visible=false;
	}
	bool CheckRoomRun();
	if(SD->Visible&&SD->Enabled&&CheckRoomRun()){
		LeftClick(SD);
	}
	//SD->Enabled=true;
}
bool cva_MM_MultiStart::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		CurrentGameInfo* g=&GSets.CGame;
		PlayerInfo* I=g->GetCurrentPlayerInfo();
		if(v_MM_Host/*&&(I->Page==0||I->Page==1)*/){

			GSets.Save();

			CurrentGameInfo* g=&GSets.CGame;
			for(int i=0;i<NPlayers||COMPSTART[i];i++){
				vf_SetRandomNationID(i);
			}		
			//strcpy(I->MapName,MISSLIST.MISS[vCurMiss].MapName);
			GSets.CGame.UnitTransform=true;
			g->RunHostGame();
			g->LeaveRoom();
			//v_MainMenu.CloseDS(); // это нужно только в интернет игре, необходимо добавить екшен в редакторе
			mInitRNDMapName=false;
		}
	}
	return true;
}
// cva_MM_SinglStart
void cva_MM_SinglStart::SetFrameState(SimpleDialog* SD){	
	if(!vls_Active){
		SetStartEnabled(SD);
		//SD->Enabled=SD->Enabled||vls_Active;
	}
}
bool cva_MM_SinglStart::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		if(!vls_Active){
			CurrentGameInfo* g=&GSets.CGame;
			// correct color
			for(int i=0;i<NPlayers||COMPSTART[i];i++){
				vf_SetRandomNationID(i);
			}
			//
			PlayerInfo* I=g->PL_INFO;
			SetMyNation(I->ColorID);					
			if(vCurMiss>=0&&vCurMiss<MISSLIST.NMiss){
				GSets.CGame.UnitTransform=NationConvertUnits;
				RunMission(vCurMiss,0);
				DriveMode()->StartMission();
				PlayGame();
			}else{
				GSets.CGame.UnitTransform=true;
				g->RunSingleplayerGame(I->ColorID);
			}
			NPlayers=1;
		}else{
			//GSets.CGame.UnitTransform=true;
			//g->RunSingleplayerGame(I->ColorID);
		}
	}
	return true;
}
// cva_MM_DialogsEditor
bool cva_MM_DialogsEditor::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		if(GetKeyState(VK_SHIFT)&0x8000){
			UnPress();
			void SelectEditor();
			SelectEditor();
		}else
			if(GetKeyState(VK_MENU)&0x8000){
				v_MainMenu.Enable=false;
				ProcessDialogsEditor();
				v_MainMenu.Enable=true;			
			}else{
				if(!Ctrl||(GetKeyState(VK_CONTROL)&0x8000)){
					vGameMode=gmEdit;
					LOADNATMASK=0xFFFFFFFF;
					PrepareEditMedia(0);
					NPlayers=1;
					EditGame();
					vGameMode=gmSin;
				}
			}
	}
	return true;
}
// cva_Multi_ManualServer
bool vManualServer=false;
void cva_Multi_ManualServer::SetFrameState(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		vManualServer=CB->State;
	}
}
// cva_Multi_ManualIPServer
_str vManualIPServer;
void cva_Multi_ManualIPServer::SetFrameState(SimpleDialog* SD){
	SD->Enabled=vManualServer;
	if(SD->DSS.GetAmount()){
		InputBox* IB=dynamic_cast<InputBox*>(SD->DSS[0]);
		if(IB){
			IB->StrMaxLen=32;
			IB->_Str=&vManualIPServer;
			IB->Str=NULL;
			IB->Enabled=vManualServer;
			//IB->Active=vManualServer;
		}
	}
}
// cva_LD_MultiGame list
void cva_LD_MultiGame::SetFrameState(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){		
		if(LD->DSS.GetAmount()&&LD->CurrentElement>=0&&LD->CurrentElement<v_pServers.GetAmount()){
			v_curServer=v_pServers[LD->CurrentElement];
		}else{
			v_curServer.m_szServerIP[0]=0;
		}
		//
		static int LastRefresh=0;
		int time=GetTickCount();
		if(time-LastRefresh>4000){
			//	if(vManualServer&&!vManualIPServer.isClear()){
			//		IPCORE.StartServerPoll(vManualIPServer.pchar());
			//	}else{
			//		IPCORE.StartServerPoll();
			//	}			
			//	LastRefresh=time;
			//}
			//if(time-LastRefresh>500){
			LD->DSS.Clear();
			v_pServers.Clear();
			LD->CurrentElement=-1;
			LPSERVER_ENTRY pServers=NULL;
			DWORD dwNS=IPCORE.EnumServers(pServers);
			if(pServers){
				for(int i=0;i<dwNS;i++,pServers++){
					if(pServers->m_bAvailable&&!strcmp(pServers->m_szSessionName,LobbyVersion)){
						int id=-1;
						for(int j=0;j<v_pServers.GetAmount();j++){
							if(strcmp(pServers->m_szUserName,v_pServers[j].m_szUserName)<0){
								v_pServers.Insert(j,*pServers);
								id=j;
								break;
							}
						}
						if(id<0){
							id=v_pServers.Add(*pServers);
						}
					}
				}
				for(int i=0;i<v_pServers.GetAmount();i++){
					_str SessName;
					SessName.print("%s [ip: %s]",v_pServers[i].m_szUserName,v_pServers[i].m_szServerIP);
					LD->AddElement(SessName);
					if(!(strcmp(v_pServers[i].m_szServerIP,v_curServer.m_szServerIP)||
						strcmp(v_pServers[i].m_szSessionName,v_curServer.m_szSessionName)||
						strcmp(v_pServers[i].m_szUserName,v_curServer.m_szUserName))){
							LD->CurrentElement=i;
							v_curServer=v_pServers[i];
						}				
				}
			}
			if(LD->CurrentElement<0) v_curServer.m_szServerIP[0]=0;

			if(vManualServer&&!vManualIPServer.isClear()){
				IPCORE.StartServerPoll(vManualIPServer.pchar());
			}else{
				IPCORE.StartServerPoll();
			}			
			LastRefresh=time;
		}
		va_ListDesk::SetFrameState(SD);		
	}
}
// cva_CircleNormaTest
CEXPORT void DrawColoredCircle(int xc,int yc,int R0,int R1,DWORD LineColor1,DWORD LineColor2,DWORD FillColor1,DWORD FillColor2);
void cva_CircleNormaTest::OnDraw(SimpleDialog* SD){
	int x=SD->x+SD->GetWidth()/2;
	int y=SD->y+SD->GetHeight()/2;
	int r=SD->GetWidth()<SD->GetHeight()?SD->GetWidth()/2:SD->GetHeight()/2;
	//DrawColoredCircle(x,y,r,r-2,0xFFFFFFFF,0xFFFFFFFF,0,0);
	//DrawColoredCircle(x,y,r*92/100,r*92/100-2,0xFFFFFF00,0xFFFFFF00,0,0);
	float d=2.0*3.1415/256.0;
	float x0=r;
	float y0=0;
	for(int i=0;i<256;i++){		
		float a1=float(i+1)*d;
		float x1,y1;		
		float n=float(r)/2.0;
		int norm=n;
		while(norm<r){
			x1=n*cos(a1);
			y1=n*sin(a1);
			norm=Norma(x1,y1);
			n+=float(r)/1000;
		}
		GPS.DrawLine(x+x0,y+y0,x+x1,y+y1,0xFFFF00FF);
		x0=x1;
		y0=y1;
		float a=float(i)*d;
		int x2,y2,x3,y3;
		x2=r*cos(a);
		y2=r*sin(a);
		x3=r*cos(a1);
		y3=r*sin(a1);
		GPS.DrawLine(x+x2,y+y2,x+x3,y+y3,0xFF0000FF);		
		x2=r*cos(a)*92/100;
		y2=r*sin(a)*92/100;
		x3=r*cos(a1)*92/100;
		y3=r*sin(a1)*92/100;
		GPS.DrawLine(x+x2,y+y2,x+x3,y+y3,0xFFFFFF00);
	}
}
// cva_MM_MultiEnter
bool cva_MM_MultiEnter::LeftClick(SimpleDialog* SD){
#ifndef _DEMO_
	if(v_MainMenu.Enable){
		vGameMode=gmMul;
		CreateMultiplaterInterface();
		cva_MM_Start::LeftClick(SD);
	}
#endif _DEMO_
	return true;
}
// cva_MM_MultiBack
void cva_MM_MultiBack::SetFrameState(SimpleDialog* SD){	
}
bool cva_MM_MultiBack::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		//IPCORE.CloseNetwork();
		//cva_MM_Close::LeftClick(SD);
	}
	return true;
}
// cva_HotKeyButton
void cva_HotKeyButton::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	for(int i=0;i<v_GlobalHotKeys.HotKeys.GetAmount();i++){
		vui_GlobalHotKey* HK=v_GlobalHotKeys.HotKeys[i];
		if(HK&&HK->KeyID==SD->HotKey){
			VitButton* VB=dynamic_cast<VitButton*>(SD);
			if(VB){
				VB->State=HK->GetState();
				VB->Visible=true;
			}			
			break;
		}
	}
}
bool cva_HotKeyButton::LeftClick(SimpleDialog* SD){	
	for(int i=0;i<v_GlobalHotKeys.HotKeys.GetAmount();i++){
		vui_GlobalHotKey* HK=v_GlobalHotKeys.HotKeys[i];
		if(HK&&HK->KeyID==SD->HotKey){
			VitButton* VB=dynamic_cast<VitButton*>(SD);
			if(VB){
				HK->Action();
			}			
			break;
		}
	}
	return true;
};
// cva_MM_RunMapEditor
bool cva_MM_RunMapEditor::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		if(GetKeyState(VK_CONTROL)&0x8000){
			v_MainMenu.Enable=false;
			ProcessDialogsEditor();
			v_MainMenu.Enable=true;
		}else{
			vGameMode=gmEdit;
			LOADNATMASK=0xFFFFFFFF;
			PrepareEditMedia(0);
			EditGame();
		}
	}
	return true;
}
void vfPlayRecorsList(){
	PlayGameMode=1;
	/*
	char FileName[]="RecordsList.xml";
	xmlQuote cfg(FileName);
	if(!cfg.ReadFromFile(FileName)){				
	cfg.AddSubQuote("Rec","rec00.rec");
	cfg.AddSubQuote("Rec","rec01.rec");				
	cfg.WriteToFile(FileName);
	};
	*/
	//
	GSets.LeibzigDemo.RecBroken=false;
	GSets.LeibzigDemo.StartRecord();
	/*
	//Enumerator* E=Dialogs=ENUM.Get("ve_MainMenuDialogs");
	for(int i=0;i<cfg.GetNSubQuotes();i++){
	xmlQuote* sub=cfg.GetSubQuote(i);
	PlayRecfile((char*)sub->Get_string());
	if(GSets.LeibzigDemo.RecBroken) break;
	//addDStoEnum(E,sub->GetQuoteName(),sub->Get_string());
	}
	*/
	PlayGameMode=0;
}
//
bool GetMusicMode();
void SetMusicMode(bool State);
void SetCDVolumeEx(int Vol);
CEXPORT int GetCDVolume();
extern long StartAboutTime;
extern long FinishAboutTime;
//
// cva_CreditsDesk
void cva_CreditsDesk::SetFrameState(SimpleDialog* SD){
	SD->DeepColor=true;
	int T=GetTickCount();
	int A=0;
	if(T>FinishAboutTime && FinishAboutTime!=0xFFFFFFF){
		A=255*(FinishAboutTime+1000-T)/1000;
	}else{
		A=255*(T-(StartAboutTime+2000))/1000;
	}	
	if(A<0){
		A=0;
	}
	if(A>255) A=255;
	SD->Diffuse=(A<<24)+0xFFFFFF;

	if(T-FinishAboutTime>1000 && FinishAboutTime!=0xFFFFFFF){
		SD->v_Actions.LeftClick(SD);
	}

}
// cva_ShowAboutText
void ShowAbout();
void cva_ShowAboutText::OnDraw(SimpleDialog* SD){
	GPS.FlushBatches();
	ShowAbout();
	GPS.FlushBatches();
}
// cva_MM_RunCredits
bool vCreditsMode=false;
bool cva_MM_RunCredits::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		StartAboutTime=GetTickCount();
		FinishAboutTime=0xFFFFFFF;
		//vCreditsMode=true;
		//vfPlayRecorsList();
		//vCreditsMode=false;
		LastKey=0;
	}
	return true;
}
// cva_MM_RunShopingDemo
bool cva_MM_RunShopingDemo::LeftClick(SimpleDialog* SD){
	LastKey=0;
	do{
		//void PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
		if(!Rolik.isClear()){
			PlayFullscreenVideo(Rolik.pchar(),0,0);
		}
		if(LastKey==0&&!Demka.isClear()){
			PlayGameMode=1;
			//vCreditsMode=true;
			//void PlayRecfile(char*);			
			PlayRecfile(Demka.pchar());
			PlayGameMode=0;
			//vCreditsMode=false;
		}
	}while(LastKey==0);
	LastKey=0;
	return true;
}
// cva_MM_RunRecordGame
bool cva_MM_RunRecordGame::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){		
		GSets.LeibzigDemo.StartVideo();
	}
	return true;
}
// cva_MM_RunSingleMiss
bool cva_MM_RunSingleMiss::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		if(SD->Name!=""){
			int id=-1;
			if(!SD->Name.isClear()){
				id=MISSLIST.GetID(SD->Name.pchar());
			}else
				if(!Name.isClear()){
					id=MISSLIST.GetID(Name.pchar());
				}
				if(id!=-1){
					//CurrentGameInfo* g=&GSets.CGame;
					//strcpy(g->cgi_CurrentMap,"B8-Aus-Frn.m3d");
					//g->RunSingleplayerGame(0);
					SetMyNation(0);
					GSets.CGame.UnitTransform=false;
					RunMission(id,0);
					DriveMode()->StartMission();
					PlayGame();
				}
		}
	}
	return true;
}
bool cva_MM_PlayOgg::LeftClick(SimpleDialog* SD){
	ov_Play(Name.pchar(),0);
	return true;
}
bool cva_MM_StopOgg::LeftClick(SimpleDialog* SD){
	ov_Stop(0);
	return true;
}
// cva_MM_ReplayCurMiss
bool cva_MM_ReplayCurMiss::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){		
		REPLAY();
	}
	return cva_MM_Close::LeftClick(SD);
}

void cva_MM_Defeat_ReplayCurMiss::SetFrameState(SimpleDialog* SD){
	SD->Visible=!GSets.AutoSaveWasDone;//(GSets.AutoSavePeriod>0 && TrueTime>10*60*1000);
}
bool cva_MM_Defeat_ReplayCurMiss::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){		
		REPLAY();
	}
	return cva_MM_Close::LeftClick(SD);
}

extern _str vls_FileName;
extern char GameName[128];
char* GetLDFileName();
void cva_MM_Defeat_LoadAutoSave::SetFrameState(SimpleDialog* SD){
	SD->Visible=GSets.AutoSaveWasDone;//(GSets.AutoSavePeriod>0 && TrueTime>10*60*1000);
}
bool cva_MM_Defeat_LoadAutoSave::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){		
		//vls_FileName="AUTO.sav";
		//strcpy(GameName,GetLDFileName());
		//extern bool vmVictory;
		//vmVictory=false;
		////GSets.CGame.UnitTransform=false;
		//vLoadFile=GameName;
		//if(!LoadInGame()){
		//	if(!InGame){
		//		PlayGame();
		//		if(vGameMode==gmCamp){
		//			void vmCheckVictory();
		//			vmCheckVictory();
		//		}
		//		v_MainMenu.Cross=1;
		//		v_MainMenu.CrossTime=0;
		//		v_MainMenu.CrossSD=NULL;
		//	}
		//}
	}
	//return cva_MM_Close::LeftClick(SD);
	return true;
}

// cva_ResPanel_Money
void cva_ResPanel_Money::SetFrameState(SimpleDialog* SD){

	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	int uMax=GetMaxUnits(NI);

	bool ShowResource=(CITY[NI].NPeas>0 || uMax>0);

	int MON[8];
	GetCorrectMoney(NI,MON);
	int n=SD->DSS.GetAmount();
	_str txt;

	int ResID=-1;
	//if(dialog){
	ResID=v_ISys.GetResourceID(GSets.CGame.InterfaceNatID,/*dialog->*/SD->ID);
	if( ResID!=-1 && ResID<6 ){
		if(SD->MouseOver)
			SD->/*dialog->*/SetHint(EngSettings.Resource.Name[ResID]);
	}
	//}

	if( ShowResource&&(ResID!=-1||SD->ID>7) || SD->ID==0 ){
		SD->Visible=true;
		for(int i=0;i<n;i++){

			SimpleDialog* dialog=SD->DSS[i];
			//dialog->Visible=false;

			TextButton* TB=dynamic_cast<TextButton*>(dialog);
			if(TB){
				if(ResID!=-1){
					//dialog->Visible=(ShowResource||TB->ID==0);
					if(MON[ResID]>999999){
						MON[ResID]=999999;
					}					
					int np=CITY[NI].ResourcePeasants[ResID];
					if(np){
						txt.Clear();
						txt.print("%d(%d)",MON[ResID],np);
					}else{
						txt=MON[ResID];
					}
					TB->SetMessage(txt.pchar());
				}else{				
					//dialog->Visible=ShowResource;
					int u=GetCurrentUnits(NI);
					txt.Clear();					
					if(uMax&&uMax<=u&&(GetTickCount()&512)){
						txt+="{CD}";
					}
					txt+=u;
					if(uMax){	
						txt+="{C}/";
						if(uMax<=u){
							txt+="{CD}";
						}
						txt+=uMax;
					}					
					TB->SetMessage(txt.pchar());
				}
			}else{
				GPPicture* GP=dynamic_cast<GPPicture*>(dialog);
				if(GP){
					if( ResID!=-1 && ResID<6 ){
						//dialog->Visible=(ShowResource||GP->ID==0);
						GP->FileID=EngSettings.Resource.File;
						GP->SetSpriteID(EngSettings.Resource.Sprite[ResID]);
					}else{
						int f,s;
						if( SD->ID!=0 && v_ISys.GetLivingPlacesPic(GSets.CGame.InterfaceNatID,f,s) ){
							GP->FileID=f;
							GP->SetSpriteID(s);
						}
					}
				}
			}

		}
	}else{
		SD->Visible=false;
	};

}
// cva_SP_EnableAttack
void cva_SP_EnableAttack::SetFrameState(SimpleDialog* SD){	
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		GPPicture* GP=dynamic_cast<GPPicture*>(SD);
		if(GP){
			if(SP->SearchVictim) GP->SetSpriteID(10);
			else GP->SetSpriteID(9);
		}
		VitButton* VB=dynamic_cast<VitButton*>(SD);
		if(VB){
			if(SP->SearchVictim) VB->State=1;
			else VB->State=0;
		}
	}
}
bool cva_SP_EnableAttack::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->NI==GSets.CGame.cgi_NatRefTBL[MyNation]){
		if(SP->SearchVictim) CmdSetSrVictim(MyNation,1);
		else CmdSetSrVictim(MyNation,0);
	}
	return true;
}
// cva_SP_id_Life
void cva_SP_id_Life::SetFrameState(SimpleDialog* SD){
	int id=SD->ID;
	ParentFrame* PF=(ParentFrame*)SD;
	while(id>0&&PF){
		id--;
		PF=PF->ParentDS;
	}
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP){

		ProgressBar* PB=dynamic_cast<ProgressBar*>(SD);

		_str Txt;
		switch(SP->Type){
			case ve_UT_cannon: {
				break; }
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				Txt.Add(I->Life);
				Txt+="/";
				Txt.Add(I->MaxLife);
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&SP->Inf.Units;
				Txt.Add(I->Life/I->Amount);
				Txt+="/";
				Txt.Add(I->LifeMax);
				if(PB){
					PB->Value=I->Life/I->Amount;
					PB->MaxValue=I->LifeMax;
				}
				break; }
			case ve_UT_building: {
				vui_BuildInfo* I=&SP->Inf.Buildings;
				Txt.Add(I->Life/I->Amount);
				Txt+="/";
				Txt.Add(I->LifeMax);
				if(PB){
					PB->Value=I->Life/I->Amount;
					PB->MaxValue=I->LifeMax;
				}
				break; }
		}
		if(Txt.pchar()){
			TextButton* T=dynamic_cast<TextButton*>(SD);
			if(T) T->SetMessage(Txt.pchar());
			if(PB){
				PB->Message=Txt;
				if(strlen(Txt.pchar())>7){
					PB->Font=GetFont("WindowText");
				}else{
					PB->Font=GetFont("WindowText2");
					if(PB->Font==NULL){
						PB->Font=GetFont("WindowText");
					}
				}
			}
		}
	}	
}
// cva_SP_id_Mana
void cva_SP_id_Mana::SetFrameState(SimpleDialog* SD){
	int id=SD->ID;
	ParentFrame* PF=(ParentFrame*)SD;
	//if(GlobalID){
	SD->ID=GlobalSelPointID;
	//}else{
	//	while(id>0&&PF){
	//		id--;
	//		PF=PF->ParentDS;
	//	}
	//}
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	SD->Visible=false;
	if( SP ){
		//HeroVariableStorage* HVS=DetectHero(SP->OB);
		//if(HVS){
		if( SP->OB->MaxMana>0 ){
			ProgressBar* PB=dynamic_cast<ProgressBar*>(SD);
			if(PB){
				PB->Visible=true;
				PB->Value=SP->OB->Mana;
				PB->MaxValue=SP->OB->MaxMana;
			}
		}
	}
}
// cva_SP_id_LifeLineHor
void cva_SP_id_LifeLineHor::SetFrameState(SimpleDialog* SD){
	int id=SD->ID;
	ParentFrame* PF=(ParentFrame*)SD;
	//if(GlobalID){
	SD->ID=GlobalSelPointID;
	//}else{
	//	while(id>0&&PF){
	//		id--;
	//		PF=PF->ParentDS;
	//	}
	//}
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	SD->Visible=false;
	int L=0,LMax=0,A=1;
	if(SP){
		switch(SP->Type){
			case ve_UT_cannon: {
				break; }
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				L=I->Life;
				LMax=I->MaxLife;
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&SP->Inf.Units;
				if(I->LifeMax==0)I->LifeMax=1;
				L=I->Life;
				LMax=I->LifeMax;
				A=1;
				break; }
			case ve_UT_building: {
				vui_BuildInfo* I=&SP->Inf.Buildings;
				L=I->Life;
				LMax=I->LifeMax;
				A=1;
				break; }
		}		
	}
	if(L>0){
		Canvas* C=dynamic_cast<Canvas*>(SD);
		if(C){
			C->L=0;
			L=C->GetWidth()*L/LMax/A;
			C->AddBar(0,0,L,C->GetHeight(),0xFF00FF00); //0xFF40CC20
			C->Visible=true;
		}
		ProgressBar* PB=dynamic_cast<ProgressBar*>(SD);
		if(PB){
			PB->Value=L;
			PB->MaxValue=LMax*A;
			PB->Visible=true;
		}
	}	
};
// cva_SP_id_Amount
void cva_SP_id_Amount::SetFrameState(SimpleDialog* SD){	
	SD->Visible=false;
	int id=SD->ID;
	ParentFrame* PF=(ParentFrame*)SD;
	if(GlobalID){
		SD->ID=GlobalSelPointID;
	}else{
		while(id>0&&PF){
			id--;
			PF=PF->ParentDS;
		}
	}
	vui_SelPoint* SP=OIS.GetSelPoint(PF);
	if(SP){
		_str Txt;
		switch(SP->Type){
			case ve_UT_cannon: {
				vui_CannInfo* I=&SP->Inf.Cannon;
				Txt.Add(I->ChargeStage);
				break; }
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				Txt.Add(I->NLiveMembers);
				break; }
			case ve_UT_units: {
				if(SP->Sort!=ve_UT_hero){
					vui_UnitInfo* I=&SP->Inf.Units;
					Txt.Add(I->Amount);
				}
				break; }
			case ve_UT_building: {
				vui_BuildInfo* I=&SP->Inf.Buildings;
				Txt.Add(I->Amount);
				break; }
		}
		if(Txt.pchar()){
			TextButton* T=dynamic_cast<TextButton*>(SD);
			if(T){
				T->SetMessage(Txt.pchar());
				T->Visible=true;
			}
			VitButton* VB=dynamic_cast<VitButton*>(SD);
			if(VB){
				VB->SetMessage(Txt.pchar());
				VB->Visible=true;
			}
		}
	}
}
// cva_SP_Patrol
void cva_SP_Patrol::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(VB&&SP&&OIS.SelPoint.GetAmount()==1){
		if(SP->Type==ve_UT_units&&!SP->Inf.Units.Peasant/*||SP->Type==ve_UT_brigade*/){
			SD->Visible=true;
			if(PATROLMODE){
				VB->State=1;
			}else{
				if(SP->Inf.Units.Patrol){
					VB->State=2;
				}else{
					VB->State=0;
				}				
			}
		}
	}
}
bool cva_SP_Patrol::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI){
		PATROLMODE=1;
	}	
	return true;
}
// cva_SP_Guard
void cva_SP_Guard::SetFrameState(SimpleDialog* SD){
	//SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		VitButton* VB=dynamic_cast<VitButton*>(SD);
		if( !VB && SD->DSS.GetAmount() )
			VB=dynamic_cast<VitButton*>(SD->DSS[0]);
		if(VB){
			if( SP->Type==ve_UT_units&&!SP->Inf.Units.Peasant || SP->Type==ve_UT_brigade ){
				//SD->Visible=true;
				if( GUARDMODE || SP->OB->Guard!=0xFFFF ) 
					VB->State=1;
				else 
					VB->State=0;
			}
		}
	}
}
bool cva_SP_Guard::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI){
		GUARDMODE=1;
	}
	return true;
}
// cva_SP_Activity0
void cva_SP_Activity0::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	vui_SelPoint* SP=OIS.GetLastSelPoint();	
	if(VB&&SP){
		switch(SP->Type){
			case ve_UT_cannon: {
				vui_CannInfo* I=&SP->Inf.Cannon;
				break; }
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				if(OIS.ActiveState&(1<<SD->ID)) VB->State=1;
				else VB->State=0;
				SD->Visible=true;
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&SP->Inf.Units;
				if(!I->Peasant){
					if(OIS.ActiveState&(1<<SD->ID)) VB->State=1;
					else VB->State=0;
					SD->Visible=true;
				}
				break; }
			case ve_UT_building: {
				vui_BuildInfo* I=&SP->Inf.Buildings;
				break; }
		}
		if(VB->Visible&&VB->State==1){
			if(VB->ID==0){
				VitCursorLowProirity=0;
			}else
				if(VB->ID==1){
					VitCursorLowProirity=10;
					GoAndAttackMode=false;
				}else
					if(VB->ID==2){
						VitCursorLowProirity=1;
						GoAndAttackMode=false;
					}
		}
	}	
}
bool cva_SP_Activity0::LeftClick(SimpleDialog* SD){	
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI){
		vui_IS_Sel_SetActivity SA;		
		SA.Data.NI=MyNation;
		SA.Data.ActivityState=SD->ID;
		SA.InterfAction();
		if(EngSettings.AutoChangeFormationType){
			if(SP){
				int n=OIS.SelPoint.GetAmount();
				for(int i=0;i<n;i++){
					vui_SelPoint* SP=OIS.SelPoint+i;
					if(SP&&SP->Type==ve_UT_brigade){
						vui_BrigInfo* I=&SP->Inf.Brigade;
						char ids[]={1,0,2};
						int ft=I->FormID[ids[SD->ID]];//0
						if(ft<0xFF){
							REFORMA(I->BrigID+(ft<<16));
							//vui_IS_Sel_MakeReformation MR;
							//MR.Data.NI=MyNation;
							//MR.Data.BrigadeID=I->BrigID;
							//MR.Data.FormType=ft;
						}						
					}
				}				
			}			
		}
	}	
	return true;
}
// cva_SP_Activity12
void cva_SP_Activity12::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(VB&&SP){
		SD->ID=0;
		switch(SP->Type){
			case ve_UT_cannon: {
				vui_CannInfo* I=&SP->Inf.Cannon;
				break; }
			case ve_UT_brigade: {
				vui_BrigInfo* I=&SP->Inf.Brigade;
				if(I->ActiveState==2||I->ActiveState==1) VB->State=1;
				else VB->State=0;
				SD->ID=I->ActiveState;
				SD->Visible=true;
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&SP->Inf.Units;
				if(!I->Peasant){
					if(I->ActiveState!=0) VB->State=1;
					else VB->State=0;
					SD->ID=I->ActiveState;
					SD->Visible=true;
				}
				break; }
			case ve_UT_building: {
				vui_BuildInfo* I=&SP->Inf.Buildings;
				break; }
		}
		if(SD->ID==1){
			VB->DSS[0]->Visible=false;
			VB->DSS[1]->Visible=true;
		}else
			if(SD->ID==2){
				VB->DSS[0]->Visible=true;
				VB->DSS[1]->Visible=false;
			}
	}
}
bool cva_SP_Activity12::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&SP->NI==NI){
		vui_IS_Sel_SetActivity SA;
		SA.Data.NI=MyNation;
		//SA.Data.ActivityState=2;
		if(SD->ID==1) SA.Data.ActivityState=2;
		else SA.Data.ActivityState=1;
		if(EngSettings.AutoChangeFormationType){			
			if(SP){
				int n=OIS.SelPoint.GetAmount();
				for(int i=0;i<n;i++){
					vui_SelPoint* SP=OIS.SelPoint+i;
					if(SP&&SP->Type==ve_UT_brigade){
						if(SD->ID==1){
							vui_BrigInfo* I=&SP->Inf.Brigade;
							int ft=I->FormID[2];
							if(ft<0xFF)REFORMA(I->BrigID+(ft<<16));
						}else{
							vui_BrigInfo* I=&SP->Inf.Brigade;
							int ft=I->FormID[0];
							if(ft<0xFF)REFORMA(I->BrigID+(ft<<16));
						}
					}
				}
			}
		}
		//if(SD->ID==1) SA.Data.ActivityState=2;
		//	else SA.Data.ActivityState=1;
		SA.InterfAction();
	}	
	return true;
}
// cva_MM_Profile
bool cva_MM_Profile::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){		
		//PlayerInterface();
	}
	return true;
}
// cva_MM_Campaign
bool cva_MM_Campaign::LeftClick(SimpleDialog* SD){
#ifndef _DEMO_
	if(v_MainMenu.Enable){		
		CPlayerSAVE	Pl;
		if(Pl.m_pPlayer.GetAmount()!=0/*||PlayerInterface()*/){
			vGameMode=gmCamp;
			//RestoreMarketPrice();//grey
			ProcessBigMap(0);
			//RestoreMarketPrice();//grey
			vGameMode=gmSin;
		}
	}
#endif _DEMO_
	return true;
}

void cva_CommonRoomParamDD::SetFrameState(SimpleDialog* SD){
	int N=GSets.RoomParams.GetNParams();
	if(N){
		SD->Visible=true;
		for(int i=0;i<SD->DSS.GetAmount();i++){
			SimpleDialog* D=SD->DSS[i];
			if(D){
				if(i<N){
					D->Visible=true;
					D->ID=i;
				}else{
					D->Visible=false;
				}
			}
		}
	}else{
		SD->Visible=false;
	}
}
void cva_CommonRoomParam::SetFrameState(SimpleDialog* SD){
	if(SD->DSS.GetAmount()>1){
		TextButton* tbName=(TextButton*)SD->DSS[0];
		if(tbName){
			char* nm=GSets.RoomParams.GetFieldName(SD->ID);
			tbName->SetMessage(GetTextByID(nm));
		}
		//ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		ComboBox* cbValue=dynamic_cast<ComboBox*>(SD->DSS[1]);
		if(cbValue){
			Enumerator* en=GSets.RoomParams.GetEnum(SD->ID);
			if(en){
				int n=en->GetAmount();
				if(cbValue->NLines!=n){
					cbValue->Clear();
					for(int i=0;i<n;i++){
						cbValue->AddLine(GetTextByID(en->GetStr(i)));
					}
					//CurLine=
				}
				if(v_MM_Host||vGameMode==gmSin){
					CurrentGameInfo* g=&GSets.CGame;
					PlayerInfo* I=g->GetCurrentPlayerInfo();
					cbValue->Enabled=( I && I->Ready==0 );
					GSets.RoomParams.SetFieldValue(SD->ID,cbValue->Lines[cbValue->CurLine]);
				}else{
					cbValue->Enabled=false;
					cbValue->Clear();
					cbValue->AddLine(GSets.RoomParams.GetFieldValue(SD->ID));
				}
			}else cbValue->Visible=false;
		}
		TextButton* tbValue=dynamic_cast<TextButton*>(SD->DSS[1]);
		if(tbValue){
			tbValue->SetMessage(GSets.RoomParams.GetFieldValue(SD->ID));
		}
	}
}

UnitActiveAbilityList* v_Targ_MA=NULL;
int v_Targ_MA_id;
int v_Targ_MA_id2;

int cva_ActAbl::curID=0;
int cva_ActAbl::curCategory=0;

// cva_ActAblDD
void cva_ActAblDD::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){

		int N=SP->GetActiveUnitAbilityAmount(Category);
		if(AblAmount>0)
			N=AblAmount;

		ListDesk* LD=dynamic_cast<ListDesk*>(SD);
		if(LD){
			int Need=N-LD->DSS.GetAmount();
			for(;Need>0;Need--){
				LD->AddElement(NULL);
			}
		}

		//for(int i=0;i<SD->DSS.GetAmount();i++){
		//	SimpleDialog* D=SD->DSS[i];
		//	if(D){
		//		D->Visible=true;
		//		D->ID=i;
		//		D->UserParam=Category;
		//	}else{
		//		D->Visible=false;
		//	}
		//}

		cva_ActAbl::curID=AblStartID;
		cva_ActAbl::curCategory=Category;

		if(HideIfEmpty)
			SD->Visible=(N>0);

	}
}
// cva_ActAbl

void DrawPercentGauge( float percent, const Rct& ext, DWORD clr );

cva_ActAbl::cva_ActAbl(){
	Lx=40;
	Ly=40;
	StsPressSpr=12;
	StsPressNSpr=2;
	StsActSpr=2;
	StsActNSpr=6;
};
void cva_ActAbl::OnDraw(SimpleDialog* SD){
	return;
}

int SpellHotKeyID=0;

void cva_ActAbl::SetFrameState(SimpleDialog* SD){

	SD->ID=curID++;
	SD->UserParam=curCategory;

	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if( SP && SP->OB->Ready /*&& DetectHero(SP->OB)*/ ){
		ActiveUnitAbility* UA=SP->GetActiveUnitAbility(SD->ID,SD->UserParam);
		if( true/*SP->NI==NI || UA&&UA->TypeAbil==0 || DetectMagazine(SP->OB) || DetectChest(SP->OB)*/ ){
			if(UA){
				SD->Visible=true;

				SD->AllowDrag=true;
				bool InShop=SP->OB->ActiveAbility->LibID!=0xFFFF;
				if( !InShop && ( SD->UserParam==adButtons ) || SD->UserParam==adPassive || SD->UserParam==adButtonAuras || SD->UserParam==adDialog || SD->UserParam==adLevelUps ){
					SD->AllowDrag=false;
				}

				// HotKeys
				if( SD->UserParam==adItems ){
					SD->HotKey=VK_F1+SD->ID;
					SD->HotCtrl=false;
					SD->HotShift=false;
					SD->HotAlt=false;
				}
				if( SD->UserParam==adPassive || SD->UserParam==adButtonAuras || SD->UserParam==adButtons ){
					SD->HotKey=VK_F1+SpellHotKeyID++;
					SD->HotCtrl=true;
					SD->HotShift=false;
					SD->HotAlt=false;
				}

				if(SD->MouseOver){
					UA->ShowRadius();
				}

				//GP->Enabled=SD->ID<5||SD->UserParam!=1;

				int f,s,fchr,finv;
				//_str h;

				OneObject* AOB=UA->OB;
				if( SD->UserParam==adLibShop ){
					UA->OB=SP->OB;
				}
				UA->GetButton(f,s,fchr,finv);
				UA->OB=AOB;

				int cooldown=UA->GetCoolDownProc();

				int ff=f;
				UnitAbility* U=UA->GetA();
				if(U){
					int ad=SD->UserParam;
					if( ad>=adHelm && ad<=adWeapon ){
						f=fchr;
					}
					if( ad==adItemsTakeOff || ad==adItems || ad==adBasketBottles || ad==adLibShop && UA->TypeAbil!=1 )
						f=finv;
				}

				if(UA->TypeAbil!=7){ // для диалоговой абилки не менять кнопку
					//SimpleDialog* Picture=SD->Find(Pic.pchar());
					SimpleDialog* Picture=Pic.Get();
					if(!Picture){
						Picture=SD;
					}
					Picture->Visible=true;
					if( Lx!=0 && Ly!=0 ){
						int W=GPS.GetGPWidth(f,s);
						int H=GPS.GetGPHeight(f,s);
						if( W!=Lx || H!=Ly ){
							Picture->EnableTransform=true;
							Picture->ParentFrame::ScaleX=(float)Lx/(float)W;
							Picture->ParentFrame::ScaleY=(float)Ly/(float)H;
						}
					}else{
						Picture->EnableTransform=false;
					}
					GPPicture* GP=dynamic_cast<GPPicture*>(Picture);
					if(GP){
						GP->FileID=f;
						GP->SetSpriteID(s);
					}else{
						VitButton* VB=dynamic_cast<VitButton*>(Picture);
						if(VB){
							VB->GP_File=f;
							VB->State=0;
							VB->SpritePassive[0]=s;
							VB->SpriteOver[0]=s;
						}
					}
					//if( cooldown>0 && cooldown<100 ){
					//	Picture->Diffuse=0xAAFFFFFF;
					//}else{
					//	Picture->Diffuse=0xFFFFFFFF;
					//}
					Picture->Diffuse=0xFFFFFFFF;

					// недостаточно денег, чтобы купить
					if( SD->UserParam==adLibShop ){
						OneObject* Lib=Group[SP->OB->ActiveAbility->LibID];
						if(Lib&&Lib->Serial==SP->OB->ActiveAbility->LibSN){
							// buy
							int res=XRESRC(NI,MoneyID);
							int GetAbilityBuyPrice(OneObject* Buyer, ActiveUnitAbility* AA);
							int price = GetAbilityBuyPrice(SP->OB,UA);
							if( res<price ){
								Picture->Diffuse=0xFFCC2020;
							}
						}
					}

					if( SP->OB->newMons->NewParams.AntiMage && U && U->Type==7 && UA->TypeAbil==1 ){
						if(cooldown>0){
							Picture->Diffuse=0xFFCC2020;
						}
					}

				}
				//f=ff;


				// количество баночек
				int a=0;
				if( SP->OB->newMons->NewParams.AntiMage && U && U->Type==7 ){
					//if( UA->TypeAbil==2 )
						a=UA->Level;
				}else if( UA->TypeAbil!=4 && UA->Clothed && DetectHero(SP->OB) ){
					UnitAbility* uA=UA->GetA();
					if(uA){
						if( uA->SellAfterUse || !uA->UnlimitedAmount ){
							a=1;
						}else if( UA->TypeAbil==1 ){
							a=UA->Amount;
						}else if( UA->TypeAbil==5 ){
							itr_AA.Create(SP->OB);
							while(ActiveUnitAbility* aa=itr_AA.Next()){
								if( aa->TypeAbil==UA->TypeAbil && /*!aa->Clothed &&*/ uA==aa->GetA() )
									a++;
							}
						}
					}
				}
				VitButton* vbBtl=dynamic_cast<VitButton*>(SD->Find(517/*"Amount"*/));
				if( vbBtl ){
					if(a>0){	
						static DString txt;
						txt=a;
						vbBtl->SetMessage(txt.pchar());
						vbBtl->Visible=true;
					}else
						vbBtl->Visible=false;
				}

				// перезарядка
				ProgressBar* PB=CoolDown.Get();

				// by Xeno: draw a red card if not enough mana
				//Modified by FlameMaster: isSilence() returns void and was not a boolean
				bool redCard = false;
				if(UA)
				{
					NewAbility* NA = (NewAbility*)UA->GetA();
					if (NA && (UA->OB->Mana < NA->ManaValue || (NA->ManaValue>0 && UA->OB->ActiveAbility->ActiveAbilities.InfluenceMask.check_bit(ABL_Silence)))) redCard = true;
				}

				if(PB)
				{
					PB->MaxValue=100;
					if(cooldown==0){
						PB->Value=0;
						PB->Visible=false;
					}else{
						if(cooldown==100){
							PB->Value=100;
							PB->ProgressColor=0xFFDD0000;
						}else{
							PB->Value=100-cooldown;
							PB->ProgressColor=0xFF2020EE;
						}
						PB->Visible=true;
					}
					if(UA->TypeAbil==0){
						PB->Value=100-cooldown;
						PB->ProgressColor=0xBB2020EE;
					}else{
						PB->Type=1;//0;
						PB->GaugeType=0;//1;
						PB->ProgressType=0;//1;
						PB->BackGP_File=f;
						PB->BackSprite=s;
						PB->BackColor=0;
						PB->ProgressGP_File=f;
						PB->ProgressSprite=s;
						PB->Diffuse=0xB2FF2222;//0xFFFFFFFF;
					}
					if (redCard)
					{
						PB->Value=0;
						PB->Visible=true;
						PB->ProgressColor=0xFF2020EE;
					}
				}

				// Status of button
				GPPicture* gpStatus=Status.Get();
				if(gpStatus){
					ActiveUnitAbility* AA=UA;
					UnitAbility* UA=AA->GetA();
					if(UA){
						//static int f=GPS.PreLoadGPImage("interf3\\ButAbi");
						int s=3; // 0 - active, 1 - press need target, 2 - magic card, 3 - aura
						if(UA->Aura){
							if(AA->AutoCast||!UA->Button){
								s=0;
							}
						}else{
							s=2;
							if (SP->ActAbl==v_Targ_MA && AA->Serial==v_Targ_MA_id2) s=1;
							Order1* OR=SP->OB->LocalOrder;
							if (OR && (OR->DoLink==&PrepareSpellLink||OR->DoLink==&CastingSpellLink) && OR->info.Spell.Serial==AA->Serial) {
								s=0;
							}
						}
						//gpStatus->SpriteID=s;
						//float sx=1.0;
						//float sy=1.0;
						//int W=GPS.GetGPWidth(f,0);
						//int H=GPS.GetGPHeight(f,0);
						//if(W!=Lx||H!=Ly){
						//	sx=(float)Lx/(float)W;
						//	sy=(float)Ly/(float)H;
						//}
						//gpStatus->EnableTransform=true;
						//gpStatus->ScaleX=sx;
						//gpStatus->ScaleY=sy;
						int sid,nspr;
						bool active=false;
						if(s==0){
							active=true;
							sid=StsActSpr;
							nspr=StsActNSpr;
						}else
							if(s==1){
								active=true;
								sid=StsPressSpr;
								nspr=StsPressNSpr;
							}
							if(active){
								gpStatus->Visible=true;								
								int spr=(GetTickCount()/80)%nspr;
								gpStatus->SetSpriteID(sid+spr);
							}else{
								gpStatus->Visible=false;
							}
					}
				}

				// Empty
				GPPicture* gpEmp=Empty.Get();
				if(gpEmp){
					gpEmp->Visible=false;
				}
				//Color
				SimpleDialog* sdColor=Color.Get();
				if(sdColor){
				 	sdColor->Visible=false;
					SimpleDialog* EDrag=SimpleDialog::CallParams.Dragged;
					if( EDrag && SD!=EDrag ){
						ActiveUnitAbility* aaDrag=SP->GetActiveUnitAbility(EDrag->ID,EDrag->UserParam);
						if( aaDrag ){
							UnitAbility* uaDrag=aaDrag->GetA();
							if(uaDrag){
								if( aaDrag->TypeAbil==4 && (SD->UserParam==uaDrag->TypeClothe+adHelm || SD->UserParam==adItemsTakeOff) ||
									aaDrag->TypeAbil==5 && (SD->UserParam==adItems || SD->UserParam==adBasketBottles)){
										sdColor->Visible=true;
										sdColor->Diffuse&=0xFFFFFF;
										byte A=SD->MouseOver?0x77:0x33;
										sdColor->Diffuse+=A<<24;
									}
							}
						}
					}
				}
				SD->CheckMouseOver(mouseX,mouseY);
				if( SD->MouseOver ){
					OneObject* AOB=UA->OB;
					if( SD->UserParam==adLibShop ){
						UA->OB=SP->OB;
					}
					extern SimpleDialog* sdAblHotKey;
					sdAblHotKey=SD;
					const char* hn=UA->GetHint();
					SD->SetHint(hn,false);
					SD->HintWithHotKey=true;
					sdAblHotKey=NULL;
					UA->OB=AOB;
				}
			}else{
				int n=SP->GetAbilitySlotAmount(SD->UserParam);
				if( SD->ID<n && DetectHero(SP->OB) ){
					// Empty
					SD->Visible=true;
					SD->AllowDrag=false;
					SimpleDialog* Picture=Pic.Get();
					if(Picture){
						Picture->Visible=false;
					}
					GPPicture* gpEmp=Empty.Get();
					if(gpEmp){
						gpEmp->Visible=true;
					}
					//Color
					SimpleDialog* sdColor=Color.Get();
					if(sdColor){
						sdColor->Visible=false;
						SimpleDialog* EDrag=SimpleDialog::CallParams.Dragged;
						if( EDrag && SD!=EDrag ){
							ActiveUnitAbility* aaDrag=SP->GetActiveUnitAbility(EDrag->ID,EDrag->UserParam);
							if( aaDrag ){
								UnitAbility* uaDrag=aaDrag->GetA();
								if(uaDrag){
									if( aaDrag->TypeAbil==4 && (SD->UserParam==uaDrag->TypeClothe+adHelm || SD->UserParam==adItemsTakeOff) ||
										aaDrag->TypeAbil==5 && (SD->UserParam==adItems || SD->UserParam==adBasketBottles)){
											sdColor->Visible=true;
											sdColor->Diffuse&=0xFFFFFF;
											byte A=SD->MouseOver?0x77:0x33;
											sdColor->Diffuse+=A<<24;
										}
								}
							}
						}
					}
					// количество баночек
					VitButton* vbBtl=dynamic_cast<VitButton*>(SD->Find(517/*"Amount"*/));
					if(vbBtl)
						vbBtl->Visible=false;
					SD->SetHint("");
					//SD->SetHint("Slot");
				}else{
					// Spares
					SD->Visible=false;
				}
			}
		}
	}
}
extern bool NOPAUSE;
bool cva_ActAbl::LeftClick(SimpleDialog* SD){
	if (NOPAUSE) {
	//if(SD->ConnectActionToBaseFunction.GetAmount()){
	//	return true;
	//}
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if( SP && SP->NI==NI && SP->OB->Ready ){
		UnitActiveAbilityList* MA=SP->ActAbl;
		ActiveUnitAbility* AA=SP->GetActiveUnitAbility(SD->ID,SD->UserParam);
		if( AA && AA->TypeAbil!=0 && !AA->Deleted ){

			UnitAbility* UA=AA->GetA();
			int Index=-1;
			if(UA){
				Index=UA->Index;
			}
			
			bool GetHero(OneObject* OB);
			bool hero=GetHero(SP->OB);

			if( hero&&AA->Clothed || !(hero||AA->Clothed) || AA->TypeAbil!=1 || SD->UserParam==adLibShop || UA&&UA->Aura 
				|| UA && UA->Type==7 && AA->TypeAbil==1 && AA->OB->newMons->NewParams.AntiMage ){

				if (SP->OB->ActiveAbility && SP->OB->ActiveAbility->LibID!=0xFFFF){
					OneObject* Lib=Group[SP->OB->ActiveAbility->LibID];
					if(Lib&&Lib->Serial==SP->OB->ActiveAbility->LibSN){

						if(SD->UserParam==adLibShop){
							// buy
							int res=XRESRC(NI,MoneyID);
							int GetAbilityBuyPrice(OneObject* Buyer, ActiveUnitAbility* AA);
							int price = GetAbilityBuyPrice(SP->OB,AA);
							if( price>0 && price<=res ){
								vui_IS_AbilityTrade AT(Lib,SP->OB,AA,NI,-price);
								AT.InterfAction();
								//AddXRESRC(NI,GoldID,-price);
								//AddAbility(UA->GetA(),SP->OB,1);
								//UA->Deleted=true;
							}
							return true;
						}else
							if(SD->UserParam==adLibHero){
								//sell
								int GetAbilitySellPrice(OneObject* Buyer, ActiveUnitAbility* AA);
								int price = GetAbilitySellPrice(SP->OB,AA);
								vui_IS_AbilityTrade AT(SP->OB,Lib,AA,NI,price);
								AT.InterfAction();
								//AddXRESRC(NI,GoldID,price);
								//AddAbility(UA->GetA(),Lib,1);
								//UA->Deleted=true;
								return true;
							}
					}
				}


				if (SP->NI==NI && AA->CanTarget(0xFFFF,-1,-1,-1)) {
					if(AA->EnableStates){
						v_Targ_MA=MA;
						v_Targ_MA_id=Index;
						v_Targ_MA_id2=AA->Serial;

						AA->evLeftClick=true;
						return true;
					}
					if(AA->isTarget()){
						v_Targ_MA=MA;
						v_Targ_MA_id=Index;
						v_Targ_MA_id2=AA->Serial;
					}else{
						if( UA && UA->AntiMageSupportSkill && AA->TypeAbil==2 )
							return true;
						vui_IS_Abl_Click IS;
						IS.Data.NI=MyNation;
						IS.Data.NIndex=SP->NIndex;
						IS.Data.AblID=Index;
						IS.Data.AblSerial=AA->Serial;
						IS.Data.Right=false;
						//
						IS.InterfAction();
					}
				}
			}

		}
	}	
	return true;
	}
}
bool cva_ActAbl::RightClick(SimpleDialog* SD){
	//if(SD->ConnectActionToBaseFunction.GetAmount()){
	//	return true;
	//}
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if( SP && SP->NI==NI ){
		UnitActiveAbilityList* MA=SP->ActAbl;
		ActiveUnitAbility* AA=SP->GetActiveUnitAbility(SD->ID,SD->UserParam);
		if( AA && AA->TypeAbil!=0 /*&& AA->Clothed*/ ){
			if(AA->EnableStates){
				AA->evRightClick=true;
				return true;
			}

			UnitAbility* UA=AA->GetA();
			int Index=-1;
			if(UA){
				Index=UA->Index;
			}

			vui_IS_Abl_Click IS;
			IS.Data.NI=MyNation;
			IS.Data.NIndex=SP->NIndex;
			IS.Data.AblID=Index;
			IS.Data.AblSerial=AA->Serial;
			IS.Data.Right=true;
			IS.InterfAction();
		}
	}	
	return true;
}
//
extern char* v_MessBmp;
extern char* v_MessText;
// cva_GM_MessPic
void cva_GM_MessPic::SetFrameState(SimpleDialog* SD){	
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP){
		static _str bmp;
		if(v_MessBmp&&bmp!=v_MessBmp){
			GP->Visible=false;
			bmp=v_MessBmp;
			if(bmp!="NONE"){
				char fn[128];
				int spr;
				sscanf(bmp.pchar(),"%s%d",fn,&spr);
				if(spr!=-1){
					GP->FileID=GPS.PreLoadGPImage(fn);
					GP->SetSpriteID(spr);
					GP->Visible=true;
				};
			}
		}
	}
}
// cva_GM_MessTxt
void cva_GM_MessTxt::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		static _str txt;
		if(txt.isClear()||strcmp(txt.pchar(),v_MessText)){
			TB->SetMessage(&v_MessText);
			txt=v_MessText;
			BaseClass* BC=SD->GetParent();
			if(BC){
				DialogsDesk* DD=dynamic_cast<DialogsDesk*>(BC->GetParent());
				if(DD&&DD->VScroller){
					DD->VScroller->SPos=0;
				}
			}
		}
	}
}
// cva_Opt_VMode
void cva_Opt_VMode::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){
		if(!SD->vm_Init){
			SD->vm_Init=true;
			CB->Clear();
			for(int i=0;i<NModes;i++){
				char cc[30];
				sprintf(cc,"%dx%d",ModeLX[i],ModeLY[i]);
				CB->AddLine(cc);
				if(GSets.SVOpt.ScreenSizeX==ModeLX[i]&&GSets.SVOpt.ScreenSizeY==ModeLY[i])CB->CurLine=i;
			};
		}	
	}
};
void cva_Opt_VMode::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){
			GSets.SVOpt.ScreenSizeX=ModeLX[CB->CurLine];
			GSets.SVOpt.ScreenSizeY=ModeLY[CB->CurLine];
			if(v_MainMenu.GameMode){
				if(RealLx!=ModeLX[CB->CurLine]||RealLy!=ModeLY[CB->CurLine]){
					SetGameDisplayModeAnyway(ModeLX[CB->CurLine],ModeLY[CB->CurLine]);
				}
			}
		}
	}
};
// cva_MM_Accept
bool cva_MM_Accept::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		ParentFrame* pf=SD;
		while(pf->ParentDS) pf=(ParentFrame*)pf->ParentDS;
		pf->vm_ActionsAccept();
		GSets.Save();
	}	
	return cva_MM_Close::LeftClick(SD);
}
// cva_MM_CancelInDesk
bool cva_MM_CancelInDesk::LeftClick(SimpleDialog* SD){	
	DialogsDesk* D=Desk.Get();
	if(D) D->vm_ActionsCancel();
	return true;
}
// cva_MM_AcceptInDesk
bool cva_MM_AcceptInDesk::LeftClick(SimpleDialog* SD){
	DialogsDesk* D=Desk.Get();
	if(D) D->vm_ActionsAccept();
	return true;
}
// cva_MM_FilesList
void cva_MM_FilesList::SetFrameState(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		// create list
		if(LD->DSS.GetAmount()==0){
			_str mask;
			if(!Folder.isClear()){				
				if(Folder.pchar()[strlen(Folder.pchar())-1]!='\\') Folder+="\\";
				mask=Folder;
			}
			mask+=Ext;
			TGSCFindInfo* FD=GSFILES.gFindFirst(mask.pchar());
			if(FD){
				do {
					LD->AddElement(FD->m_FileName);
				} while(GSFILES.gFindNext(FD));
				//delete(FD);
			};
		}
		InputBox* IB=ibName.Get();
		if(IB){
			IB->StrMaxLen=32;
			IB->_Str=&FileName;
			IB->Str=NULL;
		}
		if(LD->CurrentElement<0){
			FileName="";
		}else{
			FileName=LD->GetElement(LD->CurrentElement);
		}
	}
	// убрать по-любому
	NPlayers=1;
}
// cva_MM_Run_MissionM3D
void cva_MM_Run_MissionM3D::Accept(SimpleDialog* SD){
	_str fn;
	fn=Folder;	
	fn+=FileName;
	//ShowLoading();
	strcpy(GSets.CGame.cgi_CurrentMap,fn.pchar());
	CreateNationalMaskForMap(GSets.CGame.cgi_CurrentMap);
	PrepareGameMedia(0,0,1);
	InitGame();
	/*
	extern int StartGTime;
	extern int StartVCond;
	StartGTime=0;
	StartVCond=0;
	MISSLIST.CurrentMission=n;
	SCENINF.Load(MISSLIST.MISS[n].DLLPath,MISSLIST.MISS[n].Description);
	UnPress();
	*/
	int n=0, Diff=0;
	RunMethod=0x12340000+n+(Diff<<8);
	//
	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* I=g->PL_INFO+0;
	int AI=(I->ColorID==0)?1:0;
	I->ColorID=(AI==0)?1:0;
	SetMyNation(I->ColorID);
	StartAIEx(AI,"ai\\mak.ai.xml",0,0,0,0);
	//	
	//inline CBEDriveMode_ST* DriveMode();
	DriveMode()->StartMission();
	GSets.CGame.UnitTransform=false;
	//
	PlayGame();
}
// cva_MM_Cancel
bool cva_MM_Cancel::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable&&SD){
		ParentFrame* pf=SD;
		while(pf->ParentDS) pf=(ParentFrame*)pf->ParentDS;		
		pf->vm_ActionsCancel();
	}
	return cva_MM_Close::LeftClick(SD);
}
// cva_Opt_Options
void cva_Opt_Options::SetFrameState(SimpleDialog* SD){
#ifdef _DEMO_
	ComboBox* cbAQ=AnimationQuality.Get();
	if(cbAQ){
#ifdef _PRESSMAN_
		cbAQ->CurLine=0;
#else
		cbAQ->CurLine=1;
#endif _PRESSMAN_
	}
#endif _DEMO_
	ComboBox* cbSQ=ShadowQuality.Get();
	if(cbSQ){
		extern int Shw;
		Shw=cbSQ->CurLine;
	}
	vmIGP.Opt.Process();
}
void cva_Opt_Options::Init(SimpleDialog* SD){
	CheckBox* SH=ShowHint.Get();
	if(SH){
		SH->State=!GSets.DisableHint;
	}
	CheckBox* SV=ShowVideo.Get();
	if(SV){
		SV->State=!GSets.DisableVideo;
	}
	CheckBox* FF=FriendlyFire.Get();
	if(FF){
		FF->State=!GSets.DisableFriendlyFire;
	}
	ComboBox* cbSQ=ShadowQuality.Get();
	if(cbSQ){
		static char* tSQ0=GetTextByID("#Options_Quality_High");
		static char* tSQ1=GetTextByID("#Options_Quality_Medium");
		static char* tSQ2=GetTextByID("#Options_Quality_Low");
		cbSQ->Clear(); cbSQ->AddLine(tSQ0); cbSQ->AddLine(tSQ1); cbSQ->AddLine(tSQ2);
		if(GSets.ShadowQuality<0||GSets.ShadowQuality>2){
			GSets.ShadowQuality=0;
		}
		cbSQ->CurLine=GSets.ShadowQuality;
	}
	ComboBox* cbAQ=AnimationQuality.Get();
	if(cbAQ){
		cbAQ->Clear();
		static char* tAQ0=GetTextByID("#MO_AnimationQuality0");
		static char* tAQ1=GetTextByID("#MO_AnimationQuality1");
#ifdef _DEMO_
		_str name;
#ifdef _PRESSMAN_
		cbAQ->AddLine(tAQ0);
		name.print("{CR}%s {C}",tAQ1); cbAQ->AddLine(name.pchar());
#else
		name.print("{CR}%s {C}",tAQ0); cbAQ->AddLine(name.pchar());
		cbAQ->AddLine(tAQ1);
#endif _PRESSMAN_
#else
		cbAQ->AddLine(tAQ0);
		cbAQ->AddLine(tAQ1);
#endif _DEMO_
		cbAQ->CurLine=GSets.AnimQuality;
	}
}
void cva_Opt_Options::Accept(SimpleDialog* SD){
	CheckBox* SH=ShowHint.Get();
	if(SH){
		GSets.DisableHint=!SH->State;
	}
	CheckBox* SV=ShowVideo.Get();
	if(SV){
		GSets.DisableVideo=!SV->State;
	}
	CheckBox* FF=FriendlyFire.Get();
	if(FF){
		GSets.DisableFriendlyFire=!FF->State;
	}
	ComboBox* cbSQ=ShadowQuality.Get();
	if(cbSQ){
		GSets.ShadowQuality=cbSQ->CurLine;
	}
	ComboBox* cbAQ=AnimationQuality.Get();
	if(cbAQ){
		GSets.AnimQuality=cbAQ->CurLine;
	}
}
// cva_MM_GoldenTextXY
void cva_MM_GoldenTextXY::SetFrameState(SimpleDialog* SD){
	SD->Setx(996-SD->GetWidth());
	SD->Sety(22);
}
// cva_Opt_ArcadeMode
void cva_Opt_ArcadeMode::Init(SimpleDialog* SD){	
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		CB->State=GSets.CGame.ArcadeMode;
	}
	ComboBox* Comb=dynamic_cast<ComboBox*>(SD);
	if(Comb){
		Comb->Clear();
		Comb->AddLine(GetTextByID("#ArcadeOff"));
		Comb->AddLine(GetTextByID("#ArcadeOn"));
		Comb->CurLine=GSets.CGame.ArcadeMode;
	}
}
void cva_Opt_ArcadeMode::Accept(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		GSets.CGame.ArcadeMode=CB->State;
	}
	ComboBox* Comb=dynamic_cast<ComboBox*>(SD);
	if(Comb){
		GSets.CGame.ArcadeMode=Comb->CurLine;
	}
}
void cva_Opt_ArcadeMode::SetFrameState(SimpleDialog* SD){
	if(!Name.isClear()){
		CheckBox* CB=dynamic_cast<CheckBox*>(SD);
		if(CB){
			if(CB->State&&!GSets.CGame.ArcadeMode){			
				v_MainMenu.SetModalDesk(Name.pchar());
			}
			GSets.CGame.ArcadeMode=CB->State;
		}
		ComboBox* Comb=dynamic_cast<ComboBox*>(SD);
		if(Comb){
			if(Comb->CurLine&&!GSets.CGame.ArcadeMode){			
				v_MainMenu.SetModalDesk(Name.pchar());
			}
			GSets.CGame.ArcadeMode=Comb->CurLine;
		}
	}
}
//
DialogsDesk vArcadeModeDD,vArcadeModeMessDD;
void ArcadeModeDDLoad(){
	vArcadeModeDD.ReadFromFile("dialogs\\interface\\ArcadaMode.DialogsDesk.Dialogs.xml");
	vArcadeModeMessDD.ReadFromFile("dialogs\\interface\\ArcadaModeMessage.DialogsDesk.Dialogs.xml");
}
DialogsDesk* addArcadeMode(DialogsSystem* DS, int x, int y){
	DialogsDesk* DD=new DialogsDesk;
	vArcadeModeDD.Copy(DD);
	DD->Setx(x);
	DD->Sety(y);
	DS->AddDialog(DD);
	DialogsDesk* DD1=new DialogsDesk;
	vArcadeModeMessDD.Copy(DD1);
	DS->AddDialog(DD1);
	return DD;
}
// cva_Opt_Subtitres
void cva_Opt_Subtitres::Init(SimpleDialog* SD){	
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		CB->State=GSets.EnableSubtitres;
	}
}
void cva_Opt_Subtitres::Accept(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		GSets.EnableSubtitres=CB->State;
	}
}
//cva_Opt_AnimQuality
void cva_Opt_AnimQuality::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){
		if(!SD->vm_Init){
			SD->vm_Init=true;
			CB->Clear();
			CB->AddLine(GetTextByID("#GS_AnimQ0"));
			CB->AddLine(GetTextByID("#GS_AnimQ1"));
			CB->AddLine(GetTextByID("#GS_AnimQ2"));
			CB->CurLine=2;
			if(GSets.AnimQuality<3) CB->CurLine=GSets.AnimQuality;
		}	
	}
};
void cva_Opt_AnimQuality::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){	
			GSets.AnimQuality=CB->CurLine;
		}
	}
};
//cva_Opt_SoundVolume
void cva_Opt_SoundVolume::Init(SimpleDialog* SD){
	VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
	if(SB){
		if(!SD->vm_Init){
			SD->vm_Init=true;
			SB->SPos=SB->SMaxPos;
			if(GSets.SVOpt.SoundVolume<SB->SMaxPos) SB->SPos=GSets.SVOpt.SoundVolume;
		}	
	}
	static int pos=0;
	if(pos!=SB->SPos){
		pos=SB->SPos;
		PlayEffect(GetSound("INTERF_GOLOSVOL"),0,(SB->SPos-GSets.SVOpt.SoundVolume)*40);
	}	
};
void cva_Opt_SoundVolume::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
		if(SB){	
			GSets.SVOpt.SoundVolume=SB->SPos;
		}
	}
};
//cva_Opt_MusicVolume
CIMPORT void SetCDVolume(int Vol);
//
void cva_Opt_MusicVolume::Init(SimpleDialog* SD){
	VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
	if(SB){
		SB->SPos=SB->SMaxPos;
		if(GSets.SVOpt.MusicVolume<SB->SMaxPos) SB->SPos=GSets.SVOpt.MusicVolume;
	}	
};
void cva_Opt_MusicVolume::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
		if(SB){	
			GSets.SVOpt.MusicVolume=SB->SPos;
		}
	}
};
extern bool EnableRestoreMusic;
void cva_Opt_MusicVolume::SetFrameState(SimpleDialog* SD){
	VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
	if(SB){
		EnableRestoreMusic=false;
		SetCDVolume(SB->SPos);		
	}
}
//cva_Opt_RequiredMsPerFrame_Scr
void cva_Opt_RequiredMsPerFrame_Scr::Init(SimpleDialog* SD){
	VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
	if(SB){
		if(!SD->vm_Init){
			SD->vm_Init=true;
			SB->SPos=SB->GetMaxPos()-GSets.SVOpt.RequiredMsPerFrame;
			if(SB->SPos<0) SB->SPos=0;
		}	
	}
};
void cva_Opt_RequiredMsPerFrame_Scr::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
		if(SB){	
			GSets.SVOpt.RequiredMsPerFrame=SB->GetMaxPos()-SB->SPos;
			if(GSets.SVOpt.RequiredMsPerFrame<1) GSets.SVOpt.RequiredMsPerFrame=1;
		}
	}
};
//cva_Opt_RequiredMsPerFrame_Dig
void cva_Opt_RequiredMsPerFrame_Dig::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&SD->ParentDS){
		VScrollBar* SB=dynamic_cast<VScrollBar*>(SD->ParentDS);
		if(SB){
			DString txt;
			txt=SB->SPos;//==0?SB->GetMaxPos():SB->GetMaxPos()/SB->SPos;
			TB->SetMessage(txt.pchar());
		}
	}
}
//cva_Opt_ScrollSpeed
void cva_Opt_ScrollSpeed::Init(SimpleDialog* SD){
	VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
	if(SB){
		if(!SD->vm_Init){
			SD->vm_Init=true;
			SB->SPos=SB->SMaxPos;
			if(GSets.SVOpt.ScrollSpeed<=SB->SMaxPos) SB->SPos=GSets.SVOpt.ScrollSpeed-1;
			if(SB->SPos<0)SB->SPos=0;
		}	
	}
};
void cva_Opt_ScrollSpeed::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
		if(SB){	
			GSets.SVOpt.ScrollSpeed=SB->SPos+1;
		}
	}
};
//cva_Opt_BrightSpeed -- Added by Xeno
void cva_Opt_PEBrightSpeed::Init(SimpleDialog* SD){
	VScrollBar* SB = dynamic_cast<VScrollBar*>(SD);
	if(SB)
	{
		if(!SD->vm_Init)
		{
			SD->vm_Init = true;
			SB->SPos = SB->SMaxPos;
			if(GSets.SVOpt.AutoBrightAdaptSpeed <= SB->SMaxPos) SB->SPos = GSets.SVOpt.AutoBrightAdaptSpeed-1;
			if(SB->SPos < 0) SB->SPos = 0;
		}	
	}
};
void cva_Opt_PEBrightSpeed::Accept(SimpleDialog* SD){
	if(SD->vm_Init)
	{
		VScrollBar* SB = dynamic_cast<VScrollBar*>(SD);
		if(SB)
		{	
			GSets.SVOpt.AutoBrightAdaptSpeed = SB->SPos+1;
		}
	}
};
//cva_Opt_BrightDegree -- Added by Xeno
void cva_Opt_PEBrightDegree::Init(SimpleDialog* SD){
	VScrollBar* SB = dynamic_cast<VScrollBar*>(SD);
	if(SB)
	{
		if(!SD->vm_Init)
		{
			SD->vm_Init = true;
			SB->SPos = SB->SMaxPos;
			if(GSets.SVOpt.AutoBrightDegree <= SB->SMaxPos) SB->SPos = GSets.SVOpt.AutoBrightDegree-1;
			if(SB->SPos < 0) SB->SPos = 0;
		}	
	}
};
void cva_Opt_PEBrightDegree::Accept(SimpleDialog* SD){
	if(SD->vm_Init)
	{
		VScrollBar* SB = dynamic_cast<VScrollBar*>(SD);
		if(SB)
		{	
			GSets.SVOpt.AutoBrightDegree = SB->SPos+1;
		}
	}
};
//cva_Opt_MotionBlur -- Added by Xeno
void cva_Opt_PEMotionBlur::Init(SimpleDialog* SD){
	VScrollBar* SB = dynamic_cast<VScrollBar*>(SD);
	if(SB)
	{
		if(!SD->vm_Init)
		{
			SD->vm_Init = true;
			SB->SPos = SB->SMaxPos;
			if(GSets.SVOpt.MotionBlurAlpha <= SB->SMaxPos) SB->SPos = GSets.SVOpt.MotionBlurAlpha-1;
			if(SB->SPos < 0) SB->SPos = 0;
		}	
	}
};
void cva_Opt_PEMotionBlur::Accept(SimpleDialog* SD){
	if(SD->vm_Init)
	{
		VScrollBar* SB = dynamic_cast<VScrollBar*>(SD);
		if(SB)
		{	
			GSets.SVOpt.MotionBlurAlpha = SB->SPos+1;
		}
	}
};
//cva_Opt_MusicPlayMode
void cva_Opt_MusicPlayMode::Init(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		CB->State=GSets.SVOpt.MusicPlayMode;			
	}
};
extern bool RandomMode;
bool GetMusicMode(){	
	return RandomMode;
}
void SetMusicMode(bool State){
	if(RandomMode!=State){		
		if(!State){
			StopPlayCD();
		}else{
			if(isFinishedMP3()) PlayRandomTrack();
		}
		//RandomMode=State;
	}
}
void cva_Opt_MusicPlayMode::SetFrameState(SimpleDialog* SD){
	//static bool State=false;
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		EnableRestoreMusic=false;
		SetMusicMode(CB->State);
	}
}
void cva_Opt_MusicPlayMode::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		CheckBox* CB=dynamic_cast<CheckBox*>(SD);
		if(CB){	
			GSets.SVOpt.MusicPlayMode=CB->State;
		}
	}
};
// cva_Opt_ReflectedTerrain
void cva_Opt_ReflectedTerrain::Init(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		CB->State=GSets.SVOpt.DrawReflectedTerrain;
	}
};
void cva_Opt_ReflectedTerrain::Accept(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		GSets.SVOpt.DrawReflectedTerrain=CB->State;
	}
};
void cva_Opt_ReflectedTerrain::SetFrameState(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		extern bool Rf;
		Rf=CB->State;
	}
};
// cva_Opt_EnablePostEffects - added by Xeno
void cva_Opt_PostEffects::Init(SimpleDialog* SD)
{
	CheckBox* CB = dynamic_cast<CheckBox*>(SD);
	if(CB) CB->State = GSets.SVOpt.EnablePostEffects;
};
void cva_Opt_PostEffects::Accept(SimpleDialog* SD)
{
	CheckBox* CB = dynamic_cast<CheckBox*>(SD);
	if(CB) GSets.SVOpt.EnablePostEffects = CB->State;
};
void cva_Opt_PostEffects::SetFrameState(SimpleDialog* SD)
{
	CheckBox* CB = dynamic_cast<CheckBox*>(SD);
	if(CB)
	{
		extern bool Epe;
		Epe = CB->State;
	}
};
// cva_Opt_SelectionType
void cva_Opt_SelectionType::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){			
		CB->Clear();
		Enumerator* E=ENUM.Get("UNITS_SEL_TYPE");
		if(E){
			for(int i=0;i<E->GetAmount();i++){
				CB->AddLine(GetTextByID(E->GetStr(i)));
			}
		}
		//CB->CurLine=2;
		//if(GSets.AnimQuality<3) 
		CB->CurLine=GSets.SVOpt.SelectionType;
	}
};
void cva_Opt_SelectionType::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){	
			GSets.SVOpt.SelectionType=CB->CurLine;
		}
	}
};
// cva_Opt_WaterQuality
void cva_Opt_WaterQuality::Init(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){
		if(!SD->vm_Init){
			SD->vm_Init=true;
			CB->State=GSets.SVOpt.WaterQuality;			
		}	
	}
};
void cva_Opt_WaterQuality::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		CheckBox* CB=dynamic_cast<CheckBox*>(SD);
		if(CB){	
			GSets.SVOpt.WaterQuality=CB->State;
		}
	}
};
bool Shr;
int Shw;
bool Rf;
bool Al;
bool Cb;
bool Bm;
bool Epe;
//cva_Opt_PreSet
void cva_Opt_PreSet::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){			
		CB->Clear();
		CB->AddLine(GetTextByID("#Options_Quality_UltraHigh"));
		CB->AddLine(GetTextByID("#Options_Quality_High"));
		CB->AddLine(GetTextByID("#Options_Quality_Medium"));
		CB->AddLine(GetTextByID("#Options_Quality_Low"));
		CB->AddLine(GetTextByID("#Options_Quality_Custom"));
		//CB->CurLine=GSets.SVOpt.SelectionType;
		Shr=GSets.SVOpt.HQShaders;
		Shw=GSets.ShadowQuality;
		Rf=GSets.SVOpt.DrawReflectedTerrain;
		Al=GSets.SVOpt.Antialiasing.Enabled;
		Cb=GSets.SVOpt.TrueColor.Enabled;
		Bm=GSets.SVOpt.DrawBumpOnUnits;
	}
};
void cva_Opt_PreSet::SetFrameState(SimpleDialog* SD){
	if(SD->vm_Init){
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){
		}
	}
};
//cva_LANG_PARAM
void cva_LANG_PARAM::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){
		//CB->Clear();
		//static char* rnd=GetTextByID("#English");
		//_str name;
		//name.print("{C C0665F57}%s{C}",rnd);
		//CB->AddLine(name.pchar());
		//CB->AddLine(rnd);
		Enumerator* E=ENUM.Get("LANG_PARAM");
		if(E){
			for(int i=0;i<E->GetAmount();i++){
				CB->AddLine(E->GetStr(i));
			}
		}
		CB->CurLine=LangSettings.LangIndex;
	}
};
void cva_LANG_PARAM::Accept(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){	
		LangSettings.LangIndex=CB->CurLine;
	}
};
void cva_LANG_PARAM::SetFrameState(SimpleDialog* SD){
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){
			LangSettings.LangIndex=CB->CurLine;
	        LangSettings.WriteToFile("Text\\LanguageSettings.xml");
		}
};
//cva_Opt_ColorBit
void cva_Opt_ColorBit::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){			
		CB->Clear();
		//Enumerator* E=ENUM.Get("UNITS_SEL_TYPE");
		//if(E){
		//	for(int i=0;i<E->GetAmount();i++){
		//		CB->AddLine(GetTextByID(E->GetStr(i)));
		//	}
		//}
		CB->AddLine(GetTextByID("#Options_Video_ColorQuality_16"));
		CB->AddLine(GetTextByID("#Options_Video_ColorQuality_24"));
		CB->CurLine=GSets.SVOpt.TrueColor.Enabled;
	}
};
void cva_Opt_ColorBit::Accept(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){	
		GSets.SVOpt.TrueColor.Enabled=CB->CurLine;
	}
};
void cva_Opt_ColorBit::SetFrameState(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){	
		Cb=CB->CurLine;
	}
};
//cva_Opt_Antialiasing
void cva_Opt_Antialiasing::Init(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){			
		CB->State=GSets.SVOpt.Antialiasing.Enabled;
	}
};
void cva_Opt_Antialiasing::Accept(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){	
		GSets.SVOpt.Antialiasing.Enabled=CB->State;
	}
};
void cva_Opt_Antialiasing::SetFrameState(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(CB){	
		Al=CB->State;
	}
};
//cva_Opt_ShaderQuality
void cva_Opt_ShaderQuality::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){			
		CB->Clear();
		//Enumerator* E=ENUM.Get("UNITS_SEL_TYPE");
		//if(E){
		//	for(int i=0;i<E->GetAmount();i++){
		//		CB->AddLine(GetTextByID(E->GetStr(i)));
		//	}
		//}
		CB->AddLine(GetTextByID("#Options_Quality_Low"));
		CB->AddLine(GetTextByID("#Options_Quality_Medium"));
		CB->AddLine(GetTextByID("#Options_Quality_High"));
		CB->CurLine=GSets.SVOpt.HQShaders;
	}
};
void cva_Opt_ShaderQuality::SetFrameState(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){	
		Shr=CB->CurLine;
	}
};
void cva_Opt_ShaderQuality::Accept(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){	
		GSets.SVOpt.HQShaders=CB->CurLine;
	}
};
//cva_Opt_BumpQuality
void cva_Opt_BumpQuality::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){			
		CB->Clear();
		//Enumerator* E=ENUM.Get("UNITS_SEL_TYPE");
		//if(E){
		//	for(int i=0;i<E->GetAmount();i++){
		//		CB->AddLine(GetTextByID(E->GetStr(i)));
		//	}
		//}
		CB->AddLine(GetTextByID("#Options_Quality_Low"));
		CB->AddLine(GetTextByID("#Options_Quality_High"));
		CB->CurLine=GSets.SVOpt.DrawBumpOnUnits;
	}
};
void cva_Opt_BumpQuality::Accept(SimpleDialog* SD){
	if(SD->vm_Init){
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){	
			GSets.SVOpt.DrawBumpOnUnits=CB->CurLine;
		}
	}
};
void cva_Opt_BumpQuality::SetFrameState(SimpleDialog* SD){
	if(SD->vm_Init){
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){	
			Bm=CB->CurLine;
		}
	}
};
//
bool vf_CheckTackticalAIStart(byte NI){
	CurrentGameInfo* g=&GSets.CGame;
	for(int i=NPlayers;i<8;i++){
		if(COMPSTART[i]){
			byte cNI=COMPSTART[i]&15;
			if(cNI==NI)return true;
		}
	}
	return vGameMode==gmSin;
}
// cva_BR_Player
int brPlayer=-1;
void cva_BR_Player::SetFrameState(SimpleDialog* SD){
	if(First)
		brPlayer=0;
	else
		brPlayer++;
}
// cva_BR_PlPing
void cva_BR_PlPing::SetFrameState(SimpleDialog* SD){
	if( brPlayer>=0 && brPlayer<7)
		SD->ID=brPlayer;
	if(GameInProgress)return;
	SD->Visible=false;
	Init(SD);
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&TB->ID<NPlayers){
		cva_BR_PlMapLoading LOAD;
		LOAD.SetFrameState(SD);
		if(!SD->Visible){
			CurrentGameInfo* g=&GSets.CGame;
			PlayerInfo* I=g->PL_INFO+TB->ID;
			PlayerInfo* cI=g->GetCurrentPlayerInfo();
			if(I&&I!=cI){
				TB->Visible=true;
				_str txt;
				txt=g->GetPing(I->PlayerID);
				TB->SetMessage(txt.pchar());
			}
		}
	}
}
void cva_BR_PlPing::Init(SimpleDialog* SD){	
	if(!SD->vm_Init&&SD->ID<=NPlayers){
		SD->vm_Init=true;
	}
}
// cva_BR_PlName
extern byte vmDifficulty[8];
//int vMinAIamount=0;
int vLastSelectAI=1;
//Options (limitations) for multiplayer game:
//ROOMOPT - string with options
//options list:
//   /OPTXY					- fix some option X-option index Y-option value ,example /OPT10 - play only on land (not sea)
//   /RUN					- rum game as fast as possible
//   /PAGE0,/PAGE1,/PAGE2	- show only page 1,2 or 3
//   /NONAME				- hide players names, ????? will be shown instead
//   /RNDNAT				- allow play only random nation
//   /NAT1,/NAT2,NAT3,NAT4	- specify nation of player
//   /NOCOMP				- no AI in room
//   /HIDEMAP				- hide name of designed map
//   /DESIGNED				- specify name of designed map
//   /NOCOPY				- dont try to copy map to another computer
extern char ROOMOPT[512];
bool CheckRoomOpt(int idx,int& Value){
    char s1[32];
	sprintf(s1,"/OPT%d",idx);
	char* s=strstr(ROOMOPT,s1);
	if(s){
		Value=s[5]-'0';
		return true;
	}
	return false;
}
bool CheckRoomRun(){
	return strstr(ROOMOPT,"/RUN");
}
bool CheckRoomNOAI(){
	return strstr(ROOMOPT,"/NOCOMP");
}
bool CheckRoomRage(int idx){
	char s1[32];
	sprintf(s1,"/PAGE%d",idx);
	char* s=strstr(ROOMOPT,s1);
	return s!=NULL;
}
bool CheckRoomNoname(){
	return strstr(ROOMOPT,"/NONAME");
}
bool CheckRoomRndNat(){
	return strstr(ROOMOPT,"/RNDNAT");
}
int GetSpecNat(){
	char* s=strstr(ROOMOPT,"/NAT");
	if(s){
        return atoi(s+4);
	}
	return -1;
}
char* GetSpecMap(int& rnd){
    static char ccc[128];
	char* s=strstr(ROOMOPT,"/MAPS ");
	if(s){
        sscanf(s+10,"%s%d",ccc,&rnd);
		return ccc;
	}
	return NULL;
}
bool CheckHideMap(){
	return strstr(ROOMOPT,"/HIDEMAP");
}
bool CheckNoCopy(){
	return strstr(ROOMOPT,"/NOCOPY");
}
//
CEXPORT void SetInetPlayerName(char* Name);
//
int vCurPlayerID=0;
bool LoadDisable=false;
//
void cva_BR_PlName::SetFrameState(SimpleDialog* SD){
	if( brPlayer>=0 && brPlayer<7)
		SD->ID=brPlayer;
	if(GameInProgress)return;
	vCurPlayerID=SD->ID;
	SD->Visible=false;	
	int NPlMax=7;
	if(vGameMode==gmSin){
		NPlMax=MOptions.Players.GetN_Single();
	}else
	if(vGameMode==gmMul){
		NPlMax=MOptions.Players.GetN_Multi();
	}
	if(MOptions.MaxPlayers)NPlMax=std::min(NPlMax,MOptions.MaxPlayers);
	//
	// EW2 - skirmish 7, battles 6.
	//
	if(vGameMode==gmMul){
		NPlMax=7;		
		for(int s=0;s<MOptions.OnMissionStart.GetAmount();s++){
			StartTacticalAI* SAI=dynamic_cast<StartTacticalAI*>(MOptions.OnMissionStart[s]);
			if(SAI){
				NPlMax=6;				
			}
		}
	}
	//
	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* I=g->PL_INFO+SD->ID;
	// erease clan []
	char* Name=g->PL_INFO[SD->ID].name;
	if(Name&&Name[0]=='['){
		while(Name[0]&&Name[0]!=']'){
			Name++;
		}
		if(Name[0]){
			Name++;
			if(I==g->GetCurrentPlayerInfo()){
				SetInetPlayerName(Name);
			}
			strcpy(g->PL_INFO[SD->ID].name,Name);
		}
	}
	// only one AI
	//if(NPlMax>NPlayers+1) NPlMax=NPlayers+1;
	//
	static char* ech=GetTextByID("#Empty");
	//if(SD->ID<NPlMax){
	// load game				
		if(vls_Active){
			if((SD->ID<vSaveInfo.NPlayers||SD->ID<NPlayers)&&strcmp(vSaveInfo.Names[SD->ID],ech)){
                byte RtoS[8],StoR[8];
				memset(RtoS,0xFF,sizeof RtoS);
				memset(StoR,0xFF,sizeof StoR);
				// find name				
				for(int i=0;i<NPlayers;i++){
					char* rName=g->PL_INFO[i].name;
					for(int j=0;j<vSaveInfo.NPlayers;j++){
						char* sName=vSaveInfo.Names[j];
						if(strcmp(rName,sName)==0){
							RtoS[i]=j;
							StoR[j]=i;
						}
					}
				}
				//
				char *PName,*SName;
				PName=SName=vSaveInfo.Names[SD->ID];
				vCurPlayerID=StoR[SD->ID];
				if(StoR[SD->ID]==0xFF/*&&SD->ID<NPlayers*/){
					vCurPlayerID=-1;
					PName=NULL;
					int jj=0;
					for(int j=0;j<SD->ID;j++){
						if(StoR[j]==0xFF){
							jj++;
						}
					}
					int j=0;
					for(int i=0;i<NPlayers;i++){
						if(RtoS[i]==0xFF){
							if(j==jj){
								PName=g->PL_INFO[i].name;
								vCurPlayerID=i;
								break;
							}
							j++;
						}
					}
				}
				if(SD->ID>=vSaveInfo.NPlayers){
					SName=NULL;
					vCurPlayerID=0xFF;
				}
				//
				SD->Enabled = (PName==SName);
				SD->Diffuse=0xFFFFFFFF;
				if(PName!=SName){
					if(PName)LoadDisable=true;
					if((GetTickCount()/500)&1){
						PName=SName;
						SD->Diffuse=0x77FFFFFF;
					}
				}
				/*
				if(SD->ID>=NPlayers){
					SD->Diffuse=0x77FFFFFF;
				}
				*/
				TextButton* TB=dynamic_cast<TextButton*>(SD);
				if(TB){
					if(PName){
						TB->SetMessage(PName);
					}else{
						TB->SetMessage("");
					}					
					SD->Visible=true;					
				}
				ComboBox* CB=dynamic_cast<ComboBox*>(SD);
				if(CB){					
					CB->Clear();
					if(PName){
						CB->AddLine(PName);
					}					
					SD->Visible=true;					
				}
			}
			return;
		}
		TextButton* TB=dynamic_cast<TextButton*>(SD);
		if(TB&&SD->ID<=NPlayers){
			SD->Visible=true;
			_str name;
			if(I==g->GetCurrentPlayerInfo()){
				//strcpy(I->name,PlName);
				name="{C FFffcb2b}";
			}
			name+=I->name;
			TB->SetMessage(name.pchar());
			if(CheckRoomNoname()){
				TB->SetMessage("??????");
			}
		}
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){
			if(SD->ID>=NPlMax){
				ComboBox* CB=dynamic_cast<ComboBox*>(SD);
				if(CB){
					DWORD C=255-100-(sin(double(GetTickCount())/200.0)*100);
					CB->Diffuse=0xFFFF0000+(C<<8)+C;
				}
				if(SD->ID>=NPlMax){
					COMPSTART[SD->ID]=0;
				}
			}else CB->Diffuse=0xFFFFFFFF;
			CB->Enabled=v_MM_Host||vGameMode==gmSin;
			if(SD->ID<NPlayers){
				CB->Visible=true;				
				//
				int _curLine=CB->CurLine;
				bool _curPlayer=(I==g->GetCurrentPlayerInfo());
				//				
				CB->Clear();
				_str name;
				if(_curPlayer){
					CB->Enabled=false;
					strcpy(I->name,PlayerName.pchar());
					name="{C FFffcb2b}";
				}
				name+=I->name;
				CB->AddLine(name.pchar());
				if(!_curPlayer&&CB->Enabled){
					if(_curLine==CB->NLines){
						DWORD id=g->PL_INFO[SD->ID].PlayerID;
						g->Kick(id);
					}
					static char* K=GetTextByID("#KickPlayer");
					CB->AddLine(K);
				}
				CB->CurLine=0;
				if(COMPSTART[SD->ID]){
					for(int i=7;i>SD->ID;i--){
						COMPSTART[i]=COMPSTART[i-1];
					}
					// zamuchal gluck
					COMPSTART[SD->ID]=0;
				}
			}else
			if(!CheckRoomNOAI()&&(vGameMode!=gmMul||NPlayers>1)&&(COMPSTART[SD->ID]||CB->Enabled&&(COMPSTART[SD->ID-1]||SD->ID==NPlayers))){
				CB->Visible=true;
				bool withEmpty=SD->ID==NPlMax-1||(COMPSTART[SD->ID]&&COMPSTART[SD->ID+1]==0&&(v_MM_Host||SD->ID>1))||COMPSTART[SD->ID]==0;
				if(SD->ID>=NPlMax&&withEmpty&&COMPSTART[SD->ID]==0) CB->Visible=false;
				// only one AI
				extern int NAI_Max;
				if(SD->ID>=NPlayers+NAI_Max) CB->Visible=false;
				/*
				// only for EW2
				int curLine=-1;
				if(SD->ID==NPlayers&&COMPSTART[SD->ID]&&COMPSTART[SD->ID-1]){
					// player connected
					COMPSTART[SD->ID-1]=0;
					curLine=(COMPSTART[SD->ID]>>8)&7;
					I->MapStyle=curLine;					
				}else{
					// player disconected 
					if(SD->ID==NPlayers&&COMPSTART[SD->ID]==0&&COMPSTART[SD->ID+1]){						
						COMPSTART[SD->ID]=COMPSTART[SD->ID+1];
						COMPSTART[SD->ID+1]=0;						
						curLine=(COMPSTART[SD->ID]>>8)&7;
						I->MapStyle=curLine;					
						// zamuchal gluck
						COMPSTART[SD->ID]=0;
					}
					if(SD->ID>NPlayers&&COMPSTART[SD->ID]){						
						COMPSTART[SD->ID]=0;
						curLine=0;
						I->MapStyle=curLine;						
					}
				}
				*/
				// add new comp
				if(COMPSTART[SD->ID]==0&&(CB->CurLine!=0||vGameMode==gmSin&&SD->ID==1)){					
					// add new computer player
					if(vGameMode==gmSin&&SD->ID==1){
						CB->CurLine=vmSinglePlayerData.Difficulty;
					}
					COMPSTART[SD->ID]=1;
					if(SD->ID>=NPlMax){
						COMPSTART[SD->ID]=0;
					}
				}		
				if(vGameMode==gmSin&&SD->ID==1){					
					withEmpty=false;
				}
				if(vGameMode!=gmSin&&!v_MM_Host)withEmpty=false;
				if(CB->CurLine==0&&withEmpty&&COMPSTART[SD->ID]&&(v_MM_Host||vGameMode==gmSin)){ //v_MM_Host
					// delete computer
					COMPSTART[SD->ID]=0;					
				}else{
					// create list
					static _str curl;
					curl="";
					if(!v_MM_Host&&vGameMode!=gmSin){
						CB->CurLine=I->MapStyle;
					}
					if(CB->NLines==0&&COMPSTART[SD->ID]!=0){
						if(CB->CurLine>=0&&CB->CurLine<GlobalAI.Diff.GetAmount()){
							curl=GlobalAI.Diff[CB->CurLine].Name;
						}						
					}else{
						if(CB->CurLine>=CB->NLines)CB->CurLine=CB->NLines-1;
						curl=CB->CurLine>=0?CB->Lines[CB->CurLine]:"";
					}
					//					
					if(withEmpty){
						CB->Clear();						
						CB->AddLine(ech);								
					}else{
						if(vGameMode==gmSin&&SD->ID==1){
							vmSinglePlayerData.Difficulty=CB->CurLine;
						}
						CB->Clear();
					}
#ifdef _DEMO_
					const Nai=1;
#endif _DEMO_
					_str line;
					for(int i=0;i<GlobalAI.Diff.GetAmount();i++){
						line.Clear();
						//line.print("Ai %s",GlobalAI.Diff[i].Name);
						line=GlobalAI.Diff[i].Name;
#ifdef _DEMO_						
						if(i>=GlobalAI.Diff.GetAmount()-Nai){
							_str name;
							name.print("{C C0665F57}%s{C}",line.pchar()); //{CR}
							CB->AddLine(name.pchar());
						}else{
							CB->AddLine(line.pchar());
						}						
#else
						CB->AddLine(line.pchar());
#endif _DEMO_
					}
					if(v_MM_Host||vGameMode!=gmMul){
						// repaire CurLine
						CB->CurLine=0;
						for(int i=0;i<CB->NLines;i++) if(!strcmp(CB->Lines[i],curl.pchar())){
							CB->CurLine=i;						
							break;
						}					
						I->MapStyle=CB->CurLine;
						if(!withEmpty){
							I->MapStyle++;
						}
					}else{
						if(SD->ID>=NPlayers){
							word W=COMPSTART[SD->ID];
							int diff=(W>>8)&7;
							CB->CurLine=diff>0?diff-1:0;
							I->ColorID=W&15;
							int team=(W>>4)&15;
							I->GroupID=team;														
						}
					}
#ifdef _DEMO_
					if(CB->CurLine>=GlobalAI.Diff.GetAmount()-Nai){
						CB->CurLine=GlobalAI.Diff.GetAmount()-Nai-1;
						if(CB->CurLine<0) CB->CurLine=0;
					}					
#endif _DEMO_
					strcpy(I->name,curl.pchar());
					if(COMPSTART[SD->ID]&&(v_MM_Host||vGameMode!=gmMul)){
						word W=0;
						/*int Nat=I->NationID;
						if(Nat==0){
							Nat=GetRND(GlobalAI.NAi);
						}else{
							Nat--;
						}
						W+=Nat<<11;*/
						W+=I->GroupID<<4;
						int Diff=I->MapStyle;						
						W+=(Diff&7)<<8;					
						//int Team=(W>>4)&15;
						//W+=;
						int Color=I->ColorID&15;
						W+=Color;
						COMPSTART[SD->ID]=W;
						vmDifficulty[Color]=Diff-1;
					}
				}
			}
			if(SD->ID<NPlayers){
				CB->SetHint("#MR_Player_hint");				
			}else{
				CB->SetHint("#MR_Computer_hint");				
			}			
			if(CheckRoomNOAI()){
				memset(&COMPSTART,0,sizeof COMPSTART);
			}			
			if(SD->ID>NPlayers&&COMPSTART[SD->ID-1]==0){
				COMPSTART[SD->ID]=0;
			}
		}	
}
void cva_BR_PlName::Init(SimpleDialog* SD){	
	if(GameInProgress)return;
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){
		CB->Clear();		
	}
	if(SD->ID<=NPlayers&&!PlayerName.isClear()){
		CurrentGameInfo* g=&GSets.CGame;
		PlayerInfo* I=g->GetCurrentPlayerInfo();
		if(I==g->PL_INFO+SD->ID){				
			strcpy(PlName,PlayerName.pchar());
			strcpy(I->name,PlayerName.pchar());
		}
		TextButton* TB=dynamic_cast<TextButton*>(SD);
		if(TB){
		}
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){
		}
		if(SD->ID==0){
			memset(COMPSTART,0,sizeof COMPSTART);
		}
	}
};
//cva_BR_Ready
void cva_BR_Ready::Init(SimpleDialog* SD){
	CheckBox* CB=dynamic_cast<CheckBox*>(SD);
	if(!CB&&SD->DSS.GetAmount()){
		CB=dynamic_cast<CheckBox*>(SD->DSS[0]);
	}
	if(CB){
		CB->State=false;
	}
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	if(VB){
		VB->State=0;
	}
}
int mReady=-1;
bool cva_BR_Ready::LeftClick(SimpleDialog* SD){
	if( Name=="Button" ){
		VitButton* VB=dynamic_cast<VitButton*>(SD);
		if(VB){
			VB->State=(VB->State+1)&1;
			mReady=VB->State;
		}
	}
	return true;
}
bool vDropReady=false;
void cva_BR_Ready::SetFrameState(SimpleDialog* SD){	
	if( brPlayer>=0 && brPlayer<7)
		SD->ID=brPlayer;
	CurrentGameInfo* g=&GSets.CGame;
	if( Name=="Button" ){
		if(v_MM_Host){
			vCurPlayerID=0xFF;
		}else{
			for(int i=0;i<7;i++){
				PlayerInfo* I=g->PL_INFO+i;
				if(I==g->GetCurrentPlayerInfo()){
					vCurPlayerID=i;
					break;
				}
			}
		}
	}
	if(GameInProgress)return;
	SD->ID=vCurPlayerID;
	SD->Visible=false;
	if(NPlayers==1||vCurPlayerID==0xFF)return;
	if(SD->ID<NPlayers){		
		PlayerInfo* pi=GSets.CGame.GetCurrentPlayerInfo();
		if(SD->ID>=0&&SD->ID<7){
			CheckBox* CB=dynamic_cast<CheckBox*>(SD);
			if(!CB&&SD->DSS.GetAmount()){
				CB=dynamic_cast<CheckBox*>(SD->DSS[0]);
			}
			VitButton* VB=dynamic_cast<VitButton*>(SD);
			PlayerInfo* I=g->PL_INFO+SD->ID;
			if(I==g->GetCurrentPlayerInfo()){
				SD->Enabled=true;
				if(vDropReady){
					vDropReady=false;
					if(CB)
						CB->State=false;
					if(VB)
						VB->State=0;
				}
				if(CB){
					if(mReady!=-1){
						CB->State=(mReady==1);
						mReady=-1;
					}
					if(I->Ready!=CB->State){
						I->Ready=CB->State?1:0;
						mReady=I->Ready;
					}
				}
				if(VB){
					if(mReady!=-1){
						VB->State=mReady;
						mReady=-1;
					}
					I->Ready=VB->State;
				}
				SD->Diffuse=0xFFFFFFFF;
			}else{
                SD->Enabled=false;
				SD->Diffuse=0x80FFFFFF;
				if(CB)
					CB->State=I->Ready;
				if(VB)
					VB->State=I->Ready?1:0;
			}
			SD->Visible=true;
			//SD->SetHint("#MR_Ready_hint");
		}		
	}
};
// cva_MM_LobbyVersion
void cva_MM_LobbyVersion::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		//static _str txt;
		extern char LobbyVersion[32];
		//txt=LobbyVersion;
		//TB->SetMessage(txt.pchar());
		TB->SetMessage(LobbyVersion);
	}
}
// cva_BR_PlHero
void cva_BR_PlHero::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){
		CB->Clear();
		static char* rnd=GetTextByID("#PPH_Random");
		//_str name;
		//name.print("{C C0665F57}%s{C}",rnd);
		//CB->AddLine(name.pchar());
		CB->AddLine(rnd);
		Enumerator* E=ENUM.Get("PlayerParams_Hero");
		if(E){
			for(int i=0;i<E->GetAmount();i++){
				CB->AddLine(E->GetStr(i));
			}
		}
	}
}
void cva_BR_PlHero::SetFrameState(SimpleDialog* SD){
	if( brPlayer>=0 && brPlayer<7)
		SD->ID=brPlayer;
	if(GameInProgress)return;
	SD->Visible=false;
	if(vls_Active) return;
	if(SD->ID<NPlayers||COMPSTART[SD->ID]){
		SD->Visible=true;
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){
			CurrentGameInfo* g=&GSets.CGame;
			PlayerInfo* I=g->PL_INFO+SD->ID;
			if(I==g->GetCurrentPlayerInfo()||COMPSTART[SD->ID]&&v_MM_Host||vGameMode==gmSin){
				CB->Enabled=(I->Ready==0);
				//GSets.RoomParams.Players[SD->ID]->Hero=CB->CurLine;
				I->Hero=CB->CurLine;
			}else{
				CB->Enabled=false;
				//CB->CurLine=GSets.RoomParams.Players[SD->ID]->Hero;
				CB->CurLine=I->Hero;
			}
		}
	}
}
// cva_BR_PlRace
void cva_BR_PlRace::SetFrameState(SimpleDialog* SD){	
	if( brPlayer>=0 && brPlayer<7)
		SD->ID=brPlayer;
	if(GameInProgress)return;
	SD->Visible=false;
	if(vls_Active) return;
	if(SD->ID<NPlayers||COMPSTART[SD->ID]){
		SD->Visible=true;
		Init(SD);
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB&&SD->ID>=0&&SD->ID<7){		
			CurrentGameInfo* g=&GSets.CGame;
			PlayerInfo* I=g->PL_INFO+SD->ID;
			if(I==g->GetCurrentPlayerInfo()||COMPSTART[SD->ID]&&v_MM_Host||vGameMode==gmSin){
				CB->Enabled=(I->Ready==0);
			}else{
				CB->Enabled=false;
			}
			if(I==g->GetCurrentPlayerInfo()||COMPSTART[SD->ID]){
				byte NI=0;
				if(COMPSTART[SD->ID]){
					NI=COMPSTART[SD->ID]&15;
				}else{
					NI=I->ColorID;
				}
#ifdef _DEMO_
				if(CB->CurLine!=1) CB->CurLine=1;
				//if(CB->CurLine>2) CB->CurLine=2;
#endif _DEMO_				
				MapPlayerInfo* P=MOptions.Players.Player+NI;
				if(P){					
					if(P->DisableNationSelect){
						CB->CurLine=P->Nation+1;
						CB->Enabled=false;
					}
				}				
				if(I->NationID!=CB->CurLine){
					vDropReady=true;
				}
				I->NationID=CB->CurLine;
			}else{
				CB->CurLine=I->NationID;				
			}
			if(CheckRoomRndNat()&&I==g->GetCurrentPlayerInfo()){
				CB->CurLine=0;
				CB->Enabled=false;
			}
			int n=GetSpecNat();
			if(n!=-1&&n<CB->NLines&&I==g->GetCurrentPlayerInfo()){
				CB->CurLine=n;
				CB->Enabled=false;                
			}
		}
		SD->SetHint("#MR_Race_hint");
	}
};
void cva_BR_PlRace::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB&&!SD->vm_Init){
		SD->vm_Init=true;
		CB->Clear();
		static char* rnd=GetTextByID("#NationCombo_Random");
#ifdef _DEMO_
		_str name;
		name.print("{C C0665F57}%s{C}",rnd);
		CB->AddLine(name.pchar());		
#else
		CB->AddLine(rnd);
#endif _DEMO_
		int NG=GlobalAI.NAi;
		for(int j=0;j<NG;j++){			
#ifdef _DEMO_	
			name.Clear();
			if(j!=0){
				name.print("{C C0665F57}%s{C}",GlobalAI.Ai[j].Message);//{CR}
			}else{
				name=GlobalAI.Ai[j].Message;
			}

			CB->AddLine(name.pchar());
#else
			CB->AddLine(GlobalAI.Ai[j].Message);
#endif _DEMO_
		};
		CB->CurLine=0;
	}		
}
// cva_BR_PlColor
int NAI_Max=0;
byte* GetPlayerLockColors(PlayerInfo* I){
	CurrentGameInfo* g=&GSets.CGame;
	// detect ai
	bool ai=false;
	for(int i=0;i<NPlayers||COMPSTART[i];i++){
		PlayerInfo* cI=g->PL_INFO+i;
		if(I==cI){
			if(COMPSTART[i]) ai=true;
			break;
		}
	}
	//
	static byte LockColor[8];
	memset(LockColor,0,sizeof LockColor);
	// Tactical AI for battles
	bool battles=false;
	NAI_Max=10;
	for(int s=0;s<MOptions.OnMissionStart.GetAmount();s++){
		StartTacticalAI* SAI=dynamic_cast<StartTacticalAI*>(MOptions.OnMissionStart[s]);
		if(SAI){
			NAI_Max=1;
			battles=true;
			if(ai){
				for(int i=0;i<8;i++){
					if(i!=SAI->NI){
						LockColor[i]++;
					}
				}
			}else{
				//LockColor[i]++;
			}
		}
	}	
	// lock used color
	char* GetSpecMap(int& rnd);
	int rnd=0;
	char* NAME=GetSpecMap(rnd);
	//
	if(battles||NAME&&strstr(NAME,"RANDOM")){
		PlayerInfo* CI=g->GetHostPlayerInfo();
		if(I&&I!=CI&&(vGameMode!=gmSin||ai)){
			for(int i=0;i<NPlayers||COMPSTART[i];i++){
				PlayerInfo* cI=g->PL_INFO+i;
				if(I!=cI){				
					LockColor[cI->ColorID]++;				
				}
			}
		}
	}
	// lock disabled colors
	if(!MOptions.RandomizePlayersPositions){
		for(i=0;i<7;i++){
			MapPlayerInfo* P=MOptions.Players.Player+i;
			if(vGameMode==gmMul&&P->DisableInMultiplayer||vGameMode==gmSin&&P->DisableInSingle){
				LockColor[i]++;
			}
		}
	}	
	// lock ai colors for player
	if(!ai){
		int id=-1;
		for(int i=0;i<NPlayers;i++){
			PlayerInfo* cI=g->PL_INFO+i;
			if(I==cI){
				id=i;
				break;
			}
		}
		if(id!=-1){
			for(i=NPlayers;i<8&&i<COMPSTART[i];i++){
				if(COMPSTART[i]){
					PlayerInfo* cI=g->PL_INFO+i;
					LockColor[cI->ColorID]++;
				}
			}
		}
	}
	//
	return LockColor;
}
bool CheckPlayerColor(PlayerInfo* I){
	byte* lock=GetPlayerLockColors(I);
	return lock[I->ColorID]==0;	
}
bool SetNextPlayerColor(PlayerInfo* I){
	byte* lock=GetPlayerLockColors(I);
	int i=0;
	int prevColor=I->ColorID;
	do{
		I->ColorID=I->ColorID<6?I->ColorID+1:0;		
		i++;
		if(i>8) return false;
	}while(lock[I->ColorID]>0);
	if(I->ColorID!=prevColor){
		vDropReady=true;
	}
	return true;
};
bool SetPrevPlayerColor(PlayerInfo* I){
	byte* lock=GetPlayerLockColors(I);
	int i=0;
	int prevColor=I->ColorID;
	do{
		I->ColorID=I->ColorID==0?6:I->ColorID-1;
		i++;
		if(i>8) return false;
	}while(lock[I->ColorID]>0);
	if(I->ColorID!=prevColor){
		vDropReady=true;
	}
	return true;
};
extern bool vmBattles;
void ShowColorFlag(GPPicture* GP, byte ColorID){
	static int f=GPS.PreLoadGPImage("interf3\\RoomNatColor");
	int n=GPS.GPNFrames(f);
	static int t[7];
	static bool init=false;
	if(!init){
		init=true;
		for(int i=0;i<7;i++) t[i]=GetTickCount()-(rand()%2000);
	}			
	int s=((GetTickCount()-t[GP->ID])/40)%n;
	GP->SetFileID(&f);
	GP->SetSpriteID(s);
	//
	GP->Diffuse=MulDWORD(GetNatColor(ColorID),300);
}
//
bool vEnableColorCheck=false;
//
void cva_BR_PlColor::SetFrameState(SimpleDialog* SD){
	if( brPlayer>=0 && brPlayer<7)
		SD->ID=brPlayer;
	if(GameInProgress)return;
	SD->Visible=false;
	SD->Enabled=true;
	SD->SetHint("#MR_Color_hint");
	Init(SD);
	SD->Diffuse=0xFFFFFFFF;
	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* I=g->PL_INFO+SD->ID;	
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	//
	if(GP&&Animated){
		if(vls_Active){
			static char* ech=GetTextByID("#Empty");
			if((SD->ID<vSaveInfo.NPlayers||SD->ID<NPlayers)&&strcmp(vSaveInfo.Names[SD->ID],ech)){
				//for(int i=0;i<vSaveInfo.NPlayers;i++){					
					//if(strcmp(vSaveInfo.Names[i],I->name)==0){
						SD->Visible=true;
						if(I==g->GetCurrentPlayerInfo()||COMPSTART[SD->ID]&&v_MM_Host||vGameMode==gmSin)
							SD->Enabled=(I->Ready==0);
						//byte NI=vSaveInfo.NatRefTBL[vSaveInfo.Colors[i]];
						byte NI=vSaveInfo.NatRefTBL[vSaveInfo.Colors[SD->ID]];
						ShowColorFlag(GP,NI);
						//break;
					//};
				//}
			}
			return;
		}
	}
	//
	if(GP&&SD->ID>=0&&(SD->ID<NPlayers||COMPSTART[SD->ID])){
		if(!CheckPlayerColor(I)){
			vEnableColorCheck=true;
			LeftClick(SD);
			vEnableColorCheck=false;
		}
		GP->Visible=true;
		if(I==g->GetCurrentPlayerInfo())
			SD->Enabled=(I->Ready==0);
		if(OneSprited){
			if(Animated){
				ShowColorFlag(GP,I->ColorID);
			}else{
				GP->Nation=I->ColorID;
			}
		}else{
			GP->SetSpriteID(I->ColorID+1);
		}		
		if(v_MSErr.Color[I->ColorID]){
			if((GetTickCount()>>9)&1){
				GP->Diffuse=0x20FFFFFF;
			}			
		}		
	}
};
void cva_BR_PlColor::Init(SimpleDialog* SD){
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP&&!SD->vm_Init){
		SD->vm_Init=true;
		//if( brPlayer>=0 && brPlayer<7)
		//	SD->ID=brPlayer;
		PlayerInfo* I=GSets.CGame.PL_INFO+SD->ID;
		if(I)
			I->ColorID=SD->ID;
	}		
}
bool cva_BR_PlColor::LeftClick(SimpleDialog* SD){
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP&&SD->vm_Init&&!vls_Active){
		CurrentGameInfo* g=&GSets.CGame;
		PlayerInfo* I=g->PL_INFO+SD->ID;
		if(I==g->GetCurrentPlayerInfo()||COMPSTART[SD->ID]&&(vGameMode==gmSin||v_MM_Host)||vEnableColorCheck){
			SetNextPlayerColor(I);
		}		
	}	
	return true;
}
bool cva_BR_PlColor::RightClick(SimpleDialog* SD){
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP&&SD->vm_Init&&!vls_Active){
		CurrentGameInfo* g=&GSets.CGame;
		PlayerInfo* I=g->PL_INFO+SD->ID;
		if(I==g->GetCurrentPlayerInfo()||COMPSTART[SD->ID]&&(vGameMode==gmSin||v_MM_Host)||vEnableColorCheck){		
			SetPrevPlayerColor(I);
		}		
	}	
	return true;
}
// cva_BR_PlTeam
void cva_BR_PlTeam::SetFrameState(SimpleDialog* SD){
	if( brPlayer>=0 && brPlayer<7)
		SD->ID=brPlayer;
	if(GameInProgress)return;
	SD->Visible=false;
	if(vls_Active) return;
	PlayerInfo* PI=GSets.CGame.GetCurrentPlayerInfo();
	//if(PI) PI->GroupID=0;
	if((SD->ID<NPlayers||COMPSTART[SD->ID])&&!MOptions.Players.DisableTeams){
		//Init(SD);		
		if(SD->ID>=0){
			CurrentGameInfo* g=&GSets.CGame;
			PlayerInfo* I=g->PL_INFO+SD->ID;
			_str txt;
			if(I->GroupID){
				txt=I->GroupID;
			}else{
				txt="-";
			}						
			SD->Visible=true;
			SD->Diffuse=0xFFFFFFFF;
			if(v_MSErr.Team[I->GroupID]){
				if((GetTickCount()>>9)&1){
					SD->Diffuse=0x8FFFFFFF;
				}				
			}	
			TextButton* TB=dynamic_cast<TextButton*>(SD);
			if( !TB && SD->DSS.GetAmount() ){
				TB=dynamic_cast<TextButton*>(SD->DSS[0]);
			}
			if(TB){
				TB->SetMessage(txt.pchar());
			}
			GP_TextButton* gpTB=dynamic_cast<GP_TextButton*>(SD);
			if(gpTB){
				gpTB->SetMessage(txt.pchar());
			}
			MapPlayerInfo* MP=&MOptions.Players.Player[I->ColorID];
			if(MP->LockTeam){
				SD->Enabled=false;
				SD->Diffuse=0x8FFFFFFF;
				I->GroupID=MP->Team;
			}else{
				SD->Enabled=true;
				SD->Diffuse=0xFFFFFFFF;
				// only for EW2
				//SD->Enabled=false;
				//I->GroupID=0;
			}
			if(I==PI)
				SD->Enabled&=(I->Ready==0);
			SD->Enabled&=(I==PI||COMPSTART[SD->ID]&&(vGameMode==gmSin||v_MM_Host));
		}
		SD->SetHint("#MR_Team_hint");
	}
};
/*
void cva_BR_PlTeam::Init(SimpleDialog* SD){
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP&&!SD->vm_Init){
		SD->vm_Init=true;
		PlayerInfo* I=GSets.CGame.PL_INFO+SD->ID;
		I->ColorID=SD->ID;
	}		
}
*/
bool cva_BR_PlTeam::LeftClick(SimpleDialog* SD){
	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* I=g->PL_INFO+SD->ID;
	if(I==g->GetCurrentPlayerInfo()||vGameMode==gmSin||v_MM_Host){
		I->GroupID=I->GroupID<4?I->GroupID+1:0;
	}		
	return true;
}
bool cva_BR_PlTeam::RightClick(SimpleDialog* SD){
	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* I=g->PL_INFO+SD->ID;
	if(I==g->GetCurrentPlayerInfo()||vGameMode==gmSin||v_MM_Host){
		I->GroupID=I->GroupID==0?4:I->GroupID-1;
	}		
	return true;
}
//
bool CheckSaveVerison(char* FileName);
//
_str cfl_Dir;
_str cfl_LastDir;
_str cfl_Mask;
int cfl_Level;
bool vf_CreateFileList(ListDesk* LD,bool addmode=false){
	if(!LD) return false;
	if(!addmode)LD->DSS.Clear();
	//LD->CurrentElement=-1;
	//vls_FileName.Clear();
	//
	if(!cfl_LastDir.isClear()&&cfl_Dir!=cfl_LastDir){
		cfl_Dir=cfl_LastDir;		
	}else{
		cfl_LastDir=cfl_Dir;
	}
	//
	WIN32_FIND_DATA	DD;
	HANDLE	hFindDir;
	_str dir;
	dir=cfl_Dir;	
	if(!dir.isClear()){
		dir+="\\";
	}
	dir+="*";
	hFindDir=FindFirstFile(dir.pchar(),&DD);
	if(hFindDir!=INVALID_HANDLE_VALUE){
		do{
			if((DD.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&strcmp(DD.cFileName,".")){
				if( cfl_Level>=0 ){
					if(strcmp(DD.cFileName,"..")==0){
						//LD->CurrentElement=-1;
						if(cfl_Level>0){
							LD->AddElement("<..>");
						}					
					}else{
						_str d;
						d.print("<%s>",DD.cFileName);
						LD->AddElement(d.pchar());
					}
				}
			}
		}while(FindNextFile(hFindDir,&DD));

		_str f;
		f.print("%s\\%s",cfl_Dir.pchar(),cfl_Mask.pchar());
		//TGSCFindInfo* FD=NULL;
		//FD=GSFILES.gFindFirst(f.pchar());
		//if(FD){
		//	do{
		//		int pos=LD->AddElement(FD->m_FileName);	
		//	}while(GSFILES.gFindNext(FD));
		//};
		hFindDir=FindFirstFile(f.pchar(),&DD);
		if(hFindDir!=INVALID_HANDLE_VALUE){
			ClassArray<_str> Names;
			DynArray<DWORD> TimesHigh;
			DynArray<DWORD> TimesLow;
			do{
				if( !(DD.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) ){
					DWORD THigh=DD.ftLastWriteTime.dwHighDateTime;
					DWORD TLow=DD.ftLastWriteTime.dwLowDateTime;
					_str* N=new _str;
					N->Assign(DD.cFileName);
					int n=Names.GetAmount();
					for(int i=0;i<n;i++){
						if( THigh>TimesHigh[i] || THigh==TimesHigh[i]&&TLow>TimesLow[i] ){
							break;
						}
					}
					Names.Insert(i,N);
					TimesHigh.Insert(i,THigh);
					TimesLow.Insert(i,TLow);
				}
			}while(FindNextFile(hFindDir,&DD));
			int n=Names.GetAmount();
			for(int i=0;i<n;i++){
				LD->AddElement(Names[i]->pchar());
			}
		}

		LD->CurrentElement=LD->GetElement(vls_FileName.pchar());
		if(LD->CurrentElement==-1){			
			vls_FileName.Clear();
		}
		return true;
	}
	return false;
}
//
char* GetLDFileName();
//
bool rdEnable=false;
//
_str vls_FileName;
bool vls_Active=false;
// cva_LS_FileList
bool vls_ReInit=false;
void cva_LS_FileList::SetFrameState(SimpleDialog* SD){	
#ifndef _DEMO_
	va_ListDesk::SetFrameState(SD);
	if(vls_ReInit){
		Init(SD);
		vmIGP.GameModeForSave=vGameMode;
		vls_ReInit=false;
	}
	//
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	//CurrentGameInfo* g=&GSets.CGame;
	//PlayerInfo* I=g->GetCurrentPlayerInfo();
	/*&&I&&v_MM_Host*/
	rdEnable=false;
	if(LD){
		if(LD->CurrentElement!=-1&&LD->CurrentElement!=prev){
			//
			memset(&vSaveInfo,0,sizeof vSaveInfo);
			//
			//vls_FileName.Clear();
			prev=LD->CurrentElement;
			const char* ce=LD->GetElement(LD->CurrentElement);
			if(ce[0]=='<'){
				if(ce[1]=='.'&&ce[2]=='.'){
					char* cd=cfl_Dir.pchar();					
					for(int i=strlen(cd);i>=0;i--){
						if(cd[i]=='\\'||i==0){
							cd[i]=0;
							cfl_Dir.L=strlen(cd);
							break;
						}
					}
					cfl_Level--;
				}else{
					if(!cfl_Dir.isClear()){
						cfl_Dir+="\\";
					}					
					cfl_Dir+=ce+1;
					cfl_Dir.Replace(">","");
					//cfl_Dir.pchar()[strlen(cfl_Dir.pchar())-1]=0;
					cfl_Level++;
				}
				cfl_LastDir.Clear();
				vf_CreateFileList(LD);
				prev=-1;
			}else{				
				/*
				vls_FileName=cfl_Dir;
				if(!cfl_Dir.isClear()){
					vls_FileName+="\\";
				}
				*/
				//vls_FileName+=LD->GetElement(LD->CurrentElement);
				vls_FileName=LD->GetElement(LD->CurrentElement);
				// preload *.sav info
				vSaveInfo.Preload(GetLDFileName());
			}			
		}else{
			//vls_FileName.Clear();
		}
		//
		rdEnable=(LD->CurrentElement!=-1);
		if(!InGame&&vGameMode==gmMul&&!v_MM_Host){
			PlayerInfo* I=GSets.CGame.GetHostPlayerInfo();
			//
			LD->CurrentElement=-1;
			vls_FileName.Clear();
			//
			if(I&&I->MapName){
				vSaveInfo.Preload(I->MapName);
				for(int i=0;i<LD->DSS.GetAmount();i++){
					vls_FileName=LD->GetElement(i);
					if(strcmp(GetLDFileName(),I->MapName)==0){
						LD->CurrentElement=i;
						break;
					}
				}
			}else{
				memset(&vSaveInfo,0,sizeof vSaveInfo);

			}
		}
		//
		if(vGameMode==gmMul&&v_MM_Host){
			PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
			if(I){
				if(!vls_FileName.isClear()&&rdEnable){
					char* fn=GetLDFileName();
					strcpy(I->MapName,fn);
				}else{
					I->MapName[0]=0;
				}
			}
		}
		//
		LD->Enabled=(vGameMode!=gmMul||v_MM_Host);
		//
		vls_Active=true;
		mMapPreview=false;
	}	
#endif _DEMO_
}
char* GetAutoSaveFileName(){
	static DString FN;
	FN.Clear();
	if( vGameMode==gmSin ){
		FN="save\\Single\\AUTO.sav";
	}else if( vGameMode==gmCamp ){
		//FN="save\\Camp\\AUTO.sav";
		FN="save\\Camp\\";
		FN+=vmSinglePlayerData.PlayerName;
		FN+="\\AUTO.sav";
	}else{
		FN="save\\Multi\\AUTO.sav";
	}
	return FN.pchar();
}
void cva_LS_FileList::Init(SimpleDialog* SD){
#ifndef _DEMO_
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){		
		if(vGameMode==gmEdit){
			cfl_Level=1;
			cfl_Dir="Maps";			
			cfl_Mask="*.m3d";
			vf_CreateFileList(LD);
		}else
		if(vGameMode==gmSin){
			cfl_Level=0;

			cfl_Dir="save\\Single";
			cfl_LastDir.Clear();
			cfl_Mask="*.sav";			
			vf_CreateFileList(LD);

			cfl_Dir="records";
			cfl_LastDir.Clear();
			cfl_Mask="*.rec";			
			vf_CreateFileList(LD,true);

			cfl_Dir="save\\Single";
			cfl_LastDir.Clear();
			cfl_Mask="*.sav";
		}else
		if(vGameMode==gmCamp){
			//cfl_Level=0;
			//cfl_LastDir.Clear();
			cfl_Mask="*.sav";
			cfl_Level=0;
			_str BaseDir="save\\Camp\\";
			BaseDir+=vmSinglePlayerData.PlayerName;
			cfl_Dir=BaseDir;
			cfl_LastDir.Clear();
			vf_CreateFileList(LD);
			cfl_Level=-1;
			cfl_Dir="save\\Camp";
			cfl_LastDir.Clear();
			vf_CreateFileList(LD,true);
			cfl_Level=0;
			cfl_Dir=BaseDir;
			cfl_LastDir=BaseDir;
		}else{
			cfl_Level=0;
			cfl_Dir="save\\Multi";
			cfl_LastDir.Clear();
			cfl_Mask="*.sav";			
			vf_CreateFileList(LD);
		}
		CreateDirectory(cfl_Dir.pchar(),0);
		prev=-1;
	}
#endif _DEMO_
}
// cva_LS_LoadName
void cva_LS_LoadName::Init(SimpleDialog* SD){
	InputBox* IB=dynamic_cast<InputBox*>(SD);	
	if(IB){
		IB->StrMaxLen=32;
		IB->Enabled=false;		
        IB->_Str=&vls_FileName;
	}
}
// cva_LS_SaveName
void cva_LS_SaveName::Init(SimpleDialog* SD){
	InputBox* IB=dynamic_cast<InputBox*>(SD);	
	if(IB){
		IB->StrMaxLen=18;		
		IB->Enabled=true;
		IB->Active=true;
		IB->_Str=&vls_FileName;
	}
}
void cva_LS_SaveName::SetFrameState(SimpleDialog* SD){
	InputBox* IB=dynamic_cast<InputBox*>(SD);	
	if(IB&&!IB->_Str->isClear()){
		IB->_Str->Replace("\\","");
		IB->_Str->Replace("/","");
		IB->_Str->Replace(":","");
		IB->_Str->Replace(";","");
		IB->_Str->Replace("<","");
		IB->_Str->Replace(">","");
		IB->_Str->Replace("|","");
		IB->_Str->Replace("&","");
		IB->_Str->Replace("#","");
	}
}
int vMapLoading=100;
// cva_BR_PlMapLoading
void cva_BR_PlMapLoading::SetFrameState(SimpleDialog* SD){
	if(GameInProgress)return;
	GSets.SVOpt.RequiredMsPerFrame=30;
	SD->Visible=false;	
	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* I=g->GetCurrentPlayerInfo();
	int GetFileProgress(char* FileName,DWORD HashValue);
	PlayerInfo* PI=g->GetHostPlayerInfo();
	if(PI){
		vMapLoading=GetFileProgress(PI->MapName,PI->MapHashValue);
		if(vMapLoading==100){
			PlayerInfo* CPI=g->GetCurrentPlayerInfo();
			if(CPI){
				CPI->MapHashValue=PI->MapHashValue;
			}
		}
	}
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	if(VB){
		VB->State=0;
		VB->Visible=true;
	}
	if(I==g->PL_INFO+SD->ID&&vMapLoading<100){
		SD->Visible=true;
		if(PI){
			TextButton* TB=dynamic_cast<TextButton*>(SD);
			if(TB){
				_str txt;
				txt.print("%d%%",vMapLoading);
				TB->SetMessage(txt.pchar());
			}		
		}
		if(VB){
			if(vMapLoading==100){
				VB->State=1;
			}
		}		
	}
	ProgressBar* PB=dynamic_cast<ProgressBar*>(SD);
	if(PB&&PI){
		PB->Value=vMapLoading;
		PB->Visible=vMapLoading!=100;
	}
	DialogsDesk* DD=dynamic_cast<DialogsDesk*>(SD);
	if(DD){
		DD->Visible=vMapLoading!=100;
	}
}
// cva_BR_SavList
void cva_BR_SavList::SetFrameState(SimpleDialog* SD){
#ifndef _DEMO_
	va_ListDesk::SetFrameState(SD);
	SD->Enabled=v_MM_Host;
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	CurrentGameInfo* g=&GSets.CGame;	
	if(LD){
		if(v_MM_Host){
			PlayerInfo* I=g->GetCurrentPlayerInfo();
			if(I){
				if(LD->CurrentElement==-1){
					I->MapName[0]=0;					
				}else{
					//strcpy(I->MapName,LD->GetElement(LD->CurrentElement));
					strcpy(I->MapName,GetLDFileName());
					mInitRNDMapName=false;
				}
				//mMapPreview=I->MapName[0];
			}
		}else{
			LD->CurrentElement=-1;
			PlayerInfo* I=g->GetHostPlayerInfo();
			if(I){
				for(int i=0;i<LD->DSS.GetAmount();i++){
					if(strstr(I->MapName,LD->GetElement(i))){
						LD->CurrentElement=i;
						break;
					}					
				}				
			}
			static int t=GetTickCount()+3000;
			if(GetTickCount()>t){				
				t=GetTickCount()+3000;
				extern bool vls_ReInit;
				vls_ReInit=true;
			}
		}
		mMapPreview=false;
	}
#endif _DEMO_
}
bool CheckSaveVerison(char* FileName){
	ResFile ff1=RReset(FileName);	
	if(ff1!=INVALID_HANDLE_VALUE){
		int sig,lap;
		RBlockRead(ff1,&sig,4);
		if(sig=='FSAK'){
			RBlockRead(ff1,&sig,4);
			RBlockRead(ff1,&lap,4);
			if(sig==sfVersion){
				return true;
			}
		}
	}
	return false;
}
void cva_BR_SavList::Init(SimpleDialog* SD){
#ifndef _DEMO_
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		/*
		LD->DSS.Clear();
		TGSCFindInfo* FD=GSFILES.gFindFirst("Save\\*.sav");
		if(FD){
			LD->CurrentElement=0;			
			_str name;
			do{	
				name="Save\\";
				name+=FD->m_FileName;
				if(CheckSaveVerison(name.pchar())){
					name=FD->m_FileName;
					LD->AddElement(name);
				}
			}while(GSFILES.gFindNext(FD));
			//delete(FD);
		};
		*/
	}
#endif _DEMO_
}
// cva_PlayerInfoTab
void cva_PlayerInfoTab::SetFrameState(SimpleDialog* SD){
	TabButton* TB=dynamic_cast<TabButton*>(SD);
	if(TB){
		TB->Enabled=true;
		if(vGameMode!=gmSin){
			PlayerInfo* I=GSets.CGame.GetHostPlayerInfo();
			if(I){
				if(v_MM_Host){
					if(TB->Group=="Skirmish"){
						if(ROOMOPT[0]){
							TB->_Click();
						}
						if(TB->State==1){
							I->Page=0;
						}
					}else
					if(TB->Group=="Battles"){
						TB->Enabled=(ROOMOPT[0]==0);
						if(TB->State==1){
							I->Page=1;
						}
					}else
					if(TB->Group=="Load"){
						TB->Enabled=(ROOMOPT[0]==0);
						if(TB->State==1){
							I->Page=2;
						}
					}
				}else{
					TB->Enabled=false;
					if(I){			
						if(I->Page==0&&TB->Group=="Skirmish"){
							TB->_Click();
						}else 
						if(I->Page==1&&TB->Group=="Battles"){
							TB->_Click();				
						}else
						if(I->Page==2&&TB->Group=="Load"){
							TB->_Click();				
						}
					}
				}		
			}
		}
	}
}

void vfSetMapMask(char* Name){
	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* I=g->GetCurrentPlayerInfo();
	if(Name&&I){
		if(!strcmp(Name,"Battles")){
			vMapMask="Missions\\Battles\\*.m3d";
			I->Page=1;
		}else{
			vMapMask="Missions\\Skirmish\\*.m3d";
			I->Page=0;
		}
		vMapListInit=false;
	}
}
bool cva_BR_SetSkirBatl::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable&&v_MM_Host){
		vfSetMapMask(SD->Name.pchar());
	}
	return true;
}
void cva_BR_SetSkirBatl::SetFrameState(SimpleDialog* SD){	
	//TabButton* TB=dynamic_cast<TabButton*>(SD);
	SD->Enabled=v_MM_Host;
	if(!v_MM_Host){
		CurrentGameInfo* g=&GSets.CGame;
		PlayerInfo* I=g->GetCurrentPlayerInfo();
		PlayerInfo* SI=g->GetHostPlayerInfo();
		if(I&&SI&&I->Page!=SI->Page){
			if(SI->Page==0&&SD->Name=="Skirmish"){
				SD->_Click();
				vfSetMapMask(SD->Name.pchar());
			}else if(SI->Page==1&&SD->Name=="Battles"){
				SD->_Click();
				vfSetMapMask(SD->Name.pchar());
			}
		}
	}
}
// cva_BR_TabMap
bool cva_BR_TabMap::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable&&v_MM_Host){
		CurrentGameInfo* g=&GSets.CGame;
		PlayerInfo* I=g->GetCurrentPlayerInfo();
		if(I){
			if(SD->Name=="Map"){ 
				I->HillType=2;
			}else
			if(SD->Name=="Saved"){ 
				I->HillType=1;
			}else
			if(SD->Name=="Random"){ 
				I->HillType=0;
			}
		}
	}
	return true;
}
void cva_BR_TabMap::SetFrameState(SimpleDialog* SD){	
	//TabButton* TB=dynamic_cast<TabButton*>(SD);	
#ifdef _DEMO_	
	SD->Enabled = SD->Name=="Map";
	if(SD->Enabled){
		SD->_Click();
	}
#else
	SD->Enabled = v_MM_Host || vGameMode==gmSin;
#endif _DEMO_
	//if(!v_MM_Host){
		CurrentGameInfo* g=&GSets.CGame;
		PlayerInfo* I=g->GetCurrentPlayerInfo();
		PlayerInfo* SI=g->GetHostPlayerInfo();
		if(I&&SI){
#ifdef _DEMO_
			SI->HillType=2;			
#endif _DEMO_
			
			SI->HillType=2;

			if(SI->HillType==2&&SD->Name=="Map"){ 
				I->HillType=2;
				SD->_Click();
			}else
			if(SI->HillType==1&&SD->Name=="Saved"){ 
				I->HillType=1;
				SD->_Click();
			}else
			if(SI->HillType==0&&SD->Name=="Random"){ 
				I->HillType=0;
				SD->_Click();
			}
		}
	//}
	if(SD->Enabled){
		CurrentGameInfo* g=&GSets.CGame;
		PlayerInfo* I=g->GetCurrentPlayerInfo();
		if(I){
			bool p1=CheckRoomRage(1);
			bool p2=CheckRoomRage(2);
			bool p3=CheckRoomRage(3);
			if(p1||p2||p3)SD->Enabled=false;
			if(SD->Name=="Map"&&p1){
				I->HillType=2;
				SD->_Click();
			}
			if(SD->Name=="Saved"&&p3){
				I->HillType=1;
				SD->_Click();
			}
			if(SD->Name=="Random"&&p2){
				I->HillType=0;
				SD->_Click();
			}
		}
	}
}

void cva_BR_MapList::SetFrameState(SimpleDialog* SD){	
	Init(SD);
	va_ListDesk::SetFrameState(SD);
	SD->Enabled=false;
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	CurrentGameInfo* g=&GSets.CGame;
	PlayerInfo* I=g->GetCurrentPlayerInfo();
	if(LD&&I){		
		if(v_MM_Host||NPlayers==1){
			//
			vCurMiss=-1;
			//
			SD->Enabled=(I->Ready==0);
			if(LD->CurrentElement==-1&&LD->DSS.GetAmount()>0){
				LD->CurrentElement=0;
			}else{
				I->MapName[0]=0;
			}
			int rnd=0;
			char* NAME=GetSpecMap(rnd);
			if(NAME&&strcmp(NAME,"RANDOM")==0){
				/*
				if(LD->DSS.GetAmount()){
					LD->CurrentElement=rnd%LD->DSS.GetAmount();
				}else{
					LD->CurrentElement=0;
				}
				*/
				NAME=NULL;
			}
			if(NAME){
				strcpy(I->MapName,NAME); 
				LD->Visible=false;
			}else{				
				if(LD->CurrentElement!=-1){
					_str n;				
					n=vMapMask;
					n.Replace("*.m3d",LD->GetElement(LD->CurrentElement));
					char* Name=n.pchar();
					//sprintf(Name,dir,LD->GetElement(LD->CurrentElement));				
					if(strcmp(Name,I->MapName)){										
						strcpy(I->MapName,Name);
						strcpy(GSets.CGame.cgi_CurrentMap,Name);
						mInitRNDMapName=false;
						DWORD GetMapHash(char* Name);
						I->MapHashValue=GetMapHash(I->MapName);					
						//n=vMapMask;
						//n.Replace("*.m3d",Name);
						CreateNationalMaskForMap(Name);
					}
					//vls_Active=false;
				}			
			}
			mMapPreview=I->MapName[0];
			if(CheckHideMap()){
				mMapPreview=false;
				LD->Visible=false;
			}
		}else{
			LD->CurrentElement=-1;
			mMapPreview=false;
			PlayerInfo* SI=g->GetHostPlayerInfo();
			if(SI){
				static _str name;
				name=SI->MapName;
				name.Replace(mMP_Dir,"");
				LD->CurrentElement=LD->GetElement(name.pchar());
				if(LD->CurrentElement>=0){
					CreateNationalMaskForMap(SI->MapName);
					mMapPreview=true;
				}
			}
			static int t=GetTickCount()+3000;
			if(GetTickCount()>t){				
				t=GetTickCount()+3000;
				vMapListInit=false;
			}	
			if(CheckHideMap()){
				mMapPreview=false;
				LD->Visible=false;
			}
		}
		vls_Active=(LD->CurrentElement<0);
	}
}
void cva_BR_MapList::Init(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD&&(!SD->vm_Init||!vMapListInit)){
		vMapListInit=true;
		SD->vm_Init=true;		
		LD->DSS.Clear();
		CreateDirectory(mMP_Dir,0);
		if(vMapMask.pchar()[0]==0){
			vMapMask=mMP_Dir;
			vMapMask+="*.m3d";
		}
		TGSCFindInfo* FD=GSFILES.gFindFirst(vMapMask.pchar());
		if(FD){			
			_str name;
			do{				
				name=FD->m_FileName;
				LD->AddElement(name);
			}while(GSFILES.gFindNext(FD));
			//delete(FD);
		};
		LD->CurrentElement=-1;
	}
}

void cva_BR_RoSList::SetFrameState(SimpleDialog* SD)
{	
	Init(SD);
	va_ListDesk::SetFrameState(SD);
	SD->Enabled=false;
	ListDesk* LD = dynamic_cast<ListDesk*>(SD);
	CurrentGameInfo* g = &GSets.CGame;
	PlayerInfo* I = g->GetCurrentPlayerInfo();
	if(LD && I)
	{		
		if(v_MM_Host || NPlayers==1)
		{
			vCurMiss=-1;
			SD->Enabled=(I->Ready == 0);
			if(LD->CurrentElement == -1 && LD->DSS.GetAmount() > 0) LD->CurrentElement=0;
			else I->MapName[0]=0;
			int rnd = 0;
			char* NAME = GetSpecMap(rnd);
			if(NAME&&strcmp(NAME, "RANDOM") == 0) NAME=NULL;
			if(NAME)
			{
				strcpy(I->MapName, NAME); 
				LD->Visible = false;
			}
			else
			{				
				if(LD->CurrentElement != -1)
				{
					_str n;				
					n = vMapMaskRoS;
					n.Replace("*.m3d", LD->GetElement(LD->CurrentElement));
					char* Name = n.pchar();
					if(strcmp(Name, I->MapName))
					{										
						strcpy(I->MapName, Name);
						strcpy(GSets.CGame.cgi_CurrentMap, Name);
						mInitRNDMapName = false;
						DWORD GetMapHash(char* Name);
						I->MapHashValue = GetMapHash(I->MapName);					
						CreateNationalMaskForMap(Name);
					}
				}			
			}
			mMapPreview = I->MapName[0];
			if(CheckHideMap())
			{
				mMapPreview = false;
				LD->Visible = false;
			}
		}
		else
		{
			LD->CurrentElement = -1;
			mMapPreview = false;
			PlayerInfo* SI = g->GetHostPlayerInfo();
			if(SI)
			{
				static _str name;
				name = SI->MapName;
				name.Replace(mMP_DirRos, "");
				LD->CurrentElement = LD->GetElement(name.pchar());
				if(LD->CurrentElement >= 0)
				{
					CreateNationalMaskForMap(SI->MapName);
					mMapPreview = true;
				}
			}
			static int t = GetTickCount() + 3000;
			if(GetTickCount() > t)
			{				
				t = GetTickCount() + 3000;
				vMapRoSInit = false;
			}	
			if(CheckHideMap())
			{
				mMapPreview = false;
				LD->Visible = false;
			}
		}
		vls_Active=(LD->CurrentElement < 0);
	}
}
void cva_BR_RoSList::Init(SimpleDialog* SD)
{
	ListDesk* LD = dynamic_cast<ListDesk*>(SD);
	if(LD && (!SD->vm_Init || !vMapRoSInit))
	{
		vMapRoSInit = true;
		SD->vm_Init = true;		
		LD->DSS.Clear();
		CreateDirectory(mMP_DirRos, 0);
		if(vMapMaskRoS.pchar()[0] == 0)
		{
			vMapMaskRoS = mMP_DirRos;
			vMapMaskRoS += "*.m3d";
		}
		TGSCFindInfo* FD=GSFILES.gFindFirst(vMapMaskRoS.pchar());
		if(FD)
		{			
			_str name;
			do
			{				
				name = FD->m_FileName;
				LD->AddElement(name);
			}
			while(GSFILES.gFindNext(FD));
		};
		LD->CurrentElement=-1;
	}
}
// cva_BR_ChatDesk
void cva_BR_ChatDesk::Init(SimpleDialog* SD){
	ChatDesk* CD=dynamic_cast<ChatDesk*>(SD);
	if(CD&&!SD->vm_Init){
		SD->vm_Init=true;
		CD->DSS.Clear();
	}
}
// cva_BR_ChatInput
_str ChatInput;
bool EnterChat=false;
void cva_BR_ChatInput::SetFrameState(SimpleDialog* SD){
	InputBox* IB=dynamic_cast<InputBox*>(SD);
	if(IB){
		IB->StrMaxLen=250;
		IB->Active=true;
		IB->_Str=&ChatInput;
		//
		IB->_Str->Replace("\\","");
		IB->_Str->Replace("{","[");
		IB->_Str->Replace("}","]");
		//
		if(!ChatInput.pchar()[0]) ChatInput="";
		ChatDesk* CD=Desk.Get();
		if(!CD)for(int i=IB->ParentDS->DSS.GetAmount()-1;i>=0;i--){
			ChatDesk* cd=dynamic_cast<ChatDesk*>(IB->ParentDS->DSS[i]);
			if(cd&&IB->Name==cd->Name){
				CD=cd;
				break;
			}
		}
		if(CD){
			if(EnterChat&&IB->ParentDS){
				EnterChat=false;
				CurrentGameInfo* g=&GSets.CGame;
				PlayerInfo* I=g->GetCurrentPlayerInfo();
				if(I){				
					_str txt;
					txt.print("{C FFffcb2b}  %s{C FFefd7a7}: %s",I->name,ChatInput.pchar());
					CD->AddElement(txt.pchar());
					SendChat(ChatInput.pchar(),GetKeyState(VK_SHIFT)&0x8000);
					ChatInput.Clear();
				}
			}
			if(CHATSTRING[0]){
				char* I=GetPlNameByID(CHATDPID);
				_str txt;
				txt="{C FFefe7d1}  ";
				if(I){					
					txt+=I;
				}else{
					txt+="UNKNOWN";
				}
				txt.print("{C FFefd7a7}: %s",CHATSTRING);
				CD->AddElement(txt.pchar());
				CHATSTRING[0]=0;
			}
		}
	}
}
// cva_BR_ChatEnter
bool cva_BR_ChatEnter::LeftClick(SimpleDialog* SD){
	if(ChatInput.pchar()[0]) EnterChat=true;
	return true;
}
// cva_MU_NameInput
_str PlayerName;
void cva_MU_NameInput::Accept(SimpleDialog* SD){
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	if(VB){
	
	}else{
		vmSinglePlayerData.PlayerName=PlayerName;
		int n=vmSinglePlayerData.NDoneMission.GetAmount();
		for(int i=0;i<n;i++){
			vmSinglePlayerData.NDoneMission[i]=0;
		}	
		vmPlayerData.AddNewAka(PlayerName.pchar());
		vmPlayerData.Write();
	}
}
static bool NameInputEditing=false;
void cva_MU_NameInput::Init(SimpleDialog* SD){
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	if(VB){
	}
	InputBox* IB=dynamic_cast<InputBox*>(SD);
	if(IB){
		if(IB->Enabled){
			static char* PN=GetTextByID("#NewPlayerName");
			PlayerName=PN;
			IB->Selected=0;
			IB->CursPos=strlen(PN);
			NameInputEditing=true;
		}else{
			PlayerName=vmSinglePlayerData.PlayerName;
		}
	}
}
void cva_MU_NameInput::SetFrameState(SimpleDialog* SD){
	if(!SD->Visible)
		return;
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	if(VB){
		if(!NameInputEditing){
			VB->SetMessage(vmSinglePlayerData.PlayerName.pchar());
			PlayerName=vmSinglePlayerData.PlayerName;
		}
		NameInputEditing=false;
	}
	InputBox* IB=dynamic_cast<InputBox*>(SD);
	if(IB){
		IB->Active=IB->Enabled;		
		IB->_Str=&PlayerName;
		IB->StrMaxLen=17;
		SimpleDialog* sdA=sdAccept.Get();
		if(sdA){
			sdA->Enabled=IB->_Str&&!IB->_Str->isClear()||IB->Str&&IB->Str[0];
		}
		NameInputEditing=true;
	}	
}
// cva_MU_NickInput
void cva_MU_NickInput::SetFrameState(SimpleDialog* SD){
	InputBox* IB=dynamic_cast<InputBox*>(SD);
	if(IB){
		extern bool vManualServer;
		if(!vManualServer){
			IB->Active=IB->Enabled;
		}
		IB->_Str=&vmPlayerData.PlayerNick;
		IB->StrMaxLen=17;
		//
		IB->_Str->Replace("\\"," ");
		IB->_Str->Replace("{"," ");
		//
		void Translit(char* s);
		Translit(IB->_Str->pchar());
		//
		int p=0;
		while(IB->_Str->pchar()[p]==' '){
			p++;
		}
		if(p){
			strcpy(IB->_Str->pchar(),IB->_Str->pchar()+p);
		}
		//
		PlayerName=vmPlayerData.PlayerNick;
	}
}
//
CEXPORT void SetInetPlayerName(char* Name){
	if(Name&&Name[0]){
		PlayerName=Name;
		strcpy(PlName,Name);
	}
}
//
char* GetLDFileName(){
	static _str FN;	
	/*
	_str dir;
	dir=cfl_Dir;
	dir+="\\";
	FN=vls_FileName;
	FN.Replace(dir.pchar(),"");
	if(strstr(vls_FileName.pchar(),dir.pchar())==0){
	strcat(GameName,dir.pchar());
	}
	strcat(GameName,vls_FileName.pchar());
	*/
	FN=cfl_Dir;
	FN+="\\";
	FN+=vls_FileName;
	return FN.pchar();
}
//
_str ExDialogs;
bool rDelete=false;
//cva_DeleteButton
bool cva_DeleteButton::LeftClick(SimpleDialog* SD){	
	rDelete=Delete;
	v_MainMenu.ModalDesk=ExDialogs;
	return true;
}
// cva_DeleteSaveFile
void cva_DeleteSaveFile::SetFrameState(SimpleDialog* SD){
	if(vls_Active){
		SD->Enabled=rdEnable;		
		if(!SD->Enabled){
			rDelete=false;
		}
		if(rDelete){
			DeleteFile(GetLDFileName());
			vls_ReInit=true;
			rDelete=false;
		}
	}		
}
bool cva_DeleteSaveFile::LeftClick(SimpleDialog* SD){
	ExDialogs=v_MainMenu.ModalDesk;
	v_MainMenu.ModalDesk=ModalDesk;
	return true;
}
// cva_LoadInGame_Accept
void ProcessGLoadMap();
void ProcessGSaveMap();
//
void cva_LoadInGame_Accept::SetFrameState(SimpleDialog* SD){
	if(vls_Active){
		SD->Enabled=!vls_FileName.isClear();
	}	
}
bool cva_LoadInGame_Accept::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable&&!vls_FileName.isClear()&&vls_Active){
		strcpy(GameName,GetLDFileName());
		if(vGameMode==gmEdit){			
			ProcessGLoadMap();
		}else{
			extern bool vmVictory;
			vmVictory=false;
			//GSets.CGame.UnitTransform=false;
			vLoadFile=GameName;
			if(LoadInGame())return true;
			if(!InGame){
				PlayGame();
				if(vGameMode==gmCamp){
					void vmCheckVictory();
					vmCheckVictory();
				}
				v_MainMenu.Cross=1;
				v_MainMenu.CrossTime=0;
				v_MainMenu.CrossSD=NULL;
			}
			GameExit=false;
		}
	}	
	return true;
}
// cva_SaveInGame_Accept
void WriteSaveFile(char* FileName){
	CmdSaveNetworkGame(MyNation,GetRealTime(),FileName);
	ContinueGame=true;
	//_str H;
	//H.print("%s - %s",FileName,GetTextByID("INTF_CROOM_GS2"));
	//AssignHint1(H.pchar(),200);
}
//
_str ConfirmSaveFileName;
bool cva_ConfirmSaveFileName::LeftClick(SimpleDialog* SD){
	WriteSaveFile(ConfirmSaveFileName.pchar());
	return true;
}
//
void cva_SaveInGame_Accept::SetFrameState(SimpleDialog* SD){
	SD->Enabled=!vls_FileName.isClear();
}
bool cva_SaveInGame_Accept::LeftClick(SimpleDialog* SD){
	//cva_MM_Accept::LeftClick(SD);
	if(v_MainMenu.Enable&&vls_FileName.pchar()){
		_str dir;
		dir=cfl_Dir;
		dir+="\\";
		GameName[0]=0;
		if(strstr(vls_FileName.pchar(),dir.pchar())==0){
			strcat(GameName,dir.pchar());
		}
		strcat(GameName,vls_FileName.pchar());
		if(vGameMode==gmEdit){
			ProcessGSaveMap();
			v_MainMenu.Exit=true;
		}else{
			//if(vls_FileName.pchar()[0]=='s') vls_FileName.pchar()[0]='S';
			//CmdSaveNetworkGame(MyNation,GetRealTime(),vls_FileName.pchar());
			bool CheckIfFileExists(char* Name);
			if(CheckIfFileExists(GameName)&&!SD->Name.isClear()){
				ConfirmSaveFileName=GameName;
				v_MainMenu.SetModalDesk(SD->Name.pchar());
			}else{
				WriteSaveFile(GameName);
				v_MainMenu.Exit=true;
			}
		}
	}	
	return true;
}
// 
tpMessageArray* vTASKS;
tpMessageArray* vTALKS;
tpMessageArray* vHINTS;
//
bool f1_obj_init=false;
DString f1_text;
void SetF1Text(char* Help){
	if(Help){
		f1_obj_init=false;
		f1_text=Help;
	}
}
// cva_F1_Start
bool cva_F1_Start::LeftClick(SimpleDialog* SD){
	/*
	if(InEdit){
		SD->SetHint("Press Cntr+ to enter help in EditorMode");
		if(!(GetKeyState(VK_CONTROL)&0x8000)){
			return true;
		}		
	}
	*/
	if(v_MainMenu.Enable){
		if(vGetHelp){
			vGetHelp(&vTASKS,&vTALKS,&vHINTS);			
		}
		if(vTASKS&&vTASKS->GetAmount()==0) vTASKS=NULL;
		if(vTALKS&&vTALKS->GetAmount()==0) vTALKS=NULL;
		if(vHINTS&&vHINTS->GetAmount()==0) vHINTS=NULL;
		if(!(vTASKS||vTALKS||vHINTS)){			
			CreateMissText(f1_text);
			f1_obj_init=false;
		}
	}
	return cva_MM_StartInGame::LeftClick(SD);
}
extern _str vm_F1_VictoryCond;
extern char GlobalTextMouseOverCommand[256];
extern char GlobalTextCommand[256];
// cva_F1_Object
void cva_F1_Object::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		if(!vm_F1_VictoryCond.isClear()){
			TB->SetMessage(vm_F1_VictoryCond.pchar());
		}else{
			if(!f1_obj_init&&f1_text.pchar()){
				TB->SetMessage(f1_text.pchar());
				f1_obj_init=true;
				GlobalTextMouseOverCommand[0]=0;
			}
			if(GlobalTextMouseOverCommand[0]){
				SetMissionPulseZone(GlobalTextMouseOverCommand[0]);
				GlobalTextMouseOverCommand[0]=0;			
			}
			if(SCENINF.ScenaryHandler)SCENINF.ScenaryHandler();
		}
	}
}
// cva_F1_VictoryCond
_str vm_F1_VictoryCond;
void cva_F1_VictoryCond::SetFrameState(SimpleDialog* SD){
	return;
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		if(!vm_F1_VictoryCond.isClear()){
			TB->SetMessage(vm_F1_VictoryCond.pchar());
		}else{
			TB->SetMessage("");
		}		
	}
}
// cva_H_TaskList
void cva_H_TaskList::Init(SimpleDialog* SD){
	TabDesk* TD=dynamic_cast<TabDesk*>(SD);
	if(TD&&!SD->vm_Init&&vTASKS){		
		SD->vm_Init=true;		
		TD->DSS.Clear();
		int y=0;
		for(int i=0;i<vTASKS->GetAmount();i++){
			CSingleMessage* SM=(*vTASKS)[i];
			if(SM->Visible){
				TextButton* TB=new TextButton;
				TB->ActiveFont=&BlackFont;
				TB->PassiveFont=&BlackFont;
				TB->DisabledFont=&BlackFont;
				_str Mess;				
				char color[32];
				sprintf(color,"{C %x}",SM->Color);
				Mess=color;
				//Mess+=SM->TalkerID;
				//Mess+=": ";
				Mess+=GetTextByID(SM->TextID.pchar());
				//TB->MaxWidth=460;
				TB->SetMessage(Mess.pchar(),460);
				TB->Sety(y);
				y+=TB->GetHeight();				
				TD->DSS.Add(TB);
			}
		}
	}
}
// cva_H_TalkList
void cva_H_TalkList::Init(SimpleDialog* SD){
	TabDesk* TD=dynamic_cast<TabDesk*>(SD);
	if(TD&&!SD->vm_Init&&vTASKS){
		SD->vm_Init=true;
		TD->DSS.Clear();
		int y=0;
		for(int i=0;i<vTALKS->GetAmount();i++){
			CSingleMessage* SM=(*vTALKS)[i];
			if(SM->Visible){
				TextButton* TB=new TextButton;
				TB->ActiveFont=&BlackFont;
				TB->PassiveFont=&BlackFont;
				TB->DisabledFont=&BlackFont;
				_str Mess;				
				char color[32];
				sprintf(color,"{C %x}",SM->Color);
				Mess=color;
				Mess+=GetTextByID(SM->TalkerID.pchar());
				Mess+=": ";
				Mess+=GetTextByID(SM->TextID.pchar());
				int l,t,r,b;
				TD->GetMargin(l,t,r,b);
				//TB->MaxWidth=TD->GetWidth()-l-r;
				TB->SetMessage(Mess.pchar(),TD->GetWidth()-l-r);
				TB->Sety(y);
				y+=TB->GetHeight();				
				TD->DSS.Add(TB);
			}
		}
	}
}
// cva_H_HintList
void cva_H_HintList::Init(SimpleDialog* SD){
	TabDesk* TD=dynamic_cast<TabDesk*>(SD);
	if(TD&&!SD->vm_Init&&vTASKS){		
		SD->vm_Init=true;		
		TD->DSS.Clear();
		int y=0;
		for(int i=0;i<vHINTS->GetAmount();i++){
			CSingleMessage* SM=(*vHINTS)[i];
			if(SM->Visible){
				TextButton* TB=new TextButton;
				TB->ActiveFont=&BlackFont;
				TB->PassiveFont=&BlackFont;
				TB->DisabledFont=&BlackFont;
				_str Mess;				
				char color[32];
				sprintf(color,"{C %x}",SM->Color);
				Mess=color;
				//Mess+=SM->TalkerID;
				//Mess+=": ";
				Mess+=GetTextByID(SM->TextID.pchar());
				//TB->MaxWidth=460;
				TB->SetMessage(Mess.pchar(),460);
				TB->Sety(y);
				y+=TB->GetHeight();				
				TD->DSS.Add(TB);
			}
		}
	}
}
// cva_SP_in_Pic
cva_SP_in_Pic::cva_SP_in_Pic(){
    Lx=44;
    Ly=67;
}
void cva_SP_in_Pic::OnDraw(SimpleDialog* SD){
    GPPicture* GP=(GPPicture*)SD->ParentDS;
    if(GP&&GP->ID<NATIONS->NMon){		
        NewMonster* NM=NATIONS->Mon[GP->ID]->newMons;
        GP=(GPPicture*)SD;
        _str hint;
        hint=NewMonster_GetMessage(NM);
        GP->SetHint(&hint);
        if(NM->ExIcon.Icons.GetAmount()){            
            NM->ExIcon.Draw(SD->x,SD->y,Lx,Ly,MyNation);	//40,40 - must change
        }
    }
}
void cva_SP_in_Pic::SetFrameState(SimpleDialog* SD){
	GPPicture* GP=(GPPicture*)SD->ParentDS;
	if(GP&&GP->ID<NATIONS->NMon){		
		NewMonster* NM=NATIONS->Mon[GP->ID]->newMons;
		GP=(GPPicture*)SD;
		_str hint;
		hint=NewMonster_GetMessage(NM);
        GP->SetHint(&hint);
        SD->Visible=true;
        if(NM->ExIcon.Icons.GetAmount()){                        
            GP->FileID=0xFFFF;
            GP->SpriteID=-1;
        }else{		    
		    GP->SetFileID(&NM->IconFileID);
		    GP->SetSpriteID(NM->IconID);
        }
	}
}
// cva_SP_in_Amount
void cva_SP_in_Amount::SetFrameState(SimpleDialog* SD){
	GPPicture* GP=(GPPicture*)SD->ParentDS;
	if(GP&&GP->ID<NATIONS->NMon){		
		//NewMonster* NM=NATIONS->Mon[GP->ID]->newMons;
		TextButton* TB=(TextButton*)SD;
		_str txt;
		//txt.print("%d/%d",GP->UserParam&0xFFFF,GP->UserParam>>16);		
		txt.print("%d",GP->UserParam&0xFFFF);		
		TB->SetMessage(txt.pchar());
	}
}
// cva_SP_in_AmountMax
void cva_SP_in_AmountMax::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&SP&&SP->OB){
		AdvCharacter* ADC=SP->OB->MoreCharacter;		
		int Max=ADC->MaxInside+SP->OB->AddInside;
		if(Max>0){
			_str txt;
			txt.print("%d/%d",SP->OB->NInside,Max);		
			TB->SetMessage(txt.pchar());
			TB->Visible=true;
		}
	}
}
// cva_SP_in_Box
void cva_SP_in_Box::SetFrameState(SimpleDialog* SD){
	//SD->Visible=false;
}
bool cva_SP_in_Box::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if( SP && NI==SP->NI && SP->OB->DestX<0 ){
		CmdLeaveMine(SP->NI,SD->ID);
	}
	return true;
}

//cva_SP_SetRally
extern bool SetDestMode;
void cva_SP_SetRally::SetFrameState(SimpleDialog* SD){
	SimpleDialog* sd=SD;
	if( Child && SD->DSS.GetAmount() )
		sd=SD->DSS[0];
	VitButton* VB=dynamic_cast<VitButton*>(sd);
	if(VB){
		VB->State=2;
		SD->Enabled=false;
	}
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if( SP && NI==SP->NI /*&& SP->OB->Ref.General->CanDest*/ ){
		if( SP->OB->newMons->ConcPtX.GetAmount() && !SP->OB->newMons->NewParams.ControlButtonsDisable ){
			if(VB){
				if(SetDestMode)
					VB->State=1;
				else
					VB->State=0;
				SD->Enabled=true;
			}
			SD->Visible=true;
		}else if( SP->OB->NewBuilding ){
			SD->Visible=true;
		}
	}
}
bool cva_SP_SetRally::LeftClick(SimpleDialog* SD){
	SetDestMode=true;
	return true;
}

//cva_SP_Destroy
void cva_SP_Destroy::SetFrameState(SimpleDialog* SD){
	SimpleDialog* sd=SD;
	if( Child && SD->DSS.GetAmount() )
		sd=SD->DSS[0];
	VitButton* VB=dynamic_cast<VitButton*>(sd);
	if(VB){
		VB->State=2;
		SD->Enabled=false;
	}
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if( SP && NI==SP->NI ){
		if(!SP->OB->newMons->NewParams.ControlButtonsDisable){
			if(VB){
				VB->State=0;
				SD->Enabled=true;
			}
			SD->Visible=true;
		}else if( SP->OB->NewBuilding ){
			SD->Visible=true;
		}
	}
}
bool cva_SP_Destroy::LeftClick(SimpleDialog* SD){
	CmdDie(MyNation);
	return true;
}

// cva_SP_LeaveAll_Box
void cva_SP_LeaveAll_Box::SetFrameState(SimpleDialog* SD){
	SimpleDialog* sd=SD;
	if( Child && SD->DSS.GetAmount() )
		sd=SD->DSS[0];
	VitButton* VB=dynamic_cast<VitButton*>(sd);
	if(VB){
		VB->State=2;
		SD->Enabled=false;
	}
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if( SP && NI==SP->NI ){
		if( SP->OB->newMons->MaxInside && !SP->OB->newMons->NewParams.ControlButtonsDisable ){
			if(VB){
				VB->State=0;
				SD->Enabled=true;
			}
			SD->Visible=true;
		}else if( SP->OB->NewBuilding ){
			SD->Visible=true;
		}
	}
}
bool cva_SP_LeaveAll_Box::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(SP&&NI==SP->NI){
		vui_IS_LeaveMineAll Com;
		Com.Data.ObjID=SP->OB->Index;
		Com.Data.ObjSN=SP->OB->Serial;
		Com.InterfAction();
	}
	return true;
}
//
void cva_Inf_ShowExperience::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		TextButton* TB=dynamic_cast<TextButton*>(SD);
		if(TB){
            OneObject* OB=SP->OB;
			if(OB){
				char x[128];
				sprintf(x,"Exp:%d",OB->newMons->Expa);
				TB->SetMessage(x);
				TB->Visible=true;
			}
		}
	}	
}
void cva_Inf_ShowLevel::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		ProgressBar* TB=dynamic_cast<ProgressBar*>(SD);
		if(TB){
			OneObject* OB=SP->OB;
			HeroVariableStorage* S = DetectHero(OB);
			if(S){
				TB->MaxValue=S->ExperienceToNextLevel;
				TB->Value=S->Experience;									
				char x[128];
				sprintf(x,"%d",S->Level);
				TB->Message=x;
				TB->Visible=true;									
			}
			//if(OB&&OB->ActiveAbility){
			//	int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
			//	for(int i=0;i<n;i++)
			//	{
			//		if(OB->ActiveAbility->ActiveAbilities[i]->Type==7)//HeroVariableStorage
			//		{
			//			HeroVariableStorage* S = (HeroVariableStorage*)OB->ActiveAbility->ActiveAbilities[i];
			//			if(S){
			//				TB->MaxValue=S->ExperienceToNextLevel;
			//				TB->Value=S->Experience;									
			//				char x[128];
			//				sprintf(x,"%d",S->Level);
			//				TB->Message=x;
			//				TB->Visible=1;									
			//			}
			//			return;
			//		}
			//	}						
			//}
		}		
	}	
}
void cva_Inf_ShowMana::SetFrameState(SimpleDialog* SD){
	SD->Visible=0;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		ProgressBar* TB=dynamic_cast<ProgressBar*>(SD);
		if(TB){
			if(ImNSL[MyNation]){
				word MID=ImSelm[MyNation][0];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB){
						TB->MaxValue=OB->MaxMana;
						TB->Value=OB->Mana;
						char x[128];
						sprintf(x,"%d/%d",OB->Mana,OB->MaxMana);
						TB->Message=x;
						TB->Visible=TB->MaxValue>0;
						if(OB->MaxMana>=1000){
							TB->Font=GetFont("WindowText");
						}else{
							TB->Font=GetFont("WindowText2");
							if(TB->Font==NULL){
								TB->Font=GetFont("WindowText");
							}
						}
					}
				}
			}
		}		
	}	
}
void cva_Inf_BrigExp_Rome::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_brigade){
		//ProgressBar* TB=dynamic_cast<ProgressBar*>(SD);
		GPPicture* GP=dynamic_cast<GPPicture*>(SD);
		if(GP){
			//TB->Visible=true;
			vui_BrigInfo* BI=&SP->Inf.Brigade;
			Brigade* BR=CITY[SP->NI].Brigs+BI->BrigID;			
			float kills;
			int exp=BR->GetSkillStatus(kills);			
			//
			static int w=GP->GetWidth();
			DialogsDesk* DD=(DialogsDesk*)SD->ParentDS;
			if(DD){
				GP->Visible=true;
				DD->SetWidth(w*(exp+1));
			}else{
				GP->Visible=exp>=Level;
			}
		}
	}	
}
void cva_Inf_ShowRomeMorale::SetFrameState(SimpleDialog* SD){
	SD->Visible=0;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		ProgressBar* TB=dynamic_cast<ProgressBar*>(SD);
		if(TB){	
			if(SP->Sort==ve_UT_hero){
				TB->Visible=true;
				int n = SP->OB->ActiveAbility->ActiveAbilities.GetAmount();						
				for(int i=0;i<n;i++){
					ActiveRomeHero* Hr = dynamic_cast<ActiveRomeHero*> (SP->OB->ActiveAbility->ActiveAbilities[i]);
					if(Hr){
						//p->Visible=(Hr->GetExperienceToNextLevel()-*Hr->Ex)<=0;
						int Exp0=0;
						int Exp=*Hr->Ex/100;
						int Exp1=Hr->GetExperienceToNextLevel();
						TB->MaxValue=Exp1-Exp0;
						TB->Value=Exp-Exp0;
						char x[128];
						sprintf(x,"%d/%d",Exp,Exp1);
						TB->Message=x;
						static char* RH=GetTextByID("#GM_HeroExp");
						TB->SetHint(RH);
					}
				}
			}else
			if(EngSettings.MoraleType==1){
				if(ImNSL[MyNation]){
					word MID=ImSelm[MyNation][0];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB&&OB->BrigadeID!=0xFFFF){
							Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
							TB->Visible=1;
							TB->MaxValue=(BR->NMemb-NBPERSONAL)*2;
							TB->Value=BR->Morale/10000;
							char x[128];
							sprintf(x,"%d/%d",BR->Morale/10000,BR->NMemb-NBPERSONAL);
							TB->Message=x;
							TB->Visible=TB->MaxValue>0;
							static char* RH=GetTextByID("#GM_Morale");
							TB->SetHint(RH);
						}
					}
				}
			}
		}		
	}	
}
void cva_Inf_ShowSpeed::SetFrameState(SimpleDialog* SD){
	SD->Visible=0;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		TextButton* TB=dynamic_cast<TextButton*>(SD);
		if(TB){
			if(ImNSL[MyNation]){
				word MID=ImSelm[MyNation][0];
				if(MID!=0xFFFF){
                    OneObject* OB=Group[MID];
					if(OB){
						char x[128];
						sprintf(x,"Speed:%d",OB->newMons->MotionDist*OB->MoreCharacter->Speed/100);
						TB->SetMessage(x);
						TB->Visible=true;
						return;
					}
				}
			}
		}		
	}	
}
void cva_Inf_ShowStamina::SetFrameState(SimpleDialog* SD){
	SD->Visible=0;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		ProgressBar* TB=dynamic_cast<ProgressBar*>(SD);
		if(TB){
			if(ImNSL[MyNation]){
				word MID=ImSelm[MyNation][0];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB){
						TB->MaxValue=100000;
						TB->Value=OB->GetTired;
						char x[128];
						sprintf(x,"%d%%",OB->GetTired/1000);
						TB->Message=x;
						TB->Visible=TB->MaxValue>0;                        
					}
				}
			}
		}		
	}	
}
void cva_Inf_ShowAttSpeed::SetFrameState(SimpleDialog* SD){
	SD->Visible=0;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		TextButton* TB=dynamic_cast<TextButton*>(SD);
		if(TB){
			if(ImNSL[MyNation]){
				word MID=ImSelm[MyNation][0];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB){
						char x[128]="AtSpeed:";
						bool OK=false;
						for(int i=0;i<2;i++){
							float GetAttSpped(OneObject* OB,int AttType);
							float as=GetAttSpped(OB,i);
							if(as>0.0f){
								if(i>0)strcat(x,", ");								
								sprintf(x+strlen(x),"%.2f",GetAttSpped(OB,i));
								OK=true;
							}
						}
						if(OK){
							TB->SetMessage(x);
							TB->Visible=true;
						}
					}
				}
			}
		}		
	}	
}
//
bool vCurProfDel=false;
CPlayerSAVE* Profiles=NULL;
CPlayerSAVE_STRUCT* CurPlayer=NULL;
// cva_ProfList
void cva_ProfList::SetFrameState(SimpleDialog* SD){	
	SD->Enabled=!vCurProfDel;
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD&&Profiles){
		if(LD->DSS.GetAmount()!=Profiles->m_pPlayer.GetAmount()||Profiles->m_pPlayer.GetAmount()==1) SD->vm_Init=false;
		Init(SD);
		if(SD->vm_Init){
			if(LD->CurrentElement!=-1){			
				CurPlayer=Profiles->m_pPlayer[LD->CurrentElement];
			}else{
				//CurPlayer=NULL;
				//v_MainMenu.StartDS("AddProfile");
				//if(CurPlayer) v_MainMenu.CloseDS();
			}
		}
	}	
	va_ListDesk::SetFrameState(SD);
}
void cva_ProfList::Init(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD&&!SD->vm_Init&&Profiles){	
		SD->vm_Init=true;		
		LD->DSS.Clear();		
		LD->CurrentElement=-1;
		int n=Profiles->m_pPlayer.GetAmount();
		if(n>0){
			for(int i=0;i<n;i++){
				CPlayerSAVE_STRUCT* Pl=Profiles->m_pPlayer[i];
				if(Pl){
					_str n;
					n=Pl->m_chName;
					LimitString(n.pchar(),&BlackFont,SD->GetWidth()-20);
					LD->AddElement(n.pchar());					
				}
			}
			LD->CurrentElement=0;
		}
	}
}
void cva_ProfList::Accept(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD&&SD->vm_Init&&Profiles&&LD->CurrentElement>=0&&LD->CurrentElement<Profiles->m_pPlayer.GetAmount()){
		if(LD->CurrentElement>0){
			CurPlayer=new CPlayerSAVE_STRUCT;
			CPlayerSAVE_STRUCT* dp=Profiles->m_pPlayer[LD->CurrentElement];
			dp->Copy(CurPlayer);
			delete dp;
			Profiles->m_pPlayer.Del(LD->CurrentElement,1);
			Profiles->AddProfile(CurPlayer);			
		}else{
			CurPlayer=Profiles->m_pPlayer[0];
		}		
	}
}
// cva_SPD_CampMessageCheck
bool cva_SPD_CampMessageCheck::LeftClick(SimpleDialog* SD){	
	SimpleDialog* sdB=Button.Get();
	if(sdB&&Profiles){
		if(Profiles->m_pPlayer.GetAmount()==0||Profiles->m_pPlayer[0]->DisableTutorialMessage){
			sdB->v_Actions.LeftClick(sdB);
			v_MainMenu.ModalDesk=Name;
		}else{
			Profiles->m_pPlayer[0]->DisableTutorialMessage=true;			
			Profiles->SaveXML();
		}		
	}else{
		if(vmSinglePlayerData.CampMessageCheck || !EngSettings.vInterf.ShowPreCampMessage){
			//SD->v_Actions.LeftClick(SD);
			v_MainMenu.ModalDesk=Name;
		}
	}
	return true;
}
// cva_ProfAdd_Cancel
void cva_ProfAdd_Cancel::SetFrameState(SimpleDialog* SD){
	if(Profiles){
		SD->Enabled=true;//Profiles->m_pPlayer.GetAmount()!=0;
	}
}
/*
bool cva_ProfAdd_Cancel::LeftClick(SimpleDialog* SD){
	return cva_MM_Cancel::LeftClick(SD);
}
*/
// 
CPlayerSAVE_STRUCT vNewProf;
void vNewProfInit(){
	vNewProf.Init();
}
// cva_ProfAdd_Race
void cva_ProfAdd_Race::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB&&!SD->vm_Init){
		SD->vm_Init=true;
		CB->Clear();
		_str Line;
		int NG=GlobalAI.NAi;
		for(int j=0;j<NG;j++){
			if(j>=GlobalAI.NComp) Line="{C C0665F57}"; else Line="";
			Line+=GlobalAI.Ai[j].Message;
			CB->AddLine(Line.pchar());
		};
		CB->CurLine=0;
	}		
}
void cva_ProfAdd_Race::SetFrameState(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){
		Init(CB);
		if(CB->CurLine>=GlobalAI.NComp) CB->CurLine=0;
		vNewProf.m_iNation=CB->CurLine;
	}
}
// cva_ProfAdd_Diff
void cva_ProfAdd_Diff::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB&&!SD->vm_Init){
		SD->vm_Init=true;
		CB->Clear();
		_str Line;
		int def=0;
		for(int i=0;i<GlobalAI.Diff.GetAmount();i++){
			vsDiffLevel* dl=GlobalAI.Diff+i;
			if(dl->Enable){				
				Line=dl->Name;
			}else{
				Line.Clear();
				Line.print("{C C0665F57}%s{C}",dl->Name); //{CG}
			}
			
			CB->AddLine(Line.pchar());
			if(dl->Enable==2&&def==0) def=i;
		}
		CB->CurLine=def;
	}		
}
int difT=0;
void cva_ProfAdd_Diff::SetFrameState(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){		
		Init(CB);
		//
		if(GetTickCount()-difT>3000){			
			CB->CurLine=0;
		}
		difT=GetTickCount();
		//
		//if(CB->CurLine!>1)
		if(!GlobalAI.Diff[CB->CurLine].Enable){
			for(int i=0;i<GlobalAI.Diff.GetAmount();i++){
				vsDiffLevel* dl=GlobalAI.Diff+i;
				if(dl->Enable){
					CB->CurLine=i;
					break;					
				}
			}
		}		
		if(Name!="Tutorial"){
			vNewProf.m_iDifficulty=CB->CurLine;
		}
		if(CurPlayer){
			CurPlayer->c_Difficulty=CB->CurLine;
		}
	}
}
// cva_ProfAdd_Name
void cva_ProfAdd_Name::Init(SimpleDialog* SD){
	InputBox* IB=dynamic_cast<InputBox*>(SD);
	if(IB&&!SD->vm_Init){		
		SD->vm_Init=true;
		vNewProf.m_chName="";
		IB->_Str=&vNewProf.m_chName;
		IB->Active=true;
		IB->StrMaxLen=17;
	}		
}
void cva_ProfAdd_Name::SetFrameState(SimpleDialog* SD){
	InputBox* IB=dynamic_cast<InputBox*>(SD);
	if(IB){
		Init(IB);
		if(IB->_Str&&!IB->_Str->isClear()){
			IB->_Str->Replace("\\"," ");
			IB->_Str->Replace("{"," ");
			int p=0;
			while(IB->_Str->pchar()[p]==' '){
				p++;
			}
			if(p){
				strcpy(IB->_Str->pchar(),IB->_Str->pchar()+p);
			}
		}
	}
}
// cva_ProfAdd_Accept
void cva_ProfAdd_Accept::SetFrameState(SimpleDialog* SD){
	SD->Enabled=vNewProf.m_chName.pchar()&&vNewProf.m_chName.pchar()[0];
}
bool cva_ProfAdd_Accept::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		ParentFrame* pf=SD;
		while(pf->ParentDS) pf=(ParentFrame*)pf->ParentDS;
		pf->vm_ActionsAccept();
		CurPlayer=new CPlayerSAVE_STRUCT;
		vNewProf.Copy(CurPlayer);
		Profiles->AddProfile(CurPlayer);
		Profiles->SaveXML();
		pf->vm_ActionsCancel();		
	}
	return cva_MM_Close::LeftClick(SD);
}
// cva_ProfAdd_RaceFlg
void cva_ProfAdd_RaceFlg::SetFrameState(SimpleDialog* SD){
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP){		
		GP->SetSpriteID(GlobalAI.Ai[vNewProf.m_iNation].NWaterAI);
	}
}
//

// cva_ProfAdd_Port
void cva_ProfAdd_Port::SetFrameState(SimpleDialog* SD){
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP){
		GP->FileID=GlobalAI.Ai[vNewProf.m_iNation].Hero.PortFileID;
		GP->SetSpriteID(vNewProf.m_iCurHeroId);
	}
}
// cva_ProfAdd_Desc
void cva_ProfAdd_Desc::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		static char* mess=NULL;
		static int spr=-1;
		if(mess!=GlobalAI.Ai[vNewProf.m_iNation].Hero.DescriptionID||spr!=vNewProf.m_iCurHeroId){
			mess=GlobalAI.Ai[vNewProf.m_iNation].Hero.DescriptionID;
			spr=vNewProf.m_iCurHeroId;
			//_str txt;
			//txt=mess;
			//txt+=spr;
			//TB->SetMessage(GetTextByID(txt.pchar()));
			//
			//NatID=CurPlayer->m_iNation;
			//HeroID=CurPlayer->m_iCurHeroId;
			_str text;
			vNewProf.GetHeroDescription(text);
			TB->SetMessage(text.pchar());
			DialogsDesk* DD=dynamic_cast<DialogsDesk*>(SD->ParentDS);
			if(DD&&DD->VScroller){
				DD->VScroller->SPos=0;
			}
		}		
	}
}
// cva_ProfAdd_PortScr
void cva_ProfAdd_PortScr::Init(SimpleDialog* SD){
	VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
	if(SB){
		SB->SPos=0;
	}
}
void cva_ProfAdd_PortScr::SetFrameState(SimpleDialog* SD){
	VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
	if(SB){
		vNewProf.m_iCurHeroId=SB->SPos;
		//
		int GPid=GlobalAI.Ai[vNewProf.m_iNation].Hero.PortFileID;
		SB->SetMaxPos(GPS.GPNFrames(GPid)-1);
		if(vNewProf.m_iCurHeroId<0)vNewProf.m_iCurHeroId=0;
		if(vNewProf.m_iCurHeroId>GPS.GPNFrames(GPid)-1)vNewProf.m_iCurHeroId=GPS.GPNFrames(GPid)-1;
		SB->SPos=vNewProf.m_iCurHeroId;
	}	
}
bool cva_ProfAdd_PortScr::LeftClick(SimpleDialog* SD){
	UnPress();
	extern bool realLpressed;
	realLpressed=false;
	return true;
}
bool cva_ProfAdd_PortScrBut::LeftClick(SimpleDialog* SD){
	vNewProf.m_iCurHeroId+=SD->ID;
	return true;
}
// cva_Sin_ProfName
void cva_Sin_ProfName::SetFrameState(SimpleDialog* SD){
	Init(SD);
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&Profiles){
		if(Profiles->m_pPlayer.GetAmount()==0){
			//v_MainMenu.StartDS("AddProfile");
			//Profiles->SaveXML();
			v_MainMenu.CloseDS();
		}
		if(Profiles->m_pPlayer.GetAmount()!=0){			
			//static char* txt=GetTextByID("#CUR_PROFILE");
			_str txt2;
			//txt2=txt;
			//txt2+=": {CR}";
			CurPlayer=Profiles->m_pPlayer[0];
			txt2+=CurPlayer->m_chName.pchar();
			TB->SetMessage(txt2.pchar());
			PlayerName=CurPlayer->m_chName;
		}		
	}
}
void cva_Sin_ProfName::Init(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		if(!Profiles){
			Profiles=new CPlayerSAVE;
		}
	}	
}
// cva_ProfSel_Accept
void cva_ProfSel_Accept::SetFrameState(SimpleDialog* SD){
	SD->Enabled=!vCurProfDel;	
}
bool cva_ProfSel_Accept::LeftClick(SimpleDialog* SD){
	if(!vCurProfDel){
		if(v_MainMenu.Enable){
			ParentFrame* pf=SD;
			while(pf->ParentDS) pf=(ParentFrame*)pf->ParentDS;
			pf->vm_ActionsAccept();
			Profiles->SaveXML();
			pf->vm_ActionsCancel();
		}	
		return cva_MM_Close::LeftClick(SD);
	}
	return true;
}
// cva_ProfSave
bool cva_ProfSave::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable&&Profiles){
		Profiles->SaveXML();
	}
	return true;
}
// cva_ProfLoad
bool cva_ProfLoad::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable&&Profiles&&Profiles->m_pPlayer.GetAmount()!=0){
		Profiles->reset_class(Profiles);
		Profiles->LoadXML();
	}
	return true;
}
//
bool Draw_EW2_CampaignStats(SimpleDialog* SD){
	Profiles->LoadXML();
	CurPlayer=Profiles->m_pPlayer[0];
	return v_MainMenu.StartDS(v_ISys.EW2_CampaignStats.pchar());	
}
// cva_ProfCur_Desc
void cva_ProfCur_Desc::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&CurPlayer&&v_MainMenu.Enable){
		if(!(Init&&CurPlayer->m_iNation==NatID&&CurPlayer->m_iCurHeroId==HeroID)){
			NatID=CurPlayer->m_iNation;
			HeroID=CurPlayer->m_iCurHeroId;
			_str text;
			CurPlayer->GetHeroDescription(text);
			TB->SetMessage(text.pchar());
		}		
	}
}
// cva_ProfCur_Name
void cva_ProfCur_Name::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&CurPlayer&&v_MainMenu.Enable){
		_str n;
		n=CurPlayer->m_chName.pchar();
		LimitString(n.pchar(),TB->PassiveFont,95);
		TB->SetMessage(n.pchar());
	}
}
// cva_ProfCur_Race
void cva_ProfCur_Race::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&CurPlayer&&v_MainMenu.Enable&&CurPlayer->m_iNation<GlobalAI.NAi){
		TB->SetMessage(GlobalAI.Ai[CurPlayer->m_iNation].Message);
	}
}
// cva_ProfCur_Rank
void cva_ProfCur_Rank::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&CurPlayer&&v_MainMenu.Enable){
		//TB->SetMessage(GlobalAI.Ai[CurPlayer->m_iNation].Message);
		//CurPlayer->m
	}
}
// cva_ProfCur_Diff
void cva_ProfCur_Diff::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&CurPlayer&&v_MainMenu.Enable&&CurPlayer->m_iDifficulty<GlobalAI.Diff.GetAmount()){
		TB->SetMessage(GlobalAI.Diff[CurPlayer->m_iDifficulty].Name);
	}
}
// cva_ProfCur_Port
void cva_ProfCur_Port::SetFrameState(SimpleDialog* SD){
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP&&CurPlayer&&v_MainMenu.Enable&&CurPlayer->m_iNation<GlobalAI.NAi){
		GP->FileID=GlobalAI.Ai[CurPlayer->m_iNation].Hero.PortFileID;
		GP->SetSpriteID(CurPlayer->m_iCurHeroId);
	}
}
// cva_ProfCur_Ocup
void cva_ProfCur_Ocup::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&CurPlayer&&v_MainMenu.Enable){
		int c,n;
		if(CurPlayer->GetCapturedSectors(c,n)&&CurPlayer->m_iDifficulty<GlobalAI.Diff.GetAmount()){
			_str txt;
			txt=c;
			txt+="/";
			txt+=n;
			//TB->SetMessage(GlobalAI.Diff[CurPlayer->m_iDifficulty].Name);
			TB->SetMessage(txt.pchar());
		}		
	}
}
// cva_ProfDel_Desk
void cva_ProfDel_Desk::SetFrameState(SimpleDialog* SD){
	SD->Visible=vCurProfDel;
}
// cva_ProfDelete
bool cva_ProfDelete::LeftClick(SimpleDialog* SD){
	if(CurPlayer&&v_MainMenu.Enable){
		vCurProfDel=true;
	}
	return true;
}
// cva_ProfStats
bool cva_ProfStats::LeftClick(SimpleDialog* SD){
	//bool Draw_EW2_CampaignStats(SimpleDialog* SD);
	//Draw_EW2_CampaignStats(SD);
	v_MainMenu.StartDS(v_ISys.EW2_CampaignStats.pchar());
	return true;
}
// cva_ProfDel_Accept
bool cva_ProfDel_Accept::LeftClick(SimpleDialog* SD){
	if(CurPlayer&&v_MainMenu.Enable){
		if(Profiles){
			Profiles->DeleteProfile(CurPlayer);			
			if(Profiles->m_pPlayer.GetAmount()==0){
				v_MainMenu.StartDS("AddProfile");
				Profiles->SaveXML();
				v_MainMenu.CloseDS();
			}			
		}
		vCurProfDel=false;
	}
	return true;
}
// cva_ProfDel_Cancel
bool cva_ProfDel_Cancel::LeftClick(SimpleDialog* SD){
	if(CurPlayer&&v_MainMenu.Enable){
		vCurProfDel=false;
	}
	return true;
}
// cva_ProfSel_Add
void cva_ProfSel_Add::SetFrameState(SimpleDialog* SD){
	SD->Enabled=!vCurProfDel;	
}
// cva_ProfSel_Cancel
void cva_ProfSel_Cancel::SetFrameState(SimpleDialog* SD){
	SD->Enabled=!vCurProfDel;
	if(Profiles&&Profiles->m_pPlayer.GetAmount()==0){
		SD->v_Actions.LeftClick(SD);
	}
}
bool cva_ProfSel_Cancel::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		if(Profiles&&Profiles->m_pPlayer.GetAmount()!=0){
			Profiles->reset_class(Profiles);
			Profiles->LoadXML();
		}
	}
	return cva_MM_Cancel::LeftClick(SD);
}
// cva_MM_SinPrepare
bool cva_MM_SinPrepare::LeftClick(SimpleDialog* SD){
	NPlayers=1;		
	memset(&GSets.CGame.PL_INFO,0,sizeof GSets.CGame.PL_INFO);
	//PlayerName="Player";
	return true;
}
// cva_MM_SinStart
bool cva_MM_SinStart::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		if(!Profiles){
			Profiles=new CPlayerSAVE;
		}
		if(Profiles->m_pPlayer.GetAmount()==0){
			v_MainMenu.StartDS("AddProfile");
			if(Profiles->m_pPlayer.GetAmount()==0) return true;
			Profiles->SaveXML();
		}
		vGameMode=gmSin;
		NPlayers=1;		
		//PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
		//if(I) memset(I,0,sizeof PlayerInfo);
		memset(&GSets.CGame.PL_INFO,0,sizeof GSets.CGame.PL_INFO);
		cva_MM_Start::LeftClick(SD);
	}
	return true;
}
// cva_Mess_CenterQuest
bool vNewQuest=false;
bool vMessQuest=false;
int vCenterQuest=-1;
void cva_Mess_CenterQuest::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;	
	if(vGetHelp){
		vGetHelp(&vTASKS,&vTALKS,&vHINTS);
		if(!vTASKS) return;
		static int LastTaskID=-1;
		static int HideTime=AnimTime;
		if(HideTime>AnimTime){
			SD->Visible=true;
		}else for(int i=0;i<vTASKS->GetAmount();i++){
			CSingleMessage* SM=(*vTASKS)[i];
			if(SM->Visible){					
				if(i==LastTaskID+1||i==vCenterQuest){					
					TextButton* TB=NULL;
					if(SD->DSS.GetAmount()) TB=dynamic_cast<TextButton*>(SD->DSS[0]);
					if(TB&&SM->TextID.pchar()&&SM->TextID.pchar()[0]){
						_str Mess;
						char color[32];
						sprintf(color,"{C %x}",SM->Color);
						Mess=color;
						Mess+=GetTextByID(SM->TextID.pchar());
						TB->SetMessage(Mess.pchar());
						HideTime=AnimTime+6*6400;
						vNewQuest=true;
					}
					LastTaskID=i;
					vCenterQuest=-1;
					break;
				}
			}else{
				if(LastTaskID>i){
					LastTaskID=-1;
					break;
				}
			}
		}
	}
}
// cva_Mess_QuestsButton
int vHideMessQuest=0;
void cva_Mess_QuestsButton::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP){
		if(vGameMode!=gmEdit) SD->Visible=true;
		static int spr=GP->SpriteID;
	 	if(vNewQuest){
			if(vMessQuest){
				GP->SetSpriteID(spr);
				vNewQuest=false;
			}else{
				//(AnimTime>>6)&1
				if((GetTickCount()>>9)&1) GP->SetSpriteID(spr);
					else GP->SetSpriteID(spr+1);
			}
		}
		if(vMessQuest){
			if(SD->MouseOver) vHideMessQuest=AnimTime+6*6400;
			if(vHideMessQuest<AnimTime) vMessQuest=false;
		}
	}
}
bool cva_Mess_QuestsButton::LeftClick(SimpleDialog* SD){	
	if(v_MainMenu.Enable){
		vMessQuest=!vMessQuest;
		vHideMessQuest=AnimTime+6*6400;
	}
	return true;
}
// cva_Mess_Quests
void cva_Mess_Quests::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;	
	if(vGetHelp&&vMessQuest){
		vGetHelp(&vTASKS,&vTALKS,&vHINTS);
		TextButton* TB=NULL;
		if(SD->DSS.GetAmount()) TB=dynamic_cast<TextButton*>(SD->DSS[0]);
		if(vTASKS&&TB){
			SD->Visible=true;
			_str Mess;			
			for(int i=0;i<vTASKS->GetAmount();i++){
				CSingleMessage* SM=(*vTASKS)[i];
				if(SM->Visible){ //&&!SM->Deleted
					char color[32];
					char com[2]={0,0};
					com[0]=i+1;
					if(!SM->Deleted) sprintf(color,"{C %x}{R FF840100 %s}",SM->Color,com);
						else sprintf(color,"{C C0665F57}");
					Mess+=color;
					Mess+=GetTextByID(SM->TalkerID.pchar());							
					Mess+="\\";
				}
			}
			TB->SetMessage(Mess.pchar());			
			if(GlobalTextMouseOverCommand[0]){				
				CSingleMessage* SM=(*vTASKS)[GlobalTextMouseOverCommand[0]-1];
				if(SM&&SM->Visible&&SM->x&&SM->y) AddPulseBox(SM->x,SM->y);
				GlobalTextMouseOverCommand[0]=0;			
			}
			if(GlobalTextCommand[0]){
				vCenterQuest=GlobalTextCommand[0]-1;
				GlobalTextCommand[0]=0;
				UnPress();
			}

		}
	}
}
// cva_Mess_Dialogs
struct vsTalk{
	int Start; // time
	int Stop; // time of wisping
};
void cva_Mess_Dialogs::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;	
	if(vGetHelp){
		vGetHelp(&vTASKS,&vTALKS,&vHINTS);
		TextButton* TB=NULL;
		if(SD->DSS.GetAmount()) TB=dynamic_cast<TextButton*>(SD->DSS[0]);
		if(vTALKS&&TB){			
			static DynArray<vsTalk> vTalks;
			_str Mess;
			int n=0;
			for(int i=0;i<vTALKS->GetAmount()-3;i++){
				CSingleMessage* SM=(*vTALKS)[i+3];
				if(SM->Visible){
					if(i==vTalks.GetAmount()){
						vsTalk t;
						t.Start=(i==0)?AnimTime:vTalks[i-1].Stop;
						t.Stop=t.Start+6*6400;
						vTalks.Add(t);
					}
					if(i<vTalks.GetAmount()&&n<2){
						vsTalk* t=vTalks+i;
						if(AnimTime<t->Stop){
							n++;
							Mess.print("{FGH}{C %x}%s:{FCH} %s\\",
								SM->Color,GetTextByID(SM->TalkerID.pchar()),GetTextByID(SM->TextID.pchar()));
						}
					}else{
						break;
					}
				}else{
					if(i==0){
						vTalks.FastClear();
						break;
					}
				}
			}		
			if(Mess.pchar()[0]){
				TB->SetMessage(Mess.pchar());
				SD->Visible=true;
			}
		}
	}
}
// cva_M_ModalDesk
void cva_M_ModalDesk::SetFrameState(SimpleDialog* SD){
	//DialogsDesk* DD=dynamic_cast<DialogsDesk*>(SD);
	DialogsDesk* DD=(DialogsDesk*)SD;	
	if(DD){		
		DD->Visible=DD->Name==v_MainMenu.ModalDesk;
	}
}
// cva_M_ModalDeskSet
bool cva_M_ModalDeskSet::LeftClick(SimpleDialog* SD){
	LastKey=0;
#ifdef _DEMO_				
	if(!Name.isClear()&&Name!="Single"){		
		v_MainMenu.SetModalDesk(Name.pchar());
	}else 
		if(!SD->Name.isClear()&&SD->Name!="Single"){			
			v_MainMenu.SetModalDesk(SD->Name.pchar());
		}
#else
	if(!Name.isClear()){		
		v_MainMenu.SetModalDesk(Name.pchar());
	}else 
	if(!SD->Name.isClear()){		
		v_MainMenu.SetModalDesk(SD->Name.pchar());
	}
#endif _DEMO_
	return true;
}
// cva_M_ModalDeskBack
bool cva_M_ModalDeskBack::LeftClick(SimpleDialog* SD){	
	v_MainMenu.BackModalDesk();
	return true;
}
// cva_M_Accept
bool cva_M_Accept::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		ParentFrame* pf=SD;
		while(pf->ParentDS) pf=(ParentFrame*)pf->ParentDS;
		pf->vm_ActionsAccept();
	}
	return true;
}
// cva_M_ModDesSetRetChang
bool cva_M_ModDesSetRetChang::LeftClick(SimpleDialog* SD){
	cva_M_ModalDeskSet::LeftClick(SD);
	if(v_MainMenu.Enable&&SD){
		ParentFrame* pf=SD;
		while(pf->ParentDS) pf=(ParentFrame*)pf->ParentDS;		
		pf->vm_ActionsCancel();
	}
	return true;
}
// cva_M_AcceptSettings
bool cva_M_AcceptSettings::LeftClick(SimpleDialog* SD){
	if(v_MainMenu.Enable){
		ParentFrame* pf=SD;
		while(pf->ParentDS) pf=(ParentFrame*)pf->ParentDS;
		pf->vm_ActionsAccept();
		GSets.Save();
		v_MainMenu.ModalDesk="Main";
	}
	return true;
}
// cva_TutButtonVisibleAdd
void cva_TutButtonVisibleAdd::SetFrameState(SimpleDialog* SD){
	if(vValuesMap()->VIEW_OBJS(_vvMISSMGR_)){
		SD->Visible=false;
	}
}
// cva_M_Coss2_Relax
void cva_M_Coss2_Relax::SetFrameState(SimpleDialog* SD){
	GSets.ShowRecListTime=10000;
	if(GSets.ShowRecListTime&&GetTickCount()>v_MainMenu.LastLPress+GSets.ShowRecListTime){
		vfPlayRecorsList();
		v_MainMenu.LastLPress=GetTickCount();
	}
}
// cva_GameTime
void cva_GameTime::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&GSets.GameDayLength){		
		_str time;
		time.print("Day Time: %d",(AnimTime/6400)%GSets.GameDayLength);		
		TB->SetMessage(time.pchar());
	}
}
// cva_MM_StartInternet
bool cva_MM_StartInternet::LeftClick(SimpleDialog* SD){
#ifndef _DEMO_
	if(v_MainMenu.Enable && !InternetGame){		
		InternetGame=true;
		UseGSC_Login=true;
		void InternalProcess();
		InternalProcess();
		CurProtocol=3;
		vGameMode=gmMul;
		processMultiplayer();
		UseGSC_Login=false;
		InternetGame=false;
	}
#endif _DEMO_
	return true;
}
// cva_M_MultRoom_Back
DWORD vWaitServerInfo=0;
void cva_M_MultRoom_Back::SetFrameState(SimpleDialog* SD){
	//if(GameInProgress)return;
	if(!v_MM_Host){
		if(GetTickCount()>vWaitServerInfo){
			bool Host=false;
			CurrentGameInfo* g=&GSets.CGame;
			for(int i=0;i<NPlayers;i++){
				PlayerInfo* p=g->PL_INFO+i;
				if(p&&p->Host&&(GetTickCount()-GetLastAnswerT(p->PlayerID)<20000)) Host=true;
			}
			if(!(Host||GameInProgress)){
				SD->v_Actions.LeftClick(SD);
			}
		}	
		if(GameInProgress){
			CurrentGameInfo* g=&GSets.CGame;
			GSets.CGame.UnitTransform=true;
			g->RunClientGame();
			//g->LeaveRoom();
			GameInProgress=false;
			SD->v_Actions.LeftClick(SD);
		}	
	}
}
bool cva_M_MultRoom_Back::LeftClick(SimpleDialog* SD){
	CurrentGameInfo* g=&GSets.CGame;	
	g->LeaveRoom();
	return true;
}
// cva_InGameMenu_MainDesk
int cva_InGameMenu_MainDesk::CurrentID=0;
void cva_InGameMenu_MainDesk::SetFrameState(SimpleDialog* SD){
	SD->Visible = SD->Visible && ID==CurrentID;
}
// cva_InGameMenu_MainDesk_Set
bool cva_InGameMenu_MainDesk_Set::LeftClick(SimpleDialog* SD){
	cva_InGameMenu_MainDesk::CurrentID=ID;
	//if(!GSets.ReleaseInterface){
	//	cva_InGameMenu_MainDesk::CurrentID=1;
	//}
	return true;
}
//
void cva_InGameMenu_ExitConfirm::SetFrameState(SimpleDialog* SD){
	TextButton* W=Window.Get();
	TextButton* T=Text.Get();
	VitButton* E=Exit.Get();
	if(cva_InGameMenu_MainDesk::CurrentID==0){
		W->SetMessage(GMapWindow);
		T->SetMessage(GMapText);
		E->SetMessage(GMapExit);
	}else{
		W->SetMessage(SkirmishWindow);
		T->SetMessage(SkirmishText);
		E->SetMessage(SkirmishExit);
	}
}
// cva_Map_Preview
bool mMapPreview=false;
void cva_Map_Preview::SetFrameState(SimpleDialog* SD){
//	SD->OnDraw = mMapPreview?OnDrawMinimap:NULL;
	//OnDrawMinimap(SD);
	SD->Visible = mMapPreview&&!vHidePreview?OnDrawMinimap:NULL;
}
// cva_ItemChoose_Set
//extern int ItemChoose;
bool cva_ItemChoose_Set::LeftClick(SimpleDialog* SD){
	ItemChoose=ChooseValue;
	return true;
}
// Geometry
void cva_Geometry_CompactHorisontList::SetFrameState(SimpleDialog* SD){
	int x=0;
	int ly=0;
	for(int i=0;i<SD->DSS.GetAmount();i++){
		SimpleDialog* ch=SD->DSS[i];
		if(ch&&ch->Visible){
			ch->Setx(x);
			ch->Sety(0);
			if(ch->GetHeight()>ly){
				ly=ch->GetHeight();
			}
			x+=ch->GetWidth();
		}
	}
	SD->SetWidth(x);
	SD->SetHeight(ly);
}
void cva_Geometry_Align_Top::SetFrameState(SimpleDialog* SD){
	//int x=0;
	for(int i=0;i<SD->DSS.GetAmount();i++){
		SimpleDialog* ch=SD->DSS[i];
		if(ch){

		}
	}

}
void cva_Geometry_List_Horisont::SetFrameState(SimpleDialog* SD){
}
void cva_Geometry_WhiteSpace_Right::SetFrameState(SimpleDialog* SD){
}
//
CEXPORT bool vf_SettlementUpgrade_EnableState(){
	//return vGameMode!=gmCamp;
	return !isInCampaign();
}
// cva_C2_HintMessage
void cva_C2_HintMessage::SetFrameState(SimpleDialog* SD){
	int H=0;
	SimpleDialog* Mess=SD->Find("Mess");
	if(Mess){
		H+=Mess->y+Mess->GetHeight()+4;
	}
	SimpleDialog* Delim=SD->Find("Delim");
	if(Delim){
		Delim->Sety(H+4);
		H+=Delim->GetHeight()+8;
	}
	SD->SetHeight(H);
}
// cva_RND_MapParam
int mRND_MapParam[8];
DWORD GetRND_MapParam(){
	int N=GetAmountOfExtraOptions();
	DWORD r=0;
	for(int i=0;i<N;i++){
		r+=mRND_MapParam[i]<<(4*i);
	}
	return r;
}
void cva_RND_MapParam::SetFrameState(SimpleDialog* SD){
	if(Index<GetAmountOfExtraOptions()){
		SD->Visible=true;
		TextButton* TB=dynamic_cast<TextButton*>(SD->Find("Name"));
		ComboBox* CB=dynamic_cast<ComboBox*>(SD->Find("Combo"));
		if(TB&&CB){
			CB->Enabled=v_MM_Host||vGameMode==gmSin;
			if(!CB->Enabled){
				char rnd[128];
				DWORD opt=0;
				PlayerInfo* PL=GSets.CGame.GetHostPlayerInfo();
				if(PL){
					sscanf(PL->MapName,"%s%s%X",rnd,rnd,&opt);
					int L=(opt>>(Index<<2))&15;
					if(L<CB->NLines)CB->CurLine=L;
				}
			}
			TB->SetMessage((char*)GetExtraOptionName(Index));
			int NCB=GetExtraOptionAmountOfElements(Index);
			if(CB->NLines!=NCB){
				CB->Clear();
				for(int i=0;i<NCB;i++){
					CB->AddLine((char*)GetExtraOptionElementName(Index,i));
				}
				CB->CurLine=0;
			}
			//
			int newLine=CB->CurLine;
			CB->Enabled=GetExtraOptionsCompartability(Index,newLine,mRND_MapParam);
			if(newLine!=-1) CB->CurLine=newLine;
			//
			if(CheckRoomOpt(Index,newLine)){
				CB->CurLine=newLine;
				CB->Enabled=false;
			}
			mRND_MapParam[Index]=CB->CurLine;
			CB->SetHint("");
		}
	}else{
		SD->Visible=false;
	}	
}

typedef struct
{
	TextButton *tbTitle, *tbInfo, *tbWood, *tbStone, *tbGold, *tbFood, *tbIron,
		*tbLLife, *tbLife, *tbLBuildingTime, *tbBuildingTime,
		*tbLCost, *tbCost, *tbLSpeed, *tbSpeed, *tbLVision, *tbVision,
		*tbStrikeProbability, *tbVeteranExpert,
		*tbAttackSpeed, *tbAttackRadius, *tbAttackDamage,
		*tbAUII, *tbAUIII, *tbAUIV, *tbChopping, *tbPiercing, *tbCrushing,
		*tbDUIICh, *tbDUIIICh, *tbDUIVCh, *tbDUIIP, *tbDUIIIP, *tbDUIVP,
		*tbDUIICr, *tbDUIIICr, *tbDUIVCr, *tbSpread,
		*tbLSpread, *tbLStrikeProbability, *tbLVeteranExpert, *tbLDefenceUpgrades,
		*tbLAttackUpgrades, *tbLAttackIIIIIIV, *tbLDefenceIIIIIIV,
		*tbLSpecific, *tbLAbilities, *tbLAbilitiesSeparator, *tbLUpgrades,
		*tbLDescription;
	GPPicture *gpIcon, *gpAttackType, *gpIconBack, *gpIcon2Back,
		*gpAbilityA, *gpAbilityB, *gpAbilityC, *gpAbilityD, *gpAbilityE,
		*gpAbilityF, *gpAbilityG, *gpAbilityH, *gpAbilityI, *gpAbilityJ,
		*gpUASeparator, *gpISeparator, *gpEncBackGround;
	VitButton *vbSeparatorA, *vbSeparatorB, *vbSeparatorC, *vbSeparatorD,
		*vbSeparatorE, *vbSeparatorF;
	SimpleDialog *sdInfo, *sdAbility_Upgrade;
}ui_TControls;

ui_TControls ui_Controls;

// ================================================================================
// ui_AbilityShow
// ================================================================================
bool ui_AbilityShow(vui_UnitInfo *pUI, GPPicture *pP, int n)
{
	ActiveUnitAbility *pUnitAbility = NULL;
	_str Str;

	//pUnitAbility = pUI->pAbilities->AbilitiesList[n];
	OneObject* OB=OIS.GetLastSelPoint()->OB;
	pUnitAbility=OB->ActiveAbility->ActiveAbilities[n];

	if(pUnitAbility != NULL)
	{
		//if(strlen(pUnitAbility->Hint) == 0) return false;
		if(!pUnitAbility->Visible) return false;
		
		if(pP->DSS.GetAmount()){
			GPPicture* pPicture=dynamic_cast<GPPicture*>(pP->DSS[0]);
			if(pPicture){
				pPicture->FileID = pUnitAbility->FileID;
				pPicture->SetSpriteID(pUnitAbility->SpriteID);
			}
		}
		pP->Visible = true;
		char TStr[2048], Hint[2048];
		// Clearing %d
		strcpy(TStr, pUnitAbility->GetHint());
		Hint[0] = '\0';
		int L = strlen(TStr);
		char *pc = NULL;
		for(int i = 0; i < L - 1; i++)
			if((TStr[i] == '%') && (TStr[i + 1] == 'd'))
			{
				pc = &TStr[i + 2];
				TStr[i] = '\0';
				break;
			}
		strcat(Hint, TStr);
		if((pc != NULL) && (*pc != '\0')) strcat(Hint, pc);
		// Clearing %
		for(int n = 0; n < 2; n++)
		{
			strcpy(TStr, Hint);
			Hint[0] = '\0';
			L = strlen(TStr);
			pc = NULL;
			for(i = 0; i < L; i++)
				if(TStr[i] == '%')
				{
					pc = &TStr[i + 1];
					TStr[i] = '\0';
					break;
				}
			strcat(Hint, TStr);
			if((pc != NULL) && (*pc != '\0')) strcat(Hint, pc);
		}
		pP->SetHint(Hint);
		return true;
	}
	return false;
}

// ================================================================================
// ui_UpgradeShow
// ================================================================================
void ui_UpgradeShow(vui_UnitInfo *pUI, GPPicture *pP, int n)
{
	_str Str;
	if(pP->DSS.GetAmount()){
		GPPicture* pPicture=dynamic_cast<GPPicture*>(pP->DSS[0]);
		if(pPicture){
			pPicture->FileID = pUI->RUIconFileIDs[n];
			pPicture->SpriteID = pUI->RUIconSpriteIDs[n];
		}
	}
	if(strlen(pUI->RUHints[n]) == 0)
	{
		Str = "Upgrade #", Str += n, Str += " has empty hint!";
		pP->SetHint(Str.pchar());
	}
	else pP->SetHint(pUI->RUHints[n]);
	pP->Visible = true;
}

// ================================================================================
// ui_UnitInfoShow
// ================================================================================
void ui_UnitInfoShow(vui_UnitInfo *pUI, vui_BrigInfo *pBI, bool bShowAbilities,
					 ActiveUnitAbility *pAUA)
{
	_str Str;
	char szBuffer[512];

	// LifeMax
	Str = pUI->LifeMax;
	ui_Controls.tbLife->SetMessage(Str.pchar());
	// Damage
	Str = pUI->Damage[0];
	if(pUI->Damage[0] == 0) Str = "-";
	if(pUI->Damage[1] > 0)	// range attack
	{
		Str += "/";
		Str += pUI->Damage[1];
	}
	if((pUI->Damage[0] == 0) && (pUI->Damage[1] == 0)) Str = "-";
	//BRIGADE: it is needed to show additional damage values
	if(pBI != NULL)
	{
		Str += "(";
		if(pBI->MaxDamageAdd[0] == 0) Str += "-";
		else
		{
			Str += "+";
			Str += pBI->MaxDamageAdd[0];
		}
		if((pBI->MaxDamageAdd[1] > 0) && (pUI->Damage[1] > 0))
		{
			Str += "/+";
			Str += pBI->MaxDamageAdd[1];
		}
		Str += ")";
	}
	ui_Controls.tbAttackDamage->SetMessage(Str.pchar());
	// AttackType
	if(pUI->WeapType[0] == 3)	// Chopping
		ui_Controls.gpAttackType->SetSpriteID(2);
	else
		if(pUI->WeapType[0] == 2)	// Crushing
			ui_Controls.gpAttackType->SetSpriteID(0);
		else	// Piercing
			ui_Controls.gpAttackType->SetSpriteID(1);
	if((pUI->Damage[0] == 0) && (pUI->Damage[1] == 0))
		ui_Controls.gpAttackType->SetSpriteID(-1); // can't attack
	// Defence Values
	Str = pUI->Defence[0];
	if(pUI->Defence[0] == 0) Str = "-";
	if(pBI != NULL)	// BRIGADE - additional chopping defence value
	{
		Str += "(";
		if(pBI->MaxDefenceAdd[0] > 0)
		{
			Str += "+";
			Str += pBI->MaxDefenceAdd[0];
			if(Str.pchar()[0] == '-') Str.pchar()[0] = '0'; // Zero
		}
		else Str += "-";
		Str += ")";
	}
	ui_Controls.tbChopping->SetMessage(Str.pchar());
	Str = pUI->Defence[1];
	if(pUI->Defence[1] == 0) Str = "-";
	if(pBI != NULL)	// BRIGADE - additional piercing defence value
	{
		Str += "(";
		if(pBI->MaxDefenceAdd[1] > 0)
		{
			Str += "+";
			Str += pBI->MaxDefenceAdd[1];
			if(Str.pchar()[0] == '-') Str.pchar()[0] = '0'; // Zero
		}
		else Str += "-";
		Str += ")";
	}
	ui_Controls.tbPiercing->SetMessage(Str.pchar());
	Str = pUI->Defence[2];
	if(pUI->Defence[2] == 0) Str = "-";
	if(pBI != NULL)	// BRIGADE - additional crushing defence value
	{
		Str += "(";
		if(pBI->MaxDefenceAdd[2] > 0)
		{
			Str += "+";
			Str += pBI->MaxDefenceAdd[2];
			if(Str.pchar()[0] == '-') Str.pchar()[0] = '0'; // Zero
		}
		else Str += "-";
		Str += ")";
	}
	ui_Controls.tbCrushing->SetMessage(Str.pchar());
	// Speed
	Str = pUI->Speed;
	if(pUI->Speed == 0) Str = "-";
	ui_Controls.tbSpeed->SetMessage(Str.pchar());
	// BuildingTime
	Str = pUI->BuildingTime;
	ui_Controls.tbBuildingTime->SetMessage(Str.pchar());
	// Spread
	Str = pUI->Spread;
	if(pUI->Spread == 0) Str = "-";
	ui_Controls.tbSpread->SetMessage(Str.pchar());
	// Price
	Str = pUI->Price[0];	// Wood
	if(pUI->Price[0] == 0) Str = "-";
	ui_Controls.tbWood->SetMessage(Str.pchar());
	Str = pUI->Price[2];	// Stone
	if(pUI->Price[2] == 0) Str = "-";
	ui_Controls.tbStone->SetMessage(Str.pchar());
	Str = pUI->Price[1];	// Gold
	if(pUI->Price[1] == 0) Str = "-";
	ui_Controls.tbGold->SetMessage(Str.pchar());
	Str = pUI->Price[3];	// Food
	if(pUI->Price[3] == 0) Str = "-";
	ui_Controls.tbFood->SetMessage(Str.pchar());
	Str = pUI->Price[4];	// Iron
	if(pUI->Price[4] == 0) Str = "-";
	ui_Controls.tbIron->SetMessage(Str.pchar());
	// Cost
	Str = pUI->Cost;
	if(pUI->Cost == 0) Str = "-";
	ui_Controls.tbCost->SetMessage(Str.pchar());
	// Vision
	Str = pUI->Vision;
	if(pUI->Vision == 0) Str = "-";
	ui_Controls.tbVision->SetMessage(Str.pchar());
	// AttackSpeed
	sprintf(szBuffer, "%.2f", pUI->AttackSpeed);
	if(pUI->AttackSpeed == 0) strcpy(szBuffer, "-");
	ui_Controls.tbAttackSpeed->SetMessage(szBuffer);
	// AttackRadius
	Str = pUI->AttackRadius;
	if(pUI->AttackRadius == 0) Str = "-";
	ui_Controls.tbAttackRadius->SetMessage(Str.pchar());
	// StrikeProbability
	Str = pUI->StrikeProbability;
	if(pUI->StrikeProbability == 0) Str = "-";
	ui_Controls.tbStrikeProbability->SetMessage(Str.pchar());
	// VeteranExpert
	Str = pUI->VeteranKills;
	if(pUI->VeteranKills == 0) Str = "-";
	Str += " / ";
	if(pUI->ExpertKills == 0) Str += "-";
	else Str += pUI->ExpertKills;
	ui_Controls.tbVeteranExpert->SetMessage(Str.pchar());
	// AttackUpgrades
	// AUII
	Str = "+";
	Str += pUI->AttackUpgrades[0];
	if(pUI->AttackUpgrades[0] == 0) Str = "-";
	ui_Controls.tbAUII->SetMessage(Str.pchar());
	// AUIII
	Str = "+";
	Str += pUI->AttackUpgrades[1];
	if(pUI->AttackUpgrades[1] == 0) Str = "-";
	ui_Controls.tbAUIII->SetMessage(Str.pchar());
	// AUIV
	Str = "+";
	Str += pUI->AttackUpgrades[2];
	if(pUI->AttackUpgrades[2] == 0) Str = "-";
	ui_Controls.tbAUIV->SetMessage(Str.pchar());
	// DefenceUpgrades
	// Level II
	// DUIICh
	Str = "+";
	Str += pUI->DefenceUpgrades[0][0];
	if(pUI->DefenceUpgrades[0][0] == 0) Str = "-";
	ui_Controls.tbDUIICh->SetMessage(Str.pchar());
	// DUIIP
	Str = "+";
	Str += pUI->DefenceUpgrades[1][0];
	if(pUI->DefenceUpgrades[1][0] == 0) Str = "-";
	ui_Controls.tbDUIIP->SetMessage(Str.pchar());
	// DUIICr
	Str = "+";
	Str += pUI->DefenceUpgrades[2][0];
	if(pUI->DefenceUpgrades[2][0] == 0) Str = "-";
	ui_Controls.tbDUIICr->SetMessage(Str.pchar());
	// Level III
	// DUIIICh
	Str = "+";
	Str += pUI->DefenceUpgrades[0][1];
	if(pUI->DefenceUpgrades[0][1] == 0) Str = "-";
	ui_Controls.tbDUIIICh->SetMessage(Str.pchar());
	// DUIIIP
	Str = "+";
	Str += pUI->DefenceUpgrades[1][1];
	if(pUI->DefenceUpgrades[1][1] == 0) Str = "-";
	ui_Controls.tbDUIIIP->SetMessage(Str.pchar());
	// DUIIICr
	Str = "+";
	Str += pUI->DefenceUpgrades[2][1];
	if(pUI->DefenceUpgrades[2][1] == 0) Str = "-";
	ui_Controls.tbDUIIICr->SetMessage(Str.pchar());
	// Level IV
	// DUIVCh
	Str = "+";
	Str += pUI->DefenceUpgrades[0][2];
	if(pUI->DefenceUpgrades[0][2] == 0) Str = "-";
	ui_Controls.tbDUIVCh->SetMessage(Str.pchar());
	// DUIVP
	Str = "+";
	Str += pUI->DefenceUpgrades[1][2];
	if(pUI->DefenceUpgrades[1][2] == 0) Str = "-";
	ui_Controls.tbDUIVP->SetMessage(Str.pchar());
	// DUIVCr
	Str = "+";
	Str += pUI->DefenceUpgrades[2][2];
	if(pUI->DefenceUpgrades[2][2] == 0) Str = "-";
	ui_Controls.tbDUIVCr->SetMessage(Str.pchar());
	// Abilities show
	int UsingNSlots = 0;
	//
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	UnitActiveAbilityList* AL=NULL;
	if(SP&&SP->OB){
		AL=SP->OB->ActiveAbility;
	}
	//
	if(bShowAbilities == true && AL)	// for hero
	{
		int NAbilities = 0;
		//NAbilities = pUI->pAbilities->AbilitiesList.GetAmount();
		NAbilities = AL->ActiveAbilities.GetAmount();
		GPPicture *pPictures[10] = {ui_Controls.gpAbilityA, ui_Controls.gpAbilityB,
			ui_Controls.gpAbilityC, ui_Controls.gpAbilityD, ui_Controls.gpAbilityE,
			ui_Controls.gpAbilityF, ui_Controls.gpAbilityG, ui_Controls.gpAbilityH,
			ui_Controls.gpAbilityI, ui_Controls.gpAbilityJ};
		if(NAbilities >= 1)	// #A
		{
			ui_Controls.tbLAbilities->Visible = true;
			ui_Controls.gpUASeparator->Visible = true;
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 0)) UsingNSlots++;
		}
		if(NAbilities >= 2)	// #B
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 1)) UsingNSlots++;
		if(NAbilities >= 3)	// #C
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 2)) UsingNSlots++;
		if(NAbilities >= 4)	// #D
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 3)) UsingNSlots++;
		if(NAbilities >= 5)	// #E
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 4)) UsingNSlots++;
		if(NAbilities >= 6)	// #F
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 5)) UsingNSlots++;
		if(NAbilities >= 7)	// #G
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 6)) UsingNSlots++;
		if(NAbilities >= 8)	// #H
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 7)) UsingNSlots++;
		if(NAbilities >= 9)	// #I
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 8)) UsingNSlots++;
		if(NAbilities >= 10)	// #J
			if(ui_AbilityShow(pUI, pPictures[UsingNSlots], 9)) UsingNSlots++;
	}
	if(pUI->RUNumber > 0)
	{
		ui_Controls.tbLUpgrades->Visible = true;
		ui_Controls.gpUASeparator->Visible = true;
		// #A
		ui_UpgradeShow(pUI, ui_Controls.gpAbilityA, 0);
		if(pUI->RUNumber >= 2)	// #B
			ui_UpgradeShow(pUI, ui_Controls.gpAbilityB, 1);
		if(pUI->RUNumber >= 3)	// #C
			ui_UpgradeShow(pUI, ui_Controls.gpAbilityC, 2);
		if(pUI->RUNumber >= 4)	// #D
			ui_UpgradeShow(pUI, ui_Controls.gpAbilityD, 3);
		if(pUI->RUNumber >= 5)	// #E
			ui_UpgradeShow(pUI, ui_Controls.gpAbilityE, 4);
		if(pUI->RUNumber >= 6)	// #F
			ui_UpgradeShow(pUI, ui_Controls.gpAbilityF, 5);
		if(pUI->RUNumber >= 7)	// #G
			ui_UpgradeShow(pUI, ui_Controls.gpAbilityG, 6);
		if(pUI->RUNumber >= 8)	// #H
			ui_UpgradeShow(pUI, ui_Controls.gpAbilityH, 7);
		if(pUI->RUNumber >= 9)	// #I
			ui_UpgradeShow(pUI, ui_Controls.gpAbilityI, 8);
		if(pUI->RUNumber >= 10)	// #J
			ui_UpgradeShow(pUI, ui_Controls.gpAbilityJ, 9);
		UsingNSlots = pUI->RUNumber;
		if(UsingNSlots > 10) UsingNSlots = 10;
	}
	// Shifting controls: tbLDescription, gpISeparator, tbInfo
	// and changing height of Ability_Upgrades
	int yBase = 0, H = 0;
	if(UsingNSlots == 0) yBase = 420, H = 10;
	else
		if(UsingNSlots <= 4) yBase = 494, H = 470;
		else
			if(UsingNSlots <= 8) yBase = 546, H = 520;
			else yBase = 598, H = 570;
	ui_Controls.tbLDescription->Sety(yBase);
	ui_Controls.gpISeparator->Sety(yBase - 1);
	ui_Controls.tbInfo->Sety(yBase + 18);
	ui_Controls.sdAbility_Upgrade->SetHeight(H+5);
}

// ================================================================================
// ui_ControlsShow
// ================================================================================
void ui_ControlsShow(int n, bool S)
{
	_str strA, strD;
	switch(n)
	{
	case 0:	// unit or brigade
		ui_Controls.gpAbilityA->Visible = false;
		ui_Controls.gpAbilityB->Visible = false;
		ui_Controls.gpAbilityC->Visible = false;
		ui_Controls.gpAbilityD->Visible = false;
		ui_Controls.gpAbilityE->Visible = false;
		ui_Controls.gpAbilityF->Visible = false;
		ui_Controls.gpAbilityG->Visible = false;
		ui_Controls.gpAbilityH->Visible = false;
		ui_Controls.gpAbilityI->Visible = false;
		ui_Controls.gpAbilityJ->Visible = false;
		ui_Controls.tbLAbilities->Visible = false;
		ui_Controls.tbLUpgrades->Visible = false;
		ui_Controls.gpUASeparator->Visible = false;
		ui_Controls.tbLDescription->Sety(598);
		ui_Controls.gpISeparator->Sety(597);
		ui_Controls.tbInfo->Sety(616);
		ui_Controls.tbLSpeed->Visible = true;
		ui_Controls.tbSpeed->Visible = true;
		ui_Controls.gpIcon->Setxy(1, 8);
		ui_Controls.gpEncBackGround->Setxy(1, 8);
		ui_Controls.vbSeparatorA->Setxy(124, 4);
		ui_Controls.vbSeparatorA->SetWidth(110);
		ui_Controls.vbSeparatorB->Setxy(124, 37);
		ui_Controls.vbSeparatorB->SetWidth(110);
		ui_Controls.vbSeparatorC->Setxy(124, 71);
		ui_Controls.vbSeparatorC->SetWidth(110);
		ui_Controls.vbSeparatorD->Setxy(124, 104);
		ui_Controls.vbSeparatorD->SetWidth(110);
		ui_Controls.vbSeparatorE->Setxy(124, 137);
		ui_Controls.vbSeparatorE->SetWidth(110);
		ui_Controls.vbSeparatorF->Setxy(124, 169);
		ui_Controls.vbSeparatorF->SetWidth(110);
		ui_Controls.tbLLife->Setxy(125, 16);
		ui_Controls.tbLife->Setxy(205, 16);
		ui_Controls.tbLSpeed->Setxy(125, 50);
		ui_Controls.tbSpeed->Setxy(205, 50);
		ui_Controls.tbLBuildingTime->Setxy(125, 84);
		ui_Controls.tbBuildingTime->Setxy(205, 84);
		ui_Controls.tbLVision->Setxy(125, 117);
		ui_Controls.tbVision->Setxy(205, 117);
		ui_Controls.tbLCost->Setxy(125, 149);
		ui_Controls.tbCost->Setxy(205, 149);
		ui_Controls.gpEncBackGround->SpriteID = 18;
		break;
	case 1:	// hero
		ui_ControlsShow(0, false);
		break;
	case 2:	// horizontal layout
		ui_Controls.tbLSpeed->Visible = false;
		ui_Controls.tbSpeed->Visible = false;
		ui_Controls.gpIcon->Setxy(9, 8);
		ui_Controls.gpEncBackGround->Setxy(9, 8);
		ui_Controls.vbSeparatorA->Setxy(15, 108);
		ui_Controls.vbSeparatorA->SetWidth(100);
		ui_Controls.vbSeparatorB->Setxy(15, 137);
		ui_Controls.vbSeparatorB->SetWidth(100);
		ui_Controls.vbSeparatorC->Setxy(15, 169);
		ui_Controls.vbSeparatorC->SetWidth(100);
		ui_Controls.vbSeparatorD->Setxy(120, 108);
		ui_Controls.vbSeparatorD->SetWidth(100);
		ui_Controls.vbSeparatorE->Setxy(120, 137);
		ui_Controls.vbSeparatorE->SetWidth(100);
		ui_Controls.vbSeparatorF->Setxy(120, 169);
		ui_Controls.vbSeparatorF->SetWidth(100);
		ui_Controls.tbLLife->Setxy(18, 118);
		ui_Controls.tbLife->Setxy(87, 118);
		ui_Controls.tbLBuildingTime->Setxy(18, 150);
		ui_Controls.tbBuildingTime->Setxy(87, 150);
		ui_Controls.tbLVision->Setxy(123, 118);
		ui_Controls.tbVision->Setxy(195, 118);
		ui_Controls.tbLCost->Setxy(123, 150);
		ui_Controls.tbCost->Setxy(195, 150);
		ui_Controls.gpEncBackGround->SpriteID = 17;
		break;
	default:
		break;
	}
}
//

extern int MiniLx;
extern int MiniLy;
extern bool MiniMapVisible;

void cva_GameMiniMap::SetFrameState(SimpleDialog* SD){
	minix=SD->x+mapx;
	miniy=SD->y+mapy;
	MiniLx=mapLx;
	MiniLy=mapLy;
	SD->Visible=MiniMapVisible;
}

void cva_Inventory::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if( SP && SP->NI==NI && DetectHero(SP->OB) ){
		SD->Visible=IGI.Inventory;
	}
	if(IGI.Inventory){
		IGI.RenewedInventary=false;
	}else{
		IGI.Library=IGI.Inventory;
	}
	vmIGP.Info.Process();
}
void cva_Library::SetFrameState(SimpleDialog* SD){
	
	static int sX=SD->x;

	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&DetectHero(SP->OB)){
		if(SP->OB->ActiveAbility->LibID==0xFFFF)
			IGI.Library=false;
		SD->Visible=IGI.Library;
		if(SD->Visible){
			if(IGI.Inventory){
				SD->Setx(sX);
			}else{
				SD->Setx(0);
			}
		}
	}

}
//
// ================================================================================
// cva_U_Info
// ================================================================================
//
char* vUHM=NULL;
//
bool m_U_Info=false;
bool cva_U_Info_Switch::LeftClick(SimpleDialog* SD){
	m_U_Info=!m_U_Info;
	return true;

}
//
void cva_HideIfUInfoActive::SetFrameState(SimpleDialog* SD){
	SD->Visible&=!m_U_Info;
}
//
void cva_U_Info_Switch::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if(vUHM&&OIS.SelPoint.GetAmount()==1){
		vui_SelPoint* SP = OIS.GetLastSelPoint();
		if(SP&&SP->OB){
			OneObject* OB=SP->OB;
			if(OB->newMons->BigIconFile!=0xFFFF&&!(OB->NewBuilding||OB->NNUM==7)){				
				SD->Visible=true;
				VitButton* VB=dynamic_cast<VitButton*>(SD);
				if(VB){
					VB->State=m_U_Info;
				}
			}
		}
	}
}
//
void cva_U_InfoDesk::SetFrameState(SimpleDialog* SD){	
	if(OIS.SelPoint.GetAmount()==1){
		vui_SelPoint* SP=OIS.SelPoint+0;
		static word NIndex=0xFFFF;
		OneObject* OB=SP->OB;
		if(NIndex!=OB->NIndex){
			NIndex=OB->NIndex;
			char txt[64]="";
			try{strcpy(txt,OB->newMons->MD_File);}catch(...){};
			void ConvertToUp(char* str);
			ConvertToUp(txt);
			strcat(txt,".MD.HISTORY");
			//sprintf(txt,"%s.MD.HISTORY",OB->newMons->MD_File);
			vUHM=GetTextByID(txt);			
			if(txt!=vUHM){								
				VitButton* tbL=Life.Get();
				if(tbL){
					sprintf(txt,"%d",OB->newMons->Life); //MaxLife
					tbL->SetMessage(txt);
				}
				VitButton* tbB=BuildStage.Get();
				if(tbB){
					sprintf(txt,"%d",OB->newMons->ProduceStages); //NStages
					tbB->SetMessage(txt);
				}
				VitButton* tbR=RastrataNaVistrel.Get();
				if(tbR){
					int r=0;
					if(OB->newMons->NShotRes){
						r=OB->newMons->ShotRes[1];
					}
					sprintf(txt,"%d",r);
					tbR->SetMessage(txt);
				}
				VitButton* tbSt=StandGroundTime.Get();
				if(tbSt){
					sprintf(txt,"%d",OB->newMons->StandGroundTime); 
					tbSt->SetMessage(txt);
				}
				VitButton* tbSk=SkillDamageBonus.Get();
				if(tbSk){
					sprintf(txt,"%d",OB->newMons->SkillDamageFormationBonus); 
					tbSk->SetMessage(txt);
				}
			}else{				
				vUHM=NULL;
			}
		}
		SD->Visible = (vUHM && m_U_Info);
	}else{
		SD->Visible=false;
	}
}
word vHMNindex=0xFFFF;
void cva_U_HistoryMess::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(SP&&TB){
		if(SP->OB->NIndex!=vHMNindex){
			TB->SetMessage(vUHM);
			vHMNindex=SP->OB->NIndex;
		}
	}
}
void cva_U_HistoryTitle::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(SP&&TB){
		TB->SetMessage(SP->OB->newMons->Message);
	}
}
void cva_U_HistoryPicture::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(SP&&GP){
		NewMonster* NM=SP->OB->newMons;
		GP->FileID=NM->MinIconFile;
		GP->SetSpriteID(NM->MinIconIndex);
	}
}
//
void cva_U_Info::SetFrameState(SimpleDialog* SD){
	SD->Visible = false;
	if(m_U_Info&&OIS.SelPoint.GetAmount()==1){
		//
		vui_SelPoint* SP = OIS.SelPoint + 0;
		OneObject *pOO = SP->OB;
		if(!pOO) return;
		//
		ZeroMemory(&ui_Controls, sizeof(ui_TControls));
		ui_Controls.gpEncBackGround = EncBackGround.Get(),
		ui_Controls.vbSeparatorA = SeparatorA.Get(),
		ui_Controls.vbSeparatorB = SeparatorB.Get(),
		ui_Controls.vbSeparatorC = SeparatorC.Get(),
		ui_Controls.vbSeparatorD = SeparatorD.Get(),
		ui_Controls.vbSeparatorE = SeparatorE.Get(),
		ui_Controls.vbSeparatorF = SeparatorF.Get(),
		ui_Controls.tbTitle = Title.Get(), ui_Controls.tbInfo = Info.Get(),
		ui_Controls.tbWood = Wood.Get(), ui_Controls.tbStone = Stone.Get(),
		ui_Controls.tbGold = Gold.Get(), ui_Controls.tbFood = Food.Get(),
		ui_Controls.tbIron = Iron.Get(),
		ui_Controls.tbLLife = LLife.Get(), ui_Controls.tbLife = Life.Get(), 
		ui_Controls.tbLBuildingTime = LBuildingTime.Get(),
		ui_Controls.tbBuildingTime = BuildingTime.Get(),
		ui_Controls.tbLCost = LCost.Get(), ui_Controls.tbCost = Cost.Get(),
		ui_Controls.tbLSpeed = LSpeed.Get(), ui_Controls.tbSpeed = Speed.Get(),
		ui_Controls.tbLVision = LVision.Get(), ui_Controls.tbVision = Vision.Get(),
		ui_Controls.tbStrikeProbability = StrikeProbability.Get(),
		ui_Controls.tbVeteranExpert = VeteranExpert.Get(),
		ui_Controls.tbAttackSpeed = AttackSpeed.Get(),
		ui_Controls.tbAttackRadius = AttackRadius.Get(),
		ui_Controls.tbAttackDamage = AttackDamage.Get(),
		ui_Controls.tbAUII = AUII.Get(), ui_Controls.tbAUIII = AUIII.Get(),
		ui_Controls.tbAUIV = AUIV.Get(),
		ui_Controls.tbChopping = Chopping.Get(), ui_Controls.tbPiercing = Piercing.Get(),
		ui_Controls.tbCrushing = Crushing.Get(),
		ui_Controls.tbDUIICh = DUIICh.Get(), ui_Controls.tbDUIIICh = DUIIICh.Get(), 
		ui_Controls.tbDUIVCh = DUIVCh.Get(),
		ui_Controls.tbDUIIP = DUIIP.Get(), ui_Controls.tbDUIIIP = DUIIIP.Get(),
		ui_Controls.tbDUIVP = DUIVP.Get(),
		ui_Controls.tbDUIICr = DUIICr.Get(), ui_Controls.tbDUIIICr = DUIIICr.Get(),
		ui_Controls.tbDUIVCr = DUIVCr.Get(),
		ui_Controls.tbSpread = Spread.Get(),
		ui_Controls.gpIcon = Icon.Get(), ui_Controls.gpAttackType = AttackType.Get(),
		ui_Controls.gpIconBack = IconBack.Get(),
		ui_Controls.gpIcon2Back = Icon2Back.Get(),
		ui_Controls.tbLSpread = LSpread.Get(),
		ui_Controls.tbLStrikeProbability = LStrikeProbability.Get(),
		ui_Controls.tbLVeteranExpert = LVeteranExpert.Get(),
		ui_Controls.tbLDefenceUpgrades = LDefenceUpgrades.Get(),
		ui_Controls.tbLAttackUpgrades = LAttackUpgrades.Get(),
		ui_Controls.tbLAttackIIIIIIV = LAttackIIIIIIV.Get(),
		ui_Controls.tbLDefenceIIIIIIV = LDefenceIIIIIIV.Get(),
		ui_Controls.gpAbilityA = AbilityA.Get(),
		ui_Controls.gpAbilityB = AbilityB.Get(),
		ui_Controls.gpAbilityC = AbilityC.Get(),
		ui_Controls.gpAbilityD = AbilityD.Get(),
		ui_Controls.gpAbilityE = AbilityE.Get(),
		ui_Controls.gpAbilityF = AbilityF.Get(),
		ui_Controls.gpAbilityG = AbilityG.Get(),
		ui_Controls.gpAbilityH = AbilityH.Get(),
		ui_Controls.gpAbilityI = AbilityI.Get(),
		ui_Controls.gpAbilityJ = AbilityJ.Get(),
		ui_Controls.tbLSpecific = LSpecific.Get(),
		ui_Controls.tbLAbilities = LAbilities.Get(),
		ui_Controls.tbLAbilitiesSeparator = LAbilitiesSeparator.Get(),
		ui_Controls.tbLUpgrades = LUpgrades.Get(),
		ui_Controls.gpUASeparator = UASeparator.Get(),
		ui_Controls.gpISeparator = ISeparator.Get(),
		ui_Controls.tbLDescription = LDescription.Get(),
		ui_Controls.sdInfo = SDInfo.Get(),
		ui_Controls.sdAbility_Upgrade = Ability_Upgrade.Get();

		if(!ui_Controls.gpEncBackGround ||
			!ui_Controls.vbSeparatorA ||
			!ui_Controls.vbSeparatorB || !ui_Controls.vbSeparatorC ||
			!ui_Controls.vbSeparatorD || !ui_Controls.vbSeparatorE ||
			!ui_Controls.vbSeparatorF ||
			!ui_Controls.tbTitle || !ui_Controls.gpIcon || !ui_Controls.gpIconBack ||
			!ui_Controls.gpIcon2Back ||
			!ui_Controls.tbInfo || !ui_Controls.tbLLife || !ui_Controls.tbLife ||
			!ui_Controls.tbLBuildingTime || !ui_Controls.tbBuildingTime ||
			!ui_Controls.tbLCost || !ui_Controls.tbCost ||
			!ui_Controls.tbLSpeed || !ui_Controls.tbSpeed ||
			!ui_Controls.tbLVision || !ui_Controls.tbVision ||
			!ui_Controls.tbStrikeProbability || !ui_Controls.tbVeteranExpert ||
			!ui_Controls.tbAttackSpeed || !ui_Controls.tbAttackRadius ||
			!ui_Controls.gpAttackType || !ui_Controls.tbAttackDamage ||
			!ui_Controls.tbAUII || !ui_Controls.tbAUIII || !ui_Controls.tbAUIV ||
			!ui_Controls.tbChopping || !ui_Controls.tbPiercing || !ui_Controls.tbCrushing ||
			!ui_Controls.tbDUIICh || !ui_Controls.tbDUIIICh || !ui_Controls.tbDUIVCh ||
			!ui_Controls.tbDUIIP || !ui_Controls.tbDUIIIP || !ui_Controls.tbDUIVP ||
			!ui_Controls.tbDUIICr || !ui_Controls.tbDUIIICr || !ui_Controls.tbDUIVCr ||
			!ui_Controls.tbLSpread || !ui_Controls.tbLStrikeProbability ||
			!ui_Controls.tbLVeteranExpert || !ui_Controls.tbLDefenceUpgrades ||
			!ui_Controls.tbLAttackUpgrades ||
			!ui_Controls.tbLDefenceIIIIIIV ||
			!ui_Controls.tbSpread || !ui_Controls.tbLSpecific || 
			!ui_Controls.tbLAbilities ||
			!ui_Controls.tbLUpgrades ||
			!ui_Controls.gpAbilityA || !ui_Controls.gpAbilityB ||
			!ui_Controls.gpAbilityC || !ui_Controls.gpAbilityD ||
			!ui_Controls.gpAbilityE || !ui_Controls.gpAbilityF ||
			!ui_Controls.gpAbilityG || !ui_Controls.gpAbilityH ||
			!ui_Controls.gpAbilityI || !ui_Controls.gpAbilityJ ||
			!ui_Controls.gpUASeparator || !ui_Controls.gpISeparator ||
			!ui_Controls.tbLDescription || !ui_Controls.sdInfo ||
			!ui_Controls.sdAbility_Upgrade) return;

		vui_UnitInfo *pUI = NULL;
		vui_BrigInfo *pBI = NULL;
		vui_BuildInfo *pBuildInfo = NULL;
		_str STR;

		//
		NewMonster* NM = NATIONS->Mon[SP->NIndex]->newMons;
		ui_Controls.tbTitle->SetMessage(NewMonster_GetMessage(NM));

		if(NM->BigIconFile!=0xFFFF){
			ui_Controls.gpIcon->FileID = NM->BigIconFile;
			ui_Controls.gpIcon->SetSpriteID(NM->BigIconIndex);
		}else return;
		SD->Visible = true;
//		{
//			ui_Controls.gpIcon->FileID = NM->IconFileID;
//			ui_Controls.gpIcon->SetSpriteID(NM->IconID);
//		}
		bool bVertical = true;
		if(ui_Controls.gpIcon->GetWidth() > ui_Controls.gpIcon->GetHeight())
			bVertical = false; // horizontal
		ui_Controls.gpIcon->Nation=pOO->NNUM;
		if(NM->GetLongMessage() != NULL) ui_Controls.tbInfo->SetMessage(&NM->LongMessage);
		else ui_Controls.tbInfo->SetMessage(NewMonster_GetMessage(NM));

		ui_Controls.gpIconBack->Visible = false;
		ui_Controls.gpIcon2Back->Visible = false;

		switch(SP->Sort){
			case ve_UT_units:
				ui_ControlsShow(0, true);
				pUI = &SP->Inf.Units;
				if(pUI != NULL) ui_UnitInfoShow(pUI, NULL, false, NULL);
				break;
			case ve_UT_brigade:
				ui_ControlsShow(0, true);
				pBI = &SP->Inf.Brigade;
				if(pBI != NULL)
				{
					pBI->UI.Damage[0] = pBI->Damage[0],
						pBI->UI.Damage[1] = pBI->Damage[1];
					pBI->UI.Defence[0] = pBI->Defence[0],
						pBI->UI.Defence[1] = pBI->Defence[1],
						pBI->UI.Defence[2] = pBI->Defence[2];
					ui_UnitInfoShow(&pBI->UI, pBI, false, NULL);
				}
				// icon's background
				ui_Controls.gpIconBack->Visible = true;
				ui_Controls.gpIcon2Back->Visible = true;
				ui_Controls.gpIconBack->FileID = NM->IconFileID;
				ui_Controls.gpIconBack->SetSpriteID(NM->IconID);
				ui_Controls.gpIcon2Back->FileID = NM->IconFileID;
				ui_Controls.gpIcon2Back->SetSpriteID(NM->IconID);
				break;
			case ve_UT_hero:
				ui_ControlsShow(1, true);
				pUI = &SP->Inf.Units;
				ActiveUnitAbility *pAUA;
				pAUA = (ActiveUnitAbility*)SD->UserParam;
				if(pUI != NULL) ui_UnitInfoShow(pUI, NULL, true, pAUA);
				break;
			case ve_UT_building:
				ui_ControlsShow(1, true);
				if(bVertical == false) ui_ControlsShow(2, true);
				pBuildInfo = &SP->Inf.Buildings;
				if(pBuildInfo != NULL)
				{
                    pUI = &pBuildInfo->UI;
					if(pUI != NULL)
					{
						pUI->Speed = 0;
						ui_UnitInfoShow(pUI, NULL, false, NULL);
					}
				}
				break;
			case ve_UT_cannon:
				ui_ControlsShow(1, true);
				break;
			default:
				break;
		}
	}	
}
// cva_Peasant_Idle
extern int NIdlePeasant;
bool vPeasant_Idle=false;
void cva_Peasant_Idle::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if(vPeasant_Idle&&NIdlePeasant>0&&OIS.SelPoint.GetAmount()==1){
		SD->Visible=true;
		vPeasant_Idle=false;
		VitButton* VB=dynamic_cast<VitButton*>(SD);
		if(VB){			
		}
	}
}
bool cva_Peasant_Idle::LeftClick(SimpleDialog* SD){
	void CmdSelectIdlePeasants(byte NI);
	CmdSelectIdlePeasants(MyNation);
	return true;
}
// cva_Peasant_AutoWork
bool vPeasant_AutoWork=false;
void cva_Peasant_AutoWork::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if(NPlayers<2){
		if(NIdlePeasant>=0&&vPeasant_AutoWork&&OIS.SelPoint.GetAmount()==1){
			SD->Visible=true;
			vPeasant_AutoWork=false;
			VitButton* VB=dynamic_cast<VitButton*>(SD);
			if(VB){
				if(GSets.PeasantAutoWork){
					VB->State=1;
				}else{
					VB->State=0;
				}
			}
		}
	}else{
		GSets.PeasantAutoWork=false;
	}
}
bool cva_Peasant_AutoWork::LeftClick(SimpleDialog* SD){
	GSets.PeasantAutoWork=!GSets.PeasantAutoWork;	
	return true;
}
// cva_MR_Desk
void cva_MR_Desk::SetFrameState(SimpleDialog* SD){
	DialogsDesk* CH=Chat.Get();
	if(CH){
		CH->Visible=(vGameMode==gmMul);
	}
	TabButton* L=Load.Get();
	if(L){
		L->Visible=(vGameMode==gmMul);
	}
	DialogsDesk* MB=MulButtons.Get();
	DialogsDesk* SB=SinButtons.Get();
	if(MB&&SB){
		MB->Visible=(vGameMode==gmMul);
		SB->Visible=!MB->Visible;
	}	
}
// cva_MR_isServer
void cva_MR_isServer::SetFrameState(SimpleDialog* SD){	
	bool S= v_MM_Host || vGameMode!=gmMul;
	SD->Visible=(Server==S);
}

//cva_MR_GameSpeed
void cva_MR_GameSpeed::Init(SimpleDialog* SD){
	VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
	if(SB){
		SB->SPos=GSets.RoomParams.GameSpeed;
	}
}
void cva_MR_GameSpeed::SetFrameState(SimpleDialog* SD){
	VScrollBar* SB=dynamic_cast<VScrollBar*>(SD);
	if(SB){
		GSets.RoomParams.GameSpeed=SB->SPos;
		//switch(SB->SPos){
		//	case 0:
		//		break;
		//	case 1:
		//		break;
		//	case 2:
		//		break;
		//}
	}
};

// cva_vGameMode_Set
bool cva_vGameMode_Set::LeftClick(SimpleDialog* SD){
	vGameMode=(veGameMode)Mode;	
	return true;
}

void cva_vGameMode_SetVisible::SetFrameState(SimpleDialog* SD){
	bool cond=false;
	switch(Condition){
		case 0:
			cond=vGameMode==(veGameMode)Mode;
			break;
		case 1:
			cond=vGameMode!=(veGameMode)Mode;
			break;
	};
	switch(Operator){
		case 0:
			SD->Visible=cond;
			break;
		case 1:
			SD->Visible&=cond;
			break;
		case 2:
			SD->Visible|=cond;
			break;
	};
}
//
int GetTotalHeight(int x,int y);
void cva_BrigDestination::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if(OIS.SelPoint.GetAmount()==1){
		vui_SelPoint* SP=OIS.SelPoint+0;
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		if(SP->Type==ve_UT_brigade&&SP->NI==NI){
			Brigade* BR=CITY[NI].Brigs+SP->Inf.Brigade.BrigID;
			int x,y;
			if(BR->WarType&&BR->GetBrigadeDestination(x,y)){
				SD->Visible=true;
				GPPicture* GP=(GPPicture*)SD;
				if(MiniMap){
					GP->Setx((x>>6)+dx);
					GP->Sety((y>>6)+dy);
				}else{
					Vector3D v( x, y, GetTotalHeight(x, y) );
					WorldToScreenSpace( v );
					GP->Setx(v.x+dx);
					GP->Sety(v.y+dy);
				}
				GP->SpriteID=(GetTickCount()/40)%GPS.GPNFrames(GP->FileID);
				GP->Nation=NI;
			}
		}		
	}
}
// cva_BR_VirginPlayers
bool cva_BR_VirginPlayers::LeftClick(SimpleDialog* SD){	
	if(v_MM_Host||vGameMode==gmSin){
		for(int i=0;i<8;i++){
			PlayerInfo* I=GSets.CGame.PL_INFO+i;
			I->NationID=0;
		}
	}
	return true;
}
// 
void cva_MixWithWhite::SetFrameState(SimpleDialog* SD){
	SD->Diffuse=MixDWORD(Color,0xDFFFFFFF,255-SD->ID,SD->ID);
}
extern bool UseGSC_Login;
// cva_BR_SetVisibleIfInet
void cva_BR_SetVisibleIfInet::SetFrameState(SimpleDialog* SD){	
	SD->Visible=UseGSC_Login;
}
// cva_BR_SetVisibleIfLAN
void cva_BR_SetVisibleIfLAN::SetFrameState(SimpleDialog* SD){	
	SD->Visible=!UseGSC_Login;
}
// cva_BR_InetGo2Chat
void cva_BR_InetGo2Chat::SetFrameState(SimpleDialog* SD){	
	SD->Visible=UseGSC_Login;
}
bool cva_BR_InetGo2Chat::LeftClick(SimpleDialog* SD){
	UnPress();
	int ProcessInternetConnection(bool Active);
	ProcessInternetConnection(false);	
	return true;
}
// cva_BR_Title
void cva_BR_Title::SetFrameState(SimpleDialog* SD){
	VitButton* VB=dynamic_cast<VitButton*>(SD);
	if(vGameMode==gmSin){
		if(VB) VB->SetMessage(SinTitle);
	}else{
		if(VB) VB->SetMessage(MulTitle);
	}
}
// cva_LoadingTips
void cva_LoadingTips::SetFrameState(SimpleDialog* SD){
	static int mLB_id=0;
	static int mLB_time=0;	
	if(GetTickCount()-mLB_time>10000){		
		mLB_id=rand()%TipsAmount;
	}
	mLB_time=GetTickCount();
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		_str str;
		str.print("%s%d",v_ISys.LoadingTipsPrefix.pchar(),mLB_id);
		char* txt=GetTextByID(str.pchar());
		TB->SetMessage(txt);
	}
	//SD->Visible=(ID==mLB_id);
}
//cva_Loading_Back,	
void cva_Loading_Back::SetFrameState(SimpleDialog* SD){
	static int mLB_id=0;
	static int mLB_time=0;	
	if(GetTickCount()-mLB_time>10000){		
		mLB_id=rand()%Amount;
	}
	mLB_time=GetTickCount();
	SD->Visible=(ID==mLB_id);

}
//cva_NatPics
void cva_NatPics::SetFrameState(SimpleDialog* SD){
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(GP){
		static int mLB_id=0;
		static int mLB_time=0;	
		if(GetTickCount()-mLB_time>1000){		
			// set picture			
			byte NatID=0;
			if(InGame&&vGameMode!=gmCamp){
				PlayerInfo* I=GSets.CGame.GetCurrentPlayerInfo();
				if(I) NatID=I->NationID;
			}else{
				if(CurPlayer){
					NatID=CurPlayer->m_iNation;
				}
			}
			if(NatID<v_ISys.NationalPic.GetAmount()){
				cvi_GPFile* NP=v_ISys.NationalPic[NatID];
				ISM->Precache(NP->FileID);
				mLB_id=rand()%GPS.GPNFrames(NP->FileID);
				GP->FileID=NP->FileID;
				GP->SetSpriteID(mLB_id);
			}
		}
		mLB_time=GetTickCount();
		//SD->Visible=(ID==mLB_id);
	}
}
//cva_MM_UnloadBeforeMission,
void cva_MM_UnloadBeforeMission::SetFrameState(SimpleDialog* SD){
	void  UnloadBeforeMission();
	UnloadBeforeMission();
}
//
cva_PlayFullscreenVideo::cva_PlayFullscreenVideo(){
	MayBeSkipped=true;
}
bool cva_PlayFullscreenVideo::LeftClick(SimpleDialog* SD){
	if(!Name.isClear()){
		if(Credits){
			extern bool FullscreenVideoCredits;
			FullscreenVideoCredits=true;			
			StartAboutTime=GetTickCount();
			FinishAboutTime=0xFFFFFFF;
		}
		void PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
		PlayFullscreenVideo(Name.pchar(),px,py,MayBeSkipped);
	}
	LastKey=0;
	return true;
}
void cva_DemoDisable::SetFrameState(SimpleDialog* SD){
	//if(GetKeyState(VK_CONTROL)&0x8000){
	//	SD->Enabled=true;
	//}else{
	//	SD->Enabled=false;
	//}
#ifdef _DEMO_
	SD->Enabled=false;
#endif
}
//
//cva_OpenWebLink
void cva_OpenWebLink::SetFrameState(SimpleDialog* SD){
	//SD->Visible=SD->MouseOver;
	if(SD->MouseOver&&SD->Diffuse==0x00FFFFFF){		
		SetCurPtr(1);
	}
	if(!SD->MouseOver&&SD->Diffuse==0x50FFFFFF){
		SetCurPtr(0);
	}
	SD->Diffuse=SD->MouseOver?0x50FFFFFF:0x00FFFFFF;
}
bool cva_OpenWebLink::LeftClick(SimpleDialog* SD){
	if(!Link.isClear()){
		ShellExecute(NULL,"open",Link.pchar(),NULL,NULL,SW_MAXIMIZE);
	}	
	return true;
}
//cva_DemoScreen
int DS_Frame=0;
int NDS_Frame=0;
int DS_Time=0;
void cva_DemoScreen::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
#ifdef _DEMO_
#ifndef _PRESSMAN_
	NDS_Frame=Pic.GetAmount();
	if(DS_Frame<NDS_Frame){
		SD->Visible=true;		
		static int frame=0;
		if(DS_Time+SwitchTime<GetTickCount()){
			DS_Time=GetTickCount();
			BitPicture* BP=dynamic_cast<BitPicture*>(SD);
			if(BP){
				//ClonesArray<_str>* P=Pic[DS_Frame]->Str;
				BP->SetFileName(Pic[DS_Frame]->Str[frame]->pchar());
			}
			frame=(frame+1)%(Pic[DS_Frame]->Str.GetAmount());
		}
	}
#endif
#endif
	if(!SD->Visible){
		v_MainMenu.CloseDS();
	}
}
//cva_DemoNext
void cva_DemoNext::SetFrameState(SimpleDialog* SD){
	//SD->Visible=SD->MouseOver;
	SD->Visible = DS_Frame<(NDS_Frame-1);
	//
	if(SD->MouseOver&&SD->Diffuse==0x00FFFFFF){		
		SetCurPtr(2);
	}
	if(!SD->MouseOver&&SD->Diffuse==0x50FFFFFF){
		SetCurPtr(0);
	}
	SD->Diffuse=SD->MouseOver?0x50FFFFFF:0x00FFFFFF;
}
bool cva_DemoNext::LeftClick(SimpleDialog* SD){
	DS_Time=0;
	DS_Frame++;
	return true;
}
//cva_DemoExit
void cva_DemoExit::SetFrameState(SimpleDialog* SD){
	//SD->Visible=SD->MouseOver;
	SD->Visible = DS_Frame==(NDS_Frame-1);
	//
	if(SD->MouseOver&&SD->Diffuse==0x00FFFFFF){		
		SetCurPtr(11);
	}
	if(!SD->MouseOver&&SD->Diffuse==0x50FFFFFF){
		SetCurPtr(0);
	}
	SD->Diffuse=SD->MouseOver?0x50FFFFFF:0x00FFFFFF;
}
bool cva_DemoExit::LeftClick(SimpleDialog* SD){
	v_MainMenu.CloseDS();
	return true;
}
//cva_ExitToWin
bool cva_ExitToWin::LeftClick(SimpleDialog* SD){
	exit(0);
	return true;
}
// cva_Exit2MainMenu
bool v_Exit2MainMenu=0;
bool cva_Exit2MainMenu::LeftClick(SimpleDialog* SD){
	v_Exit2MainMenu=true;	
	return true;
}
// cva_BrigBinkVideo
bool vTaskCII=false;
void cva_BrigBinkVideoTry::SetFrameState(SimpleDialog* SD){
	bool TryShowBrigBinkFrame(int PlayerID);
	SD->Visible=false;
	if(OIS.SelPoint.GetAmount()==1&&CurProdID==-1){
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		if(SP&&SP->Type==ve_UT_brigade){
			SD->Visible = !GSets.DisableVideo && TryShowBrigBinkFrame(PlayerID);
		}
	}
	if(PlayerID==1){		
		bool vTCII=!SD->Visible;
		if(vTaskCII!=vTCII){
			vValuesMap()->VIEW_OBJS(_vvTASKS_CII_,vTCII);
			vTaskCII=vTCII;
		}
	}
}
void cva_BrigBinkVideoShow::OnDraw(SimpleDialog* SD){
	void ShowBrigBinkFrame(int VideoX,int VideoY, int PlayerID);
	ShowBrigBinkFrame(SD->x, SD->y, PlayerID);	
}
// Visibility
void cva_BrigBinkVisibility::SetFrameState(SimpleDialog* SD){
	SD->Visible=!GSets.DisableVideo;
}
// SetVision
bool cva_BrigBinkSetVision::LeftClick(SimpleDialog* SD){
	GSets.DisableVideo=!GSets.DisableVideo;
	return true;
}
// cva_NullHint
void cva_NullHint::SetFrameState(SimpleDialog* SD){
	if( SD->Visible && SD->MouseOver ){
		void ClearGlobalHint();
		ClearGlobalHint();
	}
}
//////////////////////////////////////////////////////////////////////////
void ShowNoCD(){
	v_MainMenu.StartDS("NoCD");
}
void ShowMemLimit(){
	v_MainMenu.StartDS("MemLimit");
}
//////////////////////////////////////////////////////////////////////////
void NormProfileName(char* s){
	if(!s)return;
	int L=strlen(s);
	for(int i=0;i<L;i++){
		byte c=s[i];
		if(c=='<'||c=='>'||c=='\\'||c=='/'||c<32){
			c='_';
		}
		s[i]=c;
	}
}

//

bool spCond_SPClassSet::Check(vui_SelPoint* SP){
	bool res=true;
	if(SP){
		int n=SPClassSet.GetAmount();
		for(int i=0;i<n;i++){
			res=false;
			if(SP->Sort==(ve_UnitType)SPClassSet[i]->SPClass){
				res=true;
				break;
			}
		}
	}
	return res;
};

//
void cva_FanUnitInfoToggle::SetFrameState(SimpleDialog* SD){
	OIS.UnitInfoVisible=false;
	if(OIS.UnitInfoToggle){
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		if(SP){
			OIS.UnitInfoVisible=true;
			oi_UnitInfo* UI=&OIS.UnitInfo;
			UI->PriceShow=false;
			//Price[6];
			//Attack[2];
			//Defence[4];
			UI->KillCount = SP->Type==ve_UT_units ? SP->Inf.Units.NKills : 0;
			UI->LifeMax=SP->OB->MaxLife;
			UI->ManaMax=SP->OB->MaxMana;
			//AttackRate;
			int GetAttSpeedBase(OneObject* OB,int AttType);
			int GetAttSpeed(OneObject* OB,int AttType);
			UI->AttackRate=GetAttSpeedBase(SP->OB,0);
			//AttackDistance;
			//MotionSpeed;
			//Message;
		}
	}
}
bool cva_FanUnitInfoToggle::LeftClick(SimpleDialog* SD){
	OIS.UnitInfoToggle=!OIS.UnitInfoToggle;
	return true;
}
//
static bool iDemoMessage=true;
void cva_DemoMessage::SetFrameState(SimpleDialog* SD){
	SD->Visible = TrueTime>30000 && TrueTime<120000 && iDemoMessage;
}
bool cva_DemoMessage::LeftClick(SimpleDialog* SD){
	iDemoMessage=false;
	return true;
}
void cva_DialogsHistory::SetFrameState(SimpleDialog* SD){
	DialogsHistory.SetMessage(SD);
}

ActiveUnitAbility* GetPickUpAbility(OneObject* OB);
OneMagazine* DetectMagazine2(OneObject* OB);

void cva_FocusedUnitHint::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if( SP && SP->NI==NI && DetectHero(SP->OB) ){
		extern word Att;
		extern word Friend;
		word Index=(Friend==0xFFFF)?Att:Friend;
		static word PrevIndex=0xFFFF;
		if(PrevIndex!=Index){
			PrevIndex=Index;
			SD->SetHint("");
			if(Index!=0xFFFF){
				OneObject* OB=Group[Index];
				ActiveUnitAbility* AA=GetPickUpAbility(OB);
				if(!AA){
					OneBonus* Bonus=DetectBonus(OB);
					if( Bonus && Bonus->GetCoolDownProc()==0 && Bonus->isCharged() ){
						NewBonus* NB=(NewBonus*)Bonus->GetA();
						if( NB && NB->ShowFocusedUnitHint ){
							AA=Bonus;
						}
						//NewBonus* NB=(NewBonus*)Bonus->GetA();
						//if(NB){
						//	UnitAbility* UA=NULL;
						//	ActiveUnitAbility* A=GetDropAbility(IOB,UA);
						//	return A;
						//}
					}
				}
				if(!AA){
					OneMagazine* Magazine=DetectMagazine2(OB);
					AA=Magazine;
				}
				if(AA){
					AA->OB=SP->OB;
					const char* hint=AA->GetHint();
					AA->OB=OB;
					SD->SetHint(hint);
					return;
				}
			}	
		}
	}else
		SD->SetHint("");
}

void cva_Inventory_HeroPortret::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		GPPicture* GP=(GPPicture*)SD;
		if(GP){
			int file=SP->OB->newMons->NewParams.PortretHeroFile;
			if(file!=-1){
				GP->FileID=file;
				GP->SetSpriteID(0);
				GP->Visible=true;
			}else{
				GP->Visible=false;
			}
		}
	}
}
int mSetHeroTab=-1;
int DetectMagazineType(OneObject* OB){
	OneMagazine* OM=DetectMagazine2(OB);
	if(OM){
		//NewMagazine* NM=(NewMagazine*)OM->GetA();
		//if(NM){
			itr_AA.Create(OB);
			while(ActiveUnitAbility* AA=itr_AA.Next()){
				if( AA && AA!=OM ){
					UnitAbility* UA=AA->GetA();
					if( UA && !UA->System ){
						switch(AA->TypeAbil){
							case 5:
								return 0; // Bottle
							case 4:
								return 1; // Clothe
							case 1:
								return 2; // Magic
						};
					}
				}
			}
		//}
	}
	return 3;
}
void cva_Inventory_ShopPortret::OnDraw(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		OneObject* OB=SP->OB;
		if(OB->ActiveAbility && OB->ActiveAbility->LibID!=0xFFFF ){
			OneObject* Lib=Group[OB->ActiveAbility->LibID];
			if( Lib && Lib->Serial==OB->ActiveAbility->LibSN ){
				int mt=DetectMagazineType(Lib);
				if( mt>=0 && mt<4 ){ //v_ISys.PortretShop.GetAmount()
					OneSpriteExIcon* OS=&v_ISys.PortretShop[mt];
					//DrawRotatedPicture(SD->x,SD->y,OS->FileID,OS->StartSpriteID,MyNation,0,0,0,1.0f,1.0f);
					//ISM->Flush();
					OS->Draw(SD->x,SD->y,0,0,MyNation);
				}
			}
		}
	}
	//OneSpriteExIcon
} 
void cva_Inventory_ShopPortret::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		OneObject* OB=SP->OB;
		if(OB->ActiveAbility && OB->ActiveAbility->LibID!=0xFFFF ){
			OneObject* Lib=Group[OB->ActiveAbility->LibID];
			if( Lib && Lib->Serial==OB->ActiveAbility->LibSN ){
				int mt=DetectMagazineType(Lib);
				if( mt>=0 && mt<4 ){ //v_ISys.PortretShop.GetAmount()
					SD->Visible=true;
					GPPicture* GP=dynamic_cast<GPPicture*>(SD);
					if(GP){
						OneSpriteExIcon* OS=&v_ISys.PortretShop[mt];
						GP->FileID=OS->FileID;
						GP->SetSpriteID(0/*OS->StartSpriteID*/);
					}
				}
			}
		}
	}
}

void cva_Inventory_TabButton::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		TabButton* TB=dynamic_cast<TabButton*>(SD);
		if( TB ){
			OneObject* OB=SP->OB;
			OneObject* Lib=NULL;
			if( /*IGI.Library &&*/ OB->ActiveAbility && OB->ActiveAbility->LibID!=0xFFFF ){
				Lib=Group[OB->ActiveAbility->LibID];
				if( Lib && Lib->Serial!=OB->ActiveAbility->LibSN ){
					Lib=NULL;
				}
			}
			//switch(mSetHeroTab){
			//case 5:
			//	mt=0; // Bottle
			//	break;
			//case 4:
			//	mt=1; // Clothe
			//	break;
			//case 1:
			//	mt=2; // Magic
			//	break;
			//};
			int mt=-1;
			int ut=-1;
			Enumerator* E=ENUM.Get("InventaryHeroTab");
			//if( Lib ){
			//}else{
			//}
			if(mSetHeroTab!=-1){
				mt=E->FindVal(mSetHeroTab);
			}else{
				if( Lib && IGI.Library ){
					mt=DetectMagazineType(Lib);
				}
			}
			if(TB->State==0){
				if( mt!=-1 && mt==SD->ID ){
					TB->_Click();
					if( mSetHeroTab!=-1 ){
						vui_IS_ExitMagazine EM(SP->OB);
						EM.InterfAction();
						IGI.Library=false;
					}
					mSetHeroTab=-1;					
				}
			}else{
				if( !Lib && mt!=-1 && mt==SD->ID ){
					mSetHeroTab=-1;
				}
				if( mSetHeroTab==-1 && SD->ID!=0xFFFFFFFF ){
					extern int lastHeroTab;
					lastHeroTab=E->GetVal(SD->ID);
				}
			}
		}
	}
}
bool cva_Inventory_TabButton::LeftClick(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){
		OneObject* OB=SP->OB;
		if(OB->ActiveAbility && OB->ActiveAbility->LibID!=0xFFFF ){
			OneObject* Lib=Group[OB->ActiveAbility->LibID];
			if( Lib && Lib->Serial==OB->ActiveAbility->LibSN ){
				int mt=DetectMagazineType(Lib);
				if(mt!=SD->ID){
					vui_IS_ExitMagazine EM(OB);
					EM.InterfAction();
					IGI.Library=false;
					Enumerator* E=ENUM.Get("InventaryHeroTab");
					mSetHeroTab=E->GetVal(SD->ID);
				}
			}
		}
	}
	return true;
}

void cva_SP_Class::SetFrameState(SimpleDialog* SD){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	SD->Visible=SPClass.Check(SP); 
}

void cva_SP_Amount::SetFrameState(SimpleDialog* SD){
	int n=OIS.SelPoint.GetAmount();
	if(n==1){
		SD->Visible=!MultySel;
	}else
	if(n>0){
		SD->Visible=MultySel;
	}else{
		SD->Visible=false;
	}
}

bool cva_ClearClick::LeftClick(SimpleDialog* SD){
	VitCursor=-1;
	return true;
}
bool cva_ClearClick::RightClick(SimpleDialog* SD){
	VitCursor=-1;
	return true;
}
