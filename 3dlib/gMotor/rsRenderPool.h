/*****************************************************************************/
/*  File:   rsRenderPool.h                                   
/*  Author: Silver, Copyright (C) GSC Game World 
/*  Desc:    Render pool is used for batching all calls, submitted to
/*                the render device, sorting it, possibly merging and finally 
/*                submitting. The sort priority is not fixed, to make it possible 
/*                to play little with it interactively.
/*  Date:   Dec 2003                                             
/*****************************************************************************/
#ifndef __RSRENDERPOOL_H__
#define __RSRENDERPOOL_H__

/*****************************************************************************/
/*    Class:    RenderAttributes
/*    Desc:    Bundle of the mesh render attributes
/*****************************************************************************/
class RenderAttributes
{
public:
    enum AttributeType
    {
        atTex0,                    //  texture layer 0
        atTex1,                    //  texture layer 1
        atTex2,                    //  texture layer 2
        atTex3,                    //  texture layer 3
        atDSS,                    //  device state block
        atMaterial,                //  material handle
        atVS,                    //  vertex shader handle
        atVSConstant,            //  vertex shader constants
        atPS,                    //  pixel shader handle
        atPSConstant,            //  pixel shader constants
        atTransparency,            //  opacity/transparency measure
        atZ,                    //  integer z value 
        atWorldTransform,        //  world transform placeholder
        atVertexBuffer,            //  vertex buffer ID
        atIndexBuffer,            //  index buffer ID
        atVBPos,                //  vertex buffer position
        atNumVert,                //  number of vertices
        atIBPos,                //  index buffer position
        atNumInd,                //  number of indices
        atMesh,                    //    handle to the mesh
        atNumAttributes            //  MUST be last
    }; // enum AttributeType
    
    DWORD                        m_Attr[atNumAttributes]; // attributes list
    int                            GetNumAttributes    () const { return (int)atNumAttributes; }
    void                        Random                ( DWORD maxVal );    
    void                        Dump                ();

    //  changes attribute sort priority
    static void                    MoveAttributeUp        ( int curPos );
    static void                    MoveAttributeDown    ( int curPos );
    static void                    EnableAttribute        ( int attrID, bool enable = true );
    static const char*            GetAttributeName    ( int idx );    
    static int                    Compare                ( const void* p1, const void* p2 );
    static void                    DumpAttrOrder        ();

    //  attribute's description
    struct AttributeStatus
    {
        int        m_Priority;        //  attribute's sorting priority
        bool    m_bEnabled;        //  true when elements are being sorted by this attribute

        AttributeStatus() : m_Priority( -1 ), m_bEnabled( true ) {}
    }; // struct AttributeStatus

    static AttributeStatus    s_AttrOrder[atNumAttributes];

}; // class RenderAttributes

class RenderElement : public RenderAttributes
{
public:
}; // class RenderElement

/*****************************************************************************/
/*    Class:    RenderPool
/*    Desc:    Reorders calls to the render device, to optimize graphics 
/*                pipeline utilization
/*****************************************************************************/
class RenderPool
{
    c2::array<RenderElement>        m_RenderElem;    //  array of render elements
    c2::array<RenderElement*>        m_ElemOrder;    //  array of reordered pointers to render elements

public:
                    RenderPool();

    void            Sort();
    void            Dump();
    void            AddElement( const RenderElement& el ) { m_RenderElem.push_back( el ); }

    int                GetNumElements() const { return m_RenderElem.size(); }
}; // class RenderPool

#endif // __RSRENDERPOOL_H__