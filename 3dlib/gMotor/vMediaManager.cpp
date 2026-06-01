/*****************************************************************************/
/*    File:    vMediaManager.cpp
/*    Desc:    Implementation of the model manager
/*    Author:  Ruslan Shestopalyuk
/*    Date:    10-15-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kTemplates.hpp"
#include "sg.h"
#include "sgNodePool.h"
#include "kHash.hpp"
#include "IMediaManager.h"
#include "sgDummy.h"
#include "sgModel.h"
#include "kTemplates.hpp"
#include "uiControl.h"
#include "IEffectManager.h"
#include "ITerrain.h"
#include "sgEffect.h"
#include "sgGeometry.h"
#include "sgAnimationBlock.h"
#include "vSkin.h"
#include "IResourceManager.h"
#include "kFilePath.h"
#include "kDirIterator.h"
#include "vModelInstance.h"
#include "sgConst.h"
#include "sgAnimation.h"

#include <map>

const int c_MdlInstPoolSize = 1024;
typedef StaticHash<     ModelInstance, 
                        EntityContext, 
                        c_DefTableSize, 
                        c_MdlInstPoolSize>    ModelInstanceHash;

/*****************************************************************************/
/*    Class:   ModelStub
/*    Desc:    
/*****************************************************************************/
struct ModelStub
{
    DWORD           m_ModelID;
    bool            m_bLoaded;
    std::string     m_ModelName;
    bool            m_bNoFile;

    ModelStub() : m_ModelID(0xFFFFFFFF), m_bLoaded(false), m_bNoFile(false){}
    ModelStub( DWORD id, const char* name ) 
        :   m_ModelID(id), 
            m_ModelName( name ), 
            m_bLoaded( false ),
            m_bNoFile( false ){}

    ModelStub( const char* name ) 
        :   m_ModelID(0xFFFFFFFF), 
            m_ModelName( name ), 
            m_bLoaded( false ),
            m_bNoFile( false ){}

    unsigned int	hash	() const
    {
        const char* s = m_ModelName.c_str();
	    unsigned int h = 0;
	    while (*s)
	    {
	    	h = (h * 729 + (*s) * 37) ^ (h >> 1);
	    	s++;
	    }
	    return h;
    }
    bool			equal	( const ModelStub& el )
    {
        return !stricmp( m_ModelName.c_str(), el.m_ModelName.c_str() );
    }
    void			copy	( const ModelStub& el )
    {
        m_ModelID   = el.m_ModelID;
        m_bLoaded   = el.m_bLoaded;
        m_ModelName = el.m_ModelName;
        m_bNoFile   = el.m_bNoFile;   
    }
}; // struct ModelStub

typedef Hash<ModelStub> ModelRegistry;
/*****************************************************************************/
/*    Class:    MediaManager
/*    Desc:    Implementaiton of the model manager interface
/*****************************************************************************/
class MediaManager :    public Group, 
                        public IMediaManager, 
                        public PSingleton<MediaManager>
{
    ModelInstanceHash   m_ModelInstances;
    ModelRegistry       m_Models;

    ModelInstance*      m_pCurInstance;
    SNode*              m_pCurModel;
    Matrix4D            m_TM;
    DWORD               m_AttachedEffectMask;
    bool                m_BoneViewMode;

public:
                        MediaManager        ();
    virtual DWORD       GetModelID          ( const char* fname );
	virtual void		ReloadModel			(DWORD idModel);
    virtual const char* GetModelFileName    ( DWORD modelID );
    virtual void        SetVisible          ( DWORD nodeID, bool bVisible = true );
	virtual void		GetModelBounds		(DWORD idModel, std::vector<AABoundBox> &Bounds, std::vector<Matrix4D> &Transforms);

    virtual DWORD       GetNodeID           ( DWORD modelID, const char* nodeName );
    virtual DWORD       GetNodeID           ( const char* name );
    virtual void        ShowNode            ( DWORD id, bool bShow = true );
    virtual Matrix4D    GetNodeTransform    ( DWORD nodeID, bool bLocalSpace = false );
    virtual void        SetNodeTransform    ( DWORD nodeID, const Matrix4D& m, bool bLocalSpace = false );
    virtual ILight*     GetLight            ( DWORD nodeID );
    virtual IGeometry*  GetGeometry         ( DWORD nodeID );
    virtual int         GetNumGeometries    ( DWORD modelID );
    virtual IGeometry*  GetGeometry         ( DWORD modelID, int idx, Matrix4D& tm );
    virtual int         GetNumSubNodes      ( DWORD modelID );
    virtual const char* GetSubNodeName      ( DWORD modelID, int idx );
    virtual DWORD       CloneNode           ( DWORD nodeID );
    virtual void        DeleteNode          ( DWORD nodeID );
    virtual void        OnFrame             ();
    virtual BaseMesh*   GetGeometry         ( int mdlID, int idx = 0 );
    virtual void        SaveMesh            ( const char* fileName , BaseMesh* bm );
    virtual void        SaveMeshes          ( const char* fileName , BaseMesh** bm , int numMeshes);
	virtual void		ExportToObj			( DWORD modelID,const char* Name );

    bool                MakeParent          ( DWORD parentID, DWORD childID, bool bParent );

    virtual void        RenderShadow        ( DWORD id, const Matrix4D* pTransform = NULL );
    virtual AABoundBox  GetBoundBox         ( DWORD id );

    virtual void        StartModel          ( DWORD modelID, const Matrix4D& tm = Matrix4D::identity, const DWORD context = NO_CONTEXT );
    virtual void        AnimateModel        ( DWORD animID, float animTime, float blendInTime = 0.5f, const Matrix4D& shiftTM = Matrix4D::identity );
    virtual void        DrawModel           ();
    virtual void        SetAttachedEffectMask(DWORD Mask);
    virtual DWORD       GetAttachedEffectMask();
    virtual void        SetBoneViewMode(bool State);
    virtual bool        GetBoneViewMode();

    virtual float       GetAnimTime         ( DWORD animID );
    virtual SNode*      GetModelRoot        ( DWORD modelID );

    virtual bool        GetHeight           ( DWORD id, Vector3D& pt, const Matrix4D* pTransform = NULL );
    virtual bool        GetLock             ( DWORD id, const Vector3D& pt, const Matrix4D* pTransform = NULL );
    virtual bool        SwitchTo            ( DWORD id, int index );
    virtual void        ReloadModels        ();
    virtual void        SetCamera           ( const Vector3D& lookAt, float viewVolWidth ){}
    virtual void        Init                ();
    virtual void        SetViewPort         ( float x, float y, float w, float h );
    virtual bool        IsBoxVisible        ( const Vector3D& minv, const Vector3D& maxv );
    virtual bool        IsSphereVisible     ( const Vector3D& center, float radius );
    virtual bool        IsPointVisible      ( const Vector3D& pt );

    virtual void        RegisterVisible     ( DWORD objID, DWORD modelID, const Matrix4D& tm );
    virtual void        RegisterVisible     ( DWORD objID, const Vector3D& minv, const Vector3D& maxv );
    virtual void        RegisterVisible     ( DWORD objID, const Vector3D& center, float radius );
    virtual void        RegisterVisible     ( DWORD objID, DWORD gpID, int frameID, const Matrix4D& tm );
    virtual void        ClearVisibleCache   ();
    virtual DWORD       PickVisible         ( float scrX, float scrY, DWORD prevID = 0xFFFFFFFF );
    virtual void        ScanHeightmap       ( DWORD nodeID, const Matrix4D& tm, float stepx, float stepy, SetHeightCallback callb );
    virtual void        ScanHeightmap       ( DWORD nodeID, const Matrix4D& tm, float stepx, float stepy, VisitHeightCallback callb );
    virtual bool        Intersects          ( float mX, float mY, DWORD modelID, const Matrix4D& tm, float& dist );

    virtual const char*     GetModelOfNodeName  ( DWORD NodeID );

    DECLARE_SCLASS(MediaManager,Group,MMGI);
private:

    DWORD               ModelToNodeID       ( DWORD id );
}; // class MediaManager

IMPLEMENT_CLASS( MediaManager );

//  global interface pointer
DIALOGS_API IMediaManager*        IMM       = MediaManager::instance();
DIALOGS_API IReflectionMap*       IRMap     = NULL;

bool  ConvertModel( SNode* pModel );
/*****************************************************************************/
/*    MediaManager implementation
/*****************************************************************************/
MediaManager::MediaManager()
{
    SetName( "Model Manager" );
    AddRef();

    m_pCurInstance = NULL;
    m_pCurModel = NULL;
    m_AttachedEffectMask = 1;
    m_BoneViewMode = false;
} // MediaManager::MediaManager

SNode* CompileModel( SNode* pRoot, const char* path );

//  shallow prefetching of the all used textures
void  PrefetchTextures( SNode* pRoot, const FilePath& path )
{
    SNode::Iterator it( pRoot );
    while (it)
    {
        SNode* pTex = (SNode*)*it;
        if (pTex->IsA<Texture>()) 
        {
            _chdir( path.GetDrive() );
            _chdir( path.GetDir() );    
            IRS->GetTextureID( pTex->GetName() );
        }
        else if (pTex->IsA<PRenderer>())
        {
            _chdir( path.GetDrive() );
            _chdir( path.GetDir() );
            PRenderer* pRen = (PRenderer*)pTex;
            pRen->SetTexName( pRen->GetTexName());
            pRen->SetTexName2( pRen->GetTexName2() );
        }
        ++it;
    }
} // PrefetchTextures

void ConvertModels( const char* root )
{
    DirIterator it( root );
    while (it)
    {
        if (it.IsDirectory()) ConvertModels( it.GetFullFilePath() );
        else 
        {
			if(strstr(it.GetFullFilePath(),"crash.c2m")){
				DWORD mID = IMM->GetModelID( it.GetFullFilePath() );
				//IMM->DeleteNode( mID );
				IMM->GetGeometry(mID);
			}
        }
        ++it;
    }
} // ConvertModels

DWORD MediaManager::GetModelID( const char* fname )
{
    if (!fname || fname[0] == 0) return c_BadID;
	static ModelStub ms;	
	ms.m_ModelName = fname;    
    DWORD id = m_Models.find( ms );
    if (id != NO_ELEMENT) return id;
    id = m_Models.add( ms );
    return id;
    /*
    FilePath path( fname );
    path.SetExt( "c2m" );
    try
    {
        //  look for resource in the resource manager
        int resID = IRM->FindResource( path.GetFullPath() );
        if (resID == -1) return -1;
        _chdir( IRM->GetHomeDirectory() );
        m_Models.push_back( ModelStub( 0xFFFFFFFF, fname ) );
        return m_Models.size() - 1;
    }
    catch(...)
    {
        _chdir( IRM->GetHomeDirectory() );
        return 0xFFFFFFFF; 
    }
    */
} // MediaManager::GetModelID

const char* MediaManager::GetModelFileName( DWORD modelID )
{
    if (modelID < 0 || modelID >= m_Models.numElem()) return "";
    return m_Models.elem( modelID ).m_ModelName.c_str();
} // MediaManager::GetModelFileName

void MediaManager::StartModel( DWORD modelID, const Matrix4D& tm, DWORD context )
{
    m_pCurInstance  = NULL;
    modelID         = ModelToNodeID( modelID );
    m_pCurModel     = NodePool::GetNode( modelID );
    m_TM            = tm;
    if (!m_pCurModel) return;

    if (context == NO_CONTEXT) return; 
    if (context != CUR_CONTEXT) PushEntityContext( context );
        
    int elID = m_ModelInstances.find( GetEntityContext() );
    if (elID == -1)
    {
        ModelInstance newInst( m_pCurModel, tm ); 
        if (m_ModelInstances.numElem() == m_ModelInstances.poolSize())
        {
            m_ModelInstances.reset();
        }
        elID = m_ModelInstances.add( GetEntityContext(), newInst );
    }   
    if (elID == -1) return;
    ModelInstance& inst = m_ModelInstances.elem( elID );
    if (inst.GetModel() != m_pCurModel) inst.Create( m_pCurModel, tm );
    m_pCurInstance = &inst;
    m_pCurInstance->SetRootTM( tm );
    if (context != CUR_CONTEXT) PopEntityContext();
} // MediaManager::StartModel

void MediaManager::AnimateModel( DWORD animID, float animTime, float blendInTime, const Matrix4D& shiftTM )
{
    if (!m_pCurModel) return; 
    Animation* pAnim = NodePool::GetNode<Animation>( ModelToNodeID( animID ) );
	if (!pAnim){
		if(m_pCurInstance)m_pCurInstance->Animate( NULL, m_TM, 0 );
		return;
	}
    pAnim->BindNode( m_pCurModel );
    pAnim->SetCurrentTime( animTime + pAnim->GetStartTime() );
    Animation::PushTime( animTime + pAnim->GetStartTime() );
    if (m_pCurInstance)
    {
        m_pCurInstance->Animate( pAnim, m_TM, blendInTime );
    }
    else
    {
        pAnim->Render();
    }
    Animation::PopTime();
} // MediaManager::AnimateModel

void MediaManager::DrawModel()
{
    if (!m_pCurModel) return;
    
    //try{
        if (m_pCurModel->IsA<PEmitter>())
        {
            TransformNode::Push( m_TM );
            int inst = IEffMgr->InstanceEffect( m_pCurModel->GetID(),m_TM );
            IEffMgr->UpdateInstance( inst, m_TM );
            TransformNode::Pop();
            return;
        }
        else if (m_pCurInstance)
        {
            m_pCurInstance->Render();
        }
        else
        {
            TransformNode::SetCalcWorldTM( true );  
            TransformNode::ResetTMStack( &m_TM );
            SNode::SetRenderTMOnly( true );
            m_pCurModel->Render();
            SNode::SetRenderTMOnly( false );
            m_pCurModel->Render();
        }
        IRS->Flush();
    //}
    //catch(...)
    //{
    //    Log.Error( "Exception occured when trying to render model." );
    //}
} // MediaManager::DrawModel

const float c_ShadowZBias = 64.0f;
void MediaManager::RenderShadow( DWORD id, const Matrix4D* pTransform )
{
    SNode* pModel = NodePool::GetNode( ModelToNodeID( id ) );
    if (!pModel) return;
    
    if (pTransform) TransformNode::ResetTMStack( pTransform );
    
    if (pTransform)
    {
        TransformNode::ResetTMStack();
        IRS->ResetWorldTM();
    }

} // MediaManager::RenderShadow
void MediaManager::SetAttachedEffectMask(DWORD Mask){
    m_AttachedEffectMask = Mask;
}
DWORD MediaManager::GetAttachedEffectMask(){
    return m_AttachedEffectMask;
}
void MediaManager::SetBoneViewMode(bool State){
    m_BoneViewMode = State;
}
bool MediaManager::GetBoneViewMode(){
    return m_BoneViewMode;
}
//-------------------------------------------------------------------------------
//  Func:  MediaManager::GetHeight
//  Desc:  Returns height at the given (pt.x, pt.y) point (result is placed in pt.z)
//  Ret:   true if there is point of mesh at (pt.x, pt.y)
//  Rmrk:  Do not call it too often, if you don't want you program to 
//            execute forever
//-------------------------------------------------------------------------------
bool MediaManager::GetHeight( DWORD id, Vector3D& pt, const Matrix4D* pTransform )
{
    SNode* pModel = NodePool::GetNode( ModelToNodeID( id ) );
    if (!pModel) return false;
        
    pt.z = 0.0f;
    Vector3D dir;
    dir.set( 0.0f, -0.5f, c_CosPId6 );

    static const float c_Away = 10000.0f;
    Vector3D pos( pt.x, pt.y, 0.0f );

    pos.addWeighted( dir, c_Away );
    dir.reverse();

    Line3D ray( pos, dir );
    int            minIdx        = -1;
    Geometry*    pMinGeom    = pModel->FindChild<Geometry>( "Navimesh" );
    if (!pMinGeom) pMinGeom = pModel->FindChild<Geometry>( "Navimesh_geom" );
    float        minDist        = 0.0f;

    if (!pMinGeom) return false;
    {
        Ray3D tray( ray );
        Matrix4D lToW( *pTransform );
        lToW.mulLeft( GetWorldTM( pMinGeom ) );
        Matrix4D wToL; wToL.inverse( lToW );
        tray.Transform( wToL );
        int triIdx;
        float dist = pMinGeom->GetMesh().PickPoly( tray, triIdx );
        if (dist < 0.0f || triIdx < 0) return false;
        Vector4D hit( tray.getDir() );
        hit *= dist;
        hit.w = 0.0f;
        hit *= lToW;
        minDist    = hit.norm();
    }
    if (pMinGeom)
    {
        pt.z = c_Away - minDist;
        return true;
    }

    return false;
} // MediaManager::GetHeight

bool MediaManager::GetLock( DWORD id, const Vector3D& pt, const Matrix4D* pTransform )
{
    SNode* pModel = NodePool::GetNode( ModelToNodeID( id ) );
    if (!pModel) return false;

    Vector3D ptLoc( pt );
    ptLoc.z = 0.0f;

    //  transform query point to the local model space
    if (pTransform)
    {
        Matrix4D invT;
        invT.inverse( *pTransform );
        ptLoc *= invT;
    }

    const float c_BigFloat = 10000.0f;
    Vector3D dir( 0.0f, 0.5f, -c_CosPId6 );
    Vector3D pos( ptLoc.x, ptLoc.y, 0.0f );
    dir.reverse();

    Line3D ray( pos, dir );

    int                minIdx        = -1;
    Geometry*    pMinGeom    = pModel->FindChild<Geometry>( "Lockmesh" );
    float            minDist        = c_BigFloat;

    if (!pMinGeom) return false;

    minDist = pMinGeom->GetMesh().PickPoly( ray, minIdx );
    if (minIdx == -1) return false;
    return true;
} // MediaManager::GetLock

bool MediaManager::SwitchTo( DWORD id, int index )
{
    Switch* pSwitch = NodePool::GetNode<Switch>( ModelToNodeID( id ) );
    if (!pSwitch) return false;
    pSwitch->SwitchTo( index );
    return true;
} // MediaManager::SetCursor

float MediaManager::GetAnimTime( DWORD animID )
{
    Animation* pAnim = NodePool::GetNode<Animation>( ModelToNodeID( animID ) );
    if (!pAnim) return 0.0f;
    return pAnim->GetAnimationTime();
} // MediaManager::GetAnimTime

DWORD MediaManager::GetNodeID( DWORD modelID, const char* nodeName )
{
    SNode* pModel = NodePool::GetNode( ModelToNodeID( modelID ) );
    if (!pModel) return c_BadID;
    SNode* pChild = pModel->FindChildByName( nodeName );
    if (!pChild) return c_BadID;
    return pChild->GetID();
} // MediaManager::GetNodeID

DWORD MediaManager::GetNodeID( const char* name )
{
    SNode* pNode = NodePool::GetNodeByName( name );
    if (!pNode) return c_BadID;
    return pNode->GetID();
}

void MediaManager::ShowNode( DWORD id, bool bShow )
{
    SNode* pNode = NodePool::GetNode( id );
    if (pNode) pNode->SetInvisible( !bShow );
} // MediaManager::ShowNode

Matrix4D MediaManager::GetNodeTransform( DWORD nodeID, bool bLocalSpace )
{
    if (nodeID == c_BadID) return Matrix4D::identity;
    if (m_pCurInstance)
    {
        return m_pCurInstance->GetNodeTransform( nodeID );
    }
    TransformNode* pNode = NodePool::GetNode<TransformNode>( nodeID );
    if (!pNode) return Matrix4D::identity;
    if (bLocalSpace) return pNode->GetTransform(); else return pNode->GetWorldTM();
} // MediaManager::GetNodeTransform

void MediaManager::SetNodeTransform( DWORD nodeID, const Matrix4D& m, 
                                            bool bLocalSpace )
{
    TransformNode* pNode = NodePool::GetNode<TransformNode>( nodeID );
    if (!pNode) return;
    if (bLocalSpace) pNode->SetTransform( m ); 
    else pNode->SetWorldTM( m );
    if (m_pCurInstance) 
    {
        m_pCurInstance->OnUpdateNode( pNode );
    }
} // MediaManager::SetNodeTransform

void MediaManager::SetVisible( DWORD nodeID, bool bVisible )
{
    SNode* pNode = NodePool::GetNode( nodeID );
    if (pNode)
    {
        pNode->SetInvisible( !bVisible );
    }
} // MediaManager::SetVisible

bool MediaManager::MakeParent( DWORD parentID, DWORD childID, bool bParent )
{
    SNode* pParent = NodePool::GetNode( parentID );
    SNode* pChild  = NodePool::GetNode( childID );
    
    if (!pParent || !pChild) return false;
    
    if (bParent)
    {
        pParent->RemoveChild( pChild );
        pParent->AddInput( pChild );
    }
    else
    {
        pParent->RemoveChild( pChild );
    }
    return true;
} // MediaManager::MakeParent

void MediaManager::ReloadModels()
{
    //  nothin here right now...
} // MediaManager::ReloadModels

ILight*    MediaManager::GetLight( DWORD nodeID )
{
    LightSource* pLight = NodePool::GetNode<LightSource>( nodeID );
    return pLight;
}

IGeometry* MediaManager::GetGeometry( DWORD nodeID )
{
    SNode* pG = NodePool::GetNode( ModelToNodeID( nodeID ) );
    if (!pG->IsA<Geometry>())
    {
        SNode::Iterator it( pG );
        while (it)
        {
            if (((SNode*)(*it))->IsA<Geometry>()) return (Geometry*)(*it);
            ++it;
        }
    }
    return (Geometry*)pG;
} // MediaManager::GetGeometry

void MediaManager::Init()
{
    if (Root::instance()->GetNChildren() == 0) Root::instance()->CreateGuts();
}

bool MediaManager::IsBoxVisible( const Vector3D& minv, const Vector3D& maxv ) 
{
    ICamera* pCam = GetCamera();
    if (!pCam) return false;
    return pCam->GetFrustum().Overlap( AABoundBox( minv, maxv ) );
} // MediaManager::IsBoxVisible

bool MediaManager::IsSphereVisible( const Vector3D& center, float radius ) 
{
    ICamera* pCam = GetCamera();
    if (!pCam) return false;
    return pCam->GetFrustum().Overlap( Sphere( center, radius ) );
} // MediaManager::IsSphereVisible

bool MediaManager::IsPointVisible( const Vector3D& pt ) 
{
    ICamera* pCam = GetCamera();
    if (!pCam) return false;
    return pCam->GetFrustum().PtIn( pt );
}

void MediaManager::RegisterVisible( DWORD objID, DWORD modelID, const Matrix4D& tm )
{
}

void MediaManager::RegisterVisible( DWORD objID, const Vector3D& minv, const Vector3D& maxv )
{
}

void MediaManager::RegisterVisible( DWORD objID, const Vector3D& center, float radius )
{
}

void MediaManager::RegisterVisible( DWORD objID, DWORD gpID, int frameID, const Matrix4D& tm )
{
}

void MediaManager::ClearVisibleCache()
{
}

DWORD MediaManager::PickVisible( float scrX, float scrY, DWORD prevID )
{
    return 0xFFFFFFFF;
}

void MediaManager::ScanHeightmap( DWORD nodeID, const Matrix4D& tm, 
                                        float stepx, float stepy,
                                        SetHeightCallback callb )
{
    SNode* pG = NodePool::GetNode( nodeID );
    if (!pG->IsA<Geometry>()) return;

    Vector3D dir = Vector3D::oZ;
    Primitive& pri = ((Geometry*)pG)->GetMesh();

    Matrix4D mdlTM = GetWorldTM( pG );
    mdlTM *= tm; 
    ::ScanHeightmap( pri, mdlTM, stepx, stepy, callb );
} // MediaManager::ScanHeightmap

void MediaManager::ScanHeightmap( DWORD nodeID, const Matrix4D& tm, 
                                      float stepx, float stepy,
                                      VisitHeightCallback callb )
{
    SNode* pG = NodePool::GetNode( nodeID );
    if (!pG->IsA<Geometry>()) return;

    Vector3D dir = Vector3D::oZ;
    Primitive& pri = ((Geometry*)pG)->GetMesh();

    Matrix4D mdlTM = GetWorldTM( pG );
    mdlTM *= tm; 
    ::ScanHeightmap( pri, mdlTM, stepx, stepy, callb );
} // MediaManager::ScanHeightmap

void MediaManager::SetViewPort( float x, float y, float w, float h )
{
} // MediaManager::SetViewPort

int    MediaManager::GetNumGeometries( DWORD modelID ) 
{
    SNode* pRoot = NodePool::GetNode( modelID );
    int nG = 0;
    Iterator it( pRoot, Geometry::FnFilter );
    while (it) { nG++; ++it; }
    return nG;
} // MediaManager::GetNumGeometries

IGeometry* MediaManager::GetGeometry( DWORD modelID, int idx, Matrix4D& tm )
{
    SNode* pRoot = NodePool::GetNode( modelID );
    int nG = 0;
    Iterator it( pRoot, Geometry::FnFilter );
    while (it) 
    { 
        if (nG == idx) 
        {
            Geometry* pGeom = (Geometry*)*it;
            tm = GetWorldTM( pGeom );
            return (IGeometry*)pGeom;
        }
        nG++; 
        ++it; 
    }
    return NULL;
} // MediaManager::GetGeometry

DWORD MediaManager::CloneNode( DWORD nodeID )
{
    SNode* pRoot = NodePool::GetNode( nodeID );
    if (!pRoot) return c_BadID;
    SNode* pNewNode = pRoot->CloneSubtree();
    return pNewNode->GetID();
}

void MediaManager::DeleteNode( DWORD nodeID )
{
    SNode* pNode = NodePool::GetNode( nodeID );
    if (!pNode) return;
    NodePool::DestroyNode( pNode );
    
} // MediaManager::DeleteNode

int    MediaManager::GetNumSubNodes( DWORD modelID )
{
    SNode* pRoot = NodePool::GetNode( ModelToNodeID(modelID) );
    int nG = 0;
    Iterator it( pRoot, TransformNode::FnFilter );
    while (it) { nG++; ++it; }
    return nG;
} // MediaManager::GetNumSubNodes

const char*    MediaManager::GetSubNodeName( DWORD modelID, int idx )
{
    SNode* pRoot = NodePool::GetNode( ModelToNodeID(modelID) );
    int nG = 0;
    Iterator it( pRoot, TransformNode::FnFilter );
    while (it) 
    { 
        if (nG == idx) 
        {
            SNode* pNode = (SNode*)*it;
            return pNode->GetName();
        }
        nG++; 
        ++it; 
    }
    return "";
} // MediaManager::GetSubNodeName

void MediaManager::OnFrame()
{
    Animation::SetupTimeDelta();
}

AABoundBox MediaManager::GetBoundBox( DWORD id )
{
    SNode* pNode = NodePool::GetNode( ModelToNodeID( id ) );
    if (!pNode) return AABoundBox::null;
    if (pNode->IsA<Model>())
    {
        return ((Model*)pNode)->GetAABB(); 
    }
    //  not sure about this...
    return AABoundBox::null;
} // MediaManager::GetBoundBox

bool MediaManager::Intersects( float mX, float mY, DWORD modelID, const Matrix4D& tm, float& dist )
{
    SNode* pNode = NodePool::GetNode( ModelToNodeID( modelID ) );
    if (!pNode) return false;
    
    ICamera* pCam = ::GetCamera();
    Ray3D ray = pCam->GetPickRay( mX, mY );
    
    Matrix4D itm;
    itm.inverse( tm );
	Ray3D rayOrig( ray );
    ray.Transform( itm );

    AABoundBox aabb = GetBoundBox( modelID );
    if (!aabb.Overlap( ray )) return false;

    Model* pModel = dynamic_cast<Model*>( pNode );
    if (pModel)
    {
        int nS = pModel->GetShellSize();
        for (int i = 0; i < nS; i++)
        {
			const AABoundBox& shell = pModel->GetShellElem( i );
			if (shell.Overlap( ray ))
			{ 
				AABoundBox box = shell;
				box.Transform( tm );
				dist = rayOrig.getOrig().distance( box.GetCenter() );
				return true;
			}
        }
        if (nS > 0) return false;
    }

	aabb.Transform( tm );
    dist = rayOrig.getOrig().distance( aabb.GetCenter() );
    return true;
} // MediaManager::Intersects

//--------------------------------------------------------------------------------------------------------------------
// MediaManager::GetModelBounds
//--------------------------------------------------------------------------------------------------------------------
void MediaManager::GetModelBounds(DWORD idModel, std::vector<AABoundBox> &Bounds, std::vector<Matrix4D> &Transforms) {
	Bounds.clear();
	Transforms.clear();

	SNode *pModel = IMM->GetModelRoot(idModel);
	if(!pModel) {
		return;
	}
	SNode::Iterator It(pModel);
	while(It) {
		SNode *pNode = (SNode *)*It;
		if(pNode->IsA<Geometry>()) {
			Geometry *pGeom = (Geometry *)pNode;
			const BaseMesh &M = pGeom->GetMesh();
			const int NVerts = M.getNVert();
			if(NVerts > 0) {
				Matrix4D T = GetWorldTM(pGeom);
				Transforms.push_back(T);
				AABoundBox bb = pGeom->GetAABB();
				Bounds.push_back(bb);
			}
		}
        It.operator++();
	}
} // MediaManager::GetModelBounds

//-----------------------------------------------------------------------------
// MediaManager::ReloadModel
//-----------------------------------------------------------------------------
void MediaManager::ReloadModel(DWORD idModel) {
	SNode *g = NodePool::GetNode(ModelToNodeID(idModel));
	if(!g) {
		return;
	}
	if(g->IsA<Model>()) {
		((Model *)g)->SetFileName(GetModelFileName(idModel));
		((Model *)g)->Reload();
		m_ModelInstances.reset();
		for(int i = 0; i < NodePool::instance().Count(); i++) {
			SNode *pNode = NodePool::instance().ToPtr()[i];
			if(!pNode) {
				continue;
			}
			if(pNode->IsA<Animation>()) {
				Animation *pAnim = (Animation *)pNode;
				if(!pAnim) {
					continue;
				}
				pAnim->SetOperated(NULL);
			}
		}
	}
} // MediaManager::ReloadModel

BaseMesh* MediaManager::GetGeometry( int mdlID, int idx )
{
    SNode* pG = NodePool::GetNode( ModelToNodeID( mdlID ) );
    if (!pG) return NULL;
    if (pG->IsA<Model>()) ((Model*)pG)->Load();
    int cGeom = 0;
    SNode::Iterator it( pG );
    while (it)
    {
        Geometry* pGeom = (Geometry*)(*it);
		if (pGeom->IsA<Geometry>())
		{
			if (cGeom == idx) return &(pGeom->GetMesh());
			cGeom++;
		}
        ++it;
    }
    return NULL;    
} // MediaManager::GetGeometry

SNode* MediaManager::GetModelRoot( DWORD modelID )
{
    DWORD nodeID = ModelToNodeID( modelID );
    return NodePool::GetNode( nodeID );
} // MediaManager::GetModelRoot

DWORD MediaManager::ModelToNodeID( DWORD id )
{
    if (id >= m_Models.numElem()) return id;//c_BadID;
    ModelStub& s = m_Models.elem( id );
    if (s.m_bNoFile) return c_BadID;
    if (!s.m_bLoaded)
    {
        FilePath resPath( s.m_ModelName.c_str() );
        resPath.SetExt( "c2m" );
        int resID = IRM->FindResource( resPath.GetFullPath() );
        if (resID == -1) 
        {
            s.m_bNoFile = true;
            Log.Error( "Could not load model <%s>", s.m_ModelName.c_str() );
            return c_BadID;
        }
        InStream& is = IRM->LockResource( resPath.GetFullPath() );

        FilePath path( IRM->GetFullPath( resID ) );
        _chdir( path.GetDrive() );
        _chdir( path.GetDir() );

        SNode* pRoot = SNode::UnserializeSubtree( is );            
        is.Close();

        if (!pRoot) 
        {
            _chdir( IRM->GetHomeDirectory() );
            s.m_bLoaded = true;
            s.m_bNoFile = true;
            return c_BadID;
        }

        PrefetchTextures( pRoot, path );

        pRoot = CompileModel( (SNode*)pRoot, path.GetFullPath() );
		//ConvertNodesToSkinGPU(pRoot);

        s.m_bLoaded = true;
        s.m_ModelID = pRoot->GetID();

        Model* pModel = dynamic_cast<Model*>( pRoot );
        if (pModel) 
        {
            pModel->CreateShell();
            AABoundBox aabb = CalculateAABB( pModel );
            pModel->SetAABB( aabb );
        }
        _chdir( IRM->GetHomeDirectory() );
        return s.m_ModelID;
    }
    return s.m_ModelID;
} // MediaManager::ModelToNodeID

#include "sgAnimationBlock.h"
bool ConvertModel( SNode* pModel )
{
    if (pModel->IsA<AnimationBlock>()) return false;
    std::vector<SNode*>  gNodes;
    SNode::Iterator it( pModel, Skin::FnFilter );
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        SNode* pParent = pNode->GetParent();
        while (pParent != pModel)
        {
            pNode = pParent;
            pParent = pParent->GetParent();
        }
        gNodes.push_back( pNode );
        ++it;
    }

    for (int i = 0; i < gNodes.size(); i++)
    {
        SNode* pNode = gNodes[i];
        pNode->AddRef();
        pModel->RemoveChild( pNode );
        pModel->AddChild( pNode );
        pNode->Release();
    }

    SNode::Iterator git( pModel, Skin::FnFilter );
    while (git)
    {
        Skin* pGeom = (Skin*)*git;
        BaseMesh* mesh = &pGeom->GetMesh();
        VertexFormat vf = mesh->getVertexFormat();
        switch (vf)
        {
        case vfVertexW1: 
            ConvertVF<Vertex1W, VertexW1>( *mesh );
            break;
        case vfVertexW2: 
            ConvertVF<Vertex2W, VertexW2>( *mesh );
            break;
        case vfVertexW3: 
            ConvertVF<Vertex3W, VertexW3>( *mesh );
            break;
        case vfVertexW4:
            ConvertVF<Vertex4W, VertexW4>( *mesh );
            break;
        }
        ++git;
    }

    return gNodes.size() != 0;
} // ConvertModel


bool LoadRawModel( const char* fname, std::vector<Vector3D>& vert, std::vector<int>& idx )
{
    DWORD mdlID = IMM->GetModelID( fname );
    if (mdlID == 0xFFFFFFFF) return false;
    SNode* pMdl = NodePool::GetNode( mdlID );
    if (!pMdl) return false;
    SNode::Iterator it( pMdl, Geometry::FnFilter );
    int cV = 0;
    while (it)
    {
        Geometry* pGeom = (Geometry*)*it;
        BaseMesh& bm = pGeom->GetMesh();
        Matrix4D tm = GetWorldTM( pGeom );
        VertexIterator vit;
        vit << bm;
        while (vit)
        {
            Vector3D v = vit.pos();
            tm.transformPt( v );
            vert.push_back( v );
            ++vit;
        }
        int nInd = bm.getNInd();
        WORD* widx = bm.getIndices();
        for (int i = 0; i < nInd; i++)
        {
            idx.push_back( widx[i] + cV );
        }
        cV += bm.getNVert();
        ++it;
    }
    return true;
} // LoadRawModel

void SaveToVRML( DWORD mID, const char* path )
{
    FILE* fp = fopen( path, "wt" );
    if (!fp) return;

    fprintf( fp, "#VRML V2.0 utf8\n" );

    SNode* pModel = IMM->GetModelRoot( mID );
    SNode::Iterator it( pModel );
    const char* texName = "";
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        if (pNode->IsA<Texture>()) texName = pNode->GetName();
        if (pNode->IsA<Geometry>())
        {
            Geometry* pGeom = (Geometry*)pNode;
            Matrix4D tm = GetWorldTM( pGeom );

            fprintf( fp, "DEF %s Transform {\n", pGeom->GetName() );
            fprintf( fp, "\ttranslation %f%f%f\n", tm.e30, tm.e31, tm.e32 );
            fprintf( fp, "\trotation %f%f%f%f\n", 0.0f, 0.0f, 0.0f, 0.0f );
            fprintf( fp, "\tchildren [\n" );
            fprintf( fp, "\t\tShape {\n" );

            //  material
            fprintf( fp, "\t\t\tappearance Appearance {\n" );
                fprintf( fp, "\t\t\t\ttexture ImageTexture {\n" );
                fprintf( fp, "\t\t\t\t\turl \x22%s\x22\n", texName );
                fprintf( fp, "\t\t\t\t}\n" );
            fprintf( fp, "\t\t\t}\n" );

            BaseMesh& bm = pGeom->GetMesh();
            int nV = bm.getNVert();
            VertexIterator vit( bm.getVertexData(), nV, CreateVertexDeclaration( bm.getVertexFormat() ) );

            fprintf( fp, "\t\t\tgeometry DEF %s-FACES IndexedFaceSet {\n\t\t\t\tccw FALSE\n\t\t\t\tsolid TRUE\n", 
                            pGeom->GetName() );
                fprintf( fp, "\t\t\t\tcoord DEF %s-COORD Coordinate { point [", pGeom->GetName() );
                for (int i = 0; i < nV; i++)
                {
                    if (i%3 == 0) fprintf( fp, "\n\t\t\t\t\t" );
                    Vector3D pos = vit.pos( i );
                    fprintf( fp, "%f %f %f, ", pos.x, pos.z, pos.y );
                }
                fprintf( fp, "\t\t\t\t]}\n" );
                fprintf( fp, "\t\t\t\ttexCoord DEF %s-TEXCOORD TextureCoordinate { point [", pGeom->GetName() );
                for (int i = 0; i < nV; i++)
                {
                    if (i%4 == 0) fprintf( fp, "\n\t\t\t\t\t" );
                    float u = vit.uv( i, 0 ).u;
                    float v = vit.uv( i, 0 ).v;
                    //u = u*2.0f - 1.0f;
                    //v = v*2.0f - 1.0f;
					v = 1 - v;
                    fprintf( fp, "%f %f, ", u, v );
                }
                fprintf( fp, "\t\t\t\t]}\n" );

                int nP = bm.getNPri();
                fprintf( fp, "\t\t\t\tcoordIndex [%s", pGeom->GetName() );
                int cI = 0;
                WORD* pIdx = bm.getIndices();
                for (int i = 0; i < nP; i++)
                {
                    if (i%16 == 0) fprintf( fp, "\n\t\t\t\t\t" );
                    fprintf( fp, "%d, %d, %d, -1", pIdx[cI], pIdx[cI + 1], pIdx[cI + 2] );
                    if (i < nP - 1) fprintf( fp, "," );
                    cI += 3;
                }
                fprintf( fp, "\t\t\t\t]\n" );

                fprintf( fp, "\t\t\t\ttexCoordIndex [%s", pGeom->GetName() );
                cI = 0;
                for (int i = 0; i < nP; i++)
                {
                    if (i%16 == 0) fprintf( fp, "\n\t\t\t\t\t" );
                    fprintf( fp, "%d, %d, %d, -1", pIdx[cI], pIdx[cI + 1], pIdx[cI + 2] );
                    if (i < nP - 1) fprintf( fp, "," );
                    cI += 3;
                }
                fprintf( fp, "\t\t\t\t]\n" );

            fprintf( fp, "\t\t\t}\n" );


            

            fprintf( fp, "\t\t}\n" );
            fprintf( fp, "\t]\n" );
            fprintf( fp, "}\n" );
        }
        ++it;
    }

    fclose( fp );
} // SaveToVRML
const char* MediaManager::GetModelOfNodeName  ( DWORD NodeID ){
    SNode* node=NodePool::GetNode(NodeID);
    while(node && node->GetParent())node=node->GetParent();
    if(node){
        DWORD nID = node->GetID();
        int n = m_Models.numElem();
        for(int i=0;i<n;i++)if(m_Models.elem(i).m_ModelID==nID){
            return m_Models.elem(i).m_ModelName.c_str();
        }
    }
    return "???";
}
void MediaManager::SaveMesh( const char* fileName , BaseMesh* bm ){
    FOutStream model_stream(fileName);
    Group* G=new Group();
    std::string str = IRS->GetShaderName(bm->getShader());
    IRM->ConvertPathToRelativeForm( str );
    G->AddChild<Shader>( str.c_str() );
    str = IRS->GetTextureName(bm->getTexture());
    IRM->ConvertPathToRelativeForm( str );
    G->AddChild<Texture>( str.c_str() );
    Geometry* GM=G->AddChild<Geometry>("geom");
    BaseMesh& WBM=GM->GetMesh();
    WBM=*bm;
    G->SerializeSubtree(model_stream);
    model_stream.CloseFile();    
    delete(G);
}
void MediaManager::SaveMeshes( const char* fileName , BaseMesh** bms , int numMeshes){
    FOutStream model_stream(fileName);
    Group* GR=new Group();
	SetColorConst* C=GR->AddChild<SetColorConst>("ColorConst");
	C->SetColor(IRS->GetColorConst());
	//SetTextureFactor* T=GR->AddChild<SetTextureFactor>("TextureFactor");
	//C->SetColor(IRS->GetTextureFactor());
    for(int i=0;i<numMeshes;i++){
        char s[12];
        sprintf(s,"node%d",i+1);
        Group* G=GR->AddChild<Group>(s);
        BaseMesh* bm = bms[i];
        std::string str = IRS->GetShaderName(bm->getShader());
        IRM->ConvertPathToRelativeForm( str );
        G->AddChild<Shader>( str.c_str() );
        str = IRS->GetTextureName(bm->getTexture());
        IRM->ConvertPathToRelativeForm( str );
        G->AddChild<Texture>( str.c_str() );
        Geometry* GM=G->AddChild<Geometry>("geom");
        BaseMesh& WBM=GM->GetMesh();
        WBM=*bm;
    }
    GR->SerializeSubtree(model_stream);
    model_stream.CloseFile();
    delete(GR);
}
#define __STDAPPLICATION__
#include "..\..\ClassEngine\dynarray.h"
#include "..\..\COSSACKS2\unichash.h"

struct pos_vertex{
	Vector3D pos;
	DWORD    obj;
	
	operator DWORD(){
		return obj+(*(DWORD*)&pos.x)+(*(DWORD*)&pos.y)+(*(DWORD*)&pos.z);
	}	
	bool   operator == (const pos_vertex& c) const{
		return obj==c.obj && pos.x==c.pos.x && pos.y==c.pos.y && pos.z==c.pos.z;
	}
};
struct uv_vertex{
	DWORD		ObjIndex;
	DWORD		PosIndex;
	Vector3D	N;
	float		u;
	float		v;
};
struct obj_triang{
	DWORD p1;
	DWORD p2;
	DWORD p3;
	DWORD uv1;
	DWORD uv2;
	DWORD uv3;
	DWORD obj;
};
Vector3D toV3D( tri_DWORD td ){
	return Vector3D(*((float*)(&td.V1)),*((float*)(&td.V2)),*((float*)(&td.V2)));	
}
tri_DWORD toTDW( Vector3D v ){
	return tri_DWORD(*((DWORD*)(&v.x)),*((DWORD*)(&v.y)),*((DWORD*)(&v.z)));
}
void MediaManager::ExportToObj( DWORD modelID,const char* Name ){
	FILE* fp = fopen( Name, "wt" );
	if (!fp) return;

	uni_hash<DWORD,pos_vertex>	PosVerts;
	std::vector<uv_vertex>		uvs;
	std::vector<SNode*>			Objs;
	std::vector<obj_triang>		Tris;

	fprintf( fp, "#exported from c2m\n" );

    SNode* pModel = IMM->GetModelRoot( modelID );
	SNode::Iterator it( pModel );
	const char* texName = "";
	int pidx=1;
	int uvidx=1;	
	int objidx=0;
	int cI=0;
	int obj0 = 1;
	while (it)
	{
		SNode* pNode = (SNode*)*it;
		if (pNode->IsA<Texture>()) texName = pNode->GetName();
		if (pNode->IsA<Geometry>())
		{
			Geometry* pGeom = (Geometry*)pNode;
			Matrix4D tm = GetWorldTM( pGeom );
			Objs.push_back( pNode );			

			BaseMesh& bm = pGeom->GetMesh();
			int nV = bm.getNVert();
			VertexIterator vit( bm.getVertexData(), nV, CreateVertexDeclaration( bm.getVertexFormat() ) );
			for (int i = 0; i < nV; i++)
			{				
				Vector3D pos = vit.pos( i );
				Vector3D nrm = vit.normal( i );
				TexCoord t = vit.uv( i,0 );
				tm.transformPt(pos);
				pos_vertex pv;
				pv.obj=objidx;
				pv.pos=pos;	
				DWORD ppos = PosVerts.get_i( pv );
				if(ppos==0xFFFFFFFF){
					ppos = PosVerts.add_i( pv,_DWORD(pidx++) );
				}
				uv_vertex uv;
				uv.N = nrm;
				uv.PosIndex = ppos;
				uv.u = t.u;
				uv.v = t.v;
				uv.ObjIndex = objidx;
				uvs.push_back( uv );
			}
			int nP = bm.getNPri();			
			WORD* pIdx = bm.getIndices();
			cI = 0;
			for (int i = 0; i < nP; i++)
			{
				obj_triang ot;
				ot.uv1 = obj0 + pIdx[ cI   ];
				ot.uv2 = obj0 + pIdx[ cI+1 ];
				ot.uv3 = obj0 + pIdx[ cI+2 ];
				ot.obj = objidx;
                Tris.push_back( ot );
				cI += 3;
			}			
			obj0 = uvs.size()+1;
			objidx++;
		}		
		it.operator++();
	}
	int sz = Tris.size();
	int prevobj=-1;
	for(int i=0;i<sz;i++)
	{
		obj_triang* ot=&Tris[i];
		if(ot->obj!=prevobj)
		{
			prevobj=ot->obj;
			//fprintf( fp, "\no %s\n\n", Objs[prevobj]->GetName() );
			int np=PosVerts.size();
			for(int j=0;j<np;j++){
                pos_vertex* p=PosVerts.key(j);
				if(p->obj==prevobj){
					fprintf( fp, "v %f %f %f\n", p->pos.y, p->pos.z, -p->pos.x );
				}
			}
			np=uvs.size();
			for(int j=0;j<np;j++){
				uv_vertex* uv=&uvs[j];
				if(uv->ObjIndex == prevobj){
					fprintf( fp, "vn %f %f %f\nvt %f %f\n",uv->N.y,uv->N.z,-uv->N.x,uv->u,1-uv->v);
				}
			}
		}
		uv_vertex* uv1=&uvs[ot->uv1-1];
		uv_vertex* uv2=&uvs[ot->uv2-1];
		uv_vertex* uv3=&uvs[ot->uv3-1];
		fprintf( fp, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", uv1->PosIndex+1, ot->uv1, ot->uv1, uv3->PosIndex+1, ot->uv3, ot->uv3, uv2->PosIndex+1, ot->uv2, ot->uv2 );
	}
	fclose( fp );
}















