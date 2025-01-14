#include <windows.h>
#include <stdlib.h>
typedef HANDLE ResFile;
//Opening the resource file
ResFile RReset(LPCSTR lpFileName)
{
	SetLastError(0);
	return CreateFile(lpFileName,GENERIC_READ,FILE_SHARE_READ,NULL,
		                         OPEN_EXISTING,0/*FILE_ATTRIBUTE_NORMAL*/,NULL);
}
//Rewriting file
ResFile RRewrite(LPCSTR lpFileName)
{
	SetLastError(0);
	return CreateFile(lpFileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		                         CREATE_ALWAYS,0,NULL);
}
//Getting size of the resource file
DWORD RFileSize(HANDLE hFile)
{
	SetLastError(0);
	return GetFileSize(hFile,NULL);
}
// Setting file position 
DWORD RSeek(ResFile hFile,int pos)
{
	SetLastError(0);
	return SetFilePointer(hFile,pos,NULL,FILE_BEGIN);
}

DWORD RSeekFromCurrent(ResFile hFile,int nBytes)
{
	SetLastError(0);
	return SetFilePointer(hFile,nBytes,NULL,FILE_CURRENT);
}

//Reading the file
DWORD RBlockRead(ResFile hFile,LPVOID lpBuffer,DWORD BytesToRead)
{
	SetLastError(0);
	DWORD readBytes;
	ReadFile(hFile,lpBuffer,BytesToRead,&readBytes,NULL);
	return readBytes;
}
//Writing the file
DWORD RBlockWrite(ResFile hFile,LPVOID lpBuffer,DWORD BytesToWrite)
{
	SetLastError(0);
	DWORD writeBytes;
	WriteFile(hFile,lpBuffer,BytesToWrite,&writeBytes,NULL);
	return writeBytes;
}
DWORD IOresult(void)
{
	return GetLastError();
}
void RClose(ResFile hFile)
{
	SetLastError(0);
	if(!CloseHandle(hFile))
		MessageBox(NULL,"Failed to close file!","Strange error",0);
}