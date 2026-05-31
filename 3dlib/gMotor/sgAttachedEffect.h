#ifndef __SGATTACHEDEFFECT_H__
#define __SGATTACHEDEFFECT_H__

#include "sgNode.h"
#include "sgNodePool.h"

class AttachedEffect : public SNode
{
    DWORD                   m_EffectID;
    std::string             m_EffectName;
    DWORD                   m_EffectMask;
    float                   m_EffectScale;
    Vector3D                m_DirectionVector;
    Vector3D                m_RelativePosition;
public:
    AttachedEffect();
    void                    DrawEffect          ( Matrix4D& TM );
    void                    SetEffectName       ( const char* name ){ m_EffectName = name; m_EffectID = 0xFFFFFFFF; }
    const char*             GetEffectName       () const { return m_EffectName.c_str(); }

    virtual void            Render              ();
    virtual void            Serialize           ( OutStream& os ) const;
    virtual void            Unserialize         ( InStream& is  );
    virtual void            Expose              ( PropertyMap& pm );
    virtual int             GetGlyphID          () const { return 1; }

    DECLARE_SCLASS(AttachedEffect,SNode,ATEF);
};

#endif //__SGATTACHEDEFFECT_H__