//-----------------------------------------------------------------------------------------------------------------//
class BigUnit : public UnitAbility
{
public:
	BigUnit();
	int Radius;
	int Width;
	int LockRadiusIfStand;
	SAVE(BigUnit)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,Width);
		REG_MEMBER(_int,LockRadiusIfStand);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
//-----------------------------------------------------------------------------------------------------------------//
class ActiveBigUnit : public ActiveUnitAbility
{
public:
	ActiveBigUnit();
	SAVE(ActiveBehaviour)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
private:
	bool FirstStand:1;
};
//-----------------------------------------------------------------------------------------------------------------//
class AggressiveMove : public UnitAbility
{
public:
	AggressiveMove();
	int Radius;
	int MinDist;
	SAVE(AggressiveMove)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,MinDist);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
//-----------------------------------------------------------------------------------------------------------------//
class ActiveAggressiveMove : public ActiveUnitAbility
{
public:
	ActiveAggressiveMove();
	int m_baseX;
	int m_baseY;
	int m_dir;
	int m_tempX;
	int m_tempY;
	SAVE(ActiveAggressiveMove)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,m_baseX);
		REG_MEMBER(_int,m_baseY);
		REG_MEMBER(_int,m_dir);
		REG_MEMBER(_int,m_tempX);
		REG_MEMBER(_int,m_tempY);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
private:
	int m_lastMoveTime;
};
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
class AggressiveSplashFire : public UnitAbility
{
public:
	AggressiveSplashFire();
	int Radius;
	int MinDist;
	int MinUnits;
	SAVE(AggressiveSplashFire)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,MinDist);
		REG_MEMBER(_int,MinUnits);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
//-----------------------------------------------------------------------------------------------------------------//
class ActiveAggressiveSplashFire : public ActiveUnitAbility
{
public:
	ActiveAggressiveSplashFire();
	
	SAVE(ActiveAggressiveSplashFire)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
private:
	int m_lastAttackTime;
};
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//

