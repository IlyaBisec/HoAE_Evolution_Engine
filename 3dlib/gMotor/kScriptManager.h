/*****************************************************************************/
/*    File:    kScriptManager.h
/*    Desc:    Script manager implementation
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-04-2004
/*****************************************************************************/
#ifndef __KSCRIPTMANAGER_H__
#define __KSCRIPTMANAGER_H__
#include "IScriptManager.h"
#include "IResourceManager.h"

/*****************************************************************************/
/*  Class:  Script
/*  Desc:   Single script resource instance
/*****************************************************************************/
class Script : public IResource
{
    std::string         m_Name;
    int                 m_ID;
    std::string         m_Source;

public:
                            Script      ();
    const char*             GetName     () const { return m_Name.c_str(); }
    int                     GetID       () const { return m_ID; }
    void                    SetID       ( int id ) { m_ID = id; }
    void                    SetName     ( const char* name ) { m_Name = name; }
    void                    SetSource   ( const char* pSrc );
    const char*             GetSource   () const { return m_Source.c_str(); }
    virtual bool            Reload      ();

}; // class Script

struct lua_State;
/*****************************************************************************/
/*    Class:    ScriptManager
/*    Desc:    Script manager implementation using Lua 
/*****************************************************************************/
class ScriptManager : public IScriptManager
{
    std::vector<Script>     m_Scripts;
    lua_State*              m_pLuaContext;

public:
                    ScriptManager    ();
                    ~ScriptManager    ();

    virtual int     GetScriptID     ( const char* name );
    virtual bool    RunScript       ( int scID );
    virtual bool    ReloadScript    ( int scID );

private:
}; // class ScriptManager

#endif // __KSCRIPTMANAGER_H__