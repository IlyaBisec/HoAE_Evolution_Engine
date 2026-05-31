/*****************************************************************************/
/*    File:    sgGizmo.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-28-2003
/*****************************************************************************/
#ifndef __SG_GIZMO_H__
#define  __SG_GIZMO_H__



/*****************************************************************************/
/*    Ñlass:    VectorField
/*    Desc:    Drawn as set of vectors
/*****************************************************************************/
class VectorField : public Geometry
{
public:
    bool            CreateNormalField( Geometry* pGeom, DWORD color = 0xFFFF0000 );
    bool            CreateVectorField( int nVectors );
    bool            AddVector( const Vector3D& v, const Vector3D& dir, DWORD color = 0xFFFF0000 );

    DECLARE_SCLASS(VectorField,Geometry,VFLD);
}; // class VectorField 



#endif // __SG_GIZMO_H__
