/*****************************************************************************/
/*	File:	MaxExport.cpp
/*	Desc:	Export from max to c2m
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07.11.2002
/*****************************************************************************/
#include "maxPre.h"

#include "resource.h"

#include "sg.h"

#include "MaxUtil.h"
#include "MaxExport.h"
#include "meshadj.h"
#include "stdmat.h"
#include "modstack.h"
#include "phyexp.h"
#include "bipexp.h"
#include "decomp.h"

#pragma warning (disable : 4530)
static maxExporterClassDesc c2mExporterDesc;

ClassDesc2* GetMaxExporterDesc() 
{ 
	return &c2mExporterDesc; 
}

/*****************************************************************************/
/*	maxExporter implementation
/*****************************************************************************/
maxExporter::maxExporter() : SceneExport(), c2mExporter()
{
	m_pPhysique = 0;
	SetPluginFileName( "c2mExport.dle" );
	curObject = NULL;
	flipTM.flipXY();

	//  calculate camera tweaking transform matrix
	Matrix4D rot;
	camTweakTM.rotation( Vector3D::oX, -c_PI );
	rot.rotation( Vector3D::oZ, c_PI * 0.5f );
	camTweakTM *= rot;
}

maxExporter::~maxExporter() 
{
}

/*****************************************************************************/
/*	3d Max base exporter class methods implementations
/*****************************************************************************/
int maxExporter::ExtCount()
{
	return 1;
}

const TCHAR *maxExporter::Ext( int n )
{		
	return _T("c2m");
}

const TCHAR *maxExporter::LongDesc()
{
	return _T("Export to the Cossacks2 model format");
}
	
const TCHAR *maxExporter::ShortDesc() 
{			
	return _T("c2m ModelObject");
}

const TCHAR *maxExporter::AuthorName()
{			
	return _T("Silver");
}

const TCHAR *maxExporter::CopyrightMessage() 
{	
	return _T("Copyright(C) 2000-2002 GSC Game World");
}

const TCHAR *maxExporter::OtherMessage1() 
{		
	return _T("");
}

const TCHAR *maxExporter::OtherMessage2() 
{		
	return _T("");
}

unsigned int maxExporter::Version()
{				
	return 100;
}

void maxExporter::ShowAbout( HWND hWnd )
{			
}

BOOL maxExporter::SupportsOptions( int ext, DWORD options )
{
	return TRUE;
}

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::DoExport
/*	Desc:	Core max exporting process entry point
/*---------------------------------------------------------------------------*/
int	maxExporter::DoExport(	const TCHAR*	name,
							ExpInterface*	ei,
							Interface*		i, 
							BOOL			suppressPrompts, 
							DWORD			options )
{
	strcpy( modelFileName, name );
	OnOpenExportDlg();
	//  re-assign silly interface names
	iExport = ei;
	iMax	= i;

	if(!suppressPrompts) ShowExportDialog();
	return TRUE;
} // maxExporter::DoExport

void maxExporter::PreTraverseSubTree( INode* node )
{
	//ObjectState os = node->EvalWorldState( curTime );

	//Control* c = node->GetTMController();
	//if (c)
	//{
	//	IBipedExport* iBip = (IBipedExport*)c->GetInterface( I_BIPINTERFACE );
	//	if (iBip)
	//	{
	//		iBip->RemoveNonUniformScale( 1 );
	//	}
	//}

	int numCh = node->NumberOfChildren();
	for (int i = 0; i < numCh; i++) PreTraverseSubTree( node->GetChildNode( i ) );
} // maxExporter::PreTraverseSubTree

SkinInfo* maxExporter::NewSkin( SkinInfo* pOrigSkin )
{
	SkinInfo skin;
	int elIdx = skins.Add( skin );
	SkinInfo* pSkin = &(skins.Elem( elIdx ));
	if (pOrigSkin)
	{
		memcpy( pSkin->bones, pOrigSkin->bones, sizeof( pSkin->bones ) );
		memcpy( pSkin->offs,  pOrigSkin->offs,  sizeof( pSkin->offs  ) );
		pSkin->nBones	= pOrigSkin->nBones;
	}
	return pSkin;
} // maxExporter::NewSkin

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::PreprocessScene	
/*	Desc:	Traverse scene counting nodes etc.
/*---------------------------------------------------------------------------*/
void maxExporter::PreprocessScene()
{
	m_SamplingInterval = 1000.0f / float( GetFrameRate() );
	
	ClearTextureTable();
	skins.Clear();

	nTransformNodes = TotalChildrenNum( iMax->GetRootNode() );
	
	nodeHash.reset();
	PreTraverseSubTree( iMax->GetRootNode() );

	if (nTransformNodes > 0) progressPerNode = 0.9f / (double)nTransformNodes;
} // maxExporter::PreprocessScene

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::ProcessNodeSubTree	
/*	Desc:	Extracts data from the given 3d max scene node (and, recurrently,
/*				from all its subtree)
/*---------------------------------------------------------------------------*/
TransformNode* maxExporter::ProcessNodeSubTree( INode* node, SNode* pParent )
{
	if (node == NULL)
	{
		Msg( "Cannot process scene graph subtree." );
		return NULL;
	}

	if (abortProcess) return NULL;

	//  acnowledge user we are in this node
	TCHAR* nodeName = node->GetName();
	Msg( "Processing node <%s>...", nodeName );
	
	MsgIndent();
	StepProgressNode();

	Animation* pParentAnim = m_pParentAnim;
	TransformNode* pNode = ProcessNode( node, pParent );
	
	//  do stuff recursively for all children
	int numCh = node->NumberOfChildren();

	for (int i = 0; i < numCh; i++) 
	{
		TransformNode* pChildNode = ProcessNodeSubTree( node->GetChildNode( i ), pNode );
		
		if (pChildNode && pNode)
		{
			pNode->AddChild( pChildNode );
		}
		if (abortProcess) return NULL;
	}
	MsgUnindent();

	m_pParentAnim = pParentAnim;
	return pNode;
} // maxExporter::ProcessNodeSubTree

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::ProcessNodeShaders
/*	Desc:	analyses node materials, adds correspondent shaders to table
/*	Parm:	node - node which material to process
/*	Ret:	false if there are no material used at the node
/*---------------------------------------------------------------------------*/
bool maxExporter::ProcessNodeShaders( INode* node )
{
	ClearNodeShaders();
	
	Mtl* mtl = node->GetMtl();
	if (!mtl) 
	{
		Msg( "No material at the node." );
		//  add empty shader
		GShader sh;
		AddNodeShader( sh );
		return false;
	}

	Class_ID mtlClassID = mtl->ClassID();
	if (mtlClassID == Class_ID(DMTL_CLASS_ID, 0))
	{
		AddNodeShader( ProcessStdMtl( mtl ) );
		return true;
	}
	
	//  material has a bunch of sub-materials
	if (mtlClassID == Class_ID(MULTI_CLASS_ID, 0))
	{
		int nSubMtl = mtl->NumSubMtls();
		for (int cmat = 0; cmat < nSubMtl; cmat++)
		{
			Mtl* subMtl = mtl->GetSubMtl( cmat );
			if (subMtl && subMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
			{
				AddNodeShader( ProcessStdMtl( subMtl ) );
			}
		}
		return true;
	}

	if (mtlClassID == Class_ID(BAKE_SHELL_CLASS_ID, 0))
	{
		int nSub = mtl->NumSubMtls();
		if (nSub == 2)
		{
			ProcessShellMtl( mtl );
		}
		else
		{
			GShader sh;
			AddNodeShader( sh );
		}
		return true;
	}
	
	Msg( "Unsupported material at the node. Geometry will be exported without material." );
	
	//  add empty shader
	GShader sh;
	AddNodeShader( sh );

	return false;
} // maxExporter::ProcessNodeShaders

void maxExporter::ProcessShellMtl( Mtl* mtl )
{
	GShader		sh;
	Class_ID	mtlClassID;
	Mtl*		dmat = mtl->GetSubMtl( 0 );
	Mtl*		lmat = mtl->GetSubMtl( 1 );

	mtlClassID = lmat->ClassID();
	if (mtlClassID != Class_ID(DMTL_CLASS_ID, 0))
	{
		Msg( "Shell material contains funny baked texture."
				"No material will be exported." );
		AddNodeShader( sh );
		return;
	}

	Shader dss;
	dss.SetName( "lmapped" );
	sh.AddAttribute( AddDssNode( &dss ) );

	
	ExtractDiffuseMap( lmat, sh, 1 );

	mtlClassID = dmat->ClassID();
	
	if (mtlClassID == Class_ID(DMTL_CLASS_ID, 0))
	{
		GShader nSh = ProcessStdMtl( dmat, false, 2 );
		nSh.AddAttribute( 0, sh.GetAttribute( 0 ) );
		nSh.AddAttribute( 0, sh.GetAttribute( 1 ) );
		AddNodeShader( nSh );
		return;
	}

	if (mtlClassID == Class_ID(MULTI_CLASS_ID, 0))
	{
		int nSubMtl = dmat->NumSubMtls();
		for (int cmat = 0; cmat < nSubMtl; cmat++)
		{
			GShader nSh = ProcessStdMtl( dmat->GetSubMtl( cmat ), false, 2 );
			nSh.AddAttribute( 0, sh.GetAttribute( 0 ) );
			nSh.AddAttribute( 0, sh.GetAttribute( 1 ) );

			AddNodeShader( nSh );
		}
		return;
	}

} // maxExporter::ProcessShellMtl

bool maxExporter::ExtractDiffuseMap( Mtl* mtl, GShader& sh, int stage )
{
	if (!mtl) return false;
	int cLay = 0;
	float amt = 1.0;
	bool bHasDifmap = false;
	Texmap* difMap = mtl->GetSubTexmap( ID_DI );
	StdMat*	smat   = (StdMat*)mtl;
	if (difMap && smat->MapEnabled( ID_DI ))
	{
		amt = smat->GetTexmapAmt( ID_DI, 0 );

		if (difMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			BitmapTex *bmt = (BitmapTex*) difMap;

			Texture tex;
			tex.SetStage( stage );
			tex.SetName( bmt->GetMapName() );
			sh.AddAttribute( AddTextureNode( &tex ) );

			//  check for transparency
			int alphaSource = bmt->GetAlphaSource();
			bHasDifmap	= true; 
		}
		cLay++;
	}
	return bHasDifmap;
} // maxExporter::ExtractDiffuseMap

bool maxExporter::ExtractBumpMap( Mtl* mtl, GShader& sh, int stage )
{
	if (!mtl) return false;
	int cLay = 0;
	float amt = 1.0;
	bool bHasBumpmap = false;
	StdMat*	smat   = (StdMat*)mtl;
	Texmap* bumpMap = mtl->GetSubTexmap( ID_BU );
	if (bumpMap && smat->MapEnabled( ID_BU ))
	{
		amt = smat->GetTexmapAmt( ID_BU, 0 );

		if (bumpMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			BitmapTex *bmt = (BitmapTex*) bumpMap;

			Texture tex;
			tex.SetStage( 1 );
			tex.SetName( bmt->GetMapName() );
			sh.AddAttribute( AddTextureNode( &tex ) );

			bHasBumpmap	= true; 
		}
		cLay++;
	}
	return bHasBumpmap;
} // maxExporter::ExtractBumpMap

bool maxExporter::ExtractEnvMap( Mtl* mtl, GShader& sh, int stage )
{
	if (!mtl) return false;
	int		cLay		= 0;
	float	amt			= 1.0;
	bool	bHasEnvmap	= false;
	StdMat*	smat		= (StdMat*)mtl;
	Texmap* envMap		= mtl->GetSubTexmap( ID_RL );
	if (envMap && smat->MapEnabled( ID_RL ))
	{
		amt = smat->GetTexmapAmt( ID_RL, 0 );

		if (envMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			BitmapTex *bmt = (BitmapTex*) envMap;

			Texture tex;
			tex.SetStage( 1 );
			tex.SetName( bmt->GetMapName() );
			sh.AddAttribute( AddTextureNode( &tex ) );

			bHasEnvmap		= true;
		}
		cLay++;
	}
	return bHasEnvmap;
} // maxExporter::ExtractEnvMap

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::ProcessStdMtl	
/*---------------------------------------------------------------------------*/
GShader maxExporter::ProcessStdMtl( Mtl* mtl, bool bAssignDss, int startStage )
{
	StdUVGen*	uvgen		= 0;
	StdMat*		smat		= (StdMat*)mtl;
	GShader		sh;

	if (!mtl) return sh;

	bool		bTwoSided	= (smat->GetTwoSided() != 0);
	bool		bHasBumpmap	= false;
	bool		bHasEnvmap	= false;

	//Material sgmtl;
	////  material colors

	//sgmtl.SetAmbient	( ColorToDWORD( mtl->GetAmbient()	) );
	//sgmtl.SetDiffuse	( ColorToDWORD( mtl->GetDiffuse()	) );
	//sgmtl.SetSpecular	( ColorToDWORD( mtl->GetSpecular()	) );
	//sgmtl.SetShininess	( mtl->GetShinStr() * 100.0f );
	//sh.AddAttribute( AddMtlNode( &sgmtl ) );

	//  diffuse map
	ExtractDiffuseMap( mtl, sh );

	////  bump map
	//bHasBumpmap = ExtractBumpMap( mtl, sh, startStage );

	////  environment map	
	//if (bHasBumpmap) startStage++;
	//bHasEnvmap = ExtractEnvMap( mtl, sh, startStage );

	
	if (bAssignDss)
	{
		//  assign device stateset
		Shader dss;
        dss.SetName( "vertex_lit" );
		if (bHasEnvmap) dss.SetName( "env_mapped" );
        sh.AddAttribute( AddDssNode( &dss ) );
	}
	return sh;
} // maxExport::ProcessStdMtl

void maxExporter::PostprocessSkinning()
{
	int nSkins = skins.GetNElem(); 
	for (int i = 0; i < nSkins; i++)
	{
		SkinInfo& skin = skins.Elem( i );
		Skin* pGeom = skin.pGeom;
		assert( pGeom );
		if (skin.nBones == 0) continue;
		for (int j = 0; j < skin.nBones; j++)
		{
			INode* pNode = skin.bones[j];
			HashedNode hnode( pNode, 0 );
			int idx = nodeHash.find( hnode );
			assert( idx != NO_ELEMENT );
			TransformNode* pBoneNode = (TransformNode*)nodeHash.elem( idx ).pNode;
			pGeom->AddBoneOffset( skin.GetBoneOffset( j ) );
			pGeom->AddInput( pBoneNode );
            pGeom->GetBaseMesh() = pGeom->GetMesh();
		}
	}
} // maxExporter::PostprocessSkinning

bool maxExporter::ShouldSkipNode( INode* node )
{
	Control *ctrl = node->GetTMController();
	if (ctrl)
	{
		//  skip biped footprints
		if (ctrl->ClassID() == FOOTPRINT_CLASS_ID) return true;
	}
	return false;
} // maxExporter::ShouldSkipNode

/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::ProcessNode	
/*	Desc:	Extracts data from the given 3d max scene node
/*---------------------------------------------------------------------------*/
TransformNode* maxExporter::ProcessNode( INode* node, SNode* pParent )
{	
	curNode = node;
	TransformNode* pNode = NULL;

	//  treat the scene root
	if (node->IsRootNode()) 
	{
		if (bExportAnimation)
		{
			m_pAnimGroup = new AnimationBlock();
			m_pParentAnim = m_pAnimGroup;
			char anmName[512];
			sprintf( anmName, "%s", modelName );
			m_pAnimGroup->SetName( anmName );
			
			Interval range = iMax->GetAnimRange();
			m_pAnimGroup->SetAnimationTime( TimeToMs( range.Duration() ) );
			m_pAnimGroup->SetStartTime( TimeToMs( range.Start() ) );
		}

		if (!bAnimationOnly) 
		{
			pNode = new TransformNode();
			pNode->SetName( modelName );
			m_pModel = (TransformNode*)pNode;
		}

		return pNode;
	}

	if (ShouldSkipNode( node )) return NULL;

	//  no node transform tweaking by default
	localNodeTweakTM = Matrix4D::identity;

	//  extract node name
	strcpy( curNodeName, node->GetName() );

	//  create animation node 
	PRSAnimation* pAnimation = NULL;
	if (bExportAnimation)
	{
		pAnimation = (PRSAnimation*)GetPRSAnimation( node );
		if (pAnimation)
		{
			pAnimation->AddInput( pNode );
			pAnimation->SetBaseAnimationName( modelName );			
			if (m_pParentAnim) m_pParentAnim->AddChild( pAnimation );
			m_pParentAnim = pAnimation;
		}
	}

	if (bAnimationOnly)
	{
		return NULL;
	}

	//  evaluate node
	ObjectState os = node->EvalWorldState( curTime );
	curObject = os.obj;
	if (!curObject) return NULL;

	//  step progress on progress indicator
	StepProgressNode();		

	//  check if node belongs to biped
	bool bBiped = false;
	Control *ctrl = node->GetTMController();
	if (ctrl)
	{
		if ((ctrl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
			(ctrl->ClassID() == BIPBODY_CONTROL_CLASS_ID)  ||
			(ctrl->ClassID() == FOOTPRINT_CLASS_ID))
		{
			bBiped = true;
		}
	}

	//  check node class ID
	Class_ID classID = curObject->ClassID();
	if (	classID == Class_ID( SIMPLE_CAM_CLASS_ID, 0 ) ||
			classID == Class_ID( LOOKAT_CAM_CLASS_ID, 0 ) )
	//  camera
	{
		localNodeTweakTM = camTweakTM;
		pNode = GetCameraNode( node );
	}
	else if (classID == Class_ID( TARGET_CLASS_ID, 0 )) 
	//  camera/light target
	{
		return NULL;
	}
	else if (	classID == Class_ID( BONE_CLASS_ID, 0 ) || 
				classID == BONE_OBJ_CLASSID || 
				bBiped) 
	//  bone
	{
		pNode = GetBoneNode( node );
	}
	else if (classID == Class_ID( DUMMY_CLASS_ID, 0 ))
	//  locator or group
	{
		//  when dummy object does not have children, we suppose it is locator.
		//	else it is grouping node.
		if (node->NumberOfChildren() == 0)
		{
			pNode = GetLocatorNode( node );
		}
		else
		{
			pNode = GetTransformNode( node );
		}
	}
	else if (	classID == Class_ID( DIR_LIGHT_CLASS_ID, 0 ) || 
				classID == Class_ID( TDIR_LIGHT_CLASS_ID, 0 ) ||
				classID == Class_ID( OMNI_LIGHT_CLASS_ID, 0 ))
	//  light
	{
		localNodeTweakTM = camTweakTM;
		pNode = GetLightNode( node );
	}
	else if (curObject->SuperClassID()	== GEOMOBJECT_CLASS_ID) 
	//  mesh
	{
		pNode = GetMeshNode( node );
	}
    //  something else. Put it into transform node in case it has children nodes.
    else
    {
        if (node->NumberOfChildren() > 0) pNode = new TransformNode();
    }

	if (pNode)
	{
        Matrix4D tm = GetNodeTransform( node );
		pNode->SetInitialTM ( tm );
        pNode->SetTransform ( tm );
		pNode->SetName      ( node->GetName() );
		pNode->SetParent    ( pParent );
		if (pAnimation) pAnimation->AddInput( pNode );
	}

	HashedNode hnode( node, pNode );
	int hIdx = nodeHash.find( hnode );
	if (hIdx == NO_ELEMENT) 
	{
		nodeHash.add( hnode );
	}
	else nodeHash.elem( hIdx ).pNode = pNode;

	return pNode;
}  // maxExporter::ProcessNode

Matrix4D maxExporter::GetNodeTransform( INode* node, TimeValue* pTime )
{
    Matrix4D flip; flip.flipYZ();

	TimeValue ctime = pTime ? *pTime : curTime;
	//  flip world around xOy plane, to make it appear in left-handed coordinate system
	Matrix4D cM( flipTM ), pM( flipTM );
	
	cM *= localNodeTweakTM; 
    Matrix3 tm = node->GetNodeTM( ctime ); 

    Matrix4D ctm = ConvertMatrix( tm );
    if (tm.Parity()) 
    {
        ctm.mulLeft( flip );
    }
    else
    {
        int t = 0;
    }
	cM *= ctm;

	INode* parent = node->GetParentNode();
	if (parent)
	{
        Matrix3 maxPTM = parent->GetNodeTM( ctime );
        Matrix4D ptm = ConvertMatrix( maxPTM );
        //if (maxPTM.Parity()) ptm.mulLeft( flip );
        pM *= ptm;
	}

	Matrix4D pMinv;
	pMinv.inverse( pM );
	cM *= pMinv;

	return cM;
} // maxExporter::ConvertTransform

Modifier* maxExporter::FindModifier( INode* node, Class_ID modID )
{
	Object* obj = node->GetObjectRef();
	if (obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* dm = (IDerivedObject*)obj;
		int nMod = dm->NumModifiers();
		for(int i = 0; i < nMod; i++)
		{
			Modifier* modifier = dm->GetModifier(i);
			Class_ID id = modifier->ClassID(); 
			if (id == modID)
			{
				return modifier;
			}
		}
	}
	return 0;
} // maxExporter::FindModifier

bool maxExporter::FetchSkinVertex( GVertex& vert, int vertIdx )
{
    if (!m_pSkin || !m_pSkinData) return false;

    //  fill in bones list
    if (curSkin->nBones == 0)
    {
        int nBones = m_pSkin->GetNumBones();
        for (int i = 0; i < nBones; i++)
        {
            INode* pBone = m_pSkin->GetBone( i );
            curSkin->AddBone( pBone );
            curSkin->SetBoneOffset( i, CalcBoneOffset( pBone ) );
        }
    }

    int nW = m_pSkinData->GetNumAssignedBones( vertIdx );
    for (int i = 0; i < nW; i++)
    {
        int   bIdx      = m_pSkinData->GetAssignedBone( vertIdx, i );
        float w         = m_pSkinData->GetBoneWeight( vertIdx, i );
        vert.boneRef[i] = bIdx;
        vert.weight[i]  = w;
    }
    vert.nWeights = nW;

    if (vert.nWeights > nWeightsPerBone) nWeightsPerBone = vert.nWeights;
    return true;
} // maxExporter::FetchSkinVertex

bool maxExporter::FetchPhysiqueVertex( GVertex& vert, int vertIdx )
{
	IPhyVertexExport* iPVertExport = 
        (IPhyVertexExport*)m_pPhysique->GetVertexInterface( vertIdx ); 
	if (!iPVertExport)
	{
		Msg( "Cannot extract interface IPhyVertexExport." );
		return false;
	}

	//  check vertex type (rigid/rigid blended)
	DWORD vtype;
	vtype = iPVertExport->GetVertexType();
	m_pPhysique->ReleaseVertexInterface( iPVertExport );    

	if (vtype == RIGID_TYPE) 
	{
		IPhyRigidVertex* iPRigid = (IPhyRigidVertex*) m_pPhysique->GetVertexInterface( vertIdx );
		if (!iPRigid)
		{
			Msg( "Cannot extract interface IPhyRigidVertex." );
			return false;
		}

		//  only single bone
		INode* boneNode = iPRigid->GetNode();
		if (!boneNode) return false;

		vert.nWeights = 1;

		bool bNeedCalcOffset = !curSkin->IsBonePresent( boneNode );

		assert( curSkin );
		int bIdx = curSkin->AddBone( boneNode );
		
		//  calculate baked bone offset
		if (bNeedCalcOffset)
		{
			curSkin->SetBoneOffset( bIdx, CalcBoneOffset( boneNode ) );
		}
		
		vert.boneRef[0] = bIdx;
		vert.weight[0] = 1.0f;
		assert( bIdx != NO_ELEMENT );

		haveBlendedVertices = true;
		m_pPhysique->ReleaseVertexInterface( iPRigid );
	}
	else if (vtype == RIGID_BLENDED_TYPE)
	{
		IPhyBlendedRigidVertex* iPBRigid = 
			(IPhyBlendedRigidVertex*) m_pPhysique->GetVertexInterface( vertIdx );
		if (!m_pPhysique)
		{
			Msg( "Cannot extract interface IPhyBlendedRigidVertex." );
			return false;
		}

		// fetch number of bones that influence this vertex
		int nBones = iPBRigid->GetNumberNodes();

		INode* boneNode = NULL;
		for (int cB = 0; cB < nBones; cB++)
		{
			boneNode = iPBRigid->GetNode( cB );

			assert( curSkin );			
			bool bNeedCalcOffset = !curSkin->IsBonePresent( boneNode );
			int bIdx = curSkin->AddBone( boneNode );
			assert( bIdx != NO_ELEMENT );

			//  calculate baked bone offset
			if (bNeedCalcOffset)
			{
				curSkin->SetBoneOffset( bIdx, CalcBoneOffset( boneNode ) );
			}
		
			float weight = iPBRigid->GetWeight( cB );

			if (!vert.AddWeight( bIdx, weight ))
			{
				Msg( "Too much weights per vertex.  Skin:%s,  Bone:%s", 
					curNodeName, boneNode->GetName() );
				vert.LeaveBiggerWeight( bIdx, weight );
				continue;
			}			
		}

		haveBlendedVertices = true;
		m_pPhysique->ReleaseVertexInterface( iPBRigid );
	}
	else
	{
		//  incorrect vertex type
		assert( false );
	}

	if (vert.nWeights > nWeightsPerBone) 
	{
		nWeightsPerBone = vert.nWeights;
	}

	return true;
} // maxExporter::FetchPhysiqueVertex

Matrix4D maxExporter::CalcBoneOffset( INode* boneNode )
{
    if (!boneNode) return Matrix4D::identity;
	Matrix3 nTM;
	Matrix3 bTM;

	bTM = boneNode->GetNodeTM( curTime );
	nTM = curNode->GetNodeTM ( curTime );

	Matrix4D tmM	= ConvertMatrix( nTM );
	Matrix4D tmB	= ConvertMatrix( bTM );

	tmM.mulLeft( flipTM );
	tmB.mulLeft( flipTM );

	Matrix4D tmBI;
	tmBI.inverse( tmB );
	Matrix4D offsM( tmM );
	offsM *= tmBI;

	return offsM;
} // maxExporter::CalcBoneOffset

bool maxExporter::InitNodeSkinning( INode* node )
{
    m_pSkin         = NULL;
    m_pSkinData     = NULL;
    m_pPhysique     = NULL;
    nWeightsPerBone = 0;
    Modifier* pMod  = NULL;
    
    //  check for 'Skin' modifier
    pMod = FindModifier( node, SKIN_CLASSID );
    if (pMod)
    {
        m_pSkin = (ISkin*) pMod->GetInterface( I_SKIN );
        if (m_pSkin) m_pSkinData = m_pSkin->GetContextInterface( node );
        bHaveWeights = true;
        return true;
    }
    
    //  check for 'Physique' modifier
	pMod = FindModifier( node, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B) );
    if (pMod)
    {
	    IPhysiqueExport* iPExport = (IPhysiqueExport*)pMod->GetInterface( I_PHYINTERFACE );
        if (!iPExport) return false;
        m_pPhysique = (IPhyContextExport*) iPExport->GetContextInterface( node );
        if (!m_pPhysique) return false;
        // convert to rigid with blending
        m_pPhysique->ConvertToRigid	( TRUE );
        m_pPhysique->AllowBlending	( TRUE );
        bHaveWeights = true;
        return true;
    }

	return false;
} // maxExporter::InitNodePhyContext

void LinkSG();
/*---------------------------------------------------------------------------*/
/*	Func:	maxExporter::StartExporting
/*	Desc:	Starts exporting scene process 
/*---------------------------------------------------------------------------*/
void maxExporter::StartExporting()
{
    static bool s_bSGLinked = false;
    if (!s_bSGLinked) { LinkSG(); s_bSGLinked = true; }

	InitInternal();

	//  texture path stuff
	CreateTextureDir();
	Msg( "Begin exporting scene %s ...", modelFileName );
	//  count scene nodes etc
	Msg( "Preprocessing..." );
	PreprocessScene();
	Msg( "Number of nodes in MAX scene graph: %d", nTransformNodes );
	
	curTime = iMax->GetTime();

	TransformNode* pScene = (TransformNode*)ProcessNodeSubTree( iMax->GetRootNode(), NULL );
	PostprocessSkinning();
	PostprocessScene();

	//  animation 
	ProcessAnimation();
	if (m_pModel && m_pAnimGroup && m_pAnimGroup->GetNChildren() > 0) 
	{
		m_pModel->AddChild( m_pAnimGroup );
	}

	if (abortProcess)
	{
		Msg( "Export was canceled." );
		return;
	}

	CopySceneTextures();

	if (!WriteModel( pScene ))
	{
		Msg( "Error writing model." );
		ShutInternal();
		return;
	}

	SetProgress( 0.95 );

	Msg( "Polygons : %d", GetTotalPolyNum() );
	Msg( "Vertices : %d", GetTotalVertNum() );
	Msg( "Textures : %d", GetTotalTexNum()  );

	SetProgress( 1.00 );

	Msg( "SUCCESS." );

	ShutInternal();
} // maxExporter::StartExporting

TransformNode* maxExporter::GetCameraNode( INode* node )
{
	CameraObject*       camObj = (CameraObject*)curObject;
	Interval            validIvl;
	CameraState         cs;

	IReflected* pCamera = NULL;

	if (camObj->EvalCameraState( curTime, validIvl, &cs ) != REF_SUCCEED)
	{
		Msg( "Error processing camera <%s>", node->GetName() );
		return NULL;
	}

	float aspect = ((float)iMax->GetRendWidth())/((float)iMax->GetRendHeight());

	if (cs.isOrtho)
	{
		float zn = cs.nearRange;
		float zf = cs.farRange;
		if (zn < c_SmallEpsilon) zn = 1.0f;
		float vVol = 2.0f * cs.tdist * tan( cs.fov * 0.5f );
        pCamera = CreateOrthoCamera( vVol, aspect, zn, zf );
	}
	else
	{
		float zn = cs.nearRange;
		float zf = cs.farRange;
		if (zn < c_SmallEpsilon) zn = 1.0f;
        pCamera = CreatePerspCamera( cs.fov, aspect, zn, zf );
	}

	return NULL;
} // maxExporter::GetCameraNode

TransformNode* maxExporter::GetLocatorNode( INode* node )
{
	Locator* pLoc = new Locator();
	return pLoc;
} // maxExporter::GetLocatorNode

TransformNode* maxExporter::GetTransformNode( INode* node )
{
	TransformNode* pNode = new TransformNode();
	return pNode;
} // maxExporter::GetTransformNode

TransformNode* maxExporter::GetLightNode( INode* node )
{
	LightSource* pLight = new LightSource();
	LightObject* lObj = (LightObject*)curObject;
	Interval validIvl;
	LightState ls;

	if (lObj->EvalLightState( curTime, validIvl, &ls ) != REF_SUCCEED)
	{
		Msg( "Error processing light <%s>", node->GetName() );
		return NULL;
	}

	if (ls.type == OMNI_LGT)
	{
		pLight = new PointLight();

	}
	else if (ls.type == AMBIENT_LGT)
	{
		return NULL;
	}
	else if (ls.type == DIRECT_LGT)
	{
		pLight = new DirectionalLight();
	}
	else if (ls.type == SPOT_LGT)
	{
		SpotLight* pSpotLight = new SpotLight();
		
		float innerCone = DegToRad( ls.hotsize );
		float outerCone = innerCone + DegToRad( ls.fallsize );
		
		pSpotLight->SetInnerCone( innerCone );
		pSpotLight->SetOuterCone( outerCone );
		pSpotLight->SetRange( ls.attenEnd );

		pLight = pSpotLight;
	}
	else
	{
		return NULL;
	}

	pLight->SetDiffuse( ColorToDWORD( ls.color ) );
	pLight->SetAmbient( 0xFF555555 );

	return pLight;
} // maxExporter::GetLightNode

TransformNode* maxExporter::GetBoneNode( INode* node )
{
	Bone* pLoc = new Bone();	
	return pLoc;
} // maxExporter::GetBoneNode

TransformNode* maxExporter::GetMeshNode( INode* node )
{
	if (!node->Renderable())
	{
		return new TransformNode();
	}

	Matrix3 nodeTM = node->GetNodeTM( curTime );
	Matrix3 nodeTMAfterWSM = node->GetObjTMAfterWSM( curTime );
	Matrix3 mOffs = nodeTMAfterWSM * Inverse( nodeTM );

	bHaveWeights = InitNodeSkinning( node );
	if (bHaveWeights) Msg( "SNode has skinning information." );

	bool hasMtl = ProcessNodeShaders( node );
	ResetGeometryStorage();

	//  triangulate object
	int			deleteIt	= FALSE;
	TriObject	*tri		= 0;
	if (curObject->CanConvertToType( Class_ID( TRIOBJ_CLASS_ID, 0 ) )) 
	{ 
		tri = (TriObject*) curObject->ConvertToType( curTime, Class_ID(TRIOBJ_CLASS_ID, 0));
	}
	if (curObject != tri) deleteIt = TRUE;
	if (!tri) 
	{
		Msg( "Could not triangulate mesh in node. SNode was skipped" );
		return NULL;
	}

	if (!node->IsNodeHidden() && node->Renderable())
	{
		//  find vertex winding direction in polygon (we need CW)
		BOOL negScale;
        Matrix3 objTM = node->GetObjectTM( curTime );
		if (DotProd( CrossProd( objTM.GetRow(0), objTM.GetRow(1) ), objTM.GetRow(2) ) < 0.0)
		{
			negScale = 0;
		}
		else
		{
			negScale = 1;
		}

		int vx[3];
		if (negScale) 
		{
			vx[0] = 2; vx[1] = 1; vx[2] = 0; 
		} 
		else 
		{ 
			vx[0] = 0; vx[1] = 1; vx[2] = 2; 
		}

		Mesh& mesh	= tri->GetMesh();

		Tab<VNormal>	vnorms;
		if (bExportNormals)
		{
			ComputeVertexNormals( mesh, vnorms );
		}

		bool hasUV  = (mesh.numTVerts != 0);
		bool hasClr = (mesh.numCVerts != 0);

		//  some cosmetics
		BOOL res = mesh.RemoveDegenerateFaces();
		if (res) Msg( "Degenerate faces were found. Fixed." );
		res = mesh.RemoveIllegalFaces();
		if (res) Msg( "Degenerate indices were found. Fixed." );

		int numPri	= mesh.getNumFaces();
		int numVert = mesh.numVerts;

		haveBlendedVertices = false;
		if (bHaveWeights)
		{
			curSkin = NewSkin();
		}

		//  loop on polys, 'splitting' vertices, if needed, by tex coords, material, smoothing 
		for (int i = 0; i < numPri; i++)
		{
			Face& face	= mesh.faces[i];

			int vidx[3];
			for (int j = 0; j < 3; j++)
			{
				GVertex gv;
				if (!hasMtl)
				{
					gv.matID = NO_MATERIAL;
				}
				else
				{
                    int nShaders = GetNNodeShaders() > 0 ? GetNNodeShaders() : 1;
					gv.matID = GetNodeShaderID( face.getMatID() % nShaders );
				}

				gv.smGroup	= -1;
				gv.objID	= 0;

				int vIdx	= face.v[vx[j]];
				
				Point3 pt	= mesh.verts[vIdx];
				pt = mOffs.PointTransform( pt );
				
				Vector4D vpt( pt.x, pt.y, pt.z, 1.0f );
				vpt.mul( flipTM );				
				gv.x		= vpt.x;
				gv.y		= vpt.y;
				gv.z		= vpt.z;
				
				if (m_pPhysique)
				{
					FetchPhysiqueVertex( gv, vIdx );
				}
                else if (m_pSkin)
                {
                    FetchSkinVertex( gv, vIdx );
                }

				//  it is assuming that there is no hard edges in the object 
				//  that is skinned by Physique. Maybe it is incorrect.
				if (!bIgnoreSmoothing && !haveBlendedVertices && !bHaveWeights) 
				{
					gv.smGroup = face.smGroup;
				}
				else 
				{
					gv.smGroup = 0xFFFFFFFF;
				}

				//  find normal in vertex taking into account if the edge 
				//  is soft or hard
				if (bExportNormals)
				{
					Point3 norm;
					norm = vnorms[vIdx].GetNormal( face.smGroup );

					Vector4D vpt( norm.x, norm.y, norm.z, 1.0f );
					vpt.mul( flipTM );
					gv.nx		= vpt.x;
					gv.ny		= vpt.y;
					gv.nz		= vpt.z;
				}

				if (bExportColors && hasClr)	
				{
					const VertColor& vcol = mesh.vertCol[mesh.vcFace[i].t[vx[j]]];
					Color col;
					col.r = vcol.x; col.g = vcol.y; col.b = vcol.z;
					gv.color = ColorToDWORD( col );
				}

				if (!bNoTex && hasUV)	
				{
					const Point3& texCoord = mesh.tVerts[mesh.tvFace[i].t[vx[j]]];
					gv.u = texCoord.x;
					gv.v = 1.0f - texCoord.y;

					//  second texture coordinate channel
					if (mesh.getNumMaps() > 1 && mesh.mapSupport( 2 ))
					{
						UVVert *mv = mesh.mapVerts( 2 );
						TVFace *mf = mesh.mapFaces( 2 );
						if (mv && mf)
						{
							const Point3& tc2 = mv[mf[i].t[vx[j]]];
							gv.u2 = tc2.x;
							gv.v2 = 1.0f - tc2.y;
						}
					}
				}

				//  new vertex index
				vidx[j] = vertHash.add( gv );
			}
			AddPolygon( vidx[0], vidx[1], vidx[2] );
		}
	}

	if (deleteIt) delete tri;

	TransformNode* pMeshNode = CreateMeshNode();
	
	//  assign skinning infos
	SNode::Iterator it( pMeshNode );
	int nSkins = 0;
	while (it)
	{
		SNode* pNode = (SNode*) *it;
		if (!strcmp( pNode->GetClassName(), "Skin" ))
		{
			Skin* pGeom = (Skin*) pNode;
			if (nSkins > 0) curSkin = NewSkin( curSkin );
			curSkin->pGeom = pGeom;
			nSkins++;
		}
		++it;
	}

	return pMeshNode;
} // maxExporter::GetMeshNode

SNode* maxExporter::GetPRSAnimation( INode* node )
{	
	PRSAnimation* pAnm = new PRSAnimation();
	Interval range = iMax->GetAnimRange();

	TimeValue samplingInterval = SecToTicks( m_SamplingInterval / 1000.0f );

	for (TimeValue t = range.Start(); t <= range.End(); t += samplingInterval)
	{
		if (t > range.End()) t = range.End();

		Matrix3 m = ConvertMatrix( GetNodeTransform( node, &t ) );
		AffineParts ap;
		decomp_affine( m, &ap );
		
		float cTime = TimeToMs( t );
		pAnm->GetPosXAnimation()->AddKey( cTime, ap.t.x );
		pAnm->GetPosYAnimation()->AddKey( cTime, ap.t.y );
		pAnm->GetPosZAnimation()->AddKey( cTime, ap.t.z );
		
		Matrix3 _rotM;
		_rotM.SetRotate( ap.q );
		Matrix4D rotM = ConvertMatrix( _rotM );
		Quaternion quat;
		quat.FromMatrix( rotM );
		pAnm->GetRotAnimation()->AddKey( cTime, quat );
		
		pAnm->GetScaleXAnimation()->AddKey( cTime, ap.k.x );
		pAnm->GetScaleYAnimation()->AddKey( cTime, ap.k.y );
		pAnm->GetScaleZAnimation()->AddKey( cTime, ap.k.z );
	}
	
	pAnm->GetRotAnimation()->Normalize();
	pAnm->GetRotAnimation()->CorrectOrientation();
	if (bReduceKeys) pAnm->ReduceKeys( m_ScaleTolerance, m_RotTolerance, m_PosTolerance );

	char anmName[256];
	sprintf( anmName, "%s", node->GetName() );
	pAnm->SetName( anmName );

	return pAnm;
} // maxExporter::GetPRSAnimation

UVAnimation* maxExporter::GetUVAnimation( Texmap* pTexMap )
{
	if (!pTexMap) return NULL;
	//StdUVGen* pUVgen = pTexMap->GetUVGen();
	//if (!pUVgen) return NULL;
	UVAnimation* pAnm = new UVAnimation();
	/*TimeValue startTime = 0;
	TimeValue endTime	= 0;
	TimeValue timeStep;
	startTime	= iMax->GetAnimRange().Start();
	endTime		= iMax->GetAnimRange().End();
	for (TimeValue cTime = startTime; cTime <= endTime; cTime += timeStep)
	{
		Matr3x4		matr;
		matr[0]	= 1.0f; 
		matr[1] = 0.0f; 
		matr[2] = 0.0f;
		
		matr[3]	= 0.0f; 
		matr[4] = 1.0f; 
		matr[5] = 0.0f;
		
		matr[6]	= uvgen->GetUOffs( cTime ); 
		matr[7]= -uvgen->GetVOffs( cTime ); 
		matr[8]= 1.0f;

		matr[9]		= 0.0f;
		matr[10]	= 0.0f;
		matr[11]	= 0.0f;

		cha.addFrame( &matr );
	}*/
	return pAnm;
} // maxExporter::GetUVAnimation

void maxExporter::ProcessAnimation()
{
	if (!bExportAnimation) return;

	Interval range = iMax->GetAnimRange();

	TimeValue samplingInterval = SecToTicks( m_SamplingInterval / 1000.0f );

	for (TimeValue t = range.Start(); t <= range.End(); t += samplingInterval)
	{
		if (t > range.End()) t = range.End();
	}
} // maxExporter::ProcessAnimation
