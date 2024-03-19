/*      Work with the resource files
 *   
 *  You must use this module for accesss to files this 
 * routine allows you to read files from disk or from 
 * the resource file, you even will not recognise where
 * the given file is.
 */
//#include <afx.h>

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
//#include "tntFileIO.h"
#include "unrar.h"
#include "Arc\GSCSet.h"
#include "smart_assert.h"
#include "newmemory.h"
void AText(char* str);
//typedef LPGSCfile ResFile;
typedef HANDLE ResFile;
bool InitDone=0;
extern CGSCset GSFILES;
void FilesInit();
//Opening the resource file
bool ProtectionMode=false;
ResFile RResetEx(LPCSTR lpFileName)
{
	char FNAME[256];
	strcpy(FNAME,lpFileName);
	int L=strlen(FNAME);
	for(int i=0;i<L;i++){
		if(FNAME[i]=='/')FNAME[i]='\\';
	};
	bool Only=GSFILES.m_ArchList&&ProtectionMode;
	FilesInit();
	//AText((char*)lpFileName);
	//SetLastError(0);
	//return CreateFile(lpFileName,GENERIC_READ,FILE_SHARE_READ,NULL,
	//OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	LPGSCfile lpf=GSFILES.gOpenFile(FNAME,Only);
	if(lpf)return ResFile(lpf);
	else return INVALID_HANDLE_VALUE;
}
bool GetRarName(LPCSTR Name,char* Dest){
	int L=strlen(Name);
	strcpy(Dest,Name);
	if(L>3){
		if(Dest[L-1]=='.')Dest[L-1]=0;
		else if(Dest[L-2]=='.')Dest[L-2]=0;
		else if(Dest[L-3]=='.')Dest[L-3]=0;
		else if(Dest[L-4]=='.')Dest[L-4]=0;
		strcat(Dest,".rar");
		return 1;
	}else return 0;
};
char** FHNames=NULL;
ResFile* FHANDLES=NULL;
int NHNames=0;
int MaxNames=0;
void AddFHandle(ResFile F,char* Name){
	if(NHNames>=MaxNames){
		MaxNames+=16;
		FHNames=(char**)realloc(FHNames,4*MaxNames);
		FHANDLES=(ResFile*)realloc(FHANDLES,4*MaxNames);
	};
	FHNames[NHNames]=znew(char,strlen(Name)+1);
	strcpy(FHNames[NHNames],Name);
	FHANDLES[NHNames]=F;
	NHNames++;
};
void EraseFName(ResFile F){
	for(int i=0;i<NHNames;i++)if(FHANDLES[i]==F){
		DeleteFile(FHNames[i]);
		free(FHNames[i]);
		if(i<NHNames-1){
			memcpy(FHNames+i,FHNames+i+1,4*(NHNames-i-1));
			memcpy(FHANDLES+i,FHANDLES+i+1,4*(NHNames-i-1));
		};
		NHNames--;
	};
};
void EraseAllFNames(){
	for(int i=0;i<NHNames;i++)DeleteFile(FHNames[i]);
};
void RCloseEx(ResFile hFile);
void ExtractArchive(char *ArcName,int Mode,char* Dest);
#define CEXPORT __declspec(dllexport)
CEXPORT
ResFile RReset(LPCSTR lpFileName)
{
	//FILE* fx=fopen("lox.lox","a");
	//fprintf(fx,"%s\n",lpFileName);
	//fclose(fx);
	SetLastError(0);
	ResFile F=RResetEx(lpFileName);
	if(F==INVALID_HANDLE_VALUE){
		RCloseEx(F);
		int L=strlen(lpFileName);
		char ccc[200];
		if(GetRarName(lpFileName,ccc)){
			F=RResetEx(ccc);
			if(F!=INVALID_HANDLE_VALUE){
				RCloseEx(F);
				ExtractArchive(ccc,0,(char*)lpFileName);
				F=RResetEx(lpFileName);
				if(F!=INVALID_HANDLE_VALUE)AddFHandle(F,(char*)lpFileName);
			};
		};
	};
	return F;
}
//Rewriting file
CEXPORT
ResFile RRewrite(LPCSTR lpFileName)
{
	FilesInit();
	//return CreateFile(lpFileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
	//	                         CREATE_ALWAYS,0,NULL);
	LPGSCfile lpf=GSFILES.gWriteOpen(lpFileName);
	if(lpf)return ResFile(lpf);
	else return INVALID_HANDLE_VALUE;
}
//Getting size of the resource file
CEXPORT
DWORD RFileSize(ResFile hFile)
{
	if(hFile==INVALID_HANDLE_VALUE)return 0;
	LPGSCfile hf=LPGSCfile(hFile);
	return GSFILES.gFileSize(hf);//GetFileSize(hFile,NULL);
}
// Setting file position 

// -- Modified by Silver ---21.02.2003
CEXPORT
DWORD RSeekFromCurrent(ResFile hFile,int nBytes)
{
	if(hFile==INVALID_HANDLE_VALUE)return 0;
	//return SetFilePointer(hFile,pos,NULL,FILE_BEGIN);
	LPGSCfile hf=LPGSCfile(hFile);
	GSFILES.gSeekFileFromCurrent(hf,nBytes);
	return 0;
}

CEXPORT
DWORD RSeek(ResFile hFile,int pos)
{
	if(hFile==INVALID_HANDLE_VALUE)return 0;
	//return SetFilePointer(hFile,pos,NULL,FILE_BEGIN);
	LPGSCfile hf=LPGSCfile(hFile);
	GSFILES.gSeekFile(hf,pos);
	return 0;
}
CEXPORT
DWORD RGetPos(ResFile hFile,int pos)
{
	if(hFile==INVALID_HANDLE_VALUE)return 0;
	LPGSCfile hf=LPGSCfile(hFile);
	return GSFILES.gFilePos(hf);	
}
//Reading the file
CEXPORT
DWORD RBlockRead(ResFile hFile,LPVOID lpBuffer,DWORD BytesToRead)
{
	if(hFile==INVALID_HANDLE_VALUE)return 0;
	//DWORD readBytes;
	//ReadFile(hFile,lpBuffer,BytesToRead,&readBytes,NULL);
	LPGSCfile hf=LPGSCfile(hFile);
	GSFILES.gReadFile(hf,LPBYTE(lpBuffer),BytesToRead);
	return BytesToRead;//readBytes;
}
//Writing the file
CEXPORT
DWORD RBlockWrite(ResFile hFile,LPVOID lpBuffer,DWORD BytesToWrite)
{
	if(hFile==INVALID_HANDLE_VALUE)return 0;
	//DWORD writeBytes;
	//WriteFile(hFile,lpBuffer,BytesToWrite,&writeBytes,NULL);
	LPGSCfile hf=LPGSCfile(hFile);
	GSFILES.gWriteFile(hf,LPBYTE(lpBuffer),BytesToWrite);
	return BytesToWrite;//writeBytes;
}
DWORD IOresult(void)
{
	return 0;//GetLastError();
}
void RCloseEx(ResFile hFile)
{
	if(hFile==INVALID_HANDLE_VALUE)return;
	//CloseHandle(hFile);
	LPGSCfile hf=LPGSCfile(hFile);
	GSFILES.gCloseFile(hf);
}
CEXPORT
void RClose(ResFile hFile)
{
	if(hFile==INVALID_HANDLE_VALUE)return;
	LPGSCfile hf=LPGSCfile(hFile);
	//CloseHandle(hFile);
	EraseFName(hFile);
	RCloseEx(hf);
}
void FilesInit(){
	if(InitDone)return;
	// 30-sep-04 by ManOwaR
	char szCurCosDir[MAX_PATH];
	if(GetModuleFileName(NULL,szCurCosDir,MAX_PATH)){
		strupr(szCurCosDir);
		if(char *pszDelim=strstr(szCurCosDir,"ENGINE.EXE")){
			pszDelim[0]='\0';
			SetCurrentDirectory(szCurCosDir);
		};
	};
	char CDR[256];
	GetCurrentDirectory(256,CDR);
	if(CDR[strlen(CDR)-1]=='\\')CDR[strlen(CDR)-1]=0;
	char ccc[290];
	sprintf(ccc,"%s\\unrar.dll",CDR);
	FILE* F=fopen(ccc,"r");
	if(F){
		fclose(F);
	}else{
		int L=strlen(CDR);
		while(L&&CDR[L]!='\\')L--;
		CDR[L]=0;
		sprintf(ccc,"%s\\unrar.dll",CDR);
		F=fopen(ccc,"r");
		if(F){
			fclose(F);
			SetCurrentDirectory(CDR);
		};
	};
	InitDone=1;
	GSFILES.gOpen();
};
void CloseRARLib();
void FilesExit(){
	CloseRARLib();
	EraseAllFNames();
	GSFILES.gClose();
	//tntCloseFolder();

};
bool CheckFileExistsEnumerator(const char* FileName);
bool CheckIfFileExists(char* Name){
	static char FileName[256];
	strcpy(FileName,Name);
	_strupr(FileName);
	
	if((strstr(FileName,".GP2") || strstr(FileName,".G17")||strstr(FileName,".G2D")||strstr(FileName,".G16")||strstr(FileName,".G17")) && !strchr(FileName,'/'))
	{
		return CheckFileExistsEnumerator(FileName);
	}
	
	ResFile F=RReset(Name);
	if(F==INVALID_HANDLE_VALUE)return false;
	RClose(F);
	return true;
	/*char tmp[256];
	strcpy(tmp,Name);
	int L=strlen(tmp);
	for(int i=0;i<L;i++)if(tmp[i]=='/')tmp[i]='\\';
	LPGSCFindInfo FI=GSFILES.gFindFirst(tmp);
    bool G=FI!=NULL;
	if(FI){
		while(GSFILES.gFindNext(FI));
	}
	return G;*/
};