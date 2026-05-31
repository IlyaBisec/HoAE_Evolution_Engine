#pragma once

class WholeClassPresentation;

//**********************************************************************//
//****************   CONDITION FOR PRESENTATION   **********************//
//**********************************************************************//
class OnePresentationCondition:public BaseClass{
public:

	virtual bool Check(BaseClass* Class,void* DataPtr,void* ExtraPtr);

	SAVE(OnePresentationCondition);
	ENDSAVE;	
};
class pc_IfClassFieldIsEqual:public OnePresentationCondition{
public:

	_str FieldName;
	_str FieldValue;

	virtual bool		Check(BaseClass* Class,void* DataPtr,void* ExtraPtr);
			const char* GetThisElementView(const char*);

	SAVE(pc_IfClassFieldIsEqual);
		REG_PARENT(OnePresentationCondition);
		REG_AUTO(FieldName);
		REG_AUTO(FieldValue);
	ENDSAVE;
};
class pc_IfClassFieldIsNotEqual:public OnePresentationCondition{
public:

	_str FieldName;
	_str FieldValue;

	virtual bool		Check(BaseClass* Class,void* DataPtr,void* ExtraPtr);
			const char* GetThisElementView(const char*);

	SAVE(pc_IfClassFieldIsNotEqual);
		REG_PARENT(OnePresentationCondition);
		REG_AUTO(FieldName);
		REG_AUTO(FieldValue);
	ENDSAVE;
};
class pc_OR:public OnePresentationCondition{
public:

	ClassArray<OnePresentationCondition> OR_List;

	virtual			bool	Check(BaseClass* Class,void* DataPtr,void* ExtraPtr);
			const	char*	GetThisElementView(const char*);

	SAVE(pc_OR);
		REG_PARENT(OnePresentationCondition);
		REG_AUTO(OR_List);
	ENDSAVE;
};
class pc_AND:public OnePresentationCondition{
public:

	ClassArray<OnePresentationCondition> AND_List;

	virtual			bool	Check(BaseClass* Class,void* DataPtr,void* ExtraPtr);
	const	char*	GetThisElementView(const char*);

	SAVE(pc_AND);
		REG_PARENT(OnePresentationCondition);
		REG_AUTO(AND_List);
	ENDSAVE;
};
class ConditionsList:public ClassArray<OnePresentationCondition>{
public:

	const char* GetThisElementView(const char*);
};
//**********************************************************************//
//***************   HELPER PRESENTATION CLASS   ***********************//
//**********************************************************************//
class OnePresentationItem:public BaseClass{
public:
	ClassArray<OnePresentationCondition> Condition;
	_str Text;
//	_str TextID;	// Use inside

	char* GetText();

	virtual const	char*	GetThisElementView(const char* LocalName);

	virtual	bool	Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);
	virtual void	Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);

	bool			LoadFildsUseTextID();
	void			FillTextID();
	char*			GetPresentationPrefix();

	SAVE(OnePresentationItem);
		REG_AUTO(Condition);
		REG_AUTO(Text);
//		INVISIBLE REG_AUTO(TextID);
	ENDSAVE;
};
class ClassPresentation:public BaseClass{
public:
	
	virtual	bool		AdditionCheck0		(const char* __ClassName, char* __DirectClassName, char* __KeyName);
	virtual	bool		haveMask			(const char* _ClassName);
	virtual	const char* GetPresClassName	();
	virtual	const char* GetThisElementView	(const char*);
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);

	SAVE(ClassPresentation);
	ENDSAVE;
};
class ClassFieldHintAndText:public BaseClass{
public:

	virtual const	char*	GetThisElementView(const char* LocalName);

    _str FieldName;
	_str Hint;
	_str TextToShow;

	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual	bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);
    
	// Use for update ::Hint and ::TextToShow filds using text ID loading from file
	bool LoadFildsUseTextID();	
	bool SaveFildsUseTextID();

	char*	GetParentName();
    void	SaveInTextFile(_str& _strPresentation);

	class CGetFildsUseTextID : public BaseFunction
	{
	public:
		void	EvaluateFunction();
		SAVE(CGetFildsUseTextID);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}GetFildsUseTextID;

	SAVE(ClassFieldHintAndText);
		REG_ENUM(_strindex,FieldName,CLASSFIELD);
		REG_AUTO(TextToShow);
		REG_AUTO(Hint);		
		REG_AUTO(GetFildsUseTextID);
	ENDSAVE;
	
};
class ClassTextAndENUMHint : public BaseClass
{
public:

	virtual const	char*	GetThisElementView(const char* LocalName);

	_str	CLASSID;	// ID of class name for show in class enumerator list
	_str	ENUMHINT;	// ID of hint for class name in class enumerator list
	_str	CLASSHINT;	// ID of hint for object of current class

	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);
	virtual	bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);

	// Use for update ::Hint and ::TextToShow filds using text ID loading from file
	bool LoadFildsUseTextID();	
	bool SaveFildsUseTextID();

	char*	GetParentName();
	void	SaveInTextFile(_str& _strPresentation);

	class CGetFildsUseTextID : public BaseFunction
	{
	public:
		void	EvaluateFunction();
		SAVE(CGetFildsUseTextID);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}GetFildsUseTextID;

	SAVE(ClassTextAndENUMHint);
		REG_AUTO(CLASSID);
		REG_AUTO(ENUMHINT);
		REG_AUTO(CLASSHINT);
		REG_AUTO(GetFildsUseTextID);
	ENDSAVE;
	
};
class TextClassPresentation:public ClassPresentation{
public:	

	_str	ClassName;

	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	ClonesArray<OnePresentationItem>	Presentation;
	ClonesArray<ClassFieldHintAndText>	FieldsDescription;
	ClassTextAndENUMHint				TextAndENUMHint;

	void	SaveInTextFile(_str& _strPresentation);
	
	SAVE(TextClassPresentation);
		REG_PARENT(ClassPresentation);
		REG_ENUM(_strindex,ClassName,ALLCLASSES);
		REG_AUTO(Presentation);
		REG_AUTO(FieldsDescription);
		REG_AUTO(TextAndENUMHint);
	ENDSAVE;
};
//**********************************************************************//
//***************   SIMPLE CLASS PRESENTATION   ************************//
//**********************************************************************//
class _int_Presentation:public ClassPresentation{
public:

	_int_Presentation();

	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	SAVE(_int_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _WORD_Presentation:public ClassPresentation{
public:

	_WORD_Presentation();

	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	SAVE(_WORD_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _DWORD_Presentation:public ClassPresentation{
public:

	_DWORD_Presentation();

	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	SAVE(_DWORD_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _UnitType_Presentation:public ClassPresentation{
public:

	_UnitType_Presentation();

	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,		
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	    
	SAVE(_UnitType_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _float_Presentation:public ClassPresentation{
public:

	_float_Presentation();

	virtual	bool		haveMask			(const char* _ClassName);
	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	SAVE(_float_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _str_Presentation:public ClassPresentation{
public:

	_str_Presentation();

	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,	
												WholeClassPresentation* pPresStore	);
	
	SAVE(_str_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _strindex_Presentation:public _str_Presentation{
public:

	virtual	const char* GetPresClassName();

	SAVE(_strindex_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _index_Presentation:public ClassPresentation{
public:

	_index_Presentation();

	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	SAVE(_index_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _bool_Presentation:public ClassPresentation{
public:

	_bool_Presentation();

	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	SAVE(_bool_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _gpfile_Presentation:public ClassPresentation{
public:

	_gpfile_Presentation();

	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	SAVE(_gpfile_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
bool	g_GetOptionParamAfterComma(_str& OptionPar,_str& _string);
class _ClassRef_Presentation:public ClassPresentation{
public:

	_ClassRef_Presentation();

	virtual	bool		haveMask			(const char* _ClassName);
	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	SAVE(_ClassRef_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _ClassPtr_Presentation:public ClassPresentation{
public:

	_ClassPtr_Presentation();

	virtual	bool		haveMask			(const char* _ClassName);
	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	  
	SAVE(_ClassPtr_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _ClassArray_Presentation:public ClassPresentation{
public:

	_ClassArray_Presentation();

	virtual	bool		haveMask			(const char* _ClassName);
	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	SAVE(_ClassArray_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
class _ClonesArray_Presentation:public ClassPresentation{
public:

	_ClonesArray_Presentation();

	virtual	bool		haveMask			(const char* _ClassName);
	virtual	const char* GetPresClassName	();
	virtual void		GetPresentationMask	(	_str&					Dest	,
												BaseClass*				Class	,
												void*					Data	,
												void*					Extra	,
												char*					Opt		,
												WholeClassPresentation* pPresStore	);
	
	SAVE(_ClonesArray_Presentation);
		REG_PARENT(ClassPresentation);
	ENDSAVE;
};
//**********************************************************************//
//***************   GENERAL PRESENTATION CLASS   ***********************//
//**********************************************************************//
class ClassPresentationList:public BaseClass{
public:

	_str								Description;
	_str								PresentationPrefix;
	ClonesArray<TextClassPresentation>	ClassList;

	_str	TextFileName;
	void	SaveInTextFile();

	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL);

	SAVE(ClassPresentationList);
		REG_FILEPATH(TextFileName,".txt");
		REG_AUTO(Description);
		REG_AUTO(PresentationPrefix);
		REG_AUTO(ClassList);
		REG_LOADSAVE("ClassEngine\\","*.ClassList.xml");
	ENDSAVE;
};
class WholeClassPresentation:public BaseClass{
public:
	WholeClassPresentation();

	ClassArray<ClassPresentation> ClassList;

	void				SetStdPresentation();
	void				AddOnePresentation(ClassPresentation* P);
	void				AddFromList(char* List);
	void				AddFromList(ClassPresentationList* List);
	void				GetPresentation(_str& Dest,BaseClass* Class);
	void				GetPresentation(char* Dest,BaseClass* Class);
	void				GetPresentation(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt);
	ClassPresentation*	GetPresentation(const char* ClassName);

	void				FormatingString(_str& Dest);	// change "/"->'\n'
	void				FormatingString(char* Dest);	// change "/"->'\n'
};

// Helper functions //////////////////////////////////////////////////////
void RegisterPresentations();
























