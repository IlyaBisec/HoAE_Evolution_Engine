/*****************************************************************/
/*  File:   sgAnimBlend.h
/*  Desc:   Animation blending routines
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Nov 2003
/*****************************************************************/
#ifndef __SGANIMBLEND_H__
#define __SGANIMBLEND_H__

/*****************************************************************************/
/*    Class:    AnimationBlock
/*    Desc:    Group of animation nodes, synchronized by time
/*****************************************************************************/
class AnimationBlock : public Animation
{
public:    
                            AnimationBlock    ();

    virtual void            Render        ();
    virtual void            Unserialize    ( InStream& is    );
    virtual void            Serialize    ( OutStream& os ) const;

    DECLARE_SCLASS(AnimationBlock,Animation,ANMB);
}; // class AnimationBlock


#endif //__SGANIMBLEND_H__

