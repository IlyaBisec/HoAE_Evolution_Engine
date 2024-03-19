/*****************************************************************************/
/*    File:    sgLight.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"

#ifndef _INLINES
#include "sgLight.inl"
#endif // _INLINES

IMPLEMENT_CLASS( LightSource           );
IMPLEMENT_CLASS( PointLight            );
IMPLEMENT_CLASS( DirectionalLight      );
IMPLEMENT_CLASS( SpotLight             );

/*****************************************************************************/
/*    LightSource implementation
/*****************************************************************************/
void LightSource::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Ambient >> m_Diffuse >> m_Specular;
} // LightSource::Unserialize

void LightSource::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Ambient << m_Diffuse << m_Specular;
} // LightSource::Serialize

void LightSource::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "LightSource", this );
    pm.f( "Diffuse",    m_Diffuse,    "color" );
    pm.f( "Ambient",    m_Ambient,    "color" );
    pm.f( "Specular",    m_Specular, "color" );
} // LightSource::Expose

void LightSource::Render()
{
    //AddActiveLight( this );
}

Frustum LightSource::GetFrustum( const AABoundBox& aabb ) const
{
    Frustum res;
    res.Create( GetTransform(), 1.0f, 1.0f, 1.0f, 0.0f, 10000.0f );
    return res;
} // LightSource::GetFrustum

/*****************************************************************************/
/*    DirectionalLight implementation
/*****************************************************************************/
void DirectionalLight::Render()
{
    Parent::Render();
    IRS->SetDirLight( this, m_Index );
} // DirectionalLight::Render

void DirectionalLight::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "DirectionalLight", this );
} // DirectionalLight::Expose

Frustum DirectionalLight::GetFrustum( const AABoundBox& aabb ) const
{
    Frustum fr;
    float radius = aabb.GetDiagonal() * 0.5f;
    float dist   = GetPos().distance( aabb.GetCenter() );

    Matrix4D ftm = GetTransform();
    Vector3D pos = aabb.GetCenter();
    Vector3D dir = GetDir();
    dir *= tmax( dist, radius );
    pos -= dir;
    ftm.setTranslation( pos );
    fr.Create( ftm, radius*2.0f, radius*2.0f, 1.0f, dist - radius, dist + radius );
    return fr;
} // DirectionalLight::GetFrustum

/*****************************************************************************/
/*    PointLight implementation
/*****************************************************************************/
PointLight::PointLight()
{
    m_Range = 10000.0f;
    m_AttA  = 1.0f;
    m_AttB  = 0.0f;
    m_AttC  = 0.0f;
}

PointLight::PointLight( const Vector3D& _pos, const Vector3D& _dir )
{
    SetPos( _pos );
    SetDir( _dir );

    m_Range      = 10000.0f;

    m_Ambient    = 0xFF000000;    
    m_Diffuse    = 0xFFFFFFFF;
    m_Specular   = 0xFFFFFFFF;

} // PointLight::PointLight

void PointLight::Render()
{
    Parent::Render();
    IRS->SetPointLight( this, m_Index );
} // PointLight::Render

void PointLight::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PointLight", this );
    pm.f( "Range", m_Range );
    pm.f( "Attenuation A", m_AttA  );
    pm.f( "Attenuation B", m_AttB  );
    pm.f( "Attenuation C", m_AttC  );

} // PointLight::Expose

void PointLight::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Range << m_AttA << m_AttB << m_AttC;
} // PointLight::Serialize

void PointLight::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Range >> m_AttA >> m_AttB >> m_AttC;
} // PointLight::Unserialize

Frustum PointLight::GetFrustum( const AABoundBox& aabb ) const
{
    Frustum fr;
    float radius = aabb.GetDiagonal() * 0.5f;
    float dist = GetPos().distance( aabb.GetCenter() );
    float k = tanf( asinf( radius / dist ) ) * 2.0f;
    float wt = (dist - radius) * k;
    float wb = (dist + radius) * k;
    Matrix3D rot;
    rot.getV2() = aabb.GetCenter();
    rot.getV2() -= GetPos();
    rot.getV2().CreateBasis( rot.getV0(), rot.getV1() );
    Matrix4D ftm( Vector3D::one, rot, GetTransform().getTranslation() );
    fr.Create( ftm, wt, wb, 1.0f, dist - radius, dist + radius );
    return fr;
} // PointLight::GetFrustum

/*****************************************************************************/
/*    SpotLight implementation
/*****************************************************************************/
void SpotLight::Render()
{
    Parent::Render();
    IRS->SetSpotLight( this, m_Index );
} // SpotLight::Render

void SpotLight::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "SpotLight", this );
    pm.f( "Inner Cone", m_InnerCone  );
    pm.f( "Outer Cone", m_InnerCone  );
    pm.f( "Cone Falloff", m_ConeFalloff  );

} // SpotLight::Expose

void SpotLight::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_InnerCone << m_OuterCone << m_ConeFalloff;
} // SpotLight::Serialize     
         
void SpotLight::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_InnerCone >> m_OuterCone >> m_ConeFalloff;
} // SpotLight::Unserialize

Frustum SpotLight::GetFrustum( const AABoundBox& aabb ) const
{
    assert( false );
    return Parent::GetFrustum( aabb );
} // SpotLight::GetFrustum



