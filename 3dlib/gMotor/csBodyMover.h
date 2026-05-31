/*****************************************************************************/
/*	File:	edBodyMover.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-13-2003
/*****************************************************************************/
#ifndef __EDBODYMOVER_H__
#define __EDBODYMOVER_H__
#include "IInput.h"

/*****************************************************************************/
/*	Class:	BodyMover
/*	Desc:	Controller for setting position & orientation of the object in 3D
/*****************************************************************************/
class BodyMover : public Controller, public IInputDispatcher
{
	bool			bDrawHeightMarker;
	bool			bRiseRotateMode;
	bool			bMoveXYMode;
	bool			bChangedPosition;
	bool			bScaleMode;

	float			markerBase;

	Line3D			pickRay;

public:
	BodyMover();
	virtual			~BodyMover();

	//  input handling
	virtual bool 	OnMouseWheel		( int delta );					
	virtual bool 	OnMouseMove			( int mX, int mY, DWORD keys );	
	virtual bool 	OnMouseLBDown	( int mX, int mY );	
	virtual bool 	OnMouseRBDown	( int mX, int mY );	
	virtual bool 	OnMouseLBDbl( int mX, int mY );				
	virtual bool	OnKeyDown			( DWORD keyCode, DWORD flags );	

	virtual void	OnDraw				();

	void			SetChangedPosition( bool changed = true );

	DECLARE_SCLASS(BodyMover,Controller,BODM);

protected:
	void			DrawHeightMarker( const Matrix4D& tr );
	void			OnMoveNode( int mX, int mY, const Line3D* ray = NULL );

}; // class BodyMover

/*****************************************************************************/
/*	Class:	HudMover
/*	Desc:	Controller for setting positions of screen-space objects
/*****************************************************************************/
class HudMover : public Controller, public IInputDispatcher
{
	bool			bDragging;
	bool			bScaling;

public:
					HudMover();


	virtual bool 	OnMouseWheel		( int delta );					
	virtual bool 	OnMouseMove			( int mX, int mY, DWORD keys );	
	virtual bool 	OnMouseLBDown	( int mX, int mY );	
	virtual bool 	OnMouseLBUp	( int mX, int mY );	
	virtual bool 	OnMouseRBDown	( int mX, int mY );	
	virtual bool 	OnMouseLBDbl( int mX, int mY );				
	virtual bool	OnKeyDown			( DWORD keyCode, DWORD flags );	

	virtual void	OnDraw				();

	DECLARE_SCLASS(HudMover,Controller,HUDM);
}; // class HudMover


#endif // __EDBODYMOVER_H__