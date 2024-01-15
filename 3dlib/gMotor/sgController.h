/*****************************************************************************/
/*    File:    sgController.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGCONTROLLER_H__
#define __SGCONTROLLER_H__

#include "sgNode.h"
#include "mAnimCurve.hpp"
#include <stack>


/*****************************************************************************/
/*    Class:    Controller
/*    Desc:    Controls scene node state
/*****************************************************************************/
class Controller : public SNode
{
protected:

    virtual void            OnAttach(){}

public:
                            Controller();
                            ~Controller();
    
    _inl void               AttachNode( SNode* _pNode );
    _inl void               DetachNode( SNode* _pNode );

    virtual void            Serialize( OutStream& os ) const;
    virtual void            Unserialize( InStream& is );


    DECLARE_SCLASS(Controller,SNode,CLER);
}; // class Controller

#ifdef _INLINES
#include "sgController.inl"
#endif // _INLINES

#endif // __SGCONTROLLER_H__
