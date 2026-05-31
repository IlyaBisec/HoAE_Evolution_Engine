#include "stdheader.h"
#define _is_in_mpl_cpp_
//#include "mpl.h"
#include "oggvor.h"
#include <stdio.h>
char FileSet[5][64]={"","","","",""};
char CurMPFile[64]="";
char MustPlayNow[64]="";
int  LastVolume=-1;
int  CurVolume=0;
bool RandomMode=0;
bool ChangeProcess=0;
int Vol=100;
bool RndFilesPresent=1;
int CurVolume0=-1;
extern int MidiSound;
//
const mMusicChanel=0;
//
void SetMP3Volume(int vol){
	ov_SetVolume(vol,mMusicChanel);
};
int GetMP3Volume(){
	/*if(CurVolume0==-1)*/
	CurVolume0=GSets.SVOpt.MusicVolume;
	return CurVolume0;
};
//
void PlayMP3File(char* FileName);
extern byte PlayGameMode;
//
extern bool InGame;
extern bool IsInVideo;
void StartPlayRandomMP3(bool CheckAnyway){
	if(IsInVideo) return;
	if(!(RndFilesPresent||CheckAnyway))return;
	char* PATH;
	extern bool InGame;
	if((!InGame)||PlayGameMode){
        PATH="Music\\"; //InMenu
	}else{
		PATH="Music\\";		
	}
	WIN32_FIND_DATA FD;
	char** FILES=NULL;
	int NFILES=0;
	char MASK[64];
	sprintf(MASK,"%s*.ogg",PATH);

	_chdir(IRM->GetHomeDirectory());

	HANDLE H=FindFirstFile(MASK,&FD);
	if(H!=INVALID_HANDLE_VALUE){
		do{
			FILES=(char**)realloc(FILES,(NFILES+1)*4);
			FILES[NFILES]=(char*)malloc(strlen(FD.cFileName)+1);
			strcpy(FILES[NFILES],FD.cFileName);
			NFILES++;
		}while(FindNextFile(H,&FD));
		FindClose(H);
	};
	if(NFILES){
		int NATTM=200;
		bool found=0;
		char NAME[128]="";
		do{
			int v=(((GetTickCount()+rand())&255)*NFILES)>>8;
			sprintf(NAME,"%s%s",PATH,FILES[v]);
			strupr(NAME);
			found=1;
			for(int j=0;j<5&&j<NFILES-1;j++)if(!strcmp(NAME,FileSet[j]))found=0;
			NATTM++;
		}while(NATTM<200&&!found);		
		if(NAME[0]){
			for(int i=0;i<4;i++)strcpy(FileSet[4-i],FileSet[3-i]);
			strcpy(FileSet[0],NAME);
			void SetCDVolume(int Vol);
			SetCDVolume(GSets.SVOpt.MusicVolume);
			PlayMP3File(NAME);
			SetCDVolume(GSets.SVOpt.MusicVolume);

		};
	}else RndFilesPresent=0;
	for(int i=0;i<NFILES;i++)delete[](FILES[i]);
	if(FILES)delete[](FILES);
	void SetCDVolume(int Vol);
	SetCDVolume(GSets.SVOpt.MusicVolume);
};
void PlayRandomMP3(){
	if(!RandomMode){
		//StartPlayRandomMP3(1);
		void ProcessAmbientMusic();
		ProcessAmbientMusic();
	};
	RandomMode=1;
};
bool STOPPED=0;
void PlayMP3File(char* FileName){
	//if((!(mpl_IsStreamFinished()||STOPPED))&&!ChangeProcess){
	//	LastVolume=MidiSound;
	//	ChangeProcess=1;
	//};
	//strcpy(MustPlayNow,FileName);
	if(FileName[0]){
		ov_Play(FileName,mMusicChanel);
		strcpy(CurMPFile,MustPlayNow);
		MustPlayNow[0]=0;
		SetMP3Volume(GSets.SVOpt.MusicVolume);
		STOPPED=0;
		ChangeProcess=0;
	}else{
		STOPPED=1;
		if(!RandomMode)ov_Stop(mMusicChanel);
	};
};
void StopPlayingMP3(){
	ov_Stop(mMusicChanel);
	STOPPED=1;
	CurMPFile[0]=0;
	ChangeProcess=0;
	SetMP3Volume(GSets.SVOpt.MusicVolume);
	//PlayMP3File("");
	RandomMode=0;
};
bool isFinishedMP3(){
	return ov_StreamFinished(mMusicChanel);
}
int T_mp3_start=0;
void ProcessMP3(bool anyway){
	if(GSets.SVOpt.MusicPlayMode==0) return;
	if(LastVolume==-1)LastVolume=GSets.SVOpt.MusicVolume;
	void ProcessAmbientMusic();
	ProcessAmbientMusic();
	int Tcr=GetTickCount();
	if(!T_mp3_start)T_mp3_start=Tcr-1000;
	if(Tcr-T_mp3_start<200&&!anyway)return;
	T_mp3_start=Tcr;
	if(ov_StreamFinished(mMusicChanel)||STOPPED){
		if(MustPlayNow[0]){
			ov_Play(MustPlayNow,mMusicChanel);
			strcpy(CurMPFile,MustPlayNow);
			MustPlayNow[0]=0;
			SetMP3Volume(GSets.SVOpt.MusicVolume);
			STOPPED=0;
			ChangeProcess=0;
		}else if(RandomMode)StartPlayRandomMP3(0);
	}else{
		int V=GetMP3Volume();
		if(V<2){
			ov_Stop(mMusicChanel);
			STOPPED=1;
			CurMPFile[0]=0;
			ChangeProcess=0;
			//SetMP3Volume(MidiSound);
		}else{
			if(ChangeProcess){
				V=0;//-=(V/6)+1;
				if(V<0)V=0;
				SetMP3Volume(V);
			};
		};
	};
};
char* GetCurrentMP3File(){
	return CurMPFile;
};