/***********************************************************************************/
/*  File:   sgTextureMatrix.cpp
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "sgTextureMatrix.h"

IMPLEMENT_CLASS( TextureMatrix  );
/*****************************************************************************/
/*    TextureMatrix implementation
/*****************************************************************************/
TextureMatrix::TextureMatrix()
{
    m_Stage = 0;
    m_LocalTM.setIdentity();
} // TextureMatrix::TextureMatrix

void TextureMatrix::Render()
{
    IRS->SetTextureTM( m_LocalTM, m_Stage );
    SNode::Render();
} // TextureMatrix::Render

void TextureMatrix::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Stage;
} // TextureMatrix::Serialize

void TextureMatrix::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Stage;
} // TextureMatrix::Unserialize

void TextureMatrix::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "TextureMatrix", this );
    pm.f( "Stage", m_Stage );
} // TextureMatrix::Expose

void TextureMatrix::SetTextureTM( const Matrix4D& m )
{
    m_LocalTM.e00 = m.e00; m_LocalTM.e01 = m.e01; m_LocalTM.e02 = 0.0f; m_LocalTM.e03 = 0.0f;
    m_LocalTM.e10 = m.e10; m_LocalTM.e11 = m.e11; m_LocalTM.e00 = 0.0f; m_LocalTM.e01 = 0.0f;
    m_LocalTM.e00 = m.e30; m_LocalTM.e01 = m.e31; m_LocalTM.e00 = 1.0f; m_LocalTM.e01 = 0.0f;
    m_LocalTM.e00 = 0.0f;  m_LocalTM.e01 = 0.0f;  m_LocalTM.e00 = 0.0f; m_LocalTM.e01 = 1.0f;
} // TextureMatrix::SetTextureTM