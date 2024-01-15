/*****************************************************************************/
/*    File:    uiPhysicsEditor.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-23-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "uiControl.h"
#include "uiWindow.h"
#include "uiNodeTree.h"


#include "uiFrameWindow.h"
#include "uiModelEditor.h"
#include "uiSceneEditor.h"
#include "uiWidgetEditor.h"
#include "uiMainFrame.h"

#include "uiKangaroo.h"
#include "uiPhysicsEditor.h"


IMPLEMENT_CLASS( PhysicsEditor );

/*****************************************************************************/
/*    PhysicsEditor implementation
/*****************************************************************************/
//Group* CreatePhysicsTemplates();
Group* CreatePhysicsTemplates();
PhysicsEditor::PhysicsEditor()
{
    //m_pPalette = CreatePhysicsTemplates();
    //m_pPalette = new Group();
    //m_pPalette->SetInvisible();

    //m_ClrTop        = 0x2DFFFFFF;
    //m_ClrMdl        = 0x2DD6D3CE;
    //m_ClrBot        = 0x2D848284;
	
} // PhysicsEditor::PhysicsEditor

SNode* PhysicsEditor::GetPhysicsRoot()
{ 
    return NULL;//this; 
}

void PhysicsEditor::Play()
{
} // PhysicsEditor::Play

void PhysicsEditor::Stop()
{
} // PhysicsEditor::Stop

void PhysicsEditor::Reset()
{

}

void PhysicsEditor::Load()
{

}

bool PhysicsEditor::Reload()
{
    return true;
}

void PhysicsEditor::Save()
{

}

void PhysicsEditor::SaveAs()
{

}

void PhysicsEditor::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "PhysicsEditor", this );
    pm.m( "Play", &PhysicsEditor::Play    );
    pm.m( "Stop", &PhysicsEditor::Stop    );
    pm.m( "Reset", &PhysicsEditor::Reset    );
    pm.m( "Load", &PhysicsEditor::Load    );
    //pm.m( "Reload", Reload    );
    pm.m( "Save", &PhysicsEditor::Save    );
    pm.m( "SaveAs", &PhysicsEditor::SaveAs    );
} // PhysicsEditor::Expose

void PhysicsEditor::Render()
{
	static bool init=false;
	if(!init){
		m_Palette.SetRootNode(m_pPalette);
		m_Palette.SetRightHand();
		//m_Palette.SetVisibleRoot( false );
		//m_Palette.SetAcceptOnDrop( false );
		m_Palette.SetDragLeafsOnly( true );
		m_Palette.AlignToViewport();
		init=true;
		AddChild(&m_Palette);
		//AddChild(&Kangaroo::instance().m_ModelEditor);
	}	
	Kangaroo::instance().m_ModelEditor.SetVisible(true);
	//Kangaroo::instance().m_ModelEditor.Render();
	//Kangaroo::instance().m_ModelEditor.m_TreeL.Render();
	//m_Palette.Render();
    if (m_pBackScene) m_pBackScene->Render();
} // PhysicsEditor::Render

bool PhysicsEditor::OnChar( DWORD charCode, DWORD flags )
{
    //if (IsInvisible()) return false;
    //if (charCode == ' ' && m_pEffect) m_pEffect->PlayEffect();
    return false;
} // PhysicsEditor::OnChar


