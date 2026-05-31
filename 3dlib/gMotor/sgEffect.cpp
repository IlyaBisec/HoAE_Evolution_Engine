/*****************************************************************************/
/*    File:    sgEffect.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    20-04-2004
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"

#include "vMesh.h"

#include "sgEffect.h"
#include "kTimer.h"
#include "kSystemDialogs.h"
#include "mNoise.h"
#include "mShapes.h"

#include "kFilePath.h"
#include "kColorTraits.h"

#include "ITerrain.h"

#include "IResourceManager.h"
#include "IShadowManager.h"

#ifndef _INLINES
#include "sgEffect.inl"
#endif // _INLINES

const float AutoUpdTime = 2.0f;
const float AutoFadeTime = 1.0f;

PEffectManager g_EMgr;
IEffectManager* IEffMgr = &g_EMgr;

IMPLEMENT_CLASS( PModularEmitter        );
IMPLEMENT_CLASS( PEmitter               );
IMPLEMENT_CLASS( PConstEmitter          );
IMPLEMENT_CLASS( PStaticEmitter         );
IMPLEMENT_CLASS( PBurstEmitter          );
IMPLEMENT_CLASS( PRampEmitter           );

IMPLEMENT_CLASS( POperator              );
IMPLEMENT_CLASS( PInitializer           );

IMPLEMENT_CLASS( PShooter               );
IMPLEMENT_CLASS( PConeShooter           );
IMPLEMENT_CLASS( PDirectShooter         );
IMPLEMENT_CLASS( PRadialShooter         );
IMPLEMENT_CLASS( PRampShooter           );

IMPLEMENT_CLASS( PForce                 );
IMPLEMENT_CLASS( PWind                  );
IMPLEMENT_CLASS( PTorque                );
IMPLEMENT_CLASS( PDrag                  );
IMPLEMENT_CLASS( PVortex                );
IMPLEMENT_CLASS( PAttract               );
IMPLEMENT_CLASS( POrbit                 );
IMPLEMENT_CLASS( PRevolve               );
IMPLEMENT_CLASS( PTurbulence            );
IMPLEMENT_CLASS( PClampVelocity         );
IMPLEMENT_CLASS( PFollow                );
IMPLEMENT_CLASS( PAlphaFade             );
IMPLEMENT_CLASS( PSizeFade              );
IMPLEMENT_CLASS( PAVelRamp              );
IMPLEMENT_CLASS( PVelRamp               );
IMPLEMENT_CLASS( PFluctuate             );
IMPLEMENT_CLASS( PAlphaRamp             );
IMPLEMENT_CLASS( PSizeRamp              );
IMPLEMENT_CLASS( PColorRamp             );
IMPLEMENT_CLASS( PColorRampInit         );
IMPLEMENT_CLASS( PNatcolor              );
IMPLEMENT_CLASS( PFrame                 );
IMPLEMENT_CLASS( PUVMove                );

IMPLEMENT_CLASS( PTarget                );
IMPLEMENT_CLASS( PRetarget              );
IMPLEMENT_CLASS( PLightning             );
IMPLEMENT_CLASS( PHoming                );

IMPLEMENT_CLASS( PPlacer                );
IMPLEMENT_CLASS( PSpherePlacer          );
IMPLEMENT_CLASS( PPointPlacer           );
IMPLEMENT_CLASS( PTargetPlacer          );
IMPLEMENT_CLASS( PBoxPlacer             );
IMPLEMENT_CLASS( PCylinderPlacer        );
IMPLEMENT_CLASS( PLinePlacer            );
IMPLEMENT_CLASS( PCirclePlacer          );
IMPLEMENT_CLASS( PModelPlacer           );
IMPLEMENT_CLASS( PCoastBreak            );

IMPLEMENT_CLASS( PSizeInit              );
IMPLEMENT_CLASS( PColorInit             );
IMPLEMENT_CLASS( PFrameInit             );

IMPLEMENT_CLASS( PRenderer              );
IMPLEMENT_CLASS( PModelRenderer         );
IMPLEMENT_CLASS( PModelRendererWithStop );
IMPLEMENT_CLASS( PTerrainDecal          );
IMPLEMENT_CLASS( PWaterDecal            );
IMPLEMENT_CLASS( PMouseBind             );
IMPLEMENT_CLASS( PChainRenderer         );        
IMPLEMENT_CLASS( PSphereRenderer        );
IMPLEMENT_CLASS( PBillboardRenderer     );
IMPLEMENT_CLASS( PConeRenderer          );

IMPLEMENT_CLASS( PTrigger               );
IMPLEMENT_CLASS( POnDeath               );
IMPLEMENT_CLASS( POnBirth               );
IMPLEMENT_CLASS( POnTimer               );
IMPLEMENT_CLASS( POnHitGround           );
IMPLEMENT_CLASS( POnHitWater            );

IMPLEMENT_CLASS( PEffectManager         );
IMPLEMENT_CLASS( PEffect                );

IMPLEMENT_CLASS( Overlay                );

void LinkEffects()
{
LINK_CLASS( PModularEmitter         );
LINK_CLASS( PEmitter                );
LINK_CLASS( PConstEmitter           );
LINK_CLASS( PStaticEmitter          );
LINK_CLASS( PBurstEmitter           );
LINK_CLASS( PRampEmitter            );

LINK_CLASS( POperator               );
LINK_CLASS( PInitializer            );

LINK_CLASS( PShooter                );
LINK_CLASS( PConeShooter            );
LINK_CLASS( PDirectShooter          );
LINK_CLASS( PRadialShooter          );
LINK_CLASS( PRampShooter            );

LINK_CLASS( PForce                  );
LINK_CLASS( PWind                   );
LINK_CLASS( PTorque                 );
LINK_CLASS( PDrag                   );
LINK_CLASS( PVortex                 );
LINK_CLASS( PAttract                );
LINK_CLASS( POrbit                  );
LINK_CLASS( PRevolve                );
LINK_CLASS( PTurbulence             );
LINK_CLASS( PClampVelocity          );
LINK_CLASS( PFollow                 );
LINK_CLASS( PAlphaFade              );
LINK_CLASS( PSizeFade               );
LINK_CLASS( PAVelRamp               );
LINK_CLASS( PVelRamp                );
LINK_CLASS( PFluctuate              );
LINK_CLASS( PAlphaRamp              );
LINK_CLASS( PSizeRamp               );
LINK_CLASS( PColorRamp              );
LINK_CLASS( PNatcolor               );
LINK_CLASS( PColorRampInit          );
LINK_CLASS( PFrame                  );
LINK_CLASS( PUVMove                 );

LINK_CLASS( PTarget                 );
LINK_CLASS( PRetarget               );
LINK_CLASS( PLightning              );
LINK_CLASS( PHoming                 );

LINK_CLASS( PPlacer                 );
LINK_CLASS( PSpherePlacer           );
LINK_CLASS( PPointPlacer            );
LINK_CLASS( PTargetPlacer           );
LINK_CLASS( PBoxPlacer              );
LINK_CLASS( PCylinderPlacer         );
LINK_CLASS( PLinePlacer             );
LINK_CLASS( PCirclePlacer           );
LINK_CLASS( PModelPlacer            );
LINK_CLASS( PCoastBreak             );

LINK_CLASS( PSizeInit               );
LINK_CLASS( PColorInit              );
LINK_CLASS( PFrameInit              );

LINK_CLASS( PRenderer               );
LINK_CLASS( PModelRenderer          );
LINK_CLASS( PModelRendererWithStop  );
LINK_CLASS( PTerrainDecal           );
LINK_CLASS( PWaterDecal             );
LINK_CLASS( PMouseBind              );
LINK_CLASS( PChainRenderer          );        
LINK_CLASS( PSphereRenderer         );
LINK_CLASS( PBillboardRenderer      );
LINK_CLASS( PConeRenderer           );

LINK_CLASS( PTrigger                );
LINK_CLASS( POnDeath                );
LINK_CLASS( POnBirth                );
LINK_CLASS( POnTimer                );
LINK_CLASS( POnHitGround            );
LINK_CLASS( POnHitWater             );

LINK_CLASS( PEffectManager          );
LINK_CLASS( PEffect                 );
}

_inline DWORD _modalp(DWORD Color,float Alpha){
    DWORD C=Color>>24;
    C=C*Alpha;
    if(C>255)C=255;
    return (Color&0xFFFFFF)+(C<<24);
}
/*****************************************************************************/
/*    PEmitter implementation
/*****************************************************************************/
PEmitter::PEmitter()
{
    m_StartTime         = 0.0f;    
    m_TotalTime         = 10.0f;    
    m_FadeoutTime       = 0.0f;
    m_MaxParticles      = 1; 

    SetLooped           ( false );    
    SetAutoUpdated      ( false );
    SetPlayedForever    ( false );
    SetPosDependent     ( false );

    m_TimeToLive        = 10.0f;    
    m_TimeToLiveV       = 0.0f;
    m_bWorldSpace       = true;
    m_pParentEmitter    = NULL;
    m_WarmupTime        = 0.0f;

    m_bSelected         = false;    
    //  FIXME: this was needed to prevent deleting emitter which is being used by manager
    AddRef();
} // PEmitter::PEmitter

float PEmitter::GetTimeToLive( const PParticle& p )
{
    return rndValuef( m_TimeToLive - m_TimeToLiveV, m_TimeToLive + m_TimeToLiveV );
}

void PEmitter::Expose( PropertyMap& pm )
{
    pm.start( "PEmitter", this );
    pm.p( "Type", &PEmitter::ClassName                           );
    pm.p( "Name", &PEmitter::GetName, &PEmitter::SetName                    );
    pm.p( "Invisible", &PEmitter::IsInvisible, &PEmitter::SetInvisible           );

    pm.f( "StartTime",                m_StartTime                         );    
    pm.f( "TotalTime",                m_TotalTime                         );  
    pm.f( "WarmupTime",               m_WarmupTime                          );
    pm.p( "RandomWarmup", &PEmitter::IsRandomWarmup, &PEmitter::SetRandomWarmup);
    pm.p( "Looped", &PEmitter::IsLooped, &PEmitter::SetLooped                 );
    pm.p( "AutoUpdate", &PEmitter::IsAutoUpdated, &PEmitter::SetAutoUpdated       );
    pm.p( "PlayForever", &PEmitter::IsPlayedForever, &PEmitter::SetPlayedForever   );
    pm.f( "WorldSpace",               m_bWorldSpace                       );

    pm.f( "TimeToLive",               m_TimeToLive                        );    
    pm.f( "+/- TimeToLive",           m_TimeToLiveV                       );    
    pm.f( "Selected",                 m_bSelected                         );

    //pm.p( "BindToEmitter",            IsBindToEmitter, SetBindToEmitter    );    
    //pm.p( "Position Dependent",     IsPosDependent, SetPosDependent        );    
} // PEmitter::Expose
int PEmitter::GetShaderForEditor()
{
    if(IEffMgr->GetSelectionState()){
        bool sel=false;
        SNode* N=this;
        do{
            PEmitter* pem=dynamic_cast<PEmitter*>(N);
            if(pem && pem->GetSelState())
            {
                sel=true;
                break;
            }
            N=N->GetParent();
        }while(N);
        if(sel)
        {
            static int sh=IRS->GetShaderID("ps_selected");
            return sh;
        }else return -1;
    }else return -1;
}
void PEmitter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    float warmupHack = m_WarmupTime + 1.0f;
    os << m_StartTime << m_TotalTime << 
        m_Flags << warmupHack << m_TimeToLive << m_TimeToLiveV << 
        m_bWorldSpace;
} // PEmitter::Serialize

void PEmitter::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_StartTime >> m_TotalTime >> m_Flags >> 
        m_WarmupTime >> m_TimeToLive >> m_TimeToLiveV >> m_bWorldSpace;
    m_WarmupTime -= 1.0f; // hack because warmup is used instead of removed m_Probability
} // PEmitter::Unserialize

void PEmitter::Process( PEmitterInstance* pEmitter )
{
    if (!pEmitter || pEmitter->m_Cycles > 0) return;
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->GetFlag( pfJustBorn ))
        {
            PushEntityContext( GetID() );
            PushEntityContext( p->m_ID );
            g_EMgr.SpawnEmitter( this, p->m_ID, pEmitter->m_ID, true );
            PopEntityContext();
            PopEntityContext();
        }
        p = p->m_pNext;
    }
} // PEmitter::Process

/*****************************************************************************/
/*    PConstEmitter implementation
/*****************************************************************************/
PConstEmitter::PConstEmitter()
{
    m_Rate = 2;
}

void PConstEmitter::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PConstEmitter", this );
    pm.f( "Rate", m_Rate );
}

void PConstEmitter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Rate;
}

void PConstEmitter::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Rate;
}

int PConstEmitter::NumToEmit( PEmitterInstance* pInstance )
{
    if (pInstance->m_CurTime < m_StartTime || 
        pInstance->m_CurTime > m_TotalTime || 
        m_Rate < c_Epsilon) return 0;

    if (pInstance->m_pEmitter->IsPosDependent())
    {
        Vector3D pos        = pInstance->m_WorldTM.getTranslation();
        Vector3D prevPos    = pInstance->m_PrevWorldTM.getTranslation();

        float ds    = pos.distance2( prevPos )*0.01f;
        float rate  = m_Rate*pInstance->m_Intensity;
        float res   = ds*rate + pInstance->m_EmitAccum;
        pInstance->m_EmitAccum += floorf( res ) / rate;
        return res;
    }

    float dt    = pInstance->m_CurTime - m_StartTime - pInstance->m_EmitAccum;
    float rate  = m_Rate*pInstance->m_Intensity;
    float res   = dt*rate;
    pInstance->m_EmitAccum += floorf( res ) / rate;
    return res;
} // PConstEmitter::NumToEmit

/*****************************************************************************/
/*    PStaticEmitter implementation
/*****************************************************************************/
PStaticEmitter::PStaticEmitter()
{
    m_bWorldSpace = false;
}

void PStaticEmitter::Expose( PropertyMap& pm )
{
    pm.start( "PStaticEmitter", this );
    pm.p( "Name", &PStaticEmitter::GetName, &PStaticEmitter::SetName                       );
    pm.p( "Invisible", &PStaticEmitter::IsInvisible, &PStaticEmitter::SetInvisible              );
    pm.f( "StartTime",        m_StartTime                            );    
    pm.p( "TotalTime", &PStaticEmitter::GetStaticTotalTime, &PStaticEmitter::SetStaticTotalTime );
    pm.f( "WarmupTime",       m_WarmupTime                           );
    pm.p( "RandomWarmup", &PStaticEmitter::IsRandomWarmup, &PStaticEmitter::SetRandomWarmup        );
    pm.p( "Looped", &PStaticEmitter::IsLooped, &PStaticEmitter::SetLooped                    );
    pm.p( "AutoUpdate", &PStaticEmitter::IsAutoUpdated, &PStaticEmitter::SetAutoUpdated          );
    pm.p( "PlayForever", &PStaticEmitter::IsPlayedForever, &PStaticEmitter::SetPlayedForever      );
    pm.f( "WorldSpace",       m_bWorldSpace                          );
}

void PStaticEmitter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
}

void PStaticEmitter::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
}

int PStaticEmitter::NumToEmit( PEmitterInstance* pInstance )
{
    if (pInstance->m_CurTime < m_StartTime) return 0;
    int res = pInstance->m_EmitAccum > 0 ? 0 : 1;//m_StartTime was there - bug!
    if (res > 0) pInstance->m_EmitAccum += 1.0f;
    return res;
}

/*****************************************************************************/
/*    PBurstEmitter implementation
/*****************************************************************************/
PBurstEmitter::PBurstEmitter()
{
    m_PNumber = 1;
}

void PBurstEmitter::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PBurstEmitter", this );
    pm.f( "Number", m_PNumber );
}

void PBurstEmitter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_PNumber;
}

void PBurstEmitter::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_PNumber;
}

int PBurstEmitter::NumToEmit(  PEmitterInstance* pInstance )
{
    if (pInstance->m_CurTime < m_StartTime) return 0;
    if (pInstance->m_EmitAccum <= 0.0f)
    {
        int res = m_PNumber*pInstance->m_Intensity;
        pInstance->m_EmitAccum = res;
        return res;
    }
    return 0;
} // PBurstEmitter::NumToEmit

/*****************************************************************************/
/*    PRampEmitter implementation
/*****************************************************************************/
PRampEmitter::PRampEmitter()
{
    m_NRepeats = 1;
    m_MinRate  = 2;
    m_MaxRate  = 10;
} // PRampEmitter::PRampEmitter

void PRampEmitter::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PRampEmitter", this );
    pm.f( "MinRate", m_MinRate );
    pm.f( "MaxRate", m_MaxRate );
    pm.f( "Repeat", m_NRepeats );
    pm.p( "NumKeys", &PRampEmitter::GetNKeys );
    pm.p( "Ramp", &PRampEmitter::GetRamp, &PRampEmitter::SetRamp );
} // PRampEmitter::Expose

void PRampEmitter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    m_Ramp.Serialize( os );
    os << m_NRepeats << m_MinRate << m_MaxRate;
} // PRampEmitter::Serialize

void PRampEmitter::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    m_Ramp.Unserialize( is );
    is >> m_NRepeats >> m_MinRate >> m_MaxRate;
} // PRampEmitter::Unserialize

int PRampEmitter::NumToEmit( PEmitterInstance* pInstance )
{
    float t = m_NRepeats * pInstance->m_CurTime/pInstance->m_TotalTime;
    t -= floorf( t );
    float rate  = m_MinRate + m_Ramp.GetAlpha( t )*(m_MaxRate - m_MinRate);   
    float dt    = pInstance->GetTimeDelta();
    float res   = dt*rate + pInstance->m_EmitAccum;
    int nP = res;
    pInstance->m_EmitAccum += res - float( nP );
    pInstance->m_EmitAccum -= float( nP );
    return nP;
} // PRampEmitter::NumToEmit

/*****************************************************************************/
/*    PModularEmitter implementation
/*****************************************************************************/
PModularEmitter::PModularEmitter()
{
    m_RndSeed       = 15731;
    m_bWorldSpace   = false;
}

void PModularEmitter::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PModularEmitter", this );
}

/*****************************************************************************/
/*    PInitializer implementation
/*****************************************************************************/
void PInitializer::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age == 0.0f) InitParticle( *p );
        p = p->m_pNext;
    }
} // PInitializer::Process

/*****************************************************************************/
/*    PPlacer implementation
/*****************************************************************************/
void PPlacer::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age == 0.0f) 
        {
            Vector3D pos = GetPosition( *p );			
            pos += m_Position;
			pEmitter->m_WorldTM.transformVec( pos );
            //if (!pEmitter->m_pEmitter->IsWorldSpace()) pEmitter->m_WorldTM.transformPt( pos );
            p->m_Position += pos;
        }
        p = p->m_pNext;
    }
} // PPlacer::Process

void PPlacer::Render()
{
    if (!DoDrawGizmo()) return;
    DrawPoint( m_Position, GetColor(), 5.0f );
    DrawText( m_Position, GetColor(), "%s", GetName() );
    FlushText();
    rsFlushLines3D();
} // PPlacer::Render

void PPlacer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PPlacer", this );
    pm.f( "PosX", m_Position.x );
    pm.f( "PosY", m_Position.y );
    pm.f( "PosZ", m_Position.z );
} // PPlacer::Expose

void PPlacer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Position;
}

void PPlacer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Position;
}

/*****************************************************************************/
/*    PTargetPlacer implementation
/*****************************************************************************/
void PTargetPlacer::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age == 0.0f) 
        {
            Vector3D pos = GetPosition( *p );
            pos += pEmitter->m_Target - pEmitter->m_WorldTM.getTranslation();
            p->m_Position = pos;
        }
        p = p->m_pNext;
    }
} // PTargetPlacer::Process

/*****************************************************************************/
/*    PModelPlacer implementation
/*****************************************************************************/
PModelPlacer::PModelPlacer()
{
    m_ModelName = "model.c2m";
    m_Scale     = 1.0f;
    m_ModelID   = 0xFFFFFFFF;
}

void PModelPlacer::SetModelFile( const char* file )
{
    char path[_MAX_PATH];
    strcpy( path, file );
    //ToRelativePath( path, _MAX_PATH );
    m_ModelName = path;
    m_ModelID = IMM->GetModelID( file );
} // PModelPlacer::SetModelFile

void PModelPlacer::Render()
{
    Parent::Render();
    if (DoDrawGizmo())
    {
        SNode* pNode = NodePool::GetNode( m_ModelID );
        if (!pNode) return;

        Iterator it( pNode, Geometry::FnFilter );
        Geometry* pGeom = (Geometry*)*it;
        if (!pGeom) return;
        BaseMesh& bm = pGeom->GetMesh();
        Matrix4D tm = GetWorldTM( pGeom );
        IRS->SetWorldTM( tm );
        static int wSh = IRS->GetShaderID( "wireShaded" );
        IRS->SetShader( wSh );
        IRS->SetShaderAutoVars();
        DrawPrimBM( bm );
    }
} // PModelPlacer::Render

void PModelPlacer::Process( PEmitterInstance* pEmitter )
{
    SNode* pNode = NodePool::GetNode( m_ModelID );
    if (!pNode) return;
    
    Iterator it( pNode, Geometry::FnFilter );
    Geometry* pGeom = (Geometry*)*it;
    if (!pGeom) return;
    Primitive& bm = pGeom->GetMesh();
    Matrix4D tm = GetWorldTM( pGeom );

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age == 0.0f) 
        {
            Vector3D pos = GetRandomPoint( bm );
            pos *= tm;
            pos *= m_Scale;
            p->m_Position += pos;
            p->m_Position += m_Position;
        }
        p = p->m_pNext;
    }
} // PModelPlacer::Process

void PModelPlacer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PModelPlacer", this );
    pm.p( "ModelFile", &PModelPlacer::GetModelFile, &PModelPlacer::SetModelFile, "#model" );
}

void PModelPlacer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_ModelName << m_Scale;
} // PModelPlacer::Serialize

void PModelPlacer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_ModelName >> m_Scale;
    SetModelFile( m_ModelName.c_str() );
} // PModelPlacer::Unserialize

/*****************************************************************************/
/*    PLinePlacer implementation
/*****************************************************************************/
PLinePlacer::PLinePlacer()
{
    m_End.set( 0.0f, 0.0f, 100.0f );
}

void PLinePlacer::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age == 0.0f) 
        {
            Vector3D dir( m_End );
            dir -= m_Position;
            float len = dir.normalize();
            dir *= rndValuef( 0.0f, len );
            dir += m_Position;
            p->m_Position += dir;
        }
        p = p->m_pNext;
    }
} // PLinePlacer::Process

void PLinePlacer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PLinePlacer", this );
    pm.f( "EndX", m_End.x );
    pm.f( "EndY", m_End.y );
    pm.f( "EndZ", m_End.z );
}

void PLinePlacer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_End;
}

void PLinePlacer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_End;
}

void PLinePlacer::Render()
{
    Parent::Render();
    if (DoDrawGizmo())
    {
        rsLine( m_Position, m_End, 0xFF0000AA, 0xFF0000AA );
    }
} // PLinePlacer::Render

/*****************************************************************************/
/*    PSpherePlacer implementation
/*****************************************************************************/
PSpherePlacer::PSpherePlacer()
{
    m_Radius    = 100.0f;
    m_bSurface    = false;
    m_bPlanar    = false;
}

Vector3D PSpherePlacer::GetPosition( const PParticle& p )
{
    if (m_bPlanar)
    {
        if (m_bSurface)
        {
            float ang = rndValuef( 0.0f, c_DoublePI );
            Vector3D res( m_Radius * cosf( ang ), m_Radius * sinf( ang ), 0.0f );
            return res;
        }
        else
        {
            float r = m_Radius*sqrtf( rndValuef() );
            float phi = rndValuef( 0, c_DoublePI );
            return Vector3D( r*cosf( phi ), r*sinf( phi ), 0.0f );
        }
    }
    else
    {
        if (m_bSurface)
        {
            float z = rndValuef( -m_Radius, m_Radius );
            float phi = rndValuef( 0, c_DoublePI );
            float cosTheta = sqrtf( 1.0f - z*z/m_Radius/m_Radius );

            Vector3D res( m_Radius * cosTheta * cosf( phi ), 
                m_Radius * cosTheta * sinf( phi ), z );
            return res;
        }
        else
        {
            float x, y, z;
            while (true)
            {
                x = rndValuef( -1.0f, 1.0f );
                y = rndValuef( -1.0f, 1.0f );
                z = rndValuef( -1.0f, 1.0f );
                if (x*x + y*y + z*z <= 1.0f) break;
            }
            return Vector3D( m_Radius*x, m_Radius*y, m_Radius*z );
        }
    }
} // PSpherePlacer::GetPosition

void PSpherePlacer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PSpherePlacer", this );
    pm.f( "Radius",        m_Radius );
    pm.f( "OnSurface",    m_bSurface );
    pm.f( "Planar",        m_bPlanar );
}

void PSpherePlacer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Radius << m_bSurface << m_bPlanar;
}

void PSpherePlacer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Radius >> m_bSurface >> m_bPlanar;
}

void PSpherePlacer::Render()
{
    Parent::Render();
    if (DoDrawGizmo())
    {
        if (m_bPlanar)
        {
            if (m_bSurface)
            {
                DrawCircle( m_Position, Vector3D::oZ, m_Radius , 0, 0xFF0000AA, 16 );
            }
            else
            {
                DrawCircle( m_Position, Vector3D::oZ, m_Radius , 0x660000AA, 0xFF0000AA, 16 );
            }
        }
        else
        {
            if (m_bSurface)
            {
                DrawSphere( Sphere( m_Position, m_Radius ), 0, 0xFF0000AA, 16 );
            }
            else
            {
                DrawSphere( Sphere( m_Position, m_Radius ), 0x660000AA, 0xFF0000AA, 16 );
            }
        }
    }
} // PSpherePlacer::Render

/*****************************************************************************/
/*    PBoxPlacer implementation
/*****************************************************************************/
PBoxPlacer::PBoxPlacer()
{
    m_DX = 100.0f;
    m_DY = 100.0f;
    m_DZ = 100.0f;
    m_bPlanar = false;
}

Vector3D PBoxPlacer::GetPosition( const PParticle& p )
{
    Vector3D pos( Vector3D::null );
    pos.x = rndValuef( 0.0f, m_DX ) - m_DX * 0.5f;
    pos.y = rndValuef( 0.0f, m_DY ) - m_DY * 0.5f;
    if (m_bPlanar) pos.z = m_DZ*0.5f; 
    else pos.z = rndValuef( 0.0f, m_DZ ) - m_DZ * 0.5f;
    return pos;
} // PBoxPlacer::GetPosition

void PBoxPlacer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PBoxPlacer", this );
    pm.f( "DX", m_DX );
    pm.f( "DY", m_DY );
    pm.f( "DZ", m_DZ );
    pm.f( "Planar", m_bPlanar );
} // PBoxPlacer::Expose

void PBoxPlacer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_DX << m_DY << m_DZ << m_bPlanar;
}

void PBoxPlacer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_DX >> m_DY >> m_DZ >> m_bPlanar;
}

void PBoxPlacer::Render()
{
    Parent::Render();
    if (DoDrawGizmo())
    {
        if (m_bPlanar)
        {
        
        }
        else
        {
            DrawAABB( AABoundBox( m_Position, m_DX*0.5f, m_DY*0.5f, m_DZ*0.5f ), 0x660000AA, 0xFF0000AA );
        }
    }
} // PBoxPlacer::Render

/*****************************************************************************/
/*    PCylinderPlacer implementation
/*****************************************************************************/
PCylinderPlacer::PCylinderPlacer()
{
    m_Radius            = 20.0f;
    m_Height            = 100.0f;
    m_bOnSurface        = false;
    m_bTopCap           = true;
    m_bBottomCap        = true;
    m_bSphericalCaps    = false;
} // PCylinderPlacer::PCylinderPlacer

Vector3D PCylinderPlacer::GetPosition( const PParticle& p )
{
    float z = rndValuef( 0.0f, m_Height );
    float x, y;
    
    if (m_bOnSurface)
    {
        float ang = rndValuef( 0.0f, c_DoublePI );
        x = m_Radius * cosf( ang );
        y = m_Radius * sinf( ang );

    }
    else
    {
        float r = m_Radius*sqrtf( rndValuef() );
        float phi = rndValuef( 0, c_DoublePI );
        x = r*cosf( phi );
        y = r*sinf( phi );
    }
    return Vector3D( x, y, z );
} // PCylinderPlacer::GetPosition

void PCylinderPlacer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PCylinderPlacer", this );
    pm.f( "Radius",           m_Radius        );
    pm.f( "Height",           m_Height        );
    pm.f( "OnSurface",        m_bOnSurface    );
} // PCylinderPlacer::Expose

void PCylinderPlacer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Radius << m_Height << 
        m_bTopCap << m_bBottomCap << m_bSphericalCaps << m_bOnSurface;
}

void PCylinderPlacer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Radius >> m_Height >>
        m_bTopCap >> m_bBottomCap >> m_bSphericalCaps >> m_bOnSurface;
}

void PCylinderPlacer::Render()
{
    Parent::Render();
    if (DoDrawGizmo())
    {
        Vector3D end( m_Position ); end.z += m_Height;
        Cylinder cyl( Line3D( m_Position, end ), m_Radius );
        if (m_bOnSurface)
        {
            DrawCylinder( cyl, 0, 0xFF0000AA, false, 16 );
        }
        else
        {
            DrawCylinder( cyl, 0x660000AA, 0xFF0000AA, true, 16 );
        }
    }
} // PCylinderPlacer::Render

/*****************************************************************************/
/*    PCirclePlacer implementation
/*****************************************************************************/
PCirclePlacer::PCirclePlacer()
{
    m_Radius            = 100.0f;
    m_Velocity          = 5.0f;
} // PCirclePlacer::PCirclePlacer

void PCirclePlacer::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    float alpha = (pEmitter->m_CurTime - pEmitter->m_StartTime)*m_Velocity;
    while (p)
    {
        if (p->m_Age == 0.0f) 
        {
            p->m_Position.x += m_Radius*cosf( alpha );
            p->m_Position.y += m_Radius*sinf( alpha );
        }
        p = p->m_pNext;
    }
} // PCirclePlacer::Process

void PCirclePlacer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PCirclePlacer", this );
    pm.f( "Radius",          m_Radius        );
    pm.f( "Velocity",        m_Velocity      );
} // PCirclePlacer::Expose

void PCirclePlacer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Radius << m_Velocity;
}

void PCirclePlacer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Radius >> m_Velocity;
}

/*****************************************************************************/
/*    PShooter implementation
/*****************************************************************************/
PShooter::PShooter()
{
    m_Velocity          = 100.0f;
    m_VelocityD         = 0.0f;
    m_AngVelocity       = Vector3D::null;
    m_AngVelocityD      = Vector3D::null;
    m_bAffectRotation   = false;
}

void PShooter::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PShooter", this );
    pm.f( "Velocity",            m_Velocity );
    pm.f( "+/- Velocity",        m_VelocityD );
    pm.f( "AngVelocity",        m_AngVelocity.x );
    pm.f( "+/- AngVelocity",    m_AngVelocityD.x );
    pm.f( "AffectRotation",        m_bAffectRotation );
} // PShooter::Expose

void PShooter::Process( PEmitterInstance* pEmitter )
{
    float s = pEmitter->m_pEmitter->IsWorldSpace() ? pEmitter->m_WorldTM.getV0().norm() : 1.0f;    
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age == 0.0f) 
        {
            float mag = rndValuef( m_Velocity - m_VelocityD, m_Velocity + m_VelocityD );
            Vector3D vel = GetDirection( *p );
			pEmitter->m_WorldTM.transformVec( vel );
            //if (pEmitter->m_pEmitter->IsWorldSpace()) pEmitter->m_WorldTM.transformVec( vel );
            vel.normalize();
            vel *= mag*s;
            p->m_Velocity += vel;
            
            p->m_AngVelocity.x = rndValuef(    m_AngVelocity.x - m_AngVelocityD.x, m_AngVelocity.x + m_AngVelocityD.x );
            p->m_AngVelocity.y = rndValuef(    m_AngVelocity.y - m_AngVelocityD.y, m_AngVelocity.y + m_AngVelocityD.y );
            p->m_AngVelocity.z = rndValuef(    m_AngVelocity.z - m_AngVelocityD.z, m_AngVelocity.z + m_AngVelocityD.z );

            if (m_bAffectRotation)
            {
                Vector3D vec( p->m_Velocity );
                g_EMgr.m_ScreenPlane.ProjectVec( vec );
                p->m_Roll = vec.Angle( g_EMgr.m_PlaneUpVec, g_EMgr.m_PlaneNVec );
            }
        }
        p = p->m_pNext;
    }
} // PShooter::Process

void PShooter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Velocity << m_VelocityD << m_AngVelocity << m_AngVelocityD << m_bAffectRotation;
} // PShooter::Serialize

void PShooter::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Velocity >> m_VelocityD >> m_AngVelocity >> m_AngVelocityD >> m_bAffectRotation;
} // PShooter::Unserialize

/*****************************************************************************/
/*    PConeShooter implementation
/*****************************************************************************/
PConeShooter::PConeShooter()
{
    m_ConeAngle = 10;
    m_bPlanar    = false;
    m_bSurface  = false;    
}

void PConeShooter::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PConeShooter", this );
    pm.f( "ConeAngle",  m_ConeAngle );
    pm.f( "Planar",     m_bPlanar );
    pm.f( "OnSurface",  m_bSurface );
} // PConeShooter::Expose

Vector3D PConeShooter::GetDirection( const PParticle& p )
{
    clamp( m_ConeAngle, 0.0f, 180.0f );
    if (m_bPlanar)
    {
        if (m_bSurface)
        {
            float ang = DegToRad( m_ConeAngle );
            int v = rndValue( 0, 1 );
            if (v == 1) return Vector3D( sinf( -ang ), 0.0f, cosf( -ang ) );
            else return Vector3D( sinf( ang ), 0.0f, cosf( ang ) );
        }
        else
        {
            float ang = DegToRad( rndValuef( -m_ConeAngle, m_ConeAngle ) );
            return Vector3D( sinf( ang ), 0.0f, cosf( ang ) );
        }
    }
    else 
    {
        if (m_bSurface)
        {
            float ang = rndValuef( 0.0f, c_DoublePI );
            float cAng = cosf( DegToRad( m_ConeAngle ) );
            float sAng = sinf( DegToRad( m_ConeAngle ) );
            Vector3D dir( sAng*cosf( ang ), sAng*sinf( ang ), cAng ); 
        }
        else
        {
            float r1 = rndValuef();
            float r2 = rndValuef();

            float m1 = 1.0f - r2 * (1.0f - cosf( DegToRad( m_ConeAngle ) )); 
            float m2 = sqrtf( 1.0f - m1*m1 );
            return Vector3D( m2*cosf( c_DoublePI * r1 ), m2*sinf( c_DoublePI * r1 ), m1 );
        }
    }
    return Vector3D::oZ;
} // PConeShooter::GetDirection

void PConeShooter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    BYTE flags = 0;
    if (m_bSurface) flags |= fSurface;
    if (m_bPlanar) flags  |= fPlanar;
    os << m_ConeAngle << flags;
} // PConeShooter::Serialize

void PConeShooter::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    BYTE flags = 0;
    is >> m_ConeAngle >> flags;
    m_bSurface = ((flags&fSurface) != 0);
    m_bPlanar  = ((flags&fPlanar)  != 0);
} // PConeShooter::Unserialize

void PConeShooter::Render()
{
    clamp( m_ConeAngle, 0.0f, 180.0f );
    if (DoDrawGizmo())
    {
        float ang = DegToRad( m_ConeAngle );
        Vector3D top ( Vector3D::null );
        Vector3D base( Vector3D::oZ );
        base *= 100.0f*cosf( ang );
        const Matrix4D& tm = TransformNode::TMStackTop();
        tm.transformPt( top );
        tm.transformPt( base );
        Cone cone( top, base, ang );
        if (m_bPlanar)
        {
            const Matrix4D c_FlatTM = Matrix4D( 1, 0, 0, 0, 
                                                0, 0, 0, 0, 
                                                0, 0, 1, 0, 
                                                0, 0, 0, 1 );
            IRS->SetWorldTM( c_FlatTM ); 
            DrawCone( cone, 0x33FF0000, 0x77FF0000, 2 );
        }
        else
        {
            DrawCone( cone, 0x33FF0000, 0x77FF0000, 16 );
        }
        rsFlushLines3D();
        rsFlushPoly3D();
    }
} // PConeShooter::Render

/*****************************************************************************/
/*    PRadialShooter implementation
/*****************************************************************************/
PRadialShooter::PRadialShooter()
{
    m_bBindToOrigin = false;
    m_bPlanar        = false;
}

void PRadialShooter::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PRadialShooter", this );
    pm.f( "BindToCentroid", m_bBindToOrigin );
    pm.f( "Planar",            m_bPlanar );
} // PRadialShooter::Expose

Vector3D PRadialShooter::GetDirection( const PParticle& p )
{
    if (m_bBindToOrigin)
    {
        Vector3D dir = p.m_Position;
        if (dir.normalize() > c_SmallEpsilon) return dir;
    }

    if (!m_bPlanar)
    {
        float r1 = rndValuef();
        float r2 = rndValuef();
        float m  = sqrtf( r2 * (1.0f - r2) );
        return Vector3D(2.0f * cosf( c_DoublePI * r1 )*m, 
                        2.0f * sinf( c_DoublePI * r1 )*m, 
                        1.0f - 2.0f*r2 );
    }
    else
    {
        float phi = rndValuef( 0, c_DoublePI );
        return Vector3D( cosf( phi ), sinf( phi ), 0.0f );
    }
    return Vector3D::null;
} // PRadialShooter::GetDirection

void PRadialShooter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_bBindToOrigin << m_bPlanar;
}

void PRadialShooter::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_bBindToOrigin >> m_bPlanar;
}

/*****************************************************************************/
/*    PRampShooter implementation
/*****************************************************************************/
PRampShooter::PRampShooter()
{
    m_Repeat  = 1;
    m_Variation = 0.0f;
}

void PRampShooter::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PRampShooter", this );
    pm.f( "Variation", m_Variation );
    pm.f( "Repeats", m_Repeat );
    pm.p( "NumKeys", &PRampShooter::GetNKeys );
    pm.p( "Ramp", &PRampShooter::GetRamp, &PRampShooter::SetRamp );
}

void PRampShooter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    m_Ramp.Serialize( os );
    os << m_Repeat << m_Variation;
}

void PRampShooter::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    m_Ramp.Unserialize( is );
    is >> m_Repeat >> m_Variation;
}

void PRampShooter::Process( PEmitterInstance* pEmitter )
{
    float t = m_Repeat * (pEmitter->m_CurTime - pEmitter->m_StartTime)/pEmitter->m_TotalTime;    
    t -= floorf( t );

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age == 0.0f)
        {
            Vector3D vel = p->m_Velocity;
            if (vel.normalize() <= c_SmallEpsilon) vel = Vector3D::GetRandomDir();
            float mag = m_Ramp.GetAlpha( t ) + rndValuef( -m_Variation, m_Variation );
            vel *= mag;
            p->m_Velocity = vel;
        }
        p = p->m_pNext;
    }
} // PRampShooter::Process

/*****************************************************************************/
/*    PDirectShooter implementation
/*****************************************************************************/
PDirectShooter::PDirectShooter()
{
    m_Direction = Vector3D::oZ;
}

void PDirectShooter::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PDirectShooter", this );
    pm.f( "DirX", m_Direction.x );
    pm.f( "DirY", m_Direction.y );
    pm.f( "DirZ", m_Direction.z );
}

void PDirectShooter::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Direction;
}

void PDirectShooter::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Direction;
}

Vector3D PDirectShooter::GetDirection( const PParticle& p )
{
    return m_Direction;
}

void PDirectShooter::Render()
{
    Parent::Render();
    if (DoDrawGizmo())
    {
        DrawArrow( Vector3D::null, m_Direction, 0xFF22AA22, 100.0f );
    }
} // PDirectShooter::Render

/*****************************************************************************/
/*    POperator implementation
/*****************************************************************************/
POperator::POperator()
{
    m_bVisual = false;
}

void POperator::Expose( PropertyMap& pm )
{
    pm.start( "POperator", this );
    pm.p( "Invisible", &POperator::POperator::IsInvisible, &POperator::SetInvisible );
    pm.p( "DrawGizmo", &POperator::DoDrawGizmo, &POperator::SetDrawGizmo );
} // POperator::Expose

/*****************************************************************************/
/*    PTrigger implementation
/*****************************************************************************/
void PTrigger::Trigger( const PEmitterInstance* pEmitter, PParticle& p )
{
    if (p.m_Age < m_Timeout) return;
    switch (m_TriggerType)
    {
    case etCreateEffect: 
        {
            PushEntityContext( p.m_ID );
            g_EMgr.SpawnEmitter( (PEmitter*)GetChild( 0 ), p.m_ID, pEmitter->m_ID );
            Matrix4D tm; tm.translation( p.m_Position );
            IEffMgr->UpdateInstance( tm );            
            PopEntityContext();
        }
        break;    
    case etDeath: 
        {
            p.m_Age = p.m_TimeToLive; 
            p.SetFlag( pfDead );
        }
        break;
    case etReflect: 
        {
            Deflect( p );
        }
        break;        
    case etResetAge: 
        {
            p.m_Age = 0.0f;
        }
        break;        
    case etStop: 
        {
            p.m_Velocity.zero();
        }
        break;            
    case etStopRotation: 
        {
            p.m_AngVelocity.zero();
        }
        break;    
    }
} // PTrigger::Trigger

void PTrigger::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PTrigger", this );
    pm.f( "Action", m_TriggerType );
    pm.f( "Damping", m_Damping );
    pm.f( "Timeout", m_Timeout );
} // PTrigger::Expose

void PTrigger::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << Enum2Byte( m_TriggerType ) << m_Damping << m_Timeout;
} // PTrigger::Serialize

void PTrigger::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> Enum2Byte( m_TriggerType ) >> m_Damping >> m_Timeout;
} // PTrigger::Unserialize

/*****************************************************************************/
/*    POnDeath implementation
/*****************************************************************************/
void POnDeath::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->GetFlag( pfDead ))
        {
            if (m_TriggerType == etCreateEffect)
            {
                if (GetNChildren() == 0) return;
                PushEntityContext( p->m_ID );
                int hInst = g_EMgr.SpawnEmitter( (PEmitter*)GetChild( 0 ), -1, pEmitter->m_ID, false );
                if (InheritPosition())
                {
                    Matrix4D tm = PEffectManager::GetParticleWorldTM(*p);
                    //Matrix4D tm;
                    //tm.translation( p->GetTransform().getTranslation() );
                    if(pEmitter->GetFlag(ifUpdated))IEffMgr->UpdateInstance( tm );
                }
                PopEntityContext();
            }
            else Trigger( pEmitter, *p );
        }
        p = p->m_pNext;
    }
} // POnDeath::Process

void POnDeath::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_bInheritPosition;
}
void POnDeath::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_bInheritPosition;
}

void POnDeath::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "POnDeath", this );
    pm.f( "InheritPosition", m_bInheritPosition );
} // POnDeath::Expose

/*****************************************************************************/
/*    POnBirth implementation
/*****************************************************************************/
void POnBirth::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age == 0.0f)
        {
            Trigger( pEmitter, *p );
        }
        p = p->m_pNext;
    }
} // POnBirth::Process

/*****************************************************************************/
/*    POnHitGround implementation
/*****************************************************************************/
void POnHitGround::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age > 0.0f)
        {    
            float h = ITerra->GetH( p->m_Position.x, p->m_Position.y );
            if ((p->m_Position.z - h)*(p->m_PrevPosition.z - h) < 0.0f) 
            {
                Trigger( pEmitter, *p );
            }
        }
        p = p->m_pNext;
    }
} // POnHitGround::Process

void POnHitGround::Deflect( PParticle& p )
{
    Vector3D normV, planeV;
    Plane pl( p.m_Position, ITerra->GetNormal( p.m_Position.x, p.m_Position.y ) );
    pl.Decompose( p.m_Velocity, normV, planeV );
    normV.reverse();
    if (normV.dot( p.m_Velocity ) >= 0.0f) 
    {
        return;
    }
    p.m_Velocity.add( normV, planeV );
    Vector3D dampV( p.m_Velocity ); 
    if (dampV.normalize() > m_Damping)
    {
        dampV *= m_Damping;
        p.m_Velocity -= dampV;
    }
    else p.m_Velocity = Vector3D::null;
} // POnHitGround::Deflect

/*****************************************************************************/
/*    POnHitWater implementation
/*****************************************************************************/
void POnHitWater::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age > 0.0f)
        {
            float h = ITerra->GetH( p->m_Position.x, p->m_Position.y );
            if (p->m_Position.z*p->m_PrevPosition.z <= 0.0f && h < 0.0f)
            {
                Trigger( pEmitter, *p );
            }
        }
        p = p->m_pNext;
    }
} // POnHitWater::Process

void POnHitWater::Deflect( PParticle& p )
{
    p.m_Velocity.z = -p.m_Velocity.z;
    p.m_Velocity *= 1.0f - m_Damping;
    p.m_AngVelocity *= 1.0f - m_Damping;
}

/*****************************************************************************/
/*    POnTimer implementation
/*****************************************************************************/
POnTimer::POnTimer()
{
    m_Time = 5.0f;
}

void POnTimer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "POnTimer", this );
    pm.f( "Time", m_Time );
}

void POnTimer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Time;
}

void POnTimer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Time;
}

void POnTimer::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (p->m_Age < m_Time && p->m_Age + dt >= m_Time)
        {
            Trigger( pEmitter, *p );
        }
        p = p->m_pNext;
    }
} // POnTimer::Process

void POnTimer::Deflect( PParticle& p )
{
    p.m_Velocity.reverse();
}

/*****************************************************************************/
/*    PRenderer implementation
/*****************************************************************************/
PRenderer::PRenderer()
{
    m_TexID            = -1;
    m_TexName        = "particle.tga";
    m_TexID2            = -1;
    m_TexName2        = "grey2x2.dds";
    m_BlendMode        = bmAdd;
    m_Intensity        = bmNormal;
    m_Tint            = 0xFFFFFFFF;
    m_Flags            = rfDoZTest;
    m_ZBias            = zbStage0;
    m_bVisual       = true;

    m_RenderBin.create( c_ParticleRenderBinSize, 0, vfVertex2t );
    m_RenderBin.setIsQuadList( true );

    SetDitherEnable ( false );
    SetBlendMode    ( bmMul );
} // PRenderer::PRenderer

void PRenderer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PRenderer", this );
    pm.p( "Pixels", &PRenderer::GetTexID, &PRenderer::SetTexID,        "texture"        );
    pm.p( "Texture", &PRenderer::GetTexName, &PRenderer::SetTexName,        "#texture"      );
    pm.p( "Pixels2", &PRenderer::GetTexID2, &PRenderer::SetTexID2,        "texture"        );
    pm.p( "Texture2", &PRenderer::GetTexName2, &PRenderer::SetTexName2,    "#texture"      );
    pm.p( "BlendMode", &PRenderer::GetBlendMode, &PRenderer::SetBlendMode    );
    pm.p( "Intensity", &PRenderer::GetIntensity, &PRenderer::SetIntensity    );
    pm.p( "DoZTest", &PRenderer::IsZTest, &PRenderer::SetZTest        );
    pm.p( "DoZWrite", &PRenderer::IsZWrite, &PRenderer::SetZWrite        );
    pm.p( "Dither", &PRenderer::IsDitherEnable, &PRenderer::SetDitherEnable );
    pm.p( "FilterTexture", &PRenderer::IsFilterTexture, &PRenderer::SetFilterTexture );
    pm.p( "HeadMoveDir", &PRenderer::IsHeadMoveDir, &PRenderer::SetHeadMoveDir    );
    pm.p( "NullLayer", &PRenderer::IsNullLayer, &PRenderer::SetNullLayer    );
    pm.p( "ZBias", &PRenderer::GetZBias, &PRenderer::SetZBias        );
    pm.f( "Tint",           m_Tint, "color" );
} // PRenderer::Expose

void PRenderer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_TexName << m_TexName2 << Enum2Byte( m_BlendMode ) << 
            Enum2Byte( m_Intensity ) << m_Tint << m_Flags << m_ZBias;            
}

void PRenderer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_TexName >> m_TexName2 >> Enum2Byte( m_BlendMode ) >> 
        Enum2Byte( m_Intensity ) >> m_Tint >> m_Flags >> m_ZBias;    
    const char* s= m_TexName2.c_str();
    if( s==NULL || s[0]==0 ){
        m_TexName2 = "grey2x2.dds";
    }

    UpdateShader();
} // PRenderer::Unserialize

void PRenderer::UpdateShader()
{
    static int ps_Add  = IRS->GetShaderID( "ps_Add"   );
    static int ps_Add2 = IRS->GetShaderID( "ps_Add2x" );
    static int ps_Add4 = IRS->GetShaderID( "ps_Add4x" );

    static int ps_Mod  = IRS->GetShaderID( "ps_Mod"   );
    static int ps_Mod2 = IRS->GetShaderID( "ps_Mod2x" );
    static int ps_Mod4 = IRS->GetShaderID( "ps_Mod4x" );

    if (m_BlendMode == bmAdd) 
    {
        if (m_Intensity == bmNormal      ) { m_ShaderID = ps_Add; return; }
        if (m_Intensity == bmIntense     ) { m_ShaderID = ps_Add2; return; }
        if (m_Intensity == bmSuperIntense) { m_ShaderID = ps_Add4; return; }
    }
    else 
    {
        if (m_Intensity == bmNormal      ) { m_ShaderID = ps_Mod; return; }
        if (m_Intensity == bmIntense     ) { m_ShaderID = ps_Mod2; return; }
        if (m_Intensity == bmSuperIntense) { m_ShaderID = ps_Mod4; return; }
    }
} // PRenderer::UpdateShader

void PRenderer::SetTexName( const char* val )
{ 
    FilePath path( val );
    m_TexName = path.GetFileWithExt();
    m_TexID = IRS->GetTextureID( m_TexName.c_str() );
    UpdateShader();
} // PRenderer::SetTexName

void PRenderer::SetTexName2( const char* val )
{ 
    FilePath path( val );
    m_TexName2 = path.GetFileWithExt();
    m_TexID2 = IRS->GetTextureID( m_TexName2.c_str() );
    UpdateShader();
} // PRenderer::SetTexName2

void PRenderer::SetShader()
{
    IRS->SetShader( m_ShaderID );
    if (!IsFilterTexture()) IRS->SetTexFilterEnable( false );
} // PRenderer::SetShader

/*****************************************************************************/
/*    PModelRenderer implemetation
/*****************************************************************************/
PModelRenderer::PModelRenderer()
{
    m_ModelID   = 0xFFFFFFFF;
    m_AnimID   = 0xFFFFFFFF;

    m_ShaderID  = -1;
    m_TexID     = -1;
    m_TexID2    = -1;

    m_UseStopFrame = false;
    m_StopFramePercent = 100;
    m_FadeOnDeathTime = 2.0f;
}

void PModelRenderer::RenderGeometry( PEmitterInstance* pEmitter )
{
    if (!pEmitter->GetFlag( ifNeedDraw )) return;

    if (m_TexID == -1) SetTexName( GetTexName() );

    //DWORD tint = m_Tint & 0x00FFFFFF;
    //tint |= (((DWORD)(pEmitter->m_Alpha*255.0f))<<24);
    //IRS->SetTextureFactor( tint );

    if (IsZWrite()) IRS->SetZWriteEnable();
    if (!IsZTest()) IRS->SetZEnable( false );
    IRS->SetDitherEnable( IsDitherEnable() );

    Matrix4D wtm = TransformNode::TMStackTop();
    if (!pEmitter->m_pEmitter->IsWorldSpace())
    {
        Vector3D tr( pEmitter->m_WorldTM.getTranslation() );
        Matrix4D tm;
        tm.scaling( pEmitter->m_WorldTM.getV0().norm() );
        tm.translate( tr );
        wtm = tm;
    }
    IRM->ConvertPathToRelativeForm( m_ModelName );
    if (m_ModelID == 0xFFFFFFFF) m_ModelID = IMM->GetModelID( m_ModelName.c_str() );
    if (m_ModelID == 0xFFFFFFFF) return;
    PParticle* p = pEmitter->m_pParticle;

    ICamera* pCam = GetCamera();
    float s = pEmitter->m_pEmitter->IsWorldSpace() ? pEmitter->m_WorldTM.getV0().norm() : 1.0f;    
    if (!pCam) return;

    PushEntityContext( (DWORD)p );

    IRM->ConvertPathToRelativeForm( m_AnimName );
    if (m_AnimID == 0xFFFFFFFF) m_AnimID = IMM->GetModelID( m_AnimName.c_str() );
    float t = 0.0f;
    if( p && m_AnimID!=0xFFFFFFFF )
    {
        float at=IMM->GetAnimTime( m_AnimID );
        if(m_UseStopFrame){            
            at *= m_StopFramePercent/100;
            t = p->m_Age*1000.0f;
            if(t>at)t=at;
        }else{
            t = fmod( p->m_Age*1000.0f, at );
        }
    }
    
    if (m_ZBias > 0)
    {
        float shiftZ = -g_EMgr.GetZBiasMultiplier()*float( m_ZBias )*s;
        pCam->ShiftZ( -shiftZ );
        pCam->Render();    
        while (p)
        {
            if (p->m_Age > 0.0f)
            {     
                Matrix4D tm = PEffectManager::GetParticleFullWorldTM(*p);
                //Matrix4D tm( p->m_Size, p->m_Rotation, Vector3D::null );                
                //tm.mulLeft(wtm);
                IMM->StartModel( m_ModelID, tm );
                IMM->AnimateModel( m_AnimID, t );
                IMM->DrawModel();
            }
            p = p->m_pNext;
        }
        pCam->ShiftZ( shiftZ );
        pCam->Render();    
    }
    else
    {
        while (p)
        {
            if (p->m_Age > 0.0f)
            {
                Matrix4D tm = PEffectManager::GetParticleFullWorldTM(*p);// p->m_Size, p->m_Rotation, Vector3D::null );                
                //tm*=wtm;
                DWORD C = _modalp(p->m_Color,pEmitter->m_Alpha);
                float dAge = p->m_Age-p->m_AgeOfLastUpd;
                if (dAge>m_FadeOnDeathTime/2){
                    dAge=2*(1-dAge/m_FadeOnDeathTime);
                    if(dAge<0){
                        p = p->m_pNext;
                        continue;
                    }
                    DWORD A=(C>>24)*dAge;
                    C=(C&0xFFFFFF)+(A<<24);
                }
                IRS->SetTextureFactor(C);
                IMM->StartModel( m_ModelID, tm );
                IMM->AnimateModel( m_AnimID, t );
                IMM->DrawModel();
            }
            p = p->m_pNext;
        }
    }
    IRS->SetTextureFactor(0xFFFFFFFF);
    PopEntityContext();

} // PModelRenderer::Process

void PModelRenderer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PModelRenderer", this );
    pm.p( "ModelObject", &PModelRenderer::GetModelName, &PModelRenderer::SetModelName, "#model" );
    pm.p( "Animation", &PModelRenderer::GetAnimName, &PModelRenderer::SetAnimName, "#model" );
} // PModelRenderer::Expose

void PModelRenderer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    std::string s1=m_ModelName;
    IRM->ConvertPathToRelativeForm( s1 );
    std::string s2=m_AnimName;
    IRM->ConvertPathToRelativeForm( s2 );
    os << s1 << s2;
}

void PModelRenderer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_ModelName >> m_AnimName;
}
void PModelRendererWithStop::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PModelRendererWithStop", this );
    pm.p( "ModelObject", &PModelRendererWithStop::GetModelName, &PModelRendererWithStop::SetModelName, "#model" );
    pm.p( "Animation", &PModelRendererWithStop::GetAnimName, &PModelRendererWithStop::SetAnimName, "#model" );
    pm.f( "FadeOnDeathTime", m_FadeOnDeathTime );
    pm.f( "UseStopFrame", m_UseStopFrame );
    pm.f( "StopFramePercent", m_StopFramePercent );    
}
void PModelRendererWithStop::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    std::string s1=m_ModelName;
    IRM->ConvertPathToRelativeForm( s1 );
    std::string s2=m_AnimName;
    IRM->ConvertPathToRelativeForm( s2 );
    os << s1 << s2 << m_UseStopFrame << m_StopFramePercent << m_FadeOnDeathTime;
}
void PModelRendererWithStop::Unserialize ( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_ModelName >> m_AnimName >> m_UseStopFrame >> m_StopFramePercent >> m_FadeOnDeathTime;
}

/*****************************************************************************/
/*    PBillboardRenderer implemetation
/*****************************************************************************/
PBillboardRenderer::PBillboardRenderer()
{
    m_Alignment = baCamera;
    m_RefPoint = Vector3D( 0.0f, 0.0f, 0.0f );
}

template <class TVert>
bool AddQuad( Primitive& pri, const Vector3D& pos, 
                float sizeX, float sizeY,
                float refX, float refY,
                float rot, DWORD color, 
                const Rct& uv, const Rct& uv2, 
                const Matrix4D& camTM )
{
    int nVert = pri.getNVert();
    if (nVert == pri.getMaxVert()) return false;
    TVert* v = ((TVert*)pri.getVertexData() + nVert);

    float cr = 1.0f;
    float sr = 0.0f;
    if (fabs( rot ) > 0.0f)
    {
        cr = cosf( rot );
        sr = sinf( rot );
    }
    float xc = refX * sizeX;
    float yc = refY * sizeY;

    Matrix4D bTM(   cr*sizeX,       sr*sizeX,       0.0f, 0.0f,
                    -sr*sizeY,      cr*sizeY,       0.0f, 0.0f, 
                    0.0f,           0.0f,           1.0f, 0.0f,
                    -cr*xc + sr*yc, -sr*xc - cr*yc, 0.0f, 1.0f );

    bTM *= camTM;
    Vector4D lt( 0.0f, 0.0f, 0.0f, 1.0f );
    Vector4D rt( 1.0f, 0.0f, 0.0f, 1.0f );
    Vector4D lb( 0.0f, 1.0f, 0.0f, 1.0f );
    Vector4D rb( 1.0f, 1.0f, 0.0f, 1.0f );

    lt *= bTM;
    rt *= bTM;
    lb *= bTM;
    rb *= bTM;

    v[0].x = lt.x + pos.x;
    v[0].y = lt.y + pos.y;
    v[0].z = lt.z + pos.z;

    v[1].x = rt.x + pos.x;
    v[1].y = rt.y + pos.y;
    v[1].z = rt.z + pos.z;

    v[2].x = lb.x + pos.x;
    v[2].y = lb.y + pos.y;
    v[2].z = lb.z + pos.z;

    v[3].x = rb.x + pos.x;
    v[3].y = rb.y + pos.y;
    v[3].z = rb.z + pos.z;

    //  color and texture coordinates
    v[0].diffuse = color;
    v[0].u         = uv.x;
    v[0].v         = uv.y + uv.h;
    v[0].u2         = uv2.x;
    v[0].v2         = uv2.y + uv2.h;

    v[1].diffuse = color;
    v[1].u         = uv.x + uv.w;
    v[1].v         = uv.y + uv.h;
    v[1].u2         = uv2.x + uv2.w;
    v[1].v2         = uv2.y + uv2.h;

    v[2].diffuse = color;
    v[2].u         = uv.x;
    v[2].v         = uv.y;
    v[2].u2         = uv2.x;
    v[2].v2         = uv2.y;

    v[3].diffuse = color;
    v[3].u         = uv.x + uv.w;
    v[3].v         = uv.y;
    v[3].u2         = uv2.x + uv2.w;
    v[3].v2         = uv2.y;

    pri.setNPri ( nVert/2 + 2 );
    pri.setNVert( nVert + 4 );
    return true;
} // AddQuad

bool PBillboardRenderer::FillGeometry( PEmitterInstance* pEmitter ) 
{ 
    if (!pEmitter->GetFlag( ifNeedDraw )) return false;

    PParticle* p = pEmitter->m_pParticle;
    ICamera* pCam = GetCamera();
    if (!pCam) return false;

    Matrix4D alignTM = Matrix4D::identity;
    Vector3D vx, vy, vz;
    if (m_Alignment == baCamera)
    {
        vx = pCam->GetWorldTM().getV0();
        vy = pCam->GetWorldTM().getV1();
        vz = pCam->GetWorldTM().getV2();
    }
    else if (m_Alignment == baPlane)
    {
        vz = Vector3D::oZ;
        vx = pEmitter->m_WorldTM.getV0();
        vy.cross( vz, vx );
        vy.normalize();
        vx.cross( vy, vz );
    }
    else if (m_Alignment == baBeam)
    {
        vz = pCam->GetWorldTM().getV2();
        vx = pEmitter->m_WorldTM.getV0();
        vy.cross( vz, vx );
        vy.normalize();
        vz.cross( vx, vy );
    }

    alignTM.e00 = vx.x; alignTM.e01 = vx.y; alignTM.e02 = vx.z;
    alignTM.e10 = vy.x; alignTM.e11 = vy.y; alignTM.e12 = vy.z;
    alignTM.e20 = vz.x; alignTM.e21 = vz.y; alignTM.e22 = vz.z;

    if (!pEmitter->m_pEmitter->IsWorldSpace())
    {
        Vector3D tr( pEmitter->m_WorldTM.getTranslation() );
        Matrix4D tm;
        tm.scaling( pEmitter->m_WorldTM.getV0().norm() );
        tm.translate( tr );
        alignTM *= tm;
    }

    float s = pEmitter->m_pEmitter->IsWorldSpace() ? pEmitter->m_WorldTM.getV0().norm() : 1.0f;    
    while (p)
    {
        if (m_RenderBin.getNVert() + 4 >= c_ParticleRenderBinSize) 
        {
            RenderGeometry( pEmitter );
        }
        if (p->GetFlag( pfDead ))       { p = p->m_pNext; continue; }
        if (p->GetFlag( pfJustBorn ))   { p = p->m_pNext; continue; }
        if (IsHeadMoveDir()) 
        {
            alignTM.getV1() = p->m_Velocity; 
            alignTM.getV1().normalize();
            alignTM.getV0().cross( alignTM.getV1(), alignTM.getV2() );
        }
        AddQuad<Vertex2t>( m_RenderBin, p->m_Position, 
                            p->m_Size.x*s, p->m_Size.y*s, 
                            m_RefPoint.x + 0.5f, m_RefPoint.y + 0.5f, 
                            p->m_Roll, _modalp(p->m_Color,pEmitter->m_Alpha), p->m_UV, p->m_UV2,
                            alignTM );
        p = p->m_pNext;
    }
    return true; 
} // PBillboardRenderer::FillGeometry

void PBillboardRenderer::RenderGeometry( PEmitterInstance* pEmitter ) 
{
    if (m_RenderBin.getNVert() == 0) return;
    ICamera* pCam = GetCamera();
    if (!pCam) return;

    if (m_TexID == -1) SetTexName( GetTexName() );

    SetShader();

    int shd = pEmitter->m_pEmitter->GetShaderForEditor();
    if(shd!=-1)IRS->SetShader(shd);

    IRS->SetTexture( m_TexID );
    if (m_TexID2 != -1)
    {
        IRS->SetTexture( m_TexID2, 1 );
    }
    else
    {
        static int wTex = IRS->GetTextureID( "white.tga" );
        IRS->SetTexture( wTex, 1 );
    }

    //DWORD tint = m_Tint & 0x00FFFFFF;
    //tint |= (((DWORD)(pEmitter->m_Alpha*255.0f))<<24);
    //IRS->SetTextureFactor( tint );

    if (IsZWrite()) IRS->SetZWriteEnable();
    if (!IsZTest()) IRS->SetZEnable( false );
    IRS->SetDitherEnable( IsDitherEnable() );
    IRS->ResetWorldTM();

    float s = pEmitter->m_pEmitter->IsWorldSpace() ? 
        pEmitter->m_WorldTM.getV0().norm() : 1.0f;    

	m_ZBias = 0;//hack by DREW

    if (m_ZBias > 0)
    {
        if (!pCam) return;
        float shiftZ = -g_EMgr.GetZBiasMultiplier()*float( m_ZBias )*s;
        pCam->ShiftZ( -shiftZ );
        pCam->Render();    

        IRS->SetShaderAutoVars();
        DrawPrimBM( m_RenderBin );

        pCam->ShiftZ( shiftZ );
        pCam->Render();    
    }
    else
    {
        IRS->SetShaderAutoVars();
        DrawPrimBM( m_RenderBin );
    }
    m_RenderBin.setNVert( 0 );
    m_RenderBin.setNPri( 0 );
} // PBillboardRenderer::RenderGeometry

void PBillboardRenderer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PBillboardRenderer", this );
    pm.f( "Align", m_Alignment );
    pm.f( "RefX", m_RefPoint.x );
    pm.f( "RefY", m_RefPoint.y );
} // PBillboardRenderer::Expose

void PBillboardRenderer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << Enum2Byte( m_Alignment ) << m_RefPoint;
}

void PBillboardRenderer::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> Enum2Byte( m_Alignment ) >> m_RefPoint;
}

/*****************************************************************************/
/*    PConeRenderer implemetation
/*****************************************************************************/
PConeRenderer::PConeRenderer()
{
    m_HeightSegs    = 1;
    m_CircleSegs    = 16;
    m_TopRadius        = 0.2f;
    m_BotRadius        = 1.0f;
    m_Height        = 2.0f;    
    m_TopAlpha      = 1.0f;
    m_BotAlpha      = 1.0f;
    UpdateMesh();
} // PConeRenderer::PConeRenderer

static Primitive*    s_pMesh = NULL;
static int            s_Vert    = 0;
static int            s_Poly    = 0;
void PutVert( float x, float y, float z, float u, float v )
{
    if (!s_pMesh) return;
    Vertex2t& cv = *((Vertex2t*)s_pMesh->getVertexData() + s_Vert);
    cv.x = x;
    cv.y = y;
    cv.z = z;
    cv.u = u;
    cv.v = v;
    s_Vert++;
} // PutVert

void PutPoly( int a, int b, int c )
{
    if (!s_pMesh) return;
    WORD* idx = s_pMesh->getIndices() + s_Poly*3;
    idx[0] = a;
    idx[1] = b;
    idx[2] = c;
    s_Poly++;
} // PutPoly

void PConeRenderer::UpdateMesh()
{
    int nV = CreateCone( m_HeightSegs, m_CircleSegs, m_Height, 
        m_TopRadius, m_BotRadius, NULL, NULL, scGetNVert );
    int nP = CreateCone( m_HeightSegs, m_CircleSegs, m_Height, 
        m_TopRadius, m_BotRadius, NULL, NULL, scGetNPoly );
    m_Mesh.create( nV, nP*3, vfVertex2t );
    s_pMesh = &m_Mesh;
    s_Vert  = 0;
    s_Poly  = 0;
    CreateCone( m_HeightSegs, m_CircleSegs, m_Height, m_TopRadius, m_BotRadius, 
        PutVert, PutPoly, scCreate );
    m_Mesh.setNVert    ( nV );
    m_Mesh.setNPri    ( nP );
    m_Mesh.setNInd    ( nP*3 );

    m_WorkMesh = m_Mesh;
} // PConeRenderer::UpdateMesh

void PConeRenderer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_HeightSegs << m_CircleSegs << m_TopRadius << m_BotRadius << 
            m_Height << m_TopAlpha << m_BotAlpha;    
} // PConeRenderer::Serialize

void PConeRenderer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_HeightSegs >> m_CircleSegs >> m_TopRadius >> m_BotRadius >> 
            m_Height >> m_TopAlpha >> m_BotAlpha;    
    UpdateMesh();
} // PConeRenderer::Unserialize
    
void PConeRenderer::RenderGeometry( PEmitterInstance* pEmitter )
{
    if (!pEmitter->GetFlag( ifNeedDraw )) return;

    if (m_TexID == -1) SetTexName( GetTexName() );

    PParticle* p = pEmitter->m_pParticle;
    m_WorkMesh.setShader ( m_ShaderID );

    int shd = pEmitter->m_pEmitter->GetShaderForEditor();
    if(shd!=-1)m_WorkMesh.setShader(shd);

    m_WorkMesh.setTexture( m_TexID );
    if (m_TexID2 != -1)
    {
        m_WorkMesh.setTexture( m_TexID2, 1    );
    }
    else
    {
        static int wTex = IRS->GetTextureID( "white.tga" );
        m_WorkMesh.setTexture( wTex, 1 );
    }
    
    Matrix4D baseTM = Matrix4D::identity;
    if (!pEmitter->m_pEmitter->IsWorldSpace())
    {
        baseTM = pEmitter->m_WorldTM;
    }

    //IRS->SetTextureFactor( m_Tint );
    while (p)
    {
        if (p->m_Age == 0.0f) { p = p->m_pNext; continue; }
        Matrix4D tm( p->m_Size, p->m_Rotation, p->m_Position );
        tm *= baseTM;
        //IRS->SetWorldTM( tm );//drew

        int nV = m_Mesh.getNVert();
        Vertex2t* vs = (Vertex2t*)m_Mesh.getVertexData();
        Vertex2t* vd = (Vertex2t*)m_WorkMesh.getVertexData();
        for (int i = 0; i < nV; i++)
        {
            float alpha = float((p->m_Color&0xFF000000)>>24)/255.0f;
            float dAlpha = m_BotAlpha + (m_TopAlpha - m_BotAlpha)*vs->z/m_Height;
            alpha *= dAlpha;
            alpha *= pEmitter->m_Alpha;
            clamp( alpha, 0.0f, 1.0f );
            alpha *= 255.0f;
            DWORD ba = ((DWORD)alpha)<<24;
            vd->diffuse = (p->m_Color&0x00FFFFFF)|ba;
            vd->u  = vs->u*p->m_UV.w  + p->m_UV.x;
            vd->v  = vs->v*p->m_UV.h  + p->m_UV.y;
            vd->u2 = vs->u*p->m_UV2.w + p->m_UV2.x;
            vd->v2 = vs->v*p->m_UV2.h + p->m_UV2.y;
            //--drew--
            Vector3D V(vs->x,vs->y,vs->z);
            tm.transformPt(V);
            vd->x=V.x;vd->y=V.y;vd->z=V.z;
            //
            vd++; vs++;
        }        
        DrawBM( m_WorkMesh );
        p = p->m_pNext;
    }
} // PConeRenderer::RenderGeometry

void PConeRenderer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PConeRenderer", this );
    pm.p( "HeightSegs", &PConeRenderer::GetNHSegs, &PConeRenderer::SetNHSegs       );
    pm.p( "CircleSegs", &PConeRenderer::GetNCSegs, &PConeRenderer::SetNCSegs       );
    pm.p( "TopRadius", &PConeRenderer::GetTopR, &PConeRenderer::SetTopR         );
    pm.p( "BottomRadius", &PConeRenderer::GetBotR, &PConeRenderer::SetBotR         );
    pm.p( "Height", &PConeRenderer::GetConeHeight, &PConeRenderer::SetConeHeight   );
    pm.p( "TopAlpha", &PConeRenderer::GetTopAlpha, &PConeRenderer::SetTopAlpha     );
    pm.p( "BotAlpha", &PConeRenderer::GetBotAlpha, &PConeRenderer::SetBotAlpha     );
} // PConeRenderer::Expose

/*****************************************************************************/
/*    PSphereRenderer implemetation
/*****************************************************************************/
PSphereRenderer::PSphereRenderer()
{
    m_bHemisphere    = true;
    m_HeightSegs    = 6.0f;
    m_CircleSegs    = 16.0f;
    m_Radius        = 1.0f;    
    m_PolePhi       = 0.0f;  
    m_PoleTheta     = 0.0f;
    UpdateMesh();
} // PSphereRenderer::PSphereRenderer

void PSphereRenderer::UpdateMesh()
{
    int nV = 0;
    int nP = 0;
    if (m_bHemisphere)
    {
        nV = CreateHemisphere( m_HeightSegs, m_CircleSegs, m_Radius, 
                                NULL, NULL, scGetNVert, m_PolePhi, m_PoleTheta );
        nP = CreateHemisphere( m_HeightSegs, m_CircleSegs, m_Radius, 
                                NULL, NULL, scGetNPoly, m_PolePhi, m_PoleTheta );
    }
    else
    {
        nV = CreateSphere( m_HeightSegs, m_CircleSegs, m_Radius, 
                                NULL, NULL, scGetNVert, m_PolePhi, m_PoleTheta );
        nP = CreateSphere( m_HeightSegs, m_CircleSegs, m_Radius, 
                                NULL, NULL, scGetNPoly, m_PolePhi, m_PoleTheta );
    }
    
    m_Mesh.create( nV, nP*3, vfVertex2t );
    s_pMesh = &m_Mesh;
    s_Vert  = 0;
    s_Poly  = 0;

    if (m_bHemisphere)
    {
        CreateHemisphere( m_HeightSegs, m_CircleSegs, m_Radius, 
                            PutVert, PutPoly, scCreate, m_PolePhi, m_PoleTheta );
    }
    else
    {
        CreateSphere( m_HeightSegs, m_CircleSegs, m_Radius, 
                            PutVert, PutPoly, scCreate, m_PolePhi, m_PoleTheta );
    }
    m_Mesh.setNVert   ( nV );
    m_Mesh.setNPri    ( nP );
    m_Mesh.setNInd    ( nP*3 );

    m_WorkMesh = m_Mesh;
} // PSphereRenderer::UpdateMesh

void PSphereRenderer::RenderGeometry( PEmitterInstance* pEmitter )
{
    if (!pEmitter->GetFlag( ifNeedDraw )) return;

    if (m_TexID == -1) SetTexName( GetTexName() );

    PParticle* p = pEmitter->m_pParticle;
    m_WorkMesh.setShader ( m_ShaderID );

    int shd = pEmitter->m_pEmitter->GetShaderForEditor();
    if(shd!=-1)m_WorkMesh.setShader(shd);

    m_WorkMesh.setTexture( m_TexID );
    if (m_TexID2 != -1)
    {
        m_WorkMesh.setTexture( m_TexID2, 1 );
    }
    else
    {
        static int wTex = IRS->GetTextureID( "white.tga" );
        m_WorkMesh.setTexture( wTex, 1 );
    }

    Matrix4D baseTM = Matrix4D::identity;
    if (!pEmitter->m_pEmitter->IsWorldSpace())
    {
        baseTM = pEmitter->m_WorldTM;
    }
    
    //IRS->SetTextureFactor( m_Tint );
    while (p)
    {
        if (p->m_Age == 0.0f) { p = p->m_pNext; continue; }
        Matrix4D tm( p->m_Size, p->m_Rotation, p->m_Position );
        tm *= baseTM;
        //IRS->SetWorldTM( tm );

        int nV = m_Mesh.getNVert();
        Vertex2t* vs = (Vertex2t*)m_Mesh.getVertexData();
        Vertex2t* vd = (Vertex2t*)m_WorkMesh.getVertexData();
        DWORD c=_modalp(p->m_Color,pEmitter->m_Alpha);
        for (int i = 0; i < nV; i++)
        {
            vd->diffuse = c;
            vd->u  = vs->u*p->m_UV.w  + p->m_UV.x;
            vd->v  = vs->v*p->m_UV.h  + p->m_UV.y;
            vd->u2 = vs->u*p->m_UV2.w + p->m_UV2.x;
            vd->v2 = vs->v*p->m_UV2.h + p->m_UV2.y;
            //--drew--
            Vector3D V(vs->x,vs->y,vs->z);
            tm.transformPt(V);
            vd->x=V.x;vd->y=V.y;vd->z=V.z;
            //
            vd++; vs++;
        }       
        
        DrawBM( m_WorkMesh );
        p = p->m_pNext;
    }
} // PSphereRenderer::RenderGeometry

void PSphereRenderer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_bHemisphere << m_HeightSegs << m_CircleSegs << m_Radius << m_PolePhi << m_PoleTheta;                                          
}                                                                  
                                                                  
void PSphereRenderer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_bHemisphere >> m_HeightSegs >> m_CircleSegs >> m_Radius >> m_PolePhi >> m_PoleTheta;
    UpdateMesh();
} // PSphereRenderer::Unserialize

void PSphereRenderer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PSphereRenderer", this );
    pm.p( "Hemisphere", &PSphereRenderer::GetHemisphere, &PSphereRenderer::SetHemisphere );
    pm.p( "HeightSegs", &PSphereRenderer::GetNHSegs, &PSphereRenderer::SetNHSegs );
    pm.p( "CircleSegs", &PSphereRenderer::GetNCSegs, &PSphereRenderer::SetNCSegs );
    pm.p( "TopRadius", &PSphereRenderer::GetRadius, &PSphereRenderer::SetRadius );
    pm.p( "PolePhi", &PSphereRenderer::GetPolePhi, &PSphereRenderer::SetPolePhi );
    pm.p( "PoleTheta", &PSphereRenderer::GetPoleTheta, &PSphereRenderer::SetPoleTheta );
} // PSphereRenderer::Expose

/*****************************************************************************/
/*    PChainRenderer implemetation
/*****************************************************************************/
PChainRenderer::PChainRenderer()
{
    SetBindHead();
    m_MaxLength = 10000.0f;
    SetTexName( "ray.tga" );
} // PChainRenderer::PChainRenderer

template <class TVert>
bool AddQuad(   Primitive& pri, const Vector3D& a, const Vector3D& b, 
                const Vector3D& c, const Vector3D& d, 
                DWORD ca, DWORD cb, DWORD cc, DWORD cd, 
                const Rct& uv, const Rct& uv2 )
{
    int nVert = pri.getNVert();
    if (nVert == pri.getMaxVert()) return false;
    TVert* v = ((TVert*)pri.getVertexData() + nVert);

    v[0] = a;
    v[1] = b;
    v[2] = c;
    v[3] = d;

    //  color and texture coordinates
    v[0].diffuse = ca;
    v[0].u         = uv.x;
    v[0].v         = uv.y + uv.h;
    v[0].u2         = uv2.x;
    v[0].v2         = uv2.y + uv2.h;

    v[1].diffuse = cb;
    v[1].u         = uv.x + uv.w;
    v[1].v         = uv.y + uv.h;
    v[1].u2         = uv2.x + uv2.w;
    v[1].v2         = uv2.y + uv2.h;

    v[2].diffuse = cc;
    v[2].u         = uv.x;
    v[2].v         = uv.y;
    v[2].u2         = uv2.x;
    v[2].v2         = uv2.y;

    v[3].diffuse = cd;
    v[3].u         = uv.x + uv.w;
    v[3].v         = uv.y;
    v[3].u2         = uv2.x + uv2.w;
    v[3].v2         = uv2.y;

    pri.setNPri ( nVert/2 + 2 );
    pri.setNVert( nVert + 4 );
    return true;
} // AddQuad

void PChainRenderer::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Flags;
} // PChainRenderer::Serialize

void PChainRenderer::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Flags;
} // PChainRenderer::Unserialize

bool PChainRenderer::FillGeometry( PEmitterInstance* pEmitter )
{
    if (!pEmitter->GetFlag( ifNeedDraw )) return false;

    ICamera* pCam = GetCamera();
    if (!pCam) return false;
    Matrix3D camTM = pCam->GetWorldTM();
    const Vector3D cDir = IsAlignGround() ? Vector3D::oZ : pCam->GetDir();

    PParticle* p = pEmitter->m_pParticle;
    while (p && (p->m_Age == 0.0f || p->GetFlag( pfDead ) || p->GetFlag( pfJustBorn ))) p = p->m_pNext;
    if (!p) return false;
    PParticle* pn = p->m_pNext;
    if (!pn) return false;

    Vector3D a, b, c, d;
    Vector3D right;
    Vector3D forward;

    if (IsBindHead() && pEmitter->m_pEmitter->IsWorldSpace())
    {
        a = pEmitter->m_WorldTM.getTranslation();
        b = a;
    }
    else
    {
        a = p->m_Position;
        b = p->m_Position;
    }

    float s = pEmitter->m_pEmitter->IsWorldSpace() ? pEmitter->m_WorldTM.getV0().norm() : 1.0f;    
    Matrix4D tm = Matrix4D::identity;
    if (!pEmitter->m_pEmitter->IsWorldSpace())
    {
        Vector3D tr( pEmitter->m_WorldTM.getTranslation() );
        tm.scaling( pEmitter->m_WorldTM.getV0().norm() );
        tm.translate( tr );
        IRS->SetWorldTM( tm );
    }

    bool bFirst = true;
    while (p)
    {
        if (m_RenderBin.getNVert() + 4 >= c_ParticleRenderBinSize) 
        {
            RenderGeometry( pEmitter );
        }

        pn = p->m_pNext;
        if (pn && p->m_Age  > 0.0f && 
            pn->m_Age > 0.0f &&
            !p->GetFlag ( pfDead ) && 
            !pn->GetFlag( pfDead ) &&
            !pn->GetFlag( pfJustBorn ))
        {
            forward.sub( pn->m_Position, p->m_Position );
            float len = forward.normalize();
            if (len >= m_MaxLength || len < c_Epsilon) 
            {
                p = pn;
                continue;
            }
            right.cross( cDir, forward );
            //  correct it
            c = pn->m_Position;
            d = pn->m_Position;
            c.addWeighted( right, -pn->m_Size.x*0.5f*s );
            d.addWeighted( right,  pn->m_Size.x*0.5f*s );
            if (bFirst)
            {
                a.addWeighted( right, -p->m_Size.x*0.5f*s );
                b.addWeighted( right,  p->m_Size.x*0.5f*s );
                bFirst = false;
            }

            Vector3D va( a ), vb( b ), vc( c ), vd( d );
            tm.transformPt( va );
            tm.transformPt( vb );
            tm.transformPt( vc );
            tm.transformPt( vd );

            //if (va.distance(vd) > 300.0f)
            //{
            //    int t = 0;
            //}

            DWORD C=_modalp(pn->m_Color,pEmitter->m_Alpha);
            AddQuad<Vertex2t>( m_RenderBin, va, vb, vc, vd, 
                C, C, C, C,
                p->m_UV, p->m_UV2 );
            a = c; b = d;
        }
        p = pn;
    }
    return true;
} // PChainRenderer::FillGeometry

void PChainRenderer::RenderGeometry( PEmitterInstance* pEmitter )
{
    ICamera* pCam = GetCamera();
    if (!pCam) return;

    if (m_TexID == -1) SetTexName( GetTexName() );

    if (m_RenderBin.getNVert() == 0) return;
    m_RenderBin.setShader ( m_ShaderID );

    int shd = pEmitter->m_pEmitter->GetShaderForEditor();
    if(shd!=-1)m_RenderBin.setShader(shd);

    m_RenderBin.setTexture( m_TexID );
    if (m_TexID2 != -1)
    {
        m_RenderBin.setTexture( m_TexID2, 1 );
    }
    else
    {
        static int wTex = IRS->GetTextureID( "white.tga" );
        m_RenderBin.setTexture( wTex, 1 );
    }

    //DWORD tint = m_Tint & 0x00FFFFFF;
    //tint |= (((DWORD)(pEmitter->m_Alpha*255.0f))<<24);
    //IRS->SetTextureFactor( tint );

    IRS->ResetWorldTM();
    float s = pEmitter->m_pEmitter->IsWorldSpace() ? pEmitter->m_WorldTM.getV0().norm() : 1.0f;    
    DrawBM( m_RenderBin );
    m_RenderBin.setNVert( 0 );
    m_RenderBin.setNPri( 0 );
} // PChainRenderer::RenderGeometry

void PChainRenderer::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PChainRenderer", this );
    pm.p( "BindHead", &PChainRenderer::IsBindHead, &PChainRenderer::SetBindHead );
    pm.p( "AlignGround", &PChainRenderer::IsAlignGround, &PChainRenderer::SetAlignGround );
    pm.f( "MaxLength", m_MaxLength );
} // PChainRenderer::Expose

/*****************************************************************************/
/*    PMouseBind implemetation
/*****************************************************************************/
PMouseBind::PMouseBind()
{
    m_Depth = 0.5f;
}

void PMouseBind::Process( PEmitterInstance* pEmitter )
{
    if (!pEmitter->GetFlag( ifNeedDraw )) return;

    ICamera* pCam = GetCamera();
    if (!pCam) return;
    
    POINT pt;
    GetCursorPos( &pt );
    float mX = pt.x;
    float mY = pt.y;
    
    Vector4D pos( mX, mY, m_Depth, 1.0f );
    pCam->ToSpace( sScreen, sWorld, pos );

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        p->m_Position = pos;
        p = p->m_pNext;
    }
} // PMouseBind::Process

void PMouseBind::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PMouseBind", this );
    pm.f( "Depth", m_Depth );
}

void PMouseBind::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Depth;                                          
}                                                                  

void PMouseBind::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Depth;
} // PMouseBind::Unserialize

/*****************************************************************************/
/*    PTerrainDecal implemetation
/*****************************************************************************/
PTerrainDecal::PTerrainDecal()
{
}

void PTerrainDecal::RenderGeometry( PEmitterInstance* pEmitter )
{
    if (!pEmitter->GetFlag( ifNeedDraw )) return;

    if (m_TexID == -1) SetTexName( GetTexName() );

    PParticle* p = pEmitter->m_pParticle;

    ICamera* pCam = GetCamera();
    if (!pCam) return;
    
    m_RenderBin.setNVert( 0 );
    m_RenderBin.setNPri ( 0 );
    
    Matrix4D wtm = Matrix4D::identity;
    if (!pEmitter->m_pEmitter->IsWorldSpace())
    {
        Vector3D tr( pEmitter->m_WorldTM.getTranslation() );
        wtm.scaling( pEmitter->m_WorldTM.getV0().norm() );
        wtm.translate( tr );
    }

    float s = pEmitter->m_pEmitter->IsWorldSpace() ? pEmitter->m_WorldTM.getV0().norm() : 1.0f;    
	//hack (by Drew)
	if(fabs(s)>1000)s=1.0f;
	//////
    while (p)
    {
        if (p->GetFlag( pfDead ))       { p = p->m_pNext; continue; }
        if (p->GetFlag( pfJustBorn ))   { p = p->m_pNext; continue; }
        
        Vector3D pos( p->m_Position );
        Vector3D tpos( pos );
        wtm.transformPt( tpos );
        tpos.z = ITerra->GetH( tpos.x, tpos.y );

        Vector3D vz = ITerra->GetNormal( tpos.x, tpos.y );
        Vector3D vx = pEmitter->m_WorldTM.getV0();
        vx.normalize();
        Vector3D vy; vy.cross( vx, vz ); 
        vy.normalize();
        vx.cross( vy, vz ); 

        Matrix4D tm( vx, vy, vz );
        //tm.translate( pos );
        AddQuad<Vertex2t>( m_RenderBin, tpos, p->m_Size.x*s, p->m_Size.y*s, 0.5f, 0.5f,
                            p->m_Roll, _modalp(p->m_Color,pEmitter->m_Alpha), p->m_UV, p->m_UV2, tm );
        p = p->m_pNext;
    }

    wtm.setTranslation( Vector3D::null );
    
    SetShader();

    int shd = pEmitter->m_pEmitter->GetShaderForEditor();
    if(shd!=-1)IRS->SetShader(shd);

    IRS->SetTexture         ( m_TexID        );
    if (m_TexID2 != -1)
    {
        IRS->SetTexture         ( m_TexID2, 1    );
    }
    else
    {
        static int wTex = IRS->GetTextureID( "white.tga" );
        IRS->SetTexture         ( wTex, 1 );
    }

    //DWORD tint = m_Tint & 0x00FFFFFF;
    //tint |= (((DWORD)(pEmitter->m_Alpha*255.0f))<<24);
    //IRS->SetTextureFactor( tint );

    if (IsZWrite()) IRS->SetZWriteEnable();
    if (!IsZTest()) IRS->SetZEnable( false );
    IRS->SetDitherEnable( IsDitherEnable() );
    
    if (m_ZBias > 0)
    {
        if (!pCam) return;
        float shiftZ = -g_EMgr.GetZBiasMultiplier()*float( m_ZBias )*s;
        pCam->ShiftZ( -shiftZ );
        pCam->Render();    
        IRS->SetShaderAutoVars();
        DrawPrimBM( m_RenderBin );
        pCam->ShiftZ( shiftZ );
        pCam->Render();    
    }
    else
    {
        IRS->SetShaderAutoVars();
        DrawPrimBM( m_RenderBin );
    }
} // PTerrainDecal::RenderGeometry

void PTerrainDecal::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PTerrainDecal", this );
}

/*****************************************************************************/
/*    PWaterDecal implemetation
/*****************************************************************************/
PWaterDecal::PWaterDecal()
{
}

void PWaterDecal::Process( PEmitterInstance* pEmitter )
{
    if (!pEmitter->GetFlag( ifNeedDraw )) return;

    PParticle* p = pEmitter->m_pParticle;
    Matrix3D camTM = Matrix3D::identity;

    m_RenderBin.setNVert( 0 );
    m_RenderBin.setNPri ( 0 );
    while (p)
    {
        if (p->m_Age > 0.0f || p->GetFlag( pfImmortal ))
        {
            AddQuad<Vertex2t>(    m_RenderBin, p->m_Position, 
                                p->m_Size.x, p->m_Size.y, 0.5f, 0.5f,
                                p->m_Roll,
                                _modalp(p->m_Color,pEmitter->m_Alpha), 
                                p->m_UV, 
                                p->m_UV2,
                                camTM );
        }
        p = p->m_pNext;
    }

    m_RenderBin.setShader ( m_ShaderID );

    int shd = pEmitter->m_pEmitter->GetShaderForEditor();
    if(shd!=-1)m_RenderBin.setShader(shd);

    m_RenderBin.setTexture( m_TexID );
    if (m_TexID2 != -1)
    {
        m_RenderBin.setTexture( m_TexID2, 1    );
    }
    else
    {
        static int wTex = IRS->GetTextureID( "white.tga" );
        m_RenderBin.setTexture( wTex, 1 );
    }
    //IRS->SetTextureFactor( m_Tint );

    DrawBM( m_RenderBin );
} // PWaterDecal::Process

void PWaterDecal::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PWaterDecal", this );
}

/*****************************************************************************/
/*    PColorInit implementation
/*****************************************************************************/
PColorInit::PColorInit()
{
    m_AvgColor    = 0xFFFFFFFF;    
    m_R = m_G = m_B = m_A = 255;
    m_RedV        = 0;        
    m_GreenV    = 0;    
    m_BlueV        = 0;    
    m_AlphaV    = 0;    
}

void PColorInit::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PColorInit", this );
    pm.p( "Color", &PColorInit::GetAvgColor, &PColorInit::SetAvgColor, "color" );
    pm.f( "dRed",    m_RedV        );
    pm.f( "dGreen", m_GreenV    );
    pm.f( "dBlue",    m_BlueV        );
    pm.f( "dAlpha", m_AlphaV    );
}

void PColorInit::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_AvgColor << m_RedV << m_GreenV << m_BlueV << m_AlphaV;
}

void PColorInit::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_AvgColor >> m_RedV >> m_GreenV >> m_BlueV >> m_AlphaV;
    SetAvgColor( m_AvgColor );
}

void PColorInit::InitParticle( PParticle& p )
{
    int r = rndValue( -m_RedV,        m_RedV     ) + m_R;
    int g = rndValue( -m_GreenV,    m_GreenV ) + m_G;
    int b = rndValue( -m_BlueV,        m_BlueV  ) + m_B;
    int a = rndValue( -m_AlphaV,    m_AlphaV ) + m_A;
    clamp( r, 0, 255 );
    clamp( g, 0, 255 );
    clamp( b, 0, 255 );
    clamp( a, 0, 255 );

    p.m_Color = (a << 24)|(r << 16)|(g << 8)|(b);
} // PColorInit::InitParticle

void PColorInit::SetAvgColor( DWORD val )
{
    m_AvgColor = val;
    m_A = (val & 0xFF000000)>>24;
    m_R = (val & 0x00FF0000)>>16;
    m_G = (val & 0x0000FF00)>>8;
    m_B = (val & 0x000000FF);
}

/*****************************************************************************/
/*    PColorRampInit implemetation
/*****************************************************************************/
PColorRampInit::PColorRampInit()
{
}

void PColorRampInit::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PColorRampInit", this );
    pm.p( "NumKeys", &PColorRampInit::GetNKeys );
    pm.p( "Ramp", &PColorRampInit::GetColorRamp, &PColorRampInit::SetColorRamp );
} // PColorRampInit::Expose

void PColorRampInit::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    m_Ramp.Serialize( os );
}

void PColorRampInit::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    m_Ramp.Unserialize( is );
} // PColorRampInit::Unserialize

void PColorRampInit::InitParticle( PParticle& p )
{
    p.m_Color = m_Ramp.GetColor( rndValuef() );
}

/*****************************************************************************/
/*    PSizeInit implementation
/*****************************************************************************/
PSizeInit::PSizeInit()
{
    m_Size    = Vector3D( 100.0f, 100.0f, 100.0f );        
    m_SizeV    = 0.0f;
}

void PSizeInit::InitParticle( PParticle& p )
{
    p.m_Size = m_Size;
    float dsize = rndValuef( -m_SizeV, m_SizeV );
    p.m_Size.x += dsize;
    p.m_Size.y += dsize;
    p.m_Size.z += dsize;
}

void PSizeInit::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PSizeInit", this );
    pm.f( "SizeX",    m_Size.x );
    pm.f( "SizeY",    m_Size.y );
    pm.f( "SizeZ",    m_Size.z );
    pm.f( "dSize",    m_SizeV );
} // PSizeInit::Expose

void PSizeInit::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Size << m_SizeV;
}

void PSizeInit::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Size >> m_SizeV;
}

/*****************************************************************************/
/*    PFrameInit implementation
/*****************************************************************************/
PFrameInit::PFrameInit()
{
    m_NRows = 4;
    m_NCols = 4;
    m_Flags = 0;
}

void PFrameInit::InitParticle( PParticle& p )
{
    int nF = m_NRows*m_NCols;
    float w = 1.0f / m_NCols;
    float h = 1.0f / m_NRows;
    
    int frame = 0;
    
    if (IsSequentialInit())
    {
        PParticle* pNext = p.m_pNext;
        if (pNext)
        {
            
        }
    } else frame = rndValue( 0, nF - 1 );

     int row = frame / m_NCols;
    int col = frame % m_NCols;
    p.m_Frame = frame;

    if (IsSecondChannel())
    {
        p.m_UV2.x = float( col )*w;
        p.m_UV2.y = float( row )*h;
        p.m_UV2.w = w;
        p.m_UV2.h = h;
    }
    else
    {
        p.m_UV.x = float( col )*w;
        p.m_UV.y = float( row )*h;
        p.m_UV.w = w;
        p.m_UV.h = h;
    }
} // PFrameInit::InitParticle

void PFrameInit::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PFrameInit", this );
    pm.f( "Columns",        m_NCols );
    pm.f( "Rows",            m_NRows );
    pm.p( "SecondChannel", &PFrameInit::IsSecondChannel, &PFrameInit::SetIsSecondChannel );
    pm.p( "SequentialInit", &PFrameInit::IsSequentialInit, &PFrameInit::SetIsSequentialInit );
} // PFrameInit::Expose

void PFrameInit::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_NCols << m_NRows << m_Flags;
}

void PFrameInit::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_NCols >> m_NRows >> m_Flags;
}

/*****************************************************************************/
/*    PAttract implemetation
/*****************************************************************************/
PAttract::PAttract()
{
    m_Pos           = Vector3D::null;   
    m_Magnitude     = 1.0f;
    m_Radius        = 20.0f;  
    m_FadeMode      = afmLinear;
} // PAttract::PAttract

void PAttract::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PAttract", this );
    pm.f( "PosX",       m_Pos.x     );
    pm.f( "PosY",       m_Pos.y     );
    pm.f( "PosZ",       m_Pos.z     );
    pm.f( "Magnitude",  m_Magnitude );
    pm.f( "Radius",     m_Radius    );
    pm.f( "FadeMode",   m_FadeMode  );
} // PAttract::Expose

void PAttract::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Pos << m_Magnitude << m_Radius << Enum2Byte( m_FadeMode );
}

void PAttract::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Pos >> m_Magnitude >> m_Radius >> Enum2Byte( m_FadeMode );
}

void PAttract::Render()
{
    if (!DoDrawGizmo()) return;
    Vector3D pos = m_Pos;
    /*if (pEmitter->m_pEmitter->IsWorldSpace())
    {
        pos += pEmitter->m_WorldTM.getTranslation();
    }*/
    DrawStar( Sphere( pos, 5.0f ), 0xFFCCFF33, 0xFFCCFF33 );
    DrawSphere( Sphere( pos, m_Radius ), 0x33CCFF33, 0x77CCFF33, 16 );
} // PAttract::Render

void PAttract::Process( PEmitterInstance* pEmitter )
{
    if (m_Radius < c_SmallEpsilon || fabs( m_Magnitude ) < c_SmallEpsilon) return;
    float dt = pEmitter->GetTimeDelta();       
       PParticle* p = pEmitter->m_pParticle;

    Vector3D wpos = m_Pos;
    if (pEmitter->m_pEmitter->IsWorldSpace())
    {
        wpos += pEmitter->m_WorldTM.getTranslation();
    }

    while (p)
    {
        Vector3D dir;

        dir.sub( wpos, p->m_Position );
        float norm2 = dir.norm2();
        if (norm2 > m_Radius*m_Radius || norm2 < c_SmallEpsilon) 
        //  outside influence
        { 
            p = p->m_pNext; 
            continue; 
        }
        float dist = dir.normalize();
        dir *= dt;
        if (m_FadeMode == afmConstant)
        {
            dir *= m_Magnitude;
        }
        else if (m_FadeMode == afmLinear)
        {
            dir *= m_Magnitude*(1.0f - dist/m_Radius);
        }
        else if (m_FadeMode == afmQuadratic)
        {
            const float c_QBend = 0.1f;
            dist = m_Radius - dist;
            dir *= dist*dist*m_Magnitude/(m_Radius*m_Radius);
        }
           p->m_Velocity += dir;
           p = p->m_pNext;
    } 
} // PAttract::Process

/*****************************************************************************/
/*    PForce implemetation
/*****************************************************************************/
PForce::PForce()
{
    m_Force.x = 0.0f;
    m_Force.y = 0.0f;
    m_Force.z = -50.0f;
}

void PForce::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PForce", this );
    pm.f( "ForceX", m_Force.x );
    pm.f( "ForceY", m_Force.y );
    pm.f( "ForceZ", m_Force.z );
} // PForce::Expose

void PForce::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Force;
} // PForce::Serialize

void PForce::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Force;
} // PForce::Unserialize

void PForce::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    Vector3D dv = m_Force;
	pEmitter->m_WorldTM.transformVec( dv );
    dv *= dt;
    
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        p->m_Velocity += dv;
        p = p->m_pNext;
    }
} // PForce::Process

/*****************************************************************************/
/*    PWind implemetation
/*****************************************************************************/
PWind::PWind()
{
    m_Magnitude.x    = 50.0f;
    m_Magnitude.y    = 0.0f;
    m_Magnitude.z    = 0.0f;
    m_Frequency        = 1.0f;
    m_Shift            = 0.0f;
}

void PWind::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PWind", this );
    pm.f( "MagX",        m_Magnitude.x    );
    pm.f( "MagY",        m_Magnitude.y    );
    pm.f( "MagZ",        m_Magnitude.z    );
    pm.f( "Frequency",    m_Frequency        );
    pm.f( "Shift",        m_Shift            );
} // PWind::Expose

void PWind::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Magnitude << m_Frequency << m_Shift;
} // PWind::Serialize

void PWind::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Magnitude >> m_Frequency >> m_Shift;
} // PWind::Unserialize

void PWind::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    Vector3D dv = m_Magnitude;
	pEmitter->m_WorldTM.transformVec( dv );
    float ampl = PerlinNoise( m_Shift*m_Frequency, 
                                (pEmitter->m_CurTime - pEmitter->m_StartTime)*m_Frequency );
    dv *= dt*ampl;
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        p->m_Velocity += dv;
        p = p->m_pNext;
    }
} // PWind::Process


/*****************************************************************************/
/*    PTorque implemetation
/*****************************************************************************/
PTorque::PTorque()
{
    m_Torque = Vector3D::oZ;
}

void PTorque::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PTorque", this );
    pm.f( "TorqueX", m_Torque.x );
    pm.f( "TorqueY", m_Torque.y );
    pm.f( "TorqueZ", m_Torque.z );
} // PTorque::Expose

void PTorque::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Torque;
} // PTorque::Serialize

void PTorque::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Torque;
} // PTorque::Unserialize

void PTorque::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    Vector3D dv = m_Torque;
	pEmitter->m_WorldTM.transformVec( dv );
    dv *= dt;

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        p->m_AngVelocity += dv;
        p = p->m_pNext;
    }
} // PTorque::Process


/*****************************************************************************/
/*    PFollow implemetation
/*****************************************************************************/
PFollow::PFollow()
{
    m_Magnitude = 10.0f;
    m_bFollowOlder = true;
}

void PFollow::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PFollow", this );
    pm.f( "Magnitude", m_Magnitude );
    pm.f( "FollowOlder", m_bFollowOlder );
} // PFollow::Expose

void PFollow::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Magnitude << m_bFollowOlder;
}

void PFollow::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Magnitude >> m_bFollowOlder;
} // PFollow::Unserialize

void PFollow::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    PParticle* p = pEmitter->m_pParticle;

    if (m_bFollowOlder)
    {
        while (p)
        {
            Vector3D v;
            PParticle* pNext = p->m_pNext;
            if (pNext)
            {
                v.sub( pNext->m_Position, p->m_Position );
                v.normalize();
                v *= m_Magnitude;
                p->m_Velocity += v;
            }
            p = p->m_pNext;
        }
    }
    else
    {
        while (p)
        {
            Vector3D v;
            PParticle* pPrev = p->m_pPrev;
            if (pPrev)
            {
                v.sub( pPrev->m_Position, p->m_Position );
                v.normalize();
                v *= m_Magnitude;
                p->m_Velocity += v;
            }
            p = p->m_pNext;
        }
    }
} // PFollow::Process

/*****************************************************************************/
/*    PAlphaFade implemetation
/*****************************************************************************/
PAlphaFade::PAlphaFade()
{
    m_bAbsolute  = true;
    m_StartAlpha = 1.0f;
    m_EndAlpha     = 0.0f;
}

void PAlphaFade::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PAlphaFade", this );
    pm.f( "StartAlpha", m_StartAlpha );
    pm.f( "EndAlpha", m_EndAlpha );
    pm.f( "Absolute", m_bAbsolute );
} // PAlphaFade::Expose

void PAlphaFade::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_StartAlpha << m_EndAlpha << m_bAbsolute;
}

void PAlphaFade::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_StartAlpha >> m_EndAlpha >> m_bAbsolute;
} // PAlphaFade::Unserialize

void PAlphaFade::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        float alpha = m_StartAlpha + (m_EndAlpha - m_StartAlpha)*p->m_Age/p->m_TimeToLive;
        clamp( alpha, 0.0f, 1.0f );
        if (!m_bAbsolute)
        {
            alpha *= (p->m_Color&0xFF000000)>>24;
        }
        else
        {
            alpha *= 255.0f;
        }
        p->m_Color &= 0x00FFFFFF;
        p->m_Color |= (DWORD( alpha ))<<24;
        p = p->m_pNext;
    }
} // PAlphaFade::Process


/*****************************************************************************/
/*    PAVelRamp implemetation
/*****************************************************************************/
PAVelRamp::PAVelRamp()
{
    m_MinVel  = -2.0f;
    m_MaxVel  = 2.0f;
    m_Repeat  = 1;
    m_Axis      = aXYZ; 
}

void PAVelRamp::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PAVelRamp", this );
    pm.f( "MinVel",        m_MinVel );
    pm.f( "MaxVel",        m_MaxVel );
    pm.f( "Axis",        m_Axis );
    pm.f( "Repeat",        m_Repeat );
    pm.p( "NumKeys", &PAVelRamp::GetNKeys );
    pm.p( "Ramp", &PAVelRamp::GetVelRamp, &PAVelRamp::SetVelRamp );
} // PAVelRamp::Expose

void PAVelRamp::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    m_Ramp.Serialize( os );
    os << m_MinVel << m_MaxVel << m_Repeat << Enum2Byte( m_Axis );
}

void PAVelRamp::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    m_Ramp.Unserialize( is );
    is >> m_MinVel >> m_MaxVel >> m_Repeat >> Enum2Byte( m_Axis );
} // PAVelRamp::Unserialize

void PAVelRamp::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        float t = m_Repeat * p->m_Age / p->m_TimeToLive;
        t -= floorf( t );
        float av = m_MinVel +  m_Ramp.GetAlpha( t )*(m_MaxVel - m_MinVel);
        if (m_Axis&1) p->m_AngVelocity.x = av;
        if (m_Axis&2) p->m_AngVelocity.y = av;
        if (m_Axis&4) p->m_AngVelocity.z = av;
        p = p->m_pNext;
    }
} // PAVelRamp::Process

/*****************************************************************************/
/*    PVelRamp implemetation
/*****************************************************************************/
PVelRamp::PVelRamp()
{
    m_MinVel  = -2.0f;
    m_MaxVel  = 2.0f;
    m_Repeat  = 1;
}

void PVelRamp::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PVelRamp", this );
    pm.f( "MinVel",        m_MinVel );
    pm.f( "MaxVel",        m_MaxVel );
    pm.f( "Repeat",        m_Repeat );
    pm.p( "NumKeys", &PVelRamp::GetNKeys );
    pm.p( "Ramp", &PVelRamp::GetVelRamp, &PVelRamp::SetVelRamp );
} // PVelRamp::Expose

void PVelRamp::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    m_Ramp.Serialize( os );
    os << m_MinVel << m_MaxVel << m_Repeat;
}

void PVelRamp::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    m_Ramp.Unserialize( is );
    is >> m_MinVel >> m_MaxVel >> m_Repeat;
} // PVelRamp::Unserialize

void PVelRamp::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    float deltaVel = m_MaxVel - m_MinVel;
    if (fabs( deltaVel ) < c_Epsilon) return;

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        float t = m_Repeat * p->m_Age / p->m_TimeToLive;
        t -= floorf( t );
        float v = m_MinVel +  m_Ramp.GetAlpha( t )*deltaVel;
        p->m_Velocity.normalize();
        p->m_Velocity *= v;
        p = p->m_pNext;
    }
} // PVelRamp::Process

/*****************************************************************************/
/*    PCoastBreak implemetation
/*****************************************************************************/
PCoastBreak::PCoastBreak()
{
    m_Magnitude     = 0.1f;
    m_WaterLevel    = 50.0f;
}

void PCoastBreak::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PCoastBreak", this );
    pm.f( "Magnitude", m_Magnitude );
    pm.f( "WaterLevel", m_WaterLevel );
} // PCoastBreak::Expose

void PCoastBreak::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Magnitude << m_WaterLevel;
}

void PCoastBreak::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Magnitude >> m_WaterLevel;
} // PCoastBreak::Unserialize

void PCoastBreak::Process( PEmitterInstance* pEmitter )
{
    if (!ITerra) return;
    float dt = pEmitter->GetTimeDelta();

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        float H = ITerra->GetH( p->m_Position.x, p->m_Position.y );
        if (H > m_WaterLevel)
        {
            p->m_Velocity *= m_Magnitude;
            p->m_Frame = 1;
            Trigger( pEmitter, *p );
        }
        p = p->m_pNext;
    }
} // PCoastBreak::Process

void PCoastBreak::Trigger( const PEmitterInstance* pEmitter, PParticle& p )
{
    
} // PCoastBreak::Trigger

/*****************************************************************************/
/*    PAlphaRamp implemetation
/*****************************************************************************/
PAlphaRamp::PAlphaRamp()
{
    m_NRepeats = 1;
}

void PAlphaRamp::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PAlphaRamp", this );
    pm.f( "Repeat", m_NRepeats );
    pm.p( "NumKeys", &PAlphaRamp::GetNKeys );
    pm.p( "Ramp", &PAlphaRamp::GetAlphaRamp, &PAlphaRamp::SetAlphaRamp );
} // PAlphaRamp::Expose

void PAlphaRamp::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    m_Ramp.Serialize( os );
    os << m_NRepeats;
}

void PAlphaRamp::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    m_Ramp.Unserialize( is );
    is >> m_NRepeats;
} // PAlphaRamp::Unserialize

void PAlphaRamp::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        float t = m_NRepeats * p->m_Age / p->m_TimeToLive;
        t -= floorf( t );
        float alpha = m_Ramp.GetAlpha( t )*255.0f;
        p->m_Color &= 0x00FFFFFF;
        p->m_Color |= ((DWORD)alpha)<<24;
        p = p->m_pNext;
    }
} // PAlphaRamp::Process

/*****************************************************************************/
/*    PColorRamp implemetation
/*****************************************************************************/
PColorRamp::PColorRamp()
{
    m_NRepeats = 1;
}

void PColorRamp::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PColorRamp", this );
    pm.f( "Repeat", m_NRepeats );
    pm.p( "NumKeys", &PColorRamp::GetNKeys );
    pm.p( "Ramp", &PColorRamp::GetColorRamp, &PColorRamp::SetColorRamp );
} // PColorRamp::Expose

void PColorRamp::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    m_Ramp.Serialize( os );
    os << m_NRepeats;
}

void PColorRamp::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    m_Ramp.Unserialize( is );
    is >> m_NRepeats;
} // PColorRamp::Unserialize

void PColorRamp::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        float t = m_NRepeats * p->m_Age / p->m_TimeToLive;
        t -= floorf( t );
        p->m_Color &= 0xFF000000;
        p->m_Color |= (m_Ramp.GetColor( t )&0x00FFFFFF);
        p = p->m_pNext;
    }
} // PColorRamp::Process

/*****************************************************************************/
/*    PNatcolor implemetation
/*****************************************************************************/
PNatcolor::PNatcolor()
{
    m_Modulate2X = true;
}

void PNatcolor::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PNatcolor", this );
    pm.f( "Modulate2X", m_Modulate2X );    
} // PColorRamp::Expose

void PNatcolor::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );    
    os << m_Modulate2X;
}

void PNatcolor::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );    
    is >> m_Modulate2X;
} // PNatcolor::Unserialize
DWORD _ModDWORD2XRGB(DWORD C1,DWORD C2){
    DWORD R=0;
    for(int i=0;i<4;i++){
        int CH1=C1&255;
        int CH2=C2&255;
        C1>>=8;
        C2>>=8;
        int CC = i==3 ? CH1 : ((CH1*CH2)>>7);
        if(CC>255)CC=255;
        R|=(CC<<(i<<3));
    }
    return R;
}
DWORD _ModDWORDRGB(DWORD C1,DWORD C2){
    DWORD R=0;
    for(int i=0;i<4;i++){
        int CH1=C1&255;
        int CH2=C2&255;
        C1>>=8;
        C2>>=8;
        int CC = i==3 ? CH1 : ((CH1*CH2)>>8);
        if(CC>255)CC=255;
        R|=(CC<<(i<<3));
    }
    return R;
}
void PNatcolor::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        DWORD C1=pEmitter->m_Natcolor;
        if(m_Modulate2X){
            p->m_Color = _ModDWORD2XRGB( p->m_Color , C1 );
        }else{
            p->m_Color = _ModDWORDRGB( p->m_Color , C1 );
        }
        p = p->m_pNext;
    }
} // PNatcolor::Process
/*****************************************************************************/
/*    PFluctuate implemetation
/*****************************************************************************/
PFluctuate::PFluctuate()
{
    m_Direction = Vector3D::oX;
    m_Variation = 5.0f;
}

void PFluctuate::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PFluctuate", this );
    pm.f( "DirX", m_Direction.x );
    pm.f( "DirY", m_Direction.y );
    pm.f( "DirZ", m_Direction.z );
    pm.f( "Variation", m_Variation );
} // PFluctuate::Expose

void PFluctuate::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Direction << m_Variation;
}

void PFluctuate::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Direction >> m_Variation;
} // PFluctuate::Unserialize

void PFluctuate::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        //  TODO: make it to be undependent of the framerate
        Vector3D f( m_Direction );
        f.x += rndValuef( -m_Variation, m_Variation );
        f.y += rndValuef( -m_Variation, m_Variation );
        f.z += rndValuef( -m_Variation, m_Variation );
        f *= dt;
        p->m_Velocity += f;
        p = p->m_pNext;
    }
} // PFluctuate::Process

/*****************************************************************************/
/*    PSizeRamp implemetation
/*****************************************************************************/
PSizeRamp::PSizeRamp()
{
    m_MinSize = 0.0f;
    m_MaxSize = 100.0f;
    m_Repeat  = 1;
    m_Axis      = aXYZ; 
}

void PSizeRamp::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PSizeRamp", this );
    pm.f( "MinSize",    m_MinSize );
    pm.f( "MaxSize",    m_MaxSize );
    pm.f( "Axis",        m_Axis );
    pm.f( "Repeat",        m_Repeat );
    pm.p( "NumKeys", &PSizeRamp::GetNKeys );
    pm.p( "Ramp", &PSizeRamp::GetSizeRamp, &PSizeRamp::SetSizeRamp );
} // PSizeRamp::Expose

void PSizeRamp::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    m_Ramp.Serialize( os );
    os << m_MinSize << m_MaxSize << m_Repeat << Enum2Byte( m_Axis );
}

void PSizeRamp::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    m_Ramp.Unserialize( is );
    is >> m_MinSize >> m_MaxSize >> m_Repeat >> Enum2Byte( m_Axis );
} // PSizeRamp::Unserialize

void PSizeRamp::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        float t = m_Repeat * p->m_Age / p->m_TimeToLive;
        t -= floorf( t );
        float sz = m_MinSize +  m_Ramp.GetAlpha( t )*(m_MaxSize - m_MinSize);
        if (m_Axis&1) p->m_Size.x = sz;
        if (m_Axis&2) p->m_Size.y = sz;
        if (m_Axis&4) p->m_Size.z = sz;
        p = p->m_pNext;
    }
} // PSizeRamp::Process

/*****************************************************************************/
/*    PSizeFade implemetation
/*****************************************************************************/
PSizeFade::PSizeFade()
{
    m_bAbsolute  = true;
    m_StartSize  = 100.0f;
    m_EndSize     = 200.0f;
}

void PSizeFade::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PSizeFade", this );
    pm.f( "StartSize", m_StartSize );
    pm.f( "EndSize", m_EndSize );
    pm.f( "Absolute", m_bAbsolute );
} // PSizeFade::Expose

void PSizeFade::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_StartSize << m_EndSize << m_bAbsolute;
}

void PSizeFade::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_StartSize >> m_EndSize >> m_bAbsolute;
} // PSizeFade::Unserialize

void PSizeFade::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        if (m_bAbsolute)
        {
            float sz = m_StartSize + (m_EndSize - m_StartSize)*p->m_Age/p->m_TimeToLive;

            p->m_Size.x = sz;
            p->m_Size.y = sz;
            p->m_Size.z = sz;
        }
        else
        {
            float sz = (m_EndSize - m_StartSize)*dt/p->m_TimeToLive;

            p->m_Size.x += sz;
            p->m_Size.y += sz;
            p->m_Size.z += sz;
        }
        p = p->m_pNext;
    }
} // PSizeFade::Process

/*****************************************************************************/
/*    PFrame implemetation
/*****************************************************************************/
PFrame::PFrame()
{
    m_Rate    = 10.0f;
    m_DRate    = 0.0f;
    m_NRows    = 4;
    m_NCols = 4;
    m_bUV2 = false;
}

void PFrame::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PFrame", this );
    pm.f( "SecondChannel", m_bUV2 );
    pm.f( "NCols", m_NCols );
    pm.f( "NRows", m_NRows );
    pm.f( "Rate",  m_Rate  );
    pm.f( "dRate", m_DRate );
} // PFrame::Expose

void PFrame::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Rate << m_DRate << m_NRows << m_NCols << m_bUV2;
}

void PFrame::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Rate >> m_DRate >> m_NRows >> m_NCols >> m_bUV2;
} // PFrame::Unserialize

void PFrame::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    int nF = m_NRows*m_NCols;
    float w = 1.0f / m_NCols;
    float h = 1.0f / m_NRows;

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        p->m_FrameTime -= dt; 
        if (p->m_FrameTime <= 0.0f) 
        {
            //  assign next frame time
            float rdt = rndValuef( m_Rate - m_DRate, m_Rate + m_DRate );
            if (rdt < c_Epsilon) rdt = c_Epsilon;
            rdt = 1.0f / rdt;
            p->m_FrameTime += rdt;
            p->m_Frame++;
            //  last frame reached
            if (p->m_Frame >= nF)
            {
                p->m_Frame = 0;
            }

            //  assign uv
            int row = p->m_Frame / m_NCols;
            int col = p->m_Frame % m_NCols;

            if (m_bUV2)
            {
                p->m_UV2.x = float( col )*w;
                p->m_UV2.y = float( row )*h;
                p->m_UV2.w = w;
                p->m_UV2.h = h;
            }
            else
            {
                p->m_UV.x = float( col )*w;
                p->m_UV.y = float( row )*h;
                p->m_UV.w = w;
                p->m_UV.h = h;
            }
        }
        p = p->m_pNext;
    }
} // PFrame::Process

/*****************************************************************************/
/*    PUVMove implemetation
/*****************************************************************************/
PUVMove::PUVMove()
{
    m_UScroll = 10.0f;
    m_VScroll = 0.0f;
    m_UTile   = 1.0f;
    m_VTile   = 1.0f;
    m_bUV2 = false;
}

void PUVMove::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PUVMove", this );
    pm.f( "SecondChannel", m_bUV2 );
    pm.f( "UScroll", m_UScroll );
    pm.f( "VScroll", m_VScroll );
    pm.f( "UTile", m_UTile );
    pm.f( "VTile", m_VTile );
} // PUVMove::Expose

void PUVMove::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_UScroll << m_VScroll << m_bUV2 << m_UTile << m_VTile;
}

void PUVMove::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_UScroll >> m_VScroll >> m_bUV2 >> m_UTile >> m_VTile;
} // PUVMove::Unserialize

void PUVMove::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    float du = m_UScroll*dt*0.01f;
    float dv = m_VScroll*dt*0.01f;
    float UScale = 1.0f / float( m_UTile );
    float VScale = 1.0f / float( m_VTile );

    PParticle* p = pEmitter->m_pParticle;

    if (m_bUV2)
    {
        while (p)
        {
            p->m_UV2.x += du;
            p->m_UV2.y += dv;
            p->m_UV2.w = UScale;
            p->m_UV2.h = VScale;
            p = p->m_pNext;
        }
    }
    else
    {
        while (p)
        {
            p->m_UV.x += du;
            p->m_UV.y += dv;
            p->m_UV.w = UScale;
            p->m_UV.h = VScale;
            p = p->m_pNext;
        }
    }
} // PUVMove::Process

/*****************************************************************************/
/*    PDrag implemetation
/*****************************************************************************/
PDrag::PDrag()
{
    m_Density            = 0.001f;            
    m_Viscosity            = 1.8e-5f;        
    m_bOverrideRadius    = 1;    
    m_ParticleRadius    = 1.0f;    
    m_A                    = 6.0f * c_PI * m_Viscosity;
    m_B                    = 0.2f * c_PI * m_Density;
    SetDensity  ( m_Density );
    SetViscosity( m_Viscosity );
} // PDrag::PDrag

void PDrag::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Density << m_Viscosity << m_bOverrideRadius << m_ParticleRadius;
} // PDrag::Serialize

void PDrag::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Density >> m_Viscosity >> m_bOverrideRadius >> m_ParticleRadius;
    SetDensity( m_Density );
    SetViscosity( m_Viscosity );
} // PDrag::Unserialize

void PDrag::SetDensity( float val )
{
    m_Density = val;
    m_B = 0.2f * c_PI * m_Density;
} // PDrag::SetDensity

void PDrag::SetViscosity( float val )
{
    m_Viscosity = val;
    m_A = 6.0f * c_PI * m_Viscosity;
} // PDrag::SetViscosity

void PDrag::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();

    //  HACK: do not allow big time deltas
    const float c_DragTimeBias = 0.05f;
    if (dt > c_DragTimeBias) dt = c_DragTimeBias;

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        Vector3D v = p->m_Velocity;
        float nv = v.normalize();
        float radius = m_bOverrideRadius ? m_ParticleRadius : p->m_Size.norm();

        float R = m_A * radius * nv + m_B * radius * radius * nv * nv;
        float dv = R * dt;
        clamp( dv, 0.0f, nv );
        v *= -dv;
        p->m_Velocity += v;
        p = p->m_pNext;
    }
} // PDrag::Process

void PDrag::SetToAir()
{
    SetViscosity( 1.8e-5f    );
    SetDensity    ( 1.2929f    );
} // PDrag::SetToAir

void PDrag::SetToWater()
{
    SetViscosity( 1.002e-3f    );
    SetDensity    ( 1.0f        );
} // PDrag::SetToWater

void PDrag::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PDrag", this );
    pm.p( "FluidDensity", &PDrag::GetDensity, &PDrag::SetDensity     );
    pm.p( "FluidViscosity", &PDrag::GetViscosity, &PDrag::SetViscosity );
    pm.f( "OverrideRadius",    m_bOverrideRadius    );
    pm.f( "ParticleRadius",    m_ParticleRadius    );
    pm.m( "SetToAir", &PDrag::SetToAir            );
    pm.m( "SetToWater", &PDrag::SetToWater            );
} // PDrag::Expose

/*****************************************************************************/
/*    PVortex implemetation
/*****************************************************************************/
PVortex::PVortex()
{
    m_Intensity            = 100.0f;
    m_CenterAttraction    = 100.0f;
}

void PVortex::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PVortex", this );
    pm.f( "Intensity", m_Intensity );
    pm.f( "CenterAttraction", m_CenterAttraction );
} // PVortex::Expose

void PVortex::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Intensity << m_CenterAttraction;
} // PVortex::Serialize

void PVortex::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Intensity >> m_CenterAttraction;
} // PVortex::Unserialize

void PVortex::Process( PEmitterInstance* pEmitter )
{
    float dt = pEmitter->GetTimeDelta();
    float intensity  = m_Intensity * dt;
    float centerAttr = m_CenterAttraction * dt;

    Vector3D center( 0, 0, 0 );
    Vector3D dir   ( 0, 0, 1 );

    const float c_DeadZone = 0.1f;

    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        Vector3D diff( p->m_Position );
        diff -= center;
        float pr = diff.dot( dir );
        if (pr > c_DeadZone)
        {
            diff = center; diff.addWeighted( dir, pr );
            Vector3D vr( p->m_Position );
            vr -= diff;
            Vector3D accDir; accDir.cross( vr, dir );
            if (accDir.norm2() > c_DeadZone*c_DeadZone)
            {
                accDir *= intensity;
                accDir.addWeighted( vr, -centerAttr );
                p->m_Velocity += accDir;
            }
        }

        p = p->m_pNext;
    }
} // PVortex::Process

/*****************************************************************************/
/*    POrbit implemetation
/*****************************************************************************/
POrbit::POrbit()
{
    m_Center      = Vector3D::null; 
    m_Axis        = Vector3D::oZ; 
    m_Velocity    = 1.0f;
}

void POrbit::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "POrbit", this );
    pm.f( "CenterX",    m_Center.x    );
    pm.f( "CenterY",    m_Center.y    );
    pm.f( "CenterZ",    m_Center.z    );
    
    pm.f( "Velocity",   m_Velocity    );

    pm.f( "AxisX",      m_Axis.x      );
    pm.f( "AxisY",      m_Axis.y      );
    pm.f( "AxisZ",      m_Axis.z      );
    
} // POrbit::Expose

void POrbit::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Center << m_Axis << m_Velocity;
} // POrbit::Serialize

void POrbit::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Center >> m_Axis >> m_Velocity;
} // POrbit::Unserialize

void POrbit::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;    
    
    float dt = pEmitter->GetTimeDelta();
    Matrix4D tm;
    tm.rotation( m_Axis, dt*m_Velocity );

    Vector3D vc = m_Center;
    if(pEmitter->m_pEmitter->IsWorldSpace()){
        pEmitter->m_WorldTM.transformPt( vc );
    }

    while (p)
    {
        tm.e30 = vc.x - vc.x*tm.e00 - vc.y*tm.e10 - vc.z*tm.e20; 
        tm.e31 = vc.y - vc.x*tm.e01 - vc.y*tm.e11 - vc.z*tm.e21;
        tm.e32 = vc.z - vc.x*tm.e02 - vc.y*tm.e12 - vc.z*tm.e22;
        tm.transformPt( p->m_Position );
        p = p->m_pNext;
    }
} // POrbit::Process

/*****************************************************************************/
/*    PRevolve implemetation
/*****************************************************************************/
PRevolve::PRevolve()
{
    m_Center      = Vector3D::null; 
    m_Axis        = Vector3D::oZ; 
    m_Velocity    = 1.0f;
}

void PRevolve::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PRevolve", this );
    pm.f( "CenterX",    m_Center.x    );
    pm.f( "CenterY",    m_Center.y    );
    pm.f( "CenterZ",    m_Center.z    );
    
    pm.f( "Velocity",   m_Velocity    );

    pm.f( "AxisX",      m_Axis.x      );
    pm.f( "AxisY",      m_Axis.y      );
    pm.f( "AxisZ",      m_Axis.z      );
    
} // PRevolve::Expose

void PRevolve::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Center << m_Axis << m_Velocity;
} // PRevolve::Serialize

void PRevolve::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Center >> m_Axis >> m_Velocity;
} // PRevolve::Unserialize

void PRevolve::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;    
    
    float dt = pEmitter->GetTimeDelta();
    Matrix3D tm;
    tm.rotation( m_Axis, dt*m_Velocity );

    while (p)
    {
        p->m_Rotation *= tm;
        p = p->m_pNext;
    }
} // PRevolve::Process

/*****************************************************************************/
/*    PClampVelocity implemetation
/*****************************************************************************/
PClampVelocity::PClampVelocity()
{
    m_MinVel = 0.0f;
    m_MaxVel = 1.0f;
}

void PClampVelocity::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PClampVelocity", this );
} // PClampVelocity::Expose

void PClampVelocity::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
} // PClampVelocity::Serialize

void PClampVelocity::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
} // PClampVelocity::Unserialize

void PClampVelocity::Process( PEmitterInstance* pEmitter )
{
    assert( false );
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        p = p->m_pNext;
    }
} // PClampVelocity::Process

/*****************************************************************************/
/*    PTurbulence implemetation
/*****************************************************************************/
PTurbulence::PTurbulence()
{
    m_Magnitude = 1.0f;
    m_Frequency = 1.0f;
    m_Phase = Vector3D( -1.0f, 0.0f, 1.0f );
} // PTurbulence::PTurbulence

void PTurbulence::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PTurbulence", this );
    pm.f( "Magnitude",  m_Magnitude );
    pm.f( "Frequency",  m_Frequency );
    pm.f( "PhaseX",     m_Phase.x );
    pm.f( "PhaseY",     m_Phase.y );
    pm.f( "PhaseZ",     m_Phase.z );
} // PTurbulence::Expose

void PTurbulence::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
} // PTurbulence::Serialize

void PTurbulence::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
} // PTurbulence::Unserialize

void PTurbulence::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        Vector3D mpos = p->m_Position;
        mpos *= m_Frequency;
        Vector3D dv;
        dv.x = PerlinNoise( mpos.x + m_Phase.x, mpos.y + m_Phase.x, mpos.z + m_Phase.x );
        dv.y = PerlinNoise( mpos.x + m_Phase.y, mpos.y + m_Phase.y, mpos.z + m_Phase.y );
        dv.z = PerlinNoise( mpos.x + m_Phase.z, mpos.y + m_Phase.z, mpos.z + m_Phase.z );
        dv *= m_Magnitude;
        p->m_Velocity += dv;
        p = p->m_pNext;
    }
} // PTurbulence::Process

/*****************************************************************************/
/*    PTarget implemetation
/*****************************************************************************/
PTarget::PTarget()
{
    m_TargetPos        = Vector3D( 0, 0, 200 ); 
    m_TargetDir        = Vector3D( 0, 0, -1 ); 
    m_TargetSource    = tsFixedTarget;
} // PTarget::PTarget

void PTarget::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PTarget", this );
    pm.f( "TargetSource", m_TargetSource );
    pm.f( "TargetX", m_TargetPos.x );
    pm.f( "TargetY", m_TargetPos.y );
    pm.f( "TargetZ", m_TargetPos.z );
} // PTarget::Expose

void PTarget::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << Enum2Byte( m_TargetSource ) << m_TargetPos << m_TargetDir;
}

void PTarget::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> Enum2Byte( m_TargetSource ) >> m_TargetPos >> m_TargetDir;
}

Vector3D PTarget::GetTargetPos( PEmitterInstance* pEmitter ) const
{
    if (m_TargetSource == tsFixedTarget)
    {
        Vector3D dst = m_TargetPos;
        pEmitter->m_WorldTM.transformPt( dst );
        return dst;
    }
    else if (m_TargetSource == tsRootTarget)
    { 
        return pEmitter->m_Target;
    }
    else if (m_TargetSource == tsChildParticle)
    {
        
    }
    return Vector3D( 0.0f, 0.0f, 300.0f );
} // PTarget::GetTargetPos

/*****************************************************************************/
/*    PRetarget implemetation
/*****************************************************************************/
PRetarget::PRetarget()
{
}

void PRetarget::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    if (!p) return;
    //IEffMgr->SetTarget( pEmitter->m_ParentID, p->m_Position ); 
} // PRetarget::Process

/*****************************************************************************/
/*    PLightning implemetation
/*****************************************************************************/
PLightning::PLightning()
{
    m_Spread    = 20.0f;    
    m_FrameRate    = 20.0f;
    m_Wildness    = 2.0f;
} // PLightning::PLightning

void PLightning::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PLightning", this );
    pm.f( "Spread",        m_Spread    );
    pm.f( "FrameRate",    m_FrameRate );
    pm.f( "Wildness",    m_Wildness    );
} // PLightning::Expose

void PLightning::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Spread << m_FrameRate << m_Wildness;
}

void PLightning::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_Spread >> m_FrameRate >> m_Wildness;
}

void PLightning::Process( PEmitterInstance* pEmitter )
{
    Vector3D org = pEmitter->m_WorldTM.getTranslation();
    Vector3D dst = pEmitter->m_Target;

    if( dst.x==0 && dst.y==0 && dst.z==300.0f && IEffMgr->GetSelectionState()==false )return;

    PParticle* pr = pEmitter->m_pParticle;
    PParticle* pl = pr; 
    if (!pr) return;
    int nP = 1;
    while( true )
    {
        if (!pr->m_pNext) break;
        nP++;
        pr = pr->m_pNext;
    }

    int seed = (DWORD)pEmitter + (pEmitter->m_CurTime - pEmitter->m_StartTime)*m_FrameRate;
    rndInit( seed );
    pl->m_Position = org;
    pr->m_Position = dst;
    m_NPoints = nP;
    Fractalize( pl, pr, nP );
    
    //  postprocess particle positions

} // PLightning::Process

void PLightning::Fractalize( PParticle* pl, PParticle* pr, int nP )
{
    if (pr == pl->m_pNext || pr == pl) return;
    PParticle* pm = pl;
    int mP = nP / 2;
    for (int i = 0; i < mP; i++) pm = pm->m_pNext;
    float fscale = float( nP ) / m_NPoints;
    pm->m_Position.z = (pl->m_Position.z + pr->m_Position.z)*0.5f + rndValuef( -m_Wildness, m_Wildness )*fscale;
    pm->m_Position.x = (pl->m_Position.x + pr->m_Position.x)*0.5f + rndValuef( -m_Spread, m_Spread )*fscale;
    pm->m_Position.y = (pl->m_Position.y + pr->m_Position.y)*0.5f + rndValuef( -m_Spread, m_Spread )*fscale;

    Fractalize( pl, pm, mP );
    Fractalize( pm, pr, nP - mP );
} // PLightning::Fractalize

/*****************************************************************************/
/*    PHoming implemetation
/*****************************************************************************/
PHoming::PHoming()
{
} // PHoming::PHoming

void PHoming::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PHoming", this );
} // PHoming::Expose

void PHoming::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
}

void PHoming::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
}

void PHoming::Process( PEmitterInstance* pEmitter )
{
    PParticle* p = pEmitter->m_pParticle;
    while (p)
    {
        p = p->m_pNext;
    }
} // PHoming::Process

/*****************************************************************************/
/*    PEffectManager implemetation
/*****************************************************************************/
PEffectManager::PEffectManager()
{
    m_ZBiasMultiplier       = 80.0f;
    m_CurrentSubset         = 0;
    m_NParticles            = 0;
    m_LastUsed              = 0;
    m_MinDt                 = 0.0005f;
    m_MaxDt                 = 0.4f;
    m_TimeDelta             = 0.0f;
    m_bPaused               = false;
    m_InstanceFrameTimeout  = c_InstanceFrameTimeout;
    m_bAutoupdate           = false; 
    m_Timeout               = AutoUpdTime;
    m_FadeTime              = AutoFadeTime;
    m_RenderQSize           = 0;
    m_PlayRate              = 1.0f;
    m_NationalColor         = 0xFFFF0000;

    memset( &m_Particle, 0, sizeof( PParticle ) * c_ParticlePoolSize );
    memset( m_ParticleStatus, 0, sizeof( m_ParticleStatus ) ); 
    SetName( "EffectManager" );

    m_bAllowSelection = false;

} // PEffectManager::PEffectManager

void PEffectManager::Reset()
{
    m_NParticles        = 0;
    m_LastUsed            = 0;
    m_bPaused           = false;
    m_RenderQSize       = 0;
    m_PlayRate          = 1.0f;
    m_Emitters.reset();
    memset( &m_Particle, 0, sizeof( PParticle ) * c_ParticlePoolSize );
    memset( m_ParticleStatus, 0, sizeof( DWORD ) * c_ParticlePoolSize ); 
} // PEffectManager::Reset

void PEffectManager::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PEffectManager", this );
    pm.f( "Paused",             m_bPaused               );
    pm.f( "ZBiasMultiplier",    m_ZBiasMultiplier       );
    pm.f( "MinTimeDelta",        m_MinDt                    );
    pm.f( "MaxTimeDelta",        m_MaxDt                    );
    pm.f( "InstanceTimeout",    m_InstanceFrameTimeout  );
    pm.p( "NumEmitterInst", &PEffectManager::GetNEmitterInst            );
    pm.f( "NumParticles",        m_NParticles,        NULL, true );
    pm.f( "LastFreed",            m_LastUsed,        NULL, true );
    pm.p( "PoolSize", &PEffectManager::GetParticlePoolSize );
    pm.m( "Dump", &PEffectManager::Dump );
} // PEffectManager::Expose

DWORD PEffectManager::GetCurEffInstance() const
{
    DWORD idx = m_Emitters.find( GetEntityContext() );
    if (idx == NO_ELEMENT) return NO_ELEMENT;
    const PEmitterInstance& inst = m_Emitters.elem( idx );
    return inst.m_StampedID;
} // PEffectManager::GetCurEffInstance

DWORD PEffectManager::SpawnEmitter( PEmitter* pEmitter, int particleID, 
                                int parentID, bool bAttach, DWORD instID,Matrix4D* StartTM )
{
    if (!pEmitter) return 0xFFFFFFFF;
    int id = m_Emitters.find( GetEntityContext() );
    if (id != NO_ELEMENT) return m_Emitters.elem( id ).m_StampedID;
    if (m_Emitters.numElem() == m_Emitters.poolSize())
    {
        static bool do_once=true;
        if(do_once){
            Log.Warning( "Could not spawn emitter <%s> - too much of them already", IMM->GetModelOfNodeName( pEmitter->GetID() ) );
            static int PreveTime=0;
            if(GetTickCount()-PreveTime>4000){
                //dumping emiters
                FILE* F=fopen("emiters.log","w");
                if(F){
                    for(int i=0;i<m_Emitters.numElem();i++){
                        PEmitterInstance& pem=m_Emitters.elem(i);
                        fprintf(F,"%s\n",IMM->GetModelOfNodeName(pem.m_pEmitter->GetID()));
                    }
                    fclose(F);
                }
                PreveTime=GetTickCount();
            }
            do_once=false;
        }
        return 0xFFFFFFFF;
    }

    if (instID == 0xFFFFFFFF)
    {
        instID = m_Emitters.add( GetEntityContext(), PEmitterInstance() );
    }
    
    PEmitterInstance& inst = m_Emitters.elem( instID );
    inst.Reset();
    inst.m_pEmitter     = pEmitter;
    inst.m_StartTime    = pEmitter->GetStartTime();
    inst.m_TotalTime    = pEmitter->GetTotalTime();
    inst.m_LastFrame    = IRS->GetCurFrame();
    inst.m_ParentID     = parentID;
    inst.m_WorldTM      = Matrix4D::identity;
    inst.m_PrevWorldTM  = Matrix4D::identity;
    inst.m_EmitterTM    = Matrix4D::identity;
    inst.m_Alpha        = 1.0f;
    inst.m_Intensity    = 1.0f;
    inst.m_Target       = Vector3D( 0.0f, 0.0f, 300.0f );
    inst.m_Cycles       = 0;
    inst.m_Timeout      = m_Timeout;
    inst.m_Subset       = m_CurrentSubset;

    inst.SetFlag( ifAutoUpdate, m_bAutoupdate || pEmitter->IsAutoUpdated() );
    inst.SetFlag( ifNeedUpdate );

    //  stamp used to validate emitter instances
    static WORD s_InstanceStamp = 0;
    s_InstanceStamp++;
    
    if (bAttach && particleID != -1)
    {
        inst.m_ParticleID = particleID;
    }

    DWORD stampedID = instID | (((DWORD)s_InstanceStamp) << 16);
    m_Emitters.elem( instID ).m_ID          = instID;
    m_Emitters.elem( instID ).m_StampedID   = stampedID;

	if( StartTM )UpdateInstance(stampedID,*StartTM);

    //  rewind warnup time
    if (pEmitter->m_WarmupTime > 0.0f)
    {
        float t = pEmitter->m_WarmupTime;
        if (pEmitter->IsRandomWarmup()) t = rndValuef( 0.0f, t );
        RewindEffect( stampedID, t );
    }

    return stampedID;
} // PEffectManager::SpawnEmitter

void PEffectManager::Pause( bool bPause )
{
    m_bPaused = bPause;
} // PEffectManager::Pause

DWORD PEffectManager::InstanceEffect( DWORD effID )
{
    PEmitter* pEff = dynamic_cast<PEmitter*>(NodePool::GetNode( effID ));
    if (!pEff || !pEff->IsA<PEmitter>()) return 0xFFFFFFFF;
    DWORD res = SpawnEmitter( pEff );
    return res;
} // PEffectManager::InstanceEffect

DWORD PEffectManager::InstanceEffect( DWORD effID,Matrix4D& TM )
{
    PEmitter* pEff = dynamic_cast<PEmitter*>(NodePool::GetNode( effID ));
    if (!pEff || !pEff->IsA<PEmitter>()) return 0xFFFFFFFF;
    DWORD res = SpawnEmitter( pEff,-1,-1,false,0xFFFFFFFF,&TM );
    return res;
} // PEffectManager::InstanceEffect

void PEffectManager::RewindEffect( DWORD hInst, float amount, float step, bool bRewChildren )
{
    //  FIXME: Temporally disabled
    return;

    if (!IsValid( hInst )) return;
    PEmitterInstance& em = GetEmitterInstance( hInst );
    
    if (amount > em.m_TotalTime) return;

	/*
    //  kill all particles	
    PParticle* pParticle = em.m_pParticle;
    while (pParticle)
    {
        m_ParticleStatus[pParticle->m_ID] = c_AlmostDeadSlot;
        pParticle = pParticle->m_pNext;
    }

    //  destroy all children emitters
    PInstanceHash::iterator it = m_Emitters.begin();
    while (it)
    {
        PEmitterInstance& cem = *it;
        if (cem.m_ParentID == em.m_ID) DestroyInstance( cem.m_StampedID );
        ++it;
    }	

    em.m_pParticle = NULL;

    PEmitter* pEff = em.m_pEmitter;
    em.Reset();
    em.m_LastFrame    = IRS->GetCurFrame();
    //em.m_WorldTM      = Matrix4D::identity;
    //em.m_PrevWorldTM  = Matrix4D::identity;
    //em.m_EmitterTM    = Matrix4D::identity;
    em.m_Alpha        = 1.0f;
    em.m_Intensity    = 1.0f;
    em.m_Target       = Vector3D( 0.0f, 0.0f, 300.0f );
    em.m_Cycles       = 0;
    em.m_Timeout      = m_Timeout;
	*/
    
    DWORD curFrame = IRS->GetCurFrame();
    rndInit( curFrame^GetTickCount() );

    //em.SetFlag( ifNeedDraw, false );
    //SetNeedUpdate( em );

    //  now update who needs that
    for (float t = 0.0f; t < amount; t += step)
    {
        PInstanceHash::iterator uit = m_Emitters.begin();
        while (uit)
        {
            PEmitterInstance& cem = *uit;
            if (cem.GetFlag( ifNeedUpdate ))
            {
                float cTime = cem.m_CurTime;
                PEmitter* pEmitter = cem.m_pEmitter;
                StepEmitter( cem, step );
                cem.m_LastFrame = curFrame;
            }
            ++uit;
        }
    }
} // PEffectManager::RewindEffect

void PEffectManager::SetNeedUpdate( PEmitterInstance& em )
{
    em.SetFlag( ifNeedUpdate );
    PInstanceHash::iterator it = m_Emitters.begin();
    while (it)
    {
        PEmitterInstance& cem = *it;
        if (cem.m_ParentID == em.m_ID) SetNeedUpdate( cem );
        ++it;
    }
} // PEffectManager::SetNeedUpdate

float PEffectManager::GetCurTime( DWORD hInst ) const
{
    if (!IsValid( hInst )) return 0.0f;
    const PEmitterInstance& em = GetEmitterInstance( hInst );
    return em.m_CurTime;
} // PEffectManager::GetCurTime

float PEffectManager::GetTotalTime( DWORD hInst ) const
{
    if (!IsValid( hInst )) return 0.0f;
    const PEmitterInstance& em = GetEmitterInstance( hInst );
    return em.m_pEmitter->GetTotalTime();
} // PEffectManager::GetTotalTime

void PEffectManager::SetAlphaFactor( DWORD hInst, float factor )
{
    if (!IsValid( hInst )) return;
    PEmitterInstance& em = GetEmitterInstance( hInst );
    em.m_Alpha = factor;
} // PEffectManager::SetAlphaFactor

void PEffectManager::SetIntensity( DWORD hInst, float intensity )
{
    if (!IsValid( hInst )) return;
    PEmitterInstance& em = GetEmitterInstance( hInst );
    em.m_Intensity = intensity;
} // PEffectManager::SetIntensity

void PEffectManager::UpdateInstance( DWORD hInst, const Matrix4D& tm )
{
    if (!IsValid( hInst )) return;
    PEmitterInstance& inst = GetEmitterInstance( hInst );
    inst.SetFlag( ifMoved, !inst.m_WorldTM.equal( tm ) );
    inst.m_PrevWorldTM  = inst.m_WorldTM;
    inst.m_WorldTM      = tm;
    inst.SetFlag( ifNeedDraw );
    inst.SetFlag( ifUpdated );
    DWORD C = IEffMgr->GetCurrentNatColor();
    if( C ) inst.m_Natcolor = C;
} // PEffectManager::SetTransform

void PEffectManager::UpdateInstance( const Matrix4D& tm )
{
    UpdateInstance( GetCurEffInstance(), tm );
}

void PEffectManager::DestroyInstance()
{
    DestroyInstance( GetCurEffInstance() );
}

bool PEffectManager::IsValid( DWORD hInst ) const
{
    if (hInst == 0xFFFFFFFF) return false;
    DWORD index = hInst&0x0000FFFF;
    if (index > m_Emitters.maxElem() || m_Emitters.numElem() == 0) return false;
    const PEmitterInstance& em = m_Emitters.elem( index );
    return (em.m_StampedID == hInst);
} // PEffectManager::IsValid

void PEffectManager::DestroyInstance( DWORD hInst )
{
    if (!IsValid( hInst )) return;
    PEmitterInstance& em = GetEmitterInstance( hInst );
    em.SetFlag( ifDestoyed, true );
    m_bSomethingDestroyed = true;
    /*
    //  free all emitter's particles 
    PParticle* p = em.m_pParticle;
    while (p) 
    { 
        p->m_Flags = 0; 
        m_ParticleStatus[p->m_ID] = c_FreeParticleSlot; 
        PParticle* pp = p->m_pNext;         
        p->m_EmitterID = 0;
        p->m_ID = 0;
        p->m_pNext = NULL;
        p=pp;
        m_NParticles--; 
    } 
    
    PInstanceHash::iterator it = m_Emitters.begin();    
    bool candestroy=true;
    
    while (it)
    {
        PEmitterInstance& cem = *it;
        //destroy all references on deleted particles
        if ( cem.m_ParticleID != -1 && m_ParticleStatus[cem.m_ParticleID] == c_FreeParticleSlot ) 
            cem.m_ParticleID = -1;
        if ( cem.m_ParentID == em.m_ID )//destroy children objects if they are not autoupdated
        {
            if( cem.GetFlag( ifAutoUpdate ) ){
                cem.m_ParentID = -1;
                cem.m_Intensity = 0.0f;
            }else DestroyInstance( cem.m_StampedID );            
        }
        ++it;
    }
    
    em.m_Intensity = 0.0f;
    em.m_pParticle = NULL;
    em.m_ParticleID = -1;
    if(!candestroy)return;
    //  delete emitter from the pool
    m_Emitters.delElem( em.m_ID );
    em.m_StampedID = 0xFFFFFFFF;
    */
}// PEffectManager::DestroyInstance

void PEffectManager::ResetInstance( DWORD hInst, bool bResetChildren )
{
    if (!IsValid( hInst )) return;
    PEmitterInstance& em = GetEmitterInstance( hInst );
    em.m_CurTime    = em.m_StartTime;
    em.m_LastTime   = em.m_CurTime - m_TimeDelta;
    em.m_EmitAccum  = em.m_StartTime;
    em.m_Cycles++;

    if (bResetChildren)
    {
        PInstanceHash::iterator it = m_Emitters.begin();
        while (it)
        {
            PEmitterInstance& cem = *it;
            if (cem.m_ParentID == em.m_ID) ResetInstance( cem.m_StampedID, true );
            ++it;
        }
    } 

    //  rewind warnup time
    if (em.m_pEmitter->m_WarmupTime > 0.0f)
    {
        float t = em.m_pEmitter->m_WarmupTime;
        if (em.m_pEmitter->IsRandomWarmup()) t = rndValuef( 0.0f, t );
        RewindEffect( hInst, t );
    }
} // PEffectManager::ResetInstance

void PEffectManager::PreRender()
{
    DWORD curFrame = IRS->GetCurFrame();
    IRS->ResetWorldTM();

    //  render null layer renderables
    //  fill geometry pass
    PInstanceHash::iterator it = m_Emitters.begin();
    while (it)
    {
        PEmitterInstance& em = *it;
        if( em.m_Subset == m_CurrentSubset )
        {
            PEmitter* pEmitter = em.m_pEmitter;

            //  apply render-only operators to particles 
            int nOp = pEmitter->GetNChildren();
            for (int i = 0; i < nOp; i++)
            {
                PRenderer* pR = (PRenderer*)pEmitter->GetChild( i );
			    if(pR){
				    if (pR->IsInvisible()) continue;
				    if (!pR->IsVisual() || !pR->IsNullLayer()) continue;
				    pR->FillGeometry( &em );
			    }
            }    
        }
        ++it;
    }

    //  render geometry pass
    PInstanceHash::iterator rit = m_Emitters.begin();
    while (rit)
    {
        PEmitterInstance& em = *rit;
        if( em.m_Subset == m_CurrentSubset )
        {
            PEmitter* pEmitter = em.m_pEmitter;

            //  apply render-only operators to particles 
            int nOp = pEmitter->GetNChildren();
            for (int i = 0; i < nOp; i++)
            {
                PRenderer* pR = (PRenderer*)pEmitter->GetChild( i );
                if (pR->IsInvisible()) continue;
                if (!pR->IsVisual() || !pR->IsNullLayer()) continue;

                pR->RenderGeometry( &em );
            }    
        }
        ++rit;
    }
} // PEffectManager::PreRender
void PEffectManager::PostEvaluate(){
    PInstanceHash::iterator rit = m_Emitters.begin();
    while (rit)
    {
        PEmitterInstance& em = *rit;
        if( em.m_Subset == m_CurrentSubset )
        {            
            em.SetFlag( ifNeedDraw, false );
            em.SetFlag( ifUpdated,false );
        }
        ++rit;
    }
}
int numUpd=0;
int numnUpd=0;
void PEffectManager::Evaluate( float dt )
{
    //  auxiliary setup
    ICamera* pCam = GetCamera();
    if (pCam)
    {
        m_ScreenPlane.fromPointNormal( pCam->GetPosition(), pCam->GetDir() );
        m_PlaneUpVec = Vector3D::oZ;
        m_ScreenPlane.ProjectVec( m_PlaneUpVec );
        if (m_PlaneUpVec.normalize() < c_Epsilon)
        {
            m_PlaneUpVec = Vector3D::oX;
            m_ScreenPlane.ProjectVec( m_PlaneUpVec );
            if (m_PlaneUpVec.normalize() < c_Epsilon)
            {
                m_PlaneUpVec = Vector3D::oY;
                m_ScreenPlane.ProjectVec( m_PlaneUpVec );
                m_PlaneUpVec.normalize();
            }
        }
        m_PlaneNVec = pCam->GetDir();
    }
    else
    {
        m_ScreenPlane = Plane::xOy;
        m_PlaneUpVec  = Vector3D::oX;
        m_PlaneNVec      = Vector3D::oZ;
    }

    DWORD curFrame = IRS->GetCurFrame();

    //  get time delta
    static Timer s_Timer;
    if (dt == 0.0f) dt = s_Timer.seconds()*2; 
    s_Timer.start();
    clamp( dt, m_MinDt, m_MaxDt );
    m_TimeDelta = dt;

    rndInit( curFrame^GetTickCount() );
    m_TimeDelta *= m_PlayRate;
    if (m_bPaused) m_TimeDelta = 0.0f;

    //  propagate instance attributes
    bool updchanged=false;
    PInstanceHash::iterator it = m_Emitters.begin();
    while (it)
    {
        PEmitterInstance& em = *it;
        if( em.m_Subset == m_CurrentSubset )
        {
            PEmitter* pEmitter = em.m_pEmitter;


            if( em.m_ParentID!=-1 ){
                PEmitterInstance& pem = m_Emitters.elem( em.m_ParentID );
                if (pem.GetFlag( ifUpdated )){
                    if(!em.GetFlag( ifUpdated )){
                        em.SetFlag( ifUpdated );
                        updchanged = true;
                        em.m_Timeout = AutoUpdTime;
                    }
                }
            }

            if (em.m_ParticleID != -1)
            {
                const PParticle& attach = m_Particle[em.m_ParticleID]; 
                PEmitterInstance& pem = m_Emitters.elem( attach.m_EmitterID );
                if( !pem.GetFlag( ifDestoyed ) ){
                    if (pem.GetFlag( ifNeedDraw )) em.SetFlag( ifNeedDraw );
                    em.m_Alpha      = pem.m_Alpha;
                    em.m_Intensity  = pem.m_Intensity;
                    em.m_Target     = pem.m_Target;
                    em.m_Natcolor   = pem.m_Natcolor;
                    /*
                    if (pem.GetFlag( ifUpdated )){
                        if(!em.GetFlag( ifUpdated )){
                            em.SetFlag( ifUpdated );
                            updchanged = true;
                        }
                    }
                    */
                    
                }
            }
            if (em.GetFlag( ifAutoUpdate )){
                em.SetFlag( ifNeedDraw);
            }
        }
        ++it;
    }
    numUpd=0;
    numnUpd=0;
    while( updchanged ){
        updchanged = false;
        PInstanceHash::iterator it = m_Emitters.begin();
        while (it)
        {
            PEmitterInstance& em = *it;
            if( em.m_Subset == m_CurrentSubset )
            {
                PEmitter* pEmitter = em.m_pEmitter;
                if( em.m_ParentID!=-1 ){
                    PEmitterInstance& pem = m_Emitters.elem( em.m_ParentID );
                    if (pem.GetFlag( ifUpdated )){
                        if(!em.GetFlag( ifUpdated )){
                            em.SetFlag( ifUpdated );
                            updchanged = true;
                            em.m_Timeout = AutoUpdTime;
                        }
                    }
                }
                if(em.GetFlag(ifUpdated))numUpd++;
                else numnUpd++;
            }
            ++it;
        }
    }

    //  process all active emitter instances
    it.reset();
    int nEmitters = m_Emitters.numElem();
    int cEmitter = 0;
    while (it)
    {
        PEmitterInstance& em = *it;
        if( em.m_Subset == m_CurrentSubset )
        {
            if (em.GetFlag( ifAutoUpdate )) em.SetFlag( ifNeedDraw );
            StepEmitter( em, m_TimeDelta );        
            if (em.GetFlag( ifNeedDraw )) em.m_LastFrame = curFrame;
        }
        ++it; cEmitter++;
    }

    //  delete emitters that are over
    it.reset();
    while (it)
    {
        PEmitterInstance& em = *it; ++it;
        if( em.m_Subset == m_CurrentSubset )
        {
            PEmitter* pEmitter = em.m_pEmitter;
            //  if instance has not been rendered during number of frames, then instance is over 
            if (curFrame - em.m_LastFrame > m_InstanceFrameTimeout) 
            {
                if(em.GetFlag( ifAutoUpdate ))
                {
                    em.m_Intensity = 0;
                }else DestroyInstance( em.m_StampedID );
                continue;
            }

            //  if instance is being auto-updated, and timeout is reached
            if (em.GetFlag( ifAutoUpdate ) && em.m_Timeout <= 0.0f)
            {
                DestroyInstance( em.m_StampedID );
                continue;
            }
            bool timeover = em.m_CurTime > pEmitter->m_TotalTime;
            bool parentover = em.m_ParticleID != -1 && m_Particle[em.m_ParticleID].GetFlag( pfDead );
            bool autoupd = em.GetFlag( ifAutoUpdate );

            if( parentover && autoupd ) em.m_Intensity = 0.0f;

            if ( timeover ||                      //  time is over
                ( (!autoupd) && parentover ) )    //  parent is over
            {
                if (pEmitter->IsLooped()) 
                {
                    ResetInstance( em.m_StampedID, true );
                }
                else
                {
                    if (em.m_pParticle || pEmitter->IsPlayedForever()) continue;
                    DestroyInstance( em.m_StampedID );                
                }
            }
        }
    }
    // deleting emmiters that should be destroyed
    if( m_CurrentSubset == 0 )
    {
        while( m_bSomethingDestroyed ){
            m_bSomethingDestroyed = false;
            it.reset();
            while (it)
            {
                PEmitterInstance& em = *it; ++it;            
                if( em.m_ParentID != -1 )
                {
                    PEmitterInstance& pem = m_Emitters.elem( em.m_ParentID );
                    if( pem.GetFlag( ifDestoyed ) )
                    {
                        if ( em.m_ParticleID != -1 )
                        {
                            DWORD& st = m_ParticleStatus[em.m_ParticleID];
                            if( st == c_FreeParticleSlot && st == c_AlmostDeadSlot ){
                                em.m_ParticleID = -1;
                                st = c_FreeParticleSlot;
                            }
                        }
                        //if ( em.m_ParentID == em.m_ID )//destroy children objects if they are not autoupdated
                        //{
                            if( em.GetFlag( ifAutoUpdate ) )
                            {
                                em.m_ParentID = -1;
                                em.m_Intensity = 0.0f;
                            }
                            else DestroyInstance( em.m_StampedID );            
                        //}
                    }
                }
                if( em.GetFlag( ifDestoyed ) )
                {
                    PParticle* p = em.m_pParticle;
                    while (p) 
                    { 
                        p->m_Flags = 0; 
                        m_ParticleStatus[p->m_ID] = c_FreeParticleSlot; 
                        PParticle* pp = p->m_pNext;         
                        p->m_EmitterID = 0;
                        p->m_ID = 0;
                        p->m_pNext = NULL;
                        p=pp;
                        m_NParticles--; 
                    }                     
                    em.m_Intensity = 0.0f;
                    em.m_pParticle = NULL;
                    em.m_ParticleID = -1;
                    //if(!candestroy)return;
                    //  delete emitter from the pool
                    m_Emitters.delElem( em.m_ID );
                    em.m_StampedID = 0xFFFFFFFF;                
                }
            }
        }    
        for(int i=0;i<c_ParticlePoolSize;i++){
            DWORD& st = m_ParticleStatus[i];
		    if( st == c_AlmostDeadSlot ){
			    st = c_FreeParticleSlot;
			    m_NParticles--;
		    }
        }
    }
} // PEffectManager::Evaluate

void PEffectManager::PostRender()
{
    static DWORD curFrame = IRS->GetCurFrame() - 1;
    curFrame = IRS->GetCurFrame();
    IRS->ResetWorldTM();

    //  render top layer renderables
    //  fill geometry pass
    PInstanceHash::iterator it = m_Emitters.begin();
    while (it)
    {        
        PEmitterInstance& em = *it;
        if( em.m_Subset == m_CurrentSubset )
        {
            PEmitter* pEmitter = em.m_pEmitter;

            //  apply render-only operators to particles 
            int nOp = pEmitter->GetNChildren();
            for (int i = 0; i < nOp; i++)
            {
                PRenderer* pR = (PRenderer*)pEmitter->GetChild( i );
                if (pR->IsInvisible()) continue;
                if (!pR->IsVisual() || pR->IsNullLayer()) continue;

                pR->FillGeometry( &em );
            }    
        }
        ++it;
    }

    //  render geometry pass
    PInstanceHash::iterator rit = m_Emitters.begin();
    while (rit)
    {
        PEmitterInstance& em = *rit;
        if( em.m_Subset == m_CurrentSubset )
        {
            PEmitter* pEmitter = em.m_pEmitter;

            //  apply render-only operators to particles 
            int nOp = pEmitter->GetNChildren();
            for (int i = 0; i < nOp; i++)
            {
                PRenderer* pR = (PRenderer*)pEmitter->GetChild( i );
                if (pR->IsInvisible()) continue;
                if (!pR->IsVisual() || pR->IsNullLayer()) continue;

                pR->RenderGeometry( &em );
            }                
        }
        ++rit;
    }
} // PEffectManager::PostRender

void PEffectManager::Render()
{
    return;
    Evaluate();
    PreRender();
    PostRender();
} // PEffectManager::Render

bool PEffectManager::EmitParticle( PEmitterInstance& em, const Vector3D& position )
{
    if (m_NParticles == c_ParticlePoolSize) return false;
    
    //    allocate particle slot
    int pID = (++m_LastUsed) & (c_ParticlePoolSize-1);
	m_LastUsed &= c_ParticlePoolSize-1;

    for (; pID < c_ParticlePoolSize; pID++)
    {
        if (m_ParticleStatus[pID] == c_FreeParticleSlot) break;
    }
    //  reached the end of the pool
    if (pID == c_ParticlePoolSize)
    {
        for (pID = 0; pID < c_ParticlePoolSize; pID++)
        {
            if (m_ParticleStatus[pID] == c_FreeParticleSlot) break;
        }
    }
    assert( m_ParticleStatus[pID] == c_FreeParticleSlot );

    m_NParticles++;
    m_ParticleStatus[pID] = c_UsedParticleSlot;
    m_LastUsed = pID;

    PParticle* tail = em.m_pParticle;
    PParticle& p = m_Particle[pID];
    //  initialize allocated particle
    p.m_Flags       = 0;
    p.m_Age         = 0.0f;
    p.m_AgeOfLastUpd= 0.0f;
    p.m_ID          = pID;
    p.m_pNext       = tail;
    p.m_pPrev       = NULL;
    p.m_Frame       = 0;
    p.m_FrameTime   = 0.0f;
    p.m_AngVelocity = Vector3D::null;
    p.m_Velocity    = Vector3D::null;
    p.m_Rotation    = Matrix3D::identity;
    p.m_Size        = Vector3D( 20, 20, 20 );
    p.m_Color       = 0xFFFFFFFF;
    p.m_EmitterID   = em.m_ID;
    p.m_TimeToLive  = em.m_pEmitter->GetTimeToLive( p );
    p.m_UV          = Rct::unit;
    p.m_UV2         = Rct::unit;
    p.m_Roll        = 0.0f;
    p.m_Position    = position;

    if (&p == p.m_pNext)
    {
        Log.Error( "Impossible loop in the particle system." );
        return false;
    }
    
    p.m_PrevPosition = p.m_Position;
    if (p.m_TimeToLive <= 0.0f) p.SetFlag( pfImmortal );
    p.SetFlag( pfJustBorn );

    //  add allocated particles to the current emitter's chain
    if (tail) tail->m_pPrev = &p;
    em.m_pParticle = &p;
    return true;
} // PEffectManager::EmitParticle

Matrix4D PEffectManager::GetParticleFullWorldTM( const PParticle& p )
{
    PEffectManager* pe=(PEffectManager*)IEffMgr;
    Vector3D vz( p.m_Velocity );
    float v = vz.normalize();
    if (v < c_Epsilon) vz = Vector3D::oZ;
    Vector3D vy, vx( Vector3D::oX );
    vy.cross( vz, vx );
    vy.normalize();
    vx.cross( vy, vz );
    Matrix4D tm( vx, vy, vz, p.m_Position );
    Matrix4D tm2( p.m_Size , p.m_Rotation ,Vector3D::null );
    tm.mulLeft(tm2);
    const PEmitterInstance& em = pe->m_Emitters.elem( p.m_EmitterID ); 
    if (em.m_pEmitter->IsWorldSpace()) return tm;
    tm *= em.m_WorldTM;
    return tm;
} // PEffectManager::GetParticleWorldTM
Matrix4D PEffectManager::GetParticleWorldTM( const PParticle& p )
{
    PEffectManager* pe=(PEffectManager*)IEffMgr;
    Vector3D vz( p.m_Velocity );
    float v = vz.normalize();
    if (v < c_Epsilon) vz = Vector3D::oZ;
    Vector3D vy, vx( Vector3D::oX );
    vy.cross( vz, vx );
    vy.normalize();
    vx.cross( vy, vz );
    Matrix4D tm( vx, vy, vz, p.m_Position );    

    const PEmitterInstance& em = pe->m_Emitters.elem( p.m_EmitterID ); 
    if (em.m_pEmitter->IsWorldSpace()) return tm;
    tm *= em.m_WorldTM;
    return tm;
}
void PEffectManager::StepEmitter( PEmitterInstance& em, float dt )
{
    PEmitter* pEmitter = em.m_pEmitter;
    if (!pEmitter || pEmitter->IsInvisible()) return;

    //  propagate emitter attributes
    if (em.m_ParentID != -1)
    {
        PEmitterInstance& pem = m_Emitters.elem( em.m_ParentID );
        if (pem.GetFlag( ifNeedDraw )) em.SetFlag( ifNeedDraw );
        em.m_Alpha      = pem.m_Alpha;
        em.m_Intensity  = pem.m_Intensity;
        em.m_Target     = pem.m_Target;
        //if(pem.m_WorldTM.e30!=0 || pem.m_WorldTM.e31!=0 || pem.m_WorldTM.e32!=0)
        //    em.m_WorldTM    = pem.m_WorldTM;
    }
    
    bool ShouldDie=false;    
    if( em.GetFlag( ifAutoUpdate ) ){
        if( em.GetFlag( ifUpdated ) ){
            em.m_Timeout = AutoUpdTime;
        }
        if(em.m_Timeout < m_FadeTime)
        {
            em.m_Alpha = em.m_Timeout/m_FadeTime;
            em.m_Alpha *= em.m_Alpha;
            if(em.m_Timeout < m_FadeTime)
            {
                em.m_Intensity = 0;
                em.m_ParticleID = -1;
                ShouldDie = true;
            }
        }else em.m_Alpha=1.0f;
        //if(em.m_Timeout<0)em.m_Alpha = 0;
    }

    //  update emitter transform
    if ( em.m_ParticleID > 0 && m_ParticleStatus[ em.m_ParticleID ] == c_UsedParticleSlot )
    { 
        const PParticle& attach = m_Particle[em.m_ParticleID]; 
        if( attach.m_EmitterID == em.m_ParentID ){
            em.m_WorldTM = GetParticleWorldTM( attach );
        }
    }

    //  emit new particles
    if(em.m_Intensity>0 && !ShouldDie){
        int nEmit = pEmitter->NumToEmit( &em );
        Vector3D pos ( Vector3D::null );
        Vector3D dPos( Vector3D::null );
        if (em.m_pEmitter->IsWorldSpace() && nEmit > 0)
        {
            if(em.m_CurTime>0){
                dPos = em.m_WorldTM.getTranslation();
                dPos -= em.m_PrevWorldTM.getTranslation(); 
                pos = em.m_WorldTM.getTranslation();
                dPos *= -1.0f/float( nEmit );
            }else{
                pos = em.m_WorldTM.getTranslation();
            }
        }
        for (int j = 0; j < nEmit; j++) 
        {
            EmitParticle( em, pos );
            pos += dPos;
        }
    }

    //  apply non render-only operators to particles 
    int nOp = pEmitter->GetNChildren();
    int nv=0;
    for (int i = 0; i < nOp; i++)
    {
        PRenderer* pR = (PRenderer*)pEmitter->GetChild( i );
        if (pR->IsInvisible()) continue;
        if (pR->IsVisual())
        {
            nv++;
            continue;
        }
        pR->Process( &em );        
    }    
            
    // update common particle attributes
    PParticle* p = em.m_pParticle;
    while (p)
    {
        p->m_PrevPosition = p->m_Position;
        p->m_Position.addWeighted( p->m_Velocity, dt );
        p->m_Age += dt;
        if( em.GetFlag( ifUpdated )){
            p->m_AgeOfLastUpd = p->m_Age;
            em.m_Timeout = AutoUpdTime;

        }
        p->SetFlag( pfJustBorn, false );

        //if (ShouldDie)
        //{
        //    DWORD A=p->m_Color>>24;
        //    DWORD C=p->m_Color&0xFFFFFF;
        //    A*=em.m_Alpha;
        //    p->m_Color=(A<<24)+C;
        //}
    
        p->m_Roll  += p->m_AngVelocity.x * dt;
    
        //  if particle is dead, remove it from the cluster
        if (p->GetFlag( pfDead ))
        {
            p->m_Flags = 0;
            PParticle* prev = p->m_pPrev;
            PParticle* next = p->m_pNext;
    
            if (prev) prev->m_pNext = next; else em.m_pParticle = next;
            if (next) next->m_pPrev = prev;
            m_ParticleStatus[p->m_ID] = c_AlmostDeadSlot;//c_FreeParticleSlot;
            p->m_pNext = NULL;
            p->m_pPrev = NULL;
            p = next;
            //m_NParticles--;
            assert( m_NParticles >= 0 );
            continue;
        }
        
        //  particle is about to die
        if ( ( p->m_Age >= p->m_TimeToLive /*|| ( nv==0 && p->m_Age>1.0 )*/ ) && !p->GetFlag( pfImmortal ) )
        {
            p->SetFlag( pfDead, true );
        }
    
        p = p->m_pNext;
    }
    
    //  update emitter instance parameters
    em.m_LastTime = em.m_CurTime;
    em.m_PrevWorldTM = em.m_WorldTM;
    em.m_CurTime  += dt;
    em.m_Timeout  -= dt;
} // PEffectManager::StepEmitter

void PEffectManager::Dump()
{
    DumpToFile( "c:\\dumps\\effmgr.txt" );
}

void PEffectManager::DumpToFile( const char* fname )
{
    FILE* fp = fopen( fname, "wt" );
    if (!fp) return;
    
    fprintf( fp, "Effect manager dump.\nNEmitters=%d\nNParticles=%d\nLastFreed=%d\n", 
                    m_Emitters.numElem(), m_NParticles, m_LastUsed );
    for (int i = 0; i < c_ParticlePoolSize; i++)
    {
        PParticle& p = m_Particle[i];
        if (!p.GetFlag( pfUsedSlot )) continue;
        fprintf( fp, "id:%d\t\t", p.m_ID );
        if (p.GetFlag( pfEmitter    )) fprintf( fp, " emitter "  );
        if (p.GetFlag( pfDead        )) fprintf( fp, " dead "     );
        if (p.GetFlag( pfImmortal    )) fprintf( fp, " immortal " );
        if (p.m_pPrev) fprintf( fp, " prev: %d", p.m_pPrev->m_ID );
        if (p.m_pNext) fprintf( fp, " next: %d", p.m_pNext->m_ID );
        fprintf( fp, "\n" );
    }

    for (int i = 0; i < m_Emitters.numElem(); i++)
    {
        PEmitterInstance& em = m_Emitters.elem( i );
        fprintf( fp, "em:%d\t\t", i );
        fprintf( fp, "\n" );
    }
    fclose( fp );
} // PEffectManager::DumpToFile

int PEffectManager::GetEffectSetID( const char* fileName )
{
    int id = m_EffectSet.size();
    if (!fileName) return - 1;
    FInStream is( fileName );
    if (is.NoFile()) return -1;
    XMLNode root( is );
    int nEff = root.GetNChildren();
    XMLNode* pChild = root.FirstChild();
    EffectSet effSet;
    for (int i = 0; i < nEff; i++)
    {
        PEffect* pEff = new PEffect();
        pEff->FromXML( pChild );
        effSet.m_Effect.push_back( pEff );
        pChild = pChild->NextSibling();
    }
    effSet.m_File = fileName;
    m_EffectSet.push_back( effSet );
    return id;
} // PEffectManager::GetEffectSetID

int PEffectManager::GetNEffects( int setID )
{
    if (setID < 0 || setID >= m_EffectSet.size()) return 0;
    return m_EffectSet[setID].m_Effect.size();
} // PEffectManager::GetNEffects

bool PEffectManager::BindEffectSet( int setID, DWORD modelID )
{
    SNode* pModel = NodePool::GetNode( modelID );
    if (!pModel) return false;
    int nEff = GetNEffects( setID );
    for (int i = 0; i < nEff; i++)
    {
        PEffect* pEff = GetEffectFromSet( setID, i );
        if (!pEff) return false;
        pEff->SetInvisible( !pEff->IsDefaultVisible() );
        SNode* pBone = pModel->FindChild<TransformNode>( pEff->GetParentBoneName() );
        if (!pBone) pBone = pModel->FindChild<Group>( pEff->GetParentBoneName() );
        if (pBone) pBone->AddChild( pEff );
    }
    return false;
} // PEffectManager::BindEffectSet

bool PEffectManager::UpdateInstance( int setID, int effID, const Matrix4D& tm )
{
    PEffect* pEff = GetEffectFromSet( setID, effID );
    if (!pEff) return false;
    TransformNode::Push( tm );
    SNode* pParent = pEff->GetParent();
    if (pParent)
    {
        Matrix4D tm;
        tm.translation( GetWorldTM( pParent ).getTranslation() );
        TransformNode::Push( tm );
    }
    pEff->Render();
    if (pParent) TransformNode::Pop();
    TransformNode::Pop();
    return true;
} // PEffectManager::UpdateInstance

int PEffectManager::FindEffectByName( int setID, const char* effName )
{
    if (setID < 0 || setID >= m_EffectSet.size()) return NULL;
    EffectSet& effs = m_EffectSet[setID];
    int nEff = effs.m_Effect.size();
    for (int i = 0; i < nEff; i++)
    {
        if (!stricmp( effName , effs.m_Effect[i]->GetEffectName() )) return i; 
    }
    return -1;
} // PEffectManager::FindEffectByName

void PEffectManager::SetAlphaFactor( int setID, int effID, float alpha )
{
    PEffect* pEff = GetEffectFromSet( setID, effID );
    if (!pEff) return;
    PushEntityContext( pEff->GetID() );
    SetAlphaFactor( alpha );
    PopEntityContext();
} // PEffectManager::SetAlphaFactor

void PEffectManager::SetIntensity( int setID, int effID, float intensity )
{
    PEffect* pEff = GetEffectFromSet( setID, effID );
    if (!pEff) return;
    PushEntityContext( pEff->GetID() );
    SetIntensity( intensity );
    PopEntityContext();
} // PEffectManager::SetIntensity

float PEffectManager::GetCurTime() const
{
    return GetCurTime( GetCurEffInstance() );
}

void PEffectManager::SetAlphaFactor( float factor )
{
    SetAlphaFactor( GetCurEffInstance(), factor );
}

void PEffectManager::SetIntensity( float intensity )
{
    SetIntensity( GetCurEffInstance(), intensity );
}

void PEffectManager::SetTarget( const Vector3D& pos )
{
    DWORD hInst = GetCurEffInstance();
    if (!IsValid( hInst )) return;
    PEmitterInstance& em = GetEmitterInstance( hInst );
    em.m_Target = pos;
} // PEffectManager::SetTarget

PEffect* PEffectManager::GetEffectFromSet( int setID, int idx )
{
    if (setID < 0 || setID >= m_EffectSet.size()) return NULL;
    EffectSet& effs = m_EffectSet[setID];
    if (idx < 0 || idx >= effs.m_Effect.size()) return NULL;
    return (PEffect*)effs.m_Effect[idx];
} // PEffectManager::GetEffectFromSet

int PEffectManager::GetParticleArea() const
{
    float area = 0;
    ICamera* pCam = GetCamera();
    Matrix4D tm = pCam->ToSpace( sWorld, sScreen );
    Vector4D camRight = pCam->GetRight();

    PInstanceHash::const_iterator it = m_Emitters.begin();
    while (it)
    {
        const PEmitterInstance& e = *it;
        const PEmitter* pEmitter = e.m_pEmitter;
        ++it;

        bool bHasVisible = false;
        for (int j = 0; j < pEmitter->GetNChildren(); j++)
        {
            if (((POperator*)pEmitter->GetChild( j ))->IsVisual())
            {
                bHasVisible = true;
                break;
            }
        }
        if (!bHasVisible) continue;
        const PParticle* p = e.m_pParticle;
        while (p)
        {
            float par = tmax( fabs( p->m_Size.x ), fabs( p->m_Size.y ) );
            Matrix4D ptm = p->GetTransform();
            Vector4D v0( ptm.getTranslation() );
            Vector4D v1( camRight ); v1 *= par;
            v1 += v0;
            v1 *= tm; v0 *= tm;
            v1.normW(); v0.normW();
            v1 -= v0;
            par = v1.norm2();
            area += par;
            p = p->m_pNext;
        }    
    }
    return area*0.5f;
} // PEffectManager::GetParticleArea

/*****************************************************************************/
/*    PEmitterInstance implemetation
/*****************************************************************************/
void PEmitterInstance::Reset()
{
    m_pParticle        = NULL;
    m_EmitAccum        = 0.0f;
    m_CurTime        = 0.0f;
    m_LastTime        = 0.0f;
    m_ParticleID    = -1;
    m_ParentID        = -1;
    m_Flags            = 0;
    m_LastFrame        = 0;
} // PEmitterInstance::Reset

int PEmitterInstance::GetNParticles() const 
{
    int res = 0;
    PParticle* pParticle = m_pParticle;
    while (pParticle)
    {
        res++;
        pParticle = pParticle->m_pNext;
    }
    return res;
} // PEmitterInstance::GetNParticles

int PEmitterInstance::GetNBorn() const
{
    int nP = 0;
    PParticle* p = m_pParticle;
    while (p)
    {
        if (p->m_Age == 0.0f) nP++;
        p = p->m_pNext;
    }
    return nP;
} // PEmitterInstance::GetNBorn

/*****************************************************************************/
/*    PEffect implemetation
/*****************************************************************************/
PEffect::PEffect()
{
    m_EffectID        = 0xFFFFFFFF;    
    m_InstanceID    = 0xFFFFFFFF;
    m_BoneName      = "";  
    m_StartTime     = 0.0f; 
    m_ActiveTime    = 0.0f;
    m_WarmupTime    = 0.0f;
    m_Priority      = 0;
    m_Flags         = 0;
} // PEffect::PEffect

void PEffect::SetEffectFile( const char* fname ) 
{ 
    char path[_MAX_PATH];
    strcpy( path, fname );
    //ToRelativePath( path, _MAX_PATH );
    m_EffectFile    = path; 
    m_EffectID        = IMM->GetModelID( fname ); 
    m_InstanceID    = 0xFFFFFFFF; 
} // PEffect::SetEffectFile

void PEffect::Render()
{
    PushEntityContext( GetID() );
    if (m_EffectID == 0xFFFFFFFF) m_EffectID = IMM->GetModelID( m_EffectFile.c_str() ); 

    DWORD instID = IEffMgr->InstanceEffect( m_EffectID );
    if (m_InstanceID != instID) IEffMgr->RewindEffect( instID, m_WarmupTime );
    m_InstanceID = instID;
    
    Matrix4D m = TransformNode::TMStackTop();
    m.mulLeft( m_LocalTM );  
    IEffMgr->UpdateInstance( m_InstanceID, m );
    PopEntityContext();

    if (DoDrawGizmo())
    {
        IRS->ResetWorldTM();
        rsEnableZ( false );
        DrawSphere( Sphere( m.getTranslation(), 5.0f ), 0, 0xFFFFFFFF, 8 );
        rsFlushLines3D();
    }
} // PEffect::Render

void PEffect::Expose( PropertyMap& pm )
{
    pm.start( "PEffect", this );
    pm.p( "Name", &PEffect::GetName, &PEffect::SetName            );
    pm.p( "Invisible", &PEffect::IsInvisible, &PEffect::SetInvisible    );
    pm.p( "DrawGizmo", &PEffect::DoDrawGizmo, &PEffect::SetDrawGizmo    );
    pm.p( "DrawAABB", &PEffect::DoDrawAABB, &PEffect::SetDrawAABB     );
    pm.p( "Scale", &PEffect::GetScaleX, &PEffect::SetScaleX       );
    pm.p( "PosX", &PEffect::GetPosX, &PEffect::SetPosX         );
    pm.p( "PosY", &PEffect::GetPosY, &PEffect::SetPosY         );
    pm.p( "PosZ", &PEffect::GetPosZ, &PEffect::SetPosZ         );
    pm.p( "RotX", &PEffect::GetEulerX, &PEffect::SetEulerX        );
    pm.p( "RotY", &PEffect::GetEulerY, &PEffect::SetEulerY        );
    pm.p( "RotZ", &PEffect::GetEulerZ, &PEffect::SetEulerZ        );
    pm.p( "Priority", &PEffect::GetPriority, &PEffect::SetPriority     );
    pm.p( "DefaultVisible", &PEffect::IsDefaultVisible, &PEffect::SetDefaultVisible );
    pm.p( "EffectFile", &PEffect::GetEffectFile, &PEffect::SetEffectFile, "#effect" );
} // PEffect::Expose

void PEffect::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    DWORD m_Reserved = 0;
    os << m_EffectFile << m_Priority << m_BoneName << 
        m_StartTime << m_ActiveTime << m_WarmupTime << m_Flags << m_Reserved;
} // PEffect::Serialize

void PEffect::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    DWORD m_Reserved = 0;
    is >> m_EffectFile >> m_Priority >> m_BoneName >> 
        m_StartTime >> m_ActiveTime >> m_WarmupTime >> m_Flags >> m_Reserved;
} // PEffect::Unserialize

XMLNode* PEffect::ToXML() const
{
    XMLNode* pRoot = new XMLNode();
    pRoot->SetTag( "Effect" );
    pRoot->AddAttr( "Name", GetName() );
    pRoot->AddValue( "EffectFile", GetEffectFile() );
    pRoot->AddValue( "ParentBone", GetParentBoneName() );
    pRoot->AddValue( "Position", GetPos() );
    float scale = GetScaleX();
    if (fabs( scale - 1.0f) > c_SmallEpsilon) pRoot->AddValue( "Scale", scale );
    if (m_Priority > 0)         pRoot->AddValue( "Priority", m_Priority );
    if (m_WarmupTime > 0.0f)    pRoot->AddValue( "Warmup", m_WarmupTime );
    pRoot->AddValue( "StartTime", m_StartTime );
    pRoot->AddValue( "ActiveTime", m_ActiveTime );
    if (IsDefaultVisible()) pRoot->AddValue( "DefaultVisible", true );
    return pRoot;
} // PEffect::ToXML

const char* PEffect::GetParentBoneName() const
{
    SNode* pParent = GetParent();
    if (!pParent) return m_BoneName.c_str();
    ((PEffect*)this)->m_BoneName = pParent->GetName();
    return m_BoneName.c_str();
} // PEffect::GetParentBoneName

bool PEffect::FromXML( XMLNode* node )
{
    if (!node) return false;
    const char* name = NULL;
    if (node->GetAttr( "Name", name )) SetName( name );
    if (node->GetValue( "EffectFile", name )) SetEffectFile( name );
    if (node->GetValue( "ParentBone", name )) m_BoneName = name;
    node->GetValue( "StartTime", m_StartTime );
    node->GetValue( "ActiveTime", m_ActiveTime );
    node->GetValue( "Priority", m_Priority );
    node->GetValue( "Warmup", m_WarmupTime );
    bool defVisible = false;
    node->GetValue( "DefaultVisible", defVisible );
    SetDefaultVisible( defVisible );
    Vector3D pos;
    if (node->GetValue( "Position", pos )) SetPos( pos );
    float scale = 1.0f;
    if (node->GetValue( "Scale", scale )) 
    {
        SetScaleX( scale );
        SetScaleY( scale );
        SetScaleZ( scale );
    }
    return false;
} // PEffect::FromXML

/*****************************************************************************/
/*    Overlay implemetation
/*****************************************************************************/
Overlay::Overlay()
{
    m_SizeX         = 256.0f;
    m_SizeY         = 256.0f;

    m_ShiftX        = 0.0f;
    m_ShiftY        = 0.0f;

    m_USpeed        = 0.0f;
    m_VSpeed        = 0.0f;
    m_U2Speed       = 0.0f;
    m_V2Speed       = 0.0f;

    m_Color         = 0xFFFFFFFF;

    m_UV            = Rct::unit;
    m_UV2           = Rct::unit;

    SetAnimated( false );
    SetShiftedBack( false );
} // Overlay::Overlay

void Overlay::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Overlay", this );
    pm.f( "SizeX",        m_SizeX  );
    pm.f( "SizeY",        m_SizeY  );
    pm.f( "ShiftX",        m_ShiftX );
    pm.f( "ShiftY",        m_ShiftY );

    pm.f( "USpeed",        m_USpeed  );
    pm.f( "VSpeed",        m_VSpeed  );
    pm.f( "U2Speed",    m_U2Speed );
    pm.f( "V2Speed",    m_V2Speed );

    pm.f( "Color",        m_Color, "color" );

    pm.f( "u",            m_UV.x );
    pm.f( "v",            m_UV.y );
    pm.f( "du",            m_UV.w );
    pm.f( "dv",            m_UV.h );

    pm.f( "u2",            m_UV2.x );
    pm.f( "v2",            m_UV2.y );
    pm.f( "du2",        m_UV2.w );
    pm.f( "dv2",        m_UV2.h );

    pm.p( "Animate", &Overlay::IsAnimated, &Overlay::SetAnimated );
    pm.p( "ShiftBack", &Overlay::IsShiftedBack, &Overlay::SetShiftedBack );
} // Overlay::Expose

void Overlay::Render()
{
    float t = float( GetTickCount() )*0.001f;
    if (IsAnimated())
    {
        m_UV.x  = t*m_USpeed;
        m_UV.y  = t*m_VSpeed;
        m_UV2.x = t*m_U2Speed;
        m_UV2.y = t*m_V2Speed;
        m_UV.x  -= floorf( m_UV.x  );
        m_UV.y  -= floorf( m_UV.y  );
        m_UV2.x -= floorf( m_UV2.x );
        m_UV2.y -= floorf( m_UV2.y );
    }

    Vector3D a( -1.0f,  1.0f, 0.0f );
    Vector3D b(  1.0f,  1.0f, 0.0f );
    Vector3D c( -1.0f, -1.0f, 0.0f );
    Vector3D d(  1.0f, -1.0f, 0.0f );

    Matrix4D tm = Matrix3D::identity;
    tm.translation( m_ShiftX/m_SizeX, m_ShiftY/m_SizeY, 0.0f );
    ICamera* pCam = GetCamera();
    if (pCam)
    {
        tm *= Matrix4D( Vector3D( m_SizeX, m_SizeY, 1.0f ), 
                        Matrix3D( pCam->GetRight(), pCam->GetUp(), pCam->GetDir() ),
                        Vector3D::null );
    }

    const float c_ZShift = 4.0f;
    if (IsShiftedBack())
    {
        pCam->ShiftZ( -c_ZShift );
    }

    tm.translate( TransformNode::TMStackTop().getTranslation() );
    tm.transformPt( a );
    tm.transformPt( b );
    tm.transformPt( c );
    tm.transformPt( d );

    rsFlushPoly3D();
    IRS->ResetWorldTM();
    Parent::Render();
    static int shID = IRS->GetShaderID( "overlay" );
    IRS->SetShader( shID );       

    rsQuad( a, b, c, d, m_UV, m_UV2, m_Color );
    rsFlushPoly3D( false );

    if (IsShiftedBack())
    {
        pCam->ShiftZ( c_ZShift );
    }

} // Overlay::Render

void Overlay::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_SizeX << m_SizeY << m_ShiftX << m_ShiftY << 
        m_USpeed << m_VSpeed << m_U2Speed << 
        m_V2Speed << m_Color << m_UV << m_UV2 << m_Flags;
} // Overlay::Serialize

void Overlay::Unserialize( InStream& is  )
{
    Parent::Unserialize( is );
    is >> m_SizeX >> m_SizeY >> m_ShiftX >> m_ShiftY >> 
        m_USpeed >> m_VSpeed >> m_U2Speed >> 
        m_V2Speed >> m_Color >> m_UV >> m_UV2 >> m_Flags;
} // Overlay::Unserialize

void PEffectManager::SetCurrentNatColor  ( DWORD Color ){
    m_NationalColor = Color;
}
DWORD PEffectManager::GetCurrentNatColor  (){
    return m_NationalColor;
}