/*****************************************************************************/
/*	File:	vBone.h
/*	Desc:	ModelObject skeleton bone
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#ifndef __VBONE_H__
#define __VBONE_H__
#include "IReflected.h"

/*****************************************************************************/
/*  Struct: ModelBone
/*  Desc:   ModelBone structure
/*****************************************************************************/
struct ModelBone : public IReflected 
{
    int			            m_ID;		        //  id of the bone within parent mesh bone list
    ModelBone*		        m_pParent;		    //  parent bone
    std::string             m_Name;             //  name of the bone
    Matrix4D                m_WorldTM;          //  bone rest pose world transform matrix
    Matrix4D                m_LocalTM;          //  bone rest pose parent-space transform matrix
    Matrix4D                m_Offset;           //  bone offset matrix
    DWORD                   m_Flags;            //  bone status flags
    int                     m_NumChildren;      //  number of children 
    ModelBone*              m_pChild;           //  pointer to the first child bone  
    ModelBone*              m_pNextSibling;     //  pointer to the next sibling node
    ModelBone*              m_pPrevSibling;     //  pointer to the previous sibling node
                            
                            ModelBone           ();

    void                    SetName             ( const char* name  ) { m_Name = name; }
    int                     GetID               () const { return m_ID; }
    virtual void		    Expose              ( PropertyMap& pm   );

    virtual IReflected*     Parent              () const { return m_pParent; }
    virtual int             NumChildren         () const { return m_NumChildren; }
    virtual const char*     GetName             () const { return m_Name.c_str(); }

    virtual IReflected*     Child               ( int idx            ) const;
    virtual bool            AddChild            ( IReflected* pChild );
    virtual bool            DelChild            ( int idx            );

    friend inline InStream& operator >>         ( InStream& is, ModelBone& val );
    friend inline OutStream& operator <<        ( OutStream& os, const ModelBone& val );

    DECLARE_CLASS(ModelBone);
}; // class ModelBone

inline InStream& operator >>( InStream& is, ModelBone& val )
{
    is >> val.m_ID >> val.m_Name >> val.m_LocalTM >> val.m_WorldTM >> 
        val.m_Offset >> val.m_Flags >> val.m_NumChildren >> 
        *((DWORD*)&val.m_pParent)       >>
        *((DWORD*)&val.m_pChild)        >>
        *((DWORD*)&val.m_pNextSibling)  >>
        *((DWORD*)&val.m_pPrevSibling);
    return is;
} 

template <class T> DWORD ID( T* ptr )
{
    if (!ptr) return 0xFFFFFFFF;
    return ptr->GetID();
}

inline OutStream& operator <<( OutStream& os, const ModelBone& val )
{
    os << val.m_ID << val.m_Name << val.m_LocalTM << val.m_WorldTM << val.m_Offset << 
        val.m_Flags << val.m_NumChildren << ID( val.m_pParent ) << ID( val.m_pChild ) << 
        ID( val.m_pNextSibling ) << ID( val.m_pPrevSibling );
    return os;
}

/*****************************************************************************/
/*  Class:  Skeleton
/*  Desc:   Bag with bones
/*****************************************************************************/
class Skeleton : public IReflected, public std::vector<ModelBone>
{
    bool                    m_bDrawBoneLabels;
    bool                    m_bDrawBones;

public:
                            Skeleton            ();

    virtual IReflected*      Parent              () const 
    { 
        if (size() == 0) return NULL; 
        return (IReflected*)(at(0).m_pParent); 
    }
    virtual int             NumChildren         () const;
    virtual IReflected*     Child               ( int idx ) const; 
    virtual void		    Expose              ( PropertyMap& pm );

    virtual const char*     GetName             () const { return "Skeleton"; }
    int                     GetNBones           () const { return size(); }
    bool                    DoDrawBoneLabels    () const { return m_bDrawBoneLabels; }
    bool                    DoDrawBones         () const { return m_bDrawBones; }

    friend inline InStream& operator >>( InStream& is, Skeleton& sk );

    DECLARE_CLASS(Skeleton);
}; // class Skeleton

inline InStream& operator >>( InStream& is, Skeleton& sk )
{
    int sz = sk.size();
    is >> sz;
    sk.resize( sz );
    for (int i = 0; i < sz; i++) is >> sk[i];
    //  bind pointers
    for (int i = 0; i < sz; i++)
    {
        ModelBone& bone = sk[i];
        
        DWORD pID = *((DWORD*)&bone.m_pParent);
        DWORD cID = *((DWORD*)&bone.m_pChild);
        DWORD nID = *((DWORD*)&bone.m_pNextSibling);
        DWORD rID = *((DWORD*)&bone.m_pPrevSibling);

        bone.m_pParent      = (pID == 0xFFFFFFFF ? NULL : &sk[pID]);		
        bone.m_pChild       = (cID == 0xFFFFFFFF ? NULL : &sk[cID]);    
        bone.m_pNextSibling = (nID == 0xFFFFFFFF ? NULL : &sk[nID]);
        bone.m_pPrevSibling = (rID == 0xFFFFFFFF ? NULL : &sk[rID]);
    }
    return is;
} 

#endif // __VBONE_H__