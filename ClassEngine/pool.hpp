//#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>
//#include <memory.h>
//#include <assert.h>
//#include <vector>
#include "..\ClassEngine\pool.h"
//#define NOPOOL
//#define STRONGCHECK
#define oneassert(x) if(!(x)){static bool w=true;if(w){assert(x);w=false;}}
std_OnePoolType::std_OnePoolType(){
	//QuantSize=8;
	//SegmentSize=1024;
}
std_OnePoolType::~std_OnePoolType()
{
	for(unsigned int i=0;i<Pool.GetAmount();i++){
		delete[](Pool[i]);
	}
}

//-----------------------------------------------------------------------------
// std_OnePoolType
//-----------------------------------------------------------------------------
void std_OnePoolType::Free() {
	for(int i = 0; i < Pool.GetAmount(); i++) {
		oneassert(Pool[i] && "NULL pointer in pool!");
		delete[] Pool[i];
	}
	Pool.Clear();
	FreeList.Clear();
}

BYTE* std_OnePoolType::Allocate()
{
#ifdef NOPOOL
	return new BYTE[QuantSize];
#else //NOPOOL
	if(FreeList.GetAmount()==0){
        BYTE* tmp=new BYTE[SegmentSize*QuantSize];
		Pool.Add(tmp);
		for (int i = 0; i < SegmentSize; i++) {
			DWORD* A = (DWORD*)(tmp + i * QuantSize);
			*A = 'FOOD';
			FreeList.Add(tmp + i * QuantSize); 
		}
	}	
	BYTE* e=FreeList.pop_back();
	DWORD* A=(DWORD*)e;	
	*A='FOOD';
	return e;
#endif
}

//-----------------------------------------------------------------------------
// TotalPool::Free
//-----------------------------------------------------------------------------
void TotalPool::Free() {
	pool_08.Free();
	pool_16.Free();
	pool_32.Free();
	//pool_64.Free();
	pool_128.Free();
}

void std_OnePoolType::Free(BYTE* Ptr)
{
#ifdef NOPOOL
	delete[](Ptr);
#else //NOPOOL
	if(Ptr){
#ifdef STRONGCHECK
		int p=int(Ptr);
		int psize=SegmentSize*QuantSize;
		bool haveit=false;
		for(int i=0;i<Pool.GetAmount();i++){
			int p0=int(Pool[i]);
			if(p>=p0 && p<p0+psize){
				haveit=true;
				break;
			}
		}
		if(!haveit){
			return;
		}
#endif //STRONGCHECK
		DWORD* A=(DWORD*)Ptr;
		oneassert(*A!='BAAD');
        *A='BAAD';
		FreeList.Add(Ptr);
	}
#endif
}
int std_OnePoolType::GetMemoryUsed(){
	return Pool.GetAmount()*SegmentSize*QuantSize;
}
void std_OnePoolType::KillDirtyGarbage(){
	if(FreeList.GetAmount()<SegmentSize*3)return;
	DWORD minA=0xFFFFFFFF;
	DWORD maxA=0;
	int packsize=SegmentSize*QuantSize;
	for(int i=0;i<Pool.GetAmount();i++)if(Pool[i]){
		int A=int(Pool[i]);
		if(A<minA)minA=A;
		if(A>maxA)maxA=A;
	}
	int N=4+(maxA-minA)/packsize;
	word* buf=new word[N];
	memset(buf,0xFF,N<<1);
	for(int i=0;i<Pool.GetAmount();i++)if(Pool[i]){
		int A=(int(Pool[i])-minA+packsize-1)/packsize;
		oneassert(A>=0&&A<N);
		buf[A]=i;
	}
	int noct=2+SegmentSize*Pool.GetAmount()/8;
	BYTE* bits=new BYTE[noct];	
	int bits_per_segment=SegmentSize/8;
	oneassert(SegmentSize==bits_per_segment*8);
    memset(bits,0,noct);
	for(int j=0;j<FreeList.GetAmount();j++){
		int p0=int(FreeList[j]);
		int p=(p0-minA)/packsize;
		//assert(p>=0 && p<N);
		if(p>=0 && p<N){
			int idx=-1;
			int g=buf[p];
			if(g!=0xFFFF){
				int A=int(Pool[g]);
				if(p0>=A && p0<A+packsize){
					idx=g;
				}
			}
			if(idx==-1){
				g=buf[p+1];
				if(g!=0xFFFF){
					int A=int(Pool[g]);
					if(p0>=A && p0<A+packsize){
						idx=g;
					}
				}
				if(idx==-1 && p>0){
					g=buf[p+1];
					if(g!=0xFFFF){
						int A=int(Pool[g]);
						if(p0>=A && p0<A+packsize){
							idx=g;
						}
					}
				}
			}
			oneassert(idx>=0 && idx<(Pool.GetAmount()));
			int bpos=idx*SegmentSize+(p0-int(Pool[idx]))/QuantSize;
			oneassert(!(bits[bpos>>3] & (1<<(bpos&7))));
			oneassert(bpos<noct*8);
			bits[bpos>>3]|=1<<(bpos&7);
		}
	}
	int N0=FreeList.GetAmount();
	int ndel=0;
	FreeList.Clear();
	for(int i=0;i<Pool.GetAmount();i++){
		int pos=i*bits_per_segment;
		bool allfree=true;
		for(int j=0;j<bits_per_segment;j++)if(bits[pos+j]!=0xFF){
            allfree=false;
			break;
		}
		if(allfree){
            delete(Pool[i]);
			Pool[i]=NULL;
			ndel++;
		}else{
            BYTE* p0=Pool[i];
			for (int j = 0; j < SegmentSize; j++) {
				if ((bits[pos + (j >> 3)] & (1 << (j & 7))) != 0) {
					DWORD* A = (DWORD*)(p0 + j * QuantSize);
					*A = 'FOOD';
					FreeList.Add(p0 + j * QuantSize);
				}
			}
		}
	}
	//assert(N0==ndel*SegmentSize+FreeList.GetAmount());
	int p=0;
	for(int i=0;i<Pool.GetAmount();i++){
		if(Pool[i]){
			Pool[p]=Pool[i];
			p++;
		}
	}
	if(p!=Pool.GetAmount())Pool.Del(p,Pool.GetAmount()-p);
	delete[]bits;
	delete[]buf;
}
//this function should be called only once, after object constructing
void std_OnePoolType::SetQuantSize(int size){
    QuantSize=size;
	SegmentSize=4096/QuantSize;
	for(unsigned int i=0;i<Pool.GetAmount();i++){
		delete[](Pool[i]);
	}
	Pool.Clear();
	FreeList.Clear();
}
void TotalPool::Init(){
	if(!m_fIsInitialized) {
		m_fIsInitialized = true;
		pool_08.SetQuantSize(8);
		pool_16.SetQuantSize(16);
		pool_32.SetQuantSize(32);
		//pool_64.SetQuantSize(64);
		pool_128.SetQuantSize(128);
	}
}
TotalPool::TotalPool(){
	m_fIsInitialized = false;
	Init();
}
std_OnePoolType* TotalPool::GetPoolBySize(int size){
#ifdef NOPOOL
	return NULL;
#endif //NOPOOL
	if(size<=8)return &pool_08;
	if(size<=16)return &pool_16;
	if(size<=32)return &pool_32;
	//if(size<=64)return &pool_64;
	if(size<=128)return &pool_128;
	return NULL;
}
TotalPool::~TotalPool(){

}
BYTE* TotalPool::Allocate(int size)
{
	Init();
	if(size==0)return NULL;
	std_OnePoolType* pool=GetPoolBySize(size);
	if(pool)return pool->Allocate();
	else return (BYTE*)malloc(size);
}

void  TotalPool::Free(BYTE* ptr,int size){
	std_OnePoolType* pool=GetPoolBySize(size);
	if(pool)pool->Free(ptr);
	else free(ptr);
}

BYTE* TotalPool::Realloc(BYTE* ptr,int size,int newsize){
	std_OnePoolType* old_pool=GetPoolBySize(size);
	std_OnePoolType* new_pool=GetPoolBySize(newsize);
	if(ptr&&old_pool&&old_pool==new_pool)return ptr;
	BYTE* nptr=Allocate(newsize);
	if(ptr&&nptr)memcpy(nptr,ptr,min(size,newsize));
    Free(ptr,size);
	return nptr;
}
void fast_queue::check()
{

}
int fast_queue::getValueIndex(float val){
	int n=queue.size();
	oneassert(n!=0);
	float idx=float((val-_minv)*n/(_maxv-_minv));
	if(idx<0)idx=0;
	if(idx>=n)idx=n-1;
	return int(idx);
}
fast_queue::fast_queue(){
	_minv=0;
	_maxv=0;
	first_nonzero=0;
}
void fast_queue::set_range(float minv,float maxv,int n_divisions){
	queue.clear();
	_minv=minv;
	_maxv=maxv;
	first_nonzero=n_divisions-1;
	for(int i=0;i<n_divisions;i++)queue.push_back(NULL);
}
void fast_queue::add(queue_base& v){
	unsigned int idx=getValueIndex(v.getCost());
	v.prev=NULL;
	v.next=queue[idx];
	if(queue[idx]){
		queue[idx]->prev=&v;
	}
	queue[idx]=&v;
	if(idx < first_nonzero)first_nonzero = idx;
}
void fast_queue::remove(queue_base& v){
	int idx=getValueIndex(v.getCost());
	if(v.prev){
		v.prev->next=v.next;		
	}else{
		oneassert(queue[idx]);
		queue[idx]=v.next;
	}
	if(v.next){
		v.next->prev=v.prev;
	}
	v.next=NULL;
	v.prev=NULL;
	if(idx==first_nonzero){
		for(;first_nonzero<queue.size()&&queue[first_nonzero]==NULL;first_nonzero++);
	}
}
queue_base* fast_queue::get_cheapest(){
	queue_base* v=queue[first_nonzero];
	float _min=v->getCost();
	if(v)return v;
	queue_base* vm=v;
	while(v&&v->next){
		v=v->next;
		if(v){
			float c=v->getCost();
			if(c<_min){
				_min=c;
				vm=v;
				break;
			}
		}
	}
	return vm;
}
void fast_queue::change(queue_base& v,float newcost){
	remove(v);
	v.setCost(newcost);
	add(v);
}
set_of_int::set_of_int(){
	set=NULL;
	set_size=0;
}
set_of_int::~set_of_int(){
	g_SetOfIntPool.Free((BYTE*)set,set_size*4);
}
void set_of_int::add(int value){
	if(have(value))return;
	set=(int*)g_SetOfIntPool.Realloc((BYTE*)set,set_size*4,set_size*4+4);
	set[set_size]=value;
	set_size++;
}
void set_of_int::del(int value){
	for(int i=0;i<set_size;i++)if(set[i]==value){
		if(i<set_size-1){
			memcpy(set+i,set+i+1,(set_size-i-1)*4);
		}
		set=(int*)g_SetOfIntPool.Realloc((BYTE*)set,set_size*4,set_size*4-4);
		set_size--;
		return;
	}
}
//int set_of_int::size (){
//    return set_size;    

//int  set_of_int::operator[] (int pos){
//	assert(pos>=0&&pos<set_size);
//    return set[pos];

void set_of_int::clear(){
	g_SetOfIntPool.Free((BYTE*)set,set_size*4);
	set=NULL;
	set_size=0;
}
bool set_of_int::have(int value) const {
	for(int i = 0; i < set_size; i++) if(set[i] == value) return true;
	return false;
}
void set_of_int::copyTo(set_of_int& dest){
	dest.clear();
	dest.set=(int*)g_SetOfIntPool.Allocate(set_size*4);
	memcpy(dest.set,set,set_size*4);
}
TotalPool* _POOL=NULL;


TotalPool g_SetOfIntPool;