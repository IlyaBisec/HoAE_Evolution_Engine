/*****************************************************************************/
/*	File:	vIKEffector.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#include "stdafx.h"
#include "vIKEffector.h"

/*****************************************************************************/
/*  IKEffector implementation
/*****************************************************************************/
IKEffector::IKEffector()
{
    m_ID            = -1;		   
    m_Name          = "";        
    m_Flags         = 0;       
} // IKEffector::IKEffector

void IKEffector::Expose( PropertyMap& pm )
{
    pm.start( "IKEffector", this );
    pm.p( "NodeType", ClassName );
    pm.p( "Name", GetName, SetName );
} // IKEffector::Expose

/*****************************************************************************/
/*  EffectorList implementation
/*****************************************************************************/
void EffectorList::Expose( PropertyMap& pm )
{
    pm.start( "EffectorList", this );
    pm.p( "NodeType", ClassName );
    pm.p( "NumEffectors", GetNEffectors );
} // EffectorList::Expose