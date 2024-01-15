/*****************************************************************************/
/*    File:    uiScrollBox.cpp
/*    Desc:    Widget with scrollable contents
/*    Author:    Ruslan Shestopalyuk
/*    Date:    30 Dec 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiScrollBox.h"

/*****************************************************************************/
/*  ScrollBox implementation
/*****************************************************************************/
IMPLEMENT_CLASS(ScrollBox);
ScrollBox::ScrollBox()
{
    //SetDraggable( false );
} // ScrollBox::ScrollBox

void ScrollBox::Render()
{
    int nCh = NumChildren();

    rsPanel( GetScreenExtents(), 0.0f, 0xFFFFFFFF, m_BgColor );
    rsFlushPoly2D();
} // ScrollBox::Render

