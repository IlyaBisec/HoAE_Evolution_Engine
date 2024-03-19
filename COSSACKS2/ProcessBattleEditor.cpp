#include "stdheader.h"	
#include "BE_HEADERS.h"
#include "unitability.h"

void	PreDrawGameProcess();
void	GFieldShow();
void	PostDrawGameProcess();
void	GlobalHandleMouse();
void	UnPress();

DialogsSystem	DLG00,DLG01,DLG02;
ClassEditor		EDT00,EDT01,EDT02;

extern	be_CNODE* ActivObject;
extern	lvCEdge*	pActivEdge;

bool	addNewNodeInAct = false;

lvCSquardsOnMap* pSquardsOnMap = NULL;
lvCZonesOnMap*   pZonesOnMap   = NULL;
//lvCGroupsOnMap*	 pGroupsOnMap  = NULL;
//lvCNodesOnMap*	 pNodesOnMap   = NULL;
//lvCVValuesOnMap* pVValuesOnMap = NULL;

SingletonCBEDriveMode*		SingletonCBEDriveMode::m_pObj		=NULL;
SingletonlvCNodesMAP*		SingletonlvCNodesMAP::m_pObj		=NULL;
SingletonlvCGroupsMAP_ST*	SingletonlvCGroupsMAP_ST::m_pObj	=NULL;
Singleton_vvMAP_ST*			Singleton_vvMAP_ST::m_pObj			=NULL;
SingletonlvCBattleShema*	SingletonlvCBattleShema::m_pObj		=NULL;
SingletonlvCScriptHandler*	SingletonlvCScriptHandler::m_pObj	=NULL;

SingletonlvCShapes*			SingletonlvCShapes::m_pObj			=NULL;
lvCRotateCamera*			lvCRotateCamera::m_pObj				=NULL;
CameraPoss*					CameraPoss::m_pObj					=NULL;
// EXPERIENCE ////////////////////////////////////////////////////////////
ExperienceManager*			ExperienceManager::m_pObj			=NULL;
// TEST //////////////////////////////////////////////////////////////////

extern	BaseMesMgrST		gMessagesMap;

BE_EDIT_PANEL	MAIN_EDIT_PANEL;


#ifdef	__LUA_DEBUGGER__
	CScriptDebugger	g_LUA_DBG;
#endif//__LUA_DEBUGGER__
// PRESENTATION //////////////////////////////////////////////////////////
WCP_Editor	g_WCP_EDITOR;
void	WCP_Editor::ShowEditor(){
    SimpleEditClass("MASK PRESENTATION EDITOR",this);
};
void	WCP_Editor::LoadMASK(){
	int N=MASK_LIST.GetAmount();
	CE_WCP.ClassList.Clear();
	CE_WCP.SetStdPresentation();
	for (int i=0; i<N; i++){
		CE_WCP.AddFromList(MASK_LIST[i]->MASK_FILE.pchar());
	};
};
void	WCP_Editor::LoadDEFMASK(){
	if (Init){
		Init=false;
		CE_WCP.ClassList.Clear();
		CE_WCP.SetStdPresentation();
		CE_WCP.AddFromList("Missions\\Text\\BE_DATA000.ClassList.xml");
		CE_WCP.AddFromList("Missions\\Text\\BE_DATA001.ClassList.xml");
		CE_WCP.AddFromList("Missions\\Text\\BE_DATA002.ClassList.xml");
		CE_WCP.AddFromList("Missions\\Text\\BE_DATA003.ClassList.xml");
	};
};
void	WCP_Editor::LOAD_ALL_MASK::EvaluateFunction(){
	WCP_Editor* pPR = dynamic_cast<WCP_Editor*>(GetParent());
	if (pPR){
		pPR->LoadMASK();
	};
};
void	CPRESENTATION::EvaluateFunction(){
	BaseClass* pPR = GetParent();
	if (pPR!=NULL) {
		_str pres;
		if (dynamic_cast<lvCStorage*>(pPR))	(dynamic_cast<lvCStorage*>(pPR))->Prepare_lua();
		CE_WCP.GetPresentation(pres,pPR);
		FILE* file=fopen(FileName.pchar(),"w");
		if (file) {
			fprintf(file,"%s",pres.pchar());
			fclose(file);
		}
	};
};
//////////////////////////////////////////////////////////////////////////

// CHECK UniqID in units on map //////////////////////////////////////////
be_CTemp_UniqID_Test	g_beTempOBJECT;
void	be_CTemp_OOLIST::__Clear(){
	UnitsList.Clear();
};
const	char*	be_CTemp_OOLIST::GetThisElementView(const char* LoaclName){
	char* strText = GetGlobalBuffer();
	if (strText!=NULL) {
		sprintf(strText,"%d",UnitsList.GetAmount());
		return strText;
	}
	return LoaclName;
};
void	be_CTemp_UniqID_Test::__Clear(){
	int N = List.GetAmount();
	while (N--) List[N]->__Clear();
	List.Clear();
};
bool	be_CTemp_UniqID_Test::IsUniqIDInList(DWORD id){
	int N = List.GetAmount();
	bool	YES = false;
	while (!YES&&N--) {
		YES = ( List[N]->UniqID == id );
	};
	return YES;
};
void	be_CTemp_UniqID_Test::AddObject(OneObject* pOO){
	int N = List.GetAmount();
	bool add = false;
	while (!add&&N--) {
		if ( List[N]->UniqID==pOO->UniqID ){
			List[N]->UnitsList.Add(pOO->Index);
			add=true;
		};
	};
	if (add==false){
		be_CTemp_OOLIST* pnewUID = new be_CTemp_OOLIST();
		pnewUID->UniqID = pOO->UniqID;
		pnewUID->UnitsList.Add(pOO->Index);
		List.Add(pnewUID);
		pnewUID=NULL;
	};
};
void	be_CTemp_UniqID_Test::createList(){
	__Clear();
	OneObject* pOO = NULL;
	OneObject* pOOj = NULL;
	for (int i=0; i<ULIMIT; i++){
		pOO = Group[i];
		if ( pOO && (!pOO->Sdoxlo||pOO->Hidden) ){
			if (IsUniqIDInList(pOO->UniqID)==false){
				int addN = 0;
				for ( int j=0; j<ULIMIT; j++ ){
					pOOj = Group[j];
					if ( j!=i && pOOj && (!pOOj->Sdoxlo||pOO->Hidden) && pOO->UniqID==pOOj->UniqID ){
						AddObject(pOOj);
						addN++;
					};
				};
				if (addN!=0) AddObject(pOO);
			};
		};
	};
};
//////////////////////////////////////////////////////////////////////////

// MESSAGES //////////////////////////////////////////////////////////////
void	SetCurrentHelp(tpMessageArray** ppTASKS,tpMessageArray** ppTALKS,tpMessageArray** ppHINTS){
	*ppTASKS=NULL;
	*ppTALKS=NULL;
	*ppHINTS=NULL;
	vvMESSGES* pMESSAGES = NULL;
	int N = vValuesMap()->V_VALUES.GetAmount();
	while (pMESSAGES==NULL&&N--) {
		if (vValuesMap()->V_VALUES[N]->InfID==_vvMESSGES_)
			pMESSAGES = dynamic_cast<vvMESSGES*>(vValuesMap()->V_VALUES[N]);
	};
	if (pMESSAGES!=NULL){
		*ppTASKS = &(pMESSAGES->TASKS);	
//		*ppTALKS = &(pMESSAGES->TALKS);
//		*ppHINTS = &(pMESSAGES->HINTS);
	};
};

// BE_EDIT_PANEL /////////////////////////////////////////////////////////
BE_EDIT_PANEL::BE_EDIT_PANEL(){
	Error=true;
	Visible=false;
	X0=25;
	Y0=60;
	dY=20;
	tX=10;
	mW=50;
};
BE_EDIT_PANEL::~BE_EDIT_PANEL(){
};
bool			BE_EDIT_PANEL::EP_LOAD(){
	if (Error==false)	return Error;
	xmlQuote	xmlData( "EDIT_PANEL" ); 
	if (xmlData.ReadFromFile( "Dialogs\\BE_EDIT_PANEL.DialogsSystem.xml" )){
		ErrorPager	Err;
		DS_PANEL.Load( xmlData,&DS_PANEL,&Err );
		EP_PANEL_VISIBLE(false);
		Error=false;
	};
	return Error;
};
bool			BE_EDIT_PANEL::EP_DRAW(){
	if (Error)	return false;
	DS_PANEL.ProcessDialogs();
	return true;
};
bool			BE_EDIT_PANEL::EP_UPDATE(){
	if (Error)	return Error;
	EP_PANEL_VISIBLE(Visible);
	EP_TEXT_VISIBLE(Visible);
	EP_PROPER_SIZE_POS();
	return Error;
};
DialogsDesk*	BE_EDIT_PANEL::EP_PANEL(){
    if (Error)	return NULL;
	DialogsDesk* pPANEL=NULL;
	int NDSS = DS_PANEL.DSS.GetAmount();
	if (NDSS>0){
		while (pPANEL==NULL && NDSS--) {
			pPANEL = dynamic_cast<DialogsDesk*>(DS_PANEL.DSS[NDSS]);
			if (pPANEL!=NULL && strcmp(pPANEL->Name.pchar(),"PANEL")!=0) pPANEL=NULL;
		};
	};
	return pPANEL;
};
TextButton*		BE_EDIT_PANEL::EP_TEXT(){
	if (Error)	return NULL;
	TextButton* pTEXT = NULL;
	DialogsDesk* pPANEL = EP_PANEL();
	if (pPANEL!=NULL) {
		pTEXT = dynamic_cast<TextButton*>(pPANEL->Find("TEXT"));
	};
	return pTEXT;
};
void			BE_EDIT_PANEL::EP_VISIBLE(bool state){
	Visible=state;
};
void			BE_EDIT_PANEL::EP_SET_TEXT(char* text){
	if (text==NULL) return;
	TextButton* pTB=EP_TEXT();
	if (pTB==NULL)	return;
	pTB->SetMessage(text);
};
void			BE_EDIT_PANEL::EP_PANEL_VISIBLE(bool state){
	if (Error)	return;
	EP_PANEL()->Visible=state;
};
void			BE_EDIT_PANEL::EP_TEXT_VISIBLE(bool state){
	if (Error)	return;
	EP_TEXT()->Visible=state;
};
void			BE_EDIT_PANEL::EP_PROPER_SIZE_POS(){
	if (Error)	return;

	int PW=RealLx-X0*2;
	int PH=RealLy/2-Y0-dY;

	EP_TEXT_POSITION(tX,tX);
	EP_TEXT_MAXWIDTH(PW-tX*2-mW);
	EP_PANEL_SIZE(PW,PH);
	EP_PANEL_POSITION(X0,Y0);
};
void			BE_EDIT_PANEL::EP_PANEL_POSITION(int x,int y){
	if (Error)	return;
	EP_PANEL()->Setx(x);
	EP_PANEL()->Sety(y);
};
void			BE_EDIT_PANEL::EP_TEXT_POSITION(int x,int y){
	if (Error)	return;
	EP_TEXT()->Setx(x);
	EP_TEXT()->Sety(y);
};
void			BE_EDIT_PANEL::EP_PANEL_SIZE(int w,int h){
	if (Error)	return;
	EP_PANEL()->SetWidth(w);
	EP_PANEL()->SetHeight(h);
};
void			BE_EDIT_PANEL::EP_TEXT_MAXWIDTH(int maxw){
	if (Error)	return;
	EP_TEXT()->MaxWidth=maxw;
};
// CBEDriveMode_ST ///////////////////////////////////////////////////////
CBEDriveMode_ST::CBEDriveMode_ST()	{ 
	visible=false; 
	OBJECT=2; 
	USE_vGRP=true;		
	NodeAct=3; 
	NodeView=1; 
	ND_RECT=true; ND_XY=false; ND_NAME=true; ND_DESCR=false;

	EdgeAct=2;
	EdgeView=1;
	EG_RECT=true; EG_DIRECT=true; EG_PROC=false; EG_START=false; EG_MODE=false;
	NodeType.SetRef("NODE",0xFFFFFFFF);

	PR_CREATE=true;
	PR_MAIN=true;
	PR_SQUAD=true;
	PROCESS=false;

	BE_SAVE = false;

	BE_vGRP_VISIBLE=true;
	BE_vGRP_TEST=false;

	MainEditType=0;
	if (EngSettings.MIS_SET.vGroupNameColor==0x00000000) EngSettings.MIS_SET.vGroupNameColor=0xFF000000;
	if (EngSettings.MIS_SET.vGroupLineColor==0x00000000) EngSettings.MIS_SET.vGroupLineColor=0xFF0000FF;
	EdgeNameColor=0xFF00FF00;
	EdgeLineColor=0xFF00FF00;
	if (EngSettings.MIS_SET.NodeNameColor==0x00000000) EngSettings.MIS_SET.NodeNameColor=0xFF00FF00;
	if (EngSettings.MIS_SET.NodeLineColor==0x00000000) EngSettings.MIS_SET.NodeLineColor=0xFF00FF00;
	if (EngSettings.MIS_SET.NodeIDColor==0x00000000) EngSettings.MIS_SET.NodeIDColor=0xFFFF0000;
	NodeStyle=1; // "Rectangle"

	EditOperation=4;
	SelectedEdge=NULL;
	SelectedNodeBeg=NULL;
	SelectedNodeEnd=NULL;
	SelectedSquadShema=NULL;
	SelectedValue=NULL;			CorectionType=0;	

	UpdateDelta		= -1;
	LockType		=  0;
	ShowHTT			= false;
	ShowLinks		= false;
	NOT_ZERO_ANLY	= false;
	ShowEXP			= false;
	ShowAST			= false;
	ShowVSB			= false;
	TRIADA_NUM		= 2;
	ShowID			= false;
	ShowXY			= false;
	Showxy			= false;
	sAreaID			= -1;
	dAreaID			= -1;
	DrawStoreWAY	= false;

	GROUP_MENU_VS	= false;
};

CBEDriveMode_ST::~CBEDriveMode_ST(){ 
	TimerData.Clear(); 
};

void CBEDriveMode_ST::Access(){
};

void	CBEDriveMode_ST::CBE_MainEdit::EvaluateFunction(){

	g_WCP_EDITOR.LoadDEFMASK();

	BE_USAGE_FLAGS	OLD_FLAG = g_BE_USAGE_FLAG;
	g_BE_USAGE_FLAG = BE_IF_DEFF;
	

	int lastItemChoose = ItemChoose;
	ItemChoose=-1;
//	vValuesMap()->SetViewType(0x00000002);
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM->MainEditType==0){
//		ProcessEditor("MainBEScript");
		ProcessEditor("ScriptEditor");
	}else if (pDM->MainEditType==1) {
		MAIN_EDIT_PANEL.EP_VISIBLE(true);
		ProcessEditor("MainBEScriptEX");
	};				
	ItemChoose = lastItemChoose;
//	vValuesMap()->SetViewType(0x00000001);
	MAIN_EDIT_PANEL.EP_VISIBLE(false);

	g_BE_USAGE_FLAG = OLD_FLAG;
};

CBEDriveMode_ST::CBE_AddFilm::CBE_AddFilm()		{scrName="New Film"; scrDescr="New Film Descr";};
CBEDriveMode_ST::CBE_AddFilm::~CBE_AddFilm()	{scrName.Clear(); scrDescr.Clear();};
void	CBEDriveMode_ST::CBE_AddFilm::EvaluateFunction(){
	BattleHandler()->AddNewFilm(scrName.pchar(),scrDescr.pchar());
};

void	CBEDriveMode_ST::CBE_EditFilm::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL) {
		BaseClass* pOBJ = dynamic_cast<BaseClass*>(pDM->scrFILM.Get());
		if (pOBJ!=NULL) {
			ReplaceEditor("ScriptFilm",pOBJ);
			int lastItemChoose = ItemChoose;
			ItemChoose=-1;
			ProcessEditor("ScriptFilm");
			ItemChoose = lastItemChoose;
		};
	};
};

void	CBEDriveMode_ST::CBE_EditGraph::EvaluateFunction(){
	BaseClass* pEditClass = &(BattleHandler()->SCRIPT_GRAPH);
	if (pEditClass!=NULL){
		ReplaceEditor("ScriptGraph",dynamic_cast<BaseClass*>(pEditClass));
		int lastItemChoose = ItemChoose;
		ItemChoose=-1;
		ProcessEditor("ScriptGraph");
		ItemChoose = lastItemChoose;
	};
};

CBEDriveMode_ST::CBE_Process::CBE_Process(){
	SQUARD_PR=MAIN_PR=true;
};

void	CBEDriveMode_ST::CBE_Process::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL) {
		pDM->PR_MAIN = MAIN_PR;
		pDM->PR_SQUAD= SQUARD_PR;
		pDM->PROCESS = !pDM->PROCESS;
	};
};

void	CBEDriveMode_ST::CBE_vGRP_VISIBLE_ALL::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL) {
		pDM->BE_vGRP_VISIBLE = !pDM->BE_vGRP_VISIBLE;
		GroupsMap()->SetVisible(pDM->BE_vGRP_VISIBLE);
	};
};

void	CBEDriveMode_ST::CBE_vGRP_VISIBLE::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL) {
		be_CGROUP* pvGRP = (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->visible = !pvGRP->visible;
		};
	};
};
CBEDriveMode_ST::CBE_ADD_vGRP::CBE_ADD_vGRP(){
	DivideAllUnits	= false;
	nameMask		= "myGroupList%d";	
	collect			= false;
	firstIndex		= 0;
	lastIndex		= 0;
	collectBuildings= false;
	collectUnits	= true;
	nation			= 0;
	lockType		= 0;
};
DWORD	CBEDriveMode_ST::CBE_ADD_vGRP::GetClassMask(){
	DWORD	MASK = 0x0000001;	// DivideAllUnits
	if ( DivideAllUnits )	MASK += 0x00000004;	// nameMask, collectUnits
	else					MASK += 0x00000002;	// Name
	if ( DivideAllUnits ){
		if ( collect )			MASK += 0x00000010; // collectBuildings, collectUnits
		else					MASK += 0x00000008; // firstIndex, lastIndex
		if ( collect ){
			if ( collectBuildings || collectUnits )	MASK += 0x00000200;	// nation, MaxDispersion
		};
	};
	return MASK;
};
void	CBEDriveMode_ST::CBE_ADD_vGRP::EvaluateFunction(){
	be_CVGROUP_ACT* pnewGROUP_ACT = NULL;
	if ( GetClassMask() & 0x00000002 ){
		// Add only one new group.
		be_CVGROUP_ACT* pnewGROUP_ACT = new be_CVGROUP_ACT();
		pnewGROUP_ACT->vAddGroup(Name.pchar());
		g_beACT_STORE.Add(pnewGROUP_ACT);
		pnewGROUP_ACT=NULL;
		return;
	};
	if ( GetClassMask() & 0x00000004 ){
		if ( collect==false ){
		// Add new groups list use nameMask
			char newName[256];
			for ( int index=firstIndex; index<=lastIndex; index++ ){
				pnewGROUP_ACT = new be_CVGROUP_ACT();
				sprintf(newName,nameMask.pchar(),index);
				pnewGROUP_ACT->vAddGroup(newName);
				g_beACT_STORE.Add(pnewGROUP_ACT);
				pnewGROUP_ACT=NULL;
			};
		}else{
			if ( nation<0 || nation>7 ) return;
			be_CTotalALPD AreasWithUnits;
			if ( (collectUnits||collectBuildings) && 
				 g_beHTT_LIST[nation][lockType].AST_createPCL(&AreasWithUnits) )
			{
				char newName[256];
				be_CGROUP* pGR = NULL;
				if ( collectUnits ){
					// collect units in group and add it use nameMask
					be_CAreaListPowerData* pALPD = NULL;
					for ( int ALPDid=0; ALPDid<AreasWithUnits.getN(); ALPDid++ ){
						pALPD = AreasWithUnits.getALPD(ALPDid);
						if ( pALPD && ( pALPD->MeleeUnitsN>0 || pALPD->RangeUnitsN>0 ) ){
							pnewGROUP_ACT = new be_CVGROUP_ACT();
							sprintf(newName,nameMask.pchar(),firstIndex+ALPDid);
							pGR = pnewGROUP_ACT->vAddGroup(newName);
							g_beACT_STORE.Add(pnewGROUP_ACT);
							for ( int alid=0 ; alid<pALPD->AreasList.GetAmount(); alid++ ){
								pGR->AddGObjectsInArea(nation,lockType,pALPD->AreasList[alid]);
							};
							pnewGROUP_ACT=NULL;
						};
					}
				};
				if ( collectBuildings ){
					// collect building in group and add it use nameMask
					be_CAreaListPowerData* pALPD = NULL;
					for ( int ALPDid=0; ALPDid<AreasWithUnits.getN(); ALPDid++ ){
						pALPD = AreasWithUnits.getALPD(ALPDid);
						if ( pALPD && pALPD->BuildingsN>0 ){
							pnewGROUP_ACT = new be_CVGROUP_ACT();
							sprintf(newName,nameMask.pchar(),firstIndex+ALPDid);
							pGR = pnewGROUP_ACT->vAddGroup(newName);
							g_beACT_STORE.Add(pnewGROUP_ACT);
							for ( int alid=0 ; alid<pALPD->AreasList.GetAmount(); alid++ ){
								pGR->AddGObjectsInArea(nation,lockType,pALPD->AreasList[alid],true);
							};
							pnewGROUP_ACT=NULL;
						};
					};
				};
			};
		};
		return;
	};
//	GroupsMap()->AddGroup(Name.pchar());
};

void	CBEDriveMode_ST::CBE_ADD_vGRP_SMART::EvaluateFunction(){
	GroupsMap()->AddGroupSmart(Name.pchar());
};

void	CBEDriveMode_ST::CBE_EDIT_vGRP::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL){
		BaseClass* pOBJ = dynamic_cast<BaseClass*>(pDM->BE_vGROUPS.Get());
		if (pOBJ!=NULL) {
			ReplaceEditor("BEvGroupEDIT",pOBJ);
			int lastItemChoose = ItemChoose;
			ItemChoose=-1;
			ProcessEditor("BEvGroupEDIT");
			ItemChoose = lastItemChoose;
		};
	};
};

void	CBEDriveMode_ST::CBE_ADD_SelUnits::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP = (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->AddSelectedUnits();
		};
	};
};

void	CBEDriveMode_ST::CBE_ON_SCREEN::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP = (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->ScreenOnGroup();
		};
	};
};
void	CBEDriveMode_ST::CBE_ON_NODE_SCREEN::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->NODE.Get()!=NULL) {
		be_CNODE* pvNODE = (be_CNODE*)(pDM->NODE.Get());
		if (pvNODE!=NULL) {
			pvNODE->ScreenOnNode();
		};
	};
};
CBEDriveMode_ST::CBE_REM_N_UNT::CBE_REM_N_UNT(){
	number=0xFFFF;
};
void	CBEDriveMode_ST::CBE_REM_N_UNT::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		be_CGROUP* pDestGRP	=  GroupsMap()->GetGroupID(destGRP_ID);
		if (pvGRP!=NULL&&pDestGRP!=NULL) {
			pvGRP->RemoveNUnitsToCGroup(pDestGRP,number);
		};
	};
};

CBEDriveMode_ST::CBE_KILL_N_UNT::CBE_KILL_N_UNT(){ 
	number=0xFFFF; 
};

void CBEDriveMode_ST::CBE_KILL_N_UNT::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->KillUnits(number);
		};
	};
};

CBEDriveMode_ST::CBE_ERASE_N_UNT::CBE_ERASE_N_UNT(){ 
	number=0xFFFF; 
};

void CBEDriveMode_ST::CBE_ERASE_N_UNT::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->EraseUnits(number);
		};
	};
};

void CBEDriveMode_ST::CBE_UN_SELECT::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->UnSelect();
		};
	};
};

CBEDriveMode_ST::CBE_SELECT::CBE_SELECT(){ 
	add=false; 
};

void CBEDriveMode_ST::CBE_SELECT::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->SelectUnits(add);
		};
	};
};

void CBEDriveMode_ST::CBE_SELECT_IN_XYR::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->SelectUnitsInZone(x,y,r,add);
		};
	};
};

void CBEDriveMode_ST::CBE_SET_NATION::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->SetNation(NI);
		};
	};
};

void CBEDriveMode_ST::CBE_SET_AGRESIVITY::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->SetAgresiveST(state);
		};
	};
};

void CBEDriveMode_ST::CBE_SEND_TO::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->SendTo(x,y,dir,type);
		};
	};
};

void CBEDriveMode_ST::CBE_SET_DIR::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->ChangeDirection(dir,type);
		};
	};
};

void CBEDriveMode_ST::CBE_SET_FORM::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP		= (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->ChengeFormation(formType);
		};
	};
};

void CBEDriveMode_ST::CBE_SET_FLAGBR::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP = (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->SetBrFlagbearers();
		};
	};
};

void CBEDriveMode_ST::CBE_CLEAR_FLAGBR::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL&&pDM->BE_vGROUPS.Get()!=NULL) {
		be_CGROUP* pvGRP = (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) {
			pvGRP->ClearBrFlagbearers();
		};
	};
};

void CBEDriveMode_ST::CBE_vValuesEdit::EvaluateFunction(){
	int lastItemChoose = ItemChoose;
	ItemChoose=-1;
	ProcessEditor("BEvValueEDIT");
	ItemChoose = lastItemChoose;
};

void CBEDriveMode_ST::CBE_CameraPossEdit::EvaluateFunction(){
	int lastItemChoose = ItemChoose;
	ItemChoose=-1;
	ReplaceEditor("CameraPoss",dynamic_cast<BaseClass*>(CameraPositons()));
	ProcessEditor("CameraPoss");
	ItemChoose = lastItemChoose;	
};

void CBEDriveMode_ST::CBE_BattleHandler::EvaluateFunction(){
	int lastItemChoose = ItemChoose;
	ItemChoose=-1;
	ReplaceEditor("BattleHendler",dynamic_cast<BaseClass*>(BattleHandler()));
	ProcessEditor("BattleHendler");
	ItemChoose = lastItemChoose;	
};
CBEDriveMode_ST::CBE_BOOL_LST::CBE_BOOL_LST(){
	OneClassStorage* OCS=CGARB.GetClass("vvBASE");
	if ( OCS!=NULL ) {
		TypeShowMask* pTempMask = NULL;
		for ( int i=0; i<OCS->Children.GetAmount(); i++ ){
			pTempMask = new TypeShowMask();
			if (pTempMask!=NULL) {
				pTempMask->ClassName = OCS->Children[i];
				MaskArray.Add(pTempMask);
                pTempMask = NULL;
			};
		};
	};
};
CBEDriveMode_ST::CBE_BOOL_LST::~CBE_BOOL_LST(){
	MaskArray.Clear();
};
void	CBEDriveMode_ST::CBE_BOOL_LST::UnSelectAll(){
	for ( int i=0; i<MaskArray.GetAmount(); i++ ){
		MaskArray[i]->Show = false;
	};
};
void	CBEDriveMode_ST::CBE_BOOL_LST::SelectAll(){
	for ( int i=0; i<MaskArray.GetAmount(); i++ ){
		MaskArray[i]->Show = true;
	};
};
void	CBEDriveMode_ST::CBE_BOOL_LST::CBE_UnSelectAll::EvaluateFunction(){
	CBEDriveMode_ST::CBE_BOOL_LST* pParent = dynamic_cast< CBEDriveMode_ST::CBE_BOOL_LST * >(GetParent());
	if (pParent!=NULL) {
		pParent->UnSelectAll();
	};
};
void	CBEDriveMode_ST::CBE_BOOL_LST::CBE_SelectAll::EvaluateFunction(){
	CBEDriveMode_ST::CBE_BOOL_LST* pParent = dynamic_cast< CBEDriveMode_ST::CBE_BOOL_LST * >(GetParent());
	if (pParent!=NULL) {
		pParent->SelectAll();
	};
};
void CBEDriveMode_ST::CBE_BOOL_LST::EvaluateFunction(){
	beSET_Obj* pSET = new beSET_Obj();
	int NM = MaskArray.GetAmount();
	for (int i=0; i<NM; i++){
		if ( MaskArray[i]->Show ){
			pSET->createArray(	(ClassArray<BaseClass>*)(&(vValuesMap()->V_VALUES)), 
								MaskArray[i]->ClassName.pchar(), 
								(i!=0) 
							);
		};
	};
	
	if (pSET!=NULL){
		SimpleEditClass("Selected value list", (BaseClass*)(pSET) );
		pSET->clearARRAY();
		delete pSET;
		pSET=NULL;
	};
}
void CBEDriveMode_ST::CBE_MISS_SET::EvaluateFunction(){
	int lastItemChoose = ItemChoose;
	ItemChoose=-1;
	ReplaceEditor("MissionSettings",dynamic_cast<BaseClass*>(&gMISS_SET));
	ProcessEditor("MissionSettings");
	ItemChoose = lastItemChoose;
};
void CBEDriveMode_ST::CBE_WCP_EDIT::EvaluateFunction(){
	g_WCP_EDITOR.ShowEditor();
};
void CBEDriveMode_ST::CBE_MESSGES_MGR::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL){
		int lastItemChoose = ItemChoose;
		ItemChoose=-1;
		ProcessEditor("MessageMgr");
		ItemChoose = lastItemChoose;
	};
};

int		CBEDriveMode_ST::GetSquardID(){
	if (USE_vGRP){
		be_CGROUP* pGR = (be_CGROUP*)(BE_vGROUPS.Get());
		if (pGR) {
			return pGR->GetObjectGlobalID();
		}
		return 0xFFFFFFFF;
	}
	return SQUARD;
};

DWORD	CBEDriveMode_ST::GetClassMask(){
	switch(OBJECT) {
		case 0:		// "NODE"
			return	0x00000001;
			break;
		case 1:		// "EDGE"
			return	0x00000002;
			break;
		case 2:		// "SCRIPT"
			return	0x00000004;
			break;
		case 3:		// "SAVE"
			return	0x00000008;
			break;
		case 4:		// "vGROUPS"
			if (BE_vGRP_TEST)	return 0x00000020;
			return	0x00000010;
			break;
		case 5:		// "vVALUES"
			return	0x00000040;
		case 6:		// "Settings"
			return	0x00000080;
			break;
		case 7:		// "EDIT"
			return	0x00000200;
			break;
		case 8:		// "TIMER"
			return	0x00000400;
			break;
		case 9:
			return	0x00000800;
			break;
		default:	// "NOTHING"
			return	0x80000000;
	};
};

DWORD	CBEDriveMode_ST::NodeMode(){
	DWORD	mode = 0;
	if (ND_RECT)	mode |= _RECT_;
	if (ND_XY)		mode |= _XY_;
	if (ND_NAME)	mode |= _NAME_;
	if (ND_DESCR)	mode |= _DESCR_;
	return	mode;
};

DWORD	CBEDriveMode_ST::EdgeMode(){
	DWORD	mode = 0;
	if (EG_RECT)	mode |= _RECT_;
	if (EG_DIRECT)	mode |= _DIRECTION_;
	if (EG_PROC)	mode |= _PROC_TIME_;
	if (EG_START)	mode |= _START_TIME_;
	if (EG_MODE)	mode |= _MODE_;
	return	mode;
};

void	CBEDriveMode_ST::StartMission(){
	NodeView=false;

	EdgeView=false;

	PR_CREATE=true;
	PR_MAIN=true;
	PR_SQUAD=true;

	BE_vGRP_VISIBLE = false;
	GroupsMap()->SetVisible(BE_vGRP_VISIBLE);	

	PROCESS=true;
};

void	CBEDriveMode_ST::StartMissionAfterLoad(){
	NodeView=false;

	EdgeView=false;

	PR_CREATE=false;
	PR_MAIN=true;
	PR_SQUAD=true;

	BE_vGRP_VISIBLE = false;
	GroupsMap()->SetVisible(BE_vGRP_VISIBLE);	

	PROCESS=true;
};



void	CBEDriveMode_ST::CBE_CREATE_DEFAULT_VG::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM==NULL)	return;
	
	LinearArray<int,_int>	natBrigList;
	LinearArray<int,_int>	vgBrigList;
    int curNAT=Nat;
	
	if (0<=curNAT&&curNAT<7){
		// Find all Brigads on map for this nation
		natBrigList.Clear();	
		for (int b=0; b<MaxBrig-11; b++){
			if ( CITY[curNAT].Brigs[b].Enabled==true ){
				natBrigList.Add(CITY[curNAT].Brigs[b].ID);
			};
		};
		// Find all Brigads on map in vGROUP for this nation 
		vgBrigList.Clear();
		GroupsMap()->GetBrigList(vgBrigList,curNAT);
		// Delete all meet elements from natBrigList
		int natN=natBrigList.GetAmount();
		bool	del = false;
		while (natN--) {
			int vgN=vgBrigList.GetAmount();
			while (!del&&vgN--) {
				if (natBrigList[natN]==vgBrigList[vgN])	del=true;
			};
			if (del) {
				natBrigList.Del(natN,1);
			};
		};
		// Create vGROUP and set name for them from natBrigList
        natN = natBrigList.GetAmount();
		if (natN!=0){
			_str	nameSQ;
			while (natN--) {
				nameSQ = "vgSQ_N";
				nameSQ += curNAT;
				nameSQ += "_";
				nameSQ += (int)(natBrigList[natN]);
				be_CGROUP* pGrp = GroupsMap()->GetGroupNM(nameSQ.pchar());
				if (pGrp==NULL) {
					GroupsMap()->AddGroup(nameSQ.pchar());
					be_CGROUP* pGrp = GroupsMap()->GetGroupNM(nameSQ.pchar());
					if (pGrp!=NULL) {
						pGrp->AddBrigad(natBrigList[natN],curNAT);
					};	
				};
			};
		};
	};
};

void	CBEDriveMode_ST::CBE_COPY_EDGE::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL) {
		lvCEdge* pNewEdge = NULL;
		pDM->SelectedEdge->GetCopy( (lvCStorage**)(&pNewEdge) );
		if (pNewEdge!=NULL) {
			pNewEdge->begID=pDM->SelectedNodeBeg->GetObjectGlobalID();
			pNewEdge->endID=pDM->SelectedNodeEnd->GetObjectGlobalID();
			BattleShema()->vAddEdge(pDM->BE_vGROUPS.Get()->GetObjectGlobalID(),pNewEdge);
		};
	};
};

void	CBEDriveMode_ST::CBE_SELECT_SQUAD::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL) {
		lvCSquardShema* pNewSS=BattleShema()->vGetSqShemaID(DriveMode()->GetSquardID());
		if (pNewSS!=NULL) {
			if (pDM->SelectedSquadShema!=NULL) {
				pDM->SelectedSquadShema->Select(false);
				if (pDM->SelectedSquadShema!=pNewSS) {
					pNewSS->Select(true);
					pDM->SelectedSquadShema=pNewSS;
				}else{
					pDM->SelectedSquadShema=NULL;
				};
			}else{
				pNewSS->Select(true);
				pDM->SelectedSquadShema=pNewSS;
			};
		};
	};
};

class lvCPara : public BaseClass{
public:
	int OldID;
	int NewID;
};
class lvCParaArr : public BaseClass
{
public:
	ClonesArray<lvCPara>	ParaArr;

	void	Clear()				{ ParaArr.Clear(); };
	void	Add(lvCPara* pPara)	{ ParaArr.Add(pPara); };
	int		GetNewID(int OldID)	{ 
		int N = ParaArr.GetAmount();
		int newID = 0;
		while (N--) {
			if (ParaArr[N]->OldID==OldID){
				N==0;
				newID=ParaArr[N]->NewID;
			};
		};
		return newID;
	};
};
void	CBEDriveMode_ST::CBE_COPY_SQUAD::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL) {
		be_CGROUP* pOldGrp = GroupsMap()->GetGroupID(pDM->BE_vGROUPS.Get()->GetObjectGlobalID());
		be_CGROUP* pNewGrp = GroupsMap()->GetGroupID(GrpID);
		if (pOldGrp!=NULL&&pNewGrp!=NULL) {
			int ox=0;
			int oy=0;
			pOldGrp->GetGroupCenter(ox,oy);
			int nx=0;
			int ny=0;
			pNewGrp->GetGroupCenter(nx,ny);
			int dx=nx-ox;
			int dy=ny-oy;

			// Create new Node for Squad Shema
			lvCParaArr	PARA;
			PARA.Clear();
			int N = NodesMap()->NodesArray.GetAmount();
			for (int i=0; i<N; i++){
				if (NodesMap()->NodesArray[i]->selected==true) {
					ox=NodesMap()->NodesArray[i]->x;
					oy=NodesMap()->NodesArray[i]->y;
					be_CNODE* pNewNode =	NodesMap()->vAddNode(ox+dx,oy+dy);
					lvCPara* pNewPara = new lvCPara();
					pNewPara->OldID=NodesMap()->NodesArray[i]->GetObjectGlobalID();
					pNewPara->NewID=pNewNode->GetObjectGlobalID();
					PARA.Add(pNewPara);
					pNewPara=NULL;
				};
			};

			// Create new Edge for new Squad Shema
			lvCEdge* pEdge=NULL;
			lvCSquardShema* pOldShema = BattleShema()->vGetSqShemaID(pDM->BE_vGROUPS.Get()->GetObjectGlobalID());
			lvCSquardShema* pNewShema = BattleShema()->vGetSqShemaID(GrpID);
			if (pOldShema!=NULL&&pNewShema!=NULL) {
				for (int i=0; i<pOldShema->SquardEdges.GetAmount(); i++){
					if (pOldShema->SquardEdges[i]->selected==true) {
						pOldShema->SquardEdges[i]->GetCopy( (lvCStorage**) (&pEdge) );
						if (pEdge!=NULL) {
							pEdge->id		= pNewShema->vGetFreeEdgeID();
							pEdge->begID	= PARA.GetNewID( pOldShema->SquardEdges[i]->begID );
							pEdge->endID	= PARA.GetNewID( pOldShema->SquardEdges[i]->endID );
							pNewShema->vAddEdge(pEdge);
						};
						pEdge=NULL;
					};
				};
			};
		};	
	};
};
void	CBEDriveMode_ST::CBE_ADD_MESSGES::EvaluateFunction(){
	if (Message.Get()!=NULL){
		AddMessageMM(Message.Get(),ParentName.pchar());
		Message.Set(NULL);
	};
};
void	CBEDriveMode_ST::CBE_FORM_SQUARD::EvaluateFunction(){
	//lvCCreateBrigade* pCurForm = new lvCCreateBrigade();
	//if (pCurForm==NULL)	return;
	//int N = SquadForm.GetAmount();
	//for (int i=0; i<N; i++){
	//	pCurForm->vGrp = SquadForm[i]->first_element;
	//	pCurForm->Use_VV = false;
	//	pCurForm->iSize = SquadForm[i]->second_element;
	//	pCurForm->Process(0);
	//};
};
void	CBEDriveMode_ST::CBE_WRITE_LOG::EvaluateFunction(){
	if (vvLOG.Get()!=NULL)	vvLOG.Get()->WriteToLogClass();
};
void	CBEDriveMode_ST::UpdateTimerData(){
	TempTimerClass* pCurTimer = NULL;
	for (int i=0; i<TimerData.GetAmount(); i++){
		pCurTimer = TimerData[i];
		if (0<=pCurTimer->TimerID&&pCurTimer->TimerID<32){
			GTimer* TM=SCENINF.TIME+pCurTimer->TimerID;
			pCurTimer->Time					= TM->Time;
			pCurTimer->Used					= TM->Used;
			pCurTimer->First				= TM->First;
			pCurTimer->LastCheckAnimTime	= TM->LastCheckAnimTime;
			pCurTimer->trueTime				= TM->trueTime;
		};
	};
};
char*	CBEDriveMode_ST::GetSaveSFileName(){
	if (BE_SAVE&&FileForScript.pchar()!=NULL) return FileForScript.pchar();
	return NULL;
};
void CBEDriveMode_ST::CBE_DelFilm::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL){
		if (pDM->scrFILM.Get()!=NULL){
			_str MessText;
			MessText = "Delete [";
			MessText += pDM->scrFILM.Get()->Name.pchar();
			MessText += "] Movie ???";
			if (MessageBox(hwnd,MessText.pchar(),"Delete",MB_ICONWARNING|MB_OK|MB_OKCANCEL)==IDOK) 
				BattleHandler()->DeleteFilm(pDM->scrFILM.Get()->Name.pchar());
		};
	};
};
void CBEDriveMode_ST::CBE_DEL_vGRP::EvaluateFunction(){
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL) {
		_str MessText;
		MessText = "Delete [";
		be_CGROUP* pvGRP = (be_CGROUP*)(pDM->BE_vGROUPS.Get());
		if (pvGRP!=NULL) MessText += pvGRP->Name.pchar();
		MessText += "] Group ???";
		if (MessageBox(hwnd,MessText.pchar(),"Delete",MB_ICONWARNING|MB_OK|MB_OKCANCEL)==IDOK){
			be_CVGROUP_ACT* pnewGROUP_ACT = new be_CVGROUP_ACT();
			pnewGROUP_ACT->vDelGroupPT(pvGRP);
			g_beACT_STORE.Add(pnewGROUP_ACT);
			pnewGROUP_ACT=NULL;
        //  GroupsMap()->DelGroupID(pDM->BE_vGROUPS.Get()->GetObjectGlobalID());
		};
	};
};
void CBEDriveMode_ST::CBE_SAVE_GROUP_STRUCT::EvaluateFunction(){
	if (FileName.pchar()==NULL)	return;
	LinearArray<int,_int> TYPES;
	GroupsMap()->GetStructNameList(TYPES);
	if (TYPES.GetAmount()>0) {
		FILE* pFile = fopen(FileName.pchar(),"w");
		if (pFile!=NULL) {
			for (int i=0; i<TYPES.GetAmount(); i++){
				fprintf(pFile,"%s%s",NATIONS[0].Mon[TYPES[i]]->MonsterID,"\r\n");
			};
			fclose(pFile);
		};
	};
};
void CBEDriveMode_ST::CBE_SAVE_GROUP_LUA::EvaluateFunction(){
	SimpleEditClass("Group list", (BaseClass*)( &(g_beVAR_STORE.GROUP_LIST) ) );
};
void CBEDriveMode_ST::CBE_SAVE_NODE_LUA::EvaluateFunction(){
	SimpleEditClass("Node list", (BaseClass*)( &(g_beVAR_STORE.NODE_LIST) ) );
};
void CBEDriveMode_ST::CBE_SAVE_GRAPH_LUA::EvaluateFunction(){
	SimpleEditClass("Graph list", (BaseClass*)(BattleHandler()) );
};
void CBEDriveMode_ST::CSK_TASK_ED::EvaluateFunction(){
	SimpleEditClass("Skirmish task editor", (BaseClass*)(&g_SkTASK) );
};
void CBEDriveMode_ST::CSK_TASK_ADD::EvaluateFunction(){
	g_addTask(NI,x,y,name.pchar()); // position on map in pixel
};
void CBEDriveMode_ST::CSK_TASK_DEL::EvaluateFunction(){
	g_delTask(name.pchar());
};
void CBEDriveMode_ST::CBE_SHOW_PROMF::EvaluateFunction(){	
	SimpleEditClass("MISSION PRO CALL", (BaseClass*)(&g_PMF) );
};
void CBEDriveMode_ST::CBE_MAS_EDITOR::EvaluateFunction(){
//	void	TEST_EDIT_ASS();
//	TEST_EDIT_ASS();
//	SimpleEditClass("Animete Picture store editor", (BaseClass*)(&g_TEST_ASS) );
};
void CBEDriveMode_ST::CBE_BEMI_INIT::EvaluateFunction(){
	extern BEMainInterface	g_BE_INTERF;
	g_BE_INTERF.INIT();
};
// TETS //
void CBEDriveMode_ST::CBE_Test_FnStore::EvaluateFunction(){
	g_WCP_EDITOR.LoadDEFMASK();
	BE_USAGE_FLAGS	OLD_FLAG = g_BE_USAGE_FLAG;
	g_BE_USAGE_FLAG = BE_IF_DEFF;
	SimpleEditClass("Function Storage", (BaseClass*)(&g_BE_FN_STORE) );
	g_BE_USAGE_FLAG = OLD_FLAG;
};
void CBEDriveMode_ST::CBE_Test_VAR_STORE::EvaluateFunction(){
	g_WCP_EDITOR.LoadDEFMASK();
	g_beVAR_STORE.UPDATE();
	be_CCAMERA::HideCameraTracks=true;
	BE_USAGE_FLAGS	OLD_FLAG = g_BE_USAGE_FLAG;
	g_BE_USAGE_FLAG = BE_IF_USE;
	SimpleEditClass("Variable Storage", (BaseClass*)(&g_beVAR_STORE) );
	g_BE_USAGE_FLAG = OLD_FLAG;
	be_CCAMERA::HideCameraTracks=false;
	// for recreate all enumerators 
	g_beOBJ_CONTROLER.SetState("be_CVariableBase"); 
};
void CBEDriveMode_ST::CBE_TempUID_TEST::EvaluateFunction(){
	SimpleEditClass("Test UniqID of units", (BaseClass*)(&g_beTempOBJECT) );
};
void CBEDriveMode_ST::CBE_SEND_MSG::EvaluateFunction(){
	be_CMSG_FROMUSER newMSG;
	SimpleEditClass("CREATE MSG", (BaseClass*)(&newMSG) );
	g_beRouteMessage( &newMSG );
};
void CBEDriveMode_ST::CBE_REM_LAST_ACTION::EvaluateFunction(){
	g_beACT_STORE.removeLAST();
};
void CBEDriveMode_ST::CBE_DEL_ALL_ACTION::EvaluateFunction(){
	g_beACT_STORE.Clear();
};
void CBEDriveMode_ST::CBE_MOVE_VAR_TO_NEW_VAR::EvaluateFunction(){
	int					N = 0;
	be_CVariableBase*	pVAR = NULL;
	// INT
	N = vValuesMap()->V_NEW_VALUES.GetAmount();
	while (N--) {
		pVAR = vValuesMap()->V_NEW_VALUES[N];
		if ( strcmp(pVAR->GetClassName(),"be_CINT")==0 ){
			g_beVAR_STORE.INT_LIST.Add((be_CINT*)pVAR);
			vValuesMap()->V_NEW_VALUES[N]=NULL;
			vValuesMap()->V_NEW_VALUES.DelElement(N);
		};
	}
	// BOOL
	N = vValuesMap()->V_NEW_VALUES.GetAmount();
	while (N--) {
		pVAR = vValuesMap()->V_NEW_VALUES[N];
		if ( strcmp(pVAR->GetClassName(),"be_CBOOL")==0 ){
			g_beVAR_STORE.BOOL_LIST.Add((be_CBOOL*)pVAR);
			vValuesMap()->V_NEW_VALUES[N]=NULL;
			vValuesMap()->V_NEW_VALUES.DelElement(N);
		};
	}
	// DWORD
	N = vValuesMap()->V_NEW_VALUES.GetAmount();
	while (N--) {
		pVAR = vValuesMap()->V_NEW_VALUES[N];
		if ( strcmp(pVAR->GetClassName(),"be_CDWORD")==0 ){
			g_beVAR_STORE.DWORD_LIST.Add((be_CDWORD*)pVAR);
			vValuesMap()->V_NEW_VALUES[N]=NULL;
			vValuesMap()->V_NEW_VALUES.DelElement(N);
		};
	}
	// FLOAT
	N = vValuesMap()->V_NEW_VALUES.GetAmount();
	while (N--) {
		pVAR = vValuesMap()->V_NEW_VALUES[N];
		if ( strcmp(pVAR->GetClassName(),"be_CFLOAT")==0 ){
			g_beVAR_STORE.FLOAT_LIST.Add((be_CFLOAT*)pVAR);
			vValuesMap()->V_NEW_VALUES[N]=NULL;
			vValuesMap()->V_NEW_VALUES.DelElement(N);
		};
	}
	// ARRAY
	N = vValuesMap()->V_NEW_VALUES.GetAmount();
	while (N--) {
		pVAR = vValuesMap()->V_NEW_VALUES[N];
		if ( strcmp(pVAR->GetClassName(),"be_CARRAY")==0 ){
			g_beVAR_STORE.ARRAY_LIST.Add((be_CARRAY*)pVAR);
			vValuesMap()->V_NEW_VALUES[N]=NULL;
			vValuesMap()->V_NEW_VALUES.DelElement(N);
		};
	}
	// GROUP
	N = GroupsMap()->GROUPS.GetAmount();
	while (N--) {
		pVAR = GroupsMap()->GROUPS[N];
		if ( strcmp(pVAR->GetClassName(),"be_CGROUP")==0 ){
			g_beVAR_STORE.GROUP_LIST.Add((be_CGROUP*)pVAR);
			GroupsMap()->GROUPS[N]=NULL;
			GroupsMap()->GROUPS.DelElement(N);
		};
	}
	// NODE
	N = NodesMap()->NodesArray.GetAmount();
	while (N--) {
		pVAR = NodesMap()->NodesArray[N];
		if ( strcmp(pVAR->GetClassName(),"be_CNODE")==0 ){
			g_beVAR_STORE.NODE_LIST.Add((be_CNODE*)pVAR);
			NodesMap()->NodesArray[N]=NULL;
			NodesMap()->NodesArray.DelElement(N);
		};
	}
};
void CBEDriveMode_ST::CBE_UPDATE_GROUPS::EvaluateFunction(){
	g_beVAR_STORE.UPDATE();	
};
void CBEDriveMode_ST::CBE_Show_Cloud_List::EvaluateFunction(){
	SimpleEditClass("CLOUD DIALOG", (BaseClass*)(&g_beCloudTest) );
};
void CBEDriveMode_ST::CBE_Show_Info_Dialog::EvaluateFunction(){
	SimpleEditClass("INFO DIALOG", (BaseClass*)(&g_beInfoDialog) );
};
void CBEDriveMode_ST::CBE_ShowCloudDialog::EvaluateFunction(){
	be_gShowCloudDialog( (be_CSTR_DIALOG*)refDialog.Get(), (be_CGROUP*)refGroup.Get() );	
};
void CBEDriveMode_ST::CBE_EditFSM_List::EvaluateFunction(){
	SimpleEditClass("Edit FSM Global List", (BaseClass*)(&g_beFSM_LOADER) );
};
// CAMERA
void CBEDriveMode_ST::CBE_SaveCamera::EvaluateFunction(){
	be_CCAMERA* pStorCamera = (be_CCAMERA*)(curEditCamera.Get());
	if ( pStorCamera==NULL ){
		pStorCamera = new be_CCAMERA();
		g_beVAR_STORE.CAMERA_LIST.Add(pStorCamera);
	};
	DriveMode()->Camera.Copy(pStorCamera,false);
	pStorCamera->SetName(DriveMode()->Camera.GetName());
	pStorCamera = NULL;
};
void CBEDriveMode_ST::CBE_LoadCamera::EvaluateFunction(){
	be_CCAMERA* pStorCamera = (be_CCAMERA*)(curEditCamera.Get());
	if ( pStorCamera==NULL ) return;
	pStorCamera->Copy(&(DriveMode()->Camera),false);
	DriveMode()->Camera.SetName(pStorCamera->GetName());
	pStorCamera = NULL;
};
// ------
void CBEDriveMode_ST::UPDATE_LUA_GC(){
	GC_COUNT = BattleHandler()->GET_GC_COUNT();
	GC_TRESHOLD = BattleHandler()->GET_GC_THRESHOLD();
};
void CBEDriveMode_ST::CBE_HTT_UPDATE::EvaluateFunction(){
	g_beHTT_LIST[MyNation][DriveMode()->LockType].Update();
};
void CBEDriveMode_ST::CBE_HTT_AST_ARRAY::EvaluateFunction(){
	//ClonesArray< LinearArray<int,_int> > AreasArray;
	//if ( g_beHTT_LIST[Nation][LockType].AST_getConectedList(Power,AreasArray) ){
	//	SimpleEditClass("ASSISTANS ON MAP",((BaseClass*)&AreasArray));		
	//};

	g_beHTT_LIST[Nation][LockType].AST_showPCL(Power);
};
void CBEDriveMode_ST::CBE_HTT_AST_CREATE_WAY::EvaluateFunction(){
	g_beFillFULLWAY_ATTACK(startAreaID,finishAreaID,Nation,MaxDanger,DangSteps,LockType);
};
void CBEDriveMode_ST::CBE_COLOR_SETINGS_SAVE::EvaluateFunction(){
	EngSettings.WriteToFile("EngineSettings.xml");
};
/////////
// CBE_HANDLER ///////////////////////////////////////////////////////////
bool CBE_HANDLER::OnGameSaving(xmlQuote& xml){
/*
	GroupsMap()->BeforeSave();
	vValuesMap()->DeleteAllSeparators();
	g_beVAR_STORE.UPDATE();

	xmlQuote*	pxmlValueMap	= new_xmlQuote( vValuesMap()->ClassName.pchar() );
	xmlQuote*	pxmlNodeMap		= new_xmlQuote( NodesMap()->ClassName.pchar()	);	
	xmlQuote*	pxmlGroupsMap	= new_xmlQuote( GroupsMap()->ClassName.pchar() );
	xmlQuote*	pxmlBattleShema	= new_xmlQuote( BattleShema()->ClassName.pchar() );
	xmlQuote*	pxmlMainScript	= new_xmlQuote( BattleHandler()->MAIN_SCRIPT.ClassName.pchar() );
	xmlQuote*	pxmlScriptFilms	= new_xmlQuote( "MissionScriptFilms" );
	xmlQuote*	pxmlScriptGraph	= new_xmlQuote( "MissionScriptGraph" );
	xmlQuote*	pxmlCameraPoss  = new_xmlQuote( "CameraPossitions"   );
	xmlQuote*	pxml_gMessagesMap = new_xmlQuote( "NewMessagesMap" );
	xmlQuote*	pxml_MISS_SET	= new_xmlQuote( "MissionSettings" );
	xmlQuote*	pxml_LUA		= new_xmlQuote( "LUA" );
	xmlQuote*	pxml_VAR		= new_xmlQuote( "VARIABLES" );
	xmlQuote*	pxml_FN			= new_xmlQuote( "FUNCTIONS" );
	// Save all additional information for mission.
	xmlQuote*	pxmlDelayedMsgSTOR	= new_xmlQuote( "DelayedMsgStore"  );
	xmlQuote*	pxmlAGroupCurSCR	= new_xmlQuote( "AGroupCurScreept" );
	xmlQuote*	pxmlVGroupCurSCR	= new_xmlQuote( "VGroupCurScreept" );


	vValuesMap()->Save(*pxmlValueMap,vValuesMap());
	NodesMap()->Save(*pxmlNodeMap,NodesMap());
	GroupsMap()->Save(*pxmlGroupsMap,GroupsMap());
	BattleShema()->Save(*pxmlBattleShema,BattleShema());
	BattleHandler()->MAIN_SCRIPT.Save(*pxmlMainScript,&(BattleHandler()->MAIN_SCRIPT));
	BattleHandler()->SCRIPT_FILMS.Save(*pxmlScriptFilms,&(BattleHandler()->SCRIPT_FILMS));
	BattleHandler()->SCRIPT_GRAPH.Save(*pxmlScriptGraph,&(BattleHandler()->SCRIPT_GRAPH));
	CameraPositons()->Save(*pxmlCameraPoss,CameraPositons());
	gMessagesMap.Save(*pxml_gMessagesMap,&gMessagesMap);
	gMISS_SET.Save(*pxml_MISS_SET,&gMISS_SET);
	BattleHandler()->LUAC.Save(*pxml_LUA,&(BattleHandler()->LUAC));
	g_beVAR_STORE.Save(*pxml_VAR,&g_beVAR_STORE);
	g_BE_FN_STORE.Save(*pxml_FN,&g_BE_FN_STORE);
	// Save all additional information for mission.
	g_beMasterDelayedMessage.Save(*pxmlDelayedMsgSTOR,&g_beMasterDelayedMessage);
	BattleHandler()->SQUARDS_SCRIPTS.Save(*pxmlAGroupCurSCR,&(BattleHandler()->SQUARDS_SCRIPTS));
	BattleHandler()->vGROUPS_SCRIPTS.Save(*pxmlVGroupCurSCR,&(BattleHandler()->vGROUPS_SCRIPTS));


	xml.AddSubQuote(pxmlValueMap);
	xml.AddSubQuote(pxmlNodeMap);
	xml.AddSubQuote(pxmlGroupsMap);
	xml.AddSubQuote(pxmlBattleShema);
	xml.AddSubQuote(pxmlMainScript);
	xml.AddSubQuote(pxmlScriptFilms);
	xml.AddSubQuote(pxmlScriptGraph);
	xml.AddSubQuote(pxmlCameraPoss);
	xml.AddSubQuote(pxml_gMessagesMap);
	xml.AddSubQuote(pxml_MISS_SET);
	xml.AddSubQuote(pxml_LUA);
	xml.AddSubQuote(pxml_VAR);
	xml.AddSubQuote(pxml_FN);
	// Save all additional information for mission.
	xml.AddSubQuote(pxmlDelayedMsgSTOR);
	xml.AddSubQuote(pxmlAGroupCurSCR);
	xml.AddSubQuote(pxmlVGroupCurSCR);


	GroupsMap()->AfterSave();
*/
	GroupsMap()->BeforeSave();
	vValuesMap()->DeleteAllSeparators();

	g_beVAR_STORE.UPDATE();

	xml.SetQuoteName("BATTLE_EDITOR_ALL_SCRIPT_v1.0");

	xmlQuote*	pxml_SCRIPT		= new_xmlQuote( "SCRIPT" );
	xmlQuote*	pxml_VAR		= new_xmlQuote( "VARIABLES" );
	xmlQuote*	pxml_FN			= new_xmlQuote( "FUNCTIONS" );
	xmlQuote*	pxml_DMSG		= new_xmlQuote( "DELAYEDMESS" );

	g_beSCRIPT_STORE.Save(*pxml_SCRIPT,&g_beSCRIPT_STORE);
	g_beVAR_STORE.Save(*pxml_VAR,&g_beVAR_STORE);
	g_BE_FN_STORE.Save(*pxml_FN,&g_BE_FN_STORE);
	g_beMasterDelayedMessage.Save(*pxml_DMSG,&g_beMasterDelayedMessage);

	xml.AddSubQuote(pxml_SCRIPT);
	xml.AddSubQuote(pxml_VAR);
	xml.AddSubQuote(pxml_FN);
	xml.AddSubQuote(pxml_DMSG);


	GroupsMap()->AfterSave();

	return true;
};
bool CBE_HANDLER::OnGameLoading(xmlQuote& xml){
/*	
	BattleHandler()->LUA_SAFE_CLOSE_MISS();

	// Delete Alert after LOAD
	g_DeleteAllAlert();

	REG_BE_FUNCTIONS_class();
	REG_BE_CONDITION_class();
	REG_BE_DataStorageXML_class();

	be_gClearCloudDilogSPEAKERS();

	vValuesMap()->reset_class(vValuesMap());
	NodesMap()->reset_class(NodesMap());
	GroupsMap()->reset_class(GroupsMap());
	BattleShema()->reset_class(BattleShema());
	BattleHandler()->MAIN_SCRIPT.reset_class(&(BattleHandler()->MAIN_SCRIPT));
	BattleHandler()->SCRIPT_FILMS.reset_class(&(BattleHandler()->SCRIPT_FILMS));
	BattleHandler()->SCRIPT_GRAPH.reset_class(&(BattleHandler()->SCRIPT_GRAPH));
	CameraPositons()->reset_class(CameraPositons());
	gMessagesMap.reset_class(&gMessagesMap);
	gMISS_SET.reset_class(&gMISS_SET);
	BattleHandler()->LUAC.reset_class(&(BattleHandler()->LUAC));
	g_beVAR_STORE.CLEAR();
	g_BE_FN_STORE.CLEAR();
	// Load all additional information for mission.
	g_beMasterDelayedMessage.reset_class(&g_beMasterDelayedMessage);
	BattleHandler()->SQUARDS_SCRIPTS.reset_class(&(BattleHandler()->SQUARDS_SCRIPTS));
	BattleHandler()->vGROUPS_SCRIPTS.reset_class(&(BattleHandler()->vGROUPS_SCRIPTS));


	GroupsMap()->BeforeLoad();

	ErrorPager	Err(0);
	if (xml.GetSubQuote(0)!=NULL)	vValuesMap()->Load(*(xml.GetSubQuote(0)),vValuesMap(),&Err);
	if (xml.GetSubQuote(1)!=NULL)	NodesMap()->Load(*(xml.GetSubQuote(1)),NodesMap(),&Err);										
	if (xml.GetSubQuote(2)!=NULL)	GroupsMap()->Load(*(xml.GetSubQuote(2)),GroupsMap(),&Err);									
	if (xml.GetSubQuote(3)!=NULL)	BattleShema()->Load(*(xml.GetSubQuote(3)),BattleShema(),&Err);
	if (xml.GetSubQuote(4)!=NULL)	BattleHandler()->MAIN_SCRIPT.Load(*(xml.GetSubQuote(4)),&(BattleHandler()->MAIN_SCRIPT),&Err);
	if (xml.GetSubQuote(5)!=NULL)	BattleHandler()->SCRIPT_FILMS.Load(*(xml.GetSubQuote(5)),&(BattleHandler()->SCRIPT_FILMS),&Err);
	if (xml.GetSubQuote(6)!=NULL)	BattleHandler()->SCRIPT_GRAPH.Load(*(xml.GetSubQuote(6)),&(BattleHandler()->SCRIPT_GRAPH),&Err);
	if (xml.GetSubQuote(7)!=NULL)	CameraPositons()->Load(*(xml.GetSubQuote(7)),CameraPositons(),&Err);
	if (xml.GetSubQuote(8)!=NULL)	gMessagesMap.Load(*(xml.GetSubQuote(8)),&gMessagesMap,&Err);
	if (xml.GetSubQuote(9)!=NULL)	gMISS_SET.Load(*(xml.GetSubQuote(9)),&gMISS_SET,&Err);
	if (xml.GetSubQuote(10)!=NULL)	BattleHandler()->LUAC.Load(*(xml.GetSubQuote(10)),&(BattleHandler()->LUAC),&Err);
	if (xml.GetSubQuote(11)!=NULL)	g_beVAR_STORE.Load(*(xml.GetSubQuote(11)),&g_beVAR_STORE,&Err);
	if (xml.GetSubQuote(12)!=NULL)	g_BE_FN_STORE.Load(*(xml.GetSubQuote(12)),&g_BE_FN_STORE,&Err);
	// Load all additional information for mission.
	if (xml.GetSubQuote(13)!=NULL)	
		g_beMasterDelayedMessage.Load(*(xml.GetSubQuote(13)),&g_beMasterDelayedMessage,&Err);
	if (xml.GetSubQuote(14)!=NULL)	
		BattleHandler()->SQUARDS_SCRIPTS.Load(*(xml.GetSubQuote(14)),&(BattleHandler()->SQUARDS_SCRIPTS),&Err);
	if (xml.GetSubQuote(15)!=NULL)	
		BattleHandler()->vGROUPS_SCRIPTS.Load(*(xml.GetSubQuote(15)),&(BattleHandler()->vGROUPS_SCRIPTS),&Err);

	GroupsMap()->AfterLoad();

	g_beVAR_STORE.UPDATE_AFTER_LOAD();	// New - need for in OneObject reference for group contenid it

	BattleHandler()->LastTime=BattleHandler()->Time=0;
	DriveMode()->StartMissionAfterLoad();

	CSingleMessagereCreate=true;
*/
	REG_BE_FUNCTIONS_class();
	REG_BE_CONDITION_class();
	REG_BE_DataStorageXML_class();

	be_gClearCloudDilogSPEAKERS();
	be_gShowCloudDialog(NULL,NULL);

	lvCBattleMapSaver::ResetAllClass();

	GroupsMap()->BeforeLoad();

	
			lvCBattleMapSaver::LoadSubQuote( xml, "FUNCTIONS"	, &g_BE_FN_STORE );
			lvCBattleMapSaver::LoadSubQuote( xml, "VARIABLES"	, &g_beVAR_STORE );
	if (	lvCBattleMapSaver::LoadSubQuote( xml, "SCRIPT"		, &g_beSCRIPT_STORE )==false ){
		xmlQuote* pSubQuoter = xml.GetSubQuote("SCRIPT");
		if ( pSubQuoter==NULL ) pSubQuoter = xml.GetSubQuote(BattleHandler()->MAIN_SCRIPT.ClassName.pchar());
		pSubQuoter = pSubQuoter->GetSubQuote("MAIN_NEW_SCRIPT");
		ErrorPager Err;
		if ( pSubQuoter ) g_beSCRIPT_STORE.MAIN_SCRIPT.Load( *pSubQuoter, &g_beSCRIPT_STORE.MAIN_SCRIPT, &Err );
	};
			lvCBattleMapSaver::LoadSubQuote( xml, "DELAYEDMESS"	, &g_beMasterDelayedMessage );

	GroupsMap()->AfterLoad();

	g_beVAR_STORE.UPDATE_AFTER_LOAD();	// New - need for in OneObject reference for group contenid it

	return true;
};
void CBE_HANDLER::OnInitAfterGameLoading(){
	DriveMode()->BE_vGRP_VISIBLE = false;
	DriveMode()->NodeView = 0;
	CBEDriveMode_ST* pDM = DriveMode();
	if (pDM!=NULL) {
		pDM->PR_MAIN = true;
		pDM->PR_SQUAD= true;
		pDM->PROCESS = true;
	};
}
bool CBE_HANDLER::OnCheckingBuildPossibility(byte NI,int Type,int& x,int& y){
	int N = gMISS_SET.DZone.GetAmount();
	bool	ret = true;
	int xxx=-1,yyy=-1,RRR=-1;
	int Nat=0;
	while (ret&&N--) {
		Nat = gMISS_SET.DZone[N]->Nat;
			
		if (gMISS_SET.DZone[N]->UseNode) {
			be_CNODE* pNode = NodesMap()->vGetNode(gMISS_SET.DZone[N]->Node);
			if (pNode!=NULL) {
				xxx=pNode->vGetX();
				yyy=pNode->vGetY();
				RRR=pNode->vGetR();
			};
		}else{
			int ZoneID = gMISS_SET.DZone[N]->Zone;
			if (0<AZones.GetAmount()&&ZoneID<AZones.GetAmount()&&AZones[ZoneID]!=NULL&&Nat==NI){
				ActiveZone* AZ=AZones[ZoneID];
				if (AZ!=NULL) {
					xxx=AZ->x;
					yyy=AZ->y;
					RRR=AZ->R;
				};
			};
		};
		
		if ( (xxx!=-1&&yyy!=-1&&RRR!=-1) && Norma((xxx)-(x>>4),(yyy)-(y>>4))<RRR )	ret=false;
		xxx=yyy=RRR=-1;
	};
	return ret;
};
void CBE_HANDLER::OnClassRegistration(){
	REG_CLASS(TempTimerClass);

	REG_CLASS(be_CTemp_OOLIST);
	REG_CLASS(be_CTemp_UniqID_Test);

	REG_CLASS(CBEDriveMode_ST);

	pSquardsOnMap = new lvCSquardsOnMap;
	Enumerator* E=ENUM.Get("ALL_GROUPS_ON_MAP");
	E->SetProcEnum(pSquardsOnMap);

	pZonesOnMap = new lvCZonesOnMap;
	E=ENUM.Get("ALL_ZONES_ON_MAP");
	E->SetProcEnum(pZonesOnMap);

	// vvTEXT
	E=ENUM.Get("ALL_vvTEXT_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_vvTEXT_ON_MAP_ENUM));
	// vvPOINT2D
	E=ENUM.Get("ALL_vvPOINT2D_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_vvPOINT2D_ON_MAP_ENUM));
	// vvPOINT_SET
	E=ENUM.Get("ALL_vvPOINT_SET_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_vvPOINT_SET_ON_MAP_ENUM));
	// vvVector3D
	E=ENUM.Get("ALL_vvVector3D_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_vvVector3D_ON_MAP_ENUM));
	// vvDIALOG
	E=ENUM.Get("ALL_vvDIALOG_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_vvDIALOG_ON_MAP_ENUM));
	// vvMissionLOG
	E=ENUM.Get("ALL_vvMissionLOG_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_vvMissionLOG_ON_MAP_ENUM));
	// lvCGraphObject
	E=ENUM.Get("ALL_lvCGraphObject_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_lvCGraphObject_ON_MAP_ENUM));
	// lvCDeffFilmMenu
	E=ENUM.Get("ALL_lvCDeffFilmMenu_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_lvCDeffFilmMenu_ON_MAP_ENUM));
	// lvCFilm
	E=ENUM.Get("ALL_lvCFilm_ON_MAP");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_lvCFilm_ON_MAP_ENUM));
		
	// MSG
	g_beREG_MSG_ROUTER_CLASS();
    
	// REG LEGO ENUMS
	g_beREG_ALL_BE_ENUM();

	// REG LEGO Classes
	be_REG_CLASS_MissGraph();
	be_REG_CLASS_Variables();
	be_REG_CLASS_VariablesStore();
	be_REG_CLASS_Parameter();
	be_REG_CLASS_Function();
	be_REG_CLASS_CFMODES();
	be_REG_CALSS_ComplexFunction();
	be_REG_CLASS_ParamFunction();
	be_REG_CLASS_HashTop();
	be_REG_CLASS_ScriptsStore();
	be_REG_CLASS_Debuger();
	be_REG_CLASS_NodesArray();
    
	REG_BE_DataStorageXML_class();
	REG_BE_FUNCTIONS_class();
	REG_BE_CONDITION_class();
	
	REG_CLASS(WCP_MASK_FILE);
	REG_CLASS(WCP_Editor);
	REG_CLASS(CPRESENTATION);

    REG_BE_CLASS();

	g_beDebuger.GetObjectGlobalID();
	be_CNodeArrayDriver::GetObjectGlobalID();
	be_CNodesDriver::GetObjectGlobalID();
	be_CGroupsDriver::GetObjectGlobalID();
	be_CSubfunctionDriver::GetObjectGlobalID();
	be_CLegoDriver::GetObjectGlobalID();

//	void	RegAnimateClassForBigMap();
//	RegAnimateClassForBigMap();
};

void CBE_HANDLER::OnInitAfterMapLoading(){

	// Update UniqID for units
	DWORD lastID = 0x00000000;
	OneObject* pOO = NULL;
	for ( int i=0; i<ULIMIT; i++ ){
		pOO = Group[i];
		if (pOO && pOO->UniqID>lastID) lastID = pOO->UniqID;
	};

	ActiveUnitAbility::CurSerial = lastID+1;
	addrand(ActiveUnitAbility::CurSerial);

	DriveMode()->PR_MODE	= 0;
	DriveMode()->PR_CREATE	= true; 
	DriveMode()->PR_MAIN	= true;
	DriveMode()->PR_SQUAD	= true;

	// TEST LUA IN RUN MISSION
	DriveMode()->InitSO		= true;

	//if (BattleHandler()->MAIN_SCRIPT.MAIN_INIT.GetAmount()>0	|| 
	//	BattleHandler()->MAIN_SCRIPT.MAIN_SCRIPTS.GetAmount()>0 ||
	//	BattleHandler()->SQUARDS_SCRIPTS.GetAmount()>0			||
	//	BattleHandler()->vGROUPS_SCRIPTS.GetAmount()>0				){
	//		DriveMode()->PR_MODE	= 0;
	//		DriveMode()->PR_CREATE	= true; 
	//		DriveMode()->PR_MAIN	= true;
	//		DriveMode()->PR_SQUAD	= true;
	//		DriveMode()->PROCESS	= true;
	//	};

	g_SkTASK.Check();
};

void CBE_HANDLER::OnUnloading(){

	void Kill_g_refs();
	Kill_g_refs();
	extern int smartobj_curPos;
	smartobj_curPos=0;
	BattleHandler()->LUA_SAFE_CLOSE_MISS();


	// TEST temp dalete all value storge
	g_beVAR_STORE.CLEAR();
	g_BE_FN_STORE.CLEAR();
	g_beSCRIPT_STORE.CLEAR();
	g_beDebuger.clear();
	be_CNodeArrayDriver::clear();
	be_CNodesDriver::clear();
	be_CGroupsDriver::clear();
	be_CSubfunctionDriver::clear();
	be_CLegoDriver::clear();

	be_gClearCloudDilogSPEAKERS();
	be_gShowCloudDialog(NULL,NULL);
	g_beInfoDialog.Visible=false;

	CBEDriveMode_ST::Delete();
	lvCNodesMAP_ST::Delete();
	lvCGroupsMAP_ST::Delete();
	vvMAP_ST::Delete();
	lvCBattleShema_ST::Delete();
	lvCScriptHandler_ST::Delete();
	lvCShapes::Delete();
	lvCRotateCamera::Delete();
	CameraPoss::Delete();

	ExperienceManager::Delete();

	g_beACT_STORE.Clear();
	ActivObject = NULL;

//	SetHelpMessagesArrays(SetCurrentHelp);
};

bool CBE_HANDLER::OnMapUnLoading(){
	g_DeleteAllAlert();
	BattleHandler()->LUA_SAFE_CLOSE_MISS();

	// TEST temp dalete all value storge
	g_beVAR_STORE.CLEAR();
	g_BE_FN_STORE.CLEAR();
	g_beSCRIPT_STORE.CLEAR();
	g_beDebuger.clear();
	be_CNodeArrayDriver::clear();
	be_CNodesDriver::clear();
	be_CGroupsDriver::clear();
	be_CSubfunctionDriver::clear();
	be_CLegoDriver::clear();
	be_gClearCloudDilogSPEAKERS();
	be_gShowCloudDialog(NULL,NULL);
	g_beInfoDialog.Visible=false;

	g_beACT_STORE.Clear();
	ActivObject = NULL;

	return true;
};
void CBE_HANDLER::OnEditorStart(){
	
	BattleHandler()->LUA_SAFE_CLOSE_MISS();

	// TEST temp dalete all value storge
	g_beVAR_STORE.CLEAR();
	g_BE_FN_STORE.CLEAR();
	g_beSCRIPT_STORE.CLEAR();
	g_beDebuger.clear();
	be_CNodeArrayDriver::clear();
	be_CNodesDriver::clear();
	be_CGroupsDriver::clear();
	be_CSubfunctionDriver::clear();
	be_CLegoDriver::clear();
	be_gClearCloudDilogSPEAKERS();
	be_gShowCloudDialog(NULL,NULL);
	g_beInfoDialog.Visible=false;

	g_beACT_STORE.Clear();
	ActivObject = NULL;

	// FOR DEBUGER
	//g_beDebuger.GetObjectGlobalID();
	//be_CNodeArrayDriver::GetObjectGlobalID();
	//be_CNodesDriver::GetObjectGlobalID();
	//be_CGroupsDriver::GetObjectGlobalID();
	//be_CLegoDriver::GetObjectGlobalID();
	//g_beSCRIPT_STORE.GetObjectGlobalID();

	// TEST /////
//	AR_FL.INIT();
};	
void UnitAbilityTest();
void CBE_HANDLER::ProcessingGame(){
	extern bool GecOptimBattleEditor;
	if(GecOptimBattleEditor) return;
	
	addrand(1928);
	//UnitAbilityTest();	
	g_beHTT_LIST.SafeUpdate(DriveMode()->UpdateDelta);
	
	BE_PROCESS_SCREPT();
	g_beDebuger.Process();
		
	addrand(2837);

	// TEST /////
//	AR_GR.PROCESS();	
};

bool CBE_HANDLER::OnAttemptToMove(OneObject* Unit,int x,int y){
	return GroupsMap()->__CheckMove(Unit);
};
void CBE_HANDLER::OnDrawOnMapAfterFogOfWar(){

	g_beVAR_STORE.DRAW();

	if ( DriveMode()->SmartObject.Get()!=NULL ) ((be_CVariableBase*)(DriveMode()->SmartObject.Get()))->vDraw();

	BattlePainter();

	GroupsMap()->DrawGroup();

	CameraPositons()->Draw();

	vValuesMap()->Draw();

	BattleHandler()->DRAW();

	gMessagesMap.DRAW();
	// TEST /////
//	AR_GR.ShowState();
};

void CBE_HANDLER::OnDrawOnMapOverAll(){
	if (DriveMode()->PROCESS){
		BattleHandler()->DRAW();
	};
	CameraDriver()->Draw();

	g_BE_INTERF.DRAW();

	g_beCloudTest.Draw();

	g_beDRAW_HashTopTable( g_beHTT_LIST[MyNation][DriveMode()->LockType] );

	// TEMP TEST
	g_beDrawGroupMenu();
	g_beDrawTree();
};

void CBE_HANDLER::OnDrawOnMiniMap(int x,int y,int Lx,int Ly){
//	if (DriveMode()->PROCESS){
//		vValuesMap()->DrawOnMiniMap(x,y,Lx,Ly);
//	};
	// for skirmish
	g_SkTASK.DrawOnMiniMap();
};
bool CBE_HANDLER::OnCheatEntering(const char* Cheat){
	if ( Cheat && strcmp(Cheat,"update UID")==0 ){
		int curUID = 0;
		for ( int i=0; i<ULIMIT; i++ ){
			if (Group[i]) Group[i]->UniqID=curUID;
			curUID++;
		};

		ActiveUnitAbility::CurSerial = curUID;
		addrand(ActiveUnitAbility::CurSerial);

	};
	
	if ( Cheat!=NULL && Cheat[0]!=0 ){
		if ( strcmp("sdd",Cheat)==0 ){
			g_WCP_EDITOR.LoadDEFMASK();
			g_beDebuger._active = false;
			g_beDebuger.setVisible(true);
			return true;
		}
		if ( strcmp("update sdd",Cheat)==0 ){
			g_beDebuger._dialog.OnInit();
			return true;
		}
		if ( strcmp("nae",Cheat)==0 ){
			g_WCP_EDITOR.LoadDEFMASK();
			be_CNodeArrayDriver::setVisible(true);
			return true;
		}
		if ( strcmp("update nae",Cheat)==0 ){
			be_CNodeArrayDriver::OnInit();
			return true;
		}
		if ( strcmp("ndd",Cheat)==0 ){
			be_CNodesDriver::setVisible(true);
			return true;
		}
		if ( strcmp("update ndd",Cheat)==0 ){
			be_CNodesDriver::OnInit();
			return true;
		}
		if ( strcmp("gdd",Cheat)==0 ){
			be_CGroupsDriver::setVisible(true);
			return true;
		}
		if ( strcmp("update gdd",Cheat)==0 ){
			be_CGroupsDriver::OnInit();
			return true;
		}
		if ( strcmp("sfdd",Cheat)==0 ){
			be_CSubfunctionDriver::setVisible(true);
			return true;
		}
		if ( strcmp("update sfdd",Cheat)==0 ){
			be_CSubfunctionDriver::OnInit();
			return true;
		}
		if ( strcmp("lego",Cheat)==0 ){
			be_CLegoDriver::setVisible(true);
			return true;
		}
		if ( strcmp("update lego",Cheat)==0 ){
			be_CLegoDriver::OnInit();
			return true;
		}
		if ( strlen(Cheat)>3 && ( Cheat[0]=='g' || Cheat[0]=='G' ) && Cheat[1]==' ' ){
			_str newGruoupName = Cheat+2;
			be_CVGROUP_ACT* pnewGROUP_ACT = new be_CVGROUP_ACT();
			be_CGROUP* pvGRP = pnewGROUP_ACT->vAddGroup(newGruoupName.pchar());
			g_beACT_STORE.Add(pnewGROUP_ACT);
			if (pvGRP!=NULL) pvGRP->AddSelectedUnits();
			pnewGROUP_ACT=NULL;
			return true;
		}
	};

	return false;
};

bool CBE_HANDLER::OnMouseHandling(	int mx,int my,
									bool& LeftPressed,
									bool& RightPressed,
									int MapCoordX,int MapCoordY,
									bool OverMiniMap){
	if ( BE_HandlerMouse() || gMessagesMap.HANDLE(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap) ){	
		UnPress();
		return true;
	};

	if (vValuesMap()->OnMouseHandling(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap)) {
		UnPress();
		return true;
	};

	// Node Array Editor
	if ( be_CNodeArrayDriver::Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap) ){
		UnPress();
		return true;
	};
	// Nodes Editor
	if ( be_CNodesDriver::Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap) ){
		UnPress();
		return true;
	};
	// Groups Editor
	if ( be_CGroupsDriver::Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap) ){
		UnPress();
		return true;
	};
	// Subfunction Editor
	if ( be_CSubfunctionDriver::Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap) ){
		UnPress();
		return true;
	};
	// Lego Editor
	if ( be_CLegoDriver::Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap) ) {
		UnPress();
		return true;
	};
	
	g_beInfoDialog.Process(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap);

	return	false;
};

bool CBE_HANDLER::OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType){
	if ( Damager==NULL || DamagedUnit==NULL || Damage==0 ) return true;
//	int AID = Damager.Index;
	int TID = DamagedUnit->Index;
	/*
	int N = g_beVAR_STORE.SMARTOBJ_LIST.GetAmount();
	be_CVariableBase* pGRP = NULL;
	for ( int i=0; i<N; i++ ){
		pGRP = (be_CVariableBase*)(g_beVAR_STORE.SMARTOBJ_LIST[i]->ControledObject.Get());
		if ( pGRP!=NULL && strcmp(pGRP->GetClassName(),"be_CGROUP")==0 && ((be_CGROUP*)pGRP)->UnderAttack==false ) {
			if ( ( (be_CGROUP*)pGRP )->_CheckGID(TID)!=0xFFFF ) ((be_CGROUP*)pGRP)->UnderAttack=true;
		};
	};
	*/
	// new version for all units on map in group
//	int		N = g_beVAR_STORE.GROUP_LIST.GetAmount();
	for ( int i=0; i<DamagedUnit->GRP_N; i++ ){
		if ( DamagedUnit->pGRP_IDS[i]!=0xFFFF ){
			int B = CPGARB.GetBranch("be_CGROUP");
			DynArray<OneClassPointer*>& CPA=CPGARB.CPointer[B];
			int TCN = CPA.GetAmount();
			ReferableBaseClass* pRefOB = NULL;
			while (TCN--) {
				OneClassPointer* CP=CPA[TCN];
				if (CP!=NULL&&CP->ClassPtr!=NULL){
					pRefOB = (ReferableBaseClass*)CP->ClassPtr;
					if ( DamagedUnit->pGRP_IDS[i] == pRefOB->GetObjectGlobalID() ){
						( (be_CGROUP*)(pRefOB) )->UnderAttack = true;
					}
				}
			}

			//for ( int j=0; j<N; j++ ){
			//	if ( g_beVAR_STORE.GROUP_LIST[j]->GetObjectGlobalID()==DamagedUnit->pGRP_IDS[i] ){
			//		g_beVAR_STORE.GROUP_LIST[j]->UnderAttack = true;
			//	};
			//};
		};
	};
	return true;
};

void CBE_HANDLER::OnUnitWasProduced(OneObject* Producer,OneObject* NewObject){
	if (Producer==NULL&&NewObject==NULL) return;
	be_CMSG Msg("MSG_NEW_UNIT",Producer->Index,Producer->Index);
	Msg.ADD_PARAM("inewGID",NewObject->Index);
	g_beRouteMessage( (be_CMsgObject*)&Msg );
	return;
};

void BE_InstallExtension(){
	InstallExtension(new CBE_HANDLER, "Start BE Editor Handler");
};
//////////////////////////////////////////////////////////////////////////

// ProcessBattleEditor ///////////////////////////////////////////////////
void	ProcessBattleEditor(){

/*
	// FIRST CALL ////////////////////////////////////////////////////////////
	SET_BE_DrawOnMapCallback();

	ActivObject = NULL;

	static bool	first = true;
	if (first) {
		first=false;
		
		pSquardsOnMap = new lvCSquardsOnMap;
		Enumerator* E=ENUM.Get("ALL_GROUPS_ON_MAP");
		E->SetProcEnum(pSquardsOnMap);

		pZonesOnMap = new lvCZonesOnMap;
		E=ENUM.Get("ALL_ZONES_ON_MAP");
		E->SetProcEnum(pZonesOnMap);

	};
	//////////////////////////////////////////////////////////////////////////
	
	DriveMode()->visible = !DriveMode()->visible;
	BattleShema()->vUpdateGroups();

	return;
*/
};
//////////////////////////////////////////////////////////////////////////

// BE_PROCESS_SCREPT /////////////////////////////////////////////////////
void UnitAbilityTest();
void	BE_PROCESS_SCREPT(){
//	if (DriveMode()->OBJECT==2){	// "SCRIPT" selected
			
		// LUA
		// DriveMode()->UPDATE_LUA_GC();
		// ---	

		if (DriveMode()->PROCESS==true) {
			if (DriveMode()->PR_CREATE==true) {
				BattleHandler()->Create();
				DriveMode()->PR_CREATE = false;
			};
			switch(DriveMode()->PR_MODE) {
			case 0:		// "All"
			//	if (DriveMode()->PR_SQUAD) BattleHandler()->PROCESS();
				//UnitAbilityTest();

				BattleHandler()->LUA_SAFE_OPEN_MISS();
				//UnitAbilityTest();
				if (DriveMode()->InitSO) { g_beINITSMARTOBJECTS(); DriveMode()->InitSO=false; };
				g_beUPDATESMARTOBJ();
				//UnitAbilityTest();

				BattleHandler()->PROCESS_MAIN(DriveMode()->PR_MAIN);

				g_beSCRIPT_STORE.CALL_ALL_SCREPTS();

				//UnitAbilityTest();
				g_beVAR_STORE.PROCESS();
				//UnitAbilityTest();

				BattleHandler()->SET_GC_THRESHOLD(0);
				//UnitAbilityTest();

			//	BattleHandler()->LUA_CallFList();
				break;	
			case 1:		// "SQUAD"
				BattleHandler()->PROCESS(DriveMode()->GetSquardID());
				break;
			case 2:		// "SWITCHED"
				BattleHandler()->PROCESS(true);
				break;
			default:
				break;
			};
			BattleHandler()->TIMER(true);
		}else{
			BattleHandler()->TIMER(false);
			// Hide task list
			vvMESSGES* pMess = dynamic_cast<vvMESSGES*>(vValuesMap()->GetVValueTypeID(_vvMESSGES_));
			if (pMess!=NULL) {
				pMess->SetMessDS_Visbility(false);
			};
		};
		DriveMode()->PR_TIME = (int)(BattleHandler()->Time);
//	}else 
	
	if (DriveMode()->OBJECT==8) {
		DriveMode()->UpdateTimerData();
	};
	return;
}
//////////////////////////////////////////////////////////////////////////

// BE_HandlerMouse ///////////////////////////////////////////////////////
extern	int	DLG_X0;	extern	int DLG01_X0;		
extern	int	DLG_Y0;	extern	int DLG01_Y0;
extern	int	DLG_W;	extern	int DLG01_W;
extern	int	DLG_H;	extern	int DLG01_H;
extern	ClassArray<ActiveGroup> AGroups;
extern	int		mouseX;
extern	int		mouseY;
extern	int		LastMx;
extern	int		LastMy;
extern	bool	Lpressed;
extern	bool	Rpressed;
bool	BE_MouseInEditor(){
	bool	inEditor = (DLG_X0<=mouseX&&mouseX<=DLG_X0+DLG_W && DLG_Y0<=mouseY&&mouseY<=DLG_Y0+DLG_H);
	//if (DriveMode.NodeView==1/* VIEW ALL */ && DriveMode.NodeAct==3/* EDIT */ && ActivObject!=NULL) {
	//	inEditor = inEditor || (lvDLG01_X0<=mouseX&&mouseX<=lvDLG01_X0+lvDLG01_W && lvDLG01_Y0<=mouseY&&mouseY<=lvDLG01_Y0+lvDLG01_H);
	//};
	return inEditor;
};

lvCNodeActionsVI*	pObjectHendler = NULL;
bool	NODE_ACTIONS_HENDLER(){
	be_CVNODE_ACT* pnewNODE_ACT = NULL;
	
	switch(DriveMode()->NodeAct) {
			case 0:		// "ADD"
				if (Lpressed&&DriveMode()->NodeView==1) {
					pnewNODE_ACT = new be_CVNODE_ACT();
					pnewNODE_ACT->vAddNode(LastMx,LastMy);
					g_beACT_STORE.Add(pnewNODE_ACT);
					pnewNODE_ACT=NULL;
					Lpressed = false;
					return	true;
				};
				break;
			case 1:		// "DELETE"
				if (Lpressed) {
					if (DriveMode()->NodeView==1) {
						BattleShema()->vDelNode(LastMx,LastMy,0xFFFFFFFF);
					//	NodesMap()->vDelNode(LastMx,LastMy);
						pnewNODE_ACT = new be_CVNODE_ACT();
						pnewNODE_ACT->vDelNode(LastMx,LastMy);
						g_beACT_STORE.Add(pnewNODE_ACT);
						pnewNODE_ACT=NULL;
					};
					if (DriveMode()->NodeView==2) {
						if ( BattleShema()->vDelNode(LastMx,LastMy,DriveMode()->GetSquardID()) ){
							NodesMap()->vDelNode(LastMx,LastMy);
						};	
					};
					Lpressed = false;
					return true;
				};
				break;
			case 2:		// "MOVE"
				if (ActivObject==NULL && Lpressed) {
					pnewNODE_ACT = new be_CVNODE_ACT();
					ActivObject = pnewNODE_ACT->vGetNode(LastMx,LastMy);
					delete pnewNODE_ACT;
					if (ActivObject!=NULL) {
						Lpressed = false;
						return true;
					};
				};
				if (ActivObject!=NULL && Lpressed) {
					ActivObject = NULL;
					Lpressed = false;
					return true;
				};
				break;
			case 3:		// "EDIT"
				if (ActivObject==NULL && Lpressed) {
					pnewNODE_ACT = new be_CVNODE_ACT();
					ActivObject = pnewNODE_ACT->vGetNode(LastMx,LastMy);
					delete pnewNODE_ACT;
					if (ActivObject!=NULL) {
					//	SET_BE_NODE_CLASS_EDITOR(reinterpret_cast<be_CNODE*>(ActivObject),true);
						ReplaceEditor("BENodeEDIT",dynamic_cast<BaseClass*>(ActivObject));
						int lastItemChoose = ItemChoose;
						ItemChoose=-1;
						ProcessEditor("BENodeEDIT");
						ItemChoose = lastItemChoose;
						pActivEdge=NULL;
						Lpressed = false;
//						ItemChoose=-1;
						ActivObject=NULL;
						return	true;
					};
				};
				if (ActivObject!=NULL && Lpressed) {
					ActivObject = NULL;
				//	SET_BE_NODE_CLASS_EDITOR(NULL,true);
					Lpressed = false;
					return	true;
				};
				break;
			default:	// "NOTHING"
				return false;
		};
		return	false;
};

bool	BE_HandlerMouse(){
	// NOT ACTIVE
//	if (DriveMode()->visible==false)	return false;

	static int NGrpOnMap = AGroups.GetAmount();
	if (NGrpOnMap!=AGroups.GetAmount()) {
		NGrpOnMap = AGroups.GetAmount();
		EDT00.ExpInfChanged=true;
		BattleShema()->vUpdateGroups();
	};
	
	// MOUSE IN DIALOG MENU
//	if (BE_MouseInEditor())			return false;

	switch(DriveMode()->OBJECT) {
	case 0:		// "NODE"
		if (DriveMode()->NodeView==1) {	// "VIEW ALL"
			pObjectHendler = dynamic_cast<lvCNodeActionsVI*>(NodesMap());
		};
		if (DriveMode()->NodeView==2) {	// "FOR GROUP"
			pObjectHendler = dynamic_cast<lvCNodeActionsVI*>(BattleShema());
		};
		return	NODE_ACTIONS_HENDLER();
		break;
	case 1:		// "EDGE"
		if (DriveMode()->EdgeView==1) {	// "VIEW"
			switch(DriveMode()->EdgeAct) {
			case 0:			// "ADD"
				if (GetKeyState(VK_ESCAPE)&0x8000){
					if (pActivEdge!=NULL){
						delete pActivEdge;
					};
					pActivEdge=NULL;
					return true;
				}
				if (addNewNodeInAct==false&&Lpressed) {
					if (pActivEdge!=NULL){
						delete pActivEdge;
					};
					pActivEdge=NULL;
                    be_CNODE* pSelND = NodesMap()->vGetNode(LastMx,LastMy);
					if (pSelND!=NULL) {
						pActivEdge = new lvCEdge();
						pActivEdge->vINIT();
						pActivEdge->vSetBeg(pSelND->GetObjectGlobalID());
						pActivEdge->endID = 0;
						addNewNodeInAct=true;
						Lpressed=false;
						return true;
					};
				};
				if (addNewNodeInAct==true&&Lpressed) {
					be_CNODE* pSelND = NodesMap()->vGetNode(LastMx,LastMy);
					if (pSelND!=NULL&&pActivEdge!=NULL) {
						pActivEdge->vSetEnd(pSelND->GetObjectGlobalID());
                        BattleShema()->vAddEdge(DriveMode()->GetSquardID(),pActivEdge);
						pActivEdge = NULL;
						addNewNodeInAct=false;
						Lpressed=false;
						return true;
					};
				};
				break;
			case 1:			// "DELETE"
				if (Lpressed) {
					if ( BattleShema()->vDelEdge(DriveMode()->GetSquardID(),LastMx,LastMy) ){
						Lpressed=false;
						return true;
					};
				};
				break;
			case 2:			// "EDIT"
				if (Lpressed) {
					if (pActivEdge!=NULL) {
						delete pActivEdge;
						pActivEdge=NULL;
					};
					pActivEdge=BattleShema()->vGetEdge(DriveMode()->GetSquardID(),LastMx,LastMy);
					if (pActivEdge!=NULL) {
					//	SET_BE_EDGE_CLASS_EDITOR(pActivEdge,true);
					//	vValuesMap()->SetViewType(0x00000002);
						ReplaceEditor("BEEdgeEDIT",dynamic_cast<BaseClass*>(pActivEdge));
						ReplaceEditor("BEEdgeEDITEX",dynamic_cast<BaseClass*>(pActivEdge));
						int lastItemChoose = ItemChoose;
						ItemChoose=-1;
						if (DriveMode()->MainEditType==0){
							ProcessEditor("BEEdgeEDIT");
						}else if (DriveMode()->MainEditType==1) {
							MAIN_EDIT_PANEL.EP_VISIBLE(true);
							ProcessEditor("BEEdgeEDITEX");
						};				
						ItemChoose = lastItemChoose;
					//	vValuesMap()->SetViewType(0x00000001);
						pActivEdge=NULL;
						Lpressed=false;
//						ItemChoose=-1;
						return true;
					};
				};
				break;
			default:		// "NOTHING"
				return false;
			};
		}
		break;
	case 7:					// "EDIT"
		switch(DriveMode()->EditOperation) {
		case 0:	// "Select Edge"
			if (Lpressed) {
				lvCEdge* pNewSelect = BattleShema()->vGetEdge(DriveMode()->GetSquardID(),LastMx,LastMy);
				if (pNewSelect!=NULL) {
					if (DriveMode()->SelectedEdge!=NULL) {
						DriveMode()->SelectedEdge->selected=false;
					};
					if (pNewSelect!=DriveMode()->SelectedEdge) {
						DriveMode()->SelectedEdge=pNewSelect;
						DriveMode()->SelectedEdge->selected=true;
					}else{
						DriveMode()->SelectedEdge=NULL;
					};
					
					Lpressed=false;
				};
			};
			break;
		case 1:	// "Select Node Beg"
			if (Lpressed) {
				be_CNODE* pNewSelect = NodesMap()->vGetNode(LastMx,LastMy,DriveMode()->GetSquardID());
				if (pNewSelect!=NULL) {
					if (DriveMode()->SelectedNodeBeg!=NULL) {
						DriveMode()->SelectedNodeBeg->selected=false;
					};
					if (pNewSelect!=DriveMode()->SelectedNodeBeg) {
						DriveMode()->SelectedNodeBeg=pNewSelect;
						DriveMode()->SelectedNodeBeg->selected=true;
						DriveMode()->SelectedNodeBeg->type=0;
					}else{
						DriveMode()->SelectedNodeBeg=NULL;
					};
					
					Lpressed=false;
				};
			};
			break;
		case 2:	// "Select Node End"
			if (Lpressed) {
				be_CNODE* pNewSelect = NodesMap()->vGetNode(LastMx,LastMy,DriveMode()->GetSquardID());
				if (pNewSelect!=NULL) {
					if (DriveMode()->SelectedNodeEnd!=NULL) {
						DriveMode()->SelectedNodeEnd->selected=false;
					};
					if (pNewSelect!=DriveMode()->SelectedNodeEnd) {
						DriveMode()->SelectedNodeEnd=pNewSelect;
						DriveMode()->SelectedNodeEnd->selected=true;
						DriveMode()->SelectedNodeEnd->type=1;
					}else{
						DriveMode()->SelectedNodeEnd=NULL;
					};
					
					Lpressed=false;
				};
			};
			break;
		case 3:	// "Select Squad"
			// Anable Select and UnSlect Edge, Node.
			if (Lpressed) {
				be_CNODE* pNewSelectN = NodesMap()->vGetNode(LastMx,LastMy,DriveMode()->GetSquardID());
				lvCEdge* pNewSelectE = BattleShema()->vGetEdge(DriveMode()->GetSquardID(),LastMx,LastMy);
				if (pNewSelectN!=NULL){
					pNewSelectN->selected = !(pNewSelectN->selected);
					Lpressed=false;
				}else if (pNewSelectE!=NULL) {
					pNewSelectE->selected = !(pNewSelectE->selected);
					Lpressed=false;
				};
			};
			break;
		case 4:	// "Select Value"
			static int LastLx;
			static int LastLy;
			if (Lpressed) {
				if (DriveMode()->SelectedValue==NULL) {
					DriveMode()->SelectedValue = vValuesMap()->GetNearestValue(LastMx,LastMy);
					if (DriveMode()->SelectedValue!=NULL) {
						LastLx = LastMx;
						LastLy = LastMy;
						Lpressed=false;
					};
				}else{
					DriveMode()->SelectedValue=NULL;
					Lpressed=false;
				};
			};
			if (DriveMode()->SelectedValue!=NULL) {
				if (Rpressed) {
					DriveMode()->CorectionType = (DriveMode()->CorectionType+1)%5;
				};
				char s[5];
				int deltaX = LastMx-LastLx;
				int deltaY = LastMy-LastLy;
				LastLx=LastMx;LastLy=LastMy;
				switch(DriveMode()->CorectionType) {
				case 0:
					strcpy(s,"XY");
					DriveMode()->SelectedValue->SetPoss(LastMx,LastMy,0);
					break;
				case 1:
					if (DriveMode()->SelectedValue->InfID==_lvCTeraforming_) {
                        ((lvCTeraforming*)(DriveMode()->SelectedValue))->Set_r(deltaX);
					};
					strcpy(s,"r");
					break;
				case 2:
					if (DriveMode()->SelectedValue->InfID==_lvCTeraforming_) {
						((lvCTeraforming*)(DriveMode()->SelectedValue))->Set_R(deltaX);
					};
					strcpy(s,"R");
					break;
				case 3:
					if (DriveMode()->SelectedValue->InfID==_lvCTeraforming_) {
						((lvCTeraforming*)(DriveMode()->SelectedValue))->Set_h(-deltaY);
					};
					strcpy(s,"h");
					break;
				case 4:
					if (DriveMode()->SelectedValue->InfID==_lvCTeraforming_) {
						((lvCTeraforming*)(DriveMode()->SelectedValue))->Set_H(-deltaY);
					};
					strcpy(s,"H");
					break;
				};
				void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
				void WorldToScreenSpace ( Vector4D& vec );
				Vector4D	p((float)LastMx-10,(float)LastMy-10,(float)GetHeight(LastMx+15,LastMy+15),1);
				WorldToScreenSpace(p);
				ShowStringEx(p.x,p.y,s,&SmallWhiteFont);
			};
		default:	// "NOTHING"
			break;
		};
		break;
	default:	// "NOTHING"
		return false;
	};
	return	false;
};
//////////////////////////////////////////////////////////////////////////
bool	ggGetIndexedString(char* source,int pos,char* dest){
	int curPos=0;
	int curLen=-1;
	for (int i=0; i<strlen(source)+1; i++){
		if (source[i]==' '||source[i]=='\0') {
			curPos++;
		};
		if (curPos==pos){
			curLen++;
		};

		if (curPos==pos+1) {
			char sss[256];
			strcpy(sss,source+i-curLen-1);
			sss[curLen+1]='\0';
			strcpy(dest,sss);
			i=strlen(source);
		};
	};
	return (curLen!=0);
};
bool PrepareToShow_BE_MAIN(ClassEditor* CE,BaseClass* BC,int Options){
	//GlobalTextMouseOverCommand
	int		adress=0;
	char	s[256];
	bool	Error=false;
	Error = !(ggGetIndexedString(GlobalTextMouseOverCommand,1,s));
	if (!Error) adress = atoi(s);
	Error = Error || !(ggGetIndexedString(GlobalTextMouseOverCommand,0,s)); 

	if (Options==1) {
		strcpy(GlobalTextMouseOverCommand,"");
		MAIN_EDIT_PANEL.EP_SET_TEXT(" ");
		MAIN_EDIT_PANEL.EP_LOAD();
		return false;
	};
	if (Options==2) {
		if (!Error&&adress!=0) {
			if (strcmp(s,"BE_CFO")==0) {	// lvCCondForOper
				lvCCondForOper* pCFO = (lvCCondForOper*)(adress);
				MAIN_EDIT_PANEL.EP_SET_TEXT(pCFO->GetSourceCode());
			};
		};
		MAIN_EDIT_PANEL.EP_UPDATE();
		MAIN_EDIT_PANEL.EP_DRAW();
		return false;
	};
	if (Options==3) {
		return false;
	};
	if (Options==4) {
		return false;
	};
	return false;
};
// BE_EDIT_CLASS /////////////////////////////////////////////////////////
void	Add_Class_To_Main_Editor(DWORD _rce_, DWORD _DILOG_EDITOR_){
	AddStdEditor("BattleEditor",dynamic_cast<BaseClass*>(DriveMode()),"",_DILOG_EDITOR_,NULL,NULL,'B');

	AddStdEditor(	
		"ScriptEditor",
		dynamic_cast<BaseClass*>(&g_beSCRIPT_STORE),
		"",
		RCE_FULLSCREEN|RCE_EXITONESCAPE|RCE_INVISIBLE	
	);

	AddStdEditor(	"MainBEScript",
					dynamic_cast<BaseClass*>(&(BattleHandler()->MAIN_SCRIPT)),
					"",
					RCE_FULLSCREEN|RCE_EXITONESCAPE|RCE_INVISIBLE				);

	AddStdEditor(	"MainBEScriptEX",
					dynamic_cast<BaseClass*>(&(BattleHandler()->MAIN_SCRIPT)),
					"",
					RCE_BOTTOM|RCE_AUTOSAVE|RCE_EXITONESCAPE|RCE_EXITONENTER,
					PrepareToShow_BE_MAIN										);

	AddStdEditor("ScriptFilm",NULL,"",_rce_|RCE_INVISIBLE);
	AddStdEditor("BEvGroupEDIT",dynamic_cast<BaseClass*>(GroupsMap()),"",_rce_|RCE_INVISIBLE);
	AddStdEditor("BEvValueEDIT",dynamic_cast<BaseClass*>(vValuesMap()),"",_rce_|RCE_INVISIBLE);

	AddStdEditor("ScriptGraph",NULL,"",_rce_|RCE_INVISIBLE);
	
	AddStdEditor("BENodeEDIT",dynamic_cast<BaseClass*>(NodesMap()),"",_rce_|RCE_INVISIBLE);

	AddStdEditor("BEEdgeEDIT",NULL,"",_rce_|RCE_INVISIBLE);
	AddStdEditor("BEEdgeEDITEX",NULL,"",RCE_BOTTOM|RCE_AUTOSAVE|RCE_EXITONESCAPE|RCE_EXITONENTER|RCE_INVISIBLE,PrepareToShow_BE_MAIN);

	AddStdEditor("CameraPoss",NULL,"",_rce_|RCE_INVISIBLE);

	// TEST //
	AddStdEditor("MessageMgr",dynamic_cast<BaseClass*>(&gMessagesMap),"",_rce_|RCE_INVISIBLE);
	AddStdEditor("MissionSettings",dynamic_cast<BaseClass*>(&gMISS_SET),"",_rce_|RCE_INVISIBLE);

	// SAVE // 
	AddStdEditor("BattleHendler",dynamic_cast<BaseClass*>(BattleHandler()),"",_rce_|RCE_INVISIBLE);
};
//////////////////////////////////////////////////////////////////////////


















