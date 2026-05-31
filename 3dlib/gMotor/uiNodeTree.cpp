/*****************************************************************************/
/*    File:    uiNodeTree.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-07-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kPropertyMap.h"
#include "uiWidget.h"
#include "uiWindow.h"
#include "uiCheckBox.h"
#include "uiButton.h"
#include "uiNodeTree.h"
#include "IFontManager.h"
#include "uiObjectInspector.h"
#include "uiMainFrame.h"

int GetEditorGlyphsID();
int GetEditorFontID();

/*****************************************************************************/
/*    NodeTree implementation
/*****************************************************************************/
IMPLEMENT_CLASS(NodeTree);
NodeTree::NodeTree()
{
    m_Root                  = NULL;
    m_pDragged              = NULL;
    m_Depth                 = 0;

    m_LinesColorBeg         = 0xFF99FFFF;
    m_LinesColorEnd         = 0xFFAAFFFF;
    m_TextColor             = 0xFF000040;
    m_DefaultNodeColor      = 0xCCD6D3CE;
    
    m_bRightHand            = false;
    m_bSelCollapse          = false;
    m_bShowGlyphs           = true;
    m_bDragLeafsOnly        = false;
    m_bDropToItself         = false;
    m_bDragCopy             = true;
    m_bHasVisibleRoot       = true;
    m_bAcceptOnDrop         = true;
    m_bEditable             = true;

    m_NodeWidth             = 83;
    m_NodeHeight            = 20;
    m_HNodeSpacing          = 10;
    m_VNodeSpacing          = 0;

    m_RootX                 = 10;
    m_RootY                 = 768.0f*0.5f;

    AlignToParent   ();
    SetDraggable    ();
    SetFocusable    ( false );

    SetName( "NodeTree" );
} // NodeTree::NodeTree

void NodeTree::SetVisibleRoot( bool val ) 
{ 
    m_bHasVisibleRoot = val; 
} // NodeTree::SetVisibleRoot

void NodeTree::SetAcceptOnDrop( bool val ) 
{ 
    m_bAcceptOnDrop = val; 
}

void NodeTree::SetEditable( bool val ) 
{
    m_bEditable = val; 
}

void NodeTree::SetDragLeafsOnly( bool val ) 
{ 
    m_bDragLeafsOnly = val; 
}

void NodeTree::SetRootPos( int x, int y ) 
{ 
    m_RootX = x; 
    m_RootY = y; 
} // NodeTree::SetRootPos   

bool NodeTree::OnMouseLBDown( int mX, int mY )
{
    IReflected* pNode = PickNode( mX, mY );

    if (pNode)
    {
        IReflected* pSel  = GetSelectedNode();
        if (pNode == pSel) 
        {
            m_bSelCollapse = !m_bSelCollapse;
        }
        else
        {
            SelectNode( pNode );
            m_bSelCollapse = false;
        }
    }
    return false;
} // NodeTree::OnMouseLBDown

bool NodeTree::OnKeyDown( DWORD keyCode, DWORD flags )
{
    if (keyCode == VK_DELETE && m_bEditable)
    {
        IReflected* pSel = GetSelectedNode();    
        if (!pSel || pSel == GetRootNode()) return false;
        IReflected* pParent = pSel->Parent();    
        if (!pParent) return false;
        pParent->DelChild( pSel );
        SelectNode( pParent );
        return true;
    }
    return false;
} // NodeTree::OnKeyDown

void NodeTree::SelectPrev()
{

}

void NodeTree::SelectNext()
{

}

void NodeTree::SwapPrev()
{

}

void NodeTree::SwapNext()
{

}

IReflected* NodeTree::GetSelectedNode() const
{
    IReflected* pNode = GetRootNode();
    int cDepth = 0;
    while (cDepth < m_Depth)
    {
        IReflected* pChild = pNode->Child( m_Path[cDepth] );
        if (!pChild) return pNode;
        pNode = pChild;
        cDepth++;
    }
    return pNode;
} // NodeTree::GetSelectedNode

IReflected* NodeTree::GetDraggedNode() const
{
    return m_pDragged;
}

IReflected* NodeTree::GetRootNode() const
{
    return m_Root;
}

void NodeTree::SelectNode( IReflected* pNode )
{
    IReflected::Iterator it( GetRootNode() );
    m_Depth = 0;
    while (it)
    {
        IReflected* pCur = *it;
        IReflected* pParent = pCur->Parent();
        if (pCur == pNode)
        {
            m_Depth = it.GetDepth();
            memcpy( m_Path, it.GetIdxPath(), m_Depth * sizeof( int ) );
            return;
        }
        ++it;
    }
} // NodeTree::SelectNode

void NodeTree::SetRootNode( IReflected* pNode )
{
    if (m_Root == pNode) return;
    m_Root = pNode;
    SelectNode( pNode );
}

void NodeTree::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "NodeTree", this );
    pm.f( "RootX",            m_RootX                );
    pm.f( "RootY",            m_RootY                );
    pm.f( "NodeWidth",        m_NodeWidth            );
    pm.f( "NodeHeight",        m_NodeHeight        );
    pm.f( "HNodeSpacing",    m_HNodeSpacing        );
    pm.f( "VNodeSpacing",    m_VNodeSpacing        );        
    pm.f( "RightHand",        m_bRightHand        );
    pm.f( "ShowGlyphs",        m_bShowGlyphs        );
    pm.f( "DragLeafsOnly",    m_bDragLeafsOnly    );
    pm.f( "DropToItself",    m_bDropToItself        );
    pm.f( "DragCopy",        m_bDragCopy            );
    pm.f( "VisibleRoot",    m_bHasVisibleRoot    );
    pm.f( "AcceptOnDrop",    m_bAcceptOnDrop        );

    pm.f( "LinesColorBeg",    m_LinesColorBeg,    "color" );
    pm.f( "LinesColorEnd",    m_LinesColorEnd,    "color" );
    pm.f( "TextColor",        m_TextColor,        "color" );
    pm.f( "DefaultNodeColor", m_DefaultNodeColor, "color" );
} // NodeTree::Expose

DWORD NodeTree::GetNodeBgColor( IReflected* pNode ) const 
{     
    DWORD clr = m_DefaultNodeColor;
    clr = pNode->GetColor();
    return clr;
} // NodeTree::GetNodeBgColor

bool NodeTree::DrawNode( IReflected* pNode, const Rct& rct, IReflected* pParent, const Rct& prct )
{
    int fontID = GetEditorFontID();

    static int ifaceID = ISM->GetPackageID( "kangaroo_interface" );
    static int iconsID = ISM->GetPackageID( "kangaroo_icons" );

    if (!pNode) return false;

    int glyphID = pNode->GetGlyphID();
    DWORD clr = GetNodeBgColor( pNode );

    ColorValue clrTop( 0xFFEEEEEE );
    ColorValue clrMdl( clr );
    ColorValue clrBot( 0xFF848284 );
    clrTop *= clrMdl;
    clrBot *= clrMdl;

    if (pNode->NumChildren() > 0)
    {
        clrTop.a *= 0.8f;
        clrMdl.a *= 0.8f;
        clrBot.a *= 0.8f;

    }
    else
    {
        clrTop.a *= 0.3f;
        clrMdl.a *= 0.3f;
        clrBot.a *= 0.3f;
    }

    if (pParent)
    {
        if (m_bRightHand)
        {
            rsLine( rct.GetRight(), rct.y + rct.h/2, 
                    prct.x - 1, prct.y + prct.h/2, 
                    0.0f, m_LinesColorBeg, m_LinesColorEnd );
        }
        else
        {
            rsLine( prct.GetRight(), prct.y + prct.h/2, 
                    rct.x - 1, rct.y + m_NodeHeight/2, 
                    0.0f, m_LinesColorBeg, m_LinesColorEnd );
        }
    }
    
    int bgSpriteID = -1;
    if (pNode->NumChildren() > 0)
    {
        bgSpriteID = 3;
    }
    else
    {
        bgSpriteID = 2;
    }
    
    if (pNode == m_RootNode && m_bHasVisibleRoot)
    {
        ISM->SetCurrentDiffuse( clr );
        ISM->DrawSprite( ifaceID, 0, Vector3D( rct.x, rct.y, 0.0f ) );
        ISM->DrawSprite( iconsID, glyphID, Vector3D( rct.x + 10, rct.y + 10, 0.0f ) );
        if (pNode == m_SelNode)
        {
            ISM->SetCurrentDiffuse( 0xFFFF5555 );
            ISM->DrawSprite( ifaceID, 1, Vector3D( rct.x, rct.y, 0.0f ) );
        }
        return false;
    }
    

    if (!rct.Overlap( m_ClipRct )) return false;

    DWORD textColor = m_TextColor;

    if (pNode->NumChildren() > 0)
    {
        ISM->SetCurrentDiffuse( 0xDDFFFFFF );
    }
    else
    {
        ISM->SetCurrentDiffuse( 0x77FFFFFF );
    }

    ISM->DrawSprite( ifaceID, bgSpriteID, Vector3D( rct.x, rct.y, 0.0f ) );

    if (pNode == m_SelNode && pNode != m_RootNode)
    {
        ISM->SetCurrentDiffuse( 0xFFFF5555 );
        ISM->DrawSprite( ifaceID, 4, Vector3D( rct.x, rct.y, 0.0f ) );
    }

    //  draw glyph
    Rct rcGlyph( rct );
    ISM->SetCurrentDiffuse( 0xCCFFFFFF );
    rcGlyph.y += 2;
    ISM->DrawSprite( iconsID, glyphID, Vector3D( rcGlyph.x, rcGlyph.y, 0.0f ) );
    ISM->Flush();

    Rct cRct( rct );
    cRct.x += rct.w - rct.h + 1;
    cRct.w = cRct.h;
    cRct.y += 2;
    cRct.Inflate( -4 );
    rsRect( cRct, 0.0f, clr );
    rsFlushPoly2D();
    rsFrame( cRct, 0.0f, 0xFF000000 );
    rsFlushLines2D();

    //  draw text
    Rct txtRct( rct.x + rct.h + 8, rct.y + 7, rct.w - rct.h - 8, rct.h - 2 );
    if (pNode->NumChildren() > 0) txtRct.w -= 10;

    const char* nodeName = pNode->GetName();
    //  if text is not fitting into our box, then clip it with viewport
    float len = IWM->GetStringWidth( fontID, nodeName );
    if (len > txtRct.w)
    {
        ISM->Flush();
        IWM->FlushText( fontID );
        Rct vp = IRS->GetViewPort();
        IRS->SetViewPort( txtRct );
        IWM->DrawString( fontID, nodeName, Vector3D( txtRct.x, txtRct.y, 0.0f ), textColor );
        ISM->Flush();
        IWM->FlushText( fontID );
        IRS->SetViewPort( vp );
    }
    else
    {
        IWM->DrawString( fontID, nodeName, Vector3D( txtRct.x, txtRct.y, 0.0f ), textColor );
    }
    return false;
} // NodeTree::DrawNode

IReflected* NodeTree::PickNode( int mX, int mY ) const
{
    Rct ext = GetScreenExtents();
    m_MX = mX + ext.x;
    m_MY = mY + ext.y;
    m_PickResult = NULL;
    ((NodeTree*)this)->Iterate(&NodeTree::PickNode );
    return m_PickResult;
} // NodeTree::PickNode

bool NodeTree::OnDrag( float dx, float dy )
{
    if (s_DragButton&mbMiddle) 
    {
        m_RootX += dx;
        m_RootY += dy;
    }
    return false;
} // NodeTree::OnDrag

bool NodeTree::PickNode( IReflected* pNode, const Rct& rct, IReflected* pParent, const Rct& prct )
{
    if (rct.PtIn( m_MX, m_MY )) 
    {
        m_PickResult = pNode;
        m_PickRct     = rct;
        return true;
    }
    return false;
} // NodeTree::PickNode

void NodeTree::Render()
{
    m_SelNode    = GetSelectedNode();
    m_RootNode    = GetRootNode();
    m_ClipRct    = IRS->GetViewPort();

    POINT pt;
    GetCursorPos( &pt );
    m_MX = pt.x;
    m_MY = pt.y;

    ISM->Flush();
    Iterate(&NodeTree::DrawNode );
    
    if (m_pDragged)
    {
        Rct rct( m_MX - m_NodeWidth/2, m_MY - m_NodeHeight/2, m_NodeWidth, m_NodeHeight );
        DrawNode( m_pDragged, rct );
    }

    ISM->Flush();
    rsFlush();

    IWM->FlushText( GetEditorGlyphsID() );
    IWM->FlushText( GetEditorFontID() );
} // NodeTree::Render

Rct NodeTree::GetRootRct() const
{
    Rct ext = GetScreenExtents();
    return Rct( m_RootX + ext.x, m_RootY + ext.y - 2, 40, 40 );
} // NodeTree::GetRootRct

void NodeTree::SetRightHand( bool val ) 
{ 
    m_bRightHand = val; 
} // NodeTree::SetRightHand

void NodeTree::Iterate( ItCallback process )
{
    IReflected* pRoot        = GetRootNode();
    IReflected* pNode        = pRoot;
    if (!pNode) return;

    Rct rct( GetRootRct() );

    rct.x += GetExtents().x;
    rct.y += GetExtents().y;

    if (m_bHasVisibleRoot) (this->*process)( pNode, rct, NULL, Rct::null );
    int cDepth = 0;
    Rct pRct( rct );

    while (cDepth <= m_Depth && pNode)
    {
        if (m_bSelCollapse && cDepth == m_Depth) break;
        int nCh = pNode->NumChildren();

        int colH = nCh*(m_NodeHeight + m_VNodeSpacing) - m_VNodeSpacing;
        rct.y = pRct.y + rct.h/2 - colH/2;
        
        float dw = rct.w + m_HNodeSpacing;
        if (m_bRightHand) dw = -(m_HNodeSpacing + m_NodeWidth);
        rct.x += dw;
        
        //  find maximal node width
        int w = m_NodeWidth;
        /*int fontID = GetEditorFontID();
        for (int i = 0; i < nCh; i++)
        {
            IReflected* pChild = pNode->Child( i );
            float len = IWM->GetStringWidth( fontID, pChild->GetName() ) + m_NodeHeight + 7;
            if (len > w) w = len;
        }*/

        rct.w = w;
        rct.h = m_NodeHeight;

        IReflected* pSelNode = NULL;
        Rct selRct;
        Rct newPRct( rct );
        IReflected* pParent = pNode->Parent();

        for (int i = 0; i < nCh; i++)
        {
            IReflected* pChild = pNode->Child( i );    

            Rct nodeRct( rct );

            if (i == m_Path[cDepth]) newPRct = rct;            
            rct.y += m_NodeHeight + m_VNodeSpacing;

            //  skip the node being currently moved
            if (pChild == m_pDragged && !m_bDragCopy) continue;
            if (pChild == m_SelNode) 
            {
                selRct = nodeRct;
                pSelNode = m_SelNode;
                continue;
            }
            //  execute callback for the node
            bool bStop = false;
            if (!m_bHasVisibleRoot && pNode == pRoot) 
            {
                bStop = (this->*process)( pChild, nodeRct, NULL, Rct::null );
            }
            else
            {
                bStop = (this->*process)( pChild, nodeRct, pNode, pRct );
            }
            if (bStop) 
            {
                for (int j = cDepth - 1; j >= 0; j--) m_StopPath[j] = m_Path[j];
                m_StopDepth = cDepth;
                m_StopPath[cDepth - 1] = i;
                return;
            }
        }

        if (pSelNode) (this->*process)( pSelNode, selRct, NULL, pRct );

        pRct = newPRct;
        pNode = pNode->Child( m_Path[cDepth] );
        cDepth++;
    }
} // NodeTree::Iterate

bool NodeTree::OnStartDrag( int mX, int mY )
{
    IReflected* pSel = PickNode( mX, mY );
    if (pSel) 
    {
        m_pDragged = pSel; 
        if (m_bDragLeafsOnly && pSel->NumChildren() != 0) 
        {
            m_pDragged = NULL;
            return false;
        }
        return true;
    }
    m_pDragged = NULL; 
    return false;
} // NodeTree::OnStartDrag

IReflected* NodeTree::OnEndDrag()
{
    if (m_pDragged && IsVisible()) 
    {
        Widget* pDropTarget = MainFrame::instance().PickWidget( s_MouseX, s_MouseY );
        if (!m_bDragCopy && m_pDragged)
        {
            IReflected* pParent = m_pDragged->Parent();
            if (pParent) pParent->DelChild( m_pDragged );
        }
    }
    IReflected* pRes = m_pDragged; 
    m_pDragged = NULL;
    return pRes;
} // NodeTree::OnEndDrag

void NodeTree::OnDrop( int mX, int mY, IReflected* pDropped )
{
    if (!IsVisible() || !pDropped) return;

    if (!m_bAcceptOnDrop || !m_bEditable) return;
    IReflected* pParent = PickNode( mX, mY );

    if (!pParent && !m_bHasVisibleRoot && pDropped != this)
    {
        pParent = GetRootNode();
    }

    if (!pParent || !pDropped || pParent == pDropped) return;
    IReflected* pInstance = pDropped->Clone();
    if (pParent->AddChild( pInstance ))
    {
        SelectNode( pInstance );
    }

    m_bSelCollapse = false;
} // NodeTree::OnDrop


