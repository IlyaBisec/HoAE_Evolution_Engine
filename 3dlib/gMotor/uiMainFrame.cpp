/*****************************************************************************/
/*    File:    uiMainFrame.cpp
/*    Desc:    Main frame window
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"

#include "uiWidget.h"
#include "uiWindow.h"
#include "uiFrameWindow.h"
#include "uiMainFrame.h"
#include "ICamera.h"

/*****************************************************************************/
/*  MainFrame implementation
/*****************************************************************************/
IMPLEMENT_CLASS(MainFrame);
MainFrame::MainFrame()
{
    AlignToViewport ();
    SetDraggable    ( false );
    SetKeptAligned  ( true );

} // MainFrame::MainFrame

void MainFrame::Render()
{
    IRS->ClearDevice( 0xFF2B2D3B, true, true );

    Rct vp = IRS->GetViewPort();
    s_ScalingFactor  = vp.w/1024.0f;
    IRS->SetShader( -1 );
    GetCamera()->SetViewport( vp );
} // MainFrame::Render

bool MainFrame::OnMouseMove( int mX, int mY, DWORD keys )
{
    s_PrevMouseX    = s_MouseX;
    s_PrevMouseY    = s_MouseY;
    s_MouseX        = mX;
    s_MouseY        = mY;
    return false;
} // MainFrame::OnMouseMove

bool MainFrame::OnMouseRBUp( int mX, int mY )
{
    s_DragButton = (MouseButton)(s_DragButton&(~mbRight));
    return false;
} // MainFrame::OnMouseRBUp

bool MainFrame::OnMouseMBUp( int mX, int mY )
{
    s_DragButton = (MouseButton)(s_DragButton&(~mbMiddle));
    return false;
} // MainFrame::OnMouseMBUp

bool MainFrame::OnMouseLBUp( int mX, int mY )
{
    s_DragButton = (MouseButton)(s_DragButton&(~mbLeft));
    return false;
} // MainFrame::OnMouseLBUp

void MainFrame::OnDrop( int mX, int mY, IReflected* pDropped )
{
    IReflected::Iterator it( this );
    while (it)
    {
        Widget* pWidget = dynamic_cast<Widget*>( *it );
        ++it;
        if (!pWidget) continue;
        Rct rc = pWidget->GetScreenExtents(); 
        if (rc.PtIn( mX, mY ) && pWidget != this) 
        {
            pWidget->OnDrop( mX - rc.x, mY - rc.y, pDropped );
        }
    }
} // MainFrame::OnDrop

Widget* MainFrame::PickWidget( int mX, int mY )
{
    IReflected::Iterator it( this );
    while (it)
    {
        Widget* pWidget = dynamic_cast<Widget*>( *it );
        ++it;
        if (!pWidget) continue;
        Rct rc = pWidget->GetScreenExtents(); 
        if (rc.PtIn( mX, mY )) return pWidget;
    }
    return NULL;
} // MainFrame::PickWidget

bool MainFrame::KeyDown( DWORD keyCode, DWORD flags )
{
    if (s_pFocused && s_pFocused != this)
    {
        if (s_pFocused->KeyDown( keyCode, flags )) return true;
    }
    return Widget::KeyDown( keyCode, flags );
} // MainFrame::KeyDown

bool MainFrame::Char( DWORD charCode, DWORD flags )
{
    if (s_pFocused && s_pFocused != this)
    {
        if (s_pFocused->Char( charCode, flags )) return true;
    }
    return Widget::Char( charCode, flags );
} // MainFrame::Char

bool MainFrame::KeyUp( DWORD keyCode, DWORD flags )
{
    if (s_pFocused && s_pFocused != this)
    {
        if (s_pFocused->KeyUp( keyCode, flags )) return true;
    }
    return Widget::KeyUp( keyCode, flags );
} // MainFrame::KeyUp




