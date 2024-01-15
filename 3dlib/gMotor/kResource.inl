/*****************************************************************************/
/*	File:	kResource.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	27.02.2003
/*****************************************************************************/

/*****************************************************************************/
/*	BaseResource implementation
/*****************************************************************************/
_inl bool BaseResource::operator <( const BaseResource& res ) 
{ 
	return usedFactor < res.usedFactor; 
}

_inl bool BaseResource::IsDismissed()	
{ 
	return dismissed;					
}

_inl void BaseResource::Hit( DWORD factor )		
{ 
	usedFactor = factor;				
}

_inl bool BaseResource::Touch()					
{ 
	if (!dismissed) return true; 
	return Restore();
}

_inl bool BaseResource::IsLocked() const
{
	return locked;
}

_inl void BaseResource::Lock()
{
	locked = true;
}

_inl void BaseResource::Unlock()
{
	locked = false;
}
	
_inl DWORD BaseResource::GetFactor() const	
{ 
	return usedFactor;				
}
