/*****************************************************************************/
/*	File:	vSkelAnimTrack.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-04-2004
/*****************************************************************************/
#include "stdafx.h"
#include "vSkeletonController.h"
#include "vSkeletonAnimation.h"
#include "vSkelAnimTrack.h"
#include "vModelInstance.h"

/*****************************************************************************/
/*  SkelAnimTrack implementation
/*****************************************************************************/
SkelAnimTrack::SkelAnimTrack()
{
    m_pAnimation    = NULL;
    m_EaseInTime    = 0.0f; 
    m_AnimLength    = 0.0f;   
    m_AnimPhase     = 0.0f;    
    m_AnimSpeed     = 1.0f;    
    m_LastAnimTime  = 0.0f; 
    m_LoopMode      = lmNone;
    m_LoopIteration = 0;
} // SkelAnimTrack::SkelAnimTrack

bool SkelAnimTrack::Apply( BoneInstance* pBones, float weight, float cTime )
{
    if (!m_pAnimation) return false;
    
    if (cTime < m_StartOperateTime) return false;
    m_bStarted = true;
    float opTime = cTime - m_StartOperateTime;
    if (opTime > m_OperateTime) 
    {
        m_bOver = true;
        return false;
    }
    //  find time inside animation record
    float totAnmTime = opTime/m_AnimSpeed + m_AnimPhase;
    float anmTime = totAnmTime;
    if (m_LoopMode == lmNone) anmTime = tmin( totAnmTime, m_AnimLength );
    else if (m_LoopMode == lmCycle) 
    {
        anmTime = fmodf( totAnmTime, m_AnimLength );
        m_LoopIteration = (totAnmTime - anmTime)/m_AnimLength;
    }
    else if (m_LoopMode == lmPong) 
    {
        anmTime = fmodf( totAnmTime, m_AnimLength );
        m_LoopIteration = (totAnmTime - anmTime)/m_AnimLength;
        if (m_LoopIteration&1) anmTime = m_AnimLength - anmTime;
    }

    int nTracks = m_pAnimation->GetNTracks();
    if (nTracks == 0) return false;
    for (int i = 0; i < nTracks; i++)
    {
        PRSTrack& track = m_pAnimation->GetTrack( i );
        int boneID = track.GetBoneID();
        if (boneID < 0) continue;
        pBones[boneID].BlendLocalSpace( weight, track.GetTransform( anmTime ) );
    }
    m_LastAnimTime      = anmTime;
    m_LastOperateTime   = cTime;
    return true;
} // SkelAnimTrack::Apply

float SkelAnimTrack::GetWeight( float cTime ) const 
{ 
    if (cTime < m_StartOperateTime) return 0.0f;
    float weight = m_Weight;
    if (m_EaseInTime > c_Epsilon)
    {
        float easeIn = (cTime - m_StartOperateTime)/m_EaseInTime;
        if (easeIn < 1.0f && easeIn >= 0.0f && m_LoopIteration == 0) 
        {
            weight *= easeIn;
        }
    }
    const float c_WeightPruneBias = 0.01f;
    if (weight < c_WeightPruneBias) return 0.0f;
    return weight; 
} // SkelAnimTrack::GetWeight