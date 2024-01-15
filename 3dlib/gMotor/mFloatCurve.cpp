/*****************************************************************************/
/*    File:     sgController.cpp
/*    Author:   Ruslan Shestopalyuk
/*    Date:     15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "mAlgo.h"
#include "mFloatCurve.h"

/*****************************************************************************/
/*    FloatCurve implementation
/*****************************************************************************/
void FloatCurve::LinearReduceKeys( float treshold )
{
	int nKeys = GetNKeys();
    if (nKeys == 0) return;

    int cKey = 0;
    float t1 = GetKeyTime( 0 );
    float v1 = GetKeyValue( 0 );
    
    float t2 = GetKeyTime( nKeys - 1 );
    float v2 = GetKeyValue( nKeys - 1 );

    int curKey = 2;
    while (curKey < nKeys)
    {
        t2 = GetKeyTime( curKey );
        v2 = GetKeyValue( curKey );
        float val = LinearInterpolate( GetKeyTime( curKey - 1 ), t1, v1, t2, v2 );
        if (fabs( val - GetKeyValue( curKey - 1 ) ) <= treshold)
        {
            DeleteKey( curKey - 1 );
            nKeys--;
        }
        else 
        {
            t1 = GetKeyTime( curKey - 1 );
            v1 = GetKeyValue( curKey - 1 );
            curKey++;
        }
    }

    //  check if there are only two identical keys left
    if (nKeys == 2 && fabs( v1 - v2 ) <= treshold)
    {
        DeleteKey( 0 );
        DeleteKey( 0 );
        SetDefaultValue( v1 );
    }
    
} // FloatCurve::LinearReduceKeys

float FloatCurve::GetMinVal() const
{
    if (m_Values.size() == 0) return m_DefaultValue;
    float minVal = m_Values[0];
    for (int i = 1; i < m_Values.size(); i++)
    {
        float val = m_Values[i];
        if (val < minVal) minVal = val;
    }
    return minVal;
} // FloatCurve::GetMinVal

float FloatCurve::GetMaxVal() const
{
    if (m_Values.size() == 0) return m_DefaultValue;
    float maxVal = m_Values[0];
    for (int i = 1; i < m_Values.size(); i++)
    {
        float val = m_Values[i];
        if (val > maxVal) maxVal = val;
    }
    return maxVal;
} // FloatCurve::GetMaxVal

bool FloatCurve::IsConstant( float tolerance ) const
{
    if (GetNKeys() == 0) return true;
    float first = GetKeyValue( 0 );
        
    for (int i = 1; i < GetNKeys(); i++)
    {
        if (fabs( GetKeyValue( i ) - first ) > tolerance) return false;
    }    
    return true;
} // FloatCurve::IsConstant


