#include "stdheader.h"
#include "BE_HEADERS.h"

BE_USAGE_FLAGS	g_BE_USAGE_FLAG = BE_IF_CALL;

be_CBasePrametr*	be_CParamList::OpenParam = NULL;

// be_CUniveraslParam ///////////////////////////////////////////////////
be_CUniveraslParam::be_CUniveraslParam(){
	GlobalReference.ClearRef();
};
be_CUniveraslParam::~be_CUniveraslParam(){
	LocalObject.Clear();	
};
bool	be_CUniveraslParam::OnChangeMember(BaseClass* MembClass,void* MembPtr,void* MembExtra,const char* MembName){
	if ( g_beCheckChar(MembName) && !strcmp(MembName,"GlobalReference") ) {
		if ( g_beCheckChar(MembClass->GetClassName()) && !strcmp(MembClass->GetClassName(),"_strindex") ){
			BaseClass* pBC = (BaseClass*)MembPtr;
			if (pBC&&g_beCheckChar(pBC->GetClassName())&& !strcmp(pBC->GetClassName(),"_str_ref")) {
				((_str_ref*)MembPtr)->ClearRef();
				return false;
			}
		}
	};
	return false;
};
void	be_CUniveraslParam::PrepareParam(bool forCreateENUM/*=false*/){
	if ( Local==true ) {
		be_CVariableBase* plVAR = LocalObject.Get();
		if (plVAR!=NULL) {
			//GlobalReference.SetRef( plVAR->Name.pchar(), plVAR->GetObjectGlobalID() );
			GlobalReference.ID = plVAR->GetObjectGlobalID();
		};
	}else if ( Local==false && forCreateENUM ){
		if ( forCreateENUM && GlobalReference.ID==0xFFFFFFFF && g_beCheckChar(GlobalReference.pchar()) ){
			be_CBasePrametr* pBP = (be_CBasePrametr*)GetParent();
			if ( pBP==NULL ) return;
			int B = CPGARB.GetBranch(pBP->GetParamType());
		//	for(int B=0;B<nCP_Branches;B++){
				DynArray<OneClassPointer*>& CPA=CPGARB.CPointer[B];
				int TCN = CPA.GetAmount();
				ReferableBaseClass* pRefOB = NULL;
				while (TCN--) {
					OneClassPointer* CP=CPA[TCN];
					if (CP!=NULL&&CP->ClassPtr!=NULL){
						pRefOB = (ReferableBaseClass*)CP->ClassPtr;
						if ( g_beCheckChar(pRefOB->Name.pchar())&&strcmp(pRefOB->Name.pchar(),GlobalReference.pchar())==0){
							CPGARB.AddRef(TCN+(B<<24));
							GlobalReference.ID = CP->ClassPtr->GetObjectGlobalID();
							return;
						}
					}
				}
		//	}

			// TOTAL SEARCH
			for(int B=0;B<nCP_Branches;B++){
				DynArray<OneClassPointer*>& CPA=CPGARB.CPointer[B];
				int TCN = CPA.GetAmount();
				ReferableBaseClass* pRefOB = NULL;
				while (TCN--) {
					OneClassPointer* CP=CPA[TCN];
					if (CP!=NULL&&CP->ClassPtr!=NULL){
						pRefOB = (ReferableBaseClass*)CP->ClassPtr;
						if ( g_beCheckChar(pRefOB->Name.pchar())&&strcmp(pRefOB->Name.pchar(),GlobalReference.pchar())==0){
							CPGARB.AddRef(TCN+(B<<24));
							GlobalReference.ID = CP->ClassPtr->GetObjectGlobalID();
							return;
						}
					}
				}
			}
		}
	}
}
void	be_CUniveraslParam::ChangeLocal(){
	SetLocal(!Local);
};
void	be_CUniveraslParam::SetLocal(bool local){
	LocalObject.Clear();
	if ( local==true ){
		_str ClassType = "";
		if (GetClassTypeFromParent(ClassType)==true) {
			OneClassStorage* pDefObj = CGARB.GetClass(ClassType.pchar());
			if ( pDefObj!=NULL && pDefObj->OneMemb!=NULL ) {
				BaseClass* pNewObj = pDefObj->OneMemb->new_element();
				if (pNewObj!=NULL) {

					// Fill ClassPtr
					LocalObject.Set((be_CVariableBase*)pNewObj);
					((be_CVariableBase*)pNewObj)->SetDefName();
					// Fill Reference
					GlobalReference.SetRef(	((be_CVariableBase*)pNewObj)->Name.pchar(),
											((be_CVariableBase*)pNewObj)->GetObjectGlobalID()
						   				  );

					Local = local;
					return;
				};
			};
		}else{
			Local = false;
			return;
		};
	}else{
		Local = local;
		return;
	};
};
bool	be_CUniveraslParam::GetClassTypeFromParent(_str& ClassName){
	BaseClass* pPar = GetParent();
	while (pPar!=NULL) {
		void* DataPtr;
		void *ExtraPtr;
		BaseClass* ClassPtr;
		if(pPar->GetElementByName("ParamType",&ClassPtr,&DataPtr,&ExtraPtr,pPar)){
			if(!strncmp(ClassPtr->GetClassName(),"_str",4)){
				_str* s=(_str*)DataPtr;
				char* c=s->pchar();
				if(g_beCheckChar(c)){
					ClassName = "be_C";
					ClassName += c;
					return true;
				};
			};
		};
		pPar = pPar->GetParent();
	};
	return false;
};
const	char*	be_CUniveraslParam::CLocalGlobal::GetThisElementView(const char* LocalName){
	be_CUniveraslParam* pUP = (be_CUniveraslParam*)GetParent();
	char*				pBUFF = GetGlobalBuffer();
	if (pUP&&pBUFF) {
		if ( pUP->Local )	sprintf(pBUFF,"%s","Use LOCAL OBJECT");
		else				sprintf(pBUFF,"%s","Use Reference to GLOBAL OBJECT");
		return pBUFF;
	};
	return LocalName;
};
void			be_CUniveraslParam::CLocalGlobal::EvaluateFunction(){
	be_CUniveraslParam* pUP = (be_CUniveraslParam*)GetParent();
	if (pUP!=NULL)	pUP->ChangeLocal();
};
DWORD			be_CUniveraslParam::GetClassMask(){

	if ( Local==true )	
		return 0x00000002;

	if ( Local==false && GetParentUSAGE()!=BE_IF_EDIT )	
		return	0x00000001;

	return 0x80000000;
};
BE_USAGE_FLAGS	be_CUniveraslParam::GetParentUSAGE(){
	be_CPrametrBase* pBP = (be_CPrametrBase*)GetParent();	
	if (pBP!=NULL) {
		return pBP->GetParentUSAGE();
	};
	return BE_IF_CALL;
};
// be_CPrametrBase //////////////////////////////////////////////////////
const	char*	be_CPrametrBase::GetThisElementView(const char* LocalName){

	char* pText = GetGlobalBuffer();
	if (pText==NULL)	return LocalName;
	if ( g_beCheckChar(ParamName.pchar())==false || g_beCheckChar(ParamType.pchar())==false ) return LocalName;
	sprintf( pText, "%s%s%s%s", "{C ff00FFFF}", ParamName.pchar(), " {C ff00FF00}: class ", ParamType.pchar() );

	return pText;

};
BE_USAGE_FLAGS	be_CPrametrBase::GetParentUSAGE(){
	be_CParamList* pPL = (be_CParamList*)GetParent();	
	if (pPL!=NULL) {
		return pPL->GetParentUSAGE();
	};
	return BE_IF_CALL;
};
DWORD			be_CPrametrBase::GetClassMask(){
	DWORD	MASK = 0x00000000;
	BE_USAGE_FLAGS	USAGE_FROM_PARENT = GetParentUSAGE();
	switch( USAGE_FROM_PARENT ) {
	case BE_IF_EDIT:
		MASK	= 0x00000001;
		break;
	case BE_IF_USE:
		MASK	= 0x00000002;
		break;
	case BE_IF_CALL:
		MASK	= 0x00000003;
		break;
	default:
		MASK	= 0xFFFFFFFF;
	};
	return MASK;
};
char	be_CPrametrBase::sPARNAME[256];
char*			be_CPrametrBase::GetParamType(){
	sprintf(sPARNAME,"%s%s","be_C",ParamType.pchar());
	return sPARNAME;
};
bool			be_CPrametrBase::SetParamName(const char* par_name){
	if ( g_beCheckChar(par_name) ){
		ParamName = par_name;
		return true;
	};
	return false;
};
bool			be_CPrametrBase::SetParamType(const char* par_type){
	if ( g_beCheckChar(par_type) ){
		if ( strncmp(par_type,"be_C",4)==0 )	ParamType = &(par_type[4]);
		else									ParamType = (char*)par_type;
		return true;
	};
	return false;
};
bool			be_CPrametrBase::SetParam(const char* par_name, const char* par_type){
	return ( SetParamName(par_name) && SetParamType(par_type) );
};
void			be_CPrametrBase::UpdateEnumForOpenParam(){
	BaseClass* pBASE = GetParent();
	_str	strCN = pBASE->GetClassName();
	if ( g_beCheckChar(strCN.pchar()) && strCN=="ClassArray.be_CPrametrBase" ){
		((be_CParamList*)(pBASE))->UpdateEnumForOpenParam();
	};
	return;	
};
void			be_CPrametrBase::_string( _str& _string ){
	_string = "";
	_string += ParamType;
	_string += " : ";
	_string += ParamName;
};
// be_CBasePrametr ///////////////////////////////////////////////////////
be_CBasePrametr::be_CBasePrametr(){
	UniveraslParam.GlobalReference.Assign("");
};
const	char*	be_CBasePrametr::GetThisElementView(const char* LocalName){

	char* pText = GetGlobalBuffer();
	if (pText==NULL)	return LocalName;
	_str paramName = "NotSet";
	BE_USAGE_FLAGS	USAGE_FROM_PARENT = GetParentUSAGE();
    if ( USAGE_FROM_PARENT == BE_IF_USE || g_BE_USAGE_FLAG == BE_IF_USE ){

		// GLUK !!! //////////////////////////////////////////////////////////////
		UniveraslParam.PrepareParam();
		BaseClass* pVarBase = (BaseClass*)UniveraslParam.GlobalReference.Get();
		if (pVarBase!=NULL&&g_beCheckChar(pVarBase->GetClassName())) {
			_str ClassTName = pVarBase->GetClassName();
			if (ClassTName=="be_CBasePrametr") {
				paramName = ((be_CPrametrBase*)pVarBase)->ParamName;
			}else{
				if (UniveraslParam.Local==true){
					((be_CVariableBase*)pVarBase)->GetValueAsSTR(paramName);
				}else{
					((be_CVariableBase*)pVarBase)->be_CVariableBase::GetValueAsSTR(paramName);
				};
			};
		};
		//////////////////////////////////////////////////////////////////////////
		
		if ( g_beCheckChar(ParamName.pchar())==false || 
			 g_beCheckChar(ParamType.pchar())==false ||
			 g_beCheckChar(paramName.pchar())==false	) return be_CPrametrBase::GetThisElementView(LocalName);
		sprintf( pText, "%s%s%s%s%s%s", 
						"{C ffFFFF00}", 
						ParamName.pchar(), 
						" = {C ff00FFFF}",
						paramName.pchar(),
						" {C ff00FF00}: class ", 
						ParamType.pchar() );
	}else{
		return be_CPrametrBase::GetThisElementView(LocalName);
	};
	UpdateEnumForOpenParam();	// Use for recteate ENUM for open param, seted in GetThisElementView
	return pText;
};
void			be_CBasePrametr::PrepareParam(bool forCreateENUM/*=false*/){
	UniveraslParam.PrepareParam(forCreateENUM);
};
bool			be_CBasePrametr::SetParamData(be_CBasePrametr* paramData){
	if ( paramData==NULL )	return false;
	if ( g_beCheckChar(ParamName.pchar())==false || g_beCheckChar(paramData->ParamName.pchar())==false ) return false;
	if ( g_beCheckChar(ParamType.pchar())==false || g_beCheckChar(paramData->ParamType.pchar())==false ) return false;
	if ( strcmp(ParamName.pchar(),paramData->ParamName.pchar())==0 ){
		if ( ParamType == paramData->ParamType ) {
			if ( paramData->UniveraslParam.Local==false && paramData->UniveraslParam.GlobalReference.ID==0xFFFFFFFF ){
			//	paramData->PrepareParam(true);
			}else{
				paramData->PrepareParam();
			};
			ReferableBaseClass* pValue	= (ReferableBaseClass*)(paramData->UniveraslParam.GlobalReference.Get());
			DWORD				ValueID = 0xFFFFFFFF;
			be_CBasePrametr*	pParam  = NULL;
			while (pValue!=NULL) {
				pParam = dynamic_cast<be_CBasePrametr*>(pValue);
				if (pParam!=NULL) {
					pValue = pParam->UniveraslParam.GlobalReference.Get();
				}else{
					ValueID = pValue->GetObjectGlobalID();
					pValue = NULL;
				};
			};
			
			UniveraslParam.GlobalReference.ID = ValueID;
						
			return ( (ValueID==0xffffffff) ? (false) : (true) );
		}else{
			// ... set if use bool, int, DWORD, float params seted in call !!!
			return false;
		};
	};
	return false;
};
bool			be_CBasePrametr::SetParam(const char* par_name, const char* par_type, bool local){
	if ( be_CPrametrBase::SetParam(par_name,par_type) ){
		UniveraslParam.SetLocal(local);
		return true;
	};
	return false;
};
be_CVariableBase*	be_CBasePrametr::GetVariable(){
	be_CVariableBase*	pVar = NULL;
	DWORD gparDataID = UniveraslParam.GlobalReference.ID;
	if ( gparDataID!=0xFFFFFFFF ) {
		int B=gparDataID>>24;		
		OneClassPointer* CP=CPGARB.CPointer[B][gparDataID&0xFFFFFF];
		if (CP!=NULL&&CP->ClassPtr!=NULL) {
			CPGARB.AddRef(gparDataID);
			pVar = (be_CVariableBase*)(CP->ClassPtr);
		};
	}else{	// Use global parameter
		UniveraslParam.PrepareParam();
		pVar = (be_CVariableBase*)(UniveraslParam.GlobalReference.Get());
	};	
	return pVar;
};
void				be_CBasePrametr::_string( _str& _string ){
	_string = "";
	_str _strTEMP;
	be_CPrametrBase::_string(_strTEMP);
	_strTEMP += " = ";
	_string += _strTEMP;
	be_CVariableBase*	_pVar = GetVariable();
	_strTEMP = "";
	if ( _pVar )	_pVar->GetValueAsSTR(_strTEMP);
	else			_strTEMP = "NULL";
	_string += _strTEMP;
};
// be_CParamList /////////////////////////////////////////////////////////
be_CParamList::be_CParamList(){
};
const char*	be_CParamList::GetThisElementView(const char* LocalName){
	OpenParam = NULL;
	int N = GetAmount();
	while ( OpenParam==NULL && N-- ) {

		be_CBasePrametr* pBP	= (be_CBasePrametr*)((*this)[N]);
		if ( pBP!=NULL ){
			bool	bBaseParam	= ClassEditor::CheckIfOpen( (BaseClass*)pBP );
	
			if ( bBaseParam && pBP->UniveraslParam.Local==false ){
				OpenParam = pBP;
			};
		};
	}
	return LocalName;
};
void be_CParamList::UpdateEnumForOpenParam(){
	if (OpenParam!=NULL ) {
		if (OpenParam->UniveraslParam.Local==false) {
			OpenParam->UniveraslParam.PrepareParam();
			OpenParam->UniveraslParam.GlobalReference.Get();
		};
	}else{
		OpenParam = NULL;
	};
};
bool be_CParamList::CreateCopyFrom(const be_CParamList& paramList){
	int MoreParam = GetAmount()-paramList.GetAmount();
	if ( MoreParam>0 ){
		MoreParam = GetAmount();
		while (MoreParam-- && GetAmount()-paramList.GetAmount()) {
			DelElement(MoreParam);
		};
	};
	be_CBasePrametr* pBP = NULL;
	for (int i=0; i<paramList.GetAmount(); i++){
		if (i<GetAmount()) pBP = (be_CBasePrametr*)((*this)[i]);
		if ( pBP==NULL || pBP->ParamName!=paramList[i]->ParamName || pBP->ParamType!=paramList[i]->ParamType ){
			pBP = new be_CBasePrametr;
			pBP->ParamName = paramList[i]->ParamName;
			pBP->ParamType = paramList[i]->ParamType;
			DelElement(i);
			Insert(i,pBP);
		};
		pBP=NULL;
	};
	return ( i == paramList.GetAmount() );
};
bool be_CParamList::SetParamData(be_CParamList& ParamList){
	bool OK = true;	
	int NP = GetAmount();
	for ( int i=0; i<NP&&OK; i++ ){
		be_CBasePrametr* pBP = (be_CBasePrametr*)((*this)[i]);
		OK = (pBP!=NULL);
		if (OK) {
			int NinP = ParamList.GetAmount();
			bool inOK = false;
			while (inOK==false&&NinP--) {
				if ( pBP->SetParamData((be_CBasePrametr*)(ParamList[NinP])) ) inOK = true;
			};
			if(!inOK){
				pBP->UniveraslParam.GlobalReference.ID=0xFFFFFFFF;
			}		
		};
	};
	return OK;
};
bool be_CParamList::SetParam( const char* par_name, be_CVariableBase* pData ){
	return 	SetParamData(par_name,pData);
};
bool be_CParamList::SetParamData( const char* par_name, be_CVariableBase* pData ){
	if ( g_beCheckChar(par_name)==false ) return false;
	int N = GetAmount();
	int id = -1;
	while ( N--&&id==-1 ) {
		if ( g_beCheckChar((*this)[N]->ParamName.pchar()) && strcmp( (*this)[N]->ParamName.pchar(), par_name )==0 ) 
			id = N;
	};
	if (id!=-1) {
		be_CBasePrametr* pBP = (be_CBasePrametr*)((*this)[id]);
		if (pBP!=NULL) {
			if (pData==NULL){
				pBP->UniveraslParam.SetLocal(false);
				pBP->UniveraslParam.GlobalReference.SetRef("NULL",0xFFFFFFFF);
			}else{
				
				pBP->SetParamType(pData->GetClassName());
				pBP->UniveraslParam.SetLocal(false);
				pBP->UniveraslParam.GlobalReference.SetRef(pData->Name.pchar(),pData->GetObjectGlobalID());
			};
		};
	};
};
bool be_CParamList::CheckParam( const char* pParamName ){
	if ( g_beCheckChar(pParamName)==false ) return false;
	int N = GetAmount();
	while ( N-- ) {
		if ( g_beCheckChar((*this)[N]->ParamName.pchar()) && strcmp( (*this)[N]->ParamName.pchar(), pParamName )==0 ) 
			return true;
	};
	return false;
};
bool be_CParamList::AddParam( const char* par_type, const char* par_name, bool local ){
	bool	AllReadyPresent = CheckParam(par_name);
	if ( AllReadyPresent==false ) {
		be_CBasePrametr* pPar = new be_CBasePrametr();
		pPar->RegisterMembers();
		Add(pPar);
		pPar->SetParam(par_name,par_type,local);
		pPar=NULL;
	}else{
		be_CBasePrametr* pPar = GetParamObj(par_name);
		pPar->UniveraslParam.SetLocal(local);
	}
	return true;
};
void				be_CParamList::DelParam( const char* par_name ){
	if ( g_beCheckChar(par_name)==false ) return;
	int N = GetAmount();
	while ( N-- ) {
		if ( g_beCheckChar((*this)[N]->ParamName.pchar()) && strcmp( (*this)[N]->ParamName.pchar(), par_name )==0 ) {
			DelElement(N);
			return;
		};
	};
};
void				be_CParamList::CLEAR(){
	int N = GetAmount();
	while (N--) {
		DelElement(N);
	};
};
be_CVariableBase*	be_CParamList::GetParam(const char* Name){
	be_CBasePrametr*	pPar = GetParamObj(Name);
	if (pPar!=NULL) return pPar->GetVariable();
	return NULL;
};
be_CBasePrametr*	be_CParamList::GetParamObj(const char* Name){
	if ( g_beCheckChar(Name)==false ) return NULL;
	int N = GetAmount();
	be_CBasePrametr*	pPar = NULL;
	while ( pPar==NULL && N-- ) {
		pPar = (be_CBasePrametr*)((*this)[N]);
		if ( pPar==NULL || g_beCheckChar(pPar->ParamName.pchar())==false || strcmp(pPar->ParamName.pchar(),Name)!=0 ) 
			pPar=NULL;
	};
	return pPar;
};
bool				be_CParamList::IsLocal(char* Name){
	be_CBasePrametr* pPO = GetParamObj(Name);
	if (pPO) return pPO->UniveraslParam.Local;
	return false;
};
void				be_CParamList::vDraw(){
	for ( int i=0; i<GetAmount(); i++ ){
		be_CBasePrametr*	pPar = (be_CBasePrametr*)((*this)[i]);
		be_CVariableBase*	pVar = NULL;
		if (pPar!=NULL) {
			DWORD gparDataID = pPar->UniveraslParam.GlobalReference.ID;
			if ( gparDataID!=0xFFFFFFFF ) {
				OneClassPointer* CP=CPGARB.CPointer[gparDataID>>24][gparDataID&0xFFFFFF];
				if (CP!=NULL&&CP->ClassPtr!=NULL) {
					CPGARB.AddRef(gparDataID);
					pVar = (be_CVariableBase*)(CP->ClassPtr);
				};
			}else{	// Use global parameter
				pPar->UniveraslParam.PrepareParam();
				pVar = (be_CVariableBase*)(pPar->UniveraslParam.GlobalReference.Get());
			};
		};
		if ( pVar ){
			pVar->vDraw();
		};
	};
};
void				be_CParamList::_string( _str& _string ){
	_string = "";
	be_CPrametrBase* _pPar = NULL;
	_str	_strPar;
	for ( int i=0; i<GetAmount(); i++ )	{
		_pPar = (*this)[i];
		if ( _pPar ){
			_strPar = "";
			_pPar->_string(_strPar);
			_string += _strPar;
			
		}else{
			_string += "UNKNOWN PARAMETR";
		};
		_string += "\n";
	};
};
be_CVariableBase*	be_CParamList::GetValue(const char* Name){
	be_CBasePrametr*	pPar = GetParamObj(Name);
	be_CVariableBase*	pVar = NULL;
	if (pPar!=NULL) {
		pPar->UniveraslParam.PrepareParam();
		pVar = (be_CVariableBase*)(pPar->UniveraslParam.GlobalReference.Get());
		if ( pVar==NULL ){
			pPar->UniveraslParam.PrepareParam( true );
			pVar = (be_CVariableBase*)(pPar->UniveraslParam.GlobalReference.Get());
		}
	};
	return pVar;
};
be_CGROUP*			be_CParamList::CreateParam_GROUP(const char* Name){
	be_CGROUP* pnewOBJ = NULL;
	if ( AddParam("be_CGROUP",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (be_CGROUP*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
be_CNODE*			be_CParamList::CreateParam_NODE(const char* Name){
	be_CNODE* pnewOBJ = NULL;
	if ( AddParam("be_CNODE",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (be_CNODE*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
be_CFLOAT*			be_CParamList::CreateParam_FLOAT(const char* Name){
	be_CFLOAT* pnewOBJ = NULL;
	if ( AddParam("be_CFLOAT",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (be_CFLOAT*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
be_CSTR*			be_CParamList::CreateParam_STR(const char* Name){
	be_CSTR* pnewOBJ = NULL;
	if ( AddParam("be_CSTR",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (be_CSTR*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
be_CINT*			be_CParamList::CreateParam_INT(const char* Name){
	be_CINT* pnewOBJ = NULL;
	if ( AddParam("be_CINT",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (be_CINT*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
be_CDWORD*			be_CParamList::CreateParam_DWORD(const char* Name){
	be_CDWORD* pnewOBJ = NULL;
	if ( AddParam("be_CDWORD",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (be_CDWORD*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
be_CBOOL*			be_CParamList::CreateParam_BOOL(const char* Name){
	be_CBOOL* pnewOBJ = NULL;
	if ( AddParam("be_CBOOL",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (be_CBOOL*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
be_CARRAY*			be_CParamList::CreateParam_ARRAY(const char* Name){
	be_CARRAY* pnewOBJ = NULL;
	if ( AddParam("be_CARRAY",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (be_CARRAY*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
UnitAbility*		be_CParamList::CreateParam_UintAbil(const char* Name){
	UnitAbility* pnewOBJ = NULL;
	if ( AddParam("UnitAbility",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (UnitAbility*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
WeaponModificator*	be_CParamList::CreateParam_WeaponMod	(const char* Name){
	WeaponModificator* pnewOBJ = NULL;
	if ( AddParam("WeaponModificators",Name,true) ){
		be_CBasePrametr* pPAR = GetParamObj(Name);
		if (pPAR!=NULL) {
			pnewOBJ = (WeaponModificator*)(pPAR->UniveraslParam.LocalObject.Get());
		};
	};
	return pnewOBJ;
};
BE_USAGE_FLAGS		be_CParamList::GetParentUSAGE(){
	BaseClass* pPAR = (BaseClass*)GetParent();
	if (pPAR!=NULL) {
		be_CFunctionBase* pFB = dynamic_cast<be_CFunctionBase*>(pPAR);	
		if (pFB!=NULL) {
			return pFB->GetParentUSAGE();
		};
	}
	return BE_IF_CALL;
};
//////////////////////////////////////////////////////////////////////////

void be_REG_CLASS_Parameter(){
	REG_CLASS(be_CUniveraslParam);
	REG_CLASS(be_CPrametrBase);
	REG_CLASS(be_CBasePrametr);
	REG_CLASS(be_CPrametr);
};






















