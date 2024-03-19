/*****************************************************************************/
/*    File:    ICamera.h
/*    Desc:    Camera access interface
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-15-2003
/*****************************************************************************/
#ifndef __ICAMERA_H__
#define __ICAMERA_H__

enum CameraSpace
{
    sWorld      = 0,
    sView       = 1,
    sProjection = 2,
    sScreen     = 3
}; // enum CameraSpace

const float c_DefViewAspect = 4.0f / 3.0f;

/*****************************************************************************/
/*    Class:    ICamera
/*    Desc:    Interface for cameras manipulation
/*****************************************************************************/
class ICamera
{
public:
    //  current camera position
    virtual void            SetPosition         ( const Vector3D& pos ) = 0;
    virtual Vector3D        GetPosition         () const = 0;

    virtual void            SetDirection        ( const Vector3D& pos ) = 0;
    virtual Vector3D        GetDirection        () const = 0;

    virtual void            SetLookAt           ( const Vector3D& pos ) = 0;
    virtual Vector3D        GetLookAt           () const = 0;

    //  current camera orientation

    virtual const Matrix4D& GetWorldTM          () const = 0;
    virtual void            SetWorldTM          ( const Matrix4D& m ) = 0;

    virtual const Matrix4D& GetViewTM           () const = 0;
    virtual void            SetViewTM           ( const Matrix4D& m ) = 0;

    virtual const Matrix4D& GetProjTM           () const = 0;
    virtual void            SetProjTM           ( const Matrix4D& m ) = 0;

    virtual const Rct&      GetViewport         () const = 0;
    virtual void            SetViewport         ( const Rct& rct ) = 0;

    virtual float           GetAspect           () const = 0;   

    virtual float           GetViewVolume       () const = 0;
    virtual void            SetViewVolume       ( float w, float zn, float zf, float aspect = c_DefViewAspect ) = 0;
    virtual void            SetOrientation      ( const Vector3D& right, const Vector3D& up, const Vector3D& dir ) = 0;


    //  applies z-bias
    virtual void            ShiftZ              ( float amount ) = 0;

    virtual Vector3D        GetDir              () const = 0;
    virtual Vector3D        GetUp               () const = 0;
    virtual Vector3D        GetRight            () const = 0;

    //  near/far clip plane
    virtual float            GetZn              () const = 0;
    virtual float            GetZf              () const = 0;

    //  set camera as current for render device
    virtual void            Render              () = 0;

    //  camera's view volume frustum
    virtual const Frustum&  GetFrustum          () const = 0;
    
    virtual void            ToSpace             ( CameraSpace src, CameraSpace dest, Vector4D& v ) = 0;
    virtual const Matrix4D& ToSpace             ( CameraSpace src, CameraSpace dest ) = 0;

    virtual Line3D          GetPickRay          ( float curX, float curY ) = 0;
}; // class ICamera

ICamera* GetCamera();
void SetCamera( ICamera* pCamera );

Matrix4D GetCameraTM();
Matrix4D GetCameraProjTM();

#endif // __ICAMERA_H__