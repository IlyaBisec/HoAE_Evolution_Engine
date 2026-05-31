/*****************************************************************************/
/*	File:	sgCamera.h
/*	Desc:	Camera-related routines
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-21-2003
/*****************************************************************************/
#ifndef __SG_CAMERA_H__
#define __SG_CAMERA_H__

#include "mMath2D.h"
#include "IMediaManager.h"

namespace sg{

const float c_DefViewAspect = 4.0f / 3.0f;

class AppWindow;

/*****************************************************************************/
/*	Class:	Canvas
/*	Desc:	Rendering view port
/*****************************************************************************/
class Canvas : public Node
{
	Rct							m_Extents;
	DWORD						m_Flags;
	DWORD						m_Color;
	
public:
								Canvas			();
	virtual void				Serialize		( OutStream&	os ) const;
	virtual void				Unserialize		( InStream&		is );
	virtual void				Expose			( PropertyMap&	pm );
	virtual void				Render			();

	//  node flags operations
	enum ViewportFlags
	{
		vfClearColor	= 0x0001,
		vfClearDepth	= 0x0002,
		vfClearStencil	= 0x0004,
		vfAutoScale		= 0x0008
	}; // enum NodeFlags

	bool					GetViewportFlag	( ViewportFlags flag ) const { return (m_Flags & flag) != 0; }
	void					SetViewportFlag	( ViewportFlags flag, bool state = true ) { if (state == true) m_Flags |= flag; else m_Flags &= ~flag; }

	void					SetBgColor		( DWORD color ) { m_Color = color; }
	DWORD					GetBgColor		() const		{ return m_Color; }

	bool					GetClearColor	() const { return GetViewportFlag( vfClearColor	  );	}
	bool					GetClearDepth	() const { return GetViewportFlag( vfClearDepth	  );	}
	bool					GetClearStencil	() const { return GetViewportFlag( vfClearStencil );	}
	bool					GetAutoScale	() const { return GetViewportFlag( vfAutoScale );		}

	void					SetClearColor	( bool val = true ) { SetViewportFlag( vfClearColor, val );	}
	void					SetClearDepth	( bool val = true ) { SetViewportFlag( vfClearDepth, val );	}
	void					SetClearStencil	( bool val = true ) { SetViewportFlag( vfClearStencil, val );	}
	void					SetAutoScale	( bool val = true ) { SetViewportFlag( vfAutoScale, val );		}

	void					SetExtents		( float x, float y, float w, float h ) { m_Extents.Set( x, y, w, h ); }
	void					SetExtents		( const Rct& rct ) { m_Extents = rct; }
	const Rct&				GetExtents		() const { return m_Extents; }

	_inl float				GetX			() const;
	_inl float				GetY			() const;
	_inl float				GetW			() const;
	_inl float				GetH			() const;

	_inl void				SetX			( float x );
	_inl void				SetY			( float y );
	_inl void				SetW			( float w );
	_inl void				SetH			( float h );
	
	NODE(Canvas,Node,VPRT);
}; // class Canvas

/*****************************************************************************/
/*	Class:	BaseCamera
/*	Desc:	Basic class for camera object
/*****************************************************************************/
class BaseCamera : public TransformNode, public ICamera
{
public:
						BaseCamera				();
	virtual				~BaseCamera				();

	virtual void		Reset					();
	bool				SetDirUp				( const Vector3D& dir,	const Vector3D& up		);
	bool				SetDir					( const Vector3D& dir ) { return SetDirUp( dir, GetUp() ); }
	bool				SetDirRight				( const Vector3D& dir,	const Vector3D& right	);
	bool				SetUpRight				( const Vector3D& up,	const Vector3D& right	);
	
	virtual _inl void	SetPos					( const Vector3D& pos );
	_inl void			SetViewM				( const Matrix4D& viewM );
	_inl void			SetProjM				( const Matrix4D& projM );
	_inl void			SetPos					( float x, float y, float z );

	virtual _inl float	GetZn					() const { return 0.0f; }
	virtual _inl float	GetZf					() const { return 0.0f; }
	virtual _inl void	SetZn					( float _Zn ){}
	virtual _inl void	SetZf					( float _Zf ){}
	virtual _inl void	SetZnf					( float _Zn, float _Zf ){}
	virtual _inl float	GetWToH					() const { return 0.0f; }
	virtual _inl void	SetWToH					( float _aspect ){}
    virtual void        SetFOV                  ( float fov ){}
    virtual float       GetFOV                  () const { return 0.0f; }

	virtual Matrix4D	GetWorldMatrix			() const { return tm; };

	virtual void		TweakProjM				( const Rct& rct ){}
	_inl void			SetTargetNode			( TransformNode* pNode );
	_inl bool			SetRightUpDir			(	const Vector3D& right, 
													const Vector3D& up, 
													const Vector3D& dir );
	void				SetYawPitchRoll			( float yaw, float pitch, float roll );
	
	_inl void			GetPos					( Vector3D& pos		) const;
	_inl void			GetDir					( Vector3D& dir		) const;
	_inl void			GetUp					( Vector3D& up		) const;
	_inl void			GetRight				( Vector3D& right	) const;

	_inl Vector3D		GetPos					() const;
	_inl Vector3D		GetDir					() const;
	_inl Vector3D		GetUp					() const;
	_inl Vector3D		GetRight				() const;
	_inl Vector3D		GetCenter				( float ratio = 0.5f ) const;

    virtual Vector3D	GetLookAt			    () const;
    virtual void	    SetLookAt			    ( const Vector3D& v );

	_inl void			ZBiasIncrease			( float ratio = 0.0005f );
	_inl void			ZBiasDecrease			( float ratio = 0.0005f );
	_inl Ray3D			GetViewRay				() const { return Ray3D( GetPos(), GetDir() ); }
	void				GetPickRay				( float curX, float curY, Line3D& ray, 
													const Matrix4D* worldMatr = NULL );
	void				GetPickFrustum			( float bX, float bY, float eX, float eY, Frustum& frustum );

	virtual void		Interpolate				( float t, const Vector3D& sLookAt, const Vector3D& sDir,
															const Vector3D& dLookAt, const Vector3D& dDir );
	virtual void		Interpolate				( float t, const Vector3D& sLookAt, const Vector3D& dLookAt );

	_inl void			ProjectionToCameraSpace	( Vector4D& pos ) const;
	_inl void			CameraToProjectionSpace	( Vector4D& pos ) const;

	_inl Matrix4D		CameraToProjectionSpace	() const;

	_inl void			ProjectionToWorldSpace	( Vector4D& pos ) const;

	_inl void			WorldToCameraSpace		( Vector4D& pos ) const;
	virtual _inl void	WorldToProjectionSpace	( Vector4D& pos ) const;

	_inl void			CameraToWorldSpace		( Vector4D& pos ) const;

	static _inl void	ScreenToProjectionSpace	( float& x, float& y, const Rct* pViewport = NULL );
	static _inl void	ProjectionToScreenSpace	( float& x, float& y, const Rct* pViewport = NULL );

	_inl void			ScreenToCameraSpace		( Vector4D& pos, const Rct* pViewport = NULL ) const;
	_inl void			ScreenToWorldSpace		( Vector4D& pos, const Rct* pViewport = NULL ) const;
	virtual _inl void	WorldToScreenSpace		( Vector4D& pos, const Rct* pViewport = NULL ) const;
    virtual _inl void	WorldToScreenSpace		( Vector3D& pos ) const;
	
	_inl void			GetViewM				( Matrix4D& m ) const;
	_inl void			GetProjM				( Matrix4D&	m ) const;
	_inl Matrix4D		GetViewM				()				const;
	_inl Matrix4D		GetProjM				()				const;
	virtual _inl Matrix4D GetViewProjM			()				const;

	_inl void			GetInvViewM				( Matrix4D& m ) const;
	_inl void			GetInvProjM				( Matrix4D& m ) const;

	void				GetCameraSpaceFrustum	( Frustum& frustum ) const;
	void				GetWorldSpaceFrustum	( Frustum& frustum ) const;
	void				GetObjectSpaceFrustum	( Frustum& frustum, const Matrix4D& objTM ) const;

	Matrix4D			WorldToScreenSpace		( const Rct* pViewport = NULL ) const;
	Matrix4D			ProjectionToScreenSpace	( const Rct* pViewport = NULL ) const;
    virtual Matrix4D    ScreenToWorldSpace      ( const Rct* pViewport = NULL ) const;
	
	_inl Matrix4D		WorldToProjectionSpace	() const;
	void				AdjustScreenSpaceSize	( Matrix4D& m, float size );


	void				FaceToPt				( const Vector3D& pt, const Vector3D& rotAxis );
	void				FaceToPt				( const Vector3D& pt );
	void				FaceToPt				( const Vector3D& pt, float distance );

	virtual void		Serialize				( OutStream&	os	 ) const;
	virtual void		Unserialize				( InStream&		is	 );
	virtual void		Expose					( PropertyMap&	pm );
	virtual void		Render					();

	void				Set						();

	static BaseCamera*	GetActiveCamera			() { return NodePool::instance().GetNode<BaseCamera>( s_ActiveCamera ); }

	static void			Freeze					() { s_bFrozen = true; }
	static void			Unfreeze				() { s_bFrozen = false; }
	static void			HideCameras				( Node* pNode, bool hide = true ); 

    Matrix4D            GetCameraTM             () const { return GetTransform(); }

	Frustum				GetFrustum				() const;
	virtual void		GetFrustum				( Frustum& fr ) const;
	void				Pan						( const Vector3D& lookAt );

	virtual void		SetProjection			( float volW, float aspect, float zn, float zf ) {}
	virtual void		ShiftZ					( float amount ) {};

	void				SetTweakAspect			( bool val = true ) { m_bTweakAspect = val; }

	NODE(BaseCamera,TransformNode,BCAM);
protected:
	Matrix4D			m_Proj;
	bool				m_bTweakAspect;

	static DWORD		s_ActiveCamera;
	static bool			s_bFrozen;
}; // class BaseCamera

/*****************************************************************************/
/*	Class:	OrthoCamera
/*	Desc:	Orthogonal camera
/*****************************************************************************/
class OrthoCamera : public BaseCamera
{
public:
							OrthoCamera 		();
	_inl float				GetW				() const;
	_inl float				GetH				() const;

	virtual _inl float		GetZn				() const;
	virtual _inl float		GetZf				() const;
	virtual _inl void		SetZn				( float _Zn );
	virtual _inl void		SetZf				( float _Zf );
	virtual _inl void		SetZnf				( float _Zn, float _Zf );
	virtual _inl float		GetWToH				() const;
	virtual _inl void		SetWToH				( float _aspect );

	_inl float				GetViewVolW			() const;
	_inl void				SetViewVolW			( float _vVol );
	virtual void			GetFrustum			( Frustum& fr ) const;


	_inl void				SetOrthoW			( float viewVolW, float wToH = c_DefViewAspect, 
													float zn = 1.0f, float zf = 100.0f );

	virtual void			Expose				( PropertyMap& pm );
	virtual void			TweakProjM			( const Rct& rct );
	virtual void			ShiftZ				( float amount );

	virtual void			SetProjection		( float volW, float aspect, 
													float zn, float zf );


	NODE(OrthoCamera,BaseCamera,OCAM);

}; // class OrthoCamera

/*****************************************************************************/
/*	Class:	PerspCamera
/*	Desc:	Left-handed perspective camera
/*****************************************************************************/
class PerspCamera : public BaseCamera
{
public:
							PerspCamera ();
	void					SetPerspFOVx( float fovx, float wToH = c_DefViewAspect, 
											float zn = 1.0f, float zf = 100.0f );
	void					SetPerspFOVy( float fovy, float wToH = c_DefViewAspect, 
											float zn = 1.0f, float zf = 100.0f );
	void					SetPerspWH	( float w, float h, float zn = 1.0f, float zf = 100.0f );
	void					SetPerspW	( float w, float wtoh, float zn = 1.0f, float zf = 100.0f );

	virtual _inl float		GetZn		() const;
	virtual _inl float		GetZf		() const;
    virtual float           GetFOV      () const { return GetFOVx(); }
    virtual void            SetFOV      ( float fov );

	virtual _inl void		SetZn		( float _Zn );
	virtual _inl void		SetZf		( float _Zf );
	virtual _inl void		SetZnf		( float _Zn, float _Zf );
	virtual _inl float		GetWToH		() const;
	virtual _inl void		SetWToH		( float _aspect );

	virtual void			TweakProjM	( const Rct& rct );

	_inl float				GetFOVy		() const;
	_inl float				GetFOVx		() const;
	_inl float				GetFOVyDeg	() const;
	_inl float				GetFOVxDeg	() const;

	_inl void				SetFOVy		( float _fovY  );
	_inl void				SetFOVx		( float _fovX  );
	_inl void				SetFOVyDeg	( float _fovYdeg );
	_inl void				SetFOVxDeg	( float _fovXdeg );

	_inl float				GetNearW	() const;
	_inl float				GetNearH	() const;
	_inl float				GetFarW		() const;
	_inl float				GetFarH		() const;

	virtual void			Expose		( PropertyMap& pm );
	virtual void			ShiftZ		( float amount );
	virtual void			SetProjection( float volW, float aspect, 
											float zn, float zf );

	NODE(PerspCamera,BaseCamera,PCAM);
}; // class PerspCamera 

} // namespace sg

#ifdef _INLINES
#include "sgCamera.inl"
#endif // _INLINES

#endif // __SG_CAMERA_H__