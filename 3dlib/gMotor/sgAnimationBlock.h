/*****************************************************************/
/*  File:   sgAnimationBlock.h
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Nov 2003
/*****************************************************************/
#ifndef __SGANIMATIONBLOCK_H__
#define __SGANIMATIONBLOCK_H__

#include "IResourceManager.h"
#include "sgAnimation.h"

/*****************************************************************************/
/*    Class:    AnimationBlock
/*    Desc:    Group of animation nodes, synchronized by time
/*****************************************************************************/
class AnimationBlock : public Animation, public IResource
{
    std::string             m_FileName;
public:    
                            AnimationBlock  ();
    virtual void            Render          ();
    const char*             GetFileName     () const { return m_FileName.c_str(); }
    void                    SetFileName     ( const char* name ) { m_FileName = name; }

    virtual bool            Reload          ();
    virtual bool            Dispose         ();

    DECLARE_SCLASS(AnimationBlock,Animation,ANMB);
}; // class AnimationBlock


#endif //__SGANIMATIONBLOCK_H__

