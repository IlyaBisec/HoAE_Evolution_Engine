/*****************************************************************************/
/*    File:    sgDummy.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGDUMMY_H__
#define __SGDUMMY_H__

#include "vField.h"
#include "sgNode.h"
#include "sgTransformNode.h"

/*****************************************************************************/
/*    Class:    Locator
/*    Desc:    Dummy node used for object placing
/*****************************************************************************/
class Locator : public TransformNode
{
public:
    DECLARE_SCLASS(Locator,TransformNode,LCTR);
}; // class Locator 

/*****************************************************************************/
/*    Class:    Bone
/*    Desc:    Dummy node used for skinning bone
/*****************************************************************************/
class Bone : public TransformNode
{
public:

    virtual void            Render();

    DECLARE_SCLASS(Bone,TransformNode,BONE);
}; // class Bone 

/*****************************************************************************/
/*    Class:    ZBias
/*    Desc:    SNode which performs z-biasing of all its children    
/*****************************************************************************/
class ZBias : public SNode
{
    float                        m_Bias;
public:
    ZBias();

    virtual void                Serialize( OutStream& os ) const;
    virtual void                Unserialize( InStream& is );
    virtual void                Render();
    virtual void                Expose( PropertyMap& pm );

    float                        GetBias() const { return m_Bias; }
    void                        SetBias( float bias ) { m_Bias = bias; }

    DECLARE_SCLASS(ZBias,SNode,ZBIA);
}; // class ZBias 

/*****************************************************************************/
/*    Class:    Group
/*    Desc:    Grouping node helper
/*****************************************************************************/
class Group : public SNode
{
public:

    void AttachEffect();
    void Expose( PropertyMap& pm );
    DECLARE_SCLASS(Group,SNode,GRUP);
}; // class Group 

/*****************************************************************************/
/*    Class:    Switch
/*    Desc:    Switching node helper
/*****************************************************************************/
class Switch : public SNode
{
    int                        curActive;
public:
                            Switch() : curActive(-1) {}

    virtual void            Render        ();
    virtual void            Serialize    ( OutStream& os ) const;
    virtual void            Unserialize    ( InStream& is    );
    virtual void            Expose        ( PropertyMap& pm );

    void                    SwitchTo    ( int nodeIdx );
    int                        GetActive    () const    { return curActive; }

    DECLARE_SCLASS(Switch,SNode,SWIT);
}; // class Group 

/*****************************************************************************/
/*    Class:    FieldPatch
/*    Desc:    
/*****************************************************************************/
class FieldPatch : public SNode
{
    FieldModel      m_Field;

    int             m_PatchesW;
    int             m_PatchesH;
    float           m_Width;
    float           m_Height;
    float           m_Age;

public:
                    FieldPatch        ();
    virtual void    Serialize        ( OutStream& os ) const;
    virtual void    Unserialize        ( InStream& is );
    virtual void    Render            ();
    virtual void    Expose            ( PropertyMap& pm );

    DECLARE_SCLASS( FieldPatch, SNode, FPAT );
}; // class FieldPatch

/*****************************************************************************/
/*    Class:    Background
/*    Desc:    Rendering view port
/*****************************************************************************/
class Background : public SNode
{
    Rct                            m_Extents;
    DWORD                        m_Flags;
    DWORD                        m_Color;

public:
    Background            ();
    virtual void                Serialize        ( OutStream&    os ) const;
    virtual void                Unserialize        ( InStream&        is );
    virtual void                Expose            ( PropertyMap&    pm );
    virtual void                Render            ();

    //  node flags operations
    enum ViewportFlags
    {
        vfClearColor    = 0x0001,
        vfClearDepth    = 0x0002,
        vfClearStencil    = 0x0004,
        vfAutoScale        = 0x0008
    }; // enum NodeFlags

    bool                    GetViewportFlag    ( ViewportFlags flag ) const { return (m_Flags & flag) != 0; }
    void                    SetViewportFlag    ( ViewportFlags flag, bool state = true ) { if (state == true) m_Flags |= flag; else m_Flags &= ~flag; }

    void                    SetBgColor        ( DWORD color ) { m_Color = color; }
    DWORD                    GetBgColor        () const        { return m_Color; }

    bool                    GetClearColor    () const { return GetViewportFlag( vfClearColor      );    }
    bool                    GetClearDepth    () const { return GetViewportFlag( vfClearDepth      );    }
    bool                    GetClearStencil    () const { return GetViewportFlag( vfClearStencil );    }
    bool                    GetAutoScale    () const { return GetViewportFlag( vfAutoScale );        }

    void                    SetClearColor    ( bool val = true ) { SetViewportFlag( vfClearColor, val );    }
    void                    SetClearDepth    ( bool val = true ) { SetViewportFlag( vfClearDepth, val );    }
    void                    SetClearStencil    ( bool val = true ) { SetViewportFlag( vfClearStencil, val );    }
    void                    SetAutoScale    ( bool val = true ) { SetViewportFlag( vfAutoScale, val );        }

    void                    SetExtents        ( float x, float y, float w, float h ) { m_Extents.Set( x, y, w, h ); }
    void                    SetExtents        ( const Rct& rct ) { m_Extents = rct; }
    const Rct&                GetExtents        () const { return m_Extents; }

    float                    GetX            () const { return m_Extents.x; }
    float                    GetY            () const { return m_Extents.y; }
    float                    GetW            () const { return m_Extents.w; }
    float                    GetH            () const { return m_Extents.h; }
    void                    SetX            ( float x ) { m_Extents.x = x; }
    void                    SetY            ( float y ) { m_Extents.y = y; }
    void                    SetW            ( float w ) { m_Extents.w = w; }
    void                    SetH            ( float h ) { m_Extents.h = h; }

    DECLARE_SCLASS(Background,SNode,VPRT);
}; // class Background



#ifdef _INLINES
#include "sgDummy.inl"
#endif // _INLINES

#endif // __SGDUMMY_H__