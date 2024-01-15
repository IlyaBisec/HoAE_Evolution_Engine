#ifndef __CBigMapQuest__
#define __CBigMapQuest__

#include "../AI_Scripts.h"						
#include "../MISSIONSCRIPT\MissionScript.h"		
#include "../MISSIONSCRIPT\EndMissionData.h"	
#include "../ClasDeclaration.h"					

//----------------------------------------------------------------------------------
// ��������� �� �������
void	VityaError(char* Mess);
//----------------------------------------------------------------------------------
// ����.��������� � ����������
int		ShowBigMapGlobalMessage( int type, int pictId, int headId, char* text, int pageN );//grey [05.03.2004]
//----------------------------------------------------------------------------------

extern	class	CPlayer;
extern	class	CSectData;
extern	class	CDiplMenu;
extern	class	CDiplomacyData;
extern	class	CROOTDRIVER;
//----------------------------------------------------------------------------------

// ---------------------------- class CQuest_STRUCT (structure) ---------------------------- 
// ---
class CQuest_STRUCT	: public BaseClass		
{
	public:
		CQuest_STRUCT()		{ Init(); }
		~CQuest_STRUCT()	{ Init(); }
		void Init()				{ main = 0 ; id = prob = turn_limit = gold = wait = -1 ;
							  for(int i=0; i < MAX_PLAYERS ;i++) { strcpy(name,"null"); owner[i] = taker[i] = -1 ; }   }
	public:
		int		id;										// ���������� ����� 
		byte	main;									// ����: 1 - ������ �������� �����, 0 - ����� ���������

		char	name[20];								// ���
		int		owner[MAX_PLAYERS];						// �����-��������
		int		taker[MAX_PLAYERS];						// �����-����������
		byte	prob;									// ����������� ��������� (probability)
		byte	turn_limit;								// ����������� �� �����
		int		gold;									// ���-�� ����� �� ����������

		int		wait;									// ���-�� ����� �������� ( f[turn_limit] )

};
// ---------------------------- class	CBigMapQuest ---------------------------- 
// queste
class CBigMapQuest
{
	public:
		CBigMapQuest();
		CBigMapQuest( int root_turn );
		~CBigMapQuest();
	public:
		int		number;										// ���-�� �������

		ClassArray<CQuest_STRUCT>	Quest;					// ������ �������
	
		// additional data
		int		ports[50];									// ������ ������

		// primary
		int		QuestID;									// id �����a
		bool	QuestGived;									// ����� ��� ������.
		bool	QuestComplete;								// ����� �������.

		// additional
		int		QuestOwner;									// Owner �����a		( "��������"   )
		int		QuestSecId;									// ��������� ������ ( "����������" )
		int		QuestTarNI;									// ��������� �����  ( "����������" )
		int		QuestParam;									// ���.�����.�����-�  

		bool	Next_Turn;									// ���� ����.����

		// ���������� ������
		CROOTDRIVER*		pROOT;
		CSectData*			pSect;
		CDiplMenu*			pDipl;
		CDiplomacyData*		pDiplData;
		CPlayer*			pPlayer[MAX_PLAYERS];	

	public:
		void	Init();
		void	GetQuestsData();

		void	NextTurn();
		void	ReCountTurnLimits( int root_turn );

		int		GetNumber();
		int		GetTrueOwner( int PlayerNI , int id );		// get random OwnerId
		int		GetRandOwner( int PlayerNI , int id );		// get random OwnerId

		bool	ProcessQuests( CROOTDRIVER* pROOT_SC , int PlayerNI , bool next_turn );
		bool	ChooseQuest( int PlayerNI , int id );
		bool	CheckQuest( int PlayerNI , int id );		// �������� ����������� ������ ������
		bool	CheckValidityOfQuest( int PlayerNI );		// �������� ������������� ��������� ������

		bool	AddInfoMessage( char* text, bool NewMess=false );	// add Quest Message

		// availabled QUESTS
		bool	AttackNation	( int PlayerNI , int id );
		bool	CaptureSector	( int PlayerNI , int id );
		bool	DefendSector	( int PlayerNI , int id );
		bool	SellSector		( int PlayerNI , int id );

		bool	AddGold			( int PlayerNI , int id );
		bool	DeleteGold		( int PlayerNI , int id );

		bool	AddRecruits		( int PlayerNI , int id );
		bool	DeleteRecruits	( int PlayerNI , int id );

		bool	Rebellion		( int PlayerNI , int id );
		bool	DeleteDefence	( int PlayerNI , int id );
		bool	DeleteResources	( int PlayerNI , int id );

		bool	AddSquad		( int PlayerNI , int id );

		//new
		bool	FallPrices		( int PlayerNI , int id );
		bool	RisePrices		( int PlayerNI , int id );

		//SAVE( CBigMapQuest ); 
		//	REG_MEMBER( _bool, QuestGived );
		//	REG_MEMBER( _int,  QuestID );
		//	REG_MEMBER( _int,  QuestOwner );
		//	REG_MEMBER( _int,  QuestSecId );
		//	REG_MEMBER( _int,  QuestTarNI );
		//	REG_MEMBER( _int,  QuestParam );
		//ENDSAVE;
};


#endif	//__CBigMapQuest__


