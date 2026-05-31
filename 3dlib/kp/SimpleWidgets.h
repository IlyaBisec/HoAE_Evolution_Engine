class BaseWidget;

typedef bool fnAction(BaseWidget* Widget);
class BaseWidget;

class WBaseCallback : public BaseClass{
public:
    int Message;
    virtual bool call(){return false;};
};
template <class BaseT>
class wpcallback : public WBaseCallback{
    typedef bool (BaseT::*FnProcess0)();
    typedef bool (BaseT::*FnProcess1)(int Param1);
    typedef bool (BaseT::*FnProcess2)(int Param1,int Param2);
    typedef bool (BaseT::*FnProcess3)(int Param1,int Param2,int Param3);
	typedef bool (BaseT::*FnProcess4)(int Param1,int Param2,int Param3,int Param4);
	typedef bool (BaseT::*FnProcess5)(int Param1,int Param2,int Param3,int Param4,int Param5);
    
	int Type;
	union{
		FnProcess0 m_fnProcess0;
		FnProcess1 m_fnProcess1;
		FnProcess2 m_fnProcess2;
		FnProcess3 m_fnProcess3;
		FnProcess4 m_fnProcess4;
		FnProcess5 m_fnProcess5;
	};

    int _Param1;
    int _Param2;
    int _Param3;    
	int _Param4;    
	int _Param5;    

    BaseT* C;
public:
	wpcallback(){
		Type=0;
		C=NULL;
		m_fnProcess0=NULL;		
	}
    wpcallback(BaseT* Class,FnProcess0 Function){
		Type=1;
        C=Class;
        m_fnProcess0=Function;        
    }
    wpcallback(BaseT* Class,FnProcess1 Function,int Param1){
		Type=2;
        C=Class;        
        m_fnProcess1=Function;        
        _Param1=Param1;
    }
    wpcallback(BaseT* Class,FnProcess2 Function,int Param1,int Param2){
		Type=3;
        C=Class;
        m_fnProcess2=Function;        
        _Param1=Param1;
        _Param2=Param2;
    }
    wpcallback(BaseT* Class,FnProcess3 Function,int Param1,int Param2,int Param3){
		Type=4;
        C=Class;
        m_fnProcess3=Function;
        _Param1=Param1;
        _Param2=Param2;
        _Param3=Param3;
    }
	wpcallback(BaseT* Class,FnProcess4 Function,int Param1,int Param2,int Param3,int Param4){
		Type=5;
		C=Class;
		m_fnProcess4=Function;
		_Param1=Param1;
		_Param2=Param2;
		_Param3=Param3;
		_Param4=Param4;
	}
	wpcallback(BaseT* Class,FnProcess4 Function,int Param1,int Param2,int Param3,int Param4,int Param5){
		Type=6;
		C=Class;
		m_fnProcess4=Function;
		_Param1=Param1;
		_Param2=Param2;
		_Param3=Param3;
		_Param4=Param4;
		_Param5=Param5;
	}
    virtual bool call(){
		if(C){
			switch(Type){
				case 1:return (C->*m_fnProcess0)();
				case 2:return (C->*m_fnProcess1)(_Param1);
				case 3:return (C->*m_fnProcess2)(_Param1,_Param2);
				case 4:return (C->*m_fnProcess3)(_Param1,_Param2,_Param3);
				case 5:return (C->*m_fnProcess4)(_Param1,_Param2,_Param3,_Param4);
				case 6:return (C->*m_fnProcess5)(_Param1,_Param2,_Param3,_Param4,_Param5);
			}
		}
		return false;
    }            
};
class TextItem:public BaseClass{
public:
    _str ID;
	_str Text;
	SAVE(TextItem){
		REG_AUTO(ID);
		REG_AUTO(Text);
	}ENDSAVE;
};
class TextManager{
	static ClonesArray<TextItem> Text;
public:
	static void Init();
    static const char* GetText(const char* ID);
	static const char* GetID(const char* ID);
};
enum wAlignType{
    wa_None  ,
	wa_Top   ,
	wa_Bottom,
	wa_Left  ,
	wa_Right ,
	wa_Center
};
enum ParentAlignTypeX{
    pa_None,
	pa_RightToLeft,
	pa_LeftToLeft,
	pa_LeftToRight,
	pa_RightToRight
};
class BaseWidget : public BaseClass{        
public:
    __constructor(BaseWidget);
    virtual ~BaseWidget();

    ClassArray<BaseWidget> SubWidgets;
    bool    		Visible;
    bool    		Enabled;
    bool    		ClipSubWidgets;
    bool    		MouseOver;
    bool    		HaveFocus;
    Rct     		Rect;
    DWORD           Code;

	float   		lMargin;
	float   		rMargin;
	float   		tMargin;
	float   		bMargin;

	bool    		AutoSetWidth;
	bool			AutoSetHeight;

	wAlignType		NeibAttachType;//relative to current-level elements
    wAlignType		WhatAttachToParent;
	wAlignType		WhereAttachToParent;	

	float			lAttachMargin;
	float			rAttachMargin;
	float			tAttachMargin;
	float			bAttachMargin;

    Vector2D CurrentPos;
    DWORD   Color;
    _str    Hint;

	void	SetHint(const char* s);

    static  BaseWidget* CurrentWidget;	
    ClassArray<WBaseCallback> Callbacks;
    void    AddCallback(DWORD Message,WBaseCallback* cb);
    void    AddFirstCallback(DWORD Message,WBaseCallback* cb);
    bool    CallCallbacks(DWORD Message);  
	void    _draw();
	void    _setup_positions();
    bool    skip_lclick();
    bool    skip_rclick();    

    SAVE(BaseWidget){
        REG_MEMBER(_bool,Visible);
        REG_MEMBER(_bool,Enabled);
        REG_MEMBER(_bool,ClipSubWidgets);
        REG_MEMBER(_float,Rect.x);
        REG_MEMBER(_float,Rect.y);
        REG_MEMBER(_float,Rect.w);
        REG_MEMBER(_float,Rect.h);
        REG_MEMBER(_color,Color);
		REG_MEMBER(_bool,AutoSetWidth);
		REG_MEMBER(_bool,AutoSetHeight);
		REG_MEMBER(_float,lMargin);
		REG_MEMBER(_float,rMargin);
		REG_MEMBER(_float,tMargin);
		REG_MEMBER(_float,bMargin);
        REG_AUTO(SubWidgets);
        REG_AUTO(Hint);
    }ENDSAVE;
};
class Widgets{
public:
    static ClassArray<BaseWidget> ActiveWidgets;
    static void CreateWidgetsListUnderPoint(BaseWidget* W,int x,int y,DynArray<BaseWidget*>& List);
    static void CreateWidgetsList(BaseWidget* W,DynArray<BaseWidget*>& List);    

    static int  MouseX;
    static int  MouseY;

    static bool lPressed;
    static bool rPressed;
    static bool mPressed;
    static int  lastKey;
    static int  lastWheel;
	static _str CurrentHint;

    static void AddActiveWidget(BaseWidget* W);
    static void RemoveActiveWidget(BaseWidget* W);
    static void RemoveAll();
    static bool ProcessWindowsMessages(MSG& msg);
    static void DrawWidgets();
    static void ClearMouseOver(BaseWidget* bw=NULL);
    static void KillFocus(BaseWidget* bw=NULL);	
};

class PanelWidget : public BaseWidget{
public:

	bool draw();
	__constructor(PanelWidget);

	SAVE(PanelWidget){
	}ENDSAVE;
};

class DrawTexRect : public BaseWidget{
public:
    Rct uv;
    int Textures[4];
    int Shader;	

    bool draw();
	__constructor(DrawTexRect);
	DrawTexRect(const char* name,int idx,int nx,int ny);
	DrawTexRect(int tex,int idx,int nx,int ny);
	void SetIcon(int tex,int idx,int nx,int ny);

    SAVE(DrawTexRect){
        REG_PARENT(BaseWidget);
        REG_MEMBER(_float,uv.x);
        REG_MEMBER(_float,uv.y);
        REG_MEMBER(_float,uv.w);
        REG_MEMBER(_float,uv.h);
        REG_MEMBER(_int,Textures[0]);
        REG_MEMBER(_int,Textures[1]);
        REG_MEMBER(_int,Textures[2]);
        REG_MEMBER(_int,Textures[3]);
        REG_MEMBER(_int,Shader);
    }ENDSAVE;
};

class TextWidget : public BaseWidget{
public:
    _str Text;
    int  Align;
	int  Size;

	bool draw();
	__constructor(TextWidget);

    SAVE(TextWidget){
        REG_PARENT(BaseWidget);
        REG_AUTO(Text);
        REG_MEMBER(_int,Align);
    }ENDSAVE;
};
class FrameWidget : public BaseWidget{
public:
	int FrameTexture;
	Rct uv;
	bool draw();
	__constructor(FrameWidget);

	SAVE(FrameWidget){
		REG_PARENT(BaseWidget);
		REG_MEMBER(_int,FrameTexture);
		REG_MEMBER(_float,uv.x);
		REG_MEMBER(_float,uv.y);
		REG_MEMBER(_float,uv.w);
		REG_MEMBER(_float,uv.h);
	}ENDSAVE;
};
class SimpleSlider : public BaseWidget{
public:
    float* VariablePtr;
    float CurrPos;
    float MinValue;
    float MaxValue;
    
    SimpleSlider(DWORD Color,float* ptr,float minv,float maxv);
    bool draw();
    bool on_mmove();

    __constructor(SimpleSlider);

    SAVE(SimpleSlider){
        REG_PARENT(BaseWidget);
    }ENDSAVE;
};
class PictMenu : public BaseWidget{
	bool ShowSubmenu(int param);
	bool HideSubmenu(int);
	bool CloseOnClick();
	bool OnClick(int idx);
	bool OnDraw();
public:
	int Margin;
	int* VariablePtr;
	int CurrPos;
	int IconsTexture;
	int IcNx;
	int IcNy;
	DynArray<int> Icons;
	DynArray<const char*> Hints;
	int Align;
	FrameWidget* FW;
	DrawTexRect* BG;

	__constructor(PictMenu);

    void Create(Rct R,int Icon,int Nx,int Ny,int* VariablePtr,int Align);//Align : 0-left 1-right 2-top 3-bottom
	void CreateDefault(Rct R,int* VariablePtr,int Align);	

	void AddPict(int Icon,const char* Hint);

	SAVE(PictMenu){
        REG_PARENT(BaseWidget);
	}ENDSAVE;

};
class TextMenu : public BaseWidget{
    bool ShowSubmenu();
	bool HideSubmenu(int);
	bool CloseOnClick();
	bool OnClick(int idx);
	bool OnDraw();
    bool DrawSel(int ptr);
    void SetupPositions();
    bool DrawSep(int Ptr);
public:
    int Margin;
    int AddLineDist;    
    int Align;
    int  ElmAlign;
    FrameWidget* FW;

    __constructor(TextMenu);

    TextMenu(int align,int ealign);//0-left 1-right 2-top 3-bottom
    TextWidget* AddLine(const char* Text,const char* Hint);
    void DelLine(int idx);
    void AddSeparator();

    SAVE(TextMenu){
        REG_PARENT(BaseWidget);
	}ENDSAVE;
};
void DrawFrame(Rct& Rect,int Texture,Rct& uv,DWORD Color);