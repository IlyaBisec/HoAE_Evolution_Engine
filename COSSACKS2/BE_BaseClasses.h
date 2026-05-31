#ifndef __BE_BaseClasses__
#define __BE_BaseClasses__

bool	BE_UseCamera();
void	BE_NewSetCamera();
void	BE_DelCamera();

// SINGLETON TEMPLATE CLASS
template<class T>
class	be_CSingleton : public BaseClass
{
public:
	be_CSingleton(){
		assert(!ms_Singleton);
		int offset = (int)(T*)1 - (int)(be_CSingleton<T>*)(T*)1;
		ms_Singleton = (T*)((int)this + offset);
	};
	~be_CSingleton(){
		assert(ms_Singleton);
		ms_Singleton=0;
	};

	static T&	GetObjectRef(void){
		assert(ms_Singleton);
		return *ms_Singleton;
	};
	static T*	GetObjectPtr(void){
		return	ms_Singleton;
	};
	
private:
	static	T* ms_Singleton;
};

template<class T>	T* be_CSingleton<T>::ms_Singleton=0;

// experience manager ////////////////////////////////////////////////////
class ExperienceManager
{
public:
	ExperienceManager()		{};
	~ExperienceManager()	{ RemoveInfo(); };
public:
	virtual	int		GetExp();								// Total Experience
	virtual	int		GetNCtg();								// Categiris Nunber
	virtual	int		GetNCol(int ctg);						// Column Number (-1 if no ctg in list)
	virtual	bool	GetExpInfo(_str& Info,int ctg,int col);	// Category/Column Info
	virtual	void	RemoveInfo();							// Remove all information
public:
	static	ExperienceManager*	GetObj()							{ return m_pObj; };
	static	void				SetObj(ExperienceManager* pObj)		{ m_pObj = pObj; };
	static	void				Delete()							{ if (m_pObj!=NULL) { delete m_pObj; m_pObj=NULL; } };
	virtual void				Accetss()							{};
    ClassArray< ClassArray< _str > >	DATA;	// Data for draw
	int									EXPA;	// Total Experience
protected:
	static	ExperienceManager*			m_pObj;
};

inline	ExperienceManager* gExpMan(){
	if (ExperienceManager::GetObj()==NULL) {
		ExperienceManager::SetObj(new ExperienceManager);
	};
	return	ExperienceManager::GetObj();
};
//////////////////////////////////////////////////////////////////////////

// PRESENTATION //////////////////////////////////////////////////////////
#include "ClassPresentation.h"
class WCP_MASK_FILE : public BaseClass
{
public:
	_str MASK_FILE;
	SAVE(WCP_MASK_FILE);
		REG_FILEPATH(MASK_FILE,".xml");
	ENDSAVE;
};
extern WholeClassPresentation CE_WCP;
class WCP_Editor : public BaseClass
{
public:
	WCP_Editor(){Init=true;};
	bool	Init;
//	WholeClassPresentation WCP;
	void	ShowEditor();

	ClonesArray< WCP_MASK_FILE >	MASK_LIST;
	void	LoadMASK();
	void	LoadDEFMASK();

	class LOAD_ALL_MASK : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(LOAD_ALL_MASK);	
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}LoadAllMask;

	SAVE(WCP_Editor);
		REG_LOADSAVE("Mission\\Text\\","WCP_LIST.xml");
		REG_AUTO(MASK_LIST);
		REG_AUTO(LoadAllMask);
	ENDSAVE;
};
extern	WCP_Editor	g_WCP_EDITOR;
class CPRESENTATION : public BaseFunction
{
public:
	_str FileName;
	void EvaluateFunction();
	SAVE(CPRESENTATION);
		REG_PARENT(BaseFunction);
		REG_FILEPATH(FileName,".lua");
	ENDSAVE;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//						PRO MISSION										//
//////////////////////////////////////////////////////////////////////////
class lvCOperation;
class COPERCOND_CALLDESCR : public BaseClass
{
public:
	COPERCOND_CALLDESCR()	{ DESCR=""; VIEW=""; NCALL=0; };
	_str	DESCR;	// operation condition description
	_str	VIEW;   // operation view
	int		NCALL;	// calling number
	virtual const	char*	GetThisElementView(const char* LocalName);
	void	saveToFile(FILE* file);
	SAVE(COPERCOND_CALLDESCR);
		REG_AUTO(DESCR);
		REG_AUTO(VIEW);
		REG_MEMBER(_int,NCALL);
	ENDSAVE;
};
class CPRO_MISS_FILER : public BaseClass
{
public:
	CPRO_MISS_FILER(){ USE_CALL_OPERATION=false; USE_CALL_OPER_COND=false; _SIZE=0; };
	template<class T> int	getNEW_SELL(BaseClass* pOB);
	void	addCALL(int ID);
	void	clearCALL_DATA();
	ClonesArray<COPERCOND_CALLDESCR>	DATA;	// Calling data
	int									_SIZE;
	bool								USE_CALL_OPERATION;
	bool								USE_CALL_OPER_COND;
	class CCLEAR : public BaseFunction{
	public:
		void EvaluateFunction();
		SAVE(CCLEAR);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}fClear;
	SubSection	ANALIZE;
	void  clearSD();
	void  setSD_MoreThenNCall(int NCall);
	class CPARM_DEPEND_ONLY : public BaseFunction
	{
	public:
		int  MoreThenNCall;
		void EvaluateFunction();
		SAVE(CPARM_DEPEND_ONLY);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_int,MoreThenNCall);
		ENDSAVE;
	}fAPLAY_PARAM_SORT;
	ClonesArray<COPERCOND_CALLDESCR>	SORT_DATA;
	void	Save_log(char* FName);
	class CSAVE_LOG : public BaseFunction
	{
	public:
		_str FName;
		void EvaluateFunction();
		SAVE(CSAVE_LOG);
			REG_PARENT(BaseFunction);
			REG_FILEPATH(FName,".log");
		ENDSAVE;
	}fSave_log;
	SAVE(CPRO_MISS_FILER);
		REG_MEMBER(_bool,USE_CALL_OPERATION);
		REG_MEMBER(_bool,USE_CALL_OPER_COND);
		REG_AUTO(fClear);
		REG_AUTO(DATA);
		REG_AUTO(ANALIZE);
		REG_AUTO(fAPLAY_PARAM_SORT);
		REG_AUTO(SORT_DATA);
		REG_AUTO(fSave_log);
	ENDSAVE;
};
extern CPRO_MISS_FILER	g_PMF;
template<class T> int		CPRO_MISS_FILER::getNEW_SELL(BaseClass* pOB){
	int ID=-1;
	T* pNOB = dynamic_cast<T*>(pOB);
	if (pNOB!=NULL){
		COPERCOND_CALLDESCR* pnew = new COPERCOND_CALLDESCR;
		pnew->DESCR = pNOB->Descr.pchar();
		pnew->VIEW  = pNOB->GetThisElementView(NULL);
		ID=DATA.GetAmount();
		DATA.Add(pnew);
		_SIZE++;
	};
	return ID;
};
//////////////////////////////////////////////////////////////////////////

// for skirmish //////////////////////////////////////////////////////////
CEXPORT bool	g_addTask(const byte NI,const int x,const int y,const char* name); // position on map in pixel
CEXPORT int		g_delTask(const char* name = NULL);
//////////////////////////////////////////////////////////////////////////

#endif//__BE_BaseClasses__
























