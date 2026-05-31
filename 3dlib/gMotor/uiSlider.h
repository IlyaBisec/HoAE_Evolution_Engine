/*****************************************************************************/
/*    File:    uiSlider.h
/*    Desc:    Slider control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UISLIDER_H__
#define __UISLIDER_H__

/*****************************************************************************/
/*    Class:    Slider
/*    Desc:    Slider control
/*****************************************************************************/    
class Slider : public Widget
{
    float           m_Position;     // current slider position, [0..1]
    int             m_ThumbWidth;

    bool            m_bDragged;
    int             m_StartDragPos;

    float           m_MinPos;
    float           m_MaxPos;
    bool            m_bDrawBorder;

    Rct             m_ThumbExt;
    Rct             m_SlideExt;

public:

                    Slider      ();
    virtual void    Expose      ( PropertyMap& pm );
    virtual void    Render        ();

    void            StartDrag   ();
    void            EndDrag     ();
    void            ChangePos   ();

    float           GetPosition () const { return m_Position; }
    void            SetPosition ( float pos ); 

    virtual bool     OnMouseMove        ( int mX, int mY, DWORD keys );
    virtual bool     OnMouseLBDown    ( int mX, int mY );            
    virtual bool     OnMouseLBUp        ( int mX, int mY );            
        
    DECLARE_CLASS(Slider);

protected:
    virtual void    DrawThumb       ();
    virtual void    DrawSlideArea   ();
    virtual void    DrawTicks       ();
    virtual bool    OnExtents       ();

}; // class Slider

#endif // __UISLIDER_H__