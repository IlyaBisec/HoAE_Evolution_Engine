/*****************************************************************************/
/*	File:	vFKController.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-04-2004
/*****************************************************************************/
#include "stdafx.h"
#include "vModelInstance.h"
#include "vFKController.h"

/*****************************************************************************/
/*  FKController implementation
/*****************************************************************************/
FKController::FKController( FKControlMode mode ) : m_ControlMode(mode)
{
    m_BoneID     = -1;
} // FKController::FKController

bool FKController::Apply( BoneInstance* pBones, float weight, float cTime )
{
    BoneInstance& bone = pBones[m_BoneID];
    Vector3D trans, scale;
    Quaternion rot;
    Vector3D curTrans, curScale;
    Quaternion curRot;

    float w2 = (cTime - m_StartOperateTime)/m_OperateTime;
    if (w2 < 0.0f) return false;
    if (w2 > 1.0f)
    {
        m_bOver = true;
        return false;
    }

    //  get initial positions as current
    if (!m_bStarted)
    {
        Matrix4D tm = bone.GetWorldTM();
        tm.Factorize( m_StartScale, m_StartRot, m_StartPos );
        m_bStarted = true;
    }

    float w1 = 1.0f - w2;

    if (m_ControlMode == fkcPosition)
    {
        trans.addWeighted( m_StartPos, m_TargetPos, w1, w2 );
        bone.GetWorldSRT( curScale, curRot, curTrans );
        bone.BlendWorldSpace( weight, trans, curRot, curScale );
    }
    else if (m_ControlMode == fkcRotation)
    {
        rot.Slerp( m_StartRot, m_TargetRot, w2 );
        bone.GetWorldSRT( curScale, curRot, curTrans );
        bone.BlendWorldSpace( weight, curTrans, rot, curScale );
    }
    else if (m_ControlMode == fkcTransform)
    {
        trans.addWeighted( m_StartPos, m_TargetPos, w1, w2 );
        scale.addWeighted( m_StartScale, m_TargetScale, w1, w2 );
        rot.Slerp( m_StartRot, m_TargetRot, w2 );
        bone.BlendWorldSpace( weight, trans, rot, scale );
    }
    return false;
} // FKController::Apply

