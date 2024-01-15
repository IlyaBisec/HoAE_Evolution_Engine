/*****************************************************************************/
/*    File:    mFilter.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-24-2003
/*****************************************************************************/
#ifndef __MFILTER_H__
#define __MFILTER_H__

/*****************************************************************************/
/*    Class:    Filter
/*****************************************************************************/
template <int order>
class Filter
{
    static const int kSize = order * order;

    WORD        kernel[order*order];
    int         kShiftX[order*order];
    int         kShiftY[order*order];
    WORD        scale;

public:
                Filter( const WORD* pKernel );    
    
    bool        PerByteConvolve( DWORD* arr, int arrW, int arrH, DWORD resMask = 0xFFFFFFFF );
    bool        PerByteConvolve( DWORD* arr, BYTE* maskArr, int arrW, int arrH, DWORD resMask = 0xFFFFFFFF );

protected:
    _inl DWORD  GetPixel( DWORD* arr, int x, int y, int w, int h ) const;
    _inl BYTE   GetPixel( BYTE* arr, int x, int y, int w, int h ) const;

}; // class Filter

float CalculateRMS( DWORD* arr1, DWORD* arr2, int nEl, DWORD resMask = 0xFFFFFFFF );

#include "mFilter.hpp"

/*****************************************************************************/
/*    Class:    ABlurringFilter
/*****************************************************************************/
class ABlurringFilter : public Filter<3>  
{
public:
    ABlurringFilter() : Filter<3>( c_ABlurringKernel ){}
}; // class ABlurringFilter

/*****************************************************************************/
/*    Class:    BBlurringFilter
/*****************************************************************************/
class BBlurringFilter : public Filter<5>  
{
public:
    BBlurringFilter() : Filter<5>( c_BBlurringKernel ){}
}; // class BBlurringFilter

/*****************************************************************************/
/*    Class:    Gaussian1Filter
/*****************************************************************************/
class Gaussian1Filter : public Filter<5>  
{
public:
    Gaussian1Filter() : Filter<5>( c_Gaussian1Kernel ){}
}; // class Gaussian1Filter

/*****************************************************************************/
/*    Class:    Gaussian2Filter
/*****************************************************************************/
class Gaussian2Filter : public Filter<5>  
{
public:
    Gaussian2Filter() : Filter<5>( c_Gaussian2Kernel ){}
}; // class Gaussian2Filter

#endif __MFILTER_H__
