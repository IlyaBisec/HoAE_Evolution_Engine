#include "stdheader.h"
#include "BE_HEADERS.h"
#include ".\cvi_HeroButtons.h"


MissSET	gMISS_SET;

extern	float fMapX;
extern	float fMapY;

extern	int AnimTime;

//////////////////////////////////////////////////////////////////////////

int GetCircleDif(int F1,int F2,int maxF/*=256*/){
	int tF1 = F1%maxF;
	int tF2 = F2%maxF;
	int dF  = abs(tF1-tF2);
	dF = min( dF, abs(maxF-dF) ) % 256;
	return dF;
};

void FI_2Pi(float& FI){			// round fi in [0,2*Pi)
	float Pi	= 3.1415f;
	float Pi2	= Pi*2.f;
	if (FI<0||FI>=Pi2) {
		FI += (-1)*Pi2*( (float)((int)(FI/Pi2)) );
		if (FI<0)	FI += Pi2;
	};
};

FILE*	f_aiLOG = NULL;
int __cdecl Msg( LPCSTR fileName, LPCSTR format, ... )
	{
	int	result = -1;
	if ( ( f_aiLOG = fopen(fileName,"a") ) != NULL ){
		size_t		length = strlen(format);
		LPSTR		_format = (LPSTR)malloc((length + 3)*sizeof(char));
		strcpy		(_format,format);
		strcat		(_format,"\n");

		va_list		marker;
		va_start	(marker,format);
					result = vfprintf(f_aiLOG,_format,marker);
		va_end		(marker);

		fclose(f_aiLOG);
	};

	return		(result);
}

void	ggg_WarnigMessage(const char* message){
	MessageBox(hwnd,message,"lua warning",MB_ICONWARNING|MB_OK);
};

extern	int	Shifter;

void LeaveAll(OneObject* OB);
bool CheckLeaveContainerAbility(OneObject* OB);

bool GetRomeHelp(cvi_RomeHelp* RH){
	if (RH==NULL)	return false;
	RH->MsgDialogs.Clear();
	vvMESSGES* pMess = dynamic_cast<vvMESSGES*>(vValuesMap()->GetVValueTypeID(_vvMESSGES_));
	if (pMess==NULL)	return false;

    CPhraseChunk* pPhrase=NULL;	// cur phrase
	cvi_MsgDialog* pMasDial=NULL;

	// Talks history
	int ChunkNUM = pMess->TALKS.GetAmount();
	for (int i=0; i<ChunkNUM; i++){
		pMasDial=new cvi_MsgDialog;
		if (pMasDial!=NULL) {
			pPhrase = dynamic_cast<CPhraseChunk*>(pMess->TALKS[i]);
			if (pPhrase!=NULL&&pPhrase->Visible){
				if (pPhrase!=NULL){
					if (pPhrase->TitleID.pchar()!=NULL) pMasDial->Title = GetTextByID(pPhrase->TitleID.pchar());
					int NPR = pPhrase->PhraseList.GetAmount();
					for (int p=0; p<NPR; p++){
						vvTEXT* pText = pPhrase->PhraseList[p]->Get();
						if (pText!=NULL) {
							cvi_Phrase* pNewPR = new cvi_Phrase;
							pNewPR->Message = reinterpret_cast<char*>(pText->Get());
							if (pText->SpeakerID.pchar()!=NULL){
								pNewPR->talkerName = GetTextByID(pText->SpeakerID.pchar());
							};
							pMasDial->Phrases.Add(pNewPR);
							pNewPR=NULL;
						};
					};
				};
				RH->MsgDialogs.Add(pMasDial);
				pMasDial=NULL;
			};
		};
	};

	// Hints History
	COneMissHint* pHint=NULL;
	int HintNUM = pMess->HINTS.GetAmount();
	for (int i=0; i<HintNUM; i++){
		pMasDial=new cvi_MsgDialog;
		if (pMasDial!=NULL) {
			pHint = dynamic_cast<COneMissHint*>(pMess->HINTS[i]);
			if (pHint!=NULL&&pHint->Visible&&pHint->TextID.pchar()!=NULL) {
				cvi_Phrase* pNewPR = new cvi_Phrase;
				pNewPR->Message = GetTextByID(pHint->TextID.pchar());
				pMasDial->Phrases.Add(pNewPR);
				pNewPR=NULL;
				RH->MsgHints.Add(pMasDial);
			}else{
				delete pMasDial;
			};
		};
		pMasDial=NULL;
	};

	// Tasks List
	CSingleMessage* pTask=NULL;
	int TaskNUM = pMess->TASKS.GetAmount();
	for (int i=0; i<TaskNUM; i++){
		pMasDial=new cvi_MsgDialog;
		if (pMasDial!=NULL) {
			pTask=pMess->TASKS[i];
			if (EngSettings.MIS_SET.DonotShowComleteQuest&&pTask->Visible==false&&pTask->Deleted==true){
				pTask->Visible=true;
			};
			if (pTask!=NULL&& pTask->Visible&&pTask->TextID.pchar()!=NULL&&pTask->TextID.pchar()!=NULL){
				cvi_Phrase* pNewPR = new cvi_Phrase;
				pNewPR->talkerName	= GetTextByID(pTask->TalkerID.pchar());
				pNewPR->Message		= GetTextByID(pTask->TextID.pchar());
				pNewPR->Deleted		= pTask->Deleted;
				pMasDial->Phrases.Add(pNewPR);
				pNewPR=NULL;
				RH->MsgObjectives.Add(pMasDial);
			}else{
				delete pMasDial;
			};
			if (EngSettings.MIS_SET.DonotShowComleteQuest&&pTask->Visible==true&&pTask->Deleted==true){
				pTask->Visible=false;
			};
		};
		pMasDial=NULL;
	};

	return false;
};


// lvCStorage //////////////////////////////////////////////////////////////
lvCStorage::lvCStorage(lvCStorage* pStorage){
	if (pStorage!=NULL){
		ClassName		= pStorage->ClassName.pchar();
		InfID			= pStorage->InfID;
		FileNameXML		= pStorage->FileNameXML.pchar();

		Name			= pStorage->Name.pchar();
		DESCR			= pStorage->DESCR.pchar();
	};
};

void	lvCStorage::vINIT (char* CLName/*=NULL*/, DWORD ID/*=0*/, char* FLName/*=NULL*/){
	ClassName	= ( (CLName==NULL) ? ("CStorage") : (CLName) );    
	InfID		= ( (ID==0) ? (_lvCStorage_) : (ID) );   
	FileNameXML	= ( (FLName==NULL) ? ("CStorage.xml") : (FLName) ); 

	Name		= "NoName";
	DESCR		= "NoDescription";
};

void	lvCStorage::vCLEAN(){
	ClassName.Clear();
	FileNameXML.Clear();

//	NAME.Clear();
	DESCR.Clear();
};

void	lvCStorage::vDeleteDynamicData(){
	return;
};

void	lvCStorage::vSetClassName		(char* name){
	if (name!=NULL) {
		ClassName = name;
	};
};

void	lvCStorage::vSetFileNameXML	(char* name){
	if (name!=NULL) {
		FileNameXML = name;
	};
};

void	lvCStorage::vSetClassID		(DWORD	_id){
	InfID = _id;
};

void	lvCStorage::vSaveXML(){
	xmlQuote	xml( ClassName.pchar() );
	this->Save(xml,this);
	xml.WriteToFile( FileNameXML.pchar() );
};

void	lvCStorage::vLoadXML(){
	vDeleteDynamicData();
	
	xmlQuote	xml( ClassName.pchar() );
	xml.ReadFromFile( FileNameXML.pchar() );
	ErrorPager	Err;
	this->Load(xml,this,&Err);
};

void	lvCStorage::vSetObjectName(char* name){
	if (name!=NULL) {
		Name = name;
	};
};

void	lvCStorage::vSetDescription(char* description){
	if (description!=NULL) {
		DESCR = description;
	};
};

// CNodesMAP_ST /////////////////////////////////////////////////////////////
void		lvCNodesMAP_ST::vINIT (char* CLName/* =NULL */,DWORD ID/* =0 */,char* FLName/* =NULL */){
	ClassName	= ( (CLName==NULL) ? ("lvCNodesMAP_ST") : (CLName) );    
	InfID		= ( (ID==0) ? (_lvCNodesMAP_ST_) : (ID) );   
	FileNameXML	= ( (FLName==NULL) ? ("lvCNodesMAP_ST.xml") : (FLName) ); 

	Name		= "NoName";
	DESCR		= "NoDescription";
};

void		lvCNodesMAP_ST::vCLEAN(){

};

void		lvCNodesMAP_ST::vDeleteDynamicData(){
	NodesArray.Clear();
};

be_CNODE*	lvCNodesMAP_ST::vAddNode(int x,int y,char* name/* =NULL */,char* descr/* =NULL */){
	int N = NodesArray.GetAmount();
	be_CNODE* pND = new be_CNODE(name,x,y);
    pND->GetObjectGlobalID();
	NodesArray.Add(pND);
	return	pND;
};

bool		lvCNodesMAP_ST::vDelNodeID(DWORD _id){
	int N = NodesArray.GetAmount();
	while (N--) {
		if (NodesArray[N]->GetObjectGlobalID()==_id) {
			NodesArray.Del(N,1);
			N=0;
			return true;
		};
	};
	return	false;
};

bool		lvCNodesMAP_ST::vDelNodePT(be_CNODE* pND){
	int N = NodesArray.GetAmount();
	while (N--) {
		if (NodesArray[N]==pND) {
			NodesArray.Del(N,1);
			N=0;
			return true;
		};
	};
	return	false;
};

bool		lvCNodesMAP_ST::vDelNode(int _x,int _y,DWORD sqID/* =0xFFFFFFFF */){
	be_CNODE* pND = vGetNode(_x,_y);
	return	vDelNodePT(pND);
};

void		lvCNodesMAP_ST::vDraw(DWORD mode/* =1 */,DWORD _id/* =0xFFffFFff */){
	int N = NodesArray.GetAmount();
	while (N--) {
		if (_id==0xFFffFFff||(NodesArray[N]->GetObjectGlobalID()==_id)) {
			NodesArray[N]->vDraw();
		};
	};
};

be_CNODE*	lvCNodesMAP_ST::vGetNode(DWORD _id){
	be_CNODE* pRET = NULL;
	int N = NodesArray.GetAmount();
	while (N--) {
		if (NodesArray[N]->GetObjectGlobalID()/*id*/==_id) {
			pRET = NodesArray[N];
		};
	};
	return	pRET;
};
be_CNODE*	lvCNodesMAP_ST::vGetNode(const char* name){
	be_CNODE* pRET = NULL;
	if (name!=NULL){
		int N = NodesArray.GetAmount();
		while (N--&&pRET==NULL) {
			if (strcmp(NodesArray[N]->Name.pchar(),name)==0) {
				pRET = NodesArray[N];
			};
		};
	};
	return	pRET;
};
be_CNODE*	lvCNodesMAP_ST::vGetNode(int _x,int _y,DWORD sqID/* =0xFFFFFFFF */){
	const	float MIN_DIST	= 300.f;

	be_CNODE* pRET = NULL;
	int N = NodesArray.GetAmount();
	if (N>0){
		pRET			= NodesArray[0];
		float minDist	= NodesArray[0]->vGetNodeDist(_x,_y);
		float dist		= 0.f; 
		while (N--) {
			dist = NodesArray[N]->vGetNodeDist(_x,_y);
			if (minDist>dist){
				minDist = dist;
				pRET = NodesArray[N];
			};
		};
		if (minDist>MIN_DIST) {
			pRET = NULL;
		};
	};
	return	pRET;
};

be_CNODE* __getNodeByName(const char* nodeName){
	return	NodesMap()->vGetNode(nodeName);
};
be_CNODE* __getNodeByID(const int nodeID){
	return	NodesMap()->vGetNode(nodeID);
};
// lvCNodesOnMap ////////////////////////////////////////////////////////
/*
DWORD lvCNodesOnMap::GetValue(const char* ID){
	be_CNODE* pNODE = NodesMap()->vGetNode(ID);
	if (pNODE!=NULL)	return pNODE->id;	
	return 0;
};
char* lvCNodesOnMap::GetValue(DWORD ID){
	be_CNODE* pNODE = NodesMap()->vGetNode(ID);
	if (pNODE!=NULL)	return pNODE->vGetName();	
	return NULL;
};
int   lvCNodesOnMap::GetAmount(){
	return NodesMap()->NodesArray.GetAmount();
};
char* lvCNodesOnMap::GetIndexedString(int idx){
	if (idx<0||GetAmount()<=idx)	return NULL;
	return NodesMap()->NodesArray[idx]->vGetName();
};
DWORD lvCNodesOnMap::GetIndexedValue(int idx){
	if (idx<0||GetAmount()<=idx)	return 0xFFFF;
	return NodesMap()->NodesArray[idx]->id;
};
*/
//////////////////////////////////////////////////////////////////////////
void CopyReIm(byte NI);
void MakeReformation(byte NI,word BrigadeID,byte FormType);
void AddUnitToSelected(byte NI,OneObject* OB);
void RotUnit(OneObject* OB,char Dir,byte OrdType);
DLLEXPORT	void SelChangeNation(byte SrcNat,byte DstNat);
DLLEXPORT	void ClearSelection(byte Nat);
void ClearBMASK();
extern bool BuildMode;
be_CGROUP*	__getGrpByName(const char* sss){
	if (sss==NULL) return NULL;
	return (be_CGROUP*)GroupsMap()->GetGroupNM(sss);
};
be_CGROUP*	__getGrpByID(int sss){
	return (be_CGROUP*)GroupsMap()->GetGroupID(sss);
};
// lvCGroupsMAP_ST ///////////////////////////////////////////////////////
bool	gg_VGrpReSize=true;
lvCGroupsMAP_ST::lvCGroupsMAP_ST(){
};
lvCGroupsMAP_ST::~lvCGroupsMAP_ST()	{
	vDeleteDynamicData();
	vCLEAN();
};
void		lvCGroupsMAP_ST::vINIT (char* CLName/*=NULL*/, DWORD ID/*=0*/, char* FLName/*=NULL*/){
	ClassName	= ( (CLName==NULL) ? ("lvCGroupsMAP_ST") : (CLName) );    
	InfID		= ( (ID==0) ? (_lvCGroupsMAP_ST_) : (ID) );   
	FileNameXML	= ( (FLName==NULL) ? ("lvCGroupsMAP_ST.xml") : (FLName) ); 

	vCLEAN();
};

void		lvCGroupsMAP_ST::vCLEAN(){
	Name		= "NoName";
	DESCR		= "NoDescription";
};

void		lvCGroupsMAP_ST::vDeleteDynamicData(){
	GROUPS.Clear();
};

void		lvCGroupsMAP_ST::AddGroup(char* name){
	bool	newGRP = (GetGroupNM(name)==NULL);
	if (newGRP) {
		be_CGROUP* pGRP = new be_CGROUP();
		pGRP->SetName(name);
		GROUPS.Add(pGRP);
		BattleShema()->AddPlan_vGRP(pGRP);
		pGRP = NULL;
	};
};
void		lvCGroupsMAP_ST::AddGroupSmart(char* name){
	bool	newGRP = (GetGroupNM(name)==NULL);
	if (newGRP) {
		lvCGroupSmart* pGRP = new lvCGroupSmart();
		pGRP->SetName(name);
		GROUPS.Add(pGRP);
		BattleShema()->AddPlan_vGRP(pGRP);
		pGRP = NULL;
	};
};
void		lvCGroupsMAP_ST::DelGroupNM(char* name){
	int N = GROUPS.GetAmount();
	while (N--) {
		if (strcmp(name,GROUPS[N]->GetName())==0) {
			N=0;
			BattleShema()->DelPlan_vGRP(GROUPS[N]);
			GROUPS.Del(N,1);
		};
	};
};

void		lvCGroupsMAP_ST::DelGroupID(DWORD id){
	int N = GROUPS.GetAmount();
	while (N--) {
		if (GROUPS[N]->GetObjectGlobalID() == id) {
			BattleShema()->DelPlan_vGRP(GROUPS[N]);
			GROUPS.Del(N,1);
			N=0;
		};
	};
};

be_CGROUP*	lvCGroupsMAP_ST::GetGroupNM(const char* name){
	be_CGROUP* pGRP = NULL;
	if (name!=NULL){
		int N = GROUPS.GetAmount();
		while (pGRP==NULL&&N--) {
			char* s=GROUPS[N]->GetName();
			if ( s&&(strcmp(name,s)==0) ) {
				pGRP = GROUPS[N];
			};
		};
	};
	return pGRP;
};

be_CGROUP*	lvCGroupsMAP_ST::GetGroupID(DWORD id){
	be_CGROUP* pGRP = NULL;
	int N = GROUPS.GetAmount();
	while (pGRP==NULL&&N--) {
		if (GROUPS[N]->GetObjectGlobalID()/*id*/ == id) {
			pGRP = GROUPS[N];
		};
	};
	return pGRP;
};

int			lvCGroupsMAP_ST::GetGroupAmount(){ 
	return GROUPS.GetAmount();
};

bool		lvCGroupsMAP_ST::GetBrigList( LinearArray<int,_int>& _list, int Nat ){
    int GRN = GetGroupAmount();
	_list.Clear();
	be_CGROUP*  pGRP	= NULL;
	OneObject* pOB	= NULL;
	
	while (GRN--) {
		pGRP = GROUPS[GRN];
		if (pGRP!=NULL&&pGRP->GetNation()==Nat){
			int OBN = pGRP->GetTotalAmount();
			while (OBN--) {
				pOB = pGRP->GetOneObj_lID(OBN);
				if (pOB!=NULL&&pOB->BrigadeID!=0xFFFF) {
					bool	add = true;
					for (int l=0; add&&(l<_list.GetAmount()); l++){
						if (_list[l]==pOB->BrigadeID)	add=false;
					};
					if (add) {
						_list.Add(pOB->BrigadeID);
					};
				};
			};
		};
	};
	if (_list.GetAmount()>0)	return true;
	return false;
};

void		lvCGroupsMAP_ST::GetStructNameList( LinearArray<int,_int>& _list ){
	int N = GetGroupAmount();
	LinearArray<int,_int>	TempList;
	while (N--) {
		GROUPS[N]->GetStructNameList(TempList);
		int NL=TempList.GetAmount();
		for (int i=0; i<NL; i++){
			_list.Add( TempList[i] );
		};
		TempList.Clear();
	};
	RemoveDublicateInList( _list );
};
void		lvCGroupsMAP_ST::DrawGroup(){
	int N = GROUPS.GetAmount();
	while (N--) {
		GROUPS[N]->vDraw();
	};
};

void		lvCGroupsMAP_ST::SetVisible(bool value){
	int N = GROUPS.GetAmount();
	while (N--) {
		GROUPS[N]->visible = value;
	};
};

void		lvCGroupsMAP_ST::BeforeSave(){
	
};

void		lvCGroupsMAP_ST::AfterSave(){
	
};

void		lvCGroupsMAP_ST::BeforeLoad(){
	GROUPS.Clear();
};

void		lvCGroupsMAP_ST::AfterLoad(){
	
};

void		lvCGroupsMAP_ST::ReSize(){
	int N=GetGroupAmount();
	while (N--){
		GROUPS[N]->RemoveDeadUnits();
	};
};
bool		lvCGroupsMAP_ST::__CheckMove(OneObject* pOB){
	if (GRP_donotMove.GetAmount()==0)	return true;
	if (pOB!=NULL && (!pOB->Sdoxlo||pOB->Hidden)){
		int GrpN = GRP_donotMove.GetAmount();
		bool	result=true;
		while ( (GrpN--)&&(result) ) {
			result = ( GRP_donotMove[GrpN]->_CheckGID(pOB->Index)!=0xFFFF );
		};
		return ( (result) ? (false) : (true) );
	};
	return false;
};
bool		lvCGroupsMAP_ST::__AddDonMoveGrp(be_CGROUP* pGrp){
	if (pGrp==NULL||pGrp->Name.pchar()==NULL) return false;
	int GrpN = GRP_donotMove.GetAmount();
	bool	needADD=true;
	while (GrpN--&&needADD) {
		if ( strcmp(GRP_donotMove[GrpN]->Name.pchar(),pGrp->Name.pchar())==0 )	needADD=false;
	};
	if (needADD){
		GRP_donotMove.Add(pGrp);
		return true;
	};
	return false;
};
bool		lvCGroupsMAP_ST::__RemDonMoveGrp(be_CGROUP* pGrp){
	if (pGrp==NULL||pGrp->Name.pchar()==NULL) return false;
	int GrpN = GRP_donotMove.GetAmount();
	bool	deleted=false;
	while (GrpN--&&!deleted) {
		if ( strcmp(GRP_donotMove[GrpN]->Name.pchar(),pGrp->Name.pchar())==0 ){
			deleted=true;
			GRP_donotMove[GrpN]=NULL;
			GRP_donotMove.DelElement(GrpN);
		};
	};
	return deleted;
};
//////////////////////////////////////////////////////////////////////////
// lvCGroupsOnMap ///////////////////////////////////////////////////////
/*
DWORD lvCGroupsOnMap::GetValue(const char* ID){
	be_CGROUP* pGRP = GroupsMap()->GetGroupNM(ID);
	if (pGRP!=NULL)	return pGRP->id;	
	return 0;
};

char* lvCGroupsOnMap::GetValue(DWORD ID){
	be_CGROUP* pGRP = GroupsMap()->GetGroupID(ID);
	if (pGRP!=NULL)	return pGRP->GetGroupName();	
	return NULL;
};

int   lvCGroupsOnMap::GetAmount(){
	return GroupsMap()->GROUPS.GetAmount();
};

char* lvCGroupsOnMap::GetIndexedString(int idx){
	if (idx<0||GetAmount()<=idx)	return NULL;
	return GroupsMap()->GROUPS[idx]->GetGroupName();
};

DWORD lvCGroupsOnMap::GetIndexedValue (int idx){
	if (idx<0||GetAmount()<=idx)	return 0xFFFF;
	return GroupsMap()->GROUPS[idx]->id;
};

*/
//======================================================================//
//===========================   vVALUES   ==============================//
//======================================================================//
// Base Value ////////////////////////////////////////////////////////////
_str		g_vvElementView;

char*			vvBASE::GetName() const{
	g_vvElementView  = "";
	g_vvElementView += "(BS)";
	g_vvElementView += (char*)Name.cpchar();
	return g_vvElementView.pchar();
};

const	char*	vvBASE::GetThisElementView(const char* LocalName){ 
	return GetName();
};

void*			vvBASE::Get(){
	return NULL;
};

void			vvBASE::Set(void* value){ 
	return;	
};

void			vvBASE::SetName(const char* _Name){ 
	if (_Name!=NULL&&CheckName(_Name)) Name=_Name; 
};

void			vvBASE::SetID(){ 
};

bool			vvBASE::CheckName(const char* name){
	bool newName = true;
	int N = vValuesMap()->V_VALUES.GetAmount();
	while (newName&&N--) {
		if (strcmp(vValuesMap()->V_VALUES[N]->Name.pchar(),name)==0) {
			newName = false;
		};
	};
	return newName;
};

void			vvBASE::Draw(){
};
// Triger ////////////////////////////////////////////////////////////////
/*
char*			be_CBOOL::GetName(){
	g_vvElementView  = "";
	g_vvElementView += "(TG)";
	g_vvElementView += Name.pchar();	
	return	g_vvElementView.pchar();
};

const	char*	be_CBOOL::GetThisElementView(const char* LocalName){
	GetName();
	if (Value)	g_vvElementView += "[true]";
	else		g_vvElementView += "[false]";
	return		g_vvElementView.pchar();
};

void*			be_CBOOL::Get(){
	return	&Value;
};

void			be_CBOOL::Set(void* value){
	bool*	pGetVal = reinterpret_cast<bool*>(value);
	if (pGetVal!=NULL)	Value = *pGetVal;
};

bool			be_CBOOL::GetValue() const{
	return Value;
};
void			be_CBOOL::SetValue(const bool value){
	Value = value;
};
*/
// Word ///////////////////////////////////////////////////////////////
/*
char*			be_CDWORD::GetName(){
	g_vvElementView  = "";
	g_vvElementView += "(WD)";
	g_vvElementView += Name.pchar();	
	return	g_vvElementView.pchar();
};

const	char*	be_CDWORD::GetThisElementView(const char* LocalName){
	GetName();
	g_vvElementView += "[";
	g_vvElementView += Value;
	g_vvElementView += "]";
	return		g_vvElementView.pchar();
};

void*			be_CDWORD::Get(){
	return &Value;
};

void			be_CDWORD::Set(void* value){
	DWORD*	pGetVal = reinterpret_cast<DWORD*>(value);
	if (pGetVal!=NULL)	Value = *pGetVal;
};

DWORD			be_CDWORD::GetValue()	const{
	return Value;
};
void			be_CDWORD::SetValue(const DWORD value){
	Value = value;
};
*/
// Integer ///////////////////////////////////////////////////////////////
/*
char*			be_CINT::GetName(){
	g_vvElementView  = "";
	g_vvElementView += "(IN)";
	g_vvElementView += Name.pchar();	
	return	g_vvElementView.pchar();
};

const	char*	be_CINT::GetThisElementView(const char* LocalName){
	GetName();
	g_vvElementView += "[";
	g_vvElementView += Value;
	g_vvElementView += "]";
	return		g_vvElementView.pchar();
};

void*			be_CINT::Get(){
	return &Value;
};

void			be_CINT::Set(void* value){
	int*	pGetVal = reinterpret_cast<int*>(value);
	if (pGetVal!=NULL)	Value = *pGetVal;
};
int				be_CINT::GetValue()	const{
	return Value;
};
void			be_CINT::SetValue(const int value){
	Value = value;
};
*/
// Text //////////////////////////////////////////////////////////////////
char*			vvTEXT::GetName(){
	g_vvElementView  = "";
	g_vvElementView += "(TX)";
	g_vvElementView += Name.pchar();	
	return	g_vvElementView.pchar();
};

const	char*	vvTEXT::GetThisElementView(const char* LocalName){
	GetName();
    g_vvElementView += "[";
	g_vvElementView += TextID;
	g_vvElementView += "]";
	return		g_vvElementView.pchar();
};

void*			vvTEXT::Get(){
	char* pText = GetGlobalBuffer();
	pText[0]=0;
	if ( pText!=NULL && strcmp(TextID.pchar(),"")!=0){
		sprintf(pText,"%s",GetTextByID(TextID.pchar()));
	};
	return pText;
};

void			vvTEXT::Set(void* value){
	vvTEXT* pGetVal = reinterpret_cast<vvTEXT*>(value);
	if (pGetVal!=NULL) {
		TextID		= pGetVal->TextID.pchar();
		oggFile		= pGetVal->oggFile.pchar();
		SpeakerID	= pGetVal->SpeakerID.pchar(); 
	};
};
char*			vvTEXT::Get_TextID		()	const{
	return (char*)TextID.cpchar();
};
char*			vvTEXT::Get_oggFile		()	const{
	return (char*)oggFile.cpchar();
};
char*			vvTEXT::Get_SpeakerID	()	const{
	return (char*)SpeakerID.cpchar();
};
void			vvTEXT::Set_TextID		(const char* text){
	if (text)	TextID		=	text;
};
void			vvTEXT::Set_oggFile		(const char* text){
	if (text)	oggFile		=	text;
};
void			vvTEXT::Set_SpeakerID	(const char* text){
	if (text)	SpeakerID	=	text;
};
// Picture ///////////////////////////////////////////////////////////////
vvPICTURE::vvPICTURE(){
	InfID		= _vvPICTURE_;
	FileID		= 0xFFFF;
	SpriteID	= -1;
};
vvPICTURE::~vvPICTURE(){
};
void			vvPICTURE::SET_DEF_POS::EvaluateFunction(){
	vvPICTURE* pPR = get_parent<vvPICTURE>();
	if (pPR!=NULL) {
		pPR->dx=0;
		pPR->dy=0;
		pPR->lx=0;
		pPR->ly=0;
	};
};
char*			vvPICTURE::GetName(){
	g_vvElementView  = "";
	g_vvElementView += "(PC)";
	g_vvElementView += Name.pchar();	
	return	g_vvElementView.pchar();
};
const	char*	vvPICTURE::GetThisElementView(const char* LocalName){
	GetName();
	g_vvElementView += "[dx=";
	g_vvElementView += dx;
	g_vvElementView += ",dy=";
	g_vvElementView += dy;
	g_vvElementView += ",lx=";
	g_vvElementView += lx;
	g_vvElementView += ",ly=";
	g_vvElementView += ly;
	g_vvElementView += "]";
	return		g_vvElementView.pchar();
};
bool			vvPICTURE::GetAsStringForMessage(_str& text){
	if (FileID==0xFFFF||SpriteID==-1)	return false;
	//{G gpID sprID dx dy lx ly}
	text = "";
	text += "{G ";
	text += FileID;
	text += " ";
	text += SpriteID;
	if (dx!=0||dy!=0){
		text += " ";
		text += dx;
		text += " ";
		text += dy;
	};
	if (lx!=0||ly!=0){
		text += " ";
		text += lx;
		text += " ";
		text += ly;
	};
	text += "}";
	return true;
};
int				vvPICTURE::GetSpriteNUM() const{
	if (FileID!=0xFFFF) return	GPS.GPNFrames(FileID);
	return 0;
};
int				vvPICTURE::GetSpriteID() const{
	return SpriteID;
};
void			vvPICTURE::SetSpriteID(const int sid){
	if (0<=sid&&sid<GetSpriteNUM()){
		SpriteID=sid;
	};
};
// Point2D ///////////////////////////////////////////////////////////////
void			vvPOINT2D::Draw(){
	if (visible==true){
		float x0 = (float)(Value.x);
		float y0 = (float)(Value.y);

		Vector3D	p00(x0-10.f,y0-10.f,(float)GetHeight((int)x0-10,(int)y0-10));
		Vector3D	p01(x0+10.f,y0+10.f,(float)GetHeight((int)x0+10,(int)y0+10));
		Vector3D	p10(x0+10.f,y0-10.f,(float)GetHeight((int)x0+10,(int)y0-10));
		Vector3D	p11(x0-10.f,y0+10.f,(float)GetHeight((int)x0-10,(int)y0+10));

		GPS.DrawLine(p00,p01,0xFF0000ff);
		GPS.DrawLine(p10,p11,0xFF0000ff);
	};
};
bool			vvPOINT2D::CondState(){
	bool state=true;
	int N=Cond.GetAmount();
	while (state&&N--) {
		state = state && Cond[N]->GetValue(0);
	};
	return state;
};
char*			vvPOINT2D::GetName(){
	g_vvElementView  = "";
	g_vvElementView += "(P2D)";
	g_vvElementView += Name.pchar();	
	return	g_vvElementView.pchar();
};

const	char*	vvPOINT2D::GetThisElementView(const char* LocalName){
	GetName();
	g_vvElementView += "[";
	g_vvElementView += Value.x;
	g_vvElementView += ",";
	g_vvElementView += Value.y;
	g_vvElementView += "]";
	return		g_vvElementView.pchar();
};

void*			vvPOINT2D::Get(){
	return &Value;
};

void			vvPOINT2D::Set(void* value){
	POINT2D* pGetVal = reinterpret_cast<POINT2D*>(value);
	if (pGetVal!=NULL){
		Value.x = pGetVal->x;
		Value.y = pGetVal->y;
	};
};

void			vvPOINT2D::Set(int x, int y){
	Value.x=x;
	Value.y=y;
};
void			vvPOINT2D::SetTR(int  x, int  y){
	if (Value.x!=x||Value.y!=y)	Value.newCoord=true;
	Value.x=x;
	Value.y=y;
};
void			vvPOINT2D::GetTR(int& x, int& y){
	Value.newCoord=false;
	x=Value.x;
	y=Value.y;
};
bool			vvPOINT2D::NewCoord(){
	return Value.newCoord;
}
int				vvPOINT2D::GetX()	const{
	return Value.x;
};
int				vvPOINT2D::GetY()	const{
	return Value.y;
};
void			vvPOINT2D::SetX(const int x){
	Value.x=x;
};
void			vvPOINT2D::SetY(const int y){
	Value.y=y;
};
// PointSet //////////////////////////////////////////////////////////////
char*			vvPOINT_SET::GetName(){
	g_vvElementView  = "";
	g_vvElementView += "(P_SET)";
	g_vvElementView += Name.pchar();	
	return	g_vvElementView.pchar();
};

const	char*	vvPOINT_SET::GetThisElementView(const char* LocalName){
	GetName();
	g_vvElementView += "[";
	g_vvElementView += Value.GetAmount();
	g_vvElementView += " P2D";
	g_vvElementView += "]";
	return		g_vvElementView.pchar();
};

void*			vvPOINT_SET::Get(){
	return &Value;
};

void			vvPOINT_SET::Set(void* value){
	ClassArray<vvPOINT2D>* pGetVal = reinterpret_cast<ClassArray<vvPOINT2D>*>(value);
	if (pGetVal!=NULL) {
		Value.Clear();
		vvPOINT2D* pP = NULL;
		int N = pGetVal->GetAmount();
		while (N--) {
			pP = new vvPOINT2D();
			if (pP!=NULL) {
				pP->Set( (*pGetVal)[N] );
				Value.Add(pP);
				pP = NULL;
			};
		};
	};
};

bool			vvPOINT_SET::TestPoint(int x,int y){
	bool	retVal = false;
	int N = Value.GetAmount();
	POINT2D* pPoint = NULL;
	while (!retVal&&N--) {
		pPoint = reinterpret_cast<POINT2D*>(Value[N]->Get());
		if (pPoint!=NULL){
			if (pPoint->x==x&&pPoint->y==y){
				retVal=true;
			};
			pPoint=NULL;
		};
	};
	return	retVal;
};

bool			vvPOINT_SET::TestPoint(vvPOINT2D* pPoint){
	if (pPoint!=NULL) {
		POINT2D* pP2D = reinterpret_cast<POINT2D*>(pPoint->Get());
		if (pP2D!=NULL) {
			return TestPoint(pP2D->x,pP2D->y);
		};
	};
	return	false;
};

void			vvPOINT_SET::CleanARR(){
	Value.Clear();
};

void			vvPOINT_SET::AddPoint(int x,int y){
	if (TestPoint(x,y)==false) {
		vvPOINT2D* pP = new vvPOINT2D();
		if (pP!=NULL){
			pP->Set(x,y);
			Value.Add(pP);
			pP=NULL;
		}
	};
};

int				vvPOINT_SET::GetNUM()	const{
	return Value.GetAmount();
};
vvPOINT2D*		vvPOINT_SET::Get_vvPOINT2D(const int id){
	if (0<=id&&id<GetNUM()) {
		return Value[id];
	};
	return NULL;
};
void	vvPOINT_SET::SetAgreeGrp::EvaluateFunction(){
	vvPOINT_SET* pPS = get_parent<vvPOINT_SET>();
	if (pPS!=NULL){
		be_CGROUP*	 pGR = GroupsMap()->GetGroupID(GrpID);
		if (pGR!=NULL) {
			int N = pGR->GetTotalAmount();
			OneObject* pOB = NULL;
			while (N--) {
				pOB = pGR->GetOneObj_lID(N);
				if (pOB!=NULL)	pPS->AddPoint(pOB->RealX>>4,pOB->RealY>>4);
			};
		};
	};
};
// vvVector3D ///////////////////////////////////////////////////////////
char*			vvVector3D::GetName(){
	g_vvElementView = "";
	g_vvElementView += "(Vec3D)";
	g_vvElementView += Name.pchar();
	return	g_vvElementView.pchar();
};

const	char*	vvVector3D::GetThisElementView(const char* LocalName){
	GetName();
	g_vvElementView += "[";
	g_vvElementView += x;
	g_vvElementView += ",";
	g_vvElementView += y;
	g_vvElementView += ",";
	g_vvElementView += z;
	g_vvElementView += "]";
	return g_vvElementView.pchar();
};

void*			vvVector3D::Get(){
	Vector3D	Vec(x,y,z);
	return &Vec;
};

void			vvVector3D::Set(void* value){
	Vector3D*	pVec3D = reinterpret_cast<Vector3D*>(value);
	if (pVec3D!=NULL) {
		x = pVec3D->x;
		y = pVec3D->y;
		z = pVec3D->z;
	};
};

void			vvVector3D::SetPos::EvaluateFunction(){
	vvVector3D* pvv3D = get_parent<vvVector3D>();
	if (pvv3D!=NULL) {
        assert( false );
        Vector3D pos = ICam->GetLookAt();
		pvv3D->x	= pos.x;
		pvv3D->y	= pos.y;
		pvv3D->z	= pos.z;
		pvv3D->mX	= mapx;
		pvv3D->mY	= mapy;
	};
};

void			vvVector3D::SetDir::EvaluateFunction(){
	vvVector3D* pvv3D = get_parent<vvVector3D>();
	if (pvv3D!=NULL) {
		Vector3D pos = ICam->GetDir();
		pvv3D->x = pos.x;
		pvv3D->y = pos.y;
		pvv3D->z = pos.z;
	};
};

void			vvVector3D::CPosToLookAt::EvaluateFunction(){
	vvVector3D* pPOS = pos.Get();
	vvVector3D* pDIR = dir.Get();

	if (pPOS==NULL||pDIR==NULL)	return;

	vvVector3D* pvv3D = get_parent<vvVector3D>();
	if (pvv3D!=NULL) {
		Vector3D curDIR = ICam->GetDir();
		Vector3D curPOS = ICam->GetDir();
		
		Vector3D tempPOS(pPOS->x,pPOS->y,pPOS->z);
		Vector3D tempDIR(pDIR->x,pDIR->y,pDIR->z);

		ICam->SetPosition(tempPOS);
		ICam->SetDirection(tempDIR);
        assert( false );

		Vector3D newPOS = ICam->GetLookAt();

		ICam->SetPosition(curPOS);
		ICam->SetDirection(curDIR);

		pvv3D->x	= newPOS.x;
		pvv3D->y	= newPOS.y;
		pvv3D->z	= newPOS.z;
	};
};
// vvDIALOG ///////////////////////////////////////////////////////////
char*			vvDIALOG::GetName(){
	g_vvElementView = "";
	g_vvElementView += "(Dialog)";
	g_vvElementView += Name.pchar();
	return g_vvElementView.pchar();
};

const	char*	vvDIALOG::GetThisElementView(const char* LocalName){
	return GetName();
};

void*			vvDIALOG::Get(){
	return &Value;
};

void			vvDIALOG::Set(void* value){
	DialogsSystem* pGetVal = reinterpret_cast<DialogsSystem*>(value);
	if (pGetVal!=NULL) {
		value = pGetVal;
	};
};
// vvFuzzyRule ///////////////////////////////////////////////////////////
vvFuzzyRule::vvFuzzyRule(){
	Init();
};
vvFuzzyRule::vvFuzzyRule(const char* name, float min0, float min100, float max100, float max0){
	Init();
	Setup(name, min0, min100, max100, max0);
};
vvFuzzyRule::~vvFuzzyRule(){
	
};
void			vvFuzzyRule::Setup(const char* name, float min0, float min100, float max100, float max0){
	Init();
	m_min0   = min0;
	m_min100 = min100;
	m_max0   = max0;
	m_max100 = max100;
	Name	 = name;
};
float			vvFuzzyRule::IsTrueToWhatDegree(float datapoint) const {
	if (datapoint < m_min0 || datapoint > m_max0)		return(0.0f);
	if (datapoint >= m_min100 && datapoint <= m_max100)	return(1.0f);
	if (datapoint >= m_min0 && datapoint <= m_min100) {
		float widthofgreyarea = (float)fabs(m_min0 - m_min100);
		float relativedatapoint = datapoint-m_min0;
		return((float)fabs(relativedatapoint / widthofgreyarea));
	};
	if (datapoint >= m_max100 && datapoint <= m_max0) {
		float widthofgreyarea = (float)fabs(m_max0 - m_max100);
		float relativedatapoint = datapoint-m_max0;
		return((float)fabs(relativedatapoint / widthofgreyarea));
	};
	assert(0);
	return(0);
};
char*			vvFuzzyRule::GetName(void) const {
	return (char*)Name.cpchar();
};
void			vvFuzzyRule::Init(void){
	InfID = _vvFuzzyRule_;
	m_min100 = m_max100 = m_min0 = m_max0 = 0.0f; 
	Name = "NoName";
};
float			__FuzzyAND(const vvFuzzyRule* rule1, float data1,const vvFuzzyRule* rule2, float data2){
    float true1 = 0.0f;
	if (rule1) true1 = rule1->IsTrueToWhatDegree(data1);
	float true2 = 0.0f;
	if (rule2) true1 = rule2->IsTrueToWhatDegree(data2);
	return(min(true1, true2));
};
// STORAGE for VVelue ////////////////////////////////////////////////////
		vvMAP_ST::vvMAP_ST(){
};

		vvMAP_ST::~vvMAP_ST(){
	vDeleteDynamicData();
	vCLEAN();
};

bool	vvMAP_ST::AskParentForUsingExpansionClass(char* MemberName,char* ClassName){
	if ( strcmp(MemberName,"V_NEW_VALUES")			==0 ){
		if ( strcmp(ClassName,"be_CBOOL")			==0 ) return true;
		if ( strcmp(ClassName,"be_CDWORD")			==0 ) return true;
		if ( strcmp(ClassName,"be_CINT")			==0 ) return true;
		if ( strcmp(ClassName,"be_CUnitType")		==0 ) return true;
		if ( strcmp(ClassName,"be_CFurmationType")	==0 ) return true;
		if ( strcmp(ClassName,"be_CFLOAT")			==0 ) return true;
		if ( strcmp(ClassName,"be_CSTR")			==0 ) return true;
		if ( strcmp(ClassName,"be_CARRAY")			==0 ) return true;
		return false;
	};
	return true;
};

void	vvMAP_ST::vINIT(char* CLName/*=NULL*/, DWORD ID/*=0*/, char* FLName/*=NULL*/){
	ClassName	= ( (CLName==NULL) ? ("vvMAP_ST") : (CLName) );    
	InfID		= ( (ID==0) ? (_vvMAP_ST_) : (ID) );   
	FileNameXML	= ( (FLName==NULL) ? ("vvMAP_ST.xml") : (FLName) ); 

	vCLEAN();
};

void	vvMAP_ST::vCLEAN(){
	Name		= "All Values";
	DESCR		= "Values used in BE";
};

void	vvMAP_ST::vDeleteDynamicData(){
	V_VALUES.Clear();
};

vvBASE*	vvMAP_ST::GetVValueID(DWORD _id){
	vvBASE* retVal = NULL;
	int N = V_VALUES.GetAmount();
	while (retVal==NULL&&N--) {
		if (V_VALUES[N]->GetObjectGlobalID()==_id) {
			retVal = V_VALUES[N];
		};
	};
	return	retVal;
};

vvBASE*	vvMAP_ST::GetVValueNM(const char* name){
	vvBASE* retVal = NULL;
	int N = V_VALUES.GetAmount();
	while (retVal==NULL&&N--) {
		if (strcmp(V_VALUES[N]->Name.pchar(),name)==0) {
			retVal = V_VALUES[N];
		};
	};
	return	retVal;
};
vvBASE*	vvMAP_ST::GetVValue(DWORD TYPE_id, DWORD _id){
	vvBASE* pValue = NULL;
	int N = V_VALUES.GetAmount();
	while (pValue==NULL&&N--) {
		if (V_VALUES[N]->InfID==TYPE_id && V_VALUES[N]->GetObjectGlobalID()==_id) {
			pValue = V_VALUES[N];
		};
	};
	return pValue;
};
vvBASE*	vvMAP_ST::GetNearestValue(float x,float y){
	vvBASE* pRet=NULL;
	for (int i=0; (i<V_VALUES.GetAmount())&&(pRet==NULL); i++){
		if (Is_vvBASE(V_VALUES[i])) {
			if (V_VALUES[i]->IsInShape(x,y)) {
				pRet = V_VALUES[i];
			};
		};
	};
	return pRet;
};
vvBASE*	vvMAP_ST::GetNearestValue(int   x,int   y){
	return GetNearestValue((float)x,(float)y);
};
vvBASE* vvMAP_ST::GetVValueTypeID(DWORD _tid){
	vvBASE* retVal = NULL;
	int N = V_VALUES.GetAmount();
	while (retVal==NULL&&N--) {
		if (V_VALUES[N]->InfID==_tid) {
			retVal = V_VALUES[N];
		};
	};
	return	retVal;
};
void	vvMAP_ST::SetViewType(DWORD _type){
	int N = V_VALUES.GetAmount();
	while (N--) {
		V_VALUES[N]->viewType = _type;
	};
};

void	vvMAP_ST::AddFirstSeparator(char* _Name,ClassArray<vvBASE>* arrX){
	lvCSubSection* pSS = new lvCSubSection();
	pSS->Descr = _Name;
	arrX->Add(NULL);
	(*arrX)[0]=(vvBASE*)pSS;
	pSS=NULL;
};

void	vvMAP_ST::AddSepareteValues(ClassArray<vvBASE>* arrX){
	if (arrX->GetAmount()>1){
		for (int i=0; i<arrX->GetAmount(); i++){
			if (i==0){
				V_VALUES.Add(NULL);
				V_VALUES[V_VALUES.GetAmount()-1]=(vvBASE*)(*arrX)[0];
			}else{
				V_VALUES.Add((*arrX)[i]);
			};
				(*arrX)[i]=NULL;
		};
	};
	arrX->Clear();
};

void	vvMAP_ST::Sort(){
	// Create arrays for all types
	ClassArray<be_CBOOL>	arrTRIGER;
	ClassArray<be_CDWORD>		arrWORD;
	ClassArray<be_CINT>	arrINTEGER;
	ClassArray<vvTEXT>		arrTEXT;
	ClassArray<vvPOINT2D>	arrPOINT2D;
	ClassArray<vvPOINT_SET>	arrPOINT_SET;
	ClassArray<vvVector3D>	arrVector3D;
	ClassArray<vvDIALOG>	arrDIALOG;
	ClassArray<vvMESSGES>	arrMESSGES;

	AddFirstSeparator(	"TRIGER"		,(ClassArray<vvBASE>*)(&arrTRIGER)		);
	AddFirstSeparator(	"WORD"			,(ClassArray<vvBASE>*)(&arrWORD)		);
	AddFirstSeparator(	"INTEGER"		,(ClassArray<vvBASE>*)(&arrINTEGER)		);
	AddFirstSeparator(	"TEXT"			,(ClassArray<vvBASE>*)(&arrTEXT)		);
	AddFirstSeparator(	"POINT2D"		,(ClassArray<vvBASE>*)(&arrPOINT2D)		);
	AddFirstSeparator(	"POINT_SET"		,(ClassArray<vvBASE>*)(&arrPOINT_SET)	);
	AddFirstSeparator(	"Vector3D"		,(ClassArray<vvBASE>*)(&arrVector3D)	);
	AddFirstSeparator(	"DIALOG"		,(ClassArray<vvBASE>*)(&arrDIALOG)		);
	AddFirstSeparator(	"MESSGES"		,(ClassArray<vvBASE>*)(&arrMESSGES)		);
	    
	int N = V_VALUES.GetAmount();
	while (N--) {
		switch(V_VALUES[N]->InfID) {
		case _be_CBOOL_:
			arrTRIGER.Add	(	(be_CBOOL*)		(V_VALUES[N])	);
			break;
		case _be_CDWORD_:
			arrWORD.Add		(	(be_CDWORD*)		(V_VALUES[N])	);
			break;
		case _be_CINT_:
			arrINTEGER.Add	(	(be_CINT*)	(V_VALUES[N])	);
			break;
		case _vvTEXT_:
			arrTEXT.Add		(	(vvTEXT*)		(V_VALUES[N])	);
			break;
		case _vvPOINT2D_:
			arrPOINT2D.Add	(	(vvPOINT2D*)	(V_VALUES[N])	);
			break;
		case _vvPOINT_SET_:
			arrPOINT_SET.Add(	(vvPOINT_SET*)	(V_VALUES[N])	);
			break;
		case _vvVector3D_:
			arrVector3D.Add	(	(vvVector3D*)	(V_VALUES[N])	);
			break;
		case _vvDIALOG_:
			arrDIALOG.Add	(	(vvDIALOG*)		(V_VALUES[N])	);
			break;
		case _vvMESSGES_:
			arrMESSGES.Add	(	(vvMESSGES*)	(V_VALUES[N])	);
			break;
//		default:
//			V_VALUES.DelElement(N);
		};
		V_VALUES[N] = NULL;
	};
	V_VALUES.Clear();

	AddSepareteValues(	(ClassArray<vvBASE>*)(&arrTRIGER)		);
	AddSepareteValues(	(ClassArray<vvBASE>*)(&arrWORD)			);
	AddSepareteValues(	(ClassArray<vvBASE>*)(&arrINTEGER)		);
	AddSepareteValues(	(ClassArray<vvBASE>*)(&arrTEXT)			);
	AddSepareteValues(	(ClassArray<vvBASE>*)(&arrPOINT2D)		);
	AddSepareteValues(	(ClassArray<vvBASE>*)(&arrPOINT_SET)	);
	AddSepareteValues(	(ClassArray<vvBASE>*)(&arrVector3D)		);
	AddSepareteValues(	(ClassArray<vvBASE>*)(&arrDIALOG)		);
	AddSepareteValues(	(ClassArray<vvBASE>*)(&arrMESSGES)		);
};

void	vvMAP_ST::DeleteAllSeparators(){
	int N = V_VALUES.GetAmount();
	while (N--) {
		if ( Is_vvBASE(V_VALUES[N]) == false ){
			V_VALUES.DelElement(N);
		};
	};
};

void	vvMAP_ST::SORT_BY_TYPE::EvaluateFunction(){
	vvMAP_ST* pMAP = get_parent<vvMAP_ST>();
	if (pMAP==NULL)	return;
	//pMAP->Sort();
};

bool	vvMAP_ST::Is_vvBASE(vvBASE* pVV){
	if (pVV==NULL)	return false;
	lvCSubSection* pSS = (lvCSubSection*)(pVV);
	if (pSS!=NULL&&pSS->InfID==_lvCSubSection_)	return false;
	return true;
};
void	vvMAP_ST::Draw(){
	for (int i=0; i<V_VALUES.GetAmount(); i++){
		vvBASE* pVV = V_VALUES[i];
		if (Is_vvBASE(pVV)) {
			pVV->Draw();
		};
	};
};

void	vvMAP_ST::DrawOnMiniMap(int x,int y,int Lx,int Ly){
	for (int i=0; i<V_VALUES.GetAmount(); i++){
		vvBASE* pVV = V_VALUES[i];
		if (Is_vvBASE(pVV)) {
			pVV->DrawOnMiniMap(x,y,Lx,Ly);
		};
	};
}
void	vvMAP_ST::VIEW_OBJS(DWORD classID,bool state){
	int N=V_VALUES.GetAmount();
	while (N--) {
		if ( V_VALUES[N]->InfID == classID ) V_VALUES[N]->visible=state;
	};
};
bool	vvMAP_ST::VIEW_OBJS(DWORD classID){
	int N=V_VALUES.GetAmount();
	while (N--) {
		if ( V_VALUES[N]->InfID == classID ) return	V_VALUES[N]->visible;
	};
	return false;
};
bool	vvMAP_ST::OnMouseHandling(int mx,int my,bool& LeftPressed,bool& RightPressed,int MapCoordX,int MapCoordY,bool OverMiniMap){
	bool intercept=false;
	int N=V_VALUES.GetAmount();
	while (!intercept&&N--) {
		if ( Is_vvBASE(V_VALUES[N]) )
			intercept=V_VALUES[N]->OnMouseHandling(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap);
	};
	return intercept;
};
// ENUMERATOR FOR VALUES /////////////////////////////////////////////////
// vvTEXT
DWORD	fn_BE_vvTEXT_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"vvTEXT");
	if (retN!=0xFFFFFFFF) return retN;

	return ADD_BE_OBJECT_TO_ENUM	(
										in															,
										out															,
										PrevCallHash												,
										(ClassArray<ReferableBaseClass>*)(&(vValuesMap()->V_VALUES)),
										"vvTEXT"
									);
};
// vvPOINT2D
DWORD	fn_BE_vvPOINT2D_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"vvPOINT2D");
	if (retN!=0xFFFFFFFF) return retN;

	return ADD_BE_OBJECT_TO_ENUM	(
										in															,
										out															,
										PrevCallHash												,
										(ClassArray<ReferableBaseClass>*)(&(vValuesMap()->V_VALUES)),
										"vvPOINT2D"
									);	
};
// vvPOINT_SET
DWORD	fn_BE_vvPOINT_SET_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"vvPOINT_SET");
	if (retN!=0xFFFFFFFF) return retN;

	return ADD_BE_OBJECT_TO_ENUM	(
										in															,
										out															,
										PrevCallHash												,
										(ClassArray<ReferableBaseClass>*)(&(vValuesMap()->V_VALUES)),
										"vvPOINT_SET"
									);	
};
// vvVector3D
DWORD	fn_BE_vvVector3D_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"vvVector3D");
	if (retN!=0xFFFFFFFF) return retN;

	return ADD_BE_OBJECT_TO_ENUM	(
										in															,
										out															,
										PrevCallHash												,
										(ClassArray<ReferableBaseClass>*)(&(vValuesMap()->V_VALUES)),
										"vvVector3D"
									);	
};
// vvDIALOG
DWORD	fn_BE_vvDIALOG_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"vvDIALOG");
	if (retN!=0xFFFFFFFF) return retN;

	return ADD_BE_OBJECT_TO_ENUM	(
										in															,
										out															,
										PrevCallHash												,
										(ClassArray<ReferableBaseClass>*)(&(vValuesMap()->V_VALUES)),
										"vvDIALOG"
									);	
};
// vvMissionLOG
DWORD	fn_BE_vvMissionLOG_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"vvMissionLOG");
	if (retN!=0xFFFFFFFF) return retN;

	return ADD_BE_OBJECT_TO_ENUM	(
										in															,
										out															,
										PrevCallHash												,
										(ClassArray<ReferableBaseClass>*)(&(vValuesMap()->V_VALUES)),
										"vvMissionLOG"
									);		
};
// lvCGraphObject
DWORD	fn_BE_lvCGraphObject_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"lvCGraphObject");
	if (retN!=0xFFFFFFFF) return retN;

	return ADD_BE_OBJECT_TO_ENUM	(
										in																	,
										out																	,
										PrevCallHash														,
										(ClassArray<ReferableBaseClass>*)(&(BattleHandler()->SCRIPT_GRAPH))	,
										"lvCGraphObject"
									);		
};
// lvCDeffFilmMenu
DWORD	fn_BE_lvCDeffFilmMenu_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"lvCDeffFilmMenu");
	if (retN!=0xFFFFFFFF) return retN;

	return ADD_BE_OBJECT_TO_ENUM	(
										in																	,
										out																	,
										PrevCallHash														,
										(ClassArray<ReferableBaseClass>*)(&(BattleHandler()->SCRIPT_GRAPH))	,
										"lvCDeffFilmMenu"
									);	
};
// lvCFilm
DWORD	fn_BE_lvCFilm_ON_MAP_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int retN = ADD_BE_PARAMS_TO_ENUM(in,out,PrevCallHash,"lvCFilm");
	if (retN!=0xFFFFFFFF) return retN;

	return ADD_BE_OBJECT_TO_ENUM	(
										in																	,
										out																	,
										PrevCallHash														,
										(ClassArray<ReferableBaseClass>*)(&(BattleHandler()->SCRIPT_FILMS))	,
										"lvCFilm"
									);	
};
//======================================================================//
//==========================   vMESSAGE   ==============================//
//======================================================================//
bool	CSingleMessagereCreate=true;
bool	CSingleMessage::NeedUpdatePointsCoord(){
	bool update=false;
	if (Use_Array){
		int N=Points.GetAmount();
		while (!update&&N--) {
			if (Points[N]->Get()!=NULL){
				update=Points[N]->Get()->NewCoord();
			};
		};
	};
	return update;
};
int		MessAlertFunc(void* param){
	int viev = *(reinterpret_cast<int*>(param));
	return viev;
};
bool	OnMissTaskOver(SimpleDialog* SD){
	if (SD!=NULL) {
		vvMESSGES* pMES=reinterpret_cast<vvMESSGES*>(SD->UserParam);
		if (pMES!=NULL) {
			if ( strcmp(SD->Name.pchar(),"Cansel")==0 )		pMES->Data_Visible(false);
			else{
				pMES->ShowTask(SD->ID);
			};
			return true;
		};
	};
	return false;
};
bool	OnMissHintClick(SimpleDialog* SD){
	if (SD!=NULL) {
		vvMESSGES* pMES=reinterpret_cast<vvMESSGES*>(SD->UserParam);
		if (pMES!=NULL) {
			pMES->HideHint(SD->ID);
			return true;
		};
	};
	return false;
};
vvMESSGES::vvMESSGES(){
	InfID=_vvMESSGES_;
	ActivateAlert="NoAlert";
	StaticAlert="NoAlert";
	MSD_Visible=true;
	Desk_Load();
	ActiveteTilda=true;
	SeparVisib=false;
};
char*			vvMESSGES::GetName(){
	g_vvElementView = "MESSAGES(";
	g_vvElementView += Name.pchar();
	g_vvElementView += ")";
	return g_vvElementView.pchar();
};

const	char*	vvMESSGES::GetThisElementView(const char* LocalName){
	return GetName();
};
void			vvMESSGES::Draw(){
	RemHintInList(gMISS_SET.ShowHint);
	Desk_Create();
	Desk_Update();
	Desk_Sort();
//	Delete_Enable(EngSettings.MIS_SET.DonotShowComleteQuest);
	AddHintInList(gMISS_SET.ShowHint);
	Desk_SetHeight();
	Desk_Draw();
	Draw_Separator();

	Data_Visible(false);

	ArrowOnMapVisible(MSD_Visible);
	ArrowDS.ProcessDialogs();
};
void			vvMESSGES::DrawOnMiniMap(int x,int y,int Lx,int Ly){
	//	Alert_Update();
	Alert_AddTASKS();
	AlertDS.ProcessDialogs();
};
bool			vvMESSGES::SetMessDS_Visbility(bool val){
	bool	oldState = MSD_Visible;
	MSD_Visible = val;
	return oldState;
};
bool			vvMESSGES::GetMessDS_Visbility(){
	return MSD_Visible;
};
bool			vvMESSGES::GetTaskState(const int Task_MessUID,bool& visible,bool& deleted){
	int N = TASKS.GetAmount();
	CSingleMessage* pSM=NULL;
	while (pSM==NULL&&N--) {
		if (TASKS[N]->MessUID==Task_MessUID) {
			pSM = TASKS[N];
		}
	};
	if (pSM!=NULL) {
		visible = pSM->Visible;
		deleted = pSM->Deleted;
	};
	return (pSM!=NULL);
};
void			vvMESSGES::Desk_Load(char* FilePath/*=NULL*/){
	if (FilePath==NULL) {
		xmlQuote	xmlData( "MissGoalSystem" ); 
		if (xmlData.ReadFromFile( "Dialogs\\MGS.DialogsSystem.xml" )){
			/*
			ErrorPager Err;
			ListDesk*	pMissionGoals = new ListDesk();
			pMissionGoals->Load( xmlData, pMissionGoals, &Err );
			MessDS.AddDialog(pMissionGoals);
			pMissionGoals = NULL;
			*/
			ErrorPager	Err;
			MessDS.Load( xmlData,&MessDS,&Err );

			Data_Visible(false);

			if (GetAnimARROW()!=NULL) GetAnimARROW()->Visible=false;
		};
	};
};
void			vvMESSGES::Desk_Create(){
	ListDesk*	pMissionGoals = Desk_GetList();
	if (pMissionGoals==NULL)	return;
	
	int NTASK = TASKS.GetAmount();
	int NMESS = pMissionGoals->DSS.GetAmount();

	int TN = NTASK;
    while (!CSingleMessagereCreate&&TN--) {
		if (pMissionGoals->GetElementByUID( TASKS[TN]->MessUID )==0) CSingleMessagereCreate=true;
	};

	if (NTASK!=NMESS || CSingleMessagereCreate) {
		pMissionGoals->DSS.Clear();
		AlertState.Clear();
		for (int i=0; i<NTASK; i++){
			if (TASKS[i]->TalkerID.pchar()!=NULL){
				TASKS[i]->MessUID = pMissionGoals->AddElement( GetTextByID(TASKS[i]->TalkerID.pchar()) );
				VitButton* pNewTB = dynamic_cast<VitButton*>(pMissionGoals->GetElementByUID(TASKS[i]->MessUID));
				if (pNewTB!=NULL) {
					pNewTB->Name = "Task";
				//	pNewTB->OnUserClick = OnMissTaskClick;
					pNewTB->OnMouseOver = OnMissTaskOver;
					pNewTB->UserParam = reinterpret_cast<int>( this );
				};
				if (TASKS[i]->Use_Array) {
					int par2 = AL_HIDE;
					for (int ii=0; ii<TASKS[i]->Points.GetAmount(); ii++){
						int secccc = TASKS[i]->MessUID*1000+ii;
						Para<int,int>* pPara = new Para<int,int>(secccc,par2);
						AlertState.Add(pPara);

						_str FullName; 
						FullName = TASKS[i]->Name.pchar();
						FullName += ii;
						g_AddAlert(	ActivateAlert.pchar(),
									TASKS[i]->Points[ii]->Get()->Value.x,
									TASKS[i]->Points[ii]->Get()->Value.y,
									MessAlertFunc,&(pPara->second_element),
									FullName.pchar()							);
						pPara=NULL;
					};
				}else if (TASKS[i]->x!=0&&TASKS[i]->y!=0) {
					int par2 = AL_HIDE;
					Para<int,int>* pPara = new Para<int,int>(TASKS[i]->MessUID,par2);
					AlertState.Add(pPara);
					g_AddAlert(ActivateAlert.pchar(),TASKS[i]->x,TASKS[i]->y,MessAlertFunc,&(pPara->second_element));
					pPara=NULL;
				};
			};
		};

		SimpleDialog* pCanselBT=Data_GetCanselBT();
		if (pCanselBT!=NULL){
		//	pCanselBT->OnUserClick = OnMissTaskClick;
			pCanselBT->UserParam = reinterpret_cast<int>( this );
		};

		Alert_Clear();
		Alert_AddTASKS();
	};
	CSingleMessagereCreate=false;
};
void			vvMESSGES::Desk_Update(){
	ListDesk*	pMissionGoals = Desk_GetList();
	if (pMissionGoals==NULL)	return;

	// for resizing ListDesc
	int MaxMessLength=0;
	int TotalHeight=0;

	VitButton* pTB=NULL;
	CSingleMessage* pTASK=NULL;
	for (int i=0; i<TASKS.GetAmount(); i++){
		pTASK = TASKS[i]; // pTASK
		if (pTASK->MessUID!=0) { 
			pTB = dynamic_cast<VitButton*>( pMissionGoals->GetElementByUID( pTASK->MessUID ) );
			if (pTB!=NULL) {
				pTB->Visible = pTASK->Visible;
				pTB->Enabled = !(pTASK->Deleted);
				if ( strcmp( GetTextByID(pTASK->TalkerID.pchar()), pTB->Message )!=0 ){
					pTB->SetMessage( GetTextByID(pTASK->TalkerID.pchar()) );
				};
				// Alert system
				if (pTASK->Use_Array || (pTASK->x!=0&&pTASK->y!=0)) {
		
					//////////////////////////////////////////////////////////////////////////
					if (pTASK->NeedUpdatePointsCoord()==true){
						// update position of alert on minimap
						for (int j=0; j<pTASK->Points.GetAmount(); j++){
							for (int ii=0; ii<pTASK->Points.GetAmount(); ii++){
								_str FullName;
								FullName = pTASK->Name.pchar();
								FullName += ii;
								int xcc,ycc;
								pTASK->Points[ii]->Get()->GetTR(xcc,ycc);
								g_SetNewAlertCoord(FullName.pchar(),xcc,ycc);
							};
						};
					};
					//////////////////////////////////////////////////////////////////////////

					for (int ia=0; ia<AlertState.GetAmount(); ia++){
						if (AlertState[ia]->first_element==pTASK->MessUID 
							|| 
							(pTASK->Use_Array==true && AlertState[ia]->first_element/1000==pTASK->MessUID) )
						{
							if (pTASK->Deleted==false) {
								if (pTASK->Use_Array==true){
									int RealID=pTASK->MessUID*1000;
									bool PointCond=true;
									int NA = pTASK->Points.GetAmount();
									for (int aa=0; aa<NA; aa++){
										if (AlertState[ia]->first_element==RealID) {
											vvPOINT2D* pPointCur = pTASK->Points[aa]->Get();
											for (int cc=0; cc<pPointCur->Cond.GetAmount(); cc++){
												PointCond = PointCond&&pPointCur->Cond[cc]->GetValue(0);
											};
											if (pTB->MouseOver&&PointCond)	AlertState[ia]->second_element = AL_VIEW;
											else							AlertState[ia]->second_element = AL_HIDE;
										};;
										RealID++;
									};
								}else{
									if (pTB->MouseOver)	AlertState[ia]->second_element = AL_VIEW;
									else				AlertState[ia]->second_element = AL_HIDE;
								};
							}else{
									AlertState[ia]->second_element = AL_DELETE;
							};

						};
					};

				};
				//if (pTB->Visible) {
				//	if (pTB->GetTextWidth()>MaxMessLength)	MaxMessLength=pTB->GetTextWidth();
				//	TotalHeight += pTB->GetHeight();
				//};
			};
		};
		pTASK=NULL;
	};
	//// update size of ListDesc by button size data
	//int left,top,right,bottom;
	//pMissionGoals->GetMargin(left,top,right,bottom);
	//int gMLX = EngSettings.MIS_SET.minTASK_Lx;
	//int gDMX = EngSettings.MIS_SET.TsskMarge_x;
	//int gDMY = EngSettings.MIS_SET.TsskMarge_y;
	//if (MaxMessLength<gMLX)	MaxMessLength=gMLX;
	//pMissionGoals->SetWidth(MaxMessLength+left+right+gDMX+gDMY);
	//pMissionGoals->SetHeight(TotalHeight+top+bottom);

	Data_Update_Position();
};
void			vvMESSGES::Desk_SetHeight(){
	ListDesk*	pMissionGoals = Desk_GetList();
	if (pMissionGoals==NULL)	return;
	// for resizing ListDesc
	int MaxMessLength=0;
	int TotalHeight=0;
	for (int i=0; i<pMissionGoals->DSS.GetAmount(); i++){
		VitButton* pTB= dynamic_cast<VitButton*>(pMissionGoals->DSS[i]);
		if (pTB!=NULL&&pTB->Visible){
			if (pTB->GetTextWidth()>MaxMessLength)	MaxMessLength=pTB->GetTextWidth();
			TotalHeight += pTB->GetHeight();
		};
	};
	int left,top,right,bottom;
	pMissionGoals->GetMargin(left,top,right,bottom);
	int gMLX = EngSettings.MIS_SET.minTASK_Lx;
	int gDMX = EngSettings.MIS_SET.TsskMarge_x;
	int gDMY = EngSettings.MIS_SET.TsskMarge_y;
	if (MaxMessLength<gMLX)	MaxMessLength=gMLX;
	pMissionGoals->SetWidth(MaxMessLength+left+right+gDMX+gDMY);
	pMissionGoals->SetHeight(TotalHeight+top+bottom);
};
void			vvMESSGES::Desk_Sort(){
	// sort by deleted
	ListDesk* pMessList = Desk_GetList();
	if (pMessList!=NULL) {
		int NMess = pMessList->DSS.GetAmount();
		int key = 0;
		SimpleDialog* pTSD=NULL;
        for (int i=1; i<NMess; i++ ){
			key = i;
			pTSD=NULL;
			while ( key>=1 && ( pMessList->DSS[key]->Enabled==true && pMessList->DSS[key-1]->Enabled==false ) ) {
				pTSD=pMessList->DSS[key-1];
				pMessList->DSS[key-1]=pMessList->DSS[key];
				pMessList->DSS[key]=pTSD;
				pTSD=NULL;
				key--;
			};
		};
	};
};
void			vvMESSGES::Desk_Draw(){
	if (MSD_Visible)	MessDS.ProcessDialogs();
};
ListDesk*		vvMESSGES::Desk_GetList(){
	ListDesk* pList=NULL;
	int NDSS = MessDS.DSS.GetAmount();
	if (NDSS>0){
		while (pList==NULL && NDSS--) {
			pList = dynamic_cast<ListDesk*>(MessDS.DSS[NDSS]);
			if (pList!=NULL && strcmp(pList->Name.pchar(),"TaskList")!=0) pList=NULL;
		};
	};
	return pList;
};
void			vvMESSGES::Delete_Enable(bool activate){
	if (activate==false)	return;
	ListDesk* pMessList = Desk_GetList();
	if (pMessList!=NULL) {
		int NMess = pMessList->DSS.GetAmount();
		while (NMess--) {
			if (pMessList->DSS[NMess]->Visible&&pMessList->DSS[NMess]->Enabled==false){
				pMessList->DSS.DelElement(NMess);
			};
		};
	};
};
DialogsDesk*	vvMESSGES::Data_GetObj(){
	DialogsDesk* pData=NULL;
	int NDSS = MessDS.DSS.GetAmount();
	if (NDSS>0){
		while (pData==NULL && NDSS--) {
			pData = dynamic_cast<DialogsDesk*>(MessDS.DSS[NDSS]);
			if (pData!=NULL && strcmp(pData->Name.pchar(),"TaskData")!=0) pData=NULL;
		};
	};
	return pData;
};
TextButton*		vvMESSGES::Data_GetTextBT(){
	TextButton* pBTN = NULL;
	DialogsDesk* pDataDesk = Data_GetObj();
	if (pDataDesk!=NULL) {
		//int N=pDataDesk->DSS.GetAmount();
		//DialogsDesk* pSubDataDesk = NULL;
		//while (pSubDataDesk==NULL&&N--) {
		//	pSubDataDesk = dynamic_cast<DialogsDesk*>(pDataDesk->DSS[N]);
		//	if ( pSubDataDesk!=NULL && strcmp(pSubDataDesk->Name.pchar(),"TaskData")!=0 ) pSubDataDesk=NULL;
		//};
		//if (pSubDataDesk!=NULL) {
		//	N=pSubDataDesk->DSS.GetAmount();
		//	while (pBTN==NULL&&N--) {
		//		pBTN = dynamic_cast<TextButton*>(pSubDataDesk->DSS[N]);
		//		if ( pBTN!=NULL && strcmp(pBTN->Name.pchar(),"Text")!=0 ) pBTN=NULL;
		//	};
		//};
		pBTN = dynamic_cast<TextButton*>(pDataDesk->Find("Text"));
	};
	return pBTN;
};
SimpleDialog*	vvMESSGES::Data_GetCanselBT(){
	SimpleDialog* pBTN = NULL;
	DialogsDesk* pDataDesk = Data_GetObj();
	if (pDataDesk!=NULL) {
		pBTN = pDataDesk->Find("Cansel");
	};
	return pBTN;	
};
void			vvMESSGES::Data_Visible(bool state){
	DialogsDesk* pTaskDesk = Data_GetObj();
	if (pTaskDesk!=NULL) {
		pTaskDesk->Visible = state;
	};
};
void			vvMESSGES::Data_Update_Position(){
	ListDesk*		pTaskList = Desk_GetList();
	DialogsDesk*	pTaskDesk = Data_GetObj();
	GPPicture*		pSEP = GetSeparator();
	TextButton*		pTB  = Data_GetTextBT();
	if (pTaskList!=NULL&&pTaskDesk!=NULL&&pSEP!=NULL&&pTB!=NULL) {
		int x0,y0;
		x0 = pTaskList->Getx() /*+ pTaskList->GetWidth() + 50*/;
		y0 = pTaskList->Gety() + pTaskList->GetHeight() + pSEP->GetHeight() + 8;
		pTaskDesk->Setx(x0);
		pTaskDesk->Sety(y0);
		pTaskDesk->SetWidth( pTaskList->GetWidth() );
		int gTMX = EngSettings.MIS_SET.TextMarge_x;
		int gTMY = EngSettings.MIS_SET.TextMarge_y;
		pTB->MaxWidth = pTaskDesk->GetWidth()-gTMX-gTMY;
		pTaskDesk->SetHeight( pTB->GetHeight()+gTMX+gTMY );
	};
};
void			vvMESSGES::Data_SetMessage(char* TextID){
	if (TextID==NULL)	return;
	TextButton* pTextBT = Data_GetTextBT();
	if (pTextBT!=NULL){
		int H=pTextBT->GetHeight();
		pTextBT->SetMessage( GetTextByID( TextID ) );
		H = pTextBT->GetHeight() - H;
		
		DialogsDesk* pDD=Data_GetObj();
		if (pDD!=NULL) {
			int tH=pDD->GetHeight();
			pDD->SetHeight(tH+H);
			SimpleDialog* pSD=pDD->Find("TaskData");
			if (pSD!=NULL) {
				int tH=pSD->GetHeight();
				pSD->SetHeight(tH+H);
			};
		};
	};
};
void			vvMESSGES::Data_SetMessage(int TaskID){
	int N=TASKS.GetAmount();
	_str text;
	while (N--) {
		if (TASKS[N]->MessUID==TaskID){
			text=TASKS[N]->TextID.pchar();
			N=0;
		};
	};
	
	N=HINTS.GetAmount();
	while (N--) {
		if (HINTS[N]->MessUID==TaskID){
			text=HINTS[N]->TextID.pchar();
			N=0;
		};
	};
	Data_SetMessage(text.pchar());
};
void			vvMESSGES::Alert_Clear(){
	AlertDS.DSS.Clear();
	ArrowDS.DSS.Clear();
};
void			vvMESSGES::AddArrowOnMiniMap(int _TID,int _x,int _y,bool _visible){
	GPPicture* pGPP = new GPPicture;
	
	pGPP->ID = TASKS[_TID]->MessUID;
	pGPP->FileID=TASKS[_TID]->FileID;
	pGPP->SetSpriteID(TASKS[_TID]->SpriteID);

	int minimapx = GetXOnMiniMap(_x,_y);
	int minimapy = GetYOnMiniMap(_x,_y);

//	pGPP->Setx(minimapx - pGPP->GetWidth() /2);
//	pGPP->Sety(minimapy - pGPP->GetHeight());

    pGPP->Setx(minimapx - pGPP->GetWidth()/2);
	pGPP->Sety(minimapy - pGPP->GetHeight()/2);

	pGPP->Visible=_visible;

	AlertDS.DSS.Add(pGPP);

	pGPP=NULL;
};
void			vvMESSGES::AddArrowOnMap(int _TID,int _x,int _y,bool _visible){
	static	int LastTimeUpdate	= GetTickCount();
	static	int LastSpriteID	= 0;
	static	int SpriteNUM		= 64;

	if (GetTickCount()-LastTimeUpdate>=(int)(1000.f/(float)SpriteNUM)){
		LastTimeUpdate=GetTickCount();
		LastSpriteID = (LastSpriteID+1)%SpriteNUM;
	};

	GPPicture* pBAM = new GPPicture;	// Big Arrow  on Map
	GPPicture* pSRC = GetAnimARROW();

	if (pSRC==NULL)	return;

	pBAM->ID = TASKS[_TID]->MessUID;
	pBAM->FileID = pSRC->FileID;
	pBAM->SetSpriteID(LastSpriteID);

	int mapposx = _x;
	int mapposy = _y;
	float	mX = (float)mapposx;
	float	mY = (float)mapposy;
	float	mZ = (float)GetHeight(mapposx,mapposy)+100.f;

	Vector3D	p(mX,mY,mZ);
	WorldToScreenSpace(p);
	mapposx=p.x;
	mapposy=p.y;

	pBAM->Setx(mapposx - pBAM->GetWidth()/2);
	pBAM->Sety(mapposy - pBAM->GetHeight());

	pBAM->Visible = _visible;

	ArrowDS.DSS.Add(pBAM);

	pBAM=NULL;
};
void			vvMESSGES::ArrowOnMapVisible(bool _vvv){
	if (_vvv==true)	return;
	int N = ArrowDS.DSS.GetAmount();
	GPPicture* pAR=NULL;
	while (N--) {
		pAR = dynamic_cast<GPPicture*>(ArrowDS.DSS[N]);
		if (pAR!=NULL) {
			pAR->Visible=_vvv;
		};
	};
};
void			vvMESSGES::Alert_AddTASKS(){
	Alert_Clear();
	for (int i=0; i<TASKS.GetAmount(); i++){
		if (TASKS[i]->Use_Array) {
			for (int ii=0; ii<TASKS[i]->Points.GetAmount(); ii++){
				/////////////////////////////////////////////////////////////////////
				bool vvv = TASKS[i]->Visible;
				if (TASKS[i]->Deleted==true)	vvv=false;
				if (TASKS[i]->Points[ii]->Get()->CondState()==false) vvv=false;

				int xxx=TASKS[i]->Points[ii]->Get()->Value.x;
				int yyy=TASKS[i]->Points[ii]->Get()->Value.y;

				AddArrowOnMiniMap(i,xxx,yyy,vvv);
				AddArrowOnMap(i,xxx,yyy,vvv);
				/////////////////////////////////////////////////////////////////////
			};
		}else if (TASKS[i]->x!=0&&TASKS[i]->y!=0) {
			/////////////////////////////////////////////////////////////////////
			bool vvv = TASKS[i]->Visible;
			if (TASKS[i]->Deleted==true)	vvv=false;

			int xxx=TASKS[i]->x;
			int yyy=TASKS[i]->y;

			AddArrowOnMiniMap(i,xxx,yyy,vvv);
			AddArrowOnMap(i,xxx,yyy,vvv);
			/////////////////////////////////////////////////////////////////////
		};
	};
};
void			vvMESSGES::Alert_Update(){
	//GPPicture* pGPP = NULL;
	//bool vis;
	//bool del;
	//for (int i=0; i<AlertDS.DSS.GetAmount(); i++){
	//	pGPP = dynamic_cast<GPPicture*>(AlertDS.DSS[i]);
	//	if (pGPP&&GetTaskState(pGPP->ID,vis,del)){
	//		pGPP->Visible=true;
	//		if (vis==false||del==false)	pGPP->Visible=false;
	//	};
	//};
};
void			vvMESSGES::ShowTask(int TaskID){
    Data_Update_Position();
	Data_SetMessage(TaskID);
	Data_Visible(true);
};
void			vvMESSGES::Separator_Visible(bool state){
	SeparVisib=state;
	GPPicture* pSEP = GetSeparator();
	if (SeparVisib==false&&pSEP!=NULL)	pSEP->Visible=false;
};
void			vvMESSGES::Draw_Separator(){
	DialogsDesk* pTaskDesk = Data_GetObj();
	if (pTaskDesk!=NULL) {
		Separator_Visible(pTaskDesk->Visible);
	};

	if (MSD_Visible==false)	return;
    GPPicture* pSEP = GetSeparator();
	if (pSEP==NULL||SeparVisib==false)	return;
	pSEP->Visible=true;

	ListDesk*	pMissionGoals = Desk_GetList();
	if (pMissionGoals!=NULL) {
		int x0 = pMissionGoals->Getx();
		int y0 = pMissionGoals->Gety();
		int H = pMissionGoals->GetHeight();
		int W = pMissionGoals->GetWidth();

		int Sx0 = x0+3;
		int Sy0 = y0+H-2+5;
		int SW	= W-3-3;
		int w	= pSEP->GetWidth();
		if (w==0)	return;
		
		pSEP->Sety(Sy0);
		for (int i=0; i<SW/w; i++){
			pSEP->Setx(Sx0+i*w);
			GPS.FlushBatches();	
			pSEP->_Draw();
		};
		pSEP->Setx(Sx0+SW-w);
		pSEP->_Draw();
	};
};
GPPicture*		vvMESSGES::GetSeparator(){
	GPPicture* pSEP = dynamic_cast<GPPicture*>(MessDS.Find("Separator"));;
	return pSEP;
};
GPPicture*		vvMESSGES::GetAnimARROW(){
	GPPicture* pARROW = dynamic_cast<GPPicture*>(MessDS.Find("Arrow"));;
	return pARROW;
};
void			vvMESSGES::AddHintInList(bool add){
	if (add==false)	return;
	ListDesk*	pMissionGoals = Desk_GetList();
	int HN = HINTS.GetAmount();
	for (int i=0; i<HN; i++){
		if (HINTS[i]->Visible&&(dynamic_cast<COneMissHint*>(HINTS[i]))->HasRead==false){
			HINTS[i]->MessUID = pMissionGoals->AddElement( GetTextByID("#MISS_HINT_VV") );
			VitButton* pNewTB = dynamic_cast<VitButton*>(pMissionGoals->GetElementByUID(HINTS[i]->MessUID));
			if (pNewTB!=NULL) {
				pNewTB->Name = "Hint";
				pNewTB->OnUserClick = OnMissHintClick;
				pNewTB->OnMouseOver = OnMissTaskOver;
				pNewTB->UserParam = reinterpret_cast<int>( this );
				pNewTB->Visible=true;
			};
		};
	};
};
void			vvMESSGES::RemHintInList(bool rem){
	if (rem==false)	return;

	ListDesk*	pMissionGoals = Desk_GetList();
	if (pMissionGoals==NULL)	return;

	for (int i=0; i<HINTS.GetAmount(); i++){
		int ID_DEL = HINTS[i]->MessUID;
		
        int NMess = pMissionGoals->DSS.GetAmount();
		while (NMess--&&ID_DEL!=-1) {
			if (pMissionGoals->DSS[NMess]->ID==ID_DEL){
				pMissionGoals->DSS.DelElement(NMess);
				ID_DEL=-1;
			};
		};
	};
};
void			vvMESSGES::HideHint(DWORD id){
	int N=HINTS.GetAmount();
	while (N--) {
		if (HINTS[N]->MessUID==id){
			(dynamic_cast<COneMissHint*>(HINTS[N]))->HasRead=true;
			N=0;
		};
	};
};
bool			vvMESSGES::OnMouseHandling(int mx,int my,bool& LeftPressed,bool& RightPressed,int MapCoordX,int MapCoordY,bool OverMiniMap){
	bool	intercept=false;
//	static	int temp=0;
	
	ListDesk*	pMissionGoals = Desk_GetList();
	if (pMissionGoals!=NULL) {
		if (pMissionGoals->MouseOver){
			pMissionGoals->Diffuse = 0xFFffffff;
		}else{
			DWORD gTAF = EngSettings.MIS_SET.NotActiveDiffuse;
			pMissionGoals->Diffuse = gTAF;
			Data_Visible(false);
		};
	};

//	if (false&&GetKeyState(VK_OEM_3)&0x8000&&temp>5) {
//		SetMessDS_Visbility(!GetMessDS_Visbility());
//		void	UnPress();
//		UnPress();
//		temp=0;
//		intercept=true;
//	};
//	temp++;

	return intercept;
};
vvMESSGES*	g_TASK_HINT_OBJ(){
	return dynamic_cast<vvMESSGES*>(vValuesMap()->GetVValueTypeID(_vvMESSGES_));
};
// vvMISSMGR /////////////////////////////////////////////////////////////
extern bool NOPAUSE;
bool	vbRESET_RCLIC(SimpleDialog* SD){
	return true;
};
bool	vbSTPS_LCLIC(SimpleDialog* SD){
	NOPAUSE = !NOPAUSE;
	vvMISSMGR* pOB = dynamic_cast<vvMISSMGR*>(vValuesMap()->GetVValue(_vvMISSMGR_,SD->UserParam));
	if (pOB!=NULL) {
		pOB->STPS_ANIM(false);
		pOB->STPS_PROC();
		return true;
	};
	return false;
};
bool	vbREST_LCLIC(SimpleDialog* SD){
	vvMISSMGR* pOB = dynamic_cast<vvMISSMGR*>(vValuesMap()->GetVValue(_vvMISSMGR_,SD->UserParam));
	if (pOB!=NULL) {
		pOB->REST_ANIM(false);
		pOB->REST_PROC();
		return true;
	};
	return false;
};
bool	vbNEXT_LCLIC(SimpleDialog* SD){
	vvMISSMGR* pOB = dynamic_cast<vvMISSMGR*>(vValuesMap()->GetVValue(_vvMISSMGR_,SD->UserParam));
	if (pOB!=NULL) {
		pOB->NEXT_ANIM(false);
		pOB->NEXT_PROC();
		return true;
	};
	return false;
};
vvMISSMGR::vvMISSMGR(){
	InfID		= _vvMISSMGR_;
	DS_FILE		= "Dialogs\\mission\\TutorialManager.DialogsSystem.xml";
	visible		= true;

	DS_X = 30;
	DS_Y = 40;

	DS_LOAD();
	DS_PREP();
	DS_SETPOS();

	markTIME();

	AnimDelay = 500;
};
vvMISSMGR::~vvMISSMGR(){
};
void			vvMISSMGR::Draw(){
	DS_DRAW();
};
bool			vvMISSMGR::DS_LOAD(){
	if (DS_FILE.pchar()!=NULL){
        xmlQuote	xmlData( "MissMgrSystem" );
		if (xmlData.ReadFromFile(DS_FILE.pchar())) {
			ErrorPager	Err;
			DS_MENU.reset_class(&DS_MENU);
			if (DS_MENU.Load( xmlData,&DS_MENU,&Err )) return true;
		};
	};
	return false;
};// load menu
bool			vvMISSMGR::DS_PREP(){
	bool res = STPS_PREP() && REST_PREP() && NEXT_PREP();
	return res;
};// prepare for work
bool			vvMISSMGR::DS_DRAW(){
	if (visible){
//		DS_SETPOS();
		ANIMATE();
		DS_MENU.ProcessDialogs();
	};
	return true;
};// draw DS object
void			vvMISSMGR::markTIME(){
	lastATime = (int)GetTickCount();
};
bool			vvMISSMGR::checkDALAY(int curTIME){
	return ( curTIME - lastATime >= AnimDelay );
};
void	g_setALPHA_(DWORD& oldCOLOR,DWORD alpha){
	DWORD oldALPHA = oldCOLOR & 0xFF000000;
	DWORD newAlpha = alpha & 0xFF000000;
	if (oldALPHA!=newAlpha) {
		oldCOLOR = ( oldCOLOR & 0x00FFFFFF ) + newAlpha;
	};
};
void			vvMISSMGR::ANIMATE_BTN(GP_TextButton* pBTN,bool curST,bool animate){
	if ( pBTN ) {
		if ( pBTN->MouseOver==false ) {
			if ( animate ) {
				if (curST) g_setALPHA_(pBTN->Diffuse,0xFF000000);
				else	   g_setALPHA_(pBTN->Diffuse,0xBB000000);
			}else{
				g_setALPHA_(pBTN->Diffuse,0xFF000000);
			};
		}else{
			g_setALPHA_(pBTN->Diffuse,0xFF000000);
		};
	};
};
void			vvMISSMGR::ANIMATE(){
	static bool curSTATE = true;
	int curTIME = GetTickCount();

	if ( checkDALAY(curTIME) ) {

		curSTATE = !curSTATE;
		markTIME();
	};
	
	GP_TextButton* pBTN = NULL;

	pBTN = DS_GetElement("StartPause");
	ANIMATE_BTN(pBTN,curSTATE,stps_ANIM);

	pBTN = DS_GetElement("Restart");
	ANIMATE_BTN(pBTN,curSTATE,rest_ANIM);

	pBTN = DS_GetElement("Next");
	ANIMATE_BTN(pBTN,curSTATE,next_ANIM);

};
bool			vvMISSMGR::DS_VISB(bool _st){
	bool old_st = visible;
	visible = _st;
	return old_st;
};// DS visibility
DialogsDesk*	vvMISSMGR::DS_GetDesck(){
	DialogsDesk* pDD=NULL;
	int NDSS = DS_MENU.DSS.GetAmount();
	if (NDSS>0){
		while (pDD==NULL && NDSS--) {
			pDD = dynamic_cast<DialogsDesk*>(DS_MENU.DSS[NDSS]);
			if (pDD!=NULL && strcmp(pDD->Name.pchar(),"Desk")!=0) pDD=NULL;
		};
	};
	return pDD;
};	
GP_TextButton*	vvMISSMGR::DS_GetElement(char* name){
	if (name==NULL)	return NULL;
	GP_TextButton* pOB = NULL;
	DialogsDesk* pDD = DS_GetDesck();
	if (pDD!=NULL) {
		pOB = dynamic_cast<GP_TextButton*>(pDD->Find(name));
	};
	return pOB;
};// Get element from DS by name
void			vvMISSMGR::DS_SETPOS(){
	DialogsDesk* pDD = DS_GetDesck();
    if (pDD!=NULL) {
		pDD->Setx(RealLx-pDD->GetWidth()-DS_X);
		pDD->Sety(DS_Y);
	};
};
bool			vvMISSMGR::STPS_PREP(){
	GP_TextButton* pOB = DS_GetElement("StartPause");
	if (pOB!=NULL) {
		pOB->UserParam = (int)GetObjectGlobalID();
		pOB->OnUserClick = vbSTPS_LCLIC;
		pOB->OnUserRightClick = vbRESET_RCLIC;
		return true;
	};
	return false;
};// Prepare Start/Pause
bool			vvMISSMGR::STPS_ENABLED(bool _st){
	bool	st = false;
	GP_TextButton* pOB = DS_GetElement("StartPause");
	if (pOB!=NULL) {
		st				= pOB->Enabled;
		pOB->Enabled	= _st;
	};
	return st;
};// Start/Pause active state
bool			vvMISSMGR::STPS_ANIM(bool _st){
	bool oldST = _st;
	stps_ANIM = _st;
	return oldST;
};
void			vvMISSMGR::STPS_PROC(){
	int N = STPS_SCRIPT.GetAmount();
	for (int i=0; i<N; i++){
		STPS_SCRIPT[i]->Process(0);
	};
};// process Start/Pause script
bool			vvMISSMGR::REST_PREP(){
	GP_TextButton* pOB = DS_GetElement("Restart");
	if (pOB!=NULL) {
		pOB->UserParam = (int)GetObjectGlobalID();
		pOB->OnUserClick = vbREST_LCLIC;
		pOB->OnUserRightClick = vbRESET_RCLIC;
		return true;
	};
	return false;
};// Prepare Restart
bool			vvMISSMGR::REST_ENABLED(bool _st){
	bool	st = false;
	GP_TextButton* pOB = DS_GetElement("Restart");
	if (pOB!=NULL) {
		st				= pOB->Enabled;
		pOB->Enabled	= _st;
	};
	return st;
};// Restart active state
bool			vvMISSMGR::REST_ANIM(bool _st){
	bool oldST = _st;
	rest_ANIM = _st;
	return oldST;
};
void			vvMISSMGR::REST_PROC(){
	int N = REST_SCRIPT.GetAmount();
	for (int i=0; i<N; i++){
		REST_SCRIPT[i]->Process(0);
	};
};// process Restart script
bool			vvMISSMGR::NEXT_PREP(){
	GP_TextButton* pOB = DS_GetElement("Next");
	if (pOB!=NULL) {
		pOB->UserParam = (int)GetObjectGlobalID();
		pOB->OnUserClick = vbNEXT_LCLIC;
		pOB->OnUserRightClick = vbRESET_RCLIC;
		return true;
	};
	return false;
};// Prepare Next
bool			vvMISSMGR::NEXT_ENABLED(bool _st){
	bool	st = false;
	GP_TextButton* pOB = DS_GetElement("Next");
	if (pOB!=NULL) {
		st				= pOB->Enabled;
		pOB->Enabled	= _st;
	};
	return st;
};// Next active state
bool			vvMISSMGR::NEXT_ANIM(bool _st){
	bool oldST = _st;
	next_ANIM = _st;
	return oldST;
};
void			vvMISSMGR::NEXT_PROC(){
	int N = NEXT_SCRIPT.GetAmount();
	for (int i=0; i<N; i++){
		NEXT_SCRIPT[i]->Process(0);
	};
};// process Next script
void			vvMISSMGR::MM_INIT::EvaluateFunction(){
	vvMISSMGR* pPR = get_parent<vvMISSMGR>();
	if (pPR!=NULL) {
		pPR->DS_LOAD();
		pPR->DS_PREP();
		pPR->DS_SETPOS();
		pPR->STPS_ENABLED(SP_EN);
		pPR->REST_ENABLED(RS_EN);
		pPR->NEXT_ENABLED(NX_EN);
	};
};
// vvTASKS_CII ///////////////////////////////////////////////////////////
char*	vvTEXT_EX::GetFinishedString(){ 
	if (TextID.pchar()!=NULL){
		if (TEXT.pchar()!=NULL) TEXT.Clear();
		
		//////////////////////////////////////////////////////////////////////////
		
		char	data[4096];
		char*	sPart = NULL;
		char	first[4096];
		char	second[4096];
		sprintf(data,"%s",GetTextByID(TextID.pchar()));
		int NP = paramList.GetAmount();
		int cP=0;
		if (NP==0){
			TEXT = data;
			return TEXT.pchar();
		};

		// Have some params for insert
		bool stop=false;
		int iteration=0;
		while (!stop) {
			iteration++;
			sprintf(first,"%s","");
			sprintf(second,"%s","");
			sPart = strstr(data,"%");
			if (sPart!=NULL) {
				int nc = (int)(sPart-data+1);
				strncpy(first,data,nc+2);
				first[nc+2]=0;
				if (data[nc+2]!=0)	sprintf(second,"%s",&(data[nc+2]));
				// set params
				if (cP<NP){
					int paramType = paramList[cP]->InfID;
					switch(paramType) {
					case _be_CINT_:	// insetr int param
						sprintf(data,first,((be_CINT*)(paramList[cP]))->Value);
						strcpy(first,data);
						break;
					case _vvTEXT_:		// insert string by TextID in it
						sprintf(data,first,GetTextByID(((vvTEXT*)(paramList[cP]))->TextID.pchar()));
						strcpy(first,data);
						break;
					case _vvPICTURE_:
						_str picture;
						if ( ((vvPICTURE*)(paramList[cP]))->GetAsStringForMessage(picture) ){
							sprintf(data,first,picture.pchar());
							strcpy(first,data);
						}else{
							sprintf(data,first,"NO PICTURE");
							strcpy(first,data);
						};
						break;
					};
					cP++;
				};
				// create new data string
				sprintf(data,"%s%s",first,second);
			};
			if (sPart==NULL||cP>=NP||iteration>=100) {
				stop=true;
			};
		};

		TEXT = data;

		//////////////////////////////////////////////////////////////////////////

	}else{
		TEXT = "";
	};
	return TEXT.pchar(); 
};// Get string with all applying params
bbObjInList::bbObjInList(){
	Name="";
	DS_ID=-1;
	UN_BMP_ID.Clear();
	GP_MMP_ID.Clear();
};
bbObjInList::~bbObjInList(){
	UN_BMP_ID.Clear();
	GP_MMP_ID.Clear();
};
void	bbObjInList::Anim_MMP(bool state){
	int N=GP_MMP_ID.GetAmount();
	while (N--) {
		if (GP_MMP_ID[N]!=NULL){
			GPS.FlushBatches();
			GP_MMP_ID[N]->Animate=state;
		};
	};
};
void	bbObjInList::Draw_MMP(bool stopANIM/*=false*/){ 
	int N=GP_MMP_ID.GetAmount();
	while (N--) {
		if (GP_MMP_ID[N]!=NULL){
			GPS.FlushBatches();
			GP_MMP_ID[N]->DRAW(stopANIM);
		};
	};
};
void	bbObjInList::PreDelete(){
	int N=UN_BMP_ID.GetAmount();
	OneObject* pOB=NULL;

	while (N--) {
		DelUnit(UN_BMP_ID[N].ID);
	};

	UN_BMP_ID.Clear();
	GP_MMP_ID.Clear();
};
void	bbObjInList::DelUnit(int UID){
	// erese unit - flag
	OneObject* pOB = Group[ UID ];
	if (pOB) {
		pOB->delay=6000;
		if(pOB->LockType==1)pOB->RealDir=32;
		if(pOB->BrigadeID!=0xFFFF){
			Brigade* BR=CITY[pOB->NNUM].Brigs+pOB->BrigadeID;
			BR->vLastEnemyID=0xFFFE;
		}
		pOB->Die();
		pOB = Group[ UID ];
		if (pOB) {			
			pOB->Sdoxlo=2500;
			if(pOB->NewBuilding){
				void EliminateBuilding(OneObject* OB);
				EliminateBuilding(pOB);
				Group[ UID ]=NULL;
			};
		};
	};
	////////////////////
};
void	bbObjInList::DelPoint(int PID){
	if (PID<0||PID>=GP_MMP_ID.GetAmount()||PID>=UN_BMP_ID.GetAmount()) return;
	if (GP_MMP_ID[PID]==NULL&&(UN_BMP_ID[PID].ID==0xFFFF)) return;
	DelUnit(UN_BMP_ID[PID].ID);
	UN_BMP_ID[PID].ID=0xFFFF;
	delete ( GP_MMP_ID[PID] );
	GP_MMP_ID[PID]=NULL;
};
void	bbObjInList::UpdatePosOnMiniMap(){
	int NMM=GP_MMP_ID.GetAmount();
	if (NMM<=0)	return;
	OneObject* pOB=NULL;
	lvCAnimateGP* pMMP=NULL;
	int h,w;
	for (int i=0; i<NMM; i++){
		pMMP=GP_MMP_ID[i];
		if (pMMP!=NULL){
			pOB=Group[ UN_BMP_ID[i].ID ];
			if ( pOB!=NULL ) {
				pMMP->getPictureSize(w,h);
				int mmX = GetXOnMiniMap((pOB->RealX)>>4,(pOB->RealY)>>4)-w/2;
				int mmY = GetYOnMiniMap((pOB->RealX)>>4,(pOB->RealY)>>4)-h/2;
				pMMP->SetPosXY( mmX, mmY );
			};
			pOB=NULL;
		};
		pMMP=NULL;
	};
};
bool	vbTASK_LCLIC(SimpleDialog* SD){
	if (SD!=NULL&&SD->UserParam!=0){
		vvTASKS_CII* pPR=(vvTASKS_CII*)(SD->UserParam);
		if (pPR!=NULL) {
			pPR->vv_TASK_LCLIC();
		};
	};
	return true;
};// Click on TASK in main menu
bool	vbHINT_LCLIC(SimpleDialog* SD){
	if (SD!=NULL&&SD->UserParam!=0){
		vvTASKS_CII* pPR=(vvTASKS_CII*)(SD->UserParam);
		if (pPR!=NULL) {
			pPR->vv_HINT_LCLIC();
		};
	};
	return true;
};// Click on HINT in main menu
bool	vbELSE_LCLIC(SimpleDialog* SD){
	if (SD!=NULL&&SD->UserParam!=0){
		vvTASKS_CII* pPR=(vvTASKS_CII*)(SD->UserParam);
		if (pPR!=NULL) {
			pPR->vv_ELSE_LCLIC();
		};
	};
	return true;
};// Click on ELSE in main menu
bool	vbTASK_DELET(SimpleDialog* SD){
	if (SD==NULL)	return false;
	vvTASKS_CII* pTHE_MENU = (vvTASKS_CII*)(SD->UserParam);
	if (pTHE_MENU!=NULL) {
		pTHE_MENU->delLT_TASK( atoi(SD->AllocPtr) );
	};
	return true;
};// Click on delete button in TASK list
bool	vbHINT_DELET(SimpleDialog* SD){
	if (SD==NULL)	return false;
	vvTASKS_CII* pTHE_MENU = (vvTASKS_CII*)(SD->UserParam);
	if (pTHE_MENU!=NULL) {
		pTHE_MENU->delLT_HINT( atoi(SD->AllocPtr) );
	};
	return true;
};// Click on delete button in HINT list
bool	vbELSE_DELET(SimpleDialog* SD){
	if (SD==NULL)	return false;
	vvTASKS_CII* pTHE_MENU = (vvTASKS_CII*)(SD->UserParam);
	if (pTHE_MENU!=NULL) {
		pTHE_MENU->delLT_ELSE( atoi(SD->AllocPtr) );
	};
	return true;
};// Click on delete button in ELSE list
bool	vbTASK_MOVER(SimpleDialog* SD){
	if (SD==NULL)	return false;
	vvTASKS_CII* pTHE_MENU = (vvTASKS_CII*)(SD->UserParam);
	if (pTHE_MENU!=NULL) {
		pTHE_MENU->TAKS_START_ANIM( atoi(SD->AllocPtr) );
	};
	return true;
};
void			vvTASKS_CII::Draw(){
//	SETIN_OBJECT();
	CHECK_POINT_COND();
	DS_DRAW();
};// Function from vvBASE for draw dialogs
void			vvTASKS_CII::DrawOnMiniMap(int x,int y,int Lx,int Ly){
	TASK_POINT_DRAW();
};
vvTASKS_CII::vvTASKS_CII(){
	InfID = _vvTASKS_CII_;
	ALREADY_SETIN=true;
	NeedRestore=true;
};
vvTASKS_CII::~vvTASKS_CII(){
};
void			vvTASKS_CII::SETIN_OBJECT(){
	if (ALREADY_SETIN==false){
		DS_LOAD();
		applyMM_pos();
		applyLM_pos();
		setMM_LMClick();
		ALREADY_SETIN=true;
	};
};
bool			vvTASKS_CII::DS_LOAD(){
	bool	a_ST = false;
	if (ds_FILE.pchar()!=NULL) {
		xmlQuote	xmlDATA( "TASK_HINT_ELSE" );
        if (xmlDATA.ReadFromFile(ds_FILE.pchar())) {
			ErrorPager Err;
			ds_MAIN.reset_class(&ds_MAIN);
			if (ds_MAIN.Load( xmlDATA, &ds_MAIN, &Err )) a_ST=true;
        };		
	};
	return a_ST;
};// Load DS from file
void			vvTASKS_CII::DS_DRAW(){
	if (visible){
		Restore();
		ApplyMM_state();
		ds_MAIN.ProcessDialogs();
	};
};// draw DS
DialogsDesk*	vvTASKS_CII::getMM_object(){
	DialogsDesk* pMM=NULL;
	int NDSS = ds_MAIN.DSS.GetAmount();
	if (NDSS>0){
		while (pMM==NULL && NDSS--) {
			pMM = dynamic_cast<DialogsDesk*>(ds_MAIN.DSS[NDSS]);
			if (pMM!=NULL && strcmp(pMM->Name.pchar(),"MAIN_MENU")!=0) pMM=NULL;
		};
	};
	return pMM;
};// Get DD object for main menu
GP_Button*		vvTASKS_CII::getMM_child(char* _name){
	GP_Button* pGPP = NULL;
	if (_name!=NULL) {
		DialogsDesk* pMM = getMM_object();
		if (pMM!=NULL) {
			pGPP = dynamic_cast<GP_Button*>(pMM->Find(_name));
		};
	};
	return pGPP;
};// Get GPP object from main menu (TASK,HINT,ELSE)

void			vvTASKS_CII::applyMM_pos(){
	DialogsDesk* pMM = getMM_object();
	if (pMM!=NULL) {
		pMM->Setx(mm_X);
		pMM->Sety(mm_Y);
	};
};// Set main menu position usin (mm_X,mm_Y)

void			vvTASKS_CII::UpdateMM_state(){
	// Set statest using fill
	// ...
	// ...
	// ...
};// update main menu states
void			vvTASKS_CII::ApplyMM_state(){
	// Filling policy
	if (LM_TASK_OBJ.GetAmount()==0) {
		task_sate=1;
		if (LM_OPEN_ID==1)	LM_OPEN_ID=0;
	};
	if (LM_HINT_OBJ.GetAmount()==0) {
		hint_sate=1;
		if (LM_OPEN_ID==2)	LM_OPEN_ID=0;
	};
	if (LM_ELSE_OBJ.GetAmount()==0) {
		else_sate=1;
		if (LM_OPEN_ID==3)	LM_OPEN_ID=0;
	};

	// LIST MENU
	ApplyLM_state();
	
	// LAST APLLY FOR VIEW
	int oldT = mm_last_anim_time;
	// TASK
	mm_last_anim_time = oldT;
	ApplyMM_state(getMM_child("TASK"),task_sate);
	int MM_oldT=mm_last_anim_time;
	// HINT
	mm_last_anim_time = oldT;
	ApplyMM_state(getMM_child("HINT"),hint_sate);
	int MM_oldH=mm_last_anim_time;
	// ELSE
	mm_last_anim_time = oldT;
	ApplyMM_state(getMM_child("ELSE"),else_sate);
	int MM_oldE=mm_last_anim_time;
	
	mm_last_anim_time = max( max(oldT,MM_oldT), max(MM_oldH,MM_oldE) );
};// apply current state for main menu
void			vvTASKS_CII::ApplyMM_state(GP_Button* pP,int _st){
	if (pP==NULL)	return;
	pP->Visible=true;
	switch(_st) {
	case 1:		// disable
		pP->Enabled=false;
		pP->Active=false;
		pP->Diffuse=anim_diffuse;
		break;
	case 2:		// enable
		pP->Enabled=true;
		pP->Active=false;
		pP->Diffuse=0xFFffffff;
		break;
	case 3:		// enable+animate
		pP->Enabled=true;
		if (GetTickCount()-mm_last_anim_time>=mm_anim_delay){
			if (pP->Diffuse==anim_diffuse)	pP->Diffuse=0xFFffffff;
			else							pP->Diffuse=anim_diffuse;
			mm_last_anim_time=GetTickCount();
		};
		break;
	case 4:		// opened
		pP->Enabled=true;
		pP->Active=true;
		pP->Diffuse=0xFFffffff;
		break;
	default:	// hide
		pP->Visible=false;
		pP->Enabled=false;
	};
};
void			vvTASKS_CII::setMM_LMClick(){
	GP_Button*	pGPB_TASK =	getMM_child("TASK");
	if (pGPB_TASK!=NULL)	setMM_LMClick(pGPB_TASK,this,vbTASK_LCLIC);
	GP_Button*	pGPB_HINT =	getMM_child("HINT");
	if (pGPB_HINT!=NULL)	setMM_LMClick(pGPB_HINT,this,vbHINT_LCLIC);
	GP_Button*	pGPB_ELSE =	getMM_child("ELSE");
	if (pGPB_ELSE!=NULL)	setMM_LMClick(pGPB_ELSE,this,vbELSE_LCLIC);
};
void			vvTASKS_CII::setMM_LMClick(GP_Button* pP,vvTASKS_CII* pPARAM,VCall* pFUNC){
	if (pP!=NULL&&pPARAM!=NULL&&pFUNC!=NULL) {
		pP->UserParam	=(int)pPARAM;
		pP->OnUserClick	=pFUNC;
	};
};
void			vvTASKS_CII::applyLM_pos(){
	setLM_TASK_POS();
	setLM_HINT_POS();
	setLM_ELSE_POS();
};// Set list menu position usin (dl_X,dl_Y)
void			vvTASKS_CII::setLM_ID(){
	int ID=0;
	int NT=TASK.GetAmount();
	for (int i=0; i<NT; i++){
		((vvTEXT_EX*)(TASK[i]))->id=ID;
		ID++;
	};
	int NH=HINT.GetAmount();
	for (int i=0; i<NH; i++){
		((vvTEXT_EX*)(HINT[i]))->id=ID;
		ID++;
	};
	int NE=ELSE.GetAmount();
	for (int i=0; i<NE; i++){
		((vvTEXT_EX*)(ELSE[i]))->id=ID;
		ID++;
	};
};// Set unique id for vvTEXT_EX elements
ListDesk*		vvTASKS_CII::getLM_child(char* _name){
    ListDesk* pLD=NULL;
	if (_name!=NULL) {
		int NDSS = ds_MAIN.DSS.GetAmount();
		if (NDSS>0){
			while (pLD==NULL && NDSS--) {
				pLD = dynamic_cast<ListDesk*>(ds_MAIN.DSS[NDSS]);
				if (pLD!=NULL && strcmp(pLD->Name.pchar(),_name)!=0) pLD=NULL;
			};
		};
	};
	return pLD;
};// Get GPP object from list menu (TASK,HINT,ELSE)
void			vvTASKS_CII::ApplyLM_state(){
	setLM_TASK_VIS(false);
	setLM_HINT_VIS(false);
	setLM_ELSE_VIS(false);
	switch(LM_OPEN_ID) {
	case 1:	// TASK LIST
		setLM_TASK_VIS(true);
		break;
	case 2:	// HINT LIST
		setLM_HINT_VIS(true);
		break;
	case 3:	// ELSE LIST
		setLM_ELSE_VIS(true);
		break;
	};
};// apply current state for list menu
ListDesk*		vvTASKS_CII::getLM_TASK(){
	return getLM_child("TASK_LIST");
};// Get ListDesck accoding to TASK
bool			vvTASKS_CII::setLM_TASK_VIS(bool _st){
	bool oldST=false;
	ListDesk* pLDT = getLM_TASK();
	if (pLDT!=NULL) {
		oldST=pLDT->Visible;
		pLDT->Visible=_st;
	};
	return oldST;
};// Set TASK LIST visibility
void			vvTASKS_CII::setLM_TASK_POS(){
	DialogsDesk* pDD = getMM_object();
	if (pDD!=NULL) {
		int DD_W = pDD->GetWidth();
		int DD_H = pDD->GetHeight();
		ListDesk* pLDT = getLM_TASK();
		if (pLDT!=NULL) {
			pLDT->Setx(mm_X+DD_W+dl_X);
			if (dl_Y!=-1) pLDT->Sety(dl_Y);
			else		  pLDT->Sety(mm_Y);
		};
	};
};// Set list menu for TASK position
void			vvTASKS_CII::TAKS_START_ANIM(int Task_ID){
	int mlID = Assign(Task_ID,LM_TASK_OBJ);
	if (mlID==-1)	return;
	LM_TASK_OBJ[mlID]->Anim_MMP(true);
};
void			vvTASKS_CII::TASK_POINT_DRAW(){
	int N=LM_TASK_OBJ.GetAmount();
	while (N--) {
		if (LM_TASK_OBJ[N]!=NULL){
			LM_TASK_OBJ[N]->UpdatePosOnMiniMap();
			LM_TASK_OBJ[N]->Draw_MMP(true);
		};
	};
};
bool			vvTASKS_CII::addLT_TASK(vvTEXT_EX* pT,char* name/*=NULL*/,int pos/*=0*/){
	if (pT==NULL)	return false;
	ListDesk* pLD = getLM_TASK();
	if (pLD!=NULL) {
		int newN = pLD->AddElement(pos,"");
		bbObjInList* pTL=new bbObjInList();
		pTL->DS_ID = newN;
		if (name!=NULL&&name[0]!=0)		pTL->Name=name;
		else if (pT->Name.pchar()!=NULL)	pTL->Name=pT->Name.pchar();
		else							pTL->Name="NONAME";
		//////////////////////////////////////////////////////////////////////////
		int xxx,yyy;
		int mxxx,myyy;
		int h,w;

		int np = pT->paramList_EX.GetAmount();
		vvPOINT2D* pPOS=NULL;
		for (int i=0; i<np; i++){
			pPOS = dynamic_cast<vvPOINT2D*>(pT->paramList_EX[i]);
			if (pPOS!=NULL&&pPOS->CondState()) {
				// Set point on bigMAP
				pPOS->GetTR(xxx,yyy);
				xxx=xxx<<4;
				yyy=yyy<<4;

				int CreateNewTerrMons2(byte NI,int x,int y,word Type);
				int ID=CreateNewTerrMons2(0,xxx,yyy,PointOnBigMap);
				OneObject* pOOB = Group[ID];
				if(ID!=-1&& pOOB!=NULL && (!pOOB->Sdoxlo||pOOB->Hidden)){
					OneUS* pUS = new OneUS();
					pUS->ID = pOOB->Index;
					pUS->SN = pOOB->Serial;
					pTL->UN_BMP_ID.Add(*pUS);
				};

				// Set point on miniMAP
				mxxx = GetXOnMiniMap(xxx>>4,yyy>>4);
				myyy = GetYOnMiniMap(xxx>>4,yyy>>4);
				lvCAnimateGP* pMMP = new lvCAnimateGP();
				*pMMP = PointOnMiniMap;
				h=0;
				w=0;
				pMMP->getPictureSize(w,h);
				pMMP->SetPosXY(mxxx-w/2,myyy-h/2);
				pMMP->SetVisible(true);
				pTL->GP_MMP_ID.Add(pMMP);
			};
		};
		//////////////////////////////////////////////////////////////////////////
		
		DialogsDesk* pTaskEL = (DialogsDesk*)(pLD->Find(newN));
		if (pTaskEL!=NULL) {
			TextButton*	 pTxtBTN = (TextButton*)(pTaskEL->Find("Mess"));
			GPPicture*	 pDEL_BTN = (GPPicture*)(pTaskEL->Find("Delete"));
			if (pDEL_BTN!=NULL&&pTxtBTN!=NULL) {
				pTxtBTN->SetMessage( pT->GetFinishedString() );

				pDEL_BTN->Visible=false;
				// Container Class
				pDEL_BTN->UserParam = (int)(this);
				pTxtBTN->UserParam = (int)(this);
				// Task index in list
				char sID[32];
				sprintf(sID,"%d",newN);
				pDEL_BTN->AllocPtr = (char *)malloc( strlen(sID)+1 );
				if (pDEL_BTN->AllocPtr!=NULL) {
					sprintf(pDEL_BTN->AllocPtr,"%s",sID);
				};
				pTxtBTN->AllocPtr = (char *)malloc( strlen(sID)+1 );
				if (pTxtBTN->AllocPtr!=NULL) {
					sprintf(pTxtBTN->AllocPtr,"%s",sID);
				};
				// CallBack Function
				pDEL_BTN->OnUserClick=vbTASK_DELET;
				pTxtBTN->OnMouseOver=vbTASK_MOVER;
			};
		};

		LM_TASK_OBJ.Add(pTL);
	};
	if (task_sate!=4) task_sate=3;
	return true;
};// Add element from TASK in list menu for TASK
bool			vvTASKS_CII::addLT_TASK(const char* TName,int pos/*=0*/,bool Dublicate/*=true*/){
	if (TName==NULL)	return false;
	vvTEXT_EX*	pTASK = dynamic_cast<vvTEXT_EX*>(getByNAME(TName,TASK));
	if (pTASK==NULL)	return false;
	if (Dublicate==false)	delLT_TASK(pTASK->Name.pchar());
	return addLT_TASK(pTASK,NULL,pos);
};
void			vvTASKS_CII::delLT_TASK(int Task_ID){
	int mlID = Assign(Task_ID,LM_TASK_OBJ);
	if (mlID==-1)	return;
	ListDesk* pLD = getLM_TASK();
	if (pLD!=NULL) {
		if (pLD->DelElementByUID(Task_ID)){
			LM_TASK_OBJ[mlID]->PreDelete();
			LM_TASK_OBJ.DelElement(mlID);
		};
	};
};// Delete task from menu list by it id in LM_TAKS
void			vvTASKS_CII::delLT_TASK(const char* Task_Name){
	if (Task_Name==NULL) return;
	int Task_ID=-1;
	int N=LM_TASK_OBJ.GetAmount();
	for (int i=0; (i<N)&&(Task_ID==-1); i++){
		if ( strcmp(Task_Name,LM_TASK_OBJ[i]->Name.pchar())==0 ) Task_ID=LM_TASK_OBJ[i]->DS_ID;
	};
	if (Task_ID!=-1) {
		delLT_TASK(Task_ID);
	};
};// Delete task from menu list by it id in LM_TAKS
void			vvTASKS_CII::TASK_RESTORE(){
	ListDesk* pLD = getLM_TASK();
	int N=LM_TASK_OBJ.GetAmount();

	for (int i=0; i<N; i++){
		DialogsDesk* pTaskEL = (DialogsDesk*)(pLD->Find(LM_TASK_OBJ[i]->DS_ID));
		if (pTaskEL!=NULL) {
			TextButton*	 pTxtBTN = (TextButton*)(pTaskEL->Find("Mess"));
			GPPicture*	 pDEL_BTN = (GPPicture*)(pTaskEL->Find("Delete"));
			if (pDEL_BTN!=NULL&&pTxtBTN!=NULL) {
				// Container Class
				pDEL_BTN->UserParam = (int)(this);
				pTxtBTN->UserParam = (int)(this);
				// Task index in list
				char sID[32];
				sprintf(sID,"%d",LM_TASK_OBJ[i]->DS_ID);
				pDEL_BTN->AllocPtr = (char *)malloc( strlen(sID)+1 );
				if (pDEL_BTN->AllocPtr!=NULL) {
					sprintf(pDEL_BTN->AllocPtr,"%s",sID);
				};
				pTxtBTN->AllocPtr = (char *)malloc( strlen(sID)+1 );
				if (pTxtBTN->AllocPtr!=NULL) {
					sprintf(pTxtBTN->AllocPtr,"%s",sID);
				};
				// CallBack Function
				pDEL_BTN->OnUserClick=vbTASK_DELET;
				pTxtBTN->OnMouseOver=vbTASK_MOVER;
			};
		};
	};
};
void			vvTASKS_CII::CHECK_POINT_COND(){
	int NinList = LM_TASK_OBJ.GetAmount();
	if (NinList<=0)	return;
	bbObjInList* pOBinList=NULL;
	vvTEXT_EX*	 pOBinTASK=NULL;
	for (int i=0; i<NinList; i++){
		pOBinList=LM_TASK_OBJ[i];
		pOBinTASK = dynamic_cast<vvTEXT_EX*>( getByNAME(pOBinList->Name.pchar(),TASK) );
		if (pOBinList!=NULL) {
			int nPOINT=pOBinTASK->paramList_EX.GetAmount();	
			for (int p=0; p<nPOINT; p++){
				if ( ((vvPOINT2D*)(pOBinTASK->paramList_EX[p]))->CondState()==false ){
					pOBinList->DelPoint(p);
				};
			};
		};
	};
};// Check and dell if condition for point donot true
void			vvTASKS_CII::setLT_TASK_COMPLITE(const char* Task_Name){
	if (Task_Name==NULL) return;
	int Task_ID=-1;
	int mlID=-1;
	int N=LM_TASK_OBJ.GetAmount();
	for (int i=0; (i<N)&&(Task_ID==-1); i++){
		if ( strcmp(Task_Name,LM_TASK_OBJ[i]->Name.pchar())==0 ){
			Task_ID=LM_TASK_OBJ[i]->DS_ID;
			mlID=i;
		};
	};
	if (Task_ID!=-1&&mlID!=-1) {
		ListDesk* pLD = getLM_TASK();
		if (pLD!=NULL) {
			LM_TASK_OBJ[mlID]->PreDelete();
			DialogsDesk* pTaskEL = (DialogsDesk*)(pLD->Find(LM_TASK_OBJ[mlID]->DS_ID));
			if (pTaskEL!=NULL){
				GPPicture*	 pDEL_BTN = (GPPicture*)(pTaskEL->Find("Delete"));
				if (pDEL_BTN!=NULL) {
					pDEL_BTN->Visible=true;
				};
			};
		};
	};
};
ListDesk*		vvTASKS_CII::getLM_HINT(){
	return getLM_child("HINT_LIST");
};// Get ListDesck accoding to HINT
bool			vvTASKS_CII::setLM_HINT_VIS(bool _st){
	bool oldST=false;
	ListDesk* pLDH = getLM_HINT();
	if (pLDH!=NULL) {
		oldST=pLDH->Visible;
		pLDH->Visible=_st;
	};
	return oldST;
};// Set HINT LIST visibility
void			vvTASKS_CII::setLM_HINT_POS(){
	DialogsDesk* pDD = getMM_object();
	if (pDD!=NULL) {
		int DD_W = pDD->GetWidth();
		int DD_H = pDD->GetHeight();
		ListDesk* pLDH = getLM_HINT();
		if (pLDH!=NULL) {
			pLDH->Setx(mm_X+DD_W+dl_X);
			if (dl_Y!=-1) pLDH->Sety(dl_Y);
			else		  pLDH->Sety(mm_Y);
		};
	};
};// Set list menu for HINT position
bool			vvTASKS_CII::addLT_HINT(vvTEXT_EX* pH,char* name/*=NULL*/,int pos/*=0*/){
	if (pH==NULL)	return false;
	ListDesk* pLD = getLM_HINT();
	if (pLD!=NULL) {
		int newN = pLD->AddElement(pos,"");
		bbObjInList* pHL=new bbObjInList();
		pHL->DS_ID = newN;
		if (name!=NULL&&name[0]!=0)		pHL->Name=name;
		else if (pH->Name.pchar()!=NULL)	pHL->Name=pH->Name.pchar();
		else							pHL->Name="NONAME";
		DialogsDesk* pHintEL = (DialogsDesk*)(pLD->Find(newN));
		if (pHintEL!=NULL) {
			TextButton*	 pTxtBTN = (TextButton*)(pHintEL->Find("Mess"));
			if (pTxtBTN!=NULL) {
				pTxtBTN->SetMessage( pH->GetFinishedString() );
			};
			GPPicture*	 pDEL_BTN = (GPPicture*)(pHintEL->Find("Delete"));
			if (pDEL_BTN!=NULL) {
				// Container Class
				pDEL_BTN->UserParam = (int)(this);
				// Task index in list
				char sID[32];
				sprintf(sID,"%d",newN);
				pDEL_BTN->AllocPtr = (char *)malloc( strlen(sID)+1 );
				if (pDEL_BTN->AllocPtr!=NULL) {
					sprintf(pDEL_BTN->AllocPtr,"%s",sID);
				};
				// CallBack Function
				pDEL_BTN->OnUserClick=vbHINT_DELET;
			};
		};
		LM_HINT_OBJ.Add(pHL);
	};
	if (hint_sate!=4) hint_sate=3;
	return true;
};// Add element from HINT in list menu for HINT
bool			vvTASKS_CII::addLT_HINT(const char* HName,int pos/*=0*/,bool Dublicate/*=true*/){
	if (HName==NULL)	return false;
	vvTEXT_EX*	pHINT = dynamic_cast<vvTEXT_EX*>(getByNAME(HName,HINT));
	if (pHINT==NULL)	return false;
	if (Dublicate==false)	delLT_HINT(pHINT->Name.pchar());
	return addLT_HINT(pHINT,NULL,pos);
};
void			vvTASKS_CII::delLT_HINT(int Hint_ID){
	int mlID = Assign(Hint_ID,LM_HINT_OBJ);
	if (mlID==-1)	return;
	ListDesk* pLD = getLM_HINT();
	if (pLD!=NULL) {
		if (pLD->DelElementByUID(Hint_ID)){
			LM_HINT_OBJ[mlID]->PreDelete();
			LM_HINT_OBJ.DelElement(mlID);
		};
	};
};// Delete hint from menu list by it id in LM_HINT
void			vvTASKS_CII::delLT_HINT(const char* Hint_Name){
	if (Hint_Name==NULL) return;
	int Hint_ID=-1;
	int N=LM_HINT_OBJ.GetAmount();
	for (int i=0; (i<N)&&(Hint_ID==-1); i++){
		if ( strcmp(Hint_Name,LM_HINT_OBJ[i]->Name.pchar())==0 ) Hint_ID=LM_HINT_OBJ[i]->DS_ID;
	};
	if (Hint_ID!=-1) {
		delLT_HINT(Hint_ID);
	};
};// Delete hint from menu list by it id in LM_HINT
void			vvTASKS_CII::HINT_RESTORE(){
	ListDesk* pLD = getLM_HINT();
	int N=LM_HINT_OBJ.GetAmount();

	for (int i=0; i<N; i++){
		DialogsDesk* pHintEL = (DialogsDesk*)(pLD->Find(LM_HINT_OBJ[i]->DS_ID));
		if (pHintEL!=NULL) {
			TextButton*	 pTxtBTN = (TextButton*)(pHintEL->Find("Mess"));
			GPPicture*	 pDEL_BTN = (GPPicture*)(pHintEL->Find("Delete"));
			if (pDEL_BTN!=NULL&&pTxtBTN!=NULL) {
				// Container Class
				pDEL_BTN->UserParam = (int)(this);
				// Task index in list
				char sID[32];
				sprintf(sID,"%d",LM_HINT_OBJ[i]->DS_ID);
				pDEL_BTN->AllocPtr = (char *)malloc( strlen(sID)+1 );
				if (pDEL_BTN->AllocPtr!=NULL) {
					sprintf(pDEL_BTN->AllocPtr,"%s",sID);
				};
				// CallBack Function
				pDEL_BTN->OnUserClick=vbHINT_DELET;
			};
		};
	};
};
ListDesk*		vvTASKS_CII::getLM_ELSE(){
	return getLM_child("ELSE_LIST");
};// Get ListDesck accoding to ELSE
bool			vvTASKS_CII::setLM_ELSE_VIS(bool _st){
	bool oldST=false;
	ListDesk* pLDE = getLM_ELSE();
	if (pLDE!=NULL) {
		oldST=pLDE->Visible;
		pLDE->Visible=_st;
	};
	return oldST;
};// Set ELSE LIST visibility
void			vvTASKS_CII::setLM_ELSE_POS(){
	DialogsDesk* pDD = getMM_object();
	if (pDD!=NULL) {
		int DD_W = pDD->GetWidth();
		int DD_H = pDD->GetHeight();
		ListDesk* pLDE = getLM_ELSE();
		if (pLDE!=NULL) {
			pLDE->Setx(mm_X+DD_W+dl_X);
			if (dl_Y!=-1) pLDE->Sety(dl_Y);
			else		  pLDE->Sety(mm_Y);
		};
	};
};// Set list menu for ELSE position
bool			vvTASKS_CII::addLT_ELSE(vvTEXT_EX* pE,char* name/*=NULL*/,int pos/*=0*/){
	if (pE==NULL)	return false;
	ListDesk* pLD = getLM_ELSE();
	if (pLD!=NULL) {
		int newN = pLD->AddElement(pos,"");
		bbObjInList* pEL=new bbObjInList();
		pEL->DS_ID = newN;
		if (name!=NULL&&name[0]!=0)		pEL->Name=name;
		else if (pEL->Name.pchar()!=NULL)	pEL->Name=pE->Name.pchar();
		else							pEL->Name="NONAME";
		DialogsDesk* pElseEL = (DialogsDesk*)(pLD->Find(newN));
		if (pElseEL!=NULL) {
			TextButton*	 pTxtBTN = (TextButton*)(pElseEL->Find("Mess"));
			if (pTxtBTN!=NULL) {
				pTxtBTN->SetMessage( pE->GetFinishedString() );
			};
			GPPicture*	 pDEL_BTN = (GPPicture*)(pElseEL->Find("Delete"));
			if (pDEL_BTN!=NULL) {
				// Container Class
				pDEL_BTN->UserParam = (int)(this);
				// Task index in list
				char sID[32];
				sprintf(sID,"%d",newN);
				pDEL_BTN->AllocPtr = (char *)malloc( strlen(sID)+1 );
				if (pDEL_BTN->AllocPtr!=NULL) {
					sprintf(pDEL_BTN->AllocPtr,"%s",sID);
				};
				// CallBack Function
				pDEL_BTN->OnUserClick=vbELSE_DELET;
			};
		};
		LM_ELSE_OBJ.Add(pEL);
	};
	if (else_sate!=4) else_sate=3;
	return true;
};// Add element from ELSE in list menu for ELSE
bool			vvTASKS_CII::addLT_ELSE(const char* EName,int pos/*=0*/,bool Dublicate/*=true*/){
	if (EName==NULL)	return false;
	vvTEXT_EX*	pELSE = dynamic_cast<vvTEXT_EX*>(getByNAME(EName,ELSE));
	if (pELSE==NULL)	return false;
	if (Dublicate==false)	delLT_ELSE(pELSE->Name.pchar());
	return addLT_ELSE(pELSE,NULL,pos);
};
void			vvTASKS_CII::delLT_ELSE(int Else_ID){
	int mlID = Assign(Else_ID,LM_ELSE_OBJ);
	if (mlID==-1)	return;
	ListDesk* pLD = getLM_ELSE();
	if (pLD!=NULL) {
		if (pLD->DelElementByUID(Else_ID)){
			LM_ELSE_OBJ[mlID]->PreDelete();
			LM_ELSE_OBJ.DelElement(mlID);
		};
	};
};// Delete else from menu list by it id in LM_ELSE
void			vvTASKS_CII::delLT_ELSE(const char* Else_Name){
	if (Else_Name==NULL) return;
	int Else_ID=-1;
	int N=LM_ELSE_OBJ.GetAmount();
	for (int i=0; (i<N)&&(Else_ID==-1); i++){
		if ( strcmp(Else_Name,LM_ELSE_OBJ[i]->Name.pchar())==0 ) Else_ID=LM_ELSE_OBJ[i]->DS_ID;
	};
	if (Else_ID!=-1) {
		delLT_ELSE(Else_ID);
	};
};// Delete else from menu list by it id in LM_ELSE
void			vvTASKS_CII::ELSE_RESTORE(){
	ListDesk* pLD = getLM_ELSE();
	int N=LM_ELSE_OBJ.GetAmount();

	for (int i=0; i<N; i++){
		DialogsDesk* pElseEL = (DialogsDesk*)(pLD->Find(LM_ELSE_OBJ[i]->DS_ID));
		if (pElseEL!=NULL) {
			TextButton*	 pTxtBTN = (TextButton*)(pElseEL->Find("Mess"));
			GPPicture*	 pDEL_BTN = (GPPicture*)(pElseEL->Find("Delete"));
			if (pDEL_BTN!=NULL&&pTxtBTN!=NULL) {
				// Container Class
				pDEL_BTN->UserParam = (int)(this);
				// Task index in list
				char sID[32];
				sprintf(sID,"%d",LM_ELSE_OBJ[i]->DS_ID);
				pDEL_BTN->AllocPtr = (char *)malloc( strlen(sID)+1 );
				if (pDEL_BTN->AllocPtr!=NULL) {
					sprintf(pDEL_BTN->AllocPtr,"%s",sID);
				};
				// CallBack Function
				pDEL_BTN->OnUserClick=vbELSE_DELET;
			};
		};
	};
};
int				vvTASKS_CII::Assign(int el,LinearArray<int,_int>& Arr){
	int ass=-1;
	int N=Arr.GetAmount();
	for (int i=0; (i<N)&&(ass==-1); i++){
		if (Arr[i]==el)	ass=i;
	};
	return ass;
};
int				vvTASKS_CII::Assign(int el,ClassArray<bbObjInList>& Arr){
	int ass =-1;
	int N=Arr.GetAmount();
	for (int i=0; (i<N)&&(ass==-1); i++){
		if ( Arr[i]->DS_ID==el ) ass=i;
	};
	return ass;
};
bbTEXT*			vvTASKS_CII::getByNAME(const char* name,ClassArray<bbTEXT>& Arr){
	if (name==NULL)	return NULL;
	vvTEXT_EX* pOB=NULL;
	int N=Arr.GetAmount();
	for (int i=0; i<N; i++){
		pOB = dynamic_cast<vvTEXT_EX*>(Arr[i]);
		if (pOB!=NULL&&pOB->Name.pchar()!=NULL&&strcmp(pOB->Name.pchar(),name)==0)	return pOB;
	};
	return pOB;
};
void			vvTASKS_CII::Restore(){
	if (NeedRestore){
		NeedRestore=false;
		setMM_LMClick();	// Restore mouse click on letter
		// restore click on TASK/HINT/ELSE List
		TASK_RESTORE();
		HINT_RESTORE();
		ELSE_RESTORE();
	};
};
bool			vvTASKS_CII::addLT_TASK_lua(const char* TName,int pos,bool Dublicate){
	return addLT_TASK(TName,pos,Dublicate);
};
bool			vvTASKS_CII::addLT_HINT_lua(const char* HName,int pos,bool Dublicate){
	return addLT_HINT(HName,pos,Dublicate);
};
bool			vvTASKS_CII::addLT_ELSE_lua(const char* EName,int pos,bool Dublicate){
	return addLT_ELSE(EName,pos,Dublicate);
};
void			vvTASKS_CII::delLT_TASK_lua(const char* Task_Name){
	delLT_TASK(Task_Name);
};
void			vvTASKS_CII::delLT_HINT_lua(const char* Hint_Name){
	delLT_HINT(Hint_Name);
};
void			vvTASKS_CII::delLT_ELSE_lua(const char* Else_Name){
	delLT_ELSE(Else_Name);
};
void			vvTASKS_CII::vv_TASK_LCLIC(){
	if (LM_OPEN_ID==1){
		LM_OPEN_ID=0;
		task_sate=2;
	}else if (LM_TASK_OBJ.GetAmount()>0){
		if (hint_sate==4)	hint_sate=2;
		if (else_sate==4)	else_sate=2;
		LM_OPEN_ID=1;
		task_sate=4;
	}else{
		if (LM_OPEN_ID==2) {
			LM_OPEN_ID=0;
			hint_sate=2;
		};
		if (LM_OPEN_ID==3) {
			LM_OPEN_ID=0;
			else_sate=2;
		};
	};
};
void			vvTASKS_CII::vv_HINT_LCLIC(){
	if (LM_OPEN_ID==2){
		LM_OPEN_ID=0;
		hint_sate=2;
	}else if (LM_HINT_OBJ.GetAmount()>0){
		if (task_sate==4)	task_sate=2;
		if (else_sate==4)	else_sate=2;
		LM_OPEN_ID=2;
		hint_sate=4;
	}else{
		if (LM_OPEN_ID==1) {
			LM_OPEN_ID=0;
			task_sate=2;
		};
		if (LM_OPEN_ID==3) {
			LM_OPEN_ID=0;
			else_sate=2;
		};
	};
};
void			vvTASKS_CII::vv_ELSE_LCLIC(){
	if (LM_OPEN_ID==3){
		LM_OPEN_ID=0;
		else_sate=2;
	}else if (LM_ELSE_OBJ.GetAmount()>0){
		if (task_sate==4)	task_sate=2;
		if (hint_sate==4)	hint_sate=2;
		LM_OPEN_ID=3;
		else_sate=4;
	}else{
		if (LM_OPEN_ID==1) {
			LM_OPEN_ID=0;
			task_sate=2;
		};
		if (LM_OPEN_ID==2) {
			LM_OPEN_ID=0;
			hint_sate=2;
		};
	};
};
void	vvTASKS_CII::FCN00::EvaluateFunction(){
	vvTASKS_CII* pPR = get_parent<vvTASKS_CII>();
	if (pPR!=NULL) {
		pPR->DS_LOAD();
	};
};
void	vvTASKS_CII::FCN01::EvaluateFunction(){
	vvTASKS_CII* pPR = get_parent<vvTASKS_CII>();
	if (pPR!=NULL) {
		pPR->applyMM_pos();
	};
};
void	vvTASKS_CII::FCN02::EvaluateFunction(){
	vvTASKS_CII* pPR = get_parent<vvTASKS_CII>();
	if (pPR!=NULL) {
		pPR->applyLM_pos();
	};
};
void	vvTASKS_CII::FCN03::EvaluateFunction(){
	vvTASKS_CII* pPR = get_parent<vvTASKS_CII>();
	if (pPR!=NULL) {
		pPR->setMM_LMClick();
	};
};
void	vvTASKS_CII::FCN04::EvaluateFunction(){
	vvTASKS_CII* pPR = get_parent<vvTASKS_CII>();
	if (pPR!=NULL) {
		pPR->setLM_ID();
	};
};
void	vvTASKS_CII::FCN05::EvaluateFunction(){
	vvTASKS_CII* pPR = get_parent<vvTASKS_CII>();
	if (pPR!=NULL) {
		TASK.getELEMENT("../TASK Name");
		if (TASK.INDEX!=-1){
			vvTEXT_EX* pT = dynamic_cast<vvTEXT_EX*>(pPR->TASK[TASK.INDEX]);
			pPR->addLT_TASK(pT);
		};
	};
};
void	vvTASKS_CII::FCN06::EvaluateFunction(){
	vvTASKS_CII* pPR = get_parent<vvTASKS_CII>();
	if (pPR!=NULL) {
		HINT.getELEMENT("../HINT Name");
		if (HINT.INDEX!=-1) {
			vvTEXT_EX* pH = dynamic_cast<vvTEXT_EX*>(pPR->HINT[HINT.INDEX]);
			pPR->addLT_HINT(pH);
		};
	};
};
void	vvTASKS_CII::FCN07::EvaluateFunction(){
	vvTASKS_CII* pPR = get_parent<vvTASKS_CII>();
	if (pPR!=NULL) {
		ELSE.getELEMENT("../ELSE Name");
		if (ELSE.INDEX!=-1) {
			vvTEXT_EX* pE = dynamic_cast<vvTEXT_EX*>(pPR->ELSE[ELSE.INDEX]);
			pPR->addLT_ELSE(pE);
		};
	};
};
// vvBrigAI //////////////////////////////////////////////////////////////
void	AI_BORDER::setRaport(){
	switch(CURRENT_ORDER) {
	case 0:	// ai_STOP
		OrderName = "STOP";
		break;
	case 1:	// ai_MOVE
		OrderName = "MOVE";
		break;
	};
	CreateTime = GetTickCount();
	ImplemTime = GetTickCount();
	setTimeInHMSMs(CreateTmBC,CreateTime);
	setTimeInHMSMs(ImplemTmBC,ImplemTime);
};
void	AI_BORDER::setTimeInHMSMs(_str& strval,DWORD time){
	time_t aclock;
	struct tm *newtime;

	int msec = (int)(CreateTime-(CreateTime/1000)*1000);
	aclock = (time_t)(CreateTime/1000);
	newtime->tm_hour=0;
	newtime->tm_min=0;
	newtime->tm_sec=0;
	newtime = localtime( &aclock );
	strval = "";
	strval += newtime->tm_hour;
	strval += " : ";
	strval += newtime->tm_min;
	strval += " : ";
	strval += newtime->tm_sec;
	strval += " : ";
	strval += msec;
};
DWORD	AI_BORDER::GetClassMask(){
	DWORD MASK = 0x00000001;
	switch(CURRENT_ORDER) {
	case 0:	// ai_STOP
		MASK = 0x00000001;
		break;
	case 1:	// ai_MOVE
		MASK = 0x00000002;
		break;
	};
	return MASK;
};
void	AI_BORDER::AI_RAPORT::EvaluateFunction(){
	AI_BORDER* pPR = get_parent<AI_BORDER>();
	if (pPR!=NULL) {
		pPR->setRaport();
	};
};
vvBrigAI::vvBrigAI(){
	InfID = _vvBrigAI_;
};
vvBrigAI::~vvBrigAI(){};
void	vvBrigAI::setDefSettings(){
};// Set settings in defoult
bool	vvBrigAI::setAI_BrigID(int _id){
	return true;
};// Set AI controled brigade id
bool	vvBrigAI::addEN_BrigID(int _id){
	return true;
};// add EN brigades id
void	vvBrigAI::UpdateState(){
};// Update state param
void	vvBrigAI::CreateOrderList(){
};// Create new order if need
void	vvBrigAI::ImplementOrdes(){
};// Implament order
void	vvBrigAI::PROCESS(){
	UpdateState();
	CreateOrderList();
	ImplementOrdes();
};// General process function
// OperationMesMgr ///////////////////////////////////////////////////////
OperationMesMgr::OperationMesMgr() : SimpleMesMgr() {
	// ...
};
OperationMesMgr::OperationMesMgr(OperationMesMgr& MM) : SimpleMesMgr( *( dynamic_cast<SimpleMesMgr*>(&MM) ) ) {
	SCRIPT.Clear();
	int N = MM.SCRIPT.GetAmount();
	lvCBaseScript* pCurSCR=NULL;
	while (N--) {
		MM.SCRIPT[N]->GetCopy((lvCBaseScript**)(&pCurSCR));
		if (pCurSCR!=NULL) {
			SCRIPT.Add(pCurSCR);
		};
		pCurSCR=NULL;
	};
};
OperationMesMgr::~OperationMesMgr(){
	SCRIPT.Clear();
};
void		OperationMesMgr::ShowDialog(){
	SimpleMesMgr::ShowDialog();

    int N = SCRIPT.GetAmount();
	for (int i=0; i<N; i++){
		SCRIPT[i]->Process(0);
	};
};

//======================================================================//
//======================================================================//

// lvCEdge ///////////////////////////////////////////////////////////////
#define EDGE_FONT	SmallWhiteFont1
#define pEDGE_FONT	&SmallWhiteFont1
extern int LastMx;
extern int LastMy;
void	lvDrawArrow(int x,int y,int x1,int y1,int dx,int dy,DWORD color);
void	lvDrawArrow(be_CNODE* nd1,be_CNODE* nd2,int dx,int dy,DWORD color);

lvCEdge::lvCEdge(lvCEdge* pEdge) : lvCStorage(dynamic_cast<lvCStorage*>(pEdge)) {
	if (pEdge==NULL)	return;

	begID=pEdge->begID;
	endID=pEdge->endID;
	procTime=pEdge->procTime;
	startTime=pEdge->startTime;

	// Select Condition
	lvCCondition* _pSelectCondition = NULL;
	if (pEdge->SelectCondition.Get()!=NULL){
		pEdge->SelectCondition.Get()->GetCopy(&_pSelectCondition);
	};
	if (_pSelectCondition!=NULL) SelectCondition.Set(_pSelectCondition);
	_pSelectCondition=NULL;
	
	// Condition / Operation
	lvCCondForOper* _pCondForOper=NULL;
	for (int i=0; i<pEdge->CondForOper.GetAmount(); i++){
		pEdge->CondForOper[i]->GetCopy( (lvCBaseScript**)(&_pCondForOper) );
		if (_pCondForOper!=NULL) CondForOper.Add(_pCondForOper);
		_pCondForOper=NULL;
	};

	// Stop Condition
	lvCCondition* _pStopCondition = NULL;
	if (pEdge->StopCondition.Get()!=NULL){
		pEdge->StopCondition.Get()->GetCopy(&_pStopCondition);
	};
	if (_pStopCondition!=NULL) StopCondition.Set(_pStopCondition);
	_pStopCondition=NULL;

	id=pEdge->id;
};

void	lvCEdge::vINIT (char* CLName/* =NULL */,DWORD ID/* =0 */,char* FLName/* =NULL */){
	ClassName	= ( (CLName==NULL) ? ("lvCEdge") : (CLName) );    
	InfID		= ( (ID==0) ? (_lvCEdge_) : (ID) );   
	FileNameXML	= ( (FLName==NULL) ? ("lvCEdge.xml") : (FLName) ); 

	Name		= "NoName";
	DESCR		= "NoDescription";

	begID		= 0;
	endID		= 0;
	procTime	= 0;
	startTime	= 0;
	id			= 0;
};

void	lvCEdge::vCLEAN(){
	begID		= 0;
	endID		= 0;
	procTime	= 0;
	startTime	= 0;
	id			= 0;
};

void	lvCEdge::vDeleteDynamicData(){
	SelectCondition.Clear();
	CondForOper.Clear();
};

void	lvCEdge::vSetID(DWORD _id){
	id = _id;
};

void	lvCEdge::vSetBegEndNodeID(int _beg,int _end){
	if (_beg!=0xFFFFFFFF) {
		vSetBeg(_beg);
	};
	if (_end!=0xFFFFFFFF) {	
		vSetEnd(_end);
	};
};

void	lvCEdge::vSetBegEndNodePT(be_CNODE* _beg,be_CNODE* _end){
	if (_beg!=NULL && _end!=NULL) {
		begID = _beg->GetObjectGlobalID();
		endID = _end->GetObjectGlobalID();
  	};	
};

void	lvCEdge::vSetBeg(int _x,int _y){
	be_CNODE* pND = NodesMap()->vGetNode(_x,_y);
	(pND!=NULL) ? (begID=pND->GetObjectGlobalID()) : (begID=0);
};

void	lvCEdge::vSetEnd(int _x,int _y){
	be_CNODE* pND = NodesMap()->vGetNode(_x,_y);
	(pND!=NULL) ? (endID=pND->GetObjectGlobalID()) : (endID=0);
};

void	lvCEdge::vSetBeg(int _ndID){
	be_CNODE* pND = NodesMap()->vGetNode(_ndID);
	(pND!=NULL) ? (begID=pND->GetObjectGlobalID()) : (begID=0);
};

void	lvCEdge::vSetEnd(int _ndID){
	be_CNODE* pND = NodesMap()->vGetNode(_ndID);
	(pND!=NULL) ? (endID=pND->GetObjectGlobalID()) : (endID=0);
};

void	lvCEdge::vSetProcTime(int _pT){
	procTime = _pT;
};

void	lvCEdge::vSetStartTime(int _sT){
	startTime = _sT;
};

void	lvCEdge::vSetMODE(DWORD _mode){
	
};

void	lvCEdge::vSetEdgeID(int _beg,int _end,int _pT,int _sT,DWORD _mode,DWORD _id/* =0 */,char* name/* =NULL */,char* descr/* =NULL */){
	vSetBegEndNodeID(_beg,_end);
	vSetProcTime(_pT);
	vSetStartTime(_sT);
	vSetMODE(_mode);
	vSetID(_id);
};

void	lvCEdge::vSetEdgePT(be_CNODE* _beg,be_CNODE* _end,int _pT,int _sT,DWORD _mode,DWORD _id/* =0 */,char* name/* =NULL */,char* descr/* =NULL */){
	vSetBegEndNodePT(_beg,_end);
	vSetProcTime(_pT);
	vSetStartTime(_sT);
	vSetMODE(_mode);
	vSetID(_id);
};

bool	lvCEdge::vCheckEdge(){
	return	(NodesMap()->vGetNode(begID)!=NULL)&&(NodesMap()->vGetNode(endID)!=NULL);
};

void	lvCEdge::vDrawDirection(int _x,int _y,int& maxLen,DWORD& pos,bool setLen){
	char color[56];
	sprintf(color,"%s%x%s","{C 0x",DriveMode()->EdgeNameColor,"}");
	const	int		dT	  = 4;
	const	DWORD	MAX_STR	 = 100;
	char	NDtoND[MAX_STR];
	sprintf(NDtoND,"%s%d%s%d",color,begID," -> ",endID);
	extern	int GetRLen(char* s,RLCFont* font);
	if (maxLen<GetRLen(NDtoND,pEDGE_FONT))	maxLen = GetRLen(NDtoND,pEDGE_FONT);
	if (setLen)	return;
	extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
	ShowStringEx(_x+dT,_y+dT+pos*10,NDtoND,pEDGE_FONT); pos++;
};

void	lvCEdge::vDrawProcTime(int _x,int _y,int& maxLen,DWORD& pos,bool setLen){
	char color[56];
	sprintf(color,"%s%x%s","{C 0x",DriveMode()->EdgeNameColor,"}");
	const	int		dT	  = 4;
	const	DWORD	MAX_STR	 = 100;
	char	ProcTm[MAX_STR];
	sprintf(ProcTm,"%s%s%d",color,"Proc ",procTime);
	extern	int GetRLen(char* s,RLCFont* font);
	if (maxLen<GetRLen(ProcTm,pEDGE_FONT))	maxLen = GetRLen(ProcTm,pEDGE_FONT);
	if (setLen)	return;
	extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
	ShowStringEx(_x+dT,_y+dT+pos*10,ProcTm,pEDGE_FONT); pos++;
};

void	lvCEdge::vDrawStartTime(int _x,int _y,int& maxLen,DWORD& pos,bool setLen){
	char color[56];
	sprintf(color,"%s%x%s","{C 0x",DriveMode()->EdgeNameColor,"}");
	const	int		dT	  = 4;
	const	DWORD	MAX_STR	 = 100;
	char	StarTm[MAX_STR];
	sprintf(StarTm,"%s%s%d",color,"Start ",startTime);
	extern	int GetRLen(char* s,RLCFont* font);
	if (maxLen<GetRLen(StarTm,pEDGE_FONT))	maxLen = GetRLen(StarTm,pEDGE_FONT);
	if (setLen)	return;
	extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
	ShowStringEx(_x+dT,_y+dT+pos*10,StarTm,pEDGE_FONT); pos++;
};

void	lvCEdge::vDarwMode(int _x,int _y,int& maxLen,DWORD& pos,bool setLen){
	char color[56];
	sprintf(color,"%s%x%s","{C 0x",DriveMode()->EdgeNameColor,"}");
	const	int		dT	  = 4;
	const	DWORD	MAX_STR	 = 100;
	char	ModeSt[MAX_STR];
	sprintf(ModeSt,"%s%s",color,"ATTACK");
	extern	int GetRLen(char* s,RLCFont* font);
	if (maxLen<GetRLen(ModeSt,pEDGE_FONT))	maxLen = GetRLen(ModeSt,pEDGE_FONT);
	if (setLen)	return;
	extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
	ShowStringEx(_x+dT,_y+dT+pos*10,ModeSt,pEDGE_FONT); pos++;
};

void	lvCEdge::vDarwData(int _x, int _y,float _cosF,float _sinF,DWORD mode/* =1 */){
	DWORD	color = DriveMode()->EdgeLineColor;
	const	int		dT	  = 4;

	int		MAX_STRING	= 0;
	int		curMaxLen	= 0;

	DWORD pos = 0;
	float	x = 0.f;
	float	y = 0.f;
	if (mode&_DIRECTION_)	{ MAX_STRING++; vDrawDirection((int)x,(int)y,curMaxLen,pos,true);	};
	if (mode&_PROC_TIME_)	{ MAX_STRING++; vDrawProcTime ((int)x,(int)y,curMaxLen,pos,true);	};
	if (mode&_START_TIME_)	{ MAX_STRING++; vDrawStartTime((int)x,(int)y,curMaxLen,pos,true);	};
	if (mode&_MODE_)		{ MAX_STRING++; vDarwMode	  ((int)x,(int)y,curMaxLen,pos,true);	};

	float	DELTA = 0;
	if (mode==1) {
		DELTA = 20.f;
	}else{
		DELTA = max(15.f + (float)curMaxLen/2.f + (float)dT, 10.f*(float)MAX_STRING+2.f*dT-2.f) + 5.f;
	}
	
	x = (float)_x + _sinF*DELTA;
	y = (float)_y - _cosF*2.f*DELTA;

	float	drX = (float)(curMaxLen/2+dT+1);
	float	drY = (float)(5*MAX_STRING+dT-1);

	float		rZ = (float)GetHeight((int)x,(int)y);
	Vector3D	p1(x-drX,y-drY*2.f,rZ);
	Vector3D	p2(x+drX,y-drY*2.f,rZ);
	Vector3D	p3(x+drX,y+drY*2.f,rZ);
	Vector3D	p4(x-drX,y+drY*2.f,rZ);

	if (mode&_RECT_) {
		GPS.DrawLine(p1,p2,color);
		GPS.DrawLine(p2,p3,color);
		GPS.DrawLine(p3,p4,color);
		GPS.DrawLine(p4,p1,color);
	};

	if (mode==1) {
		return;
	};
	
	Vector4D	p(p1.x,p1.y/*-260.f*/,p1.z,1);
	WorldToScreenSpace(p);
	
	pos = 0;
	if (mode&_DIRECTION_)	{ vDrawDirection(p.x,p.y,curMaxLen,pos,false); }
	if (mode&_PROC_TIME_)	{ vDrawProcTime	(p.x,p.y,curMaxLen,pos,false); }
	if (mode&_START_TIME_)	{ vDrawStartTime(p.x,p.y,curMaxLen,pos,false); }
	if (mode&_MODE_)		{ vDarwMode		(p.x,p.y,curMaxLen,pos,false); }
};

void	lvCEdge::vDraw(DWORD mode/* =1 */,DWORD _id/* =0xFFffFFff */){
	int DTX = 100;
	int DTY = 150;
	if (DriveMode()->NodeStyle==1) {
		DTX = 15;
		DTY = 25;
	};

	if (NodesMap()->vGetNode(begID)==NULL || NodesMap()->vGetNode(endID)==NULL) {
		be_CNODE* pbegND = NodesMap()->vGetNode(begID);
		be_CNODE* pendND = NodesMap()->vGetNode(endID);
		if (pbegND!=NULL) {
			lvDrawArrow(pbegND->x,pbegND->y,LastMx,LastMy,DTX,DTY,0xFFff0000);
			return;
		};
		if (pendND!=NULL) {
			lvDrawArrow(LastMx,LastMy,pendND->x,pendND->y,DTX,DTY,0xFFff0000);
			return;
		};
		return;
	}

	if (_id!=0xFFffFFff&&id!=_id) {
		return;
	};

	DWORD color = DriveMode()->EdgeLineColor;
	if (selected&&DriveMode()->OBJECT==7) color = 0xFFff0000+rand()%256;
	
	lvDrawArrow(NodesMap()->vGetNode(begID),NodesMap()->vGetNode(endID),DTX,DTY,color);

	
	if (Shifter!=5)	return;
	
	be_CNODE*	pND0 = NodesMap()->vGetNode(begID);
	be_CNODE*	pND1 = NodesMap()->vGetNode(endID);

	if (pND0==NULL || pND1==NULL) return;
		

	float	x1 = pND0->x;
	float	y1 = pND0->y;
	float	x2 = pND1->x;
	float	y2 = pND1->y;

	float	mDX = (float)DTX;
	float	mDY = (float)DTY;

	int _x0 = (int)(( x1 + x2 ) / 2.f);
	int _y0 = (int)(( y1 + y2 ) / 2.f);

	float	DDD = (float)sqrt(mDX*mDX+mDY*mDY)*0.8f;
	float	SSS = (float)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	if (-2.f*DDD-70.f<=SSS&&SSS<=2.f*DDD+70.f)	return;

	float	cosF = (x2-x1)/SSS;
	float	sinF = (y2-y1)/SSS;

	vDarwData(_x0,_y0,cosF,sinF,mode);
};

float	lvCEdge::vGetEdgeDist(int _x,int _y){
	be_CNODE* pbegND = NodesMap()->vGetNode(begID);
	be_CNODE* pendND = NodesMap()->vGetNode(endID);
	
	float x0 = (float)(pbegND->x);
	float y0 = (float)(pbegND->y);
	float x1 = (float)(pendND->x);
	float y1 = (float)(pendND->y);

	float D		= sqrtf((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	float xn	= (x1-x0)/D;
	float yn	= (y1-y0)/D;
	float x		= (float)_x-x0;
	float y		= (float)_y-y0;
	float PR	= x*xn+y*yn;

	if (PR<0) {	return sqrtf(x*x+y*y); };
	if (PR>D) {	return sqrtf(((float)_x-x1)*((float)_x-x1)*((float)_y-y1)*((float)_y-y1)); };
	
	PR = x*yn-y*xn;
	if (PR<0) {	return -PR; };
	return PR;
};

float	lvCEdge::vGetEdgeDistDirected(int _x,int _y){
	be_CNODE* pbegND = NodesMap()->vGetNode(begID);
	be_CNODE* pendND = NodesMap()->vGetNode(endID);

	float x0 = (float)(pbegND->x);
	float y0 = (float)(pbegND->y);
	float x1 = (float)(pendND->x);
	float y1 = (float)(pendND->y);

	float D		= sqrtf((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	float xn	= (x1-x0)/D;
	float yn	= (y1-y0)/D;
	float x		= (float)_x-x0;
	float y		= (float)_y-y0;
	float PR	= x*xn+y*yn;

	float dir = x*yn-y*xn;

	if (PR<0) {	
		if (dir>=0)		return sqrtf(x*x+y*y); 
		else			return -sqrtf(x*x+y*y); 
	};
	if (PR>D) {	
		if (dir>=0)		return sqrtf(((float)_x-x1)*((float)_x-x1)*((float)_y-y1)*((float)_y-y1)); 
		else			return -sqrtf(((float)_x-x1)*((float)_x-x1)*((float)_y-y1)*((float)_y-y1)); 
	};

	PR = x*yn-y*xn;
	return PR;
};

void	lvCEdge::GetCopy(lvCStorage** pCopy){
	*pCopy = dynamic_cast<lvCStorage*>(new lvCEdge(this));
};

void	lvCEdge::vSendNodesXYtoOperations(){
	int x0=0,y0=0,x1=10,y1=10;
	be_CNODE* pBegND = NodesMap()->vGetNode(begID);
	be_CNODE* pEndND = NodesMap()->vGetNode(endID);
	if (pBegND!=NULL&&pEndND!=NULL) {
		x0 = pBegND->x;	y0 = pBegND->y;
		x1 = pEndND->x;	y1 = pEndND->y;
	};
	int N = CondForOper.GetAmount();
	lvCCondition* pCOND = NULL;
	while (N--) {
		int O = CondForOper[N]->Operation.GetAmount();
		while (O--) {
			CondForOper[N]->Operation[O]->SetEdgeXY(x0,y0,x1,y1);
		};
		int C = CondForOper[N]->Condition.GetAmount();
		while (C--) {
			pCOND = dynamic_cast<lvCCondition*>(CondForOper[N]->Condition[C]);
			if (pCOND!=NULL)	pCOND->SetEdgeXY(x0,y0,x1,y1);
			pCOND = NULL;
		};
	};

	if (SelectCondition.Get()!=NULL) {
		pCOND = dynamic_cast<lvCCondition*>(SelectCondition.Get());
		if (pCOND!=NULL) pCOND->SetEdgeXY(x0,y0,x1,y1);
		pCOND = NULL;
	};

	if (StopCondition.Get()!=NULL) {
		pCOND = dynamic_cast<lvCCondition*>(StopCondition.Get());
		if (pCOND!=NULL)  pCOND->SetEdgeXY(x0,y0,x1,y1);
		pCOND = NULL;
	};
};

void	lvCEdge::vSendSquardID(DWORD sqID){
	int N = CondForOper.GetAmount();
	lvCCondition* pCOND = NULL;
	while (N--) {
		int O = CondForOper[N]->Operation.GetAmount();
		while (O--) {
			CondForOper[N]->Operation[O]->SetSquardID(sqID);
		};
		int C = CondForOper[N]->Condition.GetAmount();
		while (C--) {
			pCOND = dynamic_cast<lvCCondition*>(CondForOper[N]->Condition[C]);
			if (pCOND!=NULL)	pCOND->SetSquardID(sqID);
			pCOND = NULL;
		};
	};

	if (SelectCondition.Get()!=NULL) {
		pCOND = dynamic_cast<lvCCondition*>(SelectCondition.Get());
		if (pCOND!=NULL) pCOND->SetSquardID(sqID);
		pCOND = NULL;
	};

	if (StopCondition.Get()!=NULL) {
		pCOND = dynamic_cast<lvCCondition*>(StopCondition.Get());
		if (pCOND!=NULL)  pCOND->SetSquardID(sqID);
		pCOND = NULL;
	};
};

bool	lvCEdge::Process(int time){
	int N = CondForOper.GetAmount();
	for (int i=0; i<N; i++){
		if (CondForOper[i]->Process(time)!=0) i=N;
	};
	return	1;
};

int		lvCEdge::UpdateEdgeTime(int time, DWORD ND_ID/* =0xFFFFFFFF */){
	if ((ND_ID!=0xFFFFFFFF&&begID==ND_ID) || (ND_ID==0xFFFFFFFF)) {
		startTime = time;
		return startTime+procTime;
	};
	return time;
};

int		lvCEdge::Coplite(){
	int comp = 1;
	int CFO = CondForOper.GetAmount();
	while (comp==1&&CFO--) {
		for (int o=0; (comp==1)&&(o<CondForOper[CFO]->Operation.GetAmount()); o++){
			if (CondForOper[CFO]->Operation[o]->Complite()==0)	comp=0;
		};
	};
	return comp;
};

void	lvCEdge::RestartEdge(){
	int CON = CondForOper.GetAmount();
	while (CON--) {
		CondForOper[CON]->Restart();
	};
};
// lvCSquardShema //////////////////////////////////////////////////////////
lvCSquardShema::lvCSquardShema(lvCSquardShema* pSquardShema) : lvCStorage(dynamic_cast<lvCStorage*>(pSquardShema)) {
	if (pSquardShema==NULL)	return;
	SquardName	= pSquardShema->SquardName.pchar();
	SquardID	= pSquardShema->SquardID;
	Use_vGRP	= pSquardShema->Use_vGRP;
	lvCEdge* pEdge = NULL;
	for (int i=0; i<pSquardShema->SquardEdges.GetAmount(); i++){
		pSquardShema->SquardEdges[i]->GetCopy( (lvCStorage**)(&pEdge) );
		SquardEdges.Add(pEdge);
		pEdge=NULL;
	};
};

void		lvCSquardShema::vINIT (char* CLName/* =NULL */, DWORD ID/* =0 */, char* FLName/* =NULL */){
	ClassName	= ( (CLName==NULL) ? ("lvCSquardShema") : (CLName) );    
	InfID		= ( (ID==0) ? (_lvCSquardShema_) : (ID) );   
	FileNameXML	= ( (FLName==NULL) ? ("lvCSquardShema.xml") : (FLName) ); 

	Name		= "NoName";
	DESCR		= "NoDescription";

	SquardName	= "NoName";
	SquardID	= 0xFFFF;
};

void		lvCSquardShema::vCLEAN(){
	SquardName.Clear();
};

void		lvCSquardShema::vDeleteDynamicData(){
	SquardEdges.Clear();
};

be_CNODE*	lvCSquardShema::vAddNode(int x,int y,char* name/* =NULL */,char* descr/* =NULL */){
    return	NodesMap()->vAddNode(x,y,name,descr);
};

bool		lvCSquardShema::vDelNodeID(DWORD _id){
	bool	delComlite = false;
	while (!delComlite) {
		int N = SquardEdges.GetAmount();
		delComlite = true;
		while (N--) {
			if (SquardEdges[N]->begID==_id || SquardEdges[N]->endID==_id) {
				SquardEdges.Del(N,1);
				N=0;
				delComlite=false;
			};
		};
	};
	return	true;
};

bool		lvCSquardShema::vDelNodePT(be_CNODE* pND){
	if (pND!=NULL){
		vDelNodeID(pND->GetObjectGlobalID());
	};
	return	true;
};

bool		lvCSquardShema::vDelNode(int _x,int _y,DWORD sqID/* =0xFFFFFFFF */){
	be_CNODE* pND = vGetNode(_x,_y);
	return	vDelNodePT(pND);
};

be_CNODE*	lvCSquardShema::vGetNode(DWORD _id){
	return	NodesMap()->vGetNode(_id);
};

be_CNODE*	lvCSquardShema::vGetNode(int _x,int _y,DWORD sqID/* =0xFFFFFFFF */){
	const	float MIN_DIST	= 300.f;

	be_CNODE* pRET = NULL;
	int N = SquardEdges.GetAmount();
	
	if (N>0) {
		pRET			= vGetNode(SquardEdges[0]->begID);
		if (pRET!=NULL) {
			float minDist	= pRET->vGetNodeDist(_x,_y);
			float dist		= 0.f;
			be_CNODE*		pBegND = NULL;
			be_CNODE*		pEndND = NULL;
			while (N--) {
				pBegND = vGetNode(SquardEdges[N]->begID);
				pEndND = vGetNode(SquardEdges[N]->endID);

				if (pBegND!=NULL){
					dist = pBegND->vGetNodeDist(_x,_y);
					if (dist<minDist) {
						minDist=dist;
						pRET=pBegND;
					};
				};

				if (pEndND!=NULL) {
					dist = pEndND->vGetNodeDist(_x,_y);
					if (dist<minDist) {
						minDist=dist;
						pRET=pEndND;
					};
				};
			};
			if (minDist>MIN_DIST) {
				pRET = NULL;
			};
		};		
	};
	return	pRET;
};

DWORD		lvCSquardShema::vGetFreeEdgeID(){
	int N = SquardEdges.GetAmount();
	DWORD	newID = 0;
	while (N--) {
		if (SquardEdges[N]->id>newID)	newID = SquardEdges[N]->id;
	};
	newID++;
	return newID;
};

bool		lvCSquardShema::vAddEdgeID(int _beg,int _end,int _pT,int _sT,DWORD _mode,char* name/* =NULL */,char* descr/* =NULL */){
	int N = SquardEdges.GetAmount();
	DWORD	newID = vGetFreeEdgeID();
	lvCEdge* pEG = new lvCEdge();
	pEG->vINIT();
	pEG->vSetEdgeID(_beg,_end,_pT,_sT,_mode,newID);

	if (name!=NULL)		pEG->vSetObjectName(name);
	if (descr!=NULL)	pEG->vSetDescription(descr);
	SquardEdges.Add(pEG);
	pEG=NULL;

	return	true;
};

bool		lvCSquardShema::vAddEdgePT(be_CNODE* _beg,be_CNODE* _end,int _pT,int _sT,DWORD _mode,char* name/* =NULL */,char* descr/* =NULL */){
	int N = SquardEdges.GetAmount();
	DWORD	newID = vGetFreeEdgeID();
	lvCEdge* pEG = new lvCEdge();
	pEG->vINIT();
	pEG->vSetEdgePT(_beg,_end,_pT,_sT,_mode,newID);

	if (name!=NULL)		pEG->vSetObjectName(name);
	if (descr!=NULL)	pEG->vSetDescription(descr);
	SquardEdges.Add(pEG);
	pEG=NULL;

	return	true;
};

bool		lvCSquardShema::vAddEdge(lvCEdge* pED){
	if (pED!=NULL) {
		pED->id = vGetFreeEdgeID();
		SquardEdges.Add(pED);
		pED=NULL;
		return	true;
	};
	return	false;
};

bool		lvCSquardShema::vDelEdgeID(DWORD _id){
	int N = SquardEdges.GetAmount();
	while (N--) {
		if (SquardEdges[N]->id==_id) {
			SquardEdges.Del(N,1);
			N=0;
			return true;
		};
	};
	return	false;
};

bool		lvCSquardShema::vDelEdgePT(lvCEdge* pED){
	int N = SquardEdges.GetAmount();
	while (N--) {
		if (SquardEdges[N]==pED) {
			SquardEdges.Del(N,1);
			N=0;
			return true;
		};
	};
	return	false;
};

bool		lvCSquardShema::vDelEdge(int _x,int _y){
	lvCEdge* pED = vGetEdge(_x,_y);
	if (pED!=NULL) {
		vDelEdgePT(pED);
		return true;
	};
	return	false;
};

void		lvCSquardShema::vDraw(DWORD mode/* =1 */,DWORD _id/* =0xFFffFFff */){
	int N = SquardEdges.GetAmount();
	while (N--) {
		SquardEdges[N]->vDraw(mode);
	};
};

void		lvCSquardShema::vDrawMyNodes(DWORD mode/* =1 */){
	int N = SquardEdges.GetAmount();
	while (N--) {
		NodesMap()->vDraw(mode,SquardEdges[N]->begID);
		NodesMap()->vDraw(mode,SquardEdges[N]->endID);
	};
};

lvCEdge*	lvCSquardShema::vGetEdgeID(DWORD _id){
	lvCEdge* pRET = NULL;
	int N = SquardEdges.GetAmount();
	while (N--) {
		if (SquardEdges[N]->id==_id) {
			pRET = SquardEdges[N];
		};
	};
	return	pRET;
};

int			lvCSquardShema::vGetEdgePT(lvCEdge* pED){
	int RET = 0xFFFF;
	int N = SquardEdges.GetAmount();
	while (N--) {
		if (SquardEdges[N]==pED) {
			RET = N;
		};
	};
	return	RET;
};

lvCEdge*	lvCSquardShema::vGetEdge(int _x,int _y){
	const	float MIN_DIST	= 100.f;

	lvCEdge* pRET = NULL;
	int N = SquardEdges.GetAmount();
	if (N>0) {
		pRET			= SquardEdges[0];
		float	minDist = SquardEdges[0]->vGetEdgeDist(_x,_y);;
		float	dist	= 0.f;
		while (N--) {
			dist = SquardEdges[N]->vGetEdgeDist(_x,_y);
			if (dist<minDist) {
				minDist=dist;
				pRET = SquardEdges[N];
			};
		};
		if (minDist>MIN_DIST) {
			pRET=NULL;
		};
	};

	if (pRET!=NULL){	// Проверим нет ли обратного Edge
		int N = SquardEdges.GetAmount();
		lvCEdge* pINVERS = NULL;
		while (pINVERS==NULL&&N--) {
			if (SquardEdges[N]->begID==pRET->endID&&SquardEdges[N]->endID==pRET->begID) {
				pINVERS = SquardEdges[N];
			};
		};
		if (pINVERS!=NULL) {
			if (pRET->vGetEdgeDistDirected(_x,_y)<0) {
				pRET = pINVERS;
			};
		};
	};	

	return	pRET;
};

void		lvCSquardShema::vUpdateEdges(){
	bool	updateComplite = false;
	while (!updateComplite) {
		updateComplite = true;
		int N = SquardEdges.GetAmount();
		while (N--) {
			if (SquardEdges[N]->vCheckEdge() == false) {
				vDelEdgePT(SquardEdges[N]);
				N = 0;
				updateComplite = false;
			};
		};
	};
};

void		lvCSquardShema::vRestartEdges(){
	int EN = SquardEdges.GetAmount();
	while (EN--) {
		SquardEdges[EN]->RestartEdge();
	};
};
DWORD		lvCSquardShema::GetEdgeEndedIn(DWORD end){
	DWORD	retED_ID = 0xFFFFFFFF;
	int N = SquardEdges.GetAmount();
	while (retED_ID==0xFFFFFFFF&&N--) {
		if (SquardEdges[N]->endID==end) {
			retED_ID = N;
		};
	};
	return	retED_ID;
};

int			lvCSquardShema::GetEdgeBeginIn(DWORD beg){
	int M = SquardEdges.GetAmount();
	int N = 0;
	while (M--) {
		if (SquardEdges[M]->begID==beg){
			N++;
		};
	};
	return	N;
};

void		lvCSquardShema::GetEdgeBeginIn(lvCEdge** pEdges,int N,DWORD beg){
	int M = SquardEdges.GetAmount();
	int i = 0;
	while ((M--)&&(i<N)) {
		if (SquardEdges[M]->begID==beg){
			pEdges[i++]=SquardEdges[M];
		};
	};
};

DWORD		lvCSquardShema::vGetProbablyFirstNodeID(){
	// Поиск ребра в начале которого не заканчиваются другие.
	DWORD   retNodeID = -1;
	DWORD	retID = 0xFFffFFff;
	int N = SquardEdges.GetAmount();
	DWORD	end = 0xFFffFFff;
	DWORD	curED = 0xFFffFFff;
    if (N>0) {
		end = SquardEdges[0]->begID;
		retID = curED = 0;
	}else{
		return	0xFFffFFff;
	};
	while (curED!=0xFFffFFff&&N--) {
		curED = GetEdgeEndedIn(end);
		if (curED!=0xFFffFFff) {
			end = SquardEdges[curED]->begID;
			retID = curED;
		};
	};
	retNodeID = SquardEdges[retID]->begID;
	
	if (GetEdgeEndedIn(SquardEdges[retID]->begID)!=0xFFffFFff){
		// Finde nearest node.
		retNodeID = vGetNearestNode();	
	};

	return	retNodeID;
};

DWORD		lvCSquardShema::vGetNearestNode(){
	DWORD nearestNodeID = 0xFFFF;
	int x=0,y=0;
	if (Use_vGRP) {	// For vGroups
		be_CGROUP* pvGRP = GroupsMap()->GetGroupID(SquardID);
		if (pvGRP!=NULL) {
			pvGRP->GetGroupCenter(x,y);
		};
	}else{			// For AGroups
		if (0<=SquardID&&SquardID<=AGroups.GetAmount()-1) {
			int N = AGroups[SquardID]->Units.GetAmount();
			int NUM = 0;
            OneObject* pOB = NULL;
			while (N--) {
				if (AGroups[SquardID]->Units[N].ID<0xFFFE){
					pOB = Group[AGroups[SquardID]->Units[N].ID];
					if (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&pOB->Serial==AGroups[SquardID]->Units[N].SN) {
						x += pOB->RealX;
						y += pOB->RealY;
						NUM++;
					};
				};
				pOB = NULL;
			};
			if (NUM!=0) {
				x = (x/NUM)>>4;
				y = (y/NUM)>>4;
			}else{
				x=y=0;
			}
		};
	};

	if (x!=0||y!=0) {
		int N = SquardEdges.GetAmount();
		if (N!=0) {
			int minDist = -1;
			int begID=0,endID=0;
			be_CNODE *pbegND=NULL,*pendND=NULL;
			while (N--) {
				begID = SquardEdges[N]->begID;
				endID = SquardEdges[N]->endID;
				pbegND = NodesMap()->vGetNode(begID);
				pendND = NodesMap()->vGetNode(endID);
				if ( pbegND!=NULL && ( pbegND->vGetNodeDist(x,y)<minDist || minDist==-1 ) ){
					minDist = pbegND->vGetNodeDist(x,y);
					nearestNodeID = begID;
				};
				if ( pendND!=NULL && ( pendND->vGetNodeDist(x,y)<minDist || minDist==-1 ) ){
					minDist = pendND->vGetNodeDist(x,y);
					nearestNodeID = endID;
				};
			};
		};
	};

	return	nearestNodeID;
};

void		lvCSquardShema::GetCopy(lvCStorage** pCopy){
	*pCopy = dynamic_cast<lvCStorage*>(new lvCSquardShema(this));
};

void		lvCSquardShema::Select(bool _select){
	for (int i=0; i<SquardEdges.GetAmount(); i++){
		SquardEdges[i]->selected=_select;
		(NodesMap()->vGetNode(SquardEdges[i]->begID))->selected=_select;
		(NodesMap()->vGetNode(SquardEdges[i]->endID))->selected=_select;
	};
};

bool		lvCSquardShema::Process(int time){
	return true;
};

int			lvCSquardShema::UpdateEdgeTime(int time, DWORD ND_ID/* =0xFFFFFFFF */){
	if (ND_ID!=0xFFFFFFFF) {
		int N = SquardEdges.GetAmount();
		while (N--) {
			SquardEdges[N]->UpdateEdgeTime(time,ND_ID);
		};
	};
	return	0;
};

// lvCSquardsOnMap ///////////////////////////////////////////////////////
extern	ClassArray<ActiveGroup> AGroups;
DWORD lvCSquardsOnMap::GetValue(const char* ID){
	int N=AGroups.GetAmount();
	while (N--) {
		if (strcmp(ID,AGroups[N]->Name.pchar())==0) {
			return	N;
		};
	};
	return	0xFFFFFFFF;
};

char* lvCSquardsOnMap::GetValue(DWORD ID){
	if (0<=ID&&ID<AGroups.GetAmount()) {
		return	AGroups[ID]->Name.pchar();
	};
	return	NULL;
};

int   lvCSquardsOnMap::GetAmount(){
	return	AGroups.GetAmount();
};

char* lvCSquardsOnMap::GetIndexedString(int idx){
	return	GetValue(idx);
};

DWORD lvCSquardsOnMap::GetIndexedValue (int idx){
	if (0<=idx&&idx<AGroups.GetAmount()) return idx;
	return	0xFFFFFFFF;
};


// lvCSquardsOnMap ///////////////////////////////////////////////////////
DWORD lvCZonesOnMap::GetValue(const char* ID){
	int N=AZones.GetAmount();
	while (N--) {
		if (strcmp(ID,AZones[N]->Name.pchar())==0) {
			return	N;
		};
	};
	return	0xFFFFFFFF;
};

char* lvCZonesOnMap::GetValue(DWORD ID){
	if (0<=ID&&ID<AZones.GetAmount()) {
		return	AZones[ID]->Name.pchar();
	};
	return	NULL;
};

int   lvCZonesOnMap::GetAmount(){
	return	AZones.GetAmount();
};

char* lvCZonesOnMap::GetIndexedString(int idx){
	return	GetValue(idx);
};

DWORD lvCZonesOnMap::GetIndexedValue (int idx){
	if (0<=idx&&idx<AZones.GetAmount()) return idx;
	return	0xFFFFFFFF;
};


// lvCBattleShema_ST /////////////////////////////////////////////////////
void			lvCBattleShema_ST::vINIT (char* CLName/* =NULL */, DWORD ID/* =0 */, char* FLName/* =NULL */){
	ClassName	= ( (CLName==NULL) ? ("lvCBattleShema_ST") : (CLName) );    
	InfID		= ( (ID==0) ? (_lvCBattleShema_ST_) : (ID) );   
	FileNameXML	= ( (FLName==NULL) ? ("lvCBattleShema_ST.xml") : (FLName) ); 

	Name		= "NoName";
	DESCR		= "NoDescription";
};

void			lvCBattleShema_ST::vCLEAN(){

};

void			lvCBattleShema_ST::vDeleteDynamicData(){
	BattlePlan.Clear();
	vGRP_Plan.Clear();
};

ClassArray<lvCSquardShema>*	lvCBattleShema_ST::GetPlan(){
	if (DriveMode()->USE_vGRP) {
		return	&vGRP_Plan;
	};
	return &BattlePlan;
};

void			lvCBattleShema_ST::AddPlan_vGRP(be_CGROUP* pGRP){
	int N=vGRP_Plan.GetAmount();
	bool newSS = true;
	while (N--) {
		if (vGRP_Plan[N]->SquardID==pGRP->GetObjectGlobalID()) {
			newSS = false;
		};
	};
	if (newSS==false)	return;
	lvCSquardShema* pNewShema = new lvCSquardShema;
	pNewShema->vINIT();
	pNewShema->SquardName = pGRP->GetName();
	pNewShema->SquardID = pGRP->GetObjectGlobalID();
	pNewShema->Use_vGRP = true;
	vGRP_Plan.Add(pNewShema);
	pNewShema = NULL;	
};

void			lvCBattleShema_ST::DelPlan_vGRP(be_CGROUP* pGRP){
	int N=vGRP_Plan.GetAmount();
	int SS_ID = 0xFFFF;
	while (N--) {
		if (vGRP_Plan[N]->SquardID==pGRP->GetObjectGlobalID()) {
			SS_ID = N;
		};
	};
	if (SS_ID!=0xFFFF) {
		vGRP_Plan.Del(SS_ID,1);
	};
};

void			lvCBattleShema_ST::vDraw(DWORD mode/* =1 */,DWORD _id/* =0xFFffFFff */){
	ClassArray<lvCSquardShema>* pPLAN = GetPlan();
	if (pPLAN==NULL)	return;

	int N = pPLAN->GetAmount();
	if (_id==0xFFffFFff) {
		while (N--) {
			(*pPLAN)[N]->vDraw(mode);
		};
	}else{
		while (N--) {
			if ((*pPLAN)[N]->SquardID==_id) {
				(*pPLAN)[N]->vDraw(mode);
				N=0;
			};
		};
	};
};

void			lvCBattleShema_ST::vDrawMyNodes(DWORD _id,DWORD mode/* =1 */){
	lvCSquardShema* pSS = vGetSqShemaID(_id);
	if (pSS!=NULL) {
		pSS->vDrawMyNodes(mode);
	};
};

bool			lvCBattleShema_ST::vDelEdge(int _GrpID,int _x,int _y){
	lvCSquardShema* pSS = vGetSqShemaID(_GrpID);
	if (pSS!=NULL) {
		return	pSS->vDelEdge(_x,_y);
	};
	return false;
};

bool			lvCBattleShema_ST::vAddEdge(int _GrpID,int _begND_ID,int _endND_ID){
	if (_begND_ID==_endND_ID) {
		return false;
	};
	vUpdateGroups();
	lvCSquardShema* pSS = vGetSqShemaID(_GrpID);
	if (pSS!=NULL) {
		pSS->vAddEdgeID(_begND_ID,_endND_ID,0,0,0);
		return	true;
	};
	return false;
};

bool			lvCBattleShema_ST::vAddEdge(int _GrpID,lvCEdge* pED){
	if (pED->begID==pED->endID&&pED!=NULL) {
		delete	pED;
		pED = NULL;
	};
	vUpdateGroups();
	lvCSquardShema* pSS = vGetSqShemaID(_GrpID);
	if (pSS!=NULL) {
		pSS->vAddEdge(pED);
		return	true;
	};
	return false;
};

lvCEdge*		lvCBattleShema_ST::vGetEdge(int _GrpID,int _x,int _y){
	lvCEdge* pED = NULL;
	lvCSquardShema* pSS = vGetSqShemaID(_GrpID);
	if (pSS!=NULL) {
		pED = pSS->vGetEdge(_x,_y);
	};
	return	pED;
};

void			lvCBattleShema_ST::SelectSquardShema(int _GrpID,bool _select){
   lvCSquardShema* pSS = vGetSqShemaID(_GrpID);
   if (pSS!=NULL) {
	   pSS->Select(_select);
   };
};


be_CNODE*		lvCBattleShema_ST::vAddNode(int x,int y,char* name/* =NULL */,char* descr/* =NULL */){
	return	NodesMap()->vAddNode(x,y,name,descr);
};

bool			lvCBattleShema_ST::vDelNodeID(DWORD _id){
	ClassArray<lvCSquardShema>* pPLAN = GetPlan();
	if (pPLAN==NULL)	return false;	

	int N = pPLAN->GetAmount();
	while (N--) {
		(*pPLAN)[N]->vDelNodeID(_id);
	};
	return	true;
};

bool			lvCBattleShema_ST::vDelNodePT(be_CNODE* pND){
	ClassArray<lvCSquardShema>* pPLAN = GetPlan();
	if (pPLAN==NULL)	return false;	

	int N = pPLAN->GetAmount();
	while (N--) {
		(*pPLAN)[N]->vDelNodePT(pND);
	};
	return	true;
};

bool			lvCBattleShema_ST::vDelNode(int _x,int _y,DWORD sqID/* =0xFFFFFFFF */){
    if (sqID==0xFFFFFFFF || (sqID!=0xFFFFFFFF && vGetNode(_x,_y,sqID)!=NULL)){
		ClassArray<lvCSquardShema>* pPLAN = GetPlan();
		if (pPLAN==NULL)	return false;

		int N = pPLAN->GetAmount();
		while (N--) {
			(*pPLAN)[N]->vDelNode(_x,_y);
		};
		return	true;
	};
	return false;
};

be_CNODE*		lvCBattleShema_ST::vGetNode(DWORD _id){
	return	NodesMap()->vGetNode(_id);
};

be_CNODE*		lvCBattleShema_ST::vGetNode(int _x,int _y,DWORD sqID/* =0xFFFFFFFF */){
	if (sqID!=0xFFFFFFFF) {
		lvCSquardShema* pSS = vGetSqShemaID(sqID);
		if (pSS) {
			return	pSS->vGetNode(_x,_y);
		}
		return	NULL;
	}
	return	NodesMap()->vGetNode(_x,_y);
};

lvCSquardShema*	lvCBattleShema_ST::vGetSqShemaID(int _ID){
	ClassArray<lvCSquardShema>* pPLAN = GetPlan();
	if (pPLAN==NULL)	return NULL;

	int N = pPLAN->GetAmount();
	while (N--) {
		if ((*pPLAN)[N]->SquardID==_ID) {
			return	(*pPLAN)[N];
		};
	};
	return	NULL;
};

lvCSquardShema*	lvCBattleShema_ST::vGetSqShemaCH(char* _nameID){
	ClassArray<lvCSquardShema>* pPLAN = GetPlan();
	if (pPLAN==NULL)	return NULL;

	int N = pPLAN->GetAmount();
	while (N--) {
		if (strcmp((*pPLAN)[N]->SquardName.pchar(),_nameID)==0) {
			return	(*pPLAN)[N];
		};
	};
	return	NULL;
};

void			lvCBattleShema_ST::vUpdateGroups(){
	bool oldUSE = DriveMode()->USE_vGRP;
	DriveMode()->USE_vGRP = false;

	int newN = AGroups.GetAmount();
	int oldN = BattlePlan.GetAmount();

	lvCSquardShema* pBS = NULL;
	
	for (int i=0; i<newN; i++){
		pBS = vGetSqShemaCH(AGroups[i]->Name.pchar());
		if (pBS!=NULL) {
			pBS->SquardID = i;
		};
		pBS = NULL;
	};
	
	for (i=oldN-1;i>=0;i--){
		pBS = BattlePlan[i];
		if (pBS->SquardID<newN) {
			if (strcmp(AGroups[pBS->SquardID]->Name.pchar(),pBS->SquardName.pchar())!=0){
				BattlePlan.Del(i,1);
			};
		}else{
			BattlePlan.Del(i,1);
		};
	};

	for (i=0;i<newN;i++){
		int newID = 0xFFFF;
		// Fine new group
		if (vGetSqShemaCH(AGroups[i]->Name.pchar())==NULL) {
			newID = i;
		};
		if (newID!=0xFFFF) {
			lvCSquardShema* pNewShema = new lvCSquardShema;
			pNewShema->vINIT();
			pNewShema->SquardName = AGroups[newID]->Name.pchar();
			pNewShema->SquardID = newID;
			BattlePlan.Add(pNewShema);
			pNewShema = NULL;
		};
	};

	DriveMode()->USE_vGRP = oldUSE;
};

// lvCMainScript /////////////////////////////////////////////////////////
void	lvCMainScript::vINIT (char* CLName/*=NULL*/, DWORD ID/*=0*/, char* FLName/*=NULL*/){
	ClassName	= ( (CLName==NULL) ? ("lvCMainScript") : (CLName) );    
	InfID		= ( (ID==0) ? (_lvCMainScript_) : (ID) );   
	FileNameXML	= ( (FLName==NULL) ? ("lvCMainScript.xml") : (FLName) ); 

	Name		= "NoName";
	DESCR		= "NoDescription";
};

void	lvCMainScript::vCLEAN(){
	mastInit = true;
};

void	lvCMainScript::vDeleteDynamicData(){
	MAIN_INIT.Clear();
	MAIN_SCRIPTS.Clear();
};

bool	lvCMainScript::Process(int time){
	int N = MAIN_SCRIPTS.GetAmount();
	for (int i=0; i<N; i++){
		MAIN_SCRIPTS[i]->Process(time);
	};

	MAIN_NEW_SCRIPT.CallFunction();

	return	1;
};

bool	lvCMainScript::ProcessInit(int time){
	if (mastInit) {
		mastInit = false;
		int N = MAIN_INIT.GetAmount();
		for (int i=0; i<N; i++){
			MAIN_INIT[i]->Process(time);
		};
		return true;
	};
	return false;
};

void	lvCMainScript::Draw(){
	int N = MAIN_INIT.GetAmount();
	while (N--) {
		MAIN_INIT[N]->Draw();
	};
	N = MAIN_SCRIPTS.GetAmount();
	while (N--) {
		MAIN_SCRIPTS[N]->Draw();
	};
};
void	lvCMainScript::Prepare_lua(){
	int N=MAIN_INIT.GetAmount();
	while (N--) {
		MAIN_INIT[N]->Prepare_lua();
	};
	N=MAIN_SCRIPTS.GetAmount();
	while (N--) {
		MAIN_SCRIPTS[N]->Prepare_lua();
	};
};
// lvCFilm ///////////////////////////////////////////////////////////////
void	lvCFilm::vINIT (char* CLName/*=NULL*/, DWORD ID/*=0*/, char* FLName/*=NULL*/){
	Name	= "";    
	InfID	= ( (ID==0) ? (_lvCFilm_) : (ID) );   
		
	DESCR	= "NoDescription";
};

void	lvCFilm::vCLEAN(){
	ScriptInProcess=false;
	ScriptComplit=false;
};

void	lvCFilm::vDeleteDynamicData(){
	StartCondition.Clear();
	PrepareOperation.Clear();
	STEP_CON_OPR.Clear();
	StopCondition.Clear();
	DestroyOperation.Clear();
};

bool	lvCFilm::IsInProcess(){
	return ScriptInProcess;
};

void SetCDVolumeEx(int Vol);
CIMPORT int GetCDVolume();
bool	lvCFilm::Process(int time){
	static int	MusicVolume=GetCDVolume();
	static int	SoundVolume=GSets.SVOpt.SoundVolume*EngSettings.vInterf.VolumeSoundEvents/100;
	static bool	lastSilence=GSets.CGame.SilenceMessageEvents;

	if (ScriptComplit) return 0;
	if (ScriptInProcess==false) {
		int SCN = StartCondition.GetAmount();
		int	a_start = 1;
		while (a_start==1&&SCN--) {
			if (StartCondition[SCN]->GetValue(time)==0) a_start=0;
		};

		if (a_start){
			// Remove LMode
			void ReverseLMode();
			if (LMode==true)	ReverseLMode();

			// Set dialog volume in films
			SoundVolume=GSets.SVOpt.SoundVolume;
			GSets.SVOpt.SoundVolume=SoundVolume*GSets.SVOpt.RelativDialogSoundVolume;
			ov_SetVolume(GSets.SVOpt.SoundVolume,0);
			// Save CD MUSIC volume and set new music volume
			MusicVolume=GetCDVolume();
			SetCDVolumeEx(GSets.SVOpt.SoundVolume*GSets.SVOpt.RelativDialogMusicVolume);
			
			// Clear selection from all units
			for (int i=0; i<7; i++)	ClearSelection(i);
			ClearBMASK();
			BuildMode=false;

			// Set silence in film as default
			lastSilence=GSets.CGame.SilenceMessageEvents;
			GSets.CGame.SilenceMessageEvents=true;
			/////////////////////////////////

			void RunGameTime(bool State);
			RunGameTime(false);
			ScriptInProcess=true;
			int PON = PrepareOperation.GetAmount();
			while (PON--) {
				PrepareOperation[PON]->Process(time);
			};
			return 1;
		};
	}else{
		int N=STEP_CON_OPR.GetAmount();
		while (N--) {
			STEP_CON_OPR[N]->Process(time);
		};	

		int SCN = StopCondition.GetAmount();
		int	a_stop = 1;
		while (a_stop==1&&SCN--) {
			if (StopCondition[SCN]->GetValue(time)==0) a_stop=0;
		};

		if (a_stop){
			// Restore silence in film as default
			GSets.CGame.SilenceMessageEvents=lastSilence;
			/////////////////////////////////////

			ScriptInProcess=false;
			ScriptComplit=true;
			int DON = DestroyOperation.GetAmount();
			while (DON--) {
				DestroyOperation[DON]->Process(time);
			};
			CameraDriver()->Stop();
			void RunGameTime(bool State);
			RunGameTime(true);

			// Set dialog volume in films
			GSets.SVOpt.SoundVolume=SoundVolume;
			ov_SetVolume(100,0);
			// Save CD MUSIC volume and set new music volume
			SetCDVolumeEx(MusicVolume);

			return 0;
		};
		return 1;
	};
	return 0;
};

// lvCScriptHandler_ST ////////////////////////////////////////////////////
lvCScriptHandler_ST::~lvCScriptHandler_ST(){ 
	for (int i=0; i<DS.DSS.GetAmount(); i++) DS.DSS[i]=NULL;

	SQUARDS_SCRIPTS.Clear(); 
	vGROUPS_SCRIPTS.Clear(); 
	SCRIPT_FILMS.Clear(); 
	SCRIPT_GRAPH.Clear();

	LUA_Close();
};
bool			lvCScriptHandler_ST::PROCESS_MAIN(bool inProc/*=false*/){
	static bool change=false;
	
	GroupsMap()->ReSize();
	g_beVAR_STORE.UPDATE();

	if (inProc==false)	return	0;
	MAIN_SCRIPT.ProcessInit(Time);

	int sfN = SCRIPT_FILMS.GetAmount();
	bool	mainProc = true;
	while (mainProc&&sfN--) {
		SCRIPT_FILMS[sfN]->Process(Time);
		if (SCRIPT_FILMS[sfN]->IsInProcess()==true){
			mainProc=false;
			change=true;
			GSets.SVOpt.RequiredMsPerFrame=EngSettings.DefaultGameSpeedForCampaign;
		};
	};
	if (mainProc==true){
		if (vHeroButtons.GetVisible()==false&&DriveMode()->PROCESS==true) vHeroButtons.SetVisible( true );
		vvMESSGES* pMess = dynamic_cast<vvMESSGES*>(vValuesMap()->GetVValueTypeID(_vvMESSGES_));
		if (pMess!=NULL){
			if (pMess->GetMessDS_Visbility()==false&&change&&DriveMode()->PROCESS==true){
				pMess->SetMessDS_Visbility(true);
				change=false;
			};
		};
		MAIN_SCRIPT.Process(Time);
	}else if (mainProc==false&&vHeroButtons.GetVisible()==true){
		if (DriveMode()->PROCESS==true) vHeroButtons.SetVisible( false );
		vvMESSGES* pMess = dynamic_cast<vvMESSGES*>(vValuesMap()->GetVValueTypeID(_vvMESSGES_));
		if (pMess!=NULL&&DriveMode()->PROCESS==true){
			if (pMess->GetMessDS_Visbility()==true){
				pMess->SetMessDS_Visbility(false);
			};
		};
	};
	return	1;	
};

void			lvCScriptHandler_ST::DRAW(){
    int N = SCRIPT_GRAPH.GetAmount();
	if (DriveMode()->PROCESS){
		while(N--){
			GPS.FlushBatches();
			SCRIPT_GRAPH[N]->DRAW();
		};
	};
	DS.ProcessDialogs();
    MAIN_SCRIPT.Draw();
};

bool			lvCScriptHandler_ST::PROCESS(bool onlySwitched/* = false */){
	GroupsMap()->ReSize();

	bool	oldUSE = DriveMode()->USE_vGRP;

	DriveMode()->USE_vGRP = false;
	int sqN = SQUARDS_SCRIPTS.GetAmount();
	bool	state;
	while (sqN--) {
		if (onlySwitched) {
			state = SQUARDS_SCRIPTS[sqN]->Active;
			SQUARDS_SCRIPTS[sqN]->Active = true;
		};
		PROCESS(SQUARDS_SCRIPTS[sqN]->SQUARD);
		if (onlySwitched) {
			SQUARDS_SCRIPTS[sqN]->Active = state;
		};
	};

	DriveMode()->USE_vGRP = true;
	sqN = vGROUPS_SCRIPTS.GetAmount();
	while (sqN--) {
		if (onlySwitched) {
			state = vGROUPS_SCRIPTS[sqN]->Active;
			vGROUPS_SCRIPTS[sqN]->Active = true;
		};
		PROCESS(vGROUPS_SCRIPTS[sqN]->SQUARD);
		if (onlySwitched) {
			vGROUPS_SCRIPTS[sqN]->Active = state;
		};
	};

	DriveMode()->USE_vGRP = oldUSE;
	return true;	
};

bool			lvCScriptHandler_ST::PROCESS(int squadID){

	lvCSquardShema* pSS = BattleShema()->vGetSqShemaID(squadID);
	lvCProcSquad*	pPS = vGetSquadScriptID(squadID);
	
	if (pSS==NULL) return true;

	be_CGROUP* pGRP = GroupsMap()->GetGroupID(pSS->SquardID);
	if (pSS!=NULL&&pPS!=NULL&&pGRP!=NULL&&pGRP->GetTotalAmount()>0) {
		if (pPS->Active==true) {
			if (pPS->NodeID==0xFFFF&&pPS->EdgeID==0xFFFF){
				pPS->NodeID		= pSS->vGetProbablyFirstNodeID();
				pPS->EdgeID		= 0xFFFF;
				pPS->TimeInProc	= 0;
			};
			if (pPS->NodeID!=0xFFFF) {
				// Select curent handler edge.
				int N = pSS->GetEdgeBeginIn(pPS->NodeID);
				if (N==0)	return false;
                lvCEdge** ppEdges = new lvCEdge*[N];
				pSS->GetEdgeBeginIn(ppEdges,N,pPS->NodeID);
				lvCEdge* pSelEdge = NULL;
                for (int i=0; i<N; i++){
					if (pSelEdge==NULL && ppEdges[i]->SelectCondition.Get()!=NULL) {
						if ((ppEdges[i]->SelectCondition.Get())->GetValue(pPS->TimeInProc)==true) {
							pSelEdge = ppEdges[i];
						};
					}else if (pSelEdge==NULL && ppEdges[i]->SelectCondition.Get()==NULL){
						pSelEdge = ppEdges[i];
					};
					ppEdges[i]=NULL;
				};
				delete[]ppEdges;
				if (pSelEdge!=NULL) {
					pPS->NodeID=0xFFFF;
					pPS->EdgeID=pSelEdge->id;
					pPS->TimeInProc	= 0;
					pSelEdge->startTime = Time;
					pSelEdge->vSendNodesXYtoOperations();
					pSelEdge->vSendSquardID(squadID);
				};
			};
			if (pPS->EdgeID!=0xFFFF) {
				lvCEdge* pEDGE = pSS->vGetEdgeID(pPS->EdgeID);
				if (pEDGE==NULL) return false;
				// Process curent edge, and if action complit set 0xFFFFFFFF and new node
				pEDGE->Process(pPS->TimeInProc);
				pPS->TimeInProc = Time - pEDGE->startTime;

				lvCCondition* pEndCond = pEDGE->StopCondition.Get();
				if ( ( pEndCond!=NULL && pEndCond->GetValue(pPS->TimeInProc)==true )
					 ||
					 ( pEndCond==NULL &&  pEDGE->procTime!=0 && pEDGE->procTime < pPS->TimeInProc  ) ) 
				{
					if (pEDGE->Coplite()||(pEndCond!=NULL&&pEndCond->Power()==1)){
						pPS->EdgeID=0xFFFF;
						pPS->NodeID=pEDGE->endID;
					};
				};
			};
		};
	};
	return true;
};

void			lvCScriptHandler_ST::TIMER(bool	inProc){
    if (inProc==true) {
		if (LastTime!=0) {
			Time += GetTickCount()-LastTime;
		};
		LastTime = GetTickCount();
		return;
    };
	if (inProc==false) {
		LastTime = 0;
		return;
	}
};

bool			lvCScriptHandler_ST::Create(){
	Time = 0;
	LastTime = 0;

	SQUARDS_SCRIPTS.Clear();
	int N = BattleShema()->BattlePlan.GetAmount();
	lvCSquardShema* pSS = NULL;
	lvCProcSquad* pPS = NULL;
	while (N--) {
		pSS = BattleShema()->BattlePlan[N];

		pPS = new lvCProcSquad;
        pPS->SQ_NAME	= pSS->SquardName.pchar();
		pPS->SQUARD		= pSS->SquardID;
		pPS->Active		= true;
		pPS->NodeID		= pSS->vGetProbablyFirstNodeID();
		pPS->EdgeID		= 0xFFFF;
		pPS->TimeInProc	= 0;

		SQUARDS_SCRIPTS.Add(pPS);
		pPS = NULL;
	};

	vGROUPS_SCRIPTS.Clear();
	N = BattleShema()->vGRP_Plan.GetAmount();
	pSS = NULL;
	pPS = NULL;
	while (N--) {
		pSS = BattleShema()->vGRP_Plan[N];

		pPS = new lvCProcSquad;
		pPS->SQ_NAME	= pSS->SquardName.pchar();
		pPS->SQUARD		= pSS->SquardID;
		pPS->Active		= true;
		pPS->NodeID		= pSS->vGetProbablyFirstNodeID();
		pPS->EdgeID		= 0xFFFF;
		pPS->TimeInProc	= 0;

		vGROUPS_SCRIPTS.Add(pPS);
		pPS = NULL;
	};

	// add graph to DialogsSystem
	lvCDialogBased* pDB = NULL;
	int NG = SCRIPT_GRAPH.GetAmount();
    for (int i=0; i<NG; i++){
		if (SCRIPT_GRAPH[i]->InfID==_lvCDialogBased_){
			pDB = dynamic_cast<lvCDialogBased*>( SCRIPT_GRAPH[i] );
			if (pDB!=NULL) 	pDB->AddToDS(&DS);
			pDB=NULL;
		};
	};

	return true;
};

lvCProcSquad*	lvCScriptHandler_ST::vGetSquadScriptID(int squadID){
	lvCProcSquad* pPS = NULL;

	if (DriveMode()->USE_vGRP) {
		int N = vGROUPS_SCRIPTS.GetAmount();
		while (pPS==NULL&&N--) {
			if (vGROUPS_SCRIPTS[N]->SQUARD==squadID) {
				pPS = vGROUPS_SCRIPTS[N];
			};
		};
	}else{
		int N = SQUARDS_SCRIPTS.GetAmount();
		while (pPS==NULL&&N--) {
			if (SQUARDS_SCRIPTS[N]->SQUARD==squadID) {
				pPS = SQUARDS_SCRIPTS[N];
			};
		};
	};
	return pPS;
};

void			lvCScriptHandler_ST::AddNewFilm(char* scrName,char* scrDescr){
	if (scrName==NULL||scrDescr==NULL)	return;
	if (strcmp(scrName,"New Film")!=0&&strcmp(scrDescr,"New Film Descr")!=0){
		int N = SCRIPT_FILMS.GetAmount();
		bool	AlreadyPresent = false;
		while (!AlreadyPresent&&N--) {
			if (strcmp(SCRIPT_FILMS[N]->Name.pchar(),scrName)==0){
				AlreadyPresent = true;
			};
		};
		if (!AlreadyPresent){
			lvCFilm* pNewFilm = new lvCFilm();
			if (pNewFilm!=NULL) {
				pNewFilm->Name = scrName;
				pNewFilm->DESCR	= scrDescr;
				SCRIPT_FILMS.Add(pNewFilm);
				pNewFilm = NULL;
			};
		};
	};
};

void			lvCScriptHandler_ST::DeleteFilm(char* scrName){
	if (scrName!=NULL){
		int N = SCRIPT_FILMS.GetAmount();
		while (N--) {
			if (strcmp(SCRIPT_FILMS[N]->Name.pchar(),scrName)==0){
				SCRIPT_FILMS.DelElement(N);
				N = 0;
			};
		};
	};
};
lvCGraphObject*	lvCScriptHandler_ST::getGraphByName(const char* val){
	lvCGraphObject* ret=NULL;
	if (val!=NULL){
		int N=SCRIPT_GRAPH.GetAmount();
		while (ret==NULL&&N--) {
			if ( strcmp(val,SCRIPT_GRAPH[N]->Name.pchar())==0 ) ret=SCRIPT_GRAPH[N];
		};
	};
	return ret;
};
lvCGraphObject*	__getGraphByName(const char* val){
	return	BattleHandler()->getGraphByName(val);
};
bool			lvCScriptHandler_ST::LUA_Create(){
	bool state=true;
#ifdef  __LUA__
	if (false&&LUAC.useLua){
        state = state && LUA_MISS.OPEN();
		// class
		state = state && LUA_MISS.BIND(bind_valuesMAP		);
		state = state && LUA_MISS.BIND(bind_be_CNODE		);
		state = state && LUA_MISS.BIND(bind_be_CGROUP		);
		state = state && LUA_MISS.BIND(bind_GraphObjMAP		);
		state = state && LUA_MISS.BIND(bind_be_CLUA_FN_STORE);
		state = state && LUA_MISS.BIND(bind_be_CHashTopMAP	);
		state = state && LUA_MISS.BIND(bind_GameInterface	);
		// function
		state = state && LUA_MISS.BIND(lua_base				);
		state = state && LUA_MISS.BIND(bind_ActiveScenary	);
		state = state && LUA_MISS.BIND(bind_Condition		);
		state = state && LUA_MISS.BIND(bind_Operation		);
		if (LUAC.useLuaDEBUGER) state = state && LUA_MISS.DEBUG_PREP();
	};
#endif//__LUA__
	return state;
};
bool			lvCScriptHandler_ST::LUA_Close(){
	bool state=true;
#ifdef  __LUA__
	state = state && LUA_MISS.CLOSE();
#endif//__LUA__
	return state;
};
bool			lvCScriptHandler_ST::LUA_LoadFile(const char* FName){
#ifdef  __LUA__
	return LUA_MISS.LOAD_LUA_FILE(FName);
#endif//__LUA__
	return false;
};
void			lvCScriptHandler_ST::LUA_CallFList(){
#ifdef  __LUA__
	if ( LUA_MISS.LUA_STATE==NULL )	return;
	int N=LUAC.LUA_CALL_LIST.GetAmount();
	for ( int i=0; i<N; i++ ){
		try{
			luabind::call_function<void>( LUA_MISS.LUA_STATE, LUAC.LUA_CALL_LIST[i]->pchar() );
		}catch (...) {
			DriveMode()->PROCESS=false;
		//	_str errLUA_stack = lua_tostring(BattleHandler()->LUA_MISS.LUA_STATE, -1);
			BattleHandler()->LUA_SAFE_CLOSE_MISS();
			_str errTOPIC	= "ERROR : Call Function from Function List";
			_str errMSG		= LUAC.LUA_CALL_LIST[i]->pchar();
		//	errMSG += "\n\n";
		//	errMSG += errLUA_stack;
			MessageBox( NULL, errMSG.pchar(), errTOPIC.pchar(), MB_OK );
		};
	};
#endif//__LUA__
};

bool			lvCScriptHandler_ST::LUA_SAFE_OPEN_MISS(){
#ifdef  __LUA__
	if (LUA_MISS.LUA_STATE==NULL){
		if ( LUA_Create() ){
			// Load file in lua internal list
			int N=LUAC.LUA_FileList.GetAmount();
            for (int i=0; i<N; i++){
				LUA_LoadFile(LUAC.LUA_FileList[i]->FileName.pchar());
				if ( g_beCheckChar(LUAC.LUA_FileList[i]->AddFnName.pchar()) ){
					try{
						luabind::call_function<void>( LUA_MISS.LUA_STATE, LUAC.LUA_FileList[i]->AddFnName.pchar() );
					}catch (...) {
						DriveMode()->PROCESS=false;
					//	_str errLUA_stack = lua_tostring(BattleHandler()->LUA_MISS.LUA_STATE, -1);
						BattleHandler()->LUA_SAFE_CLOSE_MISS();
						_str errTOPIC	= "ERROR : Call Function from MAIN SCRIPT";
						_str errMSG		= LUAC.LUA_FileList[i]->AddFnName.pchar();
					//	errMSG	+= "\n\n";
					//	errMSG	+= errLUA_stack;
						MessageBox( NULL, errMSG.pchar(), errTOPIC.pchar(), MB_OK );
					};
				};
			};
			// Load file needed in current project only
			ClassArray<_str>	FileList;
			g_BE_FN_STORE.GetFileList( FileList );
			for (int i=0; i<FileList.GetAmount(); i++){
				LUA_LoadFile(FileList[i]->pchar());
			};
			FileList.Clear();
		};
	};
	return LUA_MISS.LUA_STATE==NULL;
#else//__LUA__
	return false;
#endif//__LUA__
};
void			lvCScriptHandler_ST::LUA_SAFE_CLOSE_MISS(){
#ifdef  __LUA__
	if (LUA_MISS.LUA_STATE!=NULL){
		LUA_Close();
	};
#endif//__LUA__
};
int				lvCScriptHandler_ST::GET_GC_COUNT(){
#ifdef  __LUA__
	if ( LUA_MISS.LUA_STATE!=NULL )	return lua_getgccount(LUA_MISS.LUA_STATE);
#endif//__LUA__
	return -1;
};
int				lvCScriptHandler_ST::GET_GC_THRESHOLD(){
#ifdef  __LUA__
	if ( LUA_MISS.LUA_STATE!=NULL )	return lua_getgcthreshold(LUA_MISS.LUA_STATE);
#endif//__LUA__
	return -1;
};
void			lvCScriptHandler_ST::SET_GC_THRESHOLD(int newthreshold){
#ifdef  __LUA__
	if ( LUA_MISS.LUA_STATE!=NULL )	lua_setgcthreshold (LUA_MISS.LUA_STATE,newthreshold);
#endif//__LUA__
};
void			lvCScriptHandler_ST::LUA_CREATE::EvaluateFunction(){
	lvCScriptHandler_ST* pSH = get_parent<lvCScriptHandler_ST>();
	if (pSH!=NULL){
		if (pSH->LUA_Create())	MessageBox(hwnd,"Lua open OK !!!"	,"-= LUA =-",MB_OK);
		else					MessageBox(hwnd,"Lua open ERROR !!!","-= LUA =-",MB_OK);
	};
};
void			lvCScriptHandler_ST::LUA_CLOSE::EvaluateFunction(){
	lvCScriptHandler_ST* pSH = get_parent<lvCScriptHandler_ST>();
	if (pSH!=NULL){
		if (pSH->LUA_Close())	MessageBox(hwnd,"Lua close OK !!!"	 ,"-= LUA =-",MB_OK);
		else					MessageBox(hwnd,"Lua close ERROR !!!","-= LUA =-",MB_OK);
	}
};
void			lvCScriptHandler_ST::LUA_LOAD_FILE::EvaluateFunction(){
	if	( FName.pchar()!=NULL && FName.pchar()[0] ){
		lvCScriptHandler_ST* pSH = get_parent<lvCScriptHandler_ST>();
		if (pSH!=NULL){
			_str mess; 
			mess= "Lua load file << ";
			mess+=FName.pchar();
			if ( pSH->LUA_LoadFile(FName.pchar()) )	mess+=" >> OK !!!";
			else								mess+=" >> ERROR !!!";
			MessageBox(hwnd,mess.pchar(),"-= LUA =-",MB_OK);
			if ( g_beCheckChar(AddFnName.pchar()) ){
#ifdef	__LUA__
				try{
					luabind::call_function<void>( pSH->LUA_MISS.LUA_STATE, AddFnName.pchar() );
				}catch (...) {
					DriveMode()->PROCESS=false;
				//	_str errLUA_stack = lua_tostring(BattleHandler()->LUA_MISS.LUA_STATE, -1);
					BattleHandler()->LUA_SAFE_CLOSE_MISS();
					_str errTOPIC	= "ERROR : Call Function";
					_str errMSG		= AddFnName.pchar();
				//	errMSG += "\n\n";
				//	errMSG += errLUA_stack;
					MessageBox( NULL, errMSG.pchar(), errTOPIC.pchar(), MB_OK );
				}
#endif//__LUA__
			};
		};
	}else{
		MessageBox(hwnd,"Bad file name *.lua","-= LUA =-",MB_OK);
	};
};
// vvMissionLOG //////////////////////////////////////////////////////////
vvMissionLOG::vvMissionLOG(){ 
	InfID=_vvMissionLOG_;
	Clear();
};
vvMissionLOG::~vvMissionLOG(){
	// ...
};
char*	vvMissionLOG::GetName(){
	g_vvElementView = "";
	g_vvElementView += "(QuestLOG)";
	g_vvElementView += Name.pchar();
	return	g_vvElementView.pchar();
};
const	char*	vvMissionLOG::GetThisElementView(const char* LocalName){
	GetName();
	g_vvElementView += "QN[";
	g_vvElementView += QuestN;
	g_vvElementView += "],QTE[";
	g_vvElementView += QuestTotalExperience;
	g_vvElementView += "],KTE[";
	g_vvElementView += KillsTotalExperience;
	g_vvElementView += "],TE[";
	g_vvElementView += TimeEverage;
	g_vvElementView += "],TEE[";
	g_vvElementView += TimeEverageExperience;
	g_vvElementView += "]";
	return g_vvElementView.pchar();
};
void	vvMissionLOG::Clear(){
	QuestN					= 0;
	QuestTotalExperience	= 0;
	KillsTotalExperience	= 0;
	TimeEverage				= 0;
	TimeEverageExperience	= 0;

	QuestComleteN			= 0;
	QuestComleteExperience	= 0;
	KillsTotalPseudoExperience	= 0;
	KillsPlayerPseudoExperience	= 0;
	TimePlayer					= 0;
};
void	vvMissionLOG::SetQuestData(int _QuestN, int _QuestTotalExperience){
	QuestN					= _QuestN;
	QuestTotalExperience	= _QuestTotalExperience;
	QuestComleteN			= 0;
	QuestComleteExperience	= 0;
};
void	vvMissionLOG::SetKilsData(int _KillsTotalExperience){
	KillsTotalExperience = _KillsTotalExperience;
};
void	vvMissionLOG::SetTimeData(int _TimeEverage, int _TimeEverageExperience){
	TimeEverage = _TimeEverage;
	TimeEverageExperience = _TimeEverageExperience;
};
void	vvMissionLOG::AddCopmleteQuest(int Experience){
	QuestComleteN++;
	QuestComleteExperience += Experience;
};
void	vvMissionLOG::AddKillsCopmlete(){
	KillsTotalPseudoExperience=0;
	for (int i=0; i<8; i++){
		//KillsTotalPseudoExperience += CITY[i].Account/1000;
		KillsTotalPseudoExperience += NATIONS[i].GetNationlKillingExpirience();
	};
	//KillsPlayerPseudoExperience = CITY[MyNation].Account/1000;
	KillsPlayerPseudoExperience = NATIONS[MyNation].GetNationlKillingExpirience();
	if (KillsTotalPseudoExperience==0)	return;
	float korector = 1000.f/(float)KillsTotalPseudoExperience;
	KillsTotalPseudoExperience=1000;
	KillsPlayerPseudoExperience*=korector;
};
void	vvMissionLOG::AddTimeCopmlete(){
//	TimePlayer = AnimTime/(256*25);
	int GetScaledGameTime();
	TimePlayer = GetScaledGameTime()/1000;
	if (TimePlayer<0) TimePlayer=-TimePlayer;
};
void	vvRoundValue(int& val,int key=5){
	if (key<=0)	return;
	val = (val/key)*key;
};
void	vvMissionLOG::WriteToLogClass(){
	AddKillsCopmlete();
	AddTimeCopmlete();
	// CALCULATE //
	// Time
	int Tp=0;		// Player Time
	int Tt=0;		// Everage Time
	int Ptex=0;		// Player Time Exp
	int Et=0;		// Everage Time Exp
	Tp = TimePlayer;
	Tt = TimeEverage;
	Et = TimeEverageExperience;
	Ptex = GetTimeExp(Tp,Tt,Et);
	// Kils
	int Kp=0;		// Player Kills
	int Kt=0;		// Total Kills
	int Pkex=0;		// Player Kills exp
	int Ek=0;		// Total Kills exp
	Kp = KillsPlayerPseudoExperience;
	Kt = KillsTotalPseudoExperience;
	Ek = KillsTotalExperience;
	if (Kt!=0)	Pkex = (int)( (float)Ek*( (float)Kp/(float)Kt ) );
	// Quest
	int Qp=0;		// Player Quest complete
	int Qt=0;		// Total Quest 
	int Pqex=0;		// Player Quest Exp
	int Eq=0;		// Total Quest Exp
	Qp = QuestComleteN;
	Qt = QuestN;
	Eq = QuestTotalExperience;
	Pqex = ( (QuestComleteExperience<Eq) ? (QuestComleteExperience) : (Eq) );
	// Kalibrate by coof
	float GENK = gMISS_SET.GENK;
	float TK = gMISS_SET.TK*GENK;
	float KK = gMISS_SET.KK*GENK;
	float QK = gMISS_SET.QK*GENK;
	Ptex = (int)( (float)Ptex*TK );		Et = (int)( (float)Et*TK );
	Pkex = (int)( (float)Pkex*TK );		Ek = (int)( (float)Ek*TK );
	Pqex = (int)( (float)Pqex*TK );		Eq = (int)( (float)Eq*TK );
	vvRoundValue(Ptex);		vvRoundValue(Et);
	vvRoundValue(Pkex);		vvRoundValue(Ek);
	vvRoundValue(Pqex);		vvRoundValue(Eq);
    // FORMAT //
	_str*	pTimeInfo	= new _str;
	GetTimeForamt(Tp,*pTimeInfo);	*pTimeInfo += "/";	GetTimeForamt(Tt,*pTimeInfo);
	_str*	pTimeExp	= new _str;	
	*pTimeExp  = Ptex;	*pTimeExp  += "/";	*pTimeExp  += Et;
	_str*	pKillInfo	= new _str;
	*pKillInfo = Kp;	*pKillInfo += "/";	*pKillInfo += Kt;
	_str*	pKillExp	= new _str;
	*pKillExp  = Pkex;	*pKillExp  += "/";	*pKillExp  += Ek;
	_str*	pQuestInfo	= new _str;
	*pQuestInfo= Qp;	*pQuestInfo+= "/";	*pQuestInfo+= Qt;
	_str*	pQuestExp	= new _str;
	*pQuestExp = Pqex;	*pQuestExp += "/";	*pQuestExp += Eq;
	ClassArray<_str>*	pTimeCTG  = new ClassArray<_str>;
    pTimeCTG->Add(pTimeInfo);	pTimeCTG->Add(pTimeExp);
	pTimeInfo=NULL;				pTimeExp=NULL;
	ClassArray<_str>*	pKillCTG  = new ClassArray<_str>;
	pKillCTG->Add(pKillInfo);	pKillCTG->Add(pKillExp);
	pKillInfo=NULL;				pKillExp=NULL;
	ClassArray<_str>*	pQuestCTG = new ClassArray<_str>;	
	pQuestCTG->Add(pQuestInfo);	pQuestCTG->Add(pQuestExp);
	pQuestInfo=NULL;			pQuestExp=NULL;
	// SAVE //
	gExpMan()->RemoveInfo();
	gExpMan()->EXPA = Ptex+Pkex+Pqex;
	gExpMan()->DATA.Add(pTimeCTG);
	gExpMan()->DATA.Add(pKillCTG);
	gExpMan()->DATA.Add(pQuestCTG);
	pTimeCTG=NULL;
	pKillCTG=NULL;
	pQuestCTG=NULL;
};
char*	vvMissionLOG::GetTimeForamt(int msec,_str& Time){
	int L=msec;
	int D=L/(1*60*60*24);
	L-=D*(1*60*60*24);
	int H=L/(1*60*60);
	L-=H*(1*60*60);
	int M=L/(1*60);
	L-=M*(1*60);
	int S=L/(1);
	
//	_str Time;
	if (D!=0) {
		Time += D;
		Time += ":";
	};
	
	if (0<=H&&H<=9)	Time += "0";
	if (H==0)		Time += "0";
	else			Time += H;
	Time += ":";

	if (0<=M&&M<=9)	Time += "0";
	if (M==0)		Time += "0";
	else			Time += M;
	Time += ":";

	if (0<=S&&S<=9)	Time += "0";
	if (S==0)		Time += "0";
	else			Time += S;
		
	return Time.pchar();
};
int		vvMissionLOG::GetTimeExp(int Tp,int Tt,int Et){
	if (Tt==0)	return 0;
	// e=2.71828
	// ln(2)=0.693147
	float C = 0.693147f/Tt; 
	return (int)(expf(-C*Tp)*(float)(Et*2));
};
vvMissionLOG	vvMissionLOG::operator=(vvMissionLOG& MLOG){
	QuestN					= MLOG.QuestN;
	QuestTotalExperience	= MLOG.QuestTotalExperience;
	KillsTotalExperience	= MLOG.KillsTotalExperience;
	TimeEverage				= MLOG.TimeEverage;
	TimeEverageExperience	= MLOG.TimeEverageExperience;

	QuestComleteN			= MLOG.QuestComleteN;
	QuestComleteExperience	= MLOG.QuestComleteExperience;
	KillsTotalPseudoExperience	= MLOG.KillsTotalPseudoExperience;
	KillsPlayerPseudoExperience	= MLOG.KillsPlayerPseudoExperience;
	TimePlayer					= MLOG.TimePlayer;
	return *this;
};
// experience manager ////////////////////////////////////////////////////
int		ExperienceManager::GetExp(){
	return EXPA;
}// Total Experience
int		ExperienceManager::GetNCtg(){
	return DATA.GetAmount();
}// Categiris Nunber
int		ExperienceManager::GetNCol(int ctg){
	int ret=-1;
	if (0<=ctg&&ctg<GetNCtg()){
		ret=DATA[ctg]->GetAmount();
	};
	return ret;
}// Column Number 
bool	ExperienceManager::GetExpInfo(_str& Info,int ctg,int col){
	if (0<=ctg&&ctg<GetNCtg()&&0<=col&&col<GetNCol(ctg)){
		Info = ((*(DATA[ctg]))[col])->pchar();
		return true;
	};
	return false;
}// Category/Column Info
void	ExperienceManager::RemoveInfo(){
	EXPA=0;
	DATA.Clear();
};// Remove all information
// lvCScriptMapSaver /////////////////////////////////////////////////////
extern	BaseMesMgrST		gMessagesMap;

bool lvCBattleMapSaver::SaveToXML(xmlQuote& xml){
	GroupsMap()->BeforeSave();
	vValuesMap()->DeleteAllSeparators();

	g_beVAR_STORE.UPDATE();

	xml.SetQuoteName("BATTLE_EDITOR_ALL_SCRIPT_v1.0");

	xmlQuote*	pxmlValueMap	= new_xmlQuote( vValuesMap()->ClassName.pchar() );		// not used
/*	xmlQuote*	pxmlNodeMap		= new_xmlQuote( NodesMap()->ClassName.pchar()	);		// not used
	xmlQuote*	pxmlGroupsMap	= new_xmlQuote( GroupsMap()->ClassName.pchar() );		// not used
	xmlQuote*	pxmlBattleShema	= new_xmlQuote( BattleShema()->ClassName.pchar() );		// not used
*/
	xmlQuote*	pxml_SCRIPT		= new_xmlQuote( "SCRIPT" );
/*
	xmlQuote*	pxmlScriptFilms	= new_xmlQuote( "MissionScriptFilms" );					// not used
	xmlQuote*	pxmlScriptGraph	= new_xmlQuote( "MissionScriptGraph" );					// not used
	xmlQuote*	pxmlCameraPoss  = new_xmlQuote( "CameraPossitions"   );					// not used
	xmlQuote*	pxml_gMessagesMap = new_xmlQuote( "NewMessagesMap" );					// not used
	xmlQuote*	pxml_MISS_SET	= new_xmlQuote( "MissionSettings" );					// not used
	xmlQuote*	pxml_LUA		= new_xmlQuote( "LUA" );								// not used
*/
	xmlQuote*	pxml_VAR		= new_xmlQuote( "VARIABLES" );
	xmlQuote*	pxml_FN			= new_xmlQuote( "FUNCTIONS" );


	vValuesMap()->Save(*pxmlValueMap,vValuesMap());
/*	NodesMap()->Save(*pxmlNodeMap,NodesMap());
	GroupsMap()->Save(*pxmlGroupsMap,GroupsMap());
	BattleShema()->Save(*pxmlBattleShema,BattleShema());
*/
//	BattleHandler()->MAIN_SCRIPT.Save(*pxmlMainScript,&(BattleHandler()->MAIN_SCRIPT));
	g_beSCRIPT_STORE.Save(*pxml_SCRIPT,&g_beSCRIPT_STORE);
/*
	BattleHandler()->SCRIPT_FILMS.Save(*pxmlScriptFilms,&(BattleHandler()->SCRIPT_FILMS));
	BattleHandler()->SCRIPT_GRAPH.Save(*pxmlScriptGraph,&(BattleHandler()->SCRIPT_GRAPH));
	CameraPositons()->Save(*pxmlCameraPoss,CameraPositons());
	gMessagesMap.Save(*pxml_gMessagesMap,&gMessagesMap);
	gMISS_SET.Save(*pxml_MISS_SET,&gMISS_SET);
	BattleHandler()->Save(*pxml_LUA,&(BattleHandler()->LUAC));
*/
	g_beVAR_STORE.Save(*pxml_VAR,&g_beVAR_STORE);
	g_BE_FN_STORE.Save(*pxml_FN,&g_BE_FN_STORE);

/*
	xml.AddSubQuote(pxmlValueMap);
	xml.AddSubQuote(pxmlNodeMap);
	xml.AddSubQuote(pxmlGroupsMap);
	xml.AddSubQuote(pxmlBattleShema);
*/
	xml.AddSubQuote(pxml_SCRIPT);
/*
	xml.AddSubQuote(pxmlScriptFilms);
	xml.AddSubQuote(pxmlScriptGraph);
	xml.AddSubQuote(pxmlCameraPoss);
	xml.AddSubQuote(pxml_gMessagesMap);
	xml.AddSubQuote(pxml_MISS_SET);
	xml.AddSubQuote(pxml_LUA);
*/
	xml.AddSubQuote(pxml_VAR);
	xml.AddSubQuote(pxml_FN);


	GroupsMap()->AfterSave();

	return true;
};
bool lvCBattleMapSaver::LoadSubQuote(xmlQuote& xml, char* sqName, void* pObject, ErrorPager* pERR, void* Extra){
	bool l_LoadOK = false;
	if ( sqName && sqName[0] && pObject ){
		ErrorPager Err;
		xmlQuote* pSubQuote = xml.GetSubQuote(sqName);
		if ( pSubQuote ){
			if (pERR!=NULL) return ((BaseClass*)pObject)->Load( *pSubQuote, pObject, pERR, Extra );
			else			return ((BaseClass*)pObject)->Load( *pSubQuote, pObject, &Err, Extra );
		};
	};
	return l_LoadOK;
};
bool lvCBattleMapSaver::LoadFromXML(xmlQuote& xml){
	REG_BE_FUNCTIONS_class();
	REG_BE_CONDITION_class();
	REG_BE_DataStorageXML_class();

	ResetAllClass();

	GroupsMap()->BeforeLoad();

			LoadSubQuote( xml, "FUNCTIONS"	, &g_BE_FN_STORE );
			LoadSubQuote( xml, "VARIABLES"	, &g_beVAR_STORE );
	if (	LoadSubQuote( xml, "SCRIPT"		, &g_beSCRIPT_STORE )==false ){
		xmlQuote* pSubQuoter = xml.GetSubQuote("SCRIPT");
		if ( pSubQuoter==NULL ) pSubQuoter = xml.GetSubQuote(BattleHandler()->MAIN_SCRIPT.ClassName.pchar());
		if(pSubQuoter) {
			pSubQuoter = pSubQuoter->GetSubQuote("MAIN_NEW_SCRIPT");
		}
		ErrorPager Err;
		if ( pSubQuoter ) g_beSCRIPT_STORE.MAIN_SCRIPT.Load( *pSubQuoter, &g_beSCRIPT_STORE.MAIN_SCRIPT, &Err );
	};
	ErrorPager	Err;
	if (xml.GetSubQuote(0)!=NULL)	vValuesMap()->Load(*(xml.GetSubQuote(0)),vValuesMap(),&Err);
/*	if (xml.GetSubQuote(1)!=NULL)	NodesMap()->Load(*(xml.GetSubQuote(1)),NodesMap(),&Err);										
	if (xml.GetSubQuote(2)!=NULL)	GroupsMap()->Load(*(xml.GetSubQuote(2)),GroupsMap(),&Err);									
	if (xml.GetSubQuote(3)!=NULL)	BattleShema()->Load(*(xml.GetSubQuote(3)),BattleShema(),&Err);
*/
//	if (xml.GetSubQuote(4)!=NULL)	BattleHandler()->MAIN_SCRIPT.Load(*(xml.GetSubQuote(4)),&(BattleHandler()->MAIN_SCRIPT),&Err);
/*
	if (xml.GetSubQuote(5)!=NULL)	BattleHandler()->SCRIPT_FILMS.Load(*(xml.GetSubQuote(5)),&(BattleHandler()->SCRIPT_FILMS),&Err);
	if (xml.GetSubQuote(6)!=NULL)	BattleHandler()->SCRIPT_GRAPH.Load(*(xml.GetSubQuote(6)),&(BattleHandler()->SCRIPT_GRAPH),&Err);
	if (xml.GetSubQuote(7)!=NULL)	CameraPositons()->Load(*(xml.GetSubQuote(7)),CameraPositons(),&Err);
	if (xml.GetSubQuote(8)!=NULL)	gMessagesMap.Load(*(xml.GetSubQuote(8)),&gMessagesMap,&Err);
	if (xml.GetSubQuote(9)!=NULL)	gMISS_SET.Load(*(xml.GetSubQuote(9)),&gMISS_SET,&Err);	
	if (xml.GetSubQuote(10)!=NULL)	BattleHandler()->LUAC.Load(*(xml.GetSubQuote(10)),&(BattleHandler()->LUAC),&Err);
*/
//	if (xml.GetSubQuote(11)!=NULL)	g_beVAR_STORE.Load(*(xml.GetSubQuote(11)),&g_beVAR_STORE,&Err);
//	if (xml.GetSubQuote(12)!=NULL)	g_BE_FN_STORE.Load(*(xml.GetSubQuote(12)),&g_BE_FN_STORE,&Err);

	GroupsMap()->AfterLoad();

	g_beVAR_STORE.UPDATE_AFTER_LOAD();	// New - need for in OneObject reference for group contenid it

	return true;
};
bool lvCBattleMapSaver::ResetAllClass(){
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

	g_beSCRIPT_STORE.CLEAR();
	g_beVAR_STORE.CLEAR();
	g_BE_FN_STORE.CLEAR();
	g_beMasterDelayedMessage.reset_class(&g_beMasterDelayedMessage);
	return true;
};
bool lvCBattleMapSaver::GetXmlToSaveInMap(xmlQuote& xml){
	if (DriveMode()->BE_SAVE==false){
		SaveToXML(xml);		
	}else if (DriveMode()->GetSaveSFileName()!=NULL){
		xmlQuote* xmlMSL = new_xmlQuote("MISSION_SCRIPT");
		SaveToXML(*xmlMSL);
		xmlMSL->WriteToFile( DriveMode()->GetSaveSFileName() );
	};
	return true;
};

void lvCBattleMapSaver::SetXmlFromMap(xmlQuote& xml){
	ResetAllClass();
	if (DriveMode()->BE_SAVE==false){
		LoadFromXML(xml);	
	}else if (DriveMode()->GetSaveSFileName()!=NULL){
		xmlQuote	xmlMSL( "MISSION_SCRIPT" ); 
		xmlMSL.ReadFromFile( DriveMode()->GetSaveSFileName() );
		LoadFromXML(xmlMSL);		
	};
		
};
bool lvCBattleMapSaver::SaveToXML(char* FileName){
	xmlQuote* xmlMSL = new_xmlQuote("MISSION_SCRIPT");
	SaveToXML(*xmlMSL);
	xmlMSL->WriteToFile( FileName );
	return true;
};
bool lvCBattleMapSaver::LoadFromXML(char* FileName){
	xmlQuote	xmlMSL( "MISSION_SCRIPT" ); 
	xmlMSL.ReadFromFile( FileName );
	return LoadFromXML(xmlMSL);	
};
void	RegisterBattleEditorSaver(){
	REG_XML_SAVER(lvCBattleMapSaver);
};

inline	void	REG_BE_DataStorageXML_class(){
	static mastREG = true;
	if (mastREG||true) {
		mastREG = false;
		
		REG_CLASS(lvCStorage);
        
		REG_CLASS(lvCNodesMAP_ST);
		
		REG_CLASS(lvCGroupSmart);
		REG_CLASS(lvCGroupsMAP_ST);

		REG_CLASS(vvBASE);
		REG_CLASS(vvTEXT);
		REG_CLASS(vvPICTURE);
		REG_CLASS(POINT2D);
		REG_CLASS(vvPOINT2D);
		REG_CLASS(vvPOINT_SET);
		REG_CLASS(vvVector3D);
		REG_CLASS(vvDIALOG);
		REG_CLASS(CSingleMessage);
		REG_CLASS(CPhraseChunk);
		REG_CLASS(COneMissHint);
		REG_CLASS(vvMESSGES);
		REG_CLASS(vvMAP_ST);
		REG_CLASS(vvMissionLOG);
		REG_CLASS(vvMISSMGR);
		REG_CLASS(vvBrigAI);
        REG_CLASS(vvTASKS_CII);
		REG_CLASS(vvFuzzyRule);

		REG_CLASS(bbTEXT);
		REG_CLASS(vvTEXT_EX);

		REG_CLASS(bbObjInList);

		REG_CLASS(MissZoneBuild);
		REG_CLASS(MissSET);

		REG_CLASS(IIPara);

		REG_CLASS(OperationMesMgr);
		
		REG_CLASS(lvCEdge);
		
		REG_CLASS(lvCSquardShema);
		
		REG_CLASS(lvCMainScript);
		REG_CLASS(lvCFilm);
		REG_CLASS(ClassArray<lvCFilm>);

		REG_CLASS(lvCGraphObject);
		REG_CLASS(lvCDialogBased);
		REG_CLASS(lvCBlackScreen);
		REG_CLASS(lvCMoveGP);
		REG_CLASS(lvCAAppearGP);
		REG_CLASS(lvCAnimateGP);
		REG_CLASS(lvCDeffFilmMenu);
		REG_CLASS(lvCDeffAnimeFilmMenu);
		REG_CLASS(ClassArray<lvCGraphObject>);

		REG_CLASS(lvCTeraforming);
		
		REG_CLASS(lvCBattleShema_ST);

		REG_CLASS(lvCProcSquad);
		REG_CLASS(CFileList);
		REG_CLASS(CLUA_COORDINATOR_FOR_SH);
		REG_CLASS(lvCScriptHandler_ST);

		REG_CLASS(lvCBattleMapSaver);

		REG_CLASS(VectorPara);
		REG_CLASS(CameraPoss);

		REG_CLASS(CSkirmishTASK);

		RegClassBMST();
	};
};



















