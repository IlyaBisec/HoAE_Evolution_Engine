/*****************************************************************************/
/*    File:    uiPropEditors.cpp
/*    Desc:    Standard set of property editors realization
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-13-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kPropertyMap.h"
#include "kSystemDialogs.h"
#include "kFilePath.h"
#include "uiWindow.h"
#include "uiButton.h"
#include "uiEditBox.h"
#include "uiCheckBox.h"
#include "uiPropEditors.h"

/*****************************************************************************/
/*    BoolPropEditor implementation
/*****************************************************************************/
IMPLEMENT_CLASS(BoolPropEditor);
BoolPropEditor::BoolPropEditor()
{
    m_bShowText = false;
    sigTextChanged.Connect( this, &BoolPropEditor::OnChangeText );
    sigChecked.Connect( this, &BoolPropEditor::ChangeText );
    sigUnchecked.Connect( this, &BoolPropEditor::ChangeText );
} // BoolPropEditor::BoolPropEditor

void BoolPropEditor::OnChangeText()
{
    if (!stricmp( m_Text.c_str(), "true" ) ) 
    {
        Check(); 
    }
    else 
    {
        Uncheck();
    }
}

void BoolPropEditor::ChangeText()
{
    if (IsChecked()) 
    {
        m_Text = "true"; 
    }
    else 
    {
        m_Text = "false";
    }
}

void BoolPropEditor::Render()
{
    Rct ext = GetExtents();
    Rct nExt = GetExtents();
    float w = ext.w*0.25f;
    nExt.y += 1;
    nExt.w -= 4;
    nExt.h -= 4;
    SetExtents( nExt );
    CheckButton::Render();
    SetExtents( ext );
} // BoolPropEditor::Render

/*****************************************************************************/
/*    MethodButton implementation
/*****************************************************************************/
IMPLEMENT_CLASS(MethodButton);
void MethodButton::Render()
{
    Rct ext = GetExtents();
    Rct nExt = GetExtents();
    /*float w = ext.w*0.25f;
    nExt.x += w;
    nExt.w = w*2.0f;*/
    SetExtents( nExt );
    Button::Render();
    SetExtents( ext );
} // 

