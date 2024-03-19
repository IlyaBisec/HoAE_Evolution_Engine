/*****************************************************************************/
/*    File:    sndSoundTrack.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#ifndef __SNDSOUNDTRACK_H__
#define __SNDSOUNDTRACK_H__


/*****************************************************************************/
/*    Class:    SoundTrack
/*    Desc:    Sound track playback animation
/*****************************************************************************/
class SoundTrack : public Animation
{
    bool                    bMuted;
    float                    volume;
    bool                    bLooped;

    float                    totalTime;
    float                    curTime;
    
    int                        smpID;

public:
                            SoundTrack();
    virtual                    ~SoundTrack();

    virtual void            Render();

    bool                    GetIsMuted() const { return bMuted; }
    void                    SetIsMuted( bool _muted ) { bMuted = _muted; }

    bool                    GetIsLooped() const { return bLooped; }
    void                    SetIsLooped( bool _looped ) { bLooped = _looped; }

    void                    SetVolume( float _volume );
    float                    GetVolume() const { return volume; }

    void                    SetCurrentTime( float _curTime );
    float                    GetCurrentTime() const { return curTime; }
    float                    GetTotalTime() const { return totalTime; }

    virtual void            Serialize( OutStream& os ) const;
    virtual void            Unserialize( InStream& is );

    DECLARE_SCLASS(SoundTrack,Animation,SNDT);

protected:
    int                        GetSampleID();

}; // class SoundTrack



#endif // __SNDSOUNDTRACK_H__
