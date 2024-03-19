/*****************************************************************
/*  File:   kColorValue.h                                            
/*  Desc:   Color operations declaration
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                         
/*****************************************************************/
#ifndef __KCOLORVALUE_H__
#define __KCOLORVALUE_H__

#include "ITexture.h"

/*****************************************************************
/*  Class:  ColorValue                                              
/*  Desc:   represents color value
/*****************************************************************/
class ColorValue
{
public:
    float                   a;
    float                   r;
    float                   g;
    float                   b;

                            ColorValue          (){}
                            ColorValue          ( DWORD val ) { FromARGB( val, a, r, g, b ); }
                            ColorValue          ( float _a, float _r, float _g, float _b ) : a(_a), r(_r), g(_g), b(_b) {}

    _inl void               SetARGB             ( BYTE a, BYTE r, BYTE g, BYTE b );
    _inl operator           DWORD               () const        { return ToARGB( a, r, g, b ); }

    _inl void operator      =                   ( DWORD val ) { FromARGB( val, a, r, g, b ); }
    _inl void operator      *=                  ( const ColorValue& v ) { a *= v.a; r *= v.r; g *= v.r; b *= v.r; }
    _inl void operator      *=                  ( float val ) { a *= val; r *= val; g *= val; b *= val; }

    static _inl DWORD       ToARGB              ( BYTE a, BYTE r, BYTE g, BYTE b );
    static _inl DWORD       ToARGB              ( float a, float r, float g, float b );
    static _inl void        FromARGB            ( DWORD col, BYTE& a, BYTE& r, BYTE& g, BYTE& b );
    static _inl void        FromARGB            ( DWORD col, float& a, float& r, float& g, float& b );

    static int              GetBytesPerPixel    ( ColorFormat cf );
    static int              GetBitmapSize       ( ColorFormat cf, int nPixels );
    
    static _inl DWORD       ARGB4To8            ( DWORD color );
    static _inl DWORD       Gradient            ( DWORD a, DWORD b, float grad );
    void                    Dump                ();
    float                   GetGrayScale        () const { return 0.3f*r + 0.59f*g + 0.11f*b; }

    friend ColorValue       operator*           ( const ColorValue& c1, const ColorValue& c2 )
    {
        ColorValue c;
        c.a = c1.a*c2.a;
        c.r = c1.r*c2.r;
        c.g = c1.g*c2.g;
        c.b = c1.b*c2.b;
        return c;
    }

    static ColorValue        White, Black, Red, Green, Blue, Yellow;
}; // class ColorValue

const int c_MaxRampSize = 32;
class OutStream;
class InStream;
/*****************************************************************
/*  Class:  ColorRamp
/*  Desc:   Keyframed in the [0,1] color sequence
/*****************************************************************/
class ColorRamp
{
    float            m_Time    [c_MaxRampSize];
    ColorValue        m_Key    [c_MaxRampSize];
    int                m_NKeys;

public:
                    ColorRamp    ();
    _inl DWORD        GetColor    ( float t ) const;
    _inl DWORD        GetKey        ( int idx ) const;
    _inl float        GetKeyTime    ( int idx ) const;
    _inl void        SetKey        ( int idx, DWORD color );
    _inl void        SetKeyTime    ( int idx, float t );
    _inl void        DeleteKey    ( int idx );

    int                GetNKeys    () const { return m_NKeys; }

    void            Serialize    ( OutStream& os ) const;
    void            Unserialize    ( InStream& is );

    int                AddKey        ( float t, DWORD val );
    int                AddKey        ( float t, float a, float r, float g, float b );
    int                AddKey        ( float t, float r, float g, float b );
    void            ClearKeys   ();
}; // class ColorRamp

/*****************************************************************
/*  Class:  AlphaRamp
/*  Desc:   Keyframed in the [0,1] alpha value sequence
/*****************************************************************/
class AlphaRamp
{
    float               m_Time      [c_MaxRampSize];
    float               m_Key       [c_MaxRampSize];
    int                 m_NKeys;

public:
                        AlphaRamp   ();
    _inl float          GetAlpha    ( float t ) const;
    _inl float          GetKey      ( int idx ) const;
    _inl float          GetKeyTime  ( int idx ) const;
    _inl void           SetKey      ( int idx, float alpha );
    _inl void           SetKeyTime  ( int idx, float t );
    _inl void           DeleteKey   ( int idx );

    int                 GetNKeys    () const { return m_NKeys; }

    void                Serialize   ( OutStream& os ) const;
    void                Unserialize ( InStream& is );
    int                 AddKey      ( float t, float val );
    void                ClearKeys   ();
}; // class AlphaRamp

const DWORD c_ColorPalette[] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 
                                 0xFFFF00FF, 0xFF00FFFF, 0xFFFFFF00, 
                                 0xFFFFFFFF, 0xFF0082FF, 0xFFFF8242,
                                 0xFFFF8242 };
const int c_ColorPaletteSize = sizeof(c_ColorPalette)/sizeof(DWORD);

#ifdef _INLINES
#include "kColorValue.inl"
#endif // _INLINES

#endif // __KCOLORVALUE_H__