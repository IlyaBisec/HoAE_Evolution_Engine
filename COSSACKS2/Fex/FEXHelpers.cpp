#include <windows.h>
#include <tlhelp32.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dia2.h"
#include <assert.h>
#include <atlbase.h>

#include "diacreate.h"
#include "cvconst.h"

//------------------------------------------------------------------------------------------------------
extern unsigned FEX_end_esp;
//------------------------------------------------------------------------------------------------------
int FEX_GetCallStackEBP(unsigned* CallStack, int MaxDepth)
{
	// - get the current EBP
	unsigned currEBP;
	_asm { 
		mov eax,ebp 
		mov currEBP,eax
	};
	if(FEX_end_esp){
		int tt=0;
	}
	unsigned tempstack[256];
	int i;
	for(i = 0; i < 128/* && currEBP <= FEX_end_esp*/; i++)
	{
		unsigned retAddr;
		if(IsBadReadPtr((unsigned*)currEBP + 1,4))break;
		retAddr = *((unsigned*)currEBP + 1);
		tempstack[i] = retAddr;
		// - get the prev EBP
		if(IsBadReadPtr((unsigned*)currEBP,4))break;
		currEBP = *((unsigned*)currEBP);
	}
	if(i>MaxDepth){
        memcpy(CallStack,tempstack+(i-MaxDepth),MaxDepth<<2);
		i=MaxDepth;
	}else memcpy(CallStack,tempstack,i<<2);
	return i;
}
int FEX_GetCallStackEBP_Ex(unsigned currEBP,unsigned* CallStack, int MaxDepth)
{
	if(FEX_end_esp){
		int tt=0;
	}
	int i;
	for(i = 0; i < MaxDepth/* && currEBP <= FEX_end_esp*/; i++)
	{
		unsigned retAddr;
		if(IsBadReadPtr((unsigned*)currEBP + 1,4))break;
		retAddr = *((unsigned*)currEBP + 1);
		CallStack[i] = retAddr;
		// - get the prev EBP
		if(IsBadReadPtr((unsigned*)currEBP,4))break;
		currEBP = *((unsigned*)currEBP);
	}
	return i;
}
//------------------------------------------------------------------------------------------------------
//void Fatal(const char *msg)
//{
	///MessageBox(NULL, msg, "DIA fatal error", MB_OK);
   ///exit(-1);
//}
#define Fatal(x) return false;
//------------------------------------------------------------------------------------------------------
static unsigned char _my_str[1024];
unsigned char* BSTR2STR(wchar_t* str) 
{
	for(int i = 0; i < 1024-1; i++)
	{
		wchar_t wch = str[i];
		_my_str[i] = (unsigned char)wch;
		if(wch == 0) break;
	}
	 _my_str[1023] = 0;
	return _my_str;
}
//------------------------------------------------------------------------------------------------------
#define freeString(s) LocalFree(s); s = NULL;

struct OneModuleDebugInfo{
	CComPtr<IDiaSession> pDiaSession;
	CComPtr<IDiaSymbol> pDiaSymbolGlobal;
	int BaseAdress;
	char ModuleName[128];
};

class DebugInfo{
public:
	bool IsInit;
	OneModuleDebugInfo* Modules[256];
	int	NModules;

	void CreateDebugInfo();
	bool GetDebugInfo(unsigned Address, unsigned* pLineNum, char* ppFunc, char* ppSrc);

	DebugInfo(){
		IsInit=false;
		NModules=0;
	}
};

void DebugInfo::CreateDebugInfo(){
	if(IsInit)return;
	IsInit=true;	

	//- enumerate loaded modules --------------------------------------------------			
	int type = 0, size = 0, modules = 0, addr = 0;

	char sysdir[256];
	GetSystemDirectory(sysdir, _MAX_PATH+1);
	int len1 = strlen(sysdir);

	HANDLE hModuleSnap = NULL; 
	MODULEENTRY32 me32 = {0}; 
	me32.dwSize = sizeof(MODULEENTRY32);

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	if(hModuleSnap != INVALID_HANDLE_VALUE){
		if(Module32First(hModuleSnap, &me32)){ 
			do {				
				char* str2 = me32.szExePath;
				int len2 = strlen(str2);
				int len = len1 > len2 ? len2 : len1;
				// - если начало строки совпало с системным каталогом, 
				// то это - системная либа
				if(memicmp(sysdir, str2, len) != 0){
					CComPtr<IDiaDataSource> pSource;
					HRESULT hr = NoRegCoCreate("msdia20.dll", _uuidof(DiaSourceAlt), _uuidof(IDiaDataSource), (void**)&pSource);
					if(hr==S_OK){
						OneModuleDebugInfo* di=new OneModuleDebugInfo;
						di->BaseAdress=int(me32.modBaseAddr);
						strcpy(di->ModuleName,str2);					

						wchar_t name[256];
						memset(name,0,sizeof name);
						for(int i=0;i<len2;i++)name[i]=str2[i];

						wchar_t* wszModuleFileName = &name[0];

						if(!pSource->loadDataForExe(wszModuleFileName, L".", NULL)){
							if(!pSource->openSession(&di->pDiaSession)){
								if(!di->pDiaSession->get_globalScope(&di->pDiaSymbolGlobal)){
									Modules[NModules++]=di;
									di=NULL;
								}
							}
						}
						if(!di)delete(di);
					}
				}
			} while(Module32Next(hModuleSnap, &me32));
		}
		CloseHandle(hModuleSnap);
	}
}

bool DebugInfo::GetDebugInfo(unsigned Address, unsigned* pLineNum, char* ppFunc, char* ppSrc){
	for(int i=0;i<NModules;i++){
		OneModuleDebugInfo* di=Modules[i];
		ULONGLONG rva = Address-di->BaseAdress;
		IDiaSymbol* pFunc;
		if(SUCCEEDED(di->pDiaSession->findSymbolByRVA(rva, SymTagFunction, &pFunc)) && pFunc)
		{
			// - get the function name
			BSTR name;
			if(SUCCEEDED(pFunc->get_name(&name))){
				unsigned char* str = BSTR2STR(name);
				freeString(name);		
				strcpy(ppFunc, (char*)str);
			}
			ULONGLONG length = 0;
			DWORD isect = 0;
			DWORD offset = 0;
			pFunc->get_addressSection(&isect);
			pFunc->get_addressOffset(&offset);
			pFunc->get_length(&length);
			if(isect != 0 && length > 0) 
			{
				CComPtr<IDiaEnumLineNumbers> pLines;
				if(SUCCEEDED(di->pDiaSession->findLinesByAddr(isect, offset, static_cast<DWORD>(length), &pLines))) 
				{
					CComPtr<IDiaLineNumber> pLine;
				DWORD celt;
				if(SUCCEEDED(pLines->Next(1, &pLine, &celt)) && celt == 1)
					{
						BSTR name = NULL;
						CComPtr<IDiaSourceFile> pSrc;
						pLine->get_sourceFile(&pSrc);
						if(pSrc)pSrc->get_fileName(&name);
						if(!name)name=L"???";

						unsigned char* str = BSTR2STR(name);
						freeString(name);
						strcpy(ppSrc, (char*)str);
					}
				pLine = NULL;
			}
			}
		}

		IDiaEnumLineNumbers* pLines;
		if(SUCCEEDED(di->pDiaSession->findLinesByRVA(rva, static_cast<DWORD>(1), &pLines)))
		{
			CComPtr<IDiaLineNumber> pLine;
		DWORD celt;
		if(SUCCEEDED(pLines->Next(1, &pLine, &celt)) && celt == 1)
			{
			DWORD linenum;
				BSTR name;
				CComPtr<IDiaSourceFile> pSrc;
				pLine->get_lineNumber(&linenum);
				*pLineNum = linenum;
			}
		}		
	}
	return true;
}
bool FEX_isDIAInit = false;
DebugInfo DebInf;
//------------------------------------------------------------------------------------------------------
bool FEX_GetDebugInfo(unsigned Base, unsigned Address, unsigned* pLineNum, char* ppFunc, char* ppSrc)
{
	*pLineNum = -1;

	DebInf.CreateDebugInfo();
	DebInf.GetDebugInfo(Address,pLineNum,ppFunc,ppSrc);

	return true;}
