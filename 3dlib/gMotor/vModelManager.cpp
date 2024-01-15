/*****************************************************************************/
/*	File:	vModelManager.cpp
/*	Desc:	ModelObject manager interface implementation
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#include "stdafx.h"

#include "sg.h"
#include "sgAnimBlend.h"

#include "kContext.h"
#include "kHash.hpp"
#include "kFilePath.h"
#include "kTimer.h"

#include "mFloatCurve.h"
#include "mQuatCurve.h"

#include "IMesh.h"

#include "vSkelAnimTrack.h"

#include "vMesh.h"
#include "vStaticMesh.h"
#include "vSkin.h"
#include "vModel.h"
#include "vSkeletonAnimation.h"

#include "vFKController.h"
#include "vIKConstraint.h"
#include "vSkelAnimTrack.h"

#include "vModelInstance.h"
#include "vModelManager.h"

/*****************************************************************************/
/*  ModelManager implementation
/*****************************************************************************/

ModelManager* GetModelManager()
{
    static ModelManager s_ModelMgr;
    return &s_ModelMgr;
}

IModelManager*  IModelMgr = GetModelManager();
ModelManager::ModelManager()
{
    IModelMgr           = this;
    m_CurrentModel      = -1;
    m_CurrentInstance   = -1;
    m_Timer.start();

    LINK_CLASS(StaticMesh   );
    LINK_CLASS(Skin         );

    LINK_CLASS(AnimTrack    );
    LINK_CLASS(BindEvent    );
    LINK_CLASS(EffectTrack  );
    LINK_CLASS(BonePosTrack );
    LINK_CLASS(BoneDirTrack );
    LINK_CLASS(ClipRefTrack  );
} // ModelManager::ModelManager

ModelManager::~ModelManager()
{
    for (int i = 0; i < m_Models.size(); i++) delete m_Models[i];
    for (int i = 0; i < m_Anims.size(); i++) delete m_Anims[i];    
} // ModelManager::~ModelManager

bool ConvertModel( const char* fname, ModelObject* res );
int ModelManager::GetModelID( const char* name )
{
    for (int i = 0; i < m_Models.size(); i++)
    {
        if (!stricmp( name, m_Models[i]->GetName() )) return i;
    }

    FilePath path( name );
    path.SetExt( "c2m" );
    int resID = IRM->FindResource( path.GetFullPath() );
    if (resID == -1) return -1;
    int size = 0;
    BYTE* pData = IRM->LockData( resID, size );
    if (!pData) return -1;
    MemInStream is( pData, size );
    ModelObject* pModel = new ModelObject;
    if (!pModel->Unserialize( is ))
    {
        std::string fname   = path.GetFileName();
        std::string ext     = path.GetExt();
        path.SetDir     ( IRM->GetPath( resID ) );
        path.SetFileName( fname.c_str() );
        path.SetExt     ( ext.c_str() );
        if (!ConvertModel( path, pModel )) 
        {
            delete pModel;
            return -1;
        }
    }
    IRM->UnlockData( resID );
    pModel->SetName( path.GetFileName() );
    int id = m_Models.size();
    pModel->m_ID = id;
    m_Models.push_back( pModel );
    return id;
} // ModelManager::GetModelID

bool ModelManager::ReloadModel( int mdlID )
{
    if (mdlID < 0 || mdlID >= m_Models.size()) return false;
    ModelObject* pModel = m_Models[mdlID];
    FilePath path( pModel->GetName() );
    path.SetExt( "c2m" );
    int resID = IRM->FindResource( path.GetFileWithExt() );
    if (resID == -1) return false;
    int size = 0;
    BYTE* pData = IRM->LockData( resID, size );
    if (!pData) return false;
    MemInStream is( pData, size );
    pModel->Reset();
    if (!pModel->Unserialize( is ))
    {
        if (!ConvertModel( path, pModel )) return false;
    }
    return true;
} // ModelManager::ReloadModel

ModelObject* ModelManager::GetModel( int idx )
{
    if (idx < 0 || idx >= m_Models.size()) return NULL;
    return m_Models[idx];
}

bool ModelManager::UnloadModel( int mdlID )
{
    return true;
}

bool ConvertAnimation( const char* fname, SkeletonAnimation* pAnm );
int ModelManager::GetAnimID( const char* name )
{
    for (int i = 0; i < m_Anims.size(); i++)
    {
        if (!stricmp( name, m_Anims[i]->GetName() )) return i;
    }

    FilePath path( name );
    path.SetExt( "c2m" );
    int resID = IRM->FindResource( path.GetFileWithExt() );
    if (resID == -1) return -1;
    int size = 0;
    BYTE* pData = IRM->LockData( resID, size );
    if (!pData) return -1;
    MemInStream is( pData, size );
    SkeletonAnimation* pAnim = new SkeletonAnimation;
    if (!pAnim->Unserialize( is ))
    {
        std::string fname   = path.GetFileName();
        std::string ext     = path.GetExt();
        path.SetDir     ( IRM->GetPath( resID ) );
        path.SetFileName( fname.c_str() );
        path.SetExt     ( ext.c_str() );
        if (!ConvertAnimation( path, pAnim )) 
        {
            delete pAnim;
            return -1;
        }
    }
    pAnim->SetName( path.GetFileName() );
    int id = m_Anims.size();
    pAnim->SetID( id );
    m_Anims.push_back( pAnim );
    return id;
} // ModelManager::GetAnimID

int ModelManager::GetNAnims() const
{
    return 0;
}

IModel* ModelManager::GetAnim( int idx )
{
    return NULL;
}

bool ModelManager::UnloadAnim( int anmID )
{
    return false;
} // ModelManager::UnloadAnim

ModelObject* ModelManager::GetModel()
{
    ModelInstance* pInst = GetModelInstance();
    if (!pInst) return NULL;
    int mID = pInst->m_ModelID;
    if (mID < 0 || mID >= m_Models.size()) return NULL;
    return m_Models[mID];
} // ModelManager::GetModel

ModelInstance* ModelManager::GetModelInstance()
{
    if (m_CurrentInstance > 0) return &m_ModelInstHash[m_CurrentInstance];
    int instID = m_ModelInstHash.find( m_ModelContext );
    if (instID == -1)
    {
        if (m_CurrentModel < 0 || m_CurrentModel >= m_Models.size()) return NULL;
        ModelObject* pModel = m_Models[m_CurrentModel];
        instID = m_ModelInstHash.add( m_ModelContext );   
        ModelInstance& inst = m_ModelInstHash[instID];
        if (instID == -1) return NULL;
        inst.SetModel( pModel );
        inst.m_RootTM.setIdentity();
        inst.m_ID = instID;
    }
    m_CurrentInstance = instID;
    return &m_ModelInstHash[m_CurrentInstance];
} // ModelManager::GetModelInstance

void ModelManager::Draw( const Matrix4D* pTM )
{
    ModelInstance* m = GetModelInstance();
    if (!m) return;
    if (pTM) m->SetRootTM( *pTM ); 
    m->ProcessTracks( m_Timer.seconds() );
    ModelObject* pModel = m_Models[m->m_ModelID];
    int nMeshes = pModel->GetNMeshes();
    for (int i = 0; i < nMeshes; i++)
    {
        Mesh* pMesh = (Mesh*)pModel->GetMesh( i );
        if (!pMesh->IsVisible()) continue;
        pMesh->Draw( m ); 
    }
} // ModelManager::Draw

int ModelManager::RegisterModel( IModel* pModel ) 
{ 
    return -1; 
} // ModelManager::RegisterModel

int ModelManager::GetNSlots( int mdlID ) const
{
    return 0;
} // ModelManager::GetNSlots

int ModelManager::GetNEffectors( int mdlID ) const
{
    return 0;
} // ModelManager::GetNEffectors

int ModelManager::GetNMeshes( int mdlID ) const
{
    return 0;
} // ModelManager::GetNMeshes

int ModelManager::GetSlotID( int mdlID, const char* name ) const
{
    return -1;
} // ModelManager::GetSlotID

int ModelManager::GetEffectorID( int mdlID, const char* name ) const
{
    return -1;
} // ModelManager::GetEffectorID

int ModelManager::GetMeshID( int mdlID, const char* name ) const
{
    return -1;
} // ModelManager::GetMeshID

void ModelManager::SetModel( int mdlID )
{
    m_CurrentModel = mdlID;
} // ModelManager::SetModel

int ModelManager::ScheduleAnimation( float scheduleTime, int anmID, bool  bLooped, float phase, float playTime, float speed, float easeIn, float easeOut, float weight )
{
    if (anmID < 0 || anmID >= m_Anims.size()) return -1;
    ModelInstance* m = GetModelInstance();
    if (!m) return -1;

    SkeletonAnimation* pAnim = m_Anims[anmID];
    if (!pAnim->IsBound()) pAnim->Bind( m_Models[m->m_ModelID] );
    if (playTime < 0.0f) playTime = GetAnimTime( anmID )/speed;    
    float cTime = m_Timer.seconds();
    SkelAnimTrack* pTrack = new SkelAnimTrack();
    pTrack->m_pAnimation        = pAnim;

    pTrack->m_StartOperateTime  = cTime + scheduleTime;
    pTrack->m_LastOperateTime   = cTime;
    pTrack->m_OperateTime       = playTime;

    pTrack->m_EaseInTime        = easeIn; 
    pTrack->m_Weight            = weight;
    pTrack->m_LoopMode          = bLooped ? lmCycle : lmNone;
    pTrack->m_pModelInstance    = m;

    pTrack->m_AnimLength        = GetAnimTime( anmID );  
    pTrack->m_AnimPhase         = phase;  
    pTrack->m_AnimSpeed         = speed;
    return m->AddController( pTrack );
} // ModelManager::ScheduleAnimation

int ModelManager::ScheduleBonePos( float scheduleTime, int boneID, const Vector3D& pos, float time, float w )
{
    ModelInstance* m = GetModelInstance();
    if (!m) return -1;
    if (boneID < 0 || boneID >= m->GetNBones()) return -1;

    float cTime = m_Timer.seconds();
    FKController* pTrack = new FKController( fkcPosition );
    pTrack->m_BoneID            = boneID;
    pTrack->m_StartOperateTime  = cTime + scheduleTime;
    pTrack->m_LastOperateTime   = cTime;
    pTrack->m_Weight            = w;
    pTrack->m_OperateTime       = time;
    pTrack->m_TargetPos         = pos;
    pTrack->m_pModelInstance    = m;
    return m->AddController( pTrack );
} // ModelManager::SetBonePos

int ModelManager::ScheduleBoneDir( float scheduleTime, int boneID, const Matrix3D& rot, float time, float w )
{
    ModelInstance* m = GetModelInstance();
    if (!m) return -1;
    if (boneID < 0 || boneID >= m->GetNBones()) return -1;

    Quaternion tgRot;
    tgRot.FromMatrix( rot );

    float cTime = m_Timer.seconds();
    FKController* pTrack = new FKController( fkcRotation );
    pTrack->m_BoneID            = boneID;
    pTrack->m_StartOperateTime  = cTime + scheduleTime;
    pTrack->m_LastOperateTime   = cTime;
    pTrack->m_Weight            = w;
    pTrack->m_OperateTime       = time;
    pTrack->m_TargetRot         = tgRot;
    pTrack->m_pModelInstance    = m;
    return m->AddController( pTrack );
} // ModelManager::SetBoneDir

int ModelManager::ScheduleBoneTM( float scheduleTime, int boneID, const Matrix4D& tm, float time, float w ) 
{
    ModelInstance* m = GetModelInstance();
    if (!m) return -1;
    if (boneID < 0 || boneID >= m->GetNBones()) return -1;

    Vector3D tgScale, tgPos;
    Quaternion tgRot;
    tm.Factorize( tgScale, tgRot, tgPos );

    float cTime = m_Timer.seconds();
    FKController* pTrack = new FKController( fkcTransform );
    pTrack->m_BoneID            = boneID;
    pTrack->m_StartOperateTime  = cTime + scheduleTime;
    pTrack->m_LastOperateTime   = cTime;
    pTrack->m_Weight            = w;
    pTrack->m_OperateTime       = time;
    pTrack->m_TargetPos         = tgPos;
    pTrack->m_TargetScale       = tgScale;
    pTrack->m_TargetRot         = tgRot;
    pTrack->m_pModelInstance    = m;
    return m->AddController( pTrack );
} // ModelManager::SetBoneTM

float ModelManager::GetAnimTime( int anmID )
{
    if (anmID < 0 || anmID >= m_Anims.size()) return -1;
    return m_Anims[anmID]->GetPlayTime();
} // ModelManager::GetAnimTime

int ModelManager::ScheduleIK(   float           scheduleTime,
                                int             effectorID, 
                                const Vector3D& pos,
                                float           easeIn, 
                                float           easeOut, 
                                float           weight )
{
    return -1; 
} // ModelManager::BindEffector

bool ModelManager::BindSlot( int slotID, int mdlID, const Matrix4D& bindTM )
{
    return false;
} // ModelManager::BindSlot

bool ModelManager::SetMeshShader( int meshID, int shaderID )
{
    return false;
}

bool ModelManager::EnableMesh( int meshID, bool bEnable )
{
    return false;
} // ModelManager::EnableMesh

void ModelManager::StopController( int trackID )
{
    ModelInstance* m = GetModelInstance();
    if (!m) return;
    m->StopTrack( trackID );
}

void ModelManager::StopController()
{
    ModelInstance* m = GetModelInstance();
    if (!m) return;
    m->StopAllTracks();
}

int ModelManager::GetNTracks()
{
    return 0;
}

/************************************************************************/
/*  Globals
/************************************************************************/
bool ConvertModel( const char* fname, ModelObject* res )
{
    //try{
        
        Node* pMdl = Node::CreateFromFile( fname );
        if (!pMdl) return false;
        
        //  extract bone list
        std::vector<TransformNode*>	bones;
        Node::Iterator bit( pMdl );
        while (bit)
        {
        	Node* pNode = (Node*)*bit;
        	++bit;
        	if (!pNode->IsA<TransformNode>()/* || pNode == pMdl*/) continue;
        	TransformNode* pTM = (TransformNode*)pNode;
        	bool bNoBone = true;
        	for (int i = 0; i < bones.size(); i++) 
        	{ 
        		if (bones[i] == pTM) 
        		{
        			bNoBone = false; 
        			break; 
        		} 
        	}
        	if (bNoBone) bones.push_back( pTM );
        }

        Skeleton& sk = res->m_Skeleton;
        
        //  fill skeleton bones
        int nBones = bones.size();
        for (int i = 0; i < nBones; i++)
        {
            TransformNode* pTN = bones[i];
            ModelBone bone;
            bone.m_Name         = pTN->GetName();
            bone.m_LocalTM      = pTN->GetTransform();
            bone.m_ID           = i;        
            bone.m_WorldTM      = GetWorldTM( pTN );
            bone.m_Offset       = bone.m_WorldTM;
            bone.m_Offset.inverse();
            sk.push_back( bone );
        }
        //  establish parent-child bone relationships
        for (int i = 0; i < nBones; i++)
        {
            TransformNode* pTN   = bones[i];
            ModelBone& bone         = sk[i];
            //  find parent node
            Node* pParent = pTN->GetParent();
            while (pParent && !pParent->IsA<TransformNode>()) 
            {
                pParent = pParent->GetParent();
            }
            for (int j = 0; j < nBones; j++) if (pParent == bones[j]) bone.m_pParent = &sk[j];
            //  find children nodes
            int nCh = pTN->NumChildren();
            for (int j = 0; j < nCh; j++)
            {
                Node* pChild = pTN->GetChild( j );
                for (int k = 0; k < nBones; k++) 
                {
                    if (pChild == bones[k]) 
                    {
                        bone.AddChild( &sk[k] );
                    }
                }
            }
        }
        
        res->m_Shaders.reserve( 256 );

        vector<Geometry*>   geoms;
        TransformNode* pCurTM = NULL;
        Node::Iterator it( pMdl );
        Texture*        pTex0 = NULL;
        Texture*        pTex1 = NULL;
        Texture*        pTex2 = NULL;
        Texture*        pTex3 = NULL;
        TransformNode*  pTM   = NULL;
        Shader* pSha  = NULL;
        while (it)
        {
            Node* pNode = (Node*)*it;
            ++it;
            if (pNode->IsA<Texture>())
            {
                Texture* pTex = (Texture*)pNode;
                if (pTex->GetStage() == 0) pTex0 = pTex; 
                if (pTex->GetStage() == 1) pTex1 = pTex; 
                if (pTex->GetStage() == 2) pTex2 = pTex; 
                if (pTex->GetStage() == 3) pTex3 = pTex; 
            }
            if (pNode->IsA<Shader>())
            {
                pSha = (Shader*)pNode;
            }
            if (pNode->IsA<TransformNode>()) pTM = (TransformNode*)pNode;  
            if (pNode->IsA<Geometry>())
            {
        	    Geometry* pGeom = (Geometry*)pNode;
                bool bAlreadyProcessed = false;
                for (int i = 0; i < geoms.size(); i++) if (pGeom == geoms[i]) bAlreadyProcessed = true;
                if (bAlreadyProcessed) continue;
                geoms.push_back( pGeom );

                //  create shader
                ModelShader shInst;
                if (pTex0) shInst.SetTex0( pTex0->GetName() );
                if (pTex1) shInst.SetTex1( pTex1->GetName() );
                if (pTex2) shInst.SetTex2( pTex2->GetName() );
                if (pTex3) shInst.SetTex3( pTex3->GetName() );
                if (pSha)  shInst.SetScriptName( pSha->GetName() );
                int cMaterial = -1;
                for (int i = 0; i < res->m_Shaders.size(); i++)
                {
                    if (res->m_Shaders[i] == shInst) 
                    {
                        cMaterial = i;
                        break;
                    }
                }
                if (cMaterial == -1)
                {
                    cMaterial = res->m_Shaders.size();
                    shInst.SetID( cMaterial );
                    res->m_Shaders.push_back( shInst );
                }

                int boneID = -1;
                for (int j = 0; j < nBones; j++) if (pTM == bones[j]) boneID = j;

                if (pNode->IsA<SkinnedGeometry>())
                {
                    SkinnedGeometry* pSGeom = (SkinnedGeometry*)pNode;
                    BaseMesh& bm = pSGeom->GetBaseMesh();
                    static vector<int> boneReindex; boneReindex.clear();
                    for (int i = 0; i < pSGeom->GetNChildren(); i++)
                    {
                        TransformNode* pChild = (TransformNode*)pSGeom->GetChild( i );
                        if (!pChild->IsA<TransformNode>()) continue;
                        int boneIdx = -1;
                        for (int j = 0; j < nBones; j++) if (pChild == bones[j]) boneIdx = j;
                        assert( boneIdx > 0 );
                        sk[boneIdx].m_Offset = pSGeom->GetBoneOffset( i );
                        boneReindex.push_back( boneIdx );
                    }
                    Skin* pSkin = new Skin;
                    int nV = bm.getNVert();
                    switch (bm.getVertexFormat())
                    {
                    case vfVertex1W: pSkin->SetSkinCluster( 0, 0, nV ); break;
                    case vfVertex2W: pSkin->SetSkinCluster( 1, 0, nV ); break;
                    case vfVertex3W: pSkin->SetSkinCluster( 2, 0, nV ); break;
                    case vfVertex4W: pSkin->SetSkinCluster( 3, 0, nV ); break;
                    }

                    pSkin->SetName          ( pNode->GetName() );
                    pSkin->SetModel         ( res );
                    pSkin->SetVDecl         ( CreateVertexDeclaration( bm.getVertexFormat() ) ); 
                    pSkin->SetPrimitiveType ( ptTriangleList ); 
                    pSkin->SetVertices      ( bm.getVertexData(), nV ); 
                    pSkin->SetIndices       ( (const BYTE*)bm.getIndices(), bm.getNInd() ); 
                    pSkin->SetMaterialID    ( cMaterial );
                    pSkin->CalcAABB         ();
                    pSkin->CalcBoundSphere  ();    
                    
                    VertexIterator vit( (BYTE*)pSkin->GetVertexData(), nV, pSkin->GetVDecl() );
                    while (vit)
                    {
                        for (int i = 0; i < c_MaxBlendWeights; i++) 
                        {
                            vit.blend_idx( i ) = boneReindex[vit.c_blend_idx( i )];
                        }
                        ++vit;
                    }
                    res->m_Meshes.push_back ( pSkin );
                }
                else
                {
                    BaseMesh* bm = &pGeom->GetMesh();
                    StaticMesh* pMesh = new StaticMesh;     
                    pMesh->SetName          ( pNode->GetName() );
                    pMesh->SetModel         ( res );
                    pMesh->SetVDecl         ( CreateVertexDeclaration( bm->getVertexFormat() ) ); 
                    pMesh->SetPrimitiveType ( ptTriangleList ); 
                    pMesh->SetVertices      ( bm->getVertexData(), bm->getNVert() ); 
                    pMesh->SetIndices       ( (const BYTE*)bm->getIndices(), bm->getNInd() ); 
                    pMesh->SetMaterialID    ( cMaterial );
                    pMesh->SetHostBone      ( boneID    );
                    pMesh->SetHostBoneName  ( pTM->GetName() );
                    pMesh->CalcAABB         ();
                    pMesh->CalcBoundSphere  ();
                    res->m_Meshes.push_back ( pMesh     );
                }
            }
        }    

        res->CalcAABB();
        res->CalcBoundSphere();
  /*  }
    catch(...)
    {
        return false;
    }*/

    //  sort skin clusters
    return true;
} // ConvertModel

bool ConvertAnimation( const char* fname, SkeletonAnimation* pAnm )
{
    
    Node* pNode = Node::CreateFromFile( fname );
    if (!pNode || !pNode->IsA<AnimationBlock>()) return false;
    AnimationBlock* pAnimBlock = (AnimationBlock*)pNode;
    pAnm->SetPlayTime( pAnimBlock->GetAnimationTime()*0.001f );
    Node::Iterator it( pNode );
    while (it)
    {
        Node* pNode = (Node*)*it;
        if (pNode->IsA<PRSAnimation>())
        {
            PRSAnimation* pAnim = (PRSAnimation*)pNode;
            PRSTrack track;
            track.SetName( pAnim->GetName() );
            //  copy keyframes
            if (pAnim->IsConstant())
            {
                track.SetIsPose( true );
            }
            else
            {
                track.SetIsPose( false );
                const FloatAnimationCurve* pPosX = pAnim->GetPosX();
                const FloatAnimationCurve* pPosY = pAnim->GetPosY();
                const FloatAnimationCurve* pPosZ = pAnim->GetPosZ();
                const QuatAnimationCurve*  pRot  = pAnim->GetRot();
                const FloatAnimationCurve* pScX  = pAnim->GetScaleX();
                const FloatAnimationCurve* pScY  = pAnim->GetScaleY();
                const FloatAnimationCurve* pScZ  = pAnim->GetScaleZ();

                for (int i = 0; i < pPosX->GetNKeys(); i++) track.AddPosXKey( pPosX->GetKeyTime( i )*0.001f, pPosX->GetKeyValue( i ) );
                for (int i = 0; i < pPosY->GetNKeys(); i++) track.AddPosYKey( pPosY->GetKeyTime( i )*0.001f, pPosY->GetKeyValue( i ) );
                for (int i = 0; i < pPosZ->GetNKeys(); i++) track.AddPosZKey( pPosZ->GetKeyTime( i )*0.001f, pPosZ->GetKeyValue( i ) );
                for (int i = 0; i < pRot->GetNKeys(); i++)  track.AddRotKey ( pRot->GetKeyTime( i )*0.001f, pRot->GetKeyValue( i ) );
                for (int i = 0; i < pScX->GetNKeys(); i++)  track.AddSXKey  ( pScX->GetKeyTime( i )*0.001f, pScX->GetKeyValue( i ) );
                for (int i = 0; i < pScY->GetNKeys(); i++)  track.AddSYKey  ( pScY->GetKeyTime( i )*0.001f, pScY->GetKeyValue( i ) );
                for (int i = 0; i < pScZ->GetNKeys(); i++)  track.AddSZKey  ( pScZ->GetKeyTime( i )*0.001f, pScZ->GetKeyValue( i ) );
            }
            track.SetPose( pAnim->GetTransform( 0.0f ) );
            pAnm->AddTrack( track );
        }
        ++it;
    }
    return true;
} // ConvertAnimation

