cVec3 _cVec3(Vector3D v){
    return cVec3(v.x,v.y,v.z);
}
Vector3D _Vector3D(cVec3 v){
    return Vector3D(v.x,v.y,v.z);
}

typedef comms::cVector<float,5> cVec5;

class CurveTool:public BasicTool{
public:
    bool                            m_Active;

	static int						CurveProfile;
	static float					CurveProfileLength;
    CurveTool();

    cList<cVec3>                    PointsArray;
	cList<cVec3>                    NormalsArray;
    cList<float>                    RadiusArray;
    cList<float>                    HeightArray;
	cSplineBezier3	                Points;
	cSplineTCB<cVec5>   NRH;
    float			                SplineLength;
	void			                CreateSpline();
    void			                AddPoint(cVec3 n,cVec3 p,float R,float H);
	void			                DelLast();
	void			                ClearAllPoints();
	void			                DrawSpline();
	cVec2			                TransformPointBySpline(cVec3 p,cVec2& ofsAlong,cVec2& ofsRadial,cVec3& Norm,float& dz);
	bool			                IsSplined();
    void                            ApplySplineToSurface();
	Vector3D						GetProfileShape(float T);

    virtual bool OnLMB_Up	        ();
    virtual bool OnKey              (char KeyCode);
    virtual bool IsActive           (){
        return m_Active;
    }
    virtual void Activate           (){
        m_Active=true;
    }
    virtual void DeActivate         (){
        m_Active=false;
    }
    virtual void Draw               ();
    virtual const char* GetHint     (){
        return "CURVE_TOOL_HINT";
    }
    virtual const char* GetMessage  (){
        return "CURVE_TOOL";
    }
    virtual const char* GetWorkHint (){
        if(PointsArray.Count()==0){
            return "CURVE_TOOL_HINT_ADD_POINT";
        }else{
            return "CURVE_TOOL_HINT_WORKHINT";
        }
    }
    virtual IconInfo  GetIconInfo   ();
    virtual void CreateInterface    (BaseWidget* Where){
		PenInterface::AddToolHeader("CURV_PROP");
		PictMenu* PM=new PictMenu;
		PM->Color=PenInterface::Color;
		PM->CreateDefault(Rct(0,PenInterface::GetToolHeight(),PenInterface::width,PenInterface::height),&CurveProfile,1);
		PenInterface::ToolPanel->SubWidgets.Add(PM);
		PM->AddPict(16,"CURVE_PROFILE_AS_IS");
		PM->AddPict(14,"CURVE_PROFILE_NARROW");
		PM->AddPict(15,"CURVE_PROFILE_SOFT");
		PenInterface::AddToolSlider(0.001,1,&CurveProfileLength,RedColor,"","CURV_PROP_HINT");
        PenInterface::AddToolHeader("STRIP_SHAPE");
        PenInterface::CreateStripMenu(Where);		
    };
};