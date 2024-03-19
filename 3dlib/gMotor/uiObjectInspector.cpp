/*****************************************************************************/
/*    File:    uiObjectInspector.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-13-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kPropertyMap.h"
#include "IMediaManager.h"

#include "kHose.h"

#include "uiWidget.h"
#include "uiWindow.h"
#include "uiEditBox.h"
#include "uiCheckBox.h"
#include "uiButton.h"
#include "uiPropEditors.h"
#include "uiObjectInspector.h"
#include "uiExtendedEdit.h"
#include "uiComboBox.h"

/*****************************************************************************/
/*    InspectorItem implementation
/*****************************************************************************/
InspectorItem::InspectorItem()
{
    m_Extents.Zero();
    m_IndexInMap        = -1;
    m_bSelected         = false;
    m_FgColor           = 0xFF000000;
    m_BgColor           = 0xFFD6D3CE;
    m_pEditor           = NULL;
    m_pMember           = NULL;
}; // class InspectorItem

/*****************************************************************************/
/*    ObjectInspector implementation
/*****************************************************************************/
IMPLEMENT_CLASS(ObjectInspector);
ObjectInspector::ObjectInspector()
{
    m_DefItemHeight         = 11;
    m_pExposed              = NULL;

    m_FgColor               = 0xFF000000;
    m_BgColor               = 0xCCD6D3CE;
    m_BgAltColor            = 0xCC9999A9;
    m_SelBgColor            = 0xFF6464FF;
    m_SelFgColor            = 0xFFFFFFFF;
    m_DisabledFgColor       = 0xFFCCCCCF;
    m_LinesColor1           = 0xFF303030;
    m_LinesColor2           = 0xFFFFFFFF;

    m_SectionFgColor        = 0xFFFFFF88;
    m_SectionBgColor        = 0x66969ED6;
    m_MinColWidth           = 200.0f;
    m_MaxColWidth           = 300.0f;

    m_bShowParent           = true;    

    m_Extents.w             = m_MinColWidth*2.0f;
    m_Extents.h             = 100.0f;
    SetName( "ObjectInspector" );
} // ObjectInspector::ObjectInspector

ObjectInspector::~ObjectInspector()
{
}

void ObjectInspector::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "ObjectInspector", this );
    pm.f( "DefItemHeight",      m_DefItemHeight        );
    pm.f( "BgColor",            m_BgColor,           "color" );
    pm.f( "FgColor",            m_FgColor,           "color" );
    pm.f( "SelBgColor",         m_SelBgColor,        "color" );
    pm.f( "SelFgColor",         m_SelFgColor,        "color" );
    pm.f( "DisabledFgColor",    m_DisabledFgColor,   "color" );
    pm.f( "BgAltColor",         m_BgAltColor,        "color" );
    pm.f( "LinesColor1",        m_LinesColor1,       "color" );
    pm.f( "LinesColor2",        m_LinesColor2,       "color" );
    pm.f( "SectionFgColor",     m_SectionFgColor,    "color" );
    pm.f( "SectionBgColor",     m_SectionBgColor,    "color" );
    pm.f( "MaxColWidth",        m_MaxColWidth );
    pm.f( "ShowParentMembers",  m_bShowParent );
} // ObjectInspector::CreatePropertyMap

void ObjectInspector::Bind( IReflected* pExposed )
{
    if (pExposed == m_pExposed) return;
    m_Map.Clear();
    if(pExposed) pExposed->Expose( m_Map );
    m_pExposed = pExposed;
    UpdateItems();
} // ObjectInspector::Bind

const char* ObjectInspector::GetEditorWidgetType( const char* propType ) const
{
    if (!propType || propType[0] == 0) return "EditBox";
    if (propType[0] == '#') return "ExtendedEdit";
    if (!stricmp( propType, "bool"               ))    return "BoolPropEditor";
    if (!stricmp( propType, "cstring"            ))    return "EditBox";            
    if (!stricmp( propType, "int"                ))    return "IntEditBox";
    if (!stricmp( propType, "float"              ))    return "FloatEditBox";
    if (!stricmp( propType, "double"             ))    return "FloatEditBox";
    if (!stricmp( propType, "method"             ))    return "MethodButton";
    if (!stricmp( propType, "color"              ))    return "ColorBox";
    if (!stricmp( propType, "enum"               ))    return "EnumEditor";
    if (!stricmp( propType, "direction"          ))    return "EnumEditor";
    if (!stricmp( propType, "floatAnimCurve"     ))    return "FloatCurveEditor";
    if (!stricmp( propType, "quatAnimCurve"      ))    return "QuatCurveEditor";
    if (!stricmp( propType, "colorAnimCurve"     ))    return "ColorCurveEditor";
    if (!stricmp( propType, "ColorRamp"          ))    return "ColorRampEdit";
    if (!stricmp( propType, "AlphaRamp"          ))    return "AlphaRampEdit";
    if (!strnicmp( propType, "enum_", 5          ))    return "ComboWidget";
    return "EditBox";
} // ObjectInspector::GetEditorWidgetType

void ObjectInspector::GetProperties()
{
    int nItems = m_Items.size();
    if (nItems == 0) return;
    for (int i = 0; i < nItems; i++)
    {
        InspectorItem& item = m_Items[i];
        if (item.m_pEditor)
        {
            bool res = item.m_pMember->FromString( m_Map.GetObject(), item.m_pEditor->GetText() );
        }
    }
} // ObjectInspector::GetProperties

void ObjectInspector::ClearItems()
{
    int nItems = m_Items.size();
    for (int i = 0; i < nItems; i++)
    {
        InspectorItem& item = m_Items[i];
        HoseDepot::instance().UnregisterObject( item.m_pEditor );
        delete item.m_pEditor;
    }
    m_Items.clear();
} // ObjectInspector::ClearItems

void ObjectInspector::UpdateItems()
{
    ClearItems();
    Rct rct    = GetScreenExtents();
    
    ClearChildren();
    int cIdx = 0;
    for (int i = 0; i < m_Map.GetNMembers(); i++)
    {
        m_Items.push_back( InspectorItem() );
        InspectorItem& item = m_Items.back();
        ClassMember* pProp  = m_Map.GetMember( i );
        item.m_Caption        = pProp->GetName();
        item.m_IndexInMap    = cIdx++;
        item.m_pMember        = pProp;

        item.m_FgColor        = item.m_pMember->IsReadonly() ? m_DisabledFgColor : m_FgColor;        
        if (cIdx&1) item.m_BgColor = m_BgAltColor; else item.m_BgColor = m_BgColor;

        //  assign editor
        const char* type = GetEditorWidgetType( pProp->GetTypeName() );
        Widget* pEditor = (Widget*)ObjectFactory::instance().Create( type );
        if (!pEditor) continue;
        pEditor->SetName( item.m_pMember->GetName() );
        pEditor->SetEnabled( !pProp->IsDisabled() && !pProp->IsReadonly() );

        //  establish two-directional data links
        HoseDepot::instance().AddHose( m_pExposed, pProp->GetName(), pEditor, "text" );
        HoseDepot::instance().AddHose( pEditor, "text", m_pExposed, pProp->GetName() );

        if (!strcmp( type, "ExtendedEdit" ))
        {
            ExtendedEdit* pExEdit = dynamic_cast<ExtendedEdit*>( pEditor );
            if (pExEdit) pExEdit->SetExtendedType( pProp->GetTypeName() + 1 );
        }

        if (!strcmp( type, "ComboWidget" ))
        {
            ComboWidget* pCBox = dynamic_cast<ComboWidget*>( pEditor );
            if (pCBox) 
            {
                int cVal = 0;
                pProp->Get( m_pExposed, cVal );
                int cID = 0;
                while (true) 
                {
                    const int c_BufSize = 256;
                    char buf[c_BufSize];
                    if (!pProp->Set( m_pExposed, cID )) break;
                    if (pProp->ToString( m_pExposed, buf, c_BufSize ))
                    {
                        pCBox->AddItem( buf );
                    }
                    else if (cID > 0) break;
                    cID++;
                }
                pProp->Set( m_pExposed, cVal );
            }
        }

        if (!stricmp( pProp->GetTypeName(), "method" ))
        {
            MethodButton* pMB = (MethodButton*)pEditor;
            pMB->SetText( pProp->GetName() );
            MethodFunctor func( m_pExposed, pProp );
            pMB->sigPressed.Connect( func );
        }
        pEditor->SetDraggable( false );

        item.m_pEditor = pEditor;
        AddChild( pEditor );
    }
    
    float leftColW    = GetLeftColWidth();
    float rightColW = GetRightColWidth();
    
    float cY = 0.0f;
    float cX = 0.0f;
    float cW = rct.w;

    for (int i = 0; i < m_Map.GetNMembers(); i++)
    {
        InspectorItem& item = m_Items[i];
        item.m_Extents.y = cY;
        item.m_Extents.x = 0;
        item.m_Extents.h = m_DefItemHeight;
        item.m_Extents.w = leftColW;
        if (item.m_pEditor) 
        {
            item.m_pEditor->SetExtents( Rct( leftColW + 2, cY, rightColW, m_DefItemHeight ) );
            item.m_Extents.h = item.m_pEditor->GetExtents().h;
        }
        cY += item.m_Extents.h;
    }
    SetHeight( cY );
} // ObjectInspector::UpdateItems

int    ObjectInspector::GetSelectedItem()
{
    for (int i = 0; i < m_Items.size(); i++)
    {
        if (m_Items[i].m_bSelected) return i;
    }
    return -1;
} // ObjectInspector::GetSelectedItem

float ObjectInspector::GetLeftColWidth()
{
    float w = GetMaxCaptionWidth();
    w += 2.0f;
    if (w > m_MaxColWidth) w = m_MaxColWidth;
    if (w < m_MinColWidth) w = m_MinColWidth;
    return w;
} // ObjectInspector::GetLeftColWidth

float ObjectInspector::GetRightColWidth() 
{
    float w = m_MinColWidth;
    w += 2.0f;
    if (w > m_MaxColWidth) w = m_MaxColWidth;
    if (w < m_MinColWidth) w = m_MinColWidth;
    return w + m_DefItemHeight;
} // ObjectInspector::GetRightColWidth

float ObjectInspector::GetMaxCaptionWidth() 
{
    float maxW = 0.0f;
    for (int i = 0; i < m_Items.size(); i++)
    {
        float curW = GetTextWidth( m_Items[i].m_Caption.c_str() );
        if (i == 0 || curW > maxW)
        {
            maxW = curW;
        }
    }
    return maxW;
} // ObjectInspector::GetMaxCaptionWidth

void ObjectInspector::ClearSelection()
{
    for (int i = 0; i < m_Items.size(); i++)
    {
        InspectorItem& item = m_Items[i];
        if (item.m_bSelected)
        {
            if (item.m_pEditor) 
            {
                item.m_pEditor->SetFocus( false );
            }
            item.m_bSelected = false;
        }
    }
} // ObjectInspector::GetSelectedItem

bool ObjectInspector::OnMouseLBDown( int mX, int mY )                
{ 
    if (!PtIn( mX, mY )) return false;
    float mx = mX;
    float my = mY;

    InspectorItem* pItem = GetItemByPt( mx, my );
    ClearSelection();
    if (pItem && mx > GetMaxCaptionWidth())
    //  select item, activate editor
    {
        if (!pItem->m_bSelected && pItem->m_pMember && !pItem->m_pMember->IsReadonly()) 
        {
            pItem->m_bSelected = true;
            if (pItem->m_pEditor) pItem->m_pEditor->SetFocus();
        }
    }    
    return false;
} // ObjectInspector::OnMouseLBDown

bool ObjectInspector::OnKeyDown( DWORD keyCode, DWORD flags )
{
    int nItems = GetNItems();
    if (keyCode == VK_DOWN) 
    {
        int idx = GetSelectIdx();
        if (idx >= 0) SetSelectIdx( (idx + 1) % nItems);
        return true;
    }
    if (keyCode == VK_UP)    
    {
        int idx = GetSelectIdx();
        if (idx >= 0) SetSelectIdx( (idx + nItems - 1) % nItems);
        return true;
    }
    return false;
} // ObjectInspector::OnKeyDown

int ObjectInspector::GetSelectIdx() const
{
    int cIdx = 0;
    for (int i = 0; i < m_Items.size(); i++)
    {
        const InspectorItem& item = m_Items[i];
        if (item.m_bSelected) return cIdx;
        cIdx++;
    }
    return -1;
} // ObjectInspector::GetSelectIdx

void ObjectInspector::SetSelectIdx( int idx ) 
{
    int cIdx = 0;
    for (int i = 0; i < m_Items.size(); i++)
    {
        InspectorItem& item = m_Items[i];
        if (cIdx == idx)
        {
            //if (item.m_pMember->IsReadonly()) return;
            ClearSelection();
            item.m_bSelected = true;
             if (item.m_pEditor) item.m_pEditor->SetFocus();
        }
        cIdx++;
    }
} // ObjectInspector::GetSelectIdx

InspectorItem* ObjectInspector::GetItemByPt( float pX, float pY )
{
    for (int i = 0; i < m_Items.size(); i++)
    {
        InspectorItem& item = m_Items[i];
        if (item.m_Extents.PtIn( pX, pY )) return &item;
    }
    return NULL;
} // ObjectInspector::GetItemByPt

void ObjectInspector::OnInit()
{
    
} // ObjectInspector::OnInit

void ObjectInspector::Render()
{
    int nItems = m_Items.size();
    if (nItems == 0) return;

    Rct ext = GetScreenExtents();
    
    float leftColW    = GetLeftColWidth();
    float rightColW = GetRightColWidth();

    SetWidth( leftColW + rightColW + 4.0f );

    for (int i = 0; i < nItems; i++)
    {
        InspectorItem& item = m_Items[i];
        item.m_Extents.w = leftColW + rightColW + 2.0f;
        DWORD fgColor = item.m_FgColor;
        DWORD bgColor = item.m_BgColor;

        if (item.m_bSelected) 
        {
            bgColor = m_SelBgColor;
            fgColor = m_SelFgColor;
        }
        
        Rct iext = item.m_Extents;
        iext.x += ext.x;
        iext.y += ext.y;
        Vector3D rpos( iext.x, iext.y, 0.0f );
        //  draw item background
        if (bgColor != GetBgColor()) rsRect( iext, 0.0f, bgColor );
        //  draw item caption text
        DrawText( rpos.x + 2, rpos.y, fgColor, item.m_Caption.c_str() );
    }
    
    rsLine( ext.x + leftColW, ext.y, ext.x + leftColW, ext.GetBottom(), 0.0f, m_LinesColor1 );
    rsLine( ext.x + leftColW + 1, ext.y, ext.x + leftColW + 1, ext.GetBottom(), 0.0f, m_LinesColor2 );
    
    rsPanel( Rct( ext.x - 2, ext.y - 2, ext.w + 2, ext.h + 4 ), 0.0f, 0xFFCCCCCC, 0, 0xFF848284 );
    rsFlush();
    FlushText();
} // ObjectInspector::Render


