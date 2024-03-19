#pragma warning ( disable : 4786 )

#include "UnitInterface.h"

#include <vector>
using namespace std;
typedef vector<word> VWord;

class CUpgCategory{
public:
	CUpgCategory(): Index(0xFFFF), Message(NULL) {};
	CUpgCategory( word _Index, char* _Message ) : Index(_Index), Message(_Message) {};
	CUpgCategory( word _Index, char* _Message, word* UList, int NUList);
	~CUpgCategory();
	//void Init();
	//void Free();

	word Index;
	char* Message;

	vector<VWord> Linked;		// индексы связанных апгрейдов
	vector<word> UnLinked;		// индексы не связанных апгрейдов
};

class CWeapInfo{
public:
	CWeapInfo();
	byte Kind;
	int Damage;
};

struct SMainChar{
	short* MaxDamage;
	byte* WeaponKind;
	byte* Protection;		// 16 max index
	word Shield;			// броня от любого типа вооружения
	word* AttackRadius2;
	word* AttackPause;
	word Life;
};

struct UnitType {
	word NIndex;

	// info	
	word Picture;		// индекс большой картинки с изображением рода войска
	
	int NAttTypes;
	SMainChar MC;

	//vector<CWeapInfo> WeapKind;	// вооружение рода войск	
	
	vector<short> CurDamage;
	VWord CurR2;
	VWord CurAPause;
	vector<byte> CurProtect;
	word CurShield;
	word CurLife;

	void GetCharacter(byte NI);

	// upgrades
	int NUpg;	
	word* Upg;	//  все возможные апгрейды для данного юнита

	vector<CUpgCategory> Category;	// список категорий апгрейдов
	void AddUpgCategory(word UpgCtg, char* Mess, word* UList, int NUList);
	void RunProtectUpg(byte Nat,int Level);

};

class CNation{
public:
	int NUpgCheck;		// заказанные
	bool* UpgCheck;
};

class CPlayerInfo{
public:	
	char Name[10];

	bool Connect;
	bool Ready;
	bool WaitStart;

	int NUT;
	UnitType* UT;	//	наличие данного типа юнитов у игрока
	int SelUT;

	void AddNewMon(byte NI, word NIndex);
	
	void CheckUpg(word UIndex, bool OldState);	// изменить текущие характеристики
	
};

void DoDamage(UnitType* UT, short AttIndex, byte Sign, byte Perc, int Value);
void DoProtect(UnitType* UT, short AttIndex, byte Sign, byte Perc, int Value);
void DoShield(UnitType* UT, short AttIndex, byte Sign, byte Perc, int Value);
void DoAttPause(UnitType* UT, short AttIndex, byte Sign, byte Perc, int Value);
typedef void (/*CPlayerInfo::*/*TDoUpgOperator) (UnitType* UT, short AttIndex, byte Sign, byte Perc, int Value);

class CBattle{
public:
	CBattle();
	~CBattle();
	void Free();
	
	bool Start;
	int StartTimer;
	char TimerVarName[28];
	int ShowTimer();
	void SetStartTimer(int Time);
	CPlayerInfo Info[8];
	CNation Nat[8];

	void Init();
	void Process();
	void MakeDialog(bool frame);
	void RunUpgrades();

	void AddUpgToCml(DString& CML, char* UPS, word UIndex, word NUpg, int x, int y);

};
extern CBattle Battle;

struct UpgInfo{
	word Icon;
	char* Hint;
};

CIMPORT void SetObservMode();
CIMPORT void SetNormalMode();
CIMPORT void ShowFogOfWar();
CIMPORT void HideFogOfWar();
CIMPORT int GetNMon(byte NI);
CIMPORT void GetPressentMonsters(byte NI, int* NIndex);
CIMPORT void GetListOfUpgradesForMon(byte NI,word NIndex,word* Upg,int MaxUpg,int& NUpg);
CIMPORT void GetListOfUpgradesForMon_Old(byte NI,word NIndex,word* Upg,int MaxUpg,int& NUpg);
CIMPORT int GetNUpgrades(byte NI);
CIMPORT char* GetUnitMessage(word NIndex);

CIMPORT char* GetUnitGPName(word NIndex, int& x, int& y);
CIMPORT int GetAttackNFrame(word NIndex, word AttIndex);

CIMPORT int GetUpgradeIcon(word UIndex);
CIMPORT char* GetUpgMessage(word UIndex);
CIMPORT bool GetUpgHint(byte NI, word UIndex, char* Str);
DLLIMPORT void InitialUpgradeNew(byte NI,GAMEOBJ* Upg,word Index);
CIMPORT void SetUpgEnableStatus(byte NI, word UIndex, bool Status);


DLLIMPORT void SXP_SetVar(int Index,char* Name,char* value);
DLLIMPORT char* SXP_GetVar(int Index,char* Name);
CIMPORT void GetUpgCost(byte NI, word UIndex, word* &Cost);
CIMPORT void CloseDipDialog();
CIMPORT void PopupDipDialog();
CIMPORT int GetUpgLevel(word UIndex);

CIMPORT char* GetPlayerName(byte NI);
CIMPORT byte GetPlayerColor(word ID);
CIMPORT byte GetMyPlayerColor();
CIMPORT byte GetMyPlayerID();
CIMPORT byte* GetNatRefTBL();

CIMPORT int GetMainChar(byte NI, word NIndex, SMainChar* MC);
//CIMPORT int GetMainChar(word NIndex, short* &MaxDamage, byte* &WeaponKind, byte* &Protection, word &Shield,	word* &AttackRadius2,word* &AttackPause);

CIMPORT word GetWeaponIcon(word WeaponKind);
CIMPORT word GetProtectIcon(word WeaponKind);

CIMPORT bool DetCannon(word NIndex);
CIMPORT bool DetSheep(word NIndex);

class NewUpgrade;
//{
//public:
//	//information
//	char* Name;
//	char* Message;
//	byte Level;
//	byte Branch;
//	word IconFileID;
//	word IconSpriteID;
//	char IconPosition;
//	byte NatID;
//	word  Cost[8];
//	byte CtgUpgrade;
//	byte CtgType;
//	union{
//		int  NCtg;
//		int  CtgValue;
//	};
//	word*    CtgGroup;
//	byte UnitType;
//	union{
//		int NUnits;
//		int UnitValue;
//	};
//	word* UnitGroup;
//	byte ValueType;
//	union{
//		int  Value;
//		int  NValues;
//	};
//	int* ValGroup;
//	//mechanics
//	bool Done:1;
//	bool Enabled:1;
//	bool PermanentEnabled:1;
//	bool IsDoing:1;
//	bool Gray:1;
//	bool Individual:1;
//	bool ManualDisable:1;
//	bool ManualEnable:1;
//	bool StageUp:1;
//	word NStages;
//	word CurStage;
//	byte NAutoPerform;
//	byte NAutoEnable;
//	word* AutoPerform;
//	word* AutoEnable;
//	byte Options;
//	byte StageMask;
//};

CIMPORT NewUpgrade* GetNewUpg(byte NI, word UIndex);

CIMPORT void SetDipDlgDX(int dx);
