#include "stdio.h"
#include "windows.h"
//#include "stdafx.h"
#include "ResFile.h"
#include "math.h"
#include "smart_assert.h"
#include "Fex\FEXHelpers.h"
#include "kLog.h"
#define CEXPORT __declspec(dllexport)
CEXPORT void* _ExMalloc(int Size);
#define znew(t,s) (t*)_ExMalloc((s)*sizeof(t))
extern HWND hwnd;
#undef free
#undef malloc
int TotalSize=0;
#pragma pack(push)
#pragma pack(8)
#include "FMM\FMM.H"
#pragma pack(pop)

//_inline 
int FMM_GetFreeMem(){
	return MManager.HeapBlock.AllocatedSize;
}
int FMM_GetBlocks(){
	return MManager.HeapBlock.TotalPages;
}
int FMM_GetMemDiff(){
	static int stsize=FMM_GetFreeMem();
	return FMM_GetFreeMem()-stsize;
}
int FMM_GetNHandlers(){	
	return MManager.handlers;
}
int FMM_GetAllocatedBlocks(){	
	return MManager.allocatedBlocks;
}
int FMM_GetMaxFreeBlock(){
	FMManager::FMMHANDLER* H=MManager.Handlers;
	int maxsz=0;
	while(H){
		if(H->isFree && H->TotalSize>maxsz){
			maxsz=H->TotalSize;
		}
		H=H->Next;
	}
	return maxsz;
}
int NMalloc=0;
int NMallocSmall=0;
int NFree=0;

_inline void* FM_Malloc(int size){
	
    if (!size) size = 4;
	//if(size<128)NMallocSmall++;;
	//size=((size>>10)+1)<<10;
	void* p=MManager.Allocate(size);
	//memset(p,0,size);
	//if(size)fast_memset(p,0,(size+15)&0xFFFFFFF0);
	NMalloc++;
	return p;
};
_inline void FM_free(void* ptr){
	if(!MManager.isInit)return;
	if(ptr){
		NFree++;
		//memset(ptr,0xAB,16);
		MManager.Deallocate((FMPTR)ptr);
	}
};
//#define TESTLEAKS

#ifdef TESTLEAKS

bool FAST_MEMORY=false;
bool SmartLeakDoctor=true;
bool ForcedSmartLeakDoctor=true;

#else //TESTLEAKS

bool FAST_MEMORY=true;
bool SmartLeakDoctor=false;
bool ForcedSmartLeakDoctor=false;

#endif //TESTLEAKS

#define malloc FM_Malloc
#define calloc(a,b) FM_Malloc(a)
#define free FM_free

//-----------------
void KeyTestMem(){
	///if((GetKeyState(VK_CONTROL)&0x8000)&&(GetKeyState(VK_SHIFT)&0x8000)&&(GetKeyState(VK_MENU)&0x8000)){
	if(GetKeyState(VK_CAPITAL)){
		int CheckMemoryIntegrityEx(int& StartFails,int& EndFails,int& Exceptions);
		int NS;
		if(CheckMemoryIntegrityEx(NS,NS,NS)){
			int v=0;
		}
	}
}

CEXPORT void PushSmartLeak(bool& v){
	//if(!ForcedSmartLeakDoctor){
	//	v=SmartLeakDoctor;
	//	SmartLeakDoctor=0;
	//}
};
CEXPORT void PopSmartLeak(bool& v){
	//if(!ForcedSmartLeakDoctor)SmartLeakDoctor=v;
}

#define SAFEMEMORY
#define NOERRORS
#define SUPERSAFEREALLOC

bool CheckMemBlock(byte* ptr){	
	return true;
};
void CheckDynamicalPtr(void* ptr){	
};
int AllocSize=0;
int LeakStackSize=40;
void PrintPtrHistory(void* ptr,char* buf){
	if(!ptr)return;	

	if(FAST_MEMORY)return;

	ptr=(void*)(((DWORD(ptr))&0xFFFFFFF0)+8);
	byte* data=((byte*)ptr)-8;
	DWORD SG=((DWORD*)data)[1];			
	if(SmartLeakDoctor){
		DWORD * FD=(DWORD*)(data-LeakStackSize*4);
		void PrintLineInfo(DWORD ptr,char* dest);
		char cbuf[256];
		for(int j=0;j<LeakStackSize;j++){
			PrintLineInfo(FD[j],cbuf);			
			strcat(buf,cbuf);
			if(cbuf[0])strcat(buf,"\n");
		}	
	}
}

CEXPORT
void ___ExFree(void* ptr){
	if(MManager.isFree)return;
	if(FAST_MEMORY){
		if(ptr)FM_free(ptr);
		return;
	}else{
		try{
			if(!ptr)return;	
			ptr=(void*)(((DWORD(ptr))&0xFFFFFFF0)+8);
			byte* data=((byte*)ptr)-8;
			DWORD SG=((DWORD*)data)[1];
			if(SG!='TRTS'&&SG!='DOCT'&&SG!='EERF'){
				FM_free(ptr);
				return;
			}
			if(SG!='TRTS'&&SG!='DOCT'){
				char s[1024];			
				strcpy(s,"Attemt to free pointer twice. Location of previous free:\n");
				PrintPtrHistory(ptr,s);
				SmartAssert(__FILE__,__LINE__,s);
			}		
			int size=((DWORD*)data)[0];
			((DWORD*)data)[1]='EERF';
			*((DWORD*)(data+8+size))='FDNE';		
			assert(size<100000000);
			memset(data+8,0xBC,size);
			if(SmartLeakDoctor&&SG=='DOCT'){
				unsigned* LEAK=(unsigned*)(data-LeakStackSize*4);
				memset(LEAK,0,4*LeakStackSize);
				FEX_GetCallStackEBP(LEAK,LeakStackSize);
			}
			AllocSize-=size+12;
			if(SmartLeakDoctor)AllocSize-=LeakStackSize*4;
			if(SG=='DOCT')data-=LeakStackSize*4;		
			free(data);
		}catch(...){};
	}
};

//-----------------------------------------------------------------------------
// _ExFree
//-----------------------------------------------------------------------------
CEXPORT void _ExFree(void *Ptr) {
	if(NULL == Ptr) { // Nothing to free.
		return;
	}

	if(Ptr < MManager.HeapBlock.BaseAddress || Ptr > MManager.HeapBlock.BaseAddress + MManager.HeapBlock.TotalSize) {
		// Block was allocated globally:		
		try{
			GlobalFree(Ptr);
		}catch(...){}
	} else {
		// Block was allocated by built - in memory manager:
		___ExFree(Ptr);
	}
} // _ExFree



void CheckVirtTBL(void* ptr){
	if(ptr){
		if(!((DWORD*)ptr)[0]){
			char s[1024];			
			strcpy(s,"Virtual table is bad. May be the class is released. Free history:\n");
			PrintPtrHistory(ptr,s);
			SmartAssert(__FILE__,__LINE__,s);
		}
	}
}

CEXPORT
void* ___ExMalloc(int Size){
	if(MManager.isFree)return NULL;
	if(FAST_MEMORY){
		return FM_Malloc(Size);
	}else{
		assert(Size<50000000);
		TotalSize++;
		int sz0=0;
		if(SmartLeakDoctor){
			Size+=LeakStackSize*4;
			sz0=LeakStackSize;
		}
		byte* data=(byte*)calloc(Size+12,1);
		((DWORD*)data)[sz0]=Size-sz0*4;
		((DWORD*)data)[sz0+1]=SmartLeakDoctor?'DOCT':'TRTS';
		*((DWORD*)(data+8+Size))='LNIF';
		if(SmartLeakDoctor){
			unsigned* LEAK=(unsigned*)data;
			memset(LEAK,0,4*LeakStackSize);
			FEX_GetCallStackEBP(LEAK,LeakStackSize);
		}
		AllocSize+=Size+12;	
		return data+8+sz0*4;
	}
};

//-----------------------------------------------------------------------------
// _ExMalloc
//-----------------------------------------------------------------------------
CEXPORT void * _ExMalloc(int Size) {
	void *Ptr = ___ExMalloc(Size);
	if(NULL == Ptr) {
		// Built - in memory manager cann't allocate block.
		// So we'll try to allocate it globally.
		Ptr = GlobalAlloc(GMEM_FIXED, Size);
	}
	return Ptr;
} // _ExMalloc


CEXPORT
void* ___ExRealloc(void* ptr,int Size){
	if(FAST_MEMORY){
		int oldSize = 0;
		if (ptr)
		{
			FMManager::FMMHANDLER* h = MManager.GetHandlerByAddress((FMPTR)ptr);			
			oldSize = h->TotalSize;
			if (oldSize > Size)
			{
				oldSize = Size;
			}
		}
		void* pNewBlock = ptr;
		pNewBlock = FM_Malloc( Size );
		if (pNewBlock && ptr)
		{
			memcpy( pNewBlock, ptr, oldSize );		
			FM_free( ptr );
		}
		return pNewBlock;
	}else{
		if(ptr){
			byte* data=((byte*)ptr)-8;
			CheckMemBlock(data);
			int size=((DWORD*)data)[0];
			void* PTR=ptr;
			PTR=_ExMalloc(Size);
			if(PTR){
				try{
					memcpy(PTR,ptr,Size>size?size:Size);
					_ExFree(ptr);
				}catch(...){};
			};
			return PTR;
		}else return _ExMalloc(Size);
	}
};

//-----------------------------------------------------------------------------
// _ExRealloc
//-----------------------------------------------------------------------------
CEXPORT void * _ExRealloc(void *Ptr, int Size) {
	if(!Ptr) {
		return _ExMalloc(Size);
	}
	char Info[1024]; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	if(Ptr < MManager.HeapBlock.BaseAddress || Ptr > MManager.HeapBlock.BaseAddress + MManager.HeapBlock.TotalSize) {
		// This buffer was allocated globally. So we'll try to reallocate it globally.
		const SIZE_T OldBufferSize = GlobalSize(Ptr);
		void *pNewBuffer = GlobalAlloc(GMEM_FIXED, Size);
		const SIZE_T SizeToCopy = min(OldBufferSize, (SIZE_T)Size);
		if(SizeToCopy) {
			CopyMemory(pNewBuffer, Ptr, SizeToCopy);
		}

		sprintf(Info, "_ExRealloc(0x%x, %d): 0x%x, ToCopy: %d (bytes)", Ptr, Size, pNewBuffer, SizeToCopy); // <<<<<
		Log.Message(Info); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		
		GlobalFree(Ptr);
		return pNewBuffer;
//		return GlobalReAlloc(Ptr,Size,GMEM_FIXED);
	} else {
		// This buffer was allocated by built - in memory manager.
		// So Reallocate it by manager (FIX - ME: Consider situation, when memory manager returns NULL)!
		void *MPtr = ___ExRealloc(Ptr, Size);
		if(!MPtr) {
			// Built - in memory manager cannot allocat block.
			// So try to allocate it globally.
			MPtr = GlobalAlloc(GMEM_FIXED, Size);
			sprintf(Info, "_ExRealloc FAILED! Block was allocated globally: sizeof(0x%x) == %d (bytes)", MPtr, Size);
			Log.Message(Info);
		}
		return MPtr;
	}
} // _ExRealloc

bool CheckMemBlockAnyway(byte* ptr){	
	return true;
};
void CheckMemoryIntegrity(){
}
int CheckMemoryIntegrityEx(int& StartFails,int& EndFails,int& Exceptions){
	if(FAST_MEMORY)return 0;

	StartFails=0;
	EndFails=0;
	Exceptions=0;
	if(!MManager.isInit)return 0;
	int N=MManager.handlers;
	int nerr;
	int NFAILS=0;
	DWORD* FAILS[32];
	FMManager::FMMHANDLER* HPtr=MManager.Handlers;
	for(int i=0;i<FMM_MAXHASHITEMS;i++){
		try{
			FMManager::FMMHANDLER* HPtr=MManager.HashTableA[i];
			while(HPtr){
				if(!HPtr->isFree){
					byte* ptr=HPtr->BaseAddress;
					int size=0;
					DWORD* CST=NULL;
					if(((DWORD*)ptr)[1]=='TRTS'){
						size=((DWORD*)ptr)[0];
					}else if(((DWORD*)ptr)[1+LeakStackSize]=='DOCT'){
						size=((DWORD*)ptr)[LeakStackSize]+LeakStackSize*4;
						CST=(DWORD*)ptr;
					}else{
						if(SmartLeakDoctor)CST=(DWORD*)ptr;
						StartFails++;					
					}
					if(((DWORD*)(ptr+size+8))[0]!='LNIF'){
						EndFails++;
						if(CST&&NFAILS<32){
							FAILS[NFAILS]=CST;
							NFAILS++;
						}
					}
				}
				HPtr=HPtr->SubNextA;
			}
		}catch(...){
			Exceptions++;
		}
	}
	if(NFAILS){
		FILE* F=fopen("MemDamage.log","w");
		if(F){
			for(int i=0;i<NFAILS;i++){
				void PrintLineInfo(DWORD ptr,char* dest);
				char buf[256];
				for(int j=0;j<LeakStackSize;j++){
					PrintLineInfo(FAILS[i][j],buf);
					if(buf[0]){
						fprintf(F,"%s\n",buf);
					}
				}
				fprintf(F,"\n");
			}
			fclose(F);
		}
	}
	return StartFails+EndFails+Exceptions;
}
void CheckMemoryIntegrityEx234(){
	int StartFails;
	int EndFails;
	int Exceptions;
	//MManager.SelfTest();
	StartFails=0;
	EndFails=0;
	Exceptions=0;
	if(!MManager.isInit)return;
	int N=MManager.handlers;
	int nerr;
	int NFAILS=0;
	DWORD* FAILS[32];
	FMManager::FMMHANDLER* HPtr=MManager.Handlers;
	int p0=rand()%FMM_MAXHASHITEMS;
	for(int i=p0;i<p0+4;i++){
		try{
			FMManager::FMMHANDLER* HPtr=MManager.HashTableA[i];
			while(HPtr){
				if(!HPtr->isFree){
					byte* ptr=HPtr->BaseAddress;
					int size=0;
					DWORD* CST=NULL;
					((byte*)ptr)[8+rand()&15]=rand();					
				}
				HPtr=HPtr->SubNextA;
			}
		}catch(...){
			Exceptions++;
		}
	}	
}
void TestDynMem(){
	if(FAST_MEMORY)return;
	int s,e,x;
	CheckMemoryIntegrityEx(s,e,x);
}
void PrintAllLeaks(){
	if(FAST_MEMORY)return;

	if(!MManager.isInit)return;
	int N=MManager.handlers;
	FILE* FD=fopen("leaks.log","w");
	if(FD){
		FMManager::FMMHANDLER* HPtr=MManager.Handlers;
		for(int i=0;i<FMM_MAXHASHITEMS;i++){
			try{
				FMManager::FMMHANDLER* HPtr=MManager.HashTableA[i];
				while(HPtr){
					if(!HPtr->isFree){
						byte* ptr=HPtr->BaseAddress;
						int size=((DWORD*)ptr)[0];
						if(((DWORD*)ptr)[1]=='DOCT'){
							fprintf(FD,"\n\nLeak detected.\n");
							unsigned* LEAK=(unsigned*)(ptr+8+size-LeakStackSize*4);
							for(int i=0;i<LeakStackSize;i++)if(LEAK[LeakStackSize-1-i]){
								unsigned LineNum;
								char Func[1024];
								char FileName[1024];
								if(FEX_GetDebugInfo(0x400000,LEAK[LeakStackSize-1-i],&LineNum,Func,FileName)){
									if(LineNum!=-1){
										char* ss=strstr(FileName,"cossacks ii\\");
										if(ss)strcpy(FileName,ss+strlen("cossacks ii\\"));
										fprintf(FD,"%s %s %d\n",Func,FileName,LineNum);
									}
								}
							}
						}
					}
					HPtr=HPtr->SubNextA;
				}
			}catch(...){
				//assert(0);
			}
		}
		fclose(FD);
	}
}
void SetLeakMode(bool Mode){
	//if(FAST_MEMORY)return;
	//SmartLeakDoctor=Mode;
}
//-------------------------------FAST POOL FOR STRINGS---------------------------//