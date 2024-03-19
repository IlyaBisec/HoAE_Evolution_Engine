/*****************************************************************************/
/*	File:	uiSceneGraphBrowser.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07-07-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgFont.h"
#include "kPropertyMap.h"
#include "kInput.h"
#include "uiControl.h"
#include "sgPipeline.h"
#include "sgSkybox.h"
#include "sndSoundTrack.h"
#include "sgCursor.h"
#include "sgApplication.h"
#include "sgDeviceSettings.h"
#include "sgHardwareCaps.h"
#include "sgDecal.h"

#include "sgAnimBlend.h"
#include "sgVSPS.h"
#include "rsDeviceStates.h"
#include "sgStateBlock.h"
#include "sgBillboardGroup.h"
#include "sgParticleSystem.h"

#include "uiNodeTree.h"
#include "uiObjectInspector.h"
#include "uiSceneGraphBrowser.h"
#include "uiTransformTools.h"

#include "kSystemDialogs.h"
#include "kXMLParser.h"
#include "IMediaManager.h"

BEGIN_NAMESPACE(sg)

void DumpToH3()
{
	FILE* fp = fopen( "d:\\dumps\\h3.lvlist.txt", "wt" );
	if (!fp) return;

	Node::Iterator it( sg::Root::instance() );
	while (it)
	{	
		Node* pNode = it;

		const char* group = "node";
		if (pNode->IsA<Group>()			) group = "group";
		if (pNode->IsA<TransformNode>()	) group = "transform";
		if (pNode->IsA<Geometry>()		) group = "geometry";
		if (pNode->IsA<Texture>()		) group = "texture";
		if (pNode->IsA<Control>()		) group = "control";
		if (pNode->IsA<Light>()			) group = "light";
		if (pNode->IsA<Material>()		) group = "material";
		if (pNode->IsA<Controller>()	) group = "controller";
		if (pNode->IsA<BaseCamera>()	) group = "camera";
		if (pNode->IsA<DeviceStateSet>()) group = "dss";

		int depth = it.GetDepth();
		fprintf( fp, "%d %s %d %s\x0A", depth, pNode->GetName(), depth - 1, group );	
		++it;
	}
	fclose( fp );
}

/*****************************************************************************/
/*	SceneGraphBrowser::SGBrowserNode implementation	
/*****************************************************************************/

int DetNodeGlyph( Node* pNode )
{
	if (!pNode) return 0;

	if (pNode->HasFn( "TERC"			  )) return 55;
	if (pNode->HasFn( "TERR"			  )) return 56;
	if (pNode->HasFn( "LENS"			  )) return 57;
	if (pNode->HasFn( "STAT"			  )) return 58;
	if (pNode->HasFn( "GPFR"			  )) return 64;
	if (pNode->HasFn( "CPUN"			  )) return 65;

	//  physics
	if (pNode->HasFn( "RIGB"			  )) return 89;
	if (pNode->HasFn( "COLB"			  )) return 80;
	if (pNode->HasFn( "COLS"			  )) return 84;
	if (pNode->HasFn( "COLC"			  )) return 81;
	if (pNode->HasFn( "CLCC"			  )) return 81;
	if (pNode->HasFn( "CLPL"			  )) return 83;
	if (pNode->HasFn( "CLPR"			  )) return 82;

	if (pNode->HasFn( "JBSO"			  )) return 86;
	if (pNode->HasFn( "JHIN"			  )) return 87;
	if (pNode->HasFn( "JAMO"			  )) return 94;
	if (pNode->HasFn( "JUNI"			  )) return 92;
	if (pNode->HasFn( "JHI2"			  )) return 93;
	if (pNode->HasFn( "JSLI"			  )) return 91;


	if (pNode->HasFn( "PHYJ"			  )) return 85;
	if (pNode->HasFn( "ARTJ"			  )) return 88;
	if (pNode->HasFn( "PHJC"			  )) return 85;
	if (pNode->HasFn( "CLSP"			  )) return 82;

	if (pNode->IsA<ParticleSystem		>()) return 13;
	if (pNode->IsA<ParticleEmitter	>()) return 91;
	if (pNode->IsA<ParticleCluster	>()) return 89;
	if (pNode->IsA<ParticleAffector   >()) return 90;

	if (pNode->IsA<ModelFile			>()) return 88;
	if (pNode->IsA<AnimationFile		>()) return 87;
	if (pNode->IsA<StateBlock			>()) return 10;
	if (pNode->IsA<RenderStateBlock	>()) return 86;
	if (pNode->IsA<TextureStateBlock  >()) return 85;

	if (pNode->IsA<ModelManager		>()) return 79;
	if (pNode->IsA<VertexShaderManager>()) return 80;
	if (pNode->IsA<StateBlockManager  >()) return 81;
	if (pNode->IsA<PixelShaderManager >()) return 82;
	if (pNode->IsA<AnimationManager	>()) return 83;
	if (pNode->IsA<TextureManager		>()) return 84;

	if (pNode->IsA<Locator			>()) return 51;
	if (pNode->IsA<PixelShader		>()) return 77;
	if (pNode->IsA<PSConstBlock		>()) return 78;
	if (pNode->IsA<VertexShader		>()) return 75;
	if (pNode->IsA<VSConstBlock		>()) return 76;

	if (pNode->IsA<DiskFolder			>()) return 73;
	if (pNode->IsA<Font				>()) return 47;
	if (pNode->IsA<TextureMatrix		>()) return 62;
	if (pNode->IsA<GlyphSet			>()) return 61;
	if (pNode->IsA<DecalManager		>()) return 54;
	if (pNode->IsA<PointLight			>()) return 2;
	if (pNode->IsA<DirectionalLight	>()) return 70;
	if (pNode->IsA<SpotLight			>()) return 70;
	if (pNode->IsA<Light				>()) return 20;
	if (pNode->IsA<ZBias				>()) return 33;
	if (pNode->IsA<Material			>()) return 3;
	if (pNode->IsA<Geometry			>()) return 4;
	if (pNode->IsA<Texture			>()) return 5;
	if (pNode->IsA<Canvas			>()) return 8;
	if (pNode->IsA<RenderTarget		>()) return 9;
	if (pNode->IsA<DeviceStateSet		>()) return 10;
	if (pNode->IsA<Root				>()) return 22;
	if (pNode->IsA<Bone				>()) return 31;
	if (pNode->IsA<Application		>()) return 32;
	if (pNode->IsA<DeviceSettings		>()) return 40;
	if (pNode->IsA<SoundTrack			>()) return 35;
	if (pNode->IsA<Pipeline			>()) return 72;
	if (pNode->IsA<PipeSection		>()) return 74;
	if (pNode->IsA<PipeData			>()) return 26; 
	if (pNode->IsA<Skybox				>()) return 15;
	if (pNode->IsA<SystemCursor		>()) return 45;
	if (pNode->IsA<HardwareCursor		>()) return 44;
	if (pNode->IsA<FramerateCursor	>()) return 46;
	if (pNode->IsA<Switch				>()) return 68;
	if (pNode->IsA<AnimationBlock		>()) return 49;
	if (pNode->IsA<PerspCamera		>()) return 1;
	if (pNode->IsA<OrthoCamera		>()) return 50;
	if (pNode->IsA<BaseCamera			>()) return 1;
	if (pNode->IsA<AppWindow			>()) return 52;
	if (pNode->IsA<HardwareCaps		>()) return 53;
	if (pNode->IsA<Animation			>()) return 6;
	if (pNode->IsA<Controller			>()) return 28;
	if (pNode->IsA<TransformNode		>()) return 63;
	if (pNode->IsA<Group				>()) return 71;

	return 17;	
}

/*****************************************************************************/
/*	SceneGraphBrowser implementation	
/*****************************************************************************/
SceneGraphBrowser::SceneGraphBrowser()
{
	m_pInspector	= NULL;
	m_pBindNode		= NULL;
}

void SceneGraphBrowser::Render()
{
	if (m_pInspector && !m_pInspector->IsInvisible())
	{
		Node* pNode = GetSelectedNode();
		if (pNode != m_pBindNode)
		{
			m_pInspector->BindNode( pNode );
			m_pBindNode = pNode;
		}
	}
	Parent::Render();
} // SceneGraphBrowser::Render


bool SceneGraphBrowser::OnChar( DWORD charCode, DWORD flags )
{
	if (charCode == ' ')
	{
		SetInvisible( !IsInvisible() );
		if (!IsInvisible())
		{
			SetExtents( IRS->GetViewPort() );
		}
	}
	return false;
} // SceneGraphBrowser::OnChar

bool SceneGraphBrowser::OnKeyDown( DWORD keyCode, DWORD flags )
{
	if (IsInvisible()) return false;


	if (GetKeyState( '1' ) < 0 && GetKeyState( '2' ) < 0 && GetKeyState( '3' ) < 0)
	{
		NodePool::instance().Dump();
		return false;
	}

	if (keyCode == VK_DELETE)
	{
		Node* pSel = GetSelectedNode();	
		if (!pSel) return false;
		Node* pParent = pSel->GetParent();	
		if (!pParent) return false;
		//  add ref in case we are deleting ourselves
		AddRef();
		pParent->RemoveChild( pSel );
		SelectNode( pParent );
		Release();
		return true;
	}

	if (keyCode == 'A' && GetKeyState( VK_CONTROL ) < 0)
	{
		if (!m_pInspector)
		{
			m_pInspector = Root::instance()->FindChild<ObjectInspector>( "ObjectInspector" );
			if (!m_pInspector) m_pInspector = AddChild<ObjectInspector>( "ObjectInspector" );
		}
		m_pInspector->SetInvisible( !m_pInspector->IsInvisible() );
		return false;
	}

	if (keyCode == VK_TAB)
	{
		Group* pScene = Root::instance()->FindChild<Group>( "Scene" );
		Group* pFrame = Root::instance()->FindChild<Group>( "Frame" );

		if (pScene) pScene->SetInvisible( !pScene->IsInvisible() );
		if (pFrame) pFrame->SetInvisible( !pFrame->IsInvisible() );
	}

	if (keyCode == 'Q' && GetKeyState( VK_CONTROL ) < 0)
	{
		IRS->RecompileAllShaders();
	}

	if (keyCode == 'S' && GetKeyState( VK_CONTROL ) < 0)
	{
		const char* fileName = NULL;
		const char* fileExt = NULL;

		SaveFileDialog dlg;
		dlg.AddFilter( "Binary Model Files", "*.c2m" );
		dlg.AddFilter( "XML Model Files", "*.x2m" );
		dlg.SetDefaultExtension( "c2m" );
		static char lpstrFile[_MAX_PATH];
		if (dlg.Show())
		{
			fileName = dlg.GetFilePath();
		}
		
		if (strstr( fileName, "x2m" ))
		{
			Node* pRoot = GetSelectedNode();
			if (pRoot)
			{
				FOutStream os( fileName );
				char drive	[_MAX_DRIVE	];
				char dir	[_MAX_DIR	];
				char file	[_MAX_PATH	];
				char ext	[_MAX_EXT	];

				_splitpath( fileName, drive, dir, file, ext );
				XMLNode* pXML = pRoot->ToXML();
				pXML->Write( os );
			}
		}
		else if (fileName)
		{
			Node* pRoot = GetSelectedNode();
			if (pRoot)
			{
				FOutStream os( fileName );
				char drive	[_MAX_DRIVE	];
				char dir	[_MAX_DIR	];
				char file	[_MAX_PATH	];
				char ext	[_MAX_EXT	];

				_splitpath( fileName, drive, dir, file, ext );

				pRoot->SerializeSubtree( os );
			}
		}
	}

	if (keyCode == 'L')
	{
		const char* fileName = NULL;

		bool bCurNode = (GetKeyState( VK_CONTROL ) < 0);

		OpenFileDialog dlg;
		dlg.AddFilter( "Binary Model Files", "*.c2m" );
		dlg.AddFilter( "XML Model Files", "*.x2m" );
		dlg.SetDefaultExtension( "c2m" );
		static char lpstrFile[_MAX_PATH];
		if (dlg.Show())
		{
			fileName = dlg.GetFilePath();
			if (strstr( fileName, "x2m" ))
			{
				FInStream is( fileName );
				Node* pHostNode = sg::Root::instance()->FindChild<Group>( "Scene" );
				Node* pRoot = GetSelectedNode();
				if (pRoot && bCurNode) 
				{
					pHostNode = pRoot;
				}

				if (!pHostNode) return false;
				XMLNode xml( is );
				Node* pModel = Node::CreateSubtree( &xml );
				if (!pModel) Log.Warning( "Could not parse model: %s", fileName );
				pHostNode->AddChild( pModel );
			}
			else if (fileName)
			{
				FInStream os( fileName );
				Node* pHostNode = sg::Root::instance()->FindChild<Group>( "Scene" );
				Node* pRoot = GetSelectedNode();
				if (pRoot && bCurNode) 
				{
					pHostNode = pRoot;
				}

				if (!pHostNode) return false;
				Node* pModel = Node::UnserializeSubtree( os );
				if (!pModel) Log.Warning( "Could not load model: %s", fileName );
				pHostNode->AddChild( pModel );
			}
		}
	}
	
	if (keyCode == 'O' && GetKeyState( VK_CONTROL ) < 0)
	{
		FlattenStaticHierarchy( GetSelectedNode() );
	}

	if (GetKeyState( VK_RETURN ) < 0)
	{
		Pipeline* pPipe = (Pipeline*)GetSelectedNode();
		if (pPipe && pPipe->IsA<Pipeline>())
		{
			pPipe->Process();
		}
	}

	if (keyCode == 'A')
	{
		const char* fileName = NULL;

		OpenFileDialog dlg;
		dlg.AddFilter( "Model Files", "*.c2m" );
		dlg.SetDefaultExtension( "c2m" );
		static char lpstrFile[_MAX_PATH];
		if (dlg.Show())
		{
			fileName = dlg.GetFilePath();
		}

		if (fileName)
		{
			FInStream os( fileName );
			Node* pRoot = GetSelectedNode();
			if (!pRoot) return false;

			Node* pAnim = Node::UnserializeSubtree( os );
			Node* pScene = sg::Root::instance()->FindChild<Group>( "Scene" );
			pScene->AddChild( pAnim );
			pAnim->AttachSubtree( pRoot );
			if (pAnim->IsA<sg::Animation>())
			{
				Animation* pAnimation = (Animation*)pAnim;
				pAnimation->Loop();
				pAnimation->Play();
			}
		}
	}


	/*if (keyCode == VK_UP && GetKeyState( VK_CONTROL ) < 0)
	{
		if (!m_pSelectedNode) return false;
		SGBrowserNode* pSGParent = (SGBrowserNode*)m_pSelectedNode->m_pParent;
		if (!pSGParent) return false;
		
		Node* pNode = ((SGBrowserNode*)m_pSelectedNode)->m_pNode;
		if (!pNode) return false;
		Node* pParent = pNode->GetParent();
		int cIdx = pSGParent->GetChildIdx( m_pSelectedNode );
		if (pParent)
		{
			int nIdx = max( cIdx - 1, 0 );
			pParent->SwapChildren( cIdx, nIdx );
			m_pSelectedNode = pSGParent->GetChild( nIdx );
			UpdateGraph();
		}
	}

	if (keyCode == VK_DOWN && GetKeyState( VK_CONTROL ) < 0)
	{
		if (!m_pSelectedNode) return false;
		SGBrowserNode* pSGParent = (SGBrowserNode*)m_pSelectedNode->m_pParent;
		if (!pSGParent) return false;

		Node* pNode = ((SGBrowserNode*)m_pSelectedNode)->m_pNode;
		if (!pNode) return false;
		Node* pParent = pNode->GetParent();
		int cIdx = pSGParent->GetChildIdx( m_pSelectedNode );
		if (pParent)
		{
			int nIdx = max( cIdx + 1, 0 );
			pParent->SwapChildren( cIdx, nIdx );
			m_pSelectedNode = pSGParent->GetChild( nIdx );
			UpdateGraph();
		}
	}*/

	if (keyCode == VK_PAUSE)
	{
		Node* pNode = GetSelectedNode();
		if (pNode)
		{
			if (pNode->IsA<Animation>())
			{
				Animation* pAnimation = (Animation*)pNode;
				pAnimation->Pause();
			}
		}
	}

	if (keyCode == 'T')
	{
		Node* pNode = GetSelectedNode();
		if (pNode)
		{
			Iterator it( pNode, Geometry::FnFilter );
			while (it)
			{
				Geometry* pGeom = (Geometry*)(Node*)it;
				if (!pGeom->IsInvisible()) 
				{
					pGeom->GetPrimitive().setIsStatic( !pGeom->GetPrimitive().isStatic() );
				}
				++it;
			}
		}
	}
	
	return false;
} // SceneGraphBrowser::OnKeyDown
END_NAMESPACE(sg)
