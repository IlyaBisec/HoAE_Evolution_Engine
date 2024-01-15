/*****************************************************************
/*  File:   IAnimation.h                                      
/*  Desc:   Interface to the animations manipulation
/*	Author:	Ruslan Shestopalyuk
/*  Date:   Jun 2004											 
/*****************************************************************/
#ifndef __IANIMATION_H__ 
#define __IANIMATION_H__ 

/*****************************************************************/
/*	Class:	IAnimInstance
/*  Desc:   Interface to the instanced animation
/*****************************************************************/
class IAnimInstance
{
public:

}; // class IAnimInstance

/*****************************************************************/
/*	Class:	IAnimation
/*  Desc:   Interface to the animation of the mesh
/*****************************************************************/
class IAnimation
{
public:

}; // class IAnimation

/*****************************************************************/
/*	Class:	IController
/*  Desc:   Interface used to control mesh bones
/*****************************************************************/
class IController
{
public:

}; // class IController

/*****************************************************************/
/*	Class:	IAnimMixer
/*  Desc:   Interface used to blend different controllers to 
/*              animate model
/*****************************************************************/
class IAnimMixer
{
public:
    //  updates model state with given animation in current entity context
    virtual int     Animate             ( DWORD modelID, DWORD animID, float anmTime ) = 0;   
    
    //  setups time during which new animation is being mixed with previous
    virtual void    SetRelaxationTime   ( float relTime ) = 0;

}; // class IAnimMixer

extern IAnimMixer* IAnmMixer;

#endif // __IANIMATION_H__ 