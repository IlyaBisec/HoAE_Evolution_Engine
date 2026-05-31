/*****************************************************************************/
/*    File:    uiEditBox.cpp
/*    Desc:    EditBox control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiButton.h"
#include "uiEditBox.h"
#include "uiExtendedEdit.h"

#include "kSystemDialogs.h"
#include "kFilePath.h"

/*****************************************************************************/
/*    ExtendedEdit implementation
/*****************************************************************************/
IMPLEMENT_CLASS(ExtendedEdit);
ExtendedEdit::ExtendedEdit()
{
    AddChild( m_btnBrowse );
    AddChild( m_edPath );
    m_btnBrowse.sigPressed.Connect( this, &ExtendedEdit::Browse );
    sigTextChanged.Connect( this, &ExtendedEdit::Text2Box );
    m_edPath.sigUnfocus.Connect( this, &ExtendedEdit::Box2Text );
    m_btnBrowse.SetText( "..." );
    SetDraggable( false );
} // ExtendedEdit::ExtendedEdit

void ExtendedEdit::Render()
{
} // ExtendedEdit::Render

void ExtendedEdit::Browse()
{
    _chdir( IRM->GetHomeDirectory() );
    OpenFileDialog dlg;

    const char* type = m_ExType.c_str();
    if (!stricmp( type, "shaderscript" ))
    {
        _chdir( "Shaders\\" );
        dlg.AddFilter( "DirectX Effect Files", "*.fx" );
        dlg.SetDefaultExtension( "fx" );
    }
    if (!stricmp( type, "texture" ))
    {
        _chdir( "Textures\\" );
    }
    if (!stricmp( type, "animfile" ) || !stricmp( type, "model" ))
    {
        _chdir( "Models\\" );
        dlg.AddFilter( ".c2m animation files", "*.c2m" );
        dlg.SetDefaultExtension( "c2m" );
    }

    static char lpstrFile[_MAX_PATH];
    bool dlgRes = dlg.Show();
    ///RelaxDialog();
    if (!dlgRes) return;

    FilePath path( dlg.GetFilePath() );
    //path -= IRM->GetHomeDirectory();
    SetText( path.GetFullPath() );
    m_edPath.SetText( path.GetFullPath() );
} // ExtendedEdit::Browse

bool ExtendedEdit::OnExtents()
{
    const Rct& ext = GetExtents();
    float bw = ext.h*1.2f;
    m_btnBrowse.SetExtents( ext.w - bw, 0.0f, bw, ext.h );
    m_edPath.SetExtents( 0.0f, 0.0f, ext.w - bw, ext.h );
    return false;
} // ExtendedEdit::OnExtents

void ExtendedEdit::Box2Text() 
{ 
    FilePath path( m_edPath.GetText() );
    SetText( path.GetFileWithExt() );
} // ExtendedEdit::OnUnfocus




