// GSCarch.cpp: implementation of the CGSCarch class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "GSCarch.h"

#include <stdio.h>
#include "..\ddini.h"
#include "GSCtypes.h"
#include "GSCarch.h"
#include "GSCset.h"
#include "isiMasks.h"

;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void GSC_OpenError();
CGSCarch::CGSCarch()
{
	
}
GFILE_API
CGSCarch::~CGSCarch()
{

}

void GSC_OpenError(){
	char	szTitle[256],szMessage[1024],szModule[512];
	DWORD	dwLastError=GetLastError();

	GetModuleFileName(0x00,szModule,512);
	
	sprintf(szTitle,"%s : I/O Error (%u)",1+strrchr(szModule,'\\'),dwLastError);
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwLastError,
		LANG_USER_DEFAULT,
		szMessage,
		1024,
		NULL);

	MessageBox(NULL,szMessage,szTitle,MB_ICONERROR);

//	ExitProcess(2);

};

BOOL CGSCarch::Open(LPCSTR lpcsArchFileName)
{
	m_bDamnWindows=FALSE;

	strcpy(m_ArchName,lpcsArchFileName);

	m_hMapFile=CreateFile(
		m_ArchName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if(m_hMapFile==INVALID_HANDLE_VALUE){
		GSC_OpenError();
		return FALSE;
	};

	// Creating file mapping token
	m_hMapping=CreateFileMapping(
		m_hMapFile,
		NULL,
		PAGE_READONLY,		// in read-only mode
		0,0,				// cover entire file
		NULL);				// anonymous mapping

	if(!m_hMapping){
		m_bDamnWindows=TRUE;
		goto init_in_common_way;
	};

	// Attempting to map into process address space
	m_Header=(TGSCarchHDR*)MapViewOfFile(	
		m_hMapping,
		FILE_MAP_READ,		// in read-only mode
		0,0,				// from the very begining
		0);					// entire file

	if(!m_Header){
		CloseHandle(m_hMapping);
		m_hMapping=NULL;
		m_bDamnWindows=TRUE;
		goto init_in_common_way;
	};

	// Setting file allocation table
	m_FAT=(TGSCarchFAT*)((LPBYTE)m_Header+sizeof(TGSCarchHDR));

	m_DataOffset=sizeof(TGSCarchHDR)+m_Header->m_Entries*sizeof(TGSCarchFAT);

	return TRUE;

init_in_common_way:

	DWORD	dwBytesRead;

	// Reading header into the memory
	m_Header=(TGSCarchHDR*)malloc(sizeof(TGSCarchHDR));
	::ReadFile(m_hMapFile,m_Header,sizeof(TGSCarchHDR),&dwBytesRead,NULL);

	// Reading FAT into the memory
	m_FAT=(TGSCarchFAT*)malloc((m_Header->m_Entries)*sizeof(TGSCarchFAT));
	::ReadFile(m_hMapFile,m_FAT,(m_Header->m_Entries)*sizeof(TGSCarchFAT),&dwBytesRead,NULL);

	m_DataOffset=sizeof(TGSCarchHDR)+m_Header->m_Entries*sizeof(TGSCarchFAT);

	return TRUE;
}

BOOL CGSCarch::Close()
{
	if(m_bDamnWindows){
		SAFE_FREE(m_Header);
		SAFE_FREE(m_FAT);
		SAFE_CLOSE(m_hMapFile);		
	}else{
		SAFE_UNMAP(m_Header);
		SAFE_CLOSE_MAP(m_hMapping);
		SAFE_CLOSE(m_hMapFile);
	};

	return TRUE;
}

LPGSCfile CGSCarch::GetFileHandle(LPCSTR lpcsFileName)
{
    if(strlen(lpcsFileName)>60)return NULL;
	DWORD			i=0;
	LPGSCfile		lpFileHandle=NULL;
	LPGSCarchFAT	pFAT=NULL; 
	CHAR			sUpFileName[MAX_PATH];

	sUpFileName[0]=0;
	strcpy(sUpFileName,lpcsFileName);
	_strupr(sUpFileName);

	DWORD HASH=isiCalcHash(sUpFileName);

	for(i=0;i<m_Header->m_Entries;i++){
		pFAT=(TGSCarchFAT*)(LPBYTE(m_FAT)+i*sizeof(TGSCarchFAT));
		if(pFAT->m_Hash==HASH)
			if(!strcmp(LPCSTR(pFAT->m_FileName),sUpFileName)){
				lpFileHandle=new TGSCfile;
				lpFileHandle->m_FileHandle=i;
				lpFileHandle->m_Flags=0x80 | pFAT->m_Flags;	// archieve file
				lpFileHandle->m_Position=0;
				lpFileHandle->m_Size=pFAT->m_Size;
				lpFileHandle->m_Data=m_bDamnWindows?(LPBYTE)(m_DataOffset+(~pFAT->m_Offset)):
					(LPBYTE)((LPBYTE)m_Header+m_DataOffset+(~pFAT->m_Offset));
   				return lpFileHandle;				
			};
	};
		
	return NULL;
}

VOID CGSCarch::CloseFileHandle(LPGSCfile lpFileHandle)
{
	SAFE_DELETE(lpFileHandle);
}

VOID CGSCarch::MemDecrypt(LPBYTE lpbDestination, DWORD dwSize)
{
	BYTE Key=~(HIBYTE(_CRYPT_KEY_));

	isiDecryptMem(lpbDestination,dwSize,Key);
}

DWORD CGSCarch::GetFileSize(LPGSCfile lpFileHandle)
{
	return lpFileHandle->m_Size;
}


DWORD CGSCarch::GetFilePos(LPGSCfile lpFileHandle)
{
	return lpFileHandle->m_Position;
}

VOID CGSCarch::SetFilePos(LPGSCfile lpFileHandle, DWORD dwPosition)
{
	lpFileHandle->m_Position=dwPosition;
}

VOID CGSCarch::ReadFile(LPGSCfile lpFileHandle, LPBYTE lpbBuffer, DWORD dwSize)
{
	if(!dwSize)
		return;

	if(m_bDamnWindows){
		DWORD	dwBytesRead;
		SetFilePointer(m_hMapFile,(DWORD)lpFileHandle->m_Data+lpFileHandle->m_Position,NULL,FILE_BEGIN);
		::ReadFile(m_hMapFile,lpbBuffer,dwSize,&dwBytesRead,NULL);
	}else{
		int maxsize=lpFileHandle->m_Size-lpFileHandle->m_Position;
		if(maxsize<0)maxsize=0;
		if(dwSize>maxsize)dwSize=maxsize;
		if(dwSize)memcpy(lpbBuffer,lpFileHandle->m_Data+lpFileHandle->m_Position,dwSize);
	}
	if(dwSize && (lpFileHandle->m_Flags)&0x7F)
		MemDecrypt(lpbBuffer,dwSize);
	
	lpFileHandle->m_Position+=dwSize;
}

LPBYTE CGSCarch::GetFileData(LPGSCfile lpFileHandle)
{
	return m_bDamnWindows?NULL:lpFileHandle->m_Data;
}

LPGSCFindData CGSCarch::FindFile(LPCSTR lpcsMask)
{
	LPGSCarchFAT pFAT;
	LPGSCFindData pFindData;
	LPSTR	lpsDelim;
	CHAR	sUpMask[64];

    char drive		[_MAX_DRIVE];
    char directory	[_MAX_DIR  ];
    char filename	[_MAX_PATH ];
    char ext		[_MAX_EXT  ];

    _splitpath( lpcsMask, drive, directory, filename, ext );
    strcat(drive,directory);
    int L=strlen(drive);    

	memset(sUpMask,0,64);
	strcpy(sUpMask,lpcsMask);
	_strupr(sUpMask);
	
	DWORD i;

	pFindData=new TGSCFindData;

	strcpy(pFindData->m_Mask,sUpMask);
	pFindData->m_Found=-1;

	for(i=0;i<m_Header->m_Entries;i++){
		pFAT=(TGSCarchFAT*)(LPBYTE(m_FAT)+i*sizeof(TGSCarchFAT));
		if(isiMatchesMask(LPSTR(pFAT->m_FileName),sUpMask)){
			//lpsDelim=strrchr(LPSTR(pFAT->m_FileName),'\\');
			//if(lpsDelim)
			//	strcpy(LPSTR(pFindData->m_FileName),LPSTR(lpsDelim+1));
			//else
			//	strcpy(LPSTR(pFindData->m_FileName),LPSTR(pFAT->m_FileName));
            strcpy(pFindData->m_FileName,LPSTR(pFAT->m_FileName)+L);			
			pFindData->m_Found=i;
            pFindData->m_PreffixLength=L;
			return pFindData;
		};
	};

	SAFE_DELETE(pFindData);

	return NULL;
};

BOOL CGSCarch::NextFile(LPGSCFindData gFindData)
{
	LPGSCarchFAT pFAT;
	LPSTR lpsDelim;
	
	DWORD i;

	for(i=gFindData->m_Found+1;i<m_Header->m_Entries;i++){
		pFAT=(TGSCarchFAT*)(LPBYTE(m_FAT)+i*sizeof(TGSCarchFAT));
		if(isiMatchesMask(LPSTR(pFAT->m_FileName),LPSTR(gFindData->m_Mask))){
			//lpsDelim=strrchr(LPSTR(pFAT->m_FileName),'\\');
			//if(lpsDelim)
			//	strcpy(LPSTR(gFindData->m_FileName),LPSTR(lpsDelim+1));
			//else
			//	strcpy(LPSTR(gFindData->m_FileName),LPSTR(pFAT->m_FileName));
            strcpy(LPSTR(gFindData->m_FileName),LPSTR(pFAT->m_FileName)+gFindData->m_PreffixLength);
			gFindData->m_Found=i;
			return TRUE;
		};
	};

	SAFE_DELETE(gFindData);

	return FALSE;
}

VOID CGSCarch::FindClose(LPGSCFindData gFindData)
{
	SAFE_DELETE(gFindData);
}
