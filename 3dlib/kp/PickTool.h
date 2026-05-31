class PickTool:public BasicTool{
public:
    PickTool();
    bool                            m_Active;    
    static void  ModifyPoint        (float scale,ModPointInfo& pi,float R,float R1 );        
    
    static Vector4D Summ;
    static float W;

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
        return "PICK_TOOL_HINT";
    }
    virtual const char* GetMessage  (){
        return "PICK_TOOL";
    }
    virtual const char* GetWorkHint (){        
        return "PICK_TOOL_WORK_HINT";        
    }
    virtual IconInfo  GetIconInfo   ();
    virtual bool OnGlobalKey        (char Key);    
};