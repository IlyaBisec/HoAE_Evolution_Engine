/*****************************************************************************/
/*    File:    sndSoundSystem.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-22-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kArray.hpp"
#include "sndSoundSystem.h"

#include <mmsystem.h>
#include "dsound.h"
#include "sndWavPlayer.h"


ISoundSystem* GetSoundSystem()
{
    return &(SoundSystem::instance());
}

/*****************************************************************************/
/*    SoundSample implementation
/*****************************************************************************/
SoundSample::SoundSample()
{

}

SoundSample::~SoundSample()
{

}

int    SoundSample::GetDismissableSizeBytes() const
{
    return 0;
}

int    SoundSample::GetHeaderSizeBytes() const
{
    return 0;
}

bool SoundSample::InitPrefix()
{
    return false;
}

void SoundSample::Dismiss()
{
}

bool SoundSample::Restore()
{
    return false; 
}

/*****************************************************************************/
/*    SoundPlayer implementation
/*****************************************************************************/
SoundPlayer::SoundPlayer( const char* _name )
{
    name = _name;
    SoundSystem::instance().RegisterPlayer( this );
} // SoundPlayer::SoundPlayer

bool SoundPlayer::Init( HWND hWnd )
{
    return false;
}

bool SoundPlayer::Shut()
{
    return false;
}

bool SoundPlayer::CanPlay( const char* fileName )
{
    return false;
}

bool SoundPlayer::InitSample( const char* fileName, SoundSample& sample )
{
    if (!CanPlay( fileName )) return false;
    
    FILE* fp = fopen( fileName, "rb" );
    if (!fp) return false;
    
    fseek( fp, 0, SEEK_END );
    DWORD fsize = ftell( fp );
    fclose( fp );

    sample.SetFileSize    ( fsize        );
    sample.SetStatus    ( sndIdle    );
    sample.SetFileName    ( fileName    );
    sample.SetPlayer    ( this        );
    return true;
}

bool SoundPlayer::Play( SoundSample& sample, bool loop )
{
    return false;
}

bool SoundPlayer::Stop( SoundSample& sample )
{
    return false;
}

/*****************************************************************************/
/*    SoundSystem implementation
/*****************************************************************************/
SoundSystem::SoundSystem()
{
    storage.SetMemoryBudget( 52428800 );
}

SoundSystem::~SoundSystem()
{
}

SoundSystem& SoundSystem::instance()
{
    static SoundSystem me;
    return me;
} // SoundSystem::instance

void SoundSystem::RegisterPlayer( SoundPlayer* pPlayer )
{
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i] == pPlayer) return;
    }
    players.push_back( pPlayer );
} // SoundSystem::RegisterPlayer

bool SoundSystem::Init( HWND _hWnd )
{
    hWnd = _hWnd;
    bool allOK = true;

    for (int i = 0; i < players.size(); i++)
    {
        allOK &= players[i]->Init( hWnd );
    }

    storage.SetMemoryBudget( c_SoundPoolSize );

    return allOK;
} // SoundSystem::Init

int    SoundSystem::GetSampleID( const char* fileName )
{
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i]->CanPlay( fileName ))
        {
            SoundSample* pSample = new SoundSample();
            if (players[i]->InitSample( fileName, *pSample ))
            {
                int resID = storage.InsertResource( pSample, c2::string( fileName ) );
                return resID + 1;
            }
            delete pSample;
        }
    }
    return 0;
} // SoundSystem::GetSampleID

bool SoundSystem::Play( int smpID, float time, bool bLoop )
{
    if (smpID == 0) return sndUnknown;
    SoundSample* pSample = storage.GetResource( smpID - 1 );
    if (pSample)
    {
        if (pSample->GetPlayer()->Play( *pSample, bLoop ))
        {
            pSample->SetStatus( sndBeingPlayed );
            return true;
        }
    }
    return false;
}

bool SoundSystem::Stop( int smpID )
{
    if (smpID == 0) return sndUnknown;
    SoundSample* pSample = storage.GetResource( smpID - 1 );
    if (pSample)
    {
        if (pSample->GetPlayer()->Stop( *pSample ))
        {
            pSample->SetStatus( sndIdle );
            return true;
        }
    }
    return false;
}

bool SoundSystem::Pause( int smpID, bool bPause )
{
    return false;
}

bool SoundSystem::SetVolume( int smpID, float volume )
{
    return false;
}

float SoundSystem::GetVolume( int smpID )
{
    return 0.0f;
}

float SoundSystem::GetLength( int smpID )
{
    return 0.0f;
}

SndSampleStatus SoundSystem::GetStatus( int smpID )
{
    if (smpID == 0) return sndUnknown;
    SoundSample* pSample = storage.GetResource( smpID - 1 );
    if (pSample)
    {
        return pSample->GetStatus();
    }
    return sndUnknown;
}
