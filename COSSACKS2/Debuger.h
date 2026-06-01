#ifndef	__BE_DEBUGER__ 
#define	__BE_DEBUGER__ 

enum	DEBUGER_STATE
{
	__dsDEBUGER,
	__dsVALUE,
	__dsFUNCTION,
	__dsWATCH,
	__dsSMARTBP
};
class be_CDebugerDialog	: public BaseClass
{
public:
	friend	class be_CDebuger;

	be_CDebugerDialog();
	~be_CDebugerDialog();

	bool	OnRender();
	void	OnInit();

	SimpleDialog*	pCD_TXT;
	void	V_scrollTXT_ToElement(int _id);

	DialogsDesk*	pDD_SRC;
	DialogsDesk*	pDD_TXT;
	void*			getSelectedElementSRC();
	void			setSelectedElementSRC(void* prt);
	void			updateClassEditor();
	void			scrollSRC_ToSelectedElement();

	DialogsDesk*	pDD_WATCH;
	void			setWatchVisibility(bool _state);
	DialogsDesk*	pDD_SMARTBP;
	void			setSmartBPVisibility(bool _state);
	DialogsDesk*	pDD_FILE;

	DEBUGER_STATE	_state;
protected:
	bool			loadDS();
	DialogsSystem	_mainDS;
	bool			_init;
	bool			_error;
};

class be_CDebugerLine : public BaseClass
{
public:
	be_CDebugerLine();
	be_CDebugerLine(char* _text);
	be_CDebugerLine(char* _text,be_CFunctionBase* pFN);
	void	init(char* _text=NULL);
	_str	_text;
	//... Some data for debuger
	bool	_selected;
	bool	_breakPoint;
	void	switchBreakPoint();
	be_CFunctionBase*	pFN;		// function pointer
	SAVE(be_CDebugerLine);
		REG_AUTO(_text);
		REG_MEMBER(_bool,_selected);
		REG_MEMBER(_bool,_breakPoint);
	ENDSAVE;
};

class be_CDebugerLineList : public ClonesArray<be_CDebugerLine>
{
	SAVE(be_CDebugerLineList);
	ENDSAVE;
};
class be_CWatchLine	: public BaseClass
{
public:
	be_CWatchLine();
	~be_CWatchLine();
	ClassPtr<be_CFunctionBase>	_FN;
	_str						_TXT;
	_str						_RET;
	void	update();
	SAVE(be_CWatchLine);
		REG_AUTO(_FN);
		INVISIBLE REG_AUTO(_TXT);
		INVISIBLE REG_AUTO(_RET);
	ENDSAVE;
};
class be_CWatchLineList	: public ClonesArray<be_CWatchLine>
{
public:
	virtual bool ForceSimplification(){ return true; }
			void update();
	SAVE(be_CWatchLineList);
	ENDSAVE;
};
class be_CSmartBPLine	: public BaseClass
{
public:
	be_CSmartBPLine();
	~be_CSmartBPLine();
	ClassPtr<be_CFunctionBase>	_CONDITION;
	_str						_TXT;
	bool						_active;
	bool						_breakON;
	bool	check(bool _callConditions=true);
	SAVE(be_CSmartBPLine);
		REG_AUTO(_CONDITION);
		INVISIBLE REG_AUTO(_TXT);
		INVISIBLE REG_MEMBER(_bool,_active);
		INVISIBLE REG_MEMBER(_bool,_breakON);
	ENDSAVE;
};
class be_CSmartBPLineList : public ClonesArray<be_CSmartBPLine>
{
public:
	virtual bool ForceSimplification(){ return true; }
			bool check(bool _callConditions=true);
	SAVE(be_CSmartBPLineList);
	ENDSAVE;
};
class be_CDebuger : public ReferableBaseClass
{
public:
	~be_CDebuger();
	void	clear();
	be_CDebuger();	
	be_CDebuger(char* name);	
	void	init(char* name=NULL);

	virtual bool CheckIfObjectIsGlobal() { return true; };

	bool	_active;
	bool	_stepByStepState;
	void	Process();

	int		lastHotKey;
	void	ProcessHotKey();

	bool	_visible;
	bool	getVisible();
	void	setVisible(bool state);	
	void	FN_SWITCH_VISIBLE();

	bool	_SCRIPT_TXT;
	bool	_SCRIPT_SRC;
	void	switchTXTvsSRC();
	void	FN_SCRIPT_SWICHER();

	be_CDebugerDialog	_dialog;

	ClassRef<be_CFunctionBase>		_curSRC;

	be_CDebugerLineList	_SCRIPT_TXT_LIST;
	int		_pos;
	void	FillScriptTxtList(be_CFunctionBase* pSCRIPT);
	void	FillScriptTxtLine(be_CFunctionBase* pSCRIPT,int _spaceN=0);
	void	FillBoderLines(_str& _className,_str& _mod,_str& _line1,_str& _line2,_str& _line3);
	bool	_updateAlways;
	void	FN_UPDATE_TXT();
	void	FN_UPDATE_TXT_SEL();

	// Navigation
	int		_selected_id;
	void	clearSelection();
	void	setSelection(int _id,bool scrollOnIT=false);
	void	FN_MOVE_SELECTOR();

	// Finde
	_str	_strSearch;
	void	FN_SEARCH_STR_SWITCH_ACTIVITY();
	bool	_strSearchActive;
	class SD_SEARCH_RESULTS
	{
	public:
		void	clear();
		_str					_string;		// Searched string
		LinearArray<int,_int>	_lines_id;		// Searched positions
		int						_iterN;			// Iterations
		void	search(_str& _pstr);
	private:
		int		getNextLineID();
	}_SEARCH_RESULTS;
	void	FN_SEARCH();
	int		findeElementByPtr(BaseClass* pELEM);

	// Debuger
	void	switchBreakPoint(int _id);
	void	openSelectedLineInSRC();
	void	FN_RIGHT_CLICK();

	void	callFunction(int _id);
	bool	_nextCall;
	void	FN_NEXT_CALL();
	bool	_interrupt;
	void	FN_INTERRUPT();
	bool	_smartBP;
	be_CSmartBPLineList	_SMARTBP_LIST;
	void	deleteSmartBP(int _id);
	void	FN_DELETE_SMARTBP();
	void	FN_ADD_EDIT_SMARTBP();

	// ADIONAL
	void	FN_VARIABLES();
	void	FN_FUNCTIONS();

	bool	_watch;
	be_CWatchLineList	_WATCH_LIST;
	void	deleteWatch(int _id);
	void	FN_DELETE_WATCH();
	void	FN_ADD_EDIT_WATCH();

	// Svae/Load Scrept
	_str	_FILE_NAME;
	void	saveSCRIPT();
	void	loadSCRIPT();
	void	FN_FILE_DIALOG();
	void	FN_SAVE();
	void	FN_LOAD();

	SAVE(be_CDebuger);
		REG_BASE(ReferableBaseClass);
		INVISIBLE REG_AUTO(Name);
		
		NOSAVE	REG_MEMBER(_bool,_active);
		REG_MEMBER(_bool,_visible);
		REG_METHOD_EX(&be_CDebuger::FN_SWITCH_VISIBLE,_SWITCH_VISIBLE);

		REG_AUTO(_curSRC);

		REG_MEMBER(_bool,_SCRIPT_TXT);
		REG_MEMBER(_bool,_SCRIPT_SRC);
		REG_METHOD_EX(&be_CDebuger::FN_SCRIPT_SWICHER,_SCRIPT_SWICHER);
		REG_MEMBER(_bool,_updateAlways);

		REG_AUTO(_SCRIPT_TXT_LIST);
		REG_METHOD_EX(&be_CDebuger::FN_UPDATE_TXT,_UPDATE_TXT);
		REG_METHOD_EX(&be_CDebuger::FN_UPDATE_TXT_SEL,_UPDATE_TXT_SEL);

		REG_METHOD_EX(&be_CDebuger::FN_MOVE_SELECTOR,_MOVE_SELECTOR);
		
		REG_AUTO(_strSearch);
		REG_METHOD_EX(&be_CDebuger::FN_SEARCH_STR_SWITCH_ACTIVITY,_SEARCH_STR_SWITCH_ACTIVITY);
		REG_MEMBER(_bool,_strSearchActive);
		REG_METHOD_EX(&be_CDebuger::FN_SEARCH,_SEARCH);

		REG_METHOD_EX(&be_CDebuger::FN_RIGHT_CLICK,_RIGHT_CLICK);
		NOSAVE	REG_MEMBER(_bool,_nextCall);
		REG_METHOD_EX(&be_CDebuger::FN_NEXT_CALL,_NEXT_CALL);
		NOSAVE	REG_MEMBER(_bool,_interrupt);
		REG_METHOD_EX(&be_CDebuger::FN_INTERRUPT,_INTERRUPT);
		REG_MEMBER(_bool,_smartBP);
		REG_AUTO(_SMARTBP_LIST);
		REG_METHOD_EX(&be_CDebuger::FN_DELETE_SMARTBP,_DELETE_SMARTBP);
		REG_METHOD_EX(&be_CDebuger::FN_ADD_EDIT_SMARTBP,_ADD_EDIT_SMARTBP);

		REG_METHOD_EX(&be_CDebuger::FN_VARIABLES,_VARIABLES);
		REG_METHOD_EX(&be_CDebuger::FN_FUNCTIONS,_FUNCTIONS);
		REG_MEMBER(_bool,_watch);
		REG_AUTO(_WATCH_LIST);
		REG_METHOD_EX(&be_CDebuger::FN_DELETE_WATCH,_DELETE_WATCH);
		REG_METHOD_EX(&be_CDebuger::FN_ADD_EDIT_WATCH,_ADD_EDIT_WATCH);

		REG_AUTO(_FILE_NAME);
		REG_METHOD_EX(&be_CDebuger::FN_FILE_DIALOG,_FILE_DIALOG);
		REG_METHOD_EX(&be_CDebuger::FN_SAVE,_SAVE);
		REG_METHOD_EX(&be_CDebuger::FN_LOAD,_LOAD);

	ENDSAVE;

};

extern	be_CDebuger	g_beDebuger;

void	be_REG_CLASS_Debuger();

#endif//__BE_DEBUGER__ 