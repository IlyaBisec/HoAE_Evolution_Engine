/*****************************************************************************/
/*	File:	vSkelAnimTrack.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-04-2004
/*****************************************************************************/
#ifndef __VSKELANIMTRACK_H__
#define __VSKELANIMTRACK_H__

#include "IModel.h"
#include "vSkeletonController.h"

class SkeletonAnimation;
/*****************************************************************************/
/*  Struct: SkelAnimTrack
/*  Desc:   Skeleton controller, instanced for given model instance
/*****************************************************************************/
class SkelAnimTrack : public SkeletonController
{
public:
    SkeletonAnimation*      m_pAnimation;       //  pointer to the animation record

    float                   m_AnimLength;       //  animation record length (seconds)    
    float                   m_AnimPhase;        //  starting time inside animation record (seconds)
    float                   m_AnimSpeed;        //  animation record playback speed (1.0f is normal speed) 
    float                   m_LastAnimTime;     //  last record-space time animation was applied

    float                   m_EaseInTime;       //  track ease in time, in seconds
    LoopMode                m_LoopMode;         //  whether to loop animation
    int                     m_LoopIteration;    //  current loop iteration

                            SkelAnimTrack   ();
    virtual bool            Apply           ( BoneInstance* pBones, float weight, float cTime );
    virtual bool            IsOver          () const { return m_bOver; }
    virtual float           GetWeight       ( float cTime ) const;

}; // class SkelAnimTrack

#endif // __VSKELANIMTRACK_H__