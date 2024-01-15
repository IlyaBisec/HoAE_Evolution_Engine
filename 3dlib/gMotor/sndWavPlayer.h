/*****************************************************************************/
/*    File:    sndWavPlayer.h
/*    Desc:    Sound routines
/*    Author:    Ruslan Shestopalyuk
/*    Date:    16.08.2002
/*****************************************************************************/
#ifndef __GUI_SOUND_H__
#define __GUI_SOUND_H__
#pragma once

#include <mmsystem.h>
#include "dsound.h"
#include "sndSoundSystem.h"

class WavSample;
const int c_MaxStaticWaveSize = 20971520;

/*****************************************************************************/
/*    Class:    WavPlayer
/*    Desc:    Sound manager class
/*****************************************************************************/
class WavPlayer : public SoundPlayer
{
public:
                    WavPlayer();
                    ~WavPlayer();

    virtual bool    Init( HWND hWnd );
    virtual bool    Shut();

    bool            CanPlay    ( const char* fileName );
    bool            Play    ( SoundSample& sample, bool loop = false );
    bool            Stop    ( SoundSample& sample );

protected:
    int                CreateSoundBuffer( WavSample* wave, bool staticBuf = true );
    bool            CopyWaveToBuffer( int bufID, WavSample* wave );

    bool            Play    ( const char* fileName, bool loop = false );
    bool            Stop    ( const char* fileName );

private:
    IDirectSound*                    pDXSound;
    vector<IDirectSoundBuffer*>        buffers;
    vector<int>                        bufSizes;
    vector<WavSample*>                waves;

    HWND                            hWindow;
    map<string, int>                sndReg;
    CRITICAL_SECTION                soundCritical;

    friend class                    WavSample;
}; // class WavPlayer

/*****************************************************************************/
/*    Class:    WavSample
/*    Desc:    
/*****************************************************************************/
class WavSample
{
public:
                    WavSample();
                    ~WavSample();

    int                        GetWaveSize() const;
    const WAVEFORMATEX&        GetFormat() const { return format; }

protected:
    bool                    PreLoadWaveFile( const char* fname );
    int                        ReadNextChunk( BYTE* buf, int numBytes );
    bool                    CreateMapping( const char* fname );
    bool                    CloseMapping();

private:
    WAVEFORMATEX    format;

    BYTE*            hWave;
    int                waveSize;
    int                fileDataBeg;
    int                curPos;

    friend class    WavPlayer;
}; // class WavSample

extern WavPlayer g_WavPlayer;

#endif // __GUI_SOUND_H__