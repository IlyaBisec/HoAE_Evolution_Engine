
#ifndef __PROCESS_BATTLE_EDITOR__
#define __PROCESS_BATTLE_EDITOR__

#ifdef	__LUA_DEBUGGER__
	extern	CScriptDebugger	g_LUA_DBG;
#endif//__LUA_DEBUGGER__

#include "stdheader.h"

#include "TestEditor.h"
#include "GameExtension.h"
#include "ClassEditorsRegistration.h"
class	CBE_HANDLER;
//CEXPORT int ItemChoose;

void	SetCurrentHelp(tpMessageArray** ppTASKS,tpMessageArray** ppTALKS,tpMessageArray** ppHINTS);
// Timer /////////////////////////////////////////////////////////////////
class TempTimerClass : public BaseClass
{	
public:
	TempTimerClass()	{};
	~TempTimerClass()	{};

	int TimerID;
	int Time;
	bool Used;
	bool First;
	int LastCheckAnimTime;
	bool trueTime;

	SAVE(TempTimerClass);
		REG_MEMBER(_int,TimerID);
		REG_MEMBER(_int,Time);
		REG_MEMBER(_bool,Used);
		REG_MEMBER(_bool,First);
		REG_MEMBER(_int,LastCheckAnimTime);
		REG_MEMBER(_bool,trueTime);
	ENDSAVE;
};
// CHECK UniqID in units on map //////////////////////////////////////////
class	be_CTemp_OOLIST		 : public BaseClass
{
public:
	LinearArray< DWORD, _DWORD >	UnitsList;
	DWORD							UniqID;
	void	__Clear();
	virtual	const	char*	GetThisElementView(const char* LoaclName);
	SAVE(be_CTemp_OOLIST);
		REG_MEMBER(_DWORD,UniqID);
		REG_AUTO(UnitsList);
	ENDSAVE;
};
class	be_CTemp_UniqID_Test : public BaseClass
{
public:
	ClassArray< be_CTemp_OOLIST >		List;
	bool	IsUniqIDInList(DWORD id);
	void	AddObject(OneObject* pOO);
	void	__Clear();
	void	createList();
	class CCREATE : public BaseFunction
	{
	public:	
		void	EvaluateFunction(){
			be_CTemp_UniqID_Test* pPR = (be_CTemp_UniqID_Test*)GetParent();
			pPR->createList();
		};
		SAVE(CCREATE);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}CREATE;
	SAVE(be_CTemp_UniqID_Test);	
		REG_AUTO(CREATE);
		REG_AUTO(List);
	ENDSAVE;
};
extern be_CTemp_UniqID_Test	g_beTempOBJECT;
// BE_EDIT_PANEL /////////////////////////////////////////////////////////
class BE_EDIT_PANEL : public BaseClass
{
public:
	BE_EDIT_PANEL();
	~BE_EDIT_PANEL();

	bool	EP_LOAD();
	bool	EP_DRAW();
	bool	EP_UPDATE();

	DialogsDesk*	EP_PANEL();
	TextButton*		EP_TEXT();
    
	void			EP_VISIBLE(bool state);
	void			EP_SET_TEXT(char* text);
	
	DialogsSystem	DS_PANEL;
	bool			Visible;
	bool			Error;

	int				X0,Y0,dY,mW,tX;
protected:
	void			EP_PROPER_SIZE_POS();

	void			EP_PANEL_VISIBLE(bool state);
	void			EP_TEXT_VISIBLE(bool state);
	void			EP_PANEL_POSITION(int x,int y);
	void			EP_TEXT_POSITION(int x,int y);
	void			EP_PANEL_SIZE(int w,int h);
	void			EP_TEXT_MAXWIDTH(int maxw);
};
extern	BE_EDIT_PANEL	MAIN_EDIT_PANEL;
// CBEDriveMode_ST ///////////////////////////////////////////////////////
class SingletonCBEDriveMode : public BaseClass
{
public:
	SingletonCBEDriveMode()				{};
	virtual ~SingletonCBEDriveMode()	{};
	static SingletonCBEDriveMode* GetObj()			{ return m_pObj; };
	static void SetObj(SingletonCBEDriveMode* pObj)	{ m_pObj = pObj; };
	static void Delete() { if (m_pObj!=NULL) { delete m_pObj; m_pObj=NULL; } };
	virtual void Accetss() {};
protected:
	static	SingletonCBEDriveMode* m_pObj;
};

inline SingletonCBEDriveMode* StCBEDriveMode(){
	if (SingletonCBEDriveMode::GetObj()==NULL) SingletonCBEDriveMode::SetObj(new SingletonCBEDriveMode);
	return SingletonCBEDriveMode::GetObj();
};

class CBEDriveMode_ST : public SingletonCBEDriveMode
{
public:
	CBEDriveMode_ST();
	virtual ~CBEDriveMode_ST();

	virtual void Access();
public:
	bool	visible;
	SubSection	FilmsMenu;
	SubSection	ProcessMenu;
	SubSection	ColorSettings;
	Delimiter	D0;
	Delimiter	D1;
	SubSection	SOMenu;
	SubSection	EdgeSetMenu;
	SubSection	NodeViewMode;
	SubSection	EdgeViewMode;
    // OBJECTS
	int		OBJECT;
	bool	USE_vGRP;		// Использовать vGROUPS
		// NODE
		int			NodeAct;
		int			NodeView;
		_str_ref	NodeType;
		bool	ND_RECT;
		bool	ND_XY;
		bool	ND_NAME;
		bool	ND_DESCR;
		class CBE_SAVE_NODE_LUA : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(CBE_SAVE_NODE_LUA);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}	fNodeList;
		class CBE_ON_NODE_SCREEN : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_ON_NODE_SCREEN);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}			fON_NODE_SCREEN;			// Центр экрана на be_cNODE
		_str_ref	NODE;
		// EDGE
		int	EdgeAct;
		int EdgeView;
		bool	EG_RECT;
		bool	EG_DIRECT;
		bool	EG_PROC;
		bool	EG_START;
		bool	EG_MODE;
		int		SQUARD;
		// SCRIPT
		int		PR_MODE;
		bool	PR_CREATE;
		class CBE_MainEdit : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_MainEdit);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}	fMAIN_EDIT;

		ClassRef<lvCFilm>	scrFILM;

		class CBE_AddFilm : public BaseFunction
		{
		public:
			CBE_AddFilm();
			~CBE_AddFilm();

			_str	scrName;
			_str	scrDescr;

			void EvaluateFunction();

			SAVE(CBE_AddFilm);
				REG_PARENT(BaseFunction);
				REG_AUTO(scrName);
				REG_AUTO(scrDescr);
			ENDSAVE;
		}	fADD_FILM;

		class CBE_DelFilm : public BaseFunction
		{
		public:
			void EvaluateFunction();			

			SAVE(CBE_DelFilm);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}	fDEL_FILM;
	
		class CBE_EditFilm : public BaseFunction
		{
		public:
			void EvaluateFunction();

			SAVE(CBE_EditFilm);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}	fEDIT_FILM;
		// ScriptGraph
		class CBE_EditGraph : public BaseFunction
		{
		public:
			void EvaluateFunction();

			SAVE(CBE_EditGraph);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}	fEDIT_GRAPH;
		class CBE_SAVE_GRAPH_LUA : public BaseFunction {
		public:
			_str FileName;
			void EvaluateFunction();
			SAVE(CBE_SAVE_GRAPH_LUA);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}	fGraphList;

		bool	PR_MAIN;
		bool	PR_SQUAD;
		bool	PROCESS;
		class CBE_Process : public BaseFunction
		{
		public:
			CBE_Process();
			bool	SQUARD_PR;
			bool	MAIN_PR;
			void EvaluateFunction();

			SAVE(CBE_Process);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_bool,MAIN_PR);
				REG_MEMBER(_bool,SQUARD_PR);
			ENDSAVE;

		}	fPROCESS;
		int		PR_TIME;
			// LUA
		int		GC_COUNT;
		int		GC_TRESHOLD;
		void	UPDATE_LUA_GC();
			// ---
		bool	InitSO;
		// SAVE
		bool	BE_SAVE;		// true for save in file
		_str	FileForScript;	// fro save all script in another file *.xml
		char*	GetSaveSFileName(); // return NULL if file not finde
		// vGROUPS
        _str_ref	BE_vGROUPS;
		bool		BE_vGRP_VISIBLE;
		class CBE_vGRP_VISIBLE_ALL : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_vGRP_VISIBLE_ALL);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}	fSHOW_ALL;
		class CBE_vGRP_VISIBLE : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_vGRP_VISIBLE);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}		fSHOW_CURR;
		class CBE_ADD_vGRP : public BaseFunction
		{
		public:
			CBE_ADD_vGRP();
			DWORD	GetClassMask();
			_str Name;
			// Fro create groups use all units on map
			bool	DivideAllUnits;
				_str	nameMask;	
				bool	collect;
					// if collectUnits==false
					int	firstIndex;
					int	lastIndex;
					// else 
					bool	collectBuildings;
					bool	collectUnits;
						// if collectBuildings || collectUnits
						int		nation;
						int		lockType;
			//-------------------------------------//
			void EvaluateFunction();
			SAVE(CBE_ADD_vGRP);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_bool,DivideAllUnits);
				SAVE_SECTION(0x00000002);
					REG_AUTO(Name);
				SAVE_SECTION(0x00000004);
					REG_AUTO(nameMask);
					REG_MEMBER(_bool,collect);
					REG_MEMBER(_int,firstIndex);
				SAVE_SECTION(0x00000008);
					REG_MEMBER(_int,lastIndex);
				SAVE_SECTION(0x00000010);
					REG_MEMBER(_bool,collectBuildings);
					REG_MEMBER(_bool,collectUnits);
				SAVE_SECTION(0x00000200);
					REG_MEMBER(_int,nation);
					REG_ENUM(_index,lockType,LOCKTYPE);
			ENDSAVE;
		}			fADD_GRP;			// Создать новую vGROUP
		class CBE_ADD_vGRP_SMART : public BaseFunction
		{
		public:
			_str Name;
			void EvaluateFunction();
			SAVE(CBE_ADD_vGRP_SMART);
				REG_PARENT(BaseFunction);
				REG_AUTO(Name);
			ENDSAVE;
		}		fADD_GRP_SAMRT;		// Создать новую vGROUP
		class CBE_DEL_vGRP : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_DEL_vGRP);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}			fDEL_GRP;			// Удалить существующую vGROUP
		class CBE_EDIT_vGRP : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_EDIT_vGRP);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}			fEDIT_GRP;			// Редактировать vGROUP
		class CBE_ADD_SelUnits : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_ADD_SelUnits);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}		fADD_SEL;			// Добавить выделленых юнитов к vGROUP
		class CBE_ON_SCREEN : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_ON_SCREEN);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}			fON_SCREEN;			// Центр экрана на vGROUP
		class CBE_CREATE_DEFAULT_VG : public BaseFunction
		{
		public:
			int		Nat;

			void EvaluateFunction();

			SAVE(CBE_CREATE_DEFAULT_VG);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_int,Nat);
			ENDSAVE;
		}	fCREATE_DEFAULT_VG;
		class CBE_SAVE_GROUP_STRUCT : public BaseFunction {
		public:
			_str FileName;
			void EvaluateFunction();
			SAVE(CBE_SAVE_GROUP_STRUCT);
				REG_PARENT(BaseFunction);
				REG_FILEPATH(FileName,".txt");
			ENDSAVE;
		}	fSaveGroupStruct;
		class CBE_SAVE_GROUP_LUA : public BaseFunction {
		public:
			_str FileName;
			void EvaluateFunction();
			SAVE(CBE_SAVE_GROUP_LUA);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}		fGroupList;
		class CBE_UPDATE_GROUPS : public BaseFunction{
			void EvaluateFunction();
			SAVE(CBE_UPDATE_GROUPS);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}		UPDATE_GROUPS;
		//  TESTING vGROUPS comand
		bool	BE_vGRP_TEST;
		class CBE_REM_N_UNT : public BaseFunction
		{
		public:
			CBE_REM_N_UNT();
			int destGRP_ID;
			int number;
			void EvaluateFunction();
			SAVE(CBE_REM_N_UNT);
				REG_PARENT(BaseFunction);
				REG_ENUM(_index,destGRP_ID,ALL_be_CGROUPS_ON_MAP);
				REG_MEMBER(_int,number);
			ENDSAVE;
		}				fREM_N_UNT;		// Убрать N юнитов из vGROUP
		class CBE_KILL_N_UNT : public BaseFunction
		{
		public:
			CBE_KILL_N_UNT();
			int number;
			void EvaluateFunction();
			SAVE(CBE_KILL_N_UNT);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_int,number);
			ENDSAVE;
		}				fKILL_N_UNT;	// Убить N юнитов из vGROUP
		class CBE_ERASE_N_UNT : public BaseFunction
		{
		public:
			CBE_ERASE_N_UNT();
			int number;
			void EvaluateFunction();
			SAVE(CBE_ERASE_N_UNT);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_int,number);
			ENDSAVE;
		}			fERASE_N_UNT;	// Исчезнуть N юнитов из vGROUP
		class CBE_UN_SELECT : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_ERASE_N_UNT);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}				fUN_SELECT;		// Снять выделение
		class CBE_SELECT : public BaseFunction
		{
		public:
			CBE_SELECT();
			bool add;
			void EvaluateFunction();
			SAVE(CBE_SELECT);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_bool,add);
			ENDSAVE;
		}					fSELECT;		// Выделить группу
		class CBE_SELECT_IN_XYR : public BaseFunction
		{
		public:

			int x,y,r;
			bool add;
			void EvaluateFunction();
			SAVE(CBE_SELECT_IN_XYR);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_int,x);
				REG_MEMBER(_int,y);
				REG_MEMBER(_int,r);
				REG_MEMBER(_bool,add);
			ENDSAVE;
		}			fSELECT_XYR;	// Выделить тодько юнитов в зоне x,y,r
		class CBE_SET_NATION : public BaseFunction
		{
		public:
			int NI;
			void EvaluateFunction();
			SAVE(CBE_SET_NATION);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_int,NI);
			ENDSAVE;
		}				fSET_NATION;	// Выставить цвет
		class CBE_SET_AGRESIVITY : public BaseFunction
		{
		public:
			int state;
			void EvaluateFunction();
			SAVE(CBE_SET_AGRESIVITY);
				REG_PARENT(BaseFunction);
				REG_ENUM(_index,state,BE_UNIT_MOVE_MODE);
			ENDSAVE;
		}			fSET_AGRESIV;	// Выставить уровень агресивности
		class CBE_SEND_TO : public BaseFunction
		{
		public:
			int x,y,dir,type;
			void EvaluateFunction();
			SAVE(CBE_SEND_TO);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_int,x);
				REG_MEMBER(_int,y);
				REG_MEMBER(_int,dir);
				REG_MEMBER(_int,type);
			ENDSAVE;
		}				fSEND_TO;		// Отправить в точку
		class CBE_SET_DIR : public BaseFunction
		{
		public:
			int dir,type;
			void EvaluateFunction();
			SAVE(CBE_SET_DIR);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_int,dir);
				REG_MEMBER(_int,type);
			ENDSAVE;
		}				fSET_DIR;		// Повернуть в заданном направлении
		class CBE_SET_FORM : public BaseFunction
		{
		public:
			int formType;
			void EvaluateFunction();
			SAVE(CBE_SET_FORM);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_int,formType);
			ENDSAVE;
		}				fSET_FORM;		// Поменять построение
		class CBE_SET_FLAGBR : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_SET_FLAGBR);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		}				fSET_FLAGBR;			// Назначить флагоносцев в бригаде
		class CBE_CLEAR_FLAGBR : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_CLEAR_FLAGBR);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		}			fCLEAR_FLAGBR;			// Убрать флагоносцев из бригады
		bool  GROUP_MENU_VS;
		//	vVALUES
		class CBE_vValuesEdit : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_vValuesEdit);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}			fvVALUES_EDIT;
		class CBE_CameraPossEdit : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_CameraPossEdit);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}			fCameraPoss;
		class CBE_BattleHandler : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_BattleHandler);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}			fBattleHendler;

		class CBE_BOOL_LST : public BaseFunction
		{
		public:
			CBE_BOOL_LST();
			~CBE_BOOL_LST();
			TypeShowMaskArray	MaskArray;
			void	EvaluateFunction();
			void	UnSelectAll();
			void	SelectAll();
			// Some usful buttons
			class CBE_UnSelectAll : public BaseFunction
			{
			public:
				void EvaluateFunction();
				SAVE(CBE_UnSelectAll);
					REG_PARENT(BaseFunction);
				ENDSAVE;
			} fUnSelectAll;
			class CBE_SelectAll : public BaseFunction
			{
			public:
				void EvaluateFunction();
				SAVE(CBE_SelectAll);
				REG_PARENT(BaseFunction);
				ENDSAVE;
			} fSelectAll;
			//-------------------
			SAVE(CBE_BOOL_LST);
				REG_PARENT(BaseFunction);
				REG_AUTO(fUnSelectAll);
				REG_AUTO(fSelectAll);
				REG_AUTO(MaskArray);
			ENDSAVE;
		}			SelectViewMask;

		//	SETTINGS
		_str_ref	SmartObject;
		int			UpdateDelta;
		int			LockType;
		bool		ShowHTT;
		bool		ShowLinks;
		bool		NOT_ZERO_ANLY;
		bool		ShowEXP;
		bool		ShowAST;
		bool		ShowVSB;
		int			TRIADA_NUM;
		bool		ShowID;
		bool		ShowXY;
		bool		Showxy;
		int			sAreaID;
		int			dAreaID;
		bool		DrawStoreWAY;
		class CBE_HTT_UPDATE : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_HTT_UPDATE);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		}HTT_UPDATE;
		class CBE_HTT_AST_ARRAY : public BaseFunction
		{
		public:
			int Nation;
			int LockType;
			int Power;
			void EvaluateFunction();
			SAVE(CBE_HTT_AST_ARRAY);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_int,Nation);
			REG_ENUM(_index,LockType,LOCKTYPE);
			REG_MEMBER(_int,Power);
			ENDSAVE;
		}HTT_AST_ARRAY;
		class CBE_HTT_AST_CREATE_WAY : public BaseFunction
		{
		public:
			int Nation;
			int startAreaID;
			int finishAreaID;
			int	MaxDanger;
			int	DangSteps;
			int	LockType;
			void EvaluateFunction();
			SAVE(CBE_HTT_AST_CREATE_WAY);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_int,Nation);
			REG_MEMBER(_int,startAreaID);
			REG_MEMBER(_int,finishAreaID);
			REG_MEMBER(_int,MaxDanger);
			REG_MEMBER(_int,DangSteps);
			REG_ENUM(_index,LockType,LOCKTYPE);
			ENDSAVE;
		}HTT_AST_CREATE_WAY;
		int		MainEditType;
		DWORD	EdgeNameColor;
		DWORD	EdgeLineColor;
		class	CBE_COLOR_SETINGS_SAVE : public BaseFunction
		{
		public:
			void	EvaluateFunction();
			SAVE(CBE_COLOR_SETINGS_SAVE);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		}SaveSettings;
		int		NodeStyle;
		class CBE_MISS_SET : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_MISS_SET);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}				fMISS_SET;
		class CBE_WCP_EDIT : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_WCP_EDIT);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}				fEDIT_WCP;
		class CSK_TASK_ED : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CSK_TASK_ED);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}fSkirmisTaskEditor;	
		class CSK_TASK_ADD : public BaseFunction
		{
		public:
			int NI; int x; int y; _str name;
			void EvaluateFunction();
			SAVE(CSK_TASK_ADD);
				REG_PARENT(BaseFunction);
				REG_MEMBER(_int,NI);
				REG_MEMBER(_int,x);
				REG_MEMBER(_int,y);
				REG_AUTO(name);
			ENDSAVE;
		}fSkirmisTaskAdd;	
		class CSK_TASK_DEL : public BaseFunction
		{
		public:
			_str name;
			void EvaluateFunction();
			SAVE(CSK_TASK_DEL);
				REG_PARENT(BaseFunction);
				REG_AUTO(name);
			ENDSAVE;
		}fSkirmisTaskDel;	
		
		//  EDIT
		int				EditOperation;		// Select Edge,Node,Squard.
		lvCEdge*		SelectedEdge;
		be_CNODE*		SelectedNodeBeg;
		be_CNODE*		SelectedNodeEnd;
		lvCSquardShema*	SelectedSquadShema;
		vvBASE*			SelectedValue;
		DWORD			CorectionType;
		class CBE_COPY_EDGE : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_COPY_EDGE);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}			fCOPY_EDGE;			// Копировать выбранный edge
		class CBE_SELECT_SQUAD : public BaseFunction
		{
			public:
				void EvaluateFunction();
				SAVE(CBE_SELECT_SQUAD);
					REG_PARENT(BaseFunction);
				ENDSAVE;
		}		fSELECT_SQUAD;			// Копировать выбранный edge
		class CBE_COPY_SQUAD : public BaseFunction
		{
		public:
			int	GrpID;
			void EvaluateFunction();
			SAVE(CBE_COPY_SQUAD);
				REG_PARENT(BaseFunction);
				REG_ENUM(_index,GrpID,ALL_be_CGROUPS_ON_MAP);
			ENDSAVE;
		}			fCOPY_SQUAD;			// Копировать выбранный edge
		class CBE_MESSGES_MGR : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_MESSGES_MGR);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}		fMESSGES_MGR;
		class CBE_ADD_MESSGES : public BaseFunction
		{
		public:
			_str					ParentName;
			ClassPtr<BaseMesMgrST>	Message;
			void EvaluateFunction();
			SAVE(CBE_ADD_MESSGES);
				REG_PARENT(BaseFunction);
				REG_AUTO(ParentName);
				REG_AUTO(Message);
			ENDSAVE;
		}		fADD_MESSGES;
		class CBE_FORM_SQUARD : public BaseFunction
		{
		public:
			ClonesArray<IIPara>		SquadForm;
			void EvaluateFunction();
			SAVE(CBE_FORM_SQUARD);
				REG_PARENT(BaseFunction);
				REG_AUTO(SquadForm);
			ENDSAVE;
		}		fFORM_SQUAD;
		class CBE_WRITE_LOG : public BaseFunction
		{
		public:
			ClassRef<vvMissionLOG>	vvLOG;
			void EvaluateFunction();
			SAVE(CBE_WRITE_LOG);
				REG_PARENT(BaseFunction);
				REG_AUTO(vvLOG);
			ENDSAVE;
		}			fWRITE_LOG;
		class CBE_SHOW_PROMF : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_SHOW_PROMF);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}fBE_SHOW_PROMF;
		class CBE_MAS_EDITOR : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_MAS_EDITOR);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		} fMultyAnimateStoreEditor;
		class CBE_BEMI_INIT : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_BEMI_INIT);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		} fINIT_BEMainInterface;
		class CBE_Test_FnStore : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_Test_FnStore);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		} fFnStore;
		class CBE_Test_VAR_STORE :public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_Test_VAR_STORE);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}fVarStore;
		class CBE_REM_LAST_ACTION : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_REM_LAST_ACTION);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}REM_LAST_ACTION;
		class CBE_DEL_ALL_ACTION : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_DEL_ALL_ACTION);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}DEL_ALL_ACTION;
		//	TIMER	
		ClonesArray<TempTimerClass>		TimerData;	// Data for cur timer
		class	CBE_TempUID_TEST : public BaseFunction
		{
		public:
			void EvaluateFunction();
			SAVE(CBE_TempUID_TEST);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}SHOW_TempUID_TEST;
		class CBE_SEND_MSG : public BaseFunction
		{
		public:
			void	EvaluateFunction();
			SAVE(CBE_SEND_MSG);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}SEND_MSG;
		class CBE_MOVE_VAR_TO_NEW_VAR : public BaseFunction
		{
		public:
			void	EvaluateFunction();
			SAVE(CBE_SEND_MSG);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}MOVE_VAR_TO_NEW_VAR;
		class CBE_Show_Cloud_List : public BaseFunction
		{
		public:
			void	EvaluateFunction();
			SAVE(CBE_Show_Cloud_List);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}Show_Cloud_List;
		class CBE_Show_Info_Dialog : public BaseFunction
		{
		public:
			void	EvaluateFunction();
			SAVE(CBE_Show_Info_Dialog);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}Show_Info_Dialog;
		class CBE_ShowCloudDialog : public BaseFunction
		{
		public:
			_str_ref	refDialog;
			_str_ref	refGroup;
			void	EvaluateFunction();
			SAVE(CBE_ShowCloudDialog);
				REG_PARENT(BaseFunction);
				REG_ENUM(_strindex,refDialog,BE_DIALOGS_ENUM);
				REG_ENUM(_strindex,refGroup,ALL_be_CGROUPS_ON_MAP);
			ENDSAVE;
		}ShowCloudDialog;
		class CBE_EditFSM_List : public BaseFunction
		{
		public:
			void	EvaluateFunction();
			SAVE(CBE_EditFSM_List);
				REG_PARENT(BaseFunction);
			ENDSAVE;
		}EditFSM_List;
		void			UpdateTimerData();
		// CAMERA
		be_CCAMERA	Camera;
		class CBE_SaveCamera : public BaseFunction
		{
		public:
			_str_ref	curEditCamera;
			void		EvaluateFunction();
			SAVE(CBE_SaveCamera);
				REG_PARENT(BaseFunction);
				REG_ENUM(_strindex,curEditCamera,ALL_be_CCAMERAS_ON_MAP);
			ENDSAVE;
		}SaveCamera;		
		class CBE_LoadCamera : public BaseFunction
		{
		public:
			_str_ref	curEditCamera;
			void		EvaluateFunction();
			SAVE(CBE_LoadCamera);
				REG_PARENT(BaseFunction);
				REG_ENUM(_strindex,curEditCamera,ALL_be_CCAMERAS_ON_MAP);
			ENDSAVE;
		}LoadCamera;		
		
		//  NOTHING
public:
	int		GetSquardID();
	DWORD	GetClassMask();
	DWORD	NodeMode();
	DWORD	EdgeMode();
	void	StartMission();
	void	StartMissionAfterLoad();

public:
	SAVE(CBEDriveMode_ST);
		REG_ENUM(_index,OBJECT,BE_OBJECT_TYPE_LIST);
		// "NODE"
		SAVE_SECTION(0x00000001);
			REG_ENUM(_index,NodeAct,BE_NODE_ACTION);
			REG_ENUM(_index,NodeView,BE_NODE_VIEW_MODE);
			REG_ENUM(_strindex,NodeType,NODE_TYPE_ON_MAP);
			REG_ENUM(_strindex,BE_vGROUPS,ALL_be_CGROUPS_ON_MAP);
			REG_AUTO(fON_NODE_SCREEN);
			REG_AUTO(NodeViewMode);
			REG_MEMBER(_bool,ND_RECT);
			REG_MEMBER(_bool,ND_XY);
			REG_MEMBER(_bool,ND_NAME);
			REG_MEMBER(_bool,ND_DESCR);
			REG_AUTO(fNodeList);
			REG_ENUM(_strindex,NODE,ALL_be_CNODES_ON_MAP);
		// "EDGE"
		SAVE_SECTION(0x00000002);
			REG_ENUM(_index,EdgeAct,BE_EDGE_ACTION);
			REG_ENUM(_index,EdgeView,BE_EDGE_VIEW_MODE);
			REG_ENUM(_strindex,BE_vGROUPS,ALL_be_CGROUPS_ON_MAP);
			REG_AUTO(EdgeViewMode);
			REG_MEMBER(_bool,EG_RECT);
			REG_MEMBER(_bool,EG_DIRECT);
			REG_MEMBER(_bool,EG_PROC);
			REG_MEMBER(_bool,EG_START);
			REG_MEMBER(_bool,EG_MODE);
		// "SCRIPT"
		SAVE_SECTION(0x00000004);
			REG_MEMBER(_int,GC_COUNT);
			REG_MEMBER(_int,GC_TRESHOLD);
			REG_MEMBER(_int,PR_TIME);
			REG_MEMBER(_bool,InitSO);
			REG_AUTO(fFnStore);
			REG_AUTO(fVarStore);
			REG_AUTO(fMAIN_EDIT);
			REG_AUTO(FilmsMenu);
			REG_AUTO(scrFILM);
			REG_AUTO(fADD_FILM);
			REG_AUTO(fDEL_FILM);
			REG_AUTO(fEDIT_FILM);
			REG_AUTO(fEDIT_GRAPH);
			REG_AUTO(fGraphList);
			REG_AUTO(ProcessMenu);
			REG_ENUM(_index,PR_MODE,BE_SCREPT_PROCESS_MODE);
			REG_MEMBER(_bool,PR_CREATE);
			REG_AUTO(fPROCESS);
			REG_ENUM(_strindex,BE_vGROUPS,ALL_be_CGROUPS_ON_MAP);
		// "SAVE"
		SAVE_SECTION(0x00000008);
			REG_MEMBER(_bool,BE_SAVE);
			REG_FILEPATH(FileForScript,".xml");
		// "vGROUPS"
		SAVE_SECTION(0x00000010);	
			REG_AUTO(fADD_GRP);
			REG_AUTO(fADD_GRP_SAMRT);
			REG_AUTO(fDEL_GRP);
			REG_AUTO(fEDIT_GRP);
			REG_AUTO(fADD_SEL);
			REG_AUTO(fON_SCREEN);
			REG_AUTO(fSHOW_CURR);
			REG_AUTO(fSHOW_ALL);
			REG_AUTO(fCREATE_DEFAULT_VG);
			REG_AUTO(fSaveGroupStruct);
			REG_AUTO(fGroupList);
			REG_MEMBER(_bool,BE_vGRP_TEST);
			REG_AUTO(UPDATE_GROUPS);
			REG_ENUM(_strindex,BE_vGROUPS,ALL_be_CGROUPS_ON_MAP);
		// "vGROUPS"
		SAVE_SECTION(0x00000020);
			REG_MEMBER(_bool,BE_vGRP_TEST);
			REG_ENUM(_strindex,BE_vGROUPS,ALL_be_CGROUPS_ON_MAP);
			REG_AUTO(fREM_N_UNT);
			REG_AUTO(fKILL_N_UNT);
			REG_AUTO(fERASE_N_UNT);
			REG_AUTO(fUN_SELECT);
			REG_AUTO(fSELECT);
			REG_AUTO(fSELECT_XYR);
			REG_AUTO(fSET_NATION);
			REG_AUTO(fSET_AGRESIV);
			REG_AUTO(fSEND_TO);
			REG_AUTO(fSET_DIR);
			REG_AUTO(fSET_FORM);
			REG_AUTO(fSET_FLAGBR);
			REG_AUTO(fCLEAR_FLAGBR);
			REG_MEMBER(_bool,GROUP_MENU_VS);
		// "EDIT"
			SAVE_SECTION(0x00000200);
			REG_ENUM(_index,EditOperation,BE_EDIT_TYPE_LIST);
			REG_AUTO(fCOPY_EDGE);
			REG_AUTO(fSELECT_SQUAD);
			REG_AUTO(fCOPY_SQUAD);
			REG_AUTO(fMESSGES_MGR);
			REG_AUTO(fADD_MESSGES);
			REG_AUTO(fFORM_SQUAD);
			REG_AUTO(fWRITE_LOG);
			REG_AUTO(fSkirmisTaskEditor);
			REG_AUTO(fSkirmisTaskAdd);
			REG_AUTO(fSkirmisTaskDel);
			REG_AUTO(fBE_SHOW_PROMF);
			REG_AUTO(fMultyAnimateStoreEditor);
			REG_AUTO(fINIT_BEMainInterface);
			REG_ENUM(_strindex,BE_vGROUPS,ALL_be_CGROUPS_ON_MAP);
			REG_AUTO(REM_LAST_ACTION);
			REG_AUTO(DEL_ALL_ACTION);
		// "vVALUES"
		SAVE_SECTION(0x00000040);
			REG_AUTO(fvVALUES_EDIT);
			REG_AUTO(fCameraPoss);
			REG_AUTO(fBattleHendler);
			REG_AUTO(SelectViewMask);
		// "Settings"
		SAVE_SECTION(0x00000080);
			REG_MEMBER(_int,UpdateDelta);
			REG_ENUM(_index,LockType,LOCKTYPE);
			REG_MEMBER(_bool,ShowHTT);
			REG_MEMBER(_bool,ShowLinks);
			REG_MEMBER(_bool,NOT_ZERO_ANLY);
			REG_MEMBER(_bool,ShowEXP);
			REG_MEMBER(_bool,ShowAST);
			REG_MEMBER(_bool,ShowVSB);
			REG_MEMBER(_int,TRIADA_NUM);
			REG_MEMBER(_bool,ShowID);
			REG_MEMBER(_bool,ShowXY);
			REG_MEMBER(_bool,Showxy);
			REG_MEMBER(_bool,DrawStoreWAY);
			REG_MEMBER(_int,sAreaID);
			REG_MEMBER(_int,dAreaID);
			REG_AUTO(HTT_UPDATE);
			REG_AUTO(HTT_AST_ARRAY);
			REG_AUTO(HTT_AST_CREATE_WAY);
			REG_AUTO(fMISS_SET);
			REG_AUTO(fEDIT_WCP);
			REG_AUTO(ProcessMenu);
				REG_MEMBER(_int,MainEditType);
			REG_AUTO(ColorSettings);
				STATIC REG_MEMBER_EX(_color,EngSettings.MIS_SET.vGroupNameColor,GroupNameColor);
				STATIC REG_MEMBER_EX(_color,EngSettings.MIS_SET.vGroupLineColor,GroupLineColor);
	//		REG_AUTO(EdgeSetMenu);
	//			REG_MEMBER(_color,EdgeNameColor);
	//			REG_MEMBER(_color,EdgeLineColor);
				REG_AUTO(D0);
				STATIC REG_MEMBER_EX(_color,EngSettings.MIS_SET.NodeNameColor,NodeNameColor);
				STATIC REG_MEMBER_EX(_color,EngSettings.MIS_SET.NodeLineColor,NodeLineColor);
				STATIC REG_MEMBER_EX(_color,EngSettings.MIS_SET.NodeIDColor,NodeIDColor);
				REG_ENUM(_index,NodeStyle,BE_SETTINGS_NODE_STYLE_LIST);
				REG_AUTO(D1);
				REG_AUTO(SaveSettings);
			REG_AUTO(SOMenu);
				REG_ENUM(_strindex,SmartObject,BE_SMARTOBJECT_ENUM);
		// "TIMER"
		SAVE_SECTION(0x00000400);
			REG_AUTO(TimerData);
			REG_AUTO(SHOW_TempUID_TEST);
			REG_AUTO(SEND_MSG);
			REG_AUTO(MOVE_VAR_TO_NEW_VAR);
			REG_AUTO(Show_Cloud_List);
			REG_AUTO(ShowCloudDialog);
			REG_AUTO(Show_Info_Dialog);
			REG_AUTO(EditFSM_List);
		// CAMERA
		SAVE_SECTION(0x00000800);
			REG_AUTO(LoadCamera);
			REG_AUTO(SaveCamera);
			REG_AUTO(Camera);
		// "NOTHING"
		SAVE_SECTION(0x80000000);
	ENDSAVE;
};

inline CBEDriveMode_ST* DriveMode(){
	if (CBEDriveMode_ST::GetObj()==NULL){
		CBEDriveMode_ST::SetObj(new CBEDriveMode_ST);

		BaseClass* pDriveMode = dynamic_cast<BaseClass*>( CBEDriveMode_ST::GetObj() );
		pDriveMode->RegisterMembers();
		ReplaceEditor("BattleEditor",pDriveMode);
	};
	return (CBEDriveMode_ST*)CBEDriveMode_ST::GetObj();
};

//////////////////////////////////////////////////////////////////////////


void BE_InstallExtension();
//////////////////////////////////////////////////////////////////////////

// ProcessBattleEditor ///////////////////////////////////////////////////
void	ProcessBattleEditor();
//////////////////////////////////////////////////////////////////////////

// BE_PROCESS_SCREPT /////////////////////////////////////////////////////
void	BE_PROCESS_SCREPT();
//////////////////////////////////////////////////////////////////////////

// BE_HandlerMouse ///////////////////////////////////////////////////////
bool	BE_MouseInEditor();
bool	BE_HandlerMouse();
//////////////////////////////////////////////////////////////////////////

// BE_EDIT_CLASS /////////////////////////////////////////////////////////
void	Add_Class_To_Main_Editor(DWORD _rce_,DWORD _DILOG_EDITOR_);
//////////////////////////////////////////////////////////////////////////

#endif//__PROCESS_BATTLE_EDITOR__
//////////////////////////////////////////////////////////////////////////

































