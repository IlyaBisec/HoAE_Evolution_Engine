/*****************************************************************************/
/*    File:    uiTextureView.h
/*    Desc:    TextureView control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UITEXTUREVIEW_H__
#define __UITEXTUREVIEW_H__

/*****************************************************************************/
/*    Class:    TextureView
/*    Desc:    TextureView control
/*****************************************************************************/    
class TextureView : public EditBox
{
public:

                        TextureView   ();
    virtual void        Render            ();

    DECLARE_CLASS(TextureView);
}; // class TextureView

#endif // __UITEXTUREVIEW_H__