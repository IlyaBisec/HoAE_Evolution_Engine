/*****************************************************************************/
/*	File:	NewMon3D.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10.12.2002
/*****************************************************************************/
#ifdef _USE3D

#include "gmDefines.h"

#include "windows.h"
#include "kLog.h"
#include "kAssert.h"
#include "kArray.hpp"
#include "kHash.hpp"
#include "ResFile.h"
#include "kIO.h"
#include "kString.h"
#include "IRenderSystem.h"
#include "rsVertex.h"
#include <stdio.h>

#include "mMath2D.h"
#include "mMath3D.h"
#include "mGeom3D.h"

#include "mCamera.h"
#include "gpMesh.h"
#include "gpAnimModel.h"
#include "ddini.h"
#include "FastDraw.h"
#include "Scape3D.h"

#include "NewMon3D.h"
#include "MapDiscr.h"

Camera			MCAM;

/*****************************************************************************/
/*	NewMon3D implementation
/*****************************************************************************/
NewMon3D::NewMon3D( NewMonster* papa )
{
	papaMonster = papa;
	mdlFile[0]	= 0;
	mdl = new AnimModel();
	isLoaded = false;
}

NewMon3D::~NewMon3D()
{
	delete mdl;
}

extern IRenderSystem* IRS;

void NewMon3D::SetModelFile( const char* fname )
{
	char fullName[_MAX_PATH];
	strcpy( fullName, "Models\\" );
	strcat( fullName, fname );
	strcat( fullName, ".c2m" );
	strcpy( mdlFile, fullName );
}

//extern ShadowMgr SHM;
//extern ShadowMode shadowMode;

void NewMon3D::Draw( OneObject* OB )
{
	if (!isLoaded)
	{
		mdl->Load( mdlFile );
		isLoaded = true;
	}

	OneObjectEx* OBE = OB->ExObj;
	if (!OBE) return;
	OBE->CalculateWorldTransform(); 
	
	/*if (shadowMode != shmNoShadow)
	{
		SHM.castShadow( IRS, *mdl, OBE->getTransform() );
	}*/

	IRS->SetWorldMatrix( OBE->getTransform() );
	mdl->Draw();
	
	if (GetKeyState( VK_RCONTROL ) < 0)
	{
		mdl->DrawAABB( 0xFFFF0000 );
		mdl->DrawBoneAABBs( 0xFF0000FF );
	}

	IRS->SetWorldMatrix( Matrix4D::identity );
	OBE->SetInsideCamera( MCAM );

} // NewMon3D::Draw	

void NewMon3D::ProgressAnim( int frame )
{
	mdl->ProgressAnim( frame );
}

/*****************************************************************************/
/*	OneObjectEx implementation
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/*	Func:	OneObjectEx::CalculateWorldTransform
/*	Desc:	Builds world space transform matrix for given object's model
/*---------------------------------------------------------------------------*/
void OneObjectEx::CalculateWorldTransform()
{
	int GetHeight( int x, int y );

	float mx, my; // in map coordinates
	float wx, wy; // in nodes
	//  divide by 16
	wx = ((float)coreObj->RealX) * 0.0625f;
	wy = ((float)coreObj->RealY) * 0.0625f;
	//  divide by 32
	mx = wx * 0.03125f;
	my = wy * 0.03125f;

	//  find 'up' vector
	Vector4D& vecUp = transform.getV2();
	GetUpWorldDirection( wx, wy, vecUp );

	//  find 'forward' vector
	Vector4D& vecHead = transform.getV1();

	vecHead.x = -(float)TCos[coreObj->RealDir];
	vecHead.y = -(float)TSin[coreObj->RealDir];
	vecHead.z = 0.0f;
	
	if (vecUp.z >= c_SmallEpsilon) 
		vecHead.z = -(vecUp.x * vecHead.x + vecUp.y * vecHead.y)/vecUp.z;
	vecHead.w = 0.0f;
	vecHead.normalize();
	
	//  find 'left' vector
	Vector4D& leftV = transform.getV0();
	//  left direction is cross( head, up )
	leftV.x = vecHead.y * vecUp.z - vecHead.z * vecUp.y; 
	leftV.y = vecHead.z * vecUp.x - vecHead.x * vecUp.z;
	leftV.z = vecHead.x * vecUp.y - vecHead.y * vecUp.x;
	leftV.w = 0.0f;

	int GetHeight( int x, int y );
	float H0 = (float(GetHeight( wx - 3,	wy		)) + 
				float(GetHeight( wx + 3,	wy		)) +
				float(GetHeight( wx,		wy - 3	)) + 
				float(GetHeight( wx,		wy + 3	)) ) * 0.25f;
	//  translation
	Vector4D& trans = transform.getV3();
	trans.x = mx * c_WorldGridStepX;
	trans.y = my * c_WorldGridStepY + 5.0f;
	trans.z = H0 + 10.0f;
} // OneObjectEx::CalculateWorldTransform

/*---------------------------------------------------------------------------*/
/*	Func:	OneObjectEx::GetUpWorldDirection
/*	Desc:	Calculates 'up' vector for the object
/*---------------------------------------------------------------------------*/
void OneObjectEx::GetUpWorldDirection( int x, int y, Vector4D& up )
{
	void GetGroundNormal( int x, int y, Vector4D& vec ); 
	GetGroundNormal( x, y, up );
} // OneObjectEx::GetUpWorldDirection

void OneObjectEx::SetInsideCamera( Camera& cam )
{
	Vector3D pos( transform.e30, transform.e31, transform.e32 );
	Vector3D dir( transform.e00, transform.e10, transform.e20 ); 
	Vector3D up ( transform.e02, transform.e12, transform.e22 );

	float zn = 20.0f;
	float zf = 1000.0f;

	dir.reverse();
	float tmpX = dir.x;
	dir.x = dir.y;
	dir.y = tmpX;

	pos.z += 50.0f;
	cam.setOrtho( false );

	cam.setPos( pos ); 	
	cam.setUp ( up  );
	cam.setDir( dir );
	cam.setViewPort( 0, 0, 450, 300 );

	cam.setZn( zn );
	cam.setZf( zf );
	cam.setHFOV( c_PI / 2 );

	//  adjust pos.z for not to intersect ground with near clip rect
	/*
	Frustum& fru = (Frustum&)cam.getFrustum();
	fru.calculate( cam.getViewM(), cam.getProjM() );
	const Vector3D& lb = fru.getNearLBVec();
	const Vector3D& rb = fru.getNearRBVec();

	float maxH = 0.0f;
	for (float i = 0.0f; i < 1.0f; i += 0.125f)
	{
		Vector3D ray = lb;
		ray *= 1.0f - i;
		ray.addWeighted( rb, i );
		ray.add( pos );
		int H = GetHeight( ray.x, ray.y );
		if (ray.z < H)
		{
			pos.z += (H - ray.z); 
		}
		if (ray.z < 0.0f)
		cam.setPos( pos );
	}
	*/
} // OneObjectEx::SetInsideCamera


#endif //_USE3D
