/*****************************************************************************/
/*    File:    kScriptManager.cpp
/*    Desc:    Script manager implementation
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-04-2004
/*****************************************************************************/
#include "stdafx.h"
#include "IScriptManager.h"
#include "kFilePath.h"
#include "kScriptManager.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind.hpp>
#include <operator.hpp>

ScriptManager           g_ScriptManager;
IScriptManager*  ISC = &g_ScriptManager;

/*****************************************************************************/
/*    Script implementation
/*****************************************************************************/
Script::Script()
{
    m_ID = -1;
}

bool Script::Reload()
{
    return ISC->ReloadScript( GetID() );
} // Script::Reload

void Script::SetSource( const char* pSrc )
{
    if (!pSrc) { m_Source = ""; return; }
    m_Source = pSrc;
} // Script::SetSource

void Test()
{

}

/*****************************************************************************/
/*    ScriptManager implementation
/*****************************************************************************/
ScriptManager::ScriptManager()
{
    m_pLuaContext = lua_open();
    luabind::open( m_pLuaContext );

    lua_baselibopen ( m_pLuaContext );
    lua_tablibopen  ( m_pLuaContext );
    lua_iolibopen   ( m_pLuaContext );
    lua_strlibopen  ( m_pLuaContext );
    lua_mathlibopen ( m_pLuaContext );
    lua_dblibopen   ( m_pLuaContext );

    using namespace luabind;
    module(m_pLuaContext)
    [
        def("Test", Test)
    ];
} // ScriptManager::ScriptManager

ScriptManager::~ScriptManager()
{
    lua_close( m_pLuaContext );
} // ScriptManager::~ScriptManager

int ScriptManager::GetScriptID( const char* name )
{
    int nScr = m_Scripts.size();
    for (int i = 0; i < nScr; i++)
    {
        const Script* pScr = &m_Scripts[i];
        if (!strcmp( pScr->GetName(), name )) return i;
    }
    FilePath path( name );
    m_Scripts.push_back( Script() );
    Script* pScr = &m_Scripts.back();
    
    pScr->SetName( name );
    pScr->SetID( nScr );

    int size = 0;
    int resID = IRM->FindResource( path.GetFileWithExt() );
    IRM->BindResource( resID, pScr );
    BYTE* pData = IRM->LockData( resID, size );
    pScr->SetSource( (char*)pData );
    IRM->UnlockData( resID );

    return nScr;
} // ScriptManager::GetScriptID

bool ScriptManager::RunScript( int scID )
{
    if (scID < 0 || scID >= m_Scripts.size()) return false;
    lua_dostring( m_pLuaContext, m_Scripts[scID].GetSource() );
    return false;
} // ScriptManager::RunScript

bool ScriptManager::ReloadScript( int scID )
{
    if (scID < 0 || scID >= m_Scripts.size()) return false;
    Script* pScr = &m_Scripts[scID];
    FilePath path( pScr->GetName() );
    int size = 0;
    int resID = IRM->FindResource( path.GetFileWithExt() );
    BYTE* pData = IRM->LockData( resID, size );
    if (!pData) return false;
    pScr->SetSource( (char*)pData );
    IRM->UnlockData( resID );
    return true;
} // ScriptManager::ReloadScript


