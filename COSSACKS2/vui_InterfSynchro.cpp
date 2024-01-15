#include "stdheader.h"
//////////////////////////////////////////////////////////////////////////
Enumerator* vui_InterfSynchro::E=NULL;
int vui_InterfSynchro::Perform(char* Data){
	word* D=(word*)Data;
	addrand(D[0]);
	if(D[0]<E->GetAmount()){
		vui_InterfSynchro* A=(vui_InterfSynchro*)E->GetVal(D[0]);
		return A->SynchroAction((char*)(D+1))+2;
	}
	return -1;
};