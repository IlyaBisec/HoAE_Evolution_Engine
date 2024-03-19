#include "stdheader.h"
#include "unitability.h"
#include "BE_HEADERS.h"
#include "GameInterface.h"
#include "CurrentMapOptions.h"

bool isNotInvisible(OneObject* OB, byte SeekerNMask);
bool CheckPossibilityToAttackEnemy(OneObject* OB,OneObject* EOB);

be_OBJECT_SVITCHER	g_beOBJ_CONTROLER;

bool	g_beCheckChar(const char* lpszSTR){
	return ( (lpszSTR!=NULL) && (lpszSTR[0]!=0) );
};

// be_CVariableBase //////////////////////////////////////////////////////
be_CVariableBase::be_CVariableBase(){ 
	g_beOBJ_CONTROLER.SetState("be_CVariableBase"); 
};
be_CVariableBase::~be_CVariableBase(){ 
	g_beOBJ_CONTROLER.SetState("be_CVariableBase"); 
};
bool			be_CVariableBase::CheckIfObjectIsGlobal(){
	return true;
};
const	char*	be_CVariableBase::GetThisElementView(const char* LoaclName){
	if (LoaclName!=NULL){
		char* Presentation = GetGlobalBuffer();
		CE_WCP.GetPresentation(Presentation,this);
		if ( g_beCheckChar(Presentation) )	return Presentation;
	};
	return LoaclName;
};
DWORD	be_CVariableBase::GetClassMask(){
	BaseClass* pPAR = GetParent();
	if (pPAR!=NULL){
		if (g_beCheckChar(pPAR->GetClassName())) {
			_str parName = pPAR->GetClassName();
			if ( strstr(parName.pchar(),"ClassPtr.")!=NULL ){
				pPAR = pPAR->GetParent();
				if (pPAR!=NULL) {
					if (g_beCheckChar(pPAR->GetClassName())){
						parName = pPAR->GetClassName();
						if ( strcmp(parName.pchar(),"be_CUniveraslParam")==0 ){
							return 0x00000002;
						};
					};
				};
			};
		};
	};
	return 0x00000001;
};
void	be_CVariableBase::SetDefName(){
	_str l_ClassName = "";
	if ( GetClassName() ) l_ClassName = GetClassName();

	Name = "";
	if (g_beCheckChar(l_ClassName.pchar()))
		Name = l_ClassName;
    else 
		Name = "NOCLASSNAME";

	Name += " ";
	LARGE_INTEGER LI;
	QueryPerformanceCounter(&LI);
	Name += (int)LI.LowPart;
	/*
	if (GetClassMask()==0x00000002){
		SYSTEMTIME	stime;
		GetSystemTime( &stime );
		Name += " ";
		Name += stime.wYear;
		Name += ":";
		Name += stime.wMonth;
		Name += ":";
		Name += stime.wDay;
		Name += ":";
		Name += stime.wHour;
		Name += ":";
		Name += stime.wMinute;
		Name += ":";
		Name += stime.wSecond;
	};
	*/
};
bool	be_CVariableBase::SetName(const char* _Name){
	if ( g_beCheckChar(_Name) ){
		Name = _Name;
		return true;
	};
	return false;
};
void	be_CVariableBase::SetName_LUA(const char* _Name){
	if (SetName(_Name)==false)
		SetDefName();
};
char*	be_CVariableBase::GetName() const{
	return (char*)Name.cpchar();
};
bool	be_CVariableBase::GetValueAsSTR(_str& ValueSTR){
	ValueSTR = GetName();
	return true;
};
void	be_CVariableBase::SetRandomApplyPower(const int power){
	if (0<power&&power<=100)	RandomCoof=power;
	else						RandomCoof = 0;
};
int		be_CVariableBase::GetRandomApplyPower(){
	return RandomCoof;	
}
bool	be_CVariableBase::TryRandom(){
	if ( RandomCoof==0 || RandomCoof==100 )	return true;
	if ( (rando()%10000)<=RandomCoof*100 )	return true;
	return false;
};
// be_CARRAY /////////////////////////////////////////////////////////
be_CARRAY::be_CARRAY(){
	g_beOBJ_CONTROLER.SetState("be_CARRAY");
	SetDefName();	
};
be_CARRAY::be_CARRAY(const char* _Name){
	g_beOBJ_CONTROLER.SetState("be_CARRAY");
	if (SetName(_Name)==false) SetDefName();
};
be_CARRAY::~be_CARRAY(){
	g_beOBJ_CONTROLER.SetState("be_CARRAY");
};
void				be_CARRAY::AddValue(be_CVariableBase* val){
	_str ValName = ValuesArray.GetAmount()+1;
	_str ValType = val->GetClassName();
	ValuesArray.AddParam( ValType.pchar(), ValName.pchar(), false );
	ValuesArray.SetParamData( ValName.pchar(), val );
};
void				be_CARRAY::InsertValue(be_CVariableBase* val,int pos){
	_str ValName = "NEW_#";
		 ValName += ValuesArray.GetAmount()+1;
	_str ValType = val->GetClassName();

	bool	AllReadyPresent = ValuesArray.CheckParam(ValName.pchar());
	be_CBasePrametr* pPar = NULL;
	if ( AllReadyPresent==false ) {
		pPar = new be_CBasePrametr();
		pPar->RegisterMembers();
		ValuesArray.Insert(pos,pPar);
		pPar->SetParam( ValName.pchar(), ValType.pchar(), false);
		pPar=NULL;
	}else{
		pPar = ValuesArray.GetParamObj(ValName.pchar());
		pPar->UniveraslParam.SetLocal(false);
	}
	ValuesArray.SetParamData( ValName.pchar(), val );
};
be_CVariableBase*	be_CARRAY::GetValue(int pos){
	if ( pos<=0 || pos>ValuesArray.GetAmount() ) return NULL;
	be_CBasePrametr*	pPar = (be_CBasePrametr*)(ValuesArray[pos-1]);
	be_CVariableBase*	pVar = NULL;
	if (pPar!=NULL) {
		pPar->UniveraslParam.PrepareParam();
		pVar = (be_CVariableBase*)(pPar->UniveraslParam.GlobalReference.Get());
	};
	return pVar;
};
be_CVariableBase*	be_CARRAY::operator[](int index){
	return 	GetValue(index+1);
};
be_CVariableBase*	be_CARRAY::strGetValue(const char* pos){
	return ValuesArray.GetValue(pos);
};
void				be_CARRAY::DelValue(int pos){
	if ( pos<=0 || pos>ValuesArray.GetAmount() ) return;
	be_CBasePrametr*	pPar = (be_CBasePrametr*)(ValuesArray[pos-1]);
	if (pPar!=NULL&&g_beCheckChar(pPar->Name.pchar())) {
		ValuesArray.DelParam(pPar->Name.pchar());	
	};
};
int					be_CARRAY::GetAmount(){
	return ValuesArray.GetAmount();	
};
be_CParamList*		be_CARRAY::GetParmList(){
	return	&ValuesArray;	
};
void				be_CARRAY::vDraw(){
	if ( GetAmount()<=0 || (*this)[0]==NULL || strcmp(((*this)[0])->GetClassName(),"be_CNODE")!=0 ) return;
	be_CNODE* pND0 = NULL;
	be_CNODE* pND1 = NULL;
	be_CVariableBase* pBV = NULL;
	for ( int i=0; i<GetAmount()-1; i++ ){
		pBV = (*this)[i];
		if ( pBV!=NULL && strcmp(pBV->GetClassName(),"be_CNODE")==0 ){
			pND0 = (be_CNODE*)pBV;	
			pND0->vDraw();
		};
		pBV = (*this)[i+1];
		if ( pBV!=NULL && strcmp(pBV->GetClassName(),"be_CNODE")==0 ){
			pND1 = (be_CNODE*)pBV;	
			pND1->vDraw();
		};
		lvDrawArrow(pND0,pND1,100,150,EngSettings.MIS_SET.NodeLineColor);
		pND0=NULL;
		pND1=NULL;
	};
};
// be_CBOOL //////////////////////////////////////////////////////////////
be_CBOOL::be_CBOOL(){
	SetDefName();
	Value = false;
	g_beOBJ_CONTROLER.SetState("be_CBOOL");
};
be_CBOOL::be_CBOOL(const bool _Value){
	SetDefName();
	Value = _Value;
	g_beOBJ_CONTROLER.SetState("be_CBOOL");
};
be_CBOOL::be_CBOOL(const char* _Name){
	if (SetName(_Name)==false) SetDefName();
	Value = false;
	g_beOBJ_CONTROLER.SetState("be_CBOOL");
};
be_CBOOL::be_CBOOL(const char* _Name,const bool _Value){
	if (SetName(_Name)==false) SetDefName();
	Value = _Value;
	g_beOBJ_CONTROLER.SetState("be_CBOOL");
};
be_CBOOL::~be_CBOOL(){ 
	g_beOBJ_CONTROLER.SetState("be_CBOOL"); 
};
// be_CDWORD ////////////////////////////////////////////////////////////////
be_CDWORD::be_CDWORD(){
	SetDefName();
	Value = 0x00000000;
	g_beOBJ_CONTROLER.SetState("be_CDWORD");
};
be_CDWORD::be_CDWORD(const DWORD _Value){
	SetDefName();
	Value = _Value;
	g_beOBJ_CONTROLER.SetState("be_CDWORD");
};
be_CDWORD::be_CDWORD(const char* _Name){
	if (SetName(_Name)==false) SetDefName();
	Value = 0x00000000;
	g_beOBJ_CONTROLER.SetState("be_CDWORD");
};
be_CDWORD::be_CDWORD(const char* _Name,const DWORD _Value){
	if (SetName(_Name)==false) SetDefName();
	Value = _Value;
	g_beOBJ_CONTROLER.SetState("be_CDWORD");
};
be_CDWORD::~be_CDWORD(){ 
	g_beOBJ_CONTROLER.SetState("be_CDWORD"); 
};
// be_CINT /////////////////////////////////////////////////////////////
be_CINT::be_CINT(){
	SetDefName();
	Value = 0;
	g_beOBJ_CONTROLER.SetState("be_CINT");
};
be_CINT::be_CINT(const int _Value){
	SetDefName();
	Value = _Value;
	g_beOBJ_CONTROLER.SetState("be_CINT");
};
be_CINT::be_CINT(const char* _Name){
	if (SetName(_Name)==false) SetDefName();
	Value = 0;
	g_beOBJ_CONTROLER.SetState("be_CINT");
};
be_CINT::be_CINT(const char* _Name,const int _Value){
	if (SetName(_Name)==false) SetDefName();
	Value = _Value;
	g_beOBJ_CONTROLER.SetState("be_CINT");
};
be_CINT::~be_CINT(){ 
	g_beOBJ_CONTROLER.SetState("be_CINT"); 
};
// be_CFLOAT ///////////////////////////////////////////////////////////////
be_CFLOAT::be_CFLOAT(){
	SetDefName();
	Value = 0.0f;
	g_beOBJ_CONTROLER.SetState("be_CFLOAT");
};
be_CFLOAT::be_CFLOAT(const float _Value){
	SetDefName();
	Value = _Value;
	g_beOBJ_CONTROLER.SetState("be_CFLOAT");
};
be_CFLOAT::be_CFLOAT(const char* _Name){
	if (SetName(_Name)==false) SetDefName();
	Value = 0.0f;
	g_beOBJ_CONTROLER.SetState("be_CFLOAT");
};
be_CFLOAT::be_CFLOAT(const char* _Name,const float _Value){
	if (SetName(_Name)==false) SetDefName();
	Value = _Value;
	g_beOBJ_CONTROLER.SetState("be_CFLOAT");
};
be_CFLOAT::~be_CFLOAT(){ 
	g_beOBJ_CONTROLER.SetState("be_CFLOAT"); 
};
// be_CSTR ////////////////////////////////////////////////////////////////
be_CSTR::be_CSTR(){
	SetDefName();
	Value = "";
	g_beOBJ_CONTROLER.SetState("be_CSTR");
};
be_CSTR::be_CSTR(const char* _Name){
	if (SetName(_Name)==false) SetDefName();
	Value = "";
	g_beOBJ_CONTROLER.SetState("be_CSTR");
};
be_CSTR::be_CSTR(const char* _Name,const char* _Value){
	if (SetName(_Name)==false) SetDefName();
	Value = _Value;
	g_beOBJ_CONTROLER.SetState("be_CSTR");
};
be_CSTR::~be_CSTR(){
	g_beOBJ_CONTROLER.SetState("be_CSTR"); 
};
void	be_CSTR::SetValue(const char* val){ 
	if (g_beCheckChar(val)) Value = val; 
};
char*	be_CSTR::GetValue() const{ 
	return (char*)Value.cpchar(); 
};
// be_CSTR_DIALOG /////////////////////////////////////////////////////////
be_CSTR_DIALOG::be_CSTR_DIALOG(){
	TooLong=false;
	SetDefName();
	TextID = "";
	g_beOBJ_CONTROLER.SetState("be_CSTR_DIALOG");
	update=true;
};
be_CSTR_DIALOG::be_CSTR_DIALOG(const char* _Name){
	TooLong=false;
	if (SetName(_Name)==false) SetDefName();
	TextID = "";
	g_beOBJ_CONTROLER.SetState("be_CSTR_DIALOG");
	update=true;
};
be_CSTR_DIALOG::be_CSTR_DIALOG(const char* _Name,const char* _Value){
	TooLong=false;
	if (SetName(_Name)==false) SetDefName();
	TextID = _Value;
	g_beOBJ_CONTROLER.SetState("be_CSTR_DIALOG");
	update=true;
};
be_CSTR_DIALOG::~be_CSTR_DIALOG(){
	g_beOBJ_CONTROLER.SetState("be_CSTR_DIALOG");
};
void	be_CSTR_DIALOG::Update(){
	if (update)	{
		update=false;
		be_gConvertTextID(TextID,Speaker,Spirit,ogFile,Text);
		Test();
	};
};
void	be_CSTR_DIALOG::getStringForSave(_str& _string){
	_string  = TextID;
	_string += "    ";
	_string += "[";
	_string += Speaker;
	_string += "]";
	_string += "[";
	_string += Spirit;
	_string += "]";
	_string += "[";
	_string += ogFile;
	_string += "]";
	_string += Text;
};
bool	be_CSTR_DIALOG::Test(){
	TooLong	= g_beCloudTest.isTextTooLarge(g_beCloudTest.getCloudTypeNUM()-1,Text.pchar());
	return TooLong;
};
char*	be_CSTR_DIALOG::Get_ogFile()	const{
	return (char*)ogFile.cpchar();	
};
be_CGROUP*		be_CSTR_DIALOG::Get_Group()		const{
	return (be_CGROUP*)(((be_CSTR_DIALOG*)this)->Group.Get());
};
// be_CCAMERA /////////////////////////////////////////////////////////////
bool	be_CCAMERA::HideCameraTracks = false;
DWORD	be_CCAMERA::GetClassMask(){
	DWORD pMASK = be_CVariableBase::GetClassMask();
	if ( HideCameraTracks ) pMASK+=0x00000200;
	else					pMASK+=0x00000100;
	return pMASK;
};
be_CCAMERA::be_CCAMERA(){
	SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CCAMERA");
};
be_CCAMERA::be_CCAMERA(const char* _Name){
	if (SetName(_Name)==false) SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CCAMERA");
};
be_CCAMERA::~be_CCAMERA(){
	g_beOBJ_CONTROLER.SetState("be_CCAMERA");
};
// be_CGROUP //////////////////////////////////////////////////////////////
void	OneObject::AddInCGROUP		(DWORD	GID){
	int i = GRP_N;
	int freeID = -1;
	while (i--) {
		if ( pGRP_IDS[i]==GID ) return;
		if ( pGRP_IDS[i]==0xFFFF )	freeID=i;
	};
	if (freeID!=-1) {
		pGRP_IDS[freeID]=GID;
		return;
	};
	GRP_N++;
	DWORD* pNewList = new DWORD[GRP_N];
	for ( int i=0; i<(GRP_N-1); i++ ){
		pNewList[i] = pGRP_IDS[i];
	};
	pNewList[GRP_N-1] = GID;
	delete[] pGRP_IDS;
	pGRP_IDS = pNewList;
};
void	OneObject::RemFromCGROUP	(DWORD	GID){
	for ( int i=0; i<GRP_N; i++ ){
		if ( pGRP_IDS[i]==GID ){
			pGRP_IDS[i]=0xFFFF;
			return;
		};
	};
};
void CopyReIm(byte NI);
void MakeReformation(byte NI,word BrigadeID,byte FormType);
void AddUnitToImSelected(byte NI,OneObject* OB);
void AddUnitToSelected(byte NI,OneObject* OB);
DLLEXPORT	void SelChangeNation(byte SrcNat,byte DstNat);
DLLEXPORT	void ClearSelection(byte Nat);
#define GROUP_FONT		SmallWhiteFont1
#define pvGROUP_FONT	&SmallWhiteFont1
extern float fMapX;
extern float fMapY;
void	RemoveDublicateInList( LinearArray<int,_int>& _list ){
	int i=0; 
	while (i<_list.GetAmount()) {
		int N = _list.GetAmount();
		while ( (--N) > i ) {
			if (_list[N]==_list[i]) {
				_list.DelElement(N);
			};
		};
		i++;
	};
};
// be_CALINC //////////////////////////////////////////////////////////////
bool	be_CALINC::CheckLinc(){ 
	if ( Aid==0xFFFF || Group[Aid]==NULL || Group[Aid]->Serial!=Asn ){
		Aid = 0xFFFF;
		return false;
	};
	if ( Tid==0xFFFF || Group[Tid]==NULL || Group[Tid]->Serial!=Tsn ) {
		Tid = 0xFFFF;
		return false;
	};
	if ( CheckPossibilityToAttackEnemy(Group[Aid],Group[Tid])==false ){
		return false;
	}

	return true; 
};
void	be_CALINC::Fill1vs1Data(){
	if (CheckLinc()) {
		OneObject* pAttOB = Group[Aid];
		OneObject* pTrgOB = Group[Tid];
		// distPower
		distPower = (-1.f/50.f) * (float)Norma( (pAttOB->RealX-pTrgOB->RealX)/16, (pAttOB->RealY-pTrgOB->RealY)/16 );
		distPower = 10.f*pow( 2.f, (float)distPower );
		
		// prioPower
		prioPower = 0;
	};
};
be_CALINC::be_CALINC(){
	Aid=0xFFFF;
	Tid=0xFFFF;
	Asn=0xFFFF;		
	Tsn=0xFFFF;
	distPower=0;
	prioPower=0;
	AttackerNUM=0;
}
// be_CLINC_LIST //////////////////////////////////////////////////////////
be_CLINC_LIST::~be_CLINC_LIST(){ 
	curLincs.Clear(); 
	newLincs.Clear();  
};
void	be_CLINC_LIST::CreateCurList( int _N ){
	if ( _N<=0 ) { curSize=0; return; };
	if (curSize<_N){
		while (curSize<_N) {
			be_CALINC* pnewLinc = new be_CALINC();
			curLincs.Add(pnewLinc);
			pnewLinc = NULL;
			curSize++;
		};
	}else{
		curSize = _N;
	};
};
void	be_CLINC_LIST::FillCurList  ( be_CGROUP* pAttaker, bool fillPower/*=false*/ ){
	if ( pAttaker == NULL )	{ curSize=0; return; };
	int AttN = pAttaker->GetTotalAmount();
	if ( curSize!=AttN )	CreateCurList( AttN );
	OneObject* pAttOB = NULL;
	OneObject* pTarOB = NULL;
	for ( int i=0; i<curSize; i++ ){
		pAttOB = pAttaker->GetOneObj_lID(i);
		if (pAttOB) {
			curLincs[i]->Aid = pAttOB->Index;
			curLincs[i]->Asn = pAttOB->Serial;
			if ( pAttOB->Attack && pAttOB->EnemyID!=0xFFFF ){
				pTarOB = Group[pAttOB->EnemyID];
				if ( pTarOB && (!pTarOB->Sdoxlo||pTarOB->Hidden) && CheckPossibilityToAttackEnemy(pAttOB,pTarOB) ){
					curLincs[i]->Tid = pTarOB->Index;
					curLincs[i]->Tsn = pTarOB->Serial;

					if ( fillPower ) curLincs[i]->Fill1vs1Data();
				};
			};
		};
		pAttOB = NULL;
	};
};
void	be_CLINC_LIST::UpdateCur1vs1( be_CGROUP* pAttaker, be_CGROUP* pTarget ){
	OneObject* pAttOB = NULL;
	OneObject* pTrgOB = NULL;
	for ( int i=0; i<curSize; i++ ){
		if ( curLincs[i]->Aid!=0xFFFF ){
			pAttOB = Group[curLincs[i]->Aid];
			if ( pAttaker->_CheckGPT(pAttOB)==0xFFFF )	curLincs[i]->Aid = 0xFFFF;
		};
		
		if ( curLincs[i]->Tid!=0xFFFF ){
			pTrgOB = Group[curLincs[i]->Tid];
			if ( pTarget->_CheckGPT(pTrgOB)==0xFFFF )	curLincs[i]->Tid = 0xFFFF;
		};
		
		curLincs[i]->Fill1vs1Data();

		pAttOB = NULL;
		pTrgOB = NULL;
	};
};
void	be_CLINC_LIST::UpdateCurNvs1(){
	for (int i=0;i<curSize;i++) curLincs[i]->AttackerNUM=0;

	for (int i=0;i<curSize;i++){
		if (curLincs[i]->AttackerNUM==0){
			int iAN = GetNofCur( curLincs[i]->Tid );
			for (int j=0;j<curSize;j++){
				if ( curLincs[j]->Tid==curLincs[i]->Tid ) curLincs[i]->AttackerNUM=iAN;
			};
		};
	};
};
int		be_CLINC_LIST::GetNofCur(int _id){
	int retN = 0;
	for (int i=0;i<curSize;i++){
		if ( curLincs[i]->Tid==_id )	retN++;
	};
	return	retN;
};
// be_CGROUP //////////////////////////////////////////////////////////////
be_CGROUP::be_CGROUP() {
	SetSendDispersion(0);

	visible		= true; 
	ORDER		= vgNO_ORDERS; 

	NofNE		= 0;

	SetDefName();

	last_idAttackEnemy_ID=0;

	g_beOBJ_CONTROLER.SetState("be_CGROUP");
};
be_CGROUP::be_CGROUP(const char* name){
	SetSendDispersion(0);

	visible		= true; 
	ORDER		= vgNO_ORDERS; 

	NofNE		= 0; 

	if (name!=NULL) Name = name;
	else			SetDefName();

	last_idAttackEnemy_ID=0;

	g_beOBJ_CONTROLER.SetState("be_CGROUP");
};
be_CGROUP::~be_CGROUP(){
	Units.Clear();	
	DeadUnits.Clear();
	g_beOBJ_CONTROLER.SetState("be_CGROUP");
};
bool				be_CGROUP::GetValueAsSTR(_str& ValueSTR){
	ValueSTR = GetName();
	if ( strstr(ValueSTR.pchar(),"be_CGROUP")!=NULL ){
		ValueSTR  = "l_g";
		ValueSTR += GetObjectGlobalID();
	};
	return true;
};
en_vGroup_Order		be_CGROUP::GetORDER(){
	int N = GetTotalAmount();
	int inMOVE = 0;
	int inATTACK = 0;
	int inHAVE_SOME_ORDER = 0;
	int inBRIGADE_ORDER = 0;
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB) {
			if (pOB->Attack && pOB->EnemyID<0xFFFE)	inATTACK++;
			if (pOB->DestX>=0)						inMOVE++;
			if (pOB->LocalOrder!=NULL)				inHAVE_SOME_ORDER++;
			if (pOB->BrigadeID!=0xFFFF ){
				Brigade* BR=CITY[pOB->NNUM].Brigs+pOB->BrigadeID;
				if (BR && BR->BOrder!=NULL)	inBRIGADE_ORDER++;
			};
		};
	};
	if ( inATTACK			> 0 )	return vgATTACK;
	if ( inMOVE				> 0 )	return vgMOVE;
	if ( inHAVE_SOME_ORDER	> 0 )	return vgSOME_ORDER;
	if ( inBRIGADE_ORDER	> 0 )	return vgBRIG_ORDER;
	return vgNO_ORDERS;
};
en_vGroup_Order		be_CGROUP::GetUnitORDER(int uid){
	en_vGroup_Order	order=vgNO_ORDERS;
	int N = GetTotalAmount();
	if (0<=uid&&uid<N){
		OneObject* pOB=GetOneObj_lID(uid);
		if (pOB!=NULL) {
			if (pOB->Attack && pOB->EnemyID<0xFFFE)		order=vgATTACK;
			else if (pOB->DestX>=0 && pOB->LocalOrder)	order=vgMOVE;
			else if (pOB->LocalOrder!=NULL)				order=vgSOME_ORDER;
			if (pOB->BrigadeID!=0xFFFF ){
				Brigade* BR=CITY[pOB->NNUM].Brigs+pOB->BrigadeID;
				if (BR && BR->BOrder!=NULL)			order=vgBRIG_ORDER;
			};
		};
	};
	return	order;
};// Текущий приказ для юнита в локальном списке
bool				be_CGROUP::CheckUnitORDER(int uid,en_vGroup_Order Order){
	bool ret = false;
	int N = GetTotalAmount();
	if (0<=uid&&uid<N){
		OneObject* pOB=GetOneObj_lID(uid);
		if (pOB!=NULL) {
			DWORD l_Order = 0x00000000;
			if ( pOB->BrigadeID!=0xFFFF ){
				Brigade* BR=CITY[pOB->NNUM].Brigs+pOB->BrigadeID;
				if ( BR && BR->BOrder!=NULL ){
					l_Order += 0x00000001;	
				};
			};
			if( pOB->Attack && pOB->EnemyID<0xFFFE ) {
				l_Order += 0x00000002;
			};
			if ( pOB->DestX>=0 && pOB->LocalOrder ||  pOB->LocalOrder!=NULL && pOB->LocalOrder->OrderType==12 ) {
				l_Order += 0x00000004;
			};
			if ( pOB->LocalOrder!=NULL ) {
				l_Order += 0x00000008;
			};
			
			switch(Order) {
			case vgATTACK:
				ret = (bool)( l_Order & 0x00000002 );
				break;
			case vgMOVE:
				ret = (bool)( l_Order & 0x00000004 );
				break;
			case vgSOME_ORDER:
				ret = (bool)( l_Order & 0x00000008 );
				break;
			case vgBRIG_ORDER:
				ret = (bool)( l_Order & 0x00000001 );
				break;
			case vgNO_ORDERS:
				ret = !(bool)( l_Order & 0x0000000F );
				break;
			};
		};
	};
	return	ret;
};
int					be_CGROUP::GetORDER_NUM(en_vGroup_Order order){
	int NUM = 0;
	int N = GetTotalAmount();
	while (N--){
		if (CheckUnitORDER(N,order)) NUM++;
	};
	return NUM;
};
int					be_CGROUP::NO_ORDERS_NUM() const{
	return	((be_CGROUP*)(this))->GetORDER_NUM(vgNO_ORDERS);
};
int					be_CGROUP::MOVE_NUM() const{
	return	((be_CGROUP*)(this))->GetORDER_NUM(vgMOVE);
};
int					be_CGROUP::ATTACK_NUM() const{
	return	((be_CGROUP*)(this))->GetORDER_NUM(vgATTACK);
};
word				be_CGROUP::isInDeadList(OneObject* pOB){
	if (pOB!=NULL){
		int N = DeadUnits.GetAmount();
		while (N--) {
			if ( pOB->Index==DeadUnits[N].ID && pOB->Serial==DeadUnits[N].SN ){
				return	pOB->Index;
			};
		}
	};
	return 0xFFFF;
};
void				be_CGROUP::addDead(OneObject* pOB){
	if (pOB!=NULL && pOB->GetLiveState()!=ls_LiveBody ) {
		if (isInDeadList(pOB)==0xFFFF) {
			DeadUnits.Add( OneUS(pOB->Index,pOB->Serial) );
			NofNE++;
			addrand(NofNE);
		};
	};	
};
void				be_CGROUP::restoreResurrect(){
	int DUN = DeadUnits.GetAmount();
	OneObject* pOB = NULL;
	while (DUN--) {
		if ( 0<=DeadUnits[DUN].ID && DeadUnits[DUN].ID<0xFFFF ){
			pOB = Group[DeadUnits[DUN].ID];
			if ( pOB==NULL ){
				DeadUnits.DelElement( DUN );
			}else{
				if ( pOB->Serial!=DeadUnits[DUN].SN || pOB->GetLiveState()==ls_Erased ){
					DeadUnits.DelElement( DUN );
					pOB->RemFromCGROUP( GetObjectGlobalID() );
				}else if ( pOB->GetLiveState()==ls_LiveBody ) {
					DeadUnits.DelElement( DUN );
					AddUnitGPT(pOB);
					addname(pOB->Index);
				};
			};
		};
	};
};
int					be_CGROUP::getDeadUN(){
	return DeadUnits.GetAmount();	
};
void				be_CGROUP::RemoveDeadUnits(){
	int N = Units.GetAmount();
//	if (N==0)	return;
	while (N--)	{
		word ids = Units[N].ID; 
		if (ids<0||ids>=0xFFFF) {
			Units.DelElement(N);
			NofRE++;
			addrand(NofRE);
		}else{
			OneObject* pOB = Group[ids];
			//if (pOB==NULL || pOB->Serial!=Units[N].SN) {
			//	Units.DelElement(N);
			//	NofRE++;
			//	addrand(NofRE);
			//}else{
				if( pOB==NULL || pOB->GetLiveState()!=ls_LiveBody ){
					Units.DelElement(N);
					NofRE++;
					addrand(NofRE);
					addDead(pOB);
				};
			//};
		};
	};
	restoreResurrect();
};
void				be_CGROUP::ScreenOnGroup(){
	int	  gx=0,  gy=0;
	GetGroupCenter(gx,gy);
	if(gx != 0 || gy != 0) {
		void SetCameraPos(float, float);
		SetCameraPos((float)gx / 32.0f, (float)gy / 32.0f);
	}
/*	if (gx!=0||gy!=0) {
		float cx=0.f,cy=0.f;
		cx = ((float)gx/32.f)-(float)smaplx/2.f;
		cy = ((float)gy/32.f)-(float)smaply/2.f;
		if (abs(cx-fMapX)>=1.f/32.f || abs(cy-fMapY)>=1.f/32.f) {
			void SetCameraPos(float x,float y);
			SetCameraPos(cx,cy);
		};
	};*/
};

bool				be_CGROUP::IsGroupOnScreen(){
	int N = GetTotalAmount();

	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB){
			if ( IsOnScreen(pOB->RealX/16,pOB->RealY/16,150,150) )	return true;
		};
	};
	return false;
};
void				be_CGROUP::vDraw(bool forceDraw/*=false*/){
	if (visible==false)	return;
	if ( (!forceDraw) && (!IsGroupOnScreen()) )	return;
	DWORD color = EngSettings.MIS_SET.vGroupLineColor;
	int x,y;
	GetGroupCenter(x,y);
	static float uH = 50.f;		//+++
	static float tH = 150.f;	//+++
	static float uR = 15.f;		//+++
	static float dy = -10.f;	//+++
	if (x!=0||y!=0) {	
		int N = GetTotalAmount();
		OneObject* pOB = NULL;

		bool  onTOP = true;
		float tcX	= (float)GetMinX()-100.f;
		float tcY	= (float)GetMinY()-100.f;
		if (tcX-500.f<0)		tcX = (float)GetMaxX()+100.f;
		if (tcY-500.f<0)	  {	tcY = (float)GetMaxY()+100.f; onTOP = false; };
		Vector3D	p0(tcX,tcY,max((float)GetHeight(tcX,tcY),0.0f)+uH+tH);

		while (N--) {
			pOB = GetOneObj_lID(N);
			if (pOB!=NULL) {
				float x0 = (float)(pOB->RealX>>4);
				float y0 = (float)(pOB->RealY>>4)+dy;
				//Shapes_MAP(20.f)->regular_polygon_6D
				Vector3D	pNn[6];
				for (int i=0; i<6; i++){
					pNn[i].set(	Shapes_MAP(uR)->regular_polygon_6D[i]->x+x0,
								Shapes_MAP(uR)->regular_polygon_6D[i]->y+y0,
								max((float)GetHeight(	(int)Shapes_MAP(uR)->regular_polygon_6D[i]->x+x0,
														(int)Shapes_MAP(uR)->regular_polygon_6D[i]->y+y0	
													),
									0.0f
								   )
							  );
				};
				for (int i=0; i<6; i++) pNn[i]=SkewPt(pNn[i].x,pNn[i].y,pNn[i].z);
				GPS.DrawLine(pNn[5],pNn[0],color);
				GPS.DrawLine(pNn[0],pNn[1],color);
				GPS.DrawLine(pNn[1],pNn[2],color);
				GPS.DrawLine(pNn[2],pNn[3],color);
				GPS.DrawLine(pNn[3],pNn[4],color);

				Vector3D	pHP(pNn[0].x,pNn[0].y,pNn[0].z+uH);
				GPS.DrawLine(pNn[0],pHP,color);

				GPS.DrawLine(p0,pHP,color);
			};
		};
		Vector3D	pT;
		if (onTOP) {
			pT.set(p0.x,p0.y+25,p0.z+25.f);
		}else{
			pT.set(p0.x,p0.y-25,p0.z-25.f);
		};
		GPS.DrawLine(p0,pT,color);

		Vector3D	pL(pT.x-50.f,pT.y,pT.z);
		Vector3D	pR(pT.x+50.f,pT.y,pT.z);
		GPS.DrawLine(pL,pR,color);

		Vector4D	pSTR;
		if (onTOP) {
			pSTR.set(pL.x+10.f,pL.y/*-260.f*/-20.f,pL.z,1);
		}else{
			pSTR.set(pL.x+10.f,pL.y/*-260.f*/+20.f,pL.z,1);
		};
		WorldToScreenSpace(pSTR);

		char titleSTR[256];
		char color[56];
		sprintf(color,"%s%x%s","{C 0x",EngSettings.MIS_SET.vGroupNameColor,"}");
		sprintf(titleSTR,"%s%s%s%d%s",color,Name.pchar()," [",GetTotalAmount(),"]");
		void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
		// visual coorection
		if (onTOP) 	pSTR.y -= 5;
		else		pSTR.y += 5;
		////////////////////
		ShowStringEx(pSTR.x,pSTR.y,titleSTR,pvGROUP_FONT);	
	};
};
word				be_CGROUP::_CheckGID(word id){
	word	present = 0xFFFF;
	int N = GetTotalAmount();
	while (present==0xFFFF&&N--) {
		if (Units[N].ID==id) {
			present = N;
		};
	};
	return present;
};
word				be_CGROUP::_CheckGPT(OneObject* pOB){
	word present = 0xFFFF;
	if (pOB!=NULL){
		present = _CheckGID(pOB->Index);
		if (present!=0xFFFF) {
			if (Units[present].SN!=pOB->Serial) {
				Units.DelElement(present);
				NofRE++;
				addrand(NofRE);
				present=0xFFFF;
			};
		};
	};
	return present;
};
OneObject*			be_CGROUP::GetOneObj_gID(word gID){
	OneObject* pOB = NULL;
	int N = GetTotalAmount();
	while ( pOB==NULL && N-- ) {
		if (Units[N].ID==gID)	pOB = Group[gID];
	};
	if ( (pOB && (!pOB->Sdoxlo||pOB->Hidden) && pOB->Serial==Units[N].SN) == false ) pOB = NULL;
	return pOB;
};
OneObject*			be_CGROUP::GetOneObj_lID(word lID){
	OneObject* pOB = NULL;
	int N = GetTotalAmount();
	if (0<=lID&&lID<N&&Units[lID].ID!=0xFFFF){
		pOB = Group[Units[lID].ID];
		if ( (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&pOB->Serial==Units[lID].SN) == false ) pOB = NULL;
	};
	return pOB;
};
void				be_CGROUP::UpdateOB_GID_inGROUP(){
	int N = GetTotalAmount();
	if ( N<=0 ) return;
	OneObject* pOB = NULL;
	DWORD	GID = GetObjectGlobalID();
	while (N--) {
		pOB = (*this)[N];
		if (pOB!=NULL) pOB->AddInCGROUP( GID );
	};
};
bool				be_CGROUP::AddUnitGID(word id){
	if (id<0||id>=0xFFFF)	return false;
	return	AddUnitGPT(Group[id]);
};
bool				be_CGROUP::AddUnitGPT(OneObject* pOB){
	if (pOB!=NULL && (!pOB->Sdoxlo||pOB->Hidden)) {
		if (_CheckGPT(pOB)==0xFFFF) {
			Units.Add( OneUS(pOB->Index,pOB->Serial) );
			pOB->AddInCGROUP( GetObjectGlobalID() );
			NofNE++;
			return true;
		};
	};
	return false;
};
int					be_CGROUP::AddSelectedUnits(byte NI/*=0xFF*/){
	int NU = 0;
	byte curNI = MyNation;
	if (NI!=0xFF)	curNI=NI;
	int selN = NSL[curNI];
	if (selN==0)	return NU;
	word* pSelIDS = Selm[curNI];
	addrand(curNI);
	for (int i=0; i<selN; i++){
		if (_CheckGID(pSelIDS[i])==0xFFFF){
			AddUnitGID(pSelIDS[i]);
			NU++;
		};
	};

	// Add comandirs to group if "COSSACS II"
	if (EngSettings.GameName==1){
		LinearArray<int,_int> BrigList;
		GetBrigadeList(true,&BrigList);
		int NB=BrigList.GetAmount();
		Brigade* pBR=NULL;
		int Nat=GetNation();

		for (int i=0; i<NB; i++){
			pBR=&CITY[Nat].Brigs[ BrigList[i] ];
			if (pBR!=NULL&&pBR->NMemb>=2) {
				if (pBR->Memb[0]!=0xFFFF){ AddUnitGID(pBR->Memb[0]); NU++; }
				if (pBR->Memb[1]!=0xFFFF){ AddUnitGID(pBR->Memb[1]); NU++; }
				if (pBR->Memb[2]!=0xFFFF){ AddUnitGID(pBR->Memb[2]); NU++; }
			};
		};
	};
	return NU;
};
int					be_CGROUP::RemSelectedUnits(byte NI/*=0xFF*/){
	int NU = 0;
	byte curNI = MyNation;
	if (NI!=0xFF)	curNI=NI;
	int selN = NSL[curNI];
	if (selN==0)	return NU;
	word* pSelIDS = Selm[curNI];
	addrand(curNI);
	for (int i=0; i<selN; i++){
		if (_CheckGID(pSelIDS[i])!=0xFFFF){
			RemUnitGID(pSelIDS[i]);
			NU++;
		};
	};
	return NU;
};
void 				be_CGROUP::AddUnitsInZone(byte NI,int x,int y,int R,bool _add){
	LinearArray<int,_int>	UID;
	itr_UnitsInRadius.Create(x,y,R);
	OneObject* pOB = NULL;
	while(pOB=itr_UnitsInRadius.Next()){
		if (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&pOB->NNUM==NI){
			AddUnitGPT(pOB);
			addname(pOB->Index);
			UID.Add(pOB->Index);
		};
		pOB = NULL;
	};		
	if ( _add==false ){
		int N = GetTotalAmount();
		while (N--) {
			pOB = GetOneObj_lID(N);
			if (pOB) {
				bool remove=true;
				int n=UID.GetAmount();
				while (remove&&n--) {
					if ( pOB->Index == UID[n] )	remove=false;
				};
				if (remove){
					RemUnitGID(pOB->Index);
					NofRE++;
					addrand(NofRE);
				};
			};
		};
	};
};// Add units nation-NI from current location to group
void				be_CGROUP::AddUnitsTypeInZone(byte NI,int UnitType,int x,int y,int R,bool _add){
	LinearArray<int,_int>	UID;
	itr_UnitsInRadius.Create(x,y,R);
	OneObject* pOB = NULL;
	while(pOB=itr_UnitsInRadius.Next()){
		if (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&pOB->NNUM==NI&&pOB->NIndex==UnitType){
			AddUnitGPT(pOB);
			addname(pOB->Index);
			UID.Add(pOB->Index);
		};
		pOB = NULL;
	};		
	if ( _add==false ){
		int N = GetTotalAmount();
		while (N--) {
			pOB = GetOneObj_lID(N);
			if (pOB) {
				bool remove=true;
				int n=UID.GetAmount();
				while (remove&&n--) {
					if ( pOB->Index == UID[n] )	remove=false;
				};
				if (remove){
					RemUnitGID(pOB->Index);
					NofRE++;
					addrand(NofRE);
				};
			};
		};
	};
};
void				be_CGROUP::AddEnemyUnitsInZone(int Nat,int x,int y,int R,bool _add){
	LinearArray<int,_int>	UID;
	// Add units
	addrand(x);
	addrand(y);
	addrand(R);
	addrand(Nat);
	itr_UnitsInRadius.Create(x,y,R);
	OneObject* pOB = NULL;
	while(pOB=itr_UnitsInRadius.Next()){
		addname(pOB->Index);
		if (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&(pOB->NMask&NATIONS[Nat].NMask)==0){
			if (isNotInvisible(pOB,NATIONS[Nat].NMask)){
				// by vital
				if(!pOB->UnlimitedMotion){
					AddUnitGPT(pOB);
					addname(pOB->Index);
					UID.Add(pOB->Index);
				}
			};
		};
		pOB = NULL;
	};	

	// hack begin
	//if(Nat!=7) иначе крипы не убивают сдания
	// hack end

	// if no units - try add buildings
	if ( UID.GetAmount()<=0 ){
		itr_GetBuildingsInRadius.Create(x,y,R);
		while(pOB=itr_GetBuildingsInRadius.Next()){
			if( pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&(pOB->NMask&NATIONS[Nat].NMask)==0 ){
				// hack begin
				if( !NATIONS[pOB->NNUM].AI_Enabled )
				// hack end
				if (isNotInvisible(pOB,NATIONS[Nat].NMask)){
					AddUnitGPT(pOB);
					addname(pOB->Index);
					UID.Add(pOB->Index);
				};
			};
			pOB = NULL;
		};		
	};
	// try add to existing units in group
	if ( _add==false ){
		int N = GetTotalAmount();
		while (N--) {
			pOB = GetOneObj_lID(N);
			if (pOB) {
				bool remove=true;
				int n=UID.GetAmount();
				while (remove&&n--) {
					if ( pOB->Index == UID[n] )	remove=false;
				};
				if (remove){
					RemUnitGID(pOB->Index);
					NofRE++;
					addrand(NofRE);
				};
			};
		};
	};
};
void				be_CGROUP::AddBrigad(int BrigID, int Nat){
	if (0<=BrigID&&BrigID<MaxBrig-11) {
		if (CITY[Nat].Brigs[BrigID].Enabled){
			for (int i=0; i<CITY[Nat].Brigs[BrigID].NMemb; i++){
				AddUnitGID(CITY[Nat].Brigs[BrigID].Memb[i]);
				addrand(CITY[Nat].Brigs[BrigID].Memb[i]);
			};
		};
	};
};
void				be_CGROUP::AddGObjectsInArea(int nation,int Layer,int AreaID,bool bulidings,bool add){
	pUnitsList UL;
	collect_option FLAG = ( (bulidings) ? (COLLECT_BUILDINGS) : (COLLECT_UNITS) );
	UL.CreateFromAreaID(Layer,AreaID,FLAG,add);
	//ForEachUnit( UL,  
	//	if ( Group[unit.ID] && Group[unit.ID]->GRP_N==0 && unit.GetNation()==nation ){
	//		AddUnitGID(unit.ID);
	//	};
	//);
	{
		UL.ToBegin();
		pUnit unit;
		while(UL.Next(unit)){
			if ( Group[unit.ID] && Group[unit.ID]->GRP_N==0 && unit.GetNation()==nation ){
				AddUnitGID(unit.ID);
			};
		};
	};
};
void				be_CGROUP::AddUnitsFromGroup(be_CGROUP* pGR)
{
	if (pGR==NULL)	return;
	int addN = pGR->GetTotalAmount();
	OneObject* pOB=NULL;
	while(addN--){
		pOB = pGR->GetOneObj_lID(addN);
		if (pOB!=NULL) {
			int ids = _CheckGID(pOB->Index);
			if (ids==0xFFFF || ( ids!=0xFFFF && Units[ids].SN!=pGR->Units[addN].SN )) {
				AddUnitGID(pOB->Index);
			};
		};
	};
} // be_CGROUP::AddUnitsFromGroup
void				be_CGROUP::RemUnitGID(word id){
	word a_id = _CheckGID(id);
	if (a_id!=0xFFFF) {
		RemUnitID(a_id);
	};
};
void				be_CGROUP::RemUnitGPT(OneObject* pOB){
	word a_id = _CheckGPT(pOB);
	if (a_id!=0xFFFF) {
		RemUnitID(a_id);
	};
};
void				be_CGROUP::RemUnitID(word id){
	if (0<=id&&id<Units.GetAmount()) {
		if ( (*this)[id]!=NULL ){
			(*this)[id]->RemFromCGROUP( GetObjectGlobalID() );
			Units.DelElement(id);
			NofRE++;
			addrand(NofRE);
		};
	};
};
void				be_CGROUP::RemAllUnits(){
	Units.Clear();
	DeadUnits.Clear();
	NofRE=0;
	addrand(NofRE);
	NofNE=0;
	addrand(NofNE);
};
int					be_CGROUP::RemUnitsByNation(int NI){
	int NU = 0;
	int N = GetTotalAmount();
	if (N==0) return NU;
	OneObject* pOB=NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL&&pOB->NNUM) {
			Units.DelElement(N);			
			NofRE++;
			NU++;
		};
	};
	return NU;
};
int					be_CGROUP::RemoveNUnitsToCGroup(be_CGROUP* pGR,int N/*=0*/){
	if (pGR==NULL)	return	0;
	int remN = GetTotalAmount();
	if (N>0) remN = min(remN,N);
	int retVAL = 0;
	OneObject* pOB=NULL;
	while(remN--){
		pOB = GetOneObj_lID(remN);
		if (pOB!=NULL) {
			int ids = pGR->_CheckGID(pOB->Index);
			if (ids==0xFFFF || ( ids!=0xFFFF && pGR->Units[ids].SN!=Units[remN].SN )) {
				pGR->AddUnitGID(pOB->Index);
				addrand(pOB->Index);
				retVAL++;
			};
		};
		Units.DelElement(remN);
		NofRE++;
	};
	return retVAL;
};
int					be_CGROUP::RemoveNReadyUnitsToCGroup(be_CGROUP* pGR,int N/*=-1*/){
	if (pGR==NULL)	return	0;
	int remN = GetTotalAmount();
	if (N!=-1)	remN = min(remN,N);
	int retVAL = 0;
	OneObject* pOB=NULL;
	bool remove = false;
	while(remN--){
		pOB = GetOneObj_lID(remN);
		remove = false;
		if (pOB!=NULL&&pOB->UnlimitedMotion==false) {
			int ids = pGR->_CheckGID(pOB->Index);
			if (ids==0xFFFF || ( ids!=0xFFFF && pGR->Units[ids].SN!=Units[remN].SN )) {
				pGR->AddUnitGID(pOB->Index);
				addrand(pOB->Index);
				retVAL++;
				remove = true;
			};
		};
		if ( pOB==NULL || remove ){
			Units.DelElement(remN);
			NofRE++;
		};
	};
	return retVAL;
};
int					be_CGROUP::RemoveUnitsInZoneToCGroup(be_CNODE* pND,be_CGROUP* pGR){
	int N = GetTotalAmount();
	if ( N==0 ) return 0;
	int retN = 0;
	OneObject* pOB = NULL;
	while (N--) {
		if ( pOB = (*this)[N]){
			if ( Norma( pOB->RealX/16-pND->x, pOB->RealY/16-pND->y )<=pND->R ){
				// Canditate for remove
				int ids = pGR->_CheckGID(pOB->Index);
				if (ids==0xFFFF || ( ids!=0xFFFF && pGR->Units[ids].SN!=Units[N].SN )) {
					pGR->AddUnitGID(pOB->Index);
					addrand(pOB->Index);
					retN++;
				};
				Units.DelElement(N);
				NofRE++;
				addrand(NofRE);
			};
		};
	};
	return retN;
};
int					be_CGROUP::RemoveInvisibleForNMask(int _NMask){
	int remN = 0;
	int N = GetTotalAmount();
	if ( N<=0 ) return remN;
    OneObject* pOB = NULL;
	while (N--) {
		if ( pOB = (*this)[N] ){
			if ( isNotInvisible(pOB,_NMask)==false ){
				RemUnitGPT(pOB);
				remN++;
			};
		};
	};
	return remN;	
};
int					be_CGROUP::KillUnits(int N/*=-1*/){
	int killN = GetTotalAmount();
	if (N!=-1)	killN = min(killN,N);
	int retVAL = 0;
	OneObject* pOB = NULL;
	while (killN--) {
		pOB = GetOneObj_lID(killN);
		if(pOB!=NULL){
			void DestructBuilding(OneObject* OB);
			DestructBuilding(pOB);
			RemUnitID(killN);
			retVAL++;
		};
		pOB = NULL;
	};
	return retVAL;
};

int					be_CGROUP::EraseUnits(int N/*=-1*/){
	int killN = GetTotalAmount();
	if (N!=-1)	killN = min(killN,N);
	int retVAL = 0;
	OneObject* pOB = NULL;
	while (killN--) {
		pOB=Group[ Units[killN].ID ];
		if( pOB ){
			pOB->delay=6000;
			if(pOB->BrigadeID!=0xFFFF){
				Brigade* BR=CITY[pOB->NNUM].Brigs+pOB->BrigadeID;
				BR->vLastEnemyID=0xFFFE;
			}
			if(pOB->LockType==1)pOB->RealDir=32;
			pOB->Die(true);
			pOB=Group[Units[killN].ID];
			if(pOB){				
				pOB->Sdoxlo=2500;
				if(pOB->NewBuilding){
					void EliminateBuilding(OneObject* OB);
					EliminateBuilding(pOB);
					Group[ Units[killN].ID ]=NULL;
					RemUnitID(killN);
					retVAL++;
				};
			};
		};
		pOB = NULL;
	};
	return retVAL;
};
void				be_CGROUP::SetNation(byte NI){
	pGroup GR(this);
	ForEachUnit( GR, unit.SetNation(NI); );
/*
	be_CGROUP* pOldSel = new be_CGROUP();
	pOldSel->AddSelectedUnits(GetNation());
	SelectUnits(0);
	SelChangeNation(GetNation(),NI);
	pOldSel->SelectUnits(0);
	delete pOldSel;
*/
};
void				be_CGROUP::SelectUnits(bool add/*=0*/){
	addrand(add);
	if (!add) UnSelect();
	byte NI = GetNation();
	addrand(NI);
	if (0<=NI&&NI<7) {
		int N = GetTotalAmount();
		if ( N==0 ) return;
	//	int addTRUE = ( (RandomCoof==0||RandomCoof==100) ? (1) : ( 100/RandomCoof ) );
		OneObject* pOB=NULL;
		while (N--) {
			pOB = GetOneObj_lID(N);
			if(pOB&&pOB->NNUM==NI){
			//	if ( N%addTRUE==0 )
				if (TryRandom()==true){
					AddUnitToSelected(NI,pOB);
					AddUnitToImSelected(NI,pOB);
				};
			};
		};
		CopyReIm(NI);
	};
};
void				be_CGROUP::SelectUnitsInZone(int x, int y, int R,bool add/*=false*/){
	if (!add) UnSelect();
	byte NI = GetNation();
	if (0<=NI&&NI<7) {
		int N = GetTotalAmount();
		OneObject* pOB=NULL;
		while (N--) {
			pOB = GetOneObj_lID(N);
			if(pOB&&pOB->NNUM==NI){
				if (Norma((pOB->RealX>>4)-x,(pOB->RealY>>4)-y)<=R) {
					AddUnitToSelected(NI,pOB);
					AddUnitToImSelected(NI,pOB);
				};
			};
		};
		CopyReIm(NI);
	};
};
void				be_CGROUP::UnSelect(){
	byte NI = GetNation();
	if (0<=NI&&NI<7) {
		ClearSelection(NI);
		addrand(311);
	};
};
void				be_CGROUP::SendTo(int x, int y, int dir, int type/*=0*/){
	pGroup GR(this);
	GR.SendGroupTo(x,y,dir,false,type);
/*
	addrand(2221);
	if (GetTotalAmount()==0)	return;
	int natGRP = GetNation();
	if (natGRP<0||natGRP>7) return;
	be_CGROUP* pOldSel = new be_CGROUP();
	pOldSel->AddSelectedUnits(natGRP);
	ClearSelection(natGRP);
	addrand(1117);
	SelectUnits(0);
	void SendSelectedToXY(byte NI,int xx,int yy,short Dir,byte Prio,byte Type);
	SendSelectedToXY(natGRP,x<<4,y<<4,dir,16,type);
	ClearSelection(natGRP);
	addrand(3118);
	pOldSel->SelectUnits(0);
	delete pOldSel;
*/
};
void				be_CGROUP::SendToI(int x,int y,int dir,int type,int i,bool withAttack){
	OneObject* pOB = GetOneObj_lID(i);
	pUnit U=pOB;
	if( U.Valid() ){
		//pOB->NewMonsterSendTo(x*16,y*16,( (withAttack) ? (128) : (128+16) ),type);
		U.SendUnitFarTo(x,y,0,0,(withAttack)?128:128+16,type);
		U.RotateUnit(dir,ot_afterall);
		addname(pOB->Index);
		addrand(x);
		addrand(y);
	};
};
void				be_CGROUP::SendToDispersePosition(int x,int y){
	int N = GetTotalAmount();
	if ( N<=0 )	return;
	while (N--) {
		idSendTo(x,y,512,255,N);
	};
};
void				be_CGROUP::MarkTime(int _x,int _y,int _destD,int _powerRND){
	addrand(_x);
	addrand(_y);
    int N = GetTotalAmount();
	if ( N<=0 )	return;
	int oldD	= SendDispersion;
	int oldRND	= RandomCoof;
	SendDispersion	= _destD;
	RandomCoof		= _powerRND;
	if ( _x==0 && _y==0 ) GetGroupCenter(_x,_y);
	OneObject* pOB = NULL;
	en_vGroup_Order	OB_ORD = vgNO_ORDERS;
	while (N--) {
		if (TryRandom()){
			pOB	= (*this)[N];
			if ( pOB!=NULL ) {
				OB_ORD	= GetUnitORDER(N);
				if ( OB_ORD==vgNO_ORDERS ){
					SetOB_useDispersePosition(_x,_y,pOB,128+0,0,true);		
				};
			};
		};
	};
	SendDispersion	= oldD;
	RandomCoof		= oldRND;
};
void				be_CGROUP::SendSmartlyTo(int x, int y, int stepsN,be_CNODE* pDestNode){
	OneObject* pOB = (*this)[0];
	if ( pOB ){
		int cx=0,cy=0;
		GetGroupCenter(cx,cy);
		cx /= 64; cy /= 64;
		int start	= HashTable[pOB->LockType].GetTopAreaIndex( cx, cy );
		int finish	= HashTable[pOB->LockType].GetTopAreaIndex( x, y );
		int maxDanger = 0;
		g_beFillFULLWAY_ATTACK(start,finish,GetNation(),maxDanger,stepsN,pOB->LockType);
		SetSendDispersion( 128 + (GetTotalAmount()/50)*64 );
		Area* pArea = NULL;
		for ( int i=1; i<NWPTS; i++ ){
	        pArea = HashTable[pOB->LockType].GetTopArea(FULLWAY[i]);
			/*
			if (i==1){
				SendTo(pArea->x*64+32,pArea->y*64,512);
			}else{	 
				SendTo(pArea->x*64+32,pArea->y*64,512,2);
			};
			*/
			for ( int ii=0; ii<GetTotalAmount(); ii++ ){
				pOB = (*this)[ii];
				if ( pOB!=NULL ){
					addrand(pArea->x);
					addrand(pArea->y);
					if (i==1){
						addrand(2);
						SetOB_useDispersePosition(pArea->x*64+32,pArea->y*64,pOB,128+0,0,true);
					}else{	 
						addrand(3);
						SetOB_useDispersePosition(pArea->x*64+32,pArea->y*64,pOB,128+0,2,true);
					};
				};
			};
			
			if ( i==NWPTS-1 && pDestNode!=NULL ){
				pDestNode->vSetXY( pArea->x*64+32,pArea->y*64 );
			}
		};
	};
	return;	
};
void				be_CGROUP::ChangeDirection(int dir,int type/*=0*/){
	int x = 0;
	int y = 0;
	GetGroupCenter(x,y);
	SendTo(x,y,dir,type);
};
bool				be_CGROUP::SetAgresiveST(int stID){
	if (stID<0||stID>2) return false;
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	int trueN = 0;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB) {
			if (pOB->ActivityState!=stID){
				trueN++;
				pOB->ActivityState=stID;
				pOB->GroundState=pOB->ActivityState==2;
				if(pOB->NewState!=pOB->GroundState)pOB->NewState=pOB->GroundState;
			};
		};
	};	
	if (trueN==0)	return false;
	if(pOB&&pOB->BrigadeID!=0xFFFF){
		if(EngSettings.AutoChangeFormationType){
			Brigade* BR=CITY[pOB->NNUM].Brigs+pOB->BrigadeID;	
			OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
			if(ODE->GroupID!=0xFF){
				SingleGroup* FGD=FormGrp.Grp+ODE->GroupID;
				char ids[]={1,0,2};
				MakeReformation(pOB->NNUM,pOB->BrigadeID,FGD->IDX[ids[stID]]);
			};
		};
	};
	return true;
};
void				be_CGROUP::ChengeFormation(int formID){
	//	RemoveDeadUnits();
	int N = GetTotalAmount();

	if (EngSettings.GameName==0){
		for(int i=0;i<N;i++){
			word MID=Units[i].ID;
			if(MID!=0xFFFF){
				OneObject* OB=GetOneObj_gID(MID);
				if (OB&&0<=OB->BrigadeID&&OB->BrigadeID<=0xFFFF) {
					// For Alexander
					word fID=formID;
					if(EngSettings.AutoChangeFormationType){
						if(formID==0) fID=0xFD; else 
							if(formID==1) fID=0xFE; else 
								if(formID==2) fID=0xFF;
					};
					MakeReformation(OB->NNUM,OB->BrigadeID,fID);
				};				
				break;
			};
		};
	};
	// For Cossacs II
	if (EngSettings.GameName==1){
		LinearArray<int,_int> BrigList;
		GetBrigadeList(false,&BrigList);
		int NBR=BrigList.GetAmount();
		if (NBR==0)	return;
		Brigade* pBR=NULL;
		word fID_C=-1;
		int NNUM=GetNation();

		for (int i=0; i<NBR; i++){
			pBR=&CITY[NNUM].Brigs[ BrigList[i] ];
			if (pBR!=NULL&&pBR->Enabled==true){
				OrderDescription* ODE=ElementaryOrders+pBR->WarType-1;
				if(ODE->GroupID!=0xFF){
					SingleGroup* FGD=FormGrp.Grp+ODE->GroupID;
					if (formID<FGD->NCommon){
						fID_C=FGD->IDX[formID];
					};
				};
				if (fID_C!=-1) MakeReformation(NNUM,BrigList[i],fID_C);
			};
			pBR=NULL;
		};
	};
};
void				be_CGROUP::TakeRess(int RessID){
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL) {
			pOB->TakeResource(pOB->RealX>>4,pOB->RealY>>4,RessID,128,0);
			pOB = NULL;
		};
	};
};
void				be_CGROUP::TakeFood(){
	// FOOD == 3
	TakeRess(3);
};
void				be_CGROUP::TakeWood(){
	// WOOD == 0
	TakeRess(0);
};
void				be_CGROUP::TakeStone(){
	// STOWN == 2
	TakeRess(2);
};
void				be_CGROUP::ClearOrders(){
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL) {
			pOB->ClearOrders();
		};
	};
	LinearArray<int,_int> BrigList;
	GetBrigadeList(false,&BrigList);
	int BRN = BrigList.GetAmount();
	if (BRN>0) {
		Brigade* pBR=NULL;
		int Nat=GetNation();
		for (int i=0; i<BRN; i++){
			pBR=&CITY[Nat].Brigs[ BrigList[i] ];
			if (pBR->Enabled){
				pBR->ClearBOrders();
				pBR->ClearNewBOrders();
			};
		};
	};
};
void				be_CGROUP::SetInStandGround(){
	LinearArray<int,_int> BrigList;
	GetBrigadeList(false,&BrigList);
	int N = BrigList.GetAmount();
	if (N>0) {
		int Nat=GetNation();
		// From vui_IS.cpp [ void vui_IS_BrigHoldPosit::SynchroAction(); ]
		for (int i=0; i<N; i++){
			Brigade* BR=&(CITY[Nat].Brigs[BrigList[i]]);
			if(BR->Enabled&&BR->WarType){
				bool sel=false;
				for(int bp=0;bp<BR->NMemb;bp++){
					word ID=BR->Memb[bp];
					if(ID!=0xFFFF){
						OneObject* OB=Group[ID];
						if(OB&&OB->Serial==BR->MembSN[bp]&&!OB->Sdoxlo&&OB->Selected){
							sel=true;
						};
					};
				};

				if(sel){
					BR->ClearBOrders();
					BR->BrigDelay=0;
					void MakeStandGround(Brigade* BR);
					if(!BR->BrigDelay) MakeStandGround(BR);
				};
			};
		};
		//////////////////////////////////////////////////////////////////
	};
};// Set all brigades in SG
void				be_CGROUP::SetStandGround(bool state){
	int N = GetTotalAmount();
	if ( N<=0 ) return;
	OneObject* pOB = NULL;
	while (N--) {
		pOB = (*this)[N];
		if ( pOB ){
			pOB->StandGround = state;
		};
	};
};
void				be_CGROUP::SetHP(const int HP){
	int NU = GetTotalAmount();
	if (NU==0) return;
	OneObject* pU = NULL;
	while (NU--) {
		pU = GetOneObj_lID(NU);
		if (pU&&TryRandom()) {
			pU->Life = HP;
		};
		pU = NULL;
	};	
};// Set units curent value of hit points
void				be_CGROUP::SetLockType(int LockTypeID){
	if ( LockTypeID<0 || LockTypeID>=4 ) return;
	int N = GetTotalAmount();
	if ( N<=0 ) return;
	OneObject* pOB = NULL;
	while (N--) {
		pOB = (*this)[N];
		if (pOB) pOB->LockType = LockTypeID;
	};
};// 0-lend,1-sea,2-low fly,3-hight fly
int					be_CGROUP::GetLockType() const{
	int N = ((be_CGROUP*)(this))->GetTotalAmount();
	if ( N<=0 ) return -1;
	byte LockType = 3;
	OneObject* pOB = NULL;
	while ( N-- ) {
		pOB = (*(((be_CGROUP*)(this))))[N];
		if ( pOB!=NULL ) {
			LockType = min( LockType, pOB->LockType );
		};
	};
	return (int)LockType;
};// 0-lend,1-sea,2-low fly,3-hight fly
bool	be_CGROUP::GetStandGround(){
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	int SGN = 0;
	while (N--) {
		pOB = (*this)[N];
		if ( pOB!=NULL && pOB->StandGround==true ) SGN++;
	};
	return (bool)(SGN==GetTotalAmount());
};
bool				be_CGROUP::FillTN_array(be_CARRAY* pARR){
	if (GetTotalAmount()<=0||pARR==NULL) return false;
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	bool newType = false;
	LinearArray<int,_int> arrTN;
	while (N--) {
		pOB = (*this)[N];
		if ( pOB!=NULL ){
			newType = true;
			for ( int i=0; ( newType && i<arrTN.GetAmount() ); i++ ){
				if ( i%2 == 0 ){
					if ( arrTN[i]==pOB->NIndex ){
						newType = false;
						arrTN[i+1] += 1;
					};
				};
			};
			if (newType) {
				arrTN.Add(pOB->NIndex);
				arrTN.Add(1);
			};
		};
	};
	be_CINT* pINT = NULL;
	char s[64];
	for ( int i=0; i<arrTN.GetAmount(); i++ ){
		if ( i%2 == 0 ) {
			sprintf(s,"%s%d","UT",i/2);
			pINT = pARR->ValuesArray.CreateParam_INT(s);
		}else{
			sprintf(s,"%s%d","UN",i/2);
			pINT = pARR->ValuesArray.CreateParam_INT(s);
		};
		pINT->Value = arrTN[i];
		pINT=NULL;
	};
	return true;
};
bool				be_CGROUP::FillDIFF_TN_array(be_CARRAY* pARRdest,be_CARRAY* pARRsupport,int maxUN){
	if ( pARRdest==NULL || pARRdest->GetAmount()<=0 || pARRsupport==NULL ) return false;
	pARRsupport->ValuesArray.CLEAR();

	int			UT = -1;
	int			UN = -1;
	char		s[64];
	be_CINT*	pINT = NULL;

	int			localMAX_UN = maxUN;

	for ( int i=0; (i<pARRdest->GetAmount()/2)&&(localMAX_UN>0); i++ ){
		UT = ((be_CINT*)(pARRdest->ValuesArray[2*i]->GetVariable()))->Value;
		UN = ((be_CINT*)(pARRdest->ValuesArray[2*i+1]->GetVariable()))->Value;
		if ( GetTotalAmount2(UT)<UN ){
			sprintf(s,"%s%d","UT",i);
			pINT = pARRsupport->ValuesArray.CreateParam_INT(s);
			pINT->Value = UT;
			sprintf(s,"%s%d","UN",i);
			pINT = pARRsupport->ValuesArray.CreateParam_INT(s);
			for ( int ii=0; (ii<UN-GetTotalAmount2(UT))&&(localMAX_UN>0); ii++ ){
				localMAX_UN--;
				pINT->Value += 1;
			};
			pINT = NULL;
		};
	};

	return true;
};
bool				be_CGROUP::GetCapturability(){
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = (*this)[N];
		if ( pOB!=NULL && pOB->newMons->Capture==false ) return false;
	};
	return true;
};
void				be_CGROUP::DisableCapturability(){
	void DisableCapturability(OneObject* OB);
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while ( N-- ) {
		pOB = (*this)[N];
		if	 (pOB!=NULL&&pOB->NewBuilding) {
			DisableCapturability(pOB);
		};
	};
};
void				be_CGROUP::EnableCapturability(){
	void EnableCapturability(OneObject* OB);
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while ( N-- ) {
		pOB = (*this)[N];
		if	 (pOB!=NULL&&pOB->NewBuilding) {
			EnableCapturability(pOB);
		};
	};
};
int					be_CGROUP::GetHP() const{
	int NU = ((be_CGROUP*)(this))->GetTotalAmount();
	if (NU==0) return 0;
	OneObject* pU = NULL;
	float HP = 0.f;
	int NN = 0;
	while (NU--) {
		pU = ((be_CGROUP*)(this))->GetOneObj_lID(NU);
		if (pU) {
			if ( pU->Life>0 && pU->GetLiveState()==ls_LiveBody ) { HP += (float)(pU->Life); NN++; };
		};
		pU = NULL;
	};	
	if (NN==0) return 0;
	return (int)(HP/(float)NN);
};// Retern average value of units HP
int					be_CGROUP::GetMP() const{
	int NU = ((be_CGROUP*)(this))->GetTotalAmount();
	if (NU==0) return 0;
	OneObject* pU = NULL;
	float HP = 0.f;
	int NN = 0;
	while (NU--) {
		pU = ((be_CGROUP*)(this))->GetOneObj_lID(NU);
		if (pU) {
			if ( pU->Mana>0 && pU->GetLiveState()==ls_LiveBody ) { HP += (float)(pU->Mana); NN++; };
		};
		pU = NULL;
	};	
	if (NN==0) return 0;
	return (int)(HP/(float)NN);
};
void				be_CGROUP::SetSearchWict(const bool _flag){
	int N = GetTotalAmount();
	OneObject* pOB=NULL;
	while (N--) {
		pOB=GetOneObj_lID(N);
		if (pOB!=NULL&&TryRandom()) {
			pOB->NoSearchVictim=!_flag;
		};
		pOB=NULL;
	};
};// Set SearchWict flag for units in group
bool				be_CGROUP::SetRallyPoint(int x, int y){
	bool OK = true;
	OneObject* pOB = NULL;
	int N = GetTotalAmount();
	while (N--) {
		pOB = GetOneObj_lID(N);
		if(pOB&&pOB->NewBuilding){
			pOB->DstX=x*16;
			pOB->DstY=y*16;
		}else{
			OK = false;
		};
		pOB = NULL;
	};
	return OK;
};
void				be_CGROUP::SetAIGuard(){
	OneObject*	pOB = NULL;
	int N = GetTotalAmount();
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB) pOB->AI_Guard = true;
	};
};
void				be_CGROUP::setAIGuard(bool _state){
	OneObject*	pOB = NULL;
	int N = GetTotalAmount();
	while (N--) {
		if (pOB=(*this)[N]) pOB->AI_Guard = _state;
	};
};
bool				be_CGROUP::getAIGuard()const{
	OneObject*	pOB = NULL;
	int N = ((be_CGROUP*)this)->GetTotalAmount();
	while (N--) {
		if (pOB=(*((be_CGROUP*)this))[N]) return pOB->AI_Guard;
	};
	return false;
};
bool				be_CGROUP::GetSearchWict()	const{
	bool serch = true;
	int N = ((be_CGROUP*)(this))->GetTotalAmount();
	OneObject* pOB=NULL;
	while (N--) {
		pOB=((be_CGROUP*)(this))->GetOneObj_lID(N);
		if (pOB!=NULL) {
			serch = serch && !pOB->NoSearchVictim;
		};
		pOB=NULL;
	};
	return serch;
};// Set SearchWict flag for units in group
int					be_CGROUP::GetTotalAmount2(int UnitType, int Nat/*=0xff*/){
	int retVal = 0;
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB&&pOB->NIndex==UnitType) {
			retVal++;
			if (Nat!=0xff && pOB->NNUM!=Nat) {
				retVal--;
			};
		};
		pOB = NULL;
	};
	return	retVal;
};
void SetBrigadeFlagbearers(Brigade* BR);
void				be_CGROUP::SetBrFlagbearers(){
	int retVal = 0;
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL) {
			if (pOB->BrigadeID!=0xFFFF){
				Brigade* BR=&CITY[pOB->NNUM].Brigs[pOB->BrigadeID];
				if (BR) {
					SetBrigadeFlagbearers(BR);
					break;
				};
			};
		};
		pOB = NULL;
	};
};
void ClearBrigadeFlagbearers(Brigade* BR);
void				be_CGROUP::ClearBrFlagbearers(){
	int retVal = 0;
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL) {
			if (pOB->BrigadeID!=0xFFFF){
				Brigade* BR=&CITY[pOB->NNUM].Brigs[pOB->BrigadeID];
				if (BR) {
					ClearBrigadeFlagbearers(BR);
					RemoveDeadUnits();
					break;
				};
			};
		};
		pOB = NULL;
	};
};
void				be_CGROUP::SendToPosition(vvPOINT_SET* pPosArr){
	int UN = GetTotalAmount();
	int PN = pPosArr->Value.GetAmount();
	OneObject* pOB = NULL;
	while (UN--) {
		pOB = GetOneObj_lID(UN);
		if (pOB!=NULL){
			int x = pPosArr->Value[UN%PN]->Value.x;
			int y = pPosArr->Value[UN%PN]->Value.y;
			pOB->NewMonsterSendTo(x<<4,y<<4,128+0,0);
			addname(pOB->Index);
			addrand(x);
			addrand(y);
		};		
	};
};
bool				be_CGROUP::ChekPosition(vvPOINT_SET* pPosArr){
	bool	retVal = true;
	int UN = GetTotalAmount();
	int PN = pPosArr->Value.GetAmount();
	OneObject* pOB = NULL;
	bool	inPos = false;
	while (retVal&&UN--) {
		inPos=false;
		pOB = GetOneObj_lID(UN);
		if (pOB!=NULL){
			int ux = pOB->RealX>>4;
			int uy = pOB->RealY>>4;
			for (int i=0;!inPos&&i<PN;i++){
				int px = pPosArr->Value[i]->Value.x;
				int py = pPosArr->Value[i]->Value.y;
				if (Norma(ux-px,uy-py)<=50)	inPos = true;
			};
			if (inPos==false)	retVal=false;
		};
	};
	return retVal;
};
bool				be_CGROUP::CheckAttackPossibilityG(be_CGROUP* pEnemyGroup){
	if ( pEnemyGroup==NULL || pEnemyGroup->GetTotalAmount()<=0 ) return false;
	int myN = GetTotalAmount();
	int enN = pEnemyGroup->GetTotalAmount();
	OneObject* myOB = NULL;
	OneObject* enOB = NULL;
	while (myN--) {
		myOB=(*this)[myN];
		if ( myOB!=NULL ){
			for ( int e=0; e<enN; e++ ){
				enOB=(*pEnemyGroup)[e];
				if ( enOB!=NULL && CheckPossibilityToAttackEnemy(myOB,enOB) ){
					return true;
				};
			};
		};
	};
	return false;
};
bool				be_CGROUP::CheckAttackPossibilityXYR(int _x,int _y,int _R){
	itr_UnitsInRadius.Create(_x,_y,_R);
	itr_GetBuildingsInRadius.Create(_x,_y,_R);
	int myN = GetTotalAmount();
	OneObject* myOB = NULL;
	OneObject* enOB = NULL;
	while (myN--) {
		myOB=(*this)[myN];
		if ( myOB!=NULL ){
			while(OneObject* enOB=itr_UnitsInRadius.Next()){
				if ( enOB!=NULL && CheckPossibilityToAttackEnemy(myOB,enOB) ){
					return true;
				};
			};
			while(OneObject* enOB=itr_GetBuildingsInRadius.Next()){
				if ( enOB!=NULL && CheckPossibilityToAttackEnemy(myOB,enOB) ){
					return true;
				};
			};
		};
	};
	return false;
};
bool				be_CGROUP::CheckAttackPossibilityN(be_CNODE*  pNode){
	if ( pNode!=NULL ){
		return CheckAttackPossibilityXYR(pNode->x,pNode->y,pNode->R);
	};
	return false;
};
void UnitAbilityTest();
void				be_CGROUP::AttackGroup_NearestOB(be_CGROUP* pEnemyGroup){
	if ( pEnemyGroup==NULL || pEnemyGroup->GetTotalAmount()==0 ) return;
	//UnitAbilityTest();
	RemoveDeadUnits();
	//UnitAbilityTest();
	pEnemyGroup->RemoveDeadUnits();
	//UnitAbilityTest();
	//pEnemyGroup->RemoveInvisibleForNMask( GetNMASK() ); // by vital
	//UnitAbilityTest();
	int	enN = pEnemyGroup->GetTotalAmount();
	int myN = GetTotalAmount();
	//UnitAbilityTest();
	lincs.FillCurList(this);
	//UnitAbilityTest();
	lincs.UpdateCur1vs1(this,pEnemyGroup);
	//UnitAbilityTest();
	OneObject* pAttOB = NULL;
	for ( int i=0; i<myN; i++ ){
		pAttOB = GetOneObj_lID(i);
		if ( pAttOB && lincs.curLincs[i]->Aid==pAttOB->Index ){
			if ( lincs.curLincs[i]->Tid==0xFFFF || lincs.curLincs[i]->distPower<=5.0f ){
				int curD = 0; int minD = -1;
				OneObject* pTrgOB = NULL;
				OneObject* pTrueTrgOB = NULL;
				for ( int j=0; j<enN; j++ ){
					pTrgOB = pEnemyGroup->GetOneObj_lID(j);
					if (pTrgOB && CheckPossibilityToAttackEnemy(pAttOB,pTrgOB)){
						curD = Norma( (pAttOB->RealX-pTrgOB->RealX)/16, (pAttOB->RealY-pTrgOB->RealY)/16 );
						if ( curD<minD || minD==-1 ){
							minD=curD;
							pTrueTrgOB = pTrgOB;
						};
					};
				};
				if (pTrueTrgOB){
					pAttOB->AttackObj( pTrueTrgOB->Index,128+16,0 );
					pTrueTrgOB = NULL;
				};
			};
		};
		pAttOB=NULL;
	};
	//UnitAbilityTest();
};
void				be_CGROUP::AttackGroup_NearestOB00(be_CGROUP* pEnemyGroup){
	if ( pEnemyGroup==NULL || pEnemyGroup->GetTotalAmount()==0 ) return;
	
	int	enN = pEnemyGroup->GetTotalAmount();
	int myN = GetTotalAmount();

	OneObject** pUSED = new OneObject*[myN];
	for (int i=0;i<myN;i++) pUSED[i]=GetOneObj_lID(i);
	int USED_N = myN;

	OneObject** pENEMY = new OneObject*[enN];
	for (int i=0;i<enN;i++) pENEMY[i]=pEnemyGroup->GetOneObj_lID(i);
	int ENEMY_N = enN;
	
	int MAX_ITERATIONS = myN*enN+1;
	int EVERAGE_ATTACERS = (int)( (float)myN/(float)enN + 0.5f );

	// Delete inaf attaced units
	int AAA = 0;
	for ( int i=0; i<enN; i++ ){
		if ( pENEMY[i] ){
			AAA = 0;
			for ( int j=0; j<myN&&AAA<=EVERAGE_ATTACERS; j++ ){
				if ( pUSED[j] && pUSED[j]->Attack && pUSED[j]->EnemyID==pENEMY[i]->Index ){
					AAA++;
				};
			};
			if (AAA>EVERAGE_ATTACERS){
				AAA=0;
				for ( int j=0; j<myN&&AAA<=EVERAGE_ATTACERS; j++ ){
					if ( pUSED[j] && pUSED[j]->Attack && pUSED[j]->EnemyID==pENEMY[i]->Index && pUSED[j]->InMotion==0 ){
						pUSED[j]=NULL;
						USED_N--;
						AAA++;
					};
				};
				for ( int j=0; j<myN&&AAA<=EVERAGE_ATTACERS; j++ ){
					if ( pUSED[j] && pUSED[j]->Attack && pUSED[j]->EnemyID==pENEMY[i]->Index ){
						if	(Norma((pUSED[j]->RealX-pENEMY[i]->RealX)/16,(pUSED[j]->RealY-pENEMY[i]->RealY)/16)<100)
						{
							pUSED[j]=NULL;
							USED_N--;
							AAA++;
						};
					};
				};
				for ( int j=0; j<myN&&AAA<=EVERAGE_ATTACERS; j++ ){
					if ( pUSED[j] && pUSED[j]->Attack && pUSED[j]->EnemyID==pENEMY[i]->Index ){
						pUSED[j]=NULL;
						USED_N--;
						AAA++;
					};
				};
			
				pENEMY[i]=NULL;
				ENEMY_N--;
			};
		};
	};
	
	OneObject* pmyOB = NULL;
	OneObject* penOB = NULL;
	int useID = -1;
	int minD = -1;
	int curD = -1;
	while (USED_N&&MAX_ITERATIONS--) {
		for ( int ei=0; ei<enN&&USED_N&&ENEMY_N; ei++ ) {
			penOB = pENEMY[ei];
			if (penOB) {
				minD = -1;
				useID= -1;
				for (int i=0;i<myN;i++){
					if ( pUSED[i]!=NULL ){
						curD = Norma( penOB->RealX/16-pUSED[i]->RealX/16,penOB->RealY/16-pUSED[i]->RealY/16 );
						if ( curD<minD || minD==-1 ){
							curD = minD;
							useID= i;	
						};
					};
				};
				if ( useID!=-1 ){
					pUSED[useID]->AttackObj( penOB->Index,128+16,0 );
					pUSED[useID]=NULL;
					USED_N--;
				};
			};
		};
	}
	for (int i=0;i<myN;i++) pUSED[i]=NULL;
	delete pUSED;

	bool test = false;
	if ( test ){
		lincs.FillCurList(this);
		lincs.UpdateCur1vs1(this,pEnemyGroup);
		lincs.UpdateCurNvs1();
	};

};// Set targets for group for attack pEnemyGroup
void				be_CGROUP::AttackEnemy00(be_CGROUP* pEnemyGroup,int pow){
	int N = GetTotalAmount();
	if ( N<=0 ) return;
	OneObject* pOB = NULL;
	OneObject* pTR = NULL;
	for ( int i=0; i<N; i++ ){
		pOB = GetOneObj_lID(i);
		if (pOB) {
			if ( pOB->Attack && pOB->EnemyID!=0xFFFF && pOB->EnemyID<MAXOBJECT ){
				pTR = Group[pOB->EnemyID];
				if ( pTR ){
					if ( !(!pTR->Sdoxlo||pTR->Hidden) || pTR->Serial!=pOB->EnemySN  ) {
						pTR = NULL;
					};
				};
			};
//			if (pTR&&be_gCheckDirections(pOB,pTR)==false&&be_gGetTopDistance(pOB,pTR)>150)	pTR = NULL;
            // USE POWER
			if ( pow>0 && pTR && (rando()%1000)<pow*10 ) pTR = NULL;
			// Check current target
			/*
            if ( pTR ) {
				if ( be_gCheckAbsAttackAbility(pOB,pTR)==false )										pTR = NULL;
				if ( pTR && be_gCheckDirections(pOB,pTR)==false && pTR->GroupSpeed>pOB->GroupSpeed )	pTR = NULL;
				if ( pTR && be_gCheckCurAttackAbility(pOB,pTR)==false )									pTR = NULL;
			};
			*/
			// if need select new
			if (pTR==NULL) {
				int attackR = 0;
				if ( pOB->MoreCharacter->DetRadius2[0]!=0 )	attackR = pOB->MoreCharacter->AttackRadius2[0];
				else if (pOB->MoreCharacter->DetRadius2[1]!=0 )	attackR = pOB->MoreCharacter->AttackRadius2[1];
				if (attackR!=0) {
					if (pEnemyGroup!=NULL){
						int EN = pEnemyGroup->GetTotalAmount();
						if (EN>0) {
							int			minD		= 0;
							bool		Indirect	= false;
							OneObject*	pnewTR		= NULL;
							while (EN--) {
								if (pTR==NULL){
									pTR = pEnemyGroup->GetOneObj_lID(EN--);
									if (be_gCheckCurAttackAbility(pOB,pTR)) {
										minD = be_gGetTopDistance(pOB,pTR);
										Indirect = be_gCheckDirections(pOB,pTR);
									}else{
										pTR = NULL;
									}
								}else{
									pnewTR = pEnemyGroup->GetOneObj_lID(EN--);
									int D = be_gGetTopDistance(pOB,pnewTR);
									if (D<minD&&( !Indirect || (Indirect&&be_gCheckDirections(pOB,pnewTR)) )) {
										minD =D;
										pTR = pnewTR;
									}
									if (!Indirect) Indirect = be_gCheckDirections(pOB,pnewTR);
								};
							};
						};
					}else{
						itr_UnitsInRadius.Create(pOB->RealX/16,pOB->RealY/16,attackR);
						int			minD		= 0;
						bool		Indirect	= false;
						OneObject*	pnewTR		= NULL;
						while(pnewTR=itr_UnitsInRadius.Next()){
							if (pnewTR&&(!pnewTR->Sdoxlo||pnewTR->Hidden)&&be_gCheckCurAttackAbility(pOB,pnewTR)){
								if (pTR==NULL) {
									minD = be_gGetTopDistance(pOB,pTR);
									Indirect = be_gCheckDirections(pOB,pTR);
									pTR = pnewTR;
								}else{
									int D = be_gGetTopDistance(pOB,pnewTR);
									if (D<minD&&( !Indirect || (Indirect&&be_gCheckDirections(pOB,pnewTR)) )) {
										minD =D;
										pTR = pnewTR;
									}
									if (!Indirect) Indirect = be_gCheckDirections(pOB,pnewTR);
								}
							};
						};		
					}
				}
			};
		};
		
		// Attack Target
		if (pTR!=NULL) {
			pOB->AttackObj( pTR->Index,128+16,0 );
		};
	};
};
void				be_CGROUP::idAttackEnemy( DWORD OverOrders,bool nearest/*=false*/,int addR/*=0*/, be_CARRAY* pARR/*=NULL*/ ){
	int N = GetTotalAmount();
	if ( N<=0 ) return;
	if ( last_idAttackEnemy_ID>N ) last_idAttackEnemy_ID=0;
	OneObject*		pOB			= NULL;
	OneObject*		pOBTR		= NULL;
	OneObject*		pOB00		= NULL;
	en_vGroup_Order	OB_ORD		= vgNO_ORDERS;
	int				ddd			= -1;
	int				cur			= -1;
	int NNN = GetTotalAmount();
	int iteration = 0;
	bool _inBAD_POS = false;
//	for ( N=last_idAttackEnemy_ID; (N<NNN)&&(iteration<100); N++ ) {
	for ( N=0; N<NNN; N++ ) {
		iteration++;
		pOB		= (*this)[N];
		if ( pOB!=NULL ) {
			OB_ORD	= GetUnitORDER(N);
			// Check is unit attack buildings
			bool targetIsBuilding = false;
			if ( pOB->Attack && pOB->EnemyID!=0xFFFF && Group[pOB->EnemyID]!=NULL && Group[pOB->EnemyID]->Serial==pOB->EnemySN ){
				targetIsBuilding = Group[pOB->EnemyID]->NewBuilding;
			};
			// ------------------------------
			if ( OB_ORD==vgNO_ORDERS || OB_ORD&((OverOrders&vgATTACK)|OverOrders) || targetIsBuilding ){
				int curAttackDist = -1;
				if ( pOB->Attack && pOB->EnemyID!=0xFFFF && Group[pOB->EnemyID]!=NULL && (Group[pOB->EnemyID])->Serial==pOB->EnemySN ){
					pOB00 = Group[pOB->EnemyID];
					curAttackDist = Norma((pOB00->RealX-pOB->RealX)/16,(pOB00->RealY-pOB->RealY)/16);
					pOB00 = NULL;
				};
				int killR = pOB->MoreCharacter->VisRange/16;
				if ( killR<500 ) killR = 500;

				int Killl = 0;
				int AttackR = 0;
				if ( pOB->MoreCharacter->DetRadius2[0]!=0 ){
					Killl = pOB->MoreCharacter->AttackRadius2[0] * 2;
					AttackR = Killl/2;
				}else if (pOB->MoreCharacter->DetRadius2[1]!=0 ){
					Killl = pOB->MoreCharacter->AttackRadius2[1];
					AttackR = Killl;
				};

				if ( curAttackDist==-1 || curAttackDist>Killl ){
					if ( pARR!=NULL && pARR->GetAmount()>0 ){
						for ( int i=0; (!_inBAD_POS)&&(i<pARR->GetAmount()); i++ ){
							be_CNODE* pND = (be_CNODE*)((*pARR)[i]);
							if ( pND && Norma(pOB->RealX/16-pND->x,pOB->RealY/16-pND->y)<=pND->R ){
								_inBAD_POS = (pND->SegmFR==100);
							};
						};
					};

					if (_inBAD_POS) killR += addR;
					// Attack enemy use AttackType
					// TEST - Attack Nearest
					ddd = -1;
					if ( killR!=0 ){
						// Find target in units
						//itr_UnitsInRadius.Create(pOB->RealX/16,pOB->RealY/16,killR);
						itr_UnitsInRadiusFromOtherNations.Create(pOB->NNUM,pOB->RealX/16,pOB->RealY/16,killR);
						while(pOB00=itr_UnitsInRadiusFromOtherNations.Next()){
							if ( (pOB00->NMask&pOB->NMask)==0 && !pOB00->UnlimitedMotion ){
								cur = Norma( (pOB->RealX-pOB00->RealX)/16,(pOB->RealY-pOB00->RealY)/16 );
								if ( ddd==-1 || cur<ddd ){
									if ( CheckPossibilityToAttackEnemy(pOB,pOB00) ){
										pOBTR = pOB00;
										ddd = cur;
										if ( ddd<=AttackR ) break;
									};
								};
							};
						};
						if ( pOBTR==NULL || (nearest&&_inBAD_POS) ){
							// Find target in buildings
							itr_GetBuildingsInRadius.Create(pOB->RealX/16,pOB->RealY/16,killR);
							while(pOB00=itr_GetBuildingsInRadius.Next()){
								if ( (pOB00->NMask&pOB->NMask)==0 ){
									cur = Norma( (pOB->RealX-pOB00->RealX)/16,(pOB->RealY-pOB00->RealY)/16 );
									if ( ddd==-1 || cur<ddd ){
										if ( CheckPossibilityToAttackEnemy(pOB,pOB00) ){
											pOBTR = pOB00;
											ddd = cur;
											if ( ddd<=AttackR ) break;
										};
									};
								};
							};
						};

						if ( pOBTR!=NULL ){
							//pOB->ClearOrders();
							if ( pOB->StandGround )	pOB->AttackObj( pOBTR->Index, 128+1 , 0 );
							else					pOB->AttackObj( pOBTR->Index, 128+16, 0 );
							pOBTR = NULL;
						}else{
							if (pOB->Attack) pOB->ClearOrders();
						};
					};
				};
			};
		};
	};
	last_idAttackEnemy_ID+=100;
};
void				be_CGROUP::SetOB_useDispersePosition(int px,int py,OneObject* pOB,byte Prio,byte OrdType,bool donotCheckPos){
	addrand(px);
	addrand(py);
	if ( pOB==NULL || pOB->GetLiveState()!=ls_LiveBody ) return;
	if (GetSendDispersion()) {
		int _dx,_dy;
		GetDispersePosition(px,py,_dx,_dy,pOB);
		if (donotCheckPos || CheckOB_pos(pOB,px,py)==false) {
			pOB->NewMonsterSendTo(_dx*16,_dy*16,Prio,OrdType);
			addname(pOB->Index);
			addrand(_dx);
			addrand(_dy);
		}
	}else{
		if (donotCheckPos || CheckOB_pos(pOB,px,py)==false) {
			pOB->NewMonsterSendTo(px*16,py*16,Prio,OrdType);
			addname(pOB->Index);
			addrand(px);
			addrand(py);
		}
	};
};
void				be_CGROUP::idSendTo( int x, int y, int dir, DWORD OverOrders, int _id, int type,bool withAttack,bool donotCheckPos ){
	OneObject*		pOB = NULL;
	en_vGroup_Order	OB_ORD = vgNO_ORDERS;
	addrand(x);
	addrand(y);
	if ( _id==-1 ){
		int N = GetTotalAmount();
		if ( N<=0 ) return;
		while ( N-- ) {
			pOB		= (*this)[N];
			if ( pOB!=NULL ) {
				OB_ORD	= GetUnitORDER(N);
				if ( OB_ORD==vgNO_ORDERS || (OB_ORD&OverOrders) ){
					SetOB_useDispersePosition(x,y,pOB,( (withAttack) ? (128) : (128+16) ),type,donotCheckPos);
				};
			}
		};
	}else{
		pOB		= (*this)[_id];
		if ( pOB!=NULL ) {
			OB_ORD	= GetUnitORDER(_id);
			if ( OB_ORD==vgNO_ORDERS || (OB_ORD&OverOrders) ){
				SetOB_useDispersePosition(x,y,pOB,( (withAttack) ? (128) : (128+16) ),type,donotCheckPos);		
			};
		};
	};
};
void				be_CGROUP::Produse00(int UnitType, int UnitNUM){
	// Order -> Produse units uniformly
	int N = GetTotalAmount();
	if ( N==0 ) return;
	int curI = 0;
	OneObject* pOB = NULL;
	for ( int i=0; i<UnitNUM; i++ ){
		pOB = GetOneObj_lID(curI++);
		if (pOB){ 
			int res[8];
			for(int i=0;i<8;i++)res[i]=XRESRC(pOB->NNUM,i);
			pOB->Produce(UnitType);
			for(int i=0;i<8;i++)SetXRESRC(pOB->NNUM,i,res[i]);

		}
		if (curI>=N) curI=0;
	};
};
void				be_CGROUP::Produse00(int UnitType, int UnitNUM, be_CGROUP* pGR)
{
	int MaxOnMap	 = -1; // Get from map options
	int NP = MOptions.LimitUnitsInSettlement.GetAmount();
	for ( int i=0; (i<NP/2)&&(MaxOnMap==-1); i++ ){
		be_CUnitType* pUT = (be_CUnitType*)(MOptions.LimitUnitsInSettlement.GetValue( i*2+1 ));
		if ( pUT && pUT->GetValue()==UnitType ){
			be_CINT* pUN = (be_CINT*)(MOptions.LimitUnitsInSettlement.GetValue( i*2+2 ));
			if ( pUN ) MaxOnMap = pUN->GetValue();
		};
	}
	if ( MaxOnMap<0 ){ 
		Produse00(UnitType,UnitNUM);
		return;
	};
	int AlreadyOnMap = ( (pGR!=NULL) ? (pGR->GetTotalAmount2(UnitType)) : (0) );
	int _curUN = min( MaxOnMap-AlreadyOnMap, UnitNUM );
    if (_curUN>0) Produse00( UnitType, _curUN );
} // be_CGROUP::Produse00
bool				be_CGROUP::IsGRPAttackMe(be_CGROUP* pGRP){
	if ( !pGRP || !pGRP->GetTotalAmount() ) return false;
	bool YES = false;
	int N = pGRP->GetTotalAmount();
	OneObject* pGRP_OB = NULL;
	while (N--&&!YES) {
		pGRP_OB = pGRP->GetOneObj_lID(N);
		YES = ( pGRP_OB && pGRP_OB->Attack && pGRP_OB->EnemyID!=0xFFFF && _CheckGID(pGRP_OB->EnemyID)!=0xFFFF );
	};
	return YES;
};
int					be_CGROUP::GetTotalAmount() const{ 
	return Units.GetAmount();
};
int					be_CGROUP::GetNationAmout(int Nat){
	if ( Nat<0 || Nat>=8 ) return 0;
	int N = GetTotalAmount();
	if ( N<=0 )	return 0;
	int NA = 0;
	OneObject* pOB = NULL;
	while (N--) {
		pOB = (*this)[N];
		if ( pOB!=NULL && pOB->NNUM==Nat ) NA++;
	};
	return NA;
};// Кол-во юнитов данной нации в группе
int					be_CGROUP::GetAmountInZone(int x, int y, int R){ 
	//	RemoveDeadUnits();
	int N = GetTotalAmount();
	int retN = 0;
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL) {
			if (Norma((pOB->RealX>>4)-x,(pOB->RealY>>4)-y)<=R) retN++;
		};
	};
	return retN; 
};
void				be_CGROUP::GetGroupCenter(int& x,int& y){
	x=y=0;
	int N = GetTotalAmount();
	if (N==0)	return;	
	OneObject* pOB = NULL;
	int X=0;
	int Y=0;
	int N0=0;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL) {
			X += (pOB->RealX>>4);
			Y += (pOB->RealY>>4);
			N0++;
		};
	};
	if(N0==0)return;
	x = X/N0;
	y = Y/N0;
};
int					be_CGROUP::GetGroupX() const{
	int x,y;
	((be_CGROUP*)(this))->GetGroupCenter(x,y);
	return x;
};
int					be_CGROUP::GetGroupY() const{
	int x,y;
	((be_CGROUP*)(this))->GetGroupCenter(x,y);
	return y;
};
int					be_CGROUP::GetXi(int i) const{
	int Xi = 0;
	OneObject* pOB = ((be_CGROUP*)(this))->GetOneObj_lID(i);	
	if (pOB!=NULL) {
		Xi = pOB->RealX/16;
	};
	return Xi;
};
int					be_CGROUP::GetYi(int i) const{
	int Yi = 0;
	OneObject* pOB = ((be_CGROUP*)(this))->GetOneObj_lID(i);	
	if (pOB!=NULL) {
		Yi = pOB->RealY/16;
	};
	return Yi;
};
int					be_CGROUP::GetGID(int i){
	OneObject* pOB = (*this)[i];
	if ( pOB!=NULL ) return pOB->Index;
	return -1;
};
int					be_CGROUP::GetDIRi(int i) const{
	int DIRi = 0;
	OneObject* pOB = ((be_CGROUP*)(this))->GetOneObj_lID(i);	
	if (pOB!=NULL) {
		DIRi = pOB->RealDir;
	};
	return DIRi;
};
int					be_CGROUP::GetGIDi(int i) const{
	int GIDi = -1;
	OneObject* pOB = ((be_CGROUP*)(this))->GetOneObj_lID(i);	
	if (pOB!=NULL) {
		GIDi = pOB->Index;
	};
	return GIDi;
};
int					be_CGROUP::GetDirection(){
	int N = GetTotalAmount();
	if (N<=0)	return 0xFFFF;
	int TN = GetTotalAmount();
	int dF = 8;
	int Fi = -1;
	int trueDIR_UN = 0;
	float cosF = 0.f;
	float sinF = 0.f;
	OneObject* pOB = NULL;
	bool bSQ = false;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB){
			if (pOB->BrigadeID!=0xFFFF)	bSQ=true;

			float fF = ((float)(pOB->RealDir)/256.f)*(2.f*3.1415f);
			cosF += cosf(fF);
			sinF += sinf(fF);

		};
	};
	float prc = 0.8f;
	if (bSQ==false)	prc = 0.5f;

	float DDD = sqrt( cosF*cosF + sinF*sinF );
	if (DDD!=0.f&&DDD>=(float)TN*prc) {
		float retF=lvBE_GetVecFi(cosF/DDD,sinF/DDD);
		retF = (retF/(2.f*3.1415f))*256.f;
		return (int)(retF);
	};

	return	0xFFFF;
};
byte				be_CGROUP::GetNation() const {
	byte retNI = 0xFF;
	int N = ((be_CGROUP*)(this))->GetTotalAmount();
	addrand(N);
	if(N){
		OneObject* pOB = NULL;
		while (pOB==NULL&&N--) {
			pOB = ((be_CGROUP*)(this))->GetOneObj_lID(N);
			addrand(N);
			if(pOB){
				addname(pOB->Index);
			}
			
			//if (pOB==NULL)	pOB=NULL;
		};
		if (pOB)	retNI = pOB->ActiveAbility?pOB->ActiveAbility->BornNNUM:pOB->NNUM;
	}
	return retNI;
};
int					be_CGROUP::GetAgresiveState(){
	int Normal=0;		// 0 
	int	Peacefull=0;	// 1
	int	Aggressive=0;	// 2
	OneObject* pOB=NULL;
	int N = GetTotalAmount();
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL) {
			if (pOB->ActivityState==0)	Normal++;
			if (pOB->ActivityState==1)	Peacefull++;
			if (pOB->ActivityState==2)	Aggressive++;
		};
	};
	if (Normal>Peacefull&&Normal>Aggressive)	return 0;
	if (Peacefull>Normal&&Peacefull>Aggressive)	return 1;
	return 2;
};
bool				be_CGROUP::GetBrigadeList(bool checkCOM/*=false*/,LinearArray<int,_int>* pBrigList/*=NULL*/){
	int N = GetTotalAmount();
	if (N==0)	return false;
	if (pBrigList==NULL&&checkCOM==false) {
		bool	brigade=false;
		OneObject* pOB=NULL;
		while (!brigade&&N--) {
			pOB = GetOneObj_lID(N);
			if (pOB!=NULL && pOB->BrigadeID!=0xFFFF) {
				brigade=true;
			};
		};
		return brigade;
	}else{
		LinearArray<int,_int>	BrigListTEMP;
		LinearArray<int,_int>*	pBR_LIST		= &BrigListTEMP;
		if (pBrigList!=NULL)	pBR_LIST		= pBrigList;

		OneObject* pOB=NULL;
		while (N--) {
			pOB = GetOneObj_lID(N);
			if (pOB!=NULL && pOB->BrigadeID!=0xFFFF) {
				int NL = pBR_LIST->GetAmount();
				bool	newBrigade = true;
				while (newBrigade&&NL--){
					if ((word)((*pBR_LIST)[NL])==(pOB->BrigadeID))	newBrigade=false;
				};
				if (newBrigade)	pBR_LIST->Add(pOB->BrigadeID);
			};
		};
		if (checkCOM&&pBR_LIST->GetAmount()!=0){
			int BRN = pBR_LIST->GetAmount();
			Brigade* pBR=NULL;
			int Nat=GetNation();
			bool retVal=true;
			for (int i=0; i<BRN&&retVal; i++){
				pBR=&CITY[Nat].Brigs[ (*pBR_LIST)[i] ];
				if (pBR==NULL||pBR->NMemb<3||pBR->Memb[0]==0xFFFF||pBR->Memb[1]==0xFFFF||pBR->Memb[2]==0xFFFF){
					retVal=false;
				};
			};
			return	retVal;
		};
		return (pBR_LIST->GetAmount()!=0);
	};
	return false;
};
int					be_CGROUP::GetAmountOfNewUnits(bool clearList/*=false*/){
	int ret = NofNE;
	if (clearList)	NofNE=0;
	return ret;
};
int					be_CGROUP::GetAmountOfRemUnits(bool clearList/*=false*/){
	int ret = NofRE;
	if (clearList)	NofRE=0;
	return ret;
};	
bool				be_CGROUP::GetIsTired(){
	LinearArray<int,_int>	BrigList;
	GetBrigadeList(false,&BrigList); 
	bool Tired = false;
	int NB = BrigList.GetAmount();
	Brigade* pBR=NULL;
	int nat = GetNation();
	while (!Tired&&NB--) {
		pBR=CITY[nat].Brigs+BrigList[NB];
		if (pBR!=NULL&&pBR->Enabled) Tired = Tired || pBR->IsTired;
	};
	return Tired;
};
bool				be_CGROUP::GetReady() const{
	int N = ((be_CGROUP*)(this))->GetTotalAmount();
	OneObject* pOB = NULL;
	bool READY = true;
	while (READY&&N--) {
		pOB = ((be_CGROUP*)(this))->GetOneObj_lID(N);
		if (pOB && pOB->UnlimitedMotion) READY=false;
	};
	return READY;
};// true if all units on outdors
int					be_CGROUP::GetEnemyUnitsAroundNumber(int R){
	int xc,yc,nat,NUMBER=0;
	GetGroupCenter(xc,yc);
	nat = GetNation();
	OneObject* pOB = NULL;
	itr_UnitsInRadius.Create(xc,yc,R);
	while( pOB = itr_UnitsInRadius.Next() ){
		if ( pOB->GetLiveState()==ls_LiveBody && (pOB->NMask&NATIONS[nat].NMask)==0 )
			NUMBER++;
	};
	if ( NUMBER==0 ){
		itr_GetBuildingsInRadius.Create(xc,yc,R);
		while( pOB = itr_GetBuildingsInRadius.Next() ){
			if ( pOB->GetLiveState()==ls_LiveBody && (pOB->NMask&NATIONS[nat].NMask)==0 )
				NUMBER++;
		};
	}	
	return NUMBER;
};
int					be_CGROUP::GetNofBRLoadedGun(){
	int nofLG=0;
	LinearArray<int,_int>	BrigList;
	GetBrigadeList(false,&BrigList);
	int nBR = BrigList.GetAmount();
	if (nBR>0) {
		int Nat=GetNation();
		OneObject* pOB=NULL;
		Brigade* pBR=NULL;
		for (int i=0; i<nBR; i++){
			pBR=&CITY[Nat].Brigs[ BrigList[i] ];
			if (pBR!=NULL&&pBR->Enabled==true) {
				for(int ii=NBPERSONAL;ii<pBR->NMemb;ii++){
					pOB=Group[ pBR->Memb[i] ];
					if(pOB!=NULL&&!pOB->delay)	nofLG++;
					pOB=NULL;
				};	
			};
		};
	};
	return nofLG;
};
int					be_CGROUP::GetNInside(bool Max/*=false*/){
	int retN=0;
	OneObject* pOB=NULL;
	int N = GetTotalAmount();
	while (N--) {
		pOB=GetOneObj_lID(N);
		if (pOB!=NULL && /*(!pOB->NewBuilding) &&*/ (pOB->MoreCharacter->MaxInside)) {
			if (Max) {
				retN += pOB->MoreCharacter->MaxInside + pOB->AddInside;
			}else{
				retN += pOB->NInside;
			};
		};
		pOB=NULL;
	};
	return retN;
};
bool CheckLeaveContainerAbility(OneObject* OB);
int					be_CGROUP::GetLeaveAbility(){
	bool	retB = true;
	OneObject* pOB=NULL;
	int N = GetTotalAmount();
	while (retB&&N--) {
		pOB=GetOneObj_lID(N);
		if (pOB!=NULL) {
			if (!CheckLeaveContainerAbility(pOB)||pOB->Speed!=0)	retB = false;
		};
		pOB=NULL;
	};
	return retB;
};
void				be_CGROUP::PushNUnitAway(int NU/*=0*/){
	int NUC = NU;
	OneObject* pOB=NULL;
	int N = GetTotalAmount();
	while (N--) {
		pOB=GetOneObj_lID(N);
		if (pOB){
			int NIN = pOB->NInside;
			while (NIN--) {
				pOB->LeaveMine(Group[pOB->Inside[NIN]]->NIndex);
			};
		};
	};
};
void				be_CGROUP::GetSpeed(int& maxS,int& minS,int& average){
	bool	frs=true;
	OneObject* pOB=NULL;
	int	speed=0;
	int N=GetTotalAmount();
	while (N--) {
		pOB=GetOneObj_lID(N);
		if (pOB) {
			speed = pOB->newMons->MotionDist;
			if (speed>maxS)	maxS=speed;
			if (speed<minS)	minS=speed; 
			average+=speed;
			if (frs) {
				frs=false;
				maxS=speed;
				minS=speed;
				average=speed;
			};
		};
	};
	if (N>0)	average = average/N;
};
int					be_CGROUP::GetMaxSpeed(){
	int maxS,minS,aver;
	GetSpeed(maxS,minS,aver);
	return maxS;
};
int					be_CGROUP::GetAverageSpeed(){
	int maxS,minS,aver;
	GetSpeed(maxS,minS,aver);
	return aver;
};
int					be_CGROUP::GetMinSpeed(){
	int maxS,minS,aver;
	GetSpeed(maxS,minS,aver);
	return minS;
};
void				be_CGROUP::SetSpeed(int speed){
	OneObject* pOB=NULL;
	int N=GetTotalAmount();
	while (N--) {
		pOB=GetOneObj_lID(N);
		if (pOB!=NULL) pOB->GroupSpeed=speed;
	};
};
int 				be_CGROUP::GetMinX(){
	int minX = 0.f;
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if ( pOB!=NULL && ( minX>(pOB->RealX>>4) || minX==0.f ) ){
			minX = pOB->RealX>>4;
		};
	};
	return minX;
};
int					be_CGROUP::GetMaxX(){
	int maxX = 0.f;
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if ( pOB!=NULL && ( maxX<(pOB->RealX>>4) || maxX==0.f ) ){
			maxX = pOB->RealX>>4;
		};
	};
	return maxX;
};
int 				be_CGROUP::GetMinY(){
	int minY = 0.f;
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL && ( minY>(pOB->RealY>>4) || minY==0.f ) ){
			minY = pOB->RealY>>4;
		};
	};
	return minY;
};
int					be_CGROUP::GetMaxY(){
	int maxY = 0.f;
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if ( pOB!=NULL && ( maxY<(pOB->RealY>>4) || maxY==0.f ) ){
			maxY = pOB->RealY>>4;
		};
	};
	return maxY;
};
void				be_CGROUP::GetStructNameList( LinearArray<int,_int>& _list ){
	int N = GetTotalAmount();
	OneObject* pOB=NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB!=NULL&&pOB->NewBuilding==true) {
			_list.Add(pOB->NIndex);
		};
	};
	RemoveDublicateInList( _list );
};

bool				be_CGROUP::GetUA() const{
	return UnderAttack;
};
void				be_CGROUP::SetUA(const bool UA){
	UnderAttack = UA;
};
void				be_CGROUP::SetNMASK(const DWORD _mask){
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB) pOB->NMask = _mask; 
	};
};
int					be_CGROUP::GetNMASK() const{
	int N = ((be_CGROUP*)this)->GetTotalAmount();
	if ( N==0 ) return 255;
	int	_mask = 255;
	OneObject* pOB = NULL;
	while ( _mask==255 && N-- ) {
		pOB = ((be_CGROUP*)this)->GetOneObj_lID(N);
		if (pOB)	_mask = pOB->NMask;
	};
	return _mask;
};
void				be_CGROUP::DeleteItemDrops(){
    bool DeleteItemDrops(OneObject* OB);
	int N = GetTotalAmount();
	OneObject* pOB = NULL;
	while (N--) {
		pOB = GetOneObj_lID(N);
		if (pOB) {
			DeleteItemDrops(pOB);
		};	
	};
};
void				be_CGROUP::GiveExpa(int Expa){
	int N = GetTotalAmount();
	if ( N==0 ) return;
	OneObject* pOB = NULL;
	while (N--) {
		pOB = (*this)[N];
		if (pOB!=NULL) {
			void GiveExperience(OneObject* OB, int Expa);
			GiveExperience(pOB,Expa);
		};
	};
};

void				be_CGROUP::IncreaseLevel(int LevelAmount){
	int N = GetTotalAmount();
	if ( N==0 ) return;
	OneObject* pOB = NULL;
	while (N--) {
		pOB = (*this)[N];
		if (pOB!=NULL) {
			void IncreaseLevel(OneObject* OB, int LevelAmount);
			IncreaseLevel(pOB,LevelAmount);
		};
	};
};

void				be_CGROUP::ExploringMAP(){
	OneObject* pOB = NULL;
	int NU = GetTotalAmount();
	for ( int i=0; i<NU; i++ ){
		pOB = GetOneObj_lID(i);
		if ( pOB && GetUnitORDER(i)!=vgSOME_ORDER ){
			g_beFillFULLWAY_SCOUT( pOB->RealX/16, pOB->RealY/16 );
			Area* pArea = NULL;
			int w = NWPTS;
			while ( w-- ){
				pArea = HashTable[0].GetTopArea(FULLWAY[w]);
				if (w==NWPTS-1){
					pOB->NewMonsterSendTo((pArea->x*64+32)*16,(pArea->y*64+32)*16,128+0,0);
					addname(pOB->Index);
					addrand(pArea->x);
					addrand(pArea->y);
				}else{	 
					pOB->NewMonsterSendTo((pArea->x*64+32)*16,(pArea->y*64+32)*16,128+0,2);
					addname(pOB->Index);
					addrand(pArea->x);
					addrand(pArea->y);
				};
			};
		};
	};
};
OneObject*			be_CGROUP::operator[](int _idx){
	return GetOneObj_lID(_idx);	
};
bool				be_CGROUP::CheckOB_pos(OneObject* pOB,int _x,int _y){
	int RRR = 50;
	if ( SendDispersion!=0 ) RRR = Norma( SendDispersion, SendDispersion );
	return (bool)(Norma(pOB->RealX/16-_x,pOB->RealY/16-_y)<=RRR);
};
void				be_CGROUP::SetSendDispersion(int _SD){
	SendDispersion = _SD;	
};
int					be_CGROUP::GetSendDispersion() const{
	return SendDispersion;
};
void				be_CGROUP::GetDispersePosition(int _x, int _y, int& _dx, int& _dy, OneObject* pOB){
	int AREA_DES = SendDispersion/64;
	
	int dddX = ( (AREA_DES!=0) ? ((rando()%AREA_DES)-AREA_DES/2) : (0) );
	int dddY = ( (AREA_DES!=0) ? ((rando()%AREA_DES)-AREA_DES/2) : (0) );

	addrand(dddX);
	addrand(dddY);
	addrand(_x);
	addrand(_y);
	addrand(AREA_DES);
	addrand(pOB->Index);

	while (dddX!=0 && dddY!=0) {
		int AID = HashTable[pOB->LockType].GetTopAreaIndex(_x/64+dddX,_y/64+dddY);
		if ( AID<0xFFFE ){
			addrand(AID);
			Area* pArea = HashTable[pOB->LockType].GetTopArea(AID);
			if (pArea!=NULL) {
				_dx = (_x/64+dddX)*64 + (rando()%64) - 32;
				_dy = (_y/64+dddY)*64 + (rando()%64) - 32;
				addrand(_dx);
				addrand(_dy);
				return;
			};
		};
		if (dddX!=0) dddX += ( ( dddX>0 ) ? (-1) : (1) );
		if (dddY!=0) dddY += ( ( dddY>0 ) ? (-1) : (1) );
	};
	_dx = _x;
	_dy = _y;
	addrand(_dx);
	addrand(_dy);
};
// be_CNODE /////////////////////////////////////////////////////////////////
#define NODE_FONT	SmallWhiteFont1
#define pNODE_FONT	&SmallWhiteFont1
be_CNODE::be_CNODE(){
	vSetGParam(0,0,0,0,0);
	SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::be_CNODE(const char* _Name){
	vSetGParam(0,0,0,0,0);
	if ( SetName(_Name)==false ) SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::be_CNODE(const char* _Name,const int _x,const int _y){
	vSetGParam(_x,_y,0,0,0);
	if ( SetName(_Name)==false ) SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::be_CNODE(const char* _Name,const int _x,const int _y,const int _R){
	vSetGParam(_x,_y,_R,0,0);
	if ( SetName(_Name)==false ) SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::be_CNODE(const char* _Name,const int _x,const int _y,const int _R,const int _Dir){
	vSetGParam(_x,_y,_R,_Dir,0);
	if ( SetName(_Name)==false ) SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::be_CNODE(const char* _Name,const int _x,const int _y,const int _R,const int _Dir,const int _SegmFR){
	vSetGParam(_x,_y,_R,_Dir,_SegmFR);
	if ( SetName(_Name)==false ) SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::be_CNODE(int _x,int _y){
	vSetGParam(_x,_y,0,0,0);
	SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::be_CNODE(int _x,int _y,int _R){
	vSetGParam(_x,_y,_R,0,0);
	SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::be_CNODE(int _x,int _y,int _R,int _Dir){
	vSetGParam(_x,_y,_R,_Dir,0);
	SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::be_CNODE(int _x,int _y,int _R,int _Dir,int _SegmFR){
	vSetGParam(_x,_y,_R,_Dir,_SegmFR);
	SetDefName();
	g_beOBJ_CONTROLER.SetState("be_CNODE");
};
be_CNODE::~be_CNODE(){
	g_beOBJ_CONTROLER.SetState("be_CNODE"); 
};
bool	be_CNODE::GetValueAsSTR(_str& ValueSTR){
	ValueSTR = GetName();
	if ( strstr(ValueSTR.pchar(),"be_CNODE")!=NULL ){
		ValueSTR  = "l_n";
		ValueSTR += GetObjectGlobalID();
	};
	return true;
};
void	be_CNODE::GetTextCoordLT(int&_x,int&_y){
	float	r	= 100.0f;
	float	dx	= 10.0f;
	float	dy	= 10.0f;

	float	mX = (float)x;
	float	mY = (float)y;
	float	mZ = max((float)GetHeight(x,y),0.0f);
	float	mR = (float)r;

	Vector4D	p(mX-mR+dx,mY-mR/1.5f+dy/*-260.f*/,mZ,1);
	WorldToScreenSpace(p);

	_x = (int)p.x;
	_y = (int)p.y;
};
void	be_CNODE::GetTextCoordRT(int&_x,int&_y){
	float	r	= 100.0f;
	float	dx	= 10.0f;
	float	dy	= 10.0f;
	float	nR	= 20.f;
	float	dxB = 5.f;

	float	mX = (float)x;
	float	mY = (float)y;
	float	mZ = max((float)GetHeight(x,y),0.0f);
	if (DriveMode()->NodeStyle==1) mZ += 2.f*nR-dxB;
	float	mR = (float)r;

	Vector4D	p(mX+mR-dx,mY-mR/1.5f+dy/*-260.f*/,mZ,1);
	if (DriveMode()->NodeStyle==1) p.set(mX+dxB,mY,mZ,1);
	WorldToScreenSpace(p);

	_x = (int)p.x;
	_y = (int)p.y;
};
void	be_CNODE::GetNodeRect(int&_x,int&_y,int&_x1,int&_y1){
	int		r = 100;

	_x	= x-r;	_y	= y-(int)((float)r/1.5f);
	_x1 = x+r;	_y1 = y+(int)((float)r/1.5f);
};
void	be_CNODE::ScreenOnNode(){
	if (x!=0||y!=0) {
		float cx=0.f,cy=0.f;
		cx = ((float)x/32.f)-(float)smaplx/2.f;
		cy = ((float)y/32.f)-(float)smaply/2.f;
		if (abs(cx-fMapX)>=1.f/32.f || abs(cy-fMapY)>=1.f/32.f) {
			void SetCameraPos(float x,float y);
			SetCameraPos(cx,cy);
		};
	};	
};
void	be_CNODE::vDrawID(){
	int txX,txY;
	GetTextCoordRT(txX,txY);

	char color0[56];
	sprintf(color0,"%s%x%s","{C 0x",EngSettings.MIS_SET.NodeNameColor,"}");
	char color1[56];
	sprintf(color1,"%s%x%s","{C 0x",EngSettings.MIS_SET.NodeIDColor,"}");

	char	ID[100];
	sprintf(ID,"%s%s%s%d%s%s",color0,"ID( ",color1,GetObjectGlobalID(),color0," )");
	int		TextLen = GetRLen(ID,pNODE_FONT);

	extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);

	if (DriveMode()->NodeStyle==0) {
		ShowStringEx(txX-TextLen,txY,ID,pNODE_FONT);
	}else if (DriveMode()->NodeStyle==1) {
		ShowStringEx(txX,txY,ID,pNODE_FONT);
	};
};
void	be_CNODE::vDrawRect(int r/* =100 */){
	DWORD	col = EngSettings.MIS_SET.NodeLineColor;

	if (selected&&DriveMode()->OBJECT==7){
		if (type==0){
			col=0xFFff0000+rand()%256;
		}else if (type==1){
			col=0xFF00ff00+rand()%256;
		};
	};

	float	mX = (float)x;
	float	mY = (float)y;
	float	mZ = max((float)GetHeight(x,y),0.0f);
	float	mR = (float)r;

	Vector3D	p1(mX-mR,mY+mR/1.5f-10.0f,mZ);
	Vector3D	p2(mX-mR,mY+mR/1.5f,mZ);
	Vector3D	p3(mX-mR+10.0f,mY+mR/1.5f,mZ);
	GPS.DrawLine(p1,p2,col);
	GPS.DrawLine(p2,p3,col);

	p1.set(mX+mR-10.0f,mY+mR/1.5f,mZ);
	p2.set(mX+mR,mY+mR/1.5f,mZ);
	p3.set(mX+mR,mY+mR/1.5f-10.0f,mZ);
	GPS.DrawLine(p1,p2,col);
	GPS.DrawLine(p2,p3,col);

	p1.set(mX+mR,mY-mR/1.5f+10.0f,mZ);
	p2.set(mX+mR,mY-mR/1.5f,mZ);
	p3.set(mX+mR-10.0f,mY-mR/1.5f,mZ);
	GPS.DrawLine(p1,p2,col);
	GPS.DrawLine(p2,p3,col);

	p1.set(mX-mR+10.0f,mY-mR/1.5f,mZ);
	p2.set(mX-mR,mY-mR/1.5f,mZ);
	p3.set(mX-mR,mY-mR/1.5f+10.f,mZ);
	GPS.DrawLine(p1,p2,col);
	GPS.DrawLine(p2,p3,col);
};
void	be_CNODE::vDrawBuoy(){
	DWORD	color = EngSettings.MIS_SET.NodeLineColor;

	if (selected&&DriveMode()->OBJECT==7){
		if (type==0){
			color=0xFFff0000+rand()%256;
		}else if (type==1){
			color=0xFF00ff00+rand()%256;
		};
	};

	float x0 = (float)x;
	float y0 = (float)y;

	float		nR = 20.f;
	Vector3D	pNn[6];
	for (int i=0; i<6; i++){
		pNn[i].set(	Shapes_MAP(nR)->regular_polygon_6D[i]->x+x0,
					Shapes_MAP(nR)->regular_polygon_6D[i]->y+y0,
					max((float)GetHeight(	(int)Shapes_MAP(nR)->regular_polygon_6D[i]->x+x0,
											(int)Shapes_MAP(nR)->regular_polygon_6D[i]->y+y0
										 ),
						 0.0f)
				  );
	};	
	GPS.DrawLine(pNn[0],pNn[1],color);
	GPS.DrawLine(pNn[1],pNn[2],color);
	GPS.DrawLine(pNn[2],pNn[3],color);
	GPS.DrawLine(pNn[3],pNn[4],color);
	GPS.DrawLine(pNn[4],pNn[5],color);
	GPS.DrawLine(pNn[5],pNn[0],color);

	Vector3D	pNF[5];
	pNF[0].set(x0,y0,max((float)GetHeight((int)x0,(int)y0),0.0f));
	pNF[1].set(x0,y0,max((float)GetHeight((int)x0,(int)y0),0.0f)+2.f*nR);
	pNF[2].set(x0+3.f*nR,y0,max((float)GetHeight((int)(x0+3.f*nR),(int)y0),0.0f)+2.f*nR);
	pNF[3].set(x0+3.f*nR,y0,max((float)GetHeight((int)(x0+3.f*nR),(int)y0),0.0f)+nR);
	pNF[4].set(x0,y0,max((float)GetHeight((int)x0,(int)y0),0.0f)+nR);
	GPS.DrawLine(pNF[0],pNF[1],color);
	GPS.DrawLine(pNF[1],pNF[2],color);
	GPS.DrawLine(pNF[2],pNF[3],color);
	GPS.DrawLine(pNF[3],pNF[4],color);
};
bool	be_CNODE::InSector(float Fi){
	float		fDir=((float)Dir/256.f)*2.f*3.1415f;
	float		fSegmFR_Half=((float)SegmFR/256.f)*3.1415f;
	FI_2Pi(fDir);
	//	FI_2Pi(fSegmFR_Half);
	return	(bool)( min(abs(abs(Fi)-abs(fDir)),abs(abs(abs(Fi)-abs(fDir))-2.f*3.1415f)) <= abs(fSegmFR_Half) );
};
void	be_CNODE::vDrawCircle(){
	if (R<=0)	return;

	DWORD	color = EngSettings.MIS_SET.NodeLineColor;

	float		fx=(float)x;
	float		fy=(float)y;
	float		fR=(float)R;

	float		fDir=((float)Dir/256.f)*2.f*3.1415f;
	float		fSegmFR_Half=((float)SegmFR/256.f)*3.1415f;

	float SF1 = ( (SegmFR>0) ? (fDir-fSegmFR_Half) : (0.f) );
	float SF2 = ( (SegmFR>0) ? (fDir+fSegmFR_Half) : (0.f) );

	if (SegmFR>0) {
		// Set Sector Fi in [0,2*Pi);
		FI_2Pi(SF1);
		FI_2Pi(SF2);
	};

	float SegmD = 5.f;						// pixels for one segment
	float SegmF = SegmD/fR;					// segment angle
	int	  SegmN = (int)(2.f*3.1415f/SegmF);	// N of segments

	Vector3D	p1,p2,p0,SEN;
	SEN.set((float)x,(float)y,max((float)GetHeight(x,y),0.0f));

	for (int i=0; i<SegmN; i++){
		p1.set	(	fx+fR*cosf((float)i*SegmF),
					fy+fR*sinf((float)i*SegmF),
					max((float)GetHeight((int)(fx+fR*cosf((float)i*SegmF)),
										 (int)(fy+fR*sinf((float)i*SegmF))),
						0.0f)
				);
		p2.set	(	fx+fR*cosf((float)(i+1)*SegmF),
					fy+fR*sinf((float)(i+1)*SegmF),
					max((float)GetHeight((int)(fx+fR*cosf((float)(i+1)*SegmF)),
										 (int)(fy+fR*sinf((float)(i+1)*SegmF))),
						0.0f)
				);

		if (SegmFR>0) {
			float F1=(float)i*SegmF;
			float F2=(float)(i+1)*SegmF;
			if ( InSector(F1) && InSector(F2) ){
				GPS.DrawLine(p1,p2,color);	
			};
			if ( InSector(F2) && !InSector(F1) ){
				GPS.DrawLine(p2,SEN,color);	
			};
			if ( InSector(F1) && !InSector(F2) ){
				GPS.DrawLine(SEN,p1,color);	
			};
		}else{		
			GPS.DrawLine(p1,p2,color);
		};

		if (i==0&&SegmFR<=0)		p0.set(p1.x,p1.y,p1.z);
		if (i==SegmD-1&&SegmFR<=0)	GPS.DrawLine(p0,p2,color);
	};

	// Draw Direction
	Vector3D ForDir(	fx+(fR+50.f)*cosf(fDir),
						fy+(fR+50.f)*sinf(fDir),
						max((float)GetHeight((int)(fx+50.f*cosf(fDir)),(int)(fy+50.f*sinf(fDir))),0.0f)
				   );
	GPS.DrawLine(SEN,ForDir,color);
};
void	be_CNODE::vDrawXY(DWORD pos/* =0 */,int dy/* =10 */,int dx/* =10 */,int r/* =100 */){
	int txX,txY;
	GetTextCoordLT(txX,txY);

	char color[56];
	sprintf(color,"%s%x%s","{C 0x",EngSettings.MIS_SET.NodeNameColor,"}");

	char	XY[100];
	sprintf(XY,"%s%s%d%s%d%s",color,"X( ",x," ) : Y( ",y," )");
	extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
	ShowStringEx(txX,txY+pos*10,XY,pNODE_FONT);
};
void	be_CNODE::vDrawNAME(DWORD pos/* =1 */,int dy/* =30 */,int dx/* =10 */){
	int txX,txY;
	GetTextCoordLT(txX,txY);

	char color[56];
	sprintf(color,"%s%x%s","{C 0x",EngSettings.MIS_SET.NodeNameColor,"}");

	char	name[100];
	sprintf(name,"%s%s",color,Name.pchar());
	extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
	ShowStringEx(txX,txY+pos*10,name,pNODE_FONT);
};
void	be_CNODE::vDraw(bool forceDraw/*=false*/){
	DWORD mode = DriveMode()->NodeMode();
	int pos = 0;

	if ( (!forceDraw) && (!IsOnScreen(x,y,150,150)) )	return;

	if (DriveMode()->NodeStyle==0) {
		if (mode&_RECT_){
			vDrawRect();
			vDrawCircle();
		};
		if (Shifter==5){
			if (mode&_XY_)	  { vDrawXY(pos++); vDrawID(); };
			if (mode&_NAME_)	vDrawNAME(pos++);
		};
	}else if (DriveMode()->NodeStyle==1) {
		if (mode&_RECT_){
			vDrawBuoy();
			vDrawCircle();
		};
		if (Shifter==5) {
			if (mode&_NAME_)	vDrawNAME(pos);
			if (mode&_XY_)	  { vDrawID(); };
		};
	};
};
extern	int ADDSH;
int GetMapSize(){
	return	( 240 << ADDSH )*32;
};
void	be_CNODE::vSetX(int _x){
	//int	DX = 100;
	//if (DX+50<_x&&_x<(GetMapSize()-DX)) {
	//	x = _x;
	//};

	x = _x;
};
void	be_CNODE::vSetY(int _y){
	//int	DY = 150;
	//if (DY+200<_y&&_y<(GetMapSize()-DY)) {
	//	y = _y;
	//};

	y = _y;
};
void	be_CNODE::vSetXY(int _x,int _y){
	vSetX(_x);
	vSetY(_y);
};
void	be_CNODE::vSetR(int _R){
	if (0<=_R)	R=_R;
};
void	be_CNODE::vSetDir(int _Dir){
	if (0<=_Dir&&_Dir<=255)	Dir=_Dir;
};
void	be_CNODE::vSetSegmFR	(int _SegmFR){
	if (0<=_SegmFR&&_SegmFR<=255)	SegmFR=_SegmFR;
};
void	be_CNODE::vSetGParam	(int _x,int _y,int _R,int _Dir,int _SegmFR){
	vSetXY		(_x,_y);
	vSetR		(_R);
	vSetDir		(_Dir);
	vSetSegmFR	(_SegmFR);
};

void	be_CNODE::vAddX(int dx/* =1 */){
	int	DX = 100;

	x += dx;
	if (x<DX+50&&(GetMapSize()-DX)<x) {
		x -= dx;
	};
};
void	be_CNODE::vAddY(int dy/* =1 */){
	int	DY = 150;

	y += dy;
	if (y<DY+200&&(GetMapSize()-DY)<y) {
		y -= dy;
	};
};
void	be_CNODE::vAddXY(int dx/* =1 */,int dy/* =1 */){
	vAddX(dx);
	vAddY(dy);
};
float	be_CNODE::vGetNodeDist(int _x,int _y){
	return	sqrtf((float)(x-_x)*(float)(x-_x)+(float)(y-_y)*(float)(y-_y));
};
int		be_CNODE::vGetX()		const{
	return x;
};
int		be_CNODE::vGetY()		const{
	return y;
};
int		be_CNODE::vGetR()		const{
	return R;
};
int		be_CNODE::vGetDir()		const{
	return Dir;
};
int		be_CNODE::vGetSegmFR()	const{
	return SegmFR;
};
// be_CBLANK /////////////////////////////////////////////////////////////
be_CBLANK::be_CBLANK() : be_CNODE(){
	g_beOBJ_CONTROLER.SetState("be_CBLANK");
	R = 150;
	Dir = 0;
	SegmFR = 250;
};
be_CBLANK::be_CBLANK(const char* _Name,const int _x,const int _y) : be_CNODE(_Name,_x,_y){
	g_beOBJ_CONTROLER.SetState("be_CBLANK");
	R = 150;
	Dir = 0;
	SegmFR = 250;
};
be_CBLANK::~be_CBLANK(){
	UT_LIST.Clear();
	g_beOBJ_CONTROLER.SetState("be_CBLANK");
};
void	be_CBLANK::vDraw(){
	DWORD mode = DriveMode()->NodeMode();
	if (mode&_RECT_){
		Matrix3D	rotZ; rotZ.rotation(Vector3D(0.f,0.f,1.f),((float)Dir/256.f)*3.145f*2.f);
		Matrix4D	tm(		Vector3D((float)R/1.41f,(float)R/1.41f,SegmFR), 
							rotZ, 
							Vector3D(x,y,GetHeight(x,y)+SegmFR/2) 
					  );
		rsFlush();
		IRS->SetWorldTM(tm);
		DrawCube( Vector3D::null, EngSettings.MIS_SET.NodeLineColor, 1.f );
		rsFlush();
		IRS->SetWorldTM(Matrix4D::identity);
		
		if (mode&_NAME_){
			float tx = (float)x-(float)R/(1.41f*2.f)+5.f;
			float ty = (float)y+(float)R/(1.41f*2.f);
			Vector3D v( tx, ty, GetHeight(tx,ty)+SegmFR-5 );
			WorldToScreenSpace(v);
			DrawUT_List( v.x, v.y );
		};
	};
};
void	be_CBLANK::DrawUT_List(int x0,int y0){
	extern	void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf);
	char color[56];
	sprintf(color,"%s%x%s","{C 0x",EngSettings.MIS_SET.NodeNameColor,"}");
	char	ONE_UT[100];
	for ( int i=0; i<UT_LIST.GetAmount(); i++ ){
		if ( NATIONS->Mon[ UT_LIST[i] ]->MonsterID && NATIONS->Mon[ UT_LIST[i] ]->MonsterID[0]!=0 ){
			sprintf(ONE_UT,"%s%s",color,NATIONS->Mon[ UT_LIST[i] ]->MonsterID);
			ShowStringEx(x0,y0+i*10,ONE_UT,pNODE_FONT);		
		};
	};
};
// be_CUnitType //////////////////////////////////////////////////////////
const	char*	be_CUnitType::GetThisElementView(const char* LoaclName){
	if (LoaclName!=NULL){
		static _str	TypeName = "NONAME";
		if ( GetValueAsSTR(TypeName) ) return TypeName.pchar();
	};
	return LoaclName;		
};
bool	be_CUnitType::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="";
	BaseClass* pPtr=(BaseClass*)DataPtr;
	if( pPtr && pPtr->CheckDirectCasting() ){
		return ((be_CUnitType*)pPtr)->GetValueAsSTR(dest);
	};
};
bool	be_CUnitType::GetValueAsSTR(_str& ValueSTR){
	pUnitType _UT(Value);
	ValueSTR = _UT.GetFullName();
	return g_beCheckChar(ValueSTR.pchar());
};
//////////////////////////////////////////////////////////////////////////
// SMART GROUP ///////////////////////////////////////////////////////////
// be_CMSG
be_CMSG::be_CMSG(){
	name		= "";
	sender_id	= "";
	receiver_id = "";
	delivery_time = 0;	
};
be_CMSG::be_CMSG( const char* _name, const char* _sender_id, const char* _receiver_id ){
	name		= _name;
	sender_id	= _sender_id;
	receiver_id = _receiver_id;
	delivery_time = 0;
};
be_CMSG::be_CMSG( const char* _name, const int _sender_id, const int _receiver_id ){
	name		= _name;
	delivery_time = 0;

	be_CSMARTOBJECT* pSO = g_beGetSMARTOBJECT(_sender_id);
	if (pSO!=NULL){
		sender_id	= pSO->Name;
		sender_ref.Set(pSO);
	};

	pSO = NULL;
	pSO = g_beGetSMARTOBJECT(_receiver_id);
	if (pSO!=NULL){
		receiver_id	= pSO->Name;
		receiver_ref.Set(pSO);
	};
	
};
be_CMSG::be_CMSG( be_CMsgObject* pmsg ){
	if (pmsg!=NULL) {
		name			= pmsg->name;
		sender_id		= pmsg->sender_id;
		receiver_id		= pmsg->receiver_id;	
		delivery_time	= pmsg->delivery_time;
	};
};
char*	be_CMSG::GetMSGName() const{
	return (char*)name.cpchar();	
};
void	be_CMSG::SetMSGName(const char* _Name){
	if ( _Name!=NULL && _Name[0]!=0 ){
		name = _Name;
	};
};
const char* be_CMSG::GetSender() const{
	return (char*)sender_id.cpchar();	
};
const char* be_CMSG::GetReceiver() const{
	return (char*)receiver_id.cpchar();
};
void	be_CMSG::ADD_PARAM_REF(const char* _type,const char* _name,const char* _valueName){
	SomeData.AddParam(_type,_name,false);
	be_CBasePrametr* pPAR = SomeData.GetParamObj(_name);
	pPAR->UniveraslParam.GlobalReference.ClearRef();
	pPAR->UniveraslParam.GlobalReference.Assign( (char*)_valueName );
}
void	be_CMSG::ADD_PARAM	(const char* _name, const bool			_value){
	be_CBOOL* pnewPAR = SomeData.CreateParam_BOOL(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
void	be_CMSG::ADD_PARAM	(const char* _name, const int			_value){
	be_CINT* pnewPAR = SomeData.CreateParam_INT(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
void	be_CMSG::ADD_PARAM	(const char* _name, const DWORD			_value){
	be_CDWORD* pnewPAR = SomeData.CreateParam_DWORD(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
void	be_CMSG::ADD_PARAM	(const char* _name, const float			_value){
	be_CFLOAT* pnewPAR = SomeData.CreateParam_FLOAT(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
void	be_CMSG::ADD_PARAM	(const char* _name, const char*			_value){
	be_CSTR* pnewPAR = SomeData.CreateParam_STR(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
// be_CMSG_FROMUSER
be_CMSG_FROMUSER::be_CMSG_FROMUSER(){
	g_beOBJ_CONTROLER.SetState("be_CMSG_FROMUSER");
	_str l_ClassName = "";
	if ( GetClassName() ) l_ClassName = GetClassName();

	Name = "";
	if (g_beCheckChar(l_ClassName.pchar()))
		Name = l_ClassName;
    else 
		Name = "NOCLASSNAME";

	Name += " ";
	Name += (int)GetObjectGlobalID();
	if (GetClassMask()==0x00000002){
		SYSTEMTIME	stime;
		GetSystemTime( &stime );
		Name += " ";
		Name += stime.wYear;
		Name += ":";
		Name += stime.wMonth;
		Name += ":";
		Name += stime.wDay;
		Name += ":";
		Name += stime.wHour;
		Name += ":";
		Name += stime.wMinute;
		Name += ":";
		Name += stime.wSecond;
	};
};
be_CMSG_FROMUSER::~be_CMSG_FROMUSER(){
	g_beOBJ_CONTROLER.SetState("be_CMSG_FROMUSER");
};
void	be_CMSG_FROMUSER::ADD_PARAM	(const char* _name, const bool			_value){
	be_CBOOL* pnewPAR = SomeData.CreateParam_BOOL(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
void	be_CMSG_FROMUSER::ADD_PARAM	(const char* _name, const int			_value){
	be_CINT* pnewPAR = SomeData.CreateParam_INT(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
void	be_CMSG_FROMUSER::ADD_PARAM	(const char* _name, const DWORD			_value){
	be_CDWORD* pnewPAR = SomeData.CreateParam_DWORD(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
void	be_CMSG_FROMUSER::ADD_PARAM	(const char* _name, const float			_value){
	be_CFLOAT* pnewPAR = SomeData.CreateParam_FLOAT(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
void	be_CMSG_FROMUSER::ADD_PARAM	(const char* _name, const char*			_value){
	be_CSTR* pnewPAR = SomeData.CreateParam_STR(_name);
	if (pnewPAR!=NULL) pnewPAR->SetValue(_value);
};
bool	be_CMSG_FROMUSER::GET_PARAM	(const char* _name, bool	_value){
	be_CBasePrametr* pPAR = SomeData.GetParamObj(_name);
	if ( strcmp(pPAR->GetParamType(),"be_CBOOL")==0 ){
		_value = ((be_CBOOL*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue();
		return true;
	};
	return false;
};
bool	be_CMSG_FROMUSER::GET_PARAM	(const char* _name, int		_value){
	be_CBasePrametr* pPAR = SomeData.GetParamObj(_name);
	if ( strcmp(pPAR->GetParamType(),"be_CINT")==0 ){
		_value = ((be_CINT*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue();
		return true;
	};
	return false;
};
bool	be_CMSG_FROMUSER::GET_PARAM	(const char* _name, DWORD	_value){
	be_CBasePrametr* pPAR = SomeData.GetParamObj(_name);
	if ( strcmp(pPAR->GetParamType(),"be_CDWORD")==0 ){
		_value = ((be_CDWORD*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue();
		return true;
	};
	return false;
};
bool	be_CMSG_FROMUSER::GET_PARAM	(const char* _name, float	_value){
	be_CBasePrametr* pPAR = SomeData.GetParamObj(_name);
	if ( strcmp(pPAR->GetParamType(),"be_CFLOAT")==0 ){
		_value = ((be_CFLOAT*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue();
		return true;
	};
	return false;
};
bool	be_CMSG_FROMUSER::GET_PARAM	(const char* _name, _str&	_value){
	be_CBasePrametr* pPAR = SomeData.GetParamObj(_name);
	if ( strcmp(pPAR->GetParamType(),"be_CSTR")==0 ){
		_value = ((be_CSTR*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue();
		return true;
	};
	return false;
};
void	be_CMSG_FROMUSER::CGetFromExample::EvaluateFunction(){
	be_CMSG_FROMUSER* pExMSG = (be_CMSG_FROMUSER*)(Example.Get());
	be_CMSG_FROMUSER* pPrMSG = (be_CMSG_FROMUSER*)(GetParent());
	if (pExMSG!=NULL&&pPrMSG!=NULL) {
		pPrMSG->msgName = pExMSG->msgName;
		// copy params
		pPrMSG->SomeData.CreateCopyFrom( pExMSG->SomeData );
	};
	return;
};
void g_beRouteMessage		( be_CMSG_FROMUSER* msg ){
	be_CMSG Msg(msg->msgName.pchar(),msg->sender_id.pchar(),msg->receiver_id.pchar());
	int N = msg->SomeData.GetAmount();
	be_CBasePrametr* pPAR = NULL;
	for ( int i=0; i<N; i++ ){
		pPAR = (be_CBasePrametr*)(msg->SomeData[i]);
		if (pPAR) {
			if (pPAR->UniveraslParam.Local==false){
				Msg.ADD_PARAM_REF(pPAR->GetParamType(),pPAR->ParamName.pchar(),pPAR->UniveraslParam.GlobalReference.pchar());
			}else{
				if			( strcmp(pPAR->GetParamType(),"be_CBOOL"	)==0 ){
					Msg.ADD_PARAM(	pPAR->ParamName.pchar(),
									((be_CBOOL*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue()
								 );
				}else if	( strcmp(pPAR->GetParamType(),"be_CDWORD")==0 ){
					Msg.ADD_PARAM(	pPAR->ParamName.pchar(),
									((be_CDWORD*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue()
								 );
				}else if	( strcmp(pPAR->GetParamType(),"be_CINT"	)==0 ){
					Msg.ADD_PARAM(	pPAR->ParamName.pchar(),
									((be_CINT*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue()
								 );
				}else if	( strcmp(pPAR->GetParamType(),"be_CFLOAT")==0 ){
					Msg.ADD_PARAM(	pPAR->ParamName.pchar(),
									((be_CFLOAT*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue()
								 );
				}else if	( strcmp(pPAR->GetParamType(),"be_CSTR"	)==0 ){
					Msg.ADD_PARAM(	pPAR->ParamName.pchar(),
									((be_CSTR*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue()
								 );
				}else if	( strcmp(pPAR->GetParamType(),"be_CUnitType"	)==0 ){
					Msg.ADD_PARAM(	pPAR->ParamName.pchar(),
									((be_CINT*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue()
								 );
				}else if	( strcmp(pPAR->GetParamType(),"be_CFurmationType")==0 ){
					Msg.ADD_PARAM(	pPAR->ParamName.pchar(),
									((be_CINT*)(pPAR->UniveraslParam.LocalObject.Get()))->GetValue()
								 );
				};
			};
		};
	};
	g_beRouteMessage( (be_CMsgObject*)(&Msg) );
	return;	
};
// be_CFSM
be_CFSM::be_CFSM(){
	g_beOBJ_CONTROLER.SetState("be_CFSM");
	pDLL_FSM_FN = NULL;
	SetDefName();
};
be_CFSM::~be_CFSM(){
	g_beOBJ_CONTROLER.SetState("be_CFSM");
};
void	be_CFSM::LoadLuaFiles(){
	BattleHandler()->LUA_LoadFile(FileName.pchar());
};
void	be_CFSM::FSMInitialize( be_CSMARTOBJECT* pSO ){
	BattleHandler()->LUA_LoadFile(FileName.pchar());
	if (pSO!=NULL){
		pSO->force_state_change		= false;
		pSO->stateName				= "ST_GLOBAL";
		pSO->next_stateName			= "ST_GLOBAL";
		{	//Initialize the state machine by sending the first Initialize msg
			be_CMSG* pmsg = new be_CMSG("MSG_ENTER",pSO->Name.pchar(),pSO->Name.pchar());
			pmsg->sender_ref.Set(pSO);
			pmsg->receiver_ref.Set(pSO);
			g_beRouteMessage( (be_CMsgObject*)pmsg );
			delete pmsg;
		};
	};
};
typedef int (*CALL_FSM_FN)(pSMARTOBJECT&,_str&,pMSG&);
int		be_CFSM::DLL_ProcessStateMachine( be_CSMARTOBJECT* pSO, char* State, be_CMsgObject* msg ){
	if ( pDLL_FSM_FN!=NULL ){
		CALL_FSM_FN pCallDLL_FN = ( CALL_FSM_FN )pDLL_FSM_FN;
		return pCallDLL_FN( pSMARTOBJECT(pSO),_str(State),pMSG(msg) );
	};
	return 0;
};
void UnitAbilityTest();
bool	be_CFSM::DLL_VAILID(){
	if (pDLL_FSM_FN==NULL){
		void* FindFunctionInPermanentDLLs(const char* Name);
		static _str	decorName;
		decorName  = "?";
		decorName += FunctionName;
		decorName += "@@YAHAAVpSMARTOBJECT@@AAV_str@@AAVpMSG@@@Z";
		pDLL_FSM_FN = FindFunctionInPermanentDLLs(decorName.pchar());
	};
	return (pDLL_FSM_FN!=NULL);	
};
extern WholeClassPresentation CE_WCP;
bool	be_CFSM::ProcessStateMachine( be_CSMARTOBJECT* pSO, char* State, be_CMsgObject* msg ){
	int retVAL = false;
	be_CMSG* pMSG = NULL;
	bool needDELETE = false;
	if ( msg->GetType()==0 ){
		pMSG = new be_CMSG(msg);
		needDELETE = true;
	}else{
		pMSG = (be_CMSG*)(msg);
	};

	if ( DLL_VAILID()==true ){
		//UnitAbilityTest();
//		retVAL = DLL_ProcessStateMachine(pSO,State,msg);

//		try{
			retVAL = DLL_ProcessStateMachine(pSO,State,msg);
		//}catch (...) {
  //          DriveMode()->PROCESS=false;
		//	_str	_error_data;
		//	// some information about crashed SO
		//	// --- FSM --- //
  //          _error_data = "--- FSM ---\n";
		//		// FSM		- FSM name
		//	if (pSO) _error_data += pSO->state_machine.pchar();
		//	else	 _error_data += this->Name;
		//	_error_data += "\n\n";
		//	// ----------- //

  //          // --- MSG --- //
		//	_error_data += "--- MSG ---\n";
		//		// MSG		- msg name
		//	if (msg)	_error_data += msg->name;
		//	else		_error_data += "UNKNOWN MSG";
		//	_error_data += "\n\n";
		//	// ----------- //

		//	if ( pSO ){
		//		// --- SO  --- //
		//		_error_data += "--- SO ---\n";
		//			// SO		- SO name
		//		_error_data += "Name    -  ";
		//		_error_data += pSO->Name;
		//		_error_data += "\n\n";
		//			// STATE	- current SO state
		//		_error_data += "State   -  ";
		//		_error_data += pSO->stateName;
		//		_error_data += "\n\n";
		//			// OBJECT	- SO controled object name
		//		_error_data += "Object  -  ";
		//		_error_data += pSO->ControledObject;
		//		_error_data += "\n\n";
		//			// PARAMS	- SO params
		//		_error_data += "Params  -  \n";
		//		_str _params_data;
		//		pSO->SomeData._string(_params_data);
		//		_error_data += _params_data;
		//		// ----------- //
		//	}else{
		//		_error_data += "UNKNOWN SO";
		//	}

		//	MessageBox( NULL, _error_data.pchar(), "SCRIPT ERROR < FSM >", MB_OK );
		//};

		//UnitAbilityTest();
	}else{
#ifdef	__LUA__
		if ( BattleHandler()->LUA_MISS.LUA_STATE!=NULL ){
			try{
				retVAL = luabind::call_function<int>(	BattleHandler()->LUA_MISS.LUA_STATE, 
														FunctionName.pchar(),
														pSO,
														State,
														pMSG
													);
			}catch (...) {
				//DriveMode()->PROCESS=false;
				//BattleHandler()->LUA_SAFE_CLOSE_MISS();
				_str errTOPIC	= "ERROR : Call Function from FSM, SO - ";
				errTOPIC += pSO->GetName();
				_str errMSG		= FunctionName.pchar();
				//MessageBox( NULL, errMSG.pchar(), errTOPIC.pchar(), MB_OK );
				Log.Warning(" %s : %s",errTOPIC.pchar(),errMSG.pchar());
				AssignHint1(errTOPIC.pchar(),100);
			};
		};
#endif//__LUA__
	};
					
	if (needDELETE)	delete pMSG;
	return	( (retVAL==0) ? (false) : (true) );
};
// be_CSMARTOBJECT
be_CSMARTOBJECT::be_CSMARTOBJECT(){
	g_beOBJ_CONTROLER.SetState("be_CSMARTOBJECT");
	SetDefName();
};
be_CSMARTOBJECT::~be_CSMARTOBJECT(){
	g_beOBJ_CONTROLER.SetState("be_CSMARTOBJECT");
};
void	be_CSMARTOBJECT::FSMInitialize(){
	if ( ControledObject.Get()==NULL ) return;	
	be_CFSM* pFSM = (be_CFSM*)state_machine.Get();
	if (pFSM!=NULL) {
		pFSM->FSMInitialize( (be_CSMARTOBJECT*)this );
	};
};
void	be_CSMARTOBJECT::Update(){
	if ( ControledObject.Get()==NULL ) return;
	//////////////////////////////////////////////////////////////////////////	
//	be_CGROUP* pGrp = dynamic_cast<be_CGROUP*>(ControledObject.Get());
//	if ( pGrp ) pGrp->SetAIGuard();
	//////////////////////////////////////////////////////////////////////////
	static be_CMSG msg("MSG_UPDATE","","");	
	msg.sender_id = Name.pchar();
	msg.receiver_id = Name.pchar();
	msg.delivery_time = 0;
	be_CMSG* pmsg = &msg;
	//new be_CMSG("MSG_UPDATE",Name.pchar(),Name.pchar());
	pmsg->sender_ref.Set(this);
	pmsg->receiver_ref.Set(this);
	g_beRouteMessage( pmsg );
	//delete pmsg;
};
bool	be_CSMARTOBJECT::ProcessStateMachine( char* State, be_CMsgObject* msg ){
	if ( ControledObject.Get()==NULL ) return false;
	be_CFSM* pFSM = (be_CFSM*)state_machine.Get();
	if (pFSM!=NULL) {
		return pFSM->ProcessStateMachine( (be_CSMARTOBJECT*)this, State, msg );
	};
	return false;
};
void	be_CSMARTOBJECT::vDraw(){
	if (ControledObject.Get()!=NULL)	((be_CVariableBase*)ControledObject.Get())->vDraw();
	SomeData.vDraw();
};
bool	be_CSMARTOBJECT::SET_OBJECT(const char* _name){
	if ( g_beCheckChar(_name)==false ) return false;
	_str _NAME = _name;
	ControledObject.SetRef(_NAME.pchar(),0xFFFFFFFF);
	return true;
};
bool	be_CSMARTOBJECT::SET_OBJECT(pGroup& _group){
	if ( _group.Valid() ){
		be_CGROUP* pGR = _group.group();
		if ( pGR!=NULL ){
			ControledObject.SetRef( pGR->GetName(), pGR->GetObjectGlobalID() );
		};
	}
	return true;	
};
void	be_CSMARTOBJECT::CREATE_PRAM_FROM_FSM(){
	be_CFSM* pFSM = (be_CFSM*)state_machine.Get();
	if (pFSM!=NULL) SomeData.CreateCopyFrom( pFSM->Params );
};
void	be_CSMARTOBJECT::CCreateParams::EvaluateFunction(){
	be_CSMARTOBJECT* pPR_SO = (be_CSMARTOBJECT*)GetParent();
	if (pPR_SO!=NULL) pPR_SO->CREATE_PRAM_FROM_FSM();
};
be_CSMARTOBJECT* g_beGetSMARTOBJECT(const char* _name){
	be_CSMARTOBJECT* pSO = NULL;
	if ( g_beCheckChar(_name) ){
		int N = g_beVAR_STORE.SMARTOBJ_LIST.GetAmount();	
		while (pSO==NULL && N--) {
			pSO = g_beVAR_STORE.SMARTOBJ_LIST[N];
			if ( strcmp(_name,pSO->Name.pchar())!=0 ) pSO=NULL;
		};
	};
	return pSO;
};
be_CSMARTOBJECT* g_beGetSMARTOBJECT(const int  _OB_Index){
	be_CSMARTOBJECT*	pSO = NULL;
	int N = g_beVAR_STORE.SMARTOBJ_LIST.GetAmount();
	be_CVariableBase* pGRP = NULL;
	for ( int i=0; i<N; i++ ){
		pGRP = (be_CVariableBase*)(g_beVAR_STORE.SMARTOBJ_LIST[i]->ControledObject.Get());
		if ( pGRP!=NULL && strcmp(pGRP->GetClassName(),"be_CGROUP")==0 ) {
			if ( ( (be_CGROUP*)pGRP )->_CheckGID(_OB_Index)!=0xFFFF ) pSO = g_beVAR_STORE.SMARTOBJ_LIST[i];
		};
	};
	return pSO;
};
be_CSMARTOBJECT* g_beGetSMARTOBJECT(be_CVariableBase*	pVAR){
    be_CSMARTOBJECT*	pSO		= NULL;
	if ( pVAR && g_beCheckChar(pVAR->Name.pchar()) ){
		be_CVariableBase*	_pVAR	= NULL;
		int N = g_beVAR_STORE.SMARTOBJ_LIST.GetAmount();
		for ( int i=0; (i<N)&&(pSO==NULL); i++ ){
			_pVAR = g_beVAR_STORE.SMARTOBJ_LIST[i]->GET_OBJECT();
			if ( _pVAR && g_beCheckChar(_pVAR->Name.pchar()) && _pVAR->Name==pVAR->Name ){
				pSO = g_beVAR_STORE.SMARTOBJ_LIST[i];
			}
		};
	};
	return pSO;
}
int smartobj_curPos = 0;

void			 g_beUPDATESMARTOBJ(){
	// Upadte not more then 20 bject in one circle	
	int N = g_beVAR_STORE.SMARTOBJ_LIST.GetAmount();
	int N10 = max(N/5,20);
	for ( int i=smartobj_curPos; (i<N)&&(N10>0); i++ ){
		addrand(i);
		g_beVAR_STORE.SMARTOBJ_LIST[i]->Update();
		smartobj_curPos++;
		N10--;
	};
	if (smartobj_curPos>=N) smartobj_curPos=0;
	// send delayed messages
	g_beSendDelayedMessages();
};
void			 g_beINITSMARTOBJECTS(){
	smartobj_curPos=0;
	int N = g_beVAR_STORE.SMARTOBJ_LIST.GetAmount();
	while (N--) {
		g_beVAR_STORE.SMARTOBJ_LIST[N]->FSMInitialize();
	};
};
// ENUMERATORS ///////////////////////////////////////////////////////////
DWORD fn_BE_PAR_STATE_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int NC = 0;
/*
	if ( in->Context!=NULL ) {
		BaseClass* pBASE = dynamic_cast<BaseClass*>(in->Context);
		void* DataPtr;
		void *ExtraPtr;
		BaseClass* ClassPtr;
		if(pBASE->FindElementByName("state_machine",&ClassPtr,&DataPtr,&ExtraPtr,pBASE)){
			_str className = ClassPtr->GetClassName();
			if(strcmp(ClassPtr->GetClassName(),"_strindex")==0){
				_str_ref* pFSM_ref = (_str_ref*)DataPtr;
				if (pFSM_ref!=NULL) {
					be_CFSM* pFSM = (be_CFSM*)(pFSM_ref->Get());
					if (pFSM!=NULL) {
						NC = pFSM->STATE_LIST.GetAmount();
						for ( int i=0; i<NC; i++ ){
							out->add( pFSM->STATE_LIST[i]->pchar(),	i, NULL);
						};
					};
				};
			};
		};
	};
*/
	return NC;
};
// SUB_PROGRAM ///////////////////////////////////////////////////////////
int				be_CSUBFUNCTION::Process(){
	if (ACTIVE)	return BODY.CallFunction();	
	return 0;
};
bool			be_CSUBFUNCTION::IsMyOneObject(OneObject* pOB){
	int ArgN = InArgList.GetAmount();
	if ( ArgN==0 ) return false;
	be_CBasePrametr*	pPAR = NULL;
	be_CGROUP*			pGrp = NULL;
	bool				MY	 = false;
	while (!MY&&ArgN--) {
		pPAR = (be_CBasePrametr*)InArgList[ArgN];
		if ( pPAR && g_beCheckChar(pPAR->ParamType.pchar()) && pPAR->ParamType=="GROUP" ){
			pGrp = (be_CGROUP*)InArgList.GetParam(pPAR->ParamName.pchar());
			MY = ( pGrp!=NULL && pGrp->_CheckGPT(pOB)!=0xFFFF );
		};
	};
	return MY;
};
void			be_CSUBFUNCTION::ClearEntity(){
	ACTIVE = true;
	AbilityList.Clear();
	InArgList.CLEAR();
	BODY.clearCB();
	BODY.FirstOperand.Clear();
    INIT.clearCB();
	INIT.FirstOperand.Clear();
};
// CONVERTORS ////////////////////////////////////////////////////////////
template<class T>
T*				g_beUnivConv(be_CVariableBase* _obj){
	if (_obj==NULL) return NULL;
	return (T*)_obj;	
};
be_CBOOL*		g_beConvBOOL(be_CVariableBase* _obj){
	return g_beUnivConv<be_CBOOL>(_obj);
};
be_CDWORD*		g_beConvDWORD(be_CVariableBase* _obj){
	return g_beUnivConv<be_CDWORD>(_obj);
};
be_CINT*		g_beConvINT(be_CVariableBase* _obj){
	return g_beUnivConv<be_CINT>(_obj);
};
be_CFLOAT*		g_beConvFLOAT(be_CVariableBase* _obj){
	return g_beUnivConv<be_CFLOAT>(_obj);
};
be_CSTR*		g_beConvSTR(be_CVariableBase* _obj){
	return g_beUnivConv<be_CSTR>(_obj);
};
be_CGROUP*		g_beConvGROUP(be_CVariableBase* _obj){
	return g_beUnivConv<be_CGROUP>(_obj);
};
be_CNODE*		g_beConvNODE(be_CVariableBase* _obj){
	return g_beUnivConv<be_CNODE>(_obj);
};
be_CARRAY*		g_beConvARRAY(be_CVariableBase* _obj){
	return g_beUnivConv<be_CARRAY>(_obj);
};
be_CSTR_DIALOG*		g_beConvSTR_DIALOG(be_CVariableBase* _obj){
	return g_beUnivConv<be_CSTR_DIALOG>(_obj);
};
UnitAbility*		g_beConvABIL(be_CVariableBase* _obj){
	return g_beUnivConv<UnitAbility>(_obj);
};
WeaponModificator*	g_beConvWEAPONMOD(be_CVariableBase* _obj){
	return g_beUnivConv<WeaponModificator>(_obj);
};
// CALSS REGISTRATION ////////////////////////////////////////////////////
void be_REG_CLASS_Variables(){
	REG_CLASS(be_CVariableBase);

	REG_CLASS(be_CBOOL);
	REG_CLASS(be_CINT);
	REG_CLASS(be_CUnitType);
	REG_CLASS(be_CFurmationType);
	REG_CLASS(be_CDWORD);
	REG_CLASS(be_CFLOAT);
	REG_CLASS(be_CSTR);
	REG_CLASS(be_CSTR_DIALOG);
	REG_CLASS(be_CCAMERA);
	REG_CLASS(be_CSUBFUNCTION);
	
	REG_CLASS(be_CGROUP);
	REG_CLASS(be_CNODE);
	REG_CLASS(be_CBLANK);

	REG_CLASS(be_CARRAY);

	REG_CLASS(be_CFSM);
	REG_CLASS(be_CSMARTOBJECT);
	REG_CLASS(ClassRef<be_CSMARTOBJECT>);

	REG_CLASS(be_CMSG_FROMUSER);
	REG_CLASS(ClassRef<be_CMSG_FROMUSER>);

	Enumerator* E=ENUM.Get("BE_PAR_STATE_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_PAR_STATE_ENUM));
};
//////////////////////////////////////////////////////////////////////////

















