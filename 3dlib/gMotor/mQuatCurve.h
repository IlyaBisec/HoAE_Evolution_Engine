/*****************************************************************************/
/*    File:    mQuatCurve.hpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    20.10.2004
/*****************************************************************************/
#ifndef __MQUATCURVE_H__
#define __MQUATCURVE_H__

#include "mAnimCurve.hpp"
#include "mSplines.h"

class FloatCurve;
/*****************************************************************************/
/*    Class:    QuatCurve
/*    Desc:    Keyframed Quat value curve
/*****************************************************************************/
class QuatCurve : public AnimationCurve<Quaternion>
{
public:
                                QuatCurve() { m_DefaultValue.setIdentity(); } 

    virtual Quaternion GetValue( float time ) const
    {
        if (m_Values.size() == 0) return m_DefaultValue;

        Quaternion quat;
        int kfIdx1 = LocateTime( time );
        if (kfIdx1 < 0) 
        {
            return m_Values[0];
        }

        float t1 = m_Times[kfIdx1];
        const Quaternion&  q1 = m_Values[kfIdx1];

        int kfIdx2 = kfIdx1 + 1;
        if (kfIdx2 >= m_Times.size()) return q1;

        float t2 = m_Times[kfIdx2];
        const Quaternion&  q2 = m_Values[kfIdx2];
        if (time == t1) return q1;
        if (time == t2) return q2;

        float t = (time - t1) / (t2 - t1);

        quat.Slerp( q1, q2, t );
        return quat;
    }

    void LinearReduceKeys( float treshold );
    void CorrectOrientation();
    bool IsConstant( float tolerance ) const;
    bool FromEuler( const FloatCurve& yaw, const FloatCurve& pitch, const FloatCurve& roll );
}; // class QuatCurve

#endif  // __MQUATCURVE_H__
