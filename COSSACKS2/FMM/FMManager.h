#ifndef FMMANAGER_H
#define FMMANAGER_H

#include <math.h>

// - ��������/��������� ����� ���������������� ������
//#define FMM_LOGGING_ENABLE

// - ����� ������ �������� ��������� ����� �� ������ � �������, ������� 4� � ���������� ����,
// ��� ������� ���������� ������� Unlock(addr)
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
	����� ����� ���������� � ���� ����������:
1. �������������� (invalid)
2. ���������
3. ����������

1. ���������� ������ ��������� � ������ �� ���������� Prev/Next 
	� ������� � InvalidHandlers. 
	���� SubNext, SubPrev, BaseAddress, TotalSize - �������.

2. ��������� ������ ��������� � ������ � ����������� �� ���������� Prev/Next 
	� ������������, ��������������� ����������� ������������ ������ ������.
	����� ��������� ������ �������� �������, ������� ������ � ������� ��������
	HashTableS �� ���������� SubNextS/SubPrevS.

3. ���������� ������ ��������� � ������ �� ���������� �� ���������� Prev/Next.
	����� ���������� ������ �������� �������, ������� ������ � ������� �������
	HashTableA �� ���������� SubNextA/SubPrevA.

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
		FMPTR BaseAddress;		// - ����� ����� ������������ ����� 
		unsigned TotalSize;		// - ����� ������
		unsigned TotalPages;		// - ����� �������
		unsigned AllocatedSize;	// - ����� ���� ��������
	};
	// -------------------- private inline members -----------------------------------
	void HANDLERS_OPEN(void) {
		//SetAccess(HandlersBegin, HandlersEnd-HandlersBegin, PAGE_READWRITE);
	}
	void HANDLERS_CLOSE(void) {
		//SetAccess(HandlersBegin, HandlersEnd-HandlersBegin, PAGE_READONLY);
	}
	// -------------------- private functions definitions  ---------------------------
	// - ������� ������ ����� ��� ����������
	FMMHANDLER* GetFreeHandler(void);				
	// - ������� ���������� ����� �� ��� ������
	FMMHANDLER* GetHandlerByAddress(FMPTR addr);	
	// - ����� ����� � ������� ���������� �����
	FMMHANDLER* AddHandler(unsigned size);
	FMMHANDLER* AddPageAlignedHandler(unsigned size);
	// - ���������� �������� ������� � ������
	bool SetAccess(FMPTR base, unsigned size, unsigned mode);

	bool Initialize(unsigned totalSize = FMM_DEFAULTMEMSIZE); // - ������������� 
	bool Release(void);							// - ������������
public:
	// -------------------- data definitions -----------------------------------------
	#ifdef FMM_LOCKING_ENABLE
		static bool isLockingEnable;	// - ��������� �� ������������� ������ � ������ ������?
	#endif
		
	static bool isInit;			// - ��������������� �� ��������?
	static bool isFree;			

	VIRTUALBLOCK HeapBlock;		// - ������� ���� ���� ������ ���������
	FMPTR HeapEnd;					// - ����� ������� _����������_ ������
	bool isAllocateAllPages;	// - ������������ �� ��� �������� ������ ������ �����?
	unsigned PageSize;			// - ������ ����� �������� � ������
	unsigned PhysSize;			// - ������ ���������� ������, ��������� ��������

	// - ��� �������� ������ 
	FMMHANDLER* Handlers;
	unsigned handlers;
	int allocatedBlocks;

	FMMHANDLER* InvalidHandlers;	// - ��������� ������

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

	// - ������� ����� �� ������� ��������
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
	// - ������� ����� �� ������� �������
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
	bool GetInfo(FMMINFO* m);					// - ��������� ��������� ��������� ������
	FMPTR Allocate(int size);					// - ������������ ������
	bool Deallocate(FMPTR addr);				// - ���������� ���� ������
	
	#ifdef FMM_LOCKING_ENABLE
		int Lock(FMPTR addr);					// - ���������� ���������� �� ������/������ �����
		int Unlock(FMPTR addr);					// - ��������� ������/������ �����
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
