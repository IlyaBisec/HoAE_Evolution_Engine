/*****************************************************************************/
/*    File:    vShadowManager.h
/*    Desc:    shadow manager interface implementation
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __VSHADOWMANAGER_H__
#define __VSHADOWMANAGER_H__
#include "IShadowManager.h"

/*****************************************************************************/
/*  Class:  ShadowCaster
/*  Desc:   Describes single shadow caster
/*****************************************************************************/
struct ShadowCaster
{
    DWORD           mdlID;      // model id of the caster
	cbCastCallback* CastCallback;//additional callbact to draw shadow from different elements, that are not models
    EntityContext   context;    // caster's context
    Matrix4D        wTM;        // model world transform
    Matrix4D        shTM;       // shadow transform matrix 
    DWORD           frame;      // frame when caster was added to the shadow system
    DWORD           color;      // shadow color
}; // struct ShadowCaster
struct SimpleCaster
{
    Vector3D        Pos;
    Vector3D        Direction;
    float           Length;
    float           Width;
    DWORD           Color;
    unsigned        TexFragmentIdx:31;
    unsigned        Aligning:1;//0-vertical,1-horisontal
};
const float c_MinShadowBoxRatio = 1.0f;
/*****************************************************************************/
/*    Class:    ShadowManager
/*    Desc:    Implementation of the shadow manager
/*****************************************************************************/
class ShadowManager : public IShadowManager
{
    std::vector<ShadowCaster>   m_Casters;          //  array of the dynamic shadow casters
    std::vector<SimpleCaster>   m_Simples;          //  array of simple casters, like billoards
	std::vector<Vector3D>		m_CastBounds;

    DWORD                       m_ShadowColor;      //  color of the currently rendered shadows
    int                         m_ShadowMapID;      //  id of the shadow map texture
    int                         m_ShadowMapID2;     //  id of the shadow map texture
    Matrix4D                    m_ShadowMapTM;      //  texture transform for the shadow map
    Matrix4D                    m_UVPostProjTM;
    bool                        m_bInited;          //  whether manager is initialized
    int                         m_SMapWidth;        //  width of the shadowmap texture
    int                         m_SMapHeight;       //  height of the shadowmap texture

    float                       m_ClipBias;         //  clip plane z-direction shift

    Matrix4D                    m_LightViewTM;      //  light view matrix
    Matrix4D                    m_LightProjTM;      //  light projection matrix
    
    Vector3D                    m_LightDir;         //  current light direction
    bool                        m_bEnabled;
    bool                        m_bNeedClearSMap;
    bool                        m_bClipToGround;
    ShadowQuality               m_ShadowQuality;    //  current shadow rendering quality level

    Vector2D                    m_LT, m_RT, m_LB, m_RB;

public:
                                ShadowManager   ();
    virtual void                Render          ();
    virtual void                DrawDebugInfo   ();
    virtual void                Init            ();
    virtual bool                AddCaster       ( DWORD modelID, const Matrix4D& tm );
    virtual void                AddSimpleCaster ( Vector3D Pos,Vector3D Direction,float Width,float Height,int IndexInPalette,bool Aligning,DWORD Color);
	virtual void				AddProcCaster	( cbCastCallback* cb );
    virtual void                SetShadowColor  ( DWORD color );
    virtual DWORD               GetShadowColor  ( ) { return m_ShadowColor; }
    virtual void                SetLightDir     ( const Vector3D& dir );
    virtual int                 GetShadowMapID  () const { return m_ShadowMapID; }
    virtual const Matrix4D&     CalcShadowMapTM ();
    virtual void                Enable          ( bool bEnable = true ){ m_bEnabled = bEnable; }
    virtual void                SetShadowMapSide( int w, int h = 0 );
    virtual void                SetShadowQuality( ShadowQuality quality );
    virtual ShadowQuality       GetShadowQuality() const { return m_ShadowQuality; }
    virtual void                SetClipBias     ( float bias ) { m_ClipBias = bias; }
    virtual void                SetUVPostProjTM ( const Matrix4D& tm ) { m_UVPostProjTM = tm; }

	virtual void				AddCastingBoundaryPoint(Vector3D Pos){ if(m_bEnabled) m_CastBounds.push_back(Pos); }
	virtual void				AddCastingAABB (AABoundBox& AB);

protected:
    void                        CalculateTSM    ();
    void                        BlurShadowMap   ();
}; // class ShadowManager

#endif // __VSHADOWMANAGER_H__