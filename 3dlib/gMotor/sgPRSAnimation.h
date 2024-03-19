/***********************************************************************************/
/*  File:   sgPRSAnimation.h
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __SGPRSANIMATION_H__
#define __SGPRSANIMATION_H__

#include "sgAnimation.h"
#include "mFloatCurve.h"
#include "mQuatCurve.h"

const float c_ScaleTolerance    = 0.0001f;
const float c_RotTolerance      = 0.000001f;
const float c_PosTolerance      = 0.001f;
/*****************************************************************************/
/*    Class:    PRSAnimation
/*    Desc:    Position/rotation/scaling animation sequence controller
/*****************************************************************************/
class PRSAnimation : public Animation
{
    //  position
    FloatCurve        posX, posY, posZ;
    //  rotation
    QuatCurve        rot;
    //  scale
    FloatCurve        scX, scY, scZ;

    //  symbolic identifier of the controlled node
    std::string                m_BaseAnimationName;

public:    
    _inl                    PRSAnimation        () {}
    virtual void            Serialize            ( OutStream& os ) const;
    virtual void            Unserialize            ( InStream& is );
    virtual void            Expose                ( PropertyMap& pm );
    virtual void            Render                ();
    
    _inl Matrix4D            GetTransform        ( float time ) const;
    
    //  blend beetween two animations
    static Matrix4D            GetTransform        ( const PRSAnimation* anm1, float time1,
                                                  const PRSAnimation* anm2, float time2,
                                                  float blendFactor );
    
    _inl int                GetPosXNKeys        () const;
    _inl int                GetPosYNKeys        () const;
    _inl int                GetPosZNKeys        () const;

    _inl int                GetRotNKeys            () const;
    _inl int                GetScaleXNKeys        () const;
    _inl int                GetScaleYNKeys        () const;
    _inl int                GetScaleZNKeys        () const;

    bool                    IsConstant            ();    
    void                    ReduceKeys            (    float scaleBias = c_ScaleTolerance, 
                                                    float rotBias = c_RotTolerance, 
                                                    float posBias = c_PosTolerance );
    
    void                     AddKey                ( float keyTime, const Matrix4D& tm );

    float                    CalculateMaxTime    () const;

    float                     GetScaleDiff        ( float anmTime, const Vector3D& sc );
    float                     GetPosDiff            ( float anmTime, const Vector3D& pos );
    float                     GetRotDiff            ( float anmTime, const Quaternion& quat );

    void                    FlipXAxis           ();
    void                    FlipYAxis           ();
    void                    FlipZAxis           ();

    _inl void                SetBaseAnimationName( const char* basename );
    _inl const char*        GetBaseAnimationName() const;

    const FloatCurve*    GetPosX    () const { return &posX;    }
    const FloatCurve*    GetPosY    () const { return &posY;    }
    const FloatCurve*    GetPosZ    () const { return &posZ;    }

    const QuatCurve*    GetRot        () const { return &rot;    }
    const FloatCurve*    GetScaleX    () const { return &scX;    }
    const FloatCurve*    GetScaleY    () const { return &scY;    }
    const FloatCurve*    GetScaleZ    () const { return &scZ;    }

    FloatCurve*        GetPosXAnimation    (){ return &posX;    }
    FloatCurve*        GetPosYAnimation    (){ return &posY;    }
    FloatCurve*        GetPosZAnimation    (){ return &posZ;    }

    QuatCurve*            GetRotAnimation        (){ return &rot;    }
    FloatCurve*        GetScaleXAnimation    (){ return &scX;    }
    FloatCurve*        GetScaleYAnimation    (){ return &scY;    }
    FloatCurve*        GetScaleZAnimation    (){ return &scZ;    }


    void                    SetPosXAnimation    ( const FloatCurve* pPosX ) { if (pPosX) posX = *pPosX; }
    void                    SetPosYAnimation    ( const FloatCurve* pPosY ) { if (pPosY) posY = *pPosY; }
    void                    SetPosZAnimation    ( const FloatCurve* pPosZ ) { if (pPosZ) posZ = *pPosZ; }

    void                    SetRotAnimation        ( const QuatCurve* pRot  )    { if (pRot) rot = *pRot; }
    void                    SetScaleXAnimation    ( const FloatCurve* pScX )    { if (pScX) scX = *pScX; }
    void                    SetScaleYAnimation    ( const FloatCurve* pScY )    { if (pScY) scY = *pScY; }
    void                    SetScaleZAnimation    ( const FloatCurve* pScZ )    { if (pScZ) scZ = *pScZ; }


    DECLARE_SCLASS(PRSAnimation,Animation,PRSA);

}; // class PRSAnimation

#ifdef _INLINES
#include "sgPRSAnimation.inl"
#endif // _INLINES

#endif //__SGPRSANIMATION_H__