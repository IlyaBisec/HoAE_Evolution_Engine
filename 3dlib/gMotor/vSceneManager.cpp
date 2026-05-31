/*****************************************************************************/
/*	File:	vSceneManager.cpp
/*	Desc:	ModelObject manager interface implementation
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#include "stdafx.h"
#include "vSceneManager.h"

const RenderBit  RenderBit::c_Invalid;

SceneManager        g_SceneMgr;
SceneManager*       ISceneMgr = &g_SceneMgr;

/*****************************************************************************/
/*  SceneManager implementation
/*****************************************************************************/
SceneManager::SceneManager()
{
    ResetScene();
}

struct TaskCompare
{
    bool operator ()( const RenderBit* l, const RenderBit* r )
    {
        return (*l < *r);
    }
}; // struct TaskCompare

void SceneManager::SortTasks()
{
    if (m_bSorted) return;
    m_SortedTasks.resize( m_NTasks );
    int nT = m_NTasks;
    for (int i = 0; i < nT; i++) { m_SortedTasks[i] = &m_Tasks[i]; }
    std::sort( m_SortedTasks.begin(), m_SortedTasks.end(), TaskCompare() );
    m_bSorted = true;
} // SceneManager::SortTasks

void SceneManager::RenderScene()
{
    SortTasks();
    int nT = m_SortedTasks.size();
    for (int i = 0; i < nT; i++)
    {
        const RenderBit& rt = *m_SortedTasks[i];
        const RenderBit& pt = (i == 0) ? RenderBit::c_Invalid : *m_SortedTasks[i - 1];

        if (rt.m_ShaderID != pt.m_ShaderID) IRS->SetShader( rt.m_ShaderID, rt.m_Pass );
        for (int j = 0; j < c_MaxTextureStages; j++) 
        {
            if (rt.m_TexID[j] != pt.m_TexID[j]) IRS->SetTexture( rt.m_TexID[j], j );
        }
        if (rt.m_bHasTM) IRS->SetWorldTM( rt.m_TM ); else IRS->ResetWorldTM();

        if (rt.m_ShaderID != pt.m_ShaderID) IRS->SetShaderAutoVars();
        if (rt.m_VBufID != pt.m_VBufID || rt.m_VType != pt.m_VType) IRS->SetVB( rt.m_VBufID, rt.m_VType );
        if (rt.m_IBufID != pt.m_IBufID) 
        {
            IRS->SetIB( rt.m_IBufID );
        }
        IRS->Draw( rt.m_FirstVert, rt.m_NVert, rt.m_FirstIdx, rt.m_NIdx, rt.m_PriType );
    }
} // SceneManager::RenderScene

void SceneManager::ResetScene() 
{ 
    m_NTasks  = 0; 
    m_bSorted = false; 
} // SceneManager::ResetScene

