/*****************************************************************************/
/*    File:    uiAlgoDebugger.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    30 Dec 2004
/*****************************************************************************/
#ifndef __UIALGODEBUGGER_H__
#define __UIALGODEBUGGER_H__

#include "uiObjectInspector.h"
#include "uiRTSController.h"

/*****************************************************************************/
/*    Class:    AlgorithmDebugger
/*    Desc:    
/*****************************************************************************/    
class AlgorithmDebugger : public Window
{
    ObjectInspector         m_Props;
    RTSController           m_CamController;
    int                     m_Number;
    float                   m_Scale;
    Vector3D                m_Pos;
    Vector3D                m_Dir;
    DWORD                   m_Color;

    bool                    m_bShowBackdrop;
    bool                    m_bShowDebugStuff;
    Window                  m_Toolbar;

    float                   m_FocusDistance;         
    float                   m_FocusRatio;    
    int                     m_LOD;

    Frustum                 m_FrustumSnapshot;

public:
                            AlgorithmDebugger();

    virtual void            Render      ();
    virtual void            Expose      ( PropertyMap& pm );
    virtual void            OnInit      ();
    
    virtual bool             OnMouseWheel    ( int delta );                    
    virtual bool             OnMouseMove        ( int mX, int mY, DWORD keys );  
    virtual bool             OnMouseLBDown    ( int mX, int mY );                
    virtual bool             OnMouseMBDown    ( int mX, int mY );                
    virtual bool             OnMouseRBDown    ( int mX, int mY );                
    virtual bool             OnMouseLBUp        ( int mX, int mY );                
    virtual bool             OnMouseMBUp        ( int mX, int mY );                
    virtual bool             OnMouseRBUp        ( int mX, int mY );                
    virtual bool            OnKeyDown        ( DWORD keyCode, DWORD flags );    
    virtual bool            OnKeyUp            ( DWORD keyCode, DWORD flags );    


    DECLARE_CLASS(AlgorithmDebugger);

private:
    void                    Reset();
}; // class AlgorithmDebugger

#endif // __UIALGODEBUGGER_H__