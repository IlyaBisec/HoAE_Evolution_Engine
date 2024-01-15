/*****************************************************************************/
/*	File:	sgNode.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.04.2003
/*****************************************************************************/

/*****************************************************************************/
/*	SNode implementation
/*****************************************************************************/
_inl void SNode::Render()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
        SNode* pChild = GetChild( i );
		if (!pChild) continue;
        if (pChild->GetFlagState( nfInvisible )) continue;
        if (s_bRenderTMOnly && !m_bTransform) continue;
        pChild->Render();
	}
} // SNode::Render

_inl void SNode::SetParent( SNode* pNode )
{
	m_pParent = pNode;
} // SNode::SetParent

_inl void SNode::AddChild( SNode* pNode )
{
	if (!pNode) return;
	m_Children.push_back( pNode );
	pNode->AddRef();
	pNode->SetParent( this );
} // SNode::AddChild

_inl void SNode::AddInput( SNode* pNode )
{
	if (!pNode) return;
	m_Children.push_back( pNode );
	pNode->AddRef();
}

_inl void SNode::SwapChildren( int ch1, int ch2 )
{
	if (ch1 < 0 || ch2 < 0 || ch1 >= GetNChildren() || ch2 >= GetNChildren()) return;
	SNode* temp = m_Children[ch1];
	m_Children[ch1] = m_Children[ch2];
	m_Children[ch2] = temp;
}

_inl int SNode::GetChildIndex( SNode* pChild ) const
{
	for (int i = 0; i < GetNChildren(); i++) if (GetChild( i ) == pChild) return i;
	return -1;
} // SNode::GetChildIndex

_inl void SNode::AddChild( SNode* pNode, int position )
{
	if (!pNode) return;
	if (position >= GetNChildren()) position = GetNChildren() - 1;
    if (position < 0) position = 0;

	m_Children.insert( m_Children.begin() + position, pNode );
	pNode->AddRef();
	pNode->SetParent( this );
} // SNode::AddChild

_inl bool SNode::Owns( const SNode* pNode ) const
{
	if (!pNode) return false;
	if (pNode->GetParent() == this) return true;
	return false;
}

_inl bool SNode::GetFlagState( NodeFlags flag ) const
{
	return (m_Flags & flag) != 0;
}

_inl void SNode::SetFlagState( NodeFlags flag, bool state )
{
	if (state == true)
	{
		m_Flags |= flag;
	}
	else
	{
		m_Flags &= ~flag;
	}
} // SNode::SetFlagState

_inl bool SNode::HasInput( SNode* pChild )
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		if (pChild == GetChild( i ) && !Owns( GetChild( i ) )) return true;
	}
	return false;
} // SNode::HasInput

_inl bool SNode::HasChild( SNode* pChild, bool bSearchSubtree )
{
	if (!bSearchSubtree)
	{
		for (int i = 0; i < GetNChildren(); i++)
		{
			if (GetChild( i ) == pChild) return true;
		}
		return false;
	}

	Iterator it( this );
	while (it)
	{
		if (((SNode*)(*it)) == pChild) return true;
		++it;
	}
	return false;
} // SNode::HasChild

_inl SNode* SNode::GetInput( int idx )
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		if (!Owns( GetChild( i ) ))
		{
			if (idx == 0) return GetChild( i );
			idx--;
		}
	}
	return NULL;
} // SNode::GetInput

_inl const char* SNode::GetClassName() const 
{ 
	return ClassName(); 
}

_inl bool SNode::IsInvisible() const	
{ 
	return GetFlagState( nfInvisible ); 
}

_inl void SNode::SetInvisible( bool val )	
{ 
	SetFlagState( nfInvisible, val ); 
}

_inl bool SNode::HasFocus() const
{
    return GetFlagState( nfHasFocus ); 
}
_inl void SNode::SetFocus( bool val )
{
    SetFlagState( nfHasFocus, val ); 
}

_inl bool SNode::IsImmortal() const				
{ 
	return GetFlagState( nfImmortal ); 
}

_inl void SNode::SetImmortal( bool val )	
{ 
	SetFlagState( nfImmortal, val ); 
}

_inl bool SNode::IsDisabled() const				
{ 
	return GetFlagState( nfDisabled ); 
}

_inl void SNode::SetDisabled( bool val )	
{ 
	SetFlagState( nfDisabled, val ); 
}

_inl bool SNode::DoDrawGizmo() const				
{ 
	return GetFlagState( nfDrawGizmo ); 
}

_inl void SNode::SetDrawGizmo( bool val )	
{ 
	SetFlagState( nfDrawGizmo, val ); 
}

_inl bool SNode::DoDrawAABB() const				
{ 
	return GetFlagState( nfDrawAABB ); 
}

_inl void SNode::SetDrawAABB( bool val )	
{ 
	SetFlagState( nfDrawAABB, val ); 
}

