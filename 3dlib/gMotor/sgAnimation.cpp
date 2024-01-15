/***********************************************************************************/
/*  File:   sgAnimation.cpp
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "sgAnimation.h"
#include "sgTransformNode.h"
#include "vSkin.h"

IMPLEMENT_CLASS( Animation );

/***********************************************************************************/
/*  Animation implementation
/***********************************************************************************/
std::stack<float>    Animation::s_CurAnimTime;
std::stack<float>    Animation::s_Weight;
std::stack<float>    Animation::s_CurAnimTimeDelta;
DWORD                Animation::s_PrevTime = 0;

Animation::Animation() :    m_AnimationTime     ( 0.0f ), 
                            m_CurrentTime       ( 0.0f ), 
                            m_StartTime         ( 0.0f ),
                            m_bLooped           ( true ),
                            m_bPlayed           ( false ),
                            m_bPaused           ( false ),
                            m_pOperated         ( NULL ),
                            m_pOperatedParent   ( NULL )
{
}

void Animation::BindNode( SNode* pNode )
{
    if (m_pOperated == pNode) return;    
    int nBound = 0;
    int nBones = 0;

    Iterator it( this );
    while (it)
    {
        Animation* pAnm = dynamic_cast<Animation*>( *it );
        if (!pAnm) break;

        const char* anmName = pAnm->GetName();
        Iterator nit( pNode );
        nBones = 0;
        while (nit)
        {
            TransformNode* pTM = dynamic_cast<TransformNode*>( *nit );
            ++nit;
            if (!pTM) continue;
            nBones++;
            const char* tmName = pTM->GetName();
            if (!stricmp( anmName, tmName ))
            {
                SNode* pParent = pTM->GetParent();
                if (pParent && pParent->IsA<Skin>()) continue;
                pAnm->m_pOperated = pTM;
                nBound++;
                //  find parent
                Animation* pAnmParent = dynamic_cast<Animation*>( pAnm->GetParent() );
                if (pAnmParent)
                {
                    Iterator pit( pNode );
                    while (pit)
                    {
                        SNode* pN = (SNode*)*pit;
                        ++pit;
                        if (!pN) continue;
                        const char* name1 = pN->GetName();
                        const char* name2 = pAnmParent->GetName();
                        if (!stricmp( name1, name2 ) && pParent != pN) 
                        {
                            pAnm->m_pOperatedParent = pN;
                        }
                    }
                }
                break;
            }
        }
        ++it;
    }
    m_pOperated = pNode;
} // Animation::BindNode

void Animation::SetupTimeDelta()
{
    DWORD cTime = ::GetTickCount();
    if (s_PrevTime == 0) s_PrevTime = cTime;
    while (!s_CurAnimTimeDelta.empty()) s_CurAnimTimeDelta.pop();
    s_CurAnimTimeDelta.push( float( cTime - s_PrevTime ) );
    s_PrevTime = cTime;
} // Animation::SetupTimeDelta

void Animation::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    float reserved = 0.0f;
    os << reserved << m_AnimationTime << m_StartTime;
} // Animation::Serialize

void Animation::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    float reserved = 0.0f;
    is >> reserved >> m_AnimationTime >> m_StartTime;
} // Animation::Unserialize

void Animation::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Animation", this );
    pm.p( "TotalTime", &Animation::GetAnimationTime, &Animation::SetAnimationTime );
    pm.p( "StartTime", &Animation::GetStartTime, &Animation::SetStartTime );
    pm.f( "CurrentTime", m_CurrentTime   );
    pm.f( "Looped", m_bLooped       );
    pm.p( "Played", &Animation::IsPlaying       );
    pm.m( "Play", &Animation::Play            );
    pm.m( "Pause", &Animation::Pause           );
    pm.m( "Stop", &Animation::Stop            );
} // Animation::Expose

void Animation::Render()
{
    if (m_bPaused)
    {
        return;
    }

    if (m_bPlayed)
    {
        float cTime = GetCurrentTime() + CurTimeDelta();
        if (cTime > GetMaxTime()) 
        {
            if (m_bLooped)
            {
                cTime = fmod( cTime, GetMaxTime() ) + GetStartTime();
            }
            else
            {
                cTime = GetMaxTime();
            }
        }
        SetCurrentTime( cTime );
    }
    else
    {
        SetCurrentTime( CurTime() );
    }    
} // Animation::Render

void Animation::Play() 
{ 
    if (!m_bPlayed) SetCurrentTime( GetStartTime() );
    m_bPlayed = true;  
    m_bPaused = false;
}

bool Animation::IsPlaying() const 
{
    return m_bPlayed; 
}

void Animation::Pause() 
{
    m_bPaused = !m_bPaused;
}

void Animation::Stop() 
{ 
    m_bPlayed = false; 
    SetCurrentTime( 0.0f );
}

void Animation::Loop( bool bLoop ) 
{ 
    m_bLooped = bLoop; 
}