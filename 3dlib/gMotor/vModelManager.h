/*****************************************************************************/
/*	File:	vModelManager.h
/*	Desc:	ModelObject manager interface implementation
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#ifndef __VMODELMANAGER_H__
#define __VMODELMANAGER_H__
#include "IModelManager.h"

const int c_MaxModelInstances   = 16384;
const int c_MaxAnimInstances    = 2048;

typedef StaticHash< ModelInstance, 
                    EntityContext, 
                    c_DefTableSize, 
                    c_MaxModelInstances>        ModelInstHash;
/*****************************************************************************/
/*  Class:  ModelManager
/*  Desc:   Implementation of the model manager
/*****************************************************************************/
class ModelManager : public IModelManager
{
    EntityContext                   m_ModelContext;     //  current model context
    ModelInstHash                   m_ModelInstHash;    //  here model instances are stored
    std::vector<ModelObject*>             m_Models;           //  models registry
    std::vector<SkeletonAnimation*> m_Anims;            //  animations registry
    
    Timer                           m_Timer;

    int                             m_CurrentModel;     //  model, selected in the current context
    int                             m_CurrentInstance;  //  index of the current context's model instance

public:
                            ModelManager        ();
                            ~ModelManager       ();
    ModelInstance*          GetModelInstance    ();
    ModelObject*                  GetModel            ();
    virtual ModelObject*          GetModel            ( int mdlID );

    virtual int             GetModelID          ( const char* name );
    virtual int             GetNModels          () const { return m_Models.size(); }
    virtual bool            UnloadModel         ( int mdlID );
    virtual bool            ReloadModel         ( int mdlID );

    virtual int             GetNAnims           () const;
    virtual int             GetAnimID           ( const char* name );
    virtual IModel*         GetAnim             ( int anmID );
    virtual float           GetAnimTime         ( int anmID );
    virtual bool            UnloadAnim          ( int anmID );
    virtual bool            ReloadAnim          ( int anmID ){ return false; }

    virtual int             GetNSlots           ( int mdlID ) const;
    virtual int             GetNEffectors       ( int mdlID ) const;
    virtual int             GetNMeshes          ( int mdlID ) const;

    virtual int             GetSlotID           ( int mdlID, const char* name ) const;
    virtual int             GetEffectorID       ( int mdlID, const char* name ) const;
    virtual int             GetMeshID           ( int mdlID, const char* name ) const;

    virtual void            PushContext         ( DWORD context ) { m_ModelContext.Push( context ); m_CurrentInstance = -1; }
    virtual void            PopContext          () { m_ModelContext.Pop(); m_CurrentInstance = -1; }

    virtual int             RegisterModel       ( IModel* pModel );
    virtual void            SetModel            ( int mdlID );
    virtual void            Draw                ( const Matrix4D* pTM = NULL );
    virtual int             ScheduleAnimation   (   float scheduleTime,
                                                    int   anmID,
                                                    bool  bLooped   = false,
                                                    float phase     = 0.0f, 
                                                    float playTime  = -1.0f,
                                                    float speed     = 1.0f, 
                                                    float easeIn    = 0.0f, 
                                                    float easeOut   = 0.0f, 
                                                    float weight    = 1.0f );
    virtual int             ScheduleIK          (   float scheduleTime,
                                                    int effectorID, 
                                                    const Vector3D& pos,
                                                    float easeIn = 0.0f, float easeOut = 0.0f, 
                                                    float weight = 1.0f );
    virtual int             ScheduleBonePos     ( float scheduleTime, int boneID, const Vector3D& pos, float time, float w = 1.0f );
    virtual int             ScheduleBoneDir     ( float scheduleTime, int boneID, const Matrix3D& rot, float time, float w = 1.0f );
    virtual int             ScheduleBoneTM      ( float scheduleTime, int boneID, const Matrix4D& tm, float time, float w = 1.0f );

    virtual bool            BindSlot            ( int slotID, int mdlID, const Matrix4D& bindTM = Matrix4D::identity );
    virtual bool            SetMeshShader       ( int meshID, int shaderID );
    virtual bool            EnableMesh          ( int meshID, bool bEnable );
    virtual void            StopController      ( int trackID );
    virtual void            StopController      ();
    virtual int             GetNTracks          ();

}; // class ModelManager

#endif // __VMODELMANAGER_H__