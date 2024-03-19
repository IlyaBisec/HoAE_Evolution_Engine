/*****************************************************************************/
/*    File:    uiNodeTree.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-07-2003
/*****************************************************************************/
#ifndef __UINODETREE_H__
#define __UINODETREE_H__


const int c_MaxTreeDepth        = 128;
/*****************************************************************************/
/*    Class:    NodeTree
/*    Desc:    Browser of the scene graph subtree
/*****************************************************************************/
class NodeTree : public Window
{
public:
                        NodeTree            ();
    virtual bool        OnMouseLBDown       ( int mX, int mY );    
    virtual bool        OnKeyDown           ( DWORD keyCode, DWORD flags );
    virtual bool        OnDrag              ( float dx, float dy );

    IReflected*         GetSelectedNode     () const;
    IReflected*         GetRootNode         () const;        
    IReflected*         GetDraggedNode      () const;

    void                SetRightHand        ( bool val = true );
    void                SetAcceptOnDrop     ( bool val = true );
    void                SetEditable         ( bool val = true );
    void                SetDragLeafsOnly    ( bool val = true );
    void                SetVisibleRoot      ( bool val = true );

    void                SelectNode          ( IReflected* pNode );
    void                SetRootNode         ( IReflected* pNode );
    void                SetRootPos          ( int x, int y );

    void                SelectPrev          ();
    void                SelectNext          ();

    void                SwapPrev            ();
    void                SwapNext            ();

    bool                PtIn                ( float x, float y ) const { return (PickNode( x, y ) != NULL); }
    IReflected*         PickNode            ( int mX, int mY ) const;
    virtual void        Expose              ( PropertyMap& pm );
    virtual void        Render              ();

    virtual bool        OnStartDrag         ( int mX, int mY );
    virtual IReflected* OnEndDrag           ();
    virtual void        OnDrop              ( int mX, int mY, IReflected* pDropped );

    DECLARE_CLASS(NodeTree);

protected:

    virtual bool        DrawNode        (    IReflected* pNode,    const Rct& rct, 
                                            IReflected* pParent = NULL,    
                                            const Rct& prct = Rct::null );

    bool                PickNode        (    IReflected* pNode,    const Rct& rct, 
                                            IReflected* pParent,    const Rct& prct );

    virtual Rct            GetRootRct        () const;


    typedef bool        (NodeTree::*ItCallback)    (    IReflected* pNode,    const Rct& rct, 
                                                    IReflected* pParent,    const Rct& prct );

    void                Iterate( ItCallback process );

    virtual DWORD        GetNodeBgColor            ( IReflected* pNode ) const;

    IReflected*            m_Root;                    //  current root 
    IReflected*            m_pDragged;                //  node currently dragged 
    bool                m_bDragCopy;            //  when true IReflected is moved, else it is copied

    int                    m_Path[c_MaxTreeDepth];    
    int                    m_Depth;                //  current opened tree depth
    bool                m_bSelCollapse;            //  whether selected IReflected is collapsed
    bool                m_bRightHand;            //  root IReflected is rightmost
    bool                m_bDragLeafsOnly;        //  whether only leafs could be dragged out
    bool                m_bDropToItself;        //  whether ITreeNodes from us can be dropped here
    bool                m_bShowGlyphs;            //    whether to show glyph pictures on buttons
    bool                m_bHasVisibleRoot;        
    bool                m_bAcceptOnDrop;    
    bool                m_bEditable;


    int                    m_NodeWidth;
    int                    m_NodeHeight;
    int                    m_HNodeSpacing;
    int                    m_VNodeSpacing;

    int                    m_RootX, m_RootY;        //  coordinates of the root IReflected left top

    DWORD                m_LinesColorBeg;
    DWORD                m_LinesColorEnd;
    DWORD                m_TextColor;
    DWORD                m_DefaultNodeColor;

    //  current mouse position
    mutable int            m_MX;
    mutable int            m_MY;

    //  temporary values used in callbacks
    //    do not use them for anything else!!!
    mutable IReflected*    m_PickResult;
    Rct                    m_PickRct;
    Rct                    m_ClipRct;
    IReflected*            m_SelNode;
    IReflected*            m_RootNode;
    int                    m_StopPath[c_MaxTreeDepth];    
    int                    m_StopDepth;                

}; // class NodeTree

#endif // __UINODETREE_H__
