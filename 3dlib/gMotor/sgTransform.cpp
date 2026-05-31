/*****************************************************************************/
/*    File:    sgTransform.cpp
/*    Desc:    
/*    Author:  Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "kIOHelpers.h"
#include "kMathTypeTraits.h"
#include "sgTransform.h"

IMPLEMENT_CLASS( Transform );
/*****************************************************************************/
/*  Transform implementation
/*****************************************************************************/
void Transform::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_RestLocalTM;
    m_LocalTM = m_RestLocalTM;
} // Transform::Unserialize

void Transform::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_LocalTM;
} // Transform::Serialize

void Transform::Render()
{
} // Transform::Render

void Transform::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Transform", this );
} // Transform::Expose

