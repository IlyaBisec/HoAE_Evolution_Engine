bool EnterName();
int  processMainMenu();
bool ProcessMenuOptions();
bool RunEnciclopia(SimpleDialog* SD);
void ProcessCampagins(int Options);
int  MM_ProcessMultiPlayer();
int  MM_ProcessSinglePlayer();
bool SingleOptions();
bool MPL_WaitingGame(bool Host,bool SINGLE);
//bool StartIGame(bool);
int  MPL_ChooseConnection();
bool MPL_WaitingGame(bool Host,bool SINGLE);
bool SelectSingleMission();
bool SelectSingleBattle();
bool processMultiplayer();
void processBattleMultiplayer();
void ProcessGLoadGame();
void ProcessGSaveGame();
bool ProcessSavingFile(char* Mask,char* DestName,int Header,bool clear);
bool ProcessLoadingFile(char* Mask,char* DestName,int Header);
void ProcessGLoadGame();
void ProcessGSaveGame();
int ProcessGMainMenu();
bool ProcessGameOptions();
bool MPL_JoinGame(int ID);
void StartMixing();
void ProcessMixing();
int ProcessWars();
bool MPL_WaitingBattleGame(bool Host,int BattleID);
bool MPL_WaitingBattleGame(bool Host,int BattleID);
bool ProcessOneBattle(int BtlID);
int ProcessWars();

int ProcessMultilineQuestion(int Nx,char* Bmp1,byte or1,char* Text1,char* Quest);
void ProcessHelp();
void ProcessMissionText(char* Bmp,char* Text);
int ShowHistryItem(char* Bmp,char* Text,bool Shadow);

bool PostCampaignMessage(int n);

extern int BTLID;	// Start Battle ID
//#define AMERICANDEMO