/*****************************************************************************/
/*    File:    sgModel.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGMODEL_H__
#define __SGMODEL_H__

#include "IResourceManager.h"
#include "sgTransform.h"

class Geometry;
class TransformNode;
/*****************************************************************************/
/*    Class:    Model
/*    Desc:    
/*****************************************************************************/
const DWORD c_CurModelVersion = 0;
class Model : public SNode, public IResource
{
    std::string                         m_FileName;
    AABoundBox                          m_AABB;
    DWORD                               m_Version;
    bool                                m_bLoaded;

    std::vector<AABoundBox>             m_Shell;
    std::vector<Transform*>             m_Skeleton;

public:
                        Model           () : m_Version( c_CurModelVersion ), m_bLoaded(false) {}
    const char*         GetFileName     () const { return m_FileName.c_str(); }
    void                SetFileName     ( const char* name ) { m_FileName = name; }

    const AABoundBox&   GetAABB         () const { return m_AABB; }
    void                SetAABB         ( const AABoundBox& aabb ){ m_AABB = aabb; }

    virtual void        Serialize       ( OutStream& os ) const;
    virtual void        Unserialize     ( InStream& is );
    virtual bool        Reload          ();
    virtual bool        Load            ();
    virtual bool        Dispose         ();
    virtual void        Render          ();

    void                CreateShell     ();
    int                 GetShellSize    () const { return m_Shell.size(); }
    const AABoundBox&   GetShellElem    ( int idx ) const { return m_Shell[idx]; }


    DECLARE_SCLASS(Model,SNode,MDEL);
}; // class Model

#endif // __SGMODEL_H__