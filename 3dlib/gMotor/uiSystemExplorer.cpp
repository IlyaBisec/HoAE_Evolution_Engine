/*****************************************************************************/
/*    File:    uiSystemExplorer.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiSystemExplorer.h"

/*****************************************************************************/
/*  SystemExplorer implementation
/*****************************************************************************/
IMPLEMENT_CLASS(SystemExplorer);
SystemExplorer::SystemExplorer()
{
    AddChild        ( m_NodeTree );
    AddChild        ( m_Props );
    SetName         ( "SystemExplorer" );

    SetDraggable    ( false );
    SetKeptAligned  ( true );
} // SystemExplorer::SystemExplorer

void SystemExplorer::Expose( PropertyMap& pm )
{
    pm.start( "SystemExplorer", this );
} // SystemExplorer::Expose

void SystemExplorer::Render()
{
    
} // SystemExplorer::Render