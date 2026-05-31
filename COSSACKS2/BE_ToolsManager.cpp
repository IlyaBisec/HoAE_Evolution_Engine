#include "stdheader.h"
#include "BE_HEADERS.h"
#include "BE_ToolsManager.hpp"
#include "GameInterface.h"

void DrawColoredRect(int x,int y,int Lx,int Ly,DWORD Diffuse,int Fade);

DLLEXPORT void	ClearSelection(byte Nat);

extern bool g_fWheelIsBlocked;

bool g_beACTION(SimpleDialog* SD){
	return ( ( SD!=NULL && SD->UserParam!=0 ) ? ( (*((be_CACTION*)(SD->UserParam)))(SD) ) : (false) );	
};

// be_CTOOL_FILD /////////////////////////////////////////////////////////
void	be_CTOOL_FILD::updateICON_TEXT(){
	_str ICON_TEXT;
	switch(TYPE) {
	case FOLDER:
		switch(STAUS) {
		case OPEN:
			ICON_TEXT =  "{ITM}";
			break;
		case CLOSE:
			ICON_TEXT =  "{ITP}";
			break;
		};
		ICON_TEXT += " {ITF}";
		break;
	case OBJECT:
		ICON_TEXT += "{ITFG}";
		break;
	};
	ICON_TEXT += ( (g_beCheckChar(TEXT.pchar())) ? (TEXT) : ("No Name") );
	pTB_OBJECT->SetMessage( ICON_TEXT.pchar() );
};
void	be_CTOOL_FILD::UPDATE(int _id){
	if ( VISIBLE==true && pTB_OBJECT!=NULL ) {
		pTB_OBJECT->Sety(  _id * ( pTB_OBJECT->GetHeight() + 4 ) );
		pTB_OBJECT->Setx( DEEP * 20 );
		updateICON_TEXT();
	};
};
// be_CTOOL_TREE /////////////////////////////////////////////////////////
be_CTOOL_TREE::be_CTOOL_TREE(){
	loaded	= false;	
	Visible	= false;
	pTREE_VIEW	= NULL;
	pTREE_TOOLS = NULL;
};
be_CTOOL_TREE::~be_CTOOL_TREE(){
	
};
void	be_CTOOL_TREE::CREATE(){
	CREATE_MUNU();
	CREATE_ACTION_LIST();
};
bool	be_CTOOL_TREE::CREATE_MUNU(){
	if (loaded==false){
		loaded = true;
		xmlQuote	xmlData( "TOOL_TreeXML" ); 
		if (xmlData.ReadFromFile("Missions\\GRAPHICS\\TOOLS\\Dialog\\TTM.DialogsSystem.xml")){
			ErrorPager	Err;
			if ( MENU.Load( xmlData,&MENU,&Err ) ){
				pTREE_VIEW	= (DialogsDesk*)(MENU.DSS[0]->Find("ViewDesk"));
				pTREE_TOOLS = (DialogsDesk*)(MENU.DSS[0]->Find("ToolsDesk"));	
			};
		};
	};
	return false;
};
be_CACTION*	be_CTOOL_TREE::getACTION_byID(int _aid){
	int N = ActionList.GetAmount();
	while (N--) {
		if ( ActionList[N]->ActionID==_aid )	return ActionList[N];
	};
	return NULL;
};
bool	be_CTOOL_TREE::CREATE_ACTION(SimpleDialog* _pParentSD,char* _pElemName,int _aid){
	if ( _pParentSD==NULL || g_beCheckChar(_pElemName)==false ) return false;
	// AddFolder << OnUserClick >>
	be_CACTION* pAction = new be_CACTION((be_CBASE_TOOL*)this,_aid);
	if (pAction!=NULL){
		ActionList.Add(pAction);
		SimpleDialog* pSD = _pParentSD->Find(_pElemName);
		if ( pSD ) {
			pSD->OnUserClick	= g_beACTION;
			pSD->UserParam		= (int)(pAction);
		};
		pAction = NULL;
		return true;
	};
	return false;
};
void	be_CTOOL_TREE::CREATE_ACTION_LIST(){
	CREATE_ACTION(pTREE_TOOLS,"AddNewFolder",ADD_FOLDER);
	CREATE_ACTION(pTREE_TOOLS,"DeleteFolder",DEL_FOLDER);
	// Select Element
	be_CACTION* pAction = new be_CACTION((be_CBASE_TOOL*)this,SEL_FOLDER);
	ActionList.Add(pAction);
	pAction = NULL;
};
bool	be_CTOOL_TREE::fACTION(SimpleDialog* pSD, int _aid){
	switch(_aid) {
	case ADD_FOLDER:
		return AddFolder();
	case SEL_FOLDER:
		return SelectElement(pSD);
	case DEL_FOLDER:
		return DelSelected();
	};
	return false;
};
void	be_CTOOL_TREE::UPDATE(int _first_id,int _last_id){
	//int a_fid = ( () ? () : () );
	//int a_lid = ( () ? () : () );
	//for ( int i=a_fid; i<=a_lid; i++ ){
	//	TREE[i]->UPDATE(i);
	//};
};
void	be_CTOOL_TREE::UPDATE(){
	UPDATE(0,TREE.GetAmount()-1);
};
void	be_CTOOL_TREE::UPDATE_TREE_XY(){
	int Y = 4;
	int N = TREE.GetAmount();
	SimpleDialog* pSD = NULL;
	for ( int i=0; i<N; i++ ){
		pSD = TREE[i]->pTB_OBJECT;
		if ( pSD->Visible ){
			pSD->Sety( Y );
			pSD->Setx( TREE[i]->DEEP * 20 );
			Y += pSD->GetHeight()+4;
		};
	};
};
void	be_CTOOL_TREE::DRAW_SELECTIONS(){
    int N = SelectedElList.GetAmount();
	while (N--) {
		if ( SelectedElList[N]->Visible ){
            DrawColoredRect(	SelectedElList[N]->LastDrawX	   ,
								SelectedElList[N]->LastDrawY	-1 ,
								SelectedElList[N]->GetWidth()	+2 ,
								SelectedElList[N]->GetHeight()	+5 ,
								0xFFa6a6a6						   ,
								55
							);
		};
	};
};
int		be_CTOOL_TREE::getLastVisiblePosY(){
	int N = pTREE_VIEW->DSS.GetAmount();
	while (N--) {
		if ( pTREE_VIEW->DSS[N]->Visible ) return pTREE_VIEW->DSS[N]->Gety()+pTREE_VIEW->DSS[N]->GetHeight();
	};
	return 0;
};
void	be_CTOOL_TREE::DRAW(){
	if (Visible) {
		CREATE();
		MENU.ProcessDialogs();
		DRAW_SELECTIONS();
	};
};
int		be_CTOOL_TREE::getTREE_EL_ID(SimpleDialog* pSD){
	int N = TREE.GetAmount();
	while (N--) {
		if ( (int)(TREE[N]->pTB_OBJECT) == (int)pSD ) return N;
	};
	return -1;
};
int		be_CTOOL_TREE::OPEN_FOLDER(int _fid){
	TREE[_fid]->VISIBLE = true;
	TREE[_fid]->STAUS	= be_CTOOL_FILD::FILD_STATUS::OPEN;
	TREE[_fid]->updateICON_TEXT();

	int FN = TREE.GetAmount();
	for ( int i=_fid+1; i<FN; i++ ){
		if ( TREE[i]->DEEP == TREE[_fid]->DEEP+1 ){
			TREE[i]->VISIBLE = true;
			if ( TREE[i]->STAUS == be_CTOOL_FILD::FILD_STATUS::OPEN ) OPEN_FOLDER(i);
		};
		if ( TREE[i]->DEEP <= TREE[_fid]->DEEP ) return i;
	};
};
bool	be_CTOOL_TREE::AddFolder(){
	TextButton* pTB = new TextButton();
	be_CTOOL_FILD* pFILD  = new be_CTOOL_FILD();
	if ( pTB->ReadFromFile("Missions\\GRAPHICS\\TOOLS\\Dialog\\TTI.TextButton.Dialogs.xml") ){
		int posY = 0;
		if (SelectedElList.GetAmount()==1){
			int _elID = getTREE_EL_ID(SelectedElList[0]);
			if ( _elID!=-1 && TREE[_elID]->TYPE==be_CTOOL_FILD::FILD_TYPE::FOLDER ){
				int lastID = OPEN_FOLDER(_elID);
				UPDATE_TREE_XY();
				pFILD->DEEP = TREE[_elID]->DEEP + 1;
				pTREE_VIEW->DSS.Add(pTB);
				posY = TREE[_elID]->pTB_OBJECT->Gety()+TREE[_elID]->pTB_OBJECT->GetHeight();
				pFILD->TYPE			= be_CTOOL_FILD::FILD_TYPE::FOLDER;
				pFILD->TEXT			= "New Folder";
				pFILD->VISIBLE		= true;
				pFILD->STAUS		= be_CTOOL_FILD::FILD_STATUS::CLOSE;
				pFILD->pTB_OBJECT	= pTB;
				pTB->Setx(0);
				pTB->Sety(posY+4);
				pFILD->updateICON_TEXT();
				TREE.Insert(lastID,pFILD);
			};
		}else{
			posY = getLastVisiblePosY();
			pTREE_VIEW->DSS.Add(pTB);
			pFILD->DEEP			= 0;
			pFILD->TYPE			= be_CTOOL_FILD::FILD_TYPE::FOLDER;
			pFILD->TEXT			= "New Folder";
			pFILD->VISIBLE		= true;
			pFILD->STAUS		= be_CTOOL_FILD::FILD_STATUS::CLOSE;
			pFILD->pTB_OBJECT	= pTB;
			pTB->Setx(0);
			pTB->Sety(posY+4);
			pFILD->updateICON_TEXT();
			TREE.Add(pFILD);
		};
		
		pTB->OnUserClick	= g_beACTION;
		pTB->UserParam		= (int)(getACTION_byID(1));

		UPDATE_TREE_XY();

		return true;
	};
	delete pTB;
	delete pFILD;
	return false;
};
void	be_CTOOL_TREE::RddRemSelect(SimpleDialog* pSD){
	int N = SelectedElList.GetAmount();
	bool remove = false;
	int	 id = -1;
	while (!remove&&N--) {
		remove = ( (int)(SelectedElList[N]) == (int)pSD  );
		id = N;
	};
	if ( remove && id!=-1 ){
		SelectedElList[id] = NULL;
		SelectedElList.DelElement(id);
	}else{
		SelectedElList.Add(pSD);
	};
};
bool	be_CTOOL_TREE::SelectElement(SimpleDialog* pSD){
	if ( GetKeyState(VK_CONTROL)&8000 ){
		RddRemSelect(pSD);
		return true;
	};
	SelectedElList.DynArray<SimpleDialog*>::Clear();	
	RddRemSelect(pSD);
	return true;	
};
void	be_CTOOL_TREE::DELETE_ELEM(int _fid){
	if ( TREE[_fid]->TYPE == be_CTOOL_FILD::FILD_TYPE::OBJECT ){
		int MN = pTREE_VIEW->DSS.GetAmount();
		while (MN--) {
			if ( (int)(pTREE_VIEW->DSS[MN]) == (int)(TREE[_fid]->pTB_OBJECT) ){
				pTREE_VIEW->DSS.DelElement(MN);
				break;
			};
		};
		TREE[_fid]->pTB_OBJECT = NULL;
		TREE.DelElement(_fid);
    };
	if (  TREE[_fid]->TYPE == be_CTOOL_FILD::FILD_TYPE::FOLDER ) {
		for ( int i=_fid+1; i<TREE.GetAmount(); i++ ){
			if ( TREE[i]->DEEP-1 == TREE[_fid]->DEEP ){
				DELETE_ELEM(i);
			};
			if ( TREE[i]->DEEP <= TREE[_fid]->DEEP ) break;
		};
		TREE[_fid]->_delete = true;
	};
};
void	be_CTOOL_TREE::DELETE_MARK_EL(){
	int N = TREE.GetAmount();
	while (N--) {
		if ( TREE[N]->_delete ){
			int MN = pTREE_VIEW->DSS.GetAmount();
			while (MN--) {
				if ( (int)(pTREE_VIEW->DSS[MN]) == (int)(TREE[N]->pTB_OBJECT) ){
					pTREE_VIEW->DSS.DelElement(MN);
					break;
				};
			};
			TREE[N]->pTB_OBJECT = NULL;
			TREE.DelElement(N);
		};
	};
};
bool	be_CTOOL_TREE::DelSelected(){
	if ( SelectedElList.GetAmount()>0 ){
		int N = TREE.GetAmount();
		while (N--) {
			int SN = SelectedElList.GetAmount();
			while (SN--) {
				if ( (int)(SelectedElList[SN]) == (int)(TREE[N]->pTB_OBJECT) ){
					DELETE_ELEM(N);
					SelectedElList[SN]=NULL;
					SelectedElList.DelElement(SN);
					break;
				};
			};
		};
		DELETE_MARK_EL();
		UPDATE_TREE_XY();
		return true;
	};
	return false;	
};
//////////////////////////////////////////////////////////////////////////
void	g_beDrawTree(){
	static bool				load			= false;
	static bool				active			= false;
	static DialogsSystem	TreeDS;
	if ( DriveMode()->GROUP_MENU_VS==true && active==false ) {
		active = true;
		if ( load==false ){
			load = true;
			xmlQuote	xmlData( "TreeXML" ); 
			if (xmlData.ReadFromFile( "Missions\\GRAPHICS\\TOOLS\\Dialog\\ToolsTest.DialogsSystem.xml" )){
				ErrorPager	Err;
				TreeDS.Load( xmlData,&TreeDS,&Err );
			};
		};
		SimpleDialog* pMenu=TreeDS.DSS[0];
		pMenu->Visible = true;
	};
	if ( DriveMode()->GROUP_MENU_VS==true && active==true )	
		TreeDS.ProcessDialogs();
};

void	g_beDrawGroupMenu(){
	return;
	static bool				load			= false;
	static bool				active			= false;
	static DialogsSystem	GroupMenuDS;
	if ( DriveMode()->GROUP_MENU_VS==true && active==false ) {
		active = true;
		if ( load==false ){
			xmlQuote	xmlData( "GroupMenuXML" ); 
			if (xmlData.ReadFromFile( "Missions\\GRAPHICS\\TOOLS\\Dialog\\Group.DialogsSystem.xml" )){
				ErrorPager	Err;
				GroupMenuDS.Load( xmlData,&GroupMenuDS,&Err );
			};
		};
		SimpleDialog* pMenu=GroupMenuDS.DSS[0];
		pMenu->Visible = true;
		pMenu=GroupMenuDS.DSS[1];
		pMenu->Visible = true;
	};
	if ( DriveMode()->GROUP_MENU_VS==true && active==true )	
		GroupMenuDS.ProcessDialogs();
};

//*********************************************************************//
//*****************   BASE CLASS FOR DIALOSG **************************//
//*********************************************************************//
be_CBaseDialog::be_CBaseDialog(){
	_error = false;
	_init  = true;
};
be_CBaseDialog::~be_CBaseDialog(){
	
};
void	be_CBaseDialog::OnInit(char* fileName){
	_error = !loadDS(fileName);
	if ( !_error ){
		_init = false;
	};
};
bool	be_CBaseDialog::OnRender(){
	if ( _error ) return false;
	if ( _init ) OnInit();
	if ( !_error ){
		_mainDS.ProcessDialogs();
	};	
};
bool	be_CBaseDialog::mouseOn(){
	return false;
};
bool	be_CBaseDialog::loadDS(char* fileName){
	_mainDS.reset_class(&_mainDS);
	if ( g_beCheckChar(fileName)==false ) return false;
	return	_mainDS.ReadFromFile(fileName);
};
bool	be_CBaseDialog::mouseOnDD(DialogsDesk* pDD){
	if ( pDD && pDD->Visible ){
		int x = pDD->Getx();
		int y = pDD->Gety();
		int w = pDD->GetWidth();
		int h = pDD->GetHeight();
		return ( ( (x<=mouseX) && (mouseX<=x+w) ) && ( (y<=mouseY) && (mouseY<=y+h) ) );
	};
	return false;
};
void	be_CBaseDialog::VScroll_ToElement(int _id,SimpleDialog* pSD){
	if ( !_error && pSD!=NULL ){
		pSD->V_scrollToElement(_id);
	};	
};
void	be_CBaseDialog::updateClassEditor(DialogsDesk* pDD){
	if ( !_error && pDD!=NULL ){
		ClassEditorConnector* pCEC = pDD->UseClassEditor.Get();
		if (pCEC&&pCEC->CE) {
			BE_USAGE_FLAGS	OLD_FLAG = g_BE_USAGE_FLAG;
			g_BE_USAGE_FLAG = BE_IF_USE;
			pCEC->CE->ExpInfChanged=true;
			pCEC->CE->Process();
			g_BE_USAGE_FLAG = OLD_FLAG;	
		};
	};		
};
//*********************************************************************//
//*******************   NODE ARRAY MANAGER   **************************//
//*********************************************************************//
be_CNODE*		be_gActivNode = NULL;
//////////////////////////////////////////////////////////////////////////
// be_CNodeArrayDialog	//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
be_CNodeArrayDialog::be_CNodeArrayDialog(){
	_state = __dsVIEWER;
	pDD_ADV = NULL;
	pDD_MAIN = NULL;
};
be_CNodeArrayDialog::~be_CNodeArrayDialog(){
	
};
bool	be_CNodeArrayDialog::OnRender(){
	if ( _error ) return false;
	if ( _init ) OnInit();
	if ( !_error ){
		
		BE_USAGE_FLAGS	OLD_FLAG = g_BE_USAGE_FLAG;
		g_BE_USAGE_FLAG = BE_IF_USE;

		_mainDS.ProcessDialogs();

		g_BE_USAGE_FLAG = OLD_FLAG;			

		switch(_state) {
		case __dsVIEWER:
			{
				
			};
			break;
		case __dsEDITOR:
			{
				
			};
			break;
		};
	};
};
void	be_CNodeArrayDialog::OnInit(){
	be_CBaseDialog::OnInit("Missions\\GRAPHICS\\TOOLS\\Dialog\\NAE.DialogsSystem.xml");
	if ( !_error ){
		pDD_ADV = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_ADV") );
		pDD_MAIN = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_MAIN") );
	};
};
void	be_CNodeArrayDialog::updateClassEditor(){
	be_CBaseDialog::updateClassEditor(pDD_ADV);
};
bool	be_CNodeArrayDialog::mouseOn(){
	return 	mouseOnDD(pDD_ADV)||mouseOnDD(pDD_MAIN);
};
//////////////////////////////////////////////////////////////////////////
// be_CArrayLine		//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
be_CArrayLine::be_CArrayLine(){
	_arrayName = "NONAME";
	_visible = false;
};
be_CArrayLine::be_CArrayLine(char* _name,bool _state){
	_arrayName = ( (_name!=NULL&&_name[0]!=0) ? (_name) : ("NONAME") );
	_visible = false;
};
be_CArrayLine::~be_CArrayLine(){
	_arrayName.Clear();	
};
void	be_CArrayLine::OnRender(){
	if ( _visible==false ) return;
	be_CARRAY* pARR = g_beVAR_STORE.ARRAY_LIST.GetByName(_arrayName.pchar());
	if (pARR!=NULL) pARR->vDraw();
};
//////////////////////////////////////////////////////////////////////////
// be_CNodeArray		//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
be_CNodeArray	g_beNodeArrayEditor("NodeArrayEditor");

void	be_CNodeArray::init(char* _name/*=NULL*/){
	Name = ( (_name!=NULL&&_name[0]!=0) ? (_name) : ("NodeArrayEditorExemple") );
	_editor = false;
	_viewer = true;
	_nodeName = "NONAME";
	_nodeR = 0;
	_ADW = false;
	_showADV = (_visible&&_ADW);
	_array.Set(NULL);

	_autoIndex = false;
	_addArrayName = false;

	_nodeAdd = false;
	_nodeDel = false;
	_nodeMove = false;
	_nodeFree = true;
};
be_CNodeArray::be_CNodeArray(){
	init();
};
be_CNodeArray::be_CNodeArray(char* _name){
	init(_name);
	GetObjectGlobalID();
};
void	be_CNodeArray::clear(){
	_arrayVList.Clear();
};
be_CNodeArray::~be_CNodeArray(){
	clear();
};
bool	be_CNodeArray::Process(
								int mx,
								int my,
								bool& LeftPressed,
								bool& RightPressed,
								int MapCoordX,
								int MapCoordY,
								bool OverMiniMap
							  )
{
	processDialogs();
	_dialog.OnRender();	
	rsFlush();
	for ( int i=0; i<_arrayVList.GetAmount(); i++ ) (_arrayVList[i])->OnRender();
	rsFlush();
	g_fWheelIsBlocked=true;
	if ( _visible==false || _nodeMove==false ) g_fWheelIsBlocked=false;
	if (_visible==true) return processActions(LeftPressed,MapCoordX,MapCoordY);
	return false;
};
void	be_CNodeArray::setVisible(bool _state){
	_visible = _state;
	if ( _visible==false ){
		if ( be_gActivNode!=NULL ) be_gActivNode=NULL;
		setAction(__naFREE);
	};
};
bool	be_CNodeArray::getVisible(){
	return _visible;
};
void	be_CNodeArray::switchMode(){
	switch(_dialog._state) {
	case __dsVIEWER:
		_dialog._state = __dsEDITOR;
		_editor = true;
		_viewer = false;
		break;
	case __dsEDITOR:
		_dialog._state = __dsVIEWER;
		_editor = false;
		_viewer = true;
		break;
	};
	if ( be_gActivNode!=NULL ) be_gActivNode=NULL;
	setAction(__naFREE);
};
void	be_CNodeArray::FN_SWITCH_MODE(){
	switchMode();
};
void	be_CNodeArray::addElementVIEWER(be_CARRAY* pARR){
	if ( pARR==NULL || pARR->GetName()==NULL || (pARR->GetName())[0]==0 ) return;
	be_CArrayLine* pAL = new be_CArrayLine(pARR->GetName(),false);
	_arrayVList.Add(pAL);
	pAL = NULL;
};
void	be_CNodeArray::updateVIEWER(){
	int NAL = _arrayVList.GetAmount();
	int N = g_beVAR_STORE.ARRAY_LIST.GetAmount();	int RAN = 0;
	be_CARRAY* pARR = NULL;
	be_CArrayLine* pNAL = NULL;
	be_CVariableBase* pVAR = NULL;
	for ( int i=0; i<N; i++ ){
		NAL = _arrayVList.GetAmount();
        pARR = g_beVAR_STORE.ARRAY_LIST[i];
		if (pARR!=NULL&&pARR->GetAmount()>0) {
			pVAR = (*pARR)[0];
			if ( pVAR!=NULL && strcmp(pVAR->GetClassName(),"be_CNODE")==0 ){
				bool needAdd = ( i>=NAL );
				bool reCreate = ( !needAdd && i<NAL && strcmp(pARR->GetName(),_arrayVList[i]->_arrayName.pchar())!=0 );
				if ( needAdd ){
					be_CArrayLine* pAL = new be_CArrayLine(pARR->GetName(),false);
					_arrayVList.Add(pAL);
					pAL = NULL;
				}else if (reCreate){
					_arrayVList[i]->_arrayName = pARR->GetName();
				};
				RAN++;
			};
		};
	};
	NAL = _arrayVList.GetAmount();
	if ( NAL>RAN ){
		for ( int i=RAN; i<NAL; i++ )	_arrayVList.DelElement(i);
	};
};
void	be_CNodeArray::FN_UPDATE_VIEWER(){
	updateVIEWER();
};
void	be_CNodeArray::FN_HIDE(){
	setVisible(false);
};
//void	be_CNodeArray::NA_HIDE::EvaluateFunction(){
//	g_beNodeArrayEditor.setVisible(false);
//}
bool	be_CNodeArray::setArrayForEdit(int _id){
	if (  0<=_id && _id<_arrayVList.GetAmount() ){
		be_CArrayLine* pAL = _arrayVList[_id];
		if (pAL) {
			be_CARRAY* pARR = g_beVAR_STORE.ARRAY_LIST.GetByName(pAL->_arrayName.pchar());
			if (pARR!=NULL){
				pAL->_visible = true;
				_arrayEditName = pAL->_arrayName;
				return true;
			};
		};
	};
	return false;
};
void	be_CNodeArray::FN_GOTO_EDITOR(){
	if ( SimpleDialog::CallParams.Clicked && 0<=SimpleDialog::CallParams.Clicked->AutoIndexInArray) {
		if ( setArrayForEdit(SimpleDialog::CallParams.Clicked->AutoIndexInArray)){
			switchMode();
		}
	};
};
void	be_CNodeArray::selectArray(int _id){
	if (  0<=_id && _id<_arrayVList.GetAmount() ){
		be_CArrayLine* pAL = _arrayVList[_id];
		if (pAL) {
			be_CARRAY* pARR = g_beVAR_STORE.ARRAY_LIST.GetByName(pAL->_arrayName.pchar());
			if (pARR!=NULL){
				_arrayEditName = pAL->_arrayName;
			};
		};
	};
};
void	be_CNodeArray::FN_SELECT_ARRAY(){
	if ( SimpleDialog::CallParams.Clicked && 0<=SimpleDialog::CallParams.Clicked->AutoIndexInArray) {
		selectArray(SimpleDialog::CallParams.Clicked->AutoIndexInArray);
	};
};
void	be_CNodeArray::clearArray(){
	be_CARRAY* pARR = g_beVAR_STORE.ARRAY_LIST.GetByName(_arrayEditName.pchar());
	if (pARR!=NULL) {
		_str l_mes = "Delete all nodes in Array \"";
		l_mes += _arrayEditName;
		l_mes += "\" ?";
		if ( MessageBox(NULL, l_mes.pchar(), "Node Array Editor", MB_OKCANCEL) == IDOK ){
			pARR->ValuesArray.CLEAR();
			_dialog.updateClassEditor();
		};
	};
};
void	be_CNodeArray::reorderArray(){
	be_CARRAY* pARR = g_beVAR_STORE.ARRAY_LIST.GetByName(_arrayEditName.pchar());
	if (pARR!=NULL) {
		for ( int i=0; i<pARR->GetAmount()/2; i++ ){
			swap(pARR->ValuesArray[i],pARR->ValuesArray[pARR->GetAmount()-1-i]);
		};
		_dialog.updateClassEditor();
	};
};
void	be_CNodeArray::FN_CLEAR_ARRAY(){
	clearArray();
};
void	be_CNodeArray::FN_REORDER_ARRAY(){
	reorderArray();
};
void	be_CNodeArray::validateArray(bool _force/*=false*/){
	if (_showADV||_force){
		be_CARRAY* pARR = _array.Get();
		if (pARR!=NULL&&strcmp(pARR->GetName(),_arrayEditName.pchar())==0) return;
		pARR = g_beVAR_STORE.ARRAY_LIST.GetByName(_arrayEditName.pchar());
		_array.Set(pARR); 
		if ( pARR==NULL ) _arrayEditName="NONAME";
		else			  _arrayEditName=pARR->GetName();
		pARR = NULL;
		_dialog.updateClassEditor();
		return;
	};
};
void	be_CNodeArray::processDialogs(){
	_showADV = ( _ADW && _visible );
	validateArray();
};
void	be_CNodeArray::aplayParamMaskForAll(){
	validateArray(true);
	be_CARRAY* pARR = _array.Get();
	if (pARR!=NULL) {
		int N = pARR->ValuesArray.GetAmount();
		be_CNODE* pND = NULL;
		bool l_ReName = ( _nodeName.pchar()!=NULL && (_nodeName.pchar())[0]!=0 );
		_str	l_name = "";
		if ( _addArrayName )	l_name += _arrayEditName;
		if (l_ReName) l_name += _nodeName;
		_str	l_nodeName = "";
		_str	l_parName = "";
		int	l_R = -1;
		if ( _nodeR.pchar()!=0 && (_nodeR.pchar())[0]!=0 ) sscanf(_nodeR.pchar(),"%d",&l_R);
		while (N--) {
			pND = dynamic_cast<be_CNODE*>( (*pARR)[N] );
			if (pND!=NULL) {
				l_parName = "n"; l_parName += N;
				((be_CPrametrBase*)(pARR->ValuesArray[N]))->SetParamName(l_parName.pchar());

				if (l_R!=-1) pND->vSetR(l_R);
				if ( l_ReName ){
					l_nodeName = l_name;
					if ( _autoIndex ){ l_nodeName+="_"; l_nodeName += N; }
					pND->SetName(l_nodeName.pchar());
				};
			};
		};
		g_beOBJ_CONTROLER.SetState("be_CNODE");
		_dialog.updateClassEditor();
	};
};
void	be_CNodeArray::FN_APLAY_PARAM_MASK(){
	_str	l_mes = "Aplay : \n";
	_str	l_name = "Nodes Name Mask: ";
	if ( _addArrayName )	l_name += _arrayEditName;
	l_name += _nodeName;
	if ( _autoIndex ) l_name += "_LID";
	l_name += "  R( ";
	l_name += _nodeR;
	l_name += " )\n";
	l_mes += l_name;
	l_mes += "for all node in array \"";
	l_mes += _arrayEditName;
	l_mes += "\"?";
	if ( MessageBox(NULL, l_mes.pchar(), "Node Array Editor", MB_OKCANCEL) == IDOK ){
		aplayParamMaskForAll();
	};
}
void	be_CNodeArray::convertToLocalNodes(bool _delPrivGlobalNodes/*=false*/){
	validateArray(true);
	be_CARRAY* pARR = _array.Get();
	if (pARR!=NULL) {
		be_CNODE* pND = NULL;
		be_CNODE* pnewND = NULL;
		int N = pARR->GetAmount();
		while (N--) {
			pND = dynamic_cast<be_CNODE*>( (*pARR)[N] );
			if (pND!=NULL) {
				be_CBasePrametr* pBP = (be_CBasePrametr*)(pARR->ValuesArray[N]);
				if ( pBP && pBP->UniveraslParam.Local==false ){
					pnewND = new be_CNODE( pND->GetName(), pND->x, pND->y, pND->R, pND->Dir, pND->SegmFR );
					pBP->UniveraslParam.GlobalReference.Set(NULL);
					pBP->UniveraslParam.SetLocal(true);
					pBP->UniveraslParam.LocalObject.Set(pnewND);
					pnewND = NULL;
					if ( _delPrivGlobalNodes ){ 
						g_beVAR_STORE.NODE_LIST.DelByGID( pND->GetObjectGlobalID() );
						pND = NULL;
					};
				};
			};
		};
		g_beOBJ_CONTROLER.SetState("be_CNODE");
		_dialog.updateClassEditor();
	};
};
void	be_CNodeArray::FN_CONVERT_TO_LOCAL(){
	_str	l_mes = "Do you want convert all nodes in array \"";
	l_mes += _arrayEditName;
	l_mes += "\" to local node for array?";
	if ( MessageBox(NULL, l_mes.pchar(), "Node Array Editor", MB_OKCANCEL) == IDOK ){
		bool	l_delGlobalNodes = (MessageBox(NULL, "Do you want delete current global nodes from array?", "Node Array Editor", MB_OKCANCEL) == IDOK);
		convertToLocalNodes(l_delGlobalNodes);
	};
};
NODEARRAY_ACTIONS	be_CNodeArray::getAction(){
	if ( _nodeFree )	return __naFREE;
	if ( _nodeAdd  )	return __naADD;
	if ( _nodeDel  )	return __naDEL;
	if ( _nodeMove )	return __naMOVE;
	return __naFREE;
};
void				be_CNodeArray::setAction(NODEARRAY_ACTIONS _action){
	_nodeFree=false;
	_nodeAdd=false;
	_nodeDel=false;
	_nodeMove=false;
	switch(_action) {
	case __naADD:
		_nodeAdd=true;
		break;
	case __naDEL:
		_nodeDel=true;
		break;
	case __naMOVE:
		_nodeMove=true;
		break;
	default:
		_nodeFree=true;
	};
};
bool				be_CNodeArray::processActions(bool& LeftPressed,int MapCoordX,int MapCoordY){
	switch( getAction() ) {
	case __naADD:
		if (!mouseOnDD()) return processActionAdd(LeftPressed,MapCoordX,MapCoordY);
		break;
	case __naDEL:
		if (!mouseOnDD()) return processActionDel(LeftPressed,MapCoordX,MapCoordY);
		break;
	case __naMOVE:
		if (!mouseOnDD()) return processActionMove(LeftPressed,MapCoordX,MapCoordY);
		break;
	default:
		return processActionFree(LeftPressed,MapCoordX,MapCoordY);
	};
};
bool				be_CNodeArray::mouseOnDD(){
	return _dialog.mouseOn();
};
int					be_CNodeArray::getNearestNodeID(int mx, int my){
	validateArray(true);
	be_CARRAY* pARR = _array.Get();
	if (pARR!=NULL) {
		int N = pARR->GetAmount();
		be_CNODE* pND = NULL;
		while (N--) {
			pND = (be_CNODE*)((*pARR)[N]);
			if (pND&&Norma( mx - pND->x, my - pND->y )<250) return N;
		};
	};
	return -1;	
};
bool	be_CNodeArray::processActionFree(bool& LeftPressed,int MapCoordX,int MapCoordY){
	if ( be_gActivNode!=NULL ) be_gActivNode=NULL;
    return false;
};
void	UnPress();
bool	be_CNodeArray::processActionAdd(bool& LeftPressed,int MapCoordX,int MapCoordY){
	validateArray(true);
	be_CARRAY* pARR = _array.Get();
	if ( pARR && LeftPressed ){
		_str	l_name = "";
				
		int new_id = pARR->GetAmount();
		l_name = "n"; l_name += new_id;
		be_CNODE* pND = pARR->ValuesArray.CreateParam_NODE(l_name.pchar());

		l_name = "";
		if ( _addArrayName )	l_name += _arrayEditName;
		l_name += _nodeName;
		if ( _autoIndex ){ l_name+="_"; l_name += new_id; };
		pND->SetName(l_name.pchar());
		pND->vSetX(MapCoordX);
		pND->vSetY(MapCoordY);

		int	l_R = -1;
		if ( _nodeR.pchar()!=0 && (_nodeR.pchar())[0]!=0 ) sscanf(_nodeR.pchar(),"%d",&l_R);
		if ( l_R!=-1 ) pND->vSetR(l_R);

		_dialog.updateClassEditor();

		return true;
	}
	return false;
};
bool	be_CNodeArray::processActionDel(bool& LeftPressed,int MapCoordX,int MapCoordY){
	validateArray(true);
	be_CARRAY* pARR = _array.Get();
	if ( pARR && LeftPressed ){
		int _id = getNearestNodeID(MapCoordX,MapCoordY);
		if (_id!=-1) {
			pARR->ValuesArray.DelElement(_id);
			return true;
		};
	};
	return false;
};
extern short WheelDelta;
bool	be_CNodeArray::processActionMove(bool& LeftPressed,int MapCoordX,int MapCoordY){
	validateArray(true);
	be_CARRAY* pARR = _array.Get();
	if ( pARR && LeftPressed ){
		if ( be_gActivNode==NULL ){
			int _id = getNearestNodeID(MapCoordX,MapCoordY);
			if (_id!=-1) {
				be_gActivNode = (be_CNODE*)((*pARR)[_id]);
				return true;
			};
		};
		if ( be_gActivNode!=NULL ) {
			be_gActivNode = NULL;
			return true;
		}
	};
	if (be_gActivNode!=NULL) {
		be_gActivNode->vSetXY(MapCoordX,MapCoordY);
		// update roll button
		if(WheelDelta){
			be_gActivNode->vSetR( be_gActivNode->vGetR() + WheelDelta/12 );
			WheelDelta=0;
		}
	};
	return false;
};
//////////////////////////////////////////////////////////////////////////
// be_CNodeArrayDriver	//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool	be_CNodeArrayDriver::Process(
										int mx,
										int my,
										bool& LeftPressed,
										bool& RightPressed,
										int MapCoordX,
										int MapCoordY,
										bool OverMiniMap
									)
{
	return g_beNodeArrayEditor.Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap);	
};
void	be_CNodeArrayDriver::clear(){
	g_beNodeArrayEditor.clear();
};
void	be_CNodeArrayDriver::GetObjectGlobalID(){
	g_beNodeArrayEditor.GetObjectGlobalID();
};
void	be_CNodeArrayDriver::setVisible(bool _state){
	g_beNodeArrayEditor.setVisible(_state);
};
void	be_CNodeArrayDriver::OnInit(){
	g_beNodeArrayEditor._dialog.OnInit();
};

//*********************************************************************//
//**********************   NODES MANAGER   ****************************//
//*********************************************************************//

//////////////////////////////////////////////////////////////////////////
// be_CNodesDialog ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
be_CNodesDialog::be_CNodesDialog(){
	pDD_ADV=NULL;
	pDD_MAIN=NULL;
	pCD_TXT=NULL;
};
be_CNodesDialog::~be_CNodesDialog(){
	
};
void	be_CNodesDialog::OnInit(){
	be_CBaseDialog::OnInit("Missions\\GRAPHICS\\TOOLS\\Dialog\\NodeEditor.DialogsSystem.xml");
	if ( !_error ){
		pDD_ADV = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_NODES_LIST") );
		pDD_MAIN = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_MAIN") );
		pCD_TXT = dynamic_cast< DialogsDesk* >( _mainDS.Find("CD_TXT") );
	};
};
bool	be_CNodesDialog::mouseOn(){
	return mouseOnDD(pDD_ADV)||mouseOnDD(pDD_MAIN);
};
void	be_CNodesDialog::VScroll_ToElement(int _id){
	be_CBaseDialog::VScroll_ToElement(_id,pCD_TXT);
};
//////////////////////////////////////////////////////////////////////////
// be_CNodeLine //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
be_CNodeLine::be_CNodeLine(){
	_nodeName = "NONAME";
	_visible = false;
	_selected = false;
};
be_CNodeLine::be_CNodeLine(char* _name,bool _state/*=false*/){
	_nodeName = ( (_name!=NULL&&_name[0]!=0) ? (_name) : ("NONAME") );
	_visible = _state;
};
be_CNodeLine::~be_CNodeLine(){
	_nodeName.Clear();
};
void	be_CNodeLine::OnRender(bool _forceDraw/*=false*/){
	if ( _visible ){
		be_CNODE* pND = g_beVAR_STORE.NODE_LIST.GetByName(_nodeName.pchar());
		if (pND!=NULL) {
			pND->vDraw(_forceDraw);
		};
	};
};


//////////////////////////////////////////////////////////////////////////
// be_CNodes /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
be_CNodes	g_beNodesEditor("NodesEditor");
void	be_CNodes::init(char* _name/*=NULL*/){
	Name = ( (_name!=NULL&&_name[0]!=0) ? (_name) : ("NodesEditorExemple") );
	setVisible(false);
	_ADV = false;
	updateADV();

	_nodesDraw	= true;
	_nodesForceDraw = false;

	_nodeList.Clear();

	// Actions
	setAction();

	_nodesNameMask = "NONAME";
	_nodesR = 0;
	_nodesDIR = 0;
	_nodesSegFR = 0;
	_nodesAutoIndex = false;
	_nodesLastIndex = 0;

		// One Node Data	
	_nodeName = "NONAME";
	_nodeR = 0;
	_nodeDIR = 0;
	_nodeSegFR = 0;
	_nodeX = 0;
	_nodeY = 0;
	_node.Set(NULL);

	_visibleNodeList=false;
};
be_CNodes::be_CNodes(){
	init();
};
be_CNodes::be_CNodes(char* _name){
	init(_name);
};
void	be_CNodes::clear(){
	_nodeList.reset_class(&_nodeList);
};
be_CNodes::~be_CNodes(){
	clear();
};
bool	be_CNodes::Process(
								int mx,
								int my,
								bool& LeftPressed,
								bool& RightPressed,
								int MapCoordX,
								int MapCoordY,
								bool OverMiniMap
						   )
{
	bool oldADV = _showADV;
	processDialogs();
	_dialog.OnRender();
	if ( _visible ){
		rsFlush();
		if ( _nodesDraw==true ) for (int i=0;i<_nodeList.GetAmount(); i++) _nodeList[i]->OnRender(_nodesForceDraw);
		rsFlush();
		if ( getAction()==__ndFREE || getAction()==__ndSELECT ) aplayChangesForSelectedNode();
		if ( oldADV==false && _showADV==true ) updateNodeList();
		return processActions(LeftPressed,MapCoordX,MapCoordY);
	};
	return false;
};
void	be_CNodes::processDialogs(){
	updateADV();
};
void	be_CNodes::setVisible(bool _state){
	_visible = _state;
};
bool	be_CNodes::getVisible(){
	return _visible;
};
void	be_CNodes::FN_HIDE(){
	setVisible(false);
	setAction(__ndFREE);
};
void	be_CNodes::updateADV(){
	_showADV = ( _ADV && _visible );
	g_fWheelIsBlocked=true;
	if ( _visible==false || _nodeMove==false ) g_fWheelIsBlocked=false;
};
NODES_ACTIONS	be_CNodes::getAction(){
	if ( _nodeFree   )	return __ndFREE;
	if ( _nodeAdd    )	return __ndADD;
	if ( _nodeDel    )	return __ndDEL;
	if ( _nodeMove   )	return __ndMOVE;
	if ( _nodeSelect )	return __ndSELECT;
	return __ndFREE;
};
void	be_CNodes::setAction(NODES_ACTIONS _action/*=__naFREE*/){
	_nodeFree=false;
	_nodeAdd=false;
	_nodeDel=false;
	_nodeMove=false;
	_nodeSelect=false;
	switch(_action) {
	case __ndADD:
		_nodeAdd=true;
		break;
	case __ndDEL:
		_nodeDel=true;
		break;
	case __ndMOVE:
		_nodeMove=true;
		break;
	case __ndSELECT:
		_nodeSelect=true;
		break;
	default:
		_nodeFree=true;
	};
};
bool	be_CNodes::processActions(bool& LeftPressed,int MapCoordX,int MapCoordY){
	switch( getAction() ) {
	case __ndADD:
		if (!mouseOnDD()) return processActionAdd(LeftPressed,MapCoordX,MapCoordY);
		break;
	case __ndDEL:
		if (!mouseOnDD()) return processActionDel(LeftPressed,MapCoordX,MapCoordY);
		break;
	case __ndMOVE:
		if (!mouseOnDD()) return processActionMove(LeftPressed,MapCoordX,MapCoordY);
		break;
	case __ndSELECT:
		if (!mouseOnDD()) return processActionSelect(LeftPressed,MapCoordX,MapCoordY);
		break;
	default:
		return processActionFree(LeftPressed,MapCoordX,MapCoordY);
	};
};
bool	be_CNodes::mouseOnDD(){
	return _dialog.mouseOn();
};
int		be_CNodes::getNearestNodeID(int mx, int my){
	be_CVNODE_ACT* pnewNODE_ACT = new be_CVNODE_ACT();
	be_gActivNode = pnewNODE_ACT->vGetNode(mx,my);
	delete pnewNODE_ACT;
	pnewNODE_ACT = NULL;
	if ( be_gActivNode==NULL ) return -1;
	for ( int i=0; i<_nodeList.GetAmount(); i++ ){
		if ( be_gActivNode->Name == _nodeList[i]->_nodeName ) return i;
	};
};
bool	be_CNodes::processActionFree(bool& LeftPressed,int MapCoordX,int MapCoordY){
	if ( be_gActivNode!=NULL ) be_gActivNode=NULL;
    return false;	
};
bool	be_CNodes::processActionAdd(bool& LeftPressed,int MapCoordX,int MapCoordY){
	if ( LeftPressed ){
		_str	l_name = _nodesNameMask;
		if (_nodesAutoIndex){
			l_name += _nodesLastIndex;
			// Icress index
			int	vvv = -1;
			if ( _nodesLastIndex.pchar()!=0 && (_nodesLastIndex.pchar())[0]!=0 ) sscanf(_nodesLastIndex.pchar(),"%d",&vvv);			
			vvv++;
			_nodesLastIndex = vvv;
		};
		be_CNODE* pND = new be_CNODE(l_name.pchar(),MapCoordX,MapCoordY);
		pND->GetObjectGlobalID();
        g_beVAR_STORE.NODE_LIST.Add(pND);
		
		// R
		int	vvv = -1;
		if ( _nodesR.pchar()!=0 && (_nodesR.pchar())[0]!=0 ) sscanf(_nodesR.pchar(),"%d",&vvv);			
		if ( vvv!=pND->vGetR() ) pND->vSetR(vvv);
		// DIR
		vvv = -1;
		if ( _nodesDIR.pchar()!=0 && (_nodesDIR.pchar())[0]!=0 ) sscanf(_nodesDIR.pchar(),"%d",&vvv);			
		if ( vvv!=pND->vGetDir() ) pND->vSetDir(vvv);
		// SegFR
		vvv = -1;
		if ( _nodesSegFR.pchar()!=0 && (_nodesSegFR.pchar())[0]!=0 ) sscanf(_nodesSegFR.pchar(),"%d",&vvv);			
		if ( vvv!=pND->vGetSegmFR() ) pND->vSetSegmFR(vvv);

		pND = NULL;

		updateNodeList();
		selectNode(_nodeList.GetAmount()-1,true);
		updateSelectedNodeData();

		return true;
	}
	return false;	
};
bool	be_CNodes::processActionDel(bool& LeftPressed,int MapCoordX,int MapCoordY){
	bool _HR = false;
	if ( LeftPressed ){
		be_CVNODE_ACT* pnewNODE_ACT = new be_CVNODE_ACT();
		_HR = pnewNODE_ACT->vDelNode(MapCoordX,MapCoordY);
		delete pnewNODE_ACT;
		pnewNODE_ACT=NULL;	
		if ( _HR ) updateNodeList();
	};
	return _HR;
};
bool	be_CNodes::processActionMove(bool& LeftPressed,int MapCoordX,int MapCoordY){
	if ( LeftPressed ){
		if ( be_gActivNode==NULL ){
			int _id = getNearestNodeID(MapCoordX,MapCoordY);
			if (_id!=-1) {
				selectNode(_id,true);
				be_gActivNode = (be_CNODE*)(_node.Get());
				return true;
			};
		};
		if ( be_gActivNode!=NULL ) {
			be_gActivNode = NULL;
			return true;
		}
	};
	if (be_gActivNode!=NULL) {
		be_gActivNode->vSetXY(MapCoordX,MapCoordY);
		// update roll button
		if(WheelDelta){
			be_gActivNode->vSetR( be_gActivNode->vGetR() + WheelDelta/12 );
			WheelDelta=0;
		}
		updateSelectedNodeData();
	};
	return false;	
};
bool	be_CNodes::processActionSelect(bool& LeftPressed,int MapCoordX,int MapCoordY){
	bool _HR = false;
	if ( LeftPressed ){
		int _id = getNearestNodeID(MapCoordX,MapCoordY);
		_HR = (_id!=-1);
		if ( _HR ) selectNode(_id,true);
	};
	return _HR;
};
void	be_CNodes::deleteNode(int _id){
	if ( 0<=_id && _id<_nodeList.GetAmount() ){
		be_CNODE* pND = g_beVAR_STORE.NODE_LIST.GetByName( _nodeList[_id]->_nodeName.pchar() );
		if (pND!=NULL) {
			g_beVAR_STORE.NODE_LIST.DelByGID(pND->GetObjectGlobalID());
		};
		pND = NULL;
	};
};
void	be_CNodes::FN_DELETE(){
	int _id = -1;
	if ( SimpleDialog::CallParams.Clicked && 0<=SimpleDialog::CallParams.Clicked->AutoIndexInArray) {
		_id = SimpleDialog::CallParams.Clicked->AutoIndexInArray;
		deleteNode(_id);
	};
	updateNodeList();
	if ( _nodeList.GetAmount()>0 ){
		if ( _id>=_nodeList.GetAmount() ) _id = _nodeList.GetAmount()-1;
	}else{
		_id = -1;
	};
	selectNode(_id,true);
	updateSelectedNodeData();
};
void	be_CNodes::updateNodeList(){
	int NL = _nodeList.GetAmount();
	int N = g_beVAR_STORE.NODE_LIST.GetAmount();
	be_CNODE* pND = NULL;
	be_CNodeLine* pNL = NULL;
	for ( int i=0; i<N; i++ ){
		NL = _nodeList.GetAmount();
        pND = g_beVAR_STORE.NODE_LIST[i];
		if (pND!=NULL) {
			bool needAdd = ( i>=NL );
			bool reCreate = ( !needAdd && i<NL && strcmp(pND->GetName(),_nodeList[i]->_nodeName.pchar())!=0 );
			if ( needAdd ){
				pNL = new be_CNodeLine(pND->GetName(),true);
				_nodeList.Add(pNL);
				pNL=NULL;
			}else if (reCreate){
				_nodeList[i]->_nodeName = pND->GetName();
			};
		};
	};
	NL = _nodeList.GetAmount();
	if ( NL>N ){
		for ( int i=N; i<NL; i++ )	_nodeList.DelElement(i);
	};
};
void	be_CNodes::FN_UPDATE_LIST(){
	updateNodeList();
};
void	be_CNodes::selectNode(int _id,bool _scrollOnIt/*=false*/){
	// try un select privios node
	for ( int i=0; i<_nodeList.GetAmount(); i++ ) _nodeList[i]->_selected = false;
	// select new node
	if ( 0<=_id && _id<_nodeList.GetAmount() ){
		be_CNodeLine* pNL = _nodeList[_id];
		if (pNL!=NULL) {
			be_CNODE* pND = g_beVAR_STORE.NODE_LIST.GetByName(pNL->_nodeName.pchar());
			if ( pND!=NULL ){
				if ( pND->Name!=_nodeName || _node.Get()==NULL ){
					_nodeName = _nodeList[_id]->_nodeName;
					_node.Set( pND );
					if ( _scrollOnIt ) _dialog.VScroll_ToElement(_id);
				};
				_nodeList[_id]->_selected = true;
			};
			pND=NULL;
		};
	}else{
		_node.Set(NULL);
	};
	updateSelectedNodeData();
};
void	be_CNodes::FN_SELECT_NODE(){
	if ( SimpleDialog::CallParams.Clicked && 0<=SimpleDialog::CallParams.Clicked->AutoIndexInArray) {
		selectNode(SimpleDialog::CallParams.Clicked->AutoIndexInArray);
	};	
};
void	be_CNodes::screenOnNode(int _id){
	if ( 0<=_id && _id<_nodeList.GetAmount() ){
		be_CNodeLine* pNL = _nodeList[_id];
		if (pNL!=NULL) {
			be_CNODE* pND = g_beVAR_STORE.NODE_LIST.GetByName(pNL->_nodeName.pchar());
			if ( pND!=NULL )	pND->ScreenOnNode();
			pND=NULL;
		};
	};
};
void	be_CNodes::FN_ON_SCREEN(){
	if ( SimpleDialog::CallParams.Clicked && 0<=SimpleDialog::CallParams.Clicked->AutoIndexInArray) {
		screenOnNode(SimpleDialog::CallParams.Clicked->AutoIndexInArray);
	};	
};
void	be_CNodes::updateSelectedNodeData(){
	be_CNODE* pND = _node.Get();
	if ( pND==NULL ){
		_nodeName = "NONAME";
		_nodeR = 0;
		_nodeDIR = 0;
		_nodeSegFR = 0;
		_nodeX = 0;
		_nodeY = 0;
		return;
	}else{
		_nodeName = pND->GetName();
		_nodeR = pND->vGetR();
		_nodeDIR = pND->vGetDir();
		_nodeSegFR = pND->vGetSegmFR();
		_nodeX = pND->vGetX();
		_nodeY = pND->vGetY();
	};
};
void	be_CNodes::aplayChangesForSelectedNode(){
	be_CNODE* pND = (be_CNODE*)(_node.Get());
	if ( pND!=NULL ){
		// Name
		if ( pND->Name!=_nodeName ){
			pND->SetName( _nodeName.pchar() );
			updateNodeList();
		};
		// R
		int	vvv = -1;
		if ( _nodeR.pchar()!=0 && (_nodeR.pchar())[0]!=0 ) sscanf(_nodeR.pchar(),"%d",&vvv);			
		if ( vvv!=pND->vGetR() ) pND->vSetR(vvv);
		// DIR
		vvv = -1;
		if ( _nodeDIR.pchar()!=0 && (_nodeDIR.pchar())[0]!=0 ) sscanf(_nodeDIR.pchar(),"%d",&vvv);			
		if ( vvv!=pND->vGetDir() ) pND->vSetDir(vvv);
		// SegFR
		vvv = -1;
		if ( _nodeSegFR.pchar()!=0 && (_nodeSegFR.pchar())[0]!=0 ) sscanf(_nodeSegFR.pchar(),"%d",&vvv);			
		if ( vvv!=pND->vGetSegmFR() ) pND->vSetSegmFR(vvv);
		// X
		vvv = -1;
		if ( _nodeX.pchar()!=0 && (_nodeX.pchar())[0]!=0 ) sscanf(_nodeX.pchar(),"%d",&vvv);			
		if ( vvv!=pND->vGetX() ) pND->vSetX(vvv);
		// Y
		vvv = -1;
		if ( _nodeY.pchar()!=0 && (_nodeY.pchar())[0]!=0 ) sscanf(_nodeY.pchar(),"%d",&vvv);			
		if ( vvv!=pND->vGetY() ) pND->vSetY(vvv);
	};
};
void	be_CNodes::aplayVisibleForAllNode(){
	int N = _nodeList.GetAmount();
	while (N--) {
		_nodeList[N]->_visible = _visibleNodeList;
	};
};
void	be_CNodes::FN_APLAY_VISIBLE_LIST(){
	aplayVisibleForAllNode();
};
//////////////////////////////////////////////////////////////////////////
// be_CNodesDriver ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool	be_CNodesDriver::Process(
									int mx,
									int my,
									bool& LeftPressed,
									bool& RightPressed,
									int MapCoordX,
									int MapCoordY,
									bool OverMiniMap
								)
{
	return g_beNodesEditor.Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap);									
};
void	be_CNodesDriver::clear(){
	g_beNodesEditor.clear();
};
void	be_CNodesDriver::GetObjectGlobalID(){
	g_beNodesEditor.GetObjectGlobalID();
};
void	be_CNodesDriver::setVisible(bool _state){
	g_beNodesEditor.setVisible(_state);
};
bool	be_CNodesDriver::getVisible(){
	return g_beNodesEditor.getVisible();
};
void	be_CNodesDriver::OnInit(){
	g_beNodesEditor._dialog.OnInit();
};


//*********************************************************************//
//*********************   GROUPS MANAGER   ****************************//
//*********************************************************************//
be_CGroups	g_beGroupsEditor("GroupsEditor");
// be_CGroupsDialog /////////////////////////////////////////////////////
be_CGroupsDialog::be_CGroupsDialog(){
	pDD_ADV=NULL;
	pDD_MAIN=NULL;
	pCD_TXT=NULL;
};
be_CGroupsDialog::~be_CGroupsDialog(){
	
};
void	be_CGroupsDialog::OnInit(){
	be_CBaseDialog::OnInit("Missions\\GRAPHICS\\TOOLS\\Dialog\\GroupEditor.DialogsSystem.xml");
	if ( !_error ){
		pDD_ADV = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_ADV") );
		pDD_MAIN = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_MAIN") );
		pCD_TXT = dynamic_cast< DialogsDesk* >( _mainDS.Find("CD_TXT") );
	};
};
bool	be_CGroupsDialog::mouseOn(){
	return mouseOnDD(pDD_ADV)||mouseOnDD(pDD_MAIN);
};
void	be_CGroupsDialog::VScroll_ToElement(int _id){
	be_CBaseDialog::VScroll_ToElement(_id,pCD_TXT);
};
// be_CGroupLine ////////////////////////////////////////////////////////
be_CGroupLine::be_CGroupLine(){
	_groupName = "NONAME";
	_visible = false;
	_selected = false;
};
be_CGroupLine::be_CGroupLine(char* _name,bool _state/*=false*/){
	_groupName = ( (_name!=NULL&&_name[0]!=0) ? (_name) : ("NONAME") );
	_visible = _state;
};
be_CGroupLine::~be_CGroupLine(){
	_groupName.Clear();
};
void	be_CGroupLine::OnRender(bool _forceDraw/*=false*/){
	if ( _visible ){
		be_CGROUP* pGR = g_beVAR_STORE.GROUP_LIST.GetByName(_groupName.pchar());
		if (pGR!=NULL) {
//			pGR->visible = _visible;
			pGR->vDraw(_forceDraw);
		};
	};
};
// be_CGroups ///////////////////////////////////////////////////////////
void	be_CGroups::init(char* _name/*=NULL*/){
	Name = ( (_name!=NULL&&_name[0]!=0) ? (_name) : ("GroupsEditorExemple") );
	setVisible(false);
	_ADV = false;
	updateADV();

	_groupsDraw	= true;
	_groupsForceDraw = false;

	_groupList.Clear();

	_nameMask = "NONAME";
	_autoIndex = false;
	_lastIndex = 0;
	_generateName = true;

		// One Node Data	
	_group.Set(NULL);
	_nameGroup = "NONAME";
	_selectOnMap = false;

	_visibleGroupList=false;

	lastHotKey = 0x00;
};
be_CGroups::be_CGroups(){
	init();
};
be_CGroups::be_CGroups(char* _name){
	init(_name);
};
void	be_CGroups::clear(){
	_groupList.reset_class(&_groupList);
};
be_CGroups::~be_CGroups(){
	clear();
};
bool	be_CGroups::Process(
								int mx,
								int my,
								bool& LeftPressed,
								bool& RightPressed,
								int MapCoordX,
								int MapCoordY,
								bool OverMiniMap
							)
{
	bool oldADV = _showADV;
	processDialogs();
	_dialog.OnRender();
	if ( _visible ){
//		ProcessHotKey();
		rsFlush();
		if ( _groupsDraw==true ) for (int i=0;i<_groupList.GetAmount(); i++) _groupList[i]->OnRender(_groupsForceDraw);
		rsFlush();
		aplayChangesForSelectedGroup();
		if ( oldADV==false && _showADV==true ) updateGroupList();
		return processActions(LeftPressed,MapCoordX,MapCoordY);
	};
	return false;
};
void	be_CGroups::processDialogs(){
	updateADV();
};
void	be_CGroups::ProcessHotKey(){
	return;
	if ( LastKey==0 ) return;
	if ( lastHotKey!=0x00 && GetKeyState(lastHotKey)>=0 ) lastHotKey=0x00;
	if ( GetKeyState(VK_ESCAPE)<0 && lastHotKey!=VK_ESCAPE ){
		g_beDebuger.setVisible( false );lastHotKey=VK_ESCAPE;LastKey=0;KeyPressed=false;
	}else
	if ( GetKeyState(VK_LCONTROL)<0 || GetKeyState(VK_RCONTROL)<0 ){
		if ( GetKeyState('L')<0 && lastHotKey!='L' ) {
			_ADV = !_ADV;lastHotKey='L';LastKey=0;KeyPressed=false;
		}else
		if ( GetKeyState('X')<0 && lastHotKey!='X' ) {
			_groupsDraw = !_groupsDraw;lastHotKey='X';LastKey=0;KeyPressed=false;
		}else
		if ( GetKeyState('F')<0 && lastHotKey!='F' ) {
			_groupsForceDraw = !_groupsForceDraw;lastHotKey='F';LastKey=0;KeyPressed=false;
		}else
		if ( GetKeyState('N')<0 && lastHotKey!='N' ) {
			FN_ADD_NEW_GR();lastHotKey='N';LastKey=0;KeyPressed=false;
		}else
		if ( GetKeyState('A')<0 && lastHotKey!='A' ) {
			FN_ADD_SELECTED_UN();lastHotKey='A';LastKey=0;KeyPressed=false;
		}else
		if ( GetKeyState('R')<0 && lastHotKey!='R' ) {
			FN_REM_SELECTED_UN();lastHotKey='R';LastKey=0;KeyPressed=false;
		}else
		if ( GetKeyState('C')<0 && lastHotKey!='C' ) {
			FN_ON_SCREEN_GR();lastHotKey='C';LastKey=0;KeyPressed=false;
		}else
		if ( GetKeyState('D')<0 && lastHotKey!='D' ) {
			FN_CLEAR_GR();lastHotKey='D';LastKey=0;KeyPressed=false;
		}else
		if ( GetKeyState('U')<0 && lastHotKey!='U' ) {
			FN_UPDATE_LIST();lastHotKey='U';LastKey=0;KeyPressed=false;
		}else
		if ( GetKeyState('Q')<0 && lastHotKey!='Q' ) {
			FN_APLAY_VISIBLE_LIST();lastHotKey='Q';LastKey=0;KeyPressed=false;
		};
	}
};
void	be_CGroups::setVisible(bool _state){
	_visible = _state;
	lastHotKey = 0x00;
	if (_visible) LastKey = 0;
};
bool	be_CGroups::getVisible(){
	return _visible;
};
void	be_CGroups::FN_HIDE(){
	setVisible(false);
};
void	be_CGroups::updateADV(){
	_showADV = ( _ADV && _visible );	
};
void	be_CGroups::updateGroupList(){
	int NL = _groupList.GetAmount();
	int N = g_beVAR_STORE.GROUP_LIST.GetAmount();
	be_CGROUP* pGR = NULL;
	be_CGroupLine* pGL = NULL;
	for ( int i=0; i<N; i++ ){
		NL = _groupList.GetAmount();
        pGR = g_beVAR_STORE.GROUP_LIST[i];
		if (pGR!=NULL) {
			bool needAdd = ( i>=NL );
			bool reCreate = ( !needAdd && i<NL && strcmp(pGR->GetName(),_groupList[i]->_groupName.pchar())!=0 );
			if ( needAdd ){
				pGL = new be_CGroupLine(pGR->GetName(),true);
				pGR->visible = true;
				_groupList.Add(pGL);
				pGL=NULL;
			}else if (reCreate){
				_groupList[i]->_groupName = pGR->GetName();
			};
		};
	};
	NL = _groupList.GetAmount();
	if ( NL>N ){
		for ( int i=N; i<NL; i++ )	_groupList.DelElement(i);
	};	
};
void	be_CGroups::FN_UPDATE_LIST(){
	updateGroupList();
};
void	be_CGroups::aplayVisibleForAllGroup(){
	int N = _groupList.GetAmount();
	while (N--) {
		_groupList[N]->_visible = _visibleGroupList;
//		be_CGROUP* pGR = g_beVAR_STORE.GROUP_LIST.GetByName(_groupList[N]->_groupName.pchar());
//		if ( pGR!=NULL ) pGR->visible = _visibleGroupList;
	};
};
void	be_CGroups::FN_APLAY_VISIBLE_LIST(){
	aplayVisibleForAllGroup();
};
void	be_CGroups::addNewGroup(){
	_str newGruoupName = "";
	if (_generateName){
		newGruoupName  = _nameMask;
		if ( _autoIndex ){
			newGruoupName += "_";
			newGruoupName += _lastIndex;
			// Icress index
			int	vvv = -1;
			if ( _lastIndex.pchar()!=0 && (_lastIndex.pchar())[0]!=0 ) sscanf(_lastIndex.pchar(),"%d",&vvv);			
			vvv++;
			_lastIndex = vvv;
		};
	}else{
		// ...
		newGruoupName = "EnteredName";
	};
	be_CVGROUP_ACT* pnewGROUP_ACT = new be_CVGROUP_ACT();
	be_CGROUP* pvGRP = pnewGROUP_ACT->vAddGroup(newGruoupName.pchar());
	delete pnewGROUP_ACT; pnewGROUP_ACT=NULL;
	if (pvGRP!=NULL) pvGRP->AddSelectedUnits();
};
void	be_CGroups::FN_ADD_NEW_GR(){
	addNewGroup();
	updateGroupList();
	selectGroup(_groupList.GetAmount()-1,true);
};
void	be_CGroups::selectGroup(int _id,bool _scrollOnIt/*=false*/){
	// try un select privios group
	for ( int i=0; i<_groupList.GetAmount(); i++ ) _groupList[i]->_selected = false;
	// select new node
	if ( 0<=_id && _id<_groupList.GetAmount() ){
		be_CGroupLine* pGL = _groupList[_id];
		if (pGL!=NULL) {
			be_CGROUP* pGR = g_beVAR_STORE.GROUP_LIST.GetByName(pGL->_groupName.pchar());
			if ( pGR!=NULL && ( pGR->Name!=_nameGroup || _group.Get()==NULL ) ){
				_nameGroup = _groupList[_id]->_groupName;
				_group.Set( pGR );
				if ( _scrollOnIt ) _dialog.VScroll_ToElement(_id);
			};
			_groupList[_id]->_selected = true;
			pGR=NULL;
		};
	}else{
		_nameGroup = "NONAME";
		_group.Set(NULL);
	};
};
void	be_CGroups::FN_SELECT_GROUP_ID(){
	if ( SimpleDialog::CallParams.Clicked && 0<=SimpleDialog::CallParams.Clicked->AutoIndexInArray) {
		selectGroup(SimpleDialog::CallParams.Clicked->AutoIndexInArray);
	};	
};
void	be_CGroups::addSelected(){
	be_CGROUP* pGR = (be_CGROUP*)(_group.Get());
	if ( pGR!=NULL ){
		pGR->AddSelectedUnits();
	};
};
void	be_CGroups::FN_ADD_SELECTED_UN(){
	addSelected();
};
void	be_CGroups::remSelected(){
	be_CGROUP* pGR = (be_CGROUP*)(_group.Get());
	if ( pGR!=NULL ){
		pGR->RemSelectedUnits();
	};
};
void	be_CGroups::FN_REM_SELECTED_UN(){
	remSelected();
};
void	be_CGroups::delGroup(int _id){
	if ( 0<=_id && _id<_groupList.GetAmount() ){
		be_CGROUP* pGR = g_beVAR_STORE.GROUP_LIST.GetByName( _groupList[_id]->_groupName.pchar() );
		if (pGR!=NULL) {
			g_beVAR_STORE.GROUP_LIST.DelByGID(pGR->GetObjectGlobalID());
		};
		pGR = NULL;
	};
};
void	be_CGroups::FN_DEL_GR(){
	int _id = -1;
	if ( SimpleDialog::CallParams.Clicked && 0<=SimpleDialog::CallParams.Clicked->AutoIndexInArray) {
		_id = SimpleDialog::CallParams.Clicked->AutoIndexInArray;
		delGroup(_id);
	};
	updateGroupList();
	if ( _groupList.GetAmount()>0 ){
		if ( _id>=_groupList.GetAmount() ) _id = _groupList.GetAmount()-1;
	}else{
		_id = -1;
	};
	selectGroup(_id,true);
};
void	be_CGroups::aplayChangesForSelectedGroup(){
	be_CGROUP* pGR = (be_CGROUP*)(_group.Get());
	if ( pGR!=NULL ){
		// Name
		if ( pGR->Name!=_nameGroup ){
			pGR->SetName( _nameGroup.pchar() );
			updateGroupList();
		};
	};	
};
bool	be_CGroups::processActions(bool& LeftPressed,int MapCoordX,int MapCoordY){
	if ( _selectOnMap && ( !mouseOnDD() ) ) return processActionSelect(LeftPressed,MapCoordX,MapCoordY);
	return false;
};
bool	be_CGroups::mouseOnDD(){
	return _dialog.mouseOn();		
};
bool	be_CGroups::processActionSelect(bool& LeftPressed,int MapCoordX,int MapCoordY){
	if ( LeftPressed ){
		int selN = NSL[MyNation];
		if (selN==0)	return false;
		word* pSelIDS = Selm[MyNation];
		OneObject* pOB = NULL;
		for (int i=0; i<selN; i++){
			if ( pSelIDS[i]!=0xFFFF && pSelIDS[i]!=0xFFFE ){
				pOB = Group[ pSelIDS[i] ];
				if ( pOB!=NULL && pOB->GRP_N>0 ){
					be_CGROUP* pGR = g_beVAR_STORE.GROUP_LIST.GetByGID(pOB->pGRP_IDS[0]);
					if (pGR!=NULL) {
						for ( int j=0; j<_groupList.GetAmount(); j++ ){
							if ( _groupList[j]->_groupName==pGR->Name ){
								selectGroup(j,true);
								ClearSelection(MyNation);
								return true;
							};
						};
					};
				};
			};
		};
	};
	return false;	
};
void	be_CGroups::clearGroup(){
	be_CGROUP* pGR = (be_CGROUP*)(_group.Get());
	if ( pGR!=NULL ){
		pGR->RemAllUnits();
	};
};
void	be_CGroups::FN_CLEAR_GR(){
	clearGroup();
};
void	be_CGroups::groupOnScreen(int _id/*=-1*/){
	be_CGROUP* pGR = NULL;
	if ( _id==-1 ){
		pGR = (be_CGROUP*)(_group.Get());
	}else{
		if ( 0<=_id && _id<_groupList.GetAmount() ){
			pGR = g_beVAR_STORE.GROUP_LIST.GetByName( _groupList[_id]->_groupName.pchar() );
		};
	};
	if ( pGR!=NULL ) pGR->ScreenOnGroup();
};
void	be_CGroups::FN_ON_SCREEN_GR_ID(){
	if ( SimpleDialog::CallParams.Clicked && 0<=SimpleDialog::CallParams.Clicked->AutoIndexInArray) {
		groupOnScreen(SimpleDialog::CallParams.Clicked->AutoIndexInArray);
	};
};
void	be_CGroups::FN_ON_SCREEN_GR(){
	groupOnScreen(-1);
};
//////////////////////////////////////////////////////////////////////////
// be_CGroupsDriver	//////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool	be_CGroupsDriver::Process	(
									int mx,
									int my,
									bool& LeftPressed,
									bool& RightPressed,
									int MapCoordX,
									int MapCoordY,
									bool OverMiniMap
									)
{
	return g_beGroupsEditor.Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap);		
};
void	be_CGroupsDriver::clear(){
	g_beGroupsEditor.clear();
};
void	be_CGroupsDriver::GetObjectGlobalID(){
	g_beGroupsEditor.GetObjectGlobalID();
};
void	be_CGroupsDriver::setVisible(bool _state){
	g_beGroupsEditor.setVisible(_state);
};
bool	be_CGroupsDriver::getVisible(){
	return g_beGroupsEditor.getVisible();
};
void	be_CGroupsDriver::OnInit(){
	g_beGroupsEditor._dialog.OnInit();
};



//*********************************************************************//
//******************   SUB FUNCTION MANAGER   *************************//
//*********************************************************************//
be_CSubfunction	g_beSubfunctionEditor("SubfunctionEditor");
// be_CSubfunctionDialog ////////////////////////////////////////////////
be_CSubfunctionDialog::be_CSubfunctionDialog(){
	pDD_PARAMS		= NULL;
	pDD_REF_INT		= NULL;
	pDD_VAL_INT		= NULL;
	pDD_REF_BOOL	= NULL;
	pDD_VAL_BOOL	= NULL;
	pDD_REF_NODE	= NULL;
	pDD_REF_GROUP	= NULL;
	pDD_REF_ARRAY	= NULL;
};
be_CSubfunctionDialog::~be_CSubfunctionDialog(){
};
void	be_CSubfunctionDialog::OnInit(){
	be_CBaseDialog::OnInit("Missions\\GRAPHICS\\TOOLS\\Dialog\\SFE.DialogsSystem.xml");
	if ( !_error ){
		pDD_PARAMS		= dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_PARAMS") );
		pDD_REF_INT		= dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_REF_INT") );
		pDD_VAL_INT		= dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_VAL_INT") );
		pDD_REF_BOOL	= dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_REF_BOOL") );
		pDD_VAL_BOOL	= dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_VAL_BOOL") );
		pDD_REF_NODE	= dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_REF_NODE") );
		pDD_REF_GROUP	= dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_REF_GROUP") );
		pDD_REF_ARRAY	= dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_REF_ARRAY") );
	};
};
bool	be_CSubfunctionDialog::mouseOn(){
	return false;
};
void	be_CSubfunctionDialog::updateClassEditor(){
	be_CBaseDialog::updateClassEditor(pDD_PARAMS);
	be_CBaseDialog::updateClassEditor(pDD_REF_INT);
	be_CBaseDialog::updateClassEditor(pDD_VAL_INT);
	be_CBaseDialog::updateClassEditor(pDD_REF_BOOL);
	be_CBaseDialog::updateClassEditor(pDD_VAL_BOOL);
	be_CBaseDialog::updateClassEditor(pDD_REF_NODE);
	be_CBaseDialog::updateClassEditor(pDD_REF_GROUP);
	be_CBaseDialog::updateClassEditor(pDD_REF_ARRAY);
};
void*	be_CSubfunctionDialog::getSelectedElementSRC(){
	if ( !_error && pDD_PARAMS!=NULL ){
		ClassEditorConnector* pCEC = pDD_PARAMS->UseClassEditor.Get();
		if (pCEC&&pCEC->CE) {
			return pCEC->CE->GetCurrentSelectedPtr();
		};
	};
	return NULL;
};
// be_CSubfunction //////////////////////////////////////////////////////
void	be_CSubfunction::init(char* _name/*=NULL*/){
	Name = ( (_name!=NULL&&_name[0]!=0) ? (_name) : ("SubfunctionEditorExemple") );
	setVisible(false);
	enterNameDD = false;
	subFN_Name = "NONAME";
	subFN_ref.Set(NULL);
	subFN_Params.Clear();

	paramIsLocal	= true;	
	setParamFalse();

	selectOnMap = false;

	pLastSelection = NULL;
	updateSel = false;

	ref_INT.myMask		= 0x00000001;
	ref_BOOL.myMask		= 0x00000002;
	ref_NODE.myMask		= 0x00000004;
	ref_GROUP.myMask	= 0x00000008;
	ref_ARRAY.myMask	= 0x00000010;
	val_INT.myMask		= 0x00000020;
	val_BOOL.myMask		= 0x00000040;

	editLocalNODE	= false;
	editLocalGROUP	= false;
};
be_CSubfunction::be_CSubfunction(){
	init();
};
be_CSubfunction::be_CSubfunction(char* _name){
	init(_name);
};
void	be_CSubfunction::clear(){
	
};
be_CSubfunction::~be_CSubfunction(){
	clear();
};
bool	be_CSubfunction::Process(
									int mx,
									int my,
									bool& LeftPressed,
									bool& RightPressed,
									int MapCoordX,
									int MapCoordY,
									bool OverMiniMap
								)
{
	processDialogs();
	_dialog.OnRender();
	g_fWheelIsBlocked=true;
	if ( _visible==false || editLocalNODE==false ) g_fWheelIsBlocked=false;
	if ( _visible && !enterNameDD ){
		if ( selectOnMap ){
			pImSelected	selUN = pImSelected::units(MyNation);
			RawUnit		unit;
			OneObject*	pOB;
			DynArray<OneClassPointer*>& CPA=CPGARB.CPointer[CPGARB.GetBranch("be_CGROUP")];
			int TCN = CPA.GetAmount();
			ReferableBaseClass* pRefOB = NULL;
			for ( int i=0; i<selUN._getamount(); i++ ){
				pOB = selUN._getunit(i).object();
				if ( pOB!=NULL && pOB->GRP_N>0 ){
					for ( int j=0; j<pOB->GRP_N; j++ ){
						if ( pOB->pGRP_IDS[j]!=0xFFFF && pOB->pGRP_IDS[j]!=0xFFFE ){
							while (TCN--) {
								OneClassPointer* CP=CPA[TCN];
								if (CP!=NULL&&CP->ClassPtr!=NULL){
									pRefOB = (ReferableBaseClass*)CP->ClassPtr;
									if ( pOB->pGRP_IDS[i] == pRefOB->GetObjectGlobalID() ){
										be_CSUBFUNCTION* pSF = g_beVAR_STORE.getSubFby_GROUP( (be_CGROUP*)pRefOB );
										if ( pSF ){
											selectOnMap = false;
										//	if (subFN_ptr!=NULL) subFN_ptr->reset_class(subFN_ptr);
											subFN_ptr = pSF;
											subFN_ref.Set(pSF);
											
											subFN_Name = subFN_ptr->GetName();
											subFN_Params.Clear();
											int NFN = subFN_ptr->INIT.FirstOperand.GetAmount();
											be_CFunctionBase* pFN = NULL;
											for ( int f=0; f<NFN; f++ ){
												pFN = (be_CFunctionBase*)(subFN_ptr->INIT.FirstOperand[f]);
												if ( pFN!=NULL && strcmp(pFN->GetClassName(),"be_CRefParamFunction")==0 ){
													be_CRefParamFunction* pRFN = (be_CRefParamFunction*)pFN;
													if ( strcmp(pRFN->CallingFunctionName.pchar(),"SET_AddUserVariable")==0 ) {
														for ( int i=0; i<pRFN->InArgList.GetAmount(); i++ ){

															_str refVarName = ((be_CBasePrametr*)(pRFN->InArgList[i]))->UniveraslParam.GlobalReference.pchar();
															if ( g_beCheckChar(refVarName.pchar()) ){
																for ( int pi=0; pi<subFN_ptr->InArgList.GetAmount(); pi++ ){
																	if ( subFN_ptr->InArgList[pi]->ParamName == refVarName ){
																		subFN_Params.add( (be_CBasePrametr*)(subFN_ptr->InArgList[pi]) );
																		pi = subFN_ptr->InArgList.GetAmount();
																	}
																}
															}

														};
													}
												};
											};

											_dialog.updateClassEditor();
											return true;
										}
									}
								}
							}		
						}
					}
				}
			}
		}else{
			// Edit local NODE
			if ( paramIsLocalNODE ){
				val_NODE.nodeValue.vDraw();
				if ( editLocalNODE ){
					val_NODE.nodeValue.vSetXY(MapCoordX,MapCoordY);
					// update roll button
					if(WheelDelta){
						val_NODE.nodeValue.vSetR( val_NODE.nodeValue.vGetR() + WheelDelta/12 );
						WheelDelta=0;
					};
					if ( LeftPressed ) editLocalNODE=false;
				};
			};
			// Edit local GROUP
			if ( paramIsLocalGROUP ){
				val_GROUP.groupValue.vDraw();
			};
			// Draw local object of current SF
			darwMyObjects();
		};
	};
	return false;									
};
void	be_CSubfunction::darwMyObjects(){
	subFN_Params.derawME();
};
void	be_CSubfunction::processDialogs(){
	setParamType();
};
void	be_CSubfunction::setVisible(bool _state){
	_visible = _state;
};
bool	be_CSubfunction::getVisible(){
	return _visible;
};
void	be_CSubfunction::FN_HIDE(){
	setVisible(false);
};
void	be_CSubfunction::FN_OPENFILE(){
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	HWND hwnd=0;            // owner window
//	HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0XML\0*.xml\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_CREATEPROMPT;

	// Display the Open dialog box. 
	_str	_FILE_NAME = "";
	if (GetOpenFileName(&ofn)==TRUE){
		_FILE_NAME = (char*)(ofn.lpstrFile);
	};		
	loadSubFunction(_FILE_NAME.pchar());
};
bool	be_CSubfunction::loadSubFunction(char* fileName){
	if ( g_beCheckChar(fileName) ) {
		if (subFN_ptr!=NULL) subFN_ptr->reset_class(subFN_ptr);
		if (subFN_ptr==NULL) subFN_ptr = new be_CSUBFUNCTION();
		subFN_ref.Set(subFN_ptr);
		if ( subFN_ptr->ReadFromFile(fileName) ){
			subFN_ptr->GetObjectGlobalID();
			subFN_ptr->ACTIVE = true;
			subFN_Name = subFN_ptr->GetName();
	//		g_beVAR_STORE.SUBF_LIST.Add(pnewSF);
			subFN_Params.Clear();

			int NFN = subFN_ptr->INIT.FirstOperand.GetAmount();
			be_CFunctionBase* pFN = NULL;
			for ( int f=0; f<NFN; f++ ){
				pFN = (be_CFunctionBase*)(subFN_ptr->INIT.FirstOperand[f]);
				if ( pFN!=NULL && strcmp(pFN->GetClassName(),"be_CRefParamFunction")==0 ){
					be_CRefParamFunction* pRFN = (be_CRefParamFunction*)pFN;
					if ( strcmp(pRFN->CallingFunctionName.pchar(),"SET_AddUserVariable")==0 ) {
						for ( int i=0; i<pRFN->InArgList.GetAmount(); i++ ){

							_str refVarName = ((be_CBasePrametr*)(pRFN->InArgList[i]))->UniveraslParam.GlobalReference.pchar();
							if ( g_beCheckChar(refVarName.pchar()) ){
								for ( int pi=0; pi<subFN_ptr->InArgList.GetAmount(); pi++ ){
									if ( subFN_ptr->InArgList[pi]->ParamName == refVarName ){
										subFN_Params.add( (be_CBasePrametr*)(subFN_ptr->InArgList[pi]) );
										pi = subFN_ptr->InArgList.GetAmount();
									}
								}
							}

						};
					}
				};
			};

			subFN_ref.Set(subFN_ptr);
			_dialog.updateClassEditor();
	//		pnewSF = NULL;
			return true;
		};
	};
	subFN_ref.Set(NULL);
	subFN_Name = "NONAME";
	return false;
};
void	be_CSubfunction::FN_ADD(){
	enterNameDD = true;
};
void	be_CSubfunction::FN_OK(){
	enterNameDD = false;
	if ( g_beVAR_STORE.SUBF_LIST.GetIDByNAME(subFN_Name.pchar())!=-1 ){
		if ( MessageBox(NULL,"Already Present !!! Replace ?","ATTENTION",MB_OKCANCEL)==IDCANCEL ) return;
		else	g_beVAR_STORE.SUBF_LIST.DelElement(g_beVAR_STORE.SUBF_LIST.GetIDByNAME(subFN_Name.pchar()));
	};
	subFN_ptr->SetName(subFN_Name.pchar());
	g_beVAR_STORE.SUBF_LIST.Add(subFN_ptr);	
	subFN_ptr=NULL;
};
void	be_CSubfunction::FN_CANCEL(){
	enterNameDD = false;
};
void	be_CSubfunction::setParamFalse(){
	paramIsLocalINT		=	paramIsGlobalINT	= false;
	paramIsLocalBOOL	=	paramIsGlobalBOOL	= false;
	paramIsLocalNODE	=	paramIsGlobalNODE	= false;
	paramIsLocalGROUP	=	paramIsGlobalGROUP	= false;
	paramIsLocalARRAY	=	paramIsGlobalARRAY	= false;	
};
be_CSubfunction::sf_PT	be_CSubfunction::getParamType(){
	void* pSelParam = _dialog.getSelectedElementSRC();
	if ( pSelParam!=pLastSelection ) { pLastSelection=pSelParam; updateSel=true; }
	if ( pSelParam!=NULL ){
		be_CSF_PARAM_WRAP* pPW = (be_CSF_PARAM_WRAP*)pSelParam;
		be_CPrametrBase*   pPR = (be_CPrametrBase*)(pPW->refParam.Get());
		if (pPR!=NULL) {
			if ( pPR->ParamType == "INT"	)	return sf_INT;
			if ( pPR->ParamType == "BOOL"	)	return sf_BOOL;
			if ( pPR->ParamType == "NODE"	)	return sf_NODE;
			if ( pPR->ParamType == "GROUP"	)	return sf_GROUP;
			if ( pPR->ParamType == "ARRAY"	)	return sf_ARRAY;
		};
	};
	return	sf_UNKNOWN;	
};
void	be_CSubfunction::setParamType(){
	setParamFalse();
	sf_PT	parType = getParamType();
	switch(parType) {
	case sf_INT:
		( (paramIsLocal) ? (paramIsLocalINT=true) : (paramIsGlobalINT=true) );
		break;
	case sf_BOOL:
		( (paramIsLocal) ? (paramIsLocalBOOL=true) : (paramIsGlobalBOOL=true) );
		break;
	case sf_NODE:
		( (paramIsLocal) ? (paramIsLocalNODE=true) : (paramIsGlobalNODE=true) );
		break;
	case sf_GROUP:
		( (paramIsLocal) ? (paramIsLocalGROUP=true) : (paramIsGlobalGROUP=true) );
		break;
	case sf_ARRAY:
		( (paramIsLocal) ? (paramIsLocalARRAY=true) : (paramIsGlobalARRAY=true) );
		break;
	};
	if ( updateSel ){
		updateSel = false;
		_dialog.updateClassEditor();
	}
};
void	be_CSubfunction::confirmSelectedParameterData(){
	void* pSelParam = _dialog.getSelectedElementSRC();
	be_CSUBFUNCTION* pSF = (be_CSUBFUNCTION*)subFN_ref.Get();
	if ( pSelParam!=NULL && pSF!=NULL ){
		be_CSF_PARAM_WRAP* pPW = (be_CSF_PARAM_WRAP*)pSelParam;
		if ( pPW==NULL )	return;
		be_CBasePrametr*   pPR = (be_CBasePrametr*)(pPW->refParam.Get());
		if ( pPR==NULL )	return;
		sf_PT	parType = getParamType();
		if (paramIsLocal) {
			pPR->UniveraslParam.SetLocal(true);
			switch(parType) {
			case sf_INT:
				{
				be_CINT* pINT = (be_CINT*)pPR->UniveraslParam.LocalObject.Get();
				if ( pINT==NULL ) return;
				pINT->SetValue( val_INT.intValue );
				}
				break;
			case sf_BOOL:
				{
				be_CBOOL* pBOOL = (be_CBOOL*)pPR->UniveraslParam.LocalObject.Get();
				if ( pBOOL==NULL ) return;
				pBOOL->SetValue( val_BOOL.boolValue );
				}
				break;
			case sf_NODE:
				{
				be_CNODE* pNODE = (be_CNODE*)pPR->UniveraslParam.LocalObject.Get();
				if ( pNODE==NULL ) return;
				pNODE->vSetXY( val_NODE.nodeValue.vGetX(), val_NODE.nodeValue.vGetY() );
				pNODE->vSetR( val_NODE.nodeValue.vGetR() );
				}
				break;
			case sf_GROUP:
				{
				be_CGROUP* pGROUP = (be_CGROUP*)pPR->UniveraslParam.LocalObject.Get();
				if ( pGROUP==NULL ) return;
				pGROUP->RemAllUnits();
				val_GROUP.groupValue.RemoveNUnitsToCGroup( pGROUP );
				}
				break;
			case sf_ARRAY:
			//	pPR->UniveraslParam.GlobalReference.Set( ref_ARRAY.ref.Get() );
				break;
			};
		}else{
			pPR->UniveraslParam.SetLocal(false);
			switch(parType) {
			case sf_INT:
				pPR->UniveraslParam.GlobalReference.Set( ref_INT.ref.Get() );
				break;
			case sf_BOOL:
				pPR->UniveraslParam.GlobalReference.Set( ref_BOOL.ref.Get() );
				break;
			case sf_NODE:
				pPR->UniveraslParam.GlobalReference.Set( ref_NODE.ref.Get() );
				break;
			case sf_GROUP:
				pPR->UniveraslParam.GlobalReference.Set( ref_GROUP.ref.Get() );
				break;
			case sf_ARRAY:
				pPR->UniveraslParam.GlobalReference.Set( ref_ARRAY.ref.Get() );
				break;
			};
		}
		pPW->UpdateSTR();
		_dialog.updateClassEditor();
	};
};
void	be_CSubfunction::FN_CLEAR_GR(){
	val_GROUP.groupValue.RemAllUnits();	
};
void	be_CSubfunction::FN_ADD_SEL_GR(){
	val_GROUP.groupValue.AddSelectedUnits();
};
void	be_CSubfunction::FN_REM_SEL_GR(){
	val_GROUP.groupValue.RemSelectedUnits();
};
//////////////////////////////////////////////////////////////////////////
// be_CSubfunctionDriver	//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool	be_CSubfunctionDriver::Process(
										int mx,
										int my,
										bool& LeftPressed,
										bool& RightPressed,
										int MapCoordX,
										int MapCoordY,
										bool OverMiniMap
									  )
{
	return g_beSubfunctionEditor.Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap);
};
void	be_CSubfunctionDriver::clear(){
	g_beSubfunctionEditor.clear();
};
void	be_CSubfunctionDriver::GetObjectGlobalID(){
	g_beSubfunctionEditor.GetObjectGlobalID();
};
void	be_CSubfunctionDriver::setVisible(bool _state){
	g_beSubfunctionEditor.setVisible(_state);
};
bool	be_CSubfunctionDriver::getVisible(){
	return g_beSubfunctionEditor.getVisible();
};
void	be_CSubfunctionDriver::OnInit(){
	g_beSubfunctionEditor._dialog.OnInit();
};
//*********************************************************************//
//**********************   LEGO MANAGER   *****************************//
//*********************************************************************//
be_CLego	g_beLego("Lego");
// be_CLegoDialog ////////////////////////////////////////////////////////
be_CLegoDialog::be_CLegoDialog(){
};
be_CLegoDialog::~be_CLegoDialog(){
	
};
void	be_CLegoDialog::OnInit(){
	be_CBaseDialog::OnInit("Missions\\GRAPHICS\\TOOLS\\Dialog\\Lego.DialogsSystem.xml");
};
// be_CLego //////////////////////////////////////////////////////////////
be_CLego::be_CLego(){
	_visible = false;
	_active = false;
};
be_CLego::be_CLego(char* _name){
	Name = _name;
	_visible = false;
	_active = false;
};
be_CLego::~be_CLego(){
	
};
bool	be_CLego::Process(
							int mx,
							int my,
							bool& LeftPressed,
							bool& RightPressed,
							int MapCoordX,
							int MapCoordY,
							bool OverMiniMap
						 )
{
	processDialogs();
	_dialog.OnRender();
	return false;
};
void	be_CLego::processDialogs(){
	_visible = _active;
	if ( _active ){
		_visible =	!(
						g_beGroupsEditor.getVisible()	||
						g_beNodesEditor.getVisible()	||
						g_beNodeArrayEditor.getVisible()||
						g_beDebuger.getVisible()		||
						g_beSubfunctionEditor.getVisible()
					 );
	};
};
void	be_CLego::FN_HIDE(){
	_active = false;
};
void	be_CLego::FN_DEBUG(){
	g_WCP_EDITOR.LoadDEFMASK();
	g_beDebuger._active = false;
	g_beDebuger.setVisible(true);
}
void	be_CLego::FN_GROUP(){
	g_WCP_EDITOR.LoadDEFMASK();
	be_CGroupsDriver::setVisible(true);
}
void	be_CLego::FN_NODE(){
	g_WCP_EDITOR.LoadDEFMASK();
	be_CNodesDriver::setVisible(true);
}
void	be_CLego::FN_ARRAY(){
	g_WCP_EDITOR.LoadDEFMASK();
	be_CNodeArrayDriver::setVisible(true);
}
void	be_CLego::FN_SUBFN(){
	be_CSubfunctionDriver::setVisible(true);
};
// be_CLegoDriver ///////////////////////////////////////////////////////
bool	be_CLegoDriver::Process(
									int mx,
									int my,
									bool& LeftPressed,
									bool& RightPressed,
									int MapCoordX,
									int MapCoordY,
									bool OverMiniMap
									)
{
	return g_beLego.Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap);
};
void	be_CLegoDriver::clear(){
	
};
void	be_CLegoDriver::GetObjectGlobalID(){
	g_beLego.GetObjectGlobalID();
};
void	be_CLegoDriver::setVisible(bool _state){
	g_beLego._active = _state;
};
bool	be_CLegoDriver::getVisible(){
    return g_beLego._active;
}
void	be_CLegoDriver::OnInit(){
	g_beLego._dialog.OnInit();
};
//////////////////////////////////////////////////////////////////////////

void	be_REG_CLASS_NodesArray(){
	REG_CLASS(be_CArrayLine);
	REG_CLASS(be_CNodeArray);

	REG_CLASS(be_CNodeLine);
	REG_CLASS(be_CNodes);

	REG_CLASS(be_CGroupLine);
	REG_CLASS(be_CGroups);

	REG_CLASS(be_CSubfunction);

	REG_CLASS(be_CLego);
};











//////////////////////////////////////////////////////////////////////////

























