//
//������ ����� ����� ������ �������������� ������. ����������� 
//(� ����������� �� ��������� ��������� ����� � ���������� �������,
//������, ����� 100 �������� ��� 10 ������)
//�� �������� ���� �������, ������� �������� ������ � ������ ������,
//���������� � ������ ��������� ����� (���������, �������, ������). 
//��� ������ ��� ���� ���������� ���� (����� ���������� ������). 
//��������� ���������� ���������� � �������� ��� ����������� �������.
//��� ������ � ������������ �������? ������ ����������� ���� 
//����������� ������� ������ � ���� ������ �������� ��������� 
//���� ����. �� ������� ������� ��������� ����� ������� �� 
//���������� �� ����� �� �����. ��������� ����������� ����� ��������
//������������ ������������ ����������. �� ���������� � ��� ���������
//� ������� ��������.
//��� ����������� ����� ���������� ��� ���� ������, ����� ������ 
//��������, ����� ��������������. �� �� ���� ��� ����� ��������� ������� �� 
//��� ������� ��������� ������� (������� 10 �), ��� ������, ������� 
//� ���� ������ ��� ������, ������� ��� ���������� � �������������� ������,
//� ��� ����� � ���������. ����� ��� ����������� ����� �� ������
//�������������� ������ ��������� ���, ��� �� � ���� ������. ��� ����
//����� ������ (�� ��������) ��������� ���������.
//��������� ������ - ��� ����� ���� �� ����� ������ ���� ��������, �� ��� ������,
//� ���� ������ �������� ��� ���������? ����� - ��� �������� �� ����� ������
//���������� � ������ �������������� ���������� �� ������ �������.
//��� ���� ����������, ��� ��������� � ����� ��� ���-�� ������� �������.
//���� ��� ����� ����-�� ������������� � ����� � ����, �� �� ���������� ����
//������� ��� ������ ��������� �� �������� � ��� ������.
#include "GameInterface.h"
#include "unichash.h"


class MassivePlayer:public BaseClass{
public:
	_str     Name;
	bool     Connected;
	DWORD    ID;	
	DWORD    ColorID;
    int      CellX;
    int      CellY;
    int      CenterX;
    int      CenterY;
    pUnit    CentralUnit;
	int      LastDT;
	SaveBuf  ToSend;
	SaveBuf  GotFrom;	
	int		 CreepsKilled;
	int		 HeroLevel;
	int		 Money;
};

struct UnitsRequest{
	int  StartX;
	int  StartY;
	int  EndX;
	int  EndY;
    int  NUnits;
};
struct mmpUnitUnderControl:public BaseClass{
    pUnit		Unit;
    bool		MyOwn;    
};
struct mmpOneMapCell{
	bool InfoIsActual;
	int  LastRequestTime;
};
struct SendState{
    int			PrevSendTime;
	iVector2D   PrevCoor;    
    int         PrevLife;
	Order1*     LocalOrder;
	int			Level;
    eLiveState  LiveStatus;
};
struct DataPack{
	BYTE* Data;
	int Size;
	DWORD Peer;
};
class MassiveRoom:public GameExtension{
public:
    static bool                         IsActive;    
	int                                 MapSizeX;
	int                                 MapSizeY;
	int                                 NCellsX;
	int                                 NCellsY;
    static int                          StartMyUnitsIndex;
    static int                          FinalMyUnitsIndex;        
	int									BytesSent;
	int									FirstSendTime;
	int									BytesReceived;
	int									FirstReceiveTime;	
	pUnitsList							ToDie;
	bool								InDie;

	void								ReportSend(int Size);
	void								ReportReceive(int Size);

    //static pUnit                      ActiveProducer;
	static int							ActiveSubset;
    ClonesArray<MassivePlayer>          Players;
    //pUnitsList                          UnitsUnderControl;    
    uni_hash<SendState,tri_DWORD>       LastSendInfo;//tri_DWORD(PlayerIndex,UnitID,UnitSN)
	uni_hash<DWORD,bi_DWORD>			PlayersMap;	
	uni_hash<DWORD,bi_DWORD>			OwnHistoryMap;	
    DynArray<int>                       VisiblePlayers;//at least one my cell is on his territory
    DynArray<int>                       WellVisiblePlayers;//at least one my unit is on his terrtory
	static DynArray<DataPack>			RecBuffer;	
	static int                          MyPlayerIndex;
	bool								OwnNeytral;

	static void AddRec					(BYTE* data,int zs,DWORD Peer);
    bool IsOwn                          (pUnit& u);
    bool IsNeytral                      (pUnit& unit);
    bool IsOther                        (pUnit& unit);
    bool MakeUnitMine                   (pUnit& unit); 

    void AddUnitToControlList           (pUnit unit,bool MyOwn);
    void AddSelectedToControlList       (DWORD NI);
    void RefreshControlList             ();
	void CreatePlayersMap				();
	void FillOwnersList					(int x,int y,DynArray<int>& Owners);
	
	void CreateOneUnitData              (int ID,int SN,SaveBuf* SB);
	void ProcessUnitsRequest            (SaveBuf* SB);
    void RefreshPeersList               ();
    void HandleCenterChange             (int PlayerIndex,BYTE* buf,int& pos,int BufLen);
    void HandleUnitStateChange          (int PlayerIndex,BYTE* buf,int& pos,int BufLen);
    void HandleCommand                  (int PlayerIndex,BYTE* buf,int& pos,int BufLen);
    void HandleUnitDeath                (int PlayerIndex,BYTE* buf,int& pos,int BufLen);
	void HandleUnitErase                (int PlayerIndex,BYTE* buf,int& pos,int BufLen);
    void HandleUnitsPresenceMask        (int PlayerIndex,BYTE* buf,int& pos,int BufLen);
    void HandleMoney                    (int PlayerIndex,BYTE* buf,int& pos,int BufLen);
	void HandleReOwn                    (int PlayerIndex,BYTE* buf,int& pos,int BufLen);
	void HandleChat						(int PlayerIndex,BYTE* buf,int& pos,int BufLen);
	void HandleBuildingsInfo			(int PlayerIndex,BYTE* buf,int& pos,int BufLen);

	void ChangeMyHeroType				(int HeroTypeIndex);//index in heroes list
	void GatherMyBuildingsInfo			(BinStream* BS);	
    void CreateVisiblePlayersList       ();//fills VisiblePlayers,WellVisiblePlayers
    void CreateGroupsEnumerator			();
    void ProcessReceive                 ();
    void GatherCommands                 ();
    void RefreshUnitsState              ();    
    void ProcessCenterPosition          ();
    void SendMyCenter                   (int PlayerIndex);
    void SendAll                        ();

	void SelectRandomHero				(int Color,int HeroType);
	bool JoinServer(const char* servername,const char* PlayerName,int Color=-1,int HeroType=-1);
	bool CreateServer(const char* PlayerName,const char* MapName=NULL,int Color=-1,int HeroType=-1);
    void Process();

    bool OnCheatEntering                (const char* Cheat);
    void OnUnloading                    ();
	void OnDrawOnMapOverAll				();
	bool OnUnitDie						(OneObject* Dead,OneObject* Killer);
};

int  GetFreeIndexForUnit	();
void SetActiveProducer		(OneObject* OB);
void SetActiveProducer		();
void SetMyActiveSubset		();
void SetDefaultActiveSubset	();
bool IsMassiveGame			();
extern MassiveRoom* MRoom;