/*****************************************************************************/
/*	File:	vAnimClip.cpp
/*	Desc:	Animation file reference
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#include "stdafx.h"
#include "vSkelAnimTrack.h"
#include "vAnimClip.h"
#include "IModelManager.h"

template <class T>
void PickChildName( const char* name, IReflected* pParent, T* pChild )
{   
    if (!pParent || !pChild) return;
    char cName[256];
    int cChild = 1;
    while (true)
    {
        sprintf( cName, "%s%02d", name, cChild );
        int i = 0;
        for (; i < pParent->NumChildren(); i++)
        {
            IReflected* pCurChild = pParent->Child( i ); 
            if (!stricmp( cName, pCurChild->GetName())) break;
        }
        if (i == pParent->NumChildren()) break;
        cChild++;
    }
    pChild->SetName( cName );
} // PickChildName

/*****************************************************************************/
/*  AnimRef implementation
/*****************************************************************************/
void AnimRef::Expose( PropertyMap& pm )
{
    pm.start( "AnimRef", this );
    pm.p( "NodeType", ClassName );
    pm.p( "Name", GetName, SetName, "#animfile" );
    pm.m( "Play", Play );
} // AnimRef::Expose

void AnimRef::Play()
{
    m_AnimID = IModelMgr->GetAnimID( m_Name.c_str() );
    IModelMgr->StopController();
    IModelMgr->ScheduleAnimation( 0.0f, m_AnimID, true, 0.0f, 100000.0f, 1.0f );
}

/*****************************************************************************/
/*  AnimRefList implementation
/*****************************************************************************/
void AnimRefList::Expose( PropertyMap& pm )
{
    pm.start( "AnimRefList", this );
    pm.p( "NodeType", ClassName );
    pm.p( "NumAnimations", NumChildren );
    pm.m( "AddAnimation", AddNewAnimation );
} // AnimRefList::Expose

void AnimRefList::AddNewAnimation()
{
    push_back( AnimRef() );
    PickChildName( "Anim", this, &back() );
} // AnmList::AddNewAnimation

/*****************************************************************************/
/*  ControlClip implementation
/*****************************************************************************/
void ControlClip::Expose( PropertyMap& pm )
{
    pm.start( "ControlClip", this );
    pm.p( "NodeType",           ClassName );
    pm.p( "Name",               GetName, SetName );
    pm.p( "NumTracks",          NumChildren );
    pm.m( "AddAnimTrack",       AddAnimTrack      );
    pm.m( "AddBindEvent",       AddBindEvent      );
    pm.m( "AddEffectTrack",     AddEffectTrack    );
    pm.m( "AddBonePosTrack",    AddBonePosTrack   );
    pm.m( "AddBoneDirTrack",    AddBoneDirTrack   );
    pm.m( "AddClipRef",         AddClipRef        );
    pm.p( "TotalTime",          GetTotalTime );
    pm.p( "CurrentTime",        GetCurTime, SetCurTime );
    pm.m( "Play",               Play );
    pm.m( "Stop",               Stop );
    pm.m( "Pause",              Pause );
} // ControlClip::Expose

float ControlClip::GetCurTime() const
{
    return 0.0f;
}

void ControlClip::SetCurTime( float t )
{

}

void ControlClip::Play()
{

}

void ControlClip::Pause()
{

}

void ControlClip::Stop()
{

}

float ControlClip::GetTotalTime() const
{
    return 0.0f;
}

IReflected* ControlClip::Child( int idx ) const       
{ 
    if (idx < 0 || idx >= m_Tracks.size()) return NULL;
    return m_Tracks[idx]; 
} // ControlClip::Child

void ControlClip::AddAnimTrack()
{
    ControlClipTrack* pTrack = new AnimTrack();
    PickChildName( "Anim", this, pTrack );
    m_Tracks.push_back( pTrack );  
}

void ControlClip::AddBindEvent()
{
    ControlClipTrack* pTrack = new BindEvent();
    PickChildName( "Bind", this, pTrack );
    m_Tracks.push_back( pTrack );
}

void ControlClip::AddEffectTrack()
{
    ControlClipTrack* pTrack = new EffectTrack();
    PickChildName( "Effect", this, pTrack );
    m_Tracks.push_back( pTrack );
}

void ControlClip::AddBonePosTrack()
{
    ControlClipTrack* pTrack = new BonePosTrack();
    PickChildName( "BonePos", this, pTrack );
    m_Tracks.push_back( pTrack );
}

void ControlClip::AddBoneDirTrack()
{
    ControlClipTrack* pTrack = new BoneDirTrack();
    PickChildName( "BoneDir", this, pTrack );
    m_Tracks.push_back( pTrack );
}

void ControlClip::AddClipRef()
{
    ControlClipTrack* pTrack = new ClipRefTrack();
    PickChildName( "Clip", this, pTrack );
    m_Tracks.push_back( pTrack );
}

/*****************************************************************************/
/*  ControlClipList implementation
/*****************************************************************************/
void ControlClipList::Expose( PropertyMap& pm )
{
    pm.start( "ControlClipList", this );
    pm.p( "NodeType", ClassName );
    pm.p( "NumClips", NumChildren );
    pm.m( "AddNewClip", AddNewClip );
} // ControlClipList::Expose

IReflected* ControlClipList::Child( int idx ) const    
{ 
    if (idx < 0 || idx >= m_Clips.size()) return NULL;
    return (IReflected*)&m_Clips[idx]; 
} // ControlClipList::Child

void ControlClipList::AddNewClip()
{ 
    ControlClip clip;
    PickChildName( "Clip", this, &clip );
    m_Clips.push_back( clip ); 
} // ControlClipList::AddNewClip

bool ControlClipList::AddChild( IReflected* pChild )
{ 
    ControlClip* pClip = dynamic_cast<ControlClip*>( pChild );
    if (!pClip) return false;
    m_Clips.push_back( *pClip ); 
    return true;
} // ControlClipList::AddChild

int ControlClipList::NumChildren() const
{
    return m_Clips.size();
}

/*****************************************************************************/
/*  ControlClipTrack implementation
/*****************************************************************************/
void ControlClipTrack::Expose( PropertyMap& pm )
{
    pm.start( "ControlClipTrack", this );
    pm.p( "NodeType", ClassName );
    pm.p( "Name", GetName, SetName );
} // ControlClipTrack::Expose

void ControlClipTrack::Serialize( OutStream& os ) const
{
    os << m_Name << m_StartTime << m_PlayTime;
}

void ControlClipTrack::Unserialize( InStream& is )
{
    is >> m_Name >> m_StartTime >> m_PlayTime;
}

InStream& operator >>( InStream& is, ControlClipTrack* &pTrack )
{
    std::string cName;
    is >> cName;
    pTrack = (ControlClipTrack*)ObjectFactory::instance().Create( cName.c_str() );
    if (pTrack) pTrack->Unserialize( is );
    return is;
}

OutStream& operator <<( OutStream& os, const ControlClipTrack* pTrack )
{
    if (!pTrack) return os;
    std::string cName = pTrack->ClassName();
    os << cName;
    pTrack->Serialize( os );
    return os;
}

IMPLEMENT_CLASS(AnimTrack);
IMPLEMENT_CLASS(BindEvent);
IMPLEMENT_CLASS(EffectTrack);
IMPLEMENT_CLASS(BonePosTrack);
IMPLEMENT_CLASS(BoneDirTrack);
IMPLEMENT_CLASS(ClipRefTrack);

/*****************************************************************************/
/*  AnimTrack implementation
/*****************************************************************************/
AnimTrack::AnimTrack()
{
    m_StartTime  = 0.0f;
    m_PlayTime   = 0.0f;
    m_PlaySpeed  = 1.0f;
    m_Phase      = 0.0f;
    m_LoopMode   = lmNone;
    m_EaseIn     = 0.5f;
} // AnimTrack::AnimTrack

void AnimTrack::Expose( PropertyMap& pm )
{
    pm.start<ControlClipTrack>( "AnimTrack", this );
    pm.p( "AnimRef",   GetAnimFile, SetAnimFile, "file" );
    pm.f( "StartTime",  m_StartTime     );
    pm.f( "PlayTime",   m_PlayTime      );
    pm.f( "PlaySpeed",  m_PlaySpeed     );
    pm.f( "Phase",      m_Phase         );
    pm.f( "LoopMode",   m_LoopMode      );
    pm.f( "EaseIn",     m_EaseIn        );
    
} // AnimTrack::Expose

void AnimTrack::Serialize( OutStream& os ) const
{
    ControlClipTrack::Serialize( os );
}

void AnimTrack::Unserialize( InStream& is )
{
    ControlClipTrack::Unserialize( is );
}

/*****************************************************************************/
/*  BindEvent implementation
/*****************************************************************************/
void BindEvent::Expose( PropertyMap& pm )
{
    pm.start<ControlClipTrack>( "BindEvent", this );
} // BindEvent::Expose

void BindEvent::Serialize( OutStream& os ) const
{
    ControlClipTrack::Serialize( os );
}

void BindEvent::Unserialize( InStream& is )
{
    ControlClipTrack::Unserialize( is );
}

/*****************************************************************************/
/*  EffectTrack implementation
/*****************************************************************************/
void EffectTrack::Expose( PropertyMap& pm )
{
    pm.start<ControlClipTrack>( "EffectTrack", this );
} // EffectTrack::Expose

void EffectTrack::Serialize( OutStream& os ) const
{
    ControlClipTrack::Serialize( os );
}

void EffectTrack::Unserialize( InStream& is )
{
    ControlClipTrack::Unserialize( is );
}

/*****************************************************************************/
/*  BonePosTrack implementation
/*****************************************************************************/
void BonePosTrack::Expose( PropertyMap& pm )
{
    pm.start<ControlClipTrack>( "BonePosTrack", this );
} // BonePosTrack::Expose

void BonePosTrack::Serialize( OutStream& os ) const
{
    ControlClipTrack::Serialize( os );
}

void BonePosTrack::Unserialize( InStream& is )
{
    ControlClipTrack::Unserialize( is );
}

/*****************************************************************************/
/*  BoneDirTrack implementation
/*****************************************************************************/
void BoneDirTrack::Expose( PropertyMap& pm )
{
    pm.start<ControlClipTrack>( "BoneDirTrack", this );
} // BoneDirTrack::Expose

void BoneDirTrack::Serialize( OutStream& os ) const
{
    ControlClipTrack::Serialize( os );
}

void BoneDirTrack::Unserialize( InStream& is )
{
    ControlClipTrack::Unserialize( is );
}

/*****************************************************************************/
/*  ClipRefTrack implementation
/*****************************************************************************/
void ClipRefTrack::Expose( PropertyMap& pm )
{
    pm.start<ControlClipTrack>( "ClipRefTrack", this );
} // ClipRefTrack::Expose

void ClipRefTrack::Serialize( OutStream& os ) const
{
    ControlClipTrack::Serialize( os );
}

void ClipRefTrack::Unserialize( InStream& is )
{
    ControlClipTrack::Unserialize( is );
}