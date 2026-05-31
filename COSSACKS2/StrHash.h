#ifndef __STRHASH_H__
#define __STRHASH_H__
#include <kHash.hpp>
#ifndef NEW
#define NEW(t,s) (t*)malloc((s)*sizeof(t)) 
#endif
class StringsHash{
	class Str{
	public:
		bool Allocated;
		char* str;
		__forceinline unsigned int	hash	() const{
			int L=strlen(str);
			DWORD S=0;
			for(int i=0;i<L;i++)S+=str[i];
			return S;
		}
		__forceinline bool			equal	( const Str& el ){
			return !strcmp(el.str,str);
		}
		__forceinline void			copy	( const Str& el ){
			if(str&&Allocated)free(str);
			str=NEW(char,strlen(el.str)+1);
			Allocated=1;
			strcpy(str,el.str);
		}
		Str(){
			str=NULL;
			Allocated=0;
		}
		~Str(){
			if(str&&Allocated)free(str);
			str=NULL;
		}
	};
	Hash<Str,1027,1024> SHash;
public:
	__forceinline void clear(){
		SHash.reset();        
	}
	void add(const char* s){
		Str S;
		S.str=NEW(char,strlen(s)+1);
		strcpy(S.str,s);
		S.Allocated=1;
		SHash.add(S);
	}
	__forceinline int  find(const char*s){
		Str S;
		S.str=(char*)s;
		S.Allocated=0;
		return SHash.find(S);
	}
	__forceinline char* get(int i){
		if(i<0||i>=SHash.numElem())return NULL;
		else return SHash.elem(i).str;
	}
};
#endif //__STRHASH_H__