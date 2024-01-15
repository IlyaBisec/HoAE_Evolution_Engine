/*****************************************************************************/
/*	File:	mOctree.hpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07-10-2003
/*****************************************************************************/
#include "mIntersect.h"

/*****************************************************************************/
/*	Octree implementation
/*****************************************************************************/
OCTEMPL 
Octree<TFace, MyType>::Octree() 
{
	memset( children, 0, sizeof( children ) );
}

OCTEMPL 
Octree<TFace, MyType>::~Octree()
{
}

OCTEMPL void Octree<TFace, MyType>::
Build( TFace* faces, int nFaces )
{
	aabb = FindAABB( faces, nFaces );

	//  root center and side
	aabb.GetCenter( center );
	side = maxf(	aabb.maxv.x - aabb.minv.x, 
					aabb.maxv.y - aabb.minv.y, 
					aabb.maxv.z - aabb.minv.z );

	std::vector<TFace*> fplist;
	
	for (int i = 0; i < nFaces; i++)
	{
		fplist.push_back( &faces[i] );
	}

	RecurseBuild( fplist );
} // Octree::Build

OCTEMPL bool Octree<TFace, MyType>
::RecurseBuild( const std::vector<TFace*> &fplist )
{
	const Vector3D c_ChildCenterShift[] =
	{
		Vector3D( -0.5f, -0.5f, -0.5f ),	// - - -
		Vector3D(  0.5f, -0.5f, -0.5f ),	// + - -
		Vector3D( -0.5f,  0.5f, -0.5f ),	// - + -
		Vector3D(  0.5f,  0.5f, -0.5f ),	// + + -
		Vector3D( -0.5f, -0.5f,  0.5f ),	// - - +
		Vector3D(  0.5f, -0.5f,  0.5f ),	// + - +
		Vector3D( -0.5f,  0.5f,  0.5f ),	// - + +
		Vector3D(  0.5f,  0.5f,  0.5f )		// + + +
	};

	
	if (ProcessNode( fplist )) 
	{
		return true;
	}

	std::vector<TFace*> chList;
	for (int i = 0; i < 8; i++)
	{
		children[i] = new MyType();
		MyType* ch = children[i];

		Vector3D cshift = c_ChildCenterShift[i];
		ch->side = side * 0.5f;
		cshift *= ch->side;
		ch->center.add( center, cshift );
		ch->aabb = AABoundBox( ch->center, ch->side * 0.5f );

		//  find polygons which are inside this child
		chList.clear();
		for (int j = 0; j < fplist.size(); j++)
		{
			TFace& f = *(fplist[j]);
			if (AABBTriangleX( ch->aabb, f.v[0]->pos, f.v[1]->pos, f.v[2]->pos ))
			{
				chList.push_back( &f );
			}
		}
		
		children[i]->RecurseBuild( chList );
	}

	return false;
} // Octree::RecurseBuild

OCTEMPL _inl AABoundBox	Octree<TFace, MyType>
::FindAABB( const TFace* faces, int nFaces ) const
{
	AABoundBox aabb;
	for (int i = 0; i < nFaces; i++)
	{
		const TFace& f = faces[i];
		aabb.InsertPoint( f.v[0]->pos );
		aabb.InsertPoint( f.v[1]->pos );
		aabb.InsertPoint( f.v[2]->pos );
	}

	return aabb;
} // Octree::FindAABB

OCTEMPL _inl AABoundBox	Octree<TFace, MyType>
::FindAABB( const TFace** faces, int nFaces ) const
{
	assert( faces );
	AABoundBox aabb;
	for (int i = 0; i < nFaces; i++)
	{
		const TFace& f = *(faces[i]);
		aabb.InsertPoint( f.v[0] );
		aabb.InsertPoint( f.v[1] );
		aabb.InsertPoint( f.v[2] );
	}

	return aabb;
} // Octree::FindAABB