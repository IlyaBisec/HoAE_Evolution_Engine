//-------------------------------------------------------------------------------------------------------------
byte	GetQuestsNation(int ai_ni, int player_ni);
byte	GetTrapsSquadLimit(int player_SQ_NUM, int dif);
//-------------------------------------------------------------------------------------------------------------

//------------------class registration---------------------------
void	REG_ALL_CLASS();
void	UNREG_ALL_CLASS();
//-------------------------set-----------------------------------
void	setResource( byte NI, int wood, int food, int stone, int gold, int iron, int coal );
void	setStartPoint( int x, int y );	

//------------------------create---------------------------------
void	CreateHeroArmy( byte NI, ClassArray<CTYPENUM>* pARM, SXYF* pPOS );
void	createDefenderArmy(byte NI,SARMY_DESCRIPTION* pAA,SARMY_DESCRIPTION* pDA,
						   ClassArray<SXYF>* pP,ClassArray<SXYF>* pC,ClassArray<SXYF>* pR,
						   ClassArray<CTYPENUM>* pDAR);
void	createAttackerArmy(byte NI, SARMY_DESCRIPTION* pAA,
						   ClassArray<SXYF>* pP, ClassArray<SXYF>* pC, ClassArray<SXYF>* pR, ClassArray<SXYF>* pArt,
						   ClassArray<CTYPENUM>* pDAR, int Real_NI );
void	createAttackerArmy(CSAVE_MIS_STRUCT* pIMD);
void	createDefenderArmy(CSAVE_MIS_STRUCT* pIMD);

void	createCamp(byte NI,GAMEOBJ* pTYPE,GAMEOBJ* pF,int n,GAMEOBJ* pZONE,GAMEOBJ* pGrp);//Поставить в pZONE, n юнитов типа pTYPE, нации NI.
void	createCamp(byte NI,GAMEOBJ* pZ,int N_SQ);

void	createCamp(CSAVE_MIS_STRUCT* pIMD, _str& dir);

void	getStrategicZones( ClassArray<STRAT_ZONE>* pStr ); //grey [03.03.2004]
void	getStrategicZones( int* pID , int* pSt , int NUM ); //grey [03.03.2004]

int 	getStrategicTowerZones( int* pID , int* pState , int* pWeight ); //grey [04.08.2004]

//----------------------additional-------------------------------
int		MinLV(int a, int b);
int		MaxLV(int a, int b);
void	setDigit(unsigned long int& X, int r, int n); //X-число, r-номер разряда(0-9), n-цифра.
int		getDigit(const unsigned long int X, int r);
int		getNRate(const int value, const int rate);	//rate %-ов от value
//---------------------------------------------------------------
int		GetCannonsAmount(int nation);// grey [14.06.2004]
int		GetCapturedCannons(int nation, int realNI);//new
int		GetSquadsAmount(int nation); // grey [28.07.2004]

char*	GiveMeCoolAttackerSquadType( int nation ); //grey [10.08.2004]
//int		ProcessRandomConds(); //grey [15.12.2004]
bool	CheckAddExperience( int nation, int AddKills=10, int cond=-1, int param=-1 ); //grey [17.12.2004]

//---------------------------------------------------------------
void	CheckResource(int nation, int resource);// GREY [02.03.2004]
void	CheckAllResources(int nation);// GREY [02.03.2004]

