//----------------------------------------------------------------------------------------------------------------//
#include "stdheader.h"
//----------------------------------------------------------------------------------------------------------------//
typedef bool FileAction(HANDLE ActionFile, WIN32_FIND_DATA* WFD, const char* FileName, void* Param);

int ProcessTree(const char* lpszTreeNode , FileAction* FA, void* Param)
{
	WIN32_FIND_DATA	wfd;
	HANDLE			hFindFile;
	CHAR			szDelPath[255];
	CHAR			szNewLevel[255];
	CHAR			szDelStr[255];
	BOOL			Found;

	strcpy(szDelPath,lpszTreeNode);
	strcat(szDelPath,"\\*.*");

	hFindFile=FindFirstFile(szDelPath,&wfd);

	if(hFindFile!=INVALID_HANDLE_VALUE)
		Found=TRUE;
	else
		Found=FALSE;

	while(Found)
	{
		if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY)
		{
			strcpy(szDelStr,lpszTreeNode);
			strcat(szDelStr,"\\");
			strcat(szDelStr,wfd.cFileName);
			FA(hFindFile,&wfd, szDelStr, Param);
		}
		else
		{
			// Enter new level
			if((strcmp(wfd.cFileName,".")!=0) && (strcmp(wfd.cFileName,"..")!=0) && !strstr(wfd.cFileName,".svn"))
			{
				strcpy(szNewLevel,lpszTreeNode);
				strcat(szNewLevel,"\\");
				strcat(szNewLevel,wfd.cFileName);
				ProcessTree(szNewLevel, FA, Param);
			};
		};			
		Found=FindNextFile(hFindFile,&wfd);
	};

	FindClose(hFindFile);
	return 0;
}
//----------------------------------------------------------------------------------------------------------------//
bool AddFileName(HANDLE ActionFile, WIN32_FIND_DATA* WFD, const char* FileName, void* Param)
{
	if(FileName)
	{
		_strupr((char*)FileName);
		if(strstr(FileName,".GP2") || strstr(FileName,".G2D")||strstr(FileName,".G16")||strstr(FileName,".G17"))
		{
			static Enumerator* En = ENUM.Get("CheckFileEnumerator");
			if(FileName[0]=='\\')
				En->Add((char*)(FileName+1),1);
			if(FileName[0]=='.')
				En->Add((char*)(FileName+2),1);
			else
				En->Add((char*)(FileName),1);
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------------------------//
bool CheckFileEnumeratorFirstProcess=false;
bool CheckFileExistsEnumerator(const char* FileName)
{
	bool rez=false;
	if(FileName)
	{
		if(!CheckFileEnumeratorFirstProcess)
		{
			CheckFileEnumeratorFirstProcess=true;
			static Enumerator* En = ENUM.Get("CheckFileEnumerator");
			GFILE* F=NULL;
			GFILE* F1=Gopen("usegfiles","r");
			if(F1){
				F=Gopen("gfiles.txt","r");
				Gclose(F1);
			}
			if(F){
                bool f=false;
				do{
					bool ReadWinString(GFILE* F,char* STR,int Max);
					char s[1024];
                    f=ReadWinString(F,s,1024);
					if(f){
						_strupr(s);
                        En->Add(s,1);
					}
				}while(f);
				Gclose(F);
			}else{
				ProcessTree(".", &AddFileName, NULL);
				FILE* F=fopen("gfiles.txt","w");
				if(F){
					for(int i=0;i<En->GetAmount();i++){
						fprintf(F,"%s\n",En->GetStr(i));
					}
					fclose(F);
				}
			}
		}
		/*
		char Upp[256];
		strcpy(Upp,FileName);
		_strupr(Upp);
		*/
		static Enumerator* En = ENUM.Get("CheckFileEnumerator");
		
		if(En->Get((char*)FileName)==1)
		{
			rez=true;
		}
		else
		{
			rez=false;
		}
	}
	return rez;
}
//----------------------------------------------------------------------------------------------------------------//