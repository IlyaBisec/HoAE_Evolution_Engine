/*****************************************************************************/
/*    File:    vTreesRenderer.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "IMediaManager.h"
#include "vTerrainRenderer.h"
#include "vTreesRenderer.h"

TreesRenderer g_TreesRenderer;
TreesRenderer* ITrees = &g_TreesRenderer;

/*****************************************************************************/
/*  TreesRenderer implementation
/*****************************************************************************/
TreesRenderer::TreesRenderer()
{
    m_NTreesRendered = 0;
}

void TreesRenderer::RenderBlock( const Rct& ext, int LOD )
{
    TreesBlock::Key key( ext, LOD );
    int blockID = m_BlockHash.find( key );
    if (blockID == NO_ELEMENT)
    {
        blockID = m_BlockHash.add( key );
        TreesBlock* pBlock = &m_BlockHash.elem( blockID );
        pBlock->m_Key = key;
        CreateTreesBlock( *pBlock );
    }
    TreesBlock& block = m_BlockHash.elem( blockID );
    
    //  check whether trees block geometry buffers are still valid
    int nBits = block.m_RenderBits.size();
    bool bValid = true;
    for (int i = 0; i < nBits; i++)
    {
        if (!IRS->IsIBStampValid( block.m_RenderBits[i].m_IBufID, block.m_RenderBits[i].m_IBufStamp )) bValid = false;
        if (!IRS->IsVBStampValid( block.m_RenderBits[i].m_VBufID, block.m_RenderBits[i].m_VBufStamp )) bValid = false;
    }
    if (!bValid) 
    {
        CreateTreesBlock( block );
    }
    //  submit block to the renderer
    for (int i = 0; i < nBits; i++)
    {
        IRS->AddTask() = block.m_RenderBits[i];
    } 
    m_NTreesRendered += block.m_NObj;
}// TreesRenderer::RenderBlock

const int c_TreesVBufferBytes  = 1024*1024*16;
const int c_TreesIBufferBytes  = 1024*1024*4;

struct TreeRenderBit
{
    const TreeObject*       m_pObj;
    DWORD                   m_MeshID;
    RenderTask              m_RB;

    TreeRenderBit() : m_pObj(0), m_MeshID(0xFFFFFFFF) {}
    TreeRenderBit( const TreeObject* pObj ) : m_pObj(pObj), m_MeshID(0xFFFFFFFF) {}
}; // struct TreeRenderBit

struct BitCompare
{
    bool operator ()( const TreeRenderBit* l, const TreeRenderBit* r ) { return (l->m_RB < r->m_RB); }
}; // struct BitCompare

bool TreesRenderer::BakeTreesGeometry( TreesBlock& block, const TreeObject* trees, int nTrees )
{
    //static int vbID = -1;
    //static int ibID = -1;

    //if (vbID == -1) vbID = IRS->CreateVB( "Trees", c_TreesVBufferBytes, -1, false );
    //if (ibID == -1) ibID = IRS->CreateIB( "Trees", c_TreesIBufferBytes, isWORD, false );

    //static static_array<TreeRenderBit, c_MaxTreesInArea*4> s_Bits;
    //static static_array<TreeRenderBit*, c_MaxTreesInArea*4> s_pBits;
    //s_Bits.clear();
    //s_pBits.clear();

    ////  scan through all trees in area, split them into render bits
    //for (int i = 0; i < nTrees; i++)
    //{
    //    const TreeObject& obj = trees[i];
    //    ModelObject* pModel = IModelMgr->GetModel( obj.m_ModelID );
    //    if (!pModel) continue;
    //    int nSub = pModel->GetNMeshes();
    //    for (int j = 0; j < nSub; j++)
    //    {
    //        IMesh*          pMesh   = pModel->GetMesh( j );
    //        TreeRenderBit&  trb     = s_Bits.push_back( TreeRenderBit( &obj, pMesh ) );
    //        RenderTask&      rb      = trb.m_RB;
    //        pMesh->GetRenderAttributes( rb );
    //    }
    //}
    ////  sort render bits to enable batching
    //int nBits = s_Bits.size();
    //s_pBits.resize( nBits );
    //for (int i = 0; i < nBits; i++) s_pBits[i] = &s_Bits[i];
    //std::sort( s_pBits.begin(), s_pBits.end(), BitCompare() );

    ////  bake trees geometry into static vertex/index buffers
    //int nVert   = 0;
    //int nInd    = 0;
    //int lastBit = 0;
    //RenderTask* pRB = NULL;
    //for (int i = 0; i <= nBits; i++)
    //{
    //    const RenderTask& crb = (i == nBits) ? RenderTask::c_Invalid : s_pBits[i]->m_RB;
    //    const RenderTask& prb = (i == 0)     ? RenderTask::c_Invalid : s_pBits[i - 1]->m_RB;
    //    if (!(crb == prb))
    //    //  need to create new baked render bit
    //    {
    //        if (pRB)
    //        {
    //            const VertexDeclaration& vDecl = s_pBits[lastBit]->m_pMesh->GetVDecl();
    //            int vStride = vDecl.m_VertexSize;

    //            pRB->m_NVert = nVert;
    //            pRB->m_NIdx  = nInd;
    //            pRB->m_VType = vDecl.m_TypeID;
    //            IRS->SetVB( vbID, vDecl.m_TypeID );

    //            static std::vector<WORD> pITmp;
    //            static std::vector<BYTE> pVTmp;

    //            int   nVBytes   = vStride*nVert;
    //            int   nIBytes   = 2*nInd;

    //            if (nInd > pITmp.size())    pITmp.resize( nInd );
    //            if (nVBytes > pVTmp.size()) pVTmp.resize( nVBytes );

    //            WORD* pCurI     = &pITmp[0];
    //            BYTE* pCurV     = &pVTmp[0];

    //            if (nVBytes > c_MaxTreesInArea*1024*32 || nInd > c_MaxTreesInArea*1024)
    //            {
    //                Log.Error( "Too much static objects when baking the area!" );
    //                return false;
    //            }
    //            //  bake vertex and index data
    //            int cV = 0;
    //            for (int j = lastBit; j < i; j++)
    //            {
    //                TreeRenderBit& tb = *s_pBits[j];
    //                //  caclulate render bit world transform
    //                Matrix4D tm = tb.m_pObj->m_TM;
    //                int hostBone = tb.m_pMesh->GetHostBone();
    //                if (hostBone >= 0)
    //                {
    //                    tm *= tb.m_pMesh->GetModel()->m_Skeleton[hostBone].m_WorldTM;
    //                }
    //                //  copy indices
    //                int cNIdx  = tb.m_RB.m_NIdx;
    //                int cNVert = tb.m_RB.m_NVert;
    //                tb.m_pMesh->InstanceIndices( (BYTE*)pCurI, NULL );
    //                for (int k = 0; k < cNIdx; k++) pCurI[k] += cV;
    //                tb.m_pMesh->InstanceVertices( pCurV, NULL );
    //                VertexIterator vit;
    //                vit.reset( pCurV, cNVert, vDecl );
    //                while (vit)
    //                {
    //                    tm.transformPt( vit.pos() );
    //                    tm.transformVec( vit.normal() );
    //                    ++vit;
    //                }
    //                pCurV += cNVert*vStride;
    //                pCurI += cNIdx;
    //                cV    += cNVert;
    //            }

    //            WORD* pIdx = (WORD*)IRS->LockAppendIB( ibID, nInd, pRB->m_FirstIdx, pRB->m_IBufStamp );
    //            if (!pIdx) return false;
    //            memcpy( pIdx, &pITmp[0], nInd*2 );
    //            IRS->UnlockIB( ibID );

    //            BYTE* pVert = IRS->LockAppendVB( vbID, nVert, pRB->m_FirstVert, pRB->m_VBufStamp );
    //            if (!pVert) return false;
    //            memcpy( pVert, &pVTmp[0], nVBytes );
    //            IRS->UnlockVB( vbID );

    //            if (i == nBits) break;
    //        }
    //        block.m_RenderBits.push_back( crb );
    //        pRB             = &block.m_RenderBits.back();
    //        pRB->m_VBufID   = vbID;
    //        pRB->m_IBufID   = ibID;
    //        pRB->m_bHasTM   = true;
    //        float cx = block.m_Key.m_Ext.GetCenterX();
    //        float cy = block.m_Key.m_Ext.GetCenterY();
    //        float cz = ITerra->GetH( cx, cy );
    //        pRB->m_TM.translation( cx, cy, cz );

    //        nVert   = crb.m_NVert;
    //        nInd    = crb.m_NIdx;
    //        lastBit = i;
    //    }
    //    else
    //    {
    //        nVert += crb.m_NVert;
    //        nInd  += crb.m_NIdx;
    //    }
    //}
    return true;
} // TreesRenderer::BakeTreesGeometry

bool TreesRenderer::BakeTreesBillboards( TreesBlock& block, const TreeObject* trees, int nTrees )
{
    static int vbID = -1;
    static int ibID = -1;

    if (vbID == -1) vbID = IRS->CreateVB( "Trees", c_TreesVBufferBytes, -1, false );
    if (ibID == -1) ibID = IRS->CreateIB( "Trees", c_TreesIBufferBytes, isWORD, false );

    block.m_RenderBits.push_back( RenderTask() );
    RenderTask& rb = block.m_RenderBits.back();
    
    VertexDeclaration vDecl = CreateVertexDeclaration( vfVertex2t );
    rb.m_Pass           = 0;         
    rb.m_ShaderID       = IRS->GetShaderID( "trees_impostor_cluster" );     
    rb.m_TexID[0]       = IRS->GetTextureID( "fir.tga" );

    rb.m_VBufID         = vbID;       
    rb.m_VType          = IRS->RegisterVType( vDecl );        

    rb.m_IBufID         = ibID;  
    rb.m_IBufStamp      = 1;
    rb.m_PriType        = ptQuadList;      
    rb.m_bHasTM         = false;       
    rb.m_bTransparent   = true; 
    rb.m_Source         = "ImpostorCluster";    
    rb.m_NIdx           = 0;

    int vStride         = 32;
    rb.m_NVert          = nTrees*4;

    IRS->SetVB( rb.m_VBufID, rb.m_VType );
    BYTE* pVert         = IRS->LockAppendVB( rb.m_VBufID, rb.m_NVert, rb.m_FirstVert, rb.m_VBufStamp );
    if (!pVert) return false; 
    Vertex2t* v = (Vertex2t*)pVert;
    for (int i = 0; i < nTrees; i++)
    {
        const TreeObject& obj = trees[i];
        Vector3D pos( obj.m_TM.getTranslation() );

        /*
        ModelObject* pModel = IModelMgr->GetModel( obj.m_ModelID );
        if (!pModel) continue;
        AABoundBox aabb = pModel->GetAABB();
        aabb.Transform( obj.m_TM );
        float hh = aabb.GetDY()*2.0f;
        float hw = hh*0.5f;
        
        v[0].x  = pos.x;
        v[0].y  = pos.y;
        v[0].z  = pos.z;
        v[0].diffuse = 0xFFFFFF00;
        v[0].u  = 0.0f;
        v[0].v  = 0.0f;
        v[0].u2 = -hw;
        v[0].v2 = hh;

        v[1].x  = pos.x;
        v[1].y  = pos.y;
        v[1].z  = pos.z;
        v[1].diffuse = 0xFFFFFF00;
        v[1].u  = 1.0f;
        v[1].v  = 0.0f;
        v[1].u2 = hw;
        v[1].v2 = hh;

        v[2].x  = pos.x;
        v[2].y  = pos.y;
        v[2].z  = pos.z;
        v[2].diffuse = 0xFFFFFF00;
        v[2].u  = 0.0f;
        v[2].v  = 1.0f;
        v[2].u2 = -hw;
        v[2].v2 = 0.0f;

        v[3].x  = pos.x;
        v[3].y  = pos.y;
        v[3].z  = pos.z;
        v[3].diffuse = 0xFFFFFF00;
        v[3].u  = 1.0f;
        v[3].v  = 1.0f;
        v[3].u2 = hw;
        v[3].v2 = 0.0f;
        */

        v += 4;
    }
    IRS->UnlockVB( rb.m_VBufID );

    rb.m_NIdx           = nTrees*6;
    rb.m_PriType        = ptTriangleList;
    WORD* pIdx = (WORD*)IRS->LockAppendIB( ibID, rb.m_NIdx*2, rb.m_FirstIdx, rb.m_IBufStamp );
    if (!pIdx) return false; 
    int cV = 0;
    for (int i = 0; i < nTrees; i++)
    {
        pIdx[i*6 + 0] = cV;
        pIdx[i*6 + 1] = cV + 1;
        pIdx[i*6 + 2] = cV + 2;
        pIdx[i*6 + 3] = cV + 2;
        pIdx[i*6 + 4] = cV + 1;
        pIdx[i*6 + 5] = cV + 3;
        cV += 4;
    }
    IRS->UnlockIB( ibID );

    return true;
} // TreesRenderer::BakeTreesBillboards

bool TreesRenderer::CreateTreesBlock( TreesBlock& block )
{
    static TreesArray s_TreeList;
    if (!ITreesDB) return false;
    ITreesDB->GetTreesInArea( block.m_Key.m_Ext, s_TreeList );
    block.m_RenderBits.clear();
    block.m_NObj = s_TreeList.size();
    if (s_TreeList.size() == 0) return true;

    if (block.m_Key.m_LOD <= 3) return BakeTreesGeometry  ( block, &s_TreeList[0], s_TreeList.size() );
    if (block.m_Key.m_LOD <= 5) return BakeTreesBillboards( block, &s_TreeList[0], s_TreeList.size() );
    return true;
} // TreesRenderer::CreateTreesBlock

/*****************************************************************************/
/*  EcotopeTreesDB implementation
/*****************************************************************************/
EcotopeTreesDB      g_Ecotope;
ITreesDataBase*     ITreesDB = &g_Ecotope;

EcotopeTreesDB::EcotopeTreesDB()
{

}

bool EcotopeTreesDB::GetTreesInArea( const Rct& area, TreesArray& trees, int LOD )
{
    static int tID = IMM->GetModelID( "fir" );
    trees.clear();
    Matrix4D tm;
    
    float tx = area.x;
    float ty = area.y;
    
    float step = 256.0f;
    float dev  = 128.0f; 
   
    while (tx < area.GetRight())
    {
        while (ty < area.GetBottom())
        {
            if (trees.size() == trees.capacity()) break;
            bool bPresent = true;
            if (PerlinNoise( tx*0.0005f, ty*0.0005f ) < 0.1f) bPresent = false;
            if (bPresent)
            {
                rndInit( tx*ty );
                tx += rndValuef( -dev, dev );
                ty += rndValuef( -dev, dev );
                
                float s   = rndValuef( 0.3f, 1.1f );
                float ang = rndValuef( 0.0f, c_DoublePI );
                Vector3D axis;
                axis.x = rndValuef( -0.05f, 0.05f );
                axis.y = rndValuef( -0.05f, 0.05f );
                axis.z = sqrtf( 1.0f - axis.x*axis.x + axis.y*axis.y );
                float h = ITerra->GetH( tx, ty ) + rndValuef( -100.0f, 0.0f );
                tm.srt( s, axis, ang, Vector3D( tx, ty, h ) );
                trees.push_back( TreeObject( tID, tm ) );
            }
            ty += step;
        }
        ty = area.y;
        tx += step;
    }
    
    return true;
} // EcotopeTreesDB::GetTreesInArea

void EcotopeTreesDB::Expose( PropertyMap& pm )
{
    pm.start( "EcotopeTreesDB", this );
} // EcotopeTreesDB::Expose


