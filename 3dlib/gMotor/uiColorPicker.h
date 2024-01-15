/*****************************************************************************/
/*    File:    uiColorPicker.h
/*    Desc:    Color picker control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UICOLORPICKER_H__
#define __UICOLORPICKER_H__

#include "uiEditBox.h"

/*****************************************************************************/
/*    Class:    ColorBox
/*    Desc:    Resource database browser
/*****************************************************************************/    
class ColorBox : public EditBox
{
    DWORD               m_ColorValue;

public:
                        ColorBox        ();
    virtual void        Render          ();
    void                SetColorValue   ( DWORD c );
    DWORD               GetColorValue   () const { return m_ColorValue; }
    virtual bool         OnMouseLBDown   ( int mX, int mY );
    virtual void        Expose          ( PropertyMap& pm );
    void                OnChangeColor   ();

    DECLARE_CLASS(ColorBox);
}; // class ColorBox

/*****************************************************************************/
/*    Class:    ColorPicker
/*    Desc:    Resource database browser
/*****************************************************************************/    
class ColorPicker : public Window
{
public:
    virtual void        Render();
    

    DECLARE_CLASS(ColorPicker);

}; // class ColorPicker

#endif // __UICOLORPICKER_H__