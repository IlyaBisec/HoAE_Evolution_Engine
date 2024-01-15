/*****************************************************************************/
/*    File:    sgNode.h
/*    Desc:    Scene graph node
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGNODE_H__
#define __SGNODE_H__

#include <vector>

#include "IReflected.h"
#include "kObjectFactory.h"
#include "sgNodePool.h"

#define NOT_IMPLEMENTED        virtual DWORD GetColor() const { return 0xFFFF0000; }

class PropertyMap;
class XMLNode;

const int    c_MaxNodePathLen = 512;
const DWORD    c_BadID             = 0xFFFFFFFF;

class SNode;
/*****************************************************************************/
/*    Class:    SNode
/*    Desc:    Base class for the scene graph node
/*****************************************************************************/
#pragma pack ( push )
#pragma pack ( 4 )
class SNode : public IReflected
{
    DWORD                       m_Id;           //  unique for this session scene graph node ID
    WORD                        m_Flags;        //  set of node properties flags     
    DWORD                       m_RefCount;      
    SNode*                      m_pParent;       
    std::string                 m_Name;         //  node name
    std::vector<SNode*>         m_Children;     //  node's children
public:
    bool                        m_bTransform;
protected:    
    
    virtual                     ~SNode() {}
    friend class NodePool;

    struct PNodePair
    {
        SNode*    pNode;
        SNode*    pParent;
        PNodePair() : pNode(NULL), pParent(NULL){}
        PNodePair( SNode* node, SNode* parent ) : pNode(node), pParent(parent){}
    };
    //  serialization helper stuff
    typedef                        std::map<SNode*, DWORD>        NodePtrMap;
    typedef                        std::map<DWORD, PNodePair>    NodeIdMap;
    typedef                        std::vector<SNode*>            NodePtrList;

    //  serialization helpers
    static NodePtrMap           s_NodeMap;
    static NodeIdMap            s_NodeIdMap;
    static NodePtrList          s_NodeList;
    static bool                 s_bRenderTMOnly;

public:
    //  node flags operations
    enum NodeFlags
    {
        nfDrawGizmo         = 0x0010,    //  drawing helper
        nfInvisible         = 0x0020,    //  node is invisible (or disabled)
        nfDrawAABB          = 0x0040,    //  drawing bounding box
        nfEmbeddedData      = 0x0100,    //  node has embedded data (which is else external -  in file, for example)
        nfImmortal          = 0x0200,    //  cannot destroy this node
        nfDisabled          = 0x0400,    //  node is not active, but all children are
        nfHasFocus          = 0x0800     //  user has input focus on this node
    }; // enum NodeFlags
    

    SNode() : m_Name(""), m_Flags(0), m_RefCount(0), m_pParent(NULL), m_bTransform(false)
    {
        m_Id        = NodePool::instance().AddNode( this );
    }

    SNode( const char* name ) : m_Name(name), m_Flags(0), m_RefCount(0), m_pParent(NULL), m_bTransform(false)
    {
        m_Id        = NodePool::instance().AddNode( this );
    }

    DWORD                       AddRef  () { return ++m_RefCount; }
    int                         GetNRef () const { return m_RefCount; }
    void                        SetID   ( DWORD val ) { m_Id = val; }


    const char*                 GetName() const { return m_Name.c_str(); }
    DWORD                       GetID() const { return m_Id; }
    void                        SetName( const char* name ) { m_Name = name; }
    bool                        HasName( const char* name ) const
    {
        if (!name) return false;
        return (strcmp( name, GetName() ) == 0);
    }
    bool                        HasNameCI       ( const char* name ) const
    {
        if (!name) return false;
        return (stricmp( name, GetName() ) == 0);
    }
    
    _inl virtual void           Render          ();
    virtual bool                Destroy         ();
    virtual void                Expose          ( PropertyMap& pm );
    virtual bool                IsEqual         ( const SNode* node ) const;
    virtual IReflected*         Clone           () const;
    virtual void                Activate        () {}

    SNode*                      CloneSubtree    ();

    DWORD                       Release         ();
    _inl bool                   HasMagic        ( const char* Magic ) const;
    
    //  children operations
    virtual _inl void           AddChild        ( SNode* pNode );
    _inl void                   AddInput        ( SNode* pNode );
    
    bool                        RemoveChild     ( SNode* pNode );
    bool                        RemoveChild     ( int idx );
    virtual bool                DelChild        ( int idx ) { return RemoveChild( idx ); }
    virtual bool                DelChild        ( IReflected* pChild ) { return RemoveChild( dynamic_cast<SNode*>( pChild ) ); }
    void                        ReleaseChildren ();
    void                        ClearChildren   () { m_Children.clear(); }
    _inl void                   AddChild        ( SNode* pNode, int position );

    virtual int                 ChildIdx        ( IReflected* pChild ) const; 
    virtual bool                SetChild        ( int idx, IReflected* pChild );

    _inl void                   SetParent       ( SNode* pNode );
    _inl bool                   Owns            ( const SNode* pNode ) const; 
    _inl bool                   HasChild        ( SNode* pChild, bool bSearchSubtree = true );
    _inl bool                   HasInput        ( SNode* pChild );
    _inl SNode*                 GetInput        ( int idx );

    IReflected*                 Parent          () const            { return m_pParent; }
    int                         NumChildren     () const            { return m_Children.size(); }
    IReflected*                 Child           ( int idx ) const   { if (idx < 0 || idx >= GetNChildren()) return NULL; return m_Children[idx]; }
    virtual const char*         ClassName       () const            { return GetClassName(); }
    static SNode*               CreateFromFile  ( const char* fileName );
    bool                        WriteToFile     ( const char* fileName );
    virtual bool                AddChild        ( IReflected* pChild ) { AddChild( (SNode*)pChild ); return true; }
    
    template <class NodeT> NodeT* AddChild      ( const char* nodeName = NULL )
    {
        NodeT* pNode = new NodeT();
        if (!pNode) return NULL;
        if (nodeName) pNode->SetName( nodeName );
        AddChild( pNode );
        return pNode;
    } // SNode::AddChild

    template <class NodeT> NodeT* GetChild      ( const char* nodeName = NULL )
    {
        NodeT* pChild = FindChild<NodeT>( nodeName );
        if (!pChild) pChild = AddChild<NodeT>( nodeName );
        return pChild;
    } // SNode::GetChild

    template <class NodeT> NodeT* FindChild     ( const char* nodeName = NULL )
    {
        Iterator it( this );
        while (it)
        {
            SNode* pNode = (SNode*)*it;
            if (pNode->IsA<NodeT>())
            {
                if (nodeName)
                { 
                    if (pNode->HasName( nodeName )) return (NodeT*)pNode;
                }
                else return (NodeT*)pNode;
            }
            ++it;
            pNode = (SNode*)*it;
        }
        return NULL;
    } // SNode::FindChild

    SNode*                       FindChildByName      ( const char* nodeName );
    SNode*                       FindChildByNameCI    ( const char* nodeName );
    bool                         ReplaceChild         ( SNode* pChild, SNode* pNewChild );
    SNode*                       GetChild             ( int idx ) { if (idx < 0 || idx >= GetNChildren()) return NULL; return m_Children[idx]; }
    SNode*                       GetChild             ( int idx ) const { if (idx < 0 || idx >= GetNChildren()) return NULL; return m_Children[idx]; }
    SNode*                       GetParent            () const { return m_pParent; }
    int                          GetNChildren         () const { return m_Children.size(); } 
    _inl int                     GetChildIndex        ( SNode* pChild ) const;
    _inl void                    SwapChildren         ( int ch1, int ch2 );
    _inl const char*             GetClassName         () const;

    //  color for the node in the editor
    virtual DWORD                GetColor             () const { return 0xFFFFFFFF; }
    //  serialization
    virtual void                 Serialize            ( OutStream& os ) const;
    virtual void                 Unserialize          ( InStream& is    );

    void                         Serialize            ( OutStream& os, DWORD nBytes ) const;
    bool                         SerializeSubtree     ( OutStream& os ) const;
    static SNode*                UnserializeSubtree   ( InStream& is    );

    void                         AdjustClonedName     ( const char* name );
    void                         FixInputs            ();
    
    static SNode*                CreateSubtree        ( XMLNode* pRoot );
    virtual bool                 FromXML              ( XMLNode* pRoot );
    virtual XMLNode*             ToXML                ();
    
    virtual void                 PreSerialize         () const;    
    virtual void                 PostUnserialize      ();

    static SNode*                PostUnserialize      ( int nodeIdx );
    _inl bool                    GetFlagState         ( NodeFlags flag ) const;
    _inl void                    SetFlagState         ( NodeFlags flag, bool state = true );
    _inl bool                    IsInvisible          () const;
    _inl bool                    IsImmortal           () const;
    _inl bool                    IsDisabled           () const;
    _inl bool                    DoDrawGizmo          () const;
    _inl bool                    DoDrawAABB           () const;
    _inl bool                    HasFocus             () const;

    _inl void                    SetInvisible         ( bool val = true );
    _inl void                    SetImmortal          ( bool val = true );
    _inl void                    SetDisabled          ( bool val = true );
    _inl void                    SetDrawGizmo         ( bool val = true );
    _inl void                    SetDrawAABB          ( bool val = true );
    _inl void                    SetFocus             ( bool val = true );
    int                          CountNChildrenTotal  () const;
    
    template <class T> bool      IsA                  () const { return (dynamic_cast<const T*>( this ) != NULL); }
    static bool                  Filter               ( const SNode* pNode );    
    static bool                  FilterChildren       ( const SNode* pNode );                        
    virtual bool                 HasFn                ( const char* magic ) const; 
    virtual bool                 HasFn                ( DWORD magic ) const;
    static SNode*                CreateInstance       ();
    static DWORD                 Magic                () { return *((DWORD*)"NODE"); }

    static bool                  IsRenderTMOnly       () { return s_bRenderTMOnly; }
    static void                  SetRenderTMOnly      ( bool bVal ) { s_bRenderTMOnly = bVal; }   

private:
    static SNode*                CreateFromXML        ( XMLNode* pRoot );
}; // class SNode

#pragma pack ( pop )

#ifdef _INLINES 
#include "sgNode.inl"
#endif // _INLINES

#endif // __SGNODE_H__