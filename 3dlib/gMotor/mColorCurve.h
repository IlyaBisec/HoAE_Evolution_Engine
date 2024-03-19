/*****************************************************************************/
/*    File:    mColorCurve.hpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    20.10.2004
/*****************************************************************************/
#ifndef __MCOLORCURVE_H__
#define __MCOLORCURVE_H__

#include "mAnimCurve.hpp"
#include "mSplines.h"

/*****************************************************************************/
/*    Class:   ColorCurve
/*    Desc:    Keyframed float value curve
/*****************************************************************************/
class ColorCurve : public AnimationCurve<ColorValue>
{
public:
                            ColorCurve  () { m_DefaultValue = ColorValue::White; }
    virtual ColorValue      GetValue    ( float time ) const
    {
    	if (m_Values.size() == 0) return m_DefaultValue;

    	int kfIdx1 = LocateTime( time );
    	if (kfIdx1 < 0) return m_Values[0];

    	float		t1 = m_Times[kfIdx1];
    	ColorValue	v1 = m_Values[kfIdx1];

    	int kfIdx2 = kfIdx1 + 1;
    	if (kfIdx2 >= m_Values.size()) return v1;

    	float		t2 = m_Times[kfIdx2];
    	ColorValue	v2 = m_Values[kfIdx2];

    	if (time == t1) return v1;
    	if (time == t2) return v2;

    	ColorValue res;
    	res.a = LinearInterpolate( time, t1, v1.a, t2, v2.a );
    	res.r = LinearInterpolate( time, t1, v1.r, t2, v2.r );
    	res.g = LinearInterpolate( time, t1, v1.g, t2, v2.g );
    	res.b = LinearInterpolate( time, t1, v1.b, t2, v2.b );
    	return res;
    } // GetValue

}; // class ColorCurve

#endif  // __MANIMCURVE_H__
