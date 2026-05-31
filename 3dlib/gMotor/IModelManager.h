/*****************************************************************************/
/*    File:    IModelManager.h
/*    Desc:    ModelObject manager interface 
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __IMODELMANAGER_H__
#define __IMODELMANAGER_H__

#include "IModel.h"

class ModelInstance;
class ModelObject;
/*****************************************************************************/
/*  Class:  IModelManager
/*  Desc:   ModelObject manager interface
/*****************************************************************************/
class IModelManager
{
public:
    ///  loads model from the file, or returns ID if already loaded
    virtual int             GetModelID          ( const char* name )    = 0;
    ///  current number of models loaded
    virtual int             GetNModels          () const                = 0;
    ///  deletes model from the memory
    virtual bool            UnloadModel         ( int mdlID )           = 0;
    ///  reloads model with given ID
    virtual bool            ReloadModel         ( int mdlID )           = 0;
    
    ///  returns total number of animations 
    virtual int             GetNAnims           () const                = 0;
    virtual int             GetAnimID           ( const char* name )    = 0;
    virtual IModel*         GetAnim             ( int anmID )           = 0;
    virtual float           GetAnimTime         ( int anmID )           = 0;
    virtual bool            UnloadAnim          ( int anmID )           = 0;
    virtual bool            ReloadAnim          ( int anmID )           = 0;

    ///  retrieves number of the slots in the model
    virtual int             GetNSlots           ( int mdlID ) const     = 0;
    ///  gets handle to the model slot by its name
    virtual int             GetSlotID           ( int mdlID, const char* name ) const = 0;

    ///  retrieves number of the IK effectors in the model
    virtual int             GetNEffectors       ( int mdlID ) const     = 0;
    ///  gets handle to the model IK effector by its name
    virtual int             GetEffectorID       ( int mdlID, const char* name ) const = 0;

    ///  retrieves number of the meshes in the model
    virtual int             GetNMeshes          ( int mdlID ) const     = 0;
    ///  gets handle to the model mesh by its name
    virtual int             GetMeshID           ( int mdlID, const char* name ) const = 0;

    ///  deepens into current model instance context
    virtual void            PushContext         ( DWORD context )       = 0;
    ///  rises from the current model instance context
    virtual void            PopContext          ()                      = 0;

    ///  adds model to the model registry
    virtual int             RegisterModel       ( IModel* pModel )      = 0;
    ///  sets model drawn in current instance context
    virtual void            SetModel            ( int mdlID )           = 0;
    ///  submit model instance render task to the renderer
    virtual void            Draw                ( const Matrix4D* pTM = NULL ) = 0;
    ///  plays animation track for the current model instance
    virtual int             ScheduleAnimation   (   float scheduleTime,
                                                    int   anmID, 
                                                    bool  bLooped   = false,
                                                    float phase     = 0.0f, 
                                                    float playTime  = -1.0f,
                                                    float speed     = 1.0f, 
                                                    float easeIn    = 0.0f, 
                                                    float easeOut   = 0.0f, 
                                                    float weight    = 1.0f ) = 0;
    ///  binds IK Effector to given world-space position
    virtual int             ScheduleIK          (   float scheduleTime,
                                                    int effectorID, 
                                                    const Vector3D& pos,
                                                    float easeIn = 0.0f, float easeOut = 0.0f, 
                                                    float weight = 1.0f ) = 0;
    //  direct bone manipulation
    virtual int             ScheduleBonePos     ( float scheduleTime, int boneID, const Vector3D& pos, float time, float w = 1.0f ) = 0;
    virtual int             ScheduleBoneDir     ( float scheduleTime, int boneID, const Matrix3D& rot, float time, float w = 1.0f ) = 0;
    virtual int             ScheduleBoneTM      ( float scheduleTime, int boneID, const Matrix4D& tm, float time, float w = 1.0f ) = 0;

    ///  binds another model instance to the current model slot
    virtual bool            BindSlot            ( int slotID, int mdlID, const Matrix4D& bindTM = Matrix4D::identity ) = 0;
    ///  changes shader for given mesh instance
    virtual bool            SetMeshShader       ( int meshID, int shaderID ) = 0;
    ///  enables/disables rendering of the given mesh instance
    virtual bool            EnableMesh          ( int meshID, bool bEnable ) = 0;

    ///  discards current model instance controller mixer track with given index
    virtual void            StopController       ( int trackID ) = 0;
    virtual void            StopController       () = 0;

    ///  returns number of the currently played controller tracks
    virtual int             GetNTracks          () = 0;

    virtual ModelInstance*  GetModelInstance    () = 0;
    virtual ModelObject*    GetModel            () = 0;
    virtual ModelObject*    GetModel            ( int mdlID ) = 0;
}; // class IModelManager

extern IModelManager* IModelMgr;

#endif // __IMODELMANAGER_H__


