#pragma once
#ifndef __InterfSynchro__
#define __InterfSynchro__

#include "MapTemplates.h"
//////////////////////////////////////////////////////////////////////////
DIALOGS_API void CmdSendPureData(byte* Data,int size);
//
class DIALOGS_API vui_InterfSynchro{
public:
	static Enumerator* E;
	virtual void InterfAction() = 0;
	virtual void SynchroAction() = 0;	
	virtual int SynchroAction(char* data)=0;
	static int Perform(char* Data);
};
template <class T> class vui_IS_Base: public vui_InterfSynchro{
public:
	static int Id;
	static int Size;
	static int Shift;
	void InterfAction(){
		word* D=(word*)malloc(Size+2); D[0]=Id; memcpy(D+1,(byte*)this+Shift,Size);
		CmdSendPureData((byte*)D,Size+2);
		free(D);
	};
	int SynchroAction(char* data){
		memcpy((byte*)this+Shift,data,Size);
		SynchroAction();
		return Size;
	};
	virtual void SynchroAction()=0;
};
template <class T> int vui_IS_Base<T>::Id		= 0;
template <class T> int vui_IS_Base<T>::Size		= 0;
template <class T> int vui_IS_Base<T>::Shift	= 0;
template <class T> inline void reg_v_InterfSynchro(char* Name){
	T* x=new T;
	Enumerator* E=vui_InterfSynchro::E;
	x->Id=E->GetAmount();
	x->Size=sizeof(x->Data);
	x->Shift=(byte*)&x->Data-(byte*)x;
	E->Add(Name,(DWORD)x);
};

#endif __InterfSynchro__