class PlaneTool:public BasicTool{
public:
    PlaneTool();
    bool                            m_Active;    
    static void  ModifyPoint        (float scale,ModPointInfo& pi,float R,float R1 );        
    static void  ModifyPoint1       (float scale,ModPointInfo& pi,float R,float R1 );        
    static void  ModifyPoint2       (float scale,ModPointInfo& pi,float R,float R1 );        

    static Vector3D avgN;
    static float avgS;    
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
        return "PLANE_TOOL_HINT";
    }
    virtual const char* GetMessage  (){
        return "PLANE_TOOL";
    }
    virtual const char* GetWorkHint (){        
        return "PLANE_TOOL_WORK_HINT";        
    }
    virtual IconInfo  GetIconInfo   ();
    virtual bool OnGlobalKey        (char Key);    
};