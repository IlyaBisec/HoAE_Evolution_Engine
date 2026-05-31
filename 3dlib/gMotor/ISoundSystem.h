/*****************************************************************************/
/*    File:    ISoundSystem.h
/*    Desc:    Sound playing device abstract interface
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-22-2003
/*****************************************************************************/
#ifndef __ISOUNDSYSTEM_H__
#define __ISOUNDSYSTEM_H__

/*****************************************************************************/
/*    Enum:    SndSampleStatus
/*    Desc:    Status of the sound sample in the sound manager
/*****************************************************************************/
enum SndSampleStatus
{
    sndUnknown          = 0,
    sndBeingPlayed      = 1,
    sndIdle             = 2,
    sndPaused           = 3,
    andError            = 4
}; // enum SndSampleStatus

/*****************************************************************************/
/*    Class:    ISoundSystem, abstract interface
/*    Desc:    Sound manager interface
/*****************************************************************************/
class ISoundSystem
{
public:
    virtual int             GetSampleID  ( const char* fileName )                       = 0;
    virtual bool            Init         ( HWND _hWnd )                                 = 0;
    virtual bool            Play         ( int smpID, float time, bool bLoop = false )  = 0;
    virtual bool            Stop         ( int smpID )                                  = 0;
    virtual bool            Pause        ( int smpID, bool bPause = true )              = 0;
    virtual bool            SetVolume    ( int smpID, float volume )                    = 0;
    virtual float           GetVolume    ( int smpID )                                  = 0;
    virtual float           GetLength    ( int smpID )                                  = 0;
    virtual SndSampleStatus GetStatus    ( int smpID )                                  = 0;

}; // class ISoundSystem

ISoundSystem* GetSoundSystem();

#endif // __ISOUNDSYSTEM_H__