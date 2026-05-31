/*****************************************************************
/*  File:   Math2D.h                                             
/*    Desc:    
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#ifndef __GPMATH2D_H__
#define __GPMATH2D_H__
#pragma    once

#include <math.h>

#ifndef DIALOGS_API
#ifndef DIALOGS_USER
#define DIALOGS_API __declspec(dllexport)
#else //DIALOGS_USER
#define DIALOGS_API __declspec(dllimport)
#endif //DIALOGS_USER
#endif //DIALOGS_API

/*****************************************************************
/*  Class:  Matrix2D                                             
/*  Desc:   
/*****************************************************************/
class Matrix2D             
{
public:
    float           e00, e10, e01, e11;

    void            identity() { e00 = 1.0f; e11 = 1.0f; e10 = 0.0f; e01 = 0.0f; }
    void            rotation( float ang )
    {
        float fCos = cosf( ang );
        float fSin = sinf( ang );
        e00 = fCos;
        e01 = fSin;
        e10 = -fSin;
        e11 = fCos;
    }

}; // class Matrix2D 

/*****************************************************************
/*  Class:  Vector2D                                           
/*  Desc:   2-dimensional vector                                 
/*****************************************************************/
class DIALOGS_API Vector2D             
{
public:
    float                x, y;

                        Vector2D    () {}
                        Vector2D    ( float _x, float _y ) : x(_x), y(_y) {}
    float               dot         ( const Vector2D& v ) const { return x*v.x + y*v.y; }
    float               norm        () const { return sqrtf( x*x + y*y ); }
    float               norm2       () const { return x*x + y*y; }
    float               dist2       ( Vector2D v ) const { return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y); }
    void                normalize   () { float n = norm(); x /= n; y /= n; }
    const Vector2D&     operator += ( const Vector2D& v ) { x += v.x; y += v.y; return *this; }
    const Vector2D&     operator -= ( const Vector2D& v ) { x -= v.x; y -= v.y; return *this; }
    const Vector2D&     operator *= ( float val ) { x *= val; y *= val; return *this; }
    const Vector2D&     operator *= ( const Matrix2D& m )
    {
        float tx = x*m.e00 + y*m.e10;
        float ty = x*m.e01 + y*m.e11;
        x = tx; y = ty;
        return *this;
    }
    const Vector2D&     operator /= ( float val ){ x /= val; y /= val; return *this; }
    float                triArea     ( const Vector2D& v ) const { return v.x * y - v.y * x; }
}; // class Vector2D

typedef Vector2D     Vector2D;
typedef Matrix2D     Matrix2Df;

/*****************************************************************
/*  Class:  Rct                                                 
/*  Desc:   2D rectangular area                                   
/*****************************************************************/
class DIALOGS_API Rct
{
public:
    Rct()                                            : x(0), y(0), w(0), h(0)            {}
    Rct( float _w, float _h )                        : x(0.0f), y(0.0f), w(_w), h(_h)    {}
    Rct( float _x, float _y, float _side )            : x(_x), y(_y), w(_side), h(_side)    {}

    _inl Rct( float _x, float _y, float _w, float _h );

    float           b() const { return y + h; }
    float           r() const { return x + w; }
    _inl float        GetAspect    ()                            const;
    _inl float        MaxSide        ()                            const;
    _inl bool        PtIn        ( float pX, float pY )        const;
    _inl bool        PtInStrict    ( float pX, float pY )        const;

    _inl float        GetCenterX    ()                            const;
    _inl float        GetCenterY    ()                            const;
    _inl float        Dist2ToPt    ( float pX, float pY )        const;

    _inl void        Copy        ( const Rct& orig );
    _inl void        Deflate        ( float amt );
    _inl void        Inflate        ( float top, float right, float bottom, float left );
    _inl void        Inflate        ( float val );

    _inl void        FitInto        ( const Rct& rct );
    _inl void        CenterInto    ( const Rct& rct );
    
    _inl void        Set            ( float _x, float _y, float _w, float _h );
    _inl void        Zero        ();
    _inl bool        Overlap        ( const Rct& rct ) const;
    _inl bool        IsOutside    ( const Rct& rct ) const;
    _inl void        Union        ( const Rct& rct );

    _inl float        GetRight    () const;
    _inl float        GetBottom    () const;
    _inl bool        ClipSegment    ( Vector2D& a, Vector2D& b ) const;
    _inl void        SetPositiveDimensions();

    _inl bool         ClipHLine    ( float& px1, float& px2, float py ) const;
    _inl bool         ClipVLine    ( float px, float& py1, float& py2 ) const;
    _inl bool         Clip        ( Rct& rct ) const;
    _inl bool        IsRectInside( float px, float py, float pw, float ph ) const;


    _inl void        operator *=( float val );
    _inl void        operator /=( float val );
    _inl void        operator +=( const Vector2D& delta );
    _inl void        operator +=( const Rct& delta );


    bool operator == ( const Rct& v ) const
    {
        return (x == v.x)&&(y == v.y)&&(w == v.w)&&(h == v.h);
    }
    
    float                x, y, w, h;

    static const Rct    unit;
    static const Rct    null;
};  // class Rct

#ifdef _INLINES
#include "mMath2D.inl"
#endif // _INLINES

#endif // __GPMATH2D_H__