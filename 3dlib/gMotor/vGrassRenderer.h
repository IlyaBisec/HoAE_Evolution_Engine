/*****************************************************************************/
/*    File:    vGrassRenderer.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-12-2003
/*****************************************************************************/
#ifndef __VGRASSRENDERER_H__
#define __VGRASSRENDERER_H__

/*****************************************************************************/
/*    Class:    GrassRenderer
/*    Desc:    
/*****************************************************************************/
class GrassRenderer
{
public:
    virtual void    RenderBlock( const Rct& ext, int LOD );
}; // class GrassRenderer 

extern GrassRenderer* IGrass;

#endif // __VGRASSRENDERER_H__
