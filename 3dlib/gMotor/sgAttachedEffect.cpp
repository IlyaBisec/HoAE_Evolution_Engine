#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "kIOHelpers.h"
#include "kMathTypeTraits.h"
#include "sgTransformNode.h"
#include "sgAttachedEffect.h"
#include "kContext.h"
#include "IMediaManager.h"
#include "IResourceManager.h"


IMPLEMENT_CLASS( AttachedEffect );

AttachedEffect::AttachedEffect(){
    m_EffectID = -1;
    m_EffectName = "";    
    m_EffectMask = 1;
    m_EffectScale = 1.0f;
    m_DirectionVector = Vector3D(1,0,0);
    m_RelativePosition = Vector3D::null;
}
void AttachedEffect::DrawEffect( Matrix4D& TM ){
    if( m_EffectID == 0xFFFFFFFF ){
        m_EffectID = IMM->GetModelID( m_EffectName.c_str() );
    }
    PushEntityContext( (DWORD) this );
    DWORD eff = m_EffectID;
    if(eff==0xFFFFFFFF){
        eff = IMM->GetModelID( "Effects\\default.c2m" );
    }
    IMM->StartModel(eff,TM,DWORD(this));
    IMM->AnimateModel(eff,0);
    IMM->DrawModel();
    PopEntityContext ();
}
void AttachedEffect::Render(){
    if(!(IMM->GetAttachedEffectMask() & m_EffectMask) )return;
    SNode* n=GetParent();
    if(n){
        Matrix4D M;
        if(n->m_bTransform)M = ((TransformNode*)n)->GetTopTM();
        else M = TransformNode::TMStackTop();
        //Matrix4D M = tn->GetTopTM();
        Matrix4D M2;
        Vector3D VX=m_DirectionVector;
        Vector3D VY,VZ;
        if(VX.x==0 && VX.y==0){
            VX.normalize();
            VY=Vector3D(0,1,0);              
            VZ.cross(VX,VY);
            VZ.normalize();
            VY.cross(VZ,VX);
        }else{
            VX.normalize();                
            VZ=Vector3D(0,0,1);              
            VY.cross(VZ,VX);
            VY.normalize();
            VZ.cross(VX,VY);                
        }
        VX*=m_EffectScale;
        VY*=m_EffectScale;
        VZ*=m_EffectScale;
        M2.e00=VX.x;M2.e01=VX.y;M2.e02=VX.z;M2.e03=0.0f;
        M2.e10=VY.x;M2.e11=VY.y;M2.e12=VY.z;M2.e13=0.0f;
        M2.e20=VZ.x;M2.e21=VZ.y;M2.e22=VZ.z;M2.e23=0.0f;
        M2.e30=m_RelativePosition.x;
        M2.e31=m_RelativePosition.y;
        M2.e32=m_RelativePosition.z;
        M2.e33=1.0f;
        M.mulLeft(M2);
        DrawEffect( M );
    }
}
void AttachedEffect::Serialize( OutStream& os ) const {
    Parent::Serialize( os );
    std::string s1=m_EffectName;
    IRM->ConvertPathToRelativeForm( s1 );
    os << s1 << m_EffectMask << m_EffectScale << m_DirectionVector.x << m_DirectionVector.y << m_DirectionVector.z << m_RelativePosition.x << m_RelativePosition.y << m_RelativePosition.z;
}
void AttachedEffect::Unserialize( InStream& is ){
    Parent::Unserialize( is );
    std::string s;
    is >> s >> m_EffectMask >> m_EffectScale >> m_DirectionVector.x >> m_DirectionVector.y >> m_DirectionVector.z >> m_RelativePosition.x >> m_RelativePosition.y >> m_RelativePosition.z;
    IRM->ConvertPathToRelativeForm(s);
    m_EffectName = s;
	_chdir(IRM->GetHomeDirectory());
	m_EffectID = IMM->GetModelID( s.c_str() );
}
void AttachedEffect::Expose( PropertyMap& pm ){
    pm.start<Parent>( "AttachedEffect", this );    
    pm.p( "Effect", &AttachedEffect::GetEffectName, &AttachedEffect::SetEffectName, "#model" );
    pm.f( "EffectMask", m_EffectMask );
    pm.f( "EffectScale", m_EffectScale );

    pm.f( "DirectionVectorX", m_DirectionVector.x );
    pm.f( "DirectionVectorY", m_DirectionVector.y );
    pm.f( "DirectionVectorZ", m_DirectionVector.z );

    pm.f( "RelativePositionX", m_RelativePosition.x );
    pm.f( "RelativePositionY", m_RelativePosition.y );
    pm.f( "RelativePositionZ", m_RelativePosition.z );
}
void TransformNode::AttachEffect(){
    AttachedEffect* ef = new AttachedEffect;
    ef->SetName( "Effect" );
    AddChild( ef );
}