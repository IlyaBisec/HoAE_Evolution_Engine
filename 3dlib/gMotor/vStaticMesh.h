/*****************************************************************************/
/*	File:	vStaticMesh.h
/*	Desc:	Mesh interface implementation
/*	Author:	Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __VSTATICMESH_H__
#define __VSTATICMESH_H__

class ModelObject;
/*****************************************************************************/
/*  Class:  StaticMesh
/*  Desc:   Static mesh does not change its geometry during lifetime, 
/*           so most of the times it resides in the hardware vertex buffer
/*****************************************************************************/
class StaticMesh : public Mesh
{
    int                         m_HostBone;     //  bone in the model this mesh is attached to
    std::string                 m_HostBoneName;

    DWORD                       m_ICacheStamp;  //  hardware index caching watermark
    DWORD                       m_VCacheStamp;  //  hardware vertex caching watermark   

    int                         m_VBufferPos;
    int                         m_IBufferPos;
    
    static int                  s_VBuffer;
    static int                  s_IBuffer;


public:
                                StaticMesh      ();
    void                        SetHostBone     ( int boneIdx ) { m_HostBone = boneIdx; }
    int                         GetHostBone     () const        { return m_HostBone; }

    void                        SetHostBoneName ( const char* name ) { m_HostBoneName = name; }
    const char*                 GetHostBoneName () const             { return m_HostBoneName.c_str(); }

    virtual void		        Expose          ( PropertyMap& pm );

    virtual bool                InstanceIndices ( BYTE* pBuf, ModelInstance* pInstance );
    virtual bool                InstanceVertices( BYTE* pBuf, ModelInstance* pInstance );

    DWORD                       GetICacheStamp  () const { return m_ICacheStamp; }
    DWORD                       GetVCacheStamp  () const { return m_VCacheStamp; }

    virtual int                 GetVBufferID    () const { return s_VBuffer; }
    virtual int                 GetIBufferID    () const { return s_IBuffer; }

    virtual void                Serialize       ( OutStream& os ) const;
    virtual void                Unserialize     ( InStream& is );
    bool                        Draw            ( ModelInstance* m ); 
    
    DECLARE_CLASS(StaticMesh);
}; // class StaticMesh

#endif // __VSTATICMESH_H__