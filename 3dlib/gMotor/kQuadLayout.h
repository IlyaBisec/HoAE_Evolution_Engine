/*****************************************************************/
/*  File:   kQuadLayout.h
/*  Desc:   Sprite manager
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Sep 2004
/*****************************************************************/
#ifndef __KQUADLAYOUT_H__
#define __KQUADLAYOUT_H__

const WORD    c_PowSidesWORD[]  = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768 };
const float c_PowSidesFloat[] = { 1.0f, 2.0f, 4.0f, 8.0f, 16.0f, 32.0f, 64.0f, 128.0f, 256.0f, 512.0f, 1024.0f, 2048.0f, 4096.0f, 8192.0f, 16384.0f, 32768.0f };
const int     c_MaxQuadLevels    = 16;    //  maximal powers of quad side in texture layout

struct QuadPos
{
    float x, y;
}; // struct QuadPos

/*****************************************************************************/
/*    Struct:    QuadLayoutLevel
/*    Desc:    Helper structure for packing sprite chunks on the texture surface
/*****************************************************************************/
struct QuadLayoutLevel
{
    QuadPos        quad[3];
    WORD        numQuads;
};  // struct QuadLayoutLevel

/****************************************************************************/
/*  Class:  QuadLayout                                        
/*  Desc:    Describes layout of sprite chunks on the texture surface
/****************************************************************************/
class QuadLayout
{
    QuadLayoutLevel            m_Level[c_MaxQuadLevels + 1];
    int                        m_CurLevel;
    int                        m_MaxLevel;
    int                        m_NFreePixels;
    int                        m_MaxPixels;

public:
                QuadLayout        () : m_MaxLevel( c_MaxQuadLevels ) {}
    inline bool    AllocChunk        ( int sidePow, WORD& allocx, WORD& allocy );
    int            NumFreePixels    () const { return m_NFreePixels; }
    
    void Init( int texSide )
    {
        m_NFreePixels = texSide * texSide;
        m_MaxLevel = 0;
        while (texSide > 1) 
        {
            texSide >>= 1;
            m_MaxLevel++;                
        }
        Free();
    }

    void Free()
    {
        //  make all quads "free"
        memset( &m_Level, 0, sizeof( m_Level ) );
        m_CurLevel = m_MaxLevel;
        m_Level[m_MaxLevel].numQuads = 1;
        m_NFreePixels = m_MaxPixels;
    }
}; //  class QuadLayout

/*****************************************************************
/*  QuadLayout implementation                                 
/*****************************************************************/

/*---------------------------------------------------------------*
/*  Func:    QuadLayout::AllocChunk
/*    Desc:    Finds the place on the surface for the given chunk
/*---------------------------------------------------------------*/
inline bool QuadLayout::AllocChunk( int sidePow, WORD& allocx, WORD& allocy )
{
    if (sidePow > m_CurLevel) return false;

    assert( sidePow <= m_MaxLevel && sidePow > 0 ); 

    QuadLayoutLevel* curLevel = &m_Level[sidePow];
    //  no free place - split parent greater free chunks
    if (m_Level[sidePow].numQuads == 0) 
    {
        WORD idx = sidePow;
        while (m_Level[idx].numQuads == 0) 
        {
            idx++;
            if (idx >= c_MaxQuadLevels) return false;
        }

        //  free last chunk
        QuadLayoutLevel* parentLevel = &m_Level[idx];
        parentLevel->numQuads--;
        WORD x = parentLevel->quad[parentLevel->numQuads].x;
        WORD y = parentLevel->quad[parentLevel->numQuads].y;
        //  now do descending splitting of the last free chunk on levels
        for (int i = idx - 1; i >= sidePow; i--)
        {
            curLevel = &m_Level[i];
            curLevel->numQuads = 3;

            curLevel->quad[0].x = x;
            curLevel->quad[1].x = x;
            x += c_PowSidesWORD[i];

            curLevel->quad[2].x = x;

            curLevel->quad[0].y = y;
            curLevel->quad[2].y = y;
            y += c_PowSidesWORD[i];

            curLevel->quad[1].y = y;
        }

        //  now do allocate forth chunk
        allocx = x; 
        allocy = y;
    }
    else
        //  there IS free chunk on the level - so allocate it
    {
        curLevel->numQuads--;
        allocx = curLevel->quad[curLevel->numQuads].x; 
        allocy = curLevel->quad[curLevel->numQuads].y;
    }

    m_NFreePixels -= (int)c_PowSidesWORD[sidePow] * (int)c_PowSidesWORD[sidePow];
    return true;
} // QuadLayout::AllocChunk


#endif // __KQUADLAYOUT_H__