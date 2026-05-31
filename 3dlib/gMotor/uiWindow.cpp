/*****************************************************************************/
/*    File:    uiWindow.cpp
/*    Desc:    Window control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiWindow.h"

/*****************************************************************************/
/*  Window implementation
/*****************************************************************************/
IMPLEMENT_CLASS(Window);
Window::Window()
{
    SetDraggable();
    m_bDrawBackground = true;
} // Window::Window

void Window::Render()
{
    if (m_bDrawBackground)
    {
        rsColoredPanel( GetScreenExtents(), 0.0f, m_BgColor );
        rsFlush();
    }
} // Window::Render

void Window::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "Window", this );
    pm.f( "DrawBackground", m_bDrawBackground );
} // Window::Expose

bool Window::OnMouseMBDown( int mX, int mY )                
{ 
    if (IsDraggable() && PtIn( mX, mY )) 
    {
        SetDragged( true );
        s_DragButton = mbMiddle;
    }
    return false; 
} // Widget::OnMouseMBDown

bool Window::OnMouseMBUp( int mX, int mY )                
{ 
    if (IsDraggable() && IsDragged()) 
    {
        SetDragged( false );
        s_DragButton = (MouseButton)(s_DragButton&(~mbMiddle));
    }
    return false; 
} // Widget::OnMouseMBUp
