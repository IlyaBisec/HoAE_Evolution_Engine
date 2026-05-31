/*****************************************************************************/
/*    File:    sgSkybox.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-28-2003
/*****************************************************************************/
#ifndef __SGSKYBOX_H__
#define __SGSKYBOX_H__



/*****************************************************************************/
/*    Class:    Skybox
/*    Desc:    Well, not a big deal...
/*****************************************************************************/
class Skybox : public TransformNode
{
    DWORD                    topColor;
    DWORD                    bottomColor;
    
    bool                    bNeedReassignColors;

    static bool                s_bFrozen;

public:
                            Skybox();
    virtual void            Render();
    virtual void            Serialize( OutStream& os ) const;
    virtual void            Unserialize( InStream& is );


    _inl DWORD                GetTopColor        () const { return topColor;     }
    _inl DWORD                GetBottomColor    () const { return bottomColor; }
    
    static _inl void        Freeze()    { s_bFrozen = true;        } 
    static _inl void        Unfreeze()    { s_bFrozen = false;    } 

    _inl void                SetTopColor        ( DWORD val ) 
    { 
        topColor = val; 
        ReassignColors(); 
    }
    _inl void                SetBottomColor    ( DWORD val ) 
    { 
        bottomColor = val; 
        ReassignColors(); 
    }


    DECLARE_SCLASS(Skybox,TransformNode,SKYB);

protected:

    void                    ReassignColors();
}; // class Skybox

//class _Skybox : public _TransformNode
//{
//public:
//
//    FLOAT_NPROP( Skybox, GetTopColor,        SetTopColor        );
//    FLOAT_NPROP( Skybox, GetBottomColor,    SetBottomColor    );
//
//    SAVE(_Skybox)
//        REG_PARENT    ( _TransformNode );
//    REG_PROP    ( _color, TopColor,        GetTopColor,        SetTopColor        );
//    REG_PROP    ( _color, BottomColor,    GetBottomColor,        SetBottomColor    );
//    ENDSAVE
//}; // class _Skybox




#endif // __SGSKYBOX_H__
