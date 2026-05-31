/*****************************************************************************/
/*    File:    uiMayaController.h
/*    Desc:    Maya-like camera controller
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIMAYACONTROLLER_H__
#define __UIMAYACONTROLLER_H__

enum ControlMode
{
    cmIdle        = 0,
    cmRotate    = 1,
    cmPan        = 2,
    cmZoom        = 3
}; // enum ControlMode

/*****************************************************************************/
/*    Class:    MayaController
/*    Desc:    Mimicks maya camera controller
/*****************************************************************************/
class MayaController : public Widget
{
public:
                    MayaController      ();
    virtual void    Serialize            ( OutStream& os ) const;
    virtual void    Unserialize            ( InStream& is );
    virtual void    Expose                ( PropertyMap& pm );
    
    bool            UpdateCamera        ();
    void            Zoom                ();
    void            Pan                 ();
    void            Rotate              ();
    void            Idle                ();

    virtual bool     OnMouseMove            ( int mX, int mY, DWORD keys );
    virtual bool     OnMouseLBDown        ( int mX, int mY );
    virtual bool     OnMouseMBDown        ( int mX, int mY );
    virtual bool     OnMouseRBDown        ( int mX, int mY );

    virtual bool     OnMouseLBUp            ( int mX, int mY ) { Idle(); return false; }
    virtual bool     OnMouseMBUp            ( int mX, int mY ) { Idle(); return false; }
    virtual bool     OnMouseRBUp            ( int mX, int mY ) { Idle(); return false; }

    void            LockRotate            ( bool lock = true ) { m_bLockRotate = lock; }
    void            LockPan                ( bool lock = true ) { m_bLockPan     = lock; }
    void            LockZoom            ( bool lock = true ) { m_bLockZoom     = lock; }
	void			Invert(bool IsInverted = true) { m_IsInverted = IsInverted; }

    DECLARE_CLASS(MayaController);

protected:
    Vector3D        ScreenToVector      ( float x, float y );
    void            CreateCursorSwitch  ();

private:
    int                m_Mx, m_My;
    ControlMode        m_Mode;

    bool            m_bLockRotate;
    bool            m_bLockPan;
    bool            m_bLockZoom;
	bool			m_IsInverted;
}; // class MayaController

#endif // __UIMAYACONTROLLER_H__