/*****************************************************************************/
/*	File:	uiNodeFactoryBrowser.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kPropertyMap.h"
#include "sgFont.h"
#include "uiControl.h"
#include "uiTreeBrowser.h"
#include "uiNodeFactoryBrowser.h"

BEGIN_NAMESPACE(sg)
NodeFactoryBrowser::NodeFactoryBrowser()
{
	m_bCreatingNode		= false;
	m_bLeafsOnly		= false;

	m_ItemHeight		= 16;
	m_VertStep			= 2;
	m_FontName			= "Tahoma";
	m_FontHeight		= 9;

	m_GlyphTextureName	= "sgIcons.tga";
	m_GlyphSide			= 16;
}

NodeFactoryBrowser::~NodeFactoryBrowser()
{
}

void NodeFactoryBrowser::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "NodeFactoryBrowser", this );
	pm.f( "Only Leafs",	 m_bLeafsOnly	);
	pm.p ( "Structure File", GetStructureFile, SetStructureFile, "file" );
}

bool NodeFactoryBrowser::CreateTreeFromFactory()
{
	return false;

	std::vector<ClassDescNode*>	nodes;

	int nTypes = NodeFactory::instance().GetNTypes();
	nodes.reserve( nTypes );

	for (int i = 0; i < nTypes; i++)
	{
		ClassDescNode* pNode = new ClassDescNode();
		pNode->m_pClassDesc = &NodeFactory::instance().GetTypeDesc( i );	
		pNode->m_Caption	= pNode->m_pClassDesc->className;
		pNode->m_FgColor	= m_FgColor;
		nodes.push_back( pNode );
	}

	for (int i = 0; i < nTypes; i++)
	{
		const char* className = nodes[i]->m_pClassDesc->className;
		for (int j = 0; j < nTypes; j++)
		{
			if (!strcmp( nodes[j]->m_pClassDesc->parentClassName, className))
			{
				nodes[i]->m_Children.push_back( nodes[j] );
				nodes[j]->m_pParent = nodes[i];
			}
		}
	}

	for (int i = 0; i < nTypes; i++)
	{
		ClassDescNode* pNode = nodes[i];
		pNode->m_BgColor = pNode->m_Children.size() ? m_ClrMdl : m_LeafBgColor;
	}

	for (int i = 0; i < nTypes; i++) 
	{
		if (nodes[i]->m_pParent == NULL) SetRoot( nodes[i] );
	}

	if (m_pRoot) m_pRoot->m_bVisible = true;

	LayoutNodes();
	return false;
} // NodeFactoryBrowser::CreateTreeFromFactory

bool NodeFactoryBrowser::OnMouseLButtonUp( int mX, int mY )
{
	TreeBrowser::OnMouseLButtonUp( mX, mY );

	if (m_bCreatingNode)
	{
		ClassDescNode* pNode = (ClassDescNode*)GetNodeFromPt( mX, mY );
		if (pNode && m_pHostNode &&
			!(m_bLeafsOnly && pNode->GetNChildren() > 0))
		{
			sg::Node* pSGNode = NodeFactory::instance().CreateNode( pNode->m_pClassDesc->magic );

			char name[256];
			sprintf( name, "%s%d", pNode->m_pClassDesc->className, pNode->m_pClassDesc->numCreated );
			pSGNode->SetName( name );
			m_pHostNode->AddChild( pSGNode );
		}
		m_bCreatingNode = false;
		TBNodeIterator it( GetRoot() );
		while (it)
		{
			((ClassDescNode*)(TreeBrowserNode*)it)->m_bBeingCreated = false;
			++it;
		}
	}

	return false;
} // NodeFactoryBrowser::OnMouseLButtonUp

bool NodeFactoryBrowser::OnMouseLButtonDown( int mX, int mY )
{
	if (TreeBrowser::OnMouseLButtonDown( mX, mY )) return true;

	ClassDescNode* pNode = (ClassDescNode*)GetNodeFromPt( mX, mY );
	if (pNode)
	{
		//  create scene graph node of the selected type
		if (m_pHostNode)
		{
			pNode->m_bBeingCreated = true;
			m_bCreatingNode = true;
		}
	}
	return false;
} // NodeFactoryBrowser::OnMouseLButtonDown

bool NodeFactoryBrowser::CreateTreeFromFile()
{
	return false; 
} // NodeFactoryBrowser::CreateTreeFromFile

bool NodeFactoryBrowser::CreateTree()
{
	return CreateTreeFromFactory();
} // NodeFactoryBrowser::CreateTree

END_NAMESPACE(sg)
