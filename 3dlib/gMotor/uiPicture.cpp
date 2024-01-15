/*****************************************************************************/
/*    File:    uiPicture.cpp
/*    Desc:    Picture UI element
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "uiPicture.h"


/*****************************************************************************/
/*    Picture implementation
/*****************************************************************************/
Picture::Picture() 
            :    m_ID        (-1), 
                m_bStretch    (false), 
                m_Z            (0.0f), 
                m_Color        (0xFFFFFFFF),
                m_bFilter    (false)
{ 
    m_Ext.Zero(); 
}

Picture::~Picture()
{
    IPM->UnloadImage( m_ID );
}

void Picture::Render()
{
    if (m_ID == -1)
    {
        m_ID = IPM->GetImageID( m_File.c_str() );
        if (m_ID == -1) return;
    }
    IPM->SetDiffuse( m_Color );
    IPM->SetFiltering( m_bFilter );
    if (m_bStretch) IPM->DrawImage( m_ID, m_Ext, m_Z );
    else IPM->DrawImage( m_ID, m_Ext.x, m_Ext.y, m_Z );
} // Picture::Render

void Picture::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_File << m_Ext << m_bStretch << m_Z << m_Color << m_bFilter;
}

void Picture::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_File >> m_Ext >> m_bStretch >> m_Z >> m_Color >> m_bFilter;
}

void Picture::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Picture", this );
    pm.f( "ID", m_ID, NULL, true );
    pm.p ( "File", &Picture::GetFile, &Picture::SetFile, "file" );
    pm.f( "x",                m_Ext.x         );
    pm.f( "y",                m_Ext.y         );
    pm.f( "w",                m_Ext.w         );
    pm.f( "h",                m_Ext.h         );
    pm.f( "Stretch",        m_bStretch        );
    pm.f( "Z",                m_Z                );
    pm.f( "Color",            m_Color, "color");
    pm.f( "Filter",            m_bFilter        );

    pm.p ( "InitWidth", &Picture::GetInitialWidth  );
    pm.p ( "InitHeight", &Picture::GetInitialHeight );
    pm.p ( "FileFormat", &Picture::GetFileFormat     );
    pm.p ( "ColorFormat", &Picture::GetColorFormat     );
    pm.p ( "Valid", &Picture::IsValid             );
    pm.p ( "Loaded", &Picture::IsLoaded         );

    pm.m( "Load", &Picture::LoadImage        );
    pm.m( "Unload", &Picture::UnloadImage        );
} // Picture::Expose

void Picture::UnloadImage()
{
    IPM->UnloadImage( m_ID );
} // Picture::UnloadImage

void Picture::LoadImage()
{
    IPM->LoadImage( m_ID );
} // Picture::LoadImage

int    Picture::GetInitialWidth() const
{
    return IPM->GetWidth( m_ID );
}

int    Picture::GetInitialHeight() const
{
    return IPM->GetHeight( m_ID );
}

void Picture::SetFile( const char* file )
{
    m_File = file;
    m_ID = -1;
} // Picture::SetFile

PictureFileFormat Picture::GetFileFormat() const
{
    return IPM->GetFileFormat( m_ID );
}

ColorFormat Picture::GetColorFormat() const
{
    return IPM->GetColorFormat( m_ID );
}

bool Picture::IsLoaded() const
{
    return IPM->IsLoaded( m_ID );
}

bool Picture::IsValid() const
{
    return IPM->IsValid( m_ID );
}


