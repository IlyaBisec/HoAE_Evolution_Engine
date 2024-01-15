class ExtraShootingElement:public UnitAbility{
public:
    SubSection					TurretParams;

	int							MinDistance;
	int							MaxDistance;	
	ClassRef<WeaponModificator> Weapon;	
	int							Damage;	

    Delimiter					D1;
	int							WholeObjectModel;
	int							ShootAnimation;	
	int							WholeShootTime;
	int							TimeOfShooting;
	int							TotalRechargeTime;
	int							BulletsAmount;
	int							CartridgeRechargeTime;

	Delimiter					D2;
	int							RotateAnimation;
	int							CentralShootAngle;
	int							ShootAngleDifference;
	int							RotationSpeed;
	bool						AnyDirectionAllowed;

	Delimiter					D3;
	int							TowerCenterX;
	int							TowerCenterY;
	int							TowerCenterZ;
    int							CannonRadius;

	SAVE(ExtraShootingElement);
		REG_PARENT(UnitAbility);

		REG_AUTO(TurretParams);

		REG_MEMBER(_int,MinDistance);
		REG_MEMBER(_int,MaxDistance);		
		REG_MEMBER(_int,Damage);	
		REG_AUTO(Weapon);		

		REG_AUTO(D1);
		REG_MEMBER(_ModelID,WholeObjectModel);
		REG_MEMBER(_ModelID,ShootAnimation);		
		REG_MEMBER(_int,WholeShootTime);
		REG_MEMBER(_int,TimeOfShooting);
		REG_MEMBER(_int,TotalRechargeTime);
		REG_MEMBER(_int,BulletsAmount);
		REG_MEMBER(_int,CartridgeRechargeTime);

		REG_AUTO(D2);
		REG_MEMBER(_ModelID,RotateAnimation);
		REG_MEMBER(_int,CentralShootAngle);
		REG_MEMBER(_int,ShootAngleDifference);
		REG_MEMBER(_int,RotationSpeed);
		REG_MEMBER(_bool,AnyDirectionAllowed);

		REG_AUTO(D3);
		REG_MEMBER(_int,TowerCenterX);
		REG_MEMBER(_int,TowerCenterY);
		REG_MEMBER(_int,TowerCenterZ);
		REG_MEMBER(_int,CannonRadius);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class ActiveExtraShootingElement:public ActiveUnitAbility{
public:
	int		EnemyID;
	int		EnemySN;
	int		LastShotTime;
	int		ShootAnimationStage;
	int		CurrentAngle;
	bool	CheckPossibilityToAttackEnemy(OneObject* OB,OneObject* EOB);
	SAVE(ActiveExtraShootingElement);
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,EnemyID);
		REG_MEMBER(_int,EnemySN);
		REG_MEMBER(_int,LastShotTime);
		REG_MEMBER(_int,ShootAnimationStage);
		REG_MEMBER(_int,CurrentAngle);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
	virtual void modifyExtraAnimation(int Basic, int& Current);
};
class _3D_shooter_simulator:public ExtraShootingElement{
public:    
    int							LiftAnimation;
    _str                        ShootDirectionNodeName;    
    int                         LiftSpeed;
	float						ShotImpulseForce;
	bool						HideSpot;
	//bool						UseRightClickToShot;
	//bool						UseCtrlToRotate;
    SAVE(_3D_shooter_simulator);
        REG_BASE(UnitAbility);
        REG_PARENT(ExtraShootingElement);
        REG_AUTO(D3);
        REG_MEMBER(_ModelID,LiftAnimation);
        REG_AUTO(ShootDirectionNodeName);
        REG_MEMBER(_int,LiftSpeed);        
		REG_MEMBER(_float,ShotImpulseForce);
		REG_MEMBER(_bool,HideSpot);
		//REG_MEMBER(_bool,UseRightClickToShot);
		//REG_MEMBER(_bool,UseCtrlToRotate);
    ENDSAVE;
    ActiveUnitAbility* GetNewActiveAbility();
};
class Active_3D_shooter_simulator: public ActiveUnitAbility{
public:
    Vector3D CurDestToDraw;	
	Vector3D CurDestToDraw2;
    int		 LastShotTime;
    int 	 ShootAnimationStage;
    int 	 CurrentAngle;
    int      CurrentLiftStage;
    Matrix4D LastDrawM4;	
    //desired params
    int      destRotation;
    int      destLift;
    bool     NeedShot;
	int Bullets;
	int CartridgeReadyTime;
	//int D
    SAVE(Active_3D_shooter_simulator);
        REG_MEMBER(_int,LastShotTime);
        REG_MEMBER(_int,ShootAnimationStage);
        REG_MEMBER(_int,CurrentAngle);
        REG_MEMBER(_int,CurrentLiftStage);
		REG_MEMBER(_int,Bullets);
		REG_MEMBER(_int,CartridgeReadyTime);
    ENDSAVE;
    virtual bool Process(OneObject* OB);
    virtual void modifyExtraAnimation(int Basic, int& Current);
};


int GetCurAzim(OneObject *pOB);
void SetCurAzim(OneObject *pOB, int Angle);
int GetCurElev(OneObject *pOB);
void SetCurElev(OneObject *pOB, int Angle);

int getDestRotation(OneObject* OB);
int getDestLift(OneObject* OB);
void setDestLift(OneObject* OB,int State);
void setDestRotation(OneObject* OB,int State);
void RequireShot(OneObject* OB,int WeapIdx);

