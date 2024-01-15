/*****************************************************************************/
/*    File:    sgTransformNode.h
/*    Desc:    Movable node classes
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGTRANSFORMNODE_H__
#define __SGTRANSFORMNODE_H__

#include "sgNode.h"
#include "sgNodePool.h"

/*****************************************************************************/
/*    Class:    TransformNode
/*    Desc:    Scene graph node, which can be transformed 
/*****************************************************************************/
class TransformNode : public SNode
{
public:

    TransformNode::TransformNode() :    m_InitialTM( Matrix4D::identity ), 
                                        m_LocalTM( Matrix4D::identity ){ m_bTransform = true; }

    const Matrix4D& GetTransform() const                { return m_LocalTM; }
    void            SetTransform( const Matrix4D& matr );
    void            Transform   ( const Matrix4D& matr ){ m_LocalTM *= matr; }
    const Matrix4D& GetTopTM    () const                { return m_WorldTM; }
    void            SetTopTM    ( const Matrix4D& tm ) { m_WorldTM = tm; }

    void            Reset       ()                      { m_LocalTM.setIdentity(); }
    Vector3D        GetPos      () const { return Vector3D( m_LocalTM.e30, m_LocalTM.e31, m_LocalTM.e32 ); }
    Vector3D        GetDirX     () const { return Vector3D( m_LocalTM.e00, m_LocalTM.e01, m_LocalTM.e02 ); }
    Vector3D        GetDirY     () const { return Vector3D( m_LocalTM.e10, m_LocalTM.e11, m_LocalTM.e12 ); }
    Vector3D        GetDirZ     () const { return Vector3D( m_LocalTM.e20, m_LocalTM.e21, m_LocalTM.e22 ); }
    void            SetPos      ( Vector3D v ) { m_LocalTM.e30 = v.x; m_LocalTM.e31 = v.y; m_LocalTM.e32 = v.z;}
    void            SetDirX( Vector3D v )
    {
        m_LocalTM.e00 = v.x; m_LocalTM.e01 = v.y; m_LocalTM.e02 = v.z;
        m_InitialTM.e00 = v.x; m_InitialTM.e01 = v.y; m_InitialTM.e02 = v.z;
    }

    void            SetDirY( Vector3D v )
    {
        m_LocalTM.e10 = v.x; m_LocalTM.e11 = v.y; m_LocalTM.e12 = v.z;
    }

    void            SetDirZ( Vector3D v )
    {
        m_LocalTM.e20 = v.x; m_LocalTM.e21 = v.y; m_LocalTM.e22 = v.z;
    }

    float           GetPosX         () const    { return m_LocalTM.e30; }
    float           GetPosY         () const    { return m_LocalTM.e31; }
    float           GetPosZ         () const    { return m_LocalTM.e32; }
    void            SetPosX         ( float v ) { m_LocalTM.e30 = v; }
    void            SetPosY         ( float v ) { m_LocalTM.e31 = v; }
    void            SetPosZ         ( float v ) { m_LocalTM.e32 = v; }
    float           GetScaleX       () const    { return GetDirX().norm(); }
    float           GetScaleY       () const    { return GetDirY().norm(); }
    float           GetScaleZ       () const    { return GetDirZ().norm(); }    
    void            SetScaleX       ( float val );
    void            SetScaleY       ( float val );
    void            SetScaleZ       ( float val );
    
    const Matrix4D&         GetInitialTM        () const    { return m_InitialTM; }
    void                    SetInitialTM        ( const Matrix4D& matr ) { m_InitialTM = matr; }

    //  euler angles are in degrees
    float                   GetEulerX           () const;        
    float                   GetEulerY           () const;
    float                   GetEulerZ           () const;    

    void                    SetEulerX           ( float val );
    void                    SetEulerY           ( float val );
    void                    SetEulerZ           ( float val ); 

    void                    FlipAxis            ();
    void                    SetToInitial        ();
    void                    SetSubtreeToInitial ();

    static const Matrix4D&  TMStackTop          () { return s_TMStack.Top(); }
    static void             ResetTMStack        ( const Matrix4D* pTm = NULL );

    void                    PushTM              ( const Matrix4D& m, bool bOverride = false ) { s_TMStack.Push( m, bOverride ); m_WorldTM = s_TMStack.Top(); }
    const Matrix4D&         PopTM               () { return s_TMStack.Pop(); }

    static void             Push                ( const Matrix4D& m ) { s_TMStack.Push( m ); }
    static const Matrix4D&  Pop                 () { return s_TMStack.Pop(); }
    
    static void             SetCalcWorldTM      ( bool bCalc = true ) { s_bCalcWorldTM = bCalc; }
    Matrix4D                GetWorldTM          () const;
    void                    SetWorldTM          ( const Matrix4D& wTM );
    Matrix4D                GetParentWorldTM    () const;

    virtual void            Render              ();
    virtual void            Serialize           ( OutStream& os ) const;
    virtual void            Unserialize         ( InStream& is  );
    virtual void            Expose              ( PropertyMap& pm );

    DECLARE_SCLASS(TransformNode,SNode,MOVB);
    
protected:
    Matrix4D                m_LocalTM;          //  current node parent-related transform
    Matrix4D                m_WorldTM;          //  current node world transform

    Matrix4D                m_InitialTM;        //  initial node parent-related transform

    static MatrixStack      s_TMStack;
    static bool             s_bCalcWorldTM;
}; // class TransformNode 

/*****************************************************************************/
/*    Class:    HudNode
/*    Desc:    Scene graph node, which has position in the screen space
/*****************************************************************************/
class HudNode : public SNode
{
public:    
                            HudNode() { scale = 1.0f; pos.zero(); width = height = 0.0f; }

    _inl float                GetPosX        () const;
    _inl float                GetPosY        () const;
    _inl float                GetPosZ        () const;

    _inl void                SetPosX        ( float val );
    _inl void                SetPosY        ( float val );
    _inl void                SetPosZ        ( float val );

    _inl float                GetScale    () const;
    _inl void                SetScale    ( float val );

    _inl float                GetWidth    () const;
    _inl float                GetHeight    () const;

    _inl void                SetWidth    ( float val );
    _inl void                SetHeight    ( float val );
    
    virtual Rct                GetBounds();

    virtual void            Serialize    ( OutStream& os ) const;
    virtual void            Unserialize    ( InStream& is );
    virtual void            Expose        ( PropertyMap& pm );

    DECLARE_SCLASS(HudNode,SNode,HUDL);

protected:
    
    Vector3D                pos;
    float                    width, height;
    float                    scale;

}; // class HudElement

/*****************************************************************************/
/*    Class:    Transform2D
/*    Desc:    Transformation node on the plane
/*****************************************************************************/
class Transform2D : public SNode
{
public:
    virtual void Serialize            ( OutStream& os ) const;
    virtual void Unserialize        ( InStream& is    );
    void         SetTransform        ( const Matrix3D& m ) { m_LocalTM = m; }
    
    DECLARE_SCLASS(Transform2D,SNode,TR2D);

protected:
    Matrix3D        m_LocalTM;
}; // class Transform2D

Matrix4D GetWorldTM( SNode* pNode );



#ifdef _INLINES 
#include "sgTransformNode.inl"
#endif // _INLINES

#endif // __SGTRANSFORMNODE_H__