#ifndef __DYNARRAY_H__
#define __DYNARRAY_H__

#ifndef __STDAPPLICATION__
#include "smart_assert.h"
#else //__STDAPPLICATION__
#include "assert.h"
#endif //__STDAPPLICATION__

#define SAFE_ARRAY
template <class Elem,int Step=15>
class DynArray
{
public:
	Elem* Values;
	int NValues;
	int MaxValues;

	inline void Check(int N){
		if(N>=MaxValues){
			if(MaxValues==0)MaxValues=N+Step;
			else MaxValues=N+Step+MaxValues/2;
			Elem* tmp=(Elem*)malloc(MaxValues*sizeof Elem);
			if(Values){
				memcpy(tmp,Values,NValues*sizeof Elem);
				free(Values);
			}
			Values=tmp;
		}
	}
	inline Elem* operator + (int idx){
		assert(idx>=0&&idx<NValues);
		assert(NValues);
		return Values+idx;
	}
	inline DynArray(){
		Values=NULL;
		NValues=0;
		MaxValues=0;
	}
	inline DynArray(int Size, Elem Filling){
		Values=NULL;
		NValues=0;
		MaxValues=0;
		Add(Filling,Size);
	}
	inline ~DynArray(){
		Clear();		
	}
	inline int Add(const Elem& V){
		Check(NValues+1);
		Values[NValues]=V;
		NValues++;
		return NValues-1;
	}
	inline int Add(const Elem& V,int NTimes){
		Check(NValues+NTimes);
		for(int i=0;i<NTimes;i++){
			//Values[NValues]=V;
			memcpy(&Values[NValues],&V,sizeof Elem);
			NValues++;
		}
		return NValues-1;
	}
	inline void Del(int pos,int N){
		if(pos<0){
			N+=pos;
			pos=0;
		}
		if(N<=0||pos>=NValues)return;
		if(pos+N>NValues)N=NValues-pos;
		if(pos+N<NValues)memcpy(Values+pos,Values+pos+N,(NValues-pos-N)*sizeof Elem);
		NValues-=N;
	}

	inline int find(const Elem& v){
		for(int i=0;i<NValues;i++)if(!memcmp(&v,&Values[i],sizeof(Elem)))return i;
		return -1;
	}

	inline bool Insert(int pos,const Elem& V){
		if(pos<0||pos>NValues)return false;
		Check(NValues+1);
		if(pos<NValues)memmove(Values+pos+1,Values+pos,(NValues-pos)*sizeof Elem);
		Values[pos]=V;
		NValues++;
		return true;
	}

	inline bool Move(int from, int to){		
		if(from>=NValues) from=NValues-1; else if(from<0) from=0;
		if(to>=NValues) to=NValues-1; else if(to<0) to=0;
		if(from==to) return false;		
		Elem t=Values[from];
		if(from<to){
			memcpy(Values+from,Values+from+1,(to-from)*sizeof Elem);
		}else{
			memmove(Values+to+1,Values+to,(from-to)*sizeof Elem);
		}		
		Values[to]=t;
		return true;
	}
	inline Elem& operator [](int pos){
#ifdef SAFE_ARRAY
		assert(pos>=0&&pos<NValues);
		if(pos<0||pos>=NValues)return Values[0];//DlaBitja
#endif //SAFE_ARRAY
		return Values[pos];
	}
	inline Elem operator [](int pos) const {
#ifdef SAFE_ARRAY
		assert(pos>=0&&pos<NValues);
		if(pos<0||pos>=NValues)return Values[pos];//DlaBitja;
#endif //SAFE_ARRAY
		return Values[pos];
	}
	inline void operator += (const DynArray& A){
		AddValues(A.Values,A.NValues);
	}
	inline void operator += (Elem V){
		Add(V);
	}
	inline Elem* GetValues()const{//use only to save data!!!
		return Values;
	}
	inline void AddValues(Elem* Data,int N){
		Check(NValues+N);
		memcpy(Values+NValues,Data,N*sizeof Elem);
		NValues+=N;
	}
	inline int GetAmount()const{
		return NValues;
	}
	inline void Clear(){
		if(Values)free(Values);
		Values=NULL;
		NValues=0;
		MaxValues=0;
	}
	inline void FastClear(){		
		NValues=0;
	}
	inline void Fill(int Value){		
		memset(Values,Value,NValues*sizeof(Elem));
	}
	inline Elem Begin(){
		assert(NValues!=0);
#ifdef SAFE_ARRAY
		if(NValues==0)return Values[0];//DlaBitja;
#endif
		return Values[0];
	}
	inline Elem End(){
		assert(NValues!=0);
		if(NValues==0){
#ifdef SAFE_ARRAY
			return Values[0];//DlaBitja;
#endif
		}
		return Values[NValues-1];
	}	
	inline Elem pop_back(){
		if(NValues>0){
			return Values[--NValues];
		}else return Values[0];
	}
    inline void CopyArrayTo(DynArray<Elem>& dest){
        dest.Check(NValues);
        memcpy(dest.Values,Values,NValues*sizeof Elem);
        dest.NValues=NValues;
    }
	inline void pop_front(){
		Del(0,1);
	}
};
#endif //__DYNARRAY_H__