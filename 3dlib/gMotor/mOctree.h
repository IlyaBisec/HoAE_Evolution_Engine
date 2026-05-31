/*****************************************************************************/
/*    File:    mOctree.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-10-2003
/*****************************************************************************/
#ifndef __MOCTREE_H__
#define __MOCTREE_H__

#define OCTEMPL template <class TFace, class MyType> 
const int c_DefaultMaxFacesInLeaf = 50;

/*****************************************************************************/
/*    Class:    Octree
/*    Desc:    Octant space subdivision structure
/*****************************************************************************/
template <class TFace, class MyType>
class Octree
{
public:
    _inl                Octree();
    _inl                ~Octree();

    //  recursively builds octree 
    void                Build( TFace* faces, int nFaces );

protected:
    
    //  first part is written to face, second is returned
    _inl AABoundBox        FindAABB( const TFace* faces, int nFaces ) const;
    _inl AABoundBox        FindAABB( const TFace** faces, int nFaces ) const;

    //  recursively builds octree node children
    bool                RecurseBuild( const std::vector<TFace*> &fplist );

    virtual bool        ProcessNode( const std::vector<TFace*> &fplist ) = 0;

protected:
    MyType*                children[8];

    Vector3D            center;
    float                side;

    AABoundBox            aabb;

}; // class Octree

#include "mOctree.hpp"

#ifdef _INLINES
#include "mOctree.inl"
#endif // _INLINES

#endif // __MOCTREE_H__
