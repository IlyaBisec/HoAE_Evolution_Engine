/*****************************************************************************/
/*	File:	uiNodeFactoryBrowser.h
/*	Desc:	Dialog for creating new nodes of any type
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#ifndef __UINODEFACTORYBROWSER_H__
#define __UINODEFACTORYBROWSER_H__

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	Class:	NodeFactoryBrowser
/*	Desc:	Visual represenation of the node factory
/*				User created nodes picking fom the node type tree
/*****************************************************************************/
class NodeFactoryBrowser : public TreeBrowser
{
public:
								NodeFactoryBrowser		();
	virtual						~NodeFactoryBrowser		();

	virtual bool				CreateTree				();
	bool						CreateTreeFromFactory	();
	bool						CreateTreeFromFile		();
	virtual bool 				OnMouseLButtonDown		( int mX, int mY );
	virtual bool 				OnMouseLButtonUp		( int mX, int mY );
	
	_inl const char*			GetStructureFile		() const { return m_StructureFile; }
	_inl void					SetStructureFile		( const char* path ) { m_StructureFile = path; }

	virtual void				Expose					( PropertyMap& pm );


	NODE(NodeFactoryBrowser, TreeBrowser, NFBR);

protected:

	class ClassDescNode : public TreeBrowserNode
	{
	public:
		ClassDescNode() : m_pClassDesc(NULL), m_bBeingCreated(false){}
		const NodeClassDesc*			m_pClassDesc;
		bool							m_bBeingCreated;
	}; // class ClassDescNode

	bool						m_bLeafsOnly;		//  indicates whether only leaf nodes can be created
	c2::string					m_StructureFile;

	bool						m_bCreatingNode;

}; // class NodeFactoryBrowser

END_NAMESPACE(sg)


#endif // __UINODEFACTORYBROWSER_H__
