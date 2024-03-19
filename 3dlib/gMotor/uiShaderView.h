/*****************************************************************************/
/*    File:    uiShaderView.h
/*    Desc:    ShaderView control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UISHADERVIEW_H__
#define __UISHADERVIEW_H__

/*****************************************************************************/
/*    Class:    ShaderView
/*    Desc:    ShaderView control
/*****************************************************************************/    
class ShaderView : public EditBox
{

public:

                        ShaderView      ();
    virtual void        Render            ();

    DECLARE_CLASS(ShaderView);
}; // class ShaderView

#endif // __UISHADERVIEW_H__