/*****************************************************************************/
/*	File:	d3dFont.h
/*  Desc:	Font interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#ifndef __D3DFONT_H__
#define __D3DFONT_H__
#include "IFont.h"

/*****************************************************************************/
/*  Class:  FontDX9
/*  Desc:   Direct3D9 font class 
/*****************************************************************************/
class FontDX9 : public IFont
{
    IDirect3DDevice9*       m_pDevice;          //  device interface
    ID3DXFont*              m_pFont;            //  dx font interface

    std::string             m_Name;
    int                     m_ID;

public:
                            FontDX9         ( IDirect3DDevice9* pDevice = NULL );
                            ~FontDX9        ();

    virtual const char*     GetName         () const { return m_Name.c_str(); }
    virtual int             GetID           () const { return m_ID; }
    virtual void            SetID           ( int id ) { m_ID = id; }

    virtual bool		    Create		    ( const char* name, int height, DWORD charset = DEFAULT_CHARSET, 
                                                bool bBold = false, bool bItalic = false );
    virtual bool		    Create          ( const char* texName, int charW, int charH );
    virtual int		        GetStringWidth	( const char* str, int spacing = 1 );
    virtual int		        GetCharWidth	( BYTE ch );
    virtual int		        GetCharHeight	( BYTE ch );

    virtual bool	        DrawString		( const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 );
    virtual bool	        DrawString3D	( const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 );
    virtual bool	        DrawChar		( const Vector3D& pos, BYTE ch, DWORD color = 0xFFFFFFFF );
    virtual bool	        DrawChar		( const Vector3D& pos, const Rct& uv, DWORD color = 0xFFFFFFFF );
    virtual bool	        DrawChar		( const Vector3D& pos, const Rct& uv, float w, float h, DWORD color = 0xFFFFFFFF );
    virtual void	        Flush		    ();	

    virtual void            DeleteDeviceObjects     ();
    virtual void            InvalidateDeviceObjects ();
    virtual void            RestoreDeviceObjects    ();
   
}; // class FontDX9

#endif // __D3DFONT_H__