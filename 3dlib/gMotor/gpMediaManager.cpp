/*****************************************************************************/
/*	File:	gpMediaManager.cpp
/*	Desc:	Realization of the model manager
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-15-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "kHash.hpp"
#include "IMediaManager.h"
#include "sgDummy.h"
#include "sgShadow.h"
#include "kTemplates.hpp"
#include "uiControl.h"
#include "IEffectManager.h"
#include "ITerrain.h"
#include "sgEffect.h"
#include "sgGeometry.h"
#include "IResourceManager.h"
#include "kFilePath.h"
#include <map>

/*****************************************************************************/
/*	Class:	InstancedModel
/*****************************************************************************/
class InstancedModel
{
    std::vector<Matrix4D>       m_BoneTM;
    DWORD                       m_CurAnimID;
    float                       m_CurAnimTime;
    
    friend class MediaManager;
public:

}; // class InstancedModel

const int c_MdlInstPoolSize = 512;
typedef StaticHash<	InstancedModel, 
                    EntityContext, 
                    c_DefTableSize, 
                    c_MdlInstPoolSize>	ModelInstanceHash;

/*****************************************************************************/
/*	Class:	MediaManager
/*	Desc:	Implementaiton of the model manager interface
/*****************************************************************************/
class MediaManager :	public Group, 
						public IMediaManager, 
						public PSingleton<MediaManager>
{
    ModelInstanceHash   m_ModelInstances;

public:
						MediaManager();
	virtual DWORD		GetModelID		( const char* fname );
	virtual const char* GetModelFileName( DWORD modelID );
	virtual void		SetVisible		( DWORD nodeID, bool bVisible = true );

	virtual DWORD		GetNodeID		( DWORD modelID, const char* nodeName );
	virtual DWORD		GetNodeID		( const char* name );
	virtual void		ShowNode		( DWORD id, bool bShow = true );
	virtual Matrix4D	GetNodeTransform( DWORD nodeID, bool bLocalSpace = false );
	virtual void		SetNodeTransform( DWORD nodeID, const Matrix4D& m, 
											bool bLocalSpace = false );
	virtual ILight*		GetLight		( DWORD nodeID );
	virtual IGeometry*	GetGeometry		( DWORD nodeID );

	virtual int			GetNumGeometries( DWORD modelID );
	virtual IGeometry*	GetGeometry		( DWORD modelID, int idx, Matrix4D& tm );

	virtual int			GetNumSubNodes	( DWORD modelID );
	virtual const char*	GetSubNodeName	( DWORD modelID, int idx );

	virtual DWORD		CloneNode		( DWORD nodeID );
	virtual void		DeleteNode		( DWORD nodeID );
	virtual void		OnFrame			();

	bool				MakeParent		 	( DWORD parentID, DWORD childID, bool bParent );
	virtual int		    Render			 	( DWORD id, const Matrix4D* pTransform = NULL );
	virtual void		Render				();
	virtual void		Render				( DWORD id, int mX, int mY, float scale = 1.0f );


	virtual void		RenderShadow	 	( DWORD id, const Matrix4D* pTransform = NULL );
	virtual AABoundBox	GetBoundBox			( DWORD id );

	virtual void		Animate				( DWORD modelID, DWORD animID, float animTime, float blendInTime = 0.5f );
	virtual void		Animate				( DWORD modelID, float blendFactor,
												 DWORD animID1, float animTime1,
												 DWORD animID2, float animTime2 );
	virtual float		GetAnimTime			( DWORD animID );

	virtual bool		GetHeight			( DWORD id, Vector3D& pt, const Matrix4D* pTransform = NULL );
	virtual bool		GetLock				( DWORD id, const Vector3D& pt, const Matrix4D* pTransform = NULL );
	virtual bool		SwitchTo			( DWORD id, int index );
	virtual void		ReloadModels		();
	virtual void		SetCamera			( const Vector3D& lookAt, float viewVolWidth ){}
	virtual void		Init				();
	virtual void		SetViewPort			( float x, float y, float w, float h );

	virtual bool		IsBoxVisible		( const Vector3D& minv, const Vector3D& maxv );
	virtual bool		IsSphereVisible		( const Vector3D& center, float radius );
	virtual bool		IsPointVisible		( const Vector3D& pt );

	virtual void		RegisterVisible		( DWORD objID, DWORD modelID, const Matrix4D& tm );
	virtual void		RegisterVisible		( DWORD objID, const Vector3D& minv, const Vector3D& maxv );
	virtual void		RegisterVisible		( DWORD objID, const Vector3D& center, float radius );
	virtual void		RegisterVisible		( DWORD objID, DWORD gpID, int frameID, const Matrix4D& tm );
	virtual void		ClearVisibleCache	();
	virtual DWORD		PickVisible			( float scrX, float scrY, DWORD prevID = 0xFFFFFFFF );
	virtual void		ScanHeightmap		( DWORD nodeID, const Matrix4D& tm, float stepx, float stepy, SetHeightCallback callb );
	virtual void		ScanHeightmap		( DWORD nodeID, const Matrix4D& tm, float stepx, float stepy, VisitHeightCallback callb );
    virtual bool        Intersects          ( float mX, float mY, DWORD modelID, const Matrix4D& tm, float& dist );


	DECLARE_SCLASS(MediaManager,Group,MMGI);
private:
}; // class MediaManager

IMPLEMENT_CLASS( MediaManager );

//  global interface pointer
IMediaManager*		IMM		= MediaManager::instance();
IReflectionMap*		IRMap	= NULL;

bool  ConvertModel( Node* pModel );

/*****************************************************************************/
/*	MediaManager implementation
/*****************************************************************************/
MediaManager::MediaManager()
{
	SetName( "Model Manager" );
	AddRef();
} // MediaManager::MediaManager

Node* CompileModel( Node* pRoot, const char* path );

std::map<std::string, DWORD> g_MdlMap;
DWORD MediaManager::GetModelID( const char* fname )
{
    FilePath cpath = GetCurrentPath();
	if (!fname || fname[0] == 0) return c_BadID;
    std::map<std::string, DWORD>::iterator it = g_MdlMap.find( fname );
    if (it == g_MdlMap.end())
	{
        FilePath path( fname );
        path.SetExt( "c2m" );
		try
		{
            int resID = IRM->FindResource( path.GetFullPath() );
            if (resID == -1) return -1;
        	InStream& is = IRM->LockResource( path.GetFullPath() );
            
            FilePath path( IRM->GetPath( resID ) );
            _chdir( path.GetDrive() );
            _chdir( path.GetDir() );

            Node* pModel = Node::UnserializeSubtree( is );
        	is.Close();
            if (!pModel) return c_BadID;

            //  prefetch textures
            Node::Iterator it( pModel );
            while (it)
            {
                Node* pTex = (Node*)*it;
                if (pTex->IsA<Texture>()) pTex->Render();
                ++it;
            }

        	pModel = CompileModel( (Node*)pModel, path.GetFullPath() );
            g_MdlMap[fname] = pModel->GetID();
            cpath.SetCWD();
			return pModel->GetID();
		}
		catch(...)
		{
            cpath.SetCWD();
			return 0xFFFFFFFF; 
		}
	}
    cpath.SetCWD();
	return (*it).second;
} // MediaManager::GetModelID

const char* MediaManager::GetModelFileName( DWORD modelID )
{
    std::map<std::string, DWORD>::iterator it = g_MdlMap.begin();
    while (it != g_MdlMap.end())
    {
        if ((*it).second == modelID) return (*it).first.c_str();
        ++it;
    }
	return "";
} // MediaManager::

void MediaManager::Render( DWORD id, int mX, int mY, float scale )
{
	Node* pModel = NodePool::GetNode( id );
	if (!pModel) return;

    ICamera* pCam = ::GetCamera();
	if (!pCam) return;
	Matrix3D rot( Matrix3D::identity );
	
	Ray3D ray = pCam->GetPickRay( mX, mY );
	Vector3D xPt;

	if (ITerra)
	{
		if (!ITerra->Pick( ray.getOrig(), ray.getDir(), xPt )) return;
	}
	else
	{
		if (!ray.IntersectPlane( Plane::xOy, xPt )) return;
	}

	Matrix4D tm( Vector3D( scale, scale, scale ), rot, xPt );
	TransformNode::ResetTMStack( &tm );
	pModel->Render();
} // MediaManager::Render

int MediaManager::Render( DWORD id, const Matrix4D* pTransform )
{
	Node* pModel = NodePool::GetNode( id );
	if (!pModel) return -1;

	if (pTransform) TransformNode::Push( *pTransform );

	if (pModel->IsA<PEmitter>())
	{
		int inst = IEffMgr->InstanceEffect( pModel->GetID() );
		if (pTransform) IEffMgr->UpdateInstance( inst, *pTransform );
        else (IEffMgr->UpdateInstance( inst ));
	}
	else
	{
		pModel->Render();
	}

    if (pTransform) TransformNode::Pop();
    IRS->Flush();

    return 0;
} // MediaManager::Render

const float c_ShadowZBias = 64.0f;
void MediaManager::RenderShadow( DWORD id, const Matrix4D* pTransform )
{
	Node* pModel = NodePool::GetNode( id );
	if (!pModel) return;
	
	if (pTransform) TransformNode::ResetTMStack( pTransform );
	
    for (int i = 0; i < pModel->GetNChildren(); i++)
	{
		Node* pChild = pModel->GetChild( i );
		if (pChild->IsA<ShadowBlob>())
		{
			pChild->Render();
			break;
		}
	}

	if (pTransform)
	{
		TransformNode::ResetTMStack();
		IRS->ResetWorldTM();
	}

} // MediaManager::RenderShadow

//-------------------------------------------------------------------------------
//  Func:  MediaManager::GetHeight
//  Desc:  Returns height at the given (pt.x, pt.y) point (result is placed in pt.z)
//  Ret:   true if there is point of mesh at (pt.x, pt.y)
//  Rmrk:  Do not call it too often, if you don't want you program to 
//			execute forever
//-------------------------------------------------------------------------------
bool MediaManager::GetHeight( DWORD id, Vector3D& pt, const Matrix4D* pTransform )
{
	Node* pModel = NodePool::GetNode( id );
	if (!pModel) return false;
		
	pt.z = 0.0f;
	Vector3D dir;
	dir.set( 0.0f, -0.5f, c_CosPId6 );

	static const float c_Away = 10000.0f;
	Vector3D pos( pt.x, pt.y, 0.0f );

	pos.addWeighted( dir, c_Away );
	dir.reverse();

	Line3D ray( pos, dir );
	int			minIdx		= -1;
	Geometry*	pMinGeom	= pModel->FindChild<Geometry>( "Navimesh" );
	if (!pMinGeom) pMinGeom = pModel->FindChild<Geometry>( "Navimesh_geom" );
	float		minDist		= 0.0f;

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
		minDist	= hit.norm();
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
	Node* pModel = NodePool::GetNode( id );
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

	int				minIdx		= -1;
	Geometry*	pMinGeom	= pModel->FindChild<Geometry>( "Lockmesh" );
	float			minDist		= c_BigFloat;

	if (!pMinGeom) return false;

	minDist = pMinGeom->GetMesh().PickPoly( ray, minIdx );
	if (minIdx == -1) return false;
	return true;
} // MediaManager::GetLock

bool MediaManager::SwitchTo( DWORD id, int index )
{
	Switch* pSwitch = NodePool::GetNode<Switch>( id );
	if (!pSwitch) return false;
	pSwitch->SwitchTo( index );
	return true;
} // MediaManager::SetCursor

void MediaManager::Animate( DWORD modelID, DWORD animID, float animTime, float blendInTime )
{
	Node* pModel = NodePool::GetNode( modelID );
	if (!pModel) return;

	Animation*	pAnim = NodePool::GetNode<Animation>( animID );
    //  if no animation, restore model state to default position
	if (!pAnim) 
	{
		Iterator it( this, TransformNode::FnFilter );
		while (it) 
		{
			TransformNode* pNode = (TransformNode*)*it;
			pNode->SetToInitial();
			++it;
		}
		return;
	}

	if (pModel) pAnim->BindNode( pModel );

	pAnim->SetCurrentTime( animTime + pAnim->GetStartTime() );
	Animation::PushTime( animTime + pAnim->GetStartTime() );
	pAnim->Render();
	Animation::PopTime();

    //  update model instance
    int elID = m_ModelInstances.find( GetEntityContext() );
    if (elID == -1)
    {
        InstancedModel newInst; 
        elID = m_ModelInstances.add( GetEntityContext(), newInst );
    }   
    if (elID == -1) return;
    InstancedModel& inst = m_ModelInstances.elem( elID );

} // MediaManager::Animate

void MediaManager::Animate( DWORD modelID, float blendFactor,
								DWORD animID1, float animTime1,
								DWORD animID2, float animTime2 )
{
	Node* pModel = NodePool::GetNode( modelID );
	if (!pModel) return;

	Animation* pAnim1 = NodePool::GetNode<Animation>( animID1 );
	Animation* pAnim2 = NodePool::GetNode<Animation>( animID2 );

	if (!pAnim1 || !pAnim2) return; 
	
	pAnim1->BindNode( pModel );
	pAnim2->BindNode( pModel );

	Iterator it1( pAnim1, PRSAnimation::FnFilter );
	Iterator it2( pAnim2, PRSAnimation::FnFilter );

	while (it1)
	{
		PRSAnimation* pAnm1 = (PRSAnimation*)*it1;
		PRSAnimation* pAnm2 = (PRSAnimation*)*it2;
		TransformNode* pTM = (TransformNode*)pAnm1->GetInput( 0 );
		if (pTM && pTM->IsA<TransformNode>())
		{
			assert( !strcmp( pAnm1->GetName(), pAnm2->GetName() ) && 
					!strcmp( pAnm1->GetName(), pTM->GetName() ) );
			Matrix4D tm = PRSAnimation::GetTransform( pAnm1, animTime1, pAnm2, animTime2, blendFactor );
			pTM->SetTransform( tm );
		}
		++it1;
		++it2;
	}

} // MediaManager::Animate

float MediaManager::GetAnimTime( DWORD animID )
{
	Animation* pAnim = NodePool::GetNode<Animation>( animID );
	if (!pAnim) return 0.0f;
	return pAnim->GetAnimationTime();
} // MediaManager::GetAnimTime

DWORD MediaManager::GetNodeID( DWORD modelID, const char* nodeName )
{
	Node* pModel = NodePool::GetNode( modelID );
	if (!pModel) return c_BadID;
	Node* pChild = pModel->FindChildByName( nodeName );
	if (!pChild) return c_BadID;
	return pChild->GetID();
} // MediaManager::GetNodeID

DWORD MediaManager::GetNodeID( const char* name )
{
	Node* pNode = NodePool::GetNodeByName( name );
	if (!pNode) return c_BadID;
	return pNode->GetID();
}

void MediaManager::ShowNode( DWORD id, bool bShow )
{
	Node* pNode = NodePool::GetNode( id );
	if (pNode) pNode->SetInvisible( !bShow );
} // MediaManager::ShowNode

Matrix4D MediaManager::GetNodeTransform( DWORD nodeID, bool bLocalSpace )
{
	if (nodeID == c_BadID) return Matrix4D::identity;
	TransformNode* pNode = NodePool::GetNode<TransformNode>( nodeID );
	if (!pNode) return Matrix4D::identity;
	if (bLocalSpace) return pNode->GetTransform(); 
		else return pNode->GetWorldTM();
} // MediaManager::GetNodeTransform

void MediaManager::SetNodeTransform( DWORD nodeID, const Matrix4D& m, 
											bool bLocalSpace )
{
	TransformNode* pNode = NodePool::GetNode<TransformNode>( nodeID );
	if (!pNode) return;
	if (bLocalSpace) pNode->SetTransform( m ); else pNode->SetWorldTM( m );
} // MediaManager::SetNodeTransform

void MediaManager::SetVisible( DWORD nodeID, bool bVisible )
{
	Node* pNode = NodePool::GetNode( nodeID );
	if (pNode)
	{
		pNode->SetInvisible( !bVisible );
	}
} // MediaManager::SetVisible

bool MediaManager::MakeParent( DWORD parentID, DWORD childID, bool bParent )
{
	Node* pParent = NodePool::GetNode( parentID );
	Node* pChild  = NodePool::GetNode( childID );
	
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

ILight*	MediaManager::GetLight( DWORD nodeID )
{
	LightSource* pLight = NodePool::GetNode<LightSource>( nodeID );
	return pLight;
}

IGeometry*	MediaManager::GetGeometry( DWORD nodeID )
{
	Node* pG = NodePool::GetNode( nodeID );
	if (!pG->IsA<Geometry>())
	{
		pG = pG->FindChildFn<Geometry>();
	}
	return (Geometry*)pG;
} // MediaManager::GetGeometry

void MediaManager::Init()
{
	if (Root::instance()->GetNChildren() == 0) Root::instance()->CreateGuts();
}

void MediaManager::Render()
{
	Root::instance()->Render();
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
	Node* pG = NodePool::GetNode( nodeID );
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
	Node* pG = NodePool::GetNode( nodeID );
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

int	MediaManager::GetNumGeometries( DWORD modelID ) 
{
	Node* pRoot = NodePool::GetNode( modelID );
	int nG = 0;
	Iterator it( pRoot, Geometry::FnFilter );
	while (it) { nG++; ++it; }
	return nG;
} // MediaManager::GetNumGeometries

IGeometry* MediaManager::GetGeometry( DWORD modelID, int idx, Matrix4D& tm )
{
	Node* pRoot = NodePool::GetNode( modelID );
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
	Node* pRoot = NodePool::GetNode( nodeID );
	if (!pRoot) return c_BadID;
	Node* pNewNode = pRoot->CloneSubtree();
	return pNewNode->GetID();
}

void MediaManager::DeleteNode( DWORD nodeID )
{
	Node* pNode = NodePool::GetNode( nodeID );
	if (!pNode) return;
    NodePool::DestroyNode( pNode );

    std::map<std::string, DWORD>::iterator it = g_MdlMap.begin();
    while (it != g_MdlMap.end())
    {
        if ((*it).second == nodeID)     
        {
            g_MdlMap.erase( it );
            break;
        }
        ++it;
    }
} // MediaManager::DeleteNode

int	MediaManager::GetNumSubNodes( DWORD modelID )
{
	Node* pRoot = NodePool::GetNode( modelID );
	int nG = 0;
	Iterator it( pRoot, TransformNode::FnFilter );
	while (it) { nG++; ++it; }
	return nG;
} // MediaManager::GetNumSubNodes

const char*	MediaManager::GetSubNodeName( DWORD modelID, int idx )
{
	Node* pRoot = NodePool::GetNode( modelID );
	int nG = 0;
	Iterator it( pRoot, TransformNode::FnFilter );
	while (it) 
	{ 
		if (nG == idx) 
		{
			Node* pNode = (Node*)*it;
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
	Node* pNode = NodePool::GetNode( id );
	if (!pNode) return AABoundBox::null;
    if (pNode->IsA<Model>())
    {
        return ((Model*)pNode)->GetAABB();
    }
    //  not sure about this...
    return AABoundBox::null;
} // MediaManager::GetBoundBox

bool MediaManager::Intersects( float mX, float mY, DWORD modelID, 
                                    const Matrix4D& tm, float& dist )
{
    Node* pNode = NodePool::GetNode( modelID );
    if (!pNode) return false;
    AABoundBox aabb = GetBoundBox( modelID );
    
    ICamera* pCam = ::GetCamera();
    Ray3D ray = pCam->GetPickRay( mX, mY );
    
    Matrix4D itm;
    itm.inverse( tm );
    ray.Transform( itm );
    if (!aabb.Overlap( ray )) return false;
    dist = ray.getOrig().distance( aabb.GetCenter() );
    return true;
} // MediaManager::Intersects

#include "sgAnimBlend.h"
bool ConvertModel( Node* pModel )
{
    if (pModel->IsA<AnimationBlock>()) return false;
    std::vector<Node*>  gNodes;
    Node::Iterator it( pModel, SkinnedGeometry::FnFilter );
    while (it)
    {
        Node* pNode = (Node*)*it;
        Node* pParent = pNode->GetParent();
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
        Node* pNode = gNodes[i];
        pNode->AddRef();
        pModel->RemoveChild( pNode );
        pModel->AddChild( pNode );
        pNode->Release();
    }

    Node::Iterator git( pModel, SkinnedGeometry::FnFilter );
    while (git)
    {
        SkinnedGeometry* pGeom = (SkinnedGeometry*)*git;
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
    Node* pMdl = NodePool::GetNode( mdlID );
    if (!pMdl) return false;
    Node::Iterator it( pMdl, Geometry::FnFilter );
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















