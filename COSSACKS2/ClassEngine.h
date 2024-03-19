#ifndef __CLASSENGINE_H__
#define __CLASSENGINE_H__
#pragma pack(1)
#include "newmemory.h"
#include "..\DipServer\DString.h"
#include "xmlQuote.h"
#include "StrHash.h"
#include "DynArray.h"
#define IMMEDIATE_ERROR
class ErrorPager{
	DString Msg;
public:	
	bool Critical;
	_inline void print(char* mask,...){
		va_list args;
		va_start(args,mask);
		char temp[512];
		vsprintf(temp,mask,args);
		va_end(args);
		Msg.Add(temp);
#ifdef IMMEDIATE_ERROR
		if(MessageBox(NULL,temp,"Class engine error",MB_ICONERROR|MB_OKCANCEL)==IDCANCEL){
			exit(1);
		}
#endif //IMMEDIATE_ERROR
	}
	_inline void xml_print(xmlQuote& xml,char* mask,...){
		va_list args;
		va_start(args,mask);
		char temp[1024];
		vsprintf(temp,mask,args);
		va_end(args);
		DString D;
		xml.GetXMLSource(&D);
		sprintf(temp+strlen(temp),"XML source:\n\n%s\n",D.str);
		Msg.Add(temp);
#ifdef IMMEDIATE_ERROR
		if(MessageBox(NULL,temp,"Class engine error",MB_ICONERROR|MB_OKCANCEL)==IDCANCEL){
			exit(1);
		}
#endif //IMMEDIATE_ERROR
	}
	void Clear();
};
_inline PrintError(char* mask,...){
	va_list args;
	va_start(args,mask);
	char temp[512];
	vsprintf(temp,mask,args);
	va_end(args);
#ifdef IMMEDIATE_ERROR
	if(MessageBox(NULL,temp,"Class engine error",MB_ICONERROR|MB_OKCANCEL)==IDCANCEL){
		exit(1);
	}
#endif //IMMEDIATE_ERROR
}
class BaseClass;
class Enumerators;
class Enumerator;
class OneClassMemberStorage{
public:
	OneClassMemberStorage();
	~OneClassMemberStorage();
	BaseClass* Member;
	char* xmlID;
	int OffsetFromClassRoot;
	bool UseReference;
	void* ExtraData;
};
class OneClassStorage{
public:
	BaseClass* OneMemb;
	DynArray<OneClassMemberStorage*> Members;
	StringsHash MembIDS;
	_inline OneClassMemberStorage* CreateMember(const char* ID){
		OneClassMemberStorage* M=new OneClassMemberStorage;
		MembIDS.add(ID);
        Members.Add(M);
		M->xmlID=NEW(char,strlen(ID)+1);
		strcpy(M->xmlID,ID);
		return M;
	}
};
class Enumerator{
	StringsHash Strings;
	DynArray<DWORD> Values;
public:
	char* EnumeratorID;
	void  Add(char* s){
		Strings.add(s);
		Values.Add(Values.GetAmount());
	}
	void  Add(char* s,DWORD V){
		Strings.add(s);
		Values.Add(V);
	}
	DWORD Get(char* s){
		int idx=Strings.find(s);
		if(idx!=-1){
			return Values[idx];
		}else return -1;
	}
	char* Get(DWORD V){
		int idx=Values.find(V);
		if(idx!=-1){
            return Strings.get(idx);
		}else return "";
	}
	void  Clear(){
		Strings.clear();
        Values.Clear();
	}
};
class Enumerators{
	DynArray<Enumerator*> Enums;
	StringsHash EnumNames;
public:
	_inline Enumerator* Create(const char* Name){
        Enumerator* E=new Enumerator;
		Enums.Add(E);
		EnumNames.add(Name);
		E->EnumeratorID=EnumNames.get(EnumNames.find(Name));
		return E;
	}
	_inline Enumerator* Get(const char* Name){
		int idx=EnumNames.find(Name);
		if(idx!=-1){
            return Enums[idx];
		}else{
			return Create(Name);
		}
	}
	_inline ClearAll(){
		Enums.Clear();
		EnumNames.clear();
	}    
};
Enumerators ENUM;
class ClassGarbage{
	DynArray<OneClassStorage*> Storage;
	StringsHash NamesHash;
public:
	//ClassGarbage();
	//~ClassGarbage();
	_inline OneClassStorage* GetClass(const char* ClassName){
		int idx=NamesHash.find(ClassName);
		if(idx!=-1)return Storage[idx];
		return NULL;
	}
	_inline OneClassStorage* CreateClassStorage(const char* ClassName){
		NamesHash.add(ClassName);
		OneClassStorage* ST=new OneClassStorage;
		Storage.Add(ST);
		return ST;
	}
};
class ClassCreator{
};
#define AUTONEW(x)	virtual BaseClass* new_element(){return new x;}\
	virtual OneClassStorage* GetClassStorage(){\
		static OneClassStorage* OCS=NULL;\
		if(!OCS)OCS=CGARB.GetClass(GetClassName());\
		return OCS;\
	}
//	virtual x* new_element(){return new x;}
extern ClassGarbage CGARB;
class BaseClass{
public:
	_inline void RegisterOneMember(BaseClass* Member,void* ptr,const char* id,void* Extra=NULL){
		OneClassStorage* CST=GetClassStorage();
		if(!CST){
			CST=CGARB.CreateClassStorage(GetClassName());
		}
		OneClassMemberStorage* MS=CST->CreateMember(id);
		MS->Member=Member;
		MS->UseReference=0;
		MS->OffsetFromClassRoot=int(ptr)-int(this);
		MS->ExtraData=Extra;
	}
	_inline void RegisterOneMemberPtr(BaseClass* Member,void* ptr,char* id){
		OneClassStorage* CST=GetClassStorage();
		if(!CST){
			CST=CGARB.CreateClassStorage(GetClassName());
		}
		OneClassMemberStorage* MS=CST->CreateMember(id);
		MS->Member=Member;
		MS->UseReference=1;
		MS->OffsetFromClassRoot=int(ptr)-int(this);
	}
	_inline bool GetElementByName(const char* ElmName,BaseClass** ClassPtr,void** ElmPtr,void** ExtraPtr,void* base){
		int N=GetAmountOfElements();
		for(int i=0;i<N;i++){
			if(!strcmp(ElmName,GetElementID(i))){
				*ClassPtr=GetElementClass(i);
				*ElmPtr=GetElementPtr(i,base);
				*ExtraPtr=GetElementExtraPtr(i);
				return true;
			}
		}
		int idx=ExpandWith(ElmName,base);
		if(idx!=-1){
			*ClassPtr=GetElementClass(idx);
			*ElmPtr=GetElementPtr(idx,base);
			return true;
		}
		return false;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();
	virtual void RegisterMembers();
	virtual int GetAmountOfElements();
	virtual const char* GetElementID(int Index);
	virtual BaseClass* GetElementClass(int Index);
	virtual void* GetElementPtr(int Index,void* base);
	virtual void* GetElementExtraPtr(int Index);
	virtual int ExpandWith(const char* ElmName,void* base){
		return -1;
	}
	AUTONEW(BaseClass);
};
//--implementation of some classes--//
class base_int:public BaseClass{
public:
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();    	
	AUTONEW(base_int);
};
class base_DWORD:public BaseClass{
public:
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();    	
	AUTONEW(base_int);
};
class base_int_enum:public BaseClass{
public:
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL){
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			xml.Assign_string(E->Get(*((int*)ClassPtr)));
		}
	}
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			const char* s=xml.Get_string();
			if(s){
				int p=E->Get((char*)xml.Get_string());
				if(p!=-1){
					*((int*)ClassPtr)=E->Get((char*)xml.Get_string());
					return true;
				}else Error->xml_print(xml,"identifier \"%s\" not found in list \"%s\" for variable \"%s\"\n",xml.Get_string(),E->EnumeratorID,xml.GetQuoteName());
			}else Error->xml_print(xml,"xml node is empty for variable %s\n",xml.GetQuoteName());
		}else Error->xml_print(xml,"Enumerator not assigned for type \"base_int_enum\" for variable %s\n",xml.GetQuoteName());
		return false;
	}
	virtual const char* GetClassName(){
		return "base_int_enum";
	}
	AUTONEW(base_int_enum);
};
class base_int_accamulator:public BaseClass{
public:
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL){
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			xml.Assign_string(E->Get(*((int*)ClassPtr)));
		}
	}
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			char* S=(char*)xml.Get_string();
			if(S[0]){
				int p=E->Get(S);
				if(p==-1){
					E->Add(S);
					p=E->Get(S);
				}
				*((int*)ClassPtr)=p;
				return true;
			}else Error->xml_print(xml,"xml node is empty for variable %s\n",xml.GetQuoteName());
		}else Error->xml_print(xml,"Enumerator not assigned for type <base_int_accamulator> for variable %s\n",xml.GetQuoteName());
		return false;
	}
	virtual const char* GetClassName(){
		return "base_int_accamulator";
	}
	AUTONEW(base_int_enum);
};
#define TEMPL(c,elm) #c"."#elm
template<class X>class ClassArray:public BaseClass,public DynArray<X*>{
public:
	const char* GetClassName(){
		static char N[64]="";
		if(!N[0]){
			X* x=new X;
			sprintf(N,"ClassArray.%s",x->GetClassName());
			delete(x);
		}
		return N;
	}
	int GetAmountOfElements(){
		return GetAmount();
	}
	const char* GetElementID(int Index){
		return (*this)[Index]->GetClassName();
	}
	BaseClass* GetElementClass(int Index){
		return (*this)[Index];
	}
	void* GetElementPtr(int Index,void* base){
		return (*((ClassArray<X>*)base))[Index];
	}
	virtual int ExpandWith(const char* ElmName,void* base){
        ClassArray<X>* BC=(ClassArray<X>*)base;
		OneClassStorage* OCS=CGARB.GetClass(ElmName);
		if(OCS){
			BaseClass* NewBase=OCS->OneMemb;
			if(NewBase){
	            BaseClass* B1=NewBase->new_element();
				BC->Add((X*)B1);
				return BC->GetAmount()-1;
			}else return -1;
		}else return -1;
	}
	AUTONEW(ClassArray);
};
template<class X,class BaseForX>class LinearArray:public BaseClass,public DynArray<X>{
public:
	BaseForX B;
	X B1;
	const char* GetClassName(){
		static char N[64]="";
		if(!N[0]){
			BaseForX* x=new BaseForX;
			sprintf(N,"LinearArray.%s.%s",typeid(X).name(),x->GetClassName());
			delete(x);
		}
		return N;
	}
	int GetAmountOfElements(){
		return GetAmount();
	}
	const char* GetElementID(int Index){
		static char temp[16];
		sprintf(temp,"e%d",Index);
		return temp;
	}
	BaseClass* GetElementClass(int Index){
		return &B;
	}
	void* GetElementPtr(int Index,void* base){
		LinearArray<X,BaseForX>* AR=(LinearArray<X,BaseForX>*)base;
		return AR->GetValues()+Index;
	}
	virtual int ExpandWith(const char* ElmName,void* base){
		LinearArray<X,BaseForX>* BC=(LinearArray<X,BaseForX>*)base;
		BC->Add(B1);
		return BC->GetAmount()-1;
	}
	typedef LinearArray<X,BaseForX> LA;
	AUTONEW(LA);
};

#define REG_MEMBER(type,fieldID)\
	{\
		type* m=new type;\
		RegisterOneMember(m,&fieldID,#fieldID);\
	}
#define REG_ENUM(type,fieldID,EnumID)\
{\
	type* m=new type;\
	Enumerator* E=ENUM.Get(#EnumID);\
	if(!E)E=ENUM.Create(#EnumID);\
	RegisterOneMember(m,&fieldID,#fieldID,E);\
}
_inline char* ConvTypeName(DString& D){
	D.Replace("class ","");
	D.Replace("<",".");
	D.Replace(">","");
	D.Replace(",",".");
	return D.str;
}
#define TOSTR(x) #x
template <class X>
void reg_auto(BaseClass* S,X& x,void* EnumID=NULL){
	DString D;
	D.Assign((char*)typeid(X).name());
	ConvTypeName(D);
	X* m=new X;
	S->RegisterOneMember(m,&x,D.str,EnumID);
	D.Free();
}
template <class X>
void reg_class(X* x){
	X* elm=(X*)x->new_element();
	DString D;
	D.Assign((char*)typeid(X).name());
	ConvTypeName(D);
	OneClassStorage*OCS=CGARB.GetClass(D.str);
	if(!OCS)OCS=CGARB.CreateClassStorage(D.str);
	if(OCS->OneMemb){
        delete(elm);
	}else OCS->OneMemb=elm;
	D.Free();
}
#define REG_AUTO(x) reg_auto(this,x);reg_class(&x)
#define REG_AUTO_ENUM(x,y) {Enumerator* E=ENUM.Get(#y);reg_auto(this,x,E);reg_class(&x);}
#define REG_CLASS(x) {x*m;reg_class(m=new(x));delete(m);}
#ifdef IMPLEMENT_CLASS_FACTORY
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
//~~~~~~~~~~~BaseClass~~~~~~~~~~~/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
const char* BaseClass::GetClassName(){
	return "BaseClass";
}
void BaseClass::RegisterMembers(){
}
int BaseClass::GetAmountOfElements(){
	OneClassStorage* CST=GetClassStorage();
	if(CST)return CST->Members.GetAmount();
	else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return 0;
	}
}
const char* BaseClass::GetElementID(int Index){
	OneClassStorage* CST=GetClassStorage();
	assert(Index>=0&&Index<CST->Members.GetAmount());
	if(CST)return CST->Members[Index]->xmlID;
	else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return 0;
	}
}
BaseClass* BaseClass::GetElementClass(int Index){
	OneClassStorage* CST=GetClassStorage();
	assert(Index>=0&&Index<CST->Members.GetAmount());
	if(CST)return CST->Members[Index]->Member;
	else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return 0;
	}
}
void* BaseClass::GetElementPtr(int Index,void* base){
	OneClassStorage* CST=GetClassStorage();
	assert(Index>=0&&Index<CST->Members.GetAmount());
	if(CST){
		if(CST->Members[Index]->UseReference){
	     return *((char**)(((char*)base)+CST->Members[Index]->OffsetFromClassRoot));
		}else return ((char*)base)+CST->Members[Index]->OffsetFromClassRoot;
	}else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return NULL;
	}
}
void* BaseClass::GetElementExtraPtr(int Index){
	OneClassStorage* CST=GetClassStorage();
	if(CST){
		if(Index>=0&&Index<CST->Members.GetAmount()){
		    return CST->Members[Index]->ExtraData;
		}else return NULL;
	}else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return 0;
	}
}
void BaseClass::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	RegisterMembers();
	BaseClass* BC=(BaseClass*)ClassPtr;
	int N=BC->GetAmountOfElements();
	for(int i=0;i<N;i++){
		xmlQuote* xi=new xmlQuote((char*)BC->GetElementID(i));
		void* mptr=BC->GetElementPtr(i,ClassPtr);
		void* eptr=BC->GetElementExtraPtr(i);
		if(!eptr)eptr=Extra;
		if(mptr){
            BaseClass* BC1=BC->GetElementClass(i);
			BC1->Save(*xi,mptr,eptr);
		}else{
			xi->Assign_string("NULL");
		}
		xml.AddSubQuote(xi);
	}
}
bool BaseClass::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	RegisterMembers();
	BaseClass* BC=(BaseClass*)ClassPtr;
	int N=xml.GetNSubQuotes();
	for(int i=0;i<N;i++){
		xmlQuote* sub=xml.GetSubQuote(i);
		BaseClass* BASE;
		void* BasePtr;
		void* Extra2=NULL;
		if(BC->GetElementByName(sub->GetQuoteName(),&BASE,&BasePtr,&Extra2,ClassPtr)){
			if(!Extra2)Extra2=Extra;
			BASE->Load(*sub,BasePtr,Error,Extra2);
		}else{
			Error->xml_print(xml,"Unknown identifier %s of class %s.\n",sub->GetQuoteName(),BC->GetClassName());
            return false;
		}
	}
	return true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
//~~~~~~~~~~~base_int~~~~~~~~~~~~/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
void base_int::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	int* V=(int*)ClassPtr;
	xml.Assign_int(*V);
}
bool base_int::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	int* V=(int*)ClassPtr;
	const char* c=xml.Get_string();
	*V=0;
	int z=0;
	if(c)z=sscanf(c,"%d",V);
	if(z!=1){
		Error->Critical=1;
		Error->xml_print(xml,"unable to read DWORD from XML: %s\n",xml.Get_string());
		return false;
	}
	return true;
};
const char* base_int::GetClassName(){
	return "base_int";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
//~~~~~~~~~~~base_DWORD~~~~~~~~~~~~/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
void base_DWORD::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	DWORD* V=(DWORD*)ClassPtr;
	char c[16];
	sprintf(c,"%X",*V);
	xml.Assign_string(c);
}
bool base_DWORD::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	DWORD* V=(DWORD*)ClassPtr;
	const char* c=xml.Get_string();
	*V=0;
	int z=0;
	if(c)z=sscanf(c,"%X",V);
	if(z!=1){
		Error->Critical=1;
		Error->xml_print(xml,"unable to read DWORD from XML: %s\n",xml.Get_string());
		return false;
	}
	return true;
};
const char* base_DWORD::GetClassName(){
	return "base_DWORD";
}
OneClassMemberStorage::OneClassMemberStorage(){
	Member=NULL;
	xmlID=NULL;
	UseReference=0;
}
OneClassMemberStorage::~OneClassMemberStorage(){
	if(Member)delete(Member);
	if(xmlID)free(xmlID);
	Member=NULL;
	xmlID=NULL;
}
ClassGarbage CGARB;

#endif //IMPLEMENT_CLASS_FACTORY

#endif __CLASSENGINE_H__