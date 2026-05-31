/*****************************************************************************/
/*    File:    uiRatchet.cpp
/*    Desc:    Ratchet control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiRatchet.h"

/*****************************************************************************/
/*  Ratchet implementation
/*****************************************************************************/
int Ratchet::s_SpriteID = -1;
int Ratchet::s_NFrames  = 1;
IMPLEMENT_CLASS(Ratchet);
Ratchet::Ratchet()
{
    m_bChanging = false;
    m_MouseX = 0;
    m_Position = 0.0f;
    m_PrevPosition = 0.0f;
    m_Frame = 0;
    m_TextColor = 0xFFFFFFFF;
} // Ratchet::Ratchet

void Ratchet::Render()
{
    if (s_SpriteID == -1) 
    {
        s_SpriteID = ISM->GetPackageID( "ratchet" );
        s_NFrames = ISM->GetNFrames( s_SpriteID );
    }
    Rct ext = GetScreenExtents();
    ISM->SetCurrentDiffuse( GetTextColor() );
    ISM->DrawSprite( s_SpriteID, m_Frame, Vector3D( ext.x, ext.y, 0.0f ) );
    ISM->Flush();
} // Ratchet::Render

void Ratchet::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "Ratchet", this );
} // Ratchet::Expose

void Ratchet::AddValue( int delta )
{
    if (delta == 0) return;
    m_PrevPosition = m_Position;
    m_Position += float( delta )*0.5f;
    float frameDelta = -float( delta )*0.5f;
    m_Frame = m_Frame + frameDelta;
    if (m_Frame < 0) m_Frame = s_NFrames - 1;
    if (m_Frame >= s_NFrames) m_Frame = 0;
    sigChanged();
} // Ratchet::AddValue

bool Ratchet::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (m_bChanging)
    {
        if (GetKeyState( VK_LBUTTON ) >= 0)
        {
            m_bChanging = false;
            return true;
        }
        AddValue( mX - m_MouseX );
        m_MouseX = mX;
        return true;
    }
    return false;
} // Ratchet::OnMouseMove

bool Ratchet::OnMouseLBDown( int mX, int mY )
{
    if (!PtIn( mX, mY )) return false;
    m_bChanging = true;
    m_MouseX = mX;
    m_PrevPosition = m_Position;
    return true;
} // Ratchet::OnMouseLBDown

bool Ratchet::OnMouseLBUp( int mX, int mY )
{
    m_bChanging = false;
    return false;
} // Ratchet::OnMouseLBUp

