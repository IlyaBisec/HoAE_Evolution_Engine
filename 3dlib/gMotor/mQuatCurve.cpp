/*****************************************************************************/
/*    File:    QuatCurve.cpp
/*    Author:  Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "mAlgo.h"
#include "mFloatCurve.h"
#include "mQuatCurve.h"


/*****************************************************************************/
/*    QuatCurve implementation
/*****************************************************************************/
bool QuatCurve::FromEuler( const FloatCurve& yaw, const FloatCurve& pitch, const FloatCurve& roll )
{
    if (yaw.GetNKeys() == 0 && pitch.GetNKeys() == 0 && roll.GetNKeys() == 0)
    {
        Quaternion quat;
        quat.FromEulerAngles( yaw.GetDefaultValue(), pitch.GetDefaultValue(), roll.GetDefaultValue() );
        SetDefaultValue( quat );
        return true;
    }

    Quaternion quat;
    float curTime = 0.0f;
    float maxTime = tmax( yaw.GetMaxTime(), pitch.GetMaxTime(), roll.GetMaxTime() ) + 1.0f;
    
    int maxY = yaw.GetNKeys() ? yaw.  GetNKeys() - 1 : 0;
    int maxP = pitch.GetNKeys() ? pitch.GetNKeys() - 1 : 0;
    int maxR = roll.GetNKeys() ? roll. GetNKeys() - 1 : 0;
    
    int cY = 0; 
    int cP = 0;
    int cR = 0;

    float ty = yaw.GetNKeys()    ? yaw.GetKeyTime    ( 0 ) : maxTime;
    float tp = pitch.GetNKeys() ? pitch.GetKeyTime    ( 0 ) : maxTime;
    float tr = roll.GetNKeys()    ? roll.GetKeyTime    ( 0 ) : maxTime;

    while (cY < maxY || cP < maxP || cR < maxR)
    {
        curTime = tmin( ty, tp, tr );

        if (ty == curTime) 
        {
            if (cY < maxY)
            {
                cY++;
                ty = yaw.GetKeyTime( cY );
            }
        }

        if (tp == curTime) 
        {
            if (cP < maxP)
            {
                cP++;
                tp = pitch.GetKeyTime( cP );
            }
        }

        if (tr == curTime) 
        {
            if (cR < maxR)
            {
                cR++;
                tr = roll.GetKeyTime( cR );
            }
        }

        quat.FromEulerAngles( yaw.    GetValue( curTime ), 
                              pitch.GetValue( curTime ),
                              roll. GetValue( curTime ) );
        AddKey( curTime, quat );    
    }

    return true;
} // QuatCurve::FromEuler


void QuatCurve::LinearReduceKeys( float treshold )
{
	int nKeys = GetNKeys();
	if (nKeys == 0) return;

	int cKey = 0;
	float t1 = GetKeyTime( 0 );
	Quaternion v1 = GetKeyValue( 0 );

	float t2 = GetKeyTime( nKeys - 1 );
	Quaternion v2 = GetKeyValue( nKeys - 1 );
    Quaternion val;    

    int curKey = 2;
    while (curKey < nKeys)
    {
        t2 = GetKeyTime( curKey );
        v2 = GetKeyValue( curKey );

        float t = (GetKeyTime( curKey - 1 ) - t1) / (t2 - t1);
        val.Slerp( v1, v2, t );
        val -= GetKeyValue( curKey - 1 );
        if (!v1.InSameHemisphere( v2 ))
        {
            int tt = 0;
        }
        if (val.norm2() <= treshold && v1.InSameHemisphere( v2 ))
        {
            DeleteKey( curKey - 1 );
            nKeys--;
        }
        else 
        {
            t1 = GetKeyTime ( curKey - 1 );
            v1 = GetKeyValue( curKey - 1 );
            curKey++;
        }
    }

    //  check if there are only two identical keys left
    if (nKeys == 2)
    {
        v2 -= v1;
        if (v2.norm2() <= treshold)
        {
            DeleteKey( 0 );
            DeleteKey( 0 );
            SetDefaultValue( v1 );
        }
    }
} // QuatCurve::LinearReduceKeys

bool QuatCurve::IsConstant( float tolerance ) const
{
    if (GetNKeys() == 0) return true;
    Quaternion first = GetKeyValue( 0 );

    for (int i = 1; i < GetNKeys(); i++)
    {
        Quaternion quat = GetKeyValue( i );
        quat -= first;
        if (fabs( quat.norm2() ) > tolerance) return false;
    }    
    return true;
} //  QuatCurve::IsConstant

//  corrects quaternion sequence to make all neighbor quaternions
//  oriented by shortest arc
void QuatCurve::CorrectOrientation()
{
    int nKeys = GetNKeys();
    for (int i = 1; i < nKeys; i++)
    {
        float cosTheta = m_Values[i - 1].dot( m_Values[i] );
        if (cosTheta < 0.0f)
        {
            m_Values[i].reverse();
        }
    }
} // QuatCurve::CorrectOrientation





