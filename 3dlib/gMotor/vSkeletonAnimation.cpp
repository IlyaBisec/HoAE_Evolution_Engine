/*****************************************************************************/
/*	File:	vSkeletonAnimation.cpp
/*	Author:	Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "kContext.h"
#include "kHash.hpp"

#include "mFloatCurve.h"
#include "mQuatCurve.h"
#include "vSkeletonAnimation.h"
#include "vModelInstance.h"
#include "vModel.h"

/*****************************************************************************/
/*  PRSTrack implementation
/*****************************************************************************/
PRSTrack::PRSTrack()
{
    m_BoneID    = -1;    
    m_bPose     = true;     
    m_Pose      = Matrix4D::identity;      
} // PRSTrack::PRSTrack

void PRSTrack::Expose( PropertyMap& pm )
{
    pm.start( "PRSTrack", this );
    pm.p( "NodeType", ClassName );
    pm.p( "BoneName", GetName, SetName );
} // PRSTrack::Expose

void PRSTrack::SetPose( const Matrix4D& pose ) 
{ 
    m_Pose = pose;
    Vector3D sc, tr;
    Quaternion rot;
    m_Pose.Factorize( sc, rot, tr );

    m_PosX.SetDefaultValue( tr.x );
    m_PosY.SetDefaultValue( tr.y );
    m_PosZ.SetDefaultValue( tr.z );

    m_ScX.SetDefaultValue( sc.x );
    m_ScY.SetDefaultValue( sc.y );
    m_ScZ.SetDefaultValue( sc.z );

    m_Rot.SetDefaultValue( rot );                 
} // PRSTrack::SetPose

/*****************************************************************************/
/*  SkeletonAnimation implementation
/*****************************************************************************/
SkeletonAnimation::SkeletonAnimation()
{
    m_ID = -1;
    m_bBound = false;
} // SkeletonAnimation::SkeletonAnimation

void SkeletonAnimation::Expose( PropertyMap& pm )
{
    pm.start( "SkeletonAnimation", this );
    pm.p( "NodeType", ClassName );
    pm.p( "Name", GetName, SetName );
} // SkeletonAnimation::Expose

void SkeletonAnimation::Bind( ModelObject* pModel )
{
    int nTracks = m_Tracks.size();
    for (int i = 0; i < nTracks; i++)
    {
        PRSTrack& track = m_Tracks[i];
        track.SetBoneID( pModel->GetBoneIndex( track.GetName() ) );
    }
} // SkeletonAnimation::Bind


