/*****************************************************************************/
/*	File:	sgController.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.04.2003
/*****************************************************************************/

#include "mSplines.h"

/*****************************************************************************/
/*	Controller implementation
/*****************************************************************************/
_inl void Controller::AttachNode( SNode* _pNode )
{
	AddChild( _pNode );
	OnAttach();
} // Controller::AttachNode

_inl void Controller::DetachNode( SNode* _pNode )
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		if (GetChild( i ) == _pNode)
		{
			RemoveChild( i );
		}
	}
} // Controller::AttachToNode


