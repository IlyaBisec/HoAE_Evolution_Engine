
typedef HANDLE ResFile;
//Opening the resource file
ResFile RReset(LPCSTR lpFileName);
//Rewriting file
ResFile RRewrite(LPCSTR lpFileName);
//Getting size of the resource file
DWORD RFileSize(HANDLE hFile);
// Setting file position 
DWORD RSeek(ResFile hFile,int pos);
DWORD RSeekFromCurrent(ResFile hFile,int nBytes);
//Reading the file
DWORD RBlockRead(ResFile hFile,LPVOID lpBuffer,DWORD BytesToRead);
//Writing the file
DWORD RBlockWrite(ResFile hFile,LPVOID lpBuffer,DWORD BytesToWrite);
//Returns last error
DWORD IOresult(void);
//Close the file
void RClose(ResFile hFile);