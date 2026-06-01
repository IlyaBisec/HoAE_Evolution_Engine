/*****************************************************************************/
/*    File:    vCollider.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#include "stdafx.h"
#include "vCollider.h"

/*****************************************************************************/
/*  Collider implementation
/*****************************************************************************/
Collider::Collider()
{
    m_ID            = -1;           
    m_Name          = "";        
    m_Flags         = 0;       
} // Collider::Collider

void Collider::Expose( PropertyMap& pm )
{
    pm.start( "Collider", this );
    pm.p( "NodeType", &Collider::ClassName );
    pm.p( "Name", &Collider::GetName, &Collider::SetName );
} // Collider::Expose

/*****************************************************************************/
/*  ColliderList implementation
/*****************************************************************************/
void ColliderList::Expose( PropertyMap& pm )
{
    pm.start( "ColliderList", this );
    pm.p( "NodeType", &ColliderList::ClassName );
    pm.p( "NumColliders", &ColliderList::GetNColliders );
} // ColliderList::Expose