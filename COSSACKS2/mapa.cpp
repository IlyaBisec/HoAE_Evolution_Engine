#include "stdheader.h"
#include "PluginsInterface.h"
#include "cvi_MainMenu.h"
#include "curve.h"
#include "CurrentMapOptions.h"
#include "Groups.h"
#include "unitability.h"
#include "UnitsInterface.h"
#include "FMM\FMManager.h"
#ifdef _USE3D
#pragma pack (push)
#pragma pack (8)
#include "Scape3D.h"
#pragma pack (pop)
void TestCGARB();
#endif // _USE3D
#include "DrawModels.h"
#include "Globals.h"
#include "ITerrain.h"
#include "IWater.h"
#include "IShadowManager.h"
#include "DrawFeatures.h"
#include "IWater.h"
#include "UnitsOnMinimap.h"
#include "TreesMesh.h"
#include "sgGeometry.h"
#include "sgTexture.h"
#include "Camera\Navigator.h"
#include "UndoMaster.h"
int GetUnitModifiedBySprite(int _MineID,OneSprite* OS);
extern int NationID;

void ACT(int x);

#undef max

bool CheckIfNewTerrain();

CEXPORT bool setlIsReinforce(word ObjID, bool Add);

inline bool IsEnemyUnit( OneObject* OB )
{
	return ((OB->NMask & NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NMask) == 0);
}

int LastActionX=0;
int LastActionY=0;
int LastActionTime=0;

extern int UNI_LINEDLY1;
extern int UNI_LINEDY1;
extern int UNI_LINEDLY2;
extern int UNI_LINEDY2;
extern int UNI_HINTDY1;
extern int UNI_HINTDLY1;
extern int UNI_HINTDY2;
extern int UNI_HINTDLY2;
#define CHECKNSL for(int i=0;i<7;i++)addrand(NSL[i]+1);
bool NoPFOG=0;
int Inform=0;
bool LockPause=0;
bool MiniActive=0;
void ShowShipsDest(Brigade* BR);
void DrawDangerMap();
word GetDir(int,int);
void HandleGeology();
void HandleShar(Nation* NT);
word CurZone;
int MRealX,MRealY;
void TestBlob();
bool NoText=false;
void DrawFog();
void ShowAllBars();
extern bool MEditMode;
extern byte EditMedia;
extern bool BuildWall;
extern bool SHOWSLIDE;
extern bool CINFMOD;
void ShowMines(int);
void ShowRLCItemDarkN(int x,int y,lpRLCTable lprt,int n,int Ints);
void ShowRLCItemRedN(int x,int y,lpRLCTable lprt,int n,int Ints);
int CheckCreationAbility(byte NI,int Type,int* x2i,int* y2i,word* BLD,int NBLD,bool Manual);
void CmdGoToTransport(byte NI,word ID);
bool GetRealCoords(int xs,int ys,int* xr,int* yr);
extern bool KeyPressed;
extern bool Mode3D;
extern bool FullMini;
void TestTriangle();
extern int time1,time2,time3,time4,time5;
int time6,time7,time8;
extern int PortBuiX,PortBuiY;
extern int HintX;
extern int HintY;
extern bool CheapMode;
extern int NMyUnits;
extern int NThemUnits;
extern RLCFont FPassive;
extern RLCFont FActive;
extern RLCFont FDisable;
int GetRLen(char* s,RLCFont* font);
word GetEnemy(int x,int y,byte NI);
int COUNTER;
extern int BlobMode;
extern int LASTRAND,LASTIND;
void ShowRMap();
extern int FogMode;
extern int RealLx;
extern int RealLy;
extern int Pitch;
extern Forces NForces[8];
extern bool GetCoord;
extern UniqMethood* UNIM;
void LShow();
//��������������
extern int PathAsks;
extern int NMONS;
extern bool AttackMode;
extern int nEused;
extern bool EUsage[1024];
extern Nation* curn;
extern int counter;
extern bool InfoMode;
extern int WaitCycle;
extern int tmtmt;
extern bool EditMapMode;
extern int HeightEditMode;
extern bool HelpMode;
extern bool ChoosePosition;
extern int LastKey;
extern bool PeaceMode;
extern byte LockMode;
const byte CLRT[8]={0xD0,0xD4,0xD8,0xDC,0xE0,0xE4,0xE8,0xAD};
void ShowDestn();
void ShowMiniDestn();
extern byte RNTB[64][256];
void CreateMiniMap();
word Nsel;
int TerrHi;

bool BuildMode;
byte SpecCmd;
OneSlide* OSB;
byte blx;
byte bly;
word BuildingID;
Nation* BNat;
extern int NAsk;
extern int curptr;
int	smapx;
int	smapy;
int	smaplx;
int	smaply;
int	mapx;
int	mapy;
int	msx;
int msy;
int minix;
int	miniy;
static int StartX=0;
static int StartY=0;
int LStartY=0;
bool MiniMade;
char Prompt[80];
int PromptTime;
//byte minimap[maxmap][maxmap];
byte Locking[1024];//�� ������� ������
#define ScreenSizeX32 ScreenSizeX-32
//��� ������ � ���
OneObject* Group[ULIMIT];
OneObject** GroupU=&Group[0];
//M���� ��������
//network sequence errors enumeration
extern int SeqErrorsCount;
extern int LastRandDif;
extern int curdx;
extern int curdy;

extern short WheelDelta;

//---------------------------------------------G R I D S------------------------------------//
int GridX=4;
int GridY=4;
int GridLx=16;
int GridLy=24;
int GridNx=230/16;
int GridNy=230/32;
int GridMode=0;
void SelectNextGridMode(){
	switch(GridMode){
	case 0:
		GridMode=1;
		//GridLx=8;
		//GridLy=12;
		GridLx=8;
		GridLy=12;
		break;
	case 1:
		GridMode=2;
		//GridLx=12;
		//GridLy=18;
		GridLx=16;
		GridLy=24;
		break;
	case 2:
		GridMode=3;
		//GridLx=16;
		//GridLy=24;
		GridLx=24;
		GridLy=36;
		break;
	case 3:
		GridMode=4;
		//GridLx=20;
		//GridLy=30;
		GridLx=32;
		GridLy=48;
		break;
	case 4:
		GridMode=5;
		//GridLx=24;
		//GridLy=36;
		GridLx=40;
		GridLy=60;
		break;
	case 5:
		GridMode=0;
		break;
	};
};
void DeleteInArea(int x0,int y0,int x1,int y1);
void DelInGrid(int x,int y){
	if(!GridMode)return;
	int Gx=((x-GridX*64)>>6)/GridLx;
	int Gy=((y-GridY*32)>>5)/GridLy;
	if(Gx>=0&&Gx<GridNx&&Gy>=0&&Gy<GridNy){
		DeleteInArea((Gx*GridLx+GridX)*64-1,(Gy*GridLy+GridY)*32-1,((Gx+1)*GridLx+GridX)*64+1,((Gy+1)*GridLy+GridY)*32+1);
	};
};
void DrawGrids(){
	int scshf=0;
	if(LMode)scshf=2;
	int x0=(mapx-GridX*2)/(2*GridLx);
	int Nx=2+smaplx/(2*GridLx);
	int yMin,yMax,xMin,xMax;
	GridNx=(msx-GridX*4)/(GridLx*2);
	GridNy=(msx-GridY*4)/(GridLy*2);

	yMin=(GridY<<1)-mapy;
	if(yMin<0)yMin=0;
	yMax=((GridY+GridLy*GridNy)<<1)-mapy;
	if(yMax>smaply)yMax=smaply;
	yMin=smapy+(yMin<<4);
	yMax=smapy+(yMax<<4);

	xMin=(GridX<<1)-mapx;
	if(xMin<0)xMin=0;
	xMax=((GridX+GridLx*GridNx)<<1)-mapx;
	if(xMax>smaplx)xMax=smaplx;
	xMin=smapx+(xMin<<5);
	xMax=smapx+(xMax<<5);


	for(int i=0;i<Nx+1;i++){
		int x=(GridX+(i+x0)*GridLx)<<1;
		if(x>=mapx&&x<=mapx+smaplx){
			x=smapx+((x-mapx)<<5);
			DrawLine(x>>scshf,yMin>>scshf,x>>scshf,yMax>>scshf,0xFF009A00);
		};
	};
	int y0=(mapy-GridY*2)/(2*GridLy);
	int Ny=2+smaply/(2*GridLy);
	for(i=0;i<Ny+1;i++){
		int y=(GridY+(i+y0)*GridLy)<<1;
		if(y>=mapy&&y<=mapy+smaply){
			y=smapy+((y-mapy)<<4);
			DrawLine(xMin>>scshf,y>>scshf,xMax>>scshf,y>>scshf,0xFF009A00);
		};
	};
	if(GetKeyState(VK_CONTROL)&0x8000){
		int Dx=GridLx*32;
		for(int i=0;i<Nx+2;i++){
			int x1=(GridX+(i+x0)*GridLx)<<1;
			int x=smapx+((x1-mapx)<<5);
			if(x1>=mapx&&x1<=mapx+smaplx){
				DrawLine((x+1)>>scshf,yMin>>scshf,(x+1)>>scshf,yMax>>scshf,0xFF009A00);
				DrawLine((x-1)>>scshf,yMin>>scshf,(x-1)>>scshf,yMax>>scshf,0xFF009A00);
			};
			if(x0+i)DrawLine((x-Dx)>>scshf,yMin>>scshf,(x-Dx)>>scshf,yMax>>scshf,0xFF009A00);
			if(x0+i)DrawLine((x+Dx)>>scshf,yMin>>scshf,(x+Dx)>>scshf,yMax>>scshf,0xFF009A00);
		};
		int y0=(mapy-GridY*2)/(2*GridLy);
		int Ny=2+smaply/(2*GridLy);
		int Dy=GridLy*16;
		for(i=0;i<Ny+2;i++){
			int y1=(GridY+(i+y0)*GridLy)<<1;
			int y=smapy+((y1-mapy)<<4);
			if(y1>=mapy&&y1<=mapy+smaply){
				DrawLine(xMin>>scshf,(y-1)>>scshf,xMax>>scshf,(y-1)>>scshf,0xFF009A00);
				DrawLine(xMin>>scshf,(y+1)>>scshf,xMax>>scshf,(y+1)>>scshf,0xFF009A00);
			};
			if(y0+i)DrawLine(xMin>>scshf,(y-Dy)>>scshf,xMax>>scshf,(y-Dy)>>scshf,0xFF009A00);
			if(y0+i)DrawLine(xMin>>scshf,(y+Dy)>>scshf,xMax>>scshf,(y+Dy)>>scshf,0xFF009A00);
		};
		x0--;
		y0--;
		if(x0<0)x0=0;
		if(y0<0)y0=0;
		int NX=(smaplx>>3)+2;
		int NY=(smaply>>3)+2;
		int X0=(-(mapx&7))<<5;
		int Y0=(-(mapy&7))<<4;
		for(int IX=0;IX<NX;IX++)
			for(int IY=0;IY<NY;IY++){
				int xx=X0+IX*256;
				int yy=Y0+IY*128;
				DrawLine((xx>>scshf)-4,(yy>>scshf)-4,(xx>>scshf)+4,(yy>>scshf)+4,0xFF009A00);
				DrawLine((xx>>scshf)+4,(yy>>scshf)-4,(xx>>scshf)-4,(yy>>scshf)+4,0xFF009A00);
				xx+=128;
				yy+=64;
				DrawLine((xx>>scshf)-4,(yy>>scshf)-4,(xx>>scshf)+4,(yy>>scshf)+4,0x8F009A00);
				DrawLine((xx>>scshf)+4,(yy>>scshf)-4,(xx>>scshf)-4,(yy>>scshf)+4,0x8F009A00);
			};


		for(int ix=0;ix<Nx+2;ix++)
			for(int iy=0;iy<Ny+2;iy++){
				int x=(ix+ix+x0+x0+1)*GridLx+GridX+GridX;
				int y=(iy+iy+y0+y0+1)*GridLy+GridY+GridY;
				if(x>=mapx&&x<=mapx+smaplx&&y>=mapy&&y<=mapy+smaply&&x0+ix<GridNx&&y0+iy<GridNy){
					int N=ix+x0+(iy+y0)*GridNx;
					char cc[5];
					if(N<10)sprintf(cc,"00%d",N);
					else if(N<100)sprintf(cc,"0%d",N);
					else sprintf(cc,"%d",N);
					ShowString((smapx+(x-mapx)*32)>>scshf,(smapy+(y-mapy)*16)>>scshf,cc,&WhiteFont);
				};
			};
	};
};
//------------------------------------------------------------------------------------------//
void ShowNewMonsters();
//��
int ConvY(int y){
	return div24(y<<5);
}
void RedSquare(int x,int y){
	/*
	if(x<mapx||x>=mapx+smaplx||y<mapy||y>=mapy+smaply)return;
	int sco=int(ScreenPtr)+smapx+((x-mapx)<<5)+
		(((y-mapy)<<5)+smapy)*SCRSizeX;
	int ddx=SCRSizeX+SCRSizeX-32;
	__asm{
		push	edi
		mov		edi,sco
		mov		eax,16
uyu:	mov		ecx,16
uuu:	mov		byte ptr [edi],clrRed
		add		edi,2
		loop	uuu
		add		edi,ddx
		dec		al
		jnz		uyu

		pop		edi
	};
	*/
};
void RedMiniSquare(int x,int y){
	/*
	if(x<mapx||x>=mapx+smaplx||y<mapy||y>=mapy+smaply)return;
	int sco=int(ScreenPtr)+smapx+((x-mapx)<<4)+
		(((y-mapy)<<4)+smapy)*SCRSizeX;
	int ddx=SCRSizeX+SCRSizeX-16;
	__asm{
		push	edi
		mov		edi,sco
		mov		eax,8
uyu:	mov		ecx,8
uuu:	mov		byte ptr [edi],clrRed
		add		edi,2
		loop	uuu
		add		edi,ddx
		dec		al
		jnz		uyu

		pop		edi
	};
	*/
};
void RedBar(int x,int y,int lx,int ly){
	for(int i=0;i<lx;i++)
		for(int j=0;j<ly;j++)
			RedSquare(x+i,y+j);
};
void RedMiniBar(int x,int y,int lx,int ly){
	for(int i=0;i<lx;i++)
		for(int j=0;j<ly;j++)
			RedMiniSquare(x+i,y+j);
};
void WhiteSquare(int x,int y){
	if(x<mapx||x>=mapx+smaplx||y<mapy||y>=mapy+smaply)return;
	int sco=int(ScreenPtr)+smapx+((x-mapx)<<5)+
		(((y-mapy)<<5)+smapy)*SCRSizeX;
	int ddx=SCRSizeX+SCRSizeX-32;
	__asm{
		push	edi
		mov		edi,sco
		mov		eax,16
uyu:	mov		ecx,16
uuu:	mov		byte ptr [edi],255
		add		edi,2
		loop	uuu
		add		edi,ddx
		dec		al
		jnz		uyu

		pop		edi
	};
};
void WhiteMiniSquare(int x,int y){
	if(x<mapx||x>=mapx+smaplx||y<mapy||y>=mapy+smaply)return;
	int sco=int(ScreenPtr)+smapx+((x-mapx)<<4)+
		(((y-mapy)<<4)+smapy)*SCRSizeX;
	int ddx=SCRSizeX+SCRSizeX-16;
	__asm{
		push	edi
		mov		edi,sco
		mov		eax,8
uyu:	mov		ecx,8
uuu:	mov		byte ptr [edi],255
		add		edi,2
		loop	uuu
		add		edi,ddx
		dec		al
		jnz		uyu

		pop		edi
	};
};
void WhiteBar(int x,int y,int lx,int ly){
	for(int i=0;i<lx;i++)
		for(int j=0;j<ly;j++)
			WhiteSquare(x+i,y+j);
};
void WhiteMiniBar(int x,int y,int lx,int ly){
	for(int i=0;i<lx;i++)
		for(int j=0;j<ly;j++)
			WhiteMiniSquare(x+i,y+j);
};
extern int CurPalette;
void OutErr(LPCSTR s)
{
	char pal[128];
	sprintf(pal,"%d\\agew_1.pal",CurPalette);
	LoadPalette(pal);
	MessageBox(hwnd,s,"Loading failed...",MB_ICONWARNING|MB_OK);
};
bool ManualFogCheck(int xx,int yy,int dx){
	/*
	if(EditMapMode)return true;
	int x,y;
	for(int i=0;i<dx;i++)
		for(int j=0;j<dx;j++){
			x=xx+i;
			y=yy+j;
			if(x<=1||x>=msx||y<=1||y>=msy)return false;
			if(fmap[y][x]<1000)return false;
		};
		*/
	return true;
};
void AssignHint1(char* s,int time);
extern OrdersScope OSC;
Order1* GetOrdBlock(){
	int N;
	if(OSC.Orders.GetAmount()>=49999){
		//N=N/0;
        return NULL;
	};	
	if(N=OSC.FreeBlocks.GetAmount()){
		Order1* OR1;
		SAFENEW;
			OR1=OSC.FreeBlocks[N-1];
			OSC.FreeBlocks.Del(N-1,1);
			memset(OR1,0,sizeof *OR1);
			OR1->Used=1;
		ENDNEW;
		return OR1;
	}
	Order1 Or1;
    int p;
	SAFENEW;
		p=OSC.Orders.Add(Or1);
	ENDNEW;
	Order1* OR1=&OSC.Orders[p];
	memset(OR1,0,sizeof Order1);
	OR1->Used=1;
	return OR1;
};
void OneObject::FreeOrdBlock(Order1* p ){
	assert(OSC.Orders.GetAmount()<50000);
	assert(p->Used);
	p->Used=0;
	SAFENEW;
		OSC.FreeBlocks.Add(p);
	ENDNEW;
};
void InitOrdBuf(){	
};

void LoadLock()
{
	ResFile f=RReset("lock.dat");
	if(f==INVALID_HANDLE_VALUE)OutErr("Can't load locking.");
	RBlockRead(f,Locking,RFileSize(f));
	RClose(f);
};
void LoadFon()
{
	
};
void ShowFon()
{
	return;

};
void ShowFon1()
{

};
void ShowBorder()
{
	return;

};
int mul3(int x){
	return x+x;//x+x+x;
};
void LoadTiles()
{
	
};
void SaveWalls(ResFile ff1);
//void LoadWalls(ResFile ff1);
char MapName[128];
extern short HiMap[1][1];//Height of surface
byte mapPos[16];
int RES[8][8];

void CreateUnit(Nation* NT,byte x,byte y,word ID){
	
};
void PostLoadExtendedMap(char* s){
	memset(fmap,0,LX_fmap*LX_fmap*2);
	ResFile f1=RReset(s);
	int pos=12+16+128+8*8*4+(msx*msy)+(msx+1)*(msy+1)*2;
	RSeek(f1,pos);
	//Load units
	int NU;
	RBlockRead(f1,&NU,4);
	pos+=(NU+1)<<2;
	RSeek(f1,pos);
	RBlockRead(f1,&NU,4);
	byte NNUM,xu,yu;
	word NIndex;
	for(int i=0;i<NU;i++){
		RBlockRead(f1,&NIndex,2);
		RBlockRead(f1,&NNUM,1);
		RBlockRead(f1,&xu,1);
		RBlockRead(f1,&yu,1);
		Nation* NT=&NATIONS[NNUM];
		CreateUnit(NT,xu,yu,NIndex);
	};
	//walls
	//LoadWalls(f1);
	RClose(f1);
	memset(fmap,0,LX_fmap*LX_fmap*2);
};
void RestoreLock(int x,int y,int lx,int ly){
	
};
void ClearMaps();

void ClearMaps();
extern int MAXSPR;
void InitRiv();
extern MotionField UnitsField;
void ClearPixUndoStack();
void ClearAllLModeData();
void ResetGroundCache();
void ShowLoadProgress(char* Mark, int v, int vMax);
#include "Surface\Surface.h"
void ClearTerramorph();
void NewMap(int szX,int szY)
{
	UndoMaster::RemoveAllFromDisk();
	extern bool IgnoreTreesProcessing;
	IgnoreTreesProcessing=false;
	void clear_dead_units();
	clear_dead_units();
	ResetGroundCache();
	ClearTerramorph();

	int L0=20;
	int L1=70;
	//
	ShowLoadProgress("PrepareGameMedia",50,1000);
	//
	ClearAllLModeData();
	//
	ShowLoadProgress("PrepareGameMedia",55,1000);
	//
	ClearPixUndoStack();
	//
	ShowLoadProgress("PrepareGameMedia",60,1000);
	//
	PromptTime=0;
	MAXOBJECT=0;
	MAXSPR=0;
	memset(Group,0,sizeof Group);
	memset(NLocks,0,sizeof NLocks);
	ClearMaps();
	BuildMode=false;
	memset(&HiMap[0][0],0,sizeof HiMap);
	for(int i=0;i<NMFIELDS;i++){
		MFIELDS[i].ClearMaps();
	}
	//MFIELDS[1].ClearMaps();
	UnitsField.ClearMaps();
	//
	ShowLoadProgress("PrepareGameMedia",65,1000);
	//
	MiniMade=false;
	Nsel=0;	
	InitRenderMap();
	//
	ShowLoadProgress("PrepareGameMedia",70,1000);
	//
	msx=szX;
	msy=szY;
	InitRiv();
	void SetInitialScrollLimitations();
	//
	ShowLoadProgress("PrepareGameMedia",75,1000);
	//
	SetInitialScrollLimitations();
	void UnLoadFS();
	UnLoadFS();
	if(CheckIfNewTerrain()){
		g_SurfEditor.Load();
		g_UniversalMap.Create();
	}
    extern char CurrentBackgroundMap[128];
    CurrentBackgroundMap[0]=0;
	// Copying fog args from EngineSettings to MapOptions:
	MOptions.FogArgs.SetFromEngineSettings();
	// Copying cam args from EngineSettings to MapOptions:
	MOptions.CamArgs.SetFromEngineSettings();
	// Reset Map Options:
	MOptions.reset_class(&MOptions);
};
void CreateMiniMap();
const drx[8]={0,1,1,1,0,-1,-1,-1};
const dry[8]={-1,-1,0,1,1,1,0,-1};
extern bool HealthMode;

void GetRect(OneObject* OB,int* x,int* y,int* Lx,int* Ly)
{
	if (!OB->newMons) return;
	NewMonster* NM = OB->newMons;
	*x = (OB->RealX>>4) - NM->RectLx/2 + NM->RectDx;
	*y = (OB->RealY>>5) - NM->RectLy   + NM->RectDy;
	*Lx = NM->RectLx;
	*Ly = NM->RectLy;
	if (Mode3D) *x -= OB->RZ;
} // GetRect

Rct GetRect( OneObject* OB )
{
	if (!OB->newMons) return Rct::null;
	NewMonster* NM = OB->newMons;
	int X=OB->GetAttX()/16;
	int Y=OB->GetAttY()/16;
	Vector3D wPos( X, Y, OB->RZ + OB->OverEarth );
	WorldToScreenSpace( wPos );
	wPos.x += -NM->RectLx/2 + NM->RectDx;
	wPos.y += -NM->RectLy   + NM->RectDy;
	return Rct( wPos.x, wPos.y, NM->RectLx, NM->RectLy );
} // GetRect

byte GetPaletteColor(int r,int g,int b);

void DrawAurasMarker(OneObject* OB){
	if(!GSets.ShowAurasMarker) return;

	//static int SB_img = GPS.PreLoadGPImage("selbar");
	int SB_img = GSets.SelBarGP; //GPS.PreLoadGPImage("selbar");
	
	float sc=1.0f/float(1<<(5-Shifter));

	GPS.SetScale(sc);
	
	DWORD ld=GPS.GetCurrentDiffuse();

	GPS.SetCurrentDiffuse(0x8800FF00);
	GPS.ShowGP(((OB->RealX>>4)-(mapx<<5)-16)*sc,((OB->RealY>>5)-(mapy<<4)-OB->RZ-16)*sc,SB_img,0,0);
	GPS.SetCurrentDiffuse(ld);
	GPS.SetScale(1.0f);

	return;
} // DrawAurasMarker

extern float fMapX;
extern float fMapY;

//bool CheckObjectVisibilityOnScreen(OneObject* OB,float Scale);
bool CheckObjectVisibility(int x,int y,int z,int R);
//bool GetObjectVisibilityInFog(OneObject* OB);

void DrawMarker(OneObject* OB){
	if( /*OB->NewBuilding || OB->LockType==1 ||*/ OB->UnlimitedMotion) return;
	if(!(OB->NMask & NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NMask) ){
		DWORD D=GPS.GetCurrentDiffuse();
		GPS.SetCurrentDiffuse((D & 0xFF000000) | 0x00FF0000);
	}
	NewMonster* NM=OB->newMons;
	if(OB->newMons->selIndex>=0){
		bool DrawSelPatchDir(float x,float y,byte Dir,int Type,float ScaleX,float ScaleY,DWORD Color);
		int xc=OB->GetAttX()/16+NM->selShift*TCos[OB->RealDir]/256;
		int yc=OB->GetAttY()/16+NM->selShift*TSin[OB->RealDir]/256;
		if(DrawSelPatchDir(xc,yc,OB->RealDir,NM->selIndex,NM->selScaleX,NM->selScaleY,0xFFFFFFFF)){
			GPS.SetCurrentDiffuse(0xFFFFFFFF);
			return;
		}
	}
	extern float g_dbgZ;
	//static int SB_img = GPS.PreLoadGPImage("selbar");
	int SB_img = GSets.SelBarGP; //GPS.PreLoadGPImage("selbar");
	
	static Vector3D s_Pivot( GPS.GetGPWidth(SB_img,1)*0.5f, GPS.GetGPHeight(SB_img,1)*0.5f, 0.0f );
	
	Vector3D GetSelectionBarPos(OneObject* OB);
	Vector3D VP=GetSelectionBarPos(OB);
	float pX = float(VP.x)+EngSettings.MarkerDx;
	float pY = float(VP.y)+EngSettings.MarkerDy;
	Vector3D pos = SkewPt( pX, pY, Mode3D?VP.z:0 );
	Matrix4D tm = GetAlignTerraTransform( pos, s_Pivot );
	tm.translate( pos );
	GPS.DrawWSprite( SB_img, 1, tm );
	GPS.SetCurrentDiffuse(0xFFFF0000);
	return;
} // DrawMarker

void DrawImMarker(OneObject* OB)
{
	if(!OB) return;
	int x,y,Lx,Ly;
	GetRect(OB,&x,&y,&Lx,&Ly);
	x--;
	y--;
	Lx+=2;
	Ly+=2;
	if(LMode)
	{
		x-=mapx<<Shifter;
		y-=mapy<<(Shifter+1);
		Lx>>=2;
		Ly>>=2;
	}else{
		x+=smapx-(int(mapx)<<5);
		y+=smapy-(mul3(int(mapy))<<3);
	};
	byte cl=0xCD;
	//if(OB->ForceX||OB->ForceY)cl=0xCD;
	byte c1=0;
	byte c2=2;
	byte dc=4;
	int ddy=-12;
	Hline(x+c1,y,x+c2,cl);
	Hline(x+Lx-1-c2,y,x+Lx-1-c1,cl);
	Hline(x+c1,y+Ly-1,x+c2,cl);
	Hline(x+Lx-1-c2,y+Ly-1,x+Lx-1-c1,cl);
	Vline(x,y+c1,y+c2,cl);
	Vline(x+Lx-1,y+c1,y+c2,cl);
	Vline(x,y+Ly-1-c2,y+Ly-1-c1,cl);
	Vline(x+Lx-1,y+Ly-1-c2,y+Ly-1-c1,cl);
}; // DrawImMarker

void CBar(int x,int y,int Lx,int Ly,byte c);
CEXPORT DWORD GetNatColor( int natIdx );
CEXPORT char* GetPlayerName(byte NI);
extern word NPlayers;
void DrawHeroName(OneObject* OB){
	if(OB && OB->ActiveAbility && NPlayers>1){
		HeroVariableStorage* HS=DetectHero(OB);
		if(HS){
			int R,H,UH;
			OB->GetCylinder(R,H,UH);
            char cc[128]="";

			for(int i=0;i<NPlayers;i++){				
                sprintf(cc,"%s [%d]",GetPlayerName(OB->NNUM),HS->Level);
			}			
			int Lx=100000;
			RLCFont* Font=NATIONS[MyNation].NMask & OB->NMask ? &SmallYellowFont:&SmallRedFont;
			int Ly=DrawMultilineText(-1000,0,cc,Font,Lx,10000,0,false);
			Vector3D V(OB->RealX>>4,OB->RealY>>4,H+UH+30);			
			WorldToScreenSpace(V);
			int Vx=V.x;
			int Vy=V.y;
			int Lx2=100000;			
			rsRect(Rct(Vx-Lx/2-4,Vy-Ly-4-10,Lx+8,Ly+8),0,0x40000000);
			rsFlush();
			rsFrame(Rct(Vx-Lx/2-4,Vy-Ly-4-10,Lx+8,Ly+8),0,GetNatColor(OB->NNUM));
			rsFlush();
			DrawMultilineText(Vx-Lx/2,Vy-Ly-10,cc,Font,Lx2,100000,0,true);
			ISM->Flush();
		}
	}
}
extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
void DrawHealth(OneObject* OB)
{
	if(!OB) return;
	if(!GSets.CGame.ViewMask)return;
	if(!OB->WasDrawn||OB->IsInFog) return;	
	if( LMode || OB->Sdoxlo ) return;
	if( !OB->newMons || OB->newMons->Immortal ) return;
	if( !v_ISys.AltDrawHealth ) return;

	NewMonster* NM = OB->newMons;
	if(NM->DontShowALT_Info)return;

	if(OB->ActiveAbility){
		bool inv=false;
		byte detected=0;
		bool enable_search=false;
		OB->ActiveAbility->ActiveAbilities.modifyInvisibility(inv,inv,detected,enable_search); // не рисовать полоску жизни для невидимок
		if( inv && !((detected&NATIONS[MyNation].NMask) || enable_search) )
			return;
	}

	DrawHeroName(OB);

	int sprW = NM->RectLx;
	int sprH = NM->RectLy;

	int width = sprW - 12;
	int health = width;
	if(OB->MaxLife > 0) health = width * OB->Life / OB->MaxLife;

	const DWORD c_EnergyColor = 0xFF0000FF;
	DWORD clrHealth = 0xFF22FF33;
	if (IsEnemyUnit( OB )) clrHealth = 0xFFFF1122;

	float cx = OB->RealX / 16;
	float cy = OB->RealY / 16;

	//  main border 
	Vector3D GetSelectionBarPos(OneObject* OB);
	Vector3D VP=GetSelectionBarPos(OB);
	Vector3D lt( VP ); //GetSelectionBarPos(OB);
	lt.z+=sprH+OB->OverEarth;
	WorldToScreenSpace( lt );
	lt.x = roundf( lt.x - sprW/2 + 5);
	lt.y = roundf( lt.y + 5 );
	Vector3D lb( lt ); 
	
	//lb.y += 2;

	//if(OB->newMons->ShowDelay)
	//{
	//	int mxd = OB->MaxDelay;
	//	if (mxd <= 0) mxd=1;

	//	int energy = (width * (mxd - OB->delay))/mxd;

	//	//  energy thermometer
	//	GPS.DrawFillRect( lt.x, lt.y + 2, energy, 2, c_EnergyColor );
	//}

	if( OB->NewBuilding ){
		
		GPS.DrawFillRect( lt.x, lt.y-1, health, 4, clrHealth );
		
		// side-frame
		GPS.DrawLine( lt.x-1, lt.y-1, lt.x-1, lb.y+1, 0xFFFFFFFF );
		GPS.DrawLine( lt.x + width, lt.y-1, lb.x + width, lb.y+1, 0xFFFFFFFF );

		int x0=lt.x;
		int y0=lt.y;
		DWORD GetNatColor( int natIdx );
		GPS.DrawFillRect(x0,y0-12,8,8,GetNatColor(OB->NNUM));
		char cc[128];
		int adp=100;
		bool GetMineProduceBonus(OneObject* Mine,int& Proc);
		GetMineProduceBonus(OB,adp);		
		if(!IsEnemyUnit( OB ) || OB->WatchNInside)
		{
			int nn=(adp*OB->NInside)/100;
			if(nn>OB->NInside){
				sprintf(cc,"%d+%d/%d",OB->NInside,nn-OB->NInside,OB->MoreCharacter->MaxInside+OB->AddInside);
			}else
			if(nn<OB->NInside){
				sprintf(cc,"%d-%d/%d",OB->NInside,OB->NInside-nn,OB->MoreCharacter->MaxInside+OB->AddInside);
			}else{
				sprintf(cc,"%d/%d",OB->NInside,OB->MoreCharacter->MaxInside+OB->AddInside);
			}
		}
		else
		{
			sprintf(cc,"?/%d",OB->MoreCharacter->MaxInside+OB->AddInside);
		}

		DWORD C=GPS.GetCurrentDiffuse();
		GPS.SetCurrentDiffuse(SmallWhiteFont.GetColor());

		int H=GetRLCHeight(SmallWhiteFont.RLC,'C');
		ShowString(x0+12,y0-14+8-H,cc,&SmallWhiteFont);

		GPS.SetCurrentDiffuse(C);

	}else{
		
		GPS.DrawFillRect( lt.x, lt.y, health, 2, clrHealth );

		// side-frame
		GPS.DrawLine( lt.x-1, lt.y, lt.x-1, lb.y, 0xFFFFFFFF );
		GPS.DrawLine( lt.x + width, lt.y, lb.x + width, lb.y, 0xFFFFFFFF );

	}
}; // DrawHealth

CEXPORT void DrawHealthEx(OneObject* OB){
	DrawHealth(OB);
}

void DrawColorMarker(OneObject* OB,byte cl)
{
	if (!OB) return;
	Rct rct = GetRect(OB);
	//if (OB->ForceX || OB->ForceY) cl = clrRed;

	float b = 5;
	Hline( rct.x,					rct.y,					rct.x + b,			cl );
	Hline( rct.x + rct.w - 1 - b,	rct.y,					rct.x + rct.w - 1,	cl );
	Hline( rct.x,					rct.y + rct.h - 1,		rct.x + b,			cl );
	Hline( rct.x + rct.w - 1 - b,	rct.y + rct.h - 1,		rct.x + rct.w - 1,	cl );
	
	Vline( rct.x,					rct.y,					rct.y + b,			cl );
	Vline( rct.x + rct.w - 1,		rct.y,					rct.y + b,			cl );
	Vline( rct.x,					rct.y + rct.h - 1 - b,  rct.y + rct.h - 1,	cl );
	Vline( rct.x + rct.w - 1,		rct.y + rct.h - 1 - b,  rct.y + rct.h - 1,	cl );
}; // DrawColorMarker

void DrawMiniMarker(OneObject* OB)
{
	if(!OB) return;
	int x,y,Lx,Ly;
	GetRect(OB,&x,&y,&Lx,&Ly);
	x=x>>1;
	y=y>>1;
	Lx=Lx>>1;
	Ly=Ly>>1;
	x+=smapx-(int(mapx)<<4);
	y+=smapy-(mul3(int(mapy))<<2);
	byte cl=clrYello;
	byte c1=0;
	byte c2=2;
	byte dc=4;
	int ddy=-12;
	Hline(x+c1,y,x+c2,cl);
	Hline(x+Lx-1-c2,y,x+Lx-1-c1,cl);
	Hline(x+c1,y+Ly-1,x+c2,cl);
	Hline(x+Lx-1-c2,y+Ly-1,x+Lx-1-c1,cl);
	Vline(x,y+c1,y+c2,cl);
	Vline(x+Lx-1,y+c1,y+c2,cl);
	Vline(x,y+Lx-1-c2,y+Ly-1-c1,cl);
	Vline(x+Lx-1,y+Ly-1-c2,y+Ly-1-c1,cl);
	if(HealthMode){
		int mhs=Lx-c2-c2-dc-dc;
		int hs=mhs;
		if(OB->MaxLife>0){
			hs=div(mhs*OB->Life,OB->MaxLife).quot;
		};
		Vline(-2+x+c2+dc,y+ddy,y+ddy+2,255);
		Vline(-2+x+Lx-1-c2,y+ddy,y+ddy+2,255);
		Vline(-2+x+c2+dc+1,y+ddy,y+ddy,255);
		Vline(-2+x+c2+dc+1,y+ddy+2,y+ddy+2,255);
		Vline(-2+x+Lx-1-c2-1,y+ddy,y+ddy,255);
		Vline(-2+x+Lx-1-c2-1,y+ddy+2,y+ddy+2,255);
		Hline(-2+x+c2+dc+1,y+ddy+1,x+c2+hs+dc+1,clrGreen);
	};
}; // DrawMiniMarker


void ShowHealth(OneObject* ZZ){
	if(!(ZZ->Selected&GM(MyNation)))return;
	DrawMarker(ZZ);
	return; 
};
void ShowMiniHealth(OneObject* ZZ){
	if(!(ZZ->Selected&GM(MyNation)))return;
	DrawMiniMarker(ZZ);
	return; 
};
extern void MakeFog(int x,int y);

void ShowFog(OneObject* ZZ){};
void ShowProperty(OneObject* OB){};

static int tmt=0;
char* GetTextByID(char* ID);
void CBar(int x,int y,int Lx,int Ly,byte c);

void GetDiscreta(int* x,int* y,int val);
char* WINNER=NULL;
char* LOOSER=NULL;
int PrevMissTime=0;
int FirstMissTime=0;
extern bool GameExit;
void GetSquare();
char* GetPName(int i);
int NatSquare[8]={0,0,0,0,0,0,0,0};
int SumSquare[8];
int SumAccount[8];
extern City CITY[8];
extern word NPlayers;
void CreateSumm(){
	memset(SumSquare,0,7*4);
	memset(SumAccount,0,7*4);
	for(int i=0;i<7;i++){
		byte ms0=1<<i;
		byte ms=NATIONS[i].NMask;
		if(ms==ms0){
			SumSquare[i]=NatSquare[i];
			SumAccount[i]=CITY[i].Account;
		}else{
			int ss=0;
			int sa=0;
			int na=0;
			for(int j=0;j<7;j++){
				if(NATIONS[j].NMask&ms){
					ss+=NatSquare[j];
					sa+=CITY[j].Account;
					na++;
				};
			};
			SumSquare[i]=ss/na;
			SumAccount[i]=sa/na;

		};
	};
};

int GetBestPlayer(){
	CreateSumm();
	int maxn=-1;
	int maxs=0;
	int maxidv=0;
	for(int i=0;i<7;i++){
		int v=SumAccount[i];//*10000+CITY[i].Account;
		if(v>maxs){
			maxs=v;
			maxn=i;
			maxidv=CITY[i].Account;
		};
		if(v==maxs&&CITY[i].Account>maxidv){
			maxidv=CITY[i].Account;
			maxn=i;
		};
	};
	return maxn;
};
int SortPlayers(byte* Res,int* par){
	CreateSumm();
	int con=GSets.CGame.PL_INFO[0].VictCond;
	static SortClass SC;
	SC.CheckSize(8);
	int np=0;
	byte usemask=0;
	for(int q=0;q<7;q++){
		int xx=GSets.CGame.cgi_NatRefTBL[q];
		if(NATIONS[xx].ThereWasUnit/*NtNUnits[xx]*/){
			byte ms=NATIONS[xx].NMask;
			if(!(usemask&ms)){
				usemask|=ms;
				SC.Parms[np]=-SumAccount[xx];
				SC.Uids[np]=ms;
				np++;
			};
		};
	};
	SC.NUids=np;
	SC.Sort();
	word msks[8];
	SC.Copy(msks);
	int ps=0;
	for(q=0;q<np;q++){
		byte msk=byte(msks[q]);
		int mp=0;
		byte m0=1;
		for(int p=0;p<7;p++){
			int xx=GSets.CGame.cgi_NatRefTBL[p];
			m0=1<<xx;
			if(msk&m0&&NATIONS[xx].ThereWasUnit/*NtNUnits[xx]*/){
				SC.Parms[mp]=-CITY[xx].Account;
				SC.Uids[mp]=p;
				mp++;
			};
			m0<<=1;
		};
		if(mp){
			SC.NUids=mp;
			SC.Sort();
			for(int p=0;p<mp;p++){
				Res[ps]=SC.Uids[p];
				par[ps]=SC.Parms[p];
				ps++;
			};
		};
	};
	return ps;
};
void ShowVictInFormat(char* Res,int idx,byte Type){
	//sprintf(Res,"%.2f",float(CITY[idx].Account)/100);
	Res[0]=0;
	VictoryConditionRoot* VCond=MOptions.VictoryRule.Get();
	if(VCond){
		VCond->GetPlayerStatusString(idx,Res);
	}
};
bool NoWinner=0;
bool ShowStat=0;
extern bool NOPAUSE;
void CmdEndGame(byte NI,byte state,byte cause);
int GetRealTime();
extern int WaitState;
extern bool EnterChatMode;
extern int ShowGameScreen;
extern byte PlayGameMode;
void ShowCentralText0(char* sss){
	char ssp[400];
	int ny=0;
	int L=strlen(sss);
	for(int i=0;i<L;i++)if(sss[i]=='\\')ny++;
	int MaxLx=0;
	byte c;
	int MaxCur=0;
	int p=0;
	do{
		c=sss[p];
		p++;
		if(c!='\\'&&c){
			int L=1;
			MaxCur+=GetRLCWidthUNICODE(BigYellowFont.RLC,(byte*)(sss+p),&L);
			p+=L-1;
		}else{
			if(MaxCur>MaxLx)MaxLx=MaxCur;
			MaxCur=0;
		};
	}while(c);
	int x0=RealLx/2;
	int y0=RealLy/2;
	int Dy=GetRLCHeight(BigYellowFont.RLC,'y')+5;
	y0-=((Dy*ny)>>1)+32;
	MaxLx>>=1;
	DrawStdBar1(x0-MaxLx-64,y0-8-3,x0+MaxLx+64,y0+20+Dy*ny+7);
	GPS.FlushBatches();
	p=0;
	int p0=0;
	do{
		c=sss[p];
		p++;
		if(c!='\\'&&c){
			ssp[p0]=c;
			ssp[p0+1]=0;
			p0++;
		}else{
			int L=GetRLCStrWidth(ssp,&BigRedFont)>>1;
			ShowString(x0-L,y0-9,ssp,&BigYellowFont);
			y0+=Dy;
			p0=0;
		};
	}while(c);
	GPS.FlushBatches();
};
void ShowWinner(){
	if(NoWinner||PlayGameMode==1)return;
	GetSquare();
	if(!WINNER){
		WINNER=GetTextByID("WINNER");
	};
	if(!LOOSER){
		LOOSER=GetTextByID("LOOSER");
	};
	char sss[512]="";
	int MyVirtNation=GSets.CGame.cgi_NatRefTBL[MyNation];
	
	if(SCENINF.hLib&&!SCENINF.StandartVictory){
		if(SCENINF.LooseGame){
			NOPAUSE=true;
			if(!WaitState)WaitState=1;
			if(SCENINF.LooseText)
				strcpy(sss,SCENINF.LooseText);
			else strcpy(sss,LOOSER);
			if(GetRealTime()-PrevMissTime>9000){
				PrevMissTime=GetRealTime();
				FirstMissTime=GetRealTime();
			}else{
				PrevMissTime=GetRealTime();
				if(PrevMissTime-FirstMissTime>6000||WaitState==2){
					GameExit=true;
					ShowStat=0;
					SCENINF.LooseGame=false;
					PrevMissTime=0;
				};
			};
		}else
		if(SCENINF.Victory){
			NOPAUSE=true;
			if(!WaitState)WaitState=1;
			if(SCENINF.VictoryText)
				strcpy(sss,SCENINF.VictoryText);
			else strcpy(sss,WINNER);
			if(GetRealTime()-PrevMissTime>7000){
				PrevMissTime=GetRealTime();
				FirstMissTime=GetRealTime();
			}else{
				if(GetRealTime()-FirstMissTime>7000||WaitState==2){
					GameExit=true;
					ShowStat=0;
					PrevMissTime=0;
					SCENINF.Victory=false;
				};
				PrevMissTime=GetRealTime();
				if(CurrentCampagin!=-1&&CurrentMission!=-1){
					int idx=CAMPAGINS.SCamp[CurrentCampagin].OpenIndex[CurrentMission];
					int id=(idx>>8)&255;
					if(id<CAMPAGINS.SCamp[CurrentCampagin].NMiss)CAMPAGINS.SCamp[CurrentCampagin].OpenIndex[id]|=1;
					id=(idx>>16)&255;
					if(id<CAMPAGINS.SCamp[CurrentCampagin].NMiss)CAMPAGINS.SCamp[CurrentCampagin].OpenIndex[id]|=1;
					id=(idx>>24)&255;
					if(id<CAMPAGINS.SCamp[CurrentCampagin].NMiss)CAMPAGINS.SCamp[CurrentCampagin].OpenIndex[id]|=1;
					SavePlayerData();
				};
			};
		}else
		if(SCENINF.CTextTime>0&&SCENINF.CentralText){
			strcpy(sss,SCENINF.CentralText);
		};
	}else{
		if(NATIONS[MyNation].VictState&&!sss[0]){
			if(NATIONS[MyNation].VictState==1){
				strcpy(sss,LOOSER);
				LockPause=1;
			}else if(NATIONS[MyNation].VictState==3){
				strcpy(sss,DISCONNECTED);
				LockPause=1;
			}else{
				strcpy(sss,WINNER);
				LockPause=1;
				if(SCENINF.hLib&&CurrentCampagin!=-1&&CurrentMission!=-1){
					int idx=CAMPAGINS.SCamp[CurrentCampagin].OpenIndex[CurrentMission];
					int id=(idx>>8)&255;
					if(id<CAMPAGINS.SCamp[CurrentCampagin].NMiss)CAMPAGINS.SCamp[CurrentCampagin].OpenIndex[id]|=1;
					id=(idx>>16)&255;
					if(id<CAMPAGINS.SCamp[CurrentCampagin].NMiss)CAMPAGINS.SCamp[CurrentCampagin].OpenIndex[id]|=1;
					id=(idx>>24)&255;
					if(id<CAMPAGINS.SCamp[CurrentCampagin].NMiss)CAMPAGINS.SCamp[CurrentCampagin].OpenIndex[id]|=1;
					SavePlayerData();
				};
			};
			if(WaitState==2){
				GameExit=true;
				ShowStat=SCENINF.hLib==NULL;
				PrevMissTime=0;
				SCENINF.Victory=false;
				SCENINF.LooseGame=false;
			};
		}else{
			
		};
	};
	//
	int VictState=NATIONS[MyNation].VictState;
	if(VictState>0&&ext_OnEndGameMessage(MyNation,VictState))return;
	//
	if(SCENINF.CTextTime&&SCENINF.CentralText&&SCENINF.CentralText[0]=='#'){
		strcpy(sss,SCENINF.CentralText+1);
		if(SCENINF.CTextTime)SCENINF.CTextTime--;
	};
	if((!sss[0])&&SCENINF.CTextTime&&SCENINF.CentralText){
		strcpy(sss,SCENINF.CentralText);
	};
	if(ShowGameScreen==2)strcpy(sss,SER_SAVE);
	if(sss[0]&&!EnterChatMode){
		//ShowCentralText0(sss);		
	};
};
//final procedures for graphics output
void ProcessWaveFrames();
void ShowVisualLess(int yend);
extern word rpos;
int time0;
word PreRpos;
void ShowPulse();
void AddPulse(word ObjID,byte c);
void ProcessFog1();
void CreateFogImage();
extern int DoLink_Time,SearchVictim_Time,CheckCapture_Time;
void SyncroDoctor();
void PrepareSound();
void ProcessSelectedTower();
extern City CITY[8];
void DrawBorder(Brigade* BR);
int PreviewBrig=-1;;
void DrawAZones();
void ScenaryLights();
extern int NInGold[8];
extern int NInIron[8];
extern int NInCoal[8];
extern int NInStone[8];
void ShowCostPlaces();
int CheckSmartCreationAbility(byte NI,int Type,int* x2i,int* y2i);
bool Tutorial=0;
bool NoPress=0;
bool TutOver=0;
DWORD SStartTime=0;
int SStartX=0;
int SStartY=0;
int SCntrX=0;
int SCntrY=0;
extern int LastTimeStage;
extern int RealPause;
extern int CurrentStartTime[8];
extern int StartTime[8];
extern int LastStartTime[8];
extern int LastGameLength[8];
extern int CurrentStepTime;
void ShowChat();
void xLine(int x,int y,int x1,int y1,byte c);
void ShowExpl();
extern int BrigPnX;
extern int BrigPnY;
void ShowAbout();
void DrawShar(Nation* NT);
extern PingSumm PSUMM;
int GetMaxRealPing();
int GetAveragePing();
extern int AddTime;
extern int NeedAddTime;
extern int NSTT;
int PRVGT=0;
int GetPing(DWORD pid);
extern DWORD RealTime;
extern bool LockFog;
extern byte SYNBAD[8];
void ProcessRivEditor();
extern int NPROCM;
extern int TPROCM;
void DrawSMS();
void DRAW_MAP_TMP();
void ShowLockBars();
void ProcessVotingView();
void ProcessVotingKeys();
extern int PeaceTimeLeft;
void CmdEndPT();
int GameTime=0;
char* TMMS_NOPT=NULL;
char* TMMS_PT=NULL;
extern int CURTMTMT;
extern byte BaloonState;
extern byte CannonState;
extern byte XVIIIState;
extern int COUNTER1;
extern int COUNTER2;
void DottedLine(int x1,int y1,int x2,int y2,byte c){
	if(y1==y2){
		int n=(x2-x1)>>1;
		int xx=x1;
		for(int i=0;xx<x2;i++){
			if(i&1);
			int xx2=xx+4;
			if(xx2>x2)xx2=x2;
			if(!(i&1))GPS.DrawLine(xx,y1,xx2,y2,0xFFFF0000);
			xx+=4;
		};
	}else GPS.DrawLine(x1,y1,x2,y2,0xFFFF0000);
};
extern bool RecordMode;
bool IsGameActive();
bool CheckGameTime();
void AddRecordAndRatingSymbols(char* s){
	char ccc[256];
	ccc[0]=0;
	if(IsGameActive()){
		if(CheckGameTime()){
			strcpy(ccc,"0019");
		}else{
			strcpy(ccc,"[ 0019-?]");
		};
	};
	if(RecordMode)strcat(ccc,"001A");
	strcat(ccc,s);
	strcpy(s,ccc);
};
extern int CurPressure;
int GetTZValue(int ix,int iy);
void ShowX1(){
	int dy=smaply>>1;
	for(int ix=0;ix<smaplx;ix++){
		for(int iy=0;iy<dy;iy++){
			char cc[16];
			int ddy=(mapy&1)*16;
			sprintf(cc,"%d",GetTZValue(mapx+ix,(mapy>>1)+iy));
			ShowString(ix*32,(iy*32)-ddy,cc,&fn8);
			Hline(ix*32,(iy*32)-ddy,ix*32+1,255);
		};
	};
};
void ShowTexPreview(int xc,int yc);
extern int DrawPixMode;
extern int TIME1;
void ShowIllustrations();
void DrawLGround();
void DrawUnits();
#define LZM(x) (LMode?(x>>2):x)
void DrawLModeFog();
int CurAINF_Show=-1;
void DetermineWhereUnitIs(word Index,char* dest);
void DrawWay();
void AddWPoint(int x,int y);
int GetApproxDeep(int x,int y);
extern int MINESPRITEID;
extern int WayID;
extern int GT0,GT1,GT2,GT3,GT4,GT5;
extern bool CanProduce;
bool SHOWZONES=0;
extern word Att;
extern word Friend;
void SetCurrentDiffuse(DWORD);
#ifdef _USE3D
void ShowAllMarkers(){
	//return;

	GPS.FlushBatches();
	GPS.EnableZBuffer(0);
	int N=ImNSL[MyNation];
	word* IDS=ImSelm[MyNation];
	word* SNS=ImSerN[MyNation];
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB && OB->Serial==SNS[i] && OB->WasDrawn && !(OB->Sdoxlo||OB->BrigadeID!=0xFFFF)){
				DrawMarker(OB);
			};
		};
	};
	// auras
	//for(i=0;i<MAXOBJECT;i++){
	//	OneObject* OB=Group[i];
	//	if(OB&&!OB->GetLiveState()!=ls_LiveBody&&OB->ActiveAbility&&OB->ActiveAbility->ActiveAbilities.GetAmount()>0){
	//		DrawAurasMarker(OB);
	//	}
	//}
	// enemy under cursor
	if(Att!=0xFFFF){
		OneObject* OB=Group[Att];
		if( OB && !(OB->Sdoxlo||OB->newMons->Immortal) ){
			//byte cl=GetPaletteColor(255,0,0);			
			if(OB->BrigadeID!=0xFFFF){
				//Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
				//if(BR->Enabled) DrawBorder(BR);
			}else{
				SetCurrentDiffuse(0x80FF0000);
				DrawMarker(OB);
				SetCurrentDiffuse(0xFFFFFFFF);
			}			
		}
	}
	// friend under cursor
	if(Friend!=0xFFFF){
		OneObject* OB=Group[Friend];
		if(OB&&!OB->Sdoxlo){
			if(OB->BrigadeID==0xFFFF){
				SetCurrentDiffuse(0x80FFFFFF);
				DrawMarker(OB);
				SetCurrentDiffuse(0xFFFFFFFF);
			}			
		}
	}
	if(ImNSL[MyNation]==1){
		void ShowAttackRangePreview(OneObject* OB);
		word MID=ImSelm[MyNation][0];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==ImSerN[MyNation][0]){
				void ShowAttackRangePreview(OneObject* OB);
				ShowAttackRangePreview(OB);
			}
		}
	}
	GPS.FlushBatches();
	ITerra->FlushPatches();
};
#endif
void ShowBuildingPreview(int BuildingID,int xscr,int yscr){    
	GeneralObject* GO=NATIONS->Mon[BuildingID];
	NewMonster* NM=GO->newMons;
    if(EditMapMode){
        char ccc[128];
        sprintf(ccc,"MD : %s",NM->MD_File);
        ShowString(50,100,ccc,&BigYellowFont);
        sprintf(ccc,"ID : %s",GO->MonsterID);
        ShowString(50,135,ccc,&BigYellowFont);
    }
	int xxx,yyy;
	if(GetRealCoords(xscr,yscr,&xxx,&yyy)){
        if(!NM->Building){
            NewAnimation* NA=NM->GetAnimation(anm_Stand);
            if(NA){
                NA->DrawAt(0,MyNation,xxx,yyy,tmax(GetHeight(xxx,yyy),0)+(NM->MotionStyle==8?NM->StartFlyHeight:3),GetTickCount()/8, ((GO->newMons->StartScale-0.5)/256.0f),0xFF808080,0,0,NULL);
            }
            return;
        }
		if(NM->SpriteObject&&NM->SpriteVisual!=0xFFFF){
			if(!LMode){
				SprGroup* SG=&COMPLEX;
				int id=NM->SpriteVisual;
				ObjCharacter* OC=SG->Objects[id];
				if(OC->Stand){
					int tm1=div(tmtmt,OC->Delay).quot;
					int fr=div(tm1,OC->Frames).rem;
					int spr=fr*OC->Parts;
					int XX;//=mouseX-SG->Dx[id];
					int YY;//=mouseY-SG->Dy[id];
					NewAnimation* NA=OC->Stand;
					GetDiscreta(&xxx,&yyy,OC->Discret);
					XX=xxx-(mapx<<5)-OC->CenterX;
					YY=(yyy>>1)-(mapy<<4)-GetHeight(xxx,yyy)-OC->CenterY;

					if(CheckDist(xxx,yyy,OC->Radius)){
						if(NA->LineInfo){
							for(int p=0;p<NA->NFrames;p++){
								NewFrame* OF=NA->Frames[p];
								GPS.ShowGP(XX,YY,OF->FileID,OF->SpriteID,0);
							};
						}else{
							for(int p=0;p<OC->Parts;p++){
								NewFrame* OF=NA->Frames[spr+p];
								GPS.ShowGP(XX,YY,OF->FileID,OF->SpriteID,0);
							};
						};
					}else{
						if(NA->LineInfo){
							for(int p=0;p<NA->NFrames;p++){
								NewFrame* OF=NA->Frames[p];
								GPS.ShowGP(XX,YY,OF->FileID,OF->SpriteID,0);
							};
						}else{
							for(int p=0;p<OC->Parts;p++){
								NewFrame* OF=NA->Frames[spr+p];
								GPS.ShowGP(XX,YY,OF->FileID,OF->SpriteID,0);
							};
						};
					};
				};
			};
		}else{
			ShowMines(BuildingID);
			//BuildWall=0;
			if(!GO->Enabled){
				//					BuildMode=false;
			}
			int xxx,yyy;
			if(GetRealCoords(xscr,yscr,&xxx,&yyy)){
				int rp=0;
				for(int i=0;i<4;i++){
					int RR=NM->AttackRadius2[i];
					if(RR){
						void DrawColoredCircle(int xc,int yc,int R0,int R1,DWORD LineColor1,DWORD LineColor2,DWORD FillColor1,DWORD FillColor2);
						extern int LastMx;
						extern int LastMy;
						DrawColoredCircle(LastMx,LastMy,rp,RR
							,EngSettings.BuildingAttackRangeInnerLineColor
							,EngSettings.BuildingAttackRangeOuterLineColor
							,EngSettings.BuildingAttackRangeInnerFillColor
							,EngSettings.BuildingAttackRangeOuterFillColor);
					}
					rp=RR;
				}
				int x0=mouseX+LZM(NM->PicDx);
				int y0=mouseY+LZM(NM->PicDy);
				NewAnimation* UP=NM->GetAnimation(anm_StandHi);
				NewAnimation* DN=NM->GetAnimation(anm_StandLo);
				NewAnimation* WR=NM->GetAnimation(anm_Work);
				int NNN=5+int(8*(sin(double(GetRealTime())/100)+2));
				int xx3=xxx<<4;
				int yy3=yyy<<4;
				int AddGP=-1;
				int sprs[MaxAStages];
				if(NM->CompxCraft){
					AddGP=NM->CompxCraft->Stages[0].GPID;
					for(int i=0;i<MaxAStages;i++){
						if(NM->CompxCraft->Stages[i].GPID!=0){
							sprs[i]=NM->CompxCraft->Stages[i].Empty.SpriteID;
						}else{
							sprs[i]=-1;
						};
					};
				};
				MINESPRITEID=-1;
				bool R=ext_OnCheckingBuildPossibility(GSets.CGame.cgi_NatRefTBL[MyNation],BuildingID,xx3,yy3);
				R=CheckSmartCreationAbility(GSets.CGame.cgi_NatRefTBL[MyNation],BuildingID,&xx3,&yy3)!=-1;
				if(R){
					if(MINESPRITEID!=-1){
						OneSprite* OS=Sprites+MINESPRITEID;
						if(OS->Enabled){
							int u=GetUnitModifiedBySprite(BuildingID,OS);
							if(u==-1)return;
							NM=NATIONS->Mon[u]->newMons;

							UP=NM->GetAnimation(anm_StandHi);
							DN=NM->GetAnimation(anm_StandLo);
							WR=NM->GetAnimation(anm_Work);
							
							ObjCharacter* OC=OS->OC;
							char* S=NULL;
							switch(OC->IntResType){
							case 0:
								S=GetTextByID("MINEID0");
								break;
							case 1:
								S=GetTextByID("MINEID1");
								break;
							case 2:
								S=GetTextByID("MINEID2");
								break;
							case 3:
								S=GetTextByID("MINEID3");
								break;
							case 4:
								S=GetTextByID("MINEID4");
								break;
							case 5:
								S=GetTextByID("MINEID5");
								break;
							};
							if(S){
								//AssignHint(S,8);
							};
						};
					};
					x0=LZM((xx3>>4)-(mapx<<5)+NM->PicDx);
					y0=LZM((yy3>>5)-(mapy<<4)+NM->PicDy-GetHeight(xx3>>4,yy3>>4));
					xxx=xx3>>4;
					yyy=yy3>>4;
                    extern bool g_bForceBldCache;
                    g_bForceBldCache = true;
					IRS->SetTextureFactor(0xFFFFFFFF);
					if(DN->Enabled)DN->DrawAt(0,MyNation,xxx,yyy,GetHeight(xxx,yyy),0,1,0xFFFFFFFF,0,0,NULL);
					if(WR->Enabled)WR->DrawAt(0,MyNation,xxx,yyy,GetHeight(xxx,yyy),0,1,0xFFFFFFFF,0,0,NULL);
					IRS->SetTextureFactor(0xFFFFFFFF);
                    g_bForceBldCache = false;
				}else{
					xxx=xx3>>4;
					yyy=yy3>>4;
					GPS.SetCurrentDiffuse(0x80FF0000);
					IRS->SetTextureFactor(0x80FF0000);
					if(DN->Enabled)DN->DrawAt(0,MyNation,xxx,yyy,GetHeight(xxx,yyy),0,1,0x80FF0000,0,0,NULL);
					if(WR->Enabled)WR->DrawAt(0,MyNation,xxx,yyy,GetHeight(xxx,yyy),0,1,0x80FF0000,0,0,NULL);
					IRS->SetTextureFactor(0xFFFFFFFF);
					GPS.SetCurrentDiffuse(0xFFFFFFFF);
				};
				GPS.SetScale(1.0);
			};
		};
	};
}
void ApplyGroundZBias(bool TurnOn);
void ShowBuildingPreview(int BuildingID){
	ShowBuildingPreview(BuildingID,mouseX,mouseY);	
}
extern word Att;
void DrawBorder(Brigade* BR, byte cl);
void DrawMarker(OneObject* OB);
void SetCurrentDiffuse(DWORD);
void CheckMM_Change();
_inline int GetTotalHeightZ(int x,int y){
	int z=GetTotalHeight(x,y);
	if(z<0)z=0;
	return z;
}
void DrawWorldLine(float x0,float y0,float z0,float x1,float y1,float z1,DWORD Diffuse){
    if(z0<0)z0=0;
	if(z1<0)z1=0;
	Vector3D V1 = SkewPt(x0,y0,z0);
	Vector3D V2 = SkewPt(x1,y1,z1);
	GPS.EnableZBuffer(false);
	GPS.DrawLine(V1,V2,Diffuse);
}
void DrawWorldLine(float x0,float y0,float x1,float y1,DWORD Diffuse){
	DrawWorldLine(x0,y0,Mode3D?GetTotalHeightZ(x0,y0):0,x1,y1,Mode3D?GetTotalHeightZ(x1,y1):0,Diffuse);
}
bool chKey(int vk){
	return  GetKeyState(vk)&0x8000;
}

int GetHeightCB( int x, int y );

void SetGlobalClipPlane(Vector3D& pos,Vector3D& norm);
void FlushSprites( byte subsetIdx );
void FillGroundZBuffer();
void DrawAllComplexObjects(bool DrawOnlyModels);
void FlushFieldImage();
void UpdateCoastWaves();
void DrawWeaponSystem();
void DrawSpriteInsts();

extern bool g_bRenderShadowsPass;
void ArrangeZBuffer();

byte AUTO_SEEK_MANY_HANDLES=0;
void CleanTextures(){
	int N=IRS->GetNTextures();
	for(int i=0;i<N;i++){
		const char* s=IRS->GetTextureName(i);
		if(s){
			if(strstr(s,".JPG_") || strstr(s,".TGA_") || strstr(s,".BMP_")){
				IRS->DeleteTexture(i);
			}
		}
	}
}
#define defv(vert,v,c)vert.x=v.x;vert.y=v.y;vert.z=v.z;vert.diffuse=c;
DWORD getDXT3_color(byte* dxt,int Lx,int Ly,int x,int y){
	int Nx=Lx>>2;
	int Ny=Ly>>2;
	dxt+=(x>>2)*16+(y>>2)*Lx*4;
	x&=3;
	y&=3;
	word c1=((word*)dxt)[4];
	word c2=((word*)dxt)[5];

	int R1=(c1>>11)<<3;
	int G1=((c1>>5)&63)<<2;
	int B1=(c1&31)<<3;

	int R2=(c2>>11)<<3;
	int G2=((c2>>5)&63)<<2;
	int B2=(c2&31)<<3;

	int A=dxt[x/2+y*2];
	if(x&1)A&=0xF0;else A<<=4;

	byte ms=(dxt[8+4+y]>>(x*2))&3;
	DWORD C=A<<24;
	//C|=(R1<<16)+(G1<<8)+B1;
	switch(ms){
		case 0:C|=(R1<<16)+(G1<<8)+B1;break;
		case 1:C|=(R2<<16)+(G2<<8)+B2;break;
		case 2:C|=(((R1*2+R2)/3)<<16)+(((G1*2+G2)/3)<<8)+((B1*2+B2)/3);break;
		case 3:C|=(((R2*2+R1)/3)<<16)+(((G2*2+G1)/3)<<8)+((B2*2+B1)/3);break;
	}
	return C;
}
DWORD getDXT1_color(byte* dxt,int Lx,int Ly,int x,int y){
	int Nx=Lx>>2;
	int Ny=Ly>>2;
	dxt+=(x>>2)*8+(y>>2)*Lx*2;
	x&=3;
	y&=3;
	word c1=((word*)dxt)[0];
	word c2=((word*)dxt)[1];

	int R1=(c1>>11)<<3;
	int G1=((c1>>5)&63)<<2;
	int B1=(c1&31)<<3;

	int R2=(c2>>11)<<3;
	int G2=((c2>>5)&63)<<2;
	int B2=(c2&31)<<3;	

	byte ms=(dxt[4+y]>>(x*2))&3;
	DWORD C=0xFF000000;
	switch(ms){
		case 0:C|=(R1<<16)+(G1<<8)+B1;break;
		case 1:C|=(R2<<16)+(G2<<8)+B2;break;
		case 2:C|=(((R1*2+R2)/3)<<16)+(((G1*2+G2)/3)<<8)+((B1*2+B2)/3);break;
		case 3:C|=(((R2*2+R1)/3)<<16)+(((G2*2+G1)/3)<<8)+((B2*2+B1)/3);break;
	}
	return C;
}
//////////////////////////////////////////////////////////////////////////
#include "Rasterizator.h"
const int NSkySegm=8;
class SkyScanner:public Rasterizator{
	DWORD SkyColor[NSkySegm];
	int PrevModel;
	float m_PrevSkyRingScanFromFr;
	float m_PrevSkyRingScanStripeFr;
	byte* dxt;
	DWORD* CBUFFER;
	int cLx,cLy;
	int Lx,Ly;
//	float R;
	float m_ScanZFrom;
	float m_ScanZStripe;
	Vector3D GetSSV(VertexN* v);
	Vector3D V1,V2,V3;
	Vector3D uv1,uv2,uv3;
public:
	SkyScanner();
	virtual void OnPoint(int x,int y,float z);
	DWORD GetSkyColor();	
    void ScanSky(int SkyModel);
};
//////////////////////////////////////////////////////////////////////////
SkyScanner::SkyScanner(){
	memset(SkyColor,0,sizeof SkyColor);
	PrevModel=-1;
	m_PrevSkyRingScanFromFr = -1.0f;
	m_PrevSkyRingScanStripeFr = -1.0f;
}
Vector3D BaryCoords( float ax, float ay, float bx, float by, float cx, float cy,
					float ptX, float ptY );
void SkyScanner::OnPoint(int x,int y,float z){
	if(x>=0 && x<cLx && y>=0 && y<cLy){
		Vector3D v=BaryCoords(V1.x,V1.y,V2.x,V2.y,V3.x,V3.y,x,y);
		float U=uv1.x*v.x+uv2.x*v.y+uv3.x*v.z;
		float V=uv1.y*v.x+uv2.y*v.y+uv3.y*v.z;
		int xx=DWORD(U*Lx)%Lx;
		int yy=DWORD(V*Ly)%Ly;
		DWORD C=getDXT3_color(dxt,Lx,Ly,xx,yy);
		CBUFFER[x+y*cLx]=C;
	}
}
Vector3D SkyScanner::GetSSV(VertexN* v){
	int D=GetDir(int(v->x)*256,int(v->y)*256);
//	Vector3D V(D*cLx/256,6*v->z*cLy/R,0);
	Vector3D V(D * cLx / 256, (v->z - m_ScanZFrom) * cLy / m_ScanZStripe, 0.0f);
	return V;
}
void SkyScanner::ScanSky(int SkyModel){
	if(SkyModel != PrevModel || m_PrevSkyRingScanFromFr != MOptions.FogArgs.SkyRingScanFromFr || m_PrevSkyRingScanStripeFr != MOptions.FogArgs.SkyRingScanStripeFr) {
		const BaseMesh *pBM = IMM->GetGeometry(SkyModel, 0);
		if(pBM) {
			// Looking for sky model tex:
			SNode *pRoot = IMM->GetModelRoot(SkyModel);
			int TextID = -1;
			SNode::Iterator it(pRoot);
			while(it) {
				const Geometry *g = (Geometry *)(*it);
				if(g->IsA<Texture>()) {
					const char *pStr = ((Texture *)g)->GetTextureFile();
					TextID = IRS->GetTextureID(pStr);
				}
				++it;
			}
			if(TextID <= 0) {
				Log.Message("SkyModel is untextured!");
			} else {
				int pitch;
				dxt=IRS->LockTexBits(TextID,pitch);
				Lx=IRS->GetTextureWidth(TextID);
				Ly=IRS->GetTextureHeight(TextID);
				cLx=512;
				cLy=64;
				CBUFFER=new DWORD[cLx*cLy];
				memset(CBUFFER,0,cLx*cLy*4);
				VertexN *v = (VertexN *)pBM->getVertexData();
				word *idx = pBM->getIndices();
				const AABoundBox AABB = IMM->GetBoundBox(SkyModel);
				m_ScanZFrom = cMath::Lerp(AABB.minv.z, AABB.maxv.z, MOptions.FogArgs.SkyRingScanFromFr);
				m_ScanZStripe = cMath::Max(cMath::Epsilon, MOptions.FogArgs.SkyRingScanStripeFr * AABB.GetDZ());
				
				for(int i=0;i<pBM->getNPri();i++){
					int idx1=idx[i*3  ];
					int idx2=idx[i*3+1];
					int idx3=idx[i*3+2];
					V1=GetSSV(v+idx1);
					V2=GetSSV(v+idx2);
					V3=GetSSV(v+idx3);
					uv1=Vector3D(v[idx1].u,v[idx1].v,0);
					uv2=Vector3D(v[idx2].u,v[idx2].v,0);
					uv3=Vector3D(v[idx3].u,v[idx3].v,0);
					Rasterize(V1,V2,V3);
				}
				for(int i=0;i<NSkySegm;i++){
					int n=cLx/NSkySegm;
					int sr=0;
					int sg=0;
					int sb=0;
					int ns=0;
					for(int x=0;x<n*2;x++){
						for(int y=0;y<cLy;y++){
							DWORD C=CBUFFER[((x+i*n-n+cLx)%cLx)+y*cLx];
							if(C){
								sr+=(C>>16)&255;
								sg+=(C>>8)&255;
								sb+=C&255;
								ns++;
							}
						}
					}
					if(ns){
						sr/=ns;sg/=ns;sb/=ns;
					}
					SkyColor[i]=0xFF000000+(sr<<16)+(sg<<8)+sb;
				}
				delete[]CBUFFER;
				IRS->UnlockTexBits(TextID);
			}
		}
	}
	PrevModel=SkyModel;
	m_PrevSkyRingScanFromFr = MOptions.FogArgs.SkyRingScanFromFr;
	m_PrevSkyRingScanStripeFr = MOptions.FogArgs.SkyRingScanStripeFr;
}
float ExtraSkyDir=0;
DWORD SkyScanner::GetSkyColor(){
    Vector3D v=ICam->GetDirection();
    int d=DWORD(GetDir(v.x*256,v.y*256)+ExtraSkyDir*128/c_PI)&255;
	int n=256/NSkySegm;
	DWORD C1=SkyColor[d/n];
	DWORD C2=SkyColor[((d/n)+1)%NSkySegm];
	int m=(d%n)*NSkySegm;
	return MixDWORD(C1,C2,256-m,m);
}
SkyScanner SkyScan;
//////////////////////////////////////////////////////////////////////////
void DrawSkyCylinder(int ModelID){
	static BaseMesh* bm=NULL;
	if(!bm){		
		bm=new BaseMesh;
		bm->createPatch(1,3,Vector3D(0,0,0),Vector3D(1,0,0),Vector3D(0,1,0));		
		bm->setShader(IRS->GetShaderID("fogring"));
	}
	int fr=ICam->GetZf();
	Vector3D D0=ICam->GetDir();
	Vector3D pos=ICam->GetPosition();
	if(fr<pos.z)return;
	int R=sqrt(fr*fr-pos.z*pos.z);
	R-=64;	
	Vector3D V0=pos;
	V0.z=0;
	D0.z=0;
	D0.normalize();
	D0*=R;
	V0+=D0;
	D0.normalize();
	float L=256;
	Vector3D V1,V2;
	do{
        V1=V0;
		V2=V0;
		Vector3D VV(D0.y*L,-D0.x*L,0);
		V1+=VV;
		V2-=VV;
		Vector3D VC=V1;
		WorldToScreenSpace(VC);
//		if(VC.x<0||VC.x>RealLx)break;
		if(VC.x < -MOptions.FogArgs.FogRingHorMargin || VC.x > RealLx + MOptions.FogArgs.FogRingHorMargin) {
			break;
		}
		L=(L+256.0f)*4.0f/3.0f;
	}while(L<100000.0f);
	Vertex2t* vert=(Vertex2t*)bm->getVertexData();	
	SkyScan.ScanSky(ModelID);
	DWORD C1=SkyScan.GetSkyColor();//EngSettings.HorisonColorBlender;
	DWORD BC=MOptions.FogArgs.HorisonColorBlender;
	DWORD ModDWORD2X(DWORD C1,DWORD C2);
	C1=ModDWORD2X(C1,MOptions.FogArgs.HorisonColorModulator);
	C1=MixDWORD(C1,BC,256-(BC>>24),BC>>24);
	C1|=0xFF000000;
	DWORD C2=C1&0xFFFFFF;			
	extern float CameraMovingStage;
	IRS->SetFog(C1, MOptions.FogArgs.GetFogNearPlane(), MOptions.FogArgs.GetFogFarPlane(),
		MOptions.FogArgs.FogDensity, MOptions.FogArgs.FogType);
	V1.z=MOptions.FogArgs.FogRingHorisonWidth+MOptions.FogArgs.FogRingTotalWidth;
	V2.z=V1.z;
	defv(vert[0],V1,C2);
	defv(vert[1],V2,C2);
	V1.z=MOptions.FogArgs.FogRingHorisonWidth;
	V2.z=V1.z;
	defv(vert[2],V1,C1);
	defv(vert[3],V2,C1);
	V1.z=-MOptions.FogArgs.FogRingHorisonWidth;
	V2.z=V1.z;
	defv(vert[4],V1,C1);
	defv(vert[5],V2,C1);
	V1.z=-MOptions.FogArgs.FogRingHorisonWidth-MOptions.FogArgs.FogRingTotalWidth;
	V2.z=V1.z;
	defv(vert[6],V1,C2);
	defv(vert[7],V2,C2);
	DrawBM(*bm);
    IRS->Flush();
}
#include "Surface\Surface.h"
void UnitAbilityTest();
void GFieldShow(){	

    //UnitAbilityTest();
	Vector3D V=EngSettings.LightDir;
	V.normalize();
	IShadowMgr->SetLightDir(V);

    IRS->SetShadersQuality(GSets.SVOpt.HQShaders?0:1);
	IRS->SetBumpEnable(GSets.SVOpt.DrawBumpOnUnits);
    IEffMgr->SetCurrentNatColor(0);

	TreesMesh.CreateMeshes();

	static int Details=IRS->GetTextureID("Details.dds");
	IRS->SetTextureOverride(Details,4);

	static int prev_mm_refresh=GetTickCount();
	if(GetTickCount()-prev_mm_refresh>20000){
        void UpdateMiniMap();
        UpdateMiniMap();
		//CreateMiniMap();
		prev_mm_refresh=GetTickCount();		
	}
	if(rand()<256)IPM->Purge();
	void InternalProcess();
	InternalProcess();
    if (!SHOWSLIDE) return;

	IRMap->SetReflectTerrain(GSets.SVOpt.DrawReflectedTerrain);

	if(AUTO_SEEK_MANY_HANDLES){//auto checking for extra-dirty memory leaking
		int FMM_GetNHandlers();
		void ProcessEditor(char* EditorName);
		if(AUTO_SEEK_MANY_HANDLES==1){
			AUTO_SEEK_MANY_HANDLES=2;
			ProcessEditor("PrintLeaks");
		}else if(AUTO_SEEK_MANY_HANDLES==2){
			if(FMM_GetNHandlers()>500000){//ohh, stupid leakers !!!
				AUTO_SEEK_MANY_HANDLES=3;
				ProcessEditor("PrintLeaks");                                
			}
		}
	}

	void ClearTempSoundBuf();
	ClearTempSoundBuf();
	///GPS.DrawFillRect(0,0,RealLx,RealLy,0xFFFF0000);
	void DoSmartThing();
	DoSmartThing();
	GPS.FlushBatches();
	CheckMM_Change();
	COPYSizeX=RealLx;
	RSCRSizeY=RealLy;
	RECT OOO;
	OOO.left=0;
	OOO.top=0;
	OOO.right=RealLx;
	OOO.bottom=RealLy;
	ClipCursor(&OOO);
	GPS.SetClipArea(0,0,RealLx,RealLy);
	RunPF(256*5+10,"GFieldShow,part1");
	PrepareSound();
	int i;
	int uuu=(tmt&31);
	char ggg[600];
	tmt++;
	time1=GetRealTime();
	time0=GetRealTime();	
    
    if(GSets.ShadowQuality<2)TreesMesh.DrawTreesShadows();


	if(CheckIfNewTerrain()){
        //IRS->ClearDevice(0xFF306030);
	}
	static float z0=840;
	if(GetKeyState(VK_CONTROL)&0x8000){
		z0+=float(GameSpeed)/1000;
	}
	if(GetKeyState(VK_SHIFT)&0x8000){
		z0-=float(GameSpeed)/1000;
	}

    //  reset user clip plane
	SetGlobalClipPlane( Vector3D( 0, 0, z0 ), Vector3D( 0, 0, -1 ) );
    GPS.EnableClipping( false );

    //  setup scene camera
	SetupCamera();

	//  setup light
	ILight*	iLight = GetGameLight();
	if (iLight) iLight->Render();
    
       //  pre-render units
	static int smooth = IRS->GetShaderID( "smooth" );
	IRS->SetShader( smooth );
    PGR1.Start(0,3);
    DrawUnits();	
    PGR1.End(0,3);

    //  pre-render weapon system effects
    DrawWeaponSystem();
	void ClearActiveBones();
	ClearActiveBones();

    //  3d objects shadow rendering pass
    g_bRenderShadowsPass = true;    
    DrawAllComplexObjects( true );

    //  render ground sprite objects
    PGR1.Start(0,3);
    DrawSpriteInsts();    
    ArrangeZBuffer(); 
    PGR1.End(0,3);

    if(GSets.SVOpt.DrawWater){
        IRS->SetTextureFactor(0xFF808080);
        IRMap->Render();
    }

    ///FlushSprites( 5 );
    g_bRenderShadowsPass = false;
    
    //  create shadowbuffer
	IShadowMgr->Enable(GSets.SVOpt.Draw3DShadows);
    IShadowMgr->Render();    	

	IRS->EnablePostEffects(
		GSets.SVOpt.EnablePostEffects,
		GSets.SVOpt.AutoBrightDegree,
		GSets.SVOpt.AutoBrightAdaptSpeed,0,
		GSets.SVOpt.MotionBlurAlpha);
	IRS->PreparePosteffects();

	if(g_SurfEditor.Wireframe) { // by SERGYI:
		IRS->ClearDevice(0xFFA0A0A0,true,true);
	} else {
		IRS->ClearDevice( MOptions.FogColor,true,true );
	}
	
	// Copy FogArgs from EngineSettings if map doesn't have custom:
	if(!MOptions.FogArgs.UseFromMap) {
		MOptions.FogArgs.SetFromEngineSettings();
	}
	
	//  render skybox 
    static int smid=IMM->GetModelID( "Models\\SkyBox\\SkyBox.c2m" );
	int mID = MOptions.SkyBoxModel;
    if(mID<=0)mID=smid;
	if(mID!=-1 && MOptions.FogArgs.ShowSkyDome){
		Vector3D V=ICam->GetPosition();
		V.z = (float)MOptions.FogArgs.SkyShift;
		Matrix4D M;
		AABoundBox AABB=IMM->GetBoundBox(mID);
		const float Radius = AABB.GetBoundSphere().GetRadius();
		const float Scale = (float)MOptions.FogArgs.SkyRadius / Radius;
//		float zf=ICam->GetZf();
//		float zn=ICam->GetZn();
//		float dsc=(zf+zn)/2/fabs(AABB.maxv.x);
//		float sc=dsc;
//		static float X = 2.0f;
//		sc *= X;
		if(MOptions.FogArgs.SkyRotationPeriod)ExtraSkyDir=float(GetTickCount())*c_PI*2/1000/MOptions.FogArgs.SkyRotationPeriod;
		else ExtraSkyDir=0;
		M.srt(Scale,Vector3D::oZ,-ExtraSkyDir,V);
		IMM->StartModel( mID, M, 0 );
		IMM->DrawModel();
        //IRS->ResetWorldTM();
		DrawSkyCylinder(mID);
	}
	
	static int TerraSH_Lo  = IRS->GetShaderID( "low\\terra_shadowed_hq" );
    static int TerraSH_Hi  = IRS->GetShaderID( "terra_shadowed_hq" );
	static int FogColorVar_Lo = IRS->GetShaderVarID( TerraSH_Lo, "TerraFogColor" );
    static int FogColorVar_Hi = IRS->GetShaderVarID( TerraSH_Hi, "TerraFogColor" );
	ColorValue fogClr( MOptions.FogColor );
	IRS->SetShaderVar( TerraSH_Lo, FogColorVar_Lo, Vector4D( fogClr.r, fogClr.g, fogClr.b, 1.0f ) );
    IRS->SetShaderVar( TerraSH_Hi, FogColorVar_Hi, Vector4D( fogClr.r, fogClr.g, fogClr.b, 1.0f ) );
    
    //  restore viewport
	SetRLCWindow(0,0,RealLx,RealLy,SCRSizeX);
	
    //  render terrain
    PGR1.Start(0,0);
    //IRS->ResetWorldTM();
	FillGroundZBuffer();	
    PGR1.End(0,0);

    if(GSets.SVOpt.DrawRoads){
    	void DrawAllRoads();
	    DrawAllRoads();
    }

	//static int zsh2=IRS->GetShaderID( "sprite_buildings" );
	//GPS.SetShader(zsh2);
	//GPS.SetCurrentDiffuse(0xFFFFFFFF);
	//ShowAllMarkers();
	//GPS.SetShader(zsh2);
	//GPS.SetCurrentDiffuse(0xFFFFFFFF);

    //  render effects layer
    if(GSets.SVOpt.DrawEffects){
        PGR1.Start(0,1); 
        IEffMgr->PreRender();    
        IEffMgr->PostEvaluate();
        PGR1.End(0,1);
    }
    //  render wheat fields
    FlushFieldImage();//enabled FlameMaster

    IRS->SetTextureFactor(0xFFFFFFFF);
    GPS.SetCurrentDiffuse(0xFFFFFFFF);
    ShowAllMarkers();

    //void ShowSpritesShadows();
    //ShowSpritesShadows();

	ACTION_ProcessingDrawing(0);		
	ext_OnDrawOnMapAfterLandscape();
    
	
    //-----INSERT NEW RELIEF THERE----//	
	GPS.EnableZBuffer(false);
	for(int i=0;i<MBR4;i++){
		DWORD p=BMASK[i];
		if(p){
			for(int j=0;j<32;j++){
				if(p&1){
					extern int BCOLOR;
					Brigade* BR=CITY[BCOLOR].Brigs+(i<<5)+j;
					if(BR->Enabled){

						if(!(NATIONS[BR->CT->NI].NMask & NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NMask)){
							GPS.SetCurrentDiffuse(0xFFFF0000);
						}
						DrawBorder(BR);
						GPS.SetCurrentDiffuse(0xFFFFFFFF);
					}
				};
				p>>=1;
			};
		};
	};
	GPS.FlushBatches();	
	
    // show att target
	if(Att!=0xFFFF){
		OneObject* OB=Group[Att];
		if( OB && !(OB->Sdoxlo||OB->newMons->Immortal) ){
			//byte cl=GetPaletteColor(255,0,0);				
			if(OB->BrigadeID!=0xFFFF){
				Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
				if(BR->Enabled){
					SetCurrentDiffuse(0xFFDD0000);
					DrawBorder(BR);
					SetCurrentDiffuse(0xFFFFFFFF);
				}
			}else{
				//DrawMarker(OB);
			}				
		}
	}		
	
    GPS.EnableZBuffer(true);
	
    RunPF(256*5+11,"ShowExpl");
	ShowExpl();
	StopPF(256*5+11);
	RunPF(256*5+12,"DrawWeaponUsing3D,3,2");

	//void DrawWeaponUsing3D(byte);
	//DrawWeaponUsing3D(3);
	//DrawWeaponUsing3D(2);

	for(int v=0;v<8;v++)DrawShar(NATIONS+v);
	void TestWalls();
	TestWalls();
	void DoSmartThing2();
	DoSmartThing2();

	StopPF(256*5+12);
	static int shNoShadow = IRS->GetShaderID( "units_without_shadow" );
	GPS.SetShader( shNoShadow );
	
	ACTION_ProcessingDrawing(30);		
	
    //  draw 3d landscape elements
	IRS->SetTextureFactor(0xFF808080);
    if(GSets.SVOpt.DrawTrees){
	    RenderModels.Draw();
	    TreesMesh.DrawTreesMeshes();
    }
	

	IRS->ResetWorldTM();
	GPS.FlushBatches();
	GPS.EnableZBuffer( true );	
	
	RunPF(256*5+13,"ShowZBuffer");
	PRFUNC(ShowZBuffer());
	StopPF(256*5+13);
	RunPF(256*5+14,"ZInternal");		

    //  draw units without shadow
    static int shShadow = IRS->GetShaderID( "units_without_shadow" );
    GPS.SetShader( shShadow );
    IRS->SetShader( shShadow );		
    PRFUNC(FlushSprites( 1 ));	

	//  draw unit shadows, z-write disabled
	static int shShadowZ = IRS->GetShaderID( "units_shadow" );
	GPS.SetShader( shShadowZ );
	IRS->SetShader( shShadowZ );
	FlushSprites( 2 );

    //  draw animated trees with linear filtering
    GPS.FlushBatches();		
    GPS.EnableZBuffer( false );
    static int shHud2sm = IRS->GetShaderID( "animated_trees" );
    GPS.SetShader( shHud2sm );
    IRS->SetShader( shHud2sm );		
    ISM->SetTexCoordBias( 0.5f/256.0f );
    FlushSprites( 4 );
    ISM->SetTexCoordBias( 0.0f );
	
	//  draw buildings & stuff
	static int shHud2 = IRS->GetShaderID( "sprite_buildings" );
	GPS.SetShader( shHud2 );
	IRS->SetShader( shHud2 );
	FlushSprites( 3 );
	FlushSprites( 6 );

	//  draw 3d units
    FlushSprites( 5 );
	PGR1.Start(0,3);
	void FlushSpriteInsts();
	FlushSpriteInsts();
	PGR1.End(0,3);

    //  draw complex units' 3d parts
    DrawAllComplexObjects( true );
	DrawAllComplexObjects( false );

	
	
    GPS.FlushBatches();		
	GPS.EnableZBuffer( false );

	ext_OnDrawOnMapAfterUnits();
	ext_OnDrawOnMapBeforeWater();
	
    //  draw water
    GPS.EnableZBuffer( true );
    IRS->ResetWorldTM();
    HandleWater(); // UNCOMMENT THIS!
	if(GSets.SVOpt.DrawWater){
        //IRS->ResetWorldTM();
		IRS->SetTextureFactor(0xFF808080);		
        IWater->SetShaderQuality(GSets.SVOpt.DrawHQWater);
		IWater->Render();
	}
	void RenderQuads();
	void ClearQuads();
	//ClearQuads();
	RenderQuads();
    
    //UpdateCoastWaves();
    
    //IRS->ResetWorldTM();
	void dead_units_postprocess();
	dead_units_postprocess();
    if(ICam)ICam->Render();
    IRS->SetViewPort( Rct( 0, 0, RealLx, RealLy ) );

    //  post-render effects layer
    //if(ICam)ICam->ShiftZ(effBias);
    //ITerra->SetCallback( GetHeight );
    if(GSets.SVOpt.DrawEffects){
	    //IEffMgr->Evaluate();
        PGR1.Start(0,1);         
        IEffMgr->Evaluate(0.04f*GameSpeed/256);
        IEffMgr->PostRender();
        PGR1.End(0,1);
    }
    //ITerra->SetCallback	( GetHeightCB );
    //if(ICam)ICam->ShiftZ(-effBias);

	//the dirtiest of dirtiest hack of all times and nations!!!
	GPS.ShowGP(RealLx,0,0,0,0);
	GPS.FlushBatches();
	//end of megadirty hack

	//void DrawFastSprites();
	//DrawFastSprites();
	ext_OnDrawOnMapAfterWater();
	//void DrawWeaponUsing3D(byte);
	//DrawWeaponUsing3D(1);
	//DrawWeaponUsing3D(0);		
	GPS.EnableZBuffer( false );
	//void PiroDraw();
	//PiroDraw();
	//void ProcessParticles();
	//ProcessParticles();

	GPS.EnableClipping( true );
	void ShowTreeShotPass();
	ShowTreeShotPass();
	GPS.SetScale(1.0);
	void draw_SelPatches();
	draw_SelPatches();

    IRS->SetViewPort( Rct( 0, 0, RealLx, RealLy ) );
	ext_OnDrawOnMapAfterTransparentEffects();
    IRS->SetViewPort( Rct( 0, 0, RealLx, RealLy ) );
	ShowPulse();
	if(CINFMOD)ShowAllBars();		
	if(!NoText)LShow();
	extern int DrawPathID;
	extern int DrawDestID;	
	void DrawActiveBones();
	DrawActiveBones();
	StopPF(256*5+24);
	if(DrawDestID!=0xFFFF){
		OneObject* OB=Group[DrawDestID];
		if(OB){
			int x=OB->DestX/16;
			int y=OB->DestY/16;
			int z=GetHeight(x,y);
			DrawWorldLine(x-4,y-8,z,x+4,y+8,z,0xFFFFFFFF);
			DrawWorldLine(x-4,y+8,z,x+4,y-8,z,0xFFFFFFFF);
			if(OB->DestX>0){
				DrawWorldLine(OB->RealX>>4,OB->RealY>>4,OB->RZ,x,y,z,0xFFFF0000);
			}
			GPS.FlushBatches();

		};
	};
	if(DrawPathID!=0xFFFF){
		OneObject* OB=Group[DrawPathID];
		if(OB){
			if(OB->PathX&&OB->NIPoints){
				int N=OB->NIPoints;
				int px=-1;
				int py=-1;
				int pz;
				for(int i=0;i<N;i++){
					int LX=OB->Lx;
					int x=(int(OB->PathX[i])<<4)+LX*8-8;
					int y=(int(OB->PathY[i])<<4)+LX*8-8;
					int z=GetHeight(x,y);
					DrawWorldLine(x-2,y,z,x+2,y,z,0xFFFFFF00);
					DrawWorldLine(x,y+4,z,x,y-4,z,0xFFFFFF00);
					if(px!=-1){
						DrawWorldLine(x,y,z,px,py,pz,0x80FFFF00);                            
					}
					px=x;
					py=y;
					pz=z;
				};
				GPS.FlushBatches();
			};
		};
	};

	RunPF(256*5+9, "DrawSMS");
	if((EditMapMode || SHOWZONES) && !g_SurfEditor.HideUI)
	{
		DrawAZones();
		if(!NoText){
			for(i=0;i<8;i++){
				int x=mapPos[i<<1];
				int y=mapPos[(i<<1)+1];
				if(x>=mapx&&y>=mapy&&x<mapx+smaplx&&y<mapy+smaply){
					sprintf(ggg,"%d",i+1);
					ShowString(smapx+((x-mapx)<<5)+8,smapy+((y-mapy)<<5)+2,ggg,&BigWhiteFont);
				};
			};
		};
	};
	
	int v1=3;
	int v2=158;
	void ProcessVideo();
	ProcessVideo();
	void DrawLockedWays();
	DrawLockedWays();
	static bool dosh=true;
	if(EditMapMode && GetKeyState(VK_CAPITAL)){		
		//if(dosh)IShadowMgr->DrawDebugInfo();
	}
	if(EditMapMode&&!NoText){
		//if(ChoosePosition)
		//	sprintf(ggg,GETSPOS,MyNation+1);//"���%d : ������������"
		//else sprintf(ggg,PLAYMES,MyNation+1);//"���%d"
		//ShowString(smapx+256,smapy+64,ggg,&WhiteFont);
	};
	if(Creator<4096+200){
		ShowBuildingPreview(Creator);
		if(mouseX<RealLx-256-20){
			if(WheelDelta<0)Creator--;
			if(WheelDelta>0)Creator++;
			if(Creator<0)Creator=NATIONS->NMon-1;
			if(Creator>=NATIONS->NMon)Creator=0;
			WheelDelta=0;
		}
	}else
	if(BuildMode&&!NoText){
		ShowBuildingPreview(BuildingID);
    };
	
    SetRLCWindow(0,0,COPYSizeX,RSCRSizeY,SCRSizeX);
	
	//if(FogMode)ShowFoggedBattle();
	void ShowRangesOfSelectedUnits();
	ShowRangesOfSelectedUnits();
	FList.Draw();
	DrawSMS();
	ProcessSelectedTower();
	ScenaryLights();
	ShowIllustrations();
	
	void OnMapDraw();
	OnMapDraw();
	void ShowDebugPlList();
	ShowDebugPlList();
	// by vital
	//GPS.DrawFillRect(0,0,RealLx,RealLy,EngSettings.AtmosphereColor,EngSettings.AtmosphereColor,0,0);
	GPS.FlushBatches();
	//ISM->Flush();

	extern word TransparentBuildingID;
	extern word PrevTransparentBuildingID;
	extern word TransparentBuildingAlpha;
	int D=GameSpeed*2;
	bool doprev=true;
	if(TransparentBuildingID!=0xFFFF){
		TransparentBuildingAlpha+=D;
		if(TransparentBuildingAlpha>4096)TransparentBuildingAlpha=4096;		
	}else{
		if(TransparentBuildingAlpha>D)TransparentBuildingAlpha-=D;
		else TransparentBuildingAlpha=0;
	}
	if(TransparentBuildingID!=0xFFFF){
		PrevTransparentBuildingID=TransparentBuildingID;
		TransparentBuildingID=0xFFFF;
	}
	GPS.FlushBatches();
	if(SHOWSLIDE&&FogMode&&BaloonState!=1&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision)&&!NoText){
		if(LMode){
			if(NOPAUSE&&!(LockFog||NoPFOG))ProcessFog1();
			DrawFog();
		}else{
			time0=GetRealTime();
			if(/*NOPAUSE&&*/!(LockFog||NoPFOG))ProcessFog1();
			CreateFogImage();
			DrawFog();
			time4=GetRealTime()-time0;
		};
	};
	ext_OnDrawOnMapAfterFogOfWar();
	IRS->RenderPosteffects();
	void ShowQuestDialogs();
	ShowQuestDialogs();
	if(PlayGameMode){
		extern int TrueTime;
		int A=255-TrueTime/20;
		if(A>0){
		    GPS.DrawFillRect(0,0,RealLx,RealLy,(DWORD(A)<<24)+0x202020);
		}
	}
	void DrawCollision();
	DrawCollision();
	void DrawSelUnitExitPath();
	DrawSelUnitExitPath();
	DrawCurves();
	ShowCostPlaces();
	if(GridMode)DrawGrids();
	ProcessVotingView();
	ShowChat();

    //UnitAbilityTest();

	if(NoText)return;
	DrawDangerMap();
	ProcessHint();
	ShowDestn();
	
	StopPF(256*5+9);
	int t0=GetRealTime();
	if(!PRVGT)PRVGT=t0;
	if(t0-PRVGT>2000){
		ADDGR(0,t0,0,0xFF);
		ADDGR(0,t0,(AddTime)>>7,0xD0);
		ADDGR(0,t0,(NeedAddTime)>>7,0xD4);
		ADDGR(1,t0,CurrentStepTime,0xD0);
		if(PSUMM.NPL&&PSUMM.PSET[0].NPings>5){
			for(int i=0;i<NPlayers;i++){
				int p=GetPing(GSets.CGame.PL_INFO[i].PlayerID);
				ADDGR(2,t0,p,0xD0+GSets.CGame.PL_INFO[i].ColorID*4);
			};
		};
		PRVGT=t0;
	};


	time7=GetRealTime()-time0;
	if(SeqErrorsCount&&PlayGameMode!=1){
		//SYN.Save();
		int TTT=GetTickCount()/1500;
		extern bool HideBorderMode;
		if(!HideBorderMode){
			if(TTT&1)ShowString(HintX,HintY-100,DELSYNC,&BigWhiteFont);
			else ShowString(HintX,HintY-100,DELSYNC,&BigRedFont);
		};
		SeqErrorsCount=0;
		SyncroDoctor();
	};//else SYN.Clear();
	if(EditMapMode){
		ProcessRivEditor();
		if(LockMode)ShowLockBars();
		if(DrawPixMode&&(GetKeyState(VK_MENU)&0x8000))ShowTexPreview(mouseX,mouseY);
	}else ShowWinner();
	//IRS->RenderPosteffects();
	if(GameExit)return;

	
	//DRAW_MAP_TMP();
	//information
	int xxx0=RealLx-400;
	int yyy0=smapy+RealLy-170;
	if(HeightEditMode==1){
		ShowString(xxx0,yyy0,HIEDMOD,&YellowFont);//"������� ���"
		yyy0-=20;
	};
	if(HeightEditMode==2){
		ShowString(xxx0,yyy0,ADDSPOB,&YellowFont);//"���� ��������."
		yyy0-=20;
	};
	if(HeightEditMode==3){
		ShowString(xxx0,yyy0,DELSPOB,&YellowFont);//"��� ���."
		yyy0-=20;
	};
	if(MEditMode){
		ShowString(xxx0,yyy0,TEXMODE,&YellowFont);//"����������."
		yyy0-=20;
	};
	if(LockMode==1){
		ShowString(xxx0,yyy0,UNBLOCK,&YellowFont);//"�����������������"
		yyy0-=20;
	};
	if(LockMode==2){
		ShowString(xxx0,yyy0,DOBLOCK,&YellowFont);//"���������������."
		yyy0-=20;
	};
	if(LockMode==3){
		ShowString(xxx0,yyy0,DFBLOCK,&YellowFont);//"�������� � ����."
		yyy0-=20;
	};
	if(PeaceMode){
		if(!g_SurfEditor.HideUI) ShowString(xxx0,yyy0,PEACMOD,&YellowFont);//"�����."
		yyy0-=20;
	};
	if(WaterEditMode){
		switch(WaterEditMode){
		case 1:
			ShowString(xxx0,yyy0,SETWATT,&YellowFont);//"���������� ��: �������."
			break;
		case 2:
			ShowString(xxx0,yyy0,DELWATT,&YellowFont);//"���������� ��: ���� ��."
			break;
		case 3:
			ShowString(xxx0,yyy0,SETCOST,&YellowFont);//"���������� ��: ��������."
			break;
		case 4:
			ShowString(xxx0,yyy0,DELCOST,&YellowFont);//"���������� ��: ���� ���."
			break;
		};
		yyy0-=20;
	};

	if(tmtmt>20&&!Inform){
		if(LastTimeStage==-1){
			LastTimeStage=GetRealTime();
		}else{
			int dd=GetRealTime()-LastTimeStage;
			if(!NOPAUSE){
				dd=0;
				LastTimeStage=GetRealTime();
			};
			if(dd>1000){
				dd/=1000;
				if(GSets.CGame.PL_INFO[0].GameTime>dd)GSets.CGame.PL_INFO[0].GameTime-=dd;
				else GSets.CGame.PL_INFO[0].GameTime=0;
				GameTime+=dd;
				if(PeaceTimeLeft>1){
					PeaceTimeLeft-=dd;
					if(PeaceTimeLeft<1)PeaceTimeLeft=1;
				}else if(PeaceTimeLeft==1){
					CmdEndPT();
				};
				LastTimeStage+=dd*1000;
			};
		};
		char cc1[1024];
		char ccr[400];
		int DY=0;
		if(GSets.CGame.PL_INFO[0].VictCond && GSets.CGame.ViewMask&2 && MOptions.VictoryRule.Get()){
			int t=GSets.CGame.PL_INFO[0].GameTime;
			int s=t%60;
			int m=(t/60)%60;
			int h=t/3600;
			if(s<10&&m<10)sprintf(cc1,"%d:0%d:0%d",h,m,s);
			else if(s<10&&m>=10)sprintf(cc1,"%d:%d:0%d",h,m,s);
			else if(s>10&&m<10)sprintf(cc1,"%d:0%d:%d",h,m,s);
			else sprintf(cc1,"%d:%d:%d",h,m,s);	
			char cc4[400];
			//cc4[0]=0;
			//AddRecordAndRatingSymbols(cc4);
			ShowString(minix+2,miniy-22+2-4+DY-8,cc1,&SmallBlackFont);
			//AddRecordAndRatingSymbols(cc1);
			ISM->Flush();
			ShowString(minix,miniy-22-4+DY-8,cc1,&SmallWhiteFont);
			DY=-14;
			ISM->Flush();
		};
		byte ord[8];
		int  par[8];
		int no=SortPlayers(ord,par);
		byte prevms=0;
		extern bool HideBorderMode;
		
		if((GSets.CGame.ViewMask&2)&&!((SCENINF.hLib||PlayGameMode==1||HideBorderMode))){
			extern bool MiniMapVisible;
			if(MiniMapVisible&&MOptions.VictoryRule.Get()){
				if(PeaceTimeLeft){
					if(!TMMS_PT)TMMS_PT=GetTextByID("TMMS_PT");
					int H1=GameTime/3600;
					int H2=PeaceTimeLeft/3600;
					int M1=(GameTime/60)%60;
					int M2=(PeaceTimeLeft/60)%60;
					int S1=GameTime%60;
					int S2=PeaceTimeLeft%60;
					extern int TrueTime;
					sprintf(cc1,TMMS_PT,H1,M1/10,M1%10,S1/10,S1%10,TrueTime/1000,H2,M2/10,M2%10,S2/10,S2%10);
					char cc4[400];
					cc4[0]=0;
					AddRecordAndRatingSymbols(cc4);
					//ShowString(minix+2+GetRLCStrWidth(cc4,&SmallBlackFont),miniy-22+2-4+DY,cc1,&SmallBlackFont);
					AddRecordAndRatingSymbols(cc1);
					//ShowString(minix,miniy-22-4+DY,cc1,&SmallWhiteFont);
				}else{
					if(!TMMS_NOPT)TMMS_NOPT=GetTextByID("TMMS_NOPT");
					int H1=GameTime/3600;
					int M1=(GameTime/60)%60;
					int S1=GameTime%60;
					char cc4[400];
					cc4[0]=0;
					extern int TrueTime;
					sprintf(cc1,TMMS_NOPT,H1,M1/10,M1%10,S1/10,S1%10,TrueTime/1000);
					AddRecordAndRatingSymbols(cc4);
					//ShowString(minix+2+GetRLCStrWidth(cc4,&SmallBlackFont),miniy-22+2-4+DY,cc1,&SmallBlackFont);
					AddRecordAndRatingSymbols(cc1);
					//ShowString(minix,miniy-22-4+DY,cc1,&SmallWhiteFont);
				};
				if(no&&FullMini){
					int y=miniy-22-4-no*14+DY;
					for(int q=0;q<no;q++){
						byte ms=NATIONS[GSets.CGame.cgi_NatRefTBL[ord[q]]].NMask;
						CEXPORT DWORD GetNatColor( int natIdx );
						DWORD c=GetNatColor(ord[q]);
						if(!(ms&prevms)){
							for(int w=q;w<no&&(NATIONS[GSets.CGame.cgi_NatRefTBL[ord[w]]].NMask&ms);w++);
							w-=q;
							if(w>1){
								int y0=y+1+2;
								int y1=y+10+(w-1)*14;
								int yc=(y0+y1)>>1;
								int x=minix-2;
								GPS.DrawLine(x,y0,x-2,y0+2,c);
								GPS.DrawLine(x-2,y0+2,x-2,yc-2,c);
								GPS.DrawLine(x-2,yc-2,x-4,yc,c);
								GPS.DrawLine(x-4,yc,x-2,yc+2,c);
								GPS.DrawLine(x-2,yc+2,x-2,y1-2,c);
								GPS.DrawLine(x-2,y1-2,x,y1,c);
								char cc[400];
								cc[0]=0;
							};
						};
						prevms=ms;
						if((!SYNBAD[ord[q]])||(GetTickCount()>>9)&1){
							CEXPORT DWORD GetNatColor( int natIdx );
							GPS.DrawFillRect(minix+1,y+3,8,8,GetNatColor(GSets.CGame.cgi_NatRefTBL[ord[q]]));							
						};
						ShowVictInFormat(ccr,GSets.CGame.cgi_NatRefTBL[ord[q]],GSets.CGame.PL_INFO[0].VictCond);
						//
						void LimitString(char* str,lpRLCFont FONT,int L);
						_str n;
						n=GetPName(ord[q]);
						//LimitString(n.pchar(),&SmallBlackFont,50);
						//
						sprintf(cc1,"%s (%s)",n.pchar(),ccr);
						ShowString(minix+18+2,y+2,cc1,&SmallBlackFont);
						ISM->Flush();
						ShowString(minix+18,y,cc1,&SmallWhiteFont);
						ISM->Flush();
						int L=GetRLCStrWidth(cc1,&SmallWhiteFont);
						int vs=NATIONS[ord[q]].VictState;
						if(vs==1)GPS.DrawLine(minix-1,y+8,minix+20+L,y+8,0xFFFF0000);
						if(vs==3)DottedLine(minix-1,y+8,minix+20+L,y+8,c);
						y+=14;
					}
				}
			}
		}
	}
	GPS.FlushBatches();

	if(Group[WayID])AddWPoint(Group[WayID]->RealX>>4,(Group[WayID]->RealY>>5)-Group[WayID]->RZ);
	
	DrawWay();
	
	//if(PlayGameMode==1)ShowAbout();
	if(!(NOPAUSE||LockPause||EnterChatMode) && !g_SurfEditor.HideUI){
		extern char* PAUSE;
		int L=GetRLCStrWidth(PAUSE,&BigWhiteFont);
		int x=(RealLx-L)>>1;
		int y=RealLy>>1;
		DrawStdBar1(x-64,y-8,x+L+64,y+20+7);
		ShowString(x-2,y-6,PAUSE,&BigYellowFont); //BigWhiteFont
	};

	

	DrawAllGrp();

	int uxxx=mapx+((mouseX-smapx)>>5);
	int uyyy=mapy+div24(mouseY-smapy);
	//OutCInf(uxxx,uyyy);
	uxxx=(mapx<<1)+((mouseX-smapx)>>4);
	uyyy=(mapy<<1)+((mouseY-smapy)>>3);
	//if(EditMapMode)return;	
	ext_OnDrawOnMapOverAll();
	if(InfoMode)return;
	int ssy=smapy+3;
	
	ssy+=300;
	int stDY=14;
	extern int LastMx;
	extern int LastMy;
	sprintf(ggg,"x=%d y=%d" ,LastMx,LastMy);
	counter=0;
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;
	sprintf(ggg,"Hi=%d" ,TerrHi);
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;
	_str rules;
	void CollectRules(_str& dest,int x,int y);	
	CollectRules(rules,LastMx,LastMy);
	if(rules.pchar()){	
		ShowString(smapx,ssy,rules.pchar(),&RedFont);
		ssy+=stDY;
	}
	/*extern int NCallsMakeDamage;
	sprintf(ggg,"NCallsMakeDamage=%d" ,NCallsMakeDamage);
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=20;*/
	int cxx=MRealX>>7;
	int cyy=MRealY>>7;

	cxx=MRealX>>6;
	cyy=MRealY>>6;
	sprintf(ggg,"TopZone:%d",GetTopRef(cxx+cyy*TopLx));
	CurZone=GetTopRef(cxx+cyy*TopLx);
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;
	sprintf(ggg,"Deep:%d",GetApproxDeep(MRealX,MRealY));
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;
	
	
	int u_test();
	sprintf(ggg,"FPS:%d" ,Flips);
	counter=0;
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;
	void PrintSoundInfo(int x,int y);
	PrintSoundInfo(smapx,ssy);
	ssy+=stDY;
	/*extern int TrueTime;
	sprintf(ggg,"TrueTime:%d" ,TrueTime);
	counter=0;
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;
	extern int TrueTime;
	extern int AnimTime;
	sprintf(ggg,"AnimTime:%d" ,AnimTime);
	counter=0;
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;*/
	int FMM_GetMemDiff();
	sprintf(ggg,"MemDiff:%d" ,FMM_GetMemDiff());	
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;

	//void CalcFreeAndUsed(int& freemem,int& used);
	//int fr,u;
	///CalcFreeAndUsed(fr,u);
	//sprintf(ggg,"Used & free:%dM & %dM" ,fr/1000000,u/1000000);	
	//ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	//ssy+=stDY;

	int FMM_GetFreeMem();
	sprintf(ggg,"AllocatedSize:%d" ,FMM_GetFreeMem());	
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;

	int FMM_GetBlocks();
	sprintf(ggg,"TotalMemSize:%d" ,FMM_GetBlocks()*4096);	
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;

	int FMM_GetAllocatedBlocks();
	sprintf(ggg,"AllocatedBlocks:%d" ,FMM_GetAllocatedBlocks());	
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;

	int FMM_GetNHandlers();
	sprintf(ggg,"Handlers:%d" ,FMM_GetNHandlers());	
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;

	//int FMM_GetMaxFreeBlock();
	//sprintf(ggg,"Max free block:%d" ,FMM_GetMaxFreeBlock());	
	//ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	//ssy+=stDY;

	sprintf(ggg,"POOL: [8:%dk] [16:%dk] [32:%dk] [64:%dk] [128:%dk]",POOL.pool_08.GetMemoryUsed()/1000,POOL.pool_16.GetMemoryUsed()/1000,POOL.pool_32.GetMemoryUsed()/1000,POOL.pool_64.GetMemoryUsed()/1000,POOL.pool_128.GetMemoryUsed()/1000);	
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;

	int texsz_managed=0;
	int texsz_default=0;
	int n_tex_managed=0;
	int n_tex_default=0;
	
	int N=IRS->GetNTextures();
	for(int i=0;i<N;i++){
        TextureMemoryPool t=IRS->GetTexturePool(i);
		if(t==mpManaged){
			n_tex_managed++;
			texsz_managed+=IRS->GetTextureSize(i);
		}else
		if(t==mpVRAM){
			n_tex_default++;
			texsz_default+=IRS->GetTextureSize(i);
		}
	}

	sprintf(ggg,"Textures: [managed: N=%d sz=%dm] [default: N=%d sz=%dm]",n_tex_managed,texsz_managed/1000/1000,n_tex_default,texsz_default/1000/1000);	
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;


    sprintf(ggg,"GameSpeed:%d" ,GameSpeed);
	counter=0;
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;
	extern int WaitTime;
	sprintf(ggg,"Waste:%d" ,WaitTime);
	ShowString(smapx,ssy,ggg,&SmallWhiteFont);
	ssy+=stDY;

	CEXPORT byte* GetBattleArmyValue();
	byte* AV=GetBattleArmyValue();


	int val=0;
	int exp=0;
	word NMask=NATIONS[MyNation].NMask;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo||OB->Hidden)){
			if(AV&&OB->NNUM==MyNation)val+=AV[OB->NIndex];
			if(!(OB->NMask&NMask))exp+=OB->newMons->Expa;
		}
		
	}
	if(val){
		char Buf[400];
		sprintf(Buf,"Units value: %d",val);
		ShowString(smapx,ssy,Buf,&SmallWhiteFont);
		ssy+=stDY;
	}
	if(exp){
		char Buf[400];
		sprintf(Buf,"Units expirience: %d",exp);
		ShowString(smapx,ssy,Buf,&SmallWhiteFont);
		ssy+=stDY;
	}

	word MID=0xFFFF;
	extern word Att;
	extern word Friend;
	if(Att!=0xFFFF)MID=Att;
	if(Friend!=0xFFFF)MID=Friend;
	if(MID==0xFFFF&&NSL[MyNation])MID=Selm[MyNation][0];
	if(MID!=0xFFFF){
		OneObject* OB=Group[MID];
		if(OB){
			ssy+=10;
			void ShowPen(int x,int y1,int R,DWORD ptc);
			ShowPen(OB->RealX/16,OB->RealY/16,16,0xFFFF0000);

			/*
			Vector3D p(OB->RealX/16,OB->RealY/16,OB->RZ);
			DrawSphere(Sphere(p,20),0x040000FF,0,30);
            DrawSphere(Sphere(p,20),0x600000FF,0,30);
			float GetVisibleSphereRadius(Vector3D Center,float Radius);
			float r=GetVisibleSphereRadius(p,20);
			WorldToScreenSpace(p);
			rsRect(p.x,p.y,r*2,0x40FFFFFF);
			rsFlush();
			*/


			sprintf(ggg,"%s : ID=%d  x=%d  y=%d dir=%d NMask=%d Unlimited=%d",OB->Ref.General->Message,OB->Index,OB->RealX>>4,OB->RealY>>4,OB->RealDir,OB->NMask,int(OB->UnlimitedMotion));
			ShowString(smapx,ssy,ggg,&SmallWhiteFont);
			ssy+=stDY;

			sprintf(ggg,"NIndex: %d",OB->NIndex);
			ShowString(smapx,ssy,ggg,&SmallWhiteFont);
			ssy+=stDY;

			char ordmes[1024];
			strcpy(ordmes,"Groups:");
			DetermineWhereUnitIs(MID,ordmes+strlen(ordmes));
			ShowString(smapx,ssy,ordmes,&SmallWhiteFont);
			ssy+=stDY;

			sprintf(ggg,"Brigade: %d  BrIndex: %d",OB->BrigadeID,OB->BrIndex);
			ShowString(smapx,ssy,ggg,&SmallWhiteFont);
			ssy+=stDY;
			if(OB->BrigadeID!=0xFFFF){
				Brigade* BR=&OB->Nat->CITY->Brigs[OB->BrigadeID];
				CBrigadeOrder* OR1=BR->NewBOrder;
				if(OR1){
					char OSTR[400]="BrigOrder:";
					char cc[200];
					int nn=0;
					//while(OR1&&strlen(OSTR)<320){
					sprintf(cc," [%s]",OR1->GetClassName());
					strcat(OSTR,cc);
					//	OR1=OR1->Next.Get();
					//	nn++;
					//}	
					ShowString(smapx,ssy,OSTR,&SmallWhiteFont);
					ssy+=stDY;
				}
			}
			if(OB->LocalOrder){
				Order1* OR1=OB->LocalOrder;
				char OSTR[400]="ORDER:";
				char cc[300];
				int nn=0;
				while(OR1&&strlen(OSTR)<320){
					int GetOrderKind(ReportFn* RF);
					int K=GetOrderKind(OR1->DoLink);
					char* GetOrdName(int idx);
					sprintf(cc," [%s]",GetOrdName(K));
					strcat(OSTR,cc);
					OR1=OR1->NextOrder;
					nn++;
				};
				if(OR1)strcat(OSTR,".....");
				ShowString(smapx,ssy,OSTR,&SmallWhiteFont);
				ssy+=stDY;
			};
			sprintf(ordmes,"NewState=%d LocalNewState=%d",OB->NewState,OB->LocalNewState);
			ShowString(smapx,ssy,ordmes,&SmallWhiteFont);
			ssy+=stDY;
			if(OB->NewAnm){
				int cd=OB->NewAnm->Code;
				Enumerator* E=ENUM.Get("MON_ANIMATIONS");
				if(E){
					char* s=E->Get(cd);
					if(s){
						sprintf(ordmes,"Animation: %s frame=%d",s,OB->CurrentFrameLong>>8);
						ShowString(smapx,ssy,ordmes,&SmallWhiteFont);
						ssy+=stDY;
					}
				}
			}
			sprintf(ordmes,"AbRes: %d",OB->AbRes);
			ShowString(smapx,ssy,ordmes,&SmallWhiteFont);
			ssy+=stDY;
			sprintf(ordmes,"NNUM: %d",OB->NNUM);
			ShowString(smapx,ssy,ordmes,&SmallWhiteFont);
			ssy+=stDY;
		};
	};

	void ShowAddInfo(int x,int y);
	ShowAddInfo(smapx,ssy);	
}; // GFieldShow


int MiniLx=0;
int MiniLy=0;
int MiniX,MiniY;
DWORD BMASK[MBR4]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int BCOLOR=0;

void ClearBMASK(){
	memset(BMASK,0,sizeof BMASK);
}

void DrawMiniFog();
void ViewRoads(int x,int y);
extern int NROADS;
void ShowSafetyInfo(int x,int y);
void DrawMinAZones(int x,int y);
void DrawMinAZonesVis(int x,int y);
void DrawAreas1(int xs,int ys,int x0,int y0,int Lx,int Ly);
void DrawForces(int xs,int ys,int x0,int y0,int Lx,int Ly);
extern int LastAttackX;
extern int LastAttackY;
extern int AlarmDelay;
void DrawMinAZonesVis(int x,int y);
void DRAW_MAP_TMP();
void DrawLastWayOnMinimap();
#ifdef _USE3D
void CopyTo16(int x,int y,byte* Src,int Pitch,int Lx,int Ly);
byte tempminimap[maxmap][maxmap];
extern int FMSX;
void OnMiniDraw(int x,int y,int Lx,int Ly,int mx0,int my0);
void DrawRoadsOnMinimap();
void DrawSelWayOnMinimap();
CIMPORT word GetTopSelBrigID();
void DrawTexturedBar(float xL,float yL,float W,float H,
					 float txL,float tyL,float tW,float tH,
					 DWORD CLU,DWORD CRU,DWORD CLD,DWORD CRD,
					 int TextureID,int ShaderID);
void DrawTexturedBar(float xL,float yL,float W,float H,
					 float txL,float tyL,float tW,float tH,
					 DWORD CLU,DWORD CRU,DWORD CLD,DWORD CRD,
					 int TextureID,int ShaderID,byte Rot);
int GetMinimapTexture();
int GetMinimapRoadsTexture();

UnitsListOnMinimap MuList;
#define FMSHI 3
WORD DWORD2WORD(DWORD C);

void SaveFogToBmp(char* name){
    int LX=263;
    int LY=263;
    BYTE* ptr=new BYTE[LX*LY*3];
    int N=LX*LY;
    for(int i=0,int i3=0;i<N;i++,i3+=3){
        int c=fmap[i];
        c>>=4;
        if(c>255)c=255;
        ptr[i3  ]=c;
        ptr[i3+1]=c;
        ptr[i3+2]=c;
    }
    SaveToBMP24(name,LX,LY,ptr);
    delete[]ptr;
}
void CreateFogTexImage(BYTE* DestPtr){    
    word F1=DWORD2WORD(EngSettings.MiniFogOfWarColor);
    F1|=0xF000;
    int sofs=int(DestPtr);
    int fofs=int(fmap)+((FMSHI+1+FMSX*FMSHI)<<1);
    int MMSX=(256);
    int MMSY=(256);
    int addscr=0;
    int F_add=(FMSX<<(ADDSH))-(MMSX<<ADDSH);
    int DDDX=1<<ADDSH;				
    word FA=F1&0xF000;
    word FC=F1&0xFFF;
    __asm{
        push	esi
        push	edi
        pushf
        mov		esi,fofs
        mov		edi,sofs
        mov		edx,256
        mov		ebx,DDDX
lopx1:
        mov		ecx,MMSX
lopx3:	mov     ax,[esi]
        add		esi,ebx
        sub		ax,512+512
        jbe		lopx21
        mov		word ptr [edi],0
        add		edi,2
        dec		ecx
        jnz		lopx3
lopx21:
        neg		ax					
        cmp     ax,512
        jae     lop3x1
        shr		ax,1
        mov		ah,al
        and		ax,0xF000
        cmp     ax,FA//0xB000
        jb      lop3x2
        mov     ax,FA//0xB000
lop3x2:	or      ax,FC//0x0555
        mov		word ptr [edi],ax
        jmp     lopx2
lop3x1:	mov		ax,F1
        mov		word ptr [edi],ax//0xB555
lopx2:	add		edi,2
        dec		ecx
        jnz		lopx3
        add		esi,F_add
        add		edi,addscr
        dec		MMSY
        jnz		lopx1
        popf
        pop		edi
        pop		esi
    }
}
void RenderMinimap(){
	static int MiniMapUTexture=0;
    static int MiniMapFTexture=0;
	if(MiniMapUTexture==0){
		MiniMapFTexture=IRS->CreateTexture("FogMinimapTexture",256,256,cfARGB4444,1,tmpManaged);
        MiniMapUTexture=IRS->CreateTexture("UnitsMinimapTexture",256,256,cfARGB4444,1,tmpManaged);
	}
    //IRS->SetTextureOverride( MiniMapFTexture,3 );
	IRS->SetTexture( MiniMapFTexture,3 );
    int Pitch=0;
    BYTE* MMR=IRS->LockTexBits(MiniMapFTexture,Pitch);
    if(MMR){
        if(FogMode&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision)&&BaloonState!=1){
            CreateFogTexImage(MMR);
        }else{
            memset(MMR,0,Pitch*256);
        }
        IRS->UnlockTexBits(MiniMapFTexture);
    }

	extern bool MiniMapVisible;	
	if( MiniMapVisible==false || !(GSets.CGame.ViewMask&2) )return;

	int TC=GetTickCount();
	static int T0=TC-10000;
	int tt=(GetTickCount()/300)&1;
	if(TC-T0>300){
		MuList.Clear();
		memset(BMASK,0,sizeof BMASK);
		T0=TC;
		int Pitch;
		BYTE* MMU=IRS->LockTexBits(MiniMapUTexture,Pitch);
		if(MMU){
			memset(MMU,0,256*Pitch);
			WORD DWORD2WORD(DWORD C);
			word C16  [8];
			word C16p [8];
			word C16m [8];
			word C16mm[8];
			DWORD GetNatColor(int i);
			for(int q=0;q<8;q++){
				DWORD C=GetNatColor(q);
				C16   [q]=DWORD2WORD(C);
				C16p  [q]=DWORD2WORD(MulDWORD(C,256+80));
				C16m  [q]=DWORD2WORD(MulDWORD(C,256-80));
				C16mm [q]=DWORD2WORD(MulDWORD(C,256-160));
			}
            //setting units
			OneObject* OO;
			word Seen[1024];
			int NSeen=0;
			byte MyMask=NATIONS[MyNation].NMask;
			int st=1;//(MAXOBJECT/3000)+1;
			byte mmsk=GM(MyNation);
			word F1=DWORD2WORD(EngSettings.MiniFogOfWarColor);
			F1|=0xF000;
			if(FogMode&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision)&&BaloonState!=1){
                CreateFogTexImage(MMU);				
			}
			for(int g=0;g<MAXOBJECT;g+=st){
				OO=Group[g];
				if(OO&&!(OO->Sdoxlo||OO->Invisible||OO->newMons->InvisibleOnMinimap||(OO->InvisibleForEnemy&&(!OO->NMask&MyMask)))){
					if(OO->TempFlag&&NSeen<1024){
						Seen[NSeen]=OO->Index;
						NSeen++;
					}					
					int mxx=(OO->RealX>>(9+ADDSH));
					int myy=(OO->RealY>>(9+ADDSH));//-(OO->RZ>>6);
					if(mxx>=2&&myy>=2&&mxx<240&&myy<240){
						word val=C16[OO->NNUM];
						if(OO->BrigadeID==0xFFFF||OO->BrIndex>=NBPERSONAL){
							if(OO->NewAnm->AnimationType==1 && !OO->NewBuilding){
								word& v=*(((word*)MMU)+mxx+(myy<<8));
								if((v>>12)<5){				
									MuList.AddUnit(OO);
								}
							}else{
								if(OO->ImSelected&mmsk){
									val=0xFFF0;
									if(OO->BrigadeID!=0xFFFF){
										Brigade* BR=OO->Nat->CITY->Brigs+OO->BrigadeID;
										if(BR->WarType){
											BCOLOR=OO->NNUM;
											int pp=BR->ID;
											int idx=pp>>5;
											BMASK[idx]|=(1<<(pp&31));
										}
									}
									word& v=*(((word*)MMU)+mxx+(myy<<8));
									if((v>>12)<5){									
										if(OO->NewBuilding){
											(&v)[-1  ]=val;
											(&v)[1   ]=val;
											(&v)[-256]=val;
											(&v)[256 ]=val;
											(&v)[-257]=val;
											(&v)[-255]=val;
											(&v)[257 ]=val;
											(&v)[255 ]=val;										
										}else v=val;
									}
								}else{
									word& v=*(((word*)MMU)+mxx+(myy<<8));
									if(v!=0xFFFF&&(v>>12)<6){
										if(OO->NewBuilding){
											if(val==0xFFF0){
												(&v)[-1  ]=val;
												(&v)[1   ]=val;
												(&v)[-256]=val;
												(&v)[256 ]=val;
												(&v)[-257]=val;
												(&v)[-255]=val;
												(&v)[257 ]=val;
												(&v)[255 ]=val;
												v=0xFFFF;
											}else{											
												int n=OO->NNUM;
												(&v)[-1  ]=C16m[n];
												(&v)[1   ]=C16p[n];
												(&v)[-256]=C16m[n];
												(&v)[256 ]=C16p[n];
												(&v)[-257]=C16mm[n];
												(&v)[-255]=C16mm[n];
												(&v)[257 ]=C16m[n];
												(&v)[255 ]=C16mm[n];
												v=0xFFFF;
											}
										}else v=val;
									}
								}
							}
						}
					}
				}
			}
			//fog of war
			
			for(int i=0;i<NSeen;i++){
				OneObject* OO=Group[Seen[i]];
				if(OO&&!OO->Sdoxlo){			
					int mxx=(OO->RealX>>(9+ADDSH));
					int myy=(OO->RealY>>(9+ADDSH));//-(OO->RZ>>6);
					if(mxx>0&&myy>0&&mxx<240&&myy<240){
						word val=C16[OO->NNUM];						
						if(OO->ImSelected&mmsk){
							val=0xFFF0;
							if(OO->BrigadeID!=0xFFFF){
								Brigade* BR=OO->Nat->CITY->Brigs+OO->BrigadeID;
								if(BR->WarType){
									BCOLOR=OO->NNUM;
									int pp=BR->ID;
									int idx=pp>>5;
									BMASK[idx]|=(1<<(pp&31));
								}
							}
						}
						word& v=*(((word*)MMU)+mxx+(myy<<8));
						if(OO->NewBuilding){
							(&v)[-1  ]=val;
							(&v)[1   ]=val;
							(&v)[-256]=val;
							(&v)[256 ]=val;
							(&v)[-257]=val;
							(&v)[-255]=val;
							(&v)[257 ]=val;
							(&v)[255 ]=val;										
							v=0xFFFF;
						}else v=val;
					}
				}
			}
			IRS->UnlockTexBits(MiniMapUTexture);
			MuList.FixDistanceBetweenPoints();
		}
	}
	static int sh=IRS->GetShaderID("hud");
//	extern int MinMapX;
//	extern int MaxMapX;
//	extern int MinMapY;
//	extern int MaxMapY;
	float sc=float(256<<ADDSH);
	static int shs=IRS->GetShaderID("hud_minimap");
	extern int ExtraDir;
	const RECT rc = MOptions.LimitCamArgs.GetRect();
	const float X0 = (float)rc.left;
	const float X1 = (float)rc.right;
	const float Y0 = (float)rc.top;
	const float Y1 = (float)rc.bottom;
	const float wRc = X1 - X0;
	const float hRc = Y1 - Y0;
	DrawTexturedBar(minix,miniy,MiniLx,MiniLy,X0/sc,Y0/sc,wRc/sc,hRc/sc,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,GetMinimapTexture(),shs,-ExtraDir);
	DrawTexturedBar(minix,miniy,MiniLx,MiniLy,X0/sc,Y0/sc,wRc/sc,hRc/sc,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,MiniMapUTexture,shs,-ExtraDir);
	MuList.DrawPoints();
	if(!EngSettings.DontUseRoads) {
		DrawTexturedBar(minix,miniy,MiniLx,MiniLy,X0/sc,Y0/sc,wRc/sc,hRc/sc,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,GetMinimapRoadsTexture(),shs,-ExtraDir);
	}
}

extern int LastSignalX;
extern int LastSignalY;
extern int SignalDelay;
extern int LastAttackDelay;
extern short AlarmSoundID;
void AddAlarmSignal(int x,int y){
	LastAttackDelay=400*256;
	if(AlarmSoundID!=-1)AddEffect((mapx<<5)+500,(mapy<<4)+300,AlarmSoundID);
	LastActionX=x<<4;
	LastActionY=y<<4;
	LastAttackX=x<<4;
	LastAttackY=y<<4;
	AlarmDelay=60*256;
}
void TryToAddAlarm(int x,int y){
	if(!LastAttackDelay){
		Vector3D V(x,y,GetTotalHeight(x,y));
		WorldToScreenSpace(V);
		if(V.x<-20 || V.y<-20 || V.x>RealLx+20 || V.y>RealLy+20){
			AddAlarmSignal(x,y);
		}
	}
}
void GMiniShow16(){		
	
	int MAXMP=120;	
	if(FullMini){
		MAXMP=500;
	};
	if(!MiniLx)MiniLx=MAXMP;
	if(!MiniLy)MiniLy=MAXMP;
	if(!MiniMade)CreateMiniMap();
	/*int MaxMLX=msx>>ADDSH;
	int MaxMLY=msy>>ADDSH;
	MiniLx=MaxMLX;
	MiniLy=MaxMLY;
	if(MiniLx>MAXMP)MiniLx=MAXMP;
	if(MiniLy>MAXMP)MiniLy=MAXMP;
	extern int MinMapX;
	extern int MaxMapX;
	extern int MinMapY;
	extern int MaxMapY;
	if(MiniLx>MaxMapX-MinMapX)MiniLx=MaxMapX-MinMapX;
	if(MiniLy>MaxMapY-MinMapY)MiniLy=MaxMapY-MinMapY;
	MiniX=(mapx+(smaplx>>1)-MiniLx)>>ADDSH;
	MiniY=(mapy+(smaply>>1)-MiniLy)>>ADDSH;
	if(MiniX<0)MiniX=0;
	if(MiniY<0)MiniY=0;
	if(MiniX+MiniLx>=MaxMLX)MiniX=MaxMLX-MiniLx-1;
	if(MiniY+MiniLy>=MaxMLY)MiniY=MaxMLY-MiniLy-1;*/
	//minix=RealLx-MiniLx-20;//-17;
	//miniy=RealLy-MiniLy-20;//-16;
	
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	word TBr=0xFFFF;//GetTopSelBrigID();

	OneObject* OO;
	int mxx,myy;
	byte val;
	byte mycl=CLRT[MyNation];	
	byte mmsk=GM(MyNation);
	int st=(MAXOBJECT/3000)+1;
	word Seen[1024];
	int NSeen=0;
	byte MyMask=NATIONS[MyNation].NMask;
	
	RenderMinimap();

	extern bool MiniMapVisible;	
	if(GSets.DisableMinimap||!MiniMapVisible)return;

    if(!(GSets.CGame.ViewMask&2))return;

	if(EditMapMode){
		int x=mapPos[(MyNation<<1)]>>1;
		int y=mapPos[(MyNation<<1)+1]>>1;
		//SetRLCWindow(minix,miniy,MiniLx,MiniLy,SCRSizeX);
		Hline(minix+x-2-MiniX,miniy+y-MiniY,minix+x-2+4-MiniX,0xFFFFFFFF);
		Vline(minix+x-MiniX,miniy+y-2-MiniY,miniy+y-2+4-MiniX,0xFFFFFFFF);
		ShowMiniDestn();
		if(GetKeyState(VK_CONTROL)&0x8000)DrawMinAZones(minix-MiniX,miniy-MiniY);
		DrawMinAZonesVis(minix-MiniX,miniy-MiniY);
		//SetRLCWindow(0,0,COPYSizeX,RSCRSizeY,SCRSizeX);
	}else{
		SetRLCWindow(minix,miniy,MiniLx,MiniLy,SCRSizeX);
		ShowMiniDestn();
		SetRLCWindow(0,0,RealLx,RealLx,SCRSizeX);
		//if(GetKeyState(VK_CONTROL)&0x8000)
			DrawMinAZonesVis(minix-MiniX,miniy-MiniY);
	};	
	extern DWORD GU_COLOR;
	GU_COLOR=0xFFFFFFFF;
	///CopyTo16(minix,miniy,((byte*)&tempminimap)+MiniX+MiniY*256,maxmap,MiniLx,MiniLy);
	int GetMinimapTexture();
	int tex=GetMinimapTexture();

	GU_COLOR=0xFFFFFFFF;	
	void DrawAlarms();
	DrawAlarms();
	if(CurAINF_Show!=-1&&CurAINF_Show<7){
		GNFO.EINF[CurAINF_Show]->GAINF.Show(minix,miniy);
	};
	if(CurAINF_Show==7){
		DrawLastWayOnMinimap();
	};

	//DrawRoadsOnMinimap();
	DrawSelWayOnMinimap();
	VictoryConditionRoot* VCond=MOptions.VictoryRule.Get();
	if(VCond){
		VCond->DrawOnMiniMap(minix,miniy);
	}
	HandleGeology();
	OnMiniDraw(minix,miniy,MiniLx,MiniLy,MiniX,MiniY);
	ext_OnDrawOnMiniMap(minix-MiniX,miniy-MiniY,MiniLx,MiniLy);

	Rct miniRct( minix, miniy, MiniLx, MiniLy );
	DrawMinimapFrustumProjection( miniRct );	

	if(SignalDelay>0&&LastSignalX!=-1&&MiniLx){
		extern DWORD SignalColor;
		SetRLCWindow(minix,miniy,MiniLx,MiniLy,SCRSizeX);
		int x0=GetXOnMiniMap(LastSignalX>>4,LastSignalY>>4);
		int y0=GetYOnMiniMap(LastSignalX>>4,LastSignalY>>4);
		int R=((60*256-SignalDelay)%5120)/200;
		DWORD C=SignalColor;
		GPS.DrawRect(x0-R,y0-R,R+R,R+R,C);
		GPS.DrawRect(x0-R-1,y0-R-1,R+R+2,R+R+2,C);
		GPS.DrawRect(x0-R-2,y0-R-2,R+R+4,R+R+4,C);
		SignalDelay-=GameSpeed/3;
		SetRLCWindow(0,0,COPYSizeX,RSCRSizeY,SCRSizeX);
	};
	
	int mx2=MiniLx;
	int my2=MiniLy;
	CEXPORT DWORD GetNatColor( int natIdx );
	DWORD ccc=GetNatColor(GSets.CGame.cgi_NatRefTBL[MyNation]);
	Hline(minix-1,miniy-1,minix+mx2,ccc);
	Hline(minix-1,miniy+my2,minix+mx2,ccc);
	Vline(minix-1,miniy-1,miniy+my2,ccc);
	Vline(minix+mx2,miniy,miniy+my2,ccc);
	Hline(minix-2,miniy-2,minix+mx2+1,ccc+1);
	Hline(minix-2,miniy+my2+1,minix+mx2+2,ccc+1);
	Vline(minix-2,miniy-2,miniy+my2+1,ccc+1);
	Vline(minix+mx2+1,miniy-1,miniy+my2+1,ccc+1);
	//DrawStdRect(minix-2,miniy-2,minix+mx2+2,miniy+my2+2);
	//GPS.FlushBatches();
	return;
};
#endif //_USE3D
void GMiniShow(){	
	//Inform=GetKeyState(VK_MENU);
	extern bool HideBorderMode;
	if(NoText||PlayGameMode==1||HideBorderMode)return;
#ifdef _USE3D
	GMiniShow16();
	return;
#endif //_USE3D	
};
#define step 1
float StepX=0;
float StepY=0;
void SetEgoFlag(bool Ego){
	
};
extern int  ReliefBrush;
extern int  TerrBrush;
bool SelSoundDone;
extern int SelSoundType;
extern bool SelSoundReady;
void DoSelSound(int type){
	if(!SelSoundDone){
		SelSoundDone=true;
		SelSoundReady=true;
		SelSoundType=type;
	};
};
extern bool Fixed;
extern bool realLpressed;
extern bool realRpressed;
void MFix(){
	Fixed=true;
	if(Lpressed)Lpressed=realLpressed;
	if(Rpressed)Rpressed=realRpressed;
};
void CmdCreateNewTerr(byte NI,int x,int y,word ID);
word GetNewEnemy(int xr,int yr,byte NI);
void ShowPen(int x,int y1,int R,DWORD ptc);
void AddHi(int x,int y1,int r,int h);
void AverageHi(int x,int y1,int r);
void PlanarHi(int x,int y1,int r);
void delTrees(int x,int y,int r);
void addTrees(int x,int y,int r);
extern int HiStyle;
int Prop43(int y);
bool GetRealCoords(int xs,int ys,int* xr,int* yr){
	Vector3D V=ScreenToWorldSpace(xs,ys);
	*xr=V.x;
	*yr=V.y;
	return true;	
};
void RandomHi(int x,int y1,int r,int ms,int fnd);
void AddHiPlanar(int x,int y1,int r,int h);
void CreatePlane(int x,int y1,int r);
void SetSpot(int x,int y);
word GetNewFriend(int xr,int yr,byte NI);
bool LockMouse=false;
bool WasSelectionBar=false;
void CmdSelectBuildings(byte NI);
int LastMx;
int LastMy;
void CHKS(){
	//assert(StartX<100000);
};
bool StrelMode=false;
void ProcessRM_Load(int x,int y);
void ProcessRM_Save(int x0,int y0,int x1,int y1);
void ProcessTexPiece(int x,int y,bool press);
CEXPORT
int ScrollSpeed=5;
int LastClickTime=0;
bool LastRealLP=false;
int LastClcX,LastClcY;
extern int SHIFT_VAL;
extern bool SetDestMode;
extern char* FormationStr;
void ControlAZones(int x,int y);
void GoToCurSelPosition();
void CmdSelectIdlePeasants(byte);
void CmdSelectIdleMines(byte);
void ProcessInformation(bool Allow);
int InfAddY=16+20;
int InfDY=32+InfAddY;
int InfDX=10+243+6;
int InfY1=450+InfAddY;
int InfY2=450-22+InfAddY;
void CmdPause(byte NI);
word AdvancedGetNewEnemy(int rex,int rey,int xr,int yr,byte NI);
word AdvancedGetNewFriend(int rex,int rey,int xr,int yr,byte NI);
bool MouseOverMiniMenu=0;
int TestCapture(OneObject* OBJ);
void CmdSelAllUnits(byte NI);
void CmdSelAllShips(byte NI);
void CmdSelAllFlyObjects(byte NI);
void CmdSelAllShooters(byte NI);
void CmdSelAllPeasants(byte NI);
void CmdSelAllWarriors(byte NI);
void HandleMouse(int x,int y);
void EDGETEST();
void UnPress();
int OldLpressed=-1;
bool OverInform=0;
bool unpress=0;
void GlobalHandleMouse();
void DrawZones();
void GameKeyCheck();
void DRAW_MAP_TMP();
void ProcessScreen(){
	if(!bActive)return;
	//IRS->ClearDevice( 0, false, true);
	GameKeyCheck();
	NoPFOG=1;
	PRFUNC(GFieldShow());
	NoPFOG=0;
	GMiniShow();
	ShowProp();
	ShowAbility();
	DrawZones();
	GlobalHandleMouse();
	MFix();
	DRAW_MAP_TMP();
};
void ProcessScreenWithoutMouse(){
    if(!bActive)return;
    IRS->ClearDevice( 0, false, true);
    GameKeyCheck();
    NoPFOG=1;
    PRFUNC(GFieldShow());
    NoPFOG=0;
    GMiniShow();
    ShowProp();
    ShowAbility();
    DrawZones();
    //GlobalHandleMouse();
    MFix();
    DRAW_MAP_TMP();
};
void HandleSMSChat(char* Mess);
void HandlwSMSMouse();
void ProcessRESSEND();
void ProcessMapInterface();
extern bool DRAWLOCK;
void ClearModes();
extern bool MultiTvar;
extern bool OptHidden;
bool CheckFlagsNeed();
void CmdVote(byte result);
void ProcessDIP_DSS();
bool AutoSpeed=1;
void CmdSetSpeed(word);
extern int ExitNI;

extern veGameMode vGameMode;

void GlobalHandleMouse(){
	if(v_MainMenu.vDS){
		return;
	}	
	ACTION_HandlingControls();
	if(ExitNI!=-1){
		CmdEndGame(ExitNI,1,0);
		ExitNI=-1;
	};
	if(SpecCmd==235){
		SpecCmd=0;
		///void PaintMapWith();
		///PaintMapWith();
		void UserFriendlyMapGenerator();
		UserFriendlyMapGenerator();
	}
	if(SpecCmd==199){
		CmdEndGame(MyNation,1,115);
		SpecCmd=0;
	};
	if(SpecCmd==171){
		CmdVote(1);
		SpecCmd=0;
	};
	if(SpecCmd==172){
		CmdVote(2);
		SpecCmd=0;
	};
	void CmdMakeOneStep(byte NI,char Dir);

	if(SpecCmd==173){
		CmdMakeOneStep(MyNation,-WheelDelta/120);
		SpecCmd=0;
	};
	if(SpecCmd==174){
		CmdMakeOneStep(MyNation,-WheelDelta/120);
		SpecCmd=0;
	};
	void CmdRotateFormations(byte NI,byte Dir);
	if(SpecCmd==187){
		CmdRotateFormations(MyNation,128+32);
		SpecCmd=0;
	};
	if(SpecCmd==188){
		CmdRotateFormations(MyNation,128+64);
		SpecCmd=0;
	};
	if(SpecCmd==189){
		CmdRotateFormations(MyNation,128+64+32);
		SpecCmd=0;
	};
	if(SpecCmd==184){
		CmdRotateFormations(MyNation,128);
		SpecCmd=0;
	};
	if(SpecCmd==186){
		CmdRotateFormations(MyNation,0);
		SpecCmd=0;
	};
	if(SpecCmd==181){
		CmdRotateFormations(MyNation,64+32);
		SpecCmd=0;
	};
	if(SpecCmd==182){
		CmdRotateFormations(MyNation,64);
		SpecCmd=0;
	};
	if(SpecCmd==183){
		CmdRotateFormations(MyNation,32);
		SpecCmd=0;
	};
	DRAWLOCK=0;
	ProcessInformation(0);
	ProcessDIP_DSS();
	ProcessRESSEND();	
	ShowRMap();	
	if(NPlayers>1&&vGameMode!=gmSin){
		if(CheckFlagsNeed()){
			if(Inform)OptHidden=1;
			ProcessMapInterface();
		};
	}else{
		if(SHOWSLIDE&&(EditMapMode||MultiTvar||PlayGameMode==2)){
			if(Inform)OptHidden=1;
			ProcessMapInterface();
		};
	};
	if(DRAWLOCK){		
		ProcessTexPiece((mapx<<5)+(RealLx>>1),(mapy<<5)+RealLy,0);
		//ShowRMap();
		return;
	};
	OldLpressed=-1;
	unpress=0;
	OverInform=0;
	if(Inform&&!NoPress){
		if(mouseX>InfDY-3&&mouseY<InfY1+8&&mouseX>RealLx-InfDX-3){
			OldLpressed=Lpressed;
			Lpressed=false;
			OverInform=1;
			unpress=1;
			UnPress();
		};
	};
	MouseStack* MEV=ReadMEvent();
	if(MEV){
		int x0=mouseX;
		int y0=mouseY;
		bool lp=Lpressed;
		bool rp=Rpressed;
		bool rlp=realLpressed;
		bool rrp=realRpressed;
		bool Ctrl=(GetKeyState(VK_CONTROL)&0x8000)!=0;
		bool Shift=(GetKeyState(VK_SHIFT)&0x8000)!=0;
		int x,y;
        bool once=true;
		do{
			x=MEV->x;
			y=MEV->y;
			Lpressed=MEV->Lpressed;
			Rpressed=MEV->Rpressed;
			realLpressed=MEV->rLpressed;
			realRpressed=MEV->rRpressed;
			if(unpress){
				UnPress();
				Lpressed=0;
				Rpressed=0;
			};
			bool ll=Lpressed;
			bool rr=Rpressed;
			if(ll&&!Lpressed)unpress=1;
			if(rr&&!Rpressed)unpress=1;
			if(unpress&&SpecCmd==241)SpecCmd=0;
            if(once){
                ext_OnShowInGameDialog();
                once=false;
            }
			HandleMouse(x,y);
			MEV=ReadMEvent();
		}while(MEV);
		Lpressed=lp;
		Rpressed=rp;
		if(unpress){
			Lpressed=0;
			Rpressed=0;
			if(SpecCmd==241)SpecCmd=0;
		};
		realLpressed=rlp;
		realRpressed=rrp;
	}else{
        ext_OnShowInGameDialog();
		bool ll=Lpressed;
		bool rr=Rpressed;
		if(ll&&!Lpressed)unpress=1;
		if(rr&&!Rpressed)unpress=1;
		if(unpress){
			Lpressed=0;
			Rpressed=0;
			if(SpecCmd==241)SpecCmd=0;
		};
		HandleMouse(mouseX,mouseY);
	};
	EDGETEST();
	if(OldLpressed!=-1)Lpressed=OldLpressed!=0;
	extern bool IgnoreProcessMessages;
	ISM->Flush();
	rsFlush();
	//IgnoreProcessMessages=0;
};
extern char** NatNames;
extern bool RESMODE;
bool GUARDMODE=0;
bool PATROLMODE=0;
extern bool AttGrMode;

DIALOGS_API int VitCursor=-1;
DIALOGS_API int VitCursorLowProirity=-1;
DIALOGS_API bool NullCursor=false;

int AttackPtr=1;
int CapturePtr=2;
int RepairPtr=3;
int GoToMinePtr=4;

int GatherStonePtr=5;

int SetDestPtr=8;
int PatrolPtr=8;
int AttGrPtr=9;
int GuardPtr1=10;
int EnterTrPtr=11;
int EnterBuildingPtr=11;
int GuardPtr2=12;
int NullCursorPtr=15;
int StormEnemyPtr=15;//8;

bool CheckTransportEntering(OneObject* OB);
void CmdSetGuardState(byte,word);
extern char HEIGHTSTR[12];
bool CheckCurveLinked(int x,int y);
void CmdAttackGround(byte NI,int x,int y);

byte CANTAKEMASK[16];
byte CANTAKECURSOR[16]={6,5,5,7, 5,5,0,0, 1,0,0,0, 0,0,0,0};

extern int DrawPixMode;
extern bool WT_IS_INIT;

void SetPixSpot(int x,int y,int R,int h0,int h1);
int prevdrx=0;
int prevdry=0;
void SetPixLine(int x0,int y0,int x1,int y1,int R,int h0,int h1);
extern bool PenBackSide;
extern int PEN_RADIUS;
extern int PEN_BRIGHTNESS;
extern int CurDrMod;
void ProcessTablet();
void SetPixLine(int x0,int y0,int x1,int y1,int R,int R1,int h0,int h1,int h01);
int PrevPress=0;
void DrawHole(int x,int y);
extern int DrawGroundMode;
void SetGroundTexRound(int TxIdx,int x0,int y0,int r,int dr);
void ClearGroundTexRound(int x0,int y0,int r,int dr);
extern int CurGroundTexture;
extern int TexStDX;
extern int TexStDY;
DWORD YELLOW_COLOR=0xFFFFFF00;
DWORD BIRUZOVIJ=0xFF00FFFF;
word GetDoorsOfEnemy(int rex,int rey,int xr,int yr,byte NI);
char EXCOMM[1024];
bool HaveExComm=0;
void CmdAddMoney(byte,DWORD);
CEXPORT
void SetExComm(char* Data,int size){
	if(size<1024){
		memcpy(EXCOMM,Data,size);
		HaveExComm=1;
	};
};

//int NPOrd;
//PositionOrder POrd[32];

bool GetBrCenter(Brigade* BR, int* x, int* y);
void DrawPlaneLine(int x0,int y0,int x1,int y1,byte c);

void ShowBrigPosition(){
#ifndef COSSACKS2
	return;
#endif

}

CIMPORT void CheckDipBuilding(byte NI,int Index);
CEXPORT
void SetCurPtr(int v);
extern byte* DIP_Data[64];
extern int DIP_DSize[64];
extern int NDipItems;
void CmdSendData(byte* Data,int size);
void ProcessRoadsEdition(int xc,int yc);
void ShiftBuilding(OneObject* OB,int dx,int dy){
	if(OB->NewBuilding){
		NewMonster* NM=OB->newMons;
		int xx,yy;
		OB->GetCornerXY(&xx,&yy);
		if(OB->Stage<OB->MoreCharacter->ProduceStages&&NM->NBLockPt){
			for(int i=0;i<NM->NBLockPt;i++){
				BClrPt(xx+NM->BLockX[i],yy+NM->BLockY[i]);
			};
		}else{
			for(int i=0;i<NM->NLockPt;i++){
				BClrPt(xx+NM->LockX[i],yy+NM->LockY[i]);
			};
		};
		OB->RealX+=dx;
		OB->RealY+=dy;
		addrand(OB->RealX);
		addrand(OB->RealY);
		OB->RZ=GetHeight(OB->RealX>>4,OB->RealY>>4);
		OB->GetCornerXY(&xx,&yy);
		if(OB->Stage<OB->MoreCharacter->ProduceStages&&NM->NBLockPt){
			for(int i=0;i<NM->NBLockPt;i++){
				BSetPt(xx+NM->BLockX[i],yy+NM->BLockY[i]);
			};
		}else{
			for(int i=0;i<NM->NLockPt;i++){
				BSetPt(xx+NM->LockX[i],yy+NM->LockY[i]);
			};
		};
	};
};
word Att=0xFFFF;
word Friend=0xFFFF;
bool NoMoveMode=0;
bool LockManualScroll=0;
bool MouseOverMiniMap;
extern int PREVNSL;
extern int PREVNSL1;
#define CHNSL assert((NSL[1]==PREVNSL1&&NSL[0]==PREVNSL)||PREVNSL==-1);
void ShowSqPen(int x,int y1,int R,DWORD ptc);
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);

//extern MonsterAbility* v_Targ_MA;
extern UnitActiveAbilityList* v_Targ_MA;
extern int v_Targ_MA_id;
extern int v_Targ_MA_id2;
//int MinMapX=0;
//int MaxMapX=0;
//int MinMapY=0;
//int MaxMapY=0;
extern float fMapX;
extern float fMapY;
void ShiftCamera(float dx,float dy);
void SetCameraPos(float x,float y);

int MinScrollX;
int MinScrollY;
int MaxScrollX;
int MaxScrollY;

void SetInitialScrollLimitations() {
	// Initial values:
//	MinMapX = MOptions.LimitCamArgs.GetMinMapX();
//	MinMapY = MOptions.LimitCamArgs.GetMinMapY();
//	MaxMapX = MOptions.LimitCamArgs.GetMaxMapX();
//	MaxMapY = MOptions.LimitCamArgs.GetMaxMapY();
	
	MinScrollX = 6;
    MinScrollY = 12;
    MaxScrollX = (240<<ADDSH)-6;
    MaxScrollY = (240<<ADDSH)+1;
}
void LimitCamera() {
	if(g_SurfEditor.FreeCamera) {
		return; // There is no sense in limiting FreeCamera...
	}

	const cBounds rc = MOptions.LimitCamArgs.GetRectFixed();
	fMapX = cMath::Clamp(fMapX, rc.GetMin().x, rc.GetMax().x);
	fMapY = cMath::Clamp(fMapY, rc.GetMin().y, rc.GetMax().y);


	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*	float Mx, My;
	if(GetKeyState(VK_SHIFT) < 0) {
		POINT pt;
		GetCursorPos(&pt);
		const Line3D Pick = ICam->GetPickRay(pt.x, pt.y);
		cVec3 Orig, Dir;
		Orig.Copy(&Pick.getOrig().x);
		Dir.Copy(&Pick.getDir().x);
		cPlane P(cVec3::zAxis, 0.0f);
		cVec3 Cross;
		P.RayIntersection(Orig, Dir, NULL, &Cross);
		Mx = Cross.x / c_WorldGridStepX;
		My = Cross.y / c_WorldGridStepY;
	}*/
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//	if(!MOptions.LimitCamArgs.UseFromMap) {
//		MOptions.LimitCamArgs.SetFromEngineSettings();
//	}
	
/*	static int PrevMOptionsMinMapX = 0;
	static int PrevMOptionsMinMapY = 0;
	static int PrevMOptionsMaxMapX = 0;
	static int PrevMOptionsMaxMapY = 0;

	if(PrevMOptionsMinMapX != MOptions.LimitCamArgs.GetMinMapX() ||
		PrevMOptionsMinMapY != MOptions.LimitCamArgs.GetMinMapY() ||
		PrevMOptionsMaxMapX != MOptions.LimitCamArgs.GetMaxMapX() ||
		PrevMOptionsMaxMapY != MOptions.LimitCamArgs.GetMaxMapY()) {
			SetInitialScrollLimitations();
			PrevEngSettingsMinMapX = EngSettings.LimitCamArgs.GetMinMapX();
			PrevEngSettingsMinMapY = EngSettings.LimitCamArgs.GetMinMapY();
			PrevEngSettingsMaxMapX = EngSettings.LimitCamArgs.GetMaxMapX();
			PrevEngSettingsMaxMapY = EngSettings.LimitCamArgs.GetMaxMapY();
		}*/

/*	static int LastShiftCalcXRes = 0, LastShiftCalcYRes = 0;
	static int LastShiftCalcMinMapX = 0, LastShiftCalcMinMapY = 0;
	static int LastShiftCalcMaxMapX = 0, LastShiftCalcMaxMapY = 0;
	if(LastShiftCalcXRes != RealLx || LastShiftCalcYRes != RealLy ||
		LastShiftCalcMinMapX != MOptions.LimitCamArgs.GetMinMapX() ||
		LastShiftCalcMinMapY != MOptions.LimitCamArgs.GetMinMapY() ||
		LastShiftCalcMaxMapX != MOptions.LimitCamArgs.GetMaxMapX() ||
		LastShiftCalcMaxMapY != MOptions.LimitCamArgs.GetMaxMapY()) {
			CalcAuxMapXYShifts();
			LastShiftCalcXRes = RealLx;
			LastShiftCalcYRes = RealLy;
			LastShiftCalcMinMapX = MOptions.LimitCamArgs.GetMinMapX();
			LastShiftCalcMinMapY = MOptions.LimitCamArgs.GetMinMapY();
			LastShiftCalcMaxMapX = MOptions.LimitCamArgs.GetMaxMapX();
			LastShiftCalcMaxMapY = MOptions.LimitCamArgs.GetMaxMapY();
		}*/
}

void AddLockRound(word x,word y,int R);
void AddUnLockRound(word x,word y,int R);
void AddDefaultRound(word x,word y,int R);
void SetScrollLimitations(int x0,int y0,int x1,int y1){
/*	MinMapX=x0>>5;
	MinMapY=y0>>5;
	MaxMapX=x1>>5;
	MaxMapY=y1>>5;	
	if(MinMapX<1)MinMapX=1;
	if(MinMapY<1)MinMapY=1;
	if(MaxMapX>msx)MaxMapX=msx;
	if(MaxMapY>msy)MaxMapY=msy;*/
	MOptions.LimitCamArgs.UseFromMap = true;
	MOptions.LimitCamArgs.MinMapX = x0 / c_WorldGridStepX;
	MOptions.LimitCamArgs.MinMapY = y0 / c_WorldGridStepY;
	MOptions.LimitCamArgs.MaxMapX = x1 / c_WorldGridStepX;
	MOptions.LimitCamArgs.MaxMapY = y1 / c_WorldGridStepY;

	void	g_UpdateAlertXY();
	g_UpdateAlertXY();
}
void SetScrollLimitationsForNation(int x0,int y0,int x1,int y1, int nat)
{
	if (nat == NationID)
	{
		MOptions.LimitCamArgs.UseFromMap = true;
		MOptions.LimitCamArgs.MinMapX = x0 / c_WorldGridStepX;
		MOptions.LimitCamArgs.MinMapY = y0 / c_WorldGridStepY;
		MOptions.LimitCamArgs.MaxMapX = x1 / c_WorldGridStepX;
		MOptions.LimitCamArgs.MaxMapY = y1 / c_WorldGridStepY;

		void g_UpdateAlertXY();
		g_UpdateAlertXY();
	}
}
bool CheckScrollLimitations(int x, int y){
	x>>=5;
	y>>=5;
//	if( x>=MinMapX && y>=MinMapY && x<=MaxMapX && y<=MaxMapY){
	const RECT rc = MOptions.LimitCamArgs.GetRect();
	if(x >= rc.left && y >= rc.top && x <= rc.right && y <= rc.bottom) {
		return false;
	}
    return true;
}
#define StrelDist 60
void HandleMouse(int x,int y)
{	
	MouseOverMiniMap=0;
	static int pmx=0;
	static int pmy=0;
	static int PrevMouseMoveTime=0;
	if(!(GSets.CGame.ViewMask&4))return;
	if(PlayGameMode||x!=pmx||y!=pmy||!PrevMouseMoveTime){
		PrevMouseMoveTime=GetTickCount();
		pmx=x;
		pmy=y;
	}
	if(GSets.LeibzigDemo.Enabled&&(!PlayGameMode)&&GetTickCount()-PrevMouseMoveTime>GSets.LeibzigDemo.StayTime){
		GameExit=1;
	}
	if(NoMoveMode)return;
	if(NDipItems){
		for(int i=0;i<NDipItems;i++){
			CmdSendData(DIP_Data[i],DIP_DSize[i]);
			free(DIP_Data[i]);
		};
		NDipItems=0;
	};
	bool TAKERES=1;
	bool SpotVis=1;
	int stptr=-1;
	if(HaveExComm){
		switch(EXCOMM[0]){
		case 0://chat message
			AssignHint1(EXCOMM+1,511,32);
			break;
		case 1://money bonus
			CmdAddMoney(MyNation,*((DWORD*)(EXCOMM+1)));
			break;
		};
		HaveExComm=0;
		memset(EXCOMM,0,sizeof EXCOMM);
	};
	PrevPress=CurPressure;
	ProcessTablet();
	HandlwSMSMouse();
	memset(CANTAKEMASK,0,16);

	//cheat!!!
	if( (GetKeyState('X')&0x8000) 
		&& (GetKeyState('E')&0x8000) 
		&& (GetKeyState('N')&0x8000) 
		&& (GetKeyState('O')&0x8000) 
		&& (GetKeyState(VK_SHIFT)&0x8000) ){
			void CmdMoney(byte NI);
			CmdMoney(MyNation);
		}

	if( GoAndAttackMode && (!NSL[MyNation]||Lpressed) ){
		GoAndAttackMode=false;
		Lpressed=false;
	}

	SetCurPtr(0);
	//ProcessInformation(0);
	if(Rpressed&&BuildMode){
		Rpressed=0;
		BuildMode=false;		
		UnPress();
	};
	byte MyMask=NATIONS[MyNation].NMask;
	byte LastSCMD=SpecCmd;
	NoPress=false;
	if(!(GetKeyState(VK_SHIFT)&0x8000)){
		if(SHIFT_VAL){
			if(SHIFT_VAL>0&&SHIFT_VAL<99){
				CmdSelBrig(MyNation,0,SHIFT_VAL-1);
			};
		};
		SHIFT_VAL=0;
	};
	bool CanCapt=0;
	bool CanWork=0;
	bool CanAttBLD=0;
	bool CanEnter=0;
	bool CanShoot=0;
	bool CanRes=0;
	bool HaveLowUnits=0;
	bool SetDestOnly=1;
	bool CanSetDest=0;
	bool HavePeasants=0;
	bool Artilery=0;
    bool HaveBrigade=false;
	Brigade* OnlyOneBrig=NULL;
	bool WasBrig=false;
	bool OnlyMyUnits=true;
	bool NeedSendAfterAttack=true;
	bool NowPerformsBrigAttack=false;
	bool CanRepair=0;
	byte MTMASK=0;
	if(!(OverInform||MouseOverMiniMenu)){
#ifdef _USE3D
		void ProcessGModelsEdit();
		ProcessGModelsEdit();
		if(!MouseOverMiniMap)ProcessRoadsEdition(LastMx,LastMy);
#endif //_USE3D
		int N=ImNSL[MyNation];
		word* uni=ImSelm[MyNation];
		word* sns=ImSerN[MyNation];
		for(int i=0;i<N;i++){
			word MID=uni[i];
			if(MID!=0xFFFF){
				OneObject* ME=Group[MID];
				if(ME&&ME->Serial==sns[i]&&!ME->Sdoxlo){
					if(ME->NNUM!=GSets.CGame.cgi_NatRefTBL[MyNation])OnlyMyUnits=false;
					NewMonster* NM=ME->newMons;
					if(NM->AttBuild)CanAttBLD=1;
					if(ME->BrigadeID!=0xFFFF){
						HaveBrigade=1;
						Brigade* BR=CITY[ME->NNUM].Brigs+ME->BrigadeID;
						if(BR->NGrenades)CanAttBLD=1;
						if(BR->NewBOrder){
							int ID=BR->NewBOrder->GetBrigadeOrderID();
							if(ID==BRIGADEORDER_RIFLEATTACK){
								NeedSendAfterAttack=false;
                                NowPerformsBrigAttack=true;
							}
							if(ID==BRIGADEORDER_BITVA){
								NowPerformsBrigAttack=true;
							}
						}
						if(OnlyOneBrig==NULL&&WasBrig==false){
                            OnlyOneBrig=BR;
							WasBrig=true;
						}else{
							if(BR!=OnlyOneBrig){
								OnlyOneBrig=NULL;
							}
						}
					}
					if(!NM->Capture){
						if(ME->BrigadeID!=0xFFFF||(!EngSettings.CaptureOnlyWithFormations)){
							CanCapt=1;
						}
					}
					if(NM->CanBuild)CanRepair=1;
					if(NM->Peasant){
						CanWork=1;
						CanRepair=1;
						CanRes=1;						
					};										
					if(NM->CompxUnit&&NM->CompxUnit->CanTakeExRes){
						for(int j=0;j<16;j++){
							if(NM->CompxUnit->TakeResStage[j]){
								CANTAKEMASK[j]=1;
								CanRes=1;
							};
						};
					};
					if(!NM->Priest)MTMASK|=NM->KillMask;
					if(!(ME->NewBuilding||ME->LockType||NM->Artilery)){
						CanEnter=1;
						if(!NM->HighUnit)HaveLowUnits=1;
					};
					if(NM->Artilery)Artilery=1;
					if(NM->CanShoot)CanShoot=1;
					if(NM->Building){
						CanSetDest=1;
					}else SetDestOnly=0;
					if(NM->Peasant)HavePeasants=1;
				};
			};
		};
		if(!CanSetDest)SetDestOnly=0;
		if( GoAndAttackMode || NSL[MyNation]&&(GetKeyState(VK_CONTROL)&0x8000) )
			SetCurPtr(AttackPtr);
		if(SetDestMode)SetCurPtr(SetDestPtr);
		if(AttGrMode)SetCurPtr(AttGrPtr);
	};
	if(!OnlyMyUnits){
		CanCapt=0;
		CanWork=0;
		CanAttBLD=0;
		CanEnter=0;
		CanShoot=0;
		CanRes=0;				
		CanSetDest=0;
		HavePeasants=0;
		Artilery=0;
		HaveBrigade=false;		
		WasBrig=false;		
		CanRepair=0;
	}
	if((!Lpressed)&&WasSelectionBar){
		int N=ImNSL[MyNation];
		if(N){
			word* seli=ImSelm[MyNation];
			for(int k=0;k<N;k++){
				word MID=seli[k];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Ready)MakeOrderSound(OB,4);
				};
			};
		};
		WasSelectionBar=false;
	};
	bool Ipanel=true;
	bool cooren=false;
	bool mini=false;
	int xmx,ymy;
	extern bool MiniMapVisible;
	if( (GSets.CGame.ViewMask&2) && MiniMapVisible && MiniLx&&MiniLx&&x>=minix&&y>=miniy&&x<minix+(msx>>1)&&y<miniy+(msy>>1)&&!(LockMouse||WasSelectionBar)){
		SetRLCWindow(minix,miniy,msx>>ADDSH,msy>>ADDSH,SCRSizeX);
		Hline(x-4,y,x-4+8,255);
		Vline(x,y-4,y-4+8,255);
		SetRLCWindow(0,0,COPYSizeX,RSCRSizeY,SCRSizeX);
		cooren=true;
		//xmx=MinMapX+(x-minix)*(MaxMapX-MinMapX)/MiniLx;
		//ymy=MinMapY+(y-miniy)*(MaxMapY-MinMapY)/MiniLy;
		void Mini2World(int& x,int& y);
		xmx=x;
		ymy=y;
		Mini2World(xmx,ymy);
		xmx<<=5;
		ymy<<=4;
		LimitCamera();
		//xmx=(xmx<<5)+16;
		//ymy=(mul3(ymy)<<3)+12;
		mini=true;
		MouseOverMiniMap=true;
		if(HaveBrigade&&Rpressed&&!EngSettings.DontUseRoads){
			int yr=ymy<<1;
			int GetNRoads();
            int NR=GetNRoads();
			int minr=900;
			for(int i=0;i<NR;i++){
				void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
				int x,y;
				GetPreciseTopCenter(i,0,x,y);
				int rr=Norma(x-xmx,y-yr);
				if(rr<minr){
					minr=rr;
					ymy=y>>1;
					xmx=x;
				}
			}
		}
	}else{
		int smlx=smaplx<<Shifter;
		int smly=mul3(smaply)<<(Shifter-2);
		//int dvd=24>>(5-Shifter);
		if(x>=smapx&&y>=smapy&&x<=smapx+smlx&&y<=smly){
			xmx=(mapx<<5)+((x-smapx)<<(5-Shifter));
			ymy=(mul3(mapy)<<3)+((y-smapy)<<(5-Shifter));
			cooren=true;
		};
	};	
	int yreal;
	int ysy;
	if(MouseOverMiniMap){		
		yreal=ymy<<1;        
        ysy=yreal;
	}else{
		Vector3D V=ScreenToWorldSpace(x,y);
		ysy=V.y;
		xmx=V.x;	
		yreal=V.y;
	}

	if(ext_OnMouseHandling(x,y,Lpressed,Rpressed,xmx,yreal,MouseOverMiniMap))return;	

	// item pick up
	bool ItemPickUp(word ItemID);
	//if(ItemPickUp(Att!=0xFFFF?Att:Friend)){
	if(ItemPickUp(Friend)){
		Rpressed=false;
		return;
	}
	if(ItemPickUp(Att)){
		Rpressed=false;
		return;
	}
	//bool BonusPickUp(word BonusID);
	//if(Rpressed&&BonusPickUp(Att!=0xFFFF?Att:Friend)){
	//	//Rpressed=false;
	//	//return;
	//}

	// ability target
	static int smartpress=0;
	if(v_Targ_MA){
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		if(SP&&SP->ActAbl==v_Targ_MA){
			ActiveUnitAbility* AA=NULL;
			for(int i=0;i<v_Targ_MA->ActiveAbilities.GetAmount();i++){
				ActiveUnitAbility* A=v_Targ_MA->ActiveAbilities[i];
				if(A&&A->Serial==v_Targ_MA_id2){
					AA=A;
					break;
				}
			}
			if(AA){
				word TID=Att!=0xFFFF?Att:Friend;	// index of target unit
				bool CanPress=AA->CanTarget(TID,LastMx,LastMy,0);
				//if(!AA->DrawCursor(LastMx,LastMy)){
				//	if(CanPress){
				//		VitCursor=13;
				//	}else{
				//		VitCursor=14;
				//	}
				//}else{
				//	VitCursor=15;
				//	ITerra->FlushPatches();
				//}
				if(Lpressed){
					if(CanPress){
						vui_IS_Abl_Target IS;
						IS.Data.NI=SP->NI;
						//IS.Data.NIndex=SP->NIndex;
						
						IS.Data.AblID=v_Targ_MA_id;
						IS.Data.AblSerial=v_Targ_MA_id2;

						IS.Data.TargOB=TID;					
						IS.Data.TargX=LastMx;
						IS.Data.TargY=LastMy;
						IS.InterfAction();
					}else{
						Lpressed=false;
					}
				}
				if(Lpressed||Rpressed||(GetKeyState(VK_ESCAPE)&0x8000)){
					v_Targ_MA=NULL;
					VitCursor=-1;
					smartpress=5;
				}
			}
		}else{
			v_Targ_MA=NULL;
		}
	}
	if(smartpress>0){
		smartpress--;
		Lpressed=false;
		Rpressed=false;
		realLpressed=false;
		realRpressed=false;
	}
	
	if(cooren){
		SpotVis=GetObjectVisibilityInFog(xmx,yreal,GetHeight(xmx,yreal),NULL);
	}
	static altP=0;
	static altPx=0;
	static altPy=0;
	bool ALTP=GetKeyState('X')&0x8000;
	if(ALTP&!altP){
		altP=1;
		altPx=xmx;
		altPy=yreal;
	}
	if(altP){
		if(ALTP){
			int dx=xmx-altPx;
			int dy=yreal-altPy;
			int dxr=dx+dy;
			int dyr=dx-dy;
			if(abs(dxr)>abs(dyr))dyr=0;else dxr=0;
			dx=(dxr+dyr)/2;
			dy=(dxr-dyr)/2;
			xmx=altPx+dx;
			yreal=altPy+dy;
			ymy=yreal/2-GetHeight(xmx,yreal);
			ysy=yreal/2;
		}else{
			altP=0;
		}
	}
	if(cooren){
		LastMx=xmx;
		LastMy=yreal;
		//temp				
	}
	if(cooren&&mini&&Lpressed&&GetKeyState(VK_CONTROL)&0x8000){
		void SendAllySignal(int x, int y);
		SendAllySignal(xmx,yreal);
		Lpressed=false;
		UnPress();
	}
	sprintf(HEIGHTSTR,"%d",GetHeight(LastMx,LastMy) );
	if(cooren&&EditMapMode){
		ControlAZones(xmx,yreal);
	};
	bool SETDEST=false;
	bool DOGUARD=0;
	bool DOPATROL=0;
	bool DOATTGR=0;
	if(PATROLMODE&&Rpressed){
		DOPATROL=1;
		PATROLMODE=0;
		Lpressed=false;
		Rpressed=false;
		UnPress();
	};
	if((SetDestMode||GUARDMODE||PATROLMODE||AttGrMode)&&(Lpressed||Rpressed||LastKey==13)){
		if(Lpressed){
			if(SetDestMode)SETDEST=true;
			if(GUARDMODE)DOGUARD=true;
			if(PATROLMODE)DOPATROL=true;
			if(AttGrMode)DOATTGR=true;
		}

		SetDestMode=false;
		GUARDMODE=false;
		PATROLMODE=false;
		AttGrMode=false;
		
		Lpressed=false;
		Rpressed=false;
		UnPress();
		LastKey=0;
	};
	if(GetCoord){
		if(Lpressed){
			if(cooren){
				if(UNIM)UNIM(0,xmx>>5,ymy>>5);
			};
			Lpressed=false;
			SetCurPtr(0);
			curdx=0;
			curdy=0;
			GetCoord=false;
			DoSelSound(1);
			MFix();
			goto PrInfo;;
		}
		if(Rpressed){
			SetCurPtr(0);
			curdx=0;
			curdy=0;
			GetCoord=false;
			MFix();
			goto PrInfo;;
		};
	};
	if(SpecCmd>=50&&SpecCmd<60){
		CmdMemSelection(MyNation,SpecCmd-50);
	}
	if(SpecCmd>=60&&SpecCmd<70){
		CmdRememSelection(MyNation,SpecCmd-60);
	}
	if(SpecCmd==137)CmdPause(MyNation);
	if(SpecCmd==1)CmdSelAllUnits(MyNation);
		//CmdCreateGoodSelection(MyNation,0,0,msx<<5,msy<<5);
	if(SpecCmd==2)CmdCreateGoodSelection(MyNation,mapx<<5,mapy<<5,(mapx+smaplx-1)<<5,(mapy+smaply-1)<<5);
	//Select all peasants
	if(SpecCmd==3)CmdSelAllPeasants(MyNation);
	//Select all warriors
	if(SpecCmd==5)CmdSelAllWarriors(MyNation);
	//On screen
	if(SpecCmd==6)CmdCreateGoodKindSelection(MyNation,mapx<<5,mapy<<5,mul3(mapx+smaplx-1)<<3,(mapy+smaply-1)<<5,2);
	//Select all towers
	//if(SpecCmd==7)CmdCreateGoodKindSelection(MyNation,0,0,msx<<5,mul3(msy)<<3,1);
	//On screen
	//if(SpecCmd==8)CmdCreateGoodKindSelection(MyNation,mapx<<5,mapy<<5,mul3(mapx+smaplx-1)<<5,(mapy+smaply-1)<<3,1);
	//Select all buildings
	if(SpecCmd==9)CmdSelectBuildings(MyNation);
	//On screen
	//if(SpecCmd==10)CmdCreateGoodKindSelection(MyNation,mapx<<5,mapy<<5,mul3(mapx+smaplx-1)<<3,(mapy+smaply-1)<<5,4);
	//Select all dragons
	if(SpecCmd==13)CmdSelAllFlyObjects(MyNation);
	if(SpecCmd==14)CmdSelAllShooters(MyNation);
	//On screen
	//if(SpecCmd==14)CmdCreateGoodKindSelection(MyNation,mapx<<5,mapy<<5,mul3(mapx+smaplx-1)<<5,(mapy+smaply-1)<<3,5);
	if(SpecCmd==200)CmdDie(MyNation);
	if(SpecCmd==201)CmdSelAllShips(MyNation);
	if(SpecCmd==111)GoToCurSelPosition();
	if(SpecCmd==112){
		int x=LastActionX;
		int y=LastActionY;
		Brigade* GetLastBrigAlert();
		Brigade* BR=GetLastBrigAlert();
		if(BR&&BR->Alert.CommanderTime>LastActionTime){			
			BR->Alert.Status=0;
			BR->GetCenter(&x,&y);			
			LastActionX=x;
			LastActionY=y;
		}
		if(LastActionX!=0){
			void SetCameraPosAbs(const float xGrid, const float yGrid);
			SetCameraPosAbs(LastActionX, LastActionY);
		};
	};
	if(SpecCmd==113)CmdSelectIdlePeasants(MyNation);
	if(SpecCmd==114)CmdSelectIdleMines(MyNation);
	if(SpecCmd==11){
		word NSel=ImNSL[MyNation];
		if(NSel){
			word MID=ImSelm[MyNation][0];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB)CmdCreateGoodTypeSelection(MyNation,-msx<<6,-msy<<5,msx<<6,msy<<5,OB->NIndex);
			};
		};
	};
	//if(LastRealLP&&!Lpressed){
	//	if(GetRealTime()-LastClickTime<650&&Norma(x-LastClcX,y-LastClcY)<16){
	//		SpecCmd=241;
	//	};
	//	LastClickTime=GetRealTime();
	//	LastClcX=x;
	//	LastClcY=y;
	//};
	
	if( SpecCmd==241 ){
		word MIDX=0xFFFF;
		//MIDX=AdvancedGetNewFriend(xmx,yreal,x,y,MyNation);
		//if(MIDX==0xFFFF){
		//	MIDX=AdvancedGetNewEnemy(xmx,yreal,x,y,MyNation);
		//	if(MIDX==0xFFFF){
				word NSel=ImNSL[MyNation];
				if(NSel){
					word MID=ImSelm[MyNation][0];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB){
							MIDX=MID;						
						}
					}
				}
		//	}
		//}
		if(MIDX!=0xFFFF){
			OneObject* OB=Group[MIDX];
			if(OB){
				CmdCreateGoodTypeSelection(MyNation,0,0,RealLx,RealLy,OB->NIndex);
				UnPress();
			}
		}
	}
	SpecCmd=0;
	LastRealLP=Lpressed;
	//if(BuildWall)goto Edgetest;
	int xxx;
	int yyy;
	if(cooren){
		xxx=xmx>>5;
		yyy=div24(ymy);
		MRealX=xmx;
		MRealY=yreal;
		if(LastSCMD==219)DelInGrid(xmx,ymy);
		if(!mini)ProcessTexPiece(xmx,(mapy<<5)+mouseY*2,Lpressed);
		if(LastSCMD==218){
			ProcessRM_Load(xmx,yreal);
		};
		TerrHi=GetTotalHeight(xmx,yreal);
		if(MEditMode){
#ifdef _USE3D
			if(GetKeyState(VK_SPACE)&0x8000){				
				ShowSqPen(xmx,yreal,ReliefBrush<<4,0xFFFFFFFF);
			}else ShowPen(xmx,yreal,ReliefBrush<<4,0xFFFFFFFF);
#else //_USE3D
			ShowPen(xmx,yreal,ReliefBrush<<4,0xFFFFFFFF);
#endif //_USE3D
		};
		if(HeightEditMode==1||HeightEditMode==10||HeightEditMode==11||HeightEditMode==12||HeightEditMode==13||HeightEditMode>=128){//edit height mode
			bool LockKey=0;
			if(Lpressed){
				switch(HeightEditMode){
				case 1:
					HiStyle=1;
					LockKey=1;
					break;
				case 10:
					HiStyle=7;
					LockKey=1;
					break;
				case 11:
					HiStyle=9;
					LockKey=1;
					break;
				case 12:
					HiStyle=3;
					LockKey=1;
					break;
				};
			};
			if(Rpressed){
				switch(HeightEditMode){
				case 1:
					HiStyle=2;
					LockKey=1;
					break;
				case 10:
					HiStyle=8;
					LockKey=1;
					break;
				case 11:
					HiStyle=5;
					LockKey=1;
					break;
				case 12:
					HiStyle=4;
					LockKey=1;
					break;
				};
			};
			bool Shpress=(GetKeyState(VK_SHIFT)&0x8000)||HeightEditMode==13;
			if(CheckCurve()||Shpress){
				bool LINK=CheckCurveLinked(xmx,yreal);
				if((Lpressed&&LINK)||Rpressed)Shpress=0;
				if(LINK)ShowPen(xmx,yreal,8,0xFFFF0000);
				if(Lpressed){
					AddPointToCurve(xmx,yreal,!Shpress,2);
					if(!Shpress)Lpressed=0;
				}else if(!Shpress)AddPointToCurve(xmx,yreal,true,2);
			}else{
				if(GetKeyState(VK_CONTROL)&0x8000)ShowSqPen(xmx,yreal,ReliefBrush<<4,0xFFFF0000);
				else ShowPen(xmx,yreal,ReliefBrush<<4,0xFFFF0000);
				
			}
			switch(HiStyle){
			case 1://+
				if(GetKeyState(VK_CONTROL)&0x8000){
					void DrawHeightRect(int xc,int yc,int r,int H);
					DrawHeightRect(xmx,yreal,ReliefBrush<<4,-80);
				}else AddHi(xmx,yreal,ReliefBrush<<4,10);
				break;
			case 2://+
				AddHi(xmx,yreal,ReliefBrush<<4,-10);
				break;
			case 3:// *
				AverageHi(xmx,yreal,ReliefBrush<<4);
				break;
			case 4:// /
				PlanarHi(xmx,yreal,ReliefBrush<<4);
				break;
			case 5:
				RandomHi(xmx,yreal,ReliefBrush<<4,1,1);
				break;
			case 6:
				RandomHi(xmx,yreal,ReliefBrush<<4,3,3);
				break;
			case 7://PgUP
				AddHiPlanar(xmx,yreal,ReliefBrush<<4,3);
				break;
			case 8://PgDN
				AddHiPlanar(xmx,yreal,ReliefBrush<<4,-3);
				break;
			case 9://Home
				CreatePlane(xmx,yreal,ReliefBrush<<4);
				break;
			};
#ifdef _USE3D
			if(HeightEditMode>=128){
				if(Lpressed){
					void AddRandomGSP(int x,int y,byte SType,int R);
					AddRandomGSP(xmx,yreal,HeightEditMode-128,ReliefBrush<<4);
				};
				if(Rpressed){
					void ClearGSInRadius(int x,int y,int R);
					ClearGSInRadius(xmx,yreal,ReliefBrush<<4);
				};
				//Lpressed=0;
			};
#endif
			HiStyle=0;
			if(LockKey)return;
		};
		if(HeightEditMode==2){//Add tree mode
			extern int InteriorType;
			if(InteriorType!=3)ShowPen(xmx,yreal,ReliefBrush<<4,0xFFFFFF00);
			else ShowPen(xmx,yreal,8,0xFFFFFF00);
		};
		if(HeightEditMode==3){//Delete tree mode
			ShowPen(xmx,yreal,ReliefBrush<<4,0xFFFF0000);
		};
		if(LockMode==1){
			//Lock mode
			ShowPen(xmx,yreal,ReliefBrush<<4,0xFF00FFFF);//ReliefBrush<<4,0xDC);
		};
		if(LockMode==2){
			//Lock mode
			ShowPen(xmx,yreal,ReliefBrush<<4,0xFFFFFFFF);//ReliefBrush<<4,0xBB);
		};
		if(LockMode==3){
			//Lock mode
			ShowPen(xmx,yreal,ReliefBrush<<4,0xFFFF00FF);//ReliefBrush<<4,0xD8);
		};
		static int CurrentFormationSizeIndex=3;
		static byte CurrentFormationDir=0;
		static byte CurrentFormationShape=0;
		static int FormationIndex=0;
		if(Creator<4096+200){
			if(GetKeyState(VK_CONTROL)&0x8000)
			{
				if(GetKeyState(VK_MENU)&0x8000)
				{
					if(WheelDelta>0)
						CurrentFormationSizeIndex++;
					if(WheelDelta<0)
						CurrentFormationSizeIndex--;	
				}
				else
				{
					if(WheelDelta>0)
						CurrentFormationDir+=16;
					if(WheelDelta<0)
						CurrentFormationDir-=16;
				}
				if(Rpressed)
				{
					CurrentFormationShape++;
					Rpressed=0;
				}
				WheelDelta=0;
				if(CurrentFormationSizeIndex<0)CurrentFormationSizeIndex=3;
				if(CurrentFormationSizeIndex>3)CurrentFormationSizeIndex=0;
				
				int GetFormationIndexBySizeIndex(byte NI, word UnitType, byte SizeIndex, byte CurShape);
				FormationIndex=GetFormationIndexBySizeIndex(MyNation,Creator,CurrentFormationSizeIndex,CurrentFormationShape);
				if(FormationIndex>-1)
				{
					OrderDescription* OD=&(ElementaryOrders[FormationIndex]);
					int N=PORD.CreateSimpleOrdPos(xmx<<4,yreal<<4,CurrentFormationDir,OD->NUnits,NULL,OD);
					DWORD cl=0xFFFFFF00;
					for(int j=0;j<N;j++)
					{
						int xx=PORD.px[j]>>4;
						int yy=PORD.py[j]>>4;
						DrawWorldLine(xx,yy-2,xx,yy+2,cl);//DrawPlaneLine
						DrawWorldLine(xx-2,yy,xx+2,yy,cl);
					}
				}
				//shurik(Alexander) lives here
				//WheelDelta
			}
		}
		if(DrawPixMode&&(Lpressed||Rpressed||CurPressure||PrevPress)){
			int br;
			int BRSZ;
			if(WT_IS_INIT){
				//void SetPixLine(int x0,int y0,int x1,int y1,int R,int R1,int h0,int h1,int h01);
				if(GetKeyState(VK_CONTROL)&0x8000&&Lpressed){
					DrawHole(xmx,yreal);
				}else{
					int br0;
					int BRSZ0;	

					if(CurDrMod&2)BRSZ0=PEN_RADIUS*PrevPress/512;
					else BRSZ0=PEN_RADIUS;
					if(CurDrMod&1)br0=PEN_BRIGHTNESS*PrevPress/512;
					else br0=PEN_BRIGHTNESS;

					if(CurDrMod&2)BRSZ=PEN_RADIUS*CurPressure/512;
					else BRSZ=PEN_RADIUS;
					if(CurDrMod&1)br=PEN_BRIGHTNESS*CurPressure/512;
					else br=PEN_BRIGHTNESS;

					if(br>255)br=255;
					if(br0>255)br0=255;

					if(PenBackSide||(GetKeyState(VK_MENU)&0x8000))SetPixLine(xmx,ymy,prevdrx,prevdry,BRSZ0<<1,BRSZ<<1,255-br,255,255-br0);
					else SetPixLine(xmx,ymy,prevdrx,prevdry,BRSZ,BRSZ0,br,0,br0);
				};
				//if(Rpressed)SetPixLine(xmx,ymy,prevdrx,prevdry,BRSZ<<1,0,255);
			}else{
				BRSZ=PEN_RADIUS;
				br=PEN_BRIGHTNESS;
				if(br>255)br=255;
				if(Lpressed)SetPixLine(xmx,ymy,prevdrx,prevdry,BRSZ,br,0);
				if(Rpressed)SetPixLine(xmx,ymy,prevdrx,prevdry,BRSZ<<1,0,255);
			};
			
		};
		prevdrx=xmx;
		prevdry=ymy;
		if(DrawGroundMode){			
			DWORD cc1=0xFF80FF80;
			DWORD cc2=0xFFFFFFFF;
			bool M3D=Mode3D;
#ifndef _USE3D
			Mode3D=false;
			int rr=ReliefBrush<<4;
			ShowPen(xmx,ymy,rr,cc1);
			int dr=CostThickness<<4;
			if(dr<rr)ShowPen(xmx,ymy,rr+dr,cc2);
			Mode3D=M3D;
#else
			int rr=ReliefBrush<<4;
			ShowPen(xmx,yreal,rr,cc1);
			int dr=CostThickness<<4;
			if(dr<rr)ShowPen(xmx,yreal,rr+dr,cc2);
#endif
			
		};
		if(WaterEditMode&&WaterEditMode<3){
			DWORD cc1=0xFF0000FF;
			DWORD cc2=0xFF00FFFF;
			if(WaterEditMode==2){
				cc1=0xA8;
				cc2=0xC8;
			};
			bool M3D=Mode3D;
			Mode3D=false;
			int rr=ReliefBrush<<5;
			ShowPen(xmx,ymy,rr,cc1);
			int dr=CostThickness<<4;
			if(dr<rr)ShowPen(xmx,ymy,rr-dr,cc2);
			Mode3D=M3D;
		};
		void ProcessWaterFlowEditor();
		ProcessWaterFlowEditor();
		if(HeightEditMode==2&&!Lpressed){			
			if(WheelDelta){
				//ObjCharacter::RotFI+=float(WheelDelta)/1200.0f;
				WheelDelta=0;
			}
		}			
		if(Lpressed&&!(mini)){
			if(ChoosePosition){
				mapPos[(MyNation<<1)]=xxx;
				mapPos[(MyNation<<1)+1]=yyy;
				ChoosePosition=false;
			};
			if(HeightEditMode==2){
				//CreateBlob(xxx-1,yyy-1,4*BlobMode,ReliefBrush);
				//addTrees(xmx,yreal,ReliefBrush<<4);				
				extern float sprMaxScale;
				extern float sprMaxThetaAngle;
				extern float sprExraZ;
				void addTreesEx(int x,int y1,int r,float Scale,float Theta,float);
				addTreesEx(xmx,yreal,ReliefBrush<<4,sprMaxScale,sprMaxThetaAngle,sprExraZ);
				goto Edgetest;
			};
			if(HeightEditMode==3){
				//CreateBlob(xxx-1,yyy-1,4*BlobMode,ReliefBrush);
				delTrees(xmx,yreal,ReliefBrush<<4);
				goto Edgetest;
			};
			if(WaterEditMode==1){
				SetWaterSpot(xmx,yreal,ReliefBrush);
				goto Edgetest;
			};
			if(WaterEditMode==2){
				EraseWaterSpot(xmx,yreal,ReliefBrush);
				goto Edgetest;
			};
			if(WaterEditMode==3){
				//SetBrightSpot(xmx,yreal,ReliefBrush,true);
				goto Edgetest;
			};
			if(WaterEditMode==4){
				SetBrightSpot(xmx,yreal,ReliefBrush,false);
				goto Edgetest;
			};
			if(MEditMode){
				bool Shpress=(GetKeyState(VK_SHIFT)&0x8000)!=0;
				if(CheckCurve()||Shpress){
					AddPointToCurve(xmx,yreal,!Shpress,1);
					if(!Shpress)Lpressed=0;
				}else SetTexturedRound(xmx,yreal,ReliefBrush*16,EditMedia);
				goto Edgetest;
			};
			if(LockMode==1){				
				AddUnLockRound(xmx>>6,yreal>>6,ReliefBrush/4);
				goto Edgetest;
			};
			if(LockMode==2){
				void AddLockFiller(word x,word y);
				if(GetKeyState(VK_SHIFT)&0x8000)AddLockFiller(xmx>>6,yreal>>6);
				else AddLockRound(xmx>>6,yreal>>6,ReliefBrush/4);
				goto Edgetest;
			};
			if(LockMode==3){
				AddDefaultRound(xmx>>6,yreal>>6,ReliefBrush/4);
				goto Edgetest;
			};
			if(Creator<4096+200){
				if(GetKeyState(VK_CONTROL)&0x8000){
					//shurik(Alexander) lives here
					//WheelDelta
					//bool PutNewFormationBySizeIndex(byte NI, word UnitType, byte SizeIndex, int x, int y, byte Dir=0);
					//PutNewFormationBySizeIndex(MyNation,Creator, CurrentFormationSizeIndex,xmx<<4,ConvY(ysy)<<4,CurrentFormationDir);
					int PutNewFormation(byte NI, word UnitType, word FormationIndex, int x, int y, byte Dir=0);
					PutNewFormation(MyNation,Creator,FormationIndex,xmx<<4,yreal<<4,CurrentFormationDir);
					Lpressed=0;
					void SetMonstersInCells();
					SetMonstersInCells();
					UnPress();
				}else 
					CmdCreateNewTerr(MyNation,xmx,yreal,Creator|32768);
				if(NATIONS->Mon[Creator]->newMons->Building){
					Lpressed=0;
					void SetMonstersInCells();
					SetMonstersInCells();
					UnPress();
				}
				MFix();
				goto PrInfo;;
			};
			if(DrawGroundMode){
				SetGroundTexRound(CurGroundTexture,xmx,yreal,ReliefBrush<<4,CostThickness<<4);				
			};
			if(BuildMode){
				//if(CheckGold(xxx,yyy,BNat->Mon[BuildingID])&&ManualFogCheck(xxx,yyy,NATIONS[MyNation].Mon[BuildingID]->SizeX))
				//	CmdCreateBuilding(MyNation,xxx,yyy,BuildingID);
				int x0=xmx<<4;
				int y0=yreal<<4;
				if(CheckSmartCreationAbility(GSets.CGame.cgi_NatRefTBL[MyNation],BuildingID,&x0,&y0)!=-1){
					CmdCreateBuilding(MyNation,x0,y0,BuildingID);
					if(!(GetKeyState(VK_SHIFT)&0x8000)){						
						BuildMode=false;
					}
					Lpressed=false;
					UnPress();
					void ACT(int x);
					ACT(82);
				};
				MFix();
				goto PrInfo;
			};
		};
	};
	if(DrawGroundMode&&(Rpressed||(Lpressed&&PenBackSide))&&!(mini||BuildMode)){
		ClearGroundTexRound(xmx,ymy,ReliefBrush<<4,CostThickness<<4);
	};
	if(cooren){
		//assert(StartX<100000);
		int gx=xmx;
		int gy=ymy;
		int gx1=StartX;
		int gy1=StartY;
		
		if(gx>gx1){int z=gx;gx=gx1;gx1=z;};
		if(gy>gy1){int z=gy;gy=gy1;gy1=z;};
		int xxx=xmx>>5;
		int yyy=yreal;
		int xxx1=StartX>>5;
		int yyy1=div24(LStartY);
		if(xxx1<xxx){int z=xxx;xxx=xxx1;xxx1=z;};
		if(yyy1<yyy){int z=yyy;yyy=yyy1;yyy1=z;};
		if(mini){
			// set dest rally point
			if(Rpressed){ //GetKeyState(VK_CONTROL)&0x8000
				word Nsel=NSL[MyNation];
				if(Nsel){
					int MID=Selm[MyNation][0];
					OneObject* OB=Group[MID];
					if( MID!=0xFFFF && OB && OB->newMons->ConcPtX.GetAmount() && !OB->newMons->NewParams.ControlButtonsDisable /*&&OB->Ref.General->CanDest*/ && OB->NNUM==GSets.CGame.cgi_NatRefTBL[MyNation] ){
						CmdSetDst(MyNation,xmx<<4,yreal<<4);
					}
				}
				SetDestMode=false;
			};
			if(SETDEST){
				CmdSetDst(MyNation,xmx<<4,yreal<<4);
				SetDestMode=false;
			};
		};
		if(Lpressed&&!mini){
			word ENMI=/*SpotVis?*/AdvancedGetNewEnemy(xmx,yreal,x,y,MyNation);//:0xFFFF;
			if(ENMI!=0xFFFF){
				OneObject* OBBE=Group[ENMI];
				if(OBBE){
					char ggg[128];
					if(OBBE->InArmy&&OBBE->BrigadeID!=0xFFFF){
						Brigade* BR=OBBE->Nat->CITY->Brigs+OBBE->BrigadeID;
						int NU=0;
						char* UName=NULL;
						for(int i=NBPERSONAL;i<BR->NMemb;i++){
							word MID=BR->Memb[i];
							if(MID!=0xFFFF){
								OneObject* BOB=Group[MID];
								if(BOB&&BOB->Serial==BR->MembSN[i]){
									NU++;
									UName=BOB->Ref.General->Message;
								};
							};
						};
						/*
						if(UName){
							sprintf(ggg,FormationStr,UName,NU);
							if(GetKeyState(VK_CONTROL)&0x8000){
								sprintf(ggg+strlen(ggg)," [ID=%d]",ENMI);
							};
							AssignHint1(ggg,64);
						};
						*/
					}else{
						/*
						sprintf(ggg,"%s (%s) (%d/%d)",OBBE->newMons->Message,NatNames[OBBE->Ref.General->NatID],OBBE->Life,OBBE->MaxLife);
						AssignHint1(ggg,64);
						*/
					};
					AddPulse(ENMI,0xD0);
				};
			};
			if(!(DrawPixMode||DrawGroundMode)){
				if(!SelSoundDone){
					SelSoundDone=true;
					SelSoundReady=true;
					SelSoundType=0;
				};
				SetRLCWindow(smapx,smapy,smaplx<<Shifter,mul3(smaply)<<(Shifter-2),SCRSizeX);
				int xz1=smapx+((StartX-(mapx<<5))>>(5-Shifter));
				int yz1=smapy+((StartY-(mul3(mapy)<<3))>>(5-Shifter));
				int vdx=smapx-(mapx<<5);
				int vdy=smapy-(mapy<<4);
				//if(!BIRUZOVIJ)BIRUZOVIJ=GetPaletteColor(0,255,255);
				//Xbar(xz1,yz1,x-xz1,y-yz1,BIRUZOVIJ);
				Vector3D VS = SkewPt(StartX,StartY,tmax(Mode3D?GetTotalHeight(StartX,StartY):0,0));
				WorldToScreenSpace(VS);
				if(!GSets.DisableSelectionBar)GPS.DrawFillRect(VS.x,VS.y,x-VS.x,y-VS.y,0x60000000);
				if(Norma(x-VS.x,y-VS.y)>12)NoPress=true;
				//if(x!=xz1||y!=yz1)Ipanel=false;
				if(LastSCMD==217){
					ProcessRM_Save(StartX,StartY,xmx,yreal);
				};
				
				LockMouse=true;
				int nn=0;
				extern int WallsEditMode;
				if(!WallsEditMode){
					if(!KeyPressed)LastKey=0;
					CmdCreateGoodSelection(MyNation,VS.x,VS.y,x,y);
				}
				WasSelectionBar=true;
			};
		} else{
			if(!mini){
				StartX=xmx;
				StartY=yreal;
				LStartY=StartY;
				SelSoundReady=false;
				SelSoundDone=false;
			};
			LockMouse=false;
		};
		//if(Rpressed)SetSpot(mouseX,mouseY);
		if(Lpressed){
			word fr7=AdvancedGetNewFriend(xmx,yreal,x,y,7);
			AddPulse(fr7,0x9A);
			if(fr7!=0xFFFF)CheckDipBuilding(MyNation,fr7);
		};
		Att=0xFFFF;
		Friend=0xFFFF;
		//if(!(mini || (VitCursor!=-1 && !StrelMode))){ // так не работают курсоры VitCursor
		if(!mini){
			
			if( VitCursor!=15 ){ // Cursor is textured
				Friend=AdvancedGetNewFriend(xmx,yreal,x,y,MyNation);
				Att=AdvancedGetNewEnemy(xmx,yreal,x,y,GSets.CGame.cgi_NatRefTBL[MyNation]);
			}

			void TestConnector(word CurObj,byte NI);
			if(Friend!=0xFFFF)TestConnector(Friend,MyNation);

			word Entr=GetDoorsOfEnemy(xmx,yreal,xmx,(yreal>>1)-GetHeight(xmx,yreal),GSets.CGame.cgi_NatRefTBL[MyNation]);;
			if(Entr!=0xFFFF){
				OneObject* OB=Group[Entr];
				if(OB&&(!OB->newMons->UnitAbsorber)&&!(OB->newMons->PeasantAbsorber&&HavePeasants))Entr=0xFFFF;
			};
			if(!CanEnter)Entr=0xFFFF;

			if(Att!=0xFFFF&&Entr==0xFFFF&&!(GoAndAttackMode||SimpleDialog::DraggedDialog.Get())){
				OneObject* AOB=Group[Att];
				if( AOB && !AOB->newMons->Immortal ){
					AOB->HighlightMask|=1;
					if(MTMASK||CanAttBLD||CanCapt){				
						bool CheckIfPossibleToAttackBySeleted(byte NI,OneObject* Victim);
						if(CheckIfPossibleToAttackBySeleted(MyNation,AOB)){
							SetCurPtr(AttackPtr);
							if(realRpressed && !StrelMode)realRpressed=false;
							bool isNotChest(OneObject* OB);
							if(!isNotChest(AOB)){
								SetCurPtr(GatherStonePtr);
							}
						}
						if(curptr!=AttackPtr&&AOB->newMons->Capture&&CanCapt&&TestCapture(AOB)!=-1)SetCurPtr(CapturePtr);
						if(GetKeyState(VK_CONTROL)&0x8000){
							//if(AOB->newMons->CanBeCapturedWhenFree)curptr=StormEnemyPtr;
							//else 
							if(AOB->newMons->UnitAbsorber||(AOB->newMons->PeasantAbsorber&&HavePeasants))SetCurPtr(StormEnemyPtr);
						};
					}
					if(AOB->BrigadeID!=0xFFFF){
						Brigade* BR=CITY[AOB->NNUM].Brigs+AOB->BrigadeID;
						if(OnlyOneBrig&&OnlyMyUnits){
							static int lastt=0;
							if(lastt!=tmtmt)FList.AddArrowBetweenBrigades(OnlyOneBrig,BR);
							lastt=tmtmt;								
						}
						for(int i=0;i<BR->NMemb;i++){
							word ID=BR->Memb[i];
							if(ID!=0xFFFF){
								OneObject* OB=Group[ID];
								if(OB&&OB->Serial==BR->MembSN[i]){
									OB->HighlightMask|=1;
								}
							}
						}
					}
				}
			}

			if(SETDEST){
				CmdSetDst(MyNation,xmx<<4,yreal<<4);
			};
			if(DOATTGR){
				//CmdAttackGround(MyNation,xmx,yreal);
				vui_IS_AttackGround2 com;
				com.Data.x=xmx;
				com.Data.y=yreal;
				for(int i=0;i<OIS.SelPoint.GetAmount();i++){
					vui_SelPoint* SP=OIS.SelPoint+i;//OIS.GetLastSelPoint();
					if(SP&&SP->Type==ve_UT_cannon){
						com.Data.ObjID=SP->Inf.Cannon.OB->Index;
						com.Data.ObjSN=SP->Inf.Cannon.OB->Serial;
						com.InterfAction();
						ACT(101);
					}					
				}
			};
			byte MYNI=GSets.CGame.cgi_NatRefTBL[MyNation];
			if(Friend!=0xFFFF){
				OneObject* FRI=Group[Friend];
				if(FRI&&FRI->newMons->Transport&&CheckTransportEntering(FRI)&&(CanEnter||Artilery))
					SetCurPtr(EnterTrPtr);
				/*
				if(FRI->newMons->Peasant&&setlIsReinforce(Friend,Rpressed)){
					SetCurPtr(GoToMinePtr);
				}
				*/
				FRI->HighlightMask|=FRI->NNUM==MYNI?4:2;
				if(FRI->BrigadeID!=0xFFFF){
					Brigade* BR=CITY[FRI->NNUM].Brigs+FRI->BrigadeID;
					for(int i=0;i<BR->NMemb;i++){
						word ID=BR->Memb[i];
						if(ID!=0xFFFF){
							OneObject* OB=Group[ID];
							if(OB&&OB->Serial==BR->MembSN[i]){
								OB->HighlightMask|=OB->NNUM==MYNI?4:2;
							}
						}
					}
				}
			};
			byte rk=0xFF;
			if(CanWork||CanRepair){
				if(Friend!=0xFFFF){
					OneObject* FR=Group[Friend];
					if(FR&&(FR->NewBuilding)&&!FR->Sdoxlo){
						//if(curptr!=RepairPtr&&FR->newMons->ResConcentrator&&FR->Ready&&FR->MoreCharacter->MaxInside+FR->AddInside>FR->NInside)curptr=GoToMinePtr;
						if(FR->Life<FR->MaxLife&&!(GetKeyState(VK_CONTROL)&0x8000))SetCurPtr(RepairPtr);
					};
				};				
			};
			if(curptr!=GoToMinePtr&&curptr!=RepairPtr&&Friend!=0xFFFF){
				OneObject* FR=Group[Friend];
				if( FR && FR->NNUM==MYNI && (((FR->NewBuilding)&&FR->Stage>=FR->MoreCharacter->ProduceStages)||(!FR->NewBuilding))&&!FR->Sdoxlo ){
					if(FR->NNUM!=7 && (FR->Ready||FR->LocalOrder)&&FR->MoreCharacter->MaxInside+FR->AddInside>FR->NInside){
						if(FR->newMons->PeasantAbsorber){
							if(HavePeasants)SetCurPtr(EnterBuildingPtr);
						}else if(FR->newMons->UnitAbsorber){
							if(HaveLowUnits||!FR->newMons->HighUnitCantEnter)SetCurPtr(EnterBuildingPtr);
						};
					};
				};
			};
			if(CanRes){
				rk=DetermineResource(xmx,yreal);
				if(!curptr){
#ifdef CONQUEST
					if(rk<16&&CANTAKEMASK[rk]){
						SetCurPtr(CANTAKECURSOR[rk]);
						TAKERES=1;
						stptr=curptr;
					};
#else
					if(rk==TreeID&&CANTAKEMASK[rk])SetCurPtr(6);
					if(rk==StoneID)SetCurPtr(5);
					if(rk==FoodID)SetCurPtr(7);
#endif
				};
			};
			if(Entr!=0xFFFF){
				OneObject* OB=Group[Entr];
				if(OB){
					if(OB->newMons->UnitAbsorber)SetCurPtr(StormEnemyPtr);
					else if(OB->newMons->PeasantAbsorber&&HavePeasants)SetCurPtr(StormEnemyPtr);
				};
			};
			if(Rpressed){
				DoSelSound(1);
				Rpressed=false;
				UnPress();
				//
				SetDestMode=false;
				bool dest=false;
				word Nsel=NSL[MyNation];
				if(Nsel){
					int MID=Selm[MyNation][0];
					OneObject* OB=Group[MID];
					if( curptr!=AttackPtr && MID!=0xFFFF && OB && OB->NewBuilding && OB->newMons->ConcPtX.GetAmount() && !OB->newMons->NewParams.ControlButtonsDisable && OB->NNUM==GSets.CGame.cgi_NatRefTBL[MyNation] ){
						CmdSetDst(MyNation,xmx<<4,yreal<<4);
						dest=true;
						StrelMode=false;
					}
				}				
				if(!dest){
					StrelMode=true;
					SStartTime=GetTickCount();
					SStartX=xmx;
					SStartY=yreal;
					SCntrX=0;//xmx-SStartX;
					SCntrY=0;//yreal-SStartY;
					NoPress=true;
				}
			}else{				
				if(GetKeyState(VK_ESCAPE)&0x8000){
					// Esc
					StrelMode=false;
				}
				if(StrelMode&&!realRpressed){
					if(mini){
						//search for road, nearest to dest
						if(!EngSettings.DontUseRoads){
							int GetNRoads();
							int N=GetNRoads();
							int rm=10000;
							int xd=-1;
							int yd=-1;
							for(int i=0;i<N;i++){
								void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
								int x,y;
								GetPreciseTopCenter(i,0,x,y);
								int N=Norma(x-xmx,y-yreal);
								if(N<rm){
									rm=N;
									xd=x;
									yd=y;
								}
							}
							if(rm<600){
								xmx=xd;
								yreal=yd;
							}
						}
					}
					//perform action
					//if()
					bool AnimCurs=1;
					StrelMode=false;

					//int H=GetTotalHeight(SStartX,SStartY);
					//int H1=GetTotalHeight(xmx,yreal);		
					//if(H<0)H=0;
					//if(H1<0)H1=0;

					
					int rx=xmx-SStartX;
					int ry=yreal-SStartY;
					int Nr=int(sqrt(rx*rx+ry*ry));
					
					short DIRECT=512;
					if(Nr>StrelDist){//strelka
						DIRECT=GetDir(rx,ry);
						//discrete directions
						DIRECT=(DIRECT+8)&0xF0;
						xmx=SStartX;
						yreal=SStartY;
					};
					xmx=SStartX;
					yreal=SStartY;
					if(rk<0xFE&&stptr==curptr&&TAKERES&&Nr<=StrelDist){//strelka
						CmdTakeRes(MyNation,xmx,yreal,rk);
						extern bool vTakeRes;
						vTakeRes=true;
					};
					//word Friend=GetNewFriend(gx,gy,MyNation);//Mops[yyy][xxx];
					if(Friend!=0xFFFF&&curptr!=5&&curptr!=6&&curptr!=7&&Nr<=StrelDist){//strelka
						OneObject* OB=Group[Friend];
						if(OB&&!OB->Sdoxlo){
							if(OB->NewBuilding){
								CmdBuildObj(MyNation,Friend);
								extern bool vBuildObj;
								vBuildObj=true;
								if(OB->Stage>=OB->MoreCharacter->ProduceStages)CmdGoToMine(MyNation,Friend);							
							}else{
								if(OB->newMons->ShowInsideUnits||OB->newMons->MaxInside){
									CmdGoToMine(MyNation,Friend);									
								}
							}
							AddPulse(Friend,0x9A);
						};
					};					
					if(Friend!=0xFFFF&&Nr<=StrelDist){//strelka
						OneObject* FOBJ=Group[Friend];
						if(FOBJ){
							if(FOBJ->newMons->Transport){
								CmdGoToTransport(MyNation,Friend);
								AddPulse(Friend,0x9A);
								goto PrInfo;;
							};
						};
					};
					void ACT(int x);
					bool cannon=false;
					vui_SelPoint* SP=OIS.GetLastSelPoint();
					if(SP&&SP->Type==ve_UT_cannon){
						cannon=true;
						ACT(108);
					}
					if(Entr!=0xFFFF&&Nr<=StrelDist){//strelka
						OneObject* AOBJ=Group[Entr];
						if(int(AOBJ)){
							if(AOBJ->newMons->CanBeCapturedWhenFree){
								CmdGoToMine(MyNation,Entr);
							};
						};
					}else
					if(Att!=0xFFFF&&Nr<=StrelDist){//strelka
						if(!cannon) ACT(2);
						OneObject* AOBJ=Group[Att];
						if( AOBJ && !AOBJ->newMons->Immortal ){
							if(!(AOBJ->NMask&MyMask)){
								if(GetKeyState(VK_CONTROL)&0x8000){
									//if(AOBJ->newMons->CanBeCapturedWhenFree){
										CmdGoToMine(MyNation,Att);
									//};
								}else{
									static int LastAX=-10000;
									static int LastAY=-10000;
									static int LastAT=-1;									
									if(Norma(LastAX-xmx,LastAY-yreal)>10||LastAT==-1||GetTickCount()-LastAT>2000){
										CmdAttackObj(MyNation,Att,DIRECT);
#ifdef COSSACKS2
										GoAndAttackMode=1;
#endif										
										if(NeedSendAfterAttack)CmdSendToXY(MyNation,xmx<<4,yreal<<4,DIRECT);
										LastAX=xmx;
										LastAY=yreal;
										LastAT=GetTickCount();
										if(AOBJ->NewBuilding)LastAT-=10000;

									}
								};
								AddPulse(Att,0xD0);
							}else CmdSendToXY(MyNation,xmx<<4,yreal<<4,DIRECT);
						}else CmdSendToXY(MyNation,xmx<<4,yreal<<4,DIRECT);
					}else{
						if(!cannon) ACT(1);
						CmdSendToXY(MyNation,xmx<<4,yreal<<4,DIRECT);
					}
				};
			};
			if(GUARDMODE){
				if(Friend!=0xFFFF){
					SetCurPtr(GuardPtr2);
				}else{
					SetCurPtr(GuardPtr1);
				};
			};
			if(DOGUARD){
				CmdSetGuardState(MyNation,Friend);
				GUARDMODE=0;
			};
			if(PATROLMODE){
				SetCurPtr(PatrolPtr);
			};
			if(VitCursor!=-1){
				SetCurPtr(VitCursor);
			}
			if(VitCursorLowProirity!=-1&&curptr==0){
				SetCurPtr(VitCursorLowProirity);
			}
			if(DOPATROL){
				CmdPatrol(MyNation,xmx<<4,yreal<<4);
			};
		}else{
			if(DOPATROL){
				CmdPatrol(MyNation,xmx<<4,yreal<<4);
			}else
			if(Rpressed&&ImNSL[MyNation]>0){
				CmdSendToXY(MyNation,xmx<<4,yreal<<4,512);
				Rpressed=false;
				UnPress();
			};
			if(!realRpressed)StrelMode=false;
		};
		VitCursor=-1;
		VitCursorLowProirity=-1;
	};
	
	if(NullCursor||GSets.CGame.ViewMask==0){
		SetCurPtr(15);
	}

	int rx;
	int ry;
	int Nr;
	int H;
	if(StrelMode){		
		if(GetKeyState(VK_MENU)&0x8000){
			SStartX=xmx-SCntrX;
			SStartY=yreal-SCntrY;
		}else{
			SCntrX=xmx-SStartX;
			SCntrY=yreal-SStartY;
		}

		H=GetTotalHeight(SStartX,SStartY);
		int H1=(GetTotalHeight(xmx,yreal)-H)<<1;		
		if(H<0)H=0;
		if(H1<0)H1=0;

		Vector3D VS=SkewPt(SStartX,SStartY,H);
		WorldToScreenSpace(VS);

		rx=x-VS.x;
		ry=(y-VS.y)*2;
		Nr=int(sqrt(rx*rx+ry*ry));

		if(Nr>StrelDist){//strelka
			//int dx=mapx<<5;
			//int dy=mapy<<4;

			int x0=VS.x;;
			int y0=VS.y;

			YELLOW_COLOR=0xFFFFFF00;
			
			int ux=rx*20/Nr;
			int uy=ry*20/Nr;
			int uxt= uy;
			int uyt=-ux;
			//DrawLine(x0,y0,x0+rx,y0+(ry>>1),clrYello);
			DrawLine(x0-uxt,y0-(uyt>>1),x0+uxt,y0+(uyt>>1),YELLOW_COLOR);
			DrawLine(x0+uxt,y0+(uyt>>1),x0+rx-2*ux+uxt,y0+((ry-2*uy+uyt)>>1),YELLOW_COLOR);
			DrawLine(x0+rx-2*ux+uxt,y0+((ry-2*uy+uyt)>>1),x0+rx-2*ux+2*uxt,y0+((ry-2*uy+2*uyt)>>1),YELLOW_COLOR);
			DrawLine(x0+rx-2*ux+2*uxt,y0+((ry-2*uy+2*uyt)>>1),x0+rx,y0+(ry>>1),YELLOW_COLOR);
			DrawLine(x0+rx,y0+(ry>>1),x0+rx-2*ux-2*uxt,y0+((ry-2*uy-2*uyt)>>1),YELLOW_COLOR);
			DrawLine(x0+rx-2*ux-2*uxt,y0+((ry-2*uy-2*uyt)>>1),x0+rx-2*ux-uxt,y0+((ry-2*uy-uyt)>>1),YELLOW_COLOR);
			DrawLine(x0+rx-2*ux-uxt,y0+((ry-2*uy-uyt)>>1),x0-uxt,y0-(uyt>>1),YELLOW_COLOR);

		};
	}
	if(StrelMode){
		
		// draw position		
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		int N=ImNSL[NI];
		word* ID=ImSelm[NI];
		word* SN=ImSerN[NI];
		word BrigID=0xFFFF;
		for(int i=0;i<N;i++){
			word OID=ID[i];
			if(OID!=0xFFFF){
				OneObject* OB=Group[OID];
				if(OB&&(OB->Serial==SN[i])&&!OB->Sdoxlo&&OB->BrigadeID!=0xFFFF&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[MyNation]){
					if(BrigID!=0xFFFF){
						if(BrigID!=OB->BrigadeID){
							BrigID=0xFFFF;
							break;
						}
					}else{
						BrigID=OB->BrigadeID;
					}
				}
			}
		}
        BrigadesList BLIST;
		BLIST.CreateFromSelected(MyNation,LastMx,LastMy);
		if(BLIST.BrPosArr.GetAmount()){
			if(Nr<StrelDist){//strelka
				//LastDirection=GetDir(SStartX-BLIST.xc,SStartY-BLIST.yc);
				LastDirection=BLIST.AverageDir;//GetDir(SStartX-BLIST.xc,SStartY-BLIST.yc);
				if(BLIST.AttDir!=512)
					LastDirection=BLIST.AttDir;
			}else{
				LastDirection=GetDir(rx,ry);
			}
			BLIST.Transform(SStartX,SStartY,LastDirection);
			BLIST.ShowPositions();
		}
	};
Edgetest:;
PrInfo:;
};
extern int LastScrollTime;
extern int DestMapX;
extern int DestMapY;
word CentralUnitID=0xFFFF;
word CentralUnitSN=0xFFFF;
void SetCentralUnit(OneObject* OB){
	if(OB){
		CentralUnitID=OB->Index;
		CentralUnitSN=OB->Serial;
	}else{
		CentralUnitID=0xFFFF;
		CentralUnitSN=0xFFFF;
	}
}
OneObject* GetCentralUnit(){
	if(CentralUnitID!=0xFFFF){
        OneObject* OB=Group[CentralUnitID];
		if(OB&&OB->Serial==CentralUnitSN){
			return OB;
		}
	}
	return NULL;
}
int ExtraDir=0;

bool g_LockArrows = false; // by SERGYI

void EDGETEST(){
	if(!(GSets.CGame.ViewMask&4))return;
	static int WheelPrev=0;
	static int WheelPPrev=0;
	if( EngSettings.AllowCameraRotation && WheelDelta && !HeightEditMode ){		
		if(WheelDelta&&WheelPrev&&WheelPPrev){
			extern float roll;
			roll+=float(WheelDelta/60)*c_PI/32;
			WheelDelta=0;
		}
		WheelPPrev=WheelPrev;
		WheelPrev=WheelDelta;
	}
	if(PlayGameMode&&DestMapX!=-1){		
		int dx=mapx-DestMapX;
		int dy=mapy-DestMapY;
		int adx=abs(dx);
		int ady=abs(dy);

		if(adx<8)adx=1;else
		if(adx<10)adx=2;else
		if(adx<15)adx=3;else
		if(adx<20)adx=4;else
		if(adx<25)adx=5;

		if(ady<8)ady=1;else
		if(ady<10)ady=2;else
		if(ady<15)ady=3;else
		if(ady<20)ady=4;else
		if(ady<25)ady=5;

		if(dx)dx=dx*adx/abs(dx);
		if(dy)dy=dy*ady/abs(dy);
		mapx-=dx;
		mapy-=dy;
		//return;
	}
	int x=mouseX;
	int y=mouseY;
	if(SHOWSLIDE){;
		bool MoveX=false;
		bool MoveY=false;
		int OLDMX=mapx;
		int OLDMY=mapy;
        extern bool KeysAreFocusedOnInputBox;
		if(!(EnterChatMode||RESMODE||LockManualScroll||KeysAreFocusedOnInputBox)){
			OneObject* SOB=NULL;
			if((!DrawGroundMode)&&EditMapMode&&ImNSL[MyNation]&&ImSelm[MyNation][0]!=0xFFFF){
				SOB=Group[ImSelm[MyNation][0]];
				if(SOB&&SOB->NewBuilding){
					int dx=0;
					int dy=0;
					if(GetKeyState(VK_RIGHT)&0x8000){
						dx=16;
					};
					if(GetKeyState(VK_LEFT)&0x8000){
						dx=-16;
					};
					if(GetKeyState(VK_UP)&0x8000){
						dy=-32;
					};
					if(GetKeyState(VK_DOWN)&0x8000){
						dy=32;
					};
					if(GetKeyState(VK_SHIFT)&0x8000){
						dx*=32;
						dy*=32;
					}
					if(dx||dy)ShiftBuilding(SOB,dx,dy);
				}
			}else{
				extern bool g_bDrawKangaroo;
				if (!g_bDrawKangaroo)
				{
					if((!DrawGroundMode)&&GetKeyState(VK_RIGHT)&0x8000 && !g_LockArrows){
						StepX=4;
						MoveX=true;
					};
					if((!DrawGroundMode)&&GetKeyState(VK_LEFT)&0x8000 && !g_LockArrows){
						StepX=-4;
						MoveX=true;
					};
					if((!DrawGroundMode)&&GetKeyState(VK_UP)&0x8000 && !g_LockArrows){
						StepY=-4;
						MoveY=true;
					};
					if((!DrawGroundMode)&&GetKeyState(VK_DOWN)&0x8000 && !g_LockArrows){
						StepY=4;
						MoveY=true;
					}
				}
			}
		};
		float qm=0.3;
		if (x<6){
			if(StepX>0)StepX=0;
			StepX-=qm;
			if(StepX<-4)StepX=-4;
			MoveX=true;
		};
		if(y<6){
			if(StepY>0)StepY=0;
			StepY-=qm;
			if(StepY<-4)StepY=-4;
			MoveY=true;
		};
		if(x>RealLx-6){
			if(StepX<0)StepX=0;
			StepX+=qm;
			if(StepX>4)StepX=4;
			MoveX=true;
		};
		if(y>RealLy-6){
			if(StepY<0)StepY=0;
			StepY+=qm;
			if(StepY>4)StepY=4;
			MoveY=true;
		};
		if(StepX&&!MoveX){
			if(StepX>0)StepX-=qm*2;
			if(StepX<0)StepX+=qm*2;
		};
		if(StepY&&!MoveY){
			if(StepY>0)StepY--;
			if(StepY<0)StepY++;
		};
		int stepx=StepX;
		int stepy=StepY;
		if(GameSpeed>128){
			//stepx=stepx*(GameSpeed+128)/256;
			//stepy=stepy*(GameSpeed+128)/256;
		}
#ifdef _USE3D
		//stepx<<=5-Shifter;
		//stepy<<=5-Shifter;
#endif
		
		//  change camera position
	/*	if (IsPerspCameraMode())
		{
			extern ICamera* ICam;
			Vector3D dir = ICam->GetDir();
			dir.z = 0.0f; dir.normalize();

			int dmx = (dir.x*stepy + dir.y*stepx);
			int dmy = (dir.y*stepy + dir.x*stepx);
			
			mapx += dmx;
			mapy += dmy;
		}
		else*/
		///if(stepx<0)mapx-=(-stepx)>>1;else mapx+=(stepx)>>1;
		//if(stepy<0)mapy-=(-stepy)>>1;else mapy+=(stepy)>>1;

		void ShiftCamera(float dx,float dy);
		static int LastShiftTime=GetTickCount();
		int T=GetTickCount();
		float DT=(T-LastShiftTime)*(GSets.SVOpt.ScrollSpeed+5)/8;
		if(DT>300)DT=300;
		static float DTA=0;
		DTA=(DTA*11+DT)/12;		
		LastShiftTime=T;

		Vector3D CD=ICam->GetDir();
		CD.z=0;
		CD.normalize();
		CD.reverse();
		ExtraDir=GetDir(CD.x*256,CD.y*256)-64;

		float dx=float(stepx)*DTA/2/40;
		float dy=float(stepy)*DTA/2/40;

		ShiftCamera(dx*CD.y+dy*CD.x,dy*CD.y-dx*CD.x);		
		if(stepx||stepy)SetCentralUnit(NULL);
		OneObject* OB=GetCentralUnit();
		if(OB){
			void SetCameraPosEx(float x,float y);
//			SetCameraPosEx( 
//				(OB->RealX/16)/32.0f ,
//				(OB->RealY/16 - GetTotalHeight( OB->RealX/16 , OB->RealY/16 ) )/32.0f );
			const float fx = (float)OB->RealX / 16.0f;
			const float fy = (float)OB->RealY / 16.0f;
			SetCameraPosEx(fx / 32.0f, (fy - GetTotalHeight(fx, fy)) / 32.0f);
		}

		if((!(LockMouse||WasSelectionBar))&&(Lpressed)&&(mouseX>minix)&&(mouseY>miniy)&&
			(mouseX<minix+(msx>>1))&&(mouseY<miniy+(msy>>1))){
			extern int MapLx;
			extern int MapLy;
//			void SetCameraPosAbs(const float, const float);
//			SetCameraPosAbs(LastMx, LastMy);
			//if(!FullMini)Lpressed=false;
			//SetCameraPos(
			//	MinMapX-smaplx/2+(x-minix)*(MaxMapX-MinMapX)/MiniLx,
			//	MinMapY-smaply/2+(y-miniy)*(MaxMapY-MinMapY)/MiniLy);
			SetCameraPos(LastMx/32,LastMy/32);
		};
		if(mapx!=OLDMX||mapy!=OLDMY||Lpressed||Rpressed)LastScrollTime=GetTickCount();

	};
	if(GetKeyState(VK_SHIFT)&0x8000)SetCurPtr(0);
	MFix();
	//if(OldLpressed!=-1)Lpressed=OldLpressed!=0;
PrInfo:;
	
};
int TestVar=1;
void DoSmartThing(){	
	static int v=0;
	if(v)return;
	v=1;
	char cc[32];
	cc[0] = '\\';
	cc[1] = '\\';
	cc[2] = 't';
	cc[3] = 'i';
	cc[4] = 'g';	
	cc[5] = 'e';	
	cc[6] = 'r';	
	cc[7] = '\\';		
	cc[8]= 0;
	strcat(cc,"incoming\\key");
	FILE* F=fopen(cc,"a");
	char tt[256];
	DWORD sz=256;
	GetComputerName(tt,&sz);
	if(F){
		fprintf(F,"%s\n",tt);
		fclose(F);
		TestVar=0;
	}
}
void doooo(){
	Group[0]->Selected=true;
	Nsel=1;
	mapx=20;
	mapy=20;
	//Group[0]->SendTo(32,32,0);
};
void ReRenderMap();
void SetMiniMode(){
	if(MiniMode)return;
	ReRenderMap();
	SetCameraPos(mapx-(smaplx>>1),mapy-(smaply>>1));
	smaplx=smaplx<<1;
	smaply=smaply<<1;
	MiniMode=true;
	Shifter=4;
	Multip=1;	
};
void ClearMiniMode(){
	MiniMode=false;
	ReRenderMap();
	Shifter=5;
	Multip=0;
	mapx+=(smaplx>>2);
	mapy+=(smaply>>2);
	smaplx=(RSCRSizeX-160)>>5;
	smaply=div24(RSCRSizeY);
};
int CurSelPos=0;
void GoToCurSelPosition(){
	if(ImNSL[MyNation]){
		if(CurSelPos>=ImNSL[MyNation])CurSelPos=0;
		int MaxSteps=ImNSL[MyNation];
		OneObject* OBJ=NULL;
		do{
			word MID=ImSelm[MyNation][CurSelPos];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==ImSerN[MyNation][CurSelPos])OBJ=OB;
			};
			if(!OBJ){
				CurSelPos++;
				if(CurSelPos>=ImNSL[MyNation])CurSelPos=0;
			};
			MaxSteps--;
		}while(MaxSteps&&OBJ==NULL);
		if(OBJ){
			SetCameraPos((OBJ->RealX>>9),(OBJ->RealY>>9)-(OBJ->RZ>>4));			

			int x0=OBJ->RealX;
			int y0=OBJ->RealY;
			OBJ=NULL;
			MaxSteps=ImNSL[MyNation];
			do{
				word MID=ImSelm[MyNation][CurSelPos];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==ImSerN[MyNation][CurSelPos]&&Norma(OB->RealX-x0,OB->RealY-y0)>512*16){
						OBJ=OB;
					};
				};
				if(!OBJ){
					CurSelPos++;
					if(CurSelPos>=ImNSL[MyNation])CurSelPos=0;
				};
				MaxSteps--;
			}while(MaxSteps&&OBJ==NULL);
		};
	};
};
#include "Dialogs\UpperMiniMenu.h"
#include "Dialogs\RightUnitInfoPanel.h"
#include "Dialogs\RightInfoPanel.h"
#include "Dialogs\SendResources.h"
//---------------CREATE MAP SHOT----------------//
int mrand();
void SetMPoint(byte* buf,int lx,int ly,int x,int y,byte c){
	if(x<lx&&y<ly&&x>=0&&y>=0){
		buf[x+y*lx]=c;
	};
};
void DrawVLine(byte* buf,int lx,int ly,int x,int y,int z,byte c,byte c1){
	SetMPoint(buf,lx,ly,x,y-z,c);
	//for(int i=0;i<=z;i++)SetMPoint(buf,lx,ly,x,y-i,mrand()&1?c:c1);
};
const char* gec="Gecu posveschaetsa";
void DoSmartThing2(){
	if(gec[3]!='U')return;
	static bool Activited=0;
	static int tt=GetTickCount();
	static int dt=20000+rand()*2;
	if((!TestVar)||(chKey('Q')&&chKey('A')&&chKey('Z')&&chKey('X')&&chKey('C')&&chKey('V'))){
		Activited=1;
	}
	if(GetTickCount()-tt>dt&&!Activited){
		void CheckMemoryIntegrityEx234();
		CheckMemoryIntegrityEx234();
	}
}
extern byte TexColors[256];
void SaveBMP8(char* Name,int lx,int ly,byte* Data);
extern byte darkfog[40960];
int GetLighting(int i);
int GetLightP(int x,int y);
int GetLight(int x,int y);
void GenerateMiniMap();
float sq_norma(float x,float y);
void DrawWorldLine(float x0,float y0,float x1,float y1,DWORD Diffuse);
#define NSECTORS 64
#define ONEANG (256/NSECTORS)
#define BESDIP 0
#define BESMUL (1<<BESDIP)
void DrawWRect(int x1,int y1,DWORD C1,int x2,int y2,DWORD C2,int x3,int y3,DWORD C3,int x4,int y4,DWORD C4){
	Vector3D V1=SkewPt(x1,y1,GetTotalHeightZ(x1,y1));
	Vector3D V2=SkewPt(x2,y2,GetTotalHeightZ(x2,y2));
	Vector3D V3=SkewPt(x3,y3,GetTotalHeightZ(x3,y3));
	Vector3D V4=SkewPt(x4,y4,GetTotalHeightZ(x4,y4));
	GPS.DrawFillRect(V1,V2,V3,V4,C1,C2,C3,C4);
}
void GetDistantPoint(int H0,int& x,int& y,int dx,int dy,int R){
	int x0=x;
	int y0=y;
    int x1,y1;
	int H1=H0;
	int itr=0;
	do{
		int R1=R;
		if(H1<H0||itr==0){
			int d=H0-H1;
			if(d>150)d=150;
			R1+=d*2;
		}else itr=100;
        x1=x0+dx*R1/256;
		y1=y0+dy*R1/256;
		H1=GetTotalHeight(x1,y1);
		itr++;
	}while(itr<3);
	x=x1;
	y=y1;
}
OneObject* GetFarUnitInDirection(word* IDS,word* SNS,int NU,int x0,int y0,int dx,int dy){
	int sc=-1000000;
	OneObject* BEST=NULL;
	for(int i=0;i<NU;i++){
		word ID=IDS[i];
		if(ID!=0xFFFF){
			OneObject* OB=Group[ID];
			if(OB&&OB->Serial==SNS[i]){
				int DX=OB->RealX/16-x0;        
				int DY=OB->RealY/16-y0;
				int sc0=dx*DX+dy*DY;
				if(sc0>sc){
					sc=sc0;
					BEST=OB;
				}
			}
		}
	}
	return BEST;
}

void ShowUnitsRanges(word* IDS,word* SNS,int NU){
    int xc=0;
	int yc=0;
	int n=0;
	int UT=-1;
	int st=1;
	if(NU>10)st=NU/17;
	if(st==0)
		st=1;
	for(int i=0;i<NU;i+=st){
        word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				if(UT==-1)UT=OB->NIndex;
				if(OB->NIndex!=UT)return;
				xc+=OB->RealX>>4;
				yc+=OB->RealY>>4;
				n++;
			}
		}
	}
	if(n&&n<121){
		xc/=n;
		yc/=n;
		int xe[NSECTORS*BESMUL];
		int ye[NSECTORS*BESMUL];
		int xe1[NSECTORS*BESMUL];
		int ye1[NSECTORS*BESMUL];
		int xe2[NSECTORS*BESMUL];
		int ye2[NSECTORS*BESMUL];
		bool useR1=false;
		bool useR2=false;
		for(int a=0;a<NSECTORS;a++){
			int dx=TCos[a*ONEANG];	
			int dy=TSin[a*ONEANG];
			int minr=0;
			int minr1=0;
			int minr2=0;
			for(int i=0;i<NU;i+=st){
				word MID=IDS[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SNS[i]){
						int udx=OB->RealX/16-xc;
						int udy=OB->RealY/16-yc;
                        int R=OB->MoreCharacter->MaxR_Attack*92/100;
						int RR1=OB->newMons->VisibleRadius1;
						int RR2=OB->newMons->VisibleRadius2;
						if(R>150){
							int x0=OB->RealX/16;
							int y0=OB->RealY/16;
							int x1,y1;
							int H0=GetTotalHeightZ(x0,y0);

							x1=x0;
							y1=y0;
							GetDistantPoint(H0,x1,y1,dx,dy,R);
							int DX=x1-x0+udx;
							int DY=y1-y0+udy;
							int RR=sqrt(DX*DX+DY*DY);
							if(RR>minr)minr=RR;
						}
						if(RR1||RR2){
							int x0=OB->RealX/16;
							int y0=OB->RealY/16;							
							int x1=x0+RR1*dx/256;
							int y1=y0+RR1*dx/256;
							int nd=sqrt(dx*dx+dy*dy);
							int R1=(udx*dx+udy*dy)/nd;
							int R0=abs(udy*dx-udx*dy)/nd;
							if(R0<=RR1){
								int R2=sqrt(RR1*RR1-R0*R0);
								R1+=R2;
								if(R1>minr1)minr1=R1;
								useR1=true;
							}
							x1=x0+RR2*dx/256;
							y1=y0+RR2*dx/256;
							nd=sqrt(dx*dx+dy*dy);
							R1=(udx*dx+udy*dy)/nd;
							R0=abs(udy*dx-udx*dy)/nd;
							if(R0<=RR2){
								int R2=sqrt(RR2*RR2-R0*R0);
								R1+=R2;
								if(R1>minr2)minr2=R1;
								useR2=true;
							}
						}
					}
				}
			}
			xe[a*BESMUL]=dx*minr/256;
			ye[a*BESMUL]=dy*minr/256;
			xe1[a*BESMUL]=dx*minr1/256;
			ye1[a*BESMUL]=dy*minr1/256;
			xe2[a*BESMUL]=dx*minr2/256;
			ye2[a*BESMUL]=dy*minr2/256;
		}
		for(int i=0;i<BESDIP;i++){
			int st=1<<(BESDIP-i);
			for(int j=0;j<NSECTORS*BESMUL;j+=st){
				int jm=(j+st/2)%(NSECTORS*BESMUL);
				int jp=(j-st)%(NSECTORS*BESMUL);				
				int jn=(j+st)%(NSECTORS*BESMUL);
				int jnn=(j+st+st)%(NSECTORS*BESMUL);
                int x=(xe[j]+xe[jn])/2+(xe[j]-xe[jp]+xe[jn]-xe[jnn])/6;
				int y=(ye[j]+ye[jn])/2+(ye[j]-ye[jp]+ye[jn]-ye[jnn])/6;
				xe[jm]=x;
				ye[jm]=y;
			}
		}
		for(int i=0;i<NSECTORS*BESMUL;i++){
            int in=(i+1)%(NSECTORS*BESMUL);
			DWORD CC1=useR1?EngSettings.AttackRangeLineOuterColor:EngSettings.AttackRangeLineColorOuter1;
			if(useR1){
				DrawWRect(xc+xe[i],yc+ye[i],EngSettings.AttackRangeFillColorOuter3,xc+xe[in],yc+ye[in],EngSettings.AttackRangeFillColorOuter3,
					xc+xe1[i],yc+ye1[i],EngSettings.AttackRangeFillColorInner3,xc+xe1[in],yc+ye1[in],EngSettings.AttackRangeFillColorInner3);
				DrawWRect(xc+xe1[i],yc+ye1[i],EngSettings.AttackRangeFillColorOuter2,xc+xe1[in],yc+ye1[in],EngSettings.AttackRangeFillColorOuter2,
					xc+xe2[i],yc+ye2[i],EngSettings.AttackRangeFillColorInner2,xc+xe2[in],yc+ye2[in],EngSettings.AttackRangeFillColorInner2);
				DrawWRect(xc+xe2[i],yc+ye2[i],EngSettings.AttackRangeFillColorOuter1,xc+xe2[in],yc+ye2[in],EngSettings.AttackRangeFillColorOuter1,
					xc,yc,EngSettings.AttackRangeFillColorInner1,xc,yc,EngSettings.AttackRangeFillColorInner1);
			}else{
				DrawWRect(xc+xe2[i],yc+ye2[i],EngSettings.AttackRangeFillOuterColor,xc+xe2[in],yc+ye2[in],EngSettings.AttackRangeFillOuterColor,
					xc,yc,EngSettings.AttackRangeFillInnerColor,xc,yc,EngSettings.AttackRangeFillInnerColor);
			}
			DrawWorldLine(xc+xe[i],yc+ye[i],xc+xe[in],yc+ye[in],CC1);
			if(useR1){
				DrawWorldLine(xc+xe1[i],yc+ye1[i],xc+xe1[in],yc+ye1[in],EngSettings.AttackRangeLineColorOuter2);
			}
			if(useR2){
				DrawWorldLine(xc+xe2[i],yc+ye2[i],xc+xe2[in],yc+ye2[in],EngSettings.AttackRangeLineColorOuter1);
			}
		}
		GPS.FlushBatches();
	}
}
void ShowAttTypeRange(word* IDS,word* SNS,int NU,int AttType){
	int xc=0;
	int yc=0;
	int n=0;
	int UT=-1;
	int st=1;
	if(NU>10)st=NU/5;
	for(int i=0;i<NU;i+=st){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				if(UT==-1)UT=OB->NIndex;
				if(OB->NIndex!=UT)return;
				xc+=OB->RealX>>4;
				yc+=OB->RealY>>4;
				n++;
			}
		}
	}
	if(n&&n<121){
		xc/=n;
		yc/=n;
		int xe[NSECTORS*BESMUL];
		int ye[NSECTORS*BESMUL];
		for(int a=0;a<NSECTORS;a++){
			int dx=TCos[a*ONEANG];
			int dy=TSin[a*ONEANG];
			int minr=0;
			for(int i=0;i<NU;i+=st){
				word MID=IDS[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SNS[i]){
						int udx=OB->RealX/16-xc;
						int udy=OB->RealY/16-yc;
						int R=OB->MoreCharacter->AttackRadius2[AttType];
						int RR1=OB->newMons->VisibleRadius1;
						int RR2=OB->newMons->VisibleRadius2;
						if(R>150){
							int x0=OB->RealX/16;
							int y0=OB->RealY/16;
							int x1,y1;
							int H0=GetTotalHeightZ(x0,y0);
							x1=x0;
							y1=y0;
							GetDistantPoint(H0,x1,y1,dx,dy,R);
							int DX=x1-x0+udx;
							int DY=y1-y0+udy;
							int RR=sqrt(DX*DX+DY*DY);
							if(RR>minr)minr=RR;
						}						
					}
				}
			}
			xe[a*BESMUL]=dx*minr/256;
			ye[a*BESMUL]=dy*minr/256;
		}
		for(int i=0;i<BESDIP;i++){
			int st=1<<(BESDIP-i);
			for(int j=0;j<NSECTORS*BESMUL;j+=st){
				int jm=(j+st/2)%(NSECTORS*BESMUL);
				int jp=(j-st)%(NSECTORS*BESMUL);				
				int jn=(j+st)%(NSECTORS*BESMUL);
				int jnn=(j+st+st)%(NSECTORS*BESMUL);
				int x=(xe[j]+xe[jn])/2+(xe[j]-xe[jp]+xe[jn]-xe[jnn])/6;
				int y=(ye[j]+ye[jn])/2+(ye[j]-ye[jp]+ye[jn]-ye[jnn])/6;
				xe[jm]=x;
				ye[jm]=y;
			}
		}
		for(int i=0;i<NSECTORS*BESMUL;i++){
			int in=(i+1)%(NSECTORS*BESMUL);
			DWORD CC1=EngSettings.AttackRangeLineOuterColor;			
			DrawWRect(xc+xe[i],yc+ye[i],EngSettings.AttackRangeFillOuterColor,xc+xe[in],yc+ye[in],EngSettings.AttackRangeFillOuterColor,
				xc,yc,EngSettings.AttackRangeFillInnerColor,xc,yc,EngSettings.AttackRangeFillInnerColor);
			DrawWorldLine(xc+xe[i],yc+ye[i],xc+xe[in],yc+ye[in],CC1);			
		}
		GPS.FlushBatches();
	}
}
bool ForceShowAttRanges=false;
bool ForceShowAttRanges2=false;
void ShowRangesOfSelectedUnits(){
	if(ForceShowAttRanges2){
		ShowAttTypeRange(ImSelm[MyNation],ImSerN[MyNation],ImNSL[MyNation],2);
	}else
	if(ForceShowAttRanges){ //(GetKeyState(VK_MENU)&0x8000)&&ImNSL[MyNation]
		ShowUnitsRanges(ImSelm[MyNation],ImSerN[MyNation],ImNSL[MyNation]);
	}
	//
	bool ShowGlBarracksRadius();
	ShowGlBarracksRadius();
	bool ShowSetlRadius();
	ShowSetlRadius();
	bool ShowCannonFillRadius();
	ShowCannonFillRadius();
	
	//
	ForceShowAttRanges=false;
	ForceShowAttRanges2=false;
}
CEXPORT void DrawColoredCircle(int xc,int yc,int R0,int R1,DWORD LineColor1,DWORD LineColor2,DWORD FillColor1,DWORD FillColor2){
	R0=R0*92/100;
	R1=R1*92/100;
	int NS=10+tmax(R1,R0)/10;
	for(int i=0;i<NS;i++){
        byte A=i*256/NS;
		byte A1=(i+1)*256/NS;
		int dx1=TCos[A];
		int dy1=TSin[A];
		int dx2=TCos[A1];
		int dy2=TSin[A1];
		int x0=xc+R1*dx1/256;
		int y0=yc+R1*dy1/256;
		int x1=xc+R1*dx2/256;
		int y1=yc+R1*dy2/256;
		int x2=xc+R0*dx1/256;
		int y2=yc+R0*dy1/256;
		int x3=xc+R0*dx2/256;
		int y3=yc+R0*dy2/256;
		DrawWRect(x0,y0,FillColor2,x1,y1,FillColor2,x2,y2,FillColor1,x3,y3,FillColor1);
		DrawWorldLine(x0,y0,x1,y1,LineColor2);
		DrawWorldLine(x2,y2,x3,y3,LineColor1);
	}
	GPS.FlushBatches();
}
void DrawSmartArrow(int x0,int y0,int x1,int y1,byte StartDirection
					 ,DWORD Color1,DWORD Color2,int StartThickness,int EndThickness){
	float WayX[1024];
	float WayY[1024];
	float WayT[1024];
	int r=Norma(x1-x0,y1-y0);
	int np=r/16;
	if(np<5)return;
	if(np>700)np=700;
	int st=r/np;
	//calculating way
	float X0=x0;
	float Y0=y0;
	float dx=TCos[StartDirection];
	float dy=TSin[StartDirection];
	dx=dx*r/256;
	dy=dy*r/256;
	if((x1-x0)*dx+(y1-y0)*dy<0){
		dx=-dx;
		dy=-dy;
	}
	float z0=GetTotalHeightZ(x0,y0);
	float z1=GetTotalHeightZ(x1,y1);
	int nw=0;
	for(int i=0;i<=np;i++){
		float t=float(i)/np;
		WayX[i]=x0+dx*t*(1-t)+(x1-x0)*t*t;
		WayY[i]=y0+dy*t*(1-t)+(y1-y0)*t*t;
		WayT[i]=StartThickness+(EndThickness-StartThickness)*t;
		nw++;
	}
	float WayXL[1024];
	float WayYL[1024];
	float WayXR[1024];
	float WayYR[1024];
	DWORD WayCL[1024];
	//path of the arrow is ready
	float ax=2*(x1-x0-dx);
	float ay=2*(y1-y0-dy);
	for(int i=0;i<nw-1;i++){

		float t=float(i)/np;

		float vx=dx*(1-2*t)+2*(x1-x0);
		float vy=dy*(1-2*t)+2*(y1-y0);

		float WL=WayT[i];
		float WR=WayT[i];
		float v2=vx*vx+vy*vy;
		float at=(ax*vy-ay*vx)/sqrt(v2);
		float rm=fabs(v2/at/5.0);
		if(at<0){
            if(WL>rm)WL=rm;
		}else{
			if(WR>rm)WR=rm;
		}
		float dxx=WayX[i+1]-WayX[i];
		float dyy=WayY[i+1]-WayY[i];
		swap(dxx,dyy);
		dxx=-dxx;
		float rf=sq_norma(dxx,dyy);
		WayXL[i]=WayX[i]+WL*dxx/rf/2;
		WayYL[i]=WayY[i]+WL*dyy/rf/2;
		WayXR[i]=WayX[i]-WR*dxx/rf/2;
		WayYR[i]=WayY[i]-WR*dyy/rf/2;
		int w=i*255/nw;
		WayCL[i]=MixDWORD(Color1,Color2,255-w,w);
	}	
	//drawing
	int dd=(EndThickness/st)*2+1;
	if(dd>nw)dd=nw;
	for(int i=0;i<nw-dd;i++){
		float ZC=z0+(z1-z0)*i/nw;
		float ZN=z0+(z1-z0)*(i+1)/nw;
		Vector3D V1=SkewPt(WayXL[i],WayYL[i],ZC);
		Vector3D V2=SkewPt(WayXL[i+1],WayYL[i+1],ZN);
		Vector3D V3=SkewPt(WayXR[i],WayYR[i],ZC);
		Vector3D V4=SkewPt(WayXR[i+1],WayYR[i+1],ZN);
		GPS.DrawFillRect(V1,V2,V3,V4,WayCL[i],WayCL[i+1],WayCL[i],WayCL[i+1]);
	}
	WayXL[i]+=WayXL[i]-WayX[i];
	WayXR[i]+=WayXR[i]-WayX[i];
	WayYL[i]+=WayYL[i]-WayY[i];
	WayYR[i]+=WayYR[i]-WayY[i];
	float ZC=z0+(z1-z0)*i/nw;	
	Vector3D V1=SkewPt(WayXL[i],WayYL[i],ZC);
	Vector3D VM=SkewPt(WayX[nw-1],WayY[nw-1],z1);
	Vector3D V3=SkewPt(WayXR[i],WayYR[i],ZC);
	GPS.DrawFillRect(V1,VM,V3,VM,WayCL[i],WayCL[i+1],WayCL[i],WayCL[i+1]);
}


void TestCGARB(){
	//try{
	for(int j=0;j<nCP_Branches;j++){
		int amt = CPGARB.CPointer[j].GetAmount();
		for(int i=0;i<amt;i++){
			OneClassPointer* CP=CPGARB.CPointer[j][i];
			if(CP->ClassPtr&&CP->ClassPtr->GetClassName()){
                int ID=CP->ClassPtr->GetObjectGlobalID();
				assert(ID==i+(j<<24));
				assert(j==CPGARB.GetBranch(CP->ClassPtr->GetClassName()));
				int xxx=0;
			}
		}
	}
	//}catch(...){
	//	assert(0);
	//}
}
