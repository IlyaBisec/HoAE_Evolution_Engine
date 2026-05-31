/*****************************************************************************/
/*    File:    uiKangaroo.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"

#include "uiWidget.h"
#include "uiWindow.h"
#include "uiFrameWindow.h"
#include "uiModelEditor.h"
#include "uiSceneEditor.h"
#include "uiWidgetEditor.h"
#include "uiMainFrame.h"

#include "uiKangaroo.h"

#include "sg.h"
#include "sgRoot.h"
#include "gmDefines.h"

//  this is the root of the interface system
IInputDispatcher*   IInput = &MainFrame::instance();
void RenderUI()
{
    BEGIN_ONCE
        MainFrame::instance().AddChild(Kangaroo::instance());
    MainFrame::instance().Init();
    END_ONCE
        MainFrame::instance().Draw();
}
/*****************************************************************************/
/*  Kangaroo implementation
/*****************************************************************************/
IMPLEMENT_CLASS(Kangaroo);
Kangaroo::Kangaroo()
{
    AlignToViewport();

    AddChild( m_ModelEditor     );
    AddChild( m_Debugger        );
    AddChild( m_SceneEditor     );
    AddChild( m_SystemExplorer  );
    AddChild( m_WidgetEditor    );
    AddChild( m_EffectEditor    );
	AddChild( m_PhysicsEditor	);	


    m_pCharEditor = (Window*)ObjectFactory::instance().Create( "CharacterEditor" );
    if (m_pCharEditor) AddChild( m_pCharEditor );

    AddChild( m_Menu            );
    AddChild( m_Stat            );
	
    m_Editors.push_back( &m_ModelEditor     );
    m_Editors.push_back( &m_Debugger        );
    m_Editors.push_back( &m_SceneEditor     );
    m_Editors.push_back( &m_SystemExplorer  );
    m_Editors.push_back( &m_WidgetEditor    );
    m_Editors.push_back( &m_EffectEditor    );
	m_Editors.push_back( &m_PhysicsEditor   );

    if (m_pCharEditor) m_Editors.push_back( m_pCharEditor );

    SetName( "Kangaroo" );
    SetDraggable    ( false );
    SetKeptAligned  ( true );

    m_FPS = 0.0f;
} // Kangaroo::Kangaroo

void Kangaroo::Render()
{
    static Timer s_Timer;
    float sec = s_Timer.seconds(); 
    s_Timer.start();
    m_FPS = 0.0f;
    if (sec > 0.0f) m_FPS = 1.0f / sec;
} // Kangaroo::Render

void Kangaroo::OnInit()
{
    m_Menu.CreateFromScript( "kangaroo_main_menu" );
    m_Stat.CreateFromScript( "kangaroo_statistics" );
    ActivateModelEditor();
} // Kangaroo::OnInit

void Kangaroo::Exit()          
{ 
    exit( 0 ); 
} // Kangaroo::Exit

void Kangaroo::Expose( PropertyMap& pm )
{
    pm.start( "Kangaroo", this );

    pm.p( "FPS", &Kangaroo::GetFPS );
    pm.m( "ActivateModelEditor", &Kangaroo::ActivateModelEditor     );
    pm.m( "ActivateEffectEditor", &Kangaroo::ActivateEffectEditor    );
    pm.m( "ActivateWidgetEditor", &Kangaroo::ActivateWidgetEditor    );
    pm.m( "ActivateSceneEditor", &Kangaroo::ActivateSceneEditor     );
    pm.m( "ActivateSystemExplorer", &Kangaroo::ActivateSystemExplorer  );
    pm.m( "ActivateDebugger", &Kangaroo::ActivateDebugger        );
    pm.m( "ActivateCharEditor", &Kangaroo::ActivateCharEditor      );
	pm.m( "ActivatePhysics", &Kangaroo::ActivatePhysics			);
    
    pm.m( "ReloadResources", &Kangaroo::ReloadResources         );
    pm.m( "EditOptions", &Kangaroo::EditOptions             );

    pm.m( "Exit", &Kangaroo::Exit );

} // Kangaroo::Expose

bool Kangaroo::OnKeyDown( DWORD keyCode, DWORD flags )
{
    if (GetKeyState( VK_CONTROL ) < 0)
    {
        if (keyCode == 'Q') ReloadResources();
    }
    if (GetKeyState( VK_ESCAPE ) < 0)
    {
        ToggleMainMenu();
    }
    return false;
} // Kangaroo::OnKeyDown

void Kangaroo::ReloadResources()
{
    IRM->UpdateResources();
} // Kangaroo::ReloadResources

void Kangaroo::EditOptions()
{

}

void Kangaroo::HideEditors()
{
    for (int i = 0; i < m_Editors.size(); i++) m_Editors[i]->SetVisible( false );
}

