/*****************************************************************************/
/*    File:    vCamera.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    
/*****************************************************************************/
#ifndef __VCAMERA_H__
#define __VCAMERA_H__
#include "ICamera.h"
#include "IReflected.h"

#include "sgTransformNode.h"

/*****************************************************************************/
/*    Class:    Camera
/*    Desc:    Basic class for camera object
/*****************************************************************************/
class Camera : public TransformNode, public ICamera 
{
protected:
    std::string         m_Name;             //  camera name
    Matrix4D            m_ProjTM;           //  projection matrix
    Matrix4D            m_WorldTM;          //  camera world matrix
    Rct                 m_ViewPort;         //  camera rendering viewport
    
    //  cached dependent values
    Matrix4D            m_ViewTM;           //  camera view matrix (inverse world matrix)
    
    Matrix4D            m_ScreenTM;         //  projection->screen
    Matrix4D            m_ScreenProjTM;     //  screen->projection

    Matrix4D            m_WorldProjTM;      //  world->projection
    Matrix4D            m_ProjWorldTM;      //  projection->world

    Matrix4D            m_FullTM;           //  world->screen
    Matrix4D            m_InvFullTM;        //  screen->world
    Matrix4D            m_InvProjTM;        //  projection->view

    Matrix4D            m_ViewScreenTM;     //  view->screen
    Matrix4D            m_ScreenViewTM;     //  screen->view

    const Matrix4D*     m_SpaceTM[4][4];


    Frustum             m_Frustum;    
    void                Update();

public:
                        Camera                  ();
    void                SetName                 ( const char* name ) { m_Name = name; }
    const char*         GetName                 () const { return m_Name.c_str(); }
    
    Vector3D            GetPosition             () const                { return Vector3D( m_WorldTM.e30, m_WorldTM.e31, m_WorldTM.e32 ); }
    void                GetOrientation          ( Vector3D& right, Vector3D& up, Vector3D& dir );
    const Matrix4D&     GetViewTM               () const                { return m_ViewTM; }
    const Matrix4D&     GetProjTM               () const                { return m_ProjTM; }
    const Matrix4D&     GetWorldTM              () const                { return m_WorldTM; };
    const Rct&          GetViewport             () const                { return m_ViewPort; }

    Vector3D            GetRight                () const { return Vector3D( m_WorldTM.e00, m_WorldTM.e01, m_WorldTM.e02 ); }
    Vector3D            GetUp                   () const { return Vector3D( m_WorldTM.e10, m_WorldTM.e11, m_WorldTM.e12 ); }
    Vector3D            GetDir                  () const { return Vector3D( m_WorldTM.e20, m_WorldTM.e21, m_WorldTM.e22 ); }

    Line3D              GetPickRay              ( float x, float y );
    Frustum             GetPickFrustum          ( const Rct& rct );
    const Frustum&      GetFrustum              () const { return m_Frustum; }

    void                SetPosition             ( const Vector3D& pos );
    void                SetOrientation          ( const Vector3D& right, const Vector3D& up, const Vector3D& dir );
    void                SetViewTM               ( const Matrix4D& m );
    void                SetProjTM               ( const Matrix4D& m );
    void                SetWorldTM              ( const Matrix4D& m );
    void                SetViewport             ( const Rct& rct );

    virtual void        SetDirection            ( const Vector3D& pos );
    virtual Vector3D    GetDirection            () const;

    virtual void        SetLookAt               ( const Vector3D& pos );
    virtual Vector3D    GetLookAt               () const;

    virtual void        Serialize               ( OutStream& os ) const;
    virtual void        Unserialize             ( InStream&    is );
    virtual void        Expose                  ( PropertyMap&    pm );
    virtual void        Render                  ();
    void                UnitTest                ();

    void                ToSpace                 ( CameraSpace src, CameraSpace dest, Vector4D& v )
    {
        v *= ToSpace( src, dest );
        v.normW();
    } // ToSpace
    const Matrix4D&     ToSpace                 ( CameraSpace src, CameraSpace dest ) { return *m_SpaceTM[src][dest]; }
}; // class Camera

/*****************************************************************************/
/*    Class:    OrthoCamera
/*    Desc:    Orthogonal camera
/*****************************************************************************/
class OrthoCamera : public Camera
{
public:
                        OrthoCamera     ();

    virtual float       GetZn           () const                { return -m_ProjTM.e32/m_ProjTM.e22; }
    virtual float       GetZf           () const                { return (1.0f - m_ProjTM.e32)/m_ProjTM.e22; }
    virtual void        SetZn           ( float zn )            { SetViewVolume( GetViewVolume(), GetAspect(), zn, GetZf() ); }
    virtual void        SetZf           ( float zf )            { SetViewVolume( GetViewVolume(), GetAspect(), GetZn(), zf ); }
    virtual void        SetZnf          ( float zn, float zf )  { SetViewVolume( GetViewVolume(), GetAspect(), zn, zf ); }
    virtual float       GetAspect       () const                { return m_ProjTM.e11/m_ProjTM.e00; }
    virtual void        SetAspect       ( float aspect )        { SetViewVolume( GetViewVolume(), aspect, GetZn(), GetZf() ); }

    float               GetViewVolume   () const                { return 2.0f / m_ProjTM.e00; }
    virtual void        SetViewVolume   ( float w, float zn, float zf, float aspect = c_DefViewAspect )
    {
        OrthoProjectionTM( m_ProjTM, w, aspect, zn, zf ); 
    }
    virtual void        Expose          ( PropertyMap& pm );
    virtual void        ShiftZ          ( float amount );

    DECLARE_SCLASS(OrthoCamera, Camera, NOCM);
}; // class OrthoCamera

/*****************************************************************************/
/*    Class:    PerspCamera
/*    Desc:    Left-handed perspective camera
/*****************************************************************************/
class PerspCamera : public Camera
{
public:
                        PerspCamera     ();
    virtual void        SetViewVolume   ( float w, float zn, float zf, float aspect = c_DefViewAspect );
    float               GetViewVolume   () const;
    
    virtual float       GetZn           () const { return -m_ProjTM.e32/m_ProjTM.e22; }
    virtual float       GetZf           () const { return m_ProjTM.e32/(1.0f - m_ProjTM.e22); }
    virtual void        SetZn           ( float zn ) { SetFOVx( GetFOVx(), zn, GetZf(), GetAspect() ); }
    virtual void        SetZf           ( float zf ) { SetFOVx( GetFOVx(), GetZn(), zf, GetAspect() ); }
    virtual void        SetZnf          ( float zn, float zf ) { SetFOVx( GetFOVx(), zn, zf, GetAspect() ); }
    virtual float       GetAspect       () const {  return m_ProjTM.e11/m_ProjTM.e00; }
    virtual void        SetAspect       ( float aspect ) { SetViewVolume( 2.0f*GetZn()/m_ProjTM.e00, aspect, GetZn(), GetZf() ); }

    float               GetFOVy         () const { return 2.0f*atan( 1.0f/m_ProjTM.e11 ); }
    float               GetFOVx         () const { return 2.0f*atan( 1.0f/m_ProjTM.e00 ); }
    void                SetFOVy         ( float fovy ) { SetFOVy( fovy, GetZn(), GetZf(), GetAspect() ); }
    void                SetFOVx         ( float fovx ) { SetFOVx( fovx, GetZn(), GetZf(), GetAspect() ); }

    void                SetFOVx         ( float fovx, float zn = 1.0f, float zf = 100.0f, float aspect = 4.0f/3.0f );
    void                SetFOVy         ( float fovy, float zn = 1.0f, float zf = 100.0f, float aspect = 4.0f/3.0f );

    float               GetFarW         () const { return m_ProjTM.e22*2.0f*GetZn()*(m_ProjTM.e22 - 1.0f)/m_ProjTM.e00; }
    float               GetFarH         () const { return m_ProjTM.e22*2.0f*GetZn()*(m_ProjTM.e22 - 1.0f)/m_ProjTM.e11; }

    virtual void        Expose          ( PropertyMap& pm );
    virtual void        ShiftZ          ( float amount );

    DECLARE_SCLASS(PerspCamera, Camera, NPCM);
}; // class PerspCamera 

#endif // __VCAMERA_H__