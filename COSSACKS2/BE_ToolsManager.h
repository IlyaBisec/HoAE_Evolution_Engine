#ifndef	__BE__TOOLS__MANAGER__ 
#define	__BE__TOOLS__MANAGER__

class be_CBASE_TOOL : public BaseClass
{
public:
	virtual bool fACTION(SimpleDialog* pSD, int _aid) = 0;
};

class be_CACTION : public BaseClass
{
public:
	be_CACTION(){};
	be_CACTION(be_CBASE_TOOL* _pROOT,int _ActionID) { pROOT=_pROOT; ActionID=_ActionID; };
	be_CBASE_TOOL*	pROOT;		// Root object for call function : fACTION(int _aid);
	int				ActionID;	// Local action ID (universal for pROOT)
	inline bool		operator()(SimpleDialog* pSD){ return ( ( pROOT!=NULL ) ? ( pROOT->fACTION( pSD, ActionID ) ) : (false) ); };	
};

bool g_beACTION(SimpleDialog* SD);

class be_CTOOL_FILD : public be_CBASE_TOOL
{
public:
	be_CTOOL_FILD(){ _delete=false; };
	~be_CTOOL_FILD(){};

	enum FILD_TYPE
	{
		FOLDER = 0,
		OBJECT = 1
	}TYPE;
	_str	TEXT;		// text
	bool	VISIBLE;	// Visibility
	int		DEEP;		// N of deep
	enum	FILD_STATUS
	{
		OPEN  = 0,	
		CLOSE = 1
	}STAUS;
	TextButton*		pTB_OBJECT;	// Represent in tree

	void	UPDATE(int _id);
	void	updateICON_TEXT();
	bool	fACTION(SimpleDialog* pSD,int _aid){ return false; };

	bool	_delete;
};

class be_CTOOL_TREE : public be_CBASE_TOOL
{
public:
	be_CTOOL_TREE();
	~be_CTOOL_TREE();

	// System
	void	CREATE();
    void	UPDATE();
	void	UPDATE(int _first_id,int _last_id);
	void	DRAW();

	bool	Visible;
	
	// Actions
	enum	ACTION_TYPE
	{
		ADD_FOLDER = 0,
		SEL_FOLDER = 1,
		DEL_FOLDER = 2
	};
	bool	fACTION(SimpleDialog* pSD,int _aid);
protected:
	bool						loaded;
	DialogsSystem				MENU;
	DialogsDesk*				pTREE_VIEW;		// Tree		panel
	DialogsDesk*				pTREE_TOOLS;	// Actions	panel
	ClonesArray<be_CTOOL_FILD>	TREE;
	void	UPDATE_TREE_XY();
	int		getLastVisiblePosY();
	int		OPEN_FOLDER(int _fid);
	void	DELETE_ELEM(int _fid);
	void	DELETE_MARK_EL();

	ClonesArray<SimpleDialog>	SelectedElList;	// List of selected elements
	void	RddRemSelect(SimpleDialog* pSD);
	int		getTREE_EL_ID(SimpleDialog* pSD);
	void	DRAW_SELECTIONS();

	bool	CREATE_MUNU();

	ClonesArray<be_CACTION>		ActionList;
	be_CACTION*	getACTION_byID(int _aid);
	bool	CREATE_ACTION(SimpleDialog* _pParentSD,char* _pElemName,int _aid);
	void	CREATE_ACTION_LIST();

	bool	AddFolder();
	bool	SelectElement(SimpleDialog* pSD);
	bool	DelSelected();
};

// TEMP TEST
void	g_beDrawGroupMenu();
void	g_beDrawTree();

//////////////////////////////////////////////////////////////////////////
// Battle Editor Menu (new edition 9.01.2006)
//////////////////////////////////////////////////////////////////////////

//*********************************************************************//
//*****************   BASE CLASS FOR DIALOSG **************************//
//*********************************************************************//
class	be_CBaseDialog	:public BaseClass
{
public:
	be_CBaseDialog();
	~be_CBaseDialog();

	virtual	bool	OnRender();
	virtual	void	OnInit(char* fileName);
	virtual	void	OnInit()	= 0;
	virtual	bool	mouseOn();

	// Helper Interface
			void	VScroll_ToElement(int _id,SimpleDialog* pSD);
			void	updateClassEditor(DialogsDesk* pDD);

protected:
	bool			loadDS(char* fileName);
	DialogsSystem	_mainDS;
	bool			_init;
	bool			_error;

	virtual	bool	mouseOnDD(DialogsDesk* pDD);
};

//*********************************************************************//
//*******************   NODE ARRAY MANAGER   **************************//
//*********************************************************************//
class	be_CNodeArrayDriver
{
public:
	static	bool	Process(
							int mx,
							int my,
							bool& LeftPressed,
							bool& RightPressed,
							int MapCoordX,
							int MapCoordY,
							bool OverMiniMap
						   );
	static	void	clear();
	static	void	GetObjectGlobalID();
	static	void	setVisible(bool _state);
	static	void	OnInit();
};

//*********************************************************************//
//**********************   NODES MANAGER   ****************************//
//*********************************************************************//
class	be_CNodesDriver
{
public:
	static	bool	Process(
							int mx,
							int my,
							bool& LeftPressed,
							bool& RightPressed,
							int MapCoordX,
							int MapCoordY,
							bool OverMiniMap
						   );
	static	void	clear();
	static	void	GetObjectGlobalID();
	static	void	setVisible(bool _state);
	static	bool	getVisible();
	static	void	OnInit();
};

//*********************************************************************//
//*********************   GROUPS MANAGER   ****************************//
//*********************************************************************//
class	be_CGroupsDriver
{
public:
	static	bool	Process(
							int mx,
							int my,
							bool& LeftPressed,
							bool& RightPressed,
							int MapCoordX,
							int MapCoordY,
							bool OverMiniMap
						   );
	static	void	clear();
	static	void	GetObjectGlobalID();
	static	void	setVisible(bool _state);
	static	bool	getVisible();
	static	void	OnInit();
};
//*********************************************************************//
//*********************   SUBFN MANAGER   *****************************//
//*********************************************************************//
class	be_CSubfunctionDriver
{
public:
	static	bool	Process(
							int mx,
							int my,
							bool& LeftPressed,
							bool& RightPressed,
							int MapCoordX,
							int MapCoordY,
							bool OverMiniMap
						   );
	static	void	clear();
	static	void	GetObjectGlobalID();
	static	void	setVisible(bool _state);
	static	bool	getVisible();
	static	void	OnInit();
};
//*********************************************************************//
//**********************   LEGO MANAGER   *****************************//
//*********************************************************************//
class	be_CLegoDriver
{
public:
	static	bool	Process(
							int mx,
							int my,
							bool& LeftPressed,
							bool& RightPressed,
							int MapCoordX,
							int MapCoordY,
							bool OverMiniMap
						   );
	static	void	clear();
	static	void	GetObjectGlobalID();
	static	void	setVisible(bool _state);
	static	bool	getVisible();
	static	void	OnInit();
};

void	be_REG_CLASS_NodesArray();
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


#endif//__BE__TOOLS__MANAGER__
//////////////////////////////////////////////////////////////////////////








































