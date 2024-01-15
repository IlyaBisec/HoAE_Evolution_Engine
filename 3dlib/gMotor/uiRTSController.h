/*****************************************************************************/
/*    File:    uiRTSController.h
/*    Desc:    Maya-like camera controller
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIRTSCONTROLLER_H__
#define __UIRTSCONTROLLER_H__

#include "kTimer.h"

/*****************************************************************************/
/*    Class:    RTSController
/*    Desc:    Mimicks maya camera controller
/*****************************************************************************/
class RTSController : public Widget
{
public:
                    RTSController      ();
    virtual void    Serialize            ( OutStream& os ) const;
    virtual void    Unserialize            ( InStream& is );
    virtual void    Expose                ( PropertyMap& pm );
    virtual void    Render              ();
    
    bool            UpdateCamera        ();

    virtual bool    OnKeyDown            ( DWORD keyCode, DWORD flags );
    virtual bool    OnKeyUp                ( DWORD keyCode, DWORD flags );

    virtual bool     OnMouseMove            ( int mX, int mY, DWORD keys );
    virtual bool     OnMouseLBDown        ( int mX, int mY );
    virtual bool     OnMouseMBDown        ( int mX, int mY );
    virtual bool     OnMouseRBDown        ( int mX, int mY );

    virtual bool     OnMouseLBUp            ( int mX, int mY ) { return false; }
    virtual bool     OnMouseMBUp            ( int mX, int mY ) { return false; }
    virtual bool     OnMouseRBUp            ( int mX, int mY ) { m_bDragRotation = false; return false; }

    virtual bool     OnMouseLBDbl        ( int mX, int mY );

    DECLARE_CLASS(RTSController);

protected:
    void            UpdateCameraPosition();

private:
    int                m_Mx, m_My;
    bool            m_bSnapToGround;
    float           m_HeightFromGround;
    float           m_LookAtDistance;
    Timer           m_Timer;
    float           m_MoveSpeed;
    float           m_RotSpeed;

    bool            m_bDragRotation;
    float           m_DragRotation;

    float           m_DivePos;
    float           m_DiveSpeed;

    float           m_DiveStartAng;
    float           m_DiveEndAng;
    float           m_DiveStartH;
    float           m_DiveEndH;
    float           m_Dive;

    float           m_JumpAmount;
    float           m_JumpSpeed;
    Timer           m_JumpTimer;
    Vector3D        m_TargetLookAt;
    Vector3D        m_StartLookAt;
    Vector3D        m_TargetDir;
    Vector3D        m_StartDir;

}; // class RTSController

#endif // __UIRTSCONTROLLER_H__