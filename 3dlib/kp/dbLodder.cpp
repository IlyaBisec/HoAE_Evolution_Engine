/*****************************************************************
/*  File:   dbLodder.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbLodder.h"
#include "mLodder.h"



/*****************************************************************/
/*	VIPMDebugger implementation
/*****************************************************************/
VIPMDebugger::VIPMDebugger()
{
    m_NIterations    = 0;
    m_FileName       = "Models\\hydra.c2m";
    m_bDrawDebugInfo = true;
} // VIPMDebugger::VIPMDebugger

bool VIPMDebugger::OnMouseLBUp( int mX, int mY )
{
    return false;
}

bool VIPMDebugger::OnMouseLBDown( int mX, int mY )
{
	return false;
} // VIPMDebugger::OnMouseLBDown


bool VIPMDebugger::OnMouseMove( int mX, int mY, DWORD keys )
{
	return false;
} // VIPMDebugger::OnMouseMove

bool VIPMDebugger::OnMouseRBUp( int mX, int mY )
{
	return false;
} // VIPMDebugger::OnMouseRBUp

void VIPMDebugger::Expose( PropertyMap& pm )
{
	pm.start( "VIPMDebugger", this );
    pm.f( "DrawInfo", m_bDrawDebugInfo );
    pm.p( "FileName", GetFileName, SetFileName, "file|Models" );
    pm.f( "NIterations", m_NIterations );
} // VIPMDebugger::Expose

void VIPMDebugger::Render()
{
    static Geometry*    pGeom = NULL;
    static VIPMLodder   lodder;
    static BaseMesh     edgeTear;
    static BaseMesh     vipm;
    static int          mID = -1;
    static int          tID = -1;
    int mid = IMM->GetModelID( m_FileName.c_str() );
    if (mid != mID)
    {
        mID = mid;
        SNode* pRoot = NodePool::GetNode( mID );
        if (!pRoot) return;
        pGeom = pRoot->FindChild<Geometry>();
        BaseMesh& pri = pGeom->GetMesh();

        Texture* pTex = pRoot->FindChild<Texture>();
        if (pTex) tID = IRS->GetTextureID( pTex->GetName() );

        lodder.Reset();

        int nV = pri.getNVert();
        int nF = pri.getNPri();
        lodder.ReserveFaces( nF );
        lodder.ReserveVerts( nV );
        VertexIterator vit; vit << pri;
        for (int i = 0; i < nV; i++) lodder.AddVertex( vit.pos( i ) );
        WORD* idx = pri.getIndices();
        for (int i = 0; i < nF; i++) lodder.AddFace( idx[i*3], idx[i*3 + 1], idx[i*3 + 2] );

        FILE* fp = fopen( "c:\\dumps\\vipm.txt", "wt" );
        fprintf( fp, "\n\n%s\n", pGeom->GetName() );
        fclose( fp );
        lodder.Process();
        
        vipm.create( lodder.GetNVerts(), lodder.GetNFaces()*3, vfVertexN );
        VertexIterator mit;
        mit << vipm;
        for (int i = 0; i < nV; i++)
        {
            mit.pos( i ) = lodder.GetVertex( i );
            int vidx = lodder.GetOldVIndex( i );
            if (vidx == -1) continue;
            mit.normal( i ) = vit.normal( vidx );
            mit.diffuse( i ) = vit.diffuse( vidx );

            mit.uv( i ) = vit.uv( vidx );
        }
        WORD* vidx = vipm.getIndices();
        for (int i = 0; i < nF; i++)
        {
            int v0, v1, v2;
            lodder.GetFace( i, v0, v1, v2 );
            vidx[i*3    ] = v0;
            vidx[i*3 + 1] = v1;
            vidx[i*3 + 2] = v2;
        }
        vipm.setNVert( nV );
        vipm.setNInd( pri.getNInd() );
        vipm.setNPri( pri.getNPri() );
        
        CreateEdgeTearMesh( pri, edgeTear );
    }

    BaseMesh& bm = pGeom->GetMesh();
    static int shWire = IRS->GetShaderID( "wire" );
    static int shModl = IRS->GetShaderID( "smooth" );
    static int shLine = IRS->GetShaderID( "lines3D_blend" );
    
    ICamera* ICam = GetCamera();
    ICam->ShiftZ( 0.05f );
    ICam->Render();
    IRS->ResetWorldTM();

    if (m_NIterations > lodder.GetMaxCollapses()) m_NIterations = lodder.GetMaxCollapses();
    if (m_NIterations < 0) m_NIterations = 0;
    lodder.SetNCollapses( m_NIterations );
    int nFaces = lodder.GetNFaces();
    int nVert  = lodder.GetNVerts();
    WORD* idx  = vipm.getIndices();
    
    for (int i = 0; i < nFaces; i++)
    {
        int v0, v1, v2;
        lodder.GetFace( i, v0, v1, v2 );
        idx[i*3 + 0] = v0;
        idx[i*3 + 1] = v1;
        idx[i*3 + 2] = v2;
    }
    
    if (m_bDrawDebugInfo) 
    {
        VertexIterator bit; bit << vipm;
        for (int i = 0; i < nVert; i++)
        {
            int vIdx = vipm.getNVert() - i - 1;
            int colTo = lodder.GetCollapseTo( vIdx );
            if (colTo == -1) break;
            for (int j = 0; j < lodder.GetNFixFaces( vIdx ); j++)
            {
                int v0, v1, v2;
                int id = lodder.GetFixFace( vIdx, j, v0, v1, v2 );
                if (i == m_NIterations - 1)
                {
                    Vector3D& vv0 = bit.pos( v0 );
                    Vector3D& vv1 = bit.pos( v1 );
                    Vector3D& vv2 = bit.pos( v2 );
                    rsLine( vv0, vv1, 0xFFFFFFFF ); 
                    rsLine( vv1, vv2, 0xFFFFFFFF );
                    rsLine( vv2, vv0, 0xFFFFFFFF );
                } 
            }
            DrawPoint( bit.pos( vIdx ), 0xFF00FF00 );
           // DrawText( bit.pos( vIdx ), 0xFFFFAAAA, "%.4f", lodder.GetCollapseError( vIdx ) );
        }
    }
    
    vipm.setNPri ( nFaces                );
    vipm.setNVert( lodder.GetNVerts()    );
    vipm.setNInd ( nFaces*3              );

    DrawText( 10, 100, 0xFFFFFFFF, "Faces: %d", nFaces );
    DrawText( 10, 115, 0xFFFFFFFF, "Verts: %d", vipm.getNVert() );
    FlushText();

    IRS->SetShader( shModl );
    IRS->SetTexture( tID, 0 );
    DrawPrimBM( vipm );

    if (m_bDrawDebugInfo) 
    {
        IRS->SetShader( shWire );
        DrawPrimBM( vipm );

        IRS->SetShader( shLine );
        //DrawFaces( vipm, true );
        //DrawVertices( vipm, true, true );
        DrawPrimBM( edgeTear );
    }

    rsFlushLines3D();
    rsFlushLines2D();
    FlushText();
    ICam->ShiftZ( -0.05f );


} // VIPMDebugger::Render




/*****************************************************************/
/*	LODDebugger implementation
/*****************************************************************/
LODDebugger::LODDebugger()
{
} // LODDebugger::LODDebugger

bool LODDebugger::OnMouseLBUp( int mX, int mY )
{
    return false;
}

bool LODDebugger::OnMouseLBDown( int mX, int mY )
{
	return false;
} // LODDebugger::OnMouseLBDown


bool LODDebugger::OnMouseMove( int mX, int mY, DWORD keys )
{
	return false;
} // LODDebugger::OnMouseMove

bool LODDebugger::OnMouseRBUp( int mX, int mY )
{
	return false;
} // LODDebugger::OnMouseRBUp

void LODDebugger::Expose( PropertyMap& pm )
{
	pm.start( "LODDebugger", this );
} // LODDebugger::Expose

void LODDebugger::Render()
{
} // LODDebugger::Render


