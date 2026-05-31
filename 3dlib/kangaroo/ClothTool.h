class ClothTool:public BasicTool{
public:
	ClothTool();
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
		return "CLOTH_TOOL_HINT";
	}
	virtual const char* GetMessage  (){
		return "CLOTH_TOOL";
	}
	virtual const char* GetWorkHint (){        
		return "CLOTH_TOOL_WORK_HINT";        
	}
	virtual IconInfo  GetIconInfo   ();
	virtual bool OnGlobalKey        (char Key);    
	virtual void CreateInterface    (BaseWidget* Where);
};