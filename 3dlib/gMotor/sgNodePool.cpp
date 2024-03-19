/*****************************************************************************/
/*    File:    sgNodePool.cpp
/*    Desc:    Scene graph node manager
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNode.h"
#include "sgNodePool.h"


/*****************************************************************************/
/*    NodePool implementation
/*****************************************************************************/
DWORD NodePool::s_CurStamp        = 0x00000000;

NodePool::NodePool()
{
    m_NumNodes  = 0;
    m_FirstFree = 0;
}

NodePool::~NodePool()
{
    for (int i = 0; i < m_NumNodes; i++) 
    {
//        delete m_Nodes[i];
//        m_Nodes[i] = NULL;
    }
} // NodePool::~NodePool

void NodePool::Dump()
{
    FILE* fp = fopen( "c:\\dumps\\nodepool.txt", "wt" );
    if (!fp) return;
    
    fprintf( fp, "NumNodes:%d FirstFree:%d\n", m_NumNodes, m_FirstFree );

    for (int i = 0; i < m_NumNodes; i++)
    {
        SNode* pNode = m_Nodes[i];
        if (!pNode)
        {
            fprintf( fp, "-DEAD-\n" );
            continue;
        }
        fprintf( fp, "%d. <%s> ID: %X NumRef: %d\n", 
            i, pNode->GetName(), pNode->GetID(), pNode->GetNRef() );
    }

    fclose( fp );
} // NodePool::Dump

bool NodePool::DestroyNode( SNode* pNode )
{
    if (!pNode) return false;
    DWORD id = pNode->GetID();
    if (instance()._GetNode( id ) != pNode)
    {
        Log.Error( "SNode Pool is corrupt!!!" );
    }

    if (!pNode->IsImmortal()) 
    {
        instance()._ClearEntry( id );
        pNode->Release(); 
        pNode->ReleaseChildren(); 
    }
    return true;
} // NodePool::DestroyNode


bool NodePool::HasName( SNode* pNode, const char* name )
{
    if (!pNode) return false;
    return pNode->HasName( name );
}

SNode* NodePool::_GetNode( DWORD id )
{
    DWORD id2 = id & 0x0000FFFF;
    if( id2 >= m_NumNodes )return NULL;
    SNode* pNode = m_Nodes[id2];
    if (!pNode || !EqualStamps( id, pNode->GetID() )) return NULL;
    return pNode;
} // NodePool::GetNode
NodePool* _nodepool=NULL;
NodePool& NodePool::instance(){
	if(!_nodepool)_nodepool=new NodePool;
	return *_nodepool;
}

