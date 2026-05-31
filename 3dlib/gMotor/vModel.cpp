/*****************************************************************************/
/*	File:	vModel.cpp
/*	Desc:	ModelObject interface implementation
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#include "stdafx.h"
#include "kContext.h"
#include "kHash.hpp"
#include "kFilePath.h"

#include "IMesh.h"
#include "IModelManager.h"

#include "vMesh.h"
#include "vSkin.h"
#include "vModel.h"
#include "vBone.h"

/*****************************************************************************/
/*  ModelObject implementation
/*****************************************************************************/
IMPLEMENT_CLASS(ModelObject);
ModelObject::ModelObject()
{
    m_ID = -1;
} // ModelObject::ModelObject

ModelObject::~ModelObject()
{
    for (int i = 0; i < GetNMeshes(); i++)
    {
        delete m_Meshes[i];
    }
} // ModelObject::~ModelObject

void ModelObject::Reset()
{
    m_Skeleton.clear();
    m_Meshes.clear();  
    m_Shaders.clear(); 
} // ModelObject::Reset

int ModelObject::GetBoneIndex( const char* name ) const
{
    int nBones = m_Skeleton.size();
    for (int i = 0; i < nBones; i++)
    {
        if (!stricmp( name, m_Skeleton[i].GetName() )) return i;
    }
    return -1;
} // ModelObject::GetBoneIndex

int ModelObject::GetNVertices() const
{
    int nC = m_Meshes.size();
    int nV = 0;
    for (int i = 0; i < nC; i++) nV += m_Meshes[i]->GetNVerts();
    return nV;
} // ModelObject::GetNVertices

int ModelObject::GetNPoly() const
{
    int nC = m_Meshes.size();
    int nI = 0;
    for (int i = 0; i < nC; i++) 
        if (m_Meshes[i]) nI += m_Meshes[i]->GetNPoly();
    return nI;
} // ModelObject::GetNIndices

const char* ModelObject::GetVersionString() const
{
    static char buf[256];
    sprintf( buf, "%d.%d", c_ModelVersion >> 16, c_ModelVersion&0x0000FFFF );
    return buf;
} // ModelObject::GetVersionString

void ModelObject::Expose( PropertyMap& pm )
{
    pm.start( "ModelObject", this );
    pm.p( "NodeType",   ClassName           );
    pm.p( "Version",    GetVersionString    );
    pm.p( "Name",       GetName             );
    pm.p( "Polygons",   GetNPoly            );
    pm.p( "Vertices",   GetNVertices        );
    pm.p( "Materials",  GetNMaterials       );
    pm.p( "Meshes",     GetNMeshes          );
} // ModelObject::Expose

int ModelObject::NumChildren() const
{
    return 7;
}

IReflected* ModelObject::Child( int idx ) const
{
    if (idx == 0) return (IReflected*)&m_Shaders;
    if (idx == 1) return (IReflected*)&m_Skeleton;
    if (idx == 2) return (IReflected*)&m_Meshes;
    if (idx == 3) return (IReflected*)&m_Slots;
    if (idx == 4) return (IReflected*)&m_Effectors;
    if (idx == 5) return (IReflected*)&m_Colliders;
    if (idx == 6) return (IReflected*)&m_Clips;
    return NULL;
} // ModelObject::Child

bool ModelObject::Reload()
{
    IModelMgr->ReloadModel( GetID() );
    return false;
} // ModelObject::Reload

bool ModelObject::Serialize( OutStream& os )
{
    os << c_ModelMagic << c_ModelVersion << m_Name << 
        m_Skeleton << m_Shaders << m_Meshes << m_Clips << 
        m_Effectors << m_Slots << m_Animations << m_Colliders;    
    return true;                                       
} // ModelObject::Serialize                                      

bool ModelObject::Unserialize( InStream& is )
{
    DWORD magic;
    is >> magic;
    if (magic != c_ModelMagic) return false;
    DWORD version;
    is >> version;
    if (version < c_ModelVersion)
    {
        Log.Error( "Trying to load model of older version." );
        return false;
    }

    Mesh::s_pHostModel = this;
    try{
        is >> m_Name >> 
            m_Skeleton >> m_Shaders >> m_Meshes >> m_Clips >>
            m_Effectors >> m_Slots >> m_Animations >> m_Colliders;
    }
    catch (...)
    {
        Log.Error( "Corrupt model file: %s", m_Name.c_str() );
        return false;
    }

    for (int i = 0; i < m_Meshes.size(); i++) m_Meshes[i]->SetModel( this );
    for (int i = 0; i < m_Shaders.size(); i++) 
    {
        m_Shaders[i].SetID    ( i );
        m_Shaders[i].SetModel ( this );
        m_Shaders[i].Validate ();
    }
    for (int i = 0; i < m_Slots.size(); i++) m_Slots[i].SetModel( this );

    CalcAABB();
    CalcBoundSphere();
    return true;
} // ModelObject::Unserialize

void ModelObject::CalcAABB()
{
    int nM = m_Meshes.size();
    if (nM == 0) return;
    for (int i = 0; i < nM; i++)
    {
        int boneID = m_Meshes[i]->GetHostBone();
        AABoundBox box = m_Meshes[i]->GetAABB();
        if (boneID > 0)
        {
            box.Transform( m_Skeleton[boneID].m_WorldTM );  
        }
        if (i > 0) m_AABB.Union( box ); else m_AABB = box;
    }
} // Mesh::CalcAABB

void ModelObject::CalcBoundSphere()
{
    int nM = m_Meshes.size();
    if (nM == 0) return;
    m_BoundSphere = m_Meshes[0]->GetBoundSphere();
    for (int i = 0; i < nM; i++)
    {
        int boneID = m_Meshes[i]->GetHostBone();
        Sphere sp = m_Meshes[i]->GetBoundSphere();
        if (boneID > 0)
        {
            sp.Transform( m_Skeleton[boneID].m_WorldTM );  
        }
        if (i > 0) m_BoundSphere += sp; else m_BoundSphere = sp;
    }
} // Mesh::CalcBoundSphere

