/*****************************************************************************/
/*    File:    uiAlgoDebugger.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiAlgoDebugger.h"
#include "uiRTSController.h"
#include "mLodder.h"
#include "ITerrain.h"
#include "ISkyRenderer.h"
#include "kDirIterator.h"
#include "IMediaManager.h"
#include "IShadowManager.h"
#include "kTimer.h"
#include "kContext.h"
#include "sgNode.h"

/*****************************************************************************/
/*  AlgorithmDebugger implementation
/*****************************************************************************/
IMPLEMENT_CLASS(AlgorithmDebugger);
AlgorithmDebugger::AlgorithmDebugger()
{
    AddChild        ( m_Props );
    AddChild        ( m_CamController  );
    AddChild        ( m_Toolbar );
    AlignToParent   ();
    SetName         ( "Debugger" );
    SetDraggable    ( false );
    SetKeptAligned  ( true );

    m_CamController.AlignToParent();
    m_Number            = 0;
    m_Scale             = 1.0f;
    m_Pos               = Vector3D::null;
    m_Dir               = Vector3D( -1.5f, -1.0f, -1.5f );
    m_Color             = 0x77000000;
    m_bShowBackdrop     = true;

    m_FocusDistance     = 1000.0f;
    m_FocusRatio        = 0.8f;
    m_bShowDebugStuff   = false;
    m_LOD               = 0;

} // AlgorithmDebugger::AlgorithmDebugger

void AlgorithmDebugger::Expose( PropertyMap& pm )
{
    pm.start( "AlgorithmDebugger", this );
    pm.f( "Backdrop",       m_bShowBackdrop );
    pm.f( "Number",         m_Number );
    pm.f( "Scale",          m_Scale );
    pm.f( "PosX",           m_Pos.x );
    pm.f( "PosY",           m_Pos.y );
    pm.f( "PosZ",           m_Pos.z );

    pm.f( "DirX",           m_Dir.x );
    pm.f( "DirY",           m_Dir.y );
    pm.f( "DirZ",           m_Dir.z );

    pm.f( "FocusDistance",  m_FocusDistance );
    pm.f( "FocusRatio",     m_FocusRatio );
    pm.f( "DebugStuff",     m_bShowDebugStuff );
    pm.f( "Color",          m_Color );
    pm.f( "LOD",            m_LOD   );
    pm.m( "Reset", &AlgorithmDebugger::Reset );
} // AlgorithmDebugger::Expose

void AlgorithmDebugger::OnInit()
{
    m_Toolbar.CreateFromScript( "debug_anim_toolbar" );

    //Character* pChar0 = new Character();
    //pChar0->SetPosition( Vector3D( rndValuef( -10000.0f, 10000.0f ), rndValuef( -10000.0f, 10000.0f ), 0.0f ) );
    //g_NPCs.push_back( pChar0 );
    //pChar0->m_Model             = "Units\\MecHEnf\\MecHEnfM";
    //pChar0->m_RunAnim           = "Units\\MecHEnf\\MecHEnfA_run1";
    //pChar0->m_WalkAnim          = "Units\\MecHEnf\\MecHEnfA_walk1";
    //pChar0->m_DeathAnim         = "Units\\MecHEnf\\MecHEnfA_death1";
    //pChar0->m_StandAnim[0]      = "Units\\MecHEnf\\MecHEnfA_stand1";
    //pChar0->m_StandAnim[1]      = "Units\\MecHEnf\\MecHEnfA_stand2";
    //pChar0->m_StandAnim[2]      = "Units\\MecHEnf\\MecHEnfA_stand3";
    //pChar0->m_AttackAnim[0]     = "Units\\MecHEnf\\MecHEnfA_attack1";
    //pChar0->m_AttackAnim[1]     = "Units\\MecHEnf\\MecHEnfA_attack2";
    //pChar0->m_AttackAnim[2]     = "Units\\MecHEnf\\MecHEnfA_attack3";
    //pChar0->m_AttackAnim[3]     = "Units\\MecHEnf\\MecHEnfA_attack3";

    //Character* pChar1 = new Character();
    //pChar1->SetPosition( Vector3D( rndValuef( -10000.0f, 10000.0f ), rndValuef( -10000.0f, 10000.0f ), 0.0f ) );
    //g_NPCs.push_back( pChar1 );
    //pChar1->m_Model             = "Units\\IceHArc\\IceHArcM";
    //pChar1->m_RunAnim           = "Units\\IceHArc\\IceHArcA_walk1";
    //pChar1->m_WalkAnim          = "Units\\IceHArc\\IceHArcA_walk1";
    //pChar1->m_DeathAnim         = "Units\\IceHArc\\IceHArcA_death1";
    //pChar1->m_StandAnim[0]      = "Units\\IceHArc\\IceHArcA_stand1";
    //pChar1->m_StandAnim[1]      = "Units\\IceHArc\\IceHArcA_stand1";
    //pChar1->m_StandAnim[2]      = "Units\\IceHArc\\IceHArcA_stand1";
    //pChar1->m_AttackAnim[0]     = "Units\\IceHArc\\IceHArcA_attack1";
    //pChar1->m_AttackAnim[1]     = "Units\\IceHArc\\IceHArcA_spell1";
    //pChar1->m_AttackAnim[2]     = "Units\\IceHArc\\IceHArcA_spell2";
    //pChar1->m_AttackAnim[3]     = "Units\\IceHArc\\IceHArcA_spell3";

    //Character* pChar2 = new Character();
    //pChar2->SetPosition( Vector3D( rndValuef( -10000.0f, 10000.0f ), rndValuef( -10000.0f, 10000.0f ), 0.0f ) );
    //g_NPCs.push_back( pChar2 );
    //pChar2->m_Model             = "Units\\UndHLic\\UndHLicM";
    //pChar2->m_RunAnim           = "Units\\UndHLic\\UndHLicA_walk1";
    //pChar2->m_WalkAnim          = "Units\\UndHLic\\UndHLicA_walk1";
    //pChar2->m_DeathAnim         = "Units\\UndHLic\\UndHLicA_death1";
    //pChar2->m_StandAnim[0]      = "Units\\UndHLic\\UndHLicA_stand1";
    //pChar2->m_StandAnim[1]      = "Units\\UndHLic\\UndHLicA_stand1";
    //pChar2->m_StandAnim[2]      = "Units\\UndHLic\\UndHLicA_stand1";
    //pChar2->m_AttackAnim[0]     = "Units\\UndHLic\\UndHLicA_attack1";
    //pChar2->m_AttackAnim[1]     = "Units\\UndHLic\\UndHLicA_spell1";
    //pChar2->m_AttackAnim[2]     = "Units\\UndHLic\\UndHLicA_spell2";
    //pChar2->m_AttackAnim[3]     = "Units\\UndHLic\\UndHLicA_spell3";

    //Character* pChar3 = new Character();
    //pChar3->SetPosition( Vector3D( rndValuef( -10000.0f, 10000.0f ), rndValuef( -10000.0f, 10000.0f ), 0.0f ) );
    //g_NPCs.push_back( pChar3 );
    //pChar3->m_Model             = "Units\\GiaUMin\\GiaUMinM";
    //pChar3->m_RunAnim           = "Units\\GiaUMin\\GiaUMinA_walk1";
    //pChar3->m_WalkAnim          = "Units\\GiaUMin\\GiaUMinA_walk1";
    //pChar3->m_DeathAnim         = "Units\\GiaUMin\\GiaUMinA_death1";
    //pChar3->m_StandAnim[0]      = "Units\\GiaUMin\\GiaUMinA_stand1";
    //pChar3->m_StandAnim[1]      = "Units\\GiaUMin\\GiaUMinA_stand2";
    //pChar3->m_StandAnim[2]      = "Units\\GiaUMin\\GiaUMinA_stand2";
    //pChar3->m_AttackAnim[0]     = "Units\\GiaUMin\\GiaUMinA_attack1";
    //pChar3->m_AttackAnim[1]     = "Units\\GiaUMin\\GiaUMinA_attack1";
    //pChar3->m_AttackAnim[2]     = "Units\\GiaUMin\\GiaUMinA_attack2";
    //pChar3->m_AttackAnim[3]     = "Units\\GiaUMin\\GiaUMinA_attack2";

    //Character* pChar4 = new Character();
    //pChar4->SetPosition( Vector3D( rndValuef( -10000.0f, 10000.0f ), rndValuef( -10000.0f, 10000.0f ), 0.0f ) );
    //g_NPCs.push_back( pChar4 );

    //pChar4->m_Model             = "Units\\DruUTre\\DruUTreM";
    //pChar4->m_RunAnim           = "Units\\DruUTre\\DruUTreA_walk1";
    //pChar4->m_WalkAnim          = "Units\\DruUTre\\DruUTreA_walk1";
    //pChar4->m_DeathAnim         = "Units\\DruUTre\\DruUTreA_death1";
    //pChar4->m_StandAnim[0]      = "Units\\DruUTre\\DruUTreA_stand1";
    //pChar4->m_StandAnim[1]      = "Units\\DruUTre\\DruUTreA_stand2";
    //pChar4->m_StandAnim[2]      = "Units\\DruUTre\\DruUTreA_stand2";
    //pChar4->m_AttackAnim[0]     = "Units\\DruUTre\\DruUTreA_attack1";
    //pChar4->m_AttackAnim[1]     = "Units\\DruUTre\\DruUTreA_attack2";
    //pChar4->m_AttackAnim[2]     = "Units\\DruUTre\\DruUTreA_attack3";
    //pChar4->m_AttackAnim[3]     = "Units\\DruUTre\\DruUTreA_attack4";

    ////g_NPCs.clear();

} // AlgorithmDebugger::OnInit

bool AlgorithmDebugger::OnMouseWheel( int delta )                    
{ 
    return false; 
}

bool AlgorithmDebugger::OnMouseMove( int mX, int mY, DWORD keys )  
{ 
    return false; 
}

bool AlgorithmDebugger::OnMouseLBDown( int mX, int mY )                
{ 
    return false; 
}

bool AlgorithmDebugger::OnMouseMBDown( int mX, int mY )                
{ 
    return false; 
}

bool AlgorithmDebugger::OnMouseRBDown( int mX, int mY )                
{ 
    return false; 
}

bool AlgorithmDebugger::OnMouseLBUp( int mX, int mY )                
{ 
    return false; 
}

bool AlgorithmDebugger::OnMouseMBUp( int mX, int mY )                
{ 
    return false; 
}

bool AlgorithmDebugger::OnMouseRBUp( int mX, int mY )                
{ 
    return false; 
}

bool AlgorithmDebugger::OnKeyDown( DWORD keyCode, DWORD flags )    
{ 
    if (keyCode == 'C')
    {
        m_FrustumSnapshot = GetCamera()->GetFrustum();  
    }
    return false; 
} // AlgorithmDebugger::OnKeyDown

bool AlgorithmDebugger::OnKeyUp( DWORD keyCode, DWORD flags )    
{ 
    return false; 
}

void AlgorithmDebugger::Reset()
{

}

void AlgorithmDebugger::Render()
{
    ICamera* pcam = GetCamera();
    pcam->Render();

    IShadowMgr->SetShadowQuality( (ShadowQuality)m_LOD );

    m_Props.Bind( this );    
    if (m_bShowBackdrop)
    {
        ISky->Render();
    }

    IRS->Flush();

    if (m_bShowBackdrop)
    {
        Vector3D dir( m_Dir );
        dir.normalize();
        IShadowMgr->SetLightDir( dir );
        IShadowMgr->Render();
        ITerra->Render();
    
    }

    if (m_bShowDebugStuff) IShadowMgr->DrawDebugInfo();

    IRS->Flush();

    DrawFrustum( m_FrustumSnapshot, 0x22FFFF00, 0xAAFFFF00 );
    rsFlush();
} // AlgorithmDebugger::Render