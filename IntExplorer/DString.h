#if !defined(DSTRING_DEFFF)
#define DSTRING_DEFFF

//#include <crtdbg.h>
//#include <assert.h>
//#include "..\common.h"
//#include "IntExplorer.h"
//#include "ParseRQ.h"

class DString;
class DString{
public:
	char* str;
	int L;
	int MaxL;
	DString(){
			str=NULL;
			L=0;
			MaxL=0;
		};
	DString(char* s){
			str=NULL;
			L=0;
			MaxL=0;
			Assign(s);
		};
	~DString(){
			if(str)free(str);
			str=NULL;
			L=0;
			MaxL=0;
		};
	void Allocate(int LN){
			if(MaxL<LN){
				MaxL=((LN+256)&0xFFFF00);
				str=(char*)realloc(str,MaxL);
				str[L]=0;
			};
		};
	void ReadFromFile(char* file){
			Clear();
			ResFile F=RReset(file);
			if(F!=INVALID_HANDLE_VALUE){
				int sz=RFileSize(F);
				if(sz){
					Allocate(sz+1);
					RBlockRead(F,str,sz);
					str[sz]=0;
					L=strlen(str);
				};
				RClose(F);
			};
		};
	void WriteToFile (char* file){
			ResFile F=RRewrite(file);
			if(F!=INVALID_HANDLE_VALUE){
				if(str)RBlockWrite(F,str,L);
				RClose(F);
			};
		};
	void Add(DString& Str){
			if(!Str.str)return;
			Allocate(L+Str.L+1);
			strcat(str,Str.str);
			L=strlen(str);
		}; 
	void Add(const char* Str){
			Allocate(L+strlen(Str)+1);
			strcat(str,Str);
			L=strlen(str);
		};
	void Replace0(char* src,char* dst){
		if(!str)return;
		char* s=str;
		int L0=strlen(src);
		int L1=strlen(dst);
		do{
			s=strstr(s,src);
			if(s){
				int pos=s-str;
				Allocate(L+L1-L0+1);
				memmove(str+pos+L1,str+pos+L0,L-pos-L0+1);
				memcpy(str+pos,dst,L1);
				L=strlen(str);
				s=str+pos+L1;
			};
		}while(s);
	};
	/*
	void Replace(char* src,char* dst){
		if(!str)return;
		char* s=str;
		int L0=strlen(src);
		int L1=strlen(dst);
		do{
			s=strstr(s,src);
			if(s){
				int pos=s-str;
				Allocate(L+L1-L0+1);
				memmove(str+pos+L1,str+pos+L0,L-pos-L0+1);
				memcpy(str+pos,dst,L1);
				L=strlen(str);
				s=str+pos+L1;
			};
		}while(s);
	};
	*/
	void Replace(char* src,DString& dst){
			if(!dst.str)return;
			Replace(src,dst.str);
		};
	void Replace(char* src,char* dst,...){
		char ccc[512];
        va_list va;
        va_start(va,dst);
        vsprintf (ccc,dst,va);   
        va_end(va);
		Replace0(src,ccc);
	};
	void Replace(char* src,int value){
		char cc[16];
		sprintf(cc,"%d",value);
		Replace(src,cc);
	};
	void Clear(){
			L=0;
			if(str)str[0]=0;
		};
	void Free(){
			if(str)free(str);
			str=NULL;
			L=0;
			MaxL=0;
		};
	void Assign(char* Str){
			Clear();
			L=strlen(Str);
			Allocate(L+1);
			strcpy(str,Str);
		};
	void Assign(DString& Str){
			if(Str.str)Assign(Str.str);
			else Clear();
		};
	DString& operator = (char* s){
			Assign(s);
			return *this;
		};
	DString& operator + (char* s){
			Add(s);
			return *this;
		};
	DString& operator + (DString& ds1){
			Add(ds1);
			return *this;
		};
	DString& operator += (char* s){
			Add(s);
			return *this;
		};
	DString& operator += (DString& ds){
			Add(ds);
			return *this;
		};
	bool operator == (char* s){
			if(s&&str){
				return !strcmp(s,str);
			}else return false;
		};
	bool operator == (DString& ds){
			if(ds.str&&str){
				return !strcmp(ds.str,str);
			}else return false;
		};
	char operator [] (int index){
			if(str&&index>L)return str[index];
			else return 0;
		};
	void ExtractLine(DString& dst)
		{
			dst.Clear();
			char seps[]="\n";
			char *token;
			DString rez;
			rez.Assign(str);
			token=strtok(str,seps);
			if(token!=NULL)
			{
				dst.Add(token);
				int s=strlen(token);
				char* c;
				if(s<rez.L)
					c=(char*)(strstr(rez.str,token)+s+1);
				else 
					c=(char*)(strstr(rez.str,token)+s);
				Assign(c);
			}
		}
	void ExtractWord(DString& dst)
		{
			dst.Clear();
			char seps[]=" ,;:\t\n\r";
			char *token;
			DString rez;
			rez.Assign(str);
			token=strtok(str,seps);
			if(token!=NULL)
			{
				dst.Add(token);
				int s=strlen(token);
				char* c;
				if(s<rez.L)
					c=(char*)(strstr(rez.str,token)+s+1);
				else 
					c=(char*)(strstr(rez.str,token)+s);
				Assign(c);
			}else 
				Clear();
		}
	void DeleteChars(int From, int N)
		{
			if((From+N)<=L)
			{
				memmove(str+From,str+From+N,L-From-N);
				L-=N;
				str[L]=0;
			}
		}
	void Add(int nn)
		{
			char buff[32];
			if(itoa(nn,buff,10))
				Add(buff);
		}
};
#endif