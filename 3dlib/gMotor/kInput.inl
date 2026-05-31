/*****************************************************************************/
/*	File:	kInput.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-13-2003
/*****************************************************************************/

/*****************************************************************************/
/*	InputDispatcher implementation
/*****************************************************************************/
_inl bool InputDispatcher::MouseWheel( int delta )					
{
	if (pDisp) return pDisp->OnMouseWheel( delta );
	return false;
}

_inl bool InputDispatcher::MouseMove( int mX, int mY, DWORD keys )	
{
	if (pDisp) return pDisp->OnMouseMove( mX, mY, keys );	
	return false;
}

_inl bool InputDispatcher::MouseLButtonDown( int mX, int mY )				
{
	if (pDisp) return pDisp->OnMouseLButtonDown( mX, mY );	
	return false;
}

_inl bool InputDispatcher::MouseMButtonDown( int mX, int mY )				
{
	if (pDisp) return pDisp->OnMouseMButtonDown( mX, mY );	
	return false;
}

_inl bool InputDispatcher::MouseRButtonDown( int mX, int mY )	
{
	if (pDisp) return pDisp->OnMouseRButtonDown( mX, mY );	
	return false;
}

_inl bool InputDispatcher::MouseLButtonUp( int mX, int mY )		
{
	if (pDisp) return pDisp->OnMouseLButtonUp( mX, mY );	
	return false;
}

_inl bool InputDispatcher::MouseMButtonUp( int mX, int mY )		
{
	if (pDisp) return pDisp->OnMouseMButtonUp( mX, mY );	
	return false;
}

_inl bool InputDispatcher::MouseRButtonUp( int mX, int mY )
{
	if (pDisp) return pDisp->OnMouseRButtonUp( mX, mY );	
	return false;
}

_inl bool InputDispatcher::MouseLButtonDblclk( int mX, int mY )
{
	if (pDisp) return pDisp->OnMouseLButtonDblclk( mX, mY );	
	return false;
}

_inl bool InputDispatcher::MouseMButtonDblclk( int mX, int mY )
{
	if (pDisp) return pDisp->OnMouseMButtonDblclk( mX, mY );	
	return false;
}

_inl bool InputDispatcher::MouseRButtonDblclk( int mX, int mY )	
{
	if (pDisp) return pDisp->OnMouseRButtonDblclk( mX, mY );	
	return false;
}

_inl bool InputDispatcher::KeyDown( DWORD keyCode, DWORD flags )
{
	if (pDisp) return pDisp->OnKeyDown( keyCode, flags );	
	return false;
}

_inl bool InputDispatcher::Char( DWORD charCode, DWORD flags )
{
	if (pDisp) return pDisp->OnChar( charCode, flags );	
	return false;
}

_inl bool InputDispatcher::KeyUp( DWORD keyCode, DWORD flags )
{
	if (pDisp) return pDisp->OnKeyUp( keyCode, flags );	
	return false;
}


_inl void InputDispatcher::Update()
{
	if (pDisp) pDisp->OnUpdate();	
}

_inl void InputDispatcher::Draw()
{
	if (pDisp) pDisp->OnDraw();	
}

_inl void InputDispatcher::Init()
{
	if (pDisp) pDisp->OnInit();	
}

_inl void InputDispatcher::SetCoreDispatcher( InputDispatcher* _pDisp )
{
	pDisp = _pDisp; 
}

_inl void InputDispatcher::SetActive( bool _active )
{
	bActive  = _active;
	OnActivate( _active );
}

_inl bool InputDispatcher::IsActive() const
{
	return bActive == TRUE;
}


