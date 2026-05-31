#pragma once

class BasicPickInfo{
public:
    int         x;
    int         y;
    bool        OnObject;
    Vector3D    PickPos;
    float       Radius;
    Vector3D    AverageNormal();
    PolyMesh*   M;
};
struct IconInfo{
    IconInfo();
    const char* IconName;
    int         IconNx;
    int         IconNy;
    int         IconID;
};
class BasicTool:public BaseClass{    
public:
    _str                            Name;
    static  DynArray<BasicTool*>    Tools;
    static  BasicPickInfo           Pick;    
    static  bool                    SomeToolIsActive();
    static  void                    DeactivateAll();
    static  void                    ActivateTool(const char* Name);
    static  void InstallTool        (BasicTool* Tool,const char* Name);
    static  BasicTool* ActiveTool   ();
    static  BasicTool* GetTool      (const char* Name);
    static  int GetToolsAmount      (int Category);
    static  BasicTool* GetTool      (int Category,int Index);
    static  bool Locked;

    virtual bool OnLMB_Down         (){return false;}
    virtual bool OnLMB_Up           (){return false;}
    virtual bool OnRMB_Down         (){return false;}
    virtual bool OnRMB_Up           (){return false;}
    virtual bool OnMMB_Down         (){return false;}
    virtual bool OnMMB_Up           (){return false;}
    virtual bool OnMMove            (){return false;}
    virtual bool OnKey              (char KeyCode){return false;};
    virtual bool OnGlobalKey        (char KeyCode){return false;};
    virtual bool IsActive           (){return false;}
    virtual void Activate           (){};
    virtual void DeActivate         (){};
    virtual void Process            (){};
    virtual void Draw               (){};
    virtual const char* GetHint     (){return "";}
    virtual const char* GetWorkHint (){return "";}
    virtual const char* GetMessage  (){return "";}
    virtual IconInfo  GetIconInfo   ();
    virtual int GetMenuCategory     (){return 0;} //0 - surface tools 1 - selection tools
    virtual void CreateInterface    (BaseWidget* Where){};
};