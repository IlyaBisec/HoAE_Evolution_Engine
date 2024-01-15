/*****************************************************************************/
/*	File:	vModel.h
/*	Desc:	ModelObject interface implementation
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#ifndef __VMODEL_H__
#define __VMODEL_H__

#include "IModel.h"
#include "IReflected.h"

#include "IResourceManager.h"

#include "vMesh.h"
#include "vMaterial.h"
#include "vBone.h"
#include "vModelSlot.h"
#include "vIKEffector.h"
#include "vAnimClip.h"
#include "vCollider.h"

const DWORD c_ModelVersion = 0x00000001;
const DWORD c_ModelMagic   = 'DM2C';
/*****************************************************************************/
/*  Class:  ModelObject
/*  Desc:   ModelObject implementation
/*****************************************************************************/
class ModelObject :   public IReflected,
                public IModel, 
                public IResource
{
public:
  
    Skeleton                    m_Skeleton;         //  skeleton description
    MeshList                    m_Meshes;           //  model's geometry
    ShaderTable                 m_Shaders;          //  shaders used in the model
    EffectorList                m_Effectors;        //  model's IK effectors
    SlotList                    m_Slots;            //  model's attachment slots 
    AnimRefList                 m_Animations;       //  valid model's animations
    ControlClipList             m_Clips;            //  model's set of behavioural scripts
    ColliderList                m_Colliders;        //  model's collision geometries   

    std::string                 m_Name;             //  name of the model 
    int                         m_ID;               //  model's id

    AABoundBox                  m_AABB;             //  whole model rest pose bounding box
    Sphere                      m_BoundSphere;      //  whole model rest pose bounding sphere

public:
                            ModelObject               ();
                            ~ModelObject              ();

    //  IModel interface 
    virtual int             GetNVertices        () const;
    virtual int             GetNPoly            () const;
    virtual int             GetNBones           () const { return m_Skeleton.GetNBones(); }
    
    virtual int             GetNMaterials       () const { return m_Shaders.size(); }
    virtual ModelShader*         GetMaterial         ( int id ) { return &m_Shaders[id]; }

    virtual int             GetNMeshes          () const { return m_Meshes.size(); }
    virtual IMesh*          GetMesh             ( int id ) const { return m_Meshes[id]; }

    virtual bool            InstanceGeometry    ( DWORD instID, void* vbuffer ){ return false; }
    virtual const char*     GetName             () const { return m_Name.c_str(); }
    virtual int             GetID               () const { return m_ID; }

    virtual bool            Serialize           ( OutStream& os );
    virtual bool            Unserialize         ( InStream& is );
    virtual DWORD           GetVersion          (){ return c_ModelVersion; }
    const char*             GetVersionString    () const;
    void                    SetName             ( const char* name ) { m_Name = name; }

    //  IReflected interface 
    virtual void		    Expose              ( PropertyMap& pm );

    //  IReflected interface
    virtual IReflected*     Parent              () const { return NULL; }
    virtual int             NumChildren         () const;
    virtual IReflected*     Child               ( int idx ) const;
    virtual bool            AddChild            ( IReflected* pChild )   { return false; }
    virtual bool            DelChild            ( int idx )             { return false; }

    //  IResource interface
    virtual bool            Reload              ();
    virtual void            Reset               ();   

    void                    CalcAABB            ();
    void                    CalcBoundSphere     ();

    const AABoundBox&       GetAABB             () const { return m_AABB; }
    const Sphere&           GetBoundSphere      () const { return m_BoundSphere; }

    int                     GetBoneIndex        ( const char* name ) const;

    DECLARE_CLASS(ModelObject);
}; // class ModelObject

#endif // __VMODEL_H__