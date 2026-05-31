/*****************************************************************************/
/*    File:    kSystemDialogs.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    25.02.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kSystemDialogs.h"

void RelaxDialog()
{
    MSG msg;
    while (PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    Sleep( 100 );
} // RelaxDialog

/*****************************************************************************/
/*    FileDialog implementation
/*****************************************************************************/
FileDialog::FileDialog()
{
    memset( &m_Ofs, 0, sizeof(m_Ofs) );
    m_Ofs.lStructSize    = sizeof( OPENFILENAME );
    m_Ofs.lpstrFile        = m_FileName;
    m_Ofs.nMaxFile        = _MAX_PATH;
    m_Ofs.lpstrFilter    = m_Filter;
	m_Ofs.hwndOwner	= IRS->GetHWND();

    m_nFilterChars        = 0;
    m_Filter[0]            = 0;
    m_FileName[0]        = 0;	
}

void FileDialog::AddFilter( const char* descr, const char* filter )
{
    strcpy( &m_Filter[m_nFilterChars], descr );
    m_nFilterChars += strlen( descr ) + 1;
    strcpy( &m_Filter[m_nFilterChars], filter );
    m_nFilterChars += strlen( filter ) + 1;
    m_Filter[m_nFilterChars] = 0;
} // FileDialog::AddFilter

const char* FileDialog::GetFilePath()
{
    return m_FileName;
}

void FileDialog::SetDefaultExtension( const char* ext )
{
    strcpy( m_DefExt, ext );
    m_Ofs.lpstrDefExt = m_DefExt;
} // FileDialog::SetDefaultExtension

/*****************************************************************************/
/*    OpenFileDialog implementation
/*****************************************************************************/
OpenFileDialog::OpenFileDialog()
{
}

OpenFileDialog::~OpenFileDialog()
{
}

bool OpenFileDialog::Show()
{
    char cdir[_MAX_PATH];
    _getcwd( cdir, _MAX_PATH );
    m_Ofs.lpstrInitialDir = cdir;
    BOOL res = GetOpenFileName( &m_Ofs );
    return res != FALSE;
    return false;
}

/*****************************************************************************/
/*    SaveFileDialog implementation
/*****************************************************************************/
SaveFileDialog::SaveFileDialog()
{
}

SaveFileDialog::~SaveFileDialog()
{
}


bool SaveFileDialog::Show()
{
    char cdir[_MAX_PATH];
    _getcwd( cdir, _MAX_PATH );
    m_Ofs.lpstrInitialDir = cdir;
    BOOL res = GetSaveFileName( &m_Ofs );
    return res != FALSE;
}

/*****************************************************************************/
/*    PickColorDialog implementation
/*****************************************************************************/
COLORREF PickColorDialog::s_CustomColors[16] = 
{
     RGB(0x00, 0x00, 0x00),
     RGB(0xA5, 0x2A, 0x00),
     RGB(0x4B, 0x00, 0x82),
     RGB(0x28, 0x28, 0x28),

     RGB(0xFF, 0x68, 0x20),
     RGB(0x00, 0x93, 0x00),
     RGB(0x38, 0x8E, 0x8E),
     RGB(0x00, 0x00, 0xFF),

     RGB(0xFF, 0x00, 0x00),
     RGB(0x32, 0xCD, 0x32),
     RGB(0x7F, 0xFF, 0xD4),
     RGB(0x80, 0x00, 0x80),

     RGB(0xFF, 0xFF, 0x00),   
     RGB(0xC0, 0xC0, 0xC0),
     RGB(0xFF, 0xE4, 0xE1),
     RGB(0xFF, 0xFF, 0xFF)
};

PickColorDialog::PickColorDialog( DWORD color ) 
{
    memset( &m_ChooseColor, 0, sizeof(m_ChooseColor) );
    m_ChooseColor.lStructSize = sizeof(m_ChooseColor);
    //  dialog flags
    m_ChooseColor.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
    m_ChooseColor.lpCustColors = s_CustomColors;
	m_ChooseColor.hwndOwner = IRS->GetHWND();	
    SetColor( color );
}

void PickColorDialog::SetHookProcedure( LPCCHOOKPROC proc )
{
    m_ChooseColor.lpfnHook = proc;
    m_ChooseColor.Flags |= CC_ENABLEHOOK;
}

PickColorDialog::~PickColorDialog()
{
}

bool PickColorDialog::Show()
{
    BOOL res = ChooseColor( &m_ChooseColor );
    return (res != FALSE);
}

DWORD PickColorDialog::FromColorRef( COLORREF clr )
{
    return 0xFF000000 | ((clr & 0x000000FF) << 16) | ((clr & 0x00FF0000) >> 16) | (clr & 0x0000FF00);
}

COLORREF PickColorDialog::ToColorRef( DWORD clr )
{
    return 0xFF000000 | ((clr & 0x000000FF) << 16) | ((clr & 0x00FF0000) >> 16) | (clr & 0x0000FF00);
}
