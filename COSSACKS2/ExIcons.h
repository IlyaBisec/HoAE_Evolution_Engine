#pragma once

class DIALOGS_API OneExIcon:public BaseClass{
public:
	virtual void Draw(int x,int y,int Lx,int Ly,byte NI){};
	AUTONEW(OneExIcon);
};
class DIALOGS_API OneSpriteExIcon:public OneExIcon{
public:
	short FileID;
	int StartSpriteID;
	int EndSpriteID;
	int Step;
	int dx;
	int dy;
	virtual void Draw(int x,int y,int Lx,int Ly,byte NI);
	SAVE(OneSpriteExIcon){
		REG_PARENT(OneExIcon);
		REG_MEMBER(_gpfile,FileID);
		REG_MEMBER(_int,StartSpriteID);
		REG_MEMBER(_int,EndSpriteID);
		REG_MEMBER(_int,Step);
		REG_MEMBER(_int,dx);
		REG_MEMBER(_int,dy);
	}ENDSAVE;
};
class DIALOGS_API OneSpriteExIconSet:public OneExIcon{
public:
	int Current;
	int CurrentStartTime;
	int CurrentEndTime;
	ClassArray<OneSpriteExIcon> Icons;
	int Val;
	DynArray<int> ProbValue;
	virtual void Draw(int x,int y,int Lx,int Ly,byte NI);
};
class DIALOGS_API OneSpriteExIcon3D:public OneExIcon{
public:
	int   ModelID;
	int   AnimID;
	float Scale;
	float RotateSpeed;
	float StartAngle;
	float zShift;
	int   Period;
	virtual void Draw(int x,int y,int Lx,int Ly,byte NI);
};
class DIALOGS_API ExtendedIcon:public BaseClass{
public:
	ClassArray<OneExIcon> Icons;
	void Draw(int x,int y,int Lx,int Ly,byte NI);
};
class DIALOGS_API OneBoneInfo:public BaseClass{
public:
	OneBoneInfo(){
		ModelID=-1;
		NodeID=-1;
	}
	int ModelID;
	int NodeID;
	_str ModelName;
	_str NodeName;
	Vector3D MainDir;
	int GetNode(){
		if(ModelID==-1){
			ModelID=IMM->GetModelID(ModelName.pchar());
		}
		if(ModelID!=-1){
			NodeID=IMM->GetNodeID(ModelID,NodeName.pchar());
		}
		return NodeID;
	}
	SAVE(OneBoneInfo);
	REG_AUTO(ModelName);
	REG_AUTO(NodeName);
	REG_MEMBER(_float,MainDir.x);
	REG_MEMBER(_float,MainDir.y);
	REG_MEMBER(_float,MainDir.z);
	ENDSAVE;
};
class DIALOGS_API OneEmotion:public BaseClass{
public:
	word File;
	int Sprite;
	_str EmotionName;
	SAVE(OneEmotion);
	REG_MEMBER(_gpfile,File);
	REG_SPRITE(Sprite,File);
	REG_AUTO(EmotionName);
	ENDSAVE;
};