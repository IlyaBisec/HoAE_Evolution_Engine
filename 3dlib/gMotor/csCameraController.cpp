/*****************************************************************************/
/*	File:	csCameraController.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11.07.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgController.h"
#include "kInput.h"
#include "kIOHelpers.h"
#include "sgCursor.h"
#include "csCameraController.h"

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	CameraController implementation
/*****************************************************************************/
CameraController::CameraController()
{
	m_TotalTM.setIdentity();
}

CameraController::~CameraController()
{
}

void CameraController::OnAttach()
{
	if (GetNChildren() == 0) return;
	TransformNode* pNode = (TransformNode*) GetChild( 0 );
	assert( pNode->HasFn( TransformNode::Magic() ) );
}

void CameraController::Home()
{
	Matrix4D backTM;
	backTM.inverse( m_TotalTM );
	for (int i = 0; i < GetNChildren(); i++)
	{
		sg::BaseCamera* pCamera = (sg::BaseCamera*)GetChild( i );
		if (!pCamera->HasFn( BaseCamera::Magic() ) ) continue;
		Matrix4D tm = pCamera->GetTransform();
		tm *= backTM;
		pCamera->SetTransform( tm );
	}

	m_TotalTM.setIdentity();
} // CameraController::Home

void CameraController::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // CameraController::Serialize

void CameraController::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
} // CameraController::Unserialize

/*****************************************************************************/
/*	FlyController implementation
/*****************************************************************************/
FlyController::FlyController()
{
	mx = -1;
	my = -1;
	flySpeed = 0.01f;
}

FlyController::~FlyController()
{
}

bool FlyController::OnMouseWheel( int delta )
{
	return false;
}

Vector3D FlyController::ScreenToVector( float x, float y )
{
	Vector3D res; res.zero();
	/*if (!vp) return res;
	const float c_RotateRadius = 5.0f;
	
	float halfW = vp->GetW()*0.5f;
	float halfH = vp->GetH()*0.5f;
	res.x = -(x - vp->GetX() - halfW)/(c_RotateRadius * halfW);
	res.y = -(y - vp->GetY() - halfH)/(c_RotateRadius * halfH);
	res.z = 0.0f;

	res.z   = 0.0f;
	float mag = res.norm2();

	if (mag > 1.0f)
	{
		float scale = 1.0f / sqrt( mag );
		res.x *= scale;
		res.y *= scale;
	}
	else res.z = sqrt( 1.0f - mag );*/

	return res;
} // ArcBall::ScreenToVector

bool FlyController::OnMouseMove( int mX, int mY, DWORD keys )
{
//	sg::Canvas* vp = sg::Canvas::GetCurVP();
//	if (!vp) return false;
//
//	for (int i = 0; i < GetNChildren(); i++)
//	{
//		sg::TransformNode* pNode = (sg::TransformNode*)GetChild( i );
//		assert( pNode->HasFn( TransformNode::Magic() ) );
//		Matrix4D tm = pNode->GetTransform();
//
//		if (mx != -1 && my != -1)
//		{
//			Vector3D v( mx - mX, mY - my, 0.0f );
//			v /= (c_PI * 3000.0f);
//			
//			Matrix4D pitch;
//			pitch.rotation( Vector3D::oZ, v.x );
//			Matrix4D yaw;
//			yaw.rotation( Vector3D::oX, -v.y );
//
//			Matrix4D rmat = yaw;
//			//rmat *= pitch;
//			tm *= rmat;
//		}
//
//		pNode->SetTransform( tm );
//	}
//
//	if (mX <= vp->GetX()) 
//	{
//		mX = vp->GetX() + vp->GetW() - 2;
//		//SetCursorPos( mX, mY );
//	}
//
//	if (mX >= vp->GetX() + vp->GetW() - 1) 
//	{
//		mX = vp->GetX() + 1;
//		//SetCursorPos( mX, mY );
//	}
//
//	if (mY <= vp->GetY()) 
//	{
//		mY = vp->GetY() + vp->GetH() - 2;
//		//SetCursorPos( mX, mY );
//	}
//
//	if (mY >= vp->GetY() + vp->GetH() - 1) 
//	{
//		mY = vp->GetY() + 1;
//		//SetCursorPos( mX, mY );
//	}
//
//	mx = mX; my = mY;

	return false;
} // FlyController::OnMouseMove

bool FlyController::OnMouseLButtonDown( int mX, int mY )
{
	return false;
}

bool FlyController::OnMouseRButtonDown( int mX, int mY )
{
	return false;
}

bool FlyController::OnMouseLButtonUp( int mX, int mY )
{
	return false;
}

bool FlyController::OnMouseRButtonUp( int mX, int mY )
{
	return false;
}

void FlyController::OnActivate( bool bActive )
{
	if (bActive) 
	{
		//ShowCursor( FALSE );
		POINT pt;
		GetCursorPos( &pt );
		mx = pt.x;
		my = pt.y;
	}
	else
	{
		//ShowCursor( TRUE );
	}
} // FlyController::OnActivate

void FlyController::OnUpdate()
{
//	for (int i = 0; i < GetNChildren(); i++)
//	{
//		sg::TransformNode* pNode = (sg::TransformNode*)GetChild( i );
//		assert( pNode->HasFn( TransformNode::Magic() ) );
//		Matrix4D tm = pNode->GetTransform();
//		
//		static DWORD curTime = GetTickCount();
//		DWORD newTime = GetTickCount();
//		float ds = float( newTime - curTime ) * GetFlySpeed();
//		curTime = newTime;
//
//		if (GetKeyState( 'W' ) < 0 ||
//			GetKeyState( 'w' ) < 0) 
//		{
//			//  look direction
//			Vector3D* dir = (Vector3D*)&(tm.getV2());
//			Vector3D* pos = (Vector3D*)&(tm.getV3());
//			pos->addWeighted( *dir, ds );
//		}
//
//		if (GetKeyState( 'S' ) < 0 ||
//			GetKeyState( 's' ) < 0) 
//		{
//			//  look direction
//			Vector3D* dir = (Vector3D*)&(tm.getV2());
//			Vector3D* pos = (Vector3D*)&(tm.getV3());
//			pos->addWeighted( *dir, -ds );
//		}
//		
//		if (GetKeyState( 'A' ) < 0 ||
//			GetKeyState( 'a' ) < 0) 
//		{
//			//  right direction
//			Vector3D* dir = (Vector3D*)&(tm.getV0());
//			Vector3D* pos = (Vector3D*)&(tm.getV3());
//			pos->addWeighted( *dir, -ds );
//		}
//		
//		if (GetKeyState( 'D' ) < 0 ||
//			GetKeyState( 'd' ) < 0) 
//		{
//			//  right direction
//			Vector3D* dir = (Vector3D*)&(tm.getV0());
//			Vector3D* pos = (Vector3D*)&(tm.getV3());
//			pos->addWeighted( *dir, ds );
//		}
//
//		pNode->SetTransform( tm );
//	}
} // FlyController::OnUpdate

bool FlyController::OnKeyDown( DWORD keyCode, DWORD flags )
{
	return false;	
} // FlyController::OnKeyDown

void FlyController::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // FlyController::Serialize

void FlyController::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
} // FlyController::Unserialize

/*****************************************************************************/
/*	RTSController implementation
/*****************************************************************************/
RTSController::RTSController()
{
	velocity.set( 0.0f, 0.0f, 0.0f );
	damping			= 0.94f;
	angDamping		= 0.94f;
	maxVelocity 	= 1.8f;
	viewAngle		= c_PI / 6.0f;

	curZoom			= 0.0f;
	maxZoom			= 10.0f;
	minZoom			= -10.0f;

	zoomStep		= 0.1f;
	angVelocity		= 0.0f;
	maxAngVelocity	= 0.03f;

	lastTime		= 0.0f;
}

RTSController::~RTSController()
{
}

bool RTSController::OnMouseWheel( int delta )
{
	velocity.z += float( delta ) / float( WHEEL_DELTA ) * maxVelocity;
	return false;
}

bool RTSController::OnMouseMove( int mX, int mY, DWORD keys )
{
	static int MX = -1;
	if (GetKeyState( VK_RBUTTON ) < 0)
	{
		if (MX == -1) MX = mX;
		float amt = float( MX - mX ) * 0.002f;
		angVelocity -= amt;
		if (angVelocity > maxAngVelocity) angVelocity = maxAngVelocity;
		if (angVelocity < -maxAngVelocity) angVelocity = -maxAngVelocity;
	}
	MX = mX;
	return false;
} // RTSController::OnMouseMove

void RTSController::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "RTSController", this );
	pm.f( "Damping",	damping			);
	pm.f( "AngDamping", angDamping		);
	pm.f( "MaxV",		maxVelocity		);
	pm.f( "MaxAngV",	maxAngVelocity	);
}

bool RTSController::OnMouseLButtonDown( int mX, int mY )
{
	return false;
}

bool RTSController::OnMouseRButtonDown( int mX, int mY )
{
	return false;
}

bool RTSController::OnMouseLButtonUp( int mX, int mY )
{
	return false;
}

bool RTSController::OnMouseRButtonUp( int mX, int mY )
{
	return false;
}

void RTSController::OnUpdate()
{
	POINT pt;
	GetCursorPos( &pt );
	float mX = pt.x;
	float mY = pt.y;

	Rct vp = IRS->GetViewPort();

	if (mX > vp.x + vp.w - 3) velocity.x = maxVelocity; 
	else if (mX < vp.x + 3)	velocity.x = -maxVelocity; 
	
	if (mY < vp.y + 3) velocity.y = -maxVelocity; 
	else if (mY > vp.y + vp.h - 3) velocity.y = maxVelocity; 
	
	//  clamp velocity
	/*if (velocity.x >  maxVelocity) velocity.x =  maxVelocity;
	if (velocity.x < -maxVelocity) velocity.x = -maxVelocity;
	if (velocity.z >  maxVelocity) velocity.y =  maxVelocity;
	if (velocity.z < -maxVelocity) velocity.y = -maxVelocity;*/

	
	float cTime = float( GetTickCount() );
	if (lastTime == 0.0f) lastTime = cTime;
	float dTime = (cTime - lastTime);
	if (dTime > 100.0f) dTime = 100.0f;
	lastTime = cTime;

	velocity	*= damping;
	angVelocity *= angDamping;

	Vector3D ds( velocity );
	ds *= dTime;
	float dPhi = angVelocity * angDamping;

	Matrix4D rot;
	rot.rotation( Vector3D::oZ, dPhi );

	for (int i = 0; i < GetNChildren(); i++)
	{
		sg::TransformNode* pNode = (sg::TransformNode*)GetChild( i );
		assert( pNode->IsA<TransformNode>() );
		Matrix4D tm = pNode->GetTransform();
	
		Vector3D vR = tm.getV0();
		vR.z = 0.0f;
		vR.normalize();
		Vector3D vF;
		vF.cross( vR, Vector3D::oZ );
		vF.normalize();

		Vector3D tr( ds );
		tr.addWeighted( vF, vR, -ds.y, ds.x );
		
		//  translation
		Matrix4D trans;
		trans.translation( tr );

		Vector3D lookAt;
		Plane::xOy.intersect( Line3D( tm.getTranslation(), tm.getV2() ), lookAt );
		Matrix4D ftr;
		ftr.translation( lookAt );
		Matrix4D btr;
		lookAt.reverse();
		btr.translation( lookAt );
		tm *= btr;
		tm *= rot;
		tm *= ftr;
		tm *= trans;
		pNode->SetTransform( tm );
	}
} // RTSController::OnUpdate

bool RTSController::OnKeyDown( DWORD keyCode, DWORD flags )
{
	return false;	
} // RTSController::OnKeyDown

void RTSController::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << damping << angDamping	<< maxVelocity;
	os << viewAngle	<< maxZoom << minZoom << zoomStep << maxAngVelocity;
} // RTSController::Serialize

void RTSController::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> damping >> angDamping	>> maxVelocity;
	is >> viewAngle	>> maxZoom >> minZoom >> zoomStep >> maxAngVelocity;
} // RTSController::Unserialize


/*****************************************************************************/
/*	MayaController implementation
/*****************************************************************************/
MayaController::MayaController()
{
	m_Mx				= -1;
	m_My				= -1;
	m_Mode				= cmIdle;
	m_pCursorSwitch		= NULL;

	m_bLockRotate		= false;
	m_bLockPan			= false;
	m_bLockZoom			= false;
}

MayaController::~MayaController()
{
}

Vector3D MayaController::ScreenToVector( float x, float y )
{
	Rct vp = IRS->GetViewPort();
	Vector3D res; res.zero();
	const float c_RotateRadius = 5.0f;

	float halfW = vp.w*0.5f;
	float halfH = vp.h*0.5f;
	res.x = -(x - vp.x - halfW)/(c_RotateRadius * halfW);
	res.y = -(y - vp.y - halfH)/(c_RotateRadius * halfH);
	res.z = 0.0f;

	res.z   = 0.0f;
	float mag = res.norm2();

	if (mag > 1.0f)
	{
		float scale = 1.0f / sqrt( mag );
		res.x *= scale;
		res.y *= scale;
	}
	else res.z = sqrt( 1.0f - mag );

	return res;
} // MayaController::ScreenToVector

bool MayaController::OnMouseMove( int mX, int mY, DWORD keys )
{	
	if (!(keys & MK_MBUTTON) &&
		!(keys & MK_LBUTTON) &&
		!(keys & MK_RBUTTON))
	{
		m_Mode = cmIdle;
	}

	float x1 = m_Mx;
	float y1 = m_My;
	float x2 = mX;
	float y2 = mY;

	Rct vp = IRS->GetViewPort();
	BaseCamera::ScreenToProjectionSpace( x1, y1, &vp );
	BaseCamera::ScreenToProjectionSpace( x2, y2, &vp );
	
	for (int i = 0; i < GetNChildren(); i++)
	{
		Matrix4D deltaTM( Matrix4D::identity );

		sg::BaseCamera* pCamera = (sg::BaseCamera*)GetChild( i );
		if (!pCamera->IsA<BaseCamera>()) continue;
		
		Vector4D worldCenter( 0.0f, 0.0f, 0.0f, 1.0f );
		pCamera->WorldToProjectionSpace( worldCenter );

		if (m_Mode == cmRotate && !m_bLockRotate)
		{
			Vector4D center( 0.0f, 0.0f, worldCenter.z, 1.0f );
			pCamera->ProjectionToWorldSpace( center );

			//  vertical mouse movement pitches camera around axis, parallel
			//  to the camera right axis, and going through point of interest
			Matrix4D pitchTM;
			Line3D pitchAxis( center, pCamera->GetRight() );

			float pitchAng = asin( (y2 - y1) * 2.0f );
			pitchTM.rotation( pitchAxis, pitchAng );

			deltaTM *= pitchTM;

			//  horizontal mouse movement rotates camera around axis, parallel
			//  to the world oZ axis, and going through point of interest
			Matrix4D rollTM;
			float rollAng = asin( (x1 - x2)  *2.0f );

			Line3D rollAxis( center, Vector3D::oZ );
			rollTM.rotation( rollAxis, rollAng );

			deltaTM *= rollTM;
		}
		else if (m_Mode == cmPan && !m_bLockPan)
		{
			//  panning maps screen space mouse movement to the camera movement in the 
			//  plane, parallel to the clipping planes
			Vector4D w1( x1, y1, worldCenter.z, 1.0f );
			Vector4D w2( x2, y2, worldCenter.z, 1.0f );
			pCamera->ProjectionToWorldSpace( w1 );
			pCamera->ProjectionToWorldSpace( w2 );
			w1 -= w2;
			Matrix4D panTM;
			panTM.translation( w1 );
			deltaTM *= panTM;
		}
		else if (m_Mode == cmZoom && !m_bLockZoom)
		{
			//  zooming moves camera forward/backward for perspective camera
			//  and changes view volume width for ortho camera
			float dz = x1 - x2;
			if (fabs( y2 - y1 ) > fabs( dz )) dz = y2 - y1;	
			Vector4D w1( 0.0f, 0.0f, 0.7f - dz, 1.0f );
			Vector4D w2( 0.0f, 0.0f, 0.7f + dz, 1.0f );
			pCamera->ProjectionToWorldSpace( w1 );
			pCamera->ProjectionToWorldSpace( w2 );
			w1 -= w2;

			Matrix4D zoomTM( Matrix4D::identity );
			if (pCamera->HasFn( PerspCamera::Magic() ))
			{
				zoomTM.translation( w1 );
			}
			else if (pCamera->HasFn( OrthoCamera::Magic() ))
			{
				OrthoCamera* pOrtho = (OrthoCamera*)pCamera;
				float w = pOrtho->GetViewVolW();
				w += w1.z;
				if (w > 0.0f) pOrtho->SetViewVolW( w );
			}

			deltaTM *= zoomTM;
		}

		Matrix4D tm = pCamera->GetTransform();
		tm *= deltaTM;
		pCamera->SetTransform( tm );
		m_TotalTM *= deltaTM;
	}

	m_Mx = mX;
	m_My = mY;

	return false;
} // MayaController::OnMouseMove

bool MayaController::OnMouseMButtonDown( int mX, int mY )
{
	if (GetKeyState( VK_MENU ) < 0)
	{
		m_Mode = cmPan;
	}
	return false;
}

bool MayaController::OnMouseLButtonDown( int mX, int mY )
{
	if (GetKeyState( VK_MENU ) < 0)
	{
		m_Mode = cmRotate;
	}
	return false;
} // MayaController::OnMouseLButtonDown

bool MayaController::OnMouseRButtonDown( int mX, int mY )
{
	if (GetKeyState( VK_MENU ) < 0)
	{
		m_Mode = cmZoom;
	}
	return false;
}

bool MayaController::OnMouseLButtonUp( int mX, int mY )
{
	m_Mode = cmIdle;
	return false;
}

bool MayaController::OnMouseRButtonUp( int mX, int mY )
{
	m_Mode = cmIdle;
	return false;
}

bool MayaController::OnMouseMButtonUp( int mX, int mY )
{
	m_Mode = cmIdle;
	return false;
}

void MayaController::OnActivate( bool bActive )
{
	if (bActive) 
	{
		POINT pt;
		GetCursorPos( &pt );
		m_Mx = pt.x;
		m_My = pt.y;
	}
} // MayaController::OnActivate

void MayaController::CreateCursorSwitch()
{
	m_pCursorSwitch = FindChild<Switch>( "MayaCtlCursorSet" );
	if (!m_pCursorSwitch)
	{
		m_pCursorSwitch = AddChild<Switch>( "MayaCtlCursorSet" );
		SystemCursor* pIdle		= m_pCursorSwitch->AddChild<SystemCursor>( "Idle"	);
		SystemCursor* pRotate	= m_pCursorSwitch->AddChild<SystemCursor>( "Rotate"	);
		SystemCursor* pPan		= m_pCursorSwitch->AddChild<SystemCursor>( "Pan"	);
		SystemCursor* pZoom		= m_pCursorSwitch->AddChild<SystemCursor>( "Zoom"	);

		pRotate->SetFileName( "Cursors\\rotate.cur" );
		pPan->SetFileName	( "Cursors\\pan.cur"	);
		pZoom->SetFileName	( "Cursors\\zoom.cur"	);

		pRotate->SetInvisible();
		pPan->SetInvisible();
		pZoom->SetInvisible();
	}
} // MayaController::CreateCursorSwitch

void MayaController::OnUpdate()
{
	if (!m_pCursorSwitch)
	{
		CreateCursorSwitch();
	}
	m_pCursorSwitch->SwitchTo( (int)m_Mode );
} // MayaController::OnUpdate

bool MayaController::OnKeyDown( DWORD keyCode, DWORD flags )
{
	return false;	
} // MayaController::OnKeyDown

void MayaController::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // MayaController::Serialize

void MayaController::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
} // MayaController::Unserialize

void MayaController::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "MayaController", this );
	pm.f( "LockRotate",		m_bLockRotate	);
	pm.f( "LockPan",		m_bLockPan		);
	pm.f( "LockZoom",		m_bLockZoom		);
} // MayaController::Expose

END_NAMESPACE(sg)

