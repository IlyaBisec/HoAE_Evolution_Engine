/*****************************************************************
/*  File:   dbMath.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbMath.h"
#include <float.h>



/*****************************************************************/
/*	MathDebugger implementation
/*****************************************************************/
MathDebugger::MathDebugger()
{
} // MathDebugger::MathDebugger

bool MathDebugger::OnMouseLBUp( int mX, int mY )
{
    return false;
}

bool MathDebugger::OnMouseLBDown( int mX, int mY )
{
	return false;
} // MathDebugger::OnMouseLBDown


bool MathDebugger::OnMouseMove( int mX, int mY, DWORD keys )
{
	return false;
} // MathDebugger::OnMouseMove

bool MathDebugger::OnMouseRBUp( int mX, int mY )
{
	return false;
} // MathDebugger::OnMouseRBUp

void MathDebugger::Expose( PropertyMap& pm )
{
	pm.start( "MathDebugger", this );
} // MathDebugger::Expose

/*****************************************************************/
/*	Class:	LODEstimator
/*	Desc:	Used for estimating LOD ratio
/*****************************************************************/
class LODEstimator
{
    Matrix4D    m_TM;
    float       m_VpW, m_VpH;
public:
    void Init()
    {
        m_TM = IRS->GetViewTM();
        m_TM *= IRS->GetProjTM();
        Rct vp = IRS->GetViewPort();
        m_VpW = vp.w*0.5f;
        m_VpH = vp.h*0.5f;
    }

    float GetRatio( const AABoundBox& aabb, float& pixW, float& pixH )
    {
        Vector3D corner[8];
        aabb.GetCorners( corner );
        Vector4D v = corner[0];
        v *= m_TM;
        float minX = v.x/v.w;
        float minY = v.y/v.w;
        float maxX = minX;
        float maxY = minY;

        for (int i = 1; i < 8; i++)
        {
            v = corner[i];
            v *= m_TM;
            float vx = v.x / v.w;
            float vy = v.y / v.w;
            if (minX > vx) minX = vx;
            if (minY > vy) minY = vy;
            if (maxX < vx) maxX = vx;
            if (maxY < vy) maxY = vy;
        }
        pixW = (maxX - minX)*m_VpW;
        pixH = (maxY - minY)*m_VpH;
        return (pixW*pixW + pixH*pixH)/aabb.minv.distance2( aabb.maxv );
    } 

}; // class LODEstimator

/*****************************************************************/
/*	Class:	VisibilityCuller
/*	Desc:	Used for visibility frustum culling
/*****************************************************************/
class VisibilityCuller
{
    Frustum         m_Frustum;

public:
    void Init()
    {
        m_Frustum = GetCamera()->GetFrustum();
    }

    bool IsVisible( const AABoundBox& aabb )
    {
        return m_Frustum.Overlap( aabb );
    }
}; // class VisibilityCuller

/*****************************************************************/
/*	Class:	QuadTree
/*	Desc:	Generic quad-tree spatial organization data structure.
/*          Data for every node in the quad tree is represented by 
/*          DWORD handle value.
/*****************************************************************/
template <class TObject, class TLod, class TCull>
class QuadTree
{
    static const int c_MaxVisible = 6912;
    struct SNode
    {
        TObject     m_Object;
        DWORD       m_LastUsed;
        AABoundBox  m_AABB;
    }; // struct SNode
    
    int             m_Visible[c_MaxVisible];
    int             m_NVisible;
    SNode*           m_Nodes;

public:
            QuadTree                () : m_NVisible(0){}
    void    SetSide                 ( int side ) 
    {
        delete []m_Nodes; 
    }
    int             GetNVisible     () const        { return m_NVisible; }
    const int*      GetVisible      () const        { return m_Visible; }
    TObject&        GetVisible      ( int idx )     { return m_Nodes[m_Visible[idx]].m_Object; }
    bool            SetVisible      ( int* pvs, int pvsSize )   
    {
        if (pvsSize > c_MaxVisible) return false;
        memcpy( m_Visible, pvs, sizeof(int)*pvsSize );
        for (int i = 0; i < pvsSize; i++)
        {
            m_Nodes[pvs[i]].Validate( Rct::null );
        }
        return true;
    }
    void        ProcessVisibility   ( TLod& lod, TCull& cull )
    {
        m_NVisible = 0;
    }

}; // class QuadTree

struct DebugBox
{
    AABoundBox  m_AABB;

    bool        Validate( const Rct& extents )
    {
        return true;
    }
}; // class DebugBox


LODEstimator                                g_LODEstimator;
VisibilityCuller                            g_Culler;
QuadTree<   DebugBox,
            LODEstimator, 
            VisibilityCuller>               g_QuadTree;


const float c_QScale = 10000.0f;
const float c_MinimapSide = 500.0f;
void DebugErrorMetric();

void MathDebugger::Render()
{
    g_LODEstimator.Init();
    g_Culler.Init();

    g_QuadTree.ProcessVisibility( g_LODEstimator, g_Culler );

    int nVis = g_QuadTree.GetNVisible();
    for (int i = 0; i < nVis; i++)
    {
        DebugBox& box = g_QuadTree.GetVisible( i );
        AABoundBox aabb = box.m_AABB;
        DrawAABB( aabb, 0x33FF0000, 0x77FF0000 );
        Rct rct( aabb.minv.x, aabb.minv.y, aabb.GetDX(), aabb.GetDY() );
        rct *= c_MinimapSide/c_QScale;
        rsFrame( rct, 0, 0x770000FF );
    }
    rsFrame( Rct( 0, 0, c_MinimapSide, c_MinimapSide ), 0, 0xFFFF0000 );
    rsFlush();
    
    //DebugErrorMetric();
} // MathDebugger::Render



void DebugErrorMetric()
{
    float pixW, pixH;
    AABoundBox aabb( Rct( -100, -200, 300, 400 ), -10, 200 );
    DrawAABB( aabb, 0x440000FF, 0xFF0000FF );

    float ratio = g_LODEstimator.GetRatio( aabb, pixW, pixH );    
    Vector4D c = aabb.GetCenter();
    GetCamera()->ToSpace( sWorld, sScreen,  c );
    Rct crect( c.x - pixW*0.5f, c.y - pixH*0.5f, pixW, pixH );
    rsRect( crect, 0, 0x4400FF00 );
    rsFlushLines2D();

    DrawText( 10, 100, 0xFFFFFFFF, "Ratio: %f", ratio );
    FlushText();
}


