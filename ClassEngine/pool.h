#pragma once
#include <vector>

typedef unsigned char BYTE;

class DIALOGS_API std_OnePoolType
{
	int QuantSize;
	int SegmentSize;
	DynArray<BYTE*> Pool;	//list of pools, every one has size SegmentSize
	DynArray<BYTE*> FreeList;//list of free blocks, when free occurs, block will be added to this list
	                            //also, when a new segment will be added to Pool, every sub-block of Pool will
	                            //be added to a FreeList, and then the last item of FreeList we will return to user
public:
	int GetMemoryUsed();
	void KillDirtyGarbage();
	std_OnePoolType();
	~std_OnePoolType();
	BYTE*		Allocate	();
	void		Free		(BYTE* Ptr);
	__forceinline void		SetQuantSize(int size);//this function should be called only once, after object constructing
	void Free();
};
class DIALOGS_API TotalPool{

	__forceinline std_OnePoolType* GetPoolBySize(int size);//returns NULL if size>128, in this case memory will be allocated using malloc

public:

	std_OnePoolType pool_08;//pool for 8-byte blocks 
	std_OnePoolType pool_16;//---//--- 16 byte blocks
	std_OnePoolType pool_32;
	std_OnePoolType pool_64;
	std_OnePoolType pool_128;


			TotalPool	();
			~TotalPool	();
	void	Init();
	BYTE*	Allocate    (int size);
	void	Free        (BYTE* ptr,int size);
	BYTE*	Realloc     (BYTE* ptr,int size,int newsize);
	void Free();
private:
	bool m_fIsInitialized;
};
class DIALOGS_API set_of_int{
	int* set;
	int  set_size;
public:
	//set_of_int& operator = (set_of_int& bc){
	//	for(int i=0;i<bc.size();i++)add(bc[i]);
	//	return *this;
	//}
	set_of_int& operator = (const set_of_int& bc){
		for(int i=0;i<bc.size();i++)add(bc[i]);
		return *this;
	}
	set_of_int();
	~set_of_int();
	void add                      (int value);
	void del                      (int value);
	__forceinline int  size       () const {return set_size;}
	__forceinline int  operator[] (int pos) const {
		assert(pos>=0&&pos<set_size);
		return set[pos];
	};
	void clear                    ();
	__forceinline bool have       (int value) const;
	void copyTo                   (set_of_int& dest);
};
class DIALOGS_API queue_base{
public:
	queue_base* next;
	queue_base* prev;
	queue_base(){
		next=prev=NULL;
	}
	virtual float getCost(){
		return 0;
	}
	virtual void setCost(float _cost){
	}
};

class fast_queue{
	float _minv;
	float _maxv;
	unsigned int first_nonzero;
	std::vector<queue_base*> queue;
	int getValueIndex(float val);
public:
	fast_queue();
	void check();
	void set_range(float minv,float maxv,int n_divisions);
	void add(queue_base& v);
	void remove(queue_base& v);	
	void change(queue_base& v,float newcost);
	queue_base* get_cheapest();
};

extern DIALOGS_API TotalPool* _POOL;
_inline TotalPool& _getPOOL(){
	if(!_POOL)_POOL=new TotalPool;
	return *_POOL;
}

#define POOL (_getPOOL())

extern DIALOGS_API TotalPool g_SetOfIntPool;