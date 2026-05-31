//------------------------------------------------------------------------------------------------------
#include "FSampProfiler.h"
//------------------------------------------------------------------------------------------------------
#define freeString(s) LocalFree(s); s = NULL;
//------------------------------------------------------------------------------------------------------
void FSampProfiler::Fatal(const char *msg)
{
	MessageBox(NULL, msg, "Fatal error", MB_OK);
	exit(-1);
}
//------------------------------------------------------------------------------------------------------
void FSampProfiler::RegisterMissedSample(unsigned Address)
{
	unsigned* Data = (unsigned*)ModulesStream.Data;
	int Number = ModulesStream.Size/(4*3);

	for(int i = 0; i < Number; i++)
	{
		unsigned Base = Data[i*3+0];
		unsigned Size = Data[i*3+1];
		// - ищем модуль, в котором было прерывание
		if(Address >= Base && Address < Base + Size)
		{
			// - нашли модуль
			FSample smp;
			smp.Count = 0;
			smp.CallStackDepth = 1;
			smp.CallStack[0] = Base;
			FTernTreeNode* t = Insert(&smp);
			t->Sample->Count++;
			break;
		}
	}
	MissedSamples++;
}
//------------------------------------------------------------------------------------------------------
void* FSampProfiler::GetCurrentModuleBase(void)
{
	return (void*)0x400000;
//	MODULEINFO modinfo;
//	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &modinfo, sizeof(modinfo));
//	void* Base = modinfo.lpBaseOfDll;
//	return Base;
}
//------------------------------------------------------------------------------------------------------
int FSampProfiler::GetCallStackByEBP(unsigned* CallStack, int MaxDepth, unsigned _ebp)
{
	// - get the current EBP
	unsigned currEBP = _ebp;
	if(currEBP == 0)
	{
		_asm { 
			mov eax,ebp 
			mov currEBP,eax
		};
	}

	int Depth = 0;
	for(int i = 0; i < MaxDepth && i < MaxCallStackDepth; i++)
	{
		unsigned* p_prevEBP = *(unsigned**)&currEBP;
		unsigned* p_retAddr = (*(unsigned**)&currEBP + 1);
		if(IsBadReadPtr(p_prevEBP, 4)) break;
		if(IsBadReadPtr(p_retAddr, 4)) break;

		// - get the return address
		CallStack[i] = *p_retAddr;
		Depth++;
		// - get the prev EBP
		currEBP = *p_prevEBP;
	}
	return Depth;
}
//-----------------------------------------------------------------------------------
bool FSampProfiler::InitDIA(void)
{
	HRESULT hr = NoRegCoCreate("msdia20.dll", _uuidof(DiaSourceAlt), _uuidof(IDiaDataSource), (void**)&pSource);
	if(FAILED(hr))
		Fatal("Could not CoCreate CLSID_DiaSource(Alt). Register msdia20.dll or add its location to your PATH." );

	// - байтовая строка
	char szFilename[_MAX_PATH+1];	
	// - получим имя текущего модуля
	GetModuleFileName(GetModuleHandle(NULL), szFilename, _MAX_PATH);
	// - строка с расширенным набором символов
	wchar_t wszFilename[_MAX_PATH];  
	// - перевод строки в расширенную форму
	mbstowcs(wszFilename, szFilename, sizeof(wszFilename));
	
	// - грузим файл
	if(FAILED(pSource->loadDataForExe(wszFilename, NULL, NULL))) 
		Fatal("loadDataFromPdb/Exe");
	
	// - открываем сессию
	if(FAILED(pSource->openSession(&pDiaSession))) 
		Fatal("openSession");
	return true;
}
//------------------------------------------------------------------------------------------------------
char* FSampProfiler::GetFunctionByRVA(unsigned rva)
{
	static char szFunc[1024];
	szFunc[0] = 0;

	// - инициализация DIA
	if(!isDIAInitialized)
	{
		InitDIA();
		isDIAInitialized = true;
	}

	// - в роли объекта-функции выступает IDiaSymbol
	IDiaSymbol* pFunc;
	// - получаем функцию в виде символа по ее относительному виртуальному адресу
	if(SUCCEEDED(pDiaSession->findSymbolByRVA(rva, SymTagFunction, &pFunc)) && pFunc)
	{
		// - получаем имя функции
		BSTR name;
		pFunc->get_name(&name);
		
		// - преобразование формата с помощью sprintf
		sprintf(szFunc, "%ws", name);
	
		// - освобождаем строку
		freeString(name);
	}
	return szFunc;
}
//------------------------------------------------------------------------------------------------------
int FSampProfiler::FFunction_compare(const void *arg1, const void *arg2)
{
	FFunction* f1 = *(FFunction**)arg1;
	FFunction* f2 = *(FFunction**)arg2;

	if(f1->Base > f2->Base) return 1;
   if(f1->Base < f2->Base) return -1;
	return 0;
}
//------------------------------------------------------------------------------------------------------
bool FSampProfiler::DumpFunctions(char* RootFunctionName)
{
	// - инициализация DIA
	if(!isDIAInitialized)
	{
		InitDIA();
		isDIAInitialized = true;
	}
	RootFunctionIdx = -1;
	RootFunctionBase = 0;

	void* Base = GetCurrentModuleBase();
	f_FOutStream FuncStream;
	FuncStream.reload(30000*8);

	HRESULT hr;
    ULONG celt = 0;
	CComPtr<IDiaSymbol> pSymbol;
	CComPtr<IDiaEnumTables> pTables;
    if(FAILED(pDiaSession->getEnumTables(&pTables)))
		Fatal("getEnumTables");
   
	CComPtr<IDiaTable>pTable;	

	#define HashSz 2048
	DWORD Hash[HashSz];
	DWORD* Pool=new DWORD[HashSz*128];
	int PoolSize=HashSz*128;
	int PoolPos=0;
	memset(Hash,0,sizeof Hash);

    while(SUCCEEDED(hr = pTables->Next(1, &pTable, &celt)) && celt == 1) 
	{
      CComPtr<IDiaEnumSymbols> pSymbols;
      if(SUCCEEDED(pTable->QueryInterface(_uuidof(IDiaEnumSymbols), (void**)&pSymbols))) 
		{
			CComPtr<IDiaSymbol> pSymbol;
         while(SUCCEEDED(hr = pSymbols->Next(1, &pSymbol, &celt)) && celt == 1) 
			{
				DWORD symIndex;
            pSymbol->get_symIndexId(&symIndex);
            DWORD symTag;
            pSymbol->get_symTag(&symTag);
            if(symTag == SymTagFunction) 
				{
					ULONGLONG length = 0;
					DWORD rva;
					pSymbol->get_relativeVirtualAddress(&rva);
					DWORD va = *(unsigned*)&Base + rva;
					pSymbol->get_length(&length);
					if(va != 0 && length > 0) 
					{
						BSTR name;
						if(pSymbol->get_name(&name) == S_OK && name != NULL) 
						{
							bool isPresent = false;
							FFunction** Data = (FFunction**)FuncStream.Data;
							/*for(unsigned i = 0; i < FuncStream.Size/sizeof(FFunction*); i++)
							{
								if(Data[i]->Base == va)
								{
									isPresent = true;
									break;
								}
							}*/
							int p=(DWORD(va)>>4)&(HashSz-1);
							if(Hash[p]){
								DWORD* pp=Pool+Hash[p];
								while(pp){
									if(pp[0] == va){
                                        isPresent=true;
										break;
									}
									if(pp[1]){
                                        pp=Pool+pp[1];
									}else break;
								}
							}
							if(!isPresent)
							{
								//add to hash
								if(PoolPos+16>PoolSize){
									int pp=PoolSize;
									PoolSize+=PoolSize/5;
									DWORD* B1=new DWORD[PoolSize];
									memcpy(B1,Pool,pp*4);
									Pool=B1;
								}								
								if(Hash[p]){
									DWORD* pp=Pool+Hash[p];
									Pool[PoolPos+1]=pp[1];
									Pool[PoolPos]=(DWORD)va;
									Hash[p]=PoolPos;
									PoolPos+=2;
								}else{
									Pool[PoolPos+1]=0;
									Pool[PoolPos]=(DWORD)va;
									Hash[p]=PoolPos;
									PoolPos+=2;
								}
								//---------//
								FFunction* f = newFFunction();
								f->Base = va;
								f->Len = (unsigned)length;
								FuncStream.putint(*(unsigned*)&f);
								static char buf[1024];
								sprintf(buf, "%ws", name);
								if(strcmp(RootFunctionName, buf) == 0)
								{
									assert(RootFunctionBase == NULL);
									RootFunctionBase = va;
								}
							}
		  	            freeString(name);
						}
               } 
				} 
            pSymbol = NULL;
         }
      } 
      pTable = NULL;
   }
	Functions = (FFunction**)FuncStream.Data;
	FunctionsNumber = FuncStream.Size/sizeof(FFunction*);
	FuncStream.drop();
	delete(Pool);
	return true;
}
//------------------------------------------------------------------------------------------------------
void FSampProfiler::EnumerateLoadedModules(void)
{
	// - сохраняем список модулей задачи
	HANDLE hModuleSnap = NULL; 
	MODULEENTRY32 me32 = {0}; 
	me32.dwSize = sizeof(MODULEENTRY32); 
 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	if(hModuleSnap != INVALID_HANDLE_VALUE)
	{
		if(Module32First(hModuleSnap, &me32)) 
		{ 
			do { 
				int len = strlen(me32.szExePath)+1;
				char* str = new char[len];
				strcpy(str, me32.szExePath);
				ModulesStream.putint((unsigned)(me32.modBaseAddr));
				ModulesStream.putint((unsigned)(me32.modBaseSize));
				ModulesStream.putint((unsigned)(str));
				printf("0x%08x %08u %s\n", me32.modBaseAddr, me32.modBaseSize, me32.szExePath);
			} while(Module32Next(hModuleSnap, &me32)); 
		} 
		CloseHandle(hModuleSnap); 
	}
}
//------------------------------------------------------------------------------------------------------
void DumpFunctions(){
	CComPtr<IDiaDataSource> pSource;
	CComPtr<IDiaSession> pDiaSession; 

	ULONG celt = 0;
	HRESULT hr = NoRegCoCreate("msdia20.dll", _uuidof(DiaSourceAlt), _uuidof(IDiaDataSource), (void**)&pSource);
	if(FAILED(hr))return;

	//if(FAILED(pSource->loadDataFromPdb(L"cossacks2.pdb"))) return;	
	if(FAILED(pSource->loadDataForExe(L"engine.exe", NULL, NULL)))return;

	if(FAILED(pSource->openSession(&pDiaSession))) return;
	CComPtr<IDiaSymbol> pSymbol;
	CComPtr<IDiaEnumTables> pTables;
	if(FAILED(pDiaSession->getEnumTables(&pTables)))return;
	CComPtr<IDiaTable>pTable;
	DWORD Base=0x400000;
	FILE* F=fopen("dump.txt","w");
	while(SUCCEEDED(hr = pTables->Next(1, &pTable, &celt)) && celt == 1) 
	{
		CComPtr<IDiaEnumSymbols> pSymbols;
		if(SUCCEEDED(pTable->QueryInterface(_uuidof(IDiaEnumSymbols), (void**)&pSymbols))) 
		{
			CComPtr<IDiaSymbol> pSymbol;
			while(SUCCEEDED(hr = pSymbols->Next(1, &pSymbol, &celt)) && celt == 1) 
			{
				DWORD symIndex;
				pSymbol->get_symIndexId(&symIndex);
				DWORD symTag;
				pSymbol->get_symTag(&symTag);
				if(symTag == SymTagFunction) 
				{
					ULONGLONG length = 0;
					DWORD rva;
					pSymbol->get_relativeVirtualAddress(&rva);
					DWORD va = *(unsigned*)&Base + rva;
					pSymbol->get_length(&length);
					if(va != 0 && length > 0) 
					{
						BSTR name;
						if(pSymbol->get_name(&name) == S_OK && name != NULL) 
						{
							unsigned char* BSTR2STR(wchar_t* str); 
							fprintf(F,"%s A=%08X L=%d\n",BSTR2STR(name),va,length);
						}
					} 
				} 
				pSymbol = NULL;
			}
		} 
		pTable = NULL;
	}
	fclose(F);
}
