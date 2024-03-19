class SculptTool:public BasicTool{
public:
    bool                            m_Active;

    SculptTool();


    virtual void Process            ();    
    virtual bool IsActive           (){
        return m_Active;
    }
    virtual void Activate           (){
        m_Active=true;
        SurfaceModificator::ClearSculptHash();
    }
    virtual void DeActivate         (){
        m_Active=false;
        SurfaceModificator::ClearSculptHash();
    }    
    virtual const char* GetHint     (){
        return "SCULPT_TOOL_HINT";
    }
    virtual const char* GetMessage  (){
        return "SCULPT_TOOL";
    }
    virtual const char* GetWorkHint (){        
        return "SCULPT_TOOL_WORK_HINT";        
    }
    virtual IconInfo  GetIconInfo   ();    
    virtual void CreateInterface    (BaseWidget* Where){
    }
    static void Modify              (Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M);
	static void ModifyTangent       (Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M);
    static void ModifyMove          (Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M);
    static void ModifySmooth        (Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M);
    static void ModifyCollapse      (Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M);
	static void ModifyPlanar		(Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M);
    void Apply();
};