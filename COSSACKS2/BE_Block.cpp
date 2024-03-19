#include "stdheader.h"
#include "BE_HEADERS.h"

//************************************************************************
//						QUICK VALUE EDITOR								**
//************************************************************************
// beViewElemList ////////////////////////////////////////////////////////
int	beViewElemList::GetAmountOfEditableElements(void* DataPtr,DWORD Options){
	return BaseClass::GetAmountOfEditableElements( DataPtr, ( Options&(~4) ) );
};

// beSET_Obj /////////////////////////////////////////////////////////////
beSET_Obj::~beSET_Obj(){ 
	clearARRAY(); 
};
void	beSET_Obj::clearARRAY(){
	int N=ARRAY.GetAmount();
	ARRAY.Clear();
};
int		beSET_Obj::createArray( ClassArray<BaseClass>* BaseArray, char* ClassName, bool add ){
	if ( BaseArray==NULL )	return 0;
	if (!add) clearARRAY();
	int N=BaseArray->GetAmount();
	if ( N!=0 ) {
		beViewElem* pElem = NULL;
		for ( int i=0; i<N; i++ ){
			if ( (*BaseArray)[i]!=NULL && strcmp(ClassName,(*BaseArray)[i]->GetClassName())==0 ) {
				pElem = new beViewElem();
				pElem->Set( dynamic_cast<BaseClass*>((*BaseArray)[i]) );
				ARRAY.Add( pElem );
				pElem = NULL;
			};
		};

	};
	return ARRAY.GetAmount();
};
//************************************************************************

//************************************************************************
//						INDEPENDENT FUNCTION							**
//************************************************************************
// be_CFunctionBaseOLD ///////////////////////////////////////////////////
const	char*	be_CFunctionBaseOLD::GetThisElementView(const char* LocalName){
	_str strText;
	strText  = "{C ffFFFF00}";
	strText += Name;
	strText += "( ";

	// add input params for function
	for ( int i=0; i<InArgList.GetAmount(); i++ ){
		
		if ( InArgList[i]->ParamName.pchar()!=NULL && InArgList[i]->ParamName.pchar()[0]!=0 )
				strText += InArgList[i]->ParamName;
		
		if ( i!=InArgList.GetAmount()-1 )	
			strText += ", ";

	};

	strText += " )";
	char* pText = GetGlobalBuffer();
	if (pText==NULL)	return LocalName;
	sprintf( pText, "%s", strText.pchar() );
	return pText;
};

bool	be_CFunctionBaseOLD::CheckParam( const char* pParamName ){
	if ( pParamName==NULL || pParamName[0]==0 ) return false;
	int N = InArgList.GetAmount();
	while ( N-- ) {
		if (	InArgList[N]->ParamName.pchar()!=NULL &&
				InArgList[N]->ParamName.pchar()[0]!=0 &&
				strcmp( InArgList[N]->ParamName.pchar(), pParamName )==0 ) 
			return true;
	};
	return false;
};

bool	be_CFunctionBaseOLD::SetParamData(be_CParamList& paramData){
	int N=paramData.GetAmount();
	while (N--) {
		InArgList[N]->SetParamData((be_CBasePrametr*)(paramData[N]));
	};
	return true;
};
// be_CBaseBEFunction //////////////////////////////////////////////////////
int	 be_CBaseBEFunction::CallFunctions(){
	int retVAL = 0;
	for ( int i=0; i<FnBodyArray.GetAmount(); i++ ){
		retVAL = FnBodyArray[i]->Process(0);
	};
	return retVAL;
};

// be_CFunctionStore /////////////////////////////////////////////////////


//************************************************************************

//======================================================================//
//==================    GENERIC FUNCTIONS    ===========================//
//======================================================================//
const	char*	lvCGenericFN::GetThisElementView(const char* LocalName){
	
	if ( FunctionName.Get()!=NULL ){
		_str strText;
		strText  = "{C ffFFFF00}";
		strText += FunctionName.pchar();
		strText += "( ";

		// add input params for function
		for ( int i=0; i<InArgList.GetAmount(); i++ ){

			be_CBasePrametr* pPAR = (be_CBasePrametr*)(InArgList[i]);
			if ( pPAR->UniveraslParam.GlobalReference.Get()!=NULL ) {
				strText += "{C ff0000FF}";
				strText += pPAR->UniveraslParam.GlobalReference.Get()->Name;
				strText += "{C ffFFFF00}";
			}else{
				strText += InArgList[i]->ParamType;
			};
			
			if ( i!=InArgList.GetAmount()-1 )	
				strText += ", ";
		};

		strText += " )";
		char* pText = GetGlobalBuffer();
		if (pText==NULL)	return LocalName;
		sprintf( pText, "%s", strText.pchar() );
		return pText;
	};

	return "{C ffFFFF00}Select Function Name";
		
};
void			lvCGenericFN::CreateUseFnName(){
	be_CBaseBEFunction* pFn = (be_CBaseBEFunction*)(FunctionName.Get());
	if (pFn!=NULL) {
		InArgList.CreateCopyFrom(pFn->InArgList);
	};
};
void			lvCGenericFN::CCreateParamList::EvaluateFunction(){
	((lvCGenericFN*)(GetParent()))->CreateUseFnName();
};
int				lvCGenericFN::CallFunction(){
	int retValue = 0;

	BE_USAGE_FLAGS	l_OLD_USAGE_FLAG=g_BE_USAGE_FLAG;
	g_BE_USAGE_FLAG=BE_IF_CALL;

	be_CBaseBEFunction* pBFn = (be_CBaseBEFunction*)(FunctionName.Get());
	if (pBFn!=NULL) {
		pBFn->SetParamData(InArgList);
		retValue = pBFn->CallFunctions();
	};

	g_BE_USAGE_FLAG=l_OLD_USAGE_FLAG;

	return retValue;
};
//************************************************************************

void	REG_BE_CLASS(){
	REG_CLASS(TypeShowMask			);
	REG_CLASS(TypeShowMaskArray		);

	REG_CLASS(beSET_Obj				);
	//REG_CLASS(beSET_Obj<be_CDWORD>		);
	//REG_CLASS(beSET_Obj<be_CINT>	);
	//REG_CLASS(beSET_Obj<vvTEXT>		);
	//REG_CLASS(beSET_Obj<vvPICTURE>	);
	//REG_CLASS(beSET_Obj<vvPOINT2D>	);
	//REG_CLASS(beSET_Obj<vvPOINT_SET>);
	//REG_CLASS(beSET_Obj<vvVector3D>	);
	//REG_CLASS(beSET_Obj<vvDIALOG>	);

	// TEST
	REG_CLASS(be_CFunctionBaseOLD);
	REG_CLASS(be_CBaseBEFunction);

	REG_CLASS(be_CFunctionStore);

	REG_CLASS_EX(lvCGenericFN,"GENERIC");

	// TEST
	be_RegBlockENUM();
};

void	be_RegBlockENUM(){

};



//////////////////////////////////////////////////////////////////////////





















