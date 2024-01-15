#include "stdheader.h"
#include "unitability.h"
#include "Extensions.h"
#include "CurrentMapOptions.h"
#include "ai_scripts.h"
#include ".\mai_extension.h"
#include ".\cvi_campaign.h"
#include "UnitTypeGroup.h"
#include ".\vui_GlobalHotKey.h"
#include "BrigadeAINeuro.h"
#include ".\cvi_MainMenu.h"
#include ".\cvi_Missions.h"
#include "ua_Item.h"
#include "Camera\Navigator.h"
//
bool EnableClassBackup=false;

class ClassEditorRegistrator:public BaseClass{
public:
	DWORD Options;
    rce_ProcedureToCall* ProcToCall;
	rce_ProcessClassCallback* Process;
	rce_InputCallback* InputCallback;
	BaseClass* ClassToEdit;
	_str xmlName;
	_str EditorName;
	word HotKey;
	ClassEditorRegistrator(){
		Options=0;
		ProcToCall=NULL;
		InputCallback=NULL;
		ClassToEdit=NULL;
		Process=NULL;
		HotKey=0;
	}
};
ClonesArray<ClassEditorRegistrator>* CE_Registrator;
void TestRegistrator(){
	if(!CE_Registrator)CE_Registrator=new ClonesArray<ClassEditorRegistrator>;
}
void FreeRegistrator(){
	if(CE_Registrator)delete(CE_Registrator);
}
void AddStdEditor(char* Name,BaseClass* BC,const char* XmlName,DWORD Options,
				  rce_ProcessClassCallback* Process,rce_InputCallback* Input,word HotKey){
	TestRegistrator();
	ClassEditorRegistrator* CER=new ClassEditorRegistrator;
	CER->EditorName=Name;
	CER->ClassToEdit=BC;
	CER->Process=Process;
	CER->InputCallback=Input;
	CER->Options=Options;
	CER->xmlName=XmlName;
	CER->HotKey=HotKey;
	CE_Registrator->Add(CER);
}
void AddExistingEditor(char* Name,rce_ProcedureToCall* Proc){
	TestRegistrator();
	ClassEditorRegistrator* CER=new ClassEditorRegistrator;
	CER->ProcToCall=Proc;
	CER->EditorName=Name;
	CE_Registrator->Add(CER);
}
void AddExistingEditorEx(char* Name,rce_ProcedureToCall* Proc,word HotKey){
	TestRegistrator();
	ClassEditorRegistrator* CER=new ClassEditorRegistrator;
	CER->ProcToCall=Proc;
	CER->EditorName=Name;
	CER->HotKey=HotKey;
	CE_Registrator->Add(CER);
}
//extern int ItemChoose;
bool MMItemChoose(SimpleDialog* SD);
void ProcessEditor(int Index);
void EditClass(char* Name,BaseClass* BC,const char* XmlName,DWORD Options,
			   rce_ProcessClassCallback* Process,rce_InputCallback* Input){
   ItemChoose=-1;
   AddStdEditor(Name,BC,XmlName,Options,Process,Input,0);
   int N=CE_Registrator->GetAmount();
   if(N>0){
	   ProcessEditor(N-1);
	   CE_Registrator->DelElement(N-1);
   }
}
void DelStdEditor(const char* Name){
    int N=CE_Registrator->GetAmount();
    for(int i=0;i<N;i++){
        if(!strcmp(Name,(*CE_Registrator)[i]->EditorName.cpchar())){
            CE_Registrator->DelElement(i);
            i--;
            N--;
        }
    }
}

void SimpleEditClass(char* Name,BaseClass* BC,const char* XmlName,bool Autosave){
	EditClass(Name,BC,XmlName,
		Autosave?RCE_CENTRAL_POSITION|RCE_EXITONESCAPE|RCE_EXITONENTER:RCE_DEFAULT,
		NULL,NULL);
}
void ProcessEditor(int Index){
	TestRegistrator();
	if(Index<CE_Registrator->GetAmount()){
		ClassEditorRegistrator* CER=(*CE_Registrator)[Index];
		if(CER){
			if(CER->ProcToCall){
				CER->ProcToCall();
			}else{
				ClassEditorProcessor CEP;
				CEP.CreateFromClass(CER->ClassToEdit,CER->EditorName.pchar(),CER->xmlName.pchar(),CER->Options,CER->Process,CER->InputCallback);
				ItemChoose=-1;
				int r=0;
				do{
                    extern bool IgnoreProcessMessages2;
                    IgnoreProcessMessages2=true;
					CEP.PreProcess();
					if(CEP.Options&RCE_ALLOW_GAME_PROCESSING){
						CEP.ProcessGame();
					}
                    IgnoreProcessMessages2=false;
                    IgnoreProcessMessages=false;
                    ProcessMessages();
                    IgnoreProcessMessages2=true;
					if(!g_Navigator.IsPlayingTrack()) {
						r = CEP.PostProcess();
					} else {
						r = false;
					}
                    IgnoreProcessMessages2=false;					
                    if(CEP.Options&RCE_ALLOW_GAME_PROCESSING){
                        void GlobalHandleMouse();
                        GlobalHandleMouse();
                    }
                    CEP.DrawScreen();
				}while(r==0);
			}
			/*
			if(CER->ProcToCall){
                CER->ProcToCall();
			}else{
                DialogsSystem DSS;
				if(DSS.ReadFromFile("Dialogs\\classeditordialog.dialogssystem.xml")){
                    SimpleDialog* HEADER =DSS.Find("HEADER");
					SimpleDialog* OK     =DSS.Find("OK");
					SimpleDialog* CANCEL =DSS.Find("CANCEL");
					SimpleDialog* DESK   =DSS.Find("DESK");
					SimpleDialog* TDESK  =DSS.DSS[0];
					if(DESK){
						if(HEADER){
							if(CER->xmlName.pchar()&&CER->xmlName.pchar()[0]){
								CER->ClassToEdit->reset_class(CER->ClassToEdit);
								CER->ClassToEdit->SafeReadFromFile(CER->xmlName.pchar());
								CER->ClassToEdit->WriteToFile("$$$temp$$$.xml");
							}
							TextButton* TB=dynamic_cast<TextButton*>(HEADER);
							if(TB){
								TB->SetMessage(CER->EditorName.pchar());
							}
							DWORD POPT=CER->Options&(16+32+64);
							if(POPT==RCE_CENTRAL_POSITION){
                                TDESK->x=100;
								TDESK->y=100;
								TDESK->x1=RealLx-100;
								TDESK->y1=RealLy-100;
							}else
							if(POPT==RCE_RIGHT_POSITION){
								TDESK->x=RealLx-450;
								TDESK->y=10;
								TDESK->x1=RealLx;
								TDESK->y1=RealLy-245;
							}else
							if(POPT==RCE_BOTTOM){
								TDESK->x=0;
								TDESK->y=RealLy/2;
								TDESK->x1=RealLx;
								TDESK->y1=RealLy;
							}else
							if(POPT==RCE_FULLSCREEN){
								TDESK->x=0;
								TDESK->y=0;
								TDESK->x1=RealLx;
								TDESK->y1=RealLy;
							}
							DSS.ProcessDialogs();
							bool COMPLEX=(CER->Options&RCE_DOUBLEPANEL21)!=0;
							ClassEditor CE;
							ComplexClassEditor CCE;
							if(COMPLEX){
								int DIV=50;
								switch(CER->Options&RCE_DOUBLEPANEL21){
									case RCE_DOUBLEPANEL21:DIV=33;break;
									case RCE_DOUBLEPANEL12:DIV=66;break;
								}								
								CCE.Create(DESK->LastDrawX,DESK->LastDrawY,
										DESK->LastDrawX+DESK->x1-DESK->x,
										DESK->LastDrawY+DESK->y1-DESK->y,
										DIV,CER->ClassToEdit,"EmptyBorder");
							}else{
								CE.CreateFromClass(DESK,0,0,DESK->x1-DESK->x,DESK->y1-DESK->y,CER->ClassToEdit,3,"EmptyBorder");
							}
							if(CER->Process){
								CER->Process(&CE,CER->ClassToEdit,1);
							}							
							if(OK){
								OK->OnUserClick=&MMItemChoose;
								OK->UserParam=mcmOk;
								CANCEL->OnUserClick=&MMItemChoose;
								CANCEL->UserParam=mcmCancel;
							}
							if(CER->Options&RCE_HIDEOKCANCEL){
								if(OK)OK->Visible=false;								
								if(CANCEL)CANCEL->Visible=false;
								SimpleDialog* SD=DSS.Find("OKDESK");
								if(SD)SD->Visible=0;
								SD=DSS.Find("CANCELDESK");
								if(SD)SD->Visible=0;
							}
							if(CER->Options&RCE_HIDEHEADER){
								if(HEADER)HEADER->Visible=false;								
								SimpleDialog* SD=DSS.Find("HEADERDESK");
								if(SD)SD->Visible=0;
							}
							do{
								if(CER->Process){
									if(CER->Process(CE._EdClass?&CE:&CCE.Top,CER->ClassToEdit,2))ItemChoose=mcmOk;
								}
								if(CER->Options&RCE_SHOW_GAME_BACKGROUND){
									void ClearModes();
									//ClearModes();
									bool _Lpressed=Lpressed;
									bool _Rpressed=Rpressed;
									extern bool LockMouse;
									bool IGN=0;
									extern bool IgnoreProcessMessages;
									if(mouseX>TDESK->x&&mouseX<TDESK->x1&&mouseY>TDESK->y&&mouseY<TDESK->y1){
										Lpressed=false;
										Rpressed=false;
										void UnPress();
										//UnPress();										
										IgnoreProcessMessages=true;
										LockMouse=true;
										IGN=1;
									}
									if(CER->Options&RCE_ALLOW_GAME_PROCESSING){
										void PreDrawGameProcess();
										PRFUNC(PreDrawGameProcess());
									}
									void ProcessScreen();
									ProcessScreen();
									if(CER->Options&RCE_ALLOW_GAME_PROCESSING){
										void PostDrawGameProcess();
										PostDrawGameProcess();
									}
									if(IGN){
										Lpressed=_Lpressed;
										Rpressed=_Rpressed;
										IgnoreProcessMessages=false;
										LockMouse=false;
									}
									if(!IGN){
										if(CER->InputCallback){
											if(CER->InputCallback(mouseX,mouseY,Lpressed,Rpressed)){
												void UnPress();
												UnPress();
											}
										}
									}
									ProcessMessages();					
									void StdKeys();
									StdKeys();									
									DSS.ProcessDialogs();

									if(COMPLEX)CCE.Process();
									else CE.Process();

									void GSYSDRAW();
									GSYSDRAW();
								}else{
									ProcessMessages();					
									void StdKeys();
									StdKeys();
									DSS.ProcessDialogs();

									if(COMPLEX)CCE.Process();
									else CE.Process();

									DSS.RefreshView();									
								}						
								if(CER->Options&RCE_AUTOSAVE){
									static int T0=GetTickCount();
									if(GetTickCount()-T0>2000){
										if(CER->xmlName.pchar()&&CER->xmlName.pchar()[0]){
											CER->ClassToEdit->WriteToFile(CER->xmlName.pchar());
										}else{
											if(CER->EditorName.pchar()&&strstr(CER->EditorName.pchar()," AI "))
												CER->ClassToEdit->WriteToFile("ai\\autosave.ai.xml");
											///else CER->ClassToEdit->WriteToFile("autosave.xml");
										}
										T0=GetTickCount();
									}
								}
							}while(ItemChoose==-1);							
							if(CER->xmlName.pchar()&&CER->xmlName.pchar()[0]){
								if(ItemChoose==mcmCancel){
									CER->ClassToEdit->reset_class(CER->ClassToEdit);
									CER->ClassToEdit->SafeReadFromFile("$$$temp$$$.xml");								
								}
								CER->ClassToEdit->WriteToFile(CER->xmlName.pchar());
							}
							if(CER->Process&&ItemChoose==mcmOk){
								CER->Process(&CE,CER->ClassToEdit,3);
							}
							if(CER->Process&&ItemChoose==mcmCancel){
								CER->Process(&CE,CER->ClassToEdit,4);
							}
						}
					}
				}
			}
			*/
		}
	}
	ItemChoose=-1;
}
void ProcessEditor(char* EditorName){
	if (EditorName!=NULL) {
		int N = CE_Registrator->GetAmount();
		ClassEditorRegistrator* CER=NULL;
		while (N--) {
			CER = (*CE_Registrator)[N];
			if (strcmp(CER->EditorName.pchar(),EditorName)==0){
				ProcessEditor(N);
				N=0;
			};
		};
	};
};
void ReplaceEditor(char* EditorName, BaseClass* pClass){
	if (pClass!=NULL&&EditorName!=NULL){
		int N = CE_Registrator->GetAmount();
		ClassEditorRegistrator* CER=NULL;
		while (N--) {
			CER = (*CE_Registrator)[N];
			if (strcmp(CER->EditorName.pchar(),EditorName)==0 && CER->ClassToEdit!=pClass) {
				CER->ClassToEdit = pClass;
				N=0;
			};
		};
	};
};
//
char* GetKeyName(word Key);
//extern int ItemChoose;
bool MMItemChoose(SimpleDialog* SD);
bool InAllEditors=false;
//
void SelectEditor(){
	TestRegistrator();
	xmlQuote xml;
	ItemChoose=-1;
	if(xml.ReadFromFile("Dialogs\\alleditors.DialogsSystem.xml")){
		DialogsSystem DSS;
		ErrorPager EP(0);
		DSS.Load(xml,&DSS,&EP);
		ListDesk* Desk=(ListDesk*)DSS.Find("LIST");
		if(Desk){
			int x0,y0,x1,y1;
			DSS.GetDialogsFrame(x0,y0,x1,y1);
			if(x1>x0){
				DSS.x=(RealLx-x1+x0)/2;
				DSS.y=(RealLy-y1+y0)/2;
				_str name;
				int DECODER[1024];
				int r=0;
				for(int i=0;i<CE_Registrator->GetAmount();i++){
					ClassEditorRegistrator* CER=(*CE_Registrator)[i];
					if(!(CER->Options&RCE_INVISIBLE)){
						DECODER[r]=i;
						r++;
						name.Clear();
						if(CER->HotKey){
							name.print("%s ({CR}%s{C})",CER->EditorName.pchar(),GetKeyName(CER->HotKey));
						}else{
							name.print("%s",CER->EditorName.pchar());					
						}		
						Desk->AddElement(name.pchar(),CER->HotKey);
					}
				}
				Desk->CurrentElement=-1;
				InAllEditors=true;
				do{
					ProcessMessages();
					void StdKeys();
					StdKeys();
					DSS.ProcessDialogs();					
					DSS.RefreshView();
					if(Desk->CurrentElement>=0){
						void UnPress();
						UnPress();
						extern bool realLpressed;
						realLpressed=0;
						//try{
							ProcessEditor(DECODER[Desk->CurrentElement]);
						//}catch(...){};
						InAllEditors=false;
						return;
					}
				}while(ItemChoose==-1);
			}
		}
	}
}

void ClearSurfTool();
//-----------------------------------------------------------------------------
// ActivateSurfTool
//-----------------------------------------------------------------------------
bool ActivateSurfTool(const char *pToolName) {
	ClearSurfTool();
	if(pToolName) {
		typedef DynArray<GameExtension *> ExtRef;
		extern ExtRef ExtReferences[MAXVFUNC];
		ExtRef *ER = &ExtReferences[21];
		for(int i = 0; i < ER->NValues; i++){
			char *pStr = (*ER)[i]->ExtensionName.pchar();
			if(strcmp(pStr, pToolName) == 0) {
				(*ER)[i]->ActivateEditor();
				return true;
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// IsActiveSurfTool
//-----------------------------------------------------------------------------
bool IsActiveSurfTool(const char *pToolName) {
	if(pToolName) {
		typedef DynArray<GameExtension *> ExtRef;
		extern ExtRef ExtReferences[MAXVFUNC];
		ExtRef *ER = &ExtReferences[21];
		for(int i = 0; i < ER->NValues; i++){
			char *pStr = (*ER)[i]->ExtensionName.pchar();
			if(strcmp(pStr, pToolName) == 0) {
				return (*ER)[i]->CheckActivityOfEditor();
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// SelectSurfaceTool
//-----------------------------------------------------------------------------
/*void SelectSurfaceTool() {
	ClearModes();
	TestRegistrator();
	xmlQuote xml;
	ItemChoose=-1;
	if(xml.ReadFromFile("Dialogs\\SurfaceTools.DialogsSystem.xml")){
		DialogsSystem DSS;
		ErrorPager EP(0);
		DSS.Load(xml,&DSS,&EP);
		int H = RealLy - 40;
		ListDesk *pList = (ListDesk *)DSS.Find("LIST");
		assert(pList);
		DialogsDesk *pDesk = (DialogsDesk *)DSS.Find("DESK");
		assert(pDesk);
		pDesk->SetHeight(H);
		if(pList){
			int x0,y0,x1,y1;
			DSS.GetDialogsFrame(x0,y0,x1,y1);
			if(x1>x0){
				DSS.x=(RealLx-x1+x0)/2;
				DSS.y=(RealLy-y1+y0)/2;
				_str name;
				ext_CheckActivityOfEditor();
				typedef ClassArray<GameExtension> ExtScope;
				typedef DynArray<GameExtension*> ExtRef;
				//
				extern ExtRef ExtReferences[MAXVFUNC];
				extern ExtScope* ExtList;
				ExtRef* ER=&ExtReferences[21];
				int N=ER->GetAmount();				
				for(int i=0;i<N;i++){		
					char* s=(*ER)[i]->ExtensionName.pchar();
					if(!s)s="???";
					if(s){
						name=s;
					}
					if(strcmp(name.pchar(), "Colorize")) {
						pList->AddElement(name.pchar(), 'C');
					} else pList->AddElement(name.pchar(),0);
				}
				pList->CurrentElement=-1;
				do{
					ProcessMessages();
					void StdKeys();
					StdKeys();
					DSS.ProcessDialogs();					
					DSS.RefreshView();
					if(pList->CurrentElement>=0){
						GameExtension* x=(*ER)[pList->CurrentElement];
						void UnPress();
						UnPress();
						extern bool realLpressed;
						realLpressed=0;						
						x->ActivateEditor();						
						return;
					}
				}while(ItemChoose==-1);
			}
		}
	}
}*/

#include "Surface\Surface.h"

//-----------------------------------------------------------------------------------
// SurfaceEditorProcessClassCallback
//-----------------------------------------------------------------------------------
bool SurfaceEditorProcessClassCallback(ClassEditor *CE, BaseClass *BC, int Options) {
	Surface::SurfEditor *pSurfEditor = (Surface::SurfEditor*)BC;
	assert(pSurfEditor);
	if(1 == Options) {
		pSurfEditor->m_fActive = true;
		return false;
	} else if(3 == Options || 4 == Options) {
		pSurfEditor->m_fActive = false;
	} else if(!pSurfEditor->m_fActive) {
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// ShowSurfEditor
//-----------------------------------------------------------------------------
void ShowSurfEditor() {
	ClearSurfTool();
	DWORD Flags = RCE_SHOW_GAME_BACKGROUND | RCE_ALLOW_GAME_PROCESSING | RCE_RIGHT_POSITION;// | RCE_CENTRAL_POSITION;
	EditClass("SurfEditor", (BaseClass*)(&g_SurfEditor), NULL, Flags,
		&SurfaceEditorProcessClassCallback, NULL);
	g_SurfEditor.WriteToFile("\Editor.xml");
}

//-----------------------------------------------------------------------------------
// MapOptionsEditorProcessClassCallback
//-----------------------------------------------------------------------------------
bool MapOptionsEditorProcessClassCallback(ClassEditor *CE, BaseClass *BC, int Options) {
	MapOptions *pMapOptions = (MapOptions*)BC;
	assert(pMapOptions);
	if(1 == Options) {
		pMapOptions->IsActive = true;
		return false;
	} else if(3 == Options || 4 == Options) {
		pMapOptions->IsActive = false;
	} else if(!pMapOptions->IsActive) {
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// ShowMapOptions
//-----------------------------------------------------------------------------
void ShowMapOptions() {
	if(!MOptions.IsActive) {
		DWORD Flags = RCE_SHOW_GAME_BACKGROUND | RCE_ALLOW_GAME_PROCESSING | RCE_RIGHT_POSITION;
		EditClass("Map Options", (BaseClass*)(&MOptions), NULL, Flags, &MapOptionsEditorProcessClassCallback, NULL);
	}
}

ClassEditorProcessor::ClassEditorProcessor(){
	ClassToEdit=NULL;
	xmlName="";	
	Process=NULL;
	Input=NULL;
	HEADER=NULL;
	OK=NULL;
	CANCEL=NULL;
	DESK=NULL;
	TDESK=NULL;
	IGN=false;
}
ClassEditorProcessor::~ClassEditorProcessor(){
}
bool ClassEditorProcessor::_Lpressed=false;
bool ClassEditorProcessor::_Rpressed=false;
bool ClassEditorProcessor::IGN=0;

int LastAutosaveTime=0;
void ClassEditorProcessor::PreProcess(){
	if(ClassToEdit){
        if(Process){
			if(Process(CE._EdClass?&CE:&CCE.Top,ClassToEdit,2))ItemChoose=mcmOk;
		}
		if(!IGN){
			void ClearModes();
			//ClearModes();
			_Lpressed=Lpressed;
			_Rpressed=Rpressed;
			extern bool LockMouse;
			IGN=0;
			extern bool IgnoreProcessMessages;
			if( TDESK && mouseX>TDESK->x&&mouseX<TDESK->x1&&mouseY>TDESK->y&&mouseY<TDESK->y1 ){
				Lpressed=false;
				Rpressed=false;
				void UnPress();
				//UnPress();										
				IgnoreProcessMessages=true;
				LockMouse=true;
				IGN=1;
			}
		}
	}
}
int ClassEditorProcessor::PostProcess(){
	if(ClassToEdit){
		if(IGN){
			Lpressed=_Lpressed;
			Rpressed=_Rpressed;
			IgnoreProcessMessages=false;
			extern bool LockMouse;
			LockMouse=false;
		}else{
			if(Input){
				if(Input(mouseX,mouseY,Lpressed,Rpressed)){
					void UnPress();
					UnPress();
				}
			}
		}
		IGN=false;
		ProcessMessages();					
		void StdKeys();
		StdKeys();									
		DSS.ProcessDialogs();

		if(COMPLEX)CCE.Process();
		else CE.Process();
		if(Options&RCE_AUTOSAVE){			
			if(GetTickCount()-LastAutosaveTime>4*60000){
				if(xmlName.pchar()&&xmlName.pchar()[0]){
					ClassToEdit->WriteToFile(xmlName.pchar());
				}else{
					if(EditorName.pchar()&&strstr(EditorName.pchar()," AI "))
						ClassToEdit->WriteToFile("Temp\\autosave.ai.xml");
					///else CER->ClassToEdit->WriteToFile("autosave.xml");
				}
				LastAutosaveTime=GetTickCount();
			}
		}
		if(ItemChoose!=-1){
			if(xmlName.pchar()&&xmlName.pchar()[0]){
				if(ItemChoose==mcmCancel){
					if(EnableClassBackup){
						ClassToEdit->reset_class(ClassToEdit);
						ClassToEdit->SafeReadFromFile("Temp\\$$$temp$$$.xml");								
					}
				}else ClassToEdit->WriteToFile(xmlName.pchar());
			}
			if(Process&&ItemChoose==mcmOk){
				Process(&CE,ClassToEdit,3);
			}
			if(Process&&ItemChoose==mcmCancel){
				Process(&CE,ClassToEdit,4);
			}
			return ItemChoose==mcmOk?1:2;
		}
		return 0;
	}
	return 2;
}
void ClassEditorProcessor::ChangeSizeAndPos(RECT R){
	if(TDESK){
		TDESK->x=R.left;
		TDESK->y=R.top;
		TDESK->x1=R.right;
		TDESK->y1=R.bottom;
	}
}
RECT ClassEditorProcessor::GetSizeAndPos(){
	RECT R;
	R.left=TDESK->x;
	R.top=TDESK->y;
	R.right=TDESK->x1;
	R.bottom=TDESK->y1;
    return R;	
}
void ClassEditorProcessor::DrawScreen(){
	void GSYSDRAW();
	GSYSDRAW();
}
void ClassEditorProcessor::ProcessGame(){
	void PreDrawGameProcess();
	PRFUNC(PreDrawGameProcess());	
	void ProcessScreenWithoutMouse();
    ProcessScreenWithoutMouse();
	GPS.FlushBatches();
	void PostDrawGameProcess();
	PostDrawGameProcess();
}

void ClassEditorProcessor::CreateFromClass(BaseClass* ClassToEdit,const char* _EditorName
										   ,const char* _XmlName
										   ,DWORD _Options
										   ,rce_ProcessClassCallback* _Process
										   ,rce_InputCallback* _Input){
	EditorName=_EditorName;
	xmlName=_XmlName;
	Options=_Options;
	Process=_Process;
	Input=_Input;
	ClassEditorProcessor::ClassToEdit=ClassToEdit;
	LastAutosaveTime=GetTickCount();
    
	if(DSS.ReadFromFile("Dialogs\\classeditordialog.dialogssystem.xml")){
        HEADER =DSS.Find("HEADER");
		OK     =DSS.Find("OKDESK");		
		CANCEL =DSS.Find("CANCELDESK");
		DESK   =DSS.Find("DESK");
		TDESK  =DSS.DSS[0];

		if(DESK){
			if(HEADER){
				if(xmlName.pchar()&&xmlName.pchar()[0]){
					ClassToEdit->reset_class(ClassToEdit);
					ClassToEdit->SafeReadFromFile(xmlName.pchar());
					if(EnableClassBackup){
						ClassToEdit->WriteToFile("Temp\\$$$temp$$$.xml");
					}
				}
				TextButton* TB=dynamic_cast<TextButton*>(HEADER);
				if(TB){
					TB->SetMessage(EditorName.pchar());
				}
				DWORD POPT=Options&(16+32+64);
				if(POPT==RCE_CENTRAL_POSITION){
                    TDESK->x=5;
					TDESK->y=5;
					TDESK->x1=RealLx-5;
					TDESK->y1=RealLy-60;
				}else
				if(POPT==RCE_RIGHT_POSITION){
					TDESK->x=RealLx-520;
					TDESK->y=10;
					TDESK->x1=RealLx;
					TDESK->y1=RealLy-245;
				}else
				if(POPT==RCE_BOTTOM){
					TDESK->x=0;
					TDESK->y=RealLy/2;
					TDESK->x1=RealLx;
					TDESK->y1=RealLy;
				}else
				if(POPT==RCE_FULLSCREEN){
					TDESK->x=0;
					TDESK->y=0;
					TDESK->x1=RealLx;
					TDESK->y1=RealLy;
				}
				for(int i=0;i<5;i++){
					DSS.ProcessDialogs();					
				}				
				COMPLEX=(Options&RCE_DOUBLEPANEL21)!=0;
				if(COMPLEX){
					int DIV=50;
					switch(Options&RCE_DOUBLEPANEL21){
						case RCE_DOUBLEPANEL21:DIV=33;break;
						case RCE_DOUBLEPANEL12:DIV=66;break;
					}								
					CCE.Create(DESK->LastDrawX,DESK->LastDrawY,
							DESK->LastDrawX+DESK->x1-DESK->x,
							DESK->LastDrawY+DESK->y1-DESK->y,
							DIV,ClassToEdit,"EmptyBorder");
				}else{
					CE.CreateFromClass(DESK,0,0,DESK->x1-DESK->x,DESK->y1-DESK->y,ClassToEdit,3,"EmptyBorder");
				}
				if(Process){
					Process(&CE,ClassToEdit,1);
				}							
				if(OK){
					OK->OnUserClick=&MMItemChoose;
					OK->UserParam=mcmOk;
					CANCEL->OnUserClick=&MMItemChoose;
					CANCEL->UserParam=mcmCancel;
				}
				if(Options&RCE_HIDEOKCANCEL){
					if(OK)OK->Visible=false;								
					if(CANCEL)CANCEL->Visible=false;
					SimpleDialog* SD=DSS.Find("OKDESK");
					if(SD)SD->Visible=0;
					SD=DSS.Find("CANCELDESK");
					if(SD)SD->Visible=0;
				}
				if(Options&RCE_HIDEHEADER){
					if(HEADER)HEADER->Visible=false;								
					SimpleDialog* SD=DSS.Find("HEADERDESK");
					if(SD)SD->Visible=0;
				}				
			}
		}
	}
}

extern MapOptions MOptions;
extern ClassArray<ActiveZone> AZones;
extern ScriptsScope GScripts;
bool AIScripsEditorProcess(ClassEditor* CE,BaseClass* BC,int Options);
void ProcessAIEditor();
void ProcessDialogsEditor();
void ProcessDialogsEditor();
void SummonKangaroo();
void PaintMapWith();
void ExPaintMapWith();
void EditMapOptions();
void EditScripts();
void CREATE_SCRIPT(); // grey [17.03.2004]
BaseClass* GetWallsClass();
BaseClass* GetSoundClass();
BaseClass* GetWeaponClass();
bool ProcessWeaponClass(ClassEditor* CE,BaseClass* BC,int Options);
bool ProcessAbilityClass(ClassEditor* CE,BaseClass* BC,int Options);
BaseClass* GetAbilityClass();
void RegProfView();
void RegTestClass();	
void RegisterUnitlistEditor();
void	Add_Class_To_Main_Editor(DWORD _rce_,DWORD _DILOG_EDITOR_);
extern GameSettings GSets;
extern FontParam FParam;
extern TextIcons TIcons;
extern UnitTypeGroup UnitTypeGroups;
extern CNeuroStorage NeuroStorage;

//
DialogsSystem tDS;
void InsertThereRegistrationOfDifferentEditors(){		
	
	AddStdEditor("Items CookBook",&CookBook,"dialogs\\CookBook.xml",RCE_DEFAULT,NULL,NULL,'C');
		
	AddStdEditor("National GP Convertor",&NationalGP,"dialogs\\nationalgp.xml",RCE_DEFAULT,NULL,NULL,'N');

	void AddCurrentUnitEditor();
	AddCurrentUnitEditor();
	AddStdEditor("EW2 Tutorial",&EW2_Missions,EW2_MissionsXML,RCE_DEFAULT);
	AddStdEditor("Interface System",&v_ISys,v_ISysXML,RCE_DEFAULT);
	AddStdEditor("Hot Keys",&v_GlobalHotKeys,v_GlobalHotKeysXML,RCE_DEFAULT);
	AddStdEditor("vm_Campaigns",&vmCampaigns,vmCampaignXML,RCE_DEFAULT);	
	
	//AddStdEditor("temp DialogsSystem",&tDS,"",
	//	RCE_RIGHT_POSITION|RCE_HIDEHEADER|
	//	RCE_SHOW_GAME_BACKGROUND|RCE_ALLOW_GAME_PROCESSING|RCE_AUTOSAVE|
	//	RCE_EXITONESCAPE|
	//	RCE_DOUBLEPANEL12);
	
	REG_EXISTING_EDITOR("Kangaroo",SummonKangaroo);
	REG_EXISTING_EDITOR_Ex("Dialogs",ProcessDialogsEditor,'D');	
	AddStdEditor("Borders",&BScope,"dialogs\\borders.xml",RCE_DEFAULT,NULL,NULL,'O');
	void RegShowLeaks();
	RegShowLeaks();
	//REG_EXISTING_EDITOR("AI_Editor",ProcessAIEditor);	
	AddStdEditor("Font Styles",&v_FontStyle,v_FontStyleXML,RCE_DEFAULT3);
	AddStdEditor("Fonts",&FParam,"dialogs\\fonts.xml",RCE_DEFAULT3);
	extern  TextIcons TIconsTemp;
	AddStdEditor("Icons",&TIconsTemp,"",RCE_DEFAULT3);
	
	extern DialogsSystem vdsWeap;
	AddStdEditor("SP Weapon",&vdsWeap,"",RCE_DEFAULT3|RCE_DOUBLEPANEL12);	
	
	//AddStdEditor("mai Monitor",&vaiMonitor,"",RCE_DEFAULT3,NULL,NULL,'M');
	//extern ClonesArray<mai_VanGuard> mai_VanGuards;
	//AddStdEditor("mai VanGuards",&mai_VanGuards,"",RCE_DEFAULT3|RCE_DOUBLEPANEL12,NULL,NULL,'V');
	
	AddStdEditor("Started AI script",&GScripts,"",
		RCE_HIDEHEADER|RCE_HIDEOKCANCEL|
		RCE_FULLSCREEN|
		//RCE_CENTRAL_POSITION|
		//RCE_RIGHT_POSITION|
		//RCE_SHOW_GAME_BACKGROUND|
		//RCE_ALLOW_GAME_PROCESSING|
		RCE_AUTOSAVE|
		RCE_EXITONESCAPE|
		RCE_DOUBLEPANEL12,AIScripsEditorProcess,NULL,'A');

	///REG_EXISTING_EDITOR("TerrainEditor",PaintMapWith);
	AddStdEditor("MapOptions",&MOptions,"",
		RCE_RIGHT_POSITION|RCE_SHOW_GAME_BACKGROUND|
		RCE_ALLOW_GAME_PROCESSING|RCE_AUTOSAVE|
		RCE_EXITONESCAPE|RCE_HIDEHEADER,NULL,NULL,'P');
    AddStdEditor("GameSettings",&GSets,"Settings.xml",
        RCE_RIGHT_POSITION|RCE_SHOW_GAME_BACKGROUND|
        RCE_ALLOW_GAME_PROCESSING|RCE_AUTOSAVE|
        RCE_EXITONESCAPE|RCE_HIDEHEADER,NULL,NULL,'T');
	/*AddStdEditor("EngineSettings",&EngSettings,"EngineSettings.xml",
		RCE_RIGHT_POSITION|RCE_SHOW_GAME_BACKGROUND|
		RCE_ALLOW_GAME_PROCESSING|RCE_AUTOSAVE|
		RCE_EXITONESCAPE|RCE_HIDEHEADER,NULL,NULL,' ');*/
	//LocSettings.SafeReadFromFile("Text\\LocalizationSettings.xml");
	AddStdEditor("LocalizationSettings",&LocSettings,"Text\\LocalizationSettings.xml",
		RCE_RIGHT_POSITION|RCE_SHOW_GAME_BACKGROUND|
		RCE_ALLOW_GAME_PROCESSING|RCE_AUTOSAVE|
		RCE_EXITONESCAPE|RCE_HIDEHEADER,NULL,NULL,' ');	
	REG_EXISTING_EDITOR("ExTerrainEditor",ExPaintMapWith);	
	void UserFriendlyMapGenerator();
	REG_EXISTING_EDITOR("UF_Landscape",UserFriendlyMapGenerator);	
	void RegisterSurfaceEditor();
	RegisterSurfaceEditor();
	void AddLayersEditor();
	AddLayersEditor();
	void RegTreesExchangeEditor();
	RegTreesExchangeEditor();
	void AddRandomMapEditor();
	AddRandomMapEditor();
	void RegAI_router();
	RegAI_router();
	//REG_EXISTING_EDITOR("MapOptionsEditor",EditMapOptions);	
	REG_EXISTING_EDITOR("ScriptsEditor",EditScripts);	
	REG_EXISTING_EDITOR("MissionScriptEditor",CREATE_SCRIPT); // grey [17.03.2004]
	AddStdEditor("WallsEditor",GetWallsClass(),"Dialogs\\Walls.WallsList.xml",RCE_DEFAULT);	
	bool ProcessSoundClass(ClassEditor* CE,BaseClass* BC,int Options);
	AddStdEditor("SoundEditor",GetSoundClass(),"Sound\\Sounds.xml",RCE_DEFAULT,ProcessSoundClass,NULL);	
	//AddStdEditor("WeaponEditor",GetWeaponClass(),"WeaponSystem\\base.ws.xml",RCE_DEFAULT,ProcessWeaponClass,NULL,'W');
	//AddStdEditor("UnitAbilityEditor",GetAbilityClass(),"UnitsAbility\\base.ua.xml",RCE_DEFAULT,ProcessAbilityClass,NULL,'A');	
	void RegAW_Solution();
	RegAW_Solution();
	void RegBinkTextEditor();
	RegBinkTextEditor();
	RegProfView();
	RegTestClass();
	AddStdEditor("Active Zones",&AZones,"",
		RCE_RIGHT_POSITION|RCE_SHOW_GAME_BACKGROUND|
		RCE_ALLOW_GAME_PROCESSING|RCE_AUTOSAVE|
		RCE_EXITONESCAPE|RCE_HIDEHEADER);	
	AddStdEditor("TreesEditor",&TREES,"",RCE_DEFAULT);	
	AddStdEditor("StonesEditor",&STONES,"",RCE_DEFAULT);	
	extern SprGroup WALLS;
	AddStdEditor("AlignedObjectsEditor",&WALLS,"",RCE_DEFAULT);
	//Battle editor
	Add_Class_To_Main_Editor(RCE_DEFAULT,RCE_SHOW_GAME_BACKGROUND|RCE_ALLOW_GAME_PROCESSING|RCE_RIGHT_POSITION|RCE_EXITONESCAPE);
	//Alert editor
	void	EditAlertClass(DWORD _rce_, DWORD _DILOG_EDITOR_);
	EditAlertClass(RCE_DEFAULT,RCE_SHOW_GAME_BACKGROUND|RCE_ALLOW_GAME_PROCESSING|RCE_RIGHT_POSITION|RCE_EXITONESCAPE);
	//////////////
	void RegLTest();
	RegLTest();
	void reg_md_editor();
	//reg_md_editor();
	//RegisterUnitlistEditor();
	void RegFactEditor();
	RegFactEditor();
	AddStdEditor("UnitTypeGroup",&UnitTypeGroups,"dialogs\\UnitTypeGroups.xml",RCE_DEFAULT);
	AddStdEditor("NeuroStorage",&NeuroStorage,"",RCE_DEFAULT);
	void RegPresListEditor();
	RegPresListEditor();	
	void AddAlarmEditor();
	AddAlarmEditor();
	void AddMScriptEditor();
	AddMScriptEditor();
	void RegQuestEditor();
	RegQuestEditor();
	void RegGrassEditor();
	RegGrassEditor();
	void RegBLD_Editors();
	RegBLD_Editors();
	void AddAllCharsEditor();
	AddAllCharsEditor();
}