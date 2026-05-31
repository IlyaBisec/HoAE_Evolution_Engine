/*****************************************************************************/
/*	File:	gpBitmap.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	27.02.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kHash.hpp"
#include "kResource.h"
#include "kBmpTool.h"
#include "gpPackage.h"
#include "kUtilities.h"
#include "gpBitmap.h"

#ifndef _INLINES
#include "gpBitmap.inl"
#endif // _INLINES

BEGIN_NAMESPACE(sg)
/*****************************************************************/
/*  Bitmap implementation
/*****************************************************************/
Bitmap::Bitmap()
{
	pix = 0;
	lx = 0;
	ly = 0;
	cf = cfUnknown;
}

Bitmap::Bitmap( const char* fname )
{
	pix = 0;
	Load( fname );
}

Bitmap::~Bitmap()
{
	delete []pix;
	pix = 0;
}

bool Bitmap::LoadBMP24bit( ResFile& rf )
{	
	int wb	= lx * 3;
	int rwb = wb;
	if (wb & 3) rwb = (wb | 3) + 1;
	pix = new BYTE[wb * ly * 3];

	for (int i = 0; i < ly; i++)
	{
		RSeek( rf, sizeof( BMPformat ) + (ly - i - 1) * rwb ); 
		RBlockRead( rf, &pix[i * wb], wb );
	}

	//  bgr to rgb
	int sz = GetDataSize();
	BYTE* tmp  = new BYTE[sz];
	int numPix = lx * ly;
	for (int i = 0; i < numPix; i++)
	{
		tmp[i * 2 + 1] = 0xF0 | (pix[i * 3 + 2] >> 4);	// XR
		tmp[i * 2 + 0] =	(pix[i * 3 + 1] & 0xF0) | 
							(pix[i * 3 + 0] >> 4);		// GB
	}

	delete []pix;
	pix = tmp;

	RClose( rf );
	return true;
}

bool Bitmap::LoadBMP8bit( ResFile& rf, int clrNum )
{
	BYTE*	pal = new BYTE[clrNum * 4];
	RBlockRead( rf, pal, clrNum * 4 );
	
	int wb	= lx;
	int rwb = wb;
	if (wb & 3) rwb = (wb | 3) + 1;
	pix = new BYTE[wb * ly];

	for (int i = 0; i < ly; i++)
	{
		RSeek( rf, sizeof( BMPformat ) + (ly - i - 1) * rwb ); 
		RBlockRead( rf, &pix[i * wb], wb );
	}

	//  bgr to rgb
	int sz = GetDataSize();
	BYTE* tmp  = new BYTE[GetDataSize()];
	int numPix = lx * ly;
	for (int i = 0; i < numPix; i++)
	{
		DWORD* bgrx = (DWORD*)(pal + ((int)pix[i])*4);
		((WORD*)tmp)[i]=0xF000|((*bgrx>>4)&15)|(((*bgrx>>12)&15)<<4)|(((*bgrx>>20)&15)<<8);
	}
	delete []pix;
	delete []pal;
	pix = tmp;

	RClose( rf );
	return true;
}

/*---------------------------------------------------------------------------*/
/*	Func:	Bitmap::Load	
/*	Desc:	Loads static bitmap from .bmp file
/*	Parm:	fname - name of the file
/*	Ret:	true if OK
/*	TODO:	make it less lame
/*---------------------------------------------------------------------------*/
bool Bitmap::Load( const char* fname )
{
	ResFile rf = RReset( fname );
	if (rf == INVALID_HANDLE_VALUE)
	{
		Log.Error( "Couldn't open bitmap file: %s", fname );
		return false;
	}

	m_Name = fname;

	BMPformat head;
	RBlockRead( rf, &head, sizeof( BMPformat ));

    const char bmType[] = "BM";
	if (head.bfType != *((WORD*)bmType)) return false;

	if (head.biBitCount != 24 && head.biBitCount != 8) return false;

	delete []pix; 
	lx		= head.biWidth;
	ly		= head.biHeight;
	cf		= cfARGB4444; 
	
	if (head.biBitCount == 24) return LoadBMP24bit( rf );
	if (head.biBitCount == 8) return LoadBMP8bit( rf, 256 );
	
	return false;
}

/*****************************************************************************/
/*	GPBitmapChunk implementation
/*****************************************************************************/
BaseMesh		GPBitmapChunk::s_ChunkBM;
TextureDescr	GPBitmapChunk::s_BmpTD;

GPBitmapChunk::GPBitmapChunk(	GPBitmap* _papa, 
								int _relX, int _relY, 
								int _width, int _height ) 
							: BaseResource()
{
	papa	= _papa;
	width	= _width;
	height	= _height;
	relX	= _relX;
	relY	= _relY;

	pixCacheItemID	= -1;
	texID			= -1;

	shouldBeDrawn	= false;
	bScreenSpace	= true;
}

GPBitmapChunk::~GPBitmapChunk()
{
	if (pixCacheItemID >= 0 && !IsDismissed()) 
			GPSeq::s_PixelCache.FreeItem( pixCacheItemID );
}
	
int	GPBitmapChunk::GetDismissableSizeBytes() const
{
	assert( false );
	return width * height * 2;
} // GPBitmapChunk::GetDismissableSizeBytes

int	GPBitmapChunk::GetHeaderSizeBytes() const
{
	return sizeof( *this );
} // GPBitmapChunk::GetHeaderSizeBytes
	
bool GPBitmapChunk::InitPrefix()
{
	return true;
} // GPBitmapChunk::InitPrefix

void GPBitmapChunk::Dump()
{
	Log.Info( "GPBitmapChunk, relX:%d relY:%d texID:%d", relX, relY, texID );
}

void GPBitmapChunk::Dismiss()
{
	if (shouldBeDrawn)
	//  could be in case of thrashing texture cache surface 
	{
		Draw( papaX, papaY, papaZ );
	}
	dismissed = true;
} // GPBitmapChunk::Dismiss

bool GPBitmapChunk::Restore()
{
	assert( papa );
    //  copy to the texture surface
    int lineSize	= papa->GetLineSize();
	int stride		= papa->GetBytesPerPixel();
	int pitch		= 0;

	pixCacheItemID	= GPSeq::s_PixelCache.AllocCacheItem( this );
	texID			= GPSeq::s_PixelCache.GetItemTextureSurfID( pixCacheItemID );
	
	if (s_ChunkBM.getMaxVert() == 0)
	{
		s_BmpTD.setValues( c_BmpQuadSize, c_BmpQuadSize, cfARGB4444, mpSysMem, 1, tuProcedural );
		s_ChunkBM.create( 4, 0, vfTnL );
		s_ChunkBM.setIsQuadList( true );
	}
	
	//  copy to the working surface
    BYTE* pB = RC::iRS->LockTexBits( texID, pitch );
	assert( pB );
    const BYTE* pPix = papa->GetPixelData();
    pPix += relY * lineSize + relX * stride;
    util::MemcpyRect( pB, pitch, pPix, lineSize, height, width * stride );	
	RC::iRS->UnlockTexBits( texID );

	// -----------------
	//char buf[64];
	//sprintf( buf, "c:\\dumps\\bmTex%d.bmp", texID );
	//RC::iRS->SaveTexture( texID, buf );
	// -----------------

	Hit( BaseResource::curCacheFactor );
	dismissed = false;
	return true;
} // GPBitmapChunk::Restore

void GPBitmapChunk::Draw( float x, float y, float z )
{
	Touch();
	
	if (bScreenSpace)
	{
		static int shBmp = RC::iRS->GetShaderID( "hud" );
		s_ChunkBM.setShader( shBmp );

		s_ChunkBM.createPrQuad( x + relX, y + relY, width, height, z );
		VertexTnL* vert = (VertexTnL*)s_ChunkBM.getVertexData();

	    vert[0].u = 0.0f;
	    vert[0].v = 0.0f;

	    vert[1].u = 1.0f;
	    vert[1].v = 0.0f;

	    vert[2].u = 0.0f;
	    vert[2].v = 1.0f;

		vert[3].u = vert[1].u;
	    vert[3].v = vert[2].v;

		vert[0].w = 1.0f;
		vert[1].w = 1.0f;
		vert[2].w = 1.0f;
		vert[3].w = 1.0f;

		vert[0].diffuse = 0xFFFFFFFF;
		vert[1].diffuse = 0xFFFFFFFF;
		vert[2].diffuse = 0xFFFFFFFF;
		vert[3].diffuse = 0xFFFFFFFF;
	}
	else
	{
		static int shBmpLF = RC::iRS->GetShaderID( "hudLF" );
		s_ChunkBM.setShader( shBmpLF );

		s_ChunkBM.createQuad(	Vector3D( x + relX,			y + relY, z ),
								Vector3D( x + relX + width, y + relY, z ),
								Vector3D( x + relX, y + relY + height, z ),
								Vector3D( x + relX + width, y + relY + height, z ) 
								);
		Vertex2t* vert = (Vertex2t*)s_ChunkBM.getVertexData();

		vert[0].u = 0.0f;
		vert[0].v = 0.0f;

		vert[1].u = 1.0f;
		vert[1].v = 0.0f;

		vert[2].u = 0.0f;
		vert[2].v = 1.0f;

		vert[3].u = vert[1].u;
		vert[3].v = vert[2].v;

		vert[0].diffuse = 0xFFFFFFFF;
		vert[1].diffuse = 0xFFFFFFFF;
		vert[2].diffuse = 0xFFFFFFFF;
		vert[3].diffuse = 0xFFFFFFFF;
	}

	Hit( BaseResource::curCacheFactor );
	s_ChunkBM.setTexture( texID );

	RC::iRS->Draw( s_ChunkBM );

	shouldBeDrawn = false;
}; // GPBitmapChunk::Draw

/*****************************************************************/
/*  GPBitmap implementation
/*****************************************************************/
bool GPBitmap::Load( const char* fname, bool screenSpace )
{
	bScreenSpace = screenSpace;
	chunk.clear();
	return Bitmap::Load( fname );
}

void GPBitmap::Draw( float _x, float _y, float _z )
{
	x = _x; y = _y; z = _z;
	if (GetColFmt() != cfARGB4444) 
	{
		Log.Error( "GPBitmap::Draw - unsupported color format: %d.", GetColFmt() );
		return;
	}

	if (IsEmpty())
	{
		int realW = GetRealWidth();
		int realH = GetRealHeight();
		int chunksW = realW / c_BmpQuadSize;
		if (realW % c_BmpQuadSize > 0) chunksW++;
		int chunksH = realH / c_BmpQuadSize;
		if (realH % c_BmpQuadSize > 0) chunksH++;
		chunk.resize( chunksW * chunksH );
		int cChunk = 0;
		for (int cx = 0; cx < realW; cx += c_BmpQuadSize)
		{
			for (int cy = 0; cy < realH; cy += c_BmpQuadSize)
			{
				int cw = min( c_BmpQuadSize, GetRealWidth() - cx );
				int ch = min( c_BmpQuadSize, GetRealHeight() - cy );
				GPBitmapChunk&	bmChunk = chunk[cChunk];
				bmChunk.Set( this, cx, cy, cw, ch, bScreenSpace );
				cChunk++;
			}
		}
		assert( cChunk == chunksW * chunksH );
	}
	
	if (IsEmpty()) return;
	
	int nChunks = chunk.size();
	for (int i = 0; i < nChunks; i++) chunk[i].PreDraw( x, y, z );
	for (int i = 0; i < nChunks; i++)
	{
		chunk[i].Draw( x, y, z );
	}
} // GPBitmap::Draw

END_NAMESPACE(sg)


