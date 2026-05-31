/*****************************************************************************/
/*    File:    mTriangle.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-21-2003
/*****************************************************************************/
#ifndef __MTRIANGLE_H__
#define __MTRIANGLE_H__

/*****************************************************************************/
/*    Class:    Triangle2D
/*****************************************************************************/
class DIALOGS_API Triangle2D
{
public:

    typedef bool        (*PixelCallback)( int x, int y );

    Vector2D            v[3];

                        Triangle2D();
                        Triangle2D( float x0, float y0, 
                                    float x1, float y1, 
                                    float x2, float y2 );

    void                Extrude( float amount );

    _inl Vector2D        GetCenter() const;
    _inl float            Area() const;
    _inl void            GetAABB( float& x, float& y, float& w, float& h ) const;
    _inl bool            PtInside( float x, float y ) const;
    _inl bool            SameSide(    const Vector2D& p1,
                                    const Vector2D& p2,
                                    const Vector2D& a,
                                    const Vector2D& b ) const;
    
    _inl void            operator *=( float val );
    _inl void            operator +=( const Vector2D& vec );
    _inl void            operator /=( float val );
    _inl void            operator -=( const Vector2D& vec );


    Vector3D            CalcBaryCoords    ( const Vector2D& pt );
    static int            SortByXY        ( const void *pV1, const void *pV2 );

}; // class Triangle2D

DIALOGS_API Vector3D BaryCoords(    float ax, float ay, 
                        float bx, float by, 
                        float cx, float cy,
                        float ptX, float ptY );

DIALOGS_API bool BaryCoords( float ax, float ay, float bx, float by, float cx, float cy,
                 float ptX, float ptY, Vector3D& res );

DIALOGS_API bool BaryCoords(    double ax, double ay, 
                    double bx, double by, 
                    double cx, double cy,
                    double ptX, double ptY, 
                    double& bcX, double& bcY, double& bcZ );

#ifdef _INLINES
#include "mTriangle.inl"
#endif // _INLINES

#endif // __MTRIANGLE_H__