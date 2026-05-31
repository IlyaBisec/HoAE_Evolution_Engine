/*****************************************************************************/
/*	File:	csCameraController.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11.07.2003
/*****************************************************************************/
#ifndef __CAMERACONTROLLER_H__
#define __CAMERACONTROLLER_H__

namespace sg{

/*****************************************************************************/
/*	Class:	CameraController
/*	Desc:	Camera movement controller
/*****************************************************************************/
class CameraController : public  Controller, public InputDispatcher
{
protected:
	Matrix4D				m_TotalTM;

	virtual void			OnAttach();

public:
							CameraController();
	virtual					~CameraController();

	void					Home();
	
	virtual void			Serialize( OutStream& os ) const;
	virtual void			Unserialize( InStream& is );

	NODE(CameraController,Controller,CAMC);
}; // class CameraController

/*****************************************************************************/
/*	Class:	FlyController
/*	Desc:	Free flying camera
/*****************************************************************************/
class FlyController : public CameraController
{
public:
					FlyController();
					~FlyController();


	virtual bool 	OnMouseWheel		( int delta );					
	virtual bool 	OnMouseMove			( int mX, int mY, DWORD keys );	
	virtual bool 	OnMouseLButtonDown	( int mX, int mY );				
	virtual bool 	OnMouseRButtonDown	( int mX, int mY );				
	virtual bool 	OnMouseLButtonUp	( int mX, int mY );				
	virtual bool 	OnMouseRButtonUp	( int mX, int mY );
	virtual bool	OnKeyDown			( DWORD keyCode, DWORD flags );
	virtual void	OnUpdate			();
	virtual void	OnActivate			( bool bActive = true );

	virtual void	Serialize			( OutStream& os ) const;
	virtual void	Unserialize			( InStream& is );
	
	float			GetFlySpeed() const		 { return flySpeed; }
	void			SetFlySpeed( float val ) { flySpeed = val;  }


	NODE(FlyController,CameraController,FLYC);

protected:
	Vector3D		ScreenToVector( float x, float y );

private:
	int				mx, my;
	float			flySpeed;

}; // class FlyController

/*****************************************************************************/
/*	Class:	RTSController
/*	Desc:	Real time strategy-style camera controller
/*****************************************************************************/
class RTSController : public CameraController
{
public:
					RTSController();
					~RTSController();

	virtual bool 	OnMouseWheel		( int delta );					
	virtual bool 	OnMouseMove			( int mX, int mY, DWORD keys );	
	virtual bool 	OnMouseLButtonDown	( int mX, int mY );				
	virtual bool 	OnMouseRButtonDown	( int mX, int mY );				
	virtual bool 	OnMouseLButtonUp	( int mX, int mY );				
	virtual bool 	OnMouseRButtonUp	( int mX, int mY );
	virtual bool	OnKeyDown			( DWORD keyCode, DWORD flags );
	virtual void	OnUpdate			();

	virtual void	Serialize			( OutStream& os ) const;
	virtual void	Unserialize			( InStream& is );
	virtual void	Expose				( PropertyMap& pm );


	float			GetSpeedDamping		() const	{ return damping;				}
	float			GetAngSpeedDamping	() const	{ return angDamping;			}
	float			GetMaxVelocity		() const	{ return maxVelocity;			}
	float			GetMaxAngVelocity	() const	{ return maxAngVelocity;		}
	float			GetViewAngleDeg		() const	{ return RadToDeg( viewAngle );	}
	float			GetMaxZoomDistance	() const	{ return maxZoom;				}
	float			GetMinZoomDistance	() const	{ return minZoom;				}

	void			SetSpeedDamping		( float val ) { damping	= val; 				}
	void			SetAngSpeedDamping	( float val ) { angDamping = val;			}
	void			SetMaxVelocity		( float val ) { maxVelocity= val;			}
	void			SetMaxAngVelocity	( float val ) { maxAngVelocity= val;		}
	void			SetViewAngleDeg		( float val ) { viewAngle = DegToRad( val );}
	void			SetMaxZoomDistance	( float val ) { maxZoom = val;				}
	void			SetMinZoomDistance	( float val ) { minZoom = val;				}

	NODE(RTSController,CameraController,RTSC);

private:
	float			damping;
	float			angDamping;
	float			maxVelocity;

	float			viewAngle;

	float			curZoom, maxZoom, minZoom;
	float			zoomStep;

	Vector3D		velocity;
	float			angVelocity;
	float			maxAngVelocity;

	float			lastTime;
}; // class RTSController

enum ControlMode
{
	cmIdle		= 0,
	cmRotate	= 1,
	cmPan		= 2,
	cmZoom		= 3
}; // enum ControlMode

/*****************************************************************************/
/*	Class:	MayaController
/*	Desc:	Mimicks maya camera controller
/*****************************************************************************/
class MayaController : public CameraController
{
public:
	MayaController();
	~MayaController();

	virtual bool 	OnMouseMove			( int mX, int mY, DWORD keys );	
	virtual bool 	OnMouseLButtonDown	( int mX, int mY );				
	virtual bool 	OnMouseRButtonDown	( int mX, int mY );				
	virtual bool 	OnMouseLButtonUp	( int mX, int mY );				
	virtual bool 	OnMouseRButtonUp	( int mX, int mY );
	virtual bool 	OnMouseMButtonDown	( int mX, int mY );				
	virtual bool 	OnMouseMButtonUp	( int mX, int mY );				

	virtual bool	OnKeyDown			( DWORD keyCode, DWORD flags );
	virtual void	OnUpdate			();
	virtual void	OnActivate			( bool bActive = true );

	virtual void	Serialize			( OutStream& os ) const;
	virtual void	Unserialize			( InStream& is );
	virtual void	Expose				( PropertyMap& pm );

	void			LockRotate			( bool lock = true ) { m_bLockRotate = lock; }
	void			LockPan				( bool lock = true ) { m_bLockPan	 = lock; }
	void			LockZoom			( bool lock = true ) { m_bLockZoom	 = lock; }

	NODE(MayaController,CameraController,MAYC);

protected:
	Vector3D		ScreenToVector( float x, float y );
	void			CreateCursorSwitch();

private:
	int						m_Mx, m_My;
	ControlMode				m_Mode;
	Switch*					m_pCursorSwitch;

	bool					m_bLockRotate;
	bool					m_bLockPan;
	bool					m_bLockZoom;

}; // class MayaController

} // namespace sg

#endif //  __CAMERACONTROLLER_H__
