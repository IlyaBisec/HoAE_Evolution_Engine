/*****************************************************************
/*  File:   IFont.h                                      
/*  Desc:   Interface to the fonts
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Nov 2004                                             
/*****************************************************************/
#ifndef __IFONT_H__ 
#define __IFONT_H__

class Vector3D;
class Rct;
/*****************************************************************/
/*    Class:    IFont
/*    Desc:    Interface to the fonts
/*****************************************************************/
class IFont
{
public:
    virtual const char* GetName         () const = 0;
    virtual int         GetID           () const = 0;
    virtual void        SetID           ( int id ) = 0;

    virtual bool        Create          ( const char* name, int height, DWORD charset = DEFAULT_CHARSET, bool bBold = false, bool bItalic = false )    = 0;
    virtual bool        Create          ( const char* texName, int charW, int charH ) = 0;
    virtual int         GetStringWidth  ( const char* str, int spacing = 1 ) = 0;
    virtual int         GetCharWidth    ( BYTE ch ) = 0;
    virtual int         GetCharHeight   ( BYTE ch ) = 0;

    virtual bool        DrawString      ( const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 ) = 0;
    virtual bool        DrawString3D    ( const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 ) = 0;
    virtual bool        DrawChar        ( const Vector3D& pos, BYTE ch, DWORD color = 0xFFFFFFFF ) = 0;
    virtual bool        DrawChar        ( const Vector3D& pos, const Rct& uv, DWORD color = 0xFFFFFFFF ) = 0;
    virtual bool        DrawChar        ( const Vector3D& pos, const Rct& uv, float w, float h, DWORD color = 0xFFFFFFFF ) = 0;
    virtual void        Flush           () = 0;    


    virtual void        DeleteDeviceObjects     () = 0;
    virtual void        InvalidateDeviceObjects () = 0;
    virtual void        RestoreDeviceObjects    () = 0;
}; // class IFont

#endif // __IFONT_H__ 