class ActiveUnitAbility;
// ua_Button
class ua_Button: public BaseClass{
public:
	short FileID;
	int SpriteID;
	char* Hint;
	SAVE(ua_Button){
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_MEMBER(_textid,Hint);
	}ENDSAVE;
};
// ua_TargetSelector
class ua_TargetSelector: public ReferableBaseClass{
public:
	bool SelectArea;
	UserFriendlyNumericalReturner Radius; // радиус зоны поиска, пример - (Hero:Level+1)*100
	bool EnemyTarget;
	bool FriendlyTarget;
	byte BodyStyleTarget;
	bool SummonTarget;
	int MaxAmount;

	SAVE(ua_TargetSelector){
		REG_MEMBER(_bool,SelectArea);
		REG_AUTO(Radius);
		REG_MEMBER(_bool,EnemyTarget);
		REG_MEMBER(_bool,FriendlyTarget);
		REG_MEMBER(_BYTE,BodyStyleTarget);
		REG_MEMBER(_bool,SummonTarget);
		REG_MEMBER(_int,MaxAmount);
	}ENDSAVE;
};
// ua_TimeLine
class ua_Track: public ReferableBaseClass{
public:
	int StartTime;
	bool UnlimitDuration;
	int Duration;

	SAVE(ua_Track){
		REG_MEMBER(_int,StartTime);
		REG_MEMBER(_bool,UnlimitDuration);
		REG_MEMBER(_int,Duration);
	}ENDSAVE;

	virtual void Process(ActiveUnitAbility* AA, int Index) {}
	virtual void Start(ActiveUnitAbility* AA) {}
	virtual void Stop(ActiveUnitAbility* AA) {}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="StartTime=%$StartTime$%, ";
		if(UnlimitDuration){
			dest+="UnlimitDuration";
		}else{
			dest+="Duration=%$Duration$%";
		}
		return true;
	};
};
class ua_TimeLine: public ReferableBaseClass{
public:
	int Duration;
	ClassArray<ua_Track> Tracks;
	SAVE(ua_TimeLine){
		REG_MEMBER(_int,Duration);
		REG_AUTO(Tracks);
	}ENDSAVE;
};

class uat_UnitAnimation: public ua_Track{
public:
	DWORD Animatoin;
	SAVE(uat_UnitAnimation){
		REG_PARENT(ua_Track);
		REG_ENUM_EX(_index,Animatoin,MON_ANIMATIONS,AnimationCode);
	}ENDSAVE;
};
class uat_VisualEffect: public ua_Track{
private:
	DynArray<byte> RandDir;
public:
	int Place;	// Caster, Area center, Target units
	int ResetPeriod;
	
	bool ScaleToRadius;
	bool AllowFormFactor;
	bool RandomDirection;

	int PauseFrame;
	//int PauseDuration;
    UserFriendlyNumericalReturner PauseDuration;

	int Nation;
	
	bool UseSelType;
	int SelType;

	NewAnimation Effect;

	int CycledSound;

	SAVE(uat_VisualEffect){
		REG_PARENT(ua_Track);
		REG_ENUM(_index,Place,VisEffPlace);
		REG_MEMBER(_int,ResetPeriod);
		REG_MEMBER(_bool,ScaleToRadius);
		REG_MEMBER(_bool,AllowFormFactor);
		REG_MEMBER(_bool,RandomDirection);
		REG_MEMBER(_int,PauseFrame);
		//REG_MEMBER(_int,PauseDuration);
        REG_AUTO(PauseDuration);
		REG_MEMBER(_int,Nation);
		REG_MEMBER(_bool,UseSelType);
		REG_ENUM(_index,SelType,TEXTURE_CURSOR_TYPES);
		REG_AUTO(Effect);
		REG_ENUM(_index,CycledSound,ALL_SOUNDS);
	}ENDSAVE;

	uat_VisualEffect();
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){		
		static _str par;
		ua_Track::GetClassPresentationMask(par,DataPtr,ExtraPtr,options,Flags);
		dest="Effect: ";
		dest+=par;
		dest+=", Place=%$Place$%, %$Effect,graphparam$%";
		if(ResetPeriod){
			dest+=", ResetPeriod=%$ResetPeriod$%";
		}
		return true;
	};

	bool CheckDraw(ActiveUnitAbility* AA);
	virtual void Process(ActiveUnitAbility* AA, int Index);
};
class VisualEffectArray: public ClonesArray<uat_VisualEffect>{
public:
	SAVE(VisualEffectArray){
		REG_LOADSAVE("UnitsAbility\\","*.VisualEffectArray.xml");
	}ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
class uat_Modificator: public ua_Track{
public:
	ua_Button Button;
	//ua_TargetSelector TargetSelector;
	bool Aura;
	UnitModificatorArray Modificators;
	// CreateObject
	// TransformObject
	SAVE(uat_Modificator){
		REG_PARENT(ua_Track);
		REG_AUTO(Button);
		//REG_AUTO(TargetSelector);
		REG_MEMBER(_bool,Aura);
		REG_AUTO(Modificators);
	}ENDSAVE;

	virtual void Start(ActiveUnitAbility* AA);
	virtual void Stop(ActiveUnitAbility* AA);
};
// ua_Event : импульс, рождение (импульс 0), удаление
class ua_Event: public BaseClass{
public:		
	SAVE(ua_Event){
	}ENDSAVE;
	virtual bool Check(ActiveUnitAbility* AA){ return false; }
};
class uae_LeftClick: public ua_Event{
public:		
	SAVE(uae_LeftClick){
		REG_PARENT(ua_Event);
	}ENDSAVE;
	virtual bool Check(ActiveUnitAbility* AA);
};
class uae_RightClick: public ua_Event{
public:		
	SAVE(uae_RightClick){
		REG_PARENT(ua_Event);
	}ENDSAVE;
	virtual bool Check(ActiveUnitAbility* AA);
};
class uae_TimelineIsDone: public ua_Event{
public:		
	SAVE(uae_TimelineIsDone){
		REG_PARENT(ua_Event);
	}ENDSAVE;
	virtual bool Check(ActiveUnitAbility* AA);
};
// ua_Operation : drag - схватить, drop - бросить
class ua_Operation: public BaseClass{
public:		
	SAVE(ua_Operation){
	}ENDSAVE;
	virtual void Perform(ActiveUnitAbility* AA){}
};
class uao_ChangeState: public ua_Operation{
public:
	int NewState;
	SAVE(uao_ChangeState){
		REG_PARENT(ua_Operation);
		REG_MEMBER(_int,NewState);
	}ENDSAVE;
	virtual void Perform(ActiveUnitAbility* AA);
	
};
class uao_Disappear: public ua_Operation{
public:		
	SAVE(uao_Disappear){
		REG_PARENT(ua_Operation);
	}ENDSAVE;
	virtual void Perform(ActiveUnitAbility* AA);
};
class uao_ObjectDie: public ua_Operation{
public:		
	SAVE(uao_ObjectDie){
		REG_PARENT(ua_Operation);
	}ENDSAVE;
};


// ua_Impulse
class ua_Impulse: public BaseClass{
public:
	ClassPtr<ua_Event> Event;
	ClassArray<ua_Operation> Operations;
	SAVE(ua_Impulse){
		REG_AUTO(Event);
		REG_AUTO(Operations);
	}ENDSAVE;
};
// ua_State
class ua_State: public BaseClass{
public:
	// ChangeStateCondition
	int ManaCost;
	int CoolDown;
	bool Stun;
	bool Silence;
	bool SkillSilence;
	bool Death;
	// статическое
	//ClassRef<ua_Effect> eCastHit;
	//ClassRef<ua_Effect> eTargHit;
	// динамическое
	ua_Button Button;
	int InterfaceSlot;	// clothe - unclothe, ...
	ua_TargetSelector TargetSelector;
	ClonesArray<ua_Impulse> Impulses;
	ua_TimeLine TimeLine;
	ua_TimeLine TimeLineTarget;
	//
	SAVE(ua_State){
		REG_AUTO(Button);
		REG_AUTO(TargetSelector);
		REG_AUTO(Impulses);
		REG_AUTO(TimeLine);
	}ENDSAVE;
};
