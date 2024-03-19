/*****************************************************************************/
/*    File:    uiWidgetManager.h
/*    Desc:    Implementation of the ui manager
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#ifndef __UIWIDGETMANAGER_H__
#define __UIWIDGETMANAGER_H__

#include "IWidgetManager.h"


const int c_MaxFontName = 256;
const int c_MaxCharInst    = 4096;

enum BitmapFontType
{
    ftUnknown        = 0,
    ftGDIGenerated    = 1,
    ftLoaded        = 2,
    ftUniform        = 3
}; // enum BitmapFontType

/*****************************************************************************/
/*    Class:    BitmapFont
/*    Desc:    Bitmap font description
/*****************************************************************************/    
class BitmapFont
{    
public:
    BitmapFont();

protected:

    //  character
    struct Char
    {
        BYTE            m_Code;
        Rct                m_Ext;
        Rct                m_UV;
    }; // struct Char

    //  character instance
    struct CharInst
    {
        Rct                m_UV;
        DWORD            m_Color;
        Vector3D        m_Pos;
        float            w, h;
    }; // struct CharInst

    BitmapFontType        m_Type;
    Char                m_Char[256];
    int                    m_NChars;

    CharInst            m_CharInst[c_MaxCharInst];
    int                    m_NCharInst;

    BYTE                m_StartCode;
    int                    m_TexID;
    int                    m_TexWidth;
    int                    m_TexHeight;
    char                m_Name[c_MaxFontName];

    Rct                    m_FreeSpace;    //  rectangle of the unfilled texture
    Rct                    m_FreeLine;        //  unfilled rectangle to the end of the 

    
    int                    AllocateChar( BYTE code, float width, float heigth );
    bool                AddCharInst( BYTE code, const Vector3D& pos, DWORD color = 0xFFFFFFFF )
    {
        code -= m_StartCode;
        if (m_NCharInst == c_MaxCharInst) return false;
        CharInst& ch = m_CharInst[m_NCharInst++];
        ch.m_UV        = m_Char[code].m_UV;
        ch.m_Pos    = pos;
        ch.m_Color    = color;
        ch.w        = m_Char[code].m_Ext.w;
        ch.h        = m_Char[code].m_Ext.h;
        return true;
    }

    bool                AddCharInst( const Rct& uv, const Vector3D& pos, DWORD color = 0xFFFFFFFF )
    {
        if (m_NCharInst == c_MaxCharInst) return false;
        CharInst& ch = m_CharInst[m_NCharInst++];
        ch.m_UV        = uv;
        ch.m_Pos    = pos;
        ch.m_Color    = color;
        ch.w        = uv.w*m_TexWidth;
        ch.h        = uv.h*m_TexHeight;
        return true;
    }

    bool                AddCharInst( const Rct& uv, const Vector3D& pos, float w, float h, DWORD color = 0xFFFFFFFF )
    {
        if (m_NCharInst == c_MaxCharInst) return false;
        CharInst& ch = m_CharInst[m_NCharInst++];
        ch.m_UV        = uv;
        ch.m_Pos    = pos;
        ch.m_Color    = color;
        ch.w        = w;
        ch.h        = h;
        return true;
    }

    int GetCharW( BYTE code )
    {
        return m_Char[code - m_StartCode].m_Ext.w;
    }

    int GetCharH( BYTE code )
    {
        return m_Char[code - m_StartCode].m_Ext.h;
    }
    
    void                Flush();
    friend class WidgetManager;
}; // class BitmapFont


const int c_MaxFonts = 64;
/*****************************************************************************/
/*    Class:    WidgetManager
/*    Desc:    Implementation of the ui manager
/*****************************************************************************/    
class WidgetManager : public SNode, public IWidgetManager, public IDeviceClient
{
public:
                    WidgetManager        ();
                    ~WidgetManager        ();
    virtual void    OnDrop( int mX, int mY, DWORD ctx, DWORD obj );

    virtual int        GetFontID            ( const char* name );
    virtual void    DestroyFont            ( int fontID );
    virtual int        CreateFont            ( const char* name, int height, DWORD charset = DEFAULT_CHARSET, bool bBold = false, bool bItalic = false );
    virtual int        CreateUniformFont    ( const char* texName, int charW, int charH );
    virtual int        GetStringWidth        ( int fontID, const char* str, int spacing = 1 );
    virtual bool    DrawString            ( int fontID, const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 );
    virtual bool    DrawStringW            ( int fontID, const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 );
    virtual bool    DrawChar            ( int fontID, const Vector3D& pos, BYTE ch, DWORD color = 0xFFFFFFFF );
    virtual bool    DrawChar            ( int fontID, const Vector3D& pos, const Rct& uv, DWORD color = 0xFFFFFFFF );
    virtual bool    DrawChar            ( int fontID, const Vector3D& pos, const Rct& uv, 
                                            float w, float h, DWORD color = 0xFFFFFFFF );

    virtual int        GetCharWidth        ( int fontID, BYTE ch );
    virtual int        GetCharHeight        ( int fontID, BYTE ch );
    virtual void    FlushText            ( int fontID = -1 );

    virtual void    OnDestroyRS();
    virtual void    OnCreateRS(){}

    DECLARE_SCLASS(WidgetManager, SNode, WIDM);

private:
    void            CreateFullFontName    ( char* fullName, const char* fontName, int height, DWORD charset, bool bBold, bool bItalic );
    void            CreateFullFontName    ( char* fullName, const char* fontName, int charW, int charH );
    int                CreateFont            ( const char* name );
    
    BitmapFont*                m_Fonts[c_MaxFonts];
    int                        m_NFonts;


}; // class WidgetManager



#endif // __UIWIDGETMANAGER_H__