/*****************************************************************************/
/*    File:    uiComboBox.cpp
/*    Desc:    ComboWidget control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"

#include "uiButton.h"
#include "uiListBox.h"
#include "uiEditBox.h"
#include "uiComboBox.h"
#include "IFontManager.h"

/*****************************************************************************/
/*  ComboWidget implementation
/*****************************************************************************/
IMPLEMENT_CLASS(ComboWidget);
ComboWidget::ComboWidget()
{
    AddChild( m_Thumb       );
    AddChild( m_DropList    );
    AddChild( m_Selection   );
    m_DropList.SetName      ( "DropList"    );
    m_Selection.SetName     ( "Selection"   );
    m_DropList.SetAutoHeight( true );
    
    m_Thumb.SetName         ( "Thumb"       );
    m_Thumb.SetSpritePack   ( "Scripts\\kangaroo_interface" );
    m_Thumb.SetSpriteID     ( 6 );

    m_DropList.SetFocusable ( false );
    m_Selection.SetFocusable( false );

    m_bDropActive   = false;
    m_ThumbWidth    = 13;

    m_Thumb.sigPressed.Connect( this, &ComboWidget::OnDropDown );
    m_DropList.sigSelChanged.Connect( this, &ComboWidget::OnSelectItem );
    sigUnfocus.Connect( this, &ComboWidget::HideDropList );
} // ComboWidget::ComboWidget

void ComboWidget::Render()
{
    m_DropList.SetVisible( m_bDropActive );
    int bX = m_Extents.w - m_ThumbWidth;
    int thumbH = m_Extents.h;
    m_Thumb.SetExtents      ( bX, 0, m_ThumbWidth, thumbH );
    m_Selection.SetExtents  ( 0, 0, bX, m_Extents.h );
    
    float h = m_DropList.GetExtents().h;
    m_DropList.SetExtents   ( Rct( 0.0f, thumbH - 1, m_Extents.w, h ) );
    m_Selection.SetEnabled  ( IsEnabled() );
    m_Thumb.SetVisible      ( IsEnabled() );
    m_Selection.SetText     ( m_Text.c_str() );
} // ComboWidget::Render

void ComboWidget::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "ComboWidget", this );
    pm.f( "ThumbWidth", m_ThumbWidth );
    pm.f( "DropActive", m_bDropActive );
} // ComboWidget::Expose

void ComboWidget::OnDropDown()
{
    m_bDropActive = !m_bDropActive;
    SetFocus();
} // ComboWidget::OnDropDown

void ComboWidget::AddItem( const char* item )
{
    m_DropList.AddItem( item );
} // ComboWidget::AddItem

void ComboWidget::HideDropList() 
{ 
    m_bDropActive = false;
} // ComboWidget::HideDropList

void ComboWidget::OnSelectItem()
{
    SetText( m_DropList.GetText() );
    HideDropList();
} // ComboWidget::OnSelectItem

bool ComboWidget::OnMouseLBDown( int mX, int mY ) 
{ 
    if (!PtIn( mX, mY )) 
    {
        HideDropList();
        return false;
    }
    SetFocus();
    OnDropDown();
    return true;
} // ComboWidget::OnMouseLBDown



