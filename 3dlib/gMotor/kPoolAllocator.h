/*****************************************************************************/
/*    File:    kPoolAllocator.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    09-16-2003
/*****************************************************************************/
#ifndef __KPOOLALLOCATOR_H__
#define __KPOOLALLOCATOR_H__

/*****************************************************************************/
/*    Class:    PoolAllocator
/*    Desc:    Generic paged pool allocator
/*****************************************************************************/
template <int PageSize = 65536>
class PoolAllocator
{
protected:

    struct PoolPage
    {
        BYTE*               buf;
        PoolPage*           nextPage;
        int                 curSize;
        int                 nAlloc;
        int                 maxSize;

                    PoolPage() : buf(NULL), nextPage(NULL), nAlloc(0) {}
    
        BYTE*        Allocate( int numBytes )
        {
            if (maxSize - curSize < numBytes)
            {
                return NULL;
            }
            
            BYTE* pBuf = &buf[curSize];
            curSize += numBytes;
            nAlloc++;
            return pBuf;
        }

        PoolPage*    GetLastPage()
        {
            PoolPage* pPage = this;
            while (pPage->nextPage) pPage = pPage->nextPage;
            return pPage;
        }
                

    }; // class PoolPage

    PoolPage*           firstPage;
    PoolPage*           curPage;
    int                 pageSize;
    int                 nPages;

public:
                        PoolAllocator() : firstPage(NULL), curPage(NULL), 
                                            pageSize(PageSize), nPages(0) {}
    ~PoolAllocator()
    {
        Purge();
    } // PoolAllocator::~PoolAllocator

    virtual PoolPage*    AddPage()
    {
        PoolPage* substPage = OnAddPage();
        if (substPage) return substPage;

        BYTE* pBuf = new BYTE[pageSize];
        PoolPage* newPage = new( pBuf ) PoolPage();
        newPage->buf        = pBuf + sizeof( PoolPage );
        newPage->curSize    = 0;
        newPage->maxSize    = pageSize - sizeof( PoolPage );

        nPages++;

        if (curPage) curPage->nextPage = newPage;
        curPage = newPage;
        return curPage;
    }    

    virtual PoolPage*    OnAddPage()
    {
        return NULL;
    }

    void                SetPageSize( int size )
    {
        pageSize = size;
    }

    void Purge()
    {
        PoolPage* pPage = firstPage;
        while (pPage)
        {
            pPage->nAlloc = 0;
            pPage->curSize = 0;
            pPage = pPage->nextPage;
        }
        curPage   = firstPage;
    } // PoolAllocator::Purge
    
    void*                Allocate( size_t numBytes ) 
    { 
        if (!curPage) firstPage = AddPage();
        assert( curPage );
        void* pMem = curPage->Allocate( numBytes );
        while (!pMem) 
        {
            if (curPage->nextPage)
            {
                curPage = curPage->nextPage;
            }
            else
            {
                AddPage();
            }
            pMem = curPage->Allocate( numBytes );
        }
        assert( pMem );
        return pMem; 
    }

}; // class PoolAllocator

template <class T, int PageSize = 65536>
class TypedPoolAllocator : public PoolAllocator<PageSize>
{
    int                    maxPages;
    int                    nDrops;
    
    static const int c_DefaultMaxPages = 8;

public:
            TypedPoolAllocator<T, PageSize>() : maxPages(c_DefaultMaxPages), nDrops(0) {}
    
    T*        NewInstance( DWORD extraBytes = 0 )
    {
        DWORD chunkSize = sizeof( T ) + extraBytes + sizeof( DWORD );
        DWORD* buf = reinterpret_cast<DWORD*>( Allocate( chunkSize ) );
        assert( buf );
        *buf = chunkSize;
        return new(buf + 1) T();
    }

    virtual PoolPage*    OnAddPage()
    {    return NULL;
        if (nPages < maxPages) return NULL;
        
        //  drop first page
        PoolPage* pPage = firstPage;
        firstPage = firstPage->nextPage;
        curPage->GetLastPage()->nextPage = pPage;
        pPage->nextPage = NULL;
        curPage = pPage;
        DropPage( pPage );

        pPage->curSize = 0;
        pPage->nAlloc = 0;
        return pPage; 
    }

    void    SetMaxPages( int nPages ) { maxPages = nPages; }
    void    DropPage( PoolPage* page )
    {
        nDrops++;
        BYTE* pBuf = page->buf;
        for (int i = 0; i < page->nAlloc; i++)
        {
            DWORD objSz = *((DWORD*)pBuf);
            T* pObj = reinterpret_cast<T*>( pBuf + 4 );
            pObj->Drop();
            pBuf += objSz;
        }
    }

}; // class TypedPoolAllocator
//
///*****************************************************************************/
///*    Class:    pool_allocator
///*    Desc:    STL-compatible pool allocator
///*****************************************************************************/
//template<class T, PoolAllocator* allocator = &g_Pool> 
//class pool_allocator 
//{
//public:
//    typedef size_t                size_type;
//    typedef ptrdiff_t            difference_type;
//    typedef T*                    pointer;
//    typedef const T*            const_pointer;
//    typedef T&                    reference;
//    typedef const T&            const_reference;
//    typedef T                    value_type;
//
//    pointer                        address( reference x )            const { return &x; }
//    const_pointer                address( const_reference x )    const { return &x; }
//
//    explicit                    pool_allocator() {}
//    pointer                        allocate    ( size_type n, const void* ) 
//    { 
//        return reinterpret_cast<pointer>( allocator->Allocate( n * sizeof( T ) ) ); 
//    }
//    pointer                        _Charalloc    (size_t n) 
//    {
//        return reinterpret_cast<pointer>( allocator->Allocate( n ) );
//    }
//    void                        deallocate    ( void*, size_type ) {}
//    void                        deallocate    ( pointer, size_type ) {}
//    void                        construct    ( pointer p, const T& val ) { new ((void*)p) T( val ); }
//    void                        destroy        ( pointer p ) { p->T::~T(); }
//    bool                        operator ==    ( const pool_allocator& rhs ) 
//    { 
//        return &allocator == &rhs.allocator; 
//    }
//    bool                        operator !=    ( const pool_allocator& rhs ) 
//    { 
//        return &allocator != &rhs.allocator; 
//    }
//    size_type                    max_size() const 
//    { 
//        return static_cast<size_type>( -1 ) / sizeof( T ); 
//    }
//
//    template<class _Other>
//    struct rebind
//    {    
//        typedef pool_allocator<_Other> other;
//    };
//}; // class pool_allocator

#endif // __KPOOLALLOCATOR_H__
