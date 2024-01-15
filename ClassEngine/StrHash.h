#ifndef __STRHASH_H__
#define __STRHASH_H__
#include <kHash.hpp>
#ifndef NEW
#define NEW(t,s) (t*)malloc((s)*sizeof(t)) 
#endif
#pragma warning ( disable : 4251 )
#pragma pack (push)
#pragma pack ( 1 )
class Str_for_hash{
public:
	DWORD Allocated;
	char* str;
	__forceinline unsigned int	hash	() const{
		int L=strlen(str);
		DWORD S=0;
		for(int i=0;i<L;i++)S+=str[i];
		return S;
	}
	__forceinline bool			equal	( const Str_for_hash& el ){
		return !strcmp(el.str,str);
	}
	__forceinline void			copy	( const Str_for_hash& el ){
		if(str&&Allocated)free(str);
		str=NEW(char,strlen(el.str)+1);
		Allocated=1;
		strcpy(str,el.str);
	}
	Str_for_hash(){
		str=NULL;
		Allocated=0;
	}
	~Str_for_hash(){
		//if(str&&Allocated)free(str);
		//str=NULL;
	}
};
class DIALOGS_API StringsHash{	
public:
	Hash<Str_for_hash,1027,1024> SHash;
	__forceinline void clear(){
		for(int i=0;i<SHash.numElem();i++){
			if(SHash.elem(i).Allocated&&SHash.elem(i).str)free(SHash.elem(i).str);
			SHash.elem(i).str=NULL;
			SHash.elem(i).Allocated=false;
		}
		SHash.reset();        
	}
	void add(const char* s){
		if(!s) return;
		Str_for_hash S;
		S.str=(char*)s;//NEW(char,strlen(s)+1);
		//strcpy(S.str,s);
		S.Allocated=1;
		SHash.add(S);
	}
	__forceinline int  find(const char*s){
		if(!s) return 0xFFFFFFFF;
		Str_for_hash S;
		S.str=(char*)s;
		S.Allocated=0;
		return SHash.find(S);
	}
	__forceinline char* get(int i){
		if(i<0||i>=SHash.numElem())return NULL;
		else return SHash.elem(i).str;
	}
};
class DIALOGS_API CompactStringsHash{	
	Hash<Str_for_hash,257,256>* SHash;
	DynArray<char*>* SimpleList;
public:	
	CompactStringsHash(){
		SHash=NULL;
		SimpleList=NULL;
	}
	__forceinline void clear(){
		if(SHash){
			for(int i=0;i<SHash->numElem();i++){
				if(SHash->elem(i).Allocated&&SHash->elem(i).str)free(SHash->elem(i).str);
				SHash->elem(i).str=NULL;
				SHash->elem(i).Allocated=false;
			}
			SHash->reset();
			delete(SHash);
			SHash=NULL;
		}
		if(SimpleList){
			for(int i=0;i<SimpleList->GetAmount();i++){
				char* s=(*SimpleList)[i];
				if(s)POOL.Free((BYTE*)s,strlen(s)+1);
			}
            SimpleList->Clear();			
		}
	}
	int size(){
		if(SimpleList)return SimpleList->GetAmount();
		if(SHash)return SHash->numElem();
		return 0;
	}
	void add(const char* s){
		if(!s) return;
		if(size()<15){
			if(!SimpleList)SimpleList=new DynArray<char*>;
			char* st=(char*)POOL.Allocate(strlen(s)+1);
			strcpy(st,s);
			SimpleList->Add(st);
            return;
		}else{
			if(!SHash){
				SHash=new Hash<Str_for_hash,257,256>;
				for(int i=0;i<size();i++){
					Str_for_hash S;
					S.str=(*SimpleList)[i];
					S.Allocated=1;
					SHash->add(S);					
				}
				if(SimpleList){
					for(int i=0;i<SimpleList->GetAmount();i++){
						char* s=(*SimpleList)[i];
						if(s)POOL.Free((BYTE*)s,strlen(s)+1);
					}					
					delete(SimpleList);
					SimpleList=NULL;
				}
			}
		}		
		Str_for_hash S;
		S.str=(char*)s;//NEW(char,strlen(s)+1);
		//strcpy(S.str,s);
		S.Allocated=1;
		SHash->add(S);
	}
	__forceinline int  find(const char*s){
		if(!s) return 0xFFFFFFFF;
		if(SimpleList){
			for(int i=0;i<SimpleList->GetAmount();i++){
				if(!strcmp(s,(*SimpleList)[i]))return i;
			}
		}else if(SHash){
			Str_for_hash S;
			S.str=(char*)s;
			S.Allocated=0;
			return SHash->find(S);
		}
		return -1;
	}
	__forceinline char* get(int i){
		if(SimpleList){
            return (*SimpleList)[i];
		}else if(SHash){
			if(i<0||i>=SHash->numElem())return NULL;
			else return SHash->elem(i).str;
		}
		return NULL;
	}
};
#pragma pack (pop)
#endif //__STRHASH_H__