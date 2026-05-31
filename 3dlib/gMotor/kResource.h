/*****************************************************************************/
/*    File:    kResource.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#ifndef __KRESOURCE_H__
#define __KRESOURCE_H__
#pragma once

#include <string>
#include <vector>

/*****************************************************************************/
/*    Class:    BaseResource
/*    Desc:    Basic resource type
/*****************************************************************************/
class BaseResource
{
protected:
    bool                dismissed;
    bool                locked;
    DWORD                usedFactor;
    DWORD                tag;
    
public:    
                        BaseResource() : dismissed( true ), usedFactor( curCacheFactor ) 
                        {
                            static int s_Tag    = 0;
                            tag                    = s_Tag++;
                            locked                = false;
                        }
    virtual                ~BaseResource(){}

    _inl    bool        IsDismissed        ();        
    _inl    void        Hit                ( DWORD factor );        
    _inl    bool        Touch            ();        
    _inl    bool        IsLocked        () const;
    _inl    void        Lock            ();
    _inl    void        Unlock            ();
    _inl    DWORD        GetFactor        () const;    

    _inl    bool        operator <( const BaseResource& res );
    
    virtual int            GetDismissableSizeBytes() const    = 0;
    virtual int            GetHeaderSizeBytes() const        = 0;
    
    virtual bool        InitPrefix()                    = 0;
    virtual void        Dismiss()                        = 0;
    virtual bool        Restore()                        = 0;
    virtual void        Dump();
    

    static DWORD        curCacheFactor;

}; // class BaseResource

/*****************************************************************************/
/*    Class:    BaseResourceMgr
/*    Desc:    Basic resource manager
/*****************************************************************************/
template <class TRes>
class BaseResourceMgr
{
    std::string            descr;
public:

    _inl TRes*        Res( int ID ) 
    {
        assert( ID >= 0 && ID < nRes );
        return resPointer[ID];
    }

    _inl const TRes&    c_Res( int ID ) const
    {
        assert( ID >= 0 && ID < nRes );
        return resPointer[ID];
    }

    _inl int    NumRes() const
    {
        return nRes;
    }

    _inl void SetDescription( const char* description )
    {
        descr = description;
    }

protected:
    virtual int                AllocRes() = 0;

private:
    std::vector<TRes*>        resPointer;
}; // class BaseResourceMgr

/*****************************************************************************/
/*    Class:    DynamicResMgr 
/*****************************************************************************/
template <class TRes, class TKey, 
            int        tableSize    = c_DefTableSize, 
            int        minPoolSize = c_DefMinPoolSize>
class DynamicResMgr
{
    PointerHash<TRes,TKey, tableSize, minPoolSize>        hash;
    int                                                    memoryBudget;
    int                                                    nUsedBytes;
public:
    DynamicResMgr() {}
    ~DynamicResMgr()
    {
        for (int i = 0; i < hash.numElem(); i++)
        {
            //void* v=(void*)hash.elem( i );
            //delete(v);
            delete hash.elem( i );
        }
    }

    _inl int FindResource( const TKey& key )
    {
        return hash.find( key );
    }

    int InsertResource( TRes* pRes, const TKey& key )
    {
        int resID = hash.add( key, pRes );
        return resID;
    }

    TRes* GetResource( int resID )
    {
        TRes* res = hash.elem( resID );
        res->Hit( BaseResource::curCacheFactor );
        if (res->IsDismissed())
        //  rise up uninitialized or previously dismissed resource
        {
            while (!FitsInMemoryBudget( res->GetDismissableSizeBytes() ))
            {
                DismissLRU();
            }
            res->Restore();
            nUsedBytes += res->GetDismissableSizeBytes();
        }
        return res;
    }

    bool FitsInMemoryBudget( int nBytes )
    {
        return nUsedBytes + nBytes < memoryBudget;
    }

    void SetMemoryBudget( int nBytes )
    {
        memoryBudget    = nBytes;
        nUsedBytes        = 0;
    }

    void DismissLRU()
    {
        int nRes = hash.numElem();
        DWORD minfactor = BaseResource::curCacheFactor;
        TRes* candidate = hash.elem( 0 );
        for (int i = 0; i < nRes; i++)
        {
            TRes* res = hash.elem( i );
            if (!res->IsDismissed() && res->GetFactor() < minfactor)
            {
                minfactor = res->GetFactor();
                candidate = res;
            }
        }
        assert( candidate );
        candidate->Dismiss();
        nUsedBytes -= candidate->GetDismissableSizeBytes();
    }
    
}; // class DynamicResMgr

#ifdef _INLINES
#include "kResource.inl"
#endif // _INLINES

#endif // __KRESOURCE_H__