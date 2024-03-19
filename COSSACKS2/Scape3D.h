/*****************************************************************************/
/*	File:	Scape3D.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	14.11.2002
/*****************************************************************************/
#ifndef __SCAPE3D_H__
#define __SCAPE3D_H__

#include "IMediaManager.h"

void FillGroundZBuffer();

const int		c_QuadHorzTris			= 8;

const float		c_WorldGridStepX		= 32.0f;
const float		c_WorldGridStepY		= 32.0f;
const float		c_HalfWorldGridStepY	= c_WorldGridStepY / 2.0f;  

const int		c_HmapGridStepX			= 32;
const int		c_HmapGridStepY			= 32;
const int		c_HalfHmapGridStepY		= c_HmapGridStepY / 2;

const float		c_HmapToWorldX			= c_WorldGridStepX / (float)c_HmapGridStepX;
const float		c_HmapToWorldY			= c_WorldGridStepY / (float)c_HmapGridStepY;

const int		c_QuadSide			= c_QuadHorzTris * c_HmapGridStepX;
const int		c_2QuadSide			= c_QuadSide * 2;

const int		c_QuadVert	= (c_QuadHorzTris + 1) * (c_QuadHorzTris + 1);
const int		c_QuadInd	= c_QuadHorzTris * c_QuadHorzTris * 6; 

const float		c_Cos30					= 0.86602540378443864676372317075294f;
const float		c_InvCos30				= 1.15470053837925152901f;
const float		c_Tan30					= 0.57735026918962576450914878050196f;

const float		c_MaxMapHeight			= 256.0f;
const float		c_BottomFieldExtent		= c_MaxMapHeight / c_Tan30;

#pragma pack( push )
//#pragma pack( 8 )

extern DIALOGS_API IRenderSystem*	IRS;
//extern int RealLx, RealLy, mapx, mapy;
extern short* THMap;
int GetHeight(int x,int y);

float GetGround_d2x( int x, int y );
	

float		GetZRange						();
void		SetupCamera						();
ILight*		GetGameLight					();
extern		ICamera*	ICam;

void		WorldToScreenSpace				( Vector4D& vec );
void		WorldToCameraSpace				( Vector4D& vec );
void		WorldToScreenSpace				( Vector3D& vec );
Vector3D	ScreenToWorldSpace				( int mx, int my );
Matrix4D    ScreenToWorldSpace				();


void		DrawKangaroo					();
bool		IsPerspCameraMode				();
DWORD		GetMaxMapX						();
DWORD		GetMaxMapY						();
void		InvalidateTerrainPatch			( int x, int y, int w, int h );

void		DrawMinimapFrustumProjection	( const Rct& miniRct );
const Vector3D* GetCameraIntersectionCorners();
bool CheckObjectVisibility(int x,int y,int z,int R);
void		DrawMinimapRct					( const Rct& rct );

Matrix4D	GetAlignGroundTransform			( const Vector3D& pivot );
Matrix4D	GetAlignGroundLineTM			( const Vector3D& pos, const Vector3D& pivot, 
												int x1, int y1, int x2, int y2 );
Matrix4D	GetAlignTerraTransform			( const Vector3D& pos, const Vector3D& pivot );
Matrix4D	GetAlignLineTransform			( const Vector3D& pivot, int x1, int y1, int x2, int y2 );
Matrix4D	GetTopmostTransform				( const Vector3D& pivot, const Vector3D& pos );
Matrix4D	GetBillboardTransform			( const Vector3D& pivot );
Matrix4D	GetOrientedBillboardTransform	( const Vector3D& pivot, float ang, float scaleY = 1.0f );
Matrix4D	GetRolledBillboardTransform		( const Vector3D& pivot, float ang );
Matrix4D    GetPseudoProjectionTM           ( const Vector3D& pos, float& planeFactor );


void		SetTerrainZBias					( float bias );
float		GetTerrainZBias					();

void		SetLight( DWORD ambient, DWORD diffuse, DWORD specular, const Vector3D* dir );
void		GetLight( DWORD& ambient, DWORD& diffuse, DWORD& specular, Vector3D& dir );
void		SetLight( DWORD ambient, DWORD diffuse, DWORD specular, const Vector3D* dir );
void		GetLight( DWORD& ambient, DWORD& diffuse, DWORD& specular, Vector3D& dir );

Vector3D		SkewPt( float x, float y, float z );
const Matrix4D&	GetSkewTM();
void AnimateLModeSwitch( const Vector3D& moveDir, bool bToL = true );

struct OrthoCameraMode {
	static void Enable();
	static void Disable();
	static void Toggle();
	static bool IsEnabled();
};

bool WorldToClosestScreenPoint(Vector3D &Pos); // returns true if point is within frustum

#pragma pack ( pop )

#endif // __SCAPE3D_H__