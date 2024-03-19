/*****************************************************************************/
/*	File:	uiTreeBrowser.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgMovable.h"
#include "sgFont.h"
#include "kInput.h"
#include "uiControl.h"
#include "sgApplication.h"
#include "uiTreeBrowser.h"

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	TreeBrowser::TreeBrowserNode implemetation
/*****************************************************************************/
TreeBrowser::TreeBrowserNode::TreeBrowserNode()
{
	m_pParent		= NULL;
	m_bCollapsed	= true;
	m_bSelected		= false;
	m_bVisible		= false;
	m_bIsBoundNode	= false;
	m_Priority		= 0.0f;
	m_GlyphID		= -1;

	m_FgColor		= 0xFFFFFFFF;
	m_BgColor		= 0x44AAAAAA;
}

TreeBrowser::TreeBrowserNode::~TreeBrowserNode()
{
	Clear();
}

void TreeBrowser::TreeBrowserNode::Clear()
{
	for (int i = 0; i < m_Children.size(); i++)
	{
		delete m_Children[i];
	}
	m_Children.clear();
} 

int	TreeBrowser::TreeBrowserNode::GetChildIdx( TreeBrowserNode* pChild )
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		if (GetChild( i ) == pChild) return i;
	}
	return -1;
}

void TreeBrowser::TreeBrowserNode::SortChildren()
{
	assert( false );
}

void TreeBrowser::TreeBrowserNode::MoveSubtree( float dx, float dy )
{
	TBNodeIterator it( this );
	while (it)
	{
		TreeBrowserNode* pNode = it;
		pNode->m_Ext.x += dx;
		pNode->m_Ext.y += dy;
		++it;
	}
} // TreeBrowser::TreeBrowserNode::MoveSubtree

/*****************************************************************************/
/*	TreeBrowser implemetation
/*****************************************************************************/
TreeBrowser::TreeBrowser()
{
	m_pRoot			= NULL;
	m_pHostNode		= NULL;

	m_SelectedColor		= 0x660000FF;
	m_UncollapsedColor	= 0x66000066;
	
	m_FgColor			= 0xFFFFFFFF;
	m_LinesColorBeg		= 0x1199FFFF;
	m_LinesColorEnd		= 0x33AAFFFF;
	m_LeafBgColor		= 0x6656534E;


	m_pSelectedNode 	= NULL;
	m_MinItemWidth		= 60;
	m_MaxItemWidth		= 200;
	m_HorzStep			= 16; 
	m_ItemHeight		= 12;
	m_VertStep			= 4;
	m_Border			= 3;

	m_ClrTop			= 0x66FFFFFF;
	m_ClrMdl			= 0x66D6D3CE;
	m_ClrBot			= 0x66848284;

	m_FontName			= "Tahoma";
	m_FontHeight		= 9;
	
	m_GlyphSide			= 0;
	m_pIconSet			= NULL;

	SetExtents( AppWindow::instance()->GetExtents() );
}

TreeBrowser::~TreeBrowser()
{
	delete m_pRoot;
}

void TreeBrowser::ClearTree()
{
	delete m_pRoot;
	m_pRoot = NULL;
}

void TreeBrowser::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "TreeBrowser", this );
	pm.f( "SelectedColor",		m_SelectedColor,	"color" );
	pm.f( "UncollapsedColor",		m_UncollapsedColor,	"color" );
	pm.f( "TextColor",			m_FgColor,			"color" );
	pm.f( "LinesColorBeg",		m_LinesColorBeg,	"color" );
	pm.f( "LinesColorEnd",		m_LinesColorEnd,	"color" );
	pm.f( "LeafBgColor",			m_LeafBgColor,		"color" );
	pm.f( "MinItemWidth",			m_MinItemWidth	);
	pm.f( "MaxItemWidth",			m_MaxItemWidth	);
	pm.f( "Horz Step",			m_HorzStep		);
	pm.f( "Vert Step",			m_VertStep		);
	pm.f( "Item Height",			m_ItemHeight	);
	pm.f( "Border",				m_Border		);
} // TreeBrowser::Expose

GlyphSet* TreeBrowser::GetIconSet()
{
	if (!m_pIconSet)
	{
		const char* texName = m_GlyphTextureName;
		m_pIconSet = FindChild<IconSet>( "IconSet" );
		if (!m_pIconSet && texName && texName[0] != 0)
		{
			m_pIconSet = AddChild<IconSet>( "IconSet" );
			m_pIconSet->SetIconWidth ( m_GlyphSide );
			m_pIconSet->SetIconHeight( m_GlyphSide );
			m_pIconSet->AddTexture( texName );
		}
	}
	return m_pIconSet;
} // TreeBrowser::GetIconSet

void TreeBrowser::Render()
{
	Font* pFont = GetFont();

	GetIconSet();

	static DeviceStateSet* pLineShader = AddChild<DeviceStateSet>( "lines3D_blend" );

	if (!m_pRoot) 
	{
		CreateTree();
		LayoutNodes();
	}

	if (!m_pRoot) return;

	TBNodeIterator it( m_pRoot );

	int cNode = 0;
	while (it)
	{
		TreeBrowserNode* pNode = it;
		++it;

		if (!pNode->m_bVisible) continue;

		TreeBrowserNode* pParent = pNode->m_pParent;

		DWORD bgColor = pNode->GetNChildren() ? m_ClrMdl : pNode->m_BgColor;
		bgColor = pNode->m_bCollapsed ? bgColor : m_UncollapsedColor;
		bgColor = pNode->m_bSelected ? m_SelectedColor : bgColor;

		Rct nodeExt = ClientToScreen( pNode->m_Ext );

		Vector3D pos( nodeExt.x + 3, nodeExt.y + 2, GetPosZ() );

		if (pParent) 
		{
			Rct pExt = ClientToScreen( pParent->m_Ext );
			rsLine( pExt.GetRight() - 1, 
				pExt.GetCenterY(), 
				nodeExt.x, nodeExt.GetCenterY(),
				GetPosZ(), 
				m_LinesColorBeg, 
				m_LinesColorEnd );
		}
		
		if (!nodeExt.IsOutside( IRS->GetViewPort() ))
		{
			rsPanel( nodeExt, GetPosZ(), m_ClrTop, bgColor, m_ClrBot );
			if (m_pIconSet)
			{
				Vector3D gpos( nodeExt.x + 1, nodeExt.y, GetPosZ() );
				m_pIconSet->AddGlyph( gpos, pNode->m_GlyphID, 0xFFFFFFFF );
				pos.x += m_GlyphSide;
			}

			pFont->AddString( pos, pNode->m_Caption, pNode->m_FgColor );
		}
		cNode++;
	}

	pLineShader->Render();
	rsFlushPoly2D();
	rsFlushLines2D();

	Node::Render();
} // TreeBrowser::Render

int TreeBrowser::CalcColWidth( int desiredWidth )
{
	if (m_pIconSet) desiredWidth += m_GlyphSide;
	if (desiredWidth < m_MinItemWidth) desiredWidth = m_MinItemWidth;
	if (desiredWidth > m_MaxItemWidth) desiredWidth = m_MaxItemWidth;
	return desiredWidth + m_Border * 2.0f;
} // TreeBrowser::CalcColWidth

void TreeBrowser::LayoutNodes()
{
	if (!m_pRoot) return;
	Font* pFont = GetFont();
	GetIconSet();

	//  place the root
	Rct wExt = GetExtents();
	float textW = pFont->GetStringWidth( m_pRoot->m_Caption );
	Rct ext( wExt.x, wExt.GetCenterY(), CalcColWidth( textW ), m_ItemHeight );
	m_pRoot->m_Ext = ext;

	TBNodeIterator it( m_pRoot );
	while (it)
	{
		TreeBrowserNode* pNode = it;
		TreeBrowserNode* pParent = pNode->m_pParent;
		if (pParent)
		{
			Rct pExt  = pParent->m_Ext;
			int nCh   = pParent->GetNChildren();
			int myIdx = pParent->GetChildIdx( pNode );
			float width = m_MinItemWidth;

			float colHeight = float( nCh ) * (m_ItemHeight + m_VertStep) - m_VertStep;
			float top = pExt.y - 0.5f * colHeight + 0.5f * m_ItemHeight;
			if (top < wExt.y) top = wExt.y;
			if (top + colHeight > wExt.GetBottom()) top = wExt.GetBottom() - colHeight;
			assert( myIdx >= 0 );
			ext.Set( pExt.GetRight() + m_HorzStep, 
				top + myIdx * (m_ItemHeight + m_VertStep),
				m_MinItemWidth, m_ItemHeight );
			pNode->m_Ext = ext;
		}
		++it;
	}

	//  correct column widths
	it.Reset();
	while (it)
	{
		TreeBrowserNode* pNode = it;
		textW = 0.0f;
		for (int i = 0; i < pNode->GetNChildren(); i++)
		{
			float w = pFont->GetStringWidth( pNode->GetChild( i )->m_Caption );
			if (w > textW) textW = w;

			if (i == 0 || i + 1 == pNode->GetNChildren()) 
			{
				pNode->GetChild( i )->m_bIsBoundNode = true;
			}
		}
		textW = CalcColWidth( textW );
		for (int i = 0; i < pNode->GetNChildren(); i++)
		{
			TreeBrowserNode* pChild = pNode->GetChild( i );
			float dw = textW - pChild->m_Ext.w;
			pChild->MoveSubtree( dw, 0 );
			pChild->m_Ext.w += dw;
			pChild->m_Ext.x -= dw;
		}
		pNode->m_Ext = ScreenToClient( pNode->m_Ext );
		++it;
	}	

	//  initial visibility
	m_pRoot->m_bVisible = true;

} // TreeBrowser::LayoutNodes

bool TreeBrowser::OnMouseLButtonUp( int mX, int mY )
{
	return false;
} // TreeBrowser::OnMouseLButtonUp

bool TreeBrowser::OnMouseLButtonDblclk( int mX, int mY )				
{
	TreeBrowserNode* pNode = GetNodeFromPt( mX, mY );
	if (pNode == m_pRoot) SetInvisible( true );
	return false;
} // TreeBrowser::OnMouseLButtonDblclk

bool TreeBrowser::OnMouseMButtonUp( int mX, int mY )
{
	if (IsDragged())
	{
		EndDrag( mX, mY );
		return true;
	}
	return false; 
} // TreeBrowser::OnMouseMButtonUp

bool TreeBrowser::OnMouseMButtonDown( int mX, int mY )
{
	TreeBrowserNode* pNode = GetNodeFromPt( mX, mY );
	if (pNode)
	{
		BeginDrag( mX, mY );
	} 
	return false;
} // TreeBrowser::OnMouseMButtonDown

bool TreeBrowser::OnMouseLButtonDown( int mX, int mY )
{
	TreeBrowserNode* pNode = GetNodeFromPt( mX, mY );
	if (pNode)
	{
		if (pNode == m_pSelectedNode && !pNode->m_bCollapsed) 
		{
			CollapseAll();
			UncollapseBranch( pNode );
			pNode->m_bCollapsed = true;
			for (int i = 0; i < pNode->GetNChildren(); i++)
			{
				pNode->GetChild( i )->m_bVisible = false;
			}
			return false;
		}

		if (m_pSelectedNode) m_pSelectedNode->m_bSelected = false;
		m_pSelectedNode = pNode;
		pNode->m_bSelected  = true;
		CollapseAll();
		UncollapseBranch( pNode );
		return false;
	}

	return false;
} // TreeBrowser::OnMouseLButtonDown


bool TreeBrowser::OnMouseMove( int mX, int mY, DWORD keys )
{
	if (IsDragged())
	{
		OnDrag( mX, mY );
		return true;
	}

	return false;
} // TreeBrowser::OnMouseMove

void TreeBrowser::CollapseAll()
{
	TBNodeIterator it( m_pRoot );
	while (it)
	{
		TreeBrowserNode* pNode = it;
		pNode->m_bCollapsed = true;
		++it;
	}

	it.Reset();
	while (it)
	{
		TreeBrowserNode* pNode = it;
		if (pNode->m_pParent) pNode->m_bVisible = !pNode->m_pParent->m_bCollapsed;
		++it;
	}

} // TreeBrowser::CollapseAll

void TreeBrowser::UncollapseBranch( TreeBrowser::TreeBrowserNode* pNode )
{
	while (pNode)
	{
		pNode->m_bCollapsed = false;
		pNode = pNode->m_pParent;
	}

	TBNodeIterator it( m_pRoot );
	while (it)
	{
		TreeBrowserNode* pNode = it;
		if (pNode->m_pParent) pNode->m_bVisible = !pNode->m_pParent->m_bCollapsed;
		++it;
	}
} // TreeBrowser::UncollapseBranch

TreeBrowser::TreeBrowserNode* TreeBrowser::GetNodeFromPt( int x, int y )
{
	if (!m_pRoot) return NULL;
	float px = x;
	float py = y;
	ScreenToClient( px, py );
	TBNodeIterator it( m_pRoot );
	while (it)
	{
		TreeBrowserNode* pNode = it;
		if (pNode->m_bVisible && pNode->m_Ext.PtIn( px, py )) return pNode;
		++it;
	}
	return NULL;
} // TreeBrowser::GetNodeFromPt


void TreeBrowser::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void TreeBrowser::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
}

Vector3D TreeBrowser::GetRootPosition() const
{
	if (!m_pRoot) return Vector3D::null;
	return Vector3D( m_pRoot->m_Ext.x, m_pRoot->m_Ext.y, 0.0f ); 
} // TreeBrowser::GetRootPosition

void TreeBrowser::SetRootPosition( const Vector3D& pos )
{
	if (!m_pRoot) return;
	//MoveSubtree( )
} // TreeBrowser::SetRootPosition


END_NAMESPACE(sg)



