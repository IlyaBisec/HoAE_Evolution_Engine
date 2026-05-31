/*****************************************************************************/
/*    File:    IMediaManager.h
/*    Desc:    Interface for the straightforward model management
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-15-2003
/*****************************************************************************/
#ifndef __IMEDIAMANAGER_H__
#define __IMEDIAMANAGER_H__

class Vector3D;
class Vector4D;
class Matrix3D;
class Matrix4D;
class Frustum;
class Line3D;
class Rct;
class AABoundBox;
class Primitive;
class VertexIterator;
class BaseMesh;

#include "ICamera.h"
#include "ILight.h"
#include "mTransform.h"

typedef void        (*SetHeightCallback)    ( int, int, float );
typedef void        (*VisitHeightCallback)  ( int, int );

/*****************************************************************************/
/*    Class:    IGeometry
/*    Desc:    Interface for geometry manipulation
/*****************************************************************************/
class IGeometry
{
public:
    virtual void        Render          () = 0;
    virtual int         GetVertexStride () const = 0;
    virtual int         GetIndexStride  () const = 0;
    virtual int         GetNumVertices  () const = 0;
    virtual int         GetNumIndices   () const = 0;
    virtual WORD*       GetIndices      () = 0;
    virtual Vector3D    GetCenter       () const = 0;
    virtual void        GetVertexIterator( VertexIterator& it ) = 0;
    virtual int         Pick            ( const Vector3D& org, const Vector3D& dir, Vector3D& pt ) = 0;

}; // class IGeometry

/*****************************************************************************/
/*    Class:    IReflectionMap
/*    Desc:    Interface for manipulation of models, animations, cursors
/*                and other stuff. Plays role of the facade to the sg entities.
/*****************************************************************************/
class IReflectionMap
{
public:

    virtual void			AddObject               ( DWORD id, const Matrix4D* objTM ) = 0;
    virtual void			CleanObjects            () = 0;
    virtual int				GetReflectionTextureID  () const = 0;
    virtual void			Render                  () = 0;
    virtual Matrix4D		GetReflectionTexTM      () const = 0;
    virtual void			SetReflectTerrain       ( bool bReflect = true ) = 0;
	virtual void			SetReflectionPlane		(const Plane &) = 0;
	virtual const Plane		GetReflectionPlane		() const = 0;
}; // class IReflectionMap

const DWORD NO_CONTEXT = 0;
const DWORD CUR_CONTEXT = 0xFFFFFFFF;

class SNode;
/*****************************************************************************/
/*    Class:    IMediaManager
/*    Desc:    Interface for manipulation of models, animations, cursors
/*                and other stuff. Plays role of the facade to the sg entities.
/*****************************************************************************/
class IMediaManager
{
public:
    //  returns handle of the model with given file name
    virtual DWORD           GetModelID          ( const char* fname ) = 0;
	virtual void			ReloadModel			(DWORD idModel) = 0;
    virtual SNode*          GetModelRoot        ( DWORD modelID ) = 0;
    virtual const char*     GetModelFileName    ( DWORD modelID ) = 0;
	virtual void			GetModelBounds		(DWORD idModel, std::vector<AABoundBox> &Bounds, std::vector<Matrix4D> &Transforms) = 0;

    //  returns handle to the child node of the given model
    virtual DWORD           GetNodeID           ( DWORD modelID, const char* nodeName ) = 0;
    virtual Matrix4D        GetNodeTransform    ( DWORD nodeID, bool bLocalSpace = false ) = 0;        
    virtual void            SetNodeTransform    ( DWORD nodeID, const Matrix4D& m, bool bLocalSpace = false ) = 0;

    virtual DWORD           GetNodeID           ( const char* name ) = 0;
    virtual void            ShowNode            ( DWORD id, bool bShow = true ) = 0;
    virtual void            SetVisible          ( DWORD nodeID, bool bVisible = true ) = 0;
    
    virtual DWORD           CloneNode           ( DWORD nodeID ) = 0;
    virtual void            DeleteNode          ( DWORD nodeID ) = 0;

    //  returns current node transform
    virtual ILight*         GetLight            ( DWORD nodeID ) = 0;
    virtual IGeometry*      GetGeometry         ( DWORD nodeID ) = 0;

    virtual int             GetNumGeometries    ( DWORD modelID ) = 0;
    virtual IGeometry*      GetGeometry         ( DWORD modelID, int idx, Matrix4D& tm ) = 0;

    virtual int             GetNumSubNodes      ( DWORD modelID ) = 0;
    virtual const char*     GetSubNodeName      ( DWORD modelID, int idx ) = 0;

	virtual void			ExportToObj			( DWORD modelID,const char* Name ) = 0;

    virtual void            Init                () = 0;
    virtual void            OnFrame             () = 0;
    
    //  sets current node transform 
    //  makes node parentID parent/non-parent to the node childID
    virtual bool            MakeParent          ( DWORD parentID, DWORD childID, bool bParent = true ) = 0;
    
    //  starts model drawing. 
    //  if context != NO_CONTEXT, model is instanced (or existing instance within the same context is being used)
    //  if context == CUR_CONTEXT, current context is being used for model instance
    //  StartModel should be followed by DrawModel to actually submit model for drawing
    virtual void            StartModel          ( DWORD modelID, const Matrix4D& tm = Matrix4D::identity, const DWORD context = NO_CONTEXT ) = 0;
    //  applies animation to the model currently being drawn
    virtual void            AnimateModel        ( DWORD animID, float animTime, float blendInTime = 0.5f, const Matrix4D& shiftTM = Matrix4D::identity ) = 0;
    //  actually draws current model/instance
    virtual void            DrawModel           () = 0;
    //  set/get mask for attached effects
    virtual void            SetAttachedEffectMask(DWORD Mask) = 0;
    virtual DWORD           GetAttachedEffectMask() = 0;
    // set/get bone view/pick mode
    virtual void            SetBoneViewMode(bool State) = 0;
    virtual bool            GetBoneViewMode() = 0;
    
    //  returns total time of the given animation, in ms
    virtual float           GetAnimTime         ( DWORD animID ) = 0;
    virtual void            SetCamera           ( const Vector3D& lookAt, float viewVolWidth ) = 0;

    //  draws shadow for the model with given handle/world transform
    virtual void            RenderShadow        ( DWORD id, const Matrix4D* pTransform = NULL ) = 0;
    
    //  returns model bounding box
    virtual AABoundBox      GetBoundBox         ( DWORD id ) = 0;

    //  finds maximum z value of the model's mesh at the point (pt.x, pt.y), result
    //        is placed into the pt.z, returns false if no such point 
    virtual bool            GetHeight           ( DWORD id, Vector3D& pt, const Matrix4D* pTransform = NULL ) = 0; 
    
    //  finds passability of the mesh at the point
    //  returns true when the point is locked
    virtual bool            GetLock             ( DWORD id, const Vector3D& pt, const Matrix4D* pTransform = NULL ) = 0; 
    
    //  sets current switch state
    virtual bool            SwitchTo            ( DWORD id, int index ) = 0;
    virtual void            ReloadModels        () = 0;

    //  occlusion utilities
    virtual bool            IsBoxVisible        ( const Vector3D& minv, const Vector3D& maxv ) = 0;
    virtual bool            IsSphereVisible     ( const Vector3D& center, float radius ) = 0;
    virtual bool            IsPointVisible      ( const Vector3D& pt ) = 0;

    virtual BaseMesh*       GetGeometry         ( int mdlID, int idx = 0 ) = 0;
    virtual void            SaveMesh            ( const char* fileName , BaseMesh* bm ) = 0;
    virtual void            SaveMeshes          ( const char* fileName , BaseMesh** bm , int numMeshes) = 0;
    
    //  world objects space manipulation
    //  registering static model
    virtual void            RegisterVisible     ( DWORD objID, DWORD modelID, const Matrix4D& tm ) = 0;
    //  registering box
    virtual void            RegisterVisible     ( DWORD objID, const Vector3D& minv, const Vector3D& maxv    ) = 0;
    //  registering sphere
    virtual void            RegisterVisible     ( DWORD objID, const Vector3D& center, float radius ) = 0;
    //  registering sprite
    virtual void            RegisterVisible     ( DWORD objID, DWORD gpID, int frameID, const Matrix4D& tm ) = 0;
    
    //  removes all previously registered visible objects
    virtual void            ClearVisibleCache   () = 0;

    //  picking with mouse
    virtual DWORD           PickVisible         ( float scrX, float scrY, DWORD prevID = 0xFFFFFFFF ) = 0;
    virtual void            ScanHeightmap       ( DWORD nodeID, const Matrix4D& tm, float stepx, float stepy, SetHeightCallback callb ) = 0;
    virtual void            ScanHeightmap       ( DWORD nodeID, const Matrix4D& tm, float stepx, float stepy, VisitHeightCallback callb ) = 0;    
    virtual bool            Intersects          ( float mX, float mY, DWORD modelID, const Matrix4D& tm, float& dist ) = 0;
    //to use in debug
    virtual const char*     GetModelOfNodeName  ( DWORD NodeID ) = 0;
}; // class IMediaManager

//  use this 
extern DIALOGS_API IMediaManager*       IMM;
extern DIALOGS_API IReflectionMap*      IRMap;
#include <vector>
bool LoadRawModel( const char* fname, std::vector<Vector3D>& vert, 
                    std::vector<int>& idx );

#endif // __IMEDIAMANAGER_H__