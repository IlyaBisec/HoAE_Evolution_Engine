/************************************************************************************
/*  File:   mLodder.h                                             
/*    Desc:    Class for building VIPM mesh data structure
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   June 2004                                          
/************************************************************************************/
#ifndef __MLODDER_H__
#define __MLODDER_H__

#include <set>
#include <vector>
#include "mQuadric.h"

#pragma pack(push) 
#pragma pack(1) 

const int c_NotCollapsed = INT_MAX;
/*****************************************************************
/*  Struct: LODFace
/*    Desc:    Helper face data structure for lodder        
/*****************************************************************/
struct LODFace
{
    int                id;            //  face id in the mesh
    int             orderedID;  
    int                v[3];        //    vertex indices
    float            area;        //    face area
    Vector3D        normal;        //  face normal
    bool            bCollapsed;    //  whether this face was already collapsed
    int             colOrder;    //  order in which this face is to be collapsed
    int                fixFace;    //  index of the substitute fix face
    
    LODFace() : fixFace(-1), area(0.0f), bCollapsed(false), colOrder(c_NotCollapsed) {}

    LODFace( const LODFace& f ) 
    {    id = f.id; area = f.area; 
        normal = f.normal; bCollapsed = f.bCollapsed; 
        v[0] = f.v[0]; v[1] = f.v[1]; v[2] = f.v[2];
        fixFace = f.fixFace; colOrder = f.colOrder;
    }
    bool HasVertex( int vid ) { return (v[0] == vid || v[1] == vid || v[2] == vid); }

    static int Cmp( const void *arg1, const void *arg2 )
    {
        const LODFace* f1 = *((const LODFace**)arg1);
        const LODFace* f2 = *((const LODFace**)arg2);
        return f2->colOrder - f1->colOrder; 
    }
}; // struct LODFace

/*****************************************************************
/*  Struct: LODVertex
/*    Desc:    Helper vertex data structure for VIPM lodder        
/*****************************************************************/
struct LODVertex
{
    int                id;
    int             orderedID;
    Vector3D        pos;
    Vector3D        normal;
    std::set<int>    nbVertID;            //  neighbor vertices id's
    std::set<int>    nbFaceID;            //    neighbor faces id's
    int                nRemovedFaces;        //  number of faces collapsed by removing this vertex
    int                nFixedFaces;        //    number of faces for which indices are being fixed
    int                fixFaceIdx;            //    index of the first fixed face

    Quadric            quadric;            //    quadric value for this vertex
    bool            bFrozen;            //  when true vertex cannot be collapsed
    bool            bBorder;            //  when true vertex is on border
    bool            bCollapsed;            //    whether already collapsed this vertex
    float            colError;            //  collapsing error value
    int                colTo;                //    id of the vertex this vertex is being collapsed to

    int                colOrder;            //  order in which this vertex is collapsed

    
    bool operator <( const LODVertex& v ) const { return (colError < v.colError); }

    static int Cmp( const void *arg1, const void *arg2 )
    {
        const LODVertex* v1 = *((const LODVertex**)arg1);
        const LODVertex* v2 = *((const LODVertex**)arg2);
        return v2->colOrder - v1->colOrder; 
    }


}; // struct LODVertex

/*****************************************************************
/*  Class:  VIPMLodder     
/*    Desc:    Used for creating view-independent progressive mesh
/*            data structure 
/*****************************************************************/
class VIPMLodder             
{
    bool                            m_bCheckForBorder;  //  whether freeze border vertices
    int                                m_NumColVerts;      //  number of vertices collapsed
    int                                m_NumColFaces;      //  number of faces collapsed

    std::vector<LODFace>            m_Faces;            //  mesh faces
    std::vector<LODFace*>            m_SortedFaces;      //  reordered array of faces

    std::vector<LODVertex>            m_Verts;            //  mesh vertices
    std::vector<LODVertex*>            m_SortedVerts;      //  reordered array of vertices

    std::vector<LODFace>            m_FixFaces;         //  lod substitute faces
    
    std::vector<int>                m_ColFaces;         //  faces, collapsed to some extents
    int                             m_NCollapses;

public:

                    VIPMLodder            () : m_bCheckForBorder(true) {}
    
    bool            AddVertex            ( const Vector3D& v, const Vector3D& n );
    bool            AddVertex            ( const Vector3D& v );
    bool            AddFace                ( int v0, int v1, int v2 );
    void            ReserveFaces        ( int nFaces ) { m_Faces.reserve( nFaces ); m_SortedFaces.reserve( nFaces ); }
    void            ReserveVerts        ( int nVerts ) { m_Verts.reserve( nVerts ); m_SortedVerts.reserve( nVerts ); }

    void            Reset                ();
    void            Process                ( int maxCollapses = -1 );
    
    int             GetNVerts           () const { return m_SortedVerts.size() - m_NCollapses; }
    int             GetNFaces           () const { return m_ColFaces.size()/3; }
    int             GetNFixFaces        () const { return m_FixFaces.size(); }
    int             GetMaxCollapses     () const { return m_NumColVerts; }
    void            SetNCollapses       ( int nCol );

    int             GetCollapseTo       ( int vIdx ) const { return m_SortedVerts[vIdx]->colTo; }
    float           GetCollapseError    ( int vIdx ) const { return m_SortedVerts[vIdx]->colError; }
    int             GetNRemovedFaces    ( int vIdx ) const { return m_SortedVerts[vIdx]->nRemovedFaces; }
    int             GetNFixFaces        ( int vIdx ) const { return m_SortedVerts[vIdx]->nFixedFaces; }

    int             GetOldVIndex        ( int newIdx ) const; 
    Vector3D        GetVertex           ( int idx ) const { return m_SortedVerts[idx]->pos; }
    void            GetFace             ( int idx, int& v0, int& v1, int& v2 ) const;
    int             GetFixFace          ( int vIdx, int ffIdx, int& v0, int& v1, int& v2 ) const;

protected:
    Quadric            CalculateQuadric    ( int vID );
    float            GetCollapseError    ( int vSrcID, int vDstID );
    void            CheckForBorder        ();
    void            FindCollapseTo        ( int vID );
    int                NextToCollapse        ();
    bool            Collapse            ( int vID );
    void            Dump                ( const char* fname = "c:\\dumps\\vipm.txt" );

}; // class VIPMLodder
#pragma pack(pop) 

#endif // __MLODDER_H__