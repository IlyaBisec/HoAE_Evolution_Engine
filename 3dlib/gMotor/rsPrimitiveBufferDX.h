/*****************************************************************************/
/*	File:	PrimitiveBufferDX.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	26.12.2002
/*****************************************************************************/
#ifndef __PRIMITIVEBUFFER_H__
#define __PRIMITIVEBUFFER_H__

class BaseMesh;

/*****************************************************************************/
/*	Class:	IndexBuffer
/*	Desc:	wrapper for index buffer functionality
/*****************************************************************************/
class IndexBuffer
{
public:
	DXIndexBuffer*				m_pBuffer;	//  dx buffer interface

	int							m_MaxIdx;	//  maximal index in buffer
	int							m_MaxByte;	//  maximal bytes in buffer
	int							m_CurIdx;	//	current index in buffer
	int							m_CurByte;	//  current byte in buffer

					IndexBuffer ();
	void			Release		();
	virtual bool	Create		( int nInd ) = 0;
	virtual int		FillIndices	( WORD* idxPtr, int nInd, BaseMesh& bm ) = 0;

}; // class IndexBuffer

/*****************************************************************************/
/*	Class:	StaticIB
/*	Desc:	
/*****************************************************************************/
class StaticIB : public IndexBuffer
{
public:

	virtual bool 	Create		( int nInd );
	virtual int		FillIndices	( WORD* idxPtr, int nInd, BaseMesh& bm );
}; // class StaticIB

/*****************************************************************************/
/*	Class: DynamicIB
/*	Desc:
/*****************************************************************************/
class DynamicIB : public IndexBuffer
{
public:

	virtual bool 	Create		( int nInd );
	virtual int		FillIndices	( WORD* idxPtr, int nInd, BaseMesh& bm );
}; // class StaticIB

/*****************************************************************************/
/*	Class:	SharedIB
/*	Desc:	
/*****************************************************************************/
class SharedIB : public StaticIB
{
public:

	virtual int		FillIndices	( WORD* idxPtr, int nInd, BaseMesh& bm );
}; // class SharedIB

/*****************************************************************************/
/*	Class:	QuadIB
/*	Desc:
/*****************************************************************************/
class QuadIB : public SharedIB
{
public:

	virtual bool Create( int nInd );
}; // class QuadIB

const int c_DefaultStride = 32;

/*****************************************************************************/
/*	Class:	VertexBuffer
/*	Desc:
/*****************************************************************************/
class VertexBuffer
{
public:	
	DXVertexBuffer*				m_pBuffer;
	
	int							m_MaxByte;
	int							m_CurByte;
	int							m_CurVert;
	
	int							m_CurFVF;
	int							m_CurStride;
    bool                        m_bDynamic;

					VertexBuffer();
	virtual void	Create		( int nBytes, VertexFormat vf ) = 0;
	virtual int		FillVerts	( void* vert, int nVert, BaseMesh& bm ) = 0;
	void			Release		();

private:
	friend class	PrimitiveBuffer;
	
}; // class VertexBuffer

/*****************************************************************************/
/*	Class:	StaticVB
/*	Desc:	Wrapper for static vertex buffers
/*****************************************************************************/
class StaticVB : public VertexBuffer
{
public:
	virtual void	Create		( int nBytes, VertexFormat vf );
	virtual int		FillVerts	( void* vert, int nVert, BaseMesh& bm );


}; // class StaticVB

/*****************************************************************************/
/*	Class:	DynamicVB
/*	Desc:	Wrapper for dynamic vertex buffers
/*****************************************************************************/
class DynamicVB : public VertexBuffer
{
public:
	virtual void	Create		( int nBytes, VertexFormat vf );
	virtual int		FillVerts	( void* vert, int nVert, BaseMesh& bm );

}; // class DynamicVB

/*****************************************************************************/
/*	Class:	PrimitiveBuffer
/*	Desc:
/*****************************************************************************/
class PrimitiveBuffer
{
	VertexBuffer*				m_pVB;
	IndexBuffer*				m_pIB;

	DWORD						m_FVF;
	VertexFormat				m_VertexFormat;
	int							m_Stride;

	friend class				PrimitiveCache;
	friend class				VertexBuffer;

public:
				PrimitiveBuffer();

protected:
	bool		AdjustVBSize		();	
	bool		Draw				( BaseMesh& bm );
	void		SetVertexFormat		( VertexFormat vertF );
	void		AttachVB			( VertexBuffer* pVB )	{ m_pVB = pVB; }
	void		AttachIB			( IndexBuffer* pIB )	{ m_pIB = pIB; }
	
	static DWORD				s_CurFVF;
}; // class PrimitiveBuffer

const int c_StaticVBufferBytes			= 16384 * 32;
const int c_DynamicVBufferBytes			= 65535 * 64;

const int c_DynamicBufferInd			= 65535 * 2;
const int c_StaticBufferInd				= 16384 * 4;
const int c_QuadBufferInd				= 65535 * 2;

const int c_IBStride					= 2;

/*****************************************************************************/
/*	Class:	PrimitiveCache
/*	Desc:	Contains and manages primitive buffers
/*****************************************************************************/
class PrimitiveCache
{
public:
	void					Init();
	void					Shut();

	bool					InvalidateDeviceObjects();
	bool					RestoreDeviceObjects();
	bool					Draw( BaseMesh& bm ); 

private:

	QuadIB					m_QuadIB;		// static index buffer for quad data
	StaticIB				m_StaticIB;		// static index buffer for any data
	DynamicIB				m_DynamicIB;    // dynamic index buffer for any data
	StaticVB				m_StaticVB;
	DynamicVB				m_DynamicVB;
	DWORD					m_CurFVF;

	PrimitiveBuffer			m_Quad		[c_NumVertexTypes];
	PrimitiveBuffer			m_Static	[c_NumVertexTypes];
	PrimitiveBuffer			m_Dynamic	[c_NumVertexTypes];
}; // class PrimitiveCache

DWORD				VertexFormatFVF	( VertexFormat	vf		);
_inl D3DPRIMITIVETYPE	PriTypeDX		( PrimitiveType priType );

#endif // __PRIMITIVEBUFFER_H__
