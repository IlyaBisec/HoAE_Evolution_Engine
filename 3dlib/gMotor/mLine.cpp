/*****************************************************************
/*  File:   mLine.cpp             
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:    
/*****************************************************************/
#include "stdafx.h"
#include "mLine.h"

/*****************************************************************/
/*    Line2D implementation
/*****************************************************************/
bool Line2D::OneSide( const Vector2D& v1, const Vector2D& v2 ) const
{
    return ( ((v1.x - a.x)*(b.y - a.y) - (v1.y - a.y)*(b.x - a.x)) * 
        ((v2.x - a.x)*(b.y - a.y) - (v2.y - a.y)*(b.x - a.x))) >= 0; 
} // Line2D::OneSide

bool Line2D::PtOnLine( const Vector2D& p ) const
{
    float det = (p.x - a.x)*(b.y - a.y) - (p.y - a.y)*(b.x - a.x);
    return fabs( det ) < c_Epsilon;
} // Line2D::PtOnLine

Vector2D Line2D::Intersect( const Line2D& s ) const
{
    Vector2D res;
    float den = (a.x - b.x)*(s.b.y - s.a.y) + 
                (s.a.x - s.b.x)*(a.y - b.y);
    res.x = s.a.x*(s.b.y*(a.x - b.x) + b.x*a.y - a.x*b.y) + 
            s.b.x*(s.a.y*(b.x - a.x) - b.x*a.y + a.x*b.y);
    res.y = (a.y - b.y)*(s.a.x*s.b.y - s.b.x*s.a.y) + 
            (s.a.y - s.b.y)*(b.x*a.y - a.x*b.y);
    res /= den;
    return res;
} // Line2D::Intersect

bool Line2D::IsParallel( const Line2D& s ) const
{
    float den = (a.x - b.x)*(s.b.y - s.a.y) + 
        (s.a.x - s.b.x)*(a.y - b.y);
    return fabs( den ) < c_Epsilon;
} // Line2D::IsParallel








