/*****************************************************************
/*  File:   vSkinCache.h                                      
/*  Desc:   Skin cache implementation
/*    Author: Ruslan Shestopalyuk
/*  Date:   Sep 2004
/*****************************************************************/
#include "stdafx.h"
#include "IMediaManager.h"
#include "ISkinCache.h"
#include "ICamera.h"
#include "kQuadLayout.h"
#include "kHash.hpp"
#include "kBlockCache.hpp"

const int c_NumSkinCacheBlocks       = 128;      //  number of skin cache blocks
const int c_SkinCacheBlockSize       = 81920;    //  size of the skin cache block
const int c_RenderQSize         = 4096;
const int c_MaxAllocatedSkins   = 2048;
/*****************************************************************************/
/*  Class:  SkinInstance
/*  Desc:   Instance of the cached skin
/*****************************************************************************/
class SkinInstance
{
protected:
    DWORD       m_ModelID;      //  Skin model
    DWORD       m_AnimID;       //  Skin animation
    int         m_AnimTime;     //  quantized animation time
    
    DWORD       m_LastUsed;     //  last frame when skin instance was rendered
    
    int         m_BlockID;      //  id of the skin cache block
    int         m_FirstVert;    //  index of the first vertex in the vertex buffer   
    int         m_NVert;        //  number of cached vertices

    friend class SkinCache;
    friend class SkinCacheBlock;

public:
                    SkinInstance() 
                        :   m_ModelID   ( 0xFFFFFFFF),
                            m_AnimID    ( 0xFFFFFFFF),
                            m_AnimTime  ( 0         ),
                            m_BlockID   ( -1        ),
                            m_FirstVert ( 0         ),
                            m_NVert     ( 0         ),
                            m_LastUsed  ( 0         ){}
                    SkinInstance( DWORD modelID, DWORD animID, int animTime )
                        :   m_ModelID   ( modelID   ),
                            m_AnimID    ( animID    ),
                            m_AnimTime  ( animTime  ),
                            m_BlockID   ( -1        ),
                            m_FirstVert ( 0         ),
                            m_NVert     ( 0         ),
                            m_LastUsed  ( 0         ){}

                    SkinInstance( DWORD modelID )
                         :  m_ModelID   ( modelID   ),
                            m_AnimID    ( 0         ),
                            m_AnimTime  ( 0         ),
                            m_BlockID   ( -1        ),
                            m_FirstVert ( 0         ),
                            m_NVert     ( 0         ),
                            m_LastUsed  ( 0         ){}

    unsigned int hash() const
    {
        DWORD h = 2741*m_ModelID + 173*m_AnimID + 1987*m_AnimTime + 29;
        h = (h << 13) ^ h;
        h += 15731;
        return h;
    }

    bool equal( const SkinInstance& el )
    {
        return  (m_ModelID  == el.m_ModelID     ) &&
                (m_AnimID   == el.m_AnimID      ) &&
                (m_AnimTime == el.m_AnimTime    );
    }
    
    void copy( const SkinInstance& el )
    {
        m_ModelID   = el.m_ModelID; 
        m_AnimID    = el.m_AnimID;  
        m_AnimTime  = el.m_AnimTime;
    }

}; // class SkinInstance


/*****************************************************************************/
/*  Class:  SkinRenderTask
/*  Desc:   Element of the Skin render queue
/*****************************************************************************/
struct SkinRenderTask
{
    DWORD       m_ModelID;      //  skined model id
    DWORD       m_AnimID;       //  model's animation id
    float       m_AnmTime;      //  animation time
    Matrix4D    m_TM;           //  models world transform
    
    int         m_BlockID;
    int         m_ShaderID;     
    int         m_FirstVert;    //  index of the first vertex in the vertex buffer   
    int         m_NVert;        //  number of cached vertices
}; // class SkinRenderTask

/*****************************************************************************/
/*  Class:  SkinCacheBlock
/*  Desc:   Single surface of the Skin cache
/*****************************************************************************/
class SkinCacheBlock
{
protected:
    int                     m_vbID;     //  id of the vertex buffer
    
    //  allocated skin instances
    SkinInstance*           m_Allocated[c_MaxAllocatedSkins];
    int                     m_NAllocated;

    int                     m_FirstVert;
    int                     m_NAllocVert;
    int                     m_NVert;
    
    friend class            SkinCache;

public:
    SkinCacheBlock() 
    {
        m_NAllocated    = 0;
        m_FirstVert     = 0;
        m_NAllocVert    = 0;
        m_NVert         = 0;
    }

    bool Allocate( SkinInstance& imp )
    {
        if (m_NAllocated == c_MaxAllocatedSkins) return false;
        if (m_NAllocVert + imp.m_NVert > m_NVert) return false;
        imp.m_FirstVert = m_FirstVert + m_NAllocVert;
        m_NAllocVert += imp.m_NVert;
        m_Allocated[m_NAllocated++] = &imp;
        return true;
    } // SkinCacheBlock::Allocate

    void SkinCacheBlock::Free()
    {
        for (int i = 0; i < m_NAllocated; i++) 
        {
            m_Allocated[i]->m_BlockID = -1;
        }
        m_NAllocated    = 0;
        m_NAllocVert    = 0;
    } // SkinCacheBlock::Free

}; // class SkinCacheBlock

typedef Hash<SkinInstance> SkinHash;
typedef BlockCache<SkinCacheBlock, SkinInstance, c_NumSkinCacheBlocks>  SkinBlockCache;
/*****************************************************************************/
/*  Class:  SkinCache
/*  Desc:   Skin cache implementation
/*****************************************************************************/
class SkinCache : public ISkinCache
{
    SkinHash                m_Hash;                     //  storage of Skin instance desriptors
    bool                    m_bInited;                  //  true when cache is initialized
    SkinBlockCache          m_Cache;                    //  Skin render targets cache
    SkinRenderTask          m_RenderQ[c_RenderQSize];   //  render queue
    SkinRenderTask*         m_SortedRenderQ[c_RenderQSize];
    int                     m_RenderQSize;              //  current size of the Skin render queue

    float                   m_TimeQuant;                //  animation time quantising ratio
    int                     m_NCreatedSkins;            //  number of crated Skins on this frame
    
    int                     m_vbID;                     //  static vertex buffer ID

public:
                        SkinCache       ();
                        ~SkinCache      ();
    virtual void        Flush           ();
    virtual void        DrawDebugInfo   ();
    virtual bool        Init            ();
    virtual void        Draw            ( DWORD modelID, const Matrix4D& tm );
    virtual void        Draw            ( DWORD modelID, DWORD animID, float anmTime, const Matrix4D& tm );
    virtual void        DumpSurfaces    ();
    
    static int CmpRenderTasks( const void *e1, const void *e2 )
    {
        const SkinRenderTask* pTask1 = *((const SkinRenderTask**)e1);
        const SkinRenderTask* pTask2 = *((const SkinRenderTask**)e2);
        return pTask1->m_ShaderID - pTask2->m_ShaderID;
    }

protected:
    bool                CreateSkin  ( SkinInstance& imp );

}; // class SkinCache

ISkinCache* ISkCache;
SkinCache   g_SkinCache;
/*****************************************************************************/
/*  SkinCache implementation
/*****************************************************************************/
SkinCache::SkinCache()
{
    ISkCache            = this;
    m_bInited           = false;
    m_RenderQSize       = 0;
    m_TimeQuant         = 1000.0f/16.0f;
    m_NCreatedSkins     = 0;
    m_vbID              = -1;
} // SkinCache::SkinCache

SkinCache::~SkinCache()
{
}

const float c_BorderW = 8.0f;
void SkinCache::DrawDebugInfo()
{
} // SkinCache::DrawDebugInfo

void SkinCache::Flush()
{
    if (m_RenderQSize == 0) return;

    //  precache animated models' Skins
    int cTask = 0;
    m_NCreatedSkins = 0;
    for (int i = 0; i < m_RenderQSize; i++)
    {
        SkinRenderTask& rt = m_RenderQ[cTask];
        //  calculate skin attributes
        int nTime = rt.m_AnmTime/m_TimeQuant;        
        //  look for an skin instance in the cache
        int hIdx = m_Hash.add( SkinInstance( rt.m_ModelID, rt.m_AnimID, nTime ) );
        SkinInstance& hImp = m_Hash.elem( hIdx );        
        if (hImp.m_BlockID == -1) 
        {
            if (!CreateSkin( hImp )) continue;
            m_NCreatedSkins++;
        }
        else
        {
            m_Cache.HitBlock( hImp.m_BlockID );
        }
        m_SortedRenderQ[cTask] = &rt;
        cTask++;
    }
    m_RenderQSize = cTask;
   
    //  sort skins by texture
    qsort( m_SortedRenderQ, m_RenderQSize, sizeof( SkinRenderTask* ), CmpRenderTasks );
    
    for (int i = 0; i < m_RenderQSize; i++)
    {
        const SkinRenderTask* pRT = m_SortedRenderQ[cTask];
        IMM->StartModel( pRT->m_ModelID, pRT->m_TM );
        IMM->AnimateModel( pRT->m_AnimID, pRT->m_AnmTime );		
        IMM->DrawModel();
    }

    //  clear render queue
    m_RenderQSize = 0;
} // SkinCache::Flush

bool SkinCache::CreateSkin( SkinInstance& imp )
{
    //  set number of vertices in the model
    imp.m_NVert = 0;
    //  allocate place in the vertex cache
    int blockID = m_Cache.Allocate( &imp );
    if (blockID == -1) return false;
    imp.m_BlockID = blockID;
    
    //  perform skinning and copy skinned vertices to the static vertex buffer
    return true;
} // SkinCache::CreateSkin

void SkinCache::Draw( DWORD modelID, const Matrix4D& tm )
{
    if (m_RenderQSize == c_RenderQSize) Flush();
    SkinRenderTask& rt = m_RenderQ[m_RenderQSize++];
    rt.m_ModelID  = modelID;
    rt.m_AnimID   = 0xFFFFFFFF;
    rt.m_AnmTime  = 0.0f;
    rt.m_TM       = tm;
} // SkinCache::Draw

void SkinCache::Draw( DWORD modelID, DWORD animID, float anmTime, const Matrix4D& tm )
{
    if (m_RenderQSize == c_RenderQSize) Flush();
    SkinRenderTask& rt = m_RenderQ[m_RenderQSize++];
    rt.m_ModelID  = modelID;
    rt.m_AnimID   = animID;
    rt.m_AnmTime  = anmTime;
    rt.m_TM       = tm;
    rt.m_BlockID  = -1;
} // SkinCache::Draw 

bool SkinCache::Init()
{
    if (m_bInited) return true;
    
    //  create static vertex buffer
    m_vbID = IRS->CreateVB( "SkinCache", c_NumSkinCacheBlocks*c_SkinCacheBlockSize, false );
    for (int i = 0; i < c_NumSkinCacheBlocks; i++)
    {
        SkinCacheBlock& block   = m_Cache.GetBlock( i ); 
        block.m_FirstVert       = i*c_SkinCacheBlockSize;
        block.m_NVert           = c_SkinCacheBlockSize;
        block.m_NAllocated      = 0;
        block.m_vbID            = m_vbID;
    }
    m_bInited = true;
    return true;
} // SkinCache::Init

void SkinCache::DumpSurfaces()
{
} // SkinCache::DumpSurfaces



