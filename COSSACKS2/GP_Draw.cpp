#define _WINSOCKAPI_
#include "stdheader.h"
#include <malloc.h>
#include "ddini.h"
#include "ResFile.h"
#include "mode.h" 
#include "fastdraw.h"
#include "Lines.h"
#include <stdio.h>
#include "smart_assert.h"
#include <CrtDbg.h>
#include "CTables.h" 
#include "mapdiscr.h"
#include "GP_Draw.h"
#include "RealWater.h"
#include <math.h>
#include "ActiveScenary.h"
#include "TopoGraf.h"
#include "GSINC.h" 
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "GameSettings.h"
#include "float.h"
#include "kStaticArray.hpp"
#include "rsRenderSystem.h"
#include <vector>

#define FASTLOAD
bool CheckIfFileExists(char* Name); 
bool NewGPImage;
int ddd=324;
void __stdcall CDGINIT_INIT3(){
	ddd=0;
};

#define znew(t,s) (t*)malloc((s)*sizeof(t))

//  global instance of sprite manager bridge
extern GP_System GPS; 

/*****************************************************************************/
/*	SQPicture implementation
/*****************************************************************************/
SQPicture::SQPicture() : m_ID(-1)
{
}

SQPicture::SQPicture( char* name )
{
	m_ID = IPM->GetImageID( name );
}

SQPicture::~SQPicture()
{
	IPM->UnloadImage( m_ID );
}	

void SQPicture::LoadPicture( char* name, bool bUnloadOld )
{
	if (bUnloadOld) IPM->UnloadImage( m_ID );
	m_ID = IPM->GetImageID( name );
	IPM->LoadImage( m_ID );
}

void SQPicture::Draw( int x, int y, int w, int h )
{
    Vector3D pos( x, y, 0.0f );
    const Matrix4D* pTM = GPS.GetScreenSpaceTM();
    IPM->SetDiffuse( GPS.GetCurrentDiffuse() );
    if (pTM)
    {
        Matrix4D tr = *pTM;
        IPM->EnableTransform( true );
        pTM->transformPt( pos );
        tr.setTranslation( pos );
        IPM->SetTransform( tr );
        IPM->DrawImage( m_ID, 0.0f, 0.0f, 0.0f );
        return;
    }

    IPM->EnableTransform( false );
    IPM->DrawImage( m_ID, Rct( pos.x, pos.y, w, h ), pos.z );
} // SQPicture::Draw

void SQPicture::Draw( int x, int y )
{
	Vector3D pos( x, y, 0.0f );
	const Matrix4D* pTM = GPS.GetScreenSpaceTM();
	IPM->SetDiffuse( GPS.GetCurrentDiffuse() );
	if (pTM)
	{
		Matrix4D tr = *pTM;
		IPM->EnableTransform( true );
		pTM->transformPt( pos );
		tr.setTranslation( pos );
		IPM->SetTransform( tr );
		IPM->DrawImage( m_ID, 0.0f, 0.0f, 0.0f );
		return;
	}
	
	IPM->EnableTransform( false );
	IPM->DrawImage( m_ID, pos.x, pos.y, pos.z );
} // SQPicture::Draw

void SQPicture::Xlat( byte* tbl )
{

}

void SQPicture::SetLx( int lX )
{
}

void SQPicture::SetLy( int lY )
{
}

BYTE* SQPicture::GetPixelData()
{ 
	return NULL; 
}

/*****************************************************************************/
/*	GP_System implementation
/*****************************************************************************/
GP_System::GP_System() 
{
	UNITBL = (UNICODETABLE**) malloc( 4 * 4096 );
	memset( UNITBL,	0, 4096 * 4 );
	SetDrawMode( dmSprites );
	m_bUseScreenSpaceTM = false;
	m_TM.setIdentity();
}

BYTE* LoadFile( const char* path, DWORD& bufLen );
void GP_System::Init()
{
    DWORD len = 0;
    char* buf = (char*)LoadFile( "VideoConfig.txt", len );
    if (buf)
    {
        char* pBuf = strstr( buf, "ManagedSpriteCache" );
        if(pBuf) pBuf = strstr( pBuf, "=" );
        if (pBuf)
        {
            pBuf++;
            char cbuf[1024];
            sscanf( pBuf, "%s", &cbuf );
            if (!strnicmp( cbuf, "true", 4 ))
            {
                ISM->SetManagedCache( true );
            }
            else
            {
                ISM->SetManagedCache( false );
            }
        }
    }
	ISM->Init();
} // GP_System::Init

GP_System::~GP_System()
{
}

DWORD g_NationalColors[] = 
{
	0xFFA50000,
	0xFF003CC6,
	0xFF29B694,
	0xFF9C49B5,
	0xFFF78610,
	0xFF292839,
	0xFFE7E3E7,
	0xFF6B4110
};

CEXPORT DWORD GetNatColor( int natIdx )
{
	if (natIdx < 0 || natIdx >= 8) return 0xFFFFFFFF;
	return g_NationalColors[natIdx];
} // GetNatColor
DWORD MulDWORD(DWORD C1,int Mul);
CEXPORT DWORD GetNatColor( int natIdx, int Multiplicator )
{
	if (natIdx < 0 || natIdx >= 8) return 0xFFFFFFFF;
	return MulDWORD(g_NationalColors[natIdx],Multiplicator);
} // GetNatColor

void GP_System::ShowGP( int x, int y, float z, int gpID, int sprID, byte Nation )
{
	if (m_bUseScreenSpaceTM)
	{
		Matrix4D tr( m_TM );
		if (sprID >= 4095) 
		{
			tr.getV0().reverse();
			tr.getV2().reverse();
		}
		Vector3D pos(x,y,z);
		tr.transformPt( pos );
		tr.setTranslation( pos );
		SAFENEW;
			ISM->DrawSprite( gpID, sprID & 4095, tr, g_NationalColors[Nation] );
		ENDNEW;
		return;
	}
	SAFENEW;
		ISM->DrawSprite( gpID, sprID & 4095, Vector3D( x, y, z ), sprID >= 4095, g_NationalColors[Nation] );
	ENDNEW;
}

void GP_System::EnableZBuffer( bool bEnable )
{ 
    ISM->EnableZBuffer( bEnable ); rsEnableZ( bEnable ); 
}

void GP_System::ShowGP(	int x, int y, int gpID, int sprID, byte Nation )
{
	ShowGP( x, y, 0.0f, gpID, sprID, Nation );
} // GP_System::ShowGP

void GP_System::ShowGPTransformed(  int gpID, int sprID, 
								  const Matrix4D& transform, 
								  byte Nation )
{
	Matrix4D tr( transform );
	if (sprID >= 4095) 
	{
		tr.getV0().reverse();
		tr.getV2().reverse();
	}
	SAFENEW;
		ISM->DrawSprite( gpID, sprID & 4095, tr, g_NationalColors[Nation] );
	ENDNEW;
} // GP_System::ShowGPTransformed

void ConvertG17_Name( char* Name );

bool GP_System::LoadGP( int gpID )
{
	SAFENEW;
		ISM->LoadPackage( gpID );
	ENDNEW;
	return false;
}

const char*	GP_System::GetGPPath( int seqID )
{
	static char convertedName[_MAX_PATH];
	strcpy( convertedName, GetGPName( seqID ) );
	ConvertG17_Name( convertedName );
	return convertedName;
} // GP_System::GetGPPath

void GP_System::FlushBatches()
{ 
	/*
	if((REALTIME>>4)==0){
		static int Time=0;
		static int Count=0;
		if(Time!=REALTIME){
			Log.Warning("Time: %d, Count: %d",Time,Count);
			Time=REALTIME;
			Count=0;
		}else{
			Count++;
		}
	}
	*/
    //ISM->Flush(); 
    rsFlush(); 
}

int GP_System::GetGPWidth( int seqID, int sprID )
{
	int v;
	SAFENEW;
		v = ISM->GetFrameWidth( seqID, sprID );
	ENDNEW;
	return v;
}

int	GP_System::PreLoadGPImage( char* gpName )
{
	if (!ISM) return -1;
	int id;	
	SAFENEW;		
		id = ISM->GetPackageID( gpName ); 	
		char cPath[_MAX_PATH];
		strcpy( cPath, gpName );
		ConvertG17_Name( cPath );
		ISM->SetPackagePath( id, cPath );
		UNITBL[id] = UFONTS.FindFont( gpName );
	ENDNEW;
	return id; 
}// GP_System::PreLoadGPImage

void GP_System::FlushLines()
{
    IRS->ResetWorldTM();
	rsFlushLines2D();
	rsFlushPoly2D();
	rsFlushLines3D();
	rsFlushPoly3D();
} // GP_System::FlushLines

void GP_System::SetDrawMode( GPDrawMode _mode )
{
}

void GP_System::DrawWSprite( int gpID, int sprID, const Matrix4D& transf, DWORD color )
{
	SAFENEW;
		ISM->DrawWSprite( gpID, sprID, transf, color );
	ENDNEW;
}

void GP_System::DrawLine( float x1, float y1, float x2, float y2, DWORD color )
{
	if (m_bUseScreenSpaceTM)
	{
		Vector3D a( x1, y1, 1 );
		Vector3D b( x2, y2, 1 );
		m_TM.transformPt( a );
		m_TM.transformPt( b );
		rsLine( a.x, a.y, b.x, b.y, 0, color );
		return;
	}
	rsLine( x1, y1, x2, y2, 0, color );
}

void GP_System::DrawLine( const Vector3D& beg, const Vector3D& end, DWORD color )
{
	rsLine( beg, end, color );
}

void GP_System::DrawFillRect( float x, float y, float w, float h, DWORD color )
{
	rsRect( Rct( x, y, w, h ), 0.0f, color );
}

void GP_System::DrawFillRect( float x, float y, float w, float h, DWORD clt, DWORD crt, DWORD clb, DWORD crb )
{
	rsRect( Rct( x, y, w, h ), 0.0f, clt, crt, clb, crb );
}

void GP_System::DrawRect( float x, float y, float w, float h, DWORD color )
{
	rsLine( x,		y,			x + w,	y,		0, color );
	rsLine( x + w,	y,			x + w,	y + h,	0, color );
	rsLine( x + w,	y + h,		x,		y + h,	0, color );
	rsLine( x,		y + h,		x,		y,		0, color );
}

void GP_System::DrawFillRect(	const Vector3D& lt, const Vector3D& rt, 
								const Vector3D& lb, const Vector3D& rb, DWORD color )
{
	rsQuad( lt, rt, lb, rb, color );
}

void GP_System::DrawFillRect(	const Vector3D& lt, const Vector3D& rt, 
								const Vector3D& lb, const Vector3D& rb, 
								DWORD clt, DWORD crt, DWORD clb, DWORD crb )
{
	rsQuad( lt, rt, lb, rb, clt, crt, clb, crb );
}

CEXPORT void g_SetWindParam( int x, int y, int x1, int y1 )
{
	GPS.SetClipArea( x, y, x1 - x + 1, y1 - y + 1 );
}

CEXPORT void g_GetWindParam( int& x, int& y, int& x1, int& y1 )
{
	const Rct& vp = GPS.GetClipArea();
	x = vp.x; y = vp.y; x1 = vp.GetRight(); y1 = vp.GetBottom();
}	

extern int mapx;
extern int mapy;

extern byte WaterCost[65536];
//Waves
void ShowGradPicture(int x,int y,int Lx,int Ly,
					 int z1,int z2,int z3,int z4,
					 byte* Bitmap,int BMLx){	
};

#include "DrawForm.h"
#include "MapSprites.h"


LocalGP::LocalGP(){
	GPID=-1;
	Uniq=false;
};
CEXPORT
LocalGP::LocalGP(char* Name){
	GPID=-1;
	Load(Name);
};
CEXPORT
LocalGP::~LocalGP(){
	UnLoad();
};
void LocalGP::Load(char*Name){
	if(GPID!=-1)UnLoad();
	SAFENEW;
		GPID=GPS.PreLoadGPImage(Name);
		if(GPID>=0)GPS.LoadGP(GPID);
	ENDNEW;
	Uniq=NewGPImage;
};
void LocalGP::UnLoad(){
	if(GPID!=-1){
		SAFENEW;
		if(Uniq)
			GPS.UnLoadGP(GPID);
		GPID=-1;
		ENDNEW;
		Uniq=0;
	};
};
extern UNIFONTS UFONTS;
UNIFONTS::UNIFONTS(){
	NFonts=0;
	UFONTS=NULL;
};
UNIFONTS::~UNIFONTS(){
	if(UFONTS)delete[](UFONTS);
	NFonts=0;
	UFONTS=NULL;
};
void FONERR(){
	MessageBox(NULL,"Invalid Unicode.dat","ERROR!",0);
	assert(0);
};

int UNI_LINEDLY1=0;
int UNI_LINEDY1=0;
int UNI_LINEDLY2=0;
int UNI_LINEDY2=0;
int UNI_HINTDY1=0;
int UNI_HINTDLY1=0;
int UNI_HINTDY2=0;
int UNI_HINTDLY2=0;

void UNIFONTS::LoadFonts(){
	GFILE* F=Gopen("Unicode.dat","r");
	if(!F)return;
	int N;
	int z=Gscanf(F,"%d",&N);
	if(z=1){
		for(int i=0;i<N;i++){
			char ccc[128];
			int z=Gscanf(F,"%s",ccc);
			SAFENEW;
			GPS.PreLoadGPImage(ccc);
			ENDNEW;
		};
	};
	z=Gscanf(F,"%d",&N);
	if(z==1){
		NFonts=0;
		UFONTS=new OneUniFont[N];
		for(int i=0;i<N;i++){
			int q;
			int z=Gscanf(F,"%s%d",UFONTS[i].FontName,&q);
			if(z!=2)FONERR();
			if(q>4)FONERR();
			UFONTS[i].UTBL.NTables=q;
			for(int p=0;p<q;p++){
				char FNM[64];
				int uc=0;
				int z=Gscanf(F,"%d%d%d%d%d%d%s",&UFONTS[i].UTBL.USET[p].DX,
										&UFONTS[i].UTBL.USET[p].DY,
										&UFONTS[i].UTBL.USET[p].Start,
										&UFONTS[i].UTBL.USET[p].NSymbols,
										&UFONTS[i].UTBL.USET[p].GP_Start,&uc,
										FNM);
				UFONTS[i].UTBL.USET[p].UseColor=uc;
				if(z!=7)FONERR();
				SAFENEW;
				UFONTS[i].UTBL.USET[p].GPID=GPS.PreLoadGPImage(FNM);
				ENDNEW;
			};
		};
		NFonts=N;
		z=Gscanf(F,"%d",&N);
		int v;
		char ccc[64];
		for(i=0;i<N;i++){
			z=Gscanf(F,"%s%d",ccc,&v);
			if(z==2){
				if(!strcmp(ccc,"UNI_LINEDLY1")){
					UNI_LINEDLY1=v;
				}else
				if(!strcmp(ccc,"UNI_LINEDLY2")){
					UNI_LINEDLY2=v;
				}else
				if(!strcmp(ccc,"UNI_LINEDY1")){
					UNI_LINEDY1=v;
				}else
				if(!strcmp(ccc,"UNI_LINEDY2")){
					UNI_LINEDY2=v;
				}else
				if(!strcmp(ccc,"UNI_HINTDY1")){
					UNI_HINTDY1=v;
				}else
				if(!strcmp(ccc,"UNI_HINTDLY1")){
					UNI_HINTDLY1=v;
				}else
				if(!strcmp(ccc,"UNI_HINTDY2")){
					UNI_HINTDY2=v;
				}else
				if(!strcmp(ccc,"UNI_HINTDLY2")){
					UNI_HINTDLY2=v;
				};
			};
		};
	};
	Gclose(F);
};
UNICODETABLE* UNIFONTS::FindFont(char* Name){
	for(int i=0;i<NFonts;i++)if(!_stricmp(UFONTS[i].FontName,Name))
		return &UFONTS[i].UTBL;
	return NULL;
};
bool IsPerspCameraMode();

bool CheckGP_InsideEx( int gpID, int sprID, int dx, int dy, byte NI )
{	
	const DWORD c_ClickAlphaRef = 0x00000000;
	if (sprID >= 4095) sprID &= 4095;
	DWORD res = 0;
	for (int ddx = -2; ddx <= 2;ddx += 4)
    {
		for (int ddy =-2;ddy <= 2;ddy += 4)
        {
			DWORD R = GPS.GetGPAlpha( gpID, sprID, GetNatColor( NI ), dx + ddx, dy + ddy, false );
			if (R > res) res =R;
		}
	}
	return (res > c_ClickAlphaRef) && res != c_OutsideChunks;
} // CheckGP_Inside

CEXPORT bool CheckGP_Inside(int gpID,int sprID,int dx,int dy)
{	
	const DWORD c_ClickAlphaRef = 0x00000000;
	if (sprID >= 4095) sprID &= 4095;
	DWORD res = GPS.GetGPAlpha( gpID, sprID, GetNatColor(0), dx, dy, false ); 	
	return (res > c_ClickAlphaRef) && res != c_OutsideChunks;
} // CheckGP_Inside

struct VisibleGP
{
	WORD	            m_ID;
	WORD	            m_GPID;
	WORD	            m_Frame;

    DWORD               m_ModelID;
    SpriteBuilding*     m_pBuilding;
    Matrix4D            m_TM;
}; // struct VisibleGP

const int   c_MaxVisibleGP = 8192;

static_array<VisibleGP, c_MaxVisibleGP>     g_VisGP;    

void ClearVisibleGP()
{
	g_VisGP.clear();
}

void RenderVisibleGP()
{
    int nVGP = g_VisGP.size();
	for(int i = nVGP - 1; i >= 0; i--)
	{
		const VisibleGP& vgp = g_VisGP[i];
		GPS.DrawRect( vgp.m_TM.e30, vgp.m_TM.e31, 
						GPS.GetGPWidth( vgp.m_GPID, vgp.m_Frame )>>(5 - Shifter),
						GPS.GetGPHeight( vgp.m_GPID, vgp.m_Frame )>>(5 - Shifter),
						0x88FF5533 );
	}
	GPS.FlushLines();
} // RenderVisibleGP

void RegisterVisibleGP( OneObject* OB, int gpID, int sprID, int x, int y, float scale )
{
	if (!OB) return;
	if( OB->notSelectable()/*(OB->NotSelectable&&OB->NewBuilding) || OB->newMons->NotSelectable*/ )return;
    int nVGP = g_VisGP.size();
	if (nVGP == c_MaxVisibleGP)
	{
		Log.Error( "Maximal number of visible GP reached!" );
		return;
	}
	VisibleGP& vgp = g_VisGP.expand();
	vgp.m_ID	    = OB->Index;
	vgp.m_GPID	    = gpID;
	vgp.m_Frame     = sprID;
	vgp.m_TM.e30    = x;
	vgp.m_TM.e31    = y;
    vgp.m_TM.e00    = scale;
    vgp.m_ModelID   = 0xFFFFFFFF;
    vgp.m_pBuilding = NULL;
} // RegisterVisibleGP

void RegisterVisibleGP( OneObject* OB, int modelID, const Matrix4D& tm )
{
    if (!OB) return;
	//if(OB->NotSelectable||OB->newMons->NotSelectable)return;
	if(OB->notSelectable())return;
    int nVGP = g_VisGP.size();
    if (nVGP == c_MaxVisibleGP)
    {
        Log.Error( "Maximal number of visible GP reached!" );
        return;
    }
    VisibleGP& vgp = g_VisGP.expand();
    vgp.m_ID	    = OB->Index;
    vgp.m_TM        = tm;
    vgp.m_ModelID   = modelID;
    vgp.m_pBuilding = NULL;
} // RegisterVisibleGP

void RegisterVisibleGP( OneObject* OB, SpriteBuilding* sb )
{
    if (!OB) return;
    //if(OB->NotSelectable||OB->newMons->NotSelectable)return;
	if(OB->notSelectable())return;
    int nVGP = g_VisGP.size();
    if (nVGP == c_MaxVisibleGP)
    {
        Log.Error( "Maximal number of visible GP reached!" );
        return;
    }
    VisibleGP& vgp = g_VisGP.expand();
    vgp.m_ID	    = OB->Index;
    vgp.m_ModelID   = 0xFFFFFFFF;
    vgp.m_pBuilding = sb;
} // RegisterVisibleGP

WORD TransparentBuildingID      = 0xFFFF;
WORD PrevTransparentBuildingID  = 0xFFFF;
WORD TransparentBuildingAlpha   = 0;

const DWORD c_BuildingAlphaBias = 10;
Ray3D _getRay(int x,int y){
    static int rx=1756762;
    static int ry=1656762;
    static Ray3D R;
    if(x==rx && y==ry)return R;
    R=ICam->GetPickRay( x, y );
    rx=x;
    ry=y;
    return R;
}
WORD CheckCoorInGP( int x, int y )
{
	try{
		WORD    BuildingID      = 0xFFFF;
		WORD    SelUnitID       = 0xFFFF;
		bool    bSelBuilding    = true;
		WORD    mID             = 0xFFFF;
		float   minDist         = FLT_MAX;

		int nVGP = g_VisGP.size();
		for(int i = nVGP - 1; i >= 0; i--)
		{
			const VisibleGP& vgp = g_VisGP[i];
			if (false/*vgp.m_ModelID == 0xFFFFFFFF*/)
			{
				if(minDist==FLT_MAX){
					byte        cni     = 0;
					OneObject*  OB      = NULL;
					bool        bPtIn   = false;
					if (vgp.m_ID < MAXOBJECT)
					{
						OB = Group[vgp.m_ID];
						if (OB) cni = OB->NNUM;
					}

					float cx, cy;
					int gpID, sprID;
					if (vgp.m_pBuilding)
					{
						if (!vgp.m_pBuilding->m_ScreenBounds.PtIn( x, y )) 
						{
							continue;
						}

						Vector4D p0( x, y, 0.0f, 1.0f ); 
						Vector4D p1( x, y, 1.0f, 1.0f ); 
						p0 *= vgp.m_pBuilding->m_ScreenToSpriteTM;
						p1 *= vgp.m_pBuilding->m_ScreenToSpriteTM;
						float t = p0.z/(p0.z - p1.z);
						cx      = p0.x + t*(p1.x - p0.x);
						cy      = p0.y + t*(p1.y - p0.y);
						gpID    = vgp.m_pBuilding->m_GPID;
						sprID   = vgp.m_pBuilding->m_FrameID;
						DWORD alpha = ISM->GetAlpha( gpID, sprID, GetNatColor( cni ), cx, cy, true );
						bPtIn   = (alpha > c_BuildingAlphaBias);
					}
					else
					{
						cx      = (float( x ) - vgp.m_TM.e30)/vgp.m_TM.e00;
						cy      = (float( y ) - vgp.m_TM.e31)/vgp.m_TM.e00;
						gpID    = vgp.m_GPID;
						sprID   = vgp.m_Frame;
						bPtIn   = CheckGP_InsideEx( gpID, sprID, cx, cy, cni );
					}

					if (OB && bPtIn)
					{
						bool nbld = OB->NewBuilding;
						if ((nbld && bSelBuilding) || !nbld)
						{
							SelUnitID    = vgp.m_ID;
							bSelBuilding = nbld;
						}
						if (nbld)
						{
							BuildingID   = vgp.m_ID;
						}
					}
				}
			}
			else
			{
				float dist = 0.0f;
				Ray3D ray = _getRay( x, y );
				OneObject* OB=Group[vgp.m_ID];
				if(OB){
					int r,ch,uh;
					OB->GetCylinder(r,ch,uh);
					int x0=OB->GetAttX()>>4;
					int y0=OB->GetAttY()>>4;				
					AABoundBox AB(x0-r,y0-r,uh,x0+r,y0+r,uh+ch);
					if(AB.Overlap(ray)){
						dist=ray.getOrig().distance(AB.GetCenter())-int(OB->newMons->NewParams.SelPrio)*1000;
						AABoundBox AB(x0-r/2,y0-r/2,uh,x0+r/2,y0+r/2,uh+ch);
						if(AB.Overlap(ray)){
							dist-=300;
							AABoundBox AB(x0-r/4,y0-r/4,uh,x0+r/4,y0+r/4,uh+ch);
							if(AB.Overlap(ray)){
								dist-=300;
							}
						}
						if(dist<minDist){
							minDist     = dist;
							mID         = vgp.m_ID;
							SelUnitID   = mID;
						}
					}
				}
				/*
				if (IMM->Intersects( x, y, vgp.m_ModelID, vgp.m_TM, dist ))
				{
				if (dist < minDist) 
					{
						minDist     = dist;
						mID         = vgp.m_ID;
						SelUnitID   = mID;
					}
				}
				*/
			}
		}

	    

		if(BuildingID!=0xFFFF && BuildingID!=SelUnitID && SelUnitID!=0xFFFF)
		{
			OneObject* SEL=Group[SelUnitID];
			OneObject* BUI=Group[BuildingID];
			if(SEL&&BUI&&SEL->RealY<BUI->RealY)TransparentBuildingID=BuildingID;
		}

		return SelUnitID;
	}catch(...){
		return 0xFFFF;
	}
} // CheckCoorInGP

WORD CheckCoorInGP_withoutMask( int x, int y ,byte Mask)
{
	word BuildingID      = 0xFFFF;
	word SelUnitID       = 0xFFFF;
    bool bSelBuilding    = true;
	bool selbld          = true;
	float   minDist         = FLT_MAX;

    int nVGP = g_VisGP.size();
	for(int i = nVGP - 1; i >= 0; i--)
	{
		const VisibleGP& vgp = g_VisGP[i];
		if (false/*vgp.m_ModelID == 0xFFFFFFFF*/)
		{
			if(minDist==FLT_MAX){
			
				byte        cni     = 0;
				OneObject*  OB      = NULL;
				bool        bPtIn   = false;
				if (vgp.m_ID < MAXOBJECT)
				{
					OB = Group[vgp.m_ID];
					if (OB) cni = OB->NNUM;
				}

				if (!OB || (OB->NMask&Mask)) continue;

				float cx, cy;
				int gpID, sprID;
				if (vgp.m_pBuilding)
				{
					if (!vgp.m_pBuilding->m_ScreenBounds.PtIn( x, y )) 
					{
						continue;
					}

					Vector4D p0( x, y, 0.0f, 1.0f ); 
					Vector4D p1( x, y, 1.0f, 1.0f ); 
					p0 *= vgp.m_pBuilding->m_ScreenToSpriteTM;
					p1 *= vgp.m_pBuilding->m_ScreenToSpriteTM;
					float t = p0.z/(p0.z - p1.z);
					cx      = p0.x + t*(p1.x - p0.x);
					cy      = p0.y + t*(p1.y - p0.y);
					gpID    = vgp.m_pBuilding->m_GPID;
					sprID   = vgp.m_pBuilding->m_FrameID;
					DWORD alpha = ISM->GetAlpha( gpID, sprID, GetNatColor( cni ), cx, cy, true );
					bPtIn   = (alpha > c_BuildingAlphaBias);
				}
				else
				{
					cx      = (float( x ) - vgp.m_TM.e30)/vgp.m_TM.e00;
					cy      = (float( y ) - vgp.m_TM.e31)/vgp.m_TM.e00;
					gpID    = vgp.m_GPID;
					sprID   = vgp.m_Frame;
					bPtIn   = CheckGP_InsideEx( gpID, sprID, cx, cy, cni );
				}
		        
				if (OB && bPtIn)
				{
					bool nbld = OB->NewBuilding;
					if ((nbld && bSelBuilding) || !nbld)
					{
						SelUnitID    = vgp.m_ID;
						bSelBuilding = nbld;
					}
					if (nbld)
					{
						BuildingID   = vgp.m_ID;
					}
				}
			}
		}
		else
		{
            float dist = 0.0f;
			Ray3D ray = _getRay( x, y );
			OneObject* OB=Group[vgp.m_ID];
			if(OB&&!(OB->NMask&Mask)){
				int r,ch,uh;
                OB->GetCylinder(r,ch,uh);
				int x0=OB->GetAttX()>>4;
				int y0=OB->GetAttY()>>4;				
				AABoundBox AB(x0-r,y0-r,uh,x0+r,y0+r,uh+ch);
				if(AB.Overlap(ray)){					
					dist=ray.getOrig().distance(AB.GetCenter())-int(OB->newMons->NewParams.SelPrio)*1000;
					AABoundBox AB(x0-r/2,y0-r/2,uh,x0+r/2,y0+r/2,uh+ch);
					if(AB.Overlap(ray)){
						dist-=300;
						AABoundBox AB(x0-r/4,y0-r/4,uh,x0+r/4,y0+r/4,uh+ch);
						if(AB.Overlap(ray)){
							dist-=300;
						}
					}
					if(dist<minDist){
						minDist=dist;
						bool nbld=OB->NewBuilding;
						if( ( nbld && selbld ) || (!nbld) ){
							SelUnitID=vgp.m_ID;
							selbld=nbld;
						}
						if( nbld )
						{
							BuildingID=vgp.m_ID;
						}
					}
				}
			}
			/*
			if (IMM->Intersects( x, y, vgp.m_ModelID, vgp.m_TM, dist ))
			{
				if(dist<minDist){					
					OneObject* OB=Group[vgp.m_ID];
					if(OB&&!(OB->NMask&Mask)){
						minDist=dist;
						bool nbld=OB->NewBuilding;
						if( ( nbld && selbld ) || (!nbld) ){
							SelUnitID=vgp.m_ID;
							selbld=nbld;
						}
						if( nbld )
						{
							BuildingID=vgp.m_ID;
						}
					}
				}
			}
			*/
		}
	}

	if(BuildingID!=0xFFFF&&BuildingID!=SelUnitID&&SelUnitID!=0xFFFF){
		OneObject* SEL=Group[SelUnitID];
		OneObject* BUI=Group[BuildingID];
		if(SEL&&BUI&&SEL->RealY<BUI->RealY)TransparentBuildingID=BuildingID;
	}
	return SelUnitID;
} // CheckCoorInGP

#include "FCompressor.h"

extern FCompressor FCOMP;
bool FC_IsInit=0;

typedef bool FileAction(HANDLE ActionFile, WIN32_FIND_DATA* WFD, const char* FileName, void* Param);

int ProcessTree(const char* lpszTreeNode , FileAction* FA, void* Param);

bool g17_FileAction(HANDLE ActionFile, WIN32_FIND_DATA* WFD, const char* FileName, void* Param){
	char STR1[512]="Cash\\";
	strcat(STR1,FileName+2);
	int L1=strlen(FileName)+5;
	strupr(STR1);
	char* sg=strstr(STR1,".G17");
	if(sg){
		strcpy(sg,".G16");
		for(int i=5;i<L1;i++)if(STR1[i]=='\\'||STR1[i]=='/')STR1[i]='_';
		if(CheckIfFileExists(STR1)){
			DeleteFile(FileName);
		}
	}
	return true;
}
void DeleteRedundantG17(){
    ProcessTree(".",g17_FileAction,NULL);
}
char tg17_cc[128];
bool tg17_FileAction(HANDLE ActionFile, WIN32_FIND_DATA* WFD, const char* FileName, void* Param){
	try{
		strcpy(tg17_cc,FileName);
		strupr(tg17_cc);
		char* s=strstr(tg17_cc+3,".G17");
		if(!s)s=strstr(tg17_cc+3,".G2D");
		if(!s)s=strstr(tg17_cc+3,".G16");
		if(s){
			s[0]=0;
			int id=GPS.PreLoadGPImage(tg17_cc);
			for(int i=0;i<2400;i++){
				GPS.ShowGP(0,0,id,i,0);
			}
			FlipPages();
		}        
	}catch(...){
		FILE* F=fopen("bad_g17.log","a");
		if(F){
			fprintf(F,"%s\n",FileName);
			fclose(F);
		}
	}
	return true;
}
void TestAllG17(){
	FILE* F=fopen("bad_g17.log","w");
	if(F){
		fclose(F);
	}
	ProcessTree("testg17",tg17_FileAction,NULL);
	//ProcessTree("UnitsG17",tg17_FileAction,NULL);
}

void ConvertG17_Name(char* Name)
{
	char STR[512];
	char STR2[512];
	strcpy(STR,Name);
	int L=strlen(STR);
	strcat(STR,".g16");

	char g2dSTR[512];
	strcpy(g2dSTR,Name);
	strcat(g2dSTR,".g2D");

    char gp2STR[512];
	strcpy(gp2STR,Name);
	strcat(gp2STR,".gp2");

	if(GSets.AnimQuality){
		char s_g2dSTR[512];
		strcpy(s_g2dSTR,"Short\\");
        strcat(s_g2dSTR,g2dSTR);
		if(CheckIfFileExists(s_g2dSTR)) 
		{
			strcpy( Name, s_g2dSTR );
			return;
		}
	}

    if(CheckIfFileExists(gp2STR)) 
	{
		strcpy( Name, gp2STR );
		return;
	}

	if(CheckIfFileExists(g2dSTR)) 
	{
		strcpy( Name, g2dSTR );
		return;
	}

	if(!CheckIfFileExists(STR))
	{
		//checking in cash
		char STR1[512]="Cash\\";
		strcat(STR1,STR);
		int L1=L+5;
		for(int i=5;i<L1;i++)if(STR1[i]=='\\'||STR1[i]=='/')STR1[i]='_';
		
		if(!CheckIfFileExists(STR1))
		{
GOTOCASH:
			//checking for g17 file
			STR[L]=0;
			strcat(STR,".g17");
			if(CheckIfFileExists(STR))
			{
				_mkdir("Cash");
				ResFile F=RReset(STR);
				if(F!=INVALID_HANDLE_VALUE)
				{
					unsigned char* ptr=NULL;				
					DWORD sz=RFileSize(F);
					bool alloc=0;
					if(!ptr)
					{
						alloc=1;
						ptr=(unsigned char*)malloc(sz);
						RBlockRead(F,ptr,sz);
					}
					unsigned char* outbits=NULL;
					unsigned int outsize=0;
					if (FCOMP.DecompressBlock(&outbits,&outsize,ptr))
					{
						ResFile F1=RRewrite(STR1);
						if(F1!=INVALID_HANDLE_VALUE)
						{
							RBlockWrite(F1,outbits,outsize);
							RClose(F1);
						}
					}
					delete[](outbits);
					if(alloc)free(ptr);
					RClose(F);
				};
				strcpy(STR,STR1);
			};
		}
		else
		{
			if( (GetKeyState('A')&0x8000) && (GetKeyState('S')&0x8000) && (GetKeyState('D')&0x8000) && (GetKeyState('F')&0x8000) ){
				STR[L]=0;
				strcat(STR,".g17");
				DeleteFile(STR);
			}
			///if(GSets.CheckG17_dates)
			{
				//checking if file is not old
				struct _stat g17info;
				struct _stat g16info;
				strcpy(STR2,Name);
				strcat(STR2,".g17");
				if(!(_stat(STR1,&g16info)||_stat(STR2,&g17info)))
				{
					if(g17info.st_mtime>=g16info.st_mtime)
					{
						if(DeleteFile(STR1))goto GOTOCASH;
					}				
				}
			}
			strcpy(STR,STR1);
		}
	}
	strcpy(Name,STR);
};

char g_PathBuf[_MAX_PATH];
const char* ParseFilePathName( const char* path )
{
    char dir	[_MAX_PATH];
    char drive	[_MAX_PATH];
    char file	[_MAX_PATH];
    char ext	[_MAX_PATH];
    _splitpath( path, drive, dir, file, ext );
    strcpy( g_PathBuf, dir );
    strcat( g_PathBuf, file );
    return g_PathBuf;
} // ParseFilePathName

int GetShortGPID( int gpID )
{
    char path[_MAX_PATH];
    strcpy( path, ISM->GetPackagePath( gpID ) );
    _strlwr( path );
    if (strstr( path, "short" )) return gpID;
    sprintf( path, "Short\\%s", ParseFilePathName( path ) );
	char tpath[_MAX_PATH];
	strcpy(tpath,path);
	strcat(tpath,".g2d");
	if(!CheckIfFileExists(tpath))return gpID;
	return ISM->GetPackageID(path);
} // GetShortGPID

int GetLongGPID( int gpID )
{
    char path[_MAX_PATH];
    strcpy( path, ISM->GetPackagePath( gpID ) );
    _strlwr( path );
    if (!strstr( path, "short" )) return gpID;
	char* sh=strstr( path, "short\\" );
	if(!sh)return gpID;	
    strcpy(path,ParseFilePathName( sh+6 ));	
	char tpath[_MAX_PATH];
	strcpy(tpath,path);
	strcat(tpath,".g2d");
	if(!CheckIfFileExists(tpath))return gpID;
	return ISM->GetPackageID(path);
} // GetLongGPID
