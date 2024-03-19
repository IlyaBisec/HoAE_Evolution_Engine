/*****************************************************************
/*  File:   IScriptManager.h                                      
/*  Desc:   Interface to the script managing system
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Jun 2004                                             
/*****************************************************************/
#ifndef __ISCRIPTMANAGER_H__ 
#define __ISCRIPTMANAGER_H__

/*****************************************************************************/
/*    Class:    IScriptManager
/*    Desc:    Interface for managing scripts
/*****************************************************************************/
class IScriptManager
{
public:
    virtual int     GetScriptID ( const char* name ) = 0;
    virtual bool    RunScript   ( int scID ) = 0;
    virtual bool    ReloadScript( int scID ) = 0;

}; // class IScriptManager

extern IScriptManager* ISC;

#endif // __ISCRIPTMANAGER_H__ 