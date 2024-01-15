/*****************************************************************/
/*  File:   kCache.inl
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/

_inl void Cached::hit() const
{
	LRU = GetTickCount();
}

_inl BYTE* Cache::AllocBuf( Cached* item, int numBytes )
{
	BYTE* buf = new BYTE[numBytes];
	memUsed += numBytes;
	item->data = buf;
	return buf;
}

_inl void Cache::FreeBuf( Cached* item )
{
	delete []item->data;
	item->data = 0;
}

/*****************************************************************************/
/*	MCacheItem implementation
/*****************************************************************************/
_inl void MCacheItem::reset()
{
	vector<MCacheClient*>::iterator it = clients.begin();
	for(; it != clients.end(); it++) 
	{
		(*it)->invalidate();
	}


	clients.clear();
}

_inl void MCacheClient::hit( int factor )
{ 
	if (hostItem) hostItem->hit( factor ); 
}

_inl int& MCacheClient::element()
{	
	assert( hostItem );
	return hostItem->element();
}

/*****************************************************************************/
/*	SingleClientCache implementation
/*****************************************************************************/
template <int poolSize, class T>
_inl int SingleClientCache<poolSize, T>::allocItem()
{
	int itemID = 0;
	int maxLRU = GetTickCount();
	for (int i = 0; i < poolSize; i++)
	{
		if (pool[i].LRU < maxLRU) 
		{
			itemID = i;
			maxLRU = pool[i].LRU;
			if (maxLRU == 0) break;
		}
	}
	pool[itemID].LRU = GetTickCount();
	pool[itemID].purgeClient();

	return itemID;
}  // SingleClientCache::allocItem

template <int poolSize, class T>
_inl T&	SingleClientCache<poolSize, T>::getItem( int itemID )
{
	assert( itemID >= 0 && itemID < poolSize ); 
	return pool[itemID].item;
}  // SingleClientCache::getItem

template <int poolSize, class T>
_inl void SingleClientCache<poolSize, T>::hitItem( int itemID )
{
	assert( itemID >= 0 && itemID < poolSize ); 
	pool[itemID].hit();
}  // SingleClientCache::hitItem

template <int poolSize, class T>
_inl void SingleClientCache<poolSize, T>::setClient( int itemID, CacheClient* _client )
{
	assert( itemID >= 0 && itemID < poolSize ); 
	pool[itemID].client = _client;
} // SingleClientCache::addClient

template <int poolSize, class T>
_inl int SingleClientCache<poolSize, T>::getPoolSize() const
{
	return poolSize;
}

/*****************************************************************************/
/*	CacheItem implementation
/*****************************************************************************/
template <class T>
_inl void CacheItem<T>::purgeClient()
{
	if (!client) return;
	client->invalidate();
	client = 0;
}

template <class T>
_inl void CacheItem<T>::hit()
{
	LRU = GetTickCount();
}
