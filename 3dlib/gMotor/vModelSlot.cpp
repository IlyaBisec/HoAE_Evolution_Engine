/*****************************************************************************/
/*	File:	vModelSlot.cpp
/*	Desc:	ModelObject attachment point
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#include "stdafx.h"
#include "vModel.h"
#include "vModelSlot.h"

/*****************************************************************************/
/*  ModelSlot implementation
/*****************************************************************************/
ModelSlot::ModelSlot()
{
    m_ID            = -1;		   
    m_Name          = "";        
    m_Flags         = 0; 
    m_HostBoneID    = -1;
    m_pModel        = NULL;
} // ModelSlot::ModelSlot

void ModelSlot::Expose( PropertyMap& pm )
{
    pm.start( "ModelSlot", this );
    pm.p( "NodeType",       ClassName );
    pm.p( "Name",           GetName, SetName );
    pm.p( "HostBone",       GetHostBone, SetHostBone );
    pm.p( "AttachedModel",  GetAttachedName, SetAttachedName );
} // ModelSlot::Expose

int ModelSlot::GetHostBoneID() 
{
    if (m_HostBoneID == -1)
    {
        m_HostBoneID = m_pModel->GetBoneIndex( m_HostBone.c_str() );
    }
    return m_HostBoneID;
} // ModelSlot::GetHostBoneID

/*****************************************************************************/
/*  SlotList implementation
/*****************************************************************************/
void SlotList::Expose( PropertyMap& pm )
{
    pm.start( "SlotList", this );
    pm.p( "NodeType", ClassName );
    pm.p( "NumSlots", GetNSlots );
    pm.p( "DrawSlots", DoDrawSlots, SetDrawSlots );
} // SlotList::Expose