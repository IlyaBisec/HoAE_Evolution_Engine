#ifndef FMMANAGER_H
#define FMMANAGER_H

#include <math.h>

// - включить/выключить режим протоколирования работы
//#define FMM_LOGGING_ENABLE

// - режим защиты означает выделение блока по адресу и размеру, кратным 4к в залоченном виде,
// для доступа необходимо вызвать Unlock(addr)
//#define FMM_LOCKING_ENABLE

#define FMM_LOGFILENAME "FMManager.log"
#define FMM_DEFAULTMEMSIZE (1024*1024*1024)
#define FMM_PHYSMEM_PERCENT 75
#define FMM_MAXHANDLERS	(1024*1024*2)
#define FMM_MAXHASHITEMSPOW 10
#define FMM_MAXHASHITEMS (1<<FMM_MAXHASHITEMSPOW)
#define FMM_MAXHASHITEMSMASK (FMM_MAXHASHITEMS-1)
#define FMM_MINMEM_RESERVE (1024*1024*350)
typedef unsigned char* FMPTR;

/*
	Хендл может находиться в трех состояниях:
1. Несуществующий (invalid)
2. Свободный
3. Выделенный

1. Инвалидные хендлы находятся в списке по указателям Prev/Next 
	с началом в InvalidHandlers. 
	Поля SubNext, SubPrev, BaseAddress, TotalSize - нулевые.

2. Свободные хендлы находятся в списке с выделенными по указателям Prev/Next 
	в расположении, соответствующем физическому расположению блоков памяти.
	Также свободные хендлы образуют цепочки, имеющие начала в таблице размеров
	HashTableS по указателям SubNextS/SubPrevS.

3. Выделенные хендлы находятся в списке со свободными по указателям Prev/Next.
	Также выделенные хендлы образуют цепочки, имеющие начала в таблице адресов
	HashTableA по указателям SubNextA/SubPrevA.

*/

struct FMMINFO {
	unsigned AllHandlers;
	unsigned FreeHandlers;
	unsigned AllocatedHandlers;
	unsigned AllocatedSize;
	unsigned TotalPages;
	unsigned NonePages;
	unsigned NewPages;
	unsigned PresentPages;
	unsigned CashedPages;
};

//-----------------------------------------------------------------------------------
class FMManager {
	// -------------------- structures definitions -----------------------------------
public:
	HANDLE hMutex;
	struct FMMHANDLER {
		FMPTR BaseAddress;
		unsigned TotalSize;
		FMMHANDLER* Next;
		FMMHANDLER* Prev;
		union {
			FMMHANDLER* SubNextA;
			FMMHANDLER* SubNextS;
		};
		union {
			FMMHANDLER* SubPrevA;
			FMMHANDLER* SubPrevS;
		};
		union {
			bool isFree;
			unsigned r1;
		};
		unsigned r2;
		#ifdef FMM_LOCKING_ENABLE
			int Locked;
		#endif
	};
//private:
public:
	struct VIRTUALBLOCK {
		FMPTR BaseAddress;		// - адрес всего виртуального блока 
		unsigned TotalSize;		// - общий размер
		unsigned TotalPages;		// - всего страниц
		unsigned AllocatedSize;	// - всего байт выделено
	};
	// -------------------- private inline members -----------------------------------
	void HANDLERS_OPEN(void) {
		//SetAccess(HandlersBegin, HandlersEnd-HandlersBegin, PAGE_READWRITE);
	}
	void HANDLERS_CLOSE(void) {
		//SetAccess(HandlersBegin, HandlersEnd-HandlersBegin, PAGE_READONLY);
	}
	// -------------------- private functions definitions  ---------------------------
	// - вернуть пустой хендл для заполнения
	FMMHANDLER* GetFreeHandler(void);				
	// - вернуть выделенный хендл по его адресу
	FMMHANDLER* GetHandlerByAddress(FMPTR addr);	
	// - найти место и вернуть заполненый хендл
	FMMHANDLER* AddHandler(unsigned size);
	FMMHANDLER* AddPageAlignedHandler(unsigned size);
	// - установить атрибуты доступа к памяти
	bool SetAccess(FMPTR base, unsigned size, unsigned mode);

	bool Initialize(unsigned totalSize = FMM_DEFAULTMEMSIZE); // - инициализация 
	bool Release(void);							// - освобождение
public:
	// -------------------- data definitions -----------------------------------------
	#ifdef FMM_LOCKING_ENABLE
		static bool isLockingEnable;	// - разрешено ли распределение блоков в режиме защиты?
	#endif
		
	static bool isInit;			// - инициализирован ли менеджер?
	static bool isFree;			

	VIRTUALBLOCK HeapBlock;		// - главный блок всей памяти менеджера
	FMPTR HeapEnd;					// - конец текущей _выделенной_ памяти
	bool isAllocateAllPages;	// - распределять ли все страницы внутри нового блока?
	unsigned PageSize;			// - размер одной страницы в байтах
	unsigned PhysSize;			// - размер физической памяти, доступной процессу

	// - все валидные хендлы 
	FMMHANDLER* Handlers;
	unsigned handlers;
	int allocatedBlocks;

	FMMHANDLER* InvalidHandlers;	// - удаленные хендлы

	FMMHANDLER* HashTableA[FMM_MAXHASHITEMS];
	FMMHANDLER* HashTableS[FMM_MAXHASHITEMS];

	MEMORYSTATUS MemoryStatus;
	FILE* logfile;
	// -------------------- public inline members ------------------------------------
	
	unsigned FMMHASHA(FMPTR addr) 
	{
		return ((unsigned)addr>>4)&FMM_MAXHASHITEMSMASK;
	}
	void FMMADDHASHA(FMMHANDLER* h)
	{
		unsigned hash_idx = FMMHASHA(h->BaseAddress);
		h->SubNextA = HashTableA[hash_idx];
		if(HashTableA[hash_idx])
			HashTableA[hash_idx]->SubPrevA = h;
		h->SubPrevA = NULL;
		HashTableA[hash_idx] = h;
	}

	unsigned FMMHASHS(unsigned size) 
	{
//		unsigned h = (unsigned)(80.0*log(((double)(size>>4) - 1.0)/75.0 + 1.0));
		//unsigned h = (unsigned)(atan((double)(size>>4)/300.0)*300.0);
		int sz=size>>4;
		unsigned h = sz> 1024*1024 ? h = FMM_MAXHASHITEMS-1 : sz*1040/(sz+1024);
		if(h > FMM_MAXHASHITEMS-1) h = FMM_MAXHASHITEMS-1;
		return h;
	}

	void FMMADDHASHS(FMMHANDLER* h) 
	{
		unsigned hash_idx = FMMHASHS(h->TotalSize);
		h->SubNextS = HashTableS[hash_idx];
		if(HashTableS[hash_idx])
			HashTableS[hash_idx]->SubPrevS = h;
		h->SubPrevS = NULL;
		HashTableS[hash_idx] = h;
	}

	void DELETE_H(FMMHANDLER* h1)
	{
		FMMHANDLER* h0 = h1->Prev;
		FMMHANDLER* h2 = h1->Next;

		if(h0)
			h0->Next = h2;
		if(h2)
			h2->Prev = h0;
		
		if(InvalidHandlers)
			InvalidHandlers->Prev = h1;
		h1->Next = InvalidHandlers;
		h1->Prev = NULL;
		InvalidHandlers = h1;

		h1->BaseAddress = NULL;
		h1->TotalSize = NULL;
		h1->SubNextA = NULL;
		h1->SubNextS = NULL;
		h1->SubPrevA = NULL;
		h1->SubPrevS = NULL;
	}

	// - удаляем хендл из таблицы размеров
	void DELETE_HS(FMMHANDLER* h1)
	{
		FMMHANDLER* h0 = h1->SubPrevS;
		FMMHANDLER* h2 = h1->SubNextS;

		if(h2)
			h2->SubPrevS = h0;

		if(h0)
			h0->SubNextS = h2;
		else
		{
			unsigned hash_idx = FMMHASHS(h1->TotalSize);
			HashTableS[hash_idx] = h2;
		}
		h1->SubNextS = NULL;
		h1->SubPrevS = NULL;
		h1->isFree = false;
	}
	// - удаляем хендл из таблицы адресов
	void DELETE_HA(FMMHANDLER* h1)
	{
		FMMHANDLER* h0 = h1->SubPrevA;
		FMMHANDLER* h2 = h1->SubNextA;

		if(h2)
			h2->SubPrevA = h0;

		if(h0)
			h0->SubNextA = h2;
		else
		{
			unsigned hash_idx = FMMHASHA(h1->BaseAddress);
			HashTableA[hash_idx] = h2;
		}
		h1->SubNextA = NULL;
		h1->SubPrevA = NULL;
		h1->isFree = true;
	}
	// -------------------- public functions definitions  ----------------------------
	bool FMM_LOG_OPEN(void);
	bool FMM_LOG_CLOSE(void);
	void FMM_ERROR(char* str, ...);
	void FMM_LOG(char* str, ...);

	bool SelfTest(void);
	bool GetInfo(FMMINFO* m);					// - заполнить структуру состояния памяти
	FMPTR Allocate(int size);					// - распределить память
	bool Deallocate(FMPTR addr);				// - освободить блок памяти
	
	#ifdef FMM_LOCKING_ENABLE
		int Lock(FMPTR addr);					// - установить разрешение на запись/чтение блока
		int Unlock(FMPTR addr);					// - запретить запись/чтение блока
	#endif
	FMManager()
	{
		Initialize();
	}
	~FMManager() 
	{
		isFree=true;
	}
	// -------------------------------------------------------------------------------
};

#endif
