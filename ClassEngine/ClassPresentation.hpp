
#include "ClassPresentation.h"

#ifdef IMPLEMENT_CLASS_FACTORY

enum be_ClassTypeENUM
{
	be_UNDEF		= -1,
	be_ClassArray	=  0,
	be_ClonesArray	=  1,
	be_ClassRef		=  2,
	be_ClonessPtr	=  3,
	be__index		=  4,
	be__strindex	=  5
};
be_ClassTypeENUM	gbe_GetClassType( const char* pszClassName, OneClassStorage* pOCS ){
	if ( (pszClassName!=NULL&&pszClassName[0]!=0&&strstr(pszClassName,"ClassArray")!=NULL)||
		 (pOCS&&pOCS->CheckAll("ClassArray",true,false,false)))
		return	be_ClassArray;
	else if ((pszClassName!=NULL&&pszClassName[0]!=0&&strstr(pszClassName,"ClonesArray")!=NULL)||
			 (pOCS&&pOCS->CheckAll("ClonesArray",true,false,false)))
		return	be_ClonesArray;
	else if ((pszClassName!=NULL&&pszClassName[0]!=0&&strstr(pszClassName,"ClassRef")!=NULL)||
			 (pOCS&&pOCS->CheckAll("ClassRef",true,false,false)))
		return	be_ClassRef;
	else if ((pszClassName!=NULL&&pszClassName[0]!=0&&strstr(pszClassName,"ClonessPtr")!=NULL)||
			 (pOCS&&pOCS->CheckAll("ClonessPtr",true,false,false)))
		return	be_ClonessPtr;
	else if (pszClassName!=NULL&&pszClassName[0]!=0&&strstr(pszClassName,"_index")!=NULL)
		return	be__index;
	else if (pszClassName!=NULL&&pszClassName[0]!=0&&strstr(pszClassName,"_strindex")!=NULL) 
		return	be__strindex;

	return	be_UNDEF;
};
// OnePresentationCondition //////////////////////////////////////////////
bool			OnePresentationCondition::Check(BaseClass* Class,void* DataPtr,void* ExtraPtr){
	return true;
};

// pc_IfClassFieldIsEqual ////////////////////////////////////////////////
const char*		pc_IfClassFieldIsEqual::GetThisElementView(const char* LocalName){
	if ( g_beCheckChar(FieldName.pchar()) && g_beCheckChar(FieldValue.pchar()) ){
		char* s=GetGlobalBuffer();
		if (strcmp(FieldName.pchar() ,"mask" )==0)	sprintf(s,"%s & %s",FieldName.pchar(),FieldValue.pchar());
		else									sprintf(s,"%s==%s",FieldName.pchar(),FieldValue.pchar());
		return s;
	};
	return LocalName;
};

bool			pc_IfClassFieldIsEqual::Check(BaseClass* Class,void* DataPtr,void* ExtraPtr){
	if(FieldName.pchar()&&FieldValue.pchar()){

		// Check Open status
		if ( strcmp(FieldName.pchar(),"this")==0 && strcmp(FieldValue.pchar(),"OPEN")==0 ) {
			return ClassEditor::CheckIfOpen(Class);
		};

		if ( g_beCheckChar(FieldName.pchar()) && strcmp(FieldName.pchar() ,"mask" )==0 ){
			DWORD	my_mask = Class->GetClassMask();
			DWORD	in_mask;
			sscanf(FieldValue.pchar(),"%u",&in_mask);
			return ( my_mask & in_mask );
		};

		void* Data2;
		BaseClass* Class2;
		void* Extra2;
		if(Class->GetElementByName(FieldName.pchar(),&Class2,&Data2,&Extra2,DataPtr)){
			
			xmlQuote xml;
			Class2->Save(xml,Data2,Extra2);
			const char* s=xml.Get_string();

			bool empty	= (strcmp(FieldValue.pchar(),"EMPTY")==0);
			bool null	= (strcmp(FieldValue.pchar(),"NULL" )==0);
			
			if (empty||null){
				_str cName;
				BaseClass* pBC=(BaseClass*)Data2;
				cName=Class2->GetClassName();

				OneClassStorage* pOCS = CGARB.GetClass(cName.pchar());
				be_ClassTypeENUM classT = gbe_GetClassType(cName.pchar(),pOCS);
				switch(classT) {
				case be_ClassArray:
					return ( (empty) ? (pBC->GetAmountOfElements()==0) : false );
					break;
				case be_ClonesArray:
					return ( (empty) ? (pBC->GetAmountOfElements()==0) : false );
					break;
				case be_ClassRef:
					return ( (null) ? (pBC->GetElementPtr(0,pBC)==NULL) : false );
					break;
				case be_ClonessPtr:
					return ( (null) ? (pBC->GetElementPtr(0,pBC)==NULL) : false );
					break;
				case be__index:
					return ( (null) ? (s&&s[0]==0) : false );
					break;
				case be__strindex:
					return ( (null) ? (s&&s[0]==0) : false );
					break;
				};
			};
			
			if(s&&!strcmp(s,FieldValue.pchar()))return true;
		};
	};
	return false;
};
// pc_IfClassFieldIsNotEqual /////////////////////////////////////////////
const char*		pc_IfClassFieldIsNotEqual::GetThisElementView(const char* LocalName){
	if ( g_beCheckChar(FieldName.pchar()) && g_beCheckChar(FieldValue.pchar()) ){
		char* s=GetGlobalBuffer();
		if (strcmp(FieldName.pchar() ,"mask" )==0)	sprintf(s,"%s | %s",FieldName.pchar(),FieldValue.pchar());
		else									sprintf(s,"%s!=%s",FieldName.pchar(),FieldValue.pchar());
		return s;
	};
	return LocalName;
};

bool			pc_IfClassFieldIsNotEqual::Check(BaseClass* Class,void* DataPtr,void* ExtraPtr){
	if(FieldValue.pchar()){
	
		// Check Open status
		if ( strcmp(FieldName.pchar(),"this")==0 && strcmp(FieldValue.pchar(),"OPEN")==0 ) {
			return !ClassEditor::CheckIfOpen(Class);
		};

		if ( g_beCheckChar(FieldName.pchar()) && strcmp(FieldName.pchar() ,"mask" )==0 ){
			DWORD	my_mask = Class->GetClassMask();
			DWORD	in_mask;
			sscanf(FieldValue.pchar(),"%u",&in_mask);
			return ( my_mask | in_mask );
		};

		void* Data2;
		BaseClass* Class2;
		void* Extra2;
		if(Class->GetElementByName(FieldName.pchar(),&Class2,&Data2,&Extra2,DataPtr)){

			xmlQuote xml;
			Class2->Save(xml,Data2,Extra2);
			const char* s=xml.Get_string();

			bool empty	= (strcmp(FieldValue.pchar(),"EMPTY")==0);
			bool null	= (strcmp(FieldValue.pchar(),"NULL" )==0);

			if (empty||null){
				_str cName;
				BaseClass* pBC=(BaseClass*)Data2;
				cName=Class2->GetClassName();

				OneClassStorage* pOCS = CGARB.GetClass(cName.pchar());
				be_ClassTypeENUM classT = gbe_GetClassType(cName.pchar(),pOCS);
				switch(classT) {
				case be_ClassArray:
					return ( (empty) ? (pBC->GetAmountOfElements()>0) : false );
					break;
				case be_ClonesArray:
					return ( (empty) ? (pBC->GetAmountOfElements()>0) : false );
					break;
				case be_ClassRef:
					return ( (null) ? (pBC->GetElementPtr(0,pBC)!=NULL) : false );
					break;
				case be_ClonessPtr:
					return ( (null) ? (pBC->GetElementPtr(0,pBC)!=NULL) : false );
					break;
				case be__index:
					return ( (null) ? (s&&s[0]!=0) : false );
					break;
				case be__strindex:
					return ( (null) ? (s&&s[0]!=0) : false );
					break;
				};
			};
			
			if(s&&strcmp(s,FieldValue.pchar()))return true;
		};
	};
	return false;
};
// pc_OR /////////////////////////////////////////////////////////////////
const char*		pc_OR::GetThisElementView(const char*){
	static _str s;
	s="( ";
	for(int i=0;i<OR_List.GetAmount();i++){
		OnePresentationCondition* PC=OR_List[i];
		const char* ss=PC->GetThisElementView(NULL);
		if(ss){
			s+=ss;
			if(i<OR_List.GetAmount()-1) s+=" || ";
		};
	};
	s+=" )";
	return s.pchar();
};

bool			pc_OR::Check(BaseClass* Class,void* DataPtr,void* ExtraPtr){
	for(int i=0;i<OR_List.GetAmount();i++){
		bool r=OR_List[i]->Check(Class,DataPtr,ExtraPtr);
		if(r)return true;
	};
	return false;
};

// pc_AND ////////////////////////////////////////////////////////////////
const char*		pc_AND::GetThisElementView(const char*){
	static _str s;
	s="( ";
	for(int i=0;i<AND_List.GetAmount();i++){
		OnePresentationCondition* PC=AND_List[i];
		const char* ss=PC->GetThisElementView(NULL);
		if(ss){
			s+=ss;
			if(i<AND_List.GetAmount()-1) s+=" && ";
		};
	};
	s+=" )";
	return s.pchar();
};

bool			pc_AND::Check(BaseClass* Class,void* DataPtr,void* ExtraPtr){
	for(int i=0;i<AND_List.GetAmount();i++){
		bool r=AND_List[i]->Check(Class,DataPtr,ExtraPtr);
		if(!r)return false;
	};
	return true;
};

// ConditionsList ////////////////////////////////////////////////////////
const char*		ConditionsList::GetThisElementView(const char*){
	static _str s;
	s.Clear();
	for(int i=0;i<GetAmount();i++){
		OnePresentationCondition* PC=(*this)[i];
		s+=PC->GetThisElementView(NULL);
		if(i<GetAmount()-1)s+=" && ";
	};
	return s.pchar();
};

// OnePresentationItem ///////////////////////////////////////////////////
bool			OnePresentationItem::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){

	BaseClass::Load(xml,ClassPtr,Error,Extra);

	// Load fild using textID
	((OnePresentationItem*)ClassPtr)->LoadFildsUseTextID();

	return true;
};

char*			OnePresentationItem::GetPresentationPrefix(){
	void* DataPtr000;
	void *ExtraPtr000;
	BaseClass* ClassPtr000;
	BaseClass* P = this->GetParent();

	while(P){
		if(P->GetElementByName("PresentationPrefix",&ClassPtr000,&DataPtr000,&ExtraPtr000,P)){
			if(!strncmp(ClassPtr000->GetClassName(),"_str",4)){
				_str* ParentName=(_str*)DataPtr000;
				return ParentName->pchar();
			};
		};
		P=P->GetParent();
	};

	return NULL;
};

void			OnePresentationItem::FillTextID(){

	// Set TextID in right value
	BaseClass* pBC = this->GetParent();
	while ( pBC!=NULL ) {
		void* DataPtr;
		void *ExtraPtr;
		BaseClass* ClassPtr;
		if(pBC->GetElementByName("ClassName",&ClassPtr,&DataPtr,&ExtraPtr,pBC)){
			if(!strncmp(ClassPtr->GetClassName(),"_str",4)){
				TextClassPresentation* pTCP = dynamic_cast<TextClassPresentation*>(pBC);
				if (pTCP!=NULL) {
					for ( int i=0; i<pTCP->Presentation.GetAmount(); i++ ){
						if ( pTCP->Presentation[i] == this ){
							char topic[128];
							if (GetPresentationPrefix()!=NULL){
								sprintf( topic, "%s%s%s%s%s%d", "@PRES::", GetPresentationPrefix(), "::", pTCP->ClassName.pchar(), "::", i );
								Text = topic;
							};
						};
					};
					return;
				};
			};
		};
		pBC = pBC->GetParent();
	};

};

void			OnePresentationItem::Save(xmlQuote& xml,void* ClassPtr,void* Extra){

	((OnePresentationItem*)ClassPtr)->FillTextID();	// Set TextID in right value

	BaseClass::Save(xml,ClassPtr,Extra);

};

char*			OnePresentationItem::GetText(){
	return Text.pchar();
};

bool			OnePresentationItem::LoadFildsUseTextID(){

	FillTextID();
	if ( Text.pchar()!=NULL && Text.pchar()[0]!=0 && strcmp( GetTextByID(Text.pchar()), Text.pchar() ) != 0 ) {
		Text = GetTextByID(Text.pchar());
		return true;
	};

	return false;
};

const	char*	OnePresentationItem::GetThisElementView(const char* LocalName){
	if ( Text.pchar()!=NULL && Text.pchar()[0]!=0 ) return Text.pchar();
	return LocalName;
};
// ClassPresentation /////////////////////////////////////////////////////
bool			ClassPresentation::AdditionCheck0(const char* __ClassName, char* __DirectClassName, char* __KeyName){
	if ( __ClassName && __ClassName[0] ){
		OneClassStorage* pOCS = CGARB.GetClass(__ClassName);
		if ( pOCS && pOCS->OneMemb ){
			_str _dest="";
			pOCS->OneMemb->GetClassPresentationMask(_dest,NULL,NULL,NULL,0);
			if ( strstr(_dest.pchar(),__KeyName)!=NULL ) {
				return pOCS->CheckAll(__DirectClassName,true,false,false);
			};
		};
	};
	return false;
};
bool			ClassPresentation::haveMask(const char* _ClassName){ 
	return ( _ClassName && GetPresClassName() && strcmp(GetPresClassName(),_ClassName)==0); 
};

const char*		ClassPresentation::GetPresClassName(){ 
	return NULL; 
};

const char*		ClassPresentation::GetThisElementView(const char*){
	if(GetPresClassName()){
		char* s=GetGlobalBuffer();
		sprintf(s,"class: %s",GetPresClassName());
		return s;
	};
	return NULL;
};

void			ClassPresentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	return;
};

// ClassFieldHintAndText /////////////////////////////////////////////////
bool			ClassFieldHintAndText::SaveFildsUseTextID(){

	char ID[512];
	char HINT[512];

	// Load fild using textID

	if ( GetParentName()!=NULL ){

		sprintf(ID,"@ID::%s::%s",GetParentName(),FieldName.pchar());

		sprintf(HINT,"@HINT::%s::%s",GetParentName(),FieldName.pchar());

		TextToShow	= ID;
		Hint		= HINT;

		return true;

	};	

	return false;
};

void			ClassFieldHintAndText::CGetFildsUseTextID::EvaluateFunction(){ 
	((ClassFieldHintAndText*)(this->GetParent()))->LoadFildsUseTextID(); 
};

void			ClassFieldHintAndText::Save(xmlQuote& xml,void* ClassPtr,void* Extra){

	((ClassFieldHintAndText*)ClassPtr)->SaveFildsUseTextID();

	BaseClass::Save(xml,ClassPtr,Extra);
};
bool			ClassFieldHintAndText::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){

	BaseClass::Load(xml,ClassPtr,Error,Extra);

	// Load fild using textID
	((ClassFieldHintAndText*)ClassPtr)->LoadFildsUseTextID();

	return true;
};

bool			ClassFieldHintAndText::LoadFildsUseTextID(){

	char ID[512];
	char HINT[512];

	// Load fild using textID

	if ( GetParentName()!=NULL ){

		sprintf(ID,"@ID::%s::%s",GetParentName(),FieldName.pchar());
		sprintf(HINT,"@HINT::%s::%s",GetParentName(),FieldName.pchar());

		TextToShow	= GetTextByID(ID);
		Hint		= GetTextByID(HINT);

		return ( strcmp(TextToShow.pchar(),ID)==0 || strcmp(Hint.pchar(),HINT)==0 );

	};	

	return false;
};

void			ClassFieldHintAndText::SaveInTextFile(_str& _strPresentation){
	char ID[512];
	char HINT[512];

	// Load fild using textID

	if ( GetParentName()!=NULL ){

		sprintf(ID,"@ID::%s::%s",GetParentName(),FieldName.pchar());
		sprintf(HINT,"@HINT::%s::%s",GetParentName(),FieldName.pchar());

		// Save in _str data by ID and HINT		
		int maxL = max( strlen(ID), strlen(HINT) );
		char maskS[64];
		if ( maxL >= 50 )	sprintf(maskS,"%s%d%s","%-",maxL+5,"s%s");
		else				sprintf(maskS,"%s","%-52s%s");

		char ID_TEXT[512];
		char HT_TEXT[512];
		sprintf(ID_TEXT,maskS,ID,TextToShow.pchar());
		sprintf(HT_TEXT,maskS,HINT,Hint.pchar());

		_strPresentation = ID_TEXT;
		_strPresentation+="\n";
		_strPresentation+= HT_TEXT;
		_strPresentation+="\n";
	};	
};

char*			ClassFieldHintAndText::GetParentName(){
	void* DataPtr000;
	void *ExtraPtr000;
	BaseClass* ClassPtr000;
	BaseClass* P = this->GetParent();

	while(P){
		if(P->GetElementByName("ClassName",&ClassPtr000,&DataPtr000,&ExtraPtr000,P)){
			if(!strncmp(ClassPtr000->GetClassName(),"_str",4)){
				_str* ParentName=(_str*)DataPtr000;
				return ParentName->pchar();
			};
		};
		P=P->GetParent();
	};

	return NULL;
};

const	char*	ClassFieldHintAndText::GetThisElementView(const char* LocalName){
	if ( TextToShow.pchar()[0]!=0 ) {
		char* s=GetGlobalBuffer();
		sprintf(	s, "%s%s%s%s%s%s", "{C ffFFFF00}"				, 
					GetParentName()				,
					"{C ff00FF00}::{C ffFFFF00}", 
					FieldName.pchar()				, 
					"{C FFD4C19C}\n"			, 
					TextToShow.pchar() 					);
		return s;
	};

	return LocalName;
};

// ClassTextAndENUMHint //////////////////////////////////////////////////
const	char*	ClassTextAndENUMHint::GetThisElementView(const char* LocalName){
	if ( CLASSID.pchar()[0]!=0 ) {
		return	CLASSID.pchar();
	};

	return LocalName;
};
void			ClassTextAndENUMHint::Save(xmlQuote& xml,void* ClassPtr,void* Extra){

	((ClassTextAndENUMHint*)ClassPtr)->SaveFildsUseTextID();

	BaseClass::Save(xml,ClassPtr,Extra);
};
bool			ClassTextAndENUMHint::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){

	BaseClass::Load(xml,ClassPtr,Error,Extra);

	// Load fild using textID
	((ClassTextAndENUMHint*)ClassPtr)->LoadFildsUseTextID();

	return true;
};
bool			ClassTextAndENUMHint::LoadFildsUseTextID(){
	char a_CLASSID[512];
	char a_ENUMHINT[512];
	char a_CLASSHINT[512];

	// Load fild using textID
	_str parentName = GetParentName();

	if (  parentName.pchar()[0]!=0 ){

		sprintf(a_CLASSID	, "@CLASSID::%s"			, parentName.pchar());
		sprintf(a_ENUMHINT	, "@ENUMHINT::@CLASSID::%s"	, parentName.pchar());
		sprintf(a_CLASSHINT	, "@HINT::%s"				, parentName.pchar());

		CLASSID		= GetTextByID(a_CLASSID);
		ENUMHINT	= GetTextByID(a_ENUMHINT);
		CLASSHINT	= GetTextByID(a_CLASSHINT);

		return (	strcmp(CLASSID.pchar(),a_CLASSID)		==0	|| 
					strcmp(ENUMHINT.pchar(),a_ENUMHINT)		==0	|| 
					strcmp(CLASSHINT.pchar(),a_CLASSHINT)	==0		);

	};	
	
	return false;
};	
bool			ClassTextAndENUMHint::SaveFildsUseTextID(){

	char a_CLASSID[512];
	char a_ENUMHINT[512];
	char a_CLASSHINT[512];

	// Load fild using textID
	_str parentName = GetParentName();

	if ( parentName.pchar()[0]!=0 ){

		sprintf(a_CLASSID	, "@CLASSID::%s"			, parentName.pchar());
		sprintf(a_ENUMHINT	, "@ENUMHINT::@CLASSID::%s"	, parentName.pchar());
		sprintf(a_CLASSHINT	, "@HINT::%s"				, parentName.pchar());

		CLASSID		= a_CLASSID;
		ENUMHINT	= a_ENUMHINT;
		CLASSHINT	= a_CLASSHINT;

		return true;

	};	

	return false;
};
char*			ClassTextAndENUMHint::GetParentName(){
	void* DataPtr000;
	void *ExtraPtr000;
	BaseClass* ClassPtr000;
	BaseClass* P = this->GetParent();

	while(P){
		if(P->GetElementByName("ClassName",&ClassPtr000,&DataPtr000,&ExtraPtr000,P)){
			if(!strncmp(ClassPtr000->GetClassName(),"_str",4)){
				_str* ParentName=(_str*)DataPtr000;
				return ParentName->pchar();
			};
		};
		P=P->GetParent();
	};

	return NULL;
};
void			ClassTextAndENUMHint::SaveInTextFile(_str& _strPresentation){
	char a_CLASSID[512];
	char a_ENUMHINT[512];
	char a_CLASSHINT[512];

	// Load fild using textID
	_str parentName = GetParentName();

	if (  parentName.pchar()[0]!=0 ){

		sprintf(a_CLASSID	, "@CLASSID::%s"			, parentName.pchar());
		sprintf(a_ENUMHINT	, "@ENUMHINT::@CLASSID::%s"	, parentName.pchar());
		sprintf(a_CLASSHINT	, "@HINT::%s"				, parentName.pchar());

		// Save in _str data by ID and HINT		
		int maxL = max( max( strlen(a_CLASSID), strlen(a_ENUMHINT) ), strlen(a_CLASSHINT) );
		char maskS[64];
		if ( maxL >= 50 )	sprintf(maskS,"%s%d%s","%-",maxL+5,"s%s");
		else				sprintf(maskS,"%s","%-52s%s");

		char CLASSID_TEXT[512];
		char ENUMHINT_TEXT[512];
		char CLASSHINT_TEXT[512];
		sprintf(CLASSID_TEXT,maskS,a_CLASSID,CLASSID.pchar());
		sprintf(ENUMHINT_TEXT,maskS,a_ENUMHINT,ENUMHINT.pchar());
		sprintf(CLASSHINT_TEXT,maskS,a_CLASSHINT,CLASSHINT.pchar());

		_strPresentation = CLASSID_TEXT;
		_strPresentation+="\n";
		_strPresentation+= ENUMHINT_TEXT;
		_strPresentation+="\n";
		_strPresentation+= CLASSHINT_TEXT;
		_strPresentation+="\n";
	};	
};
void			ClassTextAndENUMHint::CGetFildsUseTextID::EvaluateFunction(){ 
	((ClassTextAndENUMHint*)(this->GetParent()))->LoadFildsUseTextID(); 
};

// TextClassPresentation /////////////////////////////////////////////////
const char*		TextClassPresentation::GetPresClassName(){ 
	return ClassName.pchar(); 
};

void			TextClassPresentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){	
	for(int i=0;i<Presentation.GetAmount();i++){
		OnePresentationItem* OPI=Presentation[i];
		bool well=true;		
		for(int j=0;j<OPI->Condition.GetAmount();j++){
			bool r=OPI->Condition[j]->Check(Class,Data,Extra);
			if( !(well&=r) )break;
		}
		if(well){
			Dest+=OPI->GetText();
			return;
		};
	};
};

void			TextClassPresentation::SaveInTextFile(_str& _strPresentation){

	// Heading
	_strPresentation ="/////////////////////////////////////////////////////////////////////\n";
	char ts[512];
	char topic[128]; topic[0]=0;
	if (CGARB.GetClass(ClassName.pchar())) sprintf(topic,"%s",CGARB.GetClass(ClassName.pchar())->ClassTopic);
	sprintf(ts,"%s%-49s%s%s","// ",ClassName.pchar(),topic,"\n");
	_strPresentation+= ts;	ts[0]=0;
	_strPresentation+="/////////////////////////////////////////////////////////////////////\n\n";

	//-----------------------------------------------------
	// Parameters list
	//-----------------------------------------------------
	_strPresentation+="-- PARAMETRS LIST ---------------------------------------------------\n";
	_str ss;
	for ( int i=0; i<FieldsDescription.GetAmount(); i++ ){
		FieldsDescription[i]->SaveInTextFile( ss );
		_strPresentation+= ss;
	};				   
	_strPresentation+="---------------------------------------------------------------------\n\n";
	//=====================================================

	//-----------------------------------------------------
	// Presentations Text ID for current class
	//-----------------------------------------------------
	_strPresentation+="-- PRESENTATION LIST ------------------------------------------------\n";
	for ( int i=0; i<Presentation.GetAmount(); i++ ){
		_str Text = Presentation[i]->Text.pchar();

		Presentation[i]->FillTextID();

		char maskS[64];
		if ( strlen(Presentation[i]->Text.pchar()) >= 50 )	sprintf( maskS, "%s%d%s", "%-", strlen(topic)+5,"s%s");
		else											sprintf( maskS, "%s", "%-52s%s" );

		sprintf( ts, maskS, Presentation[i]->Text.pchar(), Text.pchar() );

		_strPresentation+=ts;	ts[0]=0;
		_strPresentation+="\n";
	};
	_strPresentation+="---------------------------------------------------------------------\n\n";
	//=====================================================

	//-----------------------------------------------------
	// ClassName and hint system Text ID for current class
	//-----------------------------------------------------
	_strPresentation+="-- NAME AND HINT LIST -----------------------------------------------\n";
	TextAndENUMHint.SaveInTextFile( ss );
	_strPresentation+= ss;
	_strPresentation+="---------------------------------------------------------------------\n\n";
	//=====================================================

	// END
	_strPresentation+="/////////////////////////////////////////////////////////////////////\n";

};

// _int_Presentation /////////////////////////////////////////////////////
_int_Presentation::_int_Presentation(){

};

const char*		_int_Presentation::GetPresClassName(){ 
	return "_int"; 
};

void			_int_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	int v=*((int*)Data);
	Dest.print("%d",v);
};

// _WORD_Presentation ////////////////////////////////////////////////////
_WORD_Presentation::_WORD_Presentation(){

}; 

const char*		_WORD_Presentation::GetPresClassName(){ 
	return "_WORD"; 
};

void			_WORD_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	WORD v=*((WORD*)Data);
	Dest.print("%u",v);
};

// _DWORD_Presentation ///////////////////////////////////////////////////
_DWORD_Presentation::_DWORD_Presentation(){

};

const char*		_DWORD_Presentation::GetPresClassName(){ 
	return "_DWORD"; 
};

void			_DWORD_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	DWORD v=*((DWORD*)Data);
	Dest.print("%u",v);
};

// _UnitType_Presentation ////////////////////////////////////////////////
_UnitType_Presentation::_UnitType_Presentation(){

};  

const char*		_UnitType_Presentation::GetPresClassName(){ 
	return "_UnitType"; 
};

void			_UnitType_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	int V=*((int*)Data);
//	Dest.print("%d",V);
	Dest.print("%s",NATIONS->Mon[V]->Message);
};

// _float_Presentation ///////////////////////////////////////////////////
_float_Presentation::_float_Presentation(){
	
};

bool			_float_Presentation::haveMask(const char* _ClassName){
	return strstr(_ClassName,"_float");
};

const char*		_float_Presentation::GetPresClassName(){ 
	return "_float"; 
};

void			_float_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	float v=*((float*)Data);
	if(Opt&&Opt[0]){
		Dest.print(Opt,v);
	}else Dest.print("%.02f",v);
};

// _str_Presentation /////////////////////////////////////////////////////
_str_Presentation::_str_Presentation(){
	
};

const char*		_str_Presentation::GetPresClassName(){ 
	return "_str";
};

void			_str_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	_str* s=(_str*)Data;	
	if (Opt&&Opt[0]) {
		if (strcmp(Opt,"value")==0) {
			Dest.print("%s",s->pchar());
		};
	}else{
		Dest.print("\"%s\"",s->pchar());
	};
};

// _strindex_Presentation ////////////////////////////////////////////////
const char*		_strindex_Presentation::GetPresClassName(){ 
	return "_strindex"; 
};

// _index_Presentation ///////////////////////////////////////////////////
_index_Presentation::_index_Presentation(){

};

const char*		_index_Presentation::GetPresClassName(){ 
	return "_index"; 
};

void			_index_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	Enumerator* E=(Enumerator*)Extra;
	int v=*((int*)Data);
	Dest.print("%s",E->Get(v));
};

// _bool_Presentation ////////////////////////////////////////////////////
_bool_Presentation::_bool_Presentation(){

};  

const char*		_bool_Presentation::GetPresClassName(){ 
	return "_bool"; 
};

void			_bool_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	bool v=*((bool*)Data);
	Dest.print(v?"true":"false");
};

// _gpfile_Presentation //////////////////////////////////////////////////
_gpfile_Presentation::_gpfile_Presentation(){

};  

const char*		_gpfile_Presentation::GetPresClassName(){ 
	return "_gpfile"; 
};

void			_gpfile_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	WORD v=*((WORD*)Data);
	Dest.print("%u",v);
};

// _ClassRef_Presentation ////////////////////////////////////////////////
bool	g_GetOptionParamAfterComma(_str& _string,_str& OptionPar){
	char* pNewOpt = NULL;
	if ( strstr(_string.pchar(),"this")!=NULL ) {
		char* pComa = strstr(_string.pchar(),",");
		if (pComa!=NULL) {
			pComa++;
			char* pLock = strstr(pComa,"$%");
			if (pLock!=NULL&&pLock-pComa!=0){
				pNewOpt = (char*)malloc(pLock-pComa+1);
				strncpy(pNewOpt,pComa,pLock-pComa);
				pNewOpt[pLock-pComa]=0;
				OptionPar = pNewOpt;
				free(pNewOpt);
				pNewOpt = NULL;
			};	
		};
		return true;
	};
	return false;
};
_ClassRef_Presentation::_ClassRef_Presentation(){

};  

const char*		_ClassRef_Presentation::GetPresClassName(){ 
	return "_ClassRef."; 
};

bool			_ClassRef_Presentation::haveMask(const char* _ClassName){

	if ( AdditionCheck0(_ClassName,"ClassRef","this")==true )	return true;

	return	( _ClassName!=NULL && strstr(_ClassName,"ClassRef.")!=NULL );
};

void			_ClassRef_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	Dest="";
	BaseClass* pRef=(BaseClass*)Data;
	// check if use this
	_str NewOpt = "";
	if ( pRef->CheckDirectCasting() ){
		_str myPres = "";
		if ( pRef->GetClassPresentationMask(myPres,Data,NULL,NULL,0) ){
			if ( g_GetOptionParamAfterComma(myPres,NewOpt)==false ){
				Dest = myPres;
				return;
			};
		};
	};
	////////////////////

	Dest="%$";
	Dest+=0;
	Dest+="$%";

	if		(Opt!=NULL)								Dest += Opt;
	else if (NewOpt.pchar()[0]!=0)	Dest += NewOpt;
};

// _ClassPtr_Presentation ////////////////////////////////////////////////
_ClassPtr_Presentation::_ClassPtr_Presentation(){

};   

const char*		_ClassPtr_Presentation::GetPresClassName(){ 
	return "_ClassPtr."; 
};

bool			_ClassPtr_Presentation::haveMask(const char* _ClassName){

	if ( AdditionCheck0(_ClassName,"ClassPtr","this")==true )	return true;

	return	( _ClassName!=NULL && strstr(_ClassName,"ClassPtr.")!=NULL );
};

void			_ClassPtr_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){
	Dest="";
	BaseClass* pPtr=(BaseClass*)Data;
	// check if use this
	_str NewOpt = "";
	if ( pPtr->CheckDirectCasting() ){
		_str myPres = "";
		if ( pPtr->GetClassPresentationMask(myPres,Data,NULL,NULL,0) ){
			if ( g_GetOptionParamAfterComma(myPres,NewOpt)==false ){
				Dest = myPres;
				return;
			};
		};
	};
	////////////////////

	Dest="%$";
	Dest+=0;
	Dest+="$%";

	if		(Opt!=NULL)								Dest += Opt;
	else if (NewOpt.pchar()[0]!=0)	Dest += NewOpt;
};

// _ClassArray_Presentation //////////////////////////////////////////////
_ClassArray_Presentation::_ClassArray_Presentation(){

};  

const char*		_ClassArray_Presentation::GetPresClassName(){ 
	return "_ClassArray."; 
};

bool			_ClassArray_Presentation::haveMask(const char* _ClassName){
	
	if ( AdditionCheck0(_ClassName,"ClassArray","this")==true )	return true;
	
	return	( _ClassName!=NULL && strstr(_ClassName,"ClassArray.")!=NULL );
};

void			_ClassArray_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){

	Dest="";
	BaseClass* pArray=(BaseClass*)Data;
	// check if use this
	_str NewOpt = "";
	if ( pArray->CheckDirectCasting() ){
		_str myPres = "";
		if ( pArray->GetClassPresentationMask(myPres,Data,NULL,NULL,0) ){
			if ( g_GetOptionParamAfterComma(myPres,NewOpt)==false ){
				Dest = myPres;
				return;
			};
		};
	};
	////////////////////

	int N=pArray->GetAmountOfElements();
	for (int i=0;i<N;i++){
		Dest+="%$";
		Dest+=i;
		Dest+="$%";
		if (i+1<N) {
			if		(Opt!=NULL)								Dest += Opt;
			else if (NewOpt.pchar()[0]!=0)	Dest += NewOpt;
		};
	};
};

// _ClonesArray_Presentation /////////////////////////////////////////////
_ClonesArray_Presentation::_ClonesArray_Presentation(){

};

const char*		_ClonesArray_Presentation::GetPresClassName(){ 
	return "_ClonesArray."; 
};

bool			_ClonesArray_Presentation::haveMask(const char* _ClassName){
	
	if ( AdditionCheck0(_ClassName,"ClonesArray","this")==true )	return true;

	return	( _ClassName!=NULL && strstr(_ClassName,"ClonesArray.")!=NULL );
};

void			_ClonesArray_Presentation::GetPresentationMask(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt,WholeClassPresentation* pPresStore){

	Dest="";
	BaseClass* pArray=(BaseClass*)Data;
	// check if use this
	_str NewOpt = "";
	if ( pArray->CheckDirectCasting() ){
		_str myPres = "";
		if ( pArray->GetClassPresentationMask(myPres,Data,NULL,NULL,0) ){
			if ( g_GetOptionParamAfterComma(myPres,NewOpt)==false ){
				Dest = myPres;
				return;
			};
		};
	};
	////////////////////

	int N=pArray->GetAmountOfElements();
	for (int i=0;i<N;i++){
		Dest+="%$";
		Dest+=i;
		Dest+="$%";
		if (i+1<N) {
			if		(Opt!=NULL)								Dest += Opt;
			else if (NewOpt.pchar()[0]!=0)	Dest += NewOpt;
		};
	};
};

// ClassPresentationList /////////////////////////////////////////////////
void			ClassPresentationList::SaveInTextFile(){

	if ( TextFileName.pchar()[0]==0 ) return;

	_str _strPresentation;
	
	// Heading
	_strPresentation = "*********************************************************************\n";
	_strPresentation+= "*********************************************************************\n";
	char ts[512];
	sprintf(ts,"%s%-57s%s","****  ",Description.pchar(),"  ****\n");
	_strPresentation+= ts;	ts[0]=0;
	_strPresentation+= "*********************************************************************\n";
	_strPresentation+="\n\n";

	// save all inside objects
	_str ss;
	for ( int i=0; i<ClassList.GetAmount(); i++ ){
		ClassList[i]->SaveInTextFile( ss );
		_strPresentation+= ss;
		_strPresentation+= "\n\n";
	};

	// END
	_strPresentation+= "*********************************************************************\n";
	_strPresentation+= "*********************************************************************\n";

	_strPresentation.DString::WriteToFile(TextFileName.pchar());
};

void			ClassPresentationList::Save(xmlQuote& xml,void* ClassPtr,void* Extra){

	// Save all text id and text in *.txt file
	((ClassPresentationList*)ClassPtr)->SaveInTextFile();

	BaseClass::Save(xml,ClassPtr,Extra);

};
// WholeClassPresentation ////////////////////////////////////////////////
WholeClassPresentation::WholeClassPresentation(){
	SetStdPresentation();
};

void			WholeClassPresentation::GetPresentation(char* Dest,BaseClass* Class){ 
	_str ddd; 
	GetPresentation(ddd,Class); 
	sprintf(Dest,"%s",ddd.pchar()); 
};

void			WholeClassPresentation::AddOnePresentation(ClassPresentation* BC){
	const char* C=BC->GetPresClassName();
	if(!(C&&C[0]))return;
	ClassPresentation* CP=(ClassPresentation*)BC->new_element();
	BC->Copy(CP);
	for(int i=0;i<ClassList.GetAmount();i++){
		if(ClassList[i]->GetPresClassName()&&!strcmp(ClassList[i]->GetPresClassName(),C)){
			delete(ClassList[i]);
			ClassList[i]=CP;
			return;
		};
	};
	ClassList.Add(CP);
};

void			WholeClassPresentation::AddFromList(char* List){
	if (List&&List[0]){
		ClassPresentationList TCP; 
		if (TCP.SafeReadFromFile(List))	AddFromList(&TCP);
	};
};

void			WholeClassPresentation::AddFromList(ClassPresentationList* List){
	if (List!=NULL) {
		int clN=List->ClassList.GetAmount();
		ClassPresentation* pCP=NULL;
		for (int i=0; i<clN; i++){
			pCP = dynamic_cast<ClassPresentation*>(List->ClassList[i]);
			if (pCP!=NULL) AddOnePresentation( pCP );
		};
	};
};

void			WholeClassPresentation::SetStdPresentation(){
	OneClassStorage* OCS=CGARB.GetClass("ClassPresentation");
	if(OCS){
		for(int i=0;i<OCS->Children.GetAmount();i++){
			char* C=OCS->Children[i];
			if(C && strcmp(C,"TextClassPresentation")!=0){
				OneClassStorage* OCS2=CGARB.GetClass(C);
				if(OCS2){
					BaseClass* BC=OCS2->OneMemb->new_element();
					if( BC ){
						AddOnePresentation( (ClassPresentation*)BC );
						delete BC;
					};
				};
			};
		};
	};
};

void			WholeClassPresentation::GetPresentation(_str& Dest,BaseClass* Class){
	if ( ClassList.GetAmount()==0 ) SetStdPresentation();
	Dest="";
	if (Class&&Class->CheckDirectCasting()) {
		ClassPresentation* pCP=GetPresentation(Class->GetClassName());
		if (pCP!=NULL)
			pCP->GetPresentationMask(Dest,Class,Class,NULL,NULL,this);
		else
			Class->GetClassPresentationMask(Dest,Class,NULL,NULL,0);

		if ( Dest.pchar()[0]!=0 ) {
			FormatingString(Dest);
			GetPresentation(Dest,Class,Class,NULL,NULL);
		};

		if (Dest.pchar()[0]==0){
			const char* s=Class->GetThisElementView(NULL);
			if(s)Dest=s;
			else Dest=Class->GetClassName();
		};
	};
};

void			WholeClassPresentation::GetPresentation(_str& Dest,BaseClass* Class,void* Data,void* Extra,char* Opt){

	BaseClass* pBC=(BaseClass*)Data;
	BaseClass* pBC_TPR=NULL;
	BaseClass*	a_Class	= NULL;
	void*		a_Data	= NULL;
	void*		a_Extra	= NULL;

	int strN=strlen(Dest.pchar());
	char* str=(char*)malloc(strN+1);
	strcpy(str,Dest.pchar());
	str[strN]=0;

	char* aaa=NULL;
	char* bbb=NULL;
	char* ccc=NULL;
	char* sep=strstr(str,"%$");
	if (sep){
		do{
			// create aaa
			int aaaN = (sep-str);
			aaa=(char*)malloc(aaaN+1);
			if (aaaN) strncpy(aaa,str,aaaN);
			aaa[aaaN]=0;
			//FormatingString(aaa);

			// create bbb

			int sepOPEN=0;
			int sepCLOS=0;

			char* sepEND=sep+2;
			bool stop=true;
			do{
				sepEND = strstr(sepEND,"$%");
				char* sepTTT=sep+2;
				sepOPEN=0;
				while ( (sepTTT=strstr(sepTTT,"%$")) && (sepEND-sepTTT>0) ) {
					sepTTT+=2;
					sepOPEN++;
				};
				stop=true;
				if (sepOPEN-sepCLOS>0){ sepEND+=2;sepCLOS++;stop=false;};
			}while(!stop);

			if(!sepEND)Log.Error("Incorrect presentation item: %s",sep);

			int bbbN = (sepEND-sep-2);
			bbb=(char*)malloc(bbbN+1);
			if (bbbN) strncpy(bbb,sep+2,bbbN);
			bbb[bbbN]=0;
			// create ccc
			int cccN = strlen(sepEND)-2;
			ccc=(char*)malloc(cccN+1);
			if (cccN) strcpy(ccc,sepEND+2);
			ccc[cccN]=0;

			// fill bbb using function GetPresentation(...);
			int nameN=strlen(bbb);
			char* opte=NULL;
			char* sepc=strstr(bbb,",");
			if (sepc) {
				nameN = (sepc-bbb);
				int opteN = strlen(sepc+1);
				opte=(char*)malloc(opteN+1);
				if (opteN) strcpy(opte,sepc+1);
				opte[opteN]=0;
			};
			char* name=(char*)malloc(nameN+1);
			if (nameN) strncpy(name,bbb,nameN);
			name[nameN]=0;
			if (name[0]>=0x30&&name[0]<=0x39) {
				int index = atoi(name);
				a_Class=pBC->GetElementClass(index);
				a_Data =pBC->GetElementPtr(index,pBC);
				a_Extra=pBC->GetElementExtraPtr(index);
			}else{
				char* pPAR=name;
				pBC_TPR=pBC;
				while ( pPAR=strstr(pPAR,"PAR.") ) {
					pPAR+=4;
					pBC_TPR=(BaseClass*)(pBC_TPR->GetParent());
					// reform name
					int tNN=strlen(pPAR);
					char* tName=(char*)malloc(tNN+1);
					if (tNN) strcpy(tName,pPAR);
					tName[tNN]=0;
					free(name);
					name=(char*)malloc(tNN+1);
					if (tNN) strcpy(name,tName);
					name[tNN]=0;
					pPAR=name;
					free(tName);
				};
				pBC_TPR->GetElementByName(name,&a_Class,&a_Data,&a_Extra,pBC_TPR);
			};
			if (a_Class==NULL){
				_str ErrorMessage;
				if		(pBC_TPR!=NULL) ErrorMessage  = pBC_TPR->GetClassName();
				else if (pBC!=NULL)		ErrorMessage  = pBC->GetClassName();
				else					ErrorMessage  = "class UN_DEFINE";
				ErrorMessage += "::";
				ErrorMessage += name;
				//	MessageBox(hwnd,ErrorMessage.str,"Presentation Error",MB_ICONERROR|MB_OK);
				Log.Warning("Invalid class member %s",ErrorMessage.pchar());
			};
			free(name);
			if (a_Class!=NULL) {
				_str BBB;
				ClassPresentation* pCPA=GetPresentation(a_Class->GetClassName());
				if (pCPA){
					pCPA->GetPresentationMask(BBB,a_Class,a_Data,a_Extra,opte,this);

				}else{
					if(a_Class->CheckDirectCasting())((BaseClass*)a_Data)->GetClassPresentationMask(BBB,a_Data,a_Extra,opte,0);
					else a_Class->GetClassPresentationMask(BBB,a_Data,a_Extra,opte,0);
				};
				if(BBB.pchar()[0]){
					if ( strcmp("donotshow",BBB.pchar())==0 ){
						BBB = "";
					}else{
						FormatingString(BBB);
						// set N of space in front of presentation
						char* opt_spece = aaa;
						char lsep[2];
						lsep[0]='\n'; lsep[1]=0;
						while (strstr(opt_spece,lsep)) {
							opt_spece=strstr(opt_spece,lsep)+1;
						};						
						aaaN=strlen(opt_spece);
						// we need mines all includes of {C xxXXXXXX}
						int NofSK = 0;
						lsep[0]='{'; lsep[1]=0;
						while (strstr(opt_spece,lsep)){
							opt_spece=strstr(opt_spece,lsep)+1;
							NofSK++;
						};
						aaaN = ( ((aaaN-NofSK*12)<0) ? (0) : (aaaN-NofSK*12) );
						/////////////////////////////////////////////
						opt_spece=(char*)malloc(aaaN+1);
						if (aaaN)	for (int i=0;i<aaaN;i++)	opt_spece[i]=' ';
						opt_spece[aaaN]=0;
						//////////////////////////////////////////
						GetPresentation(BBB,a_Class,a_Data,a_Extra,opt_spece);
						free(opt_spece);
					};
				}else{
					if(a_Class->CheckDirectCasting())GetPresentation(BBB,(BaseClass*)a_Data);
				};
				if (opte) free(opte);
				bbbN = strlen(BBB.pchar());
				free(bbb);
				bbb=(char*)malloc(bbbN+1);
				if (bbbN) strcpy(bbb,BBB.pchar());
				bbb[bbbN]=0;
			};

			// restore str
			free(str);
			str=(char*)malloc(strlen(aaa)+strlen(bbb)+strlen(ccc)+1);
			sprintf(str,"%s%s%s",aaa,bbb,ccc);
			// free memory
			free(aaa);
			free(bbb);
			free(ccc);
		}while( (sep=strstr(str,"%$")) );
	};

	// add spases after \n and \r
	if (Opt&&Opt[0]) {
		int OptN=strlen(Opt);
		sep=str;
		while (*sep) {
			if (*sep=='\n'||*sep=='\r') {
				int aaaN=(sep-str+1);
				aaa=(char*)malloc(aaaN+1);
				if (aaaN) strncpy(aaa,str,aaaN);
				aaa[aaaN]=0;
				int bbbN=strlen(sep+1);
				bbb=(char*)malloc(bbbN+1);
				if (bbbN) strncpy(bbb,sep+1,bbbN);
				bbb[bbbN]=0;
				free(str);
				str=(char*)malloc(strlen(aaa)+strlen(Opt)+strlen(bbb)+1);
				sprintf(str,"%s%s%s",aaa,Opt,bbb);
				sep=&(str[aaaN+OptN]);
				free(bbb);
				free(aaa);
			};
			if(*sep)sep++;
		};
	};

	Dest=str;
	free(str);
};

ClassPresentation* WholeClassPresentation::GetPresentation(const char* ClassName){
	if (ClassName==NULL||ClassName[0]==0)	return NULL;
	ClassPresentation* pCP=NULL;
	int N=ClassList.GetAmount();
    while (pCP==NULL&&N--) {
		pCP=ClassList[N];
		if (pCP&&pCP->haveMask(ClassName)==false)	pCP=NULL;
	};
	return pCP;
};

void			WholeClassPresentation::FormatingString(_str& Dest){	
	int i=0;
	while (Dest.pchar()[i]) {
		if ( Dest.pchar()[i]== '/' ) Dest.pchar()[i]='\n';
		i++;
	};
};

void			WholeClassPresentation::FormatingString(char* Dest){
	if (Dest==NULL)	return;
	int i=0;
	while (Dest[i]) {
		if ( Dest[i]== '/' ) Dest[i]='\n';
		i++;
	};
};

// Helper functions //////////////////////////////////////////////////////
void RegisterPresentations(){
	REG_CLASS(OnePresentationCondition);
	REG_CLASS(pc_IfClassFieldIsEqual);
	REG_CLASS(pc_IfClassFieldIsNotEqual);
	REG_CLASS(pc_OR);
	REG_CLASS(pc_AND);
	REG_CLASS(OnePresentationItem);
	REG_CLASS(ClassFieldHintAndText);
	REG_CLASS(ClassTextAndENUMHint);
	REG_CLASS(ClassPresentation);
	REG_CLASS(TextClassPresentation);
	REG_CLASS(_int_Presentation);
	REG_CLASS(_WORD_Presentation);
	REG_CLASS(_DWORD_Presentation);
	REG_CLASS(_UnitType_Presentation);
	REG_CLASS(_float_Presentation);
	REG_CLASS(_str_Presentation);
	REG_CLASS(_strindex_Presentation);
	REG_CLASS(_index_Presentation);
	REG_CLASS(_bool_Presentation);
	REG_CLASS(_gpfile_Presentation);
	REG_CLASS(_ClassRef_Presentation);
	REG_CLASS(_ClassPtr_Presentation);
	REG_CLASS(_ClassArray_Presentation);
	REG_CLASS(_ClonesArray_Presentation);
	REG_CLASS(ClassPresentationList);
};


#endif //IMPLEMENT_CLASS_FECTORY























