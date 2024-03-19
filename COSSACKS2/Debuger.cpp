#include "stdheader.h"
#include "BE_HEADERS.h"

be_CDebuger	g_beDebuger("Debuger");

//////////////////////////////////////////////////////////////////////////
/////////////////////////// be_CDebugerDialog ////////////////////////////
//////////////////////////////////////////////////////////////////////////
be_CDebugerDialog::be_CDebugerDialog(){
	_error = false;
	_init  = true;
	pCD_TXT = NULL;
	pDD_SRC = NULL;
	pDD_WATCH = NULL;
	pDD_SMARTBP = NULL;
	pDD_FILE = NULL;

	_state = __dsDEBUGER;
};
be_CDebugerDialog::~be_CDebugerDialog(){
	
};
void	be_CDebugerDialog::setWatchVisibility(bool _state){
	if (pDD_WATCH!=NULL) pDD_WATCH->Visible = _state;
};
void	be_CDebugerDialog::setSmartBPVisibility(bool _state){
	if (pDD_SMARTBP!=NULL) pDD_SMARTBP->Visible = _state;
};
bool	be_CDebugerDialog::OnRender(){
	if ( _error ) return false;
	if ( _init ) OnInit();
	if ( !_error ){
		g_beDebuger._WATCH_LIST.update();
		g_beDebuger._SMARTBP_LIST.check(false);
		switch(_state) {
		case __dsDEBUGER:
			{
				_mainDS.ProcessDialogs();
				while ( g_beDebuger.getVisible() ) {
					// Additional windows
					setWatchVisibility( g_beDebuger._watch );
					setSmartBPVisibility( g_beDebuger._smartBP );
					if ( g_beDebuger._watch || g_beDebuger._smartBP ){
						if ( pDD_TXT!=NULL && pDD_TXT->GetHeight()!=385 ){
							pDD_TXT->SetHeight(385);
							pCD_TXT->SetHeight(385);
						}
						if ( pDD_TXT!=NULL && pDD_SRC->GetHeight()!=385 ) pDD_SRC->SetHeight(385);
					}else{
						if ( pDD_TXT!=NULL && pDD_TXT->GetHeight()!=570 ){
							pDD_TXT->SetHeight(570);
							pCD_TXT->SetHeight(570);
						};
						if ( pDD_TXT!=NULL && pDD_SRC->GetHeight()!=570 ) pDD_SRC->SetHeight(570);
					}
					/////////////////////
					// HOT KEY PROCESS //
					/////////////////////
				//	g_beDebuger.ProcessHotKey();
					/////////////////////
					ProcessMessages();
					_mainDS.ProcessDialogs();
					_mainDS.RefreshView();
					if ( g_beDebuger._active && g_beDebuger._stepByStepState ){
						// can get away by click
						if ( g_beDebuger._nextCall ){
							g_beDebuger._nextCall = false;
							// Interapt if last index in caller debuger
							if ( g_beDebuger._selected_id==g_beDebuger._SCRIPT_TXT_LIST.GetAmount()-1 ){
								g_beDebuger._interrupt = true;
							};
							return false;
						};
						if ( g_beDebuger._interrupt ){
							g_beDebuger._interrupt = false;
							g_beDebuger._stepByStepState = false;
							g_beDebuger.setVisible(false);
							return false;
						};
					};
					if (_state!=__dsDEBUGER) g_beDebuger.setVisible(false);
				};
			}
			break;
		case __dsVALUE:
			{
				g_WCP_EDITOR.LoadDEFMASK();
				g_beVAR_STORE.UPDATE();
				be_CCAMERA::HideCameraTracks=true;
				BE_USAGE_FLAGS	OLD_FLAG = g_BE_USAGE_FLAG;
				g_BE_USAGE_FLAG = BE_IF_USE;
				SimpleEditClass("VARIABLES", (BaseClass*)(&g_beVAR_STORE) );
				g_BE_USAGE_FLAG = OLD_FLAG;
				be_CCAMERA::HideCameraTracks=false;
				// for recreate all enumerators 
				g_beOBJ_CONTROLER.SetState("be_CVariableBase"); 
				_state = __dsDEBUGER;
				g_beDebuger.setVisible(true);
			}
			break;
		case __dsFUNCTION:
			{
				g_WCP_EDITOR.LoadDEFMASK();
				BE_USAGE_FLAGS	OLD_FLAG = g_BE_USAGE_FLAG;
				g_BE_USAGE_FLAG = BE_IF_DEFF;
				SimpleEditClass("FUNCIONS", (BaseClass*)(&g_BE_FN_STORE) );
				g_BE_USAGE_FLAG = OLD_FLAG;
				_state = __dsDEBUGER;
				g_beDebuger.setVisible(true);		
			}
			break;
		case __dsWATCH:
			{
				g_WCP_EDITOR.LoadDEFMASK();
				SimpleEditClass("WATCH EDITOR",(BaseClass*)(&(g_beDebuger._WATCH_LIST)));
				_state = __dsDEBUGER;
				g_beDebuger.setVisible(true);
			}
			break;
		case __dsSMARTBP:
			{
				g_WCP_EDITOR.LoadDEFMASK();
				SimpleEditClass("SMART BREAK POINT EDITOR",(BaseClass*)(&(g_beDebuger._SMARTBP_LIST)));
				_state = __dsDEBUGER;
				g_beDebuger.setVisible(true);
			}
			break;
		}
	};
	return true;
};
void	be_CDebugerDialog::OnInit(){
	_error = !loadDS();
	if ( !_error ){
		_init = false;
		pCD_TXT = _mainDS.Find("CD_TXT");
		pDD_SRC = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_SRC") );
		pDD_TXT = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_TXT") );
		pDD_WATCH   = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_WTCH") );
		pDD_SMARTBP = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_SMARTBP") );
		pDD_FILE = dynamic_cast< DialogsDesk* >( _mainDS.Find("DD_FILE") );
		setWatchVisibility(false);
		setSmartBPVisibility(false);
	};
};
bool	be_CDebugerDialog::loadDS(){
	_mainDS.reset_class(&_mainDS);
	return	_mainDS.ReadFromFile("Missions\\GRAPHICS\\TOOLS\\Dialog\\Debuger.DialogsSystem.xml");
};
void	be_CDebugerDialog::V_scrollTXT_ToElement(int _id){
	if ( !_error && pCD_TXT!=NULL ){
		pCD_TXT->V_scrollToElement(_id);
	};
};
void*	be_CDebugerDialog::getSelectedElementSRC(){
	if ( !_error && pDD_SRC!=NULL ){
		ClassEditorConnector* pCEC = pDD_SRC->UseClassEditor.Get();
		if (pCEC&&pCEC->CE) {
			return pCEC->CE->GetCurrentSelectedPtr();
		};
	};
	return NULL;
};
void	be_CDebugerDialog::setSelectedElementSRC(void* prt){
	if ( prt==NULL ) return;
	if ( !_error && pDD_SRC!=NULL ){
		ClassEditorConnector* pCEC = pDD_SRC->UseClassEditor.Get();
		if (pCEC&&pCEC->CE) {
			for ( int i=0; i<pCEC->CE->ElParams.GetAmount(); i++ ){
				if ( pCEC->CE->ElParams[i]->DataPtr == prt ){
					pCEC->CE->CurElement = i;
				};
			};
		};
	};
}
void	be_CDebugerDialog::updateClassEditor(){
	if ( !_error && pDD_SRC!=NULL ){
		ClassEditorConnector* pCEC = pDD_SRC->UseClassEditor.Get();
		if (pCEC&&pCEC->CE) {
			pCEC->CE->ExpInfChanged=true;
			pCEC->CE->Process();
		};
	};	
	if ( !_error && pDD_FILE!=NULL ){
		ClassEditorConnector* pCEC = pDD_FILE->UseClassEditor.Get();
		if (pCEC&&pCEC->CE) {
			pCEC->CE->ExpInfChanged=true;
			pCEC->CE->Process();
		};
	};	
};
void	be_CDebugerDialog::scrollSRC_ToSelectedElement(){
	if ( !_error && pDD_SRC!=NULL ){
		ClassEditorConnector* pCEC = pDD_SRC->UseClassEditor.Get();
		if (pCEC&&pCEC->CE) {
			pCEC->CE->setScrollerOnSelectedElement();
		};
	};	
};
//////////////////////////////////////////////////////////////////////////
/////////////////////////// be_CDebugerLine //////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define __ADD_RETT_HUCK__
void	be_CDebugerLine::init(char* _text/*=NULL*/){
	this->_text = ( (_text!=NULL) ? (_text) : ("") );	
#ifdef __ADD_RETT_HUCK__
	this->_text += "\n";
#endif//__ADD_RETT_HUCK__
	_selected	= false;
	_breakPoint = false;
};
be_CDebugerLine::be_CDebugerLine(){
	init();
};
be_CDebugerLine::be_CDebugerLine(char* _text){
	init(_text);
};
be_CDebugerLine::be_CDebugerLine(char* _text,be_CFunctionBase* pFN){
	init(_text);
	this->pFN = pFN;
};
void	be_CDebugerLine::switchBreakPoint(){
	_breakPoint = !_breakPoint;	
};
//////////////////////////////////////////////////////////////////////////
///////////////////////// be_CWatchLineList //////////////////////////////
//////////////////////////////////////////////////////////////////////////
be_CWatchLine::be_CWatchLine(){
	_TXT = "";
	_RET = "";
	_FN.Set(NULL);
};
be_CWatchLine::~be_CWatchLine(){
	_TXT.Clear();
	_RET.Clear();
	_FN.Clear();
};
void	be_CWatchLine::update(){
	be_CFunctionBase* pFN = (be_CFunctionBase*)(_FN.Get());	
	if (pFN) {
		_TXT = "";
		CE_WCP.GetPresentation(_TXT,pFN);
#ifdef __ADD_RETT_HUCK__
		_TXT += "\n";
#endif//__ADD_RETT_HUCK__
		_RET = pFN->CallFunction();
	}else{
		_TXT = "...";
		_RET = "...";
	};
};
void	be_CWatchLineList::update(){
	be_CWatchLine* pWL = NULL;
	for ( int i=0; i<GetAmount(); i++ ){
		pWL = (be_CWatchLine*)((*this)[i]);
		if ( pWL ) pWL->update();
	}
};
//////////////////////////////////////////////////////////////////////////
///////////////////////// be_CSmartBPLineList ////////////////////////////
//////////////////////////////////////////////////////////////////////////
be_CSmartBPLine::be_CSmartBPLine(){
	_CONDITION.Set(NULL);
	_TXT = "";
	_active = false;
	_breakON = false;
};
be_CSmartBPLine::~be_CSmartBPLine(){
	_CONDITION.Clear();
	_TXT.Clear();
};
bool	be_CSmartBPLine::check(bool _callConditions/*=true*/){
	_TXT = "???";
	be_CFunctionBase* pFN = (be_CFunctionBase*)(_CONDITION.Get());
	if (pFN) {
		CE_WCP.GetPresentation(_TXT,pFN);
#ifdef __ADD_RETT_HUCK__
		_TXT += "\n";
#endif//__ADD_RETT_HUCK__
		if (_callConditions&&_active) _breakON = (bool)(pFN->CallFunction());
		else _breakON = false;
	}else{
		_breakON = false;
	}
	return _breakON;
};
bool	be_CSmartBPLineList::check(bool _callConditions/*=true*/){
	bool l_breakNow = false;
	be_CSmartBPLine* pSBP = NULL;
	for ( int i=0; i<GetAmount(); i++ ){
		pSBP = (be_CSmartBPLine*)((*this)[i]);
		if ( pSBP ) l_breakNow = l_breakNow || pSBP->check(_callConditions);
	};
	return l_breakNow;
};
//////////////////////////////////////////////////////////////////////////
///////////////////////////// be_CDebuger ////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void	be_CDebuger::clear(){
	_SCRIPT_TXT_LIST.Clear();
	_WATCH_LIST.Clear();
	_SMARTBP_LIST.Clear();
	_curSRC.Set(NULL);
};
be_CDebuger::~be_CDebuger(){
	clear();
};
void	be_CDebuger::init(char* name/*=NULL*/){
	Name = ( (name!=NULL&&name[0]!=0) ? (name) : ("DebugerEemple") );

	_active		= false;
	_stepByStepState = false;
	_nextCall	= false;
	_interrupt	= false;
	_visible	= false;

	_smartBP = false;

	_SCRIPT_TXT = false;
	_SCRIPT_SRC = true;

	_SCRIPT_TXT_LIST.Clear();
	_updateAlways = false;

	_selected_id = -1;

	_strSearchActive=false;
	_SEARCH_RESULTS.clear();

	_curSRC.Set(NULL);

	_watch = false;

	_FILE_NAME = "NoFileName";

	lastHotKey = 0x00;
};
be_CDebuger::be_CDebuger(){
	init();
};
be_CDebuger::be_CDebuger(char* name) {
	init(name);
	GetObjectGlobalID();
};
bool	be_CDebuger::getVisible(){
	return _visible;	
};
void	be_CDebuger::setVisible(bool state){
	_visible = state;
	_dialog.setWatchVisibility( ( ( _visible ) ? ( _watch ) : ( false ) ) );
	_dialog.setSmartBPVisibility( ( ( _visible ) ? ( _smartBP ) : ( false ) ) );
	if (_visible){ 
		g_BE_USAGE_FLAG = BE_IF_DEFF;
		
		g_beSCRIPT_STORE.MAIN_SCRIPT.GetObjectGlobalID();
		g_beDebuger._curSRC.Set(&(g_beSCRIPT_STORE.MAIN_SCRIPT));
		g_beDebuger._dialog.updateClassEditor();
		LastKey = 0;
		lastHotKey = 0x00;
	}else{
		g_BE_USAGE_FLAG = BE_IF_CALL;

		g_beDebuger._curSRC.Set(NULL);
		lastHotKey = 0x00;
	};
};
void	be_CDebuger::FN_SWITCH_VISIBLE(){
	setVisible( !getVisible() );
};
void	be_CDebuger::switchTXTvsSRC(){
	_SCRIPT_TXT = !_SCRIPT_TXT;
	_SCRIPT_SRC = !_SCRIPT_SRC;
	if ( _updateAlways && _SCRIPT_TXT ){
		g_beDebuger.FillScriptTxtList( g_beDebuger._curSRC.Get() );
	};
	// Set selected in accordin position in TXT
	if ( _SCRIPT_TXT && _updateAlways ){
		void* pELEM = _dialog.getSelectedElementSRC();
		if ( pELEM ) {
			int _newSelectedID = -1;
			BaseClass* pBC = (BaseClass*)pELEM;
			while ( _newSelectedID==-1 && pBC ) {
				_newSelectedID = findeElementByPtr( pBC );
				pBC = pBC->GetParent();
			};
			if ( _newSelectedID!=-1 ) setSelection(_newSelectedID,true);
		};
	};
	// Set selected in accordin position in SRC
	if ( _SCRIPT_SRC && _selected_id!=-1 ) {
		if (ClassEditor::CurExpInf!=NULL) {
			ClassEditor::CurExpInf->Clear();
		};
		be_CDebugerLine* pDL = (be_CDebugerLine*)(_SCRIPT_TXT_LIST[_selected_id]);
		if (pDL) {
			be_CFunctionBase* pSelFN = pDL->pFN;
			if (pSelFN!=NULL) {
				bool l_needUpdateClassEditor = false;
				BaseClass* pBC = pSelFN->GetParent();
				while ( pBC ) {
					if ( pBC->CheckDirectCasting() ){
						if ( ClassEditor::CurExpInf==NULL ) ClassEditor::CurExpInf = new ExpandInfo();
						ClassEditor::CurExpInf->Add( (void*)pBC );
						l_needUpdateClassEditor=true;
					};
					pBC = pBC->GetParent();
				};
				if (l_needUpdateClassEditor){
					// Update ClassEditor State
					_dialog.updateClassEditor();
					// Set Selected Element
					_dialog.setSelectedElementSRC((void*)pSelFN);
					// Update ClassEditor State
					_dialog.updateClassEditor();
					// scroll to selected
					_dialog.scrollSRC_ToSelectedElement();
					// Update ClassEditor State
					_dialog.updateClassEditor();
				};
			};
		};
	};
};
void	be_CDebuger::FN_SCRIPT_SWICHER(){
	switchTXTvsSRC();
};
void	be_CDebuger::FN_UPDATE_TXT(){
	g_beSCRIPT_STORE.MAIN_SCRIPT.GetObjectGlobalID();
	_curSRC.Set(&(g_beSCRIPT_STORE.MAIN_SCRIPT));
	_dialog.updateClassEditor();
	FillScriptTxtList( _curSRC.Get() );
};
void	be_CDebuger::FN_UPDATE_TXT_SEL(){
	void* pELEM = _dialog.getSelectedElementSRC();
	if (pELEM) {
		BaseClass* pBC = (BaseClass*)(pELEM);
		be_CFunctionBase* pFN = NULL;
		while ( pBC ) {
			if ( pBC->CheckDirectCasting() ){
				pFN = dynamic_cast<be_CFunctionBase*>(pBC);
				if (pFN!=NULL) {
					pFN->GetObjectGlobalID();
					_curSRC.Set(pFN);
					_dialog.updateClassEditor();
					break;
				}
			};
			pBC = pBC->GetParent();
		};
	}else{
		g_beSCRIPT_STORE.MAIN_SCRIPT.GetObjectGlobalID();
		_curSRC.Set(&(g_beSCRIPT_STORE.MAIN_SCRIPT));
		_dialog.updateClassEditor();
	};
	FillScriptTxtList( _curSRC.Get() );
};
void	be_CDebuger::FillScriptTxtList(be_CFunctionBase* pSCRIPT){
	_SEARCH_RESULTS.clear();
	clearSelection();
	_SCRIPT_TXT_LIST.Clear();
	if ( pSCRIPT==NULL ) return;
	// Close all Dialogs
	ExpandInfo* pOD = ClassEditor::CurExpInf;
	ClassEditor::CurExpInf = NULL;
	_pos = 0;	
	FillScriptTxtLine(pSCRIPT);
	// back all opened dialogs
	if ( ClassEditor::CurExpInf!=NULL ) { 
		delete ClassEditor::CurExpInf;  
		ClassEditor::CurExpInf = NULL;
	};
	ClassEditor::CurExpInf = pOD;
};
void	be_CDebuger::FillBoderLines(_str& _className,_str& _mod,_str& _line1,_str& _line2,_str& _line3){
	if ( _className == "be_CBaseUnaryOperator"		) {
		if ( _mod=="NOT" ){
			_line1  += "{C FF00ff00}not(";
			_line2  += "{C FF00ff00})";
		}else
		if ( _mod=="-" ) {
			_line1  += "{C FF00ff00}-(";
			_line2  += "{C FF00ff00})";
		};
	}else
	if ( _className == "be_CBaseUnaryStatement"		) {
		if ( _mod=="AND" ){
			_line1  += "{C FF00ff00}(";
			_line2  += "{C FF00ff00}and";
			_line3  += "{C FF00ff00})";
		}else
		if ( _mod=="OR" ) {
			_line1  += "{C FF00ff00}(";
			_line2  += "{C FF00ff00}or";
			_line3  += "{C FF00ff00})";
		}else
		if ( _mod=="SUM" ) {
			_line1  += "{C FF00ff00}(";
			_line2  += "{C FF00ff00}+";
			_line3  += "{C FF00ff00})";
		}else
		if ( _mod=="-SUM" ) {
			_line1  += "{C FF00ff00}-(";
			_line2  += "{C FF00ff00}+";
			_line3  += "{C FF00ff00})";
		}else{
			_line1  += "{C FF00ff00}begin";
			_line2  += "{C FF00ff00}end";
		};
	}else
	if ( _className == "be_CBaseBinaryOperator"		) {
		_line1  += "{C FF00ff00}(";
		_line3  += "{C FF00ff00})";
		if		( _mod=="AND" )	_line2  += "{C FF00ff00}and";
		else if ( _mod=="OR"  ) _line2  += "{C FF00ff00}or";
		else if ( _mod=="-"	  ) _line2  += "{C FF00ff00}-";
		else if ( _mod=="+"	  )	_line2  += "{C FF00ff00}+";
		else if ( _mod=="*"   )	_line2  += "{C FF00ff00}*";
		else if ( _mod==">"   )	_line2  += "{C FF00ff00}>";
		else if ( _mod==">="  )	_line2  += "{C FF00ff00}>=";
		else if ( _mod=="<"   )	_line2  += "{C FF00ff00}<";
		else if ( _mod=="<="  )	_line2  += "{C FF00ff00}<=";
		else if ( _mod=="=="  )	_line2  += "{C FF00ff00}==";
		else if ( _mod=="!="  )	_line2  += "{C FF00ff00}!=";
	}else
	if ( _className == "be_CBaseBinaryStatement"	) {
		if ( _mod=="while(...) do ..." ) {
			_line1  += "{C FF00ff00}while (";
			_line2  += "{C FF00ff00}) do";
			_line3  += "{C FF00ff00}end";
		}else
		if ( _mod=="do ... while(...)" ) {
			_line1  += "{C FF00ff00}do";
			_line2  += "{C FF00ff00}while (";
			_line3  += "{C FF00ff00})";
		}else{
			_line1  += "{C FF00ff00}if (";
			_line2  += "{C FF00ff00}) then";
			_line3  += "{C FF00ff00}end";
		};
	}else
	if ( _className == "be_CBaseTrinaryOperator"	) {

	}else
	if ( _className == "be_CBaseTrinaryStatement"	) {
		_line1  += "{C FF00ff00}if (";
		_line2  += "{C FF00ff00}) then";
		_line3  += "{C FF00ff00}else";
	};
};
void	g_beAddSpaceAfterRET(_str& _src, _str& _dst, _str& _spaces){
	char* pRET = strchr(_src.pchar(),'\n');
	if ( pRET==NULL ) { _dst=_src; return; };
	int clen = (int)(pRET-_src.pchar());
	char* _first = new char[clen+2];
	strncpy(_first,_src.pchar(),clen+1);
	_first[clen+1]=0;
	_dst = _first; _dst += _spaces;
	delete _first;
	_str _last = "";
	clen = strlen(pRET+1)+1;
	_first = new char[clen];
	strncpy(_first,(pRET+1),clen-1);
	_first[clen]=0;
	_str l_src = _first;
	delete _first;
	g_beAddSpaceAfterRET(l_src, _last, _spaces);
	_dst += _last;
};
extern WholeClassPresentation CE_WCP;
void	be_CDebuger::FillScriptTxtLine(be_CFunctionBase* pSCRIPT,int _spaceN/*=0*/){
	if ( pSCRIPT==NULL ) return;
	pSCRIPT->checkModificator();
    _str _space = " ";
	for ( int s=0; s<_spaceN; s++ ) _space += "  ";
	be_CDebugerLine* pDL = NULL;
	_str _Line1 = _space; 
	_str _Line2 = _space; 
	_str _Line3 = _space; 
	_str	_className = pSCRIPT->GetClassName();
	if ( _className=="be_CBaseUnaryOperator" ){
		be_CBaseUnaryOperator* pFN = (be_CBaseUnaryOperator*)pSCRIPT;
		pFN->clearCB();
		_str _line = ""; CE_WCP.GetPresentation(_line,pFN);	
		_str _dst = "";
		g_beAddSpaceAfterRET(_line,_dst,_space);
		_Line1 += _dst;
		pDL = new be_CDebugerLine(_Line1.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		return;
	}else
	if ( _className=="be_CBaseUnaryStatement" ){
		be_CBaseUnaryStatement* pFN = (be_CBaseUnaryStatement*)pSCRIPT;
		pFN->clearCB();
		if ( pFN->Modificator=="AND" || pFN->Modificator=="OR"   || 
			 pFN->Modificator=="SUM" || pFN->Modificator=="-SUM" 
		   )
		{
			_str _line = ""; CE_WCP.GetPresentation(_line,pFN);	
			_str _dst = "";
			g_beAddSpaceAfterRET(_line,_dst,_space);
			_Line1 += _dst;
			pDL = new be_CDebugerLine(_Line1.pchar(),pFN);
			_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
			// Set callback ID
			pFN->addCB_ID(_pos-1);
			return;
		};
		FillBoderLines(_className,pFN->Modificator,_Line1,_Line2,_Line3);
		pDL = new be_CDebugerLine(_Line1.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		for ( int i=0; i<pFN->FirstOperand.GetAmount(); i++ ){
			FillScriptTxtLine( pFN->FirstOperand[i], _spaceN+1 );
		};
		pDL = new be_CDebugerLine(_Line2.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		return;
	}else
	if ( _className=="be_CBaseBinaryOperator" ){
		be_CBaseBinaryOperator* pFN = (be_CBaseBinaryOperator*)pSCRIPT;
		pFN->clearCB();
		_str _line = ""; CE_WCP.GetPresentation(_line,pFN);	
		_str _dst = "";
		g_beAddSpaceAfterRET(_line,_dst,_space);
		_Line1 += _dst;
		pDL = new be_CDebugerLine(_Line1.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		return;
	}else
	if ( _className=="be_CBaseBinaryStatement" ){
		be_CBaseBinaryStatement* pFN = (be_CBaseBinaryStatement*)pSCRIPT;
		pFN->clearCB();
		FillBoderLines(_className,pFN->Modificator,_Line1,_Line2,_Line3);
		pDL = new be_CDebugerLine(_Line1.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		FillScriptTxtLine( pFN->FirstOperand.Get(), _spaceN+1 );
		pDL = new be_CDebugerLine(_Line2.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		for ( int i=0; i<pFN->SecondOperand.GetAmount(); i++ ){
			FillScriptTxtLine( pFN->SecondOperand[i], _spaceN+1 );
		};
		pDL = new be_CDebugerLine(_Line3.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		return;
	}else
	if ( _className=="be_CBaseTrinaryStatement" ){
		be_CBaseTrinaryStatement* pFN = (be_CBaseTrinaryStatement*)pSCRIPT;
		pFN->clearCB();
		FillBoderLines(_className,pFN->Modificator,_Line1,_Line2,_Line3);
		pDL = new be_CDebugerLine(_Line1.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		FillScriptTxtLine( pFN->FirstOperand.Get(), _spaceN+1 );
		pDL = new be_CDebugerLine(_Line2.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		for ( int i=0; i<pFN->SecondOperand.GetAmount(); i++ ){
			FillScriptTxtLine( pFN->SecondOperand[i], _spaceN+1 );
		};
		pDL = new be_CDebugerLine(_Line3.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		for ( int i=0; i<pFN->ThirdOperand.GetAmount(); i++ ){
			FillScriptTxtLine( pFN->ThirdOperand[i], _spaceN+1 );
		};
		_str _Line4 = _space; _Line4 += "{C 0xFF00ff00}end";
		pDL = new be_CDebugerLine(_Line4.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert(_pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
		return;
	}else{	// Function can not be bit to sections
		be_CFunctionBase* pFN = (be_CFunctionBase*)pSCRIPT;
		pFN->clearCB();
		_str _line = "";
		CE_WCP.GetPresentation(_line,pSCRIPT);
		_str _line_txt = _space; _line_txt += _line;
		pDL = new be_CDebugerLine(_line_txt.pchar(),pFN);
		_SCRIPT_TXT_LIST.Insert( _pos++, pDL );	pDL=NULL;
		// Set callback ID
		pFN->addCB_ID(_pos-1);
		//////////////////
	};
};
void	be_CDebuger::Process(){
	if (_active==false){
		_dialog.OnRender();
	};
};
void	be_CDebuger::ProcessHotKey(){
	if ( LastKey==0 ) return;
	if ( lastHotKey!=0x00 && GetKeyState(lastHotKey)>=0 ) lastHotKey=0x00;
	if ( GetKeyState(VK_ESCAPE)<0 && lastHotKey!=VK_ESCAPE ){
		g_beDebuger.setVisible( false );lastHotKey=VK_ESCAPE;LastKey=0;
	}else
	if ( GetKeyState(VK_TAB)<0 && lastHotKey!=VK_TAB ) {
		FN_SCRIPT_SWICHER();lastHotKey=VK_TAB;LastKey=0;
	}else
	if ( GetKeyState(VK_F5)<0 && lastHotKey!=VK_F5 ) {
		FN_INTERRUPT();lastHotKey=VK_F5;LastKey=0;
	}else
	if ( GetKeyState(VK_F10)<0 && lastHotKey!=VK_F10 ) {
		FN_NEXT_CALL();lastHotKey=VK_F10;LastKey=0;
	}else
	if ( GetKeyState(VK_F11)<0 && lastHotKey!=VK_F11 ) {
		FN_NEXT_CALL();lastHotKey=VK_F11;LastKey=0;
	}else
	if ( GetKeyState(VK_LCONTROL)<0 || GetKeyState(VK_RCONTROL)<0 ){
		if ( GetKeyState('U')<0 && lastHotKey!='U' ) {
			FN_UPDATE_TXT();lastHotKey='U';LastKey=0;
		}else
		if ( GetKeyState('V')<0 && lastHotKey!='V' ) {
			FN_VARIABLES();lastHotKey='V';LastKey=0;
		}else
		if ( GetKeyState('F')<0 && lastHotKey!='F' ) {
			FN_FUNCTIONS();lastHotKey='F';LastKey=0;
		}else
		if ( GetKeyState('D')<0 && lastHotKey!='D' ) {
			_active = !_active;lastHotKey='D';LastKey=0;
		}else
		if ( GetKeyState('W')<0 && lastHotKey!='W' ) {
			_watch = !_watch;lastHotKey='W';LastKey=0;
		}else
		if ( GetKeyState('B')<0 && lastHotKey!='B' ) {
			_smartBP = !_smartBP;lastHotKey='B';LastKey=0;
		}else
		if ( GetKeyState('I')<0 && lastHotKey!='I' ) {
			FN_UPDATE_TXT_SEL();lastHotKey='I';LastKey=0;
		}else
		if ( GetKeyState('O')<0 && lastHotKey!='O' ) {
			FN_FILE_DIALOG();lastHotKey='O';LastKey=0;
		}else
		if ( GetKeyState('L')<0 && lastHotKey!='L' ) {
			FN_LOAD();lastHotKey='L';LastKey=0;
		}else
		if ( GetKeyState('S')<0 && lastHotKey!='S' ) {
			FN_SAVE();lastHotKey='S';LastKey=0;
		};
	}
};
void	be_CDebuger::clearSelection(){
	for ( int i=0; i<_SCRIPT_TXT_LIST.GetAmount(); i++ ){
		_SCRIPT_TXT_LIST[i]->_selected = false;
	};
	_selected_id = -1;
};
void	be_CDebuger::setSelection(int _id,bool scrollOnIT/*=false*/){
	if ( _selected_id<0 || _selected_id>=_SCRIPT_TXT_LIST.GetAmount() ) clearSelection();
	else if ( _SCRIPT_TXT_LIST[_selected_id]->_selected==false )		clearSelection();
	else _SCRIPT_TXT_LIST[_selected_id]->_selected=false;
	if ( _id<_SCRIPT_TXT_LIST.GetAmount() ){
		_selected_id = _id;
		_SCRIPT_TXT_LIST[_selected_id]->_selected=true;
		if ( scrollOnIT )	_dialog.V_scrollTXT_ToElement(_selected_id);
	}else{
		clearSelection();
	};
};
void	be_CDebuger::FN_MOVE_SELECTOR(){
	if ( SimpleDialog::CallParams.Clicked && 0<=SimpleDialog::CallParams.Clicked->AutoIndexInArray) {
		setSelection(SimpleDialog::CallParams.Clicked->AutoIndexInArray);
	};
};
void	be_CDebuger::switchBreakPoint(int _id){
	if ( 0<=_id && _id<_SCRIPT_TXT_LIST.GetAmount() ){
		_SCRIPT_TXT_LIST[_id]->switchBreakPoint();
	}
};
void	be_CDebuger::openSelectedLineInSRC(){
	FN_SCRIPT_SWICHER();	
};
void	be_CDebuger::FN_RIGHT_CLICK(){
	if ( SimpleDialog::CallParams.Clicked ){
		// switch break point
		if ( SimpleDialog::CallParams.Clicked->MouseRelX<=40 ){
			switchBreakPoint(SimpleDialog::CallParams.Clicked->AutoIndexInArray);
			return;
		};
		// open in SRC mode
		openSelectedLineInSRC();
	};
};
void	be_CDebuger::callFunction(int _id){
	if ( _id==-1 || _active==false ) return;
	if ( 
		0<=_id && _id<_SCRIPT_TXT_LIST.GetAmount() && 
		(_SCRIPT_TXT_LIST[_id]->_breakPoint||_stepByStepState||_SMARTBP_LIST.check()) 
	   )
	{
		if ( _SCRIPT_TXT==false ) switchTXTvsSRC();
		setSelection(_id,true);
		_stepByStepState = true;
		if (_visible==false) setVisible(true);
		while ( _dialog.OnRender() ){};
	};
};
void	be_CDebuger::FN_NEXT_CALL(){
	_nextCall = true;
};
void	be_CDebuger::FN_INTERRUPT(){
	_interrupt = true;
}
void	be_CDebuger::FN_SEARCH_STR_SWITCH_ACTIVITY(){
	_strSearchActive = !_strSearchActive;
};
int		be_CDebuger::SD_SEARCH_RESULTS::getNextLineID(){
	int l_curID = g_beDebuger._selected_id;
	if ( l_curID==-1 )	return _lines_id[0];
	int l_newIndex = -1;
	for ( int i=0; (l_newIndex==-1)&&(i<_lines_id.GetAmount()); i++ ){
		if ( _lines_id[i]>l_curID ) l_newIndex = i;
	};
	if ( l_newIndex==-1 ) return _lines_id[0];
	return _lines_id[l_newIndex];
};
void	be_CDebuger::SD_SEARCH_RESULTS::search(_str& _pstr){
	if ( _pstr.pchar()==NULL || (_pstr.pchar())[0]==0 ) return;
	if ( _pstr!=_string ){
		clear();
		// Recreate _lines_id
		_string = _pstr;
		be_CDebugerLine* pTXT_LINE = NULL;
		for ( int i=0; i<g_beDebuger._SCRIPT_TXT_LIST.GetAmount(); i++ ){
			pTXT_LINE = (be_CDebugerLine*)(g_beDebuger._SCRIPT_TXT_LIST[i]);
			if ( strstr( pTXT_LINE->_text.pchar(), _string.pchar() )!=NULL ){
				_lines_id.Add(i);
			};
		};
	};

	// No string in _TXT_LINE_LIST
	if ( _lines_id.GetAmount()==0 ){
		_str l_mes = "String \"";
		l_mes += _string;
		l_mes += "\" not finde.";
		MessageBox(NULL, l_mes.pchar(), "Finde Results", MB_OK);
		return;
	};
	// No more string in _TXT_LINE_LIST
	if ( _iterN==_lines_id.GetAmount() ){
		_str l_mes = "No more string \"";
		l_mes += _string;
		l_mes += "\" in script text.";
		MessageBox(NULL, l_mes.pchar(), "Finde Results", MB_OK);
		_iterN=0;
		return;
	};
	// Normal iterations
	g_beDebuger.setSelection( getNextLineID(), true );
	_iterN++;
};
void	be_CDebuger::SD_SEARCH_RESULTS::clear(){
	_string = "";
	_lines_id.Clear();
	_iterN = 0;
};
void	be_CDebuger::FN_SEARCH(){
	_strSearchActive=false;
	_SEARCH_RESULTS.search(_strSearch);
};
int		be_CDebuger::findeElementByPtr(BaseClass* pELEM){
	int l_retID = -1;
	if ( pELEM!=NULL ){
		be_CDebugerLine* pDL = NULL;
		for ( int i=0; (l_retID==-1)&&(i<_SCRIPT_TXT_LIST.GetAmount()); i++ ){
			pDL = (be_CDebugerLine*)(_SCRIPT_TXT_LIST[i]);
			if (pDL&&pDL->pFN==pELEM) {
				l_retID = i;
			};
		};
	};
	return l_retID;
};
void	be_CDebuger::FN_VARIABLES(){
	_dialog._state = __dsVALUE;
};
void	be_CDebuger::FN_FUNCTIONS(){
	_dialog._state = __dsFUNCTION;
};
void	be_CDebuger::deleteWatch(int _id){
	if ( 0<=_id && _id<_WATCH_LIST.GetAmount() ){
		_WATCH_LIST.DelElement(_id);
	}
};
void	be_CDebuger::FN_DELETE_WATCH(){
	if ( SimpleDialog::CallParams.Clicked ){
		deleteWatch(SimpleDialog::CallParams.Clicked->AutoIndexInArray);
	};
};
void	be_CDebuger::FN_ADD_EDIT_WATCH(){
	_dialog._state = __dsWATCH;
};
void	be_CDebuger::deleteSmartBP(int _id){
	if ( 0<=_id && _id<_SMARTBP_LIST.GetAmount() ){
		_SMARTBP_LIST.DelElement(_id);
	}
};
void	be_CDebuger::FN_DELETE_SMARTBP(){
	if ( SimpleDialog::CallParams.Clicked ){
		deleteSmartBP(SimpleDialog::CallParams.Clicked->AutoIndexInArray);
	};
};
void	be_CDebuger::FN_ADD_EDIT_SMARTBP(){
	_dialog._state = __dsSMARTBP;
};
void	be_CDebuger::saveSCRIPT(){
	if ( g_beCheckChar(_FILE_NAME.pchar()) ){
		lvCBattleMapSaver _saver;
		_saver.SaveToXML(_FILE_NAME.pchar());
	};
};
void	be_CDebuger::loadSCRIPT(){
	if ( g_beCheckChar(_FILE_NAME.pchar()) ){
		lvCBattleMapSaver _saver;
		_saver.LoadFromXML(_FILE_NAME.pchar());
		FN_UPDATE_TXT();
	};
};
void	be_CDebuger::FN_FILE_DIALOG(){
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

	if (GetOpenFileName(&ofn)==TRUE){
		_FILE_NAME = (char*)(ofn.lpstrFile);
	};
};
void	be_CDebuger::FN_SAVE(){
	saveSCRIPT();
};
void	be_CDebuger::FN_LOAD(){
	loadSCRIPT();
};
//////////////////////////////////////////////////////////////////////////

void	be_REG_CLASS_Debuger(){
	REG_CLASS(be_CDebugerLine);
	REG_CLASS(be_CDebugerLineList);
	REG_CLASS(be_CWatchLine);
	REG_CLASS(be_CWatchLineList);
	REG_CLASS(be_CSmartBPLine);
	REG_CLASS(be_CSmartBPLineList);
	REG_CLASS(be_CDebuger);
};