/*****************************************************************/
/*  File:   Camera.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "mGeom3D.h"
#include "mMath3D.h"
#include "mCamera.h"

#ifndef _INLINES
#include "mCamera.inl"
#endif //!_INLINES

/*****************************************************************/
/*  Camera implementation
/*****************************************************************/
Camera::Camera()
{
	reset();
}

Camera::Camera( const char* camName )
{
	reset();
}

void Camera::reset()
{
	pos.set( 0.0f, 0.0f, 0.0f );
    dir.set( 0.0f, 0.0f, 1.0f );
    up.set( 0.0f, 1.0f, 0.0f );

    Zn      = 1.0f;
    Zf      = 100.0f;
    
	scrW	= 1024.0f;
	scrH	= 768.0f;
	scrX	= 0.0f;
	scrY	= 0.0f;
    
	setHFOV( c_HalfPI );
    setOrtho( false );
	setViewVolumeWidth( 1.0f );

    at. set( 0.0f, 0.0f, 1.0f );

	calcProjM();
	calcViewM();
}

void Camera::setPos( float x, float y, float z )
{
	pos.set( x, y, z );
	calcViewM();
}

void Camera::setPos( const Vector3D& pt )
{
	pos.copy( pt );
	calcViewM();
}

/*---------------------------------------------------------------*
/*  Func:	Camera::FocusOnAABB
/*	Desc:	positions camera to fit the object in the viewport
/*			current look direction is preserved
/*	Parm:	aabb - axis-aligned bounding box of the object
/*---------------------------------------------------------------*/
void Camera::FocusOnAABB( AABoundBox& aabb )
{
	float maxSize = aabb.GetDiagonal();
	aabb.GetCenter( at );

	pos.copy( at );
	dir.normalize();
	dir *= maxSize * 2 / (2 * tan( getHFOV() / 2.0f ) );
	pos -= dir;
    dir.normalize();
    calcViewM();
}//  Camera::FocusOnAABB

/*---------------------------------------------------------------------------*/
/*	Func:	Camera::RotateAroundPoint	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void Camera::RotateAroundPoint( float ang, const Vector3D& pt )
{
	Matrix4D rotm;
	rotm.rotation( getUp(), ang );
	Vector4D ndir( getDir() );
	ndir.w = 0.0f;
	ndir.mul( rotm );
	float dist = getPos().distance( pt );
	ndir.normalize();
	setDir( ndir );
	Vector3D npos( ndir );
	npos.reverse();
	npos *= dist;
	npos.add( pt );
	setPos( npos );
} // Camera::RotateAroundPoint

void Camera::calcProjM()
{
	float w, h;

	projM.setNull();

    if (bOrtho)
	{
		w = viewVolWidth;
		h = viewVolWidth * scrH / scrW;

		projM.e00  = 2.0f / w;
		projM.e11  = 2.0f / h;
		projM.e22  = 1 / (Zf - Zn);
		projM.e32  = -Zn / (Zf - Zn);
		projM.e33  = 1.0f;
	}
	else
    {
		float fovd2 = hFov * 0.5f;
		w = 1.0f / tan( fovd2 );
		h = w * scrW / scrH;
		projM.e00  = w;
		projM.e11  = h;
		projM.e22  = Zf / (Zf - Zn);
		projM.e23  = 1.0f;
		projM.e32  = Zn * Zf / (Zn - Zf);
    }
} // Camera::calcProjM

void Camera::calcViewM()
{
	float* el = viewM.getBuf();
	Vector3D xax, yax, zax;

	zax.copy( dir );
	xax.cross( up, zax );
	xax.normalize();
	yax.cross( zax, xax );
	
	el[0] = xax.x;
	el[1] = yax.x;
	el[2] = zax.x;
	el[3] = 0.0f;
	el[4] = xax.y;
	el[5] = yax.y;
	el[6] = zax.y;
	el[7] = 0.0f;
	el[8] = xax.z;
	el[9] = yax.z;
	el[10]= zax.z;
	el[11]= 0.0f;
	el[12]= -xax.dot( pos );
	el[13]= -yax.dot( pos );
	el[14]= -zax.dot( pos );
	el[15]= 1.0f;

	fullM.mul( viewM, projM );
	//frustum.calculate( viewM, projM );
} // Camera::calcViewM

void Camera::UnviewOnXOY( Vector4D& vec )
{
	float Det = viewM.e00 * viewM.e11 - viewM.e01 * viewM.e10;
	if (fabs( Det ) < c_SmallEpsilon) return;
	float a = vec.x - viewM.e30;
	float b = vec.y - viewM.e31;
	
	vec.x = (a * viewM.e11 + b * viewM.e10) / Det;
	vec.y = (a * viewM.e01 + b * viewM.e00) / Det;
	vec.z = 0.0f;

} // Camera::UnviewOnXOY

void Camera::getPickRay( float curX, float curY, Line3D& ray, const Matrix4D* worldMatr )
{
	Vector4D orig( curX, curY, 0.0f, 1.0f );
	Vector4D dir ( curX, curY, 1.0f, 1.0f );
	ScreenToWorldSpace( orig );
	ScreenToWorldSpace( dir );
	
	orig.normW();
	dir.normW();

	if (worldMatr)
	{
		Matrix4D invW;
		invW.inverse( *worldMatr );
		orig.mul( invW );
		dir.mul( invW );
	}
	
	dir -= orig;
	dir.normalize();
	ray.setOrigDir( orig, dir );
} // Camera::getPickRay

void Camera::txtSave( FILE* fp )
{
	if (isOrtho()) fprintf( fp, "(ortho)\n" ); else fprintf( fp, "(persp)\n" );
	fprintf( fp, "pos(%-2.4f %-2.4f %-2.4f);\n dir(%-2.4f %-2.4f %-2.4f);\n ",
					pos.x, pos.y, pos.z, dir.x, dir.y, dir.z );
	fprintf( fp, "up(%-2.4f %-2.4f %-2.4f);\n",
					up.x, up.y, up.z );
	fprintf( fp, "Zn=%-2.4f; Zf=%-2.4f; vFov=%-2.4f; hFov=%-2.4f;",
					Zn, Zf, hFov, getHFOV() );
}

/*---------------------------------------------------------------------------*/
/*	Func:	Camera::MovePosLocal
/*	Desc:	Performs moving of camera position by amount in direction mdir
/*				(mdir is in local camera space)
/*---------------------------------------------------------------------------*/
void Camera::MovePosLocal( const Vector3D& mdir, float amount )
{
	Vector3D nPos( mdir );
	//  transform direction from local camera space to the world space
	Matrix3D tr;
	tr.copy( getViewM() );
	tr.inverse();
	nPos *= tr;
	nPos *= amount;
	nPos += getPos();
	setPos( nPos );
} // Camera::MovePosLocal

void Camera::MovePos( const Vector3D& mdir, float amount )
{
	Vector3D nPos( getPos() );
	nPos.addWeighted( mdir, amount );
	setPos( nPos );
} // Camera::MovePos

void Camera::Zoom( float scale )
{
	if (isOrtho())
	{
		setViewVolumeWidth( getViewVolumeWidth() / (1.0f + scale) );
		return;
	}

	MovePosLocal( Vector3D::oZ, scale * getViewVolumeWidth() );
} // Camera::Zoom




