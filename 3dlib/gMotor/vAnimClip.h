/*****************************************************************************/
/*	File:	vAnimClip.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#ifndef __VANIMCLIP_H__
#define __VANIMCLIP_H__

#include "IReflected.h"


/*****************************************************************************/
/*  Struct: AnimRef
/*  Desc:   Reference to the model animation file
/*****************************************************************************/
struct AnimRef : public IReflected 
{
    std::string             m_Name;             //  name of the animation file
    int                     m_AnimID;

    AnimRef             () : m_AnimID(-1) {}
    void                    SetName             ( const char* name  ) { m_Name = name; }
    virtual const char*     GetName             () const { return m_Name.c_str(); }
    virtual const char*     ClassName           () const { return "AnimRef"; }

    virtual void		    Expose              ( PropertyMap& pm   );
    void                    Play                ();

    friend inline InStream& operator >>( InStream& is, AnimRef& val );
    friend inline OutStream& operator <<( OutStream& os, const AnimRef& val );
}; // class AnimRef

inline InStream& operator >>( InStream& is, AnimRef& val )
{
    is >> val.m_Name;
    return is;
} 

inline OutStream& operator <<( OutStream& os, const AnimRef& val )
{
    os << val.m_Name;
    return os;
}

/*****************************************************************************/
/*  Class:  AnimRefList
/*****************************************************************************/
class AnimRefList : public IReflected, public std::vector<AnimRef>
{
public:
    virtual void		    Expose              ( PropertyMap& pm );    

    virtual int             NumChildren         () const                { return size(); }
    virtual IReflected*      Child               ( int idx ) const       
    { 
        if (idx < 0 || idx >= size()) return NULL;
        return (IReflected*)&at( idx ); 
    }
    virtual const char*     ClassName           () const                { return "AnimRefList"; }
    virtual const char*     GetName             () const                { return "Animations"; }

    virtual bool            AddChild            ( IReflected* pChild )   { return false; }
    virtual bool            DelChild            ( int idx )             { return false; }

    void                    AddNewAnimation     ();
}; // class AnimRefList

/*****************************************************************************/
/*  Class: ControlClipTrack
/*  Desc:   Reference to the model animation file
/*****************************************************************************/
class ControlClipTrack : public IReflected
{
protected:
    std::string             m_Name;             //  name of the track
    float                   m_StartTime; 
    float                   m_PlayTime; 

public:
                            ControlClipTrack       () {}
    void                    SetName             ( const char* name  ) { m_Name = name; }
    virtual const char*     GetName             () const { return m_Name.c_str(); }
    virtual const char*     ClassName           () const { return "ControlClipTrack"; }
    virtual void		    Expose              ( PropertyMap& pm );
    virtual void            Serialize           ( OutStream& os ) const;
    virtual void            Unserialize         ( InStream& is );

}; // class ControlClipTrack

InStream& operator >>( InStream& is, ControlClipTrack* &pTrack );
OutStream& operator <<( OutStream& os, const ControlClipTrack* pTrack );

/*****************************************************************************/
/*  Class:  ControlClip
/*****************************************************************************/
class ControlClip : public IReflected
{
    std::vector<ControlClipTrack*>   m_Tracks;
    std::string                   m_Name;             //  name of the clip

public:
    virtual void		    Expose              ( PropertyMap& pm );    

    virtual int             NumChildren         () const                { return m_Tracks.size(); }
    virtual IReflected*      Child               ( int idx ) const;       
    virtual const char*     ClassName           () const                { return "ControlClip"; }
    void                    SetName             ( const char* name  ) { m_Name = name; }
    virtual const char*     GetName             () const { return m_Name.c_str(); }

    virtual bool            AddChild            ( IReflected* pChild )   { return false; }
    virtual bool            DelChild            ( int idx )             { return false; }

    float                   GetTotalTime        () const;
    float                   GetCurTime          () const;
    void                    SetCurTime          ( float t );
    void                    Play                ();
    void                    Pause               ();
    void                    Stop                ();

    void                    AddAnimTrack        ();
    void                    AddBindEvent        ();
    void                    AddEffectTrack      ();
    void                    AddBonePosTrack     ();
    void                    AddBoneDirTrack     ();
    void                    AddClipRef          ();

    friend inline InStream& operator >>( InStream& is, ControlClip& val );
    friend inline OutStream& operator <<( OutStream& os, const ControlClip& val );
}; // class ControlClip

inline InStream& operator >>( InStream& is, ControlClip& val )
{
    is >> val.m_Name >> val.m_Tracks;
    return is;
} 

inline OutStream& operator <<( OutStream& os, const ControlClip& val )
{
    os << val.m_Name << val.m_Tracks;
    return os;
}

/*****************************************************************************/
/*  Class:  ControlClipList
/*****************************************************************************/
class ControlClipList : public IReflected
{
    std::vector<ControlClip>                m_Clips;

public:
    virtual void		    Expose       ( PropertyMap& pm );    

    virtual int             NumChildren  () const;
    virtual IReflected*      Child        ( int idx ) const;       
    virtual const char*     ClassName    () const                { return "ControlClipList"; }
    virtual const char*     GetName      () const                { return "Clips"; }
    virtual bool            AddChild     ( IReflected* pChild );
    virtual bool            DelChild     ( int idx )             { return false; }

    void                    AddNewClip   ();

    friend inline OutStream& operator <<( OutStream& os, ControlClipList& val );
    friend inline InStream& operator >>( InStream& is, ControlClipList& val );
}; // class ControlClipList

inline InStream& operator >>( InStream& is, ControlClipList& val )
{
    is >> val.m_Clips;
    return is;
} 

inline OutStream& operator <<( OutStream& os, ControlClipList& val )
{
    os << val.m_Clips;
    return os;
}

/*****************************************************************************/
/*  Class:  AnimTrack
/*  Desc:   Clip track, playing model animation
/*****************************************************************************/
class AnimTrack : public ControlClipTrack
{
    float                   m_PlaySpeed; 
    float                   m_Phase;     
    LoopMode                m_LoopMode;  
    float                   m_EaseIn;    
    std::string             m_AnimFile;

public:
                            AnimTrack   ();
    virtual void		    Expose      ( PropertyMap& pm );  

    const char*             GetAnimFile() const { return m_AnimFile.c_str(); }
    void                    SetAnimFile( const char* file ) { m_AnimFile = file; }

    virtual void            Serialize  ( OutStream& os ) const;
    virtual void            Unserialize( InStream& is );

    DECLARE_CLASS(AnimTrack);
}; // class AnimTrack

/*****************************************************************************/
/*  Class:  BindEvent
/*  Desc:   Clip event, binding some object to model slot
/*****************************************************************************/
class BindEvent : public ControlClipTrack
{
    std::string             m_SlotName;     //  name of the slot to bind to
    std::string             m_ModelName;    //  name of the model to bind

public:
    virtual void		    Expose       ( PropertyMap& pm );  
    virtual void            Serialize    ( OutStream& os ) const;
    virtual void            Unserialize  ( InStream& is );

    DECLARE_CLASS(BindEvent);
}; // class BindEvent

/*****************************************************************************/
/*  Class:  EffectTrack
/*  Desc:   Clip track, playing particle effect
/*****************************************************************************/
class EffectTrack : public ControlClipTrack
{
    std::string             m_EffectName;
    std::string             m_BoneName;
    float                   m_PlaySpeed; 
    float                   m_Phase;     
    LoopMode                m_LoopMode;  
    float                   m_EaseIn; 

public:
    virtual void		    Expose       ( PropertyMap& pm );
    virtual void            Serialize    ( OutStream& os ) const;
    virtual void            Unserialize  ( InStream& is );

    DECLARE_CLASS(EffectTrack);
}; // class EffectTrack

/*****************************************************************************/
/*  Class:  BonePosTrack
/*  Desc:   
/*****************************************************************************/
class BonePosTrack : public ControlClipTrack
{
    std::string             m_BoneName;
    Vector3D                m_TargetPos;
public:

    virtual void		    Expose       ( PropertyMap& pm );  
    virtual void            Serialize    ( OutStream& os ) const;
    virtual void            Unserialize  ( InStream& is );
    DECLARE_CLASS(BonePosTrack);
}; // class BonePosTrack

/*****************************************************************************/
/*  Class:  BoneDirTrack
/*  Desc:   
/*****************************************************************************/
class BoneDirTrack : public ControlClipTrack
{
    std::string             m_BoneName;
    float                   m_TargetDir;
public:
    virtual void		    Expose       ( PropertyMap& pm );
    virtual void            Serialize    ( OutStream& os ) const;
    virtual void            Unserialize  ( InStream& is );
    DECLARE_CLASS(BoneDirTrack);
}; // class BoneDirTrack

/*****************************************************************************/
/*  Class:  ClipRefTrack
/*  Desc:   Reference to another clip 
/*****************************************************************************/
class ClipRefTrack : public ControlClipTrack
{
    std::string             m_ClipName;

public:
    virtual void		    Expose       ( PropertyMap& pm );   
    virtual void            Serialize    ( OutStream& os ) const;
    virtual void            Unserialize  ( InStream& is );
    DECLARE_CLASS(ClipRefTrack);
}; // class ClipRefTrack

#endif // __VANIMCLIP_H__