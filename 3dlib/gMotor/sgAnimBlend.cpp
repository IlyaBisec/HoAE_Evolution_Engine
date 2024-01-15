/*****************************************************************/
/*  File:   sgAnimBlend.cpp
/*  Desc:   Animation blending routines
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Nov 2003
/*****************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgAnimBlend.h"
#include "kFilePath.h"

IMPLEMENT_CLASS( AnimationBlock        );

/*****************************************************************************/
/*    AnimationBlock implementation
/*****************************************************************************/
AnimationBlock::AnimationBlock()
{
}

void AnimationBlock::Render()
{
    Parent::Render();
    PushTime( GetCurrentTime() );
    Iterator it( this );
    ++it;
    while (it)
    {
        Node* pNode = (Node*)(*it);
        if (!pNode->IsInvisible()) pNode->Render();
        ++it;
    }
    PopTime();
} // AnimationBlock::Render

void AnimationBlock::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
} // AnimationBlock::Unserialize

void AnimationBlock::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
} // AnimationBlock::Unserialize


