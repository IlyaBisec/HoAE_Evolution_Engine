/*****************************************************************************/
/*    File:    uiResourceBrowser.cpp
/*    Desc:    Resource database browser dialog
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWindow.h"
#include "uiResourceBrowser.h"

/*****************************************************************************/
/*    ResourceBrowser implementation
/*****************************************************************************/    
IMPLEMENT_CLASS(ResourceBrowser);
ResourceBrowser::ResourceBrowser()
{
    CreateFromScript( "res_browser" );
}

void ResourceBrowser::Render()
{
}
