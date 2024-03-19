/*****************************************************************************/
/*    File:    sgReflection.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    09-18-2003
/*****************************************************************************/
#ifndef __SGREFLECTION_H__
#define __SGREFLECTION_H__

#include "IMediaManager.h"
#include "kContext.h"


struct ReflectedObject
{
    DWORD           m_ModelID;
    Matrix4D        m_TM;
    EntityContext   m_Context;

    ReflectedObject( DWORD mdlID, 
                     const Matrix4D& tm,
                     const EntityContext& ctx ) : 
                        m_ModelID   ( mdlID ), 
                        m_TM        ( tm ),
                        m_Context   ( ctx ) {}
}; // struct ReflectedObject

/*****************************************************************************/
/*    Class:    ReflectionMap
/*    Desc:     Provides rendering of the reflected objects
/*****************************************************************************/
class ReflectionMap : public SNode, public IReflectionMap
{
    int                             m_ReflID;               //  reflection texture
    int                             m_DepthID;              //  depth buffer surface
    Matrix4D                        m_TextureTM;
            
    int                             m_ReflectionMapSide;    //  reflection texture side
    bool                            m_bUseDepthBuffer;      //  whether z-buffer is used 
    Plane                           m_ReflectionPlane;    
    int                             m_BackdropGridNodes;

    bool                            m_bRenderReflection;
    bool                            m_bRenderBackdrop;
    bool                            m_bInited;
    bool                            m_bDrawDebugInfo;
    bool                            m_bReflectTerrain;

    std::vector<ReflectedObject>    m_Object;

public:
                        ReflectionMap           ();
    virtual void        Serialize               ( OutStream& os ) const;
    virtual void        Unserialize             ( InStream& is );
    virtual void        Render                  ();
    virtual void        Expose                  ( PropertyMap& pm );

    virtual void        AddObject               ( DWORD id, const Matrix4D* objTM );
    virtual void        CleanObjects            ();
    virtual int         GetReflectionTextureID  () const;
    virtual Matrix4D    GetReflectionTexTM      () const;
    void                Init                    ();
    virtual void        SetReflectTerrain       ( bool bReflect = true ) { m_bReflectTerrain = bReflect; }

	virtual void			SetReflectionPlane		(const Plane &P) { m_ReflectionPlane = P; }
	virtual const Plane		GetReflectionPlane		() const { return m_ReflectionPlane; }

    DECLARE_SCLASS( ReflectionMap, SNode, REFL );

protected:
    void                DrawDebugInfo           ();
}; // ReflectionMap

#endif // __SGREFLECTION_H__