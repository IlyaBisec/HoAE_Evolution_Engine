/*****************************************************************************/
/*    File:    uiListBox.cpp
/*    Desc:    ListWidget control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiListBox.h"

/*****************************************************************************/
/*  ListWidget implementation
/*****************************************************************************/
IMPLEMENT_CLASS(ListWidget);
ListWidget::ListWidget()
{
    m_bAutoHeight   = false;
    m_SelTextColor  = 0xFFFFFFFF;
    m_SelBgColor    = 0xFF0000FF;
    m_SelectedItem  = -1;
} // ListWidget::ListWidget

void ListWidget::AddItem( Widget* pItem )
{
    Rct ext = GetExtents();
    float h = 0.0f;
    for (int i = 0; i < NumChildren(); i++) 
    {
        h += GetChild( i )->GetExtents().h;
    }
    pItem->SetExtents( 0, h, ext.w, ext.w );
    pItem->SetDraggable( false );
    AddChild( pItem );
} // ListWidget::AddItem

void ListWidget::AddItem( const char* item )
{
    Rct ext = GetExtents();
    float h = 2.0f;
    float itemH = 13;
    for (int i = 0; i < NumChildren(); i++) 
    {
        itemH = GetChild( i )->GetExtents().h; 
        h += itemH;
    }
    
    if (m_bAutoHeight) ext.h = h + itemH + 2; 
    Label* pItem = new Label();
    pItem->SetExtents( 0, h, ext.w, itemH );
    pItem->SetDraggable( false );
    pItem->SetText( item );

    AddChild( pItem );
    SetExtents( ext );
} // ListWidget::AddItem

void ListWidget::Render()
{
    int nItems = NumChildren();
    Rct ext = GetScreenExtents();
    FlushText();
    rsFlush();
    rsInnerPanel( ext, 0.0f, m_BgColor );
    int cY = 2;

    if (m_SelectedItem != -1)
    {
        Widget* pItem = GetChild( m_SelectedItem );
        if (pItem)
        {
            pItem->SetTextColor( m_SelTextColor );
            pItem->SetBgColor( m_SelBgColor );
        }
    }

    for (int i = 0; i < nItems; i++)
    {
        Widget* pItem = GetChild( i );
        
        if (m_SelectedItem == i)
        {
            pItem->SetTextColor( m_SelTextColor );
            pItem->SetBgColor( m_SelBgColor );
        }
        else
        {
            pItem->SetTextColor( m_TextColor );
            pItem->SetBgColor( m_BgColor );
        }

        int h = pItem->GetExtents().h;
        pItem->SetExtents( 2.0f, cY, ext.w - 4, h );
        cY += h;
    }
    rsFlush();
    FlushText();
} // ListWidget::Render

void ListWidget::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "ListWidget", this );
 } // ListWidget::Expose

int ListWidget::PickItem( int mX, int mY )
{
    for (int i = 0; i < NumChildren(); i++)
    {
        if (GetChild( i )->GetExtents().PtIn( mX, mY )) return i;
    }
    return -1;
} // ListWidget::PickItem

bool ListWidget::OnMouseMove( int mX, int mY, DWORD keys )
{
    int idx = PickItem( mX, mY );
    if (idx == -1) return false;
    SelectItem( idx );
    return false;
} // ListWidget::OnMouseMove

bool ListWidget::OnMouseLBDown( int mX, int mY )
{
    int idx = PickItem( mX, mY );
    if (idx == -1) return false;
    SelectItem( idx );
    SetText( GetChild( idx )->GetText() );
    sigSelChanged();
    return true;
} // ListWidget::OnMouseLBDown


