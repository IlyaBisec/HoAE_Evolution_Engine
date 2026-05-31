/*****************************************************************************/
/*    File:    uiCameraTrack.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    30.06.2004
/*****************************************************************************/
#ifndef __UICAMERATRACK_H__
#define __UICAMERATRACK_H__

/*
struct CameraKeyFrame
{
    Vector3D        dir;
    Vector3D        lookAt;
}; // struct CameraKeyFrame

class CameraTrack : public AnimationCurve<CameraKeyFrame>
{
public:
    CameraTrack()
    {
        m_DefaultValue.dir = Vector3D( 0.0f, -c_CosPId6, -0.5f );
        m_DefaultValue.lookAt = Vector3D::null;
    }
    virtual CameraKeyFrame GetValue( float time ) const;
}; // class CameraTrack


class CameraPathEditor : public Dialog
{
    CameraTrack             m_Track;
public:
    
    virtual void            Expose( PropertyMap& pm );
    virtual void            Render();

    void                    Reset(){ m_Track.Reset(); }
    int                     GetNKeys() const { return m_Track.GetNKeys(); }
    
    void                    Capture();
    void                    Play();
    void                    Pause();
    void                    Stop();

    DECLARE_SCLASS(CameraPathEditor, Dialog, CPED)
}; // class CameraPathEditor

void  DrawCameraGizmo( const Vector3D& pos, const Vector3D& dir, float size = 10.0f );
*/


#endif // __UICAMERATRACK_H__