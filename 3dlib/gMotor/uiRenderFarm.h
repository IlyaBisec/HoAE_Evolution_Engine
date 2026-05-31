/*****************************************************************************/
/*    File:    uiRenderFarm.h
/*    Desc:    Render binning visualisation node
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-12-2003
/*****************************************************************************/
#ifndef __UIRENDERFARM_H__
#define __UIRENDERFARM_H__

#include "uiWindow.h"

/*****************************************************************************/
/*    Class:    RenderFarm
/*    Desc:    Interactive visualisator of the render binning voodoo
/*****************************************************************************/
class RenderFarm : public Window
{
public:
                    RenderFarm();
    
    virtual void    Render();
    virtual void    Expose( PropertyMap& pm );

}; // class Window

#endif // __UIRENDERFARM_H__