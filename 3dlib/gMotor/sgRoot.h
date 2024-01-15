/*****************************************************************************/
/*    File:      sgRoot.h
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __SGROOT_H__
#define __SGROOT_H__

class Group;

/*****************************************************************************/
/*    Class:   Root, singleton
/*    Desc:    Main entrance to the scene graph
/*****************************************************************************/
class Root : public SNode, public PSingleton<Root>
{
public:
                                  Root          ();
    virtual void                  Render        ();
    void                          CreateGuts    ();
    
    DECLARE_SCLASS(Root,SNode,ROOT);

protected:
    friend class                  NodePool;
    virtual                       ~Root();

    Group*                        CreateServices            ();            
    Group*                        CreateEditorSceneSetup    ();    
    Group*                        CreateGameSceneSetup      ();    
    Group*                        CreateFrameContainer      ();    
    Group*                        CreateEditors             ();
    Group*                        CreateModelManager        ();
}; // class Root

#endif // __SGROOT_H__