class FillTool:public BasicTool{
public:
    FillTool();
    bool                            m_Active;        

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
        return "FILL_TOOL_HINT";
    }
    virtual const char* GetMessage  (){
        return "FILL_TOOL";
    }
    virtual const char* GetWorkHint (){        
        return "FILL_TOOL_WORK_HINT";        
    }
    virtual IconInfo  GetIconInfo   ();
    virtual bool OnGlobalKey        (char Key);    
	virtual void CreateInterface    (BaseWidget* Where);
};