/*****************************************************************************/
/*	File:	BodyMover.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-13-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgController.h"
#include "csBodyMover.h"

IMPLEMENT_CLASS( BodyMover );
IMPLEMENT_CLASS( HudMover  );

/*****************************************************************************/
/*	BodyMover implementation
/*****************************************************************************/
BodyMover::BodyMover() 
{
	bDrawHeightMarker	= true;
	bChangedPosition	= true;
	bRiseRotateMode		= false;
	bMoveXYMode			= false;
	markerBase			= 0.2f;
}

void BodyMover::SetChangedPosition( bool changed )
{
	bChangedPosition = changed;
}

BodyMover::~BodyMover()
{
	//delete pNode;
}

bool BodyMover::OnMouseWheel( int delta )
{
	return false;
}

bool BodyMover::OnMouseMove( int mX, int mY, DWORD keys )
{
	bScaleMode		= false;
	bRiseRotateMode = false;
	bMoveXYMode		= false;
	
	if (GetKeyState( VK_CONTROL ) < 0)
	{
		bRiseRotateMode	= true;
	}

	if (true) 
	{
		bMoveXYMode	= true;
	}
	
	if (GetKeyState( VK_CONTROL ) < 0 && GetKeyState( VK_LMENU ) < 0)
	{
		bRiseRotateMode	= false;
		bMoveXYMode		= false;
		bScaleMode		= true;
	}

	ICamera* pCamera = GetCamera();
	if (!pCamera) return false;

	Line3D ray = pCamera->GetPickRay( mX, mY );

	OnMoveNode( mX, mY, &ray );
	return false;
} // BodyMover::OnMouseMove

void BodyMover::OnMoveNode( int mX, int mY, const Line3D* ray )
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		TransformNode* pNode = (TransformNode*)GetChild( i );
		assert( pNode->HasFn( TransformNode::Magic() ) );
		Matrix4D tm = pNode->GetTransform();

		Vector3D tr;
		static Vector3D s_RiseTransform( 0.0f, 0.0f, 0.0f );
		static bool s_LastRised = false;

		static int prX = -1000, prY = -1000;

		if (bRiseRotateMode && prX != -1000 && prY != -1000 && ray)
		{
			float dZ = 0.0f;
			float angX = ((float)(mX - prX)) * c_PI / 512.0f;
			float angY = ((float)(mY - prY)) * c_PI / 512.0f;

			if (ray->IntersectPlane( Plane::xOy, tr ))
			{
				Plane verticalPlane;
				ICamera* pCamera  = GetCamera();
				Vector3D cTr( tm.e30, tm.e31, tm.e32 );
				const Matrix4D& ctm = pCamera->GetWorldTM();
				Vector3D dir( ctm.e20, ctm.e21, ctm.e22 );
				verticalPlane.fromPointNormal( cTr, dir );
				if (ray->IntersectPlane( verticalPlane, tr ))
				{
					if (!s_LastRised) s_RiseTransform = tr;
					dZ = tr.z - s_RiseTransform.z;
					s_RiseTransform = tr;
					s_LastRised = true;

					tm.e32 += dZ;
					if (fabs( dZ ) > 0.0f) SetChangedPosition();
				}
			}

			Matrix4D rot;
			rot.rotation( Vector3D::oZ, angX );
			rot *= tm;
			tm = rot;
			if (fabs( angX ) > 0.0f) SetChangedPosition();

			prX = mX;
			prY = mY;

			pNode->SetTransform( tm );
			return;
		}

		if (bScaleMode && prX != -1000 && prY != -1000)
		{
			float s = 1.0f + 0.01f*((float)(mY - prY));
			Matrix4D sc;
			sc.scaling( s, s, s );
			sc *= tm;
			tm = sc;
			if (s > 0.0f) SetChangedPosition();

			prX = mX;
			prY = mY;

			pNode->SetTransform( tm );

			prX = mX;
			prY = mY;
			return;
		}

		if (bMoveXYMode)
		{
			if (!ray) return;
			Plane movePlane;
			Vector3D cTr( tm.e30, tm.e31, tm.e32 );
			movePlane.fromPointNormal( cTr, Vector3D::oZ );
			if (ray->IntersectPlane( movePlane, tr ))
			{
				tr.z = tm.e32;
				if (fabs( tr.x - tm.e30) + 
					fabs( tr.y - tm.e31) > 0.0f ) SetChangedPosition();
				tm.setTranslation( tr );
			}
		}	

		prX = mX;
		prY = mY;
		s_LastRised = false;
		pickRay = *ray;

		pNode->SetTransform( tm );
	}
} // BodyMover::OnMoveNode

bool BodyMover::OnMouseLBDown( int mX, int mY )	
{
	return false;
}

bool BodyMover::OnMouseRBDown( int mX, int mY )
{
	return false;
}

bool BodyMover::OnMouseLBDbl( int mX, int mY )
{
	return false;
}

bool BodyMover::OnKeyDown( DWORD keyCode, DWORD flags )
{
	if (keyCode == ' ')
	{
		RemoveChildren();
	}
	return false;
}

void BodyMover::OnDraw()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		TransformNode* pNode = (TransformNode*)GetChild( i );
		assert( pNode->HasFn( TransformNode::Magic() ) );
		DrawHeightMarker( pNode->GetTransform() );
	}
} // BodyMover::Draw

void BodyMover::DrawHeightMarker( const Matrix4D& tr )
{
	/*if (!bDrawHeightMarker) return;

	static BaseMesh hm;
	BEGIN_ONCE
		hm.create( 6, 0, vfVertex2t, ptLineList );	
		hm.setShader( IRS->GetShaderID( "lines3D" ) );
	END_ONCE

	VertexIterator v; v << hm;

	float mBase = markerBase;
	if (mBase < tr.e32 * 0.02f) mBase = tr.e32 * 0.02f;

	v[0].x = tr.e30;
	v[0].y = tr.e31;
	v[0].z = tr.e32;

	v[1].x = tr.e30;
	v[1].y = tr.e31;
	v[1].z = 0.0f;

	v[2].x = tr.e30 - mBase;
	v[2].y = tr.e31;
	v[2].z = 0.0f;

	v[3].x = tr.e30 + mBase;
	v[3].y = tr.e31;
	v[3].z = 0.0f;

	v[4].x = tr.e30;
	v[4].y = tr.e31 - mBase;
	v[4].z = 0.0f;

	v[5].x = tr.e30;
	v[5].y = tr.e31 + mBase;
	v[5].z = 0.0f;

	hm.setNPri			( 3 );
	hm.setNVert			( 6 );
	hm.setDiffuseColor	( 0xFFFFFF00 );

	IRS->ResetWorldTM();
	DrawBM( hm );*/
} // BodyMover::DrawHeightMarker

/*****************************************************************************/
/*	HudMover implementation
/*****************************************************************************/
HudMover::HudMover()
{
	bDragging	= false;
	bScaling	= false;
} // HudMover::HudMover

bool HudMover::OnMouseWheel( int delta )
{
	return false;
}

bool HudMover::OnMouseMove( int mX, int mY, DWORD keys )
{
	static int pmX = mX;
	static int pmY = mY;
	
	int dX = mX - pmX;
	int dY = mY - pmY;
	
	pmX = mX;
	pmY = mY;

	for (int i = 0; i < GetNChildren(); i++)
	{
		HudNode* pNode = (HudNode*)GetChild( i );
		if (!pNode->HasFn( HudNode::Magic() )) continue;

		if (bDragging)
		{
			pNode->SetPosX( pNode->GetPosX() + dX );
			pNode->SetPosY( pNode->GetPosY() + dY );
		}

		if (bScaling)
		{
			Rct rct = pNode->GetBounds();
			float w = rct.w;
			pNode->SetScale( pNode->GetScale() + float( dX ) / w );
		}
	}

	return false;
} // HudMover::OnMouseMove

bool HudMover::OnMouseLBDown( int mX, int mY )
{
	if (GetKeyState( VK_CONTROL ) < 0)
	{
		bScaling = true;
	}
	else bDragging = true;

	return false;
}

bool HudMover::OnMouseLBUp( int mX, int mY )
{
	bScaling = false;
	bDragging = false;
	return true;
}

bool HudMover::OnMouseRBDown( int mX, int mY )
{
	return false;
}

bool HudMover::OnMouseLBDbl( int mX, int mY )
{
	return false;
}

bool HudMover::OnKeyDown( DWORD keyCode, DWORD flags )
{
	if (keyCode == ' ')
	{
		RemoveChildren();
	}
	return false;
}

void HudMover::OnDraw()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		HudNode* pNode = (HudNode*)GetChild( i );
		if (!pNode->HasFn( HudNode::Magic() )) continue;
		Rct bound = pNode->GetBounds();
		
		bound   *= pNode->GetScale();
		bound.x += pNode->GetPosX();
		bound.y += pNode->GetPosY();

		/*rsFrame( bound, 0, 0xDD661111 );
		bound.Deflate( 1 );
		rsFrame( bound, 0, 0xDDAA2222 );
		bound.Deflate( 1 );*/
		rsFrame( bound, 0, 0xDDDD1111 );
	}

	rsFlushLines2D();

} // HudMover::OnDraw

