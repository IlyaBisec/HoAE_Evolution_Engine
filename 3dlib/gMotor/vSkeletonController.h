/*****************************************************************************/
/*	File:	vSkeletonController.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-04-2004
/*****************************************************************************/
#ifndef __VSKELETONCONTROLLER_H__
#define __VSKELETONCONTROLLER_H__

class BoneInstance;
class ModelInstance;
/*****************************************************************************/
/*  Class:  SkeletonController
/*  Desc:   Operates upon bones of the some model instance
/*****************************************************************************/
class SkeletonController
{
public:
    float               m_Weight;           //  absolute controller influence weight
    float               m_NormWeight;       //  normalized influence weight

    float               m_LastOperateTime;  //  last (absolute) time conroller was applied
    float               m_StartOperateTime; //  starting absolute time of applying controller    
    float               m_OperateTime;      //  total time controller is applied

    bool                m_bStarted;         //  controller started operating
    bool                m_bOver;            //  controller already ended operating

    ModelInstance*      m_pModelInstance;   //  pointer to the model instance we are operating on

    friend class        ModelInstance;
    
    SkeletonController()
    {
        m_Weight            = 1.0f;
        m_NormWeight        = 1.0f;
        m_LastOperateTime   = 0.0f;  
        m_StartOperateTime  = 0.0f; 
        m_OperateTime       = 0.0f;
        m_bStarted          = false;         
        m_bOver             = false;            
        m_pModelInstance    = NULL;
    }
    ///  applies bone control
    virtual bool        Apply( BoneInstance* pBones, float weight, float cTime ) = 0;
    virtual bool        IsOver() const = 0;
    virtual float       GetWeight( float cTime ) const { return m_Weight; }

}; // class SkeletonController

#endif // __VSKELETONCONTROLLER_H__