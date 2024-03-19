#pragma once
#include "GameExtension.h"
//
extern Nation NATIONS[8];
class UpgradesEnumerator: public ProcEnumerator{
public:
	virtual DWORD GetValue(const char* ID){
		for(int i=0;i<NATIONS->NUpgrades;i++)if(!strcmp(ID,NATIONS->UPGRADE[i]->Name))return i;
		return 0xFFFFFFFF;
	}	
	virtual char* GetValue(DWORD ID){
		if(ID<NATIONS->NUpgrades){
			char* s=NATIONS->UPGRADE[ID]->Name;
			return s?s:"???";
		}
		return "???";
	}
	virtual int   GetAmount(){
		return NATIONS->NUpgrades;
	}
	virtual char* GetIndexedString(int idx){
		return GetValue(idx);
	}
	virtual DWORD GetIndexedValue (int idx){
		return idx;
	}	
};
class UnitTypesEnumerator: public ProcEnumerator{
public:
	virtual DWORD GetValue(const char* ID){
		for(int i=0;i<NATIONS->NMon;i++)if(!strcmp(ID,NATIONS->Mon[i]->MonsterID))return i;
		return 0xFFFFFFFF;
	}	
	virtual char* GetValue(DWORD ID){
		if(ID<NATIONS->NMon){
			char* s=NATIONS->Mon[ID]->MonsterID;
			return s?s:"???";
		}
		return "???";
	}
	virtual int   GetAmount(){
		return NATIONS->NMon;
	}
	virtual char* GetIndexedString(int idx){
		return GetValue(idx);
	}
	virtual DWORD GetIndexedValue (int idx){
		return idx;
	}
	virtual char* GetCategory(int idx){
		extern char** NatNames;		
		extern int NNations;
		return NatNames[NATIONS->Mon[idx]->NatID];
	}
};
class FormationsEnumerator: public ProcEnumerator{
public:
	virtual DWORD GetValue(const char* ID){
		for(int i=0;i<NEOrders;i++)if(!strcmp(ID,ElementaryOrders[i].ID))return i;
		return 0xFFFFFFFF;
	}	
	virtual char* GetValue(DWORD ID){
		if(ID<NEOrders){
			char* s=ElementaryOrders[ID].ID;
			return s?s:"???";
		}
		return "???";
	}
	virtual int   GetAmount(){
		return NEOrders;
	}
	virtual char* GetIndexedString(int idx){
		return GetValue(idx);
	}
	virtual DWORD GetIndexedValue (int idx){
		return idx;
	}	
};
extern int CurAI_Color;
class AI_Condition:public BaseClass{
public:	
	ClassArray<AI_Condition> Condition;
	virtual bool Check(){
		int NC=Condition.GetAmount();
		bool COND=0;
		for(int i=0;i<NC;i++)COND|=Condition[i]->Check();
		return COND||NC==0;
	}
	virtual void GetBriefing(_str& str){		
		bool Complex=0;
		_str str1;
		str1="";
		for(int i=0;i<Condition.GetAmount();i++){
			if(i)Complex=true;
            _str s1;
			Condition[i]->GetBriefing(s1);
			if(strstr(s1.pchar()," and ")){
				str1+="(";
				str1+=s1.pchar();
				str1+=")";
			}else{			
				str1+=s1.pchar();
			}
			if(i!=Condition.GetAmount()-1){
				str1+=" or ";
			}
			if(strstr(str1.pchar()," and "))Complex=true;
		}	
		_str str2;
		if(Complex){			
			str2=str1.pchar();
			str1="(";
			str1+=str2.pchar();
			str1+=")";
		}
		if(str.pchar()&&str.pchar()[0]){
			if(str1.pchar()&&str1.pchar()[0]){
				str2.Clear();
                str2=str.pchar();
				str2+=" and ";
				str2+=str1.pchar();
				str=str2.pchar();
			}
		}else{
			str=str1.pchar();
		}
	}
	SAVE(AI_Condition)    
	ENDSAVE;
};
class AI_ConditionsArray:public AI_Condition{
public:
	_str Description;
	SAVE(AI_ConditionsArray);	
	REG_AUTO(Description);
	REG_AUTO(Condition);
	ENDSAVE;
};
class AI_Action:public BaseClass{
public:
	_str Description;    
	//int NTimes;
	virtual bool Perform(){return true;};
	virtual void GetBriefing(_str& str){};
	virtual void GetShortBriefing(_str& str){};
	SAVE(AI_Action)
	REG_AUTO(Description);
	//REG_MEMBER(_int,NTimes);
	ENDSAVE;
};
class AI_Trigger:public BaseClass{
public:
	_str Description;
	//int NTimes;
	AI_ConditionsArray Condition;
	ClassArray<AI_Action> Action;
	
	SAVE(AI_Trigger){
		REG_AUTO(Description);
		//REG_MEMBER(_int,NTimes);
		REG_AUTO(Condition);
		REG_AUTO(Action);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="";
		if(!ClassEditor::CheckIfOpen(this)){
			dest="Condition: %$Condition$%\nActions: %$Action,  \n$%";
		}else{
			dest="Trigger";
		}
		return true;
	}

};
class RulesArray:public ClassArray<AI_Trigger>{
public:
	int GetExpansionRules(){
        return 1;
	}	
};
class AI_Folder:public BaseClass{
public:
	_str Description;
	AI_ConditionsArray FolderCondition;
	RulesArray Rules;
	SAVE(AI_Folder){
		REG_AUTO(Description);
		REG_AUTO(FolderCondition);
		REG_AUTO(Rules);
	}ENDSAVE;
	virtual const char*	GetThisElementView(const char* LocalName);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="";
		if(!ClassEditor::CheckIfOpen(this)){
			dest="Condition: %$FolderCondition$%\nActions: %$Rules,  \n$%";
		}else{
			return false;
		}
		return true;
	}
};
class FoldersArray:public ClassArray<AI_Folder>{
public:
	int GetExpansionRules(){
		return 1;
	}	
};
class _UnitTypeShortcut:public _UnitType{
public:
	virtual const char* GetClassName(){
		return "_UnitTypeShortcut";
	}  
	AUTONEW(_UnitTypeShortcut);
};
class UnitDefinition:public BaseClass{
public:
	_str ShortName;
	int  UnitID;
	SAVE(UnitDefinition);
	REG_AUTO(ShortName);
	REG_MEMBER(_UnitType,UnitID);
	ENDSAVE;
};
class BuildingDefinition:public UnitDefinition{
public:	
	SAVE(BuildingDefinition);
	REG_AUTO(ShortName);
	REG_MEMBER(_UnitType,UnitID);
	ENDSAVE;
};
class UnitsArray:public ClassArray<UnitDefinition>{
public:
	int GetExpansionRules(){
		return 1;
	}
};
class BuildingsArray:public ClassArray<BuildingDefinition>{
public:
	int GetExpansionRules(){
		return 1;
	}
};
class AI_Script:public BaseClass{
public:
	_str Description;	
	UnitsArray Units;
	UnitsArray Buildings;
	FoldersArray Folders;	
	void ProcessAI_Script(byte Nation);
	SAVE(AI_Script)
		REG_LOADSAVE_FOR_CLASS(AI_Script,"ai\\","*.ai.xml");
		REG_CLASS(UnitDefinition);
		REG_CLASS(BuildingDefinition);
		REG_AUTO(Description);	
		REG_AUTO(Units);
		REG_AUTO(Buildings);	
		REG_AUTO(Folders);
	ENDSAVE;
};
struct AI_ed_params{
public:
	ClassEditor* CE;
	int* UnitID;
	char* Name;
};
bool AI_ed_proc(SimpleDialog* SD);
class _UnitType_editor:public OneElementEditor{
public:
	char Name[512];
	virtual const char* GetClassID(){return "_UnitType";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam)
	{
		int id=*(int*)DataPtr;
		strcpy(Name,NATIONS->Mon[id]->MonsterID);
		InputBox* IB=Base->addInputBox(NULL,x,y,Name,200,x1-x,y1-y,&CED_Font,&CED_Font);
		IB->OnClick=NULL;
		IB->OnUserClick=&AI_ed_proc;
		AI_ed_params* LP=new AI_ed_params;
		LP->CE=CParam->CE;
		LP->UnitID=(int*)DataPtr;
		LP->Name=Name;
        IB->AllocPtr=(char*)LP;
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		//strcpy(str,xml->Get_string());
		return true;
	}
	virtual int Get(xmlQuote* xml){
		//int i=atoi(str);
		//xml->Assign_int(i);
		return 0;
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _UnitType_editor;
	}
};
class _UnitTypeShortcut_editor:public OneElementEditor{
public:
	ComboBox* CB;
	virtual const char* GetClassID(){return "_UnitTypeShortcut";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam)
	{

		Enumerator* E=ENUM.Get("UNITS_SHORTCUTS");
		static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo");
		VScrollBar* VS;
		CB=Base->addGP_ComboBoxDLX(NULL,x,y,x1-x+1,CBB_GPFILE,0,9,0,&CED_AFont,&CED_Font,NULL);
		CB->AssignScroll(Base,&VS,3,0,8);
		for(int i=0;i<E->GetAmount();i++){
			CB->AddLine(E->GetStr(i));
		}
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
        char* s=(char*)xml->Get_string();
		Enumerator* E=ENUM.Get("UNITS_SHORTCUTS");
		if(s){
			for(int i=0;i<NATIONS->NMon;i++)if(!strcmp(s,NATIONS->Mon[i]->MonsterID)){
                int p=E->FindVal(i);
				if(p!=-1){
					CB->CurLine=p;
				}
			}
		}
		return true;
	}
	virtual int Get(xmlQuote* xml){
		if(CB->NLines){
			Enumerator* E=ENUM.Get("UNITS_SHORTCUTS");
			char* s=CB->Lines[CB->CurLine];
			int p=E->FindStr(s);
			if(p!=-1){
                xml->Assign_string(NATIONS->Mon[E->GetVal(p)]->MonsterID);
			}			
		}
		return true;
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _UnitTypeShortcut_editor;
	}
};
class AI_Condition_editor:public OneElementEditor{
public:
	char Name[512];
	AI_Condition* ai;
	virtual const char* GetClassID(){return "AI_Condition";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam)
	{
		ai=(AI_Condition*)DataPtr;
		_str brief;
		brief="";
		ai->GetBriefing(brief);
		strcpy(Name,brief.pchar());
		InputBox* IB=Base->addInputBox(NULL,x,y,Name,200,x1-x,y1-y,&CED_Font,&CED_Font);
		IB->Enabled=0;
		CurAI_Color=1;
		int idx=GPS.PreLoadGPImage("Interf3\\btnz");
		bool result=ai->Check();		
		if(result){
			Base->addGPPicture(NULL,0,y,idx,11);
		}else{
			Base->addGPPicture(NULL,0,y,idx,10);
		}
		return y1;
	}
	//virtual bool Assign(xmlQuote* xml){
	//	return true;
	//}
	virtual bool Process(){
		_str brief;
		brief="";
		if(ai){
			ai->GetBriefing(brief);
			strcpy(Name,brief.pchar());
		}
		return true;
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new AI_Condition_editor;
	}
};
_inline char* GetUnitShortcutByID(char* ID){
	Enumerator* E=ENUM.Get("UNITS_SHORTCUTS");	
	for(int i=0;i<NATIONS->NMon;i++)if(!strcmp(ID,NATIONS->Mon[i]->MonsterID)){
		int p=E->FindVal(i);
        if(p!=-1)return E->GetStr(p);
	}
	return "???";
}
_inline char* GetUnitIDByShortcut(char* Shortcut){
	Enumerator* E=ENUM.Get("UNITS_SHORTCUTS");
	int p=E->FindStr(Shortcut);
	if(p!=-1){
		return NATIONS->Mon[E->GetVal(p)]->MonsterID;
	}			
	return "???";
}
class UnitDefinition_editor:public OneElementEditor{
public:
	char Name[512];
	UnitDefinition* U;
	virtual const char* GetClassID(){return "UnitDefinition";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam)
	{
		U=(UnitDefinition*)DataPtr;
		Name[0]=0;
		InputBox* IB=Base->addInputBox(NULL,x,y,Name,200,x1-x,y1-y,&CED_Font,&CED_Font);
		IB->Enabled=0;
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		return true;
	}
	virtual int Get(xmlQuote* xml){
		sprintf(Name,"%s=%s",U->ShortName.pchar(),NATIONS->Mon[U->UnitID]->MonsterID);
		return true;
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new UnitDefinition_editor;
	}
};
class AI_Action_editor:public OneElementEditor{
public:
	char Name[512];
	AI_Action* ai;
	virtual const char* GetClassID(){return "AI_Action";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam)
	{
		ai=(AI_Action*)DataPtr;
		_str brief;
		brief="";
		ai->GetBriefing(brief);
		Name[0]=0;
		if(ai->Description.pchar())strcpy(Name,ai->Description.pchar());
		if(Name[0])strcat(Name,", ");
		strcat(Name,brief.pchar());
		InputBox* IB=Base->addInputBox(NULL,x,y,Name,200,x1-x,y1-y,&CED_Font,&CED_Font);
		IB->Enabled=0;
		return y1;
	}
	//virtual bool Assign(xmlQuote* xml){
	//	return true;
	//}
	virtual bool Process(){
		_str brief;
		brief="";
		ai->GetBriefing(brief);
		Name[0]=0;
		if(ai->Description.pchar())strcpy(Name,ai->Description.pchar());
		if(Name[0])strcat(Name,", ");
		strcat(Name,brief.pchar());
		return true;
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new AI_Action_editor;
	}
};
class AI_ConditionsArray_editor:public AI_Condition_editor{
public:	
	AI_ConditionsArray* ai;
	virtual const char* GetClassID(){return "AI_ConditionsArray";}	
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam)
	{
		ai=(AI_ConditionsArray*)DataPtr;
		_str brief;
		brief="";
		ai->GetBriefing(brief);
		Name[0]=0;
		if(ai->Description.pchar())strcpy(Name,ai->Description.pchar());
		if(Name[0])strcat(Name,", ");
		strcat(Name,brief.pchar());
		InputBox* IB=Base->addInputBox(NULL,x,y,Name,200,x1-x,y1-y,&CED_Font,&CED_Font);
		IB->Enabled=0;
		int idx=GPS.PreLoadGPImage("Interf3\\btnz");
		bool result=ai->Check();		
		if(result){
			Base->addGPPicture(NULL,0,y,idx,11);
		}else{
			Base->addGPPicture(NULL,0,y,idx,10);
		}
		return y1;
	}
	virtual int Get(xmlQuote* xml){
		_str brief;
		brief="";
		ai->GetBriefing(brief);
		Name[0]=0;
		if(ai->Description.pchar())strcpy(Name,ai->Description.pchar());
		if(Name[0])strcat(Name,", ");
		strcat(Name,brief.pchar());
		return true;
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new AI_ConditionsArray_editor;
	}
};
class AI_Trigger_editor:public OneElementEditor{
public:
	char Name[512];
	AI_Trigger* ai;
	virtual const char* GetClassID(){return "AI_Trigger";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam)
	{
		ai=(AI_Trigger*)DataPtr;
		_str brief;
		brief=ai->Description.pchar()?ai->Description.pchar():"";
		for(int i=0;i<ai->Action.GetAmount();i++){
            if(brief.pchar()&&brief.pchar()[0])brief+=", ";
			_str s1;
			ai->Action[i]->GetShortBriefing(s1);
			brief+=s1.pchar();
		}
		if(brief.pchar()){
			strncpy(Name,brief.pchar(),200);
			Name[200]=0;
		}
		InputBox* IB=Base->addInputBox(NULL,x,y,Name,200,x1-x,y1-y,&CED_Font,&CED_Font);
		IB->Enabled=0;
		return y1;
	}	
	virtual bool Process(){
		_str brief;
		brief=ai->Description.pchar()?ai->Description.pchar():"";
		for(int i=0;i<ai->Action.GetAmount();i++){
			if(brief.pchar()&&brief.pchar()[0])brief+=", ";
			_str s1;
			ai->Action[i]->GetShortBriefing(s1);
			brief+=s1.pchar();
		}
		if(brief.pchar()){
			strncpy(Name,brief.pchar(),200);
			Name[200]=0;
		}
		return true;
	}	
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new AI_Trigger_editor;
	}
};
_inline const char* getops(int i){
	const char* ops[]={">","<",">=","<=","==","!="};
	return ops[i];
}
class UnitDesc:public BaseClass{
public:
	_str UnitID;
	DialogsSystem DS;
	SAVE(UnitDesc)
	REG_AUTO(UnitID);
	REG_AUTO(DS);
	ENDSAVE;
};
//implementation of conditions and actions
_inline bool PerformAI_Op(int v1,int v2,int op){
	switch(op){
		case 0://>
			return v1>v2;
		case 1://<
			return v1<v2;
		case 2:
			return v1>=v2;
		case 3:
			return v1<=v2;
		case 4:
			return v1==v2;
		case 5:
			return v1!=v2;
		default:
			return false;
	}
}
extern Nation NATIONS[8];
class UpgradeIsRun:public AI_Condition{
public:
	bool Not;
	int UpgradeID;
	virtual bool Check(){
		//assert(UnitType>=0&&UnitType<NATIONS->NMon);
		Nation* CNAT=NATIONS+CurAI_Color;
		bool res=CNAT->UPGRADE[UpgradeID]->Done||CNAT->UPGRADE[UpgradeID]->IsDoing;
		if(Not)
			res=!res;
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		Nation* CNAT=NATIONS+CurAI_Color;
		if(Not){
			str.print("Upgrade (%s) is Not Run",CNAT->UPGRADE[UpgradeID]->Message);
		}else{
			str.print("Upgrade (%s) is Run",CNAT->UPGRADE[UpgradeID]->Message);
		}
		AI_Condition::GetBriefing(str);
	}
	SAVE(UpgradeIsRun){
		REG_PARENT(AI_Condition);
		REG_MEMBER(_bool,Not);
		REG_ENUM(_index,UpgradeID,ALL_UPGRADES);
		REG_AUTO(Condition);
	}ENDSAVE;
};

class ai_cResourcePeasants: public AI_Condition {
public:

	int ResourceID;
	int OpType;
	int Value;

	SAVE(ai_cResourcePeasants){
		REG_PARENT(AI_Condition)
		REG_ENUM(_index,ResourceID,RESTYPE);
		REG_ENUM(_index,OpType,OPTYPE)
		REG_MEMBER(_int,Value);	
		REG_AUTO(Condition);
	}ENDSAVE;

	virtual void GetBriefing(_str& str);
	virtual bool Check();
};

class AmountOfReadyUnits:public AI_Condition{
public:
	int UnitType;
	int OpType;
	int Value;
	virtual bool Check(){
		assert(UnitType>=0&&UnitType<NATIONS->NMon);
		extern City CITY[8];
        int NU=CITY[CurAI_Color].ReadyAmount[UnitType];
		bool res=PerformAI_Op(NU,Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		str="NReady(";
		str+=GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID);
		str+=")";
		str+=getops(OpType);		
		str+=Value;		
		AI_Condition::GetBriefing(str);
	}
	SAVE(AmountOfReadyUnits)
	REG_PARENT(AI_Condition)
	REG_MEMBER(_UnitTypeShortcut,UnitType);
	REG_ENUM  (_index,OpType,OPTYPE)
	REG_MEMBER(_int,Value);	
	REG_AUTO(Condition);
	ENDSAVE;
};
class UnitsProduced:public AI_Condition{
public:
	int UnitType;
	int OpType;
	int Value;
	virtual bool Check(){
		assert(UnitType>=0&&UnitType<NATIONS->NMon);
		extern City CITY[8];
		int NU=NATIONS[CurAI_Color].NProduced[UnitType];
		bool res=PerformAI_Op(NU,Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		str="NProduced(";
		str+=GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID);
		str+=")";
		str+=getops(OpType);		
		str+=Value;		
		AI_Condition::GetBriefing(str);
	}
	SAVE(UnitsProduced){
		REG_PARENT(AI_Condition);
		REG_MEMBER(_UnitTypeShortcut,UnitType);
		REG_ENUM  (_index,OpType,OPTYPE)
		REG_MEMBER(_int,Value);	
		REG_AUTO(Condition);
	}ENDSAVE;
};
class UnitsLost:public AI_Condition{
public:
	int UnitType;
	int OpType;
	int Value;
	virtual bool Check(){
		assert(UnitType>=0&&UnitType<NATIONS->NMon);
		extern City CITY[8];
		int NU=NATIONS[CurAI_Color].NKilled[UnitType];
		bool res=PerformAI_Op(NU,Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		str="NLost(";
		str+=GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID);
		str+=")";
		str+=getops(OpType);		
		str+=Value;		
		AI_Condition::GetBriefing(str);
	}
	SAVE(UnitsLost)
	REG_PARENT(AI_Condition)
	REG_MEMBER(_UnitTypeShortcut,UnitType);
	REG_ENUM  (_index,OpType,OPTYPE)
	REG_MEMBER(_int,Value);	
	REG_AUTO(Condition);
	ENDSAVE;
};
class CurResources:public AI_Condition{
public:
	int ResType;
	int OpType;
	int Value;
	CurResources(){
		ResType=0;
		OpType=0;
		Value=0;
	}
	virtual bool Check(){		
		bool res=PerformAI_Op(XRESRC(CurAI_Color,ResType),Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		Enumerator* E=ENUM.Get("RESTYPE");
		str.print("%s%s%d",E->GetStr(ResType),getops(OpType),Value);
		AI_Condition::GetBriefing(str);
	}
	SAVE(CurResources)
	REG_PARENT(AI_Condition)
	REG_ENUM  (_index,ResType,RESTYPE);
	REG_ENUM  (_index,OpType,OPTYPE)
	REG_MEMBER(_int,Value);	
	REG_AUTO(Condition);
	ENDSAVE;
};
CIMPORT int vdf_GetAmountOfSettlements(byte Owner,	int ResType, bool CheckUpgrades, int Level);
class AmountOfSettlements:public AI_Condition{
public:
	int  ResType;
	bool CheckUpgrades;
	int  Level;
	int  OpType;
	int  Value;
	AmountOfSettlements(){
		ResType=0;
		OpType=0;
		Value=0;
		Level=0;
		CheckUpgrades=0;
	}
	virtual bool Check(){		
		int NSETTL=vdf_GetAmountOfSettlements(CurAI_Color,ResType,CheckUpgrades,Level);;////for vital
		bool res=PerformAI_Op(NSETTL,Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		Enumerator* E=ENUM.Get("RESTYPE");
		if(CheckUpgrades)str.print("NSettl(%s)%s%d",E->GetStr(ResType),getops(OpType),Value);
		else str.print("NSettl(%s,Level%d)%s%d",E->GetStr(ResType),Level+1,getops(OpType),Value);
		AI_Condition::GetBriefing(str);
	}
	SAVE(AmountOfSettlements)
	REG_PARENT(AI_Condition)
	REG_ENUM  (_index,ResType,RESTYPE);
	REG_ENUM(_index,Level,LEVELS);
	REG_MEMBER(_bool,CheckUpgrades);
	REG_ENUM  (_index,OpType,OPTYPE)
	REG_MEMBER(_int,Value);	
	REG_AUTO(Condition);
	ENDSAVE;
};
CIMPORT int vdf_GetAmountOfEnemySettlements(byte Owner,	int ResType, bool CheckUpgrades, int Level);
class AmountOfEnemySettlements:public AI_Condition{
public:
	int  ResType;
	bool CheckUpgrades;
	int  Level;
	int  OpType;
	int  Value;
	AmountOfEnemySettlements(){
		ResType=0;
		OpType=0;
		Value=0;
		Level=0;
		CheckUpgrades=0;
	}
	virtual bool Check(){		
		int NSETTL=vdf_GetAmountOfEnemySettlements(CurAI_Color,ResType,CheckUpgrades,Level); ////for vital
		bool res=PerformAI_Op(NSETTL,Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		Enumerator* E=ENUM.Get("RESTYPE");
		if(CheckUpgrades)str.print("NEnemySettl(%s)%s%d",E->GetStr(ResType),getops(OpType),Value);
		else str.print("NEnemySettl(%s,Level%d)%s%d",E->GetStr(ResType),Level+1,getops(OpType),Value);
		AI_Condition::GetBriefing(str);
	}
	SAVE(AmountOfEnemySettlements)
	REG_PARENT(AI_Condition)
	REG_ENUM  (_index,ResType,RESTYPE);
	REG_ENUM(_index,Level,LEVELS);
	REG_MEMBER(_bool,CheckUpgrades);
	REG_ENUM  (_index,OpType,OPTYPE)
	REG_MEMBER(_int,Value);	
	REG_AUTO(Condition);
	ENDSAVE;
};
class TotalResource:public AI_Condition{
public:
	int ResType;
	int OpType;
	int Value;
	CurResources(){
		ResType=0;
		OpType=0;
		Value=0;
	}
	virtual bool Check(){		
		bool res=PerformAI_Op(NATIONS[CurAI_Color].ResTotal[ResType],Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		Enumerator* E=ENUM.Get("RESTYPE");
		str.print("Total(%s)%s%d",E->GetStr(ResType),getops(OpType),Value);
		AI_Condition::GetBriefing(str);
	}
	SAVE(TotalResource)
	REG_PARENT(AI_Condition)
	REG_ENUM  (_index,ResType,RESTYPE);
	REG_ENUM  (_index,OpType,OPTYPE)
	REG_MEMBER(_int,Value);	
	REG_AUTO(Condition);
	ENDSAVE;
};
class FreeLivePlaces:public AI_Condition{
public:	
	int OpType;
	int Value;
	virtual bool Check(){				
		bool res=PerformAI_Op(NATIONS[CurAI_Color].NFarms-NATIONS[CurAI_Color].NGidot,Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		str="FreePlaces";
		str+=getops(OpType);		
		str+=Value;		
		AI_Condition::GetBriefing(str);
	}
	SAVE(FreeLivePlaces)
	REG_PARENT(AI_Condition)	
	REG_ENUM  (_index,OpType,OPTYPE)
	REG_MEMBER(_int,Value);	
	REG_AUTO(Condition);
	ENDSAVE;
};
class TotalLivePlaces:public AI_Condition{
public:	
	int OpType;
	int Value;
	virtual bool Check(){				
		bool res=PerformAI_Op(NATIONS[CurAI_Color].NFarms,Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		str="TotalPlaces";
		str+=getops(OpType);		
		str+=Value;		
		AI_Condition::GetBriefing(str);
	}
	SAVE(TotalLivePlaces)
	REG_PARENT(AI_Condition)	
	REG_ENUM  (_index,OpType,OPTYPE)
	REG_MEMBER(_int,Value);	
	REG_AUTO(Condition);
	ENDSAVE;
};
class LivePlacesUsed:public AI_Condition{
public:	
	int OpType;
	int Value;
	virtual bool Check(){				
		bool res=PerformAI_Op(NATIONS[CurAI_Color].NGidot,Value,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		str="UsedPlaces";
		str+=getops(OpType);		
		str+=Value;		
		AI_Condition::GetBriefing(str);
	}
	SAVE(LivePlacesUsed);
	REG_PARENT(AI_Condition);
	REG_ENUM  (_index,OpType,OPTYPE);
	REG_MEMBER(_int,Value);	
	REG_AUTO(Condition);
	ENDSAVE;
};
class Difficulty:public AI_Condition{
public:	
	int OpType;
	int DifficultyValue;
	virtual bool Check(){		
		extern City CITY[8];
		bool res=PerformAI_Op(CITY[CurAI_Color].Difficulty,DifficultyValue,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		str="Difficulty";
		str+=getops(OpType);
		Enumerator* E=ENUM.Get("DIFFICULTY");
		if(E){
			str+=E->GetStr(DifficultyValue);
		}else str+=DifficultyValue;		
		AI_Condition::GetBriefing(str);
	}
	SAVE(Difficulty)
		REG_PARENT(AI_Condition)	
		REG_ENUM  (_index,OpType,OPTYPE)
		REG_ENUM  (_index,DifficultyValue,DIFFICULTY);	
		REG_AUTO  (Condition);
	ENDSAVE;
};
extern int AnimTime;
class OnceInPeriod:public AI_Condition{
public:	
	OnceInPeriod(){
		StartTime=0;
		Period=10;
	}
	int StartTime;
	int Period;
	virtual bool Check(){		
		int dt=Period*24*256;
		if(AnimTime-StartTime>dt){
			if(AnimTime-StartTime>dt*2){
				StartTime=AnimTime;
			}else{
				StartTime+=dt;
			}
            return true;
		}
        return false;
	}
	virtual void GetBriefing(_str& str){
		str="TrueOnceIn ";
		str+=Period;
		str+=" s";
		AI_Condition::GetBriefing(str);
	}
	SAVE(OnceInPeriod)
		REG_PARENT(AI_Condition)	
		REG_MEMBER(_int,Period)		
		REG_AUTO  (Condition);
	ENDSAVE;
};
class EnemyNearBase:public AI_Condition{
public:	
	EnemyNearBase(){
		Radius=3000;
		MinAmount=10;
		MaxAmount=10000;
		Towers=true;
		Buildings=true;
		Peasants=true;
		Infantry=true;
		Cavalery=true;
		Artillery=true;
	}
	int Radius;
	int MinAmount;
	int MaxAmount;
	bool Towers;
	bool Buildings;
	bool Peasants;
	bool Infantry;
	bool Cavalery;
	bool Artillery;
	virtual bool Check();
	virtual void GetBriefing(_str& str){
		str=MinAmount;
		str+=" enemies are near base (R<";
		str+=Radius;
		str+="pix)";
		AI_Condition::GetBriefing(str);
	}
	SAVE(EnemyNearBase)
		REG_PARENT(AI_Condition)	
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,MinAmount);
		REG_MEMBER(_int,MaxAmount);
		REG_MEMBER(_bool,Towers);
		REG_MEMBER(_bool,Buildings);
		REG_MEMBER(_bool,Peasants);
		REG_MEMBER(_bool,Infantry);
		REG_MEMBER(_bool,Cavalery);
		REG_MEMBER(_bool,Artillery);
	ENDSAVE;
};
class GamingTime:public AI_Condition{
public:	
	int OpType;
	int Time;
	virtual bool Check(){				
		bool res=PerformAI_Op(AnimTime/25/256,Time,OpType);
		return res&&AI_Condition::Check();
	}
	virtual void GetBriefing(_str& str){
		str="GamingTime";
		str+=getops(OpType);
		str+=Time;		
		str+=" s";
		AI_Condition::GetBriefing(str);
	}
	SAVE(GamingTime)
		REG_PARENT(AI_Condition)
		REG_ENUM  (_index,OpType,OPTYPE)
		REG_MEMBER(_int,Time);
		REG_AUTO  (Condition);
	ENDSAVE;
};
class BuildRequirements:public BaseClass{
public:
	bool NearRoad;
	bool NearTrees;
	bool NearStones;
	bool NearToEnemy;
	bool FarFromEnemy;
	bool FarFromSameType;
	bool MultipleConstruction;
	
	int  MinDistFromCenter;
	int  MaxDistFromCenter;
	
	int  MinPeasants;
	int  MaxPeasants;	

	SAVE(BuildRequirements){
		REG_MEMBER(_bool,NearRoad);
		REG_MEMBER(_bool,NearTrees);
		REG_MEMBER(_bool,NearStones);
		REG_MEMBER(_bool,NearToEnemy);
		REG_MEMBER(_bool,FarFromEnemy);
		REG_MEMBER(_bool,FarFromSameType);
		REG_MEMBER(_bool,MultipleConstruction);
		REG_MEMBER(_int,MinDistFromCenter);
		REG_MEMBER(_int,MaxDistFromCenter);
		REG_MEMBER(_int,MinPeasants);
		REG_MEMBER(_int,MaxPeasants);
	}ENDSAVE;

	__constructor(BuildRequirements){
		NearRoad=0;
		NearTrees=0;
		NearStones=0;	
		MinDistFromCenter=0;
		MinPeasants=10;
		MaxPeasants=15;
		MultipleConstruction=0;
		FarFromSameType=0;
		NearToEnemy=false;
		FarFromEnemy=true;
		MaxDistFromCenter=2500;
	}

	virtual bool ForceProperty(){
		return true;
	}
};
class aia_SplashBonus: public AI_Action {
public:
	int NIndex;
	int Bonus;
	SAVE(aia_SplashBonus){
		REG_PARENT(AI_Action);
		REG_MEMBER(_UnitTypeShortcut,NIndex);
		REG_MEMBER(_int,Bonus);
	}ENDSAVE;
	aia_SplashBonus(){
		Bonus=10;
	}
	virtual bool Perform();
	virtual void GetBriefing(_str& str){
		str.print("SplashBonus(%s=%d)",GetUnitShortcutByID(NATIONS->Mon[NIndex]->MonsterID),Bonus);
	};
	virtual void GetShortBriefing(_str& str){
		str.print("SplashBonus(%s=%d)",GetUnitShortcutByID(NATIONS->Mon[NIndex]->MonsterID),Bonus);
	}
};

class aia_UnitType: public BaseClass {
public:
	int UnitType;
	SAVE(aia_UnitType){
		REG_MEMBER(_UnitTypeShortcut,UnitType);
	}ENDSAVE;
};

class aia_UnitTypeLinks: public AI_Action {
public:
	int NIndex;
	ClonesArray<aia_UnitType> Links;
	SAVE(aia_UnitTypeLinks){
		REG_PARENT(AI_Action);
		REG_MEMBER(_UnitTypeShortcut,NIndex);
		REG_AUTO(Links);
	}ENDSAVE;
	aia_UnitTypeLinks(){
	}
	virtual bool Perform();
	virtual void GetBriefing(_str& str){
		str.print("UnitTypeLinks(%s),link to:",GetUnitShortcutByID(NATIONS->Mon[NIndex]->MonsterID));
		int n=Links.GetAmount();
		for(int i=0;i<n;i++){
			str.print(" %s",GetUnitShortcutByID(NATIONS->Mon[Links[i]->UnitType]->MonsterID));
		}
	};
	virtual void GetShortBriefing(_str& str){
		str.print("UnitTypeLinks(%s)",GetUnitShortcutByID(NATIONS->Mon[NIndex]->MonsterID));
	}
};

class Build:public AI_Action{
public:
	Build(){
		UnitType=0;
		MaxAmount=1;
		ResourcePercent=100;
		Probability=100;
	}
	int UnitType;
	int MaxAmount;
	int ResourcePercent;
    int Probability;
	BuildRequirements Requirements;
	virtual bool Perform(){
        bool TryUnit(byte NI,int UID,int Max,byte CostPercent,byte Probability,BuildRequirements* req);
		TryUnit(CurAI_Color,UnitType,MaxAmount,ResourcePercent,Probability,&Requirements);
		return false;
	}
	virtual void GetBriefing(_str& str){
		str.print("Build(%s),max-%d,res-%d,prob-%d",GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID),MaxAmount,ResourcePercent,Probability);
	};
	virtual void GetShortBriefing(_str& str){
		str.print("Build(%d %s)",MaxAmount,GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID));
	}
	SAVE(Build)
	REG_PARENT(AI_Action);
	REG_MEMBER(_UnitTypeShortcut,UnitType);
	REG_MEMBER(_int,MaxAmount);
	REG_MEMBER(_int,ResourcePercent);
	REG_MEMBER(_int,Probability);
	REG_AUTO(Requirements);
	ENDSAVE;
};
class Produce:public AI_Action{
public:
	Produce(){
		UnitType=0;
		MaxAmount=1;
		ResourcePercent=100;
		Probability=100;
		PackSize=8;
	}
	int UnitType;
	int MaxAmount;
	int ResourcePercent;
	int PackSize;
	int Probability;
	virtual bool Perform(){
		bool TryUnit(byte NI,int UID,int Max,byte CostPercent,byte Probability,BuildRequirements* req);
		if(NATIONS[CurAI_Color].Mon[UnitType]->newMons->Building)return false;
		for(int i=0;i<PackSize;i++)
			TryUnit(CurAI_Color,UnitType,MaxAmount,ResourcePercent,Probability,NULL);
		return false;
	}
	virtual void GetBriefing(_str& str){
		str.print("Produce(%s),max-%d,res-%d,prob-%d",GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID),MaxAmount,ResourcePercent,Probability);
	};
	virtual void GetShortBriefing(_str& str){
		str.print("Produce(%d %s)",MaxAmount,GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID));
	}
	SAVE(Produce){
		REG_PARENT(AI_Action);
		REG_MEMBER(_UnitTypeShortcut,UnitType);
		REG_MEMBER(_int,MaxAmount);
		REG_MEMBER(_int,ResourcePercent);
		REG_MEMBER(_int,PackSize);
		REG_MEMBER(_int,Probability);
	}ENDSAVE;
};
class AI_Act_DeleteNotBusy: public AI_Action {
public:
	int UnitType;
	
	SAVE(Produce){
		REG_PARENT(AI_Act_DeleteNotBusy);
		REG_MEMBER(_UnitTypeShortcut,UnitType);
	}ENDSAVE;

	virtual bool Perform();
	virtual void GetBriefing(_str& str){
		str.print("DeleteNotBusy(%s)",GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID));
	}
	virtual void GetShortBriefing(_str& str){
		str.print("DeleteNotBusy(%s)",GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID));
	}
};
class goDiversionOrder: public BaseClass{
public:
	AUTONEW(goDiversionOrder);
};
class goStorm: public goDiversionOrder{
public:
	SAVE(goStorm){
		REG_PARENT(goDiversionOrder);
	}ENDSAVE;
};
class goPesantKiller: public goDiversionOrder{
public:
	SAVE(goPesantKiller){
		REG_PARENT(goDiversionOrder);
	}ENDSAVE;
};
class goRanger: public goDiversionOrder{
public:
	SAVE(goRanger){
		REG_PARENT(goDiversionOrder);
	}ENDSAVE;
};
class GoDiversion:public AI_Action{
public:
	GoDiversion();
	int Group;
	//int curOrder;
	//ClassArray<goDiversionOrder> Orders;
	int CurAmount;
	int UnitType;
	int MinAmount;
	int MaxAmount;
	int StepAmount;
	bool SingleUnitMotion;
	bool WithoutBrigade;
	bool Melee;	
	int StormPercent;
	bool canAttackBuilding;
	int FirePercent;
	virtual bool Perform();
	virtual void GetBriefing(_str& str){
		str.print("GoDiversion"); //(%s),max-%d,res-%d,prob-%d",GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID),MaxAmount,ResourcePercent,Probability);
	};
	virtual void GetShortBriefing(_str& str){
		str.print("GoDiversion"); //(%d %s)",MaxAmount,GetUnitShortcutByID(NATIONS->Mon[UnitType]->MonsterID));
	}
	SAVE(GoDiversion){
		REG_PARENT(AI_Action);
		REG_MEMBER(_UnitTypeShortcut,UnitType);
		//REG_AUTO(Orders);
		SetReadOnlyMode(); REG_MEMBER(_int,Group);
		SetReadOnlyMode(); REG_MEMBER(_int,CurAmount);
		REG_MEMBER(_int,MinAmount);
		REG_MEMBER(_int,MaxAmount);
		REG_MEMBER(_int,StepAmount);
		REG_MEMBER(_bool,SingleUnitMotion);
		REG_MEMBER(_bool,WithoutBrigade);
		REG_MEMBER(_bool,Melee);
		SAVE_SECTION(1);
		REG_MEMBER(_int,StormPercent);
		SAVE_SECTION(0xFFFFFFFF);
		REG_MEMBER(_bool,canAttackBuilding);
		SAVE_SECTION(2);
		REG_MEMBER(_int,FirePercent);
	}ENDSAVE;
	DWORD GetClassMask(){
		DWORD res=0xF0000000;
		if(Melee) return res|=1;
		else if(canAttackBuilding) return res|=2;
		return res;		
	}
};
extern City CITY[8];
class PeonsDistribution:public AI_Action{
public:
	PeonsDistribution(){
		OnWood=10;
		OnStone=10;
		OnFood=0;
	}
	int OnWood;	
	int OnStone;	
	int OnFood;	
	virtual bool Perform(){
		CITY[CurAI_Color].NeedOnWood=OnWood;
		CITY[CurAI_Color].NeedOnStone=OnStone;
		CITY[CurAI_Color].NeedOnField=OnFood;
		return false;
	}
	virtual void GetBriefing(_str& str);
	virtual void GetShortBriefing(_str& str);
	SAVE(PeonsDistribution)
	REG_PARENT(AI_Action);
	REG_MEMBER(_int,OnWood);
	REG_MEMBER(_int,OnStone);
	REG_MEMBER(_int,OnFood);
	ENDSAVE;
};
class AddResources:public AI_Action{
public:
	int ResType;
	int Amount;	
	AddResources(){
		ResType=0;
		Amount=0;
	}
	virtual bool Perform(){
		AddXRESRC(CurAI_Color,ResType,Amount);
		return false;
	}
	virtual void GetBriefing(_str& str);
	virtual void GetShortBriefing(_str& str);
	SAVE(AddResources)
		REG_PARENT(AI_Action);
		REG_ENUM(_index,ResType,RESTYPE);
		REG_MEMBER(_int,Amount);		
	ENDSAVE;
};
class SettlementsPriory:public AI_Action{
public:	
	int ResType;
	int Prio;
	SettlementsPriory(){
		ResType=0;
		Prio=100;
	}
	virtual bool Perform();
	virtual void GetBriefing(_str& str);
	virtual void GetShortBriefing(_str& str);
	SAVE(SettlementsPriory)
	REG_PARENT(AI_Action);
	REG_ENUM(_index,ResType,RESTYPE);
	REG_MEMBER(_int,Prio);	
	ENDSAVE;
};
class BuyResource:public AI_Action{
public:	
	int ResTypeToSell;
	int ResTypeToBuy;
	int Amount;
	virtual bool Perform();
	virtual void GetBriefing(_str& str);
	virtual void GetShortBriefing(_str& str);
	SAVE(BuyResource)
		REG_PARENT(AI_Action);
		REG_ENUM(_index,ResTypeToSell,RESTYPE);
		REG_ENUM(_index,ResTypeToBuy,RESTYPE);
		REG_MEMBER(_int,Amount);
	ENDSAVE;
};
CIMPORT int vdf_RunUpgInSettlement(byte Owner, int ResType, int UpgLevel, int CostPercent, int MaxDistance);
class UpgradeSettlements:public AI_Action{
public:	
	int ResType;
	int UpgLevel;
	int CostPercent;
	int MaxDistance;
	UpgradeSettlements(){
		ResType=0;
		UpgLevel=0;
		CostPercent=100;
		MaxDistance=1000000;		
	}
	virtual bool Perform(){
		////for vital
		vdf_RunUpgInSettlement(CurAI_Color,ResType,UpgLevel,CostPercent,MaxDistance);
		return true;
	}
	virtual void GetBriefing(_str& str);
	virtual void GetShortBriefing(_str& str);
	SAVE(UpgradeSettlements)
	REG_PARENT(AI_Action);
	REG_ENUM(_index,ResType,RESTYPE);
	REG_ENUM(_index,UpgLevel,LEVELS);
	REG_MEMBER(_int,CostPercent);
	REG_MEMBER(_int,MaxDistance);
	ENDSAVE;
};
class BridgeConstruction: public AI_Action{
public:
	BridgeConstruction(){
		Enable=true;
		BridgeType=3;
	}
	bool Enable;
	int BridgeType;
	int NPeasant;
	SAVE(BridgeConstruction){
		REG_PARENT(AI_Action);
		REG_MEMBER(_int,BridgeType);
		REG_MEMBER(_int,NPeasant);
	}ENDSAVE;
	virtual void GetBriefing(_str& str){
		str.print("Bridge Construction (%d peasants)",NPeasant);
	};
	virtual void GetShortBriefing(_str& str){
		str.print("Bridge(%d)",NPeasant);
	};
	virtual bool Perform();
};

class PerformUpgrade: public AI_Action {
public:
	PerformUpgrade(){
		CostPercent=100;
		MaxCenterR=100000;
		MaxBuildingR=50000;
	}
	
	int UpgradeID;
	int CostPercent;
	int MaxCenterR;
	int MaxBuildingR;

	SAVE(PerformUpgrade){
		REG_PARENT(AI_Action);
		REG_ENUM(_index,UpgradeID,ALL_UPGRADES);
		REG_MEMBER(_int,CostPercent);
		REG_MEMBER(_int,MaxCenterR);
		REG_MEMBER(_int,MaxBuildingR);
	}ENDSAVE;

	virtual bool Perform(){
        bool TryUpgradeEx(byte NI,int UpgID,int CostPercent,int Probability,int MaxCenterR,int MaxBuildingR);
		TryUpgradeEx(CurAI_Color,UpgradeID,CostPercent,100,MaxCenterR,MaxBuildingR);	
		return true;
	};
	virtual void GetBriefing(_str& str){
		if(MaxCenterR!=1000){
			str.print("Upgrade(%s,%d%%,MaxCenterR=%d)",NATIONS->UPGRADE[UpgradeID]->Name,CostPercent,MaxCenterR);
		}else{
			str.print("Upgrade(%s,%d%%)",NATIONS->UPGRADE[UpgradeID]->Name,CostPercent);
		}		
	};
	virtual void GetShortBriefing(_str& str){
		str.print("Upg(%s)",NATIONS->UPGRADE[UpgradeID]->Name);
	};
};
class ai_aEnableIndividualUpgrade: public AI_Action {
public:
	int UpgradeID;

	SAVE(ai_aEnableIndividualUpgrade){
		REG_PARENT(AI_Action);
		REG_ENUM(_index,UpgradeID,ALL_UPGRADES);
	}ENDSAVE;

	virtual void GetShortBriefing(_str& str){
		str.print("EnbUpg(%s)",NATIONS->UPGRADE[UpgradeID]->Name);
	};
	virtual void GetBriefing(_str& str);
	virtual bool Perform();
};
class ai_aDisableIndividualUpgrade: public AI_Action {
public:
	int UpgradeID;

	SAVE(ai_aDisableIndividualUpgrade){
		REG_PARENT(AI_Action);
		REG_ENUM(_index,UpgradeID,ALL_UPGRADES);
	}ENDSAVE;

	virtual void GetShortBriefing(_str& str){
		str.print("DisUpg(%s)",NATIONS->UPGRADE[UpgradeID]->Name);
	};
	virtual void GetBriefing(_str& str);
	virtual bool Perform();
};

class AutoUpRomeHero : public AI_Action
{
public:
	AutoUpRomeHero()
	{
		AutoMove=true;
	};
	bool AutoMove;

	SAVE(AutoUpRomeHero)
		REG_PARENT(AI_Action);
		REG_MEMBER(_bool,AutoMove);
	ENDSAVE;

	virtual void GetBriefing(_str& str)
	{
		str="AutoUpRomeHero";
	};
	virtual void GetShortBriefing(_str& str)
	{
		str="AutoUpRomeHero";
	};
	virtual bool Perform();
};
//
class ScriptsScope:public BaseClass{
public:
	AI_Script Scripts[8];
	//ClassRef<AI_Script> DebugScript;
	AI_Script DebugScript;
	//ScriptsScope() { DebugScript=Scripts; }
    SAVE(ScriptsScope)
		REG_AUTO_EX(DebugScript,Script);
		/*
		for(int i=1;i<8;i++){
			char c[32];
			sprintf(c,"Script%d",i);
			REG_AUTO_EX2(Scripts[i],c);
		}
		*/
	ENDSAVE;
};
//
class cv_AI_Building: public BaseClass{
public:
	cv_AI_Building(){
		grpInvader=0xFFFF;
		Ready=false;
	}
	inline bool Process(City* CT);
	//
	bool Ready;
	word obIndex;
	word obSerial;
	int grpInvader;
	int grpRepaire;
	int repMin;
	int repMax;
	AUTONEW(cv_AI_Building);	
};
class cv_AI_Script: public BaseClass{
public:
	int addNewUnit(OneObject* OB, BuildRequirements* Req);
	void Process(City* CT);
	//
	ClonesArray<cv_AI_Building> Buildings;	
	SAVE(cv_AI_Script){
		REG_AUTO(Buildings);
	}ENDSAVE;
};
class cv_AI_ScriptExt: public GameExtension{
public:
	static BuildRequirements NewUnitReq;
	static void SetNewUnitBuildRequirements(BuildRequirements* Req);
	//
	int pLastTime;	// process last time
	cv_AI_Script Scripts[7];
	//
	virtual void ProcessingGame();
	//virtual void OnGameStart();
	virtual void OnUnloading();
	virtual void OnUnitBirth(OneObject* NewUnit);	
};