#include "stdheader.h"
#include "WeaponSystem.h"
#include "AI_Scripts.h"								//Vitya //  [11/11/2003]
#include "Dialogs\MISSIONSCRIPT\MissionScript.h"	//Vitya //  [11/11/2003]
#include "Dialogs\MISSIONSCRIPT\EndMissionData.h"	//Vitya //  [11/12/2003]
#include "Dialogs\ClasDeclaration.h"				//Vitya.
#include "Dialogs\CPlayerSave.hpp"					//Grey.
#include "cvi_MainMenu.h"
//////////////////////////////////////////////////////////////////////////
void CloseMPL();
bool CreateMultiplaterInterface();
//bool WaitingHostGame(int);
//bool CreateNamedSession(char* Name,DWORD User2,int Max);
void PlayGame();
void RunMission(int n,int Diff);
bool ProcessBigMap(int campainID);
//bool PlayerInterface();
void ProcessDialogsEditor();
void ShowLoading();
void PrepareEditMedia(byte myid);
extern bool RUNMAPEDITOR;
extern bool RUNUSERMISSION;
extern CEXPORT bool TOTALEXIT;
extern bool InternetGame;
extern bool UseGSC_Login;
extern int CurProtocol;
extern char PlName[64];
//
_str vi_ReturnID;
//
void vi_Single();
void vi_Options();
void vi_Battles();
void vi_Multi();
bool vi_BattlesMulti();
//
void KeyTestMem();
int vi_MainMenu(){
	RUNUSERMISSION=false;
	//
	v_MainMenu.Cross=1;
	v_MainMenu.CrossTime=0;
	v_MainMenu.CrossSD=NULL;
	v_MainMenu.StartDS("Main");
	//
	return mcmExit;
	//
	static DialogsSystem* DS=NULL;
	if(!DS){
		xmlQuote xml;
		if(xml.ReadFromFile("Dialogs\\v\\MM_Main.DialogsSystem.xml")){
			ErrorPager Error;
			DS = new DialogsSystem;
			DS->Load(xml,DS,&Error);
		};
	}
	static Enumerator* EN=NULL;
	if(!EN){
		EN=ENUM.Get("ve_MainMenu");
		//EN->Add("Options",mcmOptions);
		//EN->Add("Editor",mcmVideo);
		EN->Add("Exit",mcmExit);
		EN->Add("UserMission",111);
	}
	if(DS&&EN){
		// init
		InternetGame=false;
		UseGSC_Login=0;
		//
		vi_ReturnID="";		
		if(RUNMAPEDITOR) vi_ReturnID="Editor";
		if(RUNUSERMISSION) vi_ReturnID="UserMission";
		do{
			ProcessMessages();
			DS->ProcessDialogs();
			DS->RefreshView();
			//
			if(TOTALEXIT) vi_ReturnID="Exit";
			if(vi_ReturnID=="Single"){
				vi_Single();
			} else if(vi_ReturnID=="Multi"){
				vi_Multi();
			} else if(vi_ReturnID=="Options"){
				vi_Options();				
			}else if(vi_ReturnID=="Editor"){
				ProcessDialogsEditor();
				LastKey=0;
				vi_ReturnID="";
			}
		}while(vi_ReturnID=="");
	}
	LastKey=0;
	if(vi_ReturnID=="Editor"){
		ShowLoading();
		LOADNATMASK=0xFFFFFFFF;
		PrepareEditMedia(0);
		//EditGame();
	}else if(vi_ReturnID=="Inet"){
		InternetGame=true;
		UseGSC_Login=1;
		CurProtocol=3;
		processMultiplayer();
		return 2;
	}else{
		//vi_ReturnID="";	
		int ret=EN->Get(vi_ReturnID.pchar());
		if(ret!=-1) return ret;
	}
	return mcmSingle;
}
void vi_Options(){
	// init
	static DialogsSystem* DS=NULL;
	if(!DS){
		xmlQuote xml;
		if(xml.ReadFromFile("dialogs\\v\\MM_Options.DialogsSystem.xml")){
			ErrorPager Error;
			DS=new DialogsSystem;
			DS->Load(xml,DS,&Error);
		}
	}
	if(DS){		
		vi_ReturnID="";
		do{
			ProcessMessages();
			DS->ProcessDialogs();
			DS->RefreshView();
		}while(vi_ReturnID=="");
	}
	vi_ReturnID="";
	LastKey=0;
}

void vi_Single(){
	// init
	static DialogsSystem* DS=NULL;
	if(!DS){
		xmlQuote xml;
		if(xml.ReadFromFile("dialogs\\v\\MM_Single.DialogsSystem.xml")){
			ErrorPager Error;
			DS=new DialogsSystem;
			DS->Load(xml,DS,&Error);
		}
	}
	if(DS){		
		vi_ReturnID="";
		CPlayerSAVE	Pl;
        if(Pl.m_pPlayer.GetAmount()!=0/*||PlayerInterface()*/) do{
			do{
				ProcessMessages();
				DS->ProcessDialogs();
				DS->RefreshView();
			}while(vi_ReturnID=="");
			if(vi_ReturnID=="Profile"){
				//PlayerInterface();
				Pl.reset_class(&Pl);
				Pl.LoadXML();
				if(Pl.m_pPlayer.GetAmount()!=0) vi_ReturnID="";
			}else if(vi_ReturnID=="Campaign"){
				ProcessBigMap(0);
				vi_ReturnID="";
			}else if(vi_ReturnID=="Battles"){
				vi_Battles();
			}
		}while(vi_ReturnID=="");
	}
	vi_ReturnID="";
	LastKey=0;
}
int vCurMiss=-1;
void vi_Battles(){
	// init
	static DialogsSystem* DS=NULL;
	if(!DS){
		xmlQuote xml;
		if(xml.ReadFromFile("dialogs\\v\\MM_Battles.DialogsSystem.xml")){
			ErrorPager Error;
			DS=new DialogsSystem;
			DS->Load(xml,DS,&Error);
		}
	}
	if(DS){		
		vi_ReturnID="";
		vCurMiss=1;
		do{		
			do{
				ProcessMessages();
				DS->ProcessDialogs();
				DS->RefreshView();
			}while(vi_ReturnID=="");
			if(vi_ReturnID=="Start"){
				if(vCurMiss!=-1){
					RunMission(vCurMiss,0);
					PlayGame();
				}
				vi_ReturnID="";
			}
		}while(vi_ReturnID=="");
	}
	vi_ReturnID="";
	LastKey=0;
};
void vi_Multi(){
	// init
	static DialogsSystem* DS=NULL;
	if(!DS){
		xmlQuote xml;
		if(xml.ReadFromFile("dialogs\\v\\MM_Multi.DialogsSystem.xml")){
			ErrorPager Error;
			DS=new DialogsSystem;
			DS->Load(xml,DS,&Error);
		}
	}
	if(DS){
		vi_ReturnID="";
		do{			
			do{
				ProcessMessages();
				DS->ProcessDialogs();
				DS->RefreshView();
			}while(vi_ReturnID=="");
			if(vi_ReturnID=="Create"){
				//if(CreateNamedSession(PlName,0,GMMAXPL))WaitingHostGame(0);
				//if(CreateNamedSession("Vital",0,2))WaitingHostGame(0);
				strcpy(PlName,"vital");
				CloseMPL();
				CreateMultiplaterInterface();
				/*
				if(vi_BattlesMulti()){
				}else{
					vi_ReturnID="";
				}
				*/
			}
		}while(vi_ReturnID=="");
	}	
	LastKey=0;
	vi_ReturnID="";
};
bool vi_BattlesMulti(){
	// init
	static DialogsSystem* DS=NULL;
	if(!DS){
		xmlQuote xml;
		if(xml.ReadFromFile("dialogs\\v\\MM_BattlesMulti.DialogsSystem.xml")){
			ErrorPager Error;
			DS=new DialogsSystem;
			DS->Load(xml,DS,&Error);
		}
	}
	if(DS){		
		vi_ReturnID="";
		do{
			ProcessMessages();
			DS->ProcessDialogs();
			DS->RefreshView();
		}while(vi_ReturnID=="");
	}
	vi_ReturnID="";
	LastKey=0;
	if(vi_ReturnID=="Cancel") return false;
		else return true;
};
