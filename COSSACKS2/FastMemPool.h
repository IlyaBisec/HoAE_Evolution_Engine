#ifndef _FASTMEMPOOL_
#define _FASTMEMPOOL_
struct OnePoolType{
	int BaseSize;
	DynArray<DWORD*> Pool;
	DynArray<DWORD*> FreeList;
	~OnePoolType(){
		for(int i=0;i<Pool.GetAmount();i++){
			free(Pool[i]);
		}
	};
	_inline DWORD* Get(){
		int N;
		if(N=FreeList.GetAmount()){
			DWORD* P;
			SAFENEW;
				P=FreeList[N-1];
				FreeList.Del(N-1,1);
				memset(P,0,BaseSize);
			ENDNEW;
			return P;
		}
		DWORD* P;
		SAFENEW;
			P=(DWORD*)malloc(BaseSize);
			Pool.Add(P);
			memset(P,0,BaseSize);
		ENDNEW;
		return P;
	}
	_inline void Free(DWORD* Item){
		SAFENEW;
			assert(Item[0]==BaseSize);
			FreeList.Add(Item);
		ENDNEW;
	}
};
struct MemPool{
	OnePoolType Pool_16;  
	OnePoolType Pool_64;
	OnePoolType Pool_256;  
	OnePoolType Pool_1024;
	MemPool(){
		Pool_16.BaseSize=16;
		Pool_64.BaseSize=64;
		Pool_256.BaseSize=256;
		Pool_1024.BaseSize=1024;
	}
	_inline DWORD* Get(int size){

		size=4+((size+3)&0xFFFF7C);
		if(size<=1024){
			OnePoolType* MP;			
			if(size<=16)MP=&Pool_16;
			else if(size<=64)MP=&Pool_64;
			else if(size<=256)MP=&Pool_256;
			else MP=&Pool_1024;
			DWORD* r=MP->Get();
			r[0]=MP->BaseSize;
			return r+1;
		}else{
			DWORD* r;
			SAFENEW;
				r=(DWORD*)malloc(size);
				r[0]=4096;
			ENDNEW;
			return r+1;
		}
	}
	_inline void Free(DWORD* Block){
		if(!Block)return;
		Block--;
		if(Block[0]==4096){
			free(Block);
		}else{
			int sz=Block[0];
			switch(sz){
			case 16:
				Pool_16.Free(Block);
				break;
			case 64:
				Pool_64.Free(Block);
				break;
			case 256:
				Pool_256.Free(Block);
				break;
			case 1024:
				Pool_1024.Free(Block);
				break;
			default:
				assert(1);
			}
		}
	}
};
extern MemPool FMEM;
__forceinline void* FastGetMem(int size){
	return FMEM.Get(size);
}
__forceinline void FastFreeMem(void* ptr){
	FMEM.Free((DWORD*)ptr);
}
#endif //_FASTMEMPOOL_