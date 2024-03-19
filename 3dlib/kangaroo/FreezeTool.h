class FreezeTool:public BasicTool{
public:
    bool                            m_Active; 
    static int                      FreezeMode;
    static float                    HeightScale;
    static float                    ColorDiff;
    static bool                     FeezeView;
    static bool                     InvFreeze;
    
    static int                      ColorFreezeMode;//0-none 1-by dark
    static float GetFreezeAmount    (ModPointInfo* mp);
    static void  ModifyPoint        (float scale,ModPointInfo& pi,float R,float R1 );
    static void  UnFreeze           (float scale,ModPointInfo& pi,float R,float R1 );    
    static void  Apply              ();
    static void  UnFreezeAll        ();
    static void  FreezeAll          ();
	static void  InvertFreeze       ();

    FreezeTool();


    virtual void Process            ();    
    virtual bool IsActive           (){
        return m_Active;
    }
    virtual void Activate           (){
        m_Active=true;
    }
    virtual void DeActivate         (){
        m_Active=false;
    }    
    virtual const char* GetHint     (){
        return "FREEZE_TOOL_HINT";
    }
    virtual const char* GetMessage  (){
        return "FREEZE_TOOL";
    }
    virtual const char* GetWorkHint (){        
        return "FREEZE_TOOL_WORK_HINT";        
    }
    virtual IconInfo  GetIconInfo   ();
    virtual bool OnGlobalKey        (char Key);
    virtual bool OnKey              (char Key);
    bool opFreeze                   ();
    bool opUnFreeze                 ();
    bool opToggleFreezeView         ();
	bool opInvertFreeze		        ();
    virtual void CreateInterface    (BaseWidget* Where){        
        PenInterface::AddToolHeader("FREEZE");
        int y0=PenInterface::GetToolHeight();
        float w=PenInterface::width;
        float h=PenInterface::height;

        PictMenu* PM=new PictMenu;
        PM->SetHint("FREEZE_MODE");
        PM->Color=PenInterface::ToolPanel->Color;
        PM->CreateDefault(Rct(0,y0,w,h),&FreezeMode,1);
        PM->AddPict(20,"FREEZE_BY_PEN");        
        PM->AddPict(22,"FREEZE_BOTTOM_POINTS");
        PM->AddPict(21,"FREEZE_TOP_POINTS");
        PM->AddPict(23,"FREEZE_BY_COLOR");
        PM->AddPict(24,"FREEZE_BY_INVERSE_COLOR");
        PenInterface::ToolPanel->SubWidgets.Add(PM);

        PenInterface::AddToolHeader("FR_H_DIFF");
        PenInterface::AddToolSlider(0,3000,&HeightScale,RedColor,"","FR_H_DIFF_HINT");
        PenInterface::AddToolHeader("FR_CLR_DIFF");
        PenInterface::AddToolSlider(0,255,&ColorDiff,GreenColor,"","FR_CLR_DIFF_HINT");
        PenInterface::AddToolHeader("FREEZE_OPS");
        BaseWidget* F1=PenInterface::AddToolButton(25,"FREEZE_CURRENT_CONDITION");
        F1->AddCallback(WM_LBUTTONUP,new wpcallback<FreezeTool>(this,opFreeze));
        BaseWidget* F2=PenInterface::AddToolButton(26,"UNFREEZE_ALL");
        F2->AddCallback(WM_LBUTTONUP,new wpcallback<FreezeTool>(this,opUnFreeze));
		BaseWidget* F3=PenInterface::AddToolButton(37,"INVERT_FREEZE");
		F3->AddCallback(WM_LBUTTONUP,new wpcallback<FreezeTool>(this,opInvertFreeze));
        BaseWidget* F4=PenInterface::AddToolButton(19,"TOGGLE_FREEZE_VIEW");
        F4->AddCallback(WM_LBUTTONUP,new wpcallback<FreezeTool>(this,opToggleFreezeView));
    };
};