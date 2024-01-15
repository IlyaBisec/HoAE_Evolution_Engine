#include "stdheader.h"
void ProcessDialogsEditor();
void RegisterAllEditors(){
	Enumerator* E=ENUM.Get("ALLEDITORS");
	E->Add("Dialogs Editor",(DWORD)&ProcessDialogsEditor);
	E->Add("Dialogs Editor",(DWORD)&ProcessDialogsEditor);
}
///void SelectEditor(){
	/*
	DialogsSystem DSS;
	if(DSS.ReadFromFile("Dialogs\\AllDialogs.DialogsSystem.xml")){
		int x0,y0,x1,y1;
		DSS.GetDialogsFrame(x0,y0,x1,y1);
		if(x1>x0){
			DSS.x=(RealLx-x1+x0)/2;
			DSS.y=(RealLy-y1+y0)/2;
			DialogsDesk* DD=dynamic_cast<DialogsDesk*>(DSS.Find("DD"));
			if(DD){
				ClassArray<TextButton*> DTB;
				TextButton* TB=dynamic_cast<TextButton*>(DSS.Find("Sample"));
				DTB
				if(TB){
					Enumerator* E=ENUM.Get("ALLEDITORS");                    
				}
			}
		}
	}
	*/
//}