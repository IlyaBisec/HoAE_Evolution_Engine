/*****************************************************************************/
/*	File:	c2mExporter.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07.11.2002
/*****************************************************************************/
#pragma warning ( disable : 4786 ) 
#pragma warning ( disable : 4530 )

#include "resource.h"
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include "assert.h"

#include <vector>
#include <string>

#include "gmDefines.h"
#include "mMath2D.h"
#include "mMath3D.h"
#include "mGeom3D.h"
#include "mQuaternion.h"
#include "mAlgo.h"
#include "kArray.hpp"
#include "kLog.h"
#include "kResFile.h"
#include "kIO.h"
#include "kString.h"
#include "kTemplates.hpp"
#include "kTypeTraits.h"
#include "kPropertyMap.h"
#include "kUtilities.h"
#include "IRenderSystem.h"
#include "IPictureManager.h"
#include "gpMesh.h"
#include "rsVertex.h"
#include "sg.h"
#include "sgController.h"
#include "sgAnimationBlock.h"
#include "sgPRSAnimation.h"

#include "c2mExporter.h"
#include "vCamera.h"
#include "vSkin.h"


#define isslash(A)	((A)=='/'||(A)=='\\')

IRenderSystem*      IRS = NULL;
IPictureManager*    IPM = NULL;

IReflected* CreateOrthoCamera( float vVol, float aspect, float zn, float zf )
{
    OrthoCamera* pCamera = new OrthoCamera();
    pCamera->SetViewVolume( vVol, zn, zf, aspect );
    return pCamera;
} // CreateOrthoCamera

IReflected* CreatePerspCamera( float fovx, float aspect, float zn, float zf )
{
    PerspCamera* pCamera = new PerspCamera();
    pCamera->SetFOVx( fovx, zn, zf, aspect );
    return pCamera;
} // CreatePerspCamera

/*****************************************************************************/
/*	GVertex implementation
/*****************************************************************************/
DWORD GVertex::PackBoneRef( int nBones )
{
	DWORD res = 0;
	if (nBones >= 1) res |= ( DWORD( boneRef[0] )		 & 0x000000FF);
	if (nBones >= 2) res |= ((DWORD( boneRef[1] ) << 8)  & 0x0000FF00);
	if (nBones >= 3) res |= ((DWORD( boneRef[2] ) << 16) & 0x00FF0000);
	if (nBones >= 4) res |= ((DWORD( boneRef[3] ) << 24) & 0xFF000000);

	return res;
} // GVertex::PackBoneRef

bool GVertex::AddWeight( int idx, float w )
{
	for (int i = 0; i < nWeights; i++)
	{
		if (boneRef[i] == idx) 
		{
			weight[i] += w;
			return true;
		}
	}
	if (nWeights < c_MaxBonesPerVertex)
	{
		weight[nWeights] = w;
		boneRef[nWeights] = idx;
		nWeights++;
		return true;
	}

	return false;
} // GVertex::AddWeight

void GVertex::LeaveBiggerWeight( int idx, float w )
{
	int maxV = 0;
	for (int i = 0; i < nWeights; i++)
	{
		if (weight[maxV] < weight[i]) maxV = i;
		if (weight[i] < w) 
		{
			weight[i]  += w;
			boneRef[i] = idx;
			return;
		}
	}

	//  if all weights are bigger than added weight, it is added to
	//  the biggest weight
	weight[maxV] += w;
} // GVertex::LeaveBiggerWeight

void* GVertex::GetVertex( VertexFormat vf )
{
	static const int c_MaxVertexStride = 256;
	static BYTE vBuf[c_MaxVertexStride];
	switch (vf)
	{
	case vfVertex2t:
		{
			Vertex2t* pv = new(vBuf) Vertex2t();
			pv->x = x; pv->y = y; pv->z = z;
			pv->diffuse = color;
			pv->u = u; pv->v = v;
			break;
		}
	case vfVertexN:
		{
			VertexN* pv = new(vBuf) VertexN();
			pv->x = x; pv->y = y; pv->z = z;
			pv->nx = nx; pv->ny = ny; pv->nz = nz;
			pv->u = u; pv->v = v;
			break;
		}
	case vfVertexN2T:
		{
			VertexN2T* pv = new(vBuf) VertexN2T();
			pv->x = x; pv->y = y; pv->z = z;
			pv->nx = nx; pv->ny = ny; pv->nz = nz;
			pv->u = u; pv->v = v;
			//pv->u2 = u2; pv->v2 = v2;
			//pv->diffuse = color;
			break;
		}
	case vfVertex1W:
		{
			Vertex1W* pv = new(vBuf) Vertex1W();
			pv->pos.x = x; pv->pos.y = y; pv->pos.z = z;
			pv->normal.x = nx; pv->normal.y = ny; pv->normal.z = nz;
			pv->u = u; pv->v = v;
			//pv->u2 = u2; pv->v2 = v2;
			//pv->diffuse = color;

			pv->m = boneRef[0];
			break;
		}
	case vfVertex2W:
		{
			Vertex2W* pv = new(vBuf) Vertex2W();
			pv->pos.x = x; pv->pos.y = y; pv->pos.z = z;
			pv->normal.x = nx; pv->normal.y = ny; pv->normal.z = nz;
			pv->u = u; pv->v = v;
			//pv->u2 = u2; pv->v2 = v2;
			//pv->diffuse = color;

			pv->m0    = boneRef[0];
            pv->m1    = boneRef[1];
			pv->w     = weight[0];

			break;
		}
	case vfVertex3W:
		{
			Vertex3W* pv = new(vBuf) Vertex3W();
			pv->pos.x = x; pv->pos.y = y; pv->pos.z = z;
			pv->normal.x = nx; pv->normal.y = ny; pv->normal.z = nz;
			pv->u = u; pv->v = v;
			//pv->u2 = u2; pv->v2 = v2;
			//pv->diffuse = color;

            pv->m0    = boneRef[0];
            pv->m1    = boneRef[1];
            pv->m2    = boneRef[2];
            pv->w0    = weight[0];
            pv->w1    = weight[1];
			break;
		}
	case vfVertex4W:
		{
			Vertex4W* pv = new(vBuf) Vertex4W();
			pv->pos.x = x; pv->pos.y = y; pv->pos.z = z;
			pv->normal.x = nx; pv->normal.y = ny; pv->normal.z = nz;
			pv->u = u; pv->v = v;
			//pv->u2 = u2; pv->v2 = v2;
			//pv->diffuse = color;

            pv->m0    = boneRef[0];
            pv->m1    = boneRef[1];
            pv->m2    = boneRef[2];
            pv->m3    = boneRef[3];
            pv->w0    = weight[0];
            pv->w1    = weight[1];
            pv->w2    = weight[2];
			break;
		}
	default:
		assert( false );
		return NULL;
	}

	return vBuf;
} // GVertex::GetVertex

/*****************************************************************************/
/*	c2mExporter implementation
/*****************************************************************************/
HINSTANCE	c2mExporter::hInstance			= 0;
HWND		c2mExporter::hPanelWnd			= 0;
HWND		c2mExporter::hLogWnd			= 0;	
HWND		c2mExporter::hProgressWnd		= 0;
HWND		c2mExporter::hGoBtn				= 0;
HWND		c2mExporter::hTexPathWnd		= 0;
HWND		c2mExporter::hChPathWnd			= 0;
HWND		c2mExporter::hCopyTexWnd		= 0;

HWND		c2mExporter::hAnimOnly			= 0;
HWND		c2mExporter::hReduceKeys		= 0;
HWND		c2mExporter::hExportAnimWnd		= 0;
HWND		c2mExporter::hCopyTCap			= 0;

HWND		c2mExporter::hAnGroup			= 0;
HWND		c2mExporter::hNormWnd			= 0;
HWND		c2mExporter::hColWnd			= 0;
HWND		c2mExporter::hIgnoreSm			= 0;
HWND		c2mExporter::hSkinning			= 0;
HWND		c2mExporter::h2ndTexPair		= 0;
HWND		c2mExporter::hNoTex				= 0;
HWND		c2mExporter::hSuppressLog		= 0;

HWND		c2mExporter::hOverwriteTextures	= 0;

char		c2mExporter::folderPath[_MAX_PATH];
c2mExporter* c2mExporter::me;
bool		c2mExporter::exportDone;


c2mExporter::c2mExporter()
{
	//  default export settings
	bStaticVertexColors = false;
	bCopyTextures		= true;
	bKeyframe			= true;
	bExportAnimation	= false;
	bAnimationOnly		= false;

	bExportSkeletal		= false;
	bOverwriteTextures	= false;
	bExportNormals		= true;
	bExportColors		= false;
	bIgnoreSmoothing	= false;
	bNoTex				= false;
	bSuppressLog		= false;
	bReduceKeys			= true;
	nKPS				= 2;
	nWeightsPerBone		= 0;
	nIndent				= 0;
	m_pAnimGroup		= NULL;
	m_pModel			= NULL;

	me					= this;

	poly				= 0;
	pluginFileName[0]	= 0;

	m_SamplingInterval	= 1000.0f/25.0f;//40.0f; 

	m_ScaleTolerance	= c_ScaleTolerance;	
	m_RotTolerance		= c_RotTolerance;
	m_PosTolerance		= c_PosTolerance;

	Matrix4D m;
} // c2mExporter::c2Exporter

void c2mExporter::OnOpenExportDlg()
{
	char dir	[_MAX_PATH];
	char drive	[_MAX_PATH];
	char file	[_MAX_PATH];
	char ext	[_MAX_PATH];
	
	_splitpath( modelFileName, drive, dir, file, ext );

    /*
	UpOneDirTreeLevel( dir );
	int len = strlen( dir );
	if (!isslash( dir[len - 1] )) strcat( dir, "/" );
	strcat( dir, "textures" );
    */
    _makepath( texPath, drive, dir, NULL, NULL );

	exportDone = false;
	haveBlendedVertices = false;
} // c2mExporter::OnOpenExportDlg

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::CopySceneTextures
/*	Desc:	Copying scene texture files to the textures directory 
/*---------------------------------------------------------------------------*/
void c2mExporter::CopySceneTextures()
{
	char newPath[_MAX_PATH];
	char texName[_MAX_PATH];
	char drive	[_MAX_DRIVE];
	char dir	[_MAX_DIR];
	char fname	[_MAX_FNAME];
	char ext	[_MAX_EXT];

	if (!bCopyTextures) return;

	BOOL bFailIfExists = bOverwriteTextures ? FALSE : TRUE;

	if (texTable.size() == 0)
	{
		Msg( "There were no textures used in the scene." );
	}

    _splitpath( modelFileName, drive, dir, fname, ext );
    _makepath( newPath, drive, dir, NULL, NULL );
    _chdir( newPath );

	Msg( "Copying textures..." );
	std::vector<const char*>  copied;

	MsgIndent();
	for (int i = 0; i < texTable.size(); i++)
	{
		strcpy( newPath, texPath );
        Texture* pTex = texTable[i];
        if (!pTex) continue;

		_splitpath( pTex->GetName(), drive, dir, fname, ext );
		strcpy( texName, fname ); 
		strcat( texName, ext );

		bool alreadyCopied = false;
		for (int j = 0; j < copied.size(); j++)
		{
			if (!strcmp( copied[j], texName ))
			{
				alreadyCopied = true;
				break;
			}
		}
		
		if (!alreadyCopied) 
		{
			strcat( newPath, texName );
			Msg( "Source: %s", texTable[i]->GetName()	);
			Msg( "Destination:  %s", newPath			);
			CopyFile( texTable[i]->GetName(), newPath, bFailIfExists );
		}

		texTable[i]->SetName( texName );
		copied.push_back( texTable[i]->GetName() );
	}
	MsgUnindent();
} // c2mExporter::CopySceneTextures

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::OptionsDlgProc	
/*	Desc:	options dialog window procedure
/*	Parm:	
/*	Ret:	
/*	Remark:	
/*---------------------------------------------------------------------------*/
BOOL CALLBACK 
c2mExporter::OptionsDlgProc(  HWND hWnd, UINT message,
								WPARAM wParam, LPARAM lParam )
{
	switch(message) {
		case WM_INITDIALOG:
			hPanelWnd		= hWnd;
			hLogWnd			= GetDlgItem( hPanelWnd, IDC_LOG		); assert( hLogWnd		);
			hGoBtn			= GetDlgItem( hPanelWnd, IDEXPORT		); assert( hGoBtn		);
			hProgressWnd	= GetDlgItem( hPanelWnd, IDC_PROGRESS	); assert( hProgressWnd );
			hTexPathWnd		= GetDlgItem( hPanelWnd, IDCTEXPATH		); assert( hTexPathWnd	);
			hCopyTexWnd		= GetDlgItem( hPanelWnd, IDC_COPYTEX	); assert( hCopyTexWnd	);
			hOverwriteTextures 
							= GetDlgItem( hPanelWnd, IDC_OVERWRITEX ); assert( hOverwriteTextures );

			hChPathWnd		= GetDlgItem( hPanelWnd, IDC_TEXPATH	); assert( hChPathWnd	);
			hSuppressLog	= GetDlgItem( hPanelWnd, IDCSUPPRESSLOG	); assert( hSuppressLog	);
			
			//  export animation group box
			hAnimOnly		= GetDlgItem( hPanelWnd, IDCANIMONLY	); assert( hAnimOnly	);
			hReduceKeys		= GetDlgItem( hPanelWnd, IDCREDUCEKEYS	); assert( hReduceKeys	);
			hExportAnimWnd	= GetDlgItem( hPanelWnd, IDCEXPORTANIM	); assert( hExportAnimWnd );

			hAnGroup		= GetDlgItem( hPanelWnd, IDCANGROUP		); assert( hAnGroup		);
			hNormWnd		= GetDlgItem( hPanelWnd, IDCNORMALS		); assert( hNormWnd		);
			hColWnd			= GetDlgItem( hPanelWnd, IDCCOLORS		); assert( hColWnd		);  			

			hIgnoreSm		= GetDlgItem( hPanelWnd, IDCSMOOTH		); assert( hIgnoreSm	);
			hNoTex			= GetDlgItem( hPanelWnd, IDCNOTEX		); assert( hNoTex		);
			hCopyTCap		= GetDlgItem( hPanelWnd, IDCCOPYTCAP	); assert( hCopyTCap	);

			SendMessage( hProgressWnd, PBM_SETRANGE, 0, MAKELPARAM( 0, 100 ) );

			me->Options2Dialog();
			EnableControls();

			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;

		case WM_COMMAND:
		{
			switch (LOWORD( wParam ))
			{
				case IDEXPORT:
				//  start exporting process
				{
					if (!exportDone)
					{
						me->Dialog2Options();
						DisableControls(); 
						me->StartExporting(); 
					}
					else
					//  finish export phase
					{
						EndDialog( hWnd, 0 );
						return TRUE;	
					}
				}
				break;
				
				case IDC_COPYTEX:
				//  pushed checkbox "Do not copy textures"
				{
					me->Dialog2Options();
					EnableControls();
				}
				break;

				case IDC_TEXPATH: //  select directory to store textures
				{
					char* folder = BrowseForFolder();
					if (folder) 
					{
						strcpy( me->texPath, folder );
						SetWindowText( hTexPathWnd, me->texPath );
					}
				}
				case IDCCOLORS:   //  "Export colors" checkbox
				case IDCNORMALS:  //  "Export normals" checkbox
				case IDCSMOOTH:   //  "Ignore smoothing checkbox
				{
					me->Dialog2Options();
				}break;
				
				case IDCKF:
				case IDCSAMPLING:
				//  keyframing mode radio clicked
				{
					me->Dialog2Options();
					me->EnableAnimGroup( true );
				}

				case IDCEXPORTANIM:
				//  "Export animation" checkbox
				{
					me->Dialog2Options();
					me->EnableAnimGroup( me->bExportAnimation );
				}
				break;
			}
		}return TRUE;
	}
	return FALSE;
} // c2mExporter::OptionsDlgProc

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::Msg
/*	Desc:	Shows message in the log window
/*---------------------------------------------------------------------------*/
void c2mExporter::Msg( const char* format, ... )
{
	if (!hPanelWnd) return;
	
	char buffer[1024];
	buffer[0] = 0;
	for (int i = 0; i < nIndent; i++)
	{
		strcat( buffer, "    " );
	}

	va_list argList;
	va_start( argList, format );
	vsprintf( buffer + strlen( buffer ), format, argList );
	va_end( argList );

	SendMessage( hLogWnd, LB_INSERTSTRING, 0, (LPARAM)buffer );
	UpdateWindow( hLogWnd );
} // c2mExporter::Msg

void c2mExporter::MsgIndent()
{
	if (nIndent < c_MaxIndent) nIndent++;
} // c2mExporter::MsgIndent

void c2mExporter::MsgUnindent()
{
	if (nIndent > 0) nIndent--;
} // c2mExporter::MsgUnindent

void c2mExporter::DumpPolys( FILE* fp )
{
	fprintf( fp, "NVert = %d\n", vertHash.numElem() );
	fprintf( fp, "Dumping polygon list...\n" );
	for (int i = 0; i < nPoly; i++)
	{
		fprintf( fp, "%d: %d %d %d\n", i, poly[i].v1, poly[i].v2, poly[i].v3 );
		vertHash.elem( poly[i].v1 ).dump( fp );
		vertHash.elem( poly[i].v2 ).dump( fp );
		vertHash.elem( poly[i].v3 ).dump( fp );
	}
} // c2mExporter::DumpPolys

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::GetPolyClusterSize
/*	Desc:	Returns number of polygons with same attribute value as first has
/*---------------------------------------------------------------------------*/
int	c2mExporter::GetPolyClusterSize( int firstPoly, GSortAttribute attr )
{
	int num = 0;
	if (attr == gsaObject)
	//  we assume that cluster with the same object ID also 
	//  should have identical matIDs for every polygon
	{
		WORD objID = vertHash.elem( poly[firstPoly].v1 ).objID;
		WORD matID = vertHash.elem( poly[firstPoly].v1 ).matID;
		while (firstPoly < nPoly)
		{
			const GVertex& gv = vertHash.elem( poly[firstPoly].v1 );
			if (gv.objID != objID || gv.matID != matID) break;
			firstPoly++;
			num++;
		}		
		return num;
	}
	else if (attr == gsaMaterial)
	{
		WORD matID = vertHash.elem( poly[firstPoly].v1 ).matID;
		while (	firstPoly < nPoly && 
				vertHash.elem( poly[firstPoly++].v1 ).matID == matID) num++;
		return num;
	}
	else return 0;
} // c2mExporter::GetPolyClusterSize

int	c2mExporter::GetTotalPolyNum() const
{
	return nPolyTotal;
}

int	c2mExporter::GetTotalVertNum() const
{
	return nVertTotal;
}

int	c2mExporter::GetTotalTexNum() const
{
	return texTable.size();
}

GShader* c2mExporter::GetNodeShader( int idx )
{
	if (idx < 0 || idx >= nodeShaders.size())
	{
		return NULL;
	}
	return &nodeShaders[idx];
}

int	c2mExporter::GetNodeShaderID( int mtlID )
{
	if (mtlID < 0 || mtlID >= nodeShaders.size()) return NO_MATERIAL;
	return nodeShaders[mtlID].GetID();
}

GShader* c2mExporter::GetNodeShaderByID( int id )
{
	if (id < 0 || id >= nodeShaders.size()) return NULL;
	for (int i = 0; i < nodeShaders.size(); i++)
	{
		if (nodeShaders[i].GetID() == id) return &nodeShaders[i];
	}
	return NULL;
} // c2mExporter::GetNodeShaderByID

int	c2mExporter::GetNNodeShaders() const
{
	return nodeShaders.size();
}

int	c2mExporter::GetNNodeShadersReal() const
{
	return nNodeShadersReal;
}

void c2mExporter::ClearNodeShaders() 
{
	nodeShaders.clear();
	nNodeShadersReal = 0;
}

int	c2mExporter::AddNodeShader( GShader& sh ) 
{
	int i;
	for (i = 0; i < nodeShaders.size(); i++)
	{
		if (nodeShaders[i].equal( sh ))
		{
			sh.SetID( i );
			break;
		}
	}

	if (i == nodeShaders.size())
	{
		sh.SetID( nNodeShadersReal++ );
	}
	nodeShaders.push_back( sh );
	return i;
} // c2mExporter::AddNodeShader

Texture* c2mExporter::AddTextureNode( Texture* node )
{
	for (int i = 0; i < texTable.size(); i++)
	{
		if (node->IsEqual( texTable[i] ))
		{
			return texTable[i];
		}
	}
	Texture* newNode = (Texture*)node->Clone();
	texTable.push_back( newNode );
	return newNode;
} // c2mExporter::AddTextureNode

Shader*	c2mExporter::AddDssNode( Shader* node )
{
	for (int i = 0; i < dssTable.size(); i++)
	{
		if (node->IsEqual( dssTable[i] ))
		{
			return dssTable[i];
		}
	}
	Shader* newNode = (Shader*)node->Clone();
	dssTable.push_back( newNode );
	return newNode;
} // c2mExporter::AddDssNode

SurfaceMaterial* c2mExporter::AddMtlNode( SurfaceMaterial* node )
{
	for (int i = 0; i < mtlTable.size(); i++)
	{
		if (node->IsEqual( mtlTable[i] ))
		{
			return mtlTable[i];
		}
	}
	SurfaceMaterial* newNode = new SurfaceMaterial();
    *newNode = *node;
	char mtlName[c_MaxNameLen];
	sprintf( mtlName, "%s_mtl%d", modelName, mtlTable.size() );	
	if (newNode) 
    {
        newNode->SetName( mtlName );
        mtlTable.push_back( newNode );
    }
	return newNode;
} // c2mExporter::AddMtlNode

TransformNode* c2mExporter::CreateMeshNode()
{
	TransformNode* pNode = new TransformNode();
	
	//  sort polys by material
	//  if there are polys with no material, they will be last, with matID==65535 
	SortPoly( 0, nPoly, gsaMaterial );

	int cPoly	= 0;
	int nVert	= vertHash.numElem();

	if (nPoly == 0) return pNode;

	DWORD* reord = new DWORD[nVert];
	
	static const int c_MaxPolyClusters = 512;
	int clusterSz[c_MaxPolyClusters];
	//  count number of actually used materials
	
    int nClusters = 0;
    while (cPoly < nPoly)
	{
        int cSize = GetPolyClusterSize( cPoly, gsaMaterial );;
		cPoly += cSize;
        const int c_MaxPolyInCluster = 21000;
        while (cSize > c_MaxPolyInCluster)
        {
            clusterSz[nClusters] = c_MaxPolyInCluster;
            cSize -= c_MaxPolyInCluster;
            nClusters++;
        }
        if (cSize > 0)
        {
            clusterSz[nClusters] = cSize;
            nClusters++;
        }
    }
	//  cycle through all materials used in the node
	cPoly = 0;
	for (int i = 0; i < nClusters; i++)
	{
		VertexFormat curNodeVF = GetCurVertexFormat();
		int nCPoly = 0;
		int nCVert = 0;			
		nCPoly = clusterSz[i];

		//  find how much vertices are used in this poly cluster
		memset( reord, 0xFF, nVert * sizeof( DWORD ) );
		for (int j = 0; j < nCPoly; j++)
		{
			Poly& p = poly[j + cPoly];
			if (reord[p.v1] == 0xFFFFFFFF) reord[p.v1] = nCVert++;
			if (reord[p.v2] == 0xFFFFFFFF) reord[p.v2] = nCVert++;
			if (reord[p.v3] == 0xFFFFFFFF) reord[p.v3] = nCVert++;
		}

		Geometry* pGeom = NULL;
		//  create geometry container
		if (nWeightsPerBone > 0)
		{
			pGeom = new Skin();
		}
		else
		{
			pGeom = new Geometry();
		}

		pGeom->Create( nCVert, nCPoly*3, curNodeVF );

		//  extract vertices 
		memset( reord, 0xFF, nVert * sizeof( DWORD ) );
		for (int j = 0; j < nCPoly; j++)
		{
			Poly& p = poly[j + cPoly];
			DWORD v[3];
			v[0] = p.v1; v[1] = p.v2; v[2] = p.v3;
			
			for (int k = 0; k < 3; k++)
			{
				if (reord[v[k]] == 0xFFFFFFFF) 
				{
					int vbS = pGeom->AddVertex( vertHash.elem( v[k] ).GetVertex( curNodeVF ) );
					reord[v[k]] = vbS - 1;
				}
			}
			p.v1 = reord[v[0]]; p.v2 = reord[v[1]]; p.v3 = reord[v[2]];
		}

		//  arrange polys in the cluster to better utilize GPU vertex cache
		SortPoly( cPoly, nCPoly, gsaIdxProximity );

		//  add polys to the geometry
		for (int j = 0; j < nCPoly; j++)
		{
			const Poly& p = poly[j + cPoly];
			pGeom->AddPoly( p.v1, p.v2, p.v3 );
		}

		//  add all geometry attributes to the mesh
		GShader* nodeShader = GetNodeShaderByID( i );
		if (nodeShader)
		{
			for (int j = 0; j < nodeShader->GetNAttributes(); j++)
			{
				pNode->AddInput( nodeShader->GetAttribute( j ) );
			}
		}

		//  assign meaningful name to the geometry node
		char geomName[c_MaxNameLen];
		if (GetNNodeShaders() > 1)
		{
			sprintf( geomName, "%s_subg%d", curNodeName, i );
		}
		else
		{
			sprintf( geomName, "%s_geom", curNodeName );
		}
		pGeom->SetName( geomName );

		//  add to the mesh geometry itself
		pNode->AddChild( pGeom );

		cPoly += nCPoly;
	}

	delete []reord;

	nPolyTotal	+= nPoly;
	nVertTotal  += nVert;

	ResetGeometryStorage();	
	return pNode;
} //  c2mExporter::ProcessGeometry

VertexFormat c2mExporter::GetCurVertexFormat()
{
	VertexFormat vf = vfVertex2t;
	if (bExportNormals) 
	{
		if (nWeightsPerBone == 0)
		{
			if (bExportColors)
			{
				vf = vfVertexN2T;
			}
			else
			{
				vf = vfVertexN;
			}
		}
		else if (nWeightsPerBone == 1)
		{
			vf = vfVertex1W;
		}
		else if (nWeightsPerBone == 2)
		{
			vf = vfVertex2W;
		}
		else if (nWeightsPerBone == 3)
		{
			vf = vfVertex3W;
		}
		else if (nWeightsPerBone == 4)
		{
			vf = vfVertex4W;
		}	
		else
		{
		}
	}
	return vf;
} // c2mExporter::GetCurVertexFormat

c2mExporter* g_Exp = 0;
int __cdecl cmpObject( const void *e1, const void *e2 )
{
	return GVertex::cmpObject( &(g_Exp->vertHash.elem( ((Poly*)e1)->v1 )),
								&(g_Exp->vertHash.elem( ((Poly*)e2)->v1 ) ));
}

int __cdecl cmpMaterial( const void *e1, const void *e2 )
{
	return GVertex::cmpMaterial( &(g_Exp->vertHash.elem( ((Poly*)e1)->v1 )),
								&(g_Exp->vertHash.elem( ((Poly*)e2)->v1 )) );
}

//  compare function for sorting by cache index proximity
int __cdecl cmpIdxProximity( const void *e1, const void *e2 )
{
	const Poly& p1 = *((Poly*)e1);
	const Poly& p2 = *((Poly*)e2);
	
	return (p1.v1 + p1.v2 + p1.v3) - (p2.v1 + p2.v2 + p2.v3);

} // cmpIdxProximity

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::SortPoly
/*	Desc:	Sorts poly array in given range, by given attribute:
/*				gsaObject		- by object polygon belongs to
/*				gsaMaterial		- by polygon material
/*---------------------------------------------------------------------------*/
void c2mExporter::SortPoly( int first, int num, GSortAttribute attr )
{
	g_Exp = this;
	if (attr == gsaObject)
	{
		qsort( poly + first, num, sizeof( Poly ), cmpObject );
	}
	else if (attr == gsaMaterial)
	{
		qsort( poly + first, num, sizeof( Poly ), cmpMaterial );	
	}
	else if (attr == gsaIdxProximity)
	{
		qsort( poly + first, num, sizeof( Poly ), cmpIdxProximity );	
	}
} // c2mExporter::SortPoly

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::ShowExportDialog	
/*	Desc:	Brings export options dialog onto the screen
/*---------------------------------------------------------------------------*/
void c2mExporter::ShowExportDialog()
{
	HWND parent = GetActiveWindow();
	hInstance = GetModuleHandle( pluginFileName );
	DialogBox(	hInstance, 
				MAKEINTRESOURCE(IDD_C2MPANEL), 
				parent, 
				OptionsDlgProc );
	int res = GetLastError();
} // c2mExporter::ShowExportDialog

void c2mExporter::SetProgress( double progrVal )
{
	progress = progrVal;
	if (!hProgressWnd) return;
	int pos = (int)(progress * 100.0f);
	SendMessage( hProgressWnd, PBM_SETPOS, (WPARAM)pos, 0 );
} // c2mExporter::SetProgressPos

void c2mExporter::StepProgressNode()
{
	progress += progressPerNode;
	if (!hProgressWnd) return;
	int pos = (int)(progress * 100.0f);
	SendMessage( hProgressWnd, PBM_SETPOS, (WPARAM)pos, 0 );
}

bool c2mExporter::checkPolySanity( const Poly& p )
{
	int nV = vertHash.numElem();
	bool sane = (p.v1 < nV && p.v2 < nV && p.v3 < nV);
	assert( sane );
	return sane;
}

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::EnableAnimGroup	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void c2mExporter::EnableAnimGroup( bool enable ) 
{
	if (!enable)
	{
		EnableWindow( hAnimOnly,		FALSE );
		EnableWindow( hReduceKeys,	FALSE );
	}
	else
	{
		EnableWindow( hAnimOnly,		TRUE );
		EnableWindow( hReduceKeys,	TRUE );
	}
} // c2mExporter::EnableAnimGroup

bool c2mExporter::IsChecked( HWND hWnd )
{
	return (SendMessage( hWnd, BM_GETCHECK, 0, 0 ) == BST_CHECKED);
} // c2mExporter::IsChecked

void c2mExporter::SetCheck( HWND hWnd, bool checked )
{
	SendMessage(	hWnd, 
					BM_SETCHECK, 
					checked ? (WPARAM)BST_CHECKED : (WPARAM)BST_UNCHECKED, 
					0 );
} // c2mExporter::SetCheck

bool c2mExporter::GetInt( HWND hWnd, int& val )
{
	const int c_Maxlen = 512;
	char str[c_Maxlen];
	GetWindowText( hWnd, str, c_Maxlen );
	if (sscanf( str, "%d", &val ) < 1) return false;
	return true;
} // c2mExporter::GetInt

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::Dialog2Options	
/*	Desc:	Gets export options from dialog window
/*---------------------------------------------------------------------------*/
void c2mExporter::Dialog2Options()
{
	bExportAnimation	= IsChecked( hExportAnimWnd );
	bCopyTextures		= !IsChecked( hCopyTexWnd	);	
	bAnimationOnly		= IsChecked( hAnimOnly		);
	bReduceKeys			= IsChecked( hReduceKeys	);
	bExportNormals		= IsChecked( hNormWnd		);
	bOverwriteTextures	= IsChecked( hOverwriteTextures	);
	bExportColors		= IsChecked( hColWnd		);
	bIgnoreSmoothing	= IsChecked( hIgnoreSm		);
	bNoTex				= IsChecked( hNoTex			);
	bSuppressLog		= IsChecked( hSuppressLog	);

	GetWindowText( hTexPathWnd, me->texPath, _MAX_PATH );
} // c2mExporter::Dialog2Options

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::Options2Dialog
/*	Desc:	Mirrors current export options into dialog window
/*---------------------------------------------------------------------------*/
void c2mExporter::Options2Dialog()
{
	SetCheck( hExportAnimWnd,	bExportAnimation );
	SetCheck( hCopyTexWnd,		!bCopyTextures	 ); 
	SetCheck( hAnimOnly,		bAnimationOnly	 ); 
	SetCheck( hReduceKeys,		bReduceKeys		 );
	SetCheck( hNormWnd,			bExportNormals	 ); 
	SetCheck( hOverwriteTextures, bOverwriteTextures ); 
	SetCheck( hColWnd,			bExportColors	 ); 
	SetCheck( hIgnoreSm,		bIgnoreSmoothing ); 
	SetCheck( hNoTex,			bNoTex			 ); 
	SetCheck( hSuppressLog,		bSuppressLog	 ); 
	SetWindowText( hTexPathWnd, me->texPath );
} // c2mExporter::Options2Dialog

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::DisableControls	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void c2mExporter::DisableControls()
{
	EnableWindow( hGoBtn,			FALSE );
	EnableWindow( hTexPathWnd,		FALSE );
	EnableWindow( hChPathWnd,		FALSE );
	EnableWindow( hCopyTexWnd,		FALSE );

	EnableWindow( hCopyTCap,		FALSE );

	EnableWindow( hAnGroup,			FALSE );
	EnableWindow( hNormWnd,			FALSE );
	EnableWindow( hOverwriteTextures, FALSE );
	EnableWindow( hColWnd,			FALSE );
	EnableWindow( hIgnoreSm,		FALSE );
	EnableWindow( hSkinning,		FALSE );
	EnableWindow( h2ndTexPair,		FALSE );
	EnableWindow( hNoTex,			FALSE );
	EnableWindow( hSuppressLog,		FALSE );

	EnableWindow( hExportAnimWnd,	FALSE );
	me->EnableAnimGroup( false );
} // c2mExporter::DisableControls

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::EnableControls	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void c2mExporter::EnableControls()
{
	EnableWindow( hGoBtn,			TRUE );
	EnableWindow( hCopyTCap,		TRUE );

	if (me->bCopyTextures)
	{
		EnableWindow( hTexPathWnd,			TRUE );
		EnableWindow( hChPathWnd,			TRUE );
		EnableWindow( hOverwriteTextures,	TRUE );
	}
	else
	{
		EnableWindow( hTexPathWnd,			FALSE );
		EnableWindow( hChPathWnd,			FALSE );
		EnableWindow( hOverwriteTextures,	FALSE );
	}

	EnableWindow( hCopyTexWnd,		TRUE );	

	EnableWindow( hAnGroup,			TRUE );
	EnableWindow( hNormWnd,			TRUE );
	EnableWindow( hColWnd,			TRUE );
	EnableWindow( hIgnoreSm,		TRUE );
	EnableWindow( hSkinning,		TRUE );
	EnableWindow( h2ndTexPair,		TRUE );
	EnableWindow( hNoTex,			TRUE );
	EnableWindow( hSuppressLog,		TRUE );

	EnableWindow( hExportAnimWnd,	TRUE );

	me->EnableAnimGroup( me->bExportAnimation );
} // c2mExporter::EnableControls

DialogResult c2mExporter::AskYesNo( const char* msg )
{
	if (MessageBox( hPanelWnd, msg, "c2mExport", MB_YESNO ) == IDYES)
	{
		return drYes;
	}
	else 
	{
		return drNo;
	}
} // c2mExporter::AskYesNo	

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::CreateTextureDir
/*	Desc:	Does stuff related to the texture directory creation
/*---------------------------------------------------------------------------*/
void c2mExporter::CreateTextureDir()
{
	//  check if texture path directory exists
	//  if not - create it
	if (bCopyTextures && _chdir( texPath ) == -1)
	{
		char askStr[512];
		sprintf( askStr, 
				"Directory <%s> does not exist. Create?..", 
				texPath );
		int res = MessageBox( 0, askStr, "cllExport", 
								MB_YESNOCANCEL | MB_ICONWARNING | MB_DEFBUTTON1 );
		if (res == IDYES)
		{
			if (_mkdir( texPath ) == 0)
			{
				Msg( "Created directory <%s>", texPath );
			}
			else
			{
				Msg( "Cannot create texture directory: %s", texPath );
				Msg( "Textures will not be copied." );
				bCopyTextures = false;	
			}
		}
		if (res == IDNO)
		{
			bCopyTextures = false;
			Msg( "Textures will not be copied." );
			bCopyTextures = false;	
		}
		if (res == IDCANCEL) 
		{
			EnableControls();
			return;
		}
	}
} // c2mExporter::CreateTextureDir

void c2mExporter::ClearTextureTable()
{
	for (int i = 0; i < texTable.size(); i++)
	{
		delete texTable[i];
	}
	texTable.clear();	
} // c2mExporter::ClearTextureTable

void c2mExporter::ResetGeometryStorage()
{
	nPoly = 0;
	vertHash.reset();
} // c2mExporter::ResetGeometryStorage

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::AddPolygon	
/*---------------------------------------------------------------------------*/
int	c2mExporter::AddPolygon( int v1, int v2, int v3 )
{
	if (nPoly >= maxPoly)
	//  grow polygon pool
	{
		Poly* npoly = new Poly[maxPoly + maxPoly / 2];
		memcpy( npoly, poly, maxPoly * sizeof( Poly ) );
		maxPoly += maxPoly / 2;
		delete []poly;
		poly = npoly;
	}
	poly[nPoly].v1 = v1;
	poly[nPoly].v2 = v2;
	poly[nPoly].v3 = v3;
	nPoly++;
	return 0;
} // c2mExporter::AddPolygon

void c2mExporter::SetPluginFileName( const char* fileName )
{
	strcpy( pluginFileName, fileName );
} // c2mExporter::SetPluginFileName

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::InitInternal
/*	Desc:	Should be necessarily called when start exporting
/*---------------------------------------------------------------------------*/
void c2mExporter::InitInternal()
{
	delete []poly;
	poly			= new Poly[c_MinPolyNum];
	maxPoly			= c_MinPolyNum;
	nPoly			= 0;

	nPolyTotal		= 0;
	nVertTotal  	= 0;

	vertHash.reset();

	SetWindowText( hGoBtn, "Export to c2m format..." );
	
	exportDone		= false;
	abortProcess	= false;
	
	nTransformNodes	= 0;

	radius2			= 0.0f;

	char	drive	[_MAX_DRIVE];
	char	dir		[_MAX_DIR];
	char	ext		[_MAX_EXT];

	nIndent			= 0;

	if (m_pAnimGroup) m_pAnimGroup->Release();
	if (m_pModel) m_pModel->Release();

	m_pAnimGroup	= NULL;
	m_pModel		= NULL;

	_splitpath( modelFileName, drive, dir, modelName, ext );
} // c2mExporter::InitInternal

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::ShutInternal
/*	Desc:	Should be necessarily called when end exporting
/*---------------------------------------------------------------------------*/
void c2mExporter::ShutInternal()
{
	delete []poly; 
	poly = 0;
	SetWindowText( hGoBtn, "OK" );
	EnableWindow( hGoBtn, TRUE );
	UpdateWindow( hPanelWnd );

	ClearTextureTable();

	for (int i = 0; i < dssTable.size(); i++)
	{
		delete dssTable[i];
	}
	dssTable.clear();

	for (int i = 0; i < mtlTable.size(); i++)
	{
		delete mtlTable[i];
	}
	mtlTable.clear();

	exportDone = true;
} // c2mExporter::ShutInternal

void c2mExporter::PostprocessScene()
{
	if (texTable.size() > 0)
	{
		Group* pTextures = new Group();
		pTextures->SetName( "Textures" );
		m_pModel->AddChild( pTextures, 0 );
		for (int i = 0; i < texTable.size(); i++)
		{
			pTextures->AddChild( texTable[i] );
		}
		pTextures->SetInvisible();
	}

	//if (mtlTable.size() > 0)
	//{
	//	Group* pMaterials = new Group();
	//	pMaterials->SetName( "Materials" );
	//	m_pModel->AddChild( pMaterials, 0 );
	//	/*for (int i = 0; i < mtlTable.size(); i++)
	//	{
	//		pMaterials->AddChild( mtlTable[i] );
	//	}*/

	//	Material* pMtl = CreateDefaultMaterial();
	//	pMaterials->AddChild( pMtl );
    
	//	//pMaterials->SetInvisible();
	//}

	if (dssTable.size() > 0)
	{
		Group* pDSS = new Group();
		pDSS->SetName( "DeviceStates" );
		m_pModel->AddChild( pDSS, 0 );
		for (int i = 0; i < dssTable.size(); i++)
		{
			pDSS->AddChild( dssTable[i] );
		}
		pDSS->SetInvisible();
	}

	if (m_pModel)
	{
		/*SNode::Iterator it( m_pModel, BaseCamera::FnFilter );
		if (it)
		{
			Switch* pCameras = new Switch();
			pCameras->SetName( "Cameras" );
			while (it)
			{
				BaseCamera* pCamera = (BaseCamera*)(SNode*)it;
				pCamera->SetInvisible();
				pCameras->AddInput( pCamera );
				++it;
			}
			pCameras->SwitchTo( -1 );
			m_pModel->AddChild( pCameras, 0 ); 
		}*/

		SNode::Iterator lit( m_pModel, LightSource::FnFilter );
		if (lit)
		{
			Group* pLights = new Group();
			pLights->SetName( "Lights" );
			while (lit)
			{
				LightSource* pLight = (LightSource*)*lit;
				pLights->AddInput( pLight );
				++lit;
			}
			m_pModel->AddChild( pLights, 0 );
		}
	}

} // c2mExporter::PostprocessScene

bool  ConvertModel( SNode* pModel );
bool c2mExporter::WriteModel( SNode* pModel )
{
	FOutStream os( modelFileName );
	if (os.NoFile())
	{
		Msg( "Error creating file <%s>", modelFileName );
		return false;
	}

	if (!pModel) 
	{
		if (!m_pAnimGroup) return false;
		return m_pAnimGroup->SerializeSubtree( os );
	}
    ConvertModel( pModel );
    return pModel->SerializeSubtree( os );
} // c2mExporter::WriteModel

void c2mExporter::UpOneDirTreeLevel( char* dir )
{
	int len = strlen( dir );
	char* pos = &dir[len - 1];
	if (isslash( *pos )) pos--;
	while (pos >= dir && !isslash( *pos )) pos--;
	if (pos >= dir) *(pos + 1) = 0;
} // c2mExporter::UpOneDirTreeLevel

/*---------------------------------------------------------------------------*/
/*	Func:	c2mExporter::BrowseForFolder	
/*	Desc:	Shows "browse for folder" dialog
/*	Ret:	folder path, NULL if user canceled
/*---------------------------------------------------------------------------*/
char* c2mExporter::BrowseForFolder()
{
	BROWSEINFO				binfo;
	char					dname[_MAX_PATH];
	ITEMIDLIST*				pIDList;
	IMalloc*				pMalloc;
	
	const char* title		= "Select textures directory";

	binfo.hwndOwner			= hPanelWnd;
	binfo.pidlRoot			= NULL;
	binfo.pszDisplayName	= dname;
	binfo.lpszTitle			= title;
	binfo.ulFlags			= BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS | 0x0040;
	binfo.lpfn				= NULL;
	binfo.lParam			= 0;
	binfo.iImage			= 0;

	pIDList = SHBrowseForFolder(&binfo);

	if (pIDList)
	{
		SHGetPathFromIDList( pIDList, folderPath );
		if (strlen( folderPath ) == 3) folderPath[2] = 0;
		if (SHGetMalloc( &pMalloc ) == NOERROR)
		{
			pMalloc->Free( pIDList );
		}
	}
	else return 0;
	return folderPath;
} // c2mExporter::onBrowsePath

SurfaceMaterial* c2mExporter::CreateDefaultMaterial()
{
	SurfaceMaterial* pMtl = new SurfaceMaterial();
	pMtl->SetAmbient		( 0xFF444444 );
	pMtl->SetDiffuse		( 0xFFFFFFFF );
	pMtl->SetSpecular		( 0xFF000000 );	
	pMtl->SetShininess		( 20.0f );

	char mtlName[256];
	sprintf( mtlName, "%s_mtl", modelName );
	pMtl->SetName( mtlName );
	return pMtl;
} // c2mExporter::CreateDefaultMaterial

