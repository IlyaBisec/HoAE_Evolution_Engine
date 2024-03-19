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

	for(int i = 0; i < MaxDepth/* && currEBP <= FEX_end_esp*/; i++)
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
int FEX_GetCallStackEBP_Ex(unsigned currEBP,unsigned* CallStack, int MaxDepth)
{
	if(FEX_end_esp){
		int tt=0;
	}

	for(int i = 0; i < MaxDepth/* && currEBP <= FEX_end_esp*/; i++)
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
CComPtr<IDiaSession> pDiaSession;
CComPtr<IDiaSymbol> pDiaSymbolGlobal;
bool FEX_isDIAInit = false;
//------------------------------------------------------------------------------------------------------
bool FEX_GetDebugInfo(unsigned Base, unsigned Address, unsigned* pLineNum, char* ppFunc, char* ppSrc)
{
	*pLineNum = -1;
	//*ppFunc = NULL;
	//*ppSrc = NULL;

	if(!FEX_isDIAInit)
	{
		CComPtr<IDiaDataSource> pSource;
		HRESULT hr = NoRegCoCreate("msdia20.dll", _uuidof(DiaSourceAlt), _uuidof(IDiaDataSource), (void**)&pSource);

		if(FAILED(hr))
			Fatal("Could not CoCreate CLSID_DiaSource(Alt). Register msdia20.dll or add its location to your PATH." );

		int NumArgs = 0;
		LPWSTR* ws = CommandLineToArgvW(L"", &NumArgs);
		wchar_t* wszModuleFileName = *ws;

		if(FAILED(pSource->loadDataForExe(wszModuleFileName, L".", NULL))) 
			Fatal("loadDataFromPdb/Exe");

		if(FAILED(pSource->openSession(&pDiaSession))) 
			Fatal("openSession");
	   if(FAILED(pDiaSession->get_globalScope(&pDiaSymbolGlobal)))
			Fatal("get_globalScope");

		FEX_isDIAInit = true;
	}   

	ULONGLONG rva = Address-Base;
	IDiaSymbol* pFunc;
	if(SUCCEEDED(pDiaSession->findSymbolByRVA(rva, SymTagFunction, &pFunc)) && pFunc)
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
			if(SUCCEEDED(pDiaSession->findLinesByAddr(isect, offset, static_cast<DWORD>(length), &pLines))) 
			{
				CComPtr<IDiaLineNumber> pLine;
		      DWORD celt;
			   if(SUCCEEDED(pLines->Next(1, &pLine, &celt)) && celt == 1)
				{
					BSTR name;
					CComPtr<IDiaSourceFile> pSrc;
					pLine->get_sourceFile(&pSrc);
					pSrc->get_fileName(&name);

					unsigned char* str = BSTR2STR(name);
					freeString(name);
					strcpy(ppSrc, (char*)str);
				}
            pLine = NULL;
         }
		}
	}

	IDiaEnumLineNumbers* pLines;
	if(SUCCEEDED(pDiaSession->findLinesByRVA(rva, static_cast<DWORD>(1), &pLines)))
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

	return true;
}
