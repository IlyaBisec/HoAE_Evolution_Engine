/*****************************************************************************/
/*    File:    kMemorySpy.cpp
/*    Desc:    Gathers statistics about memory blocks allocations/deletions
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11.03.2004
/*****************************************************************************/
#include "stdafx.h"
#include "kMemorySpy.h"

#include <vector>
#include <string>
#include <map>

/*****************************************************************************/
/*    Struct:    MemoryBlock
/*    Desc:    Block of the allocated memory
/*****************************************************************************/
struct MemoryBlock
{
    void*            m_pData;        //  pointer to the data block
    int                m_Size;            //  size of the block
    int                m_CallerID;        //  block allocator ID
    DWORD            m_AllocTime;    //  time of the allocation
    int                m_ID;            //  block identifier

    MemoryBlock() : m_pData(0), m_Size(0), 
                    m_CallerID(-1), m_AllocTime(0), m_ID(-1)
    {}
    
}; // struct MemoryBlock

/*****************************************************************************/
/*    Struct:    MemoryAllocator
/*    Desc:    Someone who allocates memory
/*****************************************************************************/
struct MemoryAllocator
{
    std::string            m_Name;
    int                    m_BytesAllocated;
    int                    m_ID;
    int                    m_NBlocks;

    MemoryAllocator() : m_Name("Unknown"), m_BytesAllocated(0), 
                        m_ID(-1), m_NBlocks(0)
    {}

}; // struct MemoryBlock

/*****************************************************************************/
/*    Class:    MemorySpy
/*    Desc:    Tracker of memory allocations
/*****************************************************************************/
class MemorySpy
{
public:
            MemorySpy    ();
            ~MemorySpy    ();

    void    AllocBlock    ( void* pData, int size, const char* name );
    void    FreeBlock    ( void* pData );
    void    Dump        ();

private:
    std::vector<MemoryBlock>            m_Blocks;
    std::vector<MemoryAllocator>        m_Allocators;

    typedef std::map<std::string, int>    AllocatorMap;
    typedef std::map<void*, int>        BlockMap;

    AllocatorMap    m_AllocatorMap;
    BlockMap        m_BlockMap;
    int                m_TotalAllocated; // how much bytes we ate totally
}; // class MemorySpy

//  instance of the manager
MemorySpy    g_MemorySpy;
/*****************************************************************************/
/*    MemorySpy implementation
/*****************************************************************************/
MemorySpy::MemorySpy()
{
    m_TotalAllocated = 0;
}

MemorySpy::~MemorySpy()
{
    assert( m_TotalAllocated == 0 );
    Dump();
}

void MemorySpy::Dump()
{
    for (int i = 0; i < m_Allocators.size(); i++)
    {
        MemoryAllocator& mal = m_Allocators[i];
        Log.Info( "Allocator %s: blocks=%d, bytes=%d", mal.m_Name.c_str(), 
                    mal.m_NBlocks, 
                    mal.m_BytesAllocated );
    }
} // MemorySpy::Dump

void MemorySpy::AllocBlock( void* pData, int size, const char* name )
{
    //  find allocator
    AllocatorMap::iterator it = m_AllocatorMap.find( std::string( name ) );
    MemoryAllocator* pAlloc = NULL;
    if (it != m_AllocatorMap.end())
    {
        pAlloc = &(m_Allocators[(*it).second]);
    }
    else
    {
        MemoryAllocator mal;
        m_Allocators.push_back( mal );
        int id = m_Allocators.size() - 1;
        pAlloc = &(m_Allocators[id]);
        pAlloc->m_Name = name;
        pAlloc->m_ID = id;
        m_AllocatorMap[name] = pAlloc->m_ID;    

    }
    assert( pAlloc );
    pAlloc->m_BytesAllocated += size;
    pAlloc->m_NBlocks++;
    //  add block
    MemoryBlock block;
    m_Blocks.push_back( block );
    int blockID            = m_Blocks.size() - 1;
    MemoryBlock* pBlock = &(m_Blocks[blockID]);
    pBlock->m_CallerID    = pAlloc->m_ID;
    pBlock->m_ID        = blockID;
    pBlock->m_pData        = pData;
    pBlock->m_Size        = size;
    m_BlockMap[pData]    = blockID;
} // MemorySpy::AllocBlock

void MemorySpy::FreeBlock( void* pData )
{

}

/*****************************************************************************/
/*    Globals
/*****************************************************************************/
void msAllocBlock( void* pData, int size, const char* name )
{
    g_MemorySpy.AllocBlock( pData, size, name );
}

void msFreeBlock( void* pData )
{
    g_MemorySpy.FreeBlock( pData );
}