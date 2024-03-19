/*****************************************************************************/
/*    File:    uiResourceBrowser.h
/*    Desc:    TexturePicker control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIRESOURCEBROWSER_H__
#define __UIRESOURCEBROWSER_H__

/*****************************************************************************/
/*    Class:    ResourceBrowser
/*    Desc:    Resource database browser
/*****************************************************************************/    
class ResourceBrowser : public Window
{
public:
                        ResourceBrowser();
    virtual void        Render();
    

    DECLARE_CLASS(ResourceBrowser);

}; // class ResourceBrowser

#endif // __UIRESOURCEBROWSER_H__