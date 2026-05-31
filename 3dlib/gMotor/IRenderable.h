/*****************************************************************************/
/*    File:    IRenderable.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __IRENDERABLE_H__
#define __IRENDERABLE_H__

/*****************************************************************************/
/*  Class:  IRenderable
/*  Desc:   Renderable entity interface
/*****************************************************************************/
class IRenderable
{
public:
    virtual void             Render() = 0;
    

}; // class IRenderable

#endif // __IRENDERABLE_H__


