#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgShader.h"
#include "sgTexture.h"
#include "sgDummy.h"
#include "sgRoot.h"
#include "sgGeometry.h"
#include "kIOHelpers.h"

#include "mHeightmap.h"
#include "vField.h"
#include "sgFog.h"

IMPLEMENT_CLASS( Fog );
/*****************************************************************************/
/*    Fog implementation
/*****************************************************************************/
Fog::Fog()
{
    m_Color            = 0xFFFFFFFF;
    m_Start            = 1.0f;
    m_End            = 100.0f;

    m_Density        = 0.5f;
    m_Type            = ftVertex;
    m_Mode            = fmLinear;

    m_bRangeBased    = false;
    m_bEnabled        = true;
}

Fog::~Fog()
{
}

void Fog::Render()
{
    if (m_bEnabled)
    {
        //IRS->SetFog( this );
        SNode::Render();
        //IRS->SetFog( NULL );
    }
    else
    {
        SNode::Render();
    }
} // Fog::Render

void Fog::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Color << m_Start << m_End << m_Density;
    BYTE bType = (BYTE) m_Type;
    BYTE bMode = (BYTE) m_Mode;

    os << bType << bMode;
    os << m_bRangeBased;
} // Fog::Serialize

void Fog::Unserialize( InStream& is    )
{
    Parent::Unserialize( is );
    is >> m_Color >> m_Start >> m_End >> m_Density;
    BYTE bType;
    BYTE bMode;
    is >> bType >> bMode;
    m_Type = (FogType)bType;
    m_Mode = (FogMode)bMode;
    is >> m_bRangeBased;
} // Fog::Unserialize

void Fog::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Fog", this );
    pm.f( "Color",        m_Color, "color" );
    pm.f( "Start",        m_Start          );
    pm.f( "End",          m_End            );

    pm.f( "Density",      m_Density        );
    pm.f( "Type",         m_Type           );
    pm.f( "Mode",         m_Mode           );

    pm.f( "Range Based",  m_bRangeBased    );
    pm.f( "Enabled",      m_bEnabled       );
}

