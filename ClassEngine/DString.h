#if !defined(DIP_SERVER_DSTRING_DEFFF)
#define DIP_SERVER_DSTRING_DEFFF
#include "..\ClassEngine\dynarray.h"
#include "..\ClassEngine\pool.h"

class DString;
class DIALOGS_API DString{
protected:
	char* str;
public:		
	int L;
	int MaxL;
	char* pchar(const char* defaultstr=""){
		return str?str:(char*)defaultstr;        
	}
	const char* cpchar(const char* defaultstr="")const{
		return str?str:(char*)defaultstr;        
	}
	const char* operator()(){
		return str?str:"";
	}
	bool equal(const char* s){
		return s&&!strcmp(s,pchar());
	}
	bool equal(DString& s){
		return !strcmp(s.pchar(),pchar());
	}
	DString();
	DString(char* s);
	~DString();
	void Allocate(int LN);
	void ReadFromFile(char* file);
	void WriteToFile (char* file);
	void Add(DString& Str);
	void Add(const char* Str);
	void Add(char Ch);
	void Add(int v);
	void print(char* mask,...);
	void Replace0(char* src,char* dst);
	void Replace(char* src,DString& dst);
	void Replace(char* src,char* dst,...);
	void Replace(char* src,int value);
	void Clear();
	inline bool isClear(){ return str==NULL||str[0]==0; }
	void Free();
	void Assign(char* Str);
	void Assign(DString& Str);
	void Assign(const int& a);
	//DString& operator = (char* s){
	//		Assign(s);
	//		return *this;
	//	};
	DString& operator = (const char* s){
			Assign((char*)s);
			return *this;
		};
	//DString& operator = (DString& ds1){
	//		Assign(ds1);
	//		return *this;
	//	};
	DString& operator = (const DString& ds1){
			Assign((DString&)ds1);
			return *this;
		};
	DString& operator = (const int& a){
			Assign(a);
			return *this;
		};
	DString& operator + (const int& a){
			Add((int)a);
			return *this;
		};
	DString& operator + (char* s){
			Add(s);
			return *this;
		};
	DString& operator + (const char* s){
			Add((char*)s);
			return *this;
		};
	DString& operator + (DString& ds1){
			Add(ds1.pchar());
			return *this;
		};
	DString& operator + (const DString& ds1){
			Add(ds1.cpchar());
			return *this;
		};
	DString& operator += (const int& a){
			Add((int)a);
			return *this;
		};
	DString& operator += (char* s){
			Add(s);
			return *this;
		};
	DString& operator += (const char* s){
			Add((char*)s);
			return *this;
		};
	DString& operator += (DString& ds){
			Add(ds.pchar());
			return *this;
		};
	DString& operator += (const DString& ds){
			Add(ds.cpchar());
			return *this;
		};
	bool operator == (char* s){
			if(s&&str){
				return !strcmp(s,str);
			}else return false;
		};
	bool operator == (const char* s){
			if((char*)s&&str){
				return !strcmp(s,str);
			}else return false;
		};
	bool operator == (DString& ds){
			if(ds.str&&str){
				return !strcmp(ds.str,str);
			}else return false;
		};
	bool operator == (const DString& ds){
			if(ds.str&&str){
				return !strcmp(ds.str,str);
			}else return false;
		};
	bool operator != (char* s){
		return !(this->operator ==(s));
		};
	bool operator != (const char* s){
		return !(this->operator ==(s));
		};
	bool operator != (DString& ds){
		return !(this->operator ==(ds));
		};
	bool operator != (const DString& ds){
		return !(this->operator ==(ds));
		};
	char operator [] (int index){
			if(str&&index<L)return str[index];
			else return 0;
		};
	void ExtractLine(DString& dst);
	void ExtractWord(DString& dst);
	void ExtractWord2(DString& dst);
	void DeleteChars(int From, int N);
	void Limit(int maxL);
};
#endif
