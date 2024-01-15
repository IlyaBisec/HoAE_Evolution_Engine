#include "stdheader.h"
#include "DrawModels.h"
#include "kContext.h"
#include "IShadowManager.h"
#include "unitability.h"
#include "kStaticArray.hpp"
#include "TreesMesh.h"
#include ".\cvi_MainMenu.h"
#include "CurrentMapOptions.h"
#include "unichash.h"

float BlendingTime = 0;//1.5f;

#ifdef _USE3D
#pragma pack (push)
#pragma pack (8)

#include "Scape3D.h"
#pragma pack (pop)
#endif // _USE3D

#define M4CellLX 32
#define M4CellLY 32

byte* Tmp4XBuf=NULL;
short* TmpOwnerX=NULL;
short* TmpOwnerY=NULL;

int CurTmpPos=0;
int MaxTmpPos=0;

byte** ReadyMap=NULL;
word*  PosMap=NULL;
byte * MiniMap=NULL;
//byte*  ReadyMap=NULL;
int OFSTBL[4096];
int MM_Lx;
int MM_Ly;
int MM_Y0;
int MMSZ=0;
int MMR_Lx;
int MMR_Ly;
//V1--\
//     ----\
//          ===V3
//     ----/
//V2--/
extern short randoma[8192];
int GPLP=0;
#define SETPX1
#define SETPX
#define SETLIT
	
/*

#define SETPX \
	off=ofs-OFSTBL[H>>8];\
	c=MiniMap[off];\
	if(V>128)c=TexPtr[tofs];\
	MiniMap[off]=c;//darkfog[16384+c+(L&0xFF00)];\
	assert(V>=-3);
*/

DWORD GetNatColor( int );

/*---------------------------------------------------------------------------*/
/*	Func:	FindRelativeZ	
/*	Desc:	Projects point (px, py) from screen space onto the ground plane
/*			and finds its z-value relatively to the (cx, cy, 0)
/*---------------------------------------------------------------------------*/
float FindRelativeZ( float px, float py, float cx, float cy )
{
	return c_Cos30 * (cy - py); 
}

int mrand();
void DrawTriangleR(int x,int y,int H1,int H2,int H3,
				   int L1,int L2,int L3,int V1,int V2,int V3,
				   byte* TexPtr){
};
//            V1
//      .     .
//V3          .
//      .     .
//            V2
void DrawTriangleL(int x,int y,int H1,int H2,int H3,
				   int L1,int L2,int L3,int V1,int V2,int V3,
				   byte* TexPtr){	
};
void LiteTriangleR(int x,int y,int H1,int H2,int H3,
				   int L1,int L2,int L3){

};
//            V1
//      .     .
//V3          .
//      .     .
//            V2
void LiteTriangleL(int x,int y,int H1,int H2,int H3,
				   int L1,int L2,int L3){	
};
#define VMAX1 (255-(randoma[X1&8191]&127))
#define VMAX2 (255-(randoma[X2&8191]&127))
#define VMAX3 (255-(randoma[X3&8191]&127))
#define VMIN1 (randoma[X1&8191]&127)
#define VMIN2 (randoma[X2&8191]&127)
#define VMIN3 (randoma[X3&8191]&127)
int GetBmOfst(int i);
void DrawTriR(int x,int y,
			  int H1,int H2,int H3,
			  int L1,int L2,int L3,
			  int T1,int T2,int T3,
			  int X1,int X2,int X3){};
void DrawTriL(int x,int y,
			  int H1,int H2,int H3,
			  int L1,int L2,int L3,
			  int T1,int T2,int T3,
			  int X1,int X2,int X3){};
void SaveBMP8(char* Name,int lx,int ly,byte* Data);
void DrawMiniMaskOnScreen(byte* Buf,int x,int y,int cx,int cy,int BufWidth);
void DrawMiniSubWaterSquare(byte* Buf,int x,int y,int cx,int cy,int BufWidth);
extern int MAXSPR;
struct OneSprInfo{
	short x,y,file,spr;
};
#define MAXLY 3000
class SmallZBuffer{
public:
	int MaxLines;
	OneSprInfo* SPRS[MAXLY];
	int NSprs[MAXLY];
	int MaxSprs[MAXLY];
	SmallZBuffer(int Ny);
	~SmallZBuffer();
	void Add(int y,int sx,int sy,int FID,int spr);
	void Draw();
};
SmallZBuffer::SmallZBuffer(int Ny){
	//memset(this,0,sizeof* this);
	//SPRS=(OneSprInfo**)malloc(4*Ny);
	memset(SPRS,0,4*Ny);
	//NSprs=(int*)malloc(4*Ny);
	memset(NSprs,0,4*Ny);
	//MaxSprs=(int*)malloc(4*Ny);
	memset(MaxSprs,0,4*Ny);
	MaxLines=Ny;
};
SmallZBuffer::~SmallZBuffer(){
	for(int i=0;i<MaxLines;i++){
		if(SPRS[i])free(SPRS[i]);
		SPRS[i]=NULL;
	};
	//if(SPRS)free(SPRS);
	//if(NSprs)free(NSprs);
	//if(MaxSprs)free(MaxSprs);
};
void SmallZBuffer::Add(int y,int sx,int sy,int FID,int spr){
	if(y>=0&&y<MaxLines){
		if(NSprs[y]>=MaxSprs[y]){
			MaxSprs[y]+=4;
			SPRS[y]=(OneSprInfo*)realloc(SPRS[y],MaxSprs[y]*sizeof OneSprInfo);
		};
		OneSprInfo* OSI=SPRS[y]+NSprs[y];
		NSprs[y]++;
		OSI->file=FID;
		OSI->spr=spr;
		OSI->x=sx;
		OSI->y=sy;
	};
};
int NDR=0;
void SmallZBuffer::Draw(){
	NDR=0;
	for(int i=0;i<MaxLines;i++){
		int N=NSprs[i];
		if(N){
			OneSprInfo* OSI=SPRS[i];
			for(int j=0;j<N;j++){
				GPS.ShowGP(OSI[j].x,OSI[j].y,OSI[j].file,OSI[j].spr,0);
				NDR++;
			};
		};
	};
};
int RSIZE;
void GenerateMiniMap(){
//#ifdef _USE3D
//	return;
//#endif // _USE3D

	TempWindow TW;
	PushWindow(&TW);
	void* sptr=ScreenPtr;
	MM_Lx=(1920<<ADDSH)+2;
	MM_Ly=(MM_Lx>>1)+2;
	MM_Y0=64;
	MMR_Lx=(MM_Lx/M4CellLX)+1;
	MMR_Ly=(MM_Ly/M4CellLY)+1;
	if(!Tmp4XBuf){
		MaxTmpPos=8000000/(M4CellLX*M4CellLY);
		Tmp4XBuf=(byte*)malloc(MaxTmpPos*M4CellLX*M4CellLY);
		TmpOwnerX=(short*)malloc(MaxTmpPos*2);
		TmpOwnerY=(short*)malloc(MaxTmpPos*2);
	};
	if(!ReadyMap){
		ReadyMap=(byte**)malloc(MMR_Lx*MMR_Ly*4);
		PosMap=(word*)malloc(MMR_Lx*MMR_Ly*2);
	};
	memset(ReadyMap,0,MMR_Lx*MMR_Ly*4);
	memset(PosMap,0xFF,MMR_Lx*MMR_Ly*2);
	memset(TmpOwnerX,0xFF,MaxTmpPos*2);
	memset(TmpOwnerY,0xFF,MaxTmpPos*2);
	CurTmpPos=0;
	MMSZ=MM_Lx*(MM_Ly*2+MM_Y0*2);
	RSIZE=MMR_Lx*MMR_Ly;
	return;
};

byte* ReserveMemoryFor4X(word x,word y){
	if(TmpOwnerX[CurTmpPos]!=-1){
		int ofs=int(TmpOwnerX[CurTmpPos])+int(TmpOwnerY[CurTmpPos])*MMR_Lx;
		//assert(ofs>=0&&ofs<RSIZE);
		ReadyMap[ofs]=NULL;
		PosMap[ofs]=0xFFFF;
	};
	TmpOwnerX[CurTmpPos]=x;
	TmpOwnerY[CurTmpPos]=y;
	byte* mem=Tmp4XBuf+CurTmpPos*M4CellLX*M4CellLY;
	int ofs=int(x)+int(y)*MMR_Lx;
	ReadyMap[ofs]=mem;
	PosMap[ofs]=CurTmpPos;
	CurTmpPos++;
	if(CurTmpPos>=MaxTmpPos)CurTmpPos=0;
	return mem;
};

bool LMode=0;
extern int RealLx;
extern int RealLy;
void PrepareSound();


void MakeAllDirtyGBUF(){
}
void ReverseLMode();
void SetLMode(int L){
	if(LMode){
		ReverseLMode();
	};
	if(L){
		int dx=mapx+(mouseX>>5);
		int dy=mapy+(mouseY>>4);
		Shifter=5-L;
		LMode=1;
		//if(!ReadyMap)GenerateMiniMap();
		smaplx<<=(5-Shifter);
		smaply<<=(5-Shifter);
		smapy=RealLy-(smaply*(16>>(5-Shifter)));

		mapx=dx-(smaplx>>1);
		mapy=dy-(smaply>>1);
		if(mapx<=0)mapx=1;
		if(mapy<=0)mapy=1;
		if(mapx+smaplx>msx+1)mapx=msx-smaplx+1;
		if(mapy+smaply>msy+1)mapy=msy-smaply+1;
		SetCursorPos(RealLx>>1,RealLy>>1);
		MakeAllDirtyGBUF();
	};
	mapx=mapx&0xFFFC;
	mapy=mapy&0xFFFC;
	PrepareSound();
};

void RebuildWaterMesh();
void SetupCamera();

void ApplyLMode()
{
    int dx = mapx+(mouseX>>5);
    int dy = mapy+(mouseY>>4);
    Shifter = 4;
    if (GetKeyState(VK_SHIFT)&0x8000) Shifter = 2;
    LMode = 1;
    smaplx <<= (5-Shifter);
    smaply <<= (5-Shifter);
    smapy = RealLy - (smaply*(16>>(5-Shifter)));

    mapx=dx-(smaplx>>1);
    mapy=dy-(smaply>>1);
    if(mapx<=0)mapx=1;
    if(mapy<=0)mapy=1;
    if(mapx+smaplx>msx+1)mapx=msx-smaplx+1;
    if(mapy+smaply>msy+1)mapy=msy-smaply+1;
}

void UnapplyLMode()
{
    int dx=mapx+(mouseX>>(Shifter));
    int dy=mapy+(mouseY>>(Shifter-1));
    LMode=0;
    smaplx>>=(5-Shifter);
    smaply>>=(5-Shifter);
    smapy=RealLy-smaply*16;
    Shifter=5;
    mapx=dx-(smaplx>>1);
    mapy=dy-(smaply>>1);
    if(mapx<=0)mapx=1;
    if(mapy<=0)mapy=1;
    if(mapx+smaplx>msx+1)mapx=msx-smaplx+1;
    if(mapy+smaply>msy+1)mapy=msy-smaply+1;
}

void ReverseLMode(){
    bool bToL;
	if(!LMode){
		ApplyLMode();
        bToL = true;
	}else bToL = false;
		
	mapx = mapx&0xFFFC;
	mapy = mapy&0xFFFC;
    PrepareSound();

    Vector3D moveDir( Vector3D::null );    
    Vector3D start = ScreenToWorldSpace( RealLx/2, RealLy/2 );
    Vector3D end   = ScreenToWorldSpace( mouseX, mouseY );
    moveDir.sub( end, start );
    AnimateLModeSwitch( moveDir, bToL );
    SetupCamera();
    RebuildWaterMesh();
    FillGroundZBuffer();
    AnimateLModeSwitch( moveDir, bToL );
    //SetCursorPos(RealLx>>1,RealLy>>1);
}; // ReverseLMode

extern int RealLx;
extern int RealLy;
int DD=0;
void CheckMM_Change();
void CheckMM4XReady(int x,int y,int Lx,int Ly);
void CopyTo16(int x,int y,byte* Src,int Pitch,int Lx,int Ly);
void DrawOneTerrSquare(byte* offs,int x,int y){

};

void DrawLGround(){
	CheckMM_Change();
	//if(GetKeyState('Q')&0x8000)DD+=4;
	//if(GetKeyState('W')&0x8000)DD-=4;
	int x0=mapx<<3;
	int y0=mapy<<2;
	int Lx=RealLx-DD;
	int Ly=RealLy;
	CheckMM4XReady(x0,y0,Lx,Ly);
	int cx0=((mapx<<3)/M4CellLX)-1;
	int cy0=((mapy<<2)/M4CellLY)-1;
	int cx1=(((mapx+smaplx)<<3)/M4CellLX)+1;
	int cy1=(((mapy+smaply)<<2)/M4CellLY)+1;
	if(cx0<0)cx0=0;
	if(cy0<0)cy0=0;
	if(cx1>=MMR_Lx)cx1=MMR_Lx-1;
	if(cy1>=MMR_Ly)cy1=MMR_Ly-1;
	for(int ix=cx0;ix<=cx1;ix++){
		for(int iy=cy0;iy<=cy1;iy++){
			int ofs=ix+iy*MMR_Lx;
			if(ReadyMap[ofs])DrawOneTerrSquare(ReadyMap[ofs],ix*M4CellLX-x0,iy*M4CellLY-y0);
		};
	};
};
#define zoomsh (5-(Shifter))
int CLM_Size=8;
int CLM_Shift=2;
int CLM_ShiftY=2;

extern int tmtmt;
#define zoom(x) ((x)>>(5 - (Shifter)))
extern int PortBuiX,PortBuiY;
extern bool NOPAUSE;
void ShowFires(OneObject* OB,int x0,int y0);
void DRAW_WAVES();
void PROSESS_WAVES();
void ShowProducedShip(OneObject* Port,int CX,int CY);

void AddPointEx(short XL,short YL,short x,short y,OneObject* OB,word FileID,word SpriteID,word FileIDex,word SpriteIDex,int Param1,int Param2);
void SetZBias(int b);

int GT0,GT1,GT2,GT3,GT4,GT5;
int PREVLX=0;
byte* BUF=NULL;
int BUFSZ=0;
int StnGP=-1;
int DerGP=-1;
int HolesGP=-1;
void GenerateMiniMapSquare(int x0,int y0,int nx,int ny){
//#ifdef _USE3D
//	return;
//#endif // _USE3D

	if(StnGP==-1){
		StnGP=GPS.PreLoadGPImage("L_Mode\\Stones");
		DerGP=GPS.PreLoadGPImage("L_Mode\\Der");
		HolesGP=GPS.PreLoadGPImage("L_Mode\\Holes");
	};
	int T0=GetTickCount();
	//if(!MiniMap)return;
	
	byte* mptr0=MiniMap;
	
	int LX0=MM_Lx;
	int LY0=MM_Ly;
	int Y0=MM_Y0;
	MM_Lx=nx<<4;
	MM_Ly=ny<<2;
	MM_Y0=256;

	MMSZ=MM_Lx*(MM_Ly+MM_Y0)*2;
	byte* mpart;
	if(MMSZ>BUFSZ){
		mpart=(byte*)realloc(BUF,MMSZ);
		BUF=mpart;
		BUFSZ=MMSZ;
	}else mpart=BUF;
	MiniMap=mpart;

	if(PREVLX!=MM_Lx){
		for(int i=0;i<2000;i++)OFSTBL[i]=(i-64)*MM_Lx;
		PREVLX=MM_Lx;
	};

	TempWindow TW;
	PushWindow(&TW);
	void* sptr=ScreenPtr;

	memset(mpart,0,MMSZ);
	int NX0=x0<<1;
	int NY0=y0;
	int NLX=MM_Lx>>4;
	int NLY=MM_Ly>>2;
	int MYY=(NLY)<<2;
	int ND=0;
	int NND=0;
	GT0=GetTickCount()-T0;
	T0=GetTickCount();
	bool F1=1;
	bool F2=0;
	for(int iy=0;iy<NLY+40&&(F1||F2);iy++){
		F2=0;
		for(int ix=0;ix<NLX;ix++){
			int YY=NY0+iy;
			if(YY>=0&&YY<MaxTH-2){
				int ofs=(NY0+iy)*VertInLine+ix+ix+NX0;
				int VR0=ofs;
				int VR1=ofs+1;
				int VR2=ofs+2;
				int VR3=ofs+VertInLine;
				int VR4=ofs+VertInLine+1;
				int VR5=ofs+VertInLine+2;
				int H0=THMap[VR0]>>1;
				int H1=THMap[VR1]>>1;
				int H2=THMap[VR2]>>1;
				int H3=THMap[VR3]>>1;
				int H4=THMap[VR4]>>1;
				int H5=THMap[VR5]>>1;
				if(H0<0)H0=0;
				if(H1<0)H1=0;
				if(H2<0)H2=0;
				if(H3<0)H3=0;
				if(H4<0)H4=0;
				if(H5<0)H5=0;

				int T0=TexMap[VR0];
				int T1=TexMap[VR1];
				int T2=TexMap[VR2];
				int T3=TexMap[VR3];
				int T4=TexMap[VR4];
				int T5=TexMap[VR5];
				int L0=GetLighting(VR0);
				int L1=GetLighting(VR1);
				int L2=GetLighting(VR2);
				int L3=GetLighting(VR3);
				int L4=GetLighting(VR4);
				int L5=GetLighting(VR5);
				int X0=ix<<4;
				int Y0=iy<<3;
				int YY=Y0>>1;
				int RY0=YY-H0;
				int RY1=YY-2-H1;
				int RY2=YY-H2;
				int RY3=YY+4-H3;
				int RY4=YY+2-H4;
				int RY5=YY+4-H5;
				if(RY0<MYY||RY1<MYY||RY2<MYY||RY3<MYY||RY4<MYY||RY5<MYY){
					F2=1;
					F1=0;
					DrawTriR(X0,Y0,H0,H3,H4,L0,L3,L4,T0,T3,T4,VR0,VR3,VR4);
					DrawTriR(X0+8,Y0-4,H1,H4,H2,L1,L4,L2,T1,T4,T2,VR1,VR4,VR2);
					DrawTriL(X0+8,Y0-4,H1,H4,H0,L1,L4,L0,T1,T4,T0,VR1,VR4,VR0);
					DrawTriL(X0+16,Y0,H2,H5,H4,L2,L4,L4,T2,T5,T4,VR2,VR5,VR4);
					ND++;
				}else NND++;
			};
		};
	};
	GT1=GetTickCount()-T0;
	T0=GetTickCount();
	//shrinking twice
	int ofs0=0;
	int ofs1=MM_Y0*MM_Lx;
	int NN1=MM_Lx*20;
	for(int iy=0;iy<MM_Ly;iy++){
		for(int ix=0;ix<MM_Lx;ix++){
			byte c=MiniMap[ofs1];
			if(c)MiniMap[ofs0]=c;
			else{
				int p=MM_Lx;
				while(p<NN1&&!(c=MiniMap[ofs1+p]))p+=MM_Lx;
				MiniMap[ofs0]=c;
			};
			ofs0++;
			ofs1++;
		};
		ofs1+=MM_Lx;
	};
	GT2=GetTickCount()-T0;
	T0=GetTickCount();
	//render add-details level
	int NDX=MM_Lx>>3;
	int NDY=MM_Ly>>3;
	for(int iy=0;iy<NDY;iy++){
		for(int ix=0;ix<NDX;ix++){
			DrawMiniMaskOnScreen(MiniMap,ix<<3,iy<<3,ix+(x0<<1),iy+(y0>>1),MM_Lx);
			DrawMiniSubWaterSquare(MiniMap,ix<<3,iy<<3,ix+(x0<<1),iy+(y0>>1),MM_Lx);
		};
	};

	ScreenPtr=sptr;
	MiniMap=mptr0;
	MM_Lx=LX0;
	MM_Ly=LY0;
	MM_Y0=Y0;
	GT4=GetTickCount()-T0;
	T0=GetTickCount();
	int NX=(nx<<4)/M4CellLX;
	int NY=(ny<<2)/M4CellLY;
	int _X0=(x0<<4)/M4CellLX;
	int _Y0=(y0<<2)/M4CellLY;
	for(int ix=0;ix<NX;ix++){
		for(int iy=0;iy<NY;iy++){
			byte* dat=ReserveMemoryFor4X(_X0+ix,_Y0+iy);
			int ofsp=int(mpart)+(ix+iy*NX*M4CellLY)*M4CellLX;
			int adofs=(NX-1)*M4CellLX;
			__asm{
				push esi
				push edi
				cld
				mov  esi,ofsp
				mov  edi,dat
				mov  ebx,M4CellLY
LPP1:			mov  ecx,M4CellLX/4
				rep  movsd
				add  esi,adofs
				dec  ebx
				jnz  LPP1
				pop  edi
				pop  esi
			};
		};
	};
	//GT5=GetTickCount()-T0;
};
int MM_MinChangeX=10000;
int MM_MinChangeY=10000;
int MM_MaxChangeX=-10000;
int MM_MaxChangeY=-10000;
void ReportCoorChange(int x,int y){
	//return;
	if(!(ReadyMap&&MM_Lx&&MM_Ly))return;
	if(x<0)x=0;
	if(y<0)y=0;
	if(x<MM_MinChangeX)MM_MinChangeX=x;
	if(y<MM_MinChangeY)MM_MinChangeY=y;
	if(x>MM_MaxChangeX)MM_MaxChangeX=x;
	if(y>MM_MaxChangeY)MM_MaxChangeY=y;
};
void ReportVertexChange(int v){
	int x=(v%VertInLine)*32;
	int y=(v/VertInLine)*32;
	if(x<0)x=0;
	if(y<0)y=0;
	if(x<MM_MinChangeX)MM_MinChangeX=x;
	if(y<MM_MinChangeY)MM_MinChangeY=y;
	if(x>MM_MaxChangeX)MM_MaxChangeX=x;
	if(y>MM_MaxChangeY)MM_MaxChangeY=y;
};
extern SprGroup WALLS;
void SetTexturesShadowInSquare(int x0,int y0,int x1,int y1);
void CheckMM_Change(){	
	/*
	if(MM_MinChangeX<=MM_MaxChangeX){
		SetTexturesShadowInSquare(MM_MinChangeX,MM_MinChangeY,MM_MaxChangeX,MM_MaxChangeY);		
		void CreateMiniMapPart(int x0,int y0,int x1,int y1,bool);
		CreateMiniMapPart(MM_MinChangeX>>6,MM_MinChangeY>>6,MM_MaxChangeX>>6,MM_MaxChangeY>>6,false);
		MM_MinChangeX= 10000;
		MM_MinChangeY= 10000;
		MM_MaxChangeX=-10000;
		MM_MaxChangeY=-10000;		
	};
	*/
};

int TGP;
extern DWORD T_Diff;
void ProcessTChanels();
void ClearFastSprites();

struct SpriteInst
{
    int         m_SpID; 
    int         m_Frame;
    int         m_ShadowFrame;
    Vector3D    m_Pos;  
    Vector3D    m_ShadowShift;
    float       m_CX;  
    float       m_CY;   
	float		m_Scale;
    DWORD       m_Color;
    DWORD       m_NatColor;
    bool        m_bForceFlush;
	Matrix3D*	camTM;//optional
	float		m_fall_stage;
	float		m_cmpcode;
}; // struct SpriteInst
//////////////////////////// 
//  dead units processing //
////////////////////////////
struct dead_unit_info{
	Matrix3D	m_camTM;
	SpriteInst	m_inst;
	bool		m_Drawn;
};
uni_hash<dead_unit_info> deads;
SpriteInst* get_dead_unit_inst(OneObject* OB,int Code){
	dead_unit_info* di=deads.get(OB->Index+(Code<<16));
	if(di){
		di->m_Drawn=true;
	}else return NULL;
	return &di->m_inst;
}
void add_dead_unit_inst(SpriteInst& si,OneObject* OB,int Code){
	dead_unit_info dui;
	dui.m_camTM=GetCamera()->GetWorldTM();
	dui.m_inst=si;
	dui.m_Drawn=true;	
	dead_unit_info* du = deads.add_once(OB->Index+(Code<<16),dui);
	du->m_inst.camTM=&du->m_camTM;
}
void dead_units_preprocess(){
	scan(deads,dead_unit_info* di,DWORD* k){
		di->m_Drawn=false;
	}scan_end;
}
void dead_units_postprocess(){
	scan(deads,dead_unit_info* di,DWORD* k){
		if(!di->m_Drawn){
			deads.del(*k);
		}
	}scan_end;
}
const int c_MaxSpriteInstDrawn = 4096;
static_array<SpriteInst,  c_MaxSpriteInstDrawn>    g_SpriteInstsDrawn;
static_array<SpriteInst*, c_MaxSpriteInstDrawn>    g_SpriteInstsSorted;

void clear_dead_units(){
	deads.reset();
	g_SpriteInstsDrawn.clear();
}
/////////////////////////////////
//  end: dead units processing //
/////////////////////////////////

int CalcDirIndex( const Vector3D& dir, int nSectors );
void DrawSpriteAsBillboard( int spID, int frID,       
                           const Vector3D& pos,      
                           const Matrix3D& camTM,    
                           int cx, int cy,  
                           DWORD color,                
                           float scale ); 

void DrawSpriteAsShadow(    int spID, int frID,        
                        const Vector3D& lightDir,  
                        const Vector3D& pos,       
                        int cx, int cy,            
                        float scale );  

extern Vector3D g_LightDir;
extern DWORD    g_ShadowColor;


struct SpriteInstCmp
{
    bool operator ()( const SpriteInst* l, const SpriteInst* r ) const
    {
        //return (l->m_Pos.y < r->m_Pos.y);
		if(l->m_fall_stage>r->m_fall_stage)return false;
		if(l->m_cmpcode>r->m_cmpcode)return false;
		return (l->m_Pos.y < r->m_Pos.y);
    }
}; // struct RenderBitCmp
int compare_sp(const void * L, const void *R){

	const SpriteInst* l=*((const SpriteInst**)L);
	const SpriteInst* r=*((const SpriteInst**)R);

	if(l->m_fall_stage > r->m_fall_stage)return 1;
	if(l->m_fall_stage < r->m_fall_stage)return -1;

	if(l->m_cmpcode > r->m_cmpcode)return 1;
	if(l->m_cmpcode < r->m_cmpcode)return -1;

	if(l->m_Pos.y < r->m_Pos.y)return 1;
	if(l->m_Pos.y > r->m_Pos.y)return -1;

	return 0;
}
void DrawSpriteAsVerticalBillboard( int spID, int frID,         //  sprite ID 
                            const Vector3D& pos,        //  sprite pos
                            const Matrix3D& camTM,      //  camera rotation matrix
                            int cx, int cy,             //  sprite pivot
                            DWORD color,                //  national color
                            float scale,				//  sprite scale
							float fall_stage);        
void FlushSpriteInsts()
{
    static int shID0 = IRS->GetShaderID( "sprite_trees_shadow" );
    int nSp = g_SpriteInstsDrawn.size();

    g_SpriteInstsSorted.clear();
	for (int i = 0; i < nSp; i++){ 
		SpriteInst* l=&g_SpriteInstsDrawn[i];
		l->m_cmpcode=l->m_SpID*512+l->m_Frame;
		g_SpriteInstsSorted.push_back( l );
	}
    
    //std::sort( g_SpriteInstsSorted.begin(), g_SpriteInstsSorted.end(), SpriteInstCmp() );
	if(g_SpriteInstsSorted.size())
		qsort(&g_SpriteInstsSorted[0],g_SpriteInstsSorted.size(),4,&compare_sp);

    //  draw shadows
	ISM->SetCurrentDiffuse( EngSettings.ShadowsColor );
    ISM->SetCurrentShader( shID0 );	
	IRS->SetTextureFactor( 0xFF808080 );
    for (int i = 0; i < nSp; i++)
    {		
        SpriteInst& st = *(g_SpriteInstsSorted[i]);
		//DWORD ModDWORD2X(DWORD C1,DWORD C2);
		//ISM->SetCurrentDiffuse( ModDWORD2X(g_ShadowColor,st.m_Color ));
        DrawSpriteAsShadow( st.m_SpID, st.m_ShadowFrame, g_LightDir, st.m_Pos+st.m_ShadowShift, st.m_CX, st.m_CY, st.m_Scale );
    }
    ISM->Flush();

    Matrix3D camTM = GetCamera()->GetWorldTM();

    static int shID_2pass = IRS->GetShaderID( "sprite_natcolor" );	
    static int shID_1pass = IRS->GetShaderID( "sprite_natcolor_1pass" );	
    int shID = GSets.SVOpt.DrawSpritesIn2Pass ? shID_2pass : shID_1pass;
	static int shID_iso = IRS->GetShaderID( "sprite_natcolor_iso" );	
	extern bool	g_bPerspCamera;
	if(g_bPerspCamera)ISM->SetCurrentShader( shID );
	else ISM->SetCurrentShader( shID_iso );
    ISM->SetCurrentDiffuse( 0xFFFFFFFF );
    
    DWORD curNatColor = 0;

	DWORD psp=0;

    for (int i = 0; i < nSp; i++)
    {		
        const SpriteInst& st = *(g_SpriteInstsSorted[i]);
		int sp=st.m_SpID+st.m_Frame*65535;
		if(sp!=psp && st.m_fall_stage<0.001){
			ISM->Flush();
		}
		psp=sp;
		ISM->SetCurrentDiffuse( st.m_Color );
        
		DrawSpriteAsVerticalBillboard( st.m_SpID, st.m_Frame, st.m_Pos, st.camTM?*st.camTM:camTM, st.m_CX, st.m_CY, st.m_NatColor, st.m_Scale, st.m_fall_stage );
        if (st.m_bForceFlush) ISM->Flush();
    }
    ISM->Flush();
    g_SpriteInstsDrawn.clear();	
} // FlushSpriteInsts
int g_ExtraViewVolume=0;
void DrawSpriteInsts()
{
    if(!GSets.SVOpt.DrawTrees)return;
    ClearFastSprites();
    ProcessTChanels();
    GPS.SetCurrentDiffuse( T_Diff );


    //  find view frustum extents on the map
    const Vector3D* vc = GetCameraIntersectionCorners();
    Frustum fr = ICam->GetFrustum();
    Vector3D ltn = fr.ltn();
    Vector3D rtn = fr.rtn();
    Vector3D rbn = fr.rbn();
    Vector3D lbn = fr.lbn();
    float minFx = tmin( tmin( ltn.x, rtn.x, rbn.x, lbn.x ), tmin( vc[0].x, vc[1].x, vc[2].x, vc[3].x ) );
    float minFy = tmin( tmin( ltn.y, rtn.y, rbn.y, lbn.y ), tmin( vc[0].y, vc[1].y, vc[2].y, vc[3].y ) );
    float maxFx = tmax( tmax( ltn.x, rtn.x, rbn.x, lbn.x ), tmax( vc[0].x, vc[1].x, vc[2].x, vc[3].x ) );
    float maxFy = tmax( tmax( ltn.y, rtn.y, rbn.y, lbn.y ), tmax( vc[0].y, vc[1].y, vc[2].y, vc[3].y ) );

    if (maxFx <= minFx || maxFy <= minFy) return;

    int spx0 = minFx/4.0f/32.0f - 1;
    int spy0 = minFy/4.0f/32.0f - 1;
    int spx1 = maxFx/4.0f/32.0f + 1;
    int spy1 = maxFy/4.0f/32.0f + 1;

    clamp( spx0, 0, VAL_SPRNX - 1 );
    clamp( spx1, 0, VAL_SPRNX - 1 );
    clamp( spy0, 0, VAL_SPRNX - 1 );
    clamp( spy1, 0, VAL_SPRNX - 1 );

	int x0 = mapx<<5;
	int y0 = (mapy)<<4;
	int Lx = smaplx<<5;
	int Ly = (smaply)<<4;
	int x1 = x0+Lx;
	int y1 = y0+Ly;
	int SH = 5-Shifter;
	static int shTrees = IRS->GetShaderID( "sprite_trees" );
	GPS.SetShader( shTrees );
    GPS.SetCurrentDiffuse( 0xFF808080 );
	bool CheckIfNewTerrain();
	bool bNewTerr = CheckIfNewTerrain();
	Vector3D vd=ICam->GetDirection();

    bool LQshadow=GSets.ShadowQuality>=2;

	for (int spx = spx0; spx <= spx1; spx++)
    {
		int ofst = spx + (spy0<<SprShf);
		int maxy;
		int xx = (spx<<7) + 64;
		for (int spy = spy0; spy <= spy1; spy++)
        {
			int  N    = NSpri[ofst];
			int* List = SpRefs[ofst];
            ofst += VAL_SPRNX;
			if (!N || !List) continue;
			for (int i = 0; i < N; i++)
            {
				OneSprite& OS = Sprites[List[i]];
				if (!OS.Enabled) continue;
                int z=OS.z;//GetHeight(OS.x,OS.y);
				if(!Mode3D)z=0;
				int ry=((OS.y)>>1)-y0;
				int ry1=ry-z;
				int rx=OS.x-x0;
				const ObjCharacter* OC = OS.OC;
                int R = tmax( OC->CenterX, OC->CenterY )+g_ExtraViewVolume;
                //  test rough object/frustum intersection
				if (!CheckObjectVisibility( OS.x, OS.y, OS.z, R )) continue;
				//int GetObjectVisibilityValueInFogForSprites(int x,int y,int z,OneObject* OB);
				//int cf=GetObjectVisibilityValueInFogForSprites(OS.x,OS.y,OS.z,NULL);
				//if(cf<16) continue;
                //  play sound effect 
				int cf=255;
				if(OS.CurrentModIndex!=-1){
                    TreeModificationParam* tm=OC->TreeMods[OS.CurrentModIndex];
					if(tm->SoundID>0 && rand()<1310){
						static NewAnimation NA;
						static bool init=false;
						if(!init){
							NA.ActivePtX = NULL;
							NA.ActivePtY = NULL;
							NA.LineInfo  = NULL;
							NA.Name      = NULL;
							init         = true;								
						}
						NA.SoundID=tm->SoundID;
						NA.SoundProbability=tm->SoundProbability*GameSpeed/256;
						if(NA.SoundProbability<2)NA.SoundProbability=2;
						NA.HotFrame=0xFF;
						NA.NFrames=1;
						PlayAnimation(&NA,0,OS.x,OS.y);
					}
				}
				if(OC->SoundID>0&&rand()<1310)
                {
					static NewAnimation NA;
					static bool init=false;
					if(!init){
						NA.ActivePtX = NULL;
						NA.ActivePtY = NULL;
						NA.LineInfo  = NULL;
						NA.Name      = NULL;
						init         = true;								
                    }
					NA.SoundID=OC->SoundID;
					NA.SoundProbability=OC->SoundProb*GameSpeed/256;
					if(NA.SoundProbability<2)NA.SoundProbability=2;
					NA.HotFrame=0xFF;
					NA.NFrames=1;
					PlayAnimation(&NA,0,OS.x,OS.y);
				}

				SprGroup* SG=OS.SG;
				if(OC->UseTexture)
                {
                    void AddFastSprite(const OneSprite& OS);
					AddFastSprite( OS );
                    continue;
				}

				extern bool g_bPerspCamera;

                if(OC->ViewType==1)
                {
					if(OS.M4){
						Matrix4D MM=*OS.M4;                        
						float shScale=1.0f;
                        if(LQshadow){
                            IShadowMgr->AddSimpleCaster(Vector3D(OS.x,OS.y,OS.z),Vector3D(0,1,0),OS.OC->LoQShadowSX*MM.e22,OS.OC->LoQShadowSY*MM.e22,OS.OC->LoQShadowID,true,0xFF000000);
                        }
						if(OS.SG==&TREES){

							if(OS.RiseStage<1){
								Matrix4D MR;
								MR.scaling(OS.RiseStage);
								MM.mulLeft(MR);
								shScale=OS.RiseStage;
							}else{
								if(OS.DeathStage<1){
									Matrix4D MR;
									Vector3D Axe(1+float(OS.Index)/4,sin(float(OS.Index))/3,0);
									Axe.normalize();									
									float c=OS.DeathStage;
									c=(1-c)*(1-c);
									float angle=c*c_PI/2;
									MR.rotation(Axe,angle);
									MM.mulLeft(MR);	
								}
							}
							if( MOptions.GlobalMapScale!=256 ){
								Matrix4D MR;
								float s=MOptions.GlobalMapScale/256.0;
								MR.scaling(s);
								MM.mulLeft(MR);
								shScale*=s;
							}
						}
						if(OS.CurrentModIndex!=-1){
                            TreeModificationParam* TM=OC->TreeMods[OS.CurrentModIndex];
							if(g_bPerspCamera || TM->SpriteForIsomerticMode<=0){
								const char* mn=IMM->GetModelFileName(TM->ModelID);
								if(strstr(mn,"_cm.")){ 
									TreesMesh.AddTree(OS.x+OS.y*16384+OS.CurrentModIndex*177351,DWORD((OS.DeathStage+OS.RiseStage*1000)*1000),Vector3D(OS.x,OS.y,OS.z),TM->ModelID,OS.Color,MM);
								}else{                                   
									RenderModels.Add(OS.Index,TM->ModelID,&MM,-vd.x*OS.x-vd.y*OS.y,OS.Color & 0x00FFFFFF + (cf<<24),TM->LeafsShaderID,TM->LeafsRiseVariable,OS.RiseStage,TM->LeafsDeathVariable,OS.DeathStage);								
									//  add to sprite trees batch
									if(false &&		TM->SpriteForShadowID!=-1){
										ObjCharacter* OC=OS.OC;
										if (g_SpriteInstsDrawn.size() < c_MaxSpriteInstDrawn)
										{
											ObjCharacter* OC2=SG->Objects[TM->SpriteForShadowID];
											SpriteInst& st = g_SpriteInstsDrawn.expand();
											st.m_SpID           = OC->FileID;
											st.m_Frame          = -1;
											st.m_ShadowFrame    = OC2->SpriteID;
											st.m_Pos            = Vector3D( OS.x, OS.y, OS.z );
											st.m_CX             = OC2->CenterX;
											st.m_CY             = OC2->CenterY;
											st.m_bForceFlush    = false;
											st.m_Color			= 0xFF808080;
											st.m_Scale			= TM->Scale*TM->ShadowScale*shScale;
                                            st.m_ShadowShift    = Vector3D::null;
											st.camTM			= NULL;
											st.m_fall_stage		= 1;
										}
									}                                    
								}
							}else{
								if (g_SpriteInstsDrawn.size() < c_MaxSpriteInstDrawn)
								{
									ObjCharacter* OC2=SG->Objects[TM->SpriteForShadowID];
									SpriteInst& st = g_SpriteInstsDrawn.expand();
									st.m_SpID           = OC->FileID;
									st.m_Frame          = TM->SpriteForIsomerticMode;
									st.m_ShadowFrame    = OC2->SpriteID;
									st.m_Pos            = Vector3D( OS.x, OS.y, OS.z );
									st.m_CX             = OC2->CenterX;
									st.m_CY             = OC2->CenterY;
                                    st.m_bForceFlush    = false;//true;
									st.m_Color			= 0xFF808080;
									st.m_Scale			= 1;
                                    st.m_ShadowShift    = Vector3D::null;
									st.camTM			= NULL;
									st.m_fall_stage		= 1;
								}
							}
						}else{
							RenderModels.Add(OS.Index,OC->ModelManagerID,&MM,-vd.x*OS.x-vd.y*OS.y,OC->TFactor);							
						}
					}
                    else
                    {						
						Matrix4D M4;
						OC->GetMatrix4D(M4,OS.x,OS.y,OS.z);
						RenderModels.Add(OS.Index,OC->ModelManagerID,&M4,-vd.x*OS.x-vd.y*OS.y,OC->TFactor);						
					}
                    continue;
				}
                
                if(OS.M4)
                {
					GPS.DrawWSprite(OC->FileID,OC->SpriteID,*OS.M4,0);
					continue;
				}

                if(SG==&COMPLEX)
                {
						void DrawFPatch(int x,int y,int z,int W,int H,float G,float S);
						DrawFPatch(OS.x,OS.y,OS.z,64,64,float(OC->Z0)/255.0,float(OC->DZ)/255.0);
				        continue;
                }
                
                if(OC->Stand)
                {
					int tm1=div(tmtmt,OC->Delay).quot;
					int fr=div(tm1+OS.x*47+OS.y*83,OC->Frames).rem;
					int spr=fr*OC->Parts;
					int z0=ry+OC->Z0;
					int XX=rx-OC->CenterX;
					int YY=ry-OC->CenterY-z;
					NewAnimation* NA=OC->Stand;
					for(int p=0;p<OC->Parts;p++){
						NewFrame* OF=NA->Frames[spr+p];
						//AddPoint(rx>>2,z0>>2,XX>>2,YY>>2,NULL,OF->FileID,OF->SpriteID,0,0);
						AddWorldPoint(OS.x,OS.y,OS.z,OC->CenterX,OC->CenterY,NULL,OF->FileID,OF->SpriteID);
						z0+=OC->DZ;
					};
                    continue;
				}
                
                if(SG==&ANMSPR)
                {
					int sp=SG->Objects[OS.SGIndex]->SpriteID;
					int dx=OC->CenterX;
					if(sp>=4096)AddSuperLoPoint((rx+dx)>>2,(ry1-OC->CenterY)>>2,NULL,OC->FileID,sp,0,0);
					else AddSuperLoPoint((rx-dx)>>2,(ry1-OC->CenterY)>>2,NULL,OC->FileID,sp,0,0);
				    continue;
                }
                
                if(SG==&TREES||SG==&WALLS||(SG==&STONES&&bNewTerr))
                {
					//  add to sprite trees batch
                    if (g_SpriteInstsDrawn.size() < c_MaxSpriteInstDrawn)
                    {
                        SpriteInst& st = g_SpriteInstsDrawn.expand();
                        st.m_SpID           = OC->FileID;
                        st.m_Frame          = OC->SpriteID;
                        st.m_ShadowFrame    = OC->SpriteID;
                        st.m_Pos            = Vector3D( OS.x, OS.y, OS.z );
                        st.m_CX             = OC->CenterX;
                        st.m_CY             = OC->CenterY;
                        st.m_bForceFlush    = false;//true;
                        st.m_ShadowShift    = Vector3D::null;
						byte GetShadowInPoint(int x,int y);
						if(!OS.Color){
							if(OC->TFactor)OS.Color=OC->TFactor;
							else OS.Color=0xFF808080;
							int A=GetShadowInPoint(OS.x,OS.y)*2;
							OS.Color=MulDWORD(OS.Color,A);
						}
						st.m_Color			= OS.Color;//0xFF808080;
						st.m_Scale			= 1.0f;
						st.camTM			= NULL;
						st.m_fall_stage		= 1;
                    }
				}
			}
		}
	}
    GPS.FlushBatches();
} // DrawSpriteInsts

void ShowSpritesShadows()
{
    if (TGP == -1) TGP = GPS.PreLoadGPImage( "TreesAll" );

    //  find view frustum extents on the map
    const Vector3D* vc = GetCameraIntersectionCorners();
    Frustum fr = ICam->GetFrustum();
    Vector3D ltn = fr.ltn();
    Vector3D rtn = fr.rtn();
    Vector3D rbn = fr.rbn();
    Vector3D lbn = fr.lbn();
    float minFx = tmin( tmin( ltn.x, rtn.x, rbn.x, lbn.x ), tmin( vc[0].x, vc[1].x, vc[2].x, vc[3].x ) );
    float minFy = tmin( tmin( ltn.y, rtn.y, rbn.y, lbn.y ), tmin( vc[0].y, vc[1].y, vc[2].y, vc[3].y ) );
    float maxFx = tmax( tmax( ltn.x, rtn.x, rbn.x, lbn.x ), tmax( vc[0].x, vc[1].x, vc[2].x, vc[3].x ) );
    float maxFy = tmax( tmax( ltn.y, rtn.y, rbn.y, lbn.y ), tmax( vc[0].y, vc[1].y, vc[2].y, vc[3].y ) );

    if (maxFx <= minFx || maxFy <= minFy) return;

    //  for every object on the map
    int spx0 = minFx/4.0f/32.0f - 1;
    int spy0 = minFy/4.0f/32.0f - 1;
    int spx1 = maxFx/4.0f/32.0f + 1;
    int spy1 = maxFy/4.0f/32.0f + 1;

    if(spx0<0)spx0=0;else
        if(spx0>=VAL_SPRNX)spx0=VAL_SPRNX-1;
    if(spy0<0)spy0=0;else
        if(spy0>=VAL_SPRNX)spy0=VAL_SPRNX-1;
    if(spx1<0)spx1=0;else
        if(spx1>=VAL_SPRNX)spx1=VAL_SPRNX-1;

    int x0=mapx<<5;
    int y0=(mapy)<<4;
    int Lx=smaplx<<5;
    int Ly=(smaply)<<4;
    int x1=x0+Lx;
    int y1=y0+Ly;
    int SH=5-Shifter;
    static int h2=IRS->GetShaderID("tree_shadows_on_ground");
    GPS.SetShader(h2);
    GPS.SetCurrentDiffuse( EngSettings.ShadowsColor );

    for(int spx=spx0;spx<=spx1;spx++){
        int ofst=spx+(spy0<<SprShf);
        int spy=spy0;
        int maxy;
        int xx=(spx<<7)+64;
        do{
            int N=NSpri[ofst];
            int* List=SpRefs[ofst];
            if(N&&List){
                int st=1;//(N/10)+1;				
                for(int i=0;i<N;i+=st){
                    OneSprite* OS=Sprites+List[i];
                    if(OS->Enabled){
                        int z=OS->z;//GetHeight(OS->x,OS->y);
                        if(!Mode3D)z=0;
                        int ry=((OS->y)>>1)-y0;
                        int ry1=ry-z;
                        int rx=OS->x-x0;
                        int SZ=128;
                        ObjCharacter* OC=OS->OC;
                        if(OC->ViewType==1)SZ=400;
                        if(CheckObjectVisibility(OS->x,OS->y,OS->z,180)){                            
                            extern bool NewSurface;
                            if (NewSurface)
                            {
                                Matrix4D shTM = GetAlignGroundTransform( Vector3D( OC->CenterX, OC->CenterY, 0.0f ) );
                                shTM.translate( SkewPt( OS->x,OS->y,OS->z ) );
                                ISM->DrawWSprite( TGP, OC->SpriteID, shTM );
                            }
                        }
                    }
                }
            }
            spy++;
            ofst+=VAL_SPRNX;
            maxy=spy<<6;
            if(Mode3D)maxy-=GetHeight(xx,maxy<<1);
        }while(spy<VAL_SPRNX&&maxy<(y1+250));        
    };
    GPS.FlushBatches();	
};
void GSSetup800();
extern int CurPalette;
void ClearAllWaves();
void ClearAllLModeData(){
	try{
		ClearAllWaves();
		if(LMode)ReverseLMode();
		if(ReadyMap){
			free(ReadyMap);
		};
	}catch(...){
	};
	MiniMap=NULL;
	ReadyMap=NULL;
	LMode=0;
};
void CheckMM4XReady(int x,int y,int Lx,int Ly){
	int NX=M4CellLX>>4;
	int NY=M4CellLX>>2;
	bool NeedDraw=0;
	int DRX,DRY,DNY;
	int x0=x/M4CellLX;
	int y0=y/M4CellLY;
	int x1=((x+Lx)/M4CellLX)+1;
	int y1=((y+Ly)/M4CellLY)+1;
	if(x1>=MMR_Lx)x1=MMR_Lx-1;
	if(y1>=MMR_Ly)y1=MMR_Ly-1;
	for(int ix=x0;ix<=x1;ix++){
		for(int iy=y0;iy<=y1;iy++){
			int ofs=ix+iy*MMR_Lx;
			if(ReadyMap[ofs]){
				if(NeedDraw){
					if(DNY==1){
						int NXX=1;
						for(int ixx=ix+1;ixx<=x1;ixx++){
							int ofs=ixx+DRY*MMR_Lx;
							if(!ReadyMap[ofs]){
								NXX++;
								ReadyMap[ofs]=(byte*)1;
								assert(ofs<RSIZE);
							}else ixx=x1+1;
						};
						GenerateMiniMapSquare(DRX*NX,DRY*NY,NX*NXX,DNY*NY);
						NeedDraw=0;
					}else{
						GenerateMiniMapSquare(DRX*NX,DRY*NY,NX,DNY*NY);
						NeedDraw=0;
					};
				};
			}else{
				ReadyMap[ofs]=(byte*)1;
				assert(ofs<RSIZE);
				if(NeedDraw)DNY++;
				else{
					DRX=ix;
					DRY=iy;
					DNY=1;
					NeedDraw=1;
				};
			};
		};
		if(NeedDraw){
			if(DNY==1){
				int NXX=1;
				for(int ixx=ix+1;ixx<=x1;ixx++){
					int ofs=ixx+DRY*MMR_Lx;
					if(!ReadyMap[ofs]){
						NXX++;
						ReadyMap[ofs]=(byte*)1;
						assert(ofs<RSIZE);
					}else ixx=x1+1;
				};
				GenerateMiniMapSquare(DRX*NX,DRY*NY,NX*NXX,DNY*NY);
				NeedDraw=0;
			}else{
				GenerateMiniMapSquare(DRX*NX,DRY*NY,NX,DNY*NY);
				NeedDraw=0;
			};
		};
	};
	//assert(_CrtCheckMemory());
};
//////////////////////////////////////////////////////////////////////////
class OneBldFlag:public BaseClass{
public:
	_str  Name;
	short gpFile;
	int   cx;
	int   cy;
	int   Start;
	int   Final;
	SAVE(OneBldFlag);
	REG_AUTO(Name);
	REG_MEMBER(_gpfile,gpFile);
	REG_MEMBER(_int,cx);
	REG_MEMBER(_int,cy);
	REG_MEMBER(_int,Start);
	REG_MEMBER(_int,Final);
	ENDSAVE;
};
class FlagsOnBuildings:public BaseClass{
public:
	ClassArray<OneBldFlag> Flags;
	SAVE(FlagsOnBuildings);
	REG_CLASS(OneBldFlag);
	REG_AUTO(Flags);
	ENDSAVE;
};
FlagsOnBuildings FBLD;
void LoadFlagsInfo(){
	xmlQuote xml;
	if(xml.ReadFromFile("dialogs\\Flags.xml")){
		ErrorPager EP;
		FBLD.Load(xml,&FBLD,&EP);
	}
}
int GetFlagIndex(char* Name){
	static bool FBINIT=0;
	if(!FBINIT){
		LoadFlagsInfo();
		FBINIT=1;
	}
	for(int i=0;i<FBLD.Flags.GetAmount();i++)if(!strcmp(FBLD.Flags[i]->Name.pchar(),Name))return i;
	return -1;
}
int GetInterpFOW(int x,int y);
extern int FogMode;
extern byte BaloonState;
bool GetObjectVisibilityInFog(OneObject* OB){
	bool OK=true;
	bool usefog=FogMode;
	if(usefog){
		int x=OB->RealX>>4;
		int y=OB->RealY>>4;
		int dp=GetInterpFOW(x,y);
		if(OB && OB->NewBuilding){
			if(dp<600)OK=false;
		}else{
			if(dp<750)OK=false;
		}
	}
	return OK;
}
bool GetObjectVisibilityInFog(OneObject* OB,int MinFogValue){
	bool OK=true;
	bool usefog=FogMode;
	if(usefog){
		int x=OB->RealX>>4;
		int y=OB->RealY>>4;
		int dp=GetInterpFOW(x,y);		
		if(dp<MinFogValue)OK=false;		
	}
	return OK;
}
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB){
    if(!MOptions.HideEnemyUnitsUnderFog)return true;
	if(OB&&OB->NNUM==7)return true;
	bool usefog=FogMode&&BaloonState!=1&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision);
	bool OK=true;
	if(usefog/*&&!(OB&&OB->NNUM==7&&OB->NewBuilding)*/){
		int dp=GetInterpFOW(x,y);
        if(OB && OB->NewBuilding){
    		if(dp<600)OK=false;
        }else{
            if(dp<750)OK=false;
        }
    }
	return OK;
}
bool GetObjectVisibilityInFogWithInvisibility(int x,int y,int z,OneObject* OB){
	if(OB&&OB->NNUM==7)return true;
	bool usefog=FogMode&&BaloonState!=1&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision);
	bool OK=true;
	if(usefog/*&&!(OB&&OB->NNUM==7&&OB->NewBuilding)*/){
        if(MOptions.HideEnemyUnitsUnderFog){
    		int dp=GetInterpFOW(x,y);		
	    	if(dp<750)OK=false;
        }
    }
	if(OB && OK && OB->ActiveAbility){
		bool inv=false;
		bool esv=true;
		byte det=0;
        OB->ActiveAbility->ActiveAbilities.modifyInvisibility(inv,inv,det,esv); // GetObjectVisibilityInFogWithInvisibility
		if(inv && !(det & NATIONS[MyNation].NMask) )OK=false;
	}
	return OK;
}
// 0 - fully invisible 255 - fully visible
int GetObjectVisibilityValueInFog(int x,int y,int z,OneObject* OB){
    if(!MOptions.HideEnemyUnitsUnderFog)return 255;
	if(OB&&OB->NNUM==7)return 255;
	bool usefog=FogMode&&BaloonState!=1&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision);
	int OK=255;
	if(usefog){
		int dp=GetInterpFOW(x,y);		
		if(dp<850)OK=0;
		else if(dp>1100)return 255;
		else if(dp<750){
			//int GetFOW2(int x,int y);
			//int V=GetFOW2(x,y/2-z);
			OK=((dp-750)*255)/(840-750);
			//OK=255-V;
			if(OK<0)OK=0;
			if(OK>255)OK=255;
		}
	}
	return OK;
}
int GetObjectVisibilityValueInFogForSprites(int x,int y,int z,OneObject* OB){
	Vector3D T(x,y,z);
	WorldToScreenSpace(T);
    int T0=(1-pow(T.z,64))*255;
	if(OB&&OB->NNUM==7)return T0;
	bool usefog=FogMode&&BaloonState!=1&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision);
	int OK=255;
	if(usefog){
		const int dp_trees=100;
		int dp=GetInterpFOW(x,y);		
		if(dp<500)OK=0;
		else if(dp>1100)return 255;
		else if(dp<dp_trees){
			//int GetFOW2(int x,int y);
			//int V=GetFOW2(x,y/2-z);
			OK=((dp-dp_trees)*255)/(940-dp_trees);
			//OK=255-V;
			if(OK<0)OK=0;
			if(OK>255)OK=255;
		}
	}
	return OK*T0/255;
}
#define NewDraw

extern int TrueTime;

#include "ua_Ability.h"

bool g_bRenderShadows;

_inline void DrawActiveAbility(OneObject* OB){	

	UnitActiveAbilityList* AL=OB->ActiveAbility;
	if( AL ){
		ActiveAbilityListArray& AArray=AL->ActiveAbilities;
		if(AArray.AnimationMask){
			int n=AArray.GetAmount();
			for(int i=0;i<n;i++){
				ActiveUnitAbility* AA=AArray[i];

				//if(AA->EfAnimationMask){
				//	UnitAbility* UA=AA->GetA();
				//	if(UA){
				//		NewAnimation* NA=UA->eAn.Get();
				//		if(NA&&NA->Enabled){
				//			DWORD h=0;
				//			OneAbility* OA=dynamic_cast<OneAbility*>(AA);
				//			if(OA){
				//				if(TrueTime-OA->eAuraStartTime>3000){
				//					OA->eAuraStartTime=TrueTime;
				//					OA->eHandler=GetTickCount();
				//				}
				//				h=(OA->eHandler<<8);
				//			}						
				//			AddAnimation(OB->RealX/16.0f,OB->RealY/16.0f,OB->RZ,NA,0,OB->RealDir,0xFFFFFFFF,OB,1,0,0,(OB->Serial<<16)+(AA->Serial<<3)+0+h);
				//		}
				//	}
				//}

				//if(AA->TypeAbil!=0){
				if( AA ){
					//extern int xAA, yAA;
					NewAnimation* NA=AA->GetAuraAnimation();				
					//if(NA&&NA->Enabled){
					//	AddAnimation(xAA,yAA,OB->RZ,NA,AA->eAuraFrame*256,0/*OB->RealDir*/,0xFFFFFFFF,OB,1,0,0,AA->Serial/*+AA->TypeAbil*0x10000000*/);
					//}
				}
			}
		}
	}
}

bool CheckObjectVisibilityOnScreen(OneObject* OB,float Scale);
void DrawMarker(OneObject* OB);

void DrawUnits()
{	
    if(!GSets.SVOpt.DrawUnits)return;
    //  sprites on the map are drawn with enabled z-buffer
	GPS.EnableZBuffer( true );
	GPS.SetScale( 1.0f / float( 1 << (5-Shifter) ) );
	ClearVisibleGP();

	dead_units_preprocess();
	
	int SCSHIFT=5-Shifter;
	CLM_Shift=SCSHIFT;
	CLM_ShiftY=SCSHIFT;
	ClearZBuffer();
	IRS->ResetWorldTM();

	int x0=mapx<<(5-CLM_Shift);
	int y0=(mapy<<(4-CLM_Shift));
	int Lx1=smaplx<<(5-CLM_Shift);
	int Ly1=smaply<<(4-CLM_Shift);


	int xx,yy;
	int mpdy    = mapy<<(4-CLM_Shift);
	int dxx     = mapx<<(5-CLM_Shift);
	int dyy     = mapy<<(4-CLM_Shift);

    //  find view frustum extents on the map
	const Vector3D* vc = GetCameraIntersectionCorners();
    Frustum fr = ICam->GetFrustum();
    Vector3D ltn = fr.ltn();
    Vector3D rtn = fr.rtn();
    Vector3D rbn = fr.rbn();
    Vector3D lbn = fr.lbn();
    float minFx = tmin( tmin( ltn.x, rtn.x, rbn.x, lbn.x ), tmin( vc[0].x, vc[1].x, vc[2].x, vc[3].x ) );
    float minFy = tmin( tmin( ltn.y, rtn.y, rbn.y, lbn.y ), tmin( vc[0].y, vc[1].y, vc[2].y, vc[3].y ) );
    float maxFx = tmax( tmax( ltn.x, rtn.x, rbn.x, lbn.x ), tmax( vc[0].x, vc[1].x, vc[2].x, vc[3].x ) );
    float maxFy = tmax( tmax( ltn.y, rtn.y, rbn.y, lbn.y ), tmax( vc[0].y, vc[1].y, vc[2].y, vc[3].y ) );

    bool usefog=FogMode&&BaloonState!=1&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision);
    
    if (maxFx <= minFx || maxFy <= minFy) return;

	//  for every object on the map
	int CX0 = minFx/4.0f/32.0f - 1;
	int CY0 = minFy/4.0f/32.0f - 1;
	int CX1 = maxFx/4.0f/32.0f + 1;
	int CY1 = maxFy/4.0f/32.0f + 1;

	int VDX=32;
	if(CX0<0)CX0=0;
	if(CY0<0)CY0=0;
    
    //  scan through cells and draw visible (non-bulding) objects
	for (int dx = CX0; dx < CX1; dx++)
    {
		for (int dy = CY0; dy < CY1; dy++)
        {
			int cell = 1 + dx + ((dy + 1)<<VAL_SHFCX);
			if(cell >= VAL_MAXCIOFS) continue;
			int NMon = MCount[cell];
			if (NMon == 0) continue;
			int ofs1 = cell<<SHFCELL;
			for (int i = 0; i < NMon; i++)
            {
				WORD MID=GetNMSL(ofs1+i);
				if(MID==0xFFFF) continue;
				OneObject* OB=Group[MID];
				if(!OB||OB->Hidden) continue;
				GPS.SetCurrentDiffuse(0xFF808080);

				if (!CheckObjectVisibilityOnScreen( OB, 1.0f)) continue;
				DWORD V=GetObjectVisibilityValueInFog(OB->RealX>>4,OB->RealY>>4,OB->RZ+OB->OverEarth,OB);
				if (V==0) continue;

				NewAnimation* NAM=OB->NewAnm;
				if(!NAM) continue;
				word Scale=OB->ObjectScale;

                OB->WasDrawn=true;
                OB->IsInFog=usefog ? GetInterpFOW(OB->RealX>>4,OB->RealY>>4)<600 : 0;

				if (OB&&OB->Sdoxlo)
				{
                    

					NewAnimation* NA=OB->newMons->Animations->Get(anm_Skeleton,OB->Index);//>GetAnimation(anm_Skeleton);
					if(NA->Enabled){
						int dt0=EngSettings.Default_DeadUnitDisappearTime;
						int dt=dt0+EngSettings.Default_SkeletonDisappearTime;                        
						if(dt<=0)dt=1;
						if(dt0<=0)dt=1;
						int C=int(OB->Sdoxlo)*(dt-OB->Sdoxlo)/dt*255*4/dt;
						if(OB->Sdoxlo<dt0){
							C=C*OB->Sdoxlo/dt0;
						}
						if(C>255)C=255;
						if(C<0)C=0;
						//C=C*V/255;
						AddAnimation(OB->RealX/16.0f,OB->RealY/16.0f,OB->RZ+OB->OverEarth,NA,
							0,float(OB->RealDirPrecise)/256.0f,0x00808080+(C<<24),OB,float(Scale)/256.0,0,0,0x23000000|OB->Serial);  
						int C1=255-int(OB->Sdoxlo)*255/(EngSettings.Default_DeadUnitDisappearTime+1);
						if(C1<0)C1=0;
						V=V*C1/255;
					}else{
						int C=255-int(OB->Sdoxlo)*255/(EngSettings.Default_DeadUnitDisappearTime+1);
						if(C<0)continue;
						V=V*C/255;
					}
				}
				
				if(OB->ActiveAbility) OB->ActiveAbility->ActiveAbilities.modifyScale(OB->newMons->StartScale,Scale);

				if(OB->MotionStyle==8)
                {
					//int vx=TCos[OB->RealDir];
					//int vy=TSin[OB->RealDir];
					//float N1=sqrt(vx*vx+vy*vy);
					//float N2=sqrt(OB->ForceX*OB->ForceX+OB->ForceY*OB->ForceY);
					//float s=0;
					//if(N1&&N2)s=atan(float(vx*OB->ForceY-vy*OB->ForceX)/N1/250.0f);
					//AddAnimation(OB->RealX/16.0f,OB->RealY/16.0f,OB->RZ+OB->OverEarth,NAM,OB->CurrentFrameLong,
					//	float(OB->RealDirPrecise)/256.0f,0xFF808080,OB,float(Scale)/256.0,0,s,0x23000000|OB->Serial);
                    AddAnimation(OB->RealX/16.0f,OB->RealY/16.0f,OB->RZ+OB->OverEarth,NAM,OB->CurrentFrameLong,
                        	float(OB->RealDirPrecise)/256.0f,0xFF808080,OB,float(Scale)/256.0,OB->ForceX*c_PI/32768,OB->ForceY*c_PI/32768,0x23000000|OB->Serial);
				}else{
					int frame=OB->CurrentFrameLong;
					if(V<240){						
						if(NAM->Code>=23&&NAM->Code<=26){//rest
                            NAM=OB->newMons->GetAnimation(anm_Stand);
							frame=0;
						}else 
						if(NAM->Code==37){//rest A1
							NAM=OB->newMons->GetAnimation(anm_Stand+1);
							frame=0;
						}
					}
					if(V>0){
						AddAnimation(OB->RealX/16.0f,OB->RealY/16.0f,OB->RZ+OB->OverEarth,NAM,
										frame,float(OB->RealDirPrecise)/256.0f,0x00808080+(V<<24),OB,float(Scale)/256.0,0,0,0x23000000|OB->Serial);
					}
				}
				DrawActiveAbility(OB);
				//DrawMarker(OB);
			}
		}
	}

    //  cycle on buildings
    //  widen cell bounds, because big buildings can span more than one cell
    CX0 -= 5; CX1 += 5;
    CY0 -= 5; CY1 += 5;
    for (int dx = CX0; dx < CX1; dx++)
    {
        for (int dy = CY0; dy < CY1; dy++)
        {
            int cell = 1 + dx + ((dy + 1)<<VAL_SHFCX);
            if(cell >= VAL_MAXCIOFS) continue;
            WORD GetOneBld( int cell, int pos = 0 );
            WORD MID;
            for (int pos = 0; (MID = GetOneBld(cell,pos)) != 0xFFFF; pos++)
            {				
                OneObject* OB=Group[MID];
                DWORD nationalColor = 0x00000000;
                if (OB)
                {
                    nationalColor = GetNatColor( OB->NNUM );
                }
                if (!OB || !OB->NewBuilding || OB->Hidden) continue;				
                
                NewMonster* NM = OB->newMons;

				word Scale=OB->ObjectScale;
				if(OB->ActiveAbility) OB->ActiveAbility->ActiveAbilities.modifyScale(OB->newMons->StartScale,Scale);

                if (!CheckObjectVisibilityOnScreen( OB,float(Scale)/256.0 )) continue;
                if (!OB->TempFlag&&!GetObjectVisibilityInFog(OB->RealX>>4,OB->RealY>>4,OB->RZ+OB->OverEarth,OB)) continue;

                OB->WasDrawn=true;
                OB->IsInFog=GetInterpFOW(OB->RealX>>4,OB->RealY>>4)<600;

                if(OB->NewBuilding && !OB->IsInFog)OB->TempFlag=1;
                if(OB->ImSelected&(1<<MyNation)){
                    int dc=int(sin(float(GetTickCount()%100000)/200.0f)*50.0f)+0x40;
                    GPS.SetCurrentDiffuse(0xFFC0C0C0-dc-(dc<<8)-(dc<<16));
                }else GPS.SetCurrentDiffuse(0xFF808080);
                if(OB->LoLayer){
                    DWORD C=0xFF808080;
                    if(OB->ImSelected&(1<<MyNation)){
                        int dc=int(sin(float(GetTickCount()%100000)/200.0f)*50.0f)+0x40;
                        C=0xFFC0C0C0-dc-(dc<<8)-(dc<<16);
                    }
                    if(OB->Sdoxlo>2000-150){
                        if(OB->Sdoxlo>2000){
                            C&=0xFFFFFF;
                        }else{
                            C&=0xFFFFFF;
                            C|=((2000-OB->Sdoxlo)*255/150)<<24;
                        }
                    }
                    AddAnimation(OB->RealX/16.0f,OB->RealY/16.0f,OB->RZ+OB->OverEarth,OB->LoLayer,(int(OB->Sdoxlo)<<8)+int(OB->SdoxloLow),0,C,OB,float(Scale)/256.0,0,0,0x23000000&OB->Serial);
                }else
                if(OB->NewAnm){
                    //OB->LoLayer->DrawAt(0,OB->NNUM,OB->RealX>>4,OB->RealY>>4,OB->RZ,OB->RealDir,1.0f,0,0,OB);
                    extern int AnimTime;
                    AddAnimation(OB->RealX/16.0f,OB->RealY/16.0f,OB->RZ+OB->OverEarth,OB->NewAnm,OB->NewAnm->NFrames?(AnimTime<<1)%(OB->NewAnm->NFrames<<8):0,0,0xFF808080,OB,float(Scale)/256.0,0,0,0x23000000|OB->Serial);
                }
                if(OB->Sdoxlo && OB->Sdoxlo<240 && !OB->NewBuilding)
                {
                    NewAnimation* NA=OB->newMons->GetAnimation(anm_StandLo);
                    if(NA){
                        Vector3D V0(OB->RealX>>4,OB->RealY>>4,OB->RZ+OB->OverEarth);
                        Vector3D CP=ICam->GetPosition();
                        Vector3D V1=CP;
                        Vector3D V2=SkewPt(OB->RealX>>4,OB->RealY>>4,OB->RZ);
                        V1-=V2;
                        V1.normalize();
                        V1*=8;
                        V0+=V1;
                        AddAnimation(V0.x,V0.y,V0.z,NA,0,0,((255-(OB->Sdoxlo*255)/240)<<24)+0x808080,OB,float(Scale)/256.0,0,0,0x23000000|OB->Serial);
                    }
                }
                xx=(OB->RealX>>(4+zoomsh))-x0;
                yy=(OB->RealY>>(5+zoomsh))-y0;
                int zz=yy;
                if(Mode3D)zz-=zoom(OB->RZ);
                int xx0=xx+zoom(NM->PicDx);
                int yy0=zz+zoom(NM->PicDy);				
                //if(!OB->Sdoxlo)ShowFires(OB,xx0,yy0);
                if(OB->newMons->Port){
                    void ShowProducedShip(OneObject* Port,int CX,int CY);
                    ShowProducedShip(OB,OB->WallX,OB->WallY);
                }
				DrawActiveAbility(OB);
				//DrawMarker(OB);
            }
        }
    } // for (int dx = CX0; dx < CX1; dx++)	

	//  disable z-buffer to let interface elements be drawn in overlay mode
	GPS.EnableZBuffer( false );
} // DrawUnits

void RegisterVisibleGP(OneObject* OB,int gpID,int sprID,int x,int y, float scale);

DynArray<int> SprCollection;

float	FallStage	= 0.0f;
int		FallAxeY	= 0;
bool	CollectMode	= 0;
float	GlobalScale	= 1.0f;

inline void DrawFallingSprite(int x,int y, float z, int FileID,int Sprite,byte NI,int FAxeY=FallAxeY,float FStage=FallStage)
{
	if(CollectMode)
	{
		SprCollection.Add(x);
		SprCollection.Add(y);
		SprCollection.Add(FileID);
		SprCollection.Add(Sprite);
		SprCollection.Add(NI);
		SprCollection.Add(FAxeY);
		SprCollection.Add(*((int*)&FStage));
		SprCollection.Add(*((int*)&z));		
	}
	GPS.SetScale( GlobalScale );
	
	if (fabs( FStage ) < 0.04)
	{
		GPS.ShowGP( x, y, z, FileID, Sprite, NI );
		return;
	}

	Plane plane;
	plane.from3Points(	Vector3D( 0, FAxeY, 0 ), 
						Vector3D( 1, FAxeY, 0 ),
						Vector3D( 0, 0, float( FAxeY ) * FStage * c_CosPId6 * 2.0f / GetZRange()) );
	//GPS.ShowGPAligned( x, y, z, plane, FileID, Sprite, NI );
} // DrawFallingSprite

struct WSprite
{
	int			m_FileID;
	int			m_FrameID;
	DWORD		m_NatColor;
	Matrix4D	m_TM;
	DWORD		Diffuse;
};

const int c_MaxWorldSprites = 8192;
WSprite WorldCollection[c_MaxWorldSprites];
int NWorldSprites = 0;
DWORD CurDiffuse=0;
void DrawWorldSprite( int FileID,int SpriteID, const Matrix4D& tm, byte NI )
{
	DWORD color = GetNatColor( NI );

	if(CollectMode)
	{
		WSprite& ws = WorldCollection[NWorldSprites++];
		assert(NWorldSprites < c_MaxWorldSprites);
		ws.m_FileID = FileID;
		ws.m_FrameID = SpriteID;
		ws.m_NatColor = color;
		ws.m_TM = tm;
		ws.Diffuse=(GPS.GetCurrentDiffuse()&0xFF000000)+0x808080;
	}

	GPS.DrawWSprite( FileID, SpriteID, tm, color );
}

void ShowCollection()
{
	CollectMode=0;
	int N=SprCollection.GetAmount();
	int* iptr=SprCollection.GetValues();
	for(int i=0;i<N;i+=8){
		DrawFallingSprite(iptr[0],iptr[1],*((float*)(iptr+7)), iptr[2],iptr[3],iptr[4],iptr[5],*((float*)(iptr+6)));
		iptr+=8;
	}

	for (int i = 0; i < NWorldSprites; i++)
	{
		WSprite& ws = WorldCollection[i];
		GPS.SetCurrentDiffuse(ws.Diffuse);
		GPS.DrawWSprite( ws.m_FileID, ws.m_FrameID, ws.m_TM, ws.m_NatColor );
	}
	GPS.FlushBatches();
}

extern int NWorldSprites;
void ClearCollection()
{
	SprCollection.NValues=0;
	NWorldSprites = 0;
}

void SetCollectMode(int mode)
{
    CollectMode=mode;
}

extern bool CINFMOD;
//--------------------------------------------------------------------------
//	Func:	DrawDebugBuildingInfo
//	Desc:	Draws building extents
//--------------------------------------------------------------------------
void DrawDebugBuildingInfo( const OneObject* OB )
{
	if(CINFMOD && OB && OB->NewBuilding && !LMode)
	{
		NewMonster* NM=OB->newMons;
		int CX=OB->RealX>>4;
		int CY=OB->RealY>>4;
		int X0=CX+NM->BuildX0;
		int Y0=CY+NM->BuildY0;
		int X1=CX+NM->BuildX1;
		int Y1=CY+NM->BuildY1;
		int D=(Y1-Y0+X1-X0)>>1;
		void DrawWorldLine(float x0,float y0,float x1,float y1,DWORD Diffuse);
		DrawWorldLine(X0,Y0,X0+D,Y0+D,0xFF00FF00);
		DrawWorldLine(X0,Y0,X1-D,Y1-D,0xFF00FF00);
		DrawWorldLine(X1,Y1,X0+D,Y0+D,0xFF00FF00);
		DrawWorldLine(X1,Y1,X1-D,Y1-D,0xFF00FF00);
	};
} // DrawDebugBuildingInfo

extern word TransparentBuildingID;
extern word PrevTransparentBuildingID;
extern word TransparentBuildingAlpha;
DWORD ModDWORD2X(DWORD C1,DWORD C2);
//--------------------------------------------------------------------------
//	Func:	VariateUnitColor
//	Desc:	Adjusts unit color to make it look slightly different
//--------------------------------------------------------------------------
void VariateUnitColor( OneObject* OB, DWORD& Color ){
	if(!OB || !OB->newMons) return;

	//checking for hilighting
	if( !OB->newMons->NewParams.PortretDisable ){
		byte M=OB->HighlightMask;
		OB->HighlightMask=0;
		DWORD CA=Color&0xFF000000;
		if(M){
			if(M&1){//enemy
				Color=CA | (EngSettings.EnemyHighliting & 0xFFFFFF);
			}else
			if(M&2){//friend
				Color=CA | (EngSettings.AllyHighliting & 0xFFFFFF);
			}else
			if(M&4){//me
				Color=CA | (EngSettings.FriendsHighliting & 0xFFFFFF);
			}			
		}
	}

	// Transparence
	if(OB){
		extern bool TransMode;
		if(OB->NewBuilding && TransMode){
			Color=(0x80<<24) | (Color & 0x00FFFFFF);            
		}else{
			int id=OB->Index;
			if(id==TransparentBuildingID||id==PrevTransparentBuildingID){		
				Color=((255-DWORD(TransparentBuildingAlpha/40))<<24) | (Color & 0x00FFFFFF);
			}
		}
	}

	if(OB->ImSelected&(1<<MyNation)){
		switch(GSets.SVOpt.SelectionType){
			case 1://Blinking
				{
					DWORD C=0;
					DWORD c1=Color;
					extern int SelColor;
					int dc=SelColor;
					for(int i=0;i<3;i++){
						int CC=(c1&255);
						CC=(CC*dc)>>7;
						if(CC>255)CC=255;
						c1>>=8;
						C|=DWORD(CC)<<(i<<3);
					}
					Color=C|(Color&0xFF000000);
					return;
				}
			case 2://red clor
				{
					if(OB->NMask&(1<<GSets.CGame.cgi_NatRefTBL[MyNation])){
						Color=0xFF00FF00;
					}else{
						Color=0xFFFF0000;
					}
					return;
				}
				return;
		}
	}

	int dc=OB->newMons->ColorVariation;
	if(dc){
		extern short randoma[8192];
		int idx=OB->Index;
		idx=idx+(idx/47);
		if(idx&1){
			dc=136+((idx*13177)&8191%(dc));
		}else{
			dc=136-((idx*13177)&8191%(dc));
		}
		DWORD C=0;
		DWORD c1=Color;
		for(int i=0;i<3;i++){
            int CC=(c1&255);
			CC=(CC*dc)>>7;
			if(CC>255)CC=255;
            c1>>=8;
			C|=DWORD(CC)<<(i<<3);
		}
		Color=C|(Color&0xFF000000);
	}	
	DWORD TF=OB->newMons->TFactor;
	DWORD ModDWORD2XRGB(DWORD C1,DWORD C2);
	if(TF!=0)Color=ModDWORD2XRGB(Color,TF);
} // VariateUnitColor

//--------------------------------------------------------------------------
//	Func:	IsVisible
//	Desc:	Whether OneObject instance is visible for nation
//--------------------------------------------------------------------------
bool IsVisible( OneObject* OB, int nationIdx )
{
	if (!OB) return true;
	if (OB->Invisible) return false;
	if (OB->InvisibleForEnemy && !(OB->NMask&NATIONS[nationIdx].NMask)) return false;
	return true;
} // IsVisible

DWORD GetNatColor( int );

enum AnimType
{
	at2D		        = 0,
	at3D		        = 1,
	atPatch		        = 2,
    atComplexShadow2D   = 3,
}; // enum AnimType

const int	c_AlignGround	= -10000; 
const int	c_AlignTopmost	= 10000;
void RegisterVisibleGP( OneObject* OB, int modelID, const Matrix4D& tm );
void ShowFiresNearBuilding(OneObject* OB,Matrix4D& M4, float planefactor)
{
    if(!OB||OB->MaxLife==0)return;
    if(OB->Stage<OB->NStages)return;
	if(OB->Sdoxlo>255)return;
	Matrix4D Mi;
	Mi.inverse(M4);
	int x=OB->newMons->PicDx;
	int y=OB->newMons->PicDy;
	int LP=100-OB->Life*100/OB->MaxLife;
	if(LP==0)return;
	Matrix4D LW2W=ScreenToWorldSpace();
	LW2W.mulLeft(M4);
	for(int k=0;k<2;k++){
        int NF=OB->newMons->NFires[k]*LP/100;
		for(int j=0;j<NF;j++){            
			int mid=-1;
			float scale=1.0f;
			int sid=-1;
			if (k==0){
				int nf=EngSettings.FiresList.GetAmount();
				if(nf){
					int id=j%nf;
                    mid=EngSettings.FiresList[id]->ModelID;
					scale=EngSettings.FiresList[id]->Scale;
					sid=EngSettings.FiresList[id]->SoundID;
				}
			}else{
				int nf=EngSettings.SmokeList.GetAmount();
				if(nf){
					int id=j%nf;
					mid=EngSettings.SmokeList[id]->ModelID;
					scale=EngSettings.SmokeList[id]->Scale;
					sid=EngSettings.SmokeList[id]->SoundID;
				}
			}
			if(mid!=-1){
				Matrix4D M;
				M.scaling(scale);
                //Vector3D pos = SkewPt(  OB->RealX/16 + x + OB->newMons->FireX[k][j],
                //                      OB->RealY/16,
                //                      OB->RZ - planefactor*(y + OB->newMons->FireY[k][j]) );
				Vector4D pos (  x + OB->newMons->FireX[k][j],0,-y - OB->newMons->FireY[k][j],1 );
                pos*=LW2W;
				pos.normW();

				M.setTranslation( pos );

				PushEntityContext(j*713+173+k*9775+OB->Index*12347);
				int id=IEffMgr->InstanceEffect(mid);				
				IEffMgr->SetAlphaFactor(id,float(255-OB->Sdoxlo)/256);
				IEffMgr->UpdateInstance(id,M);
				PopEntityContext();
				if(sid>0){
					extern CDirSound* CDS;
					CDS->HitSound(sid);
					AddEffect(OB->RealX/16,OB->RealY/16,sid);
				}
			}
		}
	}
}

const int c_NormalStep = 32.0f;
Vector3D GetTotalNormal( int x, int y )
{
    float ndenom = 0.5f / c_NormalStep;

    float l = GetTotalHeight( x - c_NormalStep, y );		
    float r = GetTotalHeight( x + c_NormalStep, y );		
    float u = GetTotalHeight( x, y - c_NormalStep );		
    float d = GetTotalHeight( x, y + c_NormalStep );		

    Vector3D normal;
    normal.x = (l - r) * ndenom;
    normal.y = (u - d) * ndenom;
    normal.z = 2.0f;
    normal.normalize();
    return normal;
}
class Matrix2DF{
public:
	float e00,e01,e10,e11;
	Matrix2DF(float a,float b,float c,float d){
		e00=a;
		e01=b;
		e10=c;
		e11=d;
	}
	Matrix2DF(){

	}
	Matrix2DF& operator = (Matrix2DF& M){
		Matrix2DF(M.e00,M.e01,M.e10,M.e11);
		return M;
	}
	float Inverse(Matrix2DF& M){
		float D=e00*e11-e01*e10;		
		M.e00=e11/D;
		M.e01=-e01/D;
		M.e10=-e10/D;
		M.e11=e00/D;
		return D;
	}
	Vector2D mul(Vector2D V){
		Vector2D Vd;
		Vd.x=V.x*e00+V.y*e10;
		Vd.y=V.x*e01+V.y*e11;
		return Vd;
	}
};
Matrix4D GetPseudoProjectionTM(const Vector3D& pos,NewMonster* NM,float& planeFactor){
	if(NM->Use3pAlign){
		float p=0;
		Matrix4D m=GetPseudoProjectionTM(pos,p);		

		int dx=NM->PicDx;
		int dy=NM->PicDy;

		Vector3D V1=SkewPt(dx+NM->AlignPt1x,(dy+NM->AlignPt1y+NM->AlignPt1z)*2,NM->AlignPt1z);
		Vector3D V2=SkewPt(dx+NM->AlignPt2x,(dy+NM->AlignPt2y+NM->AlignPt2z)*2,NM->AlignPt2z);
		Vector3D V3=SkewPt(dx+NM->AlignPt3x,(dy+NM->AlignPt3y+NM->AlignPt3z)*2,NM->AlignPt3z);

		int f1=NM->AlignPt1y;
		int f2=NM->AlignPt2y;
		int f3=NM->AlignPt3y;

		if(f1<f2&&f1<f3){
			swap(V3.x,V1.x);
			swap(V3.y,V1.y);
			swap(V3.z,V1.z);
		}else
			if(f2<f1&&f2<f3){
				swap(V2.x,V3.x);
				swap(V2.y,V3.y);
				swap(V2.z,V3.z);
			}

			float fx=(V3.x-V1.x)/(V2.x-V1.x);
			Vector3D V4=V2;
			V4*=fx;
			Vector3D V=V1;
			V*=(1-fx);
			V4+=V;

			Vector3D S1=V1;
			Vector3D S2=V2;
			Vector3D S3=V3;
			Vector3D P=pos;
			S1+=P;
			S2+=P;
			S3+=P;

			WorldToScreenSpace(S1);		
			WorldToScreenSpace(S2);
			WorldToScreenSpace(S3);

			m.transformPt(V1);
			m.transformPt(V2);
			m.transformPt(V3);
			m.transformPt(V4);

			float DY=V3.y-V4.y;
			float DX=V3.x-V4.x;

			if( fabs(DX) > fabs(DY) ){
				float A=-DY/DX;
				Matrix2DF M2(V1.x*A+V1.y,V2.x*A+V2.y,1,1);
				Matrix2DF M2I;
				M2.Inverse(M2I);
				Vector2D S(S1.x,S2.x);
				Vector2D K1020;
				K1020 = M2I.mul(S);

				float K00=A*K1020.x;
				Vector3D K011121 (S1.y,S2.y,S3.y);
				Matrix3D M3( V1.x, V2.x,    V3.x,
					V1.y, V2.y, V3.y,
					1   ,    1,    1);
				M3.inverse();
				K011121*=M3;			
				Matrix4D K
					(    K00, K011121.x, 0, 0,
					K1020.x, K011121.y, 0, 0,
					0,         0, 1, 0,
					K1020.y, K011121.z, 0, 1);
				if(K.e11<0||K.e11>3||K.e00<0||K.e00>3){
					K.setIdentity();
				}
				m*=K;
				return m;
			}else{
				float B=-DX/DY;
				Matrix2DF M2(V1.x+V1.y*B,V2.x+V2.y*B,1,1);
				Matrix2DF M2I;
				M2.Inverse(M2I);
				Vector2D S(S1.x,S2.x);
				Vector2D K0020;
				K0020 = M2I.mul(S);
				float K10=B*K0020.x;

				float DS3Y=-(S2.y-S1.y)/(S2.x-S1.x)*(V3.x*K0020.x+V3.y*K10+K0020.y-S3.x);

				Vector3D K011121 (S1.y,S2.y,S3.y+DS3Y);
				Matrix3D M3( V1.x, V2.x,    V3.x,
					V1.y, V2.y, V3.y,
					1   ,    1,    1);
				M3.inverse();
				K011121*=M3;

				Matrix4D K
					(K0020.x, K011121.x, 0, 0,
					K10, K011121.y, 0, 0,
					0,         0, 1, 0,
					K0020.y, K011121.z, 0, 1);
				if(K.e11<0||K.e11>2||K.e00<0||K.e00>2){
					K.setIdentity();
				}
				m*=K;
				return m;
			}		
	}else{
		return GetPseudoProjectionTM(pos,planeFactor);
	}
}
Matrix4D GetPseudoProjectionTM4(const Vector3D& pos,NewMonster* NM,float& planeFactor){
	if(NM->Use3pAlign){
		float p=0;
		Matrix4D m=GetPseudoProjectionTM(pos,p);		

		int dx=NM->PicDx;
		int dy=NM->PicDy;

		Vector3D V1=SkewPt(dx+NM->AlignPt1x,(dy+NM->AlignPt1y+NM->AlignPt1z)*2,NM->AlignPt1z);
		Vector3D V2=SkewPt(dx+NM->AlignPt2x,(dy+NM->AlignPt2y+NM->AlignPt2z)*2,NM->AlignPt2z);
		Vector3D V3=SkewPt(dx+NM->AlignPt3x,(dy+NM->AlignPt3y+NM->AlignPt3z)*2,NM->AlignPt3z);

		int f1=NM->AlignPt1y;
		int f2=NM->AlignPt2y;
		int f3=NM->AlignPt3y;

		if(f1<f2&&f1<f3){
			swap(V3.x,V1.x);
			swap(V3.y,V1.y);
			swap(V3.z,V1.z);
		}else
		if(f2<f1&&f2<f3){
			swap(V2.x,V3.x);
			swap(V2.y,V3.y);
			swap(V2.z,V3.z);
		}

		float fx=(V3.x-V1.x)/(V2.x-V1.x);
		Vector3D V4=V2;
		V4*=fx;
		Vector3D V=V1;
		V*=(1-fx);
		V4+=V;

		Vector3D S1=V1;
		Vector3D S2=V2;
		Vector3D S3=V3;
		Vector3D P=pos;
		S1+=P;
		S2+=P;
		S3+=P;

		WorldToScreenSpace(S1);		
		WorldToScreenSpace(S2);
		WorldToScreenSpace(S3);

		m.transformPt(V1);
		m.transformPt(V2);
		m.transformPt(V3);
		m.transformPt(V4);

		float DY=V3.y-V4.y;
		float DX=V3.x-V4.x;

		if( fabs(DX) > fabs(DY) ){
			float A=-DY/DX;
			Matrix2DF M2(V1.x*A+V1.y,V2.x*A+V2.y,1,1);
			Matrix2DF M2I;
			M2.Inverse(M2I);
			Vector2D S(S1.x,S2.x);
			Vector2D K1020;
			K1020 = M2I.mul(S);

			float K00=A*K1020.x;
			Vector3D K011121 (S1.y,S2.y,S3.y);
			Matrix3D M3( V1.x, V2.x,    V3.x,
				V1.y, V2.y, V3.y,
				1   ,    1,    1);
			M3.inverse();
			K011121*=M3;			
			Matrix4D K
				(    K00, K011121.x, 0, 0,
				K1020.x, K011121.y, 0, 0,
				0,         0, 1, 0,
				K1020.y, K011121.z, 0, 1);
			if(K.e11<0||K.e11>3||K.e00<0||K.e00>3){
				K.setIdentity();
			}
			m*=K;
			return m;
		}else{
			float B=-DX/DY;
			Matrix2DF M2(V1.x+V1.y*B,V2.x+V2.y*B,1,1);
			Matrix2DF M2I;
			M2.Inverse(M2I);
			Vector2D S(S1.x,S2.x);
			Vector2D K0020;
			K0020 = M2I.mul(S);
			float K10=B*K0020.x;

			Vector3D K011121 (S1.y,S2.y,S3.y);
			Matrix3D M3( V1.x, V2.x,    V3.x,
				V1.y, V2.y, V3.y,
				1   ,    1,    1);
			M3.inverse();
			K011121*=M3;

			Matrix4D K
				(K0020.x, K011121.x, 0, 0,
				K10, K011121.y, 0, 0,
				0,         0, 1, 0,
				K0020.y, K011121.z, 0, 1);
			if(K.e11<0||K.e11>2||K.e00<0||K.e00>2){
				K.setIdentity();
			}
			m*=K;
			return m;
		}		
	}else{
		return GetPseudoProjectionTM(pos,planeFactor);
	}
}
Matrix4D GetPseudoProjectionTM3(const Vector3D& pos,NewMonster* NM,float& planeFactor){
	if(NM->Use3pAlign){
		float p=0;
		Matrix4D m=GetPseudoProjectionTM(pos,p);		

		int dx=NM->PicDx;
		int dy=NM->PicDy;

		Vector3D V1=SkewPt(dx+NM->AlignPt1x,(dy+NM->AlignPt1y+NM->AlignPt1z)*2,NM->AlignPt1z);
		Vector3D V2=SkewPt(dx+NM->AlignPt2x,(dy+NM->AlignPt2y+NM->AlignPt2z)*2,NM->AlignPt2z);
		Vector3D V3=SkewPt(dx+NM->AlignPt3x,(dy+NM->AlignPt3y+NM->AlignPt3z)*2,NM->AlignPt3z);

		int f1=NM->AlignPt1y;
		int f2=NM->AlignPt2y;
		int f3=NM->AlignPt3y;

		if(f1<f2&&f1<f3){
			swap(V3.x,V1.x);
			swap(V3.y,V1.y);
			swap(V3.z,V1.z);
		}else
			if(f2<f1&&f2<f3){
				swap(V2.x,V3.x);
				swap(V2.y,V3.y);
				swap(V2.z,V3.z);
			}

			float fx=(V3.x-V1.x)/(V2.x-V1.x);
			Vector3D V4=V2;
			V4*=fx;
			Vector3D V=V1;
			V*=(1-fx);
			V4+=V;

			Vector3D S1=V1;
			Vector3D S2=V2;
			Vector3D S3=V3;
			Vector3D P=pos;
			S1+=P;
			S2+=P;
			S3+=P;

			WorldToScreenSpace(S1);		
			WorldToScreenSpace(S2);
			WorldToScreenSpace(S3);

			m.transformPt(V1);
			m.transformPt(V2);
			m.transformPt(V3);
			m.transformPt(V4);

			float DY=V3.y-V4.y;
			float DX=V3.x-V4.x;

			if( fabs(DX) > fabs(DY) ){
				float A=-DY/DX;
				Matrix2DF M2(V1.x*A+V1.y,V2.x*A+V2.y,1,1);
				Matrix2DF M2I;
				M2.Inverse(M2I);
				Vector2D S(S1.x,S2.x);
				Vector2D K1020;
				K1020 = M2I.mul(S);

				Vector3D K011121 (S1.y,S2.y,S3.y-V3.x*K1020.x);
				Matrix3D M3( V1.x, V2.x,    0,
					V1.y, V2.y, V3.y,
					1   ,    1,    1);
				M3.inverse();
				K011121*=M3;
				float K00=A*K1020.x;
				Matrix4D K
					(    K00, K011121.x, 0, 0,
					K1020.x, K011121.y, 0, 0,
					0,         0, 1, 0,
					K1020.y, K011121.z, 0, 1);
				if(K.e11<0||K.e11>3||K.e00<0||K.e00>3){
					K.setIdentity();
				}
				m*=K;
				return m;
			}else{
				float B=-DX/DY;
				Matrix2DF M2(V1.x+V1.y*B,V2.x+V2.y*B,1,1);
				Matrix2DF M2I;
				M2.Inverse(M2I);
				Vector2D S(S1.x,S2.x);
				Vector2D K0020;
				K0020 = M2I.mul(S);
				float K10=B*K0020.x;

				Vector3D K011121 (S1.y,S2.y,S3.y-V3.x*K10);
				Matrix3D M3( V1.x, V2.x,    0,
					V1.y, V2.y, V3.y,
					1   ,    1,    1);
				M3.inverse();
				K011121*=M3;

				Matrix4D K
					(K0020.x, K011121.x, 0, 0,
					K10, K011121.y, 0, 0,
					0,         0, 1, 0,
					K0020.y, K011121.z, 0, 1);
				if(K.e11<0||K.e11>2||K.e00<0||K.e00>2){
					K.setIdentity();
				}
				m*=K;
				return m;
			}		
	}else{
		return GetPseudoProjectionTM(pos,planeFactor);
	}
}
Matrix4D GetPseudoProjectionTM2(const Vector3D& pos,NewMonster* NM,float& planeFactor){
	if(NM->Use3pAlign){
		float p=0;
        Matrix4D m=GetPseudoProjectionTM(pos,p);		

		int dx=NM->PicDx;
		int dy=NM->PicDy;

		Vector3D V1=SkewPt(dx+NM->AlignPt1x,(dy+NM->AlignPt1y+NM->AlignPt1z)*2,NM->AlignPt1z);
		Vector3D V2=SkewPt(dx+NM->AlignPt2x,(dy+NM->AlignPt2y+NM->AlignPt2z)*2,NM->AlignPt2z);
		Vector3D V3=SkewPt(dx+NM->AlignPt3x,(dy+NM->AlignPt3y+NM->AlignPt3z)*2,NM->AlignPt3z);

		Vector3D V1N=V1;
		Vector3D V2N=V2;
		Vector3D V3N=V3;
		Vector3D P=pos;
		V1+=P;
		V2+=P;
		V3+=P;

		WorldToScreenSpace(V1);		
		WorldToScreenSpace(V2);
		WorldToScreenSpace(V3);
		//WorldToScreenSpace(P);
		//V1-=P;
		//V2-=P;
		//V3-=P;

		Matrix3D XW(
			V1.x,V1.y,1,
			V2.x,V2.y,1,
			V3.x,V3.y,1);

		m.transformPt(V1N);
		m.transformPt(V2N);
		m.transformPt(V3N);

		Matrix3D X(
			V1N.x,V1N.y,1,
			V2N.x,V2N.y,1,
			V3N.x,V3N.y,1);
        X.inverse();
		X*=XW;
		Matrix4D K(
			X.e00,X.e01,0,0,
			X.e10,X.e11,0,0,
			    0,    0,1,0,
			X.e20,X.e21,0,1);
		m*=K;
		m.e10=0;
		return m;
		/*
		int dx=NM->PicDx;
		int dy=NM->PicDx;
        Vector3D V1=SkewPt(pos.x+dx+NM->AlignPt1x,pos.y+(dy+NM->AlignPt1y+NM->AlignPt1z)*2,pos.z+NM->AlignPt1z);
		Vector3D V2=SkewPt(pos.x+dx+NM->AlignPt2x,pos.y+(dy+NM->AlignPt2y+NM->AlignPt2z)*2,pos.z+NM->AlignPt2z);
		Vector3D V3=SkewPt(pos.x+dx+NM->AlignPt3x,pos.y+(dy+NM->AlignPt3y+NM->AlignPt3z)*2,pos.z+NM->AlignPt3z);
		Vector3D V4=SkewPt(pos.x,pos.y,pos.z);

		Matrix4D MR0(V1.x, V1.y, V1.z, 1.0f, 
			V2.x, V2.y, V2.z, 1.0f,
			V3.x, V3.y, V3.z, 1.0f,
			V4.x, V4.y, V4.z, 1.0f);

		Matrix4D MR(V1.x, V1.y, V1.z, 1.0f, 
					V2.x, V2.y, V2.z, 1.0f,
					V3.x, V3.y, V3.z, 1.0f,
					V4.x, V4.y, V4.z, 1.0f);
		WorldToScreenSpace(V1);		
		WorldToScreenSpace(V2);
		WorldToScreenSpace(V3);
		WorldToScreenSpace(V4);
		Matrix4D MS(V1.x, V1.y, V1.z, 1.0f, 
					V2.x, V2.y, V2.z, 1.0f,
					V3.x, V3.y, V3.z, 1.0f,
					V4.x, V4.y, V4.z, 1.0f );
		MR.inverse();
		MS.mulLeft(MR);
		MR0*=MS;
		return MS;
		*/
	}else{
        return GetPseudoProjectionTM(pos,planeFactor);
	}
}
bool g_bRenderShadowsPass = false;
extern Rct g_ScreenViewport;
extern int ExtraDir;
void PlayAnimationEx(OneObject* OB,NewAnimation* NA,int Frame,int x,int y);
NewAnimation* LastDrawnNA = NULL;
int LastFrame = 0;
void DrawCollision(OneObject* OB);
extern byte LockGrid;

void NewAnimation::DrawSpriteUnit( OneObject* OB, const Vector3D& pos, int frame, float Dir, byte NI , float Scale )
{
    Vector3D wPos = SkewPt( pos.x, pos.y, pos.z );
    Vector3D sPos( wPos );
    WorldToScreenSpace( sPos );

    sPos.x = roundf( sPos.x );
    sPos.y = roundf( sPos.y );
    DWORD nationalColor = GetNatColor( NI );

    //  force to skip some frames according to animation quality
    extern int CurrentAnmQuality;
    if (CurrentAnmQuality > 0)
    {
        int r = CurrentAnmQuality + 1;
        frame = (((frame>>8)/r)*r) << 8;
    }

    //  frame index vodoo
    byte RealDir=byte(Dir);
    int octs,oc2,sesize,oc1,ocM,dir;				
    if(OB&&(Rotations==16||Rotations==9))
    {
        if(OB->OctantInfo==0xFF)
        {
            OB->OctantInfo=(OB->RealDir+8)>>4;
        }else{
            int cd=int(OB->OctantInfo&15)<<4;
            int dd=int(char(char(cd)-char(OB->RealDir)));
            int ad=abs(dd);
            if(ad<=8){
                OB->OctantInfo&=0x0F;
                RealDir=OB->OctantInfo<<4;
            }else if(ad<16){
                int ot=OB->OctantInfo>>4;
                if(ot<12){
                    RealDir=(OB->OctantInfo&15)<<4;
                    ot+=ad>>3;
                    OB->OctantInfo=(OB->OctantInfo&15)+(ot<<4);
                }else{
                    OB->OctantInfo=(OB->RealDir+8)>>4;
                    RealDir=OB->OctantInfo<<4;
                }
            }else{
                OB->OctantInfo=(OB->RealDir+8)>>4;
                RealDir=OB->OctantInfo<<4;
            }
        }
    }

    RealDir-=ExtraDir;

    if (Inverse) RealDir=128-RealDir;
    if (Rotations == 1)
    {
        octs=Rotations;
        oc2=Rotations;
        ocM=0;
        if(!octs)octs=1;
        sesize=0;
        oc1=octs;
        dir=0;
    }
    else if (Rotations & 1)
    {
        octs=(Rotations-1)*2;
        oc2=Rotations-1;
        if(!octs)octs=1;
        sesize=255/(octs*2);
        oc1=octs;
        ocM=oc2;
        dir=(((RealDir+64+sesize)&255)*octs)>>8;
    }
    else
    {				
        octs=Rotations;
		oc2=Rotations;
        ocM=0;
        if(!octs)octs=1;
        sesize=128/octs;
        oc1=octs;
        dir=(((RealDir+64+sesize+128)&255)*octs)>>8;
    }			

    byte dir2=dir;
    if (frame/256 >= NFrames)
    {
        return;
    }
	if(Frames.GetAmount()==0)return;

    NewFrame* NF=Frames[frame/256];

    //  fade if sdoxlo
    if (OB&&OB->Sdoxlo)
    {
        FallStage=float(OB->Sdoxlo)/50.0f;
        if(FallStage>1.0f)FallStage=1.0f;
		/*
        int C=255-int(OB->Sdoxlo)*255/EngSettings.Default_DeadUnitDisappearTime;
        if(C<0)return;
        GPS.SetCurrentDiffuse((DWORD(C)<<24)+0x00808080);
		*/
    } else FallStage = 0;

    //  prevent z-fighting of the closely standing units
    const float c_ZVariation = 4.0f;
    const float c_ZAmplitude = c_DoublePI/100.0f;
    float zVariation = 0.0f;
    if (OB && OB->BrigadeID != 0xFFFF)
    {
        rndInit( OB->Serial );
        zVariation = sinf( wPos.x*c_ZAmplitude )*c_ZVariation;
    }
    extern Vector3D g_LastDir;
    Vector3D var( g_LastDir );
    var *= zVariation; 
	Matrix3D sc;
	sc.scaling(Scale,Scale,Scale);
    if (dir < ocM)
    //  reverse
    {
        Matrix4D tm;
        if(Inverse){
            Vector3D pivot( -NF->dx, -NF->dy, 0.0f );			
            tm = (FallStage == 1.0f) ? 
                GetAlignTerraTransform( wPos, pivot )
                :	GetRolledBillboardTransform( pivot, 0.0f );			

            tm*=sc;
			tm.translate( wPos );
        }else{
            Vector3D pivot( NF->dx, -NF->dy, 0.0f );
            tm = (FallStage == 1.0f) ? 
                GetAlignTerraTransform( wPos, pivot )
                :	GetRolledBillboardTransform( pivot, 0.0f );					
            tm.getV0().reverse();
            tm.getV2().reverse();

			tm*=sc;
            tm.translate( wPos );
        }
        
        tm.translate( var );

        if (DoubleAnm)
        {
            NewFrame* NF1=Frames[(frame>>8)+NFrames];

            int sp1 = oc2 - dir + Rotations*NF->SpriteID + 4096;
            int sp2 = oc2 - dir + Rotations*NF1->SpriteID + 4096;		


            DrawWorldSprite( NF->FileID, sp1, tm, NI );	  							
            DrawWorldSprite( NF1->FileID, sp2, tm, NI );	  

            if(OB&&!OB->Sdoxlo){
                RegisterVisibleGP( OB, NF->FileID, sp1, sPos.x + zoom( NF->dx ), sPos.y + zoom( NF->dy ) );
                RegisterVisibleGP( OB, NF1->FileID, sp2, sPos.x - zoom( NF1->dx ), sPos.y + zoom( NF1->dy ) );
            }
        }
        else
        {
            int sp = oc2 - dir + Rotations*NF->SpriteID;							
            DrawWorldSprite( NF->FileID, sp, tm, NI );	  				
            if(OB&&!OB->Sdoxlo){
                RegisterVisibleGP( OB, NF->FileID, Inverse ? sp : sp + 4096, sPos.x + zoom( NF->dx ), sPos.y + zoom( NF->dy ) );
            }
        }
    }
    else
        //  non-reverse
    {
        Matrix4D tm;
        if(Inverse){
            Vector3D pivot( NF->dx, -NF->dy, 0.0f );
            tm = (FallStage == 1.0f) ? 
                GetAlignTerraTransform( wPos, pivot )
                :	GetRolledBillboardTransform( pivot, 0.0f );					
            tm.getV0().reverse();
            tm.getV2().reverse();
			tm*=sc;
            tm.translate( wPos );						
        }else{
            Vector3D pivot( -NF->dx, -NF->dy, 0.0f );
            tm = (FallStage == 1.0f) ? 
                GetAlignTerraTransform( wPos, pivot )
                :	GetRolledBillboardTransform( pivot, 0.0f );
			tm*=sc;
            tm.translate( wPos );			
        }

        tm.translate( var );
        dir=oc1-dir;
        if (DoubleAnm)
        {
            NewFrame* NF1 = Frames[(frame>>8)+NFrames];

            int sp1 = oc2 - dir + Rotations*NF->SpriteID;
            int sp2 = oc2 - dir + Rotations*NF1->SpriteID;

            DrawWorldSprite( NF->FileID, sp1, tm, NI );	  
            if(OB&&!OB->Sdoxlo){
                RegisterVisibleGP( OB, NF->FileID, sp1, sPos.x + zoom( NF->dx ), sPos.y + zoom( NF->dy ) );							
            }

            DrawWorldSprite( NF1->FileID, sp2, tm, NI );	  
            if(OB&&!OB->Sdoxlo){
                RegisterVisibleGP( OB, NF1->FileID, sp2, sPos.x + zoom( NF1->dx ), sPos.y + zoom( NF1->dy ) );
            }
        }
        else
        {
            int sp = oc2 - dir + Rotations*NF->SpriteID;
            DrawWorldSprite( NF->FileID, sp, tm, NI );	  
            if(OB&&!OB->Sdoxlo){
                RegisterVisibleGP( OB, NF->FileID, Inverse ? sp+4096 : sp, sPos.x + zoom( NF->dx ), sPos.y + zoom( NF->dy ) );
            }
        }
    }
} // DrawSpriteUnit

void NewAnimation::DrawShadowedSpriteUnit( OneObject* OB, const Vector3D& pos, int frame, float Dir, byte NI, float Scale )
{
	bool nm_deadbody=OB ? OB->newMons->NewParams.DeadBody : false;
	bool deadbody=OB ? ( OB->Sdoxlo>30 && ( NFrames == 1 || OB->NewCurSprite == NFrames-1) ) || nm_deadbody : false;
	if(deadbody){
		SpriteInst* si=get_dead_unit_inst(OB,Code);
		if(si){
			if (g_SpriteInstsDrawn.size() < c_MaxSpriteInstDrawn){
				si->m_Color=GPS.GetCurrentDiffuse();
				g_SpriteInstsDrawn.push_back(*si);
				if( OB && !OB->Sdoxlo ){
					RegisterVisibleGP( OB,0,0,0,0,0 );
				}
				return;
			}            
		}
	}
    bool DrawSH=GSets.SVOpt.Draw2DShadows && GSets.ShadowQuality<2;
    Vector3D sPos( pos );
    WorldToScreenSpace( sPos );

    sPos.x = roundf( sPos.x );
    sPos.y = roundf( sPos.y );
    DWORD nationalColor = GetNatColor( NI );
    DWORD diffuse = 0xFF808080;

    //  force to skip some frames according to animation quality
    extern int CurrentAnmQuality;
    if (GSets.AnimQuality > 0)
    {
        int r = GSets.AnimQuality + 1;
        frame = (((frame>>8)/r)*r) << 8;
    }

    byte RealDir=byte(Dir);
    int octs,oc2,sesize,oc1,ocM,dir;				
    if(OB&&(Rotations==16||Rotations==9))
    {
        if(OB->OctantInfo==0xFF)
        {
            OB->OctantInfo=(OB->RealDir+8)>>4;
        }else{
            int cd=int(OB->OctantInfo&15)<<4;
            int dd=int(char(char(cd)-char(OB->RealDir)));
            int ad=abs(dd);
            if(ad<=8){
                OB->OctantInfo&=0x0F;
                RealDir=OB->OctantInfo<<4;
            }else if(ad<16){
                int ot=OB->OctantInfo>>4;
                if(ot<12){
                    RealDir=(OB->OctantInfo&15)<<4;
                    ot+=ad>>3;
                    OB->OctantInfo=(OB->OctantInfo&15)+(ot<<4);
                }else{
                    OB->OctantInfo=(OB->RealDir+8)>>4;
                    RealDir=OB->OctantInfo<<4;
                }
            }else{
                OB->OctantInfo=(OB->RealDir+8)>>4;
                RealDir=OB->OctantInfo<<4;
            }
        }
    }

    //RealDir-=ExtraDir;

    if (Inverse) RealDir=128-RealDir;
    if (Rotations == 1)
    {
        octs=Rotations;
        oc2=Rotations;
        ocM=0;
        if(!octs)octs=1;
        sesize=0;
        oc1=octs;
        dir=0;
    }
    else if (Rotations & 1)
    {
        octs=(Rotations-1)*2;
        oc2=Rotations-1;
        if(!octs)octs=1;
        sesize=255/(octs*2);
        oc1=octs;
        ocM=oc2;
        dir=(((RealDir+64+sesize)&255)*octs)>>8;
    }
    else
    {				
        octs=Rotations;
        oc2=Rotations;
        ocM=0;
        if(!octs)octs=1;
        sesize=128/octs;
        oc1=octs;
        dir=(((RealDir+64+sesize+128)&255)*octs)>>8;
    }			

    byte dir2=dir;
    if (frame/256 >= NFrames)
    {
        return;
    }
    if(Frames.GetAmount()==0)return;

    NewFrame* NF=Frames[frame/256];

    //  fade if sdoxlo
	/*
    if (OB&&OB->Sdoxlo)
    {
        FallStage=float(OB->Sdoxlo)/50.0f;
        if(FallStage>1.0f)FallStage=1.0f;

        int C=255-int(OB->Sdoxlo)*255/EngSettings.Default_DeadUnitDisappearTime;
        if(C<0)C=0;//return;
        diffuse = (DWORD(C)<<24)+0x00808080;
    } else FallStage = 0;
	*/
	if((diffuse>>24)==0)return;

    //  prevent z-fighting of the closely standing units
    const float c_ZVariation = 4.0f;
    const float c_ZAmplitude = c_DoublePI/100.0f;
    float zVariation = 0.0f;
    if (OB && OB->BrigadeID != 0xFFFF)
    {
        rndInit( OB->Serial );
        zVariation = sinf( pos.x*c_ZAmplitude )*c_ZVariation;
    }
   extern Vector3D g_LastDir;
   Vector3D var( g_LastDir );
   var *= zVariation; 
   dir=oc1-dir;
   
   int sp = oc2 - dir + Rotations*NF->SpriteID;
   sp = NF->SpriteID;

   Matrix3D camTM = GetCamera()->GetWorldTM();
   //Vector3D viewDir( camTM.e20, camTM.e21, camTM.e22 );
   Vector3D viewDir=pos-GetCamera()->GetPosition();

   const int c_NDir = 16;
   int dirIdx = CalcDirIndex( viewDir, c_NDir );
   int lDirIdx = CalcDirIndex( g_LightDir, c_NDir );

   dirIdx = (c_NDir + dirIdx - dir)%c_NDir;
   lDirIdx = (c_NDir + lDirIdx - dir)%c_NDir;

   int lFrame = lDirIdx + sp*c_NDir;
   int bFrame = dirIdx + sp*c_NDir;
   if(OB && OB->Sdoxlo>50)lFrame=-1;

   if (g_SpriteInstsDrawn.size() < c_MaxSpriteInstDrawn)
   {
       SpriteInst& st = g_SpriteInstsDrawn.expand();
       st.m_SpID            = NF->FileID;
       st.m_Frame           = bFrame;
       st.m_ShadowFrame     = DrawSH?lFrame:-1;
       st.m_Pos             = pos;
       st.m_CX              = -NF->dx;
       st.m_CY              = -NF->dy;
       st.m_NatColor        = nationalColor;
	   st.m_Color			= GPS.GetCurrentDiffuse();
	   st.m_Scale			= Scale;
       st.m_bForceFlush     = false;
       float z0             = OB?OB->OverEarth:0;
       st.m_ShadowShift     = Vector3D(-z0*2,-z0,-z0);
	   st.camTM				= NULL;	   
	   if(nm_deadbody)st.m_fall_stage = 0;
	   else{
			if(OB && OB->Sdoxlo){
				st.m_fall_stage		= 1-(OB->Sdoxlo/30.0f);
				if(st.m_fall_stage<0)st.m_fall_stage=0;
			}else{
				st.m_fall_stage		= 1;
			}
	   }
	   

	   if(deadbody){
		   add_dead_unit_inst(st,OB,Code);
	   }
   }

   if(OB&&!OB->Sdoxlo)
   {
       extern Matrix4D g_CameraViewProjM;
       Vector3D dPos( pos.x, pos.y, pos.z + 100.0f  );
       WorldToScreenSpace( dPos );
       
       float sprScale = fabs( dPos.y - sPos.y )/100.0f;

       RegisterVisibleGP( OB, NF->FileID, Inverse ? sp + 4096 : sp, 
                          sPos.x + sprScale*NF->dx, 
                          sPos.y + sprScale*NF->dy, sprScale );
   }
   
} // DrawShadowedSpriteUnit

bool NewAnimation::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	//if (options&&!strcmp(options,"graphparam")) {
	if (AnimationType==1) {
		//dest="Animation: \"%$ModelID,value$%\"";
		//if (AnimationID!=-1) dest+=" \"%$AnimationID,value$%\"";
		dest="";
		
		char dir	[_MAX_PATH];
		char drive	[_MAX_PATH];
		char file	[_MAX_PATH];
		char ext	[_MAX_PATH];

		char* n=(char*)IMM->GetModelFileName(ModelID);
		_splitpath( n, drive, dir, file, ext );		
		dest.print("Model: \"%s%s\"",file,ext);
		
		if (AnimationID!=-1){
			n=(char*)IMM->GetModelFileName(AnimationID);
			_splitpath( n, drive, dir, file, ext );
			dest.print("\nAnimation: \"%s%s\"",file,ext);
		}
		return true;
	}else 
		if (AnimationType==2){
			dest="Animation: \"%$NFrames,value$% frames\"";
			return true;
		}
		//}
		return false;
}

typedef Hash<SpriteBuilding,1779,128> SpriteBuildingHash;
SpriteBuildingHash  g_BldHash;
extern bool g_bCameraChanged;
bool        g_bUpdateBldHash = true;
bool        g_bForceBldCache = false;
void RegisterVisibleGP( OneObject* OB, SpriteBuilding* sb );

void NewAnimation::DrawSpriteBuilding( OneObject* OB, const Vector3D& pos, int frame, byte NI )
{
    if (!g_bCameraChanged) g_bUpdateBldHash = false;
    if (g_bCameraChanged && !g_bUpdateBldHash)
    {
        g_BldHash.reset();
        g_bUpdateBldHash = true;
        g_bForceBldCache = false;
    }

    // BAAAAAAD HACK.... I hate releases
    g_bForceBldCache = true;

    Vector3D wPos = SkewPt( pos.x, pos.y, pos.z );
    Vector3D sPos( wPos );
    WorldToScreenSpace( sPos );

    sPos.x = roundf( sPos.x );
    sPos.y = roundf( sPos.y );
    DWORD nationalColor = GetNatColor( NI );

    Matrix4D m; 
    float pf = 0.0f;
    if (IsPerspCameraMode())
    {
        pf = OB ? OB->newMons->PlaneFactor : 0.0f;
        m = OB ? GetPseudoProjectionTM( wPos, OB->newMons, pf ) : 
                 GetPseudoProjectionTM( wPos, pf );
    }

    //  iterate on building parts
    int s0 = frame/256;
    int s1 = s0;
    if (LineInfo)
    {
        s0 = 0;
        s1 = NFrames - 1;
    } 

    SpriteBuilding* pPart = NULL;
    for (int i = s0; i <= s1; i++)
    {
        NewFrame* NF = Frames[i];
        int gpID = NF->FileID;
        int frID = NF->SpriteID;

        bool bNeedUpdate = (g_bUpdateBldHash || g_bForceBldCache);

        SpriteBuilding sb( OB ? OB->Index : 0xBAADF00D, i + Code*1317, gpID, frID );
        int hID = g_BldHash.find( sb );
        if (hID == NO_ELEMENT)
        {
            bNeedUpdate = true;
            hID = g_BldHash.add( sb );
        }
        SpriteBuilding* pNextPart = &g_BldHash.elem( hID );
        if (pPart) pPart->m_pNextPart = pNextPart;
        
        pPart = pNextPart;
        pPart->m_GPID    = gpID;
        pPart->m_FrameID = frID;
        if (!pPart->m_WorldPos.isEqual( wPos )) bNeedUpdate = true;

        if (bNeedUpdate)
        {
            Matrix4D fullTM = Matrix4D::identity;
            Vector3D pivot = SkewPt( -float( NF->dx ), -float( NF->dy ), 0.0f );
            if (LineInfo)
            {
                int p = i*4;
                int x1 = LineInfo[p	   ];
                int y1 = LineInfo[p + 1];
                int x2 = LineInfo[p + 2];
                int y2 = LineInfo[p + 3];

                if (x1 == c_AlignGround)
                {
                    fullTM = GetAlignGroundTransform( pivot );
                } 
                else if (x1 == c_AlignTopmost)
                {
                    fullTM = GetRolledBillboardTransform( pivot, 0 );
                }
                else
                { 
                    fullTM = GetAlignLineTransform( pivot, x1, y1, x2, y2 );
                }
            }
            else
            {
                fullTM *= GetRolledBillboardTransform( pivot, 0.0f );
                if (!IsPerspCameraMode())
                {
                    //  what is it for?..
                    fullTM.translate( SkewPt( 0, 80, 40 ) );
                }
            }

            if (!IsPerspCameraMode()) { m.translation( wPos ); m *= ICam->ToSpace( sWorld, sScreen ); }
            fullTM *= m;
            
            pPart->m_SpriteToScreenTM = fullTM;
            pPart->m_ScreenToSpriteTM.inverse( fullTM );

            //  calculate screen-space bounding frame
            Rct rct;
            ISM->GetBoundFrame( gpID, frID, rct, nationalColor );
            Vector3D rb( rct.GetRight(), rct.GetBottom(), 0.0f );
            Vector3D lt( rct.x, rct.y, 0.0f );
            fullTM.transformPt( rb );
            fullTM.transformPt( lt );

            pPart->m_ScreenBounds.x = lt.x;
            pPart->m_ScreenBounds.y = lt.y;
            
            pPart->m_ScreenBounds.w = rb.x - lt.x;
            pPart->m_ScreenBounds.h = rb.y - lt.y;

            pPart->m_WorldPos = wPos;
        }
        
        //  clip to viewport
        const Rct& bound = pPart->m_ScreenBounds;
        if (bound.x > RealLx - 2 || bound.y > RealLy - 2 || 
            bound.GetRight() < 0 || bound.GetBottom() < 0) continue;
        
        const Matrix4D& tm = pPart->m_SpriteToScreenTM;
        if (ISM->HasColorData( Frames[0]->FileID ))
        //  building with normalized color
        {
            static int nshID = IRS->GetShaderID( "building_normcolor" );
            IRS->SetTextureFactor( ISM->GetCurrentDiffuse() );
            ISM->SetCurrentShader( nshID );
            ISM->DrawNSprite( pPart->m_GPID, pPart->m_FrameID, tm, nationalColor );
            ISM->Flush();
        }
        else
        {
            ISM->DrawSprite( pPart->m_GPID, pPart->m_FrameID, tm, nationalColor );
        }

        RegisterVisibleGP( OB, pPart );

        DrawDebugBuildingInfo( OB );
        if (CINFMOD&&LineInfo)
        {
            Vector3D pivot1( -float( NF->dx ), -float( NF->dy*2 ), 0.0f );
            Vector3D lp = wPos;
            lp -= pivot1;
            WorldToScreenSpace( lp );

            int p = i*4;
            int x1 = LineInfo[p	   ] + lp.x;
            int y1 = LineInfo[p + 1] + lp.y;
            int x2 = LineInfo[p + 2] + lp.x;
            int y2 = LineInfo[p + 3] + lp.y;
            GPS.DrawLine( x1, y1, x2, y2, 0xFFFF0000 );
        }
    }
    ShowFiresNearBuilding( OB, m, pf );
} // DrawSpriteBuilding
class CurActiveBones{
public:
	int UIndex;
	OneBoneInfo* OBI;
	Matrix4D M4;	
};
DynArray<CurActiveBones> ActiveBonesList;
void ClearActiveBones(){
	ActiveBonesList.FastClear();
}
Matrix4D* GetActiveBone(int UIndex,char* BoneName){
	for(int i=0;i<ActiveBonesList.GetAmount();i++){
		CurActiveBones* AB=&ActiveBonesList[i];
		if(UIndex == AB->UIndex && AB->OBI->NodeName.equal(BoneName)) {
			return &AB->M4;
		}
	}
	return NULL;
}
bool ShowActiveBones=false;
void DrawActiveBones(){
	if(!ShowActiveBones)return;
	for(int i=0;i<ActiveBonesList.GetAmount();i++){
		CurActiveBones* AB=&ActiveBonesList[i];
		Matrix4D& M=AB->M4;
		Vector3D pos(0,0,0);
		M.transformPt(pos);
		Vector3D OX(1,0,0);
		Vector3D OY(0,1,0);
		Vector3D OZ(0,0,1);
		M.transformVec(OX);
		M.transformVec(OY);
		M.transformVec(OZ);
        OX.normalize();
		OY.normalize();
		OZ.normalize();
		DrawArrow(pos,OX,0xFFFF0000,64);
		DrawArrow(pos,OY,0xFF00FF00,64);
		DrawArrow(pos,OZ,0xFF0000FF,64);
		Vector3D m=AB->OBI->MainDir;
		M.transformVec(m);
		DrawArrow(pos,m,0xFFFFFF00,100);
	}
}
DynArray<Line3D> CurActiveBonesList;
DynArray<Line3D>* GetActiveBones(int UIndex,char* BoneMask){
	int L=strlen(BoneMask);
	CurActiveBonesList.FastClear();
	for(int i=0;i<ActiveBonesList.GetAmount();i++){
		CurActiveBones* AB=&ActiveBonesList[i];
		if(UIndex==AB->UIndex && !strncmp(BoneMask,AB->OBI->NodeName.pchar(),L)){
			Line3D L;			
			Vector3D v=AB->OBI->MainDir;
			AB->M4.transformVec(v);
			v.normalize();
			L.setOrigDir(AB->M4.getV3(),v);
			CurActiveBonesList.Add(L);
		}
	}
	return &CurActiveBonesList;
}
bool IsEffectTargeted=false;
Vector3D GlobalEffectTarget;
bool DrawSelPatchDir(float x,float y,byte Dir,int Type,float ScaleX,float ScaleY,DWORD Color);
bool _dbgDrawShotPts=false;
float GetFHeight(int x,int y);
Matrix4D GetUnitOrientation(Vector3D CurPos,float ang,int PickDX,int PickDY){	
	float _cos=cos(ang);
	float _sin=sin(ang);
	Vector3D VA[3]={Vector3D(-PickDX,-PickDY,0),Vector3D(-PickDX,PickDY,0),Vector3D(PickDX,0,0)};
	Vector3D VP[3];
	rsEnableZ(true);
	for(int i=0;i<3;i++){
		Vector3D V=CurPos+Vector3D(VA[i].x*_cos-VA[i].y*_sin,VA[i].x*_sin+VA[i].y*_cos,0);
        V.z=GetFHeight(V.x,V.y);		
		VP[i]=V;

	}
	Plane P(VP[0],VP[1],VP[2]);

	Matrix4D M;
	M.rotation(Vector3D::oZ,ang);
	Vector3D VZ(P.a,P.b,P.c);
	Vector3D VX=VP[2]-(VP[0]+VP[1])*0.5f;
	VX.normalize();
	Vector3D VY=VP[1]-VP[0];
	VY.normalize();
	M.fromBasis(VX,VY,VZ,Vector3D::null);	
	return M;
}
//--------------------------------------------------------------------------
//	Func:	NewAnimation::DrawAt	
//	Desc:	Draws animated object instance at given position and given
//			animation time
//--------------------------------------------------------------------------
void NewAnimation::DrawAt(	int	frame,					//  animation frame
							byte NI,					//  nation index
							float x, float y, float z,	//  wolrd-space position
							float Dir,					//  direction
							float Scale,				//  scale
							DWORD Color,				//  color multiplier
							float fiDir,float fiOrt,	//  orientation
							OneObject* OB				//  object instance
						  )
{	
    //  skip weapons in shadow rendering pass
    if (Code == 0xBAADF00D && g_bRenderShadowsPass) return;

	LastDrawnNA     = this;
	LastFrame       = frame>>8;
	if (frame < 0) frame = 0;

	if (!IsVisible( OB, MyNation )) return;
	//if(LockGrid)DrawCollision(OB);

    if(StartDx || StartDy){
		x+=StartDx*Scale*NewAnimation::Scale;
		y+=StartDy*Scale*NewAnimation::Scale;
	}

    Vector3D pos( x, y, z );

	//  play sound effect
	PlayAnimationEx( OB, this, frame, x, y );		

	//  slightly change unit color
	CurDiffuse=Color;
	VariateUnitColor( OB, Color );
	
	

	GPS.SetCurrentDiffuse(Color);
	Dir+=AddDirection;
	z+=AddHeight;
    NewMonster* NM=OB?OB->newMons:NULL;

	extern bool g_bPerspCamera;

	if(OB && GSets.ShadowQuality>=2 && OB->Sdoxlo<40 && !OB->newMons->NewParams.DeadBody){
        int R,H,UH;
        OB->GetCylinder(R,H,UH);
        Vector3D pos(OB->GetAttX()>>4,OB->GetAttY()>>4,UH+3);
        float A=float(OB->RealDir)*c_PI/256.0f;
        Vector3D dir(cos(A),sin(A),0);        
        IShadowMgr->AddSimpleCaster(pos,dir
            ,float(R*2)*NM->NewParams.ShadowScaleX
            ,float(H)*NM->NewParams.ShadowScaleY
            ,NM->NewParams.ShadowSet,!NM->NewParams.ShadowHAligning,OB->Sdoxlo ? ((255*32/(32+OB->Sdoxlo))<<24) : 0xFF000000 );
    }

    if(NM && _dbgDrawShotPts && NM->ShotPtZ){
        for(int i=0;i<NM->NShotPt;i++){
            DrawSphere(Sphere(Vector3D(OB->RealX/16+NM->ShotPtX[i],OB->RealY/16+NM->ShotPtY[i],OB->RZ+NM->ShotPtZ[i]),5),0xFFFF0000,0xFFFF0000);
        }
    }

	if (AnimationType == at3D)
	{		
        if(!GSets.SVOpt.Draw3DUnits)return;		
        float LoacalBlendTime=BlendingTime;
        if(NM){
            LoacalBlendTime=NM->NewParams.BlendingTime;
             if(NM->NoBlending)LoacalBlendTime=0;
        }
        Vector3D wPos = SkewPt( x, y, z );
        Vector3D sPos( wPos );
        WorldToScreenSpace( sPos );

        sPos.x = roundf( sPos.x );
        sPos.y = roundf( sPos.y );
        DWORD nationalColor = GetNatColor( NI );

        /*
        static int LightID=IMM->GetNodeID( "GameLight" );
        ILight* IL=IMM->GetLight(LightID);         
        DWORD Diff0=0;
        if (Color!=0xFF7F7F7F && IL){
            Diff0=IL->GetDiffuse();
            IL->SetDiffuse(ModDWORD2X(Diff0,Color));
            IL->Render();
        }
        */

		int Anim=-1;
		int Model=-1;
		float ATime=0;
		float exscale=1.0f;
		if(AnimSet3D.GetAmount()){			
            int cf=frame>>8;
			int df=0;
			int as_nf=AnimSet3D.GetAmount();
			int nf=0;
			int i;
			for(i=0;i<as_nf&&cf>=(nf=AnimSet3D[i]->NFrames);i++){				
				cf-=nf;
				df+=nf;
			}
			if(i>=as_nf)i=as_nf-1;
			AnimFrame3D* AF=AnimSet3D[i];
			Anim=AF->Animation;
			Model=AF->Model;
			int frame1=frame-(df<<8);
			if(AF->NFrames)ATime=AF->StartAnmTime+frame1*(AF->FinalAnmTime-AF->StartAnmTime)/AF->NFrames/256.0f;
			else ATime=0;
			if(ATime>AF->FinalAnmTime&&AF->FinalAnmTime>AF->StartAnmTime)ATime=AF->FinalAnmTime;
			exscale=AF->Scale;
			Dir+=AF->AddDir;
		}else{
			Anim=AnimationID;
			Model=ModelID;
			ATime=NFrames?float(frame)/NFrames/256.0f:0;
		}
		if(OB&&OB->LockType==1){
			fiOrt+=0.02f*cos(float(GetTickCount())/400.0f);
			fiDir+=0.01f*cos(float(GetTickCount())/815.0f);
		}
		Matrix4D M4;
		bool doskew=OB && OB->LockType==0 && !OB->NewBuilding;		
		//  unit facing direction
		float ang = Dir*3.1415f/128.0f;

		//  build object orientation matrix
		if(NM && NM->NewParams.PickGroundDY){
			M4=GetUnitOrientation(pos,ang,NM->NewParams.PickGroundDX,NM->NewParams.PickGroundDY);
		}else{
			float cosa = cosf( ang );
			float sina = sinf( ang );
			Vector3D vz = doskew?GetTotalNormal( x, y ):Vector3D(0,0,1);
			Vector3D vx(  cosa, sina, 0.0f );
			Vector3D vy( -sina, cosa, 0.0f );

			//  for ground objects skew them to align with ground normal
			if (GetTotalHeight( x, y ) > 0.0f && Code != 0xBAADF00D)
			{
				vy.cross( vz, vx );
				vy.normalize();
				vx.cross( vy, vz );
			}
			vz = Vector3D::oZ;			
			M4.e00 = vx.x; M4.e01 = vx.y; M4.e02 = vx.z; M4.e03 = 0.0f;
			M4.e10 = vy.x; M4.e11 = vy.y; M4.e12 = vy.z; M4.e13 = 0.0f;
			M4.e20 = vz.x; M4.e21 = vz.y; M4.e22 = vz.z; M4.e23 = 0.0f;
			M4.e30 = 0.0f; M4.e31 = 0.0f; M4.e32 = 0.0f; M4.e33 = 1.0f;		
		}

        if (fabs( fiOrt ) > 0.0001f)
        {
            Matrix4D m;
            m.rotation( Vector3D::oX, fiOrt*(1-Hardness) );
            M4.mulLeft( m );   
        }
        if (fabs( fiDir ) > 0.0001f) 
        {
            Matrix4D m;
            m.rotation( Vector3D::oY, fiDir*(1-Hardness) );
            M4.mulLeft( m );
        }

		Matrix4D M41;	
        //  sink dead objects into the ground
        if (OB&&OB->Sdoxlo>1400){
            float sd=float(OB->Sdoxlo*256.0+OB->SdoxloLow-1400*256)/256.0;
            z-=sd*sd/12.0f/12.0f;
        }
        //  scale-transform
		M41.st(NewAnimation::Scale*Scale*exscale, Vector3D(x,y,z));
		M4 *= M41;
		//M4 *= GetSkewTM();
		
        DWORD anmID     = 0;
        float anmTime   = 0.0f;
		DWORD anmID2    = 0;
        float anmTime2  = 0.0f;
		DWORD anmID3    = 0;
		float anmTime3  = 0.0f;
		//  self-playing animation
		if(TimeAnimationID>0&&TimeAnimationFrames>0)
		{
			float atime=IMM->GetAnimTime(TimeAnimationID);
			if(atime>0.0001){
				extern int TrueTime;
				int frame=TrueTime+
					int(float(TimeAnimationVariation)*sin(float(TrueTime)/2000))+
					int(float(TimeAnimationVariation)*sin(float(TrueTime)/1654))+
					int(float(TimeAnimationVariation)*sin(float(TrueTime)/1113));
				anmTime3 = atime*float(frame%TimeAnimationFrames)/TimeAnimationFrames;
                anmID3   = TimeAnimationID;				
			}
		}
		
		if(OB){
            ColorValue nc( nationalColor );
            IRS->SetColorConst( nationalColor );	
            IEffMgr->SetCurrentNatColor( nationalColor );
            if(NM->TFactor){
                DWORD C=ModDWORD2X(NM->TFactor,Color);
				C=(C&0x00FFFFFF)|(Color&0xFF000000);
    			IRS->SetTextureFactor( C );            
            }else IRS->SetTextureFactor( Color );
        }
        //  setup model animation
		if(Anim==-1)
		//  no animation
		{
			if (TimeAnimationID < 1)
            {
                anmTime = 0.0f;
                anmID   = 0;
            }
		}else
		{
			if(SecondAnimationID>0)
			//  double-animation
			{
				float AT=IMM->GetAnimTime(AnimationID);
				if(NFrames){		
					int NF=NFrames*256;
					int P=0;
					if(OB){
						P=OB->Phase;
					}
					float T1=(int(NF*100+frame+(P*DirFactor))%NF)*AT/NF;
					float T2=(int(NF*100+frame-(P*DirFactor))%NF)*AT/NF;					
					anmTime2= T1;
                    anmID2  = Anim;
                    anmTime = T2;
                    anmID   = SecondAnimationID;					
				}
                else 
                {
                    anmTime = 0.0f;
                    anmID   = Anim;
                }
			}
			else
			//  normal 3d animation
			{
				float AT = IMM->GetAnimTime(Anim);
                anmTime = NFrames ? ATime*AT : 0.0f;
                anmID   = Anim;                
			}
		} 

		DWORD context = (DWORD)this;
        if (OB){
            context = OB->Serial;
            IMM->SetAttachedEffectMask(OB->AttachedEffectMask);
        }else{
            IMM->SetAttachedEffectMask(1);
        }

        if(Model<=0)return;
        PushEntityContext( context );
		DWORD CTX=CUR_CONTEXT;
		if(!g_bPerspCamera){
            if(ModelID_iso>0&&Scale==1.0f)IMM->StartModel( ModelID_iso, M4, CTX);//CUR_CONTEXT );
			else IMM->StartModel( Model, M4, CTX);
		}else IMM->StartModel( Model, M4, CTX);
        //  apply animation
		if(anmID )IMM->AnimateModel( anmID , anmTime,  anmID3||anmID2?0:LoacalBlendTime ); 
		if(anmID2)IMM->AnimateModel( anmID2, anmTime2, 0 ); 
		if(anmID3)IMM->AnimateModel( anmID3, anmTime3, 0 ); 
		if(OB && OB->ActiveAbility){
			int a=anmID;
			OB->ActiveAbility->ActiveAbilities.modifyExtraAnimation(a,a);
		}

        //  render water reflection
		if (ReflectionID == 0) ReflectionID = Model; 
		if(OB && GSets.SVOpt.DrawReflectedModels && !g_bRenderShadowsPass){	 		
			Vector3D vd=ICam->GetDirection();
			if(GetHeight((OB->RealX>>4)-vd.x*256,(OB->RealY>>4)-vd.y*256)<0)
				IRMap->AddObject( ReflectionID, &M4 );
		}
        
        //  render model
		IRS->SetShaderConst( 0,500000 );
		bool NeedBack=false;
		bool NoShadow=false;

		if(OB && OB->NewBuilding){
			IRS->SetShaderConst( 1,float(OB->Stage)/float(OB->NStages+1));
			if(OB->Stage < OB->NStages){
				AABoundBox BB=IMM->GetBoundBox(Model);
				IRS->SetShaderConst( 0,BB.maxv.z*OB->Stage/OB->NStages );
				NeedBack=true;
				NoShadow=true;
			}
		}

		if(IsEffectTargeted)IEffMgr->SetTarget(GlobalEffectTarget);		
		if(g_bRenderShadowsPass==false && NM && NM->ActiveBones.GetAmount()){
			for(int i=0;i<NM->ActiveBones.GetAmount();i++){
				CurActiveBones CAB;
				int NodeID=NM->ActiveBones[i]->GetNode();
				CAB.M4=IMM->GetNodeTransform(NodeID);
				CAB.M4*=M4;
				CAB.OBI=NM->ActiveBones[i];
				CAB.UIndex=OB->Index;
				ActiveBonesList.Add(CAB);
			}
		}

        IMM->DrawModel();
		if(OB && OB->ActiveAbility){
			bool x=true;
			OB->ActiveAbility->ActiveAbilities.modifyModelAnimation(x,x);
		}

		if(NeedBack){
			IRS->SetShaderConst( 0,100000 );
		}
        
        //  register on-click
        if(OB&&!OB->Sdoxlo)RegisterVisibleGP( OB, Model, M4 );
		
		// on-click for carcass building
		int gp,fr;
		Vector3D C;
		/*if (IMM->GetModelGP( ModelID, gp, fr, C ))
		{
			if(OB&&!OB->Sdoxlo)RegisterVisibleGP( OB, gp, fr, sPos.x - C.x,sPos.y - C.y );
		}*/		

		//  particle emitter
		for(int i=0;i<Effects.GetAmount();i++){
			DWORD A=Effects[i]->GetAlpha(OB);			
			if(A){
				float f=Effects[i]->GetIntensity(OB);
				if(f>0.0f){
					IEffMgr->EnableAutoUpdate(true,6,3);
					IEffMgr->SetIntensity(Effects[i]->EffectFileID,Effects[i]->EffectNameID,f);
					IEffMgr->SetAlphaFactor(Effects[i]->EffectFileID,Effects[i]->EffectNameID,float(A)/255.0f);
					IEffMgr->UpdateInstance(Effects[i]->EffectFileID,Effects[i]->EffectNameID,M4);
					IEffMgr->EnableAutoUpdate(false);
					if(Effects[i]->SoundID>0){
						extern CDirSound* CDS;
						CDS->HitSound(Effects[i]->SoundID);
						AddEffect(x,y,Effects[i]->SoundID);
					}
				}
			}
		}

		bool inv=false;
		if(OB&&OB->ActiveAbility){
			byte mask=0;
			bool EnableSearchVictim;
			OB->ActiveAbility->ActiveAbilities.modifyInvisibility(false,inv,mask,EnableSearchVictim); // не рисовать тень для невидимок
		}
        if (!(inv||NoShadow))
        {            
            if(GSets.ShadowQuality<2){
				if(OB){
					if(!((OB->Sdoxlo>700 || OB->LockType==1))){
						IShadowMgr->AddCaster( Model, M4 );
						if(OB){
							int CR,CH,UH;
							OB->GetCylinder(CR,CH,UH);
							Vector3D P(M4.e30,M4.e31,M4.e32);
							P.z+=CH+(CH>>2);
							IShadowMgr->AddCastingBoundaryPoint(P);
						}
						IMM->RenderShadow( Model, &M4 );
					}
				}else{
					Vector3D P(M4.e30,M4.e31,M4.e32);
					P.z+=200;
					IShadowMgr->AddCastingBoundaryPoint(P);
				}
            }
        }

        PopEntityContext();
		IRS->SetShaderConst(1,1);
		IRS->SetShaderConst( 0,500000 );
        /*
        if (Diff0 && IL){            
            IL->SetDiffuse(Diff0);
            IL->Render();
        }
        */
	}
	else if (AnimationType == atComplexShadow2D ||AnimationType == at2D)
	{		
        DrawShadowedSpriteUnit( OB, pos, frame, Dir, NI, Scale);		
        //DrawSpriteUnit( OB, pos, frame, Dir, NI , Scale);
	}
	//else if(AnimationType == atComplexShadow2D || AnimationType == at2D)
    //{
     //   if(!GSets.SVOpt.Draw2DUnits)return;
	//	if(!g_bPerspCamera){
//			Scale*=5.0f/4.0f;
//		}
//		if(Rotations==9)DrawSpriteUnit( OB, pos, frame, Dir, NI, Scale*NewAnimation::Scale);
  //      else DrawShadowedSpriteUnit( OB, pos, frame, Dir, NI, Scale*NewAnimation::Scale);
	//}
	
	//  draw health for units
	extern byte PlayGameMode;
	extern word DrawHealthID;
	extern int DrawHealthCount;

	extern word Att;
	extern word Friend;
	
	if( OB && v_ISys.AltDrawHealth && ( OB->Index==DrawHealthID || v_ISys.AltDrawHealth&&(GetKeyState(VK_MENU)&0x8000) || OB->Index==Att || OB->Index==Friend ) && PlayGameMode==0 ){
        if(GSets.CGame.ViewMask==255){
    		DrawHealthCount--;
		    if(DrawHealthCount<0){
			    DrawHealthID=0xFFFF;
		    }
		    void DrawHealth(OneObject* OB);
		    DrawHealth(OB);
			ext_OnDrawOnMapOverAll();
        }
	}

	//dust&water
	extern int LastFlipTime;
	if (OB && OB->LockType != 1)
    {
		if(OB->GroupSpeed>20&&OB->RZ>15&&OB->DestX>0&&EngSettings.RunDustList.GetAmount())
        {
			int tofs=(OB->RealX>>9)+(OB->RealY>>9)*VertInLine;
			if(tofs>0&&tofs<MaxPointIndex){
				int T=TexMap[tofs];
				extern DWORD TXCOLOR[64];
				DWORD TC=TXCOLOR[T];

				int TB=TC&255;
				int TG=(TC>>8)&255;
				int TR=(TC>>16)&255;
				int TA=(TR+TG+TG+TB)/4;
				if(TG*8<TA*9&&TA>80&&TB<TA&&TA<180){
					int p=rand()%EngSettings.RunDustList.GetAmount();
					OneRandomEffectInfo* EF=EngSettings.RunDustList[p];
					static int LastEfTime=GetTickCount()-10000;
					int dt=(LastFlipTime-LastEfTime)*EF->MaxBirthPerSecond;			
					if(EF&&EF->Probability*dt*4>rand()){				
						PushEntityContext(OB->RealX+OB->RealY*37);
						IEffMgr->EnableAutoUpdate();
						/*int mid=EF->ModelID;
						Matrix4D M;
						M.scaling(EF->Scale);
						M.setTranslation(SkewPt(OB->RealX/16,OB->RealY/16,OB->RZ));
						IMM->Render(mid,&M);
						IMM->StartModel(mid, M, 0);
						IMM->DrawModel();*/
						PopEntityContext();
						IEffMgr->EnableAutoUpdate( false );
						LastEfTime=LastFlipTime;
					}            
				}
			}
		}
		if(OB->RZ<=2&&EngSettings.WaterBlobsList.GetAmount())
		{
            int p=rand()%EngSettings.WaterBlobsList.GetAmount();
			OneRandomEffectInfo* EF=EngSettings.WaterBlobsList[p];
			static int LastEfTime=GetTickCount()-10000;
			int dt=(LastFlipTime-LastEfTime)*EF->MaxBirthPerSecond;			
			if(EF&&EF->Probability*dt*4>rand()){
                PushEntityContext(OB->RealX+OB->RealY*34);
				IEffMgr->EnableAutoUpdate();
				/*int mid=EF->ModelID;
				Matrix4D M;
				M.scaling(EF->Scale);
				M.setTranslation(SkewPt(OB->RealX/16,OB->RealY/16,OB->RZ));
				IMM->Render(mid,&M);
				IMM->StartModel(mid, M, 0);
				IMM->DrawModel();*/
                IEffMgr->EnableAutoUpdate( false );
				PopEntityContext();
				LastEfTime=LastFlipTime;
			}
		}
	}

	//  additional animation (flags on buildings, for example)
	for(int i=0;i<AnmExt.GetAmount();i++)
	{
		byte D=byte(Dir);
		float scl=Scale*NewAnimation::Scale;
		int dx=int((AnmExt[i]->dx*TCos[D]-AnmExt[i]->dy*TSin[D])*scl)>>8;
		int dy=int((AnmExt[i]->dy*TCos[D]+AnmExt[i]->dx*TSin[D])*scl)>>8;		
		NewAnimation* NA=AnmExt[i]->NA;
		if(NA && NA->NFrames)
		{
			PushEntityContext(i*777+333+NA->Code);
			int p = AnmExt[i]->Period;
            if (p == 0){
                NA->DrawAt(	frame,
                    NI,
                    x + dx, y + dy, z + AnmExt[i]->dz*scl + AddHeight,
                    Dir + AnmExt[i]->dDir,
                    scl*AnmExt[i]->Scale,
                    Color, fiDir, fiOrt, OB );
            }else{
			    NA->DrawAt(	(NA->NFrames*256)*(GetTickCount()%p)/p,
						    NI,
						    x + dx, y + dy, z + AnmExt[i]->dz*scl + AddHeight,
						    Dir + AnmExt[i]->dDir,
						    scl*AnmExt[i]->Scale,
						    Color, fiDir, fiOrt, OB );
            }
			PopEntityContext();
		}		
	}
} // NewAnimation::DrawAt

bool OneBoundEffect::Parse(char* s,_str& ErrLog){
	char s1[64];
	char s2[64];
	int z=sscanf(s,"%s%s",s1,s2);
	if(z!=2)return false;
	EffectFile=s1;
	EffectName=s2;
	EffectFileID=IEffMgr->GetEffectSetID(s1);
	if(EffectFileID==-1){
		ErrLog.print("Unable to load effect: %s",s1);
		return false;
	}
	EffectNameID=IEffMgr->FindEffectByName(EffectFileID,s2);
	if(EffectNameID==-1){
		ErrLog.print("Unable to find effect <%s> in file <%s>",s2,s1);
		return false;
	}
	return true;
}
DWORD LifeDependentEffect::GetAlpha(OneObject* OB){
	if(OB&&LifePercent){
		if(OB->MaxLife){
            int A=255-OB->Life*255*100/OB->MaxLife/LifePercent;
			if(A<0)return 0;
			A*=2;
			if(A>255)A=255;
            return A;			
		}
	}
	return 0;
}
bool LifeDependentEffect::Parse(char* s,_str& ErrLog){
	char s1[64];
	char s2[64];	
	char s4[64];
	char ss[32]="";
	int lp;
	int z=sscanf(s,"%s%d%s%s%s",s4,&lp,s1,s2,ss);
	if(z<4)return false;
	if(strcmp(s4,"LIFE"))return false;
	LifePercent=lp;
	EffectFile=s1;
	EffectName=s2;
	EffectFileID=IEffMgr->GetEffectSetID(s1);
	if(EffectFileID==-1){
		ErrLog.print("Unable to load effect: %s",s1);
		return false;
	}
	EffectNameID=IEffMgr->FindEffectByName(EffectFileID,s2);
	SoundID=GetSound(ss);
	if(EffectNameID==-1){
		ErrLog.print("Unable to find effect <%s> in file <%s>",s2,s1);
		return false;
	}
	return true;
}
bool MoveDependentEffect::Parse(char* s,_str& ErrLog){
	char s1[64];
	char s2[64];	
	char s4[64];
	char ss[32]="";
	int lp;
	int z=sscanf(s,"%s%s%s%s",s4,s1,s2,ss);
	if(z<3)return false;
	if(strcmp(s4,"MOVE"))return false;
	EffectFile=s1;
	EffectName=s2;
	EffectFileID=IEffMgr->GetEffectSetID(s1);
	if(EffectFileID==-1){
		ErrLog.print("Unable to load effect: %s",s1);
		return false;
	}
	EffectNameID=IEffMgr->FindEffectByName(EffectFileID,s2);
	int GetSound(char* Name);
	SoundID=GetSound(ss);
	if(EffectNameID==-1){
		ErrLog.print("Unable to find effect <%s> in file <%s>",s2,s1);
		return false;
	}
	return true;
}
DWORD MoveDependentEffect::GetAlpha(OneObject* OBJ){
	return 0xFF;
}	
float MoveDependentEffect::GetIntensity(OneObject* OB){
	if(OB){
		if(OB->DestX>0){			
			return 1.0f;
		}
	}
	return 0;
}
