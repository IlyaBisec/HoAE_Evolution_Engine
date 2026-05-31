#pragma once
#include "ClassEditorDefines.h"

class ClassEditorProcessor{
	BaseClass* ClassToEdit;
	_str xmlName;
	_str EditorName;
	bool COMPLEX;	
	rce_ProcessClassCallback* Process;
	rce_InputCallback* Input;
	DialogsSystem DSS;
	SimpleDialog* HEADER;
	SimpleDialog* OK;
	SimpleDialog* CANCEL;
	SimpleDialog* DESK;
	SimpleDialog* TDESK;
	ClassEditor CE;
	ComplexClassEditor CCE;
	static bool _Lpressed;
	static bool _Rpressed;
	static bool IGN;
public:
	DWORD Options;
	ClassEditorProcessor();
	~ClassEditorProcessor();
    void PreProcess();
	static void ProcessGame();	
	int PostProcess();//returns  0-continue required 1 - ok and exit 2 - cancel and exit
	static void DrawScreen();
	void ChangeSizeAndPos(RECT R);
	RECT  GetSizeAndPos();
	void CreateFromClass(BaseClass* _BC,const char* _EditorName,const char* _XmlName="",DWORD _Options=RCE_DEFAULT,rce_ProcessClassCallback* _Process=NULL,rce_InputCallback* _Input=NULL);
	static void ClearPress(){
		_Lpressed=false;
		_Rpressed=false;
	}
	//example of use:
	//
	//ClassEditorProcessor CEP1;
	//ClassEditorProcessor CEP2;
	//
	//CEP1.CreateFromClass(...);
	//CEP2.CreateFromClass(...);
	//
	//int r;
	//do{
	//    r=0;
	//    CEP1.PreProcess();
	//    CEP2.PreProcess();
	//    ClassEditorProcessor::ProcessGame();
	//    r|=CEP1.PostProcess();
	//    r|=CEP2.PostProcess();
	//    ClassEditorProcessor::DrawScreen();
	//}while(r==0);
	//

};

void ProcessEditor(char* EditorName);
void ReplaceEditor(char* EditorName, BaseClass* pClass);
void AddStdEditor(char* Name,BaseClass* BC,const char* XmlName,DWORD Options,rce_ProcessClassCallback* Process=NULL,rce_InputCallback* Input=NULL,word HotKey=0);
void SimpleEditClass(char* Name,BaseClass* BC,const char* XmlName="",bool Autosave=false);
void EditClass(char* Name,BaseClass* BC,const char* XmlName,DWORD Options,
			   rce_ProcessClassCallback* Process,rce_InputCallback* Input);
#define REG_CLASS_EDITOR(Name,ObjectName,XML_File_Name,Options,ProcessCallbackIfNeed,InputCallBackIfNeed) AddStdEditor(Name,ObjectName,XML_File_Name,Options,ProcessCallbackIfNeed,InputCallBackIfNeed)