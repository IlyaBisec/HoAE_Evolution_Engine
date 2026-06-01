#pragma once
class OneClassFolderItem:public BaseClass{
public:
	SAVE(OneClassFolderItem);
	ENDSAVE;
};
class ClassSolution;
class SimpClassList:public ClassArray<OneClassFolderItem>{
	ClassSolution* GetRootSolution();
public:
	virtual bool ForceSimplification(){
		return true;
	}
	bool GetEditableElementExpansionList(ClassExpParams& EXP,int Index
		,void* DataPtr,DWORD Option,BaseClass* Parent,const char* ElmName);
	int ExpandWith(const char* ElmName,void* base);
};
class OneClassFolder:public OneClassFolderItem{
public:
	_str Name;
    SimpClassList Items;
	SAVE(OneClassFolder);
		REG_LOADSAVE_FOR_CLASS(OneClassFolder,"UnitsAbility\\","*.OneClassFolder.xml");
		REG_PARENT(OneClassFolderItem);
		REG_AUTO(Name);
		REG_AUTO(Items);
	ENDSAVE;
	const char* GetThisElementView(const char* Local){
		char* b=GetGlobalBuffer();
		sprintf(b,"{P Interf3\\btnz 26 12 3 -1 15}%s",Name.pchar());
		return b;
	}
};
class SimpClassRef:public ClassRef<ReferableBaseClass>{
public:
	virtual bool ForceSimplification(){
		return true;
	}
    AUTONEW(SimpClassRef);	
};
class OneClassItem:public OneClassFolderItem{
public:
	SimpClassRef Item;
	SAVE(OneClassItem);
		REG_AUTO(Item);
	ENDSAVE;
	virtual bool ForceSimplification(){
		return true;
	}
};
class ClassSolution:public SimpClassList{
public:
	DynArray<BaseClass*>	SourcesList;
	ClassArray<_str>		ClassNameList;
	DynArray<int>			ClassNameSourceIndex;
	ClassArray<_str>		xmlAutoSave;

	//OneClassFolder Root;

	bool AddClonesArray(BaseClass* Array,char* xmlForAutosave);
	void Autosave();

	SAVE(ClassSolution);
		//REG_AUTO(Root);
		REG_CLASS(OneClassFolderItem);
		REG_CLASS(OneClassFolder);
		REG_CLASS(OneClassItem);
	ENDSAVE;
	bool WriteToFile     (const char* Name);
	bool SafeReadFromFile(const char* Name);
	bool ReadFromFile	 (const char* Name);

};
#ifdef IMPLEMENT_CLASS_FACTORY

ClassSolution* SimpClassList::GetRootSolution(){
	BaseClass* B=GetParent();
	while(B){
		ClassSolution* CS=dynamic_cast<ClassSolution*>(B);
		if(CS)return CS;
        B=B->GetParent();
	}
	return NULL;
}

bool ClassSolution::AddClonesArray(BaseClass* Array,char* xmlForAutosave){
	if(!Array->GetExpansionRules())return false;    	
	SourcesList.Add(Array);
	_str* s=new _str;
	*s=xmlForAutosave;
	xmlAutoSave.Add(s);
	ClassExpParams PP;
	int N=Array->GetAmountOfEditableElements(Array);
	if(N>0){
		Array->GetEditableElementExpansionList(PP,N-1,Array);
		for(int i=0;i<PP.ExpList.GetAmount();i++){
			_str* st=new _str;
			*st=PP.ExpList[i];
			ClassNameList.Add(st);
			ClassNameSourceIndex.Add(SourcesList.GetAmount()-1);
		}
	}
	return true;
}
char* GetAllocatedConst(char* cc){
	static Enumerator* E=ENUM.Get("CONSTGARBAGE");
	int id=0;
	id=E->FindStr(cc);
	if(id==-1){
		E->Add(cc);
		id=E->FindStr(cc);
	}
	return E->GetStr(id);
}
bool SimpClassList::GetEditableElementExpansionList(ClassExpParams& EXP,int Index,void* DataPtr,DWORD Option,BaseClass* Parent,const char* ElmName){
	if(Index>=GetAmount()){
		EXP.TopicsList.Add("New");	
		EXP.ExpList.Add("AddNewFolder");
		EXP.TopicsIdxs.Add(0);
		ClassSolution* CS=GetRootSolution();
		if(!CS)return false;
		//adding new elements
		for(int i=0;i<CS->ClassNameList.GetAmount();i++){
			char cc[512];
			sprintf(cc,"new:%s",CS->ClassNameList[i]->pchar());
			EXP.ExpList.Add(GetAllocatedConst(cc));
			EXP.TopicsIdxs.Add(0);
		}
		//adding references to existing elements
		
		for(int i=0;i<CS->SourcesList.GetAmount();i++){
			char cc[512];
			sprintf(cc,"Exising elements (%s)",CS->SourcesList[i]->GetExpansionBaseClass());
			EXP.TopicsList.Add(GetAllocatedConst(cc));
			ClonesArray<ReferableBaseClass>* CRB=(ClonesArray<ReferableBaseClass>*)CS->SourcesList[i];
			for(int j=0;j<CRB->GetAmount();j++)if((*CRB)[j]){
				sprintf(cc,"%s:%s",(*CRB)[j]->GetClassName(),(*CRB)[j]->Name.pchar());
				EXP.ExpList.Add(GetAllocatedConst(cc));
				EXP.TopicsIdxs.Add(EXP.TopicsList.GetAmount()-1);
			}
		}
	}else return false;
}
int SimpClassList::ExpandWith(const char* ElmName,void* base){
	if(!strcmp(ElmName,"AddNewFolder")){
        OneClassFolder* CF=new OneClassFolder;
		CF->Name="NewFolder";
		return Add(CF);
	}else if(strstr(ElmName,":")){
		ClassSolution* CS=GetRootSolution();
		if(!CS)return -1;
		//searching for new element
		if(!strncmp(ElmName,"new:",4)){
			//new element
            char* clname=(char*)ElmName+4;
			int idx;
			for(idx=0;idx<CS->ClassNameList.GetAmount();idx++){
				if(!strcmp(clname,CS->ClassNameList[idx]->pchar()))break;
			}
			if(idx>=CS->ClassNameList.GetAmount())return -1;
			OneClassStorage* OCS=CGARB.GetClass(clname);
			if(OCS && OCS->OneMemb){
				BaseClass* BC=OCS->OneMemb->new_element();
				if(BC){
					ReferableBaseClass* RBC=dynamic_cast<ReferableBaseClass*>(BC);
					if(RBC){
						BaseClass* CA=CS->SourcesList[CS->ClassNameSourceIndex[idx]];
						ClassArray<ReferableBaseClass>* RCA=(ClassArray<ReferableBaseClass>*)CA;						
						char cc[64];
						sprintf(cc,"%s%d",strstr(ElmName,":")+1,GetTickCount()%10000);
						RBC->Name=cc;
						OneClassItem* OCI=new OneClassItem;
						OCI->Item.Set(RBC);
						RCA->Add(RBC);						
						return Add(OCI);
					}else delete(BC);
				}
			}
		}else{
			//searching for references on existing elements
			for(int i=0;i<CS->ClassNameList.GetAmount();i++){
				int N=strstr(ElmName,":")-ElmName;
				if(!strncmp(CS->ClassNameList[i]->pchar(),ElmName,N)){
					OneClassItem* OCI=new OneClassItem;
					xmlQuote xml;
					xmlQuote* x1=new_xmlQuota();
					x1->SetQuoteName("RefName");
					x1->Assign_string((char*)(strstr(ElmName,":")+1));
					xml.AddSubQuote(x1);
					x1=new_xmlQuota();
					x1->SetQuoteName("ClassName");
					x1->Assign_string(CS->ClassNameList[i]->pchar());
					xml.AddSubQuote(x1);					
					ErrorPager ep(0);
					OCI->Item.Load(xml,&OCI->Item,&ep,NULL);
					return Add(OCI);
				}
			}
		}
		//searching for existing element
                        
	}
	return ClassArray<OneClassFolderItem>::ExpandWith(ElmName,base);
}
void ClassSolution::Autosave(){
	for(int i=0;i<xmlAutoSave.GetAmount();i++){
		char* s=xmlAutoSave[i]->pchar();
		if(s[0]){
            SourcesList[i]->WriteToFile(s);
		}
	}
}
bool _util_LoadClassLikeFolder(BaseClass* BC,const char* xmlName,const char* FieldName);
void _util_SaveClassLikeFolder(BaseClass* BC,const char* xmlName,const char* FieldName);

bool ClassSolution::WriteToFile(const char* Name){
    Autosave();
	_util_SaveClassLikeFolder(this,Name,"Name");
	return true;
}
bool ClassSolution::SafeReadFromFile(const char* Name){
	_util_LoadClassLikeFolder(this,Name,"Name");
	return true;
}
bool ClassSolution::ReadFromFile	 (const char* Name){
	_util_LoadClassLikeFolder(this,Name,"Name");
	return true;
}
#endif //IMPLEMENT_CLASS_FACTORY