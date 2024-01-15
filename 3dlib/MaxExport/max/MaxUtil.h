/*****************************************************************************/
/*    File:    MaxUtil.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07.11.2002
/*****************************************************************************/
#ifndef __MAXUTIL_H__
#define __MAXUTIL_H__

#include "Max.h"

int            TotalChildrenNum( INode* node );
bool        IsHardEdge( const Mesh& mesh, AdjFaceList& adjFaceList, 
                        DWORD faceIdx, DWORD vert1, DWORD vert2 );
Matrix4D    ConvertMatrix( const Matrix3& tm );
Matrix3        ConvertMatrix( const Matrix4D& tm );
Matrix3        GetNodeLocalTM( INode* node, TimeValue t );
DWORD        ColorToDWORD( const Color& color );
float        TimeToMs( TimeValue t );
TimeValue    MsToTime( float ms );

/*****************************************************************************/
/*    Class:    VNormal
/*    Desc:    Helper class representing vertex normal
/*****************************************************************************/
class VNormal
{
public:
    Point3        norm;
    DWORD        smoothBits;
    VNormal        *next;
    BOOL        init;

    VNormal()
    {
        Init();
    }

    void Init()
    {
        smoothBits    = 0;
        next        = NULL;
        init        = FALSE;
        norm        = Point3( 0, 0, 0 );
    }

    VNormal( Point3 &n, DWORD sB ) 
    {
        next        = NULL;
        init        = TRUE;
        norm        = n;
        smoothBits    = sB;
    }

    ~VNormal() 
    {
        delete next;
    }

    void AddNormal( Point3 &n, DWORD sB );

    Point3 &GetNormal( DWORD sB );

    void Normalize();
}; // class VNormal

void ComputeVertexNormals( const Mesh& mesh, Tab<VNormal>& vnorms );

#endif // __MAXUTIL_H__ 