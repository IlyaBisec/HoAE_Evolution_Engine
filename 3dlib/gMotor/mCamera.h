/*****************************************************************/
/*  File:   Camera.h
/*  Desc:   3D world camera
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __CAMERA_H__
#define __CAMERA_H__

/*****************************************************************/
/*  Class:	Camera
/*****************************************************************/
class Camera 
{
public:
								Camera();
								Camera( const char* camName );

	void						reset();

	void						FocusOnAABB( AABoundBox& aabb );
	void						RotateAroundPoint( float ang, const Vector3D& pt );

	void						getPickRay( float curX, float curY, 
											Line3D& ray,
											const Matrix4D* worldMatr = NULL );

	void						MovePosLocal( const Vector3D& mdir, float amount );
	void						MovePos( const Vector3D& mdir, float amount );

	void						Zoom( float scale );

    _inl void					ProjectionToScreenSpace( float& x, float& y ) const;
	_inl void					ScreenToProjectionSpace( float& x, float& y ) const;
	
	_inl void					WorldToScreenSpace( Vector4D& pos ) const;
	_inl void					ScreenToWorldSpace( Vector4D& pos ) const;

	_inl void					ZBiasIncrease( float ratio = 0.0005f );
	_inl void					ZBiasDecrease( float ratio = 0.0005f );

	//  getters
	_inl const Matrix4D&		getViewM()			const;
	_inl const Matrix4D&		getProjM()	        const;
    _inl const Matrix4D&		getFullM()	        const;

	_inl const Frustum&			getFrustum()		const;

	_inl float					getVFOV()			const;
	_inl float					getHFOV()			const;

	_inl const Vector3D&		getPos()				const;
    _inl const Vector3D&		getDir()				const;
	_inl const Vector3D&		getAt()				    const;
    _inl const Vector3D&        getUp()                 const;

	_inl float					getViewVolumeWidth()	const;
	_inl float					getViewVolumeHeight()	const;

	_inl float					getScrW()			const;
	_inl float					getScrH()			const;
	_inl float					getScrArea()		const;

	_inl float					getZn()				const;
	_inl float					getZf()				const;

	_inl void					setViewPort( int left, int top, int width, int height );

	_inl void					setLookAt( float x, float y, float z );
	_inl void					setLookAt( const Vector3D& pt );

    _inl void                   setDir( float x, float y, float z );
    _inl void                   setDir( const Vector3D& _dir );
	_inl void                   setDir( const Vector4D& _dir );

    _inl void                   setViewM( const Matrix4D& matr );

    _inl void                   setHFOV( float _hfov );
	_inl void                   setZn( float _Zn );
    _inl void                   setZf( float _Zf );

    _inl void                   setOrtho( bool isOrtho = true );
	_inl void					setViewVolumeWidth( float vvWidth );
    _inl bool                   isOrtho() const;


    void						setPos( float x, float y, float z );
	void						setPos( const Vector3D& pt );

	void						setUp( float x, float y, float z );
	void						setUp( const Vector3D& _up );


	void						txtSave( FILE* fp );

	void						transform( const Matrix4D& matr ); 
	void						UnviewOnXOY( Vector4D& vec );

private:
	void                        calcViewM();
    void                        calcProjM();

	 //  viewport parameters
    float						scrW, scrH, scrX, scrY;

    //  projection parameters
    float						hFov;		//  horizontal field of view, in radians
    float						Zn;			//  Z value of near frustum clipping plane
	float						Zf;			//  Z value of far frustum clipping plane

	float						viewVolWidth;	//  width of the view volume
												//  (for orthogonal camera only )

    //  camera space parameters
	Vector3D					pos;		//  world camera position
	Vector3D					dir;	    //  looking direction vector
    Vector3D					up;			//  up vector

    //  is orthogonal view
    bool                        bOrtho;

    //  auxiliary parameters
	Vector3D					at;	        //  looking direction vector

	Matrix4D					viewM;	    //  view matrix
	Matrix4D					projM;	    //  projection matrix
	Matrix4D					fullM;	    //  full transform matrix
	Frustum						frustum;	//  view frustum

};  // class Camera

#ifdef _INLINES
#include "mCamera.inl"
#endif //_INLINES

#endif // __CAMERA_H__