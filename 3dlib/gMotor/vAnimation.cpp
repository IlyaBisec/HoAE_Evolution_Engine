/*****************************************************************************/
/*	File:	vAnimation.cpp
/*	Desc:	Animation interface implementation
/*	Author:	Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "IAnimation.h"
#include "kContext.h"
#include "kHash.hpp"

IAnimMixer* IAnmMixer = NULL;

/*****************************************************************************/
/*  Enum:   AnimStatus
/*  Desc:   
/*****************************************************************************/
enum AnimStatus
{
    asPlaying       = 0,
    asFinalizing    = 1,
}; // enum AnimStatus

/*****************************************************************************/
/*  Class:  AnimInstance
/*  Desc:   Instanced animation object
/*****************************************************************************/
class AnimInstance 
{
    float           m_Time;         //  last time animation was applied
    AnimStatus      m_Status;       //  
    DWORD           m_ModelID;      //  ID of the animated model
    DWORD           m_AnimID;       //  ID of the applied animation

public:

};  // class AnimInstance 

const int c_AnimPoolSize = 4096;
typedef StaticHash< AnimInstance, EntityContext, c_DefTableSize, c_AnimPoolSize> AnimInstanceHash;
/*****************************************************************************/
/*  Class:  AnimMixer
/*  Desc:   Animation mixer interface implementation   
/*****************************************************************************/
class AnimMixer : public IAnimMixer
{
    float           m_RelaxationTime;
public:
                    AnimMixer           ();
    virtual int     Animate             ( DWORD modelID, DWORD animID, float anmTime );
    virtual void    SetRelaxationTime   ( float relTime );


}; // class AnimMixer

/*****************************************************************************/
/*  AnimMixer implemetation
/*****************************************************************************/
AnimMixer::AnimMixer()
{
    IAnmMixer           = this;
    m_RelaxationTime    = 1.0f;
}

int AnimMixer::Animate( DWORD modelID, DWORD animID, float anmTime )
{
    return 0;
} // AnimMixer::Animate

void AnimMixer::SetRelaxationTime( float relTime )
{

} // AnimMixer::SetRelaxationTime