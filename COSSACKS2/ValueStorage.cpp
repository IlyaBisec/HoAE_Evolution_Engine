#include "stdheader.h"
#include "BE_HEADERS.h"

be_CValueStore g_beVAR_STORE;
be_CFSM_LOADER g_beFSM_LOADER;

// be_CValueStore
//DWORD		be_CValueStore::SetViewer(DWORD _flag){
//	DWORD	oldFLAG = CurViewFLAG;
//	CurViewFLAG = _flag;
//	return oldFLAG;
//};		
//DWORD		be_CValueStore::GetClassMask(){
//	//return CurViewFLAG;
//	return	0xFFFFFFFF;
//};
void	be_CValueStore::CLEAR(){
	BOOL_LIST.CLEAR();
	DWORD_LIST.CLEAR();
	INT_LIST.CLEAR();
	FLOAT_LIST.CLEAR();
	STR_LIST.CLEAR();
	ARRAY_LIST.CLEAR();
	GROUP_LIST.CLEAR();
	NODE_LIST.CLEAR();
	UNITSTYPE_LIST.CLEAR();
	FORMSTYPE_LIST.CLEAR();
	CAMERA_LIST.CLEAR();
	FSM_LIST.CLEAR();
	SMARTOBJ_LIST.CLEAR();
	USERMSG_LIST.CLEAR();
	DIALOGS_LIST.CLEAR();
	SUBF_LIST.CLEAR();
};
be_CSUBFUNCTION*		be_CValueStore::getSubFby_GROUP( be_CGROUP* pGR ){
	be_CSUBFUNCTION* sfRet = NULL;
	int sfN = SUBF_LIST.GetAmount();
	while ( sfN-- ) {
		sfRet = (be_CSUBFUNCTION*)SUBF_LIST[sfN];
		if (sfRet) {
			for ( int i=0; i<sfRet->InArgList.GetAmount(); i++ ){
				be_CUniveraslParam* pUPAR = &(((be_CBasePrametr*)(sfRet->InArgList[i]))->UniveraslParam);
				if ( pUPAR->Local ) {
					if	(	pUPAR->LocalObject.Get() &&
							pUPAR->LocalObject.Get()->GetObjectGlobalID()==pGR->GetObjectGlobalID()
						)
						return sfRet;
				}else{
					if ( pUPAR->GlobalReference.ID==pGR->GetObjectGlobalID() ) 
						return sfRet;
				}
			}
		}
	};

	return NULL;	
};
void	be_CValueStore::DRAW(){
	DRAW_GROUPS();
	DRAW_NODES( DriveMode()->NodeMode() );
	DRAW_BLANKS( DriveMode()->NodeMode() );
	DARW_CAMERAS();
};
void	be_CValueStore::UPDATE(){
	UPDATE_GROUPS();	
};
void	be_CValueStore::UPDATE_AFTER_LOAD(){
	UPDATE_GROUPS_AFTER_LOAD();
};
void	be_CValueStore::PROCESS(){
	PROCESS_SUBF();	
};
void	be_CValueStore::DRAW_NODES(DWORD mode){
	if ( DriveMode()->NodeView==1 && !be_CNodesDriver::getVisible() ){ /* VIEW ALL */
		int N = NODE_LIST.GetAmount();
		while (N--) {
			NODE_LIST[N]->vDraw();
		};
	};
};
void	be_CValueStore::DRAW_BLANKS(DWORD mode){
	int N = BLANK_LIST.GetAmount();
	while (N--) {
		BLANK_LIST[N]->vDraw();
	};
};
void	be_CValueStore::DRAW_GROUPS(){
	if ( DriveMode()->BE_vGRP_VISIBLE && !be_CGroupsDriver::getVisible() ){
		int N = GROUP_LIST.GetAmount();
		while (N--) {
			GROUP_LIST[N]->vDraw();
		};
	};
};
void	be_CValueStore::DARW_CAMERAS(){
	//int N = CAMERA_LIST.GetAmount();
	//while (N--) {
	//	CAMERA_LIST[N]->DRAW();
	//};
};
void	be_CValueStore::UPDATE_GROUPS(){
	int N = GROUP_LIST.GetAmount();
	while (N--) {
		GROUP_LIST[N]->RemoveDeadUnits();
	};
};
void	be_CValueStore::UPDATE_GROUPS_AFTER_LOAD(){

	// Apdate all objects be_GROUPS
	int B = CPGARB.GetBranch("be_CGROUP");
	DynArray<OneClassPointer*>& CPA=CPGARB.CPointer[B];
	int TCN = CPA.GetAmount();
	ReferableBaseClass* pRefOB = NULL;
	while (TCN--) {
		OneClassPointer* CP=CPA[TCN];
		if (CP!=NULL&&CP->ClassPtr!=NULL){
			( (be_CGROUP*)CP->ClassPtr )->UpdateOB_GID_inGROUP();
		}
	}

	//int N = GROUP_LIST.GetAmount();
	//while (N--) {
	//	GROUP_LIST[N]->UpdateOB_GID_inGROUP();
	//};
};
void	be_CValueStore::_UpdateDialogs(){
	int N = DIALOGS_LIST.GetAmount();
	while (N--) {
		DIALOGS_LIST[N]->Update();
	};
};
void	be_CValueStore::_SaveDialogs(_str& _fileName){
	FILE *stream = fopen( _fileName.pchar(), "w" );
	if (stream) {
		_str	_string;
		int N = DIALOGS_LIST.GetAmount();
		for ( int i=0; i<N; i++ ){
			DIALOGS_LIST[i]->getStringForSave(_string);
			fprintf( stream, "%s%s", _string.pchar(), "\n" );
		};
		fclose( stream );
	};
};
void	be_CValueStore::_LoadDialogs(_str& _fileName){
	FILE *stream = fopen( _fileName.pchar(), "r" );
	be_CSTR_DIALOG* pstrDLG = NULL;
	if (stream) {
		_str	OneSTR = "";
		char	pSTR2048[2048];
		while ( !feof(stream) ){
			fgets(pSTR2048,2048,stream);
			if ( pSTR2048[0]=='#' ) {
				if ( OneSTR.pchar()[0]==0 )	{
					OneSTR = pSTR2048;
				}else{
					pstrDLG = new be_CSTR_DIALOG();
					DIALOGS_LIST.Add(pstrDLG);
					be_gConvertText(	OneSTR,
										pstrDLG->TextID,
										pstrDLG->Speaker,
										pstrDLG->Spirit,
										pstrDLG->ogFile,
										pstrDLG->Text
									);
					be_gBrushTetx(pstrDLG->Text);
					if ( strlen(pstrDLG->TextID.pchar())>2 ) pstrDLG->SetName( &(pstrDLG->TextID.pchar()[1]) );
					pstrDLG->Test();
					pstrDLG=NULL;
					OneSTR = pSTR2048;
				};			
			}else{
				OneSTR += pSTR2048;
			};
		};
		fclose(stream);
	};	
};	
void	be_CValueStore::CVS_UpdateDialogs::EvaluateFunction(){
	g_beVAR_STORE._UpdateDialogs();
};
void	be_CValueStore::CVS_SaveDialogs::EvaluateFunction(){
	g_beVAR_STORE._SaveDialogs(FileName);
};
void	be_CValueStore::CVS_LoadDialogs::EvaluateFunction(){
	g_beVAR_STORE._LoadDialogs(FileName);
};
void	be_CValueStore::CVS_TestDialogs::EvaluateFunction(){
	int N = g_beVAR_STORE.DIALOGS_LIST.GetAmount();
	while (N--) {
		g_beVAR_STORE.DIALOGS_LIST[N]->Test();
	};
};
be_CONETYPE<ReferableBaseClass>*		be_CValueStore::GET( char* ClassTypeName ){
	if			( strcmp(ClassTypeName,"be_CBOOL"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&BOOL_LIST);
	}else if	( strcmp(ClassTypeName,"be_CDWORD"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&DWORD_LIST);
	}else if	( strcmp(ClassTypeName,"be_CINT"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&INT_LIST);
	}else if	( strcmp(ClassTypeName,"be_CFLOAT"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&FLOAT_LIST);
	}else if	( strcmp(ClassTypeName,"be_CSTR"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&STR_LIST);
	}else if	( strcmp(ClassTypeName,"be_CARRAY"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&ARRAY_LIST);
	}else if	( strcmp(ClassTypeName,"be_CGROUP"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&GROUP_LIST);
	}else if	( strcmp(ClassTypeName,"be_CNODE"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&NODE_LIST);
	}else if	( strcmp(ClassTypeName,"be_CBLANK"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&BLANK_LIST);
	}else if	( strcmp(ClassTypeName,"be_CUnitType"		)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&UNITSTYPE_LIST);
	}else if	( strcmp(ClassTypeName,"be_CFurmationType"	)==0 ) {
    	return	(be_CONETYPE<ReferableBaseClass>*)(&FORMSTYPE_LIST);
	}else if	( strcmp(ClassTypeName,"be_CCAMERA"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&CAMERA_LIST);
	}else if	( strcmp(ClassTypeName,"be_CFSM"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&FSM_LIST);
	}else if	( strcmp(ClassTypeName,"be_CSMARTOBJECT"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&SMARTOBJ_LIST);
	}else if	( strcmp(ClassTypeName,"be_CMSG_FROMUSER"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&USERMSG_LIST);
	}else if	( strcmp(ClassTypeName,"be_CSTR_DIALOG"		)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&DIALOGS_LIST);
	}else if	( strcmp(ClassTypeName,"be_CSUBFUNCTION"	)==0 ) {
		return	(be_CONETYPE<ReferableBaseClass>*)(&SUBF_LIST);
	}
	return NULL;
};
ReferableBaseClass*						be_CValueStore::GET( char* ClassTypeName, int ID ){
	if ( g_beCheckChar(ClassTypeName)==false ) return NULL;
	be_CONETYPE<ReferableBaseClass>* pObjectArr = GET(ClassTypeName);
	ReferableBaseClass* pObject = NULL;
	if ( pObjectArr && 0<=ID && ID<pObjectArr->GetAmount() ) {
		pObject = (*pObjectArr)[ID];
	};
	return pObject;
};
ReferableBaseClass*						be_CValueStore::GET( char* ClassTypeName, char* ObjectName ){
	if ( g_beCheckChar(ClassTypeName)==false || g_beCheckChar(ObjectName)==false ) return NULL;
	ReferableBaseClass* pObject = NULL;
	be_CONETYPE<ReferableBaseClass>* pObjectArr = GET(ClassTypeName);
	if (pObjectArr) {
		int N = pObjectArr->GetAmount();
		while ( pObject==NULL && N-- ) {
			if ( (*pObjectArr)[N] && g_beCheckChar((*pObjectArr)[N]->Name.pchar()) ){
				if ( strcmp((*pObjectArr)[N]->Name.pchar(),ObjectName)==0 ){
					pObject = (*pObjectArr)[N];
				};
			};
		};
	};
	return pObject;
};
void	be_CValueStore::PROCESS_SUBF(){
	int SFN = SUBF_LIST.GetAmount();
	for ( int i=0; i<SFN; i++ )	{
		addrand(i);
		SUBF_LIST[i]->Process();
	};
};
// ACTIONS ///////////////////////////////////////////////////////////////
be_CACT_STOR	g_beACT_STORE;
// NODE //
be_CVNODE_ACT::~be_CVNODE_ACT(){
	/*
	if		( ActionName == "AddNode" ){
		pNode = NULL;
	}else if ( ActionName == "DelNode" ){
		delete pNode;
		pNode = NULL;
	};*/
};
be_CONETYPE<be_CNODE>*	be_CVNODE_ACT::vGetNodeList(){
	if ( DriveMode()->NodeType == "NODE" ){
		return 	&(g_beVAR_STORE.NODE_LIST);
	}else if ( DriveMode()->NodeType == "BLANK" ){
		return 	(be_CONETYPE<be_CNODE>*)( &(g_beVAR_STORE.BLANK_LIST) );
	};
	return NULL;
};
bool					be_CVNODE_ACT::remove(){
	bool OK = false;
	/*
	if		( ActionName == "AddNode" ){
		OK = vGetNodeList()->DelByGID( pNode->GetObjectGlobalID() );
		pNode = NULL;
	}else if ( ActionName == "DelNode" ){
		char oldName[255];
		sprintf(oldName,"%s",pNode->Name.pchar());
		char* delPOS = strstr(oldName,"__del__");
		delPOS[0] = 0;
		pNode->Name = oldName;
		vGetNodeList()->Add(pNode);
		pNode = NULL;
		OK = true;
	};*/
	return OK;
};
be_CNODE*				be_CVNODE_ACT::vAddNode(int x,int y,char* name/*=NULL*/){
	if ( DriveMode()->NodeType == "NODE" ){
		be_CNODE* pNode = new be_CNODE(name,x,y);
        pNode->GetObjectGlobalID();
        vGetNodeList()->Add(pNode);
		ActionName  = "AddNode";
		return pNode;
	}else if ( DriveMode()->NodeType == "BLANK" ){
		be_CBLANK* pNode = new be_CBLANK(name,x,y);
        pNode->GetObjectGlobalID();
		vGetNodeList()->Add(pNode);
		ActionName  = "AddBlank";
		return (be_CNODE*)pNode;
	};
	return NULL;
};
bool					be_CVNODE_ACT::vDelNodeID(DWORD _id){
	int LID = vGetNodeList()->GetIDByGID(_id);
	if (LID!=-1){
		pNode	= vGetNode(_id);
	//	pNode->Name += "__del__";
		delete (*vGetNodeList())[LID];
		(*vGetNodeList())[LID] = NULL;
		vGetNodeList()->DelElement(LID);
	//	ActionName  = "DelNode";
		g_beOBJ_CONTROLER.SetState("be_CNODE");
		g_beOBJ_CONTROLER.SetState("be_CVariableBase");
		return true;
	}
	return false;
};
bool					be_CVNODE_ACT::vDelNodePT(be_CNODE* pND){
	if ( pND ){
		return vDelNodeID( pND->GetObjectGlobalID() );
	};
	return false;
};
bool					be_CVNODE_ACT::vDelNode(int _x,int _y){
	return vDelNodePT( vGetNode(_x,_y) );
};
be_CNODE*				be_CVNODE_ACT::vGetNode(DWORD _id){
	return (be_CNODE*)(vGetNodeList()->GetByGID(_id));
};
be_CNODE*				be_CVNODE_ACT::vGetNode(int _x,int _y){
	const	float MIN_DIST	= 300.f;

	be_CNODE* pRET = NULL;
	int N = vGetNodeList()->GetAmount();
	if (N>0){
		pRET			= (*vGetNodeList())[0];
		float minDist	= (*vGetNodeList())[0]->vGetNodeDist(_x,_y);
		float dist		= 0.f; 
		while (N--) {
			dist = (*vGetNodeList())[N]->vGetNodeDist(_x,_y);
			if (minDist>dist){
				minDist = dist;
				pRET = (*vGetNodeList())[N];
			};
		};
		if (minDist>MIN_DIST) {
			pRET = NULL;
		};
	};
	return	pRET;
};
// GROUP
be_CVGROUP_ACT::~be_CVGROUP_ACT(){
	if		( ActionName == "AddGroup" ){
		pGroup = NULL;
	}else if ( ActionName == "DelGroup" ){
		delete pGroup;
		pGroup = NULL;
	};	
};
bool					be_CVGROUP_ACT::remove(){
	bool OK = false;
	if		( ActionName == "AddGroup" ){
		OK = vGetGroupList()->DelByGID( pGroup->GetObjectGlobalID() );
		pGroup = NULL;
	}else if ( ActionName == "DelGroup" ){
		char oldName[255];
		sprintf(oldName,"%s",pGroup->Name.pchar());
		char* delPOS = strstr(oldName,"__del__");
		delPOS[0] = 0;
		pGroup->Name = oldName;
		vGetGroupList()->Add(pGroup);
		pGroup = NULL;
		OK = true;
	};
	return OK;
};
be_CONETYPE<be_CGROUP>*	be_CVGROUP_ACT::vGetGroupList(){
	return &(g_beVAR_STORE.GROUP_LIST);		
};
be_CGROUP*				be_CVGROUP_ACT::vAddGroup(const char* _name){
	pGroup = new be_CGROUP(_name);	
	vGetGroupList()->Add(pGroup);
	ActionName = "AddGroup";
	return pGroup;
};
bool					be_CVGROUP_ACT::vDelGroupID(DWORD _id){
	int LID = vGetGroupList()->GetIDByGID(_id);
	if (LID!=-1){
		pGroup	= vGetGroup(_id);
		pGroup->Name += "__del__";
		(*vGetGroupList())[LID] = NULL;
		vGetGroupList()->DelElement(LID);
		ActionName  = "DelGroup";
		g_beOBJ_CONTROLER.SetState("be_CGROUP");
		g_beOBJ_CONTROLER.SetState("be_CVariableBase");
		return true;
	}
	return false;
};
bool					be_CVGROUP_ACT::vDelGroupName(const char* _name){
	int LID = vGetGroupList()->GetIDByNAME(_name);
	if (LID!=-1){
		pGroup	= vGetGroup(_name);
		pGroup->Name += "__del__";
		(*vGetGroupList())[LID] = NULL;
		vGetGroupList()->DelElement(LID);
		ActionName  = "DelGroup";
		g_beOBJ_CONTROLER.SetState("be_CGROUP");
		g_beOBJ_CONTROLER.SetState("be_CVariableBase");
		return true;
	}
	return false;
};
bool					be_CVGROUP_ACT::vDelGroupPT(be_CGROUP* pGR){
	if (pGR!=NULL) return vDelGroupID(pGR->GetObjectGlobalID());
	return false;
};
be_CGROUP*				be_CVGROUP_ACT::vGetGroup(DWORD _id){
	return (be_CGROUP*)(vGetGroupList()->GetByGID(_id));
};
be_CGROUP*				be_CVGROUP_ACT::vGetGroup(const char* _name){
	return (be_CGROUP*)(vGetGroupList()->GetByName(_name));
};
// STORE
bool	be_CACT_STOR::removeLAST(){
	bool OK = false;
	int N = GetAmount()-1;
	if (N>=0) {
		OK = (*this)[N]->remove();
		delete (*this)[N];
		(*this)[N] = NULL;
		DelElement(N);
	};
	return OK;
};
// FSM .xml file with name and file path
be_CFSM_LOADER::be_CFSM_LOADER(){
	Init = true;	
};
void	be_CFSM_LOADER::Load(){
	if (Init) {
		Init=false;
		ReadFromFile("Missions\\FSM\\FSM_List.xml");
	};
};
bool	be_CFSM_LOADER::GetFilePath(_str& _filePath, const char* FSM_Name){
	bool OK = false;
	if (g_beCheckChar(FSM_Name)){
		Load();
		int N=FSM_FILE_LIST.GetAmount();
		while (!OK&&N--) {
			if ( strcmp(FSM_FILE_LIST[N]->Name.pchar(),FSM_Name)==0 ) {
				_filePath = FSM_FILE_LIST[N]->FilePath;		
				OK=true;
			};
		};
	};
	return OK;
};
//////////////////////////////////////////////////////////////////////////
void	be_REG_CLASS_VariablesStore(){
	REG_CLASS(be_CValueStore);	

	REG_CLASS(be_CNameFile);
	REG_CLASS(be_CFSM_LOADER);

};
//////////////////////////////////////////////////////////////////////////


















