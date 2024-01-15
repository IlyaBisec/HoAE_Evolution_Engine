/*****************************************************************************/
/*    File:    vField.h
/*    Desc:    Wheat field 
/*    Author:    Ruslan Shestopalyuk
/*    Date:    16.05.2003
/*****************************************************************************/
#ifndef __VFIELD_H__
#define __VFIELD_H__

const int    c_NumFieldRnd          = 1024;
const float  c_FieldRowsStep        = 6.0f;
const float  c_FieldRowsStepPart    = 2.5f;
const int    c_FieldBatchSize       = 16384;
const float  c_FieldMaxSegmentW     = 256.0f;
const float  c_DefStrawHeight       = 30.0f;
const float  c_FieldTextureURatio   = 1.0f / 180.0f;

/*****************************************************************************/
/*    Class:    FieldModel
/*****************************************************************************/
class FieldModel
{
    BaseMesh            m_FieldMesh;
    float               m_StrawHeight;
    bool                m_bInited;
    float               m_Rnd[c_NumFieldRnd];

public:
                        FieldModel      ();
                        ~FieldModel     ();

    _inl void           EnableWireframe ( bool enable = true );
    _inl void           Setm_StrawHeight( float _m_StrawHeight );

    void                Init();

    bool                AddPatch(   float x, float y, float z,
                                    float width, float height,
                                    const Vector3D& normal,
                                    float angle,
                                    float growRatio, 
                                    float heightScale = 1.0f );

    bool                AddPatch(   const Vector3D& lt,
                                    const Vector3D& rt,
                                    const Vector3D& lb,
                                    const Vector3D& rb,
                                    float growRatio, 
                                    float heightScale = 1.0f );
    
    void                Draw();

    _inl void           GetBoundingSphere   ( Sphere& sphere );
    
protected:
    _inl DWORD          GetGrowColor        ( float growRatio ) const;
    _inl DWORD          GetAnimationTime    () const;
    _inl float          GetStrawBend        ( float strawX, float strawY, float ftime,
                                                float& bendDirX, float& bendDirY ) const;
    _inl float          GetStrawPitch       ( float heightFromGround, float bendAmount );
    _inl float          GetStrawHeight      ( float strawX, float strawY, float growRatio );
    _inl float          GetTexShift         ( float strawX, float strawY );

}; // class FieldModel

#ifdef _INLINES
#include "vField.inl"
#endif // _INLINES

#endif // __VFIELD_H__