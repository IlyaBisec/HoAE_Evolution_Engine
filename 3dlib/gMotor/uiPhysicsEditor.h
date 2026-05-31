/*****************************************************************************/
/*    File:    uiPhysicsEditor.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    05-23-2004
/*****************************************************************************/
#ifndef __UIPHYSICSEDITOR_H__
#define __UIPHYSICSEDITOR_H__


/*****************************************************************************/
/*    Class:    PhysicsEditor    
/*    Desc:    Editor of the physics objects
/*****************************************************************************/
class PhysicsEditor : public Window
{
public:
                            PhysicsEditor    ();
    virtual void            Expose            ( PropertyMap& pm );
    virtual void            Render            ();

    void                    Play            ();
    void                    Stop            ();
    void                    Reset            ();
    void                    Load            ();
    bool                    Reload            ();
    void                    Save            ();
    void                    SaveAs            ();
    SNode*                    GetPhysicsRoot    ();

    virtual bool            OnChar            ( DWORD charCode, DWORD flags );

    Group*                    GetPalette        () { return m_pPalette; }
    
    DECLARE_SCLASS(PhysicsEditor,Window,PHED);

protected:
    Group*                    m_pPalette;    
    SNode*                    m_pBackScene;    //  background scene used for convenience    
	NodeTree                  m_Palette;
}; // class PhysicsEditor



#endif // __UIPHYSICSEDITOR_H__