/*****************************************************************/
/*  File:   sgAnimationBlock.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Nov 2003
/*****************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgAnimationBlock.h"
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
        SNode* pNode = (SNode*)(*it);
        assert( pNode->IsA<Animation>() );
        if (!pNode->IsInvisible()) pNode->Render();
        ++it;
    }
    PopTime();
} // AnimationBlock::Render

bool AnimationBlock::Reload()
{
    int resID = IRM->FindResource( m_FileName.c_str() );
    if (resID == -1) return false;
    InStream& is = IRM->LockResource( m_FileName.c_str() );

    FilePath path( IRM->GetPath( resID ) );
    _chdir( path.GetDrive() );
    _chdir( path.GetDir() );

    SNode* pModel = SNode::UnserializeSubtree( is );            
    is.Close();
    _chdir( IRM->GetHomeDirectory() );
    if (!pModel) return c_BadID;

    if (pModel->IsA<AnimationBlock>()) 
    {
        ReleaseChildren();
        for (int i = 0; i < pModel->GetNChildren(); i++)
        {
            AddChild( pModel->GetChild( i ) );
        }
        pModel->ClearChildren();
        pModel->Release();
    }
    else
    {
        AddChild( pModel );
    }

    _chdir( IRM->GetHomeDirectory() );
    return true;
} // AnimationBlock::Reload

bool AnimationBlock::Dispose()
{
    if (GetNChildren() == 0) return false;
    ReleaseChildren();
    return true;
} // AnimationBlock::Dispose


