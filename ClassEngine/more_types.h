#ifndef __MORE_TYPES_H__
#define __MORE_TYPES_H__
#pragma pack(push)
#pragma pack(1)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//some useful macro for properties definition//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//this macro can be used to access private members and bit fields
//DEF_VARPROP must be defined in public section
//classname - owner class name
//type      - type of variable
//x         - name of variable
#define DEF_VPROP(classname,type,x)\
	type _Get##x(){return x;}\
	void _Set##x(type v){x=v;}\
	static void* __Get##x(void* C){static type V=((classname*)C)->_Get##x();return &V;}\
	static void  __Set##x(void* C,void* V){((classname*)C)->_Set##x(*((type*)V))
//if you have defined DEF_VPROP int public section, you must
//define REG_VPROP in SAVE..ENDSAVE section
//type      - type of variable
//x         - name of variable
#define REG_VPROP(type,x) REG_PROP(type,x,Get##x,Set##x)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//        additional types definition        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
class DIALOGS_API _color:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==4;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();  
	virtual void reset_class(void* ptr){
		*((DWORD*)ptr)=0;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((int*)DestData)=*((int*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_color);
};

//_textid
class DIALOGS_API _variant:public _str{	
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return !strcmp(TypeName,"class _str");
	}
	virtual void GetVariants(ClonesArray<_str> &List){};
	virtual const char* GetClassName(){
		return "_variant";
	}
	AUTONEW(_variant);
};
class DIALOGS_API _ClassIndex:public _str{
public:
	_ClassIndex() : _str() { INDEX=-1; NUMBER=-1; };
	int INDEX;	// Index
	int NUMBER;	// Last element number in list

	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		bool res = _str::Load(xml,ClassPtr,Error,Extra);
		INDEX=-1; 
		NUMBER=-1;
		return res;
	};
	virtual void reset_class(void* ptr){ 
		_str::reset_class(ptr); 
		INDEX=-1; 
		NUMBER=-1; 
	};

	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==4;
	};
	virtual const char* GetClassName(){
		return "_ClassIndex";
	};
	virtual bool CheckDirectCasting(){
		return true;
	}
	BaseClass*	getARRAY(char* _ExtraPtr);
	BaseClass*	getELEMENT(char* _ExtraPtr);
	AUTONEW(_ClassIndex);
};
//
class DIALOGS_API _strindex:public _str{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return !strncmp(TypeName,"class _str",10);
	}
	virtual const char* GetClassName(){
		return "_strindex";
	}
	virtual bool CheckDirectCasting(){
		return true;
	}
	AUTONEW(_strindex);
};
class DIALOGS_API _textid:public BaseClass,public DString{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return !strcmp(TypeName,"char *");
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();   
	virtual void reset_class(void* ptr){
		char** pc=(char**)ptr;
		if(*pc){
			free(*pc);
			*pc=NULL;
		}		
	}
	AUTONEW(_textid);    
};//_textid
class DIALOGS_API _lpchar:public _textid{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return !strcmp(TypeName,"char *");
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();   
	virtual void reset_class(void* ptr){
		char** pc=(char**)ptr;
		if(*pc){
			free(*pc);
			*pc=NULL;
		}		
	}
	AUTONEW(_lpchar);    
};//_textid
class DIALOGS_API _WORD:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==2;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName(); 
	virtual void reset_class(void* ptr){
		*((WORD*)ptr)=0;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((word*)DestData)=*((word*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_WORD);
};//_WORD
class DIALOGS_API _short:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==2;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();  
	virtual void reset_class(void* ptr){
		*((short*)ptr)=0;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((word*)DestData)=*((word*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_short);
};//_short
class DIALOGS_API _char:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==1;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName(); 
	virtual void reset_class(void* ptr){
		*((char*)ptr)=0;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((char*)DestData)=*((char*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_char);
};//_char
class DIALOGS_API _BYTE:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==1;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName(); 
	virtual void reset_class(void* ptr){
		*((BYTE*)ptr)=0;
	}
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((byte*)DestData)=*((byte*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_BYTE);
};//_byte
//_font
class DIALOGS_API _font:public BaseClass,public DString{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return !strcmp(TypeName,"class RLCFont *");
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName(); 
	virtual void reset_class(void* ptr){
		*((DWORD*)ptr)=NULL;
	}
	AUTONEW(_font);    
};//_font
class DIALOGS_API _gframe:public BaseClass{
public:
	WORD FileID;
	WORD SpriteID;
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();    	
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		_gframe* dst=(_gframe*)DestData;
		dst->FileID=FileID;
		dst->SpriteID=SpriteID;
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	}
	AUTONEW(_gframe);    
};//_gframe
class DIALOGS_API _gpfile:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==2;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();    	
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((short*)DestData)=*((short*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	}
	AUTONEW(_gpfile);
};//_gpfile
//
class DIALOGS_API _picfile:public _str{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return !strcmp(TypeName,"class _str");
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();    	
	AUTONEW(_picfile);
};
class DIALOGS_API _ModelID:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==4;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();  
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((int*)DestData)=*((int*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	}
	AUTONEW(_ModelID);
};//_ModelID
class DIALOGS_API _TextureID:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==4;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();    	
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((int*)DestData)=*((int*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	}
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	}
	AUTONEW(_TextureID);
};//_TextureID
class _UnitType:public BaseClass{
public:
	virtual bool CheckCompartabilityWith(const char* TypeName,int TypeSize){
		return TypeSize==4;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual const char* GetClassName();
	virtual void reset_class(void* ptr);
	virtual bool FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContex* C){
		*((int*)DestData)=*((int*)SrcData);
		return true;
	}
	virtual void SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList);
	virtual	void LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary);
	AUTONEW(_UnitType);
};

#pragma pack(pop)
#endif //__MORE_TYPES_H__