/*****************************************************************
/*  File:   dbSkinCache.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbSkinCache.h"
#include "ISkinCache.h"
#include "ISceneEditor.h"



/*****************************************************************/
/*	SkinCacheDebugger implementation
/*****************************************************************/
SkinCacheDebugger::SkinCacheDebugger()
{
   m_bDrawModels        = false;
   m_bDrawSkinCache     = true;
   m_bApplyLOD          = false;

   m_bDrawDebugInfo     = true;
   m_bDrawScene         = false;
   m_DebugSurface       = -1;
   m_ModelName          = "Models\\tl.C2M";
   m_AnimName           = "Models\\tl_walk.C2M";

   m_NX                 = 10;
   m_NY                 = 10;
   m_bAnimate           = true;
   
   m_Scale              = 0.2f;
   m_Width              = 800.0f;

   m_ModelID            = 0xFFFFFFFF;
   m_AnimID             = 0xFFFFFFFF;
   m_SceneID            = 0xFFFFFFFF;
   m_LODTreshold        = 420.0f*420.0f;
} // SkinCacheDebugger::SkinCacheDebugger

void SkinCacheDebugger::Expose( PropertyMap& pm )
{
	pm.start( "SkinCacheDebugger", this );
    pm.f( "DrawModels",       m_bDrawModels       );
    pm.f( "DrawSkinCache",    m_bDrawSkinCache    );
    pm.f( "DrawScene",        m_bDrawScene        );
    pm.f( "DrawDebugInfo",    m_bDrawDebugInfo    );
    pm.p( "ModelName",        GetModelName, SetModelName, "#model" );
    pm.p( "AnimationName",    GetAnimName, SetAnimName, "#model" );
    pm.f( "NX",               m_NX                );
    pm.f( "NY",               m_NY                );
    pm.f( "Animate",          m_bAnimate          );
    pm.f( "Scale",            m_Scale             );
    pm.f( "Width",            m_Width             );
    pm.m( "Dump",             Dump                );
    pm.f( "ApplyLOD",         m_bApplyLOD         );
    pm.f( "LODTreshold",      m_LODTreshold       );
} // SkinCacheDebugger::Expose

void SkinCacheDebugger::Dump()
{
}

void SkinCacheDebugger::SetModelName( const char* name )
{
    m_ModelName = name;
    m_ModelID = -1;
}

void SkinCacheDebugger::SetAnimName( const char* name )
{
    m_AnimName = name;
    m_AnimID = -1;
}

struct MdlInstance
{
    DWORD       mdlID;
    DWORD       animID;
    float       animTime;
    Matrix4D    tm;
}; // struct MdlInstance

void SkinCacheDebugger::Render()
{
    if (m_ModelID == 0xFFFFFFFF) m_ModelID = IMM->GetModelID( m_ModelName.c_str() );
    if (m_AnimID  == 0xFFFFFFFF) m_AnimID  = IMM->GetModelID( m_AnimName.c_str()  );
    if (m_ModelID == 0xFFFFFFFF || m_AnimID  == 0xFFFFFFFF) return;

    if (m_SceneID == 0xFFFFFFFF) m_SceneID = IMM->GetModelID( "Models\\battle.c2m" );
    //IScEd->ShowGrid( false );
    if (m_bDrawScene) 
    {
        IMM->StartModel( m_SceneID );
        IMM->DrawModel();
    }

    std::vector<MdlInstance>    models;
    AABoundBox aabb = IMM->GetBoundBox( m_ModelID );
    float s = m_Scale*m_Width/(aabb.GetDX()*2.0f);
    rndInit( 0xBAADF00D );
    float cTime = float( GetTickCount() );
    float anmT = IMM->GetAnimTime( m_AnimID );
    float dx = m_Width/float( m_NX );
    float dy = dx;
    float bx = -float( m_NX )*dx*0.5f;
    float by = -float( m_NY )*dy*0.5f;

    ISkCache->Init();
    for (int i = 0; i < m_NX; i++)
    {
        for (int j = 0; j < m_NY; j++)
        {
            MdlInstance mdl;
            mdl.mdlID   = m_ModelID;
            mdl.animID  = m_AnimID;
            float t = rndValuef( 0.0f, anmT ) + cTime;
            t = fmodf( t, anmT );
            mdl.animTime = t;
            mdl.tm.st( m_Scale, Vector3D(   (float( i ) + 0.5f)*dx + bx, 
                                            (float( j ) + 0.5f)*dy + by, 0.0f ) );
            models.push_back( mdl );
        }
    }

    ICamera* cam = GetCamera();
    Vector3D camPos = cam->GetPosition();
    int nM = models.size();
    for (int i = 0; i < nM; i++)
    {
        const MdlInstance& m = models[i];
        bool bDrawModel = m_bDrawModels;
        if (m_bApplyLOD)
        {
            float dist2 = m.tm.getTranslation().distance2( camPos );
            if (dist2 > m_LODTreshold) bDrawModel = false;

            if (bDrawModel)
            {
                IMM->AnimateModel( m.mdlID, m.tm, m_bAnimate ? m.animID : 0xFFFFFFFF, m.animTime );
            }
            else
            {
                if (m_bAnimate) ISkCache->Draw( m.mdlID, m.animID, m.animTime, m.tm );
                else ISkCache->Draw( m.mdlID, m.tm );
            }
        }
        else
        {
            if (m_bDrawModels)
            {
                IMM->AnimateModel( m.mdlID, m.tm, m.animID, m.animTime );
            }
            if (m_bDrawSkinCache)
            {
                if (m_bAnimate) ISkCache->Draw( m.mdlID, m_bAnimate ? m.animID : 0xFFFFFFFF, m.animTime, m.tm );
                else ISkCache->Draw( m.mdlID, m.tm );
            }
        }
    }

    ISkCache->Flush();

    if (m_bDrawDebugInfo) ISkCache->DrawDebugInfo();
} // SkinCacheDebugger::Render


