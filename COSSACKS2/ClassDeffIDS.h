
#ifndef __Class_Deff_IDS__
#define __Class_Deff_IDS__

extern	_str	ggClassDescr;
extern char GlobalTextMouseOverCommand[256];

// ADITION ///////////////////////////////////////////////////////////////
#define _lvTypeNum_			('LVA'+'ADD'+0)		// lvTypeNum
#define _lvGrpNumBld_		('LVA'+'ADD'+1)		// lvGrpNumBld

// CLASS IDS /////////////////////////////////////////////////////////////
#define _lvCStorage_		('LVA'+'DTX'+0)		// lvCStorage
#define _be_CNODE_			('LVA'+'DTX'+1)		// be_CNODE
#define _lvCNodesMAP_ST_	('LVA'+'DTX'+2)		// lvCNodesMAP_ST
#define _lvCEdge_			('LVA'+'DTX'+3)		// lvCEdge
#define _lvCSquardShema_	('LVA'+'DTX'+4)		// lvCSquardShema
#define _lvCMainScript_		('LVA'+'DTX'+5)		// lvCMainScript
#define _lvCBattleShema_ST_	('LVA'+'DTX'+6)		// lvCBattleShema_ST
#define _be_CGROUP_			('LVA'+'DTX'+7)		// be_CGROUP
#define _lvCGroupsMAP_ST_	('LVA'+'DTX'+8)		// lvCGroupsMAP_ST
#define _vvMAP_ST_			('LVA'+'DTX'+9)		// vvMAP_ST
#define _vvBASE_			('LVA'+'DTX'+10)	// vvBASE
#define _be_CBOOL_		('LVA'+'DTX'+11)	// be_CBOOL
#define	_be_CDWORD_			('LVA'+'DTX'+12)	// be_CDWORD
#define	_be_CINT_			('LVA'+'DTX'+13)	// be_CINT
#define _lvCFilm_			('LVA'+'DTX'+14)	// lvCFilm
#define _vvPOINT_SET_		('LVA'+'DTX'+15)	// vvPOINT_SET
#define _vvPOINT2D_			('LVA'+'DTX'+16)	// vvPOINT2D
#define _vvTEXT_			('LVA'+'DTX'+17)	// vvTEXT
#define _vvVector3D_		('LVA'+'DTX'+18)	// vvVector3D
#define _vvDIALOG_			('LVA'+'DTX'+19)	// vvDIALOG
#define _vvMESSGES_			('LVA'+'DTX'+20)	// vvMESSGES
#define _lvCGroupUG_		('LVA'+'DTX'+21)	// lvCGroupUG
#define _vvMissionLOG_		('LVA'+'DTX'+22)	// vvMissionLOG
#define _vvPICTURE_			('LVA'+'DTX'+23)	// vvPICTURE
#define _vvMISSMGR_			('LVA'+'DTX'+24)	// vvMISSMGR
#define _vvBrigAI_			('LVA'+'DTX'+25)	// vvBrigAI
#define _vvTASKS_CII_		('LVA'+'DTX'+26)	// vvTASKS_CII
#define _vvTEXT_EX_			('LVA'+'DTX'+27)	// vvTEXT_EX
#define _vvFuzzyRule_		('LVA'+'DTX'+28)	// vvFuzzyRule
//////////////////////////////////////////////////////////////////////////

// OPERATIONS CLASS IDS //////////////////////////////////////////////////
#define	_lvCCondition_			('LVA'+'CON'+0)		// lvCCondition
#define	_lvCAriphmetical_		('LVA'+'CON'+1)		// lvCAriphmetical
#define	_lvCLogical_			('LVA'+'CON'+2)		// lvCLogical
#define	_lvCCompare_			('LVA'+'CON'+3)		// lvCCompare
#define	_lvCFunctional_			('LVA'+'CON'+4)		// lvCFunctional
#define _lvCOperation_			('LVA'+'OPR'+0)		// lvCOperation
#define _lvCCondForOper_		('LVA'+'OPR'+1)		// lvCCondForOper
#define _lvCSetUnitState_		('LVA'+'OPR'+2)		// lvCSetUnitState
#define _lvCSelectUnits_		('LVA'+'OPR'+3)		// lvCSelectUnits
#define _lvCSelSendTo_			('LVA'+'OPR'+4)		// lvCSelSendTo
#define _lvCGroupSendTo_		('LVA'+'OPR'+5)		// lvCGroupSendTo
#define _lvCSelSendToNode_		('LVA'+'OPR'+6)		// lvCSelSendToNode
#define _lvCGroupSendToNode_	('LVA'+'OPR'+7)		// lvCGroupSendToNode
#define _lvCRotateGroup_		('LVA'+'OPR'+8)		// lvCRotateGroup
#define _lvCSendToNode_			('LVA'+'OPR'+9)		// lvCSendToNode
#define _lvCSetState_			('LVA'+'OPR'+10)	// lvCSetState
#define _lvCReformation_		('LVA'+'OPR'+11)	// lvCReformation
#define _lvCBrigReformation_	('LVA'+'OPR'+12)	// lvCBrigReformation
#define _lvCChangeFriends_		('LVA'+'OPR'+13)	// lvCChangeFriends
#define _lvCSetLightSpot_		('LVA'+'OPR'+14)	// lvCSetLightSpot
#define _lvCClearLightSpot_		('LVA'+'OPR'+15)	// lvCClearLightSpot
#define _lvCSetStartPoint_		('LVA'+'OPR'+16)	// lvCSetStartPoint
#define _lvCShowVictory_		('LVA'+'OPR'+17)	// lvCShowVictory
#define _lvCLooseGame_			('LVA'+'OPR'+18)	// lvCLooseGame
#define _lvCTakeFood_			('LVA'+'OPR'+19)	// lvCTakeFood
#define _lvCTakeWood_			('LVA'+'OPR'+20)	// lvCTakeWood
#define _lvCTakeStone_			('LVA'+'OPR'+21)	// lvCTakeStone
#define _lvCSetTrigg_			('LVA'+'OPR'+22)	// lvCSetTrigg
#define _lvCBaseOperCond_		('LVA'+'OPR'+23)	// lvCBaseOperCond
#define _lvCRotate_				('LVA'+'OPR'+24)	// lvCRotate
#define _lvCSetValue_			('LVA'+'OPR'+25)	// lvCSetValue
#define _lvCKillNUnits_			('LVA'+'OPR'+26)	// lvCKillNUnits
#define _lvCEraseNUnits_		('LVA'+'OPR'+27)	// lvCEraseNUnits
#define _lvCSetRessource_		('LVA'+'OPR'+28)	// lvCSetRessource
#define _lvCGetValue_			('LVA'+'OPR'+29)	// lvCGetValue
#define _lvCChangeNation_		('LVA'+'OPR'+30)	// lvCChangeNation
#define _lvCChangeNationG_		('LVA'+'OPR'+31)	// lvCChangeNationG
#define _lvCGetScreenXY_		('LVA'+'OPR'+32)	// lvGetScreenXY
#define _lvCStartAIEx_			('LVA'+'OPR'+33)	// lvCStartAIEx
#define _lvCSetFGV_				('LVA'+'OPR'+34)	// lvCSetFGV
#define	_lvCRunTimer_			('LVA'+'OPR'+35)	// lvCRunTimer
#define _lvCSetGameMode_		('LVA'+'OPR'+36)	// lvCSetGameMode
#define _lvCShowDialog_			('LVA'+'OPR'+37)	// lvCShowDialog
#define _lvCAddTextToDlg_		('LVA'+'OPR'+38)	// lvCAddTextToDlg
#define _lvCClearDialog_		('LVA'+'OPR'+39)	// lvCClearDialog
#define _lvCSetFriends_			('LVA'+'OPR'+40)	// lvCSetFriends
#define _lvCSavePosition_		('LVA'+'OPR'+41)	// lvCSavePosition
#define _lvCSendToPosition_		('LVA'+'OPR'+42)	// lvCSendToPosition
#define _lvCPlayOGMiss_			('LVA'+'OPR'+43)	// lvCPlayOGMiss
#define _lvCStopOGMiss_			('LVA'+'OPR'+44)	// lvCStopOGMiss
#define _lvCOGSetVolume_		('LVA'+'OPR'+45)	// lvCOGSetVolume
#define _lvCOGFinishMiss_		('LVA'+'OPR'+46)	// lvCOGFinishMiss
//#define _lvCSetPlayGameMode_	('LVA'+'OPR'+47)	// lvCSetPlayGameMode
#define _lvCFreezeGame_					('LVA'+'OPR'+48)	// lvCFreezeGame
#define _lvCUnFreezeGame_				('LVA'+'OPR'+49)	// lvCUnFreezeGame
#define _lvCFreezeAndHidden_			('LVA'+'OPR'+50)	// lvCFreezeAndHidden
#define _lvCUnFreezeAndUnHidden_		('LVA'+'OPR'+51)	// lvCUnFreezeAndUnHidden
#define _lvCFreezeAndHiddenGame_		('LVA'+'OPR'+52)	// lvCFreezeAndHiddenGame
#define _lvCUnFreezeAndUnHiddenGame_	('LVA'+'OPR'+53)	// lvCUnFreezeAndUnHiddenGame
#define _lvCUnFreezeGroup_				('LVA'+'OPR'+54)	// lvCUnFreezeGroup
#define _lvCUnFreezeAndUnHiddenGroup_	('LVA'+'OPR'+55)	// lvCUnFreezeAndUnHiddenGroup
#define _lvCSetLeftPort_				('LVA'+'OPR'+56)	// lvCSetLeftPort
#define _lvCSetRightPort_				('LVA'+'OPR'+57)	// lvCSetRightPort
#define _lvCPlayText_					('LVA'+'OPR'+58)	// lvCPlayText
#define _lvCSetText_					('LVA'+'OPR'+59)	// lvCSetText
#define _lvCSetActivFrame_				('LVA'+'OPR'+60)	// lvCSetActivFrame
#define _lvCSetScrollLimit_				('LVA'+'OPR'+61)	// lvCSetScrollLimit
#define _lvCSpotNUnits_					('LVA'+'OPR'+62)	// lvCSpotNUnits
#define _lvCGoInBattle_					('LVA'+'OPR'+63)	// lvCGoInBattle
#define _lvCAddToInt_					('LVA'+'OPR'+64)	// lvCAddToInt
#define	_lvCSetLMode_					('LVA'+'OPR'+65)	// lvCSetLMode
#define	_lvCSetFogMode_					('LVA'+'OPR'+66)	// lvCSetFogMode
#define _lvCArtAttack_					('LVA'+'OPR'+67)	// lvCArtAttack
#define _lvCSetMessageState_			('LVA'+'OPR'+68)	// lvCSetMessageState
#define _lvCSelectAll_					('LVA'+'OPR'+69)	// lvCSelectAll
#define _lvCPutNewSquad_				('LVA'+'OPR'+70)	// lvCPutNewSquad
#define	_lvCSetUnitStateCII_			('LVA'+'OPR'+71)	// lvCSetUnitStateCII
#define	_lvCSendStikiToZone_			('LVA'+'OPR'+72)	// lvCSendStikiToZone
#define _lvCSetUnitEnableState_			('LVA'+'OPR'+73)	// lvCSetUnitEnableState
#define	_lvCSetUpgradeEnableStatus_		('LVA'+'OPR'+74)	// lvCSetUpgradeEnableStatus
#define _lvCBaseScript_					('LVA'+'OPR'+75)	// lvCBaseScript
#define	_lvCSubSection_					('LVA'+'OPR'+76)	// lvCSubSection
#define	_lvCAttachCameraToGroup_		('LVA'+'OPR'+77)	// lvCAttachCameraToGroup
#define _lvCFreeCamera_					('LVA'+'OPR'+78)	// lvCFreeCamera
#define	_lvCTeleport_					('LVA'+'OPR'+79)	// lvCTeleport
#define _lvCPutNewFormation_			('LVA'+'OPR'+80)	// lvCPutNewFormation
#define _lvCAddToIntEx_					('LVA'+'OPR'+81)	// lvCAddToIntEx
#define _lvCDisband_					('LVA'+'OPR'+82)	// lvCDisband
#define _lvCScare_						('LVA'+'OPR'+83)	// lvCScare
#define _lvCClearSG_					('LVA'+'OPR'+84)	// lvCClearSG
#define _lvCAddRessource_				('LVA'+'OPR'+85)	// lvCAddRessource
#define _lvCSpotGrpByUType_				('LVA'+'OPR'+86)	// lvCSpotGrpByUType
#define _lvCRemoveNUnitsTo_				('LVA'+'OPR'+87)	// lvCRemoveNUnitsTo
#define _lvCApplyTerafoming_			('LVA'+'OPR'+88)	// lvCApplyTerafoming
#define _lvCSelSetState_				('LVA'+'OPR'+89)	// lvCSelSetState
#define _lvCPushNUnitAway_				('LVA'+'OPR'+90)	// lvCPushNUnitAway
#define _lvCSendUnitsToTransport_		('LVA'+'OPR'+91)	// lvCSendUnitsToTransport
#define _lvCSetMyNation_				('LVA'+'OPR'+92)	// lvCSetMyNation
#define _lvCEqualizeSpeed_				('LVA'+'OPR'+93)	// lvCEqualizeSpeed
#define _lvCSetGameSpeed_				('LVA'+'OPR'+94)	// lvCSetGameSpeed
#define _lvCGetGameSpeed_				('LVA'+'OPR'+95)	// lvCGetGameSpeed
#define _lvCCreateBrigade_				('LVA'+'OPR'+96)	// lvCCreateBrigade
#define _lvCShowPanel_					('LVA'+'OPR'+97)	// lvCShowPanel
#define _lvCAddWallSegment_				('LVA'+'OPR'+98)	// lvCAddWallSegment
#define _lvCSetAIEnableState_			('LVA'+'OPR'+99)	// lvCSetAIEnableState
#define _lvCAddFarms_					('LVA'+'OPR'+100)	// lvCAddFarms
#define _lvCQuestData_					('LVA'+'OPR'+101)	// lvCQuestData
#define _lvCReStartSquadShema_			('LVA'+'OPR'+102)	// lvCReStartSquadShema
#define _lvCChangeAS_					('LVA'+'OPR'+103)	// lvCChangeAS
#define _lvCKillNatinZone_				('LVA'+'OPR'+104)	// lvCKillNatinZone
#define _lvCKillNatNear_				('LVA'+'OPR'+105)	// lvCKillNatNear
#define	_lvCSetSerchWFlag_				('LVA'+'OPR'+106)	// lvCSetSerchWFlag
#define _lvCShowMessageII_				('LVA'+'OPR'+107)	// lvCShowMessageII
#define _lvCBrigPanelSet_				('LVA'+'OPR'+108)	// lvCBrigPanelSet
#define _lvCFilmCopliteState_			('LVA'+'OPR'+109)	// lvCFilmCopliteState
#define _lvCClearOrders_				('LVA'+'OPR'+110)	// lvCClearOrders
#define _lvCGroupHoldNode_				('LVA'+'OPR'+111)	// lvCGroupHoldNode
#define _lvCUnloadSquad_				('LVA'+'OPR'+112)	// lvCUnloadSquad
#define _lvCAddElemTHE_CII_				('LVA'+'OPR'+113)	// lvCAddElemTHE_CII
#define _lvCDelElemTHE_CII_				('LVA'+'OPR'+114)	// lvCDelElemTHE_CII
#define _lvCPAUSE_						('LVA'+'OPR'+115)	// lvCPAUSE
#define _lvCSET_MISS_MANAGER_			('LVA'+'OPR'+116)	// lvCSET_MISS_MANAGER
#define _lvCBrigadesAmount_				('LVA'+'OPR'+117)	// lvCBrigadesAmount
#define _lvCGroupMovement_				('LVA'+'OPR'+118)	// lvCGroupMovement
#define _lvCTestFillingAbility_			('LVA'+'OPR'+119)	// lvCTestFillingAbility
#define _lvCInStandGround_				('LVA'+'OPR'+120)	// lvCInStandGround
#define _lvCSetTired_					('LVA'+'OPR'+121)	// lvCSetTired
#define _lvCArtChangeCharge_			('LVA'+'OPR'+122)	// lvCArtChangeCharge
#define _lvCArtAttackPoint_				('LVA'+'OPR'+123)	// lvCArtAttackPoint
#define	_lvCSetUpgradeDone_				('LVA'+'OPR'+124)	// lvCSetUpgradeDone
#define	_lvCClearDead_					('LVA'+'OPR'+125)	// lvCClearDead
#define _lvCSetSilence_					('LVA'+'OPR'+126)	// lvCSetSilence
// Xeno:
#define _lvCSetGameType_				('LVA'+'OPR'+127)	// lvCSetGameType
#define _lvCGetUnitMoveType_			('LVA'+'OPR'+128)	// lvCGetUnitLockType
#define _lvCSetUnitMoveType_			('LVA'+'OPR'+129)	// lvCSetUnitLockType
#define _lvCPutNewFormationParametric_	('LVA'+'OPR'+130)	// lvCPutNewFormationParametric
#define _lvCIncreaseLevel_				('LVA'+'OPR'+131)	// lvIncreaseLevel
#define _lvCSetScrollLimitNation_		('LVA'+'OPR'+132)	// lvCSetScrollLimit

//////////////////////////////////////////////////////////////////////////

// FUNCTIONS /////////////////////////////////////////////////////////////
#define _lvCBaseFunctions_		('LVA'+'FUN'+0)		// lvCBaseFunctions

#define _lvCBool_				('LVA'+'FUN'+1)		// lvCBool
#define _lvCInt_				('LVA'+'FUN'+2)		// lvCInt
#define _lvCChkTime_			('LVA'+'FUN'+3)		// lvCChkTime
#define _lvCGetUnitsAmount0_	('LVA'+'FUN'+4)		// lvCGetUnitsAmount0
#define _lvCGetUnitsAmount1_	('LVA'+'FUN'+5)		// lvCGetUnitsAmount1
#define _lvCGetUnitsAmount2_	('LVA'+'FUN'+6)		// lvCGetUnitsAmount2
#define _lvCGetTotalAmount0_	('LVA'+'FUN'+7)		// lvCGetTotalAmount0
#define _lvCGetTotalAmount1_	('LVA'+'FUN'+8)		// lvCGetTotalAmount1
#define _lvCGetTotalAmount2_	('LVA'+'FUN'+9)		// lvCGetTotalAmount2
#define _lvCGetReadyAmount_		('LVA'+'FUN'+10)	// lvCGetReadyAmount
#define _lvCGetResource_		('LVA'+'FUN'+11)	// lvCGetResource
#define _lvCGetDiff_			('LVA'+'FUN'+12)	// lvCGetDiff
#define _lvCProbably_			('LVA'+'FUN'+13)	// lvCProbably
#define _lvCGetUnitState_		('LVA'+'FUN'+14)	// lvCGetUnitState
#define _lvCTrigg_				('LVA'+'FUN'+15)	// lvCTrigg
#define _lvCAllGrpInNode_		('LVA'+'FUN'+16)	// lvCAllGrpInNode
#define _lvCAllGrpInNodeFree_	('LVA'+'FUN'+17)	// lvCAllGrpInNodeFree
#define _FG_Visible_			('LVA'+'FUN'+18)	// FG_Visible
#define _FG_InVisible_			('LVA'+'FUN'+19)	// FG_InVisible
#define _ogSTOP_				('LVA'+'FUN'+20)	// ogSTOP
#define	_lvCTimerDone_			('LVA'+'FUN'+21)	// lvCTimerDone
#define	_lvCChekPosition_		('LVA'+'FUN'+22)	// lvCChekPosition
#define _lvCGrpInNode_			('LVA'+'FUN'+23)	// lvCGrpInNode
#define _lvCGrpInNodeFree_		('LVA'+'FUN'+24)	// lvCGrpInNodeFree
#define _lvCNationIsErased_		('LVA'+'FUN'+25)	// lvCNationIsErased
#define _lvCGetLMode_			('LVA'+'FUN'+26)	// lvCGetLMode
#define	_lvCCheckButton_		('LVA'+'FUN'+27)	// lvCCheckButton
#define	_lvCGetAmount_			('LVA'+'FUN'+28)	// lvCGetAmount
#define _lvCIsBrigade_			('LVA'+'FUN'+29)	// lvCIsBrigade
#define _lvCGetFormationType_	('LVA'+'FUN'+30)	// lvCGetFormationType
#define _lvCPrOfSquadInNode_	('LVA'+'FUN'+31)	// lvCPrOfSquadInNode
#define _lvCGetUnitsAmount3_	('LVA'+'FUN'+32)	// lvCGetUnitsAmount3
#define	_lvCGetNInside_			('LVA'+'FUN'+33)	// lvCGetNInside
#define _lvCCheckLeaveAbility_	('LVA'+'FUN'+34)	// lvCCheckLeaveAbility
#define _lvCLoadingCoplite_		('LVA'+'FUN'+35)	// lvCLoadingCoplite
#define	_lvCGetNofNewUnitInGrp_	('LVA'+'FUN'+36)	// lvCGetNofNewUnitInGrp
#define _lvCGetNofMyVillage_	('LVA'+'FUN'+37)	// lvCGetNofMyVillage
#define _lvCCheckRBBP_			('LVA'+'FUN'+39)	// lvCCheckRBBP
#define _lvCIsTired_			('LVA'+'FUN'+40)	// lvCIsTired
#define _lvCVillageOwner_		('LVA'+'FUN'+41)	// lvCVillageOwner
#define _lvCGetNofBrigInNode_	('LVA'+'FUN'+42)	// lvCGetNofBrigInNode
#define _lvCGetCurGrpORDER_		('LVA'+'FUN'+43)	// lvCGetCurGrpORDER
#define _lvCGetNofBRLoadedGun_	('LVA'+'FUN'+44)	// lvCGetNofBRLoadedGun
#define	_lvCGetTime_			('LVA'+'FUN'+45)	// lvCGetElapsedTime
// Xeno:
#define _lvCGetUnitsAmountPar_	('LVA'+'FUN'+46)	// lvCGetUnitsAmountPar
//////////////////////////////////////////////////////////////////////////

// GRAPHICS //////////////////////////////////////////////////////////////
#define _lvCGraphObject_		('LVA'+'GRO'+0)		// lvCGraphObject
#define _lvCBlackScreen_		('LVA'+'GRO'+1)		// lvCBlackScreen
#define _lvCMoveGP_				('LVA'+'GRO'+2)		// lvCMoveGP
#define _lvCDeffFilmMenu_		('LVA'+'GRO'+3)		// lvCDeffFilmMenu
#define _lvCAAppearGP_			('LVA'+'GRO'+4)		// lvCAAppearGP
#define _lvCAnimateGP_			('LVA'+'GRO'+5)		// lvCAnimateGP
#define _lvCDeffAnimeFilmMenu_	('LVA'+'GPO'+6)		// lvCDeffAnimeFilmMenu
#define _lvCTeraforming_		('LVA'+'GRO'+7)		// lvCTeraforming
#define _lvCDialogBased_		('LVA'+'GRO'+8)		// lvCDialogBased
//////////////////////////////////////////////////////////////////////////

// CAMERA ////////////////////////////////////////////////////////////////
#define	_lvCSetCamera_			('LVA'+'CAM'+0)		// lvCSetCamera
#define _lvCMoveCamera_			('LVA'+'CAM'+1)		// lvCMoveCamera
#define _lvCCameraSTOP_			('LVA'+'CAM'+2)		// lvCCameraSTOP
#define _lvCSaveScreenPos_		('LVA'+'CAM'+3)		// lvCSaveScreenPos
//////////////////////////////////////////////////////////////////////////

// MESSAGES //////////////////////////////////////////////////////////////
#define _BaseMesMgrST_			('LVA'+'MSG'+0)		// BaseMesMgrST
#define _SimpleMesMgr_			('LVA'+'MSG'+1)		// SimpleMesMgr
//////////////////////////////////////////////////////////////////////////

// be_CNODE ///////////////////////////////////////////////////////////////
#define _RECT_		0x00000001	//Draw node rect
#define _XY_		0x00000002	//Draw node x,y coord
#define _NAME_		0x00000004	//Draw node name
#define _DESCR_		0x00000008	//Draw node description
//////////////////////////////////////////////////////////////////////////

// lvCEdge ///////////////////////////////////////////////////////////////
#define	_RECT_			0x00000001	//Draw edge rect
#define _DIRECTION_		0x00000010	//Darw edge direction
#define	_PROC_TIME_		0x00000020	//Darw edge process time
#define	_START_TIME_	0x00000040	//Darw edge start time
#define _MODE_			0x00000080	//Darw edge mode
//////////////////////////////////////////////////////////////////////////

#endif//__Class_Deff_IDS__