/*****************************************************************************/
/*    File:    sgNodePool.h
/*    Desc:    Scene graph node manager
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGNODEPOOL_H__
#define __SGNODEPOOL_H__

#include "kTemplates.hpp"
class SNode;
const int c_MaxNodesInPool = 65536;
/*****************************************************************************/
/*    Class:    NodePool, singleton
/*    Desc:     Global scenegraph node registry
/*****************************************************************************/
class NodePool// : public Singleton<NodePool>
{
    SNode*                   m_Nodes[c_MaxNodesInPool];    // nodes array
    int                      m_NumNodes;                    // current number of nodes (with empty entries)
    int                      m_FirstFree;                // index of the first free entry
    static DWORD             s_CurStamp;

public:
	SNode ** ToPtr() { return m_Nodes; }
	int Count() { return m_NumNodes; }

	static NodePool&		 instance();

                             NodePool   ();
    virtual                  ~NodePool  ();

    template <class NodeType> static NodeType* CreateNode()
    {
        void* buf = new BYTE[sizeof( NodeType )];
        NodeType* pNode = (NodeType*)buf;
        pNode->SetID( instance()._AddNode( pNode ) );
        return pNode;
    }

    template <class NodeType> static NodeType* GetNode( DWORD id )
    {
        SNode* pNode = (NodeType*)instance()._GetNode( id );
        if (!pNode || !pNode->IsA<NodeType>() || !EqualStamps( id, pNode->GetID() )) return NULL;
        return (NodeType*)pNode;
    }

    static SNode* GetNode( DWORD id )
    {
        return instance()._GetNode( id );
    }

    template <class NodeType> 
    static NodeType* GetNodeByName( const char* nodeName, DWORD firstID = 0 )
    {
        for (int i = firstID; i < instance().m_NumNodes; i ++)
        {
            SNode* pNode = instance().m_Nodes[i];
            if (pNode && pNode->IsA<NodeType>() && HasName( pNode, nodeName ))
            {
                return (NodeType*)pNode;
            }
         }
        return NULL;
    } // GetNodeByName

    static SNode* GetNodeByName( const char* nodeName, DWORD firstID = 0 )
    {
        for (int i = firstID; i < instance().m_NumNodes; i ++)
        {
            SNode* pNode = instance().m_Nodes[i];
            if (HasName( pNode, nodeName ))
            {
                return pNode;
            }
        }
        return NULL;
    } // GetNodeByName

    DWORD AddNode( SNode* pNode )
    {
        DWORD idx = 0;
        if (m_FirstFree < m_NumNodes)
        {
            while (m_Nodes[m_FirstFree] != NULL && m_FirstFree < m_NumNodes) m_FirstFree++;
        }
        if (m_Nodes[m_FirstFree] == NULL && m_FirstFree < m_NumNodes)
        {
            idx = m_FirstFree;    
            m_FirstFree++;
        }
        else
        {
            idx = m_NumNodes;
            m_FirstFree++;
            m_NumNodes++;
            if(m_NumNodes>=c_MaxNodesInPool){
                Log.Error("Too many nodes!!!");
            }
        }
        s_CurStamp += 0x00010000;
        m_Nodes[idx] = pNode;
        return idx | s_CurStamp;
    }

    static bool DestroyNode( SNode* pNode );

    void Dump();

protected:
    static bool     HasName     ( SNode* pNode, const char* name );
    static bool     EqualStamps ( DWORD s1, DWORD s2 ) 
    { 
        return (s1 & 0xFFFF0000) == (s2 & 0xFFFF0000); 
    }

    void _ClearEntry( DWORD id )
    {
        DWORD idx = id & 0x0000FFFF;
        m_Nodes[idx] = NULL;
        if (m_FirstFree > idx) m_FirstFree = idx;
    }


    SNode* _GetNode( DWORD id );

}; // class NodePool

 
#endif // __SGNODE_H__