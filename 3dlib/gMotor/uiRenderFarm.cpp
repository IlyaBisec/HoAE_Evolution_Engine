/*****************************************************************************/
/*    File:    uiRenderFarm.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "uiRenderFarm.h"

/*****************************************************************************/
/*    RenderFarm implementation
/*****************************************************************************/
RenderFarm::RenderFarm()
{
}

void RenderFarm::Render()
{

} // RenderFarm::Render

void RenderFarm::Expose( PropertyMap& pm )
{
    pm.start( "RenderFarm", this );
} // RenderFarm::Expose


