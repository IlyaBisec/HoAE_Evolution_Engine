#include "stdheader.h"
#include "PropertyPanel.h"

DialogsDesk* PropertyPanel::EditDesk=NULL;
DialogsSystem* PropertyPanel::DS=NULL;
BaseClass* PropertyPanel::CurrentClass=NULL;
bool PropertyPanel::Visible=false;

void PropertyPanel::ShowProperty(ReferableBaseClass* BC){
	TestInit();
	Visible=true;
	if(EditDesk){
		if(EditDesk->UseClassEditor.Get()){
			ClassEditorConnector* CC=EditDesk->UseClassEditor.Get();			
			if(CC){
				CC->ClassFieldPath="$root.";
				CC->RootClass.Set(BC);
			}
		}
	}
}
void PropertyPanel::Hide(){
    Visible=false;
}
void PropertyPanel::Show(){
	TestInit();
    Visible=true;
}
void PropertyPanel::Discard(){
	TestInit();
	if(EditDesk){
		if(EditDesk->UseClassEditor.Get()){
			ClassEditorConnector* CC=EditDesk->UseClassEditor.Get();			
			if(CC)CC->RootClass.Set(NULL);
		}
	}
}
bool PropertyPanel::isVisible(){
    return Visible;
}
void PropertyPanel::TestInit(){
	if(!DS){
		DS=new DialogsSystem;
		DS->SafeReadFromFile("Dialogs\\ObjProperty.DialogsSystem.xml");
		EditDesk=(DialogsDesk*)DS->Find("EditDesk");
	}
}
void PropertyPanel::DrawEditorInterface(){
	TestInit();
	if(EditDesk && Visible){
		DS->ProcessDialogs();
	}
}
void PropertyPanel::ClearActivityOfEditor(){
    Visible=false;
}