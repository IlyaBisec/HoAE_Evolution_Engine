/*****************************************************************
/*  File:   gpMesh.inl                                           *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Feb 2002                                             *
/*****************************************************************/
#include "kColorValue.h"
#include "kLog.h"

const BYTE						c_TexCached = 0x01;
const BYTE						c_IBCached	= 0x02;
const BYTE						c_VBCached	= 0x04;
const BYTE						c_QuadList	= 0x08;
const BYTE						c_Static	= 0x10;

_inl void operator <<( VertexIterator& it, const Primitive& bm )
{
	it.reset( bm.getVertexData(), bm.getNVert(), 
                CreateVertexDeclaration( bm.getVertexFormat() ) );
}

/*****************************************************************
/*	Primitive implementation
/*****************************************************************/
_finl BYTE*	Primitive::getVertexData() const
{ 
	return reinterpret_cast<BYTE*>( vbuf ); 
}

_finl BYTE* Primitive::getVertexData( int vIdx) const
{
	return reinterpret_cast<BYTE*>( vbuf )
				+ vIdx * Vertex::GetStride( (VertexFormat)_vertFormat );
}

_finl BYTE* Primitive::getVNormalData( int vIdx)const
{
     assert( getVertexFormat() == vfVertexN );
     return reinterpret_cast<BYTE*>( vbuf )
				+ vIdx * Vertex::GetStride( (VertexFormat)_vertFormat ) +
                sizeof( float ) * 3;
}

_finl int Primitive::getVBufSizeBytes() const 
{ 
	return vertNum * Vertex::GetStride( (VertexFormat)_vertFormat ); 
}

_finl VertexFormat Primitive::getVertexFormat() const 
{ 
	return (VertexFormat)_vertFormat; 
}

_finl int Primitive::getNVert() const		
{ 
	return vertNum; 
}

_finl int Primitive::getMaxVert()	const	
{ 
	return maxVertNum; 
}

_finl int Primitive::getMaxInd()	const	
{ 
	return maxIndNum; 
}

_finl int Primitive::getNInd() const		
{ 
	return indNum;
}

_finl int Primitive::getNPri() const
{
	return numPri;
}

_finl WORD* Primitive::getIndices() const
{
	return ibuf;
}

_finl PrimitiveType Primitive::getPriType() const
{
	return (PrimitiveType)_priType;
}

_finl int Primitive::getShader()	 const 
{ 
	return shaderID; 
}
	
_finl int Primitive::getTexture( int stage ) const 
{ 
	assert( stage >= 0 && stage < 3 );
	return texID[stage]; 
}

_finl int  Primitive::getIBPos() const	
{ 
	return ibPos; 
}

_finl int  Primitive::getVBPos() const	
{ 
	return vbPos; 
}

_finl void* Primitive::getDevHandle()	const
{
	return devHandle;
}

_finl int Primitive::getVertexStride() const 
{
	return Vertex::GetStride( getVertexFormat() );
}

_finl Vector3D Primitive::getVertexXYZ( int vertIdx )	const
{
	assert( vertIdx >= 0 && vertIdx < vertNum );

	float* pV = (float*)((BYTE*)vbuf + 
							vertIdx * Vertex::GetStride( (VertexFormat)_vertFormat ));
	return Vector3D( pV[0], pV[1], pV[2] );
}

_finl float* Primitive::vertPos( int verIdx )
{
    assert( verIdx >= 0 && verIdx < vertNum );
    float* pV = (float*)((BYTE*)vbuf + 
							verIdx * Vertex::GetStride( (VertexFormat)_vertFormat ));
	return pV;
}


_finl bool Primitive::isTexCached() const 
{ 
	return (flags & c_TexCached) != 0; 
}

_finl bool Primitive::isIBCached() const	
{ 
	return (flags & c_IBCached) != 0; 
}

_finl bool Primitive::isVBCached() const	
{
	return (flags & c_VBCached) != 0; 
}

_finl bool Primitive::isStatic() const	
{
	return (flags & c_Static) != 0; 
}

_finl bool Primitive::isQuadList() const	
{
	return (flags & c_QuadList) != 0; 
}

_finl bool Primitive::isIndexed() const
{
	return (ibuf != 0);
}

_finl void Primitive::setIBPos( int pos ) 
{ 
	ibPos = pos; 
}

_finl void Primitive::setVBPos( int pos ) 
{ 
	vbPos = pos; 
}

_finl void Primitive::setDevHandle( void* handle ) 
{ 
	devHandle = handle; 
}

_finl void Primitive::setVertexPtr( void* ptr )	
{ 
	vbuf = ptr; 
}

_finl void Primitive::setIndexPtr( WORD* ptr )
{
	ibuf = ptr;
}

_finl void Primitive::setMaxVert( int n )
{
    maxVertNum = n;
}

_finl void Primitive::setMaxInd( int n )
{
    maxIndNum = n;
}

_finl void Primitive::setTexCached( bool isCached )
{
	if (isCached) flags |= c_TexCached;
		else flags &= ~c_TexCached;
}

_finl void Primitive::setIBCached( bool isCached )
{
	if (isCached) flags |= c_IBCached;
		else flags &= ~c_IBCached;
}

_finl void Primitive::setVBCached( bool isCached )
{
	if (isCached) flags |= c_VBCached;
		else flags &= ~c_VBCached;
}	

_finl void Primitive::setIsStatic( bool isStatic )
{
	if (isStatic) flags |= c_Static;
		else flags &= ~c_Static;
}

_finl void Primitive::setIsQuadList( bool isQList )
{
	if (isQList) flags |= c_QuadList;
		else flags &= ~c_QuadList;
}

_finl void Primitive::setNVert( int n ) 
{
	assert( n <= maxVertNum );
	vertNum = n; 
}

_finl void Primitive::setNPri( int n )
{
	numPri = n;	
}

_finl void Primitive::setNInd( int n ) 
{
	assert( n <= maxIndNum || isQuadList());
	indNum = n; 
}

_finl void Primitive::incNVert( int n )
{
	vertNum += n;
	assert( vertNum <= maxVertNum );
}
 
_finl void Primitive::incNPri( int n )
{
	numPri += n;
}

_finl void Primitive::incNInd( int n )
{
	indNum += n;
	assert( indNum <= maxIndNum );
}
    
_finl int Primitive::numPriToIdx( int priNum )
{
	return calcNumIdx( getPriType(), priNum );
}

_finl int Primitive::numIdxToPri( int indNum )
{
	return calcNumPri( getPriType(), indNum );
}

_finl void Primitive::setVertexFormat( VertexFormat vf ) 
{
	_vertFormat = (unsigned char)vf;
}

_finl void Primitive::setPriType( PrimitiveType pt )
{
	_priType = (unsigned char)pt;
}

_finl void Primitive::setIndices( const WORD* idxBuf, int numIdx )
{
	if (numIdx > maxIndNum) 
	{
		Log.Error( "Couldn't set indices: ib size %d, needed %d", 
						maxIndNum, numIdx );
		return;
	}

	indNum	= numIdx;
	memcpy( ibuf, idxBuf, numIdx << 1 );
	numPri = calcNumPri( (PrimitiveType)_priType, numIdx );
} // Primitive::setIndices 

_finl void Primitive::setVertices( void* vert, int numVert )
{
	if (numVert > maxVertNum)
	{
		Log.Error( "Couldn't set vertices: vb size %d, needed %d", 
						maxVertNum, numVert );
		return;
	}

	vertNum = numVert;
	memcpy( vbuf, vert, numVert * Vertex::GetStride( (VertexFormat)_vertFormat ) );
}

_finl void Primitive::setTexture( int tex, int stage )
{
	assert( stage >= 0 && stage < 3 );
	texID[stage] = tex;
} 

_finl void Primitive::setShader( int shader )
{
	shaderID = shader;
}

_inl int Primitive::calcNumPri( PrimitiveType pType, int indNum )
{
	switch( pType )
	{
	case ptUnknown:			return 0;
	case ptTriangleList:	return indNum / 3; 	
	case ptTriangleStrip:	return indNum - 2;
	case ptLineList:		return indNum >> 1;
	case ptLineStrip:		return indNum - 1;
	case ptPointList:		return indNum; 
	case ptTriangleFan:		return indNum - 2;
	}	
	return 0;
} // Primitive::calcNumPri

_inl int Primitive::calcNumIdx( PrimitiveType pType, int priNum )
{
	switch( pType )
	{
	case ptUnknown:			return 0;
	case ptTriangleList:	return priNum * 3; 	
	case ptTriangleStrip:	return priNum + 2;
	case ptLineList:		return priNum << 1;
	case ptLineStrip:		return priNum + 1;
	case ptPointList:		return priNum; 
	case ptTriangleFan:		return priNum + 2;
	}	
	return 0;
} // Primitive::calcNumIdx

