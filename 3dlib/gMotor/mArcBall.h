/*****************************************************************************/
/*	File:	mArcBall.h
/*	Desc:	Translation/rotation/scale UI camera manipulator
/*	Author:	Ruslan Shestopalyuk
/*	Date:	04.02.2003
/*****************************************************************************/
#ifndef __MARCBALL__
#define __MARCBALL__

#pragma once

/*****************************************************************************/
/*	Class:	ArcBall
/*****************************************************************************/
class ArcBall
{
	Camera*			camera;

	int				begX;
	int				begY;
	bool			drag;
	float			rotateRadius;

public:
				ArcBall();
				ArcBall( Camera* pCam );

	void		AttachCamera( Camera* pCam ) { camera = pCam; }
	LRESULT		HandleMouseMessages(	HWND	hWnd, 
										UINT	uMsg, 
										WPARAM	wParam,
                                        LPARAM	lParam );
	
	void		OnMouseMove		( DWORD flags, int mx, int my );
	void		OnMouseWheel	( DWORD flags, int amount	  );

	void		OnLButtonDown	( DWORD flags, int mx, int my );
	void		OnLButtonUp		( DWORD flags, int mx, int my );
	
	void		OnRButtonDown	( DWORD flags, int mx, int my );
	void		OnRButtonUp		( DWORD flags, int mx, int my );

protected:
	float		getZoomAmount();
	float		getTranslateAmount();

	Vector3D	ScreenToVector( float x, float y );

}; // class ArcBall

#endif // __MARCBALL__