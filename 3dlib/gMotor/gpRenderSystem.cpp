/*****************************************************************************/
/*	File:	gpRenderSystem.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	23.01.2003
/*****************************************************************************/
#include "stdafx.h" 
#include "mMath2D.h"
#include "gpRenderSystem.h"

#ifndef _INLINES
#include "gpRenderSystem.inl"
#endif // _INLINES

IRenderSystem*	RC::iRS			= NULL;
DWORD			RC::m_CurFrame	= 0;

/*****************************************************************
/*	TextureDescr inplementation
/*****************************************************************/
TextureDescr::TextureDescr()
{
	sideX		= 256;
	sideY		= 256;
	colFmt		= cfARGB4444;
	memPool		= mpManaged;
	numMips		= 0;
	texUsage	= tuLoadable;
	client		= NULL;
	id			= c_NoID;
}

void TextureDescr::setValues( int sx, int sy, ColorFormat cf,
								MemoryPool mp, 
								int nmips,
								TextureUsage tu 
								)
{
	sideX		= sx;
	sideY		= sy;
	colFmt		= cf;
	memPool		= mp;
	numMips		= nmips;
	texUsage	= tu;
}

void TextureDescr::copy( const TextureDescr& orig )
{
	sideX		= orig.sideX;
	sideY		= orig.sideY;
	colFmt		= orig.colFmt;
	memPool		= orig.memPool;
	numMips		= orig.numMips;
	texUsage	= orig.texUsage;
}

bool TextureDescr::equal( const TextureDescr& td )
{
	return (colFmt == td.colFmt) &&
		   (memPool == td.memPool) &&
		   (numMips == td.numMips) &&
		   (sideX == td.sideX) &&
		   (sideY == td.sideY) &&
		   (texUsage == td.texUsage);
}

bool TextureDescr::less( const TextureDescr& td )
{
	return (colFmt		< td.colFmt		) ||
		   (memPool		< td.memPool	) ||
		   (numMips		< td.numMips	) ||
		   (texUsage	< td.texUsage	) ||
		   (sideX		< td.sideX		) ||
		   (sideY		< td.sideY		); 
}

/*****************************************************************************/
/*	ScreenProp implementation
/*****************************************************************************/
ScreenProp::ScreenProp()
{
	width			= 1024;
	height			= 768;
	colorFormat		= cfRGB565;
	fullScreen		= false;
	wholeScreenWnd	= true;
	refreshRate		= 75;
} // ScreenProp::ScreenProp

bool ScreenProp::equal( const ScreenProp& prop ) const
{
	return	(width			== prop.width		) &&
			(height			== prop.height		) &&
			(colorFormat	== prop.colorFormat ) &&
			(fullScreen		== prop.fullScreen	) &&
			(wholeScreenWnd	== prop.wholeScreenWnd);
}

/*****************************************************************************/
/*	DeviceDescr implementation
/*****************************************************************************/
/*
void DeviceDescr::LogStatus()
{
	String str;
	str = "DevType: ";
	if (devType == dtHAL) str += "HAL.";
		else str += "REF.";
	Log.Message( str.c_str() );
	
	str = "Adapter ordinal: ";
	str += IntToStr( adapterOrdinal );
	Log.Message( str.c_str() );

	str = "Hardware Rasterization: ";
	str += BoolToStr( hwRasterization );

	str = "Hardware T&L: ";
	str += BoolToStr( hwTnL );

	if (hwPure) Log.Message( "Pure device support." );
	else Log.Message( "No pure device support" );

	str = "Higher-order primitives. NPatches: ";
	str += BoolToStr( hwNPatches );
	str += ". Bezier patches: ";
	str += BoolToStr( hwBezier );
	str += ". RTPatches: ";
	str += BoolToStr( hwRTPatches );
	str += ".";

	Log.Message( str.c_str() );

	str = "Multitexturing capacity: ";
	str += IntToStr( texInSinglePass );
	Log.Message( str.c_str() );

	str = "Max textures in single pass: ";
	str += IntToStr( texInSinglePass );

	Log.Message( "Available depth/stencil formats:" );
	if (dsfD16)			Log.Message( " - depth16 " );
	if (dsfD15S1)		Log.Message( " - depth15 stencil 1" );
	if (dsfD24S8)		Log.Message( " - depth24 stencil 8" );
	if (dsfD24X8)		Log.Message( " - depth24" );
	if (dsfD32)			Log.Message( " - depth32" );
	if (dsfD24X4S4)		Log.Message( " - depth24 stencil 4" );
	if (dsfD16Lockable)	Log.Message( " - depth16 lockable" );

	Log.Message( "Available render-to-texture formats:" );
	if (rttARGB4444)		Log.Message( " - argb4444 " );
	if (rttRGB565)			Log.Message( " - rgb565" );
	if (rttRGB888)			Log.Message( " - rgb888" );
	if (rttA8)				Log.Message( " - a8" );
	if (rttARGB1555)		Log.Message( " - argb1555" );
} // DeviceDescr::LogStatus
*/

/*****************************************************************************/
/*	Primitive drawing utilities implementation
/*****************************************************************************/
const int c_PrimPoolSize = 4096;
const VertexFormat c_vfLines2D	= vfTnL;
const VertexFormat c_vfLines3D	= vf2Tex;
const VertexFormat c_vfPoly2D	= vfTnL;
const VertexFormat c_vfPoly3D	= vf2Tex;

static BaseMesh	g_bmLines2D, g_bmLines3D, g_bmPoly2D, g_bmPoly3D;
static int g_LineShader			 = 0;
static bool g_bLinesZEnable		 = false;

void rsEnableZ( bool enable )
{
	if (g_bLinesZEnable == enable) return;

	static int lines3D_blend	 = RC::iRS->GetShaderID( "lines3D_blend" );
	static int lines3D_blend_noZ = RC::iRS->GetShaderID( "lines3D_blendW" );
	if (enable) 
	{
		g_LineShader = lines3D_blend;
	}
	else
	{
		g_LineShader = lines3D_blend_noZ;
	}
	g_bLinesZEnable = enable;
	g_bmLines2D.setShader( g_LineShader ); 
	g_bmLines3D.setShader( g_LineShader );
	g_bmPoly2D.setShader( g_LineShader );
	g_bmPoly3D.setShader( g_LineShader );
} // rsEnableZ

void rsLine( float x1, float y1, float x2, float y2, float z, DWORD color )
{
	rsLine( x1, y1, x2, y2, z, color, color );
}

void rsLine( float x1, float y1, float x2, float y2, float z, DWORD color1, DWORD color2 )
{
	if (g_bmLines2D.getMaxVert() == 0)
	{
		g_bmLines2D.create( c_PrimPoolSize, 0, c_vfLines2D, ptLineList );	
		rsEnableZ();
	}

	VertexIterator vit;
	vit << g_bmLines2D;
	int nV = g_bmLines2D.getNVert();
	if (nV + 2 >= c_PrimPoolSize) 
	{
		rsFlushLines2D();
		nV = 0;
	}
	Vector4D& v1 = *((Vector4D*)&vit[nV]);
	Vector4D& v2 = *((Vector4D*)&vit[nV + 1]);
	v1.x = x1; v1.y = y1; v1.z = z; v1.w = 1.0f;
	v2.x = x2; v2.y = y2; v2.z = z;	v2.w = 1.0f;
	vit.diffuse( nV ) = color1;		
	vit.diffuse( nV + 1 ) = color2;	
	g_bmLines2D.setNVert( nV + 2 );
	g_bmLines2D.setNPri( (nV>>1) + 1 );
}

void rsFrame( const Rct& rct, float z, DWORD color )
{
	rsLine( rct.x, rct.y, rct.x + rct.w - 1, rct.y, z, color );
	rsLine( rct.x + rct.w - 1, rct.y, rct.x + rct.w - 1, rct.y + rct.h - 1, z, color );
	rsLine( rct.x + rct.w - 1, rct.y + rct.h - 1, rct.x, rct.y + rct.h - 1, z, color );
	rsLine( rct.x, rct.y + rct.h - 1, rct.x, rct.y, z, color );
}

void rsFlushLines2D()
{
	RC::iRS->Draw( g_bmLines2D );
	g_bmLines2D.setNVert( 0 );
	g_bmLines2D.setNPri( 0 );
}

void rsLine( const Vector3D& a, const Vector3D& b, DWORD color )
{
	rsLine( a, b, color, color );
}

void rsLine( const Vector3D& a, const Vector3D& b, DWORD color1, DWORD color2 )
{
	if (g_bmLines3D.getMaxVert() == 0)
	{
		g_bmLines3D.create( c_PrimPoolSize, 0, c_vfLines3D, ptLineList );
		rsEnableZ();
	}

	VertexIterator vit;
	vit << g_bmLines3D;
	int nV = g_bmLines3D.getNVert();
	if (nV + 2 >= c_PrimPoolSize) 
	{
		rsFlushLines3D();
		nV = 0;
	}
	vit[nV]		= a;
	vit[nV + 1] = b;
	vit.diffuse( nV ) = color1;
	vit.diffuse( nV + 1 ) = color2;
	g_bmLines3D.setNVert( nV + 2 );
	g_bmLines3D.setNPri( (nV>>1) + 1 );
}

void rsFlushLines3D()
{
	RC::iRS->Draw( g_bmLines3D );
	g_bmLines3D.setNVert( 0 );
	g_bmLines3D.setNPri( 0 );
}

void rsRect( const Rct& rct, float z, DWORD color )
{
	rsRect( rct, z, color, color, color, color );
}

void rsRect( const Rct& rct, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd )
{
	if (g_bmPoly2D.getMaxVert() == 0)
	{
		g_bmPoly2D.create( c_PrimPoolSize, 0, c_vfPoly2D, ptTriangleList );
		g_bmPoly2D.setIsQuadList( true );
		rsEnableZ();
	}

	VertexIterator vit;
	vit << g_bmPoly2D;
	int nV = g_bmPoly2D.getNVert();
	if (nV + 4 >= c_PrimPoolSize) 
	{
		rsFlushPoly2D();
		nV = 0;
	}
	Vector4D& v1 = *((Vector4D*)&vit[nV]);
	Vector4D& v2 = *((Vector4D*)&vit[nV + 1]);
	Vector4D& v3 = *((Vector4D*)&vit[nV + 2]);
	Vector4D& v4 = *((Vector4D*)&vit[nV + 3]);

	v1.x = rct.x;				v1.y = rct.y;				v1.z = z;  v1.w = 1.0f;
	v2.x = rct.x + rct.w;		v2.y = rct.y;				v2.z = z;  v2.w = 1.0f;
	v3.x = rct.x;				v3.y = rct.y + rct.h;		v3.z = z;  v3.w = 1.0f;
	v4.x = v2.x;				v4.y = v3.y;				v4.z = z;  v4.w = 1.0f;

	vit.diffuse( nV		) = ca;
	vit.diffuse( nV + 1 ) = cb;
	vit.diffuse( nV + 2 ) = cc;
	vit.diffuse( nV + 3 ) = cd;

	g_bmPoly2D.setNVert( nV + 4 );
	g_bmPoly2D.setNPri( g_bmPoly2D.getNPri() + 2 );
}

void rsPanel( const Rct& rct, float z, DWORD clrTop, DWORD clrMdl, DWORD clrBot )
{
	rsRect( Rct( rct.x, rct.y, 1, rct.h ), z, clrTop );
	rsRect( Rct( rct.x, rct.y, rct.w, 1 ), z, clrTop );

	rsRect( Rct( rct.x + 1, rct.y + 1, rct.w - 2, rct.h - 2 ), z, clrMdl );
	
	rsRect( Rct( rct.x, rct.y + rct.h - 1, rct.w, 1 ), z, clrBot );
	rsRect( Rct( rct.x + rct.w - 1, rct.y, 1, rct.h - 1 ), z, clrBot );
}

void rsFlushPoly2D()
{
	RC::iRS->Draw( g_bmPoly2D );
	g_bmPoly2D.setNVert( 0 );
	g_bmPoly2D.setNPri( 0 );
}

void rsPoly( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD acol, DWORD bcol, DWORD ccol )
{
	if (g_bmPoly3D.getMaxVert() == 0)
	{
		g_bmPoly3D.create( c_PrimPoolSize, 0, c_vfPoly3D, ptTriangleList );
		rsEnableZ();
	}

	VertexIterator vit;
	vit << g_bmPoly3D;
	int nV = g_bmPoly3D.getNVert();
	if (nV + 3 >= c_PrimPoolSize) 
	{
		rsFlushPoly3D();
		nV = 0;
	}
	Vector3D& v1 = *((Vector3D*)&vit[nV]);
	Vector3D& v2 = *((Vector3D*)&vit[nV + 1]);
	Vector3D& v3 = *((Vector3D*)&vit[nV + 2]);

	v1 = a; v2 = b; v3 = c;

	vit.diffuse( nV		) = acol;
	vit.diffuse( nV + 1 ) = bcol;
	vit.diffuse( nV + 2 ) = ccol;

	g_bmPoly3D.setNVert( nV + 3 );
	g_bmPoly3D.setNPri( g_bmPoly3D.getNPri() + 1 );
} // rsPoly

void rsPoly( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD color )
{
	rsPoly( a, b, c, color, color, color );
} // rsPoly

void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
			 DWORD acol, DWORD bcol, DWORD ccol, DWORD dcol )
{
	rsPoly( a, b, c, acol, bcol, ccol );
	rsPoly( c, b, d, ccol, bcol, dcol );

} // rsQuad

void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, DWORD color )
{
	rsQuad( a, b, c, d, color, color, color, color );
} // rsQuad

void rsFlushPoly3D()
{
	RC::iRS->Draw( g_bmPoly3D );
	g_bmPoly3D.setNVert( 0 );
	g_bmPoly3D.setNPri( 0 );
} // rsFlushPoly3D


void DrawRay( const Ray3D& ray, DWORD linesColor, float rayLen, float handleSide )
{
	Vector3D dest( ray.getDir() );
	dest *= rayLen;
	dest += ray.getOrig();
	rsLine( ray.getOrig(), dest, linesColor );
	AABoundBox aabb( ray.getOrig(), handleSide );
	DrawAABB( aabb, 0, linesColor );
} // DrawRay

void DrawPlane( const Plane& plane, DWORD fillColor, DWORD color, const Vector3D* center, float qSide, float nLen )
{
	Vector3D c = center ? *center : plane.GetPoint();
	Vector3D n = plane.normal();
	Vector3D vx, vy;
	n.CreateBasis( vx, vy );

	Vector3D lt, lb, rt, rb;
	DrawRay( Ray3D( c, plane.normal() ), color, nLen, 1.0f );
	lt.addWeighted( vx, vy, -qSide, -qSide ); lt += c;
	rt.addWeighted( vx, vy,  qSide, -qSide ); rt += c;
	lb.addWeighted( vx, vy, -qSide,  qSide ); lb += c;
	rb.addWeighted( vx, vy,  qSide,  qSide ); rb += c;

	rsQuad( lt, rt, lb, rb, fillColor );

	rsLine( lt, rt, color, color );
	rsLine( rt, rb, color, color );
	rsLine( rb, lb, color, color );
	rsLine( lb, lt, color, color );

} // DrawPlane

void DrawTriangle( const Triangle& tri, DWORD linesColor, DWORD fillColor )
{
	if (fillColor) rsPoly( tri.a, tri.b, tri.c, fillColor );
	if (linesColor) 
	{
		rsLine( tri.a, tri.b, linesColor );
		rsLine( tri.b, tri.c, linesColor );
		rsLine( tri.a, tri.c, linesColor );
	}
} // DrawTriangle

void DrawFrustum( const Frustum& frustum, DWORD fillColor, DWORD linesColor, 
				 bool drawNormals )
{
	Vector3D ltn = frustum.ltn();
	Vector3D rtn = frustum.rtn();
	Vector3D lbn = frustum.lbn();
	Vector3D rbn = frustum.rbn();
	Vector3D ltf = frustum.ltf();
	Vector3D rtf = frustum.rtf();
	Vector3D lbf = frustum.lbf();
	Vector3D rbf = frustum.rbf();

	if (linesColor != 0)
	{
		rsLine( ltn, rtn, linesColor, linesColor );
		rsLine( lbn, rbn, linesColor, linesColor );
		rsLine( ltn, lbn, linesColor, linesColor );
		rsLine( rtn, rbn, linesColor, linesColor );

		rsLine( ltf, rtf, linesColor, linesColor );
		rsLine( lbf, rbf, linesColor, linesColor );
		rsLine( ltf, lbf, linesColor, linesColor );
		rsLine( rtf, rbf, linesColor, linesColor );

		rsLine( ltn, ltf, linesColor, linesColor );
		rsLine( lbn, lbf, linesColor, linesColor );
		rsLine( rtn, rtf, linesColor, linesColor );
		rsLine( rbn, rbf, linesColor, linesColor );
	}

	if (fillColor != 0)
	{
		rsQuad( ltn, rtn, lbn, rbn, fillColor );
		rsQuad( rtf, ltf, rbf, lbf, fillColor );
		rsQuad( ltf, rtf, ltn, rtn, fillColor );
		rsQuad( rbf, lbf, rbn, lbn, fillColor );
		rsQuad( ltf, ltn, lbf, lbn, fillColor );
		rsQuad( rtn, rtf, rbn, rbf, fillColor );
	}

	if (drawNormals)
	{
		Vector3D vn( lbn );
		vn += rbn; vn += rtn; vn += ltn; vn *= 0.25f;

		Vector3D vf( lbf );
		vf += rbf; vf += rtf; vf += ltf; vf *= 0.25f;

		Vector3D vt( ltn );
		vt += rtn; vt += rtf; vt += ltf; vt *= 0.25f;

		Vector3D vb( lbn );
		vb += rbn; vb += rbf; vb += lbf; vb *= 0.25f;

		Vector3D vl( lbf );
		vl += lbn; vl += ltn; vl += ltf; vl *= 0.25f;

		Vector3D vr( rbn );
		vr += rbf; vr += rtn; vr += rtf; vr *= 0.25f;

		Vector3D ndir( frustum.plNear.	normal() );
		Vector3D fdir( frustum.plFar.	normal() );
		Vector3D ldir( frustum.plLeft.	normal() );
		Vector3D rdir( frustum.plRight.	normal() );
		Vector3D tdir( frustum.plTop.	normal() );
		Vector3D bdir( frustum.plBottom.normal() );

		ndir.normalize();
		fdir.normalize();
		ldir.normalize();
		rdir.normalize();
		tdir.normalize();
		bdir.normalize();

		static const float c_NormalLen = 100.0f;

		ndir *= c_NormalLen;
		fdir *= c_NormalLen;
		ldir *= c_NormalLen;
		rdir *= c_NormalLen;
		tdir *= c_NormalLen;
		bdir *= c_NormalLen;

		ndir += vn;
		fdir += vf;
		ldir += vl;
		rdir += vr;
		tdir += vt;
		bdir += vb;

		rsLine( vn,	ndir, linesColor, linesColor );
		rsLine( vf,	fdir, linesColor, linesColor );
		rsLine( vl,	ldir, linesColor, linesColor );
		rsLine( vr,	rdir, linesColor, linesColor );
		rsLine( vt,	tdir, linesColor, linesColor );
		rsLine( vb, bdir, linesColor, linesColor );
	}

}// DrawFrustum

void DrawCube( const Vector3D& center, DWORD color, float side )
{
	AABoundBox aabb( center, side * 0.5f );
	DrawAABB( aabb, 0, color );
} // DrawCube

void DrawAABB( const AABoundBox& aabb, DWORD fillColor, DWORD linesColor )
{
	Vector3D ltn = Vector3D( aabb.minv.x, aabb.minv.y, aabb.maxv.z );
	Vector3D rtn = Vector3D( aabb.minv.x, aabb.maxv.y, aabb.maxv.z );
	Vector3D lbn = Vector3D( aabb.minv.x, aabb.minv.y, aabb.minv.z );
	Vector3D rbn = Vector3D( aabb.minv.x, aabb.maxv.y, aabb.minv.z );
	Vector3D ltf = Vector3D( aabb.maxv.x, aabb.minv.y, aabb.maxv.z );
	Vector3D rtf = Vector3D( aabb.maxv.x, aabb.maxv.y, aabb.maxv.z );
	Vector3D lbf = Vector3D( aabb.maxv.x, aabb.minv.y, aabb.minv.z );
	Vector3D rbf = Vector3D( aabb.maxv.x, aabb.maxv.y, aabb.minv.z );

	if (linesColor != 0)
	{
		rsLine( ltn, rtn, linesColor, linesColor );
		rsLine( lbn, rbn, linesColor, linesColor );
		rsLine( ltn, lbn, linesColor, linesColor );
		rsLine( rtn, rbn, linesColor, linesColor );

		rsLine( ltf, rtf, linesColor, linesColor );
		rsLine( lbf, rbf, linesColor, linesColor );
		rsLine( ltf, lbf, linesColor, linesColor );
		rsLine( rtf, rbf, linesColor, linesColor );

		rsLine( ltn, ltf, linesColor, linesColor );
		rsLine( lbn, lbf, linesColor, linesColor );
		rsLine( rtn, rtf, linesColor, linesColor );
		rsLine( rbn, rbf, linesColor, linesColor );
	}

	if (fillColor != 0)
	{
		rsQuad( ltn, rtn, lbn, rbn, fillColor );
		rsQuad( rtf, ltf, rbf, lbf, fillColor );
		rsQuad( ltf, rtf, ltn, rtn, fillColor );
		rsQuad( rbf, lbf, rbn, lbn, fillColor );
		rsQuad( ltf, ltn, lbf, lbn, fillColor );
		rsQuad( rtn, rtf, rbn, rbf, fillColor );
	}
} // DrawAABB

const float c_Circle8C[9] = 
{
	1.0f, 0.70710677f, 0.0f, -0.70710677f, -1.0f, -0.70710677f,	0.0f, 0.70710677f, 1.0f

};

const float c_Circle8S[9] = 
{
	0.0f, 0.70710677f, 1.0f, 0.70710677f, 0.0f, -0.70710677f, -1.0f, -0.70710677f, 0.0f
};

void DrawCircle8( const Vector3D& center, const Vector3D& normal, float radius, 
				 DWORD fillColor, DWORD linesColor )
{
	Matrix4D cTM;
	Vector3D z( normal );
	Vector3D x, y;
	z.CreateBasis( x, y );
	cTM.fromBasis( x, y, z, center );
	Vector3D a, b;

	for (int i = 0; i < 8; i++)
	{
		a.set( radius * c_Circle8C[i],		radius * c_Circle8S[i],		0.0f );
		b.set( radius * c_Circle8C[i + 1],	radius * c_Circle8S[i + 1], 0.0f );

		a *= cTM;
		b *= cTM;

		if (linesColor != 0)
		{
			rsLine( a, b, linesColor, linesColor );
		}

		if (fillColor != 0)
		{
			rsPoly( a, b, center, fillColor );
		}	
	}

} // DrawCircle8

void DrawCircle( const Vector3D& center, const Vector3D& normal, float radius, 
				DWORD fillColor, DWORD linesColor, int nSegments )
{
	Matrix4D cTM;
	Vector3D z( normal );
	Vector3D x, y;
	z.CreateBasis( x, y );
	cTM.fromBasis( x, y, z, center );

	float phiStep = c_DoublePI / float( nSegments );
	Vector3D a, b;

	for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
	{
		a.set( radius * cos( phi ), radius * sin( phi ), 0.0f );
		b.set( radius * cos( phi + phiStep), radius * sin( phi + phiStep ), 0.0f );

		a *= cTM;
		b *= cTM;

		if (linesColor != 0)
		{
			rsLine( a, b, linesColor, linesColor );
		}

		if (fillColor != 0)
		{
			rsPoly( a, b, center, fillColor );
		}	
	}

} // DrawCircle

void DrawSphere( const Sphere& sphere, DWORD fillColor, DWORD linesColor, int nSegments )
{
	float phiStep = 2.0f * c_PI / float( nSegments );
	float thetaStep = phiStep;

	Vector3D a, b, c, d;

	for (float phi = 0.0f; phi < c_PI; phi += phiStep)
	{
		for (float theta = 0.0f; theta < c_DoublePI; theta += thetaStep)
		{
			a.FromSpherical( sphere.GetRadius(), theta, phi );
			a += sphere.GetCenter();

			b.FromSpherical( sphere.GetRadius(), theta + thetaStep, phi );
			b += sphere.GetCenter();

			c.FromSpherical( sphere.GetRadius(), theta, phi + phiStep );
			c += sphere.GetCenter();

			d.FromSpherical( sphere.GetRadius(), theta + thetaStep, phi + phiStep );
			d += sphere.GetCenter();

			if (linesColor != 0)
			{
				rsLine( b, d, linesColor, linesColor );
				rsLine( c, d, linesColor, linesColor );
			}

			if (fillColor != 0)
			{
				if (theta < c_PI) rsQuad( b, a, d, c, fillColor );
				else rsQuad( a, b, c, d, fillColor );
			}
		}
	}
} // DrawSphere

void DrawSpherePatch( const Sphere& sphere, 
					 DWORD fillColor, DWORD linesColor, 
					 float phiBeg, float phiEnd,
					 float thetaBeg, float thetaEnd,
					 int nSegments, const Matrix4D* pTM )
{
	float phiStep = 2.0f * c_PI / float( nSegments );
	float thetaStep = phiStep;

	Vector3D a, b, c, d;

	for (float phi = phiBeg; phi < phiEnd; phi += phiStep)
	{
		for (float theta = thetaBeg; theta < thetaEnd; theta += thetaStep)
		{
			a.FromSpherical( sphere.GetRadius(), theta, phi );
			a += sphere.GetCenter();

			b.FromSpherical( sphere.GetRadius(), theta + thetaStep, phi );
			b += sphere.GetCenter();

			c.FromSpherical( sphere.GetRadius(), theta, phi + phiStep );
			c += sphere.GetCenter();

			d.FromSpherical( sphere.GetRadius(), theta + thetaStep, phi + phiStep );
			d += sphere.GetCenter();

			if (pTM)
			{
				a *= *pTM;
				b *= *pTM;
				c *= *pTM;
				d *= *pTM;
			}

			if (linesColor != 0)
			{
				rsLine( b, d, linesColor, linesColor );
				rsLine( c, d, linesColor, linesColor );
			}

			if (fillColor != 0)
			{
				if (theta < c_PI) rsQuad( b, a, d, c, fillColor );
				else rsQuad( a, b, c, d, fillColor );
			}
		}
	}
} // DrawSpherePatch

void DrawStar( const Sphere& sphere, DWORD begColor, DWORD endColor, int nSegments )
{
	float phiStep = 2.0f * c_PI / float( nSegments );
	float thetaStep = phiStep;

	for (float phi = phiStep; phi < c_PI; phi += phiStep)
	{
		for (float theta = 0.0f; theta < c_DoublePI; theta += thetaStep)
		{
			Vector3D pt;
			pt.FromSpherical( sphere.GetRadius(), theta, phi );
			pt += sphere.GetCenter();
			rsLine( sphere.GetCenter(), pt, begColor, endColor );
		}
	}
} // DrawStar

void DrawCylinder( const Cylinder& cylinder, 
				  DWORD fillColor, DWORD linesColor, bool bCapped,
				  int nSegments )
{
	float phiStep = c_DoublePI / float( nSegments );
	Vector3D a, b, c, d;

	float H = cylinder.GetHeight();
	float r = cylinder.GetRadius();
	Matrix4D cylTM( cylinder.GetWorldTM() );

	Vector3D top = cylinder.GetTop();
	Vector3D bottom = cylinder.GetBase();

	for (float phi = phiStep; phi < c_DoublePI; phi += phiStep)
	{
		a.set( r * cos( phi ), r * sin( phi ), 0.0f );
		b.set( a.x, a.y, H );
		c.set( r * cos( phi + phiStep), r * sin( phi + phiStep ), 0.0f );
		d.set( c.x, c.y, H );

		a *= cylTM;
		b *= cylTM;
		c *= cylTM;
		d *= cylTM;

		if (linesColor != 0)
		{
			rsLine( a, b, linesColor, linesColor );
			rsLine( a, c, linesColor, linesColor );
			rsLine( b, d, linesColor, linesColor );
		}

		if (fillColor != 0)
		{
			rsQuad( a, c, b, d, fillColor );
			if (bCapped)
			{
				rsPoly( c, a, bottom, fillColor );
				rsPoly( b, d, top, fillColor );
			}
		}	
	}

} // DrawCylinder

void DrawCone( const Cone& cone, DWORD fillColor, DWORD linesColor, int nSegments )
{
	float phiStep = c_DoublePI / float( nSegments );
	Vector3D a, b;

	Matrix4D coneTM( cone.GetWorldTM() );

	Vector3D top = cone.GetTop();
	float r = cone.GetBaseRadius();

	for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
	{
		a.set( r * cos( phi ), r * sin( phi ), 0.0f );
		b.set( r * cos( phi + phiStep), r * sin( phi + phiStep ), 0.0f );

		a *= coneTM;
		b *= coneTM;

		if (linesColor != 0)
		{
			rsLine( a, b,   linesColor, linesColor );
			rsLine( b, top, linesColor, linesColor );
			rsLine( top, b, linesColor, linesColor );
		}

		if (fillColor != 0)
		{
			rsPoly( a, b, top, fillColor );
		}	
	}
} // DrawCone

void DrawCapsule( const Capsule& capsule, 
				 DWORD fillColor, DWORD linesColor, 
				 int nSegments )
{
	Cylinder cylinder( capsule );
	DrawCylinder( cylinder, fillColor, linesColor, false, nSegments );
	Sphere sphere( Vector3D( 0.0f, 0.0f, 0.0f ), capsule.GetRadius() );

	Matrix4D tm = capsule.GetWorldTM();

	tm.setTranslation( capsule.GetTop() );
	DrawSpherePatch( sphere, fillColor, linesColor, 
		0.0f, c_DoublePI, 0.0f, c_HalfPI, 
		nSegments, &tm );

	tm.setTranslation( capsule.GetBase() );
	DrawSpherePatch( sphere, fillColor, linesColor, 
		0.0f, c_DoublePI, c_HalfPI, c_PI, 
		nSegments, &tm );
} // DrawCapsule

void DrawFatSegment( const Vector3D& beg, const Vector3D& end, const Vector3D& normal, 
						float width, bool bRoundEnds, DWORD color, DWORD coreColor )
{
	rsLine( beg, end, coreColor );
	Vector3D side, dir;
	dir.sub( end, beg );
	dir.normalize();
	side.cross( normal, dir );
	side.normalize();
	
	Vector3D a( beg ), b( beg ), c( end ), d( end );
	float hw = width * 0.5f;
	a.addWeighted( side,  hw );
	b.addWeighted( side, -hw );
	c.addWeighted( side,  hw );
	d.addWeighted( side, -hw );
	
	rsQuad( a, b, c, d, color );

	if (bRoundEnds)
	{
	
	}
} // DrawFatSegment

