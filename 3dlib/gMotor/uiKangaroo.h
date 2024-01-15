/*****************************************************************************/
/*    File:    uiKangaroo.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    30 Dec 2004
/*****************************************************************************/
#ifndef __UIKANGAROO_H__
#define __UIKANGAROO_H__

#include "uiAlgoDebugger.h"
#include "uiModelEditor.h"
#include "uiSceneEditor.h"
#include "uiEffectEditor.h"
#include "uiSystemExplorer.h"
#include "uiPhysicsEditor.h"

/*****************************************************************************/
/*    Class:    Kangaroo
/*    Desc:    Kangaroo editor 
/*****************************************************************************/    
class Kangaroo : public FrameWindow
{
public:

    ModelEditor             m_ModelEditor;
    SceneEditor             m_SceneEditor;
    SystemExplorer          m_SystemExplorer;
    AlgorithmDebugger       m_Debugger;
    WidgetEditor            m_WidgetEditor;
    EffectEditor            m_EffectEditor;
	PhysicsEditor			m_PhysicsEditor;
    Window                  m_Menu;
    Window                  m_Stat;

    Window*                 m_pCharEditor;

    float                   m_FPS;

    std::vector<Widget*>    m_Editors;


                            Kangaroo                ();
    virtual void            Render                  ();
    virtual void            Expose                  ( PropertyMap& pm );
    virtual void            OnInit                  ();

    virtual bool            OnKeyDown                ( DWORD keyCode, DWORD flags );    
    
    void                    ActivateModelEditor     () { HideEditors(); m_ModelEditor.SetVisible();   HideMainMenu(); }
    void                    ActivateEffectEditor    () { HideEditors(); m_EffectEditor.SetVisible();  HideMainMenu(); }
    void                    ActivateWidgetEditor    () { HideEditors(); m_WidgetEditor.SetVisible();  HideMainMenu(); }
    void                    ActivateSceneEditor     () { HideEditors(); m_PhysicsEditor.SetVisible();   HideMainMenu(); }
    void                    ActivateSystemExplorer  () { HideEditors(); m_SystemExplorer.SetVisible();HideMainMenu(); }
    void                    ActivateDebugger        () { HideEditors(); m_Debugger.SetVisible();      HideMainMenu(); }
	void                    ActivatePhysics         () { HideEditors(); m_PhysicsEditor.SetVisible(); HideMainMenu(); }
    void                    ActivateCharEditor      () 
    { 
        if (!m_pCharEditor) return;
        HideEditors(); m_pCharEditor->SetVisible();      
        HideMainMenu();
    }

    void                    Exit                    ();
    int                     GetFPS                  () const { return m_FPS; }
    static Kangaroo&        instance                ()
    {
        static Kangaroo me;
        return me;
    }

    DECLARE_CLASS(Kangaroo);

private:
    void                    HideEditors             ();
    void                    HideMainMenu            () { m_Menu.SetVisible( false ); }
    void                    ToggleMainMenu            () { m_Menu.SetVisible( !m_Menu.IsVisible() ); }
    void                    ReloadResources         ();
    void                    EditOptions             ();

}; // class Kangaroo

#endif // __UIKANGAROO_H__