/*****************************************************************************/
/*    File:    vModelInstance.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-06-2005
/*****************************************************************************/
#ifndef __VMODELINSTANCE_H__
#define __VMODELINSTANCE_H__

#include "kTimer.h"

/*****************************************************************************/
/*    Class:    BoneInstance
/*****************************************************************************/
struct BoneInstance
{
    TransformNode*              m_pBone;
    Matrix4D                    m_TM;
    BoneInstance( TransformNode* pBone ) : m_pBone( pBone ) 
    {
        m_TM = pBone->GetTransform();
    }
}; // class BoneInstance

/*****************************************************************************/
/*    Class:    SkinInstance
/*****************************************************************************/
struct SkinInstance
{
    Skin*                       m_pSkin;
    std::vector<VertexN>        m_Vertices;

                                SkinInstance( Skin* pSkin ) : m_pSkin( pSkin ) {}
}; // class SkinInstance

/*****************************************************************************/
/*    Class:    ModelInstance
/*****************************************************************************/
class ModelInstance
{
    SNode*                          m_pModel;
    std::vector<BoneInstance>       m_Bones;
    std::vector<SkinInstance>       m_Skin;
    
    DWORD                           m_AnimID;
    float                           m_AnimTime;

    DWORD                           m_PrevAnimID;
    float                           m_PrevAnimTime;


    DWORD                           m_LastFrame;
    
    SNode*                          m_pPrevController;
    Timer                           m_BlendTimer;
    bool                            m_bBlending;
    bool                            m_bPreventBlending;
    Matrix4D                        m_RootTM;

public:
                    ModelInstance   ();
                    ModelInstance   ( SNode* pModel, const Matrix4D& rootTM ) :    
														  m_pModel            ( NULL  ),  
                                                          m_LastFrame         ( 0     ), 
                                                          m_bBlending         ( false ), 
                                                          m_pPrevController   ( NULL  ),
                                                          m_bPreventBlending  ( true )
                    {
                        Create( pModel, rootTM );
                    }
    void            Create              ( SNode* pModel, const Matrix4D& rootTM );
    void            Animate             ( SNode* pController, const Matrix4D& rootTM, float blendInTime = 0.8f );
    void            Render              ();
    SNode*          GetModel            () const { return m_pModel; }
    void            SetPreventBlending  ( bool bPrevent ) { m_bPreventBlending = bPrevent; }
    void            SetRootTM           ( const Matrix4D& tm ) { m_RootTM = tm; }
    void            OnUpdateNode        ( SNode* pNode );
    Matrix4D        GetNodeTransform    ( DWORD nodeID );

}; // class ModelInstance

#endif // __VMODELINSTANCE_H__
