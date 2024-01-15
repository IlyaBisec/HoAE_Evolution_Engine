/*****************************************************************************/
/*    File:    sgNode.cpp
/*    Desc:    Scene graph node
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "kIOHelpers.h"
#include "kPropertyMap.h"

#ifndef _INLINES 
#include "sgNode.inl"
#endif // _INLINES

/*****************************************************************************/
/*    SNode implementation
/*****************************************************************************/
SNode::NodePtrMap        SNode::s_NodeMap;
SNode::NodeIdMap         SNode::s_NodeIdMap;
SNode::NodePtrList       SNode::s_NodeList;

bool                     SNode::s_bRenderTMOnly = false;
char                     SNode::NameFilter::m_Name[c_MaxNodeNameLen];

bool SNode::SetChild( int idx, IReflected* pChild ) 
{ 
    SNode* pNode = dynamic_cast<SNode*>( pChild );
    if (idx < 0 || idx >= m_Children.size() || !pNode) return false;
    m_Children[idx] = pNode;
    return true; 
}

int SNode::ChildIdx( IReflected* pChild ) const
{
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        for (int i = 0; i < nCh; i++)
        {
            if (m_Children[i] == pChild) return i;
        }
    }
    return -1;
} // SNode::ChildIdx

void SNode::Serialize( OutStream& os, DWORD nBytes ) const
{
    DWORD magic    = ClassID();
    os << magic << nBytes;
    Serialize( os );
}

void SNode::Serialize( OutStream& os ) const
{
    DWORD nChildren = GetNChildren();
    DWORD parentID    = c_BadID;

    NodePtrMap::iterator it;
    it = s_NodeMap.find( GetParent() );
    if (it != s_NodeMap.end() && GetParent() != NULL)
    {
        parentID = (*it).second;
    }

    os << m_Name << m_Flags << parentID << nChildren;
    
    DWORD nodeID;
    for (int i = 0; i < m_Children.size(); i++)
    {
        it = s_NodeMap.find( GetChild( i ) );
        if (it == s_NodeMap.end() || GetChild( i ) == NULL)
        {
            os << c_BadID;
        }
        else
        {
            nodeID = (*it).second;
            os << nodeID;
        }
    }
} // SNode::Serialize

void SNode::Unserialize( InStream& is )
{
    DWORD nodeID, nChildren;
    DWORD parentID;
    DWORD blockSize = 0;
    is >> blockSize >> m_Name >> m_Flags >> parentID >> nChildren;

    m_pParent = reinterpret_cast<SNode*>( parentID );

    int nNodes = nChildren;
    for (int i = 0; i < nNodes; i++)
    {
        is >> nodeID;
        m_Children.push_back( reinterpret_cast<SNode*>( nodeID ) );
    }
} // SNode::Unserialize

SNode*    SNode::CreateFromFile( const char* fileName )
{
    FInStream is( fileName );
    if (is.NoFile()) return NULL;
    SNode* pNode = SNode::UnserializeSubtree( is );
    return pNode;
} // SNode::CreateFromFile

bool SNode::WriteToFile( const char* fileName ) 
{
    FOutStream os( fileName );
    if (os.NoFile()) return false;
    SerializeSubtree( os );
    return true;
} // SNode::WriteToFile

void SNode::Expose( PropertyMap& pm )
{
    pm.start( "SNode", this );
    pm.p( "Name", &SNode::GetName, &SNode::SetName    );
    pm.p( "Class", &SNode::GetClassName        );
    pm.p( "ChildrenNum", &SNode::GetNChildren        );
    //pm.p( "TotalChildren",    CountNChildrenTotal    );
    //pm.p( "ID",                GetID                );
    //pm.p( "NumRef",            GetNRef                );
    pm.p( "Invisible", &SNode::IsInvisible, &SNode::SetInvisible    );
    pm.p( "Disabled", &SNode::IsDisabled, &SNode::SetDisabled    );
    pm.p( "DrawGizmo", &SNode::DoDrawGizmo, &SNode::SetDrawGizmo    );
    pm.p( "DrawAABB", &SNode::DoDrawAABB, &SNode::SetDrawAABB    );
} // SNode::Expose

void SNode::PreSerialize() const
{
    if (s_NodeMap.find( (SNode*)this ) == s_NodeMap.end()) 
    {
        s_NodeMap[(SNode*)this] = s_NodeList.size();
        s_NodeList.push_back( (SNode*)this );
    }

    for (int i = 0; i < GetNChildren(); i++)
    {
        if (Owns( GetChild( i ) )) GetChild( i )->PreSerialize();
    }
} // SNode::PreSerialize

bool SNode::Destroy()
{
    assert( false );
    return false;
} // SNode::Delete

SNode* SNode::PostUnserialize( int nodeIdx )
{
    if (nodeIdx == c_BadID || 
        nodeIdx < 0 || 
        nodeIdx >= s_NodeList.size())
    {
        return NULL;
    }
    assert( nodeIdx >= 0 && nodeIdx < s_NodeList.size() );
    SNode* pNode = s_NodeList[nodeIdx];
    return pNode;
} // SNode::PostUnserialize

void SNode::PostUnserialize()
{
    m_pParent = SNode::PostUnserialize( reinterpret_cast<int>( m_pParent ) );
    for (int i = 0; i < m_Children.size(); i++)
    {
        m_Children[i] = SNode::PostUnserialize( reinterpret_cast<int>( m_Children[i] ) );
        if (m_Children[i] == NULL) continue;
        m_Children[i]->AddRef();
    }
    for (int i = 0; i < m_Children.size(); i++)
    {
        if (m_Children[i] == NULL) m_Children.erase( m_Children.begin() + i );
    }
} // SNode::PostUnserialize

SNode* SNode::UnserializeSubtree( InStream& is )
{
    if (!is) return NULL;
    int nNodes = 1;

    char chMagic[5]; chMagic[4] = 0;
    
    DWORD magic, lastMagic = 0;

    s_NodeList.clear();

    //  fetch root node
    is >> magic;
    SNode* root = (SNode*)ObjectFactory::instance().Create( magic );
    if (!root) return NULL;
    
    try{
        root->Unserialize( is );
        s_NodeList.push_back( root );
        
        //  fetch all other nodes in file
        while (is)
        {
            lastMagic = magic;
            is >> magic;
            SNode* cNode = (SNode*)ObjectFactory::instance().Create( magic );
            if (!cNode)
            {
                DWORD skipSz = 0;
                is >> skipSz;
                is.Skip( skipSz );
                Log.Warning( "Could not read node from input stream: <%s>", &chMagic );
            }
            else
            {
                cNode->Unserialize( is );
                s_NodeList.push_back( cNode );
                nNodes++;
            }
        }

        //  post process pointers to nodes
        for (int i = 0; i < s_NodeList.size(); i++)
        {
            SNode* cNode = s_NodeList[i];
            cNode->PostUnserialize();
        }

        s_NodeList.clear();
        return root;
    }
    catch (...)
    {
        Log.Error( "Could not load model. Error in node:<%s>", &chMagic );
        s_NodeList.clear();
        return NULL;
    }
}// SNode::UnserializeSubtree

bool SNode::SerializeSubtree( OutStream& os ) const
{
    s_NodeMap.clear();
    s_NodeList.clear();

    PreSerialize();

    int nNodes = s_NodeList.size();
    for (int i = 0; i < nNodes; i++)
    {
        SNode* pNode = s_NodeList[i];
        CountStream cs;
        pNode->Serialize( cs );
        int nBytes = cs.GetNBytes();
        pNode->Serialize( os, nBytes );
    }

    s_NodeMap.clear();
    s_NodeList.clear();

    return true;
} // SNode::Serialize

IReflected* SNode::Clone() const
{
    CountStream cs;
    SerializeSubtree( cs );
    int nBytes = cs.GetNBytes();
    MemOutStream os( nBytes );
    SerializeSubtree( os );
    MemInStream is;
    SNode* newNode = UnserializeSubtree( is );
    if (!newNode) return (SNode*)ObjectFactory::instance().Create( ClassID() );
    return newNode;
} // SNode::Clone

void SNode::AdjustClonedName( const char* name )
{
    const char* pos = &name[strlen( name ) - 1];
    while (isdigit( *pos )) pos--;
    int idx = 0;
    int nDig = sscanf( pos + 1, "%d", &idx );
    if (nDig == 0) idx = 0; else idx++;
    char buf[64];
    c2::string adjName;
    do{
        sprintf( buf, "%02d", idx );
        adjName.set( name, pos - name + 1 );
        adjName += buf;
        idx++;
    } while (NodePool::GetNodeByName( adjName ));
    m_Name = adjName;
} // SNode::AdjustClonedName

bool SNode::IsEqual( const SNode* node ) const
{
    if (!node) return false;
    return    node->HasName( m_Name.c_str() ) && (node->m_Flags == m_Flags);
}

DWORD SNode::Release()
{
    m_RefCount--;
    assert( m_RefCount >= 0 );
    if (m_RefCount == 0) 
    {
        NodePool::instance().DestroyNode( this );
        return 0;
    }
    return m_RefCount;
} // SNode::Release

int    SNode::CountNChildrenTotal() const
{
    Iterator it( (SNode*)this ); ++it; 
    int nCh = 0;
    while (it) 
    {
        nCh++;
        ++it; 
    }
    return nCh;
}

SNode* SNode::CreateSubtree( XMLNode* pRoot )
{
    s_NodeIdMap.clear();
    SNode* pNode = CreateFromXML( pRoot );
    if (!pNode) return NULL;
    Iterator it( pNode );
    pNode->FixInputs();
    return pNode;
} // SNode::CreateSubtreeFromXML

void SNode::FixInputs()
{
    for (int i = 0; i < GetNChildren(); i++)
    {
        
    }
} // SNode::CreateSubtreeFromXML

SNode* SNode::CreateFromXML( XMLNode* pRoot )
{
    SNode* pNode = (SNode*)ObjectFactory::instance().Create( pRoot->GetTag() );
    pNode->FromXML( pRoot );
    
    DWORD id = 0;
    if (!pRoot->GetAttr( "id", id ))
    {
        Log.Error( "SNode should have <id> attribute!" );
    }
    const char* name;
    if (!pRoot->GetAttr( "name", name ))
    {
        Log.Error( "SNode should have <name> attribute!" );
    }
    pNode->SetName( name );

    XMLNode* pChild = pRoot->FirstChild();
    while (pChild)
    {
        if (pChild->GetAttr( "id", id ))
        {
            const char* ref = NULL;
            if (pChild->GetAttr( "ref", ref ))
            {
                s_NodeIdMap[id] = PNodePair( pNode, NULL );
            }
            else 
            {
                SNode* pChNode = CreateFromXML( pChild );
                s_NodeIdMap[id] = PNodePair( pNode, pChNode );
            }
            pNode->m_Children.push_back( reinterpret_cast<SNode*>( id ) );
        }
        pChild = pChild->NextSibling();
    }
    
    return pNode;
} // SNode::CreateFromXML

bool SNode::FromXML( XMLNode* pRoot )
{
    return false;
} // SNode::FromXML

const int c_XMLBufSize = 1024;
XMLNode* SNode::ToXML()
{
    XMLNode* pNode = new XMLNode();
    pNode->SetTag( GetClassName() );

    pNode->AddAttr( "name", GetName() );
    pNode->AddAttr( "id", GetID() );

    int nCh = GetNChildren();
    if (nCh > 0)
    {
        for (int i = 0; i < nCh; i++)
        {
            XMLNode* pChild = NULL;
            if (Owns(GetChild( i ))) pChild = GetChild( i )->ToXML();
            else
            {
                pChild = new XMLNode();
                pChild->SetTag( GetChild( i )->GetClassName() );
                const char* input = "in";
                pChild->AddAttr( "ref", input );
                pChild->AddAttr( "name", GetChild( i )->GetName() );
                pChild->AddAttr( "id", GetChild( i )->GetID() );
            }
            pNode->AddChild( pChild );
        }
    }
    return pNode;
} // SNode::ToXML

SNode* SNode::FindChildByName( const char* nodeName )
{
    Iterator it( this );
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        if (pNode->HasName( nodeName )) return pNode;    
        ++it;
        pNode = (SNode*)*it;
    }
    return NULL;
} // FindChild

SNode* SNode::FindChildByNameCI( const char* nodeName )
{
    Iterator it( this );
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        if (pNode->HasNameCI( nodeName )) return pNode;    
        ++it;
        pNode = (SNode*)*it;
    }
    return NULL;
} // FindChild

bool SNode::ReplaceChild( SNode* pChild, SNode* pNewChild )
{
    for (int i = 0; i < GetNChildren(); i++)
    {
        if (GetChild( i ) == pChild)
        {
            m_Children[i] = pNewChild;
            pNewChild->AddRef();
            pChild->SetParent( NULL );
            pChild->Release();
            pNewChild->SetParent( this );
            return true;
        }
    }
    return false;
} // ReplaceChild

bool SNode::Filter( const SNode* pNode )    
{                                                                    
    return pNode->ClassID() == *((DWORD*)"NODE");                    
}    

bool SNode::FilterChildren( const SNode* pNode )                        
{                                                                    
    return (!pNode->GetParent() || pNode->GetParent()->Owns( pNode ));                    
}    

bool SNode::HasFn( const char* magic ) const 
{ 
    return ((*((DWORD*)magic) == *((DWORD*)"DECLARE_SCLASS"))); 
}

bool SNode::HasFn( DWORD magic ) const 
{ 
    return (magic == *((DWORD*)"DECLARE_SCLASS")); 
}

SNode* SNode::CreateInstance() 
{
    return new SNode(); 
}    

SNode* SNode::CloneSubtree()
{
    SNode* pClone = dynamic_cast<SNode*>( Clone() );
    return pClone;
} // SNode::CloneSubtree


bool SNode::RemoveChild( SNode* pNode )
{
    bool erased = false;
    for (int i = 0; i < GetNChildren(); i++)
    {
        if (GetChild( i ) == pNode)
        {
            m_Children.erase( m_Children.begin() + i );
            erased = true;
            if (Owns( pNode )) pNode->SetParent( NULL );
            pNode->Release();
        }
    }
    return erased;
} // SNode::RemoveChild

void SNode::ReleaseChildren()
{
    for (int i = 0; i < GetNChildren(); i++)
    {
        if (Owns( GetChild( i ) )) GetChild( i )->SetParent( NULL );
        GetChild( i )->Release();
    }
    m_Children.clear();
} // SNode::ReleaseChildren

bool SNode::RemoveChild( int idx )
{
    if (idx < 0 || idx >= GetNChildren()) return false;
    SNode* pNode = GetChild( idx );
    m_Children.erase( m_Children.begin() + idx );
    if (Owns( pNode )) pNode->SetParent( NULL );
    pNode->Release();
    return true;
} // SNode::RemoveChild



