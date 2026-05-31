/*****************************************************************************/
/*    File:    sndSoundTrack.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgController.h"
#include "kIO.h"
#include "kIOHelpers.h"
#include "ISoundSystem.h"

#include "sndSoundTrack.h"

IMPLEMENT_CLASS( SoundTrack );

/*****************************************************************************/
/*    SoundTrack implementation
/*****************************************************************************/
SoundTrack::SoundTrack() : bMuted( false ), bLooped( true ), volume( 1.0f )
{
    totalTime = curTime = 0.0f;
    smpID = 0;
}

SoundTrack::~SoundTrack()
{
}

int    SoundTrack::GetSampleID()
{
    if (smpID == 0)
    {
        ISoundSystem* pSystem = GetSoundSystem();
        if (!pSystem) return 0;
        smpID = pSystem->GetSampleID( GetName() );
    }
    return smpID;
} // SoundTrack::GetSampleID

void SoundTrack::Render()
{
    ISoundSystem* pSystem = GetSoundSystem();
    if (!pSystem) return;
    if (!bMuted && !GetFlagState( nfInvisible ))
    {
        if (pSystem->GetStatus( GetSampleID() ) == sndIdle)
        {
            pSystem->Play( GetSampleID(), 0.0f, bLooped );
        }
    }
    else
    {
        if (pSystem->GetStatus( GetSampleID() ) == sndBeingPlayed)
        {
            pSystem->Stop( GetSampleID() );
        }
    }
} // SoundTrack::Render

void SoundTrack::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << bMuted << volume;
} // SoundTrack::Serialize

void SoundTrack::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> bMuted >> volume;
} // SoundTrack::Unserialize

void SoundTrack::SetVolume( float _volume )
{
    ISoundSystem* pSystem = GetSoundSystem();
    if (!pSystem) return;
    if (pSystem->SetVolume( GetSampleID(), _volume ))
    {
        volume = _volume;
    }
} // SoundTrack::SetVolume

void SoundTrack::SetCurrentTime( float _curTime )
{
    ISoundSystem* pSystem = GetSoundSystem();
    if (!pSystem) return;
    if (pSystem->Play( GetSampleID(), _curTime, bLooped ))
    {
        curTime = _curTime;
    }
} // SoundTrack::SetVolume


