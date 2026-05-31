// BattleEditor.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Add function declaration in function list for battle editor
//////////////////////////////////////////////////////////////////////////
void	DeclareteDLL_FN(){

	// Clear list before add dll functions in it
	GI::dllfnClearList();

	// Declarete and export Functions
	DeclareteDLL_expFN_Mission();
	DeclareteDLL_expFN_Group();
	DeclareteDLL_expFN_Text();
	DeclareteDLL_expFN_Get();
	DeclareteDLL_expFN_Set();
	DeclareteDLL_expFN_Ability();
	DeclareteDLL_expFN_Node();

	// Declarete and export FSM ACF
	DeclareteDLL_fsm_Guard();
	DeclareteDLL_fsm_Attack();
	DeclareteDLL_fsm_Produce();
	DeclareteDLL_fsm_Dialog();
	DeclareteDLL_fsm_Saboteur();
		
};
//////////////////////////////////////////////////////////////////////////
// DLL interface
//////////////////////////////////////////////////////////////////////////
class DLL_Interface	: public GI
{
public:
	virtual void OnInitAfterMapLoading();
	virtual	void OnEditorStart();
	virtual void OnInitAfterGameLoading();
};
DLL_Interface	DLL_GI;
__declspec(dllexport)	BaseClass* GetInterface(){
	return (BaseClass*)(&DLL_GI);
};
void	DLL_Interface::OnInitAfterMapLoading(){
	// Create dll export function list for use in scrept editor
	DeclareteDLL_FN();
};
void	DLL_Interface::OnEditorStart(){
	// Create dll export function list for use in scrept editor
	DeclareteDLL_FN();
};
void	DLL_Interface::OnInitAfterGameLoading(){
	// Create dll export function list for use in scrept editor
	DeclareteDLL_FN();
}

//////////////////////////////////////////////////////////////////////////































