/*****************************************************************************/
/*	File:	vIKConstraint.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-04-2004
/*****************************************************************************/
#ifndef __VIKCONSTRAINT_H__
#define __VIKCONSTRAINT_H__

#include "vSkeletonController.h"

struct IKChainNode
{
    int         m_BoneID;
    Matrix4D    m_CurTM;
}; // struct IKChainNode

/*****************************************************************************/
/*  Class:  IKConstraint
/*  Desc:   Represents IK chain solver
/*****************************************************************************/
class IKConstraint : public SkeletonController
{
    std::vector<IKChainNode>    m_IKChain;
    int                         m_EffectorID;

public:
    virtual bool        Apply       ( BoneInstance* pBones, float weight, float cTime ) { return true; }
    virtual bool        IsOver      () const { return false; }

}; // class IKConstraint

#endif // __VIKCONSTRAINT_H__