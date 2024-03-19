#pragma once
#ifdef IMPLEMENT_CLASS_FACTORY

Enumerators ENUM;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
//~~~~~~~~~~~OneClassStorage~~~~~~~~~~~/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
OneClassStorage::~OneClassStorage()
{
	if(OneMemb)delete OneMemb;
	OneMemb=NULL;
	for(int i=0;i<Children.GetAmount();i++)if(Children[i])delete[](Children[i]);
	if(ClassTopic)delete[](ClassTopic);
	ClassTopic=NULL;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
//~~~~~~~~~~~BaseClass~~~~~~~~~~~/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
void BaseClass::SetParent(BaseClass* Parent){
	//if(CheckDirectCasting()){
	ParentBC=Parent;
	OneClassStorage* OCS=GetClassStorage();
	if(OCS->OneMemb){
		for(int i=0;i<OCS->Members.GetAmount();i++){
			OneClassMemberStorage* OCMS=OCS->Members[i];
			if(OCMS&&OCMS->Member&&OCMS->Member->CheckDirectCasting()){
				((BaseClass*)(OCMS->GetElmPtr(this)))->SetParent(this);
			}
		}
	}
	//}
}
bool BaseClass::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	if(options&&!strcmp(options,"value")){		
		xmlQuote xml;
		Save(xml,DataPtr,ExtraPtr);
		dest=xml.Get_string();
	}
	return false;
}
const char* BaseClass::GetClassName(){
	return "BaseClass";
}
const char* BaseClass::GetStringField(const char* FieldName){
	static xmlQuote xml;
	xml.ClearAll();
	if(CheckDirectCasting()){
		BaseClass* BC;
		void* ptr;
		void* extra;
		if(GetElementByName(FieldName,&BC,&ptr,&extra,this)){
			BC->Save(xml,ptr,extra);            
		}
	}
	const char* s=xml.Get_string();
	return s?s:"";
}
int   BaseClass::GetIntField  (const char* FieldName,int DefaultVal){
	int v=DefaultVal;	
	sscanf(GetStringField(FieldName),"%d",&v);
	return v;
}
float BaseClass::GetFloatField(const char* FieldName,float DefaultVal){
	float v=DefaultVal;
	sscanf(GetStringField(FieldName),"%f",&v);
	return v;
}
BaseClass* BaseClass::GetRoot(){
	BaseClass* pp=this;
	BaseClass* p=pp->GetParent();
	if(!p)return NULL;
	while(pp){
		p=pp->GetParent();
		if(p==pp || !p)return pp;
		pp=p;
	}
	return NULL;
}
BaseClass::BaseClass(){	
	ParentBC=NULL;
}
void BaseClass::GetElementMessageAndHintInEditor(DString& Message,DString& Hint,BaseClass* Parent,char* ElmID){
	Hint.Clear();
	Message.Clear();
	if(Parent->CheckDirectCasting()){
		OneClassStorage* OCS=Parent->GetClassStorage();
		int N=Parent->GetAmountOfElements();
		char ID[512];
		sprintf(ID,"@ID::%s::%s",Parent->GetClassName(),ElmID);
		extern char* GetTextByID(char*);
		char* T=GetTextByID(ID);
		bool smt=false;
		//Message
		if(T[0]!='@'){
			Message=T;
			smt=true;
		}else{
			sprintf(ID,"@ID::%s",ElmID);
			T=GetTextByID(ID);
			if(T[0]!='@'){
				Message=T;
				smt=true;
			}
		}
		//Hint
		sprintf(ID,"@HINT::%s::%s",Parent->GetClassName(),ElmID);
		T=GetTextByID(ID);                
		if(T[0]!='@'){
			Hint=T;
			smt=true;
		}else{
			sprintf(ID,"@HINT::%s",ElmID);
			T=GetTextByID(ID);
			if(T[0]!='@'){
				Hint=T;
				smt=true;
			}
		}
		if(!smt){
			bool GetHintDebugMode();
			if(GetHintDebugMode()){
				sprintf(ID,"Message:\\@ID::%s::%s\\@ID::%s\\Hint:\\@HINT::%s::%s\\@HINT::%s\\",Parent->GetClassName(),ElmID,ElmID,Parent->GetClassName(),ElmID,ElmID);
				Hint=ID;
			}
		}
	}
}
void BaseClass::reset_class(void* DataPtr){
	BaseClass* BC=(BaseClass*)DataPtr;
	if(CheckDirectCasting()){
		for(int i=0;i<BC->GetAmountOfElements();i++){
			BC->GetElementClass(i)->reset_class(BC->GetElementPtr(i,DataPtr));
		}
	}
}
void BaseClass::RegisterOneMember(BaseClass* Member,void* ptr,const char* id,void* Extra,const char* Host,bool DirectCast,DWORD Mask){
	OneClassStorage* CST=GetClassStorage();
	if(!CST){
		CST=CGARB.CreateClassStorage(GetClassName());
	}
	OneClassMemberStorage* MS=CST->CreateMember(id);
	MS->Member=Member;
	MS->UseReference=0;
	if(StaticMode){
		MS->OffsetFromClassRoot= (int)ptr;
	}else{
		MS->OffsetFromClassRoot= (BYTE*)ptr - (BYTE*)this;
	}
	MS->ExtraData=Extra;
	MS->IntPropertyR     =NULL;
	MS->GeneralPropertyR =NULL;
	MS->IntPropertyW     =NULL;
	MS->GeneralPropertyW =NULL;
	MS->HostClass=Host;
	MS->DirectCasting=DirectCast;
	MS->Mask=Mask;
	MS->ReadOnly=ReadOnlyMode;
	MS->Invisible=InvisibleMode;
	MS->StaticMode=StaticMode;
	MS->NoSaveMode=NoSaveMode;

	MS->CheckValidRegistration(this,id);

	ReadOnlyMode=false;
	InvisibleMode=false;
	StaticMode=false;
	NoSaveMode=false;
}
void BaseClass::RegisterOneMemberIntProperty(BaseClass* Member,tpIntPropertyReader* IntPropR,tpIntPropertyWriter* IntPropW,const char* id,void* Extra,const char* Host,DWORD Mask){
	OneClassStorage* CST=GetClassStorage();
	if(!CST){
		CST=CGARB.CreateClassStorage(GetClassName());
	}
	OneClassMemberStorage* MS=CST->CreateMember(id);
	MS->Member=Member;
	MS->UseReference=0;
	MS->OffsetFromClassRoot=0;
	MS->ExtraData=Extra;
	MS->IntPropertyR     =IntPropR;
	MS->GeneralPropertyR =NULL;
	MS->IntPropertyW     =IntPropW;
	MS->GeneralPropertyW =NULL;
	MS->HostClass=Host;
	MS->DirectCasting=false;
	MS->Mask=Mask;

	if(StaticMode)PrintError("STATIC Not allowed for properties: <%s %s>",Member->GetClassName(),id);

	MS->ReadOnly=ReadOnlyMode;
	MS->Invisible=InvisibleMode;
	MS->StaticMode=false;
	MS->NoSaveMode=NoSaveMode;

	ReadOnlyMode=false;
	InvisibleMode=false;
	StaticMode=false;
	NoSaveMode=false;
}
void BaseClass::RegisterOneMemberProperty(BaseClass* Member,tpPropertyReader* PropR,tpPropertyWriter* PropW,const char* id,void* Extra,const char* Host,DWORD Mask){
	OneClassStorage* CST=GetClassStorage();
	if(!CST){
		CST=CGARB.CreateClassStorage(GetClassName());
	}
	OneClassMemberStorage* MS=CST->CreateMember(id);
	MS->Member=Member;
	MS->UseReference=0;
	MS->OffsetFromClassRoot=0;
	MS->ExtraData=Extra;
	MS->IntPropertyR     =NULL;
	MS->GeneralPropertyR =PropR;
	MS->IntPropertyW     =NULL;
	MS->GeneralPropertyW =PropW;
	MS->HostClass=Host;
	MS->DirectCasting=false;
	MS->Mask=Mask;

	if(StaticMode)PrintError("STATIC Not allowed for properties: <%s %s>",Member->GetClassName(),id);

	MS->ReadOnly=ReadOnlyMode;
	MS->Invisible=InvisibleMode;
	MS->StaticMode=false;
	MS->NoSaveMode=NoSaveMode;

	ReadOnlyMode=false;
	InvisibleMode=false;
	StaticMode=false;
	NoSaveMode=false;
}
bool BaseClass::GetElementByName(const char* ElmName,BaseClass** ClassPtr,void** ElmPtr,void** ExtraPtr,void* base){
	if(!ElmName)return false;
	SetContext(this);
	int idx=ExpandWith(ElmName,base);
	if(idx!=-1){
		*ClassPtr=GetElementClass(idx);
		*ElmPtr=GetElementPtr(idx,base);
		return true;
	}
	int N=GetAmountOfElements();
	for(int i=0;i<N;i++){
		if(!strcmp(ElmName,GetElementID(i))){
			*ClassPtr=GetElementClass(i);
			*ElmPtr=GetElementPtr(i,base);
			*ExtraPtr=GetElementExtraPtr(i);
			return true;
		}
	}
	return false;
}
bool BaseClass::FindElementByName(const char* ElmName,BaseClass** ClassPtr,void** ElmPtr,void** ExtraPtr,void* base){
	SetContext(this);
	int N=GetAmountOfElements();
	for(int i=0;i<N;i++){
		if(!strcmp(ElmName,GetElementID(i))){
			*ClassPtr=GetElementClass(i);
			*ElmPtr=GetElementPtr(i,base);
			*ExtraPtr=GetElementExtraPtr(i);
			return true;
		};
	};
	return false;
}
OneClassMemberStorage* BaseClass::GetElementStorage(const char* ElmName){
	int N=GetAmountOfElements();
	for(int i=0;i<N;i++){
		if(!strcmp(ElmName,GetElementID(i))){
			OneClassStorage* OCS=GetClassStorage();
			if(!OCS)PrintError("class not registered: \"%s\"",GetClassName());
			else{ 
				int idx=i;
				if(CheckDirectCasting()){
					idx=GetRealMaskedPosition(i);
				}
				if(idx<OCS->Members.GetAmount())return OCS->Members[idx];
				else return NULL;
			}
		}
	}
	return NULL;
}
const char* BaseClass::GetElementHostClass(const char* ElmName){
	int N=GetAmountOfElements();
	for(int i=0;i<N;i++){
		if(!strcmp(ElmName,GetElementID(i))){
			OneClassStorage* OCS=GetClassStorage();
			if(!OCS)PrintError("class not registered: \"%s\"",GetClassName());
			else return OCS->Members[i]->HostClass;
		}
	}
	return NULL;
}
const char* BaseClass::GetElementHostClass(int Index){
	OneClassStorage* OCS=GetClassStorage();
	assert(Index>=0&&Index<OCS->Members.GetAmount());
	if(!OCS)PrintError("class not registered: \"%s\"",GetClassName());
	else return OCS->Members[Index]->HostClass;
	return NULL;
}
BaseClass::~BaseClass(){	
	if(CheckDirectCasting())reset_class(this);
};
void BaseClass::RegisterMembers(){
}
int BaseClass::GetAmountOfElements(){
	OneClassStorage* CST=GetClassStorage();
	//if(!CST->Members.GetAmount())
	RegisterMembers();
	if(CST){
		DWORD M=GetClassMask();
		if(M==0xFFFFFFFF){
			return CST->Members.GetAmount();
		}
		int N=CST->Members.GetAmount();
		int NR=0;
		for(int i=0;i<N;i++){
			if(CST->Members[i]->Mask&M)NR++;
		}
		return NR;
	}else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return 0;
	}
}
bool BaseClass::ForceProperty(){
	if(CheckDirectCasting()){
		BaseClass* P=GetParent();
		if(!P)return false;
		bool f=P->ForceProperty();
		return f;
	}
	return false;
}	
int BaseClass::GetRealMaskedPosition(int Index){
	if(Index>=0x1000000)return Index-0x1000000;
	DWORD M=GetClassMask();
	if(M==0xFFFFFFFF)return Index;
	OneClassStorage* CST=GetClassStorage();
	assert(Index>=0&&Index<CST->Members.GetAmount());
	if(CST){
		int N=CST->Members.GetAmount();
		for(int i=0;i<N;i++){
			if(CST->Members[i]->Mask&M){
				if(!Index)return i;
				Index--;
			}
		}
		assert(Index==0);
		return 0;
	}else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return 0;
	}
}
const char* BaseClass::GetElementID(int Index){
	OneClassStorage* CST=GetClassStorage();
	Index=GetRealMaskedPosition(Index);
	assert(Index>=0&&Index<CST->Members.GetAmount());
	if(CST){
		return CST->Members[Index]->xmlID;
	}else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return 0;
	}
}
const char* BaseClass::GetElementView(int Index,const char* LocalName){
	//Index=GetRealMaskedPosition(Index);
	if(CheckDirectCasting()&&Index<GetAmountOfElements()){
		BaseClass* BC=GetElementClass(Index);
		SetContext(BC);
		void* eptr=GetElementPtr(Index,this);
		if(BC->CheckDirectCasting()){
			BC=(BaseClass*)eptr;
			try{
				const char* s=BC->GetThisElementView(LocalName);
				if(s)return s;
			}catch(...){
				return LocalName;
			}
		}
	}
	return GetElementID(Index);
}
BaseClass* BaseClass::GetElementClass(int Index){
	Index=GetRealMaskedPosition(Index);
	OneClassStorage* CST=GetClassStorage();
	assert(Index>=0&&Index<CST->Members.GetAmount());
	if(CST)return CST->Members[Index]->Member;
	else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return 0;
	}
}
OneClassMemberStorage* BaseClass::GetElementCMS(int Index){
	Index=GetRealMaskedPosition(Index);	
	OneClassStorage* CST=GetClassStorage();
	if(!CST->Members.GetAmount())return NULL;
	assert(Index>=0&&Index<CST->Members.GetAmount());
	if(CST)return CST->Members[Index];
	else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return NULL;
	}
}
void* BaseClass::GetElementPtr(int Index,void* base){
	Index=GetRealMaskedPosition(Index);
	OneClassStorage* CST=GetClassStorage();
	assert(Index>=0&&Index<CST->Members.GetAmount());
	if(CheckDirectCasting())SetContext((BaseClass*)base);
	if(CST){
		if(CST->Members[Index]->IntPropertyR){
			static int V=0;
			V=CST->Members[Index]->IntPropertyR((BaseClass*)base);
			return &V;
		}else
			if(CST->Members[Index]->GeneralPropertyR){
				return CST->Members[Index]->GeneralPropertyR(base);
			}else
				return CST->Members[Index]->GetElmPtr(base);
	}else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return NULL;
	}
}
OneClassMemberInfo BaseClass::GetElementInfo(int Index,void* base){
    OneClassMemberInfo OCMI;
    int Index1=GetRealMaskedPosition(Index);
	OneClassStorage* CST=GetClassStorage();	
    OCMI.ParentClass=this;
    OCMI.ParentPtr=base;
	OCMI.OCMS=Index1<CST->Members.GetAmount()?CST->Members[Index1]:NULL;
	if(OCMI.OCMS){
		if(OCMI.OCMS->IntPropertyR){
			static int V=0;
			V=OCMI.OCMS->IntPropertyR((BaseClass*)base);
			OCMI.Data=&V;
		}else
		if(OCMI.OCMS->GeneralPropertyR){
			OCMI.Data=OCMI.OCMS->GeneralPropertyR(base);
		}else OCMI.Data=OCMI.OCMS->GetElmPtr(base);
	}
	OCMI.ClassPtr=GetElementClass(Index);
	OCMI.Data=GetElementPtr(Index,base);
	OCMI.Extra=GetElementExtraPtr(Index);
	if(OCMI.ClassPtr && OCMI.ClassPtr->CheckDirectCasting())OCMI.ClassPtr=(BaseClass*)OCMI.Data;	
	return OCMI;
}
OneClassMemberInfo BaseClass::GetElementInfo(const char* Name,void* base){
	OneClassMemberInfo OCMI;    
	OneClassStorage* CST=GetClassStorage();
	OCMI.Data=NULL;
	OCMI.ClassPtr=NULL;
	OCMI.OCMS=NULL;
	int Index=-1;
	for(int i=0;i<CST->Members.GetAmount();i++){
		if(!strcmp(CST->Members[i]->xmlID,Name)){
			Index=i;
			break;
		}
	}
	if(Index==-1)return OCMI;	
    OCMI.ParentClass=this;
    OCMI.ParentPtr=base;
    OCMI.OCMS=CST->Members[Index];
	if(OCMI.OCMS->IntPropertyR){
		static int V=0;
		V=OCMI.OCMS->IntPropertyR((BaseClass*)base);
		OCMI.Data=&V;
	}else
	if(OCMI.OCMS->GeneralPropertyR){
		OCMI.Data=OCMI.OCMS->GeneralPropertyR(base);
	}else OCMI.Data=OCMI.OCMS->GetElmPtr(base);
	OCMI.ClassPtr=OCMI.OCMS->Member;
	if(OCMI.ClassPtr && OCMI.ClassPtr->CheckDirectCasting())OCMI.ClassPtr=(BaseClass*)OCMI.Data;
	OCMI.Extra=OCMI.OCMS->ExtraData;
	return OCMI;    
}
BaseClass* OneClassMemberInfo::GetClass(){
	return ClassPtr;
}
void* OneClassMemberInfo::GetData(){
	return Data;
}
void* OneClassMemberInfo::GetExtra(){
	return Extra;
}
BaseClass* OneClassMemberInfo::GetParentClass(){
	return ParentClass;
}
void* OneClassMemberInfo::GetParentPtr(){
	return ParentPtr;
}
void OneClassMemberInfo::AcceptAfterWrite(){
	if(OCMS){
		if(OCMS->IntPropertyW)
		{
			int v=*((int*)Data);
			OCMS->IntPropertyW((BaseClass*)ParentPtr,v);
		}
		else
		{
			if(OCMS->GeneralPropertyW)
			{
				OCMS->GeneralPropertyW(ParentPtr,Data);
			}
		}    
	}
}
void OneClassMemberInfo::Save(xmlQuote* xml){
	if(ClassPtr){
		ClassPtr->Save(*xml,Data,Extra);
	}
}
void OneClassMemberInfo::Load(xmlQuote* xml,ErrorPager* ep){
	ErrorPager erp(0);
	if(!ep)ep=&erp;
	if(ClassPtr){
		ClassPtr->Load(*xml,Data,ep,Extra);
        AcceptAfterWrite();
	}
}
bool OneClassMemberInfo::IsInvisible(){
	return OCMS->Invisible;
}
bool OneClassMemberInfo::IsReadOnly(){
	return OCMS->ReadOnly;
}
bool OneClassMemberInfo::IsStatic(){
	return OCMS->StaticMode;
}
bool OneClassMemberInfo::IsNotSavable(){
	return OCMS->NoSaveMode;
}
bool OneClassMemberInfo::IsDirectCasting(){
	return ClassPtr->CheckDirectCasting();
}
DWORD OneClassMemberInfo::GetMask(){
	return OCMS->Mask;
}
const char* OneClassMemberInfo::Name(){
	return OCMS->xmlID;
}

bool BaseClass::LoadMember(xmlQuote& xml,void* ClassPtr,int MemIndex,ErrorPager* Error,void* Extra){
	OneClassStorage* OCS=GetClassStorage();
	//if(!OCS->Members.GetAmount())
	RegisterMembers();	
	MemIndex=GetRealMaskedPosition(MemIndex);
	assert(MemIndex>=0&&MemIndex<GetAmountOfElements());	
	OneClassMemberStorage* OCMS=OCS->Members[MemIndex];
	BaseClass* BASE=OCMS->Member;
	void* BasePtr=GetElementPtr(MemIndex,ClassPtr);
	void* Extra2=OCMS->ExtraData;
	if(!Extra2)Extra2=Extra;
	BASE->Load(xml,BasePtr,Error,Extra2);

	if(OCMS->IntPropertyW)
	{
		int v=*((int*)BasePtr);
		OCMS->IntPropertyW((BaseClass*)ClassPtr,v);
	}
	else
	{
		if(OCMS->GeneralPropertyW)
		{
			OCMS->GeneralPropertyW(ClassPtr,BasePtr);
		}
	}
	return true;
}

//  added by Silver, 21.08.2003
//  Returns true when given object is of type derived from parent class
//		with the given name
bool BaseClass::HasParentClass(const char* ParentClassName){
	RegisterMembers();
	for(int i=0;i<GetAmountOfElements();i++){
		if (!strcmp(ParentClassName,GetElementHostClass(i))) return true;	
	}
	return false;
} // BaseClass::HasParentClass

bool BaseClass::LoadMemberDirect(void* ElementPtr,void* ClassPtr,int MemIndex)
{
	MemIndex=GetRealMaskedPosition(MemIndex);
	RegisterMembers();
	assert(MemIndex>=0&&MemIndex<GetAmountOfElements());
	OneClassStorage* OCS=GetClassStorage();
	OneClassMemberStorage* OCMS=OCS->Members[MemIndex];
	if(OCMS->IntPropertyW)
	{
		int v=*((int*)ElementPtr);
		OCMS->IntPropertyW(this,v);
	}else
	{
		if(OCMS->GeneralPropertyW) OCMS->GeneralPropertyW(this,ElementPtr);
	}
	return true;
}

void* BaseClass::GetElementExtraPtr(int Index){
	Index=GetRealMaskedPosition(Index);
	OneClassStorage* CST=GetClassStorage();
	if(CST){
		if(Index>=0&&Index<CST->Members.GetAmount()){
			return CST->Members[Index]->ExtraData;
		}else return NULL;
	}else{
		PrintError("Undefined class found: \"%s\"\n",GetClassName());
		return 0;
	}
}
void BaseClass::DeleteDefaultSubFields(xmlQuote& xml,void* ClassPtr,void* Extra){
	if(CheckDirectCasting()&&!Extra){
		BaseClass* BC=(BaseClass*)ClassPtr;
		int N=BC->GetAmountOfElements();
		if(!N)return;
		bool s=BaseClass::SaveInShortForm;
		BaseClass::SaveInShortForm=false;
		BaseClass* temp=new_element();		
		xmlQuote XML;
		temp->Save(XML,temp,Extra);
		BaseClass::SaveInShortForm=s;
		for(int i=0;i<xml.GetNSubQuotes();i++){
			xmlQuote* sub=xml.GetSubQuote(i);
			xmlQuote* SUB1=XML.GetSubQuote( (char*)sub->GetQuoteName() );
			bool del=false;
			if(SUB1){
				if(!sub->GetNSubQuotes()){
					char* S=(char*)SUB1->Get_string();
					char* s=(char*)sub->Get_string();
					if( (s==NULL&&S==NULL) || (s&&S&&!strcmp(s,S)) ){
						del=true;
					}
				}
			}
			if(del){
				xml.DelSubQuote(i);
				i--;
			}
		}
		delete(temp);
	}
}
void BaseClass::Save(xmlQuote& xml,void* ClassPtr,void* Extra){	
	BaseClass* BC=(BaseClass*)ClassPtr;
	OneClassStorage* OCS=BC->GetClassStorage();
	//if(!OCS->Members.GetAmount())
	BC->RegisterMembers();	
	SetContext(BC);
	int N=BC->GetAmountOfElements();
	for(int i=0;i<N;i++){
		OneClassMemberStorage* OCMS=BC->GetElementCMS(i);
		if(OCMS==NULL||!OCMS->NoSaveMode){
			xmlQuote* xi=new_xmlQuote((char*)BC->GetElementID(i));
			void* mptr=BC->GetElementPtr(i,ClassPtr);
			void* eptr=BC->GetElementExtraPtr(i);
			if(!eptr)eptr=Extra;
			if(mptr){
				BaseClass* BC1=BC->GetElementClass(i);
				BC1->Save(*xi,mptr,eptr);
			}else{
				xi->Assign_string("NULL");
			}
			xml.AddSubQuote(xi);
		}
	}
	if(SaveInShortForm){
		DeleteDefaultSubFields(xml,ClassPtr,Extra);
	}
	const char* qn=xml.GetQuoteName();
	if(qn==NULL || qn[0]==0)xml.SetQuoteName(GetClassName());
}
void TestCGARB();
bool BaseClass::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	//TestCGARB();	
	BaseClass* BC=(BaseClass*)ClassPtr;
	OneClassStorage* OCS=BC->GetClassStorage();
	//if(!OCS->Members.GetAmount())
	BC->RegisterMembers();
	SetContext(BC);
	int N=xml.GetNSubQuotes();	
	bool WasLink=false;	
	for(int i=0;i<N;i++){
		//TestCGARB();
		xmlQuote* sub=xml.GetSubQuote(i);
		BaseClass* BASE;
		void* BasePtr;
		void* Extra2=NULL;
		//TestCGARB();
		if(BC->GetElementByName(sub->GetQuoteName(),&BASE,&BasePtr,&Extra2,ClassPtr)){
			//TestCGARB();
			if(!Extra2)Extra2=Extra;
			//TestCGARB();
			if(CheckDirectCasting()&&BASE->CheckDirectCasting())((BaseClass*)BasePtr)->SetParent((BaseClass*)ClassPtr);
			//TestCGARB();
			BASE->Load(*sub,BasePtr,Error,Extra2);			
			//TestCGARB();
			OneClassMemberStorage* OCMS=GetElementStorage(sub->GetQuoteName());
			if(OCMS){
				if(OCMS->IntPropertyW){
					int v=*((int*)BasePtr);
					OCMS->IntPropertyW((BaseClass*)ClassPtr,v);
				}else
					if(OCMS->GeneralPropertyW){
						OCMS->GeneralPropertyW(ClassPtr,BasePtr);
					}
			}
			_str* s=BC->GetObjectNamePointer();
			if(s&&s->pchar()&&s->pchar()[0]){
				if(!WasLink)CPGARB.TryToLinkClass(this);
				WasLink=true;

			}
			//TestCGARB();
		}else{
			Error->xml_print(xml,"Unknown identifier %s of class %s.\n",sub->GetQuoteName(),BC->GetClassName());            
		}
		//TestCGARB();
	}	
	_str* s=BC->GetObjectNamePointer();
	if(s&&s->pchar()&&s->pchar()[0]){
		if(!WasLink)CPGARB.TryToLinkClass(this);
	}	
	return true;
}
bool BaseClass::ReadFromFile(const char* Name){
	if(!CheckDirectCasting())return false;
	xmlQuote xml;
	if(xml.ReadFromFile((char*)Name)){
		ErrorPager err;
		CurrentSaveFile=Name;
		Load(xml,this,&err,NULL);
		CurrentSaveFile=NULL;
		return true;
	}
	return false;
}
bool BaseClass::SafeReadFromFile(const char* Name){
	if(!CheckDirectCasting())return false;
	xmlQuote xml;
	if(xml.ReadFromFile((char*)Name)){
		ErrorPager err(0);
		CurrentSaveFile=Name;
		Load(xml,this,&err,NULL);
		CurrentSaveFile=NULL;
		return true;
	}
	return false;
}
bool BaseClass::WriteToFile(const char* Name){
	if(!CheckDirectCasting())return false;
	CurrentSaveFile=Name;
	xmlQuote xml;
	SaveInShortForm=ShouldSaveInShortForm();
	Save(xml,this);
	SaveInShortForm=false;
	xml.WriteToFile((char*)Name);
	CurrentSaveFile=NULL;
	return true;
}
bool BaseClass::FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContext* C){
	if(SrcData==DestData)return true;
	DWORD M=GetClassMask();
	OneClassStorage* CST=GetClassStorage();	
	int N=0;
	if(CST)N=CST->Members.GetAmount();
	if(CheckDirectCasting() && N && CST){
		for(int i=0;i<N;i++)if(CST->Members[i]->Mask&M){			
			OneClassMemberStorage* OCMS=CST->Members[i];
			if(OCMS){
				void* d_ElmPtr;
				BaseClass* d_ClassPtr;
				void* d_ExtraPtr;
				if(Dest->GetElementByName(OCMS->xmlID,&d_ClassPtr,&d_ElmPtr,&d_ExtraPtr,DestData)){				
					void* Data=GetElementPtr(i+0x1000000,SrcData);
					BaseClass* BC=OCMS->Member;
					if(BC->CheckDirectCasting())BC=(BaseClass*)Data;
					if(d_ClassPtr->CheckDirectCasting())d_ClassPtr=(BaseClass*)d_ElmPtr;
					if(!strcmp(BC->GetClassName(),d_ClassPtr->GetClassName())){
						if(d_ElmPtr!=Data){
							BC->FullCopy(Data,OCMS->ExtraData,d_ClassPtr,d_ElmPtr,d_ExtraPtr,C);						
						}
						if(OCMS->IntPropertyW){
							int v=*((int*)d_ElmPtr);
							OCMS->IntPropertyW(Dest,v);
						}else
						if(OCMS->GeneralPropertyW){
							OCMS->GeneralPropertyW(DestData,d_ElmPtr);
						}
					}
				}
			}
		}
	}else{
		xmlQuote xml;
		Save(xml,SrcData,SrcDataExtra);
		ErrorPager EP(0);
		return Dest->Load(xml,DestData,&EP,DestExtraPtr);	
	}
	return true;
}

void ScanForRC(BaseClass* BC,CopyContext* C){
	_str* s=BC->GetObjectNamePointer();
	if(s && s->pchar()[0])C->LocalClassIDS.Add(BC->GetObjectGlobalID());
    int N=BC->GetAmountOfElements();
	for(int i=0;i<N;i++){
		BaseClass* B=BC->GetElementClass(i);
		if(B->CheckDirectCasting() && !strstr(B->GetClassName(),"ClassRef.")){
			void* Data=BC->GetElementPtr(i,BC);
			B=(BaseClass*)Data;			
			ScanForRC(B,C);
		}
	}
}
bool BaseClass::Copy(BaseClass* Dest, bool Add){
	if(!CheckDirectCasting())return false;
	if(strcmp(GetClassName(),Dest->GetClassName()))return false;
	if(!Add) Dest->reset_class(Dest);
	CopyContext C;
	ScanForRC(this,&C);
	return FullCopy(this,NULL,Dest,Dest,NULL,&C);	
}
bool ReferableBaseClass::FullCopy(void* SrcData,void* SrcDataExtra,BaseClass* Dest,void* DestData,void* DestExtraPtr,CopyContext* C){
	BaseClass::FullCopy(SrcData,SrcDataExtra,Dest,DestData,DestExtraPtr,C);
	_str* s=GetObjectNamePointer();
	if(s && s->pchar()[0]){
		_str* dn=Dest->GetObjectNamePointer();
		if(dn){
		//	(*dn)=s->pchar();
			// not repeat "_copy#"
			char* pCOPY = strstr(s->pchar(),"_copy#");
			if ( pCOPY!=NULL ){
				int len = (int)( pCOPY - s->pchar() )+1;
				char* pS = new char[len];
				strncpy( pS, s->pchar(), len-1 );
				pS[len-1]=0;
				(*dn)=pS;
				delete pS;
			}else{
				(*dn)=s->pchar();
			};
			// -------------------
			(*dn)+="_copy#";
			(*dn)+=abs(int(C->CopyGlobalID));
			CPGARB.TryToLinkClass(Dest);
		}
	}	
	return true;
}
//- special function for integrating in editor -
//Options: 1-enum leafs 2-enum nodes 4-enum adds, so 7-enum everything
int BaseClass::GetIndexOfEditableElement(int Index,void* DataPtr,DWORD Options){
	int NR=0;
	int N=GetAmountOfElements();
	if((Options&3)!=3){
		for(int i=0;i<N;i++){
			BaseClass* BC=GetElementClass(i);
			void* Data=GetElementPtr(i,DataPtr);
			bool Node=CheckIfElementIsNode(i,DataPtr);
			if(BC->CheckDirectCasting()){
				BC=(BaseClass*)Data;
				if(BC->ForceProperty())Node=false;
			}
			if(CheckDirectCasting()){
				BaseClass* B0=(BaseClass*)DataPtr;
				if(B0){
					if(!B0->GetParent())Node=true;
					if(B0->ForceProperty())Node=false;
				}
			}
			if((Node&&(Options&2))||((!Node)&&(Options&1))){
				if(NR==Index)return i;
				NR++;
			}
		}
	}else{
		if(Index<N)return Index;
		else return -1;
	}
	return -1;
}
int BaseClass::GetAmountOfEditableElements(void* DataPtr,DWORD Options){
	int NR=0;
	BaseClass* BCPTR=CheckDirectCasting()?((BaseClass*)DataPtr):this;
	int N=BCPTR->GetAmountOfElements();
	if(CheckDirectCasting()){
		BaseClass* B0=(BaseClass*)DataPtr;
		if(B0&&B0->ForceProperty()&&Options&1)Options|=4;
	}
	if((Options&3)!=3){
		for(int i=0;i<N;i++){
			BaseClass* BC=BCPTR->GetElementClass(i);
			void* Data=BCPTR->GetElementPtr(i,DataPtr);
			bool Node=BCPTR->CheckIfElementIsNode(i,DataPtr);
			if(BC->CheckDirectCasting()){
				BC=(BaseClass*)Data;
				if(BC->ForceProperty())Node=false;
			}
			if(CheckDirectCasting()){				
				BaseClass* B0=(BaseClass*)DataPtr;
				if(B0){
                    if(!B0->GetParent())Node=true;
					if(B0->ForceProperty())Node=false;
				}
			}
			if((Node&&(Options&2))||((!Node)&&(Options&1)))NR++;
		}
	}else NR=N;
	int exr=BCPTR->GetExpansionRules();
	if((exr&8)&&NR)return NR;
	return NR+int(exr!=0&&(Options&4)!=0);	
}
BaseClass* BaseClass::GetEditableElementClass(int Index,void* DataPtr,DWORD Option){
	int P=GetIndexOfEditableElement(Index,DataPtr,Option);
	if(P==-1)return NULL;
	return GetElementClass(P);	
}
void* BaseClass::GetEditableElementData(int Index,void* DataPtr,DWORD Option){
	int P=GetIndexOfEditableElement(Index,DataPtr,Option);
	if(P==-1)return NULL;
	return GetElementPtr(P,DataPtr);		
}
bool BaseClass::CheckIfElementReadOnly(int Index,void* DataPtr,DWORD Option){
	int P=GetIndexOfEditableElement(Index,DataPtr,Option);
	if(P==-1)return false;
	Index=GetRealMaskedPosition(P);
	OneClassStorage* CST=GetClassStorage();	
	if(CST&&Index>=0&&Index<CST->Members.GetAmount()){		
		return CST->Members[Index]->ReadOnly;		
	}
	return false;
}
bool BaseClass::CheckIfElementInvisible(int Index,void* DataPtr,DWORD Option){
	int P=GetIndexOfEditableElement(Index,DataPtr,Option);
	if(P==-1)return false;
	Index=GetRealMaskedPosition(P);
	OneClassStorage* CST=GetClassStorage();	
	if(CST&&Index>=0&&Index<CST->Members.GetAmount()){		
		return CST->Members[Index]->Invisible;		
	}
	return false;
}
void* BaseClass::GetEditableElementExtra(int Index,void* DataPtr,DWORD Option){
	int P=GetIndexOfEditableElement(Index,DataPtr,Option);
	if(P==-1)return NULL;
	return GetElementExtraPtr(P);		
}
bool BaseClass::GetEditableElementExpansionList(ClassExpParams& EXP,int Index,void* DataPtr,DWORD Option,BaseClass* Parent,const char* ElmName){	
	int exr=GetExpansionRules();
	if(CheckDirectCasting())SetContext((BaseClass*)DataPtr);
	int P=GetIndexOfEditableElement(Index,DataPtr,Option);
	if((exr&8)||(P==-1&&exr&3)){
		const char* bclass=(CheckDirectCasting()?(BaseClass*)DataPtr:this)->GetExpansionBaseClass();
		if(bclass){
			OneClassStorage* OCS=CGARB.GetClass(bclass);
			if(OCS){
				if(exr&1){
					bool ADD=1;
					if(Parent&&ElmName){
						ADD=Parent->AskParentForUsingExpansionClass((char*)ElmName,(char*)bclass);
					}
					if(ADD){
						EXP.ExpList.Add((char*)bclass);
						OneClassStorage* OCS1=CGARB.GetClass((char*)bclass);
						int tidx=-1;
						if(OCS1){
							if(OCS1->ClassTopic){
								tidx=EXP.TopicsList.GetAmount();
								EXP.TopicsList.Add(OCS1->ClassTopic);
							}
						}
						EXP.TopicsIdxs.Add(tidx);
					}
				}
				if(exr&2){
					int N=OCS->Children.GetAmount();
					for(int j=0;j<N;j++){
						char* CLASS=OCS->Children[j];
						if(strcmp(CLASS,bclass)){
							bool ADD=1;
							if(Parent&&ElmName){
								ADD=Parent->AskParentForUsingExpansionClass((char*)ElmName,(char*)CLASS);
							}
							if(ADD){
								EXP.ExpList.Add(CLASS);
								OneClassStorage* OCS1=CGARB.GetClass(CLASS);
								int tidx=-1;
								if(OCS1){
									if(OCS1->ClassTopic){
										for(int q=0;q<EXP.TopicsList.GetAmount();q++)if(!strcmp(EXP.TopicsList[q],OCS1->ClassTopic))tidx=q;
										if(tidx==-1){
											tidx=EXP.TopicsList.GetAmount();
											EXP.TopicsList.Add(OCS1->ClassTopic);
										}
									}
								}
								EXP.TopicsIdxs.Add(tidx);
							}
						}
					}
				}
				if(EXP.TopicsList.GetAmount()<=1){
					EXP.TopicsList.Clear();
				}
				if(EXP.ExpList.GetAmount())return true;
			}
		}
	}
	return false;
}
bool BaseClass::CheckIfElementIsNode(int Index,void* DataPtr){
	if(Index>=GetAmountOfElements())return false;
	BaseClass* BC=GetElementClass(Index);
	void* ptr=GetElementPtr(Index,DataPtr);
	if(BC&&ptr){
		if(BC->GetAmountOfEditableElements(ptr,7))return true;
	}
	return false;
}
bool BaseClass::CheckIfEditableElementIsNode(int Index,void* DataPtr,DWORD Option){
	int P=GetIndexOfEditableElement(Index,DataPtr,Option);
	if(P==-1)return false;
	return CheckIfElementIsNode(P,DataPtr);
}
const char* BaseClass::GetEditableElementView(int Index,void* DataPtr,const char* LocalName,DWORD Option){
	int P=GetIndexOfEditableElement(Index,DataPtr,Option);
	if(P==-1)return "add...";
	return GetElementView(P,LocalName);
}
const char* BaseClass::GetEditableElementName(int Index,void* DataPtr,DWORD Option){
	int P=GetIndexOfEditableElement(Index,DataPtr,Option);
	if(P==-1)return "add...";
	return GetElementID(P);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
//~~~~~~~~~~~_int~~~~~~~~~~~~/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
void _int::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	int* V=(int*)ClassPtr;
	xml.Assign_int(*V);
}
bool _int::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	int* V=(int*)ClassPtr;
	const char* c=xml.Get_string();
	*V=0;
	int z=0;
	if(c)z=sscanf(c,"%d",V);
	if(z!=1){
		Error->xml_print(xml,"unable to read DWORD from XML: %s\n",xml.Get_string());
		return false;
	}
	return true;
};
const char* _int::GetClassName(){
	return "_int";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
//~~~~~~~~~~~_DWORD~~~~~~~~~~~~/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
void _DWORD::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	DWORD* V=(DWORD*)ClassPtr;
	char c[16];
	sprintf(c,"%08X",*V);
	xml.Assign_string(c);
}
bool _DWORD::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	DWORD* V=(DWORD*)ClassPtr;
	const char* c=xml.Get_string();
	*V=0;
	int z=0;
	if(c)z=sscanf(c,"%X",V);
	if(z!=1){
		Error->xml_print(xml,"unable to read DWORD from XML: %s\n",xml.Get_string());
		return false;
	}
	return true;
};
const char* _DWORD::GetClassName(){
	return "_DWORD";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
//~~~~~~~~~~~_float~~~~~~~~~~~~/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
void _float::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	float* V=(float*)ClassPtr;
	char c[256];
	sprintf(c,"%f",*V);
	xml.Assign_string(c);
}
bool _float::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	float* V=(float*)ClassPtr;
	const char* c=xml.Get_string();
	*V=0;
	int z=0;
	if(c)z=sscanf(c,"%f",V);
	if(z!=1){
		Error->xml_print(xml,"unable to read float from XML: %s\n",xml.Get_string());
		return false;
	}
	return true;
};
const char* _float::GetClassName(){
	return "_float";
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
//~~~~~~~~~~~_bool~~~~~~~~~~~~/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
void _bool::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	bool* V=(bool*)ClassPtr;
	char c[16];
	if (*V) strcpy( c, "true" ); else strcpy( c, "false" );
	xml.Assign_string(c);
}

bool _bool::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra)
{
	bool* V=(bool*)ClassPtr;
	const char* c=xml.Get_string();
	*V=false;
	if(c){
		while (*c == ' ') c++;
		if (!_strnicmp( c, "true", 4 )) *V = true;
	}
	return true;
};
const char* _bool::GetClassName(){
	return "_bool";
}
//_str
_str::~_str(){
	DString::~DString();
}
void _str::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	_str* s=(_str*)ClassPtr;
	if(s->pchar())xml.Assign_string(s->pchar());
}
bool _str::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	_str* s=(_str*)ClassPtr;
	s->Clear();
	s->Add((char*)xml.Get_string());
	return true;
}
const char* _str::GetClassName(){
	return "_str";
}
//end _str
//_str_ref
const char* _str_ref::GetClassName(){
	return "_str_ref";
}
bool _str_ref::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* extra){
	ClearRef();
	return _str::Load(xml,ClassPtr,Error,extra);
}
ReferableBaseClass*	_str_ref::Get(){

	ReferableBaseClass* retVAL = NULL;

	ID = ( ((ID!=0xFFFFFFFF) && ((ID&0xFFFFFF)>=CPGARB.CPointer[ID>>24].GetAmount() || CPGARB.CPointer[ID>>24][ID&0xFFFFFF]->ClassPtr==NULL)) ? (0xFFFFFFFF) : (ID) );

	if ( ID==0xFFFFFFFF && ( pchar()[0]==0 || strcmp("NULL",pchar())==0 ) ) return NULL;

	if (ID==0xFFFFFFFF) {
		Enumerator* E = NULL;
		BaseClass* pPAR = GetParent();
		if (pPAR!=NULL) {
			OneClassStorage* pOCS = pPAR->GetClassStorage();
			if (pOCS!=NULL) {
				int		curOffset = ( (int)this - (int)pPAR );
				DWORD	curMask = pPAR->GetClassMask();
				for(int i=0;i<pOCS->Members.GetAmount();i++){							
					OneClassMemberStorage* pOCMS=pOCS->Members[i];
					if (pOCMS!=NULL) {
						if (pOCMS->OffsetFromClassRoot==curOffset&&(pOCMS->Mask&curMask)){
							E = (Enumerator*)(pOCMS->ExtraData);
						};
					};
				};
			};
		};
		if (E!=NULL&&pPAR!=NULL&&pchar()!=NULL&&(pchar())[0]!=0) {
			BaseClass::SetContext(pPAR);
			ID = E->Get(pchar());
		};
	};

	if (ID!=0xFFFFFFFF) {
		OneClassPointer* CP=CPGARB.CPointer[ID>>24][ID&0xFFFFFF]; 
		if (CP!=NULL&&CP->ClassPtr!=NULL) {
			CPGARB.AddRef(ID);
			retVAL = (ReferableBaseClass*)(CP->ClassPtr);
		};
	};

	BaseClass* pBASE = GetParent();
	while ( pBASE!=NULL && ID!=0xFFFFFFFF ) {
		_str strCN = pBASE->GetClassName();
		if ( strCN.pchar()[0]!=0 && strcmp(strCN.pchar(),"be_CParamFunction")==0 ){
			ID = 0xFFFFFFFF;
		};
		pBASE = pBASE->GetParent();
	};
	
	return retVAL;
};
void				_str_ref::SetEnumName(const char* enum_name/*=NULL*/){
	if ( enum_name!=NULL && enum_name[0]!=0 ){
		ENUM_NAME = enum_name;
	};	
};
bool				_str_ref::ClearRef(){
	if (ID!=0xFFFFFFFF){
		CPGARB.DelRef(ID);
		ID=0xFFFFFFFF;
	};
	return true;
};
_str_ref&			_str_ref::operator=(const _str_ref& sref){ 
	if ( ((char*)sref.cpchar())[0] ){
		ClearRef();
		Clear();
		Add(sref.cpchar()); 
		if ( sref.ENUM_NAME.cpchar()[0] ) ENUM_NAME=sref.ENUM_NAME;
		if ( sref.ID!=0xffffffff ) ID=sref.ID;
	};
	return *this;
};
void				_str_ref::SetRef(char* _name,DWORD _id){
	ID = _id;
	Assign(_name);
};
void				_str_ref::Set(ReferableBaseClass* pOB){
	if ( pOB && pOB->Name.pchar() && pOB->Name.pchar()[0] ){
		SetRef( pOB->Name.pchar(), pOB->GetObjectGlobalID() );
	};
};
bool				_str_ref::OnChangeMember(BaseClass* MembClass,void* MembPtr,void* MembExtra,const char* MembName){
	ID = 0xFFFFFFFF;	
	return false;
};
//end _str_ref
void OneClassMemberStorage::CheckValidRegistration(void* Base,const char* MemName){
	if(StaticMode){
		if(OffsetFromClassRoot<1000000){
			PrintError("CRITICAL: Non-static member <%s %s> registered like static",Member->GetClassName(),MemName);
		}
	}else{
		if(abs(OffsetFromClassRoot)>=1000000){
			PrintError("CRITICAL: Static member <%s %s> registered like non-static",Member->GetClassName(),MemName);
		}
	}
}
int num_OneClassMemberStorage=0;
OneClassMemberStorage::OneClassMemberStorage(){
	num_OneClassMemberStorage++;
	Member=NULL;
	xmlID=NULL;
	UseReference=0;
}
OneClassMemberStorage::~OneClassMemberStorage(){
	num_OneClassMemberStorage--;
	if(Member)delete(Member);
	if(xmlID)free(xmlID);
	Member=NULL;
	xmlID=NULL;
}
void Enumerators::ClearAll(){
	Enums.Clear();
	EnumNames.clear();
}    
Enumerator* Enumerators::Get(const char* Name){
	int idx=EnumNames.find(Name);
	if(idx!=-1){
		return Enums[idx];
	}else{
		return Create(Name);
	}
}
Enumerator* Enumerators::Create(const char* Name){
	Enumerator* E;
	SAFENEW
		E=new Enumerator;		
	Enums.Add(E);
	EnumNames.add(Name);
	E->EnumeratorID=EnumNames.get(EnumNames.find(Name));
	ENDNEW;
	return E;
}
OneClassStorage* ClassGarbage::GetClass(const char* ClassName){
	int idx=NamesHash.find(ClassName);
	if(idx!=-1)return Storage[idx];
	return CreateClassStorage(ClassName);
}
OneClassStorage* ClassGarbage::CreateClassStorage(const char* ClassName){
	OneClassStorage* ST;
	SAFENEW;
	NamesHash.add(ClassName);
	ST=new OneClassStorage;
	Storage.Add(ST);
	ENDNEW;
	return ST;
}
char* GetGlobalBuffer(){
	static char cc[65536*8];
	return cc;
}
void ErrorPager::print(char* mask,...){
	va_list args;
	va_start(args,mask);
	char temp[512];
	vsprintf(temp,mask,args);
	va_end(args);
	Msg.Add(temp);
#ifdef IMMEDIATE_ERROR
	if(BreakExecution){
		if(MessageBox(NULL,temp,"Class engine error",MB_ICONERROR|MB_OKCANCEL)==IDCANCEL){
			exit(1);
		}
	}
#endif //IMMEDIATE_ERROR
}
DIALOGS_API void PrintError(char* mask,...){
	va_list args;
	va_start(args,mask);
	char temp[512];
	vsprintf(temp,mask,args);
	va_end(args);
#ifdef IMMEDIATE_ERROR
	if(MessageBox(NULL,temp,"Class engine error",MB_ICONERROR|MB_OKCANCEL)==IDCANCEL){
		assert(1);
	}
#endif //IMMEDIATE_ERROR
}
void ErrorPager::xml_print(xmlQuote& xml,char* mask,...){
	if(BreakExecution){
		va_list args;
		va_start(args,mask);
		char temp[1024];
		vsprintf(temp,mask,args);
		va_end(args);
		DString D;
		xml.GetXMLSource(&D);
		if(strlen(D.pchar())>500){
			D.pchar()[500]=0;
			D.Add("...");
		}
		sprintf(temp+strlen(temp),"XML source:\n\n%s\n",D.pchar());
		Msg.Add(temp);
#ifdef IMMEDIATE_ERROR
		if(MessageBox(NULL,temp,"Class engine error",MB_ICONERROR|MB_OKCANCEL)==IDCANCEL){
			exit(1);			
		}
#endif //IMMEDIATE_ERROR
	}
}
OneClassMemberStorage* OneClassStorage::CreateMember(const char* ID){
	OneClassMemberStorage* M=new OneClassMemberStorage;
	MembIDS.add(ID);
	Members.Add(M);
	M->xmlID=NEW(char,strlen(ID)+1);
	strcpy(M->xmlID,ID);
	return M;
}
bool	OneClassStorage::CheckChildren(const char* ChildrenName){
	return CheckString(&Children,ChildrenName);
};
bool	OneClassStorage::CheckParent(const char* ParentName){
	return CheckString(&Parents,ParentName);
};
bool	OneClassStorage::CheckAll(const char* string, bool inParent, bool inChildren, bool FullCompatibility){
	bool ret = false;
	if ( inParent ){
		ret = CheckString(&Parents,string,FullCompatibility);
		if (ret) return true;
	};
	if ( inChildren ){
		ret = CheckString(&Children,string,FullCompatibility);
		if (ret) return true;
	};
	return false;
};
bool	OneClassStorage::CheckString( DynArray<char*>* pList, const char* string, bool FullCompatibility ){
	if ( pList==NULL  || pList->GetAmount()==0 )	return false;
	if ( string==NULL || string[0]==0 )				return false;
	for ( int i=0; i<pList->GetAmount(); i++ ){
		if (FullCompatibility) {
			if ( ( (*pList)[i]!=NULL && ((*pList)[i])[0]!=0 ) && strcmp((*pList)[i],string)==0 ) return true;
		}else{
			if ( ( (*pList)[i]!=NULL && ((*pList)[i])[0]!=0 ) && strstr((*pList)[i],string) ) return true;
		}
	};
	return false;
};
SimpleProcEnumerator::SimpleProcEnumerator(fnGetValueForEnumerator* fn){
	Value=fn;
}
DWORD SimpleProcEnumerator::GetValue(const char* ID){
	if(!Value)return -1;
	bool valid;
	int pos=0;
	do{
		EnumeratorContext EC(pos++);
		EnumeratorOutput  EO;
		valid=Value(&EC,&EO);
		if(valid){
			if(EO.String && !strcmp(EO.String,ID))return EO.Value;
		}
	}while(valid);
	return -1;
}	
char* SimpleProcEnumerator::GetValue(DWORD ID){
	if(!Value)return "";
	bool valid;
	int pos=0;
	do{
		EnumeratorContext EC(pos++);
		EnumeratorOutput  EO;
		valid=Value(&EC,&EO);
		if(valid){
			if(EO.Value==ID)return EO.String;
		}
	}while(valid);
	return "";	
}
int SimpleProcEnumerator::GetAmount(){
	if(!Value)return 0;
	EnumeratorContext EC(0);
	EnumeratorOutput  EO;
	Value(&EC,&EO);
	return EO.Amount;
}
char* SimpleProcEnumerator::GetIndexedString(int idx){
	if(!Value)return "";
	EnumeratorContext EC(idx);
	EnumeratorOutput  EO;
	bool valid=Value(&EC,&EO);
	if(valid)return EO.String;
	else return "";
}
DWORD SimpleProcEnumerator::GetIndexedValue (int idx){
	if(!Value)return -1;
	EnumeratorContext EC(idx);
	EnumeratorOutput  EO;
	bool valid=Value(&EC,&EO);
	if(valid)return EO.Value;
	else return -1;	
}
char* SimpleProcEnumerator::GetCategory(int idx){
	if(!Value)return NULL;
	EnumeratorContext EC(idx);
	EnumeratorOutput  EO;
	bool valid=Value(&EC,&EO);
	if(valid)return EO.Topic;
	else return NULL;
}
void EnumeratorOutput::Init(){
	Amount=0;
	String=NULL;
	Value=0;
	Topic=NULL;
	StringAllocated=false;
	TopicAllocated=false;
}
void EnumeratorOutput::Free(){
	if(StringAllocated&&String)delete[](String);
	if(TopicAllocated&&Topic)delete[](Topic);
	Init();
}
EnumeratorOutput::EnumeratorOutput(){
	Init();
}
EnumeratorOutput::~EnumeratorOutput(){
	Free();
}
EnumeratorContext::EnumeratorContext(int _ValueIndex){
	Context    = BaseClass::GetContext();
	ValueIndex = _ValueIndex;
}


EnumeratorOutputStream::EnumeratorOutputStream(){
	Hash=0xBAADF00D;
	FN=NULL;
	RealAmount=0;
}
EnumeratorOutputStream::~EnumeratorOutputStream(){
	for(int i=0;i<Items.GetAmount();i++){
		EnumeratorOutputStream::StreamItem& si=Items[RealAmount];
		delete[]si.string;
		if(si.topic)delete[]si.topic;
	}
}
void EnumeratorOutputStream::add(const char* string,DWORD value,const char* topic){
	if(RealAmount<Items.GetAmount()){
		EnumeratorOutputStream::StreamItem& si=Items[RealAmount];
		if(value!=si.value || ( strcmp(string,si.string) ) ){
			si.value=value;
			delete[]si.string;
			si.string=new char[strlen(string)+1];
			strcpy(si.string,string);
			if(si.topic)delete[]si.topic;
			si.topic=NULL;
			if(topic){
				si.topic=new char[strlen(topic)+1];
				strcpy(si.topic,topic);
			}
		}
	}else{
		EnumeratorOutputStream::StreamItem si;
		si.value=value;		
		si.string=new char[strlen(string)+1];
		strcpy(si.string,string);
		si.topic=NULL;
		if(topic){
			si.topic=new char[strlen(topic)+1];
			strcpy(si.topic,topic);
		}
		Items.Add(si);
	}
	RealAmount++;
}
int g_StringCompare( const void *arg1, const void *arg2 ){
	EnumeratorOutputStream::StreamItem* el1 = (EnumeratorOutputStream::StreamItem*)arg1;
	EnumeratorOutputStream::StreamItem* el2 = (EnumeratorOutputStream::StreamItem*)arg2;
	if ( strcmp(el1->string,"NULL")==0 ) return 0;
	return _stricmp( el1->string, el2->string );
};
void EnumeratorOutputStream::sortByString(){
	qsort	(	(void *)(Items.Values), 
		(size_t)(RealAmount), 
		sizeof( EnumeratorOutputStream::StreamItem ), 
		g_StringCompare
		);
};


RefreshableEnumeratorContext::RefreshableEnumeratorContext(){
	Context=BaseClass::GetContext();
};

void RefreshableEnumerator::RefreshStream(){
	int amount=Stream.RealAmount;
	Stream.RealAmount=0;	
	RefreshableEnumeratorContext RC;
	DWORD HASH=Stream.FN(&RC,&Stream,Stream.Hash);
	if(HASH==Stream.Hash){
		Stream.RealAmount=amount;
	}
	Stream.Hash=HASH;
}
DWORD RefreshableEnumerator::GetValue(const char* ID){
	RefreshStream();
	for(int i=0;i<Stream.RealAmount;i++){
		if(!strcmp(ID,Stream.Items[i].string))return Stream.Items[i].value;
	}
	return 0xFFFFFFFF;
}
char* RefreshableEnumerator::GetValue(DWORD ID){
	RefreshStream();
	for(int i=0;i<Stream.RealAmount;i++){
		if(ID==Stream.Items[i].value)return Stream.Items[i].string;
	}
	return "";
}
int   RefreshableEnumerator::GetAmount(){
	RefreshStream();
	return Stream.RealAmount;
}
char* RefreshableEnumerator::GetIndexedString(int idx){
	RefreshStream();
	return Stream.Items[idx].string;
}
DWORD RefreshableEnumerator::GetIndexedValue (int idx){
	RefreshStream();
	return Stream.Items[idx].value;
}
char* RefreshableEnumerator::GetCategory(int idx){
	RefreshStream();
	return Stream.Items[idx].topic;
}
RefreshableEnumerator::RefreshableEnumerator(fnRecreateEnumerator* fn){
	Stream.FN=fn;
}
DIALOGS_API void TestClassStorage(OneClassStorage** OCS);
DIALOGS_API void reg_class_bc(BaseClass* BC,char* Topic,char* Typeid,bool AutoMode){	
	static DString D;
	D=Typeid;	
	ConvTypeName(D);
	if(AutoMode && strstr(D.pchar(),"."))return;
	OneClassStorage* OCS=CGARB.GetClass(D.pchar());
	TestClassStorage(&OCS);
	if(!OCS)OCS=CGARB.CreateClassStorage(D.pchar());	
	if(OCS->OneMemb){
		try{
            OCS->OneMemb->GetClassName();
		}catch(...){
            OCS->OneMemb=NULL;
			OCS->Members.Clear();
		}
	}
	if(!OCS->OneMemb){
		BaseClass* elm=(BaseClass*)BC->new_element();
		if(!elm)return;
		OCS->OneMemb=elm;
	}
	if(!OCS->Members.GetAmount()){
		OCS->OneMemb->RegisterMembers();
	}
	if(Topic&&!OCS->ClassTopic){
		OCS->ClassTopic=new char[strlen(Topic)+1];
		strcpy(OCS->ClassTopic,Topic);
	}	
}

DIALOGS_API ClassGarbage CGARB;
DIALOGS_API ClassPointersGarbage CPGARB;
const char* BaseClass::CurrentSaveFile=NULL;
bool BaseClass::ReadOnlyMode=false;
bool BaseClass::InvisibleMode=false;
bool BaseClass::SaveInShortForm=false;
bool BaseClass::NoSaveMode=false;
bool BaseClass::StaticMode=false;
BaseClass* BaseClass::CurrentContext=NULL;
#include <xmlQuote.hpp>

#ifdef __STDAPPLICATION__

char* GetTextByID(char* x){
	return x;
}
bool GetHintDebugMode(){
	return false;
}
void PushSmartLeak(bool& v){
}
void PopSmartLeak(bool& v){
}
#endif //__STDAPPLICATION__

#include <pool.hpp>

//_color
void _color::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	DWORD* V=(DWORD*)ClassPtr;
	char c[16];
	sprintf(c,"%08X",*V);
	xml.Assign_string(c);
}
bool _color::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	DWORD* V=(DWORD*)ClassPtr;
	const char* c=xml.Get_string();
	*V=0;
	int z=0;
	if(c)z=sscanf(c,"%X",V);
	if(z!=1){
		Error->xml_print(xml,"unable to read DWORD from XML: %s\n",xml.Get_string());
		return false;
	}
	return true;
};
const char* _color::GetClassName(){
	return "_color";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~ClassPointersGarbage~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
int ClassPointersGarbage::GetBranch(const char* ClassName){
	int L=strlen(ClassName);
	DWORD S=0;
	for(int i=0;i<L;i++)S+=ClassName[i]*(i+1);
	return S%nCP_Branches;
}
DynArray<OneClassPointer*>& ClassPointersGarbage::GetCPointer(char* ClassName){
	return CPointer[GetBranch(ClassName)];
}
void TestCGARB();
DWORD ClassPointersGarbage::AddClass(BaseClass* Ptr){
	if(Ptr->GetObjectNamePointer()){
		int B=GetBranch(Ptr->GetClassName());
		DynArray<OneClassPointer*>& FPA=FreePtrs[B];
		if(FPA.GetAmount()){
			OneClassPointer* CP=FPA[FPA.GetAmount()-1];
			FPA.Del(FPA.GetAmount()-1,1);
			CP->NamePtr=Ptr->GetObjectNamePointer();	
			CP->ClassPtr=Ptr;			
			CP->ROOT=Ptr->GetRoot();
			CP->RefCount=0;
			Ptr->SetObjectGlobalID(CP->Index);
			return CP->Index;
		}
		OneClassPointer* CP;
		SAFENEW;
		CP=new OneClassPointer;
		ENDNEW;
		CP->NamePtr=Ptr->GetObjectNamePointer();	
		CP->ClassPtr=Ptr;
		CP->Index=CPointer[B].GetAmount()+(B<<24);
		CP->ROOT=Ptr->GetRoot();
		CP->RefCount=0;
		SAFENEW;
		CPointer[B].Add(CP);
		ENDNEW;
		Ptr->SetObjectGlobalID(CP->Index);
		return CP->Index;
	}
	Ptr->SetObjectGlobalID(0xFFFFFFFF);
	return (DWORD)-1;
}
void  ClassPointersGarbage::DeleteClass(BaseClass* Ptr){
	if(Ptr->GetObjectNamePointer()){
		DWORD ID=Ptr->GetObjectGlobalID();		
		if(ID!=0xFFFFFFFF){
			int B=ID>>24;
			OneClassPointer* CP=CPointer[B][ID&0xFFFFFF];
			if(CP->ClassPtr){
				CP->ClassPtr=NULL;
				CP->NamePtr=NULL;
				if(CP->RefCount==0){
					CP->ROOT=NULL;
					CP->RefName.Free();
					SAFENEW;
					//for(int i=0;i<FreePtrs.GetAmount();i++)assert(FreePtrs[i]->Index!=CP->Index);
					FreePtrs[B].Add(CP);
					ENDNEW;
				}
			}
		}
	}
}
DWORD ClassPointersGarbage::AddRef(char* ClassID,char* MemberName,BaseClass* Root){
	BaseClass* ROOT=Root->GetRoot();
	int B=GetBranch(ClassID);
	DynArray<OneClassPointer*>& CPA=CPointer[B];
    int besti=-1;
	for(int i=0;i<CPA.GetAmount();i++){
		try{
			OneClassPointer* CP=CPA[i];
			if(CP->ClassPtr&&!strcmp(CP->ClassPtr->GetClassName(),ClassID)
				&&CP->NamePtr&&CP->NamePtr->pchar()&&!strcmp(CP->NamePtr->pchar(),MemberName)){
                    besti=i;
					if(CP->ClassPtr->CheckIfObjectIsGlobal()||ROOT==CP->ClassPtr->GetRoot()){
						CPA[i]->RefCount++;
						return i+(B<<24);
					}
				}
		}catch(...){};
	}
    if(besti!=-1){
        CPA[besti]->RefCount++;
        return besti+(B<<24);
    }
	for(int i=0;i<CPA.GetAmount();i++){
		try{
			OneClassPointer* CP=CPA[i];
			if(CP->ClassPtr==NULL&&CP->NamePtr==NULL
				&&CP->RefName.pchar()[0]&&!strcmp(CP->RefName.cpchar(),MemberName)
				&&CP->ClassName.pchar()[0]&&!strcmp(CP->ClassName.cpchar(),ClassID)){
					CP->RefCount++;
					return CP->Index; 
				}  
		}catch(...){};
	}
	OneClassPointer* CP;
	DynArray<OneClassPointer*>& FPA=FreePtrs[B];
	if(FPA.GetAmount()){
		CP=FPA[FPA.GetAmount()-1];
		FPA.Del(FPA.GetAmount()-1,1);
	}else{
		SAFENEW;
		CP=new OneClassPointer;		
		CP->Index=CPA.GetAmount()+(B<<24);
		CPA.Add(CP);
		ENDNEW;
	}
	CP->FirstRef=Root;
	CP->NamePtr=NULL;
	CP->ClassPtr=NULL;
	CP->RefName=MemberName;
	CP->ClassName=ClassID;	
	CP->ROOT=ROOT;
	CP->RefCount=1;	
	return CP->Index;
}
void ClassPointersGarbage::DelRef(DWORD ID){  
	if(ID!=0xFFFFFFFF){
		int B=ID>>24;
		ID&=0xFFFFFF;
		int N=(--CPointer[B][ID]->RefCount);
		assert(N>=0);
		OneClassPointer* CP=CPointer[B][ID];
		if(N==0&&CP->ClassPtr==NULL){
			CP->ClassPtr=NULL;
			CP->NamePtr=NULL;			
			CP->ROOT=NULL;
			CP->RefName="";
			SAFENEW;
			//for(int i=0;i<FreePtrs.GetAmount();i++)assert(FreePtrs[i]->Index!=CP->Index);
			FreePtrs[B].Add(CP);			
			ENDNEW;
		}
	}
}
void ClassPointersGarbage::AddRef(DWORD ID){    
	if(ID!=0xFFFFFFFF){
		int B=ID>>24;
		ID&=0xFFFFFF;
		CPointer[B][ID]->RefCount++;
	}
}
int testv=0;
DWORD ClassPointersGarbage::TryToLinkClass(BaseClass* ClassPtr){
	if(ClassPtr->GetObjectNamePointer()){
		BaseClass* ROOT=ClassPtr->GetRoot();
		int B=GetBranch(ClassPtr->GetClassName());
		DynArray<OneClassPointer*>& CPA=CPointer[B];
		char* s=ClassPtr->GetObjectNamePointer()->pchar();
		if(s&&s[0]){
			DWORD FRef=0xFFFFFFFF;
			for(int i=0;i<CPA.GetAmount();i++){
				try{
					OneClassPointer* CP=CPA[i];
					if(!CP->ClassPtr){
						if(CP->ClassName.pchar()[0]&&CP->RefName.pchar()[0]
							&&(!strcmp(CP->ClassName.pchar(),ClassPtr->GetClassName()))
							&&(!strcmp(CP->RefName.pchar(),s))/*&&(ClassPtr->CheckIfObjectIsGlobal()||CP->ROOT==ROOT)*/){
								if(FRef!=0xFFFFFFFF){
									ClassRef<BaseClass>* CR=(ClassRef<BaseClass>*)CP->FirstRef;
									if(CR){
										DelRef(CR->CPG_Index);
										CR->CPG_Index=FRef+(B<<24);
										CP->FirstRef=NULL;
										AddRef(FRef);
									}
								}else{
									CP->ClassPtr=ClassPtr;
									CP->NamePtr=ClassPtr->GetObjectNamePointer();							
									CPGARB.DeleteClass(ClassPtr);
									ClassPtr->SetObjectGlobalID(i+(B<<24));
									CP->Index=i+(B<<24);
									CP->FirstRef=NULL;
									FRef=i+(B<<24);
								}
							}
					}
				}catch(...){
					static bool once=true;
					if(once){
						Log.Warning("TryToLinkClass failed!");
						once=false;
					}
				};
			}
		}		
	}
	return 0xFFFFFFFF;
}
ClassPointersGarbage::~ClassPointersGarbage(){
	for(int j=0;j<nCP_Branches;j++){
		for(int i=0;i<CPointer[j].GetAmount();i++){
			try{
				delete(CPointer[j][i]);
			}catch(...){};
		}
		CPointer[j].Clear();
	}	
}
DWORD ClassPointersGarbage::FindClass(BaseClass* ClassPtr){
	int B=GetBranch(ClassPtr->GetClassName());
	DynArray<OneClassPointer*>& CPA=CPointer[B];
	for(int i=0;i<CPA.GetAmount();i++)if(CPA[i]->ClassPtr==ClassPtr)return i;
	return (DWORD)-1;
}
#define IMPLEMENT_SIMPLETYPE(x)\
	void _##x::Save(xmlQuote& xml,void* ClassPtr,void* Extra){\
	x* w=(x*)ClassPtr;\
	xml.Assign_int(*w);\
	}\
	bool _##x::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){\
	x* w=(x*)ClassPtr;\
	int i;\
	int z=sscanf(xml.Get_string(),"%d",&i);\
	if(z==1){\
	*w=(x)i;\
	return true;\
	}else{\
	Error->xml_print(xml,"unable to read "#x);\
	return false;\
	}\
	}\
	const char* _##x::GetClassName(){\
	return "_"#x;\
	}
IMPLEMENT_SIMPLETYPE(WORD);
IMPLEMENT_SIMPLETYPE(short);
IMPLEMENT_SIMPLETYPE(char);
IMPLEMENT_SIMPLETYPE(BYTE);
//end simple types
#ifdef _USE3D
#ifndef NO_COSS_EXT
extern GP_System GPS;
void _gframe::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	char s[256];
	char s1[128];
	strcpy(s1,GPS.GetGPPath(FileID));
	char* c=strchr(s1,'.');
	if(c)*c=0;
	c=strstr(s1,"Cash\\");
	if(c)strcpy(s1,s1+5);	
	sprintf(s,"%s %d",s1,SpriteID);
	xml.Assign_string(s);
}
bool _gframe::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	char fname[128];
	int frame;
	int z=sscanf(xml.Get_string(),"%s%d",fname,&frame);
	if(z==2){
		FileID=(WORD)GPS.PreLoadGPImage(fname);
		SpriteID=(WORD)frame;
		return true;
	}else{
		Error->xml_print(xml,"unable to read frame");
		return false;
	}
}
const char* _gframe::GetClassName(){
	return "_gframe";
}
//
void _gpfile::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	char s[256];
	char s1[128];
	short v=*((short*)ClassPtr);
	if(v!=-1){
		strcpy(s1,GPS.GetGPName(v));
		/*
		char* c=strchr(s1,'.');
		if(c)*c=0;
		c=strstr(s1,"Cash\\");
		if(c)strcpy(s1,s1+5);
		*/
		xml.Assign_string(s1);
	}
}
bool _gpfile::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	char* s=(char*)xml.Get_string();
	*((WORD*)ClassPtr)=s?(WORD)GPS.PreLoadGPImage(s):0;
	return true;
}
const char* _gpfile::GetClassName(){
	return "_gpfile";
}
//
void _picfile::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	//char s[256];
	//char s1[128];
	char* v=((_str*)ClassPtr)->pchar();
	if(v&&v[0]){
		//strcpy(s1,GPS.GetGPName(v));
		/*
		char* c=strchr(s1,'.');
		if(c)*c=0;
		c=strstr(s1,"Cash\\");
		if(c)strcpy(s1,s1+5);
		*/
		xml.Assign_string(v);
	}
}
bool _picfile::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	*((_str*)ClassPtr)=xml.Get_string(); //(WORD)GPS.PreLoadGPImage((char*)xml.Get_string());
	return true;
}
const char* _picfile::GetClassName(){
	return "_picfile";
}
//
void _ModelID::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	char s[256];
	char s1[128];
	DWORD v=*((DWORD*)ClassPtr);
	if(v!=-1){
		strcpy(s1,IMM->GetModelFileName(v));
		xml.Assign_string(s1);
	}
}
bool _ModelID::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	char* ID=(char*)xml.Get_string();
	*((DWORD*)ClassPtr)=(DWORD)IMM->GetModelID(ID?ID:"");
	return true;
}
const char* _ModelID::GetClassName(){
	return "_ModelID";
}

void _TextureID::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	char s[256];
	char s1[128];
	DWORD v=*((DWORD*)ClassPtr);
	if(v!=-1){
		xml.Assign_string((char*)IRS->GetTextureName(v));
	}
}
bool _TextureID::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	extern IRenderSystem* IRS;
	*((DWORD*)ClassPtr)=(DWORD)IRS->GetTextureID((char*)xml.Get_string());
	return true;
}
const char* _TextureID::GetClassName(){
	return "_TextureID";
}

void _textid::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	char* s=*((char**)ClassPtr);
	if(s){
		char* GetIDByText(char*);
		//char* ID=s;
		char* ID=GetIDByText(s);
		xml.Assign_string(ID);
	}
}
bool _textid::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	char** lpS=(char**)ClassPtr;
	char* s;
	char* GetTextByID(char*);
	
	extern bool v_DialogsMissed;
	v_DialogsMissed=true;
	if(xml.Get_string()){
		//s=(char*)xml.Get_string();
		s=GetTextByID((char*)xml.Get_string());
	}else s="";
	v_DialogsMissed=false;
		
	if(*lpS)free(*lpS);
	*lpS=(char*)malloc(strlen(s)+1);
	strcpy(*lpS,s);
	return true;
}
const char* _textid::GetClassName(){
	return "_textid";
}
void _lpchar::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	char* s=*((char**)ClassPtr);
	if(s)xml.Assign_string(s);
}
bool _lpchar::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	char** lpS=(char**)ClassPtr;
	char* s;
	char* GetTextByID(char*);
	if(xml.Get_string()) s=(char*)xml.Get_string();
	else s="";		
	if(*lpS)free(*lpS);
	*lpS=(char*)malloc(strlen(s)+1);
	strcpy(*lpS,s);
	return true;
}
const char* _lpchar::GetClassName(){
	return "_lpchar";
}
void _font::Save(xmlQuote& xml,void* ClassPtr,void* Extra){
	DWORD* fp=(DWORD*)ClassPtr;
	Enumerator* EF=ENUM.Get("FONTS");
	char* s=EF->Get(*fp);
	if(!s)s=EF->GetStr(0);
	xml.Assign_string(s);
}
bool _font::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	Enumerator* EF=ENUM.Get("FONTS");
	if(xml.Get_string()){
		DWORD V=EF->Get((char*)xml.Get_string());
		if(V==0xFFFFFFFF)V=EF->GetVal(0);
		*((DWORD*)ClassPtr)=V;
	}else{
		*((DWORD*)ClassPtr)=EF->GetVal(0);
	}
	return true;
}
const char* _font::GetClassName(){
	return "_font";
}

// _ClassIndex ///////////////////////////////////////////////////////////
bool	g_SeparateStr(char* a_str,char* a_frs,char* a_sep,bool revers=false){
	a_frs[0]=0;
	if (a_str==NULL||strlen(a_str)==0||a_frs==NULL)	return false;
	if (a_sep==NULL||strlen(a_sep)==0)	{ sprintf(a_frs,"%s",a_str); a_str[0]=0;	return true; };
	char* ppp=strstr(a_str,a_sep);
	if (ppp==NULL)						{ sprintf(a_frs,"%s",a_str); a_str[0]=0;	return true; };
	int nc=(int)(ppp-a_str+1);
	strncpy(a_frs,a_str,nc);
	a_frs[nc-1]=0;
	if (a_str[nc]==0)	{	a_str[0]=0;				return true; };
	char ttt[512];
	sprintf(ttt,"%s",&(a_str[nc]));
	sprintf(a_str,"%s",ttt);
	if (revers) {
		sprintf(ttt,"%s",a_str);
		sprintf(a_str,"%s",a_frs);
		sprintf(a_frs,"%s",ttt);
	};
	return true;
};
BaseClass*	_ClassIndex::getARRAY(char* _ExtraPtr){
	if (_ExtraPtr==NULL) return NULL;
	BaseClass* OWNER = (BaseClass*)this;
	if (OWNER==NULL) return NULL;
	OWNER = (BaseClass*)(OWNER->GetParent());
	if (OWNER==NULL) return NULL;
	char	FILD_NAME[512];	// Path to fild that contain data list object
	char	ELEM_NAME[128];	// fild than show in list in editor 
	sscanf(_ExtraPtr,"%s%s",FILD_NAME,ELEM_NAME);
	if (strlen(FILD_NAME)==0||strlen(ELEM_NAME)==0)	return NULL;
	char	FRST_NAME[512];
	char	MODE_NAME[64];
	BaseClass*	ClassArrayPtr	= NULL;
	void*		ClassArrayData	= NULL;
	void*		ClassArrayExtra	= NULL;
	while (FILD_NAME[0]!=0&&OWNER!=NULL) {
		FRST_NAME[0]=0;
		MODE_NAME[0]=0;
		if (strstr(FILD_NAME,"/")!=NULL){
			g_SeparateStr(FILD_NAME,FRST_NAME,"/");	
		}else{
			sprintf(FRST_NAME,"%s",FILD_NAME);
			FILD_NAME[0]=0;
		};

		if (strstr(FRST_NAME,"^")!=NULL) {
			g_SeparateStr(FRST_NAME,MODE_NAME,"^",true);	
		};

		if (FRST_NAME[0]!=0) {	
			if (strcmp(FRST_NAME,"..")==0) {	// go to parent
				if (OWNER->CheckDirectCasting()){
					OWNER=(BaseClass*)OWNER->GetParent();
				};
			}else{								// have directly fild
				OWNER->GetElementByName(FRST_NAME,&ClassArrayPtr,&ClassArrayData,&ClassArrayExtra,OWNER);
				if(ClassArrayPtr&&ClassArrayPtr->CheckDirectCasting()) ClassArrayPtr=(BaseClass*)ClassArrayData;
				else ClassArrayPtr=NULL;
				OWNER=ClassArrayPtr;
			};
		};
		if (MODE_NAME[0]!=0){	// Have some modificator for OWNER
			if (strcmp(MODE_NAME,"REF")==0) {
				ClassRef<ReferableBaseClass>* pRefClass = (ClassRef<ReferableBaseClass>*)OWNER;
				if (OWNER!=NULL) {
					OWNER = (BaseClass*)(pRefClass->Get());
				};
			};
		};
	};
	return OWNER;
};
BaseClass*	_ClassIndex::getELEMENT(char* _ExtraPtr){
	BaseClass* pElement = NULL;
	BaseClass* pArray = getARRAY(_ExtraPtr);
	char	FILD_NAME[512];	// Path to fild that contain data list object
	char	ELEM_NAME[128];	// fild than show in list in editor 
	sscanf(_ExtraPtr,"%s%s",FILD_NAME,ELEM_NAME);
	int N=pArray->GetAmountOfElements();
	if (pArray!=NULL) {
		if (INDEX!=-1&&NUMBER==N) {
			BaseClass* ClassType=pArray->GetElementClass(INDEX);
			void* ElmData=pArray->GetElementPtr(INDEX,pArray);
			void* Extra=pArray->GetElementExtraPtr(INDEX);

			BaseClass* E_Class=NULL;
			void* E_Data;
			void* E_Extra;
			if(ClassType->GetElementByName(ELEM_NAME,&E_Class,&E_Data,&E_Extra,ClassType)){
				xmlQuote xml;
				E_Class->Save(xml,E_Data,E_Extra);
				const char* s=xml.Get_string();

				if (s!=NULL && !strcmp( s, this->pchar() )){
					pElement=(BaseClass*)(ElmData);
				};
			};
		};
		if (pElement==NULL) {
			for(int i=0;(i<N)&&(pElement==NULL);i++){
				BaseClass* ClassType=pArray->GetElementClass(i);
				void* ElmData=pArray->GetElementPtr(i,pArray);
				void* Extra=pArray->GetElementExtraPtr(i);

				BaseClass* E_Class=NULL;
				void* E_Data;
				void* E_Extra;
				if(ClassType->GetElementByName(ELEM_NAME,&E_Class,&E_Data,&E_Extra,ClassType)){
					xmlQuote xml;
					E_Class->Save(xml,E_Data,E_Extra);
					const char* s=xml.Get_string();

					if (s!=NULL && !strcmp( s, this->pchar() )){
						pElement=(BaseClass*)(ElmData);
						NUMBER=N;
						INDEX=i;
					};
				};
			};
		};
	};
	if (pElement==NULL) {
		NUMBER=-1;
		INDEX=-1;
	}
	return pElement;
};
//////////////////////////////////////////////////////////////////////////
#endif //NO_COSS_EXT
#endif //_USE3D
void BaseClass::LoadPostProcess(void* DataPtr,void* ExtraData){
	BaseClass* SF_ClassPtr=NULL;;
	void* SF_ElmPtr;
	void* SF_ExtraPtr;
	if(GetElementByName("SourceFile",&SF_ClassPtr,&SF_ElmPtr,&SF_ExtraPtr,DataPtr)){
		if(!strcmp(SF_ClassPtr->GetClassName(),"_str")){
			BaseClass* LR_ClassPtr=NULL;;
			void* LR_ElmPtr;
			void* LR_ExtraPtr;
			if(GetElementByName("LoadingRules",&LR_ClassPtr,&LR_ElmPtr,&LR_ExtraPtr,DataPtr)){
				if(*((byte*)LR_ElmPtr)){
					xmlQuote xml;
					if(xml.ReadFromFile(((_str*)SF_ElmPtr)->pchar())){
						reset_class(DataPtr);
						ErrorPager Error;
						Load(xml,DataPtr,&Error,ExtraData);
					}
				}
			}
		}
	}
	int N=GetAmountOfElements();
	for(int i=0;i<N;i++){
		LoadPostProcess(GetElementPtr(i,DataPtr),GetElementExtraPtr(i));
	}
}
DIALOGS_API BaseClass* _getClassFromPalette(const char* classname){
	DString D=(char*)classname;	
	ConvTypeName(D);	
	int idx=CGARB.NamesHash.find(D.pchar());
	if(idx!=-1){
		OneClassStorage*OCS=CGARB.Storage[idx];
		if(OCS)return OCS->OneMemb;
	}
	return NULL;
}
DIALOGS_API 
void reg_parent(BaseClass* bas,BaseClass*m){
	OneClassStorage* OCS_parent=m->GetClassStorage();	
	bool noadd=0;
	const char* st=bas->GetClassName();	
	for(int i=0;i<OCS_parent->Children.GetAmount();i++)if(!strcmp(OCS_parent->Children[i],st)){
        noadd=1;
	}
	if(!noadd){
		char* s=new char[strlen(st)+1];
		strcpy(s,bas->GetClassName());
		OCS_parent->Children.Add(s);
	}	
	OneClassStorage* OCS_my=bas->GetClassStorage();
	int Np=OCS_parent->Members.GetAmount();

	noadd=0;	
	st=m->GetClassName();
	for(int i=0;i<OCS_my->Parents.GetAmount();i++)if(!strcmp(OCS_my->Parents[i],st)){
		noadd=1;
	}
	if(!noadd){
		char* s=new char[strlen(m->GetClassName())+1];
		strcpy(s,m->GetClassName());	
		OCS_my->Parents.Add(s);
	}
	for(int i=0;i<Np;i++){
		OneClassMemberStorage* OCMS_parent=OCS_parent->Members[i];
		OneClassMemberStorage* OCMS_my=OCS_my->CreateMember(OCMS_parent->xmlID);
		OCMS_my->Member=OCMS_parent->Member->new_element();
		OCMS_my->OffsetFromClassRoot=OCMS_parent->OffsetFromClassRoot;
		OCMS_my->UseReference=OCMS_parent->UseReference;
		OCMS_my->ExtraData=OCMS_parent->ExtraData;
		OCMS_my->IntPropertyR=OCMS_parent->IntPropertyR;
		OCMS_my->GeneralPropertyR=OCMS_parent->GeneralPropertyR;
		OCMS_my->IntPropertyW=OCMS_parent->IntPropertyW;
		OCMS_my->GeneralPropertyW=OCMS_parent->GeneralPropertyW;
		OCMS_my->HostClass=OCMS_parent->HostClass;
		OCMS_my->Mask=OCMS_parent->Mask;
		OCMS_my->StaticMode=OCMS_parent->StaticMode;
		OCMS_my->ReadOnly=OCMS_parent->ReadOnly;
		OCMS_my->NoSaveMode=OCMS_parent->NoSaveMode;
		OCMS_my->Invisible=OCMS_parent->Invisible;
	}
}
DIALOGS_API 
void reg_base(BaseClass* bas,BaseClass* m){
	OneClassStorage* OCS_parent=m->GetClassStorage();	
	bool noadd=0;
	const char* st=bas->GetClassName();
	for(int i=0;i<OCS_parent->Children.GetAmount();i++)if(!strcmp(OCS_parent->Children[i],st)){
		noadd=1;
	}
	if(!noadd){
		char* s=new char[strlen(st)+1];
		strcpy(s,bas->GetClassName());
		OCS_parent->Children.Add(s);
	}	
	OneClassStorage* OCS_my=bas->GetClassStorage();
	int Np=OCS_parent->Members.GetAmount();

	noadd=0;
	st=m->GetClassName();
	for(int i=0;i<OCS_my->Parents.GetAmount();i++)if(!strcmp(OCS_my->Parents[i],st)){
		noadd=1;
	}
	if(!noadd){
		char* s=new char[strlen(st)+1];
		strcpy(s,m->GetClassName());	
		OCS_my->Parents.Add(s);
	}
}
#include "..\Cossacks2\FMM\FMManager.h"
extern FMManager MManager;
DIALOGS_API void CheckDynPtr(void* Ptr){
	//assert(Ptr >= MManager.HeapBlock.BaseAddress && Ptr <= MManager.HeapBlock.BaseAddress + MManager.HeapBlock.TotalSize);
}
DIALOGS_API void TestClassStorage(OneClassStorage** OCS){
	return;
	try{
		if( (*OCS) && (*OCS)->OneMemb ){
			(*OCS)->OneMemb->GetClassName();
			for(int i=0;i<(*OCS)->Members.GetAmount();i++){
				if((*OCS)->Members[i] && (*OCS)->Members[i]->Member){
					(*OCS)->Members[i]->Member->CheckDirectCasting();
				}
			}
		}
	}catch(...){
		*OCS=NULL;
	}
}

//MemoryBinStream

MemoryBinStream::MemoryBinStream(){
	Data=NULL;
	_Size=0;
	_MaxSize=0;
	_WritePos=0;
	_ReadPos=0;
}
MemoryBinStream::~MemoryBinStream(){
	Clear();
}
void  MemoryBinStream::CheckSize(int size){
	if(size>_MaxSize){
		if(size<=128){
			if(size<=8)size=8;
			else if(size<=16)size=16;
			else if(size<=32)size=32;
			else if(size<=64)size=64;
			else size=128;
		}else size=size+_MaxSize/2;
        Data=POOL.Realloc(Data,_MaxSize,size);
		_MaxSize=size;
	}
}
void  MemoryBinStream::Write(void* data,int size){
    CheckSize(_WritePos+size);
	memcpy(Data+_WritePos,data,size);
	_WritePos+=size;
	_Size=max(_Size,_WritePos);
}
int  MemoryBinStream::Read(void* data,int size){
	int rsize=_Size-_ReadPos;
	rsize=min(rsize,size);
	if(rsize>0)memcpy(data,Data+_ReadPos,size);
    rsize=max(rsize,0);
	_ReadPos+=rsize;
	return rsize;
}
void  MemoryBinStream::WriteToFile(const char* Name){
	ResFile F=RRewrite((char*)Name);
	if(F!=INVALID_HANDLE_VALUE){
		RBlockWrite(F,Data,_Size);
		RClose(F);
	}
}
void  MemoryBinStream::ReadFromFile(const char* Name){
	Clear();
	ResFile F=RReset((char*)Name);
	if(F!=INVALID_HANDLE_VALUE){
		int sz=RFileSize(F);
        CheckSize(sz);
		RBlockRead(F,Data,sz);
		RClose(F);
		_Size=sz;
	}
}
int   MemoryBinStream::Size(){
	return _Size;
}
void  MemoryBinStream::SetReadPos(int pos){
	_ReadPos=max(pos,0);
}
void  MemoryBinStream::SetWritePos(int pos){
	_WritePos=max(pos,0);
}
int   MemoryBinStream::GetReadPos(){
	return _ReadPos;
}
int   MemoryBinStream::GetWritePos(){
	return _WritePos;
}
BYTE* MemoryBinStream::GetData(){
	return Data;
}
void  MemoryBinStream::Clear(){
	if(Data)POOL.Free(Data,_MaxSize);
	//if(Data)free(Data);
	Data=NULL;
	_Size=0;
	_MaxSize=0;
	_WritePos=0;
	_ReadPos=0;
}
void  BinStream::WriteBYTE(BYTE c){
    Write(&c,1);
}
void  BinStream::WriteWORD(WORD w){
	Write(&w,2);
}
void  BinStream::WriteDWORD(DWORD D){
	Write(&D,4);
}
BYTE  BinStream::ReadBYTE(){
    BYTE b=0;
	Read(&b,1);
	return b;
}
WORD  BinStream::ReadWORD(){
	WORD w=0;
	Read(&w,2);
	return w;
}
DWORD BinStream::ReadDWORD(){
	DWORD D=0;
	Read(&D,4);
	return D;
}
//Load/Save binary data
void _str::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	//Out->WriteBYTE(123);
	_str& _string=*((_str*)Data);
    char* s=_string.pchar();	
	DWORD ID=ExDictionary?ExDictionary->FindStr(s):0xFFFFFFFF;	
	if(ID!=0xFFFFFFFF){
        Out->WriteBYTE(255);
		Out->WriteWORD(ID);
	}else{
		int L=strlen(s);
		int L2=L;
		if(L>253){
			Out->WriteBYTE(254);
			Out->WriteWORD(L);
		}else{
			Out->WriteBYTE(L);
		}
		Out->Write(s,L);
	}
	//Out->WriteBYTE(231);
}
void _str::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){	
	//byte qq=In->ReadBYTE();
	_str& _string=*((_str*)Data);
	int sz=In->ReadBYTE();
	if(sz==0xFF){
        WORD ID=In->ReadWORD();
		if(ID!=0xFFFF && ExDictionary){
            (*this)=ExDictionary->GetStr(ID);
		}else _string="";
	}else{
        if(sz==0xFE)sz=In->ReadWORD();
		if(sz>0){
			_string.Allocate(sz+1);
			In->Read(_string.str,sz);
			_string.str[sz]=0;
		}else _string="";
	}
	//byte pp=In->ReadBYTE();
	//int gg=qq;
	//gg=pp;
}
void _int::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,4);
}
void _int::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
    In->Read(Data,4);
}
void _DWORD::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,4);
}
void _DWORD::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,4);
}
void _float::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,4);
}
void _float::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,4);
}
void _bool::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,1);
}
void _bool::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,1);
}
void _index::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,4);
}
void _index::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,4);
}
void _byte_index::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,1);
}
void _byte_index::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,1);
}
void _word_index::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,2);
}
void _word_index::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,2);
}
void _accamulator::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,4);
}
void _accamulator::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,4);
}
void _color::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,4);
}
void _color::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,4);
}
void _WORD::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,2);
}
void _WORD::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,2);
}
void _short::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,2);
}
void _short::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,2);
}
void _char::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,1);
}
void _char::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,1);
}
void _BYTE::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,4);
}
void _BYTE::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	In->Read(Data,4);
}
void _UnitType::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	Out->Write(Data,2);
}
void _UnitType::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	int& v=*((int*)Data);
	v=0;
	In->Read(Data,2);
	if(v==0xFFFF)v=0xFFFFFFFF;
}
void BaseClass::SaveBin(void* Data,void* Extra,BinStream* Out,Enumerator* ExDictionary,Enumerator* SkipList){
	if(CheckDirectCasting()){
		BaseClass* BC=(BaseClass*)Data;
		OneClassStorage* OCS=BC->GetClassStorage();
		BC->RegisterMembers();	
		SetContext(BC);
		int N=OCS->Members.GetAmount();
		DWORD D=BC->GetClassMask();
		if(N){
			MemoryBinStream st0;
			MemoryBinStream st1;
			BaseClass* ORG=BC->new_element();
			ORG->reset_class(ORG);
            int nreal=0;
			for(int i=0;i<N;i++){
				OneClassMemberStorage* OCMS=OCS->Members[i];
				if(OCMS && OCMS->Member && OCMS->Mask&D){
					int ID=SkipList?SkipList->Get(OCMS->xmlID):-1;
					if(ID==-1){
						void* Data0=OCMS->GetElmPtr(ORG);
						void* Data1=OCMS->GetElmPtr(BC);
						BaseClass* B0=OCMS->Member;
						BaseClass* B1=OCMS->Member;
						if(B0->CheckDirectCasting()){
							B0=(BaseClass*)Data0;
							B1=(BaseClass*)Data1;
						}
						int pos0=st0.GetWritePos();
						int pos1=st1.GetWritePos();

						//st0.WriteBYTE(234);
						st0.WriteBYTE(i);
						B0->SaveBin(Data0,OCMS->ExtraData,&st0,ExDictionary,SkipList);
						//st0.WriteBYTE(235);

						//st1.WriteBYTE(234);
						st1.WriteBYTE(i);
						B1->SaveBin(Data1,OCMS->ExtraData,&st1,ExDictionary,SkipList);
						//st1.WriteBYTE(235);

						int sz0=st0.GetWritePos()-pos0;
						int sz1=st1.GetWritePos()-pos1;
						if(sz0==sz1 && !memcmp(st0.GetData()+pos0,st1.GetData()+pos1,sz0)){
							st1.SetWritePos(pos1);							
						}else nreal++;
					}
				}
			}
			delete(ORG);
			Out->WriteBYTE(nreal);
			Out->Write(st1.GetData(),st1.GetWritePos());
		}else Out->WriteBYTE(0);
	}	
}
void BaseClass::LoadBin(void* Data,void* Extra,BinStream* In,Enumerator* ExDictionary){
	if(CheckDirectCasting()){
		BaseClass* BC=(BaseClass*)Data;
		OneClassStorage* OCS=BC->GetClassStorage();
		BC->RegisterMembers();	
		SetContext(BC);
		reset_class(Data);
		int N=In->ReadBYTE();
		for(int i=0;i<N;i++){
			//int qq=In->ReadBYTE();
			int id=In->ReadBYTE();
			OneClassMemberStorage* OCMS=OCS->Members[id];
			void* Data=OCMS->GetElmPtr(BC);
			BaseClass* B0=OCMS->Member;
            if(B0->CheckDirectCasting())B0=(BaseClass*)Data;
			B0->LoadBin(Data,OCMS->ExtraData,In,ExDictionary);
			//int pp=In->ReadBYTE();
			int qq=1;
		}		
	}
}
void KillUnusableClassRegistrations(){
	bool ch=false;
	ClassArray<_str> Names;
	for(int i=0;i<CGARB.Storage.GetAmount();i++){
		OneClassStorage* OCS=CGARB.Storage[i];
		bool Good=true;
		try{
            if(OCS->OneMemb)OCS->OneMemb->GetClassName();
			for(int j=0;j<OCS->Members.GetAmount();j++){
				OneClassMemberStorage* OCMS=OCS->Members[j];
				if(OCMS && OCMS->Member){
					OCMS->Member->GetClassName();
				}
			}
		}catch(...){
            OCS->Members.Clear();
			delete(OCS);
			CGARB.Storage.Del(i,1);
			i--;
			ch=true;
			Good=false;
		}
		if(Good){
			_str* n=new _str;
            *n=CGARB.NamesHash.get(i);
			Names.Add(n);
		}
	}
	if(ch){
		CGARB.NamesHash.clear();
		for(int i=0;i<CGARB.Storage.GetAmount();i++){
			CGARB.NamesHash.add(Names[i]->cpchar());
		}
	}
}
#endif //IMPLEMENT_CLASS_FACTORY