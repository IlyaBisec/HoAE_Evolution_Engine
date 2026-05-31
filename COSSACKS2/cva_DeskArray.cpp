#include "stdheader.h"
//#include <more_types.h>
//#include "MapTemplates.h"
//#include "vui_Action.h"
#include "vui_Actions.h"
#include "cva_DeskArray.h"
//////////////////////////////////////////////////////////////////////////
bool cva_DeskAction::isExpandableFor(SimpleDialog* SD){
	return dynamic_cast<DialogsDesk*>(SD)!=NULL;
};
//
void cva_DeskArray::SetFrameState(SimpleDialog* SD){
	DialogsDesk* DD=(DialogsDesk*)SD;
	if(DD->DSS.GetAmount()==0) return;
	SimpleDialog* uTemp=DD->DSS[0];
	int uN=GetNUnit();
	for(int i=DD->DSS.GetAmount();i<uN;i++){
		SimpleDialog* uNew=(SimpleDialog*)uTemp->new_element();
		uTemp->Copy(uNew);
		DD->DSS.Add(uNew);
	}
	for(i=0;i<uN;i++){
		SimpleDialog* u=DD->DSS[i];
		SetUnit(i,u);
		if(u){
			u->Visible=true;
		}
	}
	for(;i<DD->DSS.GetAmount();i++){
		SimpleDialog* u=DD->DSS[i];
		if(u){
			u->Visible=false;
		}
	}
};
//
