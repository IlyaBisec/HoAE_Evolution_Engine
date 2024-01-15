/*****************************************************************************/
/*    File:    mSpatial.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15-11-2003
/*****************************************************************************/
#ifndef __MSPATIAL_H__
#define __MSPATIAL_H__

/*****************************************************************************/
/*    Class:    OctTree
/*    Desc:    Octree spatial subdivision data structure.
/*            Creates partition of the objects of template type TObject.
/*            TObject should have defined method 
/*            XStatus Intersect( const Plane& );
/*****************************************************************************/
template <class TObject>
class OctTree
{
public:

    class OctNode
    {
    public:
        std::vector<DWORD>    m_Obj;
        bool                m_bLeaf;
        AABoundBox            m_AABB;
        OctNode*            m_Children[8];

        OctNode() { m_bLeaf = true; memset( m_Children, 0, sizeof(m_Children) ); }

        bool        IsLeafNode() { return m_bLeaf; }
        void        AddObject( int objIdx ){ m_Obj.push_back( objIdx ); }

    }; // class OctNode

    OctNode            m_Root;
    
    bool Create( TObject* objArray, int nObj )
    {
        if (Split( m_Root, objArray ))
        {
            return true;
        }
        return false;
    } // Create

protected:
    bool Split( OctNode& node, TObject* obj )
    {

        return false;
    } // Split

}; // class OctTree


class DefaultBSPSplitPolicy
{

}; // class DefaultBSPSplitPolicy

/*****************************************************************************/
/*    Class:    BSPTree
/*    Desc:    binary space partition data structure
/*****************************************************************************/
template <    class TVert, 
            class TPoly, 
            class SplitPolicy = DefaultBSPSplitPolicy
         >
class BSPTree
{
    class BSPNode
    {
    public:
        int            m_FirstPoly;
        int            m_nPoly;
        Plane        m_SplitPlane;

        int            m_LeftChild;
        int            m_RightChild;
        
                    BSPNode() :    m_FirstPoly    (-1), 
                                m_nPoly        (-1),
                                m_LeftChild    (-1),
                                m_RightChild(-1){}

        bool        IsLeafNode    () { return m_LeftChild < 0 && m_RightChild < 0; }
        void        SetPolyRange( int firstPoly, int numPoly )
        {
            m_FirstPoly = firstPoly;
            m_nPoly = numPoly;
        }

    }; // class BSPNode

    BSPNode             m_Root;        //  root node

public:
    
    bool Create( TVert* vert, int nVert, TPoly* poly, int nPoly )
    {
        Plane plane;
        m_Root.SetPolyRange( 0, nPoly );
        if (Split( m_Root, plane, vert, poly ))
        {
            //  split polygon array

            return true;
        }
        return false;
    } // Create

protected:
    
    bool Split( BSPNode& node, Plane& plane, TVert* vert, TPoly* poly )
    {
        return FindSplitPlane( node, plane, vert, poly );
    } // Split

    bool FindSplitPlane( BSPNode& node, Plane& plane, TVert* vert, TPoly* poly )
    {
        Plane     cPlane;
        Triangle cTri;
        
        int minInOutRatio = node.m_nPoly;
        int minPlaneIdx      = -1;

        for (int i = node.m_FirstPoly + node.m_nPoly - 1; 
                i >= node.m_FirstPoly; i--)
        {
            poly[i].GetPlane( cPlane, vert );
            int nIn        = 0;
            int nOut    = 0;
            int nXSect    = 0;
            for (int j = node.m_FirstPoly + node.m_nPoly - 1; 
                    j >= node.m_FirstPoly; j--)
            {
                if (i == j) continue;                
                poly[j].GetTriangle( cTri, vert );
                XStatus st = cTri.Intersect( cPlane );
                if (st == xsOutside        ) nOut++;
                if (st == xsInside        ) nIn++;
                if (st == xsIntersects    ) nXSect++;
            }

            int inOutRatio = nIn - nOut;
            if (abs( inOutRatio ) < minInOutRatio)
            {
                minInOutRatio = abs( inOutRatio );
                minPlaneIdx = i;
            }
        }
        return false;
    } // FindSplitPlane

}; // class BSPTree

#endif // __MSPATIAL_H__
