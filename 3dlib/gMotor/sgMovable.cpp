/*****************************************************************************/
/*    File:    sgNode.cpp
/*    Desc:    Scene graph node
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "kIOHelpers.h"
#include "kMathTypeTraits.h"
#include "sgMovable.h"

#ifndef _INLINES 
#include "sgMovable.inl"
#endif // _INLINES

IMPLEMENT_CLASS( TransformNode  );
IMPLEMENT_CLASS( HudNode        );
IMPLEMENT_CLASS( Transform2D    );

/*****************************************************************************/
/*    TransformNode implementation
/*****************************************************************************/
MatrixStack     TransformNode::s_TMStack;
bool            TransformNode::s_bCalcWorldTM = false;

void TransformNode::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_InitialTM;
    m_LocalTM = m_InitialTM;
} // TransformNode::Unserialize

void TransformNode::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << GetTransform();
} // TransformNode::Serialize

void TransformNode::Render()
{
    if (s_bCalcWorldTM) PushTM( m_LocalTM );
    else PushTM( m_WorldTM, true );
    
   /* rsEnableZ( false );
    IRS->ResetWorldTM();
    DrawArrow( m_WorldTM.getTranslation(), m_WorldTM.getV2(), 0xFFFF0000, 100.0f );
    rsFlush();*/
    ///char buf[256];
    //sprintf( buf, "%s: %f %f %f\n", GetName(), m_WorldTM.e30, m_WorldTM.e31, m_WorldTM.e32 );
    //OutputDebugString( buf );
    
    m_WorldTM = GetTopTM();
    SNode::Render();
    PopTM();
} // TransformNode::Render

void TransformNode::SetTransform( const Matrix4D& matr )
{ 
    m_LocalTM = matr;
}

void TransformNode::ResetTMStack( const Matrix4D* pTm )
{
    if (pTm) s_TMStack.Reset( *pTm ); else s_TMStack.Reset();
} // TransformNode::ResetTMStack

void TransformNode::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "TransformNode", this );
    pm.p( "ScaleX", GetScaleX, SetScaleX );
    pm.p( "ScaleY", GetScaleY, SetScaleY );
    pm.p( "ScaleZ", GetScaleZ, SetScaleZ );

    pm.p( "PosX", GetPosX, SetPosX );
    pm.p( "PosY", GetPosY, SetPosY );
    pm.p( "PosZ", GetPosZ, SetPosZ );

    /*pm.p( "RotX", GetEulerX, SetEulerX );
    pm.p( "RotY", GetEulerY, SetEulerY );
    pm.p( "RotZ", GetEulerZ, SetEulerZ );*/
    pm.m( "Reset", Reset );
    pm.m( "SetToInitial", SetToInitial );
    pm.m( "SetSubtreeToInitial", SetSubtreeToInitial );
    pm.m( "FlipAxis", FlipAxis );
} // TransformNode::Expose

void TransformNode::FlipAxis()
{
    m_LocalTM.getV1().reverse();
    m_LocalTM.getV2().reverse();
}

void TransformNode::SetToInitial()
{
    m_LocalTM = m_InitialTM;
}

void TransformNode::SetSubtreeToInitial()
{
    Iterator it( this, TransformNode::FnFilter );
    while (it)
    {
        ((TransformNode*)*it)->SetToInitial();
        ++it;
    }
} // TransformNode::SetSubtreeToInitial

Matrix4D TransformNode::GetWorldTM() const
{
    Matrix4D m = GetTransform();
    m *= GetParentWorldTM();
    return m;
} // TransformNode::GetWorldTM

Matrix4D TransformNode::GetParentWorldTM() const
{
    Matrix4D m = Matrix4D::identity;
    SNode* pNode = GetParent();
    while (pNode)
    {
        if (pNode->IsA<TransformNode>())
        {
            m *= ((TransformNode*)pNode)->GetTransform();
        }
		SNode* pParent = pNode->GetParent();
        if (pNode == pParent) 
		{
			Log.Error( "SNode %s is parent of its own!!!", pNode->GetName() );
			break;
		}
		pNode = pParent; 
    }
    return m;
} // TransformNode::GetParentWorldTM

void TransformNode::SetWorldTM( const Matrix4D& wTM )
{
    Matrix4D pTM = GetParentWorldTM();
    Matrix4D lTM;
    lTM.inverse( pTM );
    lTM.mulLeft( wTM );
    SetTransform( lTM );
} // TransformNode::SetWorldTM

 float TransformNode::GetEulerX() const
{
    Matrix3D rot = m_LocalTM;
    return RadToDeg( rot.EulerXYZ().x );
} // TransformNode::GetEulerX

 float TransformNode::GetEulerY() const
{
    Matrix3D rot = m_LocalTM;
    return RadToDeg( rot.EulerXYZ().y );
} // TransformNode::GetEulerY

 float TransformNode::GetEulerZ() const
{
    Matrix3D rot = m_LocalTM;
    return RadToDeg( rot.EulerXYZ().z );
} // TransformNode::GetEulerZ

 void TransformNode::SetEulerX( float val )
{
    clamp( val, -180.0f, 180.0f );
    float sx = m_LocalTM.getV0().norm();
    float sy = m_LocalTM.getV1().norm();
    float sz = m_LocalTM.getV2().norm();

    Matrix3D rot;
    rot.rotation( DegToRad( val ), DegToRad( GetEulerY() ), DegToRad( GetEulerZ() ) );
    m_LocalTM.setRotation( rot );

    m_LocalTM.getV0() *= sx;
    m_LocalTM.getV1() *= sy;
    m_LocalTM.getV2() *= sz;
} // TransformNode::SetEulerX

 void TransformNode::SetEulerY( float val )
{
    clamp( val, -180.0f, 180.0f );
    float sx = m_LocalTM.getV0().norm();
    float sy = m_LocalTM.getV1().norm();
    float sz = m_LocalTM.getV2().norm();

    Matrix3D rot;
    rot.rotation( DegToRad( GetEulerX() ), DegToRad( val ), DegToRad( GetEulerZ() ) );
    m_LocalTM.setRotation( rot );

    m_LocalTM.getV0() *= sx;
    m_LocalTM.getV1() *= sy;
    m_LocalTM.getV2() *= sz;
} // TransformNode::SetEulerY

 void TransformNode::SetEulerZ( float val )
{
    clamp( val, -180.0f, 180.0f );
    float sx = m_LocalTM.getV0().norm();
    float sy = m_LocalTM.getV1().norm();
    float sz = m_LocalTM.getV2().norm();

    Matrix3D rot;
    rot.rotation( DegToRad( GetEulerX() ), DegToRad( GetEulerY() ), DegToRad( val ) );
    m_LocalTM.setRotation( rot );

    m_LocalTM.getV0() *= sx;
    m_LocalTM.getV1() *= sy;
    m_LocalTM.getV2() *= sz;
} // TransformNode::SetEulerZ

/*****************************************************************************/
/*    HudNode implementation
/*****************************************************************************/
void HudNode::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << pos << width << height << scale;
} // HudNode::Serialize

void HudNode::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> pos >> width >> height >> scale;
} // HudNode::Unserialize

Rct HudNode::GetBounds()
{
    return Rct( pos.x, pos.y, width, height );
}

void HudNode::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "HudNode", this );
    pm.p( "PosX",        GetPosX,    SetPosX );
    pm.p( "PosY",        GetPosY,    SetPosY );
    pm.p( "PosZ",        GetPosZ,    SetPosZ );

    pm.p( "Width",        GetWidth,    SetWidth );
    pm.p( "Height",    GetHeight,    SetHeight );

    pm.p( "Scale",        GetScale,    SetScale );
} // HudNode::Expose

/*****************************************************************************/
/*    Transform2D implementation
/*****************************************************************************/
void Transform2D::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_LocalTM;
}

void Transform2D::Unserialize( InStream& is    )
{
    Parent::Unserialize( is );
    is >> m_LocalTM;
}


Matrix4D GetWorldTM( SNode* pNode )
{
    while (pNode && !pNode->IsA<TransformNode>()) 
    {
        SNode* pParent = pNode->GetParent();
        if (pParent == pNode) break;
        pNode = pParent;
    }
    if (pNode) return ((TransformNode*)pNode)->GetWorldTM();
    return Matrix4D::identity;
}


