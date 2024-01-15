/*****************************************************************************/
/*    File:    vBone.cpp
/*    Desc:    ModelObject skeleton bone
/*    Author:    Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#include "stdafx.h"
#include "vBone.h"

IMPLEMENT_CLASS(ModelBone);
IMPLEMENT_CLASS(Skeleton);

/*****************************************************************************/
/*  ModelBone implementation
/*****************************************************************************/
ModelBone::ModelBone()
{
    m_ID            = -1;           
    m_pParent       = NULL;        
    m_Name          = "";        
    m_Flags         = 0;       
    m_NumChildren   = 0; 
    m_pChild        = NULL;      
    m_pNextSibling  = NULL;
    m_pPrevSibling  = NULL;
} // ModelBone::ModelBone

void ModelBone::Expose( PropertyMap& pm )
{
    pm.start( "ModelBone", this );
    pm.p( "NodeType", &ModelBone::ClassName );
    pm.p( "Name", &ModelBone::GetName, &ModelBone::SetName );
    pm.p( "ID", &ModelBone::GetID );
} // ModelBone::Expose

IReflected* ModelBone::Child( int idx ) const
{
    ModelBone* pChild = m_pChild;
    while (idx > 0 && pChild)
    {
        pChild = pChild->m_pNextSibling;
        idx--;
    }
    return pChild;
} // ModelBone::Child

bool ModelBone::AddChild( IReflected* pChild )
{ 
    ModelBone* pCh = dynamic_cast<ModelBone*>( pChild );
    if (!pCh) return false;
    if (!m_pChild)
    {
        m_pChild            = pCh;
        pCh->m_pParent      = this;
        pCh->m_pPrevSibling = NULL;
        pCh->m_pNextSibling = NULL;
        m_NumChildren       = 1;
        return true;
    }

    ModelBone* pLast = m_pChild;
    while (pLast->m_pNextSibling) pLast = pLast->m_pNextSibling;
    pLast->m_pNextSibling   = pCh;
    pCh->m_pPrevSibling     = pLast;
    pCh->m_pParent          = this;
    m_NumChildren++;
    return true; 
} // ModelBone::AddChild

bool ModelBone::DelChild( int idx )             
{ 
    return false; 
} // ModelBone::DelChild

/*****************************************************************************/
/*  Skeleton implementation
/*****************************************************************************/
Skeleton::Skeleton()
{
    m_bDrawBoneLabels   = false;
    m_bDrawBones        = false;
}

int Skeleton::NumChildren() const 
{ 
    int nB = size();
    int nCh = 0;
    for (int i = 0; i < nB; i++)
    {
        if (!at( i ).Parent()) nCh++;
    }
    return nCh; 
} // Skeleton::NumChildren 

IReflected* Skeleton::Child( int idx ) const 
{ 
    if (idx < 0 || idx >= size()) return NULL;
    int nB = size();
    int nCh = 0;
    for (int i = 0; i < nB; i++)
    {
        if (!at( i ).Parent()) 
        {
            if (nCh == idx) return (IReflected*)&at( i ); 
            nCh++;
        }
    }
    return NULL; 
} // Skeleton::Child

void Skeleton::Expose( PropertyMap& pm )
{
    pm.start( "Skeleton", this );
    pm.p( "NodeType", &Skeleton::ClassName );
    pm.p( "NumBones", &Skeleton::GetNBones );
    pm.f( "DrawBones", m_bDrawBones );
    pm.f( "DrawLabels", m_bDrawBoneLabels );
} // Skeleton::Expose