/*****************************************************************
/*  File:   IMesh.h                                      
/*  Desc:   Interface to the mesh manipulation
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Jun 2004                                             
/*****************************************************************/
#ifndef __IMESH_H__ 
#define __IMESH_H__  

#include "IVertexBuffer.h"

class ModelInstance;
class ModelObject;

/*****************************************************************/
/*    Class:    IMesh
/*    Desc:    Interface for managing mesh data
/*****************************************************************/
class IMesh 
{
public:
    virtual const char*                 GetName         () const = 0;

    //  geometry data manipulation
    virtual int                         GetNPoly        () const = 0;
    virtual int                         GetNVerts       () const = 0;
    virtual int                         GetNIndices     () const = 0;
    virtual const VertexDeclaration&    GetVDecl        () const = 0;
    virtual int                         GetHostBone     () const = 0;

    //  retrieving bounding volumes (in local mesh space)
    virtual const AABoundBox&           GetAABB         () const = 0;
    virtual const Sphere&               GetBoundSphere  () const = 0;

    virtual const BYTE*                 GetVertexData   () const = 0;

    //  getting indices/vertices in renderable form
    virtual bool                        InstanceIndices ( BYTE* pBuf, ModelInstance* pInstance ) = 0;
    virtual bool                        InstanceVertices( BYTE* pBuf, ModelInstance* pInstance ) = 0;

    virtual void                        GetRenderAttributes( RenderTask& attr ) = 0;
    virtual ModelObject*                GetModel        () const = 0;


    //  dumps mesh contents to XML file
    virtual bool                        SaveToXML       ( const char* fname ) = 0;
}; // class IMesh

IMesh* CreateFromXML( const char* fname );

#endif // __IMESH_H__ 