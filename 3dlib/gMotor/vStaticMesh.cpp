/*****************************************************************************/
/*	File:	vStaticMesh.cpp
/*	Desc:	Mesh interface implementation
/*	Author:	Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "vMesh.h"
#include "vModel.h"
#include "vModelInstance.h"
#include "vMaterial.h"
#include "vStaticMesh.h"
#include "kSSEUtils.h"

/*****************************************************************************/
/*  StaticMesh implementation
/*****************************************************************************/
IMPLEMENT_CLASS(StaticMesh);
int StaticMesh::s_VBuffer = -1;
int StaticMesh::s_IBuffer = -1;

StaticMesh::StaticMesh()
{
    m_HostBone      = -1;
    m_ICacheStamp   = 0;
    m_VCacheStamp   = 0;

    m_VBufferPos    = 0;
    m_IBufferPos    = 0;
} // StaticMesh::StaticMesh

void StaticMesh::Expose( PropertyMap& pm )
{
    pm.start<Mesh>( "StaticMesh", this );
    pm.p( "HostBone", GetHostBone );
    pm.p( "HostBoneName", GetHostBoneName, SetHostBoneName );
} // StaticMesh::Expose

bool StaticMesh::InstanceIndices( BYTE* pBuf, ModelInstance* pInstance )
{
    memcpy( pBuf, GetIndexData(), GetNIndices()*GetIndexStride() );
    return true;
} // StaticMesh::InstanceIndices

bool StaticMesh::InstanceVertices( BYTE* pBuf, ModelInstance* pInstance )
{
    SSE_memcpy( pBuf, GetVertexData(), GetNVerts()*GetVDecl().m_VertexSize );
    return false;
} // StaticMesh::InstanceVertices

bool StaticMesh::Draw( ModelInstance* m )
{ 
    if (!m_pShader) return false;
    if (s_IBuffer == -1) s_IBuffer = IRS->GetIBufferID( "SharedStatic" );
    if (s_VBuffer == -1) s_VBuffer = IRS->GetVBufferID( "SharedStatic" );

    int nV      = GetNVerts();
    int nI      = GetNIndices();
    int nBytes  = nV*GetVDecl().m_VertexSize;
    int vType   = GetVDecl().m_TypeID;

    IRS->SetVB( s_VBuffer, vType );

    if (!IRS->IsIBStampValid( s_IBuffer, m_ICacheStamp ))
    {
        BYTE* pIdx  = IRS->LockAppendIB( s_IBuffer, nI, m_IBufferPos, m_ICacheStamp );
        if (!pIdx) return false;
        InstanceIndices( pIdx, m );
        IRS->UnlockIB( s_IBuffer );
    }

    if (!IRS->IsVBStampValid( s_VBuffer, m_VCacheStamp ))
    {
        BYTE* pOut  = IRS->LockAppendVB( s_VBuffer, nV, m_VBufferPos, m_VCacheStamp );
        if (!pOut) return false;
        InstanceVertices( pOut, m );
        IRS->UnlockVB( s_VBuffer );
    }
    
    for (int i = 0; i < m_pShader->GetNPasses(); i++)
    {
        RenderTask& rt = IRS->AddTask();
        GetRenderAttributes( rt );
        rt.m_VBufID         = s_VBuffer;
        rt.m_FirstVert      = m_VBufferPos;
        rt.m_IBufID         = s_IBuffer;
        rt.m_FirstIdx       = m_IBufferPos;
        rt.m_bHasTM         = false;
        rt.m_Source         = GetName();

        if (m_HostBone >= 0) 
        { 
            rt.m_bHasTM = true; 
            rt.m_TM = m->GetWorldSpaceBones()[m_HostBone]; 
        }
    }
    return true; 
} // StaticMesh::Draw

void StaticMesh::Serialize( OutStream& os ) const
{
    Mesh::Serialize( os );
    os << m_HostBone;
} // StaticMesh::Serialize

void StaticMesh::Unserialize( InStream& is )
{
    Mesh::Unserialize( is );
    is >> m_HostBone;
    m_ICacheStamp = 0;
    m_VCacheStamp = 0;
} // StaticMesh::Unserialize

