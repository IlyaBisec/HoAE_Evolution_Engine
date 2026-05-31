/*****************************************************************************/
/*    File:    IModel.h
/*    Desc:    ModelObject interface 
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __IMODEL_H__
#define __IMODEL_H__

/*****************************************************************************/
/*  Class:  IModel
/*  Desc:   ModelObject interface
/*****************************************************************************/
class IModel
{
public:
    virtual int             GetNVertices        () const = 0;
    virtual int             GetNPoly            () const = 0;
    virtual int             GetNMaterials       () const = 0;
    virtual int             GetNMeshes          () const = 0;
    virtual int             GetNBones           () const = 0;

    virtual bool            InstanceGeometry    ( DWORD instID, void* vbuffer ) = 0;
    
    virtual bool            Serialize           ( OutStream& os ) = 0;
    virtual bool            Unserialize         ( InStream& is ) = 0;

    virtual void            Reset               () = 0;
    virtual DWORD           GetVersion          () = 0;
    virtual const char*     GetName             () const = 0;
    virtual int             GetID               () const = 0;

}; // class IModel

/*****************************************************************************/
/*  Enum:   LoopMode
/*  Desc:   Mode of the animation looping
/*****************************************************************************/
enum LoopMode
{
    lmNone  = 0,
    lmCycle = 1,
    lmPong  = 2
}; // enum LoopMode

ENUM( LoopMode, "LoopMode", 
     en_val( lmNone, "None"    ) << en_val( lmCycle, "Cycle" ) << en_val( lmPong, "Pong" ) );

#endif // __IMODEL_H__


