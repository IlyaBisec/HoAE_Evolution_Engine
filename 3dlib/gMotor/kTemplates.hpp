/*****************************************************************************/
/*	File:	kTemplates.hpp
/*	Desc:	Generic pattern template types
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-26-2003
/*****************************************************************************/
#ifndef __KTEMPLATES_HPP__
#define __KTEMPLATES_HPP__

#include <map>
#include <string>

/*****************************************************************************/
/*	Class:	TreeIterator
/*	Desc:	Generic tree-like structure iterator
/*****************************************************************************/
template <class NodeType>
class TreeIterator
{

	typedef bool (*FilterCallback)	( const NodeType* m_pNode );
	typedef void (*VisitCallback)	( NodeType* m_pNode );

	static const int	c_StackSize = 64;

	NodeType*			m_pRoot;
	NodeType*			m_pNode;
	
	NodeType*			m_Stack		[c_StackSize];
	int					m_IdxStack	[c_StackSize];
	int					m_CurDepth;
	int					m_CurChildIdx;

	FilterCallback		Valid;

	void Push( NodeType* pNode, int childIdx )
	{
		assert( m_CurDepth < c_StackSize );
		m_Stack[m_CurDepth] = pNode;
		m_IdxStack[m_CurDepth] = childIdx;
		m_CurDepth++;
	}

	void Pop()
	{
		m_CurDepth--;
		if (m_CurDepth < 0) return;
		m_pNode = m_Stack[m_CurDepth];
		m_CurChildIdx = m_IdxStack[m_CurDepth];
	}

	void ClearStack()
	{
		m_CurDepth = 0;
	}

	void NextNode()
	{
		if (!m_pNode) return;
		while (m_CurChildIdx >= m_pNode->NumChildren())
		{
			Pop();
			if (m_CurDepth < 0) 
			{
				m_pNode = NULL;
				return;
			}
			m_CurChildIdx++;
		}

		Push( m_pNode, m_CurChildIdx );
		m_pNode = (NodeType*)m_pNode->Child( m_CurChildIdx );
		m_CurChildIdx = 0;
	}

public:
	TreeIterator( NodeType* pRoot, FilterCallback valid = NULL )
	{
		m_pRoot			= pRoot;
		m_pNode			= pRoot;
		m_CurChildIdx	= 0;
		m_CurDepth		= 0;
		Valid			= valid;
		while (m_pNode && !IsValid( m_pNode )) NextNode();
	}

	void operator++()
	{
		NextNode();
		while (m_pNode && !IsValid( m_pNode )) NextNode();
	}

	bool IsValid( const NodeType* pNode ) const
	{
		if (!Valid) return true;
		return Valid( pNode );
	}
	
    NodeType* operator *() const
    {
        return m_pNode;
    }
	
	NodeType* GetParent() const
	{
		return (m_CurDepth > 0) ? m_Stack[m_CurDepth - 1] : NULL;
	}

	operator bool() const
	{
		return (m_pNode != NULL);
	}

	int	GetDepth() const
	{
		return m_CurDepth;
	}

	const NodeType** GetPath() const
	{
		return m_Stack;
	}

	bool SetPath( int depth, const int* path )
	{
		ClearStack();
		m_pNode = m_pRoot;
		for (int i = 0; i < depth; i++)
		{
			m_pNode = m_pNode->GetChild( path[i] );
			if (!m_pNode) 
			{
				return false;
			}
			Push( m_pNode, path[i] );
		}
		return true;
	}

	const int* GetIdxPath() const
	{
		return m_IdxStack;
	}

	void Reset()
	{
		m_pNode = m_pRoot;
		m_CurChildIdx = 0;
		m_CurDepth = 0;
		while (m_pNode && !IsValid( m_pNode )) NextNode();
	}

	void Up()
	{
		Pop();
		if (m_CurDepth < 0) 
		{
			m_pNode = NULL;
			return;
		}
		m_CurChildIdx++;

		while (m_CurChildIdx >= m_pNode->NumChildren())
		{
			Pop();
			if (m_CurDepth < 0) 
			{
				m_pNode = NULL;
				return;
			}
			m_CurChildIdx++;
		}
	}

}; // class TreeIterator

/*****************************************************************************/
/*	Class:	LinearMap
/*	Desc:	Simple linear stupid map
/*****************************************************************************/
template <class T, int MaxEl>
class LinearMap
{
	T		el[MaxEl];
	int		nEl;
public:
			LinearMap() : nEl(0){}

	int		Add( const T& _el )
	{
		for (int i = 0; i < nEl; i++)
		{
			if (el[i] == _el) return i;
		}

		if (nEl == MaxEl - 1) return -1;

		el[nEl++] = _el;
		return nEl - 1;
	}

	int		GetNElem() const { return nEl; }
	void	Clear() { nEl = 0; }

	T&		Elem( int idx ) { assert( idx >= 0 && idx < nEl ); return el[idx]; }
}; // class LinearMap

/*****************************************************************************/
/*	Class:	Singleton
/*	Desc:	Self-destructable singleton class
/*****************************************************************************/
template <class T>
class Singleton
{
public:
	static T& instance()
	{
		static T me;
		return me;
	}

protected:
			Singleton(){}
	virtual ~Singleton(){}

	friend T;
}; // class Singleton

/*****************************************************************************/
/*	Class:	Singleton
/*	Desc:	Non-selfdestructable singleton class
/*****************************************************************************/
template <class T>
class PSingleton
{
public:
	static T* instance()
	{
		static T* me = new T();
		return me;
	}

protected:
	PSingleton(){}
	virtual ~PSingleton(){}

	friend T;
}; // class PSingleton

/*****************************************************************************/
/*	Class:	Maker
/*	Desc:	Generic pluggable factory type
/*****************************************************************************/
template <class Object> 
class Maker 
{
public:
	virtual				~Maker(){}
	static Object*		MakeObject( const char* className )
	{
		MakerMap::iterator it = mkReg.find( std::string( className ) );
		if (it == mkReg.end()) return NULL;
		WORD classID = (*it).second;
		return MakeObject( classID );
	}

	static Object*		MakeObject( WORD classID )
	{
		if (classID == BadClassID || 
			classID < 0 || 
			classID >= mkList.size()) 
		{
			return NULL;
		}
		return mkList[classID]->MakeObject();
	}

	static WORD	GetClassID( const char* className )
	{
		MakerMap::iterator it = mkReg.find( std::string( className ) );
		if (it == mkReg.end()) return BadClassID;
		return (*it).second;
	}

	static const WORD	BadClassID = 0xFFFF;

protected:
	Maker( const char* className )
	{
		std::string sname = std::string( className );
		mkReg[sname] = (WORD)mkList.size();
		mkList.push_back( this );
	}

	virtual Object*		MakeObject() const = 0;

private:
	typedef Maker<Object>*					MakerPtr; 
	typedef std::map<std::string, WORD>		MakerMap;
	typedef std::vector<MakerPtr>			MakerList;

	static MakerMap		mkReg;
	static MakerList	mkList;
}; // class Maker 


#define IMPLEMENT_BASE_MAKER(BaseName)		BaseName##Maker::MakerMap	BaseName##Maker::mkReg;		\
											BaseName##Maker::MakerList	BaseName##Maker::mkList;

#define IMPLEMENT_MAKER(ClassName)			const ClassName##Maker ClassName##Maker::registerThis;	


#define DECLARE_BASE_MAKER(BaseName)		class BaseName##Maker : public Maker<BaseName##>		\
											{														\
											protected:												\
												BaseName##Maker( const char* className ) :			\
												Maker<BaseName##>( className ) {}					\
											}; 	

#define DECLARE_MAKER(ClassName,BaseName)	class ClassName##Maker : public BaseName##Maker{		\
											private:												\
												ClassName##Maker() :  BaseName##Maker(#ClassName)	\
												{													\
												}													\
												virtual BaseName##* MakeObject() const				\
												{													\
													return new ClassName##();						\
												}													\
												static const ClassName##Maker registerThis;			\
											};		

template <class T>
class LinearRegistry
{
	std::vector<T>		el;
public:
	int		Add( const T& _el )
	{
		for (int i = 0; i < el.size(); i++)
		{
			if (el[i] == _el) return i;
		}
		el.push_back( _el );
		return (int)el.size() - 1;
	}

	int		GetNElem() const { return (int)el.size(); }
	void	Clear() { el.clear(); }

	T&		Elem( int idx ) { assert( idx >= 0 && idx < el.size() ); return el[idx]; }
}; // class LinearRegistry

#endif __KTEMPLATES_HPP_
