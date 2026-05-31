/*****************************************************************************/
/*    File:    uiEditBox.cpp
/*    Desc:    EditBox control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiEditBox.h"

/*****************************************************************************/
/*  EditBox implementation
/*****************************************************************************/
IMPLEMENT_CLASS(EditBox);
DWORD EditBox::s_BlinkStart = 0;

EditBox::EditBox()
{
    m_BgColor           = 0x0;
    m_FocusedColor      = 0xAAFFFFFF;
    m_ClrCaret          = 0xFF000000;
    m_CaretBlinkOn      = 800;
    m_CaretBlinkOff     = 600;
    m_CaretPos          = 0;

    m_SelStart          = 0;
    m_SelEnd            = 0;
    m_SelColor          = 0xAA3333FF;
    m_bEditable         = true;
    SetClippedToExt( true );
} // EditBox::EditBox

void EditBox::Render()
{
    DWORD bgColor = HasFocus() ? m_FocusedColor : m_BgColor;
    Rct ext = GetScreenExtents();

    Rct vp = IRS->GetViewPort();
    rsFlushPoly2D();
    FlushText();
    if (HasFocus())
    {
        rsPanel( ext, 0.0f, 0xFF848284, bgColor, 0xFFFFFFFF );
    }
    else
    {
        rsRect( ext, 0.0f, bgColor );
    }

    //  draw selection rectangle
    if (m_SelStart != m_SelEnd && HasFocus())
    {
        static std::string str;
        str = std::string( m_Text, 0, m_SelStart );  
        float selBeg = GetTextWidth( str.c_str() );
        str = std::string( m_Text, 0, m_SelEnd );  
        float selEnd = GetTextWidth( str.c_str() );
        Rct rc( ext.x + selBeg, ext.y, selEnd - selBeg, ext.h );
        rsRect( rc, 0.0f, m_SelColor );
    }

    DWORD textColor = IsEnabled() ? m_TextColor : m_DisTextColor;
    DrawText( ext.x + 2, ext.y, textColor, GetText() );
    rsFlush();
    FlushText();

    if (HasFocus()) RenderCaret();
    rsFlush();
} // EditBox::Render

void EditBox::RenderCaret()
{
    DWORD tc = GetTickCount() - s_BlinkStart;
    Rct ext = GetScreenExtents();
    tc %= m_CaretBlinkOn + m_CaretBlinkOff;
    if (tc < m_CaretBlinkOn)
    {
        char c = m_Text[m_CaretPos];
        m_Text[m_CaretPos] = 0;
        float cpos = GetTextWidth( GetText() );
        m_Text[m_CaretPos] = c;
        rsLine( ext.x + cpos, ext.y + 1, ext.x + cpos, ext.y + GetTextHeight() - 1, 0.0f, m_ClrCaret );
    }
    rsFlush();
} // EditBox::RenderCaret

void EditBox::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "Slider", this );
    pm.f( "FocusedColor", m_FocusedColor );
} // EditBox::Expose

bool EditBox::OnFocus()
{
    if (!m_bEditable) return false;
    m_CaretPos = m_Text.size();
    ResetBlink();
    return false;
} // EditBox::OnFocus

bool EditBox::OnChar( DWORD charCode, DWORD flags )
{
    if (!m_bEditable || !IsEnabled()) return false;
    if (!HasFocus()) return false;

    if (isalpha( charCode ) || 
        isdigit( charCode ) || 
        charCode == '|' || 
        charCode == '.' || 
        charCode == '_' ||
        charCode == '-')
    {
        if (m_SelStart != m_SelEnd)
        {
            if (m_SelStart > m_SelEnd) std::swap( m_SelEnd, m_SelStart );
            m_Text.erase( m_SelStart, m_SelEnd - m_SelStart );
            m_CaretPos  = m_SelStart;
            m_SelEnd    = m_SelStart;
        }

        char str[2];
        str[0] = charCode; str[1] = 0;
        if (m_CaretPos <= m_Text.size()) m_Text.insert( m_CaretPos, str );
        m_CaretPos++;
        ResetBlink();
        sigTextChanged();
        return true;
    }
    return false;
} // EditBox::OnChar

bool EditBox::OnKeyDown( DWORD keyCode, DWORD flags )
{
    if (!m_bEditable || !IsEnabled()) return false;
    if (!HasFocus()) return false;
    if (keyCode == VK_RETURN/* || keyCode == VK_DOWN || keyCode == VK_UP*/) 
    {
        SetFocus( false );
        sigTextChanged();
        return true;
    }
    if (keyCode == VK_ESCAPE) 
    {
        SetFocus( false );
        return true;
    }
    if (keyCode == VK_DELETE)
    {
        if (m_SelStart != m_SelEnd)
        {
            if (m_SelStart > m_SelEnd) std::swap( m_SelEnd, m_SelStart );
            m_Text.erase( m_SelStart, m_SelEnd - m_SelStart );
            m_CaretPos  = m_SelStart;
            m_SelEnd    = m_SelStart;
        }
        else
        {
            m_Text.erase( m_CaretPos, 1 );
        }
        ResetBlink();
        sigTextChanged();
        return true;
    } 
    if (keyCode == VK_BACK)
    {
        if (m_CaretPos > 0) 
        {
            if (m_SelStart != m_SelEnd)
            {
                if (m_SelStart > m_SelEnd) std::swap( m_SelEnd, m_SelStart );
                m_Text.erase( m_SelStart, m_SelEnd - m_SelStart );
                m_CaretPos  = m_SelStart;
                m_SelEnd    = m_SelStart;
            }
            else
            {
                clamp( m_CaretPos, 0, (int)m_Text.size() );
                m_Text.erase( m_CaretPos - 1, 1 );
                m_CaretPos--;
                if (m_CaretPos < 0) m_CaretPos = 0;
            }
            sigTextChanged();
            ResetBlink();
        }
        return true;
    }
    if (keyCode == VK_LEFT)
    {
        m_CaretPos -= 1;
        if (m_CaretPos < 0) m_CaretPos = 0;
        ResetBlink();
        return true;
    }
    if (keyCode == VK_RIGHT)
    {
        m_CaretPos += 1;
        if (m_CaretPos > m_Text.size()) m_CaretPos = m_Text.size();
        ResetBlink();
        return true;
    }

    //  to prevent unfocused nodes processing char events
    if (isalpha( keyCode ) || 
        isdigit( keyCode ) || 
        keyCode == '|' || 
        keyCode == '.' || 
        keyCode == '_')
    {
        return true;
    }
    return false;
} // EditBox::OnKeyDown

bool EditBox::OnMouseLBDown( int mX, int mY )
{
    if (!PtIn( mX, mY ) || !IsEnabled()) 
    {
        SetFocus( false );
        return false;
    }
    if (!HasFocus()) 
    {
        SetFocus();
        return false;
    }

    int nCh = m_Text.size();
    char ch[2]; ch[1] = 0;
    int cX = 2;
    int cPos = 0;
    for (cPos = 0; cPos < nCh; cPos++)
    {
        if (mX <= cX) 
        {
            ResetBlink();
            break;
        }
        ch[0] = m_Text[cPos];
        cX += GetTextWidth( ch );
    }
    m_CaretPos = cPos;
    m_SelStart = m_CaretPos;
    m_SelEnd   = m_CaretPos; 

    ResetBlink();
    return true;
} // EditBox::OnMouseLBDown

bool EditBox::OnMouseLBDbl( int mX, int mY )
{
    if (!PtIn( mX, mY ) || !IsEnabled()) return false;
    m_SelStart = 0;
    m_SelEnd   = m_Text.size();
    return true;
} // EditBox::OnMouseLBDbl

bool EditBox::OnMouseLBUp( int mX, int mY )
{
    return false;
} // EditBox::OnMouseLBUp

bool EditBox::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (GetKeyState( VK_LBUTTON ) < 0 && HasFocus())
    {
        int nCh = m_Text.size();
        char ch[2]; ch[1] = 0;
        int cX = 2;
        int cPos = 0;
        for (cPos = 0; cPos < nCh; cPos++)
        {
            if (mX <= cX) break;
            ch[0] = m_Text[cPos];
            cX += GetTextWidth( ch );
        }
        m_SelEnd   = cPos; 
    }
    return false;
} // EditBox::OnMouseMove




