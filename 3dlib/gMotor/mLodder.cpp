/************************************************************************************
/*  File:   mLodder.cpp                                         
/*    Desc:    Routines for building VIPM mesh data structure
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   June 2004                                          
/************************************************************************************/
#include "stdafx.h"
#include "mVector.h"
#include "mTransform.h"
#include "mPlane.h"
#include "mLodder.h"
#include <queue>
#include "float.h"

/************************************************************************************/
/*    VIPMLodder implementation
/************************************************************************************/
void VIPMLodder::Reset()
{ 
    m_Faces.clear(); 
    m_Verts.clear();
    m_FixFaces.clear();
    m_SortedVerts.clear();
    m_SortedFaces.clear();
    m_NumColVerts = 0;
    m_NumColFaces = 0;

} // VIPMLodder::Reset

bool VIPMLodder::AddVertex( const Vector3D& v, const Vector3D& n )
{
    LODVertex vert;
    vert.pos        = v;
    vert.normal        = n;
    int vertID        = m_Verts.size();
    vert.id            = vertID;
    vert.orderedID    = vertID;
    vert.bFrozen    = false;
    vert.bBorder    = false;
    vert.bCollapsed = false;
    vert.colError    = 0.0f;
    vert.colOrder    = c_NotCollapsed;
    m_Verts.push_back( vert );
    return true;
} // VIPMLodder::AddVertex

bool VIPMLodder::AddVertex( const Vector3D& v )
{
    AddVertex( v, Vector3D::oZ );
    return false; 
} // VIPMLodder::AddVertex

bool VIPMLodder::AddFace( int v0, int v1, int v2 )
{
    assert( v0 >= 0 && v0 < m_Verts.size() );
    assert( v1 >= 0 && v1 < m_Verts.size() );
    assert( v2 >= 0 && v2 < m_Verts.size() );

    LODVertex& lv0 = m_Verts[v0];
    LODVertex& lv1 = m_Verts[v1];
    LODVertex& lv2 = m_Verts[v2];

    LODFace face;
    int faceID    = m_Faces.size();
    face.id        = faceID;
    face.v[0]    = v0;
    face.v[1]    = v1;
    face.v[2]    = v2;
    
    lv0.nbFaceID.insert( faceID );
    lv1.nbFaceID.insert( faceID );
    lv2.nbFaceID.insert( faceID );

    lv0.nbVertID.insert( v1 ); lv0.nbVertID.insert( v2 );
    lv1.nbVertID.insert( v0 ); lv1.nbVertID.insert( v2 );
    lv2.nbVertID.insert( v0 ); lv2.nbVertID.insert( v1 );

    face.bCollapsed = false;
    face.colOrder    = c_NotCollapsed;
    face.fixFace    = -1;

    m_Faces.push_back( face );
    return false;
} // VIPMLodder::AddFace

int VIPMLodder::GetOldVIndex( int newIdx ) const
{
    if (newIdx < 0 || newIdx >= m_SortedVerts.size()) return -1;
    return m_SortedVerts[newIdx]->id;
} // VIPMLodder::GetOldVIndex

Quadric    VIPMLodder::CalculateQuadric( int vID )
{
    Quadric q;
    LODVertex& vert = m_Verts[vID];
    int nF = vert.nbFaceID.size();
    std::set<int>::const_iterator it = vert.nbFaceID.begin();
    for (int i = 0; i < nF; i++)
    {
        LODFace* pFace = &m_Faces[*it];
        while (pFace->fixFace >= 0) pFace = &m_FixFaces[pFace->fixFace];
        if (!pFace->bCollapsed)
        {
            LODVertex& lv0 = m_Verts[pFace->v[0]];
            LODVertex& lv1 = m_Verts[pFace->v[1]];
            LODVertex& lv2 = m_Verts[pFace->v[2]];
            Plane p;
            pFace->area        = p.from3Points( lv0.pos, lv1.pos, lv2.pos );
            pFace->normal    = p.normal();

            q.AddPlane( p.a, p.b, p.c, p.d );
        }
        ++it;
    }

    return q;
} // VIPMLodder::CalculateQuadric

float VIPMLodder::GetCollapseError( int vSrcID, int vDstID )
{
    LODVertex& sv = m_Verts[vSrcID];
    LODVertex& dv = m_Verts[vDstID];
    if (sv.bFrozen || dv.bCollapsed || sv.bCollapsed ||
        sv.bBorder != dv.bBorder) return FLT_MAX;
    Quadric sq( sv.quadric );
    float err = sq.GetError( dv.pos.x, dv.pos.y, dv.pos.z );
    return err;
} // VIPMLodder::GetCollapseError

void VIPMLodder::Process( int maxCollapses )
{
    m_NumColVerts    = 0;
    m_NumColFaces    = 0;
    int nV = m_Verts.size();
    //  set status flag to border vertices
    if (m_bCheckForBorder) CheckForBorder();
    //  for each vertex find to which vertex it should be collapsed
    for (int i = 0; i < nV; i++) FindCollapseTo( i );
    //  collapse vertices
    int vID = NextToCollapse();
    while (vID != -1 && (m_NumColVerts < maxCollapses || maxCollapses == -1))
    {
        if (!Collapse( vID )) break;
        vID = NextToCollapse();
    }

    //  sort vertices in collapse order
    m_SortedVerts.clear();
    for (int i = 0; i < nV; i++) m_SortedVerts.push_back( &m_Verts[i] );
    qsort( &m_SortedVerts[0], nV, sizeof(LODVertex*), LODVertex::Cmp );
    for (int i = 0; i < nV; i++) m_SortedVerts[i]->orderedID = i;
    //  reindex faces to the new vertex order
    int nF = m_Faces.size();
    for (int i = 0; i < nF; i++)
    {
        LODFace& face = m_Faces[i];
        LODFace* pFixFace = &face;
        while (pFixFace->fixFace != -1) pFixFace = &m_FixFaces[pFixFace->fixFace];
        face.colOrder = pFixFace->colOrder;
        face.v[0] = m_Verts[face.v[0]].orderedID;
        face.v[1] = m_Verts[face.v[1]].orderedID;
        face.v[2] = m_Verts[face.v[2]].orderedID;
    }
    
    //  sort faces in collapse order
    m_SortedFaces.clear();
    for (int i = 0; i < nF; i++) m_SortedFaces.push_back( &m_Faces[i] );
    qsort( &m_SortedFaces[0], nF, sizeof(LODFace*), LODFace::Cmp );
    for (int i = 0; i < nF; i++) m_SortedFaces[i]->orderedID = i;

    //  reindex fixfaces
    int nFixFaces = m_FixFaces.size();
    for (int i = 0; i < nFixFaces; i++)
    {
        LODFace& fixFace = m_FixFaces[i];
        fixFace.v[0] = m_Verts[fixFace.v[0]].orderedID;
        fixFace.v[1] = m_Verts[fixFace.v[1]].orderedID;
        fixFace.v[2] = m_Verts[fixFace.v[2]].orderedID;
        fixFace.id   = m_Faces[fixFace.id].orderedID;
    }
    
    SetNCollapses( 0 );
} // VIPMLodder::Process

bool VIPMLodder::Collapse( int vID )
{
    LODVertex& v    = m_Verts[vID];
    assert( !v.bCollapsed && v.colTo != v.id );
    v.colOrder        = m_NumColVerts;
    v.bCollapsed    = true;
    v.fixFaceIdx    = m_FixFaces.size();

    LODVertex& vTo    = m_Verts[v.colTo];

    //  fix adjacent faces
    int nF         = v.nbFaceID.size();
    int nRemoved = 0;
    int nFixed     = 0;
    std::set<int>::const_iterator it = v.nbFaceID.begin();
    for (; it != v.nbFaceID.end(); ++it)
    {
        int faceIdx = *it;
        vTo.nbFaceID.insert( faceIdx );
        LODFace* pFace = &m_Faces[faceIdx];
        while (pFace->fixFace >= 0) 
        {
            LODFace* pFixFace = &m_FixFaces[pFace->fixFace];
            pFace = pFixFace;
        }
        if (pFace->bCollapsed) continue;

        if (pFace->HasVertex( v.colTo ))
        //  face is collapsed
        {
            assert( !pFace->bCollapsed );
            pFace->colOrder = m_NumColFaces;
            pFace->bCollapsed = true;
            m_NumColFaces++;
            nRemoved++;
        }
        else
        //  face is fixed
        {
            LODFace fixFace( *pFace );
            if (pFace->v[0] == vID) fixFace.v[0] = v.colTo;
            if (pFace->v[1] == vID) fixFace.v[1] = v.colTo;
            if (pFace->v[2] == vID) fixFace.v[2] = v.colTo;

            int curFix = m_FixFaces.size();
            pFace->fixFace = curFix;
            m_FixFaces.push_back( fixFace );

            //  recalculate collapse to values for adjacent vertices
            FindCollapseTo( fixFace.v[0] );
            FindCollapseTo( fixFace.v[1] );
            FindCollapseTo( fixFace.v[2] );
            nFixed++;
        }
    }

    //  merge neighbor vertices list
    std::set<int>::const_iterator vit = v.nbVertID.begin();
    for (; vit != v.nbVertID.end(); ++vit)
    {
        int nvID = *vit;
        if (nvID != vTo.id) vTo.nbVertID.insert( nvID );
    }

    v.nFixedFaces    = nFixed;
    v.nRemovedFaces    = nRemoved;
    m_NumColVerts++;
    return true;
} // VIPMLodder::Collapse

int    VIPMLodder::NextToCollapse()
{
    int nV            = m_Verts.size();
    float minErr    = FLT_MAX;
    int colID        = -1;
    for (int i = 0; i < nV; i++)
    {
        LODVertex& v = m_Verts[i];
        if (v.bCollapsed || v.colTo == -1) continue;
        if (v.colError < minErr)
        {
            minErr = v.colError;
            colID  = i; 
        }
    }
    return colID;
} // VIPMLodder::NextToCollapse

void VIPMLodder::FindCollapseTo( int vID )
{
    LODVertex& v = m_Verts[vID];
    if (v.bCollapsed) return;
    int nV        = v.nbVertID.size();
    v.colError    = FLT_MAX;
    v.colTo        = -1;
    v.quadric   = CalculateQuadric( vID );

    //  find neighbor to which we could collapse with minimal error
    std::set<int>::const_iterator it = v.nbVertID.begin();
    for (int i = 0; i < nV; i++)
    {
        LODVertex& cv = m_Verts[*it];
        if (cv.bCollapsed) { ++it; continue; }
        float err = GetCollapseError( vID, *it );
        if (err < v.colError)
        {
            v.colError    = err;
            v.colTo        = *it;
        }
        ++it;
    }
} // VIPMLodder::FindCollapseTo

void VIPMLodder::CheckForBorder()
{
    int nV = m_Verts.size();
    for (int i = 0; i < nV; i++)
    {
        LODVertex& v = m_Verts[i];
        //  go through neighbor vertices and count incident faces for corresponding edges
        int nVN = v.nbVertID.size();
        std::set<int>::const_iterator vit = v.nbVertID.begin();
        for (int j = 0; j < nVN; j++)
        {
            LODVertex& vn = m_Verts[*vit];
            int nF = vn.nbFaceID.size();
            std::set<int>::const_iterator fit = vn.nbFaceID.begin();
            int nAdjFaces = 0;
            //  go through all neighbor vertex faces
            for (int k = 0; k < nF; k++)
            {
                LODFace& face = m_Faces[*fit];
                if (face.HasVertex( i )) nAdjFaces++;
                ++fit;
            }
            if (nAdjFaces < 2)
            {
                v.bBorder = true;
                v.bFrozen = true;
                break;
            }
            ++vit;
        }
    }
} // VIPMLodder::CheckForBorder

void VIPMLodder::GetFace( int idx, int& v0, int& v1, int& v2 ) const
{
    v0 = m_ColFaces[idx*3    ];
    v1 = m_ColFaces[idx*3 + 1];
    v2 = m_ColFaces[idx*3 + 2];
} // VIPMLodder::GetFace

int VIPMLodder::GetFixFace( int vIdx, int ffIdx, int& v0, int& v1, int& v2 ) const
{
    const LODVertex* v = m_SortedVerts[vIdx];
    const LODFace& fixFace = m_FixFaces[v->fixFaceIdx + ffIdx];
    v0 = fixFace.v[0];
    v1 = fixFace.v[1];
    v2 = fixFace.v[2];
    return fixFace.id;
} // VIPMLodder::GetFixFace

void VIPMLodder::SetNCollapses( int nCol )
{
    if (nCol < 0 || nCol > m_NumColVerts) return;
    int nF = m_Faces.size();
    m_ColFaces.clear();
    for (int i = 0; i < nF; i++)
    {
        m_ColFaces.push_back( m_SortedFaces[i]->v[0] );
        m_ColFaces.push_back( m_SortedFaces[i]->v[1] );
        m_ColFaces.push_back( m_SortedFaces[i]->v[2] );
    }
    
    int nVert = m_Verts.size();
    int* idx = &m_ColFaces[0];
    for (int i = 0; i < nCol; i++)
    {
        int vIdx = nVert - 1 - i;
        int colTo = GetCollapseTo( vIdx );
        if (colTo == -1) break;
        for (int j = 0; j < GetNFixFaces( vIdx ); j++)
        {
            int v0, v1, v2;
            int id = GetFixFace( vIdx, j, v0, v1, v2 );
            idx[id*3 + 0] = v0;
            idx[id*3 + 1] = v1;
            idx[id*3 + 2] = v2;
        }
        int nRemoved = GetNRemovedFaces( vIdx );
        for (int j = 0; j < nRemoved; j++)
        {
            m_ColFaces.pop_back();
            m_ColFaces.pop_back();
            m_ColFaces.pop_back();
        }
    }
    m_NCollapses = nCol;
} // VIPMLodder::SetNCollapses

void VIPMLodder::Dump( const char* fname )
{
    int nV = m_Verts.size();
    FILE* fp = fopen( fname, "wt" );
    int nF = m_Faces.size();
    for (int i = 0; i < nV; i++)
    {
        LODVertex& v = m_Verts[i];
        if (v.bFrozen) fprintf( fp, "f" );
        if (v.bBorder) fprintf( fp, "b" );
        if (v.colTo == -1)
        {
            fprintf( fp, "v:%d - not touched\n", v.id );
        }
        else
        {
            fprintf( fp, "v:%d; colOrder:%d; err:%f; colTo:%d; fix:%d; rem:%d; ffix:%d\n", 
                v.id, v.colOrder, v.colError, v.colTo, 
                v.nFixedFaces, v.nRemovedFaces, v.fixFaceIdx );
        }
    }
    fprintf( fp, "\nCollapsed vertices: %d percents\n", m_NumColVerts*100/nV );
    fprintf( fp, "Collapsed faces: %d percents\n", m_NumColFaces*100/nF );
    fprintf( fp, "\nFixed faces: %d percents\n", m_FixFaces.size()*100/nF );

    fclose( fp );
} // VIPMLodder::Dump
