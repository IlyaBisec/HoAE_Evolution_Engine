/*****************************************************************/
/*  File:   gpAuxPrim.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "IRenderSystem.h"
#include "gpAuxPrim.h"

namespace AuxPrim
{

/*****************************************************************/
/*  CoorSys implementation
/*****************************************************************/
CoorSys::CoorSys()
{
}

CoorSys::~CoorSys()
{
}

void CoorSys::Init( float axisLen )
{
	create( 6, 6, vf2Tex, ptLineList );
	Vertex2t* vbuf = (Vertex2t*)getVertexData();
	WORD idx[] = { 0, 3, 1, 4, 2, 5 };

	vbuf[0].x = 0.0f;
	vbuf[0].y = 0.0f;
	vbuf[0].z = 0.0f;
	vbuf[0].diffuse = 0xFFFF0000;

	vbuf[1].x = 0.0f;
	vbuf[1].y = 0.0f;
	vbuf[1].z = 0.0f;
	vbuf[1].diffuse = 0xFF00FF00;

	vbuf[2].x = 0.0f;
	vbuf[2].y = 0.0f;
	vbuf[2].z = 0.0f;
	vbuf[2].diffuse = 0xFF0000FF;

	vbuf[3].x = axisLen;
	vbuf[3].y = 0.0f;
	vbuf[3].z = 0.0f;
	vbuf[3].diffuse = 0xFFFF0000;

	vbuf[4].x = 0.0f;
	vbuf[4].y = axisLen;
	vbuf[4].z = 0.0f;
	vbuf[4].diffuse = 0xFF00FF00;

	vbuf[5].x = 0.0f;
	vbuf[5].y = 0.0f;
	vbuf[5].z = axisLen;
	vbuf[5].diffuse = 0xFF0000FF;
	setNVert( 6 );
	setIndices( idx, 6 );

	setShader( RC::iRS->GetShaderID( "lines3d" ) );
} // CoorSys::Init

void CoorSys::Draw()
{
	RC::iRS->SetWorldMatrix( Matrix4D::identity );
	RC::iRS->Draw( *this );
}

void DrawTransformGizmo( const Matrix4D& transform )
{
	static CoorSys csys;
	BEGIN_ONCE
		csys.Init( 1.0f );
	END_ONCE

	RC::iRS->SetWorldMatrix( transform );
	RC::iRS->Draw( csys );
}

void DrawNormals( const VertexN* srcVert, int nVert, float normalLen )
{
	static BaseMesh nbm;
	
	nbm.create( nVert * 2, 0, vf2Tex, ptLineList );
	
	Vertex2t* dv = (Vertex2t*)nbm.getVertexData();

	for (int i = 0; i < nVert; i++)
	{
		Vertex2t& dv1 = dv[i*2];
		Vertex2t& dv2 = dv[i*2 + 1];

		Vector3D& p = *((Vector3D*)&(srcVert[i].x));
		Vector3D& n = *((Vector3D*)&(srcVert[i].nx));
		
		Vector3D& v2 = *((Vector3D*)&(dv2.x));

		dv1.x = p.x;
		dv1.y = p.y;
		dv1.z = p.z;

		v2.copy( p );
		v2.addWeighted( n, normalLen );
	}
	static int shLines = RC::iRS->GetShaderID( "lines3D" );
	nbm.setShader( shLines );
	nbm.setNPri( nVert );
	nbm.setNVert( nVert * 2 );

	RC::iRS->Draw( nbm );
} // DrawNormals

void CreateCoorSys( BaseMesh& bm, float axisLen )
{
	bm.create( 6, 6, vf2Tex, ptLineList );
	Vertex2t* vbuf = (Vertex2t*)bm.getVertexData();
	WORD idx[] = { 0, 3, 1, 4, 2, 5 };

	vbuf[0].x = 0.0f;
	vbuf[0].y = 0.0f;
	vbuf[0].z = 0.0f;
	vbuf[0].diffuse = 0xFFFF0000;

	vbuf[1].x = 0.0f;
	vbuf[1].y = 0.0f;
	vbuf[1].z = 0.0f;
	vbuf[1].diffuse = 0xFF00FF00;

	vbuf[2].x = 0.0f;
	vbuf[2].y = 0.0f;
	vbuf[2].z = 0.0f;
	vbuf[2].diffuse = 0xFF0000FF;

	vbuf[3].x = axisLen;
	vbuf[3].y = 0.0f;
	vbuf[3].z = 0.0f;
	vbuf[3].diffuse = 0xFFFF0000;

	vbuf[4].x = 0.0f;
	vbuf[4].y = axisLen;
	vbuf[4].z = 0.0f;
	vbuf[4].diffuse = 0xFF00FF00;

	vbuf[5].x = 0.0f;
	vbuf[5].y = 0.0f;
	vbuf[5].z = axisLen;
	vbuf[5].diffuse = 0xFF0000FF;
	bm.setNVert( 6 );
	bm.setIndices( idx, 6 );

	bm.setShader( RC::iRS->GetShaderID( "lines3d" ) );
} // CreateCoorSys

BaseMesh planeMesh;

void DrawPlane( IRenderSystem* IRS, const Plane& plane, DWORD color, 
					int sections = 8, float width = 100.0f )
{
	/*
	Vector3D lt(  );
	Vector3D rt(  );
	Vector3D lb(  );
	Vector3D rb(  );

	planeMesh.createGrid( lt, rt, lb, rb, sections, sections );
	IRS->Draw( planeMesh );
	*/
} // AuxPrim::DrawPlane

} // namespace AuxPrim

