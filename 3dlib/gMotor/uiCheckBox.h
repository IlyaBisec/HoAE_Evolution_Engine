/*****************************************************************************/
/*    File:    uiCheckBox.h
/*    Desc:    CheckButton control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UICHECKBOX_H__
#define __UICHECKBOX_H__

/*****************************************************************************/
/*    Class:    CheckButton
/*    Desc:    Checkbox control
/*****************************************************************************/    
class CheckButton : public Widget
{
    bool            m_bChecked;
    int             m_ThumbWidth;
    int             m_ThumbHeight;

protected:
    bool            m_bShowText;

public:
    Signal          sigChecked;
    Signal          sigUnchecked;

                    CheckButton  ();
    void            Toggle      ();
    void            Check       ();
    void            Uncheck     ();
    bool            IsChecked   () const { return m_bChecked; }

    virtual void    Expose      ( PropertyMap& pm );
    virtual void    Render        ();
    
    virtual bool     OnMouseLBDown( int mX, int mY );

    DECLARE_CLASS(CheckButton);
}; // class CheckButton

#endif // __UICHECKBOX_H__