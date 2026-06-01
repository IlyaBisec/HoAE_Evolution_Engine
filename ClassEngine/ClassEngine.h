#ifndef __CLASSENGINE_H__
#define __CLASSENGINE_H__
#pragma pack(push)
#pragma pack(1)
#include "..\ClassEngine\dynarray.h"
#include "..\ClassEngine\DString.h"
#include "..\ClassEngine\xmlQuote.h"
#include "..\ClassEngine\StrHash.h"
#include <typeinfo>
#define IMMEDIATE_ERROR
#define SAFETYPE

const int nCP_Branches=256;

DIALOGS_API void CheckDynPtr(void* Ptr);

DIALOGS_API void PushSmartLeak(bool& v);
DIALOGS_API void PopSmartLeak(bool& v);
#define SAFENEW {bool sl;PushSmartLeak(sl);
#define ENDNEW PopSmartLeak(sl);}
char* GetGlobalBuffer();
class DIALOGS_API ErrorPager{
	DString Msg;
public:	
	bool BreakExecution;
	ErrorPager(){
		BreakExecution=0;
	}
	ErrorPager(int bx){
		BreakExecution=bx!=0;
	}
	void print(char* mask,...);
	void xml_print(xmlQuote& xml,char* mask,...);
	void Clear();
};
DIALOGS_API void PrintError(char* mask,...);

class BaseClass;
class Enumerators;
class Enumerator;

typedef int   tpIntPropertyReader(void*);
typedef void* tpPropertyReader(void*);
typedef void  tpIntPropertyWriter(void*,int data);
typedef void  tpPropertyWriter(void*,void*);

class DIALOGS_API BinStream{
public:
	virtual void  Write(void* data,int size)=0;
	virtual int   Read(void* data,int size)=0;
	virtual void  WriteToFile(const char* Name)=0;
	virtual void  ReadFromFile(const char* Name)=0;
	virtual int   Size()=0;
	virtual void  SetReadPos(int pos)=0;
	virtual void  SetWritePos(int pos)=0;
	virtual int   GetReadPos()=0;
	virtual int   GetWritePos()=0;
	virtual BYTE* GetData()=0;
	virtual void  Clear()=0;
			void  WriteBYTE(BYTE c);
			void  WriteWORD(WORD w);
			void  WriteDWORD(DWORD D);
			BYTE  ReadBYTE();  
			WORD  ReadWORD();
			DWORD ReadDWORD();
};
class DIALOGS_API MemoryBinStream:public BinStream{
	BYTE* Data;
	int _Size;
	int _MaxSize;
	int _WritePos;
	int _ReadPos;
public:
	MemoryBinStream();
	~MemoryBinStream();
			void  CheckSize(int size);
	virtual void  Write(void* data,int size);
	virtual int   Read(void* data,int size);
	virtual void  WriteToFile(const char* Name);
	virtual void  ReadFromFile(const char* Name);
	virtual int   Size();
	virtual void  SetReadPos(int pos);
	virtual void  SetWritePos(int pos);
	virtual int   GetReadPos();
	virtual int   GetWritePos();
	virtual BYTE* GetData();
	virtual void  Clear();
};

class DIALOGS_API OneClassMemberStorage{
public:
	OneClassMemberStorage();
	~OneClassMemberStorage();
	BaseClass* Member;
	char* xmlID;
	int OffsetFromClassRoot;	
	bool UseReference;
	bool ReadOnly;
	bool Invisible;
	bool StaticMode;
	bool NoSaveMode;
	void* ExtraData;
	tpIntPropertyReader* IntPropertyR;
	tpPropertyReader*    GeneralPropertyR;
	tpIntPropertyWriter* IntPropertyW;
	tpPropertyWriter*    GeneralPropertyW;
	const char* HostClass;
	bool DirectCasting;
    DWORD Mask;
	void* GetElmPtr(void* Base){
		return StaticMode?(void*)OffsetFromClassRoot:(void*)(int(Base)+OffsetFromClassRoot);
	}
	void CheckValidRegistration(void* Base,const char* MemName);
};
class OneClassMemberInfo{
public:
    BaseClass*              ParentClass;
    void*                   ParentPtr;
    BaseClass*              ClassPtr;
    void*                   Data;
    void*                   Extra;
    OneClassMemberStorage*  OCMS;
public:
    BaseClass*  GetClass();
    void*       GetData();
    void*       GetExtra();

    BaseClass*  GetParentClass();
    void*       GetParentPtr();

    void        AcceptAfterWrite();
    void        Save(xmlQuote* xml);
    void        Load(xmlQuote* xml,ErrorPager* ep=NULL);

    bool        IsInvisible();
    bool        IsReadOnly();
    bool        IsStatic();
    bool        IsNotSavable();
    bool        IsDirectCasting();
    DWORD       GetMask();
    const char* Name();
};
class DIALOGS_API OneClassStorage{
public:
	OneClassStorage(){
		OneMemb=NULL;
		CurrentSet=-1;
		ClassTopic=NULL;
	}
	~OneClassStorage();
	int CurrentSet;
	BaseClass* OneMemb;
	DynArray<OneClassMemberStorage*> Members;
	DynArray<char*>Children;
	DynArray<char*>Parents;
	CompactStringsHash MembIDS;
	char* ClassTopic;
	OneClassMemberStorage* CreateMember(const char* ID);

	bool	CheckChildren	(const char* ChildrenName);
	bool	CheckParent		(const char* ParentName  );
	bool	CheckAll		(const char* string, bool inParent, bool inChildren, bool FullCompatibility);
private:
	bool	CheckString( DynArray<char*>* pList, const char* string, bool FullCompatibility=true );
};
//general case of procedural enumerator
//yo can get current context by calling BaseCless::GetContext()
class DIALOGS_API ProcEnumerator{
public:
	virtual DWORD GetValue(const char* ID){return 0;}	
	virtual char* GetValue(DWORD ID){return "";}	

	virtual int   GetAmount(){return 0;}
	virtual char* GetIndexedString(int idx){return "";}
	virtual DWORD GetIndexedValue (int idx){return 0;}

	virtual char* GetCategory(int idx){return NULL;}	
};
//////////////////////////////////////////////////////////////////////////
///////////         SIMPLIFIED PROCEDURAL ENUMERATOR          ////////////
//////////////////////////////////////////////////////////////////////////

class DIALOGS_API EnumeratorOutput{
public:
	void Init();
	void Free();
	EnumeratorOutput();
	~EnumeratorOutput();
	//fields to be filled by user
	int   Amount;				//amount of elements in enumerator
	char* String;				//string value for enumerator item
    DWORD Value;				//DWORD value for enumerator item
	char* Topic;				//[optional] topic for enumerator item
	//next fields are not supported now. May be in future
	bool  StringAllocated;		//[optional] true if String is not reference, but allocated.
								//In this case String should be delete[]d by class engine
	bool  TopicAllocated;		//[optional] true if Topic is not reference, but allocated. 
								//In this case String should be delete[]d by class engine
};
class DIALOGS_API EnumeratorContext{
public:
	EnumeratorContext(int _ValueIndex);
	BaseClass*	Context;   //pointer to the class, that contains this enumerator
	int			ValueIndex;//index of enumerator item
	const char* FieldName; //registered name of enumerator 
	BaseClass*  EnumClass; //class of enumerator
	void*       EnumData;  //data ptr of enumerator

};
//this function should return true and fill "out" if element valindex exists, othervice return false;
typedef bool fnGetValueForEnumerator(EnumeratorContext* in,EnumeratorOutput* out);
//very simplified version of context-dependent procedural enumerator
//To use it:
//1)create callback function of type fnGetValueForEnumerator, for example MyFunc
//2)call one time somewhere the next code:
//  Enumerator* E=ENUM.Get("ENUMERATOR_NAME");
//  E->SetProcEnum(new SimpleProcEnumerator(&MyFunc);

class DIALOGS_API SimpleProcEnumerator:public ProcEnumerator{
	fnGetValueForEnumerator* Value;
	virtual DWORD GetValue(const char* ID);
	virtual char* GetValue(DWORD ID);
	virtual int   GetAmount();
	virtual char* GetIndexedString(int idx);
	virtual DWORD GetIndexedValue (int idx);
	virtual char* GetCategory(int idx);
public:
	SimpleProcEnumerator(fnGetValueForEnumerator* fn);	
};


//////////////////////////////////////////////////////////////////////////
///////////         REFRESHABLE PROCEDURAL ENUMERATOR         ////////////
//////////////////////////////////////////////////////////////////////////
class RefreshableEnumeratorContext;
class EnumeratorOutputStream;
typedef DWORD fnRecreateEnumerator(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

class DIALOGS_API EnumeratorOutputStream{
	friend class RefreshableEnumerator;
	struct StreamItem{
		friend class RefreshableEnumerator;
		char* string;
		DWORD value;
		char* topic;
	};
	fnRecreateEnumerator* FN;
	DynArray<StreamItem> Items;
	DWORD Hash;	
	int RealAmount;
public:
	EnumeratorOutputStream();
	~EnumeratorOutputStream();
	void add(const char* string,DWORD value,const char* topic);
	void sortByString();

	friend	int g_StringCompare( const void *arg1, const void *arg2 );

};
int g_StringCompare( const void *arg1, const void *arg2 );

class DIALOGS_API RefreshableEnumeratorContext{
public:
	RefreshableEnumeratorContext();
	BaseClass*	Context;   //pointer to the class, that contains this enumerator	
	const char* FieldName; //registered name of enumerator 
	BaseClass*  EnumClass; //class of enumerator
	void*       EnumData;  //data ptr of enumerator
};

class DIALOGS_API RefreshableEnumerator:public ProcEnumerator{
	EnumeratorOutputStream Stream;
    void	RefreshStream();
	virtual DWORD GetValue(const char* ID);
	virtual char* GetValue(DWORD ID);
	virtual int   GetAmount();
	virtual char* GetIndexedString(int idx);
	virtual DWORD GetIndexedValue (int idx);
	virtual char* GetCategory(int idx);	
public:
	RefreshableEnumerator(fnRecreateEnumerator* fn);
};

//////////////////////////////////////////////////////////////////////////
///////////                  STATIC ENUMERATOR                ////////////
//////////////////////////////////////////////////////////////////////////

class DIALOGS_API Enumerator{
	CompactStringsHash Strings;
	DynArray<DWORD> Values;
	ProcEnumerator* ProcEnum;
public:
	Enumerator(){
		ProcEnum=NULL;
	}	
	~Enumerator(){
        if(ProcEnum)delete(ProcEnum);
		ProcEnum=NULL;
	}
	CompactStringsHash Topics;	
	DynArray<DWORD> TopicRefs;
	char* EnumeratorID;
	void SetProcEnum(ProcEnumerator* Proc){
        if(ProcEnum)delete(ProcEnum);
		ProcEnum=Proc;
	}
	void  Add(char* s){
		Add(s,Values.GetAmount());
	}
	void  Add(char* s,DWORD V){
		int id=Strings.find(s);
		if(id!=-1){			
			Values[id]=V;            
		}else{
			Strings.add(s);
			Values.Add(V);
			TopicRefs.Add(0xFFFFFFFF);
		}
	}
	void  Add(char* s,DWORD V,char* Topic){
		int id=Strings.find(s);
		int tid=Topics.find(Topic);
		if(tid==-1){
            Topics.add(Topic);
			tid=Topics.find(Topic);
		}
		if(id!=-1){			
			Values[id]=V;            
		}else{
			Strings.add(s);
			Values.Add(V);
			TopicRefs.Add(tid);
		}
	}
	void  Add(char* s,char* Topic){
		Add(s,Values.GetAmount(),Topic);
	}
	DWORD Get(char* s){
		if(ProcEnum){
			DWORD V=ProcEnum->GetValue(s);
			if(V!=0xFFFFFFFF)return V;
		}
		int idx=Strings.find(s);
		if(idx!=-1){
			return Values[idx];
		}else return -1;
	}
	char* Get(DWORD V){
		if(ProcEnum){
			char* s=ProcEnum->GetValue(V);
			if(s)return s;
		}
		int idx=Values.find(V);
		if(idx!=-1){
            return Strings.get(idx);
		}else return "";
	}
	char* GetStr(int idx){
		if(idx<Values.GetAmount())return Strings.get(idx);
		else if(ProcEnum)return ProcEnum->GetIndexedString(idx-Values.GetAmount());
		return "";
	}
	DWORD GetVal(int idx){
		if(idx<Values.GetAmount())return Values[idx];
		else if(ProcEnum) return ProcEnum->GetIndexedValue(idx-Values.GetAmount());
		return 0;
	}
	int GetAmount(){
		return Values.GetAmount()+(ProcEnum?ProcEnum->GetAmount():0);
	}
	int FindStr(char* s){
		int idx=Strings.find(s);
		if(idx!=-1)return idx;
		if(ProcEnum){
			int N=ProcEnum->GetAmount()-Values.GetAmount();
			for(int i=0;i<N;i++){
				char * si=ProcEnum->GetIndexedString(i);
				if(si&&!strcmp(si,s))return Values.GetAmount()+i;
			}
		}
		return -1;
	}
	int FindVal(DWORD v){
		int idx=Values.find(v);
		if(idx!=-1)return idx;
		if(ProcEnum){
			int N=ProcEnum->GetAmount()-Values.GetAmount();
			for(int i=0;i<N;i++){
				DWORD V=ProcEnum->GetIndexedValue(i);
				if(V==v)return Values.GetAmount()+i;
			}
		}
		return -1;
	}
	void  Clear(){
		Strings.clear();
        Values.Clear();
		Topics.clear();	
		TopicRefs.Clear();
	}
	virtual char* GetCategory(int idx){
		if(idx<Values.GetAmount()){
            DWORD id=TopicRefs[idx];
			if(id!=0xFFFFFFFF){
                return Topics.get(id);
			}
			return NULL;
		}else{
            if(ProcEnum)return ProcEnum->GetCategory(idx-Values.GetAmount());
		}
		return NULL;
	}	
};
class DIALOGS_API Enumerators{
	DynArray<Enumerator*> Enums;
	StringsHash EnumNames;
public:
	Enumerator* Create(const char* Name);
	Enumerator* Get(const char* Name);
	void ClearAll();
};

extern DIALOGS_API Enumerators ENUM;

class DIALOGS_API ClassGarbage{
public:
	DynArray<OneClassStorage*> Storage;
	StringsHash NamesHash;

	//ClassGarbage();
	//~ClassGarbage();
	OneClassStorage* GetClass(const char* ClassName);
	OneClassStorage* CreateClassStorage(const char* ClassName);
};
extern DIALOGS_API void TestClassStorage(OneClassStorage** OCS);
#define AUTONEW(x)	virtual BaseClass* new_element(){BaseClass* X=new x;X->GetObjectGlobalID();return X;}\
	virtual OneClassStorage* GetClassStorage(){\
		static OneClassStorage* OCS=NULL;\
		TestClassStorage(&OCS);\
		if(!OCS)OCS=CGARB.GetClass(GetClassName());\
		return OCS;\
	}
//	virtual x* new_element(){return new x;}
extern DIALOGS_API ClassGarbage CGARB;
struct ClassExpParams{
	DynArray<char*> ExpList;
	DynArray<char*> TopicsList;
	DynArray<int>   TopicsIdxs;
};
class BaseClass;
class _str;
struct OneClassPointer{
	DWORD Index;
	int RefCount;
	BaseClass* ClassPtr;
	BaseClass* ROOT;
	BaseClass* FirstRef;
	_str* NamePtr;
	DString RefName;
	DString ClassName;
	OneClassPointer(){
		Index=0;
		RefCount=0;
		ClassPtr=0;
		ROOT=NULL;
		FirstRef=NULL;
	}
};

class DIALOGS_API ClassPointersGarbage{	
public:
	DynArray<OneClassPointer*> CPointer[nCP_Branches];
	DynArray<OneClassPointer*> FreePtrs[nCP_Branches];

	~ClassPointersGarbage();

	int GetBranch(const char* ClassName);
	DynArray<OneClassPointer*>& GetCPointer(char* ClassName);

    DWORD AddClass(BaseClass* Ptr);
	void  DeleteClass(BaseClass* Ptr);
	DWORD AddRef(char* ClassID,char* MemberName,BaseClass* ROOT);
	void DelRef(DWORD ID);
	void AddRef(DWORD ID);
	DWORD TryToLinkClass(BaseClass* ClassPtr);
	DWORD FindClass(BaseClass* ClassPtr);
};
extern DIALOGS_API ClassPointersGarbage CPGARB;
class _str;
class CopyContex{
public:
	CopyContex(){
		LARGE_INTEGER L;
		QueryPerformanceCounter(&L);
		CopyGlobalID=L.LowPart;
	}
	DynArray<DWORD> LocalClassIDS;
	DWORD CopyGlobalID;
};
class DIALOGS_API BaseClass{
protected:
	//there the pointer to the parent should be stored. BaseClass is like a tree, and you always
	//can get the root of the tree using ParentBC
	BaseClass* ParentBC;
	static const char* CurrentSaveFile;
	static bool ReadOnlyMode;
	static bool InvisibleMode;	
	static bool StaticMode;
	static bool NoSaveMode;	
	static BaseClass* CurrentContext;
public:

	static bool SaveInShortForm;

	_inline static BaseClass* GetContext(){return CurrentContext;}
	_inline static void SetContext(BaseClass* BC){CurrentContext=BC;}

	//BaseClass& operator = (BaseClass& bc){
    //    assert(0);
	//	return *this;
	//}
	BaseClass& operator = (const BaseClass& bc){
		assert(0);
		return *this;
	}
	static const char* GetCurrentSaveFile(){
		return CurrentSaveFile;
	};
	//BaseClass is like a tree, and you always
	//can get the root of the tree using this function
	BaseClass* GetRoot();
	BaseClass();
	virtual ~BaseClass();
	//clearing all registered content of the class
	virtual void reset_class(void* DataPtr);
	//registration the member of the class. Used if section SAVE..ENDSAVE
	void SetReadOnlyMode(){ReadOnlyMode=true;};
	void SetInvisibleMode(){InvisibleMode=true;};
	void SetStaticMode(){StaticMode=true;};
	void SetNoSaveMode(){NoSaveMode=true;};
	void RegisterOneMember(BaseClass* Member,void* ptr,const char* id,void* Extra=NULL,const char* Host=NULL,bool DirectCast=false,DWORD Mask=0xFFFFFFFF);
	void RegisterOneMemberIntProperty(BaseClass* Member,tpIntPropertyReader* IntPropR,tpIntPropertyWriter* IntPropW,const char* id,void* Extra=NULL,const char* Host=NULL,DWORD Mask=0xFFFFFFFF);
	void RegisterOneMemberProperty(BaseClass* Member,tpPropertyReader* PropR,tpPropertyWriter* PropW,const char* id,void* Extra=NULL,const char* Host=NULL,DWORD Mask=0xFFFFFFFF);	
	//getting class member by its name. This function  returns:
	//ClassPtr - the pointer to the object, that has type of the member, 
	//for example fot int it will return pointer to _int object
	//ElmPtr - the pointer to the member data. You shoul remember, that ElmPtr and ClassPtr are
	//essentialy different pointers. ClassPtr is a type of object, ElmPtr is a pointer to the 
	//physical storage of the object
	//ExtraPtr - Some additional data, that was stored during RegisterOneMember call. It can be simply used 
	//for enumerators, also for for dependent objects 
	bool GetElementByName(const char* ElmName,BaseClass** ClassPtr,void** ElmPtr,void** ExtraPtr,void* base);
	bool FindElementByName(const char* ElmName,BaseClass** ClassPtr,void** ElmPtr,void** ExtraPtr,void* base);
	//returns information block about given member
	OneClassMemberStorage* GetElementStorage(const char* ElmName);
	const char* GetElementHostClass(const char* ElmName);
	const char* GetElementHostClass(int Index);
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();
	virtual void RegisterMembers();
	virtual int GetAmountOfElements();
	virtual const char* GetElementID(int Index);
	virtual const char* GetElementView(int Index,const char* LocalName);
	virtual const char* GetThisElementView(const char* LocalName){return NULL;};
	virtual BaseClass* GetElementClass(int Index);
	OneClassMemberStorage* GetElementCMS(int Index);
	OneClassMemberInfo GetElementInfo(int Index,void* base);
    OneClassMemberInfo GetElementInfo(const char* Name,void* base);
	virtual void* GetElementPtr(int Index,void* base);
	virtual void* GetElementExtraPtr(int Index);
	virtual bool DelElement(int Index){return false;}
	virtual void LoadPostProcess(void* DataPtr,void* ExtraData);
	bool	LoadMember(xmlQuote& xml,void* ClassPtr,int MemIndex,ErrorPager* Error,void* Extra);
	bool	LoadMemberDirect(void* ElementPtr,void* ClassPtr,int MemIndex);	
	virtual bool CheckDirectCasting(){
		return false;
	}
	virtual int ExpandWith(const char* ElmName,void* base){
		return -1;
	}
	virtual int GetExpansionRules(){
		return 0;//0-no expansion 1-expand with base type only 2-expand with child classes
	}	
	virtual const char* GetExpansionBaseClass(){
		return NULL;
	}
	virtual bool ForceProperty();//if returns true, the class will always be at bottom position in ComplexClassEditor 		
	virtual bool ForceSimplification(){return false;}//if returns true, this node will be hidden in class editor
	//optype:
	//-1 - up
	// 0 - check
	// 1 - down
	virtual bool MoveElement(int index,char opType){return false;}
	//this function is to limit expansion rules of some member of this class,
	//for example class A has members B and C, member B has expansion rules, that allows to 
	//expand class B with classes D and E. But we want that sometimes (in dependence on class C)
	//that class B should not be expanded with class E sometimes. Then 
	//A->AskParentForUsingExpansionClass("A","E") should return false
	virtual bool AskParentForUsingExpansionClass(char* MemberName,char* ClassName){return true;}
	//  added by Silver, 21.08.2003
	bool		HasParentClass(const char* ParentClassName);	
	virtual     bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DstDataExtra,CopyContex* C);
	virtual     void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual		void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	//-by Drew-
	//only for directly casting classes:
	virtual     bool ReadFromFile    (const char* Name);
	virtual     bool SafeReadFromFile(const char* Name);
	virtual     bool WriteToFile     (const char* Name);
	virtual     bool Copy            (BaseClass* Dest, bool Add=true);
	virtual		const char* GetSource(){return NULL;};
	//- special function for integrating in editor -
	virtual int        GetAmountOfEditableElements    (void* DataPtr,DWORD Options=7);//1-enum leafs 2-enum nodes 4-enum adds, so 7-enum everything
	virtual int        GetIndexOfEditableElement      (int Index,void* DataPtr,DWORD Option=7);//return index that is in range 0..GetAmountOfElements()-1
	virtual BaseClass* GetEditableElementClass        (int Index,void* DataPtr,DWORD Option=7);
	virtual void*      GetEditableElementData         (int Index,void* DataPtr,DWORD Option=7);
	virtual void*      GetEditableElementExtra        (int Index,void* DataPtr,DWORD Option=7);
	OneClassMemberInfo GetEditableElementInfo         (int Index);
	virtual bool       GetEditableElementExpansionList(ClassExpParams& EXP,int Index,void* DataPtr,DWORD Option=7,BaseClass* Parent=NULL,const char* ElmName=NULL);
	virtual bool       CheckIfElementIsNode           (int Index,void* DataPtr);//Index: 0..GetAmountOfElements()-1
    virtual bool       CheckIfEditableElementIsNode   (int Index,void* DataPtr,DWORD Option=7);
	virtual const char* GetEditableElementName        (int Index,void* DataPtr,DWORD Option=7);
	virtual const char* GetEditableElementView        (int Index,void* DataPtr,const char* LocalName,DWORD Option=7);
	virtual void       GetElementMessageAndHintInEditor(DString& Message,DString& Hint,BaseClass* Parent,char* ElmID);	
	virtual void	   EvaluateFunction()             {}
	virtual bool       CheckIfFunction()			  {return false;}
    virtual void       call(void* base,void* result)  {EvaluateFunction();};
	virtual int        GetSectionStatus()			  {return 0;}
	virtual void       SetSectionStatus				  (int s){}
	virtual bool       CheckIfElementReadOnly		  (int Index,void* DataPtr,DWORD Option=7);
	virtual bool       CheckIfElementInvisible		  (int Index,void* DataPtr,DWORD Option=7);
	//-----------masking elements of class---------------//
	virtual DWORD GetClassMask(){return 0xFFFFFFFF;}
	virtual int GetRealMaskedPosition(int Index);//in - masked position out - true position in list of all members
	//------Global Class Member Indentification------//
	virtual _str* GetObjectNamePointer(){return NULL;}	//returns pointer to the _str, that contains name 
														//of this class member,it is used for ClassRef	
	virtual bool CheckIfObjectIsGlobal(){return false;}
	virtual DWORD GetObjectGlobalID(){return 0xFFFFFFFF;}
	virtual void SetObjectGlobalID(DWORD ID){}
	virtual BaseClass* GetParent(){return ParentBC;}
	virtual void SetParent(BaseClass* Parent);
	//--------------checking for errors of registration-------------//
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){return true;}
	//-----accessing by field name------//
	const char* GetStringField(const char* FieldName);
	int   GetIntField  (const char* FieldName,int DefaultVal=0);
	float GetFloatField(const char* FieldName,float DefaultVal=0.0f);
	//-------------short form saving----------------//
	void DeleteDefaultSubFields(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool ShouldSaveInShortForm(){return true;}
	//------------- class presentation -------------//
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
	//------------- editor events ---------------//
	//editor will call next function always, when it is visible in tree
	virtual void ProcessInEditor(BaseClass* Parent){}
	//editor will call OnChangeMember every time, when member changes and editor is able to
	//recognise it. You should return true if editor must do refresh.
	virtual bool OnChangeMember(BaseClass* MembClass,void* MembPtr,void* MembExtra,const char* MembName){return false;}
	virtual	void SetEnumName(const char* enum_name=NULL){};
	AUTONEW(BaseClass);
};
//class BaseNode:public BaseClass{
//protected:
//	BaseClass* ParentBC;
//public:
//	BaseNode(){ParentBC=NULL;}
//	virtual BaseClass* GetParent(){return ParentBC;}
//	virtual void SetParent(BaseClass* Parent){ParentBC=Parent;}
//	AUTONEW(BaseNode);
//};
//_str
class DIALOGS_API _str:public DString,public BaseClass{
public:
	~_str();

	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();    	

	/* VITYA */
	_str() : DString() {};
	
	//_str& operator = (		char*		s  )	{ DString::operator = (s);			return *this;	};
	_str& operator = (const char*		s  )	{ DString::operator = (s);			return *this;	};
	_str& operator = (const int&		a  )	{ DString::operator = (a);			return *this;	};
	//_str& operator = (		DString&	ds1)	{ DString::operator = (ds1.cpchar());	return *this;	};
	_str& operator = (const DString&	ds1)	{ DString::operator = (ds1.cpchar());	return *this;	};
	//_str& operator = (		_str&		st1)	{ DString::operator = (st1.cpchar());	return *this;	};
	_str& operator = (const _str&		st1)	{ DString::operator = (st1.cpchar());	return *this;	};

	//_str(		char*		s  )	{ this->operator=(  s); };
	_str(const	char*		s  )	{ this->operator=(  s); };
	_str(const	int&		a  )	{ this->operator=(  a); };
	//_str(		DString&	ds1)	{ this->operator=(ds1); };
	_str(const	DString&	ds1)	{ this->operator=(ds1); };
	//_str(		_str&		st1)	{ this->operator=(st1); };
	_str(const	_str&		st1)	{ this->operator=(st1); };

	_str& operator + (		char*		s  )	{ DString::operator + (s);			return *this;	};
	_str& operator + (const char*		s  )	{ DString::operator + (s);			return *this;	};
	_str& operator + (const int&		a  )	{ DString::operator + (a);			return *this;	};
	_str& operator + (		DString&	ds1)	{ DString::operator + (ds1.cpchar());	return *this;	};
	_str& operator + (const DString&	ds1)	{ DString::operator + (ds1.cpchar());	return *this;	};
	_str& operator + (		_str&		st1)	{ DString::operator + (st1.cpchar());	return *this;	};
	_str& operator + (const _str&		st1)	{ DString::operator + (st1.cpchar());	return *this;	};

	//friend _str& operator + (		char*		  s,	_str& str );
	//friend _str& operator + ( const char*		  s,	_str& str );
	//friend _str& operator + ( const	int			  a,	_str& str );
	//friend _str& operator + (		DString&	ds1,	_str& str );
	//friend _str& operator + ( const DString		ds1,	_str& str );

	_str& operator += (		 char*		s  )	{ DString::operator +=(s);			return *this;	};
	_str& operator += (const char*		s  )	{ DString::operator +=(s);			return *this;	};
	_str& operator += (const int&		a  )	{ DString::operator +=(a);			return *this;	}
	_str& operator += (		 DString&	ds1)	{ DString::operator +=(ds1.cpchar());	return *this;	};
	_str& operator += (const DString&	ds1)	{ DString::operator +=(ds1.cpchar());	return *this;	};
	_str& operator += (		 _str&		st1)	{ DString::operator +=(st1.cpchar());	return *this;	};
	_str& operator += (const _str&		st1)	{ DString::operator +=(st1.cpchar());	return *this;	};

	bool operator == (		char*		s  )	{ return	DString::operator ==(s);		};
	bool operator == (const char*		s  )	{ return	DString::operator ==(s);		};
	bool operator == (		DString&	ds1)	{ return	DString::operator ==(ds1.cpchar());	};
	bool operator == (const DString&	ds1)	{ return	DString::operator ==(ds1.cpchar());	};
	bool operator == (		_str&		st1)	{ return	DString::operator ==(st1.cpchar());	};
	bool operator == (const _str&		st1)	{ return	DString::operator ==(st1.cpchar());	};

	bool operator != (		char*		s  )	{ return	DString::operator !=(s);		};
	bool operator != (const char*		s  )	{ return	DString::operator !=(s);		};
	bool operator != (		DString&	ds1)	{ return	DString::operator !=(ds1.cpchar());	};
	bool operator != (const DString&	ds1)	{ return	DString::operator !=(ds1.cpchar());		};
	bool operator != (		_str&		st1)	{ return	DString::operator !=(st1.cpchar());	};
	bool operator != (const _str&		st1)	{ return	DString::operator !=(st1.cpchar());	};
	virtual void reset_class(void* ptr){
		_str* s=dynamic_cast<_str*>((BaseClass*)ptr);
		if(s)s->Clear();
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((_str*)DestData)=((_str*)SrcData)->pchar();
		return true;
	}
	virtual     void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual		void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	//virtual bool CheckDirectCasting(){
	//	return true;
	//}
	AUTONEW(_str);    
};//_str
class ReferableBaseClass;
class _str_ref : public _str
{
public:
	const	char*				GetClassName();
public:
	_str_ref()	{ ID=0xFFFFFFFF; };
	
	virtual	void				SetEnumName(const char* enum_name=NULL);	// Set enumerator name for use in Get()
	virtual bool				ClearRef();
	virtual	_str_ref&			operator=(const _str_ref& sref);
	virtual	void				SetRef(char* _name,DWORD _id);

	virtual	void				Set(ReferableBaseClass* pOB);
	ReferableBaseClass*			Get();	// Get object selectted in editor

	virtual bool OnChangeMember(BaseClass* MembClass,void* MembPtr,void* MembExtra,const char* MembName);
	virtual bool				Load(xmlQuote& xml,void* ClassPtr,ErrorPager* errpr,void* extra);
	
public:
	_str	ENUM_NAME;		// Name of enumerator that useed current class
	DWORD	ID;				// global index of ReferableBaseClass seted in this
};
////////////////////////////
class DIALOGS_API ReferableBaseClass:public BaseClass{
	DWORD ID;
public:
	_str Name;
	ReferableBaseClass(){ID=0xFFFFFFFF;/*CPGARB.AddClass(this);*/}
	~ReferableBaseClass(){CPGARB.DeleteClass(this);}
	virtual DWORD GetObjectGlobalID(){
		if(ID==0xFFFFFFFF){			
			CPGARB.AddClass(this);
			//CPGARB.TryToLinkClass(this);
		}
		return ID;
	}
	virtual void SetObjectGlobalID(DWORD id){ID=id;}
	virtual const char* GetClassName(){return "ReferableBaseClass";}
	virtual BaseClass* GetParent(){return ParentBC;}
	virtual void SetParent(BaseClass* Parent){ParentBC=Parent;}
	virtual _str* GetObjectNamePointer(){return &Name;}	
	bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C);		
	AUTONEW(ReferableBaseClass);
};
class ReferableBaseClassWithSource:public ReferableBaseClass{
public:
	_str Source;
	bool AlwaysLoadFromSource;
	bool AlwaysSaveToSource;
	virtual const char* GetSource(){
		return Source.pchar();
	};
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL){		
		ReferableBaseClass::Save(xml,ClassPtr,Extra);
		if(GetCurrentSaveFile()){
			if(AlwaysSaveToSource&&Source.pchar()[0]){				
				xml.WriteToFile(Source.pchar());
			}
		}
	};
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		if(GetCurrentSaveFile()){
			if(AlwaysLoadFromSource&&Source.pchar()[0]){
				xml.ClearAll();
				xml.ReadFromFile(Source.pchar());
			}
		}
		return ReferableBaseClass::Load(xml,ClassPtr,Error,Extra);
	};
	virtual bool ReadFromFile(const char* Name){
		bool R=ReferableBaseClass::ReadFromFile(Name);
		if(Name[0]!='$'){
			Source=Name;
		}
		return R;
	};
	virtual bool SafeReadFromFile(const char* Name){
		bool R=ReferableBaseClass::SafeReadFromFile(Name);
		if(Name[0]!='$'){
			Source=Name;
		}
		return R;
	};
	virtual bool WriteToFile(const char* Name){
		if(Name[0]!='$'){
			Source=Name;
		}
		return ReferableBaseClass::WriteToFile(Name);
	};
};
//--implementation of some classes--//
class DIALOGS_API _int:public BaseClass{
public:
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();
	virtual void reset_class(void* ptr){
		*((int*)ptr)=0;
	}
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==4;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((int*)DestData)=*((int*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_int);
};

class DIALOGS_API _DWORD:public BaseClass{
public:
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();    	
	virtual void reset_class(void* ptr){
		*((DWORD*)ptr)=0;
	}
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==4;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((int*)DestData)=*((int*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_DWORD);
};

class DIALOGS_API _float:public BaseClass{
public:
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();  
	virtual void reset_class(void* ptr){
		*((float*)ptr)=0.0f;
	}
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return !strcmp(TypeName,"float");
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((int*)DestData)=*((int*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_float);
}; // class _float

//-----------------------------------------------------------------------------
// CtrlISlider
//-----------------------------------------------------------------------------
class CtrlISlider : public _int {
public:
	virtual const char * GetClassName() { return "CtrlISlider"; }
	AUTONEW(CtrlISlider);
};

struct CtrlISliderArgs {
	int MinValue;
	int MaxValue;
	int DefValue;
};

//-----------------------------------------------------------------------------
// CtrlFSlider
//-----------------------------------------------------------------------------
class CtrlFSlider : public _float {
public:
	virtual const char * GetClassName() { return "CtrlFSlider"; }
	AUTONEW(CtrlFSlider);
};

struct CtrlFSliderArgs {
	float MinValue;
	float MaxValue;
	float DefValue;
};

class _float01:public _float{
public:
	virtual const char* GetClassName(){
		return "_float01";
	}    	
	AUTONEW(_float01);
};
class _float0_10:public _float{
public:
	virtual const char* GetClassName(){
		return "_float0_10";
	}    	
	AUTONEW(_float0_10);
};
class DIALOGS_API _bool:public BaseClass{
public:
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName(); 
	virtual void reset_class(void* ptr){
		*((bool*)ptr)=false;
	}
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==1;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((byte*)DestData)=*((byte*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_bool);
}; // class _bool

class DIALOGS_API _index:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==4;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL){
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			xml.Assign_string(E->Get(*((int*)ClassPtr)));
		}
	}
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		void KeyTestMem();
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			const char* s=xml.Get_string();
			if(s){
				int p=E->Get((char*)xml.Get_string());
				if(p!=-1){
					*((int*)ClassPtr)=E->Get((char*)xml.Get_string());					
				}else{
					*((int*)ClassPtr)=E->GetVal(0);
					//Error->xml_print(xml,"identifier \"%s\" not found in list \"%s\" for variable \"%s\"\n",xml.Get_string(),E->EnumeratorID,xml.GetQuoteName());
				}			
			}else{
				*((int*)ClassPtr)=E->GetVal(0);
				//Error->xml_print(xml,"xml node is empty for variable %s\n",xml.GetQuoteName());
			}
			return true;
		}else Error->xml_print(xml,"Enumerator not assigned for type \"_index\" for variable %s\n",xml.GetQuoteName());
		return false;
	}
	virtual const char* GetClassName(){
		return "_index";
	}
	virtual void reset_class(void* ptr){
		*((int*)ptr)=0;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((int*)DestData)=*((int*)SrcData);
		DestExtraPtr=SrcDataExtra;
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_index);
};
class DIALOGS_API _byte_index:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==1;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL){
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			xml.Assign_string(E->Get(*((BYTE*)ClassPtr)));
		}
	}
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			const char* s=xml.Get_string();
			if(s){
				int p=E->Get((char*)xml.Get_string());
				if(p!=-1){
					*((BYTE*)ClassPtr)=E->Get((char*)xml.Get_string());
					return true;
				}else Error->xml_print(xml,"identifier \"%s\" not found in list \"%s\" for variable \"%s\"\n",xml.Get_string(),E->EnumeratorID,xml.GetQuoteName());
			}else Error->xml_print(xml,"xml node is empty for variable %s\n",xml.GetQuoteName());
		}else Error->xml_print(xml,"Enumerator not assigned for type \"_index\" for variable %s\n",xml.GetQuoteName());
		return false;
	}
	virtual const char* GetClassName(){
		return "_byte_index";
	}
	virtual void reset_class(void* ptr){
		*((byte*)ptr)=0;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((byte*)DestData)=*((byte*)SrcData);
		DestExtraPtr=SrcDataExtra;
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_byte_index);
};
class DIALOGS_API _word_index:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==2;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL){
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			xml.Assign_string(E->Get(*((WORD*)ClassPtr)));
		}
	}
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		if(Extra){
			Enumerator* E=(Enumerator*)Extra;
			const char* s=xml.Get_string();
			if(s){
				int p=E->Get((char*)xml.Get_string());
				if(p!=-1){
					*((WORD*)ClassPtr)=E->Get((char*)xml.Get_string());
					return true;
				}else Error->xml_print(xml,"identifier \"%s\" not found in list \"%s\" for variable \"%s\"\n",xml.Get_string(),E->EnumeratorID,xml.GetQuoteName());
			}else Error->xml_print(xml,"xml node is empty for variable %s\n",xml.GetQuoteName());
		}else Error->xml_print(xml,"Enumerator not assigned for type \"_index\" for variable %s\n",xml.GetQuoteName());
		return false;
	}
	virtual const char* GetClassName(){
		return "_word_index";
	}
	virtual void reset_class(void* ptr){
		*((word*)ptr)=0;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((word*)DestData)=*((word*)SrcData);
		DestExtraPtr=SrcDataExtra;
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_word_index);
};
class DIALOGS_API _accamulator:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==4;
	}
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
		}else Error->xml_print(xml,"Enumerator not assigned for type <_accamulator> for variable %s\n",xml.GetQuoteName());
		return false;
	}
	virtual const char* GetClassName(){
		return "_accamulator";
	}
	virtual void reset_class(void* ptr){
		*((int*)ptr)=0;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((int*)DestData)=*((int*)SrcData);
		DestExtraPtr=SrcDataExtra;
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_index);
};
#define TEMPL(c,elm) #c"."#elm
template<class X>class ClassArray:public BaseClass,public DynArray<X*>{
public:
	virtual ~ClassArray(){
		reset_class(this);
	};
	virtual void reset_class(void* ptr){
		ClassArray<X>* CA=(ClassArray<X>*)ptr;
		for(int i=0;i<CA->GetAmount();i++)if((*CA)[i]){
			try{
				delete(CA->GetValues()[i]);
			}catch(...){
			}
			CA->GetValues()[i]=NULL;
		}
		if(CA->Values)delete[](CA->Values);
		CA->Values=NULL;
		CA->NValues=0;
		CA->MaxValues=0;
	}
	void Clear(){
        reset_class(this);
	}
	const char* GetClassName(){
		static char N[64]="";
		if(N[0]!='C'){
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
		return ((*this)[Index])!=NULL?(*this)[Index]->GetClassName():"NULL";
	}
	BaseClass* GetElementClass(int Index){
		return (*this)[Index];
	}
	void* GetElementPtr(int Index,void* base){
		return (*((ClassArray<X>*)base))[Index];
	}
	__forceinline int Add(X* V){
		CheckDynPtr(V);
		if(V)V->SetParent(this);
		return DynArray<X*>::Add(V);
	}
	__forceinline int Add(X* V,int N){
		if(V)V->SetParent(this);
		return DynArray<X*>::Add(V,N);
	}
	__forceinline bool Insert(int pos, X* V){
		CheckDynPtr(V);
		if(V)V->SetParent(this);
		return DynArray<X*>::Insert(pos,V);
	}
	virtual int ExpandWith(const char* ElmName,void* base){
        ClassArray<X>* BC=(ClassArray<X>*)base;
		OneClassStorage* OCS=CGARB.GetClass(ElmName);
		if(OCS){
			BaseClass* NewBase=OCS->OneMemb;
			if(NewBase){
	            BaseClass* B1=NewBase->new_element();
				B1->SetParent(this);
				BC->Add((X*)B1);
				return BC->GetAmount()-1;
			}else return -1;
		}else return -1;
	}
	virtual int GetExpansionRules(){
		return 2;//0-no expansion 1-expand with base type only 2-expand with child classes
	}
	virtual bool MoveElement(int index,char opType){
		if(opType==0)return true;
		/*
		if(opType>0&&index<GetAmount()-1){			
			int dest=(GetKeyState(VK_CONTROL)&0x8000)?GetAmount()-1:index+1;
			std::swap((*this)[index],(*this)[dest]);
			return true;
		}
		if(opType<0&&index>0){
			int dest=(GetKeyState(VK_CONTROL)&0x8000)?0:index-1;
			std::swap((*this)[index],(*this)[dest]);
			return true;
		}
		*/
		return Move(index,index+opType);
	}
	virtual const char* GetExpansionBaseClass(){
		static X x;
        return x.GetClassName();
	}

	virtual bool DelElement(int index){		
		if(index>=0&&index<GetAmount()){
			if((*this)[index])delete((*this)[index]);
			Del(index,1);
			return true;
		}else return false;
	}
	virtual bool CheckDirectCasting(){
		return true;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		ClassArray<X>* SA=(ClassArray<X>*)SrcData;
		ClassArray<X>* DA=(ClassArray<X>*)DestData;
		for(int i=0;i<SA->GetAmount();i++)if((*SA)[i]){
            X* E=(X*)(*SA)[i]->new_element();
			DA->Add(E);
			(*SA)[i]->FullCopy((*SA)[i],SrcDataExtra,E,E,SrcDataExtra,C);            
		}
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
		Out->WriteDWORD(GetAmount());
		_str s;
		for(int i=0;i<GetAmount();i++){
			BaseClass* BC=(*this)[i];
            s=BC->GetClassName();
			s.SaveBin(&s,NULL,Out,ExDictionary,SkipList);
			BC->SaveBin(BC,Extra,Out,ExDictionary,SkipList);
		}
	}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
        int N=In->ReadDWORD();
		_str s;
		for(int i=0;i<N;i++){
            s.LoadBin(&s,NULL,In,ExDictionary);
			int id=ExpandWith(s.pchar(),Data);
			if(id!=-1){
				BaseClass* BC=(*this)[id];
				BC->LoadBin(BC,Extra,In,ExDictionary);
			}else{
				//Log.Warning("%s::LoadBin : unable to find class %s",GetClassName(),s.pchar());
			}
		}
	}
	AUTONEW(ClassArray);
};
//can be used only for very simple classes, that does not need to be destroyed
template<class X>class LinearClonesArray:public BaseClass,public DynArray<X>{
public:
	virtual ~LinearClonesArray(){
		reset_class(this);
	};	
	inline X* operator [](int pos) const {
#ifdef SAFE_ARRAY
		assert(pos>=0&&pos<NValues);
		if(pos<0||pos>=NValues)return &Values[pos];//DlaBitja;
#endif //SAFE_ARRAY
		return &Values[pos];
	}
	virtual void reset_class(void* ptr){
		ClassArray<X>* CA=(ClassArray<X>*)ptr;
		if(CA->Values)delete[](CA->Values);
		CA->Values=NULL;
		CA->NValues=0;
		CA->MaxValues=0;
	}
	void Clear(){
		reset_class(this);
	}
	const char* GetClassName(){
		static char N[64]="";
		if(N[0]!='C'){
			X* x=new X;
			sprintf(N,"LinearClonesArray.%s",x->GetClassName());
			delete(x);
		}
		return N;
	}
	int GetAmountOfElements(){
		return GetAmount();
	}
	const char* GetElementID(int Index){
		return ((*this)[Index])!=NULL?(*this)[Index]->GetClassName():"NULL";

	}
	BaseClass* GetElementClass(int Index){
		return (*this)[Index];
	}
	void* GetElementPtr(int Index,void* base){
		LinearClonesArray<X>* p=(LinearClonesArray<X>*)base;
		return (void*)p->GetElementClass(Index);
	}
	__forceinline int Add(X* V){
		if(V)V->SetParent(this);
		int n=DynArray<X>::Add(*V);
		free(V);
		return n;
	}
	__forceinline int Add(X* V,int N){
		if(V)V->SetParent(this);
		int n=DynArray<X>::Add(*V,N);
		free(V);
		return n;
	}
	__forceinline int Add(const X* V,int NTimes){
		Check(NValues+NTimes);
		for(int i=0;i<NTimes;i++){
			memcpy(&Values[NValues],V,sizeof X);
			NValues++;
		}
		free((void*)V);
		return NValues-1;
	}
	inline int Add(const X& V,int NTimes){
		Check(NValues+NTimes);
		for(int i=0;i<NTimes;i++){
			memcpy(&Values[NValues],&V,sizeof X);
			NValues++;
		}
		return NValues-1;
	}
	__forceinline bool Insert(int pos, X* V){
		if(V)V->SetParent(this);
		int n=DynArray<X>::Insert(pos,*V);
		free(V);
		return n;
	}
	inline int Add(const X* V){
		Check(NValues+1);
		memcpy(&Values[NValues],V,sizeof X);
		NValues++;
		free((void*)V);
		return NValues-1;
	}
	inline int AddPtr(const X* V){
		Check(NValues+1);
		memcpy(&Values[NValues],V,sizeof X);
		NValues++;
		free((void*)V);
		return NValues-1;
	}
	virtual int ExpandWith(const char* ElmName,void* base){
		LinearClonesArray<X>* BC=(LinearClonesArray<X>*)base;
		OneClassStorage* OCS=CGARB.GetClass(ElmName);
		if(OCS){
			BaseClass* NewBase=OCS->OneMemb;
			if(NewBase){
				BaseClass* B1=NewBase->new_element();
				B1->SetParent(this);
				BC->AddPtr((X*)B1);
				return BC->GetAmount()-1;
			}else return -1;
		}else return -1;
	}
	virtual int GetExpansionRules(){
		return 1;//0-no expansion 1-expand with base type only 2-expand with child classes
	}
	virtual bool MoveElement(int index,char opType){
		if(opType==0)return true;		
		return Move(index,index+opType);
	}
	virtual const char* GetExpansionBaseClass(){
		static X x;
		return x.GetClassName();
	}

	virtual bool DelElement(int index){		
		if(index>=0&&index<GetAmount()){
			if((*this)[index])delete((*this)[index]);
			Del(index,1);
			return true;
		}else return false;
	}
	virtual bool CheckDirectCasting(){
		return true;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		LinearClonesArray* SA=(LinearClonesArray*)SrcData;
		LinearClonesArray* DA=(LinearClonesArray*)DestData;
		X* E=NULL;
		for(int i=0;i<SA->GetAmount();i++){
			if(!E)E=(X*)(*SA)[i]->new_element();
            (*SA)[i]->FullCopy((*SA)[i],SrcDataExtra,E,E,SrcDataExtra,C);
			DA->Add(*E,1);			
		}
		free(E);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
		Out->WriteDWORD(GetAmount());		
		for(int i=0;i<GetAmount();i++){
			BaseClass* BC=(*this)[i];			
			BC->SaveBin(BC,Extra,Out,ExDictionary,SkipList);
		}
	}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
		int N=In->ReadDWORD();
		_str s;
		X x;
		x.reset_class(&x);
		for(int i=0;i<N;i++){
			int p=Add(&x);
			BaseClass* BC=(*this)[i];						
			BC->LoadBin(BC,Extra,In,ExDictionary);
		}
	}
	AUTONEW(LinearClonesArray);
};
template<class X>class ClonesArray:public ClassArray<X>{
public:
	virtual int GetExpansionRules(){
		return 1;//0-no expansion 1-expand with base type only 2-expand with child classes
	}
};
template <class X> class ClassPtr:public BaseClass{
	X* ptr;
public:
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		if(ptr){
			X* x=(X*)ptr->new_element();
			x->SetParent(Dest);
			((ClassPtr<X>*)Dest)->ptr=x;
			ptr->FullCopy(ptr,SrcDataExtra,x,x,SrcDataExtra,C);			
		}
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
		BaseClass* BC=Get();
		Out->WriteBYTE(BC?1:0);
		if(BC){
			_str s=BC->GetClassName();
			s.SaveBin(&s,Extra,Out,ExDictionary,SkipList);
			BC->SaveBin(BC,Extra,Out,ExDictionary,SkipList);
		}		
	}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
		int N=In->ReadBYTE();		
		if(N){
            _str s;
			s.LoadBin(&s,Extra,In,ExDictionary);
			ExpandWith(s.pchar(),Data);
            BaseClass* BC=Get();
			if(BC){
				BC->LoadBin(BC,Extra,In,ExDictionary);
			}
		}else Set(NULL);		
	}
	//ClassPtr& operator = (X* x){ptr=x;}
	X* Get(){
		return ptr;
	}
	void Set(X* x){
		ptr=x;
		if(ptr)ptr->SetParent(this);
	}


	ClassPtr(){
		ptr=NULL;
	}
	virtual ~ClassPtr(){
		reset_class(this);
	};
	virtual void reset_class(void* p){
		ClassPtr<X>* cp=(ClassPtr<X>*)p;
		if(cp->ptr)delete(cp->ptr);
		cp->ptr=NULL;		
	}
	void Clear(){
		reset_class(this);
	}
	const char* GetClassName(){
		static char N[64]="";
		if(!N[0]){
			X* x=new X;
			sprintf(N,"ClassPtr.%s",x->GetClassName());
			delete(x);
		}
		return N;
	}
	int GetAmountOfElements(){
		return ptr?1:0;
	}
	const char* GetElementID(int Index){		
		if(ptr)return ptr->GetClassName();
		else return "NULL";
	}
	BaseClass* GetElementClass(int Index){
		return ptr;
	}
	void* GetElementPtr(int Index,void* base){
		return ptr;
	}
	virtual int ExpandWith(const char* ElmName,void* base){
		reset_class(base);
		ClassPtr<X>* BC=(ClassPtr<X>*)base;
		OneClassStorage* OCS=CGARB.GetClass(ElmName);
		if(OCS){
			BaseClass* NewBase=OCS->OneMemb;
			if(NewBase){
				BC->ptr=(X*)NewBase->new_element();
				BC->ptr->SetParent(this);
				return 0;
			}else return -1;
		}else return -1;
	}
	virtual int GetExpansionRules(){
		return 2+4+8;//0-no expansion 1-expand with base type only 2-expand with child classes
		//4-allow manual type change
		//8-allow short form of presentation
	}
	virtual bool MoveElement(int index,char opType){
		return false;
	}
	virtual const char* GetExpansionBaseClass(){
		if(ptr)return NULL;
		static X x;
		return x.GetClassName();
	}

	virtual bool DelElement(int index){		
		reset_class(this);		
		return true;		
	}
	virtual bool CheckDirectCasting(){
		return true;
	}
	virtual bool ForceProperty(){return true;}
	AUTONEW(ClassPtr);
};
template <class X> class ClonePtr:public ClassPtr<X>{
	virtual int GetExpansionRules(){
		return 1+4+8;//0-no expansion 1-expand with base type only 2-expand with child classes
		//4-allow manual type change
		//8-allow short form of presentation
	}
};
template <class X> class AllPtr:public ClassPtr<X>{
	virtual int GetExpansionRules(){
		return 1+2+4+8;//0-no expansion 1-expand with base type only 2-expand with child classes
		//4-allow manual type change
		//8-allow short form of presentation
	}
};
template <class X> class ClassRef:public BaseClass{
public:
	DWORD CPG_Index;	
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		ClassRef<X>* D=((ClassRef<X>*)Dest);
		CPGARB.DelRef(D->CPG_Index);
		if(CPG_Index!=0xFFFFFFFF){
			
			int N=C->LocalClassIDS.GetAmount();
			for(int i=0;i<N;i++){
				if(CPG_Index==C->LocalClassIDS[i]){										
					OneClassPointer* CP=CPGARB.CPointer[CPG_Index>>24][CPG_Index&0xFFFFFF];
					BaseClass* BC=CP->ClassPtr;
					if(BC){
						_str* name0=BC->GetObjectNamePointer();
						if(name0){
							_str s;
						//	s=name0->pchar();
							// not repeat "_copy#"
							char* pCOPY = strstr(name0->pchar(),"_copy#");
							if ( pCOPY!=NULL ){
								int len = (int)( pCOPY - name0->pchar() )+1;
								char* pS = new char[len];
								strncpy( pS, name0->pchar(), len-1 );
								pS[len-1]=0;
								s=pS;
								delete pS;
							}else{
								s=name0->pchar();
							};
							s+="_copy#";
                            s+=abs(int(C->CopyGlobalID));
							D->CPG_Index=CPGARB.AddRef((char*)BC->GetClassName(),s.pchar(),this);
							return true;
						}
					}					
				}
			}
			
			D->CPG_Index=CPG_Index;
			CPGARB.AddRef(CPG_Index);
		}
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
		//not implemented
	}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
		//not implemented
	}
	X* Get(){
		if(CPG_Index==0xFFFFFFFF)return NULL;
        return (X*)CPGARB.CPointer[CPG_Index>>24][CPG_Index&0xFFFFFF]->ClassPtr;
	}
	void Set(X* x){		
		CPGARB.DelRef(CPG_Index);
		if(x&&x->GetObjectNamePointer()){
			CPG_Index=x->GetObjectGlobalID();
			CPGARB.AddRef(CPG_Index);
		}else CPG_Index=0xFFFFFFFF;
	}
	void SetObjectName(char* ObjName){
		CPGARB.DelRef(CPG_Index);
		static char* cname=NULL;
		if(!cname){
			X* x=new X;
			cname=(char*)x->GetClassName();
			delete(x);
		}
		CPG_Index=CPGARB.AddRef(cname,ObjName,this);
	}
	const char* GetObjectName(){
		if(CPG_Index!=0xFFFFFFFF){
            OneClassPointer* CP=CPGARB.CPointer[CPG_Index>>24][CPG_Index&0xFFFFFF];
			if(CP){
				if(CP->NamePtr)return CP->NamePtr->pchar();
				else if(CP->RefName.pchar())return CP->RefName.pchar();
			}
		}
		return NULL;
	}
	ClassRef(){
		CPG_Index=0xFFFFFFFF;
	}
	virtual ~ClassRef(){
		CPGARB.DelRef(CPG_Index);
		reset_class(this);
	};
	virtual void reset_class(void* p){
        CPG_Index=0xFFFFFFFF;		
	}
	void Clear(){
		CPGARB.DelRef(CPG_Index);
		CPG_Index=0xFFFFFFFF;
		reset_class(this);
	}
	virtual	const char* GetClassName(){
		static char N[64]="";
		if(!N[0]){
			DString D;
			D.Assign((char*)typeid(X).name());
			ConvTypeName(D);			
			sprintf(N,"ClassRef.%s",D.pchar());//???
			D.Free();
		}
		return N;
	}
	const char* GetElementID(int Index){		
		X* ptr=Get();
		if(ptr)return ptr->GetClassName();
		else return BaseClass::GetElementID(Index);
	}
	BaseClass* GetElementClass(int Index){
		return Get();
	}
	void* GetElementPtr(int Index,void* base){
		return Get();
	}
	int GetAmountOfElements(){
        X* x=Get();
		return x?1:0;
	}
	virtual bool CheckDirectCasting(){
		return true;
	}
	bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
		if(xml.GetNSubQuotes()==2){
			xmlQuote* xml1=xml.GetSubQuote("ClassName");
			xmlQuote* xml2=xml.GetSubQuote("RefName");
			if(xml1&&xml2&&xml1->Get_string()&&xml2->Get_string()){
				ClassRef* CR=((ClassRef*)ClassPtr);
				CPGARB.DelRef(CR->CPG_Index);
                CR->CPG_Index=CPGARB.AddRef(
					(char*)xml1->Get_string(),
					(char*)xml2->Get_string(),
					CR);
			}
		}else if(xml.GetNSubQuotes()==0){
            const char* name=xml.Get_string();
			if(name && name[0]){
				ClassRef* CR=((ClassRef*)ClassPtr);
				CPGARB.DelRef(CR->CPG_Index);                
				X* p=new X;
				CR->CPG_Index=CPGARB.AddRef((char*)p->GetClassName(),(char*)name,CR);
				delete(p);
			}
		}
		return true;
	}
	void Save(xmlQuote& xml,void* ClassPtr,void* Extra){
		X* ptr=((ClassRef*)ClassPtr)->Get();
		if(ptr){
			xmlQuote* xml1=new_xmlQuote("ClassName");
			xml1->Assign_string((char*)ptr->GetClassName());
			xmlQuote* xml2=new_xmlQuote("RefName");
			if(ptr->GetObjectNamePointer()&&ptr->GetObjectNamePointer()->pchar()){
				xml2->Assign_string(ptr->GetObjectNamePointer()->pchar());
			}
			xml.AddSubQuote(xml1);
			xml.AddSubQuote(xml2);
		}
	}
	virtual bool ForceProperty(){return true;}
	AUTONEW(ClassRef);
};
template<class X,class BaseForX> class LinearArray:public BaseClass,public DynArray<X>{
public:
	BaseForX B;
	X B1;
	virtual ~LinearArray(){
		reset_class(this);
	};
	virtual void reset_class(void* DataPtr){	
		LinearArray* LA=(LinearArray*)DataPtr;
		LA->DynArray<X>::~DynArray();
	}
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
	virtual bool DelElement(int index){
		if(index>=0&&index<GetAmount()){
			Del(index,1);
			return true;
		}else return false;
	}
	virtual int GetExpansionRules(){
		return 1;//0-no expansion 1-expand with base type only 2-expand with child classes
	}
	virtual bool MoveElement(int index,char opType){
		if(opType==0)return true;
		if(opType>0&&index<GetAmount()-1){
			std::swap((*this)[index],(*this)[index+1]);
			return true;
		}
		if(opType<0&&index>0){
			std::swap((*this)[index],(*this)[index-1]);
			return true;
		}
		return false;
	}
	virtual const char* GetExpansionBaseClass(){
        return "";
	}
	typedef LinearArray<X,BaseForX> LA;
	virtual bool CheckDirectCasting(){
		return true;
	}
	AUTONEW(LA);
};
#define INVISIBLE SetInvisibleMode();
#define READONLY SetReadOnlyMode();
#define STATIC SetStaticMode();
#define NOSAVE SetNoSaveMode();


#define INT_PROPERTY(classname,rf,wf)\
	static int __##rf(void* c){return ((classname*)c)->rf();}\
	static void __##wf(void* c,int v){((classname*)c)->wf(v);}

#define BOOL_PROPERTY(classname,rf,wf)\
	static void* __##rf(void* c){static bool temp;temp=((classname*)c)->rf();return &temp;}\
	static void __##wf(void* c,void* v){bool temp=*((bool*)v);((classname*)c)->wf(temp);}

#define FLOAT_PROPERTY(classname,rf,wf)\
	static float __##rf(void* c){return ((classname*)c)->rf();}\
	static void __##wf(void* c,float v){((classname*)c)->wf(v);}

#define PROPERTY(classname,rf,wf)\
	static void* __##rf(void* c){return ((classname*)c)->rf();}\
	static void __##wf(void* c,void* v){((classname*)c)->wf(v);}

#define REG_INTPROP(typ,fieldID,rf,wf)\
	{typ* m=new typ;\
	RegisterOneMemberIntProperty(m,&__##rf,&__##wf,#fieldID,NULL,this->GetClassName(),CurrMask);}

#define REG_INDEXPROP(type,fieldID,rf,wf,EnumID){type* m=new type;Enumerator* E=ENUM.Get(#EnumID);if(!E)E=ENUM.Create(#EnumID);RegisterOneMemberIntProperty(m,&__##rf,&__##wf,#fieldID,E,this->GetClassName(),CurrMask);}

#define REG_PROP(typ,fieldID,rf,wf)\
{typ* m=new typ;\
	RegisterOneMemberProperty(m,&__##rf,&__##wf,#fieldID,NULL,this->GetClassName(),CurrMask);}

#define REG_LOADSAVE(StartDir,mask){char cc[256];sprintf(cc,"LSENUM_%s",this->GetClassName());Enumerator* E=ENUM.Get(cc);if(!E->GetAmount()){E->Add(StartDir);E->Add(mask);}}

#define REG_LOADSAVE_FOR_CLASS(Class,StartDir,mask){Class* c=new Class;char cc[256];sprintf(cc,"LSENUM_%s",c->GetClassName());Enumerator* E=ENUM.Get(cc);if(!E->GetAmount()){E->Add(StartDir);E->Add(mask);delete(c);}}
#ifdef SAFETYPE
#define typerr(field)\
	if(!m->CheckCompartabilityWith(typeid(field).name(),sizeof field)){\
	PrintError("Incompartible type used for %s %s::%s : %s",typeid(field).name(),GetClassName(),#field,m->GetClassName());\
	};
#else
#define typerr(field)
#endif 

#define __constructor(x)void reset_class(void* p){((x*)p)->Init();};x(){Init();};void Init()

#define REG_MEMBER(type,fieldID)\
	{\
	type* m=_get_cmemb((type*)NULL,#type);/*new type;*/\
		BaseClass::RegisterOneMember((BaseClass*)(void*)m,&fieldID,#fieldID,NULL,this->GetClassName(),0,CurrMask);\
		typerr(fieldID);\
	}
#define REG_MEMBER_EX(type,fieldID,Name)\
{\
	type* m=_get_cmemb((type*)NULL,#type);/*new type;*/\
	BaseClass::RegisterOneMember((BaseClass*)(void*)m,&fieldID,#Name,NULL,this->GetClassName(),0,CurrMask);\
	typerr(fieldID);\
}
#define REG_MEMBER_EX2(type,fieldID,Name)\
{\
	type* m=_get_cmemb((type*)NULL,#type);/*new type;*/\
	BaseClass::RegisterOneMember((BaseClass*)(void*)m,&fieldID,Name,NULL,this->GetClassName(),0,CurrMask);\
	typerr(fieldID);\
}
#define REG_ENUM(type,fieldID,EnumID)\
{\
	type* m=_get_cmemb((type*)NULL,#type);/*new type;*/\
	Enumerator* E=ENUM.Get(#EnumID);\
	if(!E)E=ENUM.Create(#EnumID);\
	BaseClass::RegisterOneMember((BaseClass*)(void*)m,&fieldID,#fieldID,E,this->GetClassName(),0,CurrMask);\
	if (m->CheckDirectCasting())	((BaseClass*)(&fieldID))->SetEnumName(#EnumID);\
	typerr(fieldID);\
}
#define REG_ENUM_EX(type,fieldID,EnumID,Name)\
{\
	type* m=_get_cmemb((type*)NULL,#type);/*new type;*/\
	Enumerator* E=ENUM.Get(#EnumID);\
	if(!E)E=ENUM.Create(#EnumID);\
	BaseClass::RegisterOneMember((BaseClass*)(void*)m,&fieldID,#Name,E,this->GetClassName(),0,CurrMask);\
	typerr(fieldID);\
}

//-----------------------------------------------------------------------------
// REG_ISLIDER
//-----------------------------------------------------------------------------
#define REG_ISLIDER(nField, Min, Max, Def)\
	{\
		OneClassStorage *pClass = CGARB.GetClass("CtrlISlider");\
		if(pClass) {\
			BaseClass *m = pClass->OneMemb->new_element();\
			CtrlISliderArgs *pArgs = new CtrlISliderArgs;\
			pArgs->MinValue = Min, pArgs->MaxValue = Max, pArgs->DefValue = Def;\
			BaseClass::RegisterOneMember((BaseClass *)(void *)m, &nField, #nField, (void *)pArgs, this->GetClassName(), 0, CurrMask);\
			typerr(nField);\
		}\
	}

//-----------------------------------------------------------------------------
// REG_FSLIDER
//-----------------------------------------------------------------------------
#define REG_FSLIDER(fField, Min, Max, Def)\
	{\
		OneClassStorage *pClass = CGARB.GetClass("CtrlFSlider");\
		if(pClass) {\
			BaseClass *m = pClass->OneMemb->new_element();\
			CtrlFSliderArgs *pArgs = new CtrlFSliderArgs;\
			pArgs->MinValue = Min, pArgs->MaxValue = Max, pArgs->DefValue = Def;\
			BaseClass::RegisterOneMember((BaseClass *)(void *)m, &fField, #fField, (void *)pArgs, this->GetClassName(), 0, CurrMask);\
			typerr(fField);\
		}\
	}

#define REG_SPRITE(spr,fid)\
	{\
		OneClassStorage* sp=CGARB.GetClass("_sprite");\
		if(sp){\
			BaseClass* m=sp->OneMemb->new_element();\
			BaseClass::RegisterOneMember((BaseClass*)(void*)m,&spr,#spr,(void*)(int(&fid)-int(&spr)),this->GetClassName(),0,CurrMask);\
			typerr(spr);\
		}\
	}
#define REG_FILEPATH(str,mask)\
	{\
	OneClassStorage* sp=CGARB.GetClass("_str");\
	if(sp){\
	BaseClass* m=sp->OneMemb->new_element();\
	BaseClass::RegisterOneMember((BaseClass*)(void*)m,&str,#str,mask,this->GetClassName(),0,CurrMask);\
	typerr(str);\
	}\
	}
#define REG_SPRITE_EX(spr,sprName,fid)\
	{\
		OneClassStorage* sp=CGARB.GetClass("_sprite");\
		if(sp){\
			BaseClass* m=sp->OneMemb->new_element();\
			BaseClass::RegisterOneMember((BaseClass*)(void*)m,&spr,#sprName,(void*)(int(&fid)-int(&spr)),this->GetClassName(),0,CurrMask);\
			typerr(spr);\
		}\
	}
#define REG_BITFILE(file,ext){OneClassStorage* sp=CGARB.GetClass("_picfile"); if(sp){\
			BaseClass* m=sp->OneMemb->new_element();\
			BaseClass::RegisterOneMember((BaseClass*)(void*)m,&file,#file,(void*)(int(&ext)-int(&file)),this->GetClassName(),0,CurrMask);\
			typerr(file);}}

#define REG_EXTRA_MEMBER(type,fieldID,ExtraString) {type* m=new type;char* ex=(char*)ExtraString;BaseClass::RegisterOneMember((BaseClass*)(void*)m,&fieldID,#fieldID,ex,this->GetClassName(),0,CurrMask);typerr(fieldID);}
#define REG_ANM(anim,model) \
	{\
	OneClassStorage* sp=CGARB.GetClass("_ModelID");\
		if(sp){\
			BaseClass* m=sp->OneMemb->new_element();\
			BaseClass::RegisterOneMember((BaseClass*)(void*)m,&anim,#anim,(void*)(int(&anim)-int(&model)),this->GetClassName(),0,CurrMask);\
			typerr(anim);\
		}\
	}

//simplified bool bit field definition
#define BOOL_PROP(classname,Field)\
	static int __Get##Field(void* ptr){return ((classname*)ptr)->Field;}\
	static void __Set##Field(void* ptr,int v){((classname*)ptr)->Field=(v&255)!=0;}
#define REG_BOOL_PROP(Field) REG_INTPROP(_bool,Field,Get##Field,Set##Field)

#define SAVE(x) \
	typedef x BaseConvertor;\
	virtual const char* GetClassName(){return #x;};\
	virtual bool CheckDirectCasting(){\
		return true;\
	}\
	virtual BaseClass* new_element(){x* X=new x;X->GetObjectGlobalID();if(X->CheckDirectCasting()){OneClassStorage* OCS=GetClassStorage();/*if(!OCS->OneMemb)return NULL*/;for(int i=0;i<OCS->Members.GetAmount();i++){OneClassMemberStorage* OCMS=OCS->Members[i];if(OCMS&&OCMS->Member&&OCMS->Member->CheckDirectCasting()){((BaseClass*)(OCMS->GetElmPtr(X)))->SetParent(X);}}}return X;}\
	virtual OneClassStorage* GetClassStorage(){\
	static OneClassStorage* OCS=NULL;\
	TestClassStorage(&OCS);\
	if(!OCS)OCS=CGARB.GetClass(GetClassName());\
	return OCS;\
	}\
	virtual void RegisterMembers(){SAFENEW;OneClassStorage* OCS=GetClassStorage();if(CheckDirectCasting()){ for(int i=0;i<OCS->Members.GetAmount();i++){OneClassMemberStorage* OCMS=OCS->Members[i];if(OCMS&&OCMS->Member&&OCMS->Member->CheckDirectCasting()){BaseClass* B=((BaseClass*)(OCMS->GetElmPtr(this)));/*if(B->GetParent())break;*/B->SetParent(this);}}}\
	if(!(OCS&&OCS->Members.GetAmount())){DWORD CurrMask=0xFFFFFFFF;

//const char* name=typeid(*this).name()+6;char* s2=strstr(name,"::");if(s2)name=s2;if(name[0] && strcmp(name,#x)){PrintError("Incorrect class registration detected for class: %s - legistered like "#x,typeid(*this).name()+6);}

#define SAVE_EX(x,y) \
	typedef y BaseConvertor;\
	virtual const char* GetClassName(){return #x;};\
	virtual bool CheckDirectCasting(){\
	return true;\
	}\
	virtual BaseClass* new_element(){BaseClass* Y=(y*)new x;Y->GetObjectGlobalID();return Y;};\
	virtual OneClassStorage* GetClassStorage(){\
	static OneClassStorage* OCS=NULL;\
	if(!OCS)OCS=CGARB.GetClass(GetClassName());\
	return OCS;\
	}\
	virtual void RegisterMembers(){SAFENEW;OneClassStorage* OCS=GetClassStorage();\
	if(!(OCS&&OCS->Members.GetAmount())){DWORD CurrMask=0xFFFFFFFF;


#define ENDSAVE }\
	ENDNEW;\
	}
#define SAVE_SECTION(x) CurrMask=x;

#define declare_class(x)virtual const char* GetClassName(){return #x;};virtual bool CheckDirectCasting(){return true;}\
	typedef x BaseConvertor;\
	virtual BaseClass* new_element(){x* X=new x;X->GetObjectGlobalID();if(X->CheckDirectCasting()){OneClassStorage* OCS=GetClassStorage();for(int i=0;i<OCS->Members.GetAmount();i++){OneClassMemberStorage* OCMS=OCS->Members[i];if(OCMS&&OCMS->Member&&OCMS->Member->CheckDirectCasting()){((BaseClass*)(OCMS->GetElmPtr(X)))->SetParent(X);}}}return X;}\
	virtual OneClassStorage* GetClassStorage(){static OneClassStorage* OCS=NULL;if(!OCS)OCS=CGARB.GetClass(GetClassName());return OCS;}\
	virtual void RegisterMembers();
#define implement_class(x)\
	void x::RegisterMembers(){SAFENEW;OneClassStorage* OCS=GetClassStorage();if(CheckDirectCasting()){ for(int i=0;i<OCS->Members.GetAmount();i++){OneClassMemberStorage* OCMS=OCS->Members[i];if(OCMS&&OCMS->Member&&OCMS->Member->CheckDirectCasting()){BaseClass* B=((BaseClass*)(OCMS->GetElmPtr(this)));B->SetParent(this);}}}\
	if(!(OCS&&OCS->Members.GetAmount())){DWORD CurrMask=0xFFFFFFFF;
#define implement_end ENDNEW;}}
#define pure_declaration(x) SAVE(x);ENDSAVE;
DIALOGS_API BaseClass* _getClassFromPalette(const char* classname);
template <class X>
X* _get_cmemb(X* S,const char* name){
    X* bc=(X*)_getClassFromPalette(name);
	if(!bc){
		X* x=new X;
        reg_class(x);
		delete(x);
		bc=(X*)_getClassFromPalette(name);
	}
	if(!bc)bc=new X;
	return bc;
}
_inline char* ConvTypeName(DString& D){
	D.Replace("class ","");
	D.Replace("<",".");
	D.Replace(">","");
	D.Replace(",",".");
	return D.pchar();
}
template <class X>
void reg_auto(BaseClass* S,X& x,const char* name,void* EnumID=NULL,DWORD Mask=0xFFFFFFFF){
	DString D;
	D.Assign((char*)typeid(X).name());
	ConvTypeName(D);
	//X* m=new X;
	X* m=_get_cmemb((X*)NULL,D.pchar());
	S->RegisterOneMember(m,&x,name,EnumID,S->GetClassName(),1,Mask);
	D.Free();
}
template <class X,class Y>
void reg_auto_a(BaseClass* S,X& x,Y* y,const char* name,void* EnumID=NULL,DWORD Mask=0xFFFFFFFF){
	DString D;
	D.Assign((char*)typeid(X).name());
	ConvTypeName(D);
	//X* m=_get_cmemb((X*)NULL,D.pchar());
	X* m=new X;
	S->RegisterOneMember((Y*)m,&x,name,EnumID,S->GetClassName(),1,Mask);
	D.Free();
}
DIALOGS_API void reg_class_bc(BaseClass* BC,char* Topic,char* Typeid,bool AutoMode);
template <class X>
void reg_class(X* x,char* Topic=NULL){
	reg_class_bc(x,Topic,(char*)typeid(X).name(),false);	
}
template <class X>
void reg_class_auto(X* x,char* Topic=NULL){
	reg_class_bc(x,Topic,(char*)typeid(X).name(),true);		
}
_inline void UNREG_CLASS(char* Name){
	OneClassStorage*OCS=CGARB.GetClass(Name);	
	if(OCS&&OCS->OneMemb){
		OCS->Members.Clear();
		delete(OCS->OneMemb);
		OCS->OneMemb=NULL;
	}	
}
template <class X,class Y>
void reg_class_ex(X* x,Y* y,char* Topic=NULL){
	X* elm=dynamic_cast<X*>(x->new_element());
	if(!elm)return;
	DString D;
	D.Assign((char*)typeid(X).name());
	ConvTypeName(D);
	OneClassStorage*OCS=CGARB.GetClass(D.pchar());
	//if(OCS){
	//	D.Free();
	//	delete(elm);
	//	return;
	//}
	if(!OCS)OCS=CGARB.CreateClassStorage(D.pchar());
	if(OCS->OneMemb){
		delete(elm);
	}else OCS->OneMemb=(Y*)elm;
	if(!OCS->Members.GetAmount()){
		elm->RegisterMembers();
	}
	if(Topic&&!OCS->ClassTopic){
		OCS->ClassTopic=new char[strlen(Topic)+1];
		strcpy(OCS->ClassTopic,Topic);
	}
	D.Free();
}
//registering member that is class, derived from BaseClass
#define REG_AUTO(x) reg_auto(this,x,#x,NULL,CurrMask);reg_class_auto(&x);x.SetParent(this);
#define REG_AUTO2(x,y) reg_auto(this,x,#x,y,CurrMask);reg_class_auto(&x);x.SetParent(this);
#define REG_AUTO_A(x,y) reg_auto_a((y*)this,x,(y*)NULL,#x,NULL,CurrMask);
#define REG_AUTO_EX(x,Name) reg_auto(this,x,#Name,NULL,CurrMask);reg_class_auto(&x);x.SetParent(this);
#define REG_AUTO_EX_A(x,y,Name) reg_auto((y*)this,x,#Name,NULL,CurrMask);reg_class_auto(&x);
//registering member-class with enumerator
#define REG_AUTO_ENUM(x,y) {Enumerator* E=ENUM.Get(#y);reg_auto(this,x,#x,E,CurrMask);reg_class_auto(&x);x.SetParent(this);}
//registering class type that can be created by new
//for example, if you are using ClassArray<SomeClass>, then you
//must register classes, that can be stored in this array
#define REG_CLASS(x) {x*m=new(x);reg_class(m);delete(m);}
#define REG_CLASS_EX(x,Topic) {x*m=new(x);reg_class(m,Topic);delete(m);}
#define REG_CLASS_AMBIGUOUS(x,y) {x*m=new(x);reg_class_ex(m,(y*)NULL);delete(m);}
#define REG_CLASS_AMBIGUOUS_EX(x,y,Topic) {x*m=new(x);reg_class_ex(m,(y*)NULL,Topic);delete(m);}
//registering parent class. Deriving from several classses is
//not supported, but you can derive class from class, that is already derived from
//BaseClass
//template <class x>

DIALOGS_API void reg_parent(BaseClass* bas,BaseClass*m);

#define REG_PARENT(x) {x* m=_get_cmemb((x*)NULL,#x);reg_parent((BaseConvertor*)this,m);}
//template <class x>
DIALOGS_API void reg_base(BaseClass* bas,BaseClass* m);

#define REG_BASE(x) {x* m=_get_cmemb((x*)NULL,#x);reg_base((BaseConvertor*)this,m);}
class BaseFunction: public BaseClass{
public:
	const char* GetClassName(){
		return "BaseFunction";
	}
	virtual bool CheckIfFunction(){
		return true;
	}
	template< class Fn > Fn* get_parent(){
		BaseClass* B=GetParent();
		if(B){
			return dynamic_cast<Fn*>(B);
		}
		return NULL;
	}
	virtual bool CheckDirectCasting(){
		return true;
	}
    AUTONEW(BaseFunction);
};
class SubSection: public BaseClass{
	byte State;
public:
	SubSection(){
		State=1;
	}
	virtual int GetSectionStatus(){
		return State;
	}
	virtual void SetSectionStatus(int s){
		State=s;
	}
	const char* GetClassName(){
		return "SubSection";
	}
	virtual bool CheckDirectCasting(){
		return true;
	}
	virtual bool ForceProperty(){
		return true;
	}
	AUTONEW(SubSection);
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){}
};
class Delimiter: public BaseClass{	
public:
	virtual bool ForceProperty(){
		return true;
	}
	const char* GetClassName(){
		return "Delimiter";
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){}
	AUTONEW(Delimiter);
};
template <class BaseT>
class _method: public BaseClass{
    typedef void (BaseT::*FnProcess)();
    FnProcess m_fnProcess;
public:
    _method(){
        m_fnProcess=NULL;
    }
    _method(FnProcess call){
        m_fnProcess = call;
    }
    void _call(){
        if(m_fnProcess)m_fnProcess();
    }
    const char* GetClassName(){
		return "_method";
	}
	virtual bool CheckIfFunction(){
		return true;
	}	
	virtual bool CheckDirectCasting(){
		return false;
	}
    virtual void call(void* base,void* result){
        if(base){
            (((BaseT*)base)->*m_fnProcess)();
        }        
    }
    virtual void Save(xmlQuote&,void*,void*){
    }
    virtual bool Load(xmlQuote&,void*,ErrorPager* e,void*){
        return true;
    }
    AUTONEW(_method);
};
template <class BaseT>
BaseClass* _get_method( BaseT* root, void (BaseT::*call)() ){
    _method<BaseT>* m=new _method<BaseT>(call);
    return m;
}
#define REG_METHOD_EX2(method,name) \
	{BaseClass* m=_get_method(this,method); \
	 BaseClass::RegisterOneMember(m,this,name,0,GetClassName(),0,CurrMask);}

#define REG_METHOD_EX(method,name) REG_METHOD_EX2(method,#name)

#define REG_METHOD(method) REG_METHOD_EX2(method,#method)





template <class BaseT,class restype>
class _functor: public BaseClass{    
public:
    typedef restype (BaseT::*_fnprocess)();
    _fnprocess m_fnProcess;

    _functor(){
        m_fnProcess=NULL;
    }
    _functor(_fnprocess call){
        m_fnProcess = call;
    }
    virtual const char* getText(void* ptr){
        return "";
    }
    const char* GetClassName(){
		return "_functor";
	}
	virtual bool CheckIfFunction(){
		return true;
	}	
	virtual bool CheckDirectCasting(){
		return false;
	}
    virtual void call(void* base,void* result){
        if(base){
            (((BaseT*)base)->*m_fnProcess)();
        }        
    }
    virtual void Save(xmlQuote&,void*,void*){
    }
    virtual bool Load(xmlQuote&,void*,ErrorPager* e,void*){
        return true;
    }
    AUTONEW(_functor);
};
template <class BaseT>
class _float_functor:public _functor<BaseT,float>{
public:
    _float_functor(_fnprocess call){
        m_fnProcess = call;
    }
    virtual const char* getText(void* ptr){
        char* text=GetGlobalBuffer();
        if(m_fnProcess){
            sprintf(text,"%f",((BaseT*)ptr->*m_fnProcess)());
            return text;
        }else return "";
    }
};
template <class BaseT>
class _int_functor:public _functor<BaseT,int>{
public:
    _int_functor(_fnprocess call){
        m_fnProcess = call;
    }
    virtual const char* getText(void* ptr){
        char* text=GetGlobalBuffer();
        if(m_fnProcess){
            sprintf(text,"%d",((BaseT*)ptr->*m_fnProcess)());
            return text;
        }else return "";
    }
};
template <class BaseT>
class _bool_functor:public _functor<BaseT,bool>{
public:
    _bool_functor(_fnprocess call){
        m_fnProcess = call;
    }
    virtual const char* getText(void* ptr){
        char* text=GetGlobalBuffer();
        if(m_fnProcess){
            if(((BaseT*)ptr->*m_fnProcess)())return "true";
            else return "false";
        }else return "false";
    }
};
template <class BaseT>
class _char_functor:public _functor<BaseT,const char*>{
public:
    _char_functor(_fnprocess call){
        m_fnProcess = call;
    }
    virtual const char* getText(void* ptr){
        if(m_fnProcess)return ((BaseT*)ptr->*m_fnProcess)();
        else return "";
    }
};

template <class BaseT>
BaseClass* _get_float_functor( BaseT* root, float (BaseT::*call)() ){
    _float_functor<BaseT>* m=new _float_functor<BaseT>(call);
    return m;
}

template <class BaseT>
BaseClass* _get_int_functor( BaseT* root, int (BaseT::*call)() ){
    _int_functor<BaseT>* m=new _int_functor<BaseT>(call);
    return m;
}

template <class BaseT>
BaseClass* _get_bool_functor( BaseT* root, bool (BaseT::*call)() ){
    _bool_functor<BaseT>* m=new _bool_functor<BaseT>(call);
    return m;
}

template <class BaseT>
BaseClass* _get_char_functor( BaseT* root, const char* (BaseT::*call)() ){
    _char_functor<BaseT>* m=new _char_functor<BaseT>(call);
    return m;
}

#define REG_FN_FLOAT_EX2(method,name) \
{BaseClass* m=_get_float_functor(this,method); \
    BaseClass::RegisterOneMember(m,this,name,0,GetClassName(),0,CurrMask);}

#define REG_FN_FLOAT_EX(method,name) REG_FN_FLOAT_EX2(method,#name)

#define REG_FN_FLOAT(method) REG_FN_FLOAT_EX2(method,#method)


#define REG_FN_INT_EX2(method,name) \
{BaseClass* m=_get_int_functor(this,method); \
    BaseClass::RegisterOneMember(m,this,name,0,GetClassName(),0,CurrMask);}

#define REG_FN_INT_EX(method,name) REG_FN_INT_EX2(method,#name)

#define REG_FN_INT(method) REG_FN_INT_EX2(method,#method)


#define REG_FN_BOOL_EX2(method,name) \
{BaseClass* m=_get_bool_functor(this,method); \
    BaseClass::RegisterOneMember(m,this,name,0,GetClassName(),0,CurrMask);}

#define REG_FN_BOOL_EX(method,name) REG_FN_BOOL_EX2(method,#name)

#define REG_FN_BOOL(method) REG_FN_BOOL_EX2(method,#method)


#define REG_FN_CHAR_EX2(method,name) \
{BaseClass* m=_get_char_functor(this,method); \
    BaseClass::RegisterOneMember(m,this,name,0,GetClassName(),0,CurrMask);}

#define REG_FN_CHAR_EX(method,name) REG_FN_CHAR_EX2(method,#name)

#define REG_FN_CHAR(method) REG_FN_CHAR_EX2(method,#method)



#pragma pack(pop)
#endif __CLASSENGINE_H__