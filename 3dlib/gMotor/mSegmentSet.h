/*****************************************************************
/*  File:   mSegmentSet.h                                             
/*    Desc:    System of segments in 3D space 
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   Mar 2004                                          
/*****************************************************************/
#ifndef __MSEGMENTSET_H__
#define __MSEGMENTSET_H__

#include <vector>

/*****************************************************************
/*  Class:  SegmentChain                          
/*  Desc:   Chain of segments, next segment in chain begins where 
/*                previous ends
/*****************************************************************/
class SegmentChain
{
public:
                SegmentChain() : m_bClosed( false ) {}
    int            GetNPoints    () const { return m_Points.size(); }
    int            GetNSegments() const { return m_Points.size() - 1; }

    void        Clear        () { m_Points.clear(); }
    void        AddPoint    ( const Vector3D& pt ) { m_Points.push_back( pt ); }

    _inl void    Transform    ( const Matrix4D& tm );
    
    const Vector3D&    operator[]( int idx ) { return m_Points[idx]; }
    
private:
    std::vector<Vector3D>        m_Points;
    bool                        m_bClosed;
}; // class SegmentChain

/*****************************************************************
/*  Class:  SegmentSet                          
/*  Desc:   Set of segments
/*****************************************************************/
class SegmentSet
{
public:
                SegmentSet() {}
    int            GetNSegments() const { return m_Segments.size(); }

    void        Clear        () { m_Segments.clear(); }
    void        AddSegment    ( const Segment3D& seg ) { m_Segments.push_back( seg ); }
    void        AddSegment    ( const Vector3D& a, const Vector3D& b ) 
    { 
        m_Segments.push_back( Segment3D( a, b ) ); 
    }

    _inl void    Transform    ( const Matrix4D& tm );

    const Segment3D&    operator[]( int idx ) { return m_Segments[idx]; }

private:
    std::vector<Segment3D>        m_Segments;
}; // class SegmentChain

#ifdef _INLINES
#include "mSegmentSet.inl"
#endif // _INLINES

#endif // __MSEGMENTSET_H__