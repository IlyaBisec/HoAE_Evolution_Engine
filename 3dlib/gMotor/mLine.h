/*****************************************************************
/*  File:   mLine.h                                             
/*  Desc:    
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   July 2005
/*****************************************************************/
#ifndef __MLINE_H__
#define __MLINE_H__

/*****************************************************************
/*  Class:  Line2D                                                  
/*  Desc:   2D line                                                 
/*****************************************************************/
class Line2D     
{
public:
                        Line2D      ( float ax, float ay, float bx, float by ) : a( ax, ay ), b( bx, by ) {}
                        Line2D      (){}

    Vector2D            Intersect   ( const Line2D& s ) const;
    bool                OneSide     ( const Vector2D& v1, const Vector2D& v2 ) const;
    bool                IsParallel  ( const Line2D& s ) const;

    bool                InLeftHalf  ( const Vector2D& pt ) const
    {
        return ((pt.x - a.x)*(b.y - a.y) - (pt.y - a.y)*(b.x - a.x)) <= 0;
    }

    bool                InLeftHalf  ( float x, float y ) const
    {
        return ((x - a.x)*(b.y - a.y) - (y - a.y)*(b.x - a.x)) <= 0;
    }

    bool                PtOnLine    ( const Vector2D& p ) const;
    Vector2D            a, b;
}; // class Line2D

#endif // __MLINE_H__