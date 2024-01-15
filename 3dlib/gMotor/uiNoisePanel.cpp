/*****************************************************************************/
/*    File:    uiNoisePanel.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiWindow.h"
#include "uiNoisePanel.h"
/*****************************************************************************/
/*    NoisePanel implementation
/*****************************************************************************/
NoisePanel::NoisePanel()
{
    m_TexSide   = 256;
    m_TexID     = -1;
}

void NoisePanel::Render()
{    
    if (m_TexID == -1)
    {
        m_TexID = IRS->CreateTexture( GetName(), m_TexSide, m_TexSide, cfA8, 1, tmpManaged, false );
        Update();
    }

} // NoisePanel::Render

void NoisePanel::Expose( PropertyMap& pm )
{
    pm.start( "NoisePanel", this );
} // NoisePanel::Expose

void NoisePanel::Update()
{
    
} // NoisePanel::Update

