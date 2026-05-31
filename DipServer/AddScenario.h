//------------------------NEW! FUNCTIONS----------------------//

/*
ReportFn* ORDLIST[OrdCount]={
	&BuildObjLink,
	&ProduceObjLink,
	&AttackObjLink,
	&TakeResLink,
	&PerformUpgradeLink,
	&GoToMineLink,
	&LeaveMineLink,
	&TakeResourceFromSpriteLink,
	&NewMonsterSendToLink,
	&DeleteBlockLink,
	&SetUnlimitedLink,
	&ClearUnlimitedLink,
	&WaterNewMonsterSendToLink,
	&AbsorbObjectLink,
	&LeaveShipLink,
	&BuildWallLink,
	&WaterAttackLink,
	&LeaveTransportLink,
	&GoToTransportLink,
	&FishingLink,
	&CreateGatesLink,
	&NewMonsterSmartSendToLink,
	&AI_AttackPointLink,
	&GrenaderSupermanLink,
	&GotoFinePositionLink,
	&NewMonsterPreciseSendToLink,
	&RotUnitLink,
	&PatrolLink,
	&RotateShipAndDieLink,
	&SearchArmyLink,
	&NewAttackPointLink,
	&MakeOneShotLink
};
*/

struct UnitExCaps{
	int Direction;
	int DestX;
	int DestY;
	int Type;
	char* UnitID;
	int NI;
	int StandTime;
	int OrderType;
};
DLLIMPORT bool GetUnitExCaps(int Index,UnitExCaps* CAPS,bool NeedOrderType);

DLLIMPORT bool GetZoneCoor(GAMEOBJ* Zone,int* x,int* y);
DLLIMPORT void PastePiece(GAMEOBJ* Zone,char* Name);
DLLIMPORT void SelectBuildingsInZone(byte NI,GAMEOBJ* Zone,GAMEOBJ* UTP,bool Add);

extern void SendAndKill(PlayerColor Col, GAMEOBJ* Group, GAMEOBJ* DestZone, byte Type);

DLLIMPORT void InviteUnitIntoBuilding(int BIndex,int UIndex,byte Type);
DLLIMPORT void LeaveBuilding(int BIndex,int Type);
DLLIMPORT int GetInsideList(int BIndex,word** List);

DLLIMPORT void ChangeUnitColor(int Index,byte DstNI);
DLLIMPORT void AddUnitToSel(int Index);

// -----------------
DLLIMPORT byte GetUnitNMASK(int Index);
DLLIMPORT void SetNMASK(int Index,byte NMASK);
DLLIMPORT int GetUnitIcon(int Type);
DLLIMPORT int GetAmountOfGroups();
DLLIMPORT int GetAmountOfZones();
DLLIMPORT char* GetZoneName(int Index);
DLLIMPORT char* GetGroupName(int Index);
DLLIMPORT char* GetUnitName(int Type);
DLLIMPORT char* GetUnitID(int Type);

DLLIMPORT bool GetUnitGlobalInfo(int Index,OneUnit* Uni);

//int InsertUnitToGroup(GAMEOBJ* Src,GAMEOBJ* Dst,int Index);
// -----------------

//byte GetNMASK(int Index){
//	return GetUnitNMASK(int Index);
//};
extern void SetRadius(GAMEOBJ* Zone, word R);
extern int GetZoneRadius(GAMEOBJ* Zone);
extern void SetZone(GAMEOBJ* Zone, int X, int Y, int R);

// Groups
DLLIMPORT int InsertUnitToGroup(GAMEOBJ* Src,GAMEOBJ* Dst,int Index);
inline int InsertUnitToGroup(GAMEOBJ* Grp,int Index){
	return InsertUnitToGroup(NULL, Grp, Index);
};
DLLIMPORT void RemoveUnitFromGroup(GAMEOBJ* Src,int Index);
DLLIMPORT void ClearGroup(GAMEOBJ* Grp);
void RemoveUnits(GAMEOBJ* Src);
DLLIMPORT void DeleteUnitFromGroup(GAMEOBJ* Grp, int Index);
DLLIMPORT int CleanGroup(GAMEOBJ* Grp);
DLLIMPORT int CollectUnits(GAMEOBJ* Dst,GAMEOBJ* Src,int N,int NIndex=0xFFFF);
DLLIMPORT int RemoveNUnits(GAMEOBJ* Dst,GAMEOBJ* Src,int N,int NIndex=0xFFFF);
DLLIMPORT void DeleteHiddenUnits(GAMEOBJ* Grp);

DLLIMPORT bool DetectPanic(int Index);
DLLIMPORT int RemovePanicUnits(GAMEOBJ* Src,GAMEOBJ* Dst);

DLLIMPORT int GetMaxPeaceTime();
DLLIMPORT int GetPeaceTimeLeft();
