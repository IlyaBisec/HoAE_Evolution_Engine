/*****************************************************************************/
/*    File:    sgLight.h
/*    Desc:    Scene graph light
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGLIGHT_H__
#define __SGLIGHT_H__

#include "IMediaManager.h"



/*****************************************************************************/
/*    Class:    LightSource
/*    Desc:    Basic light source class
/*****************************************************************************/
class LightSource : public TransformNode, public ILight
{
public:
    _inl                    LightSource        ();
    _inl                    LightSource        ( const Vector3D& dir );
    _inl                    LightSource        ( const Vector3D& pos, const Vector3D& dir );

    _inl Vector3D            GetPos        ()    const;
    _inl Vector3D            GetDir        ()    const;

    _inl DWORD                GetAmbient    ()    const;
    _inl DWORD                GetDiffuse    ()    const;
    _inl DWORD                GetSpecular    ()    const;
    _inl float                GetRange    ()    const { return 0; }

    _inl DWORD                GetIndex    ()    const{ return m_Index; }
    _inl void                SetIndex    ( DWORD index ) { m_Index = index; }


    _inl void                SetPos        ( const Vector3D& pos );
    _inl void                SetDir        ( const Vector3D& dir );
    _inl void                SetDiffuse    ( DWORD diffuse    );
    _inl void                SetAmbient    ( DWORD ambient    );
    _inl void                SetSpecular    ( DWORD specular    );
    _inl void                SetRange    ( float range        ){}

    virtual void            Serialize    ( OutStream& os ) const;
    virtual void            Unserialize    ( InStream& is    );
    virtual void            Expose        ( PropertyMap& pm );
    virtual void            Render        ();
    virtual Frustum            GetFrustum    ( const AABoundBox& aabb ) const;

    DECLARE_SCLASS(LightSource,TransformNode,LIHT);

protected:
    DWORD                        m_Ambient;    
    DWORD                        m_Diffuse;
    DWORD                        m_Specular;
    int                            m_Index;
};  // class LightSource

/*****************************************************************************/
/*    Class:    DirectionalLight
/*    Desc:    Directional light source class
/*****************************************************************************/
class DirectionalLight : public LightSource
{
public:
    virtual void            Render        ();
    virtual void            Expose        ( PropertyMap& pm );
    virtual Frustum            GetFrustum    ( const AABoundBox& aabb ) const;


    _inl Ray3D                GetLightRay () const;

    DECLARE_SCLASS(DirectionalLight, LightSource, DIRL);
}; // class DirectionalLight

/*****************************************************************************/
/*    Class:    PointLight
/*    Desc:    Pont light source class
/*****************************************************************************/
class PointLight : public LightSource
{
    float                    m_Range;

    float                    m_AttA;
    float                    m_AttB;
    float                    m_AttC;

public:
                            PointLight        ();
                            PointLight        ( const Vector3D& _pos, const Vector3D& _dir );

    _inl float                GetRange        () const { return m_Range; }
    _inl float                GetAttenuationA    () const { return m_AttA; }
    _inl float                GetAttenuationB    () const { return m_AttB; }
    _inl float                GetAttenuationC    () const { return m_AttC; }

    _inl void                SetRange        ( float val ) { m_Range = val; }
    _inl void                SetAttenuationA    ( float val ) { m_AttA = val; }
    _inl void                SetAttenuationB    ( float val ) { m_AttB = val; }
    _inl void                SetAttenuationC    ( float val ) { m_AttC = val; }

    _inl Sphere                GetLightSphere  () const;

    virtual void            Render            ();
    virtual void            Expose            ( PropertyMap& pm );
    virtual void            Serialize        ( OutStream& os ) const;
    virtual void            Unserialize        ( InStream& is    );

    virtual Frustum            GetFrustum    ( const AABoundBox& aabb ) const;

    DECLARE_SCLASS(PointLight, LightSource, POIL);
}; // class PointLight

/*****************************************************************************/
/*    Class:    SpotLight
/*    Desc:    Spot light source class
/*****************************************************************************/
class SpotLight : public PointLight
{
    float                    m_InnerCone;    //  inner cone radius, in radians
    float                    m_OuterCone;    //  outer cone radius, in radians
    float                    m_ConeFalloff;    //  falloff between inner and outer cone

public:
    
    _inl float                GetInnerCone    () const { return m_InnerCone; }
    _inl float                GetOuterCone    () const { return m_OuterCone; }
    _inl float                GetConeFalloff    () const { return m_ConeFalloff; }

    _inl void                SetInnerCone    ( float val ) { m_InnerCone = val; }
    _inl void                SetOuterCone    ( float val ) { m_OuterCone = val; }
    _inl void                SetConeFalloff    ( float val ) { m_ConeFalloff = val; }

    virtual void            Render            ();
    virtual void            Expose            ( PropertyMap& pm );
    virtual void            Serialize        ( OutStream& os ) const;
    virtual void            Unserialize        ( InStream& is    );
    virtual Frustum            GetFrustum        ( const AABoundBox& aabb ) const;

    DECLARE_SCLASS(SpotLight, PointLight, SPOL);
}; // class SpotLight



#ifdef _INLINES
#include "sgLight.inl"
#endif // _INLINES

#endif // __SGLIGHT_H__