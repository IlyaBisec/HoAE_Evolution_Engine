/*****************************************************************************/
/*    File:    mFloatCurve.hpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    20.10.2004
/*****************************************************************************/
#ifndef __MFLOATCURVE_H__
#define __MFLOATCURVE_H__

#include "mAnimCurve.hpp"
#include "mSplines.h"

/*****************************************************************************/
/*    Class:    FloatCurve
/*    Desc:    Keyframed float value curve
/*****************************************************************************/
class FloatCurve : public AnimationCurve<float>
{
public:
    FloatCurve() { m_DefaultValue = 0.0f; }
    float                GetValue        ( float time ) const
    {
        if (m_Values.size() == 0) return m_DefaultValue;

        int kfIdx1 = LocateTime( time );
        if (kfIdx1 < 0) return m_Values[0];

        float t1 = m_Times[kfIdx1];
        float v1 = m_Values[kfIdx1];

        int kfIdx2 = kfIdx1 + 1;
        if (kfIdx2 >= m_Values.size()) return v1;

        float t2 = m_Times[kfIdx2];
        float v2 = m_Values[kfIdx2];

        if (time == t1) return v1;
        if (time == t2) return v2;

        return LinearInterpolate( time, t1, v1, t2, v2 );
    }

    void        LinearReduceKeys( float treshold );
    float       GetMinVal() const;
    float       GetMaxVal() const;
    bool        IsConstant( float tolerance ) const;

}; // class FloatCurve

#endif  // __MANIMCURVE_H__
