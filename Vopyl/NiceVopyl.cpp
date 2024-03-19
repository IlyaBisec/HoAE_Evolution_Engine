#include <windows.h>
#include <math.h>

#include "NiceVopyl.h"
#include "include/vopl_globals.h"
#include "VopylClasses.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
};

extern "C" {
	NICEVOPYL_API void __cdecl nvInit(LPDIRECTSOUND pDS, HWND hWnd);
	NICEVOPYL_API void __cdecl nvDone();
	NICEVOPYL_API void __cdecl nvPlay(LPCSTR pcszFileName,BYTE ucStream);
	NICEVOPYL_API void __cdecl nvStop(BYTE ucStream);
	NICEVOPYL_API void __cdecl nvSetVolume(BYTE ucVolume,BYTE ucStream);
	NICEVOPYL_API void __cdecl nvFadeVolume(BYTE ucVolume,BYTE ucStream);
	NICEVOPYL_API void __cdecl nvCyclic(bool bCyclic,BYTE ucStream);
	NICEVOPYL_API WORD __cdecl nvGetStreamLength(BYTE ucStream);
	NICEVOPYL_API bool __cdecl nvStreamFinished(BYTE ucStream);
};

static VOPL_CMD_BUFF	vcBuffer;
static CRITICAL_SECTION vCritSection;
static HANDLE			hThread=NULL; 
static LPMYDIRECTSOUND	pDirectSound=NULL;

DWORD WINAPI VoplWorker(LPVOID lpParam) 
{ 
	LPCStreamingSound	SSA[VOPL_NUM_CHAN];
	VOPL_CMD_BUFF		lcBuffer;
	int					i;
	bool				bOk=false;
	LONG				lVol,lTarVol;
	
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);

	for(i=0;i<VOPL_NUM_CHAN;i++)
		SSA[i]=NULL;

	for(;;){
		EnterCriticalSection(&vCritSection);
			memcpy(&lcBuffer,&vcBuffer,sizeof(VOPL_CMD_BUFF));			
		LeaveCriticalSection(&vCritSection);

		if(lcBuffer.m_bTerminate)
			break;

		for(i=0;i<VOPL_NUM_CHAN;i++){

			if(SSA[i]&&(lcBuffer.m_Chan[i].m_lTargetVolume<0)){
				// fade in or fade out
				SSA[i]->GetVolume(&lVol);
				lTarVol=lcBuffer.m_Chan[i].m_lTargetVolume;
				if(lVol>lTarVol){
					// fade out (-)
					lVol-=50;
					if(lVol<=lTarVol){
						lVol=lTarVol;
						lcBuffer.m_Chan[i].m_lTargetVolume=-lcBuffer.m_Chan[i].m_lTargetVolume;
					};
				}else{
					// fade in (+)
					lVol+=50;
					if(lVol>=lTarVol){
						lVol=lTarVol;
						lcBuffer.m_Chan[i].m_lTargetVolume=-lcBuffer.m_Chan[i].m_lTargetVolume;
					};
				};
				SSA[i]->SetVolume(lVol);
			};

			if(SSA[i]&&lcBuffer.m_Chan[i].m_bFade){
				SSA[i]->GetVolume(&lVol);
				lVol-=300;
				if(lVol<DSBVOLUME_MIN){
					delete SSA[i];
					SSA[i]=NULL;
					lcBuffer.m_Chan[i].m_bFinished=true;
					lcBuffer.m_Chan[i].m_usLength=0;
					lcBuffer.m_Chan[i].m_bFade=false;
				}else
					SSA[i]->SetVolume(lVol);
			};

			if(lcBuffer.m_Chan[i].m_vcCommand!=vcFree){
				switch(lcBuffer.m_Chan[i].m_vcCommand){
					case vcPlay		:
						if(SSA[i])delete SSA[i];
						SSA[i]=new CStreamingSound;
						if(lcBuffer.m_Chan[i].m_lTargetVolume>0)
							lcBuffer.m_Chan[i].m_lTargetVolume=-lcBuffer.m_Chan[i].m_lTargetVolume;
						SSA[i]->SetStartVolume(lcBuffer.m_Chan[i].m_lTargetVolume);
						SSA[i]->Play(pDirectSound,lcBuffer.m_Chan[i].m_szFileName);
//						lcBuffer.m_Chan[i].m_lTargetVolume=0xFF;
						lcBuffer.m_Chan[i].m_bCyclic=false;
						lcBuffer.m_Chan[i].m_bFinished=false;
						lcBuffer.m_Chan[i].m_bFade=false;
						lcBuffer.m_Chan[i].m_usLength=(WORD)SSA[i]->GetSoundSource()->GetStreamLength();
						break;
					case vcStop		:
						if(SSA[i]){
							delete SSA[i];
							SSA[i]=NULL;
							lcBuffer.m_Chan[i].m_bFinished=true;
//							lcBuffer.m_Chan[i].m_lTargetVolume=0xFF;
							lcBuffer.m_Chan[i].m_usLength=0;
							lcBuffer.m_Chan[i].m_bFade=false;
						};
						break;
					case vcVolume	:
						if(SSA[i]&&(!lcBuffer.m_Chan[i].m_bFade))
							SSA[i]->SetVolume((LONG)(2000.0f*log((lcBuffer.m_Chan[i].m_ucVolume+0.883f)/101.0f)));
						break;
					case vcCyclic	:
						if(SSA[i])
							SSA[i]->GetSoundSource()->SetLoopMode(lcBuffer.m_Chan[i].m_bCyclic);
						break;
				};
				lcBuffer.m_Chan[i].m_vcCommand=vcFree;
			};
			if(SSA[i]){
				SSA[i]->HandleBuffer();
				if(lcBuffer.m_Chan[i].m_bFinished=SSA[i]->IsFinished()){
						delete SSA[i];
						SSA[i]=NULL;
						lcBuffer.m_Chan[i].m_usLength=0;
						lcBuffer.m_Chan[i].m_bFade=false;
//						lcBuffer.m_Chan[i].m_lTargetVolume=0xFF;
				};
			};
		};

		EnterCriticalSection(&vCritSection);
			memcpy(&vcBuffer,&lcBuffer,sizeof(VOPL_CMD_BUFF));			
		LeaveCriticalSection(&vCritSection);

		Sleep(10);
	};

	for(i=0;i<VOPL_NUM_CHAN;i++)
		if(SSA[i])delete SSA[i];
	
	EnterCriticalSection(&vCritSection);
		vcBuffer.m_bTerminate=false;
	LeaveCriticalSection(&vCritSection);

	return 0; 
} 

void __cdecl nvInit(LPDIRECTSOUND pDS, HWND hWnd)
{

	DWORD dwThreadId;
	int	i;

	vcBuffer.m_bTerminate=false;

	pDirectSound=new CMyDirectSound (pDS);

	for(i=0;i<VOPL_NUM_CHAN;i++){
		vcBuffer.m_Chan[i].m_bCyclic=false;
		vcBuffer.m_Chan[i].m_bFinished=true;
		vcBuffer.m_Chan[i].m_szFileName[0]='\0';
		vcBuffer.m_Chan[i].m_ucVolume=100;
		vcBuffer.m_Chan[i].m_usLength=0;
		vcBuffer.m_Chan[i].m_vcCommand=vcFree;
		vcBuffer.m_Chan[i].m_bFade=false;
		vcBuffer.m_Chan[i].m_lTargetVolume=DSBVOLUME_MAX;
	};

	InitializeCriticalSection(&vCritSection);

    hThread=CreateThread(NULL,0,VoplWorker,NULL,0,&dwThreadId);	
};

void __cdecl nvDone()
{
	bool bOk=false;

	EnterCriticalSection(&vCritSection);
		vcBuffer.m_bTerminate=true;
	LeaveCriticalSection(&vCritSection);

	Sleep(100);

	for(;;){
		EnterCriticalSection(&vCritSection);
			if(!vcBuffer.m_bTerminate)
				bOk=true;
		LeaveCriticalSection(&vCritSection);
		if(bOk)
			break;
		Sleep(50);
	};

	CloseHandle(hThread);

	DeleteCriticalSection(&vCritSection);

	if(pDirectSound){
		delete pDirectSound;
		pDirectSound=NULL;
	}
};

void __cdecl nvPlay(LPCSTR pcszFileName,BYTE ucStream)
{
	bool bOk=false;

	if(GetFileAttributes(pcszFileName)==INVALID_FILE_ATTRIBUTES){
		char	szFileName[MAX_PATH];
		sprintf(szFileName,"File \"%s\" not found!",pcszFileName);
		MessageBox(NULL,szFileName,"Vopyl Warning",MB_OK | MB_ICONHAND);
		return;
	};
	
	for(;;){
		EnterCriticalSection(&vCritSection);
			if(vcBuffer.m_Chan[ucStream].m_vcCommand==vcFree){
					vcBuffer.m_Chan[ucStream].m_vcCommand=vcPlay;
					strcpy(vcBuffer.m_Chan[ucStream].m_szFileName,pcszFileName);
					bOk=true;
			};
		LeaveCriticalSection(&vCritSection);
		if(bOk)
			break;
		Sleep(50);
	};
};

void __cdecl nvStop(BYTE ucStream)
{
	bool bOk=false;
	
	for(;;){
		EnterCriticalSection(&vCritSection);
			if(vcBuffer.m_Chan[ucStream].m_vcCommand==vcFree){
					//vcBuffer.m_Chan[ucStream].m_vcCommand=vcStop;
					vcBuffer.m_Chan[ucStream].m_bFade=true;
//					vcBuffer.m_Chan[ucStream].m_lTargetVolume=0xFF;
					bOk=true;
			};
		LeaveCriticalSection(&vCritSection);
		if(bOk)
			break;
		Sleep(50);
	};
};

void __cdecl nvFadeVolume(BYTE ucVolume,BYTE ucStream)
{
	bool bOk=false;
	
	for(;;){
		EnterCriticalSection(&vCritSection);
			if((vcBuffer.m_Chan[ucStream].m_vcCommand==vcFree)
				||
				(vcBuffer.m_Chan[ucStream].m_vcCommand==vcPlay)				
				){
					vcBuffer.m_Chan[ucStream].m_vcCommand=vcVolume;
					vcBuffer.m_Chan[ucStream].m_ucVolume=ucVolume<=100?ucVolume:100;
					bOk=true;
			};
		LeaveCriticalSection(&vCritSection);
		if(bOk)
			break;
		Sleep(50);
	};
};

void __cdecl nvSetVolume(BYTE ucVolume,BYTE ucStream)
{
	bool bOk=false;
	
	for(;;){
		EnterCriticalSection(&vCritSection);
			if((vcBuffer.m_Chan[ucStream].m_vcCommand==vcFree)
				||
				(vcBuffer.m_Chan[ucStream].m_vcCommand==vcPlay)				
				){
					vcBuffer.m_Chan[ucStream].m_lTargetVolume=
						(LONG)(2000.0f*log(((ucVolume<=100?ucVolume:100)+0.883f)/101.0f));
					bOk=true;
			};
		LeaveCriticalSection(&vCritSection);
		if(bOk)
			break;
		Sleep(50);
	};
};

void __cdecl nvCyclic(bool bCyclic,BYTE ucStream)
{
	bool bOk=false;
	
	for(;;){
		EnterCriticalSection(&vCritSection);
			if(vcBuffer.m_Chan[ucStream].m_vcCommand==vcFree){
					vcBuffer.m_Chan[ucStream].m_vcCommand=vcCyclic;
					vcBuffer.m_Chan[ucStream].m_bCyclic=bCyclic;
					bOk=true;
			};
		LeaveCriticalSection(&vCritSection);
		if(bOk)
			break;
		Sleep(50);
	};
};

WORD __cdecl nvGetStreamLength(BYTE ucStream)
{
	bool bOk=false;
	WORD usLength;
	
	for(;;){
		EnterCriticalSection(&vCritSection);
			if(vcBuffer.m_Chan[ucStream].m_vcCommand==vcFree){
					usLength=vcBuffer.m_Chan[ucStream].m_usLength;
					bOk=true;
			};
		LeaveCriticalSection(&vCritSection);
		if(bOk)
			break;
		Sleep(50);
	};
	return usLength;
};

bool __cdecl nvStreamFinished(BYTE ucStream)
{
	bool bOk=false;
	bool bFinished;
	
	for(;;){
		EnterCriticalSection(&vCritSection);
			if(vcBuffer.m_Chan[ucStream].m_vcCommand==vcFree){
					bFinished=vcBuffer.m_Chan[ucStream].m_bFinished;
					bOk=true;
			};
		LeaveCriticalSection(&vCritSection);
		if(bOk)
			break;
		Sleep(50);
	};
	return bFinished;
};
