/*****************************************************************************/
/*	File:	vIKEffector.h
/*	Desc:	ModelObject attachment point
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#ifndef __VIKEFFECTOR_H__
#define __VIKEFFECTOR_H__
#include "IReflected.h"


/*****************************************************************************/
/*  Struct: IKEffector
/*  Desc:   ModelObject attachment point
/*****************************************************************************/
struct IKEffector : public IReflected
{
    int			            m_ID;		        //  id of the IKEffector within parent mesh IKEffector list
    std::string             m_Name;             //  name of the slot
    Matrix4D                m_Transform;        //  slot transform, relative to the host bone
    int                     m_HostBone;         //  bone index, to which slot is bound to
    DWORD                   m_Flags;            //  slot status flags

                            IKEffector          ();
    void                    SetName             ( const char* name  ) { m_Name = name; }
    virtual void		    Expose              ( PropertyMap& pm   );
    virtual const char*     GetName             () const { return m_Name.c_str(); }
    virtual const char*     ClassName           () const { return "IKEffector"; }

    friend inline InStream& operator >>( InStream& is, IKEffector& val );
    friend inline OutStream& operator <<( OutStream& os, const IKEffector& val );
}; // class IKEffector

inline InStream& operator >>( InStream& is, IKEffector& val )
{
    return is;
} 

inline OutStream& operator <<( OutStream& os, const IKEffector& val )
{
    return os;
}

/*****************************************************************************/
/*  Class:  EffectorList
/*****************************************************************************/
class EffectorList : public IReflected, public std::vector<IKEffector>
{
public:
    virtual void		    Expose              ( PropertyMap& pm );    

    virtual IReflected*      Parent              () const { return NULL; }
    virtual int             NumChildren         () const                { return size(); }
    virtual IReflected*      Child               ( int idx ) const       
    { 
        if (idx < 0 || idx >= size()) return NULL;
        return (IReflected*)&at( idx ); 
    }
    virtual const char*     ClassName           () const                { return "EffectorList"; }
    virtual const char*     GetName             () const                { return "Effectors"; }

    virtual bool            AddChild            ( IReflected* pChild )   { return false; }
    virtual bool            DelChild            ( int idx )             { return false; }

    int                     GetNEffectors       () const { return size(); }

}; // class EffectorList

#endif // __VIKEFFECTOR_H__