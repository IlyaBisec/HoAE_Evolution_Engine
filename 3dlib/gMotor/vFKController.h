/*****************************************************************************/
/*	File:	vFKController.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-04-2004
/*****************************************************************************/
#ifndef __VFKCONTROLLER_H__
#define __VFKCONTROLLER_H__
#include "vSkeletonController.h"

/*****************************************************************************/
/*  Class:  FKControlMode
/*****************************************************************************/
enum FKControlMode
{
    fkcUnknown      = 0,
    fkcPosition     = 1,    //  only root position is controlled
    fkcRotation     = 2,    //  only root rotation is controlled
    fkcTransform    = 3     //  whole root transform is controlled    
}; // enum FKControlMode

/*****************************************************************************/
/*  Class:  FKController
/*  Desc:   
/*****************************************************************************/
class FKController : public SkeletonController
{
public:
    FKControlMode       m_ControlMode;      //  which TM components we are changing
    int                 m_BoneID;           //  id of the bone we are controlling
        
    Vector3D            m_StartPos;         //  starting bone position
    Vector3D            m_TargetPos;        //  target bone position

    Quaternion          m_StartRot;         //  starting bone orientation
    Quaternion          m_TargetRot;        //  target bone orientation

    Vector3D            m_StartScale;       //  starting bone scale
    Vector3D            m_TargetScale;      //  target bone scale

                        FKController  ( FKControlMode mode );
    virtual bool        Apply         ( BoneInstance* pBones, float weight, float cTime );
    virtual bool        IsOver        () const { return m_bOver; }

}; // class FKController

#endif // __VFKCONTROLLER_H__