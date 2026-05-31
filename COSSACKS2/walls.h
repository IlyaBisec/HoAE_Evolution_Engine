#pragma once
class WallCharacter{
public:
    char* Name;
    word RIndex;
    word NBuild;
    word Ndamage;
    short dx,dy;
	word GateFile;
	short GateDx;
	short GateDy;
	short UpgradeGateIcon;
	short OpenGateIcon;
	short CloseGateIcon;
	short UpgradeTower1;
	short UpgradeTower2;
	word  GateCost[8];
	short TexRadius;
	short NTex;
	short TexPrec;
	short* Tex;
	short OpenGateSoundID;
	short CloseGateSoundID;
	byte  OpenKeyFrame;
	byte  CloseKeyFrame;
};
class WallCluster;
class WallCell{
public:
    short x;
    short y;
    byte DirMask;
    byte Type;
    byte NI;
    byte Stage;
    byte MaxStage;
    word Health;
    word MaxHealth;
    byte Sprite;
    byte SprBase;
	word ClusterIndex;
	word OIndex;
	word GateIndex;
	int Locks;
    bool Visible;
    bool CheckPosition();
	int GetLockStatus();
	void SetLockStatus();
    bool StandOnLand(WallCluster* WC);
	void CreateLocking(WallCluster* WC);
	void Landing(WallCluster* WC);
    void ClearLocking();
};
class WallSystem;
class WallCluster{
public:
    byte Type;
    int NCornPt;
    word* CornPt;
    int NCells;
    WallCell* Cells;
    WallSystem* WSys;
    short LastX;
    short LastY;
    short FinalX;
    short FinalY;
    NewMonster* NM;
	word  NIndex;
	byte  NI;
//------------------//
    WallCluster();
    ~WallCluster();
    void SetSize(int N);
    void ConnectToPoint(short x,short y);
	void ConnectToPoint(short x,short y,bool Vis);
    void UndoSegment();
    void SetPreviousSegment();
    void KeepSegment();
    void CreateSprites();
    void AddPoint(short x,short y,bool Vis);
    void Preview();
    void View();
	int  CreateData(word* Data,word Health);
	void CreateByData(word* Data);
	void SendSelectedToWork(byte NI,byte OrdType);
};
class WallSystem{
public:
    int NClusters;
    WallCluster** WCL;
//-------------------//
    WallSystem();
    ~WallSystem();
     void AddCluster(WallCluster*);
     void WallSystem::Show();
};
inline int GetLI(int x,int y){
    return x+(y<<(VAL_SHFCX+1));
};
extern int MaxLI;
extern int MaxLIX;
extern int MaxLIY;
extern int NChar;
extern WallCharacter WChar[32];
void DetermineWallClick(int x,int y);
void SetLife(WallCell* WC,int Health);
//gates
#define NGOpen 9
