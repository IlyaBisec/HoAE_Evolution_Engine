/*****************************************************************************/
/*	File:	mKDTree.hpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	15-11-2003
/*****************************************************************************/
#ifndef __MKDTREE_H__
#define __MKDTREE_H__

/*****************************************************************************/
/*	Class:	KDTree
/*	Desc:	K-d tree spatial subdivision data structure.
/*			Creates partition of the objects of template type TObject.
/*			TObject should have defined methods: 
/*			XStatus				Intersect( const Plane& );
/*			const AABoundBox&	GetAABB	 () const;
/*****************************************************************************/
template <class TObject>
class KDTree
{
	static int	s_LeafTreshold;
	
	int			m_Depth;
public:
	/*****************************************************************************/
	/*	Class:	KDNode
	/*	Desc:	Internal node type
	/*****************************************************************************/
	class KDNode
	{
	public:
		std::vector<DWORD>	m_Obj;
		bool				m_bLeaf;
		AABoundBox			m_AABB;
		KDNode*				m_Children[8];

		KDNode() { m_bLeaf = true; memset( m_Children, 0, sizeof(m_Children) ); }
		~KDNode()
		{
			for (int i = 0; i < 8; i++) delete m_Children[i];
		}

		bool		IsLeafNode() { return m_bLeaf; }
		void		AddObject( int objIdx, TObject* obj )
		{ 
			if (m_Obj.size() == 0)
			{
				m_AABB = obj[objIdx].GetAABB();
			}
			else
			{
				m_AABB.Extend( obj[objIdx].GetAABB() );
			}
			m_Obj.push_back( objIdx ); 
		}

		int			GetNChildren() const { return 8; }
		int			GetNObjects() const { return m_Obj.size(); }
		KDNode*		GetChild( int idx ) { return m_Children[idx]; }

		KDNode*		xyz() { return m_Children[0]; }
		KDNode*		xyZ() { return m_Children[1]; }
		KDNode*		xYz() { return m_Children[2]; }
		KDNode*		xYZ() { return m_Children[3]; }
		KDNode*		Xyz() { return m_Children[4]; }
		KDNode*		XyZ() { return m_Children[5]; }
		KDNode*		XYz() { return m_Children[6]; }
		KDNode*		XYZ() { return m_Children[7]; }


	}; // class KDNode
	
	typedef TreeIterator<KDNode> Iterator;

	KDNode				m_Root; // top-level node
	static const int	c_LeafTreshold	= 128;
	static const int	c_MaxDepth		= 16;

	bool Create( TObject* objArray, int nObj, int leafTreshold = c_LeafTreshold, int maxDepth = c_MaxDepth )
	{
		if (nObj == 0) return false;

		s_LeafTreshold = leafTreshold;
		m_Root.m_Obj.clear();
		for (int i = 0; i < nObj; i++) m_Root.m_Obj.push_back( i );

		if (Split( &m_Root, objArray, maxDepth ))
		{
			m_Depth = 0;
			CalcDepth( &m_Root, 0 );
			return true;
		}
		return false;
	} // Create

	int GetDepth()
	{
		return m_Depth;
	}

protected:
	bool Split( KDNode* node, TObject* obj, int depth )
	{
		if (!node || depth <= 0) return false;
		const int nObj = node->m_Obj.size();
		if (nObj <= s_LeafTreshold) return false;
		Plane xPlane( Plane::yOz );
		Plane yPlane( Plane::xOz );
		Plane zPlane( Plane::xOy );

		FitPlaneStep( node, obj, xPlane );
		FitPlaneStep( node, obj, yPlane );
		FitPlaneStep( node, obj, zPlane );

		node->m_AABB = obj[node->m_Obj[0]].GetAABB();
		for (int i = 0; i < nObj; i++) 
		{
			node->m_AABB.Extend( obj[node->m_Obj[i]].GetAABB() );
		}

		Vector3D core( -xPlane.d, -yPlane.d, -zPlane.d );
		const Vector3D& minv = node->m_AABB.minv;
		const Vector3D& maxv = node->m_AABB.maxv;

		node->m_bLeaf = false;
		for (int i = 0; i < 8; i++)
		{
			node->m_Children[i] = AllocNode();
			if (!node->m_Children[i]) return false;
		}

		for (int i = 0; i < nObj; i++)
		{
			XStatus sX = obj[node->m_Obj[i]].Intersect( xPlane );
			XStatus sY = obj[node->m_Obj[i]].Intersect( yPlane );
			XStatus sZ = obj[node->m_Obj[i]].Intersect( zPlane );

			int idx = 0;
			if (sX == xsInside) idx |= 0x4;
			if (sY == xsInside) idx |= 0x2;
			if (sZ == xsInside) idx |= 0x1;
			
			node->m_Children[idx]->AddObject( node->m_Obj[i], obj );
			
			////  if we cross boundary, add to all participating children
			//if (sX == xsIntersects) 
			//{
			//	node->m_Children[idx|0x4].AddObject( node->m_Obj[i] );
			//	node->m_Children[idx&(~0x4)].AddObject( node->m_Obj[i] );
			//}
			//if (sY == xsIntersects) 
			//{
			//	node->m_Children[idx|0x2].AddObject( node->m_Obj[i] );
			//	node->m_Children[idx&(~0x2)].AddObject( node->m_Obj[i] );
			//}
			//if (sZ == xsIntersects) 
			//{
			//	node->m_Children[idx|0x1].AddObject( node->m_Obj[i] );
			//	node->m_Children[idx&(~0x1)].AddObject( node->m_Obj[i] );
			//}
		}

		for (int i = 0; i < 8; i++)
		{
			Split( node->m_Children[i], obj, depth - 1 );
		}
		node->m_Obj.resize( 0 );
		return true;
	} // Split

	void FitPlaneBruteforce( KDNode* node, TObject* obj, Plane& plane )
	{
		Plane cPlane( plane );
		const int nObj = node->m_Obj.size();
		int minStatus  = -1;

		for (int i = 0; i < nObj; i++)
		{
			const TObject& cObj = obj[node->m_Obj[i]];
			cPlane.MoveToPoint( cObj.GetAABB().minv );
			int status = CountXStatus( node, obj, cPlane );
			if (status < minStatus || minStatus == -1)
			{
				minStatus = status;
				plane = cPlane;
			}
		}
	} // FitPlaneBruteforce

	void FitPlaneStep( KDNode* node, TObject* obj, Plane& plane )
	{
		Plane cPlane( plane );
		const int nObj = node->m_Obj.size();
		int minStatus  = -1;
		int step = min( nObj - 1, 100 );

		for (int i = 0; i < nObj; i += step)
		{
			const TObject& cObj = obj[node->m_Obj[i]];
			cPlane.MoveToPoint( cObj.GetAABB().minv );
			int status = CountXStatus( node, obj, cPlane );
			if (status < minStatus || minStatus == -1)
			{
				minStatus = status;
				plane = cPlane;
			}
		}
	} // FitPlaneStep

	int CountXStatus( KDNode* node, TObject* obj, Plane& plane )
	{
		const int nObj = node->m_Obj.size();
		int nIn		= 0;
		int nOut	= 0;
		int nXSect	= 0;

		for (int i = 0; i < nObj; i++)
		{
			XStatus st = obj[node->m_Obj[i]].Intersect( plane );
			if (st == xsOutside		) nOut++;
			if (st == xsInside		) nIn++;
			if (st == xsIntersects	) nXSect++;
		}

		return abs( nOut - nIn ) + nXSect;
	} // CountXStatus

	KDNode* AllocNode()
	{
		return new KDNode();
	}

	void CalcDepth( KDNode* pNode, int curDepth )
	{
		if (curDepth > m_Depth) m_Depth = curDepth;
		for (int i = 0; i < 8; i++)
		{
			if (pNode->m_Children[i]) CalcDepth( pNode->m_Children[i], curDepth + 1 );
		}
	}

}; // class KDTree

template <class TObject> int KDTree<TObject>::s_LeafTreshold = 8;

#endif // __MKDTREE_H__
