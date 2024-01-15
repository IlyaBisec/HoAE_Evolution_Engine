/************************************************************************************
/*  File:   mQuadric.h                                             
/*    Desc:    Quadric class, used for calculating mesh simplification error metrics
/*            Described by Michael Garland in "Surface Simplification using quadric
/*                error metrics"
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   June 2004                                          
/************************************************************************************/
#ifndef __MQUADRIC_H__
#define __MQUADRIC_H__
/*****************************************************************
/*  Class:  Quadric                                              
/*****************************************************************/
class Quadric             
{
    double            e00, e01, e02, e03;
    double                 e11, e12, e13;
    double                      e22, e23;
    double                           e33;

public:
                    Quadric() : e00(0.0f), e01(0.0f), e02(0.0f), e03(0.0f), 
                                e11(0.0f), e12(0.0f), e13(0.0f), e22(0.0f), e23(0.0f), e33(0.0f){}
                    Quadric( const Quadric& q ) 
                    {
                        e00 = q.e00; e01 = q.e01; e02 = q.e02; e03 = q.e03;
                        e11 = q.e11; e12 = q.e12; e13 = q.e13;
                        e22 = q.e22; e23 = q.e23;
                        e33 = q.e33;
                    }
    void operator    +=( const Quadric& q )
    {
        e00 += q.e00; e01 += q.e01; e02 += q.e02; e03 += q.e03;
        e11 += q.e11; e12 += q.e12; e13 += q.e13;
        e22 += q.e22; e23 += q.e23;
        e33 += q.e33;
    }
    void operator    *=( double val  ) 
    { 
        assert( false ); 
    }
    void            AddPlane( double a, double b, double c, double d )
    {
        e00 += a*a; e01 += a*b; e02 += a*c; e03 += a*d;
                    e11 += b*b; e12 += b*c; e13 += b*d;
                                e22 += c*c; e23 += c*d;
                                            e33 += d*d;
    }

    float            GetError( double x, double y, double z )
    {
        return e33 + e00*x*x + e11*y*y + e22*z*z + 
                2.0f*(e03*x + e13*y + e23*z + e01*x*y + e02*x*z + e12*y*z) ;
    }

}; // class Quadric

#endif // __MQUADRIC_H__