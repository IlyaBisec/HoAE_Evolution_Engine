/*****************************************************************************/
/*	File:	akField.cpp
/*	Desc:	Wheat field	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	16.05.2003
/*****************************************************************************/
#include "stdafx.h"
#include "akField.h"
#ifndef _INLINES
#include "akField.inl"
#endif // !_INLINES

/*****************************************************************************/
/*	FieldModel implementation
/*****************************************************************************/
FieldModel::FieldModel()
{
	Setm_StrawHeight( c_DefStrawHeight );
	m_bInited = false;
}

FieldModel::~FieldModel()
{	
}

void FieldModel::Init()
{
	m_FieldMesh.setTexture	    ( IRS->GetTextureID( "pole1.tga" )	);
	m_FieldMesh.setShader	    ( IRS->GetShaderID( "field" )		);
	m_FieldMesh.create		    ( c_FieldBatchSize, 0				);
	m_FieldMesh.setIsQuadList	( true );

	for (int i = 0; i < c_NumFieldRnd; i++)
	{
		m_Rnd[i] = rndValuef();
	}
	m_bInited = true;
} // FieldModel::Init

_inl void FieldModel::GetBoundingSphere( Sphere& sphere )
{
	m_FieldMesh.GetBoundSphere( sphere ); 
} // FieldModel::GetBoundingSphere

void FieldModel::Draw()
{
	if (!m_bInited) Init();
	int nV = m_FieldMesh.getNVert();
	Vertex2t* sv = (Vertex2t*)m_FieldMesh.getVertexData();
	DWORD ctime = GetAnimationTime();

	int nQ = nV / 4;
	int cV = 0;
	for (int i = 0; i < nQ; i++)
	{		
		Vertex2t& vlt = sv[cV + 0];
		Vertex2t& vrt = sv[cV + 1];
		Vertex2t& vlb = sv[cV + 2];
		Vertex2t& vrb = sv[cV + 3];

		float bendX, bendY;
		
		//  get bend direction & amount for this straw
		float bendAmount = GetStrawBend( vlt.x, vlt.y, ctime, bendX, bendY );
		float strawPitch = GetStrawPitch( vlt.z - vlb.z, bendAmount );

		vlt.x += bendX*bendAmount*strawPitch;
		vlt.y += bendY*bendAmount*strawPitch;

		//  get bend direction & amount for this straw
		bendAmount = GetStrawBend( vrt.x, vrt.y, ctime, bendX, bendY );
		strawPitch = GetStrawPitch( vrt.z - vrb.z, bendAmount );

		vrt.x += bendX*bendAmount*strawPitch;
		vrt.y += bendY*bendAmount*strawPitch;

		cV += 4;
	}	
	
	m_FieldMesh.setNPri( nV / 2 );

	DrawBM( m_FieldMesh );

	m_FieldMesh.setNVert( 0 );
} // FieldModel::DrawPatch

bool FieldModel::AddPatch(	const Vector3D& lt,
							const Vector3D& rt,
							const Vector3D& lb,
							const Vector3D& rb,
							float growRatio, 
							float heightScale )
{
	DWORD growColor = GetGrowColor( growRatio );

	Vector3D lRail, rRail, vcl, vcr;

	lRail.sub( lt, lb );
	rRail.sub( rt, rb );
	
	int curRow	= 0;
	float lLen = lRail.norm();
	float rLen = rRail.norm();

	lRail /= lLen;
	rRail /= rLen;

	int nRows	= lLen / c_FieldRowsStep;
	int nRows1	= rLen / c_FieldRowsStep;
	nRows = nRows1 > nRows ? nRows1 : nRows;

	float lStep = lLen / float( nRows );
	float rStep = rLen / float( nRows );
	float lStepPart = lStep*c_FieldRowsStepPart/c_FieldRowsStep;
	float rStepPart = rStep*c_FieldRowsStepPart/c_FieldRowsStep;
	
	float lC = 0.0f, rC = 0.0f;

	int nV		= nRows * 4;
	int newNV	= m_FieldMesh.getNVert() + nV;
	if (newNV > m_FieldMesh.getMaxVert())
	{
		return false;
	}
	
	Vertex2t* v = (Vertex2t*)m_FieldMesh.getVertexData() + m_FieldMesh.getNVert();
	int cV = 0;
	for (int i = 0; i < nRows; i++)
	{
		rndInit( (i*i*i*i + 128189 - 101*i)^((int)(fabs( lt.x )*fabs( lt.y ) + 1317)) );
		float lShift = curRow & 1 ? lC : lC + lStepPart;
		lC += lStep;
		
		float rShift = curRow & 1 ? rC : rC + rStepPart;
		rC += rStep;
		
		Vertex2t& vlt = v[cV + 0];
		Vertex2t& vrt = v[cV + 1];
		Vertex2t& vlb = v[cV + 2];
		Vertex2t& vrb = v[cV + 3];

		vcl.copy( lb );
		vcl.addWeighted( lRail, lShift ); 
		vcr.copy( rb );
		vcr.addWeighted( rRail, rShift ); 

		float ushift = rndValuef();
		float u1 = ushift + vcl.x*c_FieldTextureURatio; 
		float u2 = ushift + vcr.x*c_FieldTextureURatio;

		vlt.x		= vcl.x;
		vlt.y		= vcl.y;
		vlt.z		= vcl.z + GetStrawHeight( vcl.x, vcl.y, growRatio );
		vlt.v		= 0.0f;
		vlt.u		= u1;
		vlt.diffuse	= growColor;

		vlb.x		= vcl.x;
		vlb.y		= vcl.y;
		vlb.z		= vcl.z;
		vlb.v		= 32.0f / 256.0f;
		vlb.u		= u1;
		vlb.diffuse	= growColor;

		vrt.x		= vcr.x;
		vrt.y		= vcr.y;
		vrt.z		= vcr.z + GetStrawHeight( vcr.x, vcr.y, growRatio );
		vrt.u		= u2;
		vrt.v		= 0.0f;
		vrt.diffuse	= growColor;

		vrb.x		= vcr.x;
		vrb.y		= vcr.y;
		vrb.z		= vcr.z;
		vrb.v		= 32.0f / 256.0f;
		vrb.u		= u2;
		vrb.diffuse	= growColor;

		cV += 4;
		curRow++;
	}

	m_FieldMesh.setNVert( newNV );

	return true;
} // FieldModel::AddPatch

bool FieldModel::AddPatch(	float x, float y, float z,
							float width, float height,
							const Vector3D& normal,
							float angle,
							float growRatio,
							float heightScale )
{
	DWORD growColor = GetGrowColor( growRatio );

	//  patch transform
	Vector3D cX( Vector3D::oX ), cY;
	cY.cross( normal, cX );
	cX.cross( cY, normal );
	Matrix4D worldMatr, m;
	m.fromBasis( cX, cY, Vector3D::oZ, Vector3D( x, y, z ) );
	worldMatr.rotation( Vector3D::oZ, angle );
	worldMatr *= m;
	
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	
	float locY	= -halfHeight; 
	int curRow	= 0;
	int nRows	= height / c_FieldRowsStep;
	int nV		= nRows * 4;
	int newNV	= m_FieldMesh.getNVert() + nV;
	if (newNV > m_FieldMesh.getMaxVert())
	{
		return false;
	}
	
	Vertex2t* v = (Vertex2t*)m_FieldMesh.getVertexData() + m_FieldMesh.getNVert();
	int cV = 0;
	for (int i = 0; i < nRows; i++)
	{
		float rowShift = curRow & 1 ? locY : locY + c_FieldRowsStepPart;
		locY += c_FieldRowsStep;
		
		Vertex2t& vlt = v[cV + 0];
		Vertex2t& vrt = v[cV + 1];
		Vertex2t& vlb = v[cV + 2];
		Vertex2t& vrb = v[cV + 3];

		float ushift = rndValuef()*10;
		float u1 = ushift + -halfWidth + x; 
		float u2 = ushift + halfWidth + x;

		vlt.x		= -halfWidth;
		vlt.y		= rowShift;
		vlt.z		= GetStrawHeight( x - halfWidth, y, growRatio );
		vlt.v		= 0.0f;
		vlt.u		= u1;
		vlt.diffuse	= growColor;

		vrt.x		= halfWidth;
		vrt.y		= rowShift;
		vrt.z		= GetStrawHeight( x + halfWidth, y, growRatio );
		vrt.u		= u2;
		vrt.v		= 0.0f;
		vrt.diffuse	= growColor;

		vlb.x		= -halfWidth;
		vlb.y		= rowShift;
		vlb.z		= 0.0f;
		vlb.v		= 32.0f / 256.0f;
		vlb.u		= u1;
		vlb.diffuse	= growColor;

		vrb.x		= halfWidth;
		vrb.y		= rowShift;
		vrb.z		= 0.0f;
		vrb.v		= 32.0f / 256.0f;
		vrb.u		= u2;
		vrb.diffuse	= growColor;

		(*(Vector3D*)(&vlt)) *= worldMatr;
		(*(Vector3D*)(&vrt)) *= worldMatr;
		(*(Vector3D*)(&vlb)) *= worldMatr;
		(*(Vector3D*)(&vrb)) *= worldMatr;
		
		cV += 4;
		curRow++;
	}

	m_FieldMesh.setNVert( newNV );

	return true;
} // FieldModel::AddPatch

