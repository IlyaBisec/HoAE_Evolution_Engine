/*****************************************************************************/
/*	File:	gpBucket.h
/*	Desc:	Sprite render bin
/*	Author:	Ruslan Shestopalyuk
/*	Date:	19.03.2003
/*****************************************************************************/
#ifndef __GPBUCKET_H__
#define __GPBUCKET_H__

namespace sg{

const int c_MaxQuadsDrawn			= 512;

class GPChunk;
class SoftwareVS;
/*****************************************************************************/
/*	Class:	GPBucket
/*****************************************************************************/
class GPBucket : public BaseMesh
{
public:
	GPBucket() : BaseMesh() {}

	_inl void			Reset();
	_inl void			Init();
	_inl void			Flush();
	_inl int			GetCurrentVertexIdx() const;

	_inl void			AddQuadMirrored( const GPChunk& chunk, 
											float cX, float cY, float cZ, 
											DWORD diffuse );
	_inl void			AddQuad( const GPChunk& chunk, float cX, float cY, float cZ,
									DWORD diffuse );
	_inl void			AddQuad( const GPChunk& chunk, const Vector3D& pos, DWORD diffuse,
								 BaseMesh& bm, int cVert, int nVert, int cPoly, int nPoly );

	_inl void			AddQuad( const GPChunk& chunk, const Matrix4D& transf, DWORD diffuse );
	_inl void			AddQuad( const GPChunk& chunk, const Vector3D& pos, 
									BYTE* pVert, DWORD diffuse );


	_inl void			AddQuadAligned( const GPChunk& chunk, 
										const Plane& plane, 
										float cX, float cY, float cZ, 
										float scale, DWORD diffuse );
	_inl void			AddQuadMirroredAligned( const GPChunk& chunk, 
												const Plane& plane, 
												float cX, float cY, float cZ, 
												float scale, DWORD diffuse );
	_inl void			AddQuadMirroredScaled(	const GPChunk& chunk, 
												float cX, float cY, float cZ,
												float scale, DWORD diffuse );
	_inl void			AddQuadScaled(	const GPChunk& chunk, 
										float cX, float cY, float cZ,
										float scale, DWORD diffuse );
	_inl void			AddQuadVS( const GPChunk& chunk, const SoftwareVS& vs, DWORD diffuse );

	_inl void			SetCurrentVertexFormat( VertexFormat vf );
	_inl void			SetIsQuadList( bool _isQuadList = true );


	static int			s_CurShader;
	static float		s_TexCoordBias;

private:
	VertexFormat		curVF;

}; // class GPBucket

/*****************************************************************************/
/*	Class:	SoftwareVS
/*	Desc:	Functor that to some extent emulates vertex shader in software
/*****************************************************************************/
class SoftwareVS
{
public:
	virtual void ProcessVertices( Vertex2t* vert, int nV ) const = 0;
}; // class SoftwareVS

/*****************************************************************************/
/*	Class:	BendVS
/*	Desc:	Does bending of geometry (trees, for example)
/*****************************************************************************/
class BendVS : public SoftwareVS
{
	Matrix4D	tr;
	float		height;
	DWORD		time;
	float		ratio;
	float		cX, cY;
	int			phase;

public:
	virtual void ProcessVertices( Vertex2t* vert, int nV ) const;

	void		setTransform( const Matrix4D& _tr ) { tr = _tr;				}
	void		setBase		( float _cX, float _cY ){ cX = _cX; cY = _cY;	}
	void		setTime		( DWORD _time	);
	void		setHeight	( DWORD _height )		{ height = _height;		}
	void		setPhase	( int _phase	)		{ phase = _phase;		}
	
}; // class SoftwareVS

}; // namespace sg

#ifdef _INLINES
#include "gpBucket.inl"
#endif // _INLINES

#endif // __GPBUCKET_H__
