/*****************************************************************************/
/*	File:	mArcBall.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	04.02.2003
/*****************************************************************************/
#include "stdafx.h"
#include "mArcBall.h"

/*****************************************************************************/
/*	ArcBall implementation
/*****************************************************************************/
ArcBall::ArcBall() : camera(NULL), begX(-1), begY(-1), drag(false) 
{
	rotateRadius = 1.0f;
}

ArcBall::ArcBall( Camera* pCam ) : begX(-1), begY(-1), drag(false) 
{ 
	rotateRadius = 1.0f;
	AttachCamera( pCam ); 
}


LRESULT	ArcBall::HandleMouseMessages(	HWND	hWnd, 
										UINT	uMsg, 
										WPARAM	wParam,
                                        LPARAM	lParam )
{
	int mx = LOWORD(lParam);
    int my = HIWORD(lParam);

    switch( uMsg )
    {
        case WM_RBUTTONDOWN:
			OnRButtonDown( wParam, mx, my );
			return TRUE;
		case WM_LBUTTONDOWN:
            OnLButtonDown( wParam, mx, my );
			return TRUE;

		case WM_RBUTTONUP:
            OnRButtonUp( wParam, mx, my );
			return TRUE;

		case WM_LBUTTONUP:
            OnLButtonUp( wParam, mx, my );
			return TRUE;

        case WM_MOUSEWHEEL:
            OnMouseWheel( wParam, (short)HIWORD( wParam ) );
			return TRUE;

        case WM_MOUSEMOVE:
            OnMouseMove( wParam, mx, my );
			return TRUE;
    }

    return FALSE;
} // ArcBall::HandleMouseMessages

void ArcBall::OnMouseMove( DWORD flags, int mx, int my )
{
	if (!drag) return;
	if ((flags & MK_LBUTTON) && (flags & MK_RBUTTON))
	//  zoom
	{
		camera->Zoom( (float(begY - my)) * getZoomAmount() * 0.25f );
	}
	else if (flags & MK_LBUTTON)
	//  rotate
	{
		Quaternion quat;
		
		//  find begin/end vectors on the virtual sphere
		Vector3D begSpherePt = ScreenToVector( begX, begY );
		Vector3D endSpherePt = ScreenToVector( mx, my );
		
		//  transform vectors to the world space (from camera space)
		Matrix3D viewRot;
		viewRot.copy( camera->getViewM() );
		viewRot.inverse();

		begSpherePt *= viewRot;
		endSpherePt *= viewRot;

		//  find quaternion which represents this rotation (in world space)
		quat.axisToAxis( begSpherePt, endSpherePt );
		Matrix4D rot;
		rot.rotation( quat );
		
		Vector4D nd( camera->getDir() );
		Vector4D nu( camera->getUp()  );
		Vector4D np( camera->getPos() );

		nd.mul( rot );
		nu.mul( rot );
		np.mul( rot );

		camera->setDir	( nd );
		camera->setUp	( nu );
		camera->setPos	( np );
	}
	else if (flags & MK_RBUTTON)
	//  move in local xOy plane
	{
		Vector3D mdir( begX - mx, my - begY, 0.0f );
		camera->MovePosLocal( mdir, getTranslateAmount() );
	}

	begX = mx;
	begY = my;
} // ArcBall::OnMouseMove
 
void ArcBall::OnMouseWheel( DWORD flags, int amount	)
{
	camera->Zoom( (float(amount / WHEEL_DELTA)) * getZoomAmount() );
} // ArcBall::OnMouseWheel

void ArcBall::OnLButtonDown( DWORD flags, int mx, int my )
{
	drag = true;
	begX = mx;
	begY = my;
} // ArcBall::OnLButtonDown

void ArcBall::OnLButtonUp( DWORD flags, int mx, int my )
{
	drag = false;
} // ArcBall::OnLButtonUp

void ArcBall::OnRButtonDown( DWORD flags, int mx, int my )
{
	drag = true;
	begX = mx;
	begY = my;
} // ArcBall::OnRButtonDown

void ArcBall::OnRButtonUp( DWORD flags, int mx, int my )
{
	drag = false;
} // ArcBall::OnRButtonUp

float ArcBall::getZoomAmount()
{
	return 0.8f;
} // ArcBall::getZoomAmount

float ArcBall::getTranslateAmount()
{	
	if (camera->isOrtho())
	{
		return  camera->getViewVolumeWidth() / camera->getScrW();
	}
	else
	{
		return (camera->getZf() * tan( camera->getHFOV() * 0.5f ) + 
							camera->getViewVolumeWidth())/ camera->getScrW();
	}
} // ArcBall::getTranslateAmount

Vector3D ArcBall::ScreenToVector( float x, float y )
{
	assert( camera );
	Vector3D res;
	float halfW = camera->getScrW()*0.5f;
	float halfH = camera->getScrH()*0.5f;
    res.x = (x - halfW)/(rotateRadius * halfW);
    res.y = (halfH - y)/(rotateRadius * halfH);
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
} // ArcBall::ScreenToVector


