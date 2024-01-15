/***********************************************************************************/
/*  File:   sgUVAnimation.cpp
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "sgUVAnimation.h"
#include "sgTextureMatrix.h"

IMPLEMENT_CLASS( UVAnimation );

/*****************************************************************************/
/*    UVAnimation implementation
/*****************************************************************************/
UVAnimation::UVAnimation()
{
}

Matrix3D UVAnimation::GetTransform( float time ) const
{
	float scU  = m_ScU.GetValue ( time );
	float scV  = m_ScV.GetValue ( time );
	float posU = m_PosU.GetValue( time );
	float posV = m_PosV.GetValue( time );
	float rot  = m_Rot.GetValue ( time );

	float cosPhi = cosf( rot );
	float sinPhi = sinf( rot );
	
	Matrix3D m;
	m.e00 = scU*cosPhi;	
	m.e01 = scU*sinPhi;	
	m.e10 = -scV*sinPhi;	
	m.e11 = scV*cosPhi;	
	m.e20 = posU;
	m.e21 = posV;
	m.e02 = 0.0f;
	m.e12 = 0.0f;
	m.e22 = 1.0f;

	return m;
} // UVAnimation::GetTransform

void UVAnimation::Render()
{
    if (s_bFrozen) return;
    float curTime = CurTime();
    for (int i = 0; i < GetNChildren(); i++)
    {
        TextureMatrix* pNode = (TextureMatrix*)GetChild( i );
        if (!s_bAnimateInvisible && pNode->IsInvisible()) continue;
        if (Owns( pNode ) && !pNode->IsInvisible()) 
        {
            pNode->Render();
            continue;
        }
        if (!pNode->IsA<TextureMatrix>()) continue;
        float time = float( curTime );

        if (!IsDisabled()) pNode->SetTextureTM( GetTransform( time ) );
    }
} // UVAnimation::Render

void UVAnimation::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    
    m_PosU.Serialize( os );
    m_PosV.Serialize( os );
    m_ScU.Serialize( os );
    m_ScV.Serialize( os );
    m_Rot.Serialize( os );
} // UVAnimation::Serialize

void UVAnimation::Unserialize( InStream& is    )
{
    Parent::Unserialize( is );
    
    m_PosU.Unserialize( is );
    m_PosV.Unserialize( is );
    m_ScU.Unserialize( is );
    m_ScV.Unserialize( is );
    m_Rot.Unserialize( is );
} // UVAnimation::Unserialize

void UVAnimation::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "UVAnimation", this );
    pm.p( "ScaleU", &UVAnimation::GetScU, &UVAnimation::SetScU,  "floatAnimCurve" );
    pm.p( "ScaleV", &UVAnimation::GetScV, &UVAnimation::SetScV,  "floatAnimCurve" );
    pm.p( "PosU", &UVAnimation::GetPosU, &UVAnimation::SetPosU, "floatAnimCurve" );
    pm.p( "PosV", &UVAnimation::GetPosV, &UVAnimation::SetPosV, "floatAnimCurve" );
    pm.p( "Rotation", &UVAnimation::GetRot, &UVAnimation::SetRot,  "floatAnimCurve" );
} // UVAnimation::Expose