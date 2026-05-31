/*****************************************************************************/
/*	File:	uiTreeBrowser.h
/*	Desc:	Dialog for creating new nodes of any type
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#ifndef __UITREEBROWSER_H__
#define __UITREEBROWSER_H__

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	Class:	TreeBrowser
/*	Desc:	Visual represenation of the node factory
/*				User created nodes picking fom the node type tree
/*****************************************************************************/
class TreeBrowser : public Dialog
{
public:
								TreeBrowser				();
	virtual						~TreeBrowser			();

	virtual void				Render					();
	virtual void				Serialize				( OutStream& os ) const;
	virtual void				Unserialize				( InStream& is );
	virtual void				Expose					( PropertyMap& pm );

	virtual bool				CreateTree				() { return false; }
	
	virtual bool				OnMouseMove				( int mX, int mY, DWORD keys );

	virtual bool				OnMouseLButtonUp		( int mX, int mY );
	virtual bool				OnMouseLButtonDblclk	( int mX, int mY );				
	virtual bool				OnMouseLButtonDown		( int mX, int mY );
	
	virtual bool				OnMouseMButtonUp		( int mX, int mY );
	virtual bool				OnMouseMButtonDown		( int mX, int mY );


	_inl void					SetHostNode				( Node* pNode ) { m_pHostNode = pNode; }
	_inl Node*					GetHostNode				() const { return m_pHostNode; }


	NODE(TreeBrowser, Dialog, TRBR);

protected:
	/*****************************************************************************/
	/*	Class:	TreeBrowserNode
	/*	Desc:	Node of the class hierarchy
	/*****************************************************************************/
	class TreeBrowserNode
	{
	public:
							TreeBrowserNode			();
							~TreeBrowserNode		();
		void				Clear					();
		int					GetChildIdx				( TreeBrowserNode* pChild );

		int					GetNChildren			() const		{ return m_Children.size(); }
		TreeBrowserNode*	GetChild				( int idx )		
		{ 
			if (idx < 0 || idx >= m_Children.size()) return NULL;
			return m_Children[idx];	
		}
		void				SortChildren			();
		void				MoveSubtree				( float dx, float dy );
		
		virtual DWORD		GetGlyphID				() const { return 0; }
		void				AddChild				( TreeBrowserNode* pNode )
		{
			if (pNode) m_Children.push_back( pNode );
		}


		std::vector<TreeBrowserNode*>	m_Children;
		TreeBrowserNode*				m_pParent;

		//  visualities
		c2::string						m_Caption;
		bool							m_bCollapsed;
		Rct								m_Ext;
		bool							m_bSelected;
		bool							m_bVisible;
		bool							m_bIsBoundNode;
		float							m_Priority;
		DWORD							m_FgColor;
		DWORD							m_BgColor;
		int								m_GlyphID;

		friend class					TreeBrowser;
	}; // class TreeBrowserNode

	typedef TreeIterator<TreeBrowserNode>			TBNodeIterator;

	void						LayoutNodes				();
	void						CollapseAll				();
	void						UncollapseBranch		( TreeBrowserNode* pNode );
	TreeBrowserNode*			GetNodeFromPt			( int x, int y );
	int							CalcColWidth			( int desiredWidth );

	void						SetRoot					( TreeBrowserNode* pRoot ) { m_pRoot = pRoot; }
	TreeBrowserNode*			GetRoot					() const { return m_pRoot; }

	Vector3D					GetRootPosition			() const;
	void						SetRootPosition			( const Vector3D& pos );

	void						ClearTree				();
	GlyphSet*					GetIconSet				();


	Node*						m_pHostNode;

protected:
	TreeBrowserNode*			m_pRoot;
	TreeBrowserNode*			m_pSelectedNode;
	IconSet*					m_pIconSet;

	GlyphSet					m_pGlyphSet;

	DWORD						m_SelectedColor;
	DWORD						m_UncollapsedColor;
	DWORD						m_FgColor;
	DWORD						m_LeafBgColor;
	DWORD						m_LinesColorBeg;
	DWORD						m_LinesColorEnd;

	int							m_MinItemWidth;
	int							m_MaxItemWidth;
	int							m_HorzStep;	
	int							m_VertStep;			
	int							m_ItemHeight;		
	int							m_Border;			//  border width inside item box

	c2::string					m_GlyphTextureName;
	int							m_GlyphSide;
	
}; // class TreeBrowser

END_NAMESPACE(sg)


#endif // __UITREEBROWSER_H__
