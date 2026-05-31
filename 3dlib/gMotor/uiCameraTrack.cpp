/*****************************************************************************/
/*    File:    uiCameraTrack.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    30.06.2004
/*****************************************************************************/

#include "stdafx.h"
#include "sg.h"
#include "uiControl.h"
#include "uiCameraTrack.h"
/*
IMPLEMENT_CLASS( CameraPathEditor );

CameraKeyFrame CameraTrack::GetValue( float time ) const
{
    //  extract neighbor keyframes
    if (m_Values.size() == 0) return m_DefaultValue;
    CameraKeyFrame kf;
    int kfIdx1 = LocateTime( time );
    if (kfIdx1 < 0) return m_Values[0];

    float t1 = m_Times[kfIdx1];
    const CameraKeyFrame& k1 = m_Values[kfIdx1];

    int kfIdx2 = kfIdx1 + 1;
    if (kfIdx2 >= m_Times.size()) return k1;

    float t2 = m_Times[kfIdx2];
    const CameraKeyFrame&  k2 = m_Values[kfIdx2];
    if (time == t1) return k1;
    if (time == t2) return k2;

    float t = (time - t1) / (t2 - t1);

    //  interpolate lloking direction
    Vector3D sUp = Vector3D::oZ;
    Vector3D dUp = Vector3D::oZ;

    Vector3D sDir( k1.dir );
    Vector3D dDir( k2.dir );

    Vector3D sRight, dRight; 
    sRight.cross( sUp, sDir );
    dRight.cross( dUp, dDir );

    Vector3D::orthonormalize( sDir, sRight, sUp );
    Vector3D::orthonormalize( dDir, dRight, dUp );

    Matrix3D sRot( sRight, sUp, sDir );
    Matrix3D dRot( dRight, dUp, dDir );

    Quaternion sRotQ( sRot );
    Quaternion dRotQ( dRot );
    Quaternion rotQ;
    rotQ.Slerp( sRotQ, dRotQ, t );
    Matrix3D rot( rotQ );
    Vector3D pos;
    CameraKeyFrame res;
    //  interpolate look-at point
    res.lookAt.addWeighted( k1.lookAt, k2.lookAt, 1.0f - t, t );
    res.dir = rot.getV0();
    return res;
} // CameraTrack::GetValue

void  DrawCameraGizmo( const Vector3D& pos, const Vector3D& dir, float size )
{
    assert( false );
} // DrawCameraGizmo


void CameraPathEditor::Capture()
{

}

void CameraPathEditor::Play()
{

}

void CameraPathEditor::Pause()
{

}

void CameraPathEditor::Stop()
{

}

void CameraPathEditor::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "CameraPathEditor", this );
    pm.p( "NumKeys",    GetNKeys );
    pm.m( "Capture",    Capture );
    pm.m( "Play",       Play    );
    pm.m( "Pause",      Pause   );
    pm.m( "Stop",       Stop    );
} // CameraPathEditor::Expose

void CameraPathEditor::Render()
{
    Parent::Render();
} // CameraPathEditor::Render

*/
