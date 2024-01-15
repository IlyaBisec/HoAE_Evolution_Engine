/*****************************************************************************/
/*	File:	vCollider.h
/*	Desc:	ModelObject attachment point
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#ifndef __VCOLLIDER_H__
#define __VCOLLIDER_H__
#include "IReflected.h"


/*****************************************************************************/
/*  Struct: Collider
/*  Desc:   ModelObject attachment point
/*****************************************************************************/
struct Collider : public IReflected
{
    int			            m_ID;		        //  id of the Collider within parent mesh Collider list
    std::string             m_Name;             //  name of the slot
    Matrix4D                m_Transform;        //  slot transform, relative to the host bone
    int                     m_HostBone;         //  bone index, to which slot is bound to
    DWORD                   m_Flags;            //  slot status flags

                            Collider            ();
    void                    SetName             ( const char* name  ) { m_Name = name; }
    virtual void		    Expose              ( PropertyMap& pm   );
    virtual const char*     GetName             () const { return m_Name.c_str(); }
    virtual const char*     ClassName           () const { return "Collider"; }

    friend inline InStream& operator >>( InStream& is, Collider& val );
    friend inline OutStream& operator <<( OutStream& os, const Collider& val );
}; // class Collider

inline InStream& operator >>( InStream& is, Collider& val )
{
    return is;
} 

inline OutStream& operator <<( OutStream& os, const Collider& val )
{
    return os;
}

/*****************************************************************************/
/*  Class:  ColliderList
/*****************************************************************************/
class ColliderList : public IReflected, public std::vector<Collider>
{
public:
    virtual void		    Expose              ( PropertyMap& pm );    

    virtual IReflected*      Parent              () const { return NULL; }
    virtual int             NumChildren         () const { return size(); }
    virtual IReflected*      Child               ( int idx ) const       
    { 
        if (idx < 0 || idx >= size()) return NULL;
        return (IReflected*)&at( idx ); 
    }
    virtual const char*     ClassName           () const                { return "ColliderList"; }
    virtual const char*     GetName             () const                { return "Colliders"; }

    virtual bool            AddChild            ( IReflected* pChild )   { return false; }
    virtual bool            DelChild            ( int idx )             { return false; }

    int                     GetNColliders       () const { return size(); }

}; // class ColliderList

#endif // __VCOLLIDER_H__