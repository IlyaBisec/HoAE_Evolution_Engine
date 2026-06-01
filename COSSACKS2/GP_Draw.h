#ifndef __GPDRAW_H__
#define __GPDRAW_H__
#pragma once

#ifndef GP_USER
#define GP_API __declspec(dllexport)
#else
#define GP_API __declspec(dllimport)
#endif

typedef unsigned short	word;
typedef char*			LPCHAR; 
void	ErrM(char* s);

#define MaxGPIdx 3000

#pragma pack (push)
#pragma pack (1)

#define NO_PACK ((byte*)0xFFFFFFFF)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#pragma pack (push)
//#pragma pack (8)
#include "..\3dlib\gMotor\gmDefines.h"
#include "..\3dlib\gMotor\mMath2D.h"
#include "..\3dlib\gMotor\mMath3D.h"
#include "..\3dlib\gMotor\mTransform.h"
#include "..\3dlib\gMotor\mQuaternion.h"
#include "..\3dlib\gMotor\mTriangle.h"
#include "..\3dlib\gMotor\mGeom3D.h"
#include "..\3dlib\gMotor\mAlgo.h"
#include "..\3dlib\gMotor\kColorValue.h"
#include "..\3dlib\gMotor\kLog.h"
#include "..\3dlib\gMotor\IRenderSystem.h"
#include "..\3dlib\gMotor\ISpriteManager.h"
#include "..\3dlib\gMotor\IPictureManager.h"
#include "..\3dlib\gMotor\IMediaManager.h"
#include "..\3dlib\gMotor\IEffectManager.h"
#include "..\3dlib\gMotor\vMesh.h"
#pragma pack (pop)

struct OneUnicodeSet
{
	int		DY;
	int		DX;
	BYTE	UseColor;
	int		Start;
	int		NSymbols;
	int		GP_Start;
	int		GPID;
}; // struct OneUnicodeSet

struct UNICODETABLE
{
	int NTables;
	OneUnicodeSet USET[4];
}; // struct UNICODETABLE

struct OneUniFont
{
	char FontName[48];
	UNICODETABLE UTBL;
}; // struct OneUniFont

#pragma pack (pop)

class GP_API UNIFONTS
{
public:
	OneUniFont*		UFONTS;
	int				NFonts;
					
					UNIFONTS	();
					~UNIFONTS	();
	void			LoadFonts	();
	UNICODETABLE*	FindFont	(char* Name);
}; // class UNIFONTS

extern GP_API UNIFONTS UFONTS;

enum GPDrawMode
{
	dmUnknown		= 0,
	dmText			= 1,
	dmSprites		= 2
}; // enum GPDrawMode

/*****************************************************************************/
/*	Class:	SQPicture
/*	Desc:	Wrapper for drawing fat background images
/*****************************************************************************/
class GP_API SQPicture 
{
public:
					SQPicture		();
					SQPicture		( char* name );
					~SQPicture		();	

	void			LoadPicture		( char* name, bool bUnloadOld = true );
	void			Draw			( int x, int y );
    void			Draw			( int x, int y, int w, int h );
	void			Xlat			( byte* tbl );
	int				GetLx			() const { return IPM->GetWidth( m_ID ); }
	int				GetLy			() const { return IPM->GetHeight( m_ID ); }
	void			SetLx			( int lX );
	void			SetLy			( int lY );
	BYTE*			GetPixelData	();

private:

	int				m_ID;
}; // class SQPicture 

/*****************************************************************************/
/*	Class:	GP_System
/*	Desc:	Sprite manager bridge class
/*****************************************************************************/
class GP_API GP_System
{
public:
				GP_System			();
				~GP_System			();

	void		Init();
	void		ShowGP				( int x, int y, int gpID, int sprID, byte Nation );
	void		ShowGP				( int x, int y, float z, int gpID, int sprID, byte Nation );
	void 		DrawWSprite			( int gpID, int sprID, const Matrix4D& transf, DWORD color = 0 );

	void 		DrawLine			( float x1, float y1, float x2, float y2, DWORD color );
	void 		DrawLine			( const Vector3D& beg, const Vector3D& end, DWORD color );
	void 		DrawFillRect		( float x, float y, float w, float h, DWORD color );
	void 		DrawFillRect		( float x, float y, float w, float h, DWORD clt, DWORD crt, DWORD clb, DWORD crb );
	void 		DrawRect			( float x, float y, float w, float h, DWORD color );

	void 		DrawFillRect		( const Vector3D& lt, const Vector3D& rt, const Vector3D& lb, const Vector3D& rb, DWORD color );
	void 		DrawFillRect		( const Vector3D& lt, const Vector3D& rt, const Vector3D& lb, const Vector3D& rb, 
												DWORD clt, DWORD crt, DWORD clb, DWORD crb );
	const char* GetGPName			( int gpID ) { return ISM->GetPackageName( gpID ); }
	void		ShowGPTransformed	( int gpID, int sprID, const Matrix4D& transform, byte Nation );

	int			PreLoadGPImage		( char* gpName );	
	const char*	GetGPPath			( int gpID );
	bool		LoadGP				( int gpID );
	void		UnLoadGP			( int gpID ) { ISM->UnloadPackage( gpID ); }

	int			GetGPHeight			( int gpID, int frameIdx ) { return ISM->GetFrameHeight( gpID, frameIdx ); }
	int			GetGPWidth			( int gpID, int frameIdx );

	void		FlushBatches		();
    void		FlushLines			();
	int			GPNFrames			( int gpID ) { return ISM->GetNFrames( gpID ); }
	int			GetGPNum			() { return ISM->GetNPackages(); }

	void		SetClipArea			( DWORD x, DWORD y, DWORD w, DWORD h ) { if (ISM) ISM->SetClipArea( Rct( x, y, w, h ) ); }
	Rct			GetClipArea			() const					{ if (!ISM) return Rct::null; return ISM->GetClipArea(); }
	void		SetCurrentDiffuse	( DWORD color	)			{ ISM->SetCurrentDiffuse( color ); }
	int			SetShader	        ( int shaderID	)			{ return ISM->SetCurrentShader( shaderID ); }
	int			GetCurrentShader	() const					{ return ISM->GetCurrentShader(); }

	void		SetScale			( float scale	)			{ ISM->SetScale( scale ); }
	bool		IsGPLoaded			( int gpID		)			{ return ISM->IsPackageLoaded( gpID ); }

	void		EnableClipping		( bool bEnable = true )		{ ISM->EnableClipping( bEnable ); }
	void		OnFrame				()							{ ISM->OnFrame(); FlushLines(); }
	void 		EnableZBuffer		( bool bEnable = true );

	bool 		GetGPBoundFrame		( int gpID, int sprID, Rct& frame ) { return ISM->GetBoundFrame( gpID, sprID, frame ); }
	
	DWORD		GetGPAlpha			( int gpID, int sprID, DWORD color, int ptX, int ptY, bool precise = false )
	{ return ISM->GetAlpha( gpID, sprID, color, ptX, ptY, precise ); }
	DWORD		GetCurrentDiffuse	() const { return ISM->GetCurrentDiffuse(); }
	GPDrawMode	GetDrawMode			() const { return m_DrawMode; }
	void		SetDrawMode			( GPDrawMode _mode );

	void		SetScreenSpaceTM	( const Matrix4D* pTM = NULL ) 
	{ 
		if (pTM) m_TM = *pTM;
		m_bUseScreenSpaceTM = pTM ? true : false; 
	}

	const Matrix4D* GetScreenSpaceTM() const { return m_bUseScreenSpaceTM ? &m_TM : NULL; }

	//  unicode table
	UNICODETABLE**			UNITBL;			

protected:
	GPDrawMode				m_DrawMode;
	Matrix4D				m_TM;
	bool					m_bUseScreenSpaceTM;

};  //  class GP_System

/*****************************************************************************/
/*	Class:	SpriteBuilding
/*  Desc:   Structure which keeps temporally aliased information about building
/*****************************************************************************/
struct SpriteBuilding
{
    DWORD           m_ObjectID;     //  building unique object id
    DWORD           m_PartID;       //  building part id

    int             m_GPID;
    int             m_FrameID;

    Matrix4D        m_SpriteToScreenTM;
    Matrix4D        m_ScreenToSpriteTM;
    Rct             m_ScreenBounds;
    Vector3D        m_WorldPos;
    
    SpriteBuilding* m_pNextPart; 

    SpriteBuilding() : m_ObjectID( 0xFFFFFFFF ), m_PartID( 0xFFFFFFFF ), m_GPID(-1), m_FrameID(0) {}
    SpriteBuilding( DWORD objID, DWORD partID, int gpID, int sprID ) 
        : m_ObjectID( objID ), m_PartID( partID ), m_GPID( gpID ), m_FrameID( sprID ) {}

        DWORD hash () const
        {
            DWORD h = (m_ObjectID << 13)^m_ObjectID;
            h += 729*m_PartID + m_FrameID + 15731;
            return h;
        }

        bool equal( const SpriteBuilding& el ) const
        { 
            return ((m_ObjectID == el.m_ObjectID)   && 
                    (m_PartID   == el.m_PartID)     &&
                    (m_FrameID  == el.m_FrameID)); 
        }

        void copy( const SpriteBuilding& el ) 
        { 
            m_ObjectID          = el.m_ObjectID;
            m_PartID            = el.m_PartID;

            m_SpriteToScreenTM  = el.m_SpriteToScreenTM;
            m_ScreenToSpriteTM  = el.m_ScreenToSpriteTM;
            m_ScreenBounds      = el.m_ScreenBounds;
            m_GPID              = el.m_GPID;
            m_FrameID           = el.m_FrameID;
            m_WorldPos          = el.m_WorldPos;
        }
}; // struct SpriteBuilding

extern GP_API GP_System GPS;

#pragma pack (pop)

class GP_API LocalGP
{
public:
	int		GPID;
	bool	Uniq;
	
			LocalGP		();
			LocalGP		( char* Name );
			~LocalGP	();
	void	Load		( char* Name );
	void	UnLoad		();
}; // class LocalGP

CEXPORT void g_SetWindParam( int x, int y, int x1, int y1 );
CEXPORT void g_GetWindParam( int& x, int& y, int& x1, int& y1 );

class OneObject;
void    ClearVisibleGP();
void    RegisterVisibleGP( OneObject* OB, int gpID, int sprID, int x, int y, float scale = 1.0f );
int     GetShortGPID( int gpID );
int     GetLongGPID( int gpID );
#endif // __GPDRAW_H__
