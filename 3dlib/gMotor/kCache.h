/*****************************************************************/
/*  File:   Cache.h
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __KCACHE_H__
#define __KCACHE_H__
#include "gmDefines.h"

#include <vector>
using namespace std;

/*****************************************************************/
/*  Class:    Cached
/*  Desc:    
/*****************************************************************/
class Cached
{
public:
    Cached();
    ~Cached();

    _inl void hit() const;
    _inl bool haveData() const { return data != 0; }
    _inl BYTE* getDataPtr() const { return data; }

protected:
    mutable int LRU;
    BYTE*        data;

private:
    friend class Cache;
};  // class Cached

/*****************************************************************/
/*  Class:    Cache
/*  Desc:    
/*****************************************************************/
class Cache
{
public:
    Cache();
    ~Cache();

    bool        ReserveSpace( int numBytes );
    _inl BYTE*    AllocBuf( Cached* item, int numBytes );
    _inl void    FreeBuf( Cached* item );

    int            getMemUsed() const { return memUsed; }

protected:
private:
    int            memUsed;
};  // class Cache

/*****************************************************************/
/*  Class:    CacheClient
/*  Desc:    Object which uses cached resources
/*****************************************************************/
class CacheClient
{
    bool    valid;
public:
    CacheClient() : valid( false ) {}

    void invalidate()        { valid = false;    }
    void validate()            { valid = true;        }
    bool isInvalid() const    { return !valid;    }
}; // class CacheClient

/*****************************************************************/
/*  Class:    MCacheClient
/*  Desc:    Object which uses cached resources
/*****************************************************************/
class MCacheItem;
class MCacheClient
{
    MCacheItem*            hostItem;    
public:
    MCacheClient() : hostItem( 0 ) {}

    void invalidate()                    { hostItem = 0;                }
    void validate( MCacheItem * item )    { hostItem = item;            }
    bool isInvalid() const                { return (hostItem == 0);    }
    _inl int& element();                        
    _inl void hit( int factor );
}; // class CacheClient

/*****************************************************************************/
/*    Class:    MCacheItem
/*    Desc:    Single item of the multi-client cache
/*****************************************************************************/
class MCacheItem
{    
public:
    int                        cacheElem;
    WORD                    idx;
    int                        factor;
    vector<MCacheClient*>    clients;

public:
    _inl void                reset();
    _inl void                hit( int factorVal ) 
                            { 
                                factor = factorVal; 
                            }
    _inl int&                element() 
                            { 
                                return cacheElem; 
                            }
    _inl void                addClient( MCacheClient* client )
                            {
                                clients.push_back( client );    
                            }
    void                    invalidate()
    {
        for (int i = 0; i < clients.size(); i++)
        {
            clients[i]->invalidate();
        }
        clients.clear();
    }
};  // class MCacheItem

/*****************************************************************************/
/*    Class:    MCache
/*    Desc:    Cache which maintans more than one client on the each item
/*                Items are managed on the LRU basis
/*****************************************************************************/
template <int poolSize, class TItem = MCacheItem>
class MCache
{
    TItem        pool[poolSize];

public:
    MCache()
    {
        for (int i = 0; i < poolSize; i++) pool[i].idx = i; 
    }

    TItem*     allocItem()
    {
        int itemID = 0;
        int minfactor = pool[0].factor;
        for (int i = 1; i < poolSize; i++)
        {
            if (pool[i].factor < minfactor) 
            {
                itemID = i;
                minfactor = pool[i].factor;
                if (minfactor == 0) break;
            }
        }
        
        pool[itemID].reset();
        return &pool[itemID];
    }
    int            GetPoolSize() const { return poolSize; }
    int            element( int idx ) 
    {  
        assert( idx >= 0 && idx < poolSize );
        return pool[idx].element();
    }

    TItem& operator []( int idx )
    {
        assert( idx >= 0 && idx < poolSize );
        return pool[idx];
    }
}; // class MCache

/*****************************************************************************/
/*    Class:    CacheItem
/*    Desc:    Single item of the single-client cache
/*****************************************************************************/
template <class T>
class CacheItem
{    
public:
    T                        item;
    int                        LRU;
    CacheClient*            client;

    _inl void                purgeClient();
    _inl void                hit();
};  // class MultiCacheItem

/*****************************************************************************/
/*    Class:    SingleClientCache
/*    Desc:    Cache which maintans one client on the each item
/*                Items are managed on the LRU basis
/*****************************************************************************/
template <int poolSize, class T>
class SingleClientCache
{
    CacheItem<T>    pool[poolSize];
public:
    _inl int         allocItem();
    _inl T&            getItem( int itemID );
    _inl void        hitItem( int itemID );
    _inl void        setClient( int itemID, CacheClient* _client );
    _inl int        getPoolSize() const;

}; // class SingleClientCache

/*****************************************************************************/
/*    Class:    LUCache
/*    Desc:    Lock/unlock-style cache
/*****************************************************************************/
template <int poolSize, class T>
class LUCache
{
    T        pool[poolSize];            //  pool of resources
    int        freeStack[poolSize];    //  stack of indices of free resources
    int        freeTop;                //  top of the freeStack

public:
    _inl    T&        obtainItem();
    _inl    void    lockItem();
    _inl    void    unlockItem();

}; // class LUCache

#ifdef _INLINES
#include "kCache.inl"
#endif // _INLINES

#endif // __KCACHE_H__