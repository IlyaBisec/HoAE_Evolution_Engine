#pragma once
#include "MapTemplates.h"
//////////////////////////////////////////////////////////////////////////
class SimpleDialog;
class OneObject;
CEXPORT void SetCurPtr(int v);
extern DIALOGS_API int VitCursor;
//
class DIALOGS_API vui_Action: public BaseClass{
public:
	//static OneObject* OB;
	static Enumerator* E;
	static Enumerator* VUI;
	virtual void SetFrameState(SimpleDialog* SD){};	// проверка условий действия и влияние на отображаемые диалоги
	virtual bool LeftClick(SimpleDialog* SD){return false;};
	virtual bool isLeftClick(){ return false; };
	virtual bool RightClick(SimpleDialog* SD){return false;};
	virtual bool isRightClick(){ return false; };
	virtual void Init(SimpleDialog* SD){};			// инит/чтение из движка
	virtual void Accept(SimpleDialog* SD){};		// запись в движок
	virtual bool isExpandableFor(SimpleDialog* SD){ return true; };
	virtual void OnDraw(SimpleDialog* SD){};
	char* GetName(){ return Name.pchar(); };
	_str Name;
	//bool init;
	//vui_Action(){ init=false; };
	SAVE(vui_Action) 
		REG_AUTO(Name);
		//REG_MEMBER_EX(_bool,init,Init);
	ENDSAVE;
};
//
//
#define regA_(T,f1,f2,f3,f4) class T: public vui_Action { public: f1 f2 f3 f4 }
#define regA(T,f1,f2,f3,f4) class T: public vui_Action { public: f1 f2 f3 f4 SAVE(T) REG_PARENT(vui_Action); ENDSAVE; }
#define regAx(T,T2,f1,f2,f3,f4) class T: public T2 { public: f1 f2 f3 f4 SAVE(T) REG_PARENT(vui_Action); ENDSAVE; }
//
//
#define vfS void SetFrameState(SimpleDialog* SD);
#define vfSi void SetFrameState(SimpleDialog* SD){ Init(SD); };
#define vfL bool LeftClick(SimpleDialog* SD); bool isLeftClick(){ return true; };
#define vfR bool RightClick(SimpleDialog* SD); bool isRightClick(){ return true; };
#define vfLt bool LeftClick(SimpleDialog* SD){ Lpressed=0; return true; }; bool isLeftClick(){ return true; };
#define vfRt bool RightClick(SimpleDialog* SD){ Rpressed=0; return true; }; bool isRightClick(){ return true; };
#define vfI void Init(SimpleDialog* SD);
#define vfA void Accept(SimpleDialog* SD);
#define vfD void OnDraw(SimpleDialog* SD);
//
#define regAc(T,mem,reg) class T: public vui_Action { public: mem; SAVE(T) REG_PARENT(vui_Action); reg; ENDSAVE; }
#define regAcx(T,Parent,mem,reg) class T: public Parent { public: mem; SAVE(T) REG_PARENT(vui_Action); reg; ENDSAVE; }

//
#pragma warning ( disable : 4003 )
regA(va_Null);
#pragma warning ( pop )
//
class DIALOGS_API vui_ActionArray: public ClassArray<vui_Action>{
public:
	virtual bool ForceProperty();
	virtual const char* GetThisElementView(const char* LocalName);
	//
	bool isLeftClick(){ 
		for(int i=0;i<GetAmount();i++){
			vui_Action* A=GetValues()[i];
			if(A&&A->isLeftClick()){				
				return true;
			}
		}
		return false; 
	};
	bool isRightClick(){ 
		for(int i=0;i<GetAmount();i++){
			vui_Action* A=GetValues()[i];
			if(A&&A->isRightClick()){
				return true;
			}
		}
		return false; 
	};
	bool LeftClick(SimpleDialog* SD);
	bool RightClick(SimpleDialog* SD){ 
		bool click=false;
		for(int i=0;i<GetAmount();i++){
			vui_Action* A=GetValues()[i];
			if(A&&A->RightClick(SD)) click=true;
		}
		return click; 
	}
	vui_Action* isExist(vui_Action* Act){		
		if(dynamic_cast<va_Null*>(Act)){
			return Act;
		}
		for(int i=0;i<GetAmount();i++){
			vui_Action* A=GetValues()[i];
			if(A){
				const char* cname=A->GetClassName();
				if(!strcmp(cname,Act->GetClassName())) return A;
			}
		}
		return NULL;
	}
	void SetFrameState(SimpleDialog* SD);
	void Accept(SimpleDialog* SD){
		for(int i=0;i<GetAmount();i++){
			vui_Action* A=GetValues()[i];
			if(A) A->Accept(SD);
		}
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
//
enum veGameMode { gmSin=0,gmMul,gmEdit,gmCamp };
extern DIALOGS_API veGameMode vGameMode;
//
template <class T> inline void reg_v_Action(char* Name,char* Section=NULL){
	T* v=new T;
	if(Name&&Name[0]){
		v->Name=Name;
		if(Section)vui_Action::E->Add(v->GetName(),(DWORD)v,Section);		
		else vui_Action::E->Add(v->GetName(),(DWORD)v);
	}
	if(Section){
		int i=0;
		while(Section[i]!='_')i++;
		Section+=i+1;
		i=0;
		while(Section[i]!='_' && Section[i])i++;
		_str s; s=Section;
		s.pchar()[i]=0;		
		REG_CLASS_EX(T,s.pchar());
	}else REG_CLASS_EX(T,"Old style");
};
#define reg_cva(T,Name) reg_v_Action<T>(Name,#T);