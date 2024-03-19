/*****************************************************************************/
/*    File:    uiRatchet.h
/*    Desc:    Ratchet control
/*    Author:  Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIRATCHET_H__
#define __UIRATCHET_H__

#include "uiWidget.h"

/*****************************************************************************/
/*    Class:   Ratchet
/*    Desc:    
/*****************************************************************************/    
class Ratchet : public Widget
{
    float           m_Position;
    float           m_PrevPosition;

    float           m_Frame;
    bool            m_bChanging;
    int             m_MouseX;

    static int      s_SpriteID;
    static int      s_NFrames;

public:
                    Ratchet         ();
    virtual void    Render          ();
    virtual void    Expose          ( PropertyMap& pm );
    virtual bool    OnMouseMove     ( int mX, int mY, DWORD keys );
    virtual bool    OnMouseLBDown   ( int mX, int mY );            
    virtual bool    OnMouseLBUp     ( int mX, int mY );            
    void            AddValue        ( int delta );
    int             GetRatchetWidth () const { return ISM->GetFrameWidth( s_SpriteID, 0 ); }
    float           GetPosition     () const { return m_Position; }
    float           GetPrevPosition () const { return m_PrevPosition; }

    Signal          sigChanged;

    DECLARE_CLASS(Ratchet);
}; // class Ratchet

#endif // __UIRATCHET_H__