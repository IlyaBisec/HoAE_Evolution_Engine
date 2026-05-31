//*********************************************************************//
//*******************   NODE ARRAY MANAGER   **************************//
//*********************************************************************//
enum	NODEARRAY_STATE
{
	__dsVIEWER,
	__dsEDITOR
};
enum	NODEARRAY_ACTIONS
{
	__naFREE,
	__naADD,
	__naDEL,
	__naMOVE
};
class be_CNodeArrayDialog	: public be_CBaseDialog
{
public:
	friend	class be_CNodeArray;

	be_CNodeArrayDialog();
	~be_CNodeArrayDialog();

	virtual	bool	OnRender();
	virtual	void	OnInit();
			void	updateClassEditor();
	virtual	bool	mouseOn();
protected:
	NODEARRAY_STATE	_state;
	DialogsDesk*	pDD_ADV;
	DialogsDesk*	pDD_MAIN;
};

class	be_CArrayLine  : public BaseClass
{
public:
	be_CArrayLine();
	be_CArrayLine(char* _name,bool _state=false);
	~be_CArrayLine();
	_str	_arrayName;
	bool	_visible;
	void	OnRender();
	SAVE(be_CArrayLine);
		REG_AUTO(_arrayName);
		REG_MEMBER(_bool,_visible);
	ENDSAVE;
};

class	be_CNodeArray : public ReferableBaseClass
{
public:
	void	init(char* _name=NULL);
	be_CNodeArray();
	be_CNodeArray(char* _name);
	void	clear();
	~be_CNodeArray();

	virtual bool CheckIfObjectIsGlobal() { return true; };

	be_CNodeArrayDialog		_dialog;
	bool					Process(
										int mx,
										int my,
										bool& LeftPressed,
										bool& RightPressed,
										int MapCoordX,
										int MapCoordY,
										bool OverMiniMap
								   );

	bool	_visible;
	void	setVisible(bool _state);
	bool	getVisible();
	void	FN_HIDE();

	// SWITCHER
	bool	_editor;
	bool	_viewer;
	void	switchMode();
	void	FN_SWITCH_MODE();

	// VIEWER
	ClonesArray<be_CArrayLine>	_arrayVList;
	void	addElementVIEWER(be_CARRAY* pARR);
	void	updateVIEWER();
	void	FN_UPDATE_VIEWER();
	void	selectArray(int _id);
	void	FN_SELECT_ARRAY();
	void	FN_GOTO_EDITOR();
	void	aplayParamMaskForAll();
	void	FN_APLAY_PARAM_MASK();
	void	convertToLocalNodes(bool _delPrivGlobalNodes=false);
	void	FN_CONVERT_TO_LOCAL();

	// EDITOR
	_str	_arrayEditName;
	bool	setArrayForEdit(int _id);
	_str	_nodeName;
	_str	_nodeR;
	bool	_autoIndex;
	bool	_addArrayName;
	void	clearArray();
	void	reorderArray();
	void	FN_CLEAR_ARRAY();
	void	FN_REORDER_ARRAY();
	bool	_nodeAdd;
	bool	_nodeDel;
	bool	_nodeMove;
	bool	_nodeFree;
	NODEARRAY_ACTIONS	getAction();
	void				setAction(NODEARRAY_ACTIONS _action=__naFREE);
	bool				processActions(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool				mouseOnDD();
	int					getNearestNodeID(int mx, int my);
	bool				processActionFree(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool				processActionAdd(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool				processActionDel(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool				processActionMove(bool& LeftPressed,int MapCoordX,int MapCoordY);
	
	// UPDATE !!!
	bool					_ADW;
	bool					_showADV;
	ClassRef<be_CARRAY>		_array;
	void	validateArray(bool _force=false);
	void	processDialogs();

	SAVE(be_CNodeArray);
		REG_BASE(ReferableBaseClass);
		REG_AUTO(Name);

		REG_MEMBER(_bool,_visible);
		REG_METHOD_EX(FN_HIDE,_HIDE);

		REG_MEMBER(_bool,_editor);
		REG_MEMBER(_bool,_viewer);
		REG_METHOD_EX(FN_SWITCH_MODE,_SWITCH_MODE);

		REG_AUTO(_arrayVList);
		REG_METHOD_EX(FN_UPDATE_VIEWER,_UPDATE_VIEWER);
		REG_METHOD_EX(FN_SELECT_ARRAY,_SELECT_ARRAY);
		REG_METHOD_EX(FN_GOTO_EDITOR,_GOTO_EDITOR);

		REG_AUTO(_arrayEditName);
		REG_AUTO(_nodeName);
		REG_AUTO(_nodeR);
		REG_MEMBER(_bool,_autoIndex);
		REG_MEMBER(_bool,_addArrayName);
		REG_METHOD_EX(FN_APLAY_PARAM_MASK,_APLAY_PARAM_MASK);
		REG_METHOD_EX(FN_CONVERT_TO_LOCAL,_CONVERT_TO_LOCAL);
		REG_MEMBER(_bool,_nodeAdd);
		REG_MEMBER(_bool,_nodeDel);
		REG_MEMBER(_bool,_nodeMove);
		REG_MEMBER(_bool,_nodeFree);

		REG_METHOD_EX(FN_CLEAR_ARRAY,_CLEAR_ARRAY);
		REG_METHOD_EX(FN_REORDER_ARRAY,_REORDER_ARRAY);

		REG_MEMBER(_bool,_ADW);
		REG_MEMBER(_bool,_showADV);
		REG_AUTO(_array);

	ENDSAVE;
};
extern	be_CNODE*		be_gActivNode;
extern	be_CNodeArray	g_beNodeArrayEditor;
//////////////////////////////////////////////////////////////////////////


//*********************************************************************//
//**********************   NODES MANAGER   ****************************//
//*********************************************************************//
enum	NODES_ACTIONS
{
	__ndFREE,
	__ndADD,
	__ndDEL,
	__ndMOVE,
	__ndSELECT
};
class	be_CNodesDialog	: public be_CBaseDialog
{
public:
	friend	class be_CNodes;

	be_CNodesDialog();
	~be_CNodesDialog();

	virtual	void	OnInit();
	virtual	bool	mouseOn();
	void	VScroll_ToElement(int _id);
protected:
	DialogsDesk*	pDD_ADV;
	DialogsDesk*	pDD_MAIN;
	SimpleDialog*	pCD_TXT;
};
class	be_CNodeLine  : public BaseClass
{
public:
	be_CNodeLine();
	be_CNodeLine(char* _name,bool _state=false);
	~be_CNodeLine();
	_str	_nodeName;
	bool	_visible;
	bool	_selected;
	void	OnRender(bool _forceDraw=false);
	SAVE(be_CNodeLine);
		REG_AUTO(_nodeName);
		REG_MEMBER(_bool,_visible);
		REG_MEMBER(_bool,_selected);
	ENDSAVE;
};
class	be_CNodes : public ReferableBaseClass
{
public:
	void	init(char* _name=NULL);
	be_CNodes();
	be_CNodes(char* _name);
	void	clear();
	~be_CNodes();

	virtual bool CheckIfObjectIsGlobal() { return true; };

	be_CNodesDialog			_dialog;
	bool					Process(
										int mx,
										int my,
										bool& LeftPressed,
										bool& RightPressed,
										int MapCoordX,
										int MapCoordY,
										bool OverMiniMap
								   );

	void	processDialogs();

	bool	_visible;
	void	setVisible(bool _state);
	bool	getVisible();
	void	FN_HIDE();

	// Visibility nodes states
	bool	_nodesDraw;
	bool	_nodesForceDraw;

	// Actions
	bool	_nodeFree;
	bool	_nodeAdd;
	bool	_nodeDel;
	bool	_nodeMove;
	bool	_nodeSelect;
	NODES_ACTIONS	getAction();
	void			setAction(NODES_ACTIONS _action=__ndFREE);
	bool			processActions(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool			mouseOnDD();
	int				getNearestNodeID(int mx, int my);
	bool			processActionFree(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool			processActionAdd(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool			processActionDel(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool			processActionMove(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool			processActionSelect(bool& LeftPressed,int MapCoordX,int MapCoordY);
	void			deleteNode(int _id);
	void			FN_DELETE();
	
		// All Nodes
	_str	_nodesNameMask;
	_str	_nodesR;
	_str	_nodesDIR;
	_str	_nodesSegFR;

	bool	_nodesAutoIndex;
	_str	_nodesLastIndex;

		// Current Node
	_str	_nodeName;
	_str	_nodeR;
	_str	_nodeDIR;
	_str	_nodeSegFR;
	_str	_nodeX;
	_str	_nodeY;
	ClassRef<be_CNODE>	_node;
	void	selectNode(int _id,bool _scrollOnIt=false);
	void	FN_SELECT_NODE();
	void	screenOnNode(int _id);
	void	FN_ON_SCREEN();
	void	updateSelectedNodeData();
	void	aplayChangesForSelectedNode();

	// Node List 
	bool	_ADV;
	void	updateADV();
	bool	_showADV;

	ClonesArray<be_CNodeLine>	_nodeList;
	void	updateNodeList();
	void	FN_UPDATE_LIST();
	bool	_visibleNodeList;
	void	aplayVisibleForAllNode();
	void	FN_APLAY_VISIBLE_LIST();

	SAVE(be_CNodes);
		REG_BASE(ReferableBaseClass);
		REG_AUTO(Name);

		REG_MEMBER(_bool,_visible);
		REG_METHOD_EX(FN_HIDE,_HIDE);

		REG_MEMBER(_bool,_ADV);
		REG_MEMBER(_bool,_showADV);

		REG_AUTO(_nodeList);
		REG_METHOD_EX(FN_UPDATE_LIST,_UPDATE_LIST);
		REG_MEMBER(_bool,_visibleNodeList);
		REG_METHOD_EX(FN_APLAY_VISIBLE_LIST,_APLAY_VISIBLE_LIST);

		// Draw states
		REG_MEMBER(_bool,_nodesDraw);
		REG_MEMBER(_bool,_nodesForceDraw);

		// Actions
		REG_MEMBER(_bool,_nodeFree);
		REG_MEMBER(_bool,_nodeAdd);
		REG_MEMBER(_bool,_nodeDel);
		REG_MEMBER(_bool,_nodeMove);
		REG_MEMBER(_bool,_nodeSelect);

		REG_METHOD_EX(FN_DELETE,_DELETE);

		REG_AUTO(_nodesNameMask);
		REG_AUTO(_nodesR);
		REG_AUTO(_nodesDIR);
		REG_AUTO(_nodesSegFR);
		REG_MEMBER(_bool,_nodesAutoIndex);
		REG_AUTO(_nodesLastIndex);

		// One Node Data	
		REG_AUTO(_nodeName);
		REG_AUTO(_nodeR);
		REG_AUTO(_nodeDIR);
		REG_AUTO(_nodeSegFR);
		REG_AUTO(_nodeX);
		REG_AUTO(_nodeY);
		REG_AUTO(_node);
		REG_METHOD_EX(FN_SELECT_NODE,_SELECT_NODE);
		REG_METHOD_EX(FN_ON_SCREEN,_ON_SCREEN);

	ENDSAVE;
};
extern	be_CNodes	g_beNodesEditor;

//*********************************************************************//
//*********************   GROUPS MANAGER   ****************************//
//*********************************************************************//
class	be_CGroupsDialog	: public be_CBaseDialog
{
public:
	friend	class be_CGroups;

	be_CGroupsDialog();
	~be_CGroupsDialog();

	void	OnInit();
	bool	mouseOn();
	void	VScroll_ToElement(int _id);
protected:
	DialogsDesk*	pDD_ADV;
	DialogsDesk*	pDD_MAIN;
	SimpleDialog*	pCD_TXT;
};
class	be_CGroupLine  : public BaseClass
{
public:
	be_CGroupLine();
	be_CGroupLine(char* _name,bool _state=false);
	~be_CGroupLine();
	_str	_groupName;
	bool	_visible;
	bool	_selected;
	void	OnRender(bool _forceDraw=false);
	SAVE(be_CGroupLine);
		REG_AUTO(_groupName);
		REG_MEMBER(_bool,_visible);
		REG_MEMBER(_bool,_selected);
	ENDSAVE;
};
class	be_CGroups : public ReferableBaseClass
{
public:
	void	init(char* _name=NULL);
	be_CGroups();
	be_CGroups(char* _name);
	void	clear();
	~be_CGroups();

	virtual bool CheckIfObjectIsGlobal() { return true; };

	be_CGroupsDialog		_dialog;
	bool					Process(
										int mx,
										int my,
										bool& LeftPressed,
										bool& RightPressed,
										int MapCoordX,
										int MapCoordY,
										bool OverMiniMap
								   );

	void	processDialogs();
	
	int		lastHotKey;
	void	ProcessHotKey();

	bool	_visible;
	void	setVisible(bool _state);
	bool	getVisible();
	void	FN_HIDE();

	// Visibility nodes states
	bool	_groupsDraw;
	bool	_groupsForceDraw;

	// Node List 
	bool	_ADV;
	void	updateADV();
	bool	_showADV;

	ClonesArray<be_CGroupLine>	_groupList;
	void	updateGroupList();
	void	FN_UPDATE_LIST();
	bool	_visibleGroupList;
	void	aplayVisibleForAllGroup();
	void	FN_APLAY_VISIBLE_LIST();

	// Manager
	_str	_nameMask;
	bool	_autoIndex;
	_str	_lastIndex;
	bool	_generateName;
	void	addNewGroup();
	void	FN_ADD_NEW_GR();

	// Curent group
	void	selectGroup(int _id,bool _scrollOnIt=false);
	void	FN_SELECT_GROUP_ID();
	ClassRef<be_CGROUP>	_group;
    _str	_nameGroup;
	void	addSelected();
	void	FN_ADD_SELECTED_UN();
	void	remSelected();
	void	FN_REM_SELECTED_UN();
	bool	_selectOnMap;

    void	aplayChangesForSelectedGroup();
	bool	processActions(bool& LeftPressed,int MapCoordX,int MapCoordY);
	bool	mouseOnDD();
	bool	processActionSelect(bool& LeftPressed,int MapCoordX,int MapCoordY);

	void	delGroup(int _id);
	void	FN_DEL_GR();
	void	clearGroup();
	void	FN_CLEAR_GR();
	void	groupOnScreen(int _id=-1);
	void	FN_ON_SCREEN_GR_ID();
	void	FN_ON_SCREEN_GR();

	SAVE(be_CGroups);
		REG_BASE(ReferableBaseClass);
		REG_AUTO(Name);

		REG_MEMBER(_bool,_visible);
		REG_METHOD_EX(FN_HIDE,_HIDE);

		// Draw states
		REG_MEMBER(_bool,_groupsDraw);
		REG_MEMBER(_bool,_groupsForceDraw);

		REG_MEMBER(_bool,_ADV);
		REG_MEMBER(_bool,_showADV);

		REG_AUTO(_nameMask);
		REG_MEMBER(_bool,_autoIndex);
		REG_AUTO(_lastIndex);
		REG_MEMBER(_bool,_generateName);
		REG_METHOD_EX(FN_ADD_NEW_GR,_ADD_NEW_GR);
		
		REG_METHOD_EX(FN_SELECT_GROUP_ID,_SELECT_GROUP_ID);
		REG_AUTO(_nameGroup);
		REG_METHOD_EX(FN_ADD_SELECTED_UN,_ADD_SELECTED_UN);
		REG_METHOD_EX(FN_REM_SELECTED_UN,_REM_SELECTED_UN);

		REG_AUTO(_groupList);
		REG_METHOD_EX(FN_UPDATE_LIST,_UPDATE_LIST);
		REG_MEMBER(_bool,_visibleGroupList);
		REG_METHOD_EX(FN_APLAY_VISIBLE_LIST,_APLAY_VISIBLE_LIST);

		REG_MEMBER(_bool,_selectOnMap);
		REG_METHOD_EX(FN_DEL_GR,_DEL_GR);
		REG_METHOD_EX(FN_CLEAR_GR,_CLEAR_GR);
		REG_METHOD_EX(FN_ON_SCREEN_GR_ID,_ON_SCREEN_GR_ID);
		REG_METHOD_EX(FN_ON_SCREEN_GR,_ON_SCREEN_GR);

	ENDSAVE;
};
extern	be_CGroups	g_beGroupsEditor;

//*********************************************************************//
//******************   SUB FUNCTION MANAGER   *************************//
//*********************************************************************//
class	be_CSubfunctionDialog	: public be_CBaseDialog
{
public:
	friend	class be_CSubfunction;

	be_CSubfunctionDialog();
	~be_CSubfunctionDialog();

	void	OnInit();
	bool	mouseOn();
	void	updateClassEditor();

	void*	getSelectedElementSRC();
protected:
	DialogsDesk*	pDD_PARAMS;
	DialogsDesk*	pDD_REF_INT;
	DialogsDesk*	pDD_VAL_INT;
	DialogsDesk*	pDD_REF_BOOL;
	DialogsDesk*	pDD_VAL_BOOL;
	DialogsDesk*	pDD_REF_NODE;
	DialogsDesk*	pDD_REF_GROUP;
	DialogsDesk*	pDD_REF_ARRAY;
};

class	be_CSubfunction			: public ReferableBaseClass
{
public:
	void	init(char* _name=NULL);
	be_CSubfunction();
	be_CSubfunction(char* _name);
	void	clear();
	~be_CSubfunction();

	virtual bool CheckIfObjectIsGlobal() { return true; };

	be_CSubfunctionDialog	_dialog;
	bool					Process(
										int mx,
										int my,
										bool& LeftPressed,
										bool& RightPressed,
										int MapCoordX,
										int MapCoordY,
										bool OverMiniMap
								   );

	void	processDialogs();
	void	darwMyObjects();

	bool	_visible;
	void	setVisible(bool _state);
	bool	getVisible();
	void	FN_HIDE();

	// File 
	void	FN_OPENFILE();

	// SubFunction 
	_str						subFN_Name;
	ClassRef<be_CSUBFUNCTION>	subFN_ref;
	be_CSUBFUNCTION*			subFN_ptr;
	bool	selectOnMap;
	bool	loadSubFunction(char* fileName);
	void	FN_ADD();
	bool	enterNameDD;
	void	FN_OK();
	void	FN_CANCEL();

	class	be_CSF_PARAM_WRAP : public BaseClass
	{
	public:
		be_CSF_PARAM_WRAP()							{ setParam(NULL); };
		be_CSF_PARAM_WRAP( be_CPrametrBase* pPR )	{ setParam(pPR);  };
		~be_CSF_PARAM_WRAP()						{ free();		  };
		_str							strParam;
		ClassRef<ReferableBaseClass>	refParam;
		const char* GetThisElementView(const char* LocalName){ return strParam.pchar(); };
		bool  CheckIfEditableElementIsNode(int Index,void* DataPtr,DWORD Option=7) { return false; };

		void	UpdateSTR(){
			be_CPrametrBase* pPR = (be_CPrametrBase*)(refParam.Get());
			if ( pPR==NULL ) return;
			BE_USAGE_FLAGS	oldUSAGE_FLAG = g_BE_USAGE_FLAG;
			g_BE_USAGE_FLAG = BE_IF_USE;
			strParam = pPR->GetThisElementView(NULL);
			g_BE_USAGE_FLAG = oldUSAGE_FLAG;
		}

		void	derawME(){
			be_CBasePrametr*   pPR = (be_CBasePrametr*)(refParam.Get());
			if ( pPR!=NULL ){
				be_CVariableBase* pVB = NULL;
				if ( pPR->UniveraslParam.Local ){
					pVB = (be_CVariableBase*)pPR->UniveraslParam.LocalObject.Get();
				}else{
					pVB = pPR->GetVariable();
				};
				if ( pVB!=NULL ){
					if ( strcmp( pVB->GetClassName(),"be_CNODE"  )==0 )	((be_CNODE*)pVB)->vDraw();
					else 
					if ( strcmp( pVB->GetClassName(),"be_CGROUP" )==0 )	((be_CGROUP*)pVB)->vDraw();
				};
			}
		};

	protected:
		void	free()	{ strParam.Clear(); refParam.Set(NULL); };
		void	setParam( be_CPrametrBase* pPR )
				{
					if ( pPR==NULL ) { strParam="NOPARAM"; refParam.Set(NULL); return; };
					BE_USAGE_FLAGS	oldUSAGE_FLAG = g_BE_USAGE_FLAG;
					g_BE_USAGE_FLAG = BE_IF_USE;
					strParam = pPR->GetThisElementView(NULL);
					g_BE_USAGE_FLAG = oldUSAGE_FLAG;
					refParam.Set(pPR);	
				};
	public:
		SAVE(be_CSF_PARAM_WRAP);
		ENDSAVE;
	};

	class be_CSF_PARAM_LIST	: public ClonesArray< be_CSF_PARAM_WRAP >
	{
	public:
		be_CSF_PARAM_LIST()					{				};
		~be_CSF_PARAM_LIST()				{ Clear();		};
		virtual bool ForceSimplification()	{ return true;	};
		virtual int  GetExpansionRules()	{ return 0;		};
		void	add( be_CPrametrBase* pPR ){
			Add( new be_CSF_PARAM_WRAP( pPR ) );
		};

		void	derawME(){
			for ( int i=0; i<GetAmount(); i++ )	(*this)[i]->derawME();
		};

	public:
		SAVE(be_CSF_PARAM_LIST);
		ENDSAVE;
	};
	
	be_CSF_PARAM_LIST	subFN_Params;

	// Params Data
	void*	pLastSelection;
	bool	updateSel;
	bool	paramIsLocal;
	void	setParamType();
	void	setParamFalse();
	enum	sf_PT
	{
		sf_INT		= 0,
		sf_BOOL		= 1,
		sf_NODE		= 2,
		sf_GROUP	= 3,
		sf_ARRAY	= 4,
		sf_UNKNOWN	= 99
	};
	sf_PT	getParamType();
	bool	paramIsLocalINT,	paramIsGlobalINT;
	bool	paramIsLocalBOOL,	paramIsGlobalBOOL;
	bool	paramIsLocalNODE,	paramIsGlobalNODE;
	bool	paramIsLocalGROUP,	paramIsGlobalGROUP;
	bool	paramIsLocalARRAY,	paramIsGlobalARRAY;

	bool	editLocalNODE;
	bool	editLocalGROUP;

	void	FN_CLEAR_GR();
	void	FN_ADD_SEL_GR();
	void	FN_REM_SEL_GR();

	class	be_CSF_REF_ENUM : public BaseClass
	{
	public:
		be_CSF_REF_ENUM() { myMask = 0x00000001; };
		DWORD	GetClassMask()	{ return myMask; };
		DWORD	myMask;
		virtual bool ForceSimplification()	{ return true;	};
		_str_ref	ref;
		int			intValue;
		bool		boolValue;
		be_CNODE	nodeValue;
		be_CGROUP	groupValue;
		SAVE(be_CSF_REF_ENUM);
			SAVE_SECTION(0x00000001);
			REG_ENUM(_strindex,ref ,ALL_be_CINT_ON_MAP		);
			SAVE_SECTION(0x00000002);
			REG_ENUM(_strindex,ref ,ALL_be_CBOOL_ON_MAP		);
			SAVE_SECTION(0x00000004);
			REG_ENUM(_strindex,ref ,ALL_be_CNODES_ON_MAP	);
			SAVE_SECTION(0x00000008);
			REG_ENUM(_strindex,ref ,ALL_be_CGROUPS_ON_MAP	);
			SAVE_SECTION(0x00000010);
			REG_ENUM(_strindex,ref ,ALL_be_CARRAY_ON_MAP	);
			SAVE_SECTION(0x00000020);
			REG_MEMBER(_int,intValue);
			SAVE_SECTION(0x00000040);
			REG_MEMBER(_bool,boolValue);
		ENDSAVE;
	};
	be_CSF_REF_ENUM	ref_INT;
	be_CSF_REF_ENUM	val_INT;
	be_CSF_REF_ENUM	ref_BOOL;
	be_CSF_REF_ENUM	val_BOOL;
	be_CSF_REF_ENUM	ref_NODE;
	be_CSF_REF_ENUM	val_NODE;
	be_CSF_REF_ENUM	ref_GROUP;
	be_CSF_REF_ENUM	val_GROUP;
	be_CSF_REF_ENUM	ref_ARRAY;

	void	confirmSelectedParameterData();

	SAVE(be_CSubfunction);
		REG_BASE(ReferableBaseClass);
		REG_AUTO(Name);

		REG_MEMBER(_bool,_visible);
		REG_METHOD_EX(FN_HIDE,_HIDE);
		
		REG_MEMBER(_bool,selectOnMap);

		REG_AUTO(subFN_Name);
		REG_METHOD_EX(FN_OPENFILE,_OPENFILE);
		REG_METHOD_EX(FN_ADD,_ADD);
		REG_MEMBER(_bool,enterNameDD);
		REG_METHOD_EX(FN_OK,_OK);
		REG_METHOD_EX(FN_CANCEL,_CANCEL);

		REG_CLASS(be_CSF_PARAM_WRAP);
		REG_CLASS(be_CSF_PARAM_LIST);
		REG_AUTO(subFN_Params);

		REG_MEMBER(_bool,paramIsLocal);
		REG_MEMBER(_bool,paramIsLocalINT);		REG_MEMBER(_bool,paramIsGlobalINT);
		REG_MEMBER(_bool,paramIsLocalBOOL);		REG_MEMBER(_bool,paramIsGlobalBOOL);
		REG_MEMBER(_bool,paramIsLocalNODE);		REG_MEMBER(_bool,paramIsGlobalNODE);
		REG_MEMBER(_bool,paramIsLocalGROUP);	REG_MEMBER(_bool,paramIsGlobalGROUP);
		REG_MEMBER(_bool,paramIsLocalARRAY);	REG_MEMBER(_bool,paramIsGlobalARRAY);

		REG_MEMBER(_bool,editLocalNODE);
		REG_MEMBER(_bool,editLocalGROUP);

		REG_METHOD_EX(FN_CLEAR_GR	, _CLEAR_GR	  );
		REG_METHOD_EX(FN_ADD_SEL_GR	, _ADD_SEL_GR );
		REG_METHOD_EX(FN_REM_SEL_GR	, _REM_SEL_GR );

		REG_CLASS( be_CSF_REF_ENUM );
		REG_AUTO( ref_INT	);
		REG_AUTO( val_INT	);
		REG_AUTO( ref_BOOL	);
		REG_AUTO( val_BOOL	);
		REG_AUTO( ref_NODE	);
		REG_AUTO( ref_GROUP	);
		REG_AUTO( ref_ARRAY	);

		REG_METHOD_EX(confirmSelectedParameterData,_CONFIRM);

	ENDSAVE;
};
extern	be_CSubfunction	g_beSubfunctionEditor;
//*********************************************************************//
//**********************   LEGO MANAGER   *****************************//
//*********************************************************************//
class	be_CLegoDialog	: public be_CBaseDialog
{
public:
	friend	class be_CLego;

	be_CLegoDialog();
	~be_CLegoDialog();

	void	OnInit();
};
class be_CLego : public ReferableBaseClass
{
public:
	be_CLego();
	be_CLego(char* _name);
	~be_CLego();

	virtual bool CheckIfObjectIsGlobal() { return true; };

	be_CLegoDialog	_dialog;
	bool			_visible;
	void			FN_HIDE();

	bool			Process(
								int mx,
								int my,
								bool& LeftPressed,
								bool& RightPressed,
								int MapCoordX,
								int MapCoordY,
								bool OverMiniMap
							);

	bool			_active;
	void	processDialogs();

	// ACTIONS
	void	FN_DEBUG();
	void	FN_GROUP();
	void	FN_NODE();
	void	FN_ARRAY();
	void	FN_SUBFN();

	SAVE(be_CLego);
		REG_BASE(ReferableBaseClass);
		REG_AUTO(Name);		

		REG_MEMBER(_bool,_visible);
		REG_MEMBER(_bool,_active);

		REG_METHOD_EX(FN_HIDE,_HIDE);

		REG_METHOD_EX(FN_DEBUG,_DEBUG);
		REG_METHOD_EX(FN_GROUP,_GROUP);
		REG_METHOD_EX(FN_NODE,_NODE);
		REG_METHOD_EX(FN_ARRAY,_ARRAY);
		REG_METHOD_EX(FN_SUBFN,_SUBFN);

	ENDSAVE;
};
extern	be_CLego	g_beLego;
//////////////////////////////////////////////////////////////////////////























