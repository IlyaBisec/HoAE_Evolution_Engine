/*****************************************************************
/*  File:   kContext.cpp                                          
/*  Desc:   Hierarchical game entity context                                  
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   August 2004                                        
/*****************************************************************/
#include "stdafx.h"
#include "kContext.h"

EntityContext g_EntityContext;

void PushEntityContext( DWORD ctx )
{
    g_EntityContext.Push( ctx );
} 

DWORD PopEntityContext() 
{
    return g_EntityContext.Pop();
}

void ResetEntityContext()
{
    g_EntityContext.Reset();
}

const EntityContext& GetEntityContext()
{
    return g_EntityContext;
}

void SetEntityContext( const EntityContext& context )
{
    g_EntityContext = context;
}