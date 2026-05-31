/*****************************************************************************/
/*	File:	MaxUtil.cpp
/*	Desc:	3ds max helper functions
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07.11.2002
/*****************************************************************************/
#include "maxPre.h"

#include "Max.h"
#include "MeshAdj.h"

#include "c2mExporter.h"
#include "MaxUtil.h"

float TimeToMs( TimeValue t )
{
	return 1000.0f * float( t ) / float( TIME_TICKSPERSEC );
}

TimeValue MsToTime( float ms )
{
	return TimeValue( float( TIME_TICKSPERSEC ) * ms / 1000.0f );
}

Matrix3 GetNodeLocalTM( INode* node, TimeValue t )
{
	INode *parent;
	Matrix3 nodeTM, localTM;
	Matrix3 parentTM;
	parentTM.IdentityMatrix();
	
	if (!node) 
	{
		localTM.IdentityMatrix();
		return localTM;
	}
	
	nodeTM = node->GetNodeTM( t );
	parent = node->GetParentNode();

	if (parent)
	{
		parentTM = parent->GetNodeTM( t );
	}

	localTM = nodeTM * Inverse( parentTM );
	return localTM;
} // GetNodeLocalTM

/*---------------------------------------------------------------------------*/
/*	Func:	TotalChildrenNum	
/*	Desc:	returns total number of nodes in subtree
/*---------------------------------------------------------------------------*/
int TotalChildrenNum( INode* node )
{
	int nNodes = 1;
	int numCh = node->NumberOfChildren();
	for (int i = 0; i < numCh; i++) 
		nNodes += TotalChildrenNum( node->GetChildNode( i ) );
	return nNodes;
} // TotalChildrenNum

/*---------------------------------------------------------------------------*/
/*	Func:	FaceHasEdge
/*	Desc:	finds if given face has an edge with the endpoits of indices v1&v2
/*---------------------------------------------------------------------------*/
inline bool FaceHasEdge( const Face& face, DWORD v1, DWORD v2 )
{
	bool found1 = false;
	bool found2 = false;
	if (face.v[0] == v1) found1 = true;
	if (face.v[1] == v1) found1 = true;
	if (face.v[2] == v1) found1 = true;
	
	if (face.v[0] == v2) found2 = true;
	if (face.v[1] == v2) found2 = true;
	if (face.v[2] == v2) found2 = true;
	return (found1 && found2);
} // FaceHasEdge

/*---------------------------------------------------------------------------*/
/*	Func:	IsHardEdge	
/*	Desc:	returns true if edge (vert1, vert2) in face is hard
/*	Remrk:	vert1, vert2 are indices of vertices in face, not in mesh
/*---------------------------------------------------------------------------*/
bool IsHardEdge( const Mesh& mesh, AdjFaceList& adjFaceList, 
					DWORD faceIdx, DWORD vert1, DWORD vert2 )
{
	const Face&		thisFace = mesh.faces[faceIdx];
	vert1 = thisFace.v[vert1];
	vert2 = thisFace.v[vert2];

	const AdjFace&	adjFace	 = adjFaceList[faceIdx];
	int cFace = 0;
	while (cFace < 3)
	{
		DWORD adjFaceIdx = adjFace.f[cFace++];
		if (adjFaceIdx == UNDEFINED) continue;
		const Face& face = mesh.faces[adjFaceIdx];
		if (FaceHasEdge( face, vert1, vert2 ))
		{
			return ((thisFace.smGroup^face.smGroup) != 0);
		}
	}
	return false;
} // IsHardEdge

/*****************************************************************************/
/*	VNormal implementation
/*****************************************************************************/
void VNormal::AddNormal( Point3 &n, DWORD sB ) 
{
	if (!(sB & smoothBits) && init) 
	{
		if (next) 
		{
			next->AddNormal( n, sB );
		}
		else 
		{
			next = new VNormal( n, sB );
		}
	}
	else 
	{
		norm		+=	n;
		smoothBits	|=	sB;
		init		=	TRUE;
	}
} // VNormal::AddNormal

Point3& VNormal::GetNormal( DWORD sB ) 
{
	if ((smoothBits & sB) || !next) 
	{
		return norm;
	}
	else 
	{
		return next->GetNormal( sB );
	}
} // VNormal::GetNormal

void VNormal::Normalize() 
{
	VNormal*	ptr		= next;
	VNormal*	prev	= this;
	while (ptr) 
	{
		if (ptr->smoothBits & smoothBits) 
		{
			norm		+= ptr->norm;
			prev->next	= ptr->next;
			ptr			= prev->next;
		}
		else 
		{
			prev	= ptr;
			ptr		= ptr->next;
		}
	}
	
	norm = ::Normalize(norm);
	if (next) next->Normalize();
} // VNormal::Normalize

void ComputeVertexNormals( const Mesh& mesh, Tab<VNormal>& vnorms )
{
	Point3 v0, v1, v2;
	
	Face*	face	= mesh.faces; 
	Point3* verts	= mesh.verts;
	
	Tab<Point3>		fnorms;
	int nV = mesh.getNumVerts();
	int nF = mesh.getNumFaces();
	vnorms.SetCount( nV );
	fnorms.SetCount( nF );
	
	for (int i = 0; i < nV; i++) 
	{
		vnorms[i].Init();
	}
	
	for (i = 0; i < nF; i++, face++)
	{
		if (face->v[0] >= nV || 
			face->v[1] >= nV ||
			face->v[2] >= nV) continue;
		
		v0 = verts[face->v[0]];
		v1 = verts[face->v[1]];
		v2 = verts[face->v[2]];
		//  cross product == non-normalized face normal
		fnorms[i] = (v1-v0)^(v2-v1);

		for (int j = 0; j < 3; j++)
		{
			vnorms[face->v[j]].AddNormal( fnorms[i], face->smGroup );
		}
		fnorms[i] = Normalize( fnorms[i] );
	}
	
	for (i=0; i < vnorms.Count(); i++) 
	{
		vnorms[i].Normalize();
	}
} // ComputeVertexNormals

Matrix3	ConvertMatrix( const Matrix4D& tm )
{
	Matrix3 matr;
	matr.SetRow( 0, Point3( tm.e00, tm.e01, tm.e02 ) );
	matr.SetRow( 1, Point3( tm.e10, tm.e11, tm.e12 ) );
	matr.SetRow( 2, Point3( tm.e20, tm.e21, tm.e22 ) );
	matr.SetRow( 3, Point3( tm.e30, tm.e31, tm.e32 ) );
	return matr;
}

Matrix4D ConvertMatrix( const Matrix3& tm )
{
	Matrix4D matr;
	matr.e00 = tm.GetRow(0).x; 
	matr.e01 = tm.GetRow(0).y; 
	matr.e02 = tm.GetRow(0).z;
	matr.e03 = 0.0f;

	matr.e10 = tm.GetRow(1).x; 
	matr.e11 = tm.GetRow(1).y; 
	matr.e12 = tm.GetRow(1).z;
	matr.e13 = 0.0f;

	matr.e20 = tm.GetRow(2).x; 
	matr.e21 = tm.GetRow(2).y; 
	matr.e22 = tm.GetRow(2).z;
	matr.e23 = 0.0f;

	matr.e30 = tm.GetRow(3).x; 
	matr.e31 = tm.GetRow(3).y; 
	matr.e32 = tm.GetRow(3).z;
	matr.e33 = 1.0f;

	return matr;
} // ConvertMatrix

DWORD	ColorToDWORD( const Color& color )
{
	DWORD ba = 0x000000FF;
	DWORD br = (DWORD)(color.r * 255.0f);
	DWORD bg = (DWORD)(color.g * 255.0f);
	DWORD bb = (DWORD)(color.b * 255.0f);

	return  (
		((ba & 0xFF) << 24) | 
		((br & 0xFF) << 16) |
		((bg & 0xFF) << 8)  |
		(bb & 0xFF)
		);
} // ColorToDWORD
