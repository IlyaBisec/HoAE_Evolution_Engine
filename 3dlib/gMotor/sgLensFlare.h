#ifndef __SGLENSFLARE_H__
#define __SGLENSFLARE_H__


/*****************************************************************************/
/*    Class:    LensFlareElement
/*    Desc:    Single piece of lens flare
/*****************************************************************************/
class LensFlareElement : public SNode
{
public:
                    LensFlareElement();

    _inl float        GetRadius    ()    const { return radius;     }
    _inl float        GetPosition    ()    const { return position; }
    _inl DWORD        GetColor    ()    const { return color;     }
    _inl float        GetAlpha    ()    const 
    { 
        return float((color & 0xFF000000)>>24) / 255.0f;     
    }

    _inl void        SetRadius    ( float val ) { radius     = val;    }
    _inl void        SetPosition    ( float val ) { position = val; }
    _inl void        SetColor    ( DWORD val ) 
    { 
        color &= 0xFF000000;
        color |= val;
    }

    _inl void        SetAlpha    ( float val ) 
    { 
        color &= 0x00FFFFFF;
        color |= (DWORD( val*255.0f ) << 24);
    }
    
    virtual void    Render        ();
    virtual void    Expose        ( PropertyMap&  pm );
    virtual void    Serialize    ( OutStream&    os     ) const;
    virtual void    Unserialize    ( InStream&        is     );


    DECLARE_SCLASS(LensFlareElement,SNode,LFEL);

protected:
    Vector3D        screenPos;
    float            quadSide;
    DWORD            quadColor;

private:
    DWORD            color;
    float            radius;
    float            position;


    static BaseMesh s_QuadMesh;
    
    friend class    LensFlare;
}; // class LensFlareElement

const float c_FlareMinR = 0.65f;
const float c_FlareMinG = 0.39f;
const float c_FlareMinB = 0.19f;

const float c_FlareMaxR = 1.0f;
const float c_FlareMaxG = 0.8f;
const float c_FlareMaxB = 0.8f;


const int c_SunMapSide            = 16;
const int c_IntensityMapSide    = 16;

/*****************************************************************************/
/*    Class:    LensFlare
/*    Desc:    lens flare effect node
/*****************************************************************************/
class LensFlare : public SNode
{
    Vector3D            m_SunDir;        //  sun light direction
    float                m_SunDistance;  //    distance to the sun
    float                m_SunRadius;    
    float                m_Scale;

    PerspCamera*        m_pSunCamera;    //  camera used to render to sunmap
    SNode*                m_pScene;        //  scene which occludes the sun

    static bool            s_bFrozen;

public:
                        LensFlare();                

    _inl float            GetSunX() const            { return m_SunDir.x; }
    _inl float            GetSunY() const            { return m_SunDir.y; }
    _inl float            GetSunZ() const            { return m_SunDir.z; }

    _inl void            SetSunX( float val )    { m_SunDir.x = val; }
    _inl void            SetSunY( float val )    { m_SunDir.y = val; }
    _inl void            SetSunZ( float val )    { m_SunDir.z = val; }
    _inl void            SetSunDir( const Vector3D& dir ) { m_SunDir = dir; }

    _inl float            GetSunDistance() const        { return m_SunDistance; }
    _inl void            SetSunDistance( float val ) { m_SunDistance = val;  }

    _inl float            GetScale() const { return m_Scale; }
    _inl void            SetScale( float val ) { m_Scale = val; }

    void                AddElement( const char* texName, 
                                    float pos, 
                                    float radius,
                                    DWORD color );
    
    void                AddElement( const char* texName, 
                                    float pos, 
                                    float radius );


    virtual void        Render        ();
    virtual void        Expose        ( PropertyMap&    pmap );
    virtual void        Serialize    ( OutStream&    os     ) const;
    virtual void        Unserialize    ( InStream&        is     );


    void                Create        ( bool bOccluded = false );
    void                CreateSample();
    
    static void            Freeze()    { s_bFrozen = true; }
    static void            Unfreeze()    { s_bFrozen = false; }
    
    DECLARE_SCLASS(LensFlare,SNode,LENS);

protected:
    DWORD                GetRandomFlareColor() const;
    Camera*                CreateSunCamera();

}; // class LensFlare



#endif // __SGLENSFLARE_H__