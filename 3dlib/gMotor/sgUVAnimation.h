/***********************************************************************************/
/*  File:   sgUVAnimation.h
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __SGUVANIMATION_H__
#define __SGUVANIMATION_H__

#include "sgAnimation.h"
#include "mFloatCurve.h"

/*****************************************************************************/
/*    Class:    UVAnimation
/*    Desc:    Texture matrix animation 
/*****************************************************************************/
class UVAnimation : public Animation
{
    FloatCurve          m_PosU, m_PosV;
    FloatCurve          m_ScU, m_ScV;
    FloatCurve          m_Rot;

public:
                        UVAnimation         ();
    virtual void        Render              ();
    virtual void        Serialize           ( OutStream& os ) const;
    virtual void        Unserialize         ( InStream& is    );
    virtual void        Expose              ( PropertyMap& pm );

    const FloatCurve*   GetPosU             () const { return &m_PosU;   }
    const FloatCurve*   GetPosV             () const { return &m_PosV;   }
    const FloatCurve*   GetRot              () const { return &m_Rot;    }
    const FloatCurve*   GetScU              () const { return &m_ScU;    }
    const FloatCurve*   GetScV              () const { return &m_ScV;    }

    Matrix3D            GetTransform        ( float time ) const;

    void                SetPosU             ( const FloatCurve* pPosU ) { if (pPosU) m_PosU = *pPosU;   }
    void                SetPosV             ( const FloatCurve* pPosV ) { if (pPosV) m_PosU = *pPosV;   }
    void                SetRot              ( const FloatCurve* pRot  ) { if (pRot)  m_Rot  = *pRot;    }
    void                SetScU              ( const FloatCurve* pScU  ) { if (pScU)  m_ScU  = *pScU;    }
    void                SetScV              ( const FloatCurve* pScV  ) { if (pScV)  m_ScV  = *pScV;    }

    DECLARE_SCLASS(UVAnimation,Controller,UVAN);
}; // class UVAnimation

#endif //__SGUVANIMATION_H__