/*****************************************************************************/
/*	File:	gpBitmap.h
/*	Desc:	Static bitmap treatment
/*	Author:	Ruslan Shestopalyuk
/*	Date:	27.02.2003
/*****************************************************************************/
#ifndef __GPBITMAP_H__
#define __GPBITMAP_H__

struct BMPFormat;

namespace sg{

const int c_BmpQuadSize = 256;
/*****************************************************************/
/*  Class:	Bitmap
/*  Desc:	Bitmap class
/*****************************************************************/
class Bitmap
{
public:
	Bitmap( const char* fname );
	Bitmap();
	~Bitmap();

	bool				Load( const char* fname );

	_inl int			GetW() const; 
	_inl int			GetH() const;
	_inl const BYTE*	GetPixelData() const;
	_inl void			SetPixelData( BYTE* dta ) const {  }
	_inl int			GetDataSize() const;
	_inl ColorFormat	GetColFmt() const;
	_inl WORD			GetPixel( int x, int y ) const;
	_inl int			GetLineSize() const;
	_inl int			GetBytesPerPixel() const;
	_inl int			GetRealWidth() const;
	_inl int			GetRealHeight() const;

protected:
	bool				LoadBMP24bit( ResFile& rf );
	bool				LoadBMP8bit( ResFile& rf, int clrNum );

	int				lx;
	int				ly;
	ColorFormat		cf;
	BYTE*			pix;
	std::string		m_Name;
};  // class Bitmap

class GPBitmap;
/*****************************************************************************/
/*	Class:	GPBitmapChunk
/*****************************************************************************/
class GPBitmapChunk : public BaseResource
{
public:
	GPBitmapChunk() : BaseResource(), bScreenSpace( true ) {}
	GPBitmapChunk( GPBitmap* _papa, 
					int _relX, int _relY, 
					int _width, int _height );

	~GPBitmapChunk();

	_inl void			Set( GPBitmap* _papa, int _relX, int _relY, int _width, int _height, bool screenSpace = true );
	_inl void			PreDraw( float x, float y, float z ) { papaX = x; papaY = y; papaZ = z; }
	
	virtual int			GetDismissableSizeBytes() const;
	virtual int			GetHeaderSizeBytes() const;
	
	virtual bool		InitPrefix();
	virtual void		Dismiss();
	virtual bool		Restore();
	virtual void		Dump();
	void				Draw( float x, float y, float z );
	
private:
	int					relX, relY;
	int					width, height;
	GPBitmap*			papa;
	int					texID;
	int					pixCacheItemID;
	bool				shouldBeDrawn;
	float				papaX, papaY, papaZ;
	bool				bScreenSpace;

	static BaseMesh		s_ChunkBM;
	static TextureDescr	s_BmpTD;
}; // class GPBitmapChunk

/*****************************************************************/
/*  Class:	GPBitmap
/*  Desc:	compound bitmap class, used for huge background pictures
/*****************************************************************/
class GPBitmap : public Bitmap
{
public:
	GPBitmap( bool _bScreenSpace = true ) : 
	  x(0), y(0), z(0), width(0), height(0), Bitmap(), bScreenSpace( _bScreenSpace ) {}
	GPBitmap( const char* fname, bool _bScreenSpace = true ) : 
	  x(0), y(0), z(0), Bitmap( fname ), bScreenSpace( _bScreenSpace ) {}

    int         GetCW() const { return width; }
    int         GetCH() const { return height; }

	bool		Load( const char* fname, bool screenSpace );

	bool		IsEmpty() const 
	{ 
		return (chunk.size() == 0); 
	}
	void		Draw( float _x, float _y, float _z = 0.0f );

protected:
	static int	shaBitmap;

private:
	int		x, y, z;			//  current left top corner position
    int     width, height;		//  current width & height
	bool	bScreenSpace;

	std::vector<GPBitmapChunk>	chunk;
};  // class GPBitmap

}; // namespace sg

#ifdef _INLINES
#include "gpBitmap.inl"
#endif // _INLINES

#endif // __GPBITMAP_H__