#ifndef	__VALUE__STORAGE__ 
#define	__VALUE__STORAGE__

template<class T>
class	be_CONETYPE : public ClonesArray<T>
{
public:
	virtual bool ForceSimplification()	{ return true; };

	void	CLEAR(){
		int N = GetAmount();
		while (N--) {
			delete (*this)[N];
			(*this)[N] = NULL;
		};
		Clear();
	};

	virtual	bool DelByGID(DWORD ID){
		int N = GetAmount();
		T* pOB = NULL;
		while ( N-- ) {
			pOB = (*this)[N];
			if ( pOB!=NULL && pOB->GetObjectGlobalID()==ID ){
				DelElement(N);
				return true;
			};
		};
		return false;
	};
	virtual	T*	GetByGID(DWORD ID){
		int N = GetAmount();
		T* pOB = NULL;
		while ( N-- ) {
			pOB = (*this)[N];
			if ( pOB!=NULL && pOB->GetObjectGlobalID()==ID ) {
				return pOB;
			};
		};
		return pOB;
	};
	virtual	T*	GetByName(const char* _name){
		int N = GetAmount();
		T* pOB = NULL;
		while ( N-- ) {
			pOB = (*this)[N];
			if ( pOB!=NULL && pOB->Name.pchar() && pOB->Name.pchar()[0] && strcmp(pOB->Name.pchar(),_name)==0 ) {
				return pOB;
			};
		};
		return pOB;
	};
	virtual int GetIDByGID(DWORD ID){
		int N = GetAmount();
		T* pOB = NULL;
		while ( N-- ) {
			pOB = (*this)[N];
			if ( pOB!=NULL && pOB->GetObjectGlobalID()==ID ) {
				return N;
			};
		};
		return -1;
	};
	virtual int GetIDByNAME(const char* _name){
		int N = GetAmount();
		T* pOB = NULL;
		while ( N-- ) {
			pOB = (*this)[N];
			if ( pOB!=NULL && pOB->Name.pchar()!=NULL && pOB->Name.pchar()[0] && strcmp(pOB->Name.pchar(),_name)==0 ) {
				return N;
			};
		};
		return -1;
	};
};

//	HIDE_ALL			=	0
//	SHOW_ALL			=	7
//	SHOW_VARIABLES		=	1
//	SHOW_MAP_OBJECTS	=	2
//	SHOW_SMART_OBJECTS	=	4 
class	be_CValueStore : public BaseClass
{
public:
	// Viewer conttols
	//DWORD		CurViewFLAG;			// combination of enVS_VIEWER_ST
	//DWORD		SetViewer(DWORD _flag);		
	//DWORD		GetClassMask();
	// List of global variables present on map

	SubSection							Values;
		be_CONETYPE<be_CBOOL>			BOOL_LIST;
		be_CONETYPE<be_CDWORD>			DWORD_LIST;
		be_CONETYPE<be_CINT>			INT_LIST;
		be_CONETYPE<be_CFLOAT>			FLOAT_LIST;
		be_CONETYPE<be_CSTR>			STR_LIST;
		be_CONETYPE<be_CARRAY>			ARRAY_LIST;

	SubSection							MapObjects;
		be_CONETYPE<be_CGROUP>			GROUP_LIST;
		be_CONETYPE<be_CNODE>			NODE_LIST;
		be_CONETYPE<be_CBLANK>			BLANK_LIST;
		be_CONETYPE<be_CUnitType>		UNITSTYPE_LIST;
		be_CONETYPE<be_CFurmationType>	FORMSTYPE_LIST;

	SubSection							CameraObjects;
		be_CONETYPE<be_CCAMERA>			CAMERA_LIST;
			
	// List of smart control object present on map
	SubSection								SmartObjects;
			be_CONETYPE<be_CFSM>			FSM_LIST;
			be_CONETYPE<be_CSMARTOBJECT>	SMARTOBJ_LIST;
			be_CONETYPE<be_CMSG_FROMUSER>	USERMSG_LIST;

	SubSection								DialogsEditor;
			be_CONETYPE<be_CSTR_DIALOG>		DIALOGS_LIST;
			void	_UpdateDialogs();
			void	_SaveDialogs(_str& _fileName);
			void	_LoadDialogs(_str& _fileName);
			class CVS_UpdateDialogs : public BaseFunction
			{
			public:
				void	EvaluateFunction();
				SAVE(CVS_UpdateDialogs);
				REG_PARENT(BaseFunction);
				ENDSAVE;
			}UpdateDialogs;
			class CVS_SaveDialogs : public BaseFunction
			{
			public:
				_str	FileName;
				void	EvaluateFunction();
				SAVE(CVS_SaveDialogs);
				REG_PARENT(BaseFunction);
				REG_FILEPATH(FileName,".txt");
				ENDSAVE;
			}SaveDialogs;
			class CVS_LoadDialogs : public BaseFunction
			{
			public:
				_str	FileName;
				void	EvaluateFunction();
				SAVE(CVS_LoadDialogs);
				REG_PARENT(BaseFunction);
				REG_FILEPATH(FileName,".txt");
				ENDSAVE;
			}LoadDialogs;
			class CVS_TestDialogs : public BaseFunction
			{
			public:
				void	EvaluateFunction();
				SAVE(CVS_TestDialogs);
				REG_PARENT(BaseFunction);
				ENDSAVE;
			}TestDialogs;
	SubSection								SubFunctionsEditor;
			be_CONETYPE<be_CSUBFUNCTION>	SUBF_LIST;
			be_CSUBFUNCTION*				getSubFby_GROUP( be_CGROUP* pGR );

public:
	void	DRAW();
	void	UPDATE();
	void	UPDATE_AFTER_LOAD();
	void	PROCESS();
public:
	// SYSTEM
	void	CLEAR();
	// For use in enumerators
	be_CONETYPE<ReferableBaseClass>*		GET( char* ClassTypeName );	
	ReferableBaseClass*						GET( char* ClassTypeName, char* ObjectName );
	ReferableBaseClass*						GET( char* ClassTypeName, int   ID );
	
protected:
	void	DRAW_NODES(DWORD mode);
	void	DRAW_BLANKS(DWORD mode);
	void	DRAW_GROUPS();
	void	DARW_CAMERAS();

	void	UPDATE_GROUPS();
	void	UPDATE_GROUPS_AFTER_LOAD();

	void	PROCESS_SUBF();
public:
	SAVE(be_CValueStore);
		REG_LOADSAVE("Mission\\","TOTAL_VAL.xml");
		REG_AUTO(Values);
			REG_AUTO(BOOL_LIST);
			REG_AUTO(DWORD_LIST);
			REG_AUTO(INT_LIST);
			REG_AUTO(FLOAT_LIST);
			REG_AUTO(ARRAY_LIST);
		REG_AUTO(MapObjects);
			REG_AUTO(GROUP_LIST);
			REG_AUTO(NODE_LIST);
			REG_AUTO(BLANK_LIST);
		REG_AUTO(CameraObjects);
			REG_AUTO(CAMERA_LIST);
		REG_AUTO(SmartObjects);
			REG_AUTO(FSM_LIST);
			REG_AUTO(SMARTOBJ_LIST);
			REG_AUTO(USERMSG_LIST);
		REG_AUTO(DialogsEditor);
			REG_AUTO(UpdateDialogs);
			REG_AUTO(SaveDialogs);
			REG_AUTO(LoadDialogs);
			REG_AUTO(TestDialogs);
			REG_AUTO(DIALOGS_LIST);
		REG_AUTO(SubFunctionsEditor);
			REG_AUTO(SUBF_LIST);
	ENDSAVE;
};

extern	be_CValueStore g_beVAR_STORE;
// FSM .xml file with name and file path
class	be_CNameFile : public BaseClass 
{
public:
	_str	Name;
	_str	FilePath;
	SAVE(be_CNameFile);
		REG_AUTO(Name);
		REG_FILEPATH(FilePath,".xml");
	ENDSAVE;
};
class	be_CFSM_LOADER : public BaseClass
{
public:
	be_CFSM_LOADER();
	ClonesArray<be_CNameFile>	FSM_FILE_LIST;
	bool						Init;
	void						Load();
	bool						GetFilePath(_str& _filePath, const char* FSM_Name);
	SAVE(be_CFSM_LOADER);
		REG_LOADSAVE("Mission\\",".xml");
		REG_AUTO(FSM_FILE_LIST);
	ENDSAVE;
};
extern be_CFSM_LOADER g_beFSM_LOADER;
// ACTIONS ///////////////////////////////////////////////////////////////
/*
Данный клас используется для хранения стека действий разработчикаи их выполнения.
Делает возможным делать обратные действия -> Ctrl+z
*/
class be_CBASE_ACTION : public BaseClass
{
public:
	be_CBASE_ACTION() { ActionName = ""; };
	virtual	~be_CBASE_ACTION()	{  };

	_str				ActionName;
	virtual	bool		remove(){ return false; };
};
/*
Специальный интерфейсный сласс для выполнения действий с нодами
Работает только для нод.
Имеет список прямых и обратных функций для организации стека действий.
Каждое дейсвие создает и возращает объект по которому можно сделать обратное действие
*/
class be_CVNODE_ACT : public be_CBASE_ACTION
{
public:
	virtual	~be_CVNODE_ACT();

	be_CNODE*	pNode;
	
	virtual	be_CONETYPE<be_CNODE>*	vGetNodeList();

	virtual	be_CNODE*	vAddNode(int x,int y,char* name=NULL);
	
	virtual	bool		vDelNodeID(DWORD _id);
	virtual	bool		vDelNodePT(be_CNODE* pND);
	virtual	bool		vDelNode(int _x,int _y);

	virtual	be_CNODE*	vGetNode(DWORD _id);
	virtual	be_CNODE*	vGetNode(int _x,int _y);

	virtual	bool		remove();
};
/*

*/
class be_CVGROUP_ACT : public be_CBASE_ACTION
{
public:
	virtual	~be_CVGROUP_ACT();

	be_CGROUP*	pGroup;

	virtual	be_CONETYPE<be_CGROUP>*	vGetGroupList();

	virtual	be_CGROUP*	vAddGroup		(const char* _name);

	virtual	bool		vDelGroupID		(DWORD _id);
	virtual	bool		vDelGroupName	(const char* _name);
	virtual	bool		vDelGroupPT		(be_CGROUP* pGR);

	virtual	be_CGROUP*	vGetGroup(DWORD _id);
	virtual	be_CGROUP*	vGetGroup(const char* _name);

	virtual	bool		remove();
};
// ACTION STORAGE
class	be_CACT_STOR : public ClassArray<be_CBASE_ACTION>
{
public:
	bool	removeLAST();
};
extern	be_CACT_STOR	g_beACT_STORE;

void	be_REG_CLASS_VariablesStore();
//////////////////////////////////////////////////////////////////////////
#endif//__VALUE__STORAGE__