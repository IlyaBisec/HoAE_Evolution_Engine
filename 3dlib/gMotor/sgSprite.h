/*****************************************************************************/
/*    File:    sgSprite.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    09-12-2003
/*****************************************************************************/
#ifndef __SGSPRITE_H__
#define __SGSPRITE_H__

static const int    c_BadHandle = -1;
/*****************************************************************************/
/*    Class:    ScreenSprite
/*    Desc:    Single sprite reference
/*****************************************************************************/
class ScreenSprite : public HudNode
{
    int                 m_FrameIdx;
    std::string         m_FileName;
    DWORD               m_Color;
    int                 m_Handle;

public:
                        ScreenSprite() : 
                            m_Handle( c_BadHandle ), 
                            m_Color(0xFFFFFFFF), m_FrameIdx(0) {}                    

    _inl void           SetFrameIndex   ( int val )    { m_FrameIdx = val;    }
    void                SetPackageName  ( const char* name );
    
    _inl int            GetFrameIndex   () const { return m_FrameIdx; }
    _inl const char*    GetPackageName  () const { return m_FileName.c_str(); }
    _inl int            GetGPHandle     () const { return m_Handle; }
    _inl DWORD          GetColor        () const { return m_Color; }    
    int                 GetFrameWidth   () const;
    int                 GetFrameHeight  () const;

    _inl void           SetColor        ( DWORD val ) { m_Color &= 0xFF000000; m_Color |= val; }
    _inl void           SetGPHandle     ( int   val ) { m_Handle = val;}

    _inl void           GPToScreenSpace ( Vector3D& vec ); 
    _inl void           GPToScreenSpace ( Vector4D& vec );
    _inl void           ScreenToGPSpace ( Vector3D& vec ); 
    _inl void           ScreenToGPSpace ( Vector4D& vec );

    virtual Rct         GetBounds       ();

    _inl float          GetAlpha        () const { return float( (m_Color & 0xFF000000) >> 24 )/255.0f; }            
    _inl void           SetAlpha        ( float val ) 
    { 
        m_Color &= 0x00FFFFFF; 
        m_Color |= DWORD( val * 255.0f ) << 24;
    }                


    virtual void        Render           ();
    virtual void        Serialize        ( OutStream& os ) const;
    virtual void        Unserialize      ( InStream&     is );
    virtual void        Expose           ( PropertyMap& pm );
    bool                Precache         ();

    DECLARE_SCLASS(ScreenSprite,HudNode,GPFR);
}; // class ScreenSprite

/*****************************************************************************/
/*    Class:    WorldSprite
/*****************************************************************************/
class WorldSprite : public TransformNode
{
    int                     m_Handle;
    int                     m_RealFrame;
    int                     m_AnimFrame;
    int                     m_NDirections;
    
    bool                    m_bHalfCircle;
    bool                    m_bBillboard;
    bool                    m_bAnimate;

    std::string             m_GPName;

    Vector3D                m_Pivot;
    float                   m_Scale;
    int                     m_Direction;
    DWORD                   m_Color;
    DWORD                   m_PlayerColor;
    int                     m_LOD;
    int                     m_ShaderID;

    //  bounding volume
    float                   m_MaxH;
    float                   m_MaxHalfW;
    
    _inl AABoundBox         GetAABB() const 
    { 
        return AABoundBox( Vector3D( m_LocalTM.e30 - m_MaxHalfW, 
                                     m_LocalTM.e31 - m_MaxHalfW, 
                                     m_LocalTM.e32 ),
                           Vector3D( m_LocalTM.e30 + m_MaxHalfW, 
                                     m_LocalTM.e31 + m_MaxHalfW, 
                                     m_LocalTM.e32 + m_MaxH ) );
    }

public:
                            WorldSprite     ();
    virtual void            Render          ();

    virtual void            Serialize       ( OutStream& os ) const;
    virtual void            Unserialize     ( InStream&     is );
    virtual void            Expose          ( PropertyMap& pm );
    void                    SetColor        ( DWORD color ) { m_Color = color; }

    _inl bool               Intersect       ( int scrX, int scrY, const Ray3D& ray, TestPrecision precision ) const;
    _inl bool               Intersect       ( const Frustum& fr ) const;

    bool                    Precache        ();
    int                     GetNFrames      ();
    int                     GetFrameWidth   () const;
    int                     GetFrameHeight  () const;

    const char*             GetShaderID     () const { return ""; }
    void                    SetShaderID     ( const char* name );

    void                    SetPackageName  ( const char* _gpName );
    _inl const char*        GetPackageName  () const { return m_GPName.c_str();    }
    _inl Matrix4D           GetImpostorMatrix();

    _inl void               SetAnimFrame    ( int frame );
    _inl int                GetAnimFrame    () const { return m_AnimFrame; }

    DECLARE_SCLASS(WorldSprite,TransformNode,GPIM);
protected:

}; // WorldSprite



#ifdef _INLINES
#include "sgSprite.inl"
#endif // _INLINES

#endif // __SGSPRITE_H__
