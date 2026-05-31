/***********************************************************************************/
/*  File:   sgPRSAnimation.cpp
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "sgPRSAnimation.h"
#include "sgTransformNode.h"

#ifndef _INLINES
#include "sgPRSAnimation.inl"
#endif // _INLINES

IMPLEMENT_CLASS( PRSAnimation );

/*****************************************************************************/
/*    PRSAnimation implementation
/*****************************************************************************/
bool    Animation::s_bFrozen                    = false;
bool    Animation::s_bAnimateInvisible          = false;

void PRSAnimation::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    
    posX.Serialize( os );
    posY.Serialize( os );
    posZ.Serialize( os );
    rot .Serialize( os );
    scX .Serialize( os );
    scY .Serialize( os );
    scZ .Serialize( os );

    os << m_BaseAnimationName;
} // PRSAnimation::Serialize

void PRSAnimation::Unserialize( InStream& is )
{
    Parent::Unserialize( is );

    posX.Unserialize( is );
    posY.Unserialize( is );
    posZ.Unserialize( is );
    rot .Unserialize( is );
    scX .Unserialize( is );
    scY .Unserialize( is );
    scZ .Unserialize( is );

    is >> m_BaseAnimationName;
    m_AnimationTime = CalculateMaxTime();
} // PRSAnimation::Unserialize

//  blend between two animations
Matrix4D PRSAnimation::GetTransform(    const PRSAnimation* anm1, float time1,
                                        const PRSAnimation* anm2, float time2,
                                        float blendFactor )
{
    Vector3D sc1(    anm1->scX.GetValue( time1 ), 
                    anm1->scY.GetValue( time1 ), 
                    anm1->scZ.GetValue( time1 ) );

    Vector3D sc2(    anm2->scX.GetValue( time2 ), 
                    anm2->scY.GetValue( time2 ), 
                    anm2->scZ.GetValue( time2 ) );

    Quaternion quat1 = anm1->rot.GetValue( time1 );
    Quaternion quat2 = anm2->rot.GetValue( time2 );

    Vector3D tr1(    anm1->posX.GetValue( time1 ), 
                    anm1->posY.GetValue( time1 ), 
                    anm1->posZ.GetValue( time1 ) );

    Vector3D tr2(    anm2->posX.GetValue( time2 ), 
                    anm2->posY.GetValue( time2 ), 
                    anm2->posZ.GetValue( time2 ) );
    
    Vector3D    sc; 
    Quaternion    quat;  
    Vector3D    tr;  

    sc.addWeighted( sc1, sc2, 1.0f - blendFactor, blendFactor );
    quat.Slerp( quat1, quat2, blendFactor );
    tr.addWeighted( tr1, tr2, 1.0f - blendFactor, blendFactor );

    return Matrix4D( sc, quat, tr );
} // PRSAnimation::GetTransform

void PRSAnimation::Render()
{
    if (s_bFrozen) return;
    float curTime = CurTime();

    TransformNode* pNode = dynamic_cast<TransformNode*>( m_pOperated );
    if (!pNode) return;
    if (!s_bAnimateInvisible && pNode->IsInvisible()) return;
    
    //  check if need to rebind node
    //if (m_pOperatedParent && m_pOperatedParent != pNode->GetParent()) 
    //{
    //    pNode->AddRef();
    //    pNode->GetParent()->RemoveChild( pNode );
    //    m_pOperatedParent->AddChild( pNode );
    //    pNode->Release();
    //}
    
    float time = float( curTime );
	if (!IsDisabled()){
		/*
		const Matrix4D& M0=pNode->GetTransform();
		Matrix4D M1=GetTransform( time );
		Vector3D V0(M0.e30,M0.e31,M0.e32);
		Vector3D V1(M1.e30,M1.e31,M1.e32);
        float r1=V0.norm();
		float r2=V1.norm();
		float sc=r2/r1;
		M1.e30*=sc;
		M1.e31*=sc;
		M1.e32*=sc;
		pNode->SetTransform( M1 );
		*/
		pNode->SetTransform( GetTransform( time ) );
	}
} // PRSAnimation::Render

bool PRSAnimation::IsConstant()
{
    if (posX.GetNKeys() + posY.GetNKeys() + posZ.GetNKeys() > 0) return false;
    if (scX.GetNKeys() + scY.GetNKeys() + scZ.GetNKeys() > 0) return false;
    if (rot.GetNKeys() > 0) return false;
    return true;
} // PRSAnimation::IsConstant

float PRSAnimation::CalculateMaxTime() const
{
    return tmax( tmax( scX.GetMaxTime(), scY.GetMaxTime(), scZ.GetMaxTime() ), 
                 rot.GetMaxTime(), 
                 tmax( posX.GetMaxTime(), posY.GetMaxTime(), posZ.GetMaxTime() ) );
}

void PRSAnimation::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PRSAnimation", this );
    pm.p( "BaseAnimationName", &PRSAnimation::GetBaseAnimationName, &PRSAnimation::SetBaseAnimationName );

    pm.p( "ScaleX", &PRSAnimation::GetScaleX, &PRSAnimation::SetScaleXAnimation, "floatAnimCurve" );
    pm.p( "ScaleY", &PRSAnimation::GetScaleY, &PRSAnimation::SetScaleYAnimation, "floatAnimCurve" );
    pm.p( "ScaleZ", &PRSAnimation::GetScaleZ, &PRSAnimation::SetScaleZAnimation, "floatAnimCurve" );
    pm.p( "PosX", &PRSAnimation::GetPosX, &PRSAnimation::SetPosXAnimation, "floatAnimCurve" );
    pm.p( "PosY", &PRSAnimation::GetPosY, &PRSAnimation::SetPosYAnimation, "floatAnimCurve" );
    pm.p( "PosZ", &PRSAnimation::GetPosZ, &PRSAnimation::SetPosZAnimation, "floatAnimCurve" );
    pm.p( "Rotation", &PRSAnimation::GetRot, &PRSAnimation::SetRotAnimation, "quatAnimCurve" );

    pm.m( "FlipXAxis", &PRSAnimation::FlipXAxis );
    pm.m( "FlipYAxis", &PRSAnimation::FlipYAxis );
    pm.m( "FlipZAxis", &PRSAnimation::FlipZAxis );
} // PRSAnimation::Expose

void PRSAnimation::FlipXAxis()
{
    int nKeys = rot.GetNKeys();
    for (int i = 0; i < nKeys; i++)
    {
        Quaternion q = rot.GetKeyValue( i );
        
        Matrix3D m( q );
        m.getV1().reverse();
        m.getV2().reverse();
        q.FromMatrix( m );

        rot.SetKeyValue( i, q );
    }
    rot.CorrectOrientation();

    Quaternion q = rot.GetDefaultValue();
    Matrix3D m( q );
    m.getV1().reverse();
    m.getV2().reverse();
    q.FromMatrix( m );
    rot.SetDefaultValue( q );
} // PRSAnimation::FlipXAxis

void PRSAnimation::FlipYAxis()
{
    int nKeys = rot.GetNKeys();
    for (int i = 0; i < nKeys; i++)
    {
        Quaternion q = rot.GetKeyValue( i );

        Matrix3D m( q );
        m.getV0().reverse();
        m.getV2().reverse();
        q.FromMatrix( m );

        rot.SetKeyValue( i, q );
    }
    rot.CorrectOrientation();

    Quaternion q = rot.GetDefaultValue();
    Matrix3D m( q );
    m.getV0().reverse();
    m.getV2().reverse();
    q.FromMatrix( m );
    rot.SetDefaultValue( q );
} // PRSAnimation::FlipYAxis

void PRSAnimation::FlipZAxis()
{
    int nKeys = rot.GetNKeys();
    for (int i = 0; i < nKeys; i++)
    {
        Quaternion q = rot.GetKeyValue( i );

        Matrix3D m( q );
        m.getV0().reverse();
        m.getV1().reverse();
        q.FromMatrix( m );

        rot.SetKeyValue( i, q );
    }
    rot.CorrectOrientation();

    Quaternion q = rot.GetDefaultValue();
    Matrix3D m( q );
    m.getV0().reverse();
    m.getV1().reverse();
    q.FromMatrix( m );
    rot.SetDefaultValue( q );
} // PRSAnimation::FlipZAxis

void PRSAnimation::AddKey( float keyTime, const Matrix4D& tm )
{
    Vector3D scale, trans;
    Quaternion qrot;
    tm.Factorize( scale, qrot, trans );
    posX.AddKey( keyTime, trans.x );
    posY.AddKey( keyTime, trans.y );
    posZ.AddKey( keyTime, trans.z );
    scX.AddKey( keyTime, scale.x );
    scY.AddKey( keyTime, scale.y );
    scZ.AddKey( keyTime, scale.z );
    rot.AddKey( keyTime, qrot );
}

void PRSAnimation::ReduceKeys( float scaleBias, float rotBias, float posBias )
{
    rot.Normalize           ();
    rot.CorrectOrientation  ();
    posX.LinearReduceKeys   ( posBias     );
    posY.LinearReduceKeys   ( posBias     );
    posZ.LinearReduceKeys   ( posBias     );
    rot.LinearReduceKeys    ( rotBias     );
    scX.LinearReduceKeys    ( scaleBias );
    scY.LinearReduceKeys    ( scaleBias );
    scZ.LinearReduceKeys    ( scaleBias );
} // PRSAnimation::ReduceKeys

float PRSAnimation::GetScaleDiff( float anmTime, const Vector3D& sc )
{
    Vector3D cSc(    scX.GetValue( anmTime ), 
                    scY.GetValue( anmTime ),
                    scZ.GetValue( anmTime ) );
    cSc -= sc;
    return cSc.norm2();
} // PRSAnimation::GetScaleDiff

float PRSAnimation::GetPosDiff( float anmTime, const Vector3D& pos )
{
    Vector3D cPos(    posX.GetValue( anmTime ), 
                    posY.GetValue( anmTime ),
                    posZ.GetValue( anmTime ) );
    cPos -= pos;
    return cPos.norm2();
} // PRSAnimation::GetPosDiff

float PRSAnimation::GetRotDiff( float anmTime, const Quaternion& quat )
{
    Quaternion cQuat( rot.GetValue( anmTime ) );
    cQuat -= quat;
    return cQuat.norm2();
} // PRSAnimation::GetRotDiff
