#ifndef _FSAMPPROFILERALLOC_H_
#define _FSAMPPROFILERALLOC_H_

#include <windows.h>

template <class T>
struct FSPMemBlock {
	union {
		FSPMemBlock<T> *Next;
		T Element;
	};
};

template <class T>
struct FSPBigArray {
	enum { N = 1024 };
	FSPBigArray<T>* Next;
	FSPMemBlock<T> Array[N];
};

template <class T>
class FSPManager {
	FSPMemBlock<T> *FreeBlocks;
	FSPBigArray<T> *Arrays;
public:
	T* Allocate(void)
	{
		if(FreeBlocks == NULL)
		{
			void* ptr = (void*)GlobalAlloc(GMEM_FIXED, sizeof(FSPBigArray<T>));
			if(ptr == NULL) throw("GlobalAlloc error!");
			FSPBigArray<T> *A = (FSPBigArray<T>*)ptr;
			A->Next = Arrays;
			Arrays = A;
			for(int i = 0; i < FSPBigArray<T>::N; i++)
			{
				Arrays->Array[i].Next = FreeBlocks;
				FreeBlocks = &(Arrays->Array[i]);
			}
		}
		FSPMemBlock<T> *b = FreeBlocks;
		FreeBlocks = FreeBlocks->Next;
		return &(b->Element);
	}

	void Free(T* t)
	{
		FSPMemBlock<T> *b = (FSPMemBlock<T>*)t;
		b->Next  = FreeBlocks;
		FreeBlocks = b;
	}

	void FreeAll(void)
	{
		FSPBigArray<T> *A = Arrays;
		while(A)
		{
			FSPBigArray<T> *Next = A->Next;
			if(GlobalFree((HGLOBAL)A) != NULL)
				throw("GlobalFree error!");
			A = Next;
		}
		FreeBlocks = NULL;
		Arrays = NULL;
	}

	FSPManager()
	{
		FreeBlocks = NULL;
		Arrays = NULL;
	}

	~FSPManager()
	{
		FreeAll();
	}
};

#endif