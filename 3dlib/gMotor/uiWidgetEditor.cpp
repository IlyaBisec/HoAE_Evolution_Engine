/*****************************************************************************/
/*    File:    uiWidgetEditor.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-23-2003
/*****************************************************************************/
#include "stdafx.h"
#include "uiWindow.h"
#include "uiWidgetEditor.h"
#include "uiKangaroo.h"

/*****************************************************************************/
/*    WidgetEditor implementation
/*****************************************************************************/
IMPLEMENT_CLASS(WidgetEditor);
WidgetEditor::WidgetEditor()
{
    AddChild( m_Inspector );
    AddChild( m_WidgetTree );
    AlignToParent();
    SetName( "WidgetEditor" );
    SetDraggable    ( false );
    SetKeptAligned  ( true );

    m_Inspector.SetVAligh( vaTop );
    m_Inspector.SetHAligh( haCenter );
} // WidgetEditor::WidgetEditor

void WidgetEditor::Expose( PropertyMap& pm )
{
    pm.start( "WidgetEditor", this );
} // WidgetEditor::Expose

void WidgetEditor::OnInit()
{
    m_WidgetTree.SetRootNode( &Kangaroo::instance() );
}

void WidgetEditor::Render()
{
    m_Inspector.Bind( m_WidgetTree.GetSelectedNode() );

} // WidgetEditor::Render

