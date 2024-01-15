/*****************************************************************************/
/*	File:	akScape.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-11-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kHash.hpp"
#include "akScape.h"

#ifndef _INLINES
#include "akScape.inl"
#endif // _INLINES

/*****************************************************************************/
/*	ScapeQuad implementation
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeQuad::CreateInWorld	
/*	Desc:	creates quad mesh in world space
/*---------------------------------------------------------------------------*/
void ScapeQuad::CreateInWorld( int qx, int qy, int qlod )
{
	if (!THMap) return;
	lod = qlod;
	bound.x		= qx;
	bound.y		= qy;
	bound.x1	= qx + c_QuadSide;
	bound.y1	= qy + c_QuadSide;

	Vertex2t*	vert	= (Vertex2t*)mesh.getVertexData();
	int		cVert	= 0;
	int		vInLine = c_QuadHorzTris + 1;

	int		cV		= 0;
	float	bX		= ((float)qx) * c_WorldGridStepX / c_HmapGridStepX;  
	float	worldX	= bX;
	float	worldY	= ((float)qy) * c_WorldGridStepY / c_HmapGridStepY; 

	int		hmapX	= qx;
	int		hmapY	= qy;

	int		hStepX	= c_HmapGridStepX << qlod;
	int		hStepY	= c_HmapGridStepY << qlod;
	int		hStepYHalf = hStepY / 2;	
	float	wStepX	= ((float)hStepX) * c_WorldGridStepX / c_HmapGridStepX;
	float	wStepY	= ((float)hStepY) * c_WorldGridStepY / c_HmapGridStepY;
	float	wStepYHalf = wStepY * 0.5f;

	for (int i = 0; i < vInLine; i++) 
	{
		for (int j = 0; j < vInLine; j++) 
		{
			int ry = (j & 1) ? hmapY : hmapY + hStepYHalf;
			float H = (float)GetHeight( hmapX, ry );
			vert[cV].x = worldX;
			vert[cV].z = H * c_Cos30;
			vert[cV].y = worldY + ((j & 1) ? 0.0f : wStepYHalf) - H*0.5f;
			//vert[cV].y += vert[cV].z * 0.16f; 

			//  texture coordinates
			vert[cV].diffuse = 0xFFFFFFFF;//GetDW_Light( VIdx( hmapX / 32, ry / 32 ) );

			cV++;
			worldX += wStepX;
			hmapX += hStepX;
		}
		hmapX = qx;
		hmapY += hStepY;

		worldX = bX;
		worldY += wStepY;
	}

	//  now skew it
	//mesh.transform( skewMatr );
} // ScapeQuad::CreateInWorld

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeQuad::InitMesh
/*---------------------------------------------------------------------------*/
void ScapeQuad::InitMesh()
{
	mesh.create			( c_QuadVert, 0, Vertex2t::format(), ptTriangleList	);
	mesh.setIndexPtr	( ScapeQuad::qIdx									);
	mesh.setNVert		( c_QuadVert										);
	mesh.setMaxInd		( c_QuadInd											);
	mesh.setNInd		( c_QuadInd											);
	mesh.setNPri		( BaseMesh::calcNumPri( ptTriangleList, c_QuadInd ) );

	//mesh.setShader		( IRS->GetShaderID( "zground" )						);
	mesh.setShader		( IRS->GetShaderID( "fground" )						);
	mesh.setTexture		( IRS->GetTextureID( "ground2.bmp" )				);
	mesh.setTexture		( IRS->GetTextureID( "edge.tga" ), 1				);

	//  texture coordinates
	int			vInLine = c_QuadHorzTris + 1;
	int			cVert	= 0;
	float		cV		= 0.0f;
	float		cU		= 0.0f;

	//float		stepU	= 1.0f / (float)vInLine;
	//float		stepV	= 1.0f / (float)vInLine;

	float		stepU	= 1.0f / (float)vInLine;
	float		stepV	= 1.0f / (float)vInLine;

	float		halfStepV = stepV * 0.5f;
	Vertex2t* vert = (Vertex2t*)mesh.getVertexData();
	for (int i = 0; i < vInLine; i++) 
	{
		for (int j = 0; j < vInLine; j++) 
		{
			vert[cVert].u = cU;
			vert[cVert].v = cV + ((j & 1) ? 0.0f : halfStepV);

			cVert++;
			cU += stepU;
		}
		cU = 0.0f;
		cV += stepV;
	}

} // ScapeQuad::InitMesh

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeQuad::InitIndices	
/*---------------------------------------------------------------------------*/
void ScapeQuad::InitIndices()
{
	int cI = 0;
	int cV = 0;
	int vInLine = c_QuadHorzTris + 1;
	for (int i = 0; i < c_QuadHorzTris; i++)
	{
		for (int j = 0; j < c_QuadHorzTris; j++)
		{
			if (j & 1)
			{
				qIdx[cI++] = cV;
				qIdx[cI++] = cV + 1;
				qIdx[cI++] = cV + vInLine;
				qIdx[cI++] = cV + vInLine;
				qIdx[cI++] = cV + 1;
				qIdx[cI++] = cV + vInLine + 1;
			}
			else
			{
				qIdx[cI++] = cV;
				qIdx[cI++] = cV + 1;
				qIdx[cI++] = cV + 1 + vInLine;
				qIdx[cI++] = cV;
				qIdx[cI++] = cV + 1 + vInLine;
				qIdx[cI++] = cV + vInLine;	
			}
			cV++;
		}
		cV++;
	}
} // ScapeQuad::InitIndices

/*****************************************************************************/
/*	ScapeMesh implementation
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/*	Func:	ScapeMesh::Init	
/*	Desc:	
/*---------------------------------------------------------------------------*/
void ScapeMesh::Init( IRenderSystem* iRS )
{
	ScapeQuad::InitIndices();

	int cQuad = 0;
	for (int i = 0; i < quadHache.maxElem(); i++)
	{
		ScapeQuad* q = quadHache.elem( i );
		q->InitMesh();
	}

	SetNeedUpdate( false );
} // ScapeMesh::Init

const int c_ScapeMeshBorder = c_QuadSide;

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeMesh::Show
/*	Desc:	Draws bunch of scape quads
/*---------------------------------------------------------------------------*/
void ScapeMesh::Show( IRenderSystem* iRS )
{
	int		mapX	= mapx * c_HmapGridStepX;
	int		mapY	= mapy * c_HmapGridStepY;
	int		qside	= c_QuadSide;
	int		bx		= (mapX / qside) * qside - c_ScapeMeshBorder;
	int		by		= (mapY / qside) * qside - 16 - c_ScapeMeshBorder;

	int		ex		= mapX + /*unzoomex( RealLx )*/ + c_ScapeMeshBorder; 
	int		ey		= mapY + /*unzoomex( RealLy * 2 + c_BottomFieldExtent )*/ + c_ScapeMeshBorder;
	DWORD	factor	= GetTickCount();
	int		nQuads	= 0;
	int		LOD		= 0;
	float	quadSide = (float)((int)c_QuadSide << LOD);

	for (int qy = by; qy < ey; qy += quadSide)
	{					
		for (int qx = bx; qx < ex; qx += quadSide)
		{
			ScapeQuad::Attr attr( qx, qy, LOD );
			ScapeQuad* quad = quadHache.getHitAllocEl( attr, factor );
			assert( quad );
			if (!quad->hasAttr( attr ) || needUpdate) 
			{
				quad->CreateInWorld( qx, qy, LOD );
			}
			IRS->Draw( quad->mesh ); 
			nQuads++;
		}
	}
	needUpdate = false;
} // ScapeMesh::Show

void ScapeMesh::Reset()
{
	SetNeedUpdate();
}