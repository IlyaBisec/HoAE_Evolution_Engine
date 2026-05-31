/*****************************************************************************/
/*    File:    uiModelView.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiWindow.h"
#include "IMediaManager.h"
#include "ISpriteManager.h"
#include "IEffectManager.h"
#include "kTimer.h"
#include "vCamera.h"
#include "kContext.h"

#include "uiModelView.h"

void DrawThumbnail( DWORD mdlID, const Rct& vp, 
                    DWORD animID, float animTime, 
                    float rotAngle, DWORD bgColor, 
                    float scale, float shift )
{
    Rct oldVP = IRS->GetViewPort();
    Rct nvp( vp );
    oldVP.Clip( nvp );
    if (nvp.w < c_SmallEpsilon || nvp.h < c_SmallEpsilon) return;

    static PerspCamera cam;
    float       aspect = nvp.w/nvp.h;
    const float fov    = DegToRad( 45.0f ); 
    //  setup camera
    Vector3D gdir( 0.0f, -cos( c_PI/6.0f ), -sin( c_PI/6.0f ) );
    float r = 200.0f;
    AABoundBox aabb = IMM->GetBoundBox( mdlID );
    r = 0.5f*tmax( aabb.GetDX(), aabb.GetDY(), aabb.GetDZ() )/scale; 
    if (r < c_Epsilon) r = 200.0f;

    cam.SetOrientation( Vector3D::oX, Vector3D::oZ, gdir );

    Vector3D center = aabb.GetCenter();
    gdir.reverse();
    float dist = r/tanf( fov*0.5f );
    if (nvp.w > nvp.h) dist *= nvp.w/nvp.h;
    gdir *= dist;
    gdir += center;
    float zn = tmax( dist - r*2.0f, 1.0f );
    float zf = dist + r*8.0f;
    cam.SetPosition( gdir );
    cam.SetFOVx( fov, zn, zf, aspect );
    
    Vector3D cshift( center );
    cshift.reverse();
    cshift.z += r*scale*shift;
    
    Matrix4D tm = Matrix4D::identity;
    tm.translation( cshift );
    Matrix4D rot;
    rot.rotation( Vector3D::oZ, rotAngle );
    tm *= rot;
    tm.translate( center );

    ICamera* pOldCam = GetCamera();
    cam.SetViewport( nvp );
    cam.Render();
    

    rsFlush();
    ISM->Flush();
    IRS->SetViewPort( nvp );
    if (bgColor == 0) IRS->ClearDevice( 0, false, true ); 
    else IRS->ClearDevice( bgColor, true, true ); 
    
    PushEntityContext( mdlID );
    PushEntityContext( nvp.x );
    PushEntityContext( nvp.y );

    //  model case
    IMM->StartModel( mdlID, tm, CUR_CONTEXT );
    IMM->AnimateModel( animID, animTime );
    IMM->DrawModel();

    // effect case
    IEffMgr->InstanceEffect( mdlID );    
    IEffMgr->UpdateInstance();

    IEffMgr->Evaluate();
    IEffMgr->PreRender();
    IEffMgr->PostRender();
    IEffMgr->PostEvaluate();

    ISM->Flush();
    IRS->Flush();

    PopEntityContext();
    PopEntityContext();
    PopEntityContext();

    IRS->SetViewPort( oldVP );
    if (pOldCam) pOldCam->Render();

} // DrawThumbnail


IMPLEMENT_CLASS(ModelView);
/*****************************************************************************/
/*    ModelView implementation
/*****************************************************************************/
ModelView::ModelView()
{
    m_RotateSpeed   = 1.0f;
    m_FOV           = DegToRad( 45.0f );
    m_bRotate       = true;
    m_ModelID       = -1;

    m_WorldTM.setIdentity();
    SetBgColor( 0x66555577 );
    //AddChild( m_NPoly );
    //AddChild( m_NVert );
} // ModelView::ModelView

bool ModelView::OnExtents()
{
    Rct ext = GetExtents();
    m_NPoly.SetCaption  ( "Poly:" );
    m_NPoly.SetExtents  ( 5, ext.h - 24, ext.w - 10, 12 );
    m_NVert.SetCaption  ( "Vert:" );
    m_NVert.SetExtents  ( 5, ext.h - 12, ext.w - 10, 12 );
    m_NPoly.SetTextColor( 0xFFFFFF00 );
    m_NVert.SetTextColor( 0xFFFFFF00 );
    return false;
} // ModelView::OnExtents

void ModelView::Expose( PropertyMap& pm )
{   
    pm.start( "ModelView", this );
} // ModelView::Expose

void ModelView::Render()
{
    if (m_ModelID == -1) SetModel( "question_mark" );
    Window::Render();

    Rct ext( GetScreenExtents() );
    ext.Inflate( -2.0f );
    
    float angle = m_Timer.seconds()*m_RotateSpeed;
    Vector3D shift( m_ModelCenter );
    shift.reverse();

    Matrix4D tm = m_WorldTM;

    if (m_bRotate)
    {
        tm.translation( shift );
        Matrix4D rot;
        rot.rotation( Vector3D::oZ, angle );
        tm *= rot;
        tm.translate( m_ModelCenter );
    }

    ICamera* pOldCam = GetCamera();
    m_Camera.SetViewport( ext );
    
    m_Camera.Render();
    IRS->ClearDevice( 0, false, true ); 
    IMM->StartModel( m_ModelID, tm );
    IMM->DrawModel();

    if (pOldCam) pOldCam->Render();

} // ModelView::Render

bool ModelView::SetModel( const char* mdlName )
{
    m_ModelID = IMM->GetModelID( mdlName );
    if (m_ModelID < 0) 
    {
        static int qID = IMM->GetModelID( "question_mark" );
        if (qID > 0) SetModel( "question_mark" ); 
        return false;
    }
    m_Timer.reset();

    float aspect = m_Extents.w/m_Extents.h;

    //  default camera
    Vector3D gdir( 0.0f, -cos( c_PI/6.0f ), -sin( c_PI/6.0f ) );
    float r = 100.0f;
    AABoundBox aabb = IMM->GetBoundBox( m_ModelID );
    r = 0.5f*tmax( aabb.GetDX(), aabb.GetDY(), aabb.GetDZ() ); 
    if (r < c_Epsilon) r = 100.0f;

    m_Camera.SetOrientation( Vector3D::oX, Vector3D::oZ, gdir );

    m_ModelCenter = aabb.GetCenter();
    gdir.reverse();
    float dist = r/tanf( m_FOV*0.5f );
    gdir *= dist;
    gdir += m_ModelCenter;
    float zn = tmax( dist - r, 1.0f );
    float zf = dist + r*2.0f;
    m_Camera.SetPosition( gdir );
    m_Camera.SetFOVx( m_FOV, zn, zf, aspect );
    
    //m_NPoly.SetText( ToString( pMdl->GetNPoly() ) );
    //m_NVert.SetText( ToString( pMdl->GetNVertices() ) );

    return true;
} // ModelView::SetModel
