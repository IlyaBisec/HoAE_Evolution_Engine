/*****************************************************************************/
/*	File:	kNAryIterator.hpp
/*	Desc:	Generic iterrator for the trees with fixed arity
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-26-2003
/*****************************************************************************/
#ifndef __KNARYITERATOR_HPP__
#define __KNARYITERATOR_HPP__

/*****************************************************************************/
/*	Class:	VoidQuery
/*	Desc:	Default n-ary iterator filter
/*****************************************************************************/
template <class TNode> class VoidQuery
{
public:
	static bool Reject( const TNode* pNode )
	{
		return false;
	}
}; // class VoidQuery

/*****************************************************************************/
/*	Class:	OrQuery
/*	Desc:	Complex reject query, which is equivalent to TQuery1||TQuery
/*****************************************************************************/
template <class TNode, class TQuery1, class TQuery2> class OrQuery
{
public:
	static bool Reject( const TNode* pNode )
	{
		return TQuery1::Reject( pNode ) || TQuery2::Reject( pNode );
	}
}; // class OrQuery

/*****************************************************************************/
/*	Class:	NAryIterator
/*	Desc:	n-ary tree generic iterator, iterates through all nodes which
/*				conform to the given query
/*  Rmrk:	class TNode should have TNode& GetChild( int index ); method
/*****************************************************************************/
template <	class	TNode,			//  tree node class
			int		Dim, 
			int		StackSize,
			class	TQuery = VoidQuery<TNode> > 
class NAryIterator
{
	BYTE			m_NStack[StackSize];	//  current child index stack
	TNode*			m_PStack[StackSize];	//  current node pointer stack
	int				m_CurDepth;

public:
	NAryIterator( TNode* pRoot ) : m_CurDepth(0)
	{
		m_NStack[0] = 0;
		m_PStack[0] = pRoot;
		if (!pRoot || TQuery::Reject( pRoot )) m_CurDepth = -1;
	}

	//  advance to the next valid node
	void operator++()
	{
		do{
			while (!m_PStack[m_CurDepth] || m_NStack[m_CurDepth] >= Dim)
			{
				m_CurDepth--;
				if (m_CurDepth < 0) return;
				m_NStack[m_CurDepth]++;
			}

			//  deepen
			m_PStack[m_CurDepth + 1] = m_PStack[m_CurDepth]->GetChild( m_NStack[m_CurDepth] );
			m_CurDepth++; m_NStack[m_CurDepth] = 0;
		} while (!m_PStack[m_CurDepth] || TQuery::Reject( m_PStack[m_CurDepth] ));
	}

	//  returns current node
	operator TNode*() { return m_PStack[m_CurDepth]; }

	//  skips further deepening into current node's subtree
	void up()
	{
		m_NStack[m_CurDepth] = Dim;
		operator++();
	}
	
	//  true when finished traversing
	operator bool()
	{
		return m_CurDepth >= 0;
	}
}; // class NAryIterator

/*****************************************************************************/
/*	Class:	NAryLeafIterator
/*	Desc:	Same as NAryIterator, but iterates only through leaf nodes
/*  Rmrk:	class TNode should have TNode& GetChild( int index ); method
/*			Does not work for now
/*****************************************************************************/
template <	class	TNode,			
			int		Dim, 
			int		StackSize,
			class	TQuery = VoidQuery<TNode> > 
class NAryLeafIterator
{
	BYTE			m_NStack[StackSize];	//  current child index stack
	TNode*			m_PStack[StackSize];	//  current node pointer stack
	int				m_CurDepth;

public:
	NAryLeafIterator( TNode* pRoot ) : m_CurDepth(0)
	{
		m_NStack[0] = 0;
		m_PStack[0] = pRoot;
		if (!pRoot || TQuery::Reject( pRoot )) m_CurDepth = -1;
	}

	//  advance to the next valid node
	void operator++()
	{
		do{
			while (!m_PStack[m_CurDepth] || m_NStack[m_CurDepth] >= Dim)
			{
				m_CurDepth--;
				if (m_CurDepth < 0) return;
				m_NStack[m_CurDepth]++;
			}

			//  deepen
			m_PStack[m_CurDepth + 1] = 
					m_PStack[m_CurDepth]->GetChild( m_NStack[m_CurDepth] );
			m_CurDepth++; m_NStack[m_CurDepth] = 0;
		} while (!m_PStack[m_CurDepth] || TQuery::Reject( m_PStack[m_CurDepth] ));
	}

	//  returns current node
	operator TNode*() { return m_PStack[m_CurDepth]; }

	//  skips further deepening into current node's subtree
	void up()
	{
		m_NStack[m_CurDepth] = Dim;
		operator++();
	}
	
	//  true when finished traversing
	operator bool()
	{
		return m_CurDepth >= 0;
	}
}; // class NAryLeafIterator


#endif __KNARYITERATOR_HPP__
