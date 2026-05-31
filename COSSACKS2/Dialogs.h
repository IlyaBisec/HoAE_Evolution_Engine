#ifndef Dialogs_Header_1234567890
#define Dialogs_Header_1234567890

#ifdef DIALOGS_DIP_USER
#define DIP_API __declspec(dllexport)
#else
#define DIP_API __declspec(dllimport)
#endif

#ifndef DIALOGS_API
#ifndef DIALOGS_USER
#define DIALOGS_API __declspec(dllexport)
#else//DIALOGS_USER
#define DIALOGS_API __declspec(dllimport)
//#define BaseMesh void
class BaseMesh;
#endif//DIALOGS_USER
#endif//DIALOGS_API

//#define MAXDLG 1024
#include "..\ClassEngine\ClassEngine.h"
#include "..\ClassEngine\more_types.h"
#include "maptemplates.h"
#include "DrawForm.h"
#include "TypeExchange.h"

DIALOGS_API int GetSound(char* Name);
DIALOGS_API char* GetTextByID(char*);

#include "vui_Action.h"
#include "GP_Draw.h"

CEXPORT
int DrawMultilineText(int x,int y,char* s,RLCFont* DefaultFont,int& MaxLx,int MaxLy,byte CenterX,bool Draw);

class ControlToClassConnector:public BaseClass{
public:
	ClassRef<ReferableBaseClass> RootClass;
	//examples of ClassFieldPath:
	//$root.field
	//$root.class1[1].field
	//$root.field.subfeld
	_str			ClassFieldPath;
	//bool			ClassDependsOnControl;
	//bool			ControlDependsOnClass;
	BaseTriplet		GetClass();	
	SAVE(ControlToClassConnector){
		REG_AUTO(ClassFieldPath);
		REG_AUTO(RootClass);
		//REG_MEMBER(_bool,ClassDependsOnControl);
		//REG_MEMBER(_bool,ControlDependsOnClass);
	}ENDSAVE;
	virtual bool ForceSimplification(){
		return true;
	}
};
//accepts int,float,DWORD,_str,bool
class SimpleClassConnector:public ControlToClassConnector{		
public:
	_str  prev_str;
	void  _exchange(_str&  val);
	void  _exchange(char** val);
	void  _exchange(int&   val);
	void  _exchange(DWORD& val);
	void  _exchange(float& val);
	void  _exchange(bool&  val);
	SAVE(SimpleClassConnector){
		REG_PARENT(ControlToClassConnector);
	}ENDSAVE;
};
//accepts int
class IntClassConnector:public ControlToClassConnector{		
public:
	int  prev_val;
	virtual void  _exchange(int&  val);
	SAVE(IntClassConnector){
		REG_BASE(ControlToClassConnector);
		REG_AUTO_EX(ClassFieldPath,_int_VriableReference);
		REG_AUTO(RootClass);
	}ENDSAVE;
};
//accepts bool
class BoolClassConnector:public ControlToClassConnector{		
public:
	bool  prev_val;
	virtual void  _exchange(bool&  val);
	SAVE(BoolClassConnector){
		REG_BASE(ControlToClassConnector);
		REG_AUTO_EX(ClassFieldPath,_bool_VriableReference);
		REG_AUTO(RootClass);
	}ENDSAVE;
};
class IndexComparisonClassConnector:public BoolClassConnector{
public:
	int Is_true_if_equal_to;
	virtual void  _exchange(bool&  val);
	SAVE(IndexComparisonClassConnector){
		REG_BASE(ControlToClassConnector);
		REG_BASE(BoolClassConnector);
		REG_AUTO_EX(ClassFieldPath,_int_VriableReference);
		REG_MEMBER(_int,Is_true_if_equal_to);
	}ENDSAVE;	
};
//accepts _index,_byte_index
class IndexedClassConnector:public ControlToClassConnector{
	int prev_value;
public:
	void			_exchange(int& value);
	Enumerator*		_get_enum();
	SAVE(IndexedClassConnector){
		REG_BASE(ControlToClassConnector);
		REG_AUTO_EX(ClassFieldPath,_index_VariableReference);
		REG_AUTO(RootClass);
	}ENDSAVE;
};
class ClassCallParams:public BaseClass{
public:
    _str Parameter;
	_str Value;
	Delimiter D;
	virtual bool ForceSimplification(){
		return true;
	}
	SAVE(ClassCallParams);
		REG_AUTO(Parameter);
		REG_AUTO(Value);
		REG_AUTO(D);
	ENDSAVE;
};
class ClassParamList:public ClonesArray<ClassCallParams>{
	virtual bool ForceSimplification(){
		return true;
	}
};
class CallingClassConnector:public ControlToClassConnector{
public:
	void Call();
	int ReasonToCall;
	Delimiter D;
	ClassParamList Params;
	SAVE(CallingClassConnector){
		REG_CLASS(ClassCallParams);
        REG_BASE(ControlToClassConnector);
		REG_ENUM(_index,ReasonToCall,CALLACTION);
		REG_AUTO_EX(ClassFieldPath,BaseFunction_Reference);
		REG_AUTO(RootClass);
		REG_AUTO(D);
		REG_AUTO(Params);
	}ENDSAVE;
	virtual bool ForceSimplification(){return false;}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
class ArrayConnector:public ControlToClassConnector{
public:
	SAVE(ArrayConnector);
		REG_BASE(ControlToClassConnector);
		REG_AUTO_EX(ClassFieldPath,ClassArray_Reference);
		REG_AUTO(RootClass);
	ENDSAVE;    	
};
class DIALOGS_API SimpleDialog;
typedef bool VCall(SimpleDialog* SD);
typedef bool VCallXY(SimpleDialog* SD,int x,int y,int ActiveID);
class DialogsSystem;
class DIALOGS_API SimpleDialog;
class DIALOGS_API VScrollBar;
class DIALOGS_API AlexResource;

// ---
enum PropType {
	Integer = 0,
	String = 1
};

class SimpleDialog;
class UniversalButton;
class Picture;
class RLCPicture;
class TextButton;
class TextMessage;
class GP_TextButton;
class GP_Button;
class BpxTextButton;
class ListBox;
class ComplexBox;
class CustomBox;
class InputBox;
class InputBoxOld;
class CheckBox;
class ColoredBar;
class TextViewer;
class BPXView;
class RLCListBox;
class ComboBox;
class ComboBox;
class WinComboBox;
class GP_PageControl;
class VScrollBar;
class VideoButton;
class CustomBorder;
class GP_TextButton;
class ChatViewer;
class BaseMeshDialog;
class Video;
class BorderEx;
class GPPicture;
class Canvas;
class DialogsDesk;
class DialogsSystem;
class ProgressBar;
typedef void procDrawBoxElement(int x,int y,int Lx,int Ly,int Index,byte Active,int param);
typedef void tpShowHint(DialogsSystem* DSS,SimpleDialog* SD,char* str,int mX,int mY,int StayTime);
typedef void tpDrawHintBorder(int x,int y,int Lx,int Ly,DWORD Color);
class SimpleDialog;
class SimpleDialogRef:public ClassRef<SimpleDialog>{
public:
	bool ForceProperty(){
		return true;
	}
	bool CheckIfEditableElementIsNode(){
		return false;
	}
};

template <class CLASS> class _conn:public AllPtr<CLASS>{
	const char* GetThisElementView(const char* Local){
		if(Get()){
			ControlToClassConnector* CC=dynamic_cast<ControlToClassConnector*>(Get());
			if(CC){
				char* b=GetGlobalBuffer();
				sprintf(b,"{CW}%s:{CY}%s",Local,CC->ClassFieldPath.pchar());
				return b;
			}
		}
		return NULL;
	}
};
class DIALOGS_API ParentFrame:public ReferableBaseClassWithSource{
public:
	void Init();
	void reset_class(void* data){
		((ParentFrame*)data)->BaseClass::reset_class(data);
		((ParentFrame*)data)->Init();
	}
	bool Visible;
	void AttachToClass(ReferableBaseClass* BC,const char* RootName="$root");

	int	BaseX,BaseY;
	int x,y,x1,y1;	
	int LastDrawX,LastDrawY;
	bool EditMode;
	bool Selected;
	ParentFrame* ParentDS;
	DWORD Diffuse;	
	bool DeepColor:1;
	bool CoorIsGlobal:1;
	int  globalX();
	int  globalY();
	int  globalX1();
	int  globalY1();
	static int ActiveX;
	static int ActiveY;
	static int ActiveX1;
	static int ActiveY1;
	static int ActiveID;
	static int ActiveParentX;
	static int ActiveParentY;
	static SimpleDialog* ActiveParent;
	static ParentFrame* OnlyActiveParent;
	void GetDialogsFrame(int& x0,int& y0,int& x1,int& y1);
	bool CheckProcessAbility(){
		if(OnlyActiveParent){
			ParentFrame* PF=this;
			while(PF){
				if(PF==OnlyActiveParent)return true;
				if(PF==ParentDS)return false;
				PF=ParentDS;
			}
			return false;
		}else return true;
	}
	int LeftAlign;	
	int RightAlign;	
	int HorizontalCenterAlign;	
	int TopAlign;	
	int BottomAlign;	
	int VerticalCenterAlign;
	void ProcessAligning();
	void ProcessLocalAligning();

	SimpleDialogRef LeftAlignTo;
	Delimiter D1;
	SimpleDialogRef RightAlignTo;
	Delimiter D2;
	SimpleDialogRef HorizontalCenterAlignTo;
	Delimiter D3;
	SimpleDialogRef TopAlignTo;
	Delimiter D4;
	SimpleDialogRef BottomAlignTo;
	Delimiter D5;
	SimpleDialogRef VerticalCenterAlignTo;

	float LA_param;
	float RA_param;
	float HCA_param;
	float TA_param;
	float BA_param;
	float VCA_param;

	int GetLeftAlign(){
		return LeftAlign;
	}
	void SetLeftAlign(int p);
	INT_PROPERTY(ParentFrame,GetLeftAlign,SetLeftAlign);

	int GetRightAlign(){
		return RightAlign;
	}
	void SetRightAlign(int p);
	INT_PROPERTY(ParentFrame,GetRightAlign,SetRightAlign);

	int GetHCenterAlign(){
		return HorizontalCenterAlign;
	}
	void SetHCenterAlign(int p);
	INT_PROPERTY(ParentFrame,GetHCenterAlign,SetHCenterAlign);

	int GetTopAlign(){
		return TopAlign;
	}
	void SetTopAlign(int p);
	INT_PROPERTY(ParentFrame,GetTopAlign,SetTopAlign);

	int GetBottomAlign(){
		return BottomAlign;
	}
	void SetBottomAlign(int p);
	INT_PROPERTY(ParentFrame,GetBottomAlign,SetBottomAlign);

	int GetVCenterAlign(){
		return VerticalCenterAlign;
	}
	void SetVCenterAlign(int p);
	INT_PROPERTY(ParentFrame,GetVCenterAlign,SetVCenterAlign);


	ClassArray<SimpleDialog> DSS;
	void vm_ActionsAccept();
	void vm_ActionsCancel();
	
	void SetEditMode(bool Mode);

	ParentFrame();
	~ParentFrame(){
		DSS.Clear();
	}

	virtual void GetMargin(int&left,int&top,int&right,int&bottom){
		left   = 0;
		right  = 0;
		top    = 0;
		bottom = 0;
	}
	virtual void GetShift(int&dx,int&dy){
		dx=0;
		dy=0;
	}
	virtual bool ShiftDialog(int dx,int dy){
		x+=dx;
		y+=dy;
		x1+=dx;
		y1+=dy;
		return true;
	}
	virtual bool ResizeDialog(int dx,int dy){
		x1+=dx;
		y1+=dy;
		return true;
	}
	//
	void CloseDialogs();
	void AddDialog(SimpleDialog* SD);
	bool InsertDialog(int pos, SimpleDialog* SD);
	//
	SimpleDialog* Find(DWORD ID);
	bool DeleteItem(DWORD ID);

	SimpleDialog* Find(char* ID);
	bool DeleteItem(char* ID);

	Picture* addPicture(SimpleDialog* Parent,int x,int y,
		SQPicture* Active,
		SQPicture* Passive,
		SQPicture* Disabled);
	GPPicture* addGPPicture(SimpleDialog* Parent,
		int dx,int dy,int FileID,int SpriteID);
	RLCPicture* addRLCPicture(SimpleDialog* Parent,int x,int y,
		RLCTable* Active,byte apic,
		RLCTable* Passive,byte ppic,
		RLCTable* Disabled,byte dpic);
	TextMessage* addTextMessage(SimpleDialog* Parent,int x,int y,char* str,RLCFont* Font,byte Align);
	TextMessage* addsTextMessage(SimpleDialog* Parent,int x,int y,char* str);
	TextButton* addTextButton(SimpleDialog* Parent,int x,int y,char* str,
		RLCFont* Active,
		RLCFont* Passive,
		RLCFont* Disabled,
		byte Align);//==0-left, ==1-center,  ==2-right
	TextButton* addVTextButton(SimpleDialog* Parent,int x,int y,char* str,
		RLCFont* Active,
		RLCFont* Passive,
		RLCFont* Disabled,
		byte Align);//==0-left, ==1-center,  ==2-right
	TextButton* addsTextButton(SimpleDialog* Parent,int x,int y,char* str);
	GP_TextButton* addGP_TextButton(SimpleDialog* Parent,int x,int y,char* str,
		int GP_File,int Sprite,RLCFont* Active,RLCFont* Passive);
	GP_TextButton* addGP_TextButtonLimited(SimpleDialog* Parent,int x,int y,char* str,
		int GP_File,int SpriteActive,int SpritePassive,int Lx,RLCFont* Active,RLCFont* Passive);
	GP_Button* addGP_Button(SimpleDialog* Parent,int x,int y,int GP_File,int Active,int Passsive);
	BpxTextButton* addBpxTextButton(SimpleDialog* Parent,int x,int y,char* str,
		RLCFont* Active,
		RLCFont* Passive,
		RLCFont* Disabled,
		SQPicture* pActive,
		SQPicture* pPassive,
		SQPicture* pDisabled);
	VScrollBar* addVScrollBar(SimpleDialog* Parent,int x,int y,int MaxPos,int Pos,
		SQPicture* btn_up0,
		SQPicture* btn_up1,
		SQPicture* btn_up1p,
		SQPicture* btn_dn0,
		SQPicture* btn_dn1,
		SQPicture* btn_dn1p,
		SQPicture* sbar0,
		SQPicture* sbar1,
		SQPicture* marker);
	VScrollBar* addHScrollBar(SimpleDialog* Parent,int x,int y,int MaxPos,int Pos,
		SQPicture* btn_up0,
		SQPicture* btn_up1,
		SQPicture* btn_up1p,
		SQPicture* btn_dn0,
		SQPicture* btn_dn1,
		SQPicture* btn_dn1p,
		SQPicture* sbar0,
		SQPicture* sbar1,
		SQPicture* marker);
	ListBox* addListBox(SimpleDialog* Parent,int x,int y,int Ny,
		SQPicture* ItemPic,
		RLCFont* AFont,
		RLCFont* PFont,
		VScrollBar* VS);
	ListBox* addListBox(SimpleDialog* Parent,
		int x,int y,int Ny,int Lx,int Ly,
		RLCFont* AFont,
		RLCFont* PFont,
		VScrollBar* VS);
	ListBox* addGP_ListBox(SimpleDialog* Parent,int x,int y,int Ny,
		int GP_File,int Sprite,int Ly,
		RLCFont* AFont,
		RLCFont* PFont,
		VScrollBar* VS);
	ComplexBox* addComplexBox(int x,int y,int Ny,int OneLy,
		procDrawBoxElement* PDRAW,int GP_File,int Spr);
	CustomBox* addCustomBox(int x,int y,int Lx,int Ly,procDrawBoxElement* PDRAW);
	InputBoxOld* addInputBoxOld(SimpleDialog* Parent,int x,int y,char* str,int Len,SQPicture* Panel,RLCFont* RFont,RLCFont* AFont);
	InputBoxOld* addInputBoxOld(SimpleDialog* Parent,int x,int y,char* str,int Len,int Lx,int Ly,RLCFont* RFont,RLCFont* AFont,bool Centering);
	InputBoxOld* addInputBoxOld(SimpleDialog* Parent,int x,int y,_str* str,int Len,int Lx,int Ly,RLCFont* RFont,RLCFont* AFont,bool Centering);
	InputBoxOld* addInputBoxOld(SimpleDialog* Parent,
		int x,int y,char* str,
		int Len,
		int Lx,int Ly,
		RLCFont* RFont,
		RLCFont* AFont);
	InputBox* addInputBox(SimpleDialog* Parent,int x,int y,char* str,int Len,SQPicture* Panel,RLCFont* RFont,RLCFont* AFont);
	InputBox* addInputBox(SimpleDialog* Parent,int x,int y,char* str,int Len,int Lx,int Ly,RLCFont* RFont,RLCFont* AFont,bool Centering);
	InputBox* addInputBox(SimpleDialog* Parent,int x,int y,_str* str,int Len,int Lx,int Ly,RLCFont* RFont,RLCFont* AFont,bool Centering);
	InputBox* addInputBox(SimpleDialog* Parent,int x,int y,char* str,int Len,int Lx,int Ly,RLCFont* RFont,RLCFont* AFont);
	CheckBox* addCheckBox(SimpleDialog* Parent,int x,int y,char* Message,
		int group,bool State,
		SQPicture* OnPict,
		SQPicture* OffPict,
		RLCFont* Font,
		RLCFont* AFont);
	CheckBox* addGP_CheckBox(SimpleDialog* Parent,
		int x,int y,char* message,RLCFont* a_font,RLCFont* p_font,
		int group,bool State,
		int GP,int active,int passive,int mouseover);
	CheckBox* addGP_CheckBox2(SimpleDialog* Parent,
		int x,int y,char* message,RLCFont* a_font,RLCFont* p_font,
		int group,bool State,
		int GP,int active,int passive,int mouseover);
	SimpleDialog* addViewPort(int x,int y,int Nx,int Ny);
	ColoredBar* addViewPort2(int x,int y,int Nx,int Ny,byte Color);
	ColoredBar* addColoredBar(int x,int y,int Nx,int Ny,byte c);
	TextViewer* addTextViewer(SimpleDialog* Parent,int x,int y,int Lx,int Ly,char* TextFile,RLCFont* TFont); 
	BPXView* addBPXView(SimpleDialog* Parent,int x,int y,int OneLx,int OneLy,int Nx,int Ny,int RealNy,byte* Ptr,VScrollBar* VSC);
	RLCListBox* addRLCListBox(SimpleDialog* Parent,int x,int y,int Lx,int Ly,int GPIndex,byte BGColor,byte SelColor);
	ComboBox* addComboBox(SimpleDialog* Parent,int x,int y,int Lx,int Ly,int LineLy,
		byte BackColor,byte BorderColor,
		RLCFont* ActiveFont,RLCFont* PassiveFont,
		char* Contence);
	ComboBox* addGP_ComboBox(SimpleDialog* Parent,int x,int y,int GP_File,
		int UpPart,int Center,int DownPart,
		RLCFont* ActiveFont,RLCFont* PassiveFont,
		char* Contence);
	ComboBox* addGP_ComboBoxDLX(SimpleDialog* Parent,int x,int y,int LX,int GP_File,
		int UpPart,int Center,int DownPart,
		RLCFont* ActiveFont,RLCFont* PassiveFont,
		char* Contence);
	WinComboBox* addWinComboBox(SimpleDialog* Parent,char* Message,int x,int y,int Lx,int Ly,
		int ListX,int ListY,int ListLx,int ListLy,
		RLCFont* ActiveFont,RLCFont* PassiveFont,
		WinComboBox** Group,int NInGroup,int CurBox);
	GP_PageControl* addPageControl(SimpleDialog* Parent,int x,int y,int GF_File,int FirstIndex);
	VScrollBar* addGP_ScrollBar(SimpleDialog* Parent,int x,int y,
		int MaxPos,int Pos,int GP_File,
		int ScrIndex,int LineIndex,int ScrDx,int ScrDy);
	VScrollBar* addGP_ScrollBarL(SimpleDialog* Parent,int x,int y,
		int MaxPos,int Pos,int GP_File,
		int ScrIndex,int LineLx,int LineLy,int ScrDx,int ScrDy);
	VScrollBar* addNewGP_VScrollBar(SimpleDialog* Parent,int x,int y,int Ly,
		int MaxPos,int Pos,int GP_File,int Sprite);
	VideoButton* addVideoButton(SimpleDialog* Parent,int x,int y,int GP1,int GP2);
	CustomBorder* addCustomBorder(int x,int y,int x1,int y1,int gp,int* bn,int* ba,int fill_n,int fill_a);
	GP_TextButton* addStdGP_TextButton(int x,int y,int Lx,char* str,
		int GP_File,int Sprite,RLCFont* Active,RLCFont* Passive);
	ChatViewer* addChatViewer(SimpleDialog* Parent,int x,int y,int Ny,int OneLy,int OneLx
		,char*** Mess,char*** Name,int* Nchats);
	SimpleDialog* addClipper(int x0,int y0,int x1,int y1);
	Canvas* AddCanvas(int x,int y,int Lx,int Ly);
	BorderEx* addBorder(int x,int y,int x1,int y1,int Ymid,byte Style);
	Video* addVideo(int x,int y,int Lx,int Ly);
	BaseMeshDialog* addBaseMeshDialog(int x,int y,int Lx,int Ly);
	//--------------------New style elements---------------//
	UniversalButton* addUniversalButton(int x,int y,int Lx,char* str,int GP_File,
		int* SprSet,int Group,int NowSelected,bool tiling,
		RLCFont* Active,RLCFont* Passive,RLCFont* Selected);
	UniversalButton* addTextureStrip(int x,int y,int Lx,int GP_File,int L,int C1,int C2,int C3,int R,bool Tiling);
	DialogsDesk* AddDialogsDesk(int x,int y,int Lx,int Ly,const char* BorderID);
	//////////////////////////////////////////////////////////////////////////	
	inline virtual int GetWidth();
	inline virtual void SetWidth(int w);
	inline int GetHeight(){return y1-y+1;}
	inline virtual void SetHeight(int h);
	INT_PROPERTY(ParentFrame,GetWidth,SetWidth);
	INT_PROPERTY(ParentFrame,GetHeight,SetHeight);
	inline int Getx(){return x;}
	inline void Setx(int _x);
	inline int Gety(){return y;}
	inline void Sety(int _y);
	inline void Setxy(int _x, int _y);

	INT_PROPERTY(ParentFrame,Getx,Setx);
	INT_PROPERTY(ParentFrame,Gety,Sety);
	//
	bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	BOOL_PROP(ParentFrame,DeepColor);
	SubSection Position;
	SubSection ColorParams;
	SubSection Aligning;
	SubSection AutoLoadSave;

	SubSection Transform;
	bool EnableTransform;
	byte PivotPosition;
	float ShiftX;
	float ShiftY;
	float PivotDx;
	float PivotDy;
	float ScaleX;
	float ScaleY;
	float Angle;
	bool  FlipX;
	bool  FlipY;
    SubSection Optimisation;
    bool  DisableClipping;
    bool  ForceFlushSprites;
    bool  EnableLevelSorting;
    int   ExtraSubLevel;
	void GetMatrix(Matrix4D& M);
	//void reset_class(void* DataPtr);
    Matrix4D* PushMatrix(Matrix4D& TempStore);
    void PopMatrix(Matrix4D* M);
	SAVE(ParentFrame)
		SAVE_SECTION(0xFFFFFFFF);
		REG_AUTO_EX(DSS,ChildDialogs);
		//
		REG_AUTO(Aligning);		
		REG_INDEXPROP(_index,LeftAlign,GetLeftAlign,SetLeftAlign,HAlignType);
		SAVE_SECTION(2);

		REG_MEMBER_EX(_float,LA_param,LeftAlignParam);
		REG_AUTO(LeftAlignTo);

		SAVE_SECTION(0xFFFFFFFF);
		REG_AUTO(D1);
		REG_INDEXPROP(_index,RightAlign,GetRightAlign,SetRightAlign,HAlignType);
		SAVE_SECTION(4);

		REG_MEMBER_EX(_float,RA_param,RightAlignParam);
		REG_AUTO(RightAlignTo);

		SAVE_SECTION(0xFFFFFFFF);
		REG_AUTO(D2);
		REG_INDEXPROP(_index,TopAlign,GetTopAlign,SetTopAlign,VAlignType);
		SAVE_SECTION(16);

		REG_MEMBER_EX(_float,TA_param,TopAlignParam);
		REG_AUTO(TopAlignTo);

		SAVE_SECTION(0xFFFFFFFF);
		REG_AUTO(D3);
		REG_INDEXPROP(_index,BottomAlign,GetBottomAlign,SetBottomAlign,VAlignType);
		SAVE_SECTION(32);

		REG_MEMBER_EX(_float,BA_param,BottomAlignParam);
		REG_AUTO(BottomAlignTo);

		SAVE_SECTION(0xFFFFFFFF);
		REG_AUTO(D4);
		REG_INDEXPROP(_index,HorizontalCenterAlign,GetHCenterAlign,SetHCenterAlign,HAlignType);		
		SAVE_SECTION(8);

		REG_MEMBER_EX(_float,HCA_param,HCenterAlignParam);
		REG_AUTO(HorizontalCenterAlignTo);

		SAVE_SECTION(0xFFFFFFFF);
		REG_AUTO(D5);
		REG_INDEXPROP(_index,VerticalCenterAlign,GetVCenterAlign,SetVCenterAlign,VAlignType);
		SAVE_SECTION(64);

		REG_MEMBER_EX(_float,VCA_param,VCenterAlignParam);
		REG_AUTO(VerticalCenterAlignTo);

		SAVE_SECTION(0xFFFFFFFF);
		//
		REG_AUTO_EX(Position,Position&Width);		
		REG_INTPROP(_int,x,Getx,Setx);
		REG_INTPROP(_int,y,Gety,Sety);
		REG_INTPROP(_int,Width,GetWidth,SetWidth);
		REG_INTPROP(_int,Height,GetHeight,SetHeight);

		INVISIBLE NOSAVE REG_MEMBER_EX(_int,x,X);
		INVISIBLE NOSAVE REG_MEMBER_EX(_int,y,Y);

		//
		REG_AUTO(ColorParams);		
		REG_BOOL_PROP(DeepColor);
		REG_MEMBER_EX(_color,Diffuse,Color);

		REG_AUTO(Transform);
		REG_MEMBER(_bool,EnableTransform);
		REG_ENUM(_byte_index,PivotPosition,TEXTALIGN);
		REG_MEMBER(_float,PivotDx);
		REG_MEMBER(_float,PivotDy);
		REG_MEMBER(_float,ScaleX);
		REG_MEMBER(_float,ScaleY);
		REG_MEMBER(_float,Angle);
		REG_MEMBER(_bool,FlipX);
		REG_MEMBER(_bool,FlipY);

		REG_AUTO(AutoLoadSave);
		REG_FILEPATH(Source,"auto");
		REG_MEMBER(_bool,AlwaysLoadFromSource);
		REG_MEMBER(_bool,AlwaysSaveToSource);
        REG_AUTO(Optimisation);
        REG_MEMBER(_bool,DisableClipping);
        REG_MEMBER(_bool,ForceFlushSprites);
        REG_MEMBER(_bool,EnableLevelSorting);
        REG_MEMBER(_int,ExtraSubLevel);
	ENDSAVE;
	DWORD GetClassMask(){
        DWORD M=1;
		if(LeftAlign&&LeftAlign<3)M|=2;
		if(RightAlign&&RightAlign<3)M|=4;
		if(HorizontalCenterAlign&&HorizontalCenterAlign<3)M|=8;
		if(TopAlign&&TopAlign<3)M|=16;
		if(BottomAlign&&BottomAlign<3)M|=32;
		if(VerticalCenterAlign&&VerticalCenterAlign)M|=64;
		return M;
	}
	virtual bool OnChangeMember(BaseClass* MembClass,void* MembPtr,void* MembExtra,const char* MembName);
	//virtual bool ShouldSaveInShortForm(){return true;}
	virtual const char* GetThisElementView(const char* LocalName);
};
class DIALOGS_API AlexResource{
public:
	~AlexResource();

	int GetPropValueSize(void* value, PropType type) const;
	bool RegisterProp(void* value, char* name, PropType type);
	int GetNProp() const;

	virtual char*		GetPropName(int id) const;
	virtual PropType	GetPropType(int id) const;
	virtual void*		GetPropValue(int id) const;
	virtual bool		SetPropValue(int id, void* value);

	virtual int			GetPropID(char* name) const;
	virtual PropType	GetPropType(char* name) const;
	virtual void*		GetPropValue(char* name) const;
	virtual bool		SetPropValue(char* name, void* value);

	//virtual bool GetPropertyValue(char* id,xmlQuote& Value);
	//virtual bool SetPropertyValue(char* id,xmlQuote& Value);
	//virtual int  GetIntPropertyValue(char* id);
	//virtual bool SetIntPropertyValue(char* id,int v);
private:
	DynArray<char*> Name;
	DynArray<PropType> Type;
	DynArray<void*> Value;
	//int val;
};

#define VtoS(x) #x
// ---
class DIALOGS_API DialogsSystem:public ParentFrame{
	int PrevHintX;
	int PrevHintY;
	int PrevHintTime;
public:
	void Init();
	void reset_class(void* ptr){
		((DialogsSystem*)ptr)->BaseClass::reset_class(ptr);
		((DialogsSystem*)ptr)->Init();
	}
	bool IsDrawActive;// new 25.02
	short OkSound;
	short CancelSound;
	short UserClickSound;
	bool DisableHotKeyInMapEditor;
	int HintX,HintY;
	bool HintXYisGlobal;
	int HintLX,HintLY;
	int HintLineDistance;
	byte HintStyle;
	char* Hint;
	DWORD HintBackColor;
	DWORD HintFontColor;
	tpShowHint* HintCallback;
	tpDrawHintBorder* HintBorderCallback;
	bool HintFade:1;
	bool HintMovesWithMouse:1;
	bool HintBorder:1;
	byte HintAligningX;//0-left 2-right 1-center 3-auto
	byte HintAligningY;//0-up 2-down 1-center 3-auto
	int HintFadeTime;

	RLCFont* Active;
	RLCFont* Passive;
	RLCFont* Disabled;
	RLCFont* Message;
	RLCFont* HintFont;
	bool CenteredHint;
	byte HintFontAlign;

	_str DefaultHint;

	DialogsSystem(int x,int y);
	DialogsSystem();
	virtual ~DialogsSystem(){
		CloseDialogs();
	};
	void CloseDialogs();
	void SetHintStyle(bool MotionWithMouse,bool Border,bool Fade,
		int FadeTime,byte AligningX,byte AligningY,
		int dx,int dy,int MaxLx,int MaxLy,RLCFont* Font,
		DWORD BackColor,DWORD FontColor);
	void SetFonts(RLCFont* Active,
		RLCFont* Passive,
		RLCFont* Disabled,
		RLCFont* Message);

	void ProcessDialogs();
	void MarkToDraw();
	void RefreshView();
	void LoadScreenFromFile(char* XMLFile);

	//properties
	BOOL_PROP(DialogsSystem,HintFade);
	BOOL_PROP(DialogsSystem,HintMovesWithMouse);
	BOOL_PROP(DialogsSystem,HintBorder);

	SubSection ss_Hint;

	//registration
	SAVE(DialogsSystem)		
		REG_LOADSAVE("Dialogs\\","*.DialogsSystem.xml");
		REG_AUTO_EX(ss_Hint,Hint section);
		REG_MEMBER(_bool,DisableHotKeyInMapEditor);
		REG_AUTO(DefaultHint);
		REG_MEMBER(_font,HintFont);
		REG_ENUM(_byte_index,HintFontAlign,TEXTALIGN);		
		REG_MEMBER(_int,HintX);
		REG_MEMBER(_int,HintY);
		REG_MEMBER(_bool,HintXYisGlobal);
		REG_MEMBER(_int,HintLX);
		REG_MEMBER(_int,HintLY);
		REG_MEMBER(_int,HintLineDistance);
		REG_ENUM(_byte_index,HintAligningX,HintAligningX);
		REG_ENUM(_byte_index,HintAligningY,HintAligningY);
		REG_BOOL_PROP(HintFade);
		REG_MEMBER(_int,HintFadeTime);
		REG_BOOL_PROP(HintMovesWithMouse);
		REG_BOOL_PROP(HintBorder);
		REG_PARENT(ParentFrame);
	ENDSAVE;
};
//
bool vui_UserLeftClick(SimpleDialog* SD);
bool vui_UserRightClick(SimpleDialog* SD);
//
enum CallAction{
	__OnLeftClick,
	__OnRightClick,
	__OnMouseWheel,
	__OnBeginDrag,
	__OnDrag,
	__OnDropOnThis,
	__OnDropThis
};
struct ActionCallParams{
	int ActionType;
	SimpleDialog* Clicked;
	SimpleDialog* Dragged;
	SimpleDialog* DropOver;
	void set(int _ActionType,SimpleDialog* _Clicked,SimpleDialog* _Dragged,SimpleDialog* _DropOver){
        ActionType=_ActionType;
		Clicked=_Clicked;
		Dragged=_Dragged;
		DropOver=_DropOver;
	}
};
class ArrayOfCallingClassConnector:public ClonesArray<CallingClassConnector>{
public:
	virtual bool ForceProperty(){return true;}
};
class DIALOGS_API SimpleDialog: public ParentFrame{
public:
	void Init();
	void reset_class(void* data){
		((SimpleDialog*)data)->BaseClass::reset_class(data);
		((SimpleDialog*)data)->Init();
	}	
	bool Enabled;
	bool Active:1;
	bool PrevMouseOver:1;
	bool MouseOver:1;
	bool NeedToDraw:1;
	bool MouseOverActive:1;
	bool IsActive:1;//drop-down panel is active	
	bool AllocHint:1;
	bool Changed:1;	
	bool AllowDrag;
	bool AllowDropOverThisDialog;
	bool NeedRedrawAll;

	int MouseSound;
	int ClickSound;
	int UserParam;	
	char* Hint; 
	char* AllocPtr;
    
	static ClassRef<SimpleDialog> DraggedDialog;
	static ActionCallParams CallParams;
	SubSection ConnectToClass;
	int AutoIndexInArray;
	_conn<ControlToClassConnector> ThisDialogIsConnectedTo;
	ArrayOfCallingClassConnector ConnectActionToBaseFunction;	
	bool CallOnAction(int ActionID);
	bool HaveAction(int ActionID);
	BaseTriplet GetDialogConnection();
	SimpleDialog* Parent;
	SimpleDialog* Child;
	VCall* OnClick;
	VCall* OnDraw;
	VCall* OnActivate;
	VCall* OnLeave;
	VCall* OnKeyDown;
	VCall* OnMouseOver;
	VCall* Refresh;
	VCall* Destroy;
	VCall* OnUserClick;
	VCall* OnUserRightClick;
	VCallXY* MouseOverActiveZone;
	VCall* OnDrawActive; 
	VCallXY* OnNewClick;
	VScrollBar* ParentSB;
	DWORD ID;
	short MouseRelX;
	short MouseRelY;
	word ShapeFileID;
	word ShapeSpriteID;	
	int HotKey;
	bool HotCtrl;
	bool HotShift;
	bool HotAlt;
	bool HintWithHotKey;
	int CurClassIndex;//not registered temp field
	//-------------------------
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	SimpleDialog();
	~SimpleDialog();
	virtual void _Draw(){
		if(Refresh)Refresh(this);
		if(Visible&&OnDraw)OnDraw(this);		
	}
	virtual bool _Click(){
		if(OnClick)return OnClick(this);
		else return false;
	}
	virtual void _MouseOver(){
		if(OnMouseOver)OnMouseOver(this);
	}
	virtual void _DrawActive(){
		if(OnDrawActive)OnDrawActive(this);
	}

	virtual void CheckMouseOver(int mx,int my){
		if(LastDrawX<=mx&&LastDrawY<=my&&LastDrawX+x1-x>=mx&&LastDrawY+y1-y>=my&&(ParentFrame::ActiveParent==this||!ParentFrame::ActiveParent)){
			if(ShapeFileID!=0xFFFF){
				DWORD alpha = GPS.GetGPAlpha( ShapeFileID, ShapeSpriteID, 0, mx-LastDrawX, my-LastDrawY, true );
                MouseOver = (alpha > 0x8);
			}else MouseOver=1;
            MouseRelX=mx-x;
			MouseRelY=my-y;
		}else MouseOver=0;
	}
	virtual void Process(SimpleDialog** LeftClick,SimpleDialog** RightClick,SimpleDialog** HintDlg);	
	void AssignSound(int ID,int USAGE);
	void AssignSound(char* Name,int USAGE);
	void AssignID(DWORD V);
	void AssignSID(char* SV);
	//hint savage
	static void* __GetHint(void* CID){
		SimpleDialog* SD=(SimpleDialog*)CID;
		static _str s;
		s.Clear();
		if(SD->Hint){
			char* GetIDByText(char* Text);
			s.Assign(GetIDByText(SD->Hint));
		}return &s;
	}
	static void __SetHint(void* CID, void* data){
		SimpleDialog* SD=(SimpleDialog*)CID;
		_str* s=(_str*)data;
		extern bool v_DialogsMissed;
		v_DialogsMissed=true;
		SD->SetHint(s);
		v_DialogsMissed=false;
	}
	void SetHint(const char* s, bool addHotKey=false){
		_str str;
		str=s;
		SetHint(&str,addHotKey);
	}
	void SetHint(_str* s, bool addHotKey=false);
	
	bool vm_Init;
	int v_ActionID;
	vui_ActionArray v_Actions;
	class NewAction:public BaseFunction{
	public:
		static _str ActionName;
		static _str ActionGroup;
		static bool OnUserLeftClick;
		static bool OnUserRightClick;
		static bool OnDraw;
		static bool SetFrameState;
		SAVE(NewAction);
			REG_PARENT(BaseFunction);
			STATIC REG_AUTO(ActionName);
			STATIC REG_AUTO(ActionGroup);
			STATIC REG_MEMBER(_bool,OnUserLeftClick);
			STATIC REG_MEMBER(_bool,OnUserRightClick);
			STATIC REG_MEMBER(_bool,OnDraw);
			STATIC REG_MEMBER(_bool,SetFrameState);
		ENDSAVE;
		virtual bool ForceProperty(){
			return true;
		}
		void EvaluateFunction();
	};
	NewAction CreateNewAction;
	//
	virtual bool AskParentForUsingExpansionClass(char* MemberName,char* ClassName);
	virtual const char* GetThisElementView(const char* LocalName);
	//
	//_str NameForRef;
	_str* GetObjectNamePointer(){ return &Name; }

	//registration
	//BOOL_PROP(SimpleDialog,Enabled);
	//BOOL_PROP(SimpleDialog,Visible);
	int GetActionID(){
		return v_ActionID;
	}
	void SetActionID(int v){
        //v_ActionID=v;
		if(v){
			///*
			vui_Action* a=dynamic_cast<vui_Action*>((vui_Action*)v);
			if(a&&!v_Actions.isExist(a)){
				vui_Action* anew=(vui_Action*)a->new_element();
				a->Copy(anew);
				v_Actions.Add(anew);
			}
			/*/
			//*
			if(((vui_Action*)v)->LeftClick(this)) OnUserClick=vui_UserLeftClick;
				else OnUserClick=NULL;			
			if(((vui_Action*)v)->RightClick(this)) OnUserRightClick=vui_UserRightClick;
				else OnUserRightClick=NULL;
			//*/
		}
	}	
	INT_PROPERTY(SimpleDialog,GetActionID,SetActionID);

	ClassSyncroniser ConnectingDialogToClassMembers;

	virtual void _OnBeginDrag(){};
	virtual void _OnDrag(){};
	virtual void _OnDropThis(){};
	virtual void _OnDropOnThis(){};

	virtual	void V_scrollToElement(int _id,int type=0){}; // type 0-on center, 1-on top

	SAVE(SimpleDialog)		
		//REG_BOOL_PROP(Visible);		
		//REG_BOOL_PROP(Enabled);
		REG_MEMBER(_bool,Visible);		
		REG_MEMBER(_bool,Enabled);
		REG_MEMBER(_bool,AllowDrag);
		REG_MEMBER(_bool,AllowDropOverThisDialog);
		REG_AUTO(v_Actions);
		NOSAVE REG_AUTO(CreateNewAction);
		REG_INDEXPROP(_index,v_ActionID,GetActionID,SetActionID,ve_Action);
		REG_ENUM(_index,HotKey,ve_HotKey);        
		REG_MEMBER(_bool,HotCtrl);
		REG_MEMBER(_bool,HotShift);
		REG_MEMBER(_bool,HotAlt);
		REG_MEMBER(_int,ID);
		REG_AUTO(Name);				
		REG_PROP(_str,Hint,GetHint,SetHint);
		REG_ENUM(_index,MouseSound,ALL_SOUNDS);
		REG_ENUM(_index,ClickSound,ALL_SOUNDS);
		REG_PARENT(ParentFrame);
		REG_AUTO(ConnectToClass);
		REG_MEMBER(_int,AutoIndexInArray);
		REG_AUTO(ThisDialogIsConnectedTo);
		REG_AUTO(ConnectingDialogToClassMembers);
		REG_AUTO(ConnectActionToBaseFunction);		
	ENDSAVE;
};
#define CLICK_SOUND  0x1234
#define MOUSE_SOUND  0x4321
#define OK_SOUND     0x5421
#define CANCEL_SOUND 0x7513
class DIALOGS_API Video:public SimpleDialog{
public:
	//BinkPlayer* BPlay;
	//VideoLayer* VL;
	void* BPlay;
	void* VL;
	bool IsFinished:1;
	void Play(char* file);
	void Stop();
};
class DIALOGS_API Picture:public SimpleDialog{
public:
	SQPicture* PassivePicture;
	SQPicture* ActivePicture;
	SQPicture* DisabledPicture;
	bool Transparent:1;
	Picture(){};
};
class DIALOGS_API BitPicture:public SimpleDialog{
public:
	SQPicture Pic;
	_str TrueName;
	_picfile FileName;	
	bool ActualSize;
	bool SetFileName(char* File, bool Unload=true);
	//_str FileMask;
	BitPicture(){
		TrueName="";
		FileName.Clear();
		Enabled=true;
	};
	virtual void _Draw();
	/*
	void* GetFileName(){
		//
		return (void*)&PassivePicture->Name.pchar();
	}
	void SetFileName(void* fn){
		str* FN=(str*)
	}
	PROPERTY(TextButton,GetFileName,SetFileName);
	*/
	SAVE(BitPicture){
		REG_LOADSAVE("Dialogs\\","*.BitPicture.Dialogs.xml");		
		//REG_PROP(_str,FileName,GetFileName,SetFileName);
		//REG_BITFILE(FileName,FileMask);
		REG_AUTO(FileName);
		REG_MEMBER(_bool,ActualSize);
		//REG_AUTO(FileMask);
		REG_PARENT(SimpleDialog);
	}ENDSAVE
};
class DIALOGS_API TextureViewer:public SimpleDialog{
public:
	int TextureID;
	float U;
	float V;
	float LU;
	float LV;
	float Uspeed;
	float Vspeed;
	bool  Smooth;
	DWORD ColorLT;
	DWORD ColorRT;
	DWORD ColorLB;
	DWORD ColorRB;
	__constructor(TextureViewer){
		TextureID=-1;
		U=0;
		V=0;
		LU=1;
		LV=1;	
		Uspeed=0;
		Vspeed=0;
		ColorLT=0xFFFFFFFF;
		ColorRT=0xFFFFFFFF;
		ColorLB=0xFFFFFFFF;
		ColorRB=0xFFFFFFFF;
	};
	virtual void _Draw();
	SAVE(TextureViewer){
		REG_LOADSAVE("Dialogs\\","*.TextureViewer.Dialogs.xml");		
		REG_MEMBER(_TextureID,TextureID);
		REG_MEMBER(_bool,Smooth);
		REG_MEMBER(_float,U);
		REG_MEMBER(_float,V);
		REG_MEMBER(_float,LU);
		REG_MEMBER(_float,LV);
		REG_MEMBER(_float,Uspeed);
		REG_MEMBER(_float,Vspeed);
		REG_MEMBER(_color,ColorLT);
		REG_MEMBER(_color,ColorRT);
		REG_MEMBER(_color,ColorLB);
		REG_MEMBER(_color,ColorRB);
		REG_PARENT(SimpleDialog);
	}ENDSAVE
};
class DIALOGS_API BaseMeshDialog:public SimpleDialog{
public:
	BaseMesh* BM;
	int PrevX;
	int PrevY;
	void CreateSquareTnL2(float uL,float vL,float uR,float vR,float uL2,float vL2,float uR2,float vR2,char* Shader,DWORD Diffuse);
	void CreateSquareTnL(float uL,float vL,float uR,float vR,char* Shader,DWORD Diffuse);
};
class DIALOGS_API RLCPicture:public SimpleDialog{
public:
	RLCTable* PassivePicture;
	byte ppic;
	RLCTable* ActivePicture;
	byte apic;
	RLCTable* DisabledPicture;
	byte dpic;
};
class DIALOGS_API GPPicture:public SimpleDialog{
public:
	int dx,dy;
	short FileID;
	int SpriteID;
	byte Nation;
	bool Rotated;
	float Angle;
	//float ScaleX;
	//float ScaleY;
	int rx,ry;
	void SetRotScale(int rx,int ry,float a,float sx,float sy);
	GPPicture();
	void* GetFileID(){
		return &FileID;
	}
	void SetFileID(void* v){
		FileID=*((short*)v);
		x1=x+GPS.GetGPWidth(FileID,SpriteID)-1;
		y1=y+GPS.GetGPHeight(FileID,SpriteID)-1;
		if(x1<x)x1=x;
		if(y1<y)y1=y;
		if(x1>x+2000)x1=x+2000;
		if(y1>y+2000)y1=y+2000;
		ShapeFileID=FileID;
		ShapeSpriteID=SpriteID;
	}
	int GetSpriteID(){
		return SpriteID;
	}
	void SetSpriteID(int v){
		SpriteID=v;
		x1=x+GPS.GetGPWidth(FileID,SpriteID)-1;
		y1=y+GPS.GetGPHeight(FileID,SpriteID)-1;
		if(x1>x+2000)x1=x+2000;
		if(y1>y+2000)y1=y+2000;
		ShapeFileID=FileID;
		ShapeSpriteID=SpriteID;
	}
	PROPERTY(GPPicture,GetFileID,SetFileID);
	INT_PROPERTY(GPPicture,GetSpriteID,SetSpriteID);
	SAVE(GPPicture){
		
		REG_LOADSAVE("Dialogs\\","*.GPPicture.Dialogs.xml");

		REG_MEMBER(_int,dx);
		REG_MEMBER(_int,dy);
		
		REG_PROP(_gpfile,FileID,GetFileID,SetFileID);
		///REG_INTPROP(_int,SpriteID,GetSpriteID,SetSpriteID);
		REG_SPRITE(SpriteID,FileID);
		REG_MEMBER_EX(_BYTE,Nation,NationalColor);
		//REG_MEMBER_EX(_bool,Rotated,AllowRotation);
		//REG_MEMBER_EX(_float,Angle,AngleOfRotation);
		//REG_MEMBER(_float,ScaleX);
		//REG_MEMBER(_float,ScaleY);
		//REG_MEMBER_EX(_int,rx,CenterOfRotationX);
		//REG_MEMBER_EX(_int,ry,CenterOfRotationY);
		REG_PARENT(SimpleDialog);

	}ENDSAVE;
};
DIALOGS_API void DrawFilledRect3(int x0,int y0,int x1,int y1,Corners* CR,int GP_File,int StartFill,int NFill);
void DrawRect4(int x0,int y0,int x1,int y1,Corners* CR,int GP_File);
class TexFrame:public BaseClass{
public:
    int x,y,w,h,xc,yc;
    SAVE(TexFrame){
        REG_MEMBER(_int,x);
        REG_MEMBER(_int,y);
        REG_MEMBER(_int,w);
        REG_MEMBER(_int,h);
        REG_MEMBER(_int,xc);
        REG_MEMBER(_int,yc);
    }ENDSAVE;
};
class StdBorder:public BaseClass{
public:
	//int NameIndex;
	_str Name;
    bool WindowsStyle;
    bool UsesTexture;
	word GP_File;
	Corners Edges;
	int ClipDL;
	int ClipDR;
	int ClipDU;
	int ClipDD;
	int NFillers;
	int StartFiller;
	word VScroller_GP_File;
	int VScroller_DX_right;
	int VScroller_DY_top;
	int VScrolled_DY_bottom;

    int Texture;
    TexFrame LT;
    TexFrame RT;
    TexFrame LB;
    TexFrame RB;
    TexFrame Left;
    TexFrame Right;
    TexFrame Top;
    TexFrame Bottom; 
    TexFrame TexFiller;

	Delimiter D;

	int FillerTopMargin;
	int FillerBottomMargin;
	int FillerLeftMargin;
	int FillerRightMargin;

	int TopMarginParam;
	int BottomMarginParam;
	int LeftMarginParam;
	int RightMarginParam;
	//
	StdBorder(){
		GP_File=0;
		NFillers=0;
		memset(&Edges,0,sizeof Edges);
		ClipDL=0;
		ClipDR=0;
		ClipDU=0;
		ClipDD=0;
	}
	virtual void Draw(int x,int y,int x1,int y1);
	const char* GetThisElementView(const char* LocalName){	
		return Name.pchar()?Name.pchar():LocalName;
	}	
	void TestBorder();
	void AutoCreateXCYC_byOuterMargin();
	void AutoCreateXCYC_byInnerMargin();
	SAVE(StdBorder){
		//REG_ENUM_EX(_accamulator,NameIndex,BORDERS,Name);
		REG_AUTO(Name);        
        REG_MEMBER(_bool,UsesTexture);

        SAVE_SECTION(3);

        REG_MEMBER(_bool,WindowsStyle);

        SAVE_SECTION(1);

		REG_MEMBER(_gpfile,GP_File);
		REG_SPRITE_EX(Edges.CLU,LeftTop,GP_File);
		REG_SPRITE_EX(Edges.CRU,RightTop,GP_File);
		REG_SPRITE_EX(Edges.CLD,LeftBottom,GP_File);
		REG_SPRITE_EX(Edges.CRD,RightBottom,GP_File);
		REG_SPRITE_EX(Edges.LU,TopLine,GP_File);
		REG_SPRITE_EX(Edges.LD,BottomLine,GP_File);
		REG_SPRITE_EX(Edges.LL,LeftLine,GP_File);
		REG_SPRITE_EX(Edges.LR,RightLine,GP_File);		

        SAVE_SECTION(4);

        REG_MEMBER(_TextureID,Texture);
        REG_AUTO(LT);
        REG_AUTO(RT);
        REG_AUTO(LB);
        REG_AUTO(RB);
        REG_AUTO(Left);
        REG_AUTO(Right);
        REG_AUTO(Top);
        REG_AUTO(Bottom);
        REG_AUTO(TexFiller);

		REG_AUTO(D);

		REG_MEMBER(_int,FillerTopMargin);
		REG_MEMBER(_int,FillerBottomMargin);
		REG_MEMBER(_int,FillerLeftMargin);
		REG_MEMBER(_int,FillerRightMargin);

		REG_METHOD(&StdBorder::TestBorder);
		REG_METHOD(&StdBorder::AutoCreateXCYC_byOuterMargin);
		REG_METHOD(&StdBorder::AutoCreateXCYC_byInnerMargin);

		REG_MEMBER(_int,TopMarginParam);
		REG_MEMBER(_int,BottomMarginParam);
		REG_MEMBER(_int,LeftMarginParam);
		REG_MEMBER(_int,RightMarginParam);		
		REG_AUTO(D);

        SAVE_SECTION(7);

		REG_MEMBER_EX(_int,ClipDL,LeftMargin);
		REG_MEMBER_EX(_int,ClipDR,RightMargin);
		REG_MEMBER_EX(_int,ClipDU,TopMargin);
		REG_MEMBER_EX(_int,ClipDD,BottomMargin);

        SAVE_SECTION(1);

		REG_MEMBER(_int,NFillers);
		REG_SPRITE(StartFiller,GP_File);

        SAVE_SECTION(7);

		REG_MEMBER(_gpfile,VScroller_GP_File);
		REG_MEMBER(_int,VScroller_DX_right);
		REG_MEMBER(_int,VScroller_DY_top);
		REG_MEMBER(_int,VScrolled_DY_bottom);

	}ENDSAVE;
    DWORD GetClassMask(){
        if(UsesTexture)return 4;
        return WindowsStyle?2:1;
    }
};

class BordersScope: public BaseClass {
	//DynArray<int> IndexTable;
public:
	ClonesArray<StdBorder> Borders;
	StdBorder* Get(int i);
	StdBorder* Get(const char* s){
		for(int i=0;i<Borders.GetAmount();i++){
			char* s0=Borders[i]->Name.pchar();
			if(s0&&!strcmp(s0,s))return Borders[i];
		}
		return NULL;
	}
	SAVE(BordersScope)
	REG_AUTO(Borders);
    REG_CLASS(StdBorder);
	ENDSAVE;
};

extern BordersScope BScope;
class BordersEnumerator: public ProcEnumerator{
public:
	virtual DWORD GetValue(const char* ID){
		for(int i=0;i<BScope.Borders.GetAmount();i++){
			char* s=BScope.Borders[i]->Name.pchar();
			if(s&&!strcmp(s,ID))return i;
		}
		return 0xFFFFFFFF;
	}	
	virtual char* GetValue(DWORD ID){
		if(ID<BScope.Borders.GetAmount()){
			return BScope.Borders[ID]->Name.pchar();
		}
		return "";
	}
	virtual int   GetAmount(){
		return BScope.Borders.GetAmount();
	}
	virtual char* GetIndexedString(int idx){
		return GetValue(idx);
	}
	virtual DWORD GetIndexedValue (int idx){
		return idx;
    }	
};
class ClassEditor;
class ClassEditorConnector:public ControlToClassConnector{
public:
    ClassEditorConnector();
    ~ClassEditorConnector();
    ClassEditor* CE;
    void* PrevDataPtr;
    SAVE(ClassEditorConnector);
        REG_PARENT(ControlToClassConnector);
    ENDSAVE;    
};
class DialogsDesk:public SimpleDialog{
public:	
	~DialogsDesk();
	int BorderIndex;
	bool EnableHScroll;
	bool EnableVScroll;
	bool EnableMouseShift;
	bool AutoSetWidth;
	bool AutoSetHeight;
    int  AddWidth;
    int  AddHeight;
	int  MaxWidth;
	int  MaxHeight;
	int  MinWidth;
	int  MinHeight;
	int XShift;
	int YShift;
	VScrollBar* VScroller;
	VScrollBar* HScroller;
	bool HideVScroller;
    _conn<ClassEditorConnector> UseClassEditor;
	//
	ClassPtr<SimpleDialog> Element;
	bool AddElement(char* message);
	//
	virtual void GetMargin(int&left,int&top,int&right,int&bottom){
		StdBorder* SB=BScope.Get(BorderIndex);
		if(SB){
			left  = SB->ClipDL;
			right = SB->ClipDR;
			top   = SB->ClipDU;
			bottom= SB->ClipDD;
		}else{
			left  = 0;
			right = 0;
			top   = 0;
			bottom= 0;
		}
	}
	virtual void GetShift(int&x,int&y){
		x=XShift;
		y=YShift;
	}
	virtual void _Draw();
	virtual void _MouseOver();
	virtual bool _Click();
	virtual void Process(SimpleDialog** LeftClick,SimpleDialog** RightClick,SimpleDialog** HintDlg);
	virtual bool ShiftDialog(int dx,int dy);
	virtual bool ResizeDialog(int dx,int dy);
	SAVE(DialogsDesk)
		REG_LOADSAVE("Dialogs\\","*.DialogsDesk.Dialogs.xml");		
        REG_AUTO(UseClassEditor);
		REG_ENUM_EX(_index,BorderIndex,BORDERS,Border);
		REG_MEMBER_EX(_bool,EnableHScroll,EnableHorizontalScroller);
		REG_MEMBER_EX(_bool,EnableVScroll,EnableVerticalScroller);
		REG_MEMBER(_bool,HideVScroller);
		REG_MEMBER_EX(_bool,EnableMouseShift,EnableMouseShift);
		REG_MEMBER(_bool,AutoSetWidth);
        REG_MEMBER(_int,AddWidth);
		REG_MEMBER(_bool,AutoSetHeight);
        REG_MEMBER(_int,AddHeight);
		REG_MEMBER(_int,MaxWidth);
		REG_MEMBER(_int,MaxHeight);
		REG_MEMBER(_int,MinWidth);
		REG_MEMBER(_int,MinHeight);
		REG_MEMBER(_int,XShift);
		REG_MEMBER(_int,YShift);	
		REG_PARENT(SimpleDialog);
        REG_CLASS(ClassEditorConnector);
	ENDSAVE;
};
class DIALOGS_API TextMessage{
	char* Message;
	RLCFont* Font;
	byte Align;
};
class DIALOGS_API TextButton:public SimpleDialog{
public:	
	bool ColorFonts;
	DWORD cActive;
	DWORD cPassive;
	DWORD cDisabled;
	//
	RLCFont* ActiveFont;
	RLCFont* PassiveFont;
	RLCFont* DisabledFont;
	//
	char* Message;	
	int MaxWidth;
	int	xc;
	int yc;
	byte Align;	// left, center, right
	byte Vertical;
	
	//virtual int GetWidth(){ return MaxWidth; }
	//virtual void SetWidth(int w){ ParentFrame::SetWidth(w); MaxWidth=w; }
	//virtual void SetHeight(int h){y1=y+h-1;if(y1<y)y1=y;}

	void* GetMessage(){
        return (void*)&Message;
	}
	void SetMessage(void* M){
        char* ms=*((char**)M);
		SetMessage(ms);		
	}
	void SetMessage(char* ms, int Width=0);
	TextButton();
	PROPERTY(TextButton,GetMessage,SetMessage);	
	SAVE(TextButton)
		REG_LOADSAVE("Dialogs\\","*.TextButton.Dialogs.xml");		
		REG_PROP(_textid,Message,GetMessage,SetMessage);			
		REG_MEMBER(_bool,ColorFonts);
		REG_MEMBER(_color,cActive);
		REG_MEMBER(_color,cPassive);
		REG_MEMBER(_color,cDisabled);
		REG_MEMBER(_font,ActiveFont);
		REG_MEMBER(_font,PassiveFont);
		REG_MEMBER(_font,DisabledFont);
		REG_ENUM(_byte_index,Align,TEXTALIGN);		
		REG_MEMBER(_int,MaxWidth);
		REG_MEMBER(_bool,Vertical);
		REG_PARENT(SimpleDialog);		
	ENDSAVE
	virtual const char* GetThisElementView(const char* LocalName);
};
class DIALOGS_API ProgressBar:public SimpleDialog{
public:
	ProgressBar();
	virtual void _Draw();

	int   MaxValue;
	int   Value;	

	_str  Message;	

	int MessageDx;
	int MessageDy;
	RLCFont* Font;

	int Nation;

	int Type;

	int GaugeType;

	short BackGP_File;
	int   BackSprite;
	int   BackDx;
	int   BackDy;
	DWORD BackColor;

	
	short ProgressGP_File;
	int   ProgressSprite;
	int   ProgressDx;
	int   ProgressDy;
	DWORD ProgressColor;

	int   ProgressType;

	int   ProgerssMarginLU;
	int   ProgerssMarginRD;
	
	float Scale;
	
	SAVE(ProgressBar){
		REG_LOADSAVE("Dialogs\\","*.ProgressBar.xml");
		SAVE_SECTION(1);
		REG_MEMBER(_int,Value);
		REG_MEMBER(_int,MaxValue);
		REG_AUTO(Message);
		REG_MEMBER(_font,Font);
		REG_MEMBER(_int,MessageDx);
		REG_MEMBER(_int,MessageDy);

		REG_MEMBER(_int,Nation);
		REG_ENUM(_index,ProgressType,ProgressBarDirection);
		REG_ENUM(_index,Type,ProgressBarType);

		SAVE_SECTION(2);
		REG_MEMBER(_gpfile,BackGP_File);
		REG_MEMBER(_int,BackSprite);
		REG_MEMBER(_int,BackDx);
		REG_MEMBER(_int,BackDy);
		REG_MEMBER(_DWORD,BackColor);
		REG_MEMBER(_gpfile,ProgressGP_File);
		REG_MEMBER(_int,ProgressSprite);
		REG_MEMBER(_int,ProgressDx);
		REG_MEMBER(_int,ProgressDy);
		REG_MEMBER(_DWORD,ProgressColor);
		
		SAVE_SECTION(4);
		REG_ENUM(_index,GaugeType,GaugeType);
		
		SAVE_SECTION(1);		
		REG_MEMBER(_int,ProgerssMarginLU);
		REG_MEMBER(_int,ProgerssMarginRD);
		REG_MEMBER(_float,Scale);
		REG_PARENT(SimpleDialog);		
	}ENDSAVE;

	DWORD GetClassMask(){
        DWORD M=1;
		if(Type==0)
			M|=2;
		else
			M|=4;
		return M;
	}

};
class DIALOGS_API GP_TextButton:public SimpleDialog{
public:
	GP_TextButton();
	char* Message;	
	RLCFont* ActiveFont;
	RLCFont* PassiveFont;
	RLCFont* DisabledFont;
	int FontDy;
	int FontDx;
	bool Center;
	short GP_File;
	int Sprite;
	int Sprite1;
	int Nx;
	int OneLx;
	byte Align;
	void* GetMessage(){
		return (void*)&Message;
	}
	void SetMessage(void* M){
        char* ms=*((char**)M);
		SetMessage(ms);		
	}
	void SetMessage(char* ms){
		if(!Message||strlen(ms)!=strlen(Message)) Message=(char*)realloc(Message,strlen(ms)+1);
		strcpy(Message,ms);				
	}
	PROPERTY(GP_TextButton,GetMessage,SetMessage);
	void* GetFileID(){
		return &GP_File;
	}
	void SetFileID(void* v){
		GP_File=*((short*)v);
		x1=x+GPS.GetGPWidth(GP_File,Sprite);
		y1=y+GPS.GetGPHeight(GP_File,Sprite);
		if(x1<x)x1=x;
		if(y1<y)y1=y;
		if(x1>x+2000)x1=x+2000;
		if(y1>y+2000)y1=y+2000;
		ShapeFileID=GP_File;
		ShapeSpriteID=Sprite;
	}
	/*
	int GetSprite(){
		return Sprite;
	}
	void SetSprite(int v){
		Sprite=v;
		if(v>=0){
			x1=x+GPS.GetGPWidth(GP_File,Sprite);
			y1=y+GPS.GetGPHeight(GP_File,Sprite);
			if(x1>x+2000)x1=x+2000;
			if(y1>y+2000)y1=y+2000;
			ShapeFileID=GP_File;
			ShapeSpriteID=Sprite;
		}
	}
	int GetSprite1(){
		return Sprite1;
	}
	void SetSprite1(int v){
		Sprite1=v;
		if(v>=0){
			x1=x+GPS.GetGPWidth(GP_File,Sprite1);
			y1=y+GPS.GetGPHeight(GP_File,Sprite1);
			if(x1>x+2000)x1=x+2000;
			if(y1>y+2000)y1=y+2000;
			ShapeFileID=GP_File;
			ShapeSpriteID=Sprite1;
		}
	}
	*/
	PROPERTY(GP_TextButton,GetFileID,SetFileID);
		//INT_PROPERTY(GP_TextButton,GetSprite,SetSprite);
		//INT_PROPERTY(GP_TextButton,GetSprite1,SetSprite1);
		SAVE(GP_TextButton)		
		REG_LOADSAVE("Dialogs\\","*.GP_TextButton.Dialogs.xml");
		REG_MEMBER(_font,ActiveFont);
		REG_MEMBER(_font,PassiveFont);	
		REG_MEMBER(_font,DisabledFont);	
		REG_PROP(_textid,Message,GetMessage,SetMessage);		
		REG_PROP(_gpfile,FileID,GetFileID,SetFileID);		
		//REG_INTPROP(_int,Sprite1,GetSprite1,SetSprite1);
		REG_SPRITE(Sprite,GP_File);
		REG_SPRITE(Sprite1,GP_File);
		REG_MEMBER(_bool,Center);
		REG_MEMBER(_int,FontDx);
		REG_MEMBER(_int,FontDy);
		REG_PARENT(SimpleDialog);		
	ENDSAVE;
	const char* GetThisElementView(const char* LocalName);
};
class DIALOGS_API SceneElement:public BaseClass{
public:
    ClassSyncroniser ConnectElementToClass;
    SAVE(SceneElement);
        REG_AUTO(ConnectElementToClass);
    ENDSAVE;
    virtual void Render(const Matrix4D& M){};
};
class DIALOGS_API SceneCamera:public BaseClass{
public:
    //camera params
    Delimiter   D;
    Vector3D    ViewPosition;
    float       DistanceToEye;
    float       Azimuth;
    float       Elevation;
    float       Roll;    
    float       FOV;
    float       zNear;
    float       zFar;

    SAVE(SceneCamera);        
        REG_MEMBER(_float,ViewPosition.x);
        REG_MEMBER(_float,ViewPosition.y);
        REG_MEMBER(_float,ViewPosition.z);
        REG_AUTO(D);
        REG_MEMBER(_float,DistanceToEye);
        REG_MEMBER(_float,Azimuth);
        REG_MEMBER(_float,Elevation);
        REG_MEMBER(_float,Roll);    
        REG_AUTO(D);
        REG_MEMBER(_float,FOV);
        REG_MEMBER(_float,zNear);
        REG_MEMBER(_float,zFar);
        //REG_AUTO(D);
        //REG_PARENT(SceneElement);
    ENDSAVE;

    __constructor(SceneCamera){
        ViewPosition=Vector3D(0,0,0);
        DistanceToEye=1000;
        Azimuth=0;
        Elevation=30;
        Roll=0;
        FOV=60;
        zNear=100;
        zFar=2000;
    }
    virtual void Render(Rct& Rect);
};
class DIALOGS_API ModelInScene:public SceneElement{
public:
    Delimiter D;
    DWORD Model;
    DWORD Animation;
    BaseMesh* ExtraMesh;
    Vector3D Position;
    float Scale;
    float PeriodScale;
    float CurrentTimePosition;
    bool  PlayAnimation;
    bool  Rotate;
    float RotationSpeed;
    float CurrentAngle;
    int PrevTimePosition;
    Vector3D RotationAxe;
    Vector3D RotationPosition;
    ClassArray<SceneElement> ChildElements;

    SAVE(ModelInScene);        
        REG_AUTO(ChildElements);
        REG_MEMBER(_ModelID,Model);
        REG_MEMBER(_ModelID,Animation);        
        REG_AUTO(D);
        REG_MEMBER(_float,Scale);
        REG_MEMBER(_float,Position.x);
        REG_MEMBER(_float,Position.y);
        REG_MEMBER(_float,Position.z);
        REG_AUTO(D);
        REG_MEMBER(_float,PeriodScale);
        REG_MEMBER(_float,CurrentTimePosition);
        REG_MEMBER(_bool,PlayAnimation);
        REG_AUTO(D);
        REG_MEMBER(_bool,Rotate);

        SAVE_SECTION(2);

        REG_MEMBER(_float,CurrentAngle);
        REG_MEMBER(_float,RotationSpeed);
        REG_AUTO(D);
        REG_MEMBER(_float,RotationAxe.x);
        REG_MEMBER(_float,RotationAxe.y);
        REG_MEMBER(_float,RotationAxe.z);
        REG_AUTO(D);
        REG_MEMBER(_float,RotationPosition.x);
        REG_MEMBER(_float,RotationPosition.y);
        REG_MEMBER(_float,RotationPosition.z);

        SAVE_SECTION(0xFFFFFFFF);

        REG_AUTO(D);
        REG_PARENT(SceneElement)
    ENDSAVE;
    DWORD GetClassMask(){
        return Rotate?3:1;
    }
    ~ModelInScene(){
        if(ExtraMesh)delete(ExtraMesh);
        ExtraMesh=NULL;
    }
    __constructor(ModelInScene){                
        Scale = 1;
        PeriodScale = 1;
        RotationAxe = Vector3D(0,0,1);
        ExtraMesh = NULL;
    }
    virtual void Render(const Matrix4D& M);
};

class DIALOGS_API Scene3D:public SimpleDialog{
public:	
    AllPtr<SceneCamera> Camera;
    ClassArray<SceneElement> Scene;
    bool RenderEffects;
	int PrevRenderTime;
	virtual void _Draw();
	SAVE(Scene3D){		
		REG_LOADSAVE("Dialogs\\","*.Scene3D.xml");
        REG_AUTO(Camera);
        REG_AUTO(Scene);
        REG_MEMBER(_bool,RenderEffects);
		REG_PARENT(SimpleDialog);
	}ENDSAVE;
    __constructor(Scene3D){
        RenderEffects=true;
		PrevRenderTime=0;
    }
};
class DIALOGS_API VitButton:public SimpleDialog{
public:	
	short GP_File;
	int State;	
	bool OneSprited;
	bool DisableCycling;
	int SpritePassive[4];
	int SpriteOver[4];
	int SpriteDx[4];
	int Lx;	

	_str DialogDesk;

	byte Align;	// left, center, right
	char* Message;		
	RLCFont* FontPassive;
	RLCFont* FontOver;
	int FontDx, FontDy;

	int GetTextWidth();

	byte Nation;

	inline int GetHeightGP();	
	void SetMessage(char* m);
	const char* GetThisElementView(const char* LocalName);
	VitButton();
	virtual void _Draw();
	~VitButton(){
		if(Message)delete(Message);
		Message=NULL;
	}

	SubSection Text;
	SubSection Sprite;
	SubSection SimplePapa;

	SAVE(VitButton){		
		REG_LOADSAVE("Dialogs\\","*.VitButton.xml");
		REG_MEMBER(_int,State);
		//REG_MEMBER(_int,Lx);
        REG_AUTO(Text);
		REG_MEMBER(_textid,Message);		
		REG_MEMBER(_font,FontPassive);
		REG_MEMBER(_font,FontOver);
		REG_MEMBER(_int,FontDx);
		REG_MEMBER(_int,FontDy);		
		REG_ENUM(_byte_index,Align,TEXTALIGN);		
		REG_AUTO(Sprite);
		REG_MEMBER(_bool,OneSprited);
		REG_MEMBER(_bool,DisableCycling);		
		REG_MEMBER(_gpfile,GP_File);
		//REG_MEMBER_EX(_int,SpritePassive[0],SpritePassive0);
		//REG_MEMBER_EX(_int,SpriteOver[0],SpriteOver0);
		REG_SPRITE_EX(SpritePassive[0],SpritePassive0,GP_File);
		REG_SPRITE_EX(SpriteOver[0],SpriteOver0,GP_File);
		REG_MEMBER_EX(_int,SpriteDx[0],SpriteDx0);
		//REG_MEMBER_EX(_int,SpritePassive[1],SpritePassive1);
		//REG_MEMBER_EX(_int,SpriteOver[1],SpriteOver1);
		REG_SPRITE_EX(SpritePassive[1],SpritePassive1,GP_File);
		REG_SPRITE_EX(SpriteOver[1],SpriteOver1,GP_File);
		REG_MEMBER_EX(_int,SpriteDx[1],SpriteDx1);
		REG_SPRITE_EX(SpritePassive[2],SpritePassive2,GP_File);
		REG_SPRITE_EX(SpriteOver[2],SpriteOver2,GP_File);
		REG_MEMBER_EX(_int,SpriteDx[2],SpriteDx2);
		REG_MEMBER(_str,DialogDesk);
        REG_AUTO(SimplePapa);
		REG_PARENT(SimpleDialog);		
	}ENDSAVE;

	//int SpritePassive[4];	
	//byte Nation;
	//
	//int dx,dy;
	//bool Rotated;	
	//float Angle;
	//float ScaleX;
	//float ScaleY;
	//int rx,ry;
};
class DIALOGS_API TabButton: public VitButton {
public:
	_str Group;
	//virtual void _Draw();
	virtual bool _Click();	
	SAVE(TabButton){
		REG_LOADSAVE("Dialogs\\","*.TabButton.xml");
		REG_MEMBER(_str,Group);
		REG_PARENT(VitButton);
		//REG_MEMBER(_str,DialogDesk);
		//// VitButton
		//REG_MEMBER(_gpfile,GP_File);
		//REG_MEMBER(_int,State);
		//REG_MEMBER(_bool,OneSprited);
		////REG_MEMBER(_int,Lx);
		//REG_MEMBER(_textid,Message);
		//REG_MEMBER(_font,FontPassive);
		//REG_MEMBER(_font,FontOver);
		//REG_MEMBER(_int,FontDx);
		//REG_MEMBER(_int,FontDy);
		//REG_MEMBER_EX(_int,SpritePassive[0],SpritePassive0);
		//REG_MEMBER_EX(_int,SpriteOver[0],SpriteOver0);
		//REG_MEMBER_EX(_int,SpriteDx[0],SpriteDx0);
		//REG_MEMBER_EX(_int,SpritePassive[1],SpritePassive1);
		//REG_MEMBER_EX(_int,SpriteOver[1],SpriteOver1);
		//REG_MEMBER_EX(_int,SpriteDx[1],SpriteDx1);
		//REG_PARENT(SimpleDialog);		
	}ENDSAVE;
};
class DIALOGS_API TabDesk:public DialogsDesk{
public:
	_str ParentDialogID;
	//virtual void _Draw();	
	SAVE(TabDesk)
		REG_LOADSAVE("Dialogs\\","*.TabDesk.xml");		
		// DialogsDesk
		REG_ENUM_EX(_index,BorderIndex,BORDERS,Border);
		REG_MEMBER_EX(_bool,EnableHScroll,EnableHorizontalScroller);
		REG_MEMBER_EX(_bool,EnableVScroll,EnableVerticalScroller);
		REG_MEMBER(_bool,HideVScroller);
		REG_MEMBER_EX(_bool,EnableMouseShift,EnableMouseShift);
		REG_MEMBER(_int,XShift);
		REG_MEMBER(_int,YShift);
		//
		REG_MEMBER(_str,ParentDialogID);		
		REG_PARENT(SimpleDialog);		
	ENDSAVE;
};
class SimpleDialogPtr: public ClassPtr<SimpleDialog>{
public:
	bool ForceProperty() { return false; }
};
class DIALOGS_API ListDesk:public DialogsDesk{
private:
	int FreeID;
public:
	ListDesk();	

	class NewElement:public BaseFunction{
	public:
		static _str Message;
		//static _str ActionGroup;
		//static bool OnUserLeftClick;
		//static bool OnUserRightClick;
		//static bool OnDraw;
		//static bool SetFrameState;
		SAVE(NewElement);
			REG_PARENT(BaseFunction);
			STATIC REG_AUTO(Message);
			//STATIC REG_AUTO(ActionGroup);
			//STATIC REG_MEMBER(_bool,OnUserLeftClick);
			//STATIC REG_MEMBER(_bool,OnUserRightClick);
			//STATIC REG_MEMBER(_bool,OnDraw);
			//STATIC REG_MEMBER(_bool,SetFrameState);
		ENDSAVE;
		virtual bool ForceProperty(){
			return true;
		}
		void EvaluateFunction();
	};
	NewElement CreateNewElement;

	int CurrentElement;
	int PreviewAmount;
	int startX;
	int startY;
	int marginX;
	int marginY;
	SimpleDialogPtr Element;
	//int ElementLx;
	//int ElementLy;

	int AlignType;
	int AmountInLine;
	_conn<ArrayConnector> Connect_to_ClonesArray;

	SAVE(ListDesk){
		REG_LOADSAVE("Dialogs\\","*.ListDesk.xml");		

		// DialogsDesk

		REG_AUTO(CreateNewElement);

		REG_ENUM_EX(_index,BorderIndex,BORDERS,Border);
		REG_MEMBER_EX(_bool,EnableHScroll,EnableHorizontalScroller);
		REG_MEMBER_EX(_bool,EnableVScroll,EnableVerticalScroller);
		REG_MEMBER(_bool,HideVScroller);
		REG_MEMBER_EX(_bool,EnableMouseShift,EnableMouseShift);

		REG_ENUM(_index,AlignType,ListAlign);
		REG_MEMBER(_int,AmountInLine);
		
		REG_MEMBER(_int,CurrentElement);
		REG_MEMBER(_int,PreviewAmount);
		REG_MEMBER(_int,marginX);
		REG_MEMBER(_int,marginY);

		REG_AUTO(Element);
		
		REG_MEMBER(_bool,AutoSetWidth);
        REG_MEMBER(_int,AddWidth);
		REG_MEMBER(_bool,AutoSetHeight);
        REG_MEMBER(_int,AddHeight);
		REG_MEMBER(_int,MaxWidth);
		REG_MEMBER(_int,MaxHeight);
		REG_MEMBER(_int,MinWidth);
		REG_MEMBER(_int,MinHeight);

		REG_MEMBER(_int,startX);
		REG_MEMBER(_int,startY);
		REG_MEMBER(_int,XShift);
		REG_MEMBER(_int,YShift);

		REG_PARENT(SimpleDialog);
		REG_AUTO(Connect_to_ClonesArray);

	}ENDSAVE;

	void UpdateFreeID();
	SimpleDialog* GetElementByUID(int UID);
	bool DelElementByUID(int UID);

	inline bool AddElement(_str& message);
	int AddElement(char* message, int HotKey=0);
	int AddElement(int Pos, char* message, int HotKey=0);
	char* GetElement(int ID);
	int GetElement(char* message);

	virtual void _Draw();
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	inline virtual int GetWidth();

};
class SimpleDialogForClass: public BaseClass{
public:
	_str ClassName;
	ClassPtr<SimpleDialog> Dialog;
	SAVE(SimpleDialogForClass){
        REG_AUTO(ClassName);
		REG_AUTO(Dialog);
	}ENDSAVE;
};
//this function is required if you want to handle dropping.
//In your class you should declare class derived from BaseDropFunction
//and call DoDrop() if dropping is allowed
class BaseDropFunction:public BaseFunction{
public:
	ClassArray<BaseClass>* SrcArray;
	ClassArray<BaseClass>* DstArray;
	BaseClass* Element;
	int DesiredDstDropIndex;//=-1 if need to add, othervice you should insert element
	int SourceDropIndex;
	void DoDrop();
};
class DIALOGS_API ClassDesk:public DialogsDesk{
private:
	int FreeID;
public:
	ClassDesk();	

	class NewElement:public BaseFunction{
	public:
		static _str Message;
		static _str ClassName;
		SAVE(NewElement);
			REG_PARENT(BaseFunction);
			STATIC REG_AUTO(Message);
			STATIC REG_AUTO(ClassName);
		ENDSAVE;
		virtual bool ForceProperty(){
			return true;
		}
		void EvaluateFunction();
	};

	NewElement CreateNewElement;

	int CurrentElement;
	int marginX;
	int marginY;

	ClonesArray<SimpleDialogForClass> Elements;
	

	int AlignType;
	int AmountInLine;	
	_conn<ArrayConnector> Connect_to_ClassArray;
	_conn<CallingClassConnector> Call_BaseDropFunction_OnDrop;
	
	virtual void _OnDropOnThis();

	SAVE(ClassDesk){
		REG_LOADSAVE("Dialogs\\","*.ClassDesk.xml");		
		REG_CLASS(SimpleDialogForClass);
		// DialogsDesk
		REG_AUTO(CreateNewElement);
		REG_ENUM_EX(_index,BorderIndex,BORDERS,Border);
		REG_MEMBER_EX(_bool,EnableHScroll,EnableHorizontalScroller);
		REG_MEMBER_EX(_bool,EnableVScroll,EnableVerticalScroller);
		REG_MEMBER(_bool,HideVScroller);
		REG_MEMBER_EX(_bool,EnableMouseShift,EnableMouseShift);

		REG_MEMBER(_bool,AutoSetWidth);
		REG_MEMBER(_bool,AutoSetHeight);

		REG_ENUM(_index,AlignType,ListAlign);
		REG_MEMBER(_int,AmountInLine);

		REG_MEMBER(_int,XShift);
		REG_MEMBER(_int,YShift);

		REG_MEMBER(_int,CurrentElement);
		REG_MEMBER(_int,marginX);
		REG_MEMBER(_int,marginY);

		REG_AUTO(Elements);

		REG_PARENT(SimpleDialog);
		REG_AUTO(Connect_to_ClassArray);
		REG_AUTO(Call_BaseDropFunction_OnDrop);
	}ENDSAVE;

	void UpdateFreeID();
	SimpleDialog* GetElementByUID(int UID);
	bool DelElementByUID(int UID);

	inline bool AddElement(_str& message,const char* ClassName="");
	int AddElement(char* message, int HotKey=0,const char* ClassName="");
	int AddElement(int Pos, char* message, int HotKey=0,const char* ClassName="");
	char* GetElement(int ID);
	int GetElement(char* message);

	virtual void _Draw();
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);

	virtual	void V_scrollToElement(int _id,int type=0); // type 0-on center, 1-on top
};
class DIALOGS_API ChatDesk:public DialogsDesk{
public:
	SimpleDialogPtr Element;
	int dY;
	bool AddElement(char* message);
	//char* GetElement(int ID);
	virtual void _Draw();
	virtual void Process(SimpleDialog** LeftClick,SimpleDialog** RightClick,SimpleDialog** HintDlg);
	SAVE(ChatDesk)
		REG_LOADSAVE("Dialogs\\","*.ChatDesk.xml");		
		// DialogsDesk		
		REG_MEMBER(_int,dY);
		REG_ENUM_EX(_index,BorderIndex,BORDERS,Border);
		REG_MEMBER_EX(_bool,EnableHScroll,EnableHorizontalScroller);
		REG_MEMBER_EX(_bool,EnableVScroll,EnableVerticalScroller);
		REG_MEMBER(_bool,HideVScroller);
		REG_MEMBER_EX(_bool,EnableMouseShift,EnableMouseShift);
		REG_MEMBER(_int,XShift);
		REG_MEMBER(_int,YShift);
		//		
		REG_AUTO(Element);
		REG_PARENT(SimpleDialog);
	ENDSAVE;
};
class DIALOGS_API UniversalButton:public TextButton{
public:
	char* Message;
	RLCFont* ActiveFont;
	RLCFont* PassiveFont;
	RLCFont* SelectedFont;
	int  FontDy;
	int  FontDx;
	bool Center;
	int  GP_File;
	int  SpritesSet[30];
	int  Group;
	int  State;
	bool Tiling;
};

class DIALOGS_API VideoButton:public SimpleDialog{
public:
	int GP_on;
	int GP_off;
	int N_on;
	int N_off;
	int CurSprite;
	int Stage;
	int LastTime;
};
void DrawHeaderEx(int x0,int y0,int Lx,int GP_File,int FrameL,int FrameR,int FrameC1,int FrameC2,int FrameC3);
bool GP_ScrollBar_OnDraw(SimpleDialog* SD);
class DIALOGS_API VScrollBar:public SimpleDialog{
public:
	VScrollBar();
	int SMaxPos,SPos;	

	//----------GP_ScrollBar------------//
	short GP_File;
	int ScrollIndex;
	int LineIndex;
	int ScrDx,ScrDy;
	int ScrLx,ScrLy;
	int LineLx;
	int LineLy;
	virtual void _Draw(){
		if(LineLx>LineLy){
			DrawHeaderEx(x,y,LineLx-1,GP_File,ScrollIndex+1,ScrollIndex+2,ScrollIndex+3,ScrollIndex+4,ScrollIndex+5);
			GP_ScrollBar_OnDraw(this);
		}else{
			LineLy=y1-y-2*GPS.GetGPHeight(GP_File,ScrollIndex)+1;
			SimpleDialog::_Draw();
		}
		//GP_ScrollBar_OnDraw((SimpleDialog*)this);
	}
	int GetScrollIndex(){ return ScrollIndex; }
	void SetScrollIndex(int v){
		ScrollIndex=v;
		ScrLx=GPS.GetGPWidth(GP_File,ScrollIndex);
		if(LineLx>LineLy) ScrLy=GPS.GetGPHeight(GP_File,ScrollIndex);
			else ScrLy=GPS.GetGPHeight(GP_File,ScrollIndex+4);
	}
	
	virtual void SetWidth(int w){ w=GPS.GetGPWidth(GP_File,ScrollIndex); ParentFrame::SetWidth(w); }	
	//virtual void SetHeight(int h){ if(h<1)h=1; LineLy=h-2*GPS.GetGPHeight(GP_File,ScrollIndex); ParentFrame::SetHeight(h); }

	INT_PROPERTY(VScrollBar,GetScrollIndex,SetScrollIndex);
	SAVE(VScrollBar)		
		REG_LOADSAVE("Dialogs\\","*.VScrollBar.xml");		
		REG_MEMBER(_gpfile,GP_File);
		REG_INTPROP(_int,ScrollIndex,GetScrollIndex,SetScrollIndex);
		REG_MEMBER(_int,LineLx);
		REG_MEMBER(_int,LineLy);
		REG_MEMBER(_int,SPos);		
		REG_MEMBER(_int,SMaxPos);		
		REG_MEMBER(_int,ScrDx);
		REG_MEMBER(_int,ScrDy);
		REG_PARENT(SimpleDialog);		
	ENDSAVE
	//
	int StartGP_Spr;	
	//----------
	int sbx,sby,sblx,sbly;
	int LastTime;
	byte mkly,btnly;
	bool Zaxvat;
	byte Zaxvat2;//new 31.03
	SQPicture* btn_up0;
	SQPicture* btn_up1;
	SQPicture* btn_up1p;
	SQPicture* btn_dn0;
	SQPicture* btn_dn1;
	SQPicture* btn_dn1p;
	SQPicture* sbar0;
	SQPicture* sbar1;
	SQPicture* marker;
	int OnesDy;
	void SetMaxPos(int i){SMaxPos=i;};
	int GetMaxPos(){return SMaxPos;};
	int GetPos(){return SPos;};
};
class DIALOGS_API HorisontalSlider:public VScrollBar{
public:
	HorisontalSlider();
	SAVE(HorisontalSlider);
		REG_PARENT(VScrollBar);
		REG_BASE(SimpleDialog);
		REG_LOADSAVE("Dialogs\\","*.HorisontalSlider.xml");
	ENDSAVE;
};
class DIALOGS_API BpxTextButton:public TextButton{
public:
	SQPicture* PassivePicture;
	SQPicture* ActivePicture;
	SQPicture* DisabledPicture;
};


#define MaxColumn 16
class DIALOGS_API ComplexBox:public SimpleDialog{
public:
	procDrawBoxElement* DRAWPROC;
	int N;
	int CurrentItem;
	int TopStart;
	int NOnScr;
	int OneLy;
	int GP_Index;
	int StSprite;
	int M_OvrItem;
	int FirstVisItem;
	int param;
	VScrollBar* VS;
};
class DIALOGS_API Canvas:public SimpleDialog{
public:
	Canvas();
	int BottomY;
	int L;
	int MaxL;
	VScrollBar* VS;
	byte* DrawData;
	void AddLine(int x,int y,int x1,int y1,DWORD c);
	void AddSprite(int x,int y,int GPidx,int Sprite);
	void AddBar(int x,int y,int Lx,int Ly,DWORD c);
	void AddRect(int x,int y,int Lx,int Ly,DWORD c);
	void AddText(int x,int y,char* Text,RLCFont* Font);
	void AddCText(int x,int y,char* Text,RLCFont* Font);
	void AddBar4(int x,int y,int Lx,int Ly,DWORD C1,DWORD C2,DWORD C3,DWORD C4);
	void AddThickLines(int* xy_array,int npoints,int thickness,DWORD Color);
	void AddMenuText(int x,int y,char* Text,RLCFont* PFont,RLCFont* AFont,byte align);
	void CheckSize(int sz);
	void Clear();
	SAVE(Canvas)		
		REG_LOADSAVE("Dialogs\\","*.Canvas.xml");
		REG_PARENT(SimpleDialog);
	ENDSAVE
};
class DIALOGS_API CustomBox:public SimpleDialog{
public:
	procDrawBoxElement* DRAWPROC;
	int param;
};
struct ListBoxItem{
	char* Message;
	int Param1;
	byte Flags;
	ListBoxItem* NextItem; 
};
class DIALOGS_API ListBox:public SimpleDialog{
public:
	ListBoxItem* FirstItem;
	ListBoxItem* LastItem;
	SQPicture* ItemPic;
	RLCFont* AFont;
	RLCFont* PFont;
	RLCFont* DFont;
	byte ny;
	byte oneLy;
	int	 oneLx;
	int NItems;
	int CurItem;
	int FLItem;
	int GP_File;
	int Sprite;
	int FontDy;
	int FontDx;
	int M_Over;
	int CurMouseOver;
	VScrollBar* VS;
	ListBoxItem* GetItem(int i);
	void AddItem(char* str,int info);
	void AddStaticItem(char* str,int info);
	void ClearItems();
	void SetFirstPos(int i);
	void SetCurrentItem(int i);
};
class DIALOGS_API RLCListBox:public SimpleDialog{
public:
	//RLCTable Items;
	int GPIndex;
	byte* Choosed;
	int NItems;
	byte BackColor;
	byte SelColor;
	int  XPos;
	int  MaxXpos;
	bool Done;
};
class DIALOGS_API InputBoxOld:public SimpleDialog{
public:
	char* Str;
	_str* _Str;
	_str  DefaultStr;
	_conn<SimpleClassConnector> Connect_edit_string_to_fileld;
	int CursPos;
	int totdx;
	int StrMaxLen;
	RLCFont* Font;
	RLCFont* AFont;
	SQPicture* Pict;
	bool Centering;
	bool Anonim;
	//
	InputBoxOld();
	SAVE(InputBoxOld);		
		REG_LOADSAVE("Dialogs\\","*.InputBoxOld.xml");
		REG_MEMBER(_font,Font);
		REG_MEMBER(_font,AFont);
		REG_MEMBER(_int,StrMaxLen);		
		/*
		REG_MEMBER(_gpfile,GP_File);
		REG_MEMBER(_int,GroupIndex);
		REG_MEMBER(_bool,State);
		REG_MEMBER_EX(_int,Sprite0,SpriteOver);
		REG_MEMBER_EX(_int,Sprite1,SpriteActive);
		REG_MEMBER_EX(_int,Sprite2,SpritePassive);
		REG_MEMBER_EX(_font,Font,FontActive);
		REG_MEMBER_EX(_font,AFont,FontPassive);
		REG_MEMBER(_textid,Message);
		REG_MEMBER(_bool,Central);
		*/
		REG_PARENT(SimpleDialog);
		REG_AUTO(Connect_edit_string_to_fileld);
	ENDSAVE;
};
//////////////////////////////////////////////////////////
//------class InputBox----------------//
class DIALOGS_API InputBox:public SimpleDialog{
public:
	char* Str;
	_str* _Str;
	_str  DefaultStr;
	_conn<SimpleClassConnector> Connect_edit_string_to_fileld;
	int Selected;
	int CursPos;
	int totdx;
	int StrMaxLen;
	RLCFont* Font;
	RLCFont* AFont;
	SQPicture* Pict;
	bool Enter;
	bool Multiline;
	bool Centering;
	bool Anonim;
	//
	InputBox();
	SAVE(InputBox);		
		REG_LOADSAVE("Dialogs\\","*.InputBox.xml");
		REG_MEMBER(_font,Font);
		REG_MEMBER(_font,AFont);
		REG_MEMBER(_int,StrMaxLen);		
		REG_PARENT(SimpleDialog);
		REG_AUTO(Connect_edit_string_to_fileld);
	ENDSAVE;
};
///////////////////////////////////////////////////////////
class DIALOGS_API DialogsSystem;
int GetRLen(char* s,RLCFont* font);
class DIALOGS_API CheckBox:public SimpleDialog{
public:
	ParentFrame* DS;
	//SQPicture* OnPic;
	//SQPicture* OffPic;
	char* Message;	
	bool State;	
	//bool Transparent;
	bool Central;
	int GroupIndex;
	RLCFont* Font;
	RLCFont* AFont;
	//GP
	short GP_File;
	int Sprite0;
	int Sprite1;
	int Sprite2;
	int Sprite3;	
	//
	CheckBox();
	virtual void _Draw(){
		if(!Central)x1=x+GPS.GetGPWidth(GP_File,Sprite1)+GetRLen(Message,AFont);
		y1=y+GPS.GetGPHeight(GP_File,Sprite1);
		OnDraw(this);
	}
	SAVE(CheckBox);		
		REG_LOADSAVE("Dialogs\\","*.CheckBox.xml");
		REG_MEMBER(_gpfile,GP_File);
		REG_MEMBER(_int,GroupIndex);
		REG_MEMBER(_bool,State);		
		REG_MEMBER_EX(_int,Sprite0,SpriteOver);
		REG_MEMBER_EX(_int,Sprite1,SpriteActive);
		REG_MEMBER_EX(_int,Sprite2,SpritePassive);
		REG_MEMBER_EX(_font,Font,FontActive);
		REG_MEMBER_EX(_font,AFont,FontPassive);
		REG_MEMBER(_textid,Message);		
		REG_MEMBER(_bool,Central);
		REG_PARENT(SimpleDialog);		
	ENDSAVE;
};
class BackgroundMap:public SimpleDialog{
    _str MapName;
    _str MainTrackName;
    SAVE(BackgroundMap);
        REG_BASE(SimpleDialog);
        REG_MEMBER(_bool,Visible);
        REG_FILEPATH(MapName,".m3d");
        REG_AUTO(MainTrackName);
		REG_AUTO(ConnectingDialogToClassMembers);
		REG_AUTO(ConnectActionToBaseFunction);		
    ENDSAVE;
    virtual void _Draw();
};

bool ColoredBar_OnDraw(SimpleDialog* SD);
class DIALOGS_API ColoredBar:public SimpleDialog{
public:
	ColoredBar(){ color=0xFF; }
	byte color;
	byte Style;
	virtual void _Draw(){ 
		if(Visible&&OnDraw) OnDraw(this);
		else ColoredBar_OnDraw(this); 
	}
	SAVE(ColoredBar){
		REG_LOADSAVE("Dialogs\\","*.ColoredBar.xml");
		REG_PARENT(SimpleDialog);	
		REG_MEMBER(_BYTE,color);
		REG_MEMBER(_BYTE,Style);
	}ENDSAVE;
};
class DIALOGS_API ChatViewer:public SimpleDialog{
public:
	char*** Mess;
	char*** Names;
	int* NChats;
	int ChatDY;
	int MaxLx;
	int OneLy;
	int ScrNy;
	// new 16.02
	RLCFont* Font1;
	RLCFont* Font2;
};
struct LineInfo{
	bool NeedFormat;
	word LineSize;
	word NSpaces;
	int  Offset;
	int  NextOffset;
	word LineWidth;
};
class DIALOGS_API TextViewer:public SimpleDialog{
public:
	char* TextPtr;
	int TextSize;
	int NLines;
	int Line;
	int PageSize;
	int Lx;
	word SymSize;
	RLCFont* Font;
	void GetNextLine(LineInfo*);
	void CreateLinesList();
	char** LinePtrs;
	word*  LineSize;
	word*  LineWidth;
	bool*  NeedFormat;
	word*  NSpaces;
	VScrollBar* VS;
	void AssignScroll(VScrollBar* SB);
	void LoadFile(char* Name);
};
class DIALOGS_API BPXView:public SimpleDialog{
public:
	byte* Ptr;
	int OneLx;
	int OneLy;
	int Nx,Ny;
	int RealNy;
	int PosY;
	int ChoosedPos;
	int DECLX;
	byte* Choosed;
	bool Done;
	bool EnableSelection;
	VScrollBar* VSC;
	byte Mode;
};
class DIALOGS_API WinComboBox;
class DIALOGS_API WinComboBox:public SimpleDialog{
public:
	RLCFont* ActiveFont;
	RLCFont* PassiveFont;
	int ListX0;
	int ListY0;
	int ListX1;
	int ListY1;
	char** Lines;
	int CurLine;
	int ActiveLine;
	int NLines;
	char* Message;
	WinComboBox** Group;
	int LastChooseTime;
	int NBox;
	int CurBox;
	void AddLine(char* Text);
	void Clear();
};
class DIALOGS_API ComboBox:public SimpleDialog{
public:
	ComboBox();
	~ComboBox();
	RLCFont* ActiveFont;
	RLCFont* PassiveFont;
	int HdrLy;
	int FontDy;
	int FontDx;
	int OneLy;
	int OneLx;
	int OneDx;
	int OneDy;
	int HiLy;
	int LoLy;
	int NLines;
	int DropX;
	int DropY;
	int LightIndex;
	short GP_File;
	int UpPart;
	int Center;
	int DownPart;
	int UpLy;
	byte BackColor;
	byte BorderColor;
	char** Lines;
	int CurLine;
	const char* AutoID;
	const char* GetLineMessage(int idx);
	const char* GetLineHint(int idx);
	//--------new--------
	VScrollBar* VS;
	bool AllocatedVS;
	bool TestMode;
	int  MaxLY;
	int  YPos;
	int  DLX;
	VCall* OnSelect;
	//--------ruler(new!!)------
	bool rulermode;
	int MinDeal;
	//----attach to class management----//
	SubSection ConnectToClass;
	_conn<IndexedClassConnector> ConnectTo_index;
	_conn<CallingClassConnector> CallOnSelect;	
	//----------------------------------//

	void AssignScroll(ParentFrame* DSS,VScrollBar** SCR,int GPFile,int Sprite,int MaxLy);
	void AddLine(char* Text);
	void AddComplexLine(char* Text);
	void CreateRuler(int MinDeal,int NDeals); 
	void Clear();
	virtual bool ShiftDialog(int dx,int dy){
		SimpleDialog::ShiftDialog(dx,dy);
		if(VS)VS->ShiftDialog(dx,dy);
		return true;
	}
	SAVE(ComboBox){
		REG_LOADSAVE("Dialogs\\","*.ComboBox.xml");
		REG_MEMBER(_font,ActiveFont);
		REG_MEMBER(_font,PassiveFont);
		REG_MEMBER(_gpfile,GP_File);
		REG_MEMBER(_int,FontDx);
		REG_MEMBER(_int,FontDy);
		REG_MEMBER(_int,OneDx);
		REG_MEMBER(_int,OneDy);
		REG_MEMBER(_int,Center);
		REG_MEMBER(_int,MaxLY);
		REG_PARENT(SimpleDialog);
		REG_AUTO(ConnectTo_index);
		REG_AUTO(CallOnSelect);
		//REG_AUTO(ConnectToClass);		
	}ENDSAVE;
};
class DIALOGS_API GP_Button:public SimpleDialog{
public:
	GP_Button();
	int GP_File;
	int ActiveFrame;
	int PassiveFrame;	
	void* GetFileID(){
		return &GP_File;
	}
	void SetFileID(void* v){
		GP_File=*((short*)v);
		x1=x+GPS.GetGPWidth(GP_File,PassiveFrame);
		y1=y+GPS.GetGPHeight(GP_File,PassiveFrame);
		if(x1<x)x1=x;
		if(y1<y)y1=y;
		if(x1>x+2000)x1=x+2000;
		if(y1>y+2000)y1=y+2000;
		ShapeFileID=GP_File;
		ShapeSpriteID=PassiveFrame;
	}
	int GetPassiveFrame(){
		return PassiveFrame;
	}
	void SetPassiveFrame(int v){
		PassiveFrame=v;
		x1=x+GPS.GetGPWidth(GP_File,PassiveFrame);
		y1=y+GPS.GetGPHeight(GP_File,PassiveFrame);
		if(x1>x+2000)x1=x+2000;
		if(y1>y+2000)y1=y+2000;
		ShapeFileID=GP_File;
		ShapeSpriteID=PassiveFrame;
	}
	PROPERTY(GP_Button,GetFileID,SetFileID);
	INT_PROPERTY(GP_Button,GetPassiveFrame,SetPassiveFrame);
	SAVE(GP_Button);	
	REG_PROP(_gpfile,GP_File,GetFileID,SetFileID);
	REG_INTPROP(_int,PassiveFrame,GetPassiveFrame,SetPassiveFrame);
	REG_MEMBER(_int,ActiveFrame);
	REG_PARENT(SimpleDialog);	
	ENDSAVE;
};
struct OnePage{
	int x,y,x1,y1;
	int Index;
};
class DIALOGS_API GP_PageControl:public SimpleDialog{
public:
	int GP_File;
	int CurPage;
	OnePage* Pages;
	int NPages;
	void AddPage(int x0,int y0,int x1,int y1,int Index);
};
class DIALOGS_API BorderEx:public SimpleDialog{
public:
	int ymid;
	byte Style;
};
class DIALOGS_API CustomBorder:public SimpleDialog{
public:
	int GP;
	int BOR_N[8];
	int BOR_A[8];
	int FILL_N;
	int FILL_A;
};


int SearchStr(char** Res,char* s,int count);
#define mcmExit		0xFF
#define mcmSingle	0xF1
#define mcmMulti	0xF2
#define mcmVideo	0xF3
#define mcmResume	0xF4
#define mcmOk		0xF5
#define mcmCancel	0xF6
#define mcmLoad		0xF7
#define mcmSave		0xF8
#define mcmHost		0xF9
#define mcmJoin		0xFA
#define mcmRefresh	0xFB
#define mcmEdit		0xFC
#define mcmAll		0xFD
#define mcmGraph	0xE0
#define mcmSound	0xE1
#define mcmSpeed	0xE2
#define mcmSnWeapon 0xE3
#define mcmSnBirth	0xE4
#define mcmSnOrder	0xE5
#define mcmSnAttack 0xE6
#define mcmOptions	0xE7
#define mcmHelp		0xE8

#include "AlexVisualSystem.h"

struct DIALOGS_API GAMEOBJ{
	word Index;
	word Serial;
	int  Type;
	//
	bool CreateZone(int x, int y, int r);
	bool isGroup();
	word FindUnits(word ID, word SN);
};
//////////////////////////////////////////////////////////////////////
class OneTextIcon:public BaseClass{
public:
	_str Name;
	int   IconType;//0 - sprite 1 - texture part 2 - 3d object
	//for 3d object//
	int   ModelID;
	float Scale;
	float RotationSpeed;    
	//for  texture//
	int TextureID;
	int IconX;
	int IconY;
	int IconLx;
	int IconLy;
	int TopMargin;
	int BottomMargin;
	int LeftMargin;
	int RightMargin;
	//for g2d file//
	short gpFile;
	int   Sprite;
	int   dx;
	int   dy;
	int   Lx;
	int   Ly;
	int   ExtraBottomDy;
	SAVE(OneTextIcon);
	REG_AUTO(Name);
	REG_ENUM(_index,IconType,ICONTYPE);

	SAVE_SECTION(1);
	REG_MEMBER(_gpfile,gpFile);
	REG_MEMBER(_int,Sprite);
	REG_MEMBER(_int,dx);
	REG_MEMBER(_int,dy);
	REG_MEMBER(_int,Lx);
	REG_MEMBER(_int,Ly);
	REG_MEMBER(_int,ExtraBottomDy);

	SAVE_SECTION(4);
    REG_MEMBER(_ModelID,ModelID);
	REG_MEMBER(_float,Scale);
	REG_MEMBER(_float01,RotationSpeed);

	SAVE_SECTION(2);
	REG_MEMBER(_TextureID,TextureID);
	REG_MEMBER(_int,IconX);
	REG_MEMBER(_int,IconY);

	SAVE_SECTION(6);
	REG_MEMBER(_int,IconLx);
	REG_MEMBER(_int,IconLy);
	REG_MEMBER(_int,TopMargin);
	REG_MEMBER(_int,BottomMargin);
	REG_MEMBER(_int,LeftMargin);
	REG_MEMBER(_int,RightMargin);

	ENDSAVE;
	const char* GetThisElementView(const char*){
		char* s=GetGlobalBuffer();
		sprintf(s,"{I%s}%s",Name.pchar(),Name.pchar());
		return s;
	}	
	DWORD GetClassMask(){
		return IconType==0?1:IconType==1?2:4;
	}
};
class TextIcons:public BaseClass{
public:
	ClonesArray<OneTextIcon> Icons;
	OneTextIcon* get(char* s){
		for(int i=0;i<Icons.GetAmount();i++)if(Icons[i]->Name.pchar()){
			if(!strcmp(Icons[i]->Name.pchar(),s))return Icons[i];
		}
		return NULL;
	}
	SAVE(TextIcons);
	REG_LOADSAVE("Icons\\","*.IconList.xml");
	REG_CLASS(OneTextIcon);
	REG_AUTO(Icons);
	ENDSAVE;
};
class OneFontParam:public BaseClass{
public:
	OneFontParam(){
		YShift=0;
		Top=0;
		Bottom=0;
		gpFont=0;
		DefColor=0xFFFFFFFF;
		gpShadow=-1;
		ShadowDx=0;
		ShadowDy=0;
	}
	_str Name;
	int YShift;
	int Top;
	int Bottom;	
	short gpFont;
	DWORD DefColor;
	bool EnableShadow;
	short gpShadow;
	int ShadowDx;
	int ShadowDy;	
	SAVE(OneFontParam){
		REG_AUTO(Name);
		REG_MEMBER(_gpfile,gpFont);	
		REG_MEMBER(_DWORD,DefColor);
		REG_MEMBER(_int,YShift);
		REG_MEMBER(_int,Top);
		REG_MEMBER(_int,Bottom);
		REG_MEMBER(_bool,EnableShadow);
		SAVE_SECTION(2);
		REG_MEMBER(_gpfile,gpShadow);
		REG_MEMBER(_int,ShadowDx);
		REG_MEMBER(_int,ShadowDy);		
	}ENDSAVE;
	DWORD GetClassMask(){
		if(EnableShadow) return 2;
		return 1;
	}
};
class FontParam:public BaseClass{
public:
	ClassArray<OneFontParam> Fonts;    
	void Read(char* Name){
		reset_class(this);
		SafeReadFromFile(Name);		
	}
	OneFontParam* GetByGP(int IDX){
		for(int i=0;i<Fonts.GetAmount();i++){
			if(Fonts[i]->gpFont==IDX)return Fonts[i];
		}
		OneFontParam* OFP=new OneFontParam;
		OFP->Bottom=GPS.GetGPHeight(IDX,'W');
		OFP->Top=2;
		OFP->gpFont=IDX;
		OFP->DefColor=0xFFFFFFFF;
		OFP->Name="?";
		Fonts.Add(OFP);
		//saving
		xmlQuote xml;
		Fonts.Save(xml,&Fonts);
		xml.WriteToFile("Dialogs\\fonts2.xml");
		return OFP;
	}
	OneFontParam* GetByName(char* Name){
		for(int i=0;i<Fonts.GetAmount();i++)if(Fonts[i]->Name.pchar()){
			if(!strcmp(Fonts[i]->Name.pchar(),Name))return Fonts[i];
		}
		return NULL;
	}
	SAVE(FontParam){
		REG_CLASS(OneFontParam);
		REG_AUTO(Fonts);
	}ENDSAVE;
};
//commands list
//text...{command}text...
//{command}:
//{C XXXXXXXX} - draw colored
//{CR} FFB83B3F
//{CN} FF502515
//{CW} FFFFFFFF
//{CY} FFFFFF00
//{CG} FF60A05A
//{F} - set to default font
//{F???} - different fonts, ???-name of the font
//{P filename spridx dx dy lx ly}
//{G gpID sprID dx dy lx ly}
//{I???} - show icon, ???-picture name
//{R XXXXXXXX reference} - add ref string, XXXXXXXX - color
//{R XXXXXXXX $HINT HintName} - add hinted string,
//returns y-size of text
//////////////////////////////////////////////////////////////////////

#endif