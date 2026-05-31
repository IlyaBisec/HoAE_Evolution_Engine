/*****************************************************************************/
/*	File:	vModelSlot.h
/*	Desc:	ModelObject attachment point
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#ifndef __VMODELSLOT_H__
#define __VMODELSLOT_H__
#include "IReflected.h"


class ModelObject;
/*****************************************************************************/
/*  Struct: ModelSlot
/*  Desc:   ModelObject attachment point
/*****************************************************************************/
struct ModelSlot : public IReflected
{
    int			            m_ID;		        //  id of the ModelSlot within parent mesh ModelSlot list
    std::string             m_Name;             //  name of the slot
    Matrix4D                m_Transform;        //  slot transform, relative to the host bone
    std::string             m_HostBone;         //  bone index, to which slot is bound to
    int                     m_HostBoneID;
    DWORD                   m_Flags;            //  slot status flags
    ModelObject*                  m_pModel;
    std::string             m_AttachedModel;    //  name of attached model

    ModelSlot                                   ();
    void                    SetName             ( const char* name  ) { m_Name = name; }
    virtual void		    Expose              ( PropertyMap& pm   );

    virtual const char*     GetName             () const { return m_Name.c_str(); }
    virtual const char*     ClassName           () const { return "ModelSlot"; }

    void                    SetAttachedName     ( const char* name  ) { m_AttachedModel = name; }
    const char*             GetAttachedName     () const { return m_AttachedModel.c_str(); }

    void                    SetHostBone         ( const char* name  ) { m_HostBone = name; }
    void                    SetModel            ( ModelObject* pModel ) { m_pModel = pModel; }
    const char*             GetHostBone         () const { return m_HostBone.c_str(); }
    int                     GetHostBoneID       ();

    friend inline InStream& operator >>( InStream& is, ModelSlot& val );
    friend inline OutStream& operator <<( OutStream& os, const ModelSlot& val );
}; // class ModelSlot

inline InStream& operator >>( InStream& is, ModelSlot& val )
{
    is >> val.m_Name >> val.m_Transform >> val.m_HostBone >> val.m_Flags >> val.m_AttachedModel;
    return is;
} 

inline OutStream& operator <<( OutStream& os, const ModelSlot& val )
{
    os << val.m_Name << val.m_Transform << val.m_HostBone << val.m_Flags << val.m_AttachedModel;
    return os;
}

/*****************************************************************************/
/*  Class:  SlotList
/*  Desc:   Bag with bones
/*****************************************************************************/
class SlotList : public IReflected, public std::vector<ModelSlot>
{
    bool                    m_bDrawSlots;

public:
    virtual void		    Expose              ( PropertyMap& pm );    

    virtual IReflected*      Parent              () const { return NULL; }
    virtual int             NumChildren         () const                { return size(); }
    virtual IReflected*      Child               ( int idx ) const       
    { 
        if (idx < 0 || idx >= size()) return NULL;
        return (IReflected*)&at( idx ); 
    }
    virtual const char*     ClassName           () const                { return "SlotList"; }
    virtual const char*     GetName             () const                { return "Slots"; }

    virtual bool            AddChild            ( IReflected* pChild )   { return false; }
    virtual bool            DelChild            ( int idx )             { return false; }
    int                     GetNSlots           () const { return size(); }

    void                    SetDrawSlots        ( bool bDraw = true )   { m_bDrawSlots = bDraw; }
    bool                    DoDrawSlots         () const                { return m_bDrawSlots; }

}; // class SlotList

#endif // __VMODELSLOT_H__