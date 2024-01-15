#include "stdheader.h"
void NLine(GFILE* f);
int mul3(int);
void ErrM(char* s);
int GetWTP1(int x,int y);
extern word TexList[128];
extern word NTextures;
WallCharacter* GetWChar(OneObject* OB);
int GetWCharID(OneObject* OB);
void AddToVisual(short uy,short x,short y,OneObject* OB,word FileID,word Sprite);
WallCluster::WallCluster(){
    
};
WallCluster::~WallCluster(){
    
};
void WallCluster::SetSize(int N){
    
};
static byte GD8[9]={7,6,5,0,0,4,1,2,3};
static char DX8[8]={0,1,1,1,0,-1,-1,-1};
static char DY8[8]={-1,-1,0,1,1,1,0,-1};
byte GetDir8(int dx,int dy){
    return GD8[dx+dx+dx+dy+4];
};
void WallCluster::ConnectToPoint(short x,short y){
	
};
void WallCluster::ConnectToPoint(short x,short y,bool Vis){

};

void WallCluster::AddPoint(short x,short y,bool Vis){

};
void WallCluster::UndoSegment(){
};
void WallCluster::KeepSegment(){
   
};
void WallCluster::SetPreviousSegment(){
   
};
void WallCluster::CreateSprites(){
    
};
//word Size
//data format:
//byte NI;
//byte Type;
//word NCells;
//word NIndex;
//word Health;
//By 1 cell:
//short x;
//short y;
int WallCluster::CreateData(word* Data,word Health){
	return 0;
};
extern bool CmdDone[ULIMIT];
void WallCluster::SendSelectedToWork(byte NI,byte OrdType){	
};
void WallCluster::CreateByData(word* Data){
	
};
//----------------<   CLASS:  WallSystem   >---------------

WallSystem::WallSystem(){
};
WallSystem::~WallSystem(){    
};
void WallSystem::AddCluster(WallCluster* WC){    
};
void WallSystem::Show(){
    for(int i=0;i<NClusters;i++){
        if(WCL[i])WCL[i]->View();
    };
};
//###----------------<   CLASS: WallCell   >----------------###
static char* Clu1="   ** "
                  "  ****"
                  " *****"
                  "***** "
                  " ***  "
                  "  *   ";

static char* Clu3="  *   "
                  " ***  "
                  "***** "
                  " *****"
                  "  ****"
                  "   ** ";

static char* Clu4="      "
                  " **** "
                  " **** "
                  " *****"
                  "  ****"
                  "   ** ";

static char* Clu5="   ** "
                  "  ****"
                  " *****"
                  " **** "
                  " **** "
                  "      ";

static char* Clu6="      "
                  "  *** "
                  " **** "
                  "***** "
                  " ***  "
                  "  *   ";

static char* Clu7="      "
                  " ***  "
                  " **** "
                  " *****"
                  " *****"
                  "   ** ";

static char* Clu8="  *   "
                  " ***  "
                  "***** "
                  " **** "
                  " **** "
                  "      ";

static char* Clu9="      "
                  " **** "
                  " **** "
                  "***** "
                  " ***  "
                  "  *   ";

static char* Clu10="   ** "
                   "  ****"
                   " *****"
                   " **** "
                   " **** "
                   "      ";

static char* Clu11="  *   "
                   " **** "
                   "***** "
                   " **** "
                   "  *** "
                   "      ";

static char* CluF="      "
                  " **** "
                  " **** "
                  " **** "
                  " **** "
                  "      ";
char* GetLockData(byte id){
    switch(id){
    case 0:return CluF;
    case 1:return Clu1;
    case 2:return CluF;
    case 3:return Clu3;
    case 4:return Clu4;
    case 5:return Clu5;
    case 6:return Clu6;
    case 7:return Clu7;
    case 8:return Clu8;
    case 9:return Clu9;
    case 10:return Clu10;
    case 11:return Clu11;
    case 12:return CluF;
    };
    return NULL;
};
bool CheckBuildingsForWalls(int x,int y);
bool WallCell::CheckPosition(){
	return true;
};
int WallCell::GetLockStatus(){	
    return 0;
};
bool GetTCStatus(int x,int y);
void WallCell::SetLockStatus(){
	
};
void WallCell::ClearLocking(){
    
};
int NChar;
WallCharacter WChar[32];
WallCell** WRefs;
void SetTexturedRound(int x,int y,int rx,byte Tex);
bool WallCell::StandOnLand(WallCluster* WC){
    return true;
};

void WallCell::CreateLocking(WallCluster* WC){    
};
void WallCell::Landing(WallCluster* WC){	
};
void WallBuildStage(WallCell* WC){
	
};
void SetCellLife(int x,int y,int Life,bool Base){	
};
const int  Wdx[4]={0,1,1,1};
const int  Wdy[4]={-1,-1,0,1};
void SetLife(WallCell* WC,int Health){	
};
//Usage of walls
extern int NRLFiles;
extern char* RLNames[512];
void LoadWallSprites(char* Name,char* Gate,word Type){
	
};
int GetIconByName(char* Name);
int GetResID(char*);
extern char* SoundID[512];
extern word NSounds;
int SearchStr(char** Res,char* s,int count);
void LoadAllWalls(){
	return;    
};
int GetWallType(char* Name){    
    return -1;
};
extern bool TransMode;
void WallCluster::View(){    
};
void WallCluster::Preview(){    
};
WallCluster TMPCluster;
WallSystem WSys;
byte WallType;

bool BuildWall;
bool FirstWall;
void SetWallBuildMode(byte NI,word NIndex){    
};
void WallHandleDraw(){    
};
static int xpre=100000000;
static int ypre=100000000;
int ConvScrY(int x,int y);
void CmdCreateWall(byte NI);
extern byte SpecCmd;
void WallHandleMouse(){
    
};
int GetTopDistanceW(int xa,int ya,int xb,int yb){
	if(xa<0||ya<0||xb<0||yb<0||
		xa>=TopLx||xb>=TopLx||ya>=TopLx||yb>=TopLx)return 0xFFFF;
	int Top1=GetTopRef(xa+(ya<<TopSH));
	int Top2=GetTopRef(xb+(yb<<TopSH));
	if(Top1==0xFFFE)return Norma(xa-xb,ya-yb);
	if(Top1>=0xFFFE||Top2>=0xFFFE)return 0xFFFF;
	if(Top1==Top2)return Norma(xa-xb,ya-yb);
	int NA=GetNAreas();
	int Next1=GetMotionLinks(Top1*NA+Top2);
	if(Next1==0xFFFF)return 0xFFFF;
	if(Next1==Top2)return Norma(xa-xb,ya-yb);
	int Next2=GetMotionLinks(Top2*NA+Top1);
	if(Next2==0xFFFF)return 0xFFFF;
	if(Next2==Top1)return Norma(xa-xb,ya-yb);
	Area* AR1=GetTopMap(Next1);
	Area* AR2=GetTopMap(Next2);
	if(Next1==Next2)return Norma(xa-AR1->x,ya-AR1->y)+Norma(xb-AR2->x,yb-AR2->y);
	return Norma(xa-AR1->x,ya-AR1->y)+Norma(xb-AR2->x,yb-AR2->y)+GetLinksDist(Next1*NA+Next2);
};
int GetTopDistance(int xa,int ya,int xb,int yb);

void BuildWallLink(OneObject* OB);
bool OneObject::BuildWall(int xx,int yy,byte Prio,byte OrdType,bool TempBlock){	
	return false;
};
void BuildWallLink(OneObject* OBJ){	
};
void DetermineWallClick(int x,int y){
	
};
int GetWTP(int x,int y){
	return -1;
};
int GetWTP1(int x,int y){
	return -1;
};
void SetWTP(int x,int y,int Sprite,int Base){
	
};
bool CheckGateUpgrade(OneObject* OB){
	
	return true;
};
bool CheckOpenGate(OneObject* OB){
	
	return false;
};
bool CheckCloseGate(OneObject* OB){
	
	return false;
};
void CreateGatesLink( OneObject* OBJ);
//extern int RESRC[8][8];
bool CreateGates(OneObject* OB){
	return true;
};
void CreateGatesLink( OneObject* OBJ){
	
};
//Gates handling
int NGates;
extern int HISPEED;
extern word OWNER;
bool CheckVisibility(int x1,int y1,int x2,int y2,word MyID){
	return true;
};

extern int tmtmt;
