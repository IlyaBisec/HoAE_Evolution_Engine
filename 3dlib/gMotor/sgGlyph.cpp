/*****************************************************************************/
/*	File:	sgGlyph.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-18-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgMovable.h"
#include "sgAssetNode.h"
#include "sgGeometry.h"
#include "sgShader.h"
#include "sgTexture.h"
#include "kIOHelpers.h"
#include "mAlgo.h"
#include "sgGlyph.h"

#ifndef _INLINES
#include "sgGlyph.inl"
#endif // !_INLINES

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	GlyphPage implementation
/*****************************************************************************/
GlyphPage::GlyphPage()
{
	GetPrimitive().setIsQuadList( true );
	m_Color			= 0xFFFFFF00;
	m_bScreenSpace	= true;
	m_bStatic		= false;

} // GlyphPage::GlyphPage

void GlyphPage::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "GlyphPage", this );
	pm.p( "Screen Space", IsScreenSpace, SetIsScreenSpace );
} // GlyphPage::Expose

void GlyphPage::Render()
{
	Geometry::Render();
	if (!IsStatic())
	{
		GetPrimitive().setNVert( 0 );
		GetPrimitive().setNInd ( 0 );
		GetPrimitive().setNPri ( 0 );
	}
} // GlyphPage::Render

void GlyphPage::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Glyphs << m_FreeLine << m_FreeSpace;
} // GlyphPage::Serialize

void GlyphPage::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_Glyphs >> m_FreeLine >> m_FreeSpace;
} // GlyphPage::Unserialize

GlyphPage::Glyph* GlyphPage::AllocateGlyph( DWORD code, float width, float m_Height )
{
	Glyph nGlyph;
	nGlyph.code = code;

	if (width == 0 || m_Height == 0) return NULL;
	
	if (m_FreeLine.w <= width + 8 || m_FreeLine.h < m_Height)
	{
		if (m_FreeSpace.h < m_Height || m_FreeSpace.w < width) return NULL;
		nGlyph.ext.x = 0.0f;
		nGlyph.ext.y = m_FreeSpace.y;
		nGlyph.ext.w = width;
		nGlyph.ext.h = m_Height;

		m_FreeLine.w	=  m_FreeSpace.w;
		m_FreeLine.h	=  m_Height;
		m_FreeLine.x	=  width;
		m_FreeLine.y	=  m_FreeSpace.y;

		m_FreeSpace.y += m_Height;
		m_FreeSpace.h -= m_Height;
	}
	else
	{
		nGlyph.ext.x = m_FreeLine.x;
		nGlyph.ext.y = m_FreeLine.y;
		nGlyph.ext.w = width;
		nGlyph.ext.h = m_Height;

		m_FreeLine.w -= width;
		m_FreeLine.x += width;
	}
	
	nGlyph.uv.x = nGlyph.ext.x / float( m_TexWidth );
	nGlyph.uv.w = nGlyph.ext.w / float( m_TexWidth );
	nGlyph.uv.y = nGlyph.ext.y / float( m_TexHeight );
	nGlyph.uv.h = nGlyph.ext.h / float( m_TexHeight );

	m_Glyphs.push_back( nGlyph );
	return &m_Glyphs[m_Glyphs.size() - 1];
} // GlyphPage::AllocateGlyph

const float c_HalfPixel = 0.5f;

Rct* GlyphPage::AddGlyph( int GlyphIdx, const Vector3D& pos, DWORD clr, const Matrix4D& tm )
{
	Glyph& c = m_Glyphs[GlyphIdx];	

	BaseMesh&	bm = GetPrimitive();
	int			nV = bm.getNVert();
	if (nV + 4 > bm.getMaxVert()) return NULL;

	bm.setNVert( nV + 4 );
	bm.setNInd ( bm.getNInd() + 6 );
	bm.setNPri ( bm.getNPri() + 2 );

	if (bm.getVertexFormat() == vfTnL)
	{
		VertexTnL* v = (VertexTnL*) bm.getVertexData();
		v += nV;
		VertexTnL& v0 = v[0];
		VertexTnL& v1 = v[1];
		VertexTnL& v2 = v[2];
		VertexTnL& v3 = v[3];
		
		v0.x = pos.x - c_HalfPixel; 
		v0.y = pos.y - c_HalfPixel; 
		v0.z = pos.z; 
		
		v0.u = c.uv.x; 
		v0.v = c.uv.y;
		
		v1.x = v0.x + c.ext.w; 
		v1.y = v0.y; 
		v1.z = pos.z;
		
		v1.u = c.uv.GetRight(); 
		v1.v = c.uv.y;

		v2.x = v0.x; 
		v2.y = v0.y + c.ext.h; 
		v2.z = pos.z;
		
		v2.u = c.uv.x; 
		v2.v = c.uv.GetBottom();

		v3.x = v1.x; 
		v3.y = v2.y; 
		v3.z = pos.z;
		
		v3.u = v1.u; 
		v3.v = v2.v;

		v0.diffuse = clr;
		v1.diffuse = clr;
		v2.diffuse = clr;
		v3.diffuse = clr;

		v0.w = 1.0f;
		v1.w = 1.0f;
		v2.w = 1.0f;
		v3.w = 1.0f;

		*((Vector3D*)&v[0]) *= tm;
		*((Vector3D*)&v[1]) *= tm;
		*((Vector3D*)&v[2]) *= tm;
		*((Vector3D*)&v[3]) *= tm;
	}

	return &(m_Glyphs[GlyphIdx].ext);
} // GlyphPage::AddGlyph

void GlyphPage::ClearMesh()
{
	GetPrimitive().setNVert( 0 );
	GetPrimitive().setNInd ( 0 );
	GetPrimitive().setNPri ( 0 );
}

void GlyphPage::ClearGlyphs()
{
	m_Glyphs.clear();
	m_FreeSpace.Set( 0.0f, 0.0f, c_GlyphTextureWidth, c_GlyphTextureHeight );
	m_FreeLine.Zero();
}

Texture* GlyphPage::GetTexture()
{
	Iterator it( this );
	while (it)
	{
		Node* pNode = (Node*)it;
		if (pNode->HasFn( Texture::Magic() )) return (Texture*)pNode;
		++it;
	}
	return NULL;
}

bool GlyphPage::CreatePage( float width, float height, const char* texFileName )
{
	Texture*  pTex	= AddChild<Texture>( GetName() );
	
	if (texFileName == NULL || texFileName[0] == 0) texFileName = GetName();
	if (!texFileName) return false;

	int texID = IRS->GetTextureID( texFileName );

	if (texID == -1)
	{
		m_TexWidth  = width;
		m_TexHeight = height;

		pTex->SetStage			( 0						);
		pTex->SetWidth			( m_TexWidth			);
		pTex->SetHeight			( m_TexHeight			);
		pTex->SetColorFormat	( c_GlyphColorFormat	);
		pTex->SetMemoryPool		( mpManaged				);
		pTex->SetIsProcedural	( true					);
		pTex->SetNMips			( 1						);
		
		pTex->CreateTexture();
	}
	else
	{
		pTex->SetTexID( texID );
		m_TexWidth  = pTex->GetWidth();
		m_TexHeight = pTex->GetHeight();
	}

	
	m_FreeSpace.Set( 0.0f, 0.0f, m_TexWidth, m_TexHeight );
	m_FreeLine.Zero();

	Create( c_GlyphBucketSize*4, 0, vfTnL, ptTriangleList );
	BaseMesh& bm = GetPrimitive();
	VertexTnL* pV = (VertexTnL*)bm.getVertexData();
	for (int i = 0; i < bm.getMaxVert(); i++)
	{
		pV[i].w = 1.0f;
	}

	return true;
} // GlyphPage::CreatePage

OutStream& operator << ( OutStream& os, const GlyphPage::Glyph& ch )
{
	os << ch.code << ch.ext;
	return os;
}

InStream&  operator >> ( InStream& is, GlyphPage::Glyph& ch )
{
	is >> ch.code >> ch.ext;
	return is;
}

bool GlyphPage::InitFromXML( const char* fileName )
{
	return false;	
}

int	GlyphPage::CreateGlyph( const char* name )
{
	Glyph glyph;
	int len = min( strlen( name ), c_MaxGlyphNameLen - 1 );
	strncpy( glyph.name, name, len );
	glyph.name[len] = 0;
	m_Glyphs.push_back( glyph );
	return m_Glyphs.size() - 1;
} // GlyphPage::CreateGlyph

bool GlyphPage::SetGlyphRect( int id, const Rct& rct )
{
	if (id < 0 || id >= m_Glyphs.size()) return false;
	m_Glyphs[id].ext = rct;
	m_Glyphs[id].uv  = rct;
	m_Glyphs[id].uv.x /= m_TexWidth;
	m_Glyphs[id].uv.w /= m_TexWidth;
	m_Glyphs[id].uv.y /= m_TexWidth;
	m_Glyphs[id].uv.h /= m_TexWidth;

	return true;
} // GlyphPage::SetGlyphRect

void GlyphPage::SetGlyphTexture( const char* texName )
{
	Texture* pTex = AddChild<Texture>( texName );
	DeviceStateSet* pDSS = AddChild<DeviceStateSet>( "decals" );

	m_TexWidth	= pTex->GetWidth();
	m_TexHeight = pTex->GetHeight();
} // GlyphPage::SetGlyphTexture

/*****************************************************************************/
/*	GlyphSet implementation
/*****************************************************************************/
GlyphSet::GlyphSet()
{
	m_TM.setIdentity();
}

void GlyphSet::CleanPageBuckets()
{
	for (int i = 0; i < m_Pages.size(); i++)
	{
		m_Pages[i]->GetPrimitive().setNVert( 0 );
		m_Pages[i]->GetPrimitive().setNInd ( 0 );
		m_Pages[i]->GetPrimitive().setNPri ( 0 );
	}
}

int GlyphSet::GetPageID( const char* pageName )
{
	for (int i = 0; i < GetNPages(); i++)
	{
		if (!strcmp( pageName,  GetPage( i )->GetName() ) )
		{
			return i;
		}
	}
	return -1;
} // GlyphSet::GetPage

GlyphPage* GlyphSet::CreatePage( float width, float height, const char* texFileName )
{
	char name[64];
	sprintf( name, "%s_page%d", GetName(), GetNPages() );
	GlyphPage* pPage = AddChild<GlyphPage>( name );
	pPage->CreatePage( width, height, texFileName );
	m_Pages.push_back( pPage );
	return pPage;
} // Font::GlyphSet

void GlyphSet::AddGlyph( const Vector3D& pos, DWORD code, DWORD color )
{
	int cPage = 0;
	for (int i = 0; i < GetNPages(); i++)
	{
		GlyphPage* pPage = GetPage( i );
		int glyphIdx = pPage->FindGlyph( code );
		if (glyphIdx == -1) continue;
		const Rct* pCharExt = pPage->AddGlyph( glyphIdx, pos, color, m_TM );

		if (!pCharExt) 
		{
			Render();
			pCharExt = pPage->AddGlyph( glyphIdx, pos, color, m_TM );
		}
	}
} // GlyphSet::AddGlyph

GlyphPage* GlyphSet::AddPage( const char* pageName )
{
	GlyphPage* pRes = AddChild<GlyphPage>( pageName );
	m_Pages.push_back( pRes );
	return pRes;
}


bool GlyphSet::InitFromXML( const char* fileName )
{
	return false;
} // GlyphSet::InitFromXML

bool GlyphSet::FindGlyph( const char* name, int& page, int& id ) const
{
	return false;
}

/*****************************************************************************/
/*	IconSet implementation
/*****************************************************************************/
void IconSet::Serialize( OutStream&	os ) const
{
	Parent::Serialize( os );
	os << m_IconWidth << m_IconHeight;
}

void IconSet::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_IconWidth >> m_IconHeight;
}

void IconSet::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "IconSet", this );
	pm.f( "IconWidth",	m_IconWidth );
	pm.f( "IconHeight",	m_IconHeight );
}

bool IconSet::AddTexture( const char* texName )
{
	AddChild<DeviceStateSet>( "icons" );

	GlyphPage* pPage = CreatePage( 0, 0, texName );
	if (!pPage) return false;
	float cY = 0.0f;

	if (m_IconWidth == 0 || m_IconHeight == 0) return false;
	
	int cGlyph = 0;
	while (cY < pPage->GetTexHeight())
	{
		float cX = 0.0f;
		while (cX < pPage->GetTexWidth())
		{
			pPage->AllocateGlyph( cGlyph, m_IconWidth, m_IconHeight );
			cX += m_IconWidth;
			cGlyph++;
		}
		cY += m_IconHeight;
	}
	return true;
} // IconSet::AddTexture

END_NAMESPACE(sg)
