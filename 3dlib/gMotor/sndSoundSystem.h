/*****************************************************************************/
/*    File:    sndSoundSystem.h
/*    Desc:    Sound manager implementation
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-22-2003
/*****************************************************************************/
#ifndef __SND_SOUNDSYSTEM_H__
#define __SND_SOUNDSYSTEM_H__

#include "ISoundSystem.h"
#include "kHash.hpp"
#include "kResource.h"

class SoundPlayer;
/*****************************************************************************/
/*    Class:    SoundSample
/*    Desc:    Single sound sample
/*****************************************************************************/
class SoundSample : public BaseResource
{
    SoundPlayer*            pPlayer;

    c2::string                fName;
    SndSampleStatus            status;
    DWORD                    fileSize;

public:
                            SoundSample();
                            ~SoundSample();

    virtual int                GetDismissableSizeBytes() const;
    virtual int                GetHeaderSizeBytes() const;

    virtual bool            InitPrefix();
    virtual void            Dismiss();
    virtual bool            Restore();

    const char*                GetFileName() const { return fName; }
    void                    SetFileName( const char* fileName ) { fName = fileName; }

    SndSampleStatus            GetStatus() const { return status; }
    void                    SetStatus( SndSampleStatus _status ) { status = _status; }

    void                    SetFileSize( DWORD size ) { fileSize = size; }
    int                        GetFileSize() const { return fileSize; }

    void                    SetPlayer( SoundPlayer* _pPlayer ) { pPlayer = _pPlayer; }
    SoundPlayer*            GetPlayer() const { return pPlayer; }

}; // class SoundSample

/*****************************************************************************/
/*    Class:    SoundPlayer
/*    Desc:    Sound player interface
/*****************************************************************************/
class SoundPlayer
{
    c2::string                name;
public:
                            SoundPlayer( const char* _name );

    virtual bool            Init        ( HWND    hWnd );
    virtual bool            Shut        ();
    virtual bool            CanPlay        ( const char* fileName );
    virtual    bool            InitSample    ( const char* fileName, SoundSample& sample );
    virtual bool            Play        ( SoundSample& sample, bool loop = false );
    virtual bool            Stop        ( SoundSample& sample );

}; // class SoundPlayer

const int c_SoundPoolSize = 10485760; // 10 Mb
/*****************************************************************************/
/*    Class:    SoundSystem, singleton
/*    Desc:    Sound manager implementation
/*****************************************************************************/
class SoundSystem : public ISoundSystem
{
public:
                            ~SoundSystem();

    virtual int                GetSampleID    ( const char* fileName );
    virtual bool            Init        ( HWND _hWnd );
    virtual bool            Play         ( int smpID, float time, bool bLoop = false );
    virtual bool            Stop         ( int smpID );
    virtual bool            Pause         ( int smpID, bool bPause = true );
    virtual bool            SetVolume    ( int smpID, float volume );
    virtual float            GetVolume    ( int smpID );
    virtual float            GetLength    ( int smpID );
    virtual SndSampleStatus GetStatus    ( int smpID );


    static SoundSystem&        instance();
    void                    RegisterPlayer( SoundPlayer* pPlayer );

private:
                            SoundSystem();

    DynamicResMgr<SoundSample, c2::string>        storage;
    c2::array<SoundPlayer*>                        players;
    

    HWND                                        hWnd;

}; // class SoundSystem

#endif // __SND_SOUNDSYSTEM_H__
