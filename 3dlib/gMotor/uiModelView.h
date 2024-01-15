/*****************************************************************************/
/*    File:    uiModelView.h
/*    Desc:    Previews model file
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Dec 2004
/*****************************************************************************/
#ifndef __UIMODELVIEW_H__
#define __UIMODELVIEW_H__

#include "uiLabel.h"
#include "uiWindow.h"
#include "vCamera.h"
#include "kTimer.h"


/*****************************************************************************/
/*    Class:    ModelView
/*    Desc:    Previews model file
/*****************************************************************************/    
class ModelView : public Window
{
    int                 m_ModelID;
    int                 m_PolyCount;
    int                 m_VertexCount;
    PerspCamera         m_Camera;

    AABoundBox          m_AABB;
    int                 m_PolyLimit;
    float               m_RotateSpeed;
    float               m_FOV;
    Matrix4D            m_WorldTM;    
    Vector3D            m_ModelCenter;
    Timer               m_Timer;

    Label               m_NVert;
    Label               m_NPoly;
    bool                m_bRotate;

public:

                        ModelView   ();
    virtual void        Render        ();
    bool                SetModel    ( const char* mdlName );
    void                SetPolyLimit( int limit ) { m_PolyLimit = limit; }

    virtual bool        OnExtents    ();
    virtual void        Expose      ( PropertyMap& pm ); 


    DECLARE_CLASS(ModelView);
}; // class ModelView

void DrawThumbnail( DWORD mdlID, const Rct& vp, 
                   DWORD animID = 0xFFFFFFFF, float animTime = 0.0f, 
                   float rotAngle = 0.0f, DWORD bgColor = 0x00000000, float scale = 1.0f, float shift = 0.0f );

#endif // __UIMODELVIEW_H__