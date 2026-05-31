/*****************************************************************************/
/*    File:    ISkyRenderer.h
/*    Desc:    Interface for working with sky
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-15-2003
/*****************************************************************************/
#ifndef __ISKYRENDERER_H__
#define __ISKYRENDERER_H__

#include "IRenderable.h"

/*****************************************************************************/
/*    Class:    ISkyRenderer
/*    Desc:    Interface for sky manipulation
/*****************************************************************************/
class ISkyRenderer : public IRenderable
{
public:
}; // class ISkyRenderer

extern ISkyRenderer* ISky;
#endif // __ISKYRENDERER_H__