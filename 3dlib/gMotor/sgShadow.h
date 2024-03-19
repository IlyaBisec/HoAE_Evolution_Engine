/*****************************************************************************/
/*    File:    sgShadow.h
/*    Desc:    Shadow mapping vodoo
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-28-2003
/*****************************************************************************/
#ifndef __SGSHADOW_H__
#define __SGSHADOW_H__

#include "vCamera.h"


/*****************************************************************************/
/*    Class:    ShadowCaster
/*    Desc:    Instance of the shadow caster
/*****************************************************************************/
struct ShadowCaster
{
    ShadowCaster( DWORD id, const Matrix4D& tm ) : m_ModelID(id), m_TM(tm) {}

    DWORD            m_ModelID;
    Matrix4D        m_TM;
    AABoundBox        m_AABB;
}; // struct ShadowCaster

/*****************************************************************************/
/*    Class:    ShadowMapper
/*    Desc:    Base class for nodes implementing different shadow mapping algos
/*****************************************************************************/
class ShadowMapper : public Node
{
public:
                        ShadowMapper();
    virtual void        AddCaster    ( DWORD nodeID, const Matrix4D& tm );

    virtual void        Render        (){}

    DECLARE_SCLASS(ShadowMapper,Node,SHMP);

protected:

    std::vector<ShadowCaster>    m_Casters;        //  array of casters
    LightSource*                        m_pLightSource;    //  light source casting shadows    
}; // class ShadowMapper

/*****************************************************************************/
/*    Class:    BlobShadowMapper
/*    Desc:    Shadow mapper which draws dumb blobs instead of shadows
/*****************************************************************************/
class BlobShadowMapper : public ShadowMapper
{
public:
    BlobShadowMapper();

    virtual void        Render            ();
    virtual void        Expose            ( PropertyMap& pm );
    virtual void        Serialize        ( OutStream& os ) const;
    virtual void        Unserialize        ( InStream& is );
    virtual void        Create            ();

    _inl float            GetBlobWidth    () const { return m_BlobWidth; }
    _inl float            GetBlobHeight    () const { return m_BlobHeight; }
    
    _inl void            SetPivot        ( const Vector3D& pivot ) { m_Pivot = pivot; }

    DECLARE_SCLASS(BlobShadowMapper,ShadowMapper,BLSH);

protected:
    Texture*            m_pBlobTexture;
    Shader*        m_pBlobShadowShader;
    ZBias*                m_pDecalZBias;
    TextureMatrix*        m_pTextureMatrix;

    float                m_BlobWidth;
    float                m_BlobHeight;
    int                    m_BlobType;
    Rct                    m_BlobUV;
    float                m_ModelPivotHeight;
    DWORD                m_BlobColor;
    Vector3D            m_Pivot;

}; // class BlobShadowMapper

class DecalManager;
/*****************************************************************************/
/*    Class:    DBlobShadowMapper
/*    Desc:    Blob shadow mapper, blob is drawn as decal
/*****************************************************************************/
class DBlobShadowMapper : public BlobShadowMapper
{
    Geometry*            m_pQuadMesh;

public:
                        DBlobShadowMapper    ();

    virtual void        Create                ();
    virtual void        Render                ();
    
    void                SetBlobQuad            (    const Vector3D& a,
                                                const Vector3D& b,
                                                const Vector3D& c,
                                                const Vector3D& d );

    DECLARE_SCLASS(DBlobShadowMapper, BlobShadowMapper, DBSH);
}; // class DBlobShadowMapper

/*****************************************************************************/
/*    Class:    ProjectiveShadowMapper
/*    Desc:    Shadow mapper which draws projective shadows
/*****************************************************************************/
class ProjectiveShadowMapper : public ShadowMapper
{
    RenderTarget*                m_pTarget;
    Texture*                    m_pShadowTexture;
    Shader*                m_pCasterShader;
    Shader*                m_pReceiverShader;
    TextureMatrix*                m_pTextureMatrix;

public:
                                ProjectiveShadowMapper();

    virtual void                Render        ();
    virtual void                Expose        ( PropertyMap& pm );
    virtual void                Serialize    ( OutStream& os ) const;
    virtual void                Unserialize    ( InStream& is );

    DECLARE_SCLASS(ProjectiveShadowMapper,ShadowMapper,PRSH);
}; // class ProjectiveShadowMapper

/*****************************************************************************/
/*    Class:    ShadowVolumeMapper
/*    Desc:    Shadow mapper which draws shadow volume shadows
/*            (shadow volumes are CPU-calculated)
/*****************************************************************************/
class ShadowVolumeMapper : public ShadowMapper
{
    Shader*        m_pPass1Shader;
    Shader*        m_pPass2Shader;
    Shader*        m_pQuadShader;
    Geometry*            m_pShadowVolume;
    Geometry*            m_pQuad;

public:
    ShadowVolumeMapper();

    void                Render();
    void                Expose( PropertyMap& pm );
    void                Serialize( OutStream& os ) const;
    void                Unserialize( InStream& is );

    DECLARE_SCLASS(ShadowVolumeMapper,ShadowMapper,SVSH);
}; // class ShadowVolumeMapper

/*****************************************************************************/
/*    Class:    IDShadowMapper
/*    Desc:    Shadow mapper which draws object-id shadows
/*****************************************************************************/
class IDShadowMapper : public ShadowMapper
{
public:
    IDShadowMapper();

    void                Render();
    void                Expose( PropertyMap& pm );
    void                Serialize( OutStream& os ) const;
    void                Unserialize( InStream& is );

    DECLARE_SCLASS(IDShadowMapper,ShadowMapper,IDSH);
}; // class IDShadowMapper

/*****************************************************************************/
/*    Class:    ShadowBlob
/*    Desc:    Hacky blob shadow
/*****************************************************************************/
class ShadowBlob : public MorphedGeometry
{
public:
                        ShadowBlob            ();
    virtual void        Render                ();
    virtual void        Expose                ( PropertyMap& pm );
    void                Serialize            ( OutStream& os ) const;
    void                Unserialize            ( InStream& is );
    
    int                    GetNSegments        () const      { return m_NSegments; }
    void                SetNSegments        ( int nSeg )  { m_NSegments = nSeg; OnChangeStructure(); }

    float                GetBlobWidth        () const      { return m_Width; }
    void                SetBlobWidth        ( float val ) { m_Width = val; OnChangeStructure(); }

    float                GetBlobHeight        () const      { return m_Height; }
    void                SetBlobHeight        ( float val ) { m_Height = val; OnChangeStructure(); }

    float                GetShiftX            () const      { return m_ShiftCenterX; }
    void                SetShiftX            ( float val ) { m_ShiftCenterX = val; OnChangeStructure(); }

    float                GetShiftY            () const      { return m_ShiftCenterY; }
    void                SetShiftY            ( float val ) { m_ShiftCenterY = val; OnChangeStructure(); }

    static void            Freeze                () { s_bFrozen = true; }
    static void            Unfreeze            () { s_bFrozen = false; }

    DECLARE_SCLASS(ShadowBlob,MorphedGeometry,SHBL);

protected:
    virtual void            OnProcessGeometry();
    virtual void            OnChangeStructure();

    float                    m_Width, m_Height;
    DWORD                    m_Color;
    int                        m_NSegments;
    float                    m_ShiftCenterX;
    float                    m_ShiftCenterY;
    Vector3D                m_LightDir;

    static bool                s_bFrozen;
}; // class ShadowBlob

/*****************************************************************************/
/*    Class:    ProjectiveBlob
/*    Desc:    Projective shadow patch
/*****************************************************************************/
class ProjectiveBlob : public ShadowBlob
{
public:
                            ProjectiveBlob        ();
    virtual void            Render                ();
    virtual void            Expose                ( PropertyMap& pm );
    void                    Serialize            ( OutStream& os ) const;
    void                    Unserialize            ( InStream& is );

    DECLARE_SCLASS(ProjectiveBlob,ShadowBlob,PRBL);

protected:
    virtual void            OnChangeStructure();

    Matrix3D                    m_PatchRotTM;    
    int                            m_ShadowMapSide;

    RenderTarget*                m_pTarget;
    Texture*                    m_pTexture;
    OrthoCamera                    m_LightCamera;
    Shader*                m_pShadowMapDSS;
    Shader*                m_pReceiverDSS;    
}; // class ProjectiveBlob



#endif // __SGSHADOW_H__