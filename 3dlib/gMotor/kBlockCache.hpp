/*****************************************************************************/
/*	File:	kBlockCache.hpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	19.09.2004
/*****************************************************************************/
#ifndef __KBLOCKCACHE_HPP__
#define __KBLOCKCACHE_HPP__

/*****************************************************************************/
/*  Class:  BlockCache
/*  Desc:   Generic block-based cache 
/*****************************************************************************/
template <class TBlock, class TItem, int NBlocks>
class BlockCache
{
    TBlock      m_Block     [NBlocks];
    DWORD       m_LastUsed  [NBlocks];
    int         m_LastBlock;

public:
    BlockCache()
    {
        Reset();
    }

    void Reset()
    {
        memset( &m_LastUsed, 0, sizeof(m_LastUsed) );
        m_LastBlock = 0;
    }

    int Allocate( TItem* pItem )
    {
        if (!pItem) return -1;
        if (m_Block[m_LastBlock].Allocate( *pItem )) return m_LastBlock;
        //  search for least recently hit surface
        m_LastBlock = (m_LastBlock + 1)%NBlocks;
        DWORD lru = m_LastUsed[m_LastBlock];
        for (int i = 0; i < NBlocks; i++)
        {
            if (m_LastUsed[i] < lru)
            {
                m_LastBlock = i;
                lru = m_LastUsed[i];
            }
        }
        m_Block[m_LastBlock].Free();
        if (!m_Block[m_LastBlock].Allocate( *pItem )) return -1;
        return m_LastBlock;
    } // Allocate

    //  updates block's last used value
    void HitBlock( int blockIdx )
    {
        static DWORD cHit = 1;
        m_LastUsed[blockIdx] = cHit++;
    }

    const TBlock& GetBlock( int idx ) const 
    {  
        assert( idx >= 0 && idx < NBlocks );
        return m_Block[idx];
    }

    TBlock& GetBlock( int idx )
    {  
        assert( idx >= 0 && idx < NBlocks );
        return m_Block[idx];
    }
}; // class BlockCache

#endif // __KBLOCKCACHE_HPP__
