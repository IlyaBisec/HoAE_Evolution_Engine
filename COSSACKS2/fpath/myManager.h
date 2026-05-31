#ifndef _MYMANAGER_H_
#define _MYMANAGER_H_

#ifndef NULL
#define NULL 0
#endif

template <class T>
struct myMemBlock {
	union {
		myMemBlock<T> *Next;
		T Element;
	};
};

template <class T>
struct myBigArray {
   enum { N = 1024 };
	myBigArray<T>* Next;
	myMemBlock<T> Array[N];
	myBigArray()
	{
		Next = NULL;
	}
	~myBigArray()
	{
		if(Next) delete Next;
	}
};

template <class T>
class myManager {
	myMemBlock<T> *FreeBlocks;
	myBigArray<T> *Arrays;
public:
	T* Allocate(void)
	{
		if(FreeBlocks == NULL)
		{
			myBigArray<T> *A = new myBigArray<T>;
			A->Next = Arrays;
			Arrays = A;
			for(int i = 0; i < myBigArray<T>::N; i++)
			{
				Arrays->Array[i].Next = FreeBlocks;
				FreeBlocks = &(Arrays->Array[i]);
			}
		}
		myMemBlock<T> *b = FreeBlocks;
		FreeBlocks = FreeBlocks->Next;
		return &(b->Element);
	}

	void Free(T* t)
	{
		myMemBlock<T> *b = (myMemBlock<T>*)t;
		b->Next  = FreeBlocks;
		FreeBlocks = b;
	}

	myManager()
	{
		FreeBlocks = NULL;
		Arrays = NULL;
	}

	~myManager()
	{
		if(Arrays) delete Arrays;
	}
};

#endif