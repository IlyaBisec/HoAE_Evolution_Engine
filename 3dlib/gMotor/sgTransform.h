/*****************************************************************************/
/*    File:    sgTransform.h
/*    Desc:    Movable node classes
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGTRANSFORM_H__
#define __SGTRANSFORM_H__

#include "sgNode.h"
#include "sgNodePool.h"

/*****************************************************************************/
/*    Class:   Transform
/*    Desc:    Scene graph node, which can be transformed 
/*****************************************************************************/
class Transform : public SNode
{
public:
    virtual void            Render              ();
    virtual void            Serialize           ( OutStream& os ) const;
    virtual void            Unserialize         ( InStream& is );
    virtual void            Expose              ( PropertyMap& pm );

    DECLARE_SCLASS(Transform,SNode,TRFD);
    
protected:
    Matrix4D                m_LocalTM;          //  current node parent-related transform
    Matrix4D                m_WorldTM;          //  current node world transform
    Matrix4D                m_RestLocalTM;      //  initial node parent-related transform

}; // class Transform 

#endif // __SGTRANSFORM_H__