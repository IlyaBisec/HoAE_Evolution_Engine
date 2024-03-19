/*****************************************************************/
/*  File:   AuxDrawn.h
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __COORSYS_H__
#define __COORSYS_H__

namespace AuxPrim
{

const WORD c_WireBoxIdx[24] = 
{
	0, 1,   1, 5,   5, 4,   4, 0, 
	6, 7,   7, 3,   3, 2,   2, 6, 
	6, 4,   7, 5,   3, 1,   2, 0
};

/*****************************************************************/
/*  Class:	CoorSys
/*  Desc:	Cooordinate system axis
/*****************************************************************/
class CoorSys : public BaseMesh
{
public:
	CoorSys();
	~CoorSys();
	void Init( float axisLen = 100.0f );
	void Draw();

protected:
private:
};  // class CoorSys

void	DrawAABB( const AABoundBox& aabb, DWORD color );
void	DrawProjectedSphere( const Sphere& sphere, DWORD color );
void	CreateCoorSys( BaseMesh& bm, float axisLen );

void	DrawTransformGizmo( const Matrix4D& transform );
void	DrawNormals( const VertexN* srcVert, int nVert, float normalLen );

template <class TVert>
bool CreateWireAABB( const AABoundBox& aabb, TVert* v, int maxV, 
					WORD* idx, int maxIdx )
{
	if (maxV < 8) return false;
	if (maxIdx < 24) return false;

	memcpy( idx, c_WireBoxIdx, 24 * sizeof( WORD ) );

	Vector3D cr[8];
	aabb.GetCorners( cr );
	for (int i = 0; i < 8; i++)
	{
		v[i].setXYZ( cr[i].x, cr[i].y, cr[i].z );
	}

	return true;
} // CreateWireAABB

} // namespace AuxPrim

#endif // __COORSYS_H__