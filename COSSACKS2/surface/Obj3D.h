#include "..\WeaponSystem.h"
class SceneObject;
class BasicStateAction:public BaseClass{
public:
	virtual void Action(SceneObject* Current){};
	SAVE(BasicStateAction){
	}ENDSAVE;
};
class BaseSceneObject:public ReferableBaseClass{
public:
	BaseSceneObject(){
		ActivationTime=0;
	}
	ClassArray<BasicStateAction> ActionOnEndFrame;

	virtual void Render(SceneObject* ob){}
	virtual void CalcAB(){}
	virtual void DrawAB(DWORD Color){}
	virtual bool CheckPick(Ray3D& ray){return false;}

	AABoundBox  AB;
	Vector3D    Center;
	float		Radius;	
	int			ActivationTime;
	int			StateTimeLength;
	float	    StartAnimTime;
	float	    FinalAnimTime;
	float		AnimPlaySpeed;
	float		Stage;
	int			CurrStateTime;
	Matrix4D	Transform;
	Matrix4D	InvTransform;	

	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){		
		dest="State %$StateName$%";		
		return true;
	}
	
	SAVE(BaseSceneObject){
		REG_AUTO_EX(Name,StateName);
		REG_AUTO(ActionOnEndFrame);		
		
        INVISIBLE REG_MEMBER_EX(_float,Transform.e00,e00);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e01,e01);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e02,e02);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e03,e03);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e10,e10);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e11,e11);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e12,e12);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e13,e13);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e20,e20);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e21,e21);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e22,e22);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e23,e23);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e30,e30);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e31,e31);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e32,e32);
		INVISIBLE REG_MEMBER_EX(_float,Transform.e33,e33);
		
	}ENDSAVE;
};

class TransformThisObject:public BasicStateAction{
public:
	virtual void Action(SceneObject* Current);
	_str	NextStateName;

	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){		
		dest="Transform to %$NextStateName$%";			
		return true;
	}	

	SAVE(TransformThisObject){
		REG_PARENT(BasicStateAction);
        REG_AUTO(NextStateName);
	}ENDSAVE;
};
class TransformOtherObject:public BasicStateAction{
public:
	virtual void Action(SceneObject* Current);
	_str	ObjectName;
	_str	NextStateName;

	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){		
		dest="Transform %$ObjectName$% to %$NextStateName$%";			
		return true;
	}

	SAVE(TransformOtherObject){
		REG_PARENT(BasicStateAction);
		REG_AUTO(ObjectName);
		REG_AUTO(NextStateName);
	}ENDSAVE;
};
class BornWeaponOnObject : public BasicStateAction{
public:
	virtual void Action(SceneObject* Current);
    _str ObjectName;
	ClassRef<WeaponModificator> Weapon;

	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){		
		dest="Born weapon %$Weapon$% on %$ObjectName$%";			
		return true;
	}

	SAVE(BornWeaponOnObject){
		REG_PARENT(BasicStateAction);
		REG_AUTO(ObjectName);
		REG_AUTO(Weapon);
	}ENDSAVE;
};
class SceneObjectState:public BaseSceneObject{
public:
	__constructor(SceneObjectState);
	int			ModelID;
	int			AnimID;		
	DWORD		StartColor;
	DWORD		FinalColor;
	
	virtual void Render(SceneObject* ob);
	void		 CalcAB();
	void		 DrawAB(DWORD Color);
	bool		 CheckPick(Ray3D& ray);

	SAVE(SceneObjectState){        
		REG_MEMBER(_ModelID,ModelID);
		REG_ANM(AnimID,ModelID);
		REG_FSLIDER(StartAnimTime,0,1,0);
		REG_FSLIDER(FinalAnimTime,0,1,1);
		REG_MEMBER(_float,AnimPlaySpeed);
		REG_MEMBER(_color,StartColor);
		REG_MEMBER(_color,FinalColor);
		REG_PARENT(BaseSceneObject);
	}ENDSAVE;
};

class SceneObject : public ReferableBaseClass{
public:
	ClassArray<BaseSceneObject> States;
	int	 CurrStateIndex;    
	void DeleteObject();
	SAVE(SceneObject){		
		REG_AUTO(Name);
        REG_AUTO(States);
		REG_MEMBER(_int,CurrStateIndex);
		REG_METHOD(DeleteObject);
	}ENDSAVE;
};
class ObjectsList : public BaseClass{
public:	
	ClonesArray<SceneObject> Objects;	
	SAVE(ObjectsList){
		REG_AUTO(Objects);
	}ENDSAVE;
};
class SceneObjExtension : public GameExtension{
public:
	SceneObjExtension();

	static ObjectsList Objects;		
	static bool EditMode;
	static int LastPick; 
	static int CurrSelModel;
	int		 	CurrModel;
	bool		m_bActive;
	cVec3	 	m_Scale;
	float	 	m_ScaleMin, m_ScalingSpeed;
	int		 	m_CoordX, m_CoordY, m_CoordZ;
	Matrix4D 	m_T;
	
	Matrix4D CalcFullTransform();
	int BrowseForModel();
	virtual bool OnMapLoading(xmlQuote& xml);
	virtual bool OnMapSaving(xmlQuote& xml);
	virtual bool OnGameSaving(xmlQuote& xml){
		return OnMapSaving(xml);
	}
	virtual bool OnGameLoading(xmlQuote& xml){
		return OnMapLoading(xml);
	}
	void OnDrawOnMapAfterUnits();
	virtual bool OnMouseHandling(int mx, int my, bool &LeftPressed, bool &RightPressed,
		int MapCoordX, int MapCoordY, bool OverMiniMap);
	virtual void ProcessingGame();
	void DrawEditorInterface(){};
	virtual bool OnMouseWheel(int Delta);
	void ActivateEditor();
	void ClearActivityOfEditor();
	bool CheckActivityOfEditor();
	void OnUnloading();
	bool OnMapUnLoading();	
	static bool ActivateState(const char* ObjectName,const char* StateName,SceneObject* Curr=NULL);

	
	SAVE(SceneObjExtension){
		
	}ENDSAVE;
};