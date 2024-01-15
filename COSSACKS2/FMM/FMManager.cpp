#include <windows.h>
#include <stdio.h>              
#include <stdlib.h>             
#include <assert.h>             
#include "FMManager.h"
//
#define STRONGMEMORYCHECK
//#define STDMEMORY
//
//-----------------------------------------------------------------------------------
extern FMManager MManager;
//-----------------------------------------------------------------------------------
bool FMManager::isInit = false;
bool FMManager::isFree = false;
#ifdef FMM_LOCKING_ENABLE
	bool FMManager::isLockingEnable = !true;
#endif
//-----------------------------------------------------------------------------------

#define STARFORCE

//#ifdef STARFORCE
//GLOBAL PTR_AI=NULL;
//#endif
#define INMUTEX/* \
	HANDLE HMUT=CreateMutex(0,0,"FMM_MUTEX");\
	WaitForSingleObject(HMUT,INFINITE);*/
#define OUTMUTEX /*ReleaseMutex(HMUT);CloseHandle(HMUT);*/
bool FMManager::Initialize(unsigned totalSize)
{
#ifndef STDMEMORY 
	if(isInit) 
		return true;
	hMutex=CreateMutex(0,1,"fex_memory_manager");
	int err=GetLastError();
	assert(hMutex!=INVALID_HANDLE_VALUE);
	ZeroMemory(&HeapBlock, sizeof(HeapBlock));
	ZeroMemory(&MemoryStatus, sizeof(MemoryStatus));
	ZeroMemory(HashTableA, sizeof(HashTableA));
	ZeroMemory(HashTableS, sizeof(HashTableS));
	InvalidHandlers = NULL;
	Handlers = NULL;
	handlers = 0;
	allocatedBlocks = 0;
	HeapEnd = NULL;
	PageSize = 0;
	PhysSize = 0;
	isAllocateAllPages = true;

	FMM_LOG_OPEN();

	SYSTEM_INFO sSysInfo; 
	GetSystemInfo(&sSysInfo);
	GlobalMemoryStatus(&MemoryStatus);

	PageSize = sSysInfo.dwPageSize;
	PhysSize = (unsigned)((unsigned __int64)MemoryStatus.dwAvailPhys*FMM_PHYSMEM_PERCENT)/100;

	HeapBlock.BaseAddress = NULL;
	while(totalSize >= FMM_MINMEM_RESERVE)
	{
		HeapBlock.BaseAddress = 
			(FMPTR)VirtualAlloc(NULL, totalSize, MEM_RESERVE, PAGE_NOACCESS);
		if(!HeapBlock.BaseAddress)
			totalSize -= 1024*1024*20;
		else
			break;
	}
	if(!HeapBlock.BaseAddress)
	{
		FMM_ERROR("VirtualAlloc heap reserve failed\nNot enought memory to run program");
		return false;
	}
	HeapBlock.TotalSize = totalSize;
	HeapBlock.TotalPages = (HeapBlock.TotalSize + PageSize-1)/PageSize;
	HeapEnd = HeapBlock.BaseAddress;

	// - распределяем блок хендлов
	unsigned hSize = FMM_MAXHANDLERS*sizeof(Handlers[0]);
	Handlers = (FMMHANDLER*)VirtualAlloc(NULL, hSize, MEM_COMMIT, PAGE_READWRITE);
	
	HANDLERS_OPEN();
	{
		ZeroMemory(Handlers, hSize);
		handlers = 1;
		Handlers[0].BaseAddress = HeapBlock.BaseAddress;
		Handlers[0].TotalSize = HeapBlock.TotalSize;
		Handlers[0].Next = NULL;
		Handlers[0].Prev = NULL;
		Handlers[0].SubNextA = NULL;
		Handlers[0].SubNextS = NULL;
		Handlers[0].SubPrevS = NULL;
		Handlers[0].SubPrevA = NULL;
		Handlers[0].isFree = true;
		FMMADDHASHS(Handlers);
	}
	HANDLERS_CLOSE();

	isInit = true;
#endif //STDMEMORY
	return true;
}
//-----------------------------------------------------------------------------------
bool FMManager::Release(void)
{
#ifndef STDMEMORY
	if(!isInit) 
		return false;
	FMM_LOG_CLOSE();
	isInit=false;
	return !!VirtualFree((void*)HeapBlock.BaseAddress, 0, MEM_RELEASE);
#else //STDMEMORY
	return true;
#endif //STDMEMORY
}
//-----------------------------------------------------------------------------------
// Logging routines
//-----------------------------------------------------------------------------------
bool FMManager::FMM_LOG_OPEN(void)
{
	#ifdef FMM_LOGGING_ENABLE
	if((logfile = fopen(FMM_LOGFILENAME, "wt")) == NULL)
	{
		MessageBox(NULL, FMM_LOGFILENAME, "Cannot open log file", MB_OK);
		ExitProcess(-1);
	}
	#endif
	return true;
}
//-----------------------------------------------------------------------------------
bool FMManager::FMM_LOG_CLOSE(void)
{
	#ifdef FMM_LOGGING_ENABLE
	if(!logfile)
		return false;
	fclose(logfile);
	#endif
	return true;
}
//-----------------------------------------------------------------------------------
void FMManager::FMM_ERROR(char* str, ...) 
{
	if(!str)	return;
	static char buf[1024];

	va_list args;
	va_start(args, str);
	sprintf(buf, str, args);
	va_end(args);
	MessageBox(NULL, buf, "FMMANAGER ERROR!", MB_OK);
}
//-----------------------------------------------------------------------------------
void FMManager::FMM_LOG(char* str, ...) 
{
	#ifdef FMM_LOGGING_ENABLE
	if(!str)	return;

	va_list args;
	va_start(args, str);
	vfprintf(logfile, str, args);
	vprintf(str, args);
	fflush(logfile);
	va_end(args);
	#endif
}
//-----------------------------------------------------------------------------------
// Allocation routines
//-----------------------------------------------------------------------------------
FMManager::FMMHANDLER* FMManager::GetFreeHandler(void)
{
	FMMHANDLER* h = NULL;
	if(InvalidHandlers)
	{
		h = InvalidHandlers;
		InvalidHandlers = InvalidHandlers->Next;
	}
	else
	{
		if(handlers >= FMM_MAXHANDLERS)
		{
			FMM_ERROR("There are too much handlers: %d!\n", handlers);
			return NULL;
		}
		h = &Handlers[handlers++];
		h->r2 = handlers-1;
	}
	#ifdef FMM_LOCKING_ENABLE
		h->Locked = 0;
	#endif
	return h;
}
//-----------------------------------------------------------------------------------
FMManager::FMMHANDLER* FMManager::GetHandlerByAddress(FMPTR addr)
{
	FMMHANDLER* _h = NULL;
	FMMHANDLER* h = NULL;

	// - ищем по адресу среди занятых блоков
	unsigned hash_idx = FMMHASHA(addr), chain_idx = 0;

	_h = HashTableA[hash_idx];
	while(_h)
	{
		if(_h->BaseAddress == addr)
		{
			h = _h;
			break;
		}
		_h = _h->SubNextA;
		chain_idx++;
	}
	if(!h)
	{
		FMM_LOG("Cannot find block with address 0x%08x!\n", addr);
		return NULL;
	}
	return h;
}
/*-----------------------------------------------------------------------------------
	Выделение хендла происходит путем нахождения хендла подходящего размера в цепочках
таблицы размеров, начиная с индекса, полученного из запрашиваемого размера.
	Нет необходимости выравнивания - до двух хендлов. Первый (левый) хендл становится 
выделенным, и он помещается в таблицу адресов. Второй (правый) хендл становится пустым 
и помещается в таблицу размеров.
/----------------------------------------------------------------------------------*/
FMManager::FMMHANDLER* FMManager::AddHandler(unsigned size)
{
	assert(handlers);

	FMMHANDLER* _h = NULL;
	FMMHANDLER* h = NULL;

	unsigned chain_idx = 0;

	// - ищем хендл блока достаточного размера
	for(unsigned i = FMMHASHS(size); i < FMM_MAXHASHITEMS; i++)
	{
		chain_idx = 0;
		_h = HashTableS[i];
		while(_h)
		{
			// - внутри цепочки, идущей из таблицы HashTableS, все хендлы пустые
			if(_h->TotalSize >= size)
			{
				h = _h;
				break;
			}
			_h = _h->SubNextS;
			chain_idx++;
		}
		if(h) break;
	}
	if(!h)
	{
		FMM_LOG("Cannot find block of %u bytes size!\n", size);
		return NULL;
	}
	// - требуется удалить найденный хендл из таблицы размеров
	DELETE_HS(h);

	if(h->TotalSize > size)
	{
		// - получим еще один хендл, который будет соответствовать остатку блока
		_h = GetFreeHandler();
		if(!_h) return NULL;
	
		_h->BaseAddress = h->BaseAddress + size;
		_h->TotalSize = h->TotalSize - size;
		_h->isFree = true;
		h->TotalSize = size;

		FMMHANDLER* h0 = h;
		FMMHANDLER* h1 = _h;
		FMMHANDLER* h2 = h->Next;
		
		h0->Next = h1;
		h1->Next = h2;
		h1->Prev = h0;
		if(h2)
			h2->Prev = h1;

		// - помещаем пустой хендл в таблицу размеров
		FMMADDHASHS(_h);
	}
	// - помещаем занятый хендл в таблицу адресов
	FMMADDHASHA(h);

	return h;
}
_inline void fast_memset(void* _ptr,DWORD _value,DWORD _size){
	__asm{
		push ecx
		push edi
		mov ecx,_size
		cld
		mov eax,_value
		mov edi,_ptr	
		shr ecx,2
		rep stosd
		pop edi
		pop ecx
	}
}
/*-----------------------------------------------------------------------------------
	Выделение хендла происходит путем нахождения хендла подходящего размера в цепочках
таблицы размеров, начиная с индекса, полученного из запрашиваемого размера.
Необходимо выравнивание - до трех хендлов. Первый свободный, второй выделенный, третий 
свободный.
/----------------------------------------------------------------------------------*/
FMManager::FMMHANDLER* FMManager::AddPageAlignedHandler(unsigned size)
{
	assert(handlers);

	size = ((size + PageSize-1)/PageSize)*PageSize;
	FMMHANDLER* _h = NULL;
	FMMHANDLER* h = NULL;

	unsigned chain_idx = 0;

	// - ищем хендл блока достаточного размера
	for(unsigned i = FMMHASHS(size); i < FMM_MAXHASHITEMS; i++)
	{
		chain_idx = 0;
		_h = HashTableS[i];
		while(_h)
		{
			// - внутри цепочки, идущей из таблицы HashTableS, все хендлы пустые
			if(_h->TotalSize >= ((unsigned)_h->BaseAddress)%PageSize + size)
			{
				h = _h;
				break;
			}
			_h = _h->SubNextS;
			chain_idx++;
		}
		if(h) break;
	}
	if(!h)
	{
		FMM_LOG("Cannot find aligned block of %u bytes size!\n", size);
		return NULL;
	}

	// - отделяем начало хендла в пустой хендл, если не выровнено начало на размер страницы PageSize
	unsigned delta_size = (PageSize - ((unsigned)h->BaseAddress)%PageSize)%PageSize;
	if(delta_size)
	{
		// - создаем новый хендл, выровненый по границе PageSize
		_h = GetFreeHandler();
		if(!_h) return NULL;
	
		_h->BaseAddress = h->BaseAddress + delta_size;
		_h->TotalSize = h->TotalSize - delta_size;
		_h->isFree = true;
		h->TotalSize = delta_size;

		FMMHANDLER* h0 = h;
		FMMHANDLER* h1 = _h;
		FMMHANDLER* h2 = h->Next;
		
		h0->Next = h1;
		h1->Next = h2;
		h1->Prev = h0;
		if(h2)
			h2->Prev = h1;

		// - помещаем пустой хендл в таблицу размеров
		FMMADDHASHS(_h);
		h = _h;
	}
	assert(((unsigned)h->BaseAddress)%PageSize == 0);

	// - требуется удалить найденный хендл из таблицы размеров
	DELETE_HS(h);

	// - имеем выровненый по границе PageSize хендл, с размерами >= size
	if(h->TotalSize > size)
	{
		// - получим еще один хендл, который будет соответствовать остатку блока
		_h = GetFreeHandler();
		if(!_h) return NULL;
	
		_h->BaseAddress = h->BaseAddress + size;
		_h->TotalSize = h->TotalSize - size;
		_h->isFree = true;
		h->TotalSize = size;

		FMMHANDLER* h0 = h;
		FMMHANDLER* h1 = _h;
		FMMHANDLER* h2 = h->Next;
		
		h0->Next = h1;
		h1->Next = h2;
		h1->Prev = h0;
		if(h2)
			h2->Prev = h1;

		// - помещаем пустой хендл в таблицу размеров
		FMMADDHASHS(_h);
	}
	// - помещаем занятый хендл в таблицу адресов
	FMMADDHASHA(h);	
	return h;
}
//-----------------------------------------------------------------------------------
FMPTR FMManager::Allocate(int size)
{	
	allocatedBlocks++;
#ifdef STDMEMORY
	return (FMPTR)malloc((size+15)&0xFFFFFF0);
#else //STDMEMORY 
	if(!isInit && !Initialize())
		return NULL;

	INMUTEX;
	if(!size)
	{
		FMM_LOG("Cannot allocate 0 bytes!\n");
		OUTMUTEX;
		return NULL;
	}
	if(HeapBlock.AllocatedSize + size > HeapBlock.TotalSize) 
	{
		FMM_ERROR("Cannot allocate %u bytes! No free memory!\n", size);
		FMM_ERROR("Memory allocated %u\n", HeapBlock.AllocatedSize);
		OUTMUTEX;
		return NULL;
	}
	size = (size+0x0f)&~0x0f;

	FMMHANDLER* h;
	#ifdef FMM_LOCKING_ENABLE
		if(isLockingEnable)
			h = AddPageAlignedHandler(size);
		else
			h = AddHandler(size);
	#else
		h = AddHandler(size);
	#endif

	if(!h)
	{
		OUTMUTEX;
		return NULL;
	}	
	FMPTR addr1 = h->BaseAddress;
	FMPTR addr2 = addr1 + h->TotalSize;

	if(HeapEnd < addr2) HeapEnd = addr2;
	HeapBlock.AllocatedSize += size;

	SetAccess(addr1, addr2-addr1, PAGE_EXECUTE_READWRITE);

	#ifdef FMM_LOCKING_ENABLE
		if(isLockingEnable)
			Lock(addr1);
	#endif

	FMM_LOG("%u bytes allocated\n", size);
	OUTMUTEX;
	if(size)fast_memset(h->BaseAddress,0,(size+15)&0xFFFFFFF0);
	return h->BaseAddress;
#endif //STDMEMORY
}
/*-----------------------------------------------------------------------------------
	Нахождение удаляемого хендла происходит путем поиска хендла с заданным адресом
в цепочке таблицы адресов, адресуемой хеш-значением заданного адреса. При нахождении 
хендла, память освобождается, и производится удаление свободных соседей данного хендла.
Удаляемые хендлы становятся инвалидными и помещаются в список инвалидных хендлов.
/----------------------------------------------------------------------------------*/
bool FMManager::Deallocate(FMPTR addr)
{
	allocatedBlocks--;
#ifdef STDMEMORY
	free(addr);
	return true;
#else //STDMEMORY
	if(!isInit) return false;
	INMUTEX;
	FMMHANDLER* h = GetHandlerByAddress(addr);
	if(!h)
	{
		FMM_LOG("Cannot find block with address 0x%08x!\n", addr);
		OUTMUTEX;
		return false;
	}
	FMM_LOG("Deallocated %u bytes from address 0x%08x\n", h->TotalSize, addr);

	// - требуется удалить найденный хендл из таблицы адресов
	DELETE_HA(h);

	// - освободим память и установим атрибуты страниц
	FMPTR addr1 = h->BaseAddress;
	FMPTR addr2 = addr1 + h->TotalSize;

	if(HeapEnd == addr2) HeapEnd = addr1;
	HeapBlock.AllocatedSize -= h->TotalSize;

	FMMHANDLER* h0 = h->Prev;
	FMMHANDLER* h1 = h;
	FMMHANDLER* h2 = h->Next;

	//------------- free section ---- it possible that it is better without it -----------
	/*

	// - номер первой занимаемой страницы
	int page1 = (int)((addr1 - HeapBlock.BaseAddress)/PageSize);
	// - номер последней занимаемой страницы
	// - отнимаем 1, (блок > 0) для того, чтобы получить корректный номер страницы
	int page2 = (int)((addr2 - HeapBlock.BaseAddress - 1)/PageSize);

	FMMHANDLER* hPrev = h0;
	FMMHANDLER* hNext = h2;
	bool isLUse = false;
	bool isRUse = false;
	while(hPrev)
	{
		// - получим номер страницы, последней для левого блока
		int page = (int)(hPrev->BaseAddress + hPrev->TotalSize - 
				  			  HeapBlock.BaseAddress - 1)/PageSize;
		// - если вышли за пределы левой	страницы, выходим
		if(page < page1) break;
		// - если на этой странице есть непустой блок, то страница занята
		if(!hPrev->isFree) 
		{
			isLUse = true;
			break;
		}
		hPrev = hPrev->Prev;
	}
	while(hNext)
	{
		// - получим номер страницы, первой для правого блока
		int page = (int)(hNext->BaseAddress - HeapBlock.BaseAddress)/PageSize;
		// - если вышли за пределы правой страницы, выходим
		if(page > page2) break;
		// - если на этой странице есть непустой блок, то страница занята
		if(!hNext->isFree) 
		{
			isRUse = true;
			break;
		}
		hNext = hNext->Next;
	}

	if(isLUse)
		page1++;
	if(isRUse)
		page2--;
	
	if(page1 >= 0 && page2 >= page1)
	{
		// - сюда мы попадаем только в случае, если надо освободить хоть одну страницу!
		if(!VirtualFree((void*)(page1*PageSize + HeapBlock.BaseAddress), 
														(page2-page1)*PageSize+1, MEM_DECOMMIT))
		{
			//FMM_MESSAGE("Cannot decommit pages from %u to %u!\n", page1, page2);
			//return false;
		}
	}
	*/
	//------------------------------------------------------------------------------------
#ifdef STRONGMEMORYCHECK
	memset(h1->BaseAddress,0xAB,h1->TotalSize);
#endif //STRONGMEMORYCHECK

	if(h0 && h0->isFree)
	{
		// - удаляем предыдущий хендл (Prev)
		h1->TotalSize += h0->TotalSize;
		h1->BaseAddress = h0->BaseAddress;

		DELETE_HS(h0);
		DELETE_H(h0);
	}
	if(h2 && h2->isFree)
	{
		// - удаляем следующий хендл (Next)
		h1->TotalSize += h2->TotalSize;

		DELETE_HS(h2);
		DELETE_H(h2);
	}
	// - добавляем хендл в таблицу размеров
	FMMADDHASHS(h1);

#ifdef STRONGMEMORYCHECK	
	DWORD page1 = DWORD(h1->BaseAddress)/PageSize;
	DWORD pp1   = DWORD(h1->BaseAddress)%PageSize;
	if(pp1)page1++;
	DWORD page2 = DWORD(h1->BaseAddress+h1->TotalSize)/PageSize-1;
	if(page2>=page1){
		VirtualFree((void*)(page1*PageSize),(page2-page1+1)*PageSize, MEM_DECOMMIT);
	}
#endif //STRONGMEMORYCHECK

	OUTMUTEX;
	return true;
#endif //STDMEMORY
}
//-----------------------------------------------------------------------------------
// Access routines
//-----------------------------------------------------------------------------------
bool FMManager::SetAccess(FMPTR base, unsigned size, unsigned mode)
{
#ifndef STDMEMORY
	if(!VirtualAlloc(base, size, MEM_COMMIT, mode))
	{
		FMM_ERROR("Cannot commit memory at 0x%08x size 0x%08x!\n", base, size);
		return false;
	}
#endif //STDMEMORY
	return true;
}
//-----------------------------------------------------------------------------------
#ifdef FMM_LOCKING_ENABLE
int FMManager::Lock(FMPTR addr)
{
	if(!isInit || !isLockingEnable) return false;
	INMUTEX;
	FMMHANDLER* h = GetHandlerByAddress(addr);
	if(!h)
	{
		FMM_LOG("Cannot find block with address 0x%08x!\n", addr);
		OUTMUTEX;
		return -1;
	}
	assert(h->BaseAddress == addr);
	if(((unsigned)h->BaseAddress)%PageSize || h->TotalSize%PageSize)
	{
		FMM_LOG("Cannot lock block whith address 0x%08x and size %u!\n", addr, h->TotalSize);
		OUTMUTEX;
		return -1;
	}
	FMM_LOG("Locking block of %u bytes at address 0x%08x\n", h->TotalSize, h->BaseAddress);
	h->Locked++;
	if(!SetAccess(h->BaseAddress, h->TotalSize, PAGE_NOACCESS))
	{
		OUTMUTEX;
		return -1;
	}
	OUTMUTEX;
	return h->Locked;
}
#endif
//-----------------------------------------------------------------------------------
#ifdef FMM_LOCKING_ENABLE
int FMManager::Unlock(FMPTR addr)
{
#ifndef STDMEMORY 
	if(!isInit || !isLockingEnable) return false;
	INMUTEX;
	FMMHANDLER* h = GetHandlerByAddress(addr);
	if(!h)
	{
		FMM_LOG("Cannot find block with address 0x%08x!\n", addr);
		OUTMUTEX;
		return -1;
	}
	assert(h->BaseAddress == addr);
	if(((unsigned)h->BaseAddress)%PageSize || h->TotalSize%PageSize)
	{
		FMM_LOG("Cannot unlock block whith address 0x%08x and size %u!\n", addr, h->TotalSize);
		OUTMUTEX;
		return -1;
	}
	FMM_LOG("Unlocking block of %u bytes at address 0x%08x\n", h->TotalSize, h->BaseAddress);

	h->Locked--;
	if(h->Locked < 0)
	{
		FMM_LOG("!Warning!: Unlocking block of %u bytes at address 0x%08x\n", h->TotalSize, h->BaseAddress);
		FMM_LOG("           Locking count is %d\n", h->Locked);
	}

	if(!SetAccess(h->BaseAddress, h->TotalSize, PAGE_EXECUTE_READWRITE))
	{
		OUTMUTEX;
		return -1;
	}
	OUTMUTEX;
	return h->Locked;
#endif STDMEMORY 
	return 0;
}
#endif
//-----------------------------------------------------------------------------------
// Additional routines
//-----------------------------------------------------------------------------------
bool FMManager::GetInfo(FMMINFO* m)
{
#ifndef STDMEMORY 
	m->AllocatedSize = HeapBlock.AllocatedSize;
	m->AllHandlers = handlers;
	m->AllocatedHandlers = 0;
	m->FreeHandlers = 0;

	unsigned i;
	for(i = 0; i < handlers; i++)
		Handlers[i].r2 = 0;

	for(i = 0; i < FMM_MAXHASHITEMS; i++)
	{
		FMMHANDLER* h = HashTableA[i];
		unsigned chain_idx = 0;
		while(h)
		{
			if(h->r2)
			{
				FMM_LOG("Cycling detected at HashTableA[%u] in the element number %u!\n", i, chain_idx);
				return false;
			}
			h->r2 = 1;
			m->AllocatedHandlers++;
			if(m->AllocatedHandlers > handlers)
			{
				FMM_LOG("Cycling detected at HashTableA[%u]!\n", i);
				return false;
			}
			chain_idx++;
			h = h->SubNextA;
		}
	}
	
	for(i = 0; i < FMM_MAXHASHITEMS; i++)
	{
		FMMHANDLER* h = HashTableS[i];
		unsigned chain_idx = 0;
		while(h)
		{
			if(h->r2)
			{
				FMM_LOG("Cycling detected at HashTableS[%u] in the element number %u!\n", i, chain_idx);
				return false;
			}
			h->r2 = 1;
			m->FreeHandlers++;
			if(m->FreeHandlers > handlers)
			{
				FMM_LOG("Cycling detected at HashTableS[%u]!\n", i);
				return false;
			}
			chain_idx++;
			h = h->SubNextS;
		}
	}

	m->TotalPages = HeapBlock.TotalPages;
	m->NonePages = 0;
	m->NewPages = 0;
	m->PresentPages = 0;
	m->CashedPages = 0;

	MEMORY_BASIC_INFORMATION Buffer;

	unsigned TotalPages = 0;
	unsigned NonePages = 0;
	unsigned PresentPages = 0;

	unsigned addr1 = (unsigned)HeapBlock.BaseAddress;
	unsigned addr2 = addr1 + HeapBlock.TotalSize;

	while(addr1 < addr2)
	{
		VirtualQuery((void*)addr1, &Buffer, sizeof(MEMORY_BASIC_INFORMATION));
		
		if(Buffer.RegionSize > HeapBlock.TotalSize)
			Buffer.RegionSize = HeapBlock.TotalSize;

		addr1 += Buffer.RegionSize;
		unsigned pages = (Buffer.RegionSize + PageSize-1)/PageSize;
		TotalPages += pages;

		if(Buffer.State == MEM_FREE || Buffer.State == MEM_RESERVE)
			NonePages += pages;
		else
		if(Buffer.State == MEM_COMMIT)
			PresentPages += pages;
	}
#endif STDMEMORY 
	return true;
}
//-----------------------------------------------------------------------------------
bool FMManager::SelfTest(void)
{
#ifndef STDMEMORY 
	unsigned valid_handlers = 0;
	unsigned invalid_handlers = 0;
	unsigned allocated_handlers = 0;
	unsigned free_handlers = 0;

	FMMHANDLER* start_h = NULL;
	FMMHANDLER* end_h = NULL;
	FMMHANDLER* min_h = NULL;

	FMPTR base = (FMPTR)0xffffffff;
	unsigned start_num = 0;	// - число стартовых элементов, должно быть 1
	unsigned end_num = 0;	// - число конечных элементов, должно быть 1

	for(unsigned i = 0; i < handlers; i++)
	{
		if(Handlers[i].BaseAddress <= base && Handlers[i].TotalSize)
		{
			base = Handlers[i].BaseAddress;
			min_h = &Handlers[i];
		}
		if(!Handlers[i].Prev && Handlers[i].TotalSize)
		{
			start_h = &Handlers[i];
			start_num++;
		}
		if(!Handlers[i].Next && Handlers[i].TotalSize)
		{
			end_h = &Handlers[i];
			end_num++;
		}
		Handlers[i].r2 = 0;
	}
	if(start_num != 1) 
	{
		FMM_LOG("ERROR: There are %u start points in global chain!\n", start_num);
		return false;
	}
	if(end_num != 1) 
	{
		FMM_LOG("ERROR: There are %u end points in global chain!\n", end_num);
		return false;
	}
	if(start_h != min_h)
	{
		FMM_LOG("ERROR: The start handler is have not minimum base address!\n");
		return false;
	}

	FMMHANDLER* h1 = start_h;
	while(h1)
	{
		FMMHANDLER* h2 = h1->Next;
		if(h2)
		{
			if(h2->Prev != h1)
			{
				FMM_LOG("ERROR: Global chain is corrupted!\n");
				return false;
			}
		}
		else
		if(h1 != end_h)
		{
			FMM_LOG("ERROR: End handler is have not maximum base address!\n");
			return false;
		}
		h1->r2 = 1;
		h1 = h2;
		valid_handlers++;
	}

	h1 = InvalidHandlers;
	while(h1)
	{
		FMMHANDLER* h2 = h1->Next;
		if(h2)
		{
			if(h2->Prev != h1)
			{
				FMM_LOG("ERROR: Invalid handlers chain is corrupted!\n");
				return false;
			}
		}
		if(h1->r2)
		{
			FMM_LOG("ERROR: There is collision between the global and the invalid handlers chains!\n");
			return false;
		}
		h1 = h2;
		invalid_handlers++;
	}
	if(invalid_handlers + valid_handlers != handlers)
	{
		FMM_LOG("ERROR: There are %u lost handlers!\n", handlers - invalid_handlers - valid_handlers);
		return false;
	}

	// - обнулим признаки вхождения
	for(int i = 0; i < handlers; i++)
		Handlers[i].r2 = 0;

	// - проверка цепочек размеров
	for(int i = 0; i < FMM_MAXHASHITEMS; i++)
	{
		h1 = HashTableS[i];
		while(h1)
		{
			FMMHANDLER* h2 = h1->SubNextS;
			if(h2)
			{
				if(h2->SubPrevS != h1)
				{
					FMM_LOG("ERROR: Sizes chain is corrupted!\n");
					return false;
				}
			}
			if(h1->r2)
			{
				FMM_LOG("ERROR: There is collision in the sizes chain!\n");
				return false;
			}
			h1->r2 = 1;
			h1 = h2;
			free_handlers++;
		}
	}
	// - проверка цепочек адресов
	for(int i = 0; i < FMM_MAXHASHITEMS; i++)
	{
		h1 = HashTableA[i];
		while(h1)
		{
			FMMHANDLER* h2 = h1->SubNextA;
			if(h2)
			{
				if(h2->SubPrevA != h1)
				{
					FMM_LOG("ERROR: Addresses chain is corrupted!\n");
					return false;
				}
			}
			if(h1->r2)
			{
				FMM_LOG("ERROR: There is collision in the addresses chain!\n");
				return false;
			}
			h1->r2 = 2;
			h1 = h2;
			allocated_handlers++;
		}
	}
	unsigned num = free_handlers + allocated_handlers + invalid_handlers;
	if(num != handlers)
	{
		FMM_LOG("ERROR: There are %u lost handlers!\n", handlers - num);
		return false;
	}
	
	FMM_LOG("Self test is OK!\n");
#endif //STDMEMORY
	return true;
}
void CalcFreeAndUsed(int& freemem,int& used){
	freemem=0;
	used=0;
	for(int i=0;i<MManager.handlers;i++){
		if(MManager.Handlers[i].isFree){
			int sz=MManager.Handlers[i].TotalSize;
			if(sz<50000000)freemem+=sz;
		}else used+=MManager.Handlers[i].TotalSize;
	}
}
bool CheckIfPtrExists(void* ptr){
	FMManager::FMMHANDLER* h = MManager.GetHandlerByAddress((FMPTR)ptr);
	return h!=NULL;
}