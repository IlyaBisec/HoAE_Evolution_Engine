/*****************************************************************************/
/*	File:	rsVertex.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-13-2003
/*****************************************************************************/

/*****************************************************************************/
/*	Vertex implementation
/*****************************************************************************/
_inl int Vertex::GetStride( VertexFormat vf )
{
	return c_vfStride[vf];
} // Vertex::CalcStride

_inl int Vertex::GetDiffuseStride( VertexFormat vf )
{
	return c_vfDiffuseStride[vf];
}

_inl int Vertex::GetSpecularStride( VertexFormat vf )
{
    return c_vfSpecularStride[vf];
}

_inl int Vertex::GetUVStride( VertexFormat vf )
{
	return c_vfUVStride[vf];
}

_inl int Vertex::GetUV2Stride( VertexFormat vf )
{
	return c_vfUV2Stride[vf];
}

_inl int Vertex::GetNormalStride( VertexFormat vf )
{
	return c_vfNormalStride[vf];
}

_inl int Vertex::GetBlendWStride( VertexFormat vf )
{
    return c_vfBlendWStride[vf];
}

_inl int Vertex::GetBlendIStride( VertexFormat vf )
{
    return c_vfBlendIStride[vf];
}

/*****************************************************************************/
/*	VertexIterator implementation
/*****************************************************************************/
_inl VertexIterator::VertexIterator()
{
	pVert = NULL;
} // VertexIterator::VertexIterator

_inl VertexIterator::VertexIterator( BYTE* vbuf, int nV, VertexFormat vf )
{
	reset( vbuf, nV, vf );
} // VertexIterator::VertexIterator

_inl void VertexIterator::reset( BYTE* vbuf, int nV, VertexFormat vf )
{
	pVert			= vbuf;
	nVert			= nV;
	cVert			= 0;
	stride			= Vertex::GetStride( vf ); 
	vertexFormat	= vf;

	diffuseStride	= Vertex::GetDiffuseStride( vf );
	specularStride	= Vertex::GetSpecularStride( vf );
	uvStride		= Vertex::GetUVStride( vf );
	uv2Stride		= Vertex::GetUV2Stride( vf );
	normStride		= Vertex::GetNormalStride( vf );
    blendIStride    = Vertex::GetBlendIStride( vf );
    blendWStride    = Vertex::GetBlendWStride( vf );

} // VertexIterator::reset

_inl VertexIterator& VertexIterator::operator++()
{
	assert( pVert );
	pVert += stride;
	cVert++;
	return *this;
} // VertexIterator::operator++ - prefix

_inl VertexIterator& VertexIterator::operator++( int )
{
	assert( pVert );
	pVert += stride;
	cVert++;
	return *this;
} // VertexIterator::operator++ - postfix

_inl VertexIterator::operator bool() const
{
	assert( pVert );
	return cVert < nVert;
} // VertexIterator::operator bool

_inl VertexIterator::operator Vector3D&()
{
	assert( pVert );
	return *((Vector3D*)pVert);
} // VertexIterator::operator Vector3D

_inl Vector3D& VertexIterator::pos()
{
	return *((Vector3D*)pVert);
}

_inl Vector3D& VertexIterator::operator[]( int idx )
{
	assert( pVert );
	return *((Vector3D*)(&pVert[stride * idx]));
} // VertexIterator::operator[]

_inl Vector3D& VertexIterator::n()
{
	assert( normStride > 0 && pVert );
	return *((Vector3D*)(pVert + normStride));
} // VertexIterator::n

_inl DWORD&	VertexIterator::diffuse()
{
	assert( diffuseStride > 0 && pVert );
	return *((DWORD*)(pVert + diffuseStride));
} // VertexIterator::diffuse

_inl DWORD&	VertexIterator::specular()
{
	assert( specularStride > 0 && pVert );
	return *((DWORD*)(pVert + specularStride));
}

_inl float&	VertexIterator::u()
{
	assert( uvStride > 0 && pVert );
	return *((float*)(pVert + uvStride));
}

_inl float&	VertexIterator::v()
{
	assert( uvStride > 0 && pVert );
	return *((float*)(pVert + uvStride + sizeof( float )));
}

_inl float&	VertexIterator::u2()
{
	assert( uv2Stride > 0 && pVert );
	return *((float*)(pVert + uv2Stride));
}

_inl float&	VertexIterator::v2()
{
	assert( uv2Stride > 0 && pVert );
	return *((float*)(pVert + uv2Stride + sizeof( float )));
}

_inl Vector3D& VertexIterator::operator ()( int idx )
{
	assert( pVert );
	return *((Vector3D*)(&pVert[stride * idx]));
}

_inl Vector3D& VertexIterator::pos( int idx )
{
	return *((Vector3D*)(&pVert[stride * idx]));
}

_inl Vector3D& VertexIterator::n( int idx )
{
	assert( normStride > 0 && pVert );
	return *((Vector3D*)(&pVert[stride * idx + normStride]));
}

_inl DWORD&	VertexIterator::diffuse	( int idx )
{
	assert( diffuseStride > 0 && pVert );
	return *((DWORD*)(&pVert[stride * idx + diffuseStride]));
}

_inl DWORD&	VertexIterator::specular( int idx )
{
	assert( specularStride > 0 && pVert );
	return *((DWORD*)(&pVert[stride * idx + specularStride]));
}

_inl float&	VertexIterator::u( int idx )
{
	assert( uvStride > 0 && pVert );
	return *((float*)(pVert + stride * idx + uvStride));
}

_inl float&	VertexIterator::v( int idx )
{
	assert( uvStride > 0 && pVert );
	return *((float*)(pVert + stride * idx + uvStride + sizeof( float )));
}

_inl float&	VertexIterator::u2( int idx )
{
	assert( uv2Stride > 0 && pVert );
	return *((float*)(pVert + stride * idx + uv2Stride));
}

_inl float&	VertexIterator::v2( int idx )
{
	assert( uv2Stride > 0 && pVert );
	return *((float*)(pVert + stride * idx + uv2Stride + sizeof( float )));
}

