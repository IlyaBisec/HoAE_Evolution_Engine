/*****************************************************************************/
/*    File:    sndWavPlayer.cpp
/*    Desc:    Sound routines
/*    Author:    Ruslan Shestopalyuk
/*    Date:    16.08.2002
/*****************************************************************************/
#include "stdafx.h"
#include <mmsystem.h>
#include "dsound.h"
#include "sndSoundSystem.h"
#include "sndWavPlayer.h"

/*****************************************************************************/
/*    WavPlayer implementation
/*****************************************************************************/
WavPlayer::WavPlayer() : SoundPlayer( "DirectX WAV Player" )
{
    pDXSound = 0;
} // WavPlayer::WavPlayer

WavPlayer::~WavPlayer()
{
    Shut();
} // WavPlayer::~WavPlayer

/*---------------------------------------------------------------------------*/
/*    Func:    WavPlayer::init
/*    Desc:    initializes sound device    
/*---------------------------------------------------------------------------*/
bool WavPlayer::Init( HWND hWnd )
{
    hWindow = hWnd;
    InitializeCriticalSection( &soundCritical );
    HRESULT result = DirectSoundCreate(NULL, &pDXSound, NULL);
    if (result != DS_OK) return false;
    
    result = pDXSound->SetCooperativeLevel( hWnd, DSSCL_EXCLUSIVE );
    if (result != DS_OK) return false;

    return true;
} // WavPlayer::init

/*---------------------------------------------------------------------------*/
/*    Func:    WavPlayer::CreateSoundBuffer
/*    Desc:    creates secondary sound buffer for wave
/*---------------------------------------------------------------------------*/
int    WavPlayer::CreateSoundBuffer( WavSample* wave, bool staticBuf )
{
    if (!pDXSound) return -1;
    DSBUFFERDESC    bufDesc;
    HRESULT            hres;
    
    const WAVEFORMATEX& format = wave->GetFormat();
    memset( &bufDesc, 0, sizeof(DSBUFFERDESC) );

    bufDesc.dwSize            = sizeof( DSBUFFERDESC );

    bufDesc.dwFlags            = DSBCAPS_CTRLVOLUME;
    if (staticBuf) bufDesc.dwFlags |= DSBCAPS_STATIC;
    
    bufDesc.dwBufferBytes    = wave->GetWaveSize();
    bufDesc.lpwfxFormat        = (WAVEFORMATEX*) &format;

    IDirectSoundBuffer* sbuf;
    hres = pDXSound->CreateSoundBuffer( &bufDesc, &sbuf, NULL );
    if (hres != DS_OK) return -1;

    buffers.push_back( sbuf );
    bufSizes.push_back( bufDesc.dwBufferBytes );
    
    return buffers.size() - 1;
} // WavPlayer::createSoundBuffer

/*---------------------------------------------------------------------------*/
/*    Func:    WavPlayer::CopyWaveToBuffer
/*---------------------------------------------------------------------------*/
bool WavPlayer::CopyWaveToBuffer( int bufID, WavSample* wave )
{
    if (bufID < 0 || bufID >= buffers.size()) return false;
    void*        pBlock1;
    void*        pBlock2;
    DWORD        b1Bytes;
    DWORD        b2Bytes;
    HRESULT        hres;

    IDirectSoundBuffer* pBuf = buffers[bufID];

    hres = pBuf->Lock( 0, bufSizes[bufID], &pBlock1, &b1Bytes, &pBlock2, &b2Bytes, 0 );
    if (hres == DSERR_BUFFERLOST)
    {
        pBuf->Restore();
        hres = pBuf->Lock( 0, bufSizes[bufID], &pBlock1, &b1Bytes, &pBlock2, &b2Bytes, 0 );
    }
    if (hres != DS_OK) return false;
    wave->ReadNextChunk( (BYTE*)pBlock1, b1Bytes );
    pBuf->Unlock( pBlock1, b1Bytes, pBlock2, b2Bytes);

    return true;
} // WavPlayer::copyWaveToBuffer

/*---------------------------------------------------------------------------*/
/*    Func:    WavPlayer::play    
/*    Desc:    begins playing sound with given ID
/*---------------------------------------------------------------------------*/
bool WavPlayer::Play( const char* fileName, bool loop )
{
    if (!pDXSound) return false;

    EnterCriticalSection( &soundCritical );

    string sID = string( fileName );
    int sNum = 0;
    map<string, int>::iterator it = sndReg.find( sID );
    if (it == sndReg.end())
    //  no such sound
    {
        WavSample*    newWave = new WavSample();
        if (!newWave->PreLoadWaveFile( fileName )) 
        {
            LeaveCriticalSection( &soundCritical );
            return false;
        }
        waves.push_back( newWave );
        bool stBuf = true;
        if (newWave->GetWaveSize() > c_MaxStaticWaveSize) stBuf = false;
        CreateSoundBuffer( newWave, stBuf );
        sNum = waves.size() - 1;
        sndReg[sID] = sNum;
    }
    else sNum = (*it).second;

    if (sNum >= 0 && sNum < waves.size()) 
    {
        DWORD flags = 0;
        if (loop) flags |= DSBPLAY_LOOPING;
        CopyWaveToBuffer( sNum, waves[sNum] );
        buffers[sNum]->SetCurrentPosition( 0 );
        buffers[sNum]->Play( 0, 0 , flags );
    }

    LeaveCriticalSection( &soundCritical );
    return true;
} // WavPlayer::Play

/*---------------------------------------------------------------------------*/
/*    Func:    WavPlayer::Stop
/*    Desc:    stops playing sound with given ID
/*---------------------------------------------------------------------------*/
bool WavPlayer::Stop( const char* fileName )
{
    if (!pDXSound) return false;
    
    string sID = string( fileName );
    map<string, int>::iterator it = sndReg.find( sID );
    if (it == sndReg.end()) return false;
    int sNum = (*it).second;
    buffers[sNum]->Stop();
    return true;
} // WavPlayer::stop

bool WavPlayer::CanPlay( const char* fileName )
{
    char ext    [_MAX_EXT    ];
    char dir    [_MAX_DIR    ];
    char fname    [_MAX_FNAME    ];
    char drive    [_MAX_DRIVE    ];

    _splitpath( fileName, drive, dir, fname, ext );

    char* pExt = ext;
    if (*pExt == '.') pExt++;
    if (!stricmp( pExt, "wav" )) return true;
    return false;
} // WavPlayer::CanPlay

bool WavPlayer::Play( SoundSample& sample, bool loop )
{
    return Play( sample.GetFileName(), loop );
}

bool WavPlayer::Stop( SoundSample& sample )
{
    return Stop( sample.GetFileName() );
}

/*---------------------------------------------------------------------------*/
/*    Func:    WavPlayer::Shut
/*    Desc:    shuts down sound device    
/*---------------------------------------------------------------------------*/
bool WavPlayer::Shut()
{
    if (!pDXSound) return false;
    
    //  release all created secondary buffers
    for (int i = 0; i < buffers.size(); i++)
    {
        IDirectSoundBuffer* sbuf = buffers[i];
        sbuf->Release();
    }
    buffers.clear();

    for (int i = 0; i < waves.size(); i++)
    {
        delete waves[i];
    }
    waves.clear();
    bufSizes.clear();

    if (pDXSound)
    {
        pDXSound->Release();
        pDXSound = 0;
    }

    DeleteCriticalSection( &soundCritical );

    return true;
} // WavPlayer::init

/*****************************************************************************/
/*    WavSample implementation
/*****************************************************************************/
WavSample::WavSample()
{
    hWave        = 0;
    waveSize    = 0; 
    fileDataBeg    = 0;
    curPos        = 0;
}

WavSample::~WavSample()
{
    CloseMapping();
}

int    WavSample::GetWaveSize() const
{
    if (hWave) return waveSize; else return 0;
} // WavSample::GetWaveSize

/*---------------------------------------------------------------------------*/
/*    Func:    WavSample::PreLoadWaveFile
/*    Desc:    pre - loads data header from .wav file, maps file into memory
/*---------------------------------------------------------------------------*/
bool WavSample::PreLoadWaveFile( const char* fname )
{
    MMCKINFO mmCkInfoRIFF;
    MMCKINFO mmCkInfoChunk;
    MMRESULT result;
    HMMIO hMMIO;
    long bytesRead;

    // open the wave file
    hMMIO = mmioOpen( (char*) fname, NULL, MMIO_READ | MMIO_ALLOCBUF );
    if (hMMIO == NULL) return false;

    // descend into the RIFF chunk
    mmCkInfoRIFF.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );
    result = mmioDescend( hMMIO, &mmCkInfoRIFF, NULL, MMIO_FINDRIFF );
    if (result != MMSYSERR_NOERROR) return false;

    // descend into the format chunk
    mmCkInfoChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
    result = mmioDescend( hMMIO, &mmCkInfoChunk, &mmCkInfoRIFF, MMIO_FINDCHUNK );
    if (result != MMSYSERR_NOERROR) return false;

    // read the format information into the WAVEFORMATEX structure
    bytesRead = mmioRead( hMMIO, (char*)&format, sizeof(WAVEFORMATEX) );
    if (bytesRead == -1) return false;

    // ascend out of the format chunk
    result = mmioAscend( hMMIO, &mmCkInfoChunk, 0 );
    if (result != MMSYSERR_NOERROR) return false;

    // descend into the data chunk
    mmCkInfoChunk.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
    result = mmioDescend( hMMIO, &mmCkInfoChunk, &mmCkInfoRIFF, MMIO_FINDCHUNK );
    if (result != MMSYSERR_NOERROR) return false;

    // save the size of the wave data
    waveSize = mmCkInfoChunk.cksize;
    mmioClose( hMMIO, 0 );

    CreateMapping( fname );
    curPos = fileDataBeg;

    return true;
} // WavSample::PreLoadWaveFile

/*---------------------------------------------------------------------------*/
/*    Func:    WavSample::ReadNextChunk
/*    Desc:    reads next chunk of wave data from .wav file
/*    Ret:    number of bytes actually read
/*---------------------------------------------------------------------------*/
int WavSample::ReadNextChunk( BYTE* buf, int numBytes )
{
    if (!hWave) return 0;
    if (numBytes <= fileDataBeg + waveSize - curPos)
    {
        CopyMemory( buf, hWave + curPos, numBytes );
        curPos += numBytes;
        if (curPos >= fileDataBeg + waveSize) curPos = fileDataBeg;
        return numBytes;
    }
    else
    {
        int realBytes = fileDataBeg + waveSize - curPos;
        CopyMemory( buf, hWave + curPos, realBytes );
        FillMemory( buf + realBytes, numBytes - realBytes, 0 );
        curPos = fileDataBeg;
        return realBytes;
    }
} // WavSample::ReadNextChunk

/*---------------------------------------------------------------------------*/
/*    Func:    WavSample::CreateMapping
/*    Desc:    maps sound file
/*---------------------------------------------------------------------------*/
bool WavSample::CreateMapping( const char* fname )
{
    if (hWave) CloseMapping();
    
    HANDLE    hFile, hMapped;
    DWORD    fileLen;
    
    hFile = CreateFile( fname, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0 , 0 );
    if (hFile == INVALID_HANDLE_VALUE) return false;
    
    fileLen = GetFileSize( hFile, 0 );
    fileDataBeg = fileLen - waveSize;
    hMapped = CreateFileMapping( hFile, 0, PAGE_READONLY, 0, fileLen, 0 );
    CloseHandle( hFile );

    if (!hMapped) return false;
    
    hWave = (BYTE*)MapViewOfFile( hMapped, FILE_MAP_READ, 0, 0, 0 );
    CloseHandle( hMapped );

    if (!hWave) return false; 

    return true;
} // WavSample::CreateMapping

bool WavSample::CloseMapping()
{
    UnmapViewOfFile( hWave );    
    hWave        = 0;
    waveSize    = 0;
    curPos        = 0;
    fileDataBeg = 0;
    return true;
} // WavSample::CloseMapping

