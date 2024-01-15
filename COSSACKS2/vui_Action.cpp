#include "stdheader.h"
#include "WeaponSystem.h"
#include "vui_Action.h"
#include "cvi_MainMenu.h"
// vui_Action
// vui_ActionArray
void vui_ActionArray::SetFrameState(SimpleDialog* SD){
    int nActions = GetAmount();
	bool Init=SD->vm_Init;
	for(int i=0; i < nActions; i++)
    {
		vui_Action* A=(*this)[i];
		if (A){
			if(!SD->vm_Init){
				A->Init(SD);
				SD->vm_Init=false;
				Init=true;
			}
			A->SetFrameState(SD);
            if(SD->MouseOver&&(A->isLeftClick()||A->isRightClick())) VitCursor=0;
		}
	}
	SD->vm_Init=Init;
	if(SD->Hint&&SD->MouseOver&&SD->HotKey&&!SD->HintWithHotKey){
		_str txt;
		txt=SD->Hint;
		SD->SetHint(&txt,true);
	}
};
bool vui_ActionArray::ForceProperty(){
	return true;
	//return false;
}
const char* vui_ActionArray::GetThisElementView(const char* LocalName){	
	/*
	static char pf_text[1024];	
	strcpy(pf_text,LocalName);
	int n=GetAmount();
	if(n>0){
		sprintf(pf_text+strlen(pf_text)," [%d]",n);
	}
	return pf_text;
	*/
	//
	static _str txt;
	txt.Clear();
	txt.print("%s: ",LocalName);
	int n=GetAmount();
	for(int i=0;i<n&&i<3;i++){
		txt.print("%s, ",(*this)[i]->GetClassName());		
	}
	return txt.pchar();
}
bool vui_ActionArray::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="ActionArray:";
	if(!ClassEditor::CheckIfOpen(this)){
		dest+="\n%$this,  \n$%";
	}
	return true;
}
//
extern bool vIGPanelMode;
//
bool vui_ActionArray::LeftClick(SimpleDialog* SD){	
	bool click=false;
	for(int i=0;i<GetAmount();i++){
		if(v_MainMenu.Cross==1||v_MainMenu.Cross==2||vIGPanelMode) return true;
		vui_Action* A=GetValues()[i];
		if(A&&A->LeftClick(SD)) click=true;		
	}
	return click; 
};
