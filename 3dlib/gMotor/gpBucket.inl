/*****************************************************************************/
/*	File:	gpBucket.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	19.03.2003
/*****************************************************************************/

BEGIN_NAMESPACE(sg)

const float c_ZMin	= 0.4f;
const float c_ZMax	= 0.9f;

_inl DWORD ZToColor( float z )
{return 0xFFFFFFFF;
	float  fclr = (z - c_ZMin) * ((float)0xFF) / (c_ZMax - c_ZMin);
	if (fclr < 0.0f) fclr = 0.0f;
	if (fclr > (float)0xFF) fclr = (float)0xFF;
	DWORD clr = (DWORD)fclr;
	return 0xFF000000 | (clr << 16) | (clr << 8) | clr;
}

const float c_HalfPixel = 0.5f;
/*****************************************************************************/
/*	GPBucket implementation
/*****************************************************************************/
_inl void GPBucket::AddQuad( const GPChunk& chunk, float cX, float cY, float cZ, DWORD diffuse )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();
		
		vert[0].x = chunk.x + cX - c_HalfPixel;
		vert[0].y = chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ;
		vert[0].u = chunk.u + s_TexCoordBias;
		vert[0].v = chunk.v + s_TexCoordBias;
		
		vert[1].x = vert[0].x + chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2 - s_TexCoordBias;
		vert[1].v = chunk.v  + s_TexCoordBias;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = cZ;
		vert[2].u = chunk.u	 + s_TexCoordBias;
		vert[2].v = chunk.v2 - s_TexCoordBias;
		
		vert[3].x = vert[1].x ;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2 - s_TexCoordBias;
		vert[3].v = chunk.v2 - s_TexCoordBias;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();
		
		vert[0].x = chunk.x + cX;
		vert[0].y = chunk.y*2.0f + cY;
		vert[0].z = cZ;
		vert[0].u = chunk.u + s_TexCoordBias;
		vert[0].v = chunk.v + s_TexCoordBias;
		
		vert[1].x = vert[0].x + chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2 - s_TexCoordBias;
		vert[1].v = chunk.v  + s_TexCoordBias;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side*2.0f;
		vert[2].z = cZ;
		vert[2].u = chunk.u	 + s_TexCoordBias;
		vert[2].v = chunk.v2 - s_TexCoordBias;
		
		vert[3].x = vert[0].x + chunk.side;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2 - s_TexCoordBias;
		vert[3].v = chunk.v2 - s_TexCoordBias;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else
	{
		assert( false );
	}
	setNVert( getNVert() + 4 );
	setNInd ( getNInd()  + 6 );
} // GPBucket::AddQuad

_inl void GPBucket::AddQuadMirroredAligned( const GPChunk& chunk, 
											const Plane& plane, 
											float cX, float cY, float cZ, 
											float scale, DWORD diffuse )
{
		if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();
		
		float scS = scale * chunk.side;
		float scX = scale * chunk.x;
		float scY = scale * chunk.y;
		
		float scaledSide = scale * chunk.side;
		
		vert[0].x = cX - scale * chunk.x - c_HalfPixel;
		vert[0].y = scale * chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ + plane.getZ( scX, scY );
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - scS;
		vert[1].y = vert[0].y;
		vert[1].z = cZ + plane.getZ( scX + scS, scY );
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scS;
		vert[2].z = cZ + plane.getZ( scX, scY + scS );
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = vert[3].z = cZ + plane.getZ( scX + scS, scY + scS );
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

		float scS = scale * chunk.side;
		float scX = scale * chunk.x;
		float scY = scale * chunk.y;
		
		vert[0].x = cX - scale * chunk.x;
		vert[0].y = scale * chunk.y + cY;
		vert[0].z = cZ + plane.getZ( scX, scY );
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - scS;
		vert[1].y = vert[0].y;
		vert[1].z = cZ + plane.getZ( scX + scS, scY );
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scS;
		vert[2].z = cZ + plane.getZ( scX, scY + scS );
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ + plane.getZ( scX + scS, scY + scS );
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else 
	{
		assert( false );
	}
	setNVert( getNVert() + 4 );
	setNInd ( getNInd()  + 6 );
} // GPBucket::AddQuadMirroredAligned

_inl void GPBucket::AddQuadAligned( const GPChunk& chunk, 
									const Plane& plane, 
									float cX, float cY, float cZ, 
									float scale, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();
		
		float scS = scale * chunk.side;
		float scX = scale * chunk.x;
		float scY = scale * chunk.y;
		
		vert[0].x = scX + cX - c_HalfPixel;
		vert[0].y = scY + cY - c_HalfPixel;
		vert[0].z = cZ + plane.getZ( scX, scY );
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + scS;
		vert[1].y = vert[0].y;
		vert[1].z = cZ + plane.getZ( scX + scS, scY );
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scS;
		vert[2].z = cZ + plane.getZ( scX, scY + scS );
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ + plane.getZ( scX + scS, scY + scS );
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

		float scS = scale * chunk.side;
		float scX = scale * chunk.x;
		float scY = scale * chunk.y;
		
		vert[0].x = scX + cX;
		vert[0].y = scY + cY;
		vert[0].z = cZ + plane.getZ( scX, scY );
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + scS;
		vert[1].y = vert[0].y;
		vert[1].z = cZ + plane.getZ( scX + scS, scY );
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scS;
		vert[2].z = cZ + plane.getZ( scX, scY + scS );
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ + plane.getZ( scX + scS, scY + scS );
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else 
	{
		assert( false );
	}
	setNVert( getNVert() + 4 );
	setNInd ( getNInd()  + 6 );
} // GPBucket::AddQuadAligned

const float c_UVBorder		= 1.0f / 200.0f;
_inl void GPBucket::AddQuadVS( const GPChunk& chunk, const SoftwareVS& vs, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	assert( vf == vf2Tex );
	Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

	vert[0].x = chunk.x;
	vert[0].y = chunk.y;
	vert[0].z = 0.0f;
	vert[0].u = chunk.u + c_UVBorder;
	vert[0].v = chunk.v + c_UVBorder;
	
	vert[1].x = vert[0].x + chunk.side;
	vert[1].y = vert[0].y;
	vert[1].z = 0.0f;
	vert[1].u = chunk.u2 - c_UVBorder;
	vert[1].v = vert[0].v;
	
	vert[2].x = vert[0].x;
	vert[2].y = vert[0].y + chunk.side;
	vert[2].z = 0.0f;
	vert[2].u = vert[0].u;
	vert[2].v = chunk.v2 - c_UVBorder;
	
	vert[3].x = vert[1].x;
	vert[3].y = vert[2].y;
	vert[3].z = 0.0f;
	vert[3].u = vert[1].u;
	vert[3].v = vert[2].v;
	
	vert[0].diffuse = diffuse;
	vert[1].diffuse = diffuse;
	vert[2].diffuse = diffuse;
	vert[3].diffuse = diffuse;

	vs.ProcessVertices( vert, 4 );
	setNVert( getNVert() + 4 );
	setNInd ( getNInd()  + 6 );
} // GPBucket::AddQuadVS

_inl void GPBucket::AddQuad( const GPChunk& chunk, const Matrix4D& transf, DWORD diffuse )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();

		vert[0].x = chunk.x - c_HalfPixel;
		vert[0].y = chunk.y - c_HalfPixel;
		vert[0].z = 0.0f;
		vert[0].u = chunk.u + s_TexCoordBias;
		vert[0].v = chunk.v + s_TexCoordBias;
		
		vert[1].x = vert[0].x + chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = 0.0f;
		vert[1].u = chunk.u2 - s_TexCoordBias;
		vert[1].v = chunk.v	 + s_TexCoordBias;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = 0.0f;
		vert[2].u = chunk.u  + s_TexCoordBias;
		vert[2].v = chunk.v2 - s_TexCoordBias;
		
		vert[3].x = vert[0].x + chunk.side;
		vert[3].y = vert[0].y + chunk.side;
		vert[3].z = 0.0f;
		vert[3].u = chunk.u2 - s_TexCoordBias;
		vert[3].v = chunk.v2 - s_TexCoordBias;
		
		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;

		(*(Vector4D*)(vert + 0)).mul( transf );
		(*(Vector4D*)(vert + 1)).mul( transf );
		(*(Vector4D*)(vert + 2)).mul( transf );
		(*(Vector4D*)(vert + 3)).mul( transf );
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();
		
		vert[0].x = chunk.x;
		vert[0].y = chunk.y;
		vert[0].z = 0.0f;
		vert[0].u = chunk.u + s_TexCoordBias;
		vert[0].v = chunk.v + s_TexCoordBias;
		
		vert[1].x = vert[0].x + chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = 0.0f;
		vert[1].u = chunk.u2 - s_TexCoordBias;
		vert[1].v = chunk.v  + s_TexCoordBias;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = 0.0f;
		vert[2].u = chunk.u  + s_TexCoordBias;
		vert[2].v = chunk.v2 - s_TexCoordBias;
		
		vert[3].x = vert[0].x + chunk.side;
		vert[3].y = vert[0].y + chunk.side;
		vert[3].z = 0.0f;
		vert[3].u = chunk.u2 - s_TexCoordBias;
		vert[3].v = chunk.v2 - s_TexCoordBias;
		
		(*(Vector3D*)(vert + 0)) *= transf;
		(*(Vector3D*)(vert + 1)) *= transf;
		(*(Vector3D*)(vert + 2)) *= transf;
		(*(Vector3D*)(vert + 3)) *= transf;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}	
	else 
	{
		assert( false );
	}
	
	setNVert( getNVert() + 4 );
	setNInd ( getNInd()  + 6 );
} // GPBucket::AddQuad

_inl void GPBucket::AddQuadMirrored( const GPChunk& chunk, 
									float cX, float cY, float cZ, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();
		
		vert[0].x = cX - chunk.x - c_HalfPixel;
		vert[0].y = chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

		vert[0].x = cX - chunk.x;
		vert[0].y = chunk.y + cY*2.0f;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side*2.0f;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else 
	{
		assert( false );
	}

	setNVert( getNVert() + 4 );
	setNInd ( getNInd()  + 6 );
} // GPBucket::AddQuadMirrored

_inl void GPBucket::AddQuadMirroredScaled(	const GPChunk& chunk, 
											float cX, float cY, float cZ, 
											float scale, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();

	float c_MirrorUVBias = -scale / 256.0f;
	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();

		float scaledSide = scale * chunk.side;
		
		vert[0].x = cX - scale * chunk.x - c_HalfPixel;
		vert[0].y = scale * chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ;
		vert[0].u = chunk.u + c_MirrorUVBias;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - scaledSide;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2 + c_MirrorUVBias;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scaledSide;
		vert[2].z = cZ;
		vert[2].u = chunk.u + c_MirrorUVBias;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2 + c_MirrorUVBias;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();
		float scaledSide = scale * chunk.side;
		
		vert[0].x = cX - scale * chunk.x;
		vert[0].y = scale * chunk.y*2.0f + cY;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - scaledSide;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scaledSide*2.0f;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].x = cX - chunk.x;
		vert[0].y = chunk.y + cY;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x - chunk.side;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + chunk.side;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;
	}
	else 
	{
		assert( false );
	}

	setNVert( getNVert() + 4 );
	setNInd ( getNInd()  + 6 );
} // GPBucket::AddQuadScaledMirrored

_inl void GPBucket::AddQuadScaled(	const GPChunk& chunk, 
									float cX, float cY, float cZ,
									float scale, DWORD diffuse  )
{
	if (getNVert() >= (c_MaxQuadsDrawn - 1)*4) Flush();
	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{	
		VertexTnL* vert = ((VertexTnL*)getVertexData()) + getNVert();

		float scaledSide = scale * chunk.side;
		
		vert[0].x = scale * chunk.x + cX - c_HalfPixel;
		vert[0].y = scale * chunk.y + cY - c_HalfPixel;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + scaledSide;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scaledSide;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].w = 1.0f; 
		vert[1].w = 1.0f; 
		vert[2].w = 1.0f; 
		vert[3].w = 1.0f;
		
		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* vert = ((Vertex2t*)getVertexData()) + getNVert();

		float scaledSide = scale * chunk.side;
		
		vert[0].x = scale * chunk.x + cX;
		vert[0].y = scale * chunk.y*2.0f + cY;
		vert[0].z = cZ;
		vert[0].u = chunk.u;
		vert[0].v = chunk.v;
		
		vert[1].x = vert[0].x + scaledSide;
		vert[1].y = vert[0].y;
		vert[1].z = cZ;
		vert[1].u = chunk.u2;
		vert[1].v = chunk.v;
		
		vert[2].x = vert[0].x;
		vert[2].y = vert[0].y + scaledSide*2.0f;
		vert[2].z = cZ;
		vert[2].u = chunk.u;
		vert[2].v = chunk.v2;
		
		vert[3].x = vert[1].x;
		vert[3].y = vert[2].y;
		vert[3].z = cZ;
		vert[3].u = chunk.u2;
		vert[3].v = chunk.v2;

		vert[0].diffuse = diffuse;
		vert[1].diffuse = diffuse;
		vert[2].diffuse = diffuse;
		vert[3].diffuse = diffuse;
	}
	else 
	{
		assert( false );
	}

	setNVert( getNVert() + 4 );
	setNInd ( getNInd()  + 6 );
} // GPBucket::AddQuadScaled

_inl void GPBucket::AddQuad( const GPChunk& chunk, const Vector3D& pos, DWORD diffuse,
							 BaseMesh& bm, int cVert, int nVert, int cPoly, int nPoly )
{
	if (nVert > getMaxVert()) return;
	if (getNVert() >= getMaxVert() - nVert) Flush();
	if (getNInd()  >= getMaxInd() - nPoly*3) Flush();

	if (nVert > getMaxVert())  nVert = getMaxVert();
	if (nPoly > getMaxInd()/3) nPoly = getMaxInd()/3;
	
	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* dv = ((VertexTnL*)getVertexData()) + getNVert();
		VertexTnL* sv = ((VertexTnL*)bm.getVertexData()) + cVert;

		for (int i = 0; i < nVert; i++)
		{
			dv[i].x = sv[i].x + pos.x;
			dv[i].y = sv[i].y + pos.y;
			dv[i].z = sv[i].z + pos.z;
			dv[i].u = sv[i].u*(chunk.u2 - chunk.u) + chunk.u;
			dv[i].v = sv[i].v*(chunk.v2 - chunk.v) + chunk.v;
			dv[i].w = 1.0f;
			dv[i].diffuse = diffuse;
		}

	}
	else if (vf == vf2Tex)
	{
		Vertex2t* dv = ((Vertex2t*)getVertexData()) + getNVert();
		Vertex2t* sv = ((Vertex2t*)bm.getVertexData()) + cVert;
		
		for (int i = 0; i < nVert; i++)
		{
			dv[i].x = sv[i].x + pos.x;
			dv[i].y = sv[i].y + pos.y;
			dv[i].z = sv[i].z + pos.z;
			dv[i].u = sv[i].u*(chunk.u2 - chunk.u) + chunk.u;
			dv[i].v = sv[i].v*(chunk.v2 - chunk.v) + chunk.v;
			dv[i].diffuse = diffuse;
		}
	}
	else
	{
		assert( false );
	}
	
	int sVert = cVert;
	int dVert = getNVert();

	WORD* sidx = bm.getIndices() + cPoly*3;
	WORD* didx = getIndices() + getNInd();
	
	int cIdx = 0;
	for (int i = 0; i < nPoly; i++)
	{
		didx[cIdx] = sidx[cIdx] + dVert - sVert; cIdx++;
		didx[cIdx] = sidx[cIdx] + dVert - sVert; cIdx++;
		didx[cIdx] = sidx[cIdx] + dVert - sVert; cIdx++;
	}
	
	setNVert( getNVert() + nVert		);
	setNInd ( getNInd()  + nPoly * 3	);
} // GPBucket::AddQuad

_inl void GPBucket::AddQuad( const GPChunk& chunk, const Vector3D& pos, 
								BYTE* pVert, DWORD diffuse )
{
	if (getNVert() >= getMaxVert() - 4) Flush();
	if (getNInd()  >= getMaxInd() - 6) Flush();

	VertexFormat vf = getVertexFormat();
	if (vf == vfTnL)
	{
		VertexTnL* dv = ((VertexTnL*)getVertexData()) + getNVert();
		VertexTnL* sv = (VertexTnL*)pVert;

		for (int i = 0; i < 4; i++)
		{
			dv[i].x = sv[i].x + pos.x;
			dv[i].y = sv[i].y + pos.y;
			dv[i].z = sv[i].z + pos.z;
			dv[i].w = 1.0f;
			dv[i].diffuse = diffuse;
		}

		dv[0].u = dv[2].u = chunk.u;
		dv[1].u = dv[3].u = chunk.u2;

		dv[0].v = dv[2].v = chunk.v;
		dv[2].v = dv[3].v = chunk.v2;
	}
	else if (vf == vf2Tex)
	{
		Vertex2t* dv = ((Vertex2t*)getVertexData()) + getNVert();
		Vertex2t* sv = (Vertex2t*)pVert;

		for (int i = 0; i < 4; i++)
		{
			dv[i].x = sv[i].x + pos.x;
			dv[i].y = sv[i].y + pos.y;
			dv[i].z = sv[i].z + pos.z;
			dv[i].diffuse = diffuse;
		}

		dv[0].u = dv[2].u = chunk.u;
		dv[1].u = dv[3].u = chunk.u2;

		dv[0].v = dv[1].v = chunk.v;
		dv[2].v = dv[3].v = chunk.v2;
	}
	else
	{
		assert( false );
	}

	setNVert( getNVert() + 4 );
	setNInd ( getNInd()  + 6 );
} // GPBucket::AddQuad


_inl void GPBucket::Flush()
{
	if (getNVert() == 0) return;
	setShader( s_CurShader );
	RC::iRS->Draw( *this );
	setNVert( 0 );
	setNPri ( 0 );
	setNInd ( 0 );
} // GPBucket::Flush

_inl int GPBucket::GetCurrentVertexIdx() const
{
	return getNVert(); 
} // GPBucket::GetCurrentVertexIdx

_inl void GPBucket::Reset()
{
	setNVert( 0 );
	setNPri ( 0 );
	setNInd ( 0 );
}

_inl void GPBucket::Init()
{
	static int shSprite2D = RC::iRS->GetShaderID( "hud" );
		
	curVF = vfTnL;
	create		( c_MaxQuadsDrawn * 4, c_MaxQuadsDrawn * 16, vfTnL, ptTriangleList );
	setMaxInd	( c_MaxQuadsDrawn * 16		);
	setPriType	( ptTriangleList			);
	setShader	( shSprite2D				);
	
	setVertexFormat	( vfTnL					);
	setIsQuadList	( true );
	setNPri			( 0 );
}; // GPBucket::Init

_inl void GPBucket::SetCurrentVertexFormat( VertexFormat vf )
{
	if (vf != getVertexFormat())
	{
		Flush();
		setVertexFormat( vf );
		setDevHandle( 0 );
	}
} // GPBucket::SetCurrentVertexFormat

_inl void GPBucket::SetIsQuadList( bool _isQuadList )
{
	if (_isQuadList != isQuadList())
	{
		Flush();
		setIsQuadList( _isQuadList );
		setDevHandle( 0 );
	}
} // GPBucket::SetIsQuadList

END_NAMESPACE(sg)
